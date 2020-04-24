// TSQuestTextForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "TSDoc.h"
#include "TSQuestTextForm.h"
#include "DirDialog.h"
#include "TableFileNameList.h"
#include "Table.h"
#include "TableContainer.h"
#include "TableFileNameList.h"
#include "NtlBitFlagManager.h"
#include "QuestTextDataTable.h"


// CTSQuestTextForm

CTSQuestTextForm* g_pTSQuestTextForm = NULL;

IMPLEMENT_DYNCREATE(CTSQuestTextForm, CFormView)

CTSQuestTextForm::CTSQuestTextForm()
	: CFormView(CTSQuestTextForm::IDD)
	, m_pTableMng( NULL )
	, m_strPath(_T(""))
{
	g_pTSQuestTextForm = this;
}

CTSQuestTextForm::~CTSQuestTextForm()
{
	g_pTSQuestTextForm = NULL;
}

void CTSQuestTextForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_QUEST_TEXT_PATH_EDIT, m_strPath);
	DDX_Control(pDX, IDC_QUEST_TEXT_QID_FILTER_CUR_QID_EDITOR, m_ctrQIDFilter);
	DDX_Control(pDX, IDC_QUEST_TEXT_QID_FILTER_LISTVIEW, m_ctrQIDFilterList);
	DDX_Control(pDX, IDC_QUEST_TEXT_QID_FINDER_FIND_QID_EDITOR, m_ctrQIDFinder);
	DDX_Control(pDX, IDC_QUEST_TEXT_QID_FINDER_LISTVIEW, m_ctrQIDFinderList);
}

BEGIN_MESSAGE_MAP(CTSQuestTextForm, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_QUEST_TEXT_PATH_BTN, &CTSQuestTextForm::OnBnClickedQuestTextPathBtn)
	ON_EN_CHANGE(IDC_QUEST_TEXT_QID_FILTER_CUR_QID_EDITOR, &CTSQuestTextForm::OnEnChangeQuestTextQidFilterCurQidEditor)
	ON_EN_CHANGE(IDC_QUEST_TEXT_QID_FINDER_FIND_QID_EDITOR, &CTSQuestTextForm::OnEnChangeQuestTextQidFinderFindQidEditor)
END_MESSAGE_MAP()


// CTSQuestTextForm 진단입니다.

#ifdef _DEBUG
void CTSQuestTextForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTSQuestTextForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTSQuestTextForm 메시지 처리기입니다.
void CTSQuestTextForm::OnSize(UINT nType, int cx, int cy)
{
	CSize size( cx, cy );
	SetScaleToFitSize( size );

	int nHeight = 0;
	CRect rect( 0, 0, 0, 0 );
										  /* 0	 1   2   3   4   5   6   7   8   9*/
	int narHeightListExceptListView[10] = { 20, 20, 10, 20, 25, 20, 10, 25, 20, 10 };

	int nSum = 0;
	for ( int i = 0; i < 10; ++i )
	{
		nSum += narHeightListExceptListView[i];
	}
	
	int arnListViewHeight[2] = { (cy - nSum) * 4 / 5, (cy - nSum) / 5 };

	CWnd* pWnd = NULL;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Quest path
	//
	//////////////////////////////////////////////////////////////////////////

	nHeight = narHeightListExceptListView[0];

	pWnd = GetDlgItem( IDC_QUEST_TEXT_PATH_STATIC );
	if ( pWnd && pWnd->GetSafeHwnd() )
	{
		pWnd->GetClientRect( rect );

		rect.left += 5;
		rect.right += 5;
		rect.top = nHeight;
		rect.bottom = rect.top + narHeightListExceptListView[1];

		pWnd->MoveWindow( rect );
	}

	pWnd = GetDlgItem( IDC_QUEST_TEXT_PATH_BTN );
	if ( pWnd && pWnd->GetSafeHwnd() )
	{
		rect.left += rect.right + 5;
		rect.right += 50;
		rect.top = nHeight - 2;
		rect.bottom = rect.top + narHeightListExceptListView[1] - 2;

		pWnd->MoveWindow( rect );
	}

	nHeight += narHeightListExceptListView[1];

	nHeight += narHeightListExceptListView[2];

	pWnd = GetDlgItem( IDC_QUEST_TEXT_PATH_EDIT );
	if ( pWnd && pWnd->GetSafeHwnd() )
	{
		rect.SetRect( 5, nHeight, cx - 5, nHeight + narHeightListExceptListView[3] );
		pWnd->MoveWindow( rect );
	}

	nHeight += narHeightListExceptListView[3];

	//////////////////////////////////////////////////////////////////////////
	//
	//	Quest filter
	//
	//////////////////////////////////////////////////////////////////////////

	nHeight += narHeightListExceptListView[4];

	pWnd = GetDlgItem( IDC_QUEST_TEXT_QID_FILTER_STATIC );
	if ( pWnd && pWnd->GetSafeHwnd() )
	{
		pWnd->GetClientRect( rect );

		rect.left += 5;
		rect.right += 5;
		rect.top = nHeight;
		rect.bottom = rect.top + narHeightListExceptListView[5];

		pWnd->MoveWindow( rect );
	}

	pWnd = GetDlgItem( IDC_QUEST_TEXT_QID_FILTER_CUR_QID_EDITOR );
	if ( pWnd && pWnd->GetSafeHwnd() )
	{
		rect.left += rect.right + 5;
		rect.right = cx - 5;
		rect.top = nHeight;
		rect.bottom = rect.top + narHeightListExceptListView[5];

		pWnd->MoveWindow( rect );
	}

	nHeight += narHeightListExceptListView[5];

	nHeight += narHeightListExceptListView[6];

	pWnd = GetDlgItem( IDC_QUEST_TEXT_QID_FILTER_LISTVIEW );
	if ( pWnd && pWnd->GetSafeHwnd() )
	{
		rect.SetRect( 5, nHeight, cx - 5, nHeight + arnListViewHeight[0] );
		pWnd->MoveWindow( rect );
	}

	nHeight += arnListViewHeight[0];

	//////////////////////////////////////////////////////////////////////////
	//
	//	Quest finder
	//
	//////////////////////////////////////////////////////////////////////////

	nHeight += narHeightListExceptListView[7];

	pWnd = GetDlgItem( IDC_QUEST_TEXT_QID_FINDER_STATIC );
	if ( pWnd && pWnd->GetSafeHwnd() )
	{
		pWnd->GetClientRect( rect );

		rect.left += 5;
		rect.right += 5;
		rect.top = nHeight;
		rect.bottom = rect.top + narHeightListExceptListView[8];

		pWnd->MoveWindow( rect );
	}

	pWnd = GetDlgItem( IDC_QUEST_TEXT_QID_FINDER_FIND_QID_EDITOR );
	if ( pWnd && pWnd->GetSafeHwnd() )
	{
		rect.left += rect.right + 5;
		rect.right = cx - 5;
		rect.top = nHeight;
		rect.bottom = rect.top + narHeightListExceptListView[8];

		pWnd->MoveWindow( rect );
	}

	nHeight += narHeightListExceptListView[8];

	nHeight += narHeightListExceptListView[9];

	pWnd = GetDlgItem( IDC_QUEST_TEXT_QID_FINDER_LISTVIEW );
	if ( pWnd && pWnd->GetSafeHwnd() )
	{
		rect.SetRect( 5, nHeight, cx - 5, nHeight + arnListViewHeight[1] - 20 );
		pWnd->MoveWindow( rect );
	}
}

void CTSQuestTextForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_ctrQIDFilterList.Init();

	m_ctrQIDFinderList.Init();
}

void CTSQuestTextForm::OnBnClickedQuestTextPathBtn()
{
	CDirDialog clDir( m_strPath, NULL, NULL );
	clDir.m_ofn.lpstrTitle = _T("Table folder path");
	if ( IDOK != clDir.DoModal() ) return;

	m_strPath = clDir.GetPath();

	DeleteQuestTextTable();

	CreateQuestTextTable();

	UpdateData( FALSE );
}

