#include "precomp_ntlpresentation.h"

#include "NtlProfiler.h"
#include "NtlAtomic.h"

#include "NtlPLRenderState.h"

#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"

#include "NtlWorldInterface.h"
#include "NtlWorldPaletteDefine.h"

#include "NtlPLResourcePack.h"
#include "NtlPLAtomicSorter.h"
#include "NtlPLResourceManager.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLPlanet.h"
#include "NtlPLWater.h"
#include "NtlPLPlant.h"
#include "NtlPLObject.h"
#include "NtlPLGameProperty.h"

#include "NtlWorldRpLightManager.h"

#include "NtlWorldBlockManager.h"
#include "NtlWorldFieldManager4RWWorld.h"
#include "NtlWorldLTManager.h"
#include "NtlOccluderManager.h"

#include "NtlPLDecal.h"
#include "NtlPLOccluder_Base.h"

#include "NtlPLCollision.h"
#include "NtlPLRenderGeometry.h"

CNtlWorldFieldManager4RWWorld::CNtlWorldFieldManager4RWWorld(RpWorld *pNtlWorld, RwV3d& SpawnPos, EActiveWorldType _ActiveWorldtype)
:CNtlWorldFieldManager(pNtlWorld, SpawnPos, _ActiveWorldtype)
,m_pWorldBlockMananger(NULL)
{
}

CNtlWorldFieldManager4RWWorld::~CNtlWorldFieldManager4RWWorld(void)
{
}

void CNtlWorldFieldManager4RWWorld::Init(RpWorld *pNtlWorld, RwV3d& SpawnPos)
{
	CNtlWorldFieldManager::Init(pNtlWorld, SpawnPos);

	m_pWorldBlockMananger = NTL_NEW CNtlWorldBlockMananger();
	m_pWorldBlockMananger->Init(dGET_WORLD_PARAM()->WorldSize, dGET_WORLD_PARAM()->WorldBlockSize);

	CNtlWorldRpLightManager::GetInstance()->LoadSwap();

	if (!LoadProperty())
	{
#ifdef dNTL_WORLD_TOOL_MODE
		SaveProperty();
#endif
	}
}

void CNtlWorldFieldManager4RWWorld::Free()
{
	CNtlWorldRpLightManager::GetInstance()->SaveSwap();

	if (m_pWorldBlockMananger)
	{
		m_pWorldBlockMananger->Free();
		NTL_DELETE(m_pWorldBlockMananger);
	}

	for (INDOOR_SECTOR_MAP_IT it = m_mapSectors.begin(); it != m_mapSectors.end(); ++it)
	{
		SRpWorldSectorIndoor* psRpWorldSectorIndoor = it->second;
		NTL_DELETE(psRpWorldSectorIndoor);
	}
	m_mapSectors.clear();

	CNtlWorldFieldManager::Free();
}

RwBool CNtlWorldFieldManager4RWWorld::SaveCurFields()
{
#ifdef dNTL_WORLD_TOOL_MODE
	SaveProperty();
#endif

#ifdef dNTL_WORLD_TOOL_MODE
	CNtlWorldLTManager::GetInstance()->SaveBSP();
#endif

	return CNtlWorldFieldManager::SaveCurFields();
}

RwBool CNtlWorldFieldManager4RWWorld::SaveCurFieldsBeforeExit()
{
	CNtlWorldFieldManager::SaveCurFieldsBeforeExit();

#ifdef dNTL_WORLD_TOOL_MODE
	SaveProperty();

	CNtlWorldLTManager::GetInstance()->SaveBSP();
	CNtlWorldLTManager::GetInstance()->Destroy();
	CNtlWorldLTManager::DestroyInstance();
#endif

	return TRUE;
}

DWORD CNtlWorldFieldManager4RWWorld::GetMapTileProp(RwV3d& Pos)
{
	sNTL_BLOCK_PROP sNtlBlockProp;
	if (m_pWorldBlockMananger->GetABlockProp(Pos, sNtlBlockProp))
	{
		DWORD	dwProp		= sNtlBlockProp._GameProp;
		RwInt32 iSectorIdx	= GetSectorIdx(Pos);
		if (iSectorIdx != -1)
		{
			for(int i = 0; i < (int)m_pSectors[iSectorIdx].m_vecNtlPLEntityGameProperty.size(); ++i)
			{
				CNtlPLGameProperty* pNtlPLGameProperty = (CNtlPLGameProperty*)m_pSectors[iSectorIdx].m_vecNtlPLEntityGameProperty.at(i);
				if (pNtlPLGameProperty->GetShapeType() == ENTL_PL_GAME_PROPERTY_SHAPE_CIRCLE)
				{
					RwV3d vDist = pNtlPLGameProperty->GetPosition();
					vDist.x	-= Pos.x;
					vDist.y = 0.0f;
					vDist.z	-= Pos.z;

					if (vDist.x * vDist.x + vDist.z * vDist.z < pNtlPLGameProperty->GetSize().x)
						dwProp |= pNtlPLGameProperty->GetGameProperty();
				}
				else if (pNtlPLGameProperty->GetShapeType() == ENTL_PL_GAME_PROPERTY_SHAPE_PLANE)
				{
					RwV3d vDist = pNtlPLGameProperty->GetPosition();
					RwV3d vSize = pNtlPLGameProperty->GetSize();
					if (Pos.x < vDist.x + vSize.x &&
						Pos.x > vDist.x - vSize.x &&
						Pos.z < vDist.z + vSize.z &&
						Pos.z > vDist.z - vSize.z)
					{
						dwProp |= pNtlPLGameProperty->GetGameProperty();
					}
				}
			}
		}			
		return dwProp;
	}	

	return 0x00000000;
}

BYTE CNtlWorldFieldManager4RWWorld::GetMaterialTileProp(RwV3d& Pos)
{
	return 0;
}

RwReal CNtlWorldFieldManager4RWWorld::GetWorldSectorHeight(RwV3d& Pos)
{
	RwLine Line;
	RwV3dAssign(&Line.start,&Pos);
	RwV3dAssign(&Line.end,	&Pos);
	Line.start.y	= RpWorldGetBBox(m_pRpWorld)->sup.y;
	Line.end.y		= RpWorldGetBBox(m_pRpWorld)->inf.y;

	static SWorldIntersect sHFInter;
	if (Collision_IndoorIntersectionLineTopDown(Line, sHFInter))
	{
		if (sHFInter.bCollision)
		{
			return sHFInter.vCollPos.y;
		}
	}
	return RpWorldGetBBox(m_pRpWorld)->inf.y;
}

