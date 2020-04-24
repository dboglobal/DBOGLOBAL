#pragma once


#include "NtlWorldFileMemAccessor.h"

using std::map;
#include <map>

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

enum eSECTOR_METHOD_EXECUTION_TYPE_INDEX
{
	eSMETI_SECTOR,
	eSMETI_FIELD,
};


class ISectorMethod
{
public:
	typedef std::map<DWORD, VOID*>				tSELF_RET_MAP;
	typedef std::map<DWORD, VOID*>::iterator	tSELF_RET_MAP_ITER;
	typedef std::map<DWORD, VOID*>::value_type	tSELF_VALUE_TYPE;

public:
	ISectorMethod(void);
	virtual ~ISectorMethod(void);

public:
	tSELF_RET_MAP m_SelfRetMap;

public:
	// is a sector satisfied with method condition or available to apply method?
	virtual RwBool CheckSector(BYTE* _pFileMemPos) = 0;

	// destroy m_SelfRetMap;
	virtual RwBool DestroySelfRetMap() = 0;

	// analyze a sector to 
	virtual	RwBool Analyze(DWORD _SectorIdx) = 0;

	// get a analysis; be available right after analyzing
	virtual	VOID* GetAnalysis() = 0;

	// run method and return feedback to m_SelfRetMap
	virtual RwBool Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI) = 0;

	// print, display; there could be some proper output for each method
	virtual RwBool Report() = 0;
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

enum eSECTOR_METHOD_EXECUTION_TYPE_INDEX
{
	eSMETI_SECTOR,
	eSMETI_FIELD,
};


class ISectorMethod : public CNtlWorldFileMemAccessor
{
public:
	typedef std::map<DWORD, VOID*>				tSELF_RET_MAP;
	typedef std::map<DWORD, VOID*>::iterator	tSELF_RET_MAP_ITER;
	typedef std::map<DWORD, VOID*>::value_type	tSELF_VALUE_TYPE;

public:
	ISectorMethod(void);
	virtual ~ISectorMethod(void);

public:
	tSELF_RET_MAP m_SelfRetMap;

public:
	// is a sector satisfied with method condition or available to apply method?
	virtual RwBool CheckSector(BYTE* _pFileMemPos) = 0;

	// destroy m_SelfRetMap;
	virtual RwBool DestroySelfRetMap() = 0;

	// analyze a sector to 
	virtual	RwBool Analyze(DWORD _SectorIdx) = 0;

	// get a analysis; be available right after analyzing
	virtual	VOID* GetAnalysis() = 0;

	// run method and return feedback to m_SelfRetMap
	virtual RwBool Execute(eSECTOR_METHOD_EXECUTION_TYPE_INDEX _SMETI) = 0;

	// print, display; there could be some proper output for each method
	virtual RwBool Report() = 0;
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif