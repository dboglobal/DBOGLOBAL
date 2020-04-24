//*****************************************************************************
// File       : CEntityCubeContainer.cpp
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

#include "CSceneManager.h"
#include "CEntityFactory.h"

#include "CEntityCubeContainer.h"


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

void CEntityCubeContainer::Initialize( void )
{
	m_Size.Set( 0, 0, 0 );

	m_iSizeXZ	= 0;
	m_iSizeXYZ	= 0;

	m_ppppMap	= NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCubeContainer::Create( SEntityContainerProperty * pProperty )
{
	BEGIN_FUNCTION( "CEntityCubeContainer::Create" );

	if( pProperty == NULL )
	{
		END_FUNCTION( FALSE );
	}

	Assert( pProperty->IsValid() != FALSE );

	if( m_ppppMap != NULL )
	{
		Destroy();
	}

	m_Property.Assign( pProperty );

	m_Size.Size( GetMax(), GetMin() );

	if( m_Size.GetX() <= 0 || m_Size.GetY() <= 0 || m_Size.GetZ() <= 0 )
	{
		END_FUNCTION( FALSE );
	}

	m_iSizeXZ	= m_Size.GetX() * m_Size.GetZ();
	m_iSizeXYZ	= m_iSizeXZ * m_Size.GetY();

	m_ppppMap = NTL_NEW CEntity ***[m_Size.GetX()];

	SLocation3DInt					Location;

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		m_ppppMap[iIndexX] = NTL_NEW CEntity **[m_Size.GetZ()];

		for( RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			m_ppppMap[iIndexX][iIndexZ] = NTL_NEW CEntity *[m_Size.GetY()];

			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				m_ppppMap[iIndexX][iIndexZ][iIndexY] = NULL;
			}
		}
	}

	m_iCount = m_iSizeXYZ;

	END_FUNCTION( TRUE );
}


//-----------------------------------------------------------------------------
// Name			: CreateSector
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCubeContainer::CreateWorldSector( void )
{
	BEGIN_FUNCTION( "CEntityCubeContainer::CreateWorldSector" );

	SLocation3DInt					SectorIndex;
	CWorldSectorEntityProperty		WorldSectorProperty;
	SEntityCreateProperty			CreateProperty;

	CreateProperty.SetParentID( m_Property.GetParent()->GetUniqueID() );
	CreateProperty.SetCategory( E_ENTITY_CATEGORY_EXTERNAL );
	CreateProperty.SetBase( FALSE );

	WorldSectorProperty.SetSectorContainer( this );

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for(  RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				CreateProperty.SetUniqueID( ENTITY_UNIQUE_ID_AUTO );

				SectorIndex.Set( GetMin()->GetX() + iIndexX, GetMin()->GetY() + iIndexY, GetMin()->GetZ() + iIndexZ );

				WorldSectorProperty.SetSectorIndex( &SectorIndex );

				CEntity *	pResultEntity = g_pSceneManager->CreateEntity( &WorldSectorProperty, &CreateProperty );

				if( pResultEntity == NULL || m_ppppMap[iIndexX][iIndexZ][iIndexY] != pResultEntity )
				{
					END_FUNCTION( FALSE );
				}
			}
		}
	}

	END_FUNCTION( TRUE );
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityCubeContainer::Destroy( void )
{
	BEGIN_FUNCTION( "CEntityCubeContainer::Destroy" );

	if( m_ppppMap != NULL )
	{
		RemoveAll( TRUE );
	}

	Initialize();

	m_iCount = 0;

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name       : RemoveAll
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

void CEntityCubeContainer::RemoveAll( RwBool bDeleteAll )
{
	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for( RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for( RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					CEntityContainer::DestroyEntity( m_ppppMap[iIndexX][iIndexZ][iIndexY] );
				}
			}
		}
	}

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		if( m_ppppMap[iIndexX] != NULL )
		{
			for( RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
			{
				if( m_ppppMap[iIndexX][iIndexZ] != NULL )
				{
					NTL_ARRAY_DELETE( m_ppppMap[iIndexX][iIndexZ] );
					m_ppppMap[iIndexX][iIndexZ] = NULL;
				}
			}

			NTL_ARRAY_DELETE( m_ppppMap[iIndexX] );
			m_ppppMap[iIndexX] = NULL;
		}
	}

	NTL_ARRAY_DELETE( m_ppppMap );
	m_ppppMap = NULL;
	m_iCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: UpdateFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCubeContainer::UpdateFrame( void )
{
	RwBool		bResult = TRUE;

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for(  RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					if( m_ppppMap[iIndexX][iIndexZ][iIndexY]->UpdateFrame() == FALSE )
					{
						bResult = FALSE;
					}
				}
			}
		}
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCubeContainer::RenderFrame( CEntity * pRenderer )
{
	RwBool		bResult = TRUE;

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for(  RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					if( m_ppppMap[iIndexX][iIndexZ][iIndexY]->RenderFrame( pRenderer ) == FALSE )
					{
						bResult = FALSE;
					}
				}
			}
		}
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: Select
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityCubeContainer::Select( void )
{
	RwUInt32	iSelectedCount = 0;

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for(  RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					iSelectedCount += m_ppppMap[iIndexX][iIndexZ][iIndexY]->Select();
				}
			}
		}
	}

	return iSelectedCount;
}


