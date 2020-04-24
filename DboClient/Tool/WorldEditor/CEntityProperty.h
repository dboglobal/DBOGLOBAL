//*****************************************************************************
// File       : CEntityProperty.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_PROPERTY_H__
#define __C_ENTITY_PROPERTY_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CNtlPLProperty;


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
// Name       : CEntityProperty
// Desc       : 
// Author     : 
//=============================================================================

class CEntityProperty
{
	protected:

		RwUInt32						m_iUniqueID;
		void *							m_pRwPrototype;
		RwBool							m_bGlobal;
		CFixedString					m_Name;

	public:

										CEntityProperty( void ) { Initialize(); }
										CEntityProperty( const CEntityProperty & rProperty );

		virtual							~CEntityProperty( void ) { Destroy(); }

		const CEntityProperty &			operator=( const CEntityProperty & rProperty );

	public:

		inline RwUInt32					GetUniqueID( void ) const { return m_iUniqueID; }
		inline void						SetUniqueID( RwUInt32 iUniqueID ) { m_iUniqueID = iUniqueID; }

		inline void						SetRwPrototype( void * pPrototype ) { m_pRwPrototype = pPrototype; }
		inline void *					GetRwPrototype( void ) const { return m_pRwPrototype; }
		inline virtual EEntityType		GetType( void ) const { return E_ENTITY_TYPE_NONE; }

		inline const CFixedString *		GetName( void ) const { return &m_Name; }
		inline const RwChar *			GetNameBuffer( void ) const { return m_Name.GetBuffer(); }

		inline void						SetName( RwChar * pName ) { m_Name.Assign( pName ); }
		inline void						SetName( const CFixedString &Name ) { m_Name.Assign( Name ); }

		inline virtual RwBool			IsValid( void ) const { return FALSE; }

		inline virtual RwBool			HasPosition( void ) const { return FALSE; }
		inline virtual RwBool			HasLocation( void ) const { return FALSE; }
		inline virtual RwBool			HasRotation( void ) const { return FALSE; }
		inline virtual RwBool			HasScale( void ) const { return FALSE; }
		
		inline virtual RwBool			IsTerrain( void ) const { return FALSE; }
		inline virtual RwBool			IsSky( void ) const { return FALSE; }
		inline virtual RwBool			IsGlobal( void ) const { return m_bGlobal; }

		inline virtual void				SetPosition( const SPosition3D * pNewPosition ) {}
		inline virtual const SPosition3D *	GetPosition( void ) const { return &SPosition3D::POSITION_DEFAULT; }

		inline virtual void				SetScale( const SLocation3D * pNewPosition ) {}
		inline virtual const SLocation3D *	GetScale( void ) const { return &(SLocation3D::SCALE_DEFAULT); }

		virtual CEntityProperty *		Clone( void ) const = 0;

		const CFixedString *			GetTypeName( void ) const;

		virtual void					GetProperty( CEntityProperty * pPropertyToGet ) const;
		virtual void					SetProperty( CEntityProperty * pPropertyToSet );

		inline virtual const CNtlPLProperty *	GetPlProperty( void ) const { return NULL; }
		inline virtual void				SetGlobal( RwBool bGlobal ) { m_bGlobal = bGlobal; }

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );

	public:

		static const CFixedString *		GetEntityTypeName( EEntityType eType );

	public:

		static const CFixedString		ENTITY_TYPE_NAME[E_ENTITY_TYPE_COUNT];

		static const CFixedString		KEYWORD_ANIMATION;

};


//=============================================================================
// End of Class : CEntityProperty
//=============================================================================

#endif //__C_ENTITY_PROPERTY_H__

//*****************************************************************************
//
// End of File : CEntityProperty.h
//
//*****************************************************************************