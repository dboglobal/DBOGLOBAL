// AssetObjectTreeCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WorldEditor.h"

#include "CEditorApplication.h"

#include "CEntity.h"
#include "UserMsg.h"
#include "CInstanceTreeCtrl.h"

// CAssetObjectTreeCtrl

CInstanceTreeCtrl * CInstanceTreeCtrl::m_pInstance = NULL;

IMPLEMENT_DYNAMIC(CInstanceTreeCtrl, CTreeCtrl)
CInstanceTreeCtrl::CInstanceTreeCtrl()
{
	m_pInstance = this;
}

CInstanceTreeCtrl::~CInstanceTreeCtrl()
{
	m_pInstance = NULL;
}


BEGIN_MESSAGE_MAP(CInstanceTreeCtrl, CTreeCtrl)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT( NM_CLICK, OnNMClick )
END_MESSAGE_MAP()


HTREEITEM CInstanceTreeCtrl::AddTreeItem( HTREEITEM hParent, CString strName, void * pData )
{
	TV_INSERTSTRUCT	tv;
	memset( &tv, 0, sizeof( tv ) );

	tv.hParent = hParent;
	tv.hInsertAfter = NULL;
	tv.item.mask = TVIF_STATE | TVIF_TEXT;
	tv.item.pszText = strName.GetBuffer( strName.GetLength() );
	tv.item.cchTextMax = 255;

	HTREEITEM hItem = InsertItem( &tv );

	SetItemData( hItem, (DWORD_PTR)pData );

	return hItem;
}


int CALLBACK CInstanceTreeCtrl::SortByNameCB( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	CInstanceTreeCtrl *		pTreeCtrl = (CInstanceTreeCtrl *)lParamSort;
	CString    strItem1 = pTreeCtrl->GetItemText( (HTREEITEM)lParam1 );
	CString    strItem2 = pTreeCtrl->GetItemText( (HTREEITEM)lParam2 );

   return strcmp(strItem2, strItem1);
}


LRESULT CInstanceTreeCtrl::InitializeData( void )
{
	// Object

	return TRUE;
}


LRESULT CInstanceTreeCtrl::RegisterEntity( CEntity * pEntity )
{
	HTREEITEM		hParentItem;

	if( pEntity->GetParent() == NULL )
	{
		hParentItem = NULL;
	}
	else
	{
		hParentItem = GetItem( pEntity->GetParent() );
	}

	HTREEITEM		hAddedItem = AddTreeItem( hParentItem, pEntity->GetNameBuffer(), pEntity );

	m_Hash.insert( TTreeItemHashHashPair( pEntity->GetUniqueID(), hAddedItem ) );

	return TRUE;
}


LRESULT CInstanceTreeCtrl::UnregisterEntity( CEntity * pEntity )
{
	HTREEITEM		hItem;

	hItem = GetItem( pEntity );

	if( hItem != NULL )
	{
		m_Hash.erase( pEntity->GetUniqueID() );

		DeleteItem( hItem );

		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}


HTREEITEM CInstanceTreeCtrl::GetItem( CEntity * pEntity )
{
	if( GetCount() == 0 )
	{
		return NULL;
	}

	TTreeItemHashIterator		tIterator = m_Hash.find( pEntity->GetUniqueID() );

	if( tIterator == m_Hash.end() )
	{
		return NULL;
	}

	return (*tIterator).second;
/*

	for( HTREEITEM hChildItem = GetRootItem(); hChildItem != NULL; hChildItem = GetNextItem( hChildItem, TVGN_NEXT ) )
	{
		HTREEITEM hMatchItem = GetItemFB( hChildItem, pEntity );

		if( hMatchItem != NULL )
		{
			return hMatchItem;
		}
	}

	return NULL;*/
}

RwBool CInstanceTreeCtrl::SelectEntityItem( CEntity * pEntity )
{
	HTREEITEM		hItemSelected = GetItem( pEntity );

	if( hItemSelected == NULL )
	{
		return FALSE;
	}

	HTREEITEM		hParent = GetParentItem(hItemSelected);
	if( hParent != NULL )
	{
		Expand( hParent, TVE_EXPAND );
	}

	EnsureVisible( hItemSelected );
	SelectItem( hItemSelected );

	return TRUE;
}

RwBool CInstanceTreeCtrl::UnselectEntityItem( CEntity * pEntity )
{
	HTREEITEM		hItemSelected = GetItem( pEntity );

	if( hItemSelected == NULL )
	{
		return FALSE;
	}

	return Expand( hItemSelected, TVE_COLLAPSE );
}


HTREEITEM CInstanceTreeCtrl::GetItemFB( HTREEITEM hItem, CEntity * pEntity )
{
	if( hItem == NULL )
	{
		return NULL;
	}

	CEntity * pCurrentEntity = (CEntity *)GetItemData( hItem );

	if( pCurrentEntity != NULL )
	{
		if( pCurrentEntity == pEntity )
		{
			return hItem;
		}
	}

	for( HTREEITEM hChildItem = GetChildItem( hItem ); hChildItem != NULL; hChildItem = GetNextItem( hChildItem, TVGN_NEXT ) )
	{
		HTREEITEM hMatchItem = GetItemFB( hChildItem, pEntity );

		if( hMatchItem != NULL )
		{
			return hMatchItem;
		}
	}

	return NULL; 
}

void CInstanceTreeCtrl::OnSize(UINT nType, int cx, int cy)
{
	CTreeCtrl::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CInstanceTreeCtrl::OnNMClick( NMHDR *pNMHDR, LRESULT *pResult )
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DWORD dwMousePos = GetMessagePos();                   // retrieve mouse cursor position when msg was sent
	CPoint MousePos(GET_X_LPARAM(dwMousePos), GET_Y_LPARAM(dwMousePos)); // ..and put into point structure
	ScreenToClient(&MousePos);                    // make coords local to tree client area

	UINT htFlags = 0;
	HTREEITEM hItem = HitTest(MousePos, &htFlags);   // See where the click was on

	if( hItem != 0 )
	{
		CEntity * pEntity = (CEntity *)GetItemData( hItem );

		if( pEntity != NULL )
		{
			g_pEngineApp->SelectItem( E_ITEM_TYPE_ENTITY, pEntity );
		}
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
