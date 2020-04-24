/*****************************************************************************
*
* File			: DLSkinSys.h
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


class CDLSkinBaseDlg;


//////////////////////////////////////////////////////////////////////////
//
// CDLSkinSys
//
//////////////////////////////////////////////////////////////////////////


class CDLSkinSys
{
protected:
	CDLSkinBaseDlg*				m_pCurSkinDlg;

public:
	CDLSkinSys( void );
	~CDLSkinSys( void );

public:
	bool						Create( void );
	void						Delete( void );

	void						DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );

protected:
	bool						CreateSkin( unsigned int uiPublisherID );
	void						DeleteSkin( void );
};
