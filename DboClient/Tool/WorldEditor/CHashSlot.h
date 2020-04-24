//*****************************************************************************
// File       : CHashSlot.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_HASH_SLOT_H__
#define __C_HASH_SLOT_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Slot
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Slot
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Slot
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant Slot
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Slot
//-----------------------------------------------------------------------------

typedef struct SHashNode
{
	unsigned long	iKey;
	void *			pData;
} SHashNode;


typedef list<SHashNode *>	 			THashNodeList;
typedef THashNodeList::iterator			THashNodeListIterator;


//=============================================================================
// Name       : CHashSlot
// Desc       : 
// Author     : 
//=============================================================================

class CHashSlot
{
	protected:

		THashNodeList					m_tList;

	public:

										CHashSlot( void ) { Initialize(); }
										CHashSlot( unsigned long iKeyToInsert, void * pDataToInsert ) { Initialize(); Insert( iKeyToInsert, pDataToInsert ); }
										~CHashSlot( void ) { Destroy(); }

	private:

										CHashSlot( const CHashSlot & rcFactory );
		const CHashSlot &				operator=( const CHashSlot & rcFactory );

	public:

		unsigned long					GetCount( void ) const { return static_cast<unsigned long>(m_tList.size()); }
		bool							IsEmpty( void ) const { return m_tList.empty(); }

		bool							IsExist( unsigned long iKeyToFind );
		void *							GetData( unsigned long iKeyToFind );

		bool							Insert( unsigned long iKeyToInsert, void * pDataToInsert );
		bool							Remove( unsigned long iKeyToRemove );

	protected:

		void							Initialize( void );
		void							Destroy( void );

		bool							GetIterator( unsigned long iKeyToFind, THashNodeListIterator * ptIterator );

};


//=============================================================================
// End of Class : CHashSlot
//=============================================================================

#endif //__C_HASH_SLOT_H__

//*****************************************************************************
//
// End of File : CHashSlot.h
//
//*****************************************************************************