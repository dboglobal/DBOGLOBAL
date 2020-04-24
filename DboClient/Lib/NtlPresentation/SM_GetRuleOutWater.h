#pragma once

#include "isectormethod.h"

class CSM_GetRuleOutWater_RetContainer
{
public:
	enum eRULEOUT_TYPE
	{
		eRULEOUT_TYPE_NONE	= 0x0,
		eRULEOUT_TYPE_ALPHA	= 0x1,
		eRULEOUT_TYPE_LOWER	= 0x2,		
	};

	RwUInt32	eRuleOutType;
	RwV3d		vTranslation;
};

class CSM_GetRuleOutWater : public ISectorMethod
{
protected:
	CSM_GetRuleOutWater_RetContainer m_RetContainer;

public:
	CSM_GetRuleOutWater(void);
	virtual ~CSM_GetRuleOutWater(void);

public:
	virtual RwBool CheckSector(BYTE* _pFileMemPos);
	virtual RwBool DestroySelfRetMap();
	virtual	RwBool Analyze(DWORD _SectorIdx);
	virtual	VOID*  GetAnalysis();
	virtual RwBool Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI);
	virtual RwBool Report();
};
