//***********************************************************************************
//
//	File		:	Connector.h
//
//	Begin		:	2005-12-19
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


#include "NtlIOCP.h"
#include "NtlSockAddr.h"
#include "NtlSocket.h"

#include "NtlMutex.h"
#include "NtlThread.h"
#include "NtlError.h"


class CNtlSession;
class CNtlConnection;

class CNtlConnector
{
	friend class CConnectorThread;
	friend class CConnectorThreadEx;

public:

	CNtlConnector();

	virtual ~CNtlConnector();


public:

	int						CreateOneTry(LPCTSTR lpszConnectAddr, WORD wConnectPort, SESSIONTYPE sessionType);

	int						Create(	LPCTSTR lpszConnectAddr,
									WORD wConnectPort,
									SESSIONTYPE sessionType,
									DWORD dwRetryTime = 1000,
									DWORD dwProcessTime = 1000);

	void					Destroy();


public:


	SESSIONTYPE				GetSessionType() { return m_sessionType; }

	CNtlSockAddr &			GetConnectAddr() { return m_connectAddr; }

	const char *			GetConnectIP() { return m_connectAddr.GetDottedAddr(); }

	WORD					GetConnectPort() { return m_connectAddr.GetPort(); }

	DWORD					GetTotalConnectCount() { return m_dwTotalConnectCount; }

	void					GetSessionHandle(HSESSION & rSession);

	//GetSession;

public:

	int						OnAssociated(CNtlNetwork * pNetwork);

	void					OnConnected();

	void					OnDisconnected(bool bConnected);


protected:

	int						CreateThread();

	int						CreateThreadEx();

	int						DoConnect();

	void					Init();


private:

	CNtlNetwork *			m_pNetwork;

	CNtlSession *			m_pSession;

	CNtlSockAddr			m_connectAddr;

	SESSIONTYPE				m_sessionType;

private:

	DWORD					m_dwNextTry;

	DWORD					m_dwRetryTime;

	DWORD					m_dwProcessTime;

	DWORD					m_dwTotalConnectCount;

	BOOL					m_bConnected;

private:

	CConnectorThread *		m_pThread;
};


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
inline void	CNtlConnector::OnConnected()
{
	InterlockedExchange( (LONG*)&m_bConnected, TRUE );
	++m_dwTotalConnectCount;
}

inline void	CNtlConnector::OnDisconnected(bool bConnected)
{
	if( bConnected )
	{
		InterlockedExchange( (LONG*)&m_bConnected, FALSE );
	}
}
//-----------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------
// Network Monitor Thread class NEW
//---------------------------------------------------------------------------------------
class CConnectorThreadEx : public CNtlRunObject
{
public:

	CConnectorThreadEx() {}

private:

	std::list<CNtlConnector *>	m_lstConnector;
	CNtlMutex					m_mutex;

public:

	virtual void Run()
	{
		DWORD millisecs = 1000;

		while (IsRunnable())
		{
			m_mutex.Lock();

			if (m_lstConnector.begin() != m_lstConnector.end())
			{
				CNtlConnector* pConnector = m_lstConnector.front();
				if (pConnector)
				{
					millisecs = CheckConnection(pConnector);
					if (millisecs == 0)
					{
						m_lstConnector.pop_front();

						m_mutex.Unlock();
						AddConnector(pConnector);
						m_mutex.Lock();
					}
				}
			}
			
			m_mutex.Unlock();
			Wait(millisecs);

		} // end of while( IsRunnable() )

	}


	void AddConnector(CNtlConnector *pConnector)
	{
		CNtlLock mutex(&m_mutex);

		std::list<CNtlConnector *>::iterator it = m_lstConnector.begin();

		for (; it != m_lstConnector.end(); it++)
		{
			if (pConnector->m_dwNextTry < (*it)->m_dwNextTry)
			{
				break;
			}
		}

		m_lstConnector.insert(it, pConnector);
	}


	void RemConnector(CNtlConnector *pConnector)
	{
		CNtlLock mutex(&m_mutex);

		for (std::list<CNtlConnector *>::iterator it = m_lstConnector.begin(); it != m_lstConnector.end(); it++)
		{
			CNtlConnector* pCon = *it;
			if (pCon && pCon == pConnector)
			{
				m_lstConnector.erase(it);
				break;
			}
		}
	}


private:

	DWORD CheckConnection(CNtlConnector *pConnector)
	{
		DWORD dwTime = timeGetTime();

		if (pConnector->m_dwNextTry > dwTime)
			return pConnector->m_dwNextTry - dwTime;

		if (InterlockedCompareExchange((LONG*)& pConnector->m_bConnected, TRUE, TRUE))
		{
			pConnector->m_dwNextTry = pConnector->m_dwProcessTime + dwTime;
		}
		else
		{
			int rc = pConnector->DoConnect();
			if (rc != NTL_SUCCESS)
			{
				printf("Connector Connect Fail :%d[%s]", rc, NtlGetErrorMessage(rc));
			}
			else
			{
				printf("Connector Connect Success");
			}

			pConnector->m_dwNextTry = pConnector->m_dwRetryTime + dwTime;
		}

		return 0;
	}

};

typedef Loki::SingletonHolder<CConnectorThreadEx, Loki::CreateUsingNew, Loki::DefaultLifetime, Loki::ClassLevelLockable> tConnectorThreadEx;


