#pragma once


#include "ntlworldcommon.h"
#include "FileController.h"
#include "FileMem.h"
#include "NtlWorldFileMemAccessor.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

class CNtlWorldSector;
class CFileMem;

class CNtlWorldSectorManager : public CFileController, public CFileMem
{
friend class CNtlWorldFieldManager;

public:
	CNtlWorldSectorManager(void);
	virtual ~CNtlWorldSectorManager(void);

public:
	void Init();
	void Free();

private:
	// verification
	RwBool IsSectorValid(RwInt32 Idx);
	RwBool IsSectorValid(RwV3d& Pos);

protected:
	RwBool CreateInMemory(RwInt32 SectorIdx);
	RwBool DeleteInMemory(RwInt32 SectorIdx, RwBool SaveSwapInToolMode = TRUE);
	RwBool CreateFromFile(sCUR_FIELD_TEX_INFO& FieldTexInfo, RwInt32 SectorIdx, FILE* pFile);
	RwBool DeleteIntoFile(RwInt32 SectorIdx, FILE* pFile);

	RwBool UpdateTexLyr(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, RwInt32 SectorIdx, RwInt32 IdxLyr, RwInt32 IdxMenu);
	RwBool UpdateTexMat(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, RwInt32 SectorIdx, RwInt32 IdxLyr);

public:
	void	SetWorldSector(RpWorldSector *pWorldSector);
	RwInt32 GetSectorIdx(RwV3d& Pos);
	RwBool	GetNeighborSectors(RwInt32 _IdxCurSector, vector<RwInt32>& _vecNeighborSectorIndices);
	RwBool	IsSectorLoaded(RwInt32 SectorIdx);
	RwBool	UpdateSectorMap(RwV3d& Pos);

public:
	CNtlWorldSector*		m_pSectors;

	// current sector index
	RwInt32					m_IdxCurSector;
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

class CNtlWorldSector;
class CFileMem;


class CNtlWorldSectorManager : public CFileController, public CFileMem, public CNtlWorldFileMemAccessor
{
	friend class CNtlWorldFieldManager;

public:
	CNtlWorldSectorManager(void);
	virtual ~CNtlWorldSectorManager(void);

public:
	void Init();
	void Free();

private:
	// verification
	RwBool IsSectorValid(RwInt32 Idx);
	RwBool IsSectorValid(RwV3d& Pos);

protected:
	RwBool CreateInMemory(RwInt32 SectorIdx);
	RwBool DeleteInMemory(RwInt32 SectorIdx, RwBool SaveSwapInToolMode = TRUE);
	RwBool CreateFromFile(sCUR_FIELD_TEX_INFO& FieldTexInfo, RwInt32 SectorIdx, FILE* pFile);
	RwBool DeleteIntoFile(RwInt32 SectorIdx, FILE* pFile);

	RwBool UpdateTexLyr(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, RwInt32 SectorIdx, RwInt32 IdxLyr, RwInt32 IdxMenu);
	RwBool UpdateTexMat(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, RwInt32 SectorIdx, RwInt32 IdxLyr);

public:
	void	SetWorldSector(RpWorldSector *pWorldSector);
	RwInt32 GetSectorIdx(RwV3d& Pos);
	RwBool	GetNeighborSectors(RwInt32 _IdxCurSector, vector<RwInt32>& _vecNeighborSectorIndices);
	RwBool	IsSectorLoaded(RwInt32 SectorIdx);
	RwBool	UpdateSectorMap(RwV3d& Pos);

public:
	CNtlWorldSector*	m_pSectors;

	// current sector index
	RwInt32				m_IdxCurSector;
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif