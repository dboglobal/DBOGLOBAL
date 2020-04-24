#pragma once


#include "NtlWorldBrushController.h"


template <class CBObjClass>
class CNtlWB_Circle2D : public CNtlWorldBrushController
{
private:
	CBObjClass* m_pObj;  
	RwReal (CBObjClass::*m_pFunc)(RwV3d& Pos);

protected:
	RwReal GetHeight(RwV3d& Pos);

public:
	CNtlWB_Circle2D(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos), RwReal _Radius);

private:
	RwInt32		m_NumLine;
	D3DCOLOR	m_ClrLine;
	RwReal		m_StepInRadian;
	RwReal		m_2DHeight;
	RwReal		m_ExtLine;
	RwV3d		m_Pos;
	RwBool		m_Enable;

public:
	void	SetRadius(RwReal _Radius, RwBool _AppendFlag = FALSE) { if(!_AppendFlag) { m_ExtLine = _Radius; } else { m_ExtLine += _Radius; } CLAMP(m_ExtLine, 0.0f, 100.0f); }
	void	SetColor(D3DCOLOR Clr) { m_ClrLine = Clr; }
	void	SetPos(RwV3d& _Pos) { m_Pos = _Pos; }
	void	SetEnable(RwBool _Flag) { m_Enable = _Flag; }
	RwBool	GetEnable() { return m_Enable; }
	RwV3d	GetPos() { return m_Pos; }
	RwReal	GetRadius() { return m_ExtLine; }
	void	Render();
	void	Render(RwReal _CurHeight, RwV3d& _Pos);
};

template <class CBObjClass>
CNtlWB_Circle2D<CBObjClass>::CNtlWB_Circle2D(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos), RwReal _Radius)
{
	NTL_ASSERTE(pObj);
	NTL_ASSERTE(pFunc);

	m_pObj	= pObj; 
	m_pFunc	= pFunc;

	m_ID			= ID;
	m_NumLine		= 50;
	m_ClrLine		= 0xffffffff;
	m_StepInRadian	= (D3DX_PI * 2.0f) / m_NumLine;
	m_ExtLine		= _Radius;
	m_Enable		= FALSE;
}

template <class CBObjClass>
RwReal CNtlWB_Circle2D<CBObjClass>::GetHeight(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWB_Circle2D<CBObjClass>::GetHeight");
	NTL_ASSERTE(m_pObj);
	NTL_ASSERTE(m_pFunc);

	NTL_RETURN((m_pObj->*m_pFunc)(Pos));  
}

template <class CBObjClass>
void CNtlWB_Circle2D<CBObjClass>::Render()
{
	// it'd better get another render() into CNtlWB_Circle; doesn't need to add more templates

	sLINE3D			BrushLine[2];
	D3DXVECTOR3		NewPos;
	D3DXMATRIXA16	matRot, mIdentity;

	D3DXVECTOR3			Pos		= D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3			GetPos	= D3DXVECTOR3(m_Pos.x, m_Pos.y, m_Pos.z);
	RwReal				ExtLine = m_ExtLine;

	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

	BrushLine[1].Pos	= Pos * ExtLine + GetPos;
	BrushLine[1].Col	= BrushLine[0].Col = m_ClrLine;

	RwV3d BrushCenterPos;
	BrushCenterPos.x = BrushLine[1].Pos.x;
	BrushCenterPos.z = BrushLine[1].Pos.z;
	BrushLine[1].Pos.y = m_Pos.y + m_Seam2Surface;

	BeginD3DLineState();

	for(int i = 1; i < m_NumLine + 1; ++i)
	{
		BrushLine[0].Pos = BrushLine[1].Pos;

		D3DXMatrixRotationY(&matRot, i * m_StepInRadian);
		D3DXVec3TransformCoord(&NewPos, &Pos, &matRot);
		D3DXVec3Normalize(&NewPos, &NewPos);
		BrushLine[1].Pos = (NewPos * ExtLine) + GetPos;

		BrushCenterPos.x = BrushLine[1].Pos.x;
		BrushCenterPos.z = BrushLine[1].Pos.z;
		BrushLine[1].Pos.y = m_Pos.y + m_Seam2Surface;

		RwD3D9SetFVF(D3DFVF_LINE3D);
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));
	}

	EndD3DLineState();
}

template <class CBObjClass>
void CNtlWB_Circle2D<CBObjClass>::Render(RwReal _CurHeight, RwV3d& _Pos)
{
	sLINE3D			BrushLine[2];
	D3DXVECTOR3		NewPos;
	D3DXMATRIXA16	matRot, mIdentity;

	D3DXVECTOR3			Pos		= D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3			GetPos	= D3DXVECTOR3(_Pos.x, _Pos.y, _Pos.z);
	RwReal				ExtLine = m_ExtLine;

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
		BrushCenterPos.z = BrushLine[1].Pos.z;
		BrushLine[1].Pos.y = /*GetHeight(BrushCenterPos)*/_CurHeight + m_Seam2Surface;

		RwD3D9SetFVF(D3DFVF_LINE3D);
		RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));
	}

	EndD3DLineState();
}

////////////////////////////////////////////////////////////////////////////////////////////////////