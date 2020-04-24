//*****************************************************************************
// File       : CSceneManager_Collision.cpp
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

#include "CEntityCollision.h"
#include "CEntityCollisionList.h"
#include "Collision.h"

#include "CCameraEntity.h"
#include "CWorldEntity.h"
#include "CEntityContainer.h"
#include "CEntityFactory.h"

#include "CSceneManager.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: DetectCollision
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::DetectCollision( CCollisionDetectionInfo * pCollisionInfo ) const
{
	RwLine			CameraRay;

	if( GetActiveCamera() == NULL )
	{
		return FALSE;
	}

	if( pCollisionInfo->GetCondition()->GetUseScreenPos() == true )
	{
		GetActiveCamera()->GetRay( const_cast<RwV2d *>(pCollisionInfo->GetCondition()->GetScreenPos()), &CameraRay );

		pCollisionInfo->GetCondition()->SetIntersection( rpINTERSECTLINE, &CameraRay );
	}
	
	return m_pWorldList->DetectCollision( pCollisionInfo );
}


//-----------------------------------------------------------------------------
// Name			: GetHeight
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::GetHeight( SLocation3D * pLocation, RwBool bUseHeightMap ) const
{
	if( m_pWorldList == NULL )
	{
		return FALSE;
	}

	SEntityContainerProperty	sWorldListProperty( E_ENTITY_CONTAINER_TYPE_LIST, FALSE );
	CEntityContainer *			pWorldList = g_pEntityFactory->CreateEntityContainer( &sWorldListProperty );
	RwUInt32					iWorldCount = m_pWorldList->SearchByLocation( pLocation, pWorldList, E_ENTITY_TYPE_WORLD );

	if( iWorldCount == 0 )
	{
		g_pEntityFactory->DestroyEntityContainer( &pWorldList );

		return FALSE;
	}

	CWorldEntity *	pWorldEntity;
	RwBool			bDetected = FALSE;
	SLocation3D		sResultPostion( *pLocation );
	sResultPostion.SetY( -RwRealMAXVAL );
	SLocation3D		sTestPostion( *pLocation );

	for( RwUInt32 iWorldIndex = 0; iWorldIndex < iWorldCount; ++iWorldIndex )
	{
		pWorldEntity = reinterpret_cast<CWorldEntity *>(pWorldList->GetAt( iWorldIndex ));
		Assert( pWorldEntity != NULL );
		Assert( pWorldEntity->GetType() == E_ENTITY_TYPE_WORLD );

		sTestPostion.SetY( pLocation->GetY() );

		if( pWorldEntity->GetHeight( &sTestPostion, bUseHeightMap ) != FALSE )
		{			
			if( sResultPostion.GetY() < sTestPostion.GetY() )
			{
				sResultPostion.SetY( sTestPostion.GetY() );

				bDetected = TRUE;
			}
		}
	}

	g_pEntityFactory->DestroyEntityContainer( &pWorldList );

	if( bDetected != FALSE )
	{
		pLocation->SetY( sResultPostion.GetY() );
	}

	return bDetected;
}


//-----------------------------------------------------------------------------
// Name			: CreateHeightMap
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::CreateHeightMap( RwReal fPrecision, RwBool bLoadFromFile )
{
	if( m_pWorldList == NULL )
	{
		return FALSE;
	}

	CWorldEntity *		pWorldEntity;

	for( RwUInt32 iIndex = 0; iIndex < m_pWorldList->GetCount(); ++iIndex )
	{
		pWorldEntity = reinterpret_cast<CWorldEntity *>(m_pWorldList->GetAt( iIndex ));
		Assert( pWorldEntity != NULL );
		Assert( pWorldEntity->GetType() == E_ENTITY_TYPE_WORLD );

		if( pWorldEntity->CreateHeightMap( fPrecision, bLoadFromFile ) == FALSE )
		{
			return FALSE;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Convert2DTo3D
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::Convert2DTo3D( RwV2d * pLocation2D, SLocation3D * pLocation3D )
{
	CCollisionDetectionInfo		sCollisionInfo;

	sCollisionInfo.CreateByPickTerrain( pLocation2D );

	if( g_pSceneManager->DetectCollision( &sCollisionInfo ) == FALSE )
	{
		return Convert2DTo3DForce( pLocation2D, pLocation3D );
	}

	CEntityCollisionList *		pResultList = sCollisionInfo.GetResultList();
	CEntityCollision *			pEntityCollision;
	CEntity *					pEntity;

	while( pResultList->IsEmpty() == FALSE )
	{
		pEntityCollision = pResultList->GetCollisionEntityByOrder( 0 );
		Assert( pEntityCollision != NULL );

		pResultList->Remove( pEntityCollision, FALSE );

		pEntity = pEntityCollision->GetEntity();

		Assert( pEntity != NULL );

		if( pEntity != NULL )
		{
			pEntity = pEntity->GetParent();
		}

		if( pEntity != NULL )
		{
			pLocation3D->Set( *(pEntityCollision->GetLocation()) );

			return TRUE;
		}
	}

	return Convert2DTo3DForce( pLocation2D, pLocation3D );
}


//-----------------------------------------------------------------------------
// Name			: Convert2DTo3DForce
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::Convert2DTo3DForce( RwV2d * pLocation2D, SLocation3D * pLocation3D )
{
	RwLine			CameraRay;

	GetActiveCamera()->GetRay( pLocation2D, &CameraRay );

	pLocation3D->SetX( ( CameraRay.end.x * CameraRay.start.y - CameraRay.start.x * CameraRay.end.y ) / ( CameraRay.start.y - CameraRay.end.y ) );
	pLocation3D->SetY( 0.0f );
	pLocation3D->SetZ( ( CameraRay.end.z * CameraRay.start.y - CameraRay.start.z * CameraRay.end.y ) / ( CameraRay.start.y - CameraRay.end.y ) );

	return FALSE;
}


//*****************************************************************************
//
// End of File : CSceneManager_Collision.cpp
//
//*****************************************************************************
