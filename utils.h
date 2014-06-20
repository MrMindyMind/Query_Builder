#ifndef _H_UTILS
#define _H_UTILS

#include <string>
#include <list>
#include "plugin.h"

namespace Utilities
{
	void GetPawnString(AMX* amx, const cell& p_pawnString, std::string &strDest, int nLength);
	void TrimWhitespace(std::string& src);
	std::list<std::string> Split(const std::string& src, char delimeter);
}

#endif