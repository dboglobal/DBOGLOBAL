#include "stdafx.h"
#include "stdio.h"
#include "DatabaseEnv.h"
#include "NtlLog.h"

#include "MySQLDatabase.h"

#if defined(ENABLE_DATABASE_POSTGRES)
#include "PostgresDatabase.h"
#endif

#if defined(ENABLE_DATABASE_SQLITE)
#include "SQLiteDatabase.h"
#endif

Database * Database::CreateDatabaseInterface(UINT32 uType)
{
	switch (uType)
	{
	case 1:		// MYSQL
		return new MySQLDatabase();
		break;

#if defined(ENABLE_DATABASE_POSTGRES)
	case 2:		// POSTGRES
		return new PostgresDatabase();
		break;
#endif

#if defined(ENABLE_DATABASE_SQLITE)
	case 3:		// SQLITE
		return new SQLiteDatabase();
		break;
#endif
	}

	ERR_LOG(LOG_SYSTEM,"You have attempted to connect to a database that is unsupported or nonexistant.\nCheck your config and try again.");
	abort();
	//return NULL;
}