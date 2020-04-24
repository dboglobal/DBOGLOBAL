//*****************************************************************************
// File       : CEntityCollision.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_COLLISION_H__
#define __C_ENTITY_COLLISION_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CEntity;


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
// Name       : CEntityCollision
// Desc       : 
// Author     : 
//=============================================================================

class CEntityCollision
{
	protected:

		CEntity *						m_pEntity;
		RwReal							m_fDistance;

		SLocation3D						m_sLocation;

	public:

										CEntityCollision( void ) { Initialize(); }
										CEntityCollision( const CEntityCollision & rEntityCollision );

		virtual							~CEntityCollision( void ) { Initialize(); }

		const CEntityCollision &		operator =( const CEntityCollision & rEntityCollision );

	public:

		inline CEntity *				GetEntity( void ) const { return m_pEntity; }
		inline void						SetEntity( CEntity * pNewEntity ) { m_pEntity = pNewEntity; }

		inline RwReal					GetDistance( void ) const { return m_fDistance; }
		inline void						SetDistance( RwReal fNewDistance ) { m_fDistance = fNewDistance; }

		inline const SLocation3D *		GetLocation( void ) const { return &m_sLocation; }
		inline void						SetLocation ( const SLocation3D * pNewLocation ) { m_sLocation.Set( pNewLocation ); }

		void							GetLocalOffset( SLocation3D * pLocalOffset );

		void							Set( const CEntityCollision * pSourceEntityCollision );
		CEntityCollision *				Clone( void ) const;

	protected:

		virtual void					Initialize( void );

};


//=============================================================================
// End of Class : CEntityCollision
//=============================================================================

#endif //__C_ENTITY_COLLISION_H__

//*****************************************************************************
//
// End of File : CEntityCollision.h
//
//*****************************************************************************