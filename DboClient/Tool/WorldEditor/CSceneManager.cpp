//*****************************************************************************
// File       : CSceneManager.cpp
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
#include "CEntity.h"
#include "CAtomicEntityProperty.h"
#include "CAtomicEntity.h"
#include "CClumpEntityProperty.h"
#include "CClumpEntity.h"
#include "CLightEntityProperty.h"
#include "CLightEntity.h"
#include "CCameraEntityProperty.h"
#include "CCameraEntity.h"
#include "CWorldEntityProperty.h"
#include "CWorldEntity.h"
#include "CAnimationEntityProperty.h"
#include "CAnimationEntity.h"
#include "CEntityEvent.h"
#include "CEntityCommand.h"
#include "CEntityContainer.h"
#include "CEntityListContainer.h"
#include "CEntityFactory.h"
#include "CEntityEventQueue.h"
#include "CEditorApplication.h"
#include "CRenderManager.h"
#include "CLightMapManager.h"

#include "CInstanceView.h"

#include "CSceneManager.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CSceneManager *				CSceneManager::m_pInstance = NULL;
CFixedString				CSceneManager::m_FilePath = CFixedString( _T( "world\\structure" ) );
CFixedString				CSceneManager::m_FileExt = CFixedString( _T( "wlt" ) );


