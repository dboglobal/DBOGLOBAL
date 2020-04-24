////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "precomp_ntlpresentation.h"
#include "ntlworldsectorlod.h"
#include "NtlWorldSectorLODStaticIB.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IDX_MEM::IDX_MEM()
{
	_MemPos = 0;
	_pDat = NULL;
}

IDX_MEM::~IDX_MEM()
{
	NTL_ARRAY_DELETE( _pDat );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

gbGenLODIdxGrp::gbGenLODIdxGrp(bool _LeftHanded)
{
	_LeftHanded = _LeftHanded;
	_pCBody = NULL;
	_pEBody = NULL;
	ZeroMemory(_pELink, sizeof(IDX_MEM*) * CD_TOT_CNT);
}

gbGenLODIdxGrp::~gbGenLODIdxGrp()
{
	NTL_DELETE( _pCBody );
	NTL_ARRAY_DELETE( _pEBody );

	for(int i = 0; i < CD_TOT_CNT; i++)
	{
		NTL_ARRAY_DELETE( _pELink[i] );
	}
}

void gbGenLODIdxGrp::CreateCBody()
{
	// Exceptions; Level 1 hasn't a center body.
	if(_Lvl == 1)
		return;

	if(!_pCBody)
	{
		_pCBody = NTL_NEW IDX_MEM;

		// 2 triangles per quad * 3 indices per triangles.
		if(_Lvl == 0)
			// Level 0 has a unique center body type
			_pCBody->Create(1 * 2 * 3);
		else
			_pCBody->Create(((((1 << _Lvl) - 2) * ((1 << _Lvl) - 2))) * 2 * 3);
	}
}

void gbGenLODIdxGrp::CreateEBody()
{
	// Exceptions; Level 0 hasn't a edge body.
	if(_Lvl == 0)
		return;
	
	if(!_pEBody)
	{
		_pEBody = NTL_NEW IDX_MEM [4];

		// +1 additional quad per edge for 2 corner triangles * 2 triangles per quad * 3 indices per triangles
		for(int i = 0; i < CD_TOT_CNT; i++)
		{
			if(_Lvl == 1)
				// Level 1 has a unique edge body type
				_pEBody[i].Create(1 * 2 * 3);
			else
				_pEBody[i].Create((((1 << _Lvl) - 2) + 1) * 2 * 3);
		}
	}
}

void gbGenLODIdxGrp::CreateELink()
{
	// Exceptions; Level 0 hasn't a edge link.
	if(_Lvl == 0)
		return;

	int Cnt;
	int i;
	int DividedBlockCnt;

	if(!_pELink[CD_TOP] && !_pELink[CD_LEFT] && !_pELink[CD_RIGHT] && !_pELink[CD_BOTTOM])
	{
		for(i = 0; i < CD_TOT_CNT; i++)
			_pELink[i] = NTL_NEW IDX_MEM [_Lvl];

		for(i = 0; i < _Lvl; i++)
		{
			DividedBlockCnt = 1 << i;
			Cnt = (((1 << _Lvl) - 2) + DividedBlockCnt) * 3;
				
			_pELink[CD_TOP][i].Create(Cnt);
			_pELink[CD_LEFT][i].Create(Cnt);
			_pELink[CD_RIGHT][i].Create(Cnt);
			_pELink[CD_BOTTOM][i].Create(Cnt);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

gbGenLODIdx::gbGenLODIdx(int TileSize, int TileCnt, bool LeftHanded)
{
	_TileSize = TileSize;
	_TileCnt = TileCnt;
	_PtCnt = _TileCnt + 1;
	_MeshSize = _TileSize * _TileCnt;
	_MeshSizeHalf = _MeshSize / 2;
	_SPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_EPos = _SPos + D3DXVECTOR3((float)_MeshSize, 0.0f, (float)_MeshSize);
	_LeftHanded = LeftHanded;

	// exceptions : 2, 4 or if not 2^n
	if(_TileCnt < 8 || (_TileCnt % 2))
		return;

	// find depth
	_TotLvlCnt = -1;
	int LvlTmp = _TileCnt;
	while(LvlTmp)
	{
		LvlTmp >>= 1;
		++_TotLvlCnt;
	}

	// create mip-map for each level
	Create();
}

gbGenLODIdx::~gbGenLODIdx()
{
	Free();
}

void gbGenLODIdx::Free()
{
	for(int i = 0; i < static_cast<int>(_pDatList.size()); i++)
	{
		NTL_DELETE( _pDatList[i] );
	}
	_pDatList.clear();
}

void gbGenLODIdx::AppendELinkX20(gbGenLODIdxGrp* pELink)
{
	int x, z;
	int QuadCnt = GetCBodyQuadCnt(pELink->GetLvl());
	float QuadSize = GetQuadSize(pELink->GetLvl());

	WORD Tri[3];
	float XMin = _SPos.x + QuadSize;
	float XMid = _SPos.x + _MeshSizeHalf;
	float XMax = _EPos.x - QuadSize;
	float ZMin = _SPos.z + QuadSize;
	float ZMid = _SPos.z + _MeshSizeHalf;
	float ZMax = _EPos.z - QuadSize;

	//------------------------------------------------------------------------------
	//
	// top edge link
	//
	//------------------------------------------------------------------------------

	// left part
	for(x = static_cast<int>(XMin); x < static_cast<int>(XMid); x += static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(static_cast<float>(x), ZMax);
		Tri[1] = GetIdx(_SPos.x, _EPos.z);
		Tri[2] = GetIdx(x + QuadSize, ZMax);
		pELink->AppendELinkIndices(Tri, CD_TOP, 0);
	}

	// middle part
	Tri[0] = GetIdx(XMid, ZMax);
	Tri[1] = GetIdx(_SPos.x, _EPos.z);
	Tri[2] = GetIdx(_EPos.x, _EPos.z);
	pELink->AppendELinkIndices(Tri, CD_TOP, 0);

	// right part
	for(x = static_cast<int>(XMid); x < static_cast<int>(XMax); x += static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(static_cast<float>(x), ZMax);
		Tri[1] = GetIdx(_EPos.x, _EPos.z);
		Tri[2] = GetIdx(x + QuadSize, ZMax);
		pELink->AppendELinkIndices(Tri, CD_TOP, 0);
	}

	//------------------------------------------------------------------------------
	//
	// left edge link
	//
	//------------------------------------------------------------------------------

	// top part
	for(z = static_cast<int>(ZMid); z < static_cast<int>(ZMax); z += static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(XMin, static_cast<float>(z));
		Tri[1] = GetIdx(_SPos.x, _EPos.z);
		Tri[2] = GetIdx(XMin, static_cast<float>(z + QuadSize));
		pELink->AppendELinkIndices(Tri, CD_LEFT, 0);
	}

	// middle part
	Tri[0] = GetIdx(_SPos.x, _SPos.z);
	Tri[1] = GetIdx(_SPos.x, _EPos.z);
	Tri[2] = GetIdx(XMin, ZMid);
	pELink->AppendELinkIndices(Tri, CD_LEFT, 0);

	// bottom part
	for(z = static_cast<int>(ZMin); z < static_cast<int>(ZMid); z += static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(_SPos.x, _SPos.z);
		Tri[1] = GetIdx(XMin, static_cast<float>(z) + QuadSize);
		Tri[2] = GetIdx(XMin, static_cast<float>(z));
		pELink->AppendELinkIndices(Tri, CD_LEFT, 0);
	}

	//------------------------------------------------------------------------------
	//
	// right edge link
	//
	//------------------------------------------------------------------------------

	// top part
	for(z = static_cast<int>(ZMid); z < static_cast<int>(ZMax); z += static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(XMax, static_cast<float>(z));
		Tri[1] = GetIdx(XMax, static_cast<float>(z) + QuadSize);
		Tri[2] = GetIdx(_EPos.x, _EPos.z);
		pELink->AppendELinkIndices(Tri, CD_RIGHT, 0);
	}

	// middle part
	Tri[0] = GetIdx(XMax, ZMid);
	Tri[1] = GetIdx(_EPos.x, _EPos.z);
	Tri[2] = GetIdx(_EPos.x, _SPos.z);
	pELink->AppendELinkIndices(Tri, CD_RIGHT, 0);

	// bottom part
	for(z = static_cast<int>(ZMin); z < static_cast<int>(ZMid); z += static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(XMax, static_cast<float>(z));
		Tri[1] = GetIdx(XMax, static_cast<float>(z) + QuadSize);
		Tri[2] = GetIdx(_EPos.x, _SPos.z);
		pELink->AppendELinkIndices(Tri, CD_RIGHT, 0);
	}

	//------------------------------------------------------------------------------
	//
	// bottom edge link
	//
	//------------------------------------------------------------------------------

	// left part
	for(x = static_cast<int>(XMin); x < static_cast<int>(XMid); x += static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(static_cast<float>(x), ZMin);
		Tri[1] = GetIdx(static_cast<float>(x) + QuadSize, ZMin);
		Tri[2] = GetIdx(_SPos.x, _SPos.z);
		pELink->AppendELinkIndices(Tri, CD_BOTTOM, 0);
	}

	// middle part
	Tri[0] = GetIdx(XMid, ZMin);
	Tri[1] = GetIdx(_EPos.x, _SPos.z);
	Tri[2] = GetIdx(_SPos.x, _SPos.z);
	pELink->AppendELinkIndices(Tri, CD_BOTTOM, 0);

	// right part
	for(x = static_cast<int>(XMid); x < static_cast<int>(XMax); x += static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(static_cast<float>(x), ZMin);
		Tri[1] = GetIdx(static_cast<float>(x) + QuadSize, ZMin);
		Tri[2] = GetIdx(_EPos.x, _SPos.z);
		pELink->AppendELinkIndices(Tri, CD_BOTTOM, 0);
	}

}

void gbGenLODIdx::AppendELinkX2X(gbGenLODIdxGrp* pELink, int XLvl)
{
	int x, xx, z, zz;
	int Lvl = pELink->GetLvl();
	int QuadCnt = GetCBodyQuadCnt(Lvl);
	float QuadSize = GetQuadSize(Lvl);
	int BlockCnt = (1 << XLvl);
	int MidBlockCnt = BlockCnt - 2;
	int MidSideBlockCnt = MidBlockCnt / 2;
	int BlockSize = _MeshSize / BlockCnt;

	WORD Tri[3];
	float XMin = _SPos.x + QuadSize;
	float XMid = _SPos.x + _MeshSizeHalf;
	float XMax = _EPos.x - QuadSize;
	float ZMin = _SPos.z + QuadSize;
	float ZMid = _SPos.z + _MeshSizeHalf;
	float ZMax = _EPos.z - QuadSize;

	//------------------------------------------------------------------------------
	//
	// top edge link
	//
	//------------------------------------------------------------------------------

	// left
	for(x = static_cast<int>(XMin); x < static_cast<int>(_SPos.x) + BlockSize; x += static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(static_cast<float>(x), ZMax);
		Tri[1] = GetIdx(_SPos.x, _EPos.z);
		Tri[2] = GetIdx(static_cast<float>(x) + QuadSize, ZMax);
		pELink->AppendELinkIndices(Tri, CD_TOP, XLvl);
	}
	// left extra
	Tri[0] = GetIdx(_SPos.x, _EPos.z);
	Tri[1] = GetIdx(_SPos.x + BlockSize, _EPos.z);
	Tri[2] = GetIdx(_SPos.x + BlockSize, ZMax);
	pELink->AppendELinkIndices(Tri, CD_TOP, XLvl);

	// left middle
	for(x = static_cast<int>(_SPos.x) + BlockSize; x < static_cast<int>(XMid); x += BlockSize)
	{
		for(xx = x; xx < x + BlockSize; xx += static_cast<int>(QuadSize))
		{
			Tri[0] = GetIdx(static_cast<float>(xx), ZMax);
			Tri[1] = GetIdx(static_cast<float>(x), _EPos.z);
			Tri[2] = GetIdx(static_cast<float>(xx) + QuadSize, ZMax);
			pELink->AppendELinkIndices(Tri, CD_TOP, XLvl);
		}

		// left middle extra
		Tri[0] = GetIdx(static_cast<float>(x), _EPos.z);
		Tri[1] = GetIdx(static_cast<float>(x) + BlockSize, _EPos.z);
		Tri[2] = GetIdx(static_cast<float>(x) + BlockSize, ZMax);
		pELink->AppendELinkIndices(Tri, CD_TOP, XLvl);
	}

	// right middle
	for(x = static_cast<int>(_EPos.x) - BlockSize; x > static_cast<int>(XMid); x -= BlockSize)
	{
		for(xx = x; xx > x - BlockSize; xx -= static_cast<int>(QuadSize))
		{
			Tri[0] = GetIdx(static_cast<float>(xx) - QuadSize, ZMax);
			Tri[1] = GetIdx(static_cast<float>(x), _EPos.z);
			Tri[2] = GetIdx(static_cast<float>(xx), ZMax);
			pELink->AppendELinkIndices(Tri, CD_TOP, XLvl);
		}

		// right middle extra
		Tri[0] = GetIdx(static_cast<float>(x) - BlockSize, ZMax);
		Tri[1] = GetIdx(static_cast<float>(x) - BlockSize, _EPos.z);
		Tri[2] = GetIdx(static_cast<float>(x), _EPos.z);
		pELink->AppendELinkIndices(Tri, CD_TOP, XLvl);
	}

	// right
	for(x = static_cast<int>(XMax); x > static_cast<int>(_EPos.x) - BlockSize; x -= static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(static_cast<float>(x) - QuadSize, ZMax);
		Tri[1] = GetIdx(_EPos.x, _EPos.z);
		Tri[2] = GetIdx(static_cast<float>(x), ZMax);
		pELink->AppendELinkIndices(Tri, CD_TOP, XLvl);
	}

	// right extra
	Tri[0] = GetIdx(_EPos.x - BlockSize, ZMax);
	Tri[1] = GetIdx(_EPos.x - BlockSize, _EPos.z);
	Tri[2] = GetIdx(_EPos.x, _EPos.z);
	pELink->AppendELinkIndices(Tri, CD_TOP, XLvl);

	//------------------------------------------------------------------------------
	//
	// left edge link
	//
	//------------------------------------------------------------------------------

	// top
	for(z = static_cast<int>(ZMax); z > static_cast<int>(_EPos.z) - BlockSize; z -= static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(_SPos.x, _EPos.z);
		Tri[1] = GetIdx(XMin, static_cast<float>(z));
		Tri[2] = GetIdx(XMin, static_cast<float>(z) - QuadSize);
		pELink->AppendELinkIndices(Tri, CD_LEFT, XLvl);
	}
	// top extra
	Tri[0] = GetIdx(_SPos.x, _EPos.z - BlockSize);
	Tri[1] = GetIdx(_SPos.x, _EPos.z);
	Tri[2] = GetIdx(XMin, _EPos.z - BlockSize);
	pELink->AppendELinkIndices(Tri, CD_LEFT, XLvl);

	// top middle
	for(z = static_cast<int>(_EPos.z) - BlockSize; z > static_cast<int>(ZMid); z -= BlockSize)
	{
		for(zz = z; zz > z - BlockSize; zz -= static_cast<int>(QuadSize))
		{
			Tri[0] = GetIdx(static_cast<float>(XMin), static_cast<float>(zz));
			Tri[1] = GetIdx(static_cast<float>(XMin), static_cast<float>(zz) - QuadSize);
			Tri[2] = GetIdx(_SPos.x, static_cast<float>(z));
			pELink->AppendELinkIndices(Tri, CD_LEFT, XLvl);
		}

		// top middle extra
		Tri[0] = GetIdx(_SPos.x, static_cast<float>(z));
		Tri[1] = GetIdx(XMin, static_cast<float>(z) - static_cast<float>(BlockSize));
		Tri[2] = GetIdx(_SPos.x, static_cast<float>(z) - static_cast<float>(BlockSize));
		pELink->AppendELinkIndices(Tri, CD_LEFT, XLvl);
	}

	// bottom middle
	for(z = static_cast<int>(_SPos.z) + BlockSize; z < static_cast<int>(XMid); z += BlockSize)
	{
		for(zz = z; zz < z + BlockSize; zz += static_cast<int>(QuadSize))
		{
			Tri[0] = GetIdx(_SPos.x, static_cast<float>(z));
			Tri[1] = GetIdx(XMin, static_cast<float>(zz) + QuadSize);
			Tri[2] = GetIdx(XMin, static_cast<float>(zz));
			pELink->AppendELinkIndices(Tri, CD_LEFT, XLvl);
		}

		// bottom middle extra
		Tri[0] = GetIdx(_SPos.x, static_cast<float>(z));
		Tri[1] = GetIdx(_SPos.x, static_cast<float>(z) + static_cast<float>(BlockSize));
		Tri[2] = GetIdx(XMin, static_cast<float>(z + BlockSize));
		pELink->AppendELinkIndices(Tri, CD_LEFT, XLvl);
	}

	// bottom
	for(z = static_cast<int>(ZMin); z < static_cast<int>(_SPos.z) + BlockSize; z += static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(XMin, static_cast<float>(z) + QuadSize);
		Tri[1] = GetIdx(XMin, static_cast<float>(z));
		Tri[2] = GetIdx(_SPos.x, _SPos.z);
		pELink->AppendELinkIndices(Tri, CD_LEFT, XLvl);
	}

	// bottom extra
	Tri[0] = GetIdx(_SPos.x, _SPos.z);
	Tri[1] = GetIdx(_SPos.x, _SPos.z + BlockSize);
	Tri[2] = GetIdx(XMin, _SPos.z + BlockSize);
	pELink->AppendELinkIndices(Tri, CD_LEFT, XLvl);

	//------------------------------------------------------------------------------
	//
	// right edge link
	//
	//------------------------------------------------------------------------------

	// top
	for(z = static_cast<int>(ZMax); z > static_cast<int>(_EPos.z) - BlockSize; z -= static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(XMax, static_cast<float>(z));
		Tri[1] = GetIdx(_EPos.x, _EPos.z);
		Tri[2] = GetIdx(XMax, static_cast<float>(z) - QuadSize);
		pELink->AppendELinkIndices(Tri, CD_RIGHT, XLvl);
	}
	// top extra
	Tri[0] = GetIdx(XMax, _EPos.z - BlockSize);
	Tri[1] = GetIdx(_EPos.x, _EPos.z);
	Tri[2] = GetIdx(_EPos.x, _EPos.z - BlockSize);
	pELink->AppendELinkIndices(Tri, CD_RIGHT, XLvl);

	// top middle
	for(z = static_cast<int>(_EPos.z) - BlockSize; z > static_cast<int>(ZMid); z -= BlockSize)
	{
		for(zz = z; zz > z - BlockSize; zz -= static_cast<int>(QuadSize))
		{
			Tri[0] = GetIdx(XMax, static_cast<float>(zz));
			Tri[1] = GetIdx(_EPos.x, static_cast<float>(z));
			Tri[2] = GetIdx(XMax, static_cast<float>(zz) - QuadSize);
			pELink->AppendELinkIndices(Tri, CD_RIGHT, XLvl);
		}

		// top middle extra
		Tri[0] = GetIdx(XMax, static_cast<float>(z) - BlockSize);
		Tri[1] = GetIdx(_EPos.x, static_cast<float>(z));
		Tri[2] = GetIdx(_EPos.x, static_cast<float>(z) - BlockSize);
		pELink->AppendELinkIndices(Tri, CD_RIGHT, XLvl);
	}

	// bottom middle
	for(z = static_cast<int>(_SPos.z) + BlockSize; z < static_cast<int>(XMid); z += BlockSize)
	{
		for(zz = z; zz < z + BlockSize; zz += static_cast<int>(QuadSize))
		{
			Tri[0] = GetIdx(XMax, static_cast<float>(zz));
			Tri[1] = GetIdx(XMax, static_cast<float>(zz) + QuadSize);
			Tri[2] = GetIdx(_EPos.x, static_cast<float>(z));
			pELink->AppendELinkIndices(Tri, CD_RIGHT, XLvl);
		}

		// bottom middle extra
		Tri[0] = GetIdx(XMax, static_cast<float>(z) + static_cast<float>(BlockSize));
		Tri[1] = GetIdx(_EPos.x, static_cast<float>(z + BlockSize));
		Tri[2] = GetIdx(_EPos.x, static_cast<float>(z));
		pELink->AppendELinkIndices(Tri, CD_RIGHT, XLvl);
	}

	// bottom
	for(z = static_cast<int>(ZMin); z < static_cast<int>(_SPos.z) + BlockSize; z += static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(XMax, static_cast<float>(z));
		Tri[1] = GetIdx(XMax, static_cast<float>(z + QuadSize));
		Tri[2] = GetIdx(_EPos.x, _SPos.z);
		pELink->AppendELinkIndices(Tri, CD_RIGHT, XLvl);
	}

	// bottom extra
	Tri[0] = GetIdx(XMax, _SPos.z + BlockSize);
	Tri[1] = GetIdx(_EPos.x, _SPos.z + BlockSize);
	Tri[2] = GetIdx(_EPos.x, _SPos.z);
	pELink->AppendELinkIndices(Tri, CD_RIGHT, XLvl);

	//------------------------------------------------------------------------------
	//
	// bottom edge link
	//
	//------------------------------------------------------------------------------

	// left
	for(x = static_cast<int>(XMin); x < static_cast<int>(_SPos.x + BlockSize); x += static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(static_cast<float>(x), ZMin);
		Tri[1] = GetIdx(static_cast<float>(x) + QuadSize, ZMin);
		Tri[2] = GetIdx(_SPos.x, _SPos.z);
		pELink->AppendELinkIndices(Tri, CD_BOTTOM, XLvl);
	}
	// left extra
	Tri[0] = GetIdx(_SPos.x, _SPos.z);
	Tri[1] = GetIdx(_SPos.x + BlockSize, ZMin);
	Tri[2] = GetIdx(_SPos.x + BlockSize, _SPos.z);
	pELink->AppendELinkIndices(Tri, CD_BOTTOM, XLvl);

	// left middle
	for(x = static_cast<int>(_SPos.x + BlockSize); x < static_cast<int>(XMid); x += static_cast<int>(BlockSize))
	{
		for(xx = static_cast<int>(x); xx < static_cast<int>(x + BlockSize); xx += static_cast<int>(QuadSize))
		{
			Tri[0] = GetIdx(static_cast<float>(xx), ZMin);
			Tri[1] = GetIdx(static_cast<float>(xx) + QuadSize, ZMin);
			Tri[2] = GetIdx(static_cast<float>(x), _SPos.z);
			pELink->AppendELinkIndices(Tri, CD_BOTTOM, XLvl);
		}

		// left middle extra
		Tri[0] = GetIdx((float)x + BlockSize, ZMin);
		Tri[1] = GetIdx((float)x + BlockSize, _SPos.z);
		Tri[2] = GetIdx((float)x, _SPos.z);
		pELink->AppendELinkIndices(Tri, CD_BOTTOM, XLvl);
	}

	// right middle
	for(x = static_cast<int>(XMid); x < static_cast<int>(_EPos.x - BlockSize); x += static_cast<int>(BlockSize))
	{
		for(xx = x; xx < x + BlockSize; xx += static_cast<int>(QuadSize))
		{
			Tri[0] = GetIdx(static_cast<float>(xx), ZMin);
			Tri[1] = GetIdx(static_cast<float>(xx) + QuadSize, ZMin);
			Tri[2] = GetIdx(static_cast<float>(x) + BlockSize, _SPos.z);
			pELink->AppendELinkIndices(Tri, CD_BOTTOM, XLvl);
		}

		// right middle extra
		Tri[0] = GetIdx(static_cast<float>(x), ZMin);
		Tri[1] = GetIdx(static_cast<float>(x) + BlockSize, _SPos.z);
		Tri[2] = GetIdx(static_cast<float>(x), _SPos.z);
		pELink->AppendELinkIndices(Tri, CD_BOTTOM, XLvl);
	}

	// right
	for(x = static_cast<int>(XMax); x > static_cast<int>(_EPos.x - BlockSize); x -= static_cast<int>(QuadSize))
	{
		Tri[0] = GetIdx(static_cast<float>(x), ZMin);
		Tri[1] = GetIdx(_EPos.x, _SPos.z);
		Tri[2] = GetIdx(static_cast<float>(x) - QuadSize, ZMin);
		pELink->AppendELinkIndices(Tri, CD_BOTTOM, XLvl);
	}

	// right extra
	Tri[0] = GetIdx(_EPos.x - BlockSize, ZMin);
	Tri[1] = GetIdx(_EPos.x, _SPos.z);
	Tri[2] = GetIdx(_EPos.x - BlockSize, _SPos.z);
	pELink->AppendELinkIndices(Tri, CD_BOTTOM, XLvl);
}

void gbGenLODIdx::AppendELink(gbGenLODIdxGrp* pELink)
{
	for(int i = 0; i < pELink->GetLvl(); i++)
	{
		// Level x to zero has a unique type of generating polygons
		if(i == 0)
			AppendELinkX20(pELink);
		else
			AppendELinkX2X(pELink, i);
	}	
}

void gbGenLODIdx::AppendEBody(gbGenLODIdxGrp* pEBody)
{
	int x, z;
	int Lvl = pEBody->GetLvl();
	int QuadCnt = GetCBodyQuadCnt(Lvl);
	float QuadSize = GetQuadSize(Lvl);

	WORD LTIdx, LBIdx, RTIdx, RBIdx; 
	float XMin = _SPos.x + QuadSize;
	float XMid = _SPos.x + _MeshSizeHalf;
	float XMax = _EPos.x - QuadSize;
	float ZMin = _SPos.z + QuadSize;
	float ZMid = _SPos.z + _MeshSizeHalf;
	float ZMax = _EPos.z - QuadSize;

	//------------------------------------------------------------------------------
	//
	// top edge body
	//
	//------------------------------------------------------------------------------

	// left corner
	LTIdx = GetIdx(_SPos.x, _EPos.z);
	RTIdx = GetIdx(_SPos.x + QuadSize, _EPos.z);
	RBIdx = GetIdx(_SPos.x + QuadSize, _EPos.z - QuadSize);
	pEBody->AppendEBodyIndices(LTIdx, RTIdx, RBIdx, CD_TOP);
	for(x = static_cast<int>(XMin); x < static_cast<int>(XMid); x += static_cast<int>(QuadSize))
	{
		// left part
		LBIdx = GetIdx(static_cast<float>(x), ZMax);
		LTIdx = GetIdx(static_cast<float>(x), _EPos.z);
		RTIdx = GetIdx(static_cast<float>(x) + QuadSize, _EPos.z);
		RBIdx = GetIdx(static_cast<float>(x) + QuadSize, ZMax);
		pEBody->AppendEBodyIndices(LBIdx, LTIdx, RBIdx, CD_TOP);
		pEBody->AppendEBodyIndices(LTIdx, RTIdx, RBIdx, CD_TOP);
	}

	// right corner
	for(x = static_cast<int>(XMid); x < static_cast<int>(XMax); x += static_cast<int>(QuadSize))
	{
		// right part
		LBIdx = GetIdx(static_cast<float>(x), ZMax);
		LTIdx = GetIdx(static_cast<float>(x), _EPos.z);
		RTIdx = GetIdx(static_cast<float>(x) + QuadSize, _EPos.z);
		RBIdx = GetIdx(static_cast<float>(x) + QuadSize, ZMax);
		pEBody->AppendEBodyIndices(LBIdx, LTIdx, RTIdx, CD_TOP);
		pEBody->AppendEBodyIndices(LBIdx, RTIdx, RBIdx, CD_TOP);
	}

	LTIdx = GetIdx(XMax, _EPos.z);
	RTIdx = GetIdx(_EPos.x, _EPos.z);
	LBIdx = GetIdx(XMax, ZMax);
	pEBody->AppendEBodyIndices(LTIdx, RTIdx, LBIdx, CD_TOP);

	//------------------------------------------------------------------------------
	//
	// left edge body
	//
	//------------------------------------------------------------------------------

	// top corner
	LTIdx = GetIdx(_SPos.x, _EPos.z);
	RBIdx = GetIdx(_SPos.x + QuadSize, ZMax);
	LBIdx = GetIdx(_SPos.x, ZMax);
	pEBody->AppendEBodyIndices(LTIdx, RBIdx, LBIdx, CD_LEFT);
	for(z = static_cast<int>(ZMid); z < static_cast<int>(ZMax); z += static_cast<int>(QuadSize))
	{
		// top part
		LBIdx = GetIdx(_SPos.x, static_cast<float>(z));
		LTIdx = GetIdx(_SPos.x, static_cast<float>(z) + QuadSize);
		RTIdx = GetIdx(_SPos.x + QuadSize, static_cast<float>(z) + static_cast<int>(QuadSize));
		RBIdx = GetIdx(_SPos.x + QuadSize, static_cast<float>(z));
		pEBody->AppendEBodyIndices(LBIdx, LTIdx, RBIdx, CD_LEFT);
		pEBody->AppendEBodyIndices(LTIdx, RTIdx, RBIdx, CD_LEFT);
	}
	
	// bottom corner
	LTIdx = GetIdx(_SPos.x, ZMin);
	RTIdx = GetIdx(XMin, ZMin);
	LBIdx = GetIdx(_SPos.x, _SPos.z);
	pEBody->AppendEBodyIndices(LTIdx, RTIdx, LBIdx, CD_LEFT);
	for(z = static_cast<int>(ZMin); z < static_cast<int>(ZMid); z += static_cast<int>(QuadSize))
	{
		// bottom part
		LBIdx = GetIdx(_SPos.x, static_cast<float>(z));
		LTIdx = GetIdx(_SPos.x, static_cast<float>(z) + QuadSize);
		RTIdx = GetIdx(_SPos.x + QuadSize, static_cast<float>(z) + QuadSize);
		RBIdx = GetIdx(_SPos.x + QuadSize, static_cast<float>(z));
		pEBody->AppendEBodyIndices(LBIdx, LTIdx, RTIdx, CD_LEFT);
		pEBody->AppendEBodyIndices(LBIdx, RTIdx, RBIdx, CD_LEFT);
	}

	//------------------------------------------------------------------------------
	//
	// right edge body
	//
	//------------------------------------------------------------------------------

	// top corner
	LBIdx = GetIdx(_EPos.x - QuadSize, _EPos.z - QuadSize);
	RTIdx = GetIdx(_EPos.x, _EPos.z);
	RBIdx = GetIdx(_EPos.x, _EPos.z - QuadSize);
	pEBody->AppendEBodyIndices(LBIdx, RTIdx, RBIdx, CD_RIGHT);
	for(z = static_cast<int>(ZMid); z < static_cast<int>(ZMax); z += static_cast<int>(QuadSize))
	{
		// top part
		LBIdx = GetIdx(_EPos.x - QuadSize, static_cast<float>(z));
		LTIdx = GetIdx(_EPos.x - QuadSize, static_cast<float>(z) + QuadSize);
		RTIdx = GetIdx(_EPos.x, static_cast<float>(z) + QuadSize);
		RBIdx = GetIdx(_EPos.x, static_cast<float>(z));
		pEBody->AppendEBodyIndices(LBIdx, LTIdx, RTIdx, CD_RIGHT);
		pEBody->AppendEBodyIndices(LBIdx, RTIdx, RBIdx, CD_RIGHT);
	}
	
	// bottom corner
	LTIdx = GetIdx(XMax, ZMin);
	RTIdx = GetIdx(_EPos.x, ZMin);
	RBIdx = GetIdx(_EPos.x, _SPos.z);
	pEBody->AppendEBodyIndices(LTIdx, RTIdx, RBIdx, CD_RIGHT);
	for(z = static_cast<int>(ZMin); z < static_cast<int>(ZMid); z += static_cast<int>(QuadSize))
	{
		// bottom part
		LBIdx = GetIdx(_EPos.x - QuadSize, static_cast<float>(z));
		LTIdx = GetIdx(_EPos.x - QuadSize, static_cast<float>(z) + QuadSize);
		RTIdx = GetIdx(_EPos.x, static_cast<float>(z) + QuadSize);
		RBIdx = GetIdx(_EPos.x, static_cast<float>(z));
		pEBody->AppendEBodyIndices(LBIdx, LTIdx, RBIdx, CD_RIGHT);
		pEBody->AppendEBodyIndices(LTIdx, RTIdx, RBIdx, CD_RIGHT);
	}
	
	//------------------------------------------------------------------------------
	//
	// bottom edge body
	//
	//------------------------------------------------------------------------------

	// left corner
	LBIdx = GetIdx(_SPos.x, _SPos.z);
	RTIdx = GetIdx(_SPos.x + QuadSize, _SPos.z + QuadSize);
	RBIdx = GetIdx(_SPos.x + QuadSize, _SPos.z);
	pEBody->AppendEBodyIndices(LBIdx, RTIdx, RBIdx, CD_BOTTOM);
	for(x = static_cast<int>(XMin); x < static_cast<int>(XMid); x += static_cast<int>(QuadSize))
	{
		// left part
		LBIdx = GetIdx(static_cast<float>(x), _SPos.z);
		LTIdx = GetIdx(static_cast<float>(x), _SPos.z + QuadSize);
		RTIdx = GetIdx(static_cast<float>(x) + QuadSize, _SPos.z + QuadSize);
		RBIdx = GetIdx(static_cast<float>(x) + QuadSize, _SPos.z);
		pEBody->AppendEBodyIndices(LBIdx, LTIdx, RTIdx, CD_BOTTOM);
		pEBody->AppendEBodyIndices(LBIdx, RTIdx, RBIdx, CD_BOTTOM);
	}

	// right corner
	LBIdx = GetIdx(XMax, _SPos.z);
	LTIdx = GetIdx(XMax, _SPos.z + QuadSize);
	RBIdx = GetIdx(_EPos.x, _SPos.z);
	pEBody->AppendEBodyIndices(LBIdx, LTIdx, RBIdx, CD_BOTTOM);
	for(x = static_cast<int>(XMid); x < static_cast<int>(XMax); x += static_cast<int>(QuadSize))
	{
		// right part
		LBIdx = GetIdx(static_cast<float>(x), _SPos.z);
		LTIdx = GetIdx(static_cast<float>(x), _SPos.z + QuadSize);
		RTIdx = GetIdx(static_cast<float>(x) + QuadSize, _SPos.z + QuadSize);
		RBIdx = GetIdx(static_cast<float>(x) + QuadSize, _SPos.z);
		pEBody->AppendEBodyIndices(LBIdx, LTIdx, RBIdx, CD_BOTTOM);
		pEBody->AppendEBodyIndices(LTIdx, RTIdx, RBIdx, CD_BOTTOM);
	}
}

void gbGenLODIdx::AppendCBody(gbGenLODIdxGrp* pCBody)
{
	int x, z;
	int QuadCnt = GetCBodyQuadCnt(pCBody->GetLvl());
	float QuadSize = GetQuadSize(pCBody->GetLvl());
		
	WORD LTIdx, LBIdx, RTIdx, RBIdx; 
	MM_REGION LTRgn, LBRgn, RTRgn, RBRgn;
	float XMin = _SPos.x + QuadSize;
	float XMid = _SPos.x + _MeshSizeHalf;
	float XMax = _EPos.x - QuadSize;
	float ZMin = _SPos.z + QuadSize;
	float ZMid = _SPos.z + _MeshSizeHalf;
	float ZMax = _EPos.z - QuadSize;
	LTRgn.SetRgn(XMin, XMid, ZMid, ZMax);
	LBRgn.SetRgn(XMin, XMid, ZMin, ZMid);
	RTRgn.SetRgn(XMid, XMax, ZMid, ZMax);
	RBRgn.SetRgn(XMid, XMax, ZMin, ZMid);

	// the shape of center meshs looks like 'X' so create each in 4 diffrent ways below

	// left-top region
	for(z = static_cast<int>(LTRgn._ZMin); z < static_cast<int>(LTRgn._ZMax); z += static_cast<int>(QuadSize))
		for(x = static_cast<int>(LTRgn._XMin); x < static_cast<int>(LTRgn._XMax); x += static_cast<int>(QuadSize))
		{
			LBIdx = GetIdx(static_cast<float>(x), static_cast<float>(z));
			LTIdx = GetIdx(static_cast<float>(x), static_cast<float>(z) + QuadSize);
			RTIdx = GetIdx(static_cast<float>(x) + QuadSize, static_cast<float>(z) + QuadSize);
			RBIdx = GetIdx(static_cast<float>(x) + QuadSize, static_cast<float>(z));
			pCBody->AppendCBodyIndices(LBIdx, LTIdx, RBIdx);
			pCBody->AppendCBodyIndices(LTIdx, RTIdx, RBIdx);
		}

	// left-bottom region
	for(z = static_cast<int>(LBRgn._ZMin); z < static_cast<int>(LBRgn._ZMax); z += static_cast<int>(QuadSize))
		for(x = static_cast<int>(LBRgn._XMin); x < static_cast<int>(LBRgn._XMax); x += static_cast<int>(QuadSize))
		{
			LBIdx = GetIdx(static_cast<float>(x), static_cast<float>(z));
			LTIdx = GetIdx(static_cast<float>(x), static_cast<float>(z) + QuadSize);
			RTIdx = GetIdx(static_cast<float>(x) + QuadSize, static_cast<float>(z) + QuadSize);
			RBIdx = GetIdx(static_cast<float>(x) + QuadSize, static_cast<float>(z));
			pCBody->AppendCBodyIndices(LBIdx, LTIdx, RTIdx);
			pCBody->AppendCBodyIndices(LBIdx, RTIdx, RBIdx);
		}

	// right-top region
	for(z = static_cast<int>(RTRgn._ZMin); z < static_cast<int>(RTRgn._ZMax); z += static_cast<int>(QuadSize))
		for(x = static_cast<int>(RTRgn._XMin); x < static_cast<int>(RTRgn._XMax); x += static_cast<int>(QuadSize))
		{
			LBIdx = GetIdx(static_cast<float>(x), static_cast<float>(z));
			LTIdx = GetIdx(static_cast<float>(x), static_cast<float>(z) + QuadSize);
			RTIdx = GetIdx(static_cast<float>(x) + QuadSize, static_cast<float>(z) + QuadSize);
			RBIdx = GetIdx(static_cast<float>(x) + QuadSize, static_cast<float>(z));
			pCBody->AppendCBodyIndices(LTIdx, RTIdx, LBIdx);
			pCBody->AppendCBodyIndices(LBIdx, RTIdx, RBIdx);
		}

	// right-bottom region
	for(z = static_cast<int>(RBRgn._ZMin); z < static_cast<int>(RBRgn._ZMax); z += static_cast<int>(QuadSize))
		for(x = static_cast<int>(RBRgn._XMin); x < static_cast<int>(RBRgn._XMax); x += static_cast<int>(QuadSize))
		{
			LBIdx = GetIdx(static_cast<float>(x), static_cast<float>(z));
			LTIdx = GetIdx(static_cast<float>(x), static_cast<float>(z) + QuadSize);
			RTIdx = GetIdx(static_cast<float>(x) + QuadSize, static_cast<float>(z) + QuadSize);
			RBIdx = GetIdx(static_cast<float>(x) + QuadSize, static_cast<float>(z));
			pCBody->AppendCBodyIndices(LBIdx, LTIdx, RBIdx);
			pCBody->AppendCBodyIndices(LTIdx, RTIdx, RBIdx);
		}
}

gbGenLODIdxGrp* gbGenLODIdx::GetLvlDat(int Lvl)
{
	for(vector<gbGenLODIdxGrp*>::iterator iter = _pDatList.begin(); iter != _pDatList.end(); ++iter)
		if((*iter)->GetLvl() == Lvl)
			return *iter;

	return NULL;
}

void gbGenLODIdx::Create()
{
	int i;

	// Level zero; a two-poly quad for the lowest detail mip-map
	WORD RB = GetIdx(_EPos.x, _SPos.z);
	WORD RT = GetIdx(_EPos.x, _EPos.z);
	WORD LT = GetIdx(_SPos.x, _EPos.z);
	WORD LB = GetIdx(_SPos.x, _SPos.z);
	gbGenLODIdxGrp* p3M0 = NTL_NEW gbGenLODIdxGrp(_LeftHanded);
	p3M0->SetLvl(0);
	p3M0->CreateCBody();
	p3M0->AppendCBodyIndices(LT, RT, LB);
	p3M0->AppendCBodyIndices(LB, RT, RB);
	_pDatList.push_back(p3M0);

	// Level one is also unique
	WORD T = GetIdx(_SPos.x + _MeshSizeHalf, _EPos.z);
	WORD B = GetIdx(_SPos.x + _MeshSizeHalf, _SPos.z);
	WORD L = GetIdx(_SPos.x, _SPos.z + _MeshSizeHalf);
	WORD R = GetIdx(_EPos.x, _SPos.z + _MeshSizeHalf);
	WORD C = GetIdx(_SPos.x + _MeshSizeHalf, _SPos.z + _MeshSizeHalf);
	gbGenLODIdxGrp* p3M1 = NTL_NEW gbGenLODIdxGrp(_LeftHanded);
	p3M1->SetLvl(1);
	p3M1->CreateEBody();
	p3M1->AppendEBodyIndices(C, LT, T, CD_TOP);
	p3M1->AppendEBodyIndices(C, T, RT, CD_TOP);
	p3M1->AppendEBodyIndices(L, LT, C, CD_LEFT);
	p3M1->AppendEBodyIndices(L, C, LB, CD_LEFT);
	p3M1->AppendEBodyIndices(C, RT, R, CD_RIGHT);
	p3M1->AppendEBodyIndices(C, R, RB, CD_RIGHT);
	p3M1->AppendEBodyIndices(LB, C, B, CD_BOTTOM);
	p3M1->AppendEBodyIndices(B, C, RB, CD_BOTTOM);
	p3M1->CreateELink();
	p3M1->AppendELinkIndices(LT, RT, C, CD_TOP, 0);
	p3M1->AppendELinkIndices(LT, C, LB, CD_LEFT, 0);
	p3M1->AppendELinkIndices(C, RT, RB, CD_RIGHT, 0); 
	p3M1->AppendELinkIndices(LB, C, RB, CD_BOTTOM, 0);
	_pDatList.push_back(p3M1);

	// create all center body, edge body, edge link
	for(i = 2; i <= _TotLvlCnt; i++)
	{
		gbGenLODIdxGrp* p = NTL_NEW gbGenLODIdxGrp(_LeftHanded);
		p->SetLvl(i);
		p->CreateCBody(); AppendCBody(p);
		p->CreateEBody(); AppendEBody(p);
		p->CreateELink(); AppendELink(p);
		_pDatList.push_back(p);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IDX_DAT::IDX_DAT()
{
	_pSIB = NULL;
	_pSystemIB = NULL;
}

IDX_DAT::~IDX_DAT()
{
	NTL_DELETE( _pSIB );
	NTL_ARRAY_DELETE( _pSystemIB );
}

void IDX_DAT::Lock()
{
	unsigned int uStartVtx = 0;
	_pVIterator = (WORD*)(_pSIB->Lock(_IdxCnt, uStartVtx));
}

void IDX_DAT::UnLock()
{
	_pSIB->Unlock();
}

void IDX_DAT::Create(int Cnt, LPDIRECT3DDEVICE9 pD3DDev)
{
	_pSIB = NTL_NEW StaticIB<WORD>(pD3DDev, Cnt);
	_pSystemIB = NTL_NEW WORD [Cnt];

	_Indicator = 0;
	_IdxCnt = Cnt;
	_TriCnt = _IdxCnt / 3;
}

void IDX_DAT::Append(int Cnt, const WORD* pIdxDat)
{
	int IdxCnt = 0;
	while(IdxCnt < Cnt)
	{
		_pVIterator[_Indicator] = _pSystemIB[_Indicator] = pIdxDat[IdxCnt];
		_Indicator++;
		IdxCnt++;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MIP_MAP_DAT::MIP_MAP_DAT()
{
	ZeroMemory(_Bodies, sizeof(IDX_DAT) * 16);
	ZeroMemory(_Connectors, sizeof(IDX_DAT*) * CD_TOT_CNT);
}

MIP_MAP_DAT::~MIP_MAP_DAT()
{
	for(int i = 0; i < CD_TOT_CNT; i++)
	{
		NTL_ARRAY_DELETE( _Connectors[i] );
	}
}

void MIP_MAP_DAT::AppendBody(int Cnt, const WORD* pIdxDat)
{
}

void MIP_MAP_DAT::AppendLink(int Cnt, const WORD* pIdxDat)
{
}

void MIP_MAP_DAT::CreateUniqueLvl(const gbGenLODIdxGrp* pMMM, LPDIRECT3DDEVICE9 pD3DDev)
{
	int i, j;
	int BodyCnt;
	
	if(pMMM->GetLvl() == 0)
	{
		BodyCnt = pMMM->GetCBody()->GetIdxCnt();
		const WORD* pBodyIdxDat = pMMM->GetCBody()->GetDatPtr();

		_Bodies[0].Create(BodyCnt, pD3DDev);
		_Bodies[0].Lock();
		{
			// Zero level has only center body.
			_Bodies[0].Append(BodyCnt, pBodyIdxDat);
		}
		_Bodies[0].UnLock();
	}
	else if(pMMM->GetLvl() == 1)
	{
		int EBodyCnt = pMMM->GetEBody()->GetIdxCnt(); // 4 edge bodies have the same index counts, just pick the first one
		const IDX_MEM* pEBodyIdxGrp = pMMM->GetEBody();

		for(i = 0; i < 16; ++i)
		{
			BodyCnt = 0;
			
			// 16 body cases; a center body + some edge bodies
			if(!(i & (1 << 0))) BodyCnt += EBodyCnt;
			if(!(i & (1 << 1)))	BodyCnt += EBodyCnt;
			if(!(i & (1 << 2)))	BodyCnt += EBodyCnt;
			if(!(i & (1 << 3)))	BodyCnt += EBodyCnt;

			if(BodyCnt)
			{
				_Bodies[i].Create(BodyCnt, pD3DDev);
				
				_Bodies[i].Lock();
				{
					// Level one hasn't center body but edge bodies.
					for(j = 0; j < CD_TOT_CNT; ++j)
						if(!(i & (1 << j)))
							_Bodies[i].Append(EBodyCnt, pEBodyIdxGrp[j].GetDatPtr());
				}
				_Bodies[i].UnLock();
			}
			
			BodyCnt = 0;
		}

		// create connectors
		const IDX_MEM* p;
		const IDX_MEM* pELinkGrp[CD_TOT_CNT];
		int Lvl = pMMM->GetLvl();
		int Cnt;
		
		for(i = 0; i < CD_TOT_CNT; i++)
		{
			pELinkGrp[i] = pMMM->GetELink(CHUNK_DIRECTION(i));
			_Connectors[i] = NTL_NEW IDX_DAT [Lvl];
			
			for(j = 0; j < Lvl; j++)
			{
				p = (&pELinkGrp[i][j]);
				Cnt = p->GetIdxCnt();
				
				_Connectors[i][j].Create(Cnt, pD3DDev);
				_Connectors[i][j].Lock();
				{
					_Connectors[i][j].Append(Cnt, p->GetDatPtr());
				}
				_Connectors[i][j].UnLock();
			}
		}
	}
}

void MIP_MAP_DAT::Create(const gbGenLODIdxGrp* pMMM, LPDIRECT3DDEVICE9 pD3DDev)
{
	if(pMMM->GetLvl() < 2)
	{
		CreateUniqueLvl(pMMM, pD3DDev);
		return;
	}

	int i, j;
	int BodyCnt = 0;
	const WORD* pCBodyIdx = pMMM->GetCBody()->GetDatPtr();
	const IDX_MEM* pEBodyIdxGrp = pMMM->GetEBody();
	int CBodyCnt = pMMM->GetCBody()->GetIdxCnt();
	int EBodyCnt = pMMM->GetEBody()->GetIdxCnt(); // 4 edge bodies have the same index counts, just pick the first one

	// create bodies
	for(i = 0; i < 16; ++i)
	{
		BodyCnt = CBodyCnt; 
		
		// 16 body cases; a center body + some edge bodies
		if(!(i & (1 << 0))) BodyCnt += EBodyCnt;
		if(!(i & (1 << 1)))	BodyCnt += EBodyCnt;
		if(!(i & (1 << 2)))	BodyCnt += EBodyCnt;
		if(!(i & (1 << 3)))	BodyCnt += EBodyCnt;
		
		_Bodies[i].Create(BodyCnt, pD3DDev);

		_Bodies[i].Lock();
		{
			// start by copying the center portion of the tile
			_Bodies[i].Append(CBodyCnt, pCBodyIdx);
			
			// then do the rest
			for(j = 0; j < CD_TOT_CNT; ++j)
				if(!(i & (1 << j)))
					_Bodies[i].Append(EBodyCnt, pEBodyIdxGrp[j].GetDatPtr());
		}
		_Bodies[i].UnLock();

		BodyCnt = 0;
	}

	// create connectors
	const IDX_MEM* p;
	const IDX_MEM* pELinkGrp[CD_TOT_CNT];
	int Lvl = pMMM->GetLvl();
	int Cnt;

	for(i = 0; i < CD_TOT_CNT; i++)
	{
		pELinkGrp[i] = pMMM->GetELink(CHUNK_DIRECTION(i));
		_Connectors[i] = NTL_NEW IDX_DAT [Lvl];

		for(j = 0; j < Lvl; j++)
		{
			p = (&pELinkGrp[i][j]);
			Cnt = p->GetIdxCnt();

			_Connectors[i][j].Create(Cnt, pD3DDev);
			_Connectors[i][j].Lock();
			{
				_Connectors[i][j].Append(Cnt, p->GetDatPtr());
			}
			_Connectors[i][j].UnLock();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

gbDxGenLODIdx::gbDxGenLODIdx(LPDIRECT3DDEVICE9 pD3DDev, int TileSize, int TileCnt, bool LeftHanded) : gbGenLODIdx(TileSize, TileCnt, LeftHanded)
{
	_pD3DDev = pD3DDev;

	Create();
}

gbDxGenLODIdx::~gbDxGenLODIdx()
{
	NTL_ARRAY_DELETE( _pMMD );
}

const WORD* gbDxGenLODIdx::GetSystemBody(int MyLvl, bool T, bool L, bool R, bool B) const
{
	int BodyEdgeIdx = 0;
	
	if(T) BodyEdgeIdx |= 1 << CD_TOP;
	if(L) BodyEdgeIdx |= 1 << CD_LEFT;
	if(R) BodyEdgeIdx |= 1 << CD_RIGHT;
	if(B) BodyEdgeIdx |= 1 << CD_BOTTOM;
	
	if(!_pMMD[MyLvl]._Bodies[BodyEdgeIdx]._pSystemIB)
		return NULL;
	else
		return _pMMD[MyLvl]._Bodies[BodyEdgeIdx]._pSystemIB;
}

const WORD* gbDxGenLODIdx::GetSystemLink(int MyLvl, int NeiborLvl, CHUNK_DIRECTION Dir) const
{
	if(!_pMMD[MyLvl]._Connectors[Dir] || !_pMMD[MyLvl]._Connectors[Dir][NeiborLvl]._pSystemIB)
		return NULL;
	return
		_pMMD[MyLvl]._Connectors[Dir][NeiborLvl]._pSystemIB;
}

const LPDIRECT3DINDEXBUFFER9 gbDxGenLODIdx::GetBody(int MyLvl, bool T, bool L, bool R, bool B) const
{
	int BodyEdgeIdx = 0;

	if(T) BodyEdgeIdx |= 1 << CD_TOP;
	if(L) BodyEdgeIdx |= 1 << CD_LEFT;
	if(R) BodyEdgeIdx |= 1 << CD_RIGHT;
	if(B) BodyEdgeIdx |= 1 << CD_BOTTOM;

	if(!_pMMD[MyLvl]._Bodies[BodyEdgeIdx]._pSIB)
		return NULL;
	else
		return _pMMD[MyLvl]._Bodies[BodyEdgeIdx]._pSIB->GetInterface();
}

const LPDIRECT3DINDEXBUFFER9 gbDxGenLODIdx::GetLink(int MyLvl, int NeiborLvl, CHUNK_DIRECTION Dir) const
{
	if(!_pMMD[MyLvl]._Connectors[Dir] || !_pMMD[MyLvl]._Connectors[Dir][NeiborLvl]._pSIB)
		return NULL;
	return
		_pMMD[MyLvl]._Connectors[Dir][NeiborLvl]._pSIB->GetInterface();
}

int gbDxGenLODIdx::GetBodyPrimitiveNum(int MyLvl, bool T, bool L, bool R, bool B) const
{
	int BodyEdgeIdx = 0;

	if(T) BodyEdgeIdx |= 1 << CD_TOP;
	if(L) BodyEdgeIdx |= 1 << CD_LEFT;
	if(R) BodyEdgeIdx |= 1 << CD_RIGHT;
	if(B) BodyEdgeIdx |= 1 << CD_BOTTOM;

	return _pMMD[MyLvl]._Bodies[BodyEdgeIdx]._TriCnt;
}

int gbDxGenLODIdx::GetLinkPrimitiveNum(int MyLvl, int NeiborLvl, int Dir) const
{
	return _pMMD[MyLvl]._Connectors[Dir][NeiborLvl]._TriCnt;
}

void gbDxGenLODIdx::Create()
{
	int i;
	int TotLvlCnt = gbGenLODIdx::GetTotLvlCnt() + 1;

	_pMMD = NTL_NEW MIP_MAP_DAT [TotLvlCnt];

	for(i = 0; i < TotLvlCnt; i++)
		_pMMD[i].Create(gbGenLODIdx::GetLvlDat(i), _pD3DDev);

	Free();
}

void gbDxGenLODIdx::Reset()
{
	//_pD3DDev = dDEVICE->GetDevice();
}