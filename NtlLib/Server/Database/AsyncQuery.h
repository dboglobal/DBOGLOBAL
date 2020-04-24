#ifndef _ASYNCQUERY_H
#define _ASYNCQUERY_H

#include <vector>

class QueryResult;
class SQLCallbackBase;

struct AsyncQueryResult
{
	QueryResult * result;
	char * query;
};


class AsyncQuery
{
	friend class Database;

	char type_recognise;
	SQLCallbackBase * func;
	std::vector<AsyncQueryResult> queries;
	Database * db;

public:
	AsyncQuery(SQLCallbackBase * f) : func(f), type_recognise(4), db(NULL) {}
	~AsyncQuery();
	void AddQuery(const char * format, ...);
	bool Perform();
	void Callback();
	inline void SetDB(Database * dbb) { db = dbb; }
};

#endif