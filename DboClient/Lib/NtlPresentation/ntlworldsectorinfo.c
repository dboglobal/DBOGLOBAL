/******************************************************************************************************************************
 *
 * ntlworldsectorinfo.h
 *
 * copyright (c) 2005 ntl-inc Ltd.
 *
 * author : woody1019
 *
 * purpose : 
 *
 ******************************************************************************************************************************/


#include "ntlworldsectorinfo.h"
#include "ntlworldinterface.h"
#include "NtlWorldMaterialPlugin.h"


RwBool _RpNtlWorldSectorGeometryLoadFromFile(RpWorldSector *pWorldSector, RwChar *pSectorFileName, FILE* pFile)
{	
	/*
	RwStream *pStream = NULL;
	sNtlWorldSector *pNtlWorldSector = RwFreeListAlloc(dNTL_WORLD_GLOBAL(pSectorFreelist), rwMEMHINTDUR_EVENT);
	dNTL_WORLD_LOCAL(pWorldSector, pNtlSector) = pNtlWorldSector;
	pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, pSectorFileName);

	if(pStream)
	{
		if(RwStreamFindChunk(pStream, rwID_ATOMIC, NULL, NULL))
		{
			pNtlWorldSector->pAtomic = RpAtomicStreamRead(pStream);
			RwStreamClose(pStream, NULL);
			RpAtomicSetFrame(pNtlWorldSector->pAtomic, dNTL_WORLD_SECTOR_LOCAL(RpWorldSectorGetWorld(pWorldSector), pParentFrame));
		}
	}*/

	RpGeometry *pGeometry;
	RpMaterial *pMaterial;
	RwSurfaceProperties SurfProp;
	RpMorphTarget *pMorphTarget;
	RwSphere BoundingSphere;
	RwTexCoords *pTexCoord;
	RwV3d *pVList;
	RpTriangle *pTList;
	RwV2d SPos, EPos;
	RwInt32 i, j;
	RwInt16 CurPtIndices[4];
	RwRGBA *pPrelights;
	RwInt32 m, n;
	const RwRGBA White = {0xff, 0xff, 0xff, 0xff};
	sRpNtlWorldSectorSplat NtlWorlSectorSplat;
	sNtlWorldSector *pNtlSector;

	// link to the pointer
	dNTL_WORLD_LOCAL(pWorldSector, pNtlSector) = RwFreeListAlloc(dNTL_WORLD_GLOBAL(pSectorFreelist), rwMEMHINTDUR_EVENT);
	pNtlSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);

	// initialize ntl sector
	pNtlSector->pAtomic = NULL;
	memset(pNtlSector->pTexture, 0, sizeof(RwTexture*) * dNTL_SPLAT_LAYER_NUM);
	memset(&NtlWorlSectorSplat, 0, sizeof(sRpNtlWorldSectorSplat));

	// create geometry
	pGeometry = RpGeometryCreate(dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum,
								 dGET_WORLD_PARAM()->WorldSectorPolyNum,					 
								 rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);

	pPrelights = RpGeometryGetPreLightColors(pGeometry);

	/* There's only one morph target, with index 0... */
	pMorphTarget = RpGeometryGetMorphTarget(pGeometry, 0);

	/* must be added 2 make a rw mesh */
	pMaterial = RpMaterialCreate();
	SurfProp.ambient  = 1.0f;
	SurfProp.diffuse  = 1.0f;
	SurfProp.specular = 1.0f;
	RpMaterialSetSurfaceProperties(pMaterial, &SurfProp);

	pNtlSector->pTexture[0] = RwD3D9DDSTextureRead("c:\\woody", NULL);
	NtlWorlSectorSplat.pTextureBase = pNtlSector->pTexture[0];
	RpNtlWorldSetSectorSplat(pMaterial, &NtlWorlSectorSplat);
	//RpMaterialSetTexture(pMaterial, pNtlSector->pTexture[0]);

	/* create vertices & indices */
	pVList = RpMorphTargetGetVertices(pMorphTarget);
	pTList = RpGeometryGetTriangles(pGeometry);
	pTexCoord = RpGeometryGetVertexTexCoords(pGeometry, rwTEXTURECOORDINATEINDEX0);

	if(pFile)
	{
		fread(pVList, sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);
	}

	SPos.x = pWorldSector->boundingBox.inf.x;
	SPos.y = pWorldSector->boundingBox.inf.z;
	EPos.x = pWorldSector->boundingBox.sup.x;
	EPos.y = pWorldSector->boundingBox.sup.z;

	for(i = (RwInt32)SPos.y; i <= (RwInt32)EPos.y; i += dGET_WORLD_PARAM()->WorldSectorTileSize)
	{
		for(j = (RwInt32)SPos.x; j <= (RwInt32)EPos.x; j += dGET_WORLD_PARAM()->WorldSectorTileSize)
		{
			*pPrelights = White;

			m = (RwInt32)((i - SPos.y) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			n = (RwInt32)((j - SPos.x) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			pTexCoord->u = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].u;
			pTexCoord->v = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].v;

			pPrelights++;
			pTexCoord++;
		}
	}

	/*
	<3>--<1>
	|  / |
	| /  |
	<2>--<0>
	*/
	for(i = 0; i < dGET_WORLD_PARAM()->WorldSectorTileNum; ++i)
	{
		for(j = 0; j < dGET_WORLD_PARAM()->WorldSectorTileNum; ++j)
		{
			CurPtIndices[0] = j + (i * dGET_WORLD_PARAM()->WorldSectorVertNum);
			CurPtIndices[2] = CurPtIndices[0] + 1;

			CurPtIndices[1] = CurPtIndices[0] + dGET_WORLD_PARAM()->WorldSectorVertNum;
			CurPtIndices[3] = CurPtIndices[1] + 1;

			RpGeometryTriangleSetVertexIndices(pGeometry, pTList, CurPtIndices[2], CurPtIndices[1], CurPtIndices[3]);
			RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);

			RpGeometryTriangleSetVertexIndices(pGeometry, pTList, CurPtIndices[1], CurPtIndices[2], CurPtIndices[0]);
			RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);
		}
	}

	/* Calculate a bounding sphere for the atomic */
	RpMorphTargetCalcBoundingSphere(pMorphTarget, &BoundingSphere);
	RpMorphTargetSetBoundingSphere(pMorphTarget, &BoundingSphere);

	RpGeometryUnlock(pGeometry);

	/* Create an atomic from this geometry */
	pNtlSector->pAtomic = RpAtomicCreate();

	RpAtomicSetPipeline(pNtlSector->pAtomic, D3D9NtlWorldSectorAtomicPipeline);

	RpAtomicSetFrame(pNtlSector->pAtomic, dNTL_WORLD_SECTOR_LOCAL(RpWorldSectorGetWorld(pWorldSector), pParentFrame));
	RpAtomicSetGeometry(pNtlSector->pAtomic, pGeometry, 0);

	/* Remove all local references to ref-counted objects */
	RpGeometryDestroy(pGeometry);
	RpMaterialDestroy(pMaterial);

	return TRUE;
}

