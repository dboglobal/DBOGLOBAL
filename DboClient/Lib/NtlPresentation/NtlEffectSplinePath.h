//***********************************************************************************
//	File		:	NtlEffectSplinePath.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include "rwcore.h"

#include <string>

#include "NtlPLProperty.h"

#include "rtquat.h"
#include "rtslerp.h"

struct SNTL_SPLINE_CONTROL_POINT
{
	RwV3d	pos;
	RwV3d	rotate;
	RwReal	fTime;
};

struct SNTL_SPLINE_CURVE_POINT
{
	RwV3d	pos;
	RtQuat	quat;
	RwReal	fTime;
};


class CNtlEffectSplinePath : public CNtlPLProperty
{
public:
	struct INFO
	{
		RwInt32		nAxisType;
		RwInt32		nSplineType;
		RwInt32		nCurveType;
		RwInt32		nControlCount;
		RwInt32		nCurveSubDivision;
		RwV3d		vForwardFactor;
		RwReal		fPointSpeed;

		RwInt32		nRotateCount;
		RwReal		fRadius;
		RwBool		bRotateDirection;
		RwReal		fTime;
	};
	enum AXIS_TYPE
	{
		AXIS_X = 0x00,
		AXIS_Y,
		AXIS_Z,

		AXIS_TYPE_MAX_COUNT
	};
	enum SPLINE_TYPE
	{
		SPLINE_TYPE_B = 0x00,
		SPLINE_TYPE_CATMULL,
		SPLINE_TYPE_HERMITE,
		SPLINE_TYPE_BEZIER,
		SPLINE_TYPE_LINE,

		SPLINE_TYPE_MAX_COUNT
	};

	INFO						m_Info;

	SNTL_SPLINE_CONTROL_POINT*	m_pControlPoint;

	SNTL_SPLINE_CURVE_POINT*	m_pCurveData;
	RwInt32						m_nTotalCurveCount;
public:
	CNtlEffectSplinePath(const RwChar* strName);
	~CNtlEffectSplinePath(void);

	void	Destroy();
	RwBool	SetInfo(INFO& Info);
    RwBool	Load(FILE* pFile);

	void	FrameAnimation(RwReal fAniFrame, RwMatrix& matWorld, RwBool bLoop, RwBool bUseRotate, RwReal fScale = 1.0f);    

public:
	virtual RwBool Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)		{ return FALSE; }
	virtual RwBool Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)		{ return FALSE; }

};
