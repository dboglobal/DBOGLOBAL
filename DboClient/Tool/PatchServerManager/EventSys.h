/*****************************************************************************
*
* File			: EventSys.h
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


#include "EventDef.h"


//////////////////////////////////////////////////////////////////////////
//
// CEventBase
//
//////////////////////////////////////////////////////////////////////////


class CEventBase
{
public:
	virtual ~CEventBase( void ) { return; }

public:
	virtual void				MessageProc( eEVENT eMsg, sEVENT_DATA_BASE* pData ) = 0;
};


//////////////////////////////////////////////////////////////////////////
//
// CEventSys
//
//////////////////////////////////////////////////////////////////////////


class CEventSys
{
public:
	typedef std::list< CEventBase* > listdef_CalleeList;

	struct sPOST_MSG_DATA
	{
		eEVENT					eMsg;
		sEVENT_DATA_BASE*		pData;
	};
	typedef std::list< sPOST_MSG_DATA > listdef_PostMsgList;

protected:
	listdef_CalleeList			m_defCalleeList;
	listdef_PostMsgList			m_defPostMsgDataList;

public:
	CEventSys( void );
	~CEventSys( void );

public:
	bool						Create( void );
	void						Delete( void );
	void						Update( void );

	void						Attach( CEventBase* pCallee );
	void						Detach( CEventBase* pCallee );

	void						SendEvent( eEVENT eMsg, sEVENT_DATA_BASE* pData );
	void						PostEvent( eEVENT eMsg, sEVENT_DATA_BASE* pData );
};
