/******************************************************************************************************************************
 *
 * ntlworldinterface.h
 *
 * copyright (c) 2005 ntl-inc Ltd.
 *
 * author : woody1019
 *
 * purpose : 
 *
 ******************************************************************************************************************************/


/******************************************************************************************************************************
 * headers
 ******************************************************************************************************************************/
#include "precomp_ntlpresentation.h"
#include "ntlworldinterface.h"
#include "NtlWorldSector.h"
#include "NtlDebug.h"
#include "NtlMath.h"


RpWorldSector* RpNtlWorldGetVisibleSectorsCB(RpWorldSector *pWorldSector, void* pData)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);
	if(!pNtlSector)
	{
		return pWorldSector;
	}

	vector<RwInt32>* pVecVisibleSectors = static_cast< vector<RwInt32>* >(pData);

	RwV3d Pos;
	CNtlMath::MathRwV3dAssign(&Pos, pNtlSector->pNtlWorldSector->DatumPoint.x, 0.0f, pNtlSector->pNtlWorldSector->DatumPoint.z);

	RwInt32 WidthNum	= (int)(( Pos.x + dGET_WORLD_PARAM()->WorldSizeHalf ) / (float)dGET_WORLD_PARAM()->WorldSectorSize);
	RwInt32 DepthNum	= (int)(( Pos.z + dGET_WORLD_PARAM()->WorldSizeHalf ) / (float)dGET_WORLD_PARAM()->WorldSectorSize); 
	RwInt32 RetIdx		= WidthNum + (DepthNum * dGET_WORLD_PARAM()->WorldSectorNum);

	// verify the sector index is being in current world
	if(RetIdx < 0 || RetIdx > (dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum - 1))
	{
		return pWorldSector;
	}

	pVecVisibleSectors->push_back(RetIdx);

	return pWorldSector;
}

void RpNtlWorldGetVisibleSectors(vector<RwInt32>* vecVisibleSectors)
{
	RwCameraForAllSectorsInFrustum(CNtlPLGlobal::m_RwCamera, RpNtlWorldGetVisibleSectorsCB, vecVisibleSectors);
}

/******************************************************************************************************************************
* NtlWorldImportTerminationBuildCB : initialzie spatial algorithm related terminator
******************************************************************************************************************************/
RwBool NtlWorldImportTerminationBuildCB(RtWorldImportBuildSector *pBuildSector, RtWorldImportBuildStatus *pBuildStatus, void *pData)
{
	if((pBuildSector->boundingBox.sup.x - pBuildSector->boundingBox.inf.x) == dGET_WORLD_PARAM()->WorldSectorSize &&
	   (pBuildSector->boundingBox.sup.z - pBuildSector->boundingBox.inf.z) == dGET_WORLD_PARAM()->WorldSectorSize)
		return TRUE;

	return FALSE;
}

/******************************************************************************************************************************
* NtlWorldImportPartitionBuildCB : initialzie spatial algorithm related constructor
******************************************************************************************************************************/
RwReal NtlWorldImportPartitionBuildCB(RtWorldImportBuildSector *pBuildSector, RtWorldImportBuildStatus *pBuildStatus, RtWorldImportPartition *pPartition, void *pData) 
{
	if((pBuildSector->boundingBox.sup.x - pBuildSector->boundingBox.inf.x) == (pBuildSector->boundingBox.sup.z - pBuildSector->boundingBox.inf.z))
	{
		/* this partition is square. therefore, we split it perp. to the x axis */

		/* Perp. to the X axis */
		pPartition->type = 0;  

		pPartition->value = (pBuildSector->boundingBox.sup.x - pBuildSector->boundingBox.inf.x) / 2 + pBuildSector->boundingBox.inf.x;
		RtWorldImportSetPartitionStatistics(pBuildSector, pPartition);
	}
	else
	{
		/* this partition is not square. therefore, we split it perp. to the z axis */

		/* perp. to the z axis */
		pPartition->type = 8;  

		pPartition->value = (pBuildSector->boundingBox.sup.z - pBuildSector->boundingBox.inf.z) / 2 + pBuildSector->boundingBox.inf.z;
		RtWorldImportSetPartitionStatistics(pBuildSector, pPartition);
	}

	return 0.0f;
}

