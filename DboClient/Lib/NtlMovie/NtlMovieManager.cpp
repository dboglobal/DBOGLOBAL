#include "precomp_ntlmovie.h"
#include "NtlMovieManager.h"
#include "BandiVideo.h"
#include "NtlMovie.h"


HMODULE CNtlMovieManager::s_hMovieDll = NULL;
LPCREATEBANDIVIDEO CNtlMovieManager::s_pCreateBandiVideo = NULL;
CNtlMovieManager* CNtlMovieManager::s_pMovieManager = NULL;


CNtlMovieManager* CNtlMovieManager::GetInstance( void )
{
	return s_pMovieManager;
}

bool CNtlMovieManager::CreateInstance( HWND hWnd )
{
	if ( s_pMovieManager )
	{
		return true;
	}

	// OS Check - 98 은 지원하지 않는다
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	if ( GetVersionEx( &osv ) == FALSE || osv.dwPlatformId <= VER_PLATFORM_WIN32_WINDOWS )
	{
		return false;
	}

	// Load bandi library
	s_hMovieDll = LoadLibrary( BANDIVIDEO_DLL_FILE_NAME );
	if ( NULL == s_hMovieDll )
	{
		s_hMovieDll = LoadLibrary( BANDIVIDEO_RELEASE_DLL_FILE_NAME );
		if ( NULL == s_hMovieDll )
		{
			return false;
		}
	}

	// Get bandi interface
	s_pCreateBandiVideo = (LPCREATEBANDIVIDEO)GetProcAddress( s_hMovieDll, "CreateBandiVideo" );

	if ( NULL == s_pCreateBandiVideo )
	{
		FreeLibrary( s_hMovieDll );
		s_hMovieDll = NULL;

		return false;
	}

	s_pMovieManager = new CNtlMovieManager;

	if ( !s_pMovieManager->Create( hWnd ) )
	{
		delete s_pMovieManager;
		s_pMovieManager = NULL;

		s_pCreateBandiVideo = NULL;

		FreeLibrary( s_hMovieDll );
		s_hMovieDll = NULL;

		return false;
	}

	return true;
}

void CNtlMovieManager::DeleteInstance( void )
{
	if ( s_pMovieManager )
	{
		delete s_pMovieManager;
		s_pMovieManager = NULL;

		s_pCreateBandiVideo = NULL;

		FreeLibrary( s_hMovieDll );
		s_hMovieDll = NULL;
	}
}

CNtlMovieManager::CNtlMovieManager( void )
{
	m_hWnd = NULL;
	m_hDeviceRestore = NULL;
	m_hDeviceRelease = NULL;
}

CNtlMovieManager::~CNtlMovieManager( void )
{
	Delete();
}

bool CNtlMovieManager::Create( HWND hWnd )
{
	m_hWnd = hWnd;

	m_hDeviceRestore = CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRestore( this, &CNtlMovieManager::DeviceRestoreCallback );
	m_hDeviceRelease = CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRelease( this, &CNtlMovieManager::DeviceReleaseCallback );

	return true;
}

void CNtlMovieManager::Delete( void )
{
	DeleteMovieList();

	if ( m_hDeviceRelease )
	{
		CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRelease( m_hDeviceRelease );
		m_hDeviceRelease = NULL;
	}

	if ( m_hDeviceRestore )
	{
		CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRestore( m_hDeviceRestore );
		m_hDeviceRestore = NULL;
	}

	m_hWnd = NULL;
}

NTL_MOVIE_HANDLE CNtlMovieManager::CreateMovie( bool bAsync, int nX, int nY, int nWidth, int nHeight )
{
	CNtlMovie* pMovie = new CNtlMovie( bAsync );

	if ( NULL == pMovie )
	{
		return NULL;
	}

	if ( !pMovie->Create( m_hWnd, s_pCreateBandiVideo, nX, nY, nWidth, nHeight ) )
	{
		delete pMovie;

		return NULL;
	}

	m_defMovieList[pMovie] = pMovie;

	return (NTL_MOVIE_HANDLE)pMovie;
}

