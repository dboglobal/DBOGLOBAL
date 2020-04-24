// CG: This file was added by the Splash Screen component.
// Splash.cpp : implementation file
//

#include "stdafx.h"  // e. g. stdafx.h
#include "Splash.h"

#include "VenusAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

BOOL CSplashWnd::c_bShowSplashWnd;
CSplashWnd* CSplashWnd::c_pSplashWnd;
CSplashWnd::CSplashWnd()
{
	m_pGflBitmap = NULL;
}

CSplashWnd::~CSplashWnd()
{
	// Clear the static window pointer.
	ASSERT(c_pSplashWnd == this);
	c_pSplashWnd = NULL;
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	c_bShowSplashWnd = bEnable;
}

void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd /*= NULL*/, char* filename)
{
	if (!c_bShowSplashWnd || c_pSplashWnd != NULL)
		return;

	// Allocate a new splash screen, and create the window.
	c_pSplashWnd = new CSplashWnd;
	if (!c_pSplashWnd->Create(pParentWnd, filename))
	{
		delete (c_pSplashWnd);
	}
	else
		c_pSplashWnd->UpdateWindow();
}

BOOL CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (c_pSplashWnd == NULL)
		return FALSE;

	// If we get a keyboard or mouse message, hide the splash screen.
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
		c_pSplashWnd->HideSplashScreen();
		return TRUE;	// message handled here
	}

	return FALSE;	// message not handled
}

BOOL CSplashWnd::Create(CWnd* pParentWnd /*= NULL*/, char* filename)
{
	GFL_LOAD_PARAMS load_params;	
	gflGetDefaultPreviewParams(&load_params);
	load_params.Flags |= GFL_LOAD_SKIP_ALPHA;
	load_params.Origin = GFL_BOTTOM_LEFT; 
	load_params.ColorModel = GFL_BGR; 
	load_params.LinePadding = 4;

	GFL_ERROR errorcode = gflLoadBitmap(filename, &m_pGflBitmap, &load_params, NULL);
	if(m_pGflBitmap == NULL)
	{
		return FALSE;
	}

	return CreateEx(0,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, 
		m_pGflBitmap->Width, m_pGflBitmap->Height, pParentWnd->GetSafeHwnd(), NULL);
}

void CSplashWnd::HideSplashScreen()
{
	// Destroy the window, and update the mainframe.
	DestroyWindow();
	AfxGetMainWnd()->UpdateWindow();
}

void CSplashWnd::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Center the window.
	CenterWindow();

	// Set a timer to destroy the splash screen.
//	SetTimer(1, 2500, NULL);

	return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);

	if(!m_pGflBitmap) return;

	CRect rect;
	GetClientRect(rect);

	HBRUSH hBrush=::CreateSolidBrush(RGB(255, 255, 255));
	::FillRect( dc.GetSafeHdc(), rect, hBrush);
	DeleteObject(hBrush);

	unsigned char *dib_data;
	BITMAPINFOHEADER dib_info;
	GetDIBFromBitmap( m_pGflBitmap, &dib_info, &dib_data );
	StretchDIBits( dc.m_hDC, 0, 0, rect.Width(), rect.Height(),
		0, 0, m_pGflBitmap->Width, m_pGflBitmap->Height, 
		dib_data ? dib_data : m_pGflBitmap->Data, (BITMAPINFO *)&dib_info, DIB_RGB_COLORS, SRCCOPY );				
	if ( dib_data ) free( dib_data ); 
}

void CSplashWnd::OnTimer(UINT nIDEvent)
{
	// Destroy the splash screen window.
	HideSplashScreen();
}

void CSplashWnd::DestroySplashWnd()
{
	if (c_pSplashWnd)
		c_pSplashWnd->HideSplashScreen();
}