RwReal RpNtlWorldSectorGetHeight(RpWorldSector *pRpWorldSector, RwV3d *pPos)
{
	NTL_FUNCTION("RpNtlWorldSectorGetHeight");

	RwV3d *pVList;
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);

	if(!pNtlSector)
		NTL_RETURN(-999.0f);

	pVList = RpMorphTargetGetVertices(pNtlSector->pNtlWorldSector->m_pAtomic->geometry->morphTarget);

	RwReal TileSize = (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;

	RwReal SPosX = pRpWorldSector->boundingBox.inf.x + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal SPosZ = pRpWorldSector->boundingBox.inf.z + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal CPosX = pPos->x + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal CPosZ = pPos->z + dGET_WORLD_PARAM()->WorldSizeHalf;

	RwInt32 XCnt = (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize) - 1);
	RwInt32 ZCnt = (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);

	if(XCnt > dGET_WORLD_PARAM()->WorldSectorTileNum || ZCnt > dGET_WORLD_PARAM()->WorldSectorTileNum || XCnt < 0 || ZCnt < 0)
		NTL_RETURN(-999.0f);

	D3DXVECTOR3 LT = D3DXVECTOR3(XCnt * TileSize,				0.0f, ZCnt * TileSize + TileSize);
	D3DXVECTOR3 RT = D3DXVECTOR3(XCnt * TileSize + TileSize,	0.0f, ZCnt * TileSize + TileSize);
	D3DXVECTOR3 LB = D3DXVECTOR3(XCnt * TileSize,				0.0f, ZCnt * TileSize);
	D3DXVECTOR3 RB = D3DXVECTOR3(XCnt * TileSize + TileSize,	0.0f, ZCnt * TileSize);

	RwInt32 Idx;
	Idx = (RwInt32)((LT.x / TileSize) + (LT.z / TileSize) * dGET_WORLD_PARAM()->WorldSectorVertNum); LT.y = pVList[Idx].y;
	Idx = (RwInt32)((RT.x / TileSize) + (RT.z / TileSize) * dGET_WORLD_PARAM()->WorldSectorVertNum); RT.y = pVList[Idx].y;
	Idx = (RwInt32)((LB.x / TileSize) + (LB.z / TileSize) * dGET_WORLD_PARAM()->WorldSectorVertNum); LB.y = pVList[Idx].y;
	Idx = (RwInt32)((RB.x / TileSize) + (RB.z / TileSize) * dGET_WORLD_PARAM()->WorldSectorVertNum); RB.y = pVList[Idx].y;
	LT.x = dGET_WORLD_PARAM()->WorldSectorSize - LT.x + SPosX; LT.z += SPosZ;
	RT.x = dGET_WORLD_PARAM()->WorldSectorSize - RT.x + SPosX; RT.z += SPosZ;
	LB.x = dGET_WORLD_PARAM()->WorldSectorSize - LB.x + SPosX; LB.z += SPosZ;
	RB.x = dGET_WORLD_PARAM()->WorldSectorSize - RB.x + SPosX; RB.z += SPosZ;

	D3DXPLANE p;
	float DX = CPosX - RB.x;
	float DZ = CPosZ - RB.z;
	if(DZ > TileSize - DX)
		D3DXPlaneFromPoints(&p, &LB, &LT, &RT);
	else
		D3DXPlaneFromPoints(&p, &LB, &RT, &RB);

	D3DXPlaneNormalize(&p, &p);

	NTL_RETURN(-(p.a * CPosX + p.c * CPosZ + p.d) / p.b);
}

