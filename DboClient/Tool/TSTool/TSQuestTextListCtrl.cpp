// MainMenuListView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "TSQuestTextListCtrl.h"


// CTSQuestTextListCtrl

IMPLEMENT_DYNCREATE(CTSQuestTextListCtrl, CListCtrl)

CTSQuestTextListCtrl::CTSQuestTextListCtrl()
{
}

CTSQuestTextListCtrl::~CTSQuestTextListCtrl()
{
}

BEGIN_MESSAGE_MAP(CTSQuestTextListCtrl, CListCtrl)
END_MESSAGE_MAP()


// CTSQuestTextListCtrl 진단입니다.

void CTSQuestTextListCtrl::Init( void )
{
	InsertColumn( 0, _T("Index"), LVCFMT_LEFT, 50, 0 );
	InsertColumn( 1, _T("String"), LVCFMT_LEFT, 200, 1 );
}

void CTSQuestTextListCtrl::ClearAll( void )
{
	DeleteAllItems();
	m_aStringList.RemoveAll();
}

void CTSQuestTextListCtrl::AddEntity( unsigned int uiTableIndex, const CString& strQText )
{
	sENTITY sEntity;
	sEntity.strTblIdx.Format( _T("%u"), uiTableIndex );
	sEntity.strQText = strQText;

	int nIdx = (int)m_aStringList.Add( sEntity );

	LVITEM lvItem;

	ZeroMemory( &lvItem, sizeof(lvItem) );

	lvItem.iItem = nIdx;

	lvItem.mask = TVIF_TEXT|TVIF_PARAM|TVIF_STATE|TVIF_DI_SETITEM;
	lvItem.iSubItem = 0;
	lvItem.pszText = (LPSTR)m_aStringList[nIdx].strTblIdx.GetString();
	lvItem.lParam = (LPARAM)m_aStringList[nIdx].strQText.GetString();
	InsertItem( &lvItem );

	lvItem.mask = TVIF_TEXT;
	lvItem.iSubItem = 1;
	lvItem.pszText = (LPSTR)m_aStringList[nIdx].strQText.GetString();
	SetItem( &lvItem );
}

// CTSQuestTextListCtrl 메시지 처리기입니다.
