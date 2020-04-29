#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "NtlString.h"
#include "NtlQueue.h"
#include "Callback.h"
#include <queue>
#include <list>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>


struct DatabaseConnection
{
	//FastMutex Busy;
};

class QueryResult;
class QueryBuffer;

class Database
{
	friend class AsyncQuery;

public:

	Database();
	virtual ~Database();

	/************************************************************************/
	/* Virtual Functions                                                    */
	/************************************************************************/
	virtual bool Initialize(CNtlString Hostname, unsigned int port,
		CNtlString Username, CNtlString Password, CNtlString DatabaseName,
		UINT32 ConnectionCount) = 0;

	virtual void Shutdown() = 0;

	virtual QueryResult* Query(const char* QueryString, ...);
	virtual QueryResult* QueryNA(const char* QueryString);
	virtual QueryResult * FQuery(const char * QueryString, DatabaseConnection * con);
	virtual void FWaitExecute(const char * QueryString, DatabaseConnection * con);
	virtual bool WaitExecute(const char* QueryString, ...);//Wait For Request Completion
	virtual bool WaitExecuteNA(const char* QueryString);//Wait For Request Completion
	virtual bool Execute(const char* QueryString, ...);
	virtual bool ExecuteNA(const char* QueryString);

	QueryResult* TransactionQuery(const char* QueryString, ...);
	bool TransactionExecute(const char* QueryString, ...);

	inline CNtlString GetHostName() { return mHostname; }
	inline CNtlString GetDatabaseName() { return mDatabaseName; }
	inline const UINT32 GetQueueSize() { return (UINT32)m_queueQueryAfterProc.size(); }

	virtual std::string EscapeString(std::string Escape) = 0;
	virtual void EscapeLongString(const char * str, UINT32 len, std::stringstream & out) = 0;
	virtual std::string EscapeString(const char * esc, DatabaseConnection * con) = 0;

	void QueueAsyncQuery(AsyncQuery * query);
	template<class Class> bool PAsyncQuery(Class *object, void (Class::*method)(QueryResultVector&), const char *sql);
	template<class Class, typename ParamType1> bool PAsyncQuery(Class *object, void (Class::*method)(QueryResultVector&, ParamType1), ParamType1 param1, const char *sql);

	DatabaseConnection * GetFreeConnection(bool bIsSync = true);

	void PerformQueryBuffer(QueryBuffer * b);

	static Database * CreateDatabaseInterface(UINT32 uType);
	std::mutex & GetMutex() { return m_mutex; }

	virtual bool SupportsReplaceInto() = 0;
	virtual bool SupportsTableLocking() = 0;

	virtual void BeginTransaction(DatabaseConnection* conn) = 0;
	virtual void EndTransaction(DatabaseConnection* conn) = 0;
	virtual void RollbackTransaction(DatabaseConnection* conn) = 0;

protected:

	// spawn threads and shizzle
	void _Initialize();

	// actual query function
	virtual bool _SendQuery(DatabaseConnection *con, const char* Sql, bool Self) = 0;
	virtual QueryResult * _StoreQueryResult(DatabaseConnection * con) = 0;

	////////////////////////////////
	DatabaseConnection* ConnectionSync;
	DatabaseConnection* ConnectionAsync;

	UINT32 _counter;
	///////////////////////////////

	INT32 mConnectionCount;

	// For reconnecting a broken connection
	CNtlString mHostname;
	CNtlString mUsername;
	CNtlString mPassword;
	CNtlString mDatabaseName;
	UINT32 mPort;
	
public:

	static std::list<Database*> s_listDatabase;
	static std::mutex s_mutex;
	static std::condition_variable_any s_cond;
	static volatile long s_qcount;
	static volatile bool s_IsQuit;
	static std::thread* s_pThread;
	static void StartThread();
	static void DatabaseThreadRun();

public:
	void QueryTaskRun();


protected:
	void QueryThreadRun();
	std::mutex m_mutexBP;
	std::mutex m_mutexAP;
	std::mutex m_mutex;
	std::queue<AsyncQuery*> m_queueQueryBeforeProc;
	std::queue<AsyncQuery*> m_queueQueryAfterProc;

};

template<class Class>
bool
Database::PAsyncQuery(Class *object, void (Class::*method)(QueryResultVector&), const char *sql)
{
	if (!sql) return false;
	AsyncQuery * q = new AsyncQuery(new SQLClassCallbackP0<Class>(object, method));
	q->AddQuery(sql);
	this->QueueAsyncQuery(q);
	return true;
}
template<class Class, typename ParamType1>
bool
Database::PAsyncQuery(Class *object, void (Class::*method)(QueryResultVector&, ParamType1), ParamType1 param1, const char *sql)
{
	if (!sql) return false;
	AsyncQuery * q = new AsyncQuery(new SQLClassCallbackP1<Class, ParamType1>(object, method, param1));
	q->AddQuery(sql, param1);
	this->QueueAsyncQuery(q);
	return true;
}


// for security reason - gui
class scoped_sql_transaction_proc
{
public:
	scoped_sql_transaction_proc(Database* p1) : m_success(false)
	{
		m_list_db.push_back(p1);
		start();
	}
	scoped_sql_transaction_proc(Database* p1, Database* p2) : m_success(false)
	{
		m_list_db.push_back(p1);
		m_list_db.push_back(p2);
		start();
	}
	scoped_sql_transaction_proc(Database* p1, Database* p2, Database* p3) : m_success(false)
	{
		m_list_db.push_back(p1);
		m_list_db.push_back(p2);
		m_list_db.push_back(p3);
		start();
	}
	~scoped_sql_transaction_proc()
	{
		if (m_success)
		{
			for (auto it = m_list_db.begin(); it != m_list_db.end(); ++it)
			{
				(*it)->EndTransaction((*it)->GetFreeConnection());
			}
		}
		else
		{
			for (auto it = m_list_db.begin(); it != m_list_db.end(); ++it)
			{
				(*it)->RollbackTransaction((*it)->GetFreeConnection());
			}
		}
	}
	void success()
	{
		m_success = true;
	}

private:
	void start()
	{
		for (auto it = m_list_db.begin(); it != m_list_db.end(); ++it)
		{
			Database* db = *it;
			std::scoped_lock lock(db->GetMutex());
			db->BeginTransaction(db->GetFreeConnection());
		}
	}
	bool m_success;
	std::list<Database*> m_list_db;
};



#endif