//RwBool CNtlWorldFieldManager4RWWorld::GetWorldReady()
//{
// 	NTL_FUNCTION("CNtlWorldFieldManager::GetWorldReady");
// 
// 
// 	if(m_eMoved2 != eC)
// 		NTL_RETURN(TRUE);
// 
// 
// 	NTL_RETURN(FALSE);
// 	return TRUE;
//}

RwBool CNtlWorldFieldManager4RWWorld::GetWorldShadowColor(RwV3d& Pos, RwRGBA* pOutColor)
{
	return FALSE;
}

RwBool CNtlWorldFieldManager4RWWorld::IsFieldLoaded(RwInt32 FieldIdx)
{
	if(CNtlWorldSectorManager::IsSectorLoaded(CNtlWorldSectorManager::GetSectorIdx(m_pFields[FieldIdx].GetSPos())))
	{
		return TRUE;
	}
	return FALSE;
}

RwBool CNtlWorldFieldManager4RWWorld::CreateFields(RwInt32 FieldIdx)
{
#ifdef dNTL_WORLD_TOOL_MODE
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	for(int l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldBlockSize)
	{
		for(int m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldBlockSize)
		{
			RwV3d vPos;
			vPos.x = (RwReal)m;
			vPos.z = (RwReal)l;
			m_pWorldBlockMananger->GenBlockBBox(m_pWorldBlockMananger->GetBlockIdx(vPos));
		}
	}
#endif
	return CNtlWorldFieldManager::CreateFields(FieldIdx);
}

RwBool CNtlWorldFieldManager4RWWorld::CreateFieldInMemory(RwInt32 FieldIdx)
{
	if(FieldIdx == -1)
		return FALSE;

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

	return TRUE;
}

RwBool CNtlWorldFieldManager4RWWorld::CreateFieldFromFile(RwInt32 FieldIdx)
{
	if(FieldIdx == -1)
		return FALSE;

	int SectorIdx;
	int l, m;
	RwV3d SPos = m_pFields[FieldIdx].GetSPos();
	RwV3d SectorSPos;

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
		DBO_WARNING_MESSAGE("dGET_WORLD_PARAM()->WorldChar64Buf: " << dGET_WORLD_PARAM()->WorldChar64Buf);
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

	return TRUE;
}

void CNtlWorldFieldManager4RWWorld::DeleteFiles4Field(RwInt32 FieldIdx)
{
	CNtlWorldFieldManager::DeleteFiles4Field(FieldIdx);
}

RwBool CNtlWorldFieldManager4RWWorld::DeleteFields(RwInt32 FieldIdx)
{
	return CNtlWorldFieldManager::DeleteFields(FieldIdx);
}

RwBool CNtlWorldFieldManager4RWWorld::DeleteFieldInMemory(RwInt32 FieldIdx, RwBool SaveSwapInToolMode)
{
	if(FieldIdx == -1)
	{
		return FALSE;
	}

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

#ifdef dNTL_WORLD_TOOL_MODE
	int BlockIdx;
	for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldBlockSize)
	{
		for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldBlockSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			BlockIdx = m_pWorldBlockMananger->GetBlockIdx(SectorSPos);

			m_pWorldBlockMananger->SaveProperty(BlockIdx);
		}
	}
#endif

	return TRUE;
}

RwBool CNtlWorldFieldManager4RWWorld::DeleteFieldIntoFile(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldFieldManager4RWWorld::DeleteFieldIntoFile");

	if(FieldIdx == -1)
	{
		return FALSE;
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
// 		MoveFile(dGET_WORLD_PARAM()->WorldChar64Buf, cTempFileName);
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
// 		DeleteFile(cTempFileName);
// 	}

	return TRUE;
}

RwBool CNtlWorldFieldManager4RWWorld::DeleteFieldThenCreateNew(RwInt32 FieldIdx)
{
	return CNtlWorldFieldManager::DeleteFieldThenCreateNew(FieldIdx);
}

void CNtlWorldFieldManager4RWWorld::LoadPVS()
{
}

void CNtlWorldFieldManager4RWWorld::RefreshCurSectorPVS(RwInt32 _SectorIdx)
{
}

void CNtlWorldFieldManager4RWWorld::RefreshOldSectorPVS(RwInt32 _SectorIdx)
{
}

void CNtlWorldFieldManager4RWWorld::Update(RwV3d& AvatarPos, RwReal _ElapsedTime)
{
	CNtlWorldFieldManager::Update(AvatarPos, _ElapsedTime);

	m_pWorldBlockMananger->Update(_ElapsedTime, AvatarPos, m_eMoved2);

#ifdef dNTL_WORLD_TOOL_MODE
	CNtlWorldLTManager::GetInstance()->Update(_ElapsedTime);
#endif
}

void CNtlWorldFieldManager4RWWorld::CreateRpWorld()
{
	RwChar acTempPath[MAX_PATH];
	RwChar acTempTexPath[MAX_PATH];
	RwChar			szBPS[64];

#ifdef dNTL_WORLD_TOOL_MODE
	sprintf_s(acTempPath, MAX_PATH, "%s%s.bsp", dGET_WORLD_PARAM()->WorldProjectFolderName, dGET_WORLD_PARAM()->WorldBSP);
	//sprintf_s(acTempTexPath, MAX_PATH, "%stextures\\;%stextures\\lightmaps\\;texture\\ntlwe\\indoor\\;", dGET_WORLD_PARAM()->WorldProjectFolderName, dGET_WORLD_PARAM()->WorldProjectFolderName);	
#else
	sprintf_s(acTempPath, MAX_PATH, "%s\\%s.bsp", dGET_WORLD_PARAM()->WorldProjectFolderName, dGET_WORLD_PARAM()->WorldBSP);
#endif

	_itoa_s(dGET_WORLD_PARAM()->WorldResourceID, szBPS, sizeof(szBPS), 10);
	
	string str1 = "texture\\ntlwe\\ltmap\\";
	string str2 = szBPS;
	string str3 = "\\bsplm\\;";
	string strR = str1 + str2 + str3;
	sprintf_s(acTempTexPath, MAX_PATH, "texture\\ntlwe\\indoor\\;texture\\object\\;%s", strR.c_str());	

	void*		pFile	= NULL;
	RwStream*	pStream	= NULL;
	RpWorld*	pWorld	= NULL;

	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
	{
		SPackResFileData sPackFileData;
		RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(acTempPath, sPackFileData);
		if(bPack)
		{
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			pFile = RwFopen(sPackFileData.strPackFileName.c_str(), "rb");
			if(pFile)
			{
				RwFseek(pFile, sPackFileData.uiOffset, RTFILE_POS_BEGIN);
				pStream = RwStreamOpen(rwSTREAMFILE, rwSTREAMREAD, pFile);
			}
		}
	}
	else
	{
		pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, acTempPath);
	}
	DBO_ASSERT(pStream, "Stream read failed.");

	if (pStream)
	{
		if(RwStreamFindChunk(pStream, rwID_WORLD, (RwUInt32 *)NULL, (RwUInt32 *)NULL))
		{
			//RwChunkHeaderInfo HeaderInfo;
			//RwStreamReadChunkHeaderInfo(pStream, &HeaderInfo);

			//if (HeaderInfo.type == rwID_WORLD)
			{
				RwImageSetPath(acTempTexPath);
				pWorld = RpWorldStreamRead(pStream);
				DBO_ASSERT(pWorld, "World create failed.");

				if (pWorld)
				{
					m_pRpWorld = pWorld;

					CNtlPLGlobal::SetActiveWorld(m_pRpWorld);

#ifdef dNTL_WORLD_TOOL_MODE
					const RwBBox* pBBoxWorld = RpWorldGetBBox(m_pRpWorld);
					CNtlPLGlobal::m_fIndoorPlaneHeight = (pBBoxWorld->inf.y + pBBoxWorld->sup.y) * 0.5f;
#endif

#ifndef dNTL_WORLD_TOOL_MODE
					RpLtMapSetRenderStyle(static_cast<RpLtMapStyle>(rpLTMAPSTYLERENDERBASE | rpLTMAPSTYLERENDERLIGHTMAP), m_pRpWorld);
#endif

					RpPVSHook(m_pRpWorld);
				}	
			}
			RwStreamClose(pStream, NULL);
		}
	}

	if (pFile)
	{
		RwFclose(pFile);
	}
}

void CNtlWorldFieldManager4RWWorld::DestroyRpWorld()
{
	if (m_pRpWorld)
	{
		RpPVSUnhook(CNtlPLGlobal::m_pRpWorld);
	}
	CNtlWorldFieldManager::DestroyRpWorld();
}

void CNtlWorldFieldManager4RWWorld::GetSectopMap(RpWorldSector* pRpWorldSector, std::vector<CNtlWorldSector*>& vecNtlWorldSector)
{
	INDOOR_SECTOR_MAP_IT it = m_mapSectors.find(pRpWorldSector);
	if (it != m_mapSectors.end())
	{
		SRpWorldSectorIndoor* psRpWorldSectorIndoor = it->second;
		if (psRpWorldSectorIndoor)
		{
			vecNtlWorldSector = psRpWorldSectorIndoor->vecNtlWorldSector;
			return;
		}
	}
	vecNtlWorldSector.clear();
}

RpCollisionTriangle* GetNtlWorldIndoorCollisionInfoMiniIndoor(RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void *pData)
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

RpAtomic* GetNtlWorldIndoorCollisionInfo(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fRatio, void* pData)
{
	if (static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic))->GetClassType() != PLENTITY_OBJECT)
	{
		return pAtomic;
	}

	sNtlMapObjCollisionParam*	pCollisionParam = static_cast<sNtlMapObjCollisionParam*>(pData);
	CNtlPLObject*				pNtlPLObject	= static_cast<CNtlPLObject*>(RpNtlAtomicGetData(pAtomic));

	if (pNtlPLObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		pNtlPLObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		RpAtomicForAllIntersections(pAtomic, pIntersection, GetNtlWorldIndoorCollisionInfoMiniIndoor, pData);
	}

	return pAtomic;
}

// RpWorldSector* GetNtlWorldIndoorCollisionInfo(RpIntersection * pIntersection, RpWorldSector * pRpWorldSector, void *pData)
// {
// 	sNtlWorldCollisionInfo* pCollInfo = static_cast<sNtlWorldCollisionInfo*>(pData);
// 
// 	RpTriangle	*pTList		= pRpWorldSector->triangles;
// 	RwV3d		*pVList		= pRpWorldSector->vertices;
// 
// 	RpIntersection TempIS;
// 	TempIS.type = rpINTERSECTLINE;
// 
// 	RwReal	DistFromCam;
// 
// 	for(int i = 0; i < pRpWorldSector->numTriangles; ++i) 
// 	{
// 		TempIS.t.line.start = pCollInfo->RayOri;
// 		RwV3dSubMacro(&TempIS.t.line.end, &pCollInfo->RayIntersectionPt4Terrain, &pCollInfo->RayOri);
// 
// 		if(RtIntersectionLineTriangle(&TempIS.t.line.start,
// 			&TempIS.t.line.end,
// 			&pVList[pTList[i].vertIndex[0]],
// 			&pVList[pTList[i].vertIndex[1]],
// 			&pVList[pTList[i].vertIndex[2]],
// 			&DistFromCam))
// 		{
// 			RwV3d vScale;
// 			RwV3dScale(&vScale, &TempIS.t.line.end, DistFromCam);
// 			RwReal fTempDist = RwV3dLength(&vScale);
// 
// 			if(pCollInfo->fDist - fTempDist > 0.0001f)
// 			{
// 				pCollInfo->IsCollidedAtSectors = TRUE;
// 
// 				RwV3d vCollPos;
// 				RwV3dAdd(&vCollPos, &pCollInfo->RayOri, &vScale);
// 
// 				// 툴에서의 Mouse Pick Data는 TransparencyTile도 충돌해야 한다. 인도어에서는 굳이 필요 하지 않을 것 같다.
// // 				if (!dGET_COLLISION_INFO_UPDATE() && GetSceneManager()->GetWorldAttribute(vCollPos) & dNMAP_TRANSPARENCY_TILE_FLAG)
// // 				{
// // 					continue;
// // 				}
// 
// 				pCollInfo->RayIntersectionPt4Terrain = vCollPos;
// 
// 				pCollInfo->fDist = fTempDist;
// 			}
// 		}
// 	}	
// 	return pRpWorldSector;
// }

RpCollisionTriangle* GetNtlWorldIndoorCollisionInfo( RpIntersection *pIntersection, RpWorldSector* pRpWorldSector, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void* pData )
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
		
		// 툴에서의 Mouse Pick Data는 TransparencyTile도 충돌해야 한다. 인도어에서는 굳이 필요 하지 않을 것 같다.
// 		if (!dGET_COLLISION_INFO_UPDATE() && GetSceneManager()->GetWorldAttribute(vCollPos) & dNMAP_TRANSPARENCY_TILE_FLAG)
// 		{
// 			continue;
// 		}

		pCollInfo->fDist						= fDist;
		pCollInfo->RayIntersectionPt4Terrain	= vCollPos;
		pCollInfo->IsCollidedAtSectors			= TRUE;
	}
	return pRpCollisionTriangle;
}

