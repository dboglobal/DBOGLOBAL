//*****************************************************************************
// File       : CEntityEvent.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_EVENT_H__
#define __C_ENTITY_EVENT_H__

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

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CEntityEvent
// Desc       : 
// Author     : 
//=============================================================================

class CEntityEvent
{
	protected:

		EEntityEventType				m_eType;

		RwUInt32						m_iUniqueID;

		RwBool							m_bLoop;
		RwBool							m_bActive;

		RwUInt32						m_iNodeCount;
		SEntityEventNode *				m_pNodes;

		void *							m_pData;

	public:

										CEntityEvent( void ) { Initialize(); }

										~CEntityEvent( void ) { Destroy(); }

	private:

										CEntityEvent( const CEntityEvent & rEntityEvent );

		const CEntityEvent &			operator =( const CEntityEvent & rEntityEvent );

	public:

		RwBool							Create( void );
		void							Destroy( void );

		RwBool							CreateFadeInOut( RwUInt32 iUniqueID, RwReal fValue, RwReal fBeginTime, RwReal fInTime, RwReal fKeepTime, RwReal fOutTime );

		EEntityEventType				GetType( void ) const { return m_eType; }

		RwBool							IsBegin( RwReal fCurrenTime ) const;
		RwBool							IsEnd( RwReal fCurrenTime ) const;

		RwBool							IsLoop( void ) const { return m_bLoop; }

		RwBool							IsActive( void ) const { return m_bActive; }
		void							Activate( RwBool bActive ) { m_bActive = bActive; }

		RwBool							Update( RwReal fCurrenTime );

	protected:

		void							Initialize( void );

};


//=============================================================================
// End of Class : CEntityEvent
//=============================================================================

#endif //__C_ENTITY_EVENT_H__

//*****************************************************************************
//
// End of File : CEntityEvent.h
//
//*****************************************************************************