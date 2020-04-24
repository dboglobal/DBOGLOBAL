#include "precomp_ntlpresentation.h"
#include "ntlworldcommon.h"
#include "ntlworldslmanger.h"
#include "NtlWorldSector.h"


CNtlWorldSLManger g_NtlWorldSLMangager;


CNtlWorldSLManger::CNtlWorldSLManger()
{
	sNTL_WORLD_SL NtlWorldSL;
	NtlWorldSL.m_Softness		= 10.0f;
	NtlWorldSL.m_Brightness[0]	= 0.1f;
	NtlWorldSL.m_Brightness[1]	= 1.0f;
	NtlWorldSL.m_Dir			= 3;
	NtlWorldSL._Clr.alpha		= 1.0f;
	NtlWorldSL._Clr.red			= 0.0f;
	NtlWorldSL._Clr.green		= 0.0f;
	NtlWorldSL._Clr.blue		= 0.0f;
	SetAttr(NtlWorldSL);
}

CNtlWorldSLManger::~CNtlWorldSLManger(void)
{
}

float CNtlWorldSLManger::GetHeightValueFromFile(float PosX, float PosZ, FILE* pFile)
{
	RwUInt8 Value[3];
	RwInt32 NPosX = static_cast<RwInt32>(PosX);
	RwInt32 NPosZ = static_cast<RwInt32>(PosZ);
	RwInt32 XBasedOn0 = NPosX + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwInt32 ZBasedOn0 = NPosZ + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwInt32 WidthNum = dGET_WORLD_PARAM()->WorldHeightfieldNum - XBasedOn0 / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32 DepthNum = dGET_WORLD_PARAM()->WorldHeightfieldNum - ZBasedOn0 / dGET_WORLD_PARAM()->WorldSectorTileSize;
	fseek(pFile, (sizeof(RwUInt8) * 3) * (WidthNum + DepthNum * (dGET_WORLD_PARAM()->WorldHeightfieldNum + 1)), SEEK_SET);
	fread(&Value, sizeof(RwUInt8), 3, pFile);

	return (float)(Value[0] + Value[1] + Value[2]) / 3.0f  - 128.0f;
}

void CNtlWorldSLManger::SetAttr(sNTL_WORLD_SL& NtlWorldSL)
{
	m_Attr = NtlWorldSL;
}

bool CNtlWorldSLManger::SetVert(RwRGBA& FResult, DWORD* pVertPaintMap, DWORD* pVertShadowMap, float PosX, float PosZ, FILE* pFile)
{
	if(!pFile)
		return false;

	if(	(PosX < dGET_WORLD_PARAM()->WorldValueMin && PosX > dGET_WORLD_PARAM()->WorldValueMax) ||
		(PosZ < dGET_WORLD_PARAM()->WorldValueMin && PosZ > dGET_WORLD_PARAM()->WorldValueMax))
		return false;

	float LightPos0, LightPos1, Result;
	float TileSize = (float)dGET_WORLD_PARAM()->WorldSectorTileSize;

	//
	// 3---2---1
	// |       | 
	// 4  cur  0
	// |       |
	// 5---6---7
	// 

	if(m_Attr.m_Dir == 0)
		LightPos0 = GetHeightValueFromFile(PosX - TileSize, PosZ, pFile);
	else if(m_Attr.m_Dir == 1)
		LightPos0 = GetHeightValueFromFile(PosX - TileSize, PosZ + TileSize, pFile);
	else if(m_Attr.m_Dir == 2)
		LightPos0 = GetHeightValueFromFile(PosX, PosZ + TileSize, pFile);
	else if(m_Attr.m_Dir == 3)
		LightPos0 = GetHeightValueFromFile(PosX + TileSize, PosZ + TileSize, pFile);
	else if(m_Attr.m_Dir == 4)
		LightPos0 = GetHeightValueFromFile(PosX + TileSize, PosZ, pFile);
	else if(m_Attr.m_Dir == 5)
		LightPos0 = GetHeightValueFromFile(PosX + TileSize, PosZ - TileSize, pFile);
	else if(m_Attr.m_Dir == 6)
		LightPos0 = GetHeightValueFromFile(PosX, PosZ - TileSize, pFile);
	else if(m_Attr.m_Dir == 7)
		LightPos0 = GetHeightValueFromFile(PosX - TileSize, PosZ - TileSize, pFile);

	LightPos1 = GetHeightValueFromFile(PosX, PosZ, pFile);
	Result = (1.0f - (LightPos0 - LightPos1) / m_Attr.m_Softness);
	CLAMP(Result, m_Attr.m_Brightness[0], m_Attr.m_Brightness[1]);
	
	RwUInt8 ClrCurR = (RwUInt8)((*pVertPaintMap & 0x00ff0000) >> 16);
	RwUInt8 ClrCurG = (RwUInt8)((*pVertPaintMap & 0x0000ff00) >> 8);
	RwUInt8 ClrCurB = (RwUInt8)((*pVertPaintMap & 0x000000ff) >> 0);
	RwUInt8 ClrCurTmp[3];

	float SlopeLitClr = Result * 255.0f;

	ClrCurTmp[0] = (RwUInt8)(((SlopeLitClr / 255.0f) * ((RwReal)ClrCurR / 255.0f)) * 255.0f);
	ClrCurTmp[1] = (RwUInt8)(((SlopeLitClr / 255.0f) * ((RwReal)ClrCurG / 255.0f)) * 255.0f);
	ClrCurTmp[2] = (RwUInt8)(((SlopeLitClr / 255.0f) * ((RwReal)ClrCurB / 255.0f)) * 255.0f);

	/*
	// vertex shadow
	RwUInt8 ClrCurVSR, ClrCurVSG, ClrCurVSB;
	if((*pVertShadowMap & 0xff000000) >> 24 == 0x000000ff)
	{
		ClrCurVSR = (RwUInt8)((*pVertShadowMap & 0x00ff0000) >> 16);
		ClrCurVSG = (RwUInt8)((*pVertShadowMap & 0x0000ff00) >> 8);
		ClrCurVSB = (RwUInt8)((*pVertShadowMap & 0x000000ff) >> 0);
		ClrCurTmp[0] = (RwUInt8)((((RwReal)ClrCurTmp[0] / 255.0f) * ((RwReal)ClrCurVSR / 255.0f)) * 255.0f);
		ClrCurTmp[1] = (RwUInt8)((((RwReal)ClrCurTmp[1] / 255.0f) * ((RwReal)ClrCurVSG / 255.0f)) * 255.0f);
		ClrCurTmp[2] = (RwUInt8)((((RwReal)ClrCurTmp[2] / 255.0f) * ((RwReal)ClrCurVSB / 255.0f)) * 255.0f);

	}*/

	FResult.red		= (RwUInt8)(CLAMP(ClrCurTmp[0], 0, 255));
	FResult.green	= (RwUInt8)(CLAMP(ClrCurTmp[1], 0, 255));
	FResult.blue	= (RwUInt8)(CLAMP(ClrCurTmp[2], 0, 255));
	FResult.alpha	= 255;
	
	return true;
}