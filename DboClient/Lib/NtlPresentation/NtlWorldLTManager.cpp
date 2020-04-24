#include "precomp_ntlpresentation.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlPLSceneManager.h"
#include "NtlPLVisualManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldLTManager.h"
#include "NtlPLGlobal.h"
#include "NtlWorldDI.h"
#include "NtlAtomic.h"
#include "NtlPLObject.h"
#include "NtlText.h"

#include <string>
using std::string;


CNtlWorldLTManager*			CNtlWorldLTManager::g_pNtlWorldLTManager	= NULL;
RwReal						CNtlWorldLTManager::m_LocalLightMapDensity	= 10.0f;
vector<CNtlPLEntity*>		CNtlWorldLTManager::m_vecEntities4UVReset;
vector<RwTexture*>			CNtlWorldLTManager::m_vecBSPTextures;


CNtlWorldLTManager::CNtlWorldLTManager()
{
	::ZeroMemory(&m_LightingSession, sizeof(RtLtMapLightingSession));

	// Initialise the lighting scene to 'no lighting', and 'not working out any lighting'
	m_LightingSession.startObj	= 0;
	m_LightingSession.totalObj	= 0;

	m_LightMapSize		= 512;
	m_LightMapDensity	= 4.0f;
	m_LightMapScale		= 3;
	m_RenderStyle		= static_cast<RpLtMapStyle>(rpLTMAPSTYLERENDERBASE | rpLTMAPSTYLERENDERLIGHTMAP);

	strcpy_s(m_LightMapPrefix, 32, "NTL_");

	m_bCreatingLightMaps = FALSE;
	m_bClearingLightMaps = FALSE;

	m_bLightingWorld		= FALSE;
	m_bLightProgress		= 0; 
	m_bResetLighting		= FALSE;
	m_bLightingProgress		= 0.0f;
	m_bFullSceneLighting	= FALSE;

	m_bAutoSaving			= FALSE;
	m_AutoSaveTime			= 0.0f;
	m_bAutoSaveTextUI		= FALSE;
}

CNtlWorldLTManager::~CNtlWorldLTManager()
{
}

void CNtlWorldLTManager::CreateInstance()
{
	if(!g_pNtlWorldLTManager)
	{
		g_pNtlWorldLTManager = NTL_NEW CNtlWorldLTManager;
	}
}

void CNtlWorldLTManager::DestroyInstance()
{
	NTL_DELETE(g_pNtlWorldLTManager);
}

RwTexDictionary* CNtlWorldLTManager::TexDictDestroyCB(RwTexDictionary *texDict, void *data __RWUNUSED__)
{
	RwTexDictionaryDestroy(texDict);

	return(texDict);
}

RpWorldSector* CNtlWorldLTManager::SectorSetupCB(RpWorldSector* _pSector, void* _pData)
{
	// All objects get lightmapped by default
	RtLtMapWorldSectorSetFlags(_pSector, RtLtMapWorldSectorGetFlags(_pSector) | rtLTMAPOBJECTLIGHTMAP);

	return(_pSector);
}

RpMaterial* CNtlWorldLTManager::SetupMaterialsCB(RpMaterial* _pMat, void* _pData)
{
	// Most textures need to be lightmapped
	RtLtMapMaterialSetFlags(_pMat, RtLtMapMaterialGetFlags(_pMat) | rtLTMAPMATERIALLIGHTMAP);

	// Set up the lightmapping density for this material. This will be multiplied by a global density later
	if(_pMat->texture)
		RtLtMapMaterialSetLightMapDensityModifier(_pMat, CNtlWorldLTManager::m_LocalLightMapDensity);

	return(_pMat);
}

RpMaterial* CNtlWorldLTManager::ScaleMaterialsDensityCB(RpMaterial* _pMat, void* _pData)
{
	RwReal origDensity;

	origDensity = RtLtMapMaterialGetLightMapDensityModifier(_pMat);

	// Set up the lightmapping density for this material
	RtLtMapMaterialSetLightMapDensityModifier(_pMat, origDensity * (*(RwReal*)_pData));

	return _pMat;
}

