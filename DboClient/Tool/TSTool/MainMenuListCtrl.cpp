// MainMenuListView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "MainMenuListCtrl.h"
#include "TSDoc.h"
#include "EntityRegisterDiag.h"
#include "Attr_Trigger.h"
#include "OptionListBox.h"
#include "TSFrame.h"
#include "TSQuestTextForm.h"


// CMainMenuListCtrl

IMPLEMENT_DYNCREATE(CMainMenuListCtrl, CListCtrl)

CMainMenuListCtrl::CMainMenuListCtrl()
{
}

CMainMenuListCtrl::~CMainMenuListCtrl()
{
}

BEGIN_MESSAGE_MAP(CMainMenuListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CMainMenuListCtrl::OnNMDblclk)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CMainMenuListCtrl::OnLvnItemchanged)
END_MESSAGE_MAP()


// CMainMenuListCtrl 진단입니다.

#ifdef _DEBUG
void CMainMenuListCtrl::AssertValid() const
{
	CListCtrl::AssertValid();
}

#ifndef _WIN32_WCE
void CMainMenuListCtrl::Dump(CDumpContext& dc) const
{
	CListCtrl::Dump(dc);
}
#endif
#endif //_DEBUG


void CMainMenuListCtrl::Init( void )
{
	InsertColumn( 1, _T("Name"), LVCFMT_LEFT, 1000, -1 );
}

void CMainMenuListCtrl::ClearAll( void )
{
	DeleteAllItems();
	m_aStringList.RemoveAll();
}

void CMainMenuListCtrl::AddList( const CString& strName )
{
	int nIdx = (int)m_aStringList.Add( strName );

	LVITEM lvItem;
	ZeroMemory( &lvItem, sizeof(lvItem) );
	lvItem.mask = TVIF_TEXT|TVIF_PARAM;
	lvItem.iItem = nIdx;
	lvItem.pszText = (LPSTR)m_aStringList[nIdx].GetString();
	lvItem.lParam = (LPARAM)m_aStringList[nIdx].GetString();
	InsertItem( &lvItem );
}

// CMainMenuListCtrl 메시지 처리기입니다.


BOOL CMainMenuListCtrl::OnEraseBkgnd(CDC* pDC)
{
	// 배경을 다시 그리지 않는다
	return TRUE;
}

void CMainMenuListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if ( -1 == pNMLV->iItem )
	{
		*pResult = 0;
		return;
	}

	if ( CTSDoc::GetInstance()->IsProjectLoaded() )
	{
		static char szBuf[128];
		LVITEM item;
		item.mask = TVIF_HANDLE|TVIF_TEXT|TVIF_PARAM;
		item.iItem = pNMLV->iItem;
		item.iSubItem = pNMLV->iSubItem;
		item.cchTextMax = 127;
		item.pszText = szBuf;
		item.lParam = 0;
		GetItem( &item );

		CString strPath = CTSDoc::GetInstance()->GetProjectObject()->GetProjectPath();
		strPath += _T("\\");
		strPath += (TCHAR*)item.lParam;

		if ( CTSDoc::GetInstance()->IsModified() ) CTSDoc::GetInstance()->DoFileSave();

		CTSDoc::GetInstance()->SetPathName( strPath );
		CTSDoc::GetInstance()->OnOpenDocument( strPath );

		CTSDoc::GetInstance()->SelMainGroup();

		// Trigger 속성
		CTSTrigger& clTrig = CTSDoc::GetInstance()->GetProjectObject()->GetTrigger();

		CAttr_Trigger AttrPage;
		AttrPage.AddAttr( _T("tid"), clTrig.GetAttr( _T("tid") ) );
		AttrPage.AddAttr( _T("rq"), clTrig.GetAttr( _T("rq") ) );
		AttrPage.AddAttr( _T("sq"), clTrig.GetAttr( _T("sq") ) );
		AttrPage.AddAttr( _T("sm"), clTrig.GetAttr( _T("sm") ) );
		AttrPage.AddAttr( _T("title"), clTrig.GetAttr( _T("title") ) );

		COptionListBox clListBox;
		CEntityRegisterDiag clDiag;
		clDiag.SetListControl( &clListBox );

		clDiag.AddAttrPage( &AttrPage );

		if ( IDOK == clDiag.DoModal() && clDiag.GetEditedAttrPage() )
		{
			CString strValue;

			clTrig.ClearAttr();

			strValue = AttrPage.GetAttr( _T("tid") );
			if ( strValue.GetLength() != 0 ) clTrig.AddAttr( _T("tid"), strValue );

			strValue = AttrPage.GetAttr( _T("rq") );
			if ( strValue.GetLength() != 0 ) clTrig.AddAttr( _T("rq"), strValue );

			strValue = AttrPage.GetAttr( _T("sq") );
			if ( strValue.GetLength() != 0 ) clTrig.AddAttr( _T("sq"), strValue );

			strValue = AttrPage.GetAttr( _T("sm") );
			if ( strValue.GetLength() != 0 ) clTrig.AddAttr( _T("sm"), strValue );

			strValue = AttrPage.GetAttr( _T("title") );
			if ( strValue.GetLength() != 0 ) clTrig.AddAttr( _T("title"), strValue );

			CTSDoc::GetInstance()->OnShapeChange();

			CTSFrame* pMainFrame = (CTSFrame*)AfxGetMainWnd();
			if ( pMainFrame )
			{
				CTSProject* pProj = CTSDoc::GetInstance()->GetProjectObject();
				if ( pProj )
				{
					CString strTriggerID = pProj->GetTrigger().GetAttr( _T("tid") );
					CString strTitle = pProj->GetTrigger().GetAttr( _T("title") );
					CString strRepeat = pProj->GetTrigger().GetAttr( _T("rq") );
					CString strShare = pProj->GetTrigger().GetAttr( _T("sq") );

					if ( g_pTSQuestTextForm )
					{
						g_pTSQuestTextForm->SetCurSelQuestID( strTriggerID );
					}

					pMainFrame->UpdateStatusBar( strTriggerID.IsEmpty() ? CString(_T("INVALID")) : strTriggerID,
												 strTitle.IsEmpty() ? CString(_T("INVALID")) : strTitle,
												 strRepeat.IsEmpty() ? CString(_T("INVALID")) : strRepeat,
												 strShare.IsEmpty() ? CString(_T("INVALID")) : strShare );
				}
			}
		}
	}

	*pResult = 0;
}

void CMainMenuListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if ( -1 == pNMLV->iItem )
	{
		*pResult = 0;
		return;
	}

	if ( CTSDoc::GetInstance()->IsProjectLoaded() )
	{
		static char szBuf[128];
		LVITEM item;
		item.mask = TVIF_HANDLE|TVIF_TEXT|TVIF_PARAM;
		item.iItem = pNMLV->iItem;
		item.iSubItem = pNMLV->iSubItem;
		item.cchTextMax = 127;
		item.pszText = szBuf;
		item.lParam = 0;
		GetItem( &item );

		CString strPath = CTSDoc::GetInstance()->GetProjectObject()->GetProjectPath();
		strPath += _T("\\");
		strPath += (TCHAR*)item.lParam;

		if ( CTSDoc::GetInstance()->IsModified() ) CTSDoc::GetInstance()->DoFileSave();

		CTSDoc::GetInstance()->SetPathName( strPath );
		CTSDoc::GetInstance()->OnOpenDocument( strPath );

		CTSDoc::GetInstance()->SelMainGroup();

		CTSFrame* pMainFrame = (CTSFrame*)AfxGetMainWnd();
		if ( pMainFrame )
		{
			CTSProject* pProj = CTSDoc::GetInstance()->GetProjectObject();
			if ( pProj )
			{
				CString strTriggerID = pProj->GetTrigger().GetAttr( _T("tid") );
				CString strTitle = pProj->GetTrigger().GetAttr( _T("title") );
				CString strRepeat = pProj->GetTrigger().GetAttr( _T("rq") );
				CString strShare = pProj->GetTrigger().GetAttr( _T("sq") );

				if ( g_pTSQuestTextForm )
				{
					g_pTSQuestTextForm->SetCurSelQuestID( strTriggerID );
				}

				pMainFrame->UpdateStatusBar( strTriggerID.IsEmpty() ? CString(_T("INVALID")) : strTriggerID,
											 strTitle.IsEmpty() ? CString(_T("INVALID")) : strTitle,
											 strRepeat.IsEmpty() ? CString(_T("INVALID")) : strRepeat,
											 strShare.IsEmpty() ? CString(_T("INVALID")) : strShare );
			}
		}
	}

	*pResult = 0;
}
