#include "StdAfx.h"
#include "Resource.h"

#include "CCameraEntityProperty.h"
#include "CCameraEntity.h"

#include "CInstanceTreeCtrl.h"

#include "CCameraEntityPropertyTree.h"


void CCameraEntityPropertyTree::Initialize( void )
{
	CEntityPropertyTree::Initialize();

	m_pCamera					= NULL;

	m_pCameraRaster				= NULL;
	m_pCameraRasterW			= NULL;
	m_pCameraRasterH			= NULL;

	m_pCameraClipPlane			= NULL;
	m_pCameraClipPlaneN			= NULL;
	m_pCameraClipPlaneF			= NULL;

	m_pCameraBackgroundColor	= NULL;
	m_pCameraBackgroundColorR	= NULL;
	m_pCameraBackgroundColorG	= NULL;
	m_pCameraBackgroundColorB	= NULL;
	m_pCameraBackgroundColorA	= NULL;

	m_pCameraFov				= NULL;

	return;
}


bool CCameraEntityPropertyTree::Create( CWnd * pWnd, CEntity * pEntity, bool bUpdate )
{
	if( CEntityPropertyTree::Create( pWnd, pEntity, false ) == false )
	{
		return false;
	}

	// Camera

	m_pCamera = InsertItem( new CPropTreeItem() );
	m_pCamera->SetLabelText( _T( "Camera" ) );
	m_pCamera->SetInfoText( _T( "This is property of camera instance." ) );
	m_pCamera->Expand();

	// Camera Raster

	m_pCameraRaster = InsertItem( new CPropTreeItemStatic(), m_pCamera );
	m_pCameraRaster->SetLabelText( _T( "Raster" ) );
	m_pCameraRaster->SetInfoText( _T( "This is raster info of light." ) );

	{
		m_pCameraRasterW = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pCameraRaster );
		m_pCameraRasterW->SetLabelText( _T( "Width" ) );
		m_pCameraRasterW->SetInfoText( _T( "This is width of raster screen." ) );
		m_pCameraRasterW->SetValueFormat( CPropTreeItemEdit::ValueFormatNumber );

		m_pCameraRasterH = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pCameraRaster );
		m_pCameraRasterH->SetLabelText( _T( "Height" ) );
		m_pCameraRasterH->SetInfoText( _T( "This is height of raster screen" ) );
		m_pCameraRasterH->SetValueFormat( CPropTreeItemEdit::ValueFormatNumber );
	}

	// Camera Clip Plane

	m_pCameraClipPlane = InsertItem( new CPropTreeItemStatic(), m_pCamera );
	m_pCameraClipPlane->SetLabelText( _T( "Clip Plane" ) );
	m_pCameraClipPlane->SetInfoText( _T( "This is clip plane of light." ) );

	{
		m_pCameraClipPlaneN = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pCameraClipPlane );
		m_pCameraClipPlaneN->SetLabelText( _T( "Near" ) );
		m_pCameraClipPlaneN->SetInfoText( _T( "This is near distance of clip plane." ) );
		m_pCameraClipPlaneN->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );

		m_pCameraClipPlaneF = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pCameraClipPlane );
		m_pCameraClipPlaneF->SetLabelText( _T( "Far" ) );
		m_pCameraClipPlaneF->SetInfoText( _T( "This is far distance of clip plane" ) );
		m_pCameraClipPlaneF->SetValueFormat( CPropTreeItemEdit::ValueFormatFloatPointer );
	}

	// Background Color

	m_pCameraBackgroundColor = InsertItem( new CPropTreeItemStatic(), m_pCamera );
	m_pCameraBackgroundColor->SetLabelText( _T( "Background Color" ) );
	m_pCameraBackgroundColor->SetInfoText( _T( "This is background color of screen." ) );

	{
		m_pCameraBackgroundColorR = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pCameraBackgroundColor );
		m_pCameraBackgroundColorR->SetLabelText( _T( "Red" ) );
		m_pCameraBackgroundColorR->SetInfoText( _T( "This is red background color of screen." ) );
		m_pCameraBackgroundColorR->SetValueFormat( CPropTreeItemEdit::ValueFormatNumber );

		m_pCameraBackgroundColorG = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pCameraBackgroundColor );
		m_pCameraBackgroundColorG->SetLabelText( _T( "Green" ) );
		m_pCameraBackgroundColorG->SetInfoText( _T( "This is green background color of screen" ) );
		m_pCameraBackgroundColorG->SetValueFormat( CPropTreeItemEdit::ValueFormatNumber );

		m_pCameraBackgroundColorB = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pCameraBackgroundColor );
		m_pCameraBackgroundColorB->SetLabelText( _T( "Blue" ) );
		m_pCameraBackgroundColorB->SetInfoText( _T( "This is blue background color of screen" ) );
		m_pCameraBackgroundColorB->SetValueFormat( CPropTreeItemEdit::ValueFormatNumber );

		m_pCameraBackgroundColorA = (CPropTreeItemEdit*)InsertItem( new CPropTreeItemEdit(), m_pCameraBackgroundColor );
		m_pCameraBackgroundColorA->SetLabelText( _T( "Alpha" ) );
		m_pCameraBackgroundColorA->SetInfoText( _T( "This is background transparency of screen" ) );
		m_pCameraBackgroundColorA->SetValueFormat( CPropTreeItemEdit::ValueFormatNumber );
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