void CNtlWorldLTManager::Create()
{
	// Lighting for the current slice should begin with this object. It is the user's responsibility to increment this value after each slice to process the next slice of objects.
	m_LightingSession.startObj	= 0;

	// This specifies how many objects needs to be processed for the current session. Internal use only.
	m_LightingSession.totalObj	= 0;

	// initialize our lighting session
	RtLtMapLightingSessionInitialize(&m_LightingSession, CNtlPLGlobal::m_pRpWorld);

	// Set up the camera in our lighting session. Note that we leave the _pAtomic / sector lists NULL as we wish to light the world and all _pAtomics inside it (well, those within the camera frustum)
	m_LightingSession.camera = CNtlPLGlobal::m_RwCamera;

	// This density is reasonable for the example level (it lights fairly quickly and all objects fit into one 512x512 lightmap),but can be modified from the menu.
	RtLtMapLightMapSetDefaultSize(m_LightMapSize);

	// Set up sector properties for the BSP
	RpWorldForAllWorldSectors(CNtlPLGlobal::m_pRpWorld, SectorSetupCB, NULL);

	// Set up material properties for the BSP
	RpWorldForAllMaterials(CNtlPLGlobal::m_pRpWorld, SetupMaterialsCB, NULL);

	// Use LightMapping tool to calculate a density that hopefully fits all world onto a single lightmap
	RwReal WorldDensity = RtLtMapWorldCalculateDensity(CNtlPLGlobal::m_pRpWorld);
	RpWorldForAllMaterials(CNtlPLGlobal::m_pRpWorld, ScaleMaterialsDensityCB, (void*)&WorldDensity);

	// 	Alter the naming of lightmaps
	RtLtMapSetDefaultPrefixString(m_LightMapPrefix);

 	// Get it rendering with the current style
 	RpLtMapSetRenderStyle(m_RenderStyle, CNtlPLGlobal::m_pRpWorld);
}

void CNtlWorldLTManager::Destroy()
{
	DestroyLightmaps(TRUE);

	RtLtMapLightingSessionDeInitialize(&m_LightingSession);
}

void CNtlWorldLTManager::Restore()
{

}

void CNtlWorldLTManager::CreateLightMaps()
{
	RwCamera* pOldCamera;

	if(!m_bCreatingLightMaps)
		return;

	// Create lightmaps for the entire scene
	pOldCamera = m_LightingSession.camera;

	// Put the current camera in the m_LightingSession.camera then the area of camera frustum will be created
	if(m_bFullSceneLighting)
		m_LightingSession.camera = NULL;

	RtLtMapLightMapsCreate(&m_LightingSession, m_LightMapDensity, (RwRGBA *)NULL);

	if(m_bFullSceneLighting)
		m_LightingSession.camera = pOldCamera;

	m_bCreatingLightMaps = FALSE;

	// Do this primarily to ensure prelights are black (if they're white, you can't see the lightmaps!)
	m_bClearingLightMaps = TRUE;
}

RpWorldSector* CNtlWorldLTManager::SectorPreLightClearCB(RpWorldSector* _pSector, void* _pData)
{
	RwBool bClearVertexLitObjects = *(RwBool*)_pData;

	// Initialize prelights
	if(_pSector->preLitLum)
	{
		// We only clear vertex-lit objects if we're explicitly told to
		if (bClearVertexLitObjects || (!(RtLtMapWorldSectorGetFlags(_pSector) & rtLTMAPOBJECTVERTEXLIGHT)) )
		{
			RwRGBA		ClearCol = {128, 128, 128, 255}, black = {0, 0, 0, 255};
			RwUInt32	i;

			// We clear lightmapped objects to black, non-lightmapped ones to grey (the checkerboard makes cleared, lightmapped objects visible)
			if(RpLtMapWorldSectorGetLightMap(_pSector))
			{
				ClearCol = black;
			}

			for(i = 0;i < (RwUInt32)_pSector->numVertices;i++)
			{
				_pSector->preLitLum[i] = ClearCol;
			}
		}
	}

	return(_pSector);
}

RpAtomic* CNtlWorldLTManager::AtomicPreLightClearAll128CB(RpAtomic* _pAtomic, void* _pData)
{
	RwBool			bClearVertexLitObjects	= *(RwBool*)_pData;
	RpGeometry*		pGeom					= RpAtomicGetGeometry(_pAtomic);
	static RwRGBA	ClearCol				= {128, 128, 128, 255};


	if(!(RtLtMapAtomicGetFlags(_pAtomic) & rtLTMAPOBJECTLIGHTMAP))
		return _pAtomic;

	// Are there any prelights to clear?
	if(!RpGeometryGetPreLightColors(pGeom))
		return _pAtomic;

	// We only clear vertex-lit objects if we're explicitly told to
	if(!bClearVertexLitObjects && (RtLtMapAtomicGetFlags(_pAtomic) & rtLTMAPOBJECTVERTEXLIGHT))
		return _pAtomic;

	for(RwUInt32 i = 0;i < (RwUInt32)RpGeometryGetNumVertices(pGeom);i++)
	{
		RpGeometryGetPreLightColors(pGeom)[i] = ClearCol;
	}

	return(_pAtomic);
}

