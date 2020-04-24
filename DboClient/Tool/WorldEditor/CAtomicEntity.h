//*****************************************************************************
// File       : CAtomicEntity.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ATOMIC_ENTITY_H__
#define __C_ATOMIC_ENTITY_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

struct RpAtomic;
struct SRwInstanceEx;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"

#include "CEntity.h"

#include "CAtomicEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CAtomicEntity
// Desc       : 
// Author     : 
//=============================================================================

class CAtomicEntity : public CEntity
{
	protected:

		SImediateRenderInfo				m_WireFrameRenderInfo;
		SImediateRenderInfo				m_BoundingRenderInfo;

		SLocation3D						m_Direction;

		SLocation3D						m_Front;
		SLocation3D						m_Up;
		SLocation3D						m_Left;

		RwBBox							m_BBox;
		RpTie *							m_pTie;

		RwMatrix *						m_pUVAnimationMatrix[E_UV_ANIMATION_LAYER_COUNT];

	public:

										CAtomicEntity( void );
										CAtomicEntity( RwInt32 iUniqueID ) : CEntity( iUniqueID ) { Initialize(); }
										CAtomicEntity( const CAtomicEntity & rEntity );

		virtual							~CAtomicEntity( void ) { Destroy(); }

		const CAtomicEntity &			operator =( const CAtomicEntity & rEntity );

	public:

