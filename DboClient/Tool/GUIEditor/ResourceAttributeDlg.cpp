// ResourceAttributeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "ResourceAttributeDlg.h"
#include ".\resourceattributedlg.h"
#include "ResourceLayer.h"
#include "LayerMisc.h"
#include "ResourceListDlg.h"
#include "GUIResourceView.h"


// CResourceAttributeDlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CResourceAttributeDlg, CDialog)
CResourceAttributeDlg::CResourceAttributeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResourceAttributeDlg::IDD, pParent)
{
}

CResourceAttributeDlg::~CResourceAttributeDlg()
{
}

void CResourceAttributeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CResourceAttributeDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ADD_RESOURCELIST, OnBnClickedAddResourlist)
	ON_BN_CLICKED(IDC_FIX_RESOURCELIST, &CResourceAttributeDlg::OnBnClickedFixResourcelist)
END_MESSAGE_MAP()


// CResourceAttributeDlg 메시지 처리기입니다.

BOOL CResourceAttributeDlg::OnInitDialog()
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

void CResourceAttributeDlg::OnSize(UINT nType, int cx, int cy)
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

void CResourceAttributeDlg::InitProperty()
{
	m_PropertyTree.DeleteAllItems();

	// Title
	CPropTreeItem* pRoot = m_PropertyTree.InsertItem( new CPropTreeItem() );
	pRoot->Expand();
	pRoot->SetLabelText( _T( "Resource Attribute" ) );
	pRoot->SetCtrlID( PROPERTY_CAPTION );

	// ResourceName
	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem( new CPropTreeItemEdit(), pRoot );
	pEdit->SetLabelText( _T( "Name" ) );
	pEdit->SetInfoText( _T( "Resource Name" ) );
	pEdit->SetValueFormat( CPropTreeItemEdit::ValueFormatText );
	pEdit->SetItemValue( (LPARAM)0 );
	pEdit->SetCtrlID( PROPERTY_RESOURCENAME );

	// ResourceHandle
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem( new CPropTreeItemEdit(), pRoot );
	pEdit->SetLabelText( _T( "Handle" ) );
	pEdit->SetInfoText( _T( "Resource Handle" ) );
	pEdit->SetValueFormat( CPropTreeItemEdit::ValueFormatNumber );
	pEdit->SetItemValue( (LPARAM)0 );
	pEdit->SetCtrlID( PROPERTY_HANDLE );

	// FileName
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem( new CPropTreeItemEdit(), pRoot );
	pEdit->SetLabelText( _T( "FileName" ) );
	pEdit->SetInfoText( _T( "Resource Filename" ) );
	pEdit->SetValueFormat( CPropTreeItemEdit::ValueFormatText );
	pEdit->SetItemValue( (LPARAM)0 );
	pEdit->SetCtrlID( PROPERTY_FILENAME );

}

void CResourceAttributeDlg::SetFileName( LPCTSTR szFileName )
{
	CPropTreeItemEdit* pEdit = (CPropTreeItemEdit*)m_PropertyTree.FindItem( PROPERTY_FILENAME );
	pEdit->SetItemValue( (LPARAM)szFileName );
	m_PropertyTree.UpdatedItems();
}

void CResourceAttributeDlg::SetBlank()
{
	CPropTreeItemEdit* pEdit = (CPropTreeItemEdit*)m_PropertyTree.FindItem( PROPERTY_RESOURCENAME );
	pEdit->SetItemValue( (LPARAM)"" );

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.FindItem( PROPERTY_HANDLE );
	pEdit->SetItemValue( (LPARAM)0 );

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.FindItem( PROPERTY_FILENAME );
	pEdit->SetItemValue( (LPARAM)"" );

	m_PropertyTree.UpdateData();
}


void CResourceAttributeDlg::OnBnClickedAddResourlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	AddData();
	AddCaptionToList();
}

void CResourceAttributeDlg::AddCaptionToList()
{
	CResourceLayer* pView = (CResourceLayer*)CLayerMisc::GetInstance().GetLayer( CLayerMisc::RESOURCE_LAYER );
	CResourceListDlg* pDlg = (CResourceListDlg*)pView->GetRollupDlg( CResourceLayer::RESOURCELISTDLG );
	
	pDlg->DisplayList();
}

void CResourceAttributeDlg::AddData()
{
	RESOURCEDATA* pResourceData = new RESOURCEDATA;

	if( pResourceData == NULL )
		return;

	if( !InputData( pResourceData ) )
	{
		delete pResourceData;
		return;
	}

	g_pDocument->GetDataManager().AddResourceData( pResourceData );
}

BOOL CResourceAttributeDlg::InputData( RESOURCEDATA* pResourceData, BOOL bFix /* = FALSE  */)
{
	CPropTreeItemEdit* pEdit = (CPropTreeItemEdit*)m_PropertyTree.FindItem( PROPERTY_RESOURCENAME );
	CString str( (LPCTSTR)pEdit->GetItemValue() );
	pResourceData->_strName = str;

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.FindItem( PROPERTY_HANDLE );
	pResourceData->_nHandle = (INT)pEdit->GetItemValue();

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.FindItem( PROPERTY_FILENAME );
	str = (LPCTSTR)pEdit->GetItemValue();
	pResourceData->_strFileName = str;

	if( !InputCheck( pResourceData, bFix ) )
		return FALSE;

	return TRUE;
}

void CResourceAttributeDlg::FillResourceProperty( RESOURCEDATA* pResourceData )
{
	if( !pResourceData )
	{
		AfxMessageBox( "해당 리소스가 없습니다" );
		g_pResourceView->LoadImage( CString( "" ), CString( "" ) );
		return;
	}

	// 해당하는 그림 표시.
	CString strFullPath = pResourceData->_strFileName;
	if( !g_pResourceView->LoadImage( strFullPath, pResourceData->_strFileName ) )
	{
		strFullPath = g_pDocument->GetDataManager().GetWorkDir() + CString( "\\" ) + pResourceData->_strFileName;
		
		if( !g_pResourceView->LoadImage( strFullPath, pResourceData->_strFileName ) )
		{
			strFullPath = g_pDocument->GetDataManager().GetWorkDir() + CString( "\\texture\\gui\\" ) + pResourceData->_strFileName;

			if( !g_pResourceView->LoadImage( strFullPath, pResourceData->_strFileName ) )
			{
				AfxMessageBox( "해당 이미지가 없습니다" );
			}
		}
	}
	
	// 데이터 표시.
	CPropTreeItemEdit* pEdit = (CPropTreeItemEdit*)m_PropertyTree.FindItem( PROPERTY_RESOURCENAME );
	pEdit->SetItemValue( (LPARAM)( (LPCTSTR)pResourceData->_strName ) );

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.FindItem( PROPERTY_HANDLE );
	pEdit->SetItemValue( (LPARAM)pResourceData->_nHandle );

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.FindItem( PROPERTY_FILENAME );
    pEdit->SetItemValue( (LPARAM)( (LPCTSTR)pResourceData->_strFileName ) );

	m_PropertyTree.UpdatedItems();
}

void CResourceAttributeDlg::OnBnClickedFixResourcelist()
{
	// TODO: Add your control notification handler code here
	CResourceLayer* pView = (CResourceLayer*)CLayerMisc::GetInstance().GetLayer( CLayerMisc::RESOURCE_LAYER );
	CResourceListDlg* pDlg = (CResourceListDlg*)pView->GetRollupDlg( CResourceLayer::RESOURCELISTDLG );

	INT nFocusID =  pDlg->GetFocusID();

	if( nFocusID < 1 )
	{
		AfxMessageBox( "수정할 아이템을 클릭하세요" );
		return;
	}

	RESOURCEDATA* pData = g_pDocument->GetDataManager().GetResourceData( nFocusID );

	if( !pData )
		return;

	if( !InputData( pData, TRUE ) )
		return;	

	pDlg->DisplayList();
}

BOOL CResourceAttributeDlg::InputCheck( RESOURCEDATA* pResourceData, BOOL bFIX /* = FALSE  */)
{
	if( pResourceData->_strName == CString( "" ) )
	{
		AfxMessageBox( "이름을 입력해야 합니다" );
		return FALSE;
	}

	if( g_pDocument->GetDataManager().GetResourceData( pResourceData->_strName )  && !bFIX )
	{
		AfxMessageBox( "같은 이름을 입력할 수 없습니다" );
		return FALSE;
	}

	if( pResourceData->_strFileName == CString( "" ) )
	{
		AfxMessageBox( "파일이름을 입력해야 합니다" );
		return FALSE;
	}

	return TRUE;
}
