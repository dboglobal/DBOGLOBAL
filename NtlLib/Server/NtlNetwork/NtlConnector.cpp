//***********************************************************************************
//
//	File		:	NtlConnector.cpp
//
//	Begin		:	2005-12-14
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Connector Class
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlConnector.h"

#include "NtlNetwork.h"
#include "NtlSession.h"
#include "NtlSessionFactory.h"

#include "NtlDebug.h"

#include "Utils.h"


//---------------------------------------------------------------------------------------
// Network Monitor Thread class ( Network 클래스 내부용 )
//---------------------------------------------------------------------------------------
class CConnectorThread : public CNtlRunObject
{
public:

	CConnectorThread(CNtlConnector * pConnector) { SetArg( pConnector ); }

	void Run()
	{
		CNtlConnector * pConnector = (CNtlConnector*) GetArg();

		while( IsRunnable() ) 
		{
			if( TRUE == InterlockedCompareExchange( (LONG*)& pConnector->m_bConnected, TRUE, TRUE ) )
			{
				Wait( pConnector->m_dwProcessTime );
			}
			else
			{
				NTL_PRINT(PRINT_SYSTEM, "%s Connector Try Connect", GetName());

				int rc = pConnector->DoConnect();
				if( NTL_SUCCESS != rc )
				{
					NTL_PRINT(PRINT_SYSTEM, "%s Connector Connect Fail :%d[%s]", GetName(),  rc, NtlGetErrorMessage(rc));
					Wait( pConnector->m_dwRetryTime );
				}
				else
				{
					NTL_PRINT(PRINT_SYSTEM, "%s Connector Connect Success", GetName() );
				}
			}

		} // end of while( IsRunnable() )

	}

};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlConnector::CNtlConnector()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlConnector::~CNtlConnector()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlConnector::Init()
{
	m_pNetwork = NULL;

	m_pThread = NULL;

	m_dwNextTry = 0;

	m_dwRetryTime = 0;

	m_dwProcessTime = 0;

	m_dwTotalConnectCount = 0;

	m_bConnected = FALSE;
}


int CNtlConnector::CreateOneTry(LPCTSTR lpszConnectAddr, WORD wConnectPort, SESSIONTYPE sessionType)
{
	return Create(lpszConnectAddr, wConnectPort, sessionType, 0, 0);
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlConnector::Create(LPCTSTR lpszConnectAddr, WORD wConnectPort, SESSIONTYPE sessionType, DWORD dwRetryTime /* = 1000 */, DWORD dwProcessTime /* = 1000 */)
{
	m_connectAddr.SetSockAddr(lpszConnectAddr, htons(wConnectPort));

	m_sessionType = sessionType;

	m_dwProcessTime = dwProcessTime;

	m_dwRetryTime = dwRetryTime;


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlConnector::Destroy()
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlConnector::GetSessionHandle(HSESSION & rSession)
{
	if (m_pSession == NULL)
	{
		rSession = INVALID_HSESSION;
		return;
	}

	rSession = (HSESSION)m_pSession->GetSocket().GetRawSocket();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlConnector::CreateThread()
{
	CNtlString strName;
	strName.Format("ConnectorThread [%s:%u] Type[%u]", m_connectAddr.GetDottedAddr(), m_connectAddr.GetPort(), m_sessionType);

	m_pThread = new CConnectorThread( this );
	if ( NULL == m_pThread )
	{
		NTL_PRINT(PRINT_SYSTEM, "\"new CConnectorThread( this )\" failed.");
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	CNtlThread * pThread = tThreadFactory::Instance().CreateThread( m_pThread, strName.c_str(), true );
	if( NULL == pThread )
	{
		NTL_PRINT(PRINT_SYSTEM, "CNtlThreadFactory::CreateThread( m_pThread, strName, false ) failed.(NULL == pThread)");
		SAFE_DELETE( m_pThread );
		return NTL_ERR_NET_THREAD_CREATE_FAIL;
	}


	pThread->Start();


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlConnector::CreateThreadEx()
{
	tConnectorThreadEx::Instance().AddConnector(this);

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlConnector::DoConnect()
{
	m_pSession = m_pNetwork->GetSessionFactory()->CreateSession(m_sessionType);
	if (NULL == m_pSession)
	{
		return NTL_ERR_NET_CONNECTION_POOL_ALLOC_FAIL;
	}


	int rc = m_pSession->Create(m_pNetwork);
	if (NTL_SUCCESS != rc)
	{
		RELEASE_SESSION(m_pSession);
		m_pSession = NULL;
		return rc;
	}


	rc = m_pSession->PostConnect( this );
	if( NTL_SUCCESS != rc )
	{
		RELEASE_SESSION(m_pSession);
		m_pSession = NULL;
		return rc;
	}


	rc = m_pSession->CompleteConnect( 0 );
	if( NTL_SUCCESS != rc )
	{
		NTL_PRINT( PRINT_SYSTEM, "Session[%X] CompleteConnect Error : Err:%d(%s)", m_pSession, rc, NtlGetErrorMessage(rc) );
		m_pSession->Close(false);
		return rc;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlConnector::OnAssociated(CNtlNetwork * pNetwork)
{
	if( NULL == pNetwork )
	{
		NTL_PRINT(PRINT_SYSTEM, "(NULL == pNetwork)");
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	if( NULL != m_pNetwork )
	{
		NTL_PRINT(PRINT_SYSTEM, "(NULL != m_pNetwork) m_pNetwork = %016x", m_pNetwork);
		return NTL_ERR_SYS_OBJECT_ALREADY_CREATED;
	}


	m_pNetwork = pNetwork;


	// --- OLD
	int rc = CreateThread();
	if (NTL_SUCCESS != rc)
	{
		NTL_PRINT(PRINT_SYSTEM, "CreateThread() failed.(NTL_SUCCESS != rc) rc = %d", rc);
		return NTL_ERR_NET_THREAD_CREATE_FAIL;
	}

	// --- New -- HAS ISSUE: CREATES VERY HIGH PROCESS USAGE
	/*if (m_dwRetryTime)
	{
		int rc = CreateThreadEx();
		if (NTL_SUCCESS != rc)
		{
			NTL_PRINT(PRINT_SYSTEM, "CreateThreadEx() failed.(NTL_SUCCESS != rc) rc = %d", rc);
			return NTL_ERR_NET_THREAD_CREATE_FAIL;
		}
	}
	else
	{
		return DoConnect();
	}*/

	return NTL_SUCCESS;
}
