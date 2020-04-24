//***********************************************************************************
//
//	File		:	NtlThread.cpp
//
//	Begin		:	2005-11-30 
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	NTL Thread Class
//
//***********************************************************************************


#include "StdAfx.h"
#include "NtlThread.h"
#include "NtlThreadException.h"
#include "NtlLog.h"

//---------------------------------------------------------------------------------------
// Key class ( Thread 클래스 내부용 )
//---------------------------------------------------------------------------------------
class CThreadKey
{
public:
	CThreadKey()
	{
		m_dwKey = ::TlsAlloc();
		if( TLS_OUT_OF_INDEXES == m_dwKey )
		{
			THROW_THREAD_EXCEPTION( eTHREAD_ERR_TLS_KEY_CREATE, GetLastError() );
		}
	}

	~CThreadKey()
	{
		if (TLS_OUT_OF_INDEXES != m_dwKey)
		{
			::TlsFree(m_dwKey);

			m_dwKey = TLS_OUT_OF_INDEXES;
		}
	}

public:
	DWORD				GetKey() { return m_dwKey; }

private:

	DWORD				m_dwKey;
};


//---------------------------------------------------------------------------------------
// MainThread class  ( Thread 클래스 내부용 ) Application의 메인 쓰래드 정보를 생성하기 위함
//---------------------------------------------------------------------------------------
class CMainRun : public CNtlRunObject
{
public:
	void Run() { /*MainRun은 호출되지 말아야 한다.*/ };
};

class CMainThread : public CNtlThread
{
public:
	CMainThread() : CNtlThread(new CMainRun, "MainThread", true ) 
	{
		m_threadID = ::GetCurrentThreadId();
		Init();
	}
};

//---------------------------------------------------------------------------------------
// Helper class  ( Thread 클래스 내부용 )
//---------------------------------------------------------------------------------------
class CThreadHelper
{
public:
	//- yoshiki : What if 1 or more of "new" operators return NULL?
	CThreadHelper()
	{
		CNtlThread::m_pThreadKey		= new CThreadKey;
		CNtlThread::m_pMainThread		= new CMainThread;
	}

	virtual ~CThreadHelper()
	{
		SAFE_DELETE( CNtlThread::m_pMainThread );
		SAFE_DELETE( CNtlThread::m_pThreadKey );
	}
};


char * s_thread_status_string[CNtlThread::MAX_STATUS] = 
{
	"STATUS_NOT_RUNNING",		// 동작하지 않는 상태 ( 초기 상태 )
	"STATUS_PREPARING_TO_RUN",	// 동작 준비 상태
	"STATUS_RUNNING",			// 동작 상태
	"STATUS_PAUSED",			// 멈춤 상태
	"STATUS_DEAD",				// 종료 상태
};

const char * CNtlThread::GetStatusString()
{
	if( m_status >= MAX_STATUS )
		return "Unknown Status";

	return s_thread_status_string[m_status];
}


//------------------------------------------------------
// static function declare
//------------------------------------------------------
/*static void ThreadCleanUp(void * arg)
{
	CNtlThread * pThread = (CNtlThread*) arg;
	pThread->CleanUp();
}*/

static unsigned __stdcall ThreadMain(void * arg)
{
	CNtlThread * pThread = (CNtlThread*) arg;
	pThread->Execute();

	return 0;
}


//---------------------------------------------------------------------------------------
// static variable declare
//---------------------------------------------------------------------------------------
CThreadHelper	threadHelper;
CThreadKey *	CNtlThread::m_pThreadKey = 0;
CNtlThread *	CNtlThread::m_pMainThread = 0;

//------------------------------------------------------


