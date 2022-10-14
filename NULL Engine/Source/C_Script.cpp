#include "Application.h"
#include "M_ResourceManager.h"
#include "M_ScriptingSystem.h"

#include "R_Script.h"

#include "GameObject.h"

#include "C_Script.h"

C_Script::C_Script(GameObject* owner) : Component(owner, COMPONENT_TYPE::SCRIPT),
r_script(nullptr)
{

}

C_Script::~C_Script()
{

}

bool C_Script::Update()
{
	return true;
}

bool C_Script::CleanUp()
{
	return true;
}

bool C_Script::SaveState(ParsonNode& root) const
{
	return true;
}

bool C_Script::LoadState(ParsonNode& root)
{
	return true;
}

// --- C_SCRIPT METHODS ---
void C_Script::SetScript(R_Script* r_script)
{
	if (r_script == nullptr)		{ return; }
	//if (r_script == this->r_script) { return; }
	
	if (this->r_script != nullptr)
	{
		App->scripting_system->RemoveScript(this);
		App->resource_manager->FreeResource(this->r_script->GetUID());
	}
	
	App->scripting_system->AddScript(this);
	this->r_script = r_script;
}

R_Script* C_Script::GetScript() const
{
	return r_script;
}

const char* C_Script::GetScriptFile() const
{
	return (r_script != nullptr) ? r_script->GetAssetsFile() : "[NO SCRIPT RESOURCE]";
}

void C_Script::OnRecompileScript()
{
	if (r_script == nullptr) { return; }
	
	App->scripting_system->RecompileScript(this);
}

void C_Script::OnRecompileAllScripts()
{
	App->scripting_system->RecompileAllScripts();
}