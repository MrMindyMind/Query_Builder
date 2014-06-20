#include <vector>
#include <list>
#include "plugin.h"
#include "natives.h"
#include "query.h"
#include "utils.h"
#include "invoke.h"

extern std::vector<Query*> g_queryVec;
extern logprintf_t logprintf;

unsigned int Query::fieldLen = 0;
unsigned int Query::entryLen = 0;
unsigned int Query::fieldsLen = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Select
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cell AMX_NATIVE_CALL Natives::QueryBuilder_Select(AMX* amx, cell* params)
{
	Query* p_cQuery = Query::Create(amx, params[1], params[3], params[4], _QB_QUERY_TYPE_SELECT);

	if (!Query::IsValidQuery(p_cQuery))
	{
		logprintf("QueryBuilder: Empty table passed to QueryBuilder_Select.");
		return 0;
	}

	std::string strFields;
	Utilities::GetPawnString(amx, params[2], strFields, Query::fieldsLen);
	
	if (strFields.empty())
	{
		p_cQuery->AddSelect();
	}
	else
	{
		Utilities::TrimWhitespace(strFields);
		std::list<std::string> fieldsList = Utilities::Split(strFields, ',');
		for (auto it = fieldsList.begin(); it != fieldsList.end(); it++)
		{
			if (!it->empty())
				p_cQuery->AddSelect(*it);
		}
		p_cQuery->AddSelect("");
	}

	return (cell)p_cQuery;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Delete
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cell AMX_NATIVE_CALL Natives::QueryBuilder_Delete(AMX* amx, cell* params)
{
	Query* p_cQuery = Query::Create(amx, params[1], params[2], params[3], _QB_QUERY_TYPE_DELETE);

	if (!Query::IsValidQuery(p_cQuery))
	{
		logprintf("QueryBuilder: Empty table passed to QueryBuilder_Delete.");
		return 0;
	}

	return (cell) p_cQuery;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Insert
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cell AMX_NATIVE_CALL Natives::QueryBuilder_Insert(AMX* amx, cell* params)
{
	Query* p_cQuery = Query::Create(amx, params[2], NULL, params[1], _QB_QUERY_TYPE_INSERT);

	if (!Query::IsValidQuery(p_cQuery))
	{
		logprintf("QueryBuilder: Empty table passed to QueryBuilder_Insert.");
		return 0;
	}
	else
	{
		static const unsigned int static_args = 4;

		std::string strFormat;
		Utilities::GetPawnString(amx, params[3], strFormat, Query::fieldLen);

		for (unsigned int i = 0, arg; i < strFormat.length(); i++)
		{
			arg = static_args + (i * 2);

			std::string strField;
			Utilities::GetPawnString(amx, params[arg], strField, Query::fieldLen);

			cell* p_physAddr = nullptr;
			amx_GetAddr(amx, params[arg + 1], &p_physAddr);

			switch (strFormat[i])
			{
			case 'i':
				//FALLTHROUGH
			case 'd':
				p_cQuery->Insert(strField, std::to_string(static_cast<int>(*p_physAddr)));
				break;
			case 'f':
				p_cQuery->Insert(strField, std::to_string(amx_ctof(*p_physAddr)));
				break;
			case 's':
				{
					std::string strEntry;
					Utilities::GetPawnString(amx, params[arg + 1], strEntry, Query::entryLen);

					p_cQuery->Insert(strField, strEntry);

					break;
				}
			}
		}

		// passing null field & entry to the insert method will finish the query building (but won't send it).
		p_cQuery->Insert("", "");

		return (cell) p_cQuery;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Updating
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cell AMX_NATIVE_CALL Natives::QueryBuilder_Update(AMX* amx, cell* params)
{
	Query* p_cQuery = Query::Create(amx, params[1], params[2], params[3], _QB_QUERY_TYPE_UPDATE);

	if (!Query::IsValidQuery(p_cQuery))
	{
		logprintf("QueryBuilder: Empty table passed to QueryBuilder_Update");
		return 0;
	}

	return (cell)p_cQuery;
}

cell AMX_NATIVE_CALL Natives::QueryBuilder_UpdateString(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (Query::IsValidQuery(p_cQuery))
	{
		std::string strField, strEntry;
		Utilities::GetPawnString(amx, params[2], strField, Query::fieldLen);
		Utilities::GetPawnString(amx, params[3], strEntry, Query::entryLen);
		p_cQuery->Update(strField, strEntry);
	}
	else
		logprintf("Query Builder: invalid query handle.");

	return 0;
}

cell AMX_NATIVE_CALL Natives::QueryBuilder_UpdateInt(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (Query::IsValidQuery(p_cQuery))
	{
		std::string strField;
		Utilities::GetPawnString(amx, params[2], strField, Query::fieldLen);
		p_cQuery->Update(strField, std::to_string((int)params[3]));
	}
	else
		logprintf("Query Builder: invalid query handle.");

	return 0;
}

cell AMX_NATIVE_CALL Natives::QueryBuilder_UpdateFloat(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (Query::IsValidQuery(p_cQuery))
	{
		std::string strField;
		Utilities::GetPawnString(amx, params[2], strField, Query::fieldLen);
		p_cQuery->Update(strField, std::to_string(amx_ctof(params[3])));
	}
	else
		logprintf("Query Builder: invalid query handle.");

	return 0;
}

cell AMX_NATIVE_CALL Natives::QueryBuilder_MultiUpdate(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (Query::IsValidQuery(p_cQuery))
	{
		static const unsigned int static_args = 3;

		std::string strFormat;
		Utilities::GetPawnString(amx, params[2], strFormat, Query::fieldLen);

		for (unsigned int i = 0; i < strFormat.length(); i++)
		{
			std::string strField;
			Utilities::GetPawnString(amx, params[static_args + (i * 2)], strField, Query::fieldLen);

			cell *p_physAddr = nullptr;
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
				std::string strEntry;
				Utilities::GetPawnString(amx, params[static_args + 1 + (i * 2)], strEntry, Query::entryLen);

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

	if (Query::IsValidQuery(p_cQuery))
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

		if (p_cQuery != nullptr)
		{
			// Escape the query:
			unsigned int queryLen = p_cQuery->GetQueryLen() + 1;
			char* escapedQueryCStr = new char[queryLen];

			g_Invoke->callNative
				(&PAWN::mysql_escape_string,
				p_cQuery->GetQuery().c_str(), 
				escapedQueryCStr,
				queryLen,
				p_cQuery->GetConnectionHandle(), 
				queryLen);

			p_cQuery->Set(escapedQueryCStr);

			delete[] escapedQueryCStr;

			switch (params[2])
			{
			case 0:
				// Regular unthreaded query (mysql_query)
				return g_Invoke->callNative(&PAWN::mysql_query, p_cQuery->GetConnectionHandle(), p_cQuery->GetQuery().c_str(), !!params[3]);
				break;
			case 1:
				// Threaded query (mysql_tquery)
				// FALLTHROUGH
			case 2:
				// Threaded query, parallel (mysql_pquery)

				if (size > 0)
				{
					cell* variableParams = new cell[size];

					for (int i = 0; i < size; i++)
						variableParams[i] = params[i + 6];

					std::string strCallback, strFormat;
					Utilities::GetPawnString(amx, params[4], strCallback, 33);
					Utilities::GetPawnString(amx, params[5], strFormat, 17);

					// Both natives have the EXACT same parameters.
					g_Invoke->callNative(
						(params[2] == 1) ? (&PAWN::mysql_tquery) : (&PAWN::mysql_pquery), 
						p_cQuery->GetConnectionHandle(), 
						p_cQuery->GetQuery().c_str(), 
						strCallback.c_str(), 
						strFormat.c_str(), 
						variableParams, 
						(cell) amx);

					delete[] variableParams;

					variableParams = nullptr;
				}

				break;
			}
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

	if (Query::IsValidQuery(p_cQuery))
		p_cQuery->Print();
	else
		logprintf("Query Builder: invalid query handle.");

	return 0;
}

cell AMX_NATIVE_CALL Natives::QueryBuilder_QueryLen(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (Query::IsValidQuery(p_cQuery))
		return (cell)p_cQuery->GetQueryLen();
	else
		logprintf("Query Builder: invalid query handle.");

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Free Query
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cell AMX_NATIVE_CALL Natives::QueryBuilder_Build(AMX* amx, cell* params)
{
	Query* p_cQuery = Query::Create(params[1], _QB_QUERY_TYPE_FREE);

	if (!Query::IsValidQuery(p_cQuery))
	{
		logprintf("QueryBuilder: Empty table passed to QueryBuilder_Build.");
		return 0;
	}

	return (cell)p_cQuery;
}

cell AMX_NATIVE_CALL Natives::QueryBuilder_Query(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (Query::IsValidQuery(p_cQuery))
	{
		std::string strQuery; 
		Utilities::GetPawnString(amx, params[2], strQuery, 1024 + 1);

		p_cQuery->Append(strQuery);
	}
	else
		logprintf("Query Builder: invalid query handle passed to QueryBuilder_Query.");

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Other
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// QueryBuilder_GetQuery(Query:queryhandle, dest[], maxlength = sizeof(dest))
cell AMX_NATIVE_CALL Natives::QueryBuilder_GetQuery(AMX* amx, cell* params)
{
	Query* p_cQuery = (Query*)params[1];

	if (!Query::IsValidQuery(p_cQuery))
	{
		logprintf("QueryBuilder: invalid query handle passed to QueryBuilder_GetQuery");
		return 0;
	}

	cell* amx_addr = nullptr;

	amx_GetAddr(amx, params[2], &amx_addr);
	amx_SetString(amx_addr, p_cQuery->GetQuery().c_str(), 0, 0, params[3]);

	return 0;
}

cell AMX_NATIVE_CALL Natives::_QB_SetMaxLengths(AMX* amx, cell* params)
{
	Query::SetMaxLens(params[1], params[2], params[3]);
	return 0;
}

cell AMX_NATIVE_CALL Natives::_QB_Invoke_GetAddresses(AMX* amx, cell* params)
{
    return g_Invoke->getAddresses();
}