RpCollisionTriangle* GetNtlWorldIndoorCollisionInfoWater( RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void *pData)
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

RpWorldSector* GetNtlWorldIndoorCollisionInfoWater(RpIntersection * pIntersection, RpWorldSector * pRpWorldSector, void *pData)
{
	std::vector<CNtlWorldSector*> vecNtlWorldSector;
	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectopMap(pRpWorldSector, vecNtlWorldSector);

	for (int i = 0; i < (int)vecNtlWorldSector.size(); ++i)
	{
		CNtlWorldSector*	pNtlWorldSector = vecNtlWorldSector.at(i);
		RpAtomic*			pAtomic			= pNtlWorldSector->m_pWater ? pNtlWorldSector->m_pWater->_pAtom : NULL;
		
		if (pAtomic)
		{
			if (pIntersection->type == rpINTERSECTLINE && !NtlTestLineSphere(&pIntersection->t.line.start, &pIntersection->t.line.end, &pAtomic->worldBoundingSphere))
			{
				return pRpWorldSector;
			}
			RpAtomicForAllIntersections(pAtomic, pIntersection, GetNtlWorldIndoorCollisionInfoWater, pData);
		}
	}
	return pRpWorldSector;
}

RpWorldSector* GetVisibleSectorsIndoor(RpWorldSector *pWorldSector, void *data)
{
	std::vector<CNtlWorldSector*> vecNtlWorldSector;
	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectopMap(pWorldSector, vecNtlWorldSector);

	for (int i = 0; i < (int)vecNtlWorldSector.size(); ++i)
	{
		RwV3d CenterCoor;
		CenterCoor.x = vecNtlWorldSector.at(i)->DatumPoint.x;
		CenterCoor.z = vecNtlWorldSector.at(i)->DatumPoint.z;

		RwInt32 WidthNum, DepthNum;
		RwReal XBasedOn0 = CenterCoor.x + dGET_WORLD_PARAM()->WorldSizeHalf;
		RwReal ZBasedOn0 = CenterCoor.z + dGET_WORLD_PARAM()->WorldSizeHalf;
		WidthNum = (int)(XBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize);
		DepthNum = (int)(ZBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize); 
		RwInt32 RetIdx = WidthNum + (DepthNum * dGET_WORLD_PARAM()->WorldSectorNum);

		std::vector<RwInt32>::iterator it = std::find(g_vecVisibleSectors.begin(), g_vecVisibleSectors.end(), RetIdx);
		if (it != g_vecVisibleSectors.end())
		{
			continue;
		}
		g_vecVisibleSectors.push_back(RetIdx);
	}
	return pWorldSector;
}

void CNtlWorldFieldManager4RWWorld::UpdateVisibleSectors()
{
	g_vecVisibleSectors.clear();
	g_vecVisibleSectors.reserve(20);
	RwCameraForAllSectorsInFrustum(CNtlPLGlobal::m_RwCamera, GetVisibleSectorsIndoor, NULL);
}

RwBool CNtlWorldFieldManager4RWWorld::Pick(RwV3d& StartPos, RwV3d& EndPos, RwV3d& IntersectionPt, sNtlWorldCollisionInfo* pOutWorldCollsionInfo)
{

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


	RpCollisionWorldForAllIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldIndoorCollisionInfo, &NtlWorldCollisionInfo);
	if (dGET_COLLISION_INFO_UPDATE())
	{
		RpWorldForAllAtomicIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldIndoorCollisionInfo, &NtlWorldCollisionInfo);
	}
#ifdef dNTL_WORLD_TOOL_MODE
	if (dGET_CUR_PALETTE() == ePM_DECAL || dGET_CUR_PALETTE() == ePM_SHORE_LINE)
	{
		RpWorldForAllWorldSectorIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldIndoorCollisionInfoWater, &NtlWorldCollisionInfo);
	}
#else
	RpWorldForAllWorldSectorIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldIndoorCollisionInfoWater, &NtlWorldCollisionInfo);
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

RwBool CNtlWorldFieldManager4RWWorld::PickTerrain(RwV3d& StartPos, RwV3d& EndPos, RwV3d& IntersectionPt)
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

	RpCollisionWorldForAllIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldIndoorCollisionInfo, &NtlWorldCollisionInfo);
	if (dGET_COLLISION_INFO_UPDATE())
	{
		RpWorldForAllAtomicIntersections(m_pRpWorld, &rpIntersection, GetNtlWorldIndoorCollisionInfo, &NtlWorldCollisionInfo);
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

RwBool CNtlWorldFieldManager4RWWorld::GetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp)
{
	return m_pWorldBlockMananger->GetAFieldProp(Pos, NtlFieldProp);
}

RwBool CNtlWorldFieldManager4RWWorld::SetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp, eNTL_FIELD_PROPID NtlFieldPropID)
{
	return m_pWorldBlockMananger->SetAFieldProp(Pos, NtlFieldProp, NtlFieldPropID);
}

RwBool CNtlWorldFieldManager4RWWorld::GetABlockProp(RwV3d& Pos, sNTL_BLOCK_PROP& NtlBlockProp)
{
	return m_pWorldBlockMananger->GetABlockProp(Pos, NtlBlockProp);
}

RwBool CNtlWorldFieldManager4RWWorld::SetABlockProp(RwV3d& Pos, sNTL_BLOCK_PROP& NtlBlockProp, eNTL_BLOCK_PROPID NtlBlockPropID)
{
	return m_pWorldBlockMananger->SetABlockProp(Pos, NtlBlockProp, NtlBlockPropID);
}

void CNtlWorldFieldManager4RWWorld::SetAnotherField(RwBool ChangeStraightAway)
{
	CNtlWorldFieldManager::SetAnotherField(ChangeStraightAway);

	m_pWorldBlockMananger->SetAnotherBlock((ChangeStraightAway ? 0.001f : dFIELD_EFFECT_SWITCHING_TIME));
}

void CNtlWorldFieldManager4RWWorld::UpdateMsg(RwV3d& Pos)
{
	// world field switching effect : 인도어에서는 FieldChanged Message를 보내지 않는다.
// 	if(GetFieldPropVariationStarting())
// 	{
// 		// Update current map name
// 		UpdateCurMapName(Pos);
// 
// 		sNTL_FIELD_PROP NtlFieldProp;
// 		GetAFieldProp(Pos, NtlFieldProp);
// 		CNtlPLEventGenerator::IsAnotherFieldChanged(&NtlFieldProp);
// 	}
}

