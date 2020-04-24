//***********************************************************************************
//	File		:	VenusGrid.cpp
//	Desc		:	
//	Begin		:	2004.9.30
//	Copyright	:	ⓒ 2004 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\venusgrid.h"

#include "NtlPLGlobal.h"

#include "VenusCamera.h"

#include ".\manipulator.h"

#include "VenusAPI.h"

#include "NtlDebug.h"

CVenusGrid::CVenusGrid(void)
{
	m_nTileCount		= 40;
	m_fGridWidth		= 1.f;
	m_nVertexCount		= 0;
	m_pVertex			= NULL;

	m_bIntersection		= FALSE;

	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[0],  1000.f, 0.f,  1000.f);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[1], -1000.f, 0.f,  1000.f);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[2], -1000.f, 0.f, -1000.f);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[3],  1000.f, 0.f, -1000.f);
}

CVenusGrid::~CVenusGrid(void)
{
}


CVenusGrid& CVenusGrid::GetInstance(void)
{
	static CVenusGrid grid;
	return grid;
}

//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusGrid::Create()
{
	CreateGrid(m_nTileCount);

	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: CreateGrid
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusGrid::CreateGrid(RwInt32 nTileCount)
{
	if (m_pVertex != NULL)
	{
		NTL_ARRAY_DELETE(m_pVertex);
		m_pVertex = NULL;
	}

	m_nTileCount = nTileCount;
	m_nVertexCount = (m_nTileCount + 1) * 2 * (m_nTileCount + 1) * 2;

	RwInt32 nGap = (m_nTileCount / 2) % 5;


	NTL_ASSERTE(m_pVertex == NULL);
	m_pVertex = NTL_NEW RwIm3DVertex[m_nVertexCount];

	RwIm3DVertex* pBufferVertices = m_pVertex;

	// 가로
	for (RwInt32 i = 0; i <= m_nTileCount; ++ i)
	{
		if ((i % 5) != nGap)
			RwIm3DVertexSetRGBA(pBufferVertices, 170, 170, 170, 255);
		else
			RwIm3DVertexSetRGBA(pBufferVertices, 255, 255, 255, 255);
		if (i == m_nTileCount / 2) RwIm3DVertexSetRGBA(pBufferVertices, 255, 0, 0, 255);
		++ pBufferVertices;

		if ((i % 5) != nGap)
			RwIm3DVertexSetRGBA(pBufferVertices, 170, 170, 170, 255);
		else
			RwIm3DVertexSetRGBA(pBufferVertices, 255, 255, 255, 255);
		if (i == m_nTileCount / 2) RwIm3DVertexSetRGBA(pBufferVertices, 255, 0, 0, 255);
		++ pBufferVertices;
	}
	// 세로
	for (RwInt32 i = 0; i <= m_nTileCount; ++ i)
	{
		if ((i % 5) != nGap)
			RwIm3DVertexSetRGBA(pBufferVertices, 170, 170, 170, 255);
		else
			RwIm3DVertexSetRGBA(pBufferVertices, 255, 255, 255, 255);
		if (i == m_nTileCount / 2) RwIm3DVertexSetRGBA(pBufferVertices, 0, 0, 255, 255);
		++ pBufferVertices;

		if ((i % 5) != nGap)
			RwIm3DVertexSetRGBA(pBufferVertices, 170, 170, 170, 2550);
		else
			RwIm3DVertexSetRGBA(pBufferVertices, 255, 255, 255, 255);
		if (i == m_nTileCount / 2) RwIm3DVertexSetRGBA(pBufferVertices, 0, 0, 255, 255);
		++ pBufferVertices;
	}
	SetGridWidth(m_fGridWidth);
	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: SetGridWidth
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusGrid::SetGridWidth(RwReal fGridWidth)
{
	m_fGridWidth = fGridWidth;

    // 아직 버텍스버퍼가 생성되지 않았으면 값만 설정한다.
    if(!m_pVertex)
        return TRUE;

	RwReal fStart = (RwReal)(m_nTileCount / 2) * fGridWidth;
	RwV3d vStartPosition = {-fStart, 0, -fStart};

	// 가로
	RwIm3DVertex* pBufferVertices = m_pVertex;
	for (RwInt32 i = 0; i <= m_nTileCount; ++ i)
	{
		RwIm3DVertexSetPos(pBufferVertices, vStartPosition.x, vStartPosition.y, vStartPosition.z + (fGridWidth*i));
		++ pBufferVertices;

		RwIm3DVertexSetPos(pBufferVertices, vStartPosition.x + (fGridWidth*m_nTileCount), vStartPosition.y, vStartPosition.z + (fGridWidth*i));
		++ pBufferVertices;
	}
	// 세로
	for (RwInt32 i = 0; i <= m_nTileCount; ++ i)
	{
		RwIm3DVertexSetPos(pBufferVertices, vStartPosition.x + (fGridWidth*i), vStartPosition.y, vStartPosition.z);
		++ pBufferVertices;

		RwIm3DVertexSetPos(pBufferVertices, vStartPosition.x + (fGridWidth*i), vStartPosition.y, vStartPosition.z + (fGridWidth*m_nTileCount));
		++ pBufferVertices;
	}

	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[0],  fStart, 0.f,  fStart);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[1], -fStart, 0.f,  fStart);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[2], -fStart, 0.f, -fStart);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[3],  fStart, 0.f, -fStart);

	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusGrid::Destroy()
{
	if (m_pVertex != NULL)
	{
		NTL_ARRAY_DELETE(m_pVertex);
		m_pVertex = NULL;
	}

	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: Render
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusGrid::Render()
{
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);

    if(RwIm3DTransform(m_pVertex, m_nVertexCount, NULL, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
    {                         
        RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
        RwIm3DEnd();
    }

	RwMatrix matArrow;
	CVenusCamera::GetInstance().GetArrowMatrix(matArrow);

	RwMatrix vViewMatrix = *CVenusCamera::GetInstance().GetViewMatrix();

	CManipulator::GetInstance().SetPosition(RwMatrixGetPos(&matArrow)->x, RwMatrixGetPos(&matArrow)->y, RwMatrixGetPos(&matArrow)->z);
	CManipulator::GetInstance().Render(&vViewMatrix);

	return TRUE;
}

RwBool CVenusGrid::Intersection3dPoint(RwV3d* pIntersectionPoint, const RwV3d* pCenter, const RwV3d* pRayPos, const RwV3d* pRayDir)
{
	RwMatrix matWorld;
	RwMatrixSetIdentity(&matWorld);

	RwV3d vLocalRayPos, vLocalRayDir;
	API_GetLocalRay(&vLocalRayPos, &vLocalRayDir, &matWorld, pRayPos, pRayDir);

	RwV3d pos[4];
	pos[0] = m_IntersectionPlaneVertex[0].objVertex;
	pos[1] = m_IntersectionPlaneVertex[1].objVertex;
	pos[2] = m_IntersectionPlaneVertex[2].objVertex;
	pos[3] = m_IntersectionPlaneVertex[3].objVertex;

	RwReal fDistance;
	if (API_IntersectionPlane(&vLocalRayPos, &vLocalRayDir, &pos[0], &pos[1], &pos[2], &pos[3], fDistance, TRUE))
	{
		pIntersectionPoint->x = pRayPos->x + pRayDir->x * fDistance;
		pIntersectionPoint->y = pRayPos->y + pRayDir->y * fDistance;
		pIntersectionPoint->z = pRayPos->z + pRayDir->z * fDistance;
		return TRUE;
	}
	return FALSE;
}

//---------------------------------------------------------------------------
//Name		: IntersectionStart
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
RwBool CVenusGrid::IntersectionStart(POINT point, const RwV3d* pCenter)
{
	RwV3d vRayPos, vRayDir;
	CVenusCamera::GetInstance().GetMouseRay(point, vRayPos, vRayDir);

	RwV3d IntersectionPoint = { 0.f, 0.f, 0.f };
	if (Intersection3dPoint(&IntersectionPoint, pCenter, &vRayPos, &vRayDir))
	{
		m_vOldIntersectionPoint = IntersectionPoint;

		m_bIntersection = TRUE;

		m_vOldCenter = *pCenter;

		return TRUE;
	}
	return FALSE;
}

//---------------------------------------------------------------------------
//Name		: IntersectionEnd
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void CVenusGrid::IntersectionEnd()
{
	m_bIntersection = FALSE;
}

//---------------------------------------------------------------------------
//Name		: IntersectionMove
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
RwBool CVenusGrid::IntersectionMove(RwV3d* pMove, POINT point, const RwV3d* pCenter)
{
	RwV3d vRayPos, vRayDir;
	CVenusCamera::GetInstance().GetMouseRay(point, vRayPos, vRayDir);

	RwV3d vIntersectionPoint = { 0.f, 0.f, 0.f };

	if (Intersection3dPoint(&vIntersectionPoint, pCenter, &vRayPos, &vRayDir))
	{
		RwV3d vCenterMove;
		RwV3dSubMacro(&vCenterMove, pCenter, &m_vOldCenter);

		RwV3dSubMacro(pMove, &m_vOldIntersectionPoint, &vIntersectionPoint);

		RwV3dAdd(pMove, pMove, &vCenterMove);

		m_vOldCenter = *pCenter;
		m_vOldIntersectionPoint = vIntersectionPoint;

		return TRUE;
	}
	return FALSE;
}