void CTSQuestTextForm::OnEnChangeQuestTextQidFilterCurQidEditor()
{
	if ( NULL == m_pTableMng )
	{
		AfxMessageBox( _T("Quest text table이 로딩되어 있지 않습니다") );
		return;
	}

	CQuestTextDataTable* pQTbl = m_pTableMng->GetQuestTextDataTable();

	if ( NULL == pQTbl )
	{
		AfxMessageBox( _T("Quest text table이 로딩되어 있지 않습니다") );
		return;
	}

	CString strText;
	m_ctrQIDFilter.GetWindowText( strText );

	if ( !IsNumeric( strText ) )
	{
		AfxMessageBox( _T("숫자만 입력이 가능합니다") );
		return;
	}

	unsigned int uiQID = atoi( strText.GetBuffer() );

	unsigned int uiBegin	= uiQID * 100;
	unsigned int uiEnd		= uiBegin + 99;

	std::vector< sQUEST_TEXT_DATA_TBLDAT* > vecQTextDataList;
	pQTbl->FindDataRange( uiBegin, uiEnd, vecQTextDataList );

	m_ctrQIDFilterList.ClearAll();

	std::vector< sQUEST_TEXT_DATA_TBLDAT* >::iterator it = vecQTextDataList.begin();
	for ( ; it != vecQTextDataList.end(); ++it )
	{
		sQUEST_TEXT_DATA_TBLDAT* pData = *it;

		int nBufferSize = pData->wstrText.size() * 3;
		char* pMBBuffer = new char[nBufferSize];
		memset( pMBBuffer, 0, sizeof(char) * (nBufferSize) );

		WideCharToMultiByte( GetACP(), 0, pData->wstrText.c_str(), pData->wstrText.size(), pMBBuffer, nBufferSize, NULL, NULL );

		m_ctrQIDFilterList.AddEntity( pData->tblidx, pMBBuffer);

		delete [] pMBBuffer;
	}
}

void CTSQuestTextForm::OnEnChangeQuestTextQidFinderFindQidEditor()
{
	if ( NULL == m_pTableMng )
	{
		AfxMessageBox( _T("Quest text table이 로딩되어 있지 않습니다") );
		return;
	}

	CQuestTextDataTable* pQTbl = m_pTableMng->GetQuestTextDataTable();

	if ( NULL == pQTbl )
	{
		AfxMessageBox( _T("Quest text table이 로딩되어 있지 않습니다") );
		return;
	}

	CString strText;
	m_ctrQIDFinder.GetWindowText( strText );

	if ( !IsNumeric( strText ) )
	{
		AfxMessageBox( _T("숫자만 입력이 가능합니다") );
		return;
	}

	unsigned int uiQTextTblIdx = atoi( strText.GetBuffer() );

	sQUEST_TEXT_DATA_TBLDAT* pData = (sQUEST_TEXT_DATA_TBLDAT*) pQTbl->FindData( uiQTextTblIdx );

	m_ctrQIDFinderList.ClearAll();

	if ( pData )
	{
		int nBufferSize = pData->wstrText.size() * 3;
		char* pMBBuffer = new char[nBufferSize];
		memset( pMBBuffer, 0, sizeof(char) * (nBufferSize) );

		WideCharToMultiByte( GetACP(), 0, pData->wstrText.c_str(), pData->wstrText.size(), pMBBuffer, nBufferSize, NULL, NULL );

		m_ctrQIDFinderList.AddEntity( pData->tblidx, pMBBuffer);

		delete [] pMBBuffer;
	}
}

void CTSQuestTextForm::SetCurSelQuestID( CString& strTriggerID )
{
	if ( NULL == m_pTableMng )
	{
		return;
	}

	if ( !IsNumeric( strTriggerID ) )
	{
		return;
	}

	m_ctrQIDFilter.SetWindowText( strTriggerID );

	UpdateData( FALSE );
}

bool CTSQuestTextForm::CreateQuestTextTable( void )
{
	CNtlBitFlagManager flagManager;
	if (false == flagManager.Create(CTableContainer::TABLE_COUNT))
	{
		return false;
	}

	CTableFileNameList fileNameList;
	if (false == fileNameList.Create())
	{
		return false;
	}

	flagManager.Set(CTableContainer::TABLE_QUEST_TEXT_DATA);				

	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_TEXT_DATA, "Table_Quest_Text_Data");

	m_pTableMng = new CTableContainer;

	CTable::eLOADING_METHOD eLoadMethod = CTable::LOADING_METHOD_BINARY;

	if ( !m_pTableMng->Create( flagManager, m_strPath.GetBuffer(), &fileNameList, eLoadMethod, GetACP(), NULL ) )
	{
		delete m_pTableMng;
		m_pTableMng = NULL;

		CString strMsg;
		strMsg.Format( _T("Can not load the quest text table. [%s]"), m_strPath );
		AfxMessageBox( strMsg );

		return false;
	}

	return true;
}

void CTSQuestTextForm::DeleteQuestTextTable( void )
{
	if ( m_pTableMng )
	{
		m_pTableMng->Destroy();
		m_pTableMng = NULL;
	}
}

bool CTSQuestTextForm::IsNumeric( CString& strVal )
{
	int nSize = strVal.GetLength();

	for ( int i = 0; i < nSize; ++i )
	{
		if ( !::isdigit( strVal[i] ) )
		{
			return false;
		}
	}

	return true;
}
