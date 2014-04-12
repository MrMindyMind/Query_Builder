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

#pragma once

#include "SDK\amx\amx.h"
#include "SDK\plugincommon.h"

#include <bitset>
#include <list>
#include <map>
#include <stdarg.h>


namespace PAWN
{
	struct Native
	{
		const char * name;
		const char * data;
	};

	static const char * const names[] =
	{
		"mysql_query",
		"mysql_tquery",
		"mysql_pquery"
	};
	// a_mysql.inc (BlueG)
	static const Native mysql_query	=						{ "mysql_query",						"isb"};
	static const Native mysql_tquery =						{ "mysql_tquery",						"isslax"};
	static const Native mysql_pquery =						{ "mysql_pquery",						"isslax"};
};

class
	Invoke
{
public:
	Invoke()
	{
		gotAddresses = false;
	}


	int callNative(const PAWN::Native * native, ...);
	int getAddresses();

	int amx_idx;

	std::list<AMX *> amx_list;
private:
	bool gotAddresses;

	std::map<std::string, unsigned int> amx_map;
};

typedef int (* amx_Function_t)(AMX * amx, cell * params);

extern Invoke *g_Invoke;
