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
#include "ntlworldinterface.h"


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
/*
RpWorld* RpNtlWorldCreateEmpty()
{
	RpWorld *pWorld;
	RwBBox NtlWorldBBox = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};

	pWorld = RpWorldCreate(&NtlWorldBBox);

	return pWorld;
}
*/

/******************************************************************************************************************************
* RpGeoMipTileDestroy :
******************************************************************************************************************************/
RpWorld* RpNtlWorldCreate()
{
	RpWorld *pWorld;
	RtWorldImport *pWorldImport;
	RtWorldImportParameters WorldImportParams;
	sNtlWorldPlugInParam NtlWorldPlugInParam;

	RwBBox NtlWorldBBox = {	{(RwReal)dGET_WORLD_PARAM()->WorldValueMax, 0.0f, (RwReal)dGET_WORLD_PARAM()->WorldValueMax},
							{(RwReal)dGET_WORLD_PARAM()->WorldValueMin, 0.0f, (RwReal)dGET_WORLD_PARAM()->WorldValueMin}};

	/* initialize rtworldimport toolkit */
	pWorldImport = RtWorldImportCreate();
	if(pWorldImport == NULL)
		return NULL;

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
	RpWorldSetSectorRenderCallBack(pWorld, dNTL_WORLD_GLOBAL(WorldSectorCBRender));

	RtWorldImportDestroy(pWorldImport);

	return pWorld;

}