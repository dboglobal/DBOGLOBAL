#include "precomp_dboclient.h"
#include "DirectShowMovie.h"

// core
//#include "NtlDebug.h"
//
//// client
//#include "DboDef.h"
//
//HWND CDirectShowMovie::m_hMainWindow = 0;
//CDirectShowMovie::CDirectShowMovie() : m_pGraph(NULL),
//									   m_pControl(NULL),
//								       m_pEventEx(NULL),
//								       m_pVW(NULL),
//									   m_pBV(NULL),
//								       m_bPlay(false),
//									   m_eSize(ORIGINAL_SIZE),
//								       m_pCallMovieEvent(NULL)
//
//{
//	HRESULT hr = S_OK;
//	hr = CoInitialize(NULL);
//}
//
//CDirectShowMovie::~CDirectShowMovie()
//{
//	CoUninitialize();
//
//	if( IsPlay() == true )
//		Stop();
//
//	if(m_pCallMovieEvent != NULL)
//	{
//		NTL_DELETE( m_pCallMovieEvent );
//		m_pCallMovieEvent = NULL;
//	}
//}
//
//CDirectShowMovie* CDirectShowMovie::GetInstance()
//{
//	static CDirectShowMovie s_Instance;
//	return &s_Instance;
//}
//
//void CDirectShowMovie::Init(HWND hWnd, HANDLE hInstance)
//{
//	m_hMainWindow = hWnd;
//	m_hInstance = hInstance;
//}
//
//bool CDirectShowMovie::PlayWindow()
//{
//	HRESULT hr;
//	hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **)&m_pControl);
//	if (FAILED(hr))
//    {
//		return false;
//	}
//
//    hr = m_pGraph->QueryInterface(IID_IMediaEventEx, (void **)&m_pEventEx);
//	if (FAILED(hr))
//    {
//		return false;
//	}
//
//	hr = m_pGraph->QueryInterface(IID_IVideoWindow, (void **)&m_pVW);
//	if (FAILED(hr))
//    {
//		return false;
//	}
//
//	hr = m_pGraph->QueryInterface(IID_IBasicVideo, (void **)&m_pBV);
//	if (FAILED(hr))
//    {
//		return false;
//	}
//
//	
//	hr = m_pEventEx->SetNotifyWindow((OAHWND)m_hMainWindow, WM_DSHOW_MOVIE_PLAY_NOTIFY, 0);
//	if (FAILED(hr))
//    {
//		Stop();
//		return false;
//	}
//	
//	hr = m_pVW->put_AutoShow(OATRUE);
//	if (FAILED(hr))
//	{
//		Stop();
//		return false;
//	}
//	hr = m_pVW->put_Owner((OAHWND)m_hMainWindow);
//
//	if (FAILED(hr))
//    {
//		Stop();
//		return false;
//	}
//
//	hr = m_pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
//	if (FAILED(hr))
//    {
//		Stop();
//		return false;
//	}
//	m_pVW->HideCursor(OATRUE);
//	
//	Resize();
//
//	hr = m_pControl->Run();
//	if (FAILED(hr))
//    {
//		NTL_ASSERTE(0);
//		Stop();
//		return false;
//	}
//	m_pVW->put_MessageDrain((OAHWND) m_hMainWindow);
//
//	m_bPlay = true;
//
//	return true;
//}
//
//bool CDirectShowMovie::PlayFull()
//{
//	HRESULT hr;
//	hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **)&m_pControl);
//	if (FAILED(hr))
//    {
//		return false;
//	}
//
//    hr = m_pGraph->QueryInterface(IID_IMediaEventEx, (void **)&m_pEventEx);
//	if (FAILED(hr))
//    {
//		return false;
//	}
//
//	hr = m_pGraph->QueryInterface(IID_IVideoWindow, (void **)&m_pVW);
//	if (FAILED(hr))
//    {
//		return false;
//	}
//
//	hr = m_pGraph->QueryInterface(IID_IBasicVideo, (void **)&m_pBV);
//	if (FAILED(hr))
//    {
//		return false;
//	}
//
//	
//	hr = m_pEventEx->SetNotifyWindow((OAHWND)m_hMainWindow, WM_DSHOW_MOVIE_PLAY_NOTIFY, 0);
//	if (FAILED(hr))
//    {
//		Stop();
//		return false;
//	}
//	
//	hr = m_pVW->put_AutoShow(OATRUE);
//	if (FAILED(hr))
//	{
//		Stop();
//		return false;
//	}
//	/*
//	hr = m_pVW->put_Owner((OAHWND)m_hDialogWindow);
//	if (FAILED(hr))
//    {
//		Stop();
//		return false;
//	}
//
//	hr = m_pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
//	if (FAILED(hr))
//    {
//		Stop();
//		return false;
//	}
//	*/
//
//	m_pVW->HideCursor(OATRUE);
//	hr = m_pControl->Run();
//	if (FAILED(hr))
//    {
//		NTL_ASSERTE(0);
//		Stop();
//		return false;
//	}
//	
//	m_pVW->put_MessageDrain((OAHWND) m_hMainWindow);
//	m_pVW->put_FullScreenMode(OATRUE);
//
//	m_bPlay = true;
//
//	return true;
//}
//
//bool CDirectShowMovie::Play(const char *szFileName, EMoveSize eSize, bool bFullScreen)
//{
//	if( IsPlay() == true )
//		Stop();
//	
//	m_bFullScreen = bFullScreen;
//	
//	HRESULT hr;
//	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGraph);
//	if (FAILED(hr))
//    {
//		return false;
//	}
//	
//	WCHAR wszBuffer[MAX_PATH];
//	WCHAR wszBufferFileName[MAX_PATH];
//	char szPath[MAX_PATH];
//
//	GetCurrentDirectory(sizeof(szPath), szPath);
//	MultiByteToWideChar(GetACP(), 0, szPath, -1, wszBuffer, MAX_PATH);
//	MultiByteToWideChar(GetACP(), 0, szFileName, -1, wszBufferFileName, MAX_PATH);
//	swprintf_s(wszBuffer, MAX_PATH, L"%s%s%s", wszBuffer, "/", wszBufferFileName);
//	
//	hr = m_pGraph->RenderFile(wszBuffer, NULL);
//	
//	if (FAILED(hr))
//    {
//		Stop();
//		return false;
//	}
//
//	m_eSize = eSize;
//
//	if( m_bFullScreen )
//	{
//		return PlayFull();
//	}
//	else
//	{
//		return PlayWindow();
//	}
//
//	return true;
//}
//
//void CDirectShowMovie::Stop()
//{
//	m_bPlay = false;
//
//	/*	avooo's comment : 동영상 랜더링에서 애플리케이션으로 돌아올 때 WM_ACTIVATEAPP 메세지가
//						  발생한다. 이 때 m_pGraph->Release()를 호출하면 경우에 따라 다시
//						  WM_ACTIVATEAPP가 발생하는 경우가 있다. 
//						  여기서 다시 Stop()이 호출되고 m_pGraph->Release() 이 부분에서 뻑이난다.
//						  아마도 m_pEventEx->SetNotifyWindow((OAHWND)NULL, 0, 0) 에서 불필요한
//						  메세지를 보내듯 하여 주석 처리한다.
//	if(m_pEventEx != NULL)
//	{
//		m_pEventEx->SetNotifyWindow((OAHWND)NULL, 0, 0);
//	}
//	*/
//
//	if(m_pBV != NULL)
//	{
//		m_pBV->Release();
//		m_pBV = NULL;
//	}
//
//	if(m_pControl != NULL)
//	{
//		m_pControl->Release();
//		m_pControl = NULL;
//	}
//	
//	if(m_pEventEx != NULL)
//	{
//		m_pEventEx->Release();
//		m_pEventEx = NULL;
//	}
//
//	if(m_pVW != NULL)
//	{
//		m_pVW->Release();
//		m_pVW = NULL;
//	}
//
//	if(m_pGraph != NULL)
//	{
//		m_pGraph->Release();
//		m_pGraph = NULL;
//	}
//	
//	ShowWindow(m_hMainWindow, SW_RESTORE);	
//}
//
//bool CDirectShowMovie::HandleEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	if(!IsPlay())
//		return false;
//
//	if( message == WM_MOVE )
//	{
//		Resize();
//		return true;
//	}
//
//	if( (message == WM_KEYDOWN) ||
//		(message == WM_LBUTTONDOWN) ||
//		(message == WM_RBUTTONDOWN) ||
//		(message == WM_LBUTTONDBLCLK) ||
//		(message == WM_RBUTTONDBLCLK) ||
//		(message == WM_KILLFOCUS) ||
//		(message == WM_ACTIVATEAPP) ||
//		(message == WM_SIZE) )
//	{
//		if(IsPlay())
//			Stop();
//
//		if(m_pCallMovieEvent)
//				m_pCallMovieEvent->Call();
//		return true;
//	}
//
//	
//	if(message != WM_DSHOW_MOVIE_PLAY_NOTIFY)
//		return false;
//	LONG evCode;
//	LONG_PTR evParam1, evParam2;
//    HRESULT hr=S_OK;
//
//    if( m_pEventEx == NULL )
//	{
//		//NTL_ASSERTE(0);
//		return false;
//	}
//
//	while(SUCCEEDED(m_pEventEx->GetEvent(&evCode, &evParam1, &evParam2, 0)))
//    {
//        hr = m_pEventEx->FreeEventParams(evCode, evParam1, evParam2);
//
//		// Play가 끝았을 때 Application쪽으로 End Event를 보낸다.
//        if(EC_COMPLETE == evCode)
//        {
//			StopMovie();
//			break;
//		}
//    }
//	return true;
//}
//
//void CDirectShowMovie::StopMovie()
//{
//	if(IsPlay())
//	{
//		Stop();
//		if(m_pCallMovieEvent)
//			m_pCallMovieEvent->Call();
//	}
//}
//
//bool CDirectShowMovie::NotifyOwnerMessage(HWND hWnd, long uMsg, LONG_PTR wParam, LONG_PTR lParam)
//{
//	if( m_pVW == NULL)
//		return false;
//
//	HRESULT hr = S_OK;
//
//	hr = m_pVW->NotifyOwnerMessage((LONG_PTR) hWnd, uMsg, wParam, lParam);
//
//	if (FAILED(hr))
//    {
//		NTL_ASSERTE(0);
//		return false;
//	}
//
//	return true;
//}
//
//void CDirectShowMovie::ToggleFullScreen( bool bFullScreen )
//{
//	if( m_pVW == NULL )
//		return;
//
//	HRESULT hr=S_OK;
//	LONG lMode;
//
//	if( bFullScreen )
//	{
//		m_pVW->put_MessageDrain((OAHWND) m_hMainWindow);
//
//		lMode = OATRUE;
//		m_pVW->put_FullScreenMode(lMode);
//	}
//	else
//	{
//		m_pVW->put_MessageDrain((OAHWND) m_hMainWindow);
//
//		lMode = OAFALSE;
//		m_pVW->put_FullScreenMode(lMode);
//		m_pVW->SetWindowForeground(-1);
//	}
//}
//
//void CDirectShowMovie::Resize()
//{
//	if( m_bFullScreen )
//		return;
//
//	if( m_pVW == NULL ||
//		m_pBV == NULL)
//		return;
//
//	HRESULT hr;
//	if(m_pVW)
//	{
//		RECT rcView;
//		LONG videoRealWidth, videoRealHeight;
//		LONG videoLeft, videoTop, videoWidth, videoHeight;
//
//		GetClientRect(m_hMainWindow, &rcView);
//
//		hr = m_pBV->GetVideoSize(&videoRealWidth, &videoRealHeight);
//
//		if(m_eSize == ORIGINAL_SIZE)
//		{
//			videoLeft	= (rcView.right - videoRealWidth)/2;
//			videoWidth	= videoRealWidth;
//			videoTop	= (rcView.bottom - videoRealHeight)/2;
//			videoHeight = videoRealHeight;
//		}
//		else if(m_eSize == RATIO_FULL_SIZE)
//		{
//			float tempAddSize;
//
//			videoLeft = 0;
//			videoWidth = rcView.right - rcView.left;
//
//			tempAddSize = (float)(videoWidth - videoRealWidth) / (float)videoWidth;
//
//			videoTop = (rcView.bottom + rcView.top - videoRealHeight - LONG((float)videoRealHeight*tempAddSize)) / 2;
//			videoHeight = videoRealHeight + LONG((float)videoRealHeight*tempAddSize);			
//		}
//		else
//		{
//			float fScaleWidth, fScaleHeight;			
//
//			// 동영상의 가로, 세로의 길이가 화면의 그 길이와의 비율이 큰 쪽을
//			// 기준으로 동영상의 가로, 세로 길이를 늘려 화면에 꽉 차도록 한다.
//			fScaleWidth		= (float)(rcView.right	- rcView.left) / (float)videoRealWidth;
//			fScaleHeight	= (float)(rcView.bottom	- rcView.top)  / (float)videoRealHeight;
//
//			if( fScaleWidth > fScaleHeight )
//			{
//				videoWidth	= (LONG)(fScaleWidth * (float)videoRealWidth);
//				videoHeight	= (LONG)(fScaleWidth * (float)videoRealHeight);
//			}
//			else
//			{
//				videoWidth	= (LONG)(fScaleHeight * (float)videoRealWidth);
//				videoHeight	= (LONG)(fScaleHeight * (float)videoRealHeight);
//			}
//
//
//			// 화면의 중앙에 동영상이 오도록...
//			videoLeft	= ((rcView.right - rcView.left) - videoWidth)  / 2;
//			videoTop	= ((rcView.bottom - rcView.top) - videoHeight) / 2;
//		}
//
//		hr = m_pVW->SetWindowPosition(videoLeft, videoTop, videoWidth, videoHeight);
//		if (FAILED(hr))
//		{
//			NTL_ASSERTE(0);
//			Stop();
//			return;
//		}
//	}
//	if(m_pVW)
//	{
//		
//	}
//}
//
//
