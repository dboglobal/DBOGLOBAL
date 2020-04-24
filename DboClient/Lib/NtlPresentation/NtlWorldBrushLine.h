#pragma once


#include "NtlWorldBrushController.h"
#include "NtlMath.h"


using std::vector;
#include <vector>


template <class CBObjClass>
class CNtlWB_Line : public CNtlWorldBrushController
{
private:
	CBObjClass* m_pObj;
	RwReal (CBObjClass::*m_pFunc)(RwV3d& Pos);

private:
	RwV3d		m_PosS, m_PosE;
	D3DCOLOR	m_ClrLine;

protected:
	RwReal GetHeight(RwV3d& Pos);

public:
	CNtlWB_Line(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos));
	~CNtlWB_Line();

public:
	void	SetPosS(RwV3d& Pos);
	void	SetPosE(RwV3d& Pos);
	RwV3d&	GetPosS();
	RwV3d&	GetPosE();

public:
	void Render();
	void RenderEx();
};

template <class CBObjClass>
CNtlWB_Line<CBObjClass>::~CNtlWB_Line()
{
}

template <class CBObjClass>
CNtlWB_Line<CBObjClass>::CNtlWB_Line(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos))
{
	NTL_ASSERTE(pObj);
	NTL_ASSERTE(pFunc);

	m_pObj	= pObj; 
	m_pFunc	= pFunc;

	m_ID		= ID;
	m_ClrLine	= 0xff000000;

	RwV3d Temp = {0.0f, 0.0f, 0.0f};
	m_PosS = m_PosE = Temp;
}

template <class CBObjClass>
RwV3d& CNtlWB_Line<CBObjClass>::GetPosS()
{
	NTL_FUNCTION("CNtlWB_Line<CBObjClass>::GetPosS");

	NTL_RETURN(m_PosS);
}

template <class CBObjClass>
RwV3d& CNtlWB_Line<CBObjClass>::GetPosE()
{
	NTL_FUNCTION("CNtlWB_Line<CBObjClass>::GetPosE");

	NTL_RETURN(m_PosE);
}

template <class CBObjClass>
void CNtlWB_Line<CBObjClass>::SetPosS(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWB_Line<CBObjClass>::SetPosS");

	//m_PosS.x = (static_cast<RwInt32>((Pos.x + CNtlWorldBrushController::m_ExtWorldHSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize + m_PosWorldMin.x;
	//m_PosS.z = (static_cast<RwInt32>((Pos.z + CNtlWorldBrushController::m_ExtWorldHSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize + m_PosWorldMin.y;

	m_PosS = Pos;
	m_PosS.y += 0.05f;

	NTL_RETURNVOID();
}

template <class CBObjClass>
void CNtlWB_Line<CBObjClass>::SetPosE(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWB_Line<CBObjClass>::SetPosE");

	//m_PosE.x = (static_cast<RwInt32>((Pos.x + CNtlWorldBrushController::m_ExtWorldHSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize + m_PosWorldMin.x;
	//m_PosE.z = (static_cast<RwInt32>((Pos.z + CNtlWorldBrushController::m_ExtWorldHSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize + m_PosWorldMin.y;

	m_PosE = Pos; 
	m_PosE.y += 0.05f;

	NTL_RETURNVOID();
}

template <class CBObjClass>
RwReal CNtlWB_Line<CBObjClass>::GetHeight(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWB_Line<CBObjClass>::GetHeight");

	NTL_RETURN((m_pObj->*m_pFunc)(Pos));  
}

template <class CBObjClass>
void CNtlWB_Line<CBObjClass>::Render()
{
	sLINE3D				BrushLine[2];
	D3DXMATRIXA16		mIdentity;

	BeginD3DLineState();

	RwD3D9SetFVF(D3DFVF_LINE3D);

	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	BrushLine[0].Col = m_ClrLine;
	BrushLine[1].Col = m_ClrLine;
	BrushLine[0].Pos.x = m_PosS.x;
	BrushLine[0].Pos.y = m_PosS.y;
	BrushLine[0].Pos.z = m_PosS.z;
	BrushLine[1].Pos.x = m_PosE.x;
	BrushLine[1].Pos.y = m_PosE.y;
	BrushLine[1].Pos.z = m_PosE.z;

	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

	EndD3DLineState();	
}

template <class CBObjClass>
void CNtlWB_Line<CBObjClass>::RenderEx()
{
	sLINE3D				BrushLine[2];
	D3DXMATRIXA16		mIdentity;

	BeginD3DLineState();

	RwD3D9SetFVF(D3DFVF_LINE3D);

	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	RwReal fDistance = RwV3dLength(&(m_PosE - m_PosS));

	RwV3d vDir = m_PosE - m_PosS;
	RwV3dNormalize(&vDir, &vDir);

	RwV3d vTmp;
	BrushLine[0].Col = 0xffffffff;
	BrushLine[1].Col = 0xffffffff;
	BrushLine[0].Pos.x = m_PosS.x;
	BrushLine[0].Pos.z = m_PosS.z;
	vTmp.x = BrushLine[0].Pos.x;
	vTmp.z = BrushLine[0].Pos.z; 
	BrushLine[0].Pos.y = GetHeight(vTmp);
	BrushLine[1].Pos.x = BrushLine[0].Pos.x;
	BrushLine[1].Pos.y = BrushLine[0].Pos.y;
	BrushLine[1].Pos.z = BrushLine[0].Pos.z;

	RwReal AddConst = 0.2f;
	for(RwReal i = 0.0f; i < fDistance; i += AddConst)
	{
		vTmp.x = BrushLine[1].Pos.x;
		vTmp.y = BrushLine[1].Pos.y;
		vTmp.z = BrushLine[1].Pos.z;
		RwV3dIncrementScaled(&vTmp, &vDir, AddConst);

		BrushLine[0].Pos.x = BrushLine[1].Pos.x;
		BrushLine[0].Pos.y = BrushLine[1].Pos.y;
		BrushLine[0].Pos.z = BrushLine[1].Pos.z;

		BrushLine[1].Pos.x = vTmp.x;
		BrushLine[1].Pos.z = vTmp.z;
		BrushLine[1].Pos.y = GetHeight(vTmp);

		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));
	}

	BrushLine[0].Pos.x = BrushLine[1].Pos.x;
	BrushLine[0].Pos.y = BrushLine[1].Pos.y;
	BrushLine[0].Pos.z = BrushLine[1].Pos.z;
	BrushLine[1].Pos.x = m_PosE.x;
	BrushLine[1].Pos.z = m_PosE.z;
	BrushLine[1].Pos.y = GetHeight(m_PosE);

	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

	EndD3DLineState();	
}

////////////////////////////////////////////////////////////////////////////////////////////////////