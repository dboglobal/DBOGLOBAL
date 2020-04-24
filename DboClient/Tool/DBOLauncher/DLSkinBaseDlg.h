/*****************************************************************************
*
* File			: DLSkinBaseDlg.h
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
#include "BitmapDlg.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLSkinBaseDlg
//
//////////////////////////////////////////////////////////////////////////


class CDLSkinBaseDlg : public CBitmapDlg
{
public:
	CDLSkinBaseDlg( void );
	virtual ~CDLSkinBaseDlg( void );

public:
	virtual bool				CreateDlg( void ) { return false; }

	virtual void				DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );

protected:
	void						UpdateDialog( void );
};
