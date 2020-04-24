#include "precomp_ntlpresentation.h"

#include "NtlPLPlanet.h"

#include "ntlworldfieldinfo.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

CNtlWorldFieldInfo*	CNtlWorldFieldInfo::s_pInstance	= NULL;

CNtlWorldFieldInfo* CNtlWorldFieldInfo::CreateInstance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = NTL_NEW CNtlWorldFieldInfo;
	}
	return s_pInstance;
}		

void CNtlWorldFieldInfo::DestroyInstance()
{
	NTL_DELETE(s_pInstance);
}

CNtlWorldFieldInfo* CNtlWorldFieldInfo::GetInstance()
{
	return s_pInstance;
}

CNtlWorldFieldInfo::CNtlWorldFieldInfo()
{
}

CNtlWorldFieldInfo::~CNtlWorldFieldInfo()
{
}

// Name
RwBool CNtlWorldFieldInfo::FieldNameLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fread(pNtlFieldProp->_Name, sizeof(RwChar) * 64, 1, pFile);

	return TRUE;
}

RwBool CNtlWorldFieldInfo::FieldNameSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fwrite(pNtlFieldProp->_Name, sizeof(RwChar) * 64, 1, pFile);

	return TRUE;
}

RwInt32 CNtlWorldFieldInfo::FieldNameSkipToFile(FILE* pFile)
{
	fseek(pFile, sizeof(RwChar) * 64, SEEK_CUR);

	return ftell(pFile);
}

BYTE* CNtlWorldFieldInfo::FieldNameSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	fwrite(pFileMem, sizeof(RwChar) * 64, 1, pFile);
	pFileMem += sizeof(RwChar) * 64;

	return pFileMem;
}

BYTE* CNtlWorldFieldInfo::FieldNameSkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwChar) * 64;

	return pFileMem;
}


// BitProp
RwBool CNtlWorldFieldInfo::FieldBitPropLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp, RwInt32 iTileNumInField)
{
	/*(dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2)*/
	fread(pNtlFieldProp->_pBitProp, sizeof(DWORD) * iTileNumInField, 1, pFile);

	return TRUE;
}

RwBool CNtlWorldFieldInfo::FieldBitPropSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp, RwInt32 iTileNumInField)
{
	fwrite(pNtlFieldProp->_pBitProp, sizeof(DWORD) * iTileNumInField, 1, pFile);

	return TRUE;
}

RwInt32 CNtlWorldFieldInfo::FieldBitPropSkipToFile(FILE* pFile, RwInt32 iTileNumInField)
{
	fseek(pFile, sizeof(DWORD) * iTileNumInField, SEEK_CUR);

	return ftell(pFile);
}

BYTE* CNtlWorldFieldInfo::FieldBitPropSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iTileNumInField)
{
	fwrite(pFileMem,  sizeof(DWORD) * iTileNumInField, 1, pFile);
	pFileMem += sizeof(DWORD) * iTileNumInField;

	return pFileMem;
}

BYTE* CNtlWorldFieldInfo::FieldBitPropSkipToFileMem(BYTE* pFileMem, RwInt32 iTileNumInField)
{
	pFileMem += sizeof(DWORD) * iTileNumInField;

	return pFileMem;
}

// Fog
RwBool CNtlWorldFieldInfo::FieldFogLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fread(&pNtlFieldProp->_FogColor, sizeof(RwRGBA), 1, pFile);
	fread(&pNtlFieldProp->_FogCamPlane[0], sizeof(RwReal), 1, pFile);
	fread(&pNtlFieldProp->_FogCamPlane[1], sizeof(RwReal), 1, pFile);

	return TRUE;
}

RwBool CNtlWorldFieldInfo::FieldFogSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fwrite(&pNtlFieldProp->_FogColor, sizeof(RwRGBA), 1, pFile);
	fwrite(&pNtlFieldProp->_FogCamPlane[0], sizeof(RwReal), 1, pFile);
	fwrite(&pNtlFieldProp->_FogCamPlane[1], sizeof(RwReal), 1, pFile);

	return TRUE;
}

RwInt32 CNtlWorldFieldInfo::FieldFogSkipToFile(FILE* pFile)
{
	fseek(pFile, sizeof(RwRGBA), SEEK_CUR);
	fseek(pFile, sizeof(RwReal), SEEK_CUR);
	fseek(pFile, sizeof(RwReal), SEEK_CUR);

	return ftell(pFile);
}

