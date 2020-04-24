/*****************************************************************************
 *
 * File			: NTLApplication
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: NTl Application
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef __NTL_APPLICATION__
#define __NTL_APPLICATION__

#include "NtlCamera.h"
#include "NtlTimer.h"
#include "NtlGammaController.h"

#define NTL_APP_SCREEN_WIDTH		1024
#define NTL_APP_SCREEN_HEIGHT		768
#define NTL_APP_BIT_DEPTH			32

// TRUE : FULLSCREEN, FALSE : WINDOW
#define NTL_DEBUG_WINDOW_MODE		FALSE

const TCHAR kMainWindowClassName[] = _T("DragonBall");
const TCHAR kMainWindowName[]      = _T("DragonBallOnline Global");
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

typedef RwTexture*	(*CallTextureLoad)(const RwChar *pPath, const RwChar *pFileName, const RwChar *pMaskName);
//typedef RwBool		(*CallTextureLoadPath)(const RwChar* pInPath, RwChar** ppOutPath, RwInt32* pOutPathSize);

void LinkTextureLoad(CallTextureLoad fn/*, CallTextureLoadPath fnPath = NULL*/);
void UnLinkTextureLoad(void);

/**
 * \ingroup Toolkit
 * Applicaton를 생성하기 위한 Base Class 이다.
 *
 *
 */

typedef void(*fpFuncErrorHandler)(const char *pError);

class CNtlApplication
{
public:

	static RwUInt32	m_uiFrameRate;

protected:

	HWND			m_hWnd;
	HACCEL			m_hAccel;
		
	DWORD			m_wndStyle;
	UINT			m_wndFlags;
	HWND			m_hWndInsertAfter;
	RwBool			m_bMinimized;
	RwBool			m_bFullScreen;
	RwInt32			m_iBitDepth;
	RwInt32			m_iWindowPosX;		// Window X
	RwInt32			m_iWindowPosY;		// Window Y
	RwInt32			m_iWindowWidth;		// Window Width
	RwInt32			m_iWindowHeight;	// Window Height
	RwInt32			m_iScreenWidth;		// Actual screen width
	RwInt32			m_iScreenHeight;	// Actual screen height
	RwInt32			m_iUserWidth;		// The width chosen by the user
	RwInt32			m_iUserHeight;		// The height chosen by the user

	RwReal			m_fTime;
	RwReal			m_fElapsedTime;

	RwBool			m_bQuit;

	RwBool			m_bRenderEnable;	// Render가 가능 한지
	
	//The Unique Application Instance
	static CNtlApplication *m_pInstance;

	RwReal			m_fFps;
	bool			m_bShowFps;

	CNtlGammaController*	m_pGammaController;

protected:

	virtual bool	RegisterMainWindowClass(HANDLE processInstance);
	HWND	CreateMainWindow(HANDLE processInstance, int posX, int posY, int sizeX, int sizeY, RwBool bFullScreen = false);
	RwBool  Update();

	RwBool	SelectVideoMode();
	
	RwBool	RsInstallFileSystem();
	RwBool	SetChangeDefaultMode(void);

	void	ChangeWindowStyle(RwBool bFullScreen);

	RwBool	IsDefaultWindowResolution(void);

public:

	CNtlApplication();
	virtual ~CNtlApplication();
	
	RwBool	Create(HINSTANCE hInstance, RwInt32 posX, RwInt32 posY, RwInt32 sizeX, RwInt32 sizeY, RwBool bFullScreen = false);
	RwBool  Create(HWND hHandle, RwBool bFullScreen = false);

	int		MessagePump();
	
	LRESULT WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	RwInt32	GetVideoMode(RwBool bFullScreen, RwInt32 iWidth, RwInt32 iHeight, RwInt32 iBitDepth);
	RwInt32 GetBestVideoMode(RwBool bFullScreen, RwInt32 iWidth, RwInt32 iHeight, RwInt32 iBitDepth);
	virtual void	SetGammaRamp(RwReal fGamma);
	void	ChangeVideoMode(RwInt32 iWidth, RwInt32 iHeight, RwInt32 iBitDepth);

	RwBool	ToggleFullMode(void);
	void	SetShowFps(bool bFlag) { m_bShowFps = bFlag; }
	void	SetFrameRate(UINT uiFrameRate) { m_uiFrameRate = uiFrameRate; }
	
	static	CNtlApplication *GetInstance()	{ return m_pInstance; }
	HWND	GetHWnd()						{ return m_hWnd; }
	RwReal	GetFps()						{ return m_fFps; }
	RwInt32	GetWindowWidth()				{ return m_iWindowWidth; }
	RwInt32	GetWindowHeight()				{ return m_iWindowHeight; }
	RwInt32 GetScreenWidth()				{ return m_iScreenWidth; }
	RwInt32 GetScreenHeight()				{ return m_iScreenHeight; }
	RwInt32 GetUserWidth()					{ return m_iUserWidth; }
	RwInt32 GetUserHeight()					{ return m_iUserHeight; }
	RwInt32 GetBitDepth()					{ return m_iBitDepth; }
	RwBool	IsFullScreen()					{ return m_bFullScreen; }
	bool	IsShowFPS()						{ return m_bShowFps; }

	RwInt32 GetWindowPosX() { return m_iWindowPosX; }
	RwInt32 GetWindowPosY() { return m_iWindowPosY; }
	
	void	SetQuit(RwBool quit = TRUE)		{ m_bQuit = quit; }

	virtual RwBool	CanExitApp( void )		{ return TRUE; }

	virtual CNtlCamera *GetCamera()         { return NULL; }

public:
	virtual void	Init();
	virtual void	Destroy();
	virtual RwBool	Resize(RwInt32 iWidth, RwInt32 iHeight, RwBool zBuffer, RwBool bRestore_from_Minimize);
	virtual void	SetApplicationMinMax(LPMINMAXINFO lpmmi);
	
	virtual void	SetRenderEnable(RwBool bRender)  { m_bRenderEnable = bRender; }
	virtual RwBool	GetRenderEnable() { return m_bRenderEnable; }

protected:

	virtual RwBool	AttachPlugin();
	virtual RwBool	RegisterImageLoader();
	virtual RwBool	CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer);
	virtual RwBool	Update(RwReal fTime, RwReal fElapsedTime);
	virtual RwBool  FixedWindowSizing( WPARAM wParam, LPARAM lParam );
	

public:

	virtual LRESULT	PreTranslateMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam) { return 0;}
};


#endif
