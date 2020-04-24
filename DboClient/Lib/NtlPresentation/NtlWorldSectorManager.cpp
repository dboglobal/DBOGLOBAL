#include "precomp_ntlpresentation.h"
#include "ntlworldsectormanager.h"
#include "ntlworldsector.h"
#include "ntlworldsectorinfo.h"
#include "ntlworldplugin.h"
#include "NtlPLResourceManager.h"
#include "NtlPLEventGenerator.h"
#include "NtlWorldSectorLOD.h"
#include "NtlPLOptionManager.h"
#include "PerformanceChecker.h"

#include <NTLDebug.h>

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

CNtlWorldSectorManager::CNtlWorldSectorManager(void) : m_pSectors(NULL)
{
}

CNtlWorldSectorManager::~CNtlWorldSectorManager(void)
{
}

void CNtlWorldSectorManager::Init()
{
	NTL_FUNCTION("CNtlWorldSectorManager::Init");

	m_IdxCurSector = -1;

	// generate sector lod
	dGET_SECTOR_LOD() = NTL_NEW gbDxGenLODIdx(	static_cast<LPDIRECT3DDEVICE9>(RwD3D9GetCurrentD3DDevice()),
											dGET_WORLD_PARAM()->WorldSectorTileSize,
											dGET_WORLD_PARAM()->WorldSectorTileNum);

	m_pSectors = NTL_NEW CNtlWorldSector [dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum];


	CNtlWorldSectorInfo::CreateInstance();

	NTL_RETURNVOID();
}

void CNtlWorldSectorManager::Free()
{
	NTL_FUNCTION("CNtlWorldSectorManager::Free");

	CNtlWorldSectorInfo::DestroyInstance();

	NTL_DELETE(dGET_SECTOR_LOD());

	NTL_ARRAY_DELETE(m_pSectors);

	NTL_RETURNVOID();
}

void CNtlWorldSectorManager::SetWorldSector(RpWorldSector *pWorldSector)
{
	NTL_FUNCTION("CNtlWorldSectorManager::SetWorldSector");

	if(!pWorldSector)
		NTL_ASSERTE(pWorldSector);

	RwBBox BBox = pWorldSector->boundingBox;

	RwV3d CenterCoor;
	CenterCoor.x = (BBox.sup.x + BBox.inf.x) / 2;
	CenterCoor.z = (BBox.sup.z + BBox.inf.z) / 2;

	m_pSectors[GetSectorIdx(CenterCoor)].DatumPoint.x = CenterCoor.x;
	m_pSectors[GetSectorIdx(CenterCoor)].DatumPoint.z = CenterCoor.z;
	m_pSectors[GetSectorIdx(CenterCoor)].DatumPoint.y = 0.0f;
	m_pSectors[GetSectorIdx(CenterCoor)].m_pWorldSector = pWorldSector;

	NTL_RETURNVOID();
}

RwBool CNtlWorldSectorManager::GetNeighborSectors(RwInt32 _IdxCurSector, vector<RwInt32>& _vecNeighborSectorIndices)
{
	// exceptions
	if(!IsSectorValid(_IdxCurSector))
	{
		DBO_TRACE(FALSE, "CNtlWorldSectorManager::GetNeighborSectors, invalid sector index");
		return FALSE;
	}

	// reserve 8 space for indices
	_vecNeighborSectorIndices.reserve(8);

	// calculate neighbor sectors
	RwV3d	PosTmpSectorS;
	RwV3d	PosCurSectorS;
	PosCurSectorS.x = m_pSectors[_IdxCurSector].DatumPoint.x;
	PosCurSectorS.y = m_pSectors[_IdxCurSector].DatumPoint.y;
	PosCurSectorS.z = m_pSectors[_IdxCurSector].DatumPoint.z;

	// 3---2---1
	// |       | 
	// 4   8   0
	// |       |
	// 5---6---7

	// 0
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.x		-=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 1
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.x		-=	dGET_WORLD_PARAM()->WorldSectorSize;
	PosTmpSectorS.z		+=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 2
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.z		+=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 3
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.x		+=	dGET_WORLD_PARAM()->WorldSectorSize;
	PosTmpSectorS.z		+=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 4
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.x		+=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 5
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.x		+=	dGET_WORLD_PARAM()->WorldSectorSize;
	PosTmpSectorS.z		-=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 6
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.z		-=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 7
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.x		-=	dGET_WORLD_PARAM()->WorldSectorSize;
	PosTmpSectorS.z		-=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	return TRUE;
}

