#ifndef __DIRECT_APP_H_
#define __DIRECT_APP_H_

// renderware
#include "rtcharse.h"

// framework
#include "NtlApplication.h"
#include "NtlCamera.h"
#include "NtlLogic.h"

// presentation
#include "NtlPLVisualManager.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLEventHandler.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEventHandler.h"
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlFSMLookAtManager.h"
#include "InputHandler.h"
#include "InputActionMap.h"

class CNtlPLDummyWorld;
class CNtlGameCameraManager;
class CCinematicGui;

class CDirectApplication : public CNtlApplication
{
protected:

	RtCharset*					m_pCharset;
	RwRGBA						m_BackgroundColor;
	RwRGBA						m_ForegroundColor;

	// woody1019
	CNtlPLWorldEntity			*m_pWorldEntity;
	CNtlCamera					*m_pCamera;
		
	CNtlPLVisualManager			*m_pVisualManager;
	CNtlPLPropertyContainer		*m_pPropContainter;
	CNtlPLEventHandler          *m_pPLEventHandler;

	CNtlSobAvatar				*m_pAvatar;	

	//------------------------------------------------------
	// gui 
	CCinematicGui				*m_pCinematicGui;
	
	// 

	RwBool						m_bFirstInWorld;
	RwUInt32					m_uiMoveFlags;	
  	DWORD						m_dwPrevLMouseDownTime;
	DWORD						m_dwPrevRMouseDownTime;
	RwChar						m_OutputText[2][256];


public:
	RwV3d						m_vAvatarInitPos;
	
	
protected:
	
	RwBool	CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer);
	
	RwBool	Update(RwReal fTime, RwReal fElapsedTime);
	RwBool	Resize(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer, RwBool bRestore_from_Minimize);

	RwBool	AttachPlugin();

	void	CreateWorld(RwV3d& vAvatarPos, const RwChar *pWorldName);
	void	CreteAvatarInfo(RwV3d vPos);
	void	CreateAvatar(void);
	void	DeleteAvatar(void);

	void	UpdateWorld(void);
	void	UpdateSoundListener(void);

	void	SplineRender(void);
	void	RenderGrid(void);

	void	DisplayInfo(void);
	
public:

	CDirectApplication(void);
	~CDirectApplication(void);

	virtual void Destroy();
	LRESULT	PreTranslateMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	void OnIdle(void);
	void OnResize(RwUInt32 uiWidth, RwUInt32 uiHeight, RwBool zBuffer);
	
	CNtlCamera *GetCamera() { NTL_ASSERT( m_pCamera != NULL, "m_pCamera == NULL"); return m_pCamera; }

	void OnTeleport(RwReal fTeleportX, RwReal fTeleportZ);
	void OnWorldChange(void);
	void OnAvatarChange(void);
	void OnMobCreate(void);

	RwUInt32 GetMoveFlags(void) { return m_uiMoveFlags; }

public:

	int ActionMapKeyboardMove(unsigned int uiMoveFlags);
};


#endif