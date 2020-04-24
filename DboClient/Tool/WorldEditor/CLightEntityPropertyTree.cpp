#include "StdAfx.h"
#include "Resource.h"

#include "CLightEntityProperty.h"
#include "CLightEntity.h"

#include "CInstanceTreeCtrl.h"

#include "CLightEntityPropertyTree.h"


void CLightEntityPropertyTree::Initialize( void )
{
	CEntityPropertyTree::Initialize();

	m_pLight					= NULL;
	m_pLightType				= NULL;
	m_pAffectType				= NULL;

	m_pLightColor				= NULL;
	m_pLightColorR				= NULL;
	m_pLightColorG				= NULL;
	m_pLightColorB				= NULL;
	m_pLightColorA				= NULL;

	m_pShadow					= NULL;
	m_pShadowOffset				= NULL;
	m_pShadowMultipleFactor		= NULL;
	m_pShadowSubShadowFactor	= NULL;

	return;
}


bool CLightEntityPropertyTree::Create( CWnd * pWnd, CEntity * pEntity, bool bUpdate )
{
	if( CEntityPropertyTree::Create( pWnd, pEntity, false ) == false )
	{
		return false;
	}

	// Light

	m_pLight = InsertItem( new CPropTreeItem() );
	m_pLight->SetLabelText( _T( "Light" ) );
	m_pLight->SetInfoText( _T( "This is property of light instance." ) );
	m_pLight->Expand();

	// Light Type

	m_pLightType = InsertItem( new CPropTreeItemStatic(), m_pLight );
	m_pLightType->SetLabelText( _T( "Type" ) );
	m_pLightType->SetInfoText( _T( "This is type name of light." ) );

	// Affect

	m_pAffectType = InsertItem( new CPropTreeItemStatic(), m_pLight );
	m_pAffectType->SetLabelText( _T( "Affect" ) );
	m_pAffectType->SetInfoText( _T( "This is affect type of light." ) );

	// Light Color

	m_pLightColor = InsertItem( new CPropTreeItemStatic(), m_pLight );
	m_pLightColor->SetLabelText( _T( "Color" ) );
	m_pLightColor->SetInfoText( _T( "This is color of light." ) );

	{
		m_pLightColorR = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pLightColor );
		m_pLightColorR->SetLabelText( _T( "Red" ) );
		m_pLightColorR->SetInfoText( _T( "This is red intensity of light." ) );
		m_pLightColorR->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

		m_pLightColorG = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pLightColor );
		m_pLightColorG->SetLabelText( _T( "Green" ) );
		m_pLightColorG->SetInfoText( _T( "This is green intensity of light" ) );
		m_pLightColorG->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

		m_pLightColorB = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pLightColor );
		m_pLightColorB->SetLabelText( _T( "Blue" ) );
		m_pLightColorB->SetInfoText( _T( "This is blue intensity of light" ) );
		m_pLightColorB->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

		m_pLightColorA = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pLightColor );
		m_pLightColorA->SetLabelText( _T( "Alpha" ) );
		m_pLightColorA->SetInfoText( _T( "This is transparency of light" ) );
		m_pLightColorA->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );
	}

	// Shadow

	if( reinterpret_cast<CLightEntity *>(pEntity)->IsShadowLight() != FALSE )
	{
		m_pShadow = InsertItem( new CPropTreeItem() );
		m_pShadow->SetLabelText( _T( "Shadow" ) );
		m_pShadow->SetInfoText( _T( "This is property of shadow." ) );
		m_pShadow->Expand();

		m_pShadowMultipleFactor = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pShadow );
		m_pShadowMultipleFactor->SetLabelText( _T( "Multiple Factor" ) );
		m_pShadowMultipleFactor->SetInfoText( _T( "This is multiple factor of shadow. (0.0 ~ 1.0)" ) );
		m_pShadowMultipleFactor->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

		m_pShadowOffset = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pShadow );
		m_pShadowOffset->SetLabelText( _T( "Offset" ) );
		m_pShadowOffset->SetInfoText( _T( "This is offset of shadow." ) );
		m_pShadowOffset->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

		m_pShadowSubShadowFactor = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pShadow );
		m_pShadowSubShadowFactor->SetLabelText( _T( "Sub-Shadow Factor" ) );
		m_pShadowSubShadowFactor->SetInfoText( _T( "This is sub shadow factor. (0.0 ~ 1.0)" ) );
		m_pShadowSubShadowFactor->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );
	}

	if( bUpdate == true )
	{
		return Update( pEntity );
	}
	else
	{
		return true;
	}
}