RwBool CNtlWorldSectorManager::IsSectorValid(RwInt32 Idx)
{
	// verify the sector index is being in current world
	if(Idx < 0 || Idx > (dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum - 1))
		return FALSE;
	
	return TRUE;
}

RwBool CNtlWorldSectorManager::IsSectorValid(RwV3d& Pos)
{
	if(Pos.x < dGET_WORLD_PARAM()->WorldValueMax &&
	   Pos.z < dGET_WORLD_PARAM()->WorldValueMax &&
	   Pos.x >= dGET_WORLD_PARAM()->WorldValueMin &&
	   Pos.z >= dGET_WORLD_PARAM()->WorldValueMin)
		return TRUE;

	return FALSE;
}

RwInt32 CNtlWorldSectorManager::GetSectorIdx(RwV3d& Pos)
{
	if(!IsSectorValid(Pos))
		return -1;

	RwInt32 RetIdx = CNtlMath::GetSafeIdx3D(Pos, dGET_WORLD_PARAM()->WorldSizeHalf, dGET_WORLD_PARAM()->WorldSectorSize, dGET_WORLD_PARAM()->WorldSectorNum);

// 	RwInt32 WidthNum, DepthNum;
// 	RwReal XBasedOn0 = (RwReal)((RwInt32)Pos.x + dGET_WORLD_PARAM()->WorldSizeHalf);
// 	RwReal ZBasedOn0 = (RwReal)((RwInt32)Pos.z + dGET_WORLD_PARAM()->WorldSizeHalf);
// 	WidthNum = (RwInt32)(XBasedOn0 / (RwReal)dGET_WORLD_PARAM()->WorldSectorSize);
// 	DepthNum = (RwInt32)(ZBasedOn0 / (RwReal)dGET_WORLD_PARAM()->WorldSectorSize); 
// 
// 	RwInt32 RetIdx = WidthNum + (DepthNum * dGET_WORLD_PARAM()->WorldSectorNum);
	if(!IsSectorValid(RetIdx))
		return -1;

	return RetIdx;
}

RwBool CNtlWorldSectorManager::IsSectorLoaded(RwInt32 SectorIdx)
{
	NTL_FUNCTION("CNtlWorldSectorManager::IsSectorLoaded");

	if (m_pSectors[SectorIdx].m_bLoaded)
	{
		NTL_RETURN(TRUE);
	}
	NTL_RETURN(FALSE);
}

