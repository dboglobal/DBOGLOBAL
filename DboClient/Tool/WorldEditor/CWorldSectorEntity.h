//*****************************************************************************
// File       : CWorldSectorEntity.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_WORLD_SECTOR_ENTITY_H__
#define __C_WORLD_SECTOR_ENTITY_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CAtomicEntity;
class CCameraEntity;
class CEntityCubeContainer;
class CEntityContainer;
class CEntityCubeContainer;
class CHeightMap;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"

#include "CFixedString.h"

#include "CEntity.h"

#include "CWorldSectorEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CWorldSectorEntity
// Desc       : 
// Author     : 
//=============================================================================

class CWorldSectorEntity : public CEntity
{
	protected:

		RwBool							m_bEmpty;
		RwBool							m_bLocked;

		CEntity *						m_pTerrainEntity;
		SPosition3D						m_Position;

		CHeightMap *					m_pHeightMap;

	public:

										CWorldSectorEntity( void );
										CWorldSectorEntity( RwInt32 iUniqueID ) : CEntity( iUniqueID ) { Initialize(); }
										CWorldSectorEntity( const CWorldSectorEntity & rSector );

		virtual							~CWorldSectorEntity( void ) { Destroy(); }

	public:

		const CWorldSectorEntity &		operator =( const CWorldSectorEntity & rSector );

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

		inline RwBool					IsEmpty( void ) const { return m_bEmpty; }

		virtual RwBool					SetParameter( void );

		virtual RwBool					Attach( CEntity * pChild );
		virtual RwBool					Detach( CEntity * pChild );

		virtual void					SetFlag( RwUInt32 iFlag, RwBool bApplyToChild = FALSE );

		inline RwBool					IsLocked( void ) const { return m_bLocked; }

		inline const CEntity *			GetTerrainEntity( void ) const { return m_pTerrainEntity; }
		inline const CEntityContainer *	GetSectorContainer( void ) const { return ( (m_pProperty == NULL ) ? NULL : GetWorldSectorProperty()->GetSectorContainer() ); }
		inline const SLocation3DInt *	GetSectorIndex( void ) const { return ( (m_pProperty == NULL ) ? NULL : GetWorldSectorProperty()->GetSectorIndex() ); }
		virtual RwUInt32				SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultList, EEntityType eEntityType ) const;

		inline virtual const SPosition3D *	GetPosition( void ) const { return &m_Position; }
		const RwBBox *					GetBBox( void ) const;

		virtual CEntity *				SearchByRwInstance( void * pRwInstance ) const;

		virtual void					SetRwFrame( const RwFrame * pRwFrame );
		virtual RwFrame *				GetRwFrame( void ) const;

		virtual RwBool					DetectCollision( CCollisionDetectionInfo * pCollisionInfo ) const;
		RwBool							GetHeight( SLocation3D * pLocation, RwBool bUseHeightMap = TRUE ) const;

		virtual RwBool					IsPointInBounding( const SLocation3D * pLocation ) const{ return ::IsPointInBBox( pLocation->GetRwV3d(), &(GetRpWorldSector()->boundingBox) ); }

		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName );

		virtual RwBool					CreateHeightMap( RwReal fPrecision, RwBool bLoadFromFile = FALSE );

		virtual RwBool					CalculateVertexColor( SVertexColorGenerationInfo * pInfo );

	protected:

		virtual void					Initialize( void );

		inline CWorldSectorEntityProperty *	GetWorldSectorProperty( void ) const { return reinterpret_cast<CWorldSectorEntityProperty *>(GetProperty()); }
		inline RpWorldSector *			GetRpWorldSector( void ) const { return reinterpret_cast<RpWorldSector *>(GetRwInstance()); }

		RwBool							RenderWorldSectorAtomic( CEntity * pRenderer );

		RwInt32							CreateInternalMeshEntity( void );

		virtual void					SetDefaultName( void );

		RwBool							LoadTerrain( void );
		RwBool							UnloadTerrain( void );

		inline void						SetEmpty( RwBool bEmpty ) { m_bEmpty = bEmpty; }

		virtual RwBool					CalculateVisibilty( void ) const;

		inline void						Lock( void ) { m_bLocked = TRUE; }
		inline void						Unlock( void ) { m_bLocked = FALSE; }

		virtual void					InitializeChildren( void );
		virtual void					DestroyChildren( void );

		virtual RwBool					InsertChild( CEntity * pChild );
		virtual RwBool					RemoveChild( CEntity * pChild );

		RwInt32							ExportSector( CFixedStringList * pStringList, CFixedString * pName );

		virtual RwBool					ReportSelfState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );
		virtual RwBool					ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );

		virtual void					DestroyHeightMap( void );

		virtual RwBool					SetPlugInData( void );
		virtual RwBool					ResetPlugInData( void );

	protected:

		static CFixedString 			m_FilePath;
		static CFixedString 			m_FileExt;

	protected:

		static RwUInt32					m_iRwInstanceExPlugInOffset;

	public:

		static void						SetFilePath( CFixedString & rFilePath ) { m_FilePath.Assign( rFilePath ); }
		static const CFixedString *		GetFilePath( void ) { return &m_FilePath; }

		static void						SetFileExt( CFixedString & rFileExt ) { m_FileExt.Assign( rFileExt ); }
		static const CFixedString *		GetFileExt( void ) { return &m_FileExt; }

		static RwBool					RwInstanceExPlugInAttach( void );

		static void *					RwInstanceExPlugInConstructor( void * pObject, RwInt32 iOffsetInObject, RwInt32 iSizeInObject );
		static void *					RwInstanceExPlugInCopier( void * pDstRpWorldSector, const void * pSrcRpWorldSector, RwInt32 iOffsetInObject, RwInt32 iSizeInObject );

		inline static SRwInstanceEx *	GetPlugInDataFromRpWorldSector( RpWorldSector * pWorldSector )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pWorldSector);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			return reinterpret_cast<SRwInstanceEx *>(iPlugInAddress);
		}

		inline static const SRwInstanceEx *	GetPlugInDataFromRpWorldSector( const RpWorldSector * pWorldSector )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pWorldSector);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			return reinterpret_cast<SRwInstanceEx *>(iPlugInAddress);
		}

		inline static RpWorldSector * SetPlugInDataToRpWorldSector( RpWorldSector * pWorldSector, const SRwInstanceEx * pPlugInData )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pWorldSector);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			reinterpret_cast<SRwInstanceEx *>(iPlugInAddress)->SetEntity( pPlugInData->GetEntity() );

			return pWorldSector;
		}

};

//=============================================================================
// End of Class : CWorldSectorEntity
//=============================================================================

static inline RwBool IsWorldSector( const RpSector * pSector )
{
	if( pSector->type >= 0 )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


#endif //__C_WORLD_SECTOR_ENTITY_H__

//*****************************************************************************
//
// End of File : CWorldSectorEntity.h
//
//*****************************************************************************