BYTE* CNtlWorldFieldInfo::FieldFogSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	RwInt32 SizMemBlock =	sizeof(RwRGBA) +		// fog color
		(sizeof(RwReal) * 2);	// fog cam plane; near, far

	fwrite(pFileMem, SizMemBlock, 1, pFile);
	pFileMem += SizMemBlock;

	return pFileMem;
}

BYTE* CNtlWorldFieldInfo::FieldFogSkipToFileMem(BYTE* pFileMem)
{
	RwInt32 SizMemBlock =	sizeof(RwRGBA) +		// fog color
		(sizeof(RwReal) * 2);	// fog cam plane; near, far

	pFileMem += SizMemBlock;

	return pFileMem;
}

// Sky
RwBool CNtlWorldFieldInfo::FieldSkyLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fread(&pNtlFieldProp->_BaseSkyMode, sizeof(RwInt32), 1, pFile);
	fread(&pNtlFieldProp->_RGBSkyColor[0], sizeof(RwRGBA), 1, pFile);
	fread(&pNtlFieldProp->_RGBSkyColor[1], sizeof(RwRGBA), 1, pFile);
	fread(&pNtlFieldProp->_RGBSkyColor[2], sizeof(RwRGBA), 1, pFile);

	fread(&pNtlFieldProp->_NewSkyValue, sizeof(RwReal), 1, pFile);

	fread(pNtlFieldProp->_BaseSkyTexName, sizeof(RwChar) * 10, 1, pFile);
	fread(&pNtlFieldProp->_BaseSkySpeed, sizeof(RwReal), 1, pFile);
	fread(pNtlFieldProp->_BlendedTexName[0], sizeof(RwChar) * 10, 1, pFile);
	fread(pNtlFieldProp->_BlendedTexName[1], sizeof(RwChar) * 10, 1, pFile);
	fread(&pNtlFieldProp->_BlendedTexSpeed[0], sizeof(RwReal), 1, pFile);
	fread(&pNtlFieldProp->_BlendedTexSpeed[1], sizeof(RwReal), 1, pFile);

	return TRUE;
}

RwBool CNtlWorldFieldInfo::FieldSkySaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fwrite(&pNtlFieldProp->_BaseSkyMode, sizeof(RwInt32), 1, pFile);
	fwrite(&pNtlFieldProp->_RGBSkyColor[0], sizeof(RwRGBA), 1, pFile);
	fwrite(&pNtlFieldProp->_RGBSkyColor[1], sizeof(RwRGBA), 1, pFile);
	fwrite(&pNtlFieldProp->_RGBSkyColor[2], sizeof(RwRGBA), 1, pFile);

	fwrite(&pNtlFieldProp->_NewSkyValue, sizeof(RwReal), 1, pFile);

	fwrite(pNtlFieldProp->_BaseSkyTexName, sizeof(RwChar) * 10, 1, pFile);
	fwrite(&pNtlFieldProp->_BaseSkySpeed, sizeof(RwReal), 1, pFile);
	fwrite(pNtlFieldProp->_BlendedTexName[0], sizeof(RwChar) * 10, 1, pFile);
	fwrite(pNtlFieldProp->_BlendedTexName[1], sizeof(RwChar) * 10, 1, pFile);
	fwrite(&pNtlFieldProp->_BlendedTexSpeed[0], sizeof(RwReal), 1, pFile);
	fwrite(&pNtlFieldProp->_BlendedTexSpeed[1], sizeof(RwReal), 1, pFile);

	return TRUE;
}

RwInt32 CNtlWorldFieldInfo::FieldSkySkipToFile(FILE* pFile)
{
	fseek(pFile, sizeof(RwInt32), SEEK_CUR);
	fseek(pFile, sizeof(RwRGBA), SEEK_CUR);
	fseek(pFile, sizeof(RwRGBA), SEEK_CUR);
	fseek(pFile, sizeof(RwRGBA), SEEK_CUR);

	fseek(pFile, sizeof(RwReal), SEEK_CUR);

	fseek(pFile, sizeof(RwChar) * 10, SEEK_CUR);
	fseek(pFile, sizeof(RwReal), SEEK_CUR);
	fseek(pFile, sizeof(RwChar) * 10, SEEK_CUR);
	fseek(pFile, sizeof(RwChar) * 10, SEEK_CUR);
	fseek(pFile, sizeof(RwReal), SEEK_CUR);
	fseek(pFile, sizeof(RwReal), SEEK_CUR);

	return ftell(pFile);
}

