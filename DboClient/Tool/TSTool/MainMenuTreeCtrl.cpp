// MainMenuTreeCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "TSDoc.h"
#include "MainMenuTreeCtrl.h"


// CMainMenuTreeCtrl

IMPLEMENT_DYNCREATE(CMainMenuTreeCtrl, CTreeCtrl)

CMainMenuTreeCtrl::CMainMenuTreeCtrl()
{
}

CMainMenuTreeCtrl::~CMainMenuTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(CMainMenuTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CMainMenuTreeCtrl::OnTvnSelchanged)
END_MESSAGE_MAP()


// CMainMenuTreeCtrl 진단입니다.

#ifdef _DEBUG
void CMainMenuTreeCtrl::AssertValid() const
{
	CTreeCtrl::AssertValid();
}

#ifndef _WIN32_WCE
void CMainMenuTreeCtrl::Dump(CDumpContext& dc) const
{
	CTreeCtrl::Dump(dc);
}
#endif
#endif //_DEBUG

void CMainMenuTreeCtrl::ClearAll( void )
{
	if ( GetSafeHwnd() ) DeleteAllItems();
}

void CMainMenuTreeCtrl::AddMainGroup( void )
{
	if ( NULL == GetSafeHwnd() ) return;

	TV_ITEM tvitem;
	TV_INSERTSTRUCT tvstruct;

	ZeroMemory( &tvitem, sizeof(tvitem) );
	ZeroMemory( &tvstruct, sizeof(tvstruct) );

	tvitem.mask = TVIF_TEXT|TVIF_PARAM;
	tvstruct.hParent = NULL;

	tvitem.pszText = _T("Main Group");
	tvitem.lParam = (LPARAM)0;
	tvstruct.item = tvitem;
	tvstruct.hInsertAfter = TVI_LAST;
	InsertItem( &tvstruct );
}

void CMainMenuTreeCtrl::DelMainGroup( void )
{
	if ( NULL == GetSafeHwnd() ) return;

	HTREEITEM hItem = GetNextItem( NULL, TVGN_ROOT );

	do
	{
		DWORD_PTR dwVal = GetItemData( hItem );

		if ( dwVal == 0 )
		{
			DeleteItem( hItem );
			return;
		}

		hItem = GetNextItem( NULL, TVGN_NEXT );
	}
	while ( hItem );
}

void CMainMenuTreeCtrl::AddExceptionGroup( NTL_TS_TG_ID tgID )
{
	if ( NULL == GetSafeHwnd() ) return;

	static TCHAR s_tTemp[128];

	TV_ITEM tvitem;
	TV_INSERTSTRUCT tvstruct;

	ZeroMemory( &tvitem, sizeof(tvitem) );
	ZeroMemory( &tvstruct, sizeof(tvstruct) );

	tvitem.mask = TVIF_TEXT|TVIF_PARAM;
	tvstruct.hParent = NULL;

	if ( NTL_TS_EXCEPT_TLIMT_GROUP_ID_BEGIN <= tgID && tgID <= NTL_TS_EXCEPT_TLIMT_GROUP_ID_END )
	{
		sprintf_s( s_tTemp, _T("Except Time limit group ( %d )"), tgID );
	}
	else if ( NTL_TS_EXCEPT_SERVER_GROUP_ID_BEGIN <= tgID && tgID <= NTL_TS_EXCEPT_SERVER_GROUP_ID_END )
	{
		sprintf_s( s_tTemp, _T("Except Server group ( %d )"), tgID );
	}
	else if ( NTL_TS_EXCEPT_CLIENT_GROUP_ID_BEGIN <= tgID && tgID <= NTL_TS_EXCEPT_CLIENT_GROUP_ID_END )
	{
		sprintf_s( s_tTemp, _T("Except Client group ( %d )"), tgID );
	}
	else if ( tgID == NTL_TS_EXCEPT_GIVEUP_ID )
	{
		sprintf_s( s_tTemp, _T("Except Give up group ( %d )"), tgID );
	}
	else
	{
		sprintf_s( s_tTemp, _T("Error ( %d )"), tgID );
		ASSERT( 0 );
	}

	tvitem.pszText = s_tTemp;
	tvitem.lParam = (LPARAM)tgID;
	tvstruct.item = tvitem;
	tvstruct.hInsertAfter = TVI_LAST;
	InsertItem( &tvstruct );
}

void CMainMenuTreeCtrl::DelExceptionGroup( NTL_TS_TG_ID tgID )
{
	if ( NULL == GetSafeHwnd() ) return;

	ASSERT( tgID != 0 );

	HTREEITEM hItem = GetNextItem( NULL, TVGN_ROOT );

	while ( hItem )
	{
		DWORD_PTR dwVal = GetItemData( hItem );

		if ( dwVal == tgID )
		{
			DeleteItem( hItem );
			return;
		}

		hItem = GetNextItem( hItem, TVGN_NEXT );
	}
}

// CMainMenuTreeCtrl 메시지 처리기입니다.

BOOL CMainMenuTreeCtrl::OnEraseBkgnd(CDC* pDC)
{
	// 배경을 다시 그리지 않는다
	return TRUE;
}

void CMainMenuTreeCtrl::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	if ( !CTSDoc::GetInstance()->IsProjectLoaded() )
	{
		SelectItem( 0 );
		*pResult = 0;
		return;
	}

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	if ( 0 == pNMTreeView->itemNew.lParam )
	{
		CTSDoc::GetInstance()->SelMainGroup();
	}
	else
	{
		CTSDoc::GetInstance()->SelExceptGroup( (NTL_TS_TG_ID)pNMTreeView->itemNew.lParam );
	}

	*pResult = 0;
}
