#pragma once


#define dMIN_BLEND_DENSITY (0.1f)


#include "isectormethod.h"
#include "ntlworldcommon.h"


class CSM_SplatTexMinBlend_RetContainer
{
public:
	RwReal m_AverageAlphaDensity[dNTL_SPLAT_LAYER_NUM];
};


class CSM_SplatTexMinBlend : public ISectorMethod
{
public:
	CSM_SplatTexMinBlend(void);
	virtual ~CSM_SplatTexMinBlend(void);

protected:
	CSM_SplatTexMinBlend_RetContainer m_RetContainer;

public:
	virtual RwBool CheckSector(BYTE* _pFileMemPos);
	virtual RwBool DestroySelfRetMap();
	virtual	RwBool Analyze(DWORD _SectorIdx);
	virtual	VOID*  GetAnalysis();
	virtual RwBool Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI);
	virtual RwBool Report();

	VOID RemoveSplatTex(RwInt32 _FieldIdx, RwInt32 _SectorIdx, RwBool _Indices2Del[dNTL_SPLAT_LAYER_NUM]);
};
