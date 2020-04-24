#include "precomp_ntlpresentation.h"

#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"

#include "NtlPLWorldState.h"

#include "NtlPLPlanet.h"
#include "NtlPLObjectType.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RWWorld.h"

CNtlPLObjectType::CNtlPLObjectType()
:m_pPLObject(NULL)
{
}

CNtlPLObjectType::~CNtlPLObjectType()
{
	m_pPLObject = NULL;
}

void CNtlPLObjectType::Create(const CNtlPLObject* pPLObject)
{
	m_pPLObject = const_cast<CNtlPLObject*>(pPLObject);
}

void CNtlPLObjectType::Destroy()
{
	m_pPLObject = NULL;
}

void CNtlPLObjectType::Update(RwReal fElapsed)
{
}

RwBool CNtlPLObjectType::LoadFromFile(FILE *pFile)
{
	return TRUE;
}

RwBool CNtlPLObjectType::SaveIntoFile(FILE *pFile)
{
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPLObjectTypeHasProperty::CNtlPLObjectTypeHasProperty()
:m_pNtlObjectProp(NULL)
,m_fObjectPropVariationRestTime(dMINI_INDOOR_EFFECT_SWITCHING_TIME)
{
}

CNtlPLObjectTypeHasProperty::~CNtlPLObjectTypeHasProperty()
{
	NTL_DELETE(m_pNtlObjectProp);
}

void CNtlPLObjectTypeHasProperty::Create(const CNtlPLObject* pPLObject)
{
	CNtlPLObjectType::Create(pPLObject);

	if (m_pNtlObjectProp == NULL)
	{
		m_pNtlObjectProp = NTL_NEW sNTL_OBJECT_PROP(FALSE);
	}
}

void CNtlPLObjectTypeHasProperty::Destroy()
{
	NTL_DELETE(m_pNtlObjectProp);

	CNtlPLObjectType::Destroy();
}

void CNtlPLObjectTypeHasProperty::Update(RwReal fElapsed)
{
	if (!m_pNtlObjectProp || CNtlPLWorldState::GetActiveMiniIndoor() != m_pPLObject)
	{
		m_fObjectPropVariationRestTime = dMINI_INDOOR_EFFECT_SWITCHING_TIME;

		return;
	}

	// WorldState에서 한번 처리 해도 될 것 같다.
	if (m_fObjectPropVariationRestTime == dMINI_INDOOR_EFFECT_SWITCHING_TIME)
	{
		// send change world space without delay
		CNtlPLEventGenerator::IsAnotherObjectChangedWithoutDelay(m_pNtlObjectProp);
	}
	
	if (m_fObjectPropVariationRestTime > 0.0f)
	{
		m_fObjectPropVariationRestTime -= fElapsed;
		if (m_fObjectPropVariationRestTime < 0.0f)
		{
			// send change world space with delay
			CNtlPLEventGenerator::IsAnotherObjectChanged(m_pNtlObjectProp);
		}
	}
}

void CNtlPLObjectTypeHasProperty::SetObjectProperty(sNTL_FIELD_PROP& sNtlFieldProp, eNTL_FIELD_PROPID eNtlFieldPropID)
{
	switch(eNtlFieldPropID)
	{
	case eNFP_NAME:
		{
			strcpy_s(m_pNtlObjectProp->_Name, sNtlFieldProp._Name);

			break;
		}
	case eNFP_BITPROP:
		{
			break;
		}
	case eNFP_FOG:
		{
			m_pNtlObjectProp->_FogColor.red		= sNtlFieldProp._FogColor.red;
			m_pNtlObjectProp->_FogColor.green	= sNtlFieldProp._FogColor.green;
			m_pNtlObjectProp->_FogColor.blue	= sNtlFieldProp._FogColor.blue;
			m_pNtlObjectProp->_FogCamPlane[0]	= sNtlFieldProp._FogCamPlane[0];
			m_pNtlObjectProp->_FogCamPlane[1]	= sNtlFieldProp._FogCamPlane[1];	

			break;
		}
	case eNFP_SKY:
		{
			m_pNtlObjectProp->_BaseSkyMode	= sNtlFieldProp._BaseSkyMode;
			memcpy(&m_pNtlObjectProp->_RGBSkyColor[0], &sNtlFieldProp._RGBSkyColor[0], sizeof(RwRGBA));
			memcpy(&m_pNtlObjectProp->_RGBSkyColor[1], &sNtlFieldProp._RGBSkyColor[1], sizeof(RwRGBA));
			memcpy(&m_pNtlObjectProp->_RGBSkyColor[2], &sNtlFieldProp._RGBSkyColor[2], sizeof(RwRGBA));

			m_pNtlObjectProp->_NewSkyValue = sNtlFieldProp._NewSkyValue; // new
	
			strcpy_s(m_pNtlObjectProp->_BaseSkyTexName, sNtlFieldProp._BaseSkyTexName);
			m_pNtlObjectProp->_BaseSkySpeed = sNtlFieldProp._BaseSkySpeed;
			strcpy_s(m_pNtlObjectProp->_BlendedTexName[0], sNtlFieldProp._BlendedTexName[0]);
			strcpy_s(m_pNtlObjectProp->_BlendedTexName[1], sNtlFieldProp._BlendedTexName[1]);
			m_pNtlObjectProp->_BlendedTexSpeed[0] = sNtlFieldProp._BlendedTexSpeed[0];
			m_pNtlObjectProp->_BlendedTexSpeed[1] = sNtlFieldProp._BlendedTexSpeed[1];

			break;
		}
	case eNFP_LIGHT:
		{
			::CopyMemory(&m_pNtlObjectProp->_ClrLightAmbient, &sNtlFieldProp._ClrLightAmbient, sizeof(RwRGBAReal));
			for (int i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
			{
				::CopyMemory(&m_pNtlObjectProp->_ClrLightDirectional[i], &sNtlFieldProp._ClrLightDirectional[i], sizeof(RwRGBAReal));
				::CopyMemory(&m_pNtlObjectProp->_AngLightDirectional[i], &sNtlFieldProp._AngLightDirectional[i], sizeof(RwV3d));
			}
			break;
		}
	case eNFP_SOUND:
		{
			m_pNtlObjectProp->_IdxBGMTbl		= sNtlFieldProp._IdxBGMTbl;
			m_pNtlObjectProp->_IdxEnvTbl		= sNtlFieldProp._IdxEnvTbl;
			for (int i = 0; i < 5; ++i)
			{
				m_pNtlObjectProp->_IdxShareTbl[i]	= sNtlFieldProp._IdxEnvTbl;
			}			
			break;
		}

	case eNFP_BLOOM:
		{
			m_pNtlObjectProp->_MonoPower = sNtlFieldProp._MonoPower;

			break;
		}
	case eNFP_PLANET:
		{
			m_pNtlObjectProp->_NtlPlanet[0] = sNtlFieldProp._NtlPlanet[0];
			m_pNtlObjectProp->_NtlPlanet[1] = sNtlFieldProp._NtlPlanet[1];
			m_pNtlObjectProp->_NtlPlanet[2] = sNtlFieldProp._NtlPlanet[2];

			break;
		}
	case eNFP_SPECULAR:
		{					
			m_pNtlObjectProp->_NtlSpecular = sNtlFieldProp._NtlSpecular;

			break;
		}
	case eNFP_MATERIALPROP:
		{
			break;
		}
	case eNFP_FIELDCOLOR:
		{
			m_pNtlObjectProp->_FieldColor = sNtlFieldProp._FieldColor;

			break;
		}
	case eNFP_WEATHER:
		{
			for (int i = 0; i < 5; ++i)
			{
				m_pNtlObjectProp->_IdxWeather[i] = sNtlFieldProp._IdxWeather[i];
			}

			break;
		}			
	case eNFP_FIELDHEATHAZE:
		{
			m_pNtlObjectProp->_NtlHeatHaze = sNtlFieldProp._NtlHeatHaze;

			break;
		}
	}

	CNtlPLWorldState::InitSpace();
}

RwBool CNtlPLObjectTypeHasProperty::SetObjectProperty(sNTL_FIELD_PROP& sNtlFieldProp)
{	
	if (m_pNtlObjectProp)
	{
		*m_pNtlObjectProp = sNtlFieldProp;

		return TRUE;
	}
	return FALSE;
}

RwBool CNtlPLObjectTypeHasProperty::GetObjectProperty(sNTL_FIELD_PROP& sNtlFieldProp)
{	
	if (m_pNtlObjectProp)
	{
		sNtlFieldProp = *m_pNtlObjectProp;
		
		return TRUE;
	}
	return FALSE;
}

RwBool CNtlPLObjectTypeHasProperty::LoadFromFile(FILE *pFile)
{
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

	fread(m_pNtlObjectProp->_Name, 64, 1, pFile);
	//fread(pFieldProp->_pBitProp, sizeof(DWORD) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
	fread(&m_pNtlObjectProp->_FogColor, sizeof(RwRGBA), 1, pFile);
	fread(&m_pNtlObjectProp->_FogCamPlane[0], sizeof(RwReal), 1, pFile);
	fread(&m_pNtlObjectProp->_FogCamPlane[1], sizeof(RwReal), 1, pFile);
	fread(&m_pNtlObjectProp->_BaseSkyMode, sizeof(RwInt32), 1, pFile);
	fread(&m_pNtlObjectProp->_RGBSkyColor[0], sizeof(RwRGBA), 1, pFile);
	fread(&m_pNtlObjectProp->_RGBSkyColor[1], sizeof(RwRGBA), 1, pFile);
	fread(&m_pNtlObjectProp->_RGBSkyColor[2], sizeof(RwRGBA), 1, pFile);

	fread(&m_pNtlObjectProp->_NewSkyValue, sizeof(RwReal), 1, pFile);

	fread(m_pNtlObjectProp->_BaseSkyTexName, sizeof(RwChar) * 10, 1, pFile);
	fread(&m_pNtlObjectProp->_BaseSkySpeed, sizeof(RwReal), 1, pFile);
	fread(m_pNtlObjectProp->_BlendedTexName[0], sizeof(RwChar) * 10, 1, pFile);
	fread(m_pNtlObjectProp->_BlendedTexName[1], sizeof(RwChar) * 10, 1, pFile);
	fread(&m_pNtlObjectProp->_BlendedTexSpeed[0], sizeof(RwReal), 1, pFile);
	fread(&m_pNtlObjectProp->_BlendedTexSpeed[1], sizeof(RwReal), 1, pFile);
	fread(&m_pNtlObjectProp->_ClrLightAmbient, sizeof(RwRGBAReal), 1, pFile);

	for(RwInt32 j = 0; j < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++j)
	{
		fread(&m_pNtlObjectProp->_ClrLightDirectional[j], sizeof(RwRGBAReal), 1, pFile);
		fread(&m_pNtlObjectProp->_AngLightDirectional[j], sizeof(RwV3d), 1, pFile);
		//break;
	}

	fread(&m_pNtlObjectProp->_IdxBGMTbl, sizeof(RwInt32), 1, pFile);
	fread(&m_pNtlObjectProp->_IdxEnvTbl, sizeof(RwInt32), 1, pFile);
	for (RwInt32 j = 0; j < 5; ++j)
	{
		fread(&m_pNtlObjectProp->_IdxShareTbl[j], sizeof(RwInt32), 1, pFile);
	}

	fread(&m_pNtlObjectProp->_MonoPower, sizeof(RwReal), 1, pFile);

	for(RwInt32 j = 0; j < dNTLPL_PLANET_MAX_CNT; ++j)
		fread(&m_pNtlObjectProp->_NtlPlanet[j], sizeof(sNTL_PLANET), 1, pFile);

	fread(&m_pNtlObjectProp->_NtlSpecular, sizeof(sNTL_SPECULAR), 1, pFile);
	//fread(pFieldProp->_pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
	fread(&m_pNtlObjectProp->_FieldColor, sizeof(RwRGBA), 1, pFile);
	fread(m_pNtlObjectProp->_IdxWeather, sizeof(RwInt32) * 5, 1, pFile);
	fread(&m_pNtlObjectProp->_NtlHeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);

	return TRUE;
}

RwBool CNtlPLObjectTypeHasProperty::SaveIntoFile(FILE *pFile)
{
// 	GetNtlWorldFieldInfo()->FieldNameSaveIntoFile(pFile, m_pNtlObjectProp);
// 	//GetNtlWorldFieldInfo()->FieldBitPropSaveIntoFile(pFile, m_pNtlObjectProp, iTileNumInField);
// 	GetNtlWorldFieldInfo()->FieldFogSaveIntoFile(pFile, m_pNtlObjectProp);
// 	GetNtlWorldFieldInfo()->FieldSkySaveIntoFile(pFile, m_pNtlObjectProp);
// 	GetNtlWorldFieldInfo()->FieldRealLightSaveIntoFile(pFile, m_pNtlObjectProp);
// 	GetNtlWorldFieldInfo()->FieldBGMSaveIntoFile(pFile, m_pNtlObjectProp);
// 	GetNtlWorldFieldInfo()->FieldBloomSaveIntoFile(pFile, m_pNtlObjectProp);
// 	GetNtlWorldFieldInfo()->FieldPlanetSaveIntoFile(pFile, m_pNtlObjectProp);
// 	GetNtlWorldFieldInfo()->FieldSpecularSaveIntoFile(pFile, m_pNtlObjectProp);
// 	//GetNtlWorldFieldInfo()->FieldWorldMaterialSaveIntoFile(pFile, m_pNtlObjectProp, iTileNumInField);
// 	GetNtlWorldFieldInfo()->FieldColorSaveIntoFile(pFile, m_pNtlObjectProp);
// 	GetNtlWorldFieldInfo()->FieldWeatherSaveIntoFile(pFile, m_pNtlObjectProp);
// 	GetNtlWorldFieldInfo()->FieldHeatHazeSaveIntoFile(pFile, m_pNtlObjectProp);

	fwrite(m_pNtlObjectProp->_Name, 64, 1, pFile);
	//fwrite(pFieldProp->_pBitProp, sizeof(DWORD) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
	fwrite(&m_pNtlObjectProp->_FogColor, sizeof(RwRGBA), 1, pFile);
	fwrite(&m_pNtlObjectProp->_FogCamPlane[0], sizeof(RwReal), 1, pFile);
	fwrite(&m_pNtlObjectProp->_FogCamPlane[1], sizeof(RwReal), 1, pFile);
	fwrite(&m_pNtlObjectProp->_BaseSkyMode, sizeof(RwInt32), 1, pFile);
	fwrite(&m_pNtlObjectProp->_RGBSkyColor[0], sizeof(RwRGBA), 1, pFile);
	fwrite(&m_pNtlObjectProp->_RGBSkyColor[1], sizeof(RwRGBA), 1, pFile);
	fwrite(&m_pNtlObjectProp->_RGBSkyColor[2], sizeof(RwRGBA), 1, pFile);

	fwrite(&m_pNtlObjectProp->_NewSkyValue, sizeof(RwReal), 1, pFile);

	fwrite(m_pNtlObjectProp->_BaseSkyTexName, sizeof(RwChar) * 10, 1, pFile);
	fwrite(&m_pNtlObjectProp->_BaseSkySpeed, sizeof(RwReal), 1, pFile);
	fwrite(m_pNtlObjectProp->_BlendedTexName[0], sizeof(RwChar) * 10, 1, pFile);
	fwrite(m_pNtlObjectProp->_BlendedTexName[1], sizeof(RwChar) * 10, 1, pFile);
	fwrite(&m_pNtlObjectProp->_BlendedTexSpeed[0], sizeof(RwReal), 1, pFile);
	fwrite(&m_pNtlObjectProp->_BlendedTexSpeed[1], sizeof(RwReal), 1, pFile);
	fwrite(&m_pNtlObjectProp->_ClrLightAmbient, sizeof(RwRGBAReal), 1, pFile);

	for(RwInt32 j = 0; j < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++j)
	{
		fwrite(&m_pNtlObjectProp->_ClrLightDirectional[j], sizeof(RwRGBAReal), 1, pFile);
		fwrite(&m_pNtlObjectProp->_AngLightDirectional[j], sizeof(RwV3d), 1, pFile);
	}

	fwrite(&m_pNtlObjectProp->_IdxBGMTbl, sizeof(RwInt32), 1, pFile);
	fwrite(&m_pNtlObjectProp->_IdxEnvTbl, sizeof(RwInt32), 1, pFile);
	for (RwInt32 j = 0; j < 5; ++j)
	{
		fwrite(&m_pNtlObjectProp->_IdxShareTbl[j], sizeof(RwInt32), 1, pFile);
	}

	fwrite(&m_pNtlObjectProp->_MonoPower, sizeof(RwReal), 1, pFile);

	for(RwInt32 j = 0; j < dNTLPL_PLANET_MAX_CNT; ++j)
		fwrite(&m_pNtlObjectProp->_NtlPlanet[j], sizeof(sNTL_PLANET), 1, pFile);

	fwrite(&m_pNtlObjectProp->_NtlSpecular, sizeof(sNTL_SPECULAR), 1, pFile);
	//fwrite(pFieldProp->_pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2), 1, pFile);
	fwrite(&m_pNtlObjectProp->_FieldColor, sizeof(RwRGBA), 1, pFile);
	fwrite(m_pNtlObjectProp->_IdxWeather, sizeof(RwInt32) * 5, 1, pFile);
	fwrite(&m_pNtlObjectProp->_NtlHeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);

	return TRUE;
}

RwInt32 CNtlPLObjectTypeHasProperty::SkipToFile(FILE* pFile)
{
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
	RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
		sizeof(RwReal) + //newsky value
		(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
		sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
		(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

	::fseek(pFile, iPropertyRange, SEEK_CUR);

	return ftell(pFile);
}

BYTE* CNtlPLObjectTypeHasProperty::SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	//	GetNtlWorldFieldInfo()->FieldNameSaveIntoFileFromFileMem(pFile, m_pNtlObjectProp);
	// 	//GetNtlWorldFieldInfo()->FieldBitPropSaveIntoFileFromFileMem(pFile, m_pNtlObjectProp, iTileNumInField);
	// 	GetNtlWorldFieldInfo()->FieldFogSaveIntoFileFromFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldSkySaveIntoFileFromFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldRealLightSaveIntoFileFromFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldBGMSaveIntoFileFromFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldBloomSaveIntoFileFromFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldPlanetSaveIntoFileFromFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldSpecularSaveIntoFileFromFileMem(pFile, m_pNtlObjectProp);
	// 	//GetNtlWorldFieldInfo()->FieldWorldMaterialSaveIntoFileFromFileMem(pFile, m_pNtlObjectProp, iTileNumInField);
	// 	GetNtlWorldFieldInfo()->FieldColorSaveIntoFileFromFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldWeatherSaveIntoFileFromFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldHeatHazeSaveIntoFileFromFileMem(pFile, m_pNtlObjectProp);
	RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
		sizeof(RwReal) + //newsky value
		(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
		sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
		(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

	::fwrite(pFileMem, iPropertyRange, 1, pFile);
	pFileMem += iPropertyRange;

	return pFileMem;
}

BYTE* CNtlPLObjectTypeHasProperty::SkipToFileMem(BYTE* pFileMem)
{
	//	GetNtlWorldFieldInfo()->FieldNameSkipToFileMem(pFile, m_pNtlObjectProp);
	// 	//GetNtlWorldFieldInfo()->FieldBitPropSkipToFileMem(pFile, m_pNtlObjectProp, iTileNumInField);
	// 	GetNtlWorldFieldInfo()->FieldFogSkipToFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldSkySkipToFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldRealLightSkipToFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldBGMSkipToFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldBloomSkipToFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldPlanetSkipToFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldSpecularSkipToFileMem(pFile, m_pNtlObjectProp);
	// 	//GetNtlWorldFieldInfo()->FieldWorldMaterialSkipToFileMem(pFile, m_pNtlObjectProp, iTileNumInField);
	// 	GetNtlWorldFieldInfo()->FieldColorSkipToFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldWeatherSkipToFileMem(pFile, m_pNtlObjectProp);
	// 	GetNtlWorldFieldInfo()->FieldHeatHazeSkipToFileMem(pFile, m_pNt
	RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
		sizeof(RwReal) + //newsky value
		(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
		sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
		(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

	pFileMem += iPropertyRange;

	return pFileMem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPLObjectType_MiniIndoor_Close::CNtlPLObjectType_MiniIndoor_Close()
{
}

CNtlPLObjectType_MiniIndoor_Close::~CNtlPLObjectType_MiniIndoor_Close()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPLObjectType_MiniIndoor_Open::CNtlPLObjectType_MiniIndoor_Open()
{
}

CNtlPLObjectType_MiniIndoor_Open::~CNtlPLObjectType_MiniIndoor_Open()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPLObjectType_MiniIndoor_Object::CNtlPLObjectType_MiniIndoor_Object()
{
}

CNtlPLObjectType_MiniIndoor_Object::~CNtlPLObjectType_MiniIndoor_Object()
{
}