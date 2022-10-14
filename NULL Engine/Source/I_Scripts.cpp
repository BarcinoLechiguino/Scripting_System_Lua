#include "FileSystemDefinitions.h"
#include "JSONParser.h"

#include "R_Script.h"

#include "I_Scripts.h"

bool Importer::Scripts::Import(const char* buffer, R_Script* r_script)
{
	return true;
}

uint Importer::Scripts::Save(const R_Script* r_script, char** buffer)
{
	uint written;

	if (r_script == nullptr)
	{
		LOG("[ERROR] Importer: Could not Save Folder in Library! Error: R_Folder* was nullptr.");
		return 0;
	}

	std::string error_string = "[ERROR] Importer: Could not Save Folder { " + std::string(r_script->GetAssetsPath()) + " } in Library";

	ParsonNode root_node = ParsonNode();
	r_script->SaveMeta(root_node);

	std::string path = SCRIPTS_PATH + std::to_string(r_script->GetUID()) + SCRIPTS_EXTENSION;
	written = root_node.SerializeToFile(path.c_str(), buffer);
	if (written > 0)
	{
		LOG("[STATUS] Importer: Successfully saved Script { %s } to Library! Path: %s", r_script->GetAssetsPath(), path.c_str());
	}
	else
	{
		LOG("%s! Error: File System could not Write File!", error_string.c_str());
	}

	return written;
}

bool Importer::Scripts::Load(const char* buffer, R_Script* r_script)
{
	if (r_script == nullptr)
	{
		LOG("[ERROR] Importer: Could not Load Script from Library! Error: R_Script* was nullptr.");
		return false;
	}

	std::string error_string = "[ERROR] Importer: Could not Load Script{ " + std::string(r_script->GetAssetsPath()) + " } from Library!";

	if (buffer == nullptr)
	{
		LOG("%s! Error: Given buffer was nullptr.", error_string.c_str());
		return false;
	}

	ParsonNode root_node = ParsonNode(buffer);
	ParsonArray contained_array = root_node.GetArray("ContainedResources");
	if (!root_node.NodeIsValid())
	{
		LOG("%s! Error: Could not get the Root Node from the passed buffer.", error_string.c_str());
		return false;
	}
	if (!contained_array.ArrayIsValid())
	{
		LOG("%s! Error: Could not get the ModelNodes array from the Root Node.", error_string.c_str());
		return false;
	}

	for (uint i = 0; i < contained_array.size; ++i)
	{
		uint32 contained_uid = (uint32)contained_array.GetNumber(i);
	}

	LOG("[STATUS] Importer: Successfully Loaded Script { %s } from Library! UID: %lu", r_script->GetAssetsPath(), r_script->GetUID());
	
	return true;
}