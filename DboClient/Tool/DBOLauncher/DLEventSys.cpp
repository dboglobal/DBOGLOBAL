/*****************************************************************************
*
* File			: DLEventSys.cpp
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/

#include "stdafx.h"
#include "DLEventSys.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLEventSys
//
//////////////////////////////////////////////////////////////////////////


CDLEventSys::CDLEventSys( void )
{
}

CDLEventSys::~CDLEventSys( void )
{
	Delete();
}

bool CDLEventSys::Create( void )
{
	Delete();

	return true;
}

void CDLEventSys::Delete( void )
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

void CDLEventSys::Update( void )
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
				CDLEventBase* pCallee = *itCallee;

				pCallee->DLMessageProc( sMsgData.eMsg, sMsgData.pData );
			}

			if ( sMsgData.pData ) delete [] sMsgData.pData;

			itPostMsgData = m_defPostMsgDataList.erase( itPostMsgData );
		}
	}
}

void CDLEventSys::Attach( CDLEventBase* pCallee )
{
	m_defCalleeList.push_back( pCallee );
}

void CDLEventSys::Detach( CDLEventBase* pCallee )
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

void CDLEventSys::SendEvent( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	listdef_CalleeList::iterator it = m_defCalleeList.begin();

	for ( ; it != m_defCalleeList.end(); ++it )
	{
		CDLEventBase* pCallee = *it;

		pCallee->DLMessageProc( eMsg, pData );
	}
}

void CDLEventSys::PostEvent( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	sPOST_MSG_DATA sPostMsgData;

	sPostMsgData.eMsg = eMsg;
	sPostMsgData.pData = pData->Clone();

	m_defPostMsgDataList.push_back( sPostMsgData );
}
