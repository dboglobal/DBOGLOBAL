#ifndef __MYSQLDATABASE_H
#define __MYSQLDATABASE_H

#include <mysql.h>
#include "Database.h"
#include "QueryResult.h"
#include <sstream>

struct MySQLDatabaseConnection : public DatabaseConnection
{
	MYSQL * MySql;
};


class MySQLDatabase : public Database
{
	friend class QueryThread;
	friend class AsyncQuery;
public:
	MySQLDatabase();
	virtual ~MySQLDatabase();

	virtual bool Initialize(CNtlString Hostname, unsigned int port,
		CNtlString Username, CNtlString Password, CNtlString DatabaseName,
		UINT32 ConnectionCount);

	virtual void Shutdown();

	std::string EscapeString(std::string Escape);
	void EscapeLongString(const char * str, UINT32 len, std::stringstream& out);
	std::string EscapeString(const char * esc, DatabaseConnection * con);

	bool SupportsReplaceInto() { return true; }
	bool SupportsTableLocking() { return true; }

	virtual void BeginTransaction(DatabaseConnection* conn);
	virtual void EndTransaction(DatabaseConnection* conn);
	virtual void RollbackTransaction(DatabaseConnection* conn);


protected:

	bool _HandleError(MySQLDatabaseConnection*, UINT32 ErrorNumber);
	bool _SendQuery(DatabaseConnection *con, const char* Sql, bool Self = false);

	bool _Reconnect(MySQLDatabaseConnection * conn);

	QueryResult * _StoreQueryResult(DatabaseConnection * con);
};


class MySQLQueryResult : public QueryResult
{
public:
	MySQLQueryResult(MYSQL_RES* res, UINT32 FieldCount, UINT32 RowCount);
	~MySQLQueryResult();

	bool NextRow();

protected:
	MYSQL_RES* mResult;
};



#endif