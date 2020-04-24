#include "precomp_ntlpresentation.h"

#include "NtlProfiler.h"

#include "NtlPLRenderState.h"

#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"

#include "NtlPLResourcePack.h"
#include "NtlPLAtomicSorter.h"
#include "NtlPLResourceManager.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLPlanet.h"
#include "NtlPLWater.h"
#include "NtlPLPlant.h"
#include "NtlPLGameProperty.h"

#include "NtlWorldRpLightManager.h"

#include "NtlWorldBlockManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlWorldLTManager.h"
#include "NtlOccluderManager.h"

#include "NtlPLDecal.h"
#include "NtlPLOccluder_Base.h"

#include "NtlPLCollision.h"
#include "NtlPLRenderGeometry.h"

CNtlWorldBlockMananger::CNtlWorldBlockMananger()
:m_iWorldBlockSize(0)
,m_iWorldBlockNum(0)
,m_iOldBlockIdx(-1)
,m_iNewBlockIdx(-1)
,m_fAnotherBlockVariationTime(RwRealMINVAL)
,m_pWorldFieldProp(NULL)
,m_pWorldBlockProp(NULL)
{
}

CNtlWorldBlockMananger::~CNtlWorldBlockMananger()
{
}

void CNtlWorldBlockMananger::Init(RwInt32 iWorldSize, RwInt32 iWorldBlockSize)
{
	NTL_SPROFILE("CNtlWorldBlockMananger::Init");

	m_iWorldBlockSize	= iWorldBlockSize;
	m_iWorldBlockNum	= iWorldSize / iWorldBlockSize;

#ifdef dNTL_WORLD_TOOL_MODE
	NTL_ASSERT(m_iWorldBlockNum * m_iWorldBlockSize == iWorldSize, " BlockNum * BlockSize != WorldSize");
#endif

	m_pWorldFieldProp	= NTL_NEW sNTL_FIELD_PROP [m_iWorldBlockNum * m_iWorldBlockNum];
	m_pWorldBlockProp	= NTL_NEW sNTL_BLOCK_PROP [m_iWorldBlockNum * m_iWorldBlockNum];

	NTL_EPROFILE();
}

void CNtlWorldBlockMananger::Free()
{
	NTL_SPROFILE("CNtlWorldBlockMananger::Free");

	NTL_ARRAY_DELETE(m_pWorldBlockProp);
	NTL_ARRAY_DELETE(m_pWorldFieldProp);

	NTL_EPROFILE();
}

void CNtlWorldBlockMananger::Update(RwReal fElapsed, RwV3d& vPos, eNTL_WORLD_DIR eDir)
{
	UpdateAnotherBlock(fElapsed, vPos, eDir);
}

