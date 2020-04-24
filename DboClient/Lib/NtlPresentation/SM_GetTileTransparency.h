#pragma once

#include "isectormethod.h"

class CSM_GetTileTransparency_RetContainer
{
public:
	RwInt32		iFieldIndex;
	RwV3d		vTranslation;
};

class CSM_GetTileTransparency : public ISectorMethod
{
protected:
	CSM_GetTileTransparency_RetContainer m_RetContainer;

public:
	CSM_GetTileTransparency(void);
	virtual ~CSM_GetTileTransparency(void);

public:
	virtual RwBool CheckSector(BYTE* _pFileMemPos);
	virtual RwBool DestroySelfRetMap();
	virtual	RwBool Analyze(DWORD _SectorIdx);
	virtual	VOID*  GetAnalysis();
	virtual RwBool Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI);
	virtual RwBool Report();
};
