/*
    SA-MP Streamer Plugin v2.5.2
    Copyright © 2010 Incognito

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>

#include "Invoke.h"
#include "plugin.h"
#include "utils.h"

Invoke * g_Invoke;
extern logprintf_t logprintf;

int Invoke::callNative(const PAWN::Native * native, ...)
{
	if (amx_list.empty() || amx_map.find(native->name) == amx_map.end())
	{
		return 0;
	}

	unsigned int amx_addr = amx_map[native->name], count = strlen(native->data), variables = 0;
	cell * params = new cell[count + 1], * physAddr[6];
	params[0] = count * sizeof(cell);
	va_list input;
	va_start(input, native);

	for (unsigned int i = 0; i < count; ++i)
	{
		switch (native->data[i])
		{
			case 'd':
			case 'i':
			{
				params[i + 1] = va_arg(input, int);
			}
			break;
			case 'b':
			{
				params[i + 1] = va_arg(input, bool);
			}
			break;
			case 'f':
			{
				float value = (float)va_arg(input, double);
				params[i + 1] = amx_ftoc(value);
			}
			break;
			case 's':
			{
				char * string = va_arg(input, char *);
				amx_Allot(amx_list.front(), strlen(string) + 1, &params[i + 1], &physAddr[variables++]);
				amx_SetString(physAddr[variables - 1], string, 0, 0, strlen(string) + 1);
			}
			break;
			case 'v':
			{
				va_arg(input, void *);
				amx_Allot(amx_list.front(), 1, &params[i + 1], &physAddr[variables++]);
			}
			break;
			case 'p':
			{
				va_arg(input, void *);
				int size = va_arg(input, int);
				amx_Allot(amx_list.front(), size, &params[++i], &physAddr[variables++]);
				params[i + 1] = amx_Allot(amx_list.front(), size, &params[++i], &physAddr[variables++]);
			}
			break;
			// Special refrence array/string where the destination is NOT followed by the size!
			// However, the string passed to this method MUST be followed by size!
			case 'r':
			{
				va_arg(input, void *);
				int size = va_arg(input, int);
				amx_Allot(amx_list.front(), size, &params[i + 1], &physAddr[variables++]);
			}
			break;
			case 'l': // list of variables.
			{
				char * strList = va_arg(input, char *);

				amx_Allot(amx_list.front(), strlen(strList) + 1, &params[i + 1], &physAddr[variables++]);
				amx_SetString(physAddr[variables - 1], strList, 0, 0, strlen(strList) + 1);

				int len = strlen(strList);

				cell* variableParams = va_arg(input, cell*);

				AMX* amx = va_arg(input, AMX*);

				count -= 2;
				count += len;

				i++;

				// resize the params array

				cell * tmpParams = new cell[count + 1], * physAddr[6];
				memcpy(tmpParams, params, (count + 1) * sizeof(cell));

				delete[] params;
				params = tmpParams;

				params[0] = count * sizeof(cell);

				std::string destStr;

				for (int j = 0; j < len; j++, i++)
				{
					switch (strList[j])
					{
					case 'i':
					case 'd':
						params[i + 1] = (cell) variableParams[j];
						break;
					case 'f':
						params[i + 1] = amx_ftoc(variableParams[j]);
						break;
					case 'b':
						params[i + 1] = (cell) variableParams[j];
						break;
					case 's':
						Utilities::GetPawnString(amx, variableParams[j], destStr, 128);

						amx_Allot(amx_list.front(), destStr.length() + 1, &params[i + 1], &physAddr[variables++]);
						amx_SetString(physAddr[variables - 1], destStr.c_str(), 0, 0, destStr.length() + 1);

						break;
					}
				}

				// stop the loop.
				i = count;
			}
			break;
		}
	}
	va_end(input);
	amx_Function_t amx_Function = (amx_Function_t)amx_addr;
	int value = amx_Function(amx_list.front(), params);
	if (variables)
	{
		variables = 0;
		va_start(input, native);
		for (unsigned int i = 0; i < count; ++i)
		{
			switch (native->data[i])
			{
				case 's':
				{
					va_arg(input, void *);
					amx_Release(amx_list.front(), params[i + 1]);
				}
				break;
				case 'v':
				{
					unsigned int * value = va_arg(input, unsigned int *), * returnValue = (unsigned int *)physAddr[variables++];
					* value = * returnValue;
					amx_Release(amx_list.front(), params[i + 1]);
				}
				break;
				case 'r':
				// FALLTHROUGH
				case 'p':
				{
					char * text = va_arg(input, char *);
					int size = va_arg(input, int);
					amx_GetString(text, physAddr[variables++], 0, size);
					amx_Release(amx_list.front(), params[++i]);
				}
				break;
				default:
				{
					va_arg(input, void *);
				}
				break;
				case 'l':
				{
					char * strList = va_arg(input, char *);

					int len = strlen(strList);

					i++;

					for (int j = 0; j < len; j++, i++)
					{
						switch (strList[j])
						{
						case 's':
							amx_Release(amx_list.front(), params[i + 1]);
							break;
						}
					}

					i = count;
				}
				break;
			}
		}
		va_end(input);
	}
	delete [] params;
	return value;
}

int Invoke::getAddresses()
{
	if (gotAddresses)
	{
		return 1;
	}
	AMX_HEADER * amx_hdr = (AMX_HEADER *)(amx_list.back())->base;
	std::size_t size = sizeof(PAWN::names) / sizeof(const char *);
	for (std::size_t i = 0; i < size; ++i)
	{
		amx_FindNative(amx_list.back(), PAWN::names[i], &amx_idx);
		if (amx_idx != std::numeric_limits<int>::max())
		{
			unsigned int amx_addr = (unsigned int)((AMX_FUNCSTUB *)((char *)amx_hdr + amx_hdr->natives + amx_hdr->defsize * amx_idx))->address;
			if (amx_addr)
			{
				if (amx_map.find(PAWN::names[i]) == amx_map.end())
				{
					amx_map.insert(std::make_pair(PAWN::names[i], amx_addr));
				}
			}
		}
	}
	if (amx_map.size() == size)
	{
		gotAddresses = true;
		return 1;
	}
	return 0;
}
