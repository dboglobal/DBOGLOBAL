//*****************************************************************************
// File       : CEntityResource_RpInstance.cpp
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CEntityProperty.h"
#include "CAtomicEntityProperty.h"
#include "CClumpEntityProperty.h"
#include "CLightEntityProperty.h"
#include "CCameraEntityProperty.h"
#include "CWorldEntityProperty.h"
#include "CAnimationEntityProperty.h"
#include "CWorldSectorEntity.h"

#include "CEditorApplication.h"
#include "CEntityResourceManager.h"

#include "CEntityResource.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: CreateRpLight
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpLight * CEntityResource::CreateRpLight( const CLightEntityProperty * pProperty )
{
	switch( pProperty->GetLightType() )
	{
		case E_LIGHT_ENTITY_TYPE_AMBIENT:
		{
			return CreateRpLightOfAmbient( pProperty );
		}

		case E_LIGHT_ENTITY_TYPE_DIRECTIONAL:
		{
			return CreateRpLightOfDirectional( pProperty );
		}

		case E_LIGHT_ENTITY_TYPE_POINT:
		{
			return CreateRpLightOfPoint( pProperty );
		}

		case E_LIGHT_ENTITY_TYPE_SPOT:
		{
			return CreateRpLightOfSpot( pProperty );
		}

		case E_LIGHT_ENTITY_TYPE_SOFTSPOT:
		{
			return CreateRpLightOfSoftSpot( pProperty );
		}

		default:
		{
			return NULL;
		}
	}
}


//-----------------------------------------------------------------------------
// Name			: CreateRpLightOfAmbient
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpLight * CEntityResource::CreateRpLightOfAmbient( const CLightEntityProperty * pProperty )
{
	RpLight * pLight = RpLightCreate( rpLIGHTAMBIENT );
	if( pLight == NULL )
	{
		return NULL;
	}

	RpLightSetColor( pLight, pProperty->GetColor() );

//	RpLightSetFrame( pLight, RwFrameCreate() );

	return pLight;
}


//-----------------------------------------------------------------------------
// Name			: CreateRpLightOfDirectional
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpLight * CEntityResource::CreateRpLightOfDirectional( const CLightEntityProperty * pProperty )
{
	RpLight * pLight = RpLightCreate( rpLIGHTDIRECTIONAL );
	if( pLight == NULL )
	{
		return NULL;
	}

	RpLightSetColor( pLight, pProperty->GetColor() );

	RwFrame * pFrame = RwFrameCreate();

	SetFrame( pFrame, (SPosition3D *)(pProperty->GetPosition()) );

	RpLightSetFrame( pLight, pFrame );

	return pLight;
}


//-----------------------------------------------------------------------------
// Name			: CreateRpLightOfPoint
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpLight * CEntityResource::CreateRpLightOfPoint( const CLightEntityProperty * pProperty )
{
	RpLight * pLight = RpLightCreate( rpLIGHTPOINT );
	if( pLight == NULL )
	{
		return NULL;
	}

	RpLightSetColor( pLight, pProperty->GetColor() );
	RpLightSetRadius( pLight, pProperty->GetRadius() );

	RwFrame * pFrame = RwFrameCreate();

	RwFrameTranslate( pFrame, pProperty->GetPosition()->GetLocation()->GetRwV3d(), rwCOMBINEREPLACE );

	RpLightSetFrame( pLight, pFrame );

	return pLight;
}


//-----------------------------------------------------------------------------
// Name			: CreateRpLightOfSpot
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpLight * CEntityResource::CreateRpLightOfSpot( const CLightEntityProperty * pProperty )
{
	RpLight * pLight = RpLightCreate( rpLIGHTSPOT );
	if( pLight == NULL )
	{
		return NULL;
	}

	RpLightSetColor( pLight, pProperty->GetColor() );
	RpLightSetRadius( pLight, pProperty->GetRadius() );
	RpLightSetConeAngle( pLight, pProperty->GetConeAngle() );

	RwFrame * pFrame = RwFrameCreate();
	SetFrame( pFrame, (SPosition3D *)(pProperty->GetPosition()) );
	RpLightSetFrame( pLight, pFrame );

	return pLight;
}


