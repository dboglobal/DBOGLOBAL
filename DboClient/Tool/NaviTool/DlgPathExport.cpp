// DlgPathExport.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NaviTool.h"
#include "DlgPathExport.h"
#include "PathEngine.h"
#include "NaviToolUtil.h"

// CDlgPathExport 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgPathExport, CDialog)

CDlgPathExport::CDlgPathExport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPathExport::IDD, pParent)
{

}

CDlgPathExport::~CDlgPathExport()
{
}

void CDlgPathExport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EXPORT, m_editListCtrlExport);
}


BEGIN_MESSAGE_MAP(CDlgPathExport, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_BTN_PATH_EXPORT_ALL, &CDlgPathExport::OnBnClickedBtnPathExportAll)
	ON_BN_CLICKED(ID_BTN_PATH_EXPORT_SELECT, &CDlgPathExport::OnBnClickedBtnPathExportSelect)
END_MESSAGE_MAP()

BOOL CDlgPathExport::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Read all that is currently loaded.
	vecdef_WorldIDList list;
	CPathEngine::GetInstance()->GetListImportedWorldIDList( list );

	if( list.empty() )
	{
		// ErrorMessage 출력
		::AfxMessageBox( "Imported world data is empty!" );
		return TRUE;
	}

	CRect rect;
	m_editListCtrlExport.GetClientRect( rect );
	int nResourceIDWidth = rect.Width()/4;
	int nGroupIDWidth = rect.Width() - nResourceIDWidth;
	char* listHeader[3] = {"", "ResID", "GroupID" };
	int nWidth[3] = {0, nResourceIDWidth, nGroupIDWidth};
	LV_COLUMN colum;
	for( int i=1; i < 3; ++i )
	{
		colum.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		colum.fmt = LVCFMT_CENTER;
		colum.pszText = listHeader[i];
		colum.iSubItem = i;
		colum.cx = nWidth[i];
		m_editListCtrlExport.InsertColumn(i, &colum);
	}

	m_editListCtrlExport.SetExtendedStyle(
		m_editListCtrlExport.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT | 
		LVS_EX_GRIDLINES);

	int nCount = 0;
	CHAR chResourceID[16];
	CHAR chGroupID[16];
	LVITEM item;
	for each( unsigned int i in list )
	{
		sprintf_s( chResourceID, 16, "%u", i );
		sprintf_s( chGroupID, 16, "%u", 0 );

		item.mask = LVIF_TEXT;
		item.iItem = nCount;
		item.iSubItem = 0;
		item.pszText = chResourceID;
		m_editListCtrlExport.InsertItem( &item );

		item.mask = LVIF_TEXT;
		item.iItem = nCount;
		item.iSubItem = 1;
		item.pszText = chGroupID;
		m_editListCtrlExport.SetItem( &item );

		nCount++;
	}

	UpdateData(FALSE);
	// Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CDlgPathExport::GetExportList( mapdef_ExportList& list )
{
	list = m_mapExportList;
	
	return TRUE;
}

void CDlgPathExport::OnBnClickedBtnPathExportAll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog( ID_BTN_PATH_EXPORT_ALL );
}

void CDlgPathExport::OnBnClickedBtnPathExportSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	int nCount = m_editListCtrlExport.GetItemCount();
	
	for( int i=0; i < nCount; ++i )
	{
		CString strResourceID = m_editListCtrlExport.GetItemText( i, 0 );
		CString strGroupID = m_editListCtrlExport.GetItemText( i, 1 );

		int nResourceID = atoi( strResourceID.GetBuffer() );

		vecdef_GroupIDList groupIDList;

		std::vector<std::string> vecString;
		StringTokenize(vecString, std::string(strGroupID.GetBuffer()), std::string(","));
		for each( std::string str in vecString )
		{
			int nGroupId = atoi( str.c_str() );
			groupIDList.push_back( nGroupId );
		}

		m_mapExportList.insert( std::make_pair( nResourceID, groupIDList ) );
	}
	
	EndDialog( ID_BTN_PATH_EXPORT_SELECT );
}
