#ifndef _PLDOTNET_UTILS_H
#define _PLDOTNET_UTILS_H

#include <postgres.h>
#include <executor/spi.h>
#include <commands/trigger.h>
#include <fmgr.h>
#include <access/heapam.h>
#include <utils/syscache.h>
#include <utils/builtins.h>
#include <catalog/pg_proc.h>
#include <catalog/pg_type.h>
#include <funcapi.h>
#include <access/htup_details.h>


#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/tokentype.h>

void plmono_lookup_pg_function(Oid fn_oid, Form_pg_proc *p_procStruct, char **psource, Oid **p_argtypes, char ***p_argnames, char **p_argmodes, int *p_argcount);

#endif
