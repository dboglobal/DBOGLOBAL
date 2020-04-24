#include "precomp_ntlmovie.h"
#include "NtlMovie.h"
#include <d3d9.h>
#include <D3DX9.h>
#include "rwcore.h"
#include "BandiVideo.h"


#define dBVL_NTL_ID			"NTLINC_DBO_20090326"
#define dBVL_NTL_PASSWARD	"f2a1cede"


CNtlMovie::CNtlMovie( bool bAsync )
{
	m_bAsync = bAsync;
	m_hWnd = NULL;
	m_pCreateBandiVideo = NULL;
	m_pBVL = NULL;
	m_pRenderTexture = NULL;

	m_bRepeat = true;

	memset( &m_sVideoInfo, 0, sizeof( m_sVideoInfo ) );

	m_bReset = false;
}

CNtlMovie::~CNtlMovie( void )
{
	Delete();
}

bool CNtlMovie::Create( HWND hWnd, LPCREATEBANDIVIDEO pCreateBandiVideo, int nX, int nY, int nWidth, int nHeight )
{
	if ( NULL == hWnd )
	{
		return false;
	}

	if ( NULL == pCreateBandiVideo )
	{
		return false;
	}

	m_hWnd = hWnd;

	m_pCreateBandiVideo = pCreateBandiVideo;

	m_nX = nX;
	m_nY = nY;
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	if ( FAILED( m_pCreateBandiVideo( BANDI_VIDEO_LIBRARY_VERSION, (void**)&m_pBVL ) ) )
	{
		m_pCreateBandiVideo = NULL;

		return false;
	}

	LPDIRECT3DDEVICE9 pD3DDevice = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();

	if ( FAILED( m_pBVL->Create( hWnd, pD3DDevice, BVL_CONTEXT_DIRECTX9 ) ) )
	{
		m_pCreateBandiVideo = NULL;

		m_pBVL->Release();
		m_pBVL = NULL;

		return false;
	}

	if ( FAILED( m_pBVL->Verify( dBVL_NTL_ID, dBVL_NTL_PASSWARD ) ) )
	{
		m_pCreateBandiVideo = NULL;

		m_pBVL->Release();
		m_pBVL = NULL;

		return false;
	}

	return true;
}

void CNtlMovie::Delete( void )
{
	if ( m_pBVL )
	{
		m_pBVL->Release();
		m_pBVL = NULL;
	}

	m_pCreateBandiVideo = NULL;
}

bool CNtlMovie::Open( const char* pMovieName )
{
	if ( NULL == pMovieName )
	{
		return false;
	}

	if ( NULL == m_pBVL )
	{
		return false;
	}

	m_strFileName = pMovieName;

	HRESULT hr;
	if ( m_bAsync )
	{
		hr = m_pBVL->OpenAsync( pMovieName, BVL_OPEN_OPT_NONE, BVLPF_X8R8G8B8, BVL_AUDIO_DEVICE_DSOUND );
	}
	else
	{
		hr = m_pBVL->Open( pMovieName, BVL_OPEN_OPT_NONE, BVLPF_X8R8G8B8, BVL_AUDIO_DEVICE_DSOUND );
	}

	return FAILED( hr ) ? false : true;
}

bool CNtlMovie::Close( void )
{
	if ( NULL == m_pBVL )
	{
		return false;
	}

	HRESULT hr;
	if ( m_bAsync )
	{
		hr = m_pBVL->CloseAsync();
	}
	else
	{
		hr = m_pBVL->Close();
	}

	return FAILED( hr ) ? false : true;
}

bool CNtlMovie::IsPlaying( void )
{
	if ( NULL == m_pBVL )
	{
		return false;
	}

	return m_pBVL->IsPlaying() ? true : false;
}

bool CNtlMovie::Play( bool bRepeat )
{
	if ( NULL == m_pBVL )
	{
		return false;
	}

	m_bRepeat = bRepeat;

	HRESULT hr;
	if ( m_bAsync )
	{
		hr = m_pBVL->PlayAsync( bRepeat ? TRUE : FALSE );
	}
	else
	{
		hr = m_pBVL->Play( bRepeat ? TRUE : FALSE );
	}

	m_pBVL->GetVideoInfo( m_sVideoInfo );

	return FAILED( hr ) ? false : true;
}

bool CNtlMovie::Stop( void )
{
	if ( NULL == m_pBVL )
	{
		return false;
	}

	HRESULT hr;
	if ( m_bAsync )
	{
		hr = m_pBVL->StopAsync();
	}
	else
	{
		hr = m_pBVL->Stop();
	}

	return FAILED( hr ) ? false : true;
}

bool CNtlMovie::Pause( void )
{
	if ( NULL == m_pBVL )
	{
		return false;
	}

	if ( FAILED( m_pBVL->Pause() ) )
	{
		return false;
	}

	return true;
}

bool CNtlMovie::Resume( void )
{
	if ( NULL == m_pBVL )
	{
		return false;
	}

	if ( FAILED( m_pBVL->Resume() ) )
	{
		return false;
	}

	return true;
}

