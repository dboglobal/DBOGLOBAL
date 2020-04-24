// AssetObjectTreeCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "AssetObjectTreeCtrl.h"
#include "NtlPLObject.h"
#include "CEditorApplication.h"

#include "UserMsg.h"


// CAssetObjectTreeCtrl

IMPLEMENT_DYNAMIC(CAssetObjectTreeCtrl, CTreeCtrl)
CAssetObjectTreeCtrl::CAssetObjectTreeCtrl()
{
	m_pAseetObjectView = this;
}

CAssetObjectTreeCtrl::~CAssetObjectTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CAssetObjectTreeCtrl, CTreeCtrl)
	ON_WM_SIZE()
	ON_MESSAGE(WM_USER_INIT_CUSTOM_WINDOW, OnAssetData)	
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
END_MESSAGE_MAP()


HTREEITEM CAssetObjectTreeCtrl::SearchPrevItemByID( HTREEITEM hParent, RwUInt32 iID )
{
	HTREEITEM			hChildItem = GetChildItem( hParent );
	HTREEITEM			hFoundItem = TVI_FIRST;
	CNtlPLProperty *	pProperty;

	while( hChildItem != NULL )
	{
		pProperty = (CNtlPLProperty *)GetItemData( hChildItem );

		if( pProperty != NULL && pProperty->GetId() >= iID )
		{
			return hFoundItem;
		}

		hFoundItem = hChildItem;
		hChildItem = GetNextItem( hChildItem, TVGN_NEXT );
	}

	return hFoundItem;
}


HTREEITEM CAssetObjectTreeCtrl::SearchPrevItemByName( HTREEITEM hParent, const RwChar * pName )
{
	HTREEITEM			hChildItem = GetChildItem( hParent );
	HTREEITEM			hFoundItem = TVI_FIRST;
	CNtlPLProperty *	pProperty;

	while( hChildItem != NULL )
	{
		pProperty = (CNtlPLProperty *)GetItemData( hChildItem );

		if( pProperty != NULL && strcmp( pProperty->GetName(), pName ) >= 0 )
		{
			return hFoundItem;
		}

		hFoundItem = hChildItem;
		hChildItem = GetNextItem( hChildItem, TVGN_NEXT );
	}

	return hFoundItem;
}


HTREEITEM CAssetObjectTreeCtrl::SearchItemByName( HTREEITEM hParent, const RwChar * pName )
{
	HTREEITEM			hChildItem = GetChildItem( hParent );
	CString				String;

	while( hChildItem != NULL )
	{
		String = GetItemText( hChildItem );

		if( strcmp( String.GetBuffer(), pName ) == 0 )
		{
			return hChildItem;
		}

		hChildItem = GetNextItem( hChildItem, TVGN_NEXT );
	}

	return NULL;
}


HTREEITEM CAssetObjectTreeCtrl::AddTreeItem( HTREEITEM hParent, CString strName, void * pData )
{
	TV_INSERTSTRUCT			tv;
	memset( &tv, 0, sizeof( TV_INSERTSTRUCT ) );

	tv.hParent				= hParent;
	tv.hInsertAfter			= NULL;
	tv.item.mask			= TVIF_STATE | TVIF_TEXT;
	tv.item.pszText			= strName.GetBuffer( strName.GetLength() );
	tv.item.cchTextMax		= 255;

	if( pData != NULL )
	{
		CNtlPLProperty *	pProperty = (CNtlPLProperty *)pData;
		tv.hInsertAfter		= SearchPrevItemByID( hParent, pProperty->GetId() );
		//tv.hInsertAfter = SearchPrevItemByName( hParent, pProperty->GetName() );
	}
	else
	{
		tv.hInsertAfter = TVI_ROOT;
	}

	HTREEITEM hItem = InsertItem( tv.item.pszText, hParent, tv.hInsertAfter );

	SetItemData( hItem, (DWORD_PTR)pData );


	return hItem;
}

