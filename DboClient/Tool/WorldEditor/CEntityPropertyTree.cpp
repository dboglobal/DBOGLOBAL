#include "StdAfx.h"
#include "Resource.h"

#include "CEntityProperty.h"
#include "CEntity.h"

#include "CInstanceTreeCtrl.h"

#include "CEntityPropertyTree.h"


void CEntityPropertyTree::Initialize( void )
{
	m_pEntity		= NULL;

	m_pBase			= NULL;
	m_pSerialID		= NULL;
	m_pParentID		= NULL;

	m_pCategory		= NULL;
	m_pName			= NULL;
	m_pLocality		= NULL;

	m_pLocation		= NULL;
	m_pLocationX	= NULL;
	m_pLocationY	= NULL;
	m_pLocationZ	= NULL;

	m_pDirection	= NULL;
	m_pDirectionX	= NULL;
	m_pDirectionY	= NULL;
	m_pDirectionZ	= NULL;

	m_pScale		= NULL;
	m_pScaleX		= NULL;
	m_pScaleY		= NULL;
	m_pScaleZ		= NULL;

	return;
}


void CEntityPropertyTree::SetEntity( CEntity * pEntity )
{
	m_pEntity		= pEntity;

	return;
}


bool CEntityPropertyTree::Create( CWnd * pWnd, CEntity * pEntity, bool bUpdate )
{
	DWORD		dwWindowStyle = WS_CHILD | WS_VISIBLE | PTS_NOTIFY;
	CRect		RectClient;

	pWnd->GetClientRect( RectClient );

	if( CPropTree::Create( dwWindowStyle, RectClient, pWnd, IDC_PROPERTYTREE ) == FALSE )
	{
		return false;
	}

	if( pEntity == NULL )
	{
		return true;
	}

	// Base

	m_pBase = InsertItem( new CPropTreeItem() );
	m_pBase->SetLabelText( _T( "Base" ) );
	m_pBase->SetInfoText( _T( "This is property of base instance." ) );
	m_pBase->Expand();

	// Serial ID

	m_pSerialID = InsertItem( new CPropTreeItemStatic(), m_pBase );
	m_pSerialID->SetLabelText( _T( "Serial ID" ) );
	m_pSerialID->SetInfoText( _T( "This is serial ID of instance." ) );

	// Parent ID

	m_pParentID = InsertItem( new CPropTreeItemStatic(), m_pBase );
	m_pParentID->SetLabelText( _T( "Parent ID" ) );
	m_pParentID->SetInfoText( _T( "This is serial ID of parent instance." ) );

	// Category

	m_pCategory = InsertItem( new CPropTreeItemStatic(), m_pBase );
	m_pCategory->SetLabelText( _T( "Category" ) );
	m_pCategory->SetInfoText( _T( "This is category of instance.") );

	// Name

	m_pName = (CPropTreeItemEdit *)InsertItem( new CPropTreeItemEdit(), m_pBase );
	m_pName->SetLabelText( _T("Name") );
	m_pName->SetInfoText( _T( "This is name of instance.") );

	// Locality

	m_pLocality = InsertItem( new CPropTreeItemStatic(), m_pBase );
	m_pLocality->SetLabelText( _T( "Locality"));
	m_pLocality->SetInfoText( _T( "This is whether instance is global or local.") );

	// Has Location

	m_pLocation = InsertItem( new CPropTreeItemStatic(), m_pBase );
	m_pLocation->SetLabelText( _T( "Location"));
	m_pLocation->SetInfoText( _T( "This is location of instance.") );

	if( pEntity->HasLocation() != FALSE )
	{
		m_pLocationX = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pLocation );
		m_pLocationX->SetLabelText( _T( "X" ) );
		m_pLocationX->SetInfoText( _T( "This is X location of instance." ) );
		m_pLocationX->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

		m_pLocationY = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pLocation );
		m_pLocationY->SetLabelText( _T( "Y" ) );
		m_pLocationY->SetInfoText( _T( "This is Y location of instance" ) );
		m_pLocationY->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

		m_pLocationZ = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pLocation );
		m_pLocationZ->SetLabelText( _T( "Z" ) );
		m_pLocationZ->SetInfoText( _T( "This is Z location of instance" ) );
		m_pLocationZ->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );
	}

	// Has Direction

	m_pDirection = InsertItem( new CPropTreeItemStatic(), m_pBase );
	m_pDirection->SetLabelText( _T( "Direction"));
	m_pDirection->SetInfoText( _T( "This is direction of instance.") );

	if( pEntity->HasRotation() != FALSE )
	{
		m_pDirectionX = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pDirection );
		m_pDirectionX->SetLabelText( _T( "Yaw" ) );
		m_pDirectionX->SetInfoText( _T( "This is Yaw rotation of instance" ) );
		m_pDirectionX->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

		m_pDirectionY = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pDirection );
		m_pDirectionY->SetLabelText( _T( "Pitch" ) );
		m_pDirectionY->SetInfoText( _T( "This is Pitch rotation of instance" ) );
		m_pDirectionY->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

		m_pDirectionZ = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pDirection );
		m_pDirectionZ->SetLabelText( _T( "Roll" ) );
		m_pDirectionZ->SetInfoText( _T( "This is Roll rotation of instance" ) );
		m_pDirectionZ->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );
	}

	// Has Scale

	m_pScale = InsertItem( new CPropTreeItemStatic(), m_pBase );
	m_pScale->SetLabelText( _T( "Scale"));
	m_pScale->SetInfoText( _T( "This is scale of instance.") );

	if( pEntity->HasScale() != FALSE )
	{
		m_pScaleX = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pScale );
		m_pScaleX->SetLabelText( _T( "X" ) );
		m_pScaleX->SetInfoText( _T( "This is X scale of Instance" ) );
		m_pScaleX->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

		m_pScaleY = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pScale );
		m_pScaleY->SetLabelText( _T( "Y" ) );
		m_pScaleY->SetInfoText( _T( "This is Y scale of Instance" ) );
		m_pScaleY->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

		m_pScaleZ = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pScale );
		m_pScaleZ->SetLabelText( _T( "Z" ) );
		m_pScaleZ->SetInfoText( _T( "This is Z scale of Instance" ) );
		m_pScaleZ->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );
	}

	if( bUpdate == true )
	{
		return CEntityPropertyTree::Update( pEntity );
	}
	else
	{
		return true;
	}
}


