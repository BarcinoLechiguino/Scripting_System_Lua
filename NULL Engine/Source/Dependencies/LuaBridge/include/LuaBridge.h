// https://github.com/vinniefalco/LuaBridge
// Copyright 2020, Dmitry Tarakanov
// Copyright 2012, Vinnie Falco <vinnie.falco@gmail.com>
// Copyright 2007, Nathan Reed
// SPDX-License-Identifier: MIT

#pragma once

// All #include dependencies are listed here
// instead of in the individual header files.
//

#define LUABRIDGE_MAJOR_VERSION 2
#define LUABRIDGE_MINOR_VERSION 7
#define LUABRIDGE_VERSION 207

#ifndef LUA_VERSION_NUM
#error "Lua headers must be included prior to LuaBridge ones"
#endif

#include <LuaBridge/include/detail/CFunctions.h>
#include <LuaBridge/include/detail/ClassInfo.h>
#include <LuaBridge/include/detail/Constructor.h>
#include <LuaBridge/include/detail/FuncTraits.h>
#include <LuaBridge/include/detail/Iterator.h>
#include <LuaBridge/include/detail/LuaException.h>
#include <LuaBridge/include/detail/LuaHelpers.h>
#include <LuaBridge/include/detail/LuaRef.h>
#include <LuaBridge/include/detail/Namespace.h>
#include <LuaBridge/include/detail/Security.h>
#include <LuaBridge/include/detail/Stack.h>
#include <LuaBridge/include/detail/TypeList.h>
#include <LuaBridge/include/detail/TypeTraits.h>
#include <LuaBridge/include/detail/Userdata.h>