void CNtlMovieManager::DeleteMovie( NTL_MOVIE_HANDLE hMovie )
{
	mapdef_MovieList::iterator it = m_defMovieList.find( hMovie );

	if ( it != m_defMovieList.end() )
	{
		CNtlMovie* pMovie = it->second;

		pMovie->Delete();

		delete pMovie;

		m_defMovieList.erase( it );
	}
}

bool CNtlMovieManager::Open( NTL_MOVIE_HANDLE hMovie, const char* pMovieName )
{
	mapdef_MovieList::iterator it = m_defMovieList.find( hMovie );

	if ( it == m_defMovieList.end() )
	{
		return false;
	}

	return it->second->Open( pMovieName );
}

bool CNtlMovieManager::Close( NTL_MOVIE_HANDLE hMovie )
{
	mapdef_MovieList::iterator it = m_defMovieList.find( hMovie );

	if ( it == m_defMovieList.end() )
	{
		return false;
	}

	return it->second->Close();
}

bool CNtlMovieManager::IsPlaying( NTL_MOVIE_HANDLE hMovie )
{
	mapdef_MovieList::iterator it = m_defMovieList.find( hMovie );

	if ( it == m_defMovieList.end() )
	{
		return false;
	}

	return it->second->IsPlaying();
}

bool CNtlMovieManager::Play( NTL_MOVIE_HANDLE hMovie, bool bRepeat )
{
	mapdef_MovieList::iterator it = m_defMovieList.find( hMovie );

	if ( it == m_defMovieList.end() )
	{
		return false;
	}

	return it->second->Play( bRepeat );
}

bool CNtlMovieManager::Stop( NTL_MOVIE_HANDLE hMovie )
{
	mapdef_MovieList::iterator it = m_defMovieList.find( hMovie );

	if ( it == m_defMovieList.end() )
	{
		return false;
	}

	return it->second->Stop();
}

bool CNtlMovieManager::Pause( NTL_MOVIE_HANDLE hMovie )
{
	mapdef_MovieList::iterator it = m_defMovieList.find( hMovie );

	if ( it == m_defMovieList.end() )
	{
		return false;
	}

	return it->second->Pause();
}

bool CNtlMovieManager::Resume( NTL_MOVIE_HANDLE hMovie )
{
	mapdef_MovieList::iterator it = m_defMovieList.find( hMovie );

	if ( it == m_defMovieList.end() )
	{
		return false;
	}

	return it->second->Resume();
}

bool CNtlMovieManager::SetVolume( NTL_MOVIE_HANDLE hMovie, unsigned int uiVolume )
{
	mapdef_MovieList::iterator it = m_defMovieList.find( hMovie );

	if ( it == m_defMovieList.end() )
	{
		return false;
	}

	return it->second->SetVolume( uiVolume );
}

void CNtlMovieManager::SetPosition( NTL_MOVIE_HANDLE hMovie, int nX, int nY, int nWidth, int nHeight )
{
	mapdef_MovieList::iterator it = m_defMovieList.find( hMovie );

	if ( it == m_defMovieList.end() )
	{
		return;
	}

	it->second->SetPosition( nX, nY, nWidth, nHeight );
}

bool CNtlMovieManager::Render( NTL_MOVIE_HANDLE hMovie )
{
	mapdef_MovieList::iterator it = m_defMovieList.find( hMovie );

	if ( it == m_defMovieList.end() )
	{
		return false;
	}

	return it->second->Render();
}

int CNtlMovieManager::DeviceRestoreCallback( void )
{
	mapdef_MovieList::iterator it = m_defMovieList.begin();
	for ( ; it != m_defMovieList.end(); ++it )
	{
		CNtlMovie* pMovie = it->second;
		pMovie->DeviceRestoreCallback();
	}

	return 1;
}

int CNtlMovieManager::DeviceReleaseCallback( void )
{
	mapdef_MovieList::iterator it = m_defMovieList.begin();
	for ( ; it != m_defMovieList.end(); ++it )
	{
		CNtlMovie* pMovie = it->second;
		pMovie->DeviceReleaseCallback();
	}

	return 1;
}

void CNtlMovieManager::DeleteMovieList( void )
{
	mapdef_MovieList::iterator it = m_defMovieList.begin();
	for ( ; it != m_defMovieList.end(); ++it )
	{
		CNtlMovie* pMovie = it->second;
		delete pMovie;
	}
	m_defMovieList.clear();
}
