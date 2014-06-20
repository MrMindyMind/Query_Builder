#ifndef _H_NATIVES
#define _H_NATIVES

#include "plugin.h"

namespace Natives
{
	cell AMX_NATIVE_CALL QueryBuilder_Update		(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL QueryBuilder_Finish		(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL QueryBuilder_PrintQuery	(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL QueryBuilder_QueryLen		(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL QueryBuilder_UpdateString	(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL QueryBuilder_UpdateInt		(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL QueryBuilder_UpdateFloat	(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL QueryBuilder_MultiUpdate	(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL QueryBuilder_Insert		(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL QueryBuilder_Delete		(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL QueryBuilder_Build			(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL QueryBuilder_Query			(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL QueryBuilder_Select		(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL QueryBuilder_GetQuery		(AMX* amx, cell* params); // TODO: IMPLEMENT THIS!
	cell AMX_NATIVE_CALL _QB_Invoke_GetAddresses	(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL _QB_SetMaxLengths			(AMX* amx, cell* params);
}

#endif