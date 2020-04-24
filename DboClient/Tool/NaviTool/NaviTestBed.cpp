#include "StdAfx.h"
#include "NaviTestBed.h"

#include "PathEngine.h"
#include "dxutil.h"

CNaviTestBed::CNaviTestBed(void)
{
}

CNaviTestBed::~CNaviTestBed(void)
{
}

BOOL CNaviTestBed::Create( HWND hWnd, int nWidth, int nHeight )
{
	DXUtil_Timer( TIMER_START );

	//m_pTestBed = DLLExport_GetITestBed( hWnd, nWidth, nHeight );



	return TRUE;
}

void CNaviTestBed::Destroy()
{
	DXUtil_Timer( TIMER_STOP );
}

void CNaviTestBed::Update()
{
	m_fTime			= DXUtil_Timer( TIMER_GETAPPTIME );
	m_fElapsedTime	= DXUtil_Timer( TIMER_GETELAPSEDTIME );

	Update( m_fTime, m_fElapsedTime );
}

void CNaviTestBed::Update(float fTime, float fElapsedTime)
{
	CPathEngine::GetInstance()->Update();

	// Update Time

	// Render
	/*m_pTestBed->drawMesh();*/
}

void CNaviTestBed::Render()
{

}

void CNaviTestBed::TestBedMsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

}