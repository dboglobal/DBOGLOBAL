/*****************************************************************************
*
* File			: DLStateSys.h
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


class CDLStateBase;


//////////////////////////////////////////////////////////////////////////
//
// CDLStateSys
//
//////////////////////////////////////////////////////////////////////////


class CDLStateSys
{
protected:
	CDLStateBase*				m_pCurState;

public:
	CDLStateSys( void );
	~CDLStateSys( void );

public:
	bool						Create( void );
	void						Delete( void );
	void						Update( void );

	void						DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );
};
