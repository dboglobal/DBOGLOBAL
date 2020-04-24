#pragma once


#include "isectormethod.h"


class CSM_GetAABB : public ISectorMethod
{
public:
	CSM_GetAABB(void);
	virtual ~CSM_GetAABB(void);

public:
	virtual RwBool CheckSector(BYTE* _pFileMemPos) { return TRUE; }
	virtual RwBool DestroySelfRetMap(){ return TRUE; }
	virtual	RwBool Analyze(DWORD _SectorIdx){ return TRUE; }
	virtual	VOID*  GetAnalysis() { return NULL; }
	virtual RwBool Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI){ return TRUE; }
	virtual RwBool Report(){ return TRUE; }
};
