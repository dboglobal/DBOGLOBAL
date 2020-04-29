#include "precomp_ntlframework.h"
#include "NtlApplication.h"
#include "memoryhandler.h"
#include "NtlTimer.h"
//#include "NtlCoreUtil.h"
#include "NtlCoreApi.h"
#include "NtlDeviceRestoreHandler.h"
#include "NtlLoadingController.h"


CNtlApplication *CNtlApplication::m_pInstance = NULL;
RwUInt32 CNtlApplication::m_uiFrameRate = 1000;


bool minimized = false;

//Resource Arena의 Size
#define rsRESOURCESDEFAULTARENASIZE (4 << 20)
//File System에서 다룰수 있는 동시에 Open할수 있는 File의 최대 갯수
#define MAX_NB_FILES_PER_FS (20)   

CNtlApplication::CNtlApplication() : m_hWnd(NULL), 
									 m_hAccel(NULL),
									 m_bMinimized(FALSE),
									 m_iWindowPosX(0),
									 m_iWindowPosY(0),
									 m_iScreenWidth(NTL_APP_SCREEN_WIDTH),
									 m_iScreenHeight(NTL_APP_SCREEN_HEIGHT),
									 m_iWindowWidth(NTL_APP_SCREEN_WIDTH), 
									 m_iWindowHeight(NTL_APP_SCREEN_HEIGHT),
									 m_iUserWidth(NTL_APP_SCREEN_WIDTH),
									 m_iUserHeight(NTL_APP_SCREEN_HEIGHT),
									 m_iBitDepth(NTL_APP_BIT_DEPTH),
									 m_bFullScreen(FALSE),
									m_bShowFps(false),
									 m_bQuit(FALSE),
									 m_fFps(0.f),
									 m_fElapsedTime(0.f),
									 m_fTime(0.f),
									 m_bRenderEnable(TRUE)
{
	m_pInstance = this;
}

CNtlApplication::~CNtlApplication() 
{

}

/**
 * Window에서 받은 Message를 처리하는 함수
 * \param hWnd Window Handle
 * \param message Window Message
 * \param wParam 
 * \param lParam 
 * \return message의 처리 결과
 */
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return CNtlApplication::GetInstance()->WndProc(hWnd, message, wParam, lParam);
}

/**
 * Application에서 초기화 해야할 변수 함수
 */
void CNtlApplication::Init()
{
	
}

/**
 * 
 *
 * \return Window Class를 
 */
bool CNtlApplication::RegisterMainWindowClass(HANDLE processInstance)
{
	  WNDCLASS windowClass;
      
#ifdef UNDER_CE
      windowClass.style = CS_DBLCLKS;
#else
      windowClass.style = CS_BYTEALIGNWINDOW | CS_DBLCLKS;
#endif
      windowClass.lpfnWndProc = (WNDPROC)MainWndProc;
      windowClass.cbClsExtra = 0;
      windowClass.cbWndExtra = 0;
      windowClass.hInstance = (HINSTANCE)processInstance;
      windowClass.hIcon = 0;
      windowClass.hCursor = LoadCursor(0, IDC_ARROW);
      windowClass.hbrBackground = 0;
      windowClass.lpszMenuName = 0;
	  /*windowClass.*/
      windowClass.lpszClassName = kMainWindowClassName;
      
      return ( RegisterClass(&windowClass) != 0 );
}

HWND CNtlApplication::CreateMainWindow(HANDLE processInstance, int posX, int posY, int sizeX, int sizeY, RwBool bFullScreen)
{          
    // If we are in exclusive mode i.e. full screen in windows ensure the window flags
    // are correctly set.
    if (bFullScreen)
    {
        m_wndStyle = WS_POPUP;
		m_wndFlags = SWP_NOMOVE | SWP_NOSIZE;
		m_bFullScreen = TRUE;
    }
    else
    {
		m_wndStyle = WS_OVERLAPPEDWINDOW | WS_THICKFRAME;
		m_wndFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW;
		m_bFullScreen = FALSE;
    }

	RECT rect;
	rect.left = rect.top = 0;
	rect.right = sizeX;
	rect.bottom = sizeY;

	m_iWindowWidth	= m_iScreenWidth = m_iUserWidth = sizeX;
	m_iWindowHeight = m_iScreenHeight = m_iUserHeight = sizeY;
	
	// 현재 스타일이 적용된 윈도우의 크기를 계산한다.
	AdjustWindowRect(&rect, m_wndStyle, FALSE);
	
	// 적절한 크기의 Window를 생성한다.
    HWND hwnd = CreateWindow(  kMainWindowClassName, kMainWindowName,
        m_wndStyle,
        posX, posY,
        rect.right - rect.left, rect.bottom - rect.top,
        (HWND)0, (HMENU)0, (HINSTANCE)processInstance, 0);

	// Window 의 실제 크기 계산
	GetWindowRect( hwnd, &rect );
	m_iWindowWidth = rect.right - rect.left;
	m_iWindowHeight = rect.bottom - rect.top;

	// Screen 의 실제 크기 계산
	GetClientRect( hwnd, &rect );
	m_iScreenWidth = rect.right - rect.left;
	m_iScreenHeight = rect.bottom - rect.top;

    SetWindowPos(hwnd, 0, 0, 0, 0, 0, m_wndFlags);

	return hwnd;
}

