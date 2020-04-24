/*****************************************************************************
* File			: gui_particlepath.h
* Author		: HaeSung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 10. 6
* Abstract		: 
*****************************************************************************
* Desc          : DBO particle emitter의 path ( Catmullrom spline을 이용 )
*****************************************************************************/

#ifndef _GUI_PARTICLE_PATH_H_
#define _GUI_PARTICLE_PATH_H_

#include "gui_particle.h"

START_GUI

enum ePATH_TYPE
{
	PATH_LINE,
	PATH_CATMULLROM,

	PATH_NUMS,
	INVALID_PATH = 0xFF
};

struct SPathInfo
{
	int			nType;
	float		fTime;
	int			nCtrlNum;
	int			nCurveSubNum;

	SPathInfo()
	{
		nType = PATH_LINE; fTime = 0.0f; nCtrlNum = 1; nCurveSubNum = 1;
	}
};

struct SCtrlPoint
{
	RwV2d	vPos;
	float	fTime;

	SCtrlPoint()
	{
		vPos.x = 0.0f; vPos.y = 0.0f; fTime = 0.0f;
	}
};

struct SCurvePoint
{
	RwV2d	vPos;
	float	fTime;

	SCurvePoint()
	{
		vPos.x = 0.0f; vPos.y = 0.0f; fTime = 0.0f;
	}
};

/**
* \brief 파티클의 패스
*/
class CParticlePath
{
public:
	CParticlePath();
	~CParticlePath();

	void Destroy();

	void SetPath( SPathInfo& sInfo, SCtrlPoint* pCtrlPoint );

	void FrameMove( float fTime, RwV2d* pV2dPos, BOOL bLoop );

	void BuildCurveCount();
	void BuildCurveTime();

	void BuildLinCount();
	void BuildLinePoint();
	void BuildCurvePoint();

	int				GetCurveCount();
	SCurvePoint*	GetCurvePoint();

protected:
	SPathInfo		m_sPathInfo;
	SCtrlPoint*		m_pCtrlPoint;
	SCurvePoint*	m_pCurvePoint;

	int				m_nCurveCount;
};

END_GUI

#endif//_GUI_PARTICLE_PATH_H_