#include "stdafx.h"

#include "MySQLDatabase.h"
#include "Field.h"

#include "NtlLog.h"


MySQLDatabase::~MySQLDatabase()
{
	ERR_LOG(LOG_SYSTEM, "MYSQLDATABASE HAS BEEN DELETED");

	if (ConnectionSync)
		delete ConnectionSync;
	if (ConnectionAsync)
		delete ConnectionAsync;
}

MySQLDatabase::MySQLDatabase() : Database()
{

}

void MySQLDatabase::BeginTransaction(DatabaseConnection* conn)
{
	_SendQuery(conn, "START TRANSACTION", false);
}

void MySQLDatabase::EndTransaction(DatabaseConnection* conn)
{
	_SendQuery(conn, "COMMIT", false);
}

void MySQLDatabase::RollbackTransaction(DatabaseConnection* conn)
{
	_SendQuery(conn, "ROLLBACK", false);
}

bool MySQLDatabase::Initialize(CNtlString Hostname, unsigned int port, CNtlString Username, CNtlString Password, CNtlString DatabaseName, UINT32 ConnectionCount)
{
	static bool is_lib_inited = false;
	MYSQL *temp, *temp2;
	my_bool my_true = true;

	mHostname = Hostname;
	mConnectionCount = ConnectionCount;
	mUsername = Username;
	mPassword = Password;
	mDatabaseName = DatabaseName;
	mPort = port;

	ERR_LOG(LOG_SYSTEM, "Connecting to `%s`, database `%s`...\n", Hostname.c_str(), DatabaseName.c_str());

	static char *server_args[] = {
		"this_program",       /* this string is not used */
	//	"--datadir=.",
		"--key_buffer_size=32M"
	};
	static char *server_groups[] = {
		"embedded",
		"server",
		"this_program_SERVER",
		(char *)NULL
	};

	is_lib_inited = true;
	if (!is_lib_inited)
	{
		int e = mysql_library_init(sizeof(server_args) / sizeof(char *), server_args, server_groups);
		if (e)
			ERR_LOG(LOG_SYSTEM, "mysql_library_init failed, error code is : %d\n", e);
		is_lib_inited = true;
	}

	///SYNC CONNECTION
	temp = mysql_init(NULL);

	if (mysql_options(temp, MYSQL_SET_CHARSET_NAME, "utf8"))
		ERR_LOG(LOG_SYSTEM, "Could not set utf8 character set.\n");

	if (mysql_options(temp, MYSQL_OPT_COMPRESS, &my_true))
		ERR_LOG(LOG_SYSTEM, "MYSQL_OPT_COMPRESS, could not be set\n");

	if (mysql_options(temp, MYSQL_OPT_RECONNECT, &my_true))
		ERR_LOG(LOG_SYSTEM, "MYSQL_OPT_RECONNECT could not be set, connection drops may occur but will be counteracted.\n");

	temp2 = mysql_real_connect(temp, Hostname.c_str(), Username.c_str(), Password.c_str(), DatabaseName.c_str(), port, NULL, 0);
	if (temp2 == NULL)
	{
		ERR_LOG(LOG_SYSTEM, "Connection failed due to: `%s`\n", mysql_error(temp));
		return false;
	}

	ConnectionSync = new MySQLDatabaseConnection;
	((MySQLDatabaseConnection*)ConnectionSync)->MySql = temp2;


	////ASYNC CONNECTION
	//temp = mysql_init( NULL );

	//if(mysql_options(temp, MYSQL_SET_CHARSET_NAME, "utf8"))
	//	ERR_LOG(LOG_SYSTEM, "Could not set utf8 character set.");

	//if (mysql_options(temp, MYSQL_OPT_RECONNECT, &my_true))
	//	ERR_LOG(LOG_SYSTEM, "MYSQL_OPT_RECONNECT could not be set, connection drops may occur but will be counteracted.");

	//temp2 = mysql_real_connect(temp, Hostname.c_str(), Username.c_str(), Password.c_str(), DatabaseName.c_str(), port, NULL, 0);
	//if( temp2 == NULL )
	//{
	//	ERR_LOG(LOG_SYSTEM, "Connection failed due to: `%s`", mysql_error(temp));
	//	return false;
	//}

	//ConnectionAsync = new MySQLDatabaseConnection;
	//((MySQLDatabaseConnection*)ConnectionAsync)->MySql = temp2;

	Database::_Initialize();
	return true;
}

std::string MySQLDatabase::EscapeString(std::string Escape)
{
	char a2[10240] = { 0 };

	DatabaseConnection * con = GetFreeConnection();
	const char * ret;
	if (mysql_real_escape_string(static_cast<MySQLDatabaseConnection*>(con)->MySql, a2, Escape.c_str(), (unsigned long)Escape.length()) == 0)
		ret = Escape.c_str();
	else
		ret = a2;

	//con->Busy.Release();
	return std::string(ret);
}

