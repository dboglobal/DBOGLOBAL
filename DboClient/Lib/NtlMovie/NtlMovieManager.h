#ifndef _NTLMOVIE_MANAGER_H_
#define _NTLMOVIE_MANAGER_H_


#include "NtlDeviceRestoreHandler.h"
#include "NtlMovieDefine.h"


class CNtlMovie;


class CNtlMovieManager
{
// Declarations
public:
	typedef std::map< NTL_MOVIE_HANDLE, CNtlMovie* > mapdef_MovieList;


// Static member functions
protected:
	static HMODULE					s_hMovieDll;
	static LPCREATEBANDIVIDEO		s_pCreateBandiVideo;
	static CNtlMovieManager*		s_pMovieManager;

public:
	static CNtlMovieManager*		GetInstance( void );
	static bool						CreateInstance( HWND hWnd );
	static void						DeleteInstance( void );


// Constructions and Destructions
private:
	CNtlMovieManager( void );
	~CNtlMovieManager( void );


// Member variables
protected:
	HWND							m_hWnd;
	DEVICE_RESTORE_HANDLE			m_hDeviceRestore;
	DEVICE_RESTORE_HANDLE			m_hDeviceRelease;
	mapdef_MovieList				m_defMovieList;


// Operations
public:
	bool							Create( HWND hWnd );
	void							Delete( void );

	NTL_MOVIE_HANDLE				CreateMovie( bool bAsync, int nX, int nY, int nWidth, int nHeight );
	void							DeleteMovie( NTL_MOVIE_HANDLE hMovie );

	bool							Open( NTL_MOVIE_HANDLE hMovie, const char* pMovieName );
	bool							Close( NTL_MOVIE_HANDLE hMovie );

	bool							IsPlaying( NTL_MOVIE_HANDLE hMovie );
	bool							Play( NTL_MOVIE_HANDLE hMovie, bool bRepeat );
	bool							Stop( NTL_MOVIE_HANDLE hMovie );

	bool							Pause( NTL_MOVIE_HANDLE hMovie );
	bool							Resume( NTL_MOVIE_HANDLE hMovie );

	bool							SetVolume( NTL_MOVIE_HANDLE hMovie, unsigned int uiVolume );

	void							SetPosition( NTL_MOVIE_HANDLE hMovie, int nX, int nY, int nWidth, int nHeight );

	bool							Render( NTL_MOVIE_HANDLE hMovie );

// Implementations
protected:
	int								DeviceRestoreCallback( void );
	int								DeviceReleaseCallback( void );

	void							DeleteMovieList( void );
};


#endif