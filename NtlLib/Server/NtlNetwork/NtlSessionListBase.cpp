//***********************************************************************************
//
//	File		:	NtlSessionListBase.cpp
//
//	Begin		:	25 OCT 17
//
//	Copyright	:	DBO GLOBAL
//
//	Author		:	Daneos
//
//	Desc		:	
//
//***********************************************************************************


#include "stdafx.h"
#include "NtlSessionListBase.h"

#include "NtlNetwork.h"
#include "NtlSession.h"

#include "NtlLog.h"
#include "NtlError.h"



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSessionListBase::CNtlSessionListBase()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSessionListBase::~CNtlSessionListBase()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSessionListBase::Create(CNtlNetwork * pNetwork, int nSessionSize, int nExtraSize)
{
	CNtlAutoMutex mutex(&m_mutex);
	mutex.Lock();

	if (false == m_sessionList.Create(nSessionSize, nExtraSize))
	{
		return NTL_FAIL;
	}

	m_pNetworkRef = pNetwork;


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSessionListBase::Destroy()
{
	CNtlAutoMutex mutex(&m_mutex);
	mutex.Lock();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSessionListBase::Init()
{
	m_pNetworkRef = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSession * CNtlSessionListBase::Find(LISTIT pos)
{
	CNtlAutoMutex mutex(&m_mutex);
	mutex.Lock();

	CNtlSession ** ppSession = m_sessionList.GetPtr(pos);
	if (NULL == ppSession)
	{
		return NULL;
	}

	return *ppSession;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
HSESSION CNtlSessionListBase::_Add(CNtlSession * pSession)
{
	if (NULL == pSession)
	{
		NTL_PRINT(PRINT_SYSTEM, "NULL == pSession");
		return INVALID_HSESSION;
	}

	if (INVALID_HSESSION != pSession->GetHandle())
	{
		NTL_PRINT(PRINT_SYSTEM, "INVALID_HSESSION != pSession->GetHandle(), pSession->GetHandle() = %u", pSession->GetHandle());
		return INVALID_HSESSION;
	}


	CNtlAutoMutex mutex(&m_mutex);
	mutex.Lock();

	LISTIT it = m_sessionList.Insert(pSession);
	if (it == m_sessionList.End())
	{
		return INVALID_HSESSION;
	}
	
	return it;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
HSESSION CNtlSessionListBase::_Remove(CNtlSession * pSession)
{
	if (NULL == pSession)
	{
		NTL_PRINT(PRINT_SYSTEM, "NULL == pSession");
		return INVALID_HSESSION;
	}

	if (INVALID_HSESSION == pSession->GetHandle())
	{
		return INVALID_HSESSION;
	}


	CNtlAutoMutex mutex(&m_mutex);
	mutex.Lock();

	m_sessionList.Remove(pSession->GetHandle());

	return 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
HSESSION CNtlSessionListBase::_Remove(HSESSION hSession)
{
	if (INVALID_HSESSION == hSession)
	{
		return INVALID_HSESSION;
	}


	CNtlAutoMutex mutex(&m_mutex);
	mutex.Lock();

	m_sessionList.Remove(hSession);

	return 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSessionListBase::GetCurCount()
{
	CNtlAutoMutex mutex(&m_mutex);
	mutex.Lock();

	return m_sessionList.GetSize();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSessionListBase::GetMaxCount()
{
	CNtlAutoMutex mutex(&m_mutex);
	mutex.Lock();

	return m_sessionList.GetMaxSize();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSessionListBase::PacketLog(CNtlSession * pSession)
{
	//pSession->PacketLog();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSessionListBase::SetSessionHandle(CNtlSession * pSession, HSESSION hSession)
{
	pSession->m_hSession = hSession;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSessionListBase::SetAcceptingSessionHandle(CNtlSession * pSession, HSESSION hSession)
{
	pSession->m_hAcceptingSession = hSession;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSessionListBase::ValidCheck(DWORD dwTickTime)
{
	CNtlAutoMutex mutex(&m_mutex);
	mutex.Lock();

	CNtlSession * pSession = NULL;
	for (LISTIT it = m_sessionList.Begin(); it != m_sessionList.End(); )
	{
		pSession = *(m_sessionList.GetPtr(it));

		if (pSession)
		{
			//	if (false == pSession->CheckAuth(dwTime))
			//	{
			//		NTL_PRINT(PRINT_SYSTEM, "The session[%X] closed by unauthorized.", pSession);
			//		pSession->Disconnect(false);
			//	}
			/*else*/ if (false == pSession->ValidCheck(dwTickTime))
			{
				NTL_PRINT(PRINT_SYSTEM, "The session[%X] should be disconnected due to timeout.", pSession);
				pSession->Disconnect(false);
			}
			else if (pSession->PacketLogTime(dwTickTime))
				PacketLog(pSession);

			if (true == pSession->IsShutdownable()) //closed connections will be removed here
			{
				pSession->Shutdown();

				m_pNetworkRef->PostNetEventMessage((WPARAM)NETEVENT_CLOSE, (LPARAM)pSession);
			}

			if (pSession->CanbeDestroy())
			{
				it = m_sessionList.Remove(it);
				m_pNetworkRef->PostNetEventMessage((WPARAM)NETEVENT_DESTROY, (LPARAM)pSession);

				continue;
			}

		}

		it = m_sessionList.Next(it);
	}

}