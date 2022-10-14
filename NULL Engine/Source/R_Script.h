#ifndef __R_SCRIPT_H__
#define __R_SCRIPT_H__

#include "Resource.h"

class ParsonNode;

class R_Script : public Resource
{
public:
	R_Script();
	~R_Script();

	bool CleanUp() override;

	bool SaveMeta(ParsonNode& meta_root) const override;
	bool LoadMeta(const ParsonNode& meta_root) override;

public:
	// R_Script-Specific Methods

private:
	// R_Script-Specific Variables
};

#endif // !__R_SCRIPT_H__