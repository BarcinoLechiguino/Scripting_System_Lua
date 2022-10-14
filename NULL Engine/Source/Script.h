#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include <string>

#include "LuaBridge/include/LuaBridge.h"

class C_Script;
class R_Script;

struct Script
{
	Script(C_Script* component, R_Script* resource)
		: component(component)
		, resource(resource)
		, table(0)
		, tableStr("")
		, full_path("")
		, hasCompiled(false)
		, isAwake(false)
		, exclude(false)
	{}

	C_Script*			component;
	R_Script*			resource;

	luabridge::LuaRef	table;
	std::string			tableStr;							// String used to query a script's table.

	std::string			full_path;							// Path used to load the script through lua
	bool				hasCompiled;
	bool				isAwake;
	bool				exclude;
};

#endif // !__SCRIPT_H__