/*****************************************************************************
 *
 * File			: DLDownLoadSys.h
 * Author		:
 * Copyright	: (주)NTL
 * Date			:
 * Abstract		: 
 *****************************************************************************
 * Desc         : Http Protocol 을 이용한 다운로드 시스템
 *
 *****************************************************************************/


#pragma once


#include "DLEventDef.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLDownLoadSys
//
//////////////////////////////////////////////////////////////////////////


class CDLDownLoadSys
{
protected:
	bool					m_bPendingDestroy;

public:
	CDLDownLoadSys( void );
	~CDLDownLoadSys( void );

public:
	bool					Create( void );
	void					Delete( void );

	void					DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );

protected:
	bool					DoDownload( CString strServerName,
										CString strServerPath,
										CString strServerFileName,
										CString strClientPath,
										CString strClientFileName );
};
