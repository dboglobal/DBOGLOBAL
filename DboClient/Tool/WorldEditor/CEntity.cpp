//*****************************************************************************
// File       : CEntity.cpp
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

#include "CEntityProperty.h"
#include "CEntityResource.h"
#include "CEntityCommand.h"
#include "CEntityContainer.h"

#include "CRenderManager.h"
#include "CSceneManager.h"
#include "CEntityFactory.h"
#include "CEntityResourceManager.h"

#include "CEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const CFixedString CEntity::ENTITY_CATEGORY_NAME[] =
{
	CFixedString( RWSTRING( "normal" ) ),
	CFixedString( RWSTRING( "internal" ) ),
	CFixedString( RWSTRING( "helper" ) ),
	CFixedString( RWSTRING( "external" ) ),
};


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntity::Initialize( void )
{
	m_eCategory			= E_ENTITY_CATEGORY_NORMAL;

	m_iRenderPriority	= ENTITY_RENDER_PRIORITY_NONE;

	m_pProperty			= NULL;
	m_pResource			= NULL;
	m_pParent			= NULL;

	SetMovable( FALSE );
	SetVisible( FALSE );
	SetGlobal( TRUE );

	m_bSelected			= FALSE;
	m_iModified			= ENTITY_MODIFIED_NONE;

	m_iFlag				= 0L;
	m_iFrameID			= 0L;

	m_iReferenceCount	= 0L;
	m_pLinkedParent		= NULL;

	ZeroMemory( &m_sBounding, sizeof( RwSphere ) );

	InitializeChildren();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty )
{
	Assert( pEntityProperty != NULL );
	Assert( pEntityProperty->IsValid() != FALSE );

	Assert( pCreateProperty != NULL );
	Assert( pCreateProperty->IsValid() != FALSE );

	// Set Property

	m_pProperty = pEntityProperty->Clone();
	m_eCategory = pCreateProperty->GetCategory();
	m_iRenderPriority = pCreateProperty->GetRenderPriority();

	SetGlobal( m_pProperty->IsGlobal() );

	if( pCreateProperty->GetName()->GetLength() > 0 )
	{
		SetName( *(pCreateProperty->GetName()) );
	}
	else
	{
		SetDefaultName();
	}

	// Load Resource

	m_pResource = CEntityResourceManager::GetInstance()->CreateResource( pEntityProperty );
	if( m_pResource == NULL )
	{
		Destroy();

		return FALSE;
	}

	// Set Parameter

	if( SetParameter() == FALSE )
	{
		Destroy();

		return FALSE;
	}

	SetTerrain( pEntityProperty->IsTerrain() );
	SetSky( pEntityProperty->IsSky() );

	// Link Parent

	CEntity *	pParentEntity = g_pEntityFactory->GetEntity( pCreateProperty->iParentID );

	SetParent( pParentEntity );
 
	IncreaseReferenceCount();

	SetPlugInData();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: PostCreate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::PostCreate( void )
{
	CalculateBounding();

	SetVisible(	DetermineVisiblilty() );

	SetModified( ENTITY_MODIFIED_ALL );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntity::Destroy( void )
{
	DecreaseReferenceCount();

	// Unlink Parent

	SetParent( NULL );

	ResetLinkedParent();

	// Unload Resource

	CEntityResourceManager::GetInstance()->DestroyResource( &m_pResource );

	// Destroy Property

	if( m_pProperty != NULL )
	{
		NTL_DELETE( m_pProperty );
		m_pProperty = NULL;
	}

	SetName( NULL );

	return;
}


//-----------------------------------------------------------------------------
// Name			: PreDestroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntity::PreDestroy( void )
{
	DestroyChildren();

	return;
}


//-----------------------------------------------------------------------------
// Name			: UpdateFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::UpdateFrame( void )
{
	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
	{
		if( m_pChildren[iCategoryIndex] != NULL )
		{
			if( m_pChildren[iCategoryIndex]->UpdateFrame() == FALSE )
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::RenderFrame( CEntity * pRenderer )
{
	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
	{
		if( m_pChildren[iCategoryIndex] != NULL )
		{
			if( m_pChildren[iCategoryIndex]->RenderFrame( pRenderer ) == FALSE )
			{
				return FALSE;
			}
		}
	}

	ResetModified();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: InsertRenderManager
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::InsertRenderManager( void )
{
	if( g_pSceneManager->GetRenderManager() != NULL )
	{
		return g_pSceneManager->GetRenderManager()->InsertEntity( this );
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: Select
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntity::Select( void )
{
	m_bSelected = TRUE;
	RwUInt32	iSelectedCount = 1;

	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
	{
		if( m_pChildren[iCategoryIndex] != NULL )
		{
			iSelectedCount += m_pChildren[iCategoryIndex]->Select();
		}
	}

	SetModified( ENTITY_MODIFIED_SELECT );

	return iSelectedCount;
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: Unselect
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntity::Unselect( void )
{
	m_bSelected = FALSE;
	RwUInt32	iUnselectedCount = 1;

	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
	{
		if( m_pChildren[iCategoryIndex] != NULL )
		{
			iUnselectedCount += m_pChildren[iCategoryIndex]->Unselect();
		}
	}

	SetModified( ENTITY_MODIFIED_SELECT );

	return iUnselectedCount;
}


//-----------------------------------------------------------------------------
// Name			: StartAnimation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntity::StartAnimation( RwReal fTime )
{
	RwUInt32	iStartCount = 0;

	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
	{
		if( m_pChildren[iCategoryIndex] != NULL )
		{
			iStartCount += m_pChildren[iCategoryIndex]->StartAnimation( fTime );
		}
	}

	return iStartCount;
}


//-----------------------------------------------------------------------------
// Name			: StopAnimation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntity::StopAnimation( RwReal fTime )
{
	RwUInt32	iStopCount = 0;

	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
	{
		if( m_pChildren[iCategoryIndex] != NULL )
		{
			iStopCount += m_pChildren[iCategoryIndex]->StopAnimation( fTime );
		}
	}

	return iStopCount;
}


//-----------------------------------------------------------------------------
// Name			: GetConstRwInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const void * CEntity::GetConstRwInstance( void ) const
{
	return ( ( m_pResource == NULL ) ? NULL : reinterpret_cast<const void *>(m_pResource->GetInstance()) );
}


//-----------------------------------------------------------------------------
// Name			: SetDefaultName
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntity::SetDefaultName( void )
{
	TCHAR		tName[256];

	rwsprintf( tName, "%s%d", GetTypeName()->GetBuffer(), GetUniqueID() & 0x00FFFFFF );

	m_sName.Assign( tName );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetParameter
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::SetParameter( void )
{
	SetGlobal( m_pProperty->IsGlobal() );

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
// Name			: SetFlag
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntity::SetFlag( RwUInt32 iFlag, RwBool bApplyToChild )
{
	m_iFlag = iFlag;

	if( bApplyToChild != FALSE )
	{
		for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
		{
			if( m_pChildren[iCategoryIndex] != NULL )
			{
				m_pChildren[iCategoryIndex]->SetFlag( iFlag, bApplyToChild );
			}
		}
	}

	SetModified( ENTITY_MODIFIED_FLAG );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntity::SetState( EEntityState eState, RwBool bApplyToChild )
{
	m_eState = eState;

	if( bApplyToChild != FALSE )
	{
		for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
		{
			if( m_pChildren[iCategoryIndex] != NULL )
			{
				m_pChildren[iCategoryIndex]->SetState( eState, bApplyToChild );
			}
		}
	}

	SetModified( ENTITY_MODIFIED_STATE );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntity::SetPosition( const SPosition3D * pPosition, RwBool bAdded, RwBool bApplyToChild )
{
	if( m_pProperty != NULL && m_pProperty->HasPosition() != FALSE )
	{
		if( bAdded != FALSE )
		{
			if( pPosition->IsEqual( SPosition3D::POSITION_DEFAULT ) != FALSE )
			{
				return;
			}

			SPosition3D		NewPosition;

			NewPosition.Set( *(m_pProperty->GetPosition()) );
			NewPosition.Add( *pPosition );

			m_pProperty->SetPosition( &NewPosition );

			SetModified( ENTITY_MODIFIED_POSITION );
		}
		else
		{
			if( pPosition->IsEqual( *m_pProperty->GetPosition() ) == FALSE )
			{	
				m_pProperty->SetPosition( pPosition );

				SetModified( ENTITY_MODIFIED_POSITION );
			}
		}
	}

	if( bApplyToChild != FALSE )
	{
		for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
		{
			if( m_pChildren[iCategoryIndex] != NULL )
			{
				m_pChildren[iCategoryIndex]->SetPosition( pPosition, bAdded, bApplyToChild );
			}
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetScale
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntity::SetScale( const SLocation3D * pScale, RwBool bAdded, RwBool bApplyToChild )
{
	if( m_pProperty != NULL && m_pProperty->HasScale() != FALSE )
	{
		if( bAdded != FALSE )
		{
			if( pScale->IsEqual( SLocation3D::SCALE_DEFAULT ) != FALSE )
			{
				return;
			}

			SLocation3D		NewScale;

			NewScale.Set( *(m_pProperty->GetScale()) );
			NewScale.Scale( *pScale );

			m_pProperty->SetScale( &NewScale );

			SetModified( ENTITY_MODIFIED_SCALE );
		}
		else
		{
			if( pScale->IsEqual( *m_pProperty->GetScale() ) == FALSE )
			{
				m_pProperty->SetScale( pScale );

				SetModified( ENTITY_MODIFIED_SCALE );
			}
		}
	}

	if( bApplyToChild != FALSE )
	{
		for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
		{
			if( m_pChildren[iCategoryIndex] != NULL )
			{
				m_pChildren[iCategoryIndex]->SetScale( pScale, bAdded, bApplyToChild );
			}
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetParent
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntity::SetParent( CEntity * pParent )
{
	if( GetParent() == pParent )
	{
		return;
	}

	if( GetParent() != NULL )
	{
		if( GetParent()->Detach( this ) == FALSE )
		{
			Assert( !"Detach Fail" );
		}
	}

	SetParentForce( NULL );

	if( pParent != NULL )
	{
		if( pParent->Attach( this ) == FALSE )
		{
			Assert( !"Attach Fail" );
		}
	}

	if( m_pParent == NULL )
	{
		SetParentForce( pParent );
	}

	SetModified( ENTITY_MODIFIED_HIERARCHY );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetParentWorld
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const CEntity * CEntity::GetParentWorld( void ) const
{
	if( GetParent() == NULL )
	{
		return NULL;
	}

	if( GetParent()->GetType() == E_ENTITY_TYPE_WORLD )
	{
		return m_pParent;
	}

	return GetParent()->GetParentWorld();
}


//-----------------------------------------------------------------------------
// Name			: SearchEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntity::SearchEntity( CEntityContainer * pResultContainer, const SEntitySearchCondition * pSearchCondition )
{
	RwUInt32		iCount = pResultContainer->GetCount();

	if( IsEntityTypeInFilter( GetType(), pSearchCondition->GetTypeFilter() ) != FALSE )
	{
		if( IsEntityCategoryInFilter( GetCategory(), pSearchCondition->GetCategoryFilter() ) != FALSE )
		{
			pResultContainer->Insert( this );
		}
	}

	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
	{
		if( m_pChildren[iCategoryIndex] != NULL )
		{
			m_pChildren[iCategoryIndex]->SearchEntity( pResultContainer, pSearchCondition );
		}
	}

	return pResultContainer->GetCount() - iCount;
}


//-----------------------------------------------------------------------------
// Name			: InitializeChildren
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntity::InitializeChildren( void )
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

void CEntity::DestroyChildren( void )
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

RwBool CEntity::InsertChild( CEntity * pChild )
{
	Assert( pChild != NULL );

	EEntityCategory eChildCategory = pChild->GetCategory();

	Assert( eChildCategory >= E_ENTITY_CATEGORY_NORMAL );
	Assert( eChildCategory < E_ENTITY_CATEGORY_COUNT );

	if( m_pChildren[eChildCategory] == NULL )
	{
		SEntityContainerProperty		ContainerProperty( E_ENTITY_CONTAINER_TYPE_LIST, TRUE, this, g_pSceneManager );
		m_pChildren[eChildCategory] = CEntityFactory::GetInstance()->CreateEntityContainer( &ContainerProperty );
	}

	if( m_pChildren[eChildCategory]->Insert( pChild ) == FALSE )
	{
		return FALSE;
	}

	CalculateBounding();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RemoveChild
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::RemoveChild( CEntity * pChild )
{
	Assert( pChild != NULL );

	EEntityCategory eChildCategory = pChild->GetCategory();

	Assert( eChildCategory >= E_ENTITY_CATEGORY_NORMAL );
	Assert( eChildCategory < E_ENTITY_CATEGORY_COUNT );

	if( m_pChildren[eChildCategory] == NULL )
	{
		Assert( !"Null Child List" );

		return NULL;
	}

	if( m_pChildren[eChildCategory]->Remove( pChild ) == FALSE )
	{
		return FALSE;
	}

	CalculateBounding();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CEntity::Export( CFixedStringList * pStringList, CFixedString * pName )
{
	if( GetCategory() == E_ENTITY_CATEGORY_INTERNAL )
	{
		return ExportChild( pStringList, pName );
	}

	if( ExportHeader( pStringList, pName ) == FALSE )
	{
		return -1;
	}

	RwInt32			nExportCount = ExportChild( pStringList, pName );
	if( nExportCount < 0 )
	{
		return nExportCount;
	}

	return ++nExportCount;
}


//-----------------------------------------------------------------------------
// Name			: ExportHeader
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::ExportHeader( CFixedStringList * pStringList, CFixedString * pName ) const
{
	CFixedString	String;

	String.Assign( RWSTRING( "#create" ) );
	pStringList->Push( &String );

	// UniqueID

	String.AssignInteger( GetUniqueID(), true );
	pStringList->Push( &String );

	// ParentID

	if( GetParent() != NULL )
	{
		String.AssignInteger( GetParent()->GetUniqueID(), true );
	}
	else
	{
		String.AssignInteger( 0L, true );
	}

	pStringList->Push( &String );

	// Category

	String.AssignInteger( GetCategory() );
	pStringList->Push( &String );

	// TypeName

	String.Assign( *GetTypeName() );
	pStringList->Push( &String );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ExportChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CEntity::ExportChild( CFixedStringList * pStringList, CFixedString * pName )
{
	RwInt32			iTotalExportCount = 0;
	RwInt32			iCategoryExportCount;

	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
	{
		if( m_pChildren[iCategoryIndex] != NULL )
		{
			iCategoryExportCount = m_pChildren[iCategoryIndex]->Export( pStringList, pName );

			if( iCategoryExportCount < 0 )
			{
				return iCategoryExportCount;
			}

			iTotalExportCount += iCategoryExportCount;
		}
	}

	return iTotalExportCount;
}


//-----------------------------------------------------------------------------
// Name			: SearchByRwInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CEntity::SearchByRwInstance( void * pRwInstance ) const
{
	if( GetRwInstance() == pRwInstance )
	{
		return const_cast<CEntity *>(this);
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

RwUInt32 CEntity::SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultList, EEntityType eEntityType ) const
{
	RwUInt32		iMatchCount = 0;

	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_EXTERNAL; ++iCategoryIndex )
	{
		if( m_pChildren[iCategoryIndex] != NULL )
		{
			iMatchCount += m_pChildren[iCategoryIndex]->SearchByLocation( pLocation, pResultList, eEntityType );
		}
	}

	return iMatchCount;
}


//-----------------------------------------------------------------------------
// Name			: DetectCollision
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::DetectCollision( CCollisionDetectionInfo * pCollisionInfo ) const
{
	RwBool		bResult = TRUE;

	for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_EXTERNAL; ++iCategoryIndex )
	{
		if( m_pChildren[iCategoryIndex] != NULL )
		{
			if( m_pChildren[iCategoryIndex]->DetectCollision( pCollisionInfo ) == FALSE )
			{
				bResult = FALSE;
			}
		}
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: IsValid
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::IsValid( void ) const
{
	if( m_pProperty == NULL )
	{
		return FALSE;
	}

	if( m_pResource == NULL )
	{
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ReportState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::ReportState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
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

RwBool CEntity::ReportSelfState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	RwChar		szBuffer[256];
	rwsprintf( szBuffer, RWSTRING( "Id : 0x%08X, Name : \"%s\", Category : %d, Parent : 0x%08X, Mov : %d, Vis : %d, Glb : %d" ),
				m_iUniqueID, m_sName.GetBuffer(), m_eCategory,
				( ( m_pParent == NULL ) ? 0L : m_pParent->GetUniqueID() ),
				IsMovable(), IsVisible(), IsGlobal() );

	pLogMessageManager->SetLastLogMessageWithTab( szBuffer, iTab );

//	CWorldMonitor::CreateInstance( pLogMessageManager );
//	CWorldMonitor::GetInstance()->AnalysisObject( GetRwInstance() );
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

RwBool CEntity::ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	RwBool		bResult = TRUE;

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

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: GetTypeName
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const CFixedString * CEntity::GetTypeName( void ) const
{
	if( m_pProperty == NULL )
	{
		return CEntityProperty::GetEntityTypeName( E_ENTITY_TYPE_NONE );
	}

	return m_pProperty->GetTypeName();
}


//-----------------------------------------------------------------------------
// Name			: GetCategoryName
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const CFixedString * CEntity::GetCategoryName( void ) const
{
	return GetEntityCategoryName( GetCategory() );
}


//-----------------------------------------------------------------------------
// Name			: GetEntityCategoryName
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const CFixedString * CEntity::GetEntityCategoryName( EEntityCategory eCategory )
{
	if( eCategory < E_ENTITY_CATEGORY_NORMAL )
	{
		return &ENTITY_CATEGORY_NAME[E_ENTITY_CATEGORY_NORMAL];
	}

	if( eCategory >= E_ENTITY_CATEGORY_COUNT )
	{
		return &ENTITY_CATEGORY_NAME[E_ENTITY_CATEGORY_NORMAL];
	}

	return &ENTITY_CATEGORY_NAME[eCategory];
}


//-----------------------------------------------------------------------------
// Name			: Invalidate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::Invalidate( void ) const
{
	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: IsLinkedParent
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::IsLinkedParent( RwUInt32 iParentID )
{
	if( m_pLinkedParent == NULL )
	{
		return FALSE;
	}

	for( TEntityUniqueIDListIterator tIterator = m_pLinkedParent->begin(); tIterator != m_pLinkedParent->end(); ++tIterator )
	{
		if( *tIterator == iParentID )
		{
			return TRUE;
		}
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: LinkParent
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::LinkParent( RwUInt32 iParentID )
{
	if( GetParentID() == iParentID )
	{
		return FALSE;
	}

	if( m_pLinkedParent == NULL )
	{
		m_pLinkedParent = NTL_NEW TEntityUniqueIDList;

		Assert( m_pLinkedParent != NULL );
	}
	else
	{
		if( IsLinkedParent( iParentID ) != FALSE )
		{
			return FALSE;
		}
	}

	m_pLinkedParent->push_back( iParentID );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RedirectParent
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntity::RedirectParent( RwBool bLinkCurrentParent )
{
	if( m_pLinkedParent == NULL )
	{
		return FALSE;
	}

	CEntity *		pNewParent;

	for( TEntityUniqueIDListIterator tIterator = m_pLinkedParent->begin(); tIterator != m_pLinkedParent->end(); ++tIterator )
	{
		pNewParent = g_pSceneManager->GetEntity( *tIterator );

		if( pNewParent != NULL )
		{
			if( bLinkCurrentParent != FALSE && GetParent() != NULL )
			{
				LinkParent( GetParentID() );
			}

			SetParent( pNewParent );

			return TRUE;
		}
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: ResetLinkedParent
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntity::ResetLinkedParent( void )
{
	if( m_pLinkedParent == NULL )
	{
		return;
	}

	m_pLinkedParent->clear();

	NTL_DELETE( m_pLinkedParent );
	m_pLinkedParent = NULL;

	return;
}


//*****************************************************************************
//
// End of File : CEntity.cpp
//
//*****************************************************************************