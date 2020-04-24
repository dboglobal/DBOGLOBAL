#include "precomp_ntlpresentation.h"

#include "NtlDebug.h"
#include "NtlMath.h"
#include "NtlCoreUtil.h"

#include "NtlWorldCommon.h"
#include "NtlWorldDI.h"
#include "NtlWorldInterface.h"
#include "NtlWorldPaletteDefine.h"

#include "NtlWorldSLManger.h"

#include "NtlWorldSchedule.h"
#include "NtlWorldShadow.h"
#include "NtlWorldSector.h"
#include "NtlWorldSectorPVS.h"
#include "NtlWorldSectorlod.h"
#include "NtlWorldSectorInfo.h"
#include "NtlWorldSectorManager.h"
#include "NtlWorldFieldInfo.h"
#include "NtlWorldField.h"
#include "NtlWorldFieldManager.h"
#include "NtlWorldShadowManager.h"
#include "NtlWorldBGMManager.h"
#include "NtlWorldMergeManager.h"
#include "NtlWorldPathEngineManager.h"
#include "NtlWorldLTManager.h"

#include "NtlSpecularManager.h"
#include "NtlFieldColorManager.h"
#include "NtlOccluderManager.h"


#include "NtlPLGlobal.h"
#include "NtlPLWorldState.h"
#include "NtlPLHelpers.h"
#include "NtlPLRenderState.h"
#include "NtlPLResourcePack.h"

#include "NtlPLResourceManager.h"
#include "NtlPLSceneManager.h"

#include "NtlPLEventGenerator.h"

#include "NtlPLObject.h"
#include "NtlPLSky.h"
#include "NtlPLFog.h"
#include "NtlPLWater.h"
#include "NtlPLPlant.h"
#include "NtlPLLight.h"
#include "NtlPLBloom.h"
#include "NtlPLSkyDragon.h"
#include "NtlPLPlanet.h"
#include "NtlPLWorldLight_Base.h"
#include "NtlPLWorldLight_Sphere.h"
#include "NtlPLWorldLight_Box.h"
#include "NtlPLOccluder_Base.h"
#include "NtlPLOccluder_Quad.h"
#include "NtlPLWeather.h"
#include "NtlPLLight.h"
#include "NtlPLLightObject.h"

#include "NtlPLPlanetHandler.h"
#include "NtlPLWeatherHandler.h"

#include "NtlPLPropertyContainer.h"
#include "NtlPLShoreLineProp.h"

#include "NtlPLCollision.h"

#include "C2DAABB.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

vector<int>	g_vecVisibleSectors;

static RpCollisionTriangle* _IntersectionWaterTriCB( RpIntersection *pIntersection, RpCollisionTriangle *pCollTriangle, RwReal distance, void *data )
{
	sNtlWorldCollisionInfo* pCollInfo	= static_cast<sNtlWorldCollisionInfo*>(data);
	RwLine*					pLine		= &pIntersection->t.line;
	RwV3d					vDelta		= pIntersection->t.line.end - pIntersection->t.line.start;
	RwReal					fDist		= RwV3dLength(&vDelta) * distance;

	if (pCollInfo->fDist - fDist > 0.00001f)
	{
		pCollInfo->fDist						= fDist;
		pCollInfo->RayIntersectionPt4Terrain.x	= pLine->start.x + (distance * vDelta.x);
		pCollInfo->RayIntersectionPt4Terrain.y	= pLine->start.y + (distance * vDelta.y);
		pCollInfo->RayIntersectionPt4Terrain.z	= pLine->start.z + (distance * vDelta.z);
		pCollInfo->IsCollidedAtSectors			= TRUE;
	}

	return pCollTriangle;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlWorldFieldManager : 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNtlWorldFieldManager::CNtlWorldFieldManager(RpWorld *pNtlWorld, RwV3d& SpawnPos, EActiveWorldType _ActiveWorldtype)
{
	m_pRpWorld			= NULL;

	m_pSkyEntity		= NULL;
	m_pDragonSkyEntity	= NULL;
	m_pFogEntity		= NULL;
	m_pWaterEntity		= NULL;
	m_pLightEntity		= NULL;
	m_pBloomEntity		= NULL;
	m_pPlantEntity		= NULL;
	m_CurMapNameIdx		= 0xffffffff;
	m_pPlanetHandler	= NULL;
	m_pWeatherHandler	= NULL;
	m_OldSectorIdx4PVS	= -1;
	m_ActiveWorldType	= _ActiveWorldtype;

	m_pFields			= NULL;

	m_IdxCurField		= -1;

	m_iCzTestCnt		= 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CNtlWorldFieldManager :
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNtlWorldFieldManager::~CNtlWorldFieldManager(void)
{
}

RwBool CNtlWorldFieldManager::CreateLight()
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreateLight");

	if(m_pLightEntity != NULL)
	{
		DestroyLight();
	}

	m_pLightEntity = reinterpret_cast<CNtlPLLight*>(GetSceneManager()->CreateEntity(PLENTITY_LIGHT, NTL_PLEN_LIGHT));
	DBO_ASSERT(m_pLightEntity, "Entity create failed.");

	if(!m_pLightEntity)
	{
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::CreateWater()
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreateWater");

	if(m_pWaterEntity != NULL)
	{
		DestroyWater();
	}

	m_pWaterEntity = reinterpret_cast<CNtlPLWater*>(GetSceneManager()->CreateEntity(PLENTITY_WATER, "NtlWorldWater"));
	DBO_ASSERT(m_pWaterEntity, "Entity create failed.");

	if(m_pWaterEntity == NULL)
	{
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::CreatePlant()
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreatePlant");

	if(m_pPlantEntity != NULL)
	{
		DestroyPlant();
	}

	m_pPlantEntity = reinterpret_cast<CNtlPLPlant*>(GetSceneManager()->CreateEntity(PLENTITY_PLANT, NTL_PLEN_PLANT));
	DBO_ASSERT(m_pPlantEntity, "Entity create failed.");

	if(m_pPlantEntity == NULL)
	{
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::CreateDragonSky()
{
	if(m_pDragonSkyEntity)
	{
		DestroyDragoneSky();
	}

	SPLEntityCreateParam	Param;
	RwInt32					SkyTypeIdx = eSTI_DRAGONE;

	Param._pUserData	= &SkyTypeIdx;

	m_pDragonSkyEntity	= reinterpret_cast<CNtlPLSky*>(GetSceneManager()->CreateEntity(PLENTITY_SKY, "null", &Param));
	DBO_ASSERT(m_pDragonSkyEntity, "Entity create failed.");

	if(!m_pDragonSkyEntity)
	{
		return FALSE;
	}

	return TRUE;
}

RwBool CNtlWorldFieldManager::CreateSky( void )
{
	if( m_pSkyEntity != NULL )
	{
		DestroySky();
	}

	// There're two types of sky and this depends on dGET_WORLD_PARAM's sky type index
	// 1. Sphere type only for multi fields including no prop. interpolation effect
	// 2. Hemisphere type including prop. interpolation effect

	SPLEntityCreateParam	Param;
	RwInt32					SkyTypeIdx;

	switch (dGET_WORLD_PARAM()->IdxSkyType)
	{
	case eSTI_HEMISPHERE:
		SkyTypeIdx = eSTI_HEMISPHERE;
		Param._pUserData = &SkyTypeIdx;
		break;
	case eSTI_SPHERE:
		SkyTypeIdx = eSTI_SPHERE;
		Param._pUserData = &SkyTypeIdx;
		break;
	}

	m_pSkyEntity = reinterpret_cast<CNtlPLSky*>(GetSceneManager()->CreateEntity(PLENTITY_SKY, "null", &Param));
	DBO_ASSERT(m_pSkyEntity, "Entity create failed.");

	if( m_pSkyEntity == NULL )
	{
		return FALSE;
	}

	return TRUE;
}

RwBool CNtlWorldFieldManager::CreateBloom( void )
{
	if(m_pBloomEntity)
	{
		DestroyBloom();
	}

	m_pBloomEntity = reinterpret_cast<CNtlPLBloom *>(GetSceneManager()->CreateEntity(PLENTITY_BLOOM, NTL_PLEN_BLOOM));
	DBO_ASSERT(m_pBloomEntity, "Entity create failed.");

	if (m_pBloomEntity == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

void CNtlWorldFieldManager::DestroyLight()
{
	if(m_pLightEntity)
	{
		GetSceneManager()->DeleteEntity(m_pLightEntity);
		m_pLightEntity = NULL;
	}
}

void CNtlWorldFieldManager::DestroyBloom()
{
	if(m_pBloomEntity)
	{
		GetSceneManager()->DeleteEntity(m_pBloomEntity);
		m_pBloomEntity = NULL;
	}
}

void CNtlWorldFieldManager::DestroySky()
{
	if( m_pSkyEntity != NULL )
	{
		GetSceneManager()->DeleteEntity( m_pSkyEntity );
		m_pSkyEntity = NULL;
	}

	return;
}

void CNtlWorldFieldManager::DestroyDragoneSky()
{
	if(m_pDragonSkyEntity)
	{
		GetSceneManager()->DeleteEntity(m_pDragonSkyEntity);
		m_pDragonSkyEntity = NULL;
	}
}

void CNtlWorldFieldManager::UpdateSky( RwV3d * pPosAvatar )
{
	if( m_pSkyEntity != NULL )
	{
		m_pSkyEntity->SetPosition( pPosAvatar );
	}

	return;
}

void CNtlWorldFieldManager::UpdateDragonSky(RwV3d * pPosAvatar)
{
	if(m_pDragonSkyEntity)
	{
		m_pDragonSkyEntity->SetPosition(pPosAvatar);
	}

	return;
}

RwBool CNtlWorldFieldManager::CreateFog( void )
{
	if( m_pFogEntity != NULL )
	{
		DestroyFog();
	}

	m_pFogEntity = reinterpret_cast<CNtlPLFog *>(GetSceneManager()->CreateEntity( PLENTITY_FOG, "null" ));
	DBO_ASSERT(m_pFogEntity, "Entity create failed.");

	if( m_pFogEntity == NULL )
	{
		return FALSE;
	}

	return TRUE;
}

void CNtlWorldFieldManager::DestroyWater()
{
	if(m_pWaterEntity != NULL)
	{
		GetSceneManager()->DeleteEntity(m_pWaterEntity);
		m_pWaterEntity = NULL;
	}
}

void CNtlWorldFieldManager::DestroyFog()
{
	if( m_pFogEntity != NULL )
	{
		GetSceneManager()->DeleteEntity( m_pFogEntity );
		m_pFogEntity = NULL;
	}
}

void CNtlWorldFieldManager::DestroyPlant()
{
	if (m_pPlantEntity != NULL)
	{
		GetSceneManager()->DeleteEntity(m_pPlantEntity);
		m_pPlantEntity = NULL;
	}
}

void CNtlWorldFieldManager::UpdateFog(RwV3d * pPosAvatar)
{
	if( m_pFogEntity != NULL )
	{
		m_pFogEntity->SetPosition( pPosAvatar );
	}

	return;
}

VOID CNtlWorldFieldManager::LoadPVS()
{
	BYTE	PVSFlag	= FALSE;
	BYTE	NeighborVisibility[dPVS_TOT_CELL_CNT *  dPVS_LAYER_CNT];

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);

	FILE *pFile = NULL;	
	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
	{
		static RwChar chPackPatch[NTL_MAX_DIR_PATH];
		strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "-------");

		SPackResFileData sPackFileData;
		RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(chPackPatch, sPackFileData);
		if(bPack)
		{
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			::fopen_s(&pFile, sPackFileData.strPackFileName.c_str(), "rb");
			if(pFile)
			{
				fseek(pFile, sPackFileData.uiOffset, SEEK_SET);
			}
		}
	}
	else
	{
		pFile = fopen("-------", "rb");
	}

	DBO_ASSERT(pFile, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");

	if (pFile)
	{
		int iError = 1;
		for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum; ++i)
		{
			iError = fread(&PVSFlag, sizeof(BYTE), 1, pFile);
			if(PVSFlag)
			{
				iError = fread(NeighborVisibility, sizeof(BYTE) * dPVS_TOT_CELL_CNT * dPVS_LAYER_CNT, 1, pFile);
				m_pSectors[i].m_pNtlWorldSectorPVS->SetVisibilityNeighbor(NeighborVisibility);
			}
		}

		fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
}

void CNtlWorldFieldManager::CreateRpWorld()
{
	m_pRpWorld = RpNtlWorldCreate();
	DBO_ASSERT(m_pRpWorld, "World create failed.");
	CNtlPLGlobal::SetActiveWorld(m_pRpWorld);
}

RpLight* NtlWorldRemoveLightWhenWorldDestroy(RpLight* light, void *data)
{	
	RpWorldRemoveLight((RpWorld*)data, light);

	return light;
}

RpAtomic* NtlWorldLogAtmoicWhenWorldDestroy(RpAtomic* atomic, void *data)
{
	// Destroy 시 Atomic이 물고 있는 Class 접근하여 이름을 알아온다.
	if (RpNtlAtomicGetData(atomic))
	{
		CNtlPLEntity* pPLEntity = static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(atomic));

		NtlLogFilePrintArgs("Atomics still exist : PTR(%u) TYPE(%u) NAME(%s)", (RwUInt32)pPLEntity, pPLEntity->GetClassType(), pPLEntity->GetName());
	}
	else
	{
		NtlLogFilePrint("Atomics still exist : RpNtlAtomicGetData(NULL)");
	}

	return atomic;
}

RpClump* NtlWorldRemoveClumpWhenWorldDestroy(RpClump* clump, void *data)
{	
	RpClumpForAllAtomics(clump, NtlWorldLogAtmoicWhenWorldDestroy, data);
	RpWorldRemoveClump((RpWorld *)data, clump);

	return clump;
}

void CNtlWorldFieldManager::DestroyRpWorld()
{
	if (m_pRpWorld)
	{
		if (!rwLinkListEmpty(&m_pRpWorld->directionalLightList) ||
			!rwLinkListEmpty(&m_pRpWorld->lightList))
		{
			NtlLogFilePrint("Lights still exist in the world when world was destroyed");
			RpWorldForAllLights(m_pRpWorld, NtlWorldRemoveLightWhenWorldDestroy, m_pRpWorld);
		}
		if (!rwLinkListEmpty(&m_pRpWorld->clumpList))
		{
			NtlLogFilePrint("Clumps still exist in the world when world was destroyed");
			RpWorldForAllClumps(m_pRpWorld, NtlWorldRemoveClumpWhenWorldDestroy, m_pRpWorld);
		}		
		RpWorldDestroy(m_pRpWorld);
	}
	CNtlPLGlobal::SetActiveWorld(NULL);
}

void CNtlWorldFieldManager::Init(RpWorld *pNtlWorld, RwV3d& SpawnPos)
{
	NTL_FUNCTION("CNtlWorldFieldManager::Init");

	CreateRpWorld();

	CNtlWorldFieldInfo::CreateInstance();

	InitSingleInstance();

	// 인도어
	CNtlWorldSectorManager::Init();

	RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// initialize m_pFields
	RwV3d SPos;

	m_pFields = NTL_NEW CNtlWorldField [dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum];

	for(int i = -dGET_WORLD_PARAM()->WorldSizeHalf; i < dGET_WORLD_PARAM()->WorldSizeHalf; i+= dGET_WORLD_PARAM()->WorldFieldSize)
	{
		for(int j = -dGET_WORLD_PARAM()->WorldSizeHalf; j < dGET_WORLD_PARAM()->WorldSizeHalf; j+= dGET_WORLD_PARAM()->WorldFieldSize)
		{
			SPos.x = static_cast<RwReal>(j);
			SPos.z = static_cast<RwReal>(i);
			RwInt32 Idx = GetFieldIdx(SPos);
			m_pFields[Idx].Init(SPos);
		}
	}

	// initialize common uv coordinates
	NtlCommonTexCoord = NTL_NEW RwTexCoords [dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum];
	for(int i = 0; i < dGET_WORLD_PARAM()->WorldSectorVertNum; ++i)
	{
		for(int j = 0; j < dGET_WORLD_PARAM()->WorldSectorVertNum; ++j)
		{
			NtlCommonTexCoord[j + i * dGET_WORLD_PARAM()->WorldSectorVertNum].u = (1.0f - j * dGET_WORLD_PARAM()->WorldSectorTileSize / (float)dGET_WORLD_PARAM()->WorldSectorSize);
			NtlCommonTexCoord[j + i * dGET_WORLD_PARAM()->WorldSectorVertNum].v = (1.0f - i * dGET_WORLD_PARAM()->WorldSectorTileSize / (float)dGET_WORLD_PARAM()->WorldSectorSize);
		}
	}

	// generate world sector pointer array
	CreateSectorMap(m_pRpWorld);

	// load PVS data
	LoadPVS();

	// initialize spawning position such as avatar, datum index, move flags
	RwInt32 CurFieldIdx = GetFieldIdx(SpawnPos);
	NTL_ASSERTE(CurFieldIdx != -1);

	m_OldDatumIdx = CurFieldIdx;
	m_NewDatumIdx = CurFieldIdx;

	m_eMoved2 = eC;
	::memset(&m_Fields6x6[0], -1, 36 * sizeof(RwInt32));
	::memset(&m_Fields6x6[1], -1, 36 * sizeof(RwInt32));

	// World field switching effects;
	m_FieldPropVariationRestTime = -999;
	m_FieldPropVariationStarting = FALSE;

	// Entities
	CreateSky();
	CreateDragonSky();
	GetSceneManager()->SetActiveSky(m_pSkyEntity);
	CreateFog();
	CreatePlant();
	CreateWater();
	CreateLight();
	CreateBloom();

	m_pPlanetHandler	= NTL_NEW CNtlPLPlanetHandler;
	m_pWeatherHandler	= NTL_NEW CNtlPLWeatherHandler;

	NTL_RETURNVOID();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Free :
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNtlWorldFieldManager::Free(void)
{
	NTL_FUNCTION("CNtlWorldFieldManager::Free");

	DBO_TRACE(m_iCzTestCnt == 0, "CNtlWorldFieldManager Free Cnt Test : " << m_iCzTestCnt);

	// 인도어
	CNtlWorldSectorManager::Free();

	DestroySky();
	DestroyDragoneSky();
	DestroyFog();
	DestroyPlant();
	DestroyWater();
	DestroyLight();
	DestroyBloom();

	NTL_ARRAY_DELETE(m_pFields);
	NTL_ARRAY_DELETE(NtlCommonTexCoord);

	g_vecVisibleSectors.clear();

	NTL_DELETE(m_pPlanetHandler);
	NTL_DELETE(m_pWeatherHandler);

	FreeSingleInstance();

	CNtlWorldFieldInfo::DestroyInstance();

	DestroyRpWorld();

	NTL_RETURNVOID();
}

void CNtlWorldFieldManager::InitSingleInstance()
{
	g_pNtlWorldBGMManager = NTL_NEW CNtlWorldBGMManager;

	CNtlSpecularManager::CreateInstance();

	CNtlSpecularManager::GetInstance()->Create();

	CNtlFieldColorManager::CreateInstance();
	CNtlFieldColorManager::GetInstance()->Create();

	CNtlOccluderManager::CreateInstance();

	CNtlOccluderManager::GetInstance()->Create();

	CNtlWorldRpLightManager::CreateInstance();

	CNtlWorldPathEngineManager::CreateInstance();

#ifdef dNTL_WORLD_TOOL_MODE
	FILE* pFile = NULL;
	//char Buffer[256];
	if(fopen_s(&pFile, ".\\tool\\saber\\script\\pathengine.txt", "r") != 0)
	{
		DBO_WARNING_MESSAGE("Load pathengine.txt");
		RwInt32 iCunkSize;
		fscanf(pFile, "%d", &iCunkSize);
		if (iCunkSize > 0)
		{
			CNtlWorldPathEngineManager::GetInstance()->SetChunkSize(iCunkSize);
			CNtlWorldPathEngineManager::GetInstance()->SetPathEngineMode(TRUE);
		}
		fclose(pFile);
	}

	CNtlWorldShadowManager::GetInst();

	CNtlWorldMergeManager::GetInstance()->SetFieldManager(this);
#endif dNTL_WORLD_TOOL_MODE	
}

void CNtlWorldFieldManager::FreeSingleInstance()
{
	CNtlWorldPathEngineManager::DestroyInstance();

	CNtlWorldRpLightManager::DestroyInstance();

	CNtlOccluderManager::GetInstance()->Destroy();
	CNtlOccluderManager::DestroyInstance();

	CNtlFieldColorManager::GetInstance()->Destroy();
	CNtlFieldColorManager::DestroyInstance();

	CNtlSpecularManager::GetInstance()->Destroy();
	CNtlSpecularManager::DestroyInstance();

#ifdef dNTL_WORLD_TOOL_MODE
	CNtlWorldShadowManager::GetInst()->Destroy();
#endif dNTL_WORLD_TOOL_MODE

	NTL_DELETE(g_pNtlWorldBGMManager);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateSectorMap :
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNtlWorldFieldManager::CreateSectorMap(RpWorld *pNtlWorld)
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreateSectorMap");


	RpSector *spSect;
	RpSector *spaStack[rpWORLDMAXBSPDEPTH];
	RwInt32 nStack = 0;

	spSect = pNtlWorld->rootSector;

	do
	{
		if (spSect->type < 0)
		{
			// hook the sector; it's atomic and leaf sector
			CNtlWorldSectorManager::SetWorldSector((RpWorldSector*)spSect);

			spSect = spaStack[nStack--];
		}
		else
		{
			RpPlaneSector *pspPlane = (RpPlaneSector *) spSect;

			// left then right
			spSect = pspPlane->leftSubTree;
			spaStack[++nStack] = pspPlane->rightSubTree;
		}
	}
	while (nStack >= 0);


	NTL_RETURNVOID();
}

RwBool CNtlWorldFieldManager::IsFieldValid(RwInt32 Idx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::IsFieldValid");


	// verify the sector index is being in current world
	if(Idx < 0 || Idx > (dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum - 1))
		NTL_RETURN(FALSE);


	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::IsFieldValid(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWorldFieldManager::IsFieldValid");


	if(Pos.x <= dGET_WORLD_PARAM()->WorldValueMax &&
		Pos.z <= dGET_WORLD_PARAM()->WorldValueMax &&
		Pos.x >= dGET_WORLD_PARAM()->WorldValueMin &&
		Pos.z >= dGET_WORLD_PARAM()->WorldValueMin)
		NTL_RETURN(TRUE);

	NTL_RETURN(FALSE);
}

sCUR_FIELD_TEX_INFO* CNtlWorldFieldManager::GetTexAttr()
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetTexAttr");


	if(m_IdxCurField == -1 || !(&m_pFields[m_IdxCurField]))
		NTL_RETURN(NULL);


	NTL_RETURN(&m_pFields[m_IdxCurField].GetTexAttr());
}

sCUR_FIELD_TEX_INFO* CNtlWorldFieldManager::GetTexAttr(RwInt32 IdxField)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetTexAttr");

	if(IdxField == -1 || !(&m_pFields[m_IdxCurField]))
		NTL_RETURN(NULL);

	m_IdxCurField = IdxField;


	NTL_RETURN(&m_pFields[IdxField].GetTexAttr());
}

RpAtomic* CNtlWorldFieldManager::GetAtomic(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetAtomic");

	RwInt32 IdxSector = CNtlWorldSectorManager::GetSectorIdx(Pos);

	if(IdxSector == -1)
	{
		NTL_RETURN(NULL);
	}
	else
	{
		sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(CNtlWorldSectorManager::m_pSectors[IdxSector].m_pWorldSector, pNtlSector);
		if(!pNtlSector)
		{
			NTL_RETURN(NULL);
		}

		NTL_RETURN(pNtlSector->pNtlWorldSector->m_pAtomic);
	}

	NTL_RETURN(NULL);
}

RwBool CNtlWorldFieldManager::GetWorldReady()
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetWorldReady");

	if(m_eMoved2 != eC)
		NTL_RETURN(TRUE);

	NTL_RETURN(FALSE);
}

RwInt32 CNtlWorldFieldManager::GetFieldIdx(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetFieldIdx");

	if(!IsFieldValid(Pos))
		NTL_RETURN(-1);

	RwInt32 RetIdx = CNtlMath::GetSafeIdx3D(Pos, dGET_WORLD_PARAM()->WorldSizeHalf, dGET_WORLD_PARAM()->WorldFieldSize, dGET_WORLD_PARAM()->WorldFieldNum);

// 	RwInt32 WidthNum, DepthNum;
// 	RwReal XBasedOn0 = (RwReal)((RwInt32)Pos.x + dGET_WORLD_PARAM()->WorldSizeHalf);
// 	RwReal ZBasedOn0 = (RwReal)((RwInt32)Pos.z + dGET_WORLD_PARAM()->WorldSizeHalf);
// 	WidthNum = (RwInt32)(XBasedOn0 / (RwReal)dGET_WORLD_PARAM()->WorldFieldSize);
// 	DepthNum = (RwInt32)(ZBasedOn0 / (RwReal)dGET_WORLD_PARAM()->WorldFieldSize);
// 
// 	RwInt32 RetIdx = WidthNum + (DepthNum * dGET_WORLD_PARAM()->WorldFieldNum);

	if(!IsFieldValid(RetIdx))
		NTL_RETURN(-1);

	NTL_RETURN(RetIdx);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UpdateChangeDir : 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNtlWorldFieldManager::UpdateDatumDir()
{
	// if eC then it's the first time for making terrain
	if(m_eMoved2 == eC || m_eMoved2 == ePORTAL)
	{
		return;
	}

	if(m_NewDatumIdx == m_OldDatumIdx + dGET_WORLD_PARAM()->WorldFieldNum)		
	{
		m_eMoved2 = eN;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx + dGET_WORLD_PARAM()->WorldFieldNum - 1)
	{
		m_eMoved2 = eNE;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx - 1)					
	{
		m_eMoved2 = eE;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx - dGET_WORLD_PARAM()->WorldFieldNum - 1)
	{
		m_eMoved2 = eES;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx - dGET_WORLD_PARAM()->WorldFieldNum)
	{
		m_eMoved2 = eS;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx - dGET_WORLD_PARAM()->WorldFieldNum + 1)	
	{
		m_eMoved2 = eSW;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx + 1)
	{
		m_eMoved2 = eW;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx + dGET_WORLD_PARAM()->WorldFieldNum + 1)
	{
		m_eMoved2 = eWN;
	}
	else
	{
		// 개발 시 Brack를 걸고 서버이동(버스등)을 하게 되면 m_eMove2가 변경 되지
		// 않는 경우가 발생 한다. 이 경우 ePORTAL 처리를 강행한다.
		// exception; Sync. wasn't matched
		m_eMoved2 = ePORTAL;
		DBO_TRACE(FALSE, "Critical errors occured");
#ifdef dNTL_WORLD_TOOL_MODE
		::MessageBox(NULL, "Critical errors occured", "SAVER", MB_OK);
#endif
	}
}

// find some loading parts out considering 4 x 4 area
RwBool CNtlWorldFieldManager::IsThereNewRegion2Load()
{
	NTL_FUNCTION("CNtlWorldFieldManager::IsThereNewRegion2Load");

	// -------------------------------
	// | 35 | 34 | 33 | 32 | 31 | 30 |
	// -------------------------------
	// | 29 | 28 | 27 | 26 | 25 | 24 |
	// -------------------------------
	// | 23 | 22 | 21 | 20 | 19 | 18 |
	// -------------------------------
	// | 17 | 16 | ^^ | 14 | 13 | 12 |
	// -------------------------------
	// | 11 | 10 | 09 | 08 | 07 | 06 |
	// -------------------------------
	// | 05 | 04 | 03 | 02 | 01 | 00 |
	// -------------------------------

	RwInt32 Cur6x6Idx = -1;
	for(RwInt32 i = 0; i < 36; ++i)
	{
		if(m_Fields6x6[0][i] == m_OldDatumIdx)
		{
			Cur6x6Idx = i;
			break;
		}
	}

	switch(m_eMoved2)
	{
	case eN:
		{
			if(Cur6x6Idx == 15 || Cur6x6Idx == 14)
				NTL_RETURN(FALSE);
		}
		break;
	case eNE:
		{
			if(Cur6x6Idx == 15)
				NTL_RETURN(FALSE);
		}
		break;
	case eE:
		{
			if(Cur6x6Idx == 15 || Cur6x6Idx == 21)
				NTL_RETURN(FALSE);
		}
		break;
	case eES:
		{
			if(Cur6x6Idx == 21)
				NTL_RETURN(FALSE);
		}
		break;
	case eS:
		{
			if(Cur6x6Idx == 21 || Cur6x6Idx == 20)
				NTL_RETURN(FALSE);
		}
		break;
	case eSW:
		{
			if(Cur6x6Idx == 20)
				NTL_RETURN(FALSE);
		}
		break;
	case eW:
		{
			if(Cur6x6Idx == 14 || Cur6x6Idx == 20)
				NTL_RETURN(FALSE);
		}
		break;
	case eWN:
		{
			if(Cur6x6Idx == 14)
				NTL_RETURN(FALSE);
		}
		break;
	}

	NTL_RETURN(TRUE);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UpdateNeighborFields :
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNtlWorldFieldManager::UpdateNeighborFields(RwV3d& Pos)
{
	if(m_eMoved2 != eC && m_eMoved2 != ePORTAL)
		if(!IsThereNewRegion2Load())
			return;

	// -------------------------------
	// | 35 | 34 | 33 | 32 | 31 | 30 |
	// -------------------------------
	// | 29 | 28 | 27 | 26 | 25 | 24 |
	// -------------------------------
	// | 23 | 22 | 21 | 20 | 19 | 18 |
	// -------------------------------
	// | 17 | 16 | ^^ | 14 | 13 | 12 |
	// -------------------------------
	// | 11 | 10 | 09 | 08 | 07 | 06 |
	// -------------------------------
	// | 05 | 04 | 03 | 02 | 01 | 00 |
	// -------------------------------

	RwInt32 CheckIdx;
	RwV3d NeighborPt;
	RwInt32 i, j;

	switch(m_eMoved2)
	{
	case ePORTAL:
		{

// #ifdef dNTL_WORLD_SCHEDULE_LOADING
// 			while (m_WorldScheduler.Scheduling(1.0f, m_WorldScheduler.GetLastPos()))
// 			{
// 				++m_iCzTestCnt;
// 			}
// #endif

			for(i = 0; i < 36; ++i)
				if(m_Fields6x6[0][i] != -1)
					DeleteFields(m_Fields6x6[0][i]);

#ifdef dNTL_WORLD_SCHEDULE_LOADING
			while (m_WorldScheduler.Scheduling(1.0f, m_WorldScheduler.GetLastPos()))
			{
				++m_iCzTestCnt;
			}
#endif
		}

		// do eC straight away; no breaks here

	case eC:
		{
			/*
			dGET_WORLD_PARAM()->Loading = TRUE;

			NeighborPt.x =	Pos.x;
			NeighborPt.z =	Pos.z + dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
			return;
			else
			{
			RwV3d NeighborIdx4PosFirst;
			RwV3d NeighborIdx4Pos;
			RwInt32 NeighborIdx;

			NeighborIdx4PosFirst.x = Pos.x - dGET_WORLD_PARAM()->WorldFieldSize * 3;
			NeighborIdx4PosFirst.z = Pos.z - dGET_WORLD_PARAM()->WorldFieldSize * 2;

			for(j = 0; j < 6; ++j)
			for(i = 0; i < 6; ++i)
			{
			NeighborIdx4Pos.x = NeighborIdx4PosFirst.x + i * dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborIdx4Pos.z = NeighborIdx4PosFirst.z + j * dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborIdx = GetFieldIdx(NeighborIdx4Pos);
			if(-1 == NeighborIdx)
			m_Fields6x6[1][j * 6 + i] = -1;
			else
			{
			m_Fields6x6[1][j * 6 + i] = NeighborIdx;
			CreateFields(m_Fields6x6[1][j * 6 + i]);
			}
			}

			m_eMoved2 = eIDLE;
			}

			dGET_WORLD_PARAM()->Loading = FALSE;
			*/

			dGET_WORLD_PARAM()->Loading = TRUE;

			NeighborPt.x =	Pos.x;
			NeighborPt.z =	Pos.z;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			else
			{
				RwV3d NeighborIdx4PosFirst;
				RwV3d NeighborIdx4Pos;
				RwInt32 NeighborIdx;

				NeighborIdx4PosFirst.x = Pos.x - dGET_WORLD_PARAM()->WorldFieldSize * 3;
				NeighborIdx4PosFirst.z = Pos.z - dGET_WORLD_PARAM()->WorldFieldSize * 2;

				for (j = 0; j < 6; ++j)
				{
					for (i = 0; i < 6; ++i)
					{
						NeighborIdx4Pos.x = NeighborIdx4PosFirst.x + i * dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborIdx4Pos.z = NeighborIdx4PosFirst.z + j * dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborIdx = GetFieldIdx(NeighborIdx4Pos);
						if (-1 == NeighborIdx)
							m_Fields6x6[1][j * 6 + i] = -1;
						else
						{
							m_Fields6x6[1][j * 6 + i] = NeighborIdx;
							CreateFields(m_Fields6x6[1][j * 6 + i]);
						}
					}
				}

				m_eMoved2 = eIDLE;
			}

			dGET_WORLD_PARAM()->Loading = FALSE;
		}
		break;

	case eN:
		{
			NeighborPt.x =	Pos.x;
			NeighborPt.z =	Pos.z + dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			else
			{
				// check same ids up
				for(i = 6; i < 36; ++i)
					m_Fields6x6[1][i - 6] = m_Fields6x6[0][i];

				// check new ids up
				for(i = 0; i < 6; ++i)
				{
					if(m_Fields6x6[0][30 + i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][30 + i]].GetSPos().x;
						NeighborPt.z = m_pFields[m_Fields6x6[0][30 + i]].GetSPos().z + dGET_WORLD_PARAM()->WorldFieldSize;
						m_Fields6x6[1][30 + i] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][30 + i] = -1;

					if(m_Fields6x6[1][30 + i] != -1)
						CreateFields(m_Fields6x6[1][30 + i]);
				}

				// check old ids up
				for(i = 0; i < 6; ++i)
					if(m_Fields6x6[0][i] != -1)
						DeleteFields(m_Fields6x6[0][i]);
			}
		}
		break;

	case eNE:
		{
			NeighborPt.x =	Pos.x - dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborPt.z =	Pos.z + dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			else
			{
				// check same ids up
				RwInt32 IndicesFrom[25] = {6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34};
				RwInt32 Indices2[25] = {1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29};
				for(i = 0; i < 25; ++i)
					m_Fields6x6[1][Indices2[i]] = m_Fields6x6[0][IndicesFrom[i]];

				// check new ids up
				for(i = 30; i <= 34; ++i)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z + dGET_WORLD_PARAM()->WorldFieldSize;
						m_Fields6x6[1][i + 1] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i + 1] = -1;

					if(m_Fields6x6[1][i + 1] != -1)
						CreateFields(m_Fields6x6[1][i + 1]);
				}

				for(i = 6; i <= 30; i+= 6)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x - dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z;
						m_Fields6x6[1][i - 6] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i - 6] = -1;

					if(m_Fields6x6[1][i - 6] != -1)
						CreateFields(m_Fields6x6[1][i - 6]);
				}

				NeighborPt.x = m_pFields[m_Fields6x6[0][30]].GetSPos().x - dGET_WORLD_PARAM()->WorldFieldSize;
				NeighborPt.z = m_pFields[m_Fields6x6[0][30]].GetSPos().z + dGET_WORLD_PARAM()->WorldFieldSize;
				m_Fields6x6[1][30] = GetFieldIdx(NeighborPt);
				if(m_Fields6x6[1][30] != -1)
					CreateFields(m_Fields6x6[1][30]);

				// check old ids up
				RwInt32 Indices2Del[11] = {0, 1, 2, 3, 4, 5, 11, 17, 23, 29, 35};
				for(i = 0; i < 11; ++i)
					if(m_Fields6x6[0][Indices2Del[i]] != -1)
						DeleteFields(m_Fields6x6[0][Indices2Del[i]]);
			}
		}
		break;

	case eE:
		{
			NeighborPt.x =	Pos.x - dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborPt.z =	Pos.z;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			else
			{
				// check same ids up
				RwInt32 IndicesFrom[30] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34};
				RwInt32 Indices2[30] = {1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 31, 32, 33, 34, 35};
				for(i = 0; i < 30; ++i)
					m_Fields6x6[1][Indices2[i]] = m_Fields6x6[0][IndicesFrom[i]];

				// check new ids up
				for(i = 0; i <= 30; i += 6)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x - dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z;
						m_Fields6x6[1][i] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i] = -1;

					if(m_Fields6x6[1][i] != -1)
						CreateFields(m_Fields6x6[1][i]);
				}

				// check old ids up
				for(i = 5; i <= 35; i += 6)
					if(m_Fields6x6[0][i] != -1)
						DeleteFields(m_Fields6x6[0][i]);
			}
		}
		break;

	case eES:
		{
			NeighborPt.x =	Pos.x - dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborPt.z =	Pos.z - dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			else
			{
				// check same ids up
				RwInt32 IndicesFrom[25] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28};
				RwInt32 Indices2[25] = {7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 31, 32, 33, 34, 35};
				for(i = 0; i < 25; ++i)
					m_Fields6x6[1][Indices2[i]] = m_Fields6x6[0][IndicesFrom[i]];

				// check new ids up
				for(i = 0; i <= 24; i += 6)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x - dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z;
						m_Fields6x6[1][i + 6] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i + 6] = -1;

					if(m_Fields6x6[1][i + 6] != -1)
						CreateFields(m_Fields6x6[1][i + 6]);
				}

				for(i = 0; i <= 4; ++i)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z - dGET_WORLD_PARAM()->WorldFieldSize;
						m_Fields6x6[1][i + 1] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i + 1] = -1;

					if(m_Fields6x6[1][i + 1] != -1)
						CreateFields(m_Fields6x6[1][i + 1]);
				}

				NeighborPt.x = m_pFields[m_Fields6x6[0][0]].GetSPos().x - dGET_WORLD_PARAM()->WorldFieldSize;
				NeighborPt.z = m_pFields[m_Fields6x6[0][0]].GetSPos().z - dGET_WORLD_PARAM()->WorldFieldSize;
				m_Fields6x6[1][0] = GetFieldIdx(NeighborPt);
				if(m_Fields6x6[1][0] != -1)
					CreateFields(m_Fields6x6[1][0]);

				// check old ids up
				RwInt32 Indices2Del[11] = {5, 11, 17, 23, 29, 35, 34, 33, 32, 31, 30};
				for(i = 0; i < 11; ++i)
					if(m_Fields6x6[0][Indices2Del[i]] != -1)
						DeleteFields(m_Fields6x6[0][Indices2Del[i]]);
			}
		}
		break;

	case eS:
		{
			NeighborPt.x =	Pos.x;
			NeighborPt.z =	Pos.z - dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			{
				// check same ids up
				for(i = 0; i <= 29; ++i)
					m_Fields6x6[1][i + 6] = m_Fields6x6[0][i];

				// check new ids up
				for(i = 0; i < 6; ++i)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z - dGET_WORLD_PARAM()->WorldFieldSize;
						m_Fields6x6[1][i] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i] = -1;

					if(m_Fields6x6[1][i] != -1)
						CreateFields(m_Fields6x6[1][i]);
				}

				// check old ids up
				for(i = 30; i <= 35; ++i)
					if(m_Fields6x6[0][i] != -1)
						DeleteFields(m_Fields6x6[0][i]);
			}
		}
		break;

	case eSW:
		{
			NeighborPt.x =	Pos.x + dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborPt.z =	Pos.z - dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			{
				// check same ids up
				RwInt32 IndicesFrom[25] = {1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29};
				RwInt32 Indices2[25] = {6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34};
				for(i = 0; i < 25; ++i)
					m_Fields6x6[1][Indices2[i]] = m_Fields6x6[0][IndicesFrom[i]];

				// check new ids up
				for(i = 1; i <= 5; ++i)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z - dGET_WORLD_PARAM()->WorldFieldSize;
						m_Fields6x6[1][i - 1] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i - 1] = -1;

					if(m_Fields6x6[1][i - 1] != -1)
						CreateFields(m_Fields6x6[1][i - 1]);
				}

				for(i = 5; i <= 29; i += 6)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x + dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z;
						m_Fields6x6[1][i + 6] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i + 6] = -1;

					if(m_Fields6x6[1][i + 6] != -1)
						CreateFields(m_Fields6x6[1][i + 6]);
				}

				NeighborPt.x = m_pFields[m_Fields6x6[0][5]].GetSPos().x + dGET_WORLD_PARAM()->WorldFieldSize;
				NeighborPt.z = m_pFields[m_Fields6x6[0][5]].GetSPos().z - dGET_WORLD_PARAM()->WorldFieldSize;
				m_Fields6x6[1][5] = GetFieldIdx(NeighborPt);
				if(m_Fields6x6[1][5] != -1)
					CreateFields(m_Fields6x6[1][5]);

				// check old ids up
				RwInt32 Indices2Del[11] = {0, 6, 12, 18, 24, 30, 31, 32, 33, 34, 35};
				for(i = 0; i < 11; ++i)
					if(m_Fields6x6[0][Indices2Del[i]] != -1)
						DeleteFields(m_Fields6x6[0][Indices2Del[i]]);
			}
		}
		break;

	case eW:
		{
			NeighborPt.x =	Pos.x + dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborPt.z =	Pos.z;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			{
				// check same ids up
				RwInt32 IndicesFrom[30] = {1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 31, 32, 33, 34, 35};
				RwInt32 Indices2[30] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34};
				for(i = 0; i < 30; ++i)
					m_Fields6x6[1][Indices2[i]] = m_Fields6x6[0][IndicesFrom[i]];

				// check new ids up
				for(i = 5; i <= 35; i += 6)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x + dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z;
						m_Fields6x6[1][i] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i] = -1;

					if(m_Fields6x6[1][i] != -1)
						CreateFields(m_Fields6x6[1][i]);
				}


				// check old ids up
				for(i = 0; i <= 30; i += 6)
					if(m_Fields6x6[0][i] != -1)
						DeleteFields(m_Fields6x6[0][i]);
			}
		}
		break;

	case eWN:
		{
			NeighborPt.x =	Pos.x + dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborPt.z =	Pos.z + dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			{
				// check same ids up
				RwInt32 IndicesFrom[25] = {7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 31, 32, 33, 34, 35};
				RwInt32 Indices2[25] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 12, 13, 14, 15 ,16 ,18 ,19 ,20 ,21 ,22, 24, 25, 26, 27, 28};
				for(i = 0; i < 25; ++i)
					m_Fields6x6[1][Indices2[i]] = m_Fields6x6[0][IndicesFrom[i]];

				// check new ids up
				for(i = 11; i <= 35; i += 6)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x + dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z;
						m_Fields6x6[1][i - 6] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i - 6] = -1;

					if(m_Fields6x6[1][i - 6] != -1)
						CreateFields(m_Fields6x6[1][i - 6]);
				}

				for(i = 31; i <= 35; ++i)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z + dGET_WORLD_PARAM()->WorldFieldSize;
						m_Fields6x6[1][i - 1] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i - 1] = -1;

					if(m_Fields6x6[1][i - 1] != -1)
						CreateFields(m_Fields6x6[1][i - 1]);
				}

				NeighborPt.x = m_pFields[m_Fields6x6[0][35]].GetSPos().x + dGET_WORLD_PARAM()->WorldFieldSize;
				NeighborPt.z = m_pFields[m_Fields6x6[0][35]].GetSPos().z + dGET_WORLD_PARAM()->WorldFieldSize;
				m_Fields6x6[1][35] = GetFieldIdx(NeighborPt);
				if(m_Fields6x6[1][35] != -1)
					CreateFields(m_Fields6x6[1][35]);

				// check old ids up
				RwInt32 Indices2Del[11] = {0, 1, 2, 3, 4, 5, 6, 12, 18, 24, 30};
				for(i = 0; i < 11; ++i)
					if(m_Fields6x6[0][Indices2Del[i]] != -1)
						DeleteFields(m_Fields6x6[0][Indices2Del[i]]);
			}
		}
		break;
	}

	CopyMemory(&m_Fields6x6[0], &m_Fields6x6[1], 36 * sizeof(RwInt32));
}

void CNtlWorldFieldManager::UpdateLODAttr(RwV3d& Pos)
{
	if(!dGET_WORLD_PARAM()->LODEnable)
		return;

	int l, m;
	int MyLvl, NeighborLvl;
	int WidthSectorNum;
	int DepthSectorNum;
	int CurSectorIdx;

	UpdateLODAttrLvl(Pos);

	for(int i = 0; i < 36; ++i)
	{
		if(m_Fields6x6[1][i] == -1)
			continue;

		RwV3d SPos = m_pFields[m_Fields6x6[1][i]].GetSPos();
		RwV3d SectorSPos;

		for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x = (RwReal)m;
				SectorSPos.z = (RwReal)l;
				CurSectorIdx = GetSectorIdx(SectorSPos);
				if(CurSectorIdx == -1)
					continue;

				MyLvl = m_pSectors[CurSectorIdx].m_LODAttr.MipMapLevel;
				WidthSectorNum = CurSectorIdx / dGET_WORLD_PARAM()->WorldSectorNum;
				DepthSectorNum = CurSectorIdx % dGET_WORLD_PARAM()->WorldSectorNum;

				if(DepthSectorNum + 1 < dGET_WORLD_PARAM()->WorldSectorNum)
				{
					if(IsSectorValid(CurSectorIdx + dGET_WORLD_PARAM()->WorldSectorNum))
					{
						NeighborLvl = m_pSectors[CurSectorIdx + dGET_WORLD_PARAM()->WorldSectorNum].m_LODAttr.MipMapLevel;
						m_pSectors[CurSectorIdx].m_LODAttr.TSectionLevel = NeighborLvl;
					}
				}
				if(WidthSectorNum - 1 >= 0)
				{
					if(IsSectorValid(CurSectorIdx + 1))
					{
						NeighborLvl = m_pSectors[CurSectorIdx + 1].m_LODAttr.MipMapLevel;
						m_pSectors[CurSectorIdx].m_LODAttr.LSectionLevel = NeighborLvl;
					}
				}
				if(WidthSectorNum + 1 < dGET_WORLD_PARAM()->WorldSectorNum)
				{
					if(IsSectorValid(CurSectorIdx - 1))
					{
						NeighborLvl = m_pSectors[CurSectorIdx - 1].m_LODAttr.MipMapLevel;
						m_pSectors[CurSectorIdx].m_LODAttr.RSectionLevel = NeighborLvl;
					}
				}
				if(DepthSectorNum - 1 >= 0)
				{
					if(IsSectorValid(CurSectorIdx - dGET_WORLD_PARAM()->WorldSectorNum))
					{
						NeighborLvl = m_pSectors[CurSectorIdx - dGET_WORLD_PARAM()->WorldSectorNum].m_LODAttr.MipMapLevel;
						m_pSectors[CurSectorIdx].m_LODAttr.BSectionLevel = NeighborLvl;
					}
				}
			}
		}
	}
}

void CNtlWorldFieldManager::UpdateLODAttrLvl(RwV3d& Pos)
{
	int l, m;
	int CurSectorIdx;
	float CurDistance;
	D3DXVECTOR3 vDatumSPos = D3DXVECTOR3(Pos.x, 0.0f, Pos.z);


	for(int i = 0; i < 36; ++i)
	{
		if(m_Fields6x6[1][i] == -1)
			continue;

		RwV3d SPos = m_pFields[m_Fields6x6[1][i]].GetSPos();
		RwV3d SectorSPos;

		for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x = (RwReal)m;
				SectorSPos.z = (RwReal)l;
				CurSectorIdx = GetSectorIdx(SectorSPos);

				if(CurSectorIdx == -1)
					continue;

				CurDistance = D3DXVec3Length(&(vDatumSPos - m_pSectors[CurSectorIdx].DatumPoint)) * 0.9f;
				float LengthPerLevel = dGET_WORLD_PARAM()->WorldFarPlane / dGET_SECTOR_LOD()->GetTotLvlCnt();
				m_pSectors[CurSectorIdx].m_LODAttr.MipMapLevel = dGET_SECTOR_LOD()->GetTotLvlCnt() - (int)floor(CurDistance / LengthPerLevel);
				CLAMP(m_pSectors[CurSectorIdx].m_LODAttr.MipMapLevel, 0, dGET_SECTOR_LOD()->GetTotLvlCnt());
			}
		}
	}
}

RpCollisionTriangle* WorldIntersectionAtomicTriCB(RpIntersection *pIntersection, RpCollisionTriangle *pCollTriangle, RwReal distance, void *data)
{
	sNtlMapObjCollisionParam *pCollisionParam = static_cast<sNtlMapObjCollisionParam*>(data);

	if(!pCollisionParam->pCurAtomic || pCollisionParam->DistFromCam > distance)
	{
		pCollisionParam->DistFromCam		= distance;
		pCollisionParam->IsCollided			= TRUE;
		pCollisionParam->PtIntersection.x	= pIntersection->t.line.start.x + distance * (pIntersection->t.line.end.x - pIntersection->t.line.start.x);
		pCollisionParam->PtIntersection.y	= pIntersection->t.line.start.y + distance * (pIntersection->t.line.end.y - pIntersection->t.line.start.y);
		pCollisionParam->PtIntersection.z	= pIntersection->t.line.start.z + distance * (pIntersection->t.line.end.z - pIntersection->t.line.start.z);
	}

	return pCollTriangle;
}

RpAtomic* WorldIntersectionAtomicCB(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData)
{
	if((static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic))->GetClassType() != PLENTITY_OBJECT))
	{
		return pAtomic;
	}

	sNtlMapObjCollisionParam*	pCollisionParam = static_cast<sNtlMapObjCollisionParam*>(pData);
	CNtlPLObject*				pNtlPLObject	= static_cast<CNtlPLObject*>(RpNtlAtomicGetData(pAtomic));

	if(dGET_BRUSH_ENTITY() != pNtlPLObject)
	{
#ifdef dNTL_WORLD_TOOL_MODE
		if (((pNtlPLObject->GetFlags() & NTL_PLEFLAG_COLLISION) && (RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE)) && !CNtlPLGlobal::m_bCollObjVisible)
		{
			return pAtomic;
		}
#endif 
		sNtlMapObjCollisionParam CollisionParamInTri;
		::memset(&CollisionParamInTri, 0, sizeof(sNtlMapObjCollisionParam));

		RpAtomicForAllIntersections(pAtomic, pIntersection, WorldIntersectionAtomicTriCB, &CollisionParamInTri);

		if(CollisionParamInTri.IsCollided)
		{
			if(!pCollisionParam->pCurAtomic || pCollisionParam->DistFromCam > CollisionParamInTri.DistFromCam)
			{
				pCollisionParam->pCurAtomic		= pAtomic;
				pCollisionParam->DistFromCam	= CollisionParamInTri.DistFromCam;
				pCollisionParam->IsCollided		= TRUE;
				::memcpy(&pCollisionParam->PtIntersection, &CollisionParamInTri.PtIntersection, sizeof(RwV3d));
			}
		}
	}

	return pAtomic;
}

RpCollisionTriangle* GetNtlWorldOutdoorCollisionInfoMiniIndoor(RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void *pData)
{
	sNtlWorldCollisionInfo* pCollInfo	= static_cast<sNtlWorldCollisionInfo*>(pData);
	RwLine*					pLine		= &pIntersection->t.line;
	RwV3d					vDelta		= pIntersection->t.line.end - pIntersection->t.line.start;
	RwReal					fDist		= RwV3dLength(&vDelta) * fRatio;

	if (pCollInfo->fDist - fDist > 0.00001f)
	{
		RwV3d vCollPos;
		vCollPos.x = pLine->start.x + (fRatio * vDelta.x);
		vCollPos.y = pLine->start.y + (fRatio * vDelta.y);
		vCollPos.z = pLine->start.z + (fRatio * vDelta.z);

		pCollInfo->fDist						= fDist;
		pCollInfo->RayIntersectionPt4Terrain	= vCollPos;
		pCollInfo->IsCollidedAtSectors			= TRUE;
	}
	return pRpCollisionTriangle;
}

RpAtomic* GetNtlWorldOutdoorCollisionInfo(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fRatio, void* pData)
{
	if((static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic))->GetClassType() != PLENTITY_OBJECT))
	{
		return pAtomic;
	}

	sNtlMapObjCollisionParam*	pCollisionParam = static_cast<sNtlMapObjCollisionParam*>(pData);
	CNtlPLObject*				pNtlPLObject	= static_cast<CNtlPLObject*>(RpNtlAtomicGetData(pAtomic));

	if (pNtlPLObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		pNtlPLObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		RpAtomicForAllIntersections(pAtomic, pIntersection, GetNtlWorldOutdoorCollisionInfoMiniIndoor, pData);
	}

	return pAtomic;
}

RpCollisionTriangle* GetNtlWorldOutdoorCollisionInfo(RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void *pData)
{
	sNtlWorldCollisionInfo* pCollInfo	= static_cast<sNtlWorldCollisionInfo*>(pData);
	RwLine*					pLine		= &pIntersection->t.line;
	RwV3d					vDelta		= pIntersection->t.line.end - pIntersection->t.line.start;
	RwReal					fDist		= RwV3dLength(&vDelta) * fRatio;

	if (pCollInfo->fDist - fDist > 0.00001f)
	{
		RwV3d vCollPos;
		vCollPos.x = pLine->start.x + (fRatio * vDelta.x);
		vCollPos.y = pLine->start.y + (fRatio * vDelta.y);
		vCollPos.z = pLine->start.z + (fRatio * vDelta.z);

		// 툴에서의 Mouse Pick Data는 TransparencyTile도 충돌해야 한다.
		if (!dGET_COLLISION_INFO_UPDATE() && GetSceneManager()->GetWorldAttribute(vCollPos) & dNMAP_TRANSPARENCY_TILE_FLAG)
		{
			return pRpCollisionTriangle;
		}

		pCollInfo->fDist						= fDist;
		pCollInfo->RayIntersectionPt4Terrain	= vCollPos;
		pCollInfo->IsCollidedAtSectors			= TRUE;
	}
	return pRpCollisionTriangle;
}

RpWorldSector* GetNtlWorldOutdoorCollisionInfo(RpIntersection * pIntersection, RpWorldSector * pRpWorldSector, void *pData)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);
	if (pNtlSector && pNtlSector->pNtlWorldSector)
	{
		CNtlWorldSector*	pNtlWorldSector = pNtlSector->pNtlWorldSector;
		RpAtomic*			pAtomic			= pNtlWorldSector->m_pAtomic;

		if (pAtomic)
		{
			if (pIntersection->type == rpINTERSECTLINE && !NtlTestLineSphere(&pIntersection->t.line.start, &pIntersection->t.line.end, &pAtomic->worldBoundingSphere))
			{
				return pRpWorldSector;
			}
			RpAtomicForAllIntersections(pAtomic, pIntersection, GetNtlWorldOutdoorCollisionInfo, pData);
		}
	}

	return pRpWorldSector;	
}

RpCollisionTriangle* GetNtlWorldOutdoorCollisionInfoWater( RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void *pData)
{
	sNtlWorldCollisionInfo* pCollInfo	= static_cast<sNtlWorldCollisionInfo*>(pData);
	RwLine*					pLine		= &pIntersection->t.line;
	RwV3d					vDelta		= pIntersection->t.line.end - pIntersection->t.line.start;
	RwReal					fDist		= RwV3dLength(&vDelta) * fRatio;

	if (pCollInfo->fDist - fDist > 0.00001f)
	{
		pCollInfo->fDist						= fDist;
		pCollInfo->RayIntersectionPt4Terrain.x	= pLine->start.x + (fRatio * vDelta.x);
		pCollInfo->RayIntersectionPt4Terrain.y	= pLine->start.y + (fRatio * vDelta.y);
		pCollInfo->RayIntersectionPt4Terrain.z	= pLine->start.z + (fRatio * vDelta.z);
		pCollInfo->IsCollidedAtSectors			= TRUE;
	}
	return pRpCollisionTriangle;
}

RpWorldSector* GetNtlWorldOutdoorCollisionInfoWater(RpIntersection * pIntersection, RpWorldSector * pRpWorldSector, void *pData)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);
	if (pNtlSector && pNtlSector->pNtlWorldSector)
	{
		CNtlWorldSector*	pNtlWorldSector = pNtlSector->pNtlWorldSector;
		RpAtomic*			pAtomic			= pNtlWorldSector->m_pWater ? pNtlWorldSector->m_pWater->_pAtom : NULL;

		if (pAtomic)
		{
			if (pIntersection->type == rpINTERSECTLINE && !NtlTestLineSphere(&pIntersection->t.line.start, &pIntersection->t.line.end, &pAtomic->worldBoundingSphere))
			{
				return pRpWorldSector;
			}
			RpAtomicForAllIntersections(pAtomic, pIntersection, GetNtlWorldOutdoorCollisionInfoWater, pData);
		}
	}

	return pRpWorldSector;
}

RwBool CNtlWorldFieldManager::PickTerrain(RwInt32 ScreenPosX, RwInt32 ScreenPosY, RwV3d& IntersectionPt)
{
	RwV2d ScreenPos;
	ScreenPos.x = (RwReal)ScreenPosX;
	ScreenPos.y = (RwReal)ScreenPosY;

	RwLine cameraRay;
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

	return PickTerrain(cameraRay.start, cameraRay.end, IntersectionPt);
}

RwBool CNtlWorldFieldManager::PickTerrain(RwV3d& StartPos, RwV3d& EndPos, RwV3d& IntersectionPt)
{
	if(m_eMoved2 == eC)
	{
		return FALSE;
	}

	RpIntersection rpIntersection;
	rpIntersection.type			= rpINTERSECTLINE;
	rpIntersection.t.line.start	= StartPos;
	rpIntersection.t.line.end	= EndPos;

	RwV3d	vDir;
	RwReal	fDist;
	RwV3dSubMacro(&vDir, &EndPos, &StartPos);

	fDist = RwV3dLength(&vDir);

	RwV3dNormalize(&vDir, &vDir);

	sNtlWorldCollisionInfo NtlWorldCollisionInfo;
	NtlWorldCollisionInfo.IsCollidedAtSectors		= FALSE;
	NtlWorldCollisionInfo.RayOri					= StartPos;
	NtlWorldCollisionInfo.RayDir					= vDir;
	NtlWorldCollisionInfo.RayIntersectionPt4Terrain = EndPos;
	NtlWorldCollisionInfo.fDist						= fDist;

	RpWorldForAllWorldSectorIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldOutdoorCollisionInfo, &NtlWorldCollisionInfo);
	if (dGET_COLLISION_INFO_UPDATE())
	{
		RpWorldForAllAtomicIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldOutdoorCollisionInfo, &NtlWorldCollisionInfo);
	}

	if(NtlWorldCollisionInfo.IsCollidedAtSectors)
	{
		if(NtlWorldCollisionInfo.fDist > dGET_WORLD_PARAM()->WorldFarPlane * 0.9f)
		{
			NtlWorldCollisionInfo.IsCollidedAtSectors = FALSE;
			return FALSE;
		}

		IntersectionPt = NtlWorldCollisionInfo.RayIntersectionPt4Terrain;

		return TRUE;
	}
	return FALSE;
}

RwBool CNtlWorldFieldManager::Pick(RwInt32 ScreenPosX, RwInt32 ScreenPosY, RwV3d& IntersectionPt, sNtlWorldCollisionInfo* pOutWorldCollsionInfo)
{

	RwV2d ScreenPos;
	ScreenPos.x = (RwReal)ScreenPosX;
	ScreenPos.y = (RwReal)ScreenPosY;

	RwLine cameraRay;
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

	return Pick(cameraRay.start, cameraRay.end, IntersectionPt, pOutWorldCollsionInfo);
}

RwBool CNtlWorldFieldManager::Pick(RwV3d& StartPos, RwV3d& EndPos, RwV3d& IntersectionPt, sNtlWorldCollisionInfo* pOutWorldCollsionInfo)
{
	if(m_eMoved2 == eC)
	{
		return FALSE;
	}

	RpIntersection rpIntersection;
	rpIntersection.type			= rpINTERSECTLINE;
	rpIntersection.t.line.start	= StartPos;
	rpIntersection.t.line.end	= EndPos;

	RwV3d	vDir;
	RwReal	fDist;
	RwV3dSubMacro(&vDir, &EndPos, &StartPos);

	fDist = RwV3dLength(&vDir);

	RwV3dNormalize(&vDir, &vDir);

	sNtlWorldCollisionInfo NtlWorldCollisionInfo;
	NtlWorldCollisionInfo.IsCollidedAtSectors		= FALSE;
	NtlWorldCollisionInfo.RayOri					= StartPos;
	NtlWorldCollisionInfo.RayDir					= vDir;
	NtlWorldCollisionInfo.RayIntersectionPt4Terrain = EndPos;
	NtlWorldCollisionInfo.fDist						= fDist;

	RpWorldForAllWorldSectorIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldOutdoorCollisionInfo, &NtlWorldCollisionInfo);
	if (dGET_COLLISION_INFO_UPDATE())
	{
		RpWorldForAllAtomicIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldOutdoorCollisionInfo, &NtlWorldCollisionInfo);
#ifdef dNTL_WORLD_TOOL_MODE
		if (dGET_CUR_PALETTE() == ePM_DECAL || dGET_CUR_PALETTE() == ePM_SHORE_LINE)
		{
			RpWorldForAllWorldSectorIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldOutdoorCollisionInfoWater, &NtlWorldCollisionInfo);
		}
#endif
	}
#ifndef dNTL_WORLD_TOOL_MODE
	RpWorldForAllWorldSectorIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldOutdoorCollisionInfoWater, &NtlWorldCollisionInfo);
#endif

	if(NtlWorldCollisionInfo.IsCollidedAtSectors)
	{		
		if (NtlWorldCollisionInfo.fDist > dGET_WORLD_PARAM()->WorldFarPlane * 0.9f)
		{
			NtlWorldCollisionInfo.IsCollidedAtSectors = FALSE;
		}
		else
		{
			IntersectionPt = NtlWorldCollisionInfo.RayIntersectionPt4Terrain;
		}
	}

	if (pOutWorldCollsionInfo)
	{
		RwBool FlgStopColliding = pOutWorldCollsionInfo->FlgStopColliding;
		memcpy(pOutWorldCollsionInfo, &NtlWorldCollisionInfo, sizeof(sNtlWorldCollisionInfo));
		pOutWorldCollsionInfo->FlgStopColliding = FlgStopColliding;
	}

	return NtlWorldCollisionInfo.IsCollidedAtSectors;
}

RwBool CNtlWorldFieldManager::GetHeight(RwV3d& _Pos)
{
	if(!IsFieldValid(_Pos))
	{
		return FALSE;
	}

	RwInt32 SectorIdx = GetSectorIdx(_Pos);
	if(SectorIdx == -1)
	{
		return FALSE;
	}

	if(IsSectorLoaded(SectorIdx))
	{
		_Pos.y = GetWorldSectorHeight(_Pos);
	}
	else
	{
		RwV3d	CurRVert;
		CurRVert.x = _Pos.x + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
		CurRVert.z = _Pos.z + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
		RwInt32 CurXTileCnt = static_cast<RwInt32>(CurRVert.x / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize));
		RwInt32 CurZTileCnt = static_cast<RwInt32>(CurRVert.z / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize));

		// 3 2
		// 1 0
		RwV3d InterpolationSPos[4];
		InterpolationSPos[0].x = static_cast<RwReal>(CurXTileCnt) * static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
		InterpolationSPos[0].z = static_cast<RwReal>(CurZTileCnt) * static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
		InterpolationSPos[0].x -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
		InterpolationSPos[0].z -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);

		InterpolationSPos[1].x = InterpolationSPos[0].x + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
		InterpolationSPos[1].z = InterpolationSPos[0].z;
		InterpolationSPos[2].x = InterpolationSPos[0].x;
		InterpolationSPos[2].z = InterpolationSPos[0].z + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
		InterpolationSPos[3].x = InterpolationSPos[0].x + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
		InterpolationSPos[3].z = InterpolationSPos[0].z + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);

		RwV3d MPos;
		CNtlMath::MathRwV3dAssign(&MPos, m_pSectors[SectorIdx].DatumPoint.x, m_pSectors[SectorIdx].DatumPoint.y, m_pSectors[SectorIdx].DatumPoint.z);
		GetHeightFromFile(MPos, InterpolationSPos[0]);
		GetHeightFromFile(MPos, InterpolationSPos[1]);
		GetHeightFromFile(MPos, InterpolationSPos[2]);
		GetHeightFromFile(MPos, InterpolationSPos[3]);

		D3DXVECTOR3 LT = D3DXVECTOR3(InterpolationSPos[3].x, InterpolationSPos[0].y, InterpolationSPos[3].z);
		D3DXVECTOR3 RT = D3DXVECTOR3(InterpolationSPos[2].x, InterpolationSPos[1].y, InterpolationSPos[2].z);
		D3DXVECTOR3 LB = D3DXVECTOR3(InterpolationSPos[1].x, InterpolationSPos[2].y, InterpolationSPos[1].z);
		D3DXVECTOR3 RB = D3DXVECTOR3(InterpolationSPos[0].x, InterpolationSPos[3].y, InterpolationSPos[0].z);

		D3DXPLANE p;
		float DX = _Pos.x - RB.x;
		float DZ = _Pos.z - RB.z;
		if(DZ > static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize) - DX)
		{
			D3DXPlaneFromPoints(&p, &LB, &LT, &RT);
		}
		else
		{
			D3DXPlaneFromPoints(&p, &LB, &RT, &RB);
		}

		D3DXPlaneNormalize(&p, &p);

		_Pos.y = -(p.a * _Pos.x + p.c * _Pos.z + p.d) / p.b;
	}

	return TRUE;
}

RwBool CNtlWorldFieldManager::GetHeightFromFile(RwV3d& _PosSectorDatum, RwV3d& _PosTile)
{
	RwInt32 l, m;
	RwInt32 IdxSector	= GetSectorIdx(_PosSectorDatum);
	RwInt32 IdxField	= GetFieldIdx(_PosSectorDatum);

	if(IdxField == -1 || IdxSector == -1)
	{
		return FALSE;
	}

	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxField);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}

	RwReal	TileSize;
	RwReal	SPosX;	
	RwReal	SPosZ;		
	RwReal	CPosX;		
	RwReal	CPosZ;		
	RwInt32 XCnt;		
	RwInt32 ZCnt;		
	RwInt32	CntVert;
	RwV3d	Result;

	RwV3d	SPos = m_pFields[IdxField].GetSPos();
	RwV3d	SectorSPos;
	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;

			if(GetSectorIdx(SectorSPos) == IdxSector)
			{
				TileSize	= (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;
				SPosX		= SectorSPos.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				SPosZ		= SectorSPos.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosX		= _PosTile.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosZ		= _PosTile.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				XCnt		= (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize));
				ZCnt		= (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);
				CntVert		= XCnt + ZCnt * dGET_WORLD_PARAM()->WorldSectorVertNum;

				GetNtlWorldSectorInfo()->SectorMaterialSkipToFile(pFile);
				GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileGetVertex(pFile, CntVert, 1, &Result);
				_PosTile.y = Result.y;

				::fclose(pFile);
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
				return TRUE;
			}
			else
			{				
				GetNtlWorldSectorInfo()->OutdoorSectorSkipToFile(pFile);
			}
		}
	}

	DBO_TRACE(FALSE, "CNtlWorldFieldManager::GetHeightFromFile, nothing matched.");

	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return FALSE;
}

RwBool CNtlWorldFieldManager::GetVertFromFile(RwV3d& DatumPt, RwV3d& Result)
{
	RwInt32 l, m;
	RwInt32 IdxSector	= GetSectorIdx(DatumPt);
	RwInt32 IdxField	= GetFieldIdx(DatumPt);

	if(IdxField == -1 || IdxSector == -1)
	{
		return FALSE;
	}

	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxField);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}

	RwReal	TileSize;
	RwReal	SPosX;	
	RwReal	SPosZ;		
	RwReal	CPosX;		
	RwReal	CPosZ;		
	RwInt32 XCnt;		
	RwInt32 ZCnt;		
	RwInt32	CntVert;

	RwV3d	SPos = m_pFields[IdxField].GetSPos();
	RwV3d	SectorSPos;
	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;

			if(GetSectorIdx(SectorSPos) == IdxSector)
			{
				TileSize	= (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;
				SPosX		= SectorSPos.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				SPosZ		= SectorSPos.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosX		= DatumPt.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosZ		= DatumPt.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				XCnt		= (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize));
				ZCnt		= (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);
				CntVert		= XCnt + ZCnt * dGET_WORLD_PARAM()->WorldSectorVertNum;

				GetNtlWorldSectorInfo()->SectorMaterialSkipToFile(pFile);
				GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileGetVertex(pFile, CntVert, 1, &Result);
			
				::fclose(pFile);
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
				return TRUE;
			}
			else
			{				
				GetNtlWorldSectorInfo()->OutdoorSectorSkipToFile(pFile);
			}
		}
	}

	DBO_TRACE(FALSE, "CNtlWorldFieldManager::GetVertFromFile, file stream read failed.");
	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return FALSE;
}

RwBool CNtlWorldFieldManager::GetVertInMemory(RwV3d& DatumPt, RwV3d& Result)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetVert");

	RwInt32 RetIdx = GetSectorIdx(DatumPt);
	if(RetIdx == -1)
	{
		Result.x = Result.y = Result.z = 0.0f;
		NTL_RETURN(FALSE);
	}

	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(m_pSectors[RetIdx].m_pWorldSector, pNtlSector);
	if(!pNtlSector)
	{
		Result.x = Result.y = Result.z = 0.0f;
		NTL_RETURN(FALSE);
	}

	RwReal	TileSize	= (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwReal	SPosX		= m_pSectors[RetIdx].m_pWorldSector->boundingBox.inf.x + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal	SPosZ		= m_pSectors[RetIdx].m_pWorldSector->boundingBox.inf.z + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal	CPosX		= DatumPt.x + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal	CPosZ		= DatumPt.z + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwInt32 XCnt		= (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize));
	RwInt32 ZCnt		= (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);

	RwV3d	*pVList		= RpMorphTargetGetVertices(pNtlSector->pNtlWorldSector->m_pAtomic->geometry->morphTarget);
	RwInt32	CurVertIdx	= XCnt + ZCnt * dGET_WORLD_PARAM()->WorldSectorVertNum;

	Result = pVList[CurVertIdx];

	NTL_RETURN(TRUE);
}

#ifdef dNTL_WORLD_TOOL_MODE

RwBool CNtlWorldFieldManager::SetHeight(RwV3d& PosSectorDatum, RwV3d& PosTileDatum)
{
	RwInt32 IdxSector	= GetSectorIdx(PosSectorDatum);
	RwInt32 IdxField	= GetFieldIdx(PosSectorDatum);

	if(IdxField == -1 || IdxSector == -1)
	{
		return FALSE;
	}

	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxField);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb+"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}

	RwReal	TileSize;
	RwReal	SPosX;	
	RwReal	SPosZ;		
	RwReal	CPosX;		
	RwReal	CPosZ;		
	RwInt32 XCnt;		
	RwInt32 ZCnt;		
	RwInt32	CntVert;

	RwV3d	SPos = m_pFields[IdxField].GetSPos();
	RwV3d	SectorSPos;
	for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;

			if(GetSectorIdx(SectorSPos) == IdxSector)
			{
				TileSize	= (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;
				SPosX		= SectorSPos.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				SPosZ		= SectorSPos.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosX		= PosTileDatum.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosZ		= PosTileDatum.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				XCnt		= (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize));
				ZCnt		= (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);
				CntVert		= XCnt + ZCnt * dGET_WORLD_PARAM()->WorldSectorVertNum;

				GetNtlWorldSectorInfo()->SectorMaterialSkipToFile(pFile);
				GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileGetVertex(pFile, CntVert, 1, &PosTileDatum);

				::fclose(pFile);
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

				return TRUE;
			}
			else
			{
				GetNtlWorldSectorInfo()->OutdoorSectorSkipToFile(pFile);
			}
		}
	}

	DBO_TRACE(FALSE, "CNtlWorldFieldManager::SetHeight, stream read failed.");

	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return FALSE;
}

RwBool CNtlWorldFieldManager::SetClrVertList(RwV3d& PosCurVert, RwRGBA* pClrVertList)
{
	NTL_FUNCTION("CNtlWorldFieldManager::SetClrVertList");

	RwInt32 IdxSector	= GetSectorIdx(PosCurVert);
	RwInt32 IdxField	= GetFieldIdx(PosCurVert);

	if(IdxField == -1 || IdxSector == -1)
	{
		NTL_RETURN(FALSE);
	}

	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxField);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb+"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		NTL_RETURN(FALSE);
	}

	RwReal	TileSize;
	RwReal	SPosX;	
	RwReal	SPosZ;		
	RwReal	CPosX;		
	RwReal	CPosZ;		
	RwInt32 XCnt;		
	RwInt32 ZCnt;		
	RwInt32	CntVert;

	RwV3d	SPos = m_pFields[IdxField].GetSPos();
	RwV3d	SectorSPos;
	for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;

			if(GetSectorIdx(SectorSPos) == IdxSector)
			{
				TileSize	= (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;
				SPosX		= SectorSPos.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				SPosZ		= SectorSPos.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosX		= PosCurVert.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosZ		= PosCurVert.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				XCnt		= (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize));
				ZCnt		= (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);
				CntVert		= XCnt + ZCnt * dGET_WORLD_PARAM()->WorldSectorVertNum;

				GetNtlWorldSectorInfo()->SectorMaterialSkipToFile(pFile);
				GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFile(pFile);

				::fwrite(pClrVertList, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);
				::fclose(pFile);
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

				NTL_RETURN(TRUE);
			}
			else
			{
				GetNtlWorldSectorInfo()->OutdoorSectorSkipToFile(pFile);
			}
		}
	}

	DBO_TRACE(FALSE, "CNtlWorldFieldManager::SetClrVertList, stream read failed.");

	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(FALSE);		
}

RwBool CNtlWorldFieldManager::GetClrVertList(RwV3d& PosCurVert, RwRGBA* pClrVertList)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetClrVertList");

	RwInt32 IdxSector	= GetSectorIdx(PosCurVert);
	RwInt32 IdxField	= GetFieldIdx(PosCurVert);

	if(IdxField == -1 || IdxSector == -1)
	{
		NTL_RETURN(FALSE);
	}

	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxField);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		NTL_RETURN(FALSE);
	}

	RwReal	TileSize;
	RwReal	SPosX;	
	RwReal	SPosZ;		
	RwReal	CPosX;		
	RwReal	CPosZ;		
	RwInt32 XCnt;		
	RwInt32 ZCnt;		
	RwInt32	CntVert;

	RwV3d	SPos = m_pFields[IdxField].GetSPos();
	RwV3d	SectorSPos;
	for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;

			if(GetSectorIdx(SectorSPos) == IdxSector)
			{
				TileSize	= (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;
				SPosX		= SectorSPos.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				SPosZ		= SectorSPos.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosX		= PosCurVert.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosZ		= PosCurVert.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				XCnt		= (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize));
				ZCnt		= (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);
				CntVert		= XCnt + ZCnt * dGET_WORLD_PARAM()->WorldSectorVertNum;

				GetNtlWorldSectorInfo()->SectorMaterialSkipToFile(pFile);

				::fseek(pFile, sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, SEEK_CUR);
				::fread(pClrVertList, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);
				::fclose(pFile);
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

				NTL_RETURN(TRUE);
			}
			else
			{
				GetNtlWorldSectorInfo()->OutdoorSectorSkipToFile(pFile);
			}
		}
	}

	DBO_TRACE(FALSE, "CNtlWorldFieldManager::GetClrVertList, stream read failed.");

	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(FALSE);		
}

RwBool CNtlWorldFieldManager::GetPosVertList(RwV3d& PosCurVert, RwV3d* pPosVertList)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetPosVertList");

	RwInt32 IdxSector	= GetSectorIdx(PosCurVert);
	RwInt32 IdxField	= GetFieldIdx(PosCurVert);

	if(IdxField == -1 || IdxSector == -1)
	{
		NTL_RETURN(FALSE);
	}

	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxField);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		NTL_RETURN(FALSE);
	}

	RwV3d	SPos = m_pFields[IdxField].GetSPos();
	RwV3d	SectorSPos;
	for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;

			if(GetSectorIdx(SectorSPos) == IdxSector)
			{
				GetNtlWorldSectorInfo()->SectorMaterialSkipToFile(pFile);
				GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileGetVertex(pFile, 0, dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, pPosVertList);

				::fclose(pFile);

				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

				NTL_RETURN(TRUE);
			}
			else
			{
				GetNtlWorldSectorInfo()->OutdoorSectorSkipToFile(pFile);
			}
		}
	}

	DBO_TRACE(FALSE, "CNtlWorldFieldManager::GetPosVertList, stream read failed.");

	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(FALSE);
}

void CNtlWorldFieldManager::OnSetSlopeLighting(RwUInt32 _IdxField)
{
	// exceptions
	if(!IsFieldValid(_IdxField))
	{
		DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid field index.(" << _IdxField << ")");		
		return;
	}

	// update slope lighting looping sectors
	RwV3d	SPos = m_pFields[_IdxField].GetSPos();
	RwV3d	PosSectorS;
	for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			PosSectorS.x = (RwReal)m;
			PosSectorS.z = (RwReal)l;
			OnSetSlopeLighting(GetSectorIdx(PosSectorS));
		}
	}
}

void CNtlWorldFieldManager::OnSetSlopeLighting(RwInt32 IdxSector)
{
	if(!IsSectorValid(IdxSector))
	{
		DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector index.(" << IdxSector << ")");
		return;
	}

	RwReal			LightPos0, LightPos1, Result;
	RwV3d			RetVert;
	RwV3d			TmpVert;
	RwV3d*			pVList;
	RwV3d			vSrcPos;
	RpAtomic*		pAtomic;
	RwRGBA*			pPrelights;
	RwInt32			NumVert;

	RwReal			TileSize			= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
	sNTL_WORLD_SL*	pNtlWorldSL			= NULL;
	RwBool			IsCurSectorLoaded	= IsSectorLoaded(IdxSector);

	if(IsCurSectorLoaded)
	{
		pNtlWorldSL	= m_pSectors[IdxSector].m_pNtlWorldSL;
		pAtomic		= m_pSectors[IdxSector].m_pAtomic;
		pVList		= RpMorphTargetGetVertices(pAtomic->geometry->morphTarget);
		pPrelights	= RpGeometryGetPreLightColors(pAtomic->geometry);
		NumVert		= RpGeometryGetNumVertices(pAtomic->geometry);

		::RpGeometryLock(pAtomic->geometry, rpGEOMETRYLOCKPRELIGHT);

		for(int i = 0; i < NumVert; ++i)
		{
			CNtlMath::MathRwV3dAssign(&vSrcPos, pVList[i].x, pVList[i].y, pVList->z);

			// 3---2---1
			// |       | 
			// 4  cur  0
			// |       |
			// 5---6---7

			switch(pNtlWorldSL->m_Dir)
			{
			case 0:
				{
					TmpVert.x = pVList[i].x - TileSize;
					TmpVert.z = pVList[i].z;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");		
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 1:
				{
					TmpVert.x = pVList[i].x - TileSize;
					TmpVert.z = pVList[i].z + TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 2:
				{
					TmpVert.x = pVList[i].x;
					TmpVert.z = pVList[i].z + TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 3:
				{
					TmpVert.x = pVList[i].x + TileSize;
					TmpVert.z = pVList[i].z + TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 4:
				{	
					TmpVert.x = pVList[i].x + TileSize;
					TmpVert.z = pVList[i].z;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 5:
				{
					TmpVert.x = pVList[i].x + TileSize;
					TmpVert.z = pVList[i].z - TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 6:
				{
					TmpVert.x = pVList[i].x;
					TmpVert.z = pVList[i].z - TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 7:
				{
					TmpVert.x = pVList[i].x - TileSize;
					TmpVert.z = pVList[i].z - TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;
			}

			TmpVert.x = pVList[i].x;
			TmpVert.z = pVList[i].z;

			if(!IsSectorValid(TmpVert))
			{
				DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
				continue;
			}

			if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
			{
				if(!GetVertFromFile(TmpVert, RetVert))
				{
					continue;
				}
			}
			else
			{
				if(!GetVertInMemory(TmpVert, RetVert))
				{
					continue;
				}
			}
			LightPos1 = RetVert.y;

			// calculate a slope lighting value
			Result = (1.0f - (LightPos0 - LightPos1) / pNtlWorldSL->m_Softness);
			CLAMP(Result, pNtlWorldSL->m_Brightness[0], pNtlWorldSL->m_Brightness[1]);

			RwReal ClrDiffuseR	= (RwReal)((m_pSectors[IdxSector].m_pClrDiffusePalette[i] & 0x00ff0000) >> 16) / 255.0f;
			RwReal ClrDiffuseG	= (RwReal)((m_pSectors[IdxSector].m_pClrDiffusePalette[i] & 0x0000ff00) >> 8) / 255.0f;
			RwReal ClrDiffuseB	= (RwReal)((m_pSectors[IdxSector].m_pClrDiffusePalette[i] & 0x000000ff) >> 0) / 255.0f;

			RwReal ClrSLR = (1.0f - pNtlWorldSL->_Clr.red) * Result + pNtlWorldSL->_Clr.red;
			RwReal ClrSLG = (1.0f - pNtlWorldSL->_Clr.green) * Result + pNtlWorldSL->_Clr.green;
			RwReal ClrSLB = (1.0f - pNtlWorldSL->_Clr.blue) * Result + pNtlWorldSL->_Clr.blue;

			if(static_cast<RwInt32>(pNtlWorldSL->_Clr.alpha))
			{
				(pPrelights[i]).red		= (RwUInt8)(ClrSLR * ClrDiffuseR * 255.0f);
				(pPrelights[i]).green	= (RwUInt8)(ClrSLG * ClrDiffuseG * 255.0f);
				(pPrelights[i]).blue	= (RwUInt8)(ClrSLB * ClrDiffuseB * 255.0f);
			}
			else
			{
				(pPrelights[i]).red		= (RwUInt8)(pNtlWorldSL->_Clr.red * ClrDiffuseR * 255.0f);
				(pPrelights[i]).green	= (RwUInt8)(pNtlWorldSL->_Clr.green * ClrDiffuseG * 255.0f);
				(pPrelights[i]).blue	= (RwUInt8)(pNtlWorldSL->_Clr.blue * ClrDiffuseB * 255.0f);
			}
		}

		::RpGeometryUnlock(pAtomic->geometry);
	}
	else
	{
		NumVert = dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;

		RwV3d*			pPosVertList	= NTL_NEW RwV3d [NumVert];
		RwRGBA*			pClrVertList	= NTL_NEW RwRGBA [NumVert];
		sNTL_WORLD_SL*	pNtlWorldSL		= NTL_NEW sNTL_WORLD_SL;
		DWORD*			pClrDiffuseList = NTL_NEW DWORD [NumVert];

		RwV3d PosSectorDatum;
		CNtlMath::MathRwV3dAssign(&PosSectorDatum, m_pSectors[IdxSector].DatumPoint.x, m_pSectors[IdxSector].DatumPoint.y, m_pSectors[IdxSector].DatumPoint.z);
		GetPosVertList(PosSectorDatum, pPosVertList);

		::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		::_chdir("swap");

		// Diffuse swap file
		::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.dif", IdxSector);

		FILE* pFile;
		if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			NTL_RETURNVOID();
		}

		::fread(pClrDiffuseList, sizeof(DWORD) * NumVert, 1, pFile);
		::fclose(pFile);

		// Slope lighting prop.
		::_chdir("sl");
		::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sl", IdxSector);

		if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			NTL_RETURNVOID();
		}

		::fread(pNtlWorldSL, sizeof(sNTL_WORLD_SL), 1, pFile);
		::fclose(pFile);

		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

		// cacluating slope lighting
		for(int i = 0; i < NumVert; ++i)
		{
			CNtlMath::MathRwV3dAssign(&vSrcPos, pPosVertList[i].x, pPosVertList[i].y, pPosVertList->z);

			// 3---2---1
			// |       | 
			// 4  cur  0
			// |       |
			// 5---6---7

			switch(pNtlWorldSL->m_Dir)
			{
			case 0:
				{
					TmpVert.x = pPosVertList[i].x - TileSize;
					TmpVert.z = pPosVertList[i].z;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 1:
				{
					TmpVert.x = pPosVertList[i].x - TileSize;
					TmpVert.z = pPosVertList[i].z + TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 2:
				{
					TmpVert.x = pPosVertList[i].x;
					TmpVert.z = pPosVertList[i].z + TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 3:
				{
					TmpVert.x = pPosVertList[i].x + TileSize;
					TmpVert.z = pPosVertList[i].z + TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 4:
				{	
					TmpVert.x = pPosVertList[i].x + TileSize;
					TmpVert.z = pPosVertList[i].z;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 5:
				{
					TmpVert.x = pPosVertList[i].x + TileSize;
					TmpVert.z = pPosVertList[i].z - TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 6:
				{
					TmpVert.x = pPosVertList[i].x;
					TmpVert.z = pPosVertList[i].z - TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 7:
				{
					TmpVert.x = pPosVertList[i].x - TileSize;
					TmpVert.z = pPosVertList[i].z - TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;
			}

			TmpVert.x = pPosVertList[i].x;
			TmpVert.z = pPosVertList[i].z;

			if(!IsSectorValid(TmpVert))
			{
				DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
				continue;
			}

			if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
			{
				if(!GetVertFromFile(TmpVert, RetVert))
				{
					continue;
				}
			}
			else
			{
				if(!GetVertInMemory(TmpVert, RetVert))
				{
					continue;
				}
			}
			LightPos1 = RetVert.y;

			// calculate a slope lighting value
			Result = (1.0f - (LightPos0 - LightPos1) / pNtlWorldSL->m_Softness);
			CLAMP(Result, pNtlWorldSL->m_Brightness[0], pNtlWorldSL->m_Brightness[1]);

			RwReal ClrDiffuseR	= (RwReal)((pClrDiffuseList[i] & 0x00ff0000) >> 16) / 255.0f;
			RwReal ClrDiffuseG	= (RwReal)((pClrDiffuseList[i] & 0x0000ff00) >> 8) / 255.0f;
			RwReal ClrDiffuseB	= (RwReal)((pClrDiffuseList[i] & 0x000000ff) >> 0) / 255.0f;

			RwReal ClrSLR = (1.0f - pNtlWorldSL->_Clr.red) * Result + pNtlWorldSL->_Clr.red;
			RwReal ClrSLG = (1.0f - pNtlWorldSL->_Clr.green) * Result + pNtlWorldSL->_Clr.green;
			RwReal ClrSLB = (1.0f - pNtlWorldSL->_Clr.blue) * Result + pNtlWorldSL->_Clr.blue;

			if(static_cast<RwInt32>(pNtlWorldSL->_Clr.alpha))
			{
				(pClrVertList[i]).red	= (RwUInt8)(ClrSLR * ClrDiffuseR * 255.0f);
				(pClrVertList[i]).green	= (RwUInt8)(ClrSLG * ClrDiffuseG * 255.0f);
				(pClrVertList[i]).blue	= (RwUInt8)(ClrSLB * ClrDiffuseB * 255.0f);
			}
			else
			{
				(pClrVertList[i]).red	= (RwUInt8)(pNtlWorldSL->_Clr.red * ClrDiffuseR * 255.0f);
				(pClrVertList[i]).green	= (RwUInt8)(pNtlWorldSL->_Clr.green * ClrDiffuseG * 255.0f);
				(pClrVertList[i]).blue	= (RwUInt8)(pNtlWorldSL->_Clr.blue * ClrDiffuseB * 255.0f);
			}
		}

		SetClrVertList(PosSectorDatum, pClrVertList);

		NTL_ARRAY_DELETE(pPosVertList);
		NTL_ARRAY_DELETE(pClrVertList);
		NTL_DELETE(pNtlWorldSL);
		NTL_ARRAY_DELETE(pClrDiffuseList);
	}
}

void CNtlWorldFieldManager::OnSetSlopeLighting(vector<sTARGET_VERT_SL_INFO>& vecTargetVertSLInfo)
{
	for(int j = 0; j < (int)vecTargetVertSLInfo.size(); ++j)
	{
		RpAtomic	*pCurAtomic = vecTargetVertSLInfo[j].pNtlWorldSector->m_pAtomic;
		RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
		RwRGBA		*pPrelights = RpGeometryGetPreLightColors(pCurAtomic->geometry);
		RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
		RwInt32		IdxVert		= vecTargetVertSLInfo[j].IdxVert;
		RwReal		PosX		= pVList[IdxVert].x;
		RwReal		PosZ		= pVList[IdxVert].z;
		RwInt32		TileSize	= dGET_WORLD_PARAM()->WorldSectorTileSize;
		RwReal		LightPos0, LightPos1, Result;

		RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKPRELIGHT);
		{
			//
			// 3---2---1
			// |       | 
			// 4  cur  0
			// |       |
			// 5---6---7
			// 

			RwV3d			RetVert;
			RwV3d			TmpVert;
			sNTL_WORLD_SL*	pNtlWorldSL = vecTargetVertSLInfo[j].pNtlWorldSector->m_pNtlWorldSL;

			if(pNtlWorldSL->m_Dir == 0)
			{
				TmpVert.x = pVList[IdxVert].x - TileSize;
				TmpVert.z = pVList[IdxVert].z;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 1)
			{
				TmpVert.x = pVList[IdxVert].x - TileSize;
				TmpVert.z = pVList[IdxVert].z + TileSize;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 2)
			{
				TmpVert.x = pVList[IdxVert].x;
				TmpVert.z = pVList[IdxVert].z + TileSize;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 3)
			{
				TmpVert.x = pVList[IdxVert].x + TileSize;
				TmpVert.z = pVList[IdxVert].z + TileSize;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 4)
			{
				TmpVert.x = pVList[IdxVert].x + TileSize;
				TmpVert.z = pVList[IdxVert].z;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 5)
			{
				TmpVert.x = pVList[IdxVert].x + TileSize;
				TmpVert.z = pVList[IdxVert].z - TileSize;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 6)
			{
				TmpVert.x = pVList[IdxVert].x;
				TmpVert.z = pVList[IdxVert].z - TileSize;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 7)
			{
				TmpVert.x = pVList[IdxVert].x - TileSize;
				TmpVert.z = pVList[IdxVert].z - TileSize;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}

			TmpVert.x = pVList[IdxVert].x;
			TmpVert.z = pVList[IdxVert].z;

			if(!IsSectorValid(TmpVert))
			{
				DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
				continue;
			}

			if(!GetVertInMemory(TmpVert, RetVert))
				continue;
			LightPos1 = RetVert.y;

			// calculate a slope lighting value
			Result = (1.0f - (LightPos0 - LightPos1) / pNtlWorldSL->m_Softness);
			CLAMP(Result, pNtlWorldSL->m_Brightness[0], pNtlWorldSL->m_Brightness[1]);

			RwReal ClrDiffuseR	= (RwReal)((vecTargetVertSLInfo[j].pNtlWorldSector->m_pClrDiffusePalette[IdxVert] & 0x00ff0000) >> 16) / 255.0f;
			RwReal ClrDiffuseG	= (RwReal)((vecTargetVertSLInfo[j].pNtlWorldSector->m_pClrDiffusePalette[IdxVert] & 0x0000ff00) >> 8) / 255.0f;
			RwReal ClrDiffuseB	= (RwReal)((vecTargetVertSLInfo[j].pNtlWorldSector->m_pClrDiffusePalette[IdxVert] & 0x000000ff) >> 0) / 255.0f;

			RwReal ClrSLR = (1.0f - pNtlWorldSL->_Clr.red) * Result + pNtlWorldSL->_Clr.red;
			RwReal ClrSLG = (1.0f - pNtlWorldSL->_Clr.green) * Result + pNtlWorldSL->_Clr.green;
			RwReal ClrSLB = (1.0f - pNtlWorldSL->_Clr.blue) * Result + pNtlWorldSL->_Clr.blue;

			if(static_cast<RwInt32>(pNtlWorldSL->_Clr.alpha))
			{
				(pPrelights[IdxVert]).red	= (RwUInt8)(ClrSLR * ClrDiffuseR * 255.0f);
				(pPrelights[IdxVert]).green	= (RwUInt8)(ClrSLG * ClrDiffuseG * 255.0f);
				(pPrelights[IdxVert]).blue	= (RwUInt8)(ClrSLB * ClrDiffuseB * 255.0f);
			}
			else
			{
				(pPrelights[IdxVert]).red	= (RwUInt8)(pNtlWorldSL->_Clr.red * ClrDiffuseR * 255.0f);
				(pPrelights[IdxVert]).green	= (RwUInt8)(pNtlWorldSL->_Clr.green * ClrDiffuseG * 255.0f);
				(pPrelights[IdxVert]).blue	= (RwUInt8)(pNtlWorldSL->_Clr.blue * ClrDiffuseB * 255.0f);
			}
		}
		RpGeometryUnlock(pCurAtomic->geometry);
	}

	vecTargetVertSLInfo.clear();
}

#endif

RwBool CNtlWorldFieldManager::SetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp, eNTL_FIELD_PROPID NtlFieldPropID)
{
	/*
	WorldFileFormat - FieldProperty
	Change Field property to File or Memory information.
	*/

	RwInt32 NumSectorTile	= (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2);
	RwInt32 Idx				= GetFieldIdx(Pos);


	if(Idx == -1)
	{
		return FALSE;
	}
	else
	{
		if(!IsFieldLoaded(Idx))
		{
			_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
			_chdir("fields");

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", Idx);
			_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

			FILE *pFile = NULL;
			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", Idx);
			if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb+") != 0)
			{
				DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
				return FALSE;
			}
			else
			{					
				switch(NtlFieldPropID)
				{
				case eNFP_NAME:
					{			
						::fwrite(NtlFieldProp._Name, 64, 1, pFile);

						break;
					}
				case eNFP_BITPROP:
					{
						::fseek(pFile, 64, SEEK_CUR);
						::fwrite(NtlFieldProp._pBitProp, sizeof(DWORD) * NumSectorTile, 1, pFile);

						break;
					}
				case eNFP_FOG:
					{
						::fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile, SEEK_CUR);
						::fwrite(&NtlFieldProp._FogColor, sizeof(RwRGBA), 1, pFile);
						::fwrite(&NtlFieldProp._FogCamPlane[0], sizeof(RwReal), 1, pFile);
						::fwrite(&NtlFieldProp._FogCamPlane[1], sizeof(RwReal), 1, pFile);

						break;
					}
				case eNFP_SKY:
					{
						::fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal), SEEK_CUR);
						::fwrite(&NtlFieldProp._BaseSkyMode, sizeof(RwInt32), 1, pFile);
						::fwrite(&NtlFieldProp._RGBSkyColor[0], sizeof(RwRGBA), 1, pFile);
						::fwrite(&NtlFieldProp._RGBSkyColor[1], sizeof(RwRGBA), 1, pFile);
						::fwrite(&NtlFieldProp._RGBSkyColor[2], sizeof(RwRGBA), 1, pFile);

						::fwrite(&NtlFieldProp._NewSkyValue, sizeof(RwReal), 1, pFile);

						::fwrite(NtlFieldProp._BaseSkyTexName, sizeof(RwChar) * 10, 1, pFile);
						::fwrite(&NtlFieldProp._BaseSkySpeed, sizeof(RwReal), 1, pFile);
						::fwrite(NtlFieldProp._BlendedTexName[0], sizeof(RwChar) * 10, 1, pFile);
						::fwrite(NtlFieldProp._BlendedTexName[1], sizeof(RwChar) * 10, 1, pFile);
						::fwrite(&NtlFieldProp._BlendedTexSpeed[0], sizeof(RwReal), 1, pFile);
						::fwrite(&NtlFieldProp._BlendedTexSpeed[1], sizeof(RwReal), 1, pFile);

						break;
					}
				case eNFP_LIGHT:
					{
						::fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2), SEEK_CUR);
						::fwrite(&NtlFieldProp._ClrLightAmbient, sizeof(RwRGBAReal), 1, pFile);
						for (int i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
						{
							::fwrite(&NtlFieldProp._ClrLightDirectional[i], sizeof(RwRGBAReal), 1, pFile);
							::fwrite(&NtlFieldProp._AngLightDirectional[i], sizeof(RwV3d), 1, pFile);
						}

						break;
					}
				case eNFP_SOUND:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3), SEEK_CUR);
						fwrite(&NtlFieldProp._IdxBGMTbl, sizeof(RwInt32), 1, pFile);
						fwrite(&NtlFieldProp._IdxEnvTbl, sizeof(RwInt32), 1, pFile);
						for (int i = 0; i < 5; ++i)
						{
							fwrite(&NtlFieldProp._IdxShareTbl[i], sizeof(RwInt32), 1, pFile);
						}

						break;
					}
				case eNFP_BLOOM:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + (sizeof(RwInt32) * 5), SEEK_CUR);
						fwrite(&NtlFieldProp._MonoPower, sizeof(RwReal), 1, pFile);

						break;
					}
				case eNFP_PLANET:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) + (sizeof(RwInt32) * 5), SEEK_CUR);

						for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
							fwrite(&NtlFieldProp._NtlPlanet[i], sizeof(sNTL_PLANET), 1, pFile);

						break;
					}
				case eNFP_SPECULAR:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) + (sizeof(sNTL_PLANET) * 3) + (sizeof(RwInt32) * 5), SEEK_CUR);

						fwrite(&NtlFieldProp._NtlSpecular, sizeof(sNTL_SPECULAR), 1, pFile);

						break;
					}
				case eNFP_MATERIALPROP:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) + (sizeof(sNTL_PLANET) * 3) + sizeof(sNTL_SPECULAR) + (sizeof(RwInt32) * 5), SEEK_CUR);

						fwrite(NtlFieldProp._pMaterialProp, sizeof(BYTE) * NumSectorTile, 1, pFile);

						break;
					}
				case eNFP_FIELDCOLOR:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) + (sizeof(sNTL_PLANET) * 3) + sizeof(sNTL_SPECULAR) + (sizeof(BYTE) * NumSectorTile) + (sizeof(RwInt32) * 5), SEEK_CUR);

						fwrite(&NtlFieldProp._FieldColor, sizeof(RwRGBA), 1, pFile);

						break;
					}
				case eNFP_WEATHER:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) + (sizeof(sNTL_PLANET) * 3) + sizeof(sNTL_SPECULAR) + (sizeof(BYTE) * NumSectorTile) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5), SEEK_CUR);

						fwrite(&NtlFieldProp._IdxWeather, sizeof(RwInt32) * 5, 1, pFile);

						break;
					}
				case eNFP_FIELDHEATHAZE:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) + (sizeof(sNTL_PLANET) * 3) + sizeof(sNTL_SPECULAR) + (sizeof(BYTE) * NumSectorTile) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + (sizeof(RwInt32) * 5), SEEK_CUR);

						fwrite(&NtlFieldProp._NtlHeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);
					}

				}

				::fclose(pFile);
			}

			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

			SetAnotherField();

			return TRUE;
		}
		else
		{
			switch(NtlFieldPropID)
			{
			case eNFP_NAME:
				{
					strcpy_s(m_pFields[Idx].GetProp()->_Name, NtlFieldProp._Name);

					break;
				}
			case eNFP_BITPROP:
				{
					::CopyMemory(m_pFields[Idx].GetProp()->_pBitProp, NtlFieldProp._pBitProp, sizeof(DWORD) * NumSectorTile);

					break;
				}
			case eNFP_FOG:
				{
					m_pFields[Idx].GetProp()->_FogColor.red		= NtlFieldProp._FogColor.red;
					m_pFields[Idx].GetProp()->_FogColor.green	= NtlFieldProp._FogColor.green;
					m_pFields[Idx].GetProp()->_FogColor.blue	= NtlFieldProp._FogColor.blue;
					m_pFields[Idx].GetProp()->_FogCamPlane[0]	= NtlFieldProp._FogCamPlane[0];
					m_pFields[Idx].GetProp()->_FogCamPlane[1]	= NtlFieldProp._FogCamPlane[1];	

					break;
				}
			case eNFP_SKY:
				{
					m_pFields[Idx].GetProp()->_BaseSkyMode	= NtlFieldProp._BaseSkyMode;
					memcpy(&m_pFields[Idx].GetProp()->_RGBSkyColor[0], &NtlFieldProp._RGBSkyColor[0], sizeof(RwRGBA));
					memcpy(&m_pFields[Idx].GetProp()->_RGBSkyColor[1], &NtlFieldProp._RGBSkyColor[1], sizeof(RwRGBA));
					memcpy(&m_pFields[Idx].GetProp()->_RGBSkyColor[2], &NtlFieldProp._RGBSkyColor[2], sizeof(RwRGBA));

					m_pFields[Idx].GetProp()->_NewSkyValue = NtlFieldProp._NewSkyValue; // new

					strcpy_s(m_pFields[Idx].GetProp()->_BaseSkyTexName, NtlFieldProp._BaseSkyTexName);
					m_pFields[Idx].GetProp()->_BaseSkySpeed = NtlFieldProp._BaseSkySpeed;
					strcpy_s(m_pFields[Idx].GetProp()->_BlendedTexName[0], NtlFieldProp._BlendedTexName[0]);
					strcpy_s(m_pFields[Idx].GetProp()->_BlendedTexName[1], NtlFieldProp._BlendedTexName[1]);
					m_pFields[Idx].GetProp()->_BlendedTexSpeed[0] = NtlFieldProp._BlendedTexSpeed[0];
					m_pFields[Idx].GetProp()->_BlendedTexSpeed[1] = NtlFieldProp._BlendedTexSpeed[1];

					break;
				}
			case eNFP_LIGHT:
				{
					::CopyMemory(&m_pFields[Idx].GetProp()->_ClrLightAmbient, &NtlFieldProp._ClrLightAmbient, sizeof(RwRGBAReal));
					for (int i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
					{
						::CopyMemory(&m_pFields[Idx].GetProp()->_ClrLightDirectional[i], &NtlFieldProp._ClrLightDirectional[i], sizeof(RwRGBAReal));
						::CopyMemory(&m_pFields[Idx].GetProp()->_AngLightDirectional[i], &NtlFieldProp._AngLightDirectional[i], sizeof(RwV3d));
					}
					break;
				}
			case eNFP_SOUND:
				{
					m_pFields[Idx].GetProp()->_IdxBGMTbl = NtlFieldProp._IdxBGMTbl;
					m_pFields[Idx].GetProp()->_IdxEnvTbl = NtlFieldProp._IdxEnvTbl;
					for (int i = 0; i < 5; ++i)
					{
						m_pFields[Idx].GetProp()->_IdxShareTbl[i] = NtlFieldProp._IdxShareTbl[i];
					}
					break;
				}

			case eNFP_BLOOM:
				{
					m_pFields[Idx].GetProp()->_MonoPower = NtlFieldProp._MonoPower;

					break;
				}
			case eNFP_PLANET:
				{
					m_pFields[Idx].GetProp()->_NtlPlanet[0] = NtlFieldProp._NtlPlanet[0];
					m_pFields[Idx].GetProp()->_NtlPlanet[1] = NtlFieldProp._NtlPlanet[1];
					m_pFields[Idx].GetProp()->_NtlPlanet[2] = NtlFieldProp._NtlPlanet[2];

					break;
				}
			case eNFP_SPECULAR:
				{					
					m_pFields[Idx].GetProp()->_NtlSpecular = NtlFieldProp._NtlSpecular;

					break;
				}
			case eNFP_MATERIALPROP:
				{
					::CopyMemory(m_pFields[Idx].GetProp()->_pMaterialProp, NtlFieldProp._pMaterialProp, sizeof(BYTE) * NumSectorTile);

					break;
				}
			case eNFP_FIELDCOLOR:
				{
					m_pFields[Idx].GetProp()->_FieldColor = NtlFieldProp._FieldColor;

					break;
				}
			case eNFP_WEATHER:
				{
					for (int i = 0; i < 5; ++i)
					{
						m_pFields[Idx].GetProp()->_IdxWeather[i] = NtlFieldProp._IdxWeather[i];
					}

					break;
				}			
			case eNFP_FIELDHEATHAZE:
				{
					m_pFields[Idx].GetProp()->_NtlHeatHaze = NtlFieldProp._NtlHeatHaze;

					break;
				}
			}

			SetAnotherField();

			return TRUE;
		}	
	}

	return FALSE;
}

RwBool CNtlWorldFieldManager::GetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp)
{
	/*
	WorldFileFormat - FieldProperty
	Field Property를 File 또는 Memory로 부터 정보를 가져온다.
	*/

	RwInt32 Idx = GetFieldIdx(Pos);
	if(Idx == -1)
	{
		return FALSE;
	}
	else
	{
		if(!IsFieldLoaded(Idx))
		{
			FILE *pFile = NULL;
			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", Idx);
			if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
			{
				static RwChar chPackPatch[NTL_MAX_DIR_PATH];
				strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
				strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "fields");
				strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
				strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);
				strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");

				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", Idx);

				strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				SPackResFileData sPackFileData;
				RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(chPackPatch, sPackFileData);
				if(bPack)
				{
					_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
					::fopen_s(&pFile, sPackFileData.strPackFileName.c_str(), "rb");
					if(pFile)
					{
						fseek(pFile, sPackFileData.uiOffset, SEEK_SET);
					}
				}
			}
			else
			{
				_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
				_chdir("fields");		
				_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", Idx);
				::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
				_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			}

			if(!pFile)
			{
				DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
				return FALSE;
			}
			else
			{
				fread(NtlFieldProp._Name, 64, 1, pFile);
				fread(NtlFieldProp._pBitProp, sizeof(DWORD) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
				fread(&NtlFieldProp._FogColor, sizeof(RwRGBA), 1, pFile);
				fread(&NtlFieldProp._FogCamPlane[0], sizeof(RwReal), 1, pFile);
				fread(&NtlFieldProp._FogCamPlane[1], sizeof(RwReal), 1, pFile);
				fread(&NtlFieldProp._BaseSkyMode, sizeof(RwInt32), 1, pFile);
				fread(&NtlFieldProp._RGBSkyColor[0], sizeof(RwRGBA), 1, pFile);
				fread(&NtlFieldProp._RGBSkyColor[1], sizeof(RwRGBA), 1, pFile);
				fread(&NtlFieldProp._RGBSkyColor[2], sizeof(RwRGBA), 1, pFile);

				fread(&NtlFieldProp._NewSkyValue, sizeof(RwReal), 1, pFile);

				fread(NtlFieldProp._BaseSkyTexName, sizeof(RwChar) * 10, 1, pFile);
				fread(&NtlFieldProp._BaseSkySpeed, sizeof(RwReal), 1, pFile);
				fread(NtlFieldProp._BlendedTexName[0], sizeof(RwChar) * 10, 1, pFile);
				fread(NtlFieldProp._BlendedTexName[1], sizeof(RwChar) * 10, 1, pFile);
				fread(&NtlFieldProp._BlendedTexSpeed[0], sizeof(RwReal), 1, pFile);
				fread(&NtlFieldProp._BlendedTexSpeed[1], sizeof(RwReal), 1, pFile);
				fread(&NtlFieldProp._ClrLightAmbient, sizeof(RwRGBAReal), 1, pFile);

				for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
				{
					fread(&NtlFieldProp._ClrLightDirectional[i], sizeof(RwRGBAReal), 1, pFile);
					fread(&NtlFieldProp._AngLightDirectional[i], sizeof(RwV3d), 1, pFile);
					//break;
				}				

				fread(&NtlFieldProp._IdxBGMTbl, sizeof(RwInt32), 1, pFile);
				fread(&NtlFieldProp._IdxEnvTbl, sizeof(RwInt32), 1, pFile);
				for(RwInt32 i = 0; i < 5; ++i)
				{
					fread(&NtlFieldProp._IdxShareTbl[i], sizeof(RwInt32), 1, pFile);
				}

				fread(&NtlFieldProp._MonoPower, sizeof(RwReal), 1, pFile);

				for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
					fread(&NtlFieldProp._NtlPlanet[i], sizeof(sNTL_PLANET), 1, pFile);

				fread(&NtlFieldProp._NtlSpecular, sizeof(sNTL_SPECULAR), 1, pFile);
				fread(NtlFieldProp._pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
				fread(&NtlFieldProp._FieldColor, sizeof(RwRGBA), 1, pFile);
				fread(&NtlFieldProp._IdxWeather, sizeof(RwInt32) * 5, 1, pFile);
				fread(&NtlFieldProp._NtlHeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);

				fclose(pFile);
			}

			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

			return TRUE;
		}
		else
		{
			NtlFieldProp = *m_pFields[Idx].GetProp();

			return TRUE;
		}	
	}

	return FALSE;
}

DWORD CNtlWorldFieldManager::GetMapTileProp(RwV3d& Pos)
{
	RwInt32 FieldIdx = GetFieldIdx(Pos);
	if(FieldIdx == -1)
	{
		return 0xffffffff;
	}
	else
	{
		RwV3d	SPos		= m_pFields[FieldIdx].GetSPos();
		RwInt32	XMoveCnt	= static_cast<RwInt32>(abs(Pos.x - SPos.x)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
		RwInt32	ZMoveCnt	= static_cast<RwInt32>(abs(Pos.z - SPos.z)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
		RwInt32 PosBitFlag	= XMoveCnt + (ZMoveCnt * (dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldFieldSectorNum));

		if(!m_pFields[FieldIdx].GetProp())
		{
			return 0xffffffff;
		}

		return m_pFields[FieldIdx].GetProp()->_pBitProp[PosBitFlag];
	}

	return 0xffffffff;
}

BYTE CNtlWorldFieldManager::GetMaterialTileProp(RwV3d& Pos)
{
	RwInt32 FieldIdx = GetFieldIdx(Pos);
	if(FieldIdx == -1)
	{
		return 0;
	}
	else
	{
		RwV3d	SPos		= m_pFields[FieldIdx].GetSPos();
		RwInt32	XMoveCnt	= static_cast<RwInt32>(abs(Pos.x - SPos.x)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
		RwInt32	ZMoveCnt	= static_cast<RwInt32>(abs(Pos.z - SPos.z)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
		RwInt32 Index		= XMoveCnt + (ZMoveCnt * (dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldFieldSectorNum));

		if(!m_pFields[FieldIdx].GetProp())
		{
			return 0;
		}

		return m_pFields[FieldIdx].GetProp()->_pMaterialProp[Index];
	}

	return 0;

}

RwReal CNtlWorldFieldManager::GetWaterHeight(RwV3d& Pos)
{
	RwInt32 SectorIdx = GetSectorIdx(Pos);
	if(SectorIdx == -1)
	{
		return -9999.0f;
	}
	else
	{
		if(!m_pSectors[SectorIdx].m_pWater)
		{
			return -9999.0f;
		}
		else
		{
			return m_pSectors[SectorIdx].m_pWater->_Height;
		}
	}

	return -9999.0f;
}

RwBool CNtlWorldFieldManager::GetFieldSectorIndices(RwInt32 _FieldIdx, RwInt32 _FieldSectorIndices[4])
{
	if(_FieldIdx == -1)
	{
		return FALSE;
	}

	RwV3d Pos = m_pFields[_FieldIdx].GetSPos();
	RwV3d Tmp;

	// 
	//  3 2
	//  1 0
	//
	_FieldSectorIndices[0] = GetSectorIdx(Pos);

	Tmp = Pos;
	Tmp.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	_FieldSectorIndices[1] = GetSectorIdx(Tmp);

	Tmp = Pos;
	Tmp.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	_FieldSectorIndices[2] = GetSectorIdx(Tmp);

	Tmp = Pos;
	Tmp.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	Tmp.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	_FieldSectorIndices[3] = GetSectorIdx(Tmp);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UpdateFieldMap : update field map based on Pos
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RwBool CNtlWorldFieldManager::UpdateFieldMap(RwV3d& Pos)
{
	// assert cur field is valid
	RwInt32 CurFieldIdx = GetFieldIdx(Pos);
	if(CurFieldIdx == -1)
		return FALSE;

	m_NewDatumIdx = CurFieldIdx;

	if(m_eMoved2 != eC && m_eMoved2 != ePORTAL)
		UpdateLODAttr(Pos);

	// Is there transition?
	if(m_OldDatumIdx == m_NewDatumIdx)
	{
		if(m_eMoved2 != eC && m_eMoved2 != ePORTAL)
		{
			return TRUE;
		}
	}

	// update m_eMoved2 out
	UpdateDatumDir();

	// update neighbor fields
	UpdateNeighborFields(Pos);

	// there might be another field coming up
	SetAnotherField();

	// update old datum index
	m_OldDatumIdx = m_NewDatumIdx;

	// send msgs right after another fields
	sNTL_FIELD_PROP NtlFieldProp;
	GetAFieldProp(Pos, NtlFieldProp);
	CNtlPLEventGenerator::IsAnotherFieldChangedWithoutDelay(&NtlFieldProp);

	return TRUE;
}

void CNtlWorldFieldManager::UpdateAnotherField()
{
	if(m_FieldPropVariationRestTime > 0)
	{
		m_FieldPropVariationRestTime -= g_GetElapsedTime();
		if(m_FieldPropVariationRestTime < 0)
		{
			m_FieldPropVariationRestTime = -999.0f;
			m_FieldPropVariationStarting = TRUE;
		}
	}
}

RwBool CNtlWorldFieldManager::GetFieldPropVariationStarting()
{
	if(m_FieldPropVariationStarting)
	{
		m_FieldPropVariationStarting = FALSE;
		return TRUE;
	}

	return FALSE;
}

void CNtlWorldFieldManager::SetAnotherField(RwBool ChangeStraightAway/* = FALSE*/)
{
	if(ChangeStraightAway)
	{
		m_FieldPropVariationRestTime = 0.01f;
	}
	else
	{
		if(m_eMoved2 == ePORTAL || m_eMoved2 == eIDLE)
		{
			m_FieldPropVariationRestTime = 0.01f;
		}
		else
		{
			m_FieldPropVariationRestTime = dFIELD_EFFECT_SWITCHING_TIME;
		}
	}
}

RwBool CNtlWorldFieldManager::IsFieldLoaded(RwInt32 FieldIdx)
{
	if(FieldIdx == -1)
	{
		return FALSE;
	}

	if(CNtlWorldSectorManager::IsSectorLoaded(CNtlWorldSectorManager::GetSectorIdx(m_pFields[FieldIdx].GetSPos())))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#ifdef dNTL_WORLD_TOOL_MODE

void CNtlWorldFieldManager::SaveSwapFile(RwInt32 FieldIdx)
{
	int l, m;
	int SectorIdx;
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	RwV3d SectorSPos;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);
			m_pSectors[SectorIdx].SaveSwapFile();
		}
	}
}

#endif

RwBool CNtlWorldFieldManager::SaveField(RwInt32 FieldIdx)
{
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	if(!IsFieldLoaded(FieldIdx))
	{
		CreateFields(FieldIdx);
		DeleteFields(FieldIdx);
	}
	else
	{
		DeleteFieldIntoFile(FieldIdx);

#ifdef dNTL_WORLD_TOOL_MODE
		{
			SaveSwapFile(FieldIdx);
		}
#endif
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldFieldManager::SaveCurFields()
{
	NTL_FUNCTION("CNtlWorldFieldManager::SaveCurFields");

#ifdef dNTL_WORLD_TOOL_MODE

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");

	for(RwInt32 i = 0; i < 36; ++i)
	{
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_Fields6x6[0][i]);
		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		DeleteFieldIntoFile(m_Fields6x6[1][i]);

#ifdef dNTL_WORLD_TOOL_MODE
		{
			if(m_Fields6x6[1][i] != -1)
			{
				SaveSwapFile(m_Fields6x6[1][i]);
			}
			else
			{
				DBO_TRACE(FALSE, "CNtlWorldFieldManager::SaveCurFields, invalid index");
			}
		}
#endif

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("fields");
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

#endif

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::SaveCurFieldsBeforeExit()
{
	NTL_FUNCTION("CNtlWorldFieldManager::SaveCurFieldsBeforeExit");

	// old source
	// 	if(!GetWorldReady())
	// 	{
	// 		return FALSE;
	// 	}
	// 
	// #ifdef dNTL_WORLD_TOOL_MODE
	// 
	// 	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	// 	_chdir("fields");
	// 
	// 	for(RwUInt32 i = 0; i < 36; ++i)
	// 	{
	// 		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_Fields6x6[1][i]);
	// 		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	// 
	// 		DeleteFieldIntoFile(m_Fields6x6[1][i]);
	// 		//DeleteFieldInMemory(m_Fields6x6[1][i]);
	// 
	// 		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	// 		_chdir("fields");
	// 	}
	// 
	// 	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	// 
	// 	// lightmaps
	// 	if(CNtlWorldLTManager::GetInstance() && GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	// 	{
	// 		CNtlWorldLTManager::GetInstance()->SaveBSP();
	// 		CNtlWorldLTManager::GetInstance()->Destroy();
	// 		CNtlWorldLTManager::DestroyInstance();
	// 	}
	// 
	// 	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	// 	_chdir("fields");
	// 
	// 	for(RwUInt32 i = 0; i < 36; ++i)
	// 	{
	// 		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_Fields6x6[1][i]);
	// 		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	// 
	// 		DeleteFieldInMemory(m_Fields6x6[1][i]);
	// 
	// 		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	// 		_chdir("fields");
	// 	}
	// 
	// 	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	// 
	// 	SaveCurWorldState();
	// #else
	// 	
	// 	for(RwInt32 i = 0; i < 36; ++i)
	// 	{
	// 		DeleteFieldInMemory(m_Fields6x6[1][i]);
	// 	}
	// #endif

	//	NTL_RETURN(TRUE);

	if(!GetWorldReady())
	{
		return FALSE;
	}

#ifdef dNTL_WORLD_TOOL_MODE
	SaveCurWorldState();
#endif

// #ifdef dNTL_WORLD_SCHEDULE_LOADING
// 	// 스케쥴러를 사용 중이라면, 아직 삭제되지 않은 Field가 있을 수 있다. WorldScheduler를 끝까지 돌린다.
// 	while (m_WorldScheduler.Scheduling(1.0f, m_WorldScheduler.GetLastPos()))
// 	{
// 		++m_iCzTestCnt;
// 	}
// #endif

	// FieldManager를 초기화 하는 거나 마찬가지로 m_eMove2를 eC로 변경 WorldEeady 상태를 해제 한다.
	m_eMoved2 = eC;
	for(RwInt32 i = 0; i < 36; ++i)
	{
		DeleteFields(m_Fields6x6[1][i]);
	}

#ifdef dNTL_WORLD_SCHEDULE_LOADING
	// 스케쥴러를 사용 중이라면, 아직 삭제되지 않은 Field가 있을 수 있다. WorldScheduler를 끝까지 돌린다.
	while (m_WorldScheduler.Scheduling(1.0f, m_WorldScheduler.GetLastPos()))
	{
		++m_iCzTestCnt;
	}
#endif


	NTL_RETURN(TRUE);
}

VOID CNtlWorldFieldManager::DeleteFiles4Field(RwInt32 FieldIdx)
{
	if(FieldIdx == -1)
	{
		DBO_TRACE(FALSE, "CNtlWorldFieldManager::DeleteFiles4Field, invalid index");
	}

	// delete WFIF(World Field Information File), WFPF(World Field Property File) and the folder
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", FieldIdx);
	CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);

	_chdir("../");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_rmdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	// delete swap files
	_chdir("../");
	_chdir("swap");

	int l, m;
	int SectorIdx;
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	RwV3d SectorSPos;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.dif", SectorIdx);
			CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sdw", SectorIdx);
			CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);
		}
	}

	_chdir("sl");

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sl", SectorIdx);
			CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);
		}
	}

	_chdir("../");
	_chdir("hho");

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.hho", SectorIdx);
			CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);
		}
	}

	_chdir("../");
	_chdir("occ");

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.ocq", SectorIdx);
			CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);
		}
	}

	_chdir("../");
	_chdir("pe");

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.pe", SectorIdx);
			CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);
		}
	}
	CFileController::DeleteFile("header.pe");
}

RwBool CNtlWorldFieldManager::GetNeighborFields(RwInt32 _IdxCurField, vector<RwInt32>& _vecNeighborFields)
{
	// exceptions
	if(!IsFieldValid(_IdxCurField))
	{
		DBO_TRACE(FALSE, "CNtlWorldFieldManager::GetNeighborFields, invalid field index");
		return FALSE;
	}

	// reserve 8 space for indices
	_vecNeighborFields.reserve(8);

	// calculate neighbor fields
	RwV3d	PosCurFieldS = m_pFields[_IdxCurField].GetSPos();
	RwV3d	PosTmpFieldS;

	// 3---2---1
	// |       | 
	// 4   8   0
	// |       |
	// 5---6---7

	// 0
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.x		-=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 1
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.x		-=	dGET_WORLD_PARAM()->WorldFieldSize;
	PosTmpFieldS.z		+=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 2
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.z		+=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 3
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.x		+=	dGET_WORLD_PARAM()->WorldFieldSize;
	PosTmpFieldS.z		+=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 4
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.x		+=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 5
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.x		+=	dGET_WORLD_PARAM()->WorldFieldSize;
	PosTmpFieldS.z		-=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 6
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.z		-=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 7
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.x		-=	dGET_WORLD_PARAM()->WorldFieldSize;
	PosTmpFieldS.z		-=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	return TRUE;
}

void CNtlWorldFieldManager::SetVisiblePlanet(RwBool bVisible)
{
	m_pPlanetHandler->SetVisible(bVisible);
}

RwV3d CNtlWorldFieldManager::GetFieldRelativeCoord(RwInt32 iSField, RwInt32 iEField)
{
	return (m_pFields[iEField].GetSPos() - m_pFields[iSField].GetSPos());
}

VOID CNtlWorldFieldManager::SetPVSActivation(RwBool _Flag)
{
	RwV3d	CurPos		= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwInt32	CurSecIdx	= GetSectorIdx(CurPos);

	if(_Flag)
	{
		RefreshOldSectorPVS(m_OldSectorIdx4PVS);
		RefreshCurSectorPVS(CurSecIdx);
		m_OldSectorIdx4PVS = CurSecIdx;
	}
	else
	{
		RefreshOldSectorPVS(CurSecIdx);
		RefreshOldSectorPVS(m_OldSectorIdx4PVS);
		m_OldSectorIdx4PVS = CurSecIdx;
	}
}

RwBool CNtlWorldFieldManager::RefreshPVS()
{
	RwV3d	CurPos		= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwInt32	CurSecIdx	= GetSectorIdx(CurPos);

	if(m_OldSectorIdx4PVS != CurSecIdx)
	{
		RefreshOldSectorPVS(m_OldSectorIdx4PVS);
		RefreshCurSectorPVS(CurSecIdx);
		m_OldSectorIdx4PVS = CurSecIdx;
	}

	return TRUE;
}
RwBool CNtlWorldFieldManager::RefreshFieldAll()
{
	for(RwInt32 i = 0; i < 36; ++i)
	{
		if (!RefreshField(m_Fields6x6[1][i]))
		{
			return FALSE;
		}
	}

	return TRUE;
}

RwBool CNtlWorldFieldManager::RefreshField(RwInt32 FieldIdx)
{
	// exception
	if(!IsFieldValid(FieldIdx))
	{
		DBO_TRACE(FALSE, "CNtlWorldFieldManager::RefreshField, invalid index");
		return FALSE;
	}

	// skip if unloaded
	if(IsFieldLoaded(FieldIdx))
	{
		DeleteFieldInMemory(FieldIdx, FALSE);
		CreateFields(FieldIdx);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldFieldManager::DeleteFieldThenCreateNew(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::DeleteFieldThenCreateNew");

#ifdef dNTL_WORLD_TOOL_MODE

	if(!IsFieldLoaded(FieldIdx))
	{
		DeleteFiles4Field(FieldIdx);
		CreateFields(FieldIdx);
		DeleteFields(FieldIdx);
	}
	else
	{
		DeleteFieldInMemory(FieldIdx);
		DeleteFiles4Field(FieldIdx);
		CreateFields(FieldIdx);
		DeleteFields(FieldIdx);
		CreateFields(FieldIdx);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(TRUE);

#endif

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::DeleteFieldIntoFile(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::DeleteFieldIntoFile");

	if(FieldIdx == -1)
	{
		NTL_RETURN(FALSE);
	}

	int l, m;
	int SectorIdx;
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	RwV3d SectorSPos;

	// World Field Information File
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);

	// 	RwChar cTempFileName[64];
	// 	sprintf_s(cTempFileName, 64, "wfif%d.temp", FieldIdx);
	// 
	// 	if((_access(dGET_WORLD_PARAM()->WorldChar64Buf, 0)) != -1)
	// 	{
	// 		SetFileAttributes(dGET_WORLD_PARAM()->WorldChar64Buf, FILE_ATTRIBUTE_ARCHIVE);
	// 	}

	FILE *pFile;
	::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb");

	DBO_ASSERT(pFile, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");

	if(pFile)
	{
		for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x = (RwReal)m;
				SectorSPos.z = (RwReal)l;
				SectorIdx = GetSectorIdx(SectorSPos);
				CNtlWorldSectorManager::DeleteIntoFile(SectorIdx, pFile);
			}
		}

		::fclose(pFile);
	}

	// 	if((_access(dGET_WORLD_PARAM()->WorldChar64Buf, 0)) != -1)
	// 	{
	// 		SetFileAttributes(dGET_WORLD_PARAM()->WorldChar64Buf, FILE_ATTRIBUTE_ARCHIVE);
	// 	}

	m_pFields[FieldIdx].DeleteFieldIntoFile(FieldIdx);

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::DeleteFieldInMemory(RwInt32 FieldIdx, RwBool SaveSwapInToolMode/* = TRUE*/)
{
	NTL_FUNCTION("CNtlWorldFieldManager::DeleteFieldInMemory");

	if(FieldIdx == -1)
		NTL_RETURN(FALSE);

	int l, m;
	int SectorIdx;
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	RwV3d SectorSPos;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			CNtlWorldSectorManager::DeleteInMemory(SectorIdx, SaveSwapInToolMode);
		}
	}

	m_pFields[FieldIdx].DeleteFieldInMemory(FieldIdx);

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::DeleteFields(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::DeleteFields");

	if(FieldIdx == -1)
		NTL_RETURN(FALSE);

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);

#ifdef dNTL_WORLD_TOOL_MODE

	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	// just make new files or overwrite old files at first then,
	DeleteFieldIntoFile(FieldIdx);

	// free in memory
	DeleteFieldInMemory(FieldIdx);

#else

#ifdef dNTL_WORLD_SCHEDULE_LOADING
	if (m_eMoved2 != eC && m_eMoved2 != ePORTAL && m_eMoved2 != eIDLE)
	{
		m_WorldScheduler.DeleteField(FieldIdx);
	}	
	else
	{
		m_WorldScheduler.RemoveSchedule(FieldIdx);
#endif
		// change dir.
		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		// only freeing in memory must be available
		DeleteFieldInMemory(FieldIdx);
#ifdef dNTL_WORLD_SCHEDULE_LOADING
	}
#endif

#endif

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::CreateFieldInMemory(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreateFieldInMemory");

	if(FieldIdx == -1)
		NTL_RETURN(FALSE);

	m_pFields[FieldIdx].CreateFieldInMemory(FieldIdx);

	int SectorIdx;
	int l, m;
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	RwV3d SectorSPos;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			CNtlWorldSectorManager::CreateInMemory(SectorIdx);
		}
	}

//  	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
//  	_chdir("fields");

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::CreateFieldFromFile(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreateFieldFromFile");

	if(FieldIdx == -1)
		NTL_RETURN(FALSE);

	// If there wasn't a Prop file just create in memory
	if(!m_pFields[FieldIdx].CreateFieldFromFile(FieldIdx))
	{
		m_pFields[FieldIdx].CreateFieldInMemory(FieldIdx);
	}	

	int SectorIdx;
	int l, m;
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	RwV3d SectorSPos;

	//-------------------------------------------------------------------
	// 2007.03.23 (fluorscope)
	// Pack file function added

	FILE *pFile = NULL;
	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
	{
		static RwChar chPackPatch[NTL_MAX_DIR_PATH];

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);

		strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "fields");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

		SPackResFileData sPackFileData;
		RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(chPackPatch, sPackFileData);
		if(bPack)
		{
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			::fopen_s(&pFile, sPackFileData.strPackFileName.c_str(), "rb");
			if(pFile)
			{
				fseek(pFile, sPackFileData.uiOffset, SEEK_SET);
			}
		}
	}
	else
	{
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("fields");
		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}

	DBO_ASSERT(pFile, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
	//DBO_WARNING_MESSAGE("Open file: "<< dGET_WORLD_PARAM()->WorldChar64Buf);
	if(pFile)
	{
		for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x = (RwReal)m;
				SectorSPos.z = (RwReal)l;
				SectorIdx = GetSectorIdx(SectorSPos);

				CNtlWorldSectorManager::CreateFromFile(m_pFields[FieldIdx].GetTexAttr(), SectorIdx, pFile);
			}
		}

		fclose(pFile);
	}

	NTL_RETURN(TRUE);
	/*
	if(FieldIdx == -1)
	{
	return FALSE;
	}

	int		SectorIdx;
	int		l, m;
	RwV3d	SectorSPos;
	RwV3d	SPos = m_pFields[FieldIdx].GetSPos();
	BYTE*   pPosCurFileMem;

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);

	m_FFMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
	pPosCurFileMem = m_FFMem.GetDat();

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
	for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
	{
	SectorSPos.x = (RwReal)m;
	SectorSPos.z = (RwReal)l;
	SectorIdx = GetSectorIdx(SectorSPos);

	CNtlWorldSectorManager::CreateFromFile(m_pFields[FieldIdx].GetTexAttr(), SectorIdx, pPosCurFileMem);
	pPosCurFileMem = CNtlWorldFileMemAccessor::Skip_Sector(pPosCurFileMem);
	}
	}

	// If there wasn't a Prop file just create in memory
	if(!m_pFields[FieldIdx].CreateFieldFromFile(FieldIdx))
	{
	m_pFields[FieldIdx].CreateFieldInMemory(FieldIdx);
	}

	return TRUE;*/
}

RwBool CNtlWorldFieldManager::CreateFields(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreateFields");

	if(FieldIdx == -1)
		NTL_RETURN(FALSE);

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);

#ifdef dNTL_WORLD_TOOL_MODE

	if(_chdir(dGET_WORLD_PARAM()->WorldChar64Buf) == -1)
	{
		// create folders
		_mkdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		// there are no files, create a field in memory
		CreateFieldInMemory(FieldIdx);
	}
	else
	{
		// there are files existed, load from the files
		CreateFieldFromFile(FieldIdx);
	}

#else

	// change dir.
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	// there must be files existed, load from the files

#ifdef dNTL_WORLD_SCHEDULE_LOADING
	if (m_eMoved2 != eC && m_eMoved2 != ePORTAL && m_eMoved2 != eIDLE)
	{
		m_WorldScheduler.CreateField(FieldIdx);
	}
	else
	{
		m_WorldScheduler.RemoveSchedule(FieldIdx);
#endif
		CreateFieldFromFile(FieldIdx);
#ifdef dNTL_WORLD_SCHEDULE_LOADING
	}
#endif

#endif

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(TRUE);
}

RwReal CNtlWorldFieldManager::GetWorldSectorHeight(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetWorldSectorHeight");

	RwInt32 CurSectorIdx = GetSectorIdx(Pos);
	if(CurSectorIdx == -1)
		NTL_RETURN(-999.0f);

	NTL_RETURN(RpNtlWorldSectorGetHeight(m_pSectors[CurSectorIdx].m_pWorldSector, &Pos));
}

#ifdef dNTL_WORLD_TOOL_MODE

void CNtlWorldFieldManager::SetWorldSectorHeightInRectangle(RwV3d& PosS, RwV3d& PosE, RwReal Height)
{
	sTARGET_VERT_SL_INFO TargetVertSLInfo;
	vector<sTARGET_VERT_SL_INFO> vecTargetVertSLInfo;

	if(PosS.x == PosE.x && PosS.z == PosE.z)
		return;

	RwV2d Min, Max;
	if(PosS.x >= PosE.x)
	{
		Max.x = PosS.x;
		Min.x = PosE.x;
	}
	else
	{
		Max.x = PosE.x;
		Min.x = PosS.x;
	}

	if(PosS.z >= PosE.z)
	{
		Max.y = PosS.z;
		Min.y = PosE.z;
	}
	else
	{
		Max.y = PosE.z;
		Min.y = PosS.z;
	}

	for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
	{
		RpAtomic	*pCurAtomic = m_pSectors[g_vecVisibleSectors[j]].m_pAtomic;
		RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
		RwRGBA		*pPrelights = RpGeometryGetPreLightColors(pCurAtomic->geometry);
		RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
		RwV3d		Center		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;

		RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKVERTICES);

		for(int i = 0; i < VertNum; ++i)
		{
			float Distance = sqrtf(	(pVList[i].x - Center.x) * (pVList[i].x - Center.x) +
				(pVList[i].z - Center.z) * (pVList[i].z - Center.z));

			if(	(pVList[i].x >= Min.x && pVList[i].x <= Max.x) &&
				(pVList[i].z >= Min.y && pVList[i].z <= Max.y))
			{
				pVList[i].y	= Height;
			}

			// set slope lighting
			TargetVertSLInfo.pNtlWorldSector	= &m_pSectors[g_vecVisibleSectors[j]];
			TargetVertSLInfo.IdxVert			= i;
			vecTargetVertSLInfo.push_back(TargetVertSLInfo);
		}

		RpGeometryUnlock(pCurAtomic->geometry);

		RwSphere sphereBounding;
		RpMorphTargetCalcBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
		RpMorphTargetSetBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
	}

	OnSetSlopeLighting(vecTargetVertSLInfo);
}

#endif

// Menu index; 0:create, 1:delete, 2:erase
void CNtlWorldFieldManager::UpdateCurFieldTexLyr(RwInt32 IdxLyr, RwInt32 IdxMenu)
{
	int SectorIdx;
	int l, m;
	RwV3d SPos = m_pFields[m_IdxCurField].GetSPos();
	RwV3d SectorSPos;

	// set current activated texture layer index number
	m_pFields[m_IdxCurField].GetTexAttr().IdxCurLT = IdxLyr;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			CNtlWorldSectorManager::UpdateTexLyr(m_pFields[m_IdxCurField].GetTexAttr(), SectorIdx, IdxLyr, IdxMenu);
		}
	}
}

void CNtlWorldFieldManager::UpdateCurFieldTexMat(RwInt32 IdxLyr)
{
	int SectorIdx;
	int l, m;
	RwV3d SPos = m_pFields[m_IdxCurField].GetSPos();
	RwV3d SectorSPos;

	// set current activated texture layer index number
	m_pFields[m_IdxCurField].GetTexAttr().IdxCurLT = IdxLyr;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			// watch out for (IdxLyr - 1) this is 'cos base layer doesn't need a texture matrix so it wasn't even created.
			CNtlWorldSectorManager::UpdateTexMat(m_pFields[m_IdxCurField].GetTexAttr(), SectorIdx, (IdxLyr - 1));
		}
	}
}

RwBool CNtlWorldFieldManager::SetPortalPosition(RwV3d& Pos, RwBool _RUFInitFlag/* = TRUE*/)
{
	m_eMoved2 = ePORTAL;

	if(_RUFInitFlag)
		RwFrameTranslate(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera), &Pos, rwCOMBINEREPLACE);
	//else 
	//	RwFrameTranslate(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera), &Pos, rwCOMBINEPOSTCONCAT);

	return TRUE;
}

void CNtlWorldFieldManager::UpdateCurMapName(RwV3d& Pos)
{
	sNTL_FIELD_PROP NtlFieldProp;
	GetAFieldProp(Pos, NtlFieldProp);

	if(!_stricmp(NtlFieldProp._Name, "null") || !_stricmp(NtlFieldProp._Name, ""))
	{
		m_CurMapNameIdx = 0xffffffff;
		return;
	}
	else
	{
		TBLIDX CurTblIdx = static_cast<TBLIDX>(atoi(NtlFieldProp._Name));

		if(m_CurMapNameIdx == 0xffffffff)
		{
			m_CurMapNameIdx = CurTblIdx;
			CNtlPLEventGenerator::OnMapNameChange(&m_CurMapNameIdx);
		}
		else
		{
			if(m_CurMapNameIdx != CurTblIdx)
			{
				m_CurMapNameIdx = CurTblIdx;
				CNtlPLEventGenerator::OnMapNameChange(&m_CurMapNameIdx);
			}
		}
	}
}

void CNtlWorldFieldManager::UpdateMsg(RwV3d& Pos)
{
	// world field switching effect
	if(GetFieldPropVariationStarting())
	{
		// Update current map name
		UpdateCurMapName(Pos);

		sNTL_FIELD_PROP NtlFieldProp;
		GetAFieldProp(Pos, NtlFieldProp);
		CNtlPLEventGenerator::IsAnotherFieldChanged(&NtlFieldProp);
	}
}

RwReal CNtlWorldFieldManager::GetMonoFactor()
{
	return m_pBloomEntity->GetMonoFactor();
}

VOID CNtlWorldFieldManager::RefreshOldSectorPVS(RwInt32 _SectorIdx)
{
	RwV3d Pos;
	CNtlMath::MathRwV3dAssign(&Pos, m_pSectors[_SectorIdx].DatumPoint.x, m_pSectors[_SectorIdx].DatumPoint.y, m_pSectors[_SectorIdx].DatumPoint.z);

	RwInt32 CellCnt = dPVS_SECTOR_EXTENT_CNT + dPVS_SECTOR_EXTENT_CNT + 1;

	RwV3d SPos;
	SPos.x = Pos.x - (dPVS_SECTOR_EXTENT_CNT * dGET_WORLD_PARAM()->WorldSectorSize);
	SPos.z = Pos.z - (dPVS_SECTOR_EXTENT_CNT * dGET_WORLD_PARAM()->WorldSectorSize);

	RwInt32 CurSectorIdx;
	RwV3d	CurSectorPos;

	for(RwInt32 i = 0; i < CellCnt; ++i)
	{
		for(RwInt32 j = 0; j < CellCnt; ++j)
		{
			CurSectorPos.x	= SPos.x + j * dGET_WORLD_PARAM()->WorldSectorSize;
			CurSectorPos.z	= SPos.z + i * dGET_WORLD_PARAM()->WorldSectorSize;
			CurSectorIdx	= GetSectorIdx(CurSectorPos);

			if(CurSectorIdx == -1)
			{
				continue;
			}

			m_pSectors[CurSectorIdx].m_pNtlWorldSectorPVS->SetVisibility(CurSectorIdx, TRUE);
		}
	}
}

VOID CNtlWorldFieldManager::RefreshCurSectorPVS(RwInt32 _SectorIdx)
{
	if(!m_pSectors[_SectorIdx].m_pNtlWorldSectorPVS->GetEnable())
	{
		return;
	}

	RwV3d Pos;
	CNtlMath::MathRwV3dAssign(&Pos, m_pSectors[_SectorIdx].DatumPoint.x, m_pSectors[_SectorIdx].DatumPoint.y, m_pSectors[_SectorIdx].DatumPoint.z);

	RwInt32 CellCnt = dPVS_SECTOR_EXTENT_CNT + dPVS_SECTOR_EXTENT_CNT + 1;

	RwV3d SPos;
	SPos.x = Pos.x - (dPVS_SECTOR_EXTENT_CNT * dGET_WORLD_PARAM()->WorldSectorSize);
	SPos.z = Pos.z - (dPVS_SECTOR_EXTENT_CNT * dGET_WORLD_PARAM()->WorldSectorSize);

	RwInt32 CurSectorIdx;
	RwV3d	CurSectorPos;

	for(RwInt32 i = 0; i < CellCnt; ++i)
	{
		for(RwInt32 j = 0; j < CellCnt; ++j)
		{
			CurSectorPos.x	= SPos.x + j * dGET_WORLD_PARAM()->WorldSectorSize;
			CurSectorPos.z	= SPos.z + i * dGET_WORLD_PARAM()->WorldSectorSize;
			CurSectorIdx	= GetSectorIdx(CurSectorPos);

			// exception handling
			if(CurSectorIdx == -1)
			{
				continue;
			}

			// self process
			if(CurSectorIdx == _SectorIdx)
			{
				m_pSectors[_SectorIdx].m_pNtlWorldSectorPVS->SetVisibility(_SectorIdx, TRUE);
				continue;
			}

			// others
			m_pSectors[CurSectorIdx].m_pNtlWorldSectorPVS->SetVisibility(CurSectorIdx, m_pSectors[_SectorIdx].m_pNtlWorldSectorPVS->GetVisibilityNeighbor(i * CellCnt + j));
		}
	}
}

void CNtlWorldFieldManager::UpdateSectors()
{
	// update visble sectors
	vector<RwInt32> vecVisibleSectors;
	::RpNtlWorldGetVisibleSectors(&vecVisibleSectors);

	for(unsigned int i = 0; i < vecVisibleSectors.size(); ++i)
	{
		m_pSectors[vecVisibleSectors[i]].Update();
	}

	// refresh PVS
	if(CNtlPLGlobal::m_UseTerrainPVSMode && !CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PVS_LOCKING))
	{
		RwV3d	CurPos		= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
		RwInt32	CurSecIdx	= GetSectorIdx(CurPos);

		if(m_OldSectorIdx4PVS == -1)
		{
			m_OldSectorIdx4PVS = CurSecIdx;
			RefreshCurSectorPVS(m_OldSectorIdx4PVS);
			return;
		}

		if(m_OldSectorIdx4PVS != CurSecIdx)
		{
			RefreshOldSectorPVS(m_OldSectorIdx4PVS);
			RefreshCurSectorPVS(CurSecIdx);
			m_OldSectorIdx4PVS = CurSecIdx;
		}
	}
}

RpWorldSector* GetVisibleSectorsOutdoor(RpWorldSector *pWorldSector, void *data)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);

	if(!pNtlSector)
	{
		return pWorldSector;
	}

	D3DXVECTOR3 Min, Max, RayOri, RayDir;
	Min.x = pWorldSector->boundingBox.inf.x;
	Min.y = pWorldSector->boundingBox.inf.y;
	Min.z = pWorldSector->boundingBox.inf.z;
	Max.x = pWorldSector->boundingBox.sup.x;
	Max.y = pWorldSector->boundingBox.sup.y;
	Max.z = pWorldSector->boundingBox.sup.z;

	RwV3d CenterCoor;
	CenterCoor.x = (Min.x + Max.x) / 2;
	CenterCoor.z = (Min.z + Max.z) / 2;

	RwInt32 WidthNum, DepthNum;
	RwReal XBasedOn0 = CenterCoor.x + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal ZBasedOn0 = CenterCoor.z + dGET_WORLD_PARAM()->WorldSizeHalf;
	WidthNum = (int)(XBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize);
	DepthNum = (int)(ZBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize); 
	RwInt32 RetIdx = WidthNum + (DepthNum * dGET_WORLD_PARAM()->WorldSectorNum);

	g_vecVisibleSectors.push_back(RetIdx);

	return pWorldSector;
}

void CNtlWorldFieldManager::UpdateVisibleSectors()
{
	g_vecVisibleSectors.clear();
	g_vecVisibleSectors.reserve(20);
	RwCameraForAllSectorsInFrustum(CNtlPLGlobal::m_RwCamera, GetVisibleSectorsOutdoor, NULL);
}

RwBool CNtlWorldFieldManager::GetWorldDecal(RwV3d& vPosition, RwV3d& vSize, RwInt32 nPolyMaxCount, RwInt32& nRenderVertexCount, RwReal& fUpdateVertextRadius, RwIm3DVertex* pVertices, RwReal OffsetY)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetWorldDecal");

	RwInt32 iSectorIdx = GetSectorIdx(vPosition);
	if (iSectorIdx != -1 && m_pSectors[iSectorIdx].m_IsCulled)
	{
		NTL_RETURN(FALSE);
	}

	RwReal fMax = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE);
	RwReal fMin = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE);

	if(vPosition.x < fMin || vPosition.x > fMax || vPosition.z < fMin || vPosition.z > fMax)
	{
		NTL_RETURN(FALSE);
	}

	RwBBox decalBox;
	decalBox.sup.x = vPosition.x + (vSize.x * 0.5f);
	decalBox.inf.x = vPosition.x - (vSize.x * 0.5f);
	decalBox.sup.z = vPosition.z + (vSize.z * 0.5f);
	decalBox.inf.z = vPosition.z - (vSize.z * 0.5f);

	decalBox.inf.x = static_cast<RwReal>((static_cast<RwInt32>(decalBox.inf.x) / dGET_WORLD_PARAM()->WorldSectorTileSize - 1) * dGET_WORLD_PARAM()->WorldSectorTileSize);
	decalBox.inf.z = static_cast<RwReal>((static_cast<RwInt32>(decalBox.inf.z) / dGET_WORLD_PARAM()->WorldSectorTileSize - 1) * dGET_WORLD_PARAM()->WorldSectorTileSize);
	decalBox.sup.x = static_cast<RwReal>((static_cast<RwInt32>(decalBox.sup.x) / dGET_WORLD_PARAM()->WorldSectorTileSize + 1) * dGET_WORLD_PARAM()->WorldSectorTileSize);
	decalBox.sup.z = static_cast<RwReal>((static_cast<RwInt32>(decalBox.sup.z) / dGET_WORLD_PARAM()->WorldSectorTileSize + 1) * dGET_WORLD_PARAM()->WorldSectorTileSize);

	for (RwReal fZ = decalBox.inf.z; fZ < decalBox.sup.z; fZ += dGET_WORLD_PARAM()->WorldSectorTileSize)
	{
		if(nRenderVertexCount + 2 >= nPolyMaxCount)
		{
			nRenderVertexCount += 3;
			break;
		}
		for (RwReal fX = decalBox.inf.x; fX < decalBox.sup.x; fX += dGET_WORLD_PARAM()->WorldSectorTileSize)
		{				
			RwV3d vPos;
			vPos.x = fX;
			vPos.z = fZ;
			vPos.y = GetWorldSectorHeight(vPos) + OffsetY;
			RwIm3DVertexSetPos(&pVertices[nRenderVertexCount], vPos.x, vPos.y, vPos.z);
			++nRenderVertexCount;

			vPos.x = fX;
			vPos.z = fZ + dGET_WORLD_PARAM()->WorldSectorTileSize;
			vPos.y = GetWorldSectorHeight(vPos) + OffsetY;
			RwIm3DVertexSetPos(&pVertices[nRenderVertexCount], vPos.x, vPos.y, vPos.z);
			++nRenderVertexCount;

			vPos.x = fX + dGET_WORLD_PARAM()->WorldSectorTileSize;
			vPos.z = fZ;
			vPos.y = GetWorldSectorHeight(vPos) + OffsetY;
			RwIm3DVertexSetPos(&pVertices[nRenderVertexCount], vPos.x, vPos.y, vPos.z);
			++nRenderVertexCount;

			if(nRenderVertexCount + 2 >= nPolyMaxCount)
			{
				nRenderVertexCount += 3;
				break;
			}

			vPos.x = fX;
			vPos.z = fZ + dGET_WORLD_PARAM()->WorldSectorTileSize;
			vPos.y = GetWorldSectorHeight(vPos) + OffsetY;
			RwIm3DVertexSetPos(&pVertices[nRenderVertexCount], vPos.x, vPos.y, vPos.z);
			++nRenderVertexCount;

			vPos.x = fX + dGET_WORLD_PARAM()->WorldSectorTileSize;
			vPos.z = fZ + dGET_WORLD_PARAM()->WorldSectorTileSize;
			vPos.y = GetWorldSectorHeight(vPos) + OffsetY;
			RwIm3DVertexSetPos(&pVertices[nRenderVertexCount], vPos.x, vPos.y, vPos.z);
			++nRenderVertexCount;

			vPos.x = fX + dGET_WORLD_PARAM()->WorldSectorTileSize;
			vPos.z = fZ;
			vPos.y = GetWorldSectorHeight(vPos) + OffsetY;
			RwIm3DVertexSetPos(&pVertices[nRenderVertexCount], vPos.x, vPos.y, vPos.z);
			++nRenderVertexCount;
		}
	}

	fUpdateVertextRadius = sqrtf(vSize.x * vSize.x + vSize.z * vSize.z) * 0.5f;

	// 	RwReal		Max			= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE);
	// 	RwReal		Min			= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE);
	// 
	// 	RwInt32		MidX		= static_cast<RwInt32>(vPosition.x / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 	RwInt32		MidZ		= static_cast<RwInt32>(vPosition.z / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 
	// 	// 1를 더해주는 이유는 업데이트를 덜 하기 위한 방법이다.
	// 	RwInt32		TileNumHalfX= 1 + (static_cast<RwInt32>(vSize.x * 0.5f) + (dGET_WORLD_PARAM()->WorldSectorTileSize + static_cast<RwInt32>(vSize.x * 0.5f) % dGET_WORLD_PARAM()->WorldSectorTileSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 	RwInt32		TileNumHalfZ= 1 + (static_cast<RwInt32>(vSize.z * 0.5f) + (dGET_WORLD_PARAM()->WorldSectorTileSize + static_cast<RwInt32>(vSize.z * 0.5f) % dGET_WORLD_PARAM()->WorldSectorTileSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 
	// 	// 1를 더해주는 이유는 중앙 타일은 연산에서 빼기 위한 꽁수다.
	// 	RwInt32		TileNumX	= TileNumHalfX * 2 + 1;
	// 	RwInt32		TileNumZ    = TileNumHalfZ * 2 + 1;
	// 
	// 	RwInt32		TileNum		= TileNumX * TileNumZ;
	// 
	// 	RwInt32		PolyNum		= TileNum * 2;
	// 	RwInt32		VertNum		= PolyNum * 3;
	// 
	// 	RwInt32		VertCnt		= 0;
	// 	RwInt32		i, j;
	// 	RwV3d		SPos, CPos;
	// 
	// 	if(VertNum > nPolyMaxCount)
	// 	{
	// 		NTL_RETURN(FALSE);
	// 	}
	// 
	// 	if(MidX < Min || MidX > Max || MidZ < Min || MidZ > Max)
	// 	{
	// 		NTL_RETURN(FALSE);
	// 	}
	// 
	// 	RwInt32 iSectorIdx = GetSectorIdx(vPosition);
	// 	if (iSectorIdx != -1 && m_pSectors[iSectorIdx].m_IsCulled)
	// 	{
	// 		NTL_RETURN(TRUE);
	// 	}
	// 
	// 	SPos.x = static_cast<RwReal>(MidX - ((TileNumHalfX) * dGET_WORLD_PARAM()->WorldSectorTileSize));
	// 	SPos.z = static_cast<RwReal>(MidZ - ((TileNumHalfZ) * dGET_WORLD_PARAM()->WorldSectorTileSize));
	// 
	// 	for(i = 0; i < TileNumX; ++i)
	// 	{
	// 		for(j = 0; j < TileNumZ; ++j)
	// 		{
	// 			CPos.x = SPos.x + j * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.z = SPos.z + i * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.y = GetWorldSectorHeight(CPos) + OffsetY;			
	// 			RwIm3DVertexSetPos(&pVertices[VertCnt], CPos.x, CPos.y, CPos.z);
	// 			++VertCnt;
	// 
	// 			CPos.x = SPos.x + j * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.z = SPos.z + i * dGET_WORLD_PARAM()->WorldSectorTileSize + dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.y = GetWorldSectorHeight(CPos) + OffsetY;			
	// 			RwIm3DVertexSetPos(&pVertices[VertCnt], CPos.x, CPos.y, CPos.z);
	// 			++VertCnt;
	// 
	// 			CPos.x = SPos.x + j * dGET_WORLD_PARAM()->WorldSectorTileSize + dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.z = SPos.z + i * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.y = GetWorldSectorHeight(CPos) + OffsetY;			
	// 			RwIm3DVertexSetPos(&pVertices[VertCnt], CPos.x, CPos.y, CPos.z);
	// 			++VertCnt;
	// 
	// 			CPos.x = SPos.x + j * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.z = SPos.z + i * dGET_WORLD_PARAM()->WorldSectorTileSize + dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.y = GetWorldSectorHeight(CPos) + OffsetY;			
	// 			RwIm3DVertexSetPos(&pVertices[VertCnt], CPos.x, CPos.y, CPos.z);
	// 			++VertCnt;
	// 
	// 			CPos.x = SPos.x + j * dGET_WORLD_PARAM()->WorldSectorTileSize + dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.z = SPos.z + i * dGET_WORLD_PARAM()->WorldSectorTileSize + dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.y = GetWorldSectorHeight(CPos) + OffsetY;			
	// 			RwIm3DVertexSetPos(&pVertices[VertCnt], CPos.x, CPos.y, CPos.z);
	// 			++VertCnt;
	// 
	// 			CPos.x = SPos.x + j * dGET_WORLD_PARAM()->WorldSectorTileSize + dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.z = SPos.z + i * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.y = GetWorldSectorHeight(CPos) + OffsetY;			
	// 			RwIm3DVertexSetPos(&pVertices[VertCnt], CPos.x, CPos.y, CPos.z);
	// 			++VertCnt;
	// 		}
	// 	}
	// 
	// 	nRenderVertexCount	= VertCnt;
	// 	fUpdateVertextRadius= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize * (TileNumHalfX > TileNumHalfZ ? TileNumHalfX : TileNumHalfZ));

	if (nRenderVertexCount > nPolyMaxCount)
	{
		NTL_RETURN(FALSE);
	}
	NTL_RETURN(TRUE);
}

void CNtlWorldFieldManager::UpdateShoreLine(RwReal _ElapsedTime)
{
	CNtlPLShoreLineProp* pProp = static_cast<CNtlPLShoreLineProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE));

	pProp->Update(_ElapsedTime);
}

VOID CNtlWorldFieldManager::GetVisible2DAABB(C2DAABB& _2DAABB)
{
	RwV2d Min, Max;

	for(RwInt32 i = 0; i < 36; ++i)
	{
		if(m_Fields6x6[1][i] != -1)
		{
			Min.x = m_pFields[m_Fields6x6[1][i]].GetSPos().x;
			Min.y = m_pFields[m_Fields6x6[1][i]].GetSPos().z;
			Max.x = Min.x + dGET_WORLD_PARAM()->WorldFieldSize;
			Max.y = Min.y + dGET_WORLD_PARAM()->WorldFieldSize;

			_2DAABB.SetNewMinMax(Min, Max);
		}
	}
}

RwBool CNtlWorldFieldManager::OnDragonSkyAppearence(RwBool _FlgAppeared)
{
	if(_FlgAppeared)
	{
		if(!m_pDragonSkyEntity)
		{
			return FALSE;
		}
		else
		{
			static_cast<CNtlPLSkyDragon*>(m_pDragonSkyEntity)->FadeIn();
		}
	}
	else
	{
		if(!m_pDragonSkyEntity)
		{
			return FALSE;
		}
		else
		{
			static_cast<CNtlPLSkyDragon*>(m_pDragonSkyEntity)->FadeOut();
		}
	}

	return TRUE;
}

RwInt32 CNtlWorldFieldManager::GetSelSectorIdxCntInField(RwInt32 _SectorIdx, RwInt32 _FieldIdx)
{
	RwV3d	SectorSPos;
	RwV3d	SPos	= m_pFields[_FieldIdx].GetSPos();
	RwInt32 Ret		= 0;

	for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x	= (RwReal)m;
			SectorSPos.z	= (RwReal)l;

			if(_SectorIdx == GetSectorIdx(SectorSPos))
			{
				return Ret;
			}

			++Ret;
		}
	}	

	return -999;
}

RwBool CNtlWorldFieldManager::GetWorldShadowColor(RwV3d& Pos, RwRGBA* pOutColor)
{
	RwInt32 iSectorIdx = GetSectorIdx(Pos);
	if (iSectorIdx <= -1)
	{
		return FALSE;
	}

	if (!m_pSectors[iSectorIdx].m_pNtlWorldShadow)
	{
		return FALSE;
	}

	::CopyMemory(pOutColor, &m_pSectors[iSectorIdx].m_pNtlWorldShadow->m_rgbaShadow, sizeof(RwRGBA));

	return TRUE;
}

#define NTL_SHADOW_GRAY_MIN	175
#define NTL_SHADOW_GRAY_MAX	200
RwBool CNtlWorldFieldManager::GetWorldLightColor(RwV3d& Pos, RwRGBA* pOutColor, RwBool bCalcShadow)
{
	RwInt32 iSectorIdx = GetSectorIdx(Pos);
	if (iSectorIdx <= -1)
	{
		return FALSE;
	}
	vector<RwInt32> vecNeighborSectors;
	GetNeighborSectors(iSectorIdx, vecNeighborSectors);
	vecNeighborSectors.push_back(iSectorIdx);

	// 3---2---1
	// |       | 
	// 4   8   0
	// |       |
	// 5---6---7

	for (int i = 0; i < 9; ++i)
	{
		if (vecNeighborSectors.at(i) == -1)
		{
			continue;
		}

		CNtlWorldSector* pSector = &m_pSectors[vecNeighborSectors.at(i)];

		RwV2d ptCheckPos;
		ptCheckPos.x = pSector->DatumPoint.x;
		ptCheckPos.y = pSector->DatumPoint.z;

		RwV2d ptPos;
		ptPos.x = Pos.x;
		ptPos.y = Pos.z;

		RwV2d ptDist;
		RwV2dSub(&ptDist, &ptCheckPos, &ptPos);

		// 여기를 통과하지 못하는 섹터는 -1 값으로 변경한다.
		if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD && !pSector->m_pAtomic)
		{
			vecNeighborSectors.at(i) = -1;
			continue;
		}
		if (RwV2dLength(&ptDist) > dGET_WORLD_PARAM()->WorldSectorSize)
		{
			vecNeighborSectors.at(i) = -1;
			continue;
		}		
	}

	// 위에서 재구성된 NeighborSectors만 연산된다.
	RwRGBAReal	clrfResult;
	RwBool		bResult = FALSE;

	// Box 검사.
	for (int i = 0; i < 9; ++i)
	{
		if (vecNeighborSectors.at(i) == -1)
		{
			continue;
		}



		CNtlWorldSector* pSector = &m_pSectors[vecNeighborSectors.at(i)];

		for (int j = 0; j < (int)pSector->m_vecNtlPLEntityWorldLight_Box.size(); ++j)
		{		
			CNtlPLWorldLight_Base* pWorldLight = (CNtlPLWorldLight_Base*)pSector->m_vecNtlPLEntityWorldLight_Box.at(j);
			if (pWorldLight->GetWorldLightColorf(&Pos, &clrfResult, NULL))
			{
				bResult = TRUE;
				break;
			}
		}
		if (bResult)
		{
			break;
		}
	}

	// Box에서 체크되지 않았다면 구를 검사 한다.
	if (!bResult)
	{
		// WorldLight
		RwRGBAReal	clrfTemp;
		RwRGBAReal*	pclrfTemp = const_cast<RwRGBAReal*>(CNtlFieldColorManager::GetInstance()->GetColorf());

		clrfResult = *pclrfTemp;
		for (int i = 0; i < 9; ++i)
		{
			if (vecNeighborSectors.at(i) == -1)
			{
				continue;
			}

			CNtlWorldSector* pSector = &m_pSectors[vecNeighborSectors.at(i)];

			for (int j = 0; j < (int)pSector->m_vecNtlPLEntityWorldLight_Sphere.size(); ++j)
			{		
				CNtlPLWorldLight_Base* pWorldLight = (CNtlPLWorldLight_Base*)pSector->m_vecNtlPLEntityWorldLight_Sphere.at(j);
				if (pWorldLight->GetWorldLightColorf(&Pos, &clrfResult, pclrfTemp))
				{
					clrfTemp	= clrfResult;
					pclrfTemp	= &clrfTemp;					
				}
			}
		}	

		for (int i = 0; i < 9; ++i)
		{
			if (vecNeighborSectors.at(i) == -1)
			{
				continue;
			}

			CNtlWorldSector* pSector = &m_pSectors[vecNeighborSectors.at(i)];

			for (int j = 0; j < (int)pSector->m_vecNtlPLEntityLightObject.size(); ++j)
			{		
				CNtlPLLightObject* pLightObject = (CNtlPLLightObject*)pSector->m_vecNtlPLEntityLightObject.at(j);
				if (pLightObject->GetWorldLightColorf(&Pos, &clrfResult, pclrfTemp))
				{
					clrfTemp	= clrfResult;
					pclrfTemp	= &clrfTemp;					
				}
			}

			for (int j = 0; j < (int)pSector->m_vecNtlPLEntityOrnament.size(); ++j)
			{
				CNtlPLObject* pPLObject = ((CNtlPLObject*)pSector->m_vecNtlPLEntityOrnament.at(j));

				for (ListAttachAttr::iterator it = pPLObject->GetAttachAttrList()->begin(); it != pPLObject->GetAttachAttrList()->end(); ++it)
				{
					SPLAttachAttr* pPLAttachAttr = *it;
					if (pPLAttachAttr->pPLEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
					{
						CNtlPLLightObject* pLightObject = (CNtlPLLightObject*)pPLAttachAttr->pPLEntity;
						if (pLightObject->GetWorldLightColorf(&Pos, &clrfResult, pclrfTemp))
						{
							clrfTemp	= clrfResult;
							pclrfTemp	= &clrfTemp;					
						}
					}
				}					
			}
		}
	}

	if (bCalcShadow)
	{
		RwRGBA clrTemp;
		if (GetWorldShadowColor(Pos, &clrTemp))
		{
			RwRGBAReal clrfTemp;
			RwRGBARealFromRwRGBA(&clrfTemp, &clrTemp);

			RwUInt8 uiGray	= Helper_RGB2Gray(clrTemp.red, clrTemp.green, clrTemp.blue);
			RwReal	fAdd	= 0.0f;

			if (uiGray < NTL_SHADOW_GRAY_MIN)
			{
				fAdd = static_cast<RwReal>(NTL_SHADOW_GRAY_MIN - uiGray) / 255.0f;
			}
			else if (uiGray > NTL_SHADOW_GRAY_MAX)
			{
				fAdd = static_cast<RwReal>(NTL_SHADOW_GRAY_MAX - uiGray) / 255.0f;
			}	

			clrfTemp.red	+= fAdd;
			clrfTemp.green	+= fAdd;
			clrfTemp.blue	+= fAdd;

			CLAMP(clrfTemp.red, 0.0f, 1.0f);
			CLAMP(clrfTemp.green, 0.0f, 1.0f);
			CLAMP(clrfTemp.blue, 0.0f, 1.0f);

			clrfResult.red		*= clrfTemp.red;
			clrfResult.green	*= clrfTemp.green;
			clrfResult.blue		*= clrfTemp.blue;
		}
	}

	clrfResult.alpha = 1.0f;

	RwRGBAFromRwRGBAReal(pOutColor, &clrfResult);

	return TRUE;
}

VOID CNtlWorldFieldManager::UpdateEntityHandler(RwReal _ElapsedTime)
{
	NTL_ASSERTE(m_pPlanetHandler);
	NTL_ASSERTE(m_pWeatherHandler);


	m_pPlanetHandler->Update(_ElapsedTime);
	m_pWeatherHandler->Update(_ElapsedTime);
}

void CNtlWorldFieldManager::Update(RwV3d& AvatarPos, RwReal _ElapsedTime)
{
#ifndef dNTL_WORLD_TOOL_MODE
#ifdef dNTL_WORLD_SCHEDULE_LOADING
	m_WorldScheduler.Scheduling(_ElapsedTime, AvatarPos);
#endif
#endif

	// Dragon sky direction
#ifdef dNTL_WORLD_TOOL_MODE

	static RwBool FlgDragonAppeared = FALSE;
	if(dOKEY(DIK_I))
	{
		FlgDragonAppeared = !FlgDragonAppeared;
		OnDragonSkyAppearence(FlgDragonAppeared);
	}

#endif
	UpdateVisibleSectors();

	UpdateSky(&AvatarPos);

	UpdateDragonSky(&AvatarPos);

	UpdateFog(&AvatarPos);

	UpdateShoreLine(_ElapsedTime);

	UpdateAnotherField();

	UpdateMsg(AvatarPos);

	UpdateFieldMap(AvatarPos);

	UpdateSectors();

	UpdateSectorMap(AvatarPos);

	UpdateEntityHandler(_ElapsedTime);

	CNtlSpecularManager::GetInstance()->Update(_ElapsedTime);

	CNtlFieldColorManager::GetInstance()->Update(_ElapsedTime);

	CNtlOccluderManager::GetInstance()->Update(_ElapsedTime);

	CNtlWorldRpLightManager::GetInstance()->Update(_ElapsedTime);
}

void CNtlWorldFieldManager::Render()
{
}

void CNtlWorldFieldManager::RenderWater(CNtlWorldSector* pNtlWorldSector, RxD3D9InstanceData* pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader)
{
	/*if(pNtlWorldSector->m_pWater->_pDepthMap)
	{
	SetWaterRenderStateBegin(pNtlWorldSector->m_pWater->_RenderstateSrc, pNtlWorldSector->m_pWater->_RenderstateDst, TRUE, dGET_WORLD_PARAM()->ClrDayAndNight);
	}
	else
	{
	SetWaterRenderStateBegin(pNtlWorldSector->m_pWater->_RenderstateSrc, pNtlWorldSector->m_pWater->_RenderstateDst, FALSE, dGET_WORLD_PARAM()->ClrDayAndNight);
	}*/

	m_pWaterEntity->OnRender(pNtlWorldSector, pInstancedData, pResEntryHeader);

	//SetWaterRenderStateEnd();
}

void CNtlWorldFieldManager::GetSectopMap(RpWorldSector* pRpWorldSector, std::vector<CNtlWorldSector*>& vecNtlWorldSector)
{
	vecNtlWorldSector.clear();

	sNtlWorldSector* pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);
	if (pNtlSector && pNtlSector->pNtlWorldSector)
	{
		vecNtlWorldSector.push_back(pNtlSector->pNtlWorldSector);
	}
}

#ifdef dNTL_WORLD_TOOL_MODE

void CNtlWorldFieldManager::OnWaterProc(RwInt32 IdxMenu, RwInt32 IdxSector, sSECTOR_WATER_ATTR& SectorWaterAttr)
{
	switch(IdxMenu)
	{
	case 0:
		{
			RwV3d Pos;
			Pos.x = m_pSectors[IdxSector].DatumPoint.x;
			Pos.z = m_pSectors[IdxSector].DatumPoint.z;
			SectorWaterAttr._Height = GetWorldSectorHeight(Pos) + dWATER_DEFAULT_HEIGHT_FROM_TERRAIN;
			m_pWaterEntity->OnCreate(&m_pSectors[IdxSector], SectorWaterAttr);
		}
		break;

	case 1:
		{
			RwTexture* pTexture = m_pSectors[IdxSector].m_pWater->_pDepthMap;
			m_pSectors[IdxSector].m_pWater->_pDepthMap = NULL;
			m_pWaterEntity->OnCreate(&m_pSectors[IdxSector], SectorWaterAttr);
			m_pSectors[IdxSector].m_pWater->_pDepthMap = pTexture;
			/*
			RwBool WasDepthMap = FALSE;
			if(m_pSectors[IdxSector].m_pWater->_pDepthMap)
			WasDepthMap = TRUE;

			RwV3d Pos;
			Pos.x = m_pSectors[IdxSector].DatumPoint.x;
			Pos.z = m_pSectors[IdxSector].DatumPoint.z;
			SectorWaterAttr._Height = SectorWaterAttr._Height;
			m_pWaterEntity->OnCreate(&m_pSectors[IdxSector], SectorWaterAttr);

			if(WasDepthMap)
			{
			m_pSectors[IdxSector].m_pWater->_pDepthMap =	CNtlPLResourceManager::GetInstance()->CreateTexture("DepthMap",
			SectorWaterAttr._DepthMapSize,
			SectorWaterAttr._DepthMapSize,
			32,
			rwRASTERFORMAT8888);

			RwTextureSetAddressing(m_pSectors[IdxSector].m_pWater->_pDepthMap, rwTEXTUREADDRESSCLAMP);
			RwTextureSetFilterMode(m_pSectors[IdxSector].m_pWater->_pDepthMap, rwFILTERLINEAR);

			RwV2d	PosSector;
			RwV2d	TexPt;
			RwUInt8	AlphaVariation;

			for(int i = 0; i < SectorWaterAttr._DepthMapSize; ++i)
			{
			for(int j = 0; j < SectorWaterAttr._DepthMapSize; ++j)
			{
			PosSector.x		= m_pSectors[IdxSector].DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
			PosSector.y		= m_pSectors[IdxSector].DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2);

			RwReal RelativeSize = (RwReal)dGET_WORLD_PARAM()->WorldSectorSize / (RwReal)SectorWaterAttr._DepthMapSize;
			TexPt.x	= static_cast<RwReal>(j + 1) * RelativeSize - (RelativeSize / 2.0f) + PosSector.x;
			TexPt.y	= static_cast<RwReal>(i + 1) * RelativeSize - (RelativeSize / 2.0f) + PosSector.y;

			RwV3d Temp;
			Temp.x = TexPt.x;
			Temp.z = TexPt.y;
			Temp.y = GetWorldSectorHeight(Temp);

			float Distance = sqrtf((m_pSectors[IdxSector].m_pWater->_Height - Temp.y) * (m_pSectors[IdxSector].m_pWater->_Height - Temp.y));
			CLAMP(Distance, 0.0f, dDEPTH_MAP_DIST_MAX);

			RwReal DistVariation	= ::fabs(SectorWaterAttr._DepthMapHeightVariation[0] - SectorWaterAttr._DepthMapHeightVariation[1]);
			AlphaVariation			= static_cast<RwUInt8>((Distance / 30.0f) * DistVariation + SectorWaterAttr._DepthMapHeightVariation[0]);

			TexPt.x	= static_cast<RwReal>(SectorWaterAttr._DepthMapSize) - ((fabs(TexPt.x - PosSector.x) / (float)dGET_WORLD_PARAM()->WorldSectorSize) * static_cast<RwReal>(SectorWaterAttr._DepthMapSize));
			TexPt.y	= static_cast<RwReal>(SectorWaterAttr._DepthMapSize) - ((fabs(TexPt.y - PosSector.y) / (float)dGET_WORLD_PARAM()->WorldSectorSize) * static_cast<RwReal>(SectorWaterAttr._DepthMapSize));
			TexPt.x	= static_cast<RwReal>(::abs(TexPt.x));
			TexPt.y	= static_cast<RwReal>(::abs(TexPt.y));
			CLAMP(TexPt.x, 0, SectorWaterAttr._DepthMapSize - 1);
			CLAMP(TexPt.y, 0, SectorWaterAttr._DepthMapSize - 1);
			CNtlPLResourceManager::GetInstance()->SetAlphaBits(m_pSectors[IdxSector].m_pWater->_pDepthMap, AlphaVariation, TexPt);
			}
			}
			}
			*/
		}
		break;

	case 2:
		{
			m_pWaterEntity->OnDelete(&m_pSectors[IdxSector]);
		}
		break;

	case 3:
		{
			if(m_pSectors[IdxSector].m_pWater->_pDepthMap)
			{
				RwTextureDestroy(m_pSectors[IdxSector].m_pWater->_pDepthMap);
				m_pSectors[IdxSector].m_pWater->_pDepthMap = NULL;
			}

			*m_pSectors[IdxSector].m_pWater = SectorWaterAttr;

			m_pSectors[IdxSector].m_pWater->_pDepthMap =	CNtlPLResourceManager::GetInstance()->CreateTexture("DepthMap",
				SectorWaterAttr._DepthMapSize,
				SectorWaterAttr._DepthMapSize,
				32,
				rwRASTERFORMAT8888);

			RwTextureSetAddressing(m_pSectors[IdxSector].m_pWater->_pDepthMap, rwTEXTUREADDRESSCLAMP);
			RwTextureSetFilterMode(m_pSectors[IdxSector].m_pWater->_pDepthMap, rwFILTERLINEAR);

			for(int i = 0; i < SectorWaterAttr._DepthMapSize; ++i)
			{
				for(int j = 0; j < SectorWaterAttr._DepthMapSize; ++j)
				{
					/*

					// old version of depth map

					PosSector.x		= m_pSectors[IdxSector].DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
					PosSector.y		= m_pSectors[IdxSector].DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2);

					RwReal RelativeSize = (RwReal)dGET_WORLD_PARAM()->WorldSectorSize / (RwReal)SectorWaterAttr._DepthMapSize;
					TexPt.x	= static_cast<RwReal>(j + 1) * RelativeSize - (RelativeSize / 2.0f) + PosSector.x;
					TexPt.y	= static_cast<RwReal>(i + 1) * RelativeSize - (RelativeSize / 2.0f) + PosSector.y;

					RwV3d Temp;
					Temp.x = TexPt.x;
					Temp.z = TexPt.y;
					Temp.y = GetWorldSectorHeight(Temp);

					float Distance = sqrtf((m_pSectors[IdxSector].m_pWater->_Height - Temp.y) * (m_pSectors[IdxSector].m_pWater->_Height - Temp.y));
					CLAMP(Distance, 0.0f, dDEPTH_MAP_DIST_MAX);

					RwReal DistVariation	= ::fabs(SectorWaterAttr._DepthMapHeightVariation[0] - SectorWaterAttr._DepthMapHeightVariation[1]);
					AlphaVariation			= static_cast<RwUInt8>((Distance / 30.0f) * DistVariation + SectorWaterAttr._DepthMapHeightVariation[0]);

					TexPt.x	= static_cast<RwReal>(SectorWaterAttr._DepthMapSize) - ((fabs(TexPt.x - PosSector.x) / (float)dGET_WORLD_PARAM()->WorldSectorSize) * static_cast<RwReal>(SectorWaterAttr._DepthMapSize));
					TexPt.y	= static_cast<RwReal>(SectorWaterAttr._DepthMapSize) - ((fabs(TexPt.y - PosSector.y) / (float)dGET_WORLD_PARAM()->WorldSectorSize) * static_cast<RwReal>(SectorWaterAttr._DepthMapSize));
					TexPt.x	= static_cast<RwReal>(::abs(TexPt.x));
					TexPt.y	= static_cast<RwReal>(::abs(TexPt.y));
					CLAMP(TexPt.x, 0, SectorWaterAttr._DepthMapSize - 1);
					CLAMP(TexPt.y, 0, SectorWaterAttr._DepthMapSize - 1);

					CNtlPLResourceManager::GetInstance()->SetAlphaBits(m_pSectors[IdxSector].m_pWater->_pDepthMap, AlphaVariation, TexPt);

					*/

					RwV2d TexPt;
					TexPt.x = static_cast<RwReal>(j);
					TexPt.y = static_cast<RwReal>(i);

					CNtlPLResourceManager::GetInstance()->SetAlphaBits(m_pSectors[IdxSector].m_pWater->_pDepthMap, SectorWaterAttr._RGBA.alpha, TexPt);
				}
			}
		}
		break;

	case 4:
		{
			if(m_pSectors[IdxSector].m_pWater->_pDepthMap)
			{
				RwTextureDestroy(m_pSectors[IdxSector].m_pWater->_pDepthMap);
				m_pSectors[IdxSector].m_pWater->_pDepthMap = NULL;
			}
		}
		break;
	}
}

void CNtlWorldFieldManager::RenderFieldGuidePicked()
{
	if(m_IdxCurField == -1)
		return;

	RwInt32	TileSize = dGET_WORLD_PARAM()->WorldSectorTileSize;
	sLINE3D	SLine[2];
	RwV3d	vTemp;

	BeginD3DLineState();

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	// RB -> RT
	for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum * 2; ++k)
	{
		SLine[0].Col = 0xff0000ff;
		SLine[1].Col = 0xff0000ff;

		SLine[0].Pos.x	= m_pFields[m_IdxCurField].GetSPos().x;
		SLine[0].Pos.z	= m_pFields[m_IdxCurField].GetSPos().z + (TileSize * k);
		vTemp.x = SLine[0].Pos.x;
		vTemp.z = SLine[0].Pos.z;
		SLine[0].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		SLine[1].Pos.x	= SLine[0].Pos.x;
		SLine[1].Pos.z	= SLine[0].Pos.z + TileSize;
		vTemp.x = SLine[1].Pos.x;
		vTemp.z = SLine[1].Pos.z;
		SLine[1].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		m_Buffer[m_VertCount++] = SLine[0];
		m_Buffer[m_VertCount++] = SLine[1];
	}

	// RB -> LB
	for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum * 2; ++k)
	{
		SLine[0].Col = 0xff0000ff;
		SLine[1].Col = 0xff0000ff;

		SLine[0].Pos.x	= m_pFields[m_IdxCurField].GetSPos().x + (TileSize * k);
		SLine[0].Pos.z	= m_pFields[m_IdxCurField].GetSPos().z;
		vTemp.x = SLine[0].Pos.x;
		vTemp.z = SLine[0].Pos.z;
		SLine[0].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		SLine[1].Pos.x	= SLine[0].Pos.x + TileSize;
		SLine[1].Pos.z	= SLine[0].Pos.z;
		vTemp.x = SLine[1].Pos.x;
		vTemp.z = SLine[1].Pos.z;
		SLine[1].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		m_Buffer[m_VertCount++] = SLine[0];
		m_Buffer[m_VertCount++] = SLine[1];
	}

	// RT -> LT
	for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum * 2; ++k)
	{
		SLine[0].Col = 0xff0000ff;
		SLine[1].Col = 0xff0000ff;

		SLine[0].Pos.x	= m_pFields[m_IdxCurField].GetSPos().x + (TileSize * k);
		SLine[0].Pos.z	= m_pFields[m_IdxCurField].GetSPos().z + dGET_WORLD_PARAM()->WorldFieldSize;
		vTemp.x = SLine[0].Pos.x;
		vTemp.z = SLine[0].Pos.z;
		SLine[0].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		SLine[1].Pos.x	= SLine[0].Pos.x + TileSize;
		SLine[1].Pos.z	= SLine[0].Pos.z;
		vTemp.x = SLine[1].Pos.x;
		vTemp.z = SLine[1].Pos.z;
		SLine[1].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		m_Buffer[m_VertCount++] = SLine[0];
		m_Buffer[m_VertCount++] = SLine[1];
	}

	// RB -> LT
	for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum * 2; ++k)
	{
		SLine[0].Col = 0xff0000ff;
		SLine[1].Col = 0xff0000ff;

		SLine[0].Pos.x	= m_pFields[m_IdxCurField].GetSPos().x + dGET_WORLD_PARAM()->WorldFieldSize;
		SLine[0].Pos.z	= m_pFields[m_IdxCurField].GetSPos().z + (TileSize * k);
		vTemp.x = SLine[0].Pos.x;
		vTemp.z = SLine[0].Pos.z;
		SLine[0].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		SLine[1].Pos.x	= SLine[0].Pos.x;
		SLine[1].Pos.z	= SLine[0].Pos.z + TileSize;
		vTemp.x = SLine[1].Pos.x;
		vTemp.z = SLine[1].Pos.z;
		SLine[1].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		m_Buffer[m_VertCount++] = SLine[0];
		m_Buffer[m_VertCount++] = SLine[1];
	}

	RwD3D9SetFVF(D3DFVF_LINE3D);
	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, static_cast<RwUInt32>(m_VertCount / 2), m_Buffer, sizeof(sLINE3D));

	EndD3DLineState();
}

RpWorldSector* FieldGuideLineCB(RpWorldSector *pWorldSector, void *data)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);
	if(!pNtlSector)
		return pWorldSector;


	CNtlWorldFieldManager*	pThis		= static_cast<CNtlWorldFieldManager*>(data);
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pThis->GetFields());
	RwInt32					TileSize	= dGET_WORLD_PARAM()->WorldSectorTileSize;
	sLINE3D					SLine[2];
	RwV3d					vTemp;


	// pick fields
	bool bCompare = false;

	vTemp.x = pNtlSector->pNtlWorldSector->DatumPoint.x;
	vTemp.z = pNtlSector->pNtlWorldSector->DatumPoint.z;
	int FieldIndex = pThis->GetFieldIdx(vTemp);
	for(int i = 0; i < (int)pThis->m_vecVisibleField.size(); ++i)
	{
		if(pThis->m_vecVisibleField[i] == FieldIndex)
		{
			bCompare = true;
			break;
		}
	}

	if(!bCompare)
		pThis->m_vecVisibleField.push_back(FieldIndex);

	// draw tile line(RB -> RT)
	for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum; ++k)
	{
		SLine[0].Col = 0xff00ff00;
		SLine[1].Col = 0xff00ff00;

		SLine[0].Pos.x	= pNtlSector->pNtlWorldSector->DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2.0f);
		SLine[0].Pos.z	= pNtlSector->pNtlWorldSector->DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2.0f) + (TileSize * k);
		vTemp.x = SLine[0].Pos.x;
		vTemp.z = SLine[0].Pos.z;

		RwReal PosHeight = pThis->GetWorldSectorHeight(vTemp);
		if(static_cast<RwInt32>(PosHeight) == -999)
		{
			continue;
		}
		else
		{
			SLine[0].Pos.y = PosHeight + 0.5f;
		}

		SLine[1].Pos.x	= SLine[0].Pos.x;
		SLine[1].Pos.z	= SLine[0].Pos.z + TileSize;
		vTemp.x = SLine[1].Pos.x;
		vTemp.z = SLine[1].Pos.z;

		PosHeight = pThis->GetWorldSectorHeight(vTemp);
		if(static_cast<RwInt32>(PosHeight) == -999)
		{
			continue;
		}
		else
		{
			SLine[1].Pos.y = PosHeight + 0.5f;
		}

		pThis->m_Buffer[pThis->m_VertCount++] = SLine[0];
		pThis->m_Buffer[pThis->m_VertCount++] = SLine[1];					
	}

	// RB -> LB
	for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum; ++k)
	{
		SLine[0].Col = 0xff00ff00;
		SLine[1].Col = 0xff00ff00;

		SLine[0].Pos.x	= pNtlSector->pNtlWorldSector->DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2.0f) + (TileSize * k);
		SLine[0].Pos.z	= pNtlSector->pNtlWorldSector->DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2.0f);
		vTemp.x = SLine[0].Pos.x;
		vTemp.z = SLine[0].Pos.z;

		RwReal PosHeight = pThis->GetWorldSectorHeight(vTemp);
		if(static_cast<RwInt32>(PosHeight) == -999)
		{
			continue;
		}
		else
		{
			SLine[0].Pos.y = PosHeight + 0.5f;
		}

		SLine[1].Pos.x	= SLine[0].Pos.x + TileSize;
		SLine[1].Pos.z	= SLine[0].Pos.z;
		vTemp.x = SLine[1].Pos.x;
		vTemp.z = SLine[1].Pos.z;

		PosHeight = pThis->GetWorldSectorHeight(vTemp);
		if(static_cast<RwInt32>(PosHeight) == -999)
		{
			continue;
		}
		else
		{
			SLine[1].Pos.y = PosHeight + 0.5f;
		}

		pThis->m_Buffer[pThis->m_VertCount++] = SLine[0];
		pThis->m_Buffer[pThis->m_VertCount++] = SLine[1];
	}

	return pWorldSector;
}

RpWorldSector* SectorGuideLineCB(RpWorldSector *pWorldSector, void *data)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);
	if(!pNtlSector)
		return pWorldSector;


	CNtlWorldFieldManager*	pThis		= static_cast<CNtlWorldFieldManager*>(data);
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pThis->GetFields());
	RwInt32					TileSize	= dGET_WORLD_PARAM()->WorldSectorTileSize;
	sLINE3D					SLine[2];
	RwV3d					vTemp;


	// pick fields
	bool bCompare = false;

	vTemp.x = pNtlSector->pNtlWorldSector->DatumPoint.x;
	vTemp.z = pNtlSector->pNtlWorldSector->DatumPoint.z;
	int FieldIndex = pThis->GetFieldIdx(vTemp);
	for(int i = 0; i < (int)pThis->m_vecVisibleField.size(); ++i)
	{
		if(pThis->m_vecVisibleField[i] == FieldIndex)
		{
			bCompare = true;
			break;
		}
	}

	if(!bCompare)
		pThis->m_vecVisibleField.push_back(FieldIndex);

	return pWorldSector;
}

void CNtlWorldFieldManager::RenderFieldGuide()
{
	sLINE3D	SLine[2];
	int		CurFieldIdx = 0;
	RwReal	ErrHeight	= 10.0f;
	RwInt32	TileSize	= dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwV3d	vTemp;


	BeginD3DLineState();

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	m_vecVisibleField.clear();
	RwCameraForAllSectorsInFrustum(CNtlPLGlobal::m_RwCamera, SectorGuideLineCB, this);

	m_VertCount = 0;
	if(m_vecVisibleField.size())
	{
		for(int i = 0; i < (int)m_vecVisibleField.size(); ++i)
		{
			CurFieldIdx = m_vecVisibleField[i];

			// draw tile line(RB -> RT)
			for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum * 2; ++k)
			{
				SLine[0].Col = 0xffff0000;
				SLine[1].Col = 0xffff0000;

				SLine[0].Pos.x	= m_pFields[CurFieldIdx].GetSPos().x;
				SLine[0].Pos.z	= m_pFields[CurFieldIdx].GetSPos().z + (TileSize * k);
				vTemp.x = SLine[0].Pos.x;
				vTemp.z = SLine[0].Pos.z;
				SLine[0].Pos.y = GetWorldSectorHeight(vTemp) + 1.0f;

				SLine[1].Pos.x	= SLine[0].Pos.x;
				SLine[1].Pos.z	= SLine[0].Pos.z + TileSize;
				vTemp.x = SLine[1].Pos.x;
				vTemp.z = SLine[1].Pos.z;
				SLine[1].Pos.y = GetWorldSectorHeight(vTemp) + 1.0f;

				m_Buffer[m_VertCount++] = SLine[0];
				m_Buffer[m_VertCount++] = SLine[1];
			}

			// RB -> LB
			for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum * 2; ++k)
			{
				SLine[0].Col = 0xffff0000;
				SLine[1].Col = 0xffff0000;

				SLine[0].Pos.x	= m_pFields[CurFieldIdx].GetSPos().x + (TileSize * k);
				SLine[0].Pos.z	= m_pFields[CurFieldIdx].GetSPos().z;
				vTemp.x = SLine[0].Pos.x;
				vTemp.z = SLine[0].Pos.z;
				SLine[0].Pos.y = GetWorldSectorHeight(vTemp) + 1.0f;

				SLine[1].Pos.x	= SLine[0].Pos.x + TileSize;
				SLine[1].Pos.z	= SLine[0].Pos.z;
				vTemp.x = SLine[1].Pos.x;
				vTemp.z = SLine[1].Pos.z;
				SLine[1].Pos.y = GetWorldSectorHeight(vTemp) + 1.0f;

				m_Buffer[m_VertCount++] = SLine[0];
				m_Buffer[m_VertCount++] = SLine[1];
			}
		}
	}

	RwD3D9SetFVF(D3DFVF_LINE3D);
	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, static_cast<RwUInt32>(m_VertCount / 2), m_Buffer, sizeof(sLINE3D));

	EndD3DLineState();
}

void CNtlWorldFieldManager::RenderSectorGuide()
{
	sLINE3D	SLine[2];
	int		CurFieldIdx = 0;
	RwReal	ErrHeight	= 10.0f;
	RwInt32	TileSize	= dGET_WORLD_PARAM()->WorldSectorTileSize;

	BeginD3DLineState();

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	m_vecVisibleField.clear();
	m_VertCount = 0;
	RwCameraForAllSectorsInFrustum(CNtlPLGlobal::m_RwCamera, FieldGuideLineCB, this);

	RwD3D9SetFVF(D3DFVF_LINE3D);
	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, static_cast<RwUInt32>(m_VertCount / 2), m_Buffer, sizeof(sLINE3D));

	EndD3DLineState();
}

#endif

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

vector<int>	g_vecVisibleSectors;


static RpCollisionTriangle* _IntersectionWaterTriCB( RpIntersection *pIntersection, RpCollisionTriangle *pCollTriangle, RwReal distance, void *data )
{
	sNtlWorldCollisionInfo* pCollInfo	= static_cast<sNtlWorldCollisionInfo*>(data);
	RwLine*					pLine		= &pIntersection->t.line;
	RwV3d					vDelta		= pIntersection->t.line.end - pIntersection->t.line.start;
	RwReal					fDist		= RwV3dLength(&vDelta) * distance;

	if (pCollInfo->fDist - fDist > 0.00001f)
	{
		pCollInfo->fDist						= fDist;
		pCollInfo->RayIntersectionPt4Terrain.x	= pLine->start.x + (distance * vDelta.x);
		pCollInfo->RayIntersectionPt4Terrain.y	= pLine->start.y + (distance * vDelta.y);
		pCollInfo->RayIntersectionPt4Terrain.z	= pLine->start.z + (distance * vDelta.z);
		pCollInfo->IsCollidedAtSectors			= TRUE;
	}

	return pCollTriangle;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlWorldFieldManager : 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNtlWorldFieldManager::CNtlWorldFieldManager(RpWorld *pNtlWorld, RwV3d& SpawnPos, EActiveWorldType _ActiveWorldtype)
{
	m_pRpWorld			= NULL;

	m_pSkyEntity		= NULL;
	m_pDragonSkyEntity	= NULL;
	m_pFogEntity		= NULL;
	m_pWaterEntity		= NULL;
	m_pLightEntity		= NULL;
	m_pBloomEntity		= NULL;
	m_pPlantEntity		= NULL;
	m_CurMapNameIdx		= 0xffffffff;
	m_pPlanetHandler	= NULL;
	m_pWeatherHandler	= NULL;
	m_OldSectorIdx4PVS	= -1;
	m_ActiveWorldType	= _ActiveWorldtype;

	m_pFields			= NULL;

	m_IdxCurField		= -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CNtlWorldFieldManager :
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNtlWorldFieldManager::~CNtlWorldFieldManager(void)
{
}

RwBool CNtlWorldFieldManager::CreateLight()
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreateLight");

	if(m_pLightEntity != NULL)
	{
		DestroyLight();
	}

	m_pLightEntity = reinterpret_cast<CNtlPLLight*>(GetSceneManager()->CreateEntity(PLENTITY_LIGHT, NTL_PLEN_LIGHT));
	DBO_ASSERT(m_pLightEntity, "Entity create failed.");

	if(!m_pLightEntity)
	{
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::CreateWater()
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreateWater");

	if(m_pWaterEntity != NULL)
	{
		DestroyWater();
	}

	m_pWaterEntity = reinterpret_cast<CNtlPLWater*>(GetSceneManager()->CreateEntity(PLENTITY_WATER, "NtlWorldWater"));
	DBO_ASSERT(m_pWaterEntity, "Entity create failed.");

	if(m_pWaterEntity == NULL)
	{
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::CreatePlant()
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreatePlant");

	if(m_pPlantEntity != NULL)
	{
		DestroyPlant();
	}

	m_pPlantEntity = reinterpret_cast<CNtlPLPlant*>(GetSceneManager()->CreateEntity(PLENTITY_PLANT, NTL_PLEN_PLANT));
	DBO_ASSERT(m_pPlantEntity, "Entity create failed.");

	if(m_pPlantEntity == NULL)
	{
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::CreateDragonSky()
{
	if(m_pDragonSkyEntity)
	{
		DestroyDragoneSky();
	}

	SPLEntityCreateParam	Param;
	RwInt32					SkyTypeIdx = eSTI_DRAGONE;

	Param._pUserData	= &SkyTypeIdx;

	m_pDragonSkyEntity	= reinterpret_cast<CNtlPLSky*>(GetSceneManager()->CreateEntity(PLENTITY_SKY, "null", &Param));
	DBO_ASSERT(m_pDragonSkyEntity, "Entity create failed.");

	if(!m_pDragonSkyEntity)
	{
		return FALSE;
	}

	return TRUE;
}

RwBool CNtlWorldFieldManager::CreateSky( void )
{
	if( m_pSkyEntity != NULL )
	{
		DestroySky();
	}

	// There're two types of sky and this depends on dGET_WORLD_PARAM's sky type index
	// 1. Sphere type only for multi fields including no prop. interpolation effect
	// 2. Hemisphere type including prop. interpolation effect

	SPLEntityCreateParam	Param;
	RwInt32					SkyTypeIdx;

	switch (dGET_WORLD_PARAM()->IdxSkyType)
	{
	case eSTI_HEMISPHERE:
		SkyTypeIdx = eSTI_HEMISPHERE;
		Param._pUserData = &SkyTypeIdx;
		break;
	case eSTI_SPHERE:
		SkyTypeIdx = eSTI_SPHERE;
		Param._pUserData = &SkyTypeIdx;
		break;
	}

	m_pSkyEntity = reinterpret_cast<CNtlPLSky*>(GetSceneManager()->CreateEntity(PLENTITY_SKY, "null", &Param));
	DBO_ASSERT(m_pSkyEntity, "Entity create failed.");

	if( m_pSkyEntity == NULL )
	{
		return FALSE;
	}

	return TRUE;
}

RwBool CNtlWorldFieldManager::CreateBloom( void )
{
	if(m_pBloomEntity)
	{
		DestroyBloom();
	}

	m_pBloomEntity = reinterpret_cast<CNtlPLBloom *>(GetSceneManager()->CreateEntity(PLENTITY_BLOOM, NTL_PLEN_BLOOM));
	DBO_ASSERT(m_pBloomEntity, "Entity create failed.");

	if (m_pBloomEntity == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

void CNtlWorldFieldManager::DestroyLight()
{
	if(m_pLightEntity)
	{
		GetSceneManager()->DeleteEntity(m_pLightEntity);
		m_pLightEntity = NULL;
	}
}

void CNtlWorldFieldManager::DestroyBloom()
{
	if(m_pBloomEntity)
	{
		GetSceneManager()->DeleteEntity(m_pBloomEntity);
		m_pBloomEntity = NULL;
	}
}

void CNtlWorldFieldManager::DestroySky()
{
	if( m_pSkyEntity != NULL )
	{
		GetSceneManager()->DeleteEntity( m_pSkyEntity );
		m_pSkyEntity = NULL;
	}

	return;
}

void CNtlWorldFieldManager::DestroyDragoneSky()
{
	if(m_pDragonSkyEntity)
	{
		GetSceneManager()->DeleteEntity(m_pDragonSkyEntity);
		m_pDragonSkyEntity = NULL;
	}
}

void CNtlWorldFieldManager::UpdateSky( RwV3d * pPosAvatar )
{
	if( m_pSkyEntity != NULL )
	{
		m_pSkyEntity->SetPosition( pPosAvatar );
	}

	return;
}

void CNtlWorldFieldManager::UpdateDragonSky(RwV3d * pPosAvatar)
{
	if(m_pDragonSkyEntity)
	{
		m_pDragonSkyEntity->SetPosition(pPosAvatar);
	}

	return;
}

RwBool CNtlWorldFieldManager::CreateFog( void )
{
	if( m_pFogEntity != NULL )
	{
		DestroyFog();
	}

	m_pFogEntity = reinterpret_cast<CNtlPLFog *>(GetSceneManager()->CreateEntity( PLENTITY_FOG, "null" ));
	DBO_ASSERT(m_pFogEntity, "Entity create failed.");

	if( m_pFogEntity == NULL )
	{
		return FALSE;
	}

	return TRUE;
}

void CNtlWorldFieldManager::DestroyWater()
{
	if(m_pWaterEntity != NULL)
	{
		GetSceneManager()->DeleteEntity(m_pWaterEntity);
		m_pWaterEntity = NULL;
	}
}

void CNtlWorldFieldManager::DestroyFog()
{
	if( m_pFogEntity != NULL )
	{
		GetSceneManager()->DeleteEntity( m_pFogEntity );
		m_pFogEntity = NULL;
	}
}

void CNtlWorldFieldManager::DestroyPlant()
{
	if (m_pPlantEntity != NULL)
	{
		GetSceneManager()->DeleteEntity(m_pPlantEntity);
		m_pPlantEntity = NULL;
	}
}

void CNtlWorldFieldManager::UpdateFog(RwV3d * pPosAvatar)
{
	if( m_pFogEntity != NULL )
	{
		m_pFogEntity->SetPosition( pPosAvatar );
	}

	return;
}

VOID CNtlWorldFieldManager::LoadPVS()
{
	BYTE	PVSFlag	= FALSE;
	BYTE	NeighborVisibility[dPVS_TOT_CELL_CNT * dPVS_LAYER_CNT];

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);

	FILE *pFile = NULL;	
	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
	{
		static RwChar chPackPatch[NTL_MAX_DIR_PATH];
		strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "-------");

		SPackResFileData sPackFileData;
		RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(chPackPatch, sPackFileData);
		if(bPack)
		{
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			::fopen_s(&pFile, sPackFileData.strPackFileName.c_str(), "rb");
			if(pFile)
			{
				fseek(pFile, sPackFileData.uiOffset, SEEK_SET);
			}
		}
	}
	else
	{
		pFile = fopen("-------", "rb");
	}

	DBO_ASSERT(pFile, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");

	if (pFile)
	{
		int iError = 1;
		for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum; ++i)
		{
			iError = fread(&PVSFlag, sizeof(BYTE), 1, pFile);
			if(PVSFlag)
			{
				iError = fread(NeighborVisibility, sizeof(BYTE) * dPVS_TOT_CELL_CNT * dPVS_LAYER_CNT, 1, pFile);
				m_pSectors[i].m_pNtlWorldSectorPVS->SetVisibilityNeighbor(NeighborVisibility);
			}
		}

		fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
}

void CNtlWorldFieldManager::CreateRpWorld()
{
	m_pRpWorld = RpNtlWorldCreate();
	DBO_ASSERT(m_pRpWorld, "World create failed.");
	CNtlPLGlobal::SetActiveWorld(m_pRpWorld);
}

RpLight* NtlWorldRemoveLightWhenWorldDestroy(RpLight* light, void *data)
{	
	RpWorldRemoveLight((RpWorld*)data, light);

	return light;
}

RpAtomic* NtlWorldLogAtmoicWhenWorldDestroy(RpAtomic* atomic, void *data)
{
	// Destroy 시 Atomic이 물고 있는 Class 접근하여 이름을 알아온다.
	if (RpNtlAtomicGetData(atomic))
	{
		CNtlPLEntity* pPLEntity = static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(atomic));

		NtlLogFilePrintArgs("Atomics still exist : PTR(%u) TYPE(%u) NAME(%s)", (RwUInt32)pPLEntity, pPLEntity->GetClassType(), pPLEntity->GetName());
	}
	else
	{
		NtlLogFilePrint("Atomics still exist : RpNtlAtomicGetData(NULL)");
	}

	return atomic;
}

RpClump* NtlWorldRemoveClumpWhenWorldDestroy(RpClump* clump, void *data)
{	
	RpClumpForAllAtomics(clump, NtlWorldLogAtmoicWhenWorldDestroy, data);
	RpWorldRemoveClump((RpWorld *)data, clump);

	return clump;
}

void CNtlWorldFieldManager::DestroyRpWorld()
{
	if (m_pRpWorld)
	{
		if (!rwLinkListEmpty(&m_pRpWorld->directionalLightList) ||
			!rwLinkListEmpty(&m_pRpWorld->lightList))
		{
			NtlLogFilePrint("Lights still exist in the world when world was destroyed");
			RpWorldForAllLights(m_pRpWorld, NtlWorldRemoveLightWhenWorldDestroy, m_pRpWorld);
		}
		if (!rwLinkListEmpty(&m_pRpWorld->clumpList))
		{
			NtlLogFilePrint("Clumps still exist in the world when world was destroyed");
			RpWorldForAllClumps(m_pRpWorld, NtlWorldRemoveClumpWhenWorldDestroy, m_pRpWorld);
		}		
		RpWorldDestroy(m_pRpWorld);
	}
	CNtlPLGlobal::SetActiveWorld(NULL);
}

void CNtlWorldFieldManager::Init(RpWorld *pNtlWorld, RwV3d& SpawnPos)
{
	NTL_FUNCTION("CNtlWorldFieldManager::Init");

	CreateRpWorld();

	InitSingleInstance();

	// 인도어
	CNtlWorldSectorManager::Init();

	RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// initialize m_pFields
	RwV3d SPos;

	m_pFields = NTL_NEW CNtlWorldField [dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum];

	for(int i = -dGET_WORLD_PARAM()->WorldSizeHalf; i < dGET_WORLD_PARAM()->WorldSizeHalf; i+= dGET_WORLD_PARAM()->WorldFieldSize)
	{
		for(int j = -dGET_WORLD_PARAM()->WorldSizeHalf; j < dGET_WORLD_PARAM()->WorldSizeHalf; j+= dGET_WORLD_PARAM()->WorldFieldSize)
		{
			SPos.x = static_cast<RwReal>(j);
			SPos.z = static_cast<RwReal>(i);
			RwInt32 Idx = GetFieldIdx(SPos);
			m_pFields[Idx].Init(SPos);
		}
	}

	// initialize common uv coordinates
	NtlCommonTexCoord = NTL_NEW RwTexCoords [dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum];
	for(int i = 0; i < dGET_WORLD_PARAM()->WorldSectorVertNum; ++i)
	{
		for(int j = 0; j < dGET_WORLD_PARAM()->WorldSectorVertNum; ++j)
		{
			NtlCommonTexCoord[j + i * dGET_WORLD_PARAM()->WorldSectorVertNum].u = (1.0f - j * dGET_WORLD_PARAM()->WorldSectorTileSize / (float)dGET_WORLD_PARAM()->WorldSectorSize);
			NtlCommonTexCoord[j + i * dGET_WORLD_PARAM()->WorldSectorVertNum].v = (1.0f - i * dGET_WORLD_PARAM()->WorldSectorTileSize / (float)dGET_WORLD_PARAM()->WorldSectorSize);
		}
	}

	// generate world sector pointer array
	CreateSectorMap(m_pRpWorld);

	// load PVS data
	LoadPVS();

	// initialize spawning position such as avatar, datum index, move flags
	RwInt32 CurFieldIdx = GetFieldIdx(SpawnPos);
	NTL_ASSERTE(CurFieldIdx != -1);

	m_OldDatumIdx = CurFieldIdx;
	m_NewDatumIdx = CurFieldIdx;

	m_eMoved2 = eC;
	::memset(&m_Fields6x6[0], -1, 36 * sizeof(RwInt32));
	::memset(&m_Fields6x6[1], -1, 36 * sizeof(RwInt32));

	// World field switching effects;
	m_FieldPropVariationRestTime = -999;
	m_FieldPropVariationStarting = FALSE;

	// Entities
	CreateSky();
	CreateDragonSky();
	GetSceneManager()->SetActiveSky(m_pSkyEntity);
	CreateFog();
	CreatePlant();
	CreateWater();
	CreateLight();
	CreateBloom();

	m_pPlanetHandler	= NTL_NEW CNtlPLPlanetHandler;
	m_pWeatherHandler	= NTL_NEW CNtlPLWeatherHandler;

	NTL_RETURNVOID();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Free :
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNtlWorldFieldManager::Free(void)
{
	NTL_FUNCTION("CNtlWorldFieldManager::Free");

	// 인도어
	CNtlWorldSectorManager::Free();

	DestroySky();
	DestroyDragoneSky();
	DestroyFog();
	DestroyPlant();
	DestroyWater();
	DestroyLight();
	DestroyBloom();

	NTL_ARRAY_DELETE(m_pFields);
	NTL_ARRAY_DELETE(NtlCommonTexCoord);

	g_vecVisibleSectors.clear();

	NTL_DELETE(m_pPlanetHandler);
	NTL_DELETE(m_pWeatherHandler);

	FreeSingleInstance();

	DestroyRpWorld();

	NTL_RETURNVOID();
}

void CNtlWorldFieldManager::InitSingleInstance()
{
	g_pNtlWorldBGMManager = NTL_NEW CNtlWorldBGMManager;

	CNtlSpecularManager::CreateInstance();
	CNtlSpecularManager::GetInstance()->Create();

	CNtlFieldColorManager::CreateInstance();
	CNtlFieldColorManager::GetInstance()->Create();

	CNtlOccluderManager::CreateInstance();
	CNtlOccluderManager::GetInstance()->Create();

	CNtlWorldRpLightManager::CreateInstance();

	CNtlWorldPathEngineManager::CreateInstance();

#ifdef dNTL_WORLD_TOOL_MODE
	FILE* pFile = NULL;
	//char Buffer[256];
	if(!fopen_s(&pFile, "./tool/saber/script/pathengine.txt", "r"))
	{
		RwInt32 iCunkSize;
		fscanf(pFile, "%d", &iCunkSize);
		if (iCunkSize > 0)
		{
			CNtlWorldPathEngineManager::GetInstance()->SetChunkSize(iCunkSize);
			CNtlWorldPathEngineManager::GetInstance()->SetPathEngineMode(TRUE);
		}
		fclose(pFile);
	}

	CNtlWorldShadowManager::GetInst();
	CNtlWorldMergeManager::GetInstance()->SetFieldManager(this);
#endif dNTL_WORLD_TOOL_MODE	
}

void CNtlWorldFieldManager::FreeSingleInstance()
{
	CNtlWorldPathEngineManager::DestroyInstance();

	CNtlWorldRpLightManager::DestroyInstance();

	CNtlOccluderManager::GetInstance()->Destroy();
	CNtlOccluderManager::DestroyInstance();

	CNtlFieldColorManager::GetInstance()->Destroy();
	CNtlFieldColorManager::DestroyInstance();

	CNtlSpecularManager::GetInstance()->Destroy();
	CNtlSpecularManager::DestroyInstance();

#ifdef dNTL_WORLD_TOOL_MODE
	CNtlWorldShadowManager::GetInst()->Destroy();
#endif dNTL_WORLD_TOOL_MODE

	NTL_DELETE(g_pNtlWorldBGMManager);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateSectorMap :
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNtlWorldFieldManager::CreateSectorMap(RpWorld *pNtlWorld)
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreateSectorMap");


	RpSector *spSect;
	RpSector *spaStack[rpWORLDMAXBSPDEPTH];
	RwInt32 nStack = 0;

	spSect = pNtlWorld->rootSector;

	do
	{
		if (spSect->type < 0)
		{
			// hook the sector; it's atomic and leaf sector
			CNtlWorldSectorManager::SetWorldSector((RpWorldSector*)spSect);

			spSect = spaStack[nStack--];
		}
		else
		{
			RpPlaneSector *pspPlane = (RpPlaneSector *) spSect;

			// left then right
			spSect = pspPlane->leftSubTree;
			spaStack[++nStack] = pspPlane->rightSubTree;
		}
	}
	while (nStack >= 0);


	NTL_RETURNVOID();
}

RwBool CNtlWorldFieldManager::IsFieldValid(RwInt32 Idx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::IsFieldValid");


	// verify the sector index is being in current world
	if(Idx < 0 || Idx > (dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum - 1))
		NTL_RETURN(FALSE);


	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::IsFieldValid(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWorldFieldManager::IsFieldValid");


	if(Pos.x <= dGET_WORLD_PARAM()->WorldValueMax &&
		Pos.z <= dGET_WORLD_PARAM()->WorldValueMax &&
		Pos.x >= dGET_WORLD_PARAM()->WorldValueMin &&
		Pos.z >= dGET_WORLD_PARAM()->WorldValueMin)
		NTL_RETURN(TRUE);

	NTL_RETURN(FALSE);
}

sCUR_FIELD_TEX_INFO* CNtlWorldFieldManager::GetTexAttr()
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetTexAttr");


	if(m_IdxCurField == -1 || !(&m_pFields[m_IdxCurField]))
		NTL_RETURN(NULL);


	NTL_RETURN(&m_pFields[m_IdxCurField].GetTexAttr());
}

sCUR_FIELD_TEX_INFO* CNtlWorldFieldManager::GetTexAttr(RwInt32 IdxField)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetTexAttr");

	if(IdxField == -1 || !(&m_pFields[m_IdxCurField]))
		NTL_RETURN(NULL);

	m_IdxCurField = IdxField;


	NTL_RETURN(&m_pFields[IdxField].GetTexAttr());
}

RpAtomic* CNtlWorldFieldManager::GetAtomic(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetAtomic");

	RwInt32 IdxSector = CNtlWorldSectorManager::GetSectorIdx(Pos);

	if(IdxSector == -1)
	{
		NTL_RETURN(NULL);
	}
	else
	{
		sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(CNtlWorldSectorManager::m_pSectors[IdxSector].m_pWorldSector, pNtlSector);
		if(!pNtlSector)
		{
			NTL_RETURN(NULL);
		}

		NTL_RETURN(pNtlSector->pNtlWorldSector->m_pAtomic);
	}

	NTL_RETURN(NULL);
}

RwBool CNtlWorldFieldManager::GetWorldReady()
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetWorldReady");

	if(m_eMoved2 != eC)
		NTL_RETURN(TRUE);

	NTL_RETURN(FALSE);
}

RwInt32 CNtlWorldFieldManager::GetFieldIdx(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetFieldIdx");

	if(!IsFieldValid(Pos))
		NTL_RETURN(-1);

	RwInt32 RetIdx = CNtlMath::GetSafeIdx3D(Pos, dGET_WORLD_PARAM()->WorldSizeHalf, dGET_WORLD_PARAM()->WorldFieldSize, dGET_WORLD_PARAM()->WorldFieldNum);

// 	RwInt32 WidthNum, DepthNum;
// 	RwReal XBasedOn0 = (RwReal)((RwInt32)Pos.x + dGET_WORLD_PARAM()->WorldSizeHalf);
// 	RwReal ZBasedOn0 = (RwReal)((RwInt32)Pos.z + dGET_WORLD_PARAM()->WorldSizeHalf);
// 	WidthNum = (RwInt32)(XBasedOn0 / (RwReal)dGET_WORLD_PARAM()->WorldFieldSize);
// 	DepthNum = (RwInt32)(ZBasedOn0 / (RwReal)dGET_WORLD_PARAM()->WorldFieldSize);
// 
// 	RwInt32 RetIdx = WidthNum + (DepthNum * dGET_WORLD_PARAM()->WorldFieldNum);

	if(!IsFieldValid(RetIdx))
		NTL_RETURN(-1);

	NTL_RETURN(RetIdx);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UpdateChangeDir : 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNtlWorldFieldManager::UpdateDatumDir()
{
	// if eC then it's the first time for making terrain
	if(m_eMoved2 == eC || m_eMoved2 == ePORTAL)
	{
		return;
	}

	if(m_NewDatumIdx == m_OldDatumIdx + dGET_WORLD_PARAM()->WorldFieldNum)		
	{
		m_eMoved2 = eN;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx + dGET_WORLD_PARAM()->WorldFieldNum - 1)
	{
		m_eMoved2 = eNE;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx - 1)					
	{
		m_eMoved2 = eE;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx - dGET_WORLD_PARAM()->WorldFieldNum - 1)
	{
		m_eMoved2 = eES;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx - dGET_WORLD_PARAM()->WorldFieldNum)
	{
		m_eMoved2 = eS;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx - dGET_WORLD_PARAM()->WorldFieldNum + 1)	
	{
		m_eMoved2 = eSW;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx + 1)
	{
		m_eMoved2 = eW;
	}
	else if(m_NewDatumIdx == m_OldDatumIdx + dGET_WORLD_PARAM()->WorldFieldNum + 1)
	{
		m_eMoved2 = eWN;
	}
	else
	{
		// 개발 시 Brack를 걸고 서버이동(버스등)을 하게 되면 m_eMove2가 변경 되지
		// 않는 경우가 발생 한다. 이 경우 ePORTAL 처리를 강행한다.
		// exception; Sync. wasn't matched
		m_eMoved2 = ePORTAL;
		DBO_TRACE(FALSE, "Critical errors occured");
#ifdef dNTL_WORLD_TOOL_MODE
		::MessageBox(NULL, "Critical errors occured", "SAVER", MB_OK);
#endif
	}
}

// find some loading parts out considering 4 x 4 area
RwBool CNtlWorldFieldManager::IsThereNewRegion2Load()
{
	NTL_FUNCTION("CNtlWorldFieldManager::IsThereNewRegion2Load");

	// -------------------------------
	// | 35 | 34 | 33 | 32 | 31 | 30 |
	// -------------------------------
	// | 29 | 28 | 27 | 26 | 25 | 24 |
	// -------------------------------
	// | 23 | 22 | 21 | 20 | 19 | 18 |
	// -------------------------------
	// | 17 | 16 | ^^ | 14 | 13 | 12 |
	// -------------------------------
	// | 11 | 10 | 09 | 08 | 07 | 06 |
	// -------------------------------
	// | 05 | 04 | 03 | 02 | 01 | 00 |
	// -------------------------------

	RwInt32 Cur6x6Idx = -1;
	for(RwInt32 i = 0; i < 36; ++i)
	{
		if(m_Fields6x6[0][i] == m_OldDatumIdx)
		{
			Cur6x6Idx = i;
			break;
		}
	}

	switch(m_eMoved2)
	{
	case eN:
		{
			if(Cur6x6Idx == 15 || Cur6x6Idx == 14)
				NTL_RETURN(FALSE);
		}
		break;
	case eNE:
		{
			if(Cur6x6Idx == 15)
				NTL_RETURN(FALSE);
		}
		break;
	case eE:
		{
			if(Cur6x6Idx == 15 || Cur6x6Idx == 21)
				NTL_RETURN(FALSE);
		}
		break;
	case eES:
		{
			if(Cur6x6Idx == 21)
				NTL_RETURN(FALSE);
		}
		break;
	case eS:
		{
			if(Cur6x6Idx == 21 || Cur6x6Idx == 20)
				NTL_RETURN(FALSE);
		}
		break;
	case eSW:
		{
			if(Cur6x6Idx == 20)
				NTL_RETURN(FALSE);
		}
		break;
	case eW:
		{
			if(Cur6x6Idx == 14 || Cur6x6Idx == 20)
				NTL_RETURN(FALSE);
		}
		break;
	case eWN:
		{
			if(Cur6x6Idx == 14)
				NTL_RETURN(FALSE);
		}
		break;
	}

	NTL_RETURN(TRUE);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UpdateNeighborFields :
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNtlWorldFieldManager::UpdateNeighborFields(RwV3d& Pos)
{
	if(m_eMoved2 != eC && m_eMoved2 != ePORTAL)
		if(!IsThereNewRegion2Load())
			return;

	// -------------------------------
	// | 35 | 34 | 33 | 32 | 31 | 30 |
	// -------------------------------
	// | 29 | 28 | 27 | 26 | 25 | 24 |
	// -------------------------------
	// | 23 | 22 | 21 | 20 | 19 | 18 |
	// -------------------------------
	// | 17 | 16 | ^^ | 14 | 13 | 12 |
	// -------------------------------
	// | 11 | 10 | 09 | 08 | 07 | 06 |
	// -------------------------------
	// | 05 | 04 | 03 | 02 | 01 | 00 |
	// -------------------------------

	RwInt32 CheckIdx;
	RwV3d NeighborPt;
	RwInt32 i, j;

	switch(m_eMoved2)
	{
	case ePORTAL:
		{
			for(i = 0; i < 36; ++i)
				if(m_Fields6x6[0][i] != -1)
					DeleteFields(m_Fields6x6[0][i]);
		}

		// do eC straight away; no breaks here

	case eC:
		{
			/*
			dGET_WORLD_PARAM()->Loading = TRUE;

			NeighborPt.x =	Pos.x;
			NeighborPt.z =	Pos.z + dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
			return;
			else
			{
			RwV3d NeighborIdx4PosFirst;
			RwV3d NeighborIdx4Pos;
			RwInt32 NeighborIdx;

			NeighborIdx4PosFirst.x = Pos.x - dGET_WORLD_PARAM()->WorldFieldSize * 3;
			NeighborIdx4PosFirst.z = Pos.z - dGET_WORLD_PARAM()->WorldFieldSize * 2;

			for(j = 0; j < 6; ++j)
			for(i = 0; i < 6; ++i)
			{
			NeighborIdx4Pos.x = NeighborIdx4PosFirst.x + i * dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborIdx4Pos.z = NeighborIdx4PosFirst.z + j * dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborIdx = GetFieldIdx(NeighborIdx4Pos);
			if(-1 == NeighborIdx)
			m_Fields6x6[1][j * 6 + i] = -1;
			else
			{
			m_Fields6x6[1][j * 6 + i] = NeighborIdx;
			CreateFields(m_Fields6x6[1][j * 6 + i]);
			}
			}

			m_eMoved2 = eIDLE;
			}

			dGET_WORLD_PARAM()->Loading = FALSE;
			*/

			dGET_WORLD_PARAM()->Loading = TRUE;

			NeighborPt.x =	Pos.x;
			NeighborPt.z =	Pos.z;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			else
			{
				RwV3d NeighborIdx4PosFirst;
				RwV3d NeighborIdx4Pos;
				RwInt32 NeighborIdx;

				NeighborIdx4PosFirst.x = Pos.x - dGET_WORLD_PARAM()->WorldFieldSize * 3;
				NeighborIdx4PosFirst.z = Pos.z - dGET_WORLD_PARAM()->WorldFieldSize * 2;

				for(j = 0; j < 6; ++j)
					for(i = 0; i < 6; ++i)
					{
						NeighborIdx4Pos.x = NeighborIdx4PosFirst.x + i * dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborIdx4Pos.z = NeighborIdx4PosFirst.z + j * dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborIdx = GetFieldIdx(NeighborIdx4Pos);
						if(-1 == NeighborIdx)
							m_Fields6x6[1][j * 6 + i] = -1;
						else
						{
							m_Fields6x6[1][j * 6 + i] = NeighborIdx;
							CreateFields(m_Fields6x6[1][j * 6 + i]);
						}
					}

					m_eMoved2 = eIDLE;
			}

			dGET_WORLD_PARAM()->Loading = FALSE;
		}
		break;

	case eN:
		{
			NeighborPt.x =	Pos.x;
			NeighborPt.z =	Pos.z + dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			else
			{
				// check same ids up
				for(i = 6; i < 36; ++i)
					m_Fields6x6[1][i - 6] = m_Fields6x6[0][i];

				// check new ids up
				for(i = 0; i < 6; ++i)
				{
					if(m_Fields6x6[0][30 + i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][30 + i]].GetSPos().x;
						NeighborPt.z = m_pFields[m_Fields6x6[0][30 + i]].GetSPos().z + dGET_WORLD_PARAM()->WorldFieldSize;
						m_Fields6x6[1][30 + i] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][30 + i] = -1;

					if(m_Fields6x6[1][30 + i] != -1)
						CreateFields(m_Fields6x6[1][30 + i]);
				}

				// check old ids up
				for(i = 0; i < 6; ++i)
					if(m_Fields6x6[0][i] != -1)
						DeleteFields(m_Fields6x6[0][i]);
			}
		}
		break;

	case eNE:
		{
			NeighborPt.x =	Pos.x - dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborPt.z =	Pos.z + dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			else
			{
				// check same ids up
				RwInt32 IndicesFrom[25] = {6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34};
				RwInt32 Indices2[25] = {1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29};
				for(i = 0; i < 25; ++i)
					m_Fields6x6[1][Indices2[i]] = m_Fields6x6[0][IndicesFrom[i]];

				// check new ids up
				for(i = 30; i <= 34; ++i)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z + dGET_WORLD_PARAM()->WorldFieldSize;
						m_Fields6x6[1][i + 1] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i + 1] = -1;

					if(m_Fields6x6[1][i + 1] != -1)
						CreateFields(m_Fields6x6[1][i + 1]);
				}

				for(i = 6; i <= 30; i+= 6)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x - dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z;
						m_Fields6x6[1][i - 6] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i - 6] = -1;

					if(m_Fields6x6[1][i - 6] != -1)
						CreateFields(m_Fields6x6[1][i - 6]);
				}

				NeighborPt.x = m_pFields[m_Fields6x6[0][30]].GetSPos().x - dGET_WORLD_PARAM()->WorldFieldSize;
				NeighborPt.z = m_pFields[m_Fields6x6[0][30]].GetSPos().z + dGET_WORLD_PARAM()->WorldFieldSize;
				m_Fields6x6[1][30] = GetFieldIdx(NeighborPt);
				if(m_Fields6x6[1][30] != -1)
					CreateFields(m_Fields6x6[1][30]);

				// check old ids up
				RwInt32 Indices2Del[11] = {0, 1, 2, 3, 4, 5, 11, 17, 23, 29, 35};
				for(i = 0; i < 11; ++i)
					if(m_Fields6x6[0][Indices2Del[i]] != -1)
						DeleteFields(m_Fields6x6[0][Indices2Del[i]]);
			}
		}
		break;

	case eE:
		{
			NeighborPt.x =	Pos.x - dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborPt.z =	Pos.z;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			else
			{
				// check same ids up
				RwInt32 IndicesFrom[30] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34};
				RwInt32 Indices2[30] = {1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 31, 32, 33, 34, 35};
				for(i = 0; i < 30; ++i)
					m_Fields6x6[1][Indices2[i]] = m_Fields6x6[0][IndicesFrom[i]];

				// check new ids up
				for(i = 0; i <= 30; i += 6)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x - dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z;
						m_Fields6x6[1][i] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i] = -1;

					if(m_Fields6x6[1][i] != -1)
						CreateFields(m_Fields6x6[1][i]);
				}

				// check old ids up
				for(i = 5; i <= 35; i += 6)
					if(m_Fields6x6[0][i] != -1)
						DeleteFields(m_Fields6x6[0][i]);
			}
		}
		break;

	case eES:
		{
			NeighborPt.x =	Pos.x - dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborPt.z =	Pos.z - dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			else
			{
				// check same ids up
				RwInt32 IndicesFrom[25] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28};
				RwInt32 Indices2[25] = {7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 31, 32, 33, 34, 35};
				for(i = 0; i < 25; ++i)
					m_Fields6x6[1][Indices2[i]] = m_Fields6x6[0][IndicesFrom[i]];

				// check new ids up
				for(i = 0; i <= 24; i += 6)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x - dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z;
						m_Fields6x6[1][i + 6] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i + 6] = -1;

					if(m_Fields6x6[1][i + 6] != -1)
						CreateFields(m_Fields6x6[1][i + 6]);
				}

				for(i = 0; i <= 4; ++i)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z - dGET_WORLD_PARAM()->WorldFieldSize;
						m_Fields6x6[1][i + 1] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i + 1] = -1;

					if(m_Fields6x6[1][i + 1] != -1)
						CreateFields(m_Fields6x6[1][i + 1]);
				}

				NeighborPt.x = m_pFields[m_Fields6x6[0][0]].GetSPos().x - dGET_WORLD_PARAM()->WorldFieldSize;
				NeighborPt.z = m_pFields[m_Fields6x6[0][0]].GetSPos().z - dGET_WORLD_PARAM()->WorldFieldSize;
				m_Fields6x6[1][0] = GetFieldIdx(NeighborPt);
				if(m_Fields6x6[1][0] != -1)
					CreateFields(m_Fields6x6[1][0]);

				// check old ids up
				RwInt32 Indices2Del[11] = {5, 11, 17, 23, 29, 35, 34, 33, 32, 31, 30};
				for(i = 0; i < 11; ++i)
					if(m_Fields6x6[0][Indices2Del[i]] != -1)
						DeleteFields(m_Fields6x6[0][Indices2Del[i]]);
			}
		}
		break;

	case eS:
		{
			NeighborPt.x =	Pos.x;
			NeighborPt.z =	Pos.z - dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			{
				// check same ids up
				for(i = 0; i <= 29; ++i)
					m_Fields6x6[1][i + 6] = m_Fields6x6[0][i];

				// check new ids up
				for(i = 0; i < 6; ++i)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z - dGET_WORLD_PARAM()->WorldFieldSize;
						m_Fields6x6[1][i] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i] = -1;

					if(m_Fields6x6[1][i] != -1)
						CreateFields(m_Fields6x6[1][i]);
				}

				// check old ids up
				for(i = 30; i <= 35; ++i)
					if(m_Fields6x6[0][i] != -1)
						DeleteFields(m_Fields6x6[0][i]);
			}
		}
		break;

	case eSW:
		{
			NeighborPt.x =	Pos.x + dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborPt.z =	Pos.z - dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			{
				// check same ids up
				RwInt32 IndicesFrom[25] = {1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29};
				RwInt32 Indices2[25] = {6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34};
				for(i = 0; i < 25; ++i)
					m_Fields6x6[1][Indices2[i]] = m_Fields6x6[0][IndicesFrom[i]];

				// check new ids up
				for(i = 1; i <= 5; ++i)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z - dGET_WORLD_PARAM()->WorldFieldSize;
						m_Fields6x6[1][i - 1] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i - 1] = -1;

					if(m_Fields6x6[1][i - 1] != -1)
						CreateFields(m_Fields6x6[1][i - 1]);
				}

				for(i = 5; i <= 29; i += 6)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x + dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z;
						m_Fields6x6[1][i + 6] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i + 6] = -1;

					if(m_Fields6x6[1][i + 6] != -1)
						CreateFields(m_Fields6x6[1][i + 6]);
				}

				NeighborPt.x = m_pFields[m_Fields6x6[0][5]].GetSPos().x + dGET_WORLD_PARAM()->WorldFieldSize;
				NeighborPt.z = m_pFields[m_Fields6x6[0][5]].GetSPos().z - dGET_WORLD_PARAM()->WorldFieldSize;
				m_Fields6x6[1][5] = GetFieldIdx(NeighborPt);
				if(m_Fields6x6[1][5] != -1)
					CreateFields(m_Fields6x6[1][5]);

				// check old ids up
				RwInt32 Indices2Del[11] = {0, 6, 12, 18, 24, 30, 31, 32, 33, 34, 35};
				for(i = 0; i < 11; ++i)
					if(m_Fields6x6[0][Indices2Del[i]] != -1)
						DeleteFields(m_Fields6x6[0][Indices2Del[i]]);
			}
		}
		break;

	case eW:
		{
			NeighborPt.x =	Pos.x + dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborPt.z =	Pos.z;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			{
				// check same ids up
				RwInt32 IndicesFrom[30] = {1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 31, 32, 33, 34, 35};
				RwInt32 Indices2[30] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34};
				for(i = 0; i < 30; ++i)
					m_Fields6x6[1][Indices2[i]] = m_Fields6x6[0][IndicesFrom[i]];

				// check new ids up
				for(i = 5; i <= 35; i += 6)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x + dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z;
						m_Fields6x6[1][i] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i] = -1;

					if(m_Fields6x6[1][i] != -1)
						CreateFields(m_Fields6x6[1][i]);
				}


				// check old ids up
				for(i = 0; i <= 30; i += 6)
					if(m_Fields6x6[0][i] != -1)
						DeleteFields(m_Fields6x6[0][i]);
			}
		}
		break;

	case eWN:
		{
			NeighborPt.x =	Pos.x + dGET_WORLD_PARAM()->WorldFieldSize;
			NeighborPt.z =	Pos.z + dGET_WORLD_PARAM()->WorldFieldSize;
			CheckIdx = GetFieldIdx(NeighborPt);
			if(CheckIdx == -1)
				return;
			{
				// check same ids up
				RwInt32 IndicesFrom[25] = {7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 31, 32, 33, 34, 35};
				RwInt32 Indices2[25] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 12, 13, 14, 15 ,16 ,18 ,19 ,20 ,21 ,22, 24, 25, 26, 27, 28};
				for(i = 0; i < 25; ++i)
					m_Fields6x6[1][Indices2[i]] = m_Fields6x6[0][IndicesFrom[i]];

				// check new ids up
				for(i = 11; i <= 35; i += 6)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x + dGET_WORLD_PARAM()->WorldFieldSize;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z;
						m_Fields6x6[1][i - 6] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i - 6] = -1;

					if(m_Fields6x6[1][i - 6] != -1)
						CreateFields(m_Fields6x6[1][i - 6]);
				}

				for(i = 31; i <= 35; ++i)
				{
					if(m_Fields6x6[0][i] != -1)
					{
						NeighborPt.x = m_pFields[m_Fields6x6[0][i]].GetSPos().x;
						NeighborPt.z = m_pFields[m_Fields6x6[0][i]].GetSPos().z + dGET_WORLD_PARAM()->WorldFieldSize;
						m_Fields6x6[1][i - 1] = GetFieldIdx(NeighborPt);
					}
					else
						m_Fields6x6[1][i - 1] = -1;

					if(m_Fields6x6[1][i - 1] != -1)
						CreateFields(m_Fields6x6[1][i - 1]);
				}

				NeighborPt.x = m_pFields[m_Fields6x6[0][35]].GetSPos().x + dGET_WORLD_PARAM()->WorldFieldSize;
				NeighborPt.z = m_pFields[m_Fields6x6[0][35]].GetSPos().z + dGET_WORLD_PARAM()->WorldFieldSize;
				m_Fields6x6[1][35] = GetFieldIdx(NeighborPt);
				if(m_Fields6x6[1][35] != -1)
					CreateFields(m_Fields6x6[1][35]);

				// check old ids up
				RwInt32 Indices2Del[11] = {0, 1, 2, 3, 4, 5, 6, 12, 18, 24, 30};
				for(i = 0; i < 11; ++i)
					if(m_Fields6x6[0][Indices2Del[i]] != -1)
						DeleteFields(m_Fields6x6[0][Indices2Del[i]]);
			}
		}
		break;
	}

	CopyMemory(&m_Fields6x6[0], &m_Fields6x6[1], 36 * sizeof(RwInt32));
}

void CNtlWorldFieldManager::UpdateLODAttr(RwV3d& Pos)
{
	if(!dGET_WORLD_PARAM()->LODEnable)
		return;

	int l, m;
	int MyLvl, NeighborLvl;
	int WidthSectorNum;
	int DepthSectorNum;
	int CurSectorIdx;

	UpdateLODAttrLvl(Pos);

	for(int i = 0; i < 36; ++i)
	{
		if(m_Fields6x6[1][i] == -1)
			continue;

		RwV3d SPos = m_pFields[m_Fields6x6[1][i]].GetSPos();
		RwV3d SectorSPos;

		for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x = (RwReal)m;
				SectorSPos.z = (RwReal)l;
				CurSectorIdx = GetSectorIdx(SectorSPos);
				if(CurSectorIdx == -1)
					continue;

				MyLvl = m_pSectors[CurSectorIdx].m_LODAttr.MipMapLevel;
				WidthSectorNum = CurSectorIdx / dGET_WORLD_PARAM()->WorldSectorNum;
				DepthSectorNum = CurSectorIdx % dGET_WORLD_PARAM()->WorldSectorNum;

				if(DepthSectorNum + 1 < dGET_WORLD_PARAM()->WorldSectorNum)
				{
					if(IsSectorValid(CurSectorIdx + dGET_WORLD_PARAM()->WorldSectorNum))
					{
						NeighborLvl = m_pSectors[CurSectorIdx + dGET_WORLD_PARAM()->WorldSectorNum].m_LODAttr.MipMapLevel;
						m_pSectors[CurSectorIdx].m_LODAttr.TSectionLevel = NeighborLvl;
					}
				}
				if(WidthSectorNum - 1 >= 0)
				{
					if(IsSectorValid(CurSectorIdx + 1))
					{
						NeighborLvl = m_pSectors[CurSectorIdx + 1].m_LODAttr.MipMapLevel;
						m_pSectors[CurSectorIdx].m_LODAttr.LSectionLevel = NeighborLvl;
					}
				}
				if(WidthSectorNum + 1 < dGET_WORLD_PARAM()->WorldSectorNum)
				{
					if(IsSectorValid(CurSectorIdx - 1))
					{
						NeighborLvl = m_pSectors[CurSectorIdx - 1].m_LODAttr.MipMapLevel;
						m_pSectors[CurSectorIdx].m_LODAttr.RSectionLevel = NeighborLvl;
					}
				}
				if(DepthSectorNum - 1 >= 0)
				{
					if(IsSectorValid(CurSectorIdx - dGET_WORLD_PARAM()->WorldSectorNum))
					{
						NeighborLvl = m_pSectors[CurSectorIdx - dGET_WORLD_PARAM()->WorldSectorNum].m_LODAttr.MipMapLevel;
						m_pSectors[CurSectorIdx].m_LODAttr.BSectionLevel = NeighborLvl;
					}
				}
			}
		}
	}
}

void CNtlWorldFieldManager::UpdateLODAttrLvl(RwV3d& Pos)
{
	int l, m;
	int CurSectorIdx;
	float CurDistance;
	D3DXVECTOR3 vDatumSPos = D3DXVECTOR3(Pos.x, 0.0f, Pos.z);


	for(int i = 0; i < 36; ++i)
	{
		if(m_Fields6x6[1][i] == -1)
			continue;

		RwV3d SPos = m_pFields[m_Fields6x6[1][i]].GetSPos();
		RwV3d SectorSPos;

		for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x = (RwReal)m;
				SectorSPos.z = (RwReal)l;
				CurSectorIdx = GetSectorIdx(SectorSPos);

				if(CurSectorIdx == -1)
					continue;

				CurDistance = D3DXVec3Length(&(vDatumSPos - m_pSectors[CurSectorIdx].DatumPoint)) * 0.9f;
				float LengthPerLevel = dGET_WORLD_PARAM()->WorldFarPlane / dGET_SECTOR_LOD()->GetTotLvlCnt();
				m_pSectors[CurSectorIdx].m_LODAttr.MipMapLevel = dGET_SECTOR_LOD()->GetTotLvlCnt() - (int)floor(CurDistance / LengthPerLevel);
				CLAMP(m_pSectors[CurSectorIdx].m_LODAttr.MipMapLevel, 0, dGET_SECTOR_LOD()->GetTotLvlCnt());
			}
		}
	}
}

RpCollisionTriangle* WorldIntersectionAtomicTriCB(RpIntersection *pIntersection, RpCollisionTriangle *pCollTriangle, RwReal distance, void *data)
{
	sNtlMapObjCollisionParam *pCollisionParam = static_cast<sNtlMapObjCollisionParam*>(data);

	if(!pCollisionParam->pCurAtomic || pCollisionParam->DistFromCam > distance)
	{
		pCollisionParam->DistFromCam		= distance;
		pCollisionParam->IsCollided			= TRUE;
		pCollisionParam->PtIntersection.x	= pIntersection->t.line.start.x + distance * (pIntersection->t.line.end.x - pIntersection->t.line.start.x);
		pCollisionParam->PtIntersection.y	= pIntersection->t.line.start.y + distance * (pIntersection->t.line.end.y - pIntersection->t.line.start.y);
		pCollisionParam->PtIntersection.z	= pIntersection->t.line.start.z + distance * (pIntersection->t.line.end.z - pIntersection->t.line.start.z);
	}

	return pCollTriangle;
}

RpAtomic* WorldIntersectionAtomicCB(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData)
{
	if((static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic))->GetClassType() != PLENTITY_OBJECT))
	{
		return pAtomic;
	}

	sNtlMapObjCollisionParam*	pCollisionParam = static_cast<sNtlMapObjCollisionParam*>(pData);
	CNtlPLObject*				pNtlPLObject	= static_cast<CNtlPLObject*>(RpNtlAtomicGetData(pAtomic));

	if(dGET_BRUSH_ENTITY() != pNtlPLObject)
	{
#ifdef dNTL_WORLD_TOOL_MODE
		if (((pNtlPLObject->GetFlags() & NTL_PLEFLAG_COLLISION) && (RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE)) && !CNtlPLGlobal::m_bCollObjVisible)
		{
			return pAtomic;
		}
#endif 
		sNtlMapObjCollisionParam CollisionParamInTri;
		::memset(&CollisionParamInTri, 0, sizeof(sNtlMapObjCollisionParam));

		RpAtomicForAllIntersections(pAtomic, pIntersection, WorldIntersectionAtomicTriCB, &CollisionParamInTri);

		if(CollisionParamInTri.IsCollided)
		{
			if(!pCollisionParam->pCurAtomic || pCollisionParam->DistFromCam > CollisionParamInTri.DistFromCam)
			{
				pCollisionParam->pCurAtomic		= pAtomic;
				pCollisionParam->DistFromCam	= CollisionParamInTri.DistFromCam;
				pCollisionParam->IsCollided		= TRUE;
				::memcpy(&pCollisionParam->PtIntersection, &CollisionParamInTri.PtIntersection, sizeof(RwV3d));
			}
		}
	}

	return pAtomic;
}

RpCollisionTriangle* GetNtlWorldOutdoorCollisionInfoMiniIndoor(RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void *pData)
{
	sNtlWorldCollisionInfo* pCollInfo	= static_cast<sNtlWorldCollisionInfo*>(pData);
	RwLine*					pLine		= &pIntersection->t.line;
	RwV3d					vDelta		= pIntersection->t.line.end - pIntersection->t.line.start;
	RwReal					fDist		= RwV3dLength(&vDelta) * fRatio;

	if (pCollInfo->fDist - fDist > 0.00001f)
	{
		RwV3d vCollPos;
		vCollPos.x = pLine->start.x + (fRatio * vDelta.x);
		vCollPos.y = pLine->start.y + (fRatio * vDelta.y);
		vCollPos.z = pLine->start.z + (fRatio * vDelta.z);

		pCollInfo->fDist						= fDist;
		pCollInfo->RayIntersectionPt4Terrain	= vCollPos;
		pCollInfo->IsCollidedAtSectors			= TRUE;
	}
	return pRpCollisionTriangle;
}

RpAtomic* GetNtlWorldOutdoorCollisionInfo(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fRatio, void* pData)
{
	if((static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic))->GetClassType() != PLENTITY_OBJECT))
	{
		return pAtomic;
	}

	sNtlMapObjCollisionParam*	pCollisionParam = static_cast<sNtlMapObjCollisionParam*>(pData);
	CNtlPLObject*				pNtlPLObject	= static_cast<CNtlPLObject*>(RpNtlAtomicGetData(pAtomic));

	if (pNtlPLObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		pNtlPLObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		RpAtomicForAllIntersections(pAtomic, pIntersection, GetNtlWorldOutdoorCollisionInfoMiniIndoor, pData);
	}

	return pAtomic;
}

RpCollisionTriangle* GetNtlWorldOutdoorCollisionInfo(RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void *pData)
{
	sNtlWorldCollisionInfo* pCollInfo	= static_cast<sNtlWorldCollisionInfo*>(pData);
	RwLine*					pLine		= &pIntersection->t.line;
	RwV3d					vDelta		= pIntersection->t.line.end - pIntersection->t.line.start;
	RwReal					fDist		= RwV3dLength(&vDelta) * fRatio;

	if (pCollInfo->fDist - fDist > 0.00001f)
	{
		RwV3d vCollPos;
		vCollPos.x = pLine->start.x + (fRatio * vDelta.x);
		vCollPos.y = pLine->start.y + (fRatio * vDelta.y);
		vCollPos.z = pLine->start.z + (fRatio * vDelta.z);

		// 툴에서의 Mouse Pick Data는 TransparencyTile도 충돌해야 한다.
		if (!dGET_COLLISION_INFO_UPDATE() && GetSceneManager()->GetWorldAttribute(vCollPos) & dNMAP_TRANSPARENCY_TILE_FLAG)
		{
			return pRpCollisionTriangle;
		}

		pCollInfo->fDist						= fDist;
		pCollInfo->RayIntersectionPt4Terrain	= vCollPos;
		pCollInfo->IsCollidedAtSectors			= TRUE;
	}
	return pRpCollisionTriangle;
}

RpWorldSector* GetNtlWorldOutdoorCollisionInfo(RpIntersection * pIntersection, RpWorldSector * pRpWorldSector, void *pData)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);
	if (pNtlSector && pNtlSector->pNtlWorldSector)
	{
		CNtlWorldSector*	pNtlWorldSector = pNtlSector->pNtlWorldSector;
		RpAtomic*			pAtomic			= pNtlWorldSector->m_pAtomic;

		if (pAtomic)
		{
			if (pIntersection->type == rpINTERSECTLINE && !NtlTestLineSphere(&pIntersection->t.line.start, &pIntersection->t.line.end, &pAtomic->worldBoundingSphere))
			{
				return pRpWorldSector;
			}
			RpAtomicForAllIntersections(pAtomic, pIntersection, GetNtlWorldOutdoorCollisionInfo, pData);
		}
	}

	return pRpWorldSector;	
}

RpCollisionTriangle* GetNtlWorldOutdoorCollisionInfoWater( RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void *pData)
{
	sNtlWorldCollisionInfo* pCollInfo	= static_cast<sNtlWorldCollisionInfo*>(pData);
	RwLine*					pLine		= &pIntersection->t.line;
	RwV3d					vDelta		= pIntersection->t.line.end - pIntersection->t.line.start;
	RwReal					fDist		= RwV3dLength(&vDelta) * fRatio;

	if (pCollInfo->fDist - fDist > 0.00001f)
	{
		pCollInfo->fDist						= fDist;
		pCollInfo->RayIntersectionPt4Terrain.x	= pLine->start.x + (fRatio * vDelta.x);
		pCollInfo->RayIntersectionPt4Terrain.y	= pLine->start.y + (fRatio * vDelta.y);
		pCollInfo->RayIntersectionPt4Terrain.z	= pLine->start.z + (fRatio * vDelta.z);
		pCollInfo->IsCollidedAtSectors			= TRUE;
	}
	return pRpCollisionTriangle;
}

RpWorldSector* GetNtlWorldOutdoorCollisionInfoWater(RpIntersection * pIntersection, RpWorldSector * pRpWorldSector, void *pData)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);
	if (pNtlSector && pNtlSector->pNtlWorldSector)
	{
		CNtlWorldSector*	pNtlWorldSector = pNtlSector->pNtlWorldSector;
		RpAtomic*			pAtomic			= pNtlWorldSector->m_pWater ? pNtlWorldSector->m_pWater->_pAtom : NULL;

		if (pAtomic)
		{
			if (pIntersection->type == rpINTERSECTLINE && !NtlTestLineSphere(&pIntersection->t.line.start, &pIntersection->t.line.end, &pAtomic->worldBoundingSphere))
			{
				return pRpWorldSector;
			}
			RpAtomicForAllIntersections(pAtomic, pIntersection, GetNtlWorldOutdoorCollisionInfoWater, pData);
		}
	}

	return pRpWorldSector;
}

RwBool CNtlWorldFieldManager::PickTerrain(RwInt32 ScreenPosX, RwInt32 ScreenPosY, RwV3d& IntersectionPt)
{
	RwV2d ScreenPos;
	ScreenPos.x = (RwReal)ScreenPosX;
	ScreenPos.y = (RwReal)ScreenPosY;

	RwLine cameraRay;
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

	return PickTerrain(cameraRay.start, cameraRay.end, IntersectionPt);
}

RwBool CNtlWorldFieldManager::PickTerrain(RwV3d& StartPos, RwV3d& EndPos, RwV3d& IntersectionPt)
{
	if(m_eMoved2 == eC)
	{
		return FALSE;
	}

	RpIntersection rpIntersection;
	rpIntersection.type			= rpINTERSECTLINE;
	rpIntersection.t.line.start	= StartPos;
	rpIntersection.t.line.end	= EndPos;

	RwV3d	vDir;
	RwReal	fDist;
	RwV3dSubMacro(&vDir, &EndPos, &StartPos);

	fDist = RwV3dLength(&vDir);

	RwV3dNormalize(&vDir, &vDir);

	sNtlWorldCollisionInfo NtlWorldCollisionInfo;
	NtlWorldCollisionInfo.IsCollidedAtSectors		= FALSE;
	NtlWorldCollisionInfo.RayOri					= StartPos;
	NtlWorldCollisionInfo.RayDir					= vDir;
	NtlWorldCollisionInfo.RayIntersectionPt4Terrain = EndPos;
	NtlWorldCollisionInfo.fDist						= fDist;

	RpWorldForAllWorldSectorIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldOutdoorCollisionInfo, &NtlWorldCollisionInfo);
	if (dGET_COLLISION_INFO_UPDATE())
	{
		RpWorldForAllAtomicIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldOutdoorCollisionInfo, &NtlWorldCollisionInfo);
	}

	if(NtlWorldCollisionInfo.IsCollidedAtSectors)
	{
		if(NtlWorldCollisionInfo.fDist > dGET_WORLD_PARAM()->WorldFarPlane * 0.9f)
		{
			NtlWorldCollisionInfo.IsCollidedAtSectors = FALSE;
			return FALSE;
		}

		IntersectionPt = NtlWorldCollisionInfo.RayIntersectionPt4Terrain;

		return TRUE;
	}
	return FALSE;
}

RwBool CNtlWorldFieldManager::Pick(RwInt32 ScreenPosX, RwInt32 ScreenPosY, RwV3d& IntersectionPt, sNtlWorldCollisionInfo* pOutWorldCollsionInfo)
{
	RwV2d ScreenPos;
	ScreenPos.x = (RwReal)ScreenPosX;
	ScreenPos.y = (RwReal)ScreenPosY;

	RwLine cameraRay;
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

	return Pick(cameraRay.start, cameraRay.end, IntersectionPt, pOutWorldCollsionInfo);
}

RwBool CNtlWorldFieldManager::Pick(RwV3d& StartPos, RwV3d& EndPos, RwV3d& IntersectionPt, sNtlWorldCollisionInfo* pOutWorldCollsionInfo)
{
	if(m_eMoved2 == eC)
	{
		return FALSE;
	}

	RpIntersection rpIntersection;
	rpIntersection.type			= rpINTERSECTLINE;
	rpIntersection.t.line.start	= StartPos;
	rpIntersection.t.line.end	= EndPos;

	RwV3d	vDir;
	RwReal	fDist;
	RwV3dSubMacro(&vDir, &EndPos, &StartPos);

	fDist = RwV3dLength(&vDir);

	RwV3dNormalize(&vDir, &vDir);

	sNtlWorldCollisionInfo NtlWorldCollisionInfo;
	NtlWorldCollisionInfo.IsCollidedAtSectors		= FALSE;
	NtlWorldCollisionInfo.RayOri					= StartPos;
	NtlWorldCollisionInfo.RayDir					= vDir;
	NtlWorldCollisionInfo.RayIntersectionPt4Terrain = EndPos;
	NtlWorldCollisionInfo.fDist						= fDist;

	RpWorldForAllWorldSectorIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldOutdoorCollisionInfo, &NtlWorldCollisionInfo);
	if (dGET_COLLISION_INFO_UPDATE())
	{
		RpWorldForAllAtomicIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldOutdoorCollisionInfo, &NtlWorldCollisionInfo);
#ifdef dNTL_WORLD_TOOL_MODE
		if (dGET_CUR_PALETTE() == ePM_DECAL || dGET_CUR_PALETTE() == ePM_SHORE_LINE)
		{
			RpWorldForAllWorldSectorIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldOutdoorCollisionInfoWater, &NtlWorldCollisionInfo);
		}
#endif
	}
#ifndef dNTL_WORLD_TOOL_MODE
	RpWorldForAllWorldSectorIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldOutdoorCollisionInfoWater, &NtlWorldCollisionInfo);
#endif

	if(NtlWorldCollisionInfo.IsCollidedAtSectors)
	{		
		if (NtlWorldCollisionInfo.fDist > dGET_WORLD_PARAM()->WorldFarPlane * 0.9f)
		{
			NtlWorldCollisionInfo.IsCollidedAtSectors = FALSE;
		}
		else
		{
			IntersectionPt = NtlWorldCollisionInfo.RayIntersectionPt4Terrain;
		}
	}

	if (pOutWorldCollsionInfo)
	{
		RwBool FlgStopColliding = pOutWorldCollsionInfo->FlgStopColliding;
		memcpy(pOutWorldCollsionInfo, &NtlWorldCollisionInfo, sizeof(sNtlWorldCollisionInfo));
		pOutWorldCollsionInfo->FlgStopColliding = FlgStopColliding;
	}

	return NtlWorldCollisionInfo.IsCollidedAtSectors;
}

RwBool CNtlWorldFieldManager::GetHeight(RwV3d& _Pos)
{
	if(!IsFieldValid(_Pos))
	{
		return FALSE;
	}

	RwInt32 SectorIdx = GetSectorIdx(_Pos);
	if(SectorIdx == -1)
	{
		return FALSE;
	}

	if(IsSectorLoaded(SectorIdx))
	{
		_Pos.y = GetWorldSectorHeight(_Pos);
	}
	else
	{
		RwV3d	CurRVert;
		CurRVert.x = _Pos.x + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
		CurRVert.z = _Pos.z + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
		RwInt32 CurXTileCnt = static_cast<RwInt32>(CurRVert.x / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize));
		RwInt32 CurZTileCnt = static_cast<RwInt32>(CurRVert.z / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize));

		// 3 2
		// 1 0
		RwV3d InterpolationSPos[4];
		InterpolationSPos[0].x = static_cast<RwReal>(CurXTileCnt) * static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
		InterpolationSPos[0].z = static_cast<RwReal>(CurZTileCnt) * static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
		InterpolationSPos[0].x -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);
		InterpolationSPos[0].z -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);

		InterpolationSPos[1].x = InterpolationSPos[0].x + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
		InterpolationSPos[1].z = InterpolationSPos[0].z;
		InterpolationSPos[2].x = InterpolationSPos[0].x;
		InterpolationSPos[2].z = InterpolationSPos[0].z + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
		InterpolationSPos[3].x = InterpolationSPos[0].x + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
		InterpolationSPos[3].z = InterpolationSPos[0].z + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);

		RwV3d MPos;
		CNtlMath::MathRwV3dAssign(&MPos, m_pSectors[SectorIdx].DatumPoint.x, m_pSectors[SectorIdx].DatumPoint.y, m_pSectors[SectorIdx].DatumPoint.z);
		GetHeightFromFile(MPos, InterpolationSPos[0]);
		GetHeightFromFile(MPos, InterpolationSPos[1]);
		GetHeightFromFile(MPos, InterpolationSPos[2]);
		GetHeightFromFile(MPos, InterpolationSPos[3]);

		D3DXVECTOR3 LT = D3DXVECTOR3(InterpolationSPos[3].x, InterpolationSPos[0].y, InterpolationSPos[3].z);
		D3DXVECTOR3 RT = D3DXVECTOR3(InterpolationSPos[2].x, InterpolationSPos[1].y, InterpolationSPos[2].z);
		D3DXVECTOR3 LB = D3DXVECTOR3(InterpolationSPos[1].x, InterpolationSPos[2].y, InterpolationSPos[1].z);
		D3DXVECTOR3 RB = D3DXVECTOR3(InterpolationSPos[0].x, InterpolationSPos[3].y, InterpolationSPos[0].z);

		D3DXPLANE p;
		float DX = _Pos.x - RB.x;
		float DZ = _Pos.z - RB.z;
		if(DZ > static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize) - DX)
		{
			D3DXPlaneFromPoints(&p, &LB, &LT, &RT);
		}
		else
		{
			D3DXPlaneFromPoints(&p, &LB, &RT, &RB);
		}

		D3DXPlaneNormalize(&p, &p);

		_Pos.y = -(p.a * _Pos.x + p.c * _Pos.z + p.d) / p.b;
	}

	return TRUE;
}

RwBool CNtlWorldFieldManager::GetHeightFromFile(RwV3d& _PosSectorDatum, RwV3d& _PosTile)
{
	RwInt32 l, m;
	RwInt32 IdxSector	= GetSectorIdx(_PosSectorDatum);
	RwInt32 IdxField	= GetFieldIdx(_PosSectorDatum);

	if(IdxField == -1 || IdxSector == -1)
	{
		return FALSE;
	}

	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxField);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}

	RwReal	TileSize;
	RwReal	SPosX;	
	RwReal	SPosZ;		
	RwReal	CPosX;		
	RwReal	CPosZ;		
	RwInt32 XCnt;		
	RwInt32 ZCnt;		
	RwInt32	CntVert;
	RwV3d	Result;

	RwV3d	SPos = m_pFields[IdxField].GetSPos();
	RwV3d	SectorSPos;
	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;

			if(GetSectorIdx(SectorSPos) == IdxSector)
			{
				TileSize	= (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;
				SPosX		= SectorSPos.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				SPosZ		= SectorSPos.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosX		= _PosTile.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosZ		= _PosTile.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				XCnt		= (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize));
				ZCnt		= (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);
				CntVert		= XCnt + ZCnt * dGET_WORLD_PARAM()->WorldSectorVertNum;

				_RpNtlWorldSectorReadMaterialDummy(pFile);
				::fseek(pFile, sizeof(RwV3d) * CntVert, SEEK_CUR);
				::fread(&Result, sizeof(RwV3d), 1, pFile);
				_PosTile.y = Result.y;
				::fclose(pFile);
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
				return TRUE;
			}
			else
			{
				_RpNtlWorldSectorReadDummy(pFile); 
			}
		}
	}

	DBO_TRACE(FALSE, "CNtlWorldFieldManager::GetHeightFromFile, nothing matched.");

	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return FALSE;
}

RwBool CNtlWorldFieldManager::GetVertFromFile(RwV3d& DatumPt, RwV3d& Result)
{
	RwInt32 l, m;
	RwInt32 IdxSector	= GetSectorIdx(DatumPt);
	RwInt32 IdxField	= GetFieldIdx(DatumPt);

	if(IdxField == -1 || IdxSector == -1)
	{
		return FALSE;
	}

	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxField);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}

	RwReal	TileSize;
	RwReal	SPosX;	
	RwReal	SPosZ;		
	RwReal	CPosX;		
	RwReal	CPosZ;		
	RwInt32 XCnt;		
	RwInt32 ZCnt;		
	RwInt32	CntVert;

	RwV3d	SPos = m_pFields[IdxField].GetSPos();
	RwV3d	SectorSPos;
	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;

			if(GetSectorIdx(SectorSPos) == IdxSector)
			{
				TileSize	= (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;
				SPosX		= SectorSPos.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				SPosZ		= SectorSPos.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosX		= DatumPt.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosZ		= DatumPt.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				XCnt		= (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize));
				ZCnt		= (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);
				CntVert		= XCnt + ZCnt * dGET_WORLD_PARAM()->WorldSectorVertNum;

				_RpNtlWorldSectorReadMaterialDummy(pFile);
				::fseek(pFile, sizeof(RwV3d) * CntVert, SEEK_CUR);
				::fread(&Result, sizeof(RwV3d), 1, pFile);
				::fclose(pFile);
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
				return TRUE;
			}
			else
			{
				_RpNtlWorldSectorReadDummy(pFile);
			}
		}
	}

	DBO_TRACE(FALSE, "CNtlWorldFieldManager::GetVertFromFile, file stream read failed.");
	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return FALSE;
}

RwBool CNtlWorldFieldManager::GetVertInMemory(RwV3d& DatumPt, RwV3d& Result)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetVert");

	RwInt32 RetIdx = GetSectorIdx(DatumPt);
	if(RetIdx == -1)
	{
		Result.x = Result.y = Result.z = 0.0f;
		NTL_RETURN(FALSE);
	}

	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(m_pSectors[RetIdx].m_pWorldSector, pNtlSector);
	if(!pNtlSector)
	{
		Result.x = Result.y = Result.z = 0.0f;
		NTL_RETURN(FALSE);
	}

	RwReal	TileSize	= (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwReal	SPosX		= m_pSectors[RetIdx].m_pWorldSector->boundingBox.inf.x + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal	SPosZ		= m_pSectors[RetIdx].m_pWorldSector->boundingBox.inf.z + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal	CPosX		= DatumPt.x + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal	CPosZ		= DatumPt.z + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwInt32 XCnt		= (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize));
	RwInt32 ZCnt		= (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);

	RwV3d	*pVList		= RpMorphTargetGetVertices(pNtlSector->pNtlWorldSector->m_pAtomic->geometry->morphTarget);
	RwInt32	CurVertIdx	= XCnt + ZCnt * dGET_WORLD_PARAM()->WorldSectorVertNum;

	Result = pVList[CurVertIdx];

	NTL_RETURN(TRUE);
}

#ifdef dNTL_WORLD_TOOL_MODE

RwBool CNtlWorldFieldManager::SetHeight(RwV3d& PosSectorDatum, RwV3d& PosTileDatum)
{
	RwInt32 IdxSector	= GetSectorIdx(PosSectorDatum);
	RwInt32 IdxField	= GetFieldIdx(PosSectorDatum);

	if(IdxField == -1 || IdxSector == -1)
	{
		return FALSE;
	}

	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxField);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb+"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}

	RwReal	TileSize;
	RwReal	SPosX;	
	RwReal	SPosZ;		
	RwReal	CPosX;		
	RwReal	CPosZ;		
	RwInt32 XCnt;		
	RwInt32 ZCnt;		
	RwInt32	CntVert;

	RwV3d	SPos = m_pFields[IdxField].GetSPos();
	RwV3d	SectorSPos;
	for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;

			if(GetSectorIdx(SectorSPos) == IdxSector)
			{
				TileSize	= (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;
				SPosX		= SectorSPos.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				SPosZ		= SectorSPos.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosX		= PosTileDatum.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosZ		= PosTileDatum.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				XCnt		= (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize));
				ZCnt		= (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);
				CntVert		= XCnt + ZCnt * dGET_WORLD_PARAM()->WorldSectorVertNum;

				_RpNtlWorldSectorReadMaterialDummy(pFile);

				::fseek(pFile, sizeof(RwV3d) * CntVert, SEEK_CUR);
				::fwrite(&PosTileDatum, sizeof(RwV3d), 1, pFile);
				::fclose(pFile);
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

				return TRUE;
			}
			else
			{
				_RpNtlWorldSectorReadDummy(pFile);
			}
		}
	}

	DBO_TRACE(FALSE, "CNtlWorldFieldManager::SetHeight, stream read failed.");

	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return FALSE;
}

RwBool CNtlWorldFieldManager::SetClrVertList(RwV3d& PosCurVert, RwRGBA* pClrVertList)
{
	NTL_FUNCTION("CNtlWorldFieldManager::SetClrVertList");

	RwInt32 IdxSector	= GetSectorIdx(PosCurVert);
	RwInt32 IdxField	= GetFieldIdx(PosCurVert);

	if(IdxField == -1 || IdxSector == -1)
	{
		NTL_RETURN(FALSE);
	}

	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxField);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb+"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		NTL_RETURN(FALSE);
	}

	RwReal	TileSize;
	RwReal	SPosX;	
	RwReal	SPosZ;		
	RwReal	CPosX;		
	RwReal	CPosZ;		
	RwInt32 XCnt;		
	RwInt32 ZCnt;		
	RwInt32	CntVert;

	RwV3d	SPos = m_pFields[IdxField].GetSPos();
	RwV3d	SectorSPos;
	for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;

			if(GetSectorIdx(SectorSPos) == IdxSector)
			{
				TileSize	= (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;
				SPosX		= SectorSPos.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				SPosZ		= SectorSPos.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosX		= PosCurVert.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosZ		= PosCurVert.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				XCnt		= (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize));
				ZCnt		= (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);
				CntVert		= XCnt + ZCnt * dGET_WORLD_PARAM()->WorldSectorVertNum;

				_RpNtlWorldSectorReadMaterialDummy(pFile);
				::fseek(pFile, sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, SEEK_CUR);
				::fwrite(pClrVertList, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);
				::fclose(pFile);
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

				NTL_RETURN(TRUE);
			}
			else
			{
				_RpNtlWorldSectorReadDummy(pFile);
			}
		}
	}

	DBO_TRACE(FALSE, "CNtlWorldFieldManager::SetClrVertList, stream read failed.");

	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(FALSE);		
}

RwBool CNtlWorldFieldManager::GetClrVertList(RwV3d& PosCurVert, RwRGBA* pClrVertList)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetClrVertList");

	RwInt32 IdxSector	= GetSectorIdx(PosCurVert);
	RwInt32 IdxField	= GetFieldIdx(PosCurVert);

	if(IdxField == -1 || IdxSector == -1)
	{
		NTL_RETURN(FALSE);
	}

	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxField);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		NTL_RETURN(FALSE);
	}

	RwReal	TileSize;
	RwReal	SPosX;	
	RwReal	SPosZ;		
	RwReal	CPosX;		
	RwReal	CPosZ;		
	RwInt32 XCnt;		
	RwInt32 ZCnt;		
	RwInt32	CntVert;

	RwV3d	SPos = m_pFields[IdxField].GetSPos();
	RwV3d	SectorSPos;
	for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;

			if(GetSectorIdx(SectorSPos) == IdxSector)
			{
				TileSize	= (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;
				SPosX		= SectorSPos.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				SPosZ		= SectorSPos.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosX		= PosCurVert.x + dGET_WORLD_PARAM()->WorldSizeHalf;
				CPosZ		= PosCurVert.z + dGET_WORLD_PARAM()->WorldSizeHalf;
				XCnt		= (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize));
				ZCnt		= (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);
				CntVert		= XCnt + ZCnt * dGET_WORLD_PARAM()->WorldSectorVertNum;

				_RpNtlWorldSectorReadMaterialDummy(pFile);
				::fseek(pFile, sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, SEEK_CUR);
				::fread(pClrVertList, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);
				::fclose(pFile);
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

				NTL_RETURN(TRUE);
			}
			else
			{
				_RpNtlWorldSectorReadDummy(pFile);
			}
		}
	}

	DBO_TRACE(FALSE, "CNtlWorldFieldManager::GetClrVertList, stream read failed.");

	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(FALSE);		
}

RwBool CNtlWorldFieldManager::GetPosVertList(RwV3d& PosCurVert, RwV3d* pPosVertList)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetPosVertList");

	RwInt32 IdxSector	= GetSectorIdx(PosCurVert);
	RwInt32 IdxField	= GetFieldIdx(PosCurVert);

	if(IdxField == -1 || IdxSector == -1)
	{
		NTL_RETURN(FALSE);
	}

	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("fields");

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxField);
	::_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		NTL_RETURN(FALSE);
	}

	RwV3d	SPos = m_pFields[IdxField].GetSPos();
	RwV3d	SectorSPos;
	for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;

			if(GetSectorIdx(SectorSPos) == IdxSector)
			{
				_RpNtlWorldSectorReadMaterialDummy(pFile);
				::fread(pPosVertList, sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);
				::fclose(pFile);
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

				NTL_RETURN(TRUE);
			}
			else
			{
				_RpNtlWorldSectorReadDummy(pFile);
			}
		}
	}

	DBO_TRACE(FALSE, "CNtlWorldFieldManager::GetPosVertList, stream read failed.");

	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(FALSE);
}

void CNtlWorldFieldManager::OnSetSlopeLighting(RwUInt32 _IdxField)
{
	// exceptions
	if(!IsFieldValid(_IdxField))
	{
		DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid field index.(" << _IdxField << ")");		
		return;
	}

	// update slope lighting looping sectors
	RwV3d	SPos = m_pFields[_IdxField].GetSPos();
	RwV3d	PosSectorS;
	for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			PosSectorS.x = (RwReal)m;
			PosSectorS.z = (RwReal)l;
			OnSetSlopeLighting(GetSectorIdx(PosSectorS));
		}
	}
}

void CNtlWorldFieldManager::OnSetSlopeLighting(RwInt32 IdxSector)
{
	if(!IsSectorValid(IdxSector))
	{
		DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector index.(" << IdxSector << ")");
		return;
	}

	RwReal			LightPos0, LightPos1, Result;
	RwV3d			RetVert;
	RwV3d			TmpVert;
	RwV3d*			pVList;
	RwV3d			vSrcPos;
	RpAtomic*		pAtomic;
	RwRGBA*			pPrelights;
	RwInt32			NumVert;

	RwReal			TileSize			= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
	sNTL_WORLD_SL*	pNtlWorldSL			= NULL;
	RwBool			IsCurSectorLoaded	= IsSectorLoaded(IdxSector);

	if(IsCurSectorLoaded)
	{
		pNtlWorldSL	= m_pSectors[IdxSector].m_pNtlWorldSL;
		pAtomic		= m_pSectors[IdxSector].m_pAtomic;
		pVList		= RpMorphTargetGetVertices(pAtomic->geometry->morphTarget);
		pPrelights	= RpGeometryGetPreLightColors(pAtomic->geometry);
		NumVert		= RpGeometryGetNumVertices(pAtomic->geometry);

		::RpGeometryLock(pAtomic->geometry, rpGEOMETRYLOCKPRELIGHT);

		for(int i = 0; i < NumVert; ++i)
		{
			CNtlMath::MathRwV3dAssign(&vSrcPos, pVList[i].x, pVList[i].y, pVList->z);

			// 3---2---1
			// |       | 
			// 4  cur  0
			// |       |
			// 5---6---7

			switch(pNtlWorldSL->m_Dir)
			{
			case 0:
				{
					TmpVert.x = pVList[i].x - TileSize;
					TmpVert.z = pVList[i].z;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");		
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 1:
				{
					TmpVert.x = pVList[i].x - TileSize;
					TmpVert.z = pVList[i].z + TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 2:
				{
					TmpVert.x = pVList[i].x;
					TmpVert.z = pVList[i].z + TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 3:
				{
					TmpVert.x = pVList[i].x + TileSize;
					TmpVert.z = pVList[i].z + TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 4:
				{	
					TmpVert.x = pVList[i].x + TileSize;
					TmpVert.z = pVList[i].z;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 5:
				{
					TmpVert.x = pVList[i].x + TileSize;
					TmpVert.z = pVList[i].z - TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 6:
				{
					TmpVert.x = pVList[i].x;
					TmpVert.z = pVList[i].z - TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 7:
				{
					TmpVert.x = pVList[i].x - TileSize;
					TmpVert.z = pVList[i].z - TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;
			}

			TmpVert.x = pVList[i].x;
			TmpVert.z = pVList[i].z;

			if(!IsSectorValid(TmpVert))
			{
				DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
				continue;
			}

			if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
			{
				if(!GetVertFromFile(TmpVert, RetVert))
				{
					continue;
				}
			}
			else
			{
				if(!GetVertInMemory(TmpVert, RetVert))
				{
					continue;
				}
			}
			LightPos1 = RetVert.y;

			// calculate a slope lighting value
			Result = (1.0f - (LightPos0 - LightPos1) / pNtlWorldSL->m_Softness);
			CLAMP(Result, pNtlWorldSL->m_Brightness[0], pNtlWorldSL->m_Brightness[1]);

			RwReal ClrDiffuseR	= (RwReal)((m_pSectors[IdxSector].m_pClrDiffusePalette[i] & 0x00ff0000) >> 16) / 255.0f;
			RwReal ClrDiffuseG	= (RwReal)((m_pSectors[IdxSector].m_pClrDiffusePalette[i] & 0x0000ff00) >> 8) / 255.0f;
			RwReal ClrDiffuseB	= (RwReal)((m_pSectors[IdxSector].m_pClrDiffusePalette[i] & 0x000000ff) >> 0) / 255.0f;

			RwReal ClrSLR = (1.0f - pNtlWorldSL->_Clr.red) * Result + pNtlWorldSL->_Clr.red;
			RwReal ClrSLG = (1.0f - pNtlWorldSL->_Clr.green) * Result + pNtlWorldSL->_Clr.green;
			RwReal ClrSLB = (1.0f - pNtlWorldSL->_Clr.blue) * Result + pNtlWorldSL->_Clr.blue;

			if(static_cast<RwInt32>(pNtlWorldSL->_Clr.alpha))
			{
				(pPrelights[i]).red		= (RwUInt8)(ClrSLR * ClrDiffuseR * 255.0f);
				(pPrelights[i]).green	= (RwUInt8)(ClrSLG * ClrDiffuseG * 255.0f);
				(pPrelights[i]).blue	= (RwUInt8)(ClrSLB * ClrDiffuseB * 255.0f);
			}
			else
			{
				(pPrelights[i]).red		= (RwUInt8)(pNtlWorldSL->_Clr.red * ClrDiffuseR * 255.0f);
				(pPrelights[i]).green	= (RwUInt8)(pNtlWorldSL->_Clr.green * ClrDiffuseG * 255.0f);
				(pPrelights[i]).blue	= (RwUInt8)(pNtlWorldSL->_Clr.blue * ClrDiffuseB * 255.0f);
			}
		}

		::RpGeometryUnlock(pAtomic->geometry);
	}
	else
	{
		NumVert = dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;

		RwV3d*			pPosVertList	= NTL_NEW RwV3d [NumVert];
		RwRGBA*			pClrVertList	= NTL_NEW RwRGBA [NumVert];
		sNTL_WORLD_SL*	pNtlWorldSL		= NTL_NEW sNTL_WORLD_SL;
		DWORD*			pClrDiffuseList = NTL_NEW DWORD [NumVert];

		RwV3d PosSectorDatum;
		CNtlMath::MathRwV3dAssign(&PosSectorDatum, m_pSectors[IdxSector].DatumPoint.x, m_pSectors[IdxSector].DatumPoint.y, m_pSectors[IdxSector].DatumPoint.z);
		GetPosVertList(PosSectorDatum, pPosVertList);

		::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		::_chdir("swap");

		// Diffuse swap file
		::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.dif", IdxSector);

		FILE* pFile;
		if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			NTL_RETURNVOID();
		}

		::fread(pClrDiffuseList, sizeof(DWORD) * NumVert, 1, pFile);
		::fclose(pFile);

		// Slope lighting prop.
		::_chdir("sl");
		::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sl", IdxSector);

		if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			NTL_RETURNVOID();
		}

		::fread(pNtlWorldSL, sizeof(sNTL_WORLD_SL), 1, pFile);
		::fclose(pFile);

		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

		// cacluating slope lighting
		for(int i = 0; i < NumVert; ++i)
		{
			CNtlMath::MathRwV3dAssign(&vSrcPos, pPosVertList[i].x, pPosVertList[i].y, pPosVertList->z);

			// 3---2---1
			// |       | 
			// 4  cur  0
			// |       |
			// 5---6---7

			switch(pNtlWorldSL->m_Dir)
			{
			case 0:
				{
					TmpVert.x = pPosVertList[i].x - TileSize;
					TmpVert.z = pPosVertList[i].z;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 1:
				{
					TmpVert.x = pPosVertList[i].x - TileSize;
					TmpVert.z = pPosVertList[i].z + TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 2:
				{
					TmpVert.x = pPosVertList[i].x;
					TmpVert.z = pPosVertList[i].z + TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 3:
				{
					TmpVert.x = pPosVertList[i].x + TileSize;
					TmpVert.z = pPosVertList[i].z + TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 4:
				{	
					TmpVert.x = pPosVertList[i].x + TileSize;
					TmpVert.z = pPosVertList[i].z;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 5:
				{
					TmpVert.x = pPosVertList[i].x + TileSize;
					TmpVert.z = pPosVertList[i].z - TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 6:
				{
					TmpVert.x = pPosVertList[i].x;
					TmpVert.z = pPosVertList[i].z - TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;

			case 7:
				{
					TmpVert.x = pPosVertList[i].x - TileSize;
					TmpVert.z = pPosVertList[i].z - TileSize;

					if(!IsSectorValid(TmpVert))
					{
						DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
						continue;
					}

					if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
					{
						if(!GetVertFromFile(TmpVert, RetVert))
						{
							continue;
						}
					}
					else
					{
						if(!GetVertInMemory(TmpVert, RetVert))
						{
							continue;
						}
					}
					LightPos0 = RetVert.y;
				}
				break;
			}

			TmpVert.x = pPosVertList[i].x;
			TmpVert.z = pPosVertList[i].z;

			if(!IsSectorValid(TmpVert))
			{
				DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
				continue;
			}

			if(!IsSectorLoaded(GetSectorIdx(TmpVert)))
			{
				if(!GetVertFromFile(TmpVert, RetVert))
				{
					continue;
				}
			}
			else
			{
				if(!GetVertInMemory(TmpVert, RetVert))
				{
					continue;
				}
			}
			LightPos1 = RetVert.y;

			// calculate a slope lighting value
			Result = (1.0f - (LightPos0 - LightPos1) / pNtlWorldSL->m_Softness);
			CLAMP(Result, pNtlWorldSL->m_Brightness[0], pNtlWorldSL->m_Brightness[1]);

			RwReal ClrDiffuseR	= (RwReal)((pClrDiffuseList[i] & 0x00ff0000) >> 16) / 255.0f;
			RwReal ClrDiffuseG	= (RwReal)((pClrDiffuseList[i] & 0x0000ff00) >> 8) / 255.0f;
			RwReal ClrDiffuseB	= (RwReal)((pClrDiffuseList[i] & 0x000000ff) >> 0) / 255.0f;

			RwReal ClrSLR = (1.0f - pNtlWorldSL->_Clr.red) * Result + pNtlWorldSL->_Clr.red;
			RwReal ClrSLG = (1.0f - pNtlWorldSL->_Clr.green) * Result + pNtlWorldSL->_Clr.green;
			RwReal ClrSLB = (1.0f - pNtlWorldSL->_Clr.blue) * Result + pNtlWorldSL->_Clr.blue;

			if(static_cast<RwInt32>(pNtlWorldSL->_Clr.alpha))
			{
				(pClrVertList[i]).red	= (RwUInt8)(ClrSLR * ClrDiffuseR * 255.0f);
				(pClrVertList[i]).green	= (RwUInt8)(ClrSLG * ClrDiffuseG * 255.0f);
				(pClrVertList[i]).blue	= (RwUInt8)(ClrSLB * ClrDiffuseB * 255.0f);
			}
			else
			{
				(pClrVertList[i]).red	= (RwUInt8)(pNtlWorldSL->_Clr.red * ClrDiffuseR * 255.0f);
				(pClrVertList[i]).green	= (RwUInt8)(pNtlWorldSL->_Clr.green * ClrDiffuseG * 255.0f);
				(pClrVertList[i]).blue	= (RwUInt8)(pNtlWorldSL->_Clr.blue * ClrDiffuseB * 255.0f);
			}
		}

		SetClrVertList(PosSectorDatum, pClrVertList);

		NTL_ARRAY_DELETE(pPosVertList);
		NTL_ARRAY_DELETE(pClrVertList);
		NTL_DELETE(pNtlWorldSL);
		NTL_ARRAY_DELETE(pClrDiffuseList);
	}
}

void CNtlWorldFieldManager::OnSetSlopeLighting(vector<sTARGET_VERT_SL_INFO>& vecTargetVertSLInfo)
{
	for(int j = 0; j < (int)vecTargetVertSLInfo.size(); ++j)
	{
		RpAtomic	*pCurAtomic = vecTargetVertSLInfo[j].pNtlWorldSector->m_pAtomic;
		RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
		RwRGBA		*pPrelights = RpGeometryGetPreLightColors(pCurAtomic->geometry);
		RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
		RwInt32		IdxVert		= vecTargetVertSLInfo[j].IdxVert;
		RwReal		PosX		= pVList[IdxVert].x;
		RwReal		PosZ		= pVList[IdxVert].z;
		RwInt32		TileSize	= dGET_WORLD_PARAM()->WorldSectorTileSize;
		RwReal		LightPos0, LightPos1, Result;

		RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKPRELIGHT);
		{
			//
			// 3---2---1
			// |       | 
			// 4  cur  0
			// |       |
			// 5---6---7
			// 

			RwV3d			RetVert;
			RwV3d			TmpVert;
			sNTL_WORLD_SL*	pNtlWorldSL = vecTargetVertSLInfo[j].pNtlWorldSector->m_pNtlWorldSL;

			if(pNtlWorldSL->m_Dir == 0)
			{
				TmpVert.x = pVList[IdxVert].x - TileSize;
				TmpVert.z = pVList[IdxVert].z;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 1)
			{
				TmpVert.x = pVList[IdxVert].x - TileSize;
				TmpVert.z = pVList[IdxVert].z + TileSize;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 2)
			{
				TmpVert.x = pVList[IdxVert].x;
				TmpVert.z = pVList[IdxVert].z + TileSize;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 3)
			{
				TmpVert.x = pVList[IdxVert].x + TileSize;
				TmpVert.z = pVList[IdxVert].z + TileSize;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 4)
			{
				TmpVert.x = pVList[IdxVert].x + TileSize;
				TmpVert.z = pVList[IdxVert].z;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 5)
			{
				TmpVert.x = pVList[IdxVert].x + TileSize;
				TmpVert.z = pVList[IdxVert].z - TileSize;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 6)
			{
				TmpVert.x = pVList[IdxVert].x;
				TmpVert.z = pVList[IdxVert].z - TileSize;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}
			else if(pNtlWorldSL->m_Dir == 7)
			{
				TmpVert.x = pVList[IdxVert].x - TileSize;
				TmpVert.z = pVList[IdxVert].z - TileSize;

				if(!IsSectorValid(TmpVert))
				{
					DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
					continue;
				}

				if(!GetVertInMemory(TmpVert, RetVert))
					continue;
				LightPos0 = RetVert.y;
			}

			TmpVert.x = pVList[IdxVert].x;
			TmpVert.z = pVList[IdxVert].z;

			if(!IsSectorValid(TmpVert))
			{
				DBO_TRACE(FALSE, "CNtlWorldFieldManager::OnSetSlopeLighting, invalid sector pos.");
				continue;
			}

			if(!GetVertInMemory(TmpVert, RetVert))
				continue;
			LightPos1 = RetVert.y;

			// calculate a slope lighting value
			Result = (1.0f - (LightPos0 - LightPos1) / pNtlWorldSL->m_Softness);
			CLAMP(Result, pNtlWorldSL->m_Brightness[0], pNtlWorldSL->m_Brightness[1]);

			RwReal ClrDiffuseR	= (RwReal)((vecTargetVertSLInfo[j].pNtlWorldSector->m_pClrDiffusePalette[IdxVert] & 0x00ff0000) >> 16) / 255.0f;
			RwReal ClrDiffuseG	= (RwReal)((vecTargetVertSLInfo[j].pNtlWorldSector->m_pClrDiffusePalette[IdxVert] & 0x0000ff00) >> 8) / 255.0f;
			RwReal ClrDiffuseB	= (RwReal)((vecTargetVertSLInfo[j].pNtlWorldSector->m_pClrDiffusePalette[IdxVert] & 0x000000ff) >> 0) / 255.0f;

			RwReal ClrSLR = (1.0f - pNtlWorldSL->_Clr.red) * Result + pNtlWorldSL->_Clr.red;
			RwReal ClrSLG = (1.0f - pNtlWorldSL->_Clr.green) * Result + pNtlWorldSL->_Clr.green;
			RwReal ClrSLB = (1.0f - pNtlWorldSL->_Clr.blue) * Result + pNtlWorldSL->_Clr.blue;

			if(static_cast<RwInt32>(pNtlWorldSL->_Clr.alpha))
			{
				(pPrelights[IdxVert]).red	= (RwUInt8)(ClrSLR * ClrDiffuseR * 255.0f);
				(pPrelights[IdxVert]).green	= (RwUInt8)(ClrSLG * ClrDiffuseG * 255.0f);
				(pPrelights[IdxVert]).blue	= (RwUInt8)(ClrSLB * ClrDiffuseB * 255.0f);
			}
			else
			{
				(pPrelights[IdxVert]).red	= (RwUInt8)(pNtlWorldSL->_Clr.red * ClrDiffuseR * 255.0f);
				(pPrelights[IdxVert]).green	= (RwUInt8)(pNtlWorldSL->_Clr.green * ClrDiffuseG * 255.0f);
				(pPrelights[IdxVert]).blue	= (RwUInt8)(pNtlWorldSL->_Clr.blue * ClrDiffuseB * 255.0f);
			}
		}
		RpGeometryUnlock(pCurAtomic->geometry);
	}

	vecTargetVertSLInfo.clear();
}

#endif

RwBool CNtlWorldFieldManager::SetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp, eNTL_FIELD_PROPID NtlFieldPropID)
{
	/*
	WorldFileFormat - FieldProperty
	Field Property를 File 또는 Memory 정보를 변경한다.
	*/

	RwInt32 NumSectorTile	= (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2);
	RwInt32 Idx				= GetFieldIdx(Pos);


	if(Idx == -1)
	{
		return FALSE;
	}
	else
	{
		if(!IsFieldLoaded(Idx))
		{
			_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
			_chdir("fields");

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", Idx);
			_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

			FILE *pFile = NULL;
			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", Idx);
			if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb+") != 0)
			{
				DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
				return FALSE;
			}
			else
			{					
				switch(NtlFieldPropID)
				{
				case eNFP_NAME:
					{			
						::fwrite(NtlFieldProp._Name, 64, 1, pFile);

						break;
					}
				case eNFP_BITPROP:
					{
						::fseek(pFile, 64, SEEK_CUR);
						::fwrite(NtlFieldProp._pBitProp, sizeof(DWORD) * NumSectorTile, 1, pFile);

						break;
					}
				case eNFP_FOG:
					{
						::fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile, SEEK_CUR);
						::fwrite(&NtlFieldProp._FogColor, sizeof(RwRGBA), 1, pFile);
						::fwrite(&NtlFieldProp._FogCamPlane[0], sizeof(RwReal), 1, pFile);
						::fwrite(&NtlFieldProp._FogCamPlane[1], sizeof(RwReal), 1, pFile);

						break;
					}
				case eNFP_SKY:
					{
						::fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal), SEEK_CUR);
						::fwrite(&NtlFieldProp._BaseSkyMode, sizeof(RwInt32), 1, pFile);
						::fwrite(&NtlFieldProp._RGBSkyColor[0], sizeof(RwRGBA), 1, pFile);
						::fwrite(&NtlFieldProp._RGBSkyColor[1], sizeof(RwRGBA), 1, pFile);
						::fwrite(&NtlFieldProp._RGBSkyColor[2], sizeof(RwRGBA), 1, pFile);

						::fwrite(&NtlFieldProp._NewSkyValue, sizeof(RwReal), 1, pFile);

						::fwrite(NtlFieldProp._BaseSkyTexName, sizeof(RwChar) * 10, 1, pFile);
						::fwrite(&NtlFieldProp._BaseSkySpeed, sizeof(RwReal), 1, pFile);
						::fwrite(NtlFieldProp._BlendedTexName[0], sizeof(RwChar) * 10, 1, pFile);
						::fwrite(NtlFieldProp._BlendedTexName[1], sizeof(RwChar) * 10, 1, pFile);
						::fwrite(&NtlFieldProp._BlendedTexSpeed[0], sizeof(RwReal), 1, pFile);
						::fwrite(&NtlFieldProp._BlendedTexSpeed[1], sizeof(RwReal), 1, pFile);

						break;
					}
				case eNFP_LIGHT:
					{
						::fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2), SEEK_CUR);
						::fwrite(&NtlFieldProp._ClrLightAmbient, sizeof(RwRGBAReal), 1, pFile);
						for (int i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
						{
							::fwrite(&NtlFieldProp._ClrLightDirectional[i], sizeof(RwRGBAReal), 1, pFile);
							::fwrite(&NtlFieldProp._AngLightDirectional[i], sizeof(RwV3d), 1, pFile);
						}

						break;
					}
				case eNFP_SOUND:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3), SEEK_CUR);
						fwrite(&NtlFieldProp._IdxBGMTbl, sizeof(RwInt32), 1, pFile);
						fwrite(&NtlFieldProp._IdxEnvTbl, sizeof(RwInt32), 1, pFile);
						for (int i = 0; i < 5; ++i)
						{
							fwrite(&NtlFieldProp._IdxShareTbl[i], sizeof(RwInt32), 1, pFile);
						}

						break;
					}
				case eNFP_BLOOM:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + (sizeof(RwInt32) * 5), SEEK_CUR);
						fwrite(&NtlFieldProp._MonoPower, sizeof(RwReal), 1, pFile);

						break;
					}
				case eNFP_PLANET:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) + (sizeof(RwInt32) * 5), SEEK_CUR);

						for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
							fwrite(&NtlFieldProp._NtlPlanet[i], sizeof(sNTL_PLANET), 1, pFile);

						break;
					}
				case eNFP_SPECULAR:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) + (sizeof(sNTL_PLANET) * 3) + (sizeof(RwInt32) * 5), SEEK_CUR);

						fwrite(&NtlFieldProp._NtlSpecular, sizeof(sNTL_SPECULAR), 1, pFile);

						break;
					}
				case eNFP_MATERIALPROP:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) + (sizeof(sNTL_PLANET) * 3) + sizeof(sNTL_SPECULAR) + (sizeof(RwInt32) * 5), SEEK_CUR);

						fwrite(NtlFieldProp._pMaterialProp, sizeof(BYTE) * NumSectorTile, 1, pFile);

						break;
					}
				case eNFP_FIELDCOLOR:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) + (sizeof(sNTL_PLANET) * 3) + sizeof(sNTL_SPECULAR) + (sizeof(BYTE) * NumSectorTile) + (sizeof(RwInt32) * 5), SEEK_CUR);

						fwrite(&NtlFieldProp._FieldColor, sizeof(RwRGBA), 1, pFile);

						break;
					}
				case eNFP_WEATHER:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) + (sizeof(sNTL_PLANET) * 3) + sizeof(sNTL_SPECULAR) + (sizeof(BYTE) * NumSectorTile) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5), SEEK_CUR);

						fwrite(&NtlFieldProp._IdxWeather, sizeof(RwInt32) * 5, 1, pFile);

						break;
					}
				case eNFP_FIELDHEATHAZE:
					{
						fseek(pFile, 64 + sizeof(DWORD) * NumSectorTile + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + (sizeof(RwRGBA) * 3) + (sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10 * 2) + (sizeof(RwReal) * 2) + (sizeof(RwRGBAReal) * 4) + (sizeof(RwV3d) * 3) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) + (sizeof(sNTL_PLANET) * 3) + sizeof(sNTL_SPECULAR) + (sizeof(BYTE) * NumSectorTile) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + (sizeof(RwInt32) * 5), SEEK_CUR);

						fwrite(&NtlFieldProp._NtlHeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);
					}

				}

				::fclose(pFile);
			}

			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

			SetAnotherField();

			return TRUE;
		}
		else
		{
			switch(NtlFieldPropID)
			{
			case eNFP_NAME:
				{
					strcpy_s(m_pFields[Idx].GetProp()->_Name, NtlFieldProp._Name);

					break;
				}
			case eNFP_BITPROP:
				{
					::CopyMemory(m_pFields[Idx].GetProp()->_pBitProp, NtlFieldProp._pBitProp, sizeof(DWORD) * NumSectorTile);

					break;
				}
			case eNFP_FOG:
				{
					m_pFields[Idx].GetProp()->_FogColor.red		= NtlFieldProp._FogColor.red;
					m_pFields[Idx].GetProp()->_FogColor.green	= NtlFieldProp._FogColor.green;
					m_pFields[Idx].GetProp()->_FogColor.blue	= NtlFieldProp._FogColor.blue;
					m_pFields[Idx].GetProp()->_FogCamPlane[0]	= NtlFieldProp._FogCamPlane[0];
					m_pFields[Idx].GetProp()->_FogCamPlane[1]	= NtlFieldProp._FogCamPlane[1];	

					break;
				}
			case eNFP_SKY:
				{
					m_pFields[Idx].GetProp()->_BaseSkyMode	= NtlFieldProp._BaseSkyMode;
					memcpy(&m_pFields[Idx].GetProp()->_RGBSkyColor[0], &NtlFieldProp._RGBSkyColor[0], sizeof(RwRGBA));
					memcpy(&m_pFields[Idx].GetProp()->_RGBSkyColor[1], &NtlFieldProp._RGBSkyColor[1], sizeof(RwRGBA));
					memcpy(&m_pFields[Idx].GetProp()->_RGBSkyColor[2], &NtlFieldProp._RGBSkyColor[2], sizeof(RwRGBA));

					m_pFields[Idx].GetProp()->_NewSkyValue = NtlFieldProp._NewSkyValue; // new

					strcpy_s(m_pFields[Idx].GetProp()->_BaseSkyTexName, NtlFieldProp._BaseSkyTexName);
					m_pFields[Idx].GetProp()->_BaseSkySpeed = NtlFieldProp._BaseSkySpeed;
					strcpy_s(m_pFields[Idx].GetProp()->_BlendedTexName[0], NtlFieldProp._BlendedTexName[0]);
					strcpy_s(m_pFields[Idx].GetProp()->_BlendedTexName[1], NtlFieldProp._BlendedTexName[1]);
					m_pFields[Idx].GetProp()->_BlendedTexSpeed[0] = NtlFieldProp._BlendedTexSpeed[0];
					m_pFields[Idx].GetProp()->_BlendedTexSpeed[1] = NtlFieldProp._BlendedTexSpeed[1];

					break;
				}
			case eNFP_LIGHT:
				{
					::CopyMemory(&m_pFields[Idx].GetProp()->_ClrLightAmbient, &NtlFieldProp._ClrLightAmbient, sizeof(RwRGBAReal));
					for (int i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
					{
						::CopyMemory(&m_pFields[Idx].GetProp()->_ClrLightDirectional[i], &NtlFieldProp._ClrLightDirectional[i], sizeof(RwRGBAReal));
						::CopyMemory(&m_pFields[Idx].GetProp()->_AngLightDirectional[i], &NtlFieldProp._AngLightDirectional[i], sizeof(RwV3d));
					}
					break;
				}
			case eNFP_SOUND:
				{
					m_pFields[Idx].GetProp()->_IdxBGMTbl = NtlFieldProp._IdxBGMTbl;
					m_pFields[Idx].GetProp()->_IdxEnvTbl = NtlFieldProp._IdxEnvTbl;
					for (int i = 0; i < 5; ++i)
					{
						m_pFields[Idx].GetProp()->_IdxShareTbl[i] = NtlFieldProp._IdxShareTbl[i];
					}
					break;
				}

			case eNFP_BLOOM:
				{
					m_pFields[Idx].GetProp()->_MonoPower = NtlFieldProp._MonoPower;

					break;
				}
			case eNFP_PLANET:
				{
					m_pFields[Idx].GetProp()->_NtlPlanet[0] = NtlFieldProp._NtlPlanet[0];
					m_pFields[Idx].GetProp()->_NtlPlanet[1] = NtlFieldProp._NtlPlanet[1];
					m_pFields[Idx].GetProp()->_NtlPlanet[2] = NtlFieldProp._NtlPlanet[2];

					break;
				}
			case eNFP_SPECULAR:
				{					
					m_pFields[Idx].GetProp()->_NtlSpecular = NtlFieldProp._NtlSpecular;

					break;
				}
			case eNFP_MATERIALPROP:
				{
					::CopyMemory(m_pFields[Idx].GetProp()->_pMaterialProp, NtlFieldProp._pMaterialProp, sizeof(BYTE) * NumSectorTile);

					break;
				}
			case eNFP_FIELDCOLOR:
				{
					m_pFields[Idx].GetProp()->_FieldColor = NtlFieldProp._FieldColor;

					break;
				}
			case eNFP_WEATHER:
				{
					for (int i = 0; i < 5; ++i)
					{
						m_pFields[Idx].GetProp()->_IdxWeather[i] = NtlFieldProp._IdxWeather[i];
					}

					break;
				}			
			case eNFP_FIELDHEATHAZE:
				{
					m_pFields[Idx].GetProp()->_NtlHeatHaze = NtlFieldProp._NtlHeatHaze;

					break;
				}
			}

			SetAnotherField();

			return TRUE;
		}	
	}

	return FALSE;
}

RwBool CNtlWorldFieldManager::GetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp)
{
	/*
	WorldFileFormat - FieldProperty
	Field Property를 File 또는 Memory로 부터 정보를 가져온다.
	*/

	RwInt32 Idx = GetFieldIdx(Pos);
	if(Idx == -1)
	{
		return FALSE;
	}
	else
	{
		if(!IsFieldLoaded(Idx))
		{
			FILE *pFile = NULL;
			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", Idx);
			if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
			{
				static RwChar chPackPatch[NTL_MAX_DIR_PATH];
				strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
				strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "fields");
				strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
				strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);
				strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");

				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", Idx);

				strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				SPackResFileData sPackFileData;
				RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(chPackPatch, sPackFileData);
				if(bPack)
				{
					_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
					::fopen_s(&pFile, sPackFileData.strPackFileName.c_str(), "rb");
					if(pFile)
					{
						fseek(pFile, sPackFileData.uiOffset, SEEK_SET);
					}
				}
			}
			else
			{
				_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
				_chdir("fields");		
				_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", Idx);
				::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
				_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			}

			if(!pFile)
			{
				DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
				return FALSE;
			}
			else
			{
				fread(NtlFieldProp._Name, 64, 1, pFile);
				fread(NtlFieldProp._pBitProp, sizeof(DWORD) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
				fread(&NtlFieldProp._FogColor, sizeof(RwRGBA), 1, pFile);
				fread(&NtlFieldProp._FogCamPlane[0], sizeof(RwReal), 1, pFile);
				fread(&NtlFieldProp._FogCamPlane[1], sizeof(RwReal), 1, pFile);
				fread(&NtlFieldProp._BaseSkyMode, sizeof(RwInt32), 1, pFile);
				fread(&NtlFieldProp._RGBSkyColor[0], sizeof(RwRGBA), 1, pFile);
				fread(&NtlFieldProp._RGBSkyColor[1], sizeof(RwRGBA), 1, pFile);
				fread(&NtlFieldProp._RGBSkyColor[2], sizeof(RwRGBA), 1, pFile);

				fread(&NtlFieldProp._NewSkyValue, sizeof(RwReal), 1, pFile);

				fread(NtlFieldProp._BaseSkyTexName, sizeof(RwChar) * 10, 1, pFile);
				fread(&NtlFieldProp._BaseSkySpeed, sizeof(RwReal), 1, pFile);
				fread(NtlFieldProp._BlendedTexName[0], sizeof(RwChar) * 10, 1, pFile);
				fread(NtlFieldProp._BlendedTexName[1], sizeof(RwChar) * 10, 1, pFile);
				fread(&NtlFieldProp._BlendedTexSpeed[0], sizeof(RwReal), 1, pFile);
				fread(&NtlFieldProp._BlendedTexSpeed[1], sizeof(RwReal), 1, pFile);
				fread(&NtlFieldProp._ClrLightAmbient, sizeof(RwRGBAReal), 1, pFile);

				for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
				{
					fread(&NtlFieldProp._ClrLightDirectional[i], sizeof(RwRGBAReal), 1, pFile);
					fread(&NtlFieldProp._AngLightDirectional[i], sizeof(RwV3d), 1, pFile);
					//break;
				}				

				fread(&NtlFieldProp._IdxBGMTbl, sizeof(RwInt32), 1, pFile);
				fread(&NtlFieldProp._IdxEnvTbl, sizeof(RwInt32), 1, pFile);
				for(RwInt32 i = 0; i < 5; ++i)
				{
					fread(&NtlFieldProp._IdxShareTbl[i], sizeof(RwInt32), 1, pFile);
				}

				fread(&NtlFieldProp._MonoPower, sizeof(RwReal), 1, pFile);

				for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
					fread(&NtlFieldProp._NtlPlanet[i], sizeof(sNTL_PLANET), 1, pFile);

				fread(&NtlFieldProp._NtlSpecular, sizeof(sNTL_SPECULAR), 1, pFile);
				fread(NtlFieldProp._pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
				fread(&NtlFieldProp._FieldColor, sizeof(RwRGBA), 1, pFile);
				fread(&NtlFieldProp._IdxWeather, sizeof(RwInt32) * 5, 1, pFile);
				fread(&NtlFieldProp._NtlHeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);

				fclose(pFile);
			}

			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

			return TRUE;
		}
		else
		{
			NtlFieldProp = *m_pFields[Idx].GetProp();

			return TRUE;
		}	
	}

	return FALSE;
}

DWORD CNtlWorldFieldManager::GetMapTileProp(RwV3d& Pos)
{
	RwInt32 FieldIdx = GetFieldIdx(Pos);
	if(FieldIdx == -1)
	{
		return 0xffffffff;
	}
	else
	{
		RwV3d	SPos		= m_pFields[FieldIdx].GetSPos();
		RwInt32	XMoveCnt	= static_cast<RwInt32>(abs(Pos.x - SPos.x)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
		RwInt32	ZMoveCnt	= static_cast<RwInt32>(abs(Pos.z - SPos.z)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
		RwInt32 PosBitFlag	= XMoveCnt + (ZMoveCnt * (dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldFieldSectorNum));

		if(!m_pFields[FieldIdx].GetProp())
		{
			return 0xffffffff;
		}

		return m_pFields[FieldIdx].GetProp()->_pBitProp[PosBitFlag];
	}

	return 0xffffffff;
}

BYTE CNtlWorldFieldManager::GetMaterialTileProp(RwV3d& Pos)
{
	RwInt32 FieldIdx = GetFieldIdx(Pos);
	if(FieldIdx == -1)
	{
		return 0;
	}
	else
	{
		RwV3d	SPos		= m_pFields[FieldIdx].GetSPos();
		RwInt32	XMoveCnt	= static_cast<RwInt32>(abs(Pos.x - SPos.x)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
		RwInt32	ZMoveCnt	= static_cast<RwInt32>(abs(Pos.z - SPos.z)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
		RwInt32 Index		= XMoveCnt + (ZMoveCnt * (dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldFieldSectorNum));

		if(!m_pFields[FieldIdx].GetProp())
		{
			return 0;
		}

		return m_pFields[FieldIdx].GetProp()->_pMaterialProp[Index];
	}

	return 0;

}

RwReal CNtlWorldFieldManager::GetWaterHeight(RwV3d& Pos)
{
	RwInt32 SectorIdx = GetSectorIdx(Pos);
	if(SectorIdx == -1)
	{
		return -9999.0f;
	}
	else
	{
		if(!m_pSectors[SectorIdx].m_pWater)
		{
			return -9999.0f;
		}
		else
		{
			return m_pSectors[SectorIdx].m_pWater->_Height;
		}
	}

	return -9999.0f;
}

RwBool CNtlWorldFieldManager::GetFieldSectorIndices(RwInt32 _FieldIdx, RwInt32 _FieldSectorIndices[4])
{
	if(_FieldIdx == -1)
	{
		return FALSE;
	}

	RwV3d Pos = m_pFields[_FieldIdx].GetSPos();
	RwV3d Tmp;

	// 
	//  3 2
	//  1 0
	//
	_FieldSectorIndices[0] = GetSectorIdx(Pos);

	Tmp = Pos;
	Tmp.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	_FieldSectorIndices[1] = GetSectorIdx(Tmp);

	Tmp = Pos;
	Tmp.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	_FieldSectorIndices[2] = GetSectorIdx(Tmp);

	Tmp = Pos;
	Tmp.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	Tmp.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	_FieldSectorIndices[3] = GetSectorIdx(Tmp);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UpdateFieldMap : update field map based on Pos
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RwBool CNtlWorldFieldManager::UpdateFieldMap(RwV3d& Pos)
{
	// assert cur field is valid
	RwInt32 CurFieldIdx = GetFieldIdx(Pos);
	if(CurFieldIdx == -1)
		return FALSE;

	m_NewDatumIdx = CurFieldIdx;

	if(m_eMoved2 != eC && m_eMoved2 != ePORTAL)
		UpdateLODAttr(Pos);

	// Is there transition?
	if(m_OldDatumIdx == m_NewDatumIdx)
	{
		if(m_eMoved2 != eC && m_eMoved2 != ePORTAL)
		{
			return TRUE;
		}
	}

	// update m_eMoved2 out
	UpdateDatumDir();

	// update neighbor fields
	UpdateNeighborFields(Pos);

	// there might be another field coming up
	SetAnotherField();

	// update old datum index
	m_OldDatumIdx = m_NewDatumIdx;

	// send msgs right after another fields
	sNTL_FIELD_PROP NtlFieldProp;
	GetAFieldProp(Pos, NtlFieldProp);
	CNtlPLEventGenerator::IsAnotherFieldChangedWithoutDelay(&NtlFieldProp);

	return TRUE;
}

void CNtlWorldFieldManager::UpdateAnotherField()
{
	if(m_FieldPropVariationRestTime > 0)
	{
		m_FieldPropVariationRestTime -= g_GetElapsedTime();
		if(m_FieldPropVariationRestTime < 0)
		{
			m_FieldPropVariationRestTime = -999.0f;
			m_FieldPropVariationStarting = TRUE;
		}
	}
}

RwBool CNtlWorldFieldManager::GetFieldPropVariationStarting()
{
	if(m_FieldPropVariationStarting)
	{
		m_FieldPropVariationStarting = FALSE;
		return TRUE;
	}

	return FALSE;
}

void CNtlWorldFieldManager::SetAnotherField(RwBool ChangeStraightAway/* = FALSE*/)
{
	if(ChangeStraightAway)
	{
		m_FieldPropVariationRestTime = 0.01f;
	}
	else
	{
		if(m_eMoved2 == ePORTAL || m_eMoved2 == eIDLE)
		{
			m_FieldPropVariationRestTime = 0.01f;
		}
		else
		{
			m_FieldPropVariationRestTime = dFIELD_EFFECT_SWITCHING_TIME;
		}
	}
}

RwBool CNtlWorldFieldManager::IsFieldLoaded(RwInt32 FieldIdx)
{
	if(FieldIdx == -1)
	{
		return FALSE;
	}

	if(CNtlWorldSectorManager::IsSectorLoaded(CNtlWorldSectorManager::GetSectorIdx(m_pFields[FieldIdx].GetSPos())))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#ifdef dNTL_WORLD_TOOL_MODE

void CNtlWorldFieldManager::SaveSwapFile(RwInt32 FieldIdx)
{
	int l, m;
	int SectorIdx;
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	RwV3d SectorSPos;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);
			m_pSectors[SectorIdx].SaveSwapFile();
		}
	}
}

#endif

RwBool CNtlWorldFieldManager::SaveField(RwInt32 FieldIdx)
{
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	if(!IsFieldLoaded(FieldIdx))
	{
		CreateFields(FieldIdx);
		DeleteFields(FieldIdx);
	}
	else
	{
		DeleteFieldIntoFile(FieldIdx);

#ifdef dNTL_WORLD_TOOL_MODE
		{
			SaveSwapFile(FieldIdx);
		}
#endif
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldFieldManager::SaveCurFields()
{
	NTL_FUNCTION("CNtlWorldFieldManager::SaveCurFields");

#ifdef dNTL_WORLD_TOOL_MODE

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");

	for(RwInt32 i = 0; i < 36; ++i)
	{
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_Fields6x6[0][i]);
		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		DeleteFieldIntoFile(m_Fields6x6[1][i]);

#ifdef dNTL_WORLD_TOOL_MODE
		{
			if(m_Fields6x6[1][i] != -1)
			{
				SaveSwapFile(m_Fields6x6[1][i]);
			}
			else
			{
				DBO_TRACE(FALSE, "CNtlWorldFieldManager::SaveCurFields, invalid index");
			}
		}
#endif

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("fields");
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

#endif

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::SaveCurFieldsBeforeExit()
{
	NTL_FUNCTION("CNtlWorldFieldManager::SaveCurFieldsBeforeExit");

	// old source
	// 	if(!GetWorldReady())
	// 	{
	// 		return FALSE;
	// 	}
	// 
	// #ifdef dNTL_WORLD_TOOL_MODE
	// 
	// 	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	// 	_chdir("fields");
	// 
	// 	for(RwUInt32 i = 0; i < 36; ++i)
	// 	{
	// 		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_Fields6x6[1][i]);
	// 		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	// 
	// 		DeleteFieldIntoFile(m_Fields6x6[1][i]);
	// 		//DeleteFieldInMemory(m_Fields6x6[1][i]);
	// 
	// 		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	// 		_chdir("fields");
	// 	}
	// 
	// 	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	// 
	// 	// lightmaps
	// 	if(CNtlWorldLTManager::GetInstance() && GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	// 	{
	// 		CNtlWorldLTManager::GetInstance()->SaveBSP();
	// 		CNtlWorldLTManager::GetInstance()->Destroy();
	// 		CNtlWorldLTManager::DestroyInstance();
	// 	}
	// 
	// 	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	// 	_chdir("fields");
	// 
	// 	for(RwUInt32 i = 0; i < 36; ++i)
	// 	{
	// 		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_Fields6x6[1][i]);
	// 		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	// 
	// 		DeleteFieldInMemory(m_Fields6x6[1][i]);
	// 
	// 		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	// 		_chdir("fields");
	// 	}
	// 
	// 	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	// 
	// 	SaveCurWorldState();
	// #else
	// 	
	// 	for(RwInt32 i = 0; i < 36; ++i)
	// 	{
	// 		DeleteFieldInMemory(m_Fields6x6[1][i]);
	// 	}
	// #endif

	//	NTL_RETURN(TRUE);

	if(!GetWorldReady())
	{
		return FALSE;
	}

#ifdef dNTL_WORLD_TOOL_MODE
	SaveCurWorldState();
#endif

	// FieldManager를 초기화 하는 거나 마찬가지로 m_eMove2를 eC로 변경 WorldEeady 상태를 해제 한다.
	m_eMoved2 = eC;
	for(RwInt32 i = 0; i < 36; ++i)
	{
		DeleteFields(m_Fields6x6[1][i]);
	}

#ifdef dNTL_WORLD_SCHEDULE_LOADING
	// 스케쥴러를 사용 중이라면, 아직 삭제되지 않은 Field가 있을 수 있다. WorldScheduler를 끝까지 돌린다.
	RwInt32 iCzTestCnt = 0;
	while (m_WorldScheduler.Scheduling(1.0f, m_WorldScheduler.GetLastPos()))
	{
		++iCzTestCnt;
	}
#endif
	NTL_RETURN(TRUE);
}

VOID CNtlWorldFieldManager::DeleteFiles4Field(RwInt32 FieldIdx)
{
	if(FieldIdx == -1)
	{
		DBO_TRACE(FALSE, "CNtlWorldFieldManager::DeleteFiles4Field, invalid index");
	}

	// delete WFIF(World Field Information File), WFPF(World Field Property File) and the folder
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", FieldIdx);
	CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);

	_chdir("../");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_rmdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	// delete swap files
	_chdir("../");
	_chdir("swap");

	int l, m;
	int SectorIdx;
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	RwV3d SectorSPos;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.dif", SectorIdx);
			CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sdw", SectorIdx);
			CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);
		}
	}

	_chdir("sl");

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sl", SectorIdx);
			CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);
		}
	}

	_chdir("../");
	_chdir("hho");

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.hho", SectorIdx);
			CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);
		}
	}

	_chdir("../");
	_chdir("occ");

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.ocq", SectorIdx);
			CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);
		}
	}

	_chdir("../");
	_chdir("pe");

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.pe", SectorIdx);
			CFileController::DeleteFile(dGET_WORLD_PARAM()->WorldChar64Buf);
		}
	}
	CFileController::DeleteFile("header.pe");
}

RwBool CNtlWorldFieldManager::GetNeighborFields(RwInt32 _IdxCurField, vector<RwInt32>& _vecNeighborFields)
{
	// exceptions
	if(!IsFieldValid(_IdxCurField))
	{
		DBO_TRACE(FALSE, "CNtlWorldFieldManager::GetNeighborFields, invalid field index");
		return FALSE;
	}

	// reserve 8 space for indices
	_vecNeighborFields.reserve(8);

	// calculate neighbor fields
	RwV3d	PosCurFieldS = m_pFields[_IdxCurField].GetSPos();
	RwV3d	PosTmpFieldS;

	// 3---2---1
	// |       | 
	// 4   8   0
	// |       |
	// 5---6---7

	// 0
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.x		-=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 1
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.x		-=	dGET_WORLD_PARAM()->WorldFieldSize;
	PosTmpFieldS.z		+=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 2
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.z		+=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 3
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.x		+=	dGET_WORLD_PARAM()->WorldFieldSize;
	PosTmpFieldS.z		+=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 4
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.x		+=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 5
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.x		+=	dGET_WORLD_PARAM()->WorldFieldSize;
	PosTmpFieldS.z		-=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 6
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.z		-=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	// 7
	PosTmpFieldS		=	PosCurFieldS;
	PosTmpFieldS.x		-=	dGET_WORLD_PARAM()->WorldFieldSize;
	PosTmpFieldS.z		-=	dGET_WORLD_PARAM()->WorldFieldSize;
	_vecNeighborFields.push_back(GetFieldIdx(PosTmpFieldS));

	return TRUE;
}

void CNtlWorldFieldManager::SetVisiblePlanet(RwBool bVisible)
{
	m_pPlanetHandler->SetVisible(bVisible);
}

VOID CNtlWorldFieldManager::SetPVSActivation(RwBool _Flag)
{
	RwV3d	CurPos		= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwInt32	CurSecIdx	= GetSectorIdx(CurPos);

	if(_Flag)
	{
		RefreshOldSectorPVS(m_OldSectorIdx4PVS);
		RefreshCurSectorPVS(CurSecIdx);
		m_OldSectorIdx4PVS = CurSecIdx;
	}
	else
	{
		RefreshOldSectorPVS(CurSecIdx);
		RefreshOldSectorPVS(m_OldSectorIdx4PVS);
		m_OldSectorIdx4PVS = CurSecIdx;
	}
}

RwBool CNtlWorldFieldManager::RefreshPVS()
{
	RwV3d	CurPos		= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwInt32	CurSecIdx	= GetSectorIdx(CurPos);

	if(m_OldSectorIdx4PVS != CurSecIdx)
	{
		RefreshOldSectorPVS(m_OldSectorIdx4PVS);
		RefreshCurSectorPVS(CurSecIdx);
		m_OldSectorIdx4PVS = CurSecIdx;
	}

	return TRUE;
}
RwBool CNtlWorldFieldManager::RefreshFieldAll()
{
	for(RwInt32 i = 0; i < 36; ++i)
	{
		if (!RefreshField(m_Fields6x6[1][i]))
		{
			return FALSE;
		}
	}

	return TRUE;
}

RwBool CNtlWorldFieldManager::RefreshField(RwInt32 FieldIdx)
{
	// exception
	if(!IsFieldValid(FieldIdx))
	{
		DBO_TRACE(FALSE, "CNtlWorldFieldManager::RefreshField, invalid index");
		return FALSE;
	}

	// skip if unloaded
	if(IsFieldLoaded(FieldIdx))
	{
		DeleteFieldInMemory(FieldIdx, FALSE);
		CreateFields(FieldIdx);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldFieldManager::DeleteFieldThenCreateNew(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::DeleteFieldThenCreateNew");

#ifdef dNTL_WORLD_TOOL_MODE

	if(!IsFieldLoaded(FieldIdx))
	{
		DeleteFiles4Field(FieldIdx);
		CreateFields(FieldIdx);
		DeleteFields(FieldIdx);
	}
	else
	{
		DeleteFieldInMemory(FieldIdx);
		DeleteFiles4Field(FieldIdx);
		CreateFields(FieldIdx);
		DeleteFields(FieldIdx);
		CreateFields(FieldIdx);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(TRUE);

#endif

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::DeleteFieldIntoFile(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::DeleteFieldIntoFile");

	if(FieldIdx == -1)
	{
		NTL_RETURN(FALSE);
	}

	int l, m;
	int SectorIdx;
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	RwV3d SectorSPos;

	// World Field Information File
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);

	// 	RwChar cTempFileName[64];
	// 	sprintf_s(cTempFileName, 64, "wfif%d.temp", FieldIdx);
	// 
	// 	if((_access(dGET_WORLD_PARAM()->WorldChar64Buf, 0)) != -1)
	// 	{
	// 		SetFileAttributes(dGET_WORLD_PARAM()->WorldChar64Buf, FILE_ATTRIBUTE_ARCHIVE);
	// 	}

	FILE *pFile;
	::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb");

	DBO_ASSERT(pFile, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");

	if(pFile)
	{
		for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x = (RwReal)m;
				SectorSPos.z = (RwReal)l;
				SectorIdx = GetSectorIdx(SectorSPos);
				CNtlWorldSectorManager::DeleteIntoFile(SectorIdx, pFile);
			}
		}

		::fclose(pFile);
	}

	// 	if((_access(dGET_WORLD_PARAM()->WorldChar64Buf, 0)) != -1)
	// 	{
	// 		SetFileAttributes(dGET_WORLD_PARAM()->WorldChar64Buf, FILE_ATTRIBUTE_ARCHIVE);
	// 	}

	m_pFields[FieldIdx].DeleteFieldIntoFile(FieldIdx);

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::DeleteFieldInMemory(RwInt32 FieldIdx, RwBool SaveSwapInToolMode/* = TRUE*/)
{
	NTL_FUNCTION("CNtlWorldFieldManager::DeleteFieldInMemory");

	if(FieldIdx == -1)
		NTL_RETURN(FALSE);

	int l, m;
	int SectorIdx;
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	RwV3d SectorSPos;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			CNtlWorldSectorManager::DeleteInMemory(SectorIdx, SaveSwapInToolMode);
		}
	}

	m_pFields[FieldIdx].DeleteFieldInMemory(FieldIdx);

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::DeleteFields(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::DeleteFields");

	if(FieldIdx == -1)
		NTL_RETURN(FALSE);

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);

#ifdef dNTL_WORLD_TOOL_MODE

	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	// just make new files or overwrite old files at first then,
	DeleteFieldIntoFile(FieldIdx);

	// free in memory
	DeleteFieldInMemory(FieldIdx);

#else

#ifdef dNTL_WORLD_SCHEDULE_LOADING
	if (m_eMoved2 != eC && m_eMoved2 != ePORTAL && m_eMoved2 != eIDLE)
	{
		m_WorldScheduler.DeleteField(FieldIdx);
	}	
	else
	{
		m_WorldScheduler.RemoveSchedule(FieldIdx);
#endif
		// change dir.
		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		// only freeing in memory must be available
		DeleteFieldInMemory(FieldIdx);
#ifdef dNTL_WORLD_SCHEDULE_LOADING
	}
#endif

#endif

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::CreateFieldInMemory(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreateFieldInMemory");

	if(FieldIdx == -1)
		NTL_RETURN(FALSE);

	m_pFields[FieldIdx].CreateFieldInMemory(FieldIdx);

	int SectorIdx;
	int l, m;
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	RwV3d SectorSPos;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			CNtlWorldSectorManager::CreateInMemory(SectorIdx);
		}
	}

	//  	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	//  	_chdir("fields");

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldFieldManager::CreateFieldFromFile(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreateFieldFromFile");

	if(FieldIdx == -1)
		NTL_RETURN(FALSE);

	// If there wasn't a Prop file just create in memory
	if(!m_pFields[FieldIdx].CreateFieldFromFile(FieldIdx))
	{
		m_pFields[FieldIdx].CreateFieldInMemory(FieldIdx);
	}	

	int SectorIdx;
	int l, m;
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	RwV3d SectorSPos;

	//-------------------------------------------------------------------
	// 2007.03.23 (형석)
	// Pack file 기능 추가

	FILE *pFile = NULL;
	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
	{
		static RwChar chPackPatch[NTL_MAX_DIR_PATH];

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);

		strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "fields");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

		SPackResFileData sPackFileData;
		RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(chPackPatch, sPackFileData);
		if(bPack)
		{
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			::fopen_s(&pFile, sPackFileData.strPackFileName.c_str(), "rb");
			if(pFile)
			{
				fseek(pFile, sPackFileData.uiOffset, SEEK_SET);
			}
		}
	}
	else
	{
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("fields");
		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}

	DBO_ASSERT(pFile, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");

	if(pFile)
	{
		for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x = (RwReal)m;
				SectorSPos.z = (RwReal)l;
				SectorIdx = GetSectorIdx(SectorSPos);

				CNtlWorldSectorManager::CreateFromFile(m_pFields[FieldIdx].GetTexAttr(), SectorIdx, pFile);
			}
		}

		fclose(pFile);
	}

	NTL_RETURN(TRUE);
	/*
	if(FieldIdx == -1)
	{
	return FALSE;
	}

	int		SectorIdx;
	int		l, m;
	RwV3d	SectorSPos;
	RwV3d	SPos = m_pFields[FieldIdx].GetSPos();
	BYTE*   pPosCurFileMem;

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);

	m_FFMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
	pPosCurFileMem = m_FFMem.GetDat();

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
	for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
	{
	SectorSPos.x = (RwReal)m;
	SectorSPos.z = (RwReal)l;
	SectorIdx = GetSectorIdx(SectorSPos);

	CNtlWorldSectorManager::CreateFromFile(m_pFields[FieldIdx].GetTexAttr(), SectorIdx, pPosCurFileMem);
	pPosCurFileMem = CNtlWorldFileMemAccessor::Skip_Sector(pPosCurFileMem);
	}
	}

	// If there wasn't a Prop file just create in memory
	if(!m_pFields[FieldIdx].CreateFieldFromFile(FieldIdx))
	{
	m_pFields[FieldIdx].CreateFieldInMemory(FieldIdx);
	}

	return TRUE;*/
}

RwBool CNtlWorldFieldManager::CreateFields(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldFieldManager::CreateFields");

	if(FieldIdx == -1)
		NTL_RETURN(FALSE);

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);

#ifdef dNTL_WORLD_TOOL_MODE

	if(_chdir(dGET_WORLD_PARAM()->WorldChar64Buf) == -1)
	{
		// create folders
		_mkdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		// there are no files, create a field in memory
		CreateFieldInMemory(FieldIdx);
	}
	else
	{
		// there are files existed, load from the files
		CreateFieldFromFile(FieldIdx);
	}

#else

	// change dir.
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	// there must be files existed, load from the files

#ifdef dNTL_WORLD_SCHEDULE_LOADING
	if (m_eMoved2 != eC && m_eMoved2 != ePORTAL && m_eMoved2 != eIDLE)
	{
		m_WorldScheduler.CreateField(FieldIdx);
	}
	else
	{
		m_WorldScheduler.RemoveSchedule(FieldIdx);
#endif
		CreateFieldFromFile(FieldIdx);
#ifdef dNTL_WORLD_SCHEDULE_LOADING
	}
#endif

#endif

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(TRUE);
}

RwReal CNtlWorldFieldManager::GetWorldSectorHeight(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetWorldSectorHeight");

	RwInt32 CurSectorIdx = GetSectorIdx(Pos);
	if(CurSectorIdx == -1)
		NTL_RETURN(-999.0f);

	NTL_RETURN(RpNtlWorldSectorGetHeight(m_pSectors[CurSectorIdx].m_pWorldSector, &Pos));
}

#ifdef dNTL_WORLD_TOOL_MODE

void CNtlWorldFieldManager::SetWorldSectorHeightInRectangle(RwV3d& PosS, RwV3d& PosE, RwReal Height)
{
	sTARGET_VERT_SL_INFO TargetVertSLInfo;
	vector<sTARGET_VERT_SL_INFO> vecTargetVertSLInfo;

	if(PosS.x == PosE.x && PosS.z == PosE.z)
		return;

	RwV2d Min, Max;
	if(PosS.x >= PosE.x)
	{
		Max.x = PosS.x;
		Min.x = PosE.x;
	}
	else
	{
		Max.x = PosE.x;
		Min.x = PosS.x;
	}

	if(PosS.z >= PosE.z)
	{
		Max.y = PosS.z;
		Min.y = PosE.z;
	}
	else
	{
		Max.y = PosE.z;
		Min.y = PosS.z;
	}

	for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
	{
		RpAtomic	*pCurAtomic = m_pSectors[g_vecVisibleSectors[j]].m_pAtomic;
		RwV3d		*pVList		= RpMorphTargetGetVertices(pCurAtomic->geometry->morphTarget);
		RwRGBA		*pPrelights = RpGeometryGetPreLightColors(pCurAtomic->geometry);
		RwInt32		VertNum		= RpGeometryGetNumVertices(pCurAtomic->geometry);
		RwV3d		Center		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;

		RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKVERTICES);

		for(int i = 0; i < VertNum; ++i)
		{
			float Distance = sqrtf(	(pVList[i].x - Center.x) * (pVList[i].x - Center.x) +
				(pVList[i].z - Center.z) * (pVList[i].z - Center.z));

			if(	(pVList[i].x >= Min.x && pVList[i].x <= Max.x) &&
				(pVList[i].z >= Min.y && pVList[i].z <= Max.y))
			{
				pVList[i].y	= Height;
			}

			// set slope lighting
			TargetVertSLInfo.pNtlWorldSector	= &m_pSectors[g_vecVisibleSectors[j]];
			TargetVertSLInfo.IdxVert			= i;
			vecTargetVertSLInfo.push_back(TargetVertSLInfo);
		}

		RpGeometryUnlock(pCurAtomic->geometry);

		RwSphere sphereBounding;
		RpMorphTargetCalcBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
		RpMorphTargetSetBoundingSphere(pCurAtomic->geometry->morphTarget, &sphereBounding);
	}

	OnSetSlopeLighting(vecTargetVertSLInfo);
}

#endif

// Menu index; 0:create, 1:delete, 2:erase
void CNtlWorldFieldManager::UpdateCurFieldTexLyr(RwInt32 IdxLyr, RwInt32 IdxMenu)
{
	int SectorIdx;
	int l, m;
	RwV3d SPos = m_pFields[m_IdxCurField].GetSPos();
	RwV3d SectorSPos;

	// set current activated texture layer index number
	m_pFields[m_IdxCurField].GetTexAttr().IdxCurLT = IdxLyr;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			CNtlWorldSectorManager::UpdateTexLyr(m_pFields[m_IdxCurField].GetTexAttr(), SectorIdx, IdxLyr, IdxMenu);
		}
	}
}

void CNtlWorldFieldManager::UpdateCurFieldTexMat(RwInt32 IdxLyr)
{
	int SectorIdx;
	int l, m;
	RwV3d SPos = m_pFields[m_IdxCurField].GetSPos();
	RwV3d SectorSPos;

	// set current activated texture layer index number
	m_pFields[m_IdxCurField].GetTexAttr().IdxCurLT = IdxLyr;

	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = GetSectorIdx(SectorSPos);

			// watch out for (IdxLyr - 1) this is 'cos base layer doesn't need a texture matrix so it wasn't even created.
			CNtlWorldSectorManager::UpdateTexMat(m_pFields[m_IdxCurField].GetTexAttr(), SectorIdx, (IdxLyr - 1));
		}
	}
}

RwBool CNtlWorldFieldManager::SetPortalPosition(RwV3d& Pos, RwBool _RUFInitFlag/* = TRUE*/)
{
	m_eMoved2 = ePORTAL;

	if(_RUFInitFlag)
		RwFrameTranslate(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera), &Pos, rwCOMBINEREPLACE);
	//else 
	//	RwFrameTranslate(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera), &Pos, rwCOMBINEPOSTCONCAT);

	return TRUE;
}

void CNtlWorldFieldManager::UpdateCurMapName(RwV3d& Pos)
{
	sNTL_FIELD_PROP NtlFieldProp;
	GetAFieldProp(Pos, NtlFieldProp);

	if(!_stricmp(NtlFieldProp._Name, "null") || !_stricmp(NtlFieldProp._Name, ""))
	{
		m_CurMapNameIdx = 0xffffffff;
		return;
	}
	else
	{
		TBLIDX CurTblIdx = static_cast<TBLIDX>(atoi(NtlFieldProp._Name));

		if(m_CurMapNameIdx == 0xffffffff)
		{
			m_CurMapNameIdx = CurTblIdx;
			CNtlPLEventGenerator::OnMapNameChange(&m_CurMapNameIdx);
		}
		else
		{
			if(m_CurMapNameIdx != CurTblIdx)
			{
				m_CurMapNameIdx = CurTblIdx;
				CNtlPLEventGenerator::OnMapNameChange(&m_CurMapNameIdx);
			}
		}
	}
}

void CNtlWorldFieldManager::UpdateMsg(RwV3d& Pos)
{
	// world field switching effect
	if(GetFieldPropVariationStarting())
	{
		// Update current map name
		UpdateCurMapName(Pos);

		sNTL_FIELD_PROP NtlFieldProp;
		GetAFieldProp(Pos, NtlFieldProp);
		CNtlPLEventGenerator::IsAnotherFieldChanged(&NtlFieldProp);
	}
}

RwReal CNtlWorldFieldManager::GetMonoFactor()
{
	return m_pBloomEntity->GetMonoFactor();
}

VOID CNtlWorldFieldManager::RefreshOldSectorPVS(RwInt32 _SectorIdx)
{
	RwV3d Pos;
	CNtlMath::MathRwV3dAssign(&Pos, m_pSectors[_SectorIdx].DatumPoint.x, m_pSectors[_SectorIdx].DatumPoint.y, m_pSectors[_SectorIdx].DatumPoint.z);

	RwInt32 CellCnt = dPVS_SECTOR_EXTENT_CNT + dPVS_SECTOR_EXTENT_CNT + 1;

	RwV3d SPos;
	SPos.x = Pos.x - (dPVS_SECTOR_EXTENT_CNT * dGET_WORLD_PARAM()->WorldSectorSize);
	SPos.z = Pos.z - (dPVS_SECTOR_EXTENT_CNT * dGET_WORLD_PARAM()->WorldSectorSize);

	RwInt32 CurSectorIdx;
	RwV3d	CurSectorPos;

	for(RwInt32 i = 0; i < CellCnt; ++i)
	{
		for(RwInt32 j = 0; j < CellCnt; ++j)
		{
			CurSectorPos.x	= SPos.x + j * dGET_WORLD_PARAM()->WorldSectorSize;
			CurSectorPos.z	= SPos.z + i * dGET_WORLD_PARAM()->WorldSectorSize;
			CurSectorIdx	= GetSectorIdx(CurSectorPos);

			if(CurSectorIdx == -1)
			{
				continue;
			}

			m_pSectors[CurSectorIdx].m_pNtlWorldSectorPVS->SetVisibility(CurSectorIdx, TRUE);
		}
	}
}

VOID CNtlWorldFieldManager::RefreshCurSectorPVS(RwInt32 _SectorIdx)
{
	if(!m_pSectors[_SectorIdx].m_pNtlWorldSectorPVS->GetEnable())
	{
		return;
	}

	RwV3d Pos;
	CNtlMath::MathRwV3dAssign(&Pos, m_pSectors[_SectorIdx].DatumPoint.x, m_pSectors[_SectorIdx].DatumPoint.y, m_pSectors[_SectorIdx].DatumPoint.z);

	RwInt32 CellCnt = dPVS_SECTOR_EXTENT_CNT + dPVS_SECTOR_EXTENT_CNT + 1;

	RwV3d SPos;
	SPos.x = Pos.x - (dPVS_SECTOR_EXTENT_CNT * dGET_WORLD_PARAM()->WorldSectorSize);
	SPos.z = Pos.z - (dPVS_SECTOR_EXTENT_CNT * dGET_WORLD_PARAM()->WorldSectorSize);

	RwInt32 CurSectorIdx;
	RwV3d	CurSectorPos;

	for(RwInt32 i = 0; i < CellCnt; ++i)
	{
		for(RwInt32 j = 0; j < CellCnt; ++j)
		{
			CurSectorPos.x	= SPos.x + j * dGET_WORLD_PARAM()->WorldSectorSize;
			CurSectorPos.z	= SPos.z + i * dGET_WORLD_PARAM()->WorldSectorSize;
			CurSectorIdx	= GetSectorIdx(CurSectorPos);

			// exception handling
			if(CurSectorIdx == -1)
			{
				continue;
			}

			// self process
			if(CurSectorIdx == _SectorIdx)
			{
				m_pSectors[_SectorIdx].m_pNtlWorldSectorPVS->SetVisibility(_SectorIdx, TRUE);
				continue;
			}

			// others
			m_pSectors[CurSectorIdx].m_pNtlWorldSectorPVS->SetVisibility(CurSectorIdx, m_pSectors[_SectorIdx].m_pNtlWorldSectorPVS->GetVisibilityNeighbor(i * CellCnt + j));
		}
	}
}

void CNtlWorldFieldManager::UpdateSectors()
{
	// update visble sectors
	vector<RwInt32> vecVisibleSectors;
	::RpNtlWorldGetVisibleSectors(&vecVisibleSectors);

	for(unsigned int i = 0; i < vecVisibleSectors.size(); ++i)
	{
		m_pSectors[vecVisibleSectors[i]].Update();
	}

	// refresh PVS
	if(CNtlPLGlobal::m_UseTerrainPVSMode && !CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PVS_LOCKING))
	{
		RwV3d	CurPos		= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
		RwInt32	CurSecIdx	= GetSectorIdx(CurPos);

		if(m_OldSectorIdx4PVS == -1)
		{
			m_OldSectorIdx4PVS = CurSecIdx;
			RefreshCurSectorPVS(m_OldSectorIdx4PVS);
			return;
		}

		if(m_OldSectorIdx4PVS != CurSecIdx)
		{
			RefreshOldSectorPVS(m_OldSectorIdx4PVS);
			RefreshCurSectorPVS(CurSecIdx);
			m_OldSectorIdx4PVS = CurSecIdx;
		}
	}
}

RpWorldSector* GetVisibleSectorsOutdoor(RpWorldSector *pWorldSector, void *data)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);

	if(!pNtlSector)
	{
		return pWorldSector;
	}

	D3DXVECTOR3 Min, Max, RayOri, RayDir;
	Min.x = pWorldSector->boundingBox.inf.x;
	Min.y = pWorldSector->boundingBox.inf.y;
	Min.z = pWorldSector->boundingBox.inf.z;
	Max.x = pWorldSector->boundingBox.sup.x;
	Max.y = pWorldSector->boundingBox.sup.y;
	Max.z = pWorldSector->boundingBox.sup.z;

	RwV3d CenterCoor;
	CenterCoor.x = (Min.x + Max.x) / 2;
	CenterCoor.z = (Min.z + Max.z) / 2;

	RwInt32 WidthNum, DepthNum;
	RwReal XBasedOn0 = CenterCoor.x + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal ZBasedOn0 = CenterCoor.z + dGET_WORLD_PARAM()->WorldSizeHalf;
	WidthNum = (int)(XBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize);
	DepthNum = (int)(ZBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize); 
	RwInt32 RetIdx = WidthNum + (DepthNum * dGET_WORLD_PARAM()->WorldSectorNum);

	g_vecVisibleSectors.push_back(RetIdx);

	return pWorldSector;
}

void CNtlWorldFieldManager::UpdateVisibleSectors()
{
	g_vecVisibleSectors.clear();
	g_vecVisibleSectors.reserve(20);
	RwCameraForAllSectorsInFrustum(CNtlPLGlobal::m_RwCamera, GetVisibleSectorsOutdoor, NULL);
}

RwBool CNtlWorldFieldManager::GetWorldDecal(RwV3d& vPosition, RwV3d& vSize, RwInt32 nPolyMaxCount, RwInt32& nRenderVertexCount, RwReal& fUpdateVertextRadius, RwIm3DVertex* pVertices, RwReal OffsetY)
{
	NTL_FUNCTION("CNtlWorldFieldManager::GetWorldDecal");

	RwInt32 iSectorIdx = GetSectorIdx(vPosition);
	if (iSectorIdx != -1 && m_pSectors[iSectorIdx].m_IsCulled)
	{
		NTL_RETURN(FALSE);
	}

	RwReal fMax = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE);
	RwReal fMin = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE);

	if(vPosition.x < fMin || vPosition.x > fMax || vPosition.z < fMin || vPosition.z > fMax)
	{
		NTL_RETURN(FALSE);
	}

	RwBBox decalBox;
	decalBox.sup.x = vPosition.x + (vSize.x * 0.5f);
	decalBox.inf.x = vPosition.x - (vSize.x * 0.5f);
	decalBox.sup.z = vPosition.z + (vSize.z * 0.5f);
	decalBox.inf.z = vPosition.z - (vSize.z * 0.5f);

	decalBox.inf.x = static_cast<RwReal>((static_cast<RwInt32>(decalBox.inf.x) / dGET_WORLD_PARAM()->WorldSectorTileSize - 1) * dGET_WORLD_PARAM()->WorldSectorTileSize);
	decalBox.inf.z = static_cast<RwReal>((static_cast<RwInt32>(decalBox.inf.z) / dGET_WORLD_PARAM()->WorldSectorTileSize - 1) * dGET_WORLD_PARAM()->WorldSectorTileSize);
	decalBox.sup.x = static_cast<RwReal>((static_cast<RwInt32>(decalBox.sup.x) / dGET_WORLD_PARAM()->WorldSectorTileSize + 1) * dGET_WORLD_PARAM()->WorldSectorTileSize);
	decalBox.sup.z = static_cast<RwReal>((static_cast<RwInt32>(decalBox.sup.z) / dGET_WORLD_PARAM()->WorldSectorTileSize + 1) * dGET_WORLD_PARAM()->WorldSectorTileSize);

	for (RwReal fZ = decalBox.inf.z; fZ < decalBox.sup.z; fZ += dGET_WORLD_PARAM()->WorldSectorTileSize)
	{
		if(nRenderVertexCount + 2 >= nPolyMaxCount)
		{
			nRenderVertexCount += 3;
			break;
		}
		for (RwReal fX = decalBox.inf.x; fX < decalBox.sup.x; fX += dGET_WORLD_PARAM()->WorldSectorTileSize)
		{				
			RwV3d vPos;
			vPos.x = fX;
			vPos.z = fZ;
			vPos.y = GetWorldSectorHeight(vPos) + OffsetY;
			RwIm3DVertexSetPos(&pVertices[nRenderVertexCount], vPos.x, vPos.y, vPos.z);
			++nRenderVertexCount;

			vPos.x = fX;
			vPos.z = fZ + dGET_WORLD_PARAM()->WorldSectorTileSize;
			vPos.y = GetWorldSectorHeight(vPos) + OffsetY;
			RwIm3DVertexSetPos(&pVertices[nRenderVertexCount], vPos.x, vPos.y, vPos.z);
			++nRenderVertexCount;

			vPos.x = fX + dGET_WORLD_PARAM()->WorldSectorTileSize;
			vPos.z = fZ;
			vPos.y = GetWorldSectorHeight(vPos) + OffsetY;
			RwIm3DVertexSetPos(&pVertices[nRenderVertexCount], vPos.x, vPos.y, vPos.z);
			++nRenderVertexCount;

			if(nRenderVertexCount + 2 >= nPolyMaxCount)
			{
				nRenderVertexCount += 3;
				break;
			}

			vPos.x = fX;
			vPos.z = fZ + dGET_WORLD_PARAM()->WorldSectorTileSize;
			vPos.y = GetWorldSectorHeight(vPos) + OffsetY;
			RwIm3DVertexSetPos(&pVertices[nRenderVertexCount], vPos.x, vPos.y, vPos.z);
			++nRenderVertexCount;

			vPos.x = fX + dGET_WORLD_PARAM()->WorldSectorTileSize;
			vPos.z = fZ + dGET_WORLD_PARAM()->WorldSectorTileSize;
			vPos.y = GetWorldSectorHeight(vPos) + OffsetY;
			RwIm3DVertexSetPos(&pVertices[nRenderVertexCount], vPos.x, vPos.y, vPos.z);
			++nRenderVertexCount;

			vPos.x = fX + dGET_WORLD_PARAM()->WorldSectorTileSize;
			vPos.z = fZ;
			vPos.y = GetWorldSectorHeight(vPos) + OffsetY;
			RwIm3DVertexSetPos(&pVertices[nRenderVertexCount], vPos.x, vPos.y, vPos.z);
			++nRenderVertexCount;
		}
	}

	fUpdateVertextRadius = sqrtf(vSize.x * vSize.x + vSize.z * vSize.z) * 0.5f;

	// 	RwReal		Max			= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE);
	// 	RwReal		Min			= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE);
	// 
	// 	RwInt32		MidX		= static_cast<RwInt32>(vPosition.x / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 	RwInt32		MidZ		= static_cast<RwInt32>(vPosition.z / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 
	// 	// 1를 더해주는 이유는 업데이트를 덜 하기 위한 방법이다.
	// 	RwInt32		TileNumHalfX= 1 + (static_cast<RwInt32>(vSize.x * 0.5f) + (dGET_WORLD_PARAM()->WorldSectorTileSize + static_cast<RwInt32>(vSize.x * 0.5f) % dGET_WORLD_PARAM()->WorldSectorTileSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 	RwInt32		TileNumHalfZ= 1 + (static_cast<RwInt32>(vSize.z * 0.5f) + (dGET_WORLD_PARAM()->WorldSectorTileSize + static_cast<RwInt32>(vSize.z * 0.5f) % dGET_WORLD_PARAM()->WorldSectorTileSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 
	// 	// 1를 더해주는 이유는 중앙 타일은 연산에서 빼기 위한 꽁수다.
	// 	RwInt32		TileNumX	= TileNumHalfX * 2 + 1;
	// 	RwInt32		TileNumZ    = TileNumHalfZ * 2 + 1;
	// 
	// 	RwInt32		TileNum		= TileNumX * TileNumZ;
	// 
	// 	RwInt32		PolyNum		= TileNum * 2;
	// 	RwInt32		VertNum		= PolyNum * 3;
	// 
	// 	RwInt32		VertCnt		= 0;
	// 	RwInt32		i, j;
	// 	RwV3d		SPos, CPos;
	// 
	// 	if(VertNum > nPolyMaxCount)
	// 	{
	// 		NTL_RETURN(FALSE);
	// 	}
	// 
	// 	if(MidX < Min || MidX > Max || MidZ < Min || MidZ > Max)
	// 	{
	// 		NTL_RETURN(FALSE);
	// 	}
	// 
	// 	RwInt32 iSectorIdx = GetSectorIdx(vPosition);
	// 	if (iSectorIdx != -1 && m_pSectors[iSectorIdx].m_IsCulled)
	// 	{
	// 		NTL_RETURN(TRUE);
	// 	}
	// 
	// 	SPos.x = static_cast<RwReal>(MidX - ((TileNumHalfX) * dGET_WORLD_PARAM()->WorldSectorTileSize));
	// 	SPos.z = static_cast<RwReal>(MidZ - ((TileNumHalfZ) * dGET_WORLD_PARAM()->WorldSectorTileSize));
	// 
	// 	for(i = 0; i < TileNumX; ++i)
	// 	{
	// 		for(j = 0; j < TileNumZ; ++j)
	// 		{
	// 			CPos.x = SPos.x + j * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.z = SPos.z + i * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.y = GetWorldSectorHeight(CPos) + OffsetY;			
	// 			RwIm3DVertexSetPos(&pVertices[VertCnt], CPos.x, CPos.y, CPos.z);
	// 			++VertCnt;
	// 
	// 			CPos.x = SPos.x + j * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.z = SPos.z + i * dGET_WORLD_PARAM()->WorldSectorTileSize + dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.y = GetWorldSectorHeight(CPos) + OffsetY;			
	// 			RwIm3DVertexSetPos(&pVertices[VertCnt], CPos.x, CPos.y, CPos.z);
	// 			++VertCnt;
	// 
	// 			CPos.x = SPos.x + j * dGET_WORLD_PARAM()->WorldSectorTileSize + dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.z = SPos.z + i * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.y = GetWorldSectorHeight(CPos) + OffsetY;			
	// 			RwIm3DVertexSetPos(&pVertices[VertCnt], CPos.x, CPos.y, CPos.z);
	// 			++VertCnt;
	// 
	// 			CPos.x = SPos.x + j * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.z = SPos.z + i * dGET_WORLD_PARAM()->WorldSectorTileSize + dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.y = GetWorldSectorHeight(CPos) + OffsetY;			
	// 			RwIm3DVertexSetPos(&pVertices[VertCnt], CPos.x, CPos.y, CPos.z);
	// 			++VertCnt;
	// 
	// 			CPos.x = SPos.x + j * dGET_WORLD_PARAM()->WorldSectorTileSize + dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.z = SPos.z + i * dGET_WORLD_PARAM()->WorldSectorTileSize + dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.y = GetWorldSectorHeight(CPos) + OffsetY;			
	// 			RwIm3DVertexSetPos(&pVertices[VertCnt], CPos.x, CPos.y, CPos.z);
	// 			++VertCnt;
	// 
	// 			CPos.x = SPos.x + j * dGET_WORLD_PARAM()->WorldSectorTileSize + dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.z = SPos.z + i * dGET_WORLD_PARAM()->WorldSectorTileSize;
	// 			CPos.y = GetWorldSectorHeight(CPos) + OffsetY;			
	// 			RwIm3DVertexSetPos(&pVertices[VertCnt], CPos.x, CPos.y, CPos.z);
	// 			++VertCnt;
	// 		}
	// 	}
	// 
	// 	nRenderVertexCount	= VertCnt;
	// 	fUpdateVertextRadius= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize * (TileNumHalfX > TileNumHalfZ ? TileNumHalfX : TileNumHalfZ));

	if (nRenderVertexCount > nPolyMaxCount)
	{
		NTL_RETURN(FALSE);
	}
	NTL_RETURN(TRUE);
}

void CNtlWorldFieldManager::UpdateShoreLine(RwReal _ElapsedTime)
{
	CNtlPLShoreLineProp* pProp = static_cast<CNtlPLShoreLineProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE));

	pProp->Update(_ElapsedTime);
}

VOID CNtlWorldFieldManager::GetVisible2DAABB(C2DAABB& _2DAABB)
{
	RwV2d Min, Max;

	for(RwInt32 i = 0; i < 36; ++i)
	{
		if(m_Fields6x6[1][i] != -1)
		{
			Min.x = m_pFields[m_Fields6x6[1][i]].GetSPos().x;
			Min.y = m_pFields[m_Fields6x6[1][i]].GetSPos().z;
			Max.x = Min.x + dGET_WORLD_PARAM()->WorldFieldSize;
			Max.y = Min.y + dGET_WORLD_PARAM()->WorldFieldSize;

			_2DAABB.SetNewMinMax(Min, Max);
		}
	}
}

RwBool CNtlWorldFieldManager::OnDragonSkyAppearence(RwBool _FlgAppeared)
{
	if(_FlgAppeared)
	{
		if(!m_pDragonSkyEntity)
		{
			return FALSE;
		}
		else
		{
			static_cast<CNtlPLSkyDragon*>(m_pDragonSkyEntity)->FadeIn();
		}
	}
	else
	{
		if(!m_pDragonSkyEntity)
		{
			return FALSE;
		}
		else
		{
			static_cast<CNtlPLSkyDragon*>(m_pDragonSkyEntity)->FadeOut();
		}
	}

	return TRUE;
}

RwInt32 CNtlWorldFieldManager::GetSelSectorIdxCntInField(RwInt32 _SectorIdx, RwInt32 _FieldIdx)
{
	RwV3d	SectorSPos;
	RwV3d	SPos	= m_pFields[_FieldIdx].GetSPos();
	RwInt32 Ret		= 0;

	for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x	= (RwReal)m;
			SectorSPos.z	= (RwReal)l;

			if(_SectorIdx == GetSectorIdx(SectorSPos))
			{
				return Ret;
			}

			++Ret;
		}
	}	

	return -999;
}

RwBool CNtlWorldFieldManager::GetWorldShadowColor(RwV3d& Pos, RwRGBA* pOutColor)
{
	RwInt32 iSectorIdx = GetSectorIdx(Pos);
	if (iSectorIdx <= -1)
	{
		return FALSE;
	}

	if (!m_pSectors[iSectorIdx].m_pNtlWorldShadow)
	{
		return FALSE;
	}

	::CopyMemory(pOutColor, &m_pSectors[iSectorIdx].m_pNtlWorldShadow->m_rgbaShadow, sizeof(RwRGBA));

	return TRUE;
}

#define NTL_SHADOW_GRAY_MIN	175
#define NTL_SHADOW_GRAY_MAX	200
RwBool CNtlWorldFieldManager::GetWorldLightColor(RwV3d& Pos, RwRGBA* pOutColor, RwBool bCalcShadow)
{
	RwInt32 iSectorIdx = GetSectorIdx(Pos);
	if (iSectorIdx <= -1)
	{
		return FALSE;
	}
	vector<RwInt32> vecNeighborSectors;
	GetNeighborSectors(iSectorIdx, vecNeighborSectors);
	vecNeighborSectors.push_back(iSectorIdx);

	// 3---2---1
	// |       | 
	// 4   8   0
	// |       |
	// 5---6---7

	for (int i = 0; i < 9; ++i)
	{
		if (vecNeighborSectors.at(i) == -1)
		{
			continue;
		}

		CNtlWorldSector* pSector = &m_pSectors[vecNeighborSectors.at(i)];

		RwV2d ptCheckPos;
		ptCheckPos.x = pSector->DatumPoint.x;
		ptCheckPos.y = pSector->DatumPoint.z;

		RwV2d ptPos;
		ptPos.x = Pos.x;
		ptPos.y = Pos.z;

		RwV2d ptDist;
		RwV2dSub(&ptDist, &ptCheckPos, &ptPos);

		// 여기를 통과하지 못하는 섹터는 -1 값으로 변경한다.
		if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD && !pSector->m_pAtomic)
		{
			vecNeighborSectors.at(i) = -1;
			continue;
		}
		if (RwV2dLength(&ptDist) > dGET_WORLD_PARAM()->WorldSectorSize)
		{
			vecNeighborSectors.at(i) = -1;
			continue;
		}		
	}

	// 위에서 재구성된 NeighborSectors만 연산된다.
	RwRGBAReal	clrfResult;
	RwBool		bResult = FALSE;

	// Box 검사.
	for (int i = 0; i < 9; ++i)
	{
		if (vecNeighborSectors.at(i) == -1)
		{
			continue;
		}



		CNtlWorldSector* pSector = &m_pSectors[vecNeighborSectors.at(i)];

		for (int j = 0; j < (int)pSector->m_vecNtlPLEntityWorldLight_Box.size(); ++j)
		{		
			CNtlPLWorldLight_Base* pWorldLight = (CNtlPLWorldLight_Base*)pSector->m_vecNtlPLEntityWorldLight_Box.at(j);
			if (pWorldLight->GetWorldLightColorf(&Pos, &clrfResult, NULL))
			{
				bResult = TRUE;
				break;
			}
		}
		if (bResult)
		{
			break;
		}
	}

	// Box에서 체크되지 않았다면 구를 검사 한다.
	if (!bResult)
	{
		// WorldLight
		RwRGBAReal	clrfTemp;
		RwRGBAReal*	pclrfTemp = const_cast<RwRGBAReal*>(CNtlFieldColorManager::GetInstance()->GetColorf());

		clrfResult = *pclrfTemp;
		for (int i = 0; i < 9; ++i)
		{
			if (vecNeighborSectors.at(i) == -1)
			{
				continue;
			}

			CNtlWorldSector* pSector = &m_pSectors[vecNeighborSectors.at(i)];

			for (int j = 0; j < (int)pSector->m_vecNtlPLEntityWorldLight_Sphere.size(); ++j)
			{		
				CNtlPLWorldLight_Base* pWorldLight = (CNtlPLWorldLight_Base*)pSector->m_vecNtlPLEntityWorldLight_Sphere.at(j);
				if (pWorldLight->GetWorldLightColorf(&Pos, &clrfResult, pclrfTemp))
				{
					clrfTemp	= clrfResult;
					pclrfTemp	= &clrfTemp;					
				}
			}
		}	

		for (int i = 0; i < 9; ++i)
		{
			if (vecNeighborSectors.at(i) == -1)
			{
				continue;
			}

			CNtlWorldSector* pSector = &m_pSectors[vecNeighborSectors.at(i)];

			for (int j = 0; j < (int)pSector->m_vecNtlPLEntityLightObject.size(); ++j)
			{		
				CNtlPLLightObject* pLightObject = (CNtlPLLightObject*)pSector->m_vecNtlPLEntityLightObject.at(j);
				if (pLightObject->GetWorldLightColorf(&Pos, &clrfResult, pclrfTemp))
				{
					clrfTemp	= clrfResult;
					pclrfTemp	= &clrfTemp;					
				}
			}

			for (int j = 0; j < (int)pSector->m_vecNtlPLEntityOrnament.size(); ++j)
			{
				CNtlPLObject* pPLObject = ((CNtlPLObject*)pSector->m_vecNtlPLEntityOrnament.at(j));

				for (ListAttachAttr::iterator it = pPLObject->GetAttachAttrList()->begin(); it != pPLObject->GetAttachAttrList()->end(); ++it)
				{
					SPLAttachAttr* pPLAttachAttr = *it;
					if (pPLAttachAttr->pPLEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
					{
						CNtlPLLightObject* pLightObject = (CNtlPLLightObject*)pPLAttachAttr->pPLEntity;
						if (pLightObject->GetWorldLightColorf(&Pos, &clrfResult, pclrfTemp))
						{
							clrfTemp	= clrfResult;
							pclrfTemp	= &clrfTemp;					
						}
					}
				}					
			}
		}
	}

	if (bCalcShadow)
	{
		RwRGBA clrTemp;
		if (GetWorldShadowColor(Pos, &clrTemp))
		{
			RwRGBAReal clrfTemp;
			RwRGBARealFromRwRGBA(&clrfTemp, &clrTemp);

			RwUInt8 uiGray	= Helper_RGB2Gray(clrTemp.red, clrTemp.green, clrTemp.blue);
			RwReal	fAdd	= 0.0f;

			if (uiGray < NTL_SHADOW_GRAY_MIN)
			{
				fAdd = static_cast<RwReal>(NTL_SHADOW_GRAY_MIN - uiGray) / 255.0f;
			}
			else if (uiGray > NTL_SHADOW_GRAY_MAX)
			{
				fAdd = static_cast<RwReal>(NTL_SHADOW_GRAY_MAX - uiGray) / 255.0f;
			}	

			clrfTemp.red	+= fAdd;
			clrfTemp.green	+= fAdd;
			clrfTemp.blue	+= fAdd;

			CLAMP(clrfTemp.red, 0.0f, 1.0f);
			CLAMP(clrfTemp.green, 0.0f, 1.0f);
			CLAMP(clrfTemp.blue, 0.0f, 1.0f);

			clrfResult.red		*= clrfTemp.red;
			clrfResult.green	*= clrfTemp.green;
			clrfResult.blue		*= clrfTemp.blue;
		}
	}

	clrfResult.alpha = 1.0f;

	RwRGBAFromRwRGBAReal(pOutColor, &clrfResult);

	return TRUE;
}

VOID CNtlWorldFieldManager::UpdateEntityHandler(RwReal _ElapsedTime)
{
	NTL_ASSERTE(m_pPlanetHandler);
	NTL_ASSERTE(m_pWeatherHandler);


	m_pPlanetHandler->Update(_ElapsedTime);
	m_pWeatherHandler->Update(_ElapsedTime);
}

void CNtlWorldFieldManager::Update(RwV3d& AvatarPos, RwReal _ElapsedTime)
{
#ifndef dNTL_WORLD_TOOL_MODE
#ifdef dNTL_WORLD_SCHEDULE_LOADING
	m_WorldScheduler.Scheduling(_ElapsedTime, AvatarPos);
#endif
#endif

	// Dragon sky direction
#ifdef dNTL_WORLD_TOOL_MODE

	static RwBool FlgDragonAppeared = FALSE;
	if(dOKEY(DIK_I))
	{
		FlgDragonAppeared = !FlgDragonAppeared;
		OnDragonSkyAppearence(FlgDragonAppeared);
	}

#endif
	UpdateVisibleSectors();

	UpdateSky(&AvatarPos);

	UpdateDragonSky(&AvatarPos);

	UpdateFog(&AvatarPos);

	UpdateShoreLine(_ElapsedTime);

	UpdateAnotherField();

	UpdateMsg(AvatarPos);

	UpdateFieldMap(AvatarPos);

	UpdateSectors();

	UpdateSectorMap(AvatarPos);

	UpdateEntityHandler(_ElapsedTime);

	CNtlSpecularManager::GetInstance()->Update(_ElapsedTime);

	CNtlFieldColorManager::GetInstance()->Update(_ElapsedTime);

	CNtlOccluderManager::GetInstance()->Update(_ElapsedTime);

	CNtlWorldRpLightManager::GetInstance()->Update(_ElapsedTime);
}

void CNtlWorldFieldManager::Render()
{
}

void CNtlWorldFieldManager::RenderWater(CNtlWorldSector* pNtlWorldSector, RxD3D9InstanceData* pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader)
{
	/*if(pNtlWorldSector->m_pWater->_pDepthMap)
	{
	SetWaterRenderStateBegin(pNtlWorldSector->m_pWater->_RenderstateSrc, pNtlWorldSector->m_pWater->_RenderstateDst, TRUE, dGET_WORLD_PARAM()->ClrDayAndNight);
	}
	else
	{
	SetWaterRenderStateBegin(pNtlWorldSector->m_pWater->_RenderstateSrc, pNtlWorldSector->m_pWater->_RenderstateDst, FALSE, dGET_WORLD_PARAM()->ClrDayAndNight);
	}*/

	m_pWaterEntity->OnRender(pNtlWorldSector, pInstancedData, pResEntryHeader);

	//SetWaterRenderStateEnd();
}

void CNtlWorldFieldManager::GetSectopMap(RpWorldSector* pRpWorldSector, std::vector<CNtlWorldSector*>& vecNtlWorldSector)
{
	vecNtlWorldSector.clear();

	sNtlWorldSector* pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);
	if (pNtlSector && pNtlSector->pNtlWorldSector)
	{
		vecNtlWorldSector.push_back(pNtlSector->pNtlWorldSector);
	}
}

RwV3d CNtlWorldFieldManager::GetFieldRelativeCoord(RwInt32 iSField, RwInt32 iEField)
{
	return (m_pFields[iEField].GetSPos() - m_pFields[iSField].GetSPos());
}

#ifdef dNTL_WORLD_TOOL_MODE

void CNtlWorldFieldManager::OnWaterProc(RwInt32 IdxMenu, RwInt32 IdxSector, sSECTOR_WATER_ATTR& SectorWaterAttr)
{
	switch(IdxMenu)
	{
	case 0:
		{
			RwV3d Pos;
			Pos.x = m_pSectors[IdxSector].DatumPoint.x;
			Pos.z = m_pSectors[IdxSector].DatumPoint.z;
			SectorWaterAttr._Height = GetWorldSectorHeight(Pos) + dWATER_DEFAULT_HEIGHT_FROM_TERRAIN;
			m_pWaterEntity->OnCreate(&m_pSectors[IdxSector], SectorWaterAttr);
		}
		break;

	case 1:
		{
			RwTexture* pTexture = m_pSectors[IdxSector].m_pWater->_pDepthMap;
			m_pSectors[IdxSector].m_pWater->_pDepthMap = NULL;
			m_pWaterEntity->OnCreate(&m_pSectors[IdxSector], SectorWaterAttr);
			m_pSectors[IdxSector].m_pWater->_pDepthMap = pTexture;
			/*
			RwBool WasDepthMap = FALSE;
			if(m_pSectors[IdxSector].m_pWater->_pDepthMap)
			WasDepthMap = TRUE;

			RwV3d Pos;
			Pos.x = m_pSectors[IdxSector].DatumPoint.x;
			Pos.z = m_pSectors[IdxSector].DatumPoint.z;
			SectorWaterAttr._Height = SectorWaterAttr._Height;
			m_pWaterEntity->OnCreate(&m_pSectors[IdxSector], SectorWaterAttr);

			if(WasDepthMap)
			{
			m_pSectors[IdxSector].m_pWater->_pDepthMap =	CNtlPLResourceManager::GetInstance()->CreateTexture("DepthMap",
			SectorWaterAttr._DepthMapSize,
			SectorWaterAttr._DepthMapSize,
			32,
			rwRASTERFORMAT8888);

			RwTextureSetAddressing(m_pSectors[IdxSector].m_pWater->_pDepthMap, rwTEXTUREADDRESSCLAMP);
			RwTextureSetFilterMode(m_pSectors[IdxSector].m_pWater->_pDepthMap, rwFILTERLINEAR);

			RwV2d	PosSector;
			RwV2d	TexPt;
			RwUInt8	AlphaVariation;

			for(int i = 0; i < SectorWaterAttr._DepthMapSize; ++i)
			{
			for(int j = 0; j < SectorWaterAttr._DepthMapSize; ++j)
			{
			PosSector.x		= m_pSectors[IdxSector].DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
			PosSector.y		= m_pSectors[IdxSector].DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2);

			RwReal RelativeSize = (RwReal)dGET_WORLD_PARAM()->WorldSectorSize / (RwReal)SectorWaterAttr._DepthMapSize;
			TexPt.x	= static_cast<RwReal>(j + 1) * RelativeSize - (RelativeSize / 2.0f) + PosSector.x;
			TexPt.y	= static_cast<RwReal>(i + 1) * RelativeSize - (RelativeSize / 2.0f) + PosSector.y;

			RwV3d Temp;
			Temp.x = TexPt.x;
			Temp.z = TexPt.y;
			Temp.y = GetWorldSectorHeight(Temp);

			float Distance = sqrtf((m_pSectors[IdxSector].m_pWater->_Height - Temp.y) * (m_pSectors[IdxSector].m_pWater->_Height - Temp.y));
			CLAMP(Distance, 0.0f, dDEPTH_MAP_DIST_MAX);

			RwReal DistVariation	= ::fabs(SectorWaterAttr._DepthMapHeightVariation[0] - SectorWaterAttr._DepthMapHeightVariation[1]);
			AlphaVariation			= static_cast<RwUInt8>((Distance / 30.0f) * DistVariation + SectorWaterAttr._DepthMapHeightVariation[0]);

			TexPt.x	= static_cast<RwReal>(SectorWaterAttr._DepthMapSize) - ((fabs(TexPt.x - PosSector.x) / (float)dGET_WORLD_PARAM()->WorldSectorSize) * static_cast<RwReal>(SectorWaterAttr._DepthMapSize));
			TexPt.y	= static_cast<RwReal>(SectorWaterAttr._DepthMapSize) - ((fabs(TexPt.y - PosSector.y) / (float)dGET_WORLD_PARAM()->WorldSectorSize) * static_cast<RwReal>(SectorWaterAttr._DepthMapSize));
			TexPt.x	= static_cast<RwReal>(::abs(TexPt.x));
			TexPt.y	= static_cast<RwReal>(::abs(TexPt.y));
			CLAMP(TexPt.x, 0, SectorWaterAttr._DepthMapSize - 1);
			CLAMP(TexPt.y, 0, SectorWaterAttr._DepthMapSize - 1);
			CNtlPLResourceManager::GetInstance()->SetAlphaBits(m_pSectors[IdxSector].m_pWater->_pDepthMap, AlphaVariation, TexPt);
			}
			}
			}
			*/
		}
		break;

	case 2:
		{
			m_pWaterEntity->OnDelete(&m_pSectors[IdxSector]);
		}
		break;

	case 3:
		{
			if(m_pSectors[IdxSector].m_pWater->_pDepthMap)
			{
				RwTextureDestroy(m_pSectors[IdxSector].m_pWater->_pDepthMap);
				m_pSectors[IdxSector].m_pWater->_pDepthMap = NULL;
			}

			*m_pSectors[IdxSector].m_pWater = SectorWaterAttr;

			m_pSectors[IdxSector].m_pWater->_pDepthMap =	CNtlPLResourceManager::GetInstance()->CreateTexture("DepthMap",
				SectorWaterAttr._DepthMapSize,
				SectorWaterAttr._DepthMapSize,
				32,
				rwRASTERFORMAT8888);

			RwTextureSetAddressing(m_pSectors[IdxSector].m_pWater->_pDepthMap, rwTEXTUREADDRESSCLAMP);
			RwTextureSetFilterMode(m_pSectors[IdxSector].m_pWater->_pDepthMap, rwFILTERLINEAR);

			for(int i = 0; i < SectorWaterAttr._DepthMapSize; ++i)
			{
				for(int j = 0; j < SectorWaterAttr._DepthMapSize; ++j)
				{
					/*

					// old version of depth map

					PosSector.x		= m_pSectors[IdxSector].DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
					PosSector.y		= m_pSectors[IdxSector].DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2);

					RwReal RelativeSize = (RwReal)dGET_WORLD_PARAM()->WorldSectorSize / (RwReal)SectorWaterAttr._DepthMapSize;
					TexPt.x	= static_cast<RwReal>(j + 1) * RelativeSize - (RelativeSize / 2.0f) + PosSector.x;
					TexPt.y	= static_cast<RwReal>(i + 1) * RelativeSize - (RelativeSize / 2.0f) + PosSector.y;

					RwV3d Temp;
					Temp.x = TexPt.x;
					Temp.z = TexPt.y;
					Temp.y = GetWorldSectorHeight(Temp);

					float Distance = sqrtf((m_pSectors[IdxSector].m_pWater->_Height - Temp.y) * (m_pSectors[IdxSector].m_pWater->_Height - Temp.y));
					CLAMP(Distance, 0.0f, dDEPTH_MAP_DIST_MAX);

					RwReal DistVariation	= ::fabs(SectorWaterAttr._DepthMapHeightVariation[0] - SectorWaterAttr._DepthMapHeightVariation[1]);
					AlphaVariation			= static_cast<RwUInt8>((Distance / 30.0f) * DistVariation + SectorWaterAttr._DepthMapHeightVariation[0]);

					TexPt.x	= static_cast<RwReal>(SectorWaterAttr._DepthMapSize) - ((fabs(TexPt.x - PosSector.x) / (float)dGET_WORLD_PARAM()->WorldSectorSize) * static_cast<RwReal>(SectorWaterAttr._DepthMapSize));
					TexPt.y	= static_cast<RwReal>(SectorWaterAttr._DepthMapSize) - ((fabs(TexPt.y - PosSector.y) / (float)dGET_WORLD_PARAM()->WorldSectorSize) * static_cast<RwReal>(SectorWaterAttr._DepthMapSize));
					TexPt.x	= static_cast<RwReal>(::abs(TexPt.x));
					TexPt.y	= static_cast<RwReal>(::abs(TexPt.y));
					CLAMP(TexPt.x, 0, SectorWaterAttr._DepthMapSize - 1);
					CLAMP(TexPt.y, 0, SectorWaterAttr._DepthMapSize - 1);

					CNtlPLResourceManager::GetInstance()->SetAlphaBits(m_pSectors[IdxSector].m_pWater->_pDepthMap, AlphaVariation, TexPt);

					*/

					RwV2d TexPt;
					TexPt.x = static_cast<RwReal>(j);
					TexPt.y = static_cast<RwReal>(i);

					CNtlPLResourceManager::GetInstance()->SetAlphaBits(m_pSectors[IdxSector].m_pWater->_pDepthMap, SectorWaterAttr._RGBA.alpha, TexPt);
				}
			}
		}
		break;

	case 4:
		{
			if(m_pSectors[IdxSector].m_pWater->_pDepthMap)
			{
				RwTextureDestroy(m_pSectors[IdxSector].m_pWater->_pDepthMap);
				m_pSectors[IdxSector].m_pWater->_pDepthMap = NULL;
			}
		}
		break;
	}
}

void CNtlWorldFieldManager::RenderFieldGuidePicked()
{
	if(m_IdxCurField == -1)
		return;

	RwInt32	TileSize = dGET_WORLD_PARAM()->WorldSectorTileSize;
	sLINE3D	SLine[2];
	RwV3d	vTemp;

	BeginD3DLineState();

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	// RB -> RT
	for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum * 2; ++k)
	{
		SLine[0].Col = 0xff0000ff;
		SLine[1].Col = 0xff0000ff;

		SLine[0].Pos.x	= m_pFields[m_IdxCurField].GetSPos().x;
		SLine[0].Pos.z	= m_pFields[m_IdxCurField].GetSPos().z + (TileSize * k);
		vTemp.x = SLine[0].Pos.x;
		vTemp.z = SLine[0].Pos.z;
		SLine[0].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		SLine[1].Pos.x	= SLine[0].Pos.x;
		SLine[1].Pos.z	= SLine[0].Pos.z + TileSize;
		vTemp.x = SLine[1].Pos.x;
		vTemp.z = SLine[1].Pos.z;
		SLine[1].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		m_Buffer[m_VertCount++] = SLine[0];
		m_Buffer[m_VertCount++] = SLine[1];
	}

	// RB -> LB
	for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum * 2; ++k)
	{
		SLine[0].Col = 0xff0000ff;
		SLine[1].Col = 0xff0000ff;

		SLine[0].Pos.x	= m_pFields[m_IdxCurField].GetSPos().x + (TileSize * k);
		SLine[0].Pos.z	= m_pFields[m_IdxCurField].GetSPos().z;
		vTemp.x = SLine[0].Pos.x;
		vTemp.z = SLine[0].Pos.z;
		SLine[0].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		SLine[1].Pos.x	= SLine[0].Pos.x + TileSize;
		SLine[1].Pos.z	= SLine[0].Pos.z;
		vTemp.x = SLine[1].Pos.x;
		vTemp.z = SLine[1].Pos.z;
		SLine[1].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		m_Buffer[m_VertCount++] = SLine[0];
		m_Buffer[m_VertCount++] = SLine[1];
	}

	// RT -> LT
	for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum * 2; ++k)
	{
		SLine[0].Col = 0xff0000ff;
		SLine[1].Col = 0xff0000ff;

		SLine[0].Pos.x	= m_pFields[m_IdxCurField].GetSPos().x + (TileSize * k);
		SLine[0].Pos.z	= m_pFields[m_IdxCurField].GetSPos().z + dGET_WORLD_PARAM()->WorldFieldSize;
		vTemp.x = SLine[0].Pos.x;
		vTemp.z = SLine[0].Pos.z;
		SLine[0].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		SLine[1].Pos.x	= SLine[0].Pos.x + TileSize;
		SLine[1].Pos.z	= SLine[0].Pos.z;
		vTemp.x = SLine[1].Pos.x;
		vTemp.z = SLine[1].Pos.z;
		SLine[1].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		m_Buffer[m_VertCount++] = SLine[0];
		m_Buffer[m_VertCount++] = SLine[1];
	}

	// RB -> LT
	for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum * 2; ++k)
	{
		SLine[0].Col = 0xff0000ff;
		SLine[1].Col = 0xff0000ff;

		SLine[0].Pos.x	= m_pFields[m_IdxCurField].GetSPos().x + dGET_WORLD_PARAM()->WorldFieldSize;
		SLine[0].Pos.z	= m_pFields[m_IdxCurField].GetSPos().z + (TileSize * k);
		vTemp.x = SLine[0].Pos.x;
		vTemp.z = SLine[0].Pos.z;
		SLine[0].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		SLine[1].Pos.x	= SLine[0].Pos.x;
		SLine[1].Pos.z	= SLine[0].Pos.z + TileSize;
		vTemp.x = SLine[1].Pos.x;
		vTemp.z = SLine[1].Pos.z;
		SLine[1].Pos.y = GetWorldSectorHeight(vTemp) + 1.5f;

		m_Buffer[m_VertCount++] = SLine[0];
		m_Buffer[m_VertCount++] = SLine[1];
	}

	RwD3D9SetFVF(D3DFVF_LINE3D);
	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, static_cast<RwUInt32>(m_VertCount / 2), m_Buffer, sizeof(sLINE3D));

	EndD3DLineState();
}

RpWorldSector* FieldGuideLineCB(RpWorldSector *pWorldSector, void *data)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);
	if(!pNtlSector)
		return pWorldSector;


	CNtlWorldFieldManager*	pThis		= static_cast<CNtlWorldFieldManager*>(data);
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pThis->GetFields());
	RwInt32					TileSize	= dGET_WORLD_PARAM()->WorldSectorTileSize;
	sLINE3D					SLine[2];
	RwV3d					vTemp;


	// pick fields
	bool bCompare = false;

	vTemp.x = pNtlSector->pNtlWorldSector->DatumPoint.x;
	vTemp.z = pNtlSector->pNtlWorldSector->DatumPoint.z;
	int FieldIndex = pThis->GetFieldIdx(vTemp);
	for(int i = 0; i < (int)pThis->m_vecVisibleField.size(); ++i)
	{
		if(pThis->m_vecVisibleField[i] == FieldIndex)
		{
			bCompare = true;
			break;
		}
	}

	if(!bCompare)
		pThis->m_vecVisibleField.push_back(FieldIndex);

	// draw tile line(RB -> RT)
	for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum; ++k)
	{
		SLine[0].Col = 0xff00ff00;
		SLine[1].Col = 0xff00ff00;

		SLine[0].Pos.x	= pNtlSector->pNtlWorldSector->DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2.0f);
		SLine[0].Pos.z	= pNtlSector->pNtlWorldSector->DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2.0f) + (TileSize * k);
		vTemp.x = SLine[0].Pos.x;
		vTemp.z = SLine[0].Pos.z;

		RwReal PosHeight = pThis->GetWorldSectorHeight(vTemp);
		if(static_cast<RwInt32>(PosHeight) == -999)
		{
			continue;
		}
		else
		{
			SLine[0].Pos.y = PosHeight + 0.5f;
		}

		SLine[1].Pos.x	= SLine[0].Pos.x;
		SLine[1].Pos.z	= SLine[0].Pos.z + TileSize;
		vTemp.x = SLine[1].Pos.x;
		vTemp.z = SLine[1].Pos.z;

		PosHeight = pThis->GetWorldSectorHeight(vTemp);
		if(static_cast<RwInt32>(PosHeight) == -999)
		{
			continue;
		}
		else
		{
			SLine[1].Pos.y = PosHeight + 0.5f;
		}

		pThis->m_Buffer[pThis->m_VertCount++] = SLine[0];
		pThis->m_Buffer[pThis->m_VertCount++] = SLine[1];					
	}

	// RB -> LB
	for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum; ++k)
	{
		SLine[0].Col = 0xff00ff00;
		SLine[1].Col = 0xff00ff00;

		SLine[0].Pos.x	= pNtlSector->pNtlWorldSector->DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2.0f) + (TileSize * k);
		SLine[0].Pos.z	= pNtlSector->pNtlWorldSector->DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2.0f);
		vTemp.x = SLine[0].Pos.x;
		vTemp.z = SLine[0].Pos.z;

		RwReal PosHeight = pThis->GetWorldSectorHeight(vTemp);
		if(static_cast<RwInt32>(PosHeight) == -999)
		{
			continue;
		}
		else
		{
			SLine[0].Pos.y = PosHeight + 0.5f;
		}

		SLine[1].Pos.x	= SLine[0].Pos.x + TileSize;
		SLine[1].Pos.z	= SLine[0].Pos.z;
		vTemp.x = SLine[1].Pos.x;
		vTemp.z = SLine[1].Pos.z;

		PosHeight = pThis->GetWorldSectorHeight(vTemp);
		if(static_cast<RwInt32>(PosHeight) == -999)
		{
			continue;
		}
		else
		{
			SLine[1].Pos.y = PosHeight + 0.5f;
		}

		pThis->m_Buffer[pThis->m_VertCount++] = SLine[0];
		pThis->m_Buffer[pThis->m_VertCount++] = SLine[1];
	}

	return pWorldSector;
}

RpWorldSector* SectorGuideLineCB(RpWorldSector *pWorldSector, void *data)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);
	if(!pNtlSector)
		return pWorldSector;


	CNtlWorldFieldManager*	pThis		= static_cast<CNtlWorldFieldManager*>(data);
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pThis->GetFields());
	RwInt32					TileSize	= dGET_WORLD_PARAM()->WorldSectorTileSize;
	sLINE3D					SLine[2];
	RwV3d					vTemp;


	// pick fields
	bool bCompare = false;

	vTemp.x = pNtlSector->pNtlWorldSector->DatumPoint.x;
	vTemp.z = pNtlSector->pNtlWorldSector->DatumPoint.z;
	int FieldIndex = pThis->GetFieldIdx(vTemp);
	for(int i = 0; i < (int)pThis->m_vecVisibleField.size(); ++i)
	{
		if(pThis->m_vecVisibleField[i] == FieldIndex)
		{
			bCompare = true;
			break;
		}
	}

	if(!bCompare)
		pThis->m_vecVisibleField.push_back(FieldIndex);

	return pWorldSector;
}

void CNtlWorldFieldManager::RenderFieldGuide()
{
	sLINE3D	SLine[2];
	int		CurFieldIdx = 0;
	RwReal	ErrHeight	= 10.0f;
	RwInt32	TileSize	= dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwV3d	vTemp;


	BeginD3DLineState();

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	m_vecVisibleField.clear();
	RwCameraForAllSectorsInFrustum(CNtlPLGlobal::m_RwCamera, SectorGuideLineCB, this);

	m_VertCount = 0;
	if(m_vecVisibleField.size())
	{
		for(int i = 0; i < (int)m_vecVisibleField.size(); ++i)
		{
			CurFieldIdx = m_vecVisibleField[i];

			// draw tile line(RB -> RT)
			for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum * 2; ++k)
			{
				SLine[0].Col = 0xffff0000;
				SLine[1].Col = 0xffff0000;

				SLine[0].Pos.x	= m_pFields[CurFieldIdx].GetSPos().x;
				SLine[0].Pos.z	= m_pFields[CurFieldIdx].GetSPos().z + (TileSize * k);
				vTemp.x = SLine[0].Pos.x;
				vTemp.z = SLine[0].Pos.z;
				SLine[0].Pos.y = GetWorldSectorHeight(vTemp) + 1.0f;

				SLine[1].Pos.x	= SLine[0].Pos.x;
				SLine[1].Pos.z	= SLine[0].Pos.z + TileSize;
				vTemp.x = SLine[1].Pos.x;
				vTemp.z = SLine[1].Pos.z;
				SLine[1].Pos.y = GetWorldSectorHeight(vTemp) + 1.0f;

				m_Buffer[m_VertCount++] = SLine[0];
				m_Buffer[m_VertCount++] = SLine[1];
			}

			// RB -> LB
			for(int k = 0; k < dGET_WORLD_PARAM()->WorldSectorTileNum * 2; ++k)
			{
				SLine[0].Col = 0xffff0000;
				SLine[1].Col = 0xffff0000;

				SLine[0].Pos.x	= m_pFields[CurFieldIdx].GetSPos().x + (TileSize * k);
				SLine[0].Pos.z	= m_pFields[CurFieldIdx].GetSPos().z;
				vTemp.x = SLine[0].Pos.x;
				vTemp.z = SLine[0].Pos.z;
				SLine[0].Pos.y = GetWorldSectorHeight(vTemp) + 1.0f;

				SLine[1].Pos.x	= SLine[0].Pos.x + TileSize;
				SLine[1].Pos.z	= SLine[0].Pos.z;
				vTemp.x = SLine[1].Pos.x;
				vTemp.z = SLine[1].Pos.z;
				SLine[1].Pos.y = GetWorldSectorHeight(vTemp) + 1.0f;

				m_Buffer[m_VertCount++] = SLine[0];
				m_Buffer[m_VertCount++] = SLine[1];
			}
		}
	}

	RwD3D9SetFVF(D3DFVF_LINE3D);
	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, static_cast<RwUInt32>(m_VertCount / 2), m_Buffer, sizeof(sLINE3D));

	EndD3DLineState();
}

void CNtlWorldFieldManager::RenderSectorGuide()
{
	sLINE3D	SLine[2];
	int		CurFieldIdx = 0;
	RwReal	ErrHeight	= 10.0f;
	RwInt32	TileSize	= dGET_WORLD_PARAM()->WorldSectorTileSize;

	BeginD3DLineState();

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	m_vecVisibleField.clear();
	m_VertCount = 0;
	RwCameraForAllSectorsInFrustum(CNtlPLGlobal::m_RwCamera, FieldGuideLineCB, this);

	RwD3D9SetFVF(D3DFVF_LINE3D);
	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, static_cast<RwUInt32>(m_VertCount / 2), m_Buffer, sizeof(sLINE3D));

	EndD3DLineState();
}

#endif

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif