#pragma once


////////////////////////////////////////////////////////////////////////////////////////////////////


#include "NtlWorldBrushController.h"
#include "NtlMath.h"

using std::vector;
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////


template <class CBObjClass>
class CNtlWB_Road : public CNtlWorldBrushController
{
private:
	CBObjClass* m_pObj;
	RwReal (CBObjClass::*m_pFunc)(RwV3d& Pos);

private:
	RwReal		m_Length;
	RwV3d		m_PosS, m_PosE;
	D3DCOLOR	m_ClrLine;

protected:
	RwReal GetHeight(RwV3d& Pos);

public:
	RwV3d m_OrhogonalOBB[2][3];

public:
	CNtlWB_Road(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos));
	~CNtlWB_Road();

public:
	VOID	SetLength(RwReal _Width);
	RwReal	GetLength();
	void	SetPosS(RwV3d& Pos);
	void	SetPosE(RwV3d& Pos);
	RwV3d&	GetPosS();
	RwV3d&	GetPosE();

public:
	void Render();
};

template <class CBObjClass>
CNtlWB_Road<CBObjClass>::~CNtlWB_Road()
{
}

template <class CBObjClass>
CNtlWB_Road<CBObjClass>::CNtlWB_Road(eNTL_BRUSH_TYPE ID, CBObjClass* pObj, RwReal (CBObjClass::*pFunc)(RwV3d& Pos))
{
	NTL_ASSERTE(pObj);
	NTL_ASSERTE(pFunc);

	m_pObj	= pObj; 
	m_pFunc	= pFunc;

	m_ID		= ID;
	m_ClrLine	= 0xffffffff;

	RwV3d Temp = {0.0f, 0.0f, 0.0f};
	m_PosS = m_PosE = Temp;

	m_Length = dGET_WORLD_PARAM()->WorldSectorTileSize * 5.0f;
}

template <class CBObjClass>
VOID CNtlWB_Road<CBObjClass>::SetLength(RwReal _Width)
{
	m_Length = _Width;
}

template <class CBObjClass>
RwReal CNtlWB_Road<CBObjClass>::GetLength()
{
	return m_Length;
}

template <class CBObjClass>
RwV3d& CNtlWB_Road<CBObjClass>::GetPosS()
{
	NTL_FUNCTION("CNtlWB_Road<CBObjClass>::GetPosS");

	NTL_RETURN(m_PosS);
}

template <class CBObjClass>
RwV3d& CNtlWB_Road<CBObjClass>::GetPosE()
{
	NTL_FUNCTION("CNtlWB_Road<CBObjClass>::GetPosE");

	NTL_RETURN(m_PosE);
}

template <class CBObjClass>
void CNtlWB_Road<CBObjClass>::SetPosS(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWB_Road<CBObjClass>::SetPosS");

	//m_PosS.x = (static_cast<RwInt32>((Pos.x + CNtlWorldBrushController::m_ExtWorldHSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize + m_PosWorldMin.x;
	//m_PosS.z = (static_cast<RwInt32>((Pos.z + CNtlWorldBrushController::m_ExtWorldHSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize + m_PosWorldMin.y;
	m_PosS.x = Pos.x;
	m_PosS.z = Pos.z;

	RwV3d PosTmp;
	PosTmp.x = m_PosS.x;
	PosTmp.z = m_PosS.z;
	m_PosS.y = GetHeight(PosTmp);

	m_PosE		= m_PosS;

	NTL_RETURNVOID();
}

template <class CBObjClass>
void CNtlWB_Road<CBObjClass>::SetPosE(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWB_Road<CBObjClass>::SetPosE");

	//m_PosE.x = (static_cast<RwInt32>((Pos.x + CNtlWorldBrushController::m_ExtWorldHSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize + m_PosWorldMin.x;
	//m_PosE.z = (static_cast<RwInt32>((Pos.z + CNtlWorldBrushController::m_ExtWorldHSize)) / dGET_WORLD_PARAM()->WorldSectorTileSize) * dGET_WORLD_PARAM()->WorldSectorTileSize + m_PosWorldMin.y;
	m_PosE.x = Pos.x;
	m_PosE.z = Pos.z;

	RwV3d PosTmp;
	PosTmp.x = Pos.x;
	PosTmp.z = Pos.z;
	m_PosE.y = GetHeight(PosTmp);

	NTL_RETURNVOID();
}

