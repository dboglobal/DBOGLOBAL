//*****************************************************************************
// File       : CClumpEntity.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_CLUMP_ENTITY_H__
#define __C_CLUMP_ENTITY_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

struct RwFrame;
struct RpClump;
struct RpHAnimHierarchy;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"

#include "CEntity.h"

#include "CClumpEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CClumpEntity
// Desc       : 
// Author     : 
//=============================================================================

class CClumpEntity : public CEntity
{
	protected:

	public:

										CClumpEntity( void );
										CClumpEntity( RwInt32 iUniqueID ) : CEntity( iUniqueID ) { Initialize(); }
										CClumpEntity( const CClumpEntity & rEntity );

		virtual							~CClumpEntity( void ) { Destroy(); }

		const CClumpEntity &			operator =( const CClumpEntity & rEntity );

	public:

		virtual RwBool					Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty );
		virtual RwBool					PostCreate( void );

		virtual void					Destroy( void );
		virtual void					PreDestroy( void );

		virtual RwBool					UpdateFrame( void );
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL );

		virtual RwBool					Attach( CEntity * pChild );
		virtual RwBool					Detach( CEntity * pChild );

		inline const CFixedString *		GetPropertyName( void ) const { return ( ( GetClumpProperty() != NULL ) ? GetClumpProperty()->GetName() : NULL ); }

		virtual void					SetRwFrame( const RwFrame * pRwFrame );
		virtual RwFrame *				GetRwFrame( void ) const;

		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName );

		virtual RwBool					HasAnimation( void ) const { return ( ( GetClumpProperty() != NULL ) ? GetClumpProperty()->HasAnimation() : FALSE ); }

		CEntity *						GetAnimationEntity( void ) const;

	protected:

		virtual void					Initialize( void );

		inline CClumpEntityProperty *	GetClumpProperty( void ) const { return reinterpret_cast<CClumpEntityProperty *>(GetProperty()); }
		inline RpClump *				GetRpClump( void ) const { return reinterpret_cast<RpClump *>(GetRwInstance()); }

		virtual void					SetDefaultName( void );

		RwInt32							CreateInternalAtomicEntity( void );
		RwInt32							CreateInternalLightEntity( void );
		RwBool							CreateAnimationEntity( void );

		RwLinkList *					GetRpAtomicList( void ) const { return &(GetRpClump()->atomicList); }
		RwLinkList *					GetRpLightList( void ) const { return &(GetRpClump()->lightList); }

		RpHAnimHierarchy *				GetAnimationHierarchy( void ) const;

		virtual RwBool					SetPlugInData( void );
		virtual RwBool					ResetPlugInData( void );

	protected:

		static RwUInt32					m_iRwInstanceExPlugInOffset;

	public:

		static RwBool					RwInstanceExPlugInAttach( void );

		static void *					RwInstanceExPlugInConstructor( void * pObject, RwInt32 iOffsetInObject, RwInt32 iSizeInObject );
		static void *					RwInstanceExPlugInCopier( void * pDstRpClump, const void * pSrcRpClump, RwInt32 iOffsetInObject, RwInt32 iSizeInObject );

		inline static SRwInstanceEx *	GetPlugInDataFromRpClump( RpClump * pClump )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pClump);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			return reinterpret_cast<SRwInstanceEx *>(iPlugInAddress);
		}

		inline static const SRwInstanceEx *	GetPlugInDataFromRpClump( const RpClump * pClump )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pClump);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			return reinterpret_cast<SRwInstanceEx *>(iPlugInAddress);
		}

		inline static RpClump * SetPlugInDataToRpClump( RpClump * pClump, const SRwInstanceEx * pPlugInData )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pClump);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			reinterpret_cast<SRwInstanceEx *>(iPlugInAddress)->SetEntity( pPlugInData->GetEntity() );

			return pClump;
		}

};


//=============================================================================
// End of Class : CClumpEntity
//=============================================================================

#endif //__C_CLUMP_ENTITY_H__

//*****************************************************************************
//
// End of File : CClumpEntity.h
//
//*****************************************************************************