//*****************************************************************************
// File       : CAtomicEntity_RpAtomic.cpp
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

#include "CSceneManager.h"

#include "CAtomicEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

static RwBlendFunction DualPassSrcBlendMode = rwBLENDSRCALPHA;
static RwBlendFunction DualPassDestBlendMode = rwBLENDINVSRCALPHA;


//-----------------------------------------------------------------------------
// Name			: GetRpGeometry
// Desc			: 
// Privilege	: 
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpGeometry * CAtomicEntity::GetRpGeometry( void ) const
{
	if( GetRpAtomic() == NULL )
	{
		return NULL;
	}

	return RpAtomicGetGeometry( GetRpAtomic() );
}


//-----------------------------------------------------------------------------
// Name			: CreateUVAnimation
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::CreateUVAnimation( void )
{
	for( RwUInt32 iLayer = 0; iLayer < GetUVAnimationLayer(); ++iLayer )
	{
		m_pUVAnimationMatrix[iLayer] = RwMatrixCreate();
	}

	RpGeometry *	pRpGeometry = RpAtomicGetGeometry( GetRpAtomic() );
	Assert( pRpGeometry != NULL );

//	RwUInt32		iMaterialCount = RpGeometryGetNumMaterials( pRpGeometry );

	RpMaterial *	pBaseMaterial;

	pBaseMaterial = RpGeometryGetMaterial( pRpGeometry, 0 );
	Assert( pBaseMaterial != NULL );

	RwTexture *		pDualTexture = RpMatFXMaterialGetDualTexture( pBaseMaterial );
 	if( pDualTexture == NULL )
	{
		return TRUE;
	}

	pDualTexture = RwTextureRead( RwTextureGetName( pDualTexture ), NULL );
	if( pDualTexture == NULL )
	{
		return FALSE;
	}

	RpMatFXMaterialSetEffects( pBaseMaterial, rpMATFXEFFECTDUALUVTRANSFORM );
	RpMatFXMaterialSetupDualTexture( pBaseMaterial, pDualTexture, DualPassSrcBlendMode, DualPassDestBlendMode );
	RpMatFXMaterialSetUVTransformMatrices( pBaseMaterial,
											m_pUVAnimationMatrix[E_UV_ANIMATION_LAYER_BASE],
											m_pUVAnimationMatrix[E_UV_ANIMATION_LAYER_DUAL] );

	RwTextureSetFilterMode( pDualTexture, rwFILTERLINEAR );
	RwTextureSetAddressing( pDualTexture, rwTEXTUREADDRESSWRAP );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: DestroyUVAnimation
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAtomicEntity::DestroyUVAnimation( void )
{
	for( RwUInt32 iLayer = 0; iLayer < GetUVAnimationLayer(); ++iLayer )
	{
		if( m_pUVAnimationMatrix[iLayer] != NULL )
		{
			RwMatrixDestroy( m_pUVAnimationMatrix[iLayer] );
			m_pUVAnimationMatrix[iLayer] = NULL;
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetUVAnimationMatrix
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::SetUVAnimationMatrix( EUVAnimationLayer eLayer, SPosition3D * pPosition, SLocation3D * pScale )
{
	if( m_pUVAnimationMatrix[eLayer] == NULL )
	{
		return FALSE;
	}

	if( pScale == NULL )
	{
		SetMatrix( m_pUVAnimationMatrix[eLayer], pPosition );
	}

	return TRUE;
}


//*****************************************************************************
//
// End of File : CAtomicEntity_RpAtomic.cpp
//
//*****************************************************************************