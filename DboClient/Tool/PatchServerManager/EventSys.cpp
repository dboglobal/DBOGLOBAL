/*****************************************************************************
*
* File			: EventSys.cpp
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/

#include "stdafx.h"
#include "EventSys.h"


//////////////////////////////////////////////////////////////////////////
//
// CEventSys
//
//////////////////////////////////////////////////////////////////////////


CEventSys::CEventSys( void )
{
}

CEventSys::~CEventSys( void )
{
	Delete();
}

bool CEventSys::Create( void )
{
	Delete();

	return true;
}

void CEventSys::Delete( void )
{
	m_defCalleeList.clear();

	if ( !m_defPostMsgDataList.empty() )
	{
		listdef_PostMsgList::iterator itPostMsgData = m_defPostMsgDataList.begin();

		for ( ; itPostMsgData != m_defPostMsgDataList.end(); )
		{
			sPOST_MSG_DATA& sMsgData = *itPostMsgData;

			if ( sMsgData.pData ) delete [] sMsgData.pData;
		}

		m_defPostMsgDataList.clear();
	}
}

void CEventSys::Update( void )
{
	if ( !m_defPostMsgDataList.empty() )
	{
		listdef_PostMsgList::iterator itPostMsgData = m_defPostMsgDataList.begin();

		for ( ; itPostMsgData != m_defPostMsgDataList.end(); )
		{
			sPOST_MSG_DATA& sMsgData = *itPostMsgData;

			listdef_CalleeList::iterator itCallee = m_defCalleeList.begin();

			for ( ; itCallee != m_defCalleeList.end(); ++itCallee )
			{
				CEventBase* pCallee = *itCallee;

				pCallee->MessageProc( sMsgData.eMsg, sMsgData.pData );
			}

			if ( sMsgData.pData ) delete [] sMsgData.pData;

			itPostMsgData = m_defPostMsgDataList.erase( itPostMsgData );
		}
	}
}

void CEventSys::Attach( CEventBase* pCallee )
{
	m_defCalleeList.push_back( pCallee );
}

void CEventSys::Detach( CEventBase* pCallee )
{
	listdef_CalleeList::iterator it = m_defCalleeList.begin();

	for ( ; it != m_defCalleeList.end(); )
	{
		if ( (void*)*it == (void*)pCallee )
		{
			it = m_defCalleeList.erase( it );
		}
		else
		{
			++it;
		}
	}
}

void CEventSys::SendEvent( eEVENT eMsg, sEVENT_DATA_BASE* pData )
{
	listdef_CalleeList::iterator it = m_defCalleeList.begin();

	for ( ; it != m_defCalleeList.end(); ++it )
	{
		CEventBase* pCallee = *it;

		pCallee->MessageProc( eMsg, pData );
	}
}

void CEventSys::PostEvent( eEVENT eMsg, sEVENT_DATA_BASE* pData )
{
	sPOST_MSG_DATA sPostMsgData;

	sPostMsgData.eMsg = eMsg;
	sPostMsgData.pData = pData->Clone();

	m_defPostMsgDataList.push_back( sPostMsgData );
}
