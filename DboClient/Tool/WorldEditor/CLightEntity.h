//*****************************************************************************
// File       : CLightEntity.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_LIGHT_ENTITY_H__
#define __C_LIGHT_ENTITY_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

struct RpLight;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"

#include "CEntity.h"

#include "CLightEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CLightEntity
// Desc       : 
// Author     : 
//=============================================================================

class CLightEntity : public CEntity
{
	protected:

	public:

										CLightEntity( void );
										CLightEntity( RwInt32 iUniqueID ) : CEntity( iUniqueID ) { Initialize(); }
										CLightEntity( const CLightEntity & rEntity );

		virtual							~CLightEntity( void ) { Destroy(); }

		const CLightEntity &			operator =( const CLightEntity & rEntity );

	public:

		virtual RwBool					Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty );
		virtual void					Destroy( void );

		virtual RwBool					UpdateFrame( void );
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL );

		ELightEntityType				GetLightType( void ) const { return ( ( GetLightProperty() == NULL ) ? E_LIGHT_ENTITY_TYPE_NONE : GetLightProperty()->GetLightType() ); }

		const SColorFloat *				GetColor( void ) const { return GetLightProperty()->GetColor(); }
		void							SetColor( const SColorFloat * pColor );

		RwReal							GetRadius( void ) const { return GetLightProperty()->GetRadius(); }

		virtual void					SetRwFrame( const RwFrame * pRwFrame );
		virtual RwFrame *				GetRwFrame( void ) const;

		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName );

		const CFixedString *			GetLightTypeName( void ) const { return ( ( GetLightProperty() != NULL ) ? GetLightProperty()->GetLightTypeName() : CLightEntityProperty::GetLightTypeName( E_LIGHT_ENTITY_TYPE_NONE ) ); }

		inline RwBool					IsTerrainLight( void ) const { return ( ( GetLightProperty() == NULL ) ? FALSE : GetLightProperty()->IsTerrainLight() ); }
		inline RwBool					IsShadowLight( void ) const { return ( ( GetLightProperty() == NULL ) ? FALSE : GetLightProperty()->IsShadowLight() ); }

		inline RwReal					GetShadowOffset( void ) const { return ( ( GetLightProperty() == NULL ) ? FALSE : GetLightProperty()->GetShadowOffset() ); }
		inline void						SetShadowOffset( RwReal fShadowOffset ) { if( GetLightProperty() != NULL ) GetLightProperty()->SetShadowOffset( fShadowOffset ); }

		inline RwReal					GetDirectionalCutOff( void ) const { return ( ( GetLightProperty() == NULL ) ? 0.0f : GetLightProperty()->GetDirectionalCutOff() ); }
		inline void						SetDirectionalCutOff( RwReal fDirectionalCutOff ) { if( GetLightProperty() != NULL ) GetLightProperty()->SetDirectionalCutOff( fDirectionalCutOff ); }

		inline RwReal					GetMultipleFactor( void ) const { return ( ( GetLightProperty() == NULL ) ? 0.0f : GetLightProperty()->GetMultipleFactor() ); }
		inline void						SetMultipleFactor( RwReal fMultipleFactor ) { if( GetLightProperty() != NULL ) GetLightProperty()->SetMultipleFactor( fMultipleFactor ); }

		inline RwReal					GetSubShadowFactor( void ) const { return ( ( GetLightProperty() == NULL ) ? 0.0f : GetLightProperty()->GetSubShadowFactor() ); }
		inline void						SetSubShadowFactor( RwReal fSubShadowFactor ) { if( GetLightProperty() != NULL ) GetLightProperty()->SetSubShadowFactor( fSubShadowFactor ); }

	protected:

		virtual void					Initialize( void );

		virtual RwBool					SetParameter( void );

		inline CLightEntityProperty *	GetLightProperty( void ) const { return reinterpret_cast<CLightEntityProperty *>(GetProperty()); }
		inline RpLight *				GetRpLight( void ) const { return reinterpret_cast<RpLight *>(GetRwInstance()); }

		void							SetRadius( RwReal fRadius );

		virtual RwBool					SetPlugInData( void );
		virtual RwBool					ResetPlugInData( void );

	protected:

		static RwUInt32					m_iRwInstanceExPlugInOffset;

	public:

		static RwBool					RwInstanceExPlugInAttach( void );

		static void *					RwInstanceExPlugInConstructor( void * pObject, RwInt32 iOffsetInObject, RwInt32 iSizeInObject );
		static void *					RwInstanceExPlugInCopier( void * pDstRpLight, const void * pSrcRpLight, RwInt32 iOffsetInObject, RwInt32 iSizeInObject );

		inline static SRwInstanceEx * GetPlugInDataFromRpLight( RpLight * pLight )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pLight);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			return reinterpret_cast<SRwInstanceEx *>(iPlugInAddress);
		}

		inline static const SRwInstanceEx *	GetPlugInDataFromRpLight( const RpLight * pLight )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pLight);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			return reinterpret_cast<SRwInstanceEx *>(iPlugInAddress);
		}

		inline static RpLight * SetPlugInDataToRpLight( RpLight * pLight, const SRwInstanceEx * pPlugInData )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pLight);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			reinterpret_cast<SRwInstanceEx *>(iPlugInAddress)->SetEntity( pPlugInData->GetEntity() );

			return pLight;
		}

};


//=============================================================================
// End of Class : CLightEntity
//=============================================================================

#endif //__C_LIGHT_ENTITY_H__

//*****************************************************************************
//
// End of File : CLightEntity.h
//
//*****************************************************************************