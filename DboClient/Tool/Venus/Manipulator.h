//***********************************************************************************
//	File		:	Manipulator.h
//	Desc		:	
//	Begin		:	2005. 1.20
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include <d3dx9.h>
#include <rwcore.h>

#define N3_PI		3.1415926535897932384626433832795028841971693993751f
#define N3_RAD(x)	((x)*(N3_PI/180.0f))


/**
 * \ingroup Venus
 * \brief 선택된 이펙트들의 이동,회전,크기변환등을 처리하는 클래스
 * \date 2006-08-09
 * \author agebreak
 */
class CManipulator
{
public:
enum AXIS_TYPE
{
	AXIS_TYPE_X = 0x00,
	AXIS_TYPE_Y,
	AXIS_TYPE_Z,
	AXIS_TYPE_XY,
	AXIS_TYPE_YZ,
	AXIS_TYPE_ZX,
	AXIS_TYPE_XYZ,
	AXIS_TYPE_NONE,
};

	FLOAT				m_fCylinderRadius;
	FLOAT				m_fSize;
	D3DXVECTOR3			m_vPosition;	

	D3DXMATRIX			m_matWorld;

	// Arrow Mesh 의 위치 좌표
	D3DXMATRIX			m_ArrowmatWorld[3];

	// Render 할때의 위치 행렬
	D3DXMATRIX			m_matAxisWorld;
	D3DXMATRIX			m_matViewWorld[3];

	RwMatrix			m_matRotatePlane;

	RwInt32				m_nArrowType;

	RwIm3DVertex		m_CylinderVertex[3][10];

	RwIm3DVertex		m_LineVertex[6];

	RwIm3DVertex		m_ScalePlaneVertex[9];
	RwIm3DVertex		m_RotatePlaneVertex[20];
	RwIm3DVertex		m_MovePlaneVertex[18];

	RwIm3DVertex		m_RotateLineVertex[19];


	RwIm3DVertex		m_IntersectionPlaneVertex[3][4];

	RwV3d				m_vOldIntersectionPoint;
	POINT				m_posOldIntersectionPoint;
public:
	CManipulator();
	~CManipulator();

	static	CManipulator&	GetInstance(void);

	void	Render(RwMatrix* pViewMatrix);	

	void	RenderLine(RwInt32 nType);
	void	RenderAxis(RwInt32 nType);
	void	RenderPlane(RwInt32 nType);

	void	SetSize(FLOAT size);
	void	SetScale(FLOAT scale);
	void	SetPosition(FLOAT x, FLOAT y, FLOAT z);

	RwBool	Intersection3dPoint(RwV3d* pIntersectionPoint, const RwV3d* pRayPos, const RwV3d* pRayDir);

	RwBool	IntersectionStart(POINT point);
	void	IntersectionEnd();
	RwBool	IntersectionMove(RwMatrix* pEditMatrix, POINT point);
	RwBool	IntersectionMove(RwV3d* pEditVector, POINT point);

	int		IntersectionArrow(const RwV3d* pRayPos, const RwV3d* pRayDir);

	FLOAT	GetDrawScaleFactor(const D3DXVECTOR3* pRayPos, float x, float y, float z, FLOAT factor = 30.f);
	FLOAT	ModulateCameraScaleMatrix(D3DXMATRIX& mat, const D3DXVECTOR3* pRayPos, float x, float y, float z, FLOAT factor = 30.f);
	void	BuildAxis();

	void	GetEditMatrix(RwMatrix& matResult, RwInt32 nEditMode, RwInt32 nArrarType, POINT gap);
	void	GetMoveDistance(RwMatrix& matMove, RwInt32 nArrarType, POINT gap, RwReal fSpeed);

	void	GetRotateDistance(RwMatrix* pRotateMatrix, RwInt32 nArrarType, POINT gap, RwReal fSpeed);
	void	GetRotateDistance(RwV3d* pRotateVector, RwInt32 nArrarType, POINT gap, RwReal fSpeed);

	void	GetScaleDistance(RwMatrix* pScaleMatrix, RwInt32 nArrarType, POINT gap, RwReal fSpeed);
	void	GetScaleDistance(RwV3d* pScaleVector, RwInt32 nArrarType, POINT gap, RwReal fSpeed);

public:
	int		GetArrowType()				{ return m_nArrowType; }
	void	SetArrowType(int nType)		{ m_nArrowType = nType; }
};
