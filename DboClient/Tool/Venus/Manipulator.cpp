//***********************************************************************************
//	File		:	Manipulator.h
//	Desc		:	
//	Begin		:	2005. 1.20
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include "VenusConfig.h"
#include "VenusAPI.h"
#include "VenusCamera.h"
#include "NtlEffectDefine.h"
#include "NtlDebug.h"
#include ".\manipulator.h"

CManipulator::CManipulator()
{
	SetPosition(.0f, .0f, .0f);
	m_fSize = 1.5f;
	m_fCylinderRadius	= 0.8f;
	m_nArrowType = AXIS_TYPE_NONE;

	BuildAxis();

	RwIm3DVertexSetRGBA(&m_LineVertex[0], 255, 0, 0, 255);
	RwIm3DVertexSetRGBA(&m_LineVertex[1], 0, 0, 0, 255);
	RwIm3DVertexSetRGBA(&m_LineVertex[2], 0, 255, 0, 255);
	RwIm3DVertexSetRGBA(&m_LineVertex[3], 0, 0, 0, 255);
	RwIm3DVertexSetRGBA(&m_LineVertex[4], 0, 0, 255, 255);
	RwIm3DVertexSetRGBA(&m_LineVertex[5], 0, 0, 0, 255);

	RwIm3DVertexSetPos(&m_LineVertex[0], m_fSize,0.0f,0.0f);
	RwIm3DVertexSetPos(&m_LineVertex[1], 0.0f , 0.0f,0.0f);
	RwIm3DVertexSetPos(&m_LineVertex[2], 0.0f ,m_fSize,0.0f);
	RwIm3DVertexSetPos(&m_LineVertex[3], 0.0f ,0.0f,0.0f);
	RwIm3DVertexSetPos(&m_LineVertex[4], 0.0f ,0.0f,m_fSize);
	RwIm3DVertexSetPos(&m_LineVertex[5], 0.0f ,0.0f,0.0f);

	RwReal fPlaneSize = m_fSize * (3.f/4.f);
	// ====================================================
	// Move Plane
	// ====================================================
	// AXIS_TYPE_XY,
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[0], 0, 0, 0, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[1], 255, 0, 0, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[2], 255, 255, 0, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[3], 0, 0, 0, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[4], 255, 255, 0, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[5], 0, 255, 0, 75);

	RwIm3DVertexSetPos(&m_MovePlaneVertex[0], 0.0f, 0.0f,0.0f);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[1], fPlaneSize, 0.0f,0.0f);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[2], fPlaneSize, fPlaneSize, 0.0f);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[3], 0.0f, 0.0f,0.0f);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[4], fPlaneSize, fPlaneSize, 0.0f);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[5], 0.0f, fPlaneSize,0.0f);


	// AXIS_TYPE_YZ,
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[6], 0, 0, 0, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[7], 0, 255, 0, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[8], 0, 255, 255, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[9], 0, 0, 0, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[10], 0, 255, 255, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[11], 0, 0, 255, 75);

	RwIm3DVertexSetPos(&m_MovePlaneVertex[6], 0.0f, 0.0f,0.0f);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[7], 0.0f, fPlaneSize,0.0f);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[8], 0.0f, fPlaneSize, fPlaneSize);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[9], 0.0f, 0.0f,0.0f);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[10], 0.0f, fPlaneSize, fPlaneSize);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[11], 0.0f, 0.0f, fPlaneSize);

	// AXIS_TYPE_ZX,
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[12], 0, 0, 0, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[13], 0, 0, 255, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[14], 255, 0, 255, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[15], 0, 0, 0, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[16], 255, 0, 255, 75);
	RwIm3DVertexSetRGBA(&m_MovePlaneVertex[17], 255, 0, 0, 75);

	RwIm3DVertexSetPos(&m_MovePlaneVertex[12], 0.0f, 0.0f,0.0f);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[13], 0.0f, 0.0f, fPlaneSize);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[14], fPlaneSize, 0.0f, fPlaneSize);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[15], 0.0f, 0.0f,0.0f);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[16], fPlaneSize, 0.0f, fPlaneSize);
	RwIm3DVertexSetPos(&m_MovePlaneVertex[17], fPlaneSize, 0.0f, 0.0f);


	// ====================================================
	// Rotate Plane
	// ====================================================
	RwIm3DVertexSetPos(&m_RotatePlaneVertex[0], 0.f, 0.f, 0.f);
	RwIm3DVertexSetRGBA(&m_RotatePlaneVertex[0], 200, 200, 200, 75);

