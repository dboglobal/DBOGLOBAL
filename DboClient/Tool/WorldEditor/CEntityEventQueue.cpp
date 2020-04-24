//*****************************************************************************
// File       : CEntityEventQueue.cpp
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

#include "CEntityEvent.h"

#include "CEntityEventQueue.h"


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

void CEntityEventQueue::Initialize( void )
{
	m_tList.clear();

	m_iCount = 0;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityEventQueue::Destroy( void )
{
	CEntityEvent *				pEventToDestroyed;
	TEntityEventListIterator	tIterator = m_tList.begin();

	while( tIterator != m_tList.end() )
	{
		pEventToDestroyed = *tIterator;
		Assert( pEventToDestroyed != NULL );
		++tIterator;

		NTL_DELETE( pEventToDestroyed );
	}

	Initialize();
	
	return;
}


//-----------------------------------------------------------------------------
// Name			: Insert
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityEventQueue::Insert( CEntityEvent * pEventToInsert )
{
	Assert( pEventToInsert != NULL );

	m_tList.push_back( pEventToInsert );
	++m_iCount;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Update
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityEventQueue::Update( RwReal fCurrenTime )
{
	CEntityEvent *		pEvent;

	TEntityEventListIterator tIterator = m_tList.begin();

	while( tIterator != m_tList.end() )
	{
		pEvent = *tIterator;

		pEvent->Update( fCurrenTime );

		if( pEvent->IsEnd( fCurrenTime ) != FALSE && pEvent->IsActive() == FALSE )
		{
			TEntityEventListIterator	tIteratorToDestroy = tIterator++;

			m_tList.erase( tIteratorToDestroy );
			--m_iCount;

			NTL_DELETE( pEvent );
		}
		else
		{
			++tIterator;
		}
	}

	return TRUE;
}


//*****************************************************************************
//
// End of File : CEntityEventQueue.cpp
//
//*****************************************************************************