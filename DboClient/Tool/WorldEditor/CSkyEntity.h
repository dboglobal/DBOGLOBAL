//*****************************************************************************
// File       : CSkyEntity.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_SKY_ENTITY_H__
#define __C_SKY_ENTITY_H__

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

#include "CEntity.h"

#include "CSkyEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CSkyEntity
// Desc       : 
// Author     : 
//=============================================================================

class CSkyEntity : public CEntity
{
	protected:

		SPosition3D						m_Position;

	public:

										CSkyEntity( void );
										CSkyEntity( RwInt32 iUniqueID ) : CEntity( iUniqueID ) { Initialize(); }
										CSkyEntity( const CSkyEntity & rEntity );

		virtual							~CSkyEntity( void ) { Destroy(); }

		const CSkyEntity &				operator =( const CSkyEntity & rEntity );

	public:

		virtual RwBool					Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty );
		virtual RwBool					PostCreate( void );

		virtual void					Destroy( void );
		virtual void					PreDestroy( void );

		virtual RwBool					UpdateFrame( void );
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL );

		virtual RwBool					Attach( CEntity * pChild );
		virtual RwBool					Detach( CEntity * pChild );

		inline virtual const SPosition3D *	GetPosition( void ) const { return &m_Position; }
		virtual void					SetPosition( const SPosition3D * pPosition, RwBool bAdded = FALSE, RwBool bChildren = FALSE );

		inline RwReal					GetUVAnimationSpeed( EUVAnimationLayer eLayer ) const { return ( (GetSkyProperty() == NULL ) ? 0.0f : GetSkyProperty()->GetUVAnimationSpeed( eLayer ) ); }
		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName );

	protected:

		virtual void					Initialize( void );

		inline CSkyEntityProperty *		GetSkyProperty( void ) const { return reinterpret_cast<CSkyEntityProperty *>(GetProperty()); }

		virtual RwBool					SetParameter( void );

		RwBool							UpdateUVAnimation( void );

};


//=============================================================================
// End of Class : CSkyEntity
//=============================================================================

#endif //__C_SKY_ENTITY_H__

//*****************************************************************************
//
// End of File : CSkyEntity.h
//
//*****************************************************************************