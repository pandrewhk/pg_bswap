-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_bswap" to load this file. \quit

CREATE FUNCTION bswap(anyelement)
    RETURNS anyelement
    AS 'MODULE_PATHNAME'
    LANGUAGE C STRICT IMMUTABLE PARALLEL SAFE;
