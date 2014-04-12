#include <iostream>

#include "query.h"
#include "plugin.h"

extern logprintf_t 
    logprintf;

// Single Table
Query::Query(std::string strTable, std::string strCondition, cell connectionHandle, unsigned short type) : m_strCondition(strCondition), m_nConnectionHandle(connectionHandle), m_nType(type)
{
	switch (type)
	{
	case _QB_QUERY_TYPE_UPDATE:
		InitUpdateQuery();
		m_strQuery += (strTable + "` SET ");
		break;
	case _QB_QUERY_TYPE_DELETE:
		InitDeleteQuery();
		m_strQuery += (strTable + '`'); 
		break;
	}
}

Query::Query(std::string table, cell connectionHandle) : m_nConnectionHandle(connectionHandle)
{
	InitInsertQuery();

	m_strQuery += table + "` () VALUES ()";
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

unsigned int Query::GetQueryLen(void)
{
	return m_strQuery.length();
}

unsigned short Query::GetQueryType(void)
{
	return m_nType;
}

void Query::Update(std::string field, std::string entry)
{
	m_strQuery += '`' + field + "`='" + entry + "',";
}

void Query::AddCondition(void)
{
	// Delete last comma
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

// Private Functions

void inline Query::InitUpdateQuery(void)
{
	m_strQuery = ("UPDATE `");
}

void inline Query::InitInsertQuery(void)
{
	m_strQuery = ("INSERT INTO `");
}

void inline Query::InitDeleteQuery(void)
{
	m_strQuery = ("DELETE FROM `");
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