#pragma once


////////////////////////////////////////////////////////////////////////////////////////////////////


#include "NtlWorldBrushController.h"
#include "NtlMath.h"
#include "NtlPLRenderState.h"

using std::vector;
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////


template <class CBObjClass>
class CNtlWB_RectangleInDetail : public CNtlWorldBrushController
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
	CNtlWB_RectangleInDetail(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos));
	~CNtlWB_RectangleInDetail();

public:
	void	SetPosS(RwV3d& Pos);
	void	SetPosE(RwV3d& Pos);
	RwV3d&	GetPosS();
	RwV3d&	GetPosE();

public:
	void Render();
};

template <class CBObjClass>
CNtlWB_RectangleInDetail<CBObjClass>::~CNtlWB_RectangleInDetail()
{
}

template <class CBObjClass>
CNtlWB_RectangleInDetail<CBObjClass>::CNtlWB_RectangleInDetail(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos))
{
	NTL_ASSERTE(pObj);
	NTL_ASSERTE(pFunc);

	m_pObj	= pObj; 
	m_pFunc	= pFunc;

	m_ID		= ID;
	m_ClrLine	= 0xffffffff;

	RwV3d Temp = {0.0f, 0.0f, 0.0f};
	m_PosS = m_PosE = Temp;
}

template <class CBObjClass>
RwV3d& CNtlWB_RectangleInDetail<CBObjClass>::GetPosS()
{
	NTL_FUNCTION("CNtlWB_RectangleInDetail<CBObjClass>::GetPosS");

	NTL_RETURN(m_PosS);
}

template <class CBObjClass>
RwV3d& CNtlWB_RectangleInDetail<CBObjClass>::GetPosE()
{
	NTL_FUNCTION("CNtlWB_RectangleInDetail<CBObjClass>::GetPosE");

	NTL_RETURN(m_PosE);
}

template <class CBObjClass>
void CNtlWB_RectangleInDetail<CBObjClass>::SetPosS(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWB_RectangleInDetail<CBObjClass>::SetPosS");

	//m_PosS.x = (static_cast<RwInt32>((Pos.x + CNtlWorldBrushController::m_ExtWorldHSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize + m_PosWorldMin.x;
	//m_PosS.z = (static_cast<RwInt32>((Pos.z + CNtlWorldBrushController::m_ExtWorldHSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize + m_PosWorldMin.y;

	m_PosS	= Pos;
	m_PosE	= m_PosS;

	NTL_RETURNVOID();
}

template <class CBObjClass>
void CNtlWB_RectangleInDetail<CBObjClass>::SetPosE(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWB_RectangleInDetail<CBObjClass>::SetPosE");

	m_PosE = Pos;
	//m_PosE.x = (static_cast<RwInt32>((Pos.x + CNtlWorldBrushController::m_ExtWorldHSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize + m_PosWorldMin.x;
	//m_PosE.z = (static_cast<RwInt32>((Pos.z + CNtlWorldBrushController::m_ExtWorldHSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize + m_PosWorldMin.y;

	NTL_RETURNVOID();
}

template <class CBObjClass>
RwReal CNtlWB_RectangleInDetail<CBObjClass>::GetHeight(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWB_RectangleInDetail<CBObjClass>::GetHeight");

	NTL_RETURN((m_pObj->*m_pFunc)(Pos));  
}

