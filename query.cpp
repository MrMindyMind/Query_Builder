#include <iostream>
#include "query.h"
#include "plugin.h"
#include "utils.h"
#include "Invoke.h"

extern std::vector<Query*> g_queryVec;
extern logprintf_t logprintf;

Query::Query(std::string strTable, std::string strCondition, cell connectionHandle, unsigned short type) 
	: m_strCondition(strCondition), m_nConnectionHandle(connectionHandle), m_nType(type)
{
	InitQuery(type, strTable);
}

Query::Query(std::string table, cell connectionHandle) : m_nConnectionHandle(connectionHandle)
{
	Query(table, NULL, connectionHandle, _QB_QUERY_TYPE_INSERT);
}

void Query::Insert(std::string field, std::string value)
{
	static int
		fieldPos = 0,
		valuePos = 0;

	if (!field.empty())
	{
		if (!fieldPos)
		{
			fieldPos = m_strQuery.length() - 11;
			valuePos = m_strQuery.length() - 1;
		}

		field.insert(0, "`");
		field.insert(field.length(), "`");

		value.insert(0, "'");
		value.insert(value.length(), "'");

		m_strQuery.insert(fieldPos, field);
		m_strQuery.insert(fieldPos + field.length(), ",");

		fieldPos += 1 + field.length();
		valuePos += 1 + field.length();

		m_strQuery.insert(valuePos, value);
		m_strQuery.insert(valuePos + value.length(), ",");

		valuePos += value.length() + 1;
	}
	else
	{
		// Remove commas.
		m_strQuery.erase(valuePos-1, 1);
		m_strQuery.erase(fieldPos-1, 1);
	}
}

void Query::Print(void)
{
	logprintf("%s", m_strQuery.c_str());
}

void Query::Append(const std::string& entry)
{
	m_strQuery += entry;
}

void Query::Set(char* cStr)
{
	m_strQuery.assign(cStr);
}

unsigned int Query::GetQueryLen(void)
{
	return m_strQuery.length();
}

unsigned short Query::GetQueryType(void)
{
	return m_nType;
}

void Query::Update(const std::string& field, const std::string& entry)
{
	m_strQuery += '`' + field + "`='" + entry + "',";
}

void Query::AddSelect(const std::string& field)
{
	static unsigned int length = 0;
	if (field.empty())
	{
		int pos = m_strQuery.find_last_of(',');

		if (pos != std::string::npos)
			m_strQuery.erase(pos, 1);
	}
	else
	{
		m_strQuery.insert(7 + length, "`" + field + "`,");
		length += field.length() + 3;
	}
}

void Query::AddSelect(void)
{
	m_strQuery.insert(7, "*");
}

void Query::AddCondition(void)
{
	// Delete last comma on update queries.
	if (GetQueryType() == _QB_QUERY_TYPE_UPDATE)
		m_strQuery.pop_back();

	if (!m_strCondition.empty())
		m_strQuery.append(" WHERE " + m_strCondition);
}

cell Query::GetConnectionHandle(void)
{
	return m_nConnectionHandle;
}

std::string Query::GetQuery(void)
{
	return m_strQuery;
}

Query* Query::Create(cell connectionHandle, unsigned short type)
{
	Query* p_cQuery = new Query("", "", connectionHandle, type);

	if (p_cQuery != nullptr)
	{
		g_queryVec.push_back(p_cQuery);
		return p_cQuery;
	}

	return nullptr;
}

Query* Query::Create(AMX* amx, const cell& table, const cell& condition, cell connectionHandle, unsigned short type)
{
	std::string strTable;
	Utilities::GetPawnString(amx, table, strTable, 32 + 1);

	if (!strTable.empty())
	{
		std::string strCondition;
		Utilities::GetPawnString(amx, condition, strCondition, 64 + 1);

		Query* p_cQuery = new Query(strTable, strCondition, connectionHandle, type);

		if (p_cQuery != nullptr)
		{
			g_queryVec.push_back(p_cQuery);
			return p_cQuery;
		}
	}

	return nullptr;
}

bool Query::IsValidQuery(Query* query)
{
	if (query == nullptr)
		return false;

	// If it's in the vector it's valid.
	for (auto it = g_queryVec.begin(); it != g_queryVec.end(); it++)
		if (*it == query)
			return true;

	return false;
}

void Query::SetMaxLens(unsigned int fieldLen, unsigned int entryLen, unsigned int fieldsLen)
{
	Query::fieldLen		= fieldLen;
	Query::entryLen		= entryLen;
	Query::fieldsLen	= fieldsLen;
}

// Private Functions

void inline Query::InitQuery(int type, const std::string& table) {
	switch (type) 
	{
	case _QB_QUERY_TYPE_DELETE:
		m_strQuery = "DELETE FROM `" + table + "`";
		break;
	case _QB_QUERY_TYPE_INSERT:
		m_strQuery = "INSERT INTO `" + table + "` () VALUES ()";
		break;
	case _QB_QUERY_TYPE_UPDATE:
		m_strQuery = "UPDATE `" + table + "` SET ";
		break;
	case _QB_QUERY_TYPE_SELECT:
		m_strQuery = "SELECT  FROM `" + table + "`";
		break;
	}
}

// Just in case i ever need this...

	// Scan for multiple tables.

	/*unsigned int 
		index	= 0,
		tables	= 0,
		i		= 0;

	// Trim any spaces and seperate tables (by commas).
	for (auto it = strTables.begin(); it != strTables.end(); it++, i++)
	{
		if (*it == ' ')
		{
			it = strTables.erase(it);
			it--;
			i--;
		}
		else if (*it == ',')
		{
			m_strQuery.append(strTables, index, (i-index));
			m_strQuery += ("`,`");

			index = i+1;

			tables++;
		}
	}


	if (tables)
	{
		m_strQuery.append(strTables, index, (i-index));
		m_multiTables = true;
	}
	else
	{
		m_strQuery += strTables;
		m_multiTables = false;
	}

	m_strQuery += ("` SET ");*/

	/*if (m_multiTables)
	{
		if (!table.empty())
			m_strQuery.append('`' + table += "`.`" + field + "`='" + entry + "',");
		else
			m_multiTables = false;
	}

	if (!m_multiTables)
		m_strQuery.append('`' + field + "`='" + entry + "'");*/