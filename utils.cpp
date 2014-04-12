#include "utils.h"

void Utilities::GetPawnString(AMX* amx, cell p_pawnString, std::string &strDest, int nLength)
{
	cell
		*p_physAddr	= nullptr;

	amx_GetAddr(amx, p_pawnString, &p_physAddr);

	if (p_physAddr != nullptr)
	{
		char
			*p_szString	= new char[nLength];

		amx_GetString(p_szString, p_physAddr, 0, nLength);

		strDest.assign(p_szString);

		delete[] p_szString;
	}
}