bool CCameraEntityPropertyTree::Update( CEntity * pEntity, bool bUpdate )
{
	CEntityPropertyTree::Update( pEntity, false );

	Assert( pEntity->GetType() == E_ENTITY_TYPE_CAMERA );

	CCameraEntity *			pCameraEntity = reinterpret_cast<CCameraEntity *>(pEntity);
	CCameraEntityProperty *	pCameraEntityProperty = reinterpret_cast<CCameraEntityProperty *>(pCameraEntity->GetProperty());
	CFixedString			String;


	// Raster

	{
		POINT		Raster;
		Raster.x	= pCameraEntityProperty->GetRasterInfo()->GetWidth();
		Raster.y	= pCameraEntityProperty->GetRasterInfo()->GetHeight();

		m_pCameraRasterW->SetItemValue( (LPARAM)(Raster.x) );
		m_pCameraRasterH->SetItemValue( (LPARAM)(Raster.y) );

		TCHAR		szBuffer[256];
		_stprintf( szBuffer, _T( "(Width : %d, Height : %d)" ), Raster.x, Raster.y );
		String.Assign( szBuffer );

		m_pCameraRaster->SetItemValue( (LPARAM)String.GetBuffer() );
	}

	// Clip Plane

	{
		RwV2d		ClipPlane;
		ClipPlane.x	= pCameraEntityProperty->GetClipPlane()->GetNear();
		ClipPlane.y	= pCameraEntityProperty->GetClipPlane()->GetFar();

		m_pCameraClipPlaneN->SetItemValue( (LPARAM)(&ClipPlane.x) );
		m_pCameraClipPlaneF->SetItemValue( (LPARAM)(&ClipPlane.y) );

		TCHAR			szBuffer[256];
		_stprintf( szBuffer, _T( "(Near : %.1f, Far : %.1f)" ), ClipPlane.x, ClipPlane.y );
		String.Assign( szBuffer );

		m_pCameraClipPlane->SetItemValue( (LPARAM)String.GetBuffer() );
	}


	// Background Color

	{
		RwRGBA		Color;
		Color.red	= pCameraEntity->GetBackgroundColor()->GetRed();
		Color.green	= pCameraEntity->GetBackgroundColor()->GetGreen();
		Color.blue	= pCameraEntity->GetBackgroundColor()->GetBlue();
		Color.alpha	= pCameraEntity->GetBackgroundColor()->GetAlpha();

		m_pCameraBackgroundColorR->SetItemValue( (LPARAM)(Color.red) );
		m_pCameraBackgroundColorG->SetItemValue( (LPARAM)(Color.green) );
		m_pCameraBackgroundColorB->SetItemValue( (LPARAM)(Color.blue) );
		m_pCameraBackgroundColorA->SetItemValue( (LPARAM)(Color.alpha) );

		TCHAR		szBuffer[256];
		_stprintf( szBuffer, _T( "(%d, %d, %d, %d)" ), Color.red, Color.green, Color.blue, Color.alpha );
		String.Assign( szBuffer );

		m_pCameraBackgroundColor->SetItemValue( (LPARAM)String.GetBuffer() );
	}

	if( bUpdate == true )
	{
		UpdatedItems();
	}

	return true;
}


bool CCameraEntityPropertyTree::Apply( CPropTreeItem * pProperty, bool bUpdate )
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

