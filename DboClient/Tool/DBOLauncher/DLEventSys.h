/*****************************************************************************
*
* File			: DLEventSys.h
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


#include "DLEventDef.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLEventBase
//
//////////////////////////////////////////////////////////////////////////


class CDLEventBase
{
public:
	virtual ~CDLEventBase( void ) { return; }

public:
	virtual void				DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData ) = 0;
};


//////////////////////////////////////////////////////////////////////////
//
// CDLEventSys
//
//////////////////////////////////////////////////////////////////////////


class CDLEventSys
{
public:
	typedef std::list< CDLEventBase* > listdef_CalleeList;

	struct sPOST_MSG_DATA
	{
		eDL_EVENT				eMsg;
		sDL_EVENT_DATA_BASE*	pData;
	};
	typedef std::list< sPOST_MSG_DATA > listdef_PostMsgList;

protected:
	listdef_CalleeList			m_defCalleeList;
	listdef_PostMsgList			m_defPostMsgDataList;

public:
	CDLEventSys( void );
	~CDLEventSys( void );

public:
	bool						Create( void );
	void						Delete( void );
	void						Update( void );

	void						Attach( CDLEventBase* pCallee );
	void						Detach( CDLEventBase* pCallee );

	void						SendEvent( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );
	void						PostEvent( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );
};
