#include <vector>

#include "plugin.h"
#include "natives.h"
#include "query.h"
#include "utils.h"
#include "invoke.h"

extern std::vector<Query*>
	g_queryVec;

extern logprintf_t 
    logprintf;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Delete
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cell AMX_NATIVE_CALL Natives::QueryBuilder_Delete(AMX* amx, cell* params)
{
	std::string
		strTable;

	Utilities::GetPawnString(amx, params[1], strTable);

	if (!strTable.empty())
	{
		std::string
			strCondition;

		Utilities::GetPawnString(amx, params[2], strCondition);

		Query* cQuery = new Query(strTable, strCondition, params[3], _QB_QUERY_TYPE_DELETE);

		if (cQuery != nullptr)
		{
			g_queryVec.push_back(cQuery);

			return (cell) cQuery;
		}
	}
	else
		logprintf("QueryBuilder: Empty table passed to QueryBuilder_Delete.");

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Insert
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cell AMX_NATIVE_CALL Natives::QueryBuilder_Insert(AMX* amx, cell* params)
{
	std::string
		strTable;

	Utilities::GetPawnString(amx, params[2], strTable);

	if (!strTable.empty())
	{
		Query* cQuery = new Query(strTable, params[1]);

		g_queryVec.push_back(cQuery);

		if (cQuery != nullptr)
		{
			std::string
				strFormat,
				strField;

			static const unsigned int
				static_args = 4;

			Utilities::GetPawnString(amx, params[3], strFormat);

			for (unsigned int i = 0; i < strFormat.length(); i++)
			{
				Utilities::GetPawnString(amx, params[static_args + (i * 2)], strField);

				cell
					*p_physAddr	= nullptr;

				amx_GetAddr(amx, params[static_args + (i * 2) + 1], &p_physAddr);

				switch (strFormat[i])
				{
				case 'i':
					//FALLTHROUGH
				case 'd':
					cQuery->Insert(strField, std::to_string(static_cast<int>(*p_physAddr)));
					break;
				case 'f':
					cQuery->Insert(strField, std::to_string(amx_ctof(*p_physAddr)));
					break;
				case 's':
					{
						std::string
							strEntry;

						Utilities::GetPawnString(amx, params[static_args + (i * 2) + 1], strEntry);

						cQuery->Insert(strField, strEntry);

						break;
					}
				}
			}

			cQuery->Insert("", "");

			return (cell) cQuery;
		}
	}
	else
		logprintf("QueryBuilder: Empty table passed to QueryBuilder_Insert");

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Updating
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cell AMX_NATIVE_CALL Natives::QueryBuilder_Update(AMX* amx, cell* params)
{
	std::string 
		strTable;

	Utilities::GetPawnString(amx, params[1], strTable);

	if (!strTable.empty())
	{
		std::string
			strCondition;

		Utilities::GetPawnString(amx, params[2], strCondition);

		Query* cQuery = new Query(strTable, strCondition, params[3], _QB_QUERY_TYPE_UPDATE);

		if (cQuery != nullptr)
		{
			g_queryVec.push_back(cQuery);

			return (cell) cQuery;
		}
	}
	else
		logprintf("QueryBuilder: Empty table passed to QueryBuilder_Update");

	return 0;
}

cell AMX_NATIVE_CALL Natives::QueryBuilder_UpdateString(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (p_cQuery != nullptr)
	{
		std::string 
			strField,
			strEntry;

		Utilities::GetPawnString(amx, params[2], strField);
		Utilities::GetPawnString(amx, params[3], strEntry);

		p_cQuery->Update(strField, strEntry);
	}
	else
		logprintf("Query Builder: invalid query handle passed to QueryBuilder_UpdateString.");

	return 0;
}

cell AMX_NATIVE_CALL Natives::QueryBuilder_UpdateInt(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (p_cQuery != nullptr)
	{
		std::string 
			strField;

		Utilities::GetPawnString(amx, params[2], strField);

		p_cQuery->Update(strField, std::to_string(static_cast<int>(params[3])));
	}
	else
		logprintf("Query Builder: invalid query handle.");

	return 0;
}

cell AMX_NATIVE_CALL Natives::QueryBuilder_UpdateFloat(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (p_cQuery != nullptr)
	{
		std::string 
			strField;

		Utilities::GetPawnString(amx, params[2], strField);

		p_cQuery->Update(strField, std::to_string(amx_ctof(params[3])));
	}
	else
		logprintf("Query Builder: invalid query handle.");

	return 0;
}

cell AMX_NATIVE_CALL Natives::QueryBuilder_MultiUpdate(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (p_cQuery != nullptr)
	{
		static const unsigned int
			static_args = 3;

		std::string
			strFormat;

		Utilities::GetPawnString(amx, params[2], strFormat);

		for (unsigned int i = 0; i < strFormat.length(); i++)
		{
			std::string
				strField;

			Utilities::GetPawnString(amx, params[static_args + (i * 2)], strField);

			cell
				*p_physAddr	= nullptr;

			amx_GetAddr(amx, params[static_args + (i * 2) + 1], &p_physAddr);

			switch (strFormat[i])
			{
			case 'i':
				// FALLTHROUGH
			case 'd':
				p_cQuery->Update(strField, std::to_string(static_cast<int>(*p_physAddr)));
				break;
			case 'f':
				p_cQuery->Update(strField, std::to_string(amx_ctof(*p_physAddr)));
				break;
			case 's':
				std::string
					strEntry;

				Utilities::GetPawnString(amx, params[static_args + 1 + (i * 2)], strEntry);

				p_cQuery->Update(strField, strEntry);
				break;
			}
		}
	}
	else
		logprintf("Query Builder: invalid query handle.");

	return 0;
}

cell AMX_NATIVE_CALL Natives::QueryBuilder_Finish(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (p_cQuery != nullptr)
	{
		switch (p_cQuery->GetQueryType())
		{
		case _QB_QUERY_TYPE_DELETE:
			// FALLTHROUGH
		case _QB_QUERY_TYPE_UPDATE:
			p_cQuery->AddCondition();
			break;
		}

		// Size should hold all variable parameters excluding the constant ones.
		int size = params[0]/sizeof(cell) - 5;

		switch (params[2])
		{
		case 0:
			// Regular unthreaded query (mysql_query)
			return g_Invoke->callNative(&PAWN::mysql_query, p_cQuery->GetConnectionHandle(), (p_cQuery->GetQuery()).c_str(), !!params[3]);
			break;
		case 1:
			// Threaded query (mysql_tquery)
			// FALLTHROUGH
		case 2:
			// Threaded query, parallel (mysql_pquery)

			std::string
				strCallback,
				strFormat;

			Utilities::GetPawnString(amx, params[4], strCallback);
			Utilities::GetPawnString(amx, params[5], strFormat);

			if (size > 0)
			{
				cell* variableParams = new cell[size];

				for (int i = 0; i < size; i++)
					variableParams[i] = params[i + 6];

				// Both natives have the EXACT same parameters.
				g_Invoke->callNative((params[2] == 1) ? (&PAWN::mysql_tquery) : (&PAWN::mysql_pquery), p_cQuery->GetConnectionHandle(), (p_cQuery->GetQuery()).c_str(), strCallback.c_str(), strFormat.c_str(), variableParams, (cell) amx);

				delete[] variableParams;

				variableParams = nullptr;
			}
			else
				g_Invoke->callNative((params[2] == 1) ? (&PAWN::mysql_tquery) : (&PAWN::mysql_pquery), p_cQuery->GetConnectionHandle(), (p_cQuery->GetQuery()).c_str(), strCallback.c_str(), strFormat.c_str(), nullptr, (cell) amx);

			break;
		}

		for (auto it = g_queryVec.begin(); it != g_queryVec.end(); it++)
		{
			if (*it == p_cQuery)
			{
				delete *it;

				g_queryVec.erase(it);

				break;
			}

		}
	}
	else
		logprintf("Query Builder: invalid query handle.");

	return 0;
}

cell AMX_NATIVE_CALL Natives::QueryBuilder_PrintQuery(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (p_cQuery != nullptr)
		p_cQuery->Print();
	else
		logprintf("Query Builder: invalid query handle.");

	return 0;
}

cell AMX_NATIVE_CALL Natives::QueryBuilder_QueryLen(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (p_cQuery != nullptr)
		return (cell)p_cQuery->GetQueryLen();
	else
		logprintf("Query Builder: invalid query handle.");

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Other
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cell AMX_NATIVE_CALL Natives::_QB_Invoke_GetAddresses(AMX* amx, cell* params)
{
    return g_Invoke->getAddresses();
}