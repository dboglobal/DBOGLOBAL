/*****************************************************************************
 *
 * File			: DLPatcherSys.h
 * Author		:
 * Copyright	: (주)NTL
 * Date			:
 * Abstract		: 
 *****************************************************************************
 * Desc         : Patch Data를 Patch를 한다. ( Download가 아님 )
 *
 *****************************************************************************/


#pragma once


#include "patchwin.h"
#include "DLEventDef.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLPatcherSys
//
//////////////////////////////////////////////////////////////////////////


class CDLPatcherSys
{
protected:
	enum ePATCH_SYS_CUR_STATE
	{
		ePATCH_SYS_CUR_STATE_NORMAL,
		ePATCH_SYS_CUR_STATE_WARNING,
		ePATCH_SYS_CUR_STATE_ERROR
	};

protected:
	static BYTE				s_byPatchCurState;
	static bool				s_bPendingDestroy;

public:
	CDLPatcherSys( void );
	~CDLPatcherSys( void );

public:
	bool					Create( void );
	void					Delete( void );

	void					DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );

protected:
	bool					DoPatch( CString strDestPatchPath, CString strCurFullPatchFileName );

	static LPVOID CALLBACK	CallBack( UINT Id, LPVOID lpParm );
};