LRESULT CAssetObjectTreeCtrl::OnAssetData(WPARAM wParam, LPARAM lParam)
{
	// Object

	HTREEITEM		hObjectGroup = AddTreeItem( NULL, "[Object]", NULL );

	HTREEITEM		hConstructionShop		= AddTreeItem( hObjectGroup, "[Construction Shop]",		NULL );
	HTREEITEM		hConstructionBuilding	= AddTreeItem( hObjectGroup, "[Construction Building]",	NULL );
	HTREEITEM		hConstructionRoad		= AddTreeItem( hObjectGroup, "[Construction Road]",		NULL );
	HTREEITEM		hConstructionWall		= AddTreeItem( hObjectGroup, "[Construction Wall]",		NULL );

	HTREEITEM		hLandscapeGame			= AddTreeItem( hObjectGroup, "[LandScape Game]",		NULL );
	HTREEITEM		hLandscapeLife			= AddTreeItem( hObjectGroup, "[LandScape Life]",		NULL );
	HTREEITEM		hLandscapeSight			= AddTreeItem( hObjectGroup, "[LandScape Sight]",		NULL );

	HTREEITEM		hMatterTool				= AddTreeItem( hObjectGroup, "[Matter Tool]",			NULL );
	HTREEITEM		hMatterCarry			= AddTreeItem( hObjectGroup, "[Matter Carry]",			NULL );

	HTREEITEM		hNatureTree				= AddTreeItem( hObjectGroup, "[Nature Tree]",			NULL );
	HTREEITEM		hNatureGrass			= AddTreeItem( hObjectGroup, "[Nature Grass]",			NULL );
	HTREEITEM		hNatureRock				= AddTreeItem( hObjectGroup, "[Nature Rock]",			NULL );
	HTREEITEM		hNatureAnimal			= AddTreeItem( hObjectGroup, "[Nature Animal]",			NULL );

	HTREEITEM		hEtcs					= AddTreeItem( hObjectGroup, "[Etcs]",					NULL );

	CNtlPLPropertyContainer::MapProp::iterator		it;
	CNtlPLPropertyContainer::MapProp * pMapObjectProp = &(CNtlPLPropertyContainer::GetInstance()->Gets( PLENTITY_OBJECT ));

	for( it = pMapObjectProp->begin(); it != pMapObjectProp->end(); ++it )
	{
		CNtlPLObjectProperty *	pObjectProperty = reinterpret_cast<CNtlPLObjectProperty *>((*it).second);

		switch( pObjectProperty->GetCategory() )
		{
			case E_OBJECT_CATEGORY_CONSTRUCTION_SHOP:
			{
				AddTreeItem( hConstructionShop, (*it).second->GetName(), (void*)(*it).second );

				break;
			}

			case E_OBJECT_CATEGORY_CONSTRUCTION_BUILDING:
			{
				AddTreeItem( hConstructionBuilding, (*it).second->GetName(), (void*)(*it).second );

				break;
			}

			case E_OBJECT_CATEGORY_CONSTRUCTION_WALL:
			{
				AddTreeItem( hConstructionWall, (*it).second->GetName(), (void*)(*it).second );

				break;
			}

			case E_OBJECT_CATEGORY_CONSTRUCTION_ROAD:
			{
				AddTreeItem( hConstructionRoad, (*it).second->GetName(), (void*)(*it).second );

				break;
			}

			case E_OBJECT_CATEGORY_LANDSCAPE_GAME:
			{
				AddTreeItem( hLandscapeGame, (*it).second->GetName(), (void*)(*it).second );

				break;
			}

			case E_OBJECT_CATEGORY_LANDSCAPE_LIFE:
			{
				AddTreeItem( hLandscapeLife, (*it).second->GetName(), (void*)(*it).second );

				break;
			}

			case E_OBJECT_CATEGORY_LANDSCAPE_SIGHT:
			{
				AddTreeItem( hLandscapeSight, (*it).second->GetName(), (void*)(*it).second );

				break;
			}

			case E_OBJECT_CATEGORY_MATTER_TOOL:
			{
				AddTreeItem( hMatterTool, (*it).second->GetName(), (void*)(*it).second );

				break;
			}

			case E_OBJECT_CATEGORY_MATTER_CARRY:
			{
				AddTreeItem( hMatterCarry, (*it).second->GetName(), (void*)(*it).second );

				break;
			}

			case E_OBJECT_CATEGORY_NATURE_TREE:
			{
				AddTreeItem( hNatureTree, (*it).second->GetName(), (void*)(*it).second );

				break;
			}

			case E_OBJECT_CATEGORY_NATURE_GRASS:
			{
				AddTreeItem( hNatureGrass, (*it).second->GetName(), (void*)(*it).second );

				break;
			}

			case E_OBJECT_CATEGORY_NATURE_ROCK:
			{
				AddTreeItem( hNatureRock, (*it).second->GetName(), (void*)(*it).second );

				break;
			}

			case E_OBJECT_CATEGORY_NATURE_ANIMAL:
			{
				AddTreeItem( hNatureAnimal, (*it).second->GetName(), (void*)(*it).second );

				break;
			}

			default:
			{
				AddTreeItem( hEtcs, (*it).second->GetName(), (void*)(*it).second );
			}
		}
	}

	// Light

	HTREEITEM		hLightGroup = AddTreeItem( NULL, "[Light]", NULL );

	// Effect

	HTREEITEM		hEffectGroup = AddTreeItem( NULL, "[Effect]", NULL );

	CNtlPLPropertyContainer::MapProp * pMapEffectProp = &(CNtlPLPropertyContainer::GetInstance()->Gets( PLENTITY_EFFECT ));

	for( it = pMapEffectProp->begin(); it != pMapEffectProp->end(); ++it )
	{
		AddTreeItem( hEffectGroup, (*it).second->GetName(), (void*)(*it).second); 
	}

	// Character

	HTREEITEM		hCharacterGroup = AddTreeItem( NULL, "[Character]", NULL );

	CNtlPLPropertyContainer::MapProp * pMapCharacterProp = &(CNtlPLPropertyContainer::GetInstance()->Gets( PLENTITY_CHARACTER ));

	for(it = pMapCharacterProp->begin(); it != pMapCharacterProp->end(); ++it )
	{
		AddTreeItem( hCharacterGroup, (*it).second->GetName(), (void*)(*it).second); 
	}

	return TRUE;
}

void CAssetObjectTreeCtrl::OnSize(UINT nType, int cx, int cy)
{
	CTreeCtrl::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CAssetObjectTreeCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DWORD dwMousePos = GetMessagePos();                   // retrieve mouse cursor position when msg was sent
	CPoint MousePos(GET_X_LPARAM(dwMousePos), GET_Y_LPARAM(dwMousePos)); // ..and put into point structure
	ScreenToClient(&MousePos);                    // make coords local to tree client area

	UINT htFlags = 0;
	HTREEITEM hItem = HitTest(MousePos, &htFlags);   // See where the click was on

	if( hItem != 0 )
	{
		CNtlPLProperty * pProp = (CNtlPLProperty *)GetItemData( hItem );

		if( pProp != NULL )
		{
			g_pEngineApp->SelectItem( E_ITEM_TYPE_PROPERTY, pProp );
		}
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