//-----------------------------------------------------------------------------
// Name			: Unselect
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityCubeContainer::Unselect( void )
{
	RwUInt32	iUnselectedCount = 0;

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for(  RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					iUnselectedCount += m_ppppMap[iIndexX][iIndexZ][iIndexY]->Unselect();
				}
			}
		}
	}

	return iUnselectedCount;
}



//-----------------------------------------------------------------------------
// Name			: StartAnimation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityCubeContainer::StartAnimation( RwReal fTime )
{
	RwUInt32	iSelectedCount = 0;

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for(  RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					iSelectedCount += m_ppppMap[iIndexX][iIndexZ][iIndexY]->StartAnimation( fTime );
				}
			}
		}
	}

	return iSelectedCount;
}


//-----------------------------------------------------------------------------
// Name			: StopAnimation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityCubeContainer::StopAnimation( RwReal fTime )
{
	RwUInt32	iSelectedCount = 0;

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for(  RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					iSelectedCount += m_ppppMap[iIndexX][iIndexZ][iIndexY]->StopAnimation( fTime );
				}
			}
		}
	}

	return iSelectedCount;
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CEntityCubeContainer::GetWorldSector( RwInt32 iIndexX, RwInt32 iIndexY, RwInt32 iIndexZ ) const
{
	if( iIndexX < GetMin()->GetX() || iIndexX > GetMax()->GetX() )
	{
		return NULL;
	}

	if( iIndexZ < GetMin()->GetZ() || iIndexZ > GetMax()->GetZ() )
	{
		return NULL;
	}

	if( iIndexY < GetMin()->GetY() || iIndexY > GetMax()->GetY() )
	{
		return NULL;
	}

	return m_ppppMap[iIndexX-GetMin()->GetX()][iIndexZ-GetMin()->GetZ()][iIndexY-GetMin()->GetY()];
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CEntityCubeContainer::GetWorldSector( RwInt32 iIndexX, RwInt32 iIndexZ ) const
{
	return GetWorldSector( iIndexX, 0, iIndexZ );
}


//-----------------------------------------------------------------------------
// Name			: GetWorldSector
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CEntityCubeContainer::GetWorldSector( SLocation3DInt * pSectorIndex ) const
{
	Assert( pSectorIndex != NULL );

	return GetWorldSector( pSectorIndex->GetX(), pSectorIndex->GetY(), pSectorIndex->GetZ() );
}


//-----------------------------------------------------------------------------
// Name			: SetFlag
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityCubeContainer::SetFlag( RwUInt32 iFlag, RwBool bApplyToChild )
{
	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for(  RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					m_ppppMap[iIndexX][iIndexZ][iIndexY]->SetFlag( iFlag, bApplyToChild );
				}
			}
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityCubeContainer::SetState( EEntityState eState, RwBool bApplyToChild )
{
	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for(  RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					m_ppppMap[iIndexX][iIndexZ][iIndexY]->SetState( eState, bApplyToChild );
				}
			}
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name       : SetPosition
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

void CEntityCubeContainer::SetPosition( const SPosition3D * pPosition, RwBool bAdded, RwBool bApplyToChild )
{
	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for(  RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					m_ppppMap[iIndexX][iIndexZ][iIndexY]->SetPosition( pPosition, bAdded, bApplyToChild );
				}
			}
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name       : SetPosition
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

void CEntityCubeContainer::SetScale( const SLocation3D * pScale, RwBool bAdded, RwBool bApplyToChild )
{
	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for(  RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					m_ppppMap[iIndexX][iIndexZ][iIndexY]->SetScale( pScale, bAdded, bApplyToChild );
				}
			}
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CEntityCubeContainer::GetEntity( RwUInt32 iUniqueID )
{
	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for( RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for( RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					if( m_ppppMap[iIndexX][iIndexZ][iIndexY]->GetUniqueID() == iUniqueID )
					{
						return m_ppppMap[iIndexX][iIndexZ][iIndexY];
					}
				}
			}
		}
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: GetAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CEntityCubeContainer::GetAt( RwUInt32 iOrder )
{
	SLocation3DInt		sOffset;

	sOffset.SetY( iOrder % m_Size.GetY() );
	iOrder /= m_Size.GetY();

	sOffset.SetZ( iOrder % m_Size.GetZ() );
	sOffset.SetX( iOrder / m_Size.GetZ() );

	return m_ppppMap[sOffset.GetX()][sOffset.GetZ()][sOffset.GetY()];
}


//-----------------------------------------------------------------------------
// Name			: SearchByRwInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CEntityCubeContainer::SearchByRwInstance( void * pRwInstance )
{
	CEntity *		pMatchEntity;

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for( RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for( RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					pMatchEntity = m_ppppMap[iIndexX][iIndexZ][iIndexY]->SearchByRwInstance( pRwInstance );
					if( pMatchEntity != NULL )
					{
						return pMatchEntity;
					}
				}
			}
		}
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: SearchEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityCubeContainer::SearchEntity( CEntityContainer * pResultContainer, const SEntitySearchCondition * pSearchCondition )
{
	if( m_ppppMap == NULL )
	{
		return 0;
	}

	RwUInt32		iCount = pResultContainer->GetCount();

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for( RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for( RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					m_ppppMap[iIndexX][iIndexZ][iIndexY]->SearchEntity( pResultContainer, pSearchCondition );
				}
			}
		}
	}

	return pResultContainer->GetCount() - iCount;
}