#define ROTATE_POINTS 18
	RwReal constant = 2.0f * rwPI / ROTATE_POINTS;
	for(RwInt32 i = 0; i < ROTATE_POINTS + 1; ++i)
	{
		RwIm3DVertexSetPos(&m_RotatePlaneVertex[i+1], (RwReal)RwCos(i * constant) * fPlaneSize, (RwReal)RwSin(i * constant) * fPlaneSize, 0.0f);
		RwIm3DVertexSetRGBA(&m_RotatePlaneVertex[i+1], 200, 200, 200, 75);

		m_RotateLineVertex[i].objVertex = m_RotatePlaneVertex[i+1].objVertex;
		RwIm3DVertexSetRGBA(&m_RotateLineVertex[i], 230, 230, 230, 150);
	}

	// ====================================================
	// Scale Plane
	// ====================================================
	RwIm3DVertexSetRGBA(&m_ScalePlaneVertex[0], 0, 0, 0, 75);
	RwIm3DVertexSetRGBA(&m_ScalePlaneVertex[1], 255, 0, 0, 75);
	RwIm3DVertexSetRGBA(&m_ScalePlaneVertex[2], 0, 255, 0, 75);

	RwIm3DVertexSetRGBA(&m_ScalePlaneVertex[3], 0, 0, 0, 75);
	RwIm3DVertexSetRGBA(&m_ScalePlaneVertex[4], 0, 0, 255, 75);
	RwIm3DVertexSetRGBA(&m_ScalePlaneVertex[5], 0, 255, 0, 75);

	RwIm3DVertexSetRGBA(&m_ScalePlaneVertex[6], 0, 0, 0, 75);
	RwIm3DVertexSetRGBA(&m_ScalePlaneVertex[7], 255, 0, 0, 75);
	RwIm3DVertexSetRGBA(&m_ScalePlaneVertex[8], 0, 0, 255, 75);

	RwIm3DVertexSetPos(&m_ScalePlaneVertex[0], 0.0f, 0.0f,0.0f);
	RwIm3DVertexSetPos(&m_ScalePlaneVertex[1], fPlaneSize, 0.0f,0.0f);
	RwIm3DVertexSetPos(&m_ScalePlaneVertex[2], 0.0f, fPlaneSize,0.0f);

	RwIm3DVertexSetPos(&m_ScalePlaneVertex[3], 0.0f, 0.0f,0.0f);
	RwIm3DVertexSetPos(&m_ScalePlaneVertex[4], 0.0f,  0.0f,fPlaneSize);
	RwIm3DVertexSetPos(&m_ScalePlaneVertex[5], 0.0f, fPlaneSize,0.0f);

	RwIm3DVertexSetPos(&m_ScalePlaneVertex[6], 0.0f, 0.0f,0.0f);
	RwIm3DVertexSetPos(&m_ScalePlaneVertex[7], fPlaneSize, 0.0f,0.0f);
	RwIm3DVertexSetPos(&m_ScalePlaneVertex[8], 0.0f, 0.0f, fPlaneSize);


	// ====================================================
	// Cylinder Vertex
	// ====================================================
	RwIm3DVertexSetPos(&m_CylinderVertex[0][0], 0.f, 0.0f, -m_fCylinderRadius);
	RwIm3DVertexSetRGBA(&m_CylinderVertex[0][0], 255, 0, 0, 75);

	RwIm3DVertexSetPos(&m_CylinderVertex[1][0], 0.f, 0.0f, -m_fCylinderRadius);
	RwIm3DVertexSetRGBA(&m_CylinderVertex[1][0], 0, 255, 0, 75);

	RwIm3DVertexSetPos(&m_CylinderVertex[2][0], 0.f, 0.0f, -m_fCylinderRadius);
	RwIm3DVertexSetRGBA(&m_CylinderVertex[2][0], 0, 0, 255, 75);

#define NUMPOINTS 8
	RwReal fCylinderConstant = 2.0f * rwPI / NUMPOINTS;
	RwReal fCylinderRadius = 0.15f;
	for(RwInt32 i = 0; i < NUMPOINTS + 1; ++i)
	{
		RwIm3DVertexSetPos(&m_CylinderVertex[0][i+1], (RwReal)RwCos(i * fCylinderConstant) * fCylinderRadius, (RwReal)RwSin(i * fCylinderConstant) * fCylinderRadius, 0.0f);
		RwIm3DVertexSetRGBA(&m_CylinderVertex[0][i+1], 255, 0, 0, 75);

		RwIm3DVertexSetPos(&m_CylinderVertex[1][i+1], (RwReal)RwCos(i * fCylinderConstant) * fCylinderRadius, (RwReal)RwSin(i * fCylinderConstant) * fCylinderRadius, 0.0f);
		RwIm3DVertexSetRGBA(&m_CylinderVertex[1][i+1], 0, 255, 0, 75);

		RwIm3DVertexSetPos(&m_CylinderVertex[2][i+1], (RwReal)RwCos(i * fCylinderConstant) * fCylinderRadius, (RwReal)RwSin(i * fCylinderConstant) * fCylinderRadius, 0.0f);
		RwIm3DVertexSetRGBA(&m_CylinderVertex[2][i+1], 0, 0, 255, 75);
	}

	// ====================================================
	// Intersection Plane
	// ====================================================
	// AXIS_TYPE_XY
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[0][0],  1000.f,  1000.f, 0.f);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[0][1], -1000.f,  1000.f, 0.f);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[0][2], -1000.f, -1000.f, 0.f);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[0][3],  1000.f, -1000.f, 0.f);

	// AXIS_TYPE_YZ
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[1][0], 0.f,  1000.f,  1000.f);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[1][1], 0.f, -1000.f,  1000.f);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[1][2], 0.f, -1000.f, -1000.f);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[1][3], 0.f,  1000.f, -1000.f);

	// AXIS_TYPE_ZX
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[2][0],  1000.f, 0.f,  1000.f);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[2][1], -1000.f, 0.f,  1000.f);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[2][2], -1000.f, 0.f, -1000.f);
	RwIm3DVertexSetPos(&m_IntersectionPlaneVertex[2][3],  1000.f, 0.f, -1000.f);
}

