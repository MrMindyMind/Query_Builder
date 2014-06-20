// LICENSES:

/*
* Copyright (C) 2014 Maxim "Maxi" Glukhov.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.

CREDITS:

Incognito - Original Invoke.cpp & Invoke.h
*/

#include <vector>
#include "plugin.h"
#include "query.h"
#include "natives.h"
#include "invoke.h"

void **ppPluginData;

logprintf_t logprintf;

std::vector<Query*> g_queryVec;

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) 
{
    pAMXFunctions	= ppData[PLUGIN_DATA_AMX_EXPORTS];
    logprintf		= (logprintf_t) ppData[PLUGIN_DATA_LOGPRINTF];
	g_Invoke		= new Invoke;

    logprintf("* Query Builder plugin by Maxi loaded successfuly.");
    return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	// Delete all queries.
	for (auto it = g_queryVec.begin(); it != g_queryVec.end(); it++)
		delete *it;

	g_queryVec.clear();

    logprintf("* Query Builder plugin by Maxi unloaded successfuly.");
}

AMX_NATIVE_INFO PluginNatives[] =
{
	{"QueryBuilder_Update",			Natives::QueryBuilder_Update},
	{"QueryBuilder_Finish",			Natives::QueryBuilder_Finish},
	{"QueryBuilder_PrintQuery",		Natives::QueryBuilder_PrintQuery},
	{"QueryBuilder_UpdateString",	Natives::QueryBuilder_UpdateString},
	{"QueryBuilder_UpdateInt",		Natives::QueryBuilder_UpdateInt},
	{"QueryBuilder_UpdateFloat",	Natives::QueryBuilder_UpdateFloat},
	{"QueryBuilder_MultiUpdate",	Natives::QueryBuilder_MultiUpdate},
	{"QueryBuilder_QueryLen",		Natives::QueryBuilder_QueryLen},
	{"QueryBuilder_Insert",			Natives::QueryBuilder_Insert},
	{"QueryBuilder_Delete",			Natives::QueryBuilder_Delete},
	{"QueryBuilder_Query",			Natives::QueryBuilder_Query},
	{"QueryBuilder_Select",			Natives::QueryBuilder_Select},
	{"QueryBuilder_Build",			Natives::QueryBuilder_Build},
	{"QueryBuilder_GetQuery",		Natives::QueryBuilder_GetQuery},
	{"_QB_Invoke_GetAddresses",		Natives::_QB_Invoke_GetAddresses},
	{"_QB_SetMaxLengths",			Natives::_QB_SetMaxLengths},
    {NULL,							NULL}
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx ) 
{
	g_Invoke->amx_list.push_back(amx);

    return amx_Register(amx, PluginNatives, -1);
}


PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx ) 
{
	for (std::list<AMX *>::iterator i = g_Invoke->amx_list.begin(); i != g_Invoke->amx_list.end(); ++i)
	{
		if (* i == amx)
		{
			g_Invoke->amx_list.erase(i);
			break;
		}
	}
    return AMX_ERR_NONE;
}