//-----------------------------------------------------------------------------
// Name			: SearchByLocation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityCubeContainer::SearchByLocation( const SLocation3D * pSearchLocation, CEntityContainer * pResultList, EEntityType eEntityType )
{
	RwUInt32		iMatchCount = 0;

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for( RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for( RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					iMatchCount += m_ppppMap[iIndexX][iIndexZ][iIndexY]->SearchByLocation( pSearchLocation, pResultList, eEntityType );
				}
			}
		}
	}

	return iMatchCount;
}


//-----------------------------------------------------------------------------
// Name			: Insert
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCubeContainer::Insert( CEntity * pChild )
{
	Assert( pChild != NULL );

	const CEntity *		pWorldEntity = GetParent();
	Assert( pWorldEntity != NULL );
	Assert( pWorldEntity->GetType() == E_ENTITY_TYPE_WORLD );

	RpWorld *		pWorld = reinterpret_cast<RpWorld *>(pWorldEntity->GetRwInstance());
	Assert( pWorld != NULL );

	RpSector *		pRootSector = pWorld->rootSector;
	Assert( pRootSector != NULL );

	SLocation3DInt		Offset( 0, 0, 0 );
	SLocation3DInt		Size;

	GetSize( &Size );

	GetRpWorldSectorIndexByPositionFB( pRootSector, pChild->GetPosition()->GetLocation()->GetConstRwV3d(), &Offset, &Size );

	CEntity * pWorldSectorEntity = m_ppppMap[Offset.GetX()][Offset.GetZ()][Offset.GetY()];

	if( pChild->GetType() == E_ENTITY_TYPE_WORLD_SECTOR )
	{
		Assert( pWorldSectorEntity == NULL );

		m_ppppMap[Offset.GetX()][Offset.GetZ()][Offset.GetY()] = pChild;

		pChild->SetParentForce( const_cast<CEntity *>(pWorldEntity) );
	}
	else
	{
		//Assert( !"Can't Attached" );

		if( pWorldSectorEntity == NULL )
		{
			pChild->SetGlobal( TRUE );

			pChild->SetParentForce( const_cast<CEntity *>(pWorldEntity) );

			return (const_cast<CEntity *>(pWorldEntity))->Attach( pChild );
		}
		else
		{
			pChild->SetParentForce( pWorldSectorEntity );

			return pWorldSectorEntity->Attach( pChild );
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Remove
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCubeContainer::Remove( CEntity * pChild )
{
	Assert( pChild != NULL );

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for( RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for( RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] == pChild )
				{
					m_ppppMap[iIndexX][iIndexZ][iIndexY] = NULL;

					return TRUE;
				}
			}
		}
	}

	SEntityContainerProperty	ResultListProperty( E_ENTITY_CONTAINER_TYPE_LIST, FALSE );

	CEntityContainer * pResultList = g_pEntityFactory->CreateEntityContainer( &ResultListProperty );

	if( SearchByLocation( pChild->GetPosition()->GetLocation(), pResultList, E_ENTITY_TYPE_WORLD_SECTOR ) == 0 )
	{
		g_pEntityFactory->DestroyEntityContainer( &pResultList );

		return FALSE;
	}

	CEntity *		pWorldSector = pResultList->GetAt( 0 );
	Assert( pWorldSector != NULL );

	RwBool			bResult = pWorldSector->Detach( pChild );
	if( bResult )
	{
		--m_iCount;
	}

	g_pEntityFactory->DestroyEntityContainer( &pResultList );

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: CalculateVertexColor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCubeContainer::CalculateVertexColor( SVertexColorGenerationInfo * pInfo )
{
	RwBool		bResult = TRUE;

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for( RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for( RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					if( m_ppppMap[iIndexX][iIndexZ][iIndexY]->CalculateVertexColor( pInfo ) == FALSE )
					{
						bResult = FALSE;
					}
				}
			}
		}
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: DetectCollision
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCubeContainer::DetectCollision( CCollisionDetectionInfo * pCollisionInfo )
{
	RwBool		bResult = TRUE;

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for( RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for( RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					if( m_ppppMap[iIndexX][iIndexZ][iIndexY]->DetectCollision( pCollisionInfo ) == FALSE )
					{
						bResult = FALSE;
					}
				}
			}
		}
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CEntityCubeContainer::Export( CFixedStringList * pStringList, CFixedString * pName )
{
	RwInt32				iTotalExportCount	= 0;
	RwInt32				iSectorExportCount	= 0;

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for(  RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					iSectorExportCount = m_ppppMap[iIndexX][iIndexZ][iIndexY]->Export( pStringList, pName );

					if( iSectorExportCount < 0 )
					{
						return iSectorExportCount;
					}

					iTotalExportCount += iSectorExportCount;
				}
			}
		}
	}

	return iTotalExportCount;
}