CManipulator::~CManipulator()
{
	
}


//------------------------------------------------------------------
//	FuncName	: GetInstance
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
CManipulator& CManipulator::GetInstance(void)
{
	static CManipulator instance;
	return instance;
}

//---------------------------------------------------------------------------
//Name		: BuildAxis
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void CManipulator::Render(RwMatrix* pViewMatrix)
{
	m_matRotatePlane = *pViewMatrix;
	RwMatrixGetPos(&m_matRotatePlane)->x = RwMatrixGetPos(&m_matRotatePlane)->y = RwMatrixGetPos(&m_matRotatePlane)->z = 0.f;

	const RwV3d vViewPosition = *RwMatrixGetPos(pViewMatrix);

	// 카메라와 Manipulator 와의 거리 계산을 통해서 스케일 비율 조절
	D3DXMatrixIdentity(&m_matAxisWorld);
	ModulateCameraScaleMatrix(m_matAxisWorld, (D3DXVECTOR3 *)&vViewPosition, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	RwMatrix matRotatePlane = m_matRotatePlane;
	RwMatrixMultiply(&m_matRotatePlane, &matRotatePlane, (RwMatrix*)&m_matAxisWorld);

	for(int i = 0; i < 3; ++i)
	{
		m_matViewWorld[i] = m_ArrowmatWorld[i];
		ModulateCameraScaleMatrix(m_matViewWorld[i], (D3DXVECTOR3 *)&vViewPosition, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	}

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);

	RenderPlane(m_nArrowType);
	RenderLine(m_nArrowType);
	RenderAxis(m_nArrowType);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);

	RwMatrix matWorld;
	RwMatrixSetIdentity(&matWorld);
	RwD3D9SetTransform(D3DTS_WORLD, &matWorld);
}

