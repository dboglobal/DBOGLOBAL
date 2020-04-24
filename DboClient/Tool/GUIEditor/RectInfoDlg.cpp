// RectInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "RectInfoDlg.h"
#include ".\rectinfodlg.h"
#include "CDevILMFCMemDC.h"


// CRectInfoDlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CRectInfoDlg, CDialog)
CRectInfoDlg::CRectInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRectInfoDlg::IDD, pParent)
{
	m_bInitialized = FALSE;
}

CRectInfoDlg::~CRectInfoDlg()
{
}

void CRectInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRectInfoDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CRectInfoDlg 메시지 처리기입니다.

BOOL CRectInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	DWORD dwStyle;
	CRect rc;

	// PTS_NOTIFY - CPropTree will send notification messages to the parent window
	dwStyle = WS_CHILD | WS_VISIBLE | PTS_NOTIFY;

	// Init the control's size to cover the entire client area
	CWnd* pWindow = GetDlgItem( IDC_PROPERTY_RECTINFO );
	if( pWindow )
	{
		pWindow->GetWindowRect( rc );
		ScreenToClient( &rc );
	}

	//Create CPropTree Control
	m_PropertyTree.Create( dwStyle, rc, this, IDC_PROPERTY_RECTINFO );

	// Initialize Item
	CPropTreeItem* pRoot;
	pRoot = m_PropertyTree.InsertItem( new CPropTreeItem() );
	pRoot->SetLabelText( _T( "No Selected" ) );

	// End of Initialize
	m_bInitialized = TRUE;

	InitProperty();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CRectInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( m_bInitialized )
	{
		CWnd* pWindow = GetDlgItem( IDC_PROPERTY_RECTINFO );
		if( pWindow )
		{
			CRect rc;	
			pWindow->GetWindowRect(rc);
			ScreenToClient( &rc );
			rc.right = rc.left + cx;			
			pWindow->MoveWindow( rc.left, rc.top, cx, rc.Height() );

			if ( ::IsWindow( m_PropertyTree.GetSafeHwnd() ) )
				m_PropertyTree.MoveWindow( &rc );		
		}
	}
}

void CRectInfoDlg::InitProperty()
{
	m_PropertyTree.DeleteAllItems();

	// Title
	CPropTreeItem* pRoot = m_PropertyTree.InsertItem( new CPropTreeItem() );
	pRoot->Expand();
	pRoot->SetLabelText( _T( "Mouse and Rect Info" ) );
	pRoot->SetCtrlID( PROPERTY_CAPTION );

	// Mouse Point
	CPropTreeItemStatic* pStatic;
	pStatic = (CPropTreeItemStatic*)m_PropertyTree.InsertItem( new CPropTreeItemStatic(), pRoot );
	pStatic->SetLabelText( _T( "Mouse" ) );
	pStatic->SetInfoText( _T( "Mouse Coordinates" ) );
	pStatic->SetItemValue( (LPARAM)0 );
	pStatic->SetCtrlID( PROPERTY_MOUSEPT );

	// Select Rect
	CPropTreeItem* pTree = m_PropertyTree.InsertItem( new CPropTreeItem(), pRoot );
	pTree->Expand();
	pTree->SetLabelText( _T( "Selected Rect" ) );
	pTree->SetCtrlID( PROPERTY_GROUP_RECT );

	pStatic = (CPropTreeItemStatic*)m_PropertyTree.InsertItem( new CPropTreeItemStatic(), pTree );
	pStatic->SetLabelText( _T( "X, Y" ) );
	pStatic->SetInfoText( _T( "Rect Coordinates" ) );
	pStatic->SetItemValue( (LPARAM)0 );
	pStatic->SetCtrlID( PROPERTY_RECT_XY );

	pStatic = (CPropTreeItemStatic*)m_PropertyTree.InsertItem( new CPropTreeItemStatic(), pTree );
	pStatic->SetLabelText( _T( "Width, Height" ) );
	pStatic->SetInfoText( _T( "Rect Coordinates" ) );
	pStatic->SetItemValue( (LPARAM)0 );
	pStatic->SetCtrlID( PROPERTY_RECT_WH );

	pStatic = (CPropTreeItemStatic*)m_PropertyTree.InsertItem( new CPropTreeItemStatic(), pTree );
	pStatic->SetLabelText( _T( "RGBA" ) );
	pStatic->SetInfoText( _T( "RGBA" ) );
	pStatic->SetItemValue( (LPARAM)0 );
	pStatic->SetCtrlID( PROPERTY_RGBA );

	//// for Test
	//pStatic = (CPropTreeItemStatic*)m_PropertyTree.InsertItem( new CPropTreeItemStatic(), pRoot );
	//pStatic->SetLabelText( _T( "OriMouse" ) );
	//pStatic->SetInfoText( _T( "Original Mouse Coordinates" ) );
	//pStatic->SetItemValue( (LPARAM)0 );
	//pStatic->SetCtrlID( PROPERTY_ORIMOUSE );

	//pStatic = (CPropTreeItemStatic*)m_PropertyTree.InsertItem( new CPropTreeItemStatic(), pRoot );
	//pStatic->SetLabelText( _T( "Offset" ) );
	//pStatic->SetInfoText( _T( "Mouse Point" ) );
	//pStatic->SetItemValue( (LPARAM)0 );
	//pStatic->SetCtrlID( PROPERTY_OFFSET );

	//pStatic = (CPropTreeItemStatic*)m_PropertyTree.InsertItem( new CPropTreeItemStatic(), pRoot );
	//pStatic->SetLabelText( _T( "SrcRect" ) );
	//pStatic->SetInfoText( _T( "Displayed Rect" ) );
	//pStatic->SetItemValue( (LPARAM)0 );
	//pStatic->SetCtrlID( PROPERTY_SRCRECT );

	//pStatic = (CPropTreeItemStatic*)m_PropertyTree.InsertItem( new CPropTreeItemStatic(), pRoot );
	//pStatic->SetLabelText( _T( "ClientRect" ) );
	//pStatic->SetInfoText( _T( "Client Rect" ) );
	//pStatic->SetItemValue( (LPARAM)0 );
	//pStatic->SetCtrlID( PROPERTY_CLIENTRECT );
}

