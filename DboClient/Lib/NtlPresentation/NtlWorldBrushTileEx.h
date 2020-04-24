#pragma once


#include "NtlWorldBrushController.h"


template <class CBObjClass>
class CNtlWorldBrushTileEx : public CNtlWorldBrushController
{
private:
	CBObjClass* m_pObj;  
	RwReal (CBObjClass::*m_pFunc)(RwV3d& Pos);

protected:
	RwReal GetHeight(RwV3d& Pos);

public:
	CNtlWorldBrushTileEx(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos));
	~CNtlWorldBrushTileEx();

private:
	vector<RwInt32> m_vecTileIdx;
	RwIm3DVertex	m_VB[4];
	RwInt32			m_IdxSX, m_IdxEX;
	RwInt32			m_IdxSZ, m_IdxEZ;
	RwBool			m_UpdateVB;
	RwInt32			m_IdxSLmtX, m_IdxELmtX;
	RwInt32			m_IdxSLmtZ, m_IdxELmtZ;

	RwInt32			m_iChunkSize;
	
private:
	VOID RegisterTileIdx(RwInt32 IdxX, RwInt32 IdxZ, RwBool _DeleteTile);

public:
	vector<RwInt32>&	GetTileContainer() { return m_vecTileIdx; }
	RwUInt32			GetTileNum() { return m_vecTileIdx.size(); }
	void				SetPosS(RwV3d& Pos);
	void				SetPosE(RwV3d& Pos, RwBool _DeleteTile = FALSE);

	void				SetChunkSize(RwInt32 iChunkSize);
	RwInt32				GetChunkSize() { return m_iChunkSize; }

	void	Clear() { m_vecTileIdx.clear(); }
	VOID	Update(RwInt32 SLmtX, RwInt32 ELmtX, RwInt32 SLmtZ, RwInt32 ELmtZ);
	void	Render();
};

template <class CBObjClass>
CNtlWorldBrushTileEx<CBObjClass>::CNtlWorldBrushTileEx(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos))
{
	NTL_ASSERTE(pObj);
	NTL_ASSERTE(pFunc);

	m_pObj			= pObj; 
	m_pFunc			= pFunc;
	m_UpdateVB		= TRUE;
	DBO_WARNING_MESSAGE("CNtlWorldPathEngineManager::GetInstance()->GetChunkSize(): "<< CNtlWorldPathEngineManager::GetInstance()->GetChunkSize());
	SetChunkSize(CNtlWorldPathEngineManager::GetInstance()->GetChunkSize());
}

template <class CBObjClass>
CNtlWorldBrushTileEx<CBObjClass>::~CNtlWorldBrushTileEx()
{
	m_vecTileIdx.clear();
}

template <class CBObjClass>
VOID CNtlWorldBrushTileEx<CBObjClass>::SetChunkSize(RwInt32 iChunkSize)
{
	m_iChunkSize	= iChunkSize;

	m_vecTileIdx.clear();
	m_vecTileIdx.reserve(4* dGET_WORLD_PARAM()->WorldFieldSectorNum * CNtlWorldPathEngineManager::GetInstance()->GetChunkNum() * dGET_WORLD_PARAM()->WorldFieldSectorNum * CNtlWorldPathEngineManager::GetInstance()->GetChunkNum());
}