void CEntityPropertyTree::Destroy( void )
{
	DeleteAllItems();

	Initialize();

	return;
}


bool CEntityPropertyTree::Update( CEntity * pEntity, bool bUpdate )
{
	SetEntity( pEntity );

	CFixedString			String;

	// Serial ID

	String.AssignInteger( pEntity->GetUniqueID(), true );
	m_pSerialID->SetItemValue( (LPARAM)String.GetBuffer() );

	// Parent ID

	String.AssignInteger( pEntity->GetParentID(), true );
	m_pParentID->SetItemValue( (LPARAM)String.GetBuffer() );

	// Category

	String.Assign( *(pEntity->GetCategoryName()) );
	m_pCategory->SetItemValue( (LPARAM)String.GetBuffer() );

	// Name

	String.Assign( *(pEntity->GetName()) );
	m_pName->SetItemValue( (LPARAM)String.GetBuffer() );

	// Locality

	String.Assign( ( pEntity->IsGlobal() != FALSE ) ? _T( "global" ) : _T( "local" )  );
	m_pLocality->SetItemValue( (LPARAM)String.GetBuffer() );

	// Location

	if( pEntity->HasLocation() != FALSE )
	{
		RwV3d			Location;

		Location.x = pEntity->GetPosition()->GetX();
		Location.y = pEntity->GetPosition()->GetY();
		Location.z = pEntity->GetPosition()->GetZ();

		m_pLocationX->SetItemValue( (LPARAM)(&Location.x) );
		m_pLocationY->SetItemValue( (LPARAM)(&Location.y) );
		m_pLocationZ->SetItemValue( (LPARAM)(&Location.z) );

		TCHAR			szBuffer[256];
		_stprintf( szBuffer, _T( "(%.1f, %.1f, %.1f)" ), Location.x, Location.y, Location.z );
		String.Assign( szBuffer );
	}
	else
	{
		String.Assign( _T( "(n/a)" ) );
	}

	m_pLocation->SetItemValue( (LPARAM)String.GetBuffer() );

	// Direction

	if( pEntity->HasRotation() != FALSE )
	{
		RwV3d		Direction;
		Direction.x = pEntity->GetPosition()->GetYaw();
		Direction.y = pEntity->GetPosition()->GetPitch();
		Direction.z = pEntity->GetPosition()->GetRoll();

		m_pDirectionX->SetItemValue( (LPARAM)(&Direction.x) );
		m_pDirectionY->SetItemValue( (LPARAM)(&Direction.y) );
		m_pDirectionZ->SetItemValue( (LPARAM)(&Direction.z) );

		TCHAR			szBuffer[256];
		_stprintf( szBuffer, _T( "(%.1f, %.1f, %.1f)" ), Direction.x, Direction.y, Direction.z );
		String.Assign( szBuffer );
	}
	else
	{
		String.Assign( _T( "(n/a)" ) );
	}

	m_pDirection->SetItemValue( (LPARAM)String.GetBuffer() );

	// Scale

	if( pEntity->HasScale() != FALSE )
	{
		RwV3d		Scale;
		Scale.x = pEntity->GetScale()->GetX();
		Scale.y = pEntity->GetScale()->GetY();
		Scale.z = pEntity->GetScale()->GetZ();

		m_pScaleX->SetItemValue( (LPARAM)(&Scale.x) );
		m_pScaleY->SetItemValue( (LPARAM)(&Scale.y) );
		m_pScaleZ->SetItemValue( (LPARAM)(&Scale.z) );

		TCHAR			szBuffer[256];
		_stprintf( szBuffer, _T( "(%.1f, %.1f, %.1f)" ), Scale.x, Scale.y, Scale.z );
		String.Assign( szBuffer );
	}
	else
	{
		String.Assign( _T( "(n/a)" ) );
	}

	m_pScale->SetItemValue( (LPARAM)String.GetBuffer() );

	if( bUpdate == true )
	{
		UpdatedItems();
	}

	return true;
}


