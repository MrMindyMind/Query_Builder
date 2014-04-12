#ifndef _H_UTILS
#define _H_UTILS

#include <string>
#include "plugin.h"

namespace Utilities
{
	void GetPawnString(AMX* amx, cell p_pawnString, std::string &strDest, int nLength = 129);
}

#endif