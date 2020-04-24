#include "StdAfx.h"
#include "Resource.h"

#include "CClumpEntityProperty.h"
#include "CClumpEntity.h"

#include "CInstanceTreeCtrl.h"

#include "CClumpEntityPropertyTree.h"


void CClumpEntityPropertyTree::Initialize( void )
{
	CEntityPropertyTree::Initialize();

	m_pClump			= NULL;

	m_pPropertyName		= NULL;

	return;
}


bool CClumpEntityPropertyTree::Create( CWnd * pWnd, CEntity * pEntity, bool bUpdate )
{
	if( CEntityPropertyTree::Create( pWnd, pEntity, false ) == false )
	{
		return false;
	}

	// Clump

	m_pClump = InsertItem( new CPropTreeItem() );
	m_pClump->SetLabelText( _T( "Clump" ) );
	m_pClump->SetInfoText( _T( "This is property of clump instance." ) );
	m_pClump->Expand();

	// Clump Resource Name

	m_pPropertyName = InsertItem( new CPropTreeItemStatic(), m_pClump );
	m_pPropertyName->SetLabelText( _T( "Property Name" ) );
	m_pPropertyName->SetInfoText( _T( "This is property name of clump." ) );

	if( bUpdate == true )
	{
		return Update( pEntity );
	}
	else
	{
		return true;
	}
}


bool CClumpEntityPropertyTree::Update( CEntity * pEntity, bool bUpdate )
{
	CEntityPropertyTree::Update( pEntity, false );

	Assert( pEntity->GetType() == E_ENTITY_TYPE_CLUMP );

	CClumpEntity *			pClumpEntity = reinterpret_cast<CClumpEntity *>(pEntity);
	CFixedString			String;

	// Clump Resource Name

	String.Assign( *(pClumpEntity->GetPropertyName()) );
	m_pPropertyName->SetItemValue( (LPARAM)String.GetBuffer() );

	if( bUpdate == true )
	{
		UpdatedItems();
	}

	return true;
}


bool CClumpEntityPropertyTree::Apply( CPropTreeItem * pProperty, bool bUpdate )
{
	if( CEntityPropertyTree::Apply( pProperty, false ) == true )
	{
		return true;
	}

	bool	bModified = false;

	if( bModified == true && bUpdate == true )
	{
		Update( m_pEntity );
	}

    return bModified;
}

