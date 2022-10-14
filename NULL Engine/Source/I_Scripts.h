#ifndef __I_SCRIPTS_H__
#define __I_SCRIPTS_H__

#include "VariableTypedefs.h"

class R_Script;

namespace Importer
{
	namespace Scripts
	{
		bool Import(const char* buffer, R_Script* r_script);
		uint Save(const R_Script* r_script, char** buffer);
		bool Load(const char* buffer, R_Script* r_script);
	}
}

#endif // !__I_SCRIPTS_H__