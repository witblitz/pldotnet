
#include <postgres.h>
#include <catalog/pg_type.h>
#include <utils/builtins.h>

#include "internal.h"
#include "functionlist.h"
#include "mono_functions.h"
#include "typeconv.h"

int pldotnet_internal_elog (int level, const char *str);

void pldotnet_register_internal(const char *fqn, int ident, const Oid *paramoids, int nparam)
{
	PLDotNetFunction *func = pldotnet_add_function();

	func->is_internal = true;
	func->internal_ident = ident;
	func->nparam = 1;
	func->paramtypes = convert_pgtypes_to_argtypes(paramoids, nparam);
	func->fqn = strdup(fqn);
	func->name = strdup(fqn);
}

static Oid *_pldn_build1(unsigned int p1)
{
	unsigned int *r = (unsigned int *)malloc(1 * sizeof(unsigned int));
	r[0] = p1;
	return r;
}
static Oid *_pldn_build2(unsigned int p1, unsigned int p2)
{
	unsigned int *r = (unsigned int *)malloc(2 * sizeof(unsigned int));
	r[0] = p1;
	r[1] = p2;
	return r;
}


void pldotnet_register_internals()
{
	pldotnet_register_internal("PLMono._internal:load_dll", 1, _pldn_build1(TEXTOID), 1);

	pldotnet_register_internal("PLMono._internal:raise_elog", 2, _pldn_build2(INT4OID, TEXTOID), 2);
	mono_add_internal_call("pldotnet::_internal_elog", pldotnet_internal_elog);
}

Datum pldotnet_call_internal (int func_ident, FunctionCallInfo fcinfo)
{
	elog(DEBUG1, "Looking up internal function with ID %d", func_ident);
	switch(func_ident)
	{
		case 1:
			return Int32GetDatum(pldotnet_internal_load_dll(TextDatumGetCString(fcinfo->arg[0])));
		case 2:
			pldotnet_internal_elog(DatumGetInt32(fcinfo->arg[0]), TextDatumGetCString(fcinfo->arg[1]));
			return Int32GetDatum(0);
		default:
			elog(ERROR, "Unknown internal function identifier %d", func_ident);
	}
	return 0;
}

int pldotnet_internal_load_dll (const char *name)
{
	pldotnet_load_assembly(name);
	return 0;
}

int pldotnet_internal_elog (int level, const char *str)
{
	elog(level, str);
	return 0;
}


