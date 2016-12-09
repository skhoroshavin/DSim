
#pragma once

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "storage/storage.h"

void dsim_lua_register_storage( lua_State *l, struct dsim_storage *s, const char *name );
