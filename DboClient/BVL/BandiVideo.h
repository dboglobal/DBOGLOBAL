////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
///			Bandi Video header definition - "BandiVideo.h"
/// 
/// @author   
/// @date     Tuesday, November 18, 2008  5:48:48 PM
/// 
/// Copyright(C) 2009 Bandisoft, All rights reserved.
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <windows.h>

#ifndef ASSERT
#	include "assert.h"
#	define ASSERT(x) assert(x)
#endif

#ifndef _INC_TCHAR
#	ifdef _UNICODE
#		define _T(x)	L##x
#	else
#		define _T(x)	x
#	endif
#endif

#ifndef PURE
#	define PURE = 0
#endif

#define _BVL_MAKE_VER(a,b,c,d)				((a<<24)+(b<<16)+(c<<8)+d)
#define BANDI_VIDEO_LIBRARY_VERSION			_BVL_MAKE_VER(1,1,0,5)		// 1.1.0.5

///e// Notification Messages
///k// 이벤트 발생시 NOTIFY 메시지
///e// Use RegisterWindowMessage() function to register messages
///k// RegisterWindowMessage()로 선언한후 사용한다.

extern UINT	WM_BVNOTIFY_EOF;				// END OF FILE NOTIFICATION, WPARAM: BandiVideo * pBV 
extern UINT	WM_BVNOTIFY_OPENED;				///k// OpenAsync() 호출시 파일 열기 결과 통보, WPARAM: 성공 여부(HRESULT) ///e// result of OpenAsync(), WPARAM: HRESULT value of OpenAsync()

///e// definition of volume min, volume max
///k// 볼륨 최대, 최소값
#define	BVL_MIN_VOLUME	0
#define	BVL_MAX_VOLUME	255


///e// pixel type of video in movie file
///k// 이미지의 픽셀 포맷(타입)
enum BVL_PIXEL_FORMAT
{
	BVLPF_X8R8G8B8,							///e// use D3DFMT_X8R8G8B8 in D3D9	///k // D3DFMT_X8R8G8B8 사용
	BVLPF_A8R8G8B8,							///e// use D3DFMT_A8R8G8B8 in D3D9	///k // D3DFMT_A8R8G8B8 사용
	BVLPF_R5G6B5,							///e// use D3DFMT_R5G6B5 in D3D9	///k // D3DFMT_R5G6B5 사용
};

///e// Options for Open() option
///k// Open() 함수 옵션
enum BVL_OPEN_OPT
{
	BVL_OPEN_OPT_NONE,						///k // 파일 열기만 하고 재생은 하지 않음	///e// Open only
	BVL_OPEN_OPT_AUTOPLAY,					///k // 파일 열기가 완료되면 자동 재생		///e// Open and play
	BVL_OPEN_OPT_AUTOPLAY_REPEAT,			///k // 파일 열기가 완료되면 자동 반복 재생	///e// Open and play repeatedly
};

///e// Audio devide enumerations
///k// 오디오 장치 설정
enum BVL_AUDIO_DEVICE
{
	BVL_AUDIO_DEVICE_NULL,					///e // disable audio track										///k // 음성재생 없음		
	BVL_AUDIO_DEVICE_DSOUND					///e // select Direct Sound to play audio track in video file	///k // DirectSound 를 이용하여 음성을 재생함
};


enum BVL_CONTEXT_TYPE
{
	BVL_CONTEXT_MEMORYBUFFER,				///k // 메모리 버퍼에 영상 데이타를 저장		///e// Use MemoryBuffer to decode video frame
	BVL_CONTEXT_DIRECTX9,					///k // DX9 텍스처에 영상 데이타를 저장			///e// Use DirectX 9.0 to decode video frame
};

///e// Current status of Video player
///k// 비디오의 현재 상태
enum BVL_STATUS
{
	BVL_STATUS_UNINITIALIZED=0,				///k // 비디오 라이브러리가 Create 되지 않음								///e// Video library object is not initialized
	BVL_STATUS_CREATED=1,					///k // 비디오 라이브러리가 생성되었으나 파일을 Open 하지는 않았음			///e// Video library object is initialized but video file is not opened
	BVL_STATUS_STANDBY=1,					///k // 비디오 라이브러리가 생성되었으나 파일을 Open 하지는 않았음			///e// Video library object is initialized but video file is not opened
	BVL_STATUS_OPENED,						///k // 비디오 라이브러리가 생성되었고 파일이 Open 되어 재생가능한 상태임	///e// Video library object is initialized and video file is opened
	BVL_STATUS_PLAYING,						///k // 비디오가 재생중임													///e// and video player is in playing.
	BVL_STATUS_PAUSED,						///k // 비디오가 재생중 정지된 상태임										///e// Video player is paused.
	BVL_STATUS_SEEKING						///k // 비디오가 SEEKING 처리를 하고 있음									///e// Video player is in seeking.
};

///e// Video Information
///k// 비디오 정보
struct	BVLVideoInfo
{
	BVL_STATUS			Status;				///k // 현재의 재생상태를 저장하고 있습니다.										///e// Current status of video player
	BVL_PIXEL_FORMAT	Format;				///k // 동영상 재생 라이브러리에서 동영상을 렌더링한 표면정보를 저장하고 있습니다.	///e// Current surface type of video player
	BVL_OPEN_OPT		Playflag;			///k // 플레이시에 사용되는 옵션을 저장하고 있습니다.								///e// Current playflag of video player

	UINT				width;				///k // 현재 Open 된 동영상의 가로크기를 저장하고 있습니다.							///e// Width of video frame
	UINT				height;				///k // 현재 Open 된 동영상의 세로크기를 저장하고 있습니다.							///e// Height of video frame

	UINT				ErrorFrames;		///k // 동영상 재생 라이브러리에서 에러가 생긴 프레임수를 나타냅니다.				///e// Total number of error frames
	double				CurrentPlayTime;	///k // 현재 재생이 되고있는 프레임의 시간정보를 나타냅니다.						///e// Timestamp of current frame in video player 
	double				CurrentRenderTime;	///k // 가장최근에 제공된 프레임의 시간을 담고 있습니다.							///e// Timestamp of last presented frame 
	double				TotalPlayTime;		///k // 현재 재생이 되고있는 동영상의 총 재생 시간을 초단위로 나타냅니다.			///e// Total duration of video file
};

typedef	struct IBVLMemoryBuffer *LPBVLFRAME, *LPGDIFRAME;
struct IDirect3DTexture9;

///e// Buffer Class when using HBITMAP or DIB section
///k// HBITMAP 이나 DIB를 이용할 경우에 사용할 버퍼 클래스
struct IBVLMemoryBuffer
{
	virtual	HBITMAP		GetHBitmap(void)		const PURE;	///k // IBVLMemoryBuffer의 HBitmap을 얻음		///e// Get HBitmap of current video frame
	virtual	BYTE*		GetRasterBuffer(void)	const PURE; ///k // IBVLMemoryBuffer의 메모리주소를 얻음	///e// Get memory address of current video frame
};


///e// Video Source interface
///k// 비디오 소스 인터페이스
struct IBandiVideoSource
{
public:
	virtual	INT		Read(BYTE* pBuf, INT nBytesToRead) PURE;
	virtual INT64	SetPosition(INT64 nPos) PURE;
	virtual INT64	GetPosition() PURE;
	virtual INT64	GetLength() PURE;
};


///e// Video Library interface
///k// 비디오 재생 인터페이스
struct IBandiVideo
{
public:
	virtual	HRESULT	Create(HWND	hWnd, LPVOID context=NULL, BVL_CONTEXT_TYPE contextType=BVL_CONTEXT_DIRECTX9) PURE;
	virtual	HRESULT	Release() PURE;
	virtual HRESULT Verify(LPCSTR szID, LPCSTR szKey) PURE;
	virtual UINT	GetVersion() PURE;

	// synchronous 
	virtual HRESULT Open(LPCWSTR szPathName, BVL_OPEN_OPT option, BVL_PIXEL_FORMAT format, BVL_AUDIO_DEVICE ad) PURE;
	virtual HRESULT Open(LPCSTR  szPathName, BVL_OPEN_OPT option, BVL_PIXEL_FORMAT format, BVL_AUDIO_DEVICE ad) PURE;
	virtual HRESULT Open(IBandiVideoSource* pSource, BVL_OPEN_OPT option, BVL_PIXEL_FORMAT format, BVL_AUDIO_DEVICE ad) PURE;
	virtual HRESULT	Play(BOOL bRepeat) PURE;
	virtual HRESULT	Seek(double	time) PURE;
	virtual HRESULT	Stop(void) PURE;
	virtual HRESULT	Close(void) PURE;

	// asynchronous
	virtual HRESULT OpenAsync(LPCWSTR szPathName, BVL_OPEN_OPT option, BVL_PIXEL_FORMAT format, BVL_AUDIO_DEVICE ad) PURE;
	virtual HRESULT OpenAsync(LPCSTR  szPathName, BVL_OPEN_OPT option, BVL_PIXEL_FORMAT format, BVL_AUDIO_DEVICE ad) PURE;
	virtual HRESULT OpenAsync(IBandiVideoSource* pSource, BVL_OPEN_OPT option, BVL_PIXEL_FORMAT format, BVL_AUDIO_DEVICE ad) PURE;
	virtual HRESULT	PlayAsync(BOOL bRepeat) PURE;
	virtual HRESULT	SeekAsync(double time) PURE;
	virtual HRESULT	StopAsync(void) PURE;
	virtual HRESULT	CloseAsync(void) PURE;

	virtual HRESULT	Pause(void) PURE;
	virtual HRESULT	Resume(void) PURE;
	virtual HRESULT	SetVolume(UINT volume) PURE;		// 0 ~ 255 

	virtual HRESULT	SetAlpha(UINT alpha) PURE;
	virtual BOOL	IsPlaying() PURE;
	virtual	LPVOID	GetLastFrame(void) PURE;
	virtual	HRESULT	GetLastFrame(void **ppRenderTarget) PURE;
	virtual HRESULT	GetVideoInfo(BVLVideoInfo &info) PURE;
	virtual	HRESULT	ReleaseLastFrame(void* pRenderTarget) PURE;
	virtual	BOOL	CheckFrame(void) PURE;
	virtual	double	FramesToTime(INT frames) PURE;
	virtual	INT		TimeToFrames(double time) PURE;
};
//////////////////////////////////////////////////////////////////////////////////////


#define _FACBV  0x586
#define MAKE_BVHRESULT( code )				MAKE_HRESULT( 1, _FACBV, code )
#define MAKE_BVSTATUS( code )				MAKE_HRESULT( 0, _FACBV, code )

////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         utility class for create bandi video object
///         
/// @date   Friday, Nov 20, 2008  01:05:13 PM
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN64
#	error Not implemented yet
#else
#	ifdef _DEBUG
#		define BANDIVIDEO_DLL_FILE_NAME		_T("BVL32_d.dll")
#	else
#		define BANDIVIDEO_DLL_FILE_NAME		_T("BVL32.dll")
#	endif
#	define BANDIVIDEO_RELEASE_DLL_FILE_NAME	_T("BVL32.dll")
#endif
///e// error code
///k// 에러코드
#define BVERR_LOAD_LIBRARY_FAIL				MAKE_BVHRESULT(0x1020)		///k // LoadLibrary 함수 실행중 에러											///e // Faild to LoadLibrary()
#define BVERR_ALREADY_CREATED				MAKE_BVHRESULT(0x1021)		///k // 비디오 라이브러리의 인스턴스가 이미 생성되었음							///e // Video library instance is already created
#define BVERR_GET_PROC_ADDRESS_FAIL			MAKE_BVHRESULT(0x1022)		///k // 비디오 라이브러리의 CreateBandiVideo 함수의 주소를 얻는데 실패하였음	///e // Failed to get proc address of CreateBandiVideo()
#define BVERR_LIBRARY_NOT_LOADED			MAKE_BVHRESULT(0x1023)		///k // 라이브러리가 로드되지 않았음											///e // Video library is not loaded
#define BVERR_UNSUPPORTED_OS				MAKE_BVHRESULT(0x1024)		///k // 지원되지 않는 운영체제임												///e // Unsupported operating system
#define BVERR_INVALID_VERSION				MAKE_BVHRESULT(0x1025)		///k // 비디오 라이브러리의 헤더와 DLL 의 버젼이 안맞음							///e // Mismatch DLL version and header file version
#define BVERR_OUT_OF_MEMORY					MAKE_BVHRESULT(0x1026)		///k // 메모리가 부족하여 비디오라이브러리의 인스턴스를 생성할수 없음			///e //	Out of memory to create video library instance
#define BVERR_FAILED						MAKE_BVHRESULT(0x1027)		///k // 함수의 정해진 명령을 처리하는데 실패하였음								///e // Failed to appropriate operatio
#define BVERR_NOT_INITIALIZED				MAKE_BVHRESULT(0x1028)		///k // 초기화 되지 않은 함수를 사용하였음										///e //	Uninitialied function is used 
#define BVERR_TIMEOUT						MAKE_BVHRESULT(0x1029)      ///k // 비디오파일을 읽는중 지정된 시간을 초과하였음							///e // Timeout in video playing
#define BVERR_NOT_ENOUGH_DATA				MAKE_BVHRESULT(0x102a)		///k // 비디오파일을 읽는중 완전한 데이터를 읽지 못했음							///e // Timeout in video file reading
#define BVERR_NULL_PTR						MAKE_BVHRESULT(0x102b)      ///k // 인자로 NULL 인자가 대입되었음											///e // unsupported NULL pointer is in parameter 
#define BVERR_INIT							MAKE_BVHRESULT(0x102c)      ///k // 초기화에 실패하였음														///e // failed to initialize video library
#define BVERR_SYNC							MAKE_BVHRESULT(0x102d)      ///k // AVSYNC를 맞추는데 실패하였음											///e // Failed to sync audio and video
#define BVERR_NOT_ENOUGH_BUFFER				MAKE_BVHRESULT(0x102e)      ///k // 파일 읽기 버퍼를 생성하는데 실패하였음									///e // Failed to create file read buffer
#define BVERR_END_OF_STREAM					MAKE_BVHRESULT(0x102f)		///k // 파일의 끝까지 재생하였음												///e // End of stream
#define BVERR_OPEN_FAILED					MAKE_BVHRESULT(0x1030)      ///k // 파일이 없거나 지원되지 않는 컨테이너 형식임								///e // Unsupported file or not exist
#define BVERR_ALLOC							MAKE_BVHRESULT(0x1031)      ///k // 메모리를 할당하기가 실패하였음											///e // Failed to create memory buffer
#define BVERR_INVALID_STREAM				MAKE_BVHRESULT(0x1032)		///k // 재생파일의 형식이 다름													///e // Invalid stream in video file
#define BVERR_UNSUPPORTED					MAKE_BVHRESULT(0x1033)		///k // 지원되지않는 파일임														///e //	Unsupported file 
#define BVERR_NOT_IMPLEMENTED				MAKE_BVHRESULT(0x1034)		///k // 아직 구현되지 않은 기능임												///e // Unsupported function
#define BVERR_INVALID_PARAMS				MAKE_BVHRESULT(0x1035)		///k // 잘못된 파라메터															///e // Invalid parameter 
#define BVERR_ALREADY_PLAYING				MAKE_BVHRESULT(0x1037)		///k // 현재 이미 플레이 중임													///e // video library is in playing
#define BVERR_NOT_OPENED					MAKE_BVHRESULT(0x1038)		///k // Open 되지 않은 파일에서 재생시도										///e // Unopened video file 
#define BVERR_CREATE_FAILED					MAKE_BVHRESULT(0x1039)		///k // Create함수가 제대로 종료되지 않음										///e // Create function is not completed
#define BVERR_ASYNCQUEUE_FULL				MAKE_BVHRESULT(0x103a)		///k // ASYNC 명령큐가 가득참													///e // Async operation queue is full
#define BVERR_UNSUPPORTED_V_CODEC			MAKE_BVHRESULT(0x103b)		///k // 지원되지 않는 비디오 코덱												///e //	Unsupported video codec.
#define BVERR_UNSUPPORTED_A_CODEC			MAKE_BVHRESULT(0x103c)		///k // 지원되지 않는 오디오 코덱												///e //	Unsupported audio codec.

#define BVL_OK								S_OK
#define BVWRN_INVALID_STREAM				MAKE_BVSTATUS(0x0001)		///k // 스트림을 읽는도중 에러가 발생하였으나 재생은 가능함						///e // Video file has an error but playing is possible. 
#define BVWRN_REPOSITION_INPROGRESS			MAKE_BVSTATUS(0x0002)		///k // 스트림 Seeking이 동작중임												///e // Seeking is in progress
#define BVWRN_INFO_NOT_READY				MAKE_BVSTATUS(0x0003)		///k // 오디오/ 비디오 정보를 얻을수 없음										///e // Cannot obtain A/V information

typedef HRESULT (*LPCREATEBANDIVIDEO)(DWORD version,void **p);

////////////////////////////////////////////////////////////////////////////////////////////////////
///         utility class for load library and create instance for video library
/// @param  
/// @return 
/// @date   Wed, Nov 20, 2008  1:48:15 PM
////////////////////////////////////////////////////////////////////////////////////////////////////
class CBandiVideoLibrary : public IBandiVideo
{
public :
	CBandiVideoLibrary()
	{
		m_hDll = NULL;
		m_pCreateBandiVideo = NULL;
		m_pBVL = NULL;
	}

	~CBandiVideoLibrary()
	{
		DestroyLibrary();
	}

	///e// Initialize
	///k// 라이브러리 초기화

	HRESULT InitLibrary(LPCTSTR szDllPathName)
	{
		if(m_hDll) {ASSERT(0); return BVERR_ALREADY_CREATED;}

		// check os version
		OSVERSIONINFO osv;
		osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (GetVersionEx(&osv)==FALSE || 
			osv.dwPlatformId <=VER_PLATFORM_WIN32_WINDOWS)		// does not support 98
		{
			return BVERR_UNSUPPORTED_OS;
		}

		///e	// Load DLL
		///k	// DLL 로드
		m_hDll = LoadLibrary(szDllPathName);
		if(m_hDll==NULL)
		{
			m_hDll = LoadLibrary(BANDIVIDEO_RELEASE_DLL_FILE_NAME);
			if(m_hDll==NULL)
			{
				ASSERT(0);
				return BVERR_LOAD_LIBRARY_FAIL;
			}
		}

		m_pCreateBandiVideo = (LPCREATEBANDIVIDEO)GetProcAddress(m_hDll, "CreateBandiVideo");

		if(m_pCreateBandiVideo==NULL)
		{
			ASSERT(0);
			FreeLibrary(m_hDll);
			m_hDll = NULL;
			return BVERR_GET_PROC_ADDRESS_FAIL;
		}

		///e	// Create internal object
		///k	// 내부 객체 생성
		return CreateBandiVideo(m_pBVL);
	}

	///e	// Release Library and Unload
	///k	// 라이브러리 해제
	void DestroyLibrary()
	{
		if(m_pBVL) m_pBVL->Release();
		m_pBVL = NULL;

		m_pCreateBandiVideo = NULL;
		if(m_hDll) FreeLibrary(m_hDll);
		m_hDll = NULL;
	}

	BOOL IsLibraryLoaded()
	{
		return m_pCreateBandiVideo ? TRUE : FALSE;
	}

	BOOL IsCreated()
	{
		return m_pBVL ? TRUE : FALSE;
	}

	///e // 
	///e // IBandiVideo object create function
	///e // Except for several instances to create, please do not call this function directly.

	///k // IBandiVideo 객체 생성함수
	///k // 여러개의 인스턴스를 생성하는 경우를 제외하고는 이 함수는 직접 호출하지 
	///k // 마시기 바랍니다.

	HRESULT CreateBandiVideo(IBandiVideo*& pBVL)
	{
		HRESULT hr;

		///e // Check if library is initialized
		///k // 라이브러리 초기화 검사

		if(m_pCreateBandiVideo==NULL) {ASSERT(0); return BVERR_LIBRARY_NOT_LOADED;}

		///e // Create Bandi Video Library object
		///k // 비디오 재생객체 생성

		hr = m_pCreateBandiVideo(BANDI_VIDEO_LIBRARY_VERSION, (void**)&pBVL);
		if(FAILED(hr)){ASSERT(0);return hr;}

		return BVL_OK;
	}

public :	// IBandiVideo
	virtual	HRESULT	Create(HWND	hWnd, LPVOID context=NULL, BVL_CONTEXT_TYPE contextType=BVL_CONTEXT_DIRECTX9)
	{
		if (m_pBVL==NULL) 
		{
			HRESULT hr;
			hr=CreateBandiVideo(m_pBVL);
			ASSERT(m_pBVL);
		}
		return m_pBVL->Create(hWnd, context, contextType);
	}

	virtual	HRESULT	Release() 
	{
		//ASSERT(m_pBVL);
		HRESULT	hr=S_OK;
		if	(m_pBVL) hr=m_pBVL->Release();
		m_pBVL = NULL;
		return hr;
	}

	virtual HRESULT Verify(LPCSTR szID, LPCSTR szKey) 
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->Verify(szID, szKey);
	}

	virtual UINT GetVersion()
	{
		if(m_pBVL==NULL) {ASSERT(0); return (UINT)E_FAIL;}
		return m_pBVL->GetVersion();
	}

	virtual HRESULT Open(LPCWSTR szPathName,BVL_OPEN_OPT option=BVL_OPEN_OPT_NONE,
		BVL_PIXEL_FORMAT format=BVLPF_X8R8G8B8,BVL_AUDIO_DEVICE ad=BVL_AUDIO_DEVICE_DSOUND)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->Open(szPathName,option, format,ad);
	}

	virtual HRESULT Open(LPCSTR  szPathName,BVL_OPEN_OPT option=BVL_OPEN_OPT_NONE,
		BVL_PIXEL_FORMAT format=BVLPF_X8R8G8B8,BVL_AUDIO_DEVICE ad=BVL_AUDIO_DEVICE_DSOUND )
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->Open(szPathName,option, format,ad);
	}

	virtual HRESULT Open(IBandiVideoSource* pSource,BVL_OPEN_OPT option=BVL_OPEN_OPT_NONE,
		BVL_PIXEL_FORMAT format=BVLPF_X8R8G8B8,BVL_AUDIO_DEVICE ad=BVL_AUDIO_DEVICE_DSOUND )
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->Open(pSource,option, format,ad);
	}


	virtual HRESULT	Play(BOOL bRepeat=FALSE) 
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->Play(bRepeat);
	}

	virtual HRESULT	Seek(double	time) 
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->Seek(time);
	}

	virtual HRESULT	Stop(void)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->Stop();
	}

	virtual HRESULT	Close(void)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->Close();
	}

	virtual HRESULT OpenAsync(LPCWSTR szPathName,BVL_OPEN_OPT option=BVL_OPEN_OPT_NONE,
		BVL_PIXEL_FORMAT format=BVLPF_X8R8G8B8,BVL_AUDIO_DEVICE ad=BVL_AUDIO_DEVICE_DSOUND)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->OpenAsync(szPathName, option,format,ad );
	}
	virtual HRESULT OpenAsync(LPCSTR  szPathName,BVL_OPEN_OPT option=BVL_OPEN_OPT_NONE,
		BVL_PIXEL_FORMAT format=BVLPF_X8R8G8B8,BVL_AUDIO_DEVICE ad=BVL_AUDIO_DEVICE_DSOUND )
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->OpenAsync(szPathName, option,format,ad );
	}
	virtual HRESULT OpenAsync(IBandiVideoSource* pSource,BVL_OPEN_OPT option=BVL_OPEN_OPT_NONE,
		BVL_PIXEL_FORMAT format=BVLPF_X8R8G8B8,BVL_AUDIO_DEVICE ad=BVL_AUDIO_DEVICE_DSOUND )
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->OpenAsync(pSource, option,format,ad );
	}
	virtual HRESULT	PlayAsync(BOOL bRepeat=FALSE)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->PlayAsync(bRepeat);
	}
	virtual HRESULT	SeekAsync(double time)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->SeekAsync(time);
	}
	virtual HRESULT	StopAsync(void) 
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->StopAsync();
	}
	virtual HRESULT	CloseAsync(void) 
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->CloseAsync();
	}

	virtual HRESULT	Pause(void) 
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->Pause();
	}
	virtual HRESULT	Resume(void)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->Resume();
	}
	virtual HRESULT	SetVolume(UINT volume)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->SetVolume(volume);
	}

	virtual HRESULT	SetAlpha(UINT alpha)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->SetAlpha(alpha);
	}
	virtual BOOL	IsPlaying() 
	{
		if(m_pBVL==NULL) {ASSERT(0); return FALSE;}
		return m_pBVL->IsPlaying();
	}
	virtual	LPVOID	GetLastFrame(void)
	{
		if(m_pBVL==NULL) {ASSERT(0); return NULL;}
		return m_pBVL->GetLastFrame();
	}
	virtual	HRESULT	GetLastFrame(void **ppRenderTarget)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->GetLastFrame(ppRenderTarget);
	}
	virtual HRESULT	GetVideoInfo(BVLVideoInfo &info)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->GetVideoInfo(info);
	}
	virtual	HRESULT	ReleaseLastFrame(void* pRenderTarget)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->ReleaseLastFrame(pRenderTarget);
	}
	virtual	BOOL	CheckFrame(void)
	{
		if(m_pBVL==NULL) {return FALSE;}
		return m_pBVL->CheckFrame();
	}
	virtual	double	FramesToTime(INT frames)
	{
		if(m_pBVL==NULL) {return FALSE;}
		return m_pBVL->FramesToTime(frames);
	}
	virtual	INT		TimeToFrames(double time)
	{
		if(m_pBVL==NULL) {return FALSE;}
		return m_pBVL->TimeToFrames(time);
	}

public :
	///e	// type casting helper function
	///k	// GetLastFrame 함수를 인자에따라 다른 함수가 호출되게 함
	virtual	HRESULT	GetLastFrame(IBVLMemoryBuffer **ppRenderTarget)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->GetLastFrame((void**)ppRenderTarget);
	}
	virtual	HRESULT	GetLastFrame(IDirect3DTexture9 **ppRenderTarget)
	{
		if(m_pBVL==NULL) {ASSERT(0); return E_FAIL;}
		return m_pBVL->GetLastFrame((void**)ppRenderTarget);
	}


private :
	HMODULE				m_hDll;
	LPCREATEBANDIVIDEO	m_pCreateBandiVideo;
	IBandiVideo*		m_pBVL;
};

