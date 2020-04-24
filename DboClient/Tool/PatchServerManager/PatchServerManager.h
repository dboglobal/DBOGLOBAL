// PatchServerManager.h : main header file for the PROJECT_NAME application
//

#pragma once


#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif


#include "resource.h"
#include "EventSys.h"
#include "PSMngConfig.h"
#include "UpdateDataMng.h"



class CPatchServerManagerApp : public CWinApp, public CEventBase
{
// Member variables
protected:
	int						m_nExitCode;

	CEventSys				m_clEventSys;

	CPSMngConfig			m_clPSMngConfig;

	CUpdateDataMng			m_clUpdateDataMng;


// Constructions and Destruction
public:
	CPatchServerManagerApp( void );


// Attributes
public:
	CEventSys&				GetEventSys( void )				{ return m_clEventSys; }
	CPSMngConfig&			GetPSMngConfig( void )			{ return m_clPSMngConfig; }

	int						GetExitCode( void )				{ return m_nExitCode; }
	void					SetExitCode( int nExitCode )	{ m_nExitCode = nExitCode; }


// Overrides
public:
	virtual BOOL			InitInstance( void );
	virtual int				ExitInstance( void );

	virtual void			MessageProc( eEVENT eMsg, sEVENT_DATA_BASE* pData );

protected:
	TCHAR*					GetCalendar( void );
    void                    SetWorkingDirectory();          ///< 현재 실행파일 경로를 작업 경로로 설정한다.

	static BOOL WINAPI		ConsoleHandlerRoutine( DWORD dwCtrlType );


// Implementation
	DECLARE_MESSAGE_MAP()
};


extern CPatchServerManagerApp theApp;