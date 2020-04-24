//*****************************************************************************
// File       : CEntityResourceManager.cpp
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

#include "CEntityProperty.h"
#include "CAtomicEntityProperty.h"
#include "CClumpEntityProperty.h"
#include "CWorldEntityProperty.h"

#include "CEntityResource.h"
#include "CEditorApplication.h"

#include "CEntityResourceManager.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CEntityResourceManager * CEntityResourceManager::m_pInstance = NULL;


//-----------------------------------------------------------------------------
// Name			: CreateInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityResourceManager::CreateInstance( void )
{
	if( m_pInstance == NULL )
	{
		m_pInstance = NTL_NEW CEntityResourceManager;

		if( m_pInstance != NULL )
		{
			return TRUE;
		}
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: DestroyInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityResourceManager::DestroyInstance( void )
{
	if( m_pInstance != NULL )
	{
		NTL_DELETE( m_pInstance );
		m_pInstance = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityResourceManager * CEntityResourceManager::GetInstance( void )
{
	return m_pInstance;
}


//-----------------------------------------------------------------------------
// Name			: CreateResource
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityResource * CEntityResourceManager::CreateResource( const CEntityProperty * pProperty )
{
	Assert( pProperty != NULL );

	CEntityResource * pResource = NTL_NEW( CEntityResource );
	Assert( pResource != NULL );

	if( pResource->Create( pProperty ) == FALSE )
	{
		DestroyResource( &pResource );

		return NULL;
	}

	return pResource;
}


//-----------------------------------------------------------------------------
// Name			: CreateResource
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityResourceManager::DestroyResource( CEntityResource ** ppResource )
{
	if( ppResource == NULL )
	{
		return;
	}

	CEntityResource * pResource = *ppResource;

	if( ppResource == NULL )
	{
		return;
	}

	NTL_DELETE( pResource );
	pResource = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: WorldImportPartitionBuildCB
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwReal WorldImportPartitionBuildCB( RtWorldImportBuildSector * pBuildSector, RtWorldImportBuildStatus * pBuildStatus, RtWorldImportPartition * pPartition, void * pData ) 
{
	SSectorCreateInfo * pSectorCreateInfo = (SSectorCreateInfo *)pData;

	RwV3d			SectorSize;

	SectorSize.x	= pBuildSector->boundingBox.sup.x - pBuildSector->boundingBox.inf.x;
	SectorSize.y	= pBuildSector->boundingBox.sup.y - pBuildSector->boundingBox.inf.y;
	SectorSize.z	= pBuildSector->boundingBox.sup.z - pBuildSector->boundingBox.inf.z;

	SLocation3DInt	SectorDivision;

	SectorDivision.SetX( SectorSize.x / pSectorCreateInfo->GetSectorSize()->x );
	SectorDivision.SetY( SectorSize.y / pSectorCreateInfo->GetSectorSize()->y );
	SectorDivision.SetZ( SectorSize.z / pSectorCreateInfo->GetSectorSize()->z );

	if( SectorDivision.GetX() >= SectorDivision.GetZ() )
	{
		if( SectorDivision.GetX() >= SectorDivision.GetY() )
		{
			pPartition->type	= RP_SECTOR_TYPE_PLANE_X;
			pPartition->value	= (pBuildSector->boundingBox.sup.x - pBuildSector->boundingBox.inf.x) * 0.5f + pBuildSector->boundingBox.inf.x;
		}
		else
		{
			pPartition->type	= RP_SECTOR_TYPE_PLANE_Y;
			pPartition->value	= (pBuildSector->boundingBox.sup.y - pBuildSector->boundingBox.inf.y) * 0.5f + pBuildSector->boundingBox.inf.y;
		}
	}
	else
	{
		if( SectorDivision.GetZ() >= SectorDivision.GetY() )
		{
			pPartition->type	= RP_SECTOR_TYPE_PLANE_Z;
			pPartition->value	= (pBuildSector->boundingBox.sup.z - pBuildSector->boundingBox.inf.z) * 0.5f + pBuildSector->boundingBox.inf.z;
		}
		else
		{
			pPartition->type	= RP_SECTOR_TYPE_PLANE_Y;
			pPartition->value	= (pBuildSector->boundingBox.sup.y - pBuildSector->boundingBox.inf.y) * 0.5f + pBuildSector->boundingBox.inf.y;
		}
	}

	RtWorldImportSetPartitionStatistics( pBuildSector, pPartition );

	return 0.0f;
}


//-----------------------------------------------------------------------------
// Name			: WorldImportPartitionTerminateCB
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool WorldImportPartitionTerminateCB( RtWorldImportBuildSector * pBuildSector, RtWorldImportBuildStatus * pBuildStatus, void * pData )
{
	SSectorCreateInfo * pSectorCreateInfo = (SSectorCreateInfo *)pData;

	if( ( pBuildSector->boundingBox.sup.x - pBuildSector->boundingBox.inf.x ) <= pSectorCreateInfo->GetSectorSize()->x &&
		( pBuildSector->boundingBox.sup.y - pBuildSector->boundingBox.inf.y ) <= pSectorCreateInfo->GetSectorSize()->y &&
		( pBuildSector->boundingBox.sup.z - pBuildSector->boundingBox.inf.z ) <= pSectorCreateInfo->GetSectorSize()->z )
	{
		return TRUE;
	}

	return FALSE;
}


//*****************************************************************************
//
// End of File : CEntityResourceManager.cpp
//
//*****************************************************************************