/******************************************************************************************************************************
 *
 * ntlworldcommon_c
 *
 * copyright (c) 2005 ntl-inc Ltd. 2018 daneos
 *
 * author : woody1019, daneos
 *
 * purpose : ntl world structure collection
 *
 ******************************************************************************************************************************/


/******************************************************************************************************************************
* headers
******************************************************************************************************************************/
#include "precomp_ntlpresentation.h"
#include "ntlworldcommon.h"


/******************************************************************************************************************************
* global variables
******************************************************************************************************************************/
RwInt32 NtlWorldGlobalOffset = -1;
RwInt32 NtlWorldLocalOffset = -1;
RwInt32 NtlWorldSectorLocalOffset = -1;
RwTexCoords *NtlCommonTexCoord = NULL;
RxPipeline *D3D9NtlWorldSectorAtomicPipeline = NULL;
RxPipeline *D3D9NtlWorldWaterSectorAtomicPipeline = NULL;
RxPipelineNode*	g_pNodeWorld = NULL;
RxPipelineNode*	g_pNodeWorldWater = NULL;
sNtlWorldParam g_sNtlWorldParam;
gbDxGenLODIdx *g_pLODIdx = NULL;
sNtlWorldCollisionInfo g_NtlWorldCollisionInfo;
CNtlPLEntity* g_pBrushMapObjEntity = NULL;
CNtlWorldMouse g_Mouse;
RwBool g_bCollsionInfoUpdate = FALSE;