//------------------------------------------------------
// 쓰레드 Run이 실행되기 전에 1번 호출
//------------------------------------------------------
void CNtlThread::Init()
{
	InterlockedExchange((LONG*)&m_status, eSTATUS_RUNNING);

	if( !TlsSetValue( m_pThreadKey->GetKey(), (LPVOID) this ) )
	{
		THROW_THREAD_EXCEPTION( eTHREAD_ERR_TLS_SET_VALUE, GetLastError() );
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThread::Close()
{
	if( m_pRunObject )
	{
		m_pRunObject->Close();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThread::CleanUp()
{
	InterlockedExchange((LONG*)&m_status, eSTATUS_DEAD);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThread::Execute()
{
	Init();

	try
	{
		m_pRunObject->Run();
	}
	catch( CNtlThreadException e )
	{
		e.Dump();
	}
	catch(...)
	{				
	}

	CleanUp();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThread::Start()
{
	m_hThread = (HANDLE) _beginthreadex(NULL, 0, ThreadMain, this, 0, &m_threadID);

	if( !m_hThread )
	{
		THROW_THREAD_EXCEPTION(eTHREAD_ERR_THREAD_CREATE, GetLastError());
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThread::Exit()
{
	CleanUp();
	_endthreadex(0);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThread::Wait()
{
	//-- thread sleep

	m_event.Wait();

	//-- thread wakeup
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlThread::Wait(unsigned int millisecs)
{
	//-- thread sleep

	int rc = m_event.Wait( millisecs );

	//-- thread wakeup

	return rc;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThread::Notify(CNtlThread * pThread)
{
	pThread->m_event.Notify();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThread::Join()
{
	int rc = ::WaitForSingleObject( m_hThread, INFINITE );
	if( WAIT_FAILED == rc )
	{
		THROW_THREAD_EXCEPTION( eTHREAD_ERR_THREAD_JOIN, GetLastError() );
	}
	else
	{
		::CloseHandle( m_hThread );
	}

	InterlockedExchange((LONG*)&m_status, eSTATUS_NOT_RUNNING);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlThread::CNtlThread(CNtlRunObject * pRunObject, const char * name, bool bAutoDelete)
{
	// 사용 안함 상태
	InterlockedExchange((LONG*)&m_status, eSTATUS_NOT_RUNNING);

	m_bAutoDelete			= bAutoDelete;
	m_bSignaled				= false;
	m_strName				= name;

	m_pRunObject			= pRunObject;

	m_pRunObject->SetThread( this );

	// 초기화 상태
	InterlockedExchange((LONG*)&m_status, eSTATUS_PREPARING_TO_RUN);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlThread::~CNtlThread()
{
	//if( eSTATUS_NOT_RUNNING != m_status && this != CNtlThread::m_pMainThread  )
	//{
	//}

	if( m_bAutoDelete && m_pRunObject)
	{
		SAFE_DELETE( m_pRunObject );
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlThread * CNtlThread::GetCurrentThread()
{
	return (CNtlThread*) TlsGetValue( m_pThreadKey->GetKey() );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Thread 정리의 두가지 경우
// 1. 종료시에는 미사용중 빼고 모두 정리
// 2. 종료시가 아닐 경우에는 현재 DEAD 상태의 THREAD만 정리
// 주의사항!! : GarbageCollect의 경우 Lock()이 되고 들어온다.
//-----------------------------------------------------------------------------------
CNtlThreadFactory::CNtlThreadFactory()
{
	m_bClosed = false;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlThreadFactory::~CNtlThreadFactory()
{
	Shutdown();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThreadFactory::GarbageCollect(bool bShutDown)
{
	// clean thread
	CNtlThread * pThread;

	{
		CNtlLock lock(&m_Mutex);
		pThread = (CNtlThread*)m_ThreadList.GetFirst();
	}

	while( pThread )
	{
		if( pThread->IsAutoDelete() && 
			( bShutDown && !pThread->IsStatus( CNtlThread::eSTATUS_NOT_RUNNING ) ||
			 !bShutDown && pThread->IsStatus( CNtlThread::eSTATUS_DEAD ) ) )
		{
			pThread->Join();
			
			CNtlLock lock(&m_Mutex);

			pThread = (CNtlThread*) m_ThreadList.GetFirst();	// 처음부터 재정리
		}
		else
		{
			CNtlLock lock(&m_Mutex);

			pThread = (CNtlThread*) pThread->GetNext();
		}
	}

	// Exit thread
	CNtlLock lock(&m_Mutex);

	pThread = (CNtlThread*) m_ThreadList.GetFirst();
	while( pThread )
	{
		CNtlThread * pNext = (CNtlThread*) pThread->GetNext();
		if( pThread->IsStatus( CNtlThread::eSTATUS_NOT_RUNNING ) )
		{
			m_ThreadList.Remove( pThread );
			SAFE_DELETE( pThread );
		}

		pThread = pNext;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThreadFactory::SingleGarbageCollect(CNtlThread* pGarbageThread)
{
	CNtlLock lock(&m_Mutex);

	CNtlThread* pThread = (CNtlThread*)(m_ThreadList.GetFirst());
	while (NULL != pThread)
	{
		if (pGarbageThread == pThread)
		{
			if (pThread->IsStatus(CNtlThread::eSTATUS_NOT_RUNNING) ||
				pThread->IsStatus(CNtlThread::eSTATUS_PREPARING_TO_RUN))
			{
				m_ThreadList.Remove(pThread);
				SAFE_DELETE(pThread);
			}

			return;
		}

		pThread = (CNtlThread*) pThread->GetNext();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlThread * CNtlThreadFactory::CreateThread(CNtlRunObject * pRunObject, const char * name, bool bAutoDelete)
{
	GarbageCollect();

	CNtlThread * pThread = new CNtlThread( pRunObject, name, bAutoDelete );
	if (NULL == pThread)
	{
		NTL_LOG_ASSERT("\"new CNtlThread( pRunObject, name, bAutoDelete )\" failed.");
		return NULL;
	}

	CNtlLock lock(&m_Mutex);

	m_ThreadList.Append(pThread);

	return pThread;
}


void CNtlThreadFactory::AllThreadDump()
{
	int nThreadCount = 0;
	bool bIsDump = false;

	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	CNtlLock lock(&m_Mutex);

	CNtlThread * pThread = (CNtlThread*)m_ThreadList.GetFirst();
	while (pThread)
	{
		if (pThread->IsStatus(CNtlThread::eSTATUS_RUNNING))
		{
			if (nThreadCount == 0)
				bIsDump = true;

			ERR_LOG(LOG_WARNING, "Handle(%d) Thread SnapShot", pThread->m_hThread);
		//	CNtlMiniDump::ThreadSnapshot(pThread->m_hThread, &SystemTime, bIsDump);

			++nThreadCount;
			bIsDump = false;
		}
		else
		{
			ERR_LOG(LOG_WARNING, "Handle(%d) Thread skip state(%s)", pThread->m_hThread, pThread->GetStatusString());
		}

		pThread = (CNtlThread*)pThread->GetNext();
	}
}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThreadFactory::Shutdown()
{
	CloseAll();
	JoinAll();
	GarbageCollect(true);

	//Lock();

	//if( false == m_bClosed )
	//{
	//	m_bClosed = true;

	//	// 쓰레드 종료
	//	CNtlThread * pThread = (CNtlThread*) m_pThreadList->GetFirst();
	//	while( pThread )
	//	{
	//		Unlock();

	//		pThread->Close();

	//		Lock();

	//		pThread = (CNtlThread*) pThread->GetNext();
	//	}
	//}

	//GarbageCollect( true );

	//Unlock();
}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThreadFactory::CloseAll()
{
	CNtlLock lock(&m_Mutex);

	if( false == m_bClosed )
	{
		m_bClosed = true;

		// 쓰레드 종료
		CNtlThread * pThread = (CNtlThread*) m_ThreadList.GetFirst();
		while( pThread )
		{
			pThread->Close();

			pThread = (CNtlThread*) pThread->GetNext();
		}
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThreadFactory::JoinAll()
{
	CNtlThread * pThread;

	{
		CNtlLock lock(&m_Mutex);
		pThread = (CNtlThread*)m_ThreadList.GetFirst();
	}

	while( pThread )
	{
		pThread->Join();

		CNtlLock lock(&m_Mutex);
		pThread = (CNtlThread*) pThread->GetNext();
	}
}




#include <iostream>

using namespace std;
void CNtlThread::UnitTest()
{
	class CTestRun : public CNtlRunObject
	{
	public:

		CTestRun()
			:m_bRun(true) {}

		void Run()
		{
			while( m_bRun )
			{
				cout << "TestRun : " << this->GetName() << " : " << GetThread()->GetStatusString() << endl;

				Sleep( 33 );
			}
		}

		void SetRunFlag(bool bFlag) { m_bRun = bFlag; }

	private:

		bool volatile m_bRun;

	};

	try
	{
		CTestRun * pTestRun = new CTestRun;

		CNtlThread * pThread = tThreadFactory::Instance().CreateThread( pTestRun, "Test Thread");

		pThread->Start();

		Sleep(100);

		pTestRun->SetRunFlag( false );

		pThread->Join();
	}
	catch( CNtlThreadException e )
	{
		e.Dump();
	}
	catch( ... )
	{
	}
}

