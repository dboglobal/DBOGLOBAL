/*****************************************************************************
*
* File			: DLSkinSys.cpp
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#include "stdafx.h"
#include "DLSkinSys.h"
#include "DLSkinBaseDlg.h"
#include "DLSkinDlg_Dev.h"
#include "DLSkinDlg_CJIKor.h"
#include "DBOLauncherDef.h"
#include "NtlService.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLSkinSys
//
//////////////////////////////////////////////////////////////////////////


CDLSkinSys::CDLSkinSys( void )
{
	m_pCurSkinDlg = NULL;
}

CDLSkinSys::~CDLSkinSys( void )
{
	Delete();
}

bool CDLSkinSys::Create( void )
{
	Delete();

	return true;
}

void CDLSkinSys::Delete( void )
{
	DeleteSkin();
}

void CDLSkinSys::DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	if ( eDL_EVENT_CREATE_SKIN == eMsg )
	{
		sDL_ED_CREATE_SKIN* pEvtData = (sDL_ED_CREATE_SKIN*) pData;

		pEvtData->bResult = CreateSkin( pEvtData->uiPublisherID );
	}
	else if ( eDL_EVENT_DELETE_SKIN == eMsg )
	{
		DeleteSkin();
	}

	if ( m_pCurSkinDlg )
	{
		m_pCurSkinDlg->DLMessageProc( eMsg, pData );
	}
}

bool CDLSkinSys::CreateSkin( unsigned int uiPublisherID )
{
	DeleteSkin();

	switch ( uiPublisherID )
	{
	case eDBO_SERVICE_PUBLISHER_NTL:
		{
			m_pCurSkinDlg = new CDLSkinDlg_Dev;
		}
		break;

	case eDBO_SERVICE_PUBLISHER_CJI_KOR:
		{
			m_pCurSkinDlg = new CDLSkinDlg_CJIKor;
		}
		break;
	}

	if ( NULL == m_pCurSkinDlg )
	{
		return false;
	}

	if ( !m_pCurSkinDlg->CreateDlg() )
	{
		delete m_pCurSkinDlg;

		m_pCurSkinDlg = NULL;

		return false;
	}

	return true;
}

void CDLSkinSys::DeleteSkin( void )
{
	if ( m_pCurSkinDlg )
	{
		delete m_pCurSkinDlg;
		m_pCurSkinDlg = NULL;
	}
}