/*****************************************************************************
*
* File			: DLSkinBaseDlg.cpp
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#include "stdafx.h"
#include "DLSkinBaseDlg.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLSkinBaseDlg
//
//////////////////////////////////////////////////////////////////////////


CDLSkinBaseDlg::CDLSkinBaseDlg( void )
{
}

CDLSkinBaseDlg::~CDLSkinBaseDlg( void )
{
}

void CDLSkinBaseDlg::DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
}

void CDLSkinBaseDlg::UpdateDialog( void )
{
	bool bContinue = true;
	MSG msg;

	while ( bContinue )
	{
		if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			bContinue = false;
		}
	}
}