RwBool _RpNtlWorldSectorGeometrySaveIntoFile(RpWorldSector *pWorldSector, RwChar *pSectorFileName, FILE* pFile)
{
	/*
	sNtlWorldSector *pNtlWorldSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);
	RwStream *pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMWRITE, pSectorFileName);
	RpAtomicStreamWrite(pNtlWorldSector->pAtomic, pStream);
	RwStreamClose(pStream, NULL);
	*/

	sNtlWorldSector *pNtlWorldSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);

	RwV3d *pVList;
	pVList = RpMorphTargetGetVertices(pNtlWorldSector->pAtomic->geometry->morphTarget);

	if(pFile)
	{
		fwrite(pVList ,sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);
	}

	return TRUE;
}

RwBool _RpNtlWorldSectorGeometryDelete(RpWorldSector *pWorldSector)
{
	RwInt32 level = 0;
	RwInt32 i;
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);
	
	for(i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		if(pNtlSector->pTexture[i])
			RwTextureDestroy(pNtlSector->pTexture[i]);

	RpAtomicSetFrame(pNtlSector->pAtomic, NULL);
	RpAtomicDestroy(pNtlSector->pAtomic);
	RwFreeListFree(dNTL_WORLD_GLOBAL(pSectorFreelist), pNtlSector);

	dNTL_WORLD_LOCAL(pWorldSector, pNtlSector) = NULL;

	return TRUE;
}

sNtlWorldSector* _RpNtlWorldSectorGeometryCreate(RpWorldSector *pWorldSector)
{
	sNtlWorldSector *pNtlSector = RwFreeListAlloc(dNTL_WORLD_GLOBAL(pSectorFreelist), rwMEMHINTDUR_EVENT);

	RpGeometry *pGeometry;
	RpMaterial *pMaterial;
	RwSurfaceProperties SurfProp;
	RpMorphTarget *pMorphTarget;
	RwSphere BoundingSphere;
	RwTexCoords *pTexCoord;
	RwV3d *pVList;
	RpTriangle *pTList;
	RwV2d SPos, EPos;
	RwInt32 i, j;
	RwInt16 CurPtIndices[4];
	RwRGBA *pPrelights;
	RwInt32 m, n;
	const RwRGBA White = {0xff, 0xff, 0xff, 0xff};
	sRpNtlWorldSectorSplat NtlWorlSectorSplat;

	// initialize ntl sector
	pNtlSector->pAtomic = NULL;
	memset(pNtlSector->pTexture, 0, sizeof(RwTexture*) * dNTL_SPLAT_LAYER_NUM);
	memset(&NtlWorlSectorSplat, 0, sizeof(sRpNtlWorldSectorSplat));
	
	// create geometry
	pGeometry = RpGeometryCreate(dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum,
								 dGET_WORLD_PARAM()->WorldSectorPolyNum,					 
								 rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);

	pPrelights = RpGeometryGetPreLightColors(pGeometry);

	/* There's only one morph target, with index 0... */
	pMorphTarget = RpGeometryGetMorphTarget(pGeometry, 0);

	/* must be added 2 make a rw mesh */
	pMaterial = RpMaterialCreate();
	SurfProp.ambient  = 1.0f;
	SurfProp.diffuse  = 1.0f;
	SurfProp.specular = 1.0f;
	RpMaterialSetSurfaceProperties(pMaterial, &SurfProp);

	pNtlSector->pTexture[0] = RwD3D9DDSTextureRead("c:\\woody", NULL);
	NtlWorlSectorSplat.pTextureBase = pNtlSector->pTexture[0];
	RpNtlWorldSetSectorSplat(pMaterial, &NtlWorlSectorSplat);
	//RpMaterialSetTexture(pMaterial, pNtlSector->pTexture[0]);

	/* create vertices & indices */
	pVList = RpMorphTargetGetVertices(pMorphTarget);
	pTList = RpGeometryGetTriangles(pGeometry);
	pTexCoord = RpGeometryGetVertexTexCoords(pGeometry, rwTEXTURECOORDINATEINDEX0);

	SPos.x = pWorldSector->boundingBox.inf.x;
	SPos.y = pWorldSector->boundingBox.inf.z;
	EPos.x = pWorldSector->boundingBox.sup.x;
	EPos.y = pWorldSector->boundingBox.sup.z;

	for(i = (RwInt32)SPos.y; i <= (RwInt32)EPos.y; i += dGET_WORLD_PARAM()->WorldSectorTileSize)
	{
		for(j = (RwInt32)SPos.x; j <= (RwInt32)EPos.x; j += dGET_WORLD_PARAM()->WorldSectorTileSize)
		{
			*pPrelights = White;

			pVList->x = (RwReal)j;
			pVList->y = 0.0f;
			pVList->z = (RwReal)i;
			
			m = (RwInt32)((i - SPos.y) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			n = (RwInt32)((j - SPos.x) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			pTexCoord->u = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].u;
			pTexCoord->v = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].v;

			pPrelights++;
			pVList++;
			pTexCoord++;
		}
	}

	/*
	<3>--<1>
	 |  / |
	 | /  |
	<2>--<0>
	*/
	for(i = 0; i < dGET_WORLD_PARAM()->WorldSectorTileNum; ++i)
	{
		for(j = 0; j < dGET_WORLD_PARAM()->WorldSectorTileNum; ++j)
		{
			CurPtIndices[0] = j + (i * dGET_WORLD_PARAM()->WorldSectorVertNum);
			CurPtIndices[2] = CurPtIndices[0] + 1;

			CurPtIndices[1] = CurPtIndices[0] + dGET_WORLD_PARAM()->WorldSectorVertNum;
			CurPtIndices[3] = CurPtIndices[1] + 1;

			RpGeometryTriangleSetVertexIndices(pGeometry, pTList, CurPtIndices[2], CurPtIndices[1], CurPtIndices[3]);
			RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);

			RpGeometryTriangleSetVertexIndices(pGeometry, pTList, CurPtIndices[1], CurPtIndices[2], CurPtIndices[0]);
			RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);
		}
	}

	/* Calculate a bounding sphere for the atomic */
	RpMorphTargetCalcBoundingSphere(pMorphTarget, &BoundingSphere);
	RpMorphTargetSetBoundingSphere(pMorphTarget, &BoundingSphere);

	RpGeometryUnlock(pGeometry);

	/* Create an atomic from this geometry */
	pNtlSector->pAtomic = RpAtomicCreate();

	RpAtomicSetPipeline(pNtlSector->pAtomic, D3D9NtlWorldSectorAtomicPipeline);

	RpAtomicSetFrame(pNtlSector->pAtomic, dNTL_WORLD_SECTOR_LOCAL(RpWorldSectorGetWorld(pWorldSector), pParentFrame));
	RpAtomicSetGeometry(pNtlSector->pAtomic, pGeometry, 0);

	/* Remove all local references to ref-counted objects */
	RpGeometryDestroy(pGeometry);
	RpMaterialDestroy(pMaterial);

	return pNtlSector;
}

RwBool _RpNtlWorldSectorRender(RpWorldSector *pRpWorldSector)
{
	return TRUE;
}