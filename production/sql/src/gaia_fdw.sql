---------------------------------------------
-- Copyright (c) Gaia Platform LLC
-- All rights reserved.
---------------------------------------------
-- complain if script is sourced in psql, rather than via CREATE EXTENSION

\echo Use "CREATE EXTENSION gaia_fdw" to load this file. \quit
CREATE FUNCTION gaia_fdw_handler ()
  RETURNS fdw_handler
  AS 'MODULE_PATHNAME'
  LANGUAGE C
  STRICT;

CREATE FUNCTION gaia_fdw_validator (text[], oid)
  RETURNS void
  AS 'MODULE_PATHNAME'
  LANGUAGE C
  STRICT;

CREATE FOREIGN DATA WRAPPER gaia_fdw HANDLER gaia_fdw_handler VALIDATOR gaia_fdw_validator;