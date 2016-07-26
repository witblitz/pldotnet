#ifndef __PLDOTNET_INTERNAL_H_
#define __PLDOTNET_INTERNAL_H_

void pldotnet_register_internals();
void pldotnet_register_internal(const char *fqn, int ident, const Oid *param_oids, int nparam);

Datum pldotnet_call_internal (int func_ident, FunctionCallInfo fcinfo);
int pldotnet_internal_load_dll (const char *name);

#endif
