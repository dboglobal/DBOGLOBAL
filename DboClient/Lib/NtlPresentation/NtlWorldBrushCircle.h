#pragma once


////////////////////////////////////////////////////////////////////////////////////////////////////


#include "NtlWorldBrushController.h"
#include "NtlPLRenderState.h"

#include "NtlPLWorldState.h"

////////////////////////////////////////////////////////////////////////////////////////////////////


template <class CBObjClass>
class CNtlWB_Circle : public CNtlWorldBrushController
{
//CUSTOM_RTTI_SYSTEM_DECL;

private:
	CBObjClass* m_pObj;  
	RwReal (CBObjClass::*m_pFunc)(RwV3d& Pos);

protected:
	RwReal GetHeight(RwV3d& Pos);

public:
	CNtlWB_Circle(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos));

private:
	RwInt32		m_NumLine;
	D3DCOLOR	m_ClrLine;
	RwReal		m_StepInRadian;

public:
	void SetColor(D3DCOLOR Clr) { m_ClrLine = Clr; }
	void Render();
	void Render(RwV3d& PosMid, RwReal ExtLineTemp, RwV3d& Scale, RwInt32 iNumLine = 50);
};

template <class CBObjClass>
CNtlWB_Circle<CBObjClass>::CNtlWB_Circle(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos))
{
	NTL_ASSERTE(pObj);
	NTL_ASSERTE(pFunc);

	m_pObj	= pObj; 
	m_pFunc	= pFunc;

	m_ID			= ID;
	m_NumLine		= 50;
	m_ClrLine		= 0xffffffff;
	m_StepInRadian	= (D3DX_PI * 2.0f) / m_NumLine;
}

template <class CBObjClass>
RwReal CNtlWB_Circle<CBObjClass>::GetHeight(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWB_Circle<CBObjClass>::GetHeight");
	NTL_ASSERTE(m_pObj);
	NTL_ASSERTE(m_pFunc);

	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD || CNtlPLWorldState::GetActiveMiniIndoor())
	{
		NTL_RETURN(Pos.y);
	}

	NTL_RETURN((m_pObj->*m_pFunc)(Pos));  
}

template <class CBObjClass>
void CNtlWB_Circle<CBObjClass>::Render()
{
	sLINE3D			BrushLine[2];
	D3DXVECTOR3		NewPos;
	D3DXMATRIXA16	matRot, mIdentity;

	D3DXVECTOR3			Pos		= D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3			GetPos	= D3DXVECTOR3(	dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x,
												dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y,
												dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z);
	RwReal				ExtLine = dGET_WORLD_PARAM()->NumBrushRadiusInTile * dGET_WORLD_PARAM()->WorldSectorTileSize; 

	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	BrushLine[1].Pos	= Pos * ExtLine + GetPos;
	BrushLine[1].Col	= BrushLine[0].Col = m_ClrLine;

	RwV3d BrushCenterPos;
	BrushCenterPos.x = BrushLine[1].Pos.x;
	BrushCenterPos.y = BrushLine[1].Pos.y;
	BrushCenterPos.z = BrushLine[1].Pos.z;
	BrushLine[1].Pos.y = GetHeight(BrushCenterPos) + m_Seam2Surface;

	BeginD3DLineState();

	for(int i = 1; i < m_NumLine + 1; ++i)
	{
		BrushLine[0].Pos = BrushLine[1].Pos;

		D3DXMatrixRotationY(&matRot, i * m_StepInRadian);
		D3DXVec3TransformCoord(&NewPos, &Pos, &matRot);
		D3DXVec3Normalize(&NewPos, &NewPos);
		BrushLine[1].Pos = (NewPos * ExtLine) + GetPos;

		BrushCenterPos.x = BrushLine[1].Pos.x;
		BrushCenterPos.y = BrushLine[1].Pos.y;
		BrushCenterPos.z = BrushLine[1].Pos.z;
		BrushLine[1].Pos.y = GetHeight(BrushCenterPos) + m_Seam2Surface;

		RwD3D9SetFVF(D3DFVF_LINE3D);
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));
	}

	EndD3DLineState();	
}

template <class CBObjClass>
void CNtlWB_Circle<CBObjClass>::Render(RwV3d& PosMid, RwReal ExtLineTemp, RwV3d& Scale, RwInt32 iNumLine)
{
	sLINE3D			BrushLine[2];
	D3DXVECTOR3		NewPos;
	D3DXMATRIXA16	matRot, mIdentity;

	D3DXVECTOR3			Pos		= D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	RwReal				ExtLine = ExtLineTemp; 
	D3DXVECTOR3			GetPos;
	GetPos.x = PosMid.x;
	GetPos.y = PosMid.y; 
	GetPos.z = PosMid.z;

	ExtLine *= Scale.x;

	BrushLine[1].Pos	= Pos * ExtLine + GetPos;
	BrushLine[1].Col	= BrushLine[0].Col = m_ClrLine;

	RwV3d BrushCenterPos;
	BrushCenterPos.x = BrushLine[1].Pos.x;
	BrushCenterPos.y = BrushLine[1].Pos.y;
	BrushCenterPos.z = BrushLine[1].Pos.z;
	BrushLine[1].Pos.y = GetHeight(BrushCenterPos) + m_Seam2Surface;

	BeginD3DLineState();

	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	RwReal fStepInRadian = (D3DX_PI * 2.0f) / iNumLine;
	for(int i = 1; i < iNumLine + 1; ++i)
	{
		BrushLine[0].Pos = BrushLine[1].Pos;

		D3DXMatrixRotationY(&matRot, i * fStepInRadian);
		D3DXVec3TransformCoord(&NewPos, &Pos, &matRot);
		D3DXVec3Normalize(&NewPos, &NewPos);
		BrushLine[1].Pos = (NewPos * ExtLine) + GetPos;

		BrushCenterPos.x = BrushLine[1].Pos.x;
		BrushCenterPos.y = BrushLine[1].Pos.y;
		BrushCenterPos.z = BrushLine[1].Pos.z;
		BrushLine[1].Pos.y = GetHeight(BrushCenterPos) + m_Seam2Surface;

		RwD3D9SetFVF(D3DFVF_LINE3D);
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));
	}

	EndD3DLineState();
}

///////////////////////////////////////////////////////////////////////////////////////////////////