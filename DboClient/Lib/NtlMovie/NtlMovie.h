#ifndef _NTL_MOVIE_H_
#define _NTL_MOVIE_H_


#include "NtlMovieDefine.h"
#include "BandiVideo.h"
#include <string>

interface IBandiVideo;


class CNtlMovie
{
// Member variables
protected:
	bool							m_bAsync;
	HWND							m_hWnd;
	LPCREATEBANDIVIDEO				m_pCreateBandiVideo;
	IBandiVideo*					m_pBVL;
	std::string						m_strFileName;
	int								m_nX, m_nY, m_nWidth, m_nHeight;
	void*							m_pRenderTexture;

	bool							m_bRepeat;

	BVLVideoInfo					m_sVideoInfo;

	bool							m_bReset;


// Constructions and Destructions
public:
	CNtlMovie( bool bAsync );
	~CNtlMovie( void );


// Operations
public:
	bool							Create( HWND hWnd, LPCREATEBANDIVIDEO pCreateBandiVideo, int nX, int nY, int nWidth, int nHeight );

	void							Delete( void );

	bool							Open( const char* pMovieName );

	bool							Close( void );

	bool							IsPlaying( void );

	bool							Play( bool bRepeat );

	bool							Stop( void );

	bool							Pause( void );

	bool							Resume( void );

	bool							SetVolume( unsigned int uiVolume );

	void							SetPosition( int nX, int nY, int nWidth, int nHeight );

	bool							Render( void );

	int								DeviceRestoreCallback( void );

	int								DeviceReleaseCallback( void );


// Implementations
protected:
	bool							UpdateRenderTexture( void );
};


#endif