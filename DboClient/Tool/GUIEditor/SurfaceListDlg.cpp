// SurfaceListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "GUIEditorDoc.h"
#include "SurfaceListDlg.h"
#include ".\surfacelistdlg.h"
#include "SurfaceLayer.h"
#include "SurfaceAttributeDlg.h"
#include "LayerMisc.h"


// CSurfaceListDlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CSurfaceListDlg, CDialog)
CSurfaceListDlg::CSurfaceListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSurfaceListDlg::IDD, pParent)
{
	m_bInitialized = FALSE;
}

CSurfaceListDlg::~CSurfaceListDlg()
{
}

void CSurfaceListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSurfaceListDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_REMOVE_SURFACELIST, &CSurfaceListDlg::OnBnClickedRemoveSurfacelist)
	ON_BN_CLICKED(IDC_SAVE_SURFACELIST, &CSurfaceListDlg::OnBnClickedSaveSurfacelist)
	ON_BN_CLICKED(IDC_CLEAR_SURFACELIST, &CSurfaceListDlg::OnBnClickedClearSurfacelist)
END_MESSAGE_MAP()


// CSurfaceListDlg 메시지 처리기입니다.

BOOL CSurfaceListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	DWORD dwStyle;
	CRect rc;

	// PTS_NOTIFY - CPropTree will send notification messages to the parent window
	dwStyle = WS_CHILD | WS_VISIBLE | PTS_NOTIFY;

	// Init the control's size to cover the entire client area
	CWnd* pWindow = GetDlgItem( IDC_PROPERTY_STATIC );
	if( pWindow )
	{
		pWindow->GetWindowRect( rc );
		ScreenToClient( &rc );
	}

	//Create CPropTree Control
	m_PropertyTree.Create( dwStyle, rc, this, IDC_PROPERTY_STATIC );

	CPropTreeItem* pRoot;
	pRoot = m_PropertyTree.InsertItem( new CPropTreeItem() );
	pRoot->SetLabelText( _T( "Surface List" ) );
	pRoot->SetCtrlID( PROPERTY_NONE );

	// End of Initialize
	m_bInitialized = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSurfaceListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( m_bInitialized )
	{
		CWnd* pWindow = GetDlgItem( IDC_PROPERTY_STATIC );
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

void CSurfaceListDlg::DisplayList()
{
	m_PropertyTree.DeleteAllItems();

	// Title
	CPropTreeItem* pRoot = m_PropertyTree.InsertItem( new CPropTreeItem() );
	pRoot->Expand();
	pRoot->SetLabelText( _T( "Surface List" ) );
	pRoot->SetCtrlID( PROPERTY_CAPTION );

	CDataManager::listSurfaceData listSurface = g_pDocument->GetDataManager().GetSurfaceList();
	CDataManager::listSurfaceData::iterator it;
	INT i;

	for( i = 0, it = listSurface.begin() ; it != listSurface.end() ; ++it, ++i )
	{
		AddItem( (*it)->_strName, i + 1, pRoot );
	}	   

	m_PropertyTree.UpdatedItems();
}

void CSurfaceListDlg::AddItem( CString strCaption, INT nIndex, CPropTreeItem* pRoot )
{
	// SurfaceName
	CPropTreeItemStatic* pEdit = (CPropTreeItemStatic*)m_PropertyTree.InsertItem( new CPropTreeItemEdit(), pRoot );
	CString strTemp;
	strTemp.Format( "%d", nIndex );
	pEdit->SetLabelText( _T( (LPCTSTR)strTemp ) );
	strTemp.Format( "Item %d", nIndex );
	pEdit->SetInfoText( _T( (LPCTSTR)strTemp ) );
	pEdit->SetItemValue( (LPARAM)((LPCTSTR)strCaption) );
	pEdit->SetCtrlID( nIndex );
}

void CSurfaceListDlg::OnBnClickedRemoveSurfacelist()
{
	// TODO: Add your control notification handler code here
	CPropTreeItem* pFocusedItem = m_PropertyTree.GetFocusedItem();

	if( pFocusedItem == NULL )
		return;

	INT nID = pFocusedItem->GetCtrlID();

	if( nID < 1 )
		return;

	g_pDocument->GetDataManager().RemoveSurfaceData( nID );

	DisplayList();
}

BOOL CSurfaceListDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	NMPROPTREE* pInfo = (NMPROPTREE*)lParam;

	switch( pInfo->hdr.code )
	{
	case PTN_SELCHANGE:
		ShowAttribute( pInfo->pItem );
		break;
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CSurfaceListDlg::ShowAttribute( CPropTreeItem* pItem )
{
	if( !pItem )
		return;

	INT nCtrlID = pItem->GetCtrlID();

	CSurfaceLayer* pView = (CSurfaceLayer*)CLayerMisc::GetInstance().GetLayer( CLayerMisc::SURFACE_LAYER );
	CSurfaceAttributeDlg* pDlg = (CSurfaceAttributeDlg*)pView->GetRollupDlg( CSurfaceLayer::SURFACEATTRIBUTEDLG );
	SURFACEDATA* pData = g_pDocument->GetDataManager().GetSurfaceData( nCtrlID );
	pDlg->FillSurfaceProperty( pData );
}

void CSurfaceListDlg::OnBnClickedSaveSurfacelist()
{
	// TODO: Add your control notification handler code hre
	g_pDocument->GetDataManager().SaveSrfFile();
}

INT CSurfaceListDlg::GetFocusID()
{
	CPropTreeItem* pItem = m_PropertyTree.GetFocusedItem();
	
	return pItem ? pItem->GetCtrlID() : 0 ;
}

void CSurfaceListDlg::OnBnClickedClearSurfacelist()
{
	// TODO: Add your control notification handler code here
	g_pDocument->GetDataManager().ClearSrfFile();	
	DisplayList();
}