template <class CBObjClass>
void CNtlWB_RectangleInDetail<CBObjClass>::Render()
{
	sLINE3D				BrushLine[2];
	RwV3d				Temp;
	RwInt32				ExtTile = 1;
	D3DXMATRIXA16		mIdentity;

	BrushLine[0].Col = m_ClrLine;
	BrushLine[1].Col = m_ClrLine;		

	RwBool XIncreasePriority = FALSE;
	RwBool ZIncreasePriority = FALSE;
	if(m_PosS.x > m_PosE.x)	XIncreasePriority = TRUE;
	if(m_PosS.z > m_PosE.z)	ZIncreasePriority = TRUE;

	BeginD3DLineState();

	RwD3D9SetFVF(D3DFVF_LINE3D);

	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	if(XIncreasePriority)
	{
		// top
		BrushLine[0].Pos.x = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.x));
		BrushLine[0].Pos.z = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.z));
		CNtlMath::MathRwV3dAssign(&Temp, BrushLine[0].Pos.x, 0.0f, BrushLine[0].Pos.z);
		BrushLine[0].Pos.y = GetHeight(Temp) + m_Seam2Surface;

		for(RwInt32 i = static_cast<RwInt32>(m_PosS.x); i >= static_cast<RwInt32>(m_PosE.x); i -= ExtTile)
		{
			BrushLine[1].Pos.x = static_cast<RwReal>(i);
			BrushLine[1].Pos.z = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.z));
			CNtlMath::MathRwV3dAssign(&Temp, BrushLine[1].Pos.x, 0.0f, BrushLine[1].Pos.z);
			BrushLine[1].Pos.y = GetHeight(Temp) + m_Seam2Surface;

			RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

			BrushLine[0].Pos = BrushLine[1].Pos;
		}

		// bottom
		BrushLine[0].Pos.x = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.x));
		BrushLine[0].Pos.z = static_cast<RwReal>(static_cast<RwInt32>(m_PosE.z));
		CNtlMath::MathRwV3dAssign(&Temp, BrushLine[0].Pos.x, 0.0f, BrushLine[0].Pos.z);
		BrushLine[0].Pos.y = GetHeight(Temp) + m_Seam2Surface;

		for(RwInt32 i = static_cast<RwInt32>(m_PosS.x); i >= static_cast<RwInt32>(m_PosE.x); i -= ExtTile)
		{
			BrushLine[1].Pos.x = static_cast<RwReal>(i);
			BrushLine[1].Pos.z = static_cast<RwReal>(static_cast<RwInt32>(m_PosE.z));
			CNtlMath::MathRwV3dAssign(&Temp, BrushLine[1].Pos.x, 0.0f, BrushLine[1].Pos.z);
			BrushLine[1].Pos.y = GetHeight(Temp) + m_Seam2Surface;

			RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

			BrushLine[0].Pos = BrushLine[1].Pos;
		}
	}
	else
	{
		// top
		BrushLine[0].Pos.x = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.x));
		BrushLine[0].Pos.z = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.z));
		CNtlMath::MathRwV3dAssign(&Temp, BrushLine[0].Pos.x, 0.0f, BrushLine[0].Pos.z);
		BrushLine[0].Pos.y = GetHeight(Temp) + m_Seam2Surface;

		for(RwInt32 i = static_cast<RwInt32>(m_PosS.x); i <= static_cast<RwInt32>(m_PosE.x); i += ExtTile)
		{
			BrushLine[1].Pos.x = static_cast<RwReal>(i);
			BrushLine[1].Pos.z = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.z));
			CNtlMath::MathRwV3dAssign(&Temp, BrushLine[1].Pos.x, 0.0f, BrushLine[1].Pos.z);
			BrushLine[1].Pos.y = GetHeight(Temp) + m_Seam2Surface;

			RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

			BrushLine[0].Pos = BrushLine[1].Pos;
		}

		// bottom
		BrushLine[0].Pos.x = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.x));
		BrushLine[0].Pos.z = static_cast<RwReal>(static_cast<RwInt32>(m_PosE.z));
		CNtlMath::MathRwV3dAssign(&Temp, BrushLine[0].Pos.x, 0.0f, BrushLine[0].Pos.z);
		BrushLine[0].Pos.y = GetHeight(Temp) + m_Seam2Surface;

		for(RwInt32 i = static_cast<RwInt32>(m_PosS.x); i <= static_cast<RwInt32>(m_PosE.x); i += ExtTile)
		{
			BrushLine[1].Pos.x = static_cast<RwReal>(i);
			BrushLine[1].Pos.z = static_cast<RwReal>(static_cast<RwInt32>(m_PosE.z));
			CNtlMath::MathRwV3dAssign(&Temp, BrushLine[1].Pos.x, 0.0f, BrushLine[1].Pos.z);
			BrushLine[1].Pos.y = GetHeight(Temp) + m_Seam2Surface;

			RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

			BrushLine[0].Pos = BrushLine[1].Pos;
		}
	}

	if(ZIncreasePriority)
	{
		// left
		BrushLine[0].Pos.x = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.x));
		BrushLine[0].Pos.z = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.z));
		CNtlMath::MathRwV3dAssign(&Temp, BrushLine[0].Pos.x, 0.0f, BrushLine[0].Pos.z);
		BrushLine[0].Pos.y = GetHeight(Temp) + m_Seam2Surface;

		for(RwInt32 i = static_cast<RwInt32>(m_PosS.z); i >= static_cast<RwInt32>(m_PosE.z); i -= ExtTile)
		{
			BrushLine[1].Pos.x = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.x));
			BrushLine[1].Pos.z = static_cast<RwReal>(i);
			CNtlMath::MathRwV3dAssign(&Temp, BrushLine[1].Pos.x, 0.0f, BrushLine[1].Pos.z);
			BrushLine[1].Pos.y = GetHeight(Temp) + m_Seam2Surface;

			RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

			BrushLine[0].Pos = BrushLine[1].Pos;
		}


		// right
		BrushLine[0].Pos.x = static_cast<RwReal>(static_cast<RwInt32>(m_PosE.x));
		BrushLine[0].Pos.z = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.z));
		CNtlMath::MathRwV3dAssign(&Temp, BrushLine[0].Pos.x, 0.0f, BrushLine[0].Pos.z);
		BrushLine[0].Pos.y = GetHeight(Temp) + m_Seam2Surface;

		for(RwInt32 i = static_cast<RwInt32>(m_PosS.z); i >= static_cast<RwInt32>(m_PosE.z); i -= ExtTile)
		{
			BrushLine[1].Pos.x = static_cast<RwReal>(static_cast<RwInt32>(m_PosE.x));
			BrushLine[1].Pos.z = static_cast<RwReal>(i);
			CNtlMath::MathRwV3dAssign(&Temp, BrushLine[1].Pos.x, 0.0f, BrushLine[1].Pos.z);
			BrushLine[1].Pos.y = GetHeight(Temp) + m_Seam2Surface;

			RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

			BrushLine[0].Pos = BrushLine[1].Pos;
		}
	}
	else
	{
		// left
		BrushLine[0].Pos.x = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.x));
		BrushLine[0].Pos.z = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.z));
		CNtlMath::MathRwV3dAssign(&Temp, BrushLine[0].Pos.x, 0.0f, BrushLine[0].Pos.z);
		BrushLine[0].Pos.y = GetHeight(Temp) + m_Seam2Surface;

		for(RwInt32 i = static_cast<RwInt32>(m_PosS.z); i <= static_cast<RwInt32>(m_PosE.z); i += ExtTile)
		{
			BrushLine[1].Pos.x = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.x));
			BrushLine[1].Pos.z = static_cast<RwReal>(i);
			CNtlMath::MathRwV3dAssign(&Temp, BrushLine[1].Pos.x, 0.0f, BrushLine[1].Pos.z);
			BrushLine[1].Pos.y = GetHeight(Temp) + m_Seam2Surface;

			RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

			BrushLine[0].Pos = BrushLine[1].Pos;
		}


		// right
		BrushLine[0].Pos.x = static_cast<RwReal>(static_cast<RwInt32>(m_PosE.x));
		BrushLine[0].Pos.z = static_cast<RwReal>(static_cast<RwInt32>(m_PosS.z));
		CNtlMath::MathRwV3dAssign(&Temp, BrushLine[0].Pos.x, 0.0f, BrushLine[0].Pos.z);
		BrushLine[0].Pos.y = GetHeight(Temp) + m_Seam2Surface;

		for(RwInt32 i = static_cast<RwInt32>(m_PosS.z); i <= static_cast<RwInt32>(m_PosE.z); i += ExtTile)
		{
			BrushLine[1].Pos.x = static_cast<RwReal>(static_cast<RwInt32>(m_PosE.x));
			BrushLine[1].Pos.z = static_cast<RwReal>(i);
			CNtlMath::MathRwV3dAssign(&Temp, BrushLine[1].Pos.x, 0.0f, BrushLine[1].Pos.z);
			BrushLine[1].Pos.y = GetHeight(Temp) + m_Seam2Surface;

			RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

			BrushLine[0].Pos = BrushLine[1].Pos;
		}
	}

	EndD3DLineState();
}


////////////////////////////////////////////////////////////////////////////////////////////////////