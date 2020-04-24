//*****************************************************************************
// File       : CEntityCollisionList.cpp
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
#include "CEntityCollision.h"

#include "CEntityCollisionList.h"


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

void CEntityCollisionList::Initialize( void )
{
	TEntityCollisionList		tSwapList;
	m_tList.swap( tSwapList );

	m_tList.clear();
	m_iCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCollisionList::Create( void )
{
	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityCollisionList::Destroy( void )
{
	RemoveAll( TRUE );

	return;
}


//-----------------------------------------------------------------------------
// Name			: RemoveAll
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityCollisionList::RemoveAll( RwBool bDelete )
{
	if( m_iCount > 0 )
	{
		if( bDelete != FALSE )
		{
			CEntityCollision *				pEntityCollisionToDestroyed;
			TEntityCollisionListIterator	tIterator = m_tList.begin();

			while( tIterator != m_tList.end() )
			{
				pEntityCollisionToDestroyed = *tIterator;
				Assert( pEntityCollisionToDestroyed != NULL );
				++tIterator;

				NTL_DELETE( pEntityCollisionToDestroyed );
			}
		}

		Initialize();
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: Insert
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCollisionList::Insert( CEntityCollision * pEntityCollisionToInsert )
{
	Assert( pEntityCollisionToInsert != NULL );

	CEntityCollision *	pEntityCollision;

	for( TEntityCollisionListIterator tIterator = m_tList.begin(); tIterator != m_tList.end(); ++tIterator )
	{
		pEntityCollision = *tIterator;

		if( pEntityCollision->GetDistance() > pEntityCollisionToInsert->GetDistance() )
		{
			m_tList.insert( tIterator, pEntityCollisionToInsert->Clone() );
			++m_iCount;

			return TRUE;
		}

		if( pEntityCollision->GetEntity() == pEntityCollisionToInsert->GetEntity() )
		{
			return FALSE;
		}
	}

	m_tList.push_back( pEntityCollisionToInsert->Clone() );
	++m_iCount;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Remove
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCollisionList::Remove( CEntityCollision * pEntityCollisionToRemove, RwBool bDelete )
{
	Assert( pEntityCollisionToRemove != NULL );

	TEntityCollisionListIterator	tIterator;
	CEntityCollision *				pInfo;

	for( tIterator = m_tList.begin(); tIterator != m_tList.end(); ++tIterator )
	{
		pInfo = *tIterator;
		if( pInfo == pEntityCollisionToRemove )
		{
			m_tList.erase( tIterator );
			--m_iCount;

			if( bDelete != FALSE )
			{
				NTL_DELETE( pInfo );
			}

			return TRUE;
		}
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: Replace
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCollisionList::Replace( CEntityCollision * pEntityCollisionToRemove, CEntityCollision * pEntityCollisionToInsert, RwBool bDelete )
{
	Assert( pEntityCollisionToRemove != NULL );
	Assert( pEntityCollisionToInsert != NULL );

	RwBool		bResult =  TRUE;

	if( Remove( pEntityCollisionToRemove, bDelete ) == FALSE )
	{
		bResult = FALSE;
	}

	if( Insert( pEntityCollisionToInsert ) == FALSE )
	{
		bResult = FALSE;
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: GetCollisionEntityByOrder
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityCollision * CEntityCollisionList::GetCollisionEntityByOrder( RwUInt32 iOrder, EEntityTypeFilter eTypeFilter )
{
	CEntityCollision *	pEntityCollision;

	for( TEntityCollisionListIterator tIterator = m_tList.begin(); tIterator != m_tList.end(); ++tIterator )
	{
		pEntityCollision = *tIterator;
		Assert( pEntityCollision != NULL );

		CEntity *		pEntity = pEntityCollision->GetEntity();
		Assert( pEntity != NULL );

		if( IsEntityTypeInFilter( pEntity->GetType(), eTypeFilter ) == FALSE )
		{
			continue;
		}

		if( iOrder == 0 )
		{
			return pEntityCollision;
		}
		else
		{
			--iOrder;
		}
	}

	return NULL;
}


//*****************************************************************************
//
// End of File : CEntityCollisionList.cpp
//
//*****************************************************************************