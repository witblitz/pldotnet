
#include <postgres.h>
#include <glib.h>
#include "mono_functions.h"
#include "pldotnet.h"
#include "functionlist.h"
#include "typeconv.h"

MonoDomain *domain = NULL;

extern PLDotNetFunction *pldotnet_function_list;
extern char *pldotnet_assembly_list;

void pldotnet_mono_init()
{
	if (!domain)
	{
		elog(DEBUG1, "pldotnet_mono_init()");
		domain = mono_jit_init ("pldotnet");
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
	MonoClass *klass;
	gpointer iter;
	gpointer param_iter;
	MonoMethod *method;
	MonoMethodSignature *sig;
	MonoType *ptype;
	//MonoImageOpenStatus status;
	//MonoType *rettype;

	const char *namespace;
	const char *classname;

	PLDotNetFunction *func;

	int i;

	elog(DEBUG1, "Loading %s", name);

	assembly = mono_domain_assembly_open (domain, name);

	if (!assembly)
	{
		elog(ERROR, "Error loading assembly %s\n", name);
	}

	image = mono_assembly_get_image(assembly);

	int rows = mono_image_get_table_rows (image, MONO_TABLE_TYPEDEF);

	/* we start the count from 1 because we skip the special type <Module> */
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
			elog(DEBUG1, "Added function at %p", func);

			func->klass = klass;
			func->method = method;
			func->name = strdup(mono_method_get_name(method));
			func->fqn = (char *)malloc(strlen(func->name) + strlen(classname) + strlen(namespace) + 4);
			sprintf(func->fqn, "%s.%s:%s", namespace, classname, func->name);

			elog(DEBUG1, "Method FQN: %s", func->fqn);
			
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
	
	elog(DEBUG1, "Done loading assembly");

}

Datum pldotnet_call_function(PLDotNetFunction *func, void **params)
{
	MonoObject *result;
	Datum ret;
	MonoObject *exc = (MonoObject *)malloc(sizeof (MonoObject)); //exception
	//char *str;

	elog(DEBUG1, "Invoke");

	result = mono_runtime_invoke(func->method, NULL, params, &exc);
	
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

