
#include <postgres.h>
#include <glib.h>
#include "mono_functions.h"
#include "pldotnet.h"
#include "functionlist.h"
#include "typeconv.h"
#include "_pldotnetcore.h"

MonoDomain *domain = NULL;

extern PLDotNetFunction *pldotnet_function_list;
extern char *pldotnet_assembly_list;
extern PLDotNetFunction *pldotnet_function_run_code;

void pldotnet_mono_init()
{
	if (!domain)
	{
		elog(DEBUG1, "pldotnet_mono_init()");
		domain = mono_jit_init ("pldotnet");

		pldotnet_load_assembly_from_data ("__pldotnetcore", pldotnetcore_dll, pldotnetcore_dll_len);
		pldotnet_function_run_code = pldotnet_find_function_by_fqn("PLDotNet.code_executor:run", NULL, -1);
		pldotnet_function_run_code->resultType = TYPEFUNC_OTHER;
	}
	else
	{
	}
}

void pldotnet_load_assemblies()
{
}

//Populate pldotnet_function_list with all functions found in assembly
void pldotnet_load_assembly (const char *name)
{
	MonoAssembly *assembly;
	MonoImage *image;

	elog(DEBUG1, "Loading %s", name);

	assembly = mono_domain_assembly_open (domain, name);

	if (!assembly)
	{
		elog(ERROR, "Error loading assembly %s\n", name);
	}

	image = mono_assembly_get_image(assembly);
	
	pldotnet_load_functions_from_image(name, image);

	elog(DEBUG1, "Done loading assembly");

}

//Populate pldotnet_function_list with all functions found in assembly
int pldotnet_load_assembly_from_data (const char *name, const char *data, unsigned int data_len)
{
	MonoImage *image;
	MonoImageOpenStatus status;
	MonoAssembly *assembly;

	elog(DEBUG1, "Loading %s from data", name);

	image = mono_image_open_from_data_with_name ((char *)data, data_len, true, &status, false, name);
	if (!image)
	{
		elog(ERROR, "Failed to load data %d", status);
	}

	assembly = mono_assembly_load_from_full(image, name, &status, false);

	pldotnet_load_functions_from_image(name, image);

	elog(DEBUG1, "Done loading %s", name);

}

//
int pldotnet_load_functions_from_image(const char *name, MonoImage *image)
{
	MonoClass *klass;
	gpointer iter;
	gpointer param_iter;
	MonoMethod *method;
	MonoMethodSignature *sig;
	MonoType *ptype;
	MonoImageOpenStatus status;
	//MonoType *rettype;

	const char *namespace;
	const char *classname;

	PLDotNetFunction *func;

	int i;

	int rows = mono_image_get_table_rows (image, MONO_TABLE_TYPEDEF);

	/* loop through classes. we start the count from 1 because we skip the special type <Module> */
	for (i = 1; i < rows; ++i) 
	{
		klass = mono_class_get (image, (i + 1) | MONO_TOKEN_TYPE_DEF);
		classname = mono_class_get_name (klass);
		namespace = mono_class_get_namespace (klass);
		
		elog(DEBUG1, "Class name: %s.%s", namespace, classname);

		iter = NULL;
		while ((method = mono_class_get_methods(klass, &iter)))
		{
			elog(DEBUG1, "Class Method: %s", mono_method_get_name(method));

			func = pldotnet_add_function();

			func->klass = klass;
			func->method = method;
			func->name = strdup(mono_method_get_name(method));
			func->fqn = (char *)malloc(strlen(func->name) + strlen(classname) + strlen(namespace) + 4);
			sprintf(func->fqn, "%s.%s:%s", namespace, classname, func->name);

			elog(DEBUG1, "Method FQN: %s (%p)", func->fqn, func);
			
			sig = mono_method_signature(method);
			
			func->returntype = (unsigned int)mono_type_get_type(mono_signature_get_return_type(sig));
			func->nparam = mono_signature_get_param_count(sig);
			func->paramtypes = (unsigned int *)malloc(func->nparam * sizeof(unsigned int));

			int j = 0;

			param_iter = NULL;
			while ((ptype = mono_signature_get_params(sig, &param_iter)))
			{
				char *pname = mono_type_get_name(ptype);
				unsigned int ptypetype = (unsigned int)mono_type_get_type(ptype);

				func->paramtypes[j] = ptypetype;

				elog(DEBUG1, "Param %d [%s] Type [%u][%u]", j, pname, ptypetype, func->paramtypes[j]);

				j++;
			}
		}
	}
	
	elog(DEBUG1, "Done loading functions");

}


Datum pldotnet_call_function(PLDotNetFunction *func, void **params)
{
	MonoObject *result;
	Datum ret;
	MonoObject *exc = (MonoObject *)malloc(sizeof (MonoObject)); //exception
	//char *str;

	elog(DEBUG1, "Invoking %p", func);

	result = mono_runtime_invoke(func->method, NULL, params, &exc);

	if (exc)
	{
		elog(ERROR, "Exception");
		return ret;
	}
	
	elog(DEBUG1, "Done. Got result %p", result);

	if (func->resultType == TYPEFUNC_SCALAR)
	{
		switch (func->returntype)
		{
			case MONO_TYPE_I4:
			case MONO_TYPE_U4:
			case MONO_TYPE_PTR:
			case MONO_TYPE_I1:
			case MONO_TYPE_U1:
			case MONO_TYPE_BOOLEAN:
			case MONO_TYPE_I2:
			case MONO_TYPE_U2:
			case MONO_TYPE_CHAR:
			case MONO_TYPE_I8:
			case MONO_TYPE_U8:
			case MONO_TYPE_R4:
			case MONO_TYPE_R8:
				ret = convert_value_to_datum(mono_object_unbox(result), func->resultOid);
				break;
			case MONO_TYPE_STRING:
				ret = CStringGetTextDatum(mono_string_to_utf8((MonoString*) result));
				break;
			default:
				elog(ERROR, "I do not know what to do with mono return type %u\n", func->returntype);
		}

	}
	else
	{
		elog(ERROR, "Can only return a SCALAR");
	}
	return ret;
}

