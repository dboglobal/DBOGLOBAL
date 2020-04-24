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

#ifndef __MOVIE_PLAYER__
#define __MOVIE_PLAYER__

// core
#include "NtlDeviceRestoreHandler.h"
#include "NtlCallbackWidget.h"

// client
#include "BandiVideo.h"

enum eMovieSizeType
{
	MOVIE_SIZE_ORIGINAL,
	MOVIE_SIZE_RATIO_FULL,
	MOVIE_SIZE_FULL,
};

enum eMOVIE_HANDLE_EVENT_RESULT
{
	MOVIE_HANDLE_EVENT_OK,
	MOVIE_HANDLE_EVENT_STOP_MOVIE,
};

class CMoviePlayer
{
public:
	static RwBool	CreateInstance();
	static VOID		DestoryInstance();
	static CMoviePlayer*	GetInstance();

	CMoviePlayer();
	virtual ~CMoviePlayer();

	RwBool			Play(const char* pcFileName, eMovieSizeType eSizeType);
	VOID			Render();
	RwBool			Stop();

	VOID			SetVolume(RwReal fVolume);
	RwReal			GetVolume();

	RwBool			IsPlay();

	eMOVIE_HANDLE_EVENT_RESULT HandleEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	template <class Callbackclass>
	void SetCallBack(Callbackclass *cbclass,RwInt32 (Callbackclass::*callback)(void));

protected:
	RwBool			ReadyMovie(const char* pcFileName, eMovieSizeType eSizeType);
	VOID			StopMovie();

	void			RenderD3D(IDirect3DTexture9 *texture);
	RwInt32			ReleaseHandler();
	VOID			Release();

protected:
	static CMoviePlayer* m_pInstance;

	CBandiVideoLibrary		m_BVL;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	CNtlCallbackParam0*		m_pCallback;

	DEVICE_RESTORE_HANDLE	m_handleReleaseHandler;

	std::string				m_strMovieFileName;

	eMovieSizeType			m_eSize;
	RwBool					m_bPlay;		///< BVL에서 제공하는 IsPlay 함수는 BVL 라이브러리를 불러오기 전까지 쓸 수가 없다	
	RwBool					m_bRelease;
	RwReal					m_fVolume;
};

static CMoviePlayer* GetMoviePlayer()
{
	return CMoviePlayer::GetInstance();
}

template <class Callbackclass>
void CMoviePlayer::SetCallBack(Callbackclass *cbclass,RwInt32 (Callbackclass::*callback)(void))
{
	if(m_pCallback != NULL)
	{
		DBO_FAIL("Already exist movie callback");
		return;
	}

	m_pCallback = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
}

#endif
