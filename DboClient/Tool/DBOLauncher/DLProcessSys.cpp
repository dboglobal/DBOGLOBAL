/*****************************************************************************
*
* File			: DLProcessSys.cpp
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/

#include "stdafx.h"
#include "DLProcessSys.h"
#include "DLMessageSys.h"
#include "DLCmdLine.h"
#include "DLDownLoadSys.h"
#include "DLPatcherSys.h"
#include "DLIntegritySys.h"
#include "DLSkinSys.h"
#include "DLStateSys.h"
#include "DBOLauncher.h"


CDLProcessSys g_clDLProcessSys;


CDLProcessSys::CDLProcessSys( void )
{
	m_pEvtSys = NULL;
	m_pMsgSys = NULL;
	m_pDownloadSys = NULL;
	m_pPatcherSys = NULL;
	m_pIntegritySys = NULL;
	m_pSkinSys = NULL;
	m_pStateSys = NULL;
}

CDLProcessSys::~CDLProcessSys( void )
{
	Delete();
}

bool CDLProcessSys::Create( void )
{
	//////////////////////////////////////////////////////////////////////////
	//
	//	Event system
	//
	//////////////////////////////////////////////////////////////////////////

	m_pEvtSys = new CDLEventSys;

	if ( NULL == m_pEvtSys )
	{
		return false;
	}

	if ( !m_pEvtSys->Create() )
	{
		return false;
	}

	m_pEvtSys->Attach( this );

	//////////////////////////////////////////////////////////////////////////
	//
	//	Message system
	//
	//////////////////////////////////////////////////////////////////////////

	m_pMsgSys = new CDLMessageSys;

	if ( NULL == m_pMsgSys )
	{
		return false;
	}

	if ( !m_pMsgSys->Create() )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Load command line
	//
	//////////////////////////////////////////////////////////////////////////

	if ( !g_clDLCmdLine.LoadCmdLine( theApp.m_lpCmdLine ) )
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Download system
	//
	//////////////////////////////////////////////////////////////////////////

	m_pDownloadSys = new CDLDownLoadSys;

	if ( NULL == m_pDownloadSys )
	{
		return false;
	}

	if ( !m_pDownloadSys->Create() )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Patcher system
	//
	//////////////////////////////////////////////////////////////////////////

	m_pPatcherSys = new CDLPatcherSys;

	if ( NULL == m_pPatcherSys )
	{
		return false;
	}

	if ( !m_pPatcherSys->Create() )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Skin system
	//
	//////////////////////////////////////////////////////////////////////////

	m_pSkinSys = new CDLSkinSys;

	if ( NULL == m_pSkinSys )
	{
		return false;
	}

	if ( !m_pSkinSys->Create() )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Integrity system
	//
	//////////////////////////////////////////////////////////////////////////

	m_pIntegritySys = new CDLIntegritySys;

	if ( NULL == m_pIntegritySys )
	{
		return false;
	}

	if ( !m_pIntegritySys->Create() )
	{
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	State system
	//
	//////////////////////////////////////////////////////////////////////////

	m_pStateSys = new CDLStateSys;

	if ( NULL == m_pStateSys )
	{
		return false;
	}

	if ( !m_pStateSys->Create() )
	{
		return false;
	}

	return true;
}

void CDLProcessSys::Delete( void )
{
	if ( m_pStateSys )
	{
		delete m_pStateSys;
		m_pStateSys = NULL;
	}

	if ( m_pIntegritySys )
	{
		delete m_pIntegritySys;
		m_pIntegritySys = NULL;
	}

	if ( m_pSkinSys )
	{
		delete m_pSkinSys;
		m_pSkinSys = NULL;
	}

	if ( m_pPatcherSys )
	{
		delete m_pPatcherSys;
		m_pPatcherSys = NULL;
	}

	if ( m_pDownloadSys )
	{
		delete m_pDownloadSys;
		m_pDownloadSys = NULL;
	}

	if ( m_pMsgSys )
	{
		delete m_pMsgSys;
		m_pMsgSys = NULL;
	}

	if ( m_pEvtSys )
	{
		m_pEvtSys->Detach( this );

		delete m_pEvtSys;
		m_pEvtSys = NULL;
	}
}

bool CDLProcessSys::Update( void )
{
	if ( m_pEvtSys )
	{
		m_pEvtSys->Update();
	}

	if ( m_pStateSys )
	{
		m_pStateSys->Update();
	}

	return true;
}

void CDLProcessSys::DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	if ( m_pMsgSys )
	{
		m_pMsgSys->DLMessageProc( eMsg, pData );
	}

	if ( m_pDownloadSys )
	{
		m_pDownloadSys->DLMessageProc( eMsg, pData );
	}

	if ( m_pPatcherSys )
	{
		m_pPatcherSys->DLMessageProc( eMsg, pData );
	}

	if ( m_pSkinSys )
	{
		m_pSkinSys->DLMessageProc( eMsg, pData );
	}

	if ( m_pIntegritySys )
	{
		m_pIntegritySys->DLMessageProc( eMsg, pData );
	}

	if ( m_pStateSys )
	{
		m_pStateSys->DLMessageProc( eMsg, pData );
	}
}
