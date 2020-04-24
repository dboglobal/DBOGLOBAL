//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ntlworldfield.cpp
//
//  copyright (c) 2005 ntl Ltd.
//
//  author : woody1019
//
//  purpose : manage field
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// headers
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "precomp_ntlpresentation.h"
#include "ntlworldfieldinfo.h"
#include "NtlWorldfield.h"
#include "NtlDebug.h"
#include "NtlPLSky.h"
#include "ntlworldcommon.h"
#include "NtlPLResourcePack.h"
#include "NtlPLPlanet.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlWorldField : 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNtlWorldField::CNtlWorldField()
{
	m_pProp = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CNtlWorldField :
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNtlWorldField::~CNtlWorldField(void)
{
	Free();
}

void CNtlWorldField::Init(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWorldField::Init");

	m_SPos = Pos;

	SetTexAttrDefault();

	NTL_RETURNVOID();
}

VOID CNtlWorldField::SetTexAttrDefault()
{
	::ZeroMemory(&m_TexAttr, sizeof(sCUR_FIELD_TEX_INFO));
	::sprintf_s(m_TexAttr.BLT.StrName, dGET_WORLD_PARAM()->WorldTexAddr, "NTL");
	::sprintf_s(m_TexAttr.DLT[0].StrName, dGET_WORLD_PARAM()->WorldTexAddr, "not registered");
	::sprintf_s(m_TexAttr.DLT[1].StrName, dGET_WORLD_PARAM()->WorldTexAddr, "not registered");
	::sprintf_s(m_TexAttr.DLT[2].StrName, dGET_WORLD_PARAM()->WorldTexAddr, "not registered");
	::sprintf_s(m_TexAttr.DLT[3].StrName, dGET_WORLD_PARAM()->WorldTexAddr, "not registered");
	::sprintf_s(m_TexAttr.DLT[4].StrName, dGET_WORLD_PARAM()->WorldTexAddr, "not registered");
	m_TexAttr.BLT.AngTexAdd = dGET_WORLD_PARAM()->WorldTexAddr;
	m_TexAttr.DLT[0].AngTexAdd = dGET_WORLD_PARAM()->WorldTexAddr;
	m_TexAttr.DLT[1].AngTexAdd = dGET_WORLD_PARAM()->WorldTexAddr;
	m_TexAttr.DLT[2].AngTexAdd = dGET_WORLD_PARAM()->WorldTexAddr;
	m_TexAttr.DLT[3].AngTexAdd = dGET_WORLD_PARAM()->WorldTexAddr;
	m_TexAttr.DLT[4].AngTexAdd = dGET_WORLD_PARAM()->WorldTexAddr;
}

RwBool CNtlWorldField::CreateFieldInMemory(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldField::CreateFieldInMemory");

	m_pProp = NTL_NEW sNTL_FIELD_PROP;

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldField::CreateFieldFromFile(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldField::CreateFieldFromFile");

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE *pFile = NULL;

	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
	{
		static RwChar chPackPatch[NTL_MAX_DIR_PATH];
		strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "fields");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", FieldIdx);

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
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", FieldIdx);

		::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
	}

	if(pFile == NULL)
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		NTL_RETURN(FALSE);
	}
	else
	{
		m_pProp = NTL_NEW sNTL_FIELD_PROP;

		RwInt32 iTileNumInField = (dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldFieldSectorNum) * (dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldFieldSectorNum);

		GetNtlWorldFieldInfo()->FieldNameLoadFromFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldBitPropLoadFromFile(pFile, m_pProp, iTileNumInField);
		GetNtlWorldFieldInfo()->FieldFogLoadFromFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldSkyLoadFromFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldRealLightLoadFromFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldBGMLoadFromFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldBloomLoadFromFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldPlanetLoadFromFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldSpecularLoadFromFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldWorldMaterialLoadFromFile(pFile, m_pProp, iTileNumInField);
		GetNtlWorldFieldInfo()->FieldColorLoadFromFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldWeatherLoadFromFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldHeatHazeLoadFromFile(pFile, m_pProp);
						  
		fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldField::DeleteFieldInMemory(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldField::DeleteFieldInMemory");

	SetTexAttrDefault();

	NTL_DELETE(m_pProp);

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldField::DeleteFieldIntoFile(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldField::DeleteFieldIntoFile");
	
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE *pFile = NULL;
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", FieldIdx);
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		NTL_RETURN(FALSE);
	}
	else
	{
		RwInt32 iTileNumInField = (dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldFieldSectorNum) * (dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldFieldSectorNum);

		GetNtlWorldFieldInfo()->FieldNameSaveIntoFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldBitPropSaveIntoFile(pFile, m_pProp, iTileNumInField);
		GetNtlWorldFieldInfo()->FieldFogSaveIntoFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldSkySaveIntoFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldRealLightSaveIntoFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldBGMSaveIntoFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldBloomSaveIntoFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldPlanetSaveIntoFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldSpecularSaveIntoFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldWorldMaterialSaveIntoFile(pFile, m_pProp, iTileNumInField);
		GetNtlWorldFieldInfo()->FieldColorSaveIntoFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldWeatherSaveIntoFile(pFile, m_pProp);
		GetNtlWorldFieldInfo()->FieldHeatHazeSaveIntoFile(pFile, m_pProp);

		::fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(TRUE);
}

void CNtlWorldField::Free()
{
	NTL_FUNCTION("CNtlWorldField::Free");

	NTL_RETURNVOID();
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlWorldField : 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNtlWorldField::CNtlWorldField()
{
	m_pProp = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CNtlWorldField :
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNtlWorldField::~CNtlWorldField(void)
{
	Free();
}

void CNtlWorldField::Init(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWorldField::Init");

	m_SPos = Pos;

	SetTexAttrDefault();

	NTL_RETURNVOID();
}

VOID CNtlWorldField::SetTexAttrDefault()
{
	::ZeroMemory(&m_TexAttr, sizeof(sCUR_FIELD_TEX_INFO));
	::sprintf_s(m_TexAttr.BLT.StrName, dGET_WORLD_PARAM()->WorldTexAddr, "NTL");
	::sprintf_s(m_TexAttr.DLT[0].StrName, dGET_WORLD_PARAM()->WorldTexAddr, "not registered");
	::sprintf_s(m_TexAttr.DLT[1].StrName, dGET_WORLD_PARAM()->WorldTexAddr, "not registered");
	::sprintf_s(m_TexAttr.DLT[2].StrName, dGET_WORLD_PARAM()->WorldTexAddr, "not registered");
	::sprintf_s(m_TexAttr.DLT[3].StrName, dGET_WORLD_PARAM()->WorldTexAddr, "not registered");
	::sprintf_s(m_TexAttr.DLT[4].StrName, dGET_WORLD_PARAM()->WorldTexAddr, "not registered");
	m_TexAttr.BLT.AngTexAdd = dGET_WORLD_PARAM()->WorldTexAddr;
	m_TexAttr.DLT[0].AngTexAdd = dGET_WORLD_PARAM()->WorldTexAddr;
	m_TexAttr.DLT[1].AngTexAdd = dGET_WORLD_PARAM()->WorldTexAddr;
	m_TexAttr.DLT[2].AngTexAdd = dGET_WORLD_PARAM()->WorldTexAddr;
	m_TexAttr.DLT[3].AngTexAdd = dGET_WORLD_PARAM()->WorldTexAddr;
	m_TexAttr.DLT[4].AngTexAdd = dGET_WORLD_PARAM()->WorldTexAddr;
}

RwBool CNtlWorldField::CreateFieldInMemory(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldField::CreateFieldInMemory");

	m_pProp = NTL_NEW sNTL_FIELD_PROP;

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldField::CreateFieldFromFile(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldField::CreateFieldFromFile");

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE *pFile = NULL;

	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
	{
		static RwChar chPackPatch[NTL_MAX_DIR_PATH];
		strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "fields");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", FieldIdx);

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
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", FieldIdx);

		::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
	}

	if(pFile == NULL)
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		NTL_RETURN(FALSE);
	}
	else
	{
		/*
		WorldFileFormat - FieldProperty
		Field Property를 File로부터 Load 하는 부분 이다.
		*/

		m_pProp = NTL_NEW sNTL_FIELD_PROP;

		fread(m_pProp->_Name, 64, 1, pFile);
		fread(m_pProp->_pBitProp, sizeof(DWORD) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
		fread(&m_pProp->_FogColor, sizeof(RwRGBA), 1, pFile);
		fread(&m_pProp->_FogCamPlane[0], sizeof(RwReal), 1, pFile);
		fread(&m_pProp->_FogCamPlane[1], sizeof(RwReal), 1, pFile);
		fread(&m_pProp->_BaseSkyMode, sizeof(RwInt32), 1, pFile);
		fread(&m_pProp->_RGBSkyColor[0], sizeof(RwRGBA), 1, pFile);
		fread(&m_pProp->_RGBSkyColor[1], sizeof(RwRGBA), 1, pFile);
		fread(&m_pProp->_RGBSkyColor[2], sizeof(RwRGBA), 1, pFile);
		fread(m_pProp->_BaseSkyTexName, sizeof(RwChar) * 10, 1, pFile);
		fread(&m_pProp->_BaseSkySpeed, sizeof(RwReal), 1, pFile);
		fread(m_pProp->_BlendedTexName[0], sizeof(RwChar) * 10, 1, pFile);
		fread(m_pProp->_BlendedTexName[1], sizeof(RwChar) * 10, 1, pFile);
		fread(&m_pProp->_BlendedTexSpeed[0], sizeof(RwReal), 1, pFile);
		fread(&m_pProp->_BlendedTexSpeed[1], sizeof(RwReal), 1, pFile);
		fread(&m_pProp->_ClrLightAmbient, sizeof(RwRGBAReal), 1, pFile);

		for (RwInt32 i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
		{
			fread(&m_pProp->_ClrLightDirectional[i], sizeof(RwRGBAReal), 1, pFile);
			fread(&m_pProp->_AngLightDirectional[i], sizeof(RwV3d), 1, pFile);
			if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
			{
				fseek(pFile, sizeof(RwRGBAReal), SEEK_CUR);
				fseek(pFile, sizeof(RwV3d), SEEK_CUR);
				fseek(pFile, sizeof(RwRGBAReal), SEEK_CUR);
				fseek(pFile, sizeof(RwV3d), SEEK_CUR);
				break;
			}
		}

		fread(&m_pProp->_IdxBGMTbl, sizeof(RwInt32), 1, pFile);
		fread(&m_pProp->_IdxEnvTbl, sizeof(RwInt32), 1, pFile);
		for (RwInt32 i = 0; i < 5; ++i)
		{
			fread(&m_pProp->_IdxShareTbl[i], sizeof(RwInt32), 1, pFile);
		}

		fread(&m_pProp->_MonoPower, sizeof(RwReal), 1, pFile);

		for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
			fread(&m_pProp->_NtlPlanet[i], sizeof(sNTL_PLANET), 1, pFile);

		fread(&m_pProp->_NtlSpecular, sizeof(sNTL_SPECULAR), 1, pFile);
		fread(m_pProp->_pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
		fread(&m_pProp->_FieldColor, sizeof(RwRGBA), 1, pFile);
		fread(m_pProp->_IdxWeather, sizeof(RwInt32) * 5, 1, pFile);
		fread(&m_pProp->_NtlHeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);

		fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldField::DeleteFieldInMemory(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldField::DeleteFieldInMemory");

	SetTexAttrDefault();

	NTL_DELETE(m_pProp);

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldField::DeleteFieldIntoFile(RwInt32 FieldIdx)
{
	NTL_FUNCTION("CNtlWorldField::DeleteFieldIntoFile");

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	FILE *pFile = NULL;
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", FieldIdx);
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		NTL_RETURN(FALSE);
	}
	else
	{
		/*
		WorldFileFormat - FieldProperty
		Field Property를 File로 Save 하는 부분 이다.
		*/
		fwrite(m_pProp->_Name, 64, 1, pFile);
		fwrite(m_pProp->_pBitProp, sizeof(DWORD) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
		fwrite(&m_pProp->_FogColor, sizeof(RwRGBA), 1, pFile);
		fwrite(&m_pProp->_FogCamPlane[0], sizeof(RwReal), 1, pFile);
		fwrite(&m_pProp->_FogCamPlane[1], sizeof(RwReal), 1, pFile);
		fwrite(&m_pProp->_BaseSkyMode, sizeof(RwInt32), 1, pFile);
		fwrite(&m_pProp->_RGBSkyColor[0], sizeof(RwRGBA), 1, pFile);
		fwrite(&m_pProp->_RGBSkyColor[1], sizeof(RwRGBA), 1, pFile);
		fwrite(&m_pProp->_RGBSkyColor[2], sizeof(RwRGBA), 1, pFile);
		fwrite(m_pProp->_BaseSkyTexName, sizeof(RwChar) * 10, 1, pFile);
		fwrite(&m_pProp->_BaseSkySpeed, sizeof(RwReal), 1, pFile);
		fwrite(m_pProp->_BlendedTexName[0], sizeof(RwChar) * 10, 1, pFile);
		fwrite(m_pProp->_BlendedTexName[1], sizeof(RwChar) * 10, 1, pFile);
		fwrite(&m_pProp->_BlendedTexSpeed[0], sizeof(RwReal), 1, pFile);
		fwrite(&m_pProp->_BlendedTexSpeed[1], sizeof(RwReal), 1, pFile);
		fwrite(&m_pProp->_ClrLightAmbient, sizeof(RwRGBAReal), 1, pFile);

		for(RwInt32 i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
		{
			fwrite(&m_pProp->_ClrLightDirectional[i], sizeof(RwRGBAReal), 1, pFile);
			fwrite(&m_pProp->_AngLightDirectional[i], sizeof(RwV3d), 1, pFile);
		}

		fwrite(&m_pProp->_IdxBGMTbl, sizeof(RwInt32), 1, pFile);
		fwrite(&m_pProp->_IdxEnvTbl, sizeof(RwInt32), 1, pFile);
		for (RwInt32 i = 0; i < 5; ++i)
		{
			fwrite(&m_pProp->_IdxShareTbl[i], sizeof(RwInt32), 1, pFile);
		}

		fwrite(&m_pProp->_MonoPower, sizeof(RwReal), 1, pFile);

		for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
			fwrite(&m_pProp->_NtlPlanet[i], sizeof(sNTL_PLANET), 1, pFile);

		fwrite(&m_pProp->_NtlSpecular, sizeof(sNTL_SPECULAR), 1, pFile);
		fwrite(m_pProp->_pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
		fwrite(&m_pProp->_FieldColor, sizeof(RwRGBA), 1, pFile);
		fwrite(m_pProp->_IdxWeather, sizeof(RwInt32) * 5, 1, pFile);
		fwrite(&m_pProp->_NtlHeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);

		::fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	NTL_RETURN(TRUE);
}

void CNtlWorldField::Free()
{
	NTL_FUNCTION("CNtlWorldField::Free");

	NTL_RETURNVOID();
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif