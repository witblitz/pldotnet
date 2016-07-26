#ifndef _PLDOTNET_PLDOTNET_H
#define _PLDOTNET_PLDOTNET_H

#include <postgres.h>
#include <funcapi.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/tokentype.h>


struct _PLDotNetFunction 
{
	char *name;
	char *fqn; //namespace.class.function

	int nparam; //param count
	unsigned int *paramtypes; //list of param types (values in MonoTypeEnum)
	unsigned int returntype;

	//the postgres stuff
	Oid oid; //function Oid
	char *source;
	Oid *paramoids; //list of param types (Oid values)

	Oid resultOid;
	TypeFuncClass resultType; //result type (TYPEFUNC_SCALAR, TYPEFUNC_COMPOSITE, TYPEFUNC_RECORD, TYPEFUNC_OTHER)
	TupleDesc resultTupleDesc; //result tuple description

	//the mono stuff
	MonoClass *klass;
	MonoMethod *method;

	bool is_internal; //is internal function?
	int internal_ident; //

	void *next;
};

typedef struct _PLDotNetFunction PLDotNetFunction;


#endif