VOID SaveCurWorldState()
{
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);

	// create a global world param
	FILE*	pFile = NULL;
	RwInt32 iFileResult = fopen_s(&pFile, "#######.gwp", "wb");
	if (iFileResult != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		fwrite(dGET_WORLD_PARAM()->WorldVer,				10,					1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldHeightfieldNum,	sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldSectorTileSize,	sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldSectorTileNum,		sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldSectorVertNum,		sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldSectorSize,		sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldSectorNum,			sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldSectorHalfNum,		sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldSectorPolyNum,		sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldFieldSectorNum,	sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldFieldSize,			sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldFieldNum,			sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldFieldHalfNum,		sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldSize,				sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldSizeHalf,			sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldValueMax,			sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldValueMin,			sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldTexAddr,			sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldPolyCnt,			sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldNearPlane,			sizeof(RwReal),		1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldFarPlane,			sizeof(RwReal),		1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldCamSpeedPerSec,	sizeof(RwReal),		1, pFile);
		fwrite(dGET_WORLD_PARAM()->WorldRAW,				64,					1, pFile);

		fwrite(&dGET_WORLD_PARAM()->WorldTriggerSerialID,	sizeof(RwUInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->IdxSkyType,				sizeof(RwInt32),	1, pFile);

		fwrite(&dGET_WORLD_PARAM()->WorldType,				sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldBlockSize,			sizeof(RwInt32),	1, pFile);
		fwrite(&dGET_WORLD_PARAM()->WorldBlockNum,			sizeof(RwInt32),	1, pFile);
		fwrite(dGET_WORLD_PARAM()->WorldBSP,				64,					1, pFile);

		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
		{
			if (dGET_WORLD_PARAM()->WorldType == 1) // example dbo_battle_001
			{
				fwrite(&dGET_WORLD_PARAM()->WorldObjectSerialID, sizeof(RwUInt32), 1, pFile);
			}
		}
		else
		{
			fwrite(&dGET_WORLD_PARAM()->WorldObjectSerialID, sizeof(RwUInt32), 1, pFile);
			fwrite(&dGET_WORLD_PARAM()->WorldResourceID, sizeof(RwUInt32), 1, pFile);
		}

		fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
}

VOID SaveFileWorldState(sNtlWorldParam* pNtlWorldParam, FILE* pFile)
{
	fwrite(pNtlWorldParam->WorldVer,				10,					1, pFile);
	fwrite(&pNtlWorldParam->WorldHeightfieldNum,	sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldSectorTileSize,	sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldSectorTileNum,		sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldSectorVertNum,		sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldSectorSize,		sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldSectorNum,			sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldSectorHalfNum,		sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldSectorPolyNum,		sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldFieldSectorNum,	sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldFieldSize,			sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldFieldNum,			sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldFieldHalfNum,		sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldSize,				sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldSizeHalf,			sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldValueMax,			sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldValueMin,			sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldTexAddr,			sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldPolyCnt,			sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldNearPlane,			sizeof(RwReal),		1, pFile);
	fwrite(&pNtlWorldParam->WorldFarPlane,			sizeof(RwReal),		1, pFile);
	fwrite(&pNtlWorldParam->WorldCamSpeedPerSec,	sizeof(RwReal),		1, pFile);
	fwrite(pNtlWorldParam->WorldRAW,				64,					1, pFile);

	fwrite(&pNtlWorldParam->WorldTriggerSerialID,	sizeof(RwUInt32),	1, pFile);
	fwrite(&pNtlWorldParam->IdxSkyType,				sizeof(RwInt32),	1, pFile);

	fwrite(&pNtlWorldParam->WorldType,				sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldBlockSize,			sizeof(RwInt32),	1, pFile);
	fwrite(&pNtlWorldParam->WorldBlockNum,			sizeof(RwInt32),	1, pFile);
	fwrite(pNtlWorldParam->WorldBSP,				64,					1, pFile);

	if (dNTL_WORLD_VERSION_COMPARE(pNtlWorldParam->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		if (pNtlWorldParam->WorldType == 1) // example dbo_battle_001
		{
			fwrite(&pNtlWorldParam->WorldObjectSerialID, sizeof(RwUInt32), 1, pFile);
		}
	}
	else
	{
		fwrite(&pNtlWorldParam->WorldObjectSerialID, sizeof(RwUInt32), 1, pFile);
		fwrite(&pNtlWorldParam->WorldResourceID, sizeof(RwUInt32), 1, pFile);
	}
}

VOID LoadFileWorldState(sNtlWorldParam* pNtlWorldParam, FILE* pFile)
{	
	fread(pNtlWorldParam->WorldVer,					10,					1, pFile); 
//	DBO_WARNING_MESSAGE("pNtlWorldParam->WorldVer: " << pNtlWorldParam->WorldVer);
	strcpy_s(pNtlWorldParam->WorldSaveVer, 10, pNtlWorldParam->WorldVer);
	strcpy_s(pNtlWorldParam->WorldLoadVer, 10, pNtlWorldParam->WorldVer);

	fread(&pNtlWorldParam->WorldHeightfieldNum,		sizeof(RwInt32),	1, pFile); 
	fread(&pNtlWorldParam->WorldSectorTileSize,		sizeof(RwInt32),	1, pFile); 
	fread(&pNtlWorldParam->WorldSectorTileNum,		sizeof(RwInt32),	1, pFile); 
	fread(&pNtlWorldParam->WorldSectorVertNum,		sizeof(RwInt32),	1, pFile); 
	fread(&pNtlWorldParam->WorldSectorSize,			sizeof(RwInt32),	1, pFile); 
	fread(&pNtlWorldParam->WorldSectorNum,			sizeof(RwInt32),	1, pFile); 
	fread(&pNtlWorldParam->WorldSectorHalfNum,		sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldSectorPolyNum,		sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldFieldSectorNum,		sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldFieldSize,			sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldFieldNum,			sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldFieldHalfNum,		sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldSize,				sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldSizeHalf,			sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldValueMax,			sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldValueMin,			sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldTexAddr,			sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldPolyCnt,			sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldNearPlane,			sizeof(RwReal),		1, pFile);
	fread(&pNtlWorldParam->WorldFarPlane,			sizeof(RwReal),		1, pFile);
	fread(&pNtlWorldParam->WorldCamSpeedPerSec,		sizeof(RwReal),		1, pFile);
	fread(pNtlWorldParam->WorldRAW,					64,					1, pFile);

	fread(&pNtlWorldParam->WorldTriggerSerialID,	sizeof(RwUInt32),	1, pFile);
	fread(&pNtlWorldParam->IdxSkyType,				sizeof(RwInt32),	1, pFile);

	fread(&pNtlWorldParam->WorldType,				sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldBlockSize,			sizeof(RwInt32),	1, pFile);
	fread(&pNtlWorldParam->WorldBlockNum,			sizeof(RwInt32),	1, pFile);
	fread(pNtlWorldParam->WorldBSP,					64,					1, pFile);

	if(dNTL_WORLD_VERSION_COMPARE(pNtlWorldParam->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		if (pNtlWorldParam->WorldType == 1) // example dbo_battle_001
		{
			fread(&pNtlWorldParam->WorldObjectSerialID, sizeof(RwUInt32), 1, pFile);
		}
	}
	else
	{
		fread(&dGET_WORLD_PARAM()->WorldObjectSerialID, sizeof(RwUInt32), 1, pFile);
		fread(&pNtlWorldParam->WorldResourceID, sizeof(RwUInt32), 1, pFile);
	}

	pNtlWorldParam->LODEnable				= FALSE;
	pNtlWorldParam->NumBrushRadiusInTile	= 3.0f;
	pNtlWorldParam->BlockGuideLine			= FALSE;
	pNtlWorldParam->FieldGuideLine			= FALSE;
	pNtlWorldParam->SectorGuideLine			= FALSE;
	pNtlWorldParam->FlgHeightValueShow		= FALSE;
	pNtlWorldParam->ClrDayAndNight			= 0x00000000;
	pNtlWorldParam->ClrDiffuserPicker		= 0x00000000;
	pNtlWorldParam->ClrBaseAmbient[0]		= 100;
	pNtlWorldParam->ClrBaseAmbient[1]		= 100;
	pNtlWorldParam->ClrBaseAmbient[2]		= 100;
	pNtlWorldParam->ClrShadow				= 0xff777777;
}