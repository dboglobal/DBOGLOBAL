//*****************************************************************************
// File       : CEntityCollisionList.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_COLLISION_LIST_H__
#define __C_ENTITY_COLLISION_LIST_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

class CEntityCollision;


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef list<CEntityCollision *>	 	TEntityCollisionList;
typedef TEntityCollisionList::iterator	TEntityCollisionListIterator;


//=============================================================================
// Name       : CEntityCollisionList
// Desc       : 
// Author     : 
//=============================================================================

class CEntityCollisionList
{
	protected:

		TEntityCollisionList			m_tList;
		RwUInt32						m_iCount;

	public:

										CEntityCollisionList( void ) { Initialize(); }

		virtual 						~CEntityCollisionList( void ) { Destroy(); }

	private:

										CEntityCollisionList( const CEntityCollisionList & rContainer );

		const CEntityCollisionList &	operator =( const CEntityCollisionList & rContainer );

	public:

		RwBool							Create( void );
		void							Destroy( void );

		RwBool							IsEmpty( void ) const { return ( ( m_iCount > 0 ) ? FALSE : TRUE ); }
		RwUInt32						GetCount( void ) const { return m_iCount; }

		CEntityCollision *				GetCollisionEntityByOrder( RwUInt32 iOrder, EEntityTypeFilter eTypeFilter = E_ENTITY_TYPE_FILTER_ALL );

		RwBool							Insert( CEntityCollision * pEntityCollisionToInsert );
		RwBool							Remove( CEntityCollision * pEntityCollisionToRemove, RwBool bDelete = TRUE );
		void							RemoveAll( RwBool bDelete = TRUE );
		RwBool							Replace( CEntityCollision * pEntityCollisionToRemove, CEntityCollision * pEntityCollisionToInsert, RwBool bDelete = TRUE );

	protected:

		void							Initialize( void );

};

//=============================================================================
// End of Class : CEntityCollisionList
//=============================================================================

#endif //__C_ENTITY_COLLISION_LIST_H__

//*****************************************************************************
//
// End of File : CEntityCollisionList.h
//
//*****************************************************************************