		virtual RwBool					Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty );
		virtual RwBool					PostCreate( void );

		virtual void					Destroy( void );
		virtual void					PreDestroy( void );

		virtual RwBool					UpdateFrame( void );
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL );
		virtual RwBool					RenderPrimitive( void );

		virtual RwUInt32				Select( void );
		virtual RwUInt32				Unselect( void );

		virtual RwBool					Attach( CEntity * pChild );
		virtual RwBool					Detach( CEntity * pChild );

		virtual void					SetRwFrame( const RwFrame * pRwFrame );
		virtual RwFrame *				GetRwFrame( void ) const;

		virtual RwBool					IsPointInBounding( const SLocation3D * pLocation ) const;

		virtual void					CalculateBounding( void );
		const RwBBox *					GetBoundingBox( void ) const { return &m_BBox; }

		virtual RwBool					DetectCollision( CCollisionDetectionInfo * pCollisionInfo ) const;

		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName );

		RwBool							ApplyColorToWireFrame( RwRGBA * pColor );
		RwBool							ApplyColorToBounding( RwRGBA * pColor );

		RwUInt32						GetUVAnimationLayer( void ) const { return (( GetAtomicProperty() == NULL ) ? 0 : (GetAtomicProperty()->GetUVAnimationLayer())); }
		RwBool							HasUVAnimation( void ) const { return (( GetUVAnimationLayer() == 0 ) ? FALSE : TRUE ); }

		RwBool							SetUVAnimationMatrix( EUVAnimationLayer eLayer, SPosition3D * pPosition, SLocation3D * pScale = NULL );

		EShadowOption					GetShadowProjectOption( void ) const { return (( GetAtomicProperty() == NULL ) ? E_SHADOW_OPTION_NONE : (GetAtomicProperty()->GetShadowProjectOption())); }
		EShadowOption					GetShadowSubjectOption( void ) const { return (( GetAtomicProperty() == NULL ) ? E_SHADOW_OPTION_NONE : (GetAtomicProperty()->GetShadowSubjectOption())); }

		virtual void					Initialize( void );

		RwBool							CreateWireFrame( void );
		void							DestroyWireFrame( void );

		RwBool							RenderSolid( void );
		RwBool							RenderWireFrame( void );
		RwBool							RenderBounding( void );
		RwBool							RenderIndicator( void );

		inline CAtomicEntityProperty *	GetAtomicProperty( void ) const { return reinterpret_cast<CAtomicEntityProperty *>(GetProperty()); }
		inline RpAtomic *				GetRpAtomic( void ) const { return reinterpret_cast<RpAtomic *>(GetRwInstance()); }
		RpGeometry *					GetRpGeometry( void ) const;

		virtual RwBool					CreateUVAnimation( void );
		virtual void					DestroyUVAnimation( void );

		virtual RwBool					SetPlugInData( void );
		virtual RwBool					ResetPlugInData( void );

		RwInt32							CreateInternalMeshEntity( void );

		virtual RwBool					CalculateVertexColor( SVertexColorGenerationInfo * pInfo );

		RwReal							CalculateVertexShadowFactor( const RwV3d * pVertex, const RwV3d * pLight, RwReal fMulitpleFactor, RwReal fOffset, RwReal fSubShadowFactor );
		RwReal							CalculateVertexShadowPartialFactor( RwLine * pLine, RwReal fEffectiveFactor, RwReal fSubeffectiveFactor, RwReal fOffset );
		RwReal							CalculateVertexNormalFactor( const RwV3d * pNormal, const RwV3d * pLight, RwReal fCutOff );

	protected:

		static RwBool					m_bRenderSolid;

		static RwBool					m_bRenderSolidForTerrain;
		static RwBool					m_bRenderWireFrameForTerrain;
		static RwBool					m_bRenderBoundingForTerrain;

		static RwBool					m_bRenderSolidForSelectedTerrain;
		static RwBool					m_bRenderWireFrameForSelectedTerrain;
		static RwBool					m_bRenderBoundingForSelectedTerrain;

		static RwBool					m_bRenderSolidForObject;
		static RwBool					m_bRenderWireFrameForObject;
		static RwBool					m_bRenderBoundingForObject;

		static RwBool					m_bRenderSolidForSelectedObject;
		static RwBool					m_bRenderWireFrameForSelectedObject;
		static RwBool					m_bRenderBoundingForSelectedObject;

		static RwRGBA					m_WireFrameColorForTerrain;
		static RwRGBA					m_WireFrameColorForSelectedTerrain;
		static RwRGBA					m_BoundingColorForTerrain;
		static RwRGBA					m_BoundingColorForSelectedTerrain;

		static RwRGBA					m_WireFrameColorForObject;
		static RwRGBA					m_WireFrameColorForSelectedObject;
		static RwRGBA					m_BoundingColorForObject;
		static RwRGBA					m_BoundingColorForSelectedObject;

		static RwUInt32					m_iRwInstanceExPlugInOffset;

	public:

		static void						SetRenderSolid( RwBool bRender ) { m_bRenderSolid = bRender; }

		static void						SetRenderSolidForTerrain( RwBool bRender ) { m_bRenderSolidForTerrain = bRender; }
		static void						SetRenderWireFrameForTerrain( RwBool bRender ) { m_bRenderWireFrameForTerrain = bRender; }
		static void						SetRenderBoundingForTerrain( RwBool bRender ) { m_bRenderBoundingForTerrain = bRender; }

		static void						SetRenderSolidForSelectedTerrain( RwBool bRender ) { m_bRenderSolidForSelectedTerrain = bRender; }
		static void						SetRenderWireFrameForSelectedTerrain( RwBool bRender ) { m_bRenderWireFrameForSelectedTerrain = bRender; }
		static void						SetRenderBoundingForSelectedTerrain( RwBool bRender ) { m_bRenderBoundingForSelectedTerrain = bRender; }

		static void						SetRenderSolidForObject( RwBool bRender ) { m_bRenderSolidForObject = bRender; }
		static void						SetRenderWireFrameForObject( RwBool bRender ) { m_bRenderWireFrameForObject = bRender; }
		static void						SetRenderBoundingForObject( RwBool bRender ) { m_bRenderBoundingForObject = bRender; }

		static void						SetRenderSolidForSelectedObject( RwBool bRender ) { m_bRenderSolidForSelectedObject = bRender; }
		static void						SetRenderWireFrameForSelectedObject( RwBool bRender ) { m_bRenderWireFrameForSelectedObject = bRender; }
		static void						SetRenderBoundingForSelectedObject( RwBool bRender ) { m_bRenderBoundingForSelectedObject = bRender; }

		static RwBool					IsRenderSolidForTerrain( void ) { return m_bRenderSolidForTerrain; }
		static RwBool					IsRenderWireFrameForTerrain( void ) { return m_bRenderWireFrameForTerrain; }
		static RwBool					IsRenderBoundingForTerrain( void ) { return m_bRenderBoundingForTerrain; }

		static RwBool					IsRenderSolidForObject( void ) { return m_bRenderSolidForObject; }
		static RwBool					IsRenderWireFrameForObject( void ) { return m_bRenderWireFrameForObject; }
		static RwBool					IsRenderBoundingForObject( void ) { return m_bRenderBoundingForObject; }

		static RwBool					IsRenderSolidForSelectedTerrain( void ) { return m_bRenderSolidForSelectedTerrain; }
		static RwBool					IsRenderWireFrameForSelectedTerrain( void ) { return m_bRenderWireFrameForSelectedTerrain; }
		static RwBool					IsRenderBoundingForSelectedTerrain( void ) { return m_bRenderBoundingForSelectedTerrain; }

		static RwBool					IsRenderSolidForSelectedObject( void ) { return m_bRenderSolidForSelectedObject; }
		static RwBool					IsRenderWireFrameForSelectedObject( void ) { return m_bRenderWireFrameForSelectedObject; }
		static RwBool					IsRenderBoundingForSelectedObject( void ) { return m_bRenderBoundingForSelectedObject; }

		static void						SetWireFrameColorForTerrain( RwRGBA * pColor ) { memcpy( &m_WireFrameColorForTerrain, pColor, sizeof( RwRGBA ) ); }
		static void						SetWireFrameColorForSelectedTerrain( RwRGBA * pColor ) { memcpy( &m_WireFrameColorForSelectedTerrain, pColor, sizeof( RwRGBA ) ); }
		static void						SetBoundingColorForTerrain( RwRGBA * pColor ) { memcpy( &m_BoundingColorForTerrain, pColor, sizeof( RwRGBA ) ); }
		static void						SetBoundingColorForSelectedTerrain( RwRGBA * pColor ) { memcpy( &m_BoundingColorForSelectedTerrain, pColor, sizeof( RwRGBA ) ); }

		static void						SetColorWireFrameColorForObject( RwRGBA * pColor ) { memcpy( &m_WireFrameColorForObject, pColor, sizeof( RwRGBA ) ); }
		static void						SetColorWireFrameColorForSelectedObject( RwRGBA * pColor ) { memcpy( &m_WireFrameColorForSelectedObject, pColor, sizeof( RwRGBA ) ); }
		static void						SetColorBoundingColorForObject( RwRGBA * pColor ) { memcpy( &m_BoundingColorForObject, pColor, sizeof( RwRGBA ) ); }
		static void						SetColorBoundingColorForSelectedObject( RwRGBA * pColor ) { memcpy( &m_BoundingColorForSelectedObject, pColor, sizeof( RwRGBA ) ); }

		static RwBool					RwInstanceExPlugInAttach( void );

		static void *					RwInstanceExPlugInConstructor( void * pObject, RwInt32 iOffsetInObject, RwInt32 iSizeInObject );
		static void *					RwInstanceExPlugInCopier( void * pDstRpAtomic, const void * pSrcRpAtomic, RwInt32 iOffsetInObject, RwInt32 iSizeInObject );

		inline static SRwInstanceEx *	GetPlugInDataFromRpAtomic( RpAtomic * pAtomic )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pAtomic);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			return reinterpret_cast<SRwInstanceEx *>(iPlugInAddress);
		}

		inline static const SRwInstanceEx *	GetPlugInDataFromRpAtomic( const RpAtomic * pAtomic )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pAtomic);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			return reinterpret_cast<SRwInstanceEx *>(iPlugInAddress);
		}

		inline static RpAtomic * SetPlugInDataToRpAtomic( RpAtomic * pAtomic, const SRwInstanceEx * pPlugInData )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pAtomic);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			reinterpret_cast<SRwInstanceEx *>(iPlugInAddress)->SetEntity( pPlugInData->GetEntity() );

			return pAtomic;
		}

};


//=============================================================================
// End of Class : CAtomicEntity
//=============================================================================

#endif //__C_ATOMIC_ENTITY_H__

//*****************************************************************************
//
// End of File : CAtomicEntity.h
//
//*****************************************************************************