//*****************************************************************************
// File       : CEntityHashContainer.cpp
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

#include "CEntityHashContainer.h"


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

void CEntityHashContainer::Initialize( void )
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

RwBool CEntityHashContainer::Create( SEntityContainerProperty * pProperty )
{
	Assert( pProperty != NULL );
	Assert( pProperty->IsValid() != FALSE );

	m_Property.Assign( pProperty );

	if( m_ptHash != NULL )
	{
		Destroy();
	}

	m_ptHash = NTL_NEW TEntityHash;
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

void CEntityHashContainer::Destroy( void )
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

RwBool CEntityHashContainer::UpdateFrame( void )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return TRUE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	CEntity *				pEntityToUpdate;
	TEntityHashIterator		tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityToUpdate = (*tIterator).second;
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

RwBool CEntityHashContainer::RenderFrame( CEntity * pRenderer )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	CEntity *				pEntityToRender;
	TEntityHashIterator		tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityToRender = (*tIterator).second;
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

RwBool CEntityHashContainer::RenderPrimitive( void )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	CEntity *				pEntityToRender;
	TEntityHashIterator		tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityToRender = (*tIterator).second;
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

RwUInt32 CEntityHashContainer::Select( void )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwUInt32				iSelectedCount = 0;
	CEntity *				pEntityToSelect;
	TEntityHashIterator		tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityToSelect = (*tIterator).second;
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

RwUInt32 CEntityHashContainer::Unselect( void )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwUInt32				iUnselectedCount = 0;
	CEntity *				pEntityToUnselect;
	TEntityHashIterator		tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityToUnselect = (*tIterator).second;
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

RwUInt32 CEntityHashContainer::StartAnimation( RwReal fTime )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwUInt32				iSelectedCount = 0;
	CEntity *				pEntityToSelect;
	TEntityHashIterator		tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityToSelect = (*tIterator).second;
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

RwUInt32 CEntityHashContainer::StopAnimation( RwReal fTime )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwUInt32				iSelectedCount = 0;
	CEntity *				pEntityToSelect;
	TEntityHashIterator		tIterator = m_ptHash->begin();

	while( tIterator != m_ptHash->end() )
	{
		pEntityToSelect = (*tIterator).second;
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

void CEntityHashContainer::RemoveAll( RwBool bDeleteAll )
{
	if( bDeleteAll != FALSE )
	{
		CEntity *				pEntityToDestroyed;
		TEntityHashIterator		tIterator = m_ptHash->begin();

		while( tIterator != m_ptHash->end() )
		{
			pEntityToDestroyed = (*tIterator).second;
			Assert( pEntityToDestroyed != NULL );
			++tIterator;

			CEntityContainer::DestroyEntity( pEntityToDestroyed );
		}
	}

	m_ptHash->clear();
	m_iCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name       : Insert
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CEntityHashContainer::Insert( CEntity * pEntityToInsert )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	Assert( pEntityToInsert != NULL );

//	if( GetEntity( pEntityToInsert->GetUniqueID() ) != NULL )
//	{
//		Assert( !"Duplicated Entity" );
//
//		return FALSE;
//	}

	m_ptHash->insert( TEntityHashPair( pEntityToInsert->GetUniqueID(), pEntityToInsert ) );
	++m_iCount;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name       : Remove
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CEntityHashContainer::Remove( CEntity * pEntityToRemove )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	Assert( pEntityToRemove != NULL );

	TEntityHashIterator		tIterator = m_ptHash->find( pEntityToRemove->GetUniqueID() );

	if( tIterator == m_ptHash->end() )
	{
		Assert( !"Can't Find" );

		return FALSE;
	}

	Assert( pEntityToRemove == (*tIterator).second );

	m_ptHash->erase( tIterator );
	--m_iCount;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name       : Replace
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CEntityHashContainer::Replace( CEntity * pEntityToRemove, CEntity * pEntityToInsert )
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

void CEntityHashContainer::SetFlag( RwUInt32 iFlag, RwBool bApplyToChild )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	CEntity *			pEntity;

	for( TEntityHashIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntity = (*tIterator).second;

		if( pEntity != NULL )
		{
			pEntity->SetFlag( iFlag, bApplyToChild );
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

void CEntityHashContainer::SetState( EEntityState eState, RwBool bApplyToChild )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	CEntity *			pEntity;

	for( TEntityHashIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntity = (*tIterator).second;

		if( pEntity != NULL )
		{
			pEntity->SetState( eState, bApplyToChild );
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

void CEntityHashContainer::SetPosition( const SPosition3D * pPosition, RwBool bAdded, RwBool bApplyToChild )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	CEntity *			pEntity;

	for( TEntityHashIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntity = (*tIterator).second;

		if( pEntity != NULL )
		{
			pEntity->SetPosition( pPosition, bAdded, bApplyToChild );
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

void CEntityHashContainer::SetScale( const SLocation3D * pScale, RwBool bAdded, RwBool bApplyToChild )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	CEntity *			pEntity;

	for( TEntityHashIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntity = (*tIterator).second;

		if( pEntity != NULL )
		{
			pEntity->SetScale( pScale, bAdded, bApplyToChild );
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

CEntity * CEntityHashContainer::GetEntity( RwUInt32 iUniqueID )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	TEntityHashIterator		tIterator = m_ptHash->find( iUniqueID );

	if( tIterator == m_ptHash->end() )
	{
		return NULL;
	}

	return (*tIterator).second;
}


//-----------------------------------------------------------------------------
// Name       : GetAt
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

CEntity * CEntityHashContainer::GetAt( RwUInt32 iOrder )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	for( TEntityHashIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		if( iOrder == 0 )
		{
			return (*tIterator).second;
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

RwUInt32 CEntityHashContainer::SearchEntity( CEntityContainer * pResultContainer, const SEntitySearchCondition * pSearchCondition )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwUInt32			iCount = pResultContainer->GetCount();
	CEntity *			pEntity;

	for( TEntityHashIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntity = (*tIterator).second;

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

CEntity * CEntityHashContainer::SearchByRwInstance( void * pRwInstance )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return NULL;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	CEntity *		pElementEntity;
	CEntity *		pMatchEntity;

	for( TEntityHashIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pElementEntity = (*tIterator).second;
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

RwUInt32 CEntityHashContainer::SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultHash, EEntityType eEntityType ) //const
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwUInt32		iMatchCount = 0;
	CEntity *		pEntity;

	for( TEntityHashIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntity = (*tIterator).second;
		if( pEntity != NULL )
		{
			iMatchCount += pEntity->SearchByLocation( pLocation, pResultHash, eEntityType );
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

 RwBool CEntityHashContainer::DetectCollision( CCollisionDetectionInfo * pCollisionInfo )
 {
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwBool		bResult = TRUE;
	CEntity *	pEntity;

	for( TEntityHashIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntity = (*tIterator).second;
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

 RwBool CEntityHashContainer::CalculateVertexColor( SVertexColorGenerationInfo * pInfo )
 {
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwBool		bResult = TRUE;
	CEntity *	pEntity;

	for( TEntityHashIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntity = (*tIterator).second;
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

RwInt32 CEntityHashContainer::Export( CFixedStringList * pStringList, CFixedString * pName )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return 0;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwInt32			iExportCountTotal = 0;
	RwInt32			iExportCountElement;
	CEntity *		pEntity;

	for( TEntityHashIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntity = (*tIterator).second;
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

RwBool CEntityHashContainer::ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
#ifdef _COMPILE_OPTION_CHECK_CONTAINER_POINTER_
	if( m_ptHash == NULL )
	{
		return FALSE;
	}
#endif //_COMPILE_OPTION_CHECK_CONTAINER_POINTER_

	RwBool			bResult = TRUE;
	CEntity *		pEntity;

	for( TEntityHashIterator tIterator = m_ptHash->begin(); tIterator != m_ptHash->end(); ++tIterator )
	{
		pEntity = (*tIterator).second;
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
// End of File : CEntityHashContainer.cpp
//
//*****************************************************************************