
#include <postgres.h>
#include <executor/spi.h>
#include <commands/trigger.h>
#include <fmgr.h>
#include <access/heapam.h>
#include <utils/syscache.h>
#include <utils/guc.h>
#include <utils/builtins.h>
#include <catalog/pg_proc.h>
#include <catalog/pg_type.h>
#include <funcapi.h>

#include <string.h>

#include "mono_functions.h"
#include "pldotnet.h"
#include "utils.h"
#include "functionlist.h"
#include "typeconv.h"
#include "internal.h"


#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

static bool pldotnet_assembly_list_check_hook(char **newvalue, void **extra, GucSource source);

PLDotNetFunction *pldotnet_function_list = NULL;
PLDotNetFunction *pldotnet_function_list_end = NULL;
PLDotNetFunction *pldotnet_function_run_code = NULL;

char *pldotnet_assembly_list = NULL;
char *pldotnet_assembly_dirs = NULL;

static bool pldotnet_assembly_list_check_hook(char **newvalue, void **extra, GucSource source)
{
	return true;
}


void _PG_init(void)
{
	static bool inited = false;

	if (inited)
		return;

	DefineCustomStringVariable("pldotnet.assembly_list",
		gettext_noop("List of assemblies to load (full paths)"),
		NULL,
		&pldotnet_assembly_list,
		"",
		PGC_USERSET, GUC_LIST_INPUT,
		pldotnet_assembly_list_check_hook,
		NULL,
		NULL);
	
	pldotnet_mono_init();
	pldotnet_register_internals();

	inited = true;
}

PG_FUNCTION_INFO_V1(pldotnet_call_handler);

Datum pldotnet_call_handler(PG_FUNCTION_ARGS)
{
	Datum retval;
	char *proc_source;
	PLDotNetFunction *func = NULL;

	Form_pg_proc procStruct = NULL;
	Oid *argtypes = NULL;
	char **argnames = NULL;
	char *argmodes = NULL;
	int argcount = 0;
	int i;

	unsigned int *mono_argtypes;


	if (CALLED_AS_TRIGGER(fcinfo))
	{
		/*
		* Called as a trigger procedure
		*/
		//TriggerData *trigdata = (TriggerData *) fcinfo->context;

		retval = Int32GetDatum(10);
	}
	else
	{
		/*
		* Called as a function
		*/
		
		elog(DEBUG1, "STARTING NOW");
		//pldotnet_print_all_functions();

		elog(DEBUG1, "Calling function OID: %u", fcinfo->flinfo->fn_oid);
		
		//pldotnet_print_all_functions();

		// see if we have this Oid in cache

		func = pldotnet_find_function_by_oid(fcinfo->flinfo->fn_oid);

		if (func == NULL)
		{
			elog(DEBUG1, "Function not found by OID");

			plmono_lookup_pg_function(fcinfo->flinfo->fn_oid, &procStruct, &proc_source, &argtypes, &argnames, &argmodes, &argcount);
				
			//pldotnet_print_all_functions();
		
			elog(DEBUG1, "Source: [%s]", proc_source);
			elog(DEBUG1, "Number of arguments: [%d]", argcount);

			mono_argtypes = convert_pgtypes_to_argtypes(argtypes, argcount);

			func = pldotnet_find_function_by_fqn(proc_source, mono_argtypes, argcount);

			if (func == NULL)
			{
				elog(DEBUG1, "Could not find function");
			}
			else
			{
				//pldotnet_print_all_functions();
				elog(DEBUG1, "Found function");

				//Store the OID for faster lookups next time it is called
				func->oid = fcinfo->flinfo->fn_oid;
				func->paramoids = argtypes;

				func->resultType = get_call_result_type(fcinfo, NULL, &(func->resultTupleDesc));
				func->resultOid = procStruct->prorettype;

				elog(DEBUG1, "resultOid: %u", func->resultOid);
			}
		}

		if (func != NULL)
		{
			if (func->is_internal)
			{
				return pldotnet_call_internal(func->internal_ident, fcinfo);
			}
			else
			{
				void **calling_args = NULL;

				if (func->nparam > 0)
				{
					calling_args = (void **)palloc(func->nparam * sizeof(void *));
					for (i = 0; i < func->nparam; i++)
					{
						calling_args[i] = convert_datum_to_value(fcinfo->arg[i], func->paramoids[i]);
					}
				}

				elog(DEBUG1, "Ready to call function with %d arguments", func->nparam);
				retval = pldotnet_call_function(func, calling_args);
			}
		}
		else
		{
			void **calling_args = NULL;

			calling_args = (void **)palloc(1 * sizeof(void *));
			calling_args[0] = mono_string_new(mono_domain_get(), proc_source);

			elog(DEBUG1, "Ready to call code [%s]", proc_source);

			retval = pldotnet_call_function(pldotnet_function_run_code, calling_args);
			
			elog(ERROR, "DONE");
		}

	}


	return retval;
}