RwReal RpNtlWorldSectorGetHeight(CNtlWorldSector* pNtlWorldSector, RwV3d* pPos, RwV3d* pVList)
{
	NTL_FUNCTION("RpNtlWorldSectorGetHeight");

	if(!pNtlWorldSector)
		NTL_RETURN(-999.0f);

	RwReal TileSize = (RwReal)dGET_WORLD_PARAM()->WorldSectorTileSize;

	RwReal SPosX = pNtlWorldSector->DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2) + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal SPosZ = pNtlWorldSector->DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2) + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal CPosX = pPos->x + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal CPosZ = pPos->z + dGET_WORLD_PARAM()->WorldSizeHalf;

	RwInt32 XCnt = (RwInt32)(dGET_WORLD_PARAM()->WorldSectorTileNum - (RwInt32)((CPosX - SPosX) / TileSize) - 1);
	RwInt32 ZCnt = (RwInt32)((CPosZ - SPosZ) / (RwReal)TileSize);

	if(XCnt > dGET_WORLD_PARAM()->WorldSectorTileNum || ZCnt > dGET_WORLD_PARAM()->WorldSectorTileNum || XCnt < 0 || ZCnt < 0)
		NTL_RETURN(-999.0f);

	D3DXVECTOR3 LT = D3DXVECTOR3(XCnt * TileSize,				0.0f, ZCnt * TileSize + TileSize);
	D3DXVECTOR3 RT = D3DXVECTOR3(XCnt * TileSize + TileSize,	0.0f, ZCnt * TileSize + TileSize);
	D3DXVECTOR3 LB = D3DXVECTOR3(XCnt * TileSize,				0.0f, ZCnt * TileSize);
	D3DXVECTOR3 RB = D3DXVECTOR3(XCnt * TileSize + TileSize,	0.0f, ZCnt * TileSize);

	RwInt32 Idx;
	Idx = (RwInt32)((LT.x / TileSize) + (LT.z / TileSize) * dGET_WORLD_PARAM()->WorldSectorVertNum); LT.y = pVList[Idx].y;
	Idx = (RwInt32)((RT.x / TileSize) + (RT.z / TileSize) * dGET_WORLD_PARAM()->WorldSectorVertNum); RT.y = pVList[Idx].y;
	Idx = (RwInt32)((LB.x / TileSize) + (LB.z / TileSize) * dGET_WORLD_PARAM()->WorldSectorVertNum); LB.y = pVList[Idx].y;
	Idx = (RwInt32)((RB.x / TileSize) + (RB.z / TileSize) * dGET_WORLD_PARAM()->WorldSectorVertNum); RB.y = pVList[Idx].y;
	LT.x = dGET_WORLD_PARAM()->WorldSectorSize - LT.x + SPosX; LT.z += SPosZ;
	RT.x = dGET_WORLD_PARAM()->WorldSectorSize - RT.x + SPosX; RT.z += SPosZ;
	LB.x = dGET_WORLD_PARAM()->WorldSectorSize - LB.x + SPosX; LB.z += SPosZ;
	RB.x = dGET_WORLD_PARAM()->WorldSectorSize - RB.x + SPosX; RB.z += SPosZ;

	D3DXPLANE p;
	float DX = CPosX - RB.x;
	float DZ = CPosZ - RB.z;
	if(DZ > TileSize - DX)
		D3DXPlaneFromPoints(&p, &LB, &LT, &RT);
	else
		D3DXPlaneFromPoints(&p, &LB, &RT, &RB);

	D3DXPlaneNormalize(&p, &p);

	NTL_RETURN(-(p.a * CPosX + p.c * CPosZ + p.d) / p.b);
}

/******************************************************************************************************************************
* RpGeoMipTileDestroy :
******************************************************************************************************************************/
RpWorld* RpNtlWorldCreate()
{
	NTL_FUNCTION( "RpNtlWorldCreate ");

	RpWorld *pWorld;
	RtWorldImport *pWorldImport;
	RtWorldImportParameters WorldImportParams;
	sNtlWorldPlugInParam NtlWorldPlugInParam;

	RwBBox NtlWorldBBox = {	{(RwReal)dGET_WORLD_PARAM()->WorldValueMax, 512.0f, (RwReal)dGET_WORLD_PARAM()->WorldValueMax},
							{(RwReal)dGET_WORLD_PARAM()->WorldValueMin, -512.0f, (RwReal)dGET_WORLD_PARAM()->WorldValueMin}};

	/* initialize rtworldimport toolkit */
	pWorldImport = RtWorldImportCreate();
	if(pWorldImport == NULL)
		NTL_RETURN(NULL);

	/* initialize rtworldimport parameter and we don't need any flags, as the bsp itself will have no geometry so flags = 0 */
	RtWorldImportParametersInit(&WorldImportParams);
	WorldImportParams.flags = 0;
	WorldImportParams.conditionGeometry = FALSE;
	WorldImportParams.calcNormals = FALSE;
	WorldImportParams.userSpecifiedBBox = TRUE;
	WorldImportParams.userBBox = NtlWorldBBox;

	/* set the bsp tree building callbacks (these will generate a quadtree-like structure of sectors which the geoMip tiles will fit exactly into). */
	RtWorldImportSetBuildCallBacks(NtlWorldImportPartitionBuildCB, NtlWorldImportTerminationBuildCB);

	/* create the bsp world from the worldImport. */
	pWorld = RtWorldImportCreateWorld(pWorldImport, &WorldImportParams);

	/* now populate the bsp with the GeoMipTiles */
	NtlWorldPlugInParam.pWorld = pWorld;

	/* for optimisation reasons, all tiles share one frame */
	dNTL_WORLD_SECTOR_LOCAL(pWorld, pParentFrame) = RwFrameCreate();
	NtlWorldPlugInParam.pParentFrame = dNTL_WORLD_SECTOR_LOCAL(pWorld, pParentFrame);

	/* set the world sector render callback to render tiled geometry */
	//RpWorldSetSectorRenderCallBack(pWorld, dNTL_WORLD_GLOBAL(WorldSectorCBRender));

	RtWorldImportDestroy(pWorldImport);

	NTL_RETURN(pWorld);
}