void CManipulator::RenderLine(RwInt32 nType)
{
	switch(nType)
	{
	case AXIS_TYPE_X:
		if( RwIm3DTransform(m_LineVertex, 2, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}
		break;
	case AXIS_TYPE_Y:
		if( RwIm3DTransform(m_LineVertex+2, 2, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}
		break;
	case AXIS_TYPE_Z:
		if( RwIm3DTransform(m_LineVertex+4, 2, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}
		break;
	case AXIS_TYPE_XY:
		if( RwIm3DTransform(m_LineVertex, 4, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}
		break;
	case AXIS_TYPE_YZ:
		if( RwIm3DTransform(m_LineVertex+2, 4, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}
		break;
	case AXIS_TYPE_ZX:
		if( RwIm3DTransform(m_LineVertex+4, 2, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}
		if( RwIm3DTransform(m_LineVertex, 2, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}
		break;
	case AXIS_TYPE_NONE:
		if( RwIm3DTransform(m_LineVertex, 6, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}
	}
}

void CManipulator::RenderAxis(RwInt32 nType)
{
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
	for(int i = 0; i < 3; ++i)
	{
		if (m_nArrowType == AXIS_TYPE_Y || m_nArrowType == AXIS_TYPE_Z || m_nArrowType == AXIS_TYPE_YZ) if (i == 0) continue;
		if (m_nArrowType == AXIS_TYPE_X || m_nArrowType == AXIS_TYPE_Z || m_nArrowType == AXIS_TYPE_ZX) if (i == 1) continue;
		if (m_nArrowType == AXIS_TYPE_X || m_nArrowType == AXIS_TYPE_Y || m_nArrowType == AXIS_TYPE_XY) if (i == 2) continue;

		if( RwIm3DTransform(m_CylinderVertex[i], 10, (RwMatrix *)&m_matViewWorld[i], rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
		{
			RwIm3DRenderPrimitive(rwPRIMTYPETRIFAN );        
			RwIm3DEnd();
		}
	}
	RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLBACK);
}

void CManipulator::RenderPlane(RwInt32 nType)
{
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);

	switch(CVenusConfig::GetInstance().m_nEditMode)
	{
	case CVenusConfig::EDIT_MODE_MOVE:
		{
			switch(nType)
			{
			case AXIS_TYPE_XY:
				if( RwIm3DTransform(m_MovePlaneVertex, 6, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);        
					RwIm3DEnd();
				}
				break;
			case AXIS_TYPE_YZ:
				if( RwIm3DTransform(m_MovePlaneVertex+6, 6, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);        
					RwIm3DEnd();
				}
				break;
			case AXIS_TYPE_ZX:
				if( RwIm3DTransform(m_MovePlaneVertex+12, 6, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);        
					RwIm3DEnd();
				}
				break;
			case AXIS_TYPE_NONE:
				if( RwIm3DTransform(m_MovePlaneVertex, 18, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);        
					RwIm3DEnd();
				}
				break;
			}
		}
		break;
	case CVenusConfig::EDIT_MODE_ROTATE:
		{
			if( RwIm3DTransform(m_RotatePlaneVertex, 20, (RwMatrix *)&m_matRotatePlane, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
			{
				RwIm3DRenderPrimitive(rwPRIMTYPETRIFAN);        
				RwIm3DEnd();
			}
			if( RwIm3DTransform(m_RotateLineVertex, 19, (RwMatrix *)&m_matRotatePlane, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
			{
				RwIm3DRenderPrimitive(rwPRIMTYPEPOLYLINE);        
				RwIm3DEnd();
			}
		}
		break;
	case CVenusConfig::EDIT_MODE_SCALE:
		{
			switch(nType)
			{
			case AXIS_TYPE_XY:
				if( RwIm3DTransform(m_ScalePlaneVertex, 3, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);        
					RwIm3DEnd();
				}
				break;
			case AXIS_TYPE_YZ:
				if( RwIm3DTransform(m_ScalePlaneVertex+3, 3, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);        
					RwIm3DEnd();
				}
				break;
			case AXIS_TYPE_ZX:
				if( RwIm3DTransform(m_ScalePlaneVertex+6, 3, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);        
					RwIm3DEnd();
				}
				break;
			case AXIS_TYPE_NONE:
				if( RwIm3DTransform(m_ScalePlaneVertex, 9, (RwMatrix *)&m_matAxisWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);        
					RwIm3DEnd();
				}
				break;
			}
		}
		break;
	}
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
}

//---------------------------------------------------------------------------
//Name		: SetSize
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void CManipulator::SetSize(FLOAT size)
{
	m_fSize = size;
	if(m_fSize < 0.0f)
	{
		m_fSize = 0.0001f;
	}

	BuildAxis();
}

//---------------------------------------------------------------------------
//Name		: SetPosition
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void CManipulator::SetPosition(FLOAT x, FLOAT y, FLOAT z) 
{
	D3DXMatrixTranslation(&m_matWorld, x, y, z);
	m_vPosition.x = x;
	m_vPosition.y = y;
	m_vPosition.z = z;

	BuildAxis();
}	

//---------------------------------------------------------------------------
//Name		: Intersection3dPoint
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
RwBool CManipulator::Intersection3dPoint(RwV3d* pIntersectionPoint, const RwV3d* pRayPos, const RwV3d* pRayDir)
{
	RwMatrix matArrowWorld = *(CVenusCamera::GetInstance().GetViewMatrix());
	RwMatrixGetPos(&matArrowWorld)->x = RwMatrixGetPos(&matArrowWorld)->y = RwMatrixGetPos(&matArrowWorld)->z = 0.f;
	RwMatrixTranslate(&matArrowWorld, (RwV3d *)&m_vPosition, rwCOMBINEPOSTCONCAT);

	RwMatrix matPlaneWorld;
	RwMatrixTranslate(&matPlaneWorld, (RwV3d *)&m_vPosition, rwCOMBINEREPLACE);

	RwReal fDistance = -10000.f;
	RwV3d pos[4];
	RwV3d vLocalRayPos, vLocalRayDir;
	switch(m_nArrowType)
	{
	case AXIS_TYPE_X:
	case AXIS_TYPE_Y:
	case AXIS_TYPE_Z:
		{
			API_GetLocalRay(&vLocalRayPos, &vLocalRayDir, &matArrowWorld, pRayPos, pRayDir);
			pos[0] = m_IntersectionPlaneVertex[AXIS_TYPE_X][0].objVertex;
			pos[1] = m_IntersectionPlaneVertex[AXIS_TYPE_X][1].objVertex;
			pos[2] = m_IntersectionPlaneVertex[AXIS_TYPE_X][2].objVertex;
			pos[3] = m_IntersectionPlaneVertex[AXIS_TYPE_X][3].objVertex;

			if (API_IntersectionPlane(&vLocalRayPos, &vLocalRayDir, &pos[0], &pos[1], &pos[2], &pos[3], fDistance, TRUE))
			{
				pIntersectionPoint->x = pIntersectionPoint->y = pIntersectionPoint->z = 0.f;
				switch(m_nArrowType)
				{
					case AXIS_TYPE_X:	pIntersectionPoint->x = pRayPos->x + pRayDir->x * fDistance;	break;
					case AXIS_TYPE_Y:	pIntersectionPoint->y = pRayPos->y + pRayDir->y * fDistance;	break;
					case AXIS_TYPE_Z:	pIntersectionPoint->z = pRayPos->z + pRayDir->z * fDistance;	break;
				}
			}
		}
		break;
	case AXIS_TYPE_XY:
	case AXIS_TYPE_YZ:
	case AXIS_TYPE_ZX:
		{
			API_GetLocalRay(&vLocalRayPos, &vLocalRayDir, &matPlaneWorld, pRayPos, pRayDir);

			pos[0] = m_IntersectionPlaneVertex[m_nArrowType-AXIS_TYPE_XY][0].objVertex;
			pos[1] = m_IntersectionPlaneVertex[m_nArrowType-AXIS_TYPE_XY][1].objVertex;
			pos[2] = m_IntersectionPlaneVertex[m_nArrowType-AXIS_TYPE_XY][2].objVertex;
			pos[3] = m_IntersectionPlaneVertex[m_nArrowType-AXIS_TYPE_XY][3].objVertex;

			if (API_IntersectionPlane(&vLocalRayPos, &vLocalRayDir, &pos[0], &pos[1], &pos[2], &pos[3], fDistance, TRUE))
			{
				pIntersectionPoint->x = pRayPos->x + pRayDir->x * fDistance;
				pIntersectionPoint->y = pRayPos->y + pRayDir->y * fDistance;
				pIntersectionPoint->z = pRayPos->z + pRayDir->z * fDistance;
			}
		}
		break;
	}

	if (fDistance < 0.f) return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
//Name		: IntersectionStart
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
RwBool CManipulator::IntersectionStart(POINT point)
{
	RwV3d vRayPos, vRayDir;
	CVenusCamera::GetInstance().GetMouseRay(point, vRayPos, vRayDir);

	m_nArrowType = CManipulator::GetInstance().IntersectionArrow(&vRayPos, &vRayDir);
	if (m_nArrowType == CManipulator::AXIS_TYPE_NONE) return FALSE;

	switch(CVenusConfig::GetInstance().m_nEditMode)
	{
	case CVenusConfig::EDIT_MODE_MOVE:
		{
			RwV3d IntersectionPoint = { 0.f, 0.f, 0.f };
			if (Intersection3dPoint(&IntersectionPoint, &vRayPos, &vRayDir))
			{
				m_vOldIntersectionPoint = IntersectionPoint;
			}
		}
		break;
	case CVenusConfig::EDIT_MODE_ROTATE:
	case CVenusConfig::EDIT_MODE_SCALE:
		{
			m_posOldIntersectionPoint = point;
		}
		break;
	}
	return (m_nArrowType == AXIS_TYPE_NONE) ? FALSE : TRUE;
}
//---------------------------------------------------------------------------
//Name		: IntersectionEnd
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void CManipulator::IntersectionEnd()
{
	m_nArrowType = AXIS_TYPE_NONE;
}

//---------------------------------------------------------------------------
//Name		: IntersectionMove
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
RwBool CManipulator::IntersectionMove(RwMatrix* pEditMatrix, POINT point)
{
	switch(CVenusConfig::GetInstance().m_nEditMode)
	{
	case CVenusConfig::EDIT_MODE_MOVE:
		{
			RwV3d vRayPos, vRayDir;
			CVenusCamera::GetInstance().GetMouseRay(point, vRayPos, vRayDir);

			RwV3d vIntersectionPoint;
			if (Intersection3dPoint(&vIntersectionPoint, &vRayPos, &vRayDir))
			{
				RwV3d gap;
				RwV3dSubMacro(&gap, &vIntersectionPoint, &m_vOldIntersectionPoint);
				m_vOldIntersectionPoint = vIntersectionPoint;

				RwMatrixTranslate(pEditMatrix, &gap, rwCOMBINEREPLACE);
				return TRUE;
			}
		}
		break;
	case CVenusConfig::EDIT_MODE_ROTATE:
		{
			POINT gap;
			gap.x = point.x - m_posOldIntersectionPoint.x;
			gap.y = point.y - m_posOldIntersectionPoint.y;
			m_posOldIntersectionPoint = point;

			GetRotateDistance(pEditMatrix, m_nArrowType, gap, CVenusConfig::GetInstance().m_fEditRotateSpeed);
			return TRUE;
		}
		break;
	case CVenusConfig::EDIT_MODE_SCALE:
		{
			POINT gap;
			gap.x = point.x - m_posOldIntersectionPoint.x;
			gap.y = point.y - m_posOldIntersectionPoint.y;
			m_posOldIntersectionPoint = point;

			GetScaleDistance(pEditMatrix, m_nArrowType, gap, CVenusConfig::GetInstance().m_fEditScaleSpeed);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------
//Name		: IntersectionMove
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
RwBool CManipulator::IntersectionMove(RwV3d* pEditVector, POINT point)
{
	switch(CVenusConfig::GetInstance().m_nEditMode)
	{
	case CVenusConfig::EDIT_MODE_MOVE:
		{
			RwV3d vRayPos, vRayDir;
			CVenusCamera::GetInstance().GetMouseRay(point, vRayPos, vRayDir);

			RwV3d vIntersectionPoint;
			if (Intersection3dPoint(&vIntersectionPoint, &vRayPos, &vRayDir))
			{
				RwV3dSubMacro(pEditVector, &vIntersectionPoint, &m_vOldIntersectionPoint);
				m_vOldIntersectionPoint = vIntersectionPoint;
				return TRUE;
			}
		}
		break;
	case CVenusConfig::EDIT_MODE_ROTATE:
		{
			POINT gap;
			gap.x = point.x - m_posOldIntersectionPoint.x;
			gap.y = point.y - m_posOldIntersectionPoint.y;
			m_posOldIntersectionPoint = point;

			GetRotateDistance(pEditVector, m_nArrowType, gap, CVenusConfig::GetInstance().m_fEditRotateSpeed);
			return TRUE;
		}
		break;
	case CVenusConfig::EDIT_MODE_SCALE:
		{
			POINT gap;
			gap.x = point.x - m_posOldIntersectionPoint.x;
			gap.y = point.y - m_posOldIntersectionPoint.y;
			m_posOldIntersectionPoint = point;

			GetScaleDistance(pEditVector, m_nArrowType, gap, CVenusConfig::GetInstance().m_fEditScaleSpeed);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------
//Name		: IntersectionArrow
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
int CManipulator::IntersectionArrow(const RwV3d* pRayPos, const RwV3d* pRayDir)
{
	int nInsectionType = AXIS_TYPE_NONE;

	// ===================================================
	// Plane 면 체크
	// ===================================================
	RwV3d vLocalRayPos, vLocalRayDir;
	API_GetLocalRay(&vLocalRayPos, &vLocalRayDir, (RwMatrix*)&m_matAxisWorld, pRayPos, pRayDir);

	switch(CVenusConfig::GetInstance().m_nEditMode)
	{
	case CVenusConfig::EDIT_MODE_MOVE:
		{
			FLOAT fSize = 0.f;
			RwV3d pos[3];	
			for(int i = 0; i < 3; ++i)
			{
				pos[0] = m_MovePlaneVertex[i * 6 + 0].objVertex;
				pos[1] = m_MovePlaneVertex[i * 6 + 1].objVertex;
				pos[2] = m_MovePlaneVertex[i * 6 + 2].objVertex;

				if(API_IntersectionPolygonFromRay(&vLocalRayPos, &vLocalRayDir, &pos[0], &pos[1], &pos[2], &fSize))
				{
					return nInsectionType = AXIS_TYPE_XY + i;
				}
				else if(API_IntersectionPolygonFromRay(&vLocalRayPos, &vLocalRayDir, &pos[0], &pos[2], &pos[1], &fSize))
				{
					return nInsectionType = AXIS_TYPE_XY + i;
				}

				pos[0] = m_MovePlaneVertex[i * 6 + 3].objVertex;
				pos[1] = m_MovePlaneVertex[i * 6 + 4].objVertex;
				pos[2] = m_MovePlaneVertex[i * 6 + 5].objVertex;

				if(API_IntersectionPolygonFromRay(&vLocalRayPos, &vLocalRayDir, &pos[0], &pos[1], &pos[2], &fSize))
				{
					return nInsectionType = AXIS_TYPE_XY + i;
				}
				else if(API_IntersectionPolygonFromRay(&vLocalRayPos, &vLocalRayDir, &pos[0], &pos[2], &pos[1], &fSize))
				{
					return nInsectionType = AXIS_TYPE_XY + i;
				}
			}
		}
		break;
	case CVenusConfig::EDIT_MODE_ROTATE:
		{
			RwReal fPlaneSize = m_fSize * (3.f/4.f);
			fPlaneSize *= RwV3dLength(RwMatrixGetUp(&m_matRotatePlane));

			if (API_IntersectionSphereFromRay(pRayPos, pRayDir, RwMatrixGetPos(&m_matRotatePlane), fPlaneSize))
			{
				return nInsectionType = AXIS_TYPE_XYZ;
			}
		}
		break;
	case CVenusConfig::EDIT_MODE_SCALE:
		{
			FLOAT fSize = 0.f;
			RwV3d pos[3];	
			for(int i = 0; i < 3; ++i)
			{
				pos[0] = m_ScalePlaneVertex[i * 3 + 0].objVertex;
				pos[1] = m_ScalePlaneVertex[i * 3 + 1].objVertex;
				pos[2] = m_ScalePlaneVertex[i * 3 + 2].objVertex;

				if(API_IntersectionPolygonFromRay(&vLocalRayPos, &vLocalRayDir, &pos[0], &pos[1], &pos[2], &fSize))
				{
					return nInsectionType = AXIS_TYPE_XY + i;
				}
				else if(API_IntersectionPolygonFromRay(&vLocalRayPos, &vLocalRayDir, &pos[0], &pos[2], &pos[1], &fSize))
				{
					return nInsectionType = AXIS_TYPE_XY + i;
				}
			}
		}
		break;
	}

	// ===================================================
	// Mesh 체크
	// ===================================================
	for(RwInt32 i = 0; i < 3; ++i)
	{
		RwV3d vLocalRayPos, vLocalRayDir;
		API_GetLocalRay(&vLocalRayPos, &vLocalRayDir, (RwMatrix*)&m_matViewWorld[i], pRayPos, pRayDir);

		RwV3d point = { 0, 0, -m_fCylinderRadius/2.f };
		if (API_IntersectionSphereFromRay(&vLocalRayPos, &vLocalRayDir, &point, m_fCylinderRadius/2.f))
		{
			return nInsectionType = (AXIS_TYPE_X+i);
		}
	}

	return nInsectionType;
}

//---------------------------------------------------------------------------
//Name		: BuildAxis
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
FLOAT CManipulator::GetDrawScaleFactor(const D3DXVECTOR3* pRayPos, float x, float y, float z, FLOAT factor)
{	
	FLOAT distance = sqrtf((pRayPos->x-x)*(pRayPos->x-x) + (pRayPos->y-y)*(pRayPos->y-y) + (pRayPos->z-z)*(pRayPos->z-z));
	return distance / factor;
}

//---------------------------------------------------------------------------
//Name		: BuildAxis
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
FLOAT CManipulator::ModulateCameraScaleMatrix(D3DXMATRIX& mat, const D3DXVECTOR3* pRayPos, float x, float y, float z, FLOAT factor)
{
	FLOAT distancefactor = GetDrawScaleFactor(pRayPos, x, y, z, factor);

	D3DXMATRIX scalematrix;
	D3DXMatrixScaling(&scalematrix, distancefactor, distancefactor, distancefactor);
	D3DXMatrixMultiply(&mat, &scalematrix, &mat);

	mat._41 *= distancefactor;
	mat._42 *= distancefactor;
	mat._43 *= distancefactor;

	D3DXMatrixMultiply(&mat, &mat, &m_matWorld);

	return distancefactor;
}


//---------------------------------------------------------------------------
//Name		: BuildAxis
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void CManipulator::BuildAxis()
{
	D3DXMATRIX rot;
	D3DXMatrixIdentity( &rot );
	D3DXMatrixTranslation( &m_ArrowmatWorld[0], m_fSize, 0, 0 );	
	D3DXMatrixRotationY( &rot, N3_RAD(-90) );
	D3DXMatrixMultiply( &m_ArrowmatWorld[0], &rot, &m_ArrowmatWorld[0] );

	D3DXMatrixTranslation( &m_ArrowmatWorld[1], 0, m_fSize, 0 );	
	D3DXMatrixRotationX( &rot, N3_RAD(90) );	
	D3DXMatrixMultiply( &m_ArrowmatWorld[1], &rot, &m_ArrowmatWorld[1] );

	D3DXMatrixTranslation( &m_ArrowmatWorld[2], 0, 0, m_fSize );	
	D3DXMatrixRotationY( &rot, N3_RAD(180) );
	D3DXMatrixMultiply( &m_ArrowmatWorld[2], &rot, &m_ArrowmatWorld[2] );
}

//---------------------------------------------------------------------------
//Name		: GetEditMatrix
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void CManipulator::GetEditMatrix(RwMatrix& matResult, RwInt32 nEditMode, RwInt32 nArrarType, POINT gap)
{
	switch(nEditMode)
	{
	case CVenusConfig::EDIT_MODE_MOVE:
		{
			GetMoveDistance(matResult, nArrarType, gap, CVenusConfig::GetInstance().m_fEditMoveSpeed);
		}
		break;
	case CVenusConfig::EDIT_MODE_ROTATE:
		{
			GetRotateDistance(&matResult, nArrarType, gap, CVenusConfig::GetInstance().m_fEditRotateSpeed);
		}
		break;
	case CVenusConfig::EDIT_MODE_SCALE:
		{
			GetScaleDistance(&matResult, nArrarType, gap, CVenusConfig::GetInstance().m_fEditScaleSpeed);
		}
		break;
	}
}

//---------------------------------------------------------------------------
//Name		: GetMoveDistance
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void CManipulator::GetMoveDistance(RwMatrix& matMove, RwInt32 nArrarType, POINT gap, RwReal fSpeed)
{
	RwV3d vMoveDistance = { 0.f, 0.f, 0.f };
	switch(nArrarType)
	{
	case AXIS_TYPE_X:
		{
			vMoveDistance.x = -gap.x * fSpeed;
		}
		break;
	case AXIS_TYPE_Y:
		{
			vMoveDistance.y = -gap.y * fSpeed;
		}
		break;
	case AXIS_TYPE_Z:
		{
			vMoveDistance.z = -gap.y * fSpeed;
		}
		break;
	case AXIS_TYPE_XY:
		{
			vMoveDistance.x = -gap.x * fSpeed;
			vMoveDistance.y = -gap.y * fSpeed;
		}
		break;
	case AXIS_TYPE_YZ:
		{
			vMoveDistance.y = -gap.y * fSpeed;
			vMoveDistance.z = -gap.x * fSpeed;
		}
		break;
	case AXIS_TYPE_ZX:
		{
			vMoveDistance.x = -gap.x * fSpeed;
			vMoveDistance.z = -gap.y * fSpeed;
		}
		break;
	}
	RwMatrixTranslate(&matMove, &vMoveDistance, rwCOMBINEREPLACE);
}

//------------------------------------------------------------------
//	FuncName	: GetRotateDistance
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CManipulator::GetRotateDistance(RwMatrix* pRotateMatrix, RwInt32 nArrarType, POINT gap, RwReal fSpeed)
{
	RwV3d vRotate;
	GetRotateDistance(&vRotate, nArrarType, gap, fSpeed);

	RwMatrixRotate(pRotateMatrix, &VEC_X, vRotate.x, rwCOMBINEREPLACE);
	RwMatrixRotate(pRotateMatrix, &VEC_Y, vRotate.y, rwCOMBINEPRECONCAT);
	RwMatrixRotate(pRotateMatrix, &VEC_Z, vRotate.z, rwCOMBINEPRECONCAT);
}

//------------------------------------------------------------------
//	FuncName	: GetRotateDistance
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CManipulator::GetRotateDistance(RwV3d* pRotateVector, RwInt32 nArrarType, POINT gap, RwReal fSpeed)
{
	pRotateVector->x = pRotateVector->y = pRotateVector->z = 0.f;
	switch(nArrarType)
	{
	case AXIS_TYPE_X:
		{
			pRotateVector->x = gap.x * fSpeed;
		}
		break;
	case AXIS_TYPE_Y:
		{
			pRotateVector->y = gap.x * fSpeed;
		}
		break;
	case AXIS_TYPE_Z:
		{
			pRotateVector->z = gap.y * fSpeed;
		}
		break;
	case AXIS_TYPE_XYZ:
		{
			pRotateVector->z = gap.x * fSpeed;
			pRotateVector->x = gap.y * fSpeed;
		}
		break;
	default:
		{
			NTL_ASSERTE(!"default");
		}
		break;
	}
}

//------------------------------------------------------------------
//	FuncName	: GetScaleDistance
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CManipulator::GetScaleDistance(RwMatrix* pScaleMatrix, RwInt32 nArrarType, POINT gap, RwReal fSpeed)
{
	RwV3d vScale;
	GetScaleDistance(&vScale, nArrarType, gap, fSpeed);

	RwMatrixScale(pScaleMatrix, &vScale, rwCOMBINEREPLACE);
}

//------------------------------------------------------------------
//	FuncName	: GetScaleDistance
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CManipulator::GetScaleDistance(RwV3d* pScaleVector, RwInt32 nArrarType, POINT gap, RwReal fSpeed)
{
	pScaleVector->x = pScaleVector->y = pScaleVector->z = 1.f;
	switch(nArrarType)
	{
	case AXIS_TYPE_X:
		{
			pScaleVector->x += gap.x * fSpeed;
		}
		break;
	case AXIS_TYPE_Y:
		{
			pScaleVector->y += gap.y * fSpeed;
		}
		break;
	case AXIS_TYPE_Z:
		{
			pScaleVector->z += gap.y * fSpeed;
		}
		break;
	case AXIS_TYPE_XY:
	case AXIS_TYPE_YZ:
	case AXIS_TYPE_ZX:
		{
			pScaleVector->x += (RwReal)gap.y * fSpeed;
			pScaleVector->y += (RwReal)gap.y * fSpeed;
			pScaleVector->z += (RwReal)gap.y * fSpeed;
		}
		break;
	}
}

