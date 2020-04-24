#ifndef __DBO_APPLICATION__
#define __DBO_APPLICATION__

#include "NtlApplication.h"
#include "NtlLogic.h"
#include "NtlRender.h"
#include "NtlPLPropertyContainer.h"
#include "DboGlobal.h"
#include "NtlPLEventHandler.h"



class CSoundManager;
class CFModSound;
class CNtlCamera;
class CNtlPLVisualManager;
class CStageManager;
class CDumpCmdManager;
class CDboEventHandler;



class CDboApplication : public CNtlApplication
{
protected:

	CNtlCamera*		m_pCamera;
	
	RtCharset*		m_pCharset;
	RwRGBA			m_BackgroundColor;
	RwRGBA			m_ForegroundColor;
	
	CStageManager				*m_pStageMgr;
	CNtlPLVisualManager			*m_pVisualManager;
	CNtlPLPropertyContainer		m_PropContainter;

	CDboGlobal					m_DboGlobal;
	CNtlPLEventHandler          *m_pPLEventHandler;
	CDboEventHandler			*m_pDboEventHandler;
	CDumpCmdManager				*m_pDumpManager;

	CNtlGammaController*		m_pGamma;

	// 
  	DWORD						m_dwPrevLMouseDownTime;
	DWORD						m_dwPrevRMouseDownTime;
	RwChar						m_OutputText[2][256];

	HANDLE						m_hInstance;
	RwBool						m_bMouseBackgroundCapture;

	float			m_fLastClientInfoUpdate;
	DBOTIME			m_lastClientInfoSent;


protected:

	void    SetErrorReport();                       ///< Error Report(Bug Trap)관련 설정을 한다.

	RwBool	CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer);
	LRESULT	PreTranslateMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	RwBool	Update(RwReal fTime, RwReal fElapsedTime);
	
	RwBool	Resize(RwInt32 iWidth, RwInt32 iHeight, RwBool zBuffer, RwBool bRestore_from_Minimize);
	void	SetApplicationMinMax(LPMINMAXINFO lpmmi);

	void	DisplayInfo();
	void	DisplayFps();
	RwBool	AttachPlugin();
	
	virtual bool RegisterMainWindowClass( HANDLE processInstance );		///< 윈도우 등록 함수를 오버라이딩 한다.
	virtual void SetGammaRamp( RwReal fGamma );

	virtual RwBool	CanExitApp( void );

public:

	CDboApplication(void);
	~CDboApplication(void);

	virtual void   Destroy();
	
	virtual void SetRenderEnable(RwBool bRender);

	CNtlCamera *GetCamera() { NTL_ASSERT( m_pCamera != NULL, "m_pCamera == NULL"); return m_pCamera; }

	HANDLE	GetWinInstance() { return m_hInstance; }

	virtual RwBool Create( HINSTANCE hInstance, RwInt32 posX, RwInt32 posY, RwInt32 sizeX, RwInt32 sizeY, RwBool bFullScreen);
};

#endif
