//*****************************************************************************
// File       : CWorldEntity.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_WORLD_ENTITY_H__
#define __C_WORLD_ENTITY_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

struct RpWorld;
class CCameraEntity;
class CWorldSectorEntity;
class CEntityCubeContainer;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"

#include "CEntity.h"
#include "CFixedStringList.h"

#include "CWorldEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CWorldEntity
// Desc       : 
// Author     : 
//=============================================================================

class CWorldEntity : public CEntity
{
	protected:

		CEntityCubeContainer *			m_pWorldSectorMap;

	public:

										CWorldEntity( void );
										CWorldEntity( RwInt32 iUniqueID ) : CEntity( iUniqueID ) { Initialize(); }
										CWorldEntity( const CWorldEntity & rEntity );

		virtual							~CWorldEntity( void ) { Destroy(); }

		const CWorldEntity &			operator =( const CWorldEntity & rEntity );

	public:

		virtual RwBool					Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty );
		virtual RwBool					PostCreate( void );

		virtual void					Destroy( void );
		virtual void					PreDestroy( void );

		virtual RwBool					UpdateFrame( void );
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL );

		virtual RwUInt32				Select( void );
		virtual RwUInt32				Unselect( void );

		virtual RwUInt32				StartAnimation( RwReal fTime = 0.0f );
		virtual RwUInt32				StopAnimation( RwReal fTime = 0.0f );

		virtual RwBool					Attach( CEntity * pChild );
		virtual RwBool					Detach( CEntity * pChild );

		virtual void					SetFlag( RwUInt32 iFlag, RwBool bApplyToChild = FALSE );

		inline const CFixedString *		GetPropertyName( void ) const { return GetWorldProperty()->GetName(); }

		const RwBBox *					GetBBox( void ) const;

		virtual CEntity *				SearchByRwInstance( void * pRwInstance ) const;
		virtual RwUInt32				SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultList, EEntityType eEntityType ) const;

		virtual RwBool					IsPointInBounding( const SLocation3D * pLocation ) const;

		virtual RwBool					DetectCollision( CCollisionDetectionInfo * pCollisionInfo ) const;
		RwBool							GetHeight( SLocation3D * pLocation, RwBool bUseHeightMap = TRUE ) const;

		inline CEntityCubeContainer *	GetWorldSectorMap( void ) const { return m_pWorldSectorMap; }
		CEntity *						GetWorldSector( RwInt32 iIndexX, RwInt32 iIndexY, RwInt32 iIndexZ ) const;
		CEntity *						GetWorldSector( RwInt32 iIndexX, RwInt32 iIndexZ ) const;
		CEntity *						GetWorldSector( SLocation3DInt * pLocation ) const;

		const RpWorldSector *			GetRpWorldSectorByPosition( SLocation3DInt * pOffset, SLocation3DInt * pSize ) const;

		inline const SLocation3DInt *	GetSectorMin( void ) const { return (( m_pProperty == NULL ) ? NULL : GetWorldProperty()->GetSectorMin() ); }
		inline const SLocation3DInt *	GetSectorMax( void ) const { return (( m_pProperty == NULL ) ? NULL : GetWorldProperty()->GetSectorMax() ); }

		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName );

		virtual RwBool					CreateHeightMap( RwReal fPrecision, RwBool bLoadFromFile = FALSE );

	protected:

		virtual void					Initialize( void );

		virtual RwBool					InsertChild( CEntity * pChild );
		virtual RwBool					RemoveChild( CEntity * pChild );

		RwBool							RenderWorldSector( CCameraEntity * pRenderer );

		inline CWorldEntityProperty *	GetWorldProperty( void ) const { return reinterpret_cast<CWorldEntityProperty *>(GetProperty()); }
		inline RpWorld *				GetRpWorld( void ) const { return reinterpret_cast<RpWorld *>(GetRwInstance()); }

		RwBool							AddRpClump( RpClump * pRpClump );
		RwBool							RemoveRpClump( RpClump * pRpClump );

		const RpClump *					GetRpClump( RwInt32 iClumpIndex ) const;
		RwInt32							GetRpClumpCount( void ) const;

		RwBool							AddRpAtomic( RpAtomic * pRpAtomic );
		RwBool							RemoveRpAtomic( RpAtomic *pRpAtomic );

		RwBool							AddRpLight( RpLight * pRpLight );
		RwBool							RemoveRpLight( RpLight * pRpLight );

		RwBool							AddRwCamera( RwCamera * pRwCamera );
		RwBool							RemoveRwCamera( RwCamera * pRwCamera );

		RwBool							CreateWorldSectorMap( const CWorldEntityProperty * pWorldProperty );
		void							DestroyWorldSectorMap( void );

		virtual RwInt32					ExportChild( CFixedStringList * pStringList, CFixedString * pName );

		virtual RwBool					ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );
		virtual RwBool					ReportSelfState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );

		virtual RwBool					SetPlugInData( void );
		virtual RwBool					ResetPlugInData( void );

		virtual RwBool					CalculateVertexColor( SVertexColorGenerationInfo * pInfo );

	protected:

		static RwUInt32					m_iRwInstanceExPlugInOffset;

	public:

		static RwBool					RwInstanceExPlugInAttach( void );

		static void *					RwInstanceExPlugInConstructor( void * pObject, RwInt32 iOffsetInObject, RwInt32 iSizeInObject );
		static void *					RwInstanceExPlugInCopier( void * pDstRpWorld, const void * pSrcRpWorld, RwInt32 iOffsetInObject, RwInt32 iSizeInObject );

		inline static SRwInstanceEx *	GetPlugInDataFromRpWorld( RpWorld * pWorld )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pWorld);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			return reinterpret_cast<SRwInstanceEx *>(iPlugInAddress);
		}

		inline static const SRwInstanceEx *	GetPlugInDataFromRpWorld( const RpWorld * pWorld )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pWorld);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			return reinterpret_cast<SRwInstanceEx *>(iPlugInAddress);
		}

		inline static RpWorld * SetPlugInDataToRpWorld( RpWorld * pWorld, const SRwInstanceEx * pPlugInData )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pWorld);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			reinterpret_cast<SRwInstanceEx *>(iPlugInAddress)->SetEntity( pPlugInData->GetEntity() );

			return pWorld;
		}
};


//=============================================================================
// End of Class : CWorldEntity
//=============================================================================

#endif //__C_WORLD_ENTITY_H__

//*****************************************************************************
//
// End of File : CWorldEntity.h
//
//*****************************************************************************