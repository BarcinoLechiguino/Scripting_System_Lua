#ifndef __C_SCRIPT_H__
#define __C_SCRIPT_H__

#include "Component.h"

class R_Script;
class GameObject;

class C_Script : public Component
{
public:
	C_Script(GameObject* owner);
	~C_Script();

	bool Update		() override;
	bool CleanUp	() override;

	bool SaveState	(ParsonNode& root) const override;
	bool LoadState	(ParsonNode& root) override;

	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::SCRIPT; }

public:
	void		SetScript(R_Script* r_script);
	R_Script*	GetScript() const;

	const char* GetScriptFile() const;

	void		OnRecompileScript();
	void		OnRecompileAllScripts();

private:
	R_Script* r_script;
};

#endif // !__C_SCRIPT_H__