RwBool CNtlWorldSectorManager::CreateInMemory(RwInt32 SectorIdx)
{
	NTL_FUNCTION("CNtlWorldSectorManager::CreateInMemory");

	#ifdef dNTL_WORLD_TOOL_MODE
	
		RpNtlWorldSectorGeometryCreate(&m_pSectors[SectorIdx]);
	
	#endif

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldSectorManager::DeleteInMemory(RwInt32 SectorIdx, RwBool SaveSwapInToolMode/* = TRUE*/)
{
	NTL_FUNCTION("CNtlWorldSectorManager::DeleteInMemory");

	if(SaveSwapInToolMode)
	{
		RpNtlWorldSectorGeometryDelete(&m_pSectors[SectorIdx]);
	}
	else
	{
		RpNtlWorldSectorGeometryDeleteWithoutSwapFileSaving(&m_pSectors[SectorIdx]);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldSectorManager::CreateFromFile(sCUR_FIELD_TEX_INFO& FieldTexInfo, RwInt32 SectorIdx, FILE* pFile)
{
	NTL_FUNCTION("CNtlWorldSectorManager::CreateFromFile");

	RpNtlWorldSectorGeometryCreateFromFile(FieldTexInfo, &m_pSectors[SectorIdx], pFile);

#ifdef dNTL_WORLD_COMBINE
	if (CNtlPLOptionManager::GetInstance()->GetTerrainTextrueCombine())
	{
		m_pSectors[SectorIdx].m_pTextureCombine = NtlTerrainCombineTextureGenerator(SectorIdx);
	}
#endif

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldSectorManager::DeleteIntoFile(RwInt32 SectorIdx, FILE* pFile)
{
	NTL_FUNCTION("CNtlWorldSectorManager::DeleteIntoFile");

	RpNtlWorldSectorGeometryDeleteIntoFile(&m_pSectors[SectorIdx], pFile);

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldSectorManager::UpdateTexLyr(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, RwInt32 SectorIdx, RwInt32 IdxLyr, RwInt32 IdxMenu)
{
	NTL_FUNCTION("CNtlWorldSectorManager::UpdateTexLyr");

	NTL_RETURN(RpNtlWorldSectorUpdateTexLyr(CurFieldTexInfo, &m_pSectors[SectorIdx], IdxLyr, IdxMenu));
}

RwBool CNtlWorldSectorManager::UpdateTexMat(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, RwInt32 SectorIdx, RwInt32 IdxLyr)
{
	NTL_FUNCTION("CNtlWorldSectorManager::UpdateTexMat");
	
	NTL_RETURN(RpNtlWorldSectorUpdateTexMat(CurFieldTexInfo, &m_pSectors[SectorIdx], IdxLyr));
}

RwBool CNtlWorldSectorManager::UpdateSectorMap(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWorldSectorManager::UpdateSectorMap");
	RwInt32	CurSecIdx = GetSectorIdx(Pos);
	if (CurSecIdx <= -1)
	{
		NTL_RETURN(FALSE);
	}

	if (CurSecIdx != m_IdxCurSector)
	{
		m_IdxCurSector = CurSecIdx;

		// send event
		CNtlPLEventGenerator::IsAnotherSectorChangedWithoutDelay(&m_pSectors[m_IdxCurSector]);
	}

	NTL_RETURN(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

CNtlWorldSectorManager::CNtlWorldSectorManager(void) : m_pSectors(NULL)
{
}

CNtlWorldSectorManager::~CNtlWorldSectorManager(void)
{
}

void CNtlWorldSectorManager::Init()
{
	NTL_FUNCTION("CNtlWorldSectorManager::Init");

	m_IdxCurSector = -1;

	// generate sector lod
	dGET_SECTOR_LOD() = NTL_NEW gbDxGenLODIdx(	static_cast<LPDIRECT3DDEVICE9>(RwD3D9GetCurrentD3DDevice()),
		dGET_WORLD_PARAM()->WorldSectorTileSize,
		dGET_WORLD_PARAM()->WorldSectorTileNum);

	m_pSectors = NTL_NEW CNtlWorldSector [dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum];

	NTL_RETURNVOID();
}

void CNtlWorldSectorManager::Free()
{
	NTL_FUNCTION("CNtlWorldSectorManager::Free");

	NTL_DELETE(dGET_SECTOR_LOD());

	NTL_ARRAY_DELETE(m_pSectors);

	NTL_RETURNVOID();
}

void CNtlWorldSectorManager::SetWorldSector(RpWorldSector *pWorldSector)
{
	NTL_FUNCTION("CNtlWorldSectorManager::SetWorldSector");

	if(!pWorldSector)
		NTL_ASSERTE(pWorldSector);

	RwBBox BBox = pWorldSector->boundingBox;

	RwV3d CenterCoor;
	CenterCoor.x = (BBox.sup.x + BBox.inf.x) / 2;
	CenterCoor.z = (BBox.sup.z + BBox.inf.z) / 2;

	m_pSectors[GetSectorIdx(CenterCoor)].DatumPoint.x = CenterCoor.x;
	m_pSectors[GetSectorIdx(CenterCoor)].DatumPoint.z = CenterCoor.z;
	m_pSectors[GetSectorIdx(CenterCoor)].DatumPoint.y = 0.0f;
	m_pSectors[GetSectorIdx(CenterCoor)].m_pWorldSector = pWorldSector;

	NTL_RETURNVOID();
}

RwBool CNtlWorldSectorManager::GetNeighborSectors(RwInt32 _IdxCurSector, vector<RwInt32>& _vecNeighborSectorIndices)
{
	// exceptions
	if(!IsSectorValid(_IdxCurSector))
	{
		DBO_TRACE(FALSE, "CNtlWorldSectorManager::GetNeighborSectors, invalid sector index");
		return FALSE;
	}

	// reserve 8 space for indices
	_vecNeighborSectorIndices.reserve(8);

	// calculate neighbor sectors
	RwV3d	PosTmpSectorS;
	RwV3d	PosCurSectorS;
	PosCurSectorS.x = m_pSectors[_IdxCurSector].DatumPoint.x;
	PosCurSectorS.y = m_pSectors[_IdxCurSector].DatumPoint.y;
	PosCurSectorS.z = m_pSectors[_IdxCurSector].DatumPoint.z;

	// 3---2---1
	// |       | 
	// 4   8   0
	// |       |
	// 5---6---7

	// 0
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.x		-=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 1
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.x		-=	dGET_WORLD_PARAM()->WorldSectorSize;
	PosTmpSectorS.z		+=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 2
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.z		+=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 3
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.x		+=	dGET_WORLD_PARAM()->WorldSectorSize;
	PosTmpSectorS.z		+=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 4
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.x		+=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 5
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.x		+=	dGET_WORLD_PARAM()->WorldSectorSize;
	PosTmpSectorS.z		-=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 6
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.z		-=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	// 7
	PosTmpSectorS		=	PosCurSectorS;
	PosTmpSectorS.x		-=	dGET_WORLD_PARAM()->WorldSectorSize;
	PosTmpSectorS.z		-=	dGET_WORLD_PARAM()->WorldSectorSize;
	_vecNeighborSectorIndices.push_back(GetSectorIdx(PosTmpSectorS));

	return TRUE;
}

RwBool CNtlWorldSectorManager::IsSectorValid(RwInt32 Idx)
{
	// verify the sector index is being in current world
	if(Idx < 0 || Idx > (dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum - 1))
		return FALSE;

	return TRUE;
}

RwBool CNtlWorldSectorManager::IsSectorValid(RwV3d& Pos)
{
	if(Pos.x < dGET_WORLD_PARAM()->WorldValueMax &&
		Pos.z < dGET_WORLD_PARAM()->WorldValueMax &&
		Pos.x >= dGET_WORLD_PARAM()->WorldValueMin &&
		Pos.z >= dGET_WORLD_PARAM()->WorldValueMin)
		return TRUE;

	return FALSE;
}

RwInt32 CNtlWorldSectorManager::GetSectorIdx(RwV3d& Pos)
{
	if(!IsSectorValid(Pos))
		return -1;

	RwInt32 RetIdx = CNtlMath::GetSafeIdx3D(Pos, dGET_WORLD_PARAM()->WorldSizeHalf, dGET_WORLD_PARAM()->WorldSectorSize, dGET_WORLD_PARAM()->WorldSectorNum);

// 	RwInt32 WidthNum, DepthNum;
// 	RwReal XBasedOn0 = (RwReal)((RwInt32)Pos.x + dGET_WORLD_PARAM()->WorldSizeHalf);
// 	RwReal ZBasedOn0 = (RwReal)((RwInt32)Pos.z + dGET_WORLD_PARAM()->WorldSizeHalf);
// 	WidthNum = (RwInt32)(XBasedOn0 / (RwReal)dGET_WORLD_PARAM()->WorldSectorSize);
// 	DepthNum = (RwInt32)(ZBasedOn0 / (RwReal)dGET_WORLD_PARAM()->WorldSectorSize); 
// 
// 	RwInt32 RetIdx = WidthNum + (DepthNum * dGET_WORLD_PARAM()->WorldSectorNum);
	if(!IsSectorValid(RetIdx))
		return -1;

	return RetIdx;
}

RwBool CNtlWorldSectorManager::IsSectorLoaded(RwInt32 SectorIdx)
{
	NTL_FUNCTION("CNtlWorldSectorManager::IsSectorLoaded");

	if (m_pSectors[SectorIdx].m_bLoaded)
	{
		NTL_RETURN(TRUE);
	}
	NTL_RETURN(FALSE);
}

RwBool CNtlWorldSectorManager::CreateInMemory(RwInt32 SectorIdx)
{
	NTL_FUNCTION("CNtlWorldSectorManager::CreateInMemory");

#ifdef dNTL_WORLD_TOOL_MODE

	RpNtlWorldSectorGeometryCreate(&m_pSectors[SectorIdx]);

#endif

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldSectorManager::DeleteInMemory(RwInt32 SectorIdx, RwBool SaveSwapInToolMode/* = TRUE*/)
{
	NTL_FUNCTION("CNtlWorldSectorManager::DeleteInMemory");

	if(SaveSwapInToolMode)
	{
		RpNtlWorldSectorGeometryDelete(&m_pSectors[SectorIdx]);
	}
	else
	{
		_RpNtlWorldSectorGeometryDeleteWithoutSwapFileSaving(&m_pSectors[SectorIdx]);
	}

	NTL_RETURN(TRUE);
}

#include "NtlWorldShadow.h"
RwBool CNtlWorldSectorManager::CreateFromFile(sCUR_FIELD_TEX_INFO& FieldTexInfo, RwInt32 SectorIdx, FILE* pFile)
{
	NTL_FUNCTION("CNtlWorldSectorManager::CreateFromFile");

	RpNtlWorldSectorGeometryCreateFromFile(FieldTexInfo, &m_pSectors[SectorIdx], pFile);
	
// #ifdef dNTL_WORLD_TOOL_MODE
// 	CNtlWorldShadow* pNtlWorldShadow = m_pSectors[SectorIdx].m_pNtlWorldShadow;
// 	if (pNtlWorldShadow && pNtlWorldShadow->m_pTexShadow)
// 	{
// 		if ((pNtlWorldShadow->m_rgbaShadow.red == 128 &&
// 			pNtlWorldShadow->m_rgbaShadow.green == 128 &&
// 			pNtlWorldShadow->m_rgbaShadow.blue == 128) &&
// 			(pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.red == 128 &&
// 			pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.green == 128 &&
// 			pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.blue == 128))
// 		{
// 			NTL_TRACE(FALSE, "[SECTOR_SHADOW_DEFAULT] IDX(" << SectorIdx << ")");
// 		}
// 		else if ((pNtlWorldShadow->m_rgbaShadow.red == 128 &&
// 			pNtlWorldShadow->m_rgbaShadow.green == 128 &&
// 			pNtlWorldShadow->m_rgbaShadow.blue == 128) &&
// 			(pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.red != 128 ||
// 			pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.green != 128 ||
// 			pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.blue != 128))
// 		{
// 			pNtlWorldShadow->m_rgbaShadow = pNtlWorldShadow->m_NtlWorldShadowParam._RGBA;
// 		}
// 		else if ((pNtlWorldShadow->m_rgbaShadow.red != 128 ||
// 			pNtlWorldShadow->m_rgbaShadow.green != 128 ||
// 			pNtlWorldShadow->m_rgbaShadow.blue != 128) &&
// 			(pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.red == 128 &&
// 			pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.green == 128 &&
// 			pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.blue == 128))
// 		{
// 			pNtlWorldShadow->m_NtlWorldShadowParam._RGBA = pNtlWorldShadow->m_rgbaShadow;
// 		}
// 		else if (pNtlWorldShadow->m_rgbaShadow.red != pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.red ||
// 			pNtlWorldShadow->m_rgbaShadow.green != pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.green ||
// 			pNtlWorldShadow->m_rgbaShadow.blue != pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.blue)
// 		{
// 			NTL_TRACE(FALSE, "[SECTOR_SHADOW_COLOR] IDX(" << SectorIdx << ") ShadowColor(" <<
// 				(RwInt32)pNtlWorldShadow->m_rgbaShadow.red << "," << (RwInt32)pNtlWorldShadow->m_rgbaShadow.green << "," <<
// 				(RwInt32)pNtlWorldShadow->m_rgbaShadow.blue << ") SwapColor(" << (RwInt32)pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.red << "," <<
// 				(RwInt32)pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.green << "," << (RwInt32)pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.blue << ")");
// 			pNtlWorldShadow->m_NtlWorldShadowParam._RGBA = pNtlWorldShadow->m_rgbaShadow;
// 		}
// 	}
// #endif

#ifdef dNTL_WORLD_COMBINE
	if (CNtlPLOptionManager::GetInstance()->GetTerrainTextrueCombine())
	{
		m_pSectors[SectorIdx].m_pTextureCombine = NtlTerrainCombineTextureGenerator(SectorIdx);
	}
#endif

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldSectorManager::DeleteIntoFile(RwInt32 SectorIdx, FILE* pFile)
{
	NTL_FUNCTION("CNtlWorldSectorManager::DeleteIntoFile");

	RpNtlWorldSectorGeometryDeleteIntoFile(&m_pSectors[SectorIdx], pFile);

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldSectorManager::UpdateTexLyr(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, RwInt32 SectorIdx, RwInt32 IdxLyr, RwInt32 IdxMenu)
{
	NTL_FUNCTION("CNtlWorldSectorManager::UpdateTexLyr");

	NTL_RETURN(RpNtlWorldSectorUpdateTexLyr(CurFieldTexInfo, &m_pSectors[SectorIdx], IdxLyr, IdxMenu));
}

RwBool CNtlWorldSectorManager::UpdateTexMat(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, RwInt32 SectorIdx, RwInt32 IdxLyr)
{
	NTL_FUNCTION("CNtlWorldSectorManager::UpdateTexMat");

	NTL_RETURN(RpNtlWorldSectorUpdateTexMat(CurFieldTexInfo, &m_pSectors[SectorIdx], IdxLyr));
}

RwBool CNtlWorldSectorManager::UpdateSectorMap(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWorldSectorManager::UpdateSectorMap");
	RwInt32	CurSecIdx = GetSectorIdx(Pos);
	if (CurSecIdx <= -1)
	{
		NTL_RETURN(FALSE);
	}

	if (CurSecIdx != m_IdxCurSector)
	{
		m_IdxCurSector = CurSecIdx;

		// send event
		CNtlPLEventGenerator::IsAnotherSectorChangedWithoutDelay(&m_pSectors[m_IdxCurSector]);
	}

	NTL_RETURN(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif