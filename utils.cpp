#include "utils.h"

extern logprintf_t logprintf;

void Utilities::GetPawnString(AMX* amx, const cell& p_pawnString, std::string &strDest, int nLength)
{
	if (p_pawnString == NULL)
	{
		strDest.clear();
	}
	else
	{
		cell *p_physAddr = nullptr;

		amx_GetAddr(amx, p_pawnString, &p_physAddr);

		if (p_physAddr != nullptr)
		{
			char
				*p_szString = new char[nLength];

			amx_GetString(p_szString, p_physAddr, 0, nLength);

			strDest.assign(p_szString);

			delete[] p_szString;
		}
	}
}

std::list<std::string> Utilities::Split(const std::string& src, char delimeter)
{
	std::list<std::string> split_strings;

	int lastIndex = 0;

	for (unsigned int i = 0; i < src.length(); i++)
	{
		if (src.at(i) == delimeter)
		{
			split_strings.push_back(src.substr(lastIndex, i-lastIndex));
			lastIndex = i + 1;
		}
	}

	if (lastIndex != (src.length() - 1))
	{
		split_strings.push_back(src.substr(lastIndex, src.length()-lastIndex));
	}

	return split_strings;
}

void Utilities::TrimWhitespace(std::string& src)
{
	for (auto it = src.begin(); it != src.end();)
	{
		if (*it == ' ')
			it = src.erase(it);
		else
			it++;
	}
}