RwBool CNtlWorldFieldManager4RWWorld::UpdateFieldMap(RwV3d& Pos)
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

	// send msgs right after another fields : 인도어 에서는 FieldChagedMessage를 보내지 않는다.
	// CNtlPLEventGenerator::IsAnotherFieldChangedWithoutDelay();

	return TRUE;
}

void CNtlWorldFieldManager4RWWorld::CreateSectorMap(RpWorld *pNtlWorld)
{
	RpSector *spSect;
	RpSector *spaStack[rpWORLDMAXBSPDEPTH];
	RwInt32 nStack = 0;

	spSect = pNtlWorld->rootSector;

	do
	{
		if (spSect->type < 0) // Is RpWorldSector
		{
			// hook the sector; it's atomic and leaf sector
			SetWorldSectorIndoor((RpWorldSector*)spSect);

			spSect = spaStack[nStack--];
		}
		else // Is RpPlaneSector
		{
			RpPlaneSector *pspPlane = (RpPlaneSector *) spSect;

			// left then right
			spSect = pspPlane->leftSubTree;
			spaStack[++nStack] = pspPlane->rightSubTree;
		}
	}
	while (nStack >= 0);

	RwReal fSectorHalfSize = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2);
	for (RwInt32 iZ = -dGET_WORLD_PARAM()->WorldSizeHalf; iZ < dGET_WORLD_PARAM()->WorldSizeHalf; iZ += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for (RwInt32 iX = -dGET_WORLD_PARAM()->WorldSizeHalf; iX < dGET_WORLD_PARAM()->WorldSizeHalf; iX += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			RwV3d vPos;
			vPos.x = static_cast<RwReal>(iX);
			vPos.z = static_cast<RwReal>(iZ);
			
			RwInt32 iSectorIdx = GetSectorIdx(vPos);

			if (iSectorIdx != -1)
			{
				m_pSectors[iSectorIdx].DatumPoint.x = vPos.x + fSectorHalfSize;
				m_pSectors[iSectorIdx].DatumPoint.y = 0.0f;
				m_pSectors[iSectorIdx].DatumPoint.z = vPos.z + fSectorHalfSize;
			}
		}
	}
}

void CNtlWorldFieldManager4RWWorld::SetWorldSectorIndoor(RpWorldSector* pWorldSector)
{
	SRpWorldSectorIndoor* psRpWorldSectorIndoor = m_mapSectors[pWorldSector];
	if (!psRpWorldSectorIndoor)
	{
		m_mapSectors[pWorldSector] = psRpWorldSectorIndoor = NTL_NEW SRpWorldSectorIndoor;
		psRpWorldSectorIndoor->sphereBounding.center = (pWorldSector->tightBoundingBox.sup + pWorldSector->tightBoundingBox.inf) * 0.5f;
		psRpWorldSectorIndoor->sphereBounding.radius = RwV3dLength(&((pWorldSector->tightBoundingBox.sup - pWorldSector->tightBoundingBox.inf) * 0.5f));
	}
	
	RwBBox	rwBBox = pWorldSector->boundingBox;
	RwV3d	vCenterCoord;

	RwReal	fSizeZ = rwBBox.sup.z - rwBBox.inf.z;
	do 
	{
		vCenterCoord.z = rwBBox.sup.z - fSizeZ;

		RwReal	fSizeX = rwBBox.sup.x - rwBBox.inf.x;
		do 
		{
			vCenterCoord.x = rwBBox.sup.x - fSizeX;

			RwInt32 iSectorIdx = GetSectorIdx(vCenterCoord);
			if (iSectorIdx != -1)
			{
				RwBool bNewSector = TRUE;
				for (RwInt32 i = 0; i < (RwInt32)psRpWorldSectorIndoor->vecNtlWorldSector.size(); ++i)
				{
					if (psRpWorldSectorIndoor->vecNtlWorldSector.at(i) == &m_pSectors[iSectorIdx])
					{
						bNewSector = FALSE;
						break;
					}
				}
				if (bNewSector)
				{
					psRpWorldSectorIndoor->vecNtlWorldSector.push_back(&m_pSectors[iSectorIdx]);
				}
			}
			if (fSizeX <= 0.0f)
			{
				break;
			}

			fSizeX -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
			if (fSizeX <= 0.0f)
			{
				fSizeX = 0.0f;
			}
		} while (true);

		if (fSizeZ <= 0.0f)
		{
			break;
		}

		fSizeZ -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
		if (fSizeZ <= 0.0f)
		{
			fSizeZ = 0.0f;
		}
	} while (true);

// 	RwBBox	rwBBox = pWorldSector->boundingBox;
// 	RwV3d	vCenterCoord;
// 
// 	for (RwReal fZ = rwBBox.inf.z; fZ < rwBBox.sup.z; fZ += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize))
// 	{
// 		vCenterCoord.z = fZ;
// 		for (RwReal fX = rwBBox.inf.x; fX < rwBBox.sup.x; fX += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize))
// 		{
// 			vCenterCoord.x = fX;
// 
// 			RwInt32 iSectorIdx = GetSectorIdx(vCenterCoord);
// 			if (iSectorIdx != -1)
// 			{
// 				RwBool bNewSector = TRUE;
// 				for (RwInt32 i = 0; i < (RwInt32)m_mapSectors[pWorldSector].size(); ++i)
// 				{
// 					if (m_mapSectors[pWorldSector].at(i) == &m_pSectors[iSectorIdx])
// 					{
// 						bNewSector = FALSE;
// 						break;
// 					}
// 				}
// 				if (bNewSector)
// 				{
// 					m_mapSectors[pWorldSector].push_back(&m_pSectors[iSectorIdx]);
// 				}
// 			}
// 		}
// 	}
}

RwBool CNtlWorldFieldManager4RWWorld::LoadProperty()
{
	if (m_pWorldBlockMananger == NULL)
	{
		return FALSE;
	}

	return m_pWorldBlockMananger->LoadProperty();
}

RwBool CNtlWorldFieldManager4RWWorld::SaveProperty()
{
#ifdef dNTL_WORLD_TOOL_MODE
	if (m_pWorldBlockMananger == NULL)
	{
		return FALSE;
	}

	return m_pWorldBlockMananger->SaveProperty();
#else
	return TRUE;
#endif
}

void CNtlWorldFieldManager4RWWorld::BeginRenderSector()
{
	m_vecVisibleSectors.clear();
}