template <class CBObjClass>
VOID CNtlWorldBrushTileEx<CBObjClass>::Update(RwInt32 SLmtX, RwInt32 ELmtX, RwInt32 SLmtZ, RwInt32 ELmtZ)
{
	m_IdxSLmtX = SLmtX;
	m_IdxELmtX = ELmtX;
	m_IdxSLmtZ = SLmtZ;
	m_IdxELmtZ = ELmtZ;

	RwInt32 CntX;
	RwInt32 CntZ;
	RwInt32 NumWorldTile = dGET_WORLD_PARAM()->WorldSize / m_iChunkSize;

	vector<RwInt32>::iterator iter = m_vecTileIdx.begin(); 
	while(iter != m_vecTileIdx.end())
	{
		CntX = *iter - ((*iter / NumWorldTile) * NumWorldTile);
		CntZ = *iter / NumWorldTile;

		if(CntX > m_IdxELmtX || CntX < m_IdxSLmtX || CntZ < m_IdxSLmtZ || CntZ > m_IdxELmtZ)
		{
			iter = m_vecTileIdx.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

template <class CBObjClass>
RwReal CNtlWorldBrushTileEx<CBObjClass>::GetHeight(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWorldBrushTile<CBObjClass>::GetHeight");
	NTL_ASSERTE(m_pObj);
	NTL_ASSERTE(m_pFunc);

	NTL_RETURN((m_pObj->*m_pFunc)(Pos));  
}

template <class CBObjClass>
VOID CNtlWorldBrushTileEx<CBObjClass>::RegisterTileIdx(RwInt32 IdxX, RwInt32 IdxZ, RwBool _DeleteTile)
{
	if(IdxX > m_IdxELmtX || IdxX < m_IdxSLmtX || IdxZ < m_IdxSLmtZ || IdxZ > m_IdxELmtZ)
	{
		return;
	}

	RwInt32 NumTile	= dGET_WORLD_PARAM()->WorldSize / m_iChunkSize;
	RwInt32 IdxTile = IdxX + IdxZ * NumTile;

	std::vector<RwInt32>::iterator iter = find(m_vecTileIdx.begin(), m_vecTileIdx.end(), IdxTile);

	if(!_DeleteTile)
	{	
		if(iter == m_vecTileIdx.end())
		{
			m_vecTileIdx.push_back(IdxTile);
		}
	}
	else
	{
		if(iter != m_vecTileIdx.end())
		{
			m_vecTileIdx.erase(iter);
		}
	}
}

template <class CBObjClass>
void CNtlWorldBrushTileEx<CBObjClass>::SetPosS(RwV3d& Pos)
{
	RwInt32 CntX	= static_cast<RwInt32>((Pos.x + dGET_WORLD_PARAM()->WorldSizeHalf) / m_iChunkSize);
	RwInt32 CntZ	= static_cast<RwInt32>((Pos.z + dGET_WORLD_PARAM()->WorldSizeHalf) / m_iChunkSize);

	m_IdxSX = CntX;
	m_IdxSZ = CntZ;
}

template <class CBObjClass>
void CNtlWorldBrushTileEx<CBObjClass>::SetPosE(RwV3d& Pos, RwBool _DeleteTile)
{
	RwInt32 CntX	= static_cast<RwInt32>((Pos.x + dGET_WORLD_PARAM()->WorldSizeHalf) / m_iChunkSize);
	RwInt32 CntZ	= static_cast<RwInt32>((Pos.z + dGET_WORLD_PARAM()->WorldSizeHalf) / m_iChunkSize);

	m_IdxEX = CntX;
	m_IdxEZ = CntZ;

	if(m_IdxSZ < m_IdxEZ)
	{
		for(RwInt32 i = m_IdxSZ; i <= m_IdxEZ; ++i)
		{
			if(m_IdxSX < m_IdxEX)
			{
				for(RwInt32 j = m_IdxSX; j <= m_IdxEX; ++j)
				{
					RegisterTileIdx(j, i, _DeleteTile);
				}
			}
			else if(m_IdxSX > m_IdxEX)
			{
				for(RwInt32 j = m_IdxEX; j <= m_IdxSX; ++j)
				{
					RegisterTileIdx(j, i, _DeleteTile);
				}
			}
			else
			{
				// m_IdxSX == m_IdxEX
				RegisterTileIdx(m_IdxSX, i, _DeleteTile);
			}
		}
	}
	else if(m_IdxSZ > m_IdxEZ)
	{
		for(RwInt32 i = m_IdxEZ; i <= m_IdxSZ; ++i)
		{
			if(m_IdxSX < m_IdxEX)
			{
				for(RwInt32 j = m_IdxSX; j <= m_IdxEX; ++j)
				{
					RegisterTileIdx(j, i, _DeleteTile);
				}
			}
			else if(m_IdxSX > m_IdxEX)
			{
				for(RwInt32 j = m_IdxEX; j <= m_IdxSX; ++j)
				{
					RegisterTileIdx(j, i, _DeleteTile);
				}
			}
			else
			{
				// m_IdxSX == m_IdxEX
				RegisterTileIdx(m_IdxSX, i, _DeleteTile);
			}
		}
	}
	else
	{
		if(m_IdxSX < m_IdxEX)
		{
			for(RwInt32 j = m_IdxSX; j <= m_IdxEX; ++j)
			{
				// m_IdxSZ == m_IdxEZ
				RegisterTileIdx(j, m_IdxSZ, _DeleteTile);
			}
		}
		else if(m_IdxSX > m_IdxEX)
		{
			for(RwInt32 j = m_IdxEX; j <= m_IdxSX; ++j)
			{
				// m_IdxSZ == m_IdxEZ
				RegisterTileIdx(j, m_IdxSZ, _DeleteTile);
			}
		}
		else
		{
			// m_IdxSX == m_IdxEX
			// m_IdxSZ == m_IdxEZ
			RegisterTileIdx(m_IdxSX, m_IdxSZ, _DeleteTile);
		}
	}
}

template <class CBObjClass>
void CNtlWorldBrushTileEx<CBObjClass>::Render()
{
	RwInt32 CntX;
	RwInt32 CntZ;
	RwInt32 NumWorldTile	= dGET_WORLD_PARAM()->WorldSize / m_iChunkSize;
	RwReal	LenTile			= static_cast<RwReal>(m_iChunkSize);
	RwV3d	PosRB, PosLT, PosLB, PosRT;

	for(RwUInt32 i = 0; i < m_vecTileIdx.size(); ++i)
	{
		CntX = m_vecTileIdx[i] - ((m_vecTileIdx[i] / NumWorldTile) * NumWorldTile);
		CntZ = m_vecTileIdx[i] / NumWorldTile;
		PosRB.x = CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf;
		PosRB.z = CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf;
		PosRB.y = GetHeight(PosRB) + 1.0f;
		PosLT.x = PosRB.x + LenTile;
		PosLT.z = PosRB.z + LenTile;
		PosLT.y = PosRB.y; //GetHeight(PosLT) + 1.0f;
		PosLB.x = PosRB.x + LenTile;
		PosLB.z = PosRB.z;
		PosLB.y = PosRB.y; //GetHeight(PosLB) + 1.0f;
		PosRT.x = PosRB.x;
		PosRT.z = PosRB.z + LenTile;
		PosRT.y = PosRB.y; //GetHeight(PosRT) + 1.0f;

		// 0  2
		// 1  3
		RwIm3DVertexSetU(&m_VB[0], 0.0f);		
		RwIm3DVertexSetV(&m_VB[0], 0.0f);
		RwIm3DVertexSetU(&m_VB[1], 0.0f);		
		RwIm3DVertexSetV(&m_VB[1], 1.0f);
		RwIm3DVertexSetU(&m_VB[2], 1.0f);		
		RwIm3DVertexSetV(&m_VB[2], 0.0f);
		RwIm3DVertexSetU(&m_VB[3], 1.0f);		
		RwIm3DVertexSetV(&m_VB[3], 1.0f);
		RwIm3DVertexSetRGBA(&m_VB[0], 255, 255, 255, 255);
		RwIm3DVertexSetRGBA(&m_VB[1], 255, 255, 255, 255);
		RwIm3DVertexSetRGBA(&m_VB[2], 255, 255, 255, 255);
		RwIm3DVertexSetRGBA(&m_VB[3], 255, 255, 255, 255);
		RwIm3DVertexSetPos(&m_VB[0], PosLT.x, PosLT.y, PosLT.z);
		RwIm3DVertexSetPos(&m_VB[1], PosLB.x, PosLB.y, PosLB.z);
		RwIm3DVertexSetPos(&m_VB[2], PosRT.x, PosRT.y, PosRT.z);
		RwIm3DVertexSetPos(&m_VB[3], PosRB.x, PosRB.y, PosRB.z);

		if(RwIm3DTransform(m_VB, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);
			RwIm3DEnd();
		}
	}
}

// #pragma once
// 
// 
// #include "NtlWorldBrushController.h"
// 
// 
// template <class CBObjClass>
// class CNtlWorldBrushTileEx22 : public CNtlWorldBrushController
// {
// private:
// 	CBObjClass* m_pObj;  
// 	RwReal (CBObjClass::*m_pFunc)(RwV3d& Pos);
// 
// protected:
// 	RwReal GetHeight(RwV3d& Pos);
// 
// public:
// 	CNtlWorldBrushTileEx2(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos));
// 	~CNtlWorldBrushTileEx2();
// 
// private:
// 	vector<RwInt32> m_vecTileIdx;
// 	RwIm3DVertex	m_VB[4];
// 	RwInt32			m_IdxSX, m_IdxEX;
// 	RwInt32			m_IdxSZ, m_IdxEZ;
// 	RwBool			m_UpdateVB;
// 	RwInt32			m_IdxSLmtX, m_IdxELmtX;
// 	RwInt32			m_IdxSLmtZ, m_IdxELmtZ;
// 
// 	RwInt32			m_NumX, m_NumZ;
// 
// 	RwInt32			m_iChunkSize;
// 
// 	RwIm3DVertex*	m_pVertices;
// 
// private:
// 	VOID RegisterTileIdx(RwInt32 IdxX, RwInt32 IdxZ, RwBool _DeleteTile);
// 
// public:
// 	vector<RwInt32>&	GetTileContainer() { return m_vecTileIdx; }
// 	RwUInt32			GetTileNum() { return m_vecTileIdx.size(); }
// 	void				SetPosS(RwV3d& Pos);
// 	void				SetPosE(RwV3d& Pos, RwBool _DeleteTile = FALSE);
// 
// 	void				SetChunkSize(RwInt32 iChunkSize);
// 	RwInt32				GetChunkSize() { return m_iChunkSize; }
// 
// 	void	Clear() { m_vecTileIdx.clear(); }
// 	VOID	Update(RwInt32 SLmtX, RwInt32 ELmtX, RwInt32 SLmtZ, RwInt32 ELmtZ);
// 	void	Render();
// };
// 
// template <class CBObjClass>
// CNtlWorldBrushTileEx2<CBObjClass>::CNtlWorldBrushTileEx2(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos))
// {
// 	NTL_ASSERTE(pObj);
// 	NTL_ASSERTE(pFunc);
// 
// 	m_pObj			= pObj; 
// 	m_pFunc			= pFunc;
// 	m_pVertices		= NULL;
// 	m_UpdateVB		= TRUE;
// 
// 	SetChunkSize(CNtlWorldPathEngineManager::GetInstance()->GetChunkSize());
// }
// 
// template <class CBObjClass>
// CNtlWorldBrushTileEx2<CBObjClass>::~CNtlWorldBrushTileEx2()
// {
// 	if (m_pVertices)
// 	{
// 		NTL_ARRAY_DELETE(m_pVertices);
// 	}
// 	m_vecTileIdx.clear();
// }
// 
// template <class CBObjClass>
// VOID CNtlWorldBrushTileEx2<CBObjClass>::SetChunkSize(RwInt32 iChunkSize)
// {
// 	m_iChunkSize	= iChunkSize;
// 
// 	m_vecTileIdx.clear();
// 	m_vecTileIdx.reserve(4* dGET_WORLD_PARAM()->WorldFieldSectorNum * CNtlWorldPathEngineManager::GetInstance()->GetChunkNum() * dGET_WORLD_PARAM()->WorldFieldSectorNum * CNtlWorldPathEngineManager::GetInstance()->GetChunkNum());
// }
// 
// template <class CBObjClass>
// VOID CNtlWorldBrushTileEx2<CBObjClass>::Update(RwInt32 SLmtX, RwInt32 ELmtX, RwInt32 SLmtZ, RwInt32 ELmtZ)
// {
// 	m_IdxSLmtX = SLmtX;
// 	m_IdxELmtX = ELmtX;
// 	m_IdxSLmtZ = SLmtZ;
// 	m_IdxELmtZ = ELmtZ;
// 
// 	RwInt32 CntX;
// 	RwInt32 CntZ;
// 	RwInt32 NumWorldTile = dGET_WORLD_PARAM()->WorldSize / m_iChunkSize;
// 
// 	if (m_pVertices)
// 	{
// 		NTL_ARRAY_DELETE(m_pVertices);
// 	}
// 
// 	if (SLmtX < ELmtX)
// 	{
// 		m_NumX = ELmtX - SLmtX;
// 	}
// 	else
// 	{
// 		m_NumX = SLmtX - ELmtX;
// 	}
// 	if (SLmtZ < ELmtZ)
// 	{
// 		m_NumZ = ELmtZ - SLmtZ;
// 	}
// 	else
// 	{
// 		m_NumZ = SLmtZ - ELmtZ;
// 	}
// 
// 	if (m_NumX * m_NumZ != 0)
// 	{
// 		m_pVertices = NTL_NEW RwIm3DVertex [ m_NumX * m_NumZ * 6 ];
// 	}
// 
// 	vector<RwInt32>::iterator iter = m_vecTileIdx.begin(); 
// 	while(iter != m_vecTileIdx.end())
// 	{
// 		CntX = *iter - ((*iter / NumWorldTile) * NumWorldTile);
// 		CntZ = *iter / NumWorldTile;
// 
// 		if(CntX > m_IdxELmtX || CntX < m_IdxSLmtX || CntZ < m_IdxSLmtZ || CntZ > m_IdxELmtZ)
// 		{
// 			iter = m_vecTileIdx.erase(iter);
// 		}
// 		else
// 		{
// 			iter++;
// 		}
// 	}
// 
// 	m_UpdateVB = TRUE;
// }
// 
// template <class CBObjClass>
// RwReal CNtlWorldBrushTileEx2<CBObjClass>::GetHeight(RwV3d& Pos)
// {
// 	NTL_FUNCTION("CNtlWorldBrushTile<CBObjClass>::GetHeight");
// 	NTL_ASSERTE(m_pObj);
// 	NTL_ASSERTE(m_pFunc);
// 
// 	NTL_RETURN((m_pObj->*m_pFunc)(Pos));  
// }
// 
// template <class CBObjClass>
// VOID CNtlWorldBrushTileEx2<CBObjClass>::RegisterTileIdx(RwInt32 IdxX, RwInt32 IdxZ, RwBool _DeleteTile)
// {
// 	if(IdxX > m_IdxELmtX || IdxX < m_IdxSLmtX || IdxZ < m_IdxSLmtZ || IdxZ > m_IdxELmtZ)
// 	{
// 		return;
// 	}
// 
// 	RwInt32 NumTile	= dGET_WORLD_PARAM()->WorldSize / m_iChunkSize;
// 	RwInt32 IdxTile = IdxX + IdxZ * NumTile;
// 
// 	std::vector<RwInt32>::iterator iter = find(m_vecTileIdx.begin(), m_vecTileIdx.end(), IdxTile);
// 
// 	if(!_DeleteTile)
// 	{	
// 		if(iter == m_vecTileIdx.end())
// 		{
// 			m_vecTileIdx.push_back(IdxTile);
// 		}
// 	}
// 	else
// 	{
// 		if(iter != m_vecTileIdx.end())
// 		{
// 			m_vecTileIdx.erase(iter);
// 		}
// 	}
// 
// 	m_UpdateVB = TRUE;
// }
// 
// template <class CBObjClass>
// void CNtlWorldBrushTileEx2<CBObjClass>::SetPosS(RwV3d& Pos)
// {
// 	RwInt32 CntX	= static_cast<RwInt32>((Pos.x + dGET_WORLD_PARAM()->WorldSizeHalf) / m_iChunkSize);
// 	RwInt32 CntZ	= static_cast<RwInt32>((Pos.z + dGET_WORLD_PARAM()->WorldSizeHalf) / m_iChunkSize);
// 
// 	m_IdxSX = CntX;
// 	m_IdxSZ = CntZ;
// }
// 
// template <class CBObjClass>
// void CNtlWorldBrushTileEx2<CBObjClass>::SetPosE(RwV3d& Pos, RwBool _DeleteTile)
// {
// 	RwInt32 CntX	= static_cast<RwInt32>((Pos.x + dGET_WORLD_PARAM()->WorldSizeHalf) / m_iChunkSize);
// 	RwInt32 CntZ	= static_cast<RwInt32>((Pos.z + dGET_WORLD_PARAM()->WorldSizeHalf) / m_iChunkSize);
// 
// 	m_IdxEX = CntX;
// 	m_IdxEZ = CntZ;
// 
// 	if(m_IdxSZ < m_IdxEZ)
// 	{
// 		for(RwInt32 i = m_IdxSZ; i <= m_IdxEZ; ++i)
// 		{
// 			if(m_IdxSX < m_IdxEX)
// 			{
// 				for(RwInt32 j = m_IdxSX; j <= m_IdxEX; ++j)
// 				{
// 					RegisterTileIdx(j, i, _DeleteTile);
// 				}
// 			}
// 			else if(m_IdxSX > m_IdxEX)
// 			{
// 				for(RwInt32 j = m_IdxEX; j <= m_IdxSX; ++j)
// 				{
// 					RegisterTileIdx(j, i, _DeleteTile);
// 				}
// 			}
// 			else
// 			{
// 				// m_IdxSX == m_IdxEX
// 				RegisterTileIdx(m_IdxSX, i, _DeleteTile);
// 			}
// 		}
// 	}
// 	else if(m_IdxSZ > m_IdxEZ)
// 	{
// 		for(RwInt32 i = m_IdxEZ; i <= m_IdxSZ; ++i)
// 		{
// 			if(m_IdxSX < m_IdxEX)
// 			{
// 				for(RwInt32 j = m_IdxSX; j <= m_IdxEX; ++j)
// 				{
// 					RegisterTileIdx(j, i, _DeleteTile);
// 				}
// 			}
// 			else if(m_IdxSX > m_IdxEX)
// 			{
// 				for(RwInt32 j = m_IdxEX; j <= m_IdxSX; ++j)
// 				{
// 					RegisterTileIdx(j, i, _DeleteTile);
// 				}
// 			}
// 			else
// 			{
// 				// m_IdxSX == m_IdxEX
// 				RegisterTileIdx(m_IdxSX, i, _DeleteTile);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if(m_IdxSX < m_IdxEX)
// 		{
// 			for(RwInt32 j = m_IdxSX; j <= m_IdxEX; ++j)
// 			{
// 				// m_IdxSZ == m_IdxEZ
// 				RegisterTileIdx(j, m_IdxSZ, _DeleteTile);
// 			}
// 		}
// 		else if(m_IdxSX > m_IdxEX)
// 		{
// 			for(RwInt32 j = m_IdxEX; j <= m_IdxSX; ++j)
// 			{
// 				// m_IdxSZ == m_IdxEZ
// 				RegisterTileIdx(j, m_IdxSZ, _DeleteTile);
// 			}
// 		}
// 		else
// 		{
// 			// m_IdxSX == m_IdxEX
// 			// m_IdxSZ == m_IdxEZ
// 			RegisterTileIdx(m_IdxSX, m_IdxSZ, _DeleteTile);
// 		}
// 	}
// }
// 
// template <class CBObjClass>
// void CNtlWorldBrushTileEx2<CBObjClass>::Render()
// {
// 	if (m_pVertices == NULL || m_vecTileIdx.empty())
// 	{
// 		return;
// 	}
// 
// 	RwInt32 CntX;
// 	RwInt32 CntZ;
// 	RwInt32 NumWorldTile	= dGET_WORLD_PARAM()->WorldSize / m_iChunkSize;
// 	RwReal	LenTile			= static_cast<RwReal>(m_iChunkSize);
// 	RwV3d	PosRB, PosLT, PosLB, PosRT;
// 
// 	if (m_UpdateVB)
// 	{
// 		for(RwUInt32 i = 0; i < m_vecTileIdx.size(); ++i)
// 		{
// 			CntX = m_vecTileIdx[i] - ((m_vecTileIdx[i] / NumWorldTile) * NumWorldTile);
// 			CntZ = m_vecTileIdx[i] / NumWorldTile;
// 			PosRB.x = CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf;
// 			PosRB.z = CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf;
// 			PosRB.y = GetHeight(PosRB) + 1.0f;
// 			PosLT.x = PosRB.x + LenTile;
// 			PosLT.z = PosRB.z + LenTile;
// 			PosLT.y = PosRB.y; //GetHeight(PosLT) + 1.0f;
// 			PosLB.x = PosRB.x + LenTile;
// 			PosLB.z = PosRB.z;
// 			PosLB.y = PosRB.y; //GetHeight(PosLB) + 1.0f;
// 			PosRT.x = PosRB.x;
// 			PosRT.z = PosRB.z + LenTile;
// 			PosRT.y = PosRB.y; //GetHeight(PosRT) + 1.0f;
// 
// 			// 0  2
// 			// 1  3
// 			RwIm3DVertexSetU(&m_pVertices[i * 6 + 0], 0.0f);		
// 			RwIm3DVertexSetV(&m_pVertices[i * 6 + 0], 0.0f);
// 			RwIm3DVertexSetU(&m_pVertices[i * 6 + 1], 0.0f);		
// 			RwIm3DVertexSetV(&m_pVertices[i * 6 + 1], 1.0f);
// 			RwIm3DVertexSetU(&m_pVertices[i * 6 + 2], 1.0f);		
// 			RwIm3DVertexSetV(&m_pVertices[i * 6 + 2], 0.0f);
// 			RwIm3DVertexSetRGBA(&m_pVertices[i * 6 + 0], 255, 255, 255, 255);
// 			RwIm3DVertexSetRGBA(&m_pVertices[i * 6 + 1], 255, 255, 255, 255);
// 			RwIm3DVertexSetRGBA(&m_pVertices[i * 6 + 2], 255, 255, 255, 255);
// 			RwIm3DVertexSetPos(&m_pVertices[i * 6 + 0], PosLT.x, PosLT.y, PosLT.z);
// 			RwIm3DVertexSetPos(&m_pVertices[i * 6 + 1], PosLB.x, PosLB.y, PosLB.z);
// 			RwIm3DVertexSetPos(&m_pVertices[i * 6 + 2], PosRT.x, PosRT.y, PosRT.z);
// 
// 			RwIm3DVertexSetU(&m_pVertices[i * 6 + 3], 1.0f);		
// 			RwIm3DVertexSetV(&m_pVertices[i * 6 + 3], 1.0f);
// 			RwIm3DVertexSetU(&m_pVertices[i * 6 + 4], 1.0f);		
// 			RwIm3DVertexSetV(&m_pVertices[i * 6 + 4], 0.0f);
// 			RwIm3DVertexSetU(&m_pVertices[i * 6 + 5], 0.0f);		
// 			RwIm3DVertexSetV(&m_pVertices[i * 6 + 5], 1.0f);
// 
// 			RwIm3DVertexSetRGBA(&m_pVertices[i * 6 + 3], 255, 255, 255, 255);
// 			RwIm3DVertexSetRGBA(&m_pVertices[i * 6 + 4], 255, 255, 255, 255);
// 			RwIm3DVertexSetRGBA(&m_pVertices[i * 6 + 5], 255, 255, 255, 255);			
// 			RwIm3DVertexSetPos(&m_pVertices[i * 6 + 3], PosRB.x, PosRB.y, PosRB.z);
// 			RwIm3DVertexSetPos(&m_pVertices[i * 6 + 4], PosRT.x, PosRT.y, PosRT.z);
// 			RwIm3DVertexSetPos(&m_pVertices[i * 6 + 5], PosLB.x, PosLB.y, PosLB.z);			
// 		}
// 		m_UpdateVB = FALSE;
// 	}
// 
// 	if(RwIm3DTransform(m_pVertices, (int)m_vecTileIdx.size() * 6, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
// 	{
// 		RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
// 		RwIm3DEnd();
// 	}
// }