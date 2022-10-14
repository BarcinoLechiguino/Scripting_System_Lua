//#include "lua/include/lua.hpp"
extern "C" 
{
#include "lua/include/lua.h"
#include "lua/include/lualib.h"
#include "lua/include/lauxlib.h"
}

#include "LuaBridge/include/LuaBridge.h"								// Must go after the Lua includes as it is a wrapper for Lua's VM.

#include "Time.h"

#include "Application.h"
#include "M_FileSystem.h"
#include "M_Input.h"
#include "M_Scene.h"

#include "R_Script.h"

#include "GameObject.h"
#include "C_Transform.h"
#include "C_Script.h"

#include "Script.h"

#include "M_ScriptingSystem.h"
 
#pragma comment(lib, "Source/Dependencies/lua/libx86/liblua54.a")

M_ScriptingSystem::M_ScriptingSystem() : Module("ScriptingSystem"),
VM(nullptr),
current_script(nullptr)
{

}

M_ScriptingSystem::~M_ScriptingSystem()
{

}

bool M_ScriptingSystem::Init(ParsonNode& config)
{	
	VM = luaL_newstate();
	if (VM == nullptr) 
	{ 
		return false; 
	}
	
	luaL_openlibs(VM);

	CreateNamespaces();

	return true;
}

bool M_ScriptingSystem::Start()
{	
	LOG("Lua Script: Test Log from test.lua script!");
	
	return true;
}

UPDATE_STATUS M_ScriptingSystem::PreUpdate(float dt)
{	
	return UPDATE_STATUS::CONTINUE;
}

UPDATE_STATUS M_ScriptingSystem::Update(float dt)
{
	if (!App->play) { return UPDATE_STATUS::CONTINUE; }

	for (Script* script : scripts)
	{
		if (script == nullptr || script->exclude) { continue; }

		if (!script->hasCompiled)
		{
			if (!CompileScript(script)) 
			{ 
				script->exclude = true; 
				continue; 
			}
		}

		if (ScriptIsActive(script))
		{
			current_script = script;
			UpdateNamespaces();
			ExecuteScript(script);
		}
	}
	
	return UPDATE_STATUS::CONTINUE;
}

UPDATE_STATUS M_ScriptingSystem::PostUpdate(float dt)
{
	return UPDATE_STATUS::CONTINUE;
}

bool M_ScriptingSystem::CleanUp()
{
	for (Script* script : scripts)
	{
		RELEASE(script);
	}

	scripts.clear();
	script_resources.clear();
	
	return true;
}

bool M_ScriptingSystem::SaveConfiguration(ParsonNode& root) const
{
	return true;
}

bool M_ScriptingSystem::LoadConfiguration(ParsonNode& root)
{
	return true;
}

// ---------------------- SCRIPTING METHODS ---------------------
void M_ScriptingSystem::AddScript(C_Script* c_script)
{
	if (c_script == nullptr)				{ return; }
	if (c_script->GetOwner() == nullptr)	{ return; }
	if (c_script->GetScript() == nullptr)	{ return; }

	Script* new_script = new Script(c_script, c_script->GetScript());
	if (new_script != nullptr)
	{
		new_script->full_path	= std::string(ROOT_PATH) + new_script->resource->GetAssetsPath();					// Path used to load the script through lua
		new_script->tableStr	= "GetTable_" + new_script->resource->GetAssetsName();								// String used to query a script's table.
		scripts.push_back(new_script);

		auto item = std::find(script_resources.begin(), script_resources.end(), new_script->resource->GetUID());
		if (item == script_resources.end())
		{
			script_resources.push_back(new_script->resource->GetUID());
		}
	}
}

void M_ScriptingSystem::RemoveScript(C_Script* c_script)
{
	if (c_script == nullptr)				{ return; }
	if (c_script->GetScript() == nullptr)	{ return; }
	
	for (auto iter = scripts.cbegin(); iter != scripts.cend(); ++iter)
	{
		Script* script = (*iter);
		if (script->component == c_script && script->resource == c_script->GetScript())
		{
			if (script->resource->GetReferences() == 1)
			{
				auto item = std::find(script_resources.begin(), script_resources.end(), script->resource->GetUID());
				if (item != script_resources.end())
				{
					script_resources.erase(item);
				}
			}

			scripts.erase(iter);
			return;
		}
	}
}

void M_ScriptingSystem::RecompileScript(C_Script* c_script)
{
	if (c_script == nullptr) 
	{
		LOG("[ERROR] Could not recompile script! Error: Passed C_Script* was nullptr!");
		return;
	}
	
	for (Script* script : scripts)
	{
		if (script->component == c_script)
		{
			script->hasCompiled = false;
			script->exclude = false;
			script->isAwake = false;

			break;
		}
	}
}

void M_ScriptingSystem::RecompileAllScripts()
{
	for (Script* script : scripts)
	{
		script->hasCompiled = false;
		script->exclude = false;
		script->isAwake = false;
	}
}

