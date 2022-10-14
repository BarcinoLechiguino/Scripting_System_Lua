#include "JSONParser.h"

#include "R_Script.h"

R_Script::R_Script() : Resource(RESOURCE_TYPE::SCRIPT)
{

}

R_Script::~R_Script()
{

}

bool R_Script::CleanUp()
{
	return true;
}

bool R_Script::SaveMeta(ParsonNode& meta_root) const
{
	ParsonArray containedArray = meta_root.SetArray("ContainedResources");
	
	return true;
}

bool R_Script::LoadMeta(const ParsonNode& meta_root)
{
	return true;
}