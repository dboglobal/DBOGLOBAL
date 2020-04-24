#include "stdafx.h"
#include "AsyncQuery.h"
#include "Database.h"
#include "QueryResult.h"

#include "NtlLog.h"

void AsyncQuery::AddQuery(const char * format, ...)
{
	AsyncQueryResult res;
	va_list ap;
	char buffer[4096];
	va_start(ap, format);
	vsnprintf_s(buffer, 4096, format, ap);
	va_end(ap);
	res.query = _strdup(buffer);
	res.result = NULL;
	queries.push_back(res);
}

bool AsyncQuery::Perform()
{
	if (db)
	{
		DatabaseConnection * conn = db->GetFreeConnection(false);

		for (std::vector<AsyncQueryResult>::iterator itr = queries.begin(); itr != queries.end(); ++itr)
		{
			itr->result = db->FQuery(itr->query, conn);
		}

		return true;
	}
	ERR_LOG(LOG_SYSTEM, "db == NULL ! type_recognise: %c", type_recognise);
	ERR_LOG(LOG_SYSTEM, "query: %s \n", (char*)this);
	return false;
}

void AsyncQuery::Callback()
{
	try
	{
		func->run(queries);
	}
	catch (...)
	{

	}
	delete this;
}

AsyncQuery::~AsyncQuery()
{
	delete func;
	for (std::vector<AsyncQueryResult>::iterator itr = queries.begin(); itr != queries.end(); ++itr)
	{
		if (itr->result)
			delete itr->result;

		free(itr->query);
	}
}



