//***********************************************************************************
//
//	File		:	SessionList.cpp
//
//	Begin		:	2007-01-02
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************


#include "stdafx.h"
#include "NtlSessionList.h"
#include "NtlSession.h"
#include "NtlNetwork.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSessionList::CNtlSessionList()
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSessionList::~CNtlSessionList()
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlSessionList::Add(CNtlSession * pSession)
{
	HSESSION handle = CNtlSessionListBase::_Add(pSession);

	if (handle == INVALID_HSESSION)
	{
		return false;
	}

	SetSessionHandle(pSession, handle);

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlSessionList::Remove(CNtlSession * pSession)
{
	HSESSION handle = CNtlSessionListBase::_Remove(pSession);

	if (handle == INVALID_HSESSION)
	{
		return false;
	}

	SetSessionHandle(pSession, INVALID_HSESSION);

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSessionList::ValidCheck(DWORD dwTickTime)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	DWORD dwTime = timeGetTime();

	CNtlSession * pSession = NULL;
	for( LISTIT it = m_sessionList.Begin(); it != m_sessionList.End(); )
	{
		pSession = *( m_sessionList.GetPtr( it ) );

		if( pSession )
		{
			//((void (__fastcall *)(CNtlConnection *, _QWORD))v9->vfptr[1].GetAliveCheckTime)(v9, dwTime);

		//	if (false == pSession->CheckAuth(dwTime))
		//	{
		//		NTL_PRINT(PRINT_SYSTEM, "The session[%X] closed by unauthorized.", pSession);
		//		pSession->Disconnect(false);
		//	}
			/*else*/ if( false == pSession->ValidCheck( dwTickTime ) )
			{
				NTL_PRINT(PRINT_SYSTEM, "The session[%X] should be disconnected due to timeout.", pSession);
				pSession->Disconnect( false );	
			}
			else if (pSession->PacketLogTime(dwTickTime))
				PacketLog(pSession);

			if( true == pSession->IsShutdownable() ) //closed connections will be removed here
			{
				pSession->Shutdown();

				m_pNetworkRef->PostNetEventMessage( (WPARAM)NETEVENT_CLOSE, (LPARAM)pSession );
			}

			if (pSession->CanbeDestroy())
			{
				it = m_sessionList.Remove(it);
				m_pNetworkRef->PostNetEventMessage((WPARAM)NETEVENT_DESTROY, (LPARAM)pSession);

				continue;
			}

		}

		it = m_sessionList.Next( it );
	}

}