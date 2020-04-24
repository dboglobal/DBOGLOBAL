/*****************************************************************************
 *
 * File			: NTLCamera.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: NTl Camera
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef __NTL_GAME_CAMERA__
#define __NTL_GAME_CAMERA__


#include "NtlCamera.h"
#include "InputHandler.h"
#include "ceventhandler.h"
#include "NtlSLEvent.h"
#include "NtlPLCharacter.h"
#include "NtlSLEventHandler.h"
#include "NtlWeController.h"
#include "NtlPostEffectCamera.h"
#include "NtlPLVisualManager.h"

const RwV3d	_AXIS_X = { 1.0f, 0.0f, 0.0f };
const RwV3d	_AXIS_Y = { 0.0f, 1.0f, 0.0f };
const RwV3d	_AXIS_Z = { 0.0f, 0.0f, 1.0f };

class CNtlPLWorldEntity;

class CNtlGameCamera : public CNtlPostEffectCamera, public CNtlWeController, public RWS::CEventHandler
{
public:
	enum ECameraType
	{
		CAMERA_LOOK_AT,
		CAMERA_FREE,
		CAMERA_SPLINE,
	};

	CNtlGameCamera(void);
	~CNtlGameCamera(void);

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

	INPUT_HANDLE	m_handleMouseWheel;
	INPUT_HANDLE	m_handleMouseMove;
	INPUT_HANDLE	m_handleMouseDown;
	INPUT_HANDLE	m_handleKeyDown;

	RwV2d			m_OldMouseMove;
	bool			m_bMouseMoveValidFlag;

	CNtlPLCharacter	*m_pTargetCharacter;

public:
	RwReal	m_CameraFrontBackSpeed; 
	RwReal	m_CameraStrafeSpeed;
	RwReal	m_CameraUpDownSpeed;
	RwReal	m_fSpeed;
public:
	VOID	Init();
	void	HandleMouse(RwReal fAngleX, RwReal fAngleY);
	void	HandleMouse(RwReal fDeltaZ);
	void	ClampTranslation(RwV3d& Pos, RwBool IsCamMoving = TRUE);
	
	int		MouseDownHandler(unsigned int uiMouseData);
	int		MouseMoveHandler(unsigned int uiMouseData);
	int		MouseWheelHandler(unsigned int uiMouseData);
	int		KeyboardDownHandler(unsigned int uiKeyData);
	
	void	HandleEvents(RWS::CMsg &pMsg);
	void	Translation(RwV3d& NewPos, CNtlPLWorldEntity *pWorldEntity);
	
	void	UpdateControl(float fElasped);
	void	DestroyControl() {};

	void	SetFov(float fFov);
	void	SetSpeed(float fSpeed);

	void	SetZoomDist(float fZoom) {;}
	float	GetZoomDist() { return 0.0f;}
};

#endif