void CNtlWorldFieldManager4RWWorld::AddRenderSector(RpWorldSector* pRpWorldSector)
{
	INDOOR_SECTOR_MAP_IT itMap = m_mapSectors.find(pRpWorldSector);
	if (itMap != m_mapSectors.end())
	{
		SRpWorldSectorIndoor* psRpWorldSectorIndoor = itMap->second;
		if (psRpWorldSectorIndoor)
		{
			for (int i = 0; i < (int)psRpWorldSectorIndoor->vecNtlWorldSector.size(); ++i)
			{
				WORLD_SECTOR_VEC_IT itVec = std::find(m_vecVisibleSectors.begin(), m_vecVisibleSectors.end(), psRpWorldSectorIndoor->vecNtlWorldSector.at(i));
				if (itVec == m_vecVisibleSectors.end())
				{
					m_vecVisibleSectors.push_back(psRpWorldSectorIndoor->vecNtlWorldSector.at(i));
				}
			}
		}
	}
}

void CNtlWorldFieldManager4RWWorld::EndRenderSector()
{
	RwFrame*	camFrame	= RwCameraGetFrame(RwCameraGetCurrentCamera());
	RwV3d*		camPos		= RwMatrixGetPos(RwFrameGetLTM(camFrame));

	for (int i = 0; i < (int)m_vecVisibleSectors.size(); ++i)
	{
		CNtlWorldSector* pNtlWorldSector = m_vecVisibleSectors.at(i);

		if(pNtlWorldSector->m_pWater)
		{
			RpAtomic *pAtomic = pNtlWorldSector->m_pWater->_pAtom;

			if(pAtomic)
			{
				const RwSphere* atomicBoundingSphere = RpAtomicGetWorldBoundingSphere(pAtomic);
				GetAtomicSorterContainer()->Push(PLSORT_WATER, pNtlWorldSector, camPos);
			}
		}

		if(GetSceneManager()->GetPlant()&& GetSceneManager()->GetPlant()->IsVisible() && !pNtlWorldSector->m_vecPlantSet.empty())
		{
			GetSceneManager()->GetPlant()->RenderSector(pNtlWorldSector);
		}
	}
}

RwBool CNtlWorldFieldManager4RWWorld::RpSectorOccluderQuery(RpWorldSector* pSector)
{
	SRpWorldSectorIndoor* psRpWorldSectorIndoor = m_mapSectors[pSector];
	if (psRpWorldSectorIndoor)
	{
		if (!CNtlOccluderManager::GetInstance()->OccluderQuery(0, EPLOCCLUDER_FUNC_TERRAIN, &psRpWorldSectorIndoor->sphereBounding, 1))
		{
			return FALSE;
		}
	}
	return TRUE;
}

//RpWorldForAllWorldSectorIntersections
RpWorldSector* CollisionWorldSectorDecalIndoor(RpIntersection * pIntersection, RpWorldSector * pRpWorldSector, void *pData)
{
	RpTriangle	*pTList		= pRpWorldSector->triangles;
	RwV3d		*pVList		= pRpWorldSector->vertices;

	for(int i = 0; i < pRpWorldSector->numTriangles; ++i) 
	{
		if(RtIntersectionBBoxTriangle(&pIntersection->t.box,
									  &pVList[pTList[i].vertIndex[0]],
									  &pVList[pTList[i].vertIndex[1]],
									  &pVList[pTList[i].vertIndex[2]]))
		{
			DecalCallbackParam*	pDecalParam = (DecalCallbackParam*)pData;
			RwInt32				nBuffCnt	= *pDecalParam->pBufferCnt;

			// 최대 개수를 넘으면 Vertex를 생성하지 않고 캔슬한다.
			if(nBuffCnt + 2 >= pDecalParam->nMaxVertextCnt)	
			{
				*(pDecalParam->pBufferCnt) += 3;
				return NULL;
			}

			RwIm3DVertex *pImv = pDecalParam->pVertices + nBuffCnt;

			RwV3d vIn[3];
			vIn[0] = pVList[pTList[i].vertIndex[0]]; vIn[0].y += pDecalParam->fYOffset;
			vIn[1] = pVList[pTList[i].vertIndex[1]]; vIn[1].y += pDecalParam->fYOffset;
			vIn[2] = pVList[pTList[i].vertIndex[2]]; vIn[2].y += pDecalParam->fYOffset;

			RwV3d v0, v1, vN;
			RwV3dSubMacro(&v0, &vIn[1], &vIn[0]);
			RwV3dSubMacro(&v1, &vIn[2], &vIn[0]);
			RwV3dCrossProduct(&vN, &v0, &v1);

			if (0.0f <= RwV3dDotProduct(&vN, &CNtlPLGlobal::m_vYAxisV3))
			{
				RwIm3DVertexSetPos(pImv, vIn[0].x, vIn[0].y, vIn[0].z);
				RwIm3DVertexSetPos(pImv+1, vIn[1].x, vIn[1].y, vIn[1].z);
				RwIm3DVertexSetPos(pImv+2, vIn[2].x, vIn[2].y, vIn[2].z);

				*(pDecalParam->pBufferCnt) += 3;
			}
		}
	}	

	return pRpWorldSector;
}

//RpCollisionWorldForAllIntersections
RpCollisionTriangle* CollisionWorldSectorDecalIndoor(RpIntersection *pIntersection, RpWorldSector* pRpWorldSector, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void* pData)
{
	DecalCallbackParam*	pDecalParam = (DecalCallbackParam*)pData;
	RwInt32				nBuffCnt	= *pDecalParam->pBufferCnt;

	// 최대 개수를 넘으면 Vertex를 생성하지 않고 캔슬한다.
	if(nBuffCnt + 2 >= pDecalParam->nMaxVertextCnt)
	{
		*(pDecalParam->pBufferCnt) += 3;
		return NULL;
	}

	RwIm3DVertex *pImv = pDecalParam->pVertices + nBuffCnt;

	RwV3d vIn[3];
	vIn[0] = *(pRpCollisionTriangle->vertices[0]); vIn[0].y += pDecalParam->fYOffset;
	vIn[1] = *(pRpCollisionTriangle->vertices[1]); vIn[1].y += pDecalParam->fYOffset;
	vIn[2] = *(pRpCollisionTriangle->vertices[2]); vIn[2].y += pDecalParam->fYOffset;

	RwV3d v0, v1, vN;
	RwV3dSubMacro(&v0, &vIn[1], &vIn[0]);
	RwV3dSubMacro(&v1, &vIn[2], &vIn[0]);
	RwV3dCrossProduct(&vN, &v0, &v1);

	if (0.0f <= RwV3dDotProduct(&vN, &CNtlPLGlobal::m_vYAxisV3))
	{
		RwIm3DVertexSetPos(pImv, vIn[0].x, vIn[0].y, vIn[0].z);
		RwIm3DVertexSetPos(pImv+1, vIn[1].x, vIn[1].y, vIn[1].z);
		RwIm3DVertexSetPos(pImv+2, vIn[2].x, vIn[2].y, vIn[2].z);

		*(pDecalParam->pBufferCnt) += 3;
	}

	return pRpCollisionTriangle;
}

#include "PerformanceChecker.h"
RwBool CNtlWorldFieldManager4RWWorld::GetWorldDecal(RwV3d& vPosition, RwV3d& vSize, RwInt32 nPolyMaxCount, RwInt32& nRenderVertexCount, RwReal& fUpdateVertextRadius, RwIm3DVertex* pVertices, RwReal OffsetY)
{
	NTL_FUNCTION("CNtlWorldFieldManager4RWWorld::GetWorldDecal");
	
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

	DecalCallbackParam decalCallbackParam;		
	decalCallbackParam.vPos					= vPosition;
	decalCallbackParam.pBufferCnt			= (RwInt32*)&nRenderVertexCount; ///< 현재까지 생성된 Vertex 버퍼의 다음부터 담는다.
	decalCallbackParam.pVertices			= pVertices;			
	decalCallbackParam.fIntersectionRadius	= sqrtf(vSize.x * vSize.x + vSize.z * vSize.z) * 0.5f;
	decalCallbackParam.fScale				= 1.0f;
	decalCallbackParam.nMaxVertextCnt		= nPolyMaxCount;
	decalCallbackParam.fYOffset				= OffsetY;

	RpIntersection intersection;
	intersection.type = rpINTERSECTBOX;
	intersection.t.box.sup.x = vPosition.x + (vSize.x * 0.5f);
	intersection.t.box.inf.x = vPosition.x - (vSize.x * 0.5f);
	intersection.t.box.sup.y = vPosition.y + (decalCallbackParam.fIntersectionRadius/*vSize.y * 0.5f*/);
	intersection.t.box.inf.y = vPosition.y - (decalCallbackParam.fIntersectionRadius/*vSize.y * 0.5f*/);
	intersection.t.box.sup.z = vPosition.z + (vSize.z * 0.5f);
	intersection.t.box.inf.z = vPosition.z - (vSize.z * 0.5f);

	//RpWorldForAllWorldSectorIntersections(CNtlPLGlobal::m_pRpWorld, &intersection, CollisionWorldSectorDecalIndoor, &decalCallbackParam);
	RpCollisionWorldForAllIntersections(CNtlPLGlobal::m_pRpWorld, &intersection, CollisionWorldSectorDecalIndoor, &decalCallbackParam);

	fUpdateVertextRadius = decalCallbackParam.fIntersectionRadius;

	if (nRenderVertexCount > nPolyMaxCount)
	{
		NTL_RETURN(FALSE);
	}
	NTL_RETURN(TRUE);
}

#ifdef dNTL_WORLD_TOOL_MODE
void CNtlWorldFieldManager4RWWorld::OnWaterProc(RwInt32 IdxMenu, RwInt32 IdxSector, sSECTOR_WATER_ATTR& SectorWaterAttr)
{
	switch(IdxMenu)
	{
	case 0:
		{
			RwV3d Pos;
			Pos.x = m_pSectors[IdxSector].DatumPoint.x;
			Pos.z = m_pSectors[IdxSector].DatumPoint.z;
			SectorWaterAttr._Height = 50.0f;
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

void CNtlWorldFieldManager4RWWorld::RenderBlockBBox()
{
	for (int i = 0; i < (int)m_vecVisibleSectors.size(); ++i)
	{
		CNtlWorldSector* pNtlWorldSector = m_vecVisibleSectors.at(i);
		
		RwV3d vBlockPos;
		vBlockPos.x = pNtlWorldSector->DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
		vBlockPos.z = pNtlWorldSector->DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
		for(int l = (int)vBlockPos.z; l < (int)vBlockPos.z + dGET_WORLD_PARAM()->WorldSectorSize; l += dGET_WORLD_PARAM()->WorldBlockSize)
		{
			for(int m = (int)vBlockPos.x; m < (int)vBlockPos.x + dGET_WORLD_PARAM()->WorldSectorSize; m += dGET_WORLD_PARAM()->WorldBlockSize)
			{
				RwV3d vPos;
				vPos.x = (RwReal)m;
				vPos.z = (RwReal)l;

				sNTL_BLOCK_PROP sNtlBlockProp;
				if (m_pWorldBlockMananger->GetABlockProp(vPos, sNtlBlockProp))
				{
					if (sNtlBlockProp._BBox.sup.y == RwRealMINVAL ||
						sNtlBlockProp._BBox.inf.y == RwRealMAXVAL)
					{
						continue;
					}

					RwRGBA rgba = { 0, 0, 0, 25 };
					RenderGeometryBox(NULL, &sNtlBlockProp._BBox, &rgba, &rgba, 0.01f, FALSE);
				}
			}
		}
	}
}

void CNtlWorldFieldManager4RWWorld::RenderBlockGuide()
{
	sLINE3D	sLine[2];

	RwBBox	bbox		= CNtlPLGlobal::m_RwCamera->frustumBoundBox;
	RwReal	fBlockSize	= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize);

	bbox.inf.x = static_cast<RwReal>(static_cast<RwInt32>(bbox.inf.x) / static_cast<RwInt32>(fBlockSize) - 1) * fBlockSize;
	bbox.inf.y = CNtlPLGlobal::m_fIndoorPlaneHeight;
	bbox.inf.z = static_cast<RwReal>(static_cast<RwInt32>(bbox.inf.z) / static_cast<RwInt32>(fBlockSize) - 1) * fBlockSize;
	bbox.sup.x = static_cast<RwReal>(static_cast<RwInt32>(bbox.sup.x) / static_cast<RwInt32>(fBlockSize) + 1) * fBlockSize;
	bbox.sup.y = CNtlPLGlobal::m_fIndoorPlaneHeight;
	bbox.sup.z = static_cast<RwReal>(static_cast<RwInt32>(bbox.sup.z) / static_cast<RwInt32>(fBlockSize) + 1) * fBlockSize;

	m_VertCount = 0;
	for (RwReal z = bbox.inf.z; z < bbox.sup.z; z += fBlockSize)
	{
		sLine[0].Col = 0xff000000;
		sLine[1].Col = 0xff000000;

		sLine[0].Pos.x = bbox.inf.x;
		sLine[0].Pos.y = bbox.inf.y;
		sLine[0].Pos.z = z;

		sLine[1].Pos.x = bbox.sup.x;
		sLine[1].Pos.y = bbox.sup.y;
		sLine[1].Pos.z = z;

		m_Buffer[m_VertCount++] = sLine[0];
		m_Buffer[m_VertCount++] = sLine[1];

	}
	for (RwReal x = bbox.inf.x; x < bbox.sup.x; x += fBlockSize)
	{
		sLine[0].Col = 0xff000000;
		sLine[1].Col = 0xff000000;

		sLine[0].Pos.x = x;
		sLine[0].Pos.y = bbox.inf.y;
		sLine[0].Pos.z = bbox.inf.z;

		sLine[1].Pos.x = x;
		sLine[1].Pos.y = bbox.sup.y;
		sLine[1].Pos.z = bbox.sup.z;

		m_Buffer[m_VertCount++] = sLine[0];
		m_Buffer[m_VertCount++] = sLine[1];
	}

	BeginD3DLineState();

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	RwD3D9SetFVF(D3DFVF_LINE3D);
	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, static_cast<RwUInt32>(m_VertCount / 2), m_Buffer, sizeof(sLINE3D));

	EndD3DLineState();
}

void CNtlWorldFieldManager4RWWorld::RenderSectorGuide()
{
	sLINE3D	sLine[2];

	RwBBox	bbox		= CNtlPLGlobal::m_RwCamera->frustumBoundBox;
	RwReal	fBlockSize	= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	
	bbox.inf.x = static_cast<RwReal>(static_cast<RwInt32>(bbox.inf.x) / static_cast<RwInt32>(fBlockSize) - 1) * fBlockSize;
	bbox.inf.y = CNtlPLGlobal::m_fIndoorPlaneHeight;
	bbox.inf.z = static_cast<RwReal>(static_cast<RwInt32>(bbox.inf.z) / static_cast<RwInt32>(fBlockSize) - 1) * fBlockSize;
	bbox.sup.x = static_cast<RwReal>(static_cast<RwInt32>(bbox.sup.x) / static_cast<RwInt32>(fBlockSize) + 1) * fBlockSize;
	bbox.sup.y = CNtlPLGlobal::m_fIndoorPlaneHeight;
	bbox.sup.z = static_cast<RwReal>(static_cast<RwInt32>(bbox.sup.z) / static_cast<RwInt32>(fBlockSize) + 1) * fBlockSize;

	m_VertCount = 0;
	for (RwReal z = bbox.inf.z; z < bbox.sup.z; z += fBlockSize)
	{
		sLine[0].Col = 0xffff0000;
		sLine[1].Col = 0xffff0000;

		sLine[0].Pos.x = bbox.inf.x;
		sLine[0].Pos.y = bbox.inf.y;
		sLine[0].Pos.z = z;

		sLine[1].Pos.x = bbox.sup.x;
		sLine[1].Pos.y = bbox.sup.y;
		sLine[1].Pos.z = z;

		m_Buffer[m_VertCount++] = sLine[0];
		m_Buffer[m_VertCount++] = sLine[1];
		
	}
	for (RwReal x = bbox.inf.x; x < bbox.sup.x; x += fBlockSize)
	{
		sLine[0].Col = 0xffff0000;
		sLine[1].Col = 0xffff0000;

		sLine[0].Pos.x = x;
		sLine[0].Pos.y = bbox.inf.y;
		sLine[0].Pos.z = bbox.inf.z;

		sLine[1].Pos.x = x;
		sLine[1].Pos.y = bbox.sup.y;
		sLine[1].Pos.z = bbox.sup.z;

		m_Buffer[m_VertCount++] = sLine[0];
		m_Buffer[m_VertCount++] = sLine[1];
	}

	BeginD3DLineState();

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	RwD3D9SetFVF(D3DFVF_LINE3D);
	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, static_cast<RwUInt32>(m_VertCount / 2), m_Buffer, sizeof(sLINE3D));

	EndD3DLineState();
}

void CNtlWorldFieldManager4RWWorld::RenderFieldGuide()
{
	sLINE3D	sLine[2];

	RwBBox	bbox		= CNtlPLGlobal::m_RwCamera->frustumBoundBox;
	RwReal	fBlockSize	= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize);

	bbox.inf.x = static_cast<RwReal>(static_cast<RwInt32>(bbox.inf.x) / static_cast<RwInt32>(fBlockSize) - 1) * fBlockSize;
	bbox.inf.y = CNtlPLGlobal::m_fIndoorPlaneHeight;
	bbox.inf.z = static_cast<RwReal>(static_cast<RwInt32>(bbox.inf.z) / static_cast<RwInt32>(fBlockSize) - 1) * fBlockSize;
	bbox.sup.x = static_cast<RwReal>(static_cast<RwInt32>(bbox.sup.x) / static_cast<RwInt32>(fBlockSize) + 1) * fBlockSize;
	bbox.sup.y = CNtlPLGlobal::m_fIndoorPlaneHeight;
	bbox.sup.z = static_cast<RwReal>(static_cast<RwInt32>(bbox.sup.z) / static_cast<RwInt32>(fBlockSize) + 1) * fBlockSize;

	m_VertCount = 0;
	for (RwReal z = bbox.inf.z; z < bbox.sup.z; z += fBlockSize)
	{
		sLine[0].Col = 0xff0000ff;
		sLine[1].Col = 0xff0000ff;

		sLine[0].Pos.x = bbox.inf.x;
		sLine[0].Pos.y = bbox.inf.y;
		sLine[0].Pos.z = z;

		sLine[1].Pos.x = bbox.sup.x;
		sLine[1].Pos.y = bbox.sup.y;
		sLine[1].Pos.z = z;

		m_Buffer[m_VertCount++] = sLine[0];
		m_Buffer[m_VertCount++] = sLine[1];

	}
	for (RwReal x = bbox.inf.x; x < bbox.sup.x; x += fBlockSize)
	{
		sLine[0].Col = 0xff0000ff;
		sLine[1].Col = 0xff0000ff;

		sLine[0].Pos.x = x;
		sLine[0].Pos.y = bbox.inf.y;
		sLine[0].Pos.z = bbox.inf.z;

		sLine[1].Pos.x = x;
		sLine[1].Pos.y = bbox.sup.y;
		sLine[1].Pos.z = bbox.sup.z;

		m_Buffer[m_VertCount++] = sLine[0];
		m_Buffer[m_VertCount++] = sLine[1];
	}

	BeginD3DLineState();

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	RwD3D9SetFVF(D3DFVF_LINE3D);
	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, static_cast<RwUInt32>(m_VertCount / 2), m_Buffer, sizeof(sLINE3D));

	EndD3DLineState();
}

#endif