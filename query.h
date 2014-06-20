#pragma once

#define _QB_QUERY_TYPE_UPDATE		(0)
#define _QB_QUERY_TYPE_INSERT		(1)
#define _QB_QUERY_TYPE_DELETE		(2)
#define _QB_QUERY_TYPE_SELECT		(3)
#define _QB_QUERY_TYPE_FREE			(4)

#include <string>
#include "plugin.h"

class Query
{
public:

	/*
	* constructors
	*/
	Query(std::string strTable, std::string strCondition, cell connectionHandle, unsigned short type);

	/*
	* Specific to insert queries.
	*/
	Query(std::string table, cell connectionHandle);

	/*
	* setters
	*/
	void Set(char* cStr);
	void Update(const std::string& field, const std::string& entry);
	void Insert(std::string field, std::string value);
	void AddSelect(const std::string& field);
	void AddSelect(void);
	void AddCondition(void);
	void Append(const std::string& entry);

	/*
	* getters
	*/
	cell GetConnectionHandle(void);
	unsigned int GetQueryLen(void);
	unsigned short GetQueryType(void);
	std::string GetQuery(void);

	/*
	* other
	*/
	void Print(void);

	/*
	* static methods
	*/
	static Query* Create(AMX* amx, const cell& table, const cell& condition, cell connectionHandle, unsigned short type);
	static Query* Create(cell connectionHandle, unsigned short type);
	static void SetMaxLens(unsigned int fieldLen, unsigned int entryLen, unsigned int fieldsLen);
	static bool IsValidQuery(Query* query);

	/*
	* static public members
	*/
	static unsigned int fieldLen;
	static unsigned int entryLen;
	static unsigned int fieldsLen;

private:

	cell m_nConnectionHandle;
	unsigned short m_nType;
	std::string m_strQuery;
	std::string m_strCondition;

	/*
	*private methods
	*/
	void inline InitQuery(int type, const std::string& table);

};