void CRectInfoDlg::SetMousePoint( CPoint& point )
{
	// MousePoint
	CPropTreeItemStatic* pStatic = (CPropTreeItemStatic*)m_PropertyTree.FindItem( PROPERTY_MOUSEPT );
	char buf[256];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s( buf, 256, "x : %d y : %d", point.x, point.y );
#else
	sprintf( buf, "x : %d y : %d", point.x, point.y );
#endif
	pStatic->SetItemValue( (LPARAM)buf );
	
	// RGBA
	BYTE r,g,b,a;
	BOOL bResult = g_DevILImage.GetPixel( point.x, point.y, &r, &g, &b, &a );

	pStatic = (CPropTreeItemStatic*)m_PropertyTree.FindItem( PROPERTY_RGBA );	
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if( bResult )
		sprintf_s( buf, 256, "r:%d g:%d b:%d a:%d", r, g, b, a );
	else
		sprintf_s( buf, 256, "Out of Region" );
#else
	if( bResult )
		sprintf( buf, "r:%d g:%d b:%d a:%d", r, g, b, a );
	else
		sprintf( buf, "Out of Region" );
#endif
	pStatic->SetItemValue( (LPARAM)buf );
	m_PropertyTree.UpdatedItems();
}

//void CRectInfoDlg::SetOriMousePoint( CPoint& point )
//{
//	CPropTreeItemStatic* pStatic = (CPropTreeItemStatic*)m_PropertyTree.FindItem( PROPERTY_ORIMOUSE );
//	char buf[256];
//	sprintf( buf, "x : %d y : %d", point.x, point.y );
//	pStatic->SetItemValue( (LPARAM)buf );
//	m_PropertyTree.UpdatedItems();
//}

//void CRectInfoDlg::SetOffset( CPoint& offset )
//{
//	CPropTreeItemStatic* pStatic = (CPropTreeItemStatic*)m_PropertyTree.FindItem( PROPERTY_OFFSET );
//	char buf[256];
//	sprintf( buf, "x : %d y : %d", offset.x, offset.y );
//	pStatic->SetItemValue( (LPARAM)buf );
//	m_PropertyTree.UpdatedItems();
//}

void CRectInfoDlg::SetSelectedRect( CRect& rect )
{
	CPropTreeItemStatic* pStatic = (CPropTreeItemStatic*)m_PropertyTree.FindItem( PROPERTY_RECT_XY );
	char buf[256];

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s( buf, 256, "x : %d y : %d", rect.left, rect.top );
#else
	sprintf( buf, "x : %d y : %d", rect.left, rect.top );
#endif
	pStatic->SetItemValue( (LPARAM)buf );

	pStatic = (CPropTreeItemStatic*)m_PropertyTree.FindItem( PROPERTY_RECT_WH );
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s( buf, 256, "w : %d h : %d", rect.Width(), rect.Height() );
#else
	sprintf( buf, "w : %d h : %d", rect.Width(), rect.Height() );
#endif
	pStatic->SetItemValue( (LPARAM)buf );

	m_PropertyTree.UpdatedItems();
}

//void CRectInfoDlg::SetSrcRect( CRect& rect )
//{
//	CPropTreeItemStatic* pStatic = (CPropTreeItemStatic*)m_PropertyTree.FindItem( PROPERTY_SRCRECT );
//	char buf[256];
//	sprintf( buf, "x : %d y : %d width : %d height : %d", rect.left, rect.top, rect.right, rect.bottom );
//	pStatic->SetItemValue( (LPARAM)buf );
//	m_PropertyTree.UpdatedItems();
//}
//
//void CRectInfoDlg::SetClientRect( CRect& rect )
//{
//	CPropTreeItemStatic* pStatic = (CPropTreeItemStatic*)m_PropertyTree.FindItem( PROPERTY_CLIENTRECT );
//	char buf[256];
//	sprintf( buf, "x : %d y : %d width : %d height : %d", rect.left, rect.top, rect.right, rect.bottom );
//	pStatic->SetItemValue( (LPARAM)buf );
//	m_PropertyTree.UpdatedItems();
//}