//-----------------------------------------------------------------------------
// Name			: ReportSelfState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCubeContainer::ReportSelfState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	RwChar		szBuffer[256];
	rwsprintf( szBuffer, RWSTRING( "Count : %d, ( X : %d, Y : %d  Z : %d )" ),
				m_iCount, m_Size.GetX(), m_Size.GetY(), m_Size.GetZ() );

	pLogMessageManager->SetLastLogMessageWithTab( szBuffer, iTab );

	// m_pProperty->ReportState( pLogMessageManager, iTab + 1 );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ReportChildState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCubeContainer::ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	if( m_ppppMap == NULL )
	{
		return FALSE;
	}

	RwBool		bResult = TRUE;

	for( RwInt32 iIndexX = 0; iIndexX < m_Size.GetX(); ++iIndexX )
	{
		for(  RwInt32 iIndexZ = 0; iIndexZ < m_Size.GetZ(); ++iIndexZ )
		{
			for(  RwInt32 iIndexY = 0; iIndexY < m_Size.GetY(); ++iIndexY )
			{
				if( m_ppppMap[iIndexX][iIndexZ][iIndexY] != NULL )
				{
					if( m_ppppMap[iIndexX][iIndexZ][iIndexY]->ReportState( pLogMessageManager, iTab + 1 ) == FALSE )
					{
						bResult = FALSE;
					}
				}
                else
				{
					bResult = FALSE;
				}
			}
		}
	}

	return bResult;
}


//*****************************************************************************
//
// End of File : CEntityCubeContainer.cpp
//
//*****************************************************************************