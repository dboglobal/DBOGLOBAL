//*****************************************************************************
// File       : CWorldSectorEntityProperty.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_WORLD_SECTOR_ENTITY_PROPERTY_H__
#define __C_WORLD_SECTOR_ENTITY_PROPERTY_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CEntityContainer;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"

#include "CFixedString.h"

#include "CEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CWorldEntityProperty
// Desc       : 
// Author     : 
//=============================================================================

class CWorldSectorEntityProperty : public CEntityProperty
{
	protected:

		SLocation3DInt					m_sSectorIndex;
		CEntityContainer *				m_pSectorContainer;

	public:

										CWorldSectorEntityProperty( void ) { Initialize(); }
										CWorldSectorEntityProperty( const CWorldSectorEntityProperty & rProperty );

		virtual							~CWorldSectorEntityProperty( void ) { Destroy(); }

		const CWorldSectorEntityProperty &	operator=( const CWorldSectorEntityProperty & rProperty );

	public:

		virtual CEntityProperty *		Clone( void ) const;

		virtual EEntityType				GetType( void ) const { return E_ENTITY_TYPE_WORLD_SECTOR; }

		virtual RwBool					IsValid( void ) const;

		virtual RwBool					HasPosition( void ) const { return TRUE; }
		virtual RwBool					HasLocation( void ) const { return TRUE; }
		virtual RwBool					HasRotation( void ) const { return FALSE; }
		virtual RwBool					HasScale( void ) const { return FALSE; }

		const SLocation3DInt *			GetSectorIndex( void ) const { return &m_sSectorIndex; }
		CEntityContainer *				GetSectorContainer( void ) const { return m_pSectorContainer; }

		void							SetSectorIndex( const SLocation3DInt * pSectorIndex ) { m_sSectorIndex.Set( pSectorIndex ); }
		void							SetSectorContainer( CEntityContainer * pSectorContainer ) { m_pSectorContainer = pSectorContainer; }

		virtual void					GetProperty( CEntityProperty * pPropertyToGet ) const;
		virtual void					SetProperty( CEntityProperty * pPropertyToSet );

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );
};


//=============================================================================
// End of Class : CWorldSectorEntityProperty
//=============================================================================

#endif //__C_WORLD_SECTOR_ENTITY_PROPERTY_H__

//*****************************************************************************
//
// End of File : CWorldSectorEntityProperty.h
//
//*****************************************************************************