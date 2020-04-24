#pragma once


#include "ceventhandler.h"
#include "NtlWeController.h"
#include "NtlPLVisualManager.h"
#include "NtlCamera.h"
#include "NtlSobAvatar.h"

#include "InputHandler.h"

class CNtlWeAvatarController : public CNtlWeController
{
public:
	CNtlWeAvatarController(void);
	~CNtlWeAvatarController(void);

private:
	typedef struct _SCameraAutoRotate
	{
		RwBool bActive;
		RwReal fDelta;
		RwReal fCurr;
	}SCameraAutoRotate;

	SCameraAutoRotate		m_sRotCharBack;
	RwBool					m_bLButtonMoveFlag;
	RwInt32					m_iOldLButtonMouseX;
	RwInt32					m_iOldLButtonMouseY;
	
	RwBool					m_bRButtonMoveFlag;
	RwInt32					m_iOldRButtonMouseX;
	RwInt32					m_iOldRButtonMouseY;

	RwBool					m_bChangeHeadingFlag;
	RwReal					m_fChangeHeadingUpdateTime;

	INPUT_HANDLE			m_handleMouseWheel;
	INPUT_HANDLE			m_handleMouseMove;
	INPUT_HANDLE			m_handleMouseDown;
	INPUT_HANDLE			m_handleMouseUp;

	RwUInt32				m_uiCurMoveFlags;

	CNtlPLVisualManager		*m_pVisualManager;
	CNtlSobAvatar			*m_pWeAvatar;
	CNtlCamera				*m_pCamera;

	RwBool					m_bFirstCreate;
	RwBool					m_bAvatarCreate;

	float					m_fZoomDist;
private:
	void	CalcCameraRotateSensitivity(int iPosX, int iPosY, int iMouseDeltaX, int iMouseDeltaY);
	void	CalcCameraRotateToCharBack(void);
	void	UpdateCameraRotateToCharBack(RwReal fElapsed);	
	
	void	MouseUpRBtnHandler(const SMouseData *pData);
	void	MouseMoveRBtnHandler(const SMouseData *pData);
	
	int		ActionMapKeyboardMove(unsigned int uiMoveFlags);
	int     ActionJump(unsigned int uiMoveFlags);

	int		MouseWheelHandler(unsigned int uiMouseData);
	int		MouseMoveHandler(unsigned int uiMouseData);
	int		MouseDownHandler(unsigned int uiMouseData);
	int		MouseUpHandler(unsigned int uiMouseData);

public:
	VOID	Init();
	void	CreateControl(CNtlCamera *pCamera, CNtlPLVisualManager *pVisualManager);
	void	UpdateControl(float fElapsed);
	void	DestroyControl();

	void	CreateWeAvatar();
	void	SetVisibleWeAvatar(bool bFlag);

	void	SetActive(bool bFlag);
	
	void	SetFov(float fFov);
	void	SetSpeed(float fSpeed);
	
	void	SetZoomDist(float fZoom);
	void	SetPosition(RwV3d *pPos);

	static	CNtlWeAvatarController &GetInstance();
};
