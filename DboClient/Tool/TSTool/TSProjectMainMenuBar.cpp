// ProjectMainMenu.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "TSProjectMainMenuBar.h"


// CTSProjectMainMenuBar

IMPLEMENT_DYNCREATE(CTSProjectMainMenuBar, CSizingControlBarCF)

CTSProjectMainMenuBar::CTSProjectMainMenuBar()
{
	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CFrameWnd);
	m_pFrameWnd = (CFrameWnd*)pRuntimeClass->CreateObject();

	ZeroMemory( &m_Context, sizeof(m_Context) );
}

CTSProjectMainMenuBar::~CTSProjectMainMenuBar()
{
}

BOOL CTSProjectMainMenuBar::Create( CWnd* pParentWnd,
								    CRuntimeClass* pViewClass,
									CCreateContext* pContext,
									LPCTSTR lpszWindowName,
									DWORD dwStyle,
									UINT nID )
{
	ASSERT( pViewClass );
	ASSERT( pViewClass->IsDerivedFrom( RUNTIME_CLASS(CWnd) ) );

	if ( pContext )
	{
		memcpy(&m_Context, pContext, sizeof(m_Context));
	}
	else
	{
		CFrameWnd* pfw = (CFrameWnd*)AfxGetMainWnd();
		if ( pfw )
		{
			m_Context.m_pCurrentFrame = pfw;
		}
	}

	m_Context.m_pNewViewClass = pViewClass;

	return CSizingControlBarCF::Create( lpszWindowName, pParentWnd, nID, dwStyle );
}

void CTSProjectMainMenuBar::DoDataExchange(CDataExchange* pDX)
{
	CSizingControlBarCF::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTSProjectMainMenuBar, CSizingControlBarCF)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTSProjectMainMenuBar 진단입니다.

#ifdef _DEBUG
void CTSProjectMainMenuBar::AssertValid() const
{
	CSizingControlBarCF::AssertValid();
}

#ifndef _WIN32_WCE
void CTSProjectMainMenuBar::Dump(CDumpContext& dc) const
{
	CSizingControlBarCF::Dump(dc);
}
#endif
#endif //_DEBUG


// CTSProjectMainMenuBar 메시지 처리기입니다.
int CTSProjectMainMenuBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CSizingControlBarCF::OnCreate( lpCreateStruct ) == -1 )
	{
		return -1;
	}

	if ( !m_pFrameWnd->Create( NULL,
							   NULL,
							   WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,
							   CRect(0,0,0,0),
							   this,
							   NULL,
							   0,
							   &m_Context ) )
	{
		return -1;
	}

	return 0;
}

void CTSProjectMainMenuBar::OnSize(UINT nType, int cx, int cy)
{
	CSizingControlBarCF::OnSize( nType, cx, cy );

	CRect rc;
	GetClientRect( rc );
	m_pFrameWnd->MoveWindow( rc );
}