bool CLightEntityPropertyTree::Update( CEntity * pEntity, bool bUpdate )
{
	CEntityPropertyTree::Update( pEntity, false );

	Assert( pEntity->GetType() == E_ENTITY_TYPE_LIGHT );

	CLightEntity *			pLightEntity = reinterpret_cast<CLightEntity *>(pEntity);
	CFixedString			String;

	// Type

	String.Assign( *(pLightEntity->GetLightTypeName()) );
	m_pLightType->SetItemValue( (LPARAM)String.GetBuffer() );

	// Affect

	if( pLightEntity->IsTerrainLight() != FALSE )
	{
		String.Assign( RWSTRING( "terrain" ) );
	}
	else if( pLightEntity->IsShadowLight() != FALSE )
	{
		String.Assign( RWSTRING( "shadow" ) );
	}
	else
	{
		String.Assign( RWSTRING( "object" ) );
	}

	m_pAffectType->SetItemValue( (LPARAM)String.GetBuffer() );

	// Color

	{
		RwRGBAReal		Color;
		Color.red	= pLightEntity->GetColor()->GetRed();
		Color.green	= pLightEntity->GetColor()->GetGreen();
		Color.blue	= pLightEntity->GetColor()->GetBlue();
		Color.alpha	= pLightEntity->GetColor()->GetAlpha();

		m_pLightColorR->SetItemValue( (LPARAM)(&Color.red) );
		m_pLightColorG->SetItemValue( (LPARAM)(&Color.green) );
		m_pLightColorB->SetItemValue( (LPARAM)(&Color.blue) );
		m_pLightColorA->SetItemValue( (LPARAM)(&Color.alpha) );

		TCHAR			szBuffer[256];
		_stprintf( szBuffer, _T( "(%.2f, %.2f, %.2f, %.2f)" ), Color.red, Color.green, Color.blue, Color.alpha );
		String.Assign( szBuffer );

		m_pLightColor->SetItemValue( (LPARAM)String.GetBuffer() );
	}

	// Shadow

	if( pLightEntity->IsShadowLight() != FALSE )
	{
		if( m_pShadowOffset != NULL )
		{
			RwReal			fMultipleFactor = pLightEntity->GetMultipleFactor();
			m_pShadowMultipleFactor->SetItemValue( (LPARAM)(&fMultipleFactor) );

			RwReal			fShadowOffset = pLightEntity->GetShadowOffset();
			m_pShadowOffset->SetItemValue( (LPARAM)(&fShadowOffset) );

			RwReal			fSubShadowFactor = pLightEntity->GetSubShadowFactor();
			m_pShadowSubShadowFactor->SetItemValue( (LPARAM)(&fSubShadowFactor) );
		}
	}

	if( bUpdate == true )
	{
		UpdatedItems();
	}

	return true;
}


bool CLightEntityPropertyTree::Apply( CPropTreeItem * pProperty, bool bUpdate )
{
	if( CEntityPropertyTree::Apply( pProperty, false ) == true )
	{
		return true;
	}

	bool				bModified = false;
	CLightEntity *		pLightEntity = reinterpret_cast<CLightEntity *>(m_pEntity);

	if(	( pProperty->GetCtrlID() == m_pLightColorR->GetCtrlID() ) ||
		( pProperty->GetCtrlID() == m_pLightColorG->GetCtrlID() ) ||
		( pProperty->GetCtrlID() == m_pLightColorB->GetCtrlID() ) ||
		( pProperty->GetCtrlID() == m_pLightColorA->GetCtrlID() ) )
	{
		// Color

		SColorFloat				Color;

		Color.SetRed( *(float*)(m_pLightColorR->GetItemValue()) );
		Color.SetGreen( *(float*)(m_pLightColorG->GetItemValue()) );
		Color.SetBlue( *(float*)(m_pLightColorB->GetItemValue()) );
		Color.SetAlpha( *(float*)(m_pLightColorA->GetItemValue()) );

		pLightEntity->SetColor( &Color ); // Replace Color Command

		pLightEntity->Invalidate();

		bModified = true;
	}

	if(	m_pShadowMultipleFactor != NULL && pProperty->GetCtrlID() == m_pShadowMultipleFactor->GetCtrlID() )
	{
		RwReal			fNewMultipleFactor = *(float*)(m_pShadowMultipleFactor->GetItemValue());

		pLightEntity->SetMultipleFactor( fNewMultipleFactor );

		bModified = true;
	}

	if(	m_pShadowOffset != NULL && pProperty->GetCtrlID() == m_pShadowOffset->GetCtrlID() )
	{
		RwReal			fNewShadowOffset = *(float*)(m_pShadowOffset->GetItemValue());

		pLightEntity->SetShadowOffset( fNewShadowOffset );

		bModified = true;
	}

	if(	m_pShadowSubShadowFactor != NULL && pProperty->GetCtrlID() == m_pShadowSubShadowFactor->GetCtrlID() )
	{
		RwReal			fNewSubShadowFactor = *(float*)(m_pShadowSubShadowFactor->GetItemValue());

		pLightEntity->SetSubShadowFactor( fNewSubShadowFactor );

		bModified = true;
	}

	if( bModified == true && bUpdate == true )
	{
		Update( m_pEntity );
	}

    return bModified;
}

