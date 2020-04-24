////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include "NtlDebug.h"
using std::vector;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum CHUNK_DIRECTION
{
	CD_TOP = 0,
	CD_LEFT,
	CD_RIGHT,
	CD_BOTTOM,
	CD_TOT_CNT
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class IndexType> class StaticIB;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct IDX_MEM
{
public:
	IDX_MEM();
	~IDX_MEM();

private:
	int _MemPos;
	WORD* _pDat;

public:
	void Create(int Cnt);
	void Append(WORD Value);
	const int GetIdxCnt() const;
	const WORD* GetDatPtr() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class gbGenLODIdxGrp
{
public:
	gbGenLODIdxGrp(bool LeftHanded);
	~gbGenLODIdxGrp();

private:
	IDX_MEM* _pCBody;
	IDX_MEM* _pEBody;
	IDX_MEM* _pELink[CD_TOT_CNT];
	int _Lvl;
	bool _LeftHanded;

public:
	void CreateCBody();
	void CreateEBody();
	void CreateELink();
	void AppendCBody(WORD Value);
	void AppendEBody(WORD Value, CHUNK_DIRECTION Dir);
	void AppendELink(WORD Value, CHUNK_DIRECTION Dir, int Lvl);
	void AppendCBodyIndices(WORD I1, WORD I2, WORD I3);
	void AppendEBodyIndices(WORD I1, WORD I2, WORD I3, CHUNK_DIRECTION Dir);
	void AppendELinkIndices(WORD I1, WORD I2, WORD I3, CHUNK_DIRECTION Dir, int Lvl);
	void AppendELinkIndices(WORD Indices[3], CHUNK_DIRECTION Dir, int Lvl);
	
	void SetLvl(int Lvl);
	int GetLvl() const;
	IDX_MEM* GetCBody() const;
	IDX_MEM* GetEBody() const;
	IDX_MEM* GetELink(CHUNK_DIRECTION Dir) const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MM_PT
{
	int x, z;
};

struct MM_FPT
{
	float x, z;
};

struct MM_REGION
{
public:
	float _XMin, _XMax, _ZMin, _ZMax;

public:
	void SetRgn(float XMin, float XMax, float ZMin, float ZMax);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class gbGenLODIdx	
{
public:
	gbGenLODIdx(int TileSize, int TileCnt, bool LeftHanded);
	virtual ~gbGenLODIdx();

private:
	int _TileSize, _TileCnt, _PtCnt;	
	int _MeshSize, _MeshSizeHalf;
	int _CurLvl;
	int _TotLvlCnt;
	bool _LeftHanded;
	D3DXVECTOR3 _SPos, _EPos;
	vector<gbGenLODIdxGrp*> _pDatList;   
	IDX_MEM* _pBodies;
	IDX_MEM* _pConnectors[CD_TOT_CNT];

private:
	bool Analyze();
	void AppendELinkX20(gbGenLODIdxGrp* pELink);
	void AppendELinkX2X(gbGenLODIdxGrp* pELink, int XLvl);
	void AppendELink(gbGenLODIdxGrp* pELink);
	void AppendEBody(gbGenLODIdxGrp* pEBody);
	void AppendCBody(gbGenLODIdxGrp* pCBody);

	// precalculates chunked LOD index groups
	void Create();

	MM_PT GetPtPosCnt(float x, float z);
	WORD GetIdx(float x, float z);
	float GetQuadSize(int Lvl);
	int GetCBodyQuadCnt(int Lvl);

public:
	int GetTotLvlCnt() const;
	int GetTileSize() const;
	int GetTileCnt() const;
	D3DXVECTOR3 GetSPos() const;
	gbGenLODIdxGrp* GetLvlDat(int Lvl);
	void Free();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct IDX_DAT
{
	StaticIB<WORD>*	_pSIB;
	WORD* _pSystemIB;
	WORD* _pVIterator;
	int _Indicator;
	int	_IdxCnt;
	int	_TriCnt;

	IDX_DAT();
	~IDX_DAT();

	void Create(int Cnt, LPDIRECT3DDEVICE9 pD3DDev);
	void Append(int Cnt, const WORD* pIdxDat);
	void Lock();
	void UnLock();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MIP_MAP_DAT
{
	// 4 directions; 2 * 2 * 2 * 2 = 16 cases
	IDX_DAT _Bodies[16];			  

	// CHUNK_DIRECTION directions linker * x levels;
	IDX_DAT* _Connectors[CD_TOT_CNT]; 

	MIP_MAP_DAT();
	~MIP_MAP_DAT();

	void Create(const gbGenLODIdxGrp* pMMM, LPDIRECT3DDEVICE9 pD3DDev);
	void CreateUniqueLvl(const gbGenLODIdxGrp* pMMM, LPDIRECT3DDEVICE9 pD3DDev);
	void AppendBody(int Cnt, const WORD* pIdxDat);
	void AppendLink(int Cnt, const WORD* pIdxDat);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class gbDxGenLODIdx : public gbGenLODIdx
{
public:
	gbDxGenLODIdx(LPDIRECT3DDEVICE9 pD3DDev, int TileSize, int TileCnt, bool LeftHanded = true);
	virtual ~gbDxGenLODIdx();

private:
	LPDIRECT3DDEVICE9 _pD3DDev;
	MIP_MAP_DAT* _pMMD;

private:
	void Create();

public:
	const LPDIRECT3DINDEXBUFFER9 GetBody(int MyLvl, bool T, bool L, bool R, bool B) const;
	const LPDIRECT3DINDEXBUFFER9 GetLink(int MyLvl, int NeiborLvl, CHUNK_DIRECTION Dir) const;
	const WORD* GetSystemBody(int MyLvl, bool T, bool L, bool R, bool B) const;
	const WORD* GetSystemLink(int MyLvl, int NeiborLvl, CHUNK_DIRECTION Dir) const;
	int GetBodyPrimitiveNum(int MyLvl, bool T, bool L, bool R, bool B) const;
	int GetLinkPrimitiveNum(int MyLvl, int NeiborLvl, int Dir) const;

public:
	void Reset();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ntlworldsectorlod.inl"
