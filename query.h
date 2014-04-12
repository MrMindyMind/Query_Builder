#pragma once

#define _QB_QUERY_TYPE_UPDATE		(0)
#define _QB_QUERY_TYPE_INSERT		(1)
#define _QB_QUERY_TYPE_DELETE		(2)

#include <string>

#include "plugin.h"

class Query
{
	std::string m_strQuery;
	std::string m_strCondition;

private:

	cell m_nConnectionHandle;

	unsigned short m_nType;

	void inline InitUpdateQuery(void);
	void inline InitInsertQuery(void);
	void inline InitDeleteQuery(void);

public:

	// Update
	Query(std::string strTable, std::string strCondition, cell connectionHandle, unsigned short type);

	void Update(std::string field, std::string entry);
	void AddCondition(void);

	// Insert
	Query(std::string table, cell connectionHandle);

	void Insert(std::string field, std::string value);

	void Print(void);

	cell GetConnectionHandle(void);
	unsigned int GetQueryLen(void);
	unsigned short GetQueryType(void);
	std::string GetQuery(void);
};
