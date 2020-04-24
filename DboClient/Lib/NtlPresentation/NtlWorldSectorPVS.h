#pragma once


#include "NtlWorldCommon.h"
#include "NtlWorldFileMemAccessor.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

#define dPVS_LINE_TEST_EXTENT	(static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize) * 2.0f)
#define dPVS_SECTOR_EXTENT_CNT	(6)
#define dPVS_TOT_CELL_CNT		((dPVS_SECTOR_EXTENT_CNT + dPVS_SECTOR_EXTENT_CNT + 1) * (dPVS_SECTOR_EXTENT_CNT + dPVS_SECTOR_EXTENT_CNT + 1))
#define dPVS_LAYER_CNT			(6)

class CNtlWorldBrush;

class CNtlWorldSectorPVS
{
public:
	CNtlWorldSectorPVS(void);
	virtual ~CNtlWorldSectorPVS(void);

protected:
	// sector PVS visibility
	RwBool m_Visibility;

	// RB -> LT order
	BYTE* m_pVisibilityNeighbor;

private:
	RwBool TestPVS(RwInt32 _SrcSecIdx, RwInt32 _DstSecIdx, RwInt32 _CurArrayIdx, RwReal fEpsilsonHeight = 10.0f/*CharHeight*/);

public:
	static RwImage*	m_pPVSAreaFilter;
	static RwInt32	m_LockedPVSSector;

public:
	static VOID		SetPVSAreaFilter(RwImage* _pPVSAreaFilter);
	static RwInt32	GetPVSAreaTotNum();
	static RwBool	GetPVSAreaSectorArray(vector<RwInt32>& _vecPVSAreaSectorArray);
	static VOID		RenderLockingGuideLine();

public:
	RwBool	GetVisibility() { return m_Visibility; }
	VOID	SetVisibility(RwInt32 _SectorIdx, RwBool _Visible);
	RwBool	GetEnable() { return (m_pVisibilityNeighbor ? TRUE : FALSE); }
	VOID	SetVisibilityNeighbor(BYTE* _VisibilityNeighbor);
	RwInt32	GetVisibilityNeighbor(RwInt32 _NeighborIdx, RwInt32 _iLayer = 0);
	RwInt32 GetPVSAppliedNeighborSectorNum(RwInt32 _CurSectorIdx, RwInt32 _iLayer = 0);
	BYTE*	GetVisibilityNeighbor() { return m_pVisibilityNeighbor; }

	VOID	CreatePVS(RwInt32 _SectorIdx, RwReal fEpsilsonHeight = 10.0f/*CharHeight*/);
	VOID	DestroyPVS(RwInt32 _SectorIdx);
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

#define dPVS_LINE_TEST_EXTENT	(static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize) * 2.0f)
#define dPVS_SECTOR_EXTENT_CNT	(6)
#define dPVS_TOT_CELL_CNT		((dPVS_SECTOR_EXTENT_CNT + dPVS_SECTOR_EXTENT_CNT + 1) * (dPVS_SECTOR_EXTENT_CNT + dPVS_SECTOR_EXTENT_CNT + 1))
#define dPVS_LAYER_CNT			(6)

class CNtlWorldBrush;


class CNtlWorldSectorPVS : public CNtlWorldFileMemAccessor
{
public:
	CNtlWorldSectorPVS(void);
	virtual ~CNtlWorldSectorPVS(void);

protected:
	// sector PVS visibility
	RwBool m_Visibility;

	// RB -> LT order
	BYTE* m_pVisibilityNeighbor;

private:
	RwBool TestPVS(RwInt32 _SrcSecIdx, RwInt32 _DstSecIdx, RwInt32 _CurArrayIdx, RwReal fEpsilsonHeight = 10.0f/*CharHeight*/);

public:
	static RwImage*	m_pPVSAreaFilter;
	static RwInt32	m_LockedPVSSector;

public:
	static VOID		SetPVSAreaFilter(RwImage* _pPVSAreaFilter);
	static RwInt32	GetPVSAreaTotNum();
	static RwBool	GetPVSAreaSectorArray(vector<RwInt32>& _vecPVSAreaSectorArray);
	static VOID		RenderLockingGuideLine();

public:
	RwBool	GetVisibility() { return m_Visibility; }
	VOID	SetVisibility(RwInt32 _SectorIdx, RwBool _Visible);
	RwBool	GetEnable() { return (m_pVisibilityNeighbor ? TRUE : FALSE); }
	VOID	SetVisibilityNeighbor(BYTE* _VisibilityNeighbor);
	RwInt32	GetVisibilityNeighbor(RwInt32 _NeighborIdx, RwInt32 _iLayer = 0);
	RwInt32 GetPVSAppliedNeighborSectorNum(RwInt32 _CurSectorIdx, RwInt32 _iLayer = 0);
	BYTE*	GetVisibilityNeighbor() { return m_pVisibilityNeighbor; }

	VOID	CreatePVS(RwInt32 _SectorIdx, RwReal fEpsilsonHeight = 10.0f/*CharHeight*/);
	VOID	DestroyPVS(RwInt32 _SectorIdx);
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif