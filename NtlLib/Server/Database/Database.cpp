#include "stdafx.h"
#include "Database.h"
#include "mysql.h"
#include "QueryBuffer.h"
#include "AsyncQuery.h"

#include "NtlLog.h"
#include <functional>

std::list<Database*> Database::s_listDatabase;
volatile bool Database::s_IsQuit = false;
std::thread* Database::s_pThread = nullptr;
volatile long Database::s_qcount = 0;
std::mutex Database::s_mutex;
std::condition_variable_any Database::s_cond;


SQLCallbackBase::~SQLCallbackBase()
{

}



Database::Database()
{
	_counter = 0;
	ConnectionSync = NULL;
	ConnectionAsync = NULL;
	mConnectionCount = -1;   // Not connected.
	mPort = 0;
}

Database::~Database()
{
	s_listDatabase.remove(this);

	if (s_listDatabase.size() == 0 && s_pThread)
	{
		s_IsQuit = true;
		s_pThread->join();
		delete s_pThread;
		s_pThread = NULL;
	}
}

void Database::_Initialize()
{
	s_listDatabase.push_back(this);
}


DatabaseConnection * Database::GetFreeConnection(bool bIsSync/* = true*/)
{
	UNREFERENCED_PARAMETER(bIsSync);
	//if (bIsSync)
		return ConnectionSync;
	//else
	//	return ConnectionAsync;
}

QueryResult * Database::Query(const char* QueryString, ...)
{
	//ERR_LOG(LOG_SYSTEM, "Direct Query %s", QueryString);
	char sql[4096];
	va_list vlist;
	va_start(vlist, QueryString);
	vsnprintf_s(sql, 4096, QueryString, vlist);
	va_end(vlist);

	// Send the query
	QueryResult * qResult = NULL;
	DatabaseConnection * con = GetFreeConnection();

	qResult = FQuery(sql, con);

	return qResult;
}

QueryResult * Database::QueryNA(const char* QueryString)
{
	// Send the query
	QueryResult * qResult = NULL;
	DatabaseConnection * con = GetFreeConnection();

	qResult = FQuery(QueryString, con);

	return qResult;
}

QueryResult * Database::FQuery(const char * QueryString, DatabaseConnection * con)
{
	// Send the query
	QueryResult * qResult = NULL;

	std::scoped_lock lock(m_mutex);

	if (_SendQuery(con, QueryString, false))
		qResult = _StoreQueryResult(con);

	return qResult;
}

void Database::FWaitExecute(const char * QueryString, DatabaseConnection * con)
{
	// Send the query
	std::scoped_lock lock(m_mutex);
	_SendQuery(con, QueryString, false);
}

bool Database::Execute(const char* QueryString, ...)
{
	char query[4096];
	va_list vlist;
	va_start(vlist, QueryString);
	vsnprintf_s(query, 4096, QueryString, vlist);
	va_end(vlist);
	ExecuteNA(query);
	return true;
}

bool Database::ExecuteNA(const char* QueryString)
{
	char * pBuffer = _strdup(QueryString);
	{
		std::scoped_lock lock(m_mutexBP);
		m_queueQueryBeforeProc.push((AsyncQuery*)pBuffer);
	}

	std::scoped_lock lock(s_mutex);
	++s_qcount;
	s_cond.notify_one();

	return true;
}

//this will wait for completion
bool Database::WaitExecute(const char* QueryString, ...)
{
	//ERR_LOG(LOG_SYSTEM, "Direct Query %s", QueryString);
	char sql[4096];
	va_list vlist;
	va_start(vlist, QueryString);
	vsnprintf_s(sql, 4096, QueryString, vlist);
	va_end(vlist);

	return WaitExecuteNA(sql);
}

QueryResult* Database::TransactionQuery(const char* QueryString, ...)
{
	char sql[4096];
	va_list vlist;
	va_start(vlist, QueryString);
	vsnprintf_s(sql, 4096, QueryString, vlist);
	va_end(vlist);

	// Send the query
	DatabaseConnection * con = GetFreeConnection();
	QueryResult * qResult = NULL;
	if (_SendQuery(con, sql, false))
		qResult = _StoreQueryResult(con);

	return qResult;
}

bool Database::TransactionExecute(const char* QueryString, ...)
{
	char sql[4096];
	va_list vlist;
	va_start(vlist, QueryString);
	vsnprintf_s(sql, 4096, QueryString, vlist);
	va_end(vlist);
	DatabaseConnection* con = GetFreeConnection();
	return _SendQuery(con, sql, false);
}

bool Database::WaitExecuteNA(const char* QueryString)
{
	DatabaseConnection * con = GetFreeConnection();

	std::scoped_lock lock(m_mutex);

	return _SendQuery(con, QueryString, false);
}

void Database::QueueAsyncQuery(AsyncQuery * query)
{
	query->db = this;
	{
		std::scoped_lock lock(m_mutexBP);
		m_queueQueryBeforeProc.push(query);
	}

	std::scoped_lock lock(s_mutex);
	++s_qcount;
	s_cond.notify_one();
}

void Database::QueryTaskRun()
{
	std::queue<AsyncQuery*> q;
	{
		std::scoped_lock lock(m_mutexAP);

		if (!m_queueQueryAfterProc.empty())
			std::swap(q, m_queueQueryAfterProc);
	}
	while (!q.empty())
	{
		AsyncQuery* p = q.front();
		q.pop();

		p->Callback();
	}
}


void Database::QueryThreadRun()
{
	std::queue<AsyncQuery*> q;
	{
		std::scoped_lock sl(m_mutexBP);

		if (!m_queueQueryBeforeProc.empty())
			std::swap(q, m_queueQueryBeforeProc);
		else
			return;
	}

	{
		std::scoped_lock lock(s_mutex);
		s_qcount -= (long)q.size();
	}

	std::vector<char*> v;
	while (!q.empty())
	{
		AsyncQuery* p = q.front();
		q.pop();
		if (p && p->type_recognise == 4)
		{
			if (p->Perform())
			{
				std::scoped_lock sl(m_mutexAP);
				m_queueQueryAfterProc.push(p);
			}
			else 
				v.push_back((char*)p);
		}
		else
			v.push_back((char*)p);
	}

	if (v.size() == 0)
		return;
	
	scoped_sql_transaction_proc sstp(this);
	DatabaseConnection * con = GetFreeConnection(false);
	for (size_t i = 0; i < v.size(); ++i)
	{
		char* p = v[i];
		{
			if (0 != _strnicmp(p, "select", 6))
			{
				_SendQuery(con, p, false);
			}
			else
			{
				printf("execute a select command \n");
				//assert(0 && "execute a select command");
			}
		}
		free(p);
	}
	sstp.success();
}



void Database::StartThread()
{
	if (!s_pThread)
	{
		s_IsQuit = false;
		s_pThread = new std::thread(&Database::DatabaseThreadRun);
	}
}

void Database::DatabaseThreadRun()
{
	mysql_thread_init();

	while (!s_IsQuit)
	{
		{
			std::unique_lock lock(s_mutex);
			if (!s_qcount)
			{
				auto now = std::chrono::system_clock::now();
				if (s_cond.wait_until(lock, now + std::chrono::milliseconds(10)) == std::cv_status::timeout)
					continue;
			}
		}

		std::for_each(s_listDatabase.begin(), s_listDatabase.end(), std::mem_fn(&Database::QueryThreadRun));
	}

	mysql_thread_end();
}