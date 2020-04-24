//*****************************************************************************
// File       : CEntityListContainer.cpp
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

#include "CEntityListContainer.h"


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

void CEntityListContainer::Initialize( void )
{
	CEntityContainer::Initialize();

	m_ptList = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityListContainer::Create( SEntityContainerProperty * pProperty )
{
	Assert( pProperty != NULL );
	Assert( pProperty->IsValid() != FALSE );

	m_Property.Assign( pProperty );

	if( m_ptList != NULL )
	{
		Destroy();
	}

	m_ptList = NTL_NEW TEntityList;
	m_ptList->clear();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityListContainer::Destroy( void )
{
	if( m_ptList != NULL )
	{
		RemoveAll( IsDeep() );

		NTL_DELETE( m_ptList );
		m_ptList = NULL;
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

RwBool CEntityListContainer::UpdateFrame( void )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	CEntity *				pEntityToUpdate;
	TEntityListIterator		tIterator = m_ptList->begin();

	while( tIterator != m_ptList->end() )
	{
		pEntityToUpdate = *tIterator;
		Assert( pEntityToUpdate != NULL );
		++tIterator;

		if( pEntityToUpdate->UpdateFrame() == FALSE )
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

RwBool CEntityListContainer::RenderFrame( CEntity * pRenderer )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	CEntity *				pEntityToRender;
	TEntityListIterator		tIterator = m_ptList->begin();

	while( tIterator != m_ptList->end() )
	{
		pEntityToRender = *tIterator;
		Assert( pEntityToRender != NULL );
		++tIterator;

		if( pEntityToRender->RenderFrame( pRenderer ) == FALSE )
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

RwBool CEntityListContainer::RenderPrimitive( void )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	CEntity *				pEntityToRender;
	TEntityListIterator		tIterator = m_ptList->begin();

	while( tIterator != m_ptList->end() )
	{
		pEntityToRender = *tIterator;
		Assert( pEntityToRender != NULL );
		++tIterator;

		if( pEntityToRender->RenderPrimitive() == FALSE )
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

RwUInt32 CEntityListContainer::Select( void )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwUInt32				iSelectedCount = 0;
	CEntity *				pEntityToSelect;
	TEntityListIterator		tIterator = m_ptList->begin();

	while( tIterator != m_ptList->end() )
	{
		pEntityToSelect = *tIterator;
		++tIterator;

		iSelectedCount += pEntityToSelect->Select();
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

RwUInt32 CEntityListContainer::Unselect( void )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwUInt32	iUnselectedCount = 0;
	CEntity *				pEntityToUnselect;
	TEntityListIterator		tIterator = m_ptList->begin();

	while( tIterator != m_ptList->end() )
	{
		pEntityToUnselect = *tIterator;
		++tIterator;

		iUnselectedCount += pEntityToUnselect->Unselect();
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

RwUInt32 CEntityListContainer::StartAnimation( RwReal fTime )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwUInt32				iSelectedCount = 0;
	CEntity *				pEntityToSelect;
	TEntityListIterator		tIterator = m_ptList->begin();

	while( tIterator != m_ptList->end() )
	{
		pEntityToSelect = *tIterator;
		++tIterator;

		iSelectedCount += pEntityToSelect->StartAnimation( fTime );
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

RwUInt32 CEntityListContainer::StopAnimation( RwReal fTime )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwUInt32				iSelectedCount = 0;
	CEntity *				pEntityToSelect;
	TEntityListIterator		tIterator = m_ptList->begin();

	while( tIterator != m_ptList->end() )
	{
		pEntityToSelect = *tIterator;
		++tIterator;

		iSelectedCount += pEntityToSelect->StopAnimation( fTime );
	}

	return iSelectedCount;
}


//-----------------------------------------------------------------------------
// Name       : RemoveAll
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

void CEntityListContainer::RemoveAll( RwBool bDeleteAll )
{
	if( bDeleteAll != FALSE )
	{
		CEntity *				pEntityToDestroyed;
		TEntityListIterator		tIterator = m_ptList->begin();

		while( tIterator != m_ptList->end() )
		{
			pEntityToDestroyed = *tIterator;
			Assert( pEntityToDestroyed != NULL );
			++tIterator;

			CEntityContainer::DestroyEntity( pEntityToDestroyed );
		}
	}

	TEntityList		tDummy;

	m_ptList->swap( tDummy );

	Assert( m_ptList->empty() == true );

	m_iCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name       : Insert
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CEntityListContainer::Insert( CEntity * pEntityToInsert )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	Assert( pEntityToInsert != NULL );

#ifdef _COMPILE_OPTION_CHECK_ENTITY_DUPLICATION_
	if( GetEntity( pEntityToInsert->GetUniqueID() ) != NULL )
	{
		Assert( !"Duplicated Entity" );

		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_ENTITY_DUPLICATION_

	m_ptList->push_back( pEntityToInsert );
	++m_iCount;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name       : Remove
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CEntityListContainer::Remove( CEntity * pEntityToRemove )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	Assert( pEntityToRemove != NULL );

	TEntityListIterator		tIterator;

	for( tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		CEntity * pEntity = *tIterator;
		Assert( pEntity != NULL );

		if( pEntity->GetUniqueID() == pEntityToRemove->GetUniqueID() )
		{
			m_ptList->erase( tIterator );
			--m_iCount;

			return TRUE;
		}
	}

	Assert( !"Can't Find" );

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name       : Replace
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CEntityListContainer::Replace( CEntity * pEntityToRemove, CEntity * pEntityToInsert )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	Assert( pEntityToRemove != NULL );
	Assert( pEntityToInsert != NULL );

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		if( (*tIterator)->GetUniqueID() == pEntityToRemove->GetUniqueID() )
		{
			*tIterator = pEntityToInsert;

			return TRUE;
		}
	}

	Assert( !"Can't Find" );

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name       : SetFlag
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

void CEntityListContainer::SetFlag( RwUInt32 iFlag, RwBool bApplyToChild )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		if( (*tIterator) != NULL )
		{
			(*tIterator)->SetFlag( iFlag, bApplyToChild );
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

void CEntityListContainer::SetState( EEntityState eState, RwBool bApplyToChild )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		if( (*tIterator) != NULL )
		{
			(*tIterator)->SetState( eState, bApplyToChild );
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

void CEntityListContainer::SetPosition( const SPosition3D * pPosition, RwBool bAdded, RwBool bApplyToChild )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		if( (*tIterator) != NULL )
		{
			(*tIterator)->SetPosition( pPosition, bAdded, bApplyToChild );
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name       : SetScale
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

void CEntityListContainer::SetScale( const SLocation3D * pScale, RwBool bAdded, RwBool bApplyToChild )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		if( (*tIterator) != NULL )
		{
			(*tIterator)->SetScale( pScale, bAdded, bApplyToChild );
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name       : GetEntity
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

CEntity * CEntityListContainer::GetEntity( RwUInt32 iUniqueID )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		NULL;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		if( (*tIterator)->GetUniqueID() == iUniqueID )
		{
			return *tIterator;
		}
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Name       : GetAt
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

CEntity * CEntityListContainer::GetAt( RwUInt32 iOrder )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return NULL;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		if( iOrder == 0 )
		{
			return *tIterator;
		}
		else
		{
			--iOrder;
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

RwUInt32 CEntityListContainer::SearchEntity( CEntityContainer * pResultContainer, const SEntitySearchCondition * pSearchCondition )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwUInt32		iCount = pResultContainer->GetCount();

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		CEntity * pEntity = *tIterator;
		if( pEntity != NULL )
		{
			pEntity->SearchEntity( pResultContainer, pSearchCondition );
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

CEntity * CEntityListContainer::SearchByRwInstance( void * pRwInstance )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return NULL;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	CEntity *		pElementEntity;
	CEntity *		pMatchEntity;

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		pElementEntity = *tIterator;
		if( pElementEntity != NULL )
		{
			pMatchEntity = pElementEntity->SearchByRwInstance( pRwInstance );
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

RwUInt32 CEntityListContainer::SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultList, EEntityType eEntityType ) //const
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwUInt32		iMatchCount = 0;

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		CEntity * pEntity = *tIterator;
		if( pEntity != NULL )
		{
			iMatchCount += pEntity->SearchByLocation( pLocation, pResultList, eEntityType );
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

RwBool CEntityListContainer::DetectCollision( CCollisionDetectionInfo * pCollisionInfo )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwBool		bResult = TRUE;

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		CEntity * pEntity = *tIterator;
		if( pEntity != NULL )
		{
			if( pEntity->DetectCollision( pCollisionInfo ) == FALSE )
			{
				bResult = FALSE;
			}
		}
	}

	return bResult;
}



//-----------------------------------------------------------------------------
// Name			: CalculateVertexColor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityListContainer::CalculateVertexColor( SVertexColorGenerationInfo * pInfo )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwBool		bResult = TRUE;

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		CEntity * pEntity = *tIterator;
		if( pEntity != NULL )
		{
			if( pEntity->CalculateVertexColor( pInfo ) == FALSE )
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

RwInt32 CEntityListContainer::Export( CFixedStringList * pStringList, CFixedString * pName )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwInt32		iExportCountTotal = 0;
	RwInt32		iExportCountElement;

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		CEntity * pEntity = *tIterator;
		if( pEntity != NULL )
		{
			iExportCountElement = pEntity->Export( pStringList, pName );
			if( iExportCountElement < 0 )
			{
				return iExportCountElement;
			}

			iExportCountTotal += iExportCountElement;
		}
	}

	return iExportCountTotal;
}


//-----------------------------------------------------------------------------
// Name			: ReportSelfState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityListContainer::ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptList == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwBool		bResult = TRUE;

	for( TEntityListIterator tIterator = m_ptList->begin(); tIterator != m_ptList->end(); ++tIterator )
	{
		CEntity * pEntity = *tIterator;
		if( pEntity != NULL )
		{
			if( pEntity->ReportState( pLogMessageManager, iTab + 1 ) == FALSE )
			{
				bResult = FALSE;
			}
		}
	}

	return bResult;
}


//*****************************************************************************
//
// End of File : CEntityListContainer.cpp
//
//*****************************************************************************