//-----------------------------------------------------------------------------
// Name			: CreateRpLightOfSoftSpot
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpLight * CEntityResource::CreateRpLightOfSoftSpot( const CLightEntityProperty * pProperty )
{
	RpLight * pLight = RpLightCreate( rpLIGHTSPOTSOFT );
	if( pLight == NULL )
	{
		return NULL;
	}

	RpLightSetColor( pLight, pProperty->GetColor() );
	RpLightSetRadius( pLight, pProperty->GetRadius() );
	RpLightSetConeAngle( pLight, pProperty->GetConeAngle() );

	RwFrame * pFrame = RwFrameCreate();
	SetFrame( pFrame, (SPosition3D *)(pProperty->GetPosition()) );
	RpLightSetFrame( pLight, pFrame );

	return pLight;
}


//-----------------------------------------------------------------------------
// Name			: CreateRwCamera
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwCamera * CEntityResource::CreateRwCamera( const CCameraEntityProperty * pProperty )
{
	RwCamera * pCamera = RwCameraCreate();
	if( pCamera == NULL )
	{
		return NULL;
	}

	RwCameraSetRaster( pCamera, RwRasterCreate( pProperty->GetRasterInfo()->GetWidth(), pProperty->GetRasterInfo()->GetHeight(), 0, rwRASTERTYPECAMERA ) );

	if( pProperty->GetRasterInfo()->HasZBuffer() != FALSE )
	{
		RwCameraSetZRaster( pCamera, RwRasterCreate( pProperty->GetRasterInfo()->GetWidth(), pProperty->GetRasterInfo()->GetHeight(), 0, rwRASTERTYPEZBUFFER ) );
	}

	RwCameraSetNearClipPlane( pCamera, pProperty->GetClipPlane()->GetNear() );
	RwCameraSetFarClipPlane( pCamera, pProperty->GetClipPlane()->GetFar() );
	RwCameraSetFrame( pCamera, RwFrameCreate() );

	return pCamera;
}


void CEntityResource::DestroyRpLight( RpLight * pRpLight, RwBool bDestroyFrame )
{
	RwFrame *	pFrame = RpLightGetFrame( pRpLight );;
	
	RpLightDestroy( pRpLight );

	if( bDestroyFrame != FALSE )
	{
		if( pFrame != NULL )
		{
			RwFrameDestroy( pFrame );
		}
	}
}

//-----------------------------------------------------------------------------
// Name			: CreateRpWorld
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpWorld * CEntityResource::CreateRpWorld( const CWorldEntityProperty * pProperty )
{
	// Create RtWorldImport

	if( pProperty->IsEmpty() != FALSE )
	{
		return RpWorldCreate( pProperty->GetBounding()->GetRwBBox() );
	}

	RtWorldImport *			pWorldImport;

	pWorldImport = RtWorldImportCreate();
	if( pWorldImport == NULL )
	{
		return NULL;
	}

	// Setup RtWorldImport

	const SBoundingBox *	pWorldBBox = pProperty->GetBounding();

	RtWorldImportParameters		WorldImportParameters;
	RtWorldImportParametersInit( &WorldImportParameters );

	WorldImportParameters.flags				= 0;
	WorldImportParameters.conditionGeometry	= FALSE;
	WorldImportParameters.calcNormals		= FALSE;
	WorldImportParameters.userSpecifiedBBox	= TRUE;
	WorldImportParameters.userBBox			= *(pWorldBBox->GetRwBBox());

	RwV3d					SectorSize;

	pWorldBBox->GetSize( &SectorSize );

	SSectorCreateInfo		SectorCreateInfo;
	SectorCreateInfo.SectorSize.x	= pProperty->GetBounding()->GetSizeX() / static_cast<RwReal>(pProperty->GetSectorMax()->GetX() - pProperty->GetSectorMin()->GetX() + 1);
	SectorCreateInfo.SectorSize.y	= pProperty->GetBounding()->GetSizeY() / static_cast<RwReal>(pProperty->GetSectorMax()->GetY() - pProperty->GetSectorMin()->GetY() + 1);
	SectorCreateInfo.SectorSize.z	= pProperty->GetBounding()->GetSizeZ() / static_cast<RwReal>(pProperty->GetSectorMax()->GetZ() - pProperty->GetSectorMin()->GetZ() + 1);

	// Set Create RpWorldSector Callback

	RtWorldImportSetBuildCallBacks( WorldImportPartitionBuildCB, WorldImportPartitionTerminateCB );
	RtWorldImportSetBuildCallBacksUserData( &SectorCreateInfo, &SectorCreateInfo );

	// Create RpWorld By RtWorldImport

	RpWorld *				pWorld = RtWorldImportCreateWorld( pWorldImport, &WorldImportParameters );

	SectorCreateInfo.pWorld			= pWorld;
	SectorCreateInfo.pParentFrame	= RwFrameCreate();

	// Create Terrain Atomic For All World Sector

	RtWorldImportDestroy( pWorldImport );

	return pWorld;
}


