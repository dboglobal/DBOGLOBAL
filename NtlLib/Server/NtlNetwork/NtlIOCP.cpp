//***********************************************************************************
//
//	File		:	NtlIOCP.cpp
//
//	Begin		:	2005-12-13
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Windows I/O Completeion Port Class
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlIOCP.h"

#include "NtlNetwork.h"
#include "NtlSession.h"

#include "NtlError.h"
#include "NtlLog.h"
#include "NtlThread.h"


//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
const ULONG_PTR THREAD_CLOSE = (ULONG_PTR)(-1);	// thread terminate value
//---------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------
// Worker Thread class ( IOCP inner class )
//---------------------------------------------------------------------------------------
class CIocpWorkerThread : public CNtlRunObject
{
public:

	CIocpWorkerThread(CNtlIocp * pIOCP) { SetArg( pIOCP ); }


	virtual void Run()
	{
		CNtlIocp * pIOCP = (CNtlIocp *) GetArg();

		int rc = 0;
		BOOL bResult = FALSE;
		DWORD dwBytesTransferred = 0;


		while( IsRunnable() )
		{
			CNtlSession * pSession = NULL;
			sIOCONTEXT * pIOContext = NULL;

			bResult = GetQueuedCompletionStatus( pIOCP->m_hIOCP,
												&dwBytesTransferred,
												(ULONG_PTR*) &pSession,
												(LPOVERLAPPED*) &pIOContext,
												INFINITE );


			if (THREAD_CLOSE == (ULONG_PTR)pSession)
			{
				NTL_PRINT(PRINT_SYSTEM, "Thread Close");
				return;
			}

			if (NULL == pIOContext)
			{
				NTL_PRINT(PRINT_SYSTEM, "NULL == pIOContext");
				continue;
			}

			pSession = (CNtlSession*)pIOContext->param;
			if (NULL == pSession)
			{
				NTL_PRINT(PRINT_SYSTEM, "pIOContext->param is NULL.(NULL == pSession)");
				continue;
			}


			if (FALSE == bResult)
			{
				rc = GetLastError();
				//NTL_PRINT(PRINT_SYSTEM, "Session[%X] GQCS Error : Err:%d(%s)", pSession, rc, NtlGetErrorMessage(rc));
				pSession->Close(false);
			}
			else
			{
				rc = pSession->CompleteIO(pIOContext, dwBytesTransferred);
				if (NTL_SUCCESS != rc)
				{
					//	NTL_PRINT( PRINT_SYSTEM, "Session[%X] CompleteIO Error : Err:%d(%s)", pSession, rc, NtlGetErrorMessage(rc) );
					pSession->Close(false);
				}
			}


			// Close상태로 변경되고 바로 Session이 Shutdown으로 변경 될 수 있으므로 위치를 이전
			pSession->DecreasePostIoCount();

		} // end while(1)
	}


