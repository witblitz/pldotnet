#ifndef _PLDOTNET_MONOFUNCTIONS_H
#define _PLDOTNET_MONOFUNCTIONS_H

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/tokentype.h>

#include "pldotnet.h"

void pldotnet_mono_init();
Datum pldotnet_call_function(PLDotNetFunction *func, void **params);
void pldotnet_load_assembly (const char *name);

#endif
