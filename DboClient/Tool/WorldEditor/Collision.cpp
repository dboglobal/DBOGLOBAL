//*****************************************************************************
// File       : Collision.cpp
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

#include "CEntity.h"
#include "CWorldEntity.h"

#include "CEntityCubeContainer.h"
#include "CEntityFactory.h"

#include "CAtomicEntity.h"

#include "CEntityCollision.h"
#include "CEntityCollisionList.h"

#include "Collision.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: GetLocationFromIntersection
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void GetLocationFromIntersection( RwV3d * pLocation, RpIntersection * pIntersection )
{
	switch( pIntersection->type )
	{
		case rpINTERSECTLINE:
		{
			memcpy( pLocation, &(pIntersection->t.line.start), sizeof( RwV3d ) );

			break;
		}

		case rpINTERSECTPOINT:
		{
			memcpy( pLocation, &(pIntersection->t.point), sizeof( RwV3d ) );

			break;
		}

		case rpINTERSECTSPHERE:
		{
			memcpy( pLocation, &(pIntersection->t.sphere.center), sizeof( RwV3d ) );

			break;
		}

		case rpINTERSECTBOX:
		{
			RwV3d		sCenter;

			sCenter.x = (pIntersection->t.box.inf.x + pIntersection->t.box.sup.x) * 0.5f;
			sCenter.y = (pIntersection->t.box.inf.y + pIntersection->t.box.sup.y) * 0.5f;
			sCenter.z = (pIntersection->t.box.inf.z + pIntersection->t.box.sup.z) * 0.5f;

			memcpy( pLocation, &sCenter, sizeof( RwV3d ) );

			break;
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: CreateByPickTerrain
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCollisionDetectionInfo::CreateByPickTerrain( const RwV2d * pScreenPos )
{
	m_pCondition = NTL_NEW CCollisionDetectionConditon;
	Assert( m_pCondition != NULL );

	m_pCondition->SetUseScreenPos( TRUE );
	m_pCondition->SetScreenPos( pScreenPos );
	m_pCondition->SetTerrainDetectionLevel( E_COLLISION_DETECTION_LEVEL_MESH );
	m_pCondition->SetObjectDetectionLevel( E_COLLISION_DETECTION_LEVEL_NONE );
	m_pCondition->SetHelperDetectionLevel( E_COLLISION_DETECTION_LEVEL_NONE );

	m_pResultList = NTL_NEW CEntityCollisionList;
	m_pResultList->Create();

	SetRpAtomic( NULL );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: CreateByPickObject
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCollisionDetectionInfo::CreateByPickObject( const RwV2d * pScreenPos )
{
	m_pCondition = NTL_NEW CCollisionDetectionConditon;
	Assert( m_pCondition != NULL );

	m_pCondition->SetUseScreenPos( TRUE );
	m_pCondition->SetScreenPos( pScreenPos );
	m_pCondition->SetTerrainDetectionLevel( E_COLLISION_DETECTION_LEVEL_NONE );
	m_pCondition->SetObjectDetectionLevel( E_COLLISION_DETECTION_LEVEL_MESH );
	m_pCondition->SetHelperDetectionLevel( E_COLLISION_DETECTION_LEVEL_NONE );

	m_pResultList = NTL_NEW CEntityCollisionList;
	m_pResultList->Create();

	SetRpAtomic( NULL );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: CreateByDirectionalLight
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCollisionDetectionInfo::CreateByDirectionalLight( const RwLine * pLightLine, RwBool bForward )
{
	m_pCondition = NTL_NEW CCollisionDetectionConditon;
	Assert( m_pCondition != NULL );

	RpIntersection		sIntersection;
	sIntersection.type	= rpINTERSECTLINE;
	memcpy( &(sIntersection.t.line), pLightLine, sizeof( RwLine ) );

	m_pCondition->SetUseScreenPos( FALSE );
	m_pCondition->SetIntersection( &sIntersection );

	if( bForward != FALSE )
	{
		m_pCondition->SetTerrainDetectionLevel( E_COLLISION_DETECTION_LEVEL_MESH );
	}
	else
	{
		m_pCondition->SetTerrainDetectionLevel( E_COLLISION_DETECTION_LEVEL_NONE );
	}

	m_pCondition->SetObjectDetectionLevel( E_COLLISION_DETECTION_LEVEL_MESH );
	m_pCondition->SetHelperDetectionLevel( E_COLLISION_DETECTION_LEVEL_NONE );

	m_pResultList = NTL_NEW CEntityCollisionList;
	m_pResultList->Create();

	SetRpAtomic( NULL );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: CreateByGetHeight
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CCollisionDetectionInfo::CreateByGetHeight( const RwV3d * pPosition )
{
	m_pCondition = NTL_NEW CCollisionDetectionConditon;
	Assert( m_pCondition != NULL );

	RpIntersection		sIntersection;
	sIntersection.type	= rpINTERSECTLINE;
	memcpy( &(sIntersection.t.line.start), pPosition, sizeof( RwV3d ) );
	memcpy( &(sIntersection.t.line.end), pPosition, sizeof( RwV3d ) );

	sIntersection.t.line.start.y	= +500.0f;
	sIntersection.t.line.end.y		= -500.0f;

	m_pCondition->SetUseScreenPos( FALSE );
	m_pCondition->SetIntersection( &sIntersection );

	m_pCondition->SetTerrainDetectionLevel( E_COLLISION_DETECTION_LEVEL_MESH );
	m_pCondition->SetObjectDetectionLevel( E_COLLISION_DETECTION_LEVEL_MESH );
	m_pCondition->SetHelperDetectionLevel( E_COLLISION_DETECTION_LEVEL_NONE );

	m_pResultList = NTL_NEW CEntityCollisionList;
	m_pResultList->Create();

	SetRpAtomic( NULL );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CCollisionDetectionInfo::Initialize( void )
{
	m_pCondition = NULL;
	m_pResultList = NULL;

	SetRpAtomic( NULL );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CCollisionDetectionInfo::Destroy( void )
{
	if( m_pCondition != NULL )
	{
		NTL_DELETE( m_pCondition );
		m_pCondition = NULL;
	}

	if( m_pResultList != NULL )
	{
		NTL_DELETE( m_pResultList );
		m_pResultList = NULL;
	}

	SetRpAtomic( NULL );

	return;
}


//-----------------------------------------------------------------------------
// Name			: DetectAtomicInWorldCB
// Desc			: Detect Collision by Atomic in Clump
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpAtomic * DetectAtomicInWorldCB( RpIntersection * pRpIntersection, RpWorldSector * pRpSector, RpAtomic * pRpAtomic, RwReal fDistance, void * pData )
{
	CAtomicEntity *				pAtomicEntity = reinterpret_cast<CAtomicEntity *>(CAtomicEntity::GetPlugInDataFromRpAtomic( pRpAtomic )->GetEntity());

	if( pAtomicEntity == NULL )
	{
		return pRpAtomic;
	}

	CCollisionDetectionInfo *	pCollisionInfo = reinterpret_cast<CCollisionDetectionInfo *>(pData);

	pAtomicEntity->DetectCollision( pCollisionInfo );

	return pRpAtomic;
}


//-----------------------------------------------------------------------------
// Name			: DetectPositionInAtomicCB
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpCollisionTriangle * DetectPositionInAtomicCB( RpIntersection * pIntersection, RpCollisionTriangle * pCollisionTriangle, RwReal fDistance, void * pData )
{
	CCollisionDetectionInfo *	pCollisionInfo = reinterpret_cast<CCollisionDetectionInfo *>(pData);

	if( fDistance < 0.0f && fDistance > 1.0f )
	{
		return pCollisionTriangle;
	}

	CEntity *			pAtomicEntity = CAtomicEntity::GetPlugInDataFromRpAtomic( pCollisionInfo->GetRpAtomic() )->GetEntity();

	if( pAtomicEntity == NULL )
	{
		return pCollisionTriangle;
	}

	CEntityCollision	sNewResult;
	sNewResult.SetEntity( pAtomicEntity );

	GetPositionInLine( &(pCollisionInfo->GetCondition()->GetIntersection()->t.line), fDistance, sNewResult.GetLocation()->GetRwV3d() );

	RwV3d				sDistaceVector;

	GetLocationFromIntersection( &sDistaceVector, const_cast<RpIntersection *>(pCollisionInfo->GetCondition()->GetIntersection()) );

	RwV3dSubMacro( &sDistaceVector, pAtomicEntity->GetPosition()->GetLocation(), &sDistaceVector );

	fDistance = RwV3dLength( &sDistaceVector );

	sNewResult.SetDistance( fDistance );

	pCollisionInfo->GetResultList()->Insert( &sNewResult );

	return pCollisionTriangle;
}


//*****************************************************************************
//
// End of File : Collision.cpp
//
//*****************************************************************************