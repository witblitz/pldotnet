
#include "typeconv.h"

inline unsigned int convert_pgtype_to_monotype(Oid oid)
{
	switch (oid)
	{
		case BOOLOID:
			return MONO_TYPE_BOOLEAN;
		case INT2OID:
			return MONO_TYPE_I2;
		case INT4OID:
			return MONO_TYPE_I4;
		case INT8OID:
			return MONO_TYPE_I8;
		case TEXTOID:
			return MONO_TYPE_STRING;
		default:
			elog(ERROR, "Data type with OID %d is not supported by PL/Mono", oid);
	}
	return 0;
}

unsigned int *convert_pgtypes_to_argtypes(const Oid *pgtypelist, int len)
{
	int i;
	unsigned int *ret = (unsigned int *)malloc(len * sizeof(unsigned int));
	for (i = 0; i < len; i++)
		ret[i] = convert_pgtype_to_monotype(pgtypelist[i]);
	return ret;
}


void *convert_datum_to_value(Datum val, Oid oid)
{
	void *obj = NULL;
	//MonoError *err = NULL;
	const char *cstring;

	switch (oid)
	{
		case BOOLOID:
			obj = (int32 *) palloc(sizeof(int32));
			*((int32 *) obj) = DatumGetBool(val);
			break;

		case INT2OID:
			obj = (int16 *) palloc(sizeof(int16));
			*((int16 *) obj) = DatumGetInt16(val);
			break;

		case INT4OID:
			obj = (int32 *) palloc(sizeof(int32));
			*((int32 *) obj) = DatumGetInt32(val);
			break;

		case INT8OID:
			obj = (int64 *) palloc(sizeof(int64));
			*((int64 *) obj) = DatumGetInt64(val);
			break;

		case FLOAT4OID:
			obj = (float4 *) palloc(sizeof(float4));
			*((float4 *) obj) = DatumGetFloat4(val);
			break;

		case FLOAT8OID:
			obj = (float8 *) palloc(sizeof(float8));
			*((float8 *) obj) = DatumGetFloat8(val);
			break;

		case TEXTOID:
			cstring = TextDatumGetCString(val);
			//obj = mono_string_new_checked(mono_domain_get(), cstring, err);
			obj = mono_string_new(mono_domain_get(), cstring);
			break;

		default:
			elog(ERROR, "Data type with OID %d is not supported by PL/Mono", oid);
	}

	return obj;

}

Datum convert_value_to_datum(void *obj, Oid typeoid)
{
	Datum val = Int32GetDatum(0);

	switch (typeoid)
	{
		case BOOLOID:
			val = BoolGetDatum(*((int32*) obj));
			break;

		case INT2OID:
			val = Int16GetDatum(*((int16*) obj));
			break;

		case INT4OID:
			val = Int32GetDatum(*((int32*) obj));
			break;

		case INT8OID:
			val = Int64GetDatum(*((int64*) obj));
			break;

		case FLOAT4OID:
			val = Float4GetDatum(*((float4*) obj));
			break;

		case FLOAT8OID:
			val = Float8GetDatum(*((float8*) obj));
			break;

		case TEXTOID:
			elog(DEBUG1, "Getting data %p", obj);
			val = CStringGetTextDatum(mono_string_to_utf8((MonoString*) obj));
			break;

		default:
			elog(ERROR, "Data type with OID %d is not supported by PL/Mono", typeoid);
	}

	return val;

}

