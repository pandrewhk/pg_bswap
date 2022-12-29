CREATE EXTENSION pg_bswap;
-- val fixed
SELECT bswap(1::bool);
SELECT bswap(1::char);
SELECT bswap(1::int2);
SELECT bswap(1::int4);
SELECT bswap(1::int8);
SELECT bswap(-2::int2);
SELECT bswap(-2::int4);
SELECT bswap(-2::int8);
-- ref varlena
SELECT bswap('abc'::text);
SELECT bswap('123'::json);
-- ref fixed
select bswap(('a0eebc99-9c0b-4ef8-bb6d-6bb9bd380a11')::uuid);