RpClump* CNtlWorldLTManager::ClumpPreLightClearAll128CB(RpClump* _pClump, void* _pData)
{
	RpClumpForAllAtomics(_pClump, AtomicPreLightClearAll128CB, _pData);

	return(_pClump);
}

RpAtomic* CNtlWorldLTManager::AtomicPreLightClearLMappedBlackCB(RpAtomic* _pAtomic, void* _pData)
{
	RwBool		bClearVertexLitObjects	= *(RwBool*)_pData;
	RpGeometry* pGeom					= RpAtomicGetGeometry(_pAtomic);
	RwRGBA		ClearCol				= {0, 0, 0, 255};

	if(!(RtLtMapAtomicGetFlags(_pAtomic) & rtLTMAPOBJECTLIGHTMAP))
		return _pAtomic;

	// Are there any prelights to clear?
	if(!RpGeometryGetPreLightColors(pGeom))
		return _pAtomic;

	// We only clear vertex-lit objects if we're explicitly told to
	if(!bClearVertexLitObjects && (RtLtMapAtomicGetFlags(_pAtomic) & rtLTMAPOBJECTVERTEXLIGHT))
		return _pAtomic;

	// Objects without a light map (those outside the frustrum usually) are not made black */
	if(!RpLtMapAtomicGetLightMap(_pAtomic))
		return _pAtomic;

	for(RwUInt32 i = 0;i < (RwUInt32)RpGeometryGetNumVertices(pGeom);i++)
	{
		RpGeometryGetPreLightColors(pGeom)[i] = ClearCol;
	}

	return(_pAtomic);
}

RpClump* CNtlWorldLTManager::ClumpPreLightClearLMappedBlackCB(RpClump* _pClump, void* _pData)
{
	RpClumpForAllAtomics(_pClump, AtomicPreLightClearLMappedBlackCB, _pData);

	return(_pClump);
}

void CNtlWorldLTManager::ClearLightMaps()
{
	RwBool		bClearVertexLitObjects	= TRUE;
	RwCamera*	pOldCamera				= NULL;


	if(!m_bClearingLightMaps)
		return;

	// If the scene doesn't have lightmaps already, create 'em
	if(RpLtMapWorldLightMapsQuery(m_LightingSession.world) == 0)
	{
		m_bCreatingLightMaps = TRUE;
		return;
	}

	// Clear lightmaps to a black'n'white checkerboard
	if(m_bFullSceneLighting)
		m_LightingSession.camera = NULL;

	RtLtMapLightMapsClear(&m_LightingSession, (RwRGBA*) NULL);

	if(m_bFullSceneLighting)
		m_LightingSession.camera = pOldCamera;


	// Also clear prelights to black (for lightmapped objects) or grey (for non-lightmapped objects)
	RpWorldForAllWorldSectors(m_LightingSession.world, SectorPreLightClearCB, &bClearVertexLitObjects);
	RpWorldForAllClumps(m_LightingSession.world, ClumpPreLightClearAll128CB, &bClearVertexLitObjects);
	RpWorldForAllClumps(m_LightingSession.world, ClumpPreLightClearLMappedBlackCB, &bClearVertexLitObjects);

	m_bClearingLightMaps = FALSE;
}

RpLight* CNtlWorldLTManager::LightToggleCB(RpLight* _pLight, void* _pData)
{
	RpLightSetFlags(_pLight, RpLightGetFlags(_pLight) ^ (rpLIGHTLIGHTWORLD | rpLIGHTLIGHTATOMICS));

	return(_pLight);
}