BYTE* CNtlWorldFieldInfo::FieldSkySaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	RwInt32 SizMemBlock =	sizeof(RwInt32)	+			// base sky mode
		(sizeof(RwRGBA) * 3) +		// rgb sky color
		sizeof(RwReal) +			// unknown new sky value
		(sizeof(RwChar) * 10) +		// base sky texture name
		sizeof(RwReal) +			// base sky speed
		(sizeof(RwChar) * 10 * 2) +	// blended tex name
		sizeof(RwReal) * 2;			// blended tex speed

	fwrite(pFileMem, SizMemBlock, 1, pFile);
	pFileMem += SizMemBlock;

	return pFileMem;
}

BYTE* CNtlWorldFieldInfo::FieldSkySkipToFileMem(BYTE* pFileMem)
{
	RwInt32 SizMemBlock =	sizeof(RwInt32)	+			// base sky mode
		(sizeof(RwRGBA) * 3) +		// rgb sky color
		sizeof(RwReal) +			// unknown new sky value
		(sizeof(RwChar) * 10) +		// base sky texture name
		sizeof(RwReal) +			// base sky speed
		(sizeof(RwChar) * 10 * 2) +	// blended tex name
		sizeof(RwReal) * 2;			// blended tex speed

	pFileMem += SizMemBlock;

	return pFileMem;
}

// RealLight
RwBool CNtlWorldFieldInfo::FieldRealLightLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fread(&pNtlFieldProp->_ClrLightAmbient, sizeof(RwRGBAReal), 1, pFile);

	for (RwInt32 i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
	{
		fread(&pNtlFieldProp->_ClrLightDirectional[i], sizeof(RwRGBAReal), 1, pFile);
		fread(&pNtlFieldProp->_AngLightDirectional[i], sizeof(RwV3d), 1, pFile);
	}

	return TRUE;
}

RwBool CNtlWorldFieldInfo::FieldRealLightSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fwrite(&pNtlFieldProp->_ClrLightAmbient, sizeof(RwRGBAReal), 1, pFile);

	for (RwInt32 i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
	{
		fwrite(&pNtlFieldProp->_ClrLightDirectional[i], sizeof(RwRGBAReal), 1, pFile);
		fwrite(&pNtlFieldProp->_AngLightDirectional[i], sizeof(RwV3d), 1, pFile);
	}

	return TRUE;
}

RwInt32 CNtlWorldFieldInfo::FieldRealLightSkipToFile(FILE* pFile)
{
	fseek(pFile, sizeof(RwRGBAReal), SEEK_CUR);

	fseek(pFile, sizeof(RwRGBAReal) * dNTL_PL_DIRECTIONAL_LIGHT_NUM, SEEK_CUR);
	fseek(pFile, sizeof(RwV3d) * dNTL_PL_DIRECTIONAL_LIGHT_NUM, SEEK_CUR);

	return ftell(pFile);
}

BYTE* CNtlWorldFieldInfo::FieldRealLightSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	RwInt32 SizMemBlock =	sizeof(RwRGBAReal) +	// light ambient
		sizeof(RwRGBAReal) +	// light directional
		sizeof(RwV3d) +			// light directional angle	
		sizeof(RwRGBAReal) +	// light directional
		sizeof(RwV3d) +			// light directional angle	
		sizeof(RwRGBAReal) +	// light directional
		sizeof(RwV3d);			// light directional angle	

	fwrite(pFileMem, SizMemBlock, 1, pFile);
	pFileMem += SizMemBlock;

	return pFileMem;
}

BYTE* CNtlWorldFieldInfo::FieldRealLightSkipToFileMem(BYTE* pFileMem)
{
	RwInt32 SizMemBlock =	sizeof(RwRGBAReal) +	// light ambient
		sizeof(RwRGBAReal) +	// light directional
		sizeof(RwV3d) +			// light directional angle	
		sizeof(RwRGBAReal) +	// light directional
		sizeof(RwV3d) +			// light directional angle	
		sizeof(RwRGBAReal) +	// light directional
		sizeof(RwV3d);			// light directional angle	

	pFileMem += SizMemBlock;

	return pFileMem;
}

// BGM
RwBool CNtlWorldFieldInfo::FieldBGMLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fread(&pNtlFieldProp->_IdxBGMTbl, sizeof(RwInt32), 1, pFile);
	fread(&pNtlFieldProp->_IdxEnvTbl, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < 5; ++i)
	{
		fread(&pNtlFieldProp->_IdxShareTbl[i], sizeof(RwInt32), 1, pFile);
	}

	return TRUE;
}