RwUInt8 RpNtlWorldUInt8PropLerp(RwUInt8 Src, RwUInt8 Dst, RwReal SrcTime, RwReal DstTime)
{
	if(Src < 0 || Dst < 0)
		return FALSE;

	RwReal TimeRate		= (DstTime - SrcTime) / DstTime;
	RwReal DistRate		= static_cast<RwReal>(::fabs(static_cast<RwReal>(Src) - static_cast<RwReal>(Dst)));
	RwUInt8 Variation	= static_cast<RwUInt8>(DistRate * TimeRate);

	if(Src < Dst && Variation)
	{
		Src = Src + Variation;
	}
	else if(Src > Dst && Variation)
	{
		Src = Src - Variation;
	}

	return Src;
}

RwReal RpNtlWorldRealPropLerp(RwReal Src, RwReal Dst, RwReal SrcTime, RwReal DstTime)
{
	if(Src < 0 || Dst < 0)
		return FALSE;

	RwReal TimeRate		= (DstTime - SrcTime) / DstTime;
	RwReal DistRate		= static_cast<RwReal>(::fabs((Src - Dst)));
	RwReal Variation	= DistRate * TimeRate;

	if(Src < Dst && Variation)
	{
		Src = Src + Variation;
	}
	else if(Src > Dst && Variation)
	{
		Src = Src - Variation;
	}

	return Src;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlWorldObjVisionAnalyzer* CNtlWorldObjVisionAnalyzer::GetInstance(void)
{
	static CNtlWorldObjVisionAnalyzer NtlWorldObjVisionAnalyzer;
	return &NtlWorldObjVisionAnalyzer;
}

CNtlWorldObjVisionAnalyzer::CNtlWorldObjVisionAnalyzer(void)
{
	m_ObjVision = 0x00000000;

	m_ObjVision |= (eNWOVA_MINI_INDOOR_CULL | eNWOVA_DOODADS | eNWOVA_WATER | eNWOVA_EFFECTS | eNWOVA_VISIBILTITY_DIST | eNWOVA_BLOOM_EFFECT | eNWOVA_PERFORMANCE_SECTORCULL | eNWOVA_PROJECTION_SHADOW | eNWOVA_FOG_ACTIVATION | eNWOVA_PLNAT | eNWOVA_OCCLUDER_ACTIVATION | eNWOVA_WEATHER | eNWOVA_HEATHAZE | eNWOVA_LIGHTMAP | eNWOVA_SOUND | eNWOVA_OBJECT_ANIMATION);
}

CNtlWorldObjVisionAnalyzer::~CNtlWorldObjVisionAnalyzer(void)
{
}

BOOL CNtlWorldObjVisionAnalyzer::GetCheck(eNTL_WORLD_OBJ_VISION_ANLYZER eNWOVA_ID)
{
	if(m_ObjVision & eNWOVA_ID)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID CNtlWorldObjVisionAnalyzer::SetCheck(eNTL_WORLD_OBJ_VISION_ANLYZER eNWOVA_ID, BOOL SetFlag)
{
	if(SetFlag)
	{
		m_ObjVision |= eNWOVA_ID;
	}
	else
	{
		m_ObjVision &= ~eNWOVA_ID;
	}
}

VOID CNtlWorldObjVisionAnalyzer::Toggle(eNTL_WORLD_OBJ_VISION_ANLYZER eNWOVA_ID)
{
	if(m_ObjVision & eNWOVA_ID)
	{
		m_ObjVision &= ~eNWOVA_ID;
	}
	else
	{
		m_ObjVision |= eNWOVA_ID;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////