static rwD3D9DeviceReleaseCallBack g_OldDeviceReleaseCallBack = NULL;
static rwD3D9DeviceRestoreCallBack g_OldDeviceRestoreCallBack = NULL;

/**
 * Ntl의 DeviceRelease CallBack 함수
 */
static void NtlDeviceReleaseCallBack(void)
{
    if (g_OldDeviceReleaseCallBack)    
        g_OldDeviceReleaseCallBack();
	
	CNtlDeviceRestoreHandler::GetInstance()->ReleaseHandler();
	return;
}


/**
 * Ntl의 DeviceRestore CallBack 함수
 */
static void NtlDeviceRestoreCallBack(void)
{
    if (g_OldDeviceRestoreCallBack)
        g_OldDeviceRestoreCallBack();
	
	CNtlDeviceRestoreHandler::GetInstance()->RestoreHandler();
	return;
}

/**
 * Ntl의 DeviceRestore CallBack 함수를 등록을 하는 함수
 */
static void NtlSetDxDeviceEventHandler(void)
{
	g_OldDeviceReleaseCallBack = _rwD3D9DeviceGetReleaseCallback();
	_rwD3D9DeviceSetReleaseCallback(NtlDeviceReleaseCallBack);

	g_OldDeviceRestoreCallBack = _rwD3D9DeviceGetRestoreCallback();
	_rwD3D9DeviceSetRestoreCallback(NtlDeviceRestoreCallBack);
}

/**
 * Renderware의 Debug 정보를 출력하는 CallBack 함수
 */

// 
static void NtlRenderwareDebugMessageHandler(RwDebugType type __RWUNUSED__, const RwChar *str)
{
    OutputDebugString(str);
    OutputDebugString(RWSTRING("\n"));
	
	return;
}


// Disable Shortcut Keys by HoDong(2007.4.16)
// DirectX SDK Technical Articles (Disabling Shotcut Keys in Games)
STICKYKEYS g_StartupStickyKeys = {sizeof(STICKYKEYS), 0};
TOGGLEKEYS g_StartupToggleKeys = {sizeof(TOGGLEKEYS), 0};
FILTERKEYS g_StartupFilterKeys = {sizeof(FILTERKEYS), 0};    

void AllowAccessibilityShortcutKeys( bool bAllowKeys )
{
    if( bAllowKeys )
    {
        // Restore StickyKeys/etc to original state and enable Windows key      
        STICKYKEYS sk = g_StartupStickyKeys;
        TOGGLEKEYS tk = g_StartupToggleKeys;
        FILTERKEYS fk = g_StartupFilterKeys;
        
        SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
        SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
        SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);
    }
    else
    {
        // Disable StickyKeys/etc shortcuts but if the accessibility feature is on, 
        // then leave the settings alone as its probably being usefully used
 
        STICKYKEYS skOff = g_StartupStickyKeys;
        if( (skOff.dwFlags & SKF_STICKYKEYSON) == 0 )
        {
            // Disable the hotkey and the confirmation
            skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
            skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;
 
            SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
        }
 
        TOGGLEKEYS tkOff = g_StartupToggleKeys;
        if( (tkOff.dwFlags & TKF_TOGGLEKEYSON) == 0 )
        {
            // Disable the hotkey and the confirmation
            tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
            tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;
 
            SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
        }
 
        FILTERKEYS fkOff = g_StartupFilterKeys;
        if( (fkOff.dwFlags & FKF_FILTERKEYSON) == 0 )
        {
            // Disable the hotkey and the confirmation
            fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
            fkOff.dwFlags &= ~FKF_CONFIRMHOTKEY;
 
            SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
        }
    }
}


/**
 * Application를 생성하는 함수로 Window의 생성 및 Renderware Engine의 초기화
 * 들을 한다.
 * \param hInstance Window Instance
 * \param posX Window X 시작위치
 * \param posY Window Y 시작위치
 * \param sizeX Window 가로 크기
 * \param sizeY Window 세로 크기
 * \param bFullScreen Full 화면인지 Window 화면인지 확인
 * \return Application이 성공했는지에 대한 유무
 */
RwBool CNtlApplication::Create( HINSTANCE hInstance, 
							    RwInt32 posX, 
								RwInt32 posY, 
								RwInt32 sizeX, 
								RwInt32 sizeY, 
								RwBool bFullScreen)
{
	//Window 생성
	RegisterMainWindowClass(hInstance);
	m_hWnd = CreateMainWindow(hInstance, posX, posY, sizeX, sizeY, bFullScreen);

	// Disable Shortcut Keys by HoDong(2007.4.16)
	// Save the current sticky/toggle/filter key settings so they can be restored them later 
	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
    SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
    SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);
	AllowAccessibilityShortcutKeys( false );
	
	//Memory Function 및 ArenaSize 세팅
	if( !RwEngineInit(NULL, rwENGINEINITNOFREELISTS, rsRESOURCESDEFAULTARENASIZE))
	{
		DBO_FAIL("Engine initial fail !!!");
		return FALSE;
	}
	
	RwDebugSetHandler(NtlRenderwareDebugMessageHandler);

	//Plugin 등록
	AttachPlugin();

	//Renderware 엔진을 Opne한다.
	RwEngineOpenParams openParams;
	openParams.displayID = m_hWnd;
	if( !RwEngineOpen(&openParams) )
	{
		DBO_FAIL("Engine open fail !!!");
		RwEngineTerm();
		return FALSE;
	}

	//현재의 Graphics가 Mode를 지원하는지를 확인한다.
	if( !SelectVideoMode() )
	{
		DBO_FAIL("Select video mode fail !!!");
		return FALSE;
	}
	
	//multisampling test by HoDong
	//RwUInt32 uiMaxMultiSampling = RwD3D9EngineGetMaxMultiSamplingLevels();
	//RwBool bSuccess = RwD3D9ChangeMultiSamplingLevels(1);
	
	//Renderware Engine를 시작한다.
	RwD3D9EngineSetMultiThreadSafe( TRUE );
	if( !RwEngineStart() )
	{
		DBO_FAIL("Select start fail !!!");
		return FALSE;
	}	
	
	//File System Manager를 초기화 하고 Install를 한다.
	if (RtFSManagerOpen(RTFSMAN_UNLIMITED_NUM_FS) != FALSE)
    {
        if (!RsInstallFileSystem())
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
	
	//Image Loader(Bmp, Png) Enrollment
	RegisterImageLoader();
	

	ShowWindow(m_hWnd, 1);
	UpdateWindow(m_hWnd);

	// ShowWindow 한 후에 생성된 윈도우즈의 크기가 변경될 수도 있다. 변경된 크기를 업데이트한다.
	RECT rect;
	GetClientRect( m_hWnd, &rect );
	Resize( rect.right - rect.left, rect.bottom - rect.top, TRUE, FALSE );
	
	//SubSystem들 생성(Camera, Charset ...)
	if(!CreateSubSystem(m_iScreenWidth, m_iScreenHeight, TRUE))
		return FALSE;

	//Timer Start
	CNtlTimer::StartTimer();
	g_SetElapsedTime(0.f);

	//Device Restore Callback
	NtlSetDxDeviceEventHandler();

	return TRUE;
}


/**
 * Application를 생성하는 함수로 Window의 생성 및 Renderware Engine의 초기화
 * 들을 한다.(Mfc 용)
 * \param HWND Window Handle
 * \param bFullScreen Full 화면인지 Window 화면인지 확인
 * \return Application이 성공했는지에 대한 유무
 */
RwBool CNtlApplication::Create(HWND hHandle, RwBool bFullScreen)
{
	m_hWnd			= hHandle;
	m_bFullScreen	= bFullScreen;

	RECT rtRect;
	GetClientRect(m_hWnd, &rtRect);
	m_iScreenWidth  = rtRect.right - rtRect.left;
	m_iScreenHeight = rtRect.bottom - rtRect.top;

	//Memory Function 및 ArenaSize 세팅
	if( !RwEngineInit(NULL, rwENGINEINITNOFREELISTS, rsRESOURCESDEFAULTARENASIZE))
		return FALSE;

	RwDebugSetHandler(NtlRenderwareDebugMessageHandler);

	AttachPlugin();

	//Renderware 엔진을 Opne한다.
	RwEngineOpenParams openParams;
	openParams.displayID = m_hWnd;
	if( !RwEngineOpen(&openParams) )
	{
		RwEngineTerm();
		return FALSE;
	}

	//현재의 Graphics가 Mode를 지원하는지를 확인한다.
	if( !SelectVideoMode() )
	{
		return FALSE;
	}
	
	//Renderware Engine를 시작한다.
	if( !RwEngineStart() )
	{
		return FALSE;
	}	
	
	//File System Manager를 초기화 하고 Install를 한다.
	if (RtFSManagerOpen(RTFSMAN_UNLIMITED_NUM_FS) != FALSE)
    {
        if (!RsInstallFileSystem())
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
	
	//PlugIn 등록 및 Image Loader(Bmp, Png) 등록
	
	RegisterImageLoader();
	
	//SubSystem들 생성(Camera, Charset ...)
	if(!CreateSubSystem(m_iScreenWidth, m_iScreenHeight, TRUE))
		return FALSE;

	//Timer Start
	CNtlTimer::StartTimer();

	//Device Restore Callback
	NtlSetDxDeviceEventHandler();
	
	return TRUE;
}

/**
 * Application의 기능을 종료한다.
 */
void CNtlApplication::Destroy()
{
	// Disable Shortcut Keys by HoDong(2007.4.16)
	AllowAccessibilityShortcutKeys( true );
	
	RtFSManagerClose();
	
	//Stop RW engine
	RwEngineStop();
	
	//Close RW engine
	RwEngineClose();
	
	//Terminate RW engine
	RwEngineTerm();
}



/** 
 * m_nWindowWidth, m_nWindowHeight, m_nBitDepth가 현재의 카드
 * 에서 지원이 되는지를 검사한다.
 * \return Mode를 지원하는지에 대한 여부
 */
RwBool CNtlApplication::SelectVideoMode()
{
	#define MAX_SUBSYSTEMS      (16)
	RwSubSystemInfo GsubSysInfo[MAX_SUBSYSTEMS];

	RwVideoMode     vm;
	RwInt32			nGnumSubSystems = 0;
	RwInt32			nGcurSel = 0;
	RwInt32			nGcurSelVM = 0;
	RwInt32			nDefDeviceNum = 0;

	//현재 System에 Rendering을 할수 있는(Graphic Device)의 갯수를 리턴한다. 
	nGnumSubSystems = RwEngineGetNumSubSystems();
	
	/* Just to be sure ... */
    nGnumSubSystems = (nGnumSubSystems > MAX_SUBSYSTEMS) ? MAX_SUBSYSTEMS : nGnumSubSystems;
	
	//Graphics Device의 모든 이름들을 얻는다.
    for (RwInt32 subSysNum = 0; subSysNum < nGnumSubSystems; ++ subSysNum)
    {
        RwEngineGetSubSystemInfo(&GsubSysInfo[subSysNum], subSysNum);
    }
	
	//현재의 Graphics Device를 얻는다.
	nGcurSel = RwEngineGetCurrentSubSystem();
	
	//현재의 Graphics Device로 Engine Setting 한다.
    if (!RwEngineSetSubSystem(nGcurSel))
    {
        return FALSE;
    }
	
	// 최적의 해상도를 찾는다. Mode를 지원하는지 확인을 한다.
	nGcurSelVM = GetVideoMode( m_bFullScreen, m_iScreenWidth, m_iScreenHeight, m_iBitDepth );
	if( nGcurSelVM < 0 )
	{
		nGcurSelVM = GetBestVideoMode(m_bFullScreen, m_iScreenWidth, m_iScreenHeight, m_iBitDepth);
		if ( nGcurSelVM < 0 )
		{
			DBO_FAIL("Not failed best video mode !!!");
			return FALSE;
		}
	}
	
	//Set up the video mode and set the apps window
	//dimensions to match
    if (!RwEngineSetVideoMode(nGcurSelVM))
    {
        return FALSE;
    }

	RwEngineGetVideoModeInfo(&vm, RwEngineGetCurrentVideoMode());
	if( !(vm.flags & rwVIDEOMODEEXCLUSIVE) )
	{
		RECT rect;
		GetWindowRect(m_hWnd, &rect);

		m_iWindowPosX = rect.left;
		m_iWindowPosY = rect.top;
	}
	
	// 현재 선택된 해상도의 크기를 업데이트
	m_iUserWidth = vm.width;
	m_iUserHeight = vm.height;
	m_iBitDepth = vm.depth;

	return TRUE;
}

void CNtlApplication::ChangeWindowStyle(RwBool bFullScreen)
{
	m_bFullScreen = bFullScreen;

	if(m_bFullScreen)
	{
		m_wndStyle = WS_POPUP;
		m_wndFlags = SWP_SHOWWINDOW;
		m_hWndInsertAfter = HWND_TOPMOST;
	}
	else
	{
		m_wndStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		m_wndFlags = SWP_SHOWWINDOW;
		m_hWndInsertAfter = HWND_NOTOPMOST;		
	}

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = m_iUserWidth;
	rect.bottom = m_iUserHeight;
	
	AdjustWindowRect(&rect, m_wndStyle, FALSE);

	SetWindowLong(m_hWnd, GWL_STYLE, m_wndStyle);
	SetWindowPos(m_hWnd, m_hWndInsertAfter, 0, 0, rect.right - rect.left, rect.bottom - rect.top, m_wndFlags);

	// Resize 한 후에 GetClientRect로 해서 현재 Size와 크기가 안 맞는다면 맞춰서 Resize 해준다.
	RECT rectAfter;
	GetClientRect( m_hWnd, &rectAfter );
	if( rect.right - rect.left != rectAfter.right - rectAfter.left ||
		rect.bottom - rect.top != rectAfter.bottom - rectAfter.top )
	{
		Resize( rectAfter.right - rectAfter.left, rectAfter.bottom - rectAfter.top, TRUE, FALSE );
	}

	// 풀 스크린이 아닐 경우 중간 위치로 변경
	if( !m_bFullScreen )
	{
		RECT rectWindow;
		GetWindowRect( m_hWnd, &rectWindow );

		RwInt32 nScreenWidth = GetSystemMetrics( SM_CXSCREEN );
		RwInt32 nScreenHeight = GetSystemMetrics( SM_CYSCREEN );
		RwInt32 nWindowWidth = rectWindow.right - rectWindow.left;
		RwInt32 nWindowHeight = rectWindow.bottom - rectWindow.top;

		m_iWindowPosX = (RwInt32)(nScreenWidth / 2 ) - (RwInt32)( nWindowWidth / 2 );
		m_iWindowPosY = (RwInt32)(nScreenHeight / 2 ) - (RwInt32)( nWindowHeight / 2 );

		SetWindowPos( m_hWnd, 0, m_iWindowPosX, m_iWindowPosY, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
	}	
}

RwBool CNtlApplication::ToggleFullMode(void)
{
	RwBool bFullScreen = !m_bFullScreen;

	ChangeWindowStyle(bFullScreen);

	//Mode를 지원하는지 확인을 한다.
	RwInt32 nGcurSelVM = GetVideoMode(bFullScreen, m_iUserWidth, m_iUserHeight, m_iBitDepth);	
	if( nGcurSelVM < 0 )
	{
		nGcurSelVM = GetBestVideoMode( bFullScreen, m_iUserWidth, m_iUserHeight, m_iBitDepth );
		if( nGcurSelVM < 0 )
		{
			DBO_FAIL("failed to change video mode : " << m_iUserWidth << "*" << m_iUserHeight << "," << m_iBitDepth );
			return FALSE;
		}
	}
	
	//Set up the video mode and set the apps window
	//dimensions to match
    if (!RwD3D9ChangeVideoMode (nGcurSelVM))
    {
        return FALSE;
    }

	RwVideoMode vm;
	RwEngineGetVideoModeInfo(&vm, RwEngineGetCurrentVideoMode());

	// 만약 풀 스크린 Mode인데 vm.flags가 EXCLUSIVE가 아니라면 창 화면 -> 전체 화면 모드 전환 실패로 간주하고
	// 윈도우를 다시 맞춰준다. 반대의 경우도 똑같은 로직으로 적용한다.
	// 화면 전환에 성공했더라도 현재의 해상도에 맞춰서 다시 한번 윈도우 스타일을 변경한다.
	if( (vm.flags & rwVIDEOMODEEXCLUSIVE) == 0)
	{
		ChangeWindowStyle( FALSE );
	}
	else
		ChangeWindowStyle( TRUE );
	

	API_CoreToggleFullScreenNotify( B2b(bFullScreen) );

	return TRUE;
}


void CNtlApplication::ChangeVideoMode(RwInt32 iWidth, RwInt32 iHeight, RwInt32 iBitDepth)
{
	// ChangeVideo Mode will not work unless it is in FullScreen Mode.
	if(m_bFullScreen)
	{
		// Check if Mode is supported and apply if supported.
		RwInt32 nGcurSelVM = GetVideoMode(m_bFullScreen, iWidth, iHeight, iBitDepth);
		if( nGcurSelVM < 0)
		{
			DBO_WARNING_MESSAGE("Monitor does not support resolution : " << iWidth << "*" << iHeight << "," << iBitDepth );
			
			iWidth = NTL_APP_SCREEN_WIDTH;
			iHeight = NTL_APP_SCREEN_HEIGHT;
			iBitDepth = NTL_APP_BIT_DEPTH;

			nGcurSelVM = GetVideoMode(m_bFullScreen, iWidth, iHeight, iBitDepth);
			if (nGcurSelVM < 0)
			{
				DBO_FAIL("Go to DBOG/user and open SystemEnv.txt and change WINDOW_MODE to true");
				return;
			}
		}	

		// User가 선택한 해상도 업데이트
		m_iUserWidth = iWidth;
		m_iUserHeight = iHeight;
		m_iBitDepth = iBitDepth;

		if (!RwD3D9ChangeVideoMode(nGcurSelVM))
			return;

		// ToggleFullMode() 의 경우 Window의 Resize message를 발생하지만 ChangeVideoMode의 경우 Resize를 강제적으로 발생시킨다.
		RECT rect;
		GetClientRect( m_hWnd, &rect );
		Resize( rect.right - rect.left, rect.bottom - rect.top, TRUE, FALSE );
	}
	// Window Mode
	else
	{
		// User가 선택한 해상도 업데이트
		m_iUserWidth = iWidth;
		m_iUserHeight = iHeight;
		m_iBitDepth = iBitDepth;

		// 현재 바탕화면의 크기를 가져와서 바탕화면보다 더 크다면 크기는 강제로 수정한다.
		// 선택된 크기는 그대로 가진다.
		int nSystemWidth = GetSystemMetrics(SM_CXSCREEN);
		int nSystemHeight = GetSystemMetrics(SM_CYSCREEN);

		if( nSystemWidth < iWidth )
			iWidth = nSystemWidth;
		
		if( nSystemHeight < iHeight )
			iHeight = nSystemHeight;

		int nPosX = ( nSystemWidth - iWidth ) ? ((nSystemWidth - iWidth )/2) : 0;
		int nPosY = ( nSystemHeight - iHeight ) ? ((nSystemHeight - iHeight )/2) : 0;

		SetWindowPos( m_hWnd, NULL, nPosX, nPosY, iWidth, iHeight, SWP_NOZORDER );
	}
}

/**
 *
 * \param bFullScreen
 * \param nWidth Window Width
 * \param nHeight Window Height
 * \return 현재의 Video Mode를 지원을 하는지
 */
RwInt32 CNtlApplication::GetVideoMode(RwBool bFullScreen, RwInt32 nWidth, RwInt32 nHeight, RwInt32 bitDepth)
{
	RwInt32 numVidModes = RwEngineGetNumVideoModes();
	
	RwVideoMode vidModemInfo;
	for (RwInt32 vidMode = 0; vidMode < numVidModes; vidMode++)
	{
		RwEngineGetVideoModeInfo(&vidModemInfo, vidMode);		
		if (bFullScreen)
		{
			if (vidModemInfo.flags & rwVIDEOMODEEXCLUSIVE && 
				vidModemInfo.width  == nWidth && 
				vidModemInfo.height == nHeight && 
				vidModemInfo.depth  == bitDepth)
			{
				return vidMode;
			}
		}		
		else if (vidModemInfo.flags == 0)
		{
			return vidMode;
		}
	}

	return -1;
}


/**
 * File System을 Install를 한다.
 * \return Install이 되었는지에 대한 성공 유무
 */
RwBool CNtlApplication::RsInstallFileSystem()
{
    RwChar      curDirBuffer[NTL_MAX_DIR_PATH];
    RwUInt32    retValue;
    RtFileSystem *wfs, *unc;

    RwUInt32 drivesMask;
    RwInt32 drive;
    RwChar  fsName[2];
    
    /* get the current directory */
    retValue = GetCurrentDirectory(NTL_MAX_DIR_PATH, curDirBuffer);
    if (0 == retValue || retValue > NTL_MAX_DIR_PATH)
    {
        return FALSE;
    }

    /* Register a unc file system */
    
    /* tag a \ to the end of the current directory */
    /* only fails if the buffer size is exhausted */
    rwstrcat( curDirBuffer, "\\" );

    /** Setup the file system manager */
    if ((unc = RtWinFSystemInit(MAX_NB_FILES_PER_FS, curDirBuffer, "unc")) != NULL)
    {
        /* Now actually register the file system */
        if (RtFSManagerRegister(unc) == FALSE)
        {
            return (FALSE);
        }
    }
    else
    {
        return (FALSE);
    }
    
    /* Now register local file systems */
    
    CharUpper(curDirBuffer);

    /* 
     * loop through all logical drives and register a file system for each
     * valid one.
     * Start at 2: this removes a:
     */
    drivesMask = GetLogicalDrives();

    for( drive = 2, drivesMask >>= 1; drivesMask != 0; drive++, drivesMask >>= 1)
    {
        if (drivesMask & 0x01)
        {
            RwInt32 ret;
            RwChar  deviceName[4];

            deviceName[0] = drive + 'A' - 1;
            deviceName[1] = ':';
            deviceName[2] = '\\';   /* Needed for Win98 */
            deviceName[3] = '\0';

            ret = GetDriveType(deviceName);

            if ( ret != DRIVE_UNKNOWN && ret != DRIVE_NO_ROOT_DIR )
            {
                /* Fix device name */
                deviceName[2] = '\0';

                fsName[0] = deviceName[0];
                fsName[1] = '\0';

                /** Setup the file system manager */
                if ((wfs = RtWinFSystemInit(MAX_NB_FILES_PER_FS, deviceName, fsName)) != NULL)
                {
                    /* Now actually register the file system */
                    if (RtFSManagerRegister(wfs) == FALSE)
                    {
                        return (FALSE);
                    }
                    else
                    {
                        /* Set the unc file system as default if we have a unc path */
                        if (curDirBuffer[1] != ':')
                        {
                            RtFSManagerSetDefaultFileSystem(unc);
                        }
                        else if (deviceName[0] == curDirBuffer[0])
                        {
                            RtFSManagerSetDefaultFileSystem(wfs);
						//	RtFileSystemDefaultToAsync(wfs, TRUE);
                        }
                    }
                }
                else
                {
                    return (FALSE);
                }
            }
        }
    }
    
    return (TRUE);
}

/**
 * Window의 Size가 변경됐을경우 호출이 되는 함수
 * \param nWidth Window가로 Size
 * \param nHeight Window세로 Size
 * \param zBuffer ZBuffer 사용 유무
 * \return Resize가 성공적으로 되었는지의 유무
 */
RwBool CNtlApplication::Resize(RwInt32 iWidth, RwInt32 iHeight, RwBool zBuffer, RwBool bRestore_from_Minimize)
{
	RECT rect;

	// Window 의 실제 크기 계산
	GetWindowRect( m_hWnd, &rect );
	m_iWindowWidth = rect.right - rect.left;
	m_iWindowHeight = rect.bottom - rect.top;

	// Screen 의 실제 크기 계산
	GetClientRect( m_hWnd, &rect );
	m_iScreenWidth = rect.right - rect.left;
	m_iScreenHeight = rect.bottom - rect.top;

	return TRUE;
}

/**
* 애플리케이션의 위치나 사이즈가 변할 때
* WM_SIZE 나 WM_MOVE 메세지 보다 우선 처리된다
*/
void CNtlApplication::SetApplicationMinMax(LPMINMAXINFO lpmmi)
{
}

/**
 * 추가적으로 생성을 해야할것들에 대한 함수
 * \param nWidth 현재 생성된 Window의 Client의 가로크기
 * \param nHeight 현재 생성된 Window의 Client의 세로크기
 * \param zBuffer ZBuffer의 사용유무
 * \return 
 */
RwBool CNtlApplication::CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
{
	return TRUE;
}

LRESULT CNtlApplication::WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(PreTranslateMessage(window, message, wParam, lParam))
		return 0L;

	switch (message)
    {
    case WM_ACTIVATEAPP:
        minimized = !wParam;
        return( 0L ); 
    
	case WM_CLOSE:
        DestroyWindow(window);
        return( 0L);

    case WM_DESTROY:
        PostQuitMessage(0);
        return ( 0L );

    case WM_PAINT:
        PAINTSTRUCT Paint;
        BeginPaint(window, &Paint);
        //RWS::MainLoop::Render::Poll();
        EndPaint(window, &Paint);
        return ( 0L );
	
	case WM_SIZE:		
		RwRect r;
		RwBool bRestore_from_Minimize;

		r.x = 0;
		r.y = 0;
		r.w = LOWORD(lParam);
		r.h = HIWORD(lParam);

		bRestore_from_Minimize = FALSE;

		if( wParam == SIZE_MINIMIZED )
		{
			m_bMinimized = TRUE;
		}
		else if( wParam == SIZE_RESTORED )
		{
			if( m_bMinimized )
			{
				m_bMinimized = FALSE;
				bRestore_from_Minimize = TRUE;
			}
		}

		Resize(r.w, r.h, TRUE, bRestore_from_Minimize);
        return ( 0L );
	case WM_SIZING:
		return FixedWindowSizing( wParam, lParam );
	case WM_GETMINMAXINFO:
		SetApplicationMinMax( (LPMINMAXINFO)lParam );
		return( 0L );
	
	case WM_SYSKEYUP:
	case WM_SYSKEYDOWN:
		return ( 0L );	
	default:
        return ( DefWindowProc(window, message, wParam, lParam) );
    }
	return (-1L);
}

RwBool CNtlApplication::Update()
{
	CNtlTimer::UpdateTimer();

	m_fTime			= CNtlTimer::GetAppTime();
	m_fElapsedTime	= CNtlTimer::GetElapsedTime();
	m_fFps			= CNtlTimer::GetFps();

	g_SetElapsedTime(m_fElapsedTime);

	if(Update(m_fTime, m_fElapsedTime))
		return FALSE;

	return TRUE;
}

RwBool CNtlApplication::Update(RwReal fTime, RwReal fElapsedTime)
{
	return TRUE;
}

int CNtlApplication::MessagePump()
{
	int result = 0;
      
    while (!m_bQuit)
    {
        MSG message;
        
        if (PeekMessage(&message, 0, 0U, 0U, PM_REMOVE | PM_NOYIELD) )
        {
			if (message.message == WM_QUIT )
			{
				SetQuit();
				result = (int)message.wParam;
			}
			else
			{
				 if( m_hAccel == NULL || TranslateAccelerator( m_hWnd, m_hAccel, &message ) == 0)
				 {
					 TranslateMessage(&message);
					 DispatchMessage(&message);
				 }
			}
        }
        else
        {
			RwUInt32 uiStartTime = GetTickCount();
			RwUInt32 uiRateTime = (1000 / m_uiFrameRate);

			Update();

			RwUInt32 uiLocalTime = GetTickCount() - uiStartTime;
			if (uiLocalTime < uiRateTime)
			{
				Sleep(uiRateTime - uiLocalTime);
			}
        }
	}

	if( m_hAccel != NULL )
	{
        DestroyAcceleratorTable( m_hAccel );
		m_hAccel = NULL; 
	}

	return (result);
}


RwBool CNtlApplication::AttachPlugin()
{
	return TRUE;
}


CallTextureLoad		g_fnCallTextureLoad		= NULL;
//CallTextureLoadPath g_fnCallTextureLoadPath	= NULL;

void LinkTextureLoad(CallTextureLoad fn/*, CallTextureLoadPath fnPath*/)
{
	g_fnCallTextureLoad		= fn;
	//g_fnCallTextureLoadPath	= fnPath;
}

void UnLinkTextureLoad(void)
{
	g_fnCallTextureLoad = NULL;
}


RwTextureCallBackRead OldCB;

RwTexture * DDSTextureRead(const RwChar * szName, const RwChar * szMaskName)
{
	CNtlLoadingController::GetInstance()->Lock();

	RwTexture*	pTexture			= NULL; 
	//RwBool		bTextureLoadPath	= FALSE;
	RwChar*		pcPath				= NULL;
	RwInt32		iPathSize			= 0;

// 	if(g_fnCallTextureLoadPath && (bTextureLoadPath = g_fnCallTextureLoadPath(RwImageGetPath(), &pcPath, &iPathSize)))
// 	{
// 		_rwImageSwapPath(&pcPath, &iPathSize);
// 	}

	if(g_fnCallTextureLoad)
	{
		char *pPath = RwImageGetPath();
		pTexture = (*g_fnCallTextureLoad)(pPath, szName, szMaskName);
		if(pTexture)
		{
			CNtlLoadingController::GetInstance()->Unlock();
			
// 			if(bTextureLoadPath)
// 			{
// 				_rwImageSwapPath(&pcPath, &iPathSize);
// 			}

			return pTexture;
		}
	}
	
	RwChar *extender = rwstrrchr(szName, '.');
	if( extender != NULL )
	{
		if( _stricmp(extender, ".png") == 0 ||
		    _stricmp(extender, ".bmp") == 0 )
		{
			pTexture = OldCB(szName, szMaskName);
		}
	}
	
	//There are times when png comes in without extension.
	//(Eg skydummy) by HoDong (7/8/2006)
	if(!pTexture)
	{
		pTexture = RwD3D9DDSTextureRead(szName, szMaskName);
		if(pTexture == NULL)
			pTexture = OldCB(szName, szMaskName);
	}
	
	CNtlLoadingController::GetInstance()->Unlock();

// 	if(bTextureLoadPath)
// 	{
// 		_rwImageSwapPath(&pcPath, &iPathSize);
// 	}

	return pTexture;
}

void HookDDSTextureRead(void)
{    
    OldCB = RwTextureGetReadCallBack(); 
    RwTextureSetReadCallBack(DDSTextureRead);
}

RwBool CNtlApplication::RegisterImageLoader()
{
	if (!RwImageRegisterImageFormat(RWSTRING("bmp"), RtBMPImageRead, RtBMPImageWrite))
    {
        return FALSE;
    }

    if (!RwImageRegisterImageFormat(RWSTRING("png"), RtPNGImageRead, RtPNGImageWrite))
    {
        return FALSE;
    }

	HookDDSTextureRead();

	return TRUE;
}

// Gamma
void CNtlApplication::SetGammaRamp(RwReal fGamma)
{
	// 상속 받아서 구현한다.
}

/**
* \brief 가장 적합한 비디오모드를 리턴한다.
*/
RwInt32 CNtlApplication::GetBestVideoMode( RwBool bFullScreen, RwInt32 iWidth, RwInt32 iHeight, RwInt32 iBitDepth )
{
	RwInt32 numVidModes = RwEngineGetNumVideoModes();

	RwInt32 nBestVideoMode = -1;

	RwInt32 nMinWidthMargin = 99999;
	RwInt32 nMinHeightMargin = 99999;
	RwInt32 nMinBitDepthMargin = 99999;
	RwInt32 nMinSummary = 99999;
	RwInt32 nCompare = 0;

	RwVideoMode sModeInfo;

	for (RwInt32 nModeIndex = 0; nModeIndex < numVidModes; nModeIndex++)
	{
		RwEngineGetVideoModeInfo(&sModeInfo, nModeIndex);

		if( sModeInfo.width <= 800 && sModeInfo.height <= 600 )
			continue;

		// 전체 화면의 경우 넓이, 높이, 깊이 오차의 합이 가장 낮은 Video Mode를 리턴하고
		// 창 모드의 경우 Flags가 창 모드로 체크되어 있으면 무조건 리턴한다.
		if (bFullScreen)
		{
			if (sModeInfo.flags & rwVIDEOMODEEXCLUSIVE )
			{
				// 넓이 오차
				nCompare = iWidth - sModeInfo.width;
				if( nCompare >= 0 )
					nMinWidthMargin = nCompare;

				// 높이 오차
				nCompare = iHeight - sModeInfo.height;
				if( nCompare >= 0 ) 
					nMinHeightMargin = nCompare;

				// 깊이 오차
				nCompare = iBitDepth - sModeInfo.depth;
				if( nCompare >= 0 )
					nMinBitDepthMargin = nCompare;

				// 오차의 합
				nCompare = nMinWidthMargin + nMinHeightMargin + nMinBitDepthMargin;
				if( nCompare >= 0 &&
					nCompare < nMinSummary )
				{
					nMinSummary = nCompare;
					nBestVideoMode = nModeIndex;
				}

				// 정확하게 일치함
				if( nMinSummary == 0 )
					return nModeIndex;
			}
		}		
		else if (sModeInfo.flags == 0)
		{
			nBestVideoMode = nModeIndex;
		}
	}

	return nBestVideoMode;
}

/**
* \brief 고정된 비율로 윈도우 Resizing를 하는 함수
*/
RwBool CNtlApplication::FixedWindowSizing( WPARAM wParam, LPARAM lParam )
{
	RECT* pRect = reinterpret_cast<RECT*>(lParam);
	int nCurrentWindowWidth = pRect->right - pRect->left;
	int nCurrentWindowHeight = pRect->bottom - pRect->top;

	float fWidthAdjust = (float)m_iWindowWidth / (float)m_iWindowHeight;
	float fHeightAdjust = (float)m_iWindowHeight / (float)m_iWindowWidth;

	switch( wParam )
	{
		// 높이가 늘어난다.
	case WMSZ_BOTTOMLEFT:
	case WMSZ_BOTTOMRIGHT:
	case WMSZ_LEFT:
	case WMSZ_RIGHT:
		{
			int nFixHeight = (int)((float)nCurrentWindowWidth * fHeightAdjust);

			pRect->bottom = pRect->top + nFixHeight;
		}
		break;
	case WMSZ_TOP:
		{
			int nFixWidth = (int)((float)nCurrentWindowHeight * fWidthAdjust);

			pRect->left = pRect->right - nFixWidth;
		}
		break;
	case WMSZ_BOTTOM:
		{		
			int nFixWidth = (int)((float)nCurrentWindowHeight * fWidthAdjust);

			pRect->right = pRect->left + nFixWidth;
		}
		break;
	case WMSZ_TOPLEFT:
	case WMSZ_TOPRIGHT:
		{
			int nFixHeight = (int)((float)nCurrentWindowWidth * fHeightAdjust);

			pRect->top = pRect->bottom - nFixHeight;
		}
		break;
	default:
		break;
	}

	return TRUE;
}