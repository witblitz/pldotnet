
#include "pldotnet.h"

extern PLDotNetFunction *pldotnet_function_list;
extern PLDotNetFunction *pldotnet_function_list_end;

void pldotnet_print_all_functions()
{
	PLDotNetFunction *f = pldotnet_function_list;

	elog(DEBUG1, "Function List Start Pointer: [%p]", f);
	
	char *s = (char *)malloc(1024);
	s[0] = '\0';

	char *paramtypelist = (char *)malloc(1024);
	char *paramoidlist = (char *)malloc(1024);
	char *buf = (char *)malloc(10);

	while (f)
	{
		paramtypelist[0] = '\0';
		int i;
		for (i = 0; i < f->nparam; i++)
		{
			snprintf(buf, 10, "%u,", f->paramtypes[i]);
			strcat(paramtypelist, buf);
		}

		snprintf(s, 1024, "Name: %s; FQN: %s; Params: [%s]", f->name, f->fqn, paramtypelist);
		elog(DEBUG1, "Function info: [%s]", s);
		f = f->next;
	}

	free(paramtypelist); free(paramoidlist); free(buf);
}

PLDotNetFunction *pldotnet_add_function()
{
	PLDotNetFunction *newfunc = (PLDotNetFunction *)malloc(sizeof (PLDotNetFunction));

	newfunc->name = NULL;
	newfunc->fqn = NULL;
	newfunc->source = NULL;
	newfunc->next = NULL;
	newfunc->oid = -1;
	newfunc->nparam = 0;
	newfunc->is_internal = false;

	if (!pldotnet_function_list)
	{
		pldotnet_function_list = newfunc;
		pldotnet_function_list_end = pldotnet_function_list;
	}
	else
	{
		pldotnet_function_list_end->next = newfunc;
		pldotnet_function_list_end = newfunc;
	}
	return newfunc;
}

static inline char *_pldn_build_uint_list(unsigned int *p, int n)
{
	char *r = (char *)palloc((n * 12) + 1);
	char *s = (char *)palloc(10);
	int i;
	r[0] = '\0';
	for (i = 0; i < n; i++)
	{
		snprintf(s, 10, "%u, ", p[i]);
		strcat(r, s);
	}
	return r;
}

//if nparam < 0 only the name will be checked
PLDotNetFunction *pldotnet_find_function_by_fqn(char *fqn, unsigned int *paramtypes, int nparam)
{
	PLDotNetFunction *f = pldotnet_function_list;

	if (paramtypes)
		elog(DEBUG1, "Searching for function [%s] with %d parameters [%s]", fqn, nparam, _pldn_build_uint_list(paramtypes, nparam));
	else
		elog(DEBUG1, "Searching for function [%s]", fqn);

	while (f)
	{
		if (strcmp(f->fqn, fqn) == 0 && (f->nparam == nparam || nparam < 0))
		{
			if (nparam > 0)
			{
				if (bcmp(f->paramtypes, paramtypes, nparam * sizeof(unsigned int)) == 0)
					return f;
			}
			else
			{
				return f;
			}
		}
		f = f->next;
	}
	return NULL;
}


PLDotNetFunction *pldotnet_find_function_by_oid(Oid oid)
{
	PLDotNetFunction *f = pldotnet_function_list;

	while (f)
	{
		if (f->oid == oid)
			return f;
		f = f->next;
	}
	return NULL;
}