void MySQLDatabase::EscapeLongString(const char * str, UINT32 len, std::stringstream& out)
{
	char a2[10240] = { 0 };

	DatabaseConnection * con = GetFreeConnection();
	const char * ret;
	if (mysql_real_escape_string(static_cast<MySQLDatabaseConnection*>(con)->MySql, a2, str, (unsigned long)len) == 0)
		ret = str;
	else
		ret = a2;

	out.write(a2, (std::streamsize)strlen(a2));
	//con->Busy.Release();
}

std::string MySQLDatabase::EscapeString(const char * esc, DatabaseConnection * con)
{
	char a2[10240] = { 0 };
	const char * ret;
	if (mysql_real_escape_string(static_cast<MySQLDatabaseConnection*>(con)->MySql, a2, (char*)esc, (unsigned long)strlen(esc)) == 0)
		ret = esc;
	else
		ret = a2;

	return std::string(ret);
}

void MySQLDatabase::Shutdown()
{
	// TODO
}

bool MySQLDatabase::_SendQuery(DatabaseConnection *con, const char* Sql, bool Self)
{
	int result = mysql_query(static_cast<MySQLDatabaseConnection*>(con)->MySql, Sql);
	if (result > 0)
	{
		if (Self == false && _HandleError(static_cast<MySQLDatabaseConnection*>(con), mysql_errno(static_cast<MySQLDatabaseConnection*>(con)->MySql)))
		{
			// Re-send the query, the connection was successful.
			// The true on the end will prevent an endless loop here, as it will
			// stop after sending the query twice.
			result = _SendQuery(con, Sql, true);
		}
		else
			ERR_LOG(LOG_SYSTEM, "Sql query failed due to [%s], Query: [%s]", mysql_error(static_cast<MySQLDatabaseConnection*>(con)->MySql), Sql);
	}

	return (result == 0 ? true : false);
}

bool MySQLDatabase::_HandleError(MySQLDatabaseConnection * con, UINT32 ErrorNumber)
{
	// Handle errors that should cause a reconnect to the Database.
	switch (ErrorNumber)
	{
	case 2006:  // Mysql server has gone away
	case 2008:  // Client ran out of memory
	case 2013:  // Lost connection to sql server during query
	case 2055:  // Lost connection to sql server - system error
	{
					// Let's instruct a reconnect to the db when we encounter these errors.
					return _Reconnect(con);
	}break;
	}

	return false;
}

MySQLQueryResult::MySQLQueryResult(MYSQL_RES* res, UINT32 FieldCount, UINT32 RowCount) : QueryResult(FieldCount, RowCount), mResult(res)
{
	mCurrentRow = new Field[FieldCount];
}

MySQLQueryResult::~MySQLQueryResult()
{
	mysql_free_result(mResult);
	delete[] mCurrentRow;
}

bool MySQLQueryResult::NextRow()
{
	MYSQL_ROW row = mysql_fetch_row(mResult);
	if (row == NULL)
		return false;

	for (UINT32 i = 0; i < mFieldCount; ++i)
		mCurrentRow[i].SetValue(row[i]);

	return true;
}

QueryResult * MySQLDatabase::_StoreQueryResult(DatabaseConnection * con)
{
	MySQLQueryResult * res;
	MySQLDatabaseConnection * db = static_cast<MySQLDatabaseConnection*>(con);
	MYSQL_RES * pRes = mysql_store_result(db->MySql);
	UINT32 uRows = (UINT32)mysql_affected_rows(db->MySql);
	UINT32 uFields = (UINT32)mysql_field_count(db->MySql);

	if (uRows == 0 || uFields == 0 || pRes == 0)
	{
		if (pRes != NULL)
			mysql_free_result(pRes);

		return NULL;
	}

	res = new MySQLQueryResult(pRes, uFields, uRows);
	res->NextRow();

	return res;
}

bool MySQLDatabase::_Reconnect(MySQLDatabaseConnection * conn)
{
	MYSQL * temp, *temp2;

	temp = mysql_init(NULL);
	temp2 = mysql_real_connect(temp, mHostname.c_str(), mUsername.c_str(), mPassword.c_str(), mDatabaseName.c_str(), mPort, NULL, 0);
	if (temp2 == NULL)
	{
		ERR_LOG(LOG_SYSTEM, "Could not reconnect to database because of `%s`", mysql_error(temp));
		mysql_close(temp);
		return false;
	}

	if (conn->MySql != NULL)
		mysql_close(conn->MySql);

	conn->MySql = temp;

	ERR_LOG(LOG_SYSTEM, "reconnect database succeed! ip:%s", mHostname.c_str());
	return true;
}
