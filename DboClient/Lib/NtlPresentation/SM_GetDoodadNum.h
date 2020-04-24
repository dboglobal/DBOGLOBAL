#pragma once


#include "isectormethod.h"


class CSM_GetDoodadNum : public ISectorMethod
{
public:
	CSM_GetDoodadNum(void);
	virtual ~CSM_GetDoodadNum(void);

public:
	virtual RwBool CheckSector(BYTE* _pFileMemPos) { return TRUE; }
	virtual RwBool DestroySelfRetMap(){ return TRUE; }
	virtual	RwBool Analyze(DWORD _SectorIdx){ return TRUE; }
	virtual	VOID*  GetAnalysis() { return NULL; }
	virtual RwBool Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI){ return TRUE; }
	virtual RwBool Report(){ return TRUE; }
};
