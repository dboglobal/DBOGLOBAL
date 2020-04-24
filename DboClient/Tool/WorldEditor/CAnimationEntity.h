//*****************************************************************************
// File       : CAnimationEntity.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ANIMATION_ENTITY_H__
#define __C_ANIMATION_ENTITY_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

struct RpHAnimHierarchy;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"

#include "CEntity.h"

#include "CAnimationEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CAnimationEntity
// Desc       : 
// Author     : 
//=============================================================================

class CAnimationEntity : public CEntity
{
	protected:

		RpHAnimHierarchy *				m_pAnimationHierarchy;

		RwBool							m_bAnimate;
		RwReal							m_fAnimationTime;

	public:

										CAnimationEntity( void );
										CAnimationEntity( RwInt32 iUniqueID ) : CEntity( iUniqueID ) { Initialize(); }
										CAnimationEntity( const CAnimationEntity & rEntity );

		virtual							~CAnimationEntity( void ) { Destroy(); }

		const CAnimationEntity &		operator =( const CAnimationEntity & rEntity );

	public:

		virtual RwBool					Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty );
		virtual RwBool					PostCreate( void );

		virtual void					Destroy( void );
		virtual void					PreDestroy( void );

		virtual RwBool					UpdateFrame( void );
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL );

		virtual RwBool					Attach( CEntity * pChild );
		virtual RwBool					Detach( CEntity * pChild );

		inline RwReal					GetAnimationSpeed( void ) const { return ( (GetAnimationProperty() == NULL ) ? 0.0f : GetAnimationProperty()->GetAnimationSpeed() ); }
		inline void						SetAnimationSpeed( RwReal fAnimationSpeed ) const { return ( (GetAnimationProperty() == NULL ) ? 0.0f : GetAnimationProperty()->SetAnimationSpeed( fAnimationSpeed ) ); }

		inline RwReal					GetAnimationOffset( void ) const { return ( (GetAnimationProperty() == NULL ) ? 0.0f : GetAnimationProperty()->GetAnimationOffset() ); }
		inline void						SetAnimationOffset( RwReal fAnimationOffset ) const { return ( (GetAnimationProperty() == NULL ) ? 0.0f : GetAnimationProperty()->SetAnimationOffset( fAnimationOffset ) ); }

		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName );

		inline RpHAnimHierarchy *		GetAnimationHierarchy( void ) const { return m_pAnimationHierarchy; }
		void							SetAnimationHierarchy( RpHAnimHierarchy * pAnimationHierarchy );

		virtual RwUInt32				StartAnimation( RwReal fTime = 0.0f );
		virtual RwUInt32				StopAnimation( RwReal fTime = 0.0f );

		inline void						SetAnimationTime( RwReal fTime ) { m_fAnimationTime = fTime; }

	protected:

		virtual void					Initialize( void );

		inline CAnimationEntityProperty *	GetAnimationProperty( void ) const { return reinterpret_cast<CAnimationEntityProperty *>(GetProperty()); }

		inline RtAnimAnimation *		GetRtAnimAnimation( void ) const { return reinterpret_cast<RtAnimAnimation *>(GetRwInstance()); }

		virtual RwBool					SetParameter( void );

		RwBool							UpdateUVAnimation( void );

};


//=============================================================================
// End of Class : CAnimationEntity
//=============================================================================

RpHAnimHierarchy * GetAnimationHierarchyFromClump( RpClump *clump );
RwFrame * GetChildFrameFromAnimationHierarchy(RwFrame *frame, void *data);
RpAtomic * SetAnimationHierarchyForSkinAtomic(RpAtomic *atomic, void *data);


#endif //__C_ANIMATION_ENTITY_H__

//*****************************************************************************
//
// End of File : CAnimationEntity.h
//
//*****************************************************************************