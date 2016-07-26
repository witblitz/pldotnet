#ifndef __PLDOTNET_TYPECONV_H_
#define __PLDOTNET_TYPECONV_H_

#include <postgres.h>
#include <catalog/pg_type.h>
#include <utils/builtins.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/tokentype.h>
#include <mono/metadata/object.h>

inline unsigned int convert_pgtype_to_monotype(Oid oid);
unsigned int *convert_pgtypes_to_argtypes(const Oid *pgtypelist, int len);
void *convert_datum_to_value(Datum val, Oid oid);
Datum convert_value_to_datum(void *, Oid);

#endif
