#ifndef __CHUNKEDLOD_INL__
#define __CHUNKEDLOD_INL__


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const int IDX_MEM::GetIdxCnt() const {return _MemPos;}
inline const WORD* IDX_MEM::GetDatPtr() const {return _pDat;}
inline void IDX_MEM::Create(int Cnt){_pDat = NTL_NEW WORD [Cnt];}
inline void IDX_MEM::Append(WORD Value){_pDat[_MemPos++] = Value;}
inline IDX_MEM* gbGenLODIdxGrp::GetCBody() const {return _pCBody;}
inline IDX_MEM* gbGenLODIdxGrp::GetEBody() const {return _pEBody;}
inline IDX_MEM* gbGenLODIdxGrp::GetELink(CHUNK_DIRECTION Dir) const {return _pELink[Dir];}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void gbGenLODIdxGrp::AppendCBody(WORD Value){_pCBody->Append(Value);}
inline void gbGenLODIdxGrp::AppendEBody(WORD Value, CHUNK_DIRECTION Dir){_pEBody[Dir].Append(Value);}
inline void gbGenLODIdxGrp::AppendELink(WORD Value, CHUNK_DIRECTION Dir, int Lvl){_pELink[Dir][Lvl].Append(Value);}
inline void gbGenLODIdxGrp::AppendCBodyIndices(WORD I1, WORD I2, WORD I3)
{
	AppendCBody(I1);
	AppendCBody(I3);
	AppendCBody(I2);
	/*
	if(_LeftHanded)
	{
		AppendCBody(I1);
		AppendCBody(I2);
		AppendCBody(I3);
	}
	else
	{
		AppendCBody(I1);
		AppendCBody(I3);
		AppendCBody(I2);
	}*/
}
inline void gbGenLODIdxGrp::AppendEBodyIndices(WORD I1, WORD I2, WORD I3, CHUNK_DIRECTION Dir)
{
	AppendEBody(I1, Dir);
	AppendEBody(I3, Dir);
	AppendEBody(I2, Dir);
	/*
	if(_LeftHanded)
	{
		AppendEBody(I1, Dir);
		AppendEBody(I2, Dir);
		AppendEBody(I3, Dir);
	}
	else
	{
		AppendEBody(I1, Dir);
		AppendEBody(I3, Dir);
		AppendEBody(I2, Dir);
	}*/
}
inline void gbGenLODIdxGrp::AppendELinkIndices(WORD I1, WORD I2, WORD I3, CHUNK_DIRECTION Dir, int Lvl)
{
	AppendELink(I1, Dir, Lvl);
	AppendELink(I3, Dir, Lvl);
	AppendELink(I2, Dir, Lvl);
	/*
	if(_LeftHanded)
	{
		AppendELink(I1, Dir, Lvl);
		AppendELink(I2, Dir, Lvl);
		AppendELink(I3, Dir, Lvl);
	}
	else
	{
		AppendELink(I1, Dir, Lvl);
		AppendELink(I3, Dir, Lvl);
		AppendELink(I2, Dir, Lvl);
	}*/
}
inline void gbGenLODIdxGrp::AppendELinkIndices(WORD Indices[3], CHUNK_DIRECTION Dir, int Lvl)
{
	AppendELink(Indices[0], Dir, Lvl);
	AppendELink(Indices[2], Dir, Lvl);
	AppendELink(Indices[1], Dir, Lvl);
	/*
	if(_LeftHanded)
	{
		AppendELink(Indices[0], Dir, Lvl);
		AppendELink(Indices[1], Dir, Lvl);
		AppendELink(Indices[2], Dir, Lvl);
	}
	else
	{
		AppendELink(Indices[0], Dir, Lvl);
		AppendELink(Indices[2], Dir, Lvl);
		AppendELink(Indices[1], Dir, Lvl);
	}*/
}
inline void gbGenLODIdxGrp::SetLvl(int Lvl){_Lvl = Lvl;}
inline int gbGenLODIdxGrp::GetLvl() const {return _Lvl;}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void MM_REGION::SetRgn(float XMin, float XMax, float ZMin, float ZMax){_XMin = XMin; _XMax = XMax; _ZMin = ZMin, _ZMax = ZMax;}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline int gbGenLODIdx::GetTotLvlCnt() const {return _TotLvlCnt;}
inline int gbGenLODIdx::GetTileSize() const {return _TileSize;}
inline int gbGenLODIdx::GetTileCnt() const {return _TileCnt;}
inline D3DXVECTOR3 gbGenLODIdx::GetSPos() const {return _SPos;}
inline int gbGenLODIdx::GetCBodyQuadCnt(int Lvl)
{
	// Level 0 has only one quad.
	if(!Lvl) return 1;

	// Level 1 hasn't.
	if(Lvl == 1) return 0;

	return ((1 << Lvl) - 2) * ((1 << Lvl) - 2);
}

inline MM_PT gbGenLODIdx::GetPtPosCnt(float x, float z)
{
	MM_PT pt;
	
	pt.x = (int)(x - _SPos.x) / _TileSize;
	pt.z = (int)(z - _SPos.z) / _TileSize;

	return pt;
}
inline WORD gbGenLODIdx::GetIdx(float x, float z)
{
	MM_PT pt = GetPtPosCnt(x, z);

	return (pt.z * _PtCnt + pt.x);
}
inline float gbGenLODIdx::GetQuadSize(int Lvl)
{
	// exceptions
	if(Lvl < 2)
		return -1;

	return ((float)_MeshSize / (1 << Lvl));
}


#endif __CHUNKEDLOD_INL__