Script* M_ScriptingSystem::GetCurrentScript()
{
	return current_script;
}

// --- PRIVATE METHODS ---------------------------------
void M_ScriptingSystem::CreateNamespaces()
{
	luabridge::getGlobalNamespace(VM)
		.beginNamespace("Event")
			.beginClass<ScriptingEvents>("Utils")
				.addConstructor<void (*) (void)>()
				.addFunction("LOG", &ScriptingEvents::LuaLog)
				
				.addFunction("GetRealDT", &ScriptingEvents::GetRealDT)
				.addFunction("GetGameDT", &ScriptingEvents::GetGameDT)
				
				.addFunction("GetKeyState", &ScriptingEvents::GetKeyState)
				.addFunction("IsKeyDown", &ScriptingEvents::IsKeyDown)
				.addFunction("IsKeyRepeat", &ScriptingEvents::IsKeyRepeat)
				.addFunction("IsKeyUp", &ScriptingEvents::IsKeyUp)

				.addFunction("Instantiate", &ScriptingEvents::Instantiate)
				.addFunction("Destroy", &ScriptingEvents::Destroy)
				.addFunction("Translate", &ScriptingEvents::Translate)
				.addFunction("Rotate", &ScriptingEvents::Rotate)
				.addFunction("Scale", &ScriptingEvents::Scale)
			.endClass()
		.endNamespace();
}

void M_ScriptingSystem::UpdateNamespaces()
{
	luabridge::getGlobalNamespace(VM)
		.beginNamespace("Event")
			.beginClass<ScriptingEvents>("Utils")
				.addConstructor<void (*) (void)>()
				.addFunction("LOG", &ScriptingEvents::LuaLog)
				
				.addFunction("GetRealDT", &ScriptingEvents::GetRealDT)
				.addFunction("GetGameDT", &ScriptingEvents::GetGameDT)
				
				.addFunction("GetKeyState", &ScriptingEvents::GetKeyState)
				.addFunction("IsKeyDown", &ScriptingEvents::IsKeyDown)
				.addFunction("IsKeyRepeat", &ScriptingEvents::IsKeyRepeat)
				.addFunction("IsKeyUp", &ScriptingEvents::IsKeyUp)
	
				.addFunction("Instantiate", &ScriptingEvents::Instantiate)
				.addFunction("Destroy", &ScriptingEvents::Destroy)
				.addFunction("Translate", &ScriptingEvents::Translate)
				.addFunction("Rotate", &ScriptingEvents::Rotate)
				.addFunction("Scale", &ScriptingEvents::Scale)
			.endClass()
		.endNamespace();
}

bool M_ScriptingSystem::CompileScript(Script* script)
{
	bool result = luaL_dofile(VM, script->full_path.c_str());
	if (result != LUA_OK)
	{
		std::string luaError = lua_tostring(VM, -1);
		LOG("[ERROR] Could not load script file! Lua Error: %s", luaError.c_str());
		return false;
	}

	luabridge::LuaRef GetTable = luabridge::getGlobal(VM, script->tableStr.c_str());
	if (GetTable.isNil())
	{
		LOG("[ERROR] Could not get GetTable() from script { %s }!", script->resource->GetAssetsFile());
		return false;
	}

	luabridge::LuaRef table(GetTable());
	if (table.isNil())
	{
		LOG("[ERROR] Could not get Lua Table from script { %s }!", script->resource->GetAssetsFile());
		return false;
	}

	script->table = table;
	script->hasCompiled = true;

	return true;
}

bool M_ScriptingSystem::ExecuteScript(Script* script)
{
	if (!script->isAwake)
	{
		script->table["Start"]();
		script->isAwake = true;
	}

	script->table["Update"]();
	
	return true;
}

bool M_ScriptingSystem::ScriptIsActive(Script* script) const
{
	return (script->component->IsActive() && script->component->GetOwner()->IsActive());
}

// ---------------------- SCRIPTING EVENTS ----------------------
void ScriptingEvents::LuaLog(const char* message) const
{
	LOG("%s", message);
}

float ScriptingEvents::GetRealDT() const
{
	return Time::Real::GetDT();
}

float ScriptingEvents::GetGameDT() const
{
	return Time::Game::GetDT();
}

KEY_STATE ScriptingEvents::GetKeyState(const char* key) const
{
	SDL_Scancode key_id = SDL_GetScancodeFromName(key);
	if (key_id == SDL_SCANCODE_UNKNOWN)
	{
		LOG("[ERROR] Script passed an unknown key name string! Key: %s", key);
		return KEY_STATE::KEY_ERROR;
	}
	
	return App->input->GetKey(key_id);
}

bool ScriptingEvents::IsKeyDown(const char* key) const
{
	return (GetKeyState(key) == KEY_STATE::KEY_DOWN);
}

bool ScriptingEvents::IsKeyRepeat(const char* key) const
{
	return (GetKeyState(key) == KEY_STATE::KEY_REPEAT);
}

