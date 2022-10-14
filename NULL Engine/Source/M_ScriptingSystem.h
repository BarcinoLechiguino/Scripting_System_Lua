#ifndef __M_SCRIPTING_SYSTEM_H__
#define __M_SCRIPTING_SYSTEM_H__

#include "Module.h"

struct lua_State;
struct Script;

class GameObject;
class C_Script;

enum class KEY_STATE;

class M_ScriptingSystem : public Module
{
public:
	M_ScriptingSystem();
	~M_ScriptingSystem();

	bool			Init				(ParsonNode& parsonNode) override;
	bool			Start				() override;
	UPDATE_STATUS	PreUpdate			(float dt) override;
	UPDATE_STATUS	Update				(float dt) override;
	UPDATE_STATUS	PostUpdate			(float dt) override;
	bool			CleanUp				() override;

	bool			SaveConfiguration	(ParsonNode& root) const;
	bool			LoadConfiguration	(ParsonNode& root);

public:
	void	AddScript			(C_Script* c_script);
	void	RemoveScript		(C_Script* c_script);

	void	RecompileScript		(C_Script* c_script);
	void	RecompileAllScripts	();

	Script* GetCurrentScript	();

private:																	// Module-specific Functions
	void	CreateNamespaces	();
	void	UpdateNamespaces	();

	bool	CompileScript		(Script* script);
	bool	ExecuteScript		(Script* script);

	bool	ScriptIsActive		(Script* script) const;

private:																	// Module-specific Variables
	lua_State*				VM;												// Lua's Virtual Machine Class.
	Script*					current_script;
	std::vector<Script*>	scripts;
	std::vector<uint32>		script_resources;
};

class ScriptingEvents
{
public:
	ScriptingEvents() {}
	~ScriptingEvents() {}

public:
	// Debug
	void		LuaLog		(const char* message) const;

	// Time
	float		GetRealDT	() const;
	float		GetGameDT	() const;

	// Input
	KEY_STATE	GetKeyState	(const char* key) const;
	bool		IsKeyDown	(const char* key) const;
	bool		IsKeyRepeat	(const char* key) const;
	bool		IsKeyUp		(const char* key) const;

	// GameObject
	void		Instantiate	(uint32 reference_UID, float pos_x, float pos_y, float pos_z, float rot_x, float rot_y, float rot_z, float scale_factor);
	void		Destroy		(bool self_destroy, uint32 UID);

	void		Translate	(float x, float y, float z);
	void		Rotate		(float x, float y, float z);
	void		Scale		(float x, float y, float z);
};

#endif // !__M_SCRIPTING_SYSTEM_H__