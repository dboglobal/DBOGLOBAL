//*****************************************************************************
// File       : CHashTable.cpp
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

#include "CHashTable.h"


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

bool CHashTable::Create( unsigned long iTableSize )
{
	m_iTableSize	= iTableSize;
	m_iCount		= 0;

	m_ppTable		= new CHashSlot *[iTableSize];
	ZeroMemory( m_ppTable, sizeof( CHashSlot * ) * iTableSize );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CHashTable::Destroy( void )
{
	if( m_ppTable != NULL )
	{
		for( unsigned long iSlotIndex = 0; iSlotIndex < m_iTableSize; ++iSlotIndex )
		{
			if( m_ppTable[iSlotIndex] != NULL )
			{
				delete m_ppTable[iSlotIndex];
			}
		}

		delete[] m_ppTable;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetSlotIndex
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

unsigned long CHashTable::GetSlotIndex( unsigned long iKeyToConvert ) const
{
	return ( HIWORD( iKeyToConvert ) + LOWORD( iKeyToConvert ) ) % m_iTableSize;
}


//-----------------------------------------------------------------------------
// Name			: Insert
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CHashTable::Insert( unsigned long iKeyToInsert, void * pDataToInsert )
{
	unsigned long iSlotIndex = GetSlotIndex( iKeyToInsert );

	if( m_ppTable[iSlotIndex] == NULL )
	{
		m_ppTable[iSlotIndex] = new CHashSlot;
	}

	if( m_ppTable[iSlotIndex]->Insert( iKeyToInsert, pDataToInsert ) == false )
	{
		return false;
	}

	++m_iCount;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Remove
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CHashTable::Remove( unsigned long iKeyToRemove )
{
	unsigned long iSlotIndex = GetSlotIndex( iKeyToRemove );

	if( m_ppTable[iSlotIndex] == NULL )
	{
		return false;
	}

	if( m_ppTable[iSlotIndex]->Remove( iKeyToRemove ) == false )
	{
		return false;
	}

	if(  m_ppTable[iSlotIndex]->IsEmpty() == true )
	{
		delete m_ppTable[iSlotIndex];
		m_ppTable[iSlotIndex] = NULL;
	}

	--m_iCount;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: IsExist
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CHashTable::IsExist( unsigned long iKeyToFind )
{
	unsigned long iSlotIndex = GetSlotIndex( iKeyToFind );

	if( m_ppTable[iSlotIndex] == NULL )
	{
		return false;
	}

	return m_ppTable[iSlotIndex]->IsExist( iKeyToFind );
}


//-----------------------------------------------------------------------------
// Name			: GetData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void * CHashTable::GetData( unsigned long iKeyToFind )
{
	unsigned long iSlotIndex = GetSlotIndex( iKeyToFind );

	if( m_ppTable[iSlotIndex] == NULL )
	{
		return NULL;
	}

	return m_ppTable[iSlotIndex]->GetData( iKeyToFind );
}


//*****************************************************************************
//
// End of File : CHashTable.cpp
//
//*****************************************************************************