bool CNtlMovie::SetVolume( unsigned int uiVolume )
{
	if ( NULL == m_pBVL )
	{
		return false;
	}

	if ( FAILED( m_pBVL->SetVolume( uiVolume ) ) )
	{
		return false;
	}

	return true;
}

void CNtlMovie::SetPosition( int nX, int nY, int nWidth, int nHeight )
{
	m_nX = nX;
	m_nY = nY;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

bool CNtlMovie::Render( void )
{
	if ( m_bReset )
	{
		return true;
	}

	if ( NULL == m_pBVL )
	{
		return false;
	}

	if ( !UpdateRenderTexture() )
	{
		return false;
	}

	float fTop		= (float)m_nY;
	float fBottom	= (float)m_nY + (float)m_nHeight;
	float fLeft		= (float)m_nX;
	float fRight	= (float)m_nX + (float)m_nWidth;

	struct sVERTEX
	{
		float x, y, z, w;
		float u, v;
	};

	sVERTEX sVertex[4] =
	{
		{ fLeft,		fTop,		0.f,		1.f,		0.f,		0.f },
		{ fLeft,		fBottom,	0.f,		1.f,		0.f,		1.f },
		{ fRight,		fTop,		0.f,		1.f,		1.f,		0.f },
		{ fRight,		fBottom,	0.f,		1.f,		1.f,		1.f },
	};

	LPDIRECT3DDEVICE9 pD3DDevice = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();

	pD3DDevice->SetTexture( 0, (LPDIRECT3DTEXTURE9)m_pRenderTexture );
	pD3DDevice->SetIndices( NULL );
	pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, sVertex, sizeof( sVERTEX ) );

	return true;
}

int CNtlMovie::DeviceRestoreCallback( void )
{
	m_bReset = false;

	if ( m_pBVL && !m_strFileName.empty() )
	{
		HRESULT hr;

		hr = m_pBVL->OpenAsync( m_strFileName.c_str(), BVL_OPEN_OPT_NONE, BVLPF_X8R8G8B8, BVL_AUDIO_DEVICE_DSOUND );
		hr = m_pBVL->SeekAsync( m_sVideoInfo.CurrentPlayTime );
		hr = m_pBVL->PlayAsync( m_bRepeat );
	}

	return 1;
}

int CNtlMovie::DeviceReleaseCallback( void )
{
	m_bReset = true;

	if ( m_pRenderTexture )
	{
		((LPDIRECT3DTEXTURE9)m_pRenderTexture)->Release();
		m_pRenderTexture = NULL;
	}

	if ( m_pBVL )
	{
		HRESULT hr;

		hr = m_pBVL->GetVideoInfo( m_sVideoInfo );
		hr = m_pBVL->Close();
	}

	return 1;
}

bool CNtlMovie::UpdateRenderTexture( void )
{
	BVLVideoInfo sBandiInfo;
	if ( FAILED( m_pBVL->GetVideoInfo( sBandiInfo ) ) )
	{
		return false;
	}

	if ( !m_pBVL->CheckFrame() )
	{
		return true;
	}

	IDirect3DTexture9* pRenderTexture;
	if ( FAILED( m_pBVL->GetLastFrame( (void**)&pRenderTexture ) ) )
	{
		return true;
	}

	LPDIRECT3DDEVICE9 pD3DDevice = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();

	if ( NULL == m_pRenderTexture )
	{
		if ( FAILED( pD3DDevice->CreateTexture( sBandiInfo.width, sBandiInfo.height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, (LPDIRECT3DTEXTURE9*)&m_pRenderTexture, NULL ) ) )
		{
			m_pBVL->ReleaseLastFrame( pRenderTexture );

			return false;
		}
	}

	struct sVERTEX
	{
		float x, y, z, w;
		float u, v;
	};

	sVERTEX sVertex[4] =
	{
		{ 0.f,						0.f,						0.f,		1.f,		0.f,		0.f },
		{ 0.f,						(float)sBandiInfo.height,	0.f,		1.f,		0.f,		1.f },
		{ (float)sBandiInfo.width,	0.f,						0.f,		1.f,		1.f,		0.f },
		{ (float)sBandiInfo.width,	(float)sBandiInfo.height,	0.f,		1.f,		1.f,		1.f },
	};

	LPDIRECT3DSURFACE9 pOldSurface, pSurface;

	pD3DDevice->GetRenderTarget( 0, &pOldSurface );

	if ( SUCCEEDED( ((LPDIRECT3DTEXTURE9)m_pRenderTexture)->GetSurfaceLevel( 0, &pSurface ) ) )
	{
		pD3DDevice->SetRenderTarget( 0, pSurface );

		if ( pSurface )
		{
			pSurface->Release();
			pSurface = NULL;
		}

		pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );

		pD3DDevice->SetTexture( 0, pRenderTexture );
		pD3DDevice->SetIndices( NULL );
		pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
		pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, sVertex, sizeof( sVERTEX ) );

		pD3DDevice->SetRenderTarget( 0, pOldSurface );

		if ( pOldSurface )
		{
			pOldSurface->Release();
			pOldSurface = NULL;
		}

		m_pBVL->ReleaseLastFrame( pRenderTexture );

		return true;
	}

	return false;
}
