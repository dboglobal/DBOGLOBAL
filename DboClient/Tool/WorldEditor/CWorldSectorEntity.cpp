//*****************************************************************************
// File       : CWorldSectorEntity.cpp
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

#include "CAtomicEntity.h"
#include "CCameraEntity.h"
#include "CWorldEntity.h"
#include "CMeshEntity.h"

#include "CEntityListContainer.h"
#include "CEntityFactory.h"

#include "CEntityCubeContainer.h"
#include "CSceneManager.h"

#include "CEntityCommand.h"
#include "CSerializer.h"

#include "CLightEntity.h"
#include "CEditorApplication.h"
#include "CHeightMap.h"
#include "CRenderManager.h"

#include "Collision.h"
#include "CEntityCollisionList.h"

#include "CWorldSectorEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CFixedString CWorldSectorEntity::m_FilePath = CFixedString( _T( "world\\structure" ) );
CFixedString CWorldSectorEntity::m_FileExt = CFixedString( _T( "wst" ) );


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldSectorEntity::Initialize( void )
{
	BEGIN_FUNCTION( "CWorldSectorEntity::Initialize" );

	CEntity::Initialize();

	m_bEmpty			= TRUE;
	m_bLocked			= FALSE;

	m_pTerrainEntity	= NULL;
	m_pHeightMap		= NULL;

	InitializeChildren();

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty )
{
	BEGIN_FUNCTION( "CWorldSectorEntity::Create" );

	Assert( pEntityProperty != NULL );
	Assert( pEntityProperty->GetType() == E_ENTITY_TYPE_WORLD_SECTOR );

	const CWorldSectorEntityProperty *	pSectorProperty = reinterpret_cast<const CWorldSectorEntityProperty *>(pEntityProperty);
	CWorldEntity *						pParentWorld = reinterpret_cast<CWorldEntity *>(g_pSceneManager->GetEntity( pCreateProperty->GetParentID() ));

	SLocation3DInt						Offset;
	SLocation3DInt						Size;

	Offset.Offset( pSectorProperty->GetSectorIndex(), pParentWorld->GetSectorMin() );
	Size.Size( pParentWorld->GetSectorMax(), pParentWorld->GetSectorMin() );

	(const_cast<CEntityProperty *>(pEntityProperty))->SetRwPrototype( const_cast<void *>(reinterpret_cast<const void *>(pParentWorld->GetRpWorldSectorByPosition( &Offset, &Size ))) );
	RpWorldSector *						pWorldSector = reinterpret_cast<RpWorldSector *>(pSectorProperty->GetRwPrototype());
	Assert( pWorldSector != NULL );

	m_Position.SetX( ( pWorldSector->boundingBox.sup.x + pWorldSector->boundingBox.inf.x ) * 0.5f );
	m_Position.SetY( ( pWorldSector->boundingBox.sup.y + pWorldSector->boundingBox.inf.y ) * 0.5f );
	m_Position.SetZ( ( pWorldSector->boundingBox.sup.z + pWorldSector->boundingBox.inf.z ) * 0.5f );

	if( CEntity::Create( pEntityProperty, pCreateProperty ) == FALSE )
	{
		END_FUNCTION( FALSE );
	}

	CreateHeightMap( 0.0f, TRUE );

	END_FUNCTION( TRUE );
}