//-----------------------------------------------------------------------------
// Name			: CreateRpClump
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpClump * CEntityResource::CreateRpClump( const CClumpEntityProperty * pClumpProperty )
{
	const CNtlPLObjectProperty *	pPlObjectProperty = reinterpret_cast<const CNtlPLObjectProperty *>(CNtlPLPropertyContainer::GetInstance()->GetProperty( PLENTITY_OBJECT, pClumpProperty->GetName()->GetBuffer() ));

	RwChar		MeshPath[256];
	rwsprintf( MeshPath, RWSTRING( "%s\\%s\\%s.%s" ),
				g_pEngineApp->GetAppPath()->GetBuffer(), GetClumpMeshPath(),
				pPlObjectProperty->GetDffName(),
				GetClumpMeshExt() );

	RwChar		TexturePath[256];
	rwsprintf( TexturePath, RWSTRING( "%s\\%s\\" ),
				g_pEngineApp->GetAppPath()->GetBuffer(), GetClumpTexturePath() );

	RwImageSetPath( TexturePath );

	RpClump *	pClump = LoadClump( MeshPath );
	Assert( pClump != NULL );

	return pClump;
}


//-----------------------------------------------------------------------------
// Name			: CreateRpAtomic
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpAtomic * CEntityResource::CreateRpAtomic( const CAtomicEntityProperty * pProperty )
{
	RwChar		szFileName[256];
	rwsprintf( szFileName, RWSTRING( "%s\\%s\\%s.%s" ),
				g_pEngineApp->GetAppPath()->GetBuffer(), GetWorldMeshPath(),
				(reinterpret_cast<const CAtomicEntityProperty *>(pProperty))->GetName()->GetBuffer(),
				GetWorldMeshExt() );

	RwChar		TexturePath[256];
	rwsprintf( TexturePath, RWSTRING( "%s\\%s\\" ),
				g_pEngineApp->GetAppPath()->GetBuffer(), GetWorldTexturePath() );

	RwImageSetPath( TexturePath );

	RpAtomic *	pRpAtomic = LoadAtomic( szFileName );
	Assert( pRpAtomic != NULL );

	rwsprintf( szFileName, RWSTRING( "%s\\%s\\%s.txt" ),
				g_pEngineApp->GetAppPath()->GetBuffer(), GetWorldMeshPath(),
				(reinterpret_cast<const CAtomicEntityProperty *>(pProperty))->GetName()->GetBuffer(),
				GetWorldMeshExt() );

	CWorldMonitor::CreateInstance( szFileName );
	CWorldMonitor::GetInstance()->AnalysisObject( (void *)pRpAtomic );
	CWorldMonitor::DestroyInstance();

	return pRpAtomic;
}


//-----------------------------------------------------------------------------
// Name			: DestroyRpAtomic
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityResource::DestroyRpAtomic( RpAtomic * pRpAtomic, RwBool bDestroyFrame )
{
	Assert( pRpAtomic != NULL );

	RwFrame *	pFrame = RpAtomicGetFrame( pRpAtomic );;
	
	RpAtomicDestroy( pRpAtomic );

	if( bDestroyFrame != FALSE )
	{
		if( pFrame != NULL )
		{
			RwFrameDestroy( pFrame );
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: CreatePlObject
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNtlPLResource * CEntityResource::CreatePlObject( const CClumpEntityProperty * pProperty )
{
	RwChar		MeshPath[256];
	rwsprintf( MeshPath, RWSTRING( "%s\\%s\\%s.%s" ),
				g_pEngineApp->GetAppPath()->GetBuffer(), GetClumpMeshPath(),
				pProperty->GetName()->GetBuffer(),
				GetClumpMeshExt() );

	RwChar		TexturePath[256];
	rwsprintf( TexturePath, RWSTRING( "%s\\%s\\" ),
				g_pEngineApp->GetAppPath()->GetBuffer(), GetClumpTexturePath() );

	RwImageSetPath( TexturePath );

	CNtlPLResource * pPlResource = CNtlPLResourceManager::GetInstance()->LoadClump( MeshPath, TexturePath );

//	SaveClump( pFileName, pClump );

	return pPlResource;
}



//-----------------------------------------------------------------------------
// Name			: CreatePlAnimation
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNtlPLResource * CEntityResource::CreatePlAnimation( const CAnimationEntityProperty * pProperty )
{
	RwChar		AnimationHierarchyFullPath[256];
	rwsprintf( AnimationHierarchyFullPath, RWSTRING( "%s\\%s\\%s.%s" ),
				g_pEngineApp->GetAppPath()->GetBuffer(), GetAnimationHierarchyPath(),
				pProperty->GetName()->GetBuffer(),
				GetAnimationHierarchyExt() );

	return CNtlPLResourceManager::GetInstance()->LoadAnimation( AnimationHierarchyFullPath );
}


//-----------------------------------------------------------------------------
// Name			: FindCharacterLastPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CEntityResource::FindCharacterLastPosition( const RwChar * pFileName, RwChar chCharacterToFind )
{
	RwInt32		iIndex = rwstrlen( pFileName );

	while( --iIndex >= 0 )
	{
		if( pFileName[iIndex] == chCharacterToFind )
		{
			return iIndex;
		}
    }

	return -1;
}


//-----------------------------------------------------------------------------
// Name			: GetFileExtPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CEntityResource::GetFileExtPosition( const RwChar * pFileName )
{
	return FindCharacterLastPosition( pFileName, RWCHAR_DOT );
}


//-----------------------------------------------------------------------------
// Name			: GetFilePathPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CEntityResource::GetFilePathPosition( const RwChar * pFileName )
{
	return FindCharacterLastPosition( pFileName, RWCHAR_SEPARATOR );
}


//-----------------------------------------------------------------------------
// Name			: GetTextureDictionaryName
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityResource::GetTextureDictionaryName( RwChar * pTextureDictionaryName, RwChar * pBspName )
{
#if (defined(D3D8_DRVMODEL_H))
    static const RwChar TEXTURE_DICTIONARY_NAME_EXT[] = RWSTRING( "_d3d8.txd" );
#elif (defined(D3D9_DRVMODEL_H))
    static const RwChar TEXTURE_DICTIONARY_NAME_EXT[] = RWSTRING( "_d3d9.txd" );
#elif (defined(OPENGL_DRVMODEL_H))
    static const RwChar TEXTURE_DICTIONARY_NAME_EXT[] = RWSTRING( "_ogl.txd" );
#elif (defined(SKY))
    static const RwChar TEXTURE_DICTIONARY_NAME_EXT[] = RWSTRING( "_ps2.txd" );
#elif (defined(_XBOX))
    static const RwChar TEXTURE_DICTIONARY_NAME_EXT[] = RWSTRING( "_xbox.txd" );
#elif (defined(DOLPHIN))
    static const RwChar TEXTURE_DICTIONARY_NAME_EXT[] = RWSTRING( "_gcn.txd" );
#elif (defined(SOFTRAS_DRVMODEL_H))
    static const RwChar TEXTURE_DICTIONARY_NAME_EXT[] = RWSTRING( "_sras.txd" );
#else
    static const RwChar TEXTURE_DICTIONARY_NAME_EXT[] = RWSTRING( ".txd" );
#endif

	rwstrcpy( pTextureDictionaryName, pBspName );

	pTextureDictionaryName[GetFileExtPosition( pTextureDictionaryName )] = RWCHAR_NULL;

    rwstrcat( pTextureDictionaryName, TEXTURE_DICTIONARY_NAME_EXT );

    return;
}


//-----------------------------------------------------------------------------
// Name			: LoadTextureDictionary
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwTexDictionary * CEntityResource::LoadTextureDictionary( RwChar * pTextureDictionaryName )
{
    RwChar *			pTextureDictionaryPathName = RsPathnameCreate( pTextureDictionaryName );
	RwTexDictionary *	pTextureDictionary = (RwTexDictionary *)NULL;

	if( RwFexist( pTextureDictionaryPathName ) )
	{
		RwStream * pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, pTextureDictionaryPathName );

        if( pStream != NULL )
        {
			if( RwStreamFindChunk( pStream, rwID_TEXDICTIONARY, (RwUInt32 *)NULL, (RwUInt32 *)NULL ) )
			{
				pTextureDictionary = RwTexDictionaryStreamRead( pStream );
			}

			RwStreamClose( pStream, NULL );
		}
	}

	RsPathnameDestroy( pTextureDictionaryPathName );

	return pTextureDictionary;
}


//-----------------------------------------------------------------------------
// Name			: SetModelTexturePath
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityResource::SetModelTexturePath( const RwChar * pModelPathName )
{
	RwChar		SourcePath[1024];
	RwChar		ModelTexturePath[1024]	= { RWCHAR_NULL, };
	RwChar		GlobalTexturePath[1024]	= { RWCHAR_NULL, };

	RwInt32		iIndex;
	RwChar		chTemp = RWCHAR_NULL;

	rwstrcpy( SourcePath, pModelPathName );

	iIndex = GetFileExtPosition( SourcePath );

	if( iIndex >= 0 )
	{
		Swap( SourcePath[iIndex], chTemp );
		rwstrcpy( ModelTexturePath, SourcePath );
		Swap( SourcePath[iIndex], chTemp );

		ModelTexturePath[iIndex]	= RWCHAR_SEPARATOR;
		ModelTexturePath[++iIndex]	= RWCHAR_NULL;
	}

	iIndex = GetFilePathPosition( pModelPathName );

	if( iIndex >= 0 )
	{
		Swap( SourcePath[iIndex], chTemp );
		rwstrcpy( GlobalTexturePath, SourcePath );
		Swap( SourcePath[iIndex], chTemp );

		rwstrcat( GlobalTexturePath, RWSTRING( "\\" ) );
		rwstrcat( GlobalTexturePath, RWSTRING( GLOBAL_TEXTURE_PATH ) );
		rwstrcat( GlobalTexturePath, RWSTRING( "\\;" ) );
	}

	rwstrcat( GlobalTexturePath, ModelTexturePath );

    RwImageSetPath( GlobalTexturePath );

    return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: LoadClump
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpClump * CEntityResource::LoadClump( const RwChar * pFileName )
{
	RwStream *	pStream = NULL;
	RpClump *	pClump = NULL;
	RwChar *	pPathName = NULL;
	void *		pFile = NULL;
	RwMemory	Mem;
	RwUInt32	iSize;

	pPathName = RsPathnameCreate( pFileName );

	pFile = RwFopen( pPathName, RWSTRING( "rb" ) );
	if( pFile != NULL )
	{
		RwFseek( pFile, 0L, SEEK_END );
		iSize = RwFtell( pFile );
		RwFseek( pFile, 0L, SEEK_SET );

		Mem.start	= (RwUInt8 *)RwMalloc( iSize, rwID_NAOBJECT );
		Mem.length	= iSize;

		if( RwFread( Mem.start, iSize, 1, pFile ) )
		{
			pStream = RwStreamOpen( rwSTREAMMEMORY, rwSTREAMREAD, &Mem );
			if( pStream != NULL )
			{
				if( RwStreamFindChunk( pStream, rwID_CLUMP, NULL, NULL ))
				{
					pClump = RpClumpStreamRead( pStream );
				}

				RwStreamClose( pStream, NULL );
			}
		}

		RwFree( Mem.start );
        RwFclose( pFile );
	}

	RsPathnameDestroy( pPathName );

//	SaveClump( pFileName, pClump );

	return pClump;
}


//-----------------------------------------------------------------------------
// Name			: SaveClump
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityResource::SaveClump( const RwChar * pFilename, const RpClump * pClump )
{
	RwChar *	pPathName = RsPathnameCreate( pFilename );

	RwStream *	pStream = RwStreamOpen( rwSTREAMFILENAME, rwSTREAMWRITE, pPathName );
    if( pStream == NULL)
    {
		RsPathnameDestroy( pPathName );

		return FALSE;
	}

	RpClumpForAllAtomics( const_cast<RpClump *>(pClump), BuildAtomicCollisionDataCB, NULL );

	RpClumpStreamWrite( const_cast<RpClump *>(pClump), pStream );
	RwStreamClose( pStream, NULL );
	RsPathnameDestroy( pPathName );

	return TRUE;
}


RpAtomic * BuildAtomicCollisionDataCB(RpAtomic * atomic, void *data)
{
	if( atomic->geometry != NULL )
	{
		RpCollisionGeometryBuildData( atomic->geometry, NULL );
	}

	return atomic;
}

//-----------------------------------------------------------------------------
// Name			: SaveClump
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------
/*
RwBool CEntityResource::SaveClump( const RwChar * pFilename, const RpClump * pClump )
{
	RwMemory	sMemory;
	sMemory.start	= NULL;
	sMemory.length	= 0;

	RwStream *	pStream;
	pStream = RwStreamOpen( rwSTREAMMEMORY, rwSTREAMWRITE, &sMemory );
	if( pStream == NULL )
	{
		return FALSE;
	}

	if( RpClumpStreamWrite( const_cast<RpClump *>(pClump), pStream ) == NULL )
	{
		RwStreamClose( pStream, &sMemory );

		return FALSE;
	}

	RwStreamClose( pStream, &sMemory );

	//

	RwChar *	pPathName = RsPathnameCreate( pFileName );
	void *		pFile = RwFopen( pPathName, RWSTRING( "wb" ) );
	if( pFile == NULL )
	{
		RsPathnameDestroy( pPathName );

		return FALSE;
	}

*/
/*
	RwStream *	pStream = NULL;
	RpClump *	pClump = NULL;
	RwChar *	pPathName = NULL;
	void *		pFile = NULL;
	RwMemory	Mem;
	RwUInt32	iSize;


	
		RwFseek( pFile, 0L, SEEK_END );
		iSize = RwFtell( pFile );
		RwFseek( pFile, 0L, SEEK_SET );

		Mem.start	= (RwUInt8 *)RwMalloc( iSize, rwID_NAOBJECT );
		Mem.length	= iSize;

		if( RwFread( Mem.start, iSize, 1, pFile ) )
		{
			pStream = RwStreamOpen( rwSTREAMMEMORY, rwSTREAMWRITE, &Mem );
			if( pStream != NULL )
			{
				if( RwStreamFindChunk( pStream, rwID_CLUMP, NULL, NULL ))
				{
					pClump = RpClumpStreamRead( pStream );
				}

				RwStreamClose( pStream, NULL );
			}
		}

		RwFree( Mem.start );
        RwFclose( pFile );
	}

	RsPathnameDestroy( pPathName );
	
	return TRUE;
}
*/



//-----------------------------------------------------------------------------
// Name			: LoadAtomic
// Desc			: 
// Privilege	: proetected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpAtomic * CEntityResource::LoadAtomic( const RwChar * pFileName )
{
	RpAtomic *		pAtomic = NULL;
	RwChar *		pPathname = RsPathnameCreate( pFileName );

	if( pAtomic == NULL )
	{
		RpClump     * pClump = LoadClump( pPathname );
	 
		if( pClump == NULL )
		{
			Assert( !"pClump != NULL" );

			return NULL;
		}

		if( RpClumpGetNumAtomics( pClump ) == 0 )
		{
			Assert( !"RpClumpGetNumAtomics( pClump ) > 0" );

			return NULL;
		}

  		RwLLLink *	pLink = rwLinkListGetFirstLLLink( &(pClump->atomicList) );
		pAtomic = rwLLLinkGetData( pLink, RpAtomic, inClumpLink );

		pAtomic = RpAtomicClone( pAtomic );

		RpAtomicSetFrame( pAtomic, RwFrameCreate() );

	//	RpGeometry * pGeometry = RpAtomicGetGeometry( pAtomic );
	//	RpCollisionGeometryBuildData( pGeometry, NULL );

		RpClumpDestroy( pClump );
	}

	RsPathnameDestroy( pPathname );

    return pAtomic;
}


//-----------------------------------------------------------------------------
// Name			: LoadWorld
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpWorld * CEntityResource::LoadWorld( const RwChar * filename )
{
	RwChar *		pPath;
	RwStream *		pStream;
	RpWorld *		pWorld = NULL;

	SetModelTexturePath( filename );

    pPath = RsPathnameCreate( filename );

	if( pPath == NULL )
	{
		return NULL;
	}

	pStream = RwStreamOpen( rwSTREAMFILENAME, rwSTREAMREAD, pPath );
    if( pStream == NULL )
	{
		return NULL;
	}

	if( RwStreamFindChunk( pStream, rwID_WORLD, (RwUInt32 *)NULL, (RwUInt32 *)NULL ) != NULL )
	{
        pWorld = RpWorldStreamRead( pStream );

        RwStreamClose( pStream, NULL );
    }

    RsPathnameDestroy( pPath );

	return pWorld;
}


//-----------------------------------------------------------------------------
// Name			: ExportRpAtomic
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityResource::ExportRpAtomic( const RwChar * pString ) const
{
	RwStream *		pStream = RwStreamOpen( rwSTREAMFILENAME, rwSTREAMREAD, pString );
    if( pStream == NULL)
    {
		return FALSE;
	}

	if( RwStreamFindChunk( pStream, rwID_CLUMP, NULL, NULL ) == FALSE )
	{
		RwStreamClose( pStream, NULL );

		return FALSE;
	}

	RpClump *		pClump = RpClumpStreamRead( pStream );
	if( pClump == NULL )
	{
		RwStreamClose( pStream, NULL );

		return FALSE;
	}

    RwStreamClose( pStream, NULL );

	RpAtomic *		pAtomicSrc = reinterpret_cast<RpAtomic *>(m_pInstance);
	RwLLLink *		pLink = rwLinkListGetFirstLLLink( &(pClump->atomicList) );
	RpAtomic *		pAtomicDst = rwLLLinkGetData( pLink, RpAtomic, inClumpLink );

	RpGeometry *	pGeometrySrc = RpAtomicGetGeometry( pAtomicSrc );
	RpGeometry *	pGeometryDst = RpAtomicGetGeometry( pAtomicDst );

	if( pGeometrySrc != NULL && pGeometryDst != NULL )
	{
		RwRGBA *	pColorsSrc = RpGeometryGetPreLightColors( pGeometrySrc );
		RwRGBA *	pColorsDst = RpGeometryGetPreLightColors( pGeometryDst );

		if( pColorsSrc != NULL && pColorsDst != NULL )
		{
			memcpy( pColorsDst, pColorsSrc, sizeof( RwRGBA ) * RpGeometryGetNumVertices( pGeometrySrc ) );
		}
	}

	pStream = RwStreamOpen( rwSTREAMFILENAME, rwSTREAMWRITE, pString );
    if( pStream == NULL)
    {
		RpClumpDestroy( pClump );

		return FALSE;
	}

	RpClumpStreamWrite( pClump, pStream );

    RwStreamClose( pStream, NULL );

	RpClumpDestroy( pClump );

	return TRUE;
}

/*
RwBool CEntityResource::ExportRpAtomic( const RwChar * pString ) const
{
	RwStream *		pStream = RwStreamOpen( rwSTREAMFILENAME, rwSTREAMREAD, pString );
    if( pStream == NULL)
    {
		return FALSE;
	}

	if( RwStreamFindChunk( pStream, rwID_CLUMP, NULL, NULL ) == FALSE )
	{
		RwStreamClose( pStream, NULL );

		return FALSE;
	}

	RpClump *		pClump = RpClumpStreamRead( pStream );
	if( pClump == NULL )
	{
		RwStreamClose( pStream, NULL );

		return FALSE;
	}

    RwStreamClose( pStream, NULL );

	RpAtomic *		pAtomicSrc = reinterpret_cast<RpAtomic *>(m_pInstance);
	RwLLLink *		pLink = rwLinkListGetFirstLLLink( &(pClump->atomicList) );
	RpAtomic *		pAtomicDst = rwLLLinkGetData( pLink, RpAtomic, inClumpLink );

	RpGeometry *	pGeometrySrc = RpAtomicGetGeometry( pAtomicSrc );
	RpGeometry *	pGeometryDst = RpAtomicGetGeometry( pAtomicDst );

	if( pGeometrySrc != NULL && pGeometryDst != NULL )
	{
		RwRGBA *	pColorsSrc = RpGeometryGetPreLightColors( pGeometrySrc );
		RwRGBA *	pColorsDst = RpGeometryGetPreLightColors( pGeometryDst );

		if( pColorsSrc != NULL && pColorsDst != NULL )
		{
			memcpy( pColorsDst, pColorsSrc, sizeof( RwRGBA ) * RpGeometryGetNumVertices( pGeometrySrc ) );
		}
	}

	pStream = RwStreamOpen( rwSTREAMFILENAME, rwSTREAMWRITE, pString );
    if( pStream == NULL)
    {
		RpClumpDestroy( pClump );

		return FALSE;
	}

	RpClumpStreamWrite( pClump, pStream );

    RwStreamClose( pStream, NULL );

	RpClumpDestroy( pClump );

	return TRUE;
}
*/

//*****************************************************************************
//
// End of File : CEntityResource_RpInstance.cpp
//
//*****************************************************************************