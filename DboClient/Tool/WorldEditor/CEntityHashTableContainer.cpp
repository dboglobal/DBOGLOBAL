//*****************************************************************************
// File       : CEntityHashTableContainer.cpp
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
#include "CEntityFactory.h"

#include "CEntityHashTableContainer.h"


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

void CEntityHashTableContainer::Initialize( void )
{
	CEntityContainer::Initialize();

	m_pcHash = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityHashTableContainer::Create( SEntityContainerProperty * pProperty )
{
	Assert( pProperty != NULL );
	Assert( pProperty->IsValid() != FALSE );

	m_Property.Assign( pProperty );

	if( m_pcHash != NULL )
	{
		Destroy();
	}

	m_pcHash = NTL_NEW CHashTable( 65536 );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityHashTableContainer::Destroy( void )
{
	NTL_DELETE( m_pcHash );
	m_pcHash = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: UpdateFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityHashTableContainer::UpdateFrame( void )
{
	Assert( !"Not Supported" );

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityHashTableContainer::RenderFrame( CEntity * pRenderer )
{
	Assert( !"Not Supported" );

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: Select
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityHashTableContainer::Select( void )
{
	Assert( !"Not Supported" );

	return 0;
}


//-----------------------------------------------------------------------------
// Name			: Unselect
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityHashTableContainer::Unselect( void )
{
	Assert( !"Not Supported" );

	return 0;
}


//-----------------------------------------------------------------------------
// Name			: StartAnimation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityHashTableContainer::StartAnimation( RwReal fTime )
{
	Assert( !"Not Supported" );

	return 0;
}


//-----------------------------------------------------------------------------
// Name			: StopAnimation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityHashTableContainer::StopAnimation( RwReal fTime )
{
	Assert( !"Not Supported" );

	return 0;
}


//-----------------------------------------------------------------------------
// Name       : RemoveAll
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

void CEntityHashTableContainer::RemoveAll( RwBool bDeleteAll )
{
	Assert( !"Not Supported" );

	return;
}


//-----------------------------------------------------------------------------
// Name       : Insert
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CEntityHashTableContainer::Insert( CEntity * pEntityToInsert )
{
	if( m_pcHash == NULL )
	{
		Assert( !"Null m_pcHash" );

		return FALSE;
	}

	Assert( pEntityToInsert != NULL );

	m_pcHash->Insert( pEntityToInsert->GetUniqueID(), pEntityToInsert );
	++m_iCount;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name       : Remove
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CEntityHashTableContainer::Remove( CEntity * pEntityToRemove )
{
	if( m_pcHash == NULL )
	{
		Assert( !"Null m_pcHash" );

		return FALSE;
	}

	Assert( pEntityToRemove != NULL );

	m_pcHash->Remove( pEntityToRemove->GetUniqueID() );
	--m_iCount;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name       : Replace
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CEntityHashTableContainer::Replace( CEntity * pEntityToRemove, CEntity * pEntityToInsert )
{
	if( Remove( pEntityToRemove ) == FALSE )
	{
		return FALSE;
	}

	if( Insert( pEntityToInsert ) == FALSE )
	{
		Insert( pEntityToRemove );

		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name       : SetFlag
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

void CEntityHashTableContainer::SetFlag( RwUInt32 iFlag, RwBool bApplyToChild )
{
	Assert( !"Not Supported" );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityHashTableContainer::SetState( EEntityState eState, RwBool bApplyToChild )
{
	Assert( !"Not Supported" );

	return;
}


//-----------------------------------------------------------------------------
// Name       : SetPosition
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

void CEntityHashTableContainer::SetPosition( const SPosition3D * pPosition, RwBool bAdded, RwBool bApplyToChild )
{
	Assert( !"Not Supported" );

	return;
}


//-----------------------------------------------------------------------------
// Name       : SetScale
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

void CEntityHashTableContainer::SetScale( const SLocation3D * pScale, RwBool bAdded, RwBool bApplyToChild )
{
	Assert( !"Not Supported" );

	return;
}


//-----------------------------------------------------------------------------
// Name       : GetEntity
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

CEntity * CEntityHashTableContainer::GetEntity( RwUInt32 iUniqueID )
{
	if( m_pcHash == NULL )
	{
		Assert( !"Null m_pcHash" );

		return NULL;
	}

	return reinterpret_cast<CEntity *>(m_pcHash->GetData( iUniqueID ));
}


//-----------------------------------------------------------------------------
// Name       : GetAt
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

CEntity * CEntityHashTableContainer::GetAt( RwUInt32 iOrder )
{
	Assert( !"Not Supported" );

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: SearchEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityHashTableContainer::SearchEntity( CEntityContainer * pResultContainer, const SEntitySearchCondition * pSearchCondition )
{
	Assert( !"Not Supported" );

	return 0L;
}


//-----------------------------------------------------------------------------
// Name			: SearchByRwInstance
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CEntityHashTableContainer::SearchByRwInstance( void * pRwInstance )
{
	Assert( !"Not Supported" );

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: SearchByLocation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityHashTableContainer::SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultHash, EEntityType eEntityType ) //const
{
	Assert( !"Not Supported" );

	return 0L;
}


//-----------------------------------------------------------------------------
// Name			: DetectCollision
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

 RwBool CEntityHashTableContainer::DetectCollision( CCollisionDetectionInfo * pCollisionInfo )
 {
	Assert( !"Not Supported" );

	return FALSE;
 }

 
//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CEntityHashTableContainer::Export( CFixedStringList * pStringList, CFixedString * pName )
{
	Assert( !"Not Supported" );

	return 0L;
}


//-----------------------------------------------------------------------------
// Name			: ReportSelfState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityHashTableContainer::ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	Assert( !"Not Supported" );

	return FALSE;
}


//*****************************************************************************
//
// End of File : CEntityHashTableContainer.cpp
//
//*****************************************************************************