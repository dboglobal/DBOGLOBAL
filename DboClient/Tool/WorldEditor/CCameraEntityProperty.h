//*****************************************************************************
// File       : CCameraEntityProperty.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_CAMERA_ENTITY_PROPERTY_H__
#define __C_CAMERA_ENTITY_PROPERTY_H__

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

#include "CEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define CAMERA_ENTITY_FOV_DEFAULT		(45.0f)


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CCameraEntityProperty
// Desc       : 
// Author     : 
//=============================================================================

class CCameraEntityProperty : public CEntityProperty
{
	protected:

		SRasterInfo						m_sRasterInfo;
		SClipPlane						m_sClipPlane;
		SPosition3D						m_sPosition;
		SColorInt						m_sBackgroundColor;
		RwReal							m_fFov;

	public:

										CCameraEntityProperty( void ) { Initialize(); }
										CCameraEntityProperty( const CCameraEntityProperty & rProperty );

		virtual							~CCameraEntityProperty( void ) { Destroy(); }

		const CCameraEntityProperty &	operator=( const CCameraEntityProperty & rProperty );

	public:

		virtual CEntityProperty *		Clone( void ) const;

		inline virtual EEntityType		GetType( void ) const { return E_ENTITY_TYPE_CAMERA; }

		virtual RwBool					IsValid( void ) const { return TRUE; }

		inline virtual RwBool			HasPosition( void ) const { return TRUE; }
		inline virtual RwBool			HasLocation( void ) const { return TRUE; }
		inline virtual RwBool			HasRotation( void ) const { return TRUE; }
		inline virtual RwBool			HasScale( void ) const { return FALSE; }

		virtual void					SetPosition( const SPosition3D * pNewPosition );
		inline virtual const SPosition3D *	GetPosition( void ) const { return &m_sPosition; }

		void							SetRasterInfo( const SRasterInfo * pNewRasterInfo );
		void							SetClipPlane( const SClipPlane * pNewClipPlane );
		void							SetBackgroundColor( const SColorInt * pBackgroundColor );
		void							SetFov( RwReal fFov ) { m_fFov = fFov; }

		inline const SRasterInfo *		GetRasterInfo( void ) const { return &m_sRasterInfo; }
		inline const SClipPlane *		GetClipPlane( void ) const { return &m_sClipPlane; }
		inline const SColorInt *		GetBackgroundColor( void ) const { return &m_sBackgroundColor; }
		inline RwReal					GetFov( void ) const { return m_fFov; }

		virtual void					GetProperty( CEntityProperty * pPropertyToGet ) const;
		virtual void					SetProperty( CEntityProperty * pPropertyToSet );

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );
};


//=============================================================================
// End of Class : CCameraEntityProperty
//=============================================================================

#endif //__C_CAMERA_ENTITY_PROPERTY_H__

//*****************************************************************************
//
// End of File : CCameraEntityProperty.h
//
//*****************************************************************************