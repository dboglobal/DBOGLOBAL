/*****************************************************************************
 *
 * File			: NTLCamera.h
 * Author		: woody1019
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: NTl top view Camera
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef __NTL_CAMERA_TOP__
#define __NTL_CAMERA_TOP__


#include "NtlCamera.h"
#include "InputHandler.h"
#include "ceventhandler.h"
#include "NtlSLEvent.h"
#include "NtlPLCharacter.h"
#include "NtlSLEventHandler.h"
#include "NtlWeController.h"


class CNtlPLWorldEntity;


class CNtlCameraTop : public CNtlCamera, public CNtlWeController
{
public:
	enum ECameraType
	{
		CAMERA_LOOK_AT,
		CAMERA_FREE,
		CAMERA_SPLINE,
	};

	CNtlCameraTop(void);
	~CNtlCameraTop(void);

private:
	RwReal	m_fDistance;		//Camera와 Target과의 거리

	RwReal	m_fMaxDistance;
	RwReal	m_fMinDistance;
	RwReal	m_fSmoothDistance;
	RwReal	m_fPrefDistance;

	RwReal	m_fXAngle;
	RwReal	m_fMaxXAngle;
	RwReal	m_fMinXAngle;
	RwReal	m_fSmoothXAngle;
	RwReal	m_fPrefXAngle;

	RwReal	m_fYAngle;
	RwReal	m_fMaxYAngle;
	RwReal	m_fMinYAngle;
	RwReal	m_fSmoothYAngle;
	RwReal	m_fPrefYAngle;

	RwMatrix *m_pCachedMatrix;

	RwV3d	m_vLookAt; 
	RwReal	m_fLookAtHeight;

	ECameraType	m_iCameraType;

	RwV2d			m_OldMouseMove;
	bool			m_bMouseMoveValidFlag;

	CNtlPLCharacter	*m_pTargetCharacter;

public:
	RwReal	m_CameraFrontBackSpeed, m_CameraStrafeSpeed;
	RwReal	m_fSpeed;

public:
	VOID	Init();
	void	HandleMouse(RwReal fAngleX, RwReal fAngleY);
	void	HandleMouse(RwReal fDeltaZ);
	void	ClampTranslation(RwV3d& Pos, RwBool IsCamMoving = TRUE);
	
	void	HandleEvents(RWS::CMsg &pMsg);
	void	Translation(RwV3d& NewPos, CNtlPLWorldEntity *pWorldEntity);
	
	void	Update();
	void	UpdateControl(float fElasped);
	void	DestroyControl() {};

	void	SetFov(float fFov);
	void	SetSpeed(float fSpeed);
	void	SetZoomDist(float fZoom);
};

#endif __NTL_CAMERA_TOP__