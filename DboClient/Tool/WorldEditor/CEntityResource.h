//*****************************************************************************
// File       : CEntityResource.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_RESOURCE_H__
#define __C_ENTITY_RESOURCE_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CEntityProperty;
class CAtomicEntityProperty;
class CClumpEntityProperty;
class CLightEntityProperty;
class CCameraEntityProperty;
class CWorldEntityProperty;
class CAnimationEntityProperty;

class CNtlPLObject;


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

typedef enum EEntityResourceCategory
{
	E_ENTITY_RESOURCE_CATEGORY_NONE		= 0,
	E_ENTITY_RESOURCE_CATEGORY_DIRECT	= 1,
	E_ENTITY_RESOURCE_CATEGORY_INDIRECT	= 2,
	E_ENTITY_RESOURCE_CATEGORY_PL		= 3,
	E_ENTITY_RESOURCE_CATEGORY_COUNT
} EEntityResourceCategory;


//=============================================================================
// Name       : CEntityResource
// Desc       : 
// Author     : 
//=============================================================================

class CEntityResource
{
	protected:

		EEntityType						m_Type;
		EEntityResourceCategory			m_Category;

		void *							m_pInstance;
		CNtlPLResource *				m_pPLResource;

	public:

										CEntityResource( void ) { Initialize(); }
										CEntityResource( const CEntityResource & rEntityResource );

		virtual							~CEntityResource( void ) { Destroy(); }

		const CEntityResource &			operator =( const CEntityResource & rEntityResource );

	public:

		virtual RwBool					Create( const CEntityProperty * pProperty );
		virtual void					Destroy( void );
		virtual RwBool					Export( const CFixedString * pString ) const;

		EEntityType						GetType( void ) const { return m_Type; }
		EEntityResourceCategory			GetCategory( void ) const { return m_Category; }
		const void *					GetInstance( void ) const { return m_pInstance; }

	protected:

		virtual void					Initialize( void );

		const CNtlPLResource *			GetPLResource( void ) const { return m_pPLResource; }

		void							SetType( EEntityType Type ) { m_Type = Type; }
		void							SetCategory( EEntityResourceCategory Category ) { m_Category = Category; }
		void							SetInstance( const void * pInstance ) { m_pInstance = const_cast<void *>(pInstance); }
		void							SetPLResource( CNtlPLResource * pPLResource ) { m_pPLResource = pPLResource; }

	protected:

		RpLight *						CreateRpLight( const CLightEntityProperty * pProperty );
		RpLight *						CreateRpLightOfAmbient( const CLightEntityProperty * pProperty );
		RpLight *						CreateRpLightOfDirectional( const CLightEntityProperty * pProperty );
		RpLight *						CreateRpLightOfPoint( const CLightEntityProperty * pProperty );
		RpLight *						CreateRpLightOfSpot( const CLightEntityProperty * pProperty );
		RpLight *						CreateRpLightOfSoftSpot( const CLightEntityProperty * pProperty );
		void							DestroyRpLight( RpLight * pRpLight, RwBool bDestroyFrame = TRUE );

		RwCamera *						CreateRwCamera( const CCameraEntityProperty * pProperty );
		void							DestroyRwCamera( RwCamera * pRwCamera, RwBool bDestroyFrame = TRUE );

		RpWorld *						CreateRpWorld( const CWorldEntityProperty * pProperty );

		RpClump *						CreateRpClump( const CClumpEntityProperty * pProperty );

		RpAtomic *						CreateRpAtomic( const CAtomicEntityProperty * pProperty );
		void							DestroyRpAtomic( RpAtomic * pRpAtomic, RwBool bDestroyFrame = TRUE );

		CNtlPLResource *				CreatePlObject( const CClumpEntityProperty * pProperty );
		CNtlPLResource *				CreatePlAnimation( const CAnimationEntityProperty * pProperty );

		RwBool							ExportRpAtomic( const RwChar * pString ) const;

	public:

		static void						GetTextureDictionaryName( RwChar * texDictionaryName, RwChar * bspPath );
		static RwTexDictionary *		LoadTextureDictionary(RwChar *filename);

		static RwInt32					FindCharacterLastPosition( const RwChar * pFileName, RwChar chCharacterToFind );

		static RwInt32					GetFileExtPosition( const RwChar * pFileName );
		static RwInt32					GetFilePathPosition( const RwChar * pFileName );

		static RwBool					SetModelTexturePath( const RwChar * pModelPathName );

		static RpWorld *				LoadWorld( const RwChar * pFilename );
		static RpClump *				LoadClump( const RwChar * pFilename );
		static RpAtomic *				LoadAtomic( const RwChar * pFilename );

		static RwBool					SaveClump( const RwChar * pFilename, const RpClump * pClump );

		static const RwChar *			GetWorldBspPath( void ) { return WORLD_BSP_PATH; }
		static const RwChar *			GetWorldBspExt( void ) { return WORLD_BSP_EXT; }
		static const RwChar *			GetWorldMeshPath( void ) { return WORLD_MESH_PATH; }
		static const RwChar *			GetWorldMeshExt( void ) { return WORLD_MESH_EXT; }
		static const RwChar *			GetWorldTexturePath( void ) { return WORLD_TEXTURE_PATH; }

		static const RwChar *			GetClumpMeshPath( void ) { return CLUMP_MESH_PATH; }
		static const RwChar *			GetClumpMeshExt( void ) { return CLUMP_MESH_EXT; }
		static const RwChar *			GetClumpTexturePath( void ) { return CLUMP_TEXTURE_PATH; }

		static const RwChar *			GetAtomicMeshPath( void ) { return ATOMIC_MESH_PATH; }
		static const RwChar *			GetAtomicMeshExt( void ) { return ATOMIC_MESH_EXT; }
		static const RwChar *			GetAtomicTexturePath( void ) { return ATOMIC_TEXTURE_PATH; }

		static const RwChar *			GetAnimationHierarchyPath( void ) { return ANIMATION_HIERARCHY_PATH; }
		static const RwChar *			GetAnimationHierarchyExt( void ) { return ANIMATION_HIERARCHY_EXT; }

	protected:

		static const RwChar				WORLD_BSP_PATH[];
		static const RwChar				WORLD_BSP_EXT[];
		static const RwChar				WORLD_MESH_PATH[];
		static const RwChar				WORLD_MESH_EXT[];
		static const RwChar				WORLD_TEXTURE_PATH[];

		static const RwChar				CLUMP_MESH_PATH[];
		static const RwChar				CLUMP_MESH_EXT[];
		static const RwChar				CLUMP_TEXTURE_PATH[];

		static const RwChar				ATOMIC_MESH_PATH[];
		static const RwChar				ATOMIC_MESH_EXT[];
		static const RwChar				ATOMIC_TEXTURE_PATH[];

		static const RwChar				ANIMATION_HIERARCHY_PATH[];
		static const RwChar				ANIMATION_HIERARCHY_EXT[];

};


//=============================================================================
// End of Class : CEntityResource
//=============================================================================

struct RtWorldImportBuildSector;
struct RtWorldImportBuildStatus;
struct RtWorldImportPartition;

RwReal WorldImportPartitionBuildCB(RtWorldImportBuildSector *buildSector, RtWorldImportBuildStatus *buildStatus, RtWorldImportPartition *partition, void *pData);
RwBool WorldImportPartitionTerminateCB(RtWorldImportBuildSector *buildSector, RtWorldImportBuildStatus *buildStatus, void *pData);


RpAtomic * BuildAtomicCollisionDataCB(RpAtomic * atomic, void *data);


#endif //__C_ENTITY_RESOURCE_H__

//*****************************************************************************
//
// End of File : CEntityResource.h
//
//*****************************************************************************