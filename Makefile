MODULES = pg_bswap

EXTENSION = pg_bswap
DATA = pg_bswap--0.1.sql
PGFILEDESC = "pg_bswap - byte swap for various data"

REGRESS = pg_bswap

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