bool ScriptingEvents::IsKeyUp(const char* key) const
{
	return (GetKeyState(key) == KEY_STATE::KEY_UP);
}

void ScriptingEvents::Instantiate(uint32 reference_UID, float pos_x, float pos_y, float pos_z, float rot_x, float rot_y, float rot_z, float scale_factor)
{

}

void ScriptingEvents::Destroy(bool self_destroy, uint32 UID)
{
	if (self_destroy)
	{
		App->scene->DeleteGameObject(App->scripting_system->GetCurrentScript()->component->GetOwner());
	}
	else
	{
		// Delete GameObject by UID.
	}
}

void ScriptingEvents::Translate(float x, float y, float z)
{
	App->scripting_system->GetCurrentScript()->component->GetOwner()->transform->Translate(float3(x, y, z));
}

void ScriptingEvents::Rotate(float x, float y, float z)
{
	App->scripting_system->GetCurrentScript()->component->GetOwner()->transform->Rotate(float3(x, y, z));
}

void ScriptingEvents::Scale(float x, float y, float z)
{
	App->scripting_system->GetCurrentScript()->component->GetOwner()->transform->Scale(float3(x, y, z));
}

// ---------------------------------------------------------------------------------------
//std::string script_file = "test.lua";
//std::string script_path = "Resources/" + std::string(ASSETS_SCRIPTS_PATH) + script_file;
//static bool done = false;
//if (!done)
//{
//	//std::string test = "value = 1 + 2";
//	//int num = 1 + 2;
//	//LOG("Lua operation \"%s\" = %d", test.c_str(), num);
//
//	int start_num = 5;
//	int update_num = 10;
//	
//	LOG("Lua Script: Test Log from test.lua script!");
//	LOG("Lua operations in Start() were successful! start_test = %d", start_num);
//	LOG("Successfully called Start()! File: \"%s\"", script_file.c_str());
//	LOG("Lua operations in Update() were successful! update_test = %d", update_num);
//	LOG("Successfully called Update()! File: \"%s\"", script_file.c_str());
//
//	done = true;
//}

//std::string test = "value = 1 + 2";
//int value = luaL_dostring(VM, test.c_str());
//if (value == LUA_OK)
//{
//	lua_getglobal(VM, "value");
//	int num = lua_tointeger(VM, -1);
//	LOG("Lua operation %s = %d", test.c_str(), num);
//}

//luabridge::getGlobalNamespace(VM)
//.beginNamespace("Debug")
//.beginClass<ScriptingEvents>("Event")
//.addConstructor<void (*) (void)>()
//.addFunction("LOG", &ScriptingEvents::LuaLog)
//.endClass()
//.endNamespace();
//
//
//std::string script_file = "test.lua";
//std::string script_path = ROOT_PATH + std::string(ASSETS_SCRIPTS_PATH) + script_file;
//bool result = luaL_dofile(VM, script_path.c_str());
//if (result == LUA_OK)
//{
//	luabridge::LuaRef ScriptStart = luabridge::getGlobal(VM, "Start");
//	if (!ScriptStart.isNil())
//	{
//		ScriptStart();
//		luabridge::LuaRef number = luabridge::getGlobal(VM, "start_test");
//		if (!number.isNil())
//		{
//			LOG("Lua operations in Start() were successful! start_test = %d", number.cast<int>());
//		}
//
//		LOG("Successfully called Start()! File: %s", script_file.c_str());
//	}
//	else
//	{
//		LOG("[ERROR] Could not call Start()! File: %s, Path: %s", script_file.c_str(), script_path.c_str());
//	}
//
//	luabridge::LuaRef ScriptUpdate = luabridge::getGlobal(VM, "Update");
//	if (!ScriptUpdate.isNil())
//	{
//		for (uint i = 0; i < 10; ++i)
//		{
//			ScriptUpdate();
//		}
//
//		luabridge::LuaRef number = luabridge::getGlobal(VM, "update_test");
//		if (!number.isNil())
//		{
//			LOG("Lua operations in Update() were successful! update_test = %d", number.cast<int>());
//		}
//
//		LOG("Successfully called Update()! File: %s", script_file.c_str());
//	}
//	else
//	{
//		LOG("[ERROR] Could not call Update()! File: %s, Path: %s", script_file.c_str(), script_path.c_str());
//	}
//}
//else
//{
//	std::string luaError = lua_tostring(VM, -1);
//	LOG("[ERROR] Could not load script file! Lua Error: %s", luaError.c_str());
//}

/*bool result = luaL_dofile(VM, script->full_path.c_str());
if (result != LUA_OK)
{
	std::string luaError = lua_tostring(VM, -1);
	LOG("[ERROR] Could not load script file! Lua Error: %s", luaError.c_str());
	continue;
}

if (!script->isAwake)
{
	LOG("Loaded Script: %s", script->full_path.c_str());

	ScriptStart(script);
	script->isAwake = true;
}

ScriptUpdate(script);*/