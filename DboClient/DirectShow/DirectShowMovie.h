/*****************************************************************************
 *
 * File			: DShowMoviePlay.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2007. 6. 24	
 * Abstract		: NTl DShowMoviePlay
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

/**
 * MS의 DirectShow를 wrapping 한 class 이다.
 * Play가 끝났을 때 Application쪽으로 CallBack으로 Event를 보낸다. 현재는 Movie
 * 의 End 까지 Play가 되었을 경우만 처리를 한다.
 * DirectX Rendering시 화면 떨리는 문제(동영상 Play시 Rendering을 안되게 한다.)
 * 전체 화면 Toggle 시의 문제(동영상 Play시는 Toggle이 안되게 한다.)
 */
#ifndef __DSHOW_MOVIE_PLAY_H__
#define __DSHOW_MOVIE_PLAY_H__
/*
#include <dshow.h>
#include "NtlCallbackWidget.h"

#define WM_DSHOW_MOVIE_PLAY_NOTIFY  WM_USER+300

class CDirectShowMovie
{
public:

	CDirectShowMovie();
	~CDirectShowMovie();

	enum EMoveSize
	{
		ORIGINAL_SIZE,
		RATIO_FULL_SIZE,
		FULL_SIZE
	};

protected:

	IGraphBuilder 	*m_pGraph;
	IMediaControl 	*m_pControl;
	IMediaEventEx	*m_pEventEx;
	
	IVideoWindow	*m_pVW;
	IBasicVideo		*m_pBV;

	HANDLE			m_hInstance;
	static HWND		m_hMainWindow;							// Window Mode Movie Play 용
	
	bool			m_bPlay;
	bool			m_bFullScreen;
	EMoveSize		m_eSize;

	CNtlCallbackParam0 *m_pCallMovieEvent;					///< 현재는 Movie가 종료만 처리

protected:
	bool	PlayWindow();
	void	Stop();

public:
	static CDirectShowMovie *GetInstance();
	
	void	Init(HWND hWnd, HANDLE hInstace);
	bool	Play(const char *szFileName, EMoveSize eSize, bool bFullScreen = FALSE);
	
	void	StopMovie();	

	bool	IsPlay() { return m_bPlay; }
	
	bool	HandleEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	template <class Callbackclass>
	void SetCallBack(Callbackclass *cbclass,int (Callbackclass::*callback)(void));

	void	Resize();

	bool	NotifyOwnerMessage(HWND hWnd, long uMsg, LONG_PTR wParam, LONG_PTR lParam);
	void	ToggleFullScreen(bool bFullScreen = FALSE);
	bool	PlayFull();
};

template <class Callbackclass>
void CDirectShowMovie::SetCallBack(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	if(m_pCallMovieEvent != NULL)
	{
		NTL_DELETE( m_pCallMovieEvent );
		m_pCallMovieEvent = NULL;
	}

	m_pCallMovieEvent = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
}
*/
#endif