bool CEntityPropertyTree::Apply( CPropTreeItem * pProperty, bool bUpdate )
{
	if( pProperty == NULL )
	{
		return true;
	}

	if( m_pEntity == NULL || m_pEntity->GetCategory() == E_ENTITY_CATEGORY_INTERNAL )
	{
		return true;
	}

	bool			bModified = false;

	if( pProperty->GetCtrlID() == m_pName->GetCtrlID() )
	{
		m_pEntity->SetName( (TCHAR *)pProperty->GetItemValue() );
		m_pEntity->Invalidate();

		HTREEITEM		hTreeItem = CInstanceTreeCtrl::GetValidInstance()->GetItem( m_pEntity );

		CInstanceTreeCtrl::GetValidInstance()->SetItemText( hTreeItem, (TCHAR *)pProperty->GetItemValue() );

		bModified = true;
	}

	if( m_pLocationX != NULL )
	{
		if(	( pProperty->GetCtrlID() == m_pLocationX->GetCtrlID() ) ||
			( pProperty->GetCtrlID() == m_pLocationY->GetCtrlID() ) ||
			( pProperty->GetCtrlID() == m_pLocationZ->GetCtrlID() ) )
		{
			// Location

			bModified = true;
		}
	}

	if( m_pDirectionX != NULL )
	{
		if(	( pProperty->GetCtrlID() == m_pDirectionX->GetCtrlID() ) ||
			( pProperty->GetCtrlID() == m_pDirectionY->GetCtrlID() ) ||
			( pProperty->GetCtrlID() == m_pDirectionZ->GetCtrlID() ) )
		{
			// Rotate

			if( m_pEntity->HasRotation() != FALSE )
			{
				SPosition3D				sPosition;

				sPosition.Set( *(m_pEntity->GetPosition()) );
				sPosition.SetYaw( *(float*)(m_pDirectionX->GetItemValue()) );
				sPosition.SetPitch( *(float*)(m_pDirectionY->GetItemValue()) );
				sPosition.SetRoll( *(float*)(m_pDirectionZ->GetItemValue()) );
				
				m_pEntity->SetPosition( &sPosition, FALSE, TRUE );

				m_pEntity->Invalidate();

				bModified = true;
			}
		}
	}

	if( m_pScaleX != NULL )
	{
		if( ( pProperty->GetCtrlID() == m_pScaleX->GetCtrlID() ) ||
			( pProperty->GetCtrlID() == m_pScaleY->GetCtrlID() ) ||
			( pProperty->GetCtrlID() == m_pScaleZ->GetCtrlID() ) )
		{
			// Scale

			if( m_pEntity->GetScale() != FALSE )
			{
				SLocation3D				sScale;

				sScale.SetX( *(float*)(m_pScaleX->GetItemValue()) );
				sScale.SetY( *(float*)(m_pScaleY->GetItemValue()) );
				sScale.SetZ( *(float*)(m_pScaleZ->GetItemValue()) );

				m_pEntity->SetScale( &sScale, FALSE, TRUE );

				m_pEntity->Invalidate();

				bModified = true;
			}
		}
	}

	if( bModified == true && bUpdate == true )
	{
		CEntityPropertyTree::Update( m_pEntity );
	}

    return bModified;
}

