//*****************************************************************************
// File       : CHashTable.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_HASH_TABLE_H__
#define __C_HASH_TABLE_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CHashSlot.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CHashTable
// Desc       : 
// Author     : 
//=============================================================================

class CHashTable
{
	protected:

		unsigned long					m_iTableSize;
		unsigned long					m_iCount;

		CHashSlot **					m_ppTable;

	public:

										CHashTable( unsigned long iTableSize ) { Create( iTableSize ); }
		virtual							~CHashTable( void ) { Destroy(); }

	private:

										CHashTable( void );
										CHashTable( const CHashTable & rcFactory );
		const CHashTable &				operator=( const CHashTable & rcFactory );

	public:

		unsigned long					GetTableSize( void ) const { return m_iTableSize; }

		bool							IsEmpty( void ) const { return (!m_iCount); }
		unsigned long					GetCount( void ) const { return m_iCount; }

		bool							IsExist( unsigned long iKey );
		void *							GetData( unsigned long iKey );

		bool							Insert( unsigned long iKey, void * pData );
		bool							Remove( unsigned long iKey );

	protected:

		bool							Create( unsigned long iTableSize );
		void							Destroy( void );

		virtual unsigned long			GetSlotIndex( unsigned long iKeyToConvert ) const;
};


//=============================================================================
// End of Class : CHashTable
//=============================================================================

#endif //__C_HASH_TABLE_H__

//*****************************************************************************
//
// End of File : CHashTable.h
//
//*****************************************************************************