//-----------------------------------------------------------------------------
// Name			: CreateInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::CreateInstance( void )
{
	if( m_pInstance == NULL )
	{
		m_pInstance = NTL_NEW CSceneManager;

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

void CSceneManager::DestroyInstance( void )
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

CSceneManager * CSceneManager::GetInstance( void )
{
	return m_pInstance;
}


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CSceneManager::Initialize( void )
{
	BEGIN_FUNCTION( "CSceneManager::Initialize" );

	m_pBaseWorld		= NULL;
	m_pWorldList		= NULL;

	m_pActiveCamera		= NULL;
	m_pCameraList		= NULL;

	m_pRenderManager	= NULL;
	m_pLightMapManager	= NULL;

	m_pTrashList		= NULL;
	m_pEntityTable		= NULL;
	m_pEventQueue		= NULL;

	m_iFrameID			= 0L;

	ResetRenderCount();

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::Create( SRasterInfo * pRasterInfo )
{
	if( CEntityFactory::CreateInstance() == FALSE )
	{
		return FALSE;
	}

	m_pEventQueue = NTL_NEW CEntityEventQueue;

	SetRasterInfo( pRasterInfo );

	SEntityContainerProperty	ContainerProperty( E_ENTITY_CONTAINER_TYPE_LIST, TRUE, NULL, this );

	m_pWorldList = g_pEntityFactory->CreateEntityContainer( &ContainerProperty );
	m_pCameraList = g_pEntityFactory->CreateEntityContainer( &ContainerProperty );

	ContainerProperty.Assign( E_ENTITY_CONTAINER_TYPE_HASH_MAP, FALSE, NULL );

	m_pEntityTable = g_pEntityFactory->CreateEntityContainer( &ContainerProperty );

	m_pRenderManager = NTL_NEW CRenderManager;
	m_pRenderManager->Create();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CSceneManager::Destroy( void )
{
	BEGIN_FUNCTION( "CSceneManager::Destroy" );

	if( m_pRenderManager != NULL )
	{
		NTL_DELETE( m_pRenderManager );
	}

	DestroyLightMapManager();

	if( m_pEventQueue != NULL )
	{
		NTL_DELETE( m_pEventQueue );
		m_pEventQueue = NULL;
	}

	g_pEntityFactory->DestroyEntityContainer( &m_pWorldList );
	g_pEntityFactory->DestroyEntityContainer( &m_pCameraList );

	g_pEntityFactory->DestroyEntityContainer( &m_pEntityTable );

	CEntityFactory::DestroyInstance();

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: UpdateFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::UpdateFrame( RwReal fCurrentTime, RwReal fElaspedTime )
{
	BEGIN_FUNCTION( "CSceneManager::UpdateFrame" );

	if( m_pEventQueue != NULL )
	{
		m_pEventQueue->Update( fCurrentTime );
	}

	if( m_pWorldList != NULL )
	{
		m_pWorldList->UpdateFrame();
	}

	if( m_pCameraList != NULL )
	{
		m_pCameraList->UpdateFrame();
	}
	
	END_FUNCTION( TRUE );
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::RenderFrame( HWND hWnd )
{
	BEGIN_FUNCTION( "CSceneManager::RenderFrame" );

	ResetRenderCount();

	if( m_pActiveCamera != NULL )
	{
		++m_iFrameID;

		m_pActiveCamera->SetFrameID( m_iFrameID );
		m_pActiveCamera->SetWindow( hWnd );
		m_pActiveCamera->Clear();
	}

	if( m_pWorldList )
	{
		m_pWorldList->RenderFrame( m_pActiveCamera );
	}

	END_FUNCTION( TRUE );
}


//-----------------------------------------------------------------------------
// Name			: EndFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::EndFrame( RwUInt32 iFlags )
{
	BEGIN_FUNCTION( "CSceneManager::EndFrame" );

	if( m_pActiveCamera )
	{
		m_pActiveCamera->ShowRaster( iFlags );
	}

	END_FUNCTION( TRUE );
}



//-----------------------------------------------------------------------------
// Name			: CreateEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CSceneManager::CreateEntity( CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty )
{
	CEntity *	pEntityToCreate;

	// Set Parent UniqueID

	if( pCreateProperty->iParentID == ENTITY_UNIQUE_ID_PROPER_WORLD )
	{
		if( pEntityProperty->HasLocation() != FALSE )
		{
			SEntityContainerProperty		ContainerProperty( E_ENTITY_CONTAINER_TYPE_LIST, FALSE, NULL );
			CEntityContainer * pMatchEntityList = g_pEntityFactory->CreateEntityContainer( &ContainerProperty );

			RwUInt32	iMetchCount = g_pSceneManager->SearchByLocation( pEntityProperty->GetPosition()->GetLocation(), pMatchEntityList, E_ENTITY_TYPE_WORLD );

			if( iMetchCount == 0 )
			{
				pCreateProperty->iParentID = 0;
			}
			else
			{
				pCreateProperty->iParentID = pMatchEntityList->GetAt( 0 )->GetUniqueID();
			}

			g_pEntityFactory->DestroyEntityContainer( &pMatchEntityList );
		}
	}
	else if( pCreateProperty->iParentID != ENTITY_UNIQUE_ID_NONE )
	{
		if( g_pSceneManager->GetEntity( pCreateProperty->iParentID ) == NULL )
		{
			Assert( !"g_pSceneManager->GetEntity() == NULL" );

			return NULL;
		}
	}

	switch( pEntityProperty->GetType() )
	{
		case E_ENTITY_TYPE_ANIMATION:
		{
			CClumpEntity *		pClumpEntity = reinterpret_cast<CClumpEntity *>(GetEntity( pCreateProperty->GetParentID() ));
			if( pClumpEntity == NULL )
			{
				return NULL;
			}

			CAnimationEntity *	pAnimationEntity = reinterpret_cast<CAnimationEntity *>(pClumpEntity->GetAnimationEntity());
			if( pAnimationEntity == NULL )
			{
				break;
			}

			pAnimationEntity->GetProperty()->SetProperty( pEntityProperty );

			return pAnimationEntity;
		}
	}

	pEntityToCreate = m_pEntityTable->GetEntity( pCreateProperty->GetUniqueID() );
	if( pEntityToCreate != NULL )
	{
		pEntityToCreate->IncreaseReferenceCount();

		pEntityToCreate->LinkParent( pCreateProperty->iParentID );

		return pEntityToCreate;
	}

	pEntityToCreate = g_pEntityFactory->CreateEntity( pEntityProperty, pCreateProperty );
	if( pEntityToCreate == NULL )
	{
		Assert( !"g_pEntityFactory->CreateEntity() == NULL" );

		return NULL;
	}

	m_pEntityTable->Insert( pEntityToCreate );

	if( CInstanceTreeCtrl::GetValidInstance() != NULL )
	{
		CInstanceTreeCtrl::GetValidInstance()->RegisterEntity( pEntityToCreate );
	}

	if( pEntityToCreate->PostCreate() == FALSE )
	{
		 g_pEntityFactory->DestroyEntity( &pEntityToCreate );

		Assert( !"pEntityToCreate->PostCreate() Fail" );

		return NULL;
	}

	switch( pEntityToCreate->GetType() )
	{
		case E_ENTITY_TYPE_CAMERA:
		{
			if( m_pCameraList->Insert( pEntityToCreate ) == FALSE )
			{
				Assert( !"m_pCameraList->Insert() == NULL" );
			}

			break;
		}

		case E_ENTITY_TYPE_WORLD:
		{
			if( m_pWorldList->Insert( pEntityToCreate ) == FALSE )
			{
				Assert( !"m_pWorldList->Insert() == NULL" );
			}

			break;
		}
	}

	return pEntityToCreate;
}


//-----------------------------------------------------------------------------
// Name			: DestroyEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::DestroyEntity( CEntity ** ppEntityToDestroy )
{
	if( *ppEntityToDestroy == NULL )
	{
		return FALSE;
	}

	(*ppEntityToDestroy)->DecreaseReferenceCount(); 
	if( (*ppEntityToDestroy)->GetReferenceCount() > 0 )
	{
		return (*ppEntityToDestroy)->RedirectParent( FALSE );
	}

	m_pEntityTable->Remove( *ppEntityToDestroy );

	(*ppEntityToDestroy)->PreDestroy();

	if( CInstanceTreeCtrl::GetValidInstance() != NULL )
	{
		CInstanceTreeCtrl::GetValidInstance()->UnregisterEntity( *ppEntityToDestroy );
	}

	RwBool		bFound = FALSE;

	switch( (*ppEntityToDestroy)->GetType() )
	{
		case E_ENTITY_TYPE_CAMERA:
		{
			if( *ppEntityToDestroy == m_pActiveCamera )
			{
				m_pActiveCamera = NULL;
			}

			bFound = m_pCameraList->Remove( *ppEntityToDestroy );

			break;
		}

		case E_ENTITY_TYPE_WORLD:
		{
			if( *ppEntityToDestroy == m_pBaseWorld )
			{
				m_pBaseWorld = NULL;
			}

			bFound = m_pWorldList->Remove( *ppEntityToDestroy );

			break;
		}
	}

	g_pEntityFactory->DestroyEntity( ppEntityToDestroy );
	*ppEntityToDestroy = NULL;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: MoveEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::MoveEntity( RwUInt32 iUniqueID, const SPosition3D * pPosition )
{
	CEntity * pEntity = GetEntity( iUniqueID );
	if( pEntity == NULL )
	{
		return FALSE;
	}

	pEntity->SetPosition( pPosition );

	CEntity * pParentEntity = pEntity->GetParent();

	if( ( pParentEntity != NULL ) &&
		( pParentEntity->GetType() == E_ENTITY_TYPE_WORLD || 
		pParentEntity->GetType() == E_ENTITY_TYPE_WORLD_SECTOR ) )
	{
		if( pParentEntity->IsPointInBounding( pPosition->GetLocation() ) != FALSE )
		{
			return TRUE;
		}

		RwUInt32	iResultCount = 0;
		SEntityContainerProperty		ContainerProperty( E_ENTITY_CONTAINER_TYPE_LIST, FALSE, NULL );
/*
		CEntityContainer * pResultList = g_pEntityFactory->CreateEntityContainer( &ContainerProperty );

		iResultCount = SearchByLocation( pPosition->GetLocation(), pResultList, E_ENTITY_TYPE_WORLD );

		if( iResultCount > 0 )
		{
			for( RwUInt32 iIndex = 0; iIndex < iResultCount; ++iIndex )
			{
				CEntity * pNewWorld = pResultList->GetAt( iIndex );

				if( pNewWorld != NULL && pNewWorld->GetType() == pParentEntity->GetType() && pNewWorld != pParentEntity )
				{
					CInstanceView::GetInstance()->GetDialog()->GetTreeCtrl()->UnregisterEntity( pEntity );

					pEntity->SetParent( pNewWorld );

					CInstanceView::GetInstance()->GetDialog()->GetTreeCtrl()->RegisterEntity( pEntity );

                    break;
				}
			}
		}

		g_pEntityFactory->DestroyEntityContainer( &pResultList );
*/
		CEntityContainer * pResultSecterList = g_pEntityFactory->CreateEntityContainer( &ContainerProperty );

		iResultCount = SearchByLocation( pPosition->GetLocation(), pResultSecterList, E_ENTITY_TYPE_WORLD_SECTOR );

		if( iResultCount > 0 )
		{
			//Assert( iResultCount == 1 );

			for( RwUInt32 iIndex = 0; iIndex < iResultCount; ++iIndex )
			{
				CEntity * pNewWorldSector = pResultSecterList->GetAt( iIndex );

				if( pNewWorldSector != NULL && pNewWorldSector->GetType() == pParentEntity->GetType() && pNewWorldSector != pParentEntity )
				{
					if( CInstanceTreeCtrl::GetValidInstance() != NULL )
					{
						CInstanceTreeCtrl::GetValidInstance()->UnregisterEntity( pEntity );
					}

					pEntity->SetParent( pNewWorldSector );

					if( CInstanceTreeCtrl::GetValidInstance() != NULL )
					{
						CInstanceTreeCtrl::GetValidInstance()->RegisterEntity( pEntity );
					}

                    break;
				}
			}
		}

		g_pEntityFactory->DestroyEntityContainer( &pResultSecterList );
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: GetEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CSceneManager::GetEntity( RwUInt32 iUniqueID ) const
{
	if( iUniqueID <= ENTITY_UNIQUE_ID_RESERVED )
	{
		switch( iUniqueID )
		{
			case ENTITY_UNIQUE_ID_BASE_WORLD:
			{
				return m_pBaseWorld;
			}

			case ENTITY_UNIQUE_ID_ACTIVE_CAMERA:
			{
				return m_pActiveCamera;
			}

			default:
			{
				return NULL;
			}
		}
	}
	else
	{
		if( m_pEntityTable != NULL )
		{
			return m_pEntityTable->GetEntity( iUniqueID );
		}
		else
		{
			return g_pEntityFactory->GetEntity( iUniqueID );
		}
	}
}


//-----------------------------------------------------------------------------
// Name			: ExecuteCommand
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::ExecuteCommand( RwChar * pCommandText, RwChar * pResultText )
{
	RwBool				bResult;
	CEntityCommand *	pCommand = CEntityCommand::CreateCommand( pCommandText );
	if( pCommand != NULL )
	{
		bResult = pCommand->Execute( pResultText );
		CEntityCommand::DestroyCommand( &pCommand );
	}
	else
	{
		bResult = FALSE;
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: SearchEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CSceneManager::SearchEntity( CEntityContainer * pResultContainer, const SEntitySearchCondition * pSearchCondition )
{
	RwUInt32		iCount = pResultContainer->GetCount();
	if( m_pWorldList != NULL )
	{
		m_pWorldList->SearchEntity( pResultContainer, pSearchCondition );
	}

	if( m_pCameraList != NULL )
	{
		m_pCameraList->SearchEntity( pResultContainer, pSearchCondition );
	}

	return pResultContainer->GetCount() - iCount;
}


//-----------------------------------------------------------------------------
// Name			: SearchByRwInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CSceneManager::SearchByRwInstance( void * pRwInstance ) const
{
	if( m_pWorldList != NULL )
	{
		return m_pWorldList->SearchByRwInstance( pRwInstance );
	}
	else
	{
		return NULL;
	}
}


//-----------------------------------------------------------------------------
// Name			: SearchByLocation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CSceneManager::SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultList, EEntityType eEntityType ) const
{
	if( m_pWorldList != NULL )
	{
		return m_pWorldList->SearchByLocation( pLocation, pResultList, eEntityType );
	}

	return 0;
}


//-----------------------------------------------------------------------------
// Name			: CalculateVertexColor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::CalculateVertexColor( SVertexColorGenerationInfo * pInfo )
{
	if( m_pWorldList == NULL )
	{
		return FALSE;
	}

	return m_pWorldList->CalculateVertexColor( pInfo );
}


//-----------------------------------------------------------------------------
// Name			: StartAnimation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CSceneManager::StartAnimation( RwReal fTime )
{
	RwUInt32		iCount = 0;

	if( m_pWorldList != NULL )
	{
		iCount += m_pWorldList->StartAnimation( fTime );
	}

	return iCount;
}



//-----------------------------------------------------------------------------
// Name			: StopAnimation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CSceneManager::StopAnimation( RwReal fTime )
{
	RwUInt32		iCount = 0;

	if( m_pWorldList != NULL )
	{
		iCount += m_pWorldList->StopAnimation( fTime );
	}

	return iCount;
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CSceneManager::Export( CFixedStringList * pStringList, CFixedString * pName )
{
	if( m_pWorldList == NULL )
	{
		return -1;
	}

	pStringList->Push( const_cast<CFixedString *>(GetName()) );

	RwChar				szFileName[256];
	rwsprintf( szFileName, RWSTRING( "%s\\%s" ), m_FilePath.GetBuffer(), pName->GetBuffer() );
	RwBool				bResult = CreateDirectory( szFileName, NULL );

	CFixedStringList	StringList;
	RwInt32				iTotalCount = 0;
	RwInt32				iCount;
	
	iCount = m_pWorldList->Export( &StringList, pName );
	if( iCount < 0 )
	{
		return iCount;
	}

	iTotalCount += iCount;

	iCount = m_pCameraList->Export( &StringList, pName );
	if( iCount < 0 )
	{
		return iCount;
	}

	iTotalCount += iCount;

	rwsprintf( szFileName, RWSTRING( "%s\\%s\\%s.%s" ), m_FilePath.GetBuffer(), g_pEngineApp->GetName()->GetBuffer(), m_Name.GetBuffer(), m_FileExt.GetBuffer() );

	HANDLE		hFile = CreateFileSafe( szFileName, false );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		DWORD	dwErrorCode = ::GetLastError();

		Assert( !"CreateFile() Failure" );

		return -1;
	}

	StringList.Serialize( hFile );

	CloseHandle( hFile );

	return 0;
}

//-----------------------------------------------------------------------------
// Name			: ReportState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::ReportState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	RwBool			bResult = TRUE;

	if( ReportSelfState( pLogMessageManager, iTab ) == FALSE )
	{
		bResult = FALSE;
	}

	if( ReportChildState( pLogMessageManager, iTab ) == FALSE )
	{
		bResult = FALSE;
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: ReportSelfState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::ReportSelfState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	RwChar		szBuffer[256];
	rwsprintf( szBuffer, RWSTRING( "Name : \"%s\"" ), m_Name.GetBuffer() );

	pLogMessageManager->SetLastLogMessageWithTab( szBuffer, iTab );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ReportChildState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	RwBool		bResult = TRUE;

	if( m_pWorldList != NULL )
	{
		if( m_pWorldList->ReportState( pLogMessageManager, iTab + 1 ) == FALSE )
		{
			bResult = FALSE;
		}
	}

	if( m_pCameraList != NULL )
	{
		if( m_pCameraList->ReportState( pLogMessageManager, iTab + 1 ) == FALSE )
		{
			bResult = FALSE;
		}
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: InsertEvent
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::InsertEvent( CEntityEvent * pEvent )
{
	if( m_pEventQueue != NULL )
	{
		return m_pEventQueue->Insert( pEvent );
	}
	else
	{
		return FALSE;
	}
}


//-----------------------------------------------------------------------------
// Name			: ResetRenderCount
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CSceneManager::ResetRenderCount( void )
{
	m_iRenderCountWorld			= 0L;
	m_iRenderCountWorldSector	= 0L;
	m_iRenderCountAtomic		= 0L;
	m_iRenderCountLight			= 0L;

	return;
}


//-----------------------------------------------------------------------------
// Name			: CreateLightMapManager
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSceneManager::CreateLightMapManager( void )
{
	if( GetBaseWorld() == NULL )
	{
		return FALSE;
	}

	if( m_pLightMapManager != NULL )
	{
		DestroyLightMapManager();
	}

	m_pLightMapManager = CLightMapManager::CreateInstance();

	return m_pLightMapManager->Create( GetBaseWorld() );
}


//-----------------------------------------------------------------------------
// Name			: DestroyLightMapManager
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CSceneManager::DestroyLightMapManager( void )
{
	if( m_pLightMapManager != NULL )
	{
		CLightMapManager::DestroyInstance();
		m_pLightMapManager = NULL;
	}

	return;
}


//*****************************************************************************
//
// End of File : CSceneManager.cpp
//
//*****************************************************************************