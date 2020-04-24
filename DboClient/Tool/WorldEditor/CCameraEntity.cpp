//*****************************************************************************
// File       : CCameraEntity.cpp
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CEntityContainer.h"
#include "CEntityFactory.h"

#include "CFogEntity.h"

#include "CFixedStringList.h"

#include "CCameraEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CCameraEntity::Initialize( void )
{
	CEntity::Initialize();

	m_hWnd			= NULL;

	SetMovable( TRUE );

	m_Front.Set( &AXIS_Z );
	m_Up.Set( &AXIS_Y );
	m_Left.Set( &AXIS_X );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty )
{
	BEGIN_FUNCTION( "CCameraEntity::Create" );

	Assert( pEntityProperty != NULL );
	Assert( pEntityProperty->GetType() == E_ENTITY_TYPE_CAMERA );

	END_FUNCTION( CEntity::Create( pEntityProperty, pCreateProperty ) );
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::PostCreate( void )
{
	SetFov( GetCameraProperty()->GetFov() );

	return CEntity::PostCreate();
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CCameraEntity::Destroy( void )
{
	BEGIN_FUNCTION( "CCameraEntity::Destroy" );

	// Destroy Frame

	RwFrame *	pRwFrame = const_cast<RwFrame *>(GetRwFrame());
	if( pRwFrame != NULL )
	{
		RwCameraSetFrame( GetRwCamera(), NULL );

		RwFrameDestroy( pRwFrame );
	}

	// Destroy Raster

	RwRaster *	pRaster = RwCameraGetRaster( GetRwCamera() );
	if( pRaster != NULL )
	{
		RwCameraSetRaster( GetRwCamera(), NULL );

		RwRasterDestroy( pRaster );
	}

	// Destroy Z-Raster

	RwRaster *	pZRaster = RwCameraGetZRaster( GetRwCamera() );
	if( pZRaster != NULL )
	{
		RwCameraSetRaster( GetRwCamera(), NULL );

		RwRasterDestroy( pZRaster );
	}

	CEntity::Destroy();

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: UpdateFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::UpdateFrame( void )
{
	BEGIN_FUNCTION( "CCameraEntity::UpdateFrame" );

	if( IsModified() != FALSE )
	{
		RwFrame *		pFrame = const_cast<RwFrame *>(GetRwFrame());

		RwMatrix		Matrix;
		RwMatrix		MatrixLocal;

		RwMatrixRotate( &Matrix, &AXIS_Y, GetPosition()->GetRotation()->GetYaw(), rwCOMBINEREPLACE );
		RwMatrixCopy( &MatrixLocal, &Matrix );

		RwV3dTransformVector( &m_Front, &AXIS_Z, &Matrix );
		RwV3dTransformVector( &m_Left, &AXIS_X, &Matrix );

		RwMatrixRotate( &Matrix, &m_Left, GetPosition()->GetRotation()->GetPitch(), rwCOMBINEREPLACE );
		RwMatrixMultiply( &MatrixLocal, &MatrixLocal, &Matrix );

		RwV3dTransformVector( &m_Up, &AXIS_Y, &Matrix );
		RwV3dTransformVector( &m_Front, &m_Front, &Matrix );

		RwMatrixRotate( &Matrix, &m_Front, GetPosition()->GetRotation()->GetRoll(), rwCOMBINEREPLACE );
		RwMatrixMultiply( &MatrixLocal, &MatrixLocal, &Matrix );

		RwV3dTransformVector( &m_Up, &m_Up, &Matrix );
		RwV3dTransformVector( &m_Left, &m_Left, &Matrix );

		RwV3dNormalize( &m_Direction, &m_Front );

		RwFrameTransform( pFrame, &MatrixLocal, rwCOMBINEREPLACE );
		pFrame = RwFrameTranslate( pFrame, GetPosition()->GetLocation()->GetRwV3d(), rwCOMBINEPOSTCONCAT );

		RwFrameUpdateObjects( pFrame );
	}

	SetVisible( DetermineVisiblilty() );

	END_FUNCTION( CEntity::UpdateFrame() );
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::RenderFrame( CEntity * pRenderer )
{
	BEGIN_FUNCTION( "CCameraEntity::RenderFrame" );

/*	if( pRenderer != NULL && pRenderer->GetFrameID() <= GetFrameID() )
	{
		END_FUNCTION( TRUE );
	}
	else
	{
		SetFrameID( pRenderer->GetFrameID() );
	}
*/
	if( IsVisible() == FALSE )
	{
		END_FUNCTION( TRUE );
	}

//	UpdateFrame();

	END_FUNCTION( CEntity::RenderFrame( pRenderer ) );
}


//-----------------------------------------------------------------------------
// Name			: SetBackgroundColor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CCameraEntity::SetBackgroundColor( const SColorInt * pBackgroundColor )
{
	BEGIN_FUNCTION( "CCameraEntity::SetBackgroundColor" );

	Assert( GetCameraProperty() != NULL );

	GetCameraProperty()->SetBackgroundColor( pBackgroundColor );

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: GetBackgroundColor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const SColorInt * CCameraEntity::GetBackgroundColor( void ) const
{
	BEGIN_FUNCTION( "CCameraEntity::GetBackgroundColor" );

	Assert( GetCameraProperty() != NULL );

	END_FUNCTION( GetCameraProperty()->GetBackgroundColor() );
}


//-----------------------------------------------------------------------------
// Name			: SetParameter
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::SetParameter( void )
{
	// SetPosition

	if( m_iRenderPriority == ENTITY_RENDER_PRIORITY_NONE )
	{
		m_iRenderPriority	= ENTITY_RENDER_PRIORITY_CAMERA_DEFAULT;
	}

	SetPosition( GetCameraProperty()->GetPosition() );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Attach
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::Attach( CEntity * pChild )
{
	Assert( pChild != NULL );

	if( InsertChild( pChild ) == FALSE )
	{
		return FALSE;
	}

	// Link To World

	if( ( pChild->GetCategory() == E_ENTITY_CATEGORY_INTERNAL ) ||
		( pChild->GetCategory() == E_ENTITY_CATEGORY_EXTERNAL ) )
	{
		return TRUE;
	}

	switch( pChild->GetType() )
	{
		case E_ENTITY_TYPE_FOG:
		{
			if( SetFogDistance( (reinterpret_cast<CFogEntity *>(pChild))->GetRadius() ) == FALSE )
			{
				Assert( !"SetFogDistance() Fail" );

				return FALSE;
			}

			break;
		}

		case E_ENTITY_TYPE_SKY:
		{
			break;
		}

		default:
		{
			Assert( !"Not Supported Yet" );

			return FALSE;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Detach
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::Detach( CEntity * pChild )
{
	Assert( pChild != NULL );
	Assert( m_pChildren != NULL );

	if( RemoveChild( pChild ) == FALSE )
	{
		return FALSE;
	}

	// Unlink To World

	if( ( pChild->GetCategory() == E_ENTITY_CATEGORY_INTERNAL ) ||
		( pChild->GetCategory() == E_ENTITY_CATEGORY_EXTERNAL ) )
	{
		return TRUE;
	}

	switch( pChild->GetType() )
	{
		case E_ENTITY_TYPE_FOG:
		{
			break;
		}

		case E_ENTITY_TYPE_SKY:
		{
			break;
		}

		default:
		{
			Assert( !"Not Supported Yet" );

			return FALSE;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CCameraEntity::Export( CFixedStringList * pStringList, CFixedString * pName )
{
	if( GetCategory() == E_ENTITY_CATEGORY_INTERNAL )
	{
		return ExportChild( pStringList, pName );
	}

	if( ExportHeader( pStringList, pName ) == FALSE )
	{
		return -1;
	}

	CFixedString	String;

	// Set Base

	String.Assign( RWSTRING( "#base" ) );
	pStringList->Push( &String );

	// Location

	String.AssignFloat( GetPosition()->GetLocation()->GetX() );
	pStringList->Push( &String );

	String.AssignFloat( GetPosition()->GetLocation()->GetY() );
	pStringList->Push( &String );

	String.AssignFloat( GetPosition()->GetLocation()->GetZ() );
	pStringList->Push( &String );

	// Rotation

	String.AssignFloat( GetPosition()->GetRotation()->GetYaw() );
	pStringList->Push( &String );

	String.AssignFloat( GetPosition()->GetRotation()->GetPitch() );
	pStringList->Push( &String );

	String.AssignFloat( GetPosition()->GetRotation()->GetRoll() );
	pStringList->Push( &String );

	// Clip Plane

	String.AssignFloat( GetCameraProperty()->GetClipPlane()->GetNear() );
	pStringList->Push( &String );

	String.AssignFloat( GetCameraProperty()->GetClipPlane()->GetFar() );
	pStringList->Push( &String );

	// BackgroundColor

	String.AssignInteger( GetCameraProperty()->GetBackgroundColor()->GetRed() );
	pStringList->Push( &String );

	String.AssignInteger( GetCameraProperty()->GetBackgroundColor()->GetGreen() );
	pStringList->Push( &String );

	String.AssignInteger( GetCameraProperty()->GetBackgroundColor()->GetBlue() );
	pStringList->Push( &String );

	String.AssignInteger( GetCameraProperty()->GetBackgroundColor()->GetAlpha() );
	pStringList->Push( &String );

	// Fov

	String.AssignFloat( GetCameraProperty()->GetFov() );
	pStringList->Push( &String );

	String.Assign( RWSTRING( "\r\n" ) );
	pStringList->Push( &String );

	RwInt32			nExportCount = ExportChild( pStringList, pName );
	if( nExportCount < 0 )
	{
		return nExportCount;
	}

	return ++nExportCount;
}


//*****************************************************************************
//
// End of File : CCameraEntity.cpp
//
//*****************************************************************************