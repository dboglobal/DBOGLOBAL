#include "StdAfx.h"
#include "Resource.h"

#include "CAtomicEntityProperty.h"
#include "CAtomicEntity.h"

#include "CInstanceTreeCtrl.h"

#include "CAtomicEntityPropertyTree.h"


void CAtomicEntityPropertyTree::Initialize( void )
{
	CEntityPropertyTree::Initialize();

	m_pAtomic			= NULL;

	return;
}


bool CAtomicEntityPropertyTree::Create( CWnd * pWnd, CEntity * pEntity, bool bUpdate )
{
	if( CEntityPropertyTree::Create( pWnd, pEntity, false ) == false )
	{
		return false;
	}

	// Atomic

	m_pAtomic = InsertItem( new CPropTreeItem() );
	m_pAtomic->SetLabelText( _T( "Atomic" ) );
	m_pAtomic->SetInfoText( _T( "This is property of atomic instance." ) );
	m_pAtomic->Expand();

	// Atomic Type

	m_pAtomicType = InsertItem( new CPropTreeItemStatic(), m_pAtomic );
	m_pAtomicType->SetLabelText( _T( "Type" ) );
	m_pAtomicType->SetInfoText( _T( "This is type name of atomic." ) );


	if( bUpdate == true )
	{
		return Update( pEntity );
	}
	else
	{
		return true;
	}
}


bool CAtomicEntityPropertyTree::Update( CEntity * pEntity, bool bUpdate )
{
	CEntityPropertyTree::Update( pEntity, false );

	Assert( pEntity->GetType() == E_ENTITY_TYPE_ATOMIC );

	CAtomicEntity *			pAtomicEntity = reinterpret_cast<CAtomicEntity *>(pEntity);
	CFixedString			String;

	if( pAtomicEntity->IsTerrain() != FALSE )
	{
		String.Assign( "terrain" );
	}
	else if( pAtomicEntity->IsSky() != FALSE )
	{
		String.Assign( "sky" );
	}
	else
	{
		String.Assign( "object" );
	}

	m_pAtomicType->SetItemValue( (LPARAM)String.GetBuffer() );

	if( bUpdate == true )
	{
		UpdatedItems();
	}

	return true;
}


bool CAtomicEntityPropertyTree::Apply( CPropTreeItem * pProperty, bool bUpdate )
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

