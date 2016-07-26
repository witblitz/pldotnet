#ifndef __PLDOTNET_FUNCTIONLIST_H_
#define __PLDOTNET_FUNCTIONLIST_H_

#include "pldotnet.h"

PLDotNetFunction *pldotnet_add_function();
PLDotNetFunction *pldotnet_find_function_by_fqn(char *fqn, unsigned int *paramtypes, int nparam);
PLDotNetFunction *pldotnet_find_function_by_oid(Oid);

void pldotnet_print_all_functions();

#endif
