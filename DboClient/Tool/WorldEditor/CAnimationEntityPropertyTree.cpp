#include "StdAfx.h"
#include "Resource.h"

#include "CAnimationEntityProperty.h"
#include "CAnimationEntity.h"

#include "CInstanceTreeCtrl.h"

#include "CAnimationEntityPropertyTree.h"


void CAnimationEntityPropertyTree::Initialize( void )
{
	CEntityPropertyTree::Initialize();

	m_pAnimation				= NULL;

	m_pAnimationSpeed			= NULL;
	m_pAnimationOffset			= NULL;

	return;
}


bool CAnimationEntityPropertyTree::Create( CWnd * pWnd, CEntity * pEntity, bool bUpdate )
{
	if( CEntityPropertyTree::Create( pWnd, pEntity, false ) == false )
	{
		return false;
	}

	// Animation

	m_pAnimation = InsertItem( new CPropTreeItem() );
	m_pAnimation->SetLabelText( _T( "Animation" ) );
	m_pAnimation->SetInfoText( _T( "This is property of animation instance." ) );
	m_pAnimation->Expand();

	// Animation Raster

	m_pAnimationSpeed = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pAnimation );
	m_pAnimationSpeed->SetLabelText( _T( "Speed" ) );
	m_pAnimationSpeed->SetInfoText( _T( "This is speed of animation." ) );
	m_pAnimationSpeed->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

	m_pAnimationOffset = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pAnimation );
	m_pAnimationOffset->SetLabelText( _T( "Offset" ) );
	m_pAnimationOffset->SetInfoText( _T( "This is time offset of animation." ) );
	m_pAnimationOffset->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

	if( bUpdate == true )
	{
		return Update( pEntity );
	}
	else
	{
		return true;
	}
}


bool CAnimationEntityPropertyTree::Update( CEntity * pEntity, bool bUpdate )
{
	CEntityPropertyTree::Update( pEntity, false );

	Assert( pEntity->GetType() == E_ENTITY_TYPE_ANIMATION );

	CAnimationEntity *			pAnimationEntity = reinterpret_cast<CAnimationEntity *>(pEntity);
	CAnimationEntityProperty *	pAnimationEntityProperty = reinterpret_cast<CAnimationEntityProperty *>(pAnimationEntity->GetProperty());
	CFixedString				String;


	// Speed

	RwReal		fSpeed;
	fSpeed = pAnimationEntityProperty->GetAnimationSpeed();

	m_pAnimationSpeed->SetItemValue( (LPARAM)(&fSpeed) );

	// Offset

	RwReal		fOffset;
	fOffset = pAnimationEntityProperty->GetAnimationOffset();

	m_pAnimationOffset->SetItemValue( (LPARAM)(&fOffset) );

	if( bUpdate == true )
	{
		UpdatedItems();
	}

	return true;
}


bool CAnimationEntityPropertyTree::Apply( CPropTreeItem * pProperty, bool bUpdate )
{
	if( CEntityPropertyTree::Apply( pProperty, false ) == true )
	{
		return true;
	}

	bool				bModified = false;
	CAnimationEntity *	pAnimationEntity = reinterpret_cast<CAnimationEntity *>(m_pEntity);


	if( m_pAnimationSpeed != NULL )
	{
		if( pProperty->GetCtrlID() == m_pAnimationSpeed->GetCtrlID() )
		{
			pAnimationEntity->SetAnimationSpeed( *(float*)(m_pAnimationSpeed->GetItemValue()) );

			bModified = true;
		}
	}
	
	if( m_pAnimationOffset != NULL )
	{
		if( pProperty->GetCtrlID() == m_pAnimationOffset->GetCtrlID() )
		{
			pAnimationEntity->SetAnimationOffset( *(float*)(m_pAnimationOffset->GetItemValue()) );

			bModified = true;
		}
	}

	if( bModified == true && bUpdate == true )
	{
		m_pEntity->Invalidate();

		Update( m_pEntity );
	}

    return bModified;
}

