//*****************************************************************************
// File       : CCameraEntity_RwCamera.cpp
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

#include "CCameraEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: Clear
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::Clear( void ) const
{
	return reinterpret_cast<RwBool>(RwCameraClear( GetRwCamera(), GetBackgroundColor()->GetRwRGBA(), rwCAMERACLEARZ | rwCAMERACLEARIMAGE ));
}


//-----------------------------------------------------------------------------
// Name			: ShowRaster
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::ShowRaster( RwUInt32 iFlags ) const
{
	return reinterpret_cast<RwBool>(RwCameraShowRaster( GetRwCamera(), m_hWnd, iFlags ));
}


//-----------------------------------------------------------------------------
// Name			: BeginUpdate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::BeginUpdate( void ) const
{
	Assert( GetRwCamera() != NULL );

	return reinterpret_cast<RwBool>(RwCameraBeginUpdate( GetRwCamera() ));
}


//-----------------------------------------------------------------------------
// Name			: EndUpdate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::EndUpdate( void ) const
{
	Assert( GetRwCamera() != NULL );

	return reinterpret_cast<RwBool>(RwCameraEndUpdate( GetRwCamera() ));
}


//-----------------------------------------------------------------------------
// Name			: SetFogDistance
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::SetFogDistance( RwReal fDistance )
{
	RwCameraSetFogDistance( GetRwCamera(), fDistance );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: SetRaster
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CCameraEntity::SetRaster( const SRasterInfo * pRasterInfo )
{
	Assert( pRasterInfo != NULL );

	RwCameraSetRaster( GetRwCamera(), RwRasterCreate( pRasterInfo->GetWidth(), pRasterInfo->GetHeight(), 0, rwRASTERTYPECAMERA ) );

	if( pRasterInfo->HasZBuffer() != FALSE )
	{
		RwCameraSetZRaster( GetRwCamera(), RwRasterCreate( pRasterInfo->GetWidth(), pRasterInfo->GetHeight(), 0, rwRASTERTYPEZBUFFER ) );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetClipPlane
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CCameraEntity::SetClipPlane( const SClipPlane * pClipPlane )
{
	Assert( pClipPlane != NULL );

	RwCameraSetNearClipPlane( GetRwCamera(), pClipPlane->GetNear() );
	RwCameraSetFarClipPlane( GetRwCamera(), pClipPlane->GetFar() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetRwFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CCameraEntity::SetRwFrame( const RwFrame * pRwFrame )
{
	if( GetRwCamera() == NULL )
	{
		return;
	}

	RwCameraSetFrame( GetRwCamera(), const_cast<RwFrame *>(pRwFrame) );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetRwFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwFrame * CCameraEntity::GetRwFrame( void ) const
{
	if( GetRwCamera() == NULL )
	{
		return NULL;
	}

	return RwCameraGetFrame( GetRwCamera() );
}


//-----------------------------------------------------------------------------
// Name			: GetRay
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::GetRay( RwV2d * pScreenPosition, RwLine * pRay )
{
	if( GetRwCamera() == NULL )
	{
		return FALSE;
	}

	RwCameraCalcPixelRay( GetRwCamera(), pRay, pScreenPosition );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: SetFov
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCameraEntity::SetFov( RwReal fFov, RwReal fAspect )
{
	GetCameraProperty()->SetFov( fFov );

	RwReal		fFovRadianY = fFov * rwPI / 360.0f;

	RwV2d		Aspect;

	Aspect.x = tanf( fFovRadianY );
	Aspect.y = Aspect.x / fAspect;

	RwCameraSetViewWindow( GetRwCamera(), &Aspect );

	return TRUE;
}


//*****************************************************************************
//
// End of File : CCameraEntity_RwCamera.cpp
//
//*****************************************************************************