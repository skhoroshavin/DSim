
#pragma once

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "table/table.h"

void dsim_lua_register_table( lua_State * l, struct dsim_table * tbl, const char * name );
