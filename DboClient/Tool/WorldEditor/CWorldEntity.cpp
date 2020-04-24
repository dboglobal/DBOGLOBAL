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

#include "CFixedString.h"
#include "CFixedStringList.h"

#include "CEntityCubeContainer.h"
#include "CEntityContainer.h"
#include "CEntityFactory.h"
#include "CEntityResource.h"
#include "CEntityResourceManager.h"
#include "CEntityCubeContainer.h"
#include "CRenderManager.h"
#include "CSceneManager.h"

#include "Collision.h"

#include "CCameraEntity.h"
#include "CLightEntity.h"

#include "CWorldEntity.h"


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

void CWorldEntity::Initialize( void )
{
	CEntity::Initialize();

	m_pWorldSectorMap = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty )
{
	BEGIN_FUNCTION( "CWorldEntity::Create" );

	Assert( pEntityProperty != NULL );
	Assert( pEntityProperty->GetType() == E_ENTITY_TYPE_WORLD );

	END_FUNCTION( CEntity::Create( pEntityProperty, pCreateProperty ) );
}


//-----------------------------------------------------------------------------
// Name			: PostCreate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::PostCreate( void )
{
	BEGIN_FUNCTION( "CWorldEntity::PostCreate" );

	if( GetWorldProperty()->IsEmpty() != FALSE )
	{
		CEntity::PostCreate();

		END_FUNCTION( TRUE );
	}

	if( GetWorldProperty()->IsCustomBuild() != FALSE )
	{
		if( CreateWorldSectorMap( GetWorldProperty() ) == FALSE )
		{
			END_FUNCTION( FALSE );
		}
	}

	END_FUNCTION( CEntity::PostCreate() );
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldEntity::Destroy( void )
{
	BEGIN_FUNCTION( "CWorldEntity::Destroy" );

	CEntity::Destroy();

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: PreDestroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldEntity::PreDestroy( void )
{
	DestroyWorldSectorMap();

	CEntity::PreDestroy();

	return;
}


//-----------------------------------------------------------------------------
// Name			: UpdateFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::UpdateFrame( void )
{
	BEGIN_FUNCTION( "CWorldEntity::UpdateFrame" );

	if( m_pWorldSectorMap != NULL )
	{
		if( m_pWorldSectorMap->UpdateFrame() == FALSE )
		{
			END_FUNCTION( FALSE );
		}
	}

	END_FUNCTION( CEntity::UpdateFrame() );
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::RenderFrame( CEntity * pRenderer )
{
	BEGIN_FUNCTION( "CWorldEntity::RenderFrame" );

	if( pRenderer == NULL )
	{
		END_FUNCTION( FALSE );
	}

	if( pRenderer->GetFrameID() <= GetFrameID() )
	{
		END_FUNCTION( TRUE );
	}
	else
	{
		SetFrameID( pRenderer->GetFrameID() );
	}

	if( IsVisible() == FALSE )
	{
		END_FUNCTION( TRUE );
	}

	if( GetRpWorld() == NULL )
	{
		END_FUNCTION( FALSE );
	}

	if( pRenderer == NULL || pRenderer->GetType() != E_ENTITY_TYPE_CAMERA )
	{
		END_FUNCTION( FALSE );
	}

	CCameraEntity *	pCameraEntity = reinterpret_cast<CCameraEntity *>(pRenderer);

	pCameraEntity->SetParent( this );
	pCameraEntity->BeginUpdate();

	RwBool			bResult;

	// Render Global External Object

	bResult = CEntity::RenderFrame( pRenderer );

	// Render Internal & Normal Object

	RwD3D9SetRenderState( D3DRS_ZENABLE, TRUE );
	RwRenderStateSet( rwRENDERSTATEFOGENABLE, (void *)TRUE );
	RwRenderStateSet( rwRENDERSTATECULLMODE, (void *)rwCULLMODECULLNONE );
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0xD0 );
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONGREATEREQUAL );

	RenderWorldSector( pCameraEntity );

	g_pSceneManager->GetRenderManager()->RenderPrimitive();

	pCameraEntity->EndUpdate();
	pCameraEntity->SetParent( NULL );

	g_pSceneManager->IncreaseRenderCountWorld();

	END_FUNCTION( bResult );
}


//-----------------------------------------------------------------------------
// Name			: Select
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CWorldEntity::Select( void )
{
	RwUInt32	iSelectedCount = CEntity::Select();

	if( m_pWorldSectorMap != NULL )
	{
		iSelectedCount += m_pWorldSectorMap->Select();
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

RwUInt32 CWorldEntity::Unselect( void )
{
	RwUInt32	iUnselectedCount = CEntity::Unselect();

	if( m_pWorldSectorMap != NULL )
	{
		iUnselectedCount += m_pWorldSectorMap->Unselect();
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

RwUInt32 CWorldEntity::StartAnimation( RwReal fTime )
{
	RwUInt32	iSelectedCount = CEntity::StartAnimation( fTime );

	if( m_pWorldSectorMap != NULL )
	{
		iSelectedCount += m_pWorldSectorMap->StartAnimation( fTime );
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

RwUInt32 CWorldEntity::StopAnimation( RwReal fTime )
{
	RwUInt32	iSelectedCount = CEntity::StopAnimation( fTime );

	if( m_pWorldSectorMap != NULL )
	{
		iSelectedCount += m_pWorldSectorMap->StopAnimation( fTime );
	}

	return iSelectedCount;
}


//-----------------------------------------------------------------------------
// Name			: Attach
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::Attach( CEntity * pChild )
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
		case E_ENTITY_TYPE_ATOMIC:
		{
			if( AddRpAtomic( reinterpret_cast<RpAtomic *>(pChild->GetRwInstance()) ) == FALSE )
			{
				Assert( !"AddRwAtomic() Fail" );

				return FALSE;
			}

			break;
		}

		case E_ENTITY_TYPE_CLUMP:
		{
			if( AddRpClump( reinterpret_cast<RpClump *>(pChild->GetRwInstance()) ) == FALSE )
			{
				Assert( !"AddRwClump() Fail" );

				return FALSE;
			}

			break;
		}

		case E_ENTITY_TYPE_LIGHT:
		{
			if( AddRpLight( reinterpret_cast<RpLight *>(pChild->GetRwInstance()) ) == FALSE )
			{
				Assert( !"RpWorldAddLight() Fail" );

				return FALSE;
			}

			break;
		}

		case E_ENTITY_TYPE_CAMERA:
		{
			if( AddRwCamera( reinterpret_cast<RwCamera *>(pChild->GetRwInstance()) ) == FALSE )
			{
				Assert( !"RpWorldAddCamera() Fail" );

				return FALSE;
			}

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

RwBool CWorldEntity::Detach( CEntity * pChild )
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
		case E_ENTITY_TYPE_ATOMIC:
		{
			RemoveRpAtomic( reinterpret_cast<RpAtomic *>(pChild->GetRwInstance()) );

			break;
		}

		case E_ENTITY_TYPE_CLUMP:
		{
			RemoveRpClump( reinterpret_cast<RpClump *>(pChild->GetRwInstance()) );

			break;
		}

		case E_ENTITY_TYPE_LIGHT:
		{
			RemoveRpLight( reinterpret_cast<RpLight *>(pChild->GetRwInstance()) );

			break;
		}

		case E_ENTITY_TYPE_CAMERA:
		{
			RemoveRwCamera( reinterpret_cast<RwCamera *>(pChild->GetRwInstance()) );

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
// Name			: InsertChild
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::InsertChild( CEntity * pChild )
{
	Assert( pChild != NULL );

	if( pChild->IsGlobal() == FALSE && m_pWorldSectorMap != NULL )
	{
		return m_pWorldSectorMap->Insert( pChild );
	}

	return CEntity::InsertChild( pChild );
}


//-----------------------------------------------------------------------------
// Name			: RemoveChild
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::RemoveChild( CEntity * pChild )
{
	Assert( pChild != NULL );

	if( pChild->IsGlobal() == FALSE && m_pWorldSectorMap != NULL )
	{
		return m_pWorldSectorMap->Remove( pChild );
	}

	return CEntity::RemoveChild( pChild );
}


//-----------------------------------------------------------------------------
// Name			: SetFlag
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldEntity::SetFlag( RwUInt32 iFlag, RwBool bApplyToChild )
{
	CEntity::SetFlag( iFlag, bApplyToChild );

	if( bApplyToChild != FALSE && m_pWorldSectorMap != NULL )
	{
		m_pWorldSectorMap->SetFlag( iFlag, bApplyToChild );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: SearchByLocation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CWorldEntity::SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultList, EEntityType eEntityType ) const
{
	RwUInt32		iMatchCount = 0;

	if( eEntityType == GetType() )
	{
		if( IsPointInBounding( pLocation ) != FALSE )
		{
			++iMatchCount;

			pResultList->Insert( const_cast<CEntity *>(reinterpret_cast<const CEntity *>(this)) );
		}
	}

	if( m_pWorldSectorMap != NULL )
	{
		iMatchCount += m_pWorldSectorMap->SearchByLocation( pLocation, pResultList, eEntityType );
	}

	iMatchCount += CEntity::SearchByLocation( pLocation, pResultList, eEntityType );

	return iMatchCount;
}


//-----------------------------------------------------------------------------
// Name			: SearchByRwInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CWorldEntity::SearchByRwInstance( void * pRwInstance ) const
{
	if( GetRwInstance() == pRwInstance )
	{
		return const_cast<CEntity *>(reinterpret_cast<const CEntity *>(this));
	}

	CEntity *		pMatchEntity;

	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
	{
		if( m_pChildren[iCategoryIndex] != NULL )
		{
			pMatchEntity = m_pChildren[iCategoryIndex]->SearchByRwInstance( pRwInstance );

			if( pMatchEntity != NULL )
			{
				return pMatchEntity;
			}
		}
	}

	if( m_pWorldSectorMap != NULL )
	{
		return m_pWorldSectorMap->SearchByRwInstance( pRwInstance );
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: IsPointInBounding
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::IsPointInBounding( const SLocation3D * pLocation  ) const
{
	return IsPointInBBox( pLocation->GetConstRwV3d(), GetBBox() );
}


//-----------------------------------------------------------------------------
// Name			: DetectCollision
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::DetectCollision( CCollisionDetectionInfo * pCollisionInfo ) const
{
	RwBool		bResult = TRUE;

	RpWorldForAllAtomicIntersections( GetRpWorld(), const_cast<RpIntersection *>(pCollisionInfo->GetCondition()->GetIntersection()), DetectAtomicInWorldCB, pCollisionInfo );

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: CreateWorldSectorMap
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::CreateWorldSectorMap( const CWorldEntityProperty * pWorldProperty )
{
	if( m_pWorldSectorMap != NULL )
	{
		DestroyWorldSectorMap();
	}

	SEntityContainerProperty	ContainerProperty( E_ENTITY_CONTAINER_TYPE_CUBE, TRUE, this, g_pSceneManager );

	ContainerProperty.SetParent( this );
	ContainerProperty.Min.Set( pWorldProperty->GetSectorMin() );
	ContainerProperty.Max.Set( pWorldProperty->GetSectorMax() );

	m_pWorldSectorMap = reinterpret_cast<CEntityCubeContainer *>(g_pEntityFactory->CreateEntityContainer( &ContainerProperty ));
	if( m_pWorldSectorMap == NULL )
	{
		return FALSE;
	}

	return GetWorldSectorMap()->CreateWorldSector();
}


//-----------------------------------------------------------------------------
// Name			: DestroyWorldSectorMap
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldEntity::DestroyWorldSectorMap( void )
{
	if( m_pWorldSectorMap != NULL )
	{
		NTL_DELETE( m_pWorldSectorMap );
		m_pWorldSectorMap = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetWorldSector
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CWorldEntity::GetWorldSector( RwInt32 iIndexX, RwInt32 iIndexY, RwInt32 iIndexZ ) const
{
	if( m_pWorldSectorMap == NULL )
	{
		return NULL;
	}

	return m_pWorldSectorMap->GetWorldSector( iIndexX, iIndexY, iIndexZ );
}


//-----------------------------------------------------------------------------
// Name			: GetWorldSector
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CWorldEntity::GetWorldSector( SLocation3DInt * pLocation ) const
{
	if( m_pWorldSectorMap == NULL )
	{
		return NULL;
	}

	return m_pWorldSectorMap->GetWorldSector( pLocation );
}


//-----------------------------------------------------------------------------
// Name			: GetWorldSector
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CWorldEntity::GetWorldSector( RwInt32 iIndexX, RwInt32 iIndexZ ) const
{
	if( m_pWorldSectorMap == NULL )
	{
		return NULL;
	}

	return m_pWorldSectorMap->GetWorldSector( iIndexX, iIndexZ );
}


//-----------------------------------------------------------------------------
// Name			: GetRpWorldSecorByPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const RpWorldSector * CWorldEntity::GetRpWorldSectorByPosition( SLocation3DInt * pOffset, SLocation3DInt * pSize ) const
{
	if( GetRpWorld() == NULL )
	{
		return NULL;
	}

	return GetRpWorldSectorByIndexFB( GetRpWorld()->rootSector, pOffset, pSize );
}


//-----------------------------------------------------------------------------
// Name			: CalculateVertexColor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::CalculateVertexColor( SVertexColorGenerationInfo * pInfo )
{
	if( m_pWorldSectorMap == NULL )
	{
		return FALSE;
	}

	SVertexColorGenerationInfo		sInfo;

	if( pInfo == NULL )
	{
		pInfo = &sInfo;
		sInfo.pAmbientLight		= NULL;
		sInfo.pDirectionalLight	= NULL;
		sInfo.pShadowLight		= NULL;

		SEntityContainerProperty	sProperty( E_ENTITY_CONTAINER_TYPE_LIST, FALSE );
		CEntityContainer *			pContainer = g_pEntityFactory->CreateEntityContainer( &sProperty );

		SEntitySearchCondition		sCondition( E_ENTITY_TYPE_FILTER_LIGHT );

		SearchEntity( pContainer, &sCondition );

		CLightEntity *				pLightEntity;
		for( RwUInt32 iOrder = 0; iOrder < pContainer->GetCount(); ++iOrder )
		{
			pLightEntity = reinterpret_cast<CLightEntity *>(pContainer->GetAt( iOrder ));

			if( sInfo.pAmbientLight == NULL &&
				pLightEntity->IsTerrainLight() != FALSE &&
				pLightEntity->GetLightType() == E_LIGHT_ENTITY_TYPE_AMBIENT )
			{
                sInfo.pAmbientLight = pLightEntity;
			}

			if( sInfo.pDirectionalLight == NULL &&
				pLightEntity->IsTerrainLight() != FALSE &&
				pLightEntity->GetLightType() == E_LIGHT_ENTITY_TYPE_DIRECTIONAL )
			{
                sInfo.pDirectionalLight = pLightEntity;
			}

			if( sInfo.pShadowLight == NULL &&
				pLightEntity->IsShadowLight() != FALSE &&
				pLightEntity->GetLightType() == E_LIGHT_ENTITY_TYPE_DIRECTIONAL )
			{
                sInfo.pShadowLight = pLightEntity;
			}
		}

		g_pEntityFactory->DestroyEntityContainer( &pContainer );
	}

	return m_pWorldSectorMap->CalculateVertexColor( pInfo );
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CWorldEntity::Export( CFixedStringList * pStringList, CFixedString * pName )
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

	// Name

	String.Assign( *(GetWorldProperty()->GetName()) );
	pStringList->Push( &String );

	if( GetWorldProperty()->IsEmpty() != FALSE )
	{
		String.Assign( _T( "#empty" ) );
		pStringList->Push( &String );
	}
	else if( GetWorldProperty()->IsCustomBuild() != FALSE )
	{
		String.Assign( _T( "#custom" ) );
		pStringList->Push( &String );
	}

	// Bounding

	String.AssignFloat( GetWorldProperty()->GetBounding()->GetMinX() );
	pStringList->Push( &String );

	String.AssignFloat( GetWorldProperty()->GetBounding()->GetMinY() );
	pStringList->Push( &String );

	String.AssignFloat( GetWorldProperty()->GetBounding()->GetMinZ() );
	pStringList->Push( &String );

	String.AssignFloat( GetWorldProperty()->GetBounding()->GetMaxX() );
	pStringList->Push( &String );

	String.AssignFloat( GetWorldProperty()->GetBounding()->GetMaxY() );
	pStringList->Push( &String );

	String.AssignFloat( GetWorldProperty()->GetBounding()->GetMaxZ() );
	pStringList->Push( &String );

	// Sector

	String.AssignInteger( GetWorldProperty()->GetSectorMin()->GetX() );
	pStringList->Push( &String );

	String.AssignInteger( GetWorldProperty()->GetSectorMin()->GetY() );
	pStringList->Push( &String );

	String.AssignInteger( GetWorldProperty()->GetSectorMin()->GetZ() );
	pStringList->Push( &String );

	String.AssignInteger( GetWorldProperty()->GetSectorMax()->GetX() );
	pStringList->Push( &String );

	String.AssignInteger( GetWorldProperty()->GetSectorMax()->GetY() );
	pStringList->Push( &String );

	String.AssignInteger( GetWorldProperty()->GetSectorMax()->GetZ() );
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


//-----------------------------------------------------------------------------
// Name			: ExportChild
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CWorldEntity::ExportChild( CFixedStringList * pStringList, CFixedString * pName )
{
	RwInt32		iTotalExportCount = CEntity::ExportChild( pStringList, pName );
	if( iTotalExportCount < 0 )
	{
		return iTotalExportCount;
	}

	// Local

	RwInt32		iLocalExportCount;

	if( m_pWorldSectorMap != NULL )
	{
		iLocalExportCount = m_pWorldSectorMap->Export( pStringList, pName );

		if( iLocalExportCount < 0 )
		{
			return iLocalExportCount;
		}

		iTotalExportCount += iLocalExportCount;
	}

	CFixedString		String;
	String.Assign( RWSTRING( "\r\n" ) );
	pStringList->Push( &String );

	return iTotalExportCount;
}


//-----------------------------------------------------------------------------
// Name			: ReportSelfState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::ReportSelfState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	return CEntity::ReportSelfState( pLogMessageManager, iTab );
}


//-----------------------------------------------------------------------------
// Name			: ReportChildState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	RwBool		bResult = TRUE;

	if( CEntity::ReportChildState( pLogMessageManager, iTab ) == FALSE )
	{
		bResult = FALSE;
	}

	if( m_pWorldSectorMap != NULL )
	{
		if( m_pWorldSectorMap->ReportState( pLogMessageManager, iTab + 1 ) == FALSE )
		{
			bResult = FALSE;
		}
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: GetHeight
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::GetHeight( SLocation3D * pLocation, RwBool bUseHeightMap ) const
{
	SLocation3D		sTestLocation( *pLocation );

	sTestLocation.SetY( ( GetBBox()->inf.y + GetBBox()->sup.y ) * 0.5f );

	if( IsPointInBBox( sTestLocation.GetConstRwV3d(), GetBBox() ) == FALSE )
	{
		return FALSE;
	}

	if( m_pWorldSectorMap == NULL )
	{
		return FALSE;
	}

	SEntityContainerProperty	sResultListProperty( E_ENTITY_CONTAINER_TYPE_LIST, FALSE );
	CEntityContainer *			pResultList = g_pEntityFactory->CreateEntityContainer( &sResultListProperty );
	RwUInt32					iResultCount = m_pWorldSectorMap->SearchByLocation( pLocation, pResultList, E_ENTITY_TYPE_WORLD_SECTOR );

	if( iResultCount == 0 )
	{
		g_pEntityFactory->DestroyEntityContainer( &pResultList );

		return FALSE;
	}

	CWorldSectorEntity *		pWorldSectorEntity;
	RwBool						bDetected = FALSE;
	SLocation3D					sResultLocation( *pLocation );
	sResultLocation.SetY( GetBBox()->inf.y );

	for( RwUInt32 iIndex = 0; iIndex < iResultCount; ++iIndex )
	{
		pWorldSectorEntity = reinterpret_cast<CWorldSectorEntity *>(pResultList->GetAt( iIndex ));

		Assert( pWorldSectorEntity != NULL );
		Assert( pWorldSectorEntity->GetType() == E_ENTITY_TYPE_WORLD_SECTOR );

		sTestLocation.SetY( pLocation->GetY() );
		if( pWorldSectorEntity->GetHeight( &sTestLocation, bUseHeightMap ) != FALSE )
		{
			if( sResultLocation.GetY() < sTestLocation.GetY() )
			{
				sResultLocation.SetY( sTestLocation.GetY() );

				bDetected = TRUE;
			}
		}
	}

	g_pEntityFactory->DestroyEntityContainer( &pResultList );

	if( bDetected != FALSE )
	{
		pLocation->SetY( sResultLocation.GetY() );
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

RwBool CWorldEntity::CreateHeightMap( RwReal fPrecision, RwBool bLoadFromFile )
{
	if( m_pWorldSectorMap != NULL )
	{
		CWorldSectorEntity *	pWorldSector;

		for( RwUInt32 iOrder = 0; iOrder < m_pWorldSectorMap->GetCount(); ++iOrder )
		{
			pWorldSector = reinterpret_cast<CWorldSectorEntity *>(m_pWorldSectorMap->GetAt( iOrder ));

			if( pWorldSector != NULL )
			{
				if( pWorldSector->CreateHeightMap( fPrecision, bLoadFromFile ) == FALSE )
				{
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}


//*****************************************************************************
//
// End of File : CWorldEntity.cpp
//
//*****************************************************************************