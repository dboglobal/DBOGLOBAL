//*****************************************************************************
// File       : CWorldEntityProperty.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_WORLD_ENTITY_PROPERTY_H__
#define __C_WORLD_ENTITY_PROPERTY_H__

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

class CWorldEntityProperty : public CEntityProperty
{
	protected:

		RwBool							m_bCustomBuild;
		RwBool							m_bEmpty;
		SBoundingBox					m_sBounding;
		SLocation3DInt					m_sSectorMin;
		SLocation3DInt					m_sSectorMax;

	public:

										CWorldEntityProperty( void ) { Initialize(); }
										CWorldEntityProperty( const CWorldEntityProperty & rProperty );

		virtual							~CWorldEntityProperty( void ) { Destroy(); }

		const CWorldEntityProperty &	operator=( const CWorldEntityProperty & rProperty );

	public:

		virtual CEntityProperty *		Clone( void ) const;

		inline virtual EEntityType		GetType( void ) const { return E_ENTITY_TYPE_WORLD; }

		inline virtual RwBool			IsValid( void ) const;

		virtual RwBool					HasPosition( void ) const { return TRUE; }
		virtual RwBool					HasLocation( void ) const { return TRUE; }
		virtual RwBool					HasRotation( void ) const { return FALSE; }
		virtual RwBool					HasScale( void ) const { return FALSE; }

		inline void						SetEmpty( RwBool bEmpty ) { m_bEmpty = bEmpty; }
		inline RwBool					IsEmpty( void ) const { return m_bEmpty; }

		inline void						SetCustomBuild( RwBool bCustomBuild ) { m_bCustomBuild = bCustomBuild; }
		inline RwBool					IsCustomBuild( void ) const { return m_bCustomBuild; }

		void							SetBounding( const SBoundingBox * pNewBounding );
		inline const SBoundingBox *		GetBounding( void ) const { return &m_sBounding; }

		void							SetSectorMin( const SLocation3DInt * pNewSectorMin );
		inline const SLocation3DInt *	GetSectorMin( void ) const { return &m_sSectorMin; }	

		void							SetSectorMax( const SLocation3DInt * pNewSectorMax );
		inline const SLocation3DInt *	GetSectorMax( void ) const { return &m_sSectorMax; }

		virtual void					GetProperty( CEntityProperty * pPropertyToGet ) const;
		virtual void					SetProperty( CEntityProperty * pPropertyToSet );

		virtual const CNtlPLProperty *	GetPlProperty( void ) const;

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );
};


//=============================================================================
// End of Class : CWorldEntityProperty
//=============================================================================

#endif //__C_WORLD_ENTITY_PROPERTY_H__

//*****************************************************************************
//
// End of File : CWorldEntityProperty.h
//
//*****************************************************************************