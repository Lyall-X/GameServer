#pragma once

#pragma warning(disable : 4996)
#pragma warning(disable : 4091)
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_COPP_OVERLOAD_STANDARD_NAMES 1
#pragma warning(disable:4251)

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
