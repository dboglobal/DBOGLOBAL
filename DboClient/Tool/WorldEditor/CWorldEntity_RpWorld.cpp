//*****************************************************************************
// File       : CWorldEntity.cpp
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

#include "CEntityFactory.h"
#include "CWorldSectorEntity.h"
#include "CSceneManager.h"

#include "CWorldEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: AddRpAtomic
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::AddRpAtomic( RpAtomic * pAtomic )
{
	Assert( GetRpWorld() != NULL );
	Assert( pAtomic != NULL );

	return reinterpret_cast<RwBool>(RpWorldAddAtomic( GetRpWorld(), pAtomic ));
}


//-----------------------------------------------------------------------------
// Name			: RemoveRpAtomic
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::RemoveRpAtomic( RpAtomic * pAtomic )
{
	Assert( GetRpWorld() != NULL );
	Assert( pAtomic != NULL );

	return reinterpret_cast<RwBool>(RpWorldRemoveAtomic( GetRpWorld(), pAtomic ));
}


//-----------------------------------------------------------------------------
// Name			: AddRpClump
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::AddRpClump( RpClump * pClump )
{
	Assert( GetRpWorld() != NULL );
	Assert( pClump != NULL );

	return reinterpret_cast<RwBool>(RpWorldAddClump( GetRpWorld(), pClump ));
}


//-----------------------------------------------------------------------------
// Name			: RemoveRpClump
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::RemoveRpClump( RpClump * pClump )
{
	Assert( GetRpWorld() != NULL );
	Assert( pClump != NULL );

	return reinterpret_cast<RwBool>(RpWorldRemoveClump( GetRpWorld(), pClump ));
}


//-----------------------------------------------------------------------------
// Name			: GetRpClump
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const RpClump * CWorldEntity::GetRpClump( RwInt32 iClumpIndex ) const
{
	Assert( GetRpWorld() != NULL );
	Assert( iClumpIndex >= 0 );
	Assert( RpWorldGetNumClumps( GetRpWorld() ) < iClumpIndex );

	RwLLLink * pLink = GetLinkFromLinkedList( &(GetRpWorld()->clumpList), iClumpIndex );
	if( pLink == NULL )
		return NULL;

	return GetClumpFromLink( pLink );
}


//-----------------------------------------------------------------------------
// Name			: GetRpClumpCount
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CWorldEntity::GetRpClumpCount( void ) const
{
	Assert( GetRpWorld() != NULL );

	return RpWorldGetNumClumps( GetRpWorld() );
}


//-----------------------------------------------------------------------------
// Name			: AddRpLight
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::AddRpLight( RpLight * pRpLight )
{
	Assert( GetRpWorld() != NULL );
	Assert( pRpLight != NULL );

	return reinterpret_cast<RwBool>(RpWorldAddLight( GetRpWorld(), pRpLight ));
}


//-----------------------------------------------------------------------------
// Name			: RemoveRpLight
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::RemoveRpLight( RpLight * pRpLight )
{
	Assert( GetRpWorld() != NULL );
	Assert( pRpLight != NULL );

	return reinterpret_cast<RwBool>(RpWorldRemoveLight( GetRpWorld(), pRpLight ));
}


//-----------------------------------------------------------------------------
// Name			: AddRwCamera
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::AddRwCamera( RwCamera * pRwCamera )
{
	Assert( GetRpWorld() != NULL );
	Assert( pRwCamera != NULL );

	return reinterpret_cast<RwBool>(RpWorldAddCamera( GetRpWorld(), pRwCamera ));
}


//-----------------------------------------------------------------------------
// Name			: RemoveRwCamera
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::RemoveRwCamera( RwCamera * pRwCamera )
{
	Assert( GetRpWorld() != NULL );
	Assert( pRwCamera != NULL );

	return reinterpret_cast<RwBool>(RpWorldRemoveCamera( GetRpWorld(), pRwCamera ));
}


//-----------------------------------------------------------------------------
// Name			: GetBBox
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const RwBBox * CWorldEntity::GetBBox( void ) const
{
	if( GetRpWorld() == NULL )
	{
		return NULL;
	}

	return RpWorldGetBBox( GetRpWorld() );
}


//-----------------------------------------------------------------------------
// Name			: RenderWorldSector
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::RenderWorldSector( CCameraEntity * pCameraEntity )
{
	RwCamera *			pCamera = (RwCamera *)(pCameraEntity->GetRwInstance());
	RpWorldCameraExt *	pCameraExt = WORLDCAMERAEXTFROMCAMERA( pCamera );
    RpWorldSector **	pFrustumSectors = pCameraExt->frustumSectors;
    RwInt32             iIndex;
	SRwInstanceEx *		pRwInstanceEx;
	CEntity *			pWorldSectorEntity;

	for( iIndex = pCameraExt->position; iIndex; --iIndex )
	{
		pRwInstanceEx = CWorldSectorEntity::GetPlugInDataFromRpWorldSector( *pFrustumSectors );

		pWorldSectorEntity = pRwInstanceEx->GetEntity();

		if( pWorldSectorEntity != NULL )
		{
			if( pWorldSectorEntity->RenderFrame( pCameraEntity ) == FALSE )
			{
				return FALSE;
			}
		}

		++pFrustumSectors;
	}

	return TRUE;
}


//*****************************************************************************
//
// End of File : CWorldEntity_RpWorld.cpp
//
//*****************************************************************************