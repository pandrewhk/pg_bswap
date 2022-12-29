/*-------------------------------------------------------------------------
 *
 * pg_bswap.c
 *		byte order reversal function for PostgreSQL
 *
 * Copyright (c) 2022-2023, Andrew Pantyukhin <infofarmer@FreeBSD.org>
 *
 * IDENTIFICATION
 *		pg_bswap/pg_bswap.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include "fmgr.h"
#include "utils/lsyscache.h"
#include "port/pg_bswap.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(bswap);
Datum
bswap(PG_FUNCTION_ARGS) {
	Oid		typ = get_fn_expr_argtype(fcinfo->flinfo, 0);
	int16	typlen;
	bool	typbyval;

	if (!OidIsValid(typ))
		elog(ERROR, "could not determine data type of input");

	get_typlenbyval(typ, &typlen, &typbyval);

	//pass by reference, variable-length
	if (typlen == -1){
		struct varlena *d = PG_DETOAST_DATUM_COPY(PG_GETARG_VARLENA_PP(0));
		unsigned char *ptr = (unsigned char *) VARDATA_ANY(d);
		int32 dataLen = VARSIZE_ANY_EXHDR(d);

		unsigned char *start, *end;

		for (start = ptr, end = ptr + dataLen - 1; start < end; ++start, --end) {
			unsigned char swap = *start;
			*start = *end;
			*end = swap;
		}
		PG_RETURN_POINTER(d);
	}

	//pass by reference, fixed length
	if (typlen > 0 && !typbyval) {
		unsigned char *d = (unsigned char *) PG_GETARG_POINTER(0);
		unsigned char *r = (unsigned char *) palloc(typlen);

		for (int16 i = 0; i < typlen; ++i)
			*(r+i) = *(d+typlen-i-1); //copy backwards

		PG_RETURN_POINTER(r);
	}

	//pass by value, fixed length
	if (typbyval && typlen == 1)
		PG_RETURN_DATUM(PG_GETARG_DATUM(0));

	if (typbyval && typlen == 2)
		PG_RETURN_UINT16(pg_bswap16(PG_GETARG_UINT16(0)));

	if (typbyval && typlen == 4)
		PG_RETURN_UINT32(pg_bswap32(PG_GETARG_UINT32(0)));

	if (typbyval && typlen == 8)
		PG_RETURN_INT64(pg_bswap64(PG_GETARG_INT64(0)));

	elog(ERROR, "unexpected argument type");
	PG_RETURN_NULL();
}