template <class CBObjClass>
RwReal CNtlWB_Road<CBObjClass>::GetHeight(RwV3d& Pos)
{
	NTL_FUNCTION("CNtlWB_Road<CBObjClass>::GetHeight");

	NTL_RETURN((m_pObj->*m_pFunc)(Pos));  
}

template <class CBObjClass>
void CNtlWB_Road<CBObjClass>::Render()
{
	sLINE3D				BrushLine[2];
	RwInt32				ExtTile = dGET_WORLD_PARAM()->WorldSectorTileSize;
	D3DXMATRIXA16		mIdentity;

	BrushLine[0].Col = 0xff0000ff;
	BrushLine[1].Col = 0xff0000ff;
	BrushLine[0].Pos.x = m_PosS.x;
	BrushLine[0].Pos.y = m_PosS.y;
	BrushLine[0].Pos.z = m_PosS.z;
	BrushLine[1].Pos.x = m_PosE.x;
	BrushLine[1].Pos.y = m_PosE.y;
	BrushLine[1].Pos.z = m_PosE.z;

	BeginD3DLineState();

	//RwD3D9SetRenderState( D3DRS_ZENABLE, FALSE );
	//RwD3D9SetRenderState( D3DRS_LIGHTING, FALSE );

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);	

	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);
	
	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

	// road width for start pos
	RwReal	HalfLength = m_Length / 2.0f;
	RwV3d	vFront;
	RwV3d	vUp;
	RwV3d	vUpDst;
	RwV3d	vRight;
	RwV3d	vSidePosS[2];
	RwV3d	vSidePosE[2];
	RwV3d	vLengthLine[2];

	CopyMemory(&vSidePosS[0], &m_PosS, sizeof(RwV3d));
	CopyMemory(&vSidePosS[1], &m_PosS, sizeof(RwV3d));

	vUp.x = m_PosS.x;
	vUp.y = m_PosS.y + 1.0f;
	vUp.z = m_PosS.z;

	RwV3dSubMacro(&vFront, &m_PosE, &m_PosS);
	RwV3dNormalize(&vFront, &vFront);

	RwV3dSubMacro(&vUpDst, &vUp, &m_PosS);
	RwV3dNormalize(&vUpDst, &vUpDst);

	RwV3dCrossProduct(&vRight, &vUpDst, &vFront);
	RwV3dNormalize(&vRight, &vRight);

	RwV3dIncrementScaled(&vSidePosS[0], &vRight, HalfLength);
	RwV3dIncrementScaled(&vSidePosS[1], &vRight, -HalfLength);

	BrushLine[0].Col = 0xff00ff00;
	BrushLine[1].Col = 0xff00ff00;
	BrushLine[0].Pos.x = vSidePosS[0].x;
	BrushLine[0].Pos.y = vSidePosS[0].y;
	BrushLine[0].Pos.z = vSidePosS[0].z;
	BrushLine[1].Pos.x = vSidePosS[1].x;
	BrushLine[1].Pos.y = vSidePosS[1].y;
	BrushLine[1].Pos.z = vSidePosS[1].z;

	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

	// road width for end pos
	CopyMemory(&vSidePosE[0], &m_PosE, sizeof(RwV3d));
	CopyMemory(&vSidePosE[1], &m_PosE, sizeof(RwV3d));

	vUp.x = m_PosE.x;
	vUp.y = m_PosE.y + 1.0f;
	vUp.z = m_PosE.z;

	RwV3dSubMacro(&vFront, &m_PosS, &m_PosE);
	RwV3dNormalize(&vFront, &vFront);

	RwV3dCrossProduct(&vRight, &vUpDst, &vFront);
	RwV3dNormalize(&vRight, &vRight);

	RwV3dIncrementScaled(&vSidePosE[0], &vRight, HalfLength);
	RwV3dIncrementScaled(&vSidePosE[1], &vRight, -HalfLength);

	BrushLine[0].Col = 0xff00ff00;
	BrushLine[1].Col = 0xff00ff00;
	BrushLine[0].Pos.x = vSidePosE[0].x;
	BrushLine[0].Pos.y = vSidePosE[0].y;
	BrushLine[0].Pos.z = vSidePosE[0].z;
	BrushLine[1].Pos.x = vSidePosE[1].x;
	BrushLine[1].Pos.y = vSidePosE[1].y;
	BrushLine[1].Pos.z = vSidePosE[1].z;

	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

	// road side right vector lines
	CopyMemory(&vLengthLine[0], &vSidePosS[0], sizeof(RwV3d));
	CopyMemory(&vLengthLine[1], &vSidePosE[1], sizeof(RwV3d));

	BrushLine[0].Col = 0xffff0000;
	BrushLine[1].Col = 0xffff0000;
	CopyMemory(&BrushLine[0].Pos, &vLengthLine[0], sizeof(RwV3d));
	CopyMemory(&BrushLine[1].Pos, &vLengthLine[1], sizeof(RwV3d));

	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

	// road side left vector lines
	CopyMemory(&vLengthLine[0], &vSidePosS[1], sizeof(RwV3d));
	CopyMemory(&vLengthLine[1], &vSidePosE[0], sizeof(RwV3d));

	BrushLine[0].Col = 0xffff0000;
	BrushLine[1].Col = 0xffff0000;
	CopyMemory(&BrushLine[0].Pos, &vLengthLine[0], sizeof(RwV3d));
	CopyMemory(&BrushLine[1].Pos, &vLengthLine[1], sizeof(RwV3d));

	RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, BrushLine, sizeof(sLINE3D));

	EndD3DLineState();

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);

	// |-----------------------------|
	// | vSidePosS[1] | vSidePosS[0] |
	// |-----------------------------|
	// | vSidePosE[0] | vSidePosE[1] | 
	// |-----------------------------|

	//
	// <3>--<2>
	//  |  / |
	//  | /  |
	// <0>--<1>
	// 

	CopyMemory(&m_OrhogonalOBB[0][0], &vSidePosE[0], sizeof(RwV3d));
	CopyMemory(&m_OrhogonalOBB[0][1], &vSidePosS[0], sizeof(RwV3d));
	CopyMemory(&m_OrhogonalOBB[0][2], &vSidePosS[1], sizeof(RwV3d));
	CopyMemory(&m_OrhogonalOBB[1][0], &vSidePosS[0], sizeof(RwV3d));
	CopyMemory(&m_OrhogonalOBB[1][1], &vSidePosE[0], sizeof(RwV3d));
	CopyMemory(&m_OrhogonalOBB[1][2], &vSidePosE[1], sizeof(RwV3d));
/*
	CopyMemory(&m_OrhogonalOBB[0][0], &vSidePosE[0], sizeof(RwV3d));
	CopyMemory(&m_OrhogonalOBB[0][1], &vSidePosS[1], sizeof(RwV3d));
	CopyMemory(&m_OrhogonalOBB[0][2], &vSidePosS[0], sizeof(RwV3d));
	CopyMemory(&m_OrhogonalOBB[1][0], &vSidePosS[1], sizeof(RwV3d));
	CopyMemory(&m_OrhogonalOBB[1][1], &vSidePosE[0], sizeof(RwV3d));
	CopyMemory(&m_OrhogonalOBB[1][2], &vSidePosE[1], sizeof(RwV3d));
	*/
}


////////////////////////////////////////////////////////////////////////////////////////////////////