	virtual void Close()
	{
		CNtlIocp * pIocp = (CNtlIocp*) GetArg();
		if( pIocp )
		{
			PostQueuedCompletionStatus( pIocp->m_hIOCP, 0, THREAD_CLOSE, NULL );
		}

		CNtlRunObject::Close();
	}
};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlIocp::CNtlIocp()
:
m_hIOCP( INVALID_HANDLE_VALUE ),
m_nCreatedThreads( 0 ),
m_pNetworkRef( NULL )
{	
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlIocp::~CNtlIocp()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlIocp::Create(CNtlNetwork * pNetwork, int nCreateThreads, int nConcurrentThreads)
{
	if( NULL == pNetwork || NULL != m_pNetworkRef )
	{
		NTL_PRINT(PRINT_SYSTEM, "(NULL == pNetwork || NULL != m_pNetworkRef) m_pNetworkRef = %016x", m_pNetworkRef);
		return NTL_FAIL;
	}


	m_pNetworkRef = pNetwork;

	if( 0 == nCreateThreads )
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		// The default number of threads ( 2 * number of processors + 2  )
		nCreateThreads = 2 * si.dwNumberOfProcessors + 2;
	}


	int rc = CreateIOCP( nConcurrentThreads );
	if( NTL_SUCCESS != rc )
	{
		NTL_PRINT(PRINT_SYSTEM, "CreateIOCP( nConcurrentThreads ) failed.(NTL_SUCCESS != rc) nConcurrentThreads = %d, rc = %d", nConcurrentThreads, rc);
		return rc;
	}


	rc = CreateThreads( nCreateThreads );
	if( NTL_SUCCESS != rc )
	{
		NTL_PRINT(PRINT_SYSTEM, "CreateThreads( nCreateThreads ) failed.(NTL_SUCCESS != rc) nCreateThreads = %d, rc = %d", nCreateThreads, rc);
		return rc;
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlIocp::Destroy()
{
	if (m_hIOCP != INVALID_HANDLE_VALUE)
	{
		CloseThreads();
		::CloseHandle(m_hIOCP);
		m_hIOCP = INVALID_HANDLE_VALUE;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlIocp::CreateIOCP(int nConcurrentThreads)
{
	if( INVALID_HANDLE_VALUE != m_hIOCP )
	{
		NTL_PRINT(PRINT_SYSTEM, "(INVALID_HANDLE_VALUE != m_hIOCP) m_hIOCP = %016x", m_hIOCP);
		return NTL_FAIL;
	}

	m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, NULL, nConcurrentThreads );
	if( NULL == m_hIOCP )
	{
		return GetLastError();
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlIocp::CreateThreads(int nOpenThreads)
{
	if( 0 == nOpenThreads )
	{
		NTL_PRINT(PRINT_SYSTEM, "(0 == nOpenThreads)");
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	for( int i = 0; i < nOpenThreads; i++ )
	{
		CNtlString strName;
		strName.Format("IOCP Worker[%03d]", i);

		CIocpWorkerThread* pIOCPWorker = new CIocpWorkerThread( this );
		if (NULL == pIOCPWorker)
		{
			NTL_PRINT(PRINT_SYSTEM, "\"new CIocpWorkerThread( this )\" failed.");
			return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
		}

		CNtlThread * pThread = tThreadFactory::Instance().CreateThread( pIOCPWorker, strName.c_str(), true );
		if( NULL == pThread )
		{
			NTL_PRINT(PRINT_SYSTEM, "CNtlThreadFactory::CreateThread( pIOCPWorker, strName, true ) failed.(NULL == pThread)");
			CloseThreads();
			SAFE_DELETE( pIOCPWorker );
			return NTL_ERR_THREAD_CREATE_FAIL;
		}

		m_lstWorkers.push_back(pThread);
		pThread->Start();

		m_nCreatedThreads++;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlIocp::CloseThreads()
{
	for( int i = 0; i < m_nCreatedThreads; i++ )
	{
		PostQueuedCompletionStatus( m_hIOCP, 0, THREAD_CLOSE, NULL );
	}

	for (std::list<CNtlThread*>::iterator it = m_lstWorkers.begin(); it != m_lstWorkers.end(); it++)
	{
		CNtlThread* pGarbageThread = *it;
		if (pGarbageThread)
		{
			pGarbageThread->Join();
			tThreadFactory::Instance().SingleGarbageCollect(pGarbageThread);
		}
	}

	m_lstWorkers.clear();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlIocp::Associate(SOCKET hSock, LPCVOID pCompletionKey)
{
	if( NULL == m_hIOCP )
	{
		NTL_PRINT(PRINT_SYSTEM, "(NULL == m_hIOCP)");
		return NTL_ERR_NET_INVALID_COMPLETE_IO_HANDLE;
	}


	HANDLE handle = CreateIoCompletionPort( (HANDLE)hSock, m_hIOCP, (ULONG_PTR)pCompletionKey, 0 );
	if( NULL == handle )
	{
		NTL_PRINT(PRINT_SYSTEM, "I/O completion port couldn't be created.(NULL == handle) hSock = %016x, m_hIOCP = %016x, pCompletionKey = %016x", hSock, m_hIOCP, pCompletionKey);
		return GetLastError();
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlIocp::PostIOCPEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_hIOCP == NULL)
	{
		NTL_PRINT(PRINT_SYSTEM, "(NULL == m_hIOCP)");
		return NTL_ERR_NET_INVALID_COMPLETE_IO_HANDLE;
	}

	if (PostQueuedCompletionStatus(m_hIOCP, 0, wParam, (OVERLAPPED*)lParam) == FALSE)
		return GetLastError();

	return NTL_SUCCESS;
}