void CNtlWorldBlockMananger::UpdateAnotherBlock(RwReal fElapsed, RwV3d& vPos, eNTL_WORLD_DIR eDir)
{
	if (m_fAnotherBlockVariationTime > 0.0f)
	{
		m_fAnotherBlockVariationTime -= fElapsed;
		if (m_fAnotherBlockVariationTime < 0.0f)
		{
			// Send MapName Message
			sNTL_FIELD_PROP sNtlFieldProp;
			GetAFieldProp(vPos, sNtlFieldProp);

			if(!_stricmp(sNtlFieldProp._Name, "null") || !_stricmp(sNtlFieldProp._Name, ""))
			{
				m_CurMapNameIdx = 0xffffffff;
			}
			else
			{
				TBLIDX CurTblIdx = static_cast<TBLIDX>(atoi(sNtlFieldProp._Name));

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

			// Send Block Changed Message
			CNtlPLEventGenerator::IsAnotherBlockChanged(&sNtlFieldProp);

			return;
		}
	}

	RwInt32 iBlockIdx = GetBlockIdx(vPos);
	if (iBlockIdx == -1)
	{
		return;
	}

	m_iNewBlockIdx = iBlockIdx;
	if (m_iOldBlockIdx == m_iNewBlockIdx)
	{
		return;
	}
	m_iOldBlockIdx = m_iNewBlockIdx;

	SetAnotherBlock((eDir == eC || eDir == ePORTAL ? 0.0001f : dFIELD_EFFECT_SWITCHING_TIME));
	
	// Send Block Changed Message Without Delay
	sNTL_FIELD_PROP NtlFieldProp;
	GetAFieldProp(vPos, NtlFieldProp);
	CNtlPLEventGenerator::IsAnotherBlockChangedWithoutDelay(&NtlFieldProp);
}

RwBool CNtlWorldBlockMananger::IsBlockValid(RwV3d& vPos)
{
	if(vPos.x <= dGET_WORLD_PARAM()->WorldValueMax &&
		vPos.z <= dGET_WORLD_PARAM()->WorldValueMax &&
		vPos.x >= dGET_WORLD_PARAM()->WorldValueMin &&
		vPos.z >= dGET_WORLD_PARAM()->WorldValueMin)
	{
		return TRUE;
	}

	return FALSE;
}

RwBool CNtlWorldBlockMananger::IsBlockValid(RwInt32 iIdx)
{
	if(iIdx < 0 || iIdx > (m_iWorldBlockNum * m_iWorldBlockNum - 1))
	{
		return FALSE;
	}
	return TRUE;
}

RwInt32 CNtlWorldBlockMananger::GetBlockIdx(RwV3d& vPos)
{
	if(!IsBlockValid(vPos))
	{
		return -1;
	}

	RwInt32 RetIdx = CNtlMath::GetSafeIdx3D(vPos, dGET_WORLD_PARAM()->WorldSizeHalf, m_iWorldBlockSize, m_iWorldBlockNum);

// 	RwInt32 iWidthNum, iDepthNum;
// 	RwReal fXBasedOn0 = (RwReal)((RwInt32)vPos.x + dGET_WORLD_PARAM()->WorldSizeHalf);
// 	RwReal fZBasedOn0 = (RwReal)((RwInt32)vPos.z + dGET_WORLD_PARAM()->WorldSizeHalf);
// 	iWidthNum = (RwInt32)(fXBasedOn0 / (RwReal)m_iWorldBlockSize);
// 	iDepthNum = (RwInt32)(fZBasedOn0 / (RwReal)m_iWorldBlockSize);
// 
// 	RwInt32 RetIdx = iWidthNum + (iDepthNum * m_iWorldBlockNum);

	if(!IsBlockValid(RetIdx))
	{
		return -1;
	}

	return RetIdx;
}

void CNtlWorldBlockMananger::SetAnotherBlock(RwReal fVariationTime)
{
	m_fAnotherBlockVariationTime = fVariationTime;
}

RwBool CNtlWorldBlockMananger::GetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp)
{
	RwInt32 iIdx = GetBlockIdx(Pos);
	if (iIdx == -1)
	{
		return FALSE;
	}

	NtlFieldProp = m_pWorldFieldProp[iIdx];
	return TRUE;
}

RwBool CNtlWorldBlockMananger::SetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp, eNTL_FIELD_PROPID NtlFieldPropID)
{
	RwInt32 iIdx = GetBlockIdx(Pos);
	if (iIdx == -1)
	{
		return FALSE;
	}

	switch(NtlFieldPropID)
	{
	case eNFP_NAME:
		{
			strcpy_s(m_pWorldFieldProp[iIdx]._Name, NtlFieldProp._Name);

			break;
		}
	case eNFP_BITPROP:
		{
			//::CopyMemory(m_pWorldFieldProp[iIdx]._pBitProp, NtlFieldProp._pBitProp, sizeof(DWORD) * NumSectorTile);

			break;
		}
	case eNFP_FOG:
		{
			m_pWorldFieldProp[iIdx]._FogColor.red	= NtlFieldProp._FogColor.red;
			m_pWorldFieldProp[iIdx]._FogColor.green	= NtlFieldProp._FogColor.green;
			m_pWorldFieldProp[iIdx]._FogColor.blue	= NtlFieldProp._FogColor.blue;
			m_pWorldFieldProp[iIdx]._FogCamPlane[0]	= NtlFieldProp._FogCamPlane[0];
			m_pWorldFieldProp[iIdx]._FogCamPlane[1]	= NtlFieldProp._FogCamPlane[1];	

			break;
		}
	case eNFP_SKY:
		{
			m_pWorldFieldProp[iIdx]._BaseSkyMode	= NtlFieldProp._BaseSkyMode;
			memcpy(&m_pWorldFieldProp[iIdx]._RGBSkyColor[0], &NtlFieldProp._RGBSkyColor[0], sizeof(RwRGBA));
			memcpy(&m_pWorldFieldProp[iIdx]._RGBSkyColor[1], &NtlFieldProp._RGBSkyColor[1], sizeof(RwRGBA));
			memcpy(&m_pWorldFieldProp[iIdx]._RGBSkyColor[2], &NtlFieldProp._RGBSkyColor[2], sizeof(RwRGBA));

			m_pWorldFieldProp[iIdx]._NewSkyValue = NtlFieldProp._NewSkyValue; // new

			strcpy_s(m_pWorldFieldProp[iIdx]._BaseSkyTexName, NtlFieldProp._BaseSkyTexName);
			m_pWorldFieldProp[iIdx]._BaseSkySpeed = NtlFieldProp._BaseSkySpeed;
			strcpy_s(m_pWorldFieldProp[iIdx]._BlendedTexName[0], NtlFieldProp._BlendedTexName[0]);
			strcpy_s(m_pWorldFieldProp[iIdx]._BlendedTexName[1], NtlFieldProp._BlendedTexName[1]);
			m_pWorldFieldProp[iIdx]._BlendedTexSpeed[0] = NtlFieldProp._BlendedTexSpeed[0];
			m_pWorldFieldProp[iIdx]._BlendedTexSpeed[1] = NtlFieldProp._BlendedTexSpeed[1];

			break;
		}
	case eNFP_LIGHT:
		{
			::CopyMemory(&m_pWorldFieldProp[iIdx]._ClrLightAmbient, &NtlFieldProp._ClrLightAmbient, sizeof(RwRGBAReal));

			for (RwInt32 i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
			{
				::CopyMemory(&m_pWorldFieldProp[iIdx]._ClrLightDirectional[i], &NtlFieldProp._ClrLightDirectional[i], sizeof(RwRGBAReal));
				::CopyMemory(&m_pWorldFieldProp[iIdx]._AngLightDirectional[i], &NtlFieldProp._AngLightDirectional[i], sizeof(RwV3d));
			}			

			break;
		}
	case eNFP_SOUND:
		{
			m_pWorldFieldProp[iIdx]._IdxBGMTbl = NtlFieldProp._IdxBGMTbl;
			m_pWorldFieldProp[iIdx]._IdxEnvTbl = NtlFieldProp._IdxEnvTbl;
			for (RwInt32 i = 0; i < 5; ++i)
			{
				m_pWorldFieldProp[iIdx]._IdxShareTbl[i] = NtlFieldProp._IdxShareTbl[i];
			}
			break;
		}

	case eNFP_BLOOM:
		{
			m_pWorldFieldProp[iIdx]._MonoPower = NtlFieldProp._MonoPower;

			break;
		}
	case eNFP_PLANET:
		{
			m_pWorldFieldProp[iIdx]._NtlPlanet[0] = NtlFieldProp._NtlPlanet[0];
			m_pWorldFieldProp[iIdx]._NtlPlanet[1] = NtlFieldProp._NtlPlanet[1];
			m_pWorldFieldProp[iIdx]._NtlPlanet[2] = NtlFieldProp._NtlPlanet[2];

			break;
		}
	case eNFP_SPECULAR:
		{					
			m_pWorldFieldProp[iIdx]._NtlSpecular = NtlFieldProp._NtlSpecular;

			break;
		}
	case eNFP_MATERIALPROP:
		{
			//::CopyMemory(m_pWorldFieldProp[iIdx]._pMaterialProp, NtlFieldProp._pMaterialProp, sizeof(BYTE) * NumSectorTile);

			break;
		}
	case eNFP_FIELDCOLOR:
		{
			m_pWorldFieldProp[iIdx]._FieldColor = NtlFieldProp._FieldColor;

			break;
		}
	case eNFP_WEATHER:
		{
			for (int i = 0; i < 5; ++i)
			{
				m_pWorldFieldProp[iIdx]._IdxWeather[i] = NtlFieldProp._IdxWeather[i];
			}

			break;
		}			
	case eNFP_FIELDHEATHAZE:
		{
			m_pWorldFieldProp[iIdx]._NtlHeatHaze = NtlFieldProp._NtlHeatHaze;

			break;
		}
	}

	SaveProperty(iIdx);
	SetAnotherBlock();

	return TRUE;
}

RwBool CNtlWorldBlockMananger::GetABlockProp(RwV3d& Pos, sNTL_BLOCK_PROP& NtlBlockProp)
{
	RwInt32 iIdx = GetBlockIdx(Pos);
	if (iIdx == -1)
	{
		return FALSE;
	}

	NtlBlockProp = m_pWorldBlockProp[iIdx];
	return TRUE;
}

RwBool CNtlWorldBlockMananger::SetABlockProp(RwV3d& Pos, sNTL_BLOCK_PROP& NtlBlockProp, eNTL_BLOCK_PROPID NtlBlockPropID)
{
	RwInt32 iIdx = GetBlockIdx(Pos);
	if (iIdx == -1)
	{
		return FALSE;
	}

	switch(NtlBlockPropID)
	{
	case eNBP_GAMEPROP:
		{
			m_pWorldBlockProp[iIdx]._GameProp = NtlBlockProp._GameProp;

			break;
		}
	}

	SaveProperty(iIdx);
	SetAnotherBlock();

	return TRUE;
}

RwBool CNtlWorldBlockMananger::LoadProperty()
{
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);

	FILE* pFile = NULL;
	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
	{
		static RwChar chPackPatch[NTL_MAX_DIR_PATH];
		strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "wfp");

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
		::fopen_s(&pFile, "wfp", "rb");
	}

	if (pFile == NULL)
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}

	for (RwInt32 i = 0; i < m_iWorldBlockNum * m_iWorldBlockNum; ++i)
	{
		sNTL_FIELD_PROP* pFieldProp = &m_pWorldFieldProp[i];
		sNTL_BLOCK_PROP* pBlockProp = &m_pWorldBlockProp[i];

		// 	GetNtlWorldFieldInfo()->FieldNameLoadFromFile(pFile, m_pNtlObjectProp);
		// 	//GetNtlWorldFieldInfo()->FieldBitPropLoadFromFile(pFile, m_pProp, iTileNumInField);
		// 	GetNtlWorldFieldInfo()->FieldFogLoadFromFile(pFile, m_pNtlObjectProp);
		// 	GetNtlWorldFieldInfo()->FieldSkyLoadFromFile(pFile, m_pNtlObjectProp);
		//GetNtlWorldFieldInfo()->FieldRealLightLoadFromFile(pFile, m_pNtlObjectProp);
		// 	GetNtlWorldFieldInfo()->FieldBGMLoadFromFile(pFile, m_pNtlObjectProp);
		// 	GetNtlWorldFieldInfo()->FieldBloomLoadFromFile(pFile, m_pNtlObjectProp);
		// 	GetNtlWorldFieldInfo()->FieldPlanetLoadFromFile(pFile, m_pNtlObjectProp);
		// 	GetNtlWorldFieldInfo()->FieldSpecularLoadFromFile(pFile, m_pNtlObjectProp);
		// 	//GetNtlWorldFieldInfo()->FieldWorldMaterialLoadFromFile(pFile, m_pProp, iTileNumInField);
		// 	GetNtlWorldFieldInfo()->FieldColorLoadFromFile(pFile, m_pNtlObjectProp);
		// 	GetNtlWorldFieldInfo()->FieldWeatherLoadFromFile(pFile, m_pNtlObjectProp);
		// 	GetNtlWorldFieldInfo()->FieldHeatHazeLoadFromFile(pFile, m_pNtlObjectProp);
		// FIELD PROPERTY :: 인도어에서는 TILE PROPERTY를 사용하지 않는다.
		fread(pFieldProp->_Name, 64, 1, pFile);
		//fread(pFieldProp->_pBitProp, sizeof(DWORD) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
		fread(&pFieldProp->_FogColor, sizeof(RwRGBA), 1, pFile);
		fread(&pFieldProp->_FogCamPlane[0], sizeof(RwReal), 1, pFile);
		fread(&pFieldProp->_FogCamPlane[1], sizeof(RwReal), 1, pFile);
		fread(&pFieldProp->_BaseSkyMode, sizeof(RwInt32), 1, pFile);
		fread(&pFieldProp->_RGBSkyColor[0], sizeof(RwRGBA), 1, pFile);
		fread(&pFieldProp->_RGBSkyColor[1], sizeof(RwRGBA), 1, pFile);
		fread(&pFieldProp->_RGBSkyColor[2], sizeof(RwRGBA), 1, pFile);

		fread(&pFieldProp->_NewSkyValue, sizeof(RwReal), 1, pFile);

		fread(pFieldProp->_BaseSkyTexName, sizeof(RwChar) * 10, 1, pFile);
		fread(&pFieldProp->_BaseSkySpeed, sizeof(RwReal), 1, pFile);
		fread(pFieldProp->_BlendedTexName[0], sizeof(RwChar) * 10, 1, pFile);
		fread(pFieldProp->_BlendedTexName[1], sizeof(RwChar) * 10, 1, pFile);
		fread(&pFieldProp->_BlendedTexSpeed[0], sizeof(RwReal), 1, pFile);
		fread(&pFieldProp->_BlendedTexSpeed[1], sizeof(RwReal), 1, pFile);
		fread(&pFieldProp->_ClrLightAmbient, sizeof(RwRGBAReal), 1, pFile);

		for(RwInt32 j = 0; j < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++j)
		{
			fread(&pFieldProp->_ClrLightDirectional[j], sizeof(RwRGBAReal), 1, pFile);
			fread(&pFieldProp->_AngLightDirectional[j], sizeof(RwV3d), 1, pFile);
			//break;
		}
		
		fread(&pFieldProp->_IdxBGMTbl, sizeof(RwInt32), 1, pFile);
		fread(&pFieldProp->_IdxEnvTbl, sizeof(RwInt32), 1, pFile);
		for (RwInt32 j = 0; j < 5; ++j)
		{
			fread(&pFieldProp->_IdxShareTbl[j], sizeof(RwInt32), 1, pFile);
		}

		fread(&pFieldProp->_MonoPower, sizeof(RwReal), 1, pFile);

		for(RwInt32 j = 0; j < dNTLPL_PLANET_MAX_CNT; ++j)
			fread(&pFieldProp->_NtlPlanet[j], sizeof(sNTL_PLANET), 1, pFile);

		fread(&pFieldProp->_NtlSpecular, sizeof(sNTL_SPECULAR), 1, pFile);
		//fread(pFieldProp->_pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
		fread(&pFieldProp->_FieldColor, sizeof(RwRGBA), 1, pFile);
		fread(pFieldProp->_IdxWeather, sizeof(RwInt32) * 5, 1, pFile);
		fread(&pFieldProp->_NtlHeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);

		// BLOCK PROPERTY
		fread(&pBlockProp->_GameProp, sizeof(RwUInt32), 1, pFile);
	}

	::fclose(pFile);

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldBlockMananger::SaveProperty()
{
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);

	FILE *pFile = NULL;
	if(::fopen_s(&pFile, "wfp", "wb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << "wfp" << ")");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}

	for (RwInt32 i = 0; i < m_iWorldBlockNum * m_iWorldBlockNum; ++i)
	{
		sNTL_FIELD_PROP* pFieldProp = &m_pWorldFieldProp[i];
		sNTL_BLOCK_PROP* pBlockProp = &m_pWorldBlockProp[i];

		// 	GetNtlWorldFieldInfo()->FieldNameSaveIntoFile(pFile, pFieldProp);
		// 	//GetNtlWorldFieldInfo()->FieldBitPropSaveIntoFile(pFile, pFieldProp, iTileNumInField);
		// 	GetNtlWorldFieldInfo()->FieldFogSaveIntoFile(pFile, pFieldProp);
		// 	GetNtlWorldFieldInfo()->FieldSkySaveIntoFile(pFile, pFieldProp);
		// 	GetNtlWorldFieldInfo()->FieldRealLightSaveIntoFile(pFile, pFieldProp);
		// 	GetNtlWorldFieldInfo()->FieldBGMSaveIntoFile(pFile, pFieldProp);
		// 	GetNtlWorldFieldInfo()->FieldBloomSaveIntoFile(pFile, pFieldProp);
		// 	GetNtlWorldFieldInfo()->FieldPlanetSaveIntoFile(pFile, pFieldProp);
		// 	GetNtlWorldFieldInfo()->FieldSpecularSaveIntoFile(pFile, pFieldProp);
		// 	//GetNtlWorldFieldInfo()->FieldWorldMaterialSaveIntoFile(pFile, pFieldProp, iTileNumInField);
		// 	GetNtlWorldFieldInfo()->FieldColorSaveIntoFile(pFile, pFieldProp);
		// 	GetNtlWorldFieldInfo()->FieldWeatherSaveIntoFile(pFile, pFieldProp);
		// 	GetNtlWorldFieldInfo()->FieldHeatHazeSaveIntoFile(pFile, pFieldProp);
		// FIELD PROPERTY :: 인도어에서는 TILE PROPERTY를 사용하지 않는다.
		fwrite(pFieldProp->_Name, 64, 1, pFile);
		//fwrite(pFieldProp->_pBitProp, sizeof(DWORD) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
		fwrite(&pFieldProp->_FogColor, sizeof(RwRGBA), 1, pFile);
		fwrite(&pFieldProp->_FogCamPlane[0], sizeof(RwReal), 1, pFile);
		fwrite(&pFieldProp->_FogCamPlane[1], sizeof(RwReal), 1, pFile);
		fwrite(&pFieldProp->_BaseSkyMode, sizeof(RwInt32), 1, pFile);
		fwrite(&pFieldProp->_RGBSkyColor[0], sizeof(RwRGBA), 1, pFile);
		fwrite(&pFieldProp->_RGBSkyColor[1], sizeof(RwRGBA), 1, pFile);
		fwrite(&pFieldProp->_RGBSkyColor[2], sizeof(RwRGBA), 1, pFile);

		fwrite(&pFieldProp->_NewSkyValue, sizeof(RwReal), 1, pFile);

		fwrite(pFieldProp->_BaseSkyTexName, sizeof(RwChar) * 10, 1, pFile);
		fwrite(&pFieldProp->_BaseSkySpeed, sizeof(RwReal), 1, pFile);
		fwrite(pFieldProp->_BlendedTexName[0], sizeof(RwChar) * 10, 1, pFile);
		fwrite(pFieldProp->_BlendedTexName[1], sizeof(RwChar) * 10, 1, pFile);
		fwrite(&pFieldProp->_BlendedTexSpeed[0], sizeof(RwReal), 1, pFile);
		fwrite(&pFieldProp->_BlendedTexSpeed[1], sizeof(RwReal), 1, pFile);
		fwrite(&pFieldProp->_ClrLightAmbient, sizeof(RwRGBAReal), 1, pFile);
		
		for(RwInt32 j = 0; j < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++j)
		{
			fwrite(&pFieldProp->_ClrLightDirectional[j], sizeof(RwRGBAReal), 1, pFile);
			fwrite(&pFieldProp->_AngLightDirectional[j], sizeof(RwV3d), 1, pFile);
		}
		
		fwrite(&pFieldProp->_IdxBGMTbl, sizeof(RwInt32), 1, pFile);
		fwrite(&pFieldProp->_IdxEnvTbl, sizeof(RwInt32), 1, pFile);
		for (RwInt32 j = 0; j < 5; ++j)
		{
			fwrite(&pFieldProp->_IdxShareTbl[j], sizeof(RwInt32), 1, pFile);
		}

		fwrite(&pFieldProp->_MonoPower, sizeof(RwReal), 1, pFile);

		for(RwInt32 j = 0; j < dNTLPL_PLANET_MAX_CNT; ++j)
			fwrite(&pFieldProp->_NtlPlanet[j], sizeof(sNTL_PLANET), 1, pFile);

		fwrite(&pFieldProp->_NtlSpecular, sizeof(sNTL_SPECULAR), 1, pFile);
		//fwrite(pFieldProp->_pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
		fwrite(&pFieldProp->_FieldColor, sizeof(RwRGBA), 1, pFile);
		fwrite(pFieldProp->_IdxWeather, sizeof(RwInt32) * 5, 1, pFile);
		fwrite(&pFieldProp->_NtlHeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);

		// BLOCK PROPERTY
		fwrite(&pBlockProp->_GameProp, sizeof(RwUInt32), 1, pFile);
	}
	
	::fclose(pFile);

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldBlockMananger::SaveProperty(RwUInt32 iBlockIdx)
{
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);

	FILE *pFile = NULL;
	if(::fopen_s(&pFile, "wfp", "rb+") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << "wfp" << ")");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}

	sNTL_FIELD_PROP* pFieldProp = &m_pWorldFieldProp[iBlockIdx];
	sNTL_BLOCK_PROP* pBlockProp = &m_pWorldBlockProp[iBlockIdx];

	//for (int i = 0; i < iBlockIdx; ++i)
	//{
	// fieldprop
	//	GetNtlWorldFieldInfo()->FieldNameSkipToFile(pFile, m_pNtlObjectProp);
	// 	//GetNtlWorldFieldInfo()->FieldBitPropSkipToFile(pFile, m_pNtlObjectProp, iTileNumInField);
	// 	GetNtlWorldFieldInfo()->FieldFogSkipToFile(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldSkySkipToFile(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldRealLightSkipToFile(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldBGMSkipToFile(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldBloomSkipToFile(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldPlanetSkipToFile(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldSpecularSkipToFile(pFile, m_pNtlObjectProp);
	// 	//GetNtlWorldFieldInfo()->FieldWorldMaterialSkipToFile(pFile, m_pNtlObjectProp, iTileNumInField);
	// 	GetNtlWorldFieldInfo()->FieldColorSkipToFile(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldWeatherSkipToFile(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldHeatHazeSkipToFile(pFile, m_pNtlObjectProp);

	// blockprop
	// fseek(pFile, sizeof(RwUInt32), SEEK_CUR);
	//}



	RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
		sizeof(RwReal) + // new unknown sky value
		(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
		sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
		(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5) + // FIELDPROP
		sizeof(RwUInt32); // BLOCKBROP;

	::fseek(pFile, iPropertyRange * iBlockIdx, SEEK_CUR);

	// FIELD PROPERTY :: 인도어에서는 TILE PROPERTY를 사용하지 않는다.
	// 	GetNtlWorldFieldInfo()->FieldNameSaveIntoFile(pFile, pFieldProp);
	// 	//GetNtlWorldFieldInfo()->FieldBitPropSaveIntoFile(pFile, pFieldProp, iTileNumInField);
	// 	GetNtlWorldFieldInfo()->FieldFogSaveIntoFile(pFile, pFieldProp);
	// 	GetNtlWorldFieldInfo()->FieldSkySaveIntoFile(pFile, pFieldProp);
	// 	GetNtlWorldFieldInfo()->FieldRealLightSaveIntoFile(pFile, pFieldProp);
	// 	GetNtlWorldFieldInfo()->FieldBGMSaveIntoFile(pFile, pFieldProp);
	// 	GetNtlWorldFieldInfo()->FieldBloomSaveIntoFile(pFile, pFieldProp);
	// 	GetNtlWorldFieldInfo()->FieldPlanetSaveIntoFile(pFile, pFieldProp);
	// 	GetNtlWorldFieldInfo()->FieldSpecularSaveIntoFile(pFile, pFieldProp);
	// 	//GetNtlWorldFieldInfo()->FieldWorldMaterialSaveIntoFile(pFile, pFieldProp, iTileNumInField);
	// 	GetNtlWorldFieldInfo()->FieldColorSaveIntoFile(pFile, pFieldProp);
	// 	GetNtlWorldFieldInfo()->FieldWeatherSaveIntoFile(pFile, pFieldProp);
	// 	GetNtlWorldFieldInfo()->FieldHeatHazeSaveIntoFile(pFile, pFieldProp);
	fwrite(pFieldProp->_Name, 64, 1, pFile);
	//fwrite(pFieldProp->_pBitProp, sizeof(DWORD) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
	fwrite(&pFieldProp->_FogColor, sizeof(RwRGBA), 1, pFile);
	fwrite(&pFieldProp->_FogCamPlane[0], sizeof(RwReal), 1, pFile);
	fwrite(&pFieldProp->_FogCamPlane[1], sizeof(RwReal), 1, pFile);
	fwrite(&pFieldProp->_BaseSkyMode, sizeof(RwInt32), 1, pFile);
	fwrite(&pFieldProp->_RGBSkyColor[0], sizeof(RwRGBA), 1, pFile);
	fwrite(&pFieldProp->_RGBSkyColor[1], sizeof(RwRGBA), 1, pFile);
	fwrite(&pFieldProp->_RGBSkyColor[2], sizeof(RwRGBA), 1, pFile);

	fwrite(&pFieldProp->_NewSkyValue, sizeof(RwReal), 1, pFile);

	fwrite(pFieldProp->_BaseSkyTexName, sizeof(RwChar) * 10, 1, pFile);
	fwrite(&pFieldProp->_BaseSkySpeed, sizeof(RwReal), 1, pFile);
	fwrite(pFieldProp->_BlendedTexName[0], sizeof(RwChar) * 10, 1, pFile);
	fwrite(pFieldProp->_BlendedTexName[1], sizeof(RwChar) * 10, 1, pFile);
	fwrite(&pFieldProp->_BlendedTexSpeed[0], sizeof(RwReal), 1, pFile);
	fwrite(&pFieldProp->_BlendedTexSpeed[1], sizeof(RwReal), 1, pFile);
	fwrite(&pFieldProp->_ClrLightAmbient, sizeof(RwRGBAReal), 1, pFile);

	for(RwInt32 i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
	{
		fwrite(&pFieldProp->_ClrLightDirectional[i], sizeof(RwRGBAReal), 1, pFile);
		fwrite(&pFieldProp->_AngLightDirectional[i], sizeof(RwV3d), 1, pFile);
	}

	fwrite(&pFieldProp->_IdxBGMTbl, sizeof(RwInt32), 1, pFile);
	fwrite(&pFieldProp->_IdxEnvTbl, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < 5; ++i)
	{
		fwrite(&pFieldProp->_IdxShareTbl[i], sizeof(RwInt32), 1, pFile);
	}

	fwrite(&pFieldProp->_MonoPower, sizeof(RwReal), 1, pFile);

	for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
		fwrite(&pFieldProp->_NtlPlanet[i], sizeof(sNTL_PLANET), 1, pFile);

	fwrite(&pFieldProp->_NtlSpecular, sizeof(sNTL_SPECULAR), 1, pFile);
	//fwrite(pFieldProp->_pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
	fwrite(&pFieldProp->_FieldColor, sizeof(RwRGBA), 1, pFile);
	fwrite(pFieldProp->_IdxWeather, sizeof(RwInt32) * 5, 1, pFile);
	fwrite(&pFieldProp->_NtlHeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);

	// BLOCK PROPERTY
	fwrite(&pBlockProp->_GameProp, sizeof(RwUInt32), 1, pFile);

	::fclose(pFile);

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RpCollisionTriangle* NtlBlockBBoxGetY(RpIntersection *pIntersection, RpWorldSector* pRpWorldSector, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void* pData)
{
	RwBBox* pBBoxBlock = (RwBBox*)pData;
	
	for (int i = 0; i < 3; ++i)
	{
		if (pRpCollisionTriangle->vertices[i]->y < pBBoxBlock->inf.y)
		{
			pBBoxBlock->inf.y = pRpCollisionTriangle->vertices[i]->y;
		}
		if (pRpCollisionTriangle->vertices[i]->y > pBBoxBlock->sup.y)
		{
			pBBoxBlock->sup.y = pRpCollisionTriangle->vertices[i]->y;
		}
	}
	return pRpCollisionTriangle;
}

RwBool CNtlWorldBlockMananger::GenBlockBBox(RwInt32 iBlockIdx)
{
	const RwBBox*	pBBoxWorld = RpWorldGetBBox(CNtlPLGlobal::m_pRpWorld);
	RwBBox*			pBBoxBlock = &m_pWorldBlockProp[iBlockIdx]._BBox;
	pBBoxBlock->inf.x = static_cast<RwReal>((iBlockIdx % dGET_WORLD_PARAM()->WorldBlockNum) * dGET_WORLD_PARAM()->WorldBlockSize - dGET_WORLD_PARAM()->WorldSizeHalf);
	pBBoxBlock->inf.y = pBBoxWorld->inf.y;
	pBBoxBlock->inf.z = static_cast<RwReal>((iBlockIdx / dGET_WORLD_PARAM()->WorldBlockNum) * dGET_WORLD_PARAM()->WorldBlockSize - dGET_WORLD_PARAM()->WorldSizeHalf);

	pBBoxBlock->sup.x = pBBoxBlock->inf.x + dGET_WORLD_PARAM()->WorldBlockSize;
	pBBoxBlock->sup.y = pBBoxWorld->sup.y;
	pBBoxBlock->sup.z = pBBoxBlock->inf.z + dGET_WORLD_PARAM()->WorldBlockSize;

	RpIntersection intersection;
	intersection.type	= rpINTERSECTBOX;
	intersection.t.box	= *pBBoxBlock;

	pBBoxBlock->inf.y = RwRealMAXVAL;
	pBBoxBlock->sup.y = RwRealMINVAL;
	if (RpCollisionWorldForAllIntersections(CNtlPLGlobal::m_pRpWorld, &intersection, NtlBlockBBoxGetY, pBBoxBlock))
	{
		return TRUE;
	}
	return FALSE;
}
