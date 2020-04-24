#pragma once


#define dMAX_BLEND_DENSITY (99.0f)


#include "ISectorMethod.h"
#include "ntlworldcommon.h"


class CSM_SplatTexMaxBlend_RetContainer
{
public:
	RwReal m_AverageAlphaDensity[dNTL_SPLAT_LAYER_NUM];
};


class CSM_SplatTexMaxBlend : public ISectorMethod
{
public:
	CSM_SplatTexMaxBlend(void);
	virtual ~CSM_SplatTexMaxBlend(void);

protected:
	CSM_SplatTexMaxBlend_RetContainer m_RetContainer;

public:
	virtual RwBool CheckSector(BYTE* _pFileMemPos); 
	virtual RwBool DestroySelfRetMap();
	virtual	RwBool Analyze(DWORD _SectorIdx);
	virtual	VOID*  GetAnalysis();
	virtual RwBool Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI);
	virtual RwBool Report();

	VOID RemoveSplatTex(RwInt32 _FieldIdx, RwInt32 _SectorIdx, RwInt32 _StartIdx2Del);
};