void CNtlWorldLTManager::LightScene()
{
	static RwCamera* pLightingCamera = (RwCamera*)NULL;

	RwInt32  DeltaObjs;

	if(m_bLightingWorld == FALSE || m_bResetLighting == TRUE ||	(m_LightingSession.totalObj > 0 && m_LightingSession.startObj >= m_LightingSession.totalObj))
	{
		// Lighting is now complete, or someone loaded a new world or something, so destroy any variables that were used for lighting
		m_LightingSession.totalObj	= 0;
		m_bLightingWorld			= FALSE;
		m_bResetLighting			= FALSE;
		m_bLightingProgress			= 0;

		if(pLightingCamera)
		{
			RwFrame *frame = RwCameraGetFrame(pLightingCamera);
			RwCameraSetFrame(pLightingCamera, (RwFrame *)NULL);
			RwFrameDestroy(frame);
			RpWorldRemoveCamera(m_LightingSession.world, pLightingCamera);
			RwCameraDestroy(pLightingCamera);
			pLightingCamera = (RwCamera *)NULL;
		}

		return;
	}

	if (RpLtMapWorldLightMapsQuery(m_LightingSession.world) == 0)
	{
		// Create lightmaps if they don't exist already
		m_bCreatingLightMaps = TRUE;

		return;
	}
	
	// Beginning to lightmap a new collection of geometries / _pAtomics?
	if(m_LightingSession.totalObj == 0)
	{
		// We don't call ClearWorld() here because it is unnecessary. Any new lighting will overwrite old lighting values. So,
		// if you move any lights and then relight, you'll see the lightmaps change in only the affected areas. You could, for
		// efficiency, only relight the objects in the ROI of the change. Capture the current camera to allow it to safely move around during lighting
		if((NULL != CNtlPLGlobal::m_RwCamera) && (NULL == pLightingCamera))
		{
			RwMatrix	matrix;
			RwFrame*	pFrame;

			if(m_bFullSceneLighting)
				pLightingCamera = NULL;
			else
			{
				// Note: RpCameraClone() automatically adds the clone camera to the world owning the source camera
				pLightingCamera = RwCameraClone(CNtlPLGlobal::m_RwCamera);

				// Fix it in place 
				pFrame = RwFrameCreate();
				RwCameraSetFrame(pLightingCamera, pFrame);
				RwMatrixCopy(&matrix, RwFrameGetLTM(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
				RwFrameTransform(pFrame, &matrix, rwCOMBINEREPLACE);

				// Gather sectors-in-frustum
				// we've got heaps of problems here; effect camera must the one
				//RwCameraBeginUpdate(CNtlPLGlobal::m_RwCamera);
				//RwCameraEndUpdate(CNtlPLGlobal::m_RwCamera);
			}
		}

		m_LightingSession.startObj = 0;
	}

	// m_LightingSession.startObj persists from the last slice and we process some more samples
	m_LightingSession.numObj = 1;

	// parameters.totalSamples gets filled by RtLtMapWorldIlluminate

	// Toggle light flags so that static lights get used and dynamic ones don't
	RpWorldForAllLights(m_LightingSession.world, LightToggleCB, NULL);

	// Use the temporary clone camera
	m_LightingSession.camera = pLightingCamera;

	// Perform one 'slice' of illumination
	DeltaObjs = RtLtMapIlluminate(&m_LightingSession, (RtLtMapAreaLightGroup *)NULL, (RwUInt32) m_LightMapScale);

	// Don't use the temporary clone camera outside of this function
	m_LightingSession.camera = CNtlPLGlobal::m_RwCamera;

	// Toggle light flags back so that dynamic lights get used and static ones don't
	RpWorldForAllLights(m_LightingSession.world, LightToggleCB, NULL);

	m_LightingSession.startObj += DeltaObjs;

	m_bLightProgress = (RwReal) m_LightingSession.startObj / (RwReal)m_LightingSession.totalObj;
}

void CNtlWorldLTManager::DestroyLightmaps(RwBool _DestroyAll/* = FALSE*/)
{
	RwBool		bClearVertexLitObjects	= FALSE;
	RwCamera*	pBackupCam				= NULL;

	m_bResetLighting = TRUE;

	// Destroy the lightmaps in the scene. Temporarily disable the camera so that all lightmaps in the scene are destroyed
	if(m_bFullSceneLighting || _DestroyAll)
	{
		m_LightingSession.camera = (RwCamera*)NULL;
		pBackupCam = m_LightingSession.camera;
	}
	
	RtLtMapLightMapsDestroy(&m_LightingSession);

	if(m_bFullSceneLighting || _DestroyAll)
		m_LightingSession.camera = pBackupCam;

	// Clear prelights to white again (for non-vertex-lit objects)
	RpWorldForAllWorldSectors(m_LightingSession.world, SectorPreLightClearCB, &bClearVertexLitObjects);
	RpWorldForAllClumps(m_LightingSession.world, ClumpPreLightClearAll128CB,  &bClearVertexLitObjects);

	// Get the new prelights reinstanced
	RwResourcesEmptyArena();

	return;
}

RpAtomic* CNtlWorldLTManager::GetBSPTex4Mesh(RpAtomic* _pAtomic, void* _pData)
{
	RwBool			AlreadyExisted	= FALSE;
	CNtlPLEntity*	pPLEntity		= static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(_pAtomic));


	// in case of object entities
	if(pPLEntity)
		return _pAtomic;

	// no lightmaps
	RwTexture* pBSPLightmap	= RpLtMapAtomicGetLightMap(_pAtomic);

	if(!pBSPLightmap)
		return _pAtomic;

	for(RwUInt32 i = 0; i < CNtlWorldLTManager::m_vecBSPTextures.size(); ++i)
	{
		if(static_cast<RwTexture*>(CNtlWorldLTManager::m_vecBSPTextures[i]) == pBSPLightmap)
		{
			AlreadyExisted = TRUE;
			break;
		}
	}

	if(!AlreadyExisted)
		CNtlWorldLTManager::m_vecBSPTextures.push_back(pBSPLightmap);

	return _pAtomic;
}

RpWorldSector* CNtlWorldLTManager::GetBSPTex4Sector(RpWorldSector* _pSector, void* _pData)
{
	RwBool		AlreadyExisted	= FALSE;
	RwTexture*	pLightmap		= RpLtMapWorldSectorGetLightMap(_pSector);

	// in case of object entities
	if(pLightmap == NULL)
		return _pSector;

	for(RwUInt32 i = 0; i < CNtlWorldLTManager::m_vecBSPTextures.size(); ++i)
	{
		if(static_cast<RwTexture*>(CNtlWorldLTManager::m_vecBSPTextures[i]) == pLightmap)
		{
			AlreadyExisted = TRUE;
			break;
		}
	}

	if(!AlreadyExisted)
		CNtlWorldLTManager::m_vecBSPTextures.push_back(pLightmap);

	return _pSector;
}

RwBool CNtlWorldLTManager::SaveBSPLightmap()
{
	// delete bsp lightmaps
	RwChar			szBPS[64];

	_itoa_s(dGET_WORLD_PARAM()->WorldResourceID, szBPS, sizeof(szBPS), 10);

	string strFullPath	= dGET_WORLD_PARAM()->CurWorkingFolderName;
	string strSubPath	= "texture\\ntlwe\\ltmap\\";
	string strBSPPath	= szBPS;
	string strRstPath	= "\\bsplm";


	WIN32_FIND_DATA FindFileData;

	string  strPNG		= "\\*.png";
	string	strFindPath = strSubPath + strBSPPath + strRstPath + strPNG;
	HANDLE	hFind		= FindFirstFile(strFindPath.c_str(), &FindFileData);

	do 
	{
		if(FILE_ATTRIBUTE_DIRECTORY != (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && '.' != FindFileData.cFileName[0])
		{
			string strBSlash	= "\\";
			string strFileName	= FindFileData.cFileName;
			string strDel		= strSubPath + strBSPPath + strRstPath + strBSlash + strFileName;
			DeleteFile(strDel.c_str());
		}
	}
	while(FindNextFile(hFind, &FindFileData) != 0);

	FindClose( hFind );

	// save bsp lightmaps
	m_vecBSPTextures.clear();
	
	RpWorldForAllWorldSectors(CNtlPLGlobal::m_pRpWorld, GetBSPTex4Sector, NULL);
	
	for(RwUInt32 i = 0; i < m_vecBSPTextures.size(); ++i)
	{
		RwRaster* pRaster		= RwTextureGetRaster(static_cast<RwTexture*>(m_vecBSPTextures[i]));
		RwImage*  pLightMap32	= RwImageCreate(RwRasterGetWidth(pRaster), RwRasterGetHeight(pRaster), 32);
	
		pLightMap32 = RwImageAllocatePixels(pLightMap32);
		pLightMap32 = RwImageSetFromRaster(pLightMap32, pRaster);
	
		string strFullPath	= dGET_WORLD_PARAM()->CurWorkingFolderName;
		string strSubPath	= "\\texture\\ntlwe\\ltmap\\";
		string strBSPName	= szBPS;
		string strRstName	= "\\bsplm\\";
		string strFileName	= static_cast<RwTexture*>(m_vecBSPTextures[i])->name;
		string strExt		= ".png";
	
		strFullPath = strFullPath + strSubPath + strBSPName + strRstName + strFileName + strExt;
	
		RwImageWrite(pLightMap32, strFullPath.c_str());
		RwImageDestroy(pLightMap32);
	}
	
	m_vecBSPTextures.clear();

	return TRUE;
}

RwBool CNtlWorldLTManager::SaveBSP()
{
	RwChar szBPS[64];
	_itoa_s(dGET_WORLD_PARAM()->WorldResourceID, szBPS, sizeof(szBPS), 10);

	// save BSP lightmap images
	string FullPathFolderName	= dGET_WORLD_PARAM()->WorldProjectFolderName;
	string TheRest				= "textures\\ntlwe\\ltmap\\";

	FullPathFolderName = FullPathFolderName + TheRest;

	SaveBSPLightmap();

	// save the world, now w/a second set of lightmap UVs
	string FullPathName	= dGET_WORLD_PARAM()->WorldProjectFolderName;
	string strSlash		= "\\";
	string strBSPName	= dGET_WORLD_PARAM()->WorldBSP;
	string strBSPExt	= ".bsp";

	FullPathName = FullPathName + strSlash + strBSPName + strBSPExt;

	RwStream* pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMWRITE, FullPathName.c_str());
	DBO_ASSERT(pStream, "Stream read failed.");

	if(!pStream)
		return FALSE;

	if(m_LightingSession.world)
	{
		if(!RpWorldStreamWrite(m_LightingSession.world, pStream))
		{
			RwStreamClose(pStream, NULL);

			// Restore the app's prelighting state
			return FALSE;
		}

		RwStreamClose(pStream, NULL);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldLTManager::SaveLightmap(RwTexture* _pLightmap, RwChar* _pszPLObjUID)
{
	RwRaster* pRaster		= RwTextureGetRaster(_pLightmap);
	RwImage*  pLightMap32	= RwImageCreate(RwRasterGetWidth(pRaster), RwRasterGetHeight(pRaster), 32);
	RwChar			szBPS[64];

	pLightMap32 = RwImageAllocatePixels(pLightMap32);
	pLightMap32 = RwImageSetFromRaster(pLightMap32, pRaster);

	_itoa_s(dGET_WORLD_PARAM()->WorldResourceID, szBPS, sizeof(szBPS), 10);

	string	strFullPath	= dGET_WORLD_PARAM()->CurWorkingFolderName;
	string	strSubPath	= "\\texture\\ntlwe\\ltmap\\";
	string  strBSPName	= szBPS;
	string  strRstName	= "\\Objlm\\";
	string	strExt		= ".png";
	string	strFileName	= _pszPLObjUID;

	strFullPath = strFullPath + strSubPath + strBSPName + strRstName + strFileName + strExt;
	
	RwImageWrite(pLightMap32, strFullPath.c_str());
	RwImageDestroy(pLightMap32);

	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	return TRUE;
}

RpAtomic* CNtlWorldLTManager::FindEntities4UVReset(RpAtomic* _pAtomic, void* _pData)
{
	CNtlPLEntity*	pPLEntity		= static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(_pAtomic));
	RwBool			MultiAtomEntity	= FALSE;

	if(pPLEntity->GetClassType() != PLENTITY_OBJECT)
		return _pAtomic;

	for(RwUInt32 i = 0; i < CNtlWorldLTManager::m_vecEntities4UVReset.size(); ++i)
	{
		if(static_cast<CNtlPLEntity*>(CNtlWorldLTManager::m_vecEntities4UVReset[i]) == pPLEntity)
		{
			MultiAtomEntity = TRUE;
			break;
		}
	}

	if(!MultiAtomEntity)
		CNtlWorldLTManager::m_vecEntities4UVReset.push_back(pPLEntity);

	return _pAtomic;
}

RwBool CNtlWorldLTManager::DeleteLightmapTextureFile(RwChar* _PathName)
{
	NtlFileAttrReadOnlyRelease(_PathName);

	if(DeleteFile(_PathName))
		return TRUE;
	else
	{
		DBO_TRACE(FALSE, "Lightmap file(" << _PathName << ") wasn't deleted or existed.");
	}

	return FALSE;
}

void CNtlWorldLTManager::SetAutoSave(RwBool _Flag)
{
	m_bAutoSaving	= _Flag;
	m_AutoSaveTime	= 0.0f;
}

void CNtlWorldLTManager::UpdateAutoSave(RwReal _ElapsedTime)
{
	if(m_bAutoSaving)
	{
		if(m_bAutoSaveTextUI)
		{
			SaveBSP();
			m_bAutoSaveTextUI = FALSE;
		}

		m_AutoSaveTime += _ElapsedTime;
		if(m_AutoSaveTime >= dAUTO_SAVE_TIME)
		{
			m_bAutoSaveTextUI = TRUE;
			m_AutoSaveTime -= dAUTO_SAVE_TIME;
		}
	}
}
 
void CNtlWorldLTManager::ResetPLObjUV()
{
	CNtlWorldLTManager::m_vecEntities4UVReset.clear();
	RwCameraForAllAtomicsInFrustum(m_LightingSession.camera, FindEntities4UVReset, this);

	for(RwUInt32 i = 0; i < m_vecEntities4UVReset.size(); ++i)
	{
		if(!static_cast<CNtlPLObject*>(m_vecEntities4UVReset[i])->CheckUVsSetUpForLightMapping())
			static_cast<CNtlPLObject*>(m_vecEntities4UVReset[i])->ResetUV();
	}

	CNtlWorldLTManager::m_vecEntities4UVReset.clear();
}

void CNtlWorldLTManager::ToggleLightMapVisibility(RwBool _VisibilityFlag)
{
	if(_VisibilityFlag)
		m_RenderStyle = static_cast<RpLtMapStyle>(rpLTMAPSTYLERENDERBASE | rpLTMAPSTYLERENDERLIGHTMAP);
	else
		m_RenderStyle = static_cast<RpLtMapStyle>(rpLTMAPSTYLERENDERBASE);

	// Get it rendering with the current style
	RpLtMapSetRenderStyle(m_RenderStyle, CNtlPLGlobal::m_pRpWorld);
}

void CNtlWorldLTManager::Update(RwReal _ElapsedTime)
{
 	if(dOKEY(DIK_O) && (dGET_CUR_PALETTE() == ePM_LIGHTOBJECT))
 		m_bLightingWorld = TRUE;

	CreateLightMaps();
	ClearLightMaps();
	LightScene();

	if(dOKEY(DIK_P) && (dGET_CUR_PALETTE() == ePM_LIGHTOBJECT))
	{
		DestroyLightmaps();
		ResetPLObjUV();
		m_bCreatingLightMaps = TRUE;
	}
}

RwBool CNtlWorldLTManager::RenderUIText(RtCharset* pRtCharSet, RwInt32 LenWindowWidth, RwInt32 LenWindowHeight)
{
	if(!m_bAutoSaveTextUI && !m_bLightingWorld)
		return FALSE;

	RwRGBA ClrFont, ClrBack;
	ClrFont.red		= 255;
	ClrFont.green	= 0;
	ClrFont.blue	= 0;
	ClrFont.alpha	= 255;
	ClrBack.red		= 150;
	ClrBack.green	= 150;
	ClrBack.blue	= 150;
	ClrBack.alpha	= 0;
	RtCharsetSetColors(pRtCharSet, &ClrFont, &ClrBack);

	RwChar caption[128];

	if(m_bAutoSaveTextUI)
	{
		RsSprintf(caption, RWSTRING("Saving BSP world automatically..."));

		RwV3d v2dPoint;
		v2dPoint.x = static_cast<RwReal>(LenWindowWidth / 2 - sizeof(caption));
		v2dPoint.y = static_cast<RwReal>(LenWindowWidth / 2 + 10);

		RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);	
	}

	if(m_bLightingWorld)
	{
		RsSprintf(caption, RWSTRING("Lighting world... %.2f%% complete."), 100.0f * m_bLightProgress);

		RwV3d v2dPoint;
		v2dPoint.x = static_cast<RwReal>(LenWindowWidth / 2 - sizeof(caption));
		v2dPoint.y = static_cast<RwReal>(LenWindowWidth / 2 + 10);

		RsCharsetPrint(pRtCharSet, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);
	}
	return TRUE;
}