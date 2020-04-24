//*****************************************************************************
// File       : CHashSlot.cpp
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

#include "CHashSlot.h"


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

void CHashSlot::Initialize( void )
{
	m_tList.clear();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CHashSlot::Destroy( void )
{
	THashNodeListIterator	tIterator = m_tList.begin();
	SHashNode *				pNodeToDestroy;

	while( tIterator != m_tList.end() )
	{
		pNodeToDestroy = *tIterator;
		delete pNodeToDestroy;

		++tIterator;
	}

	m_tList.clear();

//	THashNodeList			tDummy;
//	m_tList.swap( tDummy );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Insert
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CHashSlot::Insert( unsigned long iKeyToInsert, void * pDataToInsert )
{
	SHashNode *		pNodeToInsert = new SHashNode;

	pNodeToInsert->iKey		= iKeyToInsert;
	pNodeToInsert->pData	= pDataToInsert;

    m_tList.push_back( pNodeToInsert );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Remove
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CHashSlot::Remove( unsigned long iKeyToRemove )
{
	THashNodeListIterator	tIterator;
	SHashNode *				pNodeToRemove;

	if( GetIterator( iKeyToRemove, &tIterator ) == false )
	{
		return false;
	}

	pNodeToRemove = *tIterator;
	m_tList.erase( tIterator );

	delete pNodeToRemove;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: GetIterator
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CHashSlot::GetIterator( unsigned long iKeyToFind , THashNodeListIterator * ptIterator )
{
	THashNodeListIterator	tIterator = m_tList.begin();
	SHashNode *				pNodeToDestroy;

	while( tIterator != m_tList.end() )
	{
		pNodeToDestroy = *tIterator;

		if( pNodeToDestroy->iKey == iKeyToFind )
		{
			*ptIterator = tIterator;

			return TRUE;
		}

		++tIterator;
	}

	*ptIterator = NULL;

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: IsExist
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CHashSlot::IsExist( unsigned long iKeyToFind )
{
	THashNodeListIterator	tIterator;

	return GetIterator( iKeyToFind, &tIterator );
}


//-----------------------------------------------------------------------------
// Name			: GetData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void * CHashSlot::GetData( unsigned long iKeyToFind )
{
	THashNodeListIterator	tIterator;

	if( GetIterator( iKeyToFind, &tIterator ) == false )
	{
		return NULL;
	}

	return (*tIterator)->pData;
}


//*****************************************************************************
//
// End of File : CHashSlot.cpp
//
//*****************************************************************************