RwBool CNtlWorldFieldInfo::FieldBGMSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fwrite(&pNtlFieldProp->_IdxBGMTbl, sizeof(RwInt32), 1, pFile);
	fwrite(&pNtlFieldProp->_IdxEnvTbl, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < 5; ++i)
	{
		fwrite(&pNtlFieldProp->_IdxShareTbl[i], sizeof(RwInt32), 1, pFile);
	}

	return TRUE;
}

RwInt32 CNtlWorldFieldInfo::FieldBGMSkipToFile(FILE* pFile)
{
	fseek(pFile, sizeof(RwInt32), SEEK_CUR);
	fseek(pFile, sizeof(RwInt32), SEEK_CUR);
	for (RwInt32 i = 0; i < 5; ++i)
	{
		fseek(pFile, sizeof(RwInt32), SEEK_CUR);
	}

	return ftell(pFile);
}

BYTE* CNtlWorldFieldInfo::FieldBGMSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	// Bgm
	fwrite(pFileMem, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);

	// Env
	fwrite(pFileMem, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);

	// Share
	for (int i = 0; i < 5; ++i)
	{
		fwrite(pFileMem, sizeof(RwInt32), 1, pFile);
		pFileMem += sizeof(RwInt32);
	}

	return pFileMem;
}

BYTE* CNtlWorldFieldInfo::FieldBGMSkipToFileMem(BYTE* pFileMem)
{
	// Bgm
	pFileMem += sizeof(RwInt32);

	// Env
	pFileMem += sizeof(RwInt32);

	// Share
	for (int i = 0; i < 5; ++i)
	{
		pFileMem += sizeof(RwInt32);
	}

	return pFileMem;
}

// Bloom
RwBool CNtlWorldFieldInfo::FieldBloomLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fread(&pNtlFieldProp->_MonoPower, sizeof(RwReal), 1, pFile);

	return TRUE;
}

RwBool CNtlWorldFieldInfo::FieldBloomSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fwrite(&pNtlFieldProp->_MonoPower, sizeof(RwReal), 1, pFile);

	return TRUE;
}

RwInt32 CNtlWorldFieldInfo::FieldBloomSkipToFile(FILE* pFile)
{
	fseek(pFile, sizeof(RwReal), SEEK_CUR);

	return ftell(pFile);
}

BYTE* CNtlWorldFieldInfo::FieldBloomSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	fwrite(pFileMem, sizeof(RwReal), 1, pFile);
	pFileMem += sizeof(RwReal);

	return pFileMem;
}

BYTE* CNtlWorldFieldInfo::FieldBloomSkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwReal);

	return pFileMem;
}

// Planet
RwBool CNtlWorldFieldInfo::FieldPlanetLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	for (RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
	{
		fread(&pNtlFieldProp->_NtlPlanet[i], sizeof(sNTL_PLANET), 1, pFile);
	}

	return TRUE;
}

RwBool CNtlWorldFieldInfo::FieldPlanetSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
	{
		fwrite(&pNtlFieldProp->_NtlPlanet[i], sizeof(sNTL_PLANET), 1, pFile);
	}

	return TRUE;
}

RwInt32 CNtlWorldFieldInfo::FieldPlanetSkipToFile(FILE* pFile)
{
	fseek(pFile, sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT, SEEK_CUR);

	return ftell(pFile);
}

BYTE* CNtlWorldFieldInfo::FieldPlanetSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	fwrite(pFileMem, sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT, 1, pFile);
	pFileMem += sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT;

	return pFileMem;
}

BYTE* CNtlWorldFieldInfo::FieldPlanetSkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT;

	return pFileMem;
}

// Specular
RwBool CNtlWorldFieldInfo::FieldSpecularLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fread(&pNtlFieldProp->_NtlSpecular, sizeof(sNTL_SPECULAR), 1, pFile);

	return TRUE;
}

RwBool CNtlWorldFieldInfo::FieldSpecularSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fwrite(&pNtlFieldProp->_NtlSpecular, sizeof(sNTL_SPECULAR), 1, pFile);

	return TRUE;
}

RwInt32 CNtlWorldFieldInfo::FieldSpecularSkipToFile(FILE* pFile)
{
	fseek(pFile, sizeof(sNTL_SPECULAR), SEEK_CUR);

	return ftell(pFile);
}

BYTE* CNtlWorldFieldInfo::FieldSpecularSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	fwrite(pFileMem, sizeof(sNTL_SPECULAR), 1, pFile);
	pFileMem += sizeof(sNTL_SPECULAR);

	return pFileMem;
}

