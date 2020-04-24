//***********************************************************************************
//
//	File		:	NtlAcceptor.cpp
//
//	Begin		:	2005-12-13
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Network 접속을 받아들이는 Acceptor 클래스
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlAcceptor.h"

#include "NtlSession.h"
#include "NtlSessionList.h"
#include "NtlSessionFactory.h"
#include "NtlNetwork.h"

#include "NtlLog.h"
#include "NtlError.h"
#include "NtlThread.h"

#include "NtlAcceptingSessionList.h"


//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
const unsigned int ACCEPT_RESERVE_CHECK_TIME = 5000;
//---------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------
// Acceptor Running class ( Network 클래스 내부용 )
//---------------------------------------------------------------------------------------
class CAcceptorThread : public CNtlRunObject
{
public:

	CAcceptorThread(CNtlAcceptor * pAcceptor) { SetArg( pAcceptor ); }

	void Run()
	{
		CNtlAcceptor * pAcceptor = (CNtlAcceptor*) GetArg();


		int rc = pAcceptor->ReserveAccept( pAcceptor->m_nCreateAcceptCount );
		if( NTL_SUCCESS != rc )
		{
			ERR_LOG(LOG_NETWORK, "%s Precreate ReserveAccept Fail :%d[%s]", GetName(), rc, NtlGetErrorMessage(rc));
		}
		else
		{
			NTL_PRINT(1, "%s Precreate ReserveAccept Success : Current[%d]", GetName(), pAcceptor->m_nAcceptingCount);
		}


		while( IsRunnable() )
		{	
			Wait( ACCEPT_RESERVE_CHECK_TIME );

			//Here new connections are created

			int nAcceptCount = pAcceptor->GetReserveAcceptCount();
			if( nAcceptCount > 0 )
			{
				rc = pAcceptor->ReserveAccept( nAcceptCount );
				if( NTL_SUCCESS != rc )
				{
					ERR_LOG(LOG_NETWORK, "%s ReserveAccept Fail :%d[%s]", GetName(), rc, NtlGetErrorMessage(rc));
				}
				else
				{
				//	ERR_LOG(LOG_NETWORK, "%s ReserveAccept Success : ReserveCount[%d] Accepting[%d] Accepted[%u]", GetName(), nAcceptCount, pAcceptor->m_nAcceptingCount, pAcceptor->m_nAcceptedCount);
				}
			}

		} // end of while(1)

	} // end of Run()

};



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlAcceptor::CNtlAcceptor()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlAcceptor::~CNtlAcceptor()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlAcceptor::Init()
{
	m_pNetwork = NULL;

	m_pThread = NULL;

	m_pArrListenSocket = NULL;

	m_nMaxAcceptCount = 0;

	m_nMinAcceptCount = 0;

	m_nPostAcceptCount = 0;

	m_nCreateAcceptCount = 0;


	m_nAcceptingCount = 0;

	m_nAcceptedCount = 0;

	m_dwTotalAcceptCount = 0;

	m_pAcceptingSessionList = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlAcceptor::Create(const char * lpszAddr, unsigned short nBasePort, unsigned short nRange, unsigned short nSelectedPort, SESSIONTYPE sessionType, int nMaxAcceptCount, int nPostAcceptCount, int nMinAcceptCount, int nCreateAcceptCount)
{
	if( nCreateAcceptCount > nMaxAcceptCount )
	{
		NTL_PRINT(PRINT_SYSTEM, "nCreateAcceptCount > nMaxAcceptCount, nCreateAcceptCount = %d, nMaxAcceptCount = %d", nCreateAcceptCount, nMaxAcceptCount);
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	if( NULL != m_pNetwork )
	{
		NTL_PRINT(PRINT_SYSTEM, "NULL != m_pNetwork, m_pNetwork = %016x", m_pNetwork);
		return NTL_ERR_SYS_OBJECT_ALREADY_CREATED;
	}

	m_pArrListenSocket = new CNtlSocket[nRange];
	if (m_pArrListenSocket == NULL)
	{
		NTL_PRINT(PRINT_SYSTEM, "m_pArrListenSocket == NULL");
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	m_nPortBase = nBasePort;

	m_nListenSocketSize = nRange;

	if (nSelectedPort > nBasePort)
	{
		NTL_PRINT(PRINT_SYSTEM, "nSelectedPort > nBasePort, nSelectedPort = %d, nBasePort = %d", nSelectedPort, nBasePort);
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}

	m_idxSelectedSocket = nSelectedPort - nBasePort;

	m_BitFlagManager.Create((DWORD)nRange);

	m_sessionType = sessionType;

	m_nMaxAcceptCount = nMaxAcceptCount;

	m_nPostAcceptCount = nPostAcceptCount / 20;
	if (m_nPostAcceptCount <= 0)
		m_nPostAcceptCount = 1;

	m_nMinAcceptCount = m_nPostAcceptCount;

	m_nCreateAcceptCount = m_nPostAcceptCount;

	m_listenAddr.SetSockAddr( lpszAddr, htons(nSelectedPort) );


	int rc = Listen();
	if( NTL_SUCCESS != rc )
	{
		NTL_PRINT(PRINT_SYSTEM, "Listen() failed.(NTL_SUCCESS != rc), rc = %d", rc);
		return rc;
	}


	return NTL_SUCCESS;
}


void CNtlAcceptor::ChangeListenPort(unsigned short sDestPort)
{
	m_BitFlagManager.Unset(m_idxSelectedSocket);

	if (m_pAcceptingSessionList)
		m_pAcceptingSessionList->ValidCheck(0);

	m_idxSelectedSocket = sDestPort - m_nPortBase;

	GetListenAddr().SetPort(sDestPort);

	m_BitFlagManager.Set(m_idxSelectedSocket);

	int nReserveCount = 0;

	if (m_nMaxAcceptCount - m_nAcceptedCount >= m_nCreateAcceptCount)
		nReserveCount = m_nCreateAcceptCount;
	else
		nReserveCount = m_nMaxAcceptCount - m_nAcceptedCount;

	if (nReserveCount > 0)
	{
		int rc = ReserveAccept(nReserveCount);
		if (NTL_SUCCESS != rc)
		{
			NTL_PRINT(PRINT_SYSTEM, "ReserveAccept Fail :%d[%s]", rc, NtlGetErrorMessage(rc));
		}
		else
		{
		//	NTL_PRINT(PRINT_SYSTEM, "ReserveAccept Success : ReserveCount[%d] Accepting[%d] Accepted[%u] Port[%d]", nReserveCount, m_nAcceptingCount, m_nAcceptedCount, m_idxSelectedSocket + m_nPortBase);
		}
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlAcceptor::Destroy()
{
	for (int j = 0; j < m_nListenSocketSize; j++)
		m_pArrListenSocket[j].Close();

	m_BitFlagManager.Destroy();

	SAFE_DELETE(m_pArrListenSocket);

	SAFE_DELETE(m_pAcceptingSessionList);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlAcceptor::Listen()
{
	for (unsigned short j = 0; j < m_nListenSocketSize; j++)
	{
		int rc = m_pArrListenSocket[j].Create(CNtlSocket::eSOCKET_TCP);
		if (NTL_SUCCESS != rc)
		{
			NTL_PRINT(PRINT_SYSTEM, "m_pArrListenSocket[ j ].Create() failed.(NTL_SUCCESS != rc), rc = %d", rc);
			return rc;
		}

		m_listenAddr.SetPort(j + m_nPortBase);

		rc = m_pArrListenSocket[j].Bind(m_listenAddr);
		if (NTL_SUCCESS != rc)
		{
			NTL_PRINT(PRINT_SYSTEM, "m_pArrListenSocket[ j ].Bind() failed.(NTL_SUCCESS != rc), rc = %d", rc);
			return rc;
		}


		rc = m_pArrListenSocket[j].SetReuseAddr(TRUE);
		if (NTL_SUCCESS != rc)
		{
			NTL_PRINT(PRINT_SYSTEM, "m_pArrListenSocket[ j ].SetReuseAddr() failed.(NTL_SUCCESS != rc), rc = %d", rc);
			return rc;
		}

		rc = m_pArrListenSocket[j].Listen();
		if (NTL_SUCCESS != rc)
		{
			NTL_PRINT(PRINT_SYSTEM, "m_pArrListenSocket[ j ].Listen() failed.(NTL_SUCCESS != rc), rc = %d", rc);
			return rc;
		}

	}

	m_listenAddr.SetPort(m_idxSelectedSocket + m_nPortBase);

	return NTL_SUCCESS;
}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlAcceptor::OnAssociated(CNtlNetwork * pNetwork)
{
	if( NULL == pNetwork )
	{
		NTL_PRINT(PRINT_SYSTEM, "NULL == pNetwork");
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}

	if( NULL != m_pNetwork )
	{
		NTL_PRINT(PRINT_SYSTEM, "NULL != m_pNetwork, m_pNetwork = %016x", m_pNetwork);
		return NTL_ERR_NET_ACCEPTOR_ASSOCIATE_FAIL;
	}

	if (NULL != m_pAcceptingSessionList)
	{
		NTL_PRINT(PRINT_SYSTEM, "NULL != m_pAcceptingSessionList, m_pAcceptingSessionList = %016x", m_pAcceptingSessionList);
		return NTL_ERR_NET_ACCEPTOR_ASSOCIATE_FAIL;
	}

	m_pNetwork = pNetwork;

	if (m_nListenSocketSize > 1)
	{
		m_pAcceptingSessionList = new CNtlAcceptingSessionList;
		if (m_pAcceptingSessionList == NULL)
		{
			NTL_PRINT(PRINT_SYSTEM, "m_pAcceptingSessionList == NULL");
			return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
		}

		m_pAcceptingSessionList->Create(m_pNetwork, m_nCreateAcceptCount, m_nCreateAcceptCount / 10);
	}

	int rc = CreateThread();
	if( NTL_SUCCESS != rc )
	{
		NTL_PRINT(PRINT_SYSTEM, "CreateThread() failed.(NTL_SUCCESS != rc), rc = %d", rc);
		return rc;
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void	CNtlAcceptor::OnAccepted(CNtlConnection * pConnection)
{
	DecreaseCurAcceptingCount();
	IncreaseCurAcceptedCount();

	++m_dwTotalAcceptCount;

	CNtlThread::Notify(m_pThread->GetThread());

	if (m_pAcceptingSessionList)
	{
		CNtlSession* pCon = static_cast<CNtlSession*>(pConnection); 
		if (pCon)
			m_pAcceptingSessionList->Remove(pCon);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlAcceptor::CreateThread()
{
	CNtlString strName;
	strName.Format("AcceptorThread [%s:%u] Type[%u]", m_listenAddr.GetDottedAddr(), m_listenAddr.GetPort(), m_sessionType);

	m_pThread = new CAcceptorThread( this );
	if (NULL == m_pThread)
	{
		NTL_PRINT(PRINT_SYSTEM, "\"new CAcceptorThread( this )\" failed.");
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	CNtlThread * pThread = tThreadFactory::Instance().CreateThread( m_pThread, strName.c_str(), true );
	if( NULL == pThread )
	{
		NTL_PRINT(PRINT_SYSTEM, "NULL == pThread");
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
int CNtlAcceptor::ReserveAccept(int nReserveCount)
{
	if( NULL == m_pNetwork )
	{
		NTL_PRINT(PRINT_SYSTEM, "NULL == m_pNetwork");
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}

	for( int i = 0; i < nReserveCount; i++ )
	{
		CNtlSession * pSession = m_pNetwork->GetSessionFactory()->CreateSession( m_sessionType );
		if( NULL == pSession )
		{
			NTL_PRINT(PRINT_SYSTEM, "m_pNetwork->GetSessionFactory()->CreateSession() failed.(NULL == pSession), i = %d, m_sessionType = %u", i, m_sessionType);
			return NTL_ERR_NET_CONNECTION_POOL_ALLOC_FAIL;
		}

		int rc = pSession->Create( m_pNetwork ); 
		if( NTL_SUCCESS != rc )
		{
			NTL_PRINT(PRINT_SYSTEM, "pSession->Create() failed(NTL_SUCCESS != rc), rc = %d", rc);
			RELEASE_SESSION( pSession );
			return rc;
		}

		rc = pSession->PostAccept( this );
		if( NTL_SUCCESS != rc )
		{
			NTL_PRINT(PRINT_SYSTEM, "pSession->PostAccept() failed(NTL_SUCCESS != rc), rc = %d", rc);
			RELEASE_SESSION( pSession );
			return rc;
		}

		if (m_pAcceptingSessionList)
			m_pAcceptingSessionList->Add(pSession);
	}

	//NTL_PRINT(PRINT_SYSTEM, "ReserveAccept [%d]", nReserveCount);

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Accept 하려는 개수와 Accept 가능 개수를 조사해 가능한 Accept개수를 반환한다 
//-----------------------------------------------------------------------------------
int CNtlAcceptor::GetReserveAcceptCount()
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	if( m_nAcceptingCount < m_nMinAcceptCount )
	{
		int nAcceptDiff = m_nPostAcceptCount - m_nAcceptingCount;
		int nAvailableAccept = m_nMaxAcceptCount - ( m_nAcceptingCount + m_nAcceptedCount );

		return min( nAvailableAccept, nAcceptDiff );
	}

	return 0;
}
