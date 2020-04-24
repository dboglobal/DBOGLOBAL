/*****************************************************************************
*
* File			: DBOLauncher.h
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#pragma once


#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif


#include "resource.h"		// main symbols


//////////////////////////////////////////////////////////////////////////
//
// CDBOLauncherApp
//
//////////////////////////////////////////////////////////////////////////


class CDBOLauncherApp : public CWinApp
{
protected:
	int				m_nExitCode;

public:
	CDBOLauncherApp( void );

public:
	int				GetExitCode( void );
	void			SetExitCode( int nExitCode );

// Overrides
public:
	virtual BOOL	InitInstance( void );
	virtual int		ExitInstance( void );
	virtual int		Run( void );

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};


extern CDBOLauncherApp theApp;