//-----------------------------------------------------------------------------
// Name			: PostCreate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::PostCreate( void )
{
	CreateInternalMeshEntity();

	return CEntity::PostCreate();
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldSectorEntity::Destroy( void )
{
	BEGIN_FUNCTION( "CWorldSectorEntity::Destroy" );

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

void CWorldSectorEntity::PreDestroy( void )
{
//	ResetPlugInDataToRpWorldSector();

	DestroyHeightMap();

	UnloadTerrain();

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

RwBool CWorldSectorEntity::UpdateFrame( void )
{
	BEGIN_FUNCTION( "CWorldSectorEntity::UpdateFrame" );

	if( IsEmpty() != FALSE )
	{
		END_FUNCTION( TRUE );
	}

	if( CalculateVisibilty() != FALSE )
	{
		if( m_pTerrainEntity == NULL )
		{
			if( LoadTerrain() == FALSE )
			{
				END_FUNCTION( FALSE );
			}
		}

		m_pTerrainEntity->UpdateFrame();
	}
	else
	{
		if( m_pTerrainEntity != NULL )
		{
			if( UnloadTerrain() == FALSE )
			{
				END_FUNCTION( FALSE );
			}
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

RwBool CWorldSectorEntity::RenderFrame( CEntity * pRenderer )
{
	BEGIN_FUNCTION( "CWorldSectorEntity::RenderFrame" );

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

	RwBool		bResult;

#ifdef _COMPILE_OPTION_USE_EXTERNAL_TERRAIN_
	if( m_pTerrainEntity != NULL )
	{
		Assert( IsPointInBounding( reinterpret_cast<const SLocation3D *>(&(m_pTerrainEntity->GetBounding()->center)) ) != FALSE );

		bResult = m_pTerrainEntity->RenderFrame( pRenderer );
	}
#endif //_COMPILE_OPTION_USE_EXTERNAL_TERRAIN_

//	bResult = CEntity::RenderFrame( pRenderer );

	bResult = RenderWorldSectorAtomic( pRenderer );

	g_pSceneManager->IncreaseRenderCountWorldSector();

	END_FUNCTION( TRUE );
}


//-----------------------------------------------------------------------------
// Name			: Select
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CWorldSectorEntity::Select( void )
{
	RwUInt32	iSelectedCount = CEntity::Select();

	if( m_pTerrainEntity != NULL )
	{
		iSelectedCount += m_pTerrainEntity->Select();
	}

	return iSelectedCount;
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: Unselect
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CWorldSectorEntity::Unselect( void )
{
	RwUInt32	iUnselectedCount = CEntity::Unselect();

	if( m_pTerrainEntity != NULL )
	{
		iUnselectedCount += m_pTerrainEntity->Unselect();
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

RwUInt32 CWorldSectorEntity::StartAnimation( RwReal fTime )
{
	RwUInt32	iSelectedCount = CEntity::StartAnimation( fTime );

	if( m_pTerrainEntity != NULL )
	{
		iSelectedCount += m_pTerrainEntity->StartAnimation( fTime );
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

RwUInt32 CWorldSectorEntity::StopAnimation( RwReal fTime )
{
	RwUInt32	iSelectedCount = CEntity::StopAnimation( fTime );

	if( m_pTerrainEntity != NULL )
	{
		iSelectedCount += m_pTerrainEntity->StopAnimation( fTime );
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

RwBool CWorldSectorEntity::Attach( CEntity * pChild )
{
	Assert( pChild != NULL );

	if( InsertChild( pChild ) == FALSE )
	{
		//g_Log( (RwChar *)(pChild->GetNameBuffer()) );

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
			if( RpWorldAddAtomic( reinterpret_cast<RpWorld *>(GetParent()->GetRwInstance()), reinterpret_cast<RpAtomic *>(pChild->GetRwInstance()) ) == NULL )
			{
				Assert( !"RpWorldAddAtomic() Fail" );

				return FALSE;
			}

			break;
		}

		case E_ENTITY_TYPE_CLUMP:
		{
			if( RpWorldAddClump( reinterpret_cast<RpWorld *>(GetParent()->GetRwInstance()), reinterpret_cast<RpClump *>(pChild->GetRwInstance()) ) == NULL )
			{
				Assert( !"RpWorldAddClump() Fail" );

				return FALSE;
			}

			break;
		}

		case E_ENTITY_TYPE_LIGHT:
		{
			if( RpWorldAddLight( reinterpret_cast<RpWorld *>(GetParent()->GetRwInstance()), reinterpret_cast<RpLight *>(pChild->GetRwInstance()) ) == NULL )
			{
				Assert( !"RpWorldAddLight() Fail" );

				return FALSE;
			}

			break;
		}

		case E_ENTITY_TYPE_CAMERA:
		{
		//	RpWorldAddCamera( reinterpret_cast<RpWorld *>(GetParent()->GetRwInstance()), reinterpret_cast<RwCamera *>(pChild->GetRwInstance()) );

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

RwBool CWorldSectorEntity::Detach( CEntity * pChild )
{
	Assert( pChild != NULL );
	Assert( m_pChildren != NULL );

	if( RemoveChild( pChild ) == FALSE )
	{
		return FALSE;
	}

	switch( pChild->GetType() )
	{
		case E_ENTITY_TYPE_ATOMIC:
		{
			RpWorldRemoveAtomic( reinterpret_cast<RpWorld *>(GetParent()->GetRwInstance()), reinterpret_cast<RpAtomic *>(pChild->GetRwInstance()) );

			break;
		}

		case E_ENTITY_TYPE_CLUMP:
		{
			RpWorldRemoveClump( reinterpret_cast<RpWorld *>(GetParent()->GetRwInstance()), reinterpret_cast<RpClump *>(pChild->GetRwInstance()) );

			break;
		}

		case E_ENTITY_TYPE_LIGHT:
		{
			RpWorldRemoveLight( reinterpret_cast<RpWorld *>(GetParent()->GetRwInstance()), reinterpret_cast<RpLight *>(pChild->GetRwInstance()) );

			break;
		}

		case E_ENTITY_TYPE_CAMERA:
		{
		//	RpWorldRemoveCamera( reinterpret_cast<RpWorld *>(GetParent()->GetRwInstance()), reinterpret_cast<RwCamera *>(pChild->GetRwInstance()) );

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
// Name			: CreateInternalMeshEntity
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CWorldSectorEntity::CreateInternalMeshEntity( void )
{
	RpWorldSector *		pRpWorldSector = GetRpWorldSector();
	if( pRpWorldSector == NULL )
	{
		return 0;
	}

	RpMeshHeader *		pRpMeshHeader = pRpWorldSector->mesh;
	if( pRpMeshHeader == NULL || pRpMeshHeader->numMeshes == 0 )
	{
		return 0;
	}

	return CMeshEntity::CreateInternalMeshFromMeshHeader( this, pRpMeshHeader );
}


//-----------------------------------------------------------------------------
// Name			: SetParameter
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::SetParameter( void )
{
	CEntityCubeContainer *	pMap = reinterpret_cast<CEntityCubeContainer *>(GetWorldSectorProperty()->GetSectorContainer());
	const SLocation3DInt *	pSectorIndex = GetWorldSectorProperty()->GetSectorIndex();

	RwChar		NameBuffer[256];

	const CWorldEntity *	pParentWorld = reinterpret_cast<const CWorldEntity *>(pMap->GetParent());

	if( pSectorIndex->GetY() != 0 )
	{
		rwsprintf( NameBuffer, RWSTRING( "%s%d%d%d" ),
					pParentWorld->GetPropertyName()->GetBuffer(),
					pSectorIndex->GetX(), pSectorIndex->GetY(), pSectorIndex->GetZ() );
	}
	else
	{
		rwsprintf( NameBuffer, RWSTRING( "%s%d%d" ),
					pParentWorld->GetPropertyName()->GetBuffer(),
					pSectorIndex->GetX(), pSectorIndex->GetZ() );
	}

	m_sName.Assign( NameBuffer );

	SLocation3DInt	Offset;
	SLocation3DInt	Size;

	Offset.Offset( pSectorIndex, pMap->GetMin() );

	Size.Set( reinterpret_cast<const CEntityCubeContainer *>(GetSectorContainer())->GetSize() );

	SetEmpty( FALSE );

	if( m_iRenderPriority == ENTITY_RENDER_PRIORITY_NONE )
	{
		if( GetCategory() == E_ENTITY_CATEGORY_EXTERNAL )
		{
			m_iRenderPriority	= ENTITY_RENDER_PRIORITY_EXTERNAL_DEFAULT;
		}
		else
		{
			m_iRenderPriority	= ENTITY_RENDER_PRIORITY_NORMAL_DEFAULT;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: SetDefaultName
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldSectorEntity::SetDefaultName( void )
{
	return;
}


//-----------------------------------------------------------------------------
// Name			: InitializeChildren
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldSectorEntity::InitializeChildren( void )
{
	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
	{
		m_pChildren[iCategoryIndex] = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: DestroyChildren
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldSectorEntity::DestroyChildren( void )
{
	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
	{
		CEntityFactory::GetInstance()->DestroyEntityContainer( &m_pChildren[iCategoryIndex] );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: InsertChild
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::InsertChild( CEntity * pChild )
{
	Assert( pChild != NULL );

	if( pChild->IsTerrain() != FALSE )
	{
		Assert( m_pTerrainEntity == NULL );

		m_pTerrainEntity = pChild;

		return TRUE;
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

RwBool CWorldSectorEntity::RemoveChild( CEntity * pChild )
{
	Assert( pChild != NULL );

	if( pChild->IsTerrain() != FALSE )
	{
		Assert( m_pTerrainEntity == pChild );

		m_pTerrainEntity = NULL;

		return TRUE;
	}

	return CEntity::RemoveChild( pChild );
}


//-----------------------------------------------------------------------------
// Name			: SetGlobal
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldSectorEntity::SetFlag( RwUInt32 iFlag, RwBool bApplyToChild )
{
	CEntity::SetFlag( iFlag, bApplyToChild );

	if( bApplyToChild != FALSE && m_pTerrainEntity != NULL )
	{
		m_pTerrainEntity->SetFlag( iFlag, bApplyToChild );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: CalculateVisibilty
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::CalculateVisibilty( void ) const
{
	return TRUE;
	/*
	CCameraEntity *	pCamera = g_pSceneManager->GetActiveCamera();

	if( IsPointInBounding( pCamera->GetPosition()->GetLocation() ) != FALSE )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}*/
}


//-----------------------------------------------------------------------------
// Name			: LoadTerrain
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::LoadTerrain( void )
{
	BEGIN_FUNCTION( "CWorldSectorEntity::LoadTerrain" );

	if( m_pTerrainEntity != NULL )
	{
		return TRUE;
	}

	if( IsLocked() != FALSE )
	{
		Assert( !"IsLocked" );

		END_FUNCTION( FALSE );
	}

	Lock();

	CSerializer	Serializer;
	RwInt32		iIndex = 0;

	RwChar			szName[256];
	rwsprintf( szName, RWSTRING( "%s\\%s\\%s.%s" ),
				GetFilePath()->GetBuffer(),
				g_pEngineApp->GetName()->GetBuffer(),
				GetName()->GetBuffer(),
				GetFileExt()->GetBuffer() );

	RwBool		bResult = Serializer.LoadFromFile( szName );

	if( bResult == FALSE )
	{
		CAtomicEntityProperty		TerrainProperty;
		SEntityCreateProperty		CreateProperty;

		TerrainProperty.SetPosition( GetPosition() );
		TerrainProperty.SetName( *GetName() );
		TerrainProperty.SetTerrain( TRUE );

		CreateProperty.SetUniqueID( ENTITY_UNIQUE_ID_AUTO );
		CreateProperty.SetParentID( GetUniqueID() );
		CreateProperty.SetCategory( E_ENTITY_CATEGORY_EXTERNAL );
		CreateProperty.SetBase( FALSE );
		CreateProperty.SetName( *GetName() );

		m_pTerrainEntity = g_pSceneManager->CreateEntity( &TerrainProperty, &CreateProperty );
		Assert( m_pTerrainEntity != NULL );

		Serializer.Destroy();

		return TRUE;
	}

	while( bResult != FALSE )
	{
		CEntityCommand *	pCommand = CEntityCommand::CreateCommand( &Serializer, iIndex );
		if( pCommand != NULL )
		{
			bResult = pCommand->Execute();
			if( bResult != FALSE )
			{
				CEntity * pEntity = reinterpret_cast<CEntity *>(pCommand->GetResultData());

				if( pEntity != NULL && pEntity->IsTerrain() != FALSE )
				{
					m_pTerrainEntity = pEntity;
				}
			}
			else
			{
				Assert( !"CCommand::Execute() Fail" );

				bResult = FALSE;
			}

			//++iIndex;

			CEntityCommand::DestroyCommand( &pCommand );
		}
		else
		{
		//	Assert( !"CCommand::CreateCommand() Fail" );

			bResult = FALSE;
		}
	}

	Serializer.Destroy();

//	rwsprintf( szName, RWSTRING( "%s.txt" ), GetName()->GetBuffer() );

//	CWorldMonitor::CreateInstance( szName );
//	CWorldMonitor::GetInstance()->AnalysisObject( m_pTerrainEntity->GetRwInstance() );
//	CWorldMonitor::DestroyInstance();

#ifndef _COMPILE_OPTION_USE_EXTERNAL_TERRAIN_
	RpWorldAddAtomic( (RpWorld *)GetParent()->GetRwInstance(), (RpAtomic *)m_pTerrainEntity->GetRwInstance() );
#endif //_COMPILE_OPTION_USE_EXTERNAL_TERRAIN_

	Unlock();

	END_FUNCTION( TRUE );
}


//-----------------------------------------------------------------------------
// Name			: UnloadTerrain
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::UnloadTerrain( void )
{
	BEGIN_FUNCTION( "CWorldSectorEntity::UnloadTerrain" );

	if( IsLocked() != FALSE )
	{
		END_FUNCTION( FALSE );
	}

	Lock();

	if( m_pTerrainEntity != NULL )
	{
	#ifndef _COMPILE_OPTION_USE_EXTERNAL_TERRAIN_
		RpWorldRemoveAtomic( (RpWorld *)GetParent()->GetRwInstance(), (RpAtomic *)m_pTerrainEntity->GetRwInstance() );
	#endif /_COMPILE_OPTION_USE_EXTERNAL_TERRAIN_

		g_pSceneManager->DestroyEntity( &m_pTerrainEntity );

		Assert( m_pTerrainEntity == NULL );
	}

	Unlock();

	END_FUNCTION( TRUE );
}


//-----------------------------------------------------------------------------
// Name			: SetRwFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldSectorEntity::SetRwFrame( const RwFrame * pRwFrame )
{
	if( GetParent() != NULL )
	{
		GetParent()->SetRwFrame( pRwFrame );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetRwFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwFrame * CWorldSectorEntity::GetRwFrame( void ) const
{
	if( GetParent() != NULL )
	{
		return GetParent()->GetRwFrame();
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: SearchByRwInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CWorldSectorEntity::SearchByRwInstance( void * pRwInstance ) const
{
	if( GetRwInstance() == pRwInstance )
	{
		return const_cast<CEntity *>(reinterpret_cast<const CEntity *>(this));
	}

	if( m_pTerrainEntity != NULL )
	{
		if( m_pTerrainEntity->GetRwInstance() == pRwInstance )
		{
			return m_pTerrainEntity;
		}
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

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: SearchByLocation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CWorldSectorEntity::SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultList, EEntityType eEntityType ) const
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

	iMatchCount += CEntity::SearchByLocation( pLocation, pResultList, eEntityType );

	return iMatchCount;
}


//-----------------------------------------------------------------------------
// Name			: CalculateVertexColor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::CalculateVertexColor( SVertexColorGenerationInfo * pInfo )
{
	if( m_pTerrainEntity == NULL )
	{
		return FALSE;
	}

	if( IsSelected() == FALSE )
	{
		return FALSE;
	}

	return m_pTerrainEntity->CalculateVertexColor( pInfo );
}


//-----------------------------------------------------------------------------
// Name			: DetectCollision
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::DetectCollision( CCollisionDetectionInfo * pCollisionInfo ) const
{
	RwBool		bResult = TRUE;

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CWorldSectorEntity::Export( CFixedStringList * pStringList, CFixedString * pName )
{
	CFixedString	String;

	String.Assign( RWSTRING( "#sector" ) );
	pStringList->Push( &String );

	String.Assign( *GetName() );
	pStringList->Push( &String );

	String.Assign( RWSTRING( "\r\n" ) );
	pStringList->Push( &String );

	// Export Sector To File

	CFixedStringList	StringList;

	RwInt32		iExportCount = ExportSector( &StringList, pName );
	if( iExportCount < 0 )
	{
		return iExportCount;
	}

	RwChar		szFileName[256];
	rwsprintf( szFileName, RWSTRING( "%s\\%s\\%s.%s" ), m_FilePath.GetBuffer(), g_pEngineApp->GetName()->GetBuffer(), GetName()->GetBuffer(), m_FileExt.GetBuffer() );

	HANDLE		hFile = CreateFileSafe( szFileName, false );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		Assert( !"CreateFile() Failure" );

		return -1;
	}

	StringList.Serialize( hFile );

	CloseHandle( hFile );

	return iExportCount;
}


//-----------------------------------------------------------------------------
// Name			: ExportSector
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CWorldSectorEntity::ExportSector( CFixedStringList * pStringList, CFixedString * pName )
{
	RwInt32			iTerrainCount;
	RwInt32			iChildCount;

	if( m_pTerrainEntity != NULL )
	{
		iTerrainCount = m_pTerrainEntity->Export( pStringList, pName );
	}
	else
	{
		iTerrainCount = 0;
	}

	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_EXTERNAL; ++iCategoryIndex )
	{
		if( m_pChildren[iCategoryIndex] != NULL )
		{
			iChildCount = m_pChildren[iCategoryIndex]->Export( pStringList, pName );
			if( iChildCount < 0 )
			{
				return iChildCount;
			}

			iTerrainCount += iChildCount;
		}
	}

	return iTerrainCount;
}


//-----------------------------------------------------------------------------
// Name			: ReportSelfState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::ReportSelfState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	RwChar		szBuffer[256];
	rwsprintf( szBuffer, RWSTRING( "Id : 0x%08X, Name : \"%s\", Category : %d, Parent : 0x%08X, Mov : %d, Vis : %d, Glb : %d" ),
				m_iUniqueID, GetName()->GetBuffer(), m_eCategory,
				( ( m_pParent == NULL ) ? 0L : m_pParent->GetUniqueID() ),
				IsMovable(), IsVisible(), IsGlobal() );

	pLogMessageManager->SetLastLogMessageWithTab( szBuffer, iTab );

//	CWorldMonitor::CreateInstance( pLogMessageManager );
//	CWorldMonitor::GetInstance()->AnalysisSector( GetRwInstance() );
//	CWorldMonitor::DestroyInstance();

//	m_pProperty->ReportState( pLogMessageManager, iTab + 1 );
//	m_pResource->ReportState( pLogMessageManager, iTab + 1 );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ReportChildState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	RwBool		bResult = TRUE;

	if( m_pTerrainEntity != NULL )
	{
		if( m_pTerrainEntity->ReportState( pLogMessageManager, iTab + 1 ) == FALSE )
		{
			bResult = FALSE;
		}
	}

	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
	{
		if( m_pChildren[iCategoryIndex] != NULL )
		{
			if( m_pChildren[iCategoryIndex]->ReportState( pLogMessageManager, iTab + 1 ) == FALSE )
			{
				bResult = FALSE;
			}
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: GetBBox
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const RwBBox * CWorldSectorEntity::GetBBox( void ) const
{
	if( GetRpWorldSector() == NULL )
	{
		return NULL;
	}

	return RpWorldSectorGetBBox( GetRpWorldSector() );
}


//-----------------------------------------------------------------------------
// Name			: GetHeight
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::GetHeight( SLocation3D * pLocation, RwBool bUseHeightMap ) const
{
	SLocation3D		sTestPostion( *pLocation );

	sTestPostion.SetY( ( GetBBox()->inf.y + GetBBox()->sup.y ) * 0.5f );

	if( IsPointInBBox( sTestPostion.GetConstRwV3d(), GetBBox() ) == FALSE )
	{
		return FALSE;
	}

	if( bUseHeightMap != FALSE && m_pHeightMap != NULL )
	{
		pLocation->SetY( m_pHeightMap->GetHeight( pLocation ) );

		return TRUE;
	}

	if( m_pTerrainEntity == NULL )
	{
		return FALSE;
	}

	CCollisionDetectionInfo	cDetectionInfo;
	CEntity *				pEntity;
	CEntityCollision *		pEntityCollision;

	cDetectionInfo.CreateByGetHeight( pLocation );
	g_pSceneManager->DetectCollision( &cDetectionInfo );

	for( RwInt32 iOrder = 0; iOrder < cDetectionInfo.GetResultList()->GetCount(); ++iOrder )
	{
		pEntityCollision = cDetectionInfo.GetResultList()->GetCollisionEntityByOrder( iOrder );
		pEntity = pEntityCollision->GetEntity();

		if( pEntity->GetType() == E_ENTITY_TYPE_ATOMIC )
		{
			pLocation->SetY( pEntityCollision->GetLocation()->GetY() );

			return TRUE;
		}
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: CreateHeightMap
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::CreateHeightMap( RwReal fPrecision, RwBool bLoadFromFile )
{
	if( m_pHeightMap != NULL )
	{
		DestroyHeightMap();
	}

	m_pHeightMap = NTL_NEW CHeightMap;
	Assert( m_pHeightMap != NULL );

	if( bLoadFromFile == FALSE )
	{
		const SBoundingBox * pBounding = reinterpret_cast<const SBoundingBox *>(RpWorldSectorGetBBox( GetRpWorldSector() ));

		if( pBounding == NULL )
		{
			return FALSE;
		}

		if( m_pHeightMap->Create( pBounding, fPrecision ) == FALSE )
		{
			return FALSE;
		}

		SLocation3D			sNodePosition;
		SLocation3DInt		sNodeIndex;
		SLocation3D			sSamplePosition;
		RwReal				fSampleSum;
		RwInt32				iSampleCount;
		RwInt32				iNodeCount = m_pHeightMap->GetNodeCount()->GetX() * m_pHeightMap->GetNodeCount()->GetZ();
		RwInt32				iNodeIndex = 0;

		for( sNodeIndex.SetX( 0 ); sNodeIndex.GetX() < m_pHeightMap->GetNodeCount()->GetX(); sNodeIndex.IncreaseX() )
		{
			sNodePosition.SetX( pBounding->GetMinX() + fPrecision * sNodeIndex.GetX() );

			for( sNodeIndex.SetZ( 0 ); sNodeIndex.GetZ() < m_pHeightMap->GetNodeCount()->GetZ(); sNodeIndex.IncreaseZ() )
			{
				sNodePosition.SetZ( pBounding->GetMinZ() + fPrecision * sNodeIndex.GetZ() );
				sNodePosition.SetY( GetBBox()->sup.y );

				m_pHeightMap->GetNodeIndex( &sNodePosition, &sNodeIndex );

				if( g_pSceneManager->GetHeight( &sNodePosition, FALSE ) != FALSE )
				{
					m_pHeightMap->SetNode( &sNodeIndex, sNodePosition.GetY() );
				}
				else
				{
					for( RwReal fSearchRange = 1.0f; fSearchRange < CREATE_HEIGHT_SEARCH_OFFSET_MAX; fSearchRange *= 2.0f )
					{
						fSampleSum = 0.0f;
						iSampleCount = 0;
						sSamplePosition.Set( sNodePosition );

						sSamplePosition.SetX( sSamplePosition.GetX() - fSearchRange );
						sSamplePosition.SetZ( sSamplePosition.GetZ() - fSearchRange );
						sSamplePosition.SetY( GetBBox()->sup.x );
						if( g_pSceneManager->GetHeight( &sSamplePosition, FALSE ) != FALSE )
						{
							fSampleSum += sSamplePosition.GetY();
							++iSampleCount;
						}

						sSamplePosition.SetX( sSamplePosition.GetX() + fSearchRange * 2.0f );
						sSamplePosition.SetY( GetBBox()->sup.x );
						if( g_pSceneManager->GetHeight( &sSamplePosition, FALSE ) != FALSE )
						{
							fSampleSum += sSamplePosition.GetY();
							++iSampleCount;
						}

						sSamplePosition.SetZ( sSamplePosition.GetZ() + fSearchRange * 2.0f );
						sSamplePosition.SetY( GetBBox()->sup.x );
						if( g_pSceneManager->GetHeight( &sSamplePosition, FALSE ) != FALSE )
						{
							fSampleSum += sSamplePosition.GetY();
							++iSampleCount;
						}

						sSamplePosition.SetX( sSamplePosition.GetX() - fSearchRange * 2.0f );
						sSamplePosition.SetY( GetBBox()->sup.x );
						if( g_pSceneManager->GetHeight( &sSamplePosition, FALSE ) != FALSE )
						{
							fSampleSum += sSamplePosition.GetY();
							++iSampleCount;
						}

						if( iSampleCount > 0 )
						{
							m_pHeightMap->SetNode( &sNodeIndex, fSampleSum / static_cast<RwReal>(iSampleCount) );

							break;
						}
					}

					if( iSampleCount == 0 )
					{
						m_pHeightMap->SetNode( &sNodeIndex, -RwRealMAXVAL );
					}
				}//if( g_pSceneManager->GetHeight( &sNodePosition, FALSE ) != FALSE )

				RwChar	szText[256];
				rwsprintf( szText, RWSTRING( "Create Height Map : %d / %d" ), ++iNodeIndex, iNodeCount );
				g_pEngineApp->SetStatusText( szText );
			} // for( sNodeIndex.SetZ( 0 )
		} // for( sNodeIndex.SetX( 0 )

		return m_pHeightMap->Export( GetName() );
	}
	else
	{
		return m_pHeightMap->Import( GetName() );
	}
}


//-----------------------------------------------------------------------------
// Name			: DestroyHeightMap
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldSectorEntity::DestroyHeightMap( void )
{
	if( m_pHeightMap != NULL )
	{
		NTL_DELETE( m_pHeightMap );
		m_pHeightMap = NULL;
	}

	return;
}


//*****************************************************************************
//
// End of File : CWorldSectorEntity.cpp
//
//*****************************************************************************