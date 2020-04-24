//*****************************************************************************
// File       : CEntityEventQueue.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_EVENT_QUEUE_H__
#define __C_ENTITY_EVENT_QUEUE_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CEntityEvent;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef list<CEntityEvent *>	 					TEntityEventList;
typedef TEntityEventList::iterator					TEntityEventListIterator;


//=============================================================================
// Name       : CEntityEventQueue
// Desc       : 
// Author     : 
//=============================================================================

class CEntityEventQueue
{
	protected:

		RwUInt32						m_iCount;

		TEntityEventList				m_tList;

	public:

										CEntityEventQueue( void ) { Initialize(); }

	virtual 							~CEntityEventQueue( void ) { Destroy(); }

	private:

										CEntityEventQueue( const CEntityEventQueue & rcEntityEventQueue );

		const CEntityEventQueue &		operator=( const CEntityEventQueue & rcEntityEventQueue );

	public:

		virtual RwBool					Update( RwReal fCurrenTime );

		RwBool							IsEmpty( void ) const { return (!m_iCount); }
		RwUInt32						GetCount( void ) const { return m_iCount; }

		virtual RwBool					Insert( CEntityEvent * pEventToInsert );

//		virtual RwBool					ReportState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );

};


//=============================================================================
// End of Class : CEntityEventQueue
//=============================================================================

#endif //__C_ENTITY_EVENT_QUEUE_H__

//*****************************************************************************
//
// End of File : CEntityEventQueue.h
//
//*****************************************************************************