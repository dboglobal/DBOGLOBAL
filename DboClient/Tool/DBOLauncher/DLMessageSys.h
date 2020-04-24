/*****************************************************************************
*
* File			: DLMessageSys.h
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


class CDLMsgFinder;


//////////////////////////////////////////////////////////////////////////
//
// CDLMessageSys
//
//////////////////////////////////////////////////////////////////////////


class CDLMessageSys
{
protected:
	CDLMsgFinder*			m_pMsgFinder;

public:
	CDLMessageSys( void );
	~CDLMessageSys( void );

public:
	bool					Create( void );
	void					Delete( void );

	void					DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );
};
