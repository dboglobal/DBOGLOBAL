//*****************************************************************************
// File       : CEntityPropertySortContainer.cpp
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
#include "CFixedStringList.h"

#include "CEntityPropertySortContainer.h"


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

void CEntityPropertySortContainer::Initialize( void )
{
	CEntityContainer::Initialize();

	m_ptHash = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityPropertySortContainer::Create( SEntityContainerProperty * pProperty )
{
	Assert( pProperty != NULL );
	Assert( pProperty->IsValid() != FALSE );

	m_Property.Assign( pProperty );

	if( m_ptHash != NULL )
	{
		Destroy();
	}

	m_ptHash = NTL_NEW TEntityHashContainer;
	m_ptHash->clear();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityPropertySortContainer::Destroy( void )
{
	if( m_ptHash != NULL )
	{
		RemoveAll( IsDeep() );

		NTL_DELETE( m_ptHash );
		m_ptHash = NULL;
	}

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

RwBool CEntityPropertySortContainer::UpdateFrame( void )
{
	if( m_ptHash == NULL )
	{
		return TRUE;
	}

	CEntityContainer *				pEntityContainerToUpdate;
	TEntityHashContainerIterator	tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityContainerToUpdate = (*tIterator).second;
		Assert( pEntityContainerToUpdate != NULL );
		++tIterator;

		if( pEntityContainerToUpdate->UpdateFrame() == FALSE )
		{
			return FALSE;
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

RwBool CEntityPropertySortContainer::RenderFrame( CEntity * pRenderer )
{
	if( m_ptHash == NULL )
	{
		return TRUE;
	}

	CEntityContainer *				pEntityContainerToRender;
	TEntityHashContainerIterator	tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityContainerToRender = (*tIterator).second;
		Assert( pEntityContainerToRender != NULL );
		++tIterator;

		if( pEntityContainerToRender->RenderFrame( pRenderer ) == FALSE )
		{
			return FALSE;
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

RwBool CEntityPropertySortContainer::RenderPrimitive( void )
{
	if( m_ptHash == NULL )
	{
		return TRUE;
	}

	CEntityContainer *				pEntityContainerToRender;
	TEntityHashContainerIterator	tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityContainerToRender = (*tIterator).second;
		Assert( pEntityContainerToRender != NULL );
		++tIterator;

		if( pEntityContainerToRender->RenderPrimitive() == FALSE )
		{
			return FALSE;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Select
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityPropertySortContainer::Select( void )
{
	RwUInt32	iSelectedCount = 0;

	if( m_ptHash == NULL )
	{
		return 0;
	}

	CEntityContainer *				pEntityContainerToSelect;
	TEntityHashContainerIterator	tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityContainerToSelect = (*tIterator).second;
		++tIterator;

		iSelectedCount += pEntityContainerToSelect->Select();
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

RwUInt32 CEntityPropertySortContainer::Unselect( void )
{
	RwUInt32	iUnselectedCount = 0;

	if( m_ptHash == NULL )
	{
		return 0;
	}

	CEntityContainer *				pEntityContainerToUnselect;
	TEntityHashContainerIterator	tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityContainerToUnselect = (*tIterator).second;
		++tIterator;

		iUnselectedCount += pEntityContainerToUnselect->Unselect();
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

RwUInt32 CEntityPropertySortContainer::StartAnimation( RwReal fTime )
{
	RwUInt32	iSelectedCount = 0;

	if( m_ptHash == NULL )
	{
		return 0;
	}

	CEntityContainer *				pEntityContainerToSelect;
	TEntityHashContainerIterator	tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityContainerToSelect = (*tIterator).second;
		++tIterator;

		iSelectedCount += pEntityContainerToSelect->StartAnimation( fTime );
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

RwUInt32 CEntityPropertySortContainer::StopAnimation( RwReal fTime )
{
	RwUInt32	iSelectedCount = 0;

	if( m_ptHash == NULL )
	{
		return 0;
	}

	CEntityContainer *				pEntityContainerToSelect;
	TEntityHashContainerIterator	tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityContainerToSelect = (*tIterator).second;
		++tIterator;

		iSelectedCount += pEntityContainerToSelect->StopAnimation( fTime );
	}

	return iSelectedCount;
}


//-----------------------------------------------------------------------------
// Name			: RemoveAll
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityPropertySortContainer::RemoveAll( RwBool bDeleteAll )
{
	if( bDeleteAll != FALSE )
	{
		CEntityContainer *				pEntityContainerToDestroyed;
		TEntityHashContainerIterator	tIterator = m_ptHash->begin();

		while( tIterator != m_ptHash->end() )
		{
			pEntityContainerToDestroyed = (*tIterator).second;
			Assert( pEntityContainerToDestroyed != NULL );
			++tIterator;

			g_pEntityFactory->DestroyEntityContainer( &pEntityContainerToDestroyed );
		}
	}

	m_ptHash->clear();
	m_iCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Insert
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityPropertySortContainer::Insert( CEntity * pEntityToInsert )
{
	if( m_ptHash == NULL )
	{
		Assert( !"Null m_ptHash" );

		return FALSE;
	}

	Assert( pEntityToInsert != NULL );

	CEntityContainer *		pContainer = GetEntityContainer( pEntityToInsert->GetProperty()->GetName()->GetKey() );

	if( pContainer == NULL )
	{
		SEntityContainerProperty	sContainerProperty( E_ENTITY_CONTAINER_TYPE_HASH_MAP, false, pEntityToInsert );

		pContainer = g_pEntityFactory->CreateEntityContainer( &sContainerProperty );

		m_ptHash->insert( TEntityHashContainerPair( pEntityToInsert->GetProperty()->GetName()->GetKey(), pContainer ) );
	}

	if(  pContainer->Insert( pEntityToInsert ) != FALSE )
	{
		++m_iCount;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//-----------------------------------------------------------------------------
// Name			: Remove
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityPropertySortContainer::Remove( CEntity * pEntityToRemove )
{
	if( m_ptHash == NULL )
	{
		Assert( !"Null m_ptHash" );

		return FALSE;
	}

	Assert( pEntityToRemove != NULL );

	CEntityContainer *		pContainer = GetEntityContainer( pEntityToRemove->GetProperty()->GetName()->GetKey() );

	if( pContainer == NULL )
	{
		return FALSE;
	}

	if( pContainer->Remove( pEntityToRemove ) != FALSE )
	{
		--m_iCount;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//-----------------------------------------------------------------------------
// Name			: Replace
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityPropertySortContainer::Replace( CEntity * pEntityToRemove, CEntity * pEntityToInsert )
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
// Name			: SetFlag
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityPropertySortContainer::SetFlag( RwUInt32 iFlag, RwBool bApplyToChild )
{
	if( m_ptHash == NULL )
	{
		return;
	}

	CEntityContainer *		pEntityContainer;

	for( TEntityHashContainerIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntityContainer = (*tIterator).second;

		if( pEntityContainer != NULL )
		{
			pEntityContainer->SetFlag( iFlag, bApplyToChild );
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

void CEntityPropertySortContainer::SetState( EEntityState eState, RwBool bApplyToChild )
{
	if( m_ptHash == NULL )
	{
		return;
	}

	CEntityContainer *		pEntityContainer;

	for( TEntityHashContainerIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntityContainer = (*tIterator).second;

		if( pEntityContainer != NULL )
		{
			pEntityContainer->SetState( eState, bApplyToChild );
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityPropertySortContainer::SetPosition( const SPosition3D * pPosition, RwBool bAdded, RwBool bApplyToChild )
{
	if( m_ptHash == NULL )
	{
		return;
	}

	CEntityContainer *		pEntityContainer;

	for( TEntityHashContainerIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntityContainer = (*tIterator).second;

		if( pEntityContainer != NULL )
		{
			pEntityContainer->SetPosition( pPosition, bAdded, bApplyToChild );
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetScale
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityPropertySortContainer::SetScale( const SLocation3D * pScale, RwBool bAdded, RwBool bApplyToChild )
{
	if( m_ptHash == NULL )
	{
		return;
	}

	CEntityContainer *		pEntityContainer;

	for( TEntityHashContainerIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntityContainer = (*tIterator).second;

		if( pEntityContainer != NULL )
		{
			pEntityContainer->SetScale( pScale, bAdded, bApplyToChild );
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

CEntity * CEntityPropertySortContainer::GetEntity( RwUInt32 iUniqueID )
{
	CEntity *			pEntity;
	CEntityContainer *	pEntityContainer;

	for( TEntityHashContainerIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntityContainer = (*tIterator).second;

		if( pEntityContainer != NULL )
		{
			pEntity = pEntityContainer->GetEntity( iUniqueID );
			if( pEntity != NULL )
			{
				return pEntity;
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

CEntityContainer * CEntityPropertySortContainer::GetEntityContainer( RwUInt32 iKey )
{
	if( m_ptHash == NULL )
	{
		return NULL;
	}

	TEntityHashContainerIterator	tIterator = m_ptHash->find( iKey );

	if( tIterator == m_ptHash->end() )
	{
		return NULL;
	}

	return (*tIterator).second;
}


//-----------------------------------------------------------------------------
// Name			: GetAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CEntityPropertySortContainer::GetAt( RwUInt32 iOrder )
{
	if( m_ptHash == NULL )
	{
		return NULL;
	}

	CEntityContainer *	pEntityContainer;

	for( TEntityHashContainerIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntityContainer = (*tIterator).second;

		if( pEntityContainer != NULL )
		{
			if( iOrder < pEntityContainer->GetCount() )
			{
				return pEntityContainer->GetAt( iOrder );
			}
			else
			{
				iOrder -= pEntityContainer->GetCount();
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

RwUInt32 CEntityPropertySortContainer::SearchEntity( CEntityContainer * pResultContainer, const SEntitySearchCondition * pSearchCondition )
{
	if( m_ptHash == NULL )
	{
		return 0;
	}

	RwUInt32			iCount = pResultContainer->GetCount();
	CEntityContainer *	pEntityContainer;

	for( TEntityHashContainerIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntityContainer = (*tIterator).second;

		if( pEntityContainer != NULL )
		{
			pEntityContainer->SearchEntity( pResultContainer, pSearchCondition );
		}
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

CEntity * CEntityPropertySortContainer::SearchByRwInstance( void * pRwInstance )
{
	if( m_ptHash == NULL )
	{
		return NULL;
	}

	CEntityContainer *	pEntityContainer;
	CEntity *			pMatchEntity;

	for( TEntityHashContainerIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntityContainer = (*tIterator).second;

		if( pEntityContainer != NULL )
		{
			pMatchEntity = pEntityContainer->SearchByRwInstance( pRwInstance );
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

RwUInt32 CEntityPropertySortContainer::SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultContainer, EEntityType eEntityType ) //const
{
	if( m_ptHash == NULL )
	{
		return 0;
	}

	RwUInt32			iCount = pResultContainer->GetCount();
	CEntityContainer *	pEntityContainer;

	for( TEntityHashContainerIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntityContainer = (*tIterator).second;

		if( pEntityContainer != NULL )
		{
			pEntityContainer->SearchByLocation( pLocation, pResultContainer, eEntityType );
		}
	}

	return pResultContainer->GetCount() - iCount;
}


//-----------------------------------------------------------------------------
// Name			: CalculateVertexColor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

 RwBool CEntityPropertySortContainer::CalculateVertexColor( SVertexColorGenerationInfo * pInfo )
 {
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwBool				bResult = TRUE;
	CEntityContainer *	pEntityContainer;

	for( TEntityHashContainerIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntityContainer = (*tIterator).second;

		if( pEntityContainer != NULL )
		{
			if( pEntityContainer->CalculateVertexColor( pInfo ) == FALSE )
			{
				bResult = FALSE;
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

 RwBool CEntityPropertySortContainer::DetectCollision( CCollisionDetectionInfo * pCollisionInfo )
 {
 #ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwBool				bResult = TRUE;
	CEntityContainer *	pEntityContainer;

	for( TEntityHashContainerIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntityContainer = (*tIterator).second;

		if( pEntityContainer != NULL )
		{
			if( pEntityContainer->DetectCollision( pCollisionInfo ) == FALSE )
			{
				bResult = FALSE;
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

RwInt32 CEntityPropertySortContainer::Export( CFixedStringList * pStringList, CFixedString * pName )
{
	if( m_ptHash == NULL )
	{
		return 0;
	}

	RwInt32			iTotalCount = pStringList->GetCount();
	RwInt32			iExportCount;
	CEntityContainer *	pEntityContainer;

	for( TEntityHashContainerIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntityContainer = (*tIterator).second;

		if( pEntityContainer != NULL )
		{
			iExportCount = pEntityContainer->Export( pStringList, pName );
			if( iExportCount < 0 )
			{
				return iExportCount;
			}
		}
	}

	return pStringList->GetCount() - iTotalCount;
}


//-----------------------------------------------------------------------------
// Name			: ReportSelfState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityPropertySortContainer::ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	if( m_ptHash == NULL )
	{
		return FALSE;
	}

	RwBool				bResult = TRUE;
	CEntityContainer *	pEntityContainer;

	for( TEntityHashContainerIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntityContainer = (*tIterator).second;

		if( pEntityContainer != NULL )
		{
			if( pEntityContainer->ReportState( pLogMessageManager, iTab ) == FALSE )
			{
				bResult = FALSE;
			}
		}
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: GetPropertyKeyFromEntity
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityPropertySortContainer::GetPropertyKeyFromEntity( CEntity * pEntity )
{
	if( pEntity == NULL )
	{
		return 0L;
	}

	CEntityProperty *	pEntityProperty = pEntity->GetProperty();
	if( pEntityProperty == NULL )
	{
		return 0L;
	}

	return pEntityProperty->GetName()->GetKey();
}


//*****************************************************************************
//
// End of File : CEntityPropertySortContainer.cpp
//
//*****************************************************************************