BYTE* CNtlWorldFieldInfo::FieldSpecularSkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(sNTL_SPECULAR);

	return pFileMem;
}

// WorldMaterial
RwBool CNtlWorldFieldInfo::FieldWorldMaterialLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp, RwInt32 iTileNumInField)
{
	/*(dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2)*/
	fread(pNtlFieldProp->_pMaterialProp, sizeof(BYTE) * iTileNumInField, 1, pFile);

	return TRUE;
}

RwBool CNtlWorldFieldInfo::FieldWorldMaterialSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp, RwInt32 iTileNumInField)
{
	fwrite(pNtlFieldProp->_pMaterialProp, sizeof(BYTE) * iTileNumInField, 1, pFile);

	return TRUE;
}

RwInt32 CNtlWorldFieldInfo::FieldWorldMaterialSkipToFile(FILE* pFile, RwInt32 iTileNumInField)
{
	fseek(pFile, sizeof(BYTE) * iTileNumInField, SEEK_CUR);

	return ftell(pFile);
}

BYTE* CNtlWorldFieldInfo::FieldWorldMaterialSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iTileNumInField)
{
	fwrite(pFileMem, sizeof(BYTE) * iTileNumInField, 1, pFile);
	pFileMem += sizeof(BYTE) * iTileNumInField;

	return pFileMem;
}

BYTE* CNtlWorldFieldInfo::FieldWorldMaterialSkipToFileMem(BYTE* pFileMem, RwInt32 iTileNumInField)
{
	pFileMem += sizeof(BYTE) * iTileNumInField;

	return pFileMem;
}

// FieldColor
RwBool CNtlWorldFieldInfo::FieldColorLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fread(&pNtlFieldProp->_FieldColor, sizeof(RwRGBA), 1, pFile);

	return TRUE;
}

RwBool CNtlWorldFieldInfo::FieldColorSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fwrite(&pNtlFieldProp->_FieldColor, sizeof(RwRGBA), 1, pFile);

	return TRUE;
}

RwInt32 CNtlWorldFieldInfo::FieldColorSkipToFile(FILE* pFile)
{
	fseek(pFile, sizeof(RwRGBA), SEEK_CUR);

	return ftell(pFile);
}

BYTE* CNtlWorldFieldInfo::FieldColorSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	fwrite(pFileMem, sizeof(RwRGBA), 1, pFile);
	pFileMem += sizeof(RwRGBA);

	return pFileMem;
}

BYTE* CNtlWorldFieldInfo::FieldColorSkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwRGBA);

	return pFileMem;
}

// Weather
RwBool CNtlWorldFieldInfo::FieldWeatherLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fread(pNtlFieldProp->_IdxWeather, sizeof(RwInt32) * 5, 1, pFile);

	return TRUE;
}

RwBool CNtlWorldFieldInfo::FieldWeatherSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fwrite(pNtlFieldProp->_IdxWeather, sizeof(RwInt32) * 5, 1, pFile);

	return TRUE;
}

RwInt32 CNtlWorldFieldInfo::FieldWeatherSkipToFile(FILE* pFile)
{

	fseek(pFile, sizeof(RwInt32) * 5, SEEK_CUR);

	return ftell(pFile);
}

BYTE* CNtlWorldFieldInfo::FieldWeatherSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	fwrite(pFileMem, sizeof(RwInt32) * 5, 1, pFile);
	pFileMem += sizeof(RwInt32) * 5;

	return pFileMem;
}

BYTE* CNtlWorldFieldInfo::FieldWeatherSkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwInt32) * 5;

	return pFileMem;
}

// HeatHaze
RwBool CNtlWorldFieldInfo::FieldHeatHazeLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fread(&pNtlFieldProp->_NtlHeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);

	return TRUE;
}

RwBool CNtlWorldFieldInfo::FieldHeatHazeSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp)
{
	fwrite(&pNtlFieldProp->_NtlHeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);

	return TRUE;
}

RwInt32 CNtlWorldFieldInfo::FieldHeatHazeSkipToFile(FILE* pFile)
{
	fseek(pFile, sizeof(sNTL_HEATHAZE), SEEK_CUR);

	return ftell(pFile);
}

BYTE* CNtlWorldFieldInfo::FieldHeatHazeSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	fwrite(pFileMem, sizeof(sNTL_HEATHAZE), 1, pFile);
	pFileMem += sizeof(sNTL_HEATHAZE);

	return pFileMem;
}

BYTE* CNtlWorldFieldInfo::FieldHeatHazeSkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(sNTL_HEATHAZE);

	return pFileMem;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif