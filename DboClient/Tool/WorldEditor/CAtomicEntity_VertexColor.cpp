//*****************************************************************************
// File       : CAtomicEntity.cpp
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

#include "CFixedString.h"
#include "CFixedStringList.h"

#include "CEntityContainer.h"
#include "CEntityFactory.h"
#include "CEntityResource.h"

#include "Collision.h"
#include "CEntityCollisionList.h"

#include "CCameraEntity.h"
#include "CLightEntity.h"
#include "CMeshEntity.h"

#include "CEntityCollision.h"
#include "CSceneManager.h"
#include "CEditorApplication.h"

#include "CAtomicEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: CalculateVertexColor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::CalculateVertexColor( SVertexColorGenerationInfo * pInfo )
{
	if( IsTerrain() == FALSE )
	{
		return FALSE;
	}

	if( pInfo == NULL )
	{
		return FALSE;
	}

 	RpGeometry *	pGeometry = RpAtomicGetGeometry( GetRpAtomic() );
	RwRGBA *		pColors = RpGeometryGetPreLightColors( pGeometry );
	if( pColors == NULL )
	{
		return FALSE;
	}

	RpMorphTarget *	pMorphTarget = RpGeometryGetMorphTarget( pGeometry, 0 );
	if( pMorphTarget == NULL )
	{
		return FALSE;
	}

	RwV3d *			pVertices = RpMorphTargetGetVertices( pMorphTarget );
	if( pVertices == NULL )
	{
		return FALSE;
	}

	RwV3d *			pNormals = RpMorphTargetGetVertexNormals( pMorphTarget );
	if( pNormals == NULL )
	{
		return FALSE;
	}

	RwV3d			sLightNormal;
	RwV3d			sShadowNormal;
	RwRGBAReal		sAmbient;
	RwRGBAReal		sDirectional;
	RwRGBAReal		sShadow;
	RwReal			fNormalFactor;
	RwReal			fShadowFactor;
	RwRGBAReal		sVertexColor;

	if( pInfo->pAmbientLight != NULL )
	{
		CLightEntity *	pAmbeintLight = pInfo->pAmbientLight;
		const_cast<SColorFloat *>(pAmbeintLight->GetColor())->Get( &sAmbient );
	}
	else
	{
		ZeroMemory( &sAmbient, sizeof( RwRGBAReal ) );
	}

	if( pInfo->pDirectionalLight != NULL )
	{
		CLightEntity *	pDirectionalLight = pInfo->pDirectionalLight;
		const_cast<SColorFloat *>(pDirectionalLight->GetColor())->Get( &sDirectional );

		SRotation3D *	pLightRotation = const_cast<SRotation3D *>(pDirectionalLight->GetPosition()->GetRotation());
		pLightRotation->GetDirectionVector( &sLightNormal );

		sLightNormal.x = -sLightNormal.x;
		sLightNormal.y = -sLightNormal.y;
		sLightNormal.z = -sLightNormal.z;
	}
	else
	{
		ZeroMemory( &sDirectional, sizeof( RwRGBAReal ) );
		ZeroMemory( &sLightNormal, sizeof( RwV3d ) );

		fNormalFactor = 0.0f;
	}

	if( pInfo->pShadowLight != NULL )
	{
		CLightEntity *	pShadowLight = pInfo->pShadowLight;
		const_cast<SColorFloat *>(pShadowLight->GetColor())->Get( &sShadow );

		SRotation3D *	pShadowRotation = const_cast<SRotation3D *>(pShadowLight->GetPosition()->GetRotation());
		pShadowRotation->GetDirectionVector( &sShadowNormal );

		sShadowNormal.x = -sShadowNormal.x;
		sShadowNormal.y = -sShadowNormal.y;
		sShadowNormal.z = -sShadowNormal.z;

	}
	else
	{
		ZeroMemory( &sShadow, sizeof( RwRGBAReal ) );
		ZeroMemory( &sShadowNormal, sizeof( RwV3d ) );
	}

	fShadowFactor = 0.0f;

	for( RwInt32 iVertex = 0; iVertex < RpGeometryGetNumVertices( pGeometry ); ++iVertex )
	{
		if( pInfo->pDirectionalLight != NULL )
		{
			fNormalFactor = CalculateVertexNormalFactor( &(pNormals[iVertex]), &sLightNormal, pInfo->pDirectionalLight->GetDirectionalCutOff() );
		}

		if( pInfo->pShadowLight != NULL )
		{
			fShadowFactor = CalculateVertexShadowFactor( &(pVertices[iVertex]), &sShadowNormal, pInfo->pShadowLight->GetMultipleFactor(), pInfo->pShadowLight->GetShadowOffset(), pInfo->pShadowLight->GetSubShadowFactor() );
		}

		if( pInfo->pAmbientLight == NULL && pInfo->pDirectionalLight == NULL )
		{
			sVertexColor.red	= pColors[iVertex].red * ( 1.0f - fShadowFactor * sShadow.red );
			sVertexColor.green	= pColors[iVertex].green * ( 1.0f - fShadowFactor * sShadow.green );
			sVertexColor.blue	= pColors[iVertex].blue * ( 1.0f - fShadowFactor * sShadow.blue );
		}
		else
		{
			sVertexColor.red	= sAmbient.red * ( 1.0f - fShadowFactor * sShadow.red * (1.0f - fNormalFactor) * 0.5f ) + (sDirectional.red * fNormalFactor) * ( 1.0f - fShadowFactor * sShadow.red * (1.0f - fNormalFactor) );
			sVertexColor.green	= sAmbient.green * ( 1.0f - fShadowFactor * sShadow.red * (1.0f - fNormalFactor) * 0.5f ) + (sDirectional.green * fNormalFactor) * ( 1.0f - fShadowFactor * sShadow.green * (1.0f - fNormalFactor) );
			sVertexColor.blue	= sAmbient.blue * ( 1.0f - fShadowFactor * sShadow.red * (1.0f - fNormalFactor) * 0.5f ) + (sDirectional.blue * fNormalFactor) * ( 1.0f - fShadowFactor * sShadow.blue * (1.0f - fNormalFactor) );
		}

		// Gamma

//		sVertexColor.red	= ( sVertexColor.red <= 0.081f ) ? sVertexColor.red / 4.5f : pow( (sVertexColor.red + 0.099f) / 1.099f, 1.0f / 0.45f );
//		sVertexColor.green	= ( sVertexColor.green <= 0.081f ) ? sVertexColor.green / 4.5f : pow( (sVertexColor.green + 0.099f) / 1.099f, 1.0f / 0.45f );
//		sVertexColor.blue	= ( sVertexColor.blue <= 0.081f ) ? sVertexColor.blue / 4.5f : pow( (sVertexColor.blue + 0.099f) / 1.099f, 1.0f / 0.45f );

//		sVertexColor.red	= ( sVertexColor.red <= 0.018 ) ? sVertexColor.red * 4.5f : 1.099f * pow( sVertexColor.red, 0.45f ) - 0.099f;
//		sVertexColor.green	= ( sVertexColor.green <= 0.018 ) ? sVertexColor.green * 4.5f : 1.099f * pow( sVertexColor.green, 0.45f ) - 0.099f;
//		sVertexColor.blue	= ( sVertexColor.blue <= 0.018 ) ? sVertexColor.blue * 4.5f : 1.099f * pow( sVertexColor.blue, 0.45f ) - 0.099f;

		// truncate

		if( sVertexColor.red > 1.0f )
		{
			sVertexColor.red = 1.0f;
		}

		if( sVertexColor.green > 1.0f )
		{
			sVertexColor.green = 1.0f;
		}

		if( sVertexColor.blue > 1.0f )
		{
			sVertexColor.blue = 1.0f;
		}

		if( sVertexColor.red < 0.0f )
		{
			sVertexColor.red = 0.0f;
		}

		if( sVertexColor.green < 0.0f )
		{
			sVertexColor.green = 0.0f;
		}

		if( sVertexColor.blue < 0.0f )
		{
			sVertexColor.blue = 0.0f;
		}

		// 
		pColors[iVertex].red	= static_cast<RwUInt8>(sVertexColor.red * 255.0f);
		pColors[iVertex].green	= static_cast<RwUInt8>(sVertexColor.green * 255.0f);
		pColors[iVertex].blue	= static_cast<RwUInt8>(sVertexColor.blue * 255.0f);
		pColors[iVertex].alpha	= 255;

		RwChar	szText[256];
		rwsprintf( szText, RWSTRING( "Create Shadow Map : %d / %d of %s" ), iVertex + 1, RpGeometryGetNumVertices( pGeometry ), GetParent()->GetNameBuffer() );
		g_pEngineApp->SetStatusText( szText );
	}

	RwResourcesEmptyArena();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: CalculateVertexShadowFactor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwReal CAtomicEntity::CalculateVertexShadowFactor( const RwV3d * pVertex, const RwV3d * pLight, RwReal fMultipleFactor, RwReal fShadowDetectOffset, RwReal fSubShadowFactor )
{
	SLocation3D					sLocation;

	static const RwReal			DETECT_NEAR				= 0.1f;
	static const RwReal			DETECT_FAR				= 400.0f;
	static const RwReal			EFFECTIVE_FACTOR		= 0.25f * fMultipleFactor;
	static const RwReal			SUB_EFFECTIVE_FACTOR	= fSubShadowFactor;

	RwLine						sRayLine;
	RwReal						fShadowFactor			= 0.0f;

	// left

	memcpy( &(sRayLine.start), pVertex, sizeof( RwV3d ) );

	sRayLine.start.x += pLight->x * DETECT_NEAR + fShadowDetectOffset;
	sRayLine.start.y += pLight->y * DETECT_NEAR;
	sRayLine.start.z += pLight->z * DETECT_NEAR;

	memcpy( &(sRayLine.end), pVertex, sizeof( RwV3d ) );

	sRayLine.end.x += pLight->x * DETECT_FAR + fShadowDetectOffset;
	sRayLine.end.y += pLight->y * DETECT_FAR;
	sRayLine.end.z += pLight->z * DETECT_FAR;

	fShadowFactor += CalculateVertexShadowPartialFactor( &sRayLine, EFFECTIVE_FACTOR, SUB_EFFECTIVE_FACTOR, fShadowDetectOffset );

	// right

	memcpy( &(sRayLine.start), pVertex, sizeof( RwV3d ) );

	sRayLine.start.x += pLight->x * DETECT_NEAR - fShadowDetectOffset;
	sRayLine.start.y += pLight->y * DETECT_NEAR;
	sRayLine.start.z += pLight->z * DETECT_NEAR;

	memcpy( &(sRayLine.end), pVertex, sizeof( RwV3d ) );

	sRayLine.end.x += pLight->x * DETECT_FAR - fShadowDetectOffset;
	sRayLine.end.y += pLight->y * DETECT_FAR;
	sRayLine.end.z += pLight->z * DETECT_FAR;

	fShadowFactor += CalculateVertexShadowPartialFactor( &sRayLine, EFFECTIVE_FACTOR, SUB_EFFECTIVE_FACTOR, fShadowDetectOffset );

	// front

	memcpy( &(sRayLine.start), pVertex, sizeof( RwV3d ) );

	sRayLine.start.x += pLight->x * DETECT_NEAR;
	sRayLine.start.y += pLight->y * DETECT_NEAR;
	sRayLine.start.z += pLight->z * DETECT_NEAR + fShadowDetectOffset;

	memcpy( &(sRayLine.end), pVertex, sizeof( RwV3d ) );

	sRayLine.end.x += pLight->x * DETECT_FAR;
	sRayLine.end.y += pLight->y * DETECT_FAR;
	sRayLine.end.z += pLight->z * DETECT_FAR + fShadowDetectOffset;

	fShadowFactor += CalculateVertexShadowPartialFactor( &sRayLine, EFFECTIVE_FACTOR, SUB_EFFECTIVE_FACTOR, fShadowDetectOffset );

	// back

	memcpy( &(sRayLine.start), pVertex, sizeof( RwV3d ) );

	sRayLine.start.x += pLight->x * DETECT_NEAR;
	sRayLine.start.y += pLight->y * DETECT_NEAR;
	sRayLine.start.z += pLight->z * DETECT_NEAR - fShadowDetectOffset;

	memcpy( &(sRayLine.end), pVertex, sizeof( RwV3d ) );

	sRayLine.end.x += pLight->x * DETECT_FAR;
	sRayLine.end.y += pLight->y * DETECT_FAR;
	sRayLine.end.z += pLight->z * DETECT_FAR - fShadowDetectOffset;

	fShadowFactor += CalculateVertexShadowPartialFactor( &sRayLine, EFFECTIVE_FACTOR, SUB_EFFECTIVE_FACTOR, fShadowDetectOffset );

	// center

	memcpy( &(sRayLine.start), pVertex, sizeof( RwV3d ) );
	sRayLine.start.x += pLight->x * DETECT_NEAR;
	sRayLine.start.y += pLight->y * DETECT_NEAR;
	sRayLine.start.z += pLight->z * DETECT_NEAR;

	memcpy( &(sRayLine.end), pVertex, sizeof( RwV3d ) );
	sRayLine.end.x += pLight->x * DETECT_FAR;
	sRayLine.end.y += pLight->y * DETECT_FAR;
	sRayLine.end.z += pLight->z * DETECT_FAR;

	fShadowFactor += CalculateVertexShadowPartialFactor( &sRayLine, 1.0f - EFFECTIVE_FACTOR * 4.0f, SUB_EFFECTIVE_FACTOR, 0.0f );

	return fShadowFactor;
}


//-----------------------------------------------------------------------------
// Name			: CalculateVertexShadowPartialFactor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwReal CAtomicEntity::CalculateVertexShadowPartialFactor( RwLine * pLine, RwReal fEffectiveFactor, RwReal fSubeffectiveFactor, RwReal DETECT_OFFSET )
{
	CCollisionDetectionInfo		cDetectionInfo;
	CCollisionDetectionInfo		cDetectionInfoInv;
	CEntity *					pEntity;
	CEntityCollision *			pEntityCollision;
	RwInt32						iEffectiveCount		= 0;
	RwInt32						iSubeffectiveCount	= 0;


	cDetectionInfo.CreateByDirectionalLight( pLine, FALSE );
	g_pSceneManager->DetectCollision( &cDetectionInfo );

	for( RwInt32 iOrder = 0; iOrder < cDetectionInfo.GetResultList()->GetCount(); ++iOrder )
	{
		pEntityCollision = cDetectionInfo.GetResultList()->GetCollisionEntityByOrder( iOrder );
		pEntity = pEntityCollision->GetEntity();

		if( pEntity->GetType() != E_ENTITY_TYPE_ATOMIC )
		{
			continue;
		}

		if( pEntity->IsTerrain() != FALSE )
		{
			SLocation3D			Start( &(pLine->start) );
			SLocation3D			End( &(pLine->end) );
			if( pEntityCollision->GetLocation()->GetDistance( Start ) >= DETECT_OFFSET &&
				pEntityCollision->GetLocation()->GetDistance( End ) >= DETECT_OFFSET )
			{
				++iEffectiveCount;
			}
		}
		else
		{
			if( reinterpret_cast<CAtomicEntity *>(pEntity)->GetShadowProjectOption() & E_SHADOW_OPTION_TERRAIN )
			{
				++iEffectiveCount;
			}
			else
			{
				++iSubeffectiveCount;
			}
		}
	}

	cDetectionInfo.GetResultList()->RemoveAll( TRUE );

	// Inverse

	RwV3d		sBackUp;

	memcpy( &sBackUp, &(pLine->start), sizeof( RwV3d ) );
	memcpy( &(pLine->start), &(pLine->end), sizeof( RwV3d ) );
	memcpy( &(pLine->end), &sBackUp, sizeof( RwV3d ) );

	cDetectionInfoInv.CreateByDirectionalLight( pLine, TRUE );
	g_pSceneManager->DetectCollision( &cDetectionInfoInv );

	for( RwInt32 iOrder = 0; iOrder < cDetectionInfoInv.GetResultList()->GetCount(); ++iOrder )
	{
		pEntityCollision = cDetectionInfoInv.GetResultList()->GetCollisionEntityByOrder( iOrder );
		pEntity = pEntityCollision->GetEntity();

		if( pEntity->GetType() != E_ENTITY_TYPE_ATOMIC )
		{
			continue;
		}

		if( pEntity->IsTerrain() != FALSE )
		{
			SLocation3D			Start( &(pLine->start) );
			SLocation3D			End( &(pLine->end) );
			if( pEntityCollision->GetLocation()->GetDistance( Start ) >= DETECT_OFFSET &&
				pEntityCollision->GetLocation()->GetDistance( End ) >= DETECT_OFFSET )
			{
				++iEffectiveCount;
			}
		}
		else
		{
			if( reinterpret_cast<CAtomicEntity *>(pEntity)->GetShadowProjectOption() & E_SHADOW_OPTION_TERRAIN )
			{
				++iEffectiveCount;
			}
			else
			{
				++iSubeffectiveCount;
			}
		}
	}

	cDetectionInfoInv.GetResultList()->RemoveAll( TRUE );

	RwReal				fPartialFactor;

	if( iEffectiveCount > 0 )
	{
		fPartialFactor = exp( -static_cast<RwReal>(iEffectiveCount) );
		fPartialFactor = fEffectiveFactor * ( 1.0f - fPartialFactor );

		return fPartialFactor;
	}
	else if( iSubeffectiveCount > 0 )
	{
		fPartialFactor = exp( -static_cast<RwReal>(iSubeffectiveCount) );
		fPartialFactor = fEffectiveFactor * ( 1.0f - fPartialFactor );

		return fPartialFactor * fSubeffectiveFactor;
	}
	else
	{
		return 0.0f;
	}
}


//-----------------------------------------------------------------------------
// Name			: CalculateVertexNormalFactor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwReal CAtomicEntity::CalculateVertexNormalFactor( const RwV3d * pNormal, const RwV3d * pLight, RwReal fCutOff )
{
	RwReal	fNormalFactor = RwV3dDotProduct( pNormal, pLight );

	if( fNormalFactor > 0.0f )
	{
		fNormalFactor = fNormalFactor * ( 1.0f - fCutOff ) + fCutOff;
	}
	else
	{
		fNormalFactor = fNormalFactor * fCutOff + fCutOff;
	}

	return fNormalFactor;
}


//*****************************************************************************
//
// End of File : CAtomicEntity.cpp
//
//*****************************************************************************