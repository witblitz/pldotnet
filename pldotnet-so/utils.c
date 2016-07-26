
#include "utils.h"

extern MonoDomain *domain;


/*
 * plmono_lookup_pg_function
 *
 *     Get information about function by its Oid
 *     This function was copied from https://github.com/omelnyk/plmono
 */
void
plmono_lookup_pg_function(Oid fn_oid, Form_pg_proc *p_procStruct, char **psource, Oid **p_argtypes, char ***p_argnames, char **p_argmodes, int *p_argcount)
{
	HeapTuple procTup;
	Datum sourceDatum;
	bool isnull;
	int nargs;

	procTup = SearchSysCache(PROCOID, ObjectIdGetDatum(fn_oid), 0, 0, 0);
	if (!HeapTupleIsValid(procTup))
		elog(ERROR, "Cache lookup failed for function %u", fn_oid);

	/*
	if (!p_argtypes)
		if (!(p_argtypes = palloc(sizeof(Oid**))))
			elog(ERROR, "Not enough memory");

	if (!p_argnames)
		if (!(p_argnames = palloc(sizeof(char**))))
			elog(ERROR, "Not enough memory");

	if (!p_argmodes)
		if (!(p_argmodes = palloc(sizeof(char**))))
			elog(ERROR, "Not enough memory");
	*/

	if (p_procStruct)
		*p_procStruct = (Form_pg_proc) GETSTRUCT(procTup);

	nargs = get_func_arg_info(procTup, p_argtypes, p_argnames, p_argmodes);

	if(p_argcount)
		*p_argcount = nargs;

	sourceDatum = SysCacheGetAttr(PROCOID, procTup, Anum_pg_proc_prosrc, &isnull);
	if (isnull)
		elog(ERROR, "'AS' clause of Mono function cannot be NULL'");

	*psource = pstrdup(DatumGetCString(DirectFunctionCall1(textout, sourceDatum)));
	
  	ReleaseSysCache(procTup);
}

