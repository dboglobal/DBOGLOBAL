//*****************************************************************************
// File       : CCameraEntity.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_CAMERA_ENTITY_H__
#define __C_CAMERA_ENTITY_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

struct RwFrame;
struct RwCamera;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"

#include "CEntity.h"

#include "CCameraEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define CAMERA_ASPECT_DEFAULT			1.25f

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CCameraEntity
// Desc       : 
// Author     : 
//=============================================================================

class CCameraEntity : public CEntity
{
	protected:

		HWND							m_hWnd;

		SLocation3D						m_Direction;

		SLocation3D						m_Front;
		SLocation3D						m_Up;
		SLocation3D						m_Left;

	public:

										CCameraEntity( void );
										CCameraEntity( RwInt32 iUniqueID ) : CEntity( iUniqueID ) { Initialize(); }
										CCameraEntity( const CCameraEntity & rEntity );

		virtual							~CCameraEntity( void ) { Destroy(); }

		const CCameraEntity &			operator =( const CCameraEntity & rEntity );

	public:

		virtual RwBool					Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty );
		virtual RwBool					PostCreate( void );

		virtual void					Destroy( void );

		virtual RwBool					UpdateFrame( void );
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL );

		virtual RwBool					Attach( CEntity * pChild );
		virtual RwBool					Detach( CEntity * pChild );

		void							SetWindow( HWND hWnd ) { m_hWnd = hWnd; }
		HWND							GetWindow( void ) const { return m_hWnd; }

		RwBool							Resize( RwRect * pRect );

		void							SetBackgroundColor( const SColorInt * pBackgroundColor );
		const SColorInt *				GetBackgroundColor( void ) const;

		RwBool							Clear( void ) const;
		RwBool							ShowRaster( RwUInt32 iFlags = 0 ) const;

		RwBool							BeginUpdate( void ) const;
		RwBool							EndUpdate( void ) const;

		const SLocation3D *				GetDirection( void ) const { return &m_Direction; }

		const SLocation3D *				GetFront( void ) const { return &m_Front; }
		const SLocation3D *				GetUp( void ) const { return &m_Up; }
		const SLocation3D *				GetLeft( void ) const { return &m_Left; }

		const SClipPlane *				GetClipPlane( void ) const { return ((reinterpret_cast<CCameraEntityProperty *>(m_pProperty))->GetClipPlane()); }

		virtual void					SetRwFrame( const RwFrame * pRwFrame );
		virtual RwFrame *				GetRwFrame( void ) const;

		RwBool							GetRay( RwV2d * pScreenPosition, RwLine * pRay );

		RwBool							SetFov( RwReal fFov, RwReal fAspect = CAMERA_ASPECT_DEFAULT );

		RwBool							SetFogDistance( RwReal fDistance );

		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName );

	protected:

		virtual void					Initialize( void );

		inline CCameraEntityProperty *	GetCameraProperty( void ) const { return reinterpret_cast<CCameraEntityProperty *>(GetProperty()); }
		inline RwCamera *				GetRwCamera( void ) const { return reinterpret_cast<RwCamera *>(GetRwInstance()); }

		virtual RwBool					SetParameter( void );

		void							SetRaster( const SRasterInfo * pRasterInfo );
		void							SetClipPlane( const SClipPlane * pClipPlane );

};


//=============================================================================
// End of Class : CCameraEntity
//=============================================================================

#endif //__C_CAMERA_ENTITY_H__

//*****************************************************************************
//
// End of File : CCameraEntity.h
//
//*****************************************************************************