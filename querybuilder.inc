/*

LICENSES:

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


*/

#if defined _QB_H
	#endinput
#else
	#define _QB_H
#endif

enum e_QB_QUERY_TYPE
{
	query_type_default,
	query_type_threaded,
	query_type_parallel
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Natives
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
Description: Start updating a specific table.
Parameters:
	table[] - the name of the table to update.
	condition[] - the condition that needs to met in order to execute the update (if left null will be ignored)
	connectionHandle - on which connection to execute the query (default: 1)
*/
native Query:QueryBuilder_Update(table[], condition[] = "", connectionHandle = 1);

/*
Description: Update a string
Parameters:
	Query:queryhandle - which query to update on.
	field[] - filed to update
	str[] - the new string to apply
*/
native Query:QueryBuilder_UpdateString(Query:queryhandle, field[], str[]);

/*
Description: Update an int value
Parameters:
	Query:queryhandle - which query to update on.
	field[] - filed to update
	value - value to apply.
*/
native Query:QueryBuilder_UpdateInt(Query:queryhandle, field[], value);

/*
Description: Update a float value
Parameters:
	Query:queryhandle - which query to update on.
	field[] - filed to update
	Float:value - value to apply.
*/
native Query:QueryBuilder_UpdateFloat(Query:queryhandle, field[], Float:value);

/*
Description: Finish updating.
Parameters:
	Query:queryhandle - which query to send.
	query_type - type of query: default (non-threaded), threaded, parallel (threaded)
	bool: use_cache - enable cache or not (only relevant when using default query)
	callback[] - callback to pass the result to (only relevant when using threaded/parallel query)
	format[] - variable parameters format (only relevant when using threaded/parallel query)
	{Float,_}:... - variable parameters to pass to callback (only relevant when using threaded/parallel query)
*/
native Cache:QueryBuilder_Finish(Query:queryhandle, e_QB_QUERY_TYPE: query_type, bool: use_cache = true, callback[] = "", format[] = "", {Float,_}:...);

/*
Description: Update multiple fields at once.
Parameters:
	Query:queryhandle - which query to send.
	fmat[] - query format
	    - 'i', 'd' - integers.
	    - 'f' - floats.
	    - 's' - strings.
	{Float,_}:... - arguments to pass, must be in a key-value pair (field[], {Float,_}: vlaue)
*/
native QueryBuilder_MultiUpdate(Query:query, fmat[], {Float,_}:...);

/*
Description: Get length of current query that is being built.
Parameters:
	Query:queryhandle - query handle.
*/
native QueryBuilder_QueryLen(Query:query);

/*
Description: Insert a new row to a table.
Parameters:
	connectionHandle - connection handle to run the query on.
	table[] - table to insert to.
	fmat[] - insert format
	    - 'i', 'd' - integers.
	    - 'f' - floats.
	    - 's' - strings.
    {Float,_}:... - arguments to pass, must be in a key-value pair (field[], {Float,_}: vlaue)
*/
native Query:QueryBuilder_Insert(connectionHandle, table[], fmat[], {Float,_}:...);

/*
Description: Delete from table.
Parameters:
	table[] - table to delete or delete from.
	condition[] - condition for deleting.
	connectionHandle - connection handle to run the query on.
	
WARNING: not specifying condition WILL DELETE THE ENTIRE TABLE!
*/
native Query:QueryBuilder_Delete(table[], condition[] = "", connectionHandle = 1);

/*
Description: Print query
Parameters:
	Query:queryhandle - query to print
*/
native QueryBuilder_PrintQuery(Query:query);

/*
Description: Get native addresses, internal use.. do not use.
*/

native _QB_Invoke_GetAddresses();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Invoke & Hooking
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

forward _QB_Invoke();
public _QB_Invoke()
{
	// a_mysql
	mysql_query(0, "", false);
	mysql_tquery(0, "", "", "");
	mysql_pquery(0, "", "", "");

    _QB_Invoke_GetAddresses();

	return 1;
}

public OnGameModeInit()
{
	_QB_Invoke();
	
    #if defined _QB_INVOKE_OnGameModeInit
        _QB_INVOKE_OnGameModeInit();
    #endif
    return 1;
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif

#define OnGameModeInit _QB_INVOKE_OnGameModeInit
#if defined _QB_INVOKE_OnGameModeInit
    forward _QB_INVOKE_OnGameModeInit();
#endif
