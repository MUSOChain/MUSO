#!/bin/sh

# Execute this script with a running Postgres server on the current host.
# It should work with the most generic installation of Postgres,
# and is necessary for MUSO to store data in Postgres.

# usage: sudo -u postgres ./initdb.sh
psql -c "CREATE USER MUSO"
psql -c "CREATE DATABASE MUSO WITH OWNER = MUSO"

