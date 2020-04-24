#include "precomp_dboclient.h"
#include "MoviePlayer.h"

#include <D3D9.h>
#include <D3DX9.h>

// core
#include "NtlDebug.h"

// sound
#include "NtlSoundLogic.h"

// client
#include "DboDef.h"
#include "DboApplication.h"


#define dBVL_NTL_ID			"NTLINC_DBO_20090326"
#define dBVL_NTL_PASSWARD	"f2a1cede"

#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZRHW | D3DFVF_TEX1 )

struct Vertex
{
	RwReal x, y, z, w;
	RwReal tu, tv;
};


CMoviePlayer* CMoviePlayer::m_pInstance = NULL;

UINT WM_BVNOTIFY_EOF = ::RegisterWindowMessage(_T("WM_BVNOTIFY_EOF"));

RwBool CMoviePlayer::CreateInstance()
{
	NTL_FUNCTION("CMoviePlayer::CreateInstance");

	if( m_pInstance )
	{
		DBO_FAIL("CMoviePlayer instance already exist");
		NTL_RETURN(FALSE);
	}

	m_pInstance = NTL_NEW CMoviePlayer;
	if( !m_pInstance )
	{
		DBO_FAIL("CMoviePlayer::CreateInstance : Instance Allocate Fail");
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}

VOID CMoviePlayer::DestoryInstance()
{
	NTL_FUNCTION("CMoviePlayer::DestoryInstance");

	NTL_DELETE(m_pInstance);

	NTL_RETURNVOID();
}

CMoviePlayer* CMoviePlayer::GetInstance()
{
	return m_pInstance;
}

CMoviePlayer::CMoviePlayer()
:m_pCallback(NULL)
,m_pVertexBuffer(NULL)
,m_eSize(MOVIE_SIZE_ORIGINAL)
,m_bPlay(FALSE)
,m_bRelease(FALSE)
,m_fVolume(1.f)
{

}

CMoviePlayer::~CMoviePlayer()
{
	Stop();
}

RwBool CMoviePlayer::Play(const char* pcFileName, eMovieSizeType eSizeType)
{
	if( !pcFileName )
		return FALSE;

	if( IsPlay() )
		return FALSE;

	if( false == ReadyMovie(pcFileName, eSizeType) )
		return FALSE;	

	m_handleReleaseHandler = CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRelease(this, &CMoviePlayer::ReleaseHandler);

	m_strMovieFileName		= pcFileName;
	m_eSize					= eSizeType;
	m_bPlay					= TRUE;

	SetVolume(m_fVolume);

	return TRUE;
}

RwBool CMoviePlayer::ReadyMovie(const char* pcFileName, eMovieSizeType eSizeType)
{
	if( !pcFileName )
	{
		DBO_FAIL("Invalid movie file name");
		return FALSE;
	}

	if( strlen(pcFileName) == 0 )
	{
		DBO_FAIL("movie file name is empty");
		return FALSE;
	}

	HRESULT hr = m_BVL.InitLibrary(BANDIVIDEO_DLL_FILE_NAME);

	if(FAILED(hr))
	{
		DBO_FAIL("Can not open library file of name " << BANDIVIDEO_DLL_FILE_NAME);
		return FALSE;
	}

	LPDIRECT3DDEVICE9 pD3DDevice = static_cast<LPDIRECT3DDEVICE9>( RwD3D9GetCurrentD3DDevice() );

	hr = m_BVL.Create(CNtlApplication::GetInstance()->GetHWnd(), pD3DDevice, BVL_CONTEXT_DIRECTX9);

	if(FAILED(hr))
	{
		DBO_FAIL("Can not create BVL library");
		m_BVL.DestroyLibrary();
		return FALSE;
	}


	// 데모버전의 로고표시를 없애려면 구매한 제품ID 와 인증키를 여기에 사용하여야 합니다.
	m_BVL.Verify(dBVL_NTL_ID, dBVL_NTL_PASSWARD);


	// 동영상 파일 열기, 오프닝 동영상은 OpenAsync() 대신 Open()을 사용하는것이 좋습니다.
	hr = m_BVL.Open(pcFileName, BVL_OPEN_OPT_AUTOPLAY);
	if(FAILED(hr))
	{
		DBO_FAIL("Can not open movie file of name " << pcFileName);
		m_BVL.Close();
		m_BVL.Release();
		m_BVL.DestroyLibrary();
		return FALSE;
	}

	RwInt32 iScreenWidth		= (RwInt32)GetDboGlobal()->GetScreenWidth();
	RwInt32 iScreenHeight		= (RwInt32)GetDboGlobal()->GetScreenHeight();
	BVLVideoInfo info;

	m_BVL.GetVideoInfo(info);
	pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );


	Vertex SurfaceVertices[4];

	switch(eSizeType)
	{
	case MOVIE_SIZE_ORIGINAL:
		{
			// 동영상의 가로, 세로의 해상도를 보존하면서 화면 중앙에 위치하도록 한다
			RwReal fMovieTop	= (RwReal)((iScreenHeight - info.height) / 2.f);
			RwReal fMovieBottom	= fMovieTop + info.height;
			RwReal fMovieLeft	= (RwReal)((iScreenWidth - info.width) / 2.f);
			RwReal fMovieRight	= fMovieLeft + (RwReal)info.width;

			Vertex vertices[4] =
			{
				{fMovieLeft,	fMovieTop,		0.0f,	1.f,	0.0f,	0.0f },
				{fMovieLeft,	fMovieBottom,	0.0f,	1.f,	0.0f,	1.0f },
				{fMovieRight,	fMovieTop,		0.0f,	1.f,	1.0f,	0.0f },
				{fMovieRight,	fMovieBottom,	0.0f,	1.f,	1.0f,	1.0f },
			};

			memcpy(SurfaceVertices, vertices, sizeof(SurfaceVertices));
			break;
		}
	case MOVIE_SIZE_RATIO_FULL:
		{
			// 동영상의 수평 해상도와 화면의 해상도 수평 해상도와의 비율에 따라 동영상의 수직 길이를 조절한다
			RwReal fAdjustMovieHight	= ((RwReal)iScreenWidth / (RwReal)info.width) * (RwReal)info.height;
			RwReal fMovieTop			= ((RwReal)iScreenHeight - fAdjustMovieHight) / 2.f;
			RwReal fMovieBottom			= fMovieTop + fAdjustMovieHight;

			Vertex vertices[4] =
			{
				{0.f,					fMovieTop,		0.0f,	1.f,	0.0f,	0.0f },
				{0.f,					fMovieBottom,	0.0f,	1.f,	0.0f,	1.0f },
				{(RwReal)iScreenWidth,	fMovieTop,		0.0f,	1.f,	1.0f,	0.0f },
				{(RwReal)iScreenWidth,	fMovieBottom,	0.0f,	1.f,	1.0f,	1.0f },
			};

			memcpy(SurfaceVertices, vertices, sizeof(SurfaceVertices));
			break;
		}
	case MOVIE_SIZE_FULL:
		{
			// 동영상의 가로, 세로의 해상도가 화면의 해상도 수평 해상도와의 비율이 큰 쪽을
			// 기준으로 동영상의 가로, 세로 길이를 늘려 화면에 꽉 차도록 한다.
			RwReal fScaleWidth, fScaleHeight;
			RwReal fMovieTop, fMovieBottom, fMovieLeft, fMovieRight;

			fScaleWidth		= (RwReal)iScreenWidth  / (RwReal)info.width;
			fScaleHeight	= (RwReal)iScreenHeight / (RwReal)info.height;

			if( fScaleWidth > fScaleHeight )
			{
				fMovieTop	= 0.f;
				fMovieBottom= fScaleWidth * (RwReal)info.height;
				fMovieLeft	= 0.f;
				fMovieRight	= (RwReal)iScreenWidth;
			}
			else
			{
				fMovieTop	= 0.f;
				fMovieBottom= (RwReal)iScreenHeight;
				fMovieLeft	= 0.f;
				fMovieRight	= fScaleHeight * (RwReal)info.width;
			}

			Vertex vertices[4] =
			{
				{0.f,					fMovieTop,		0.0f,	1.f,	0.0f,	0.0f },
				{0.f,					fMovieBottom,	0.0f,	1.f,	0.0f,	1.0f },
				{(RwReal)iScreenWidth,	fMovieTop,		0.0f,	1.f,	1.0f,	0.0f },
				{(RwReal)iScreenWidth,	fMovieBottom,	0.0f,	1.f,	1.0f,	1.0f },
			};

			memcpy(SurfaceVertices, vertices, sizeof(SurfaceVertices));
			break;
		}
	default:
		{
			DBO_FAIL("Invalid movie size type");
			m_BVL.Close();
			m_BVL.Release();
			m_BVL.DestroyLibrary();
			return FALSE;
		}
	}	

	pD3DDevice->CreateVertexBuffer(sizeof(SurfaceVertices),0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL );

	void* pVertices = NULL;
	m_pVertexBuffer->Lock( 0, sizeof(SurfaceVertices), (void**)&pVertices, 0 );
	memcpy( pVertices, SurfaceVertices, sizeof(SurfaceVertices) );
	m_pVertexBuffer->Unlock();

	pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian( 45.0f ),
		(RwReal)iScreenWidth / (RwReal)iScreenHeight,
		0.1f, 100.0f);
	pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	return TRUE;
}

RwBool CMoviePlayer::Stop()
{
	RwBool bResult = false;

	if( m_bPlay )
	{
		m_bPlay = FALSE;		

		CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRelease(m_handleReleaseHandler);

		m_strMovieFileName.clear();

		StopMovie();
		bResult = true;
	}

	if(m_pCallback)
	{
		m_pCallback->Call();
		NTL_DELETE(m_pCallback);
	}

	return bResult;
}

VOID CMoviePlayer::StopMovie()
{
	if(m_BVL.IsCreated())
	{
		m_BVL.Close();
		m_BVL.DestroyLibrary();
	}

	if( m_pVertexBuffer )
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}
}

VOID CMoviePlayer::SetVolume(RwReal fVolume)
{
	m_fVolume = Logic_GetFMODValidVolume(fVolume);

	if( m_BVL.IsCreated() )
	{
		UINT uiVolume = (UINT)(m_fVolume * 255.f);
		m_BVL.SetVolume(uiVolume);
	}
}

RwReal CMoviePlayer::GetVolume()
{
	return m_fVolume;
}

RwBool CMoviePlayer::IsPlay()
{
	return m_bPlay;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///         D3D 공간에 사각 평면을 생성하고 거기에 동영상 텍스처를 입힘
/// @param  
/// @return 
/// @date   Thursday, February 05, 2009  11:08:42 AM
////////////////////////////////////////////////////////////////////////////////////////////////////
void CMoviePlayer::RenderD3D(IDirect3DTexture9 *texture)
{
	LPDIRECT3DDEVICE9 pD3DDevice = static_cast<LPDIRECT3DDEVICE9>( RwD3D9GetCurrentD3DDevice() );
	HWND hWnd = CNtlApplication::GetInstance()->GetHWnd();

	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f), 1.0f, 0 );

	D3DXMATRIX matWorld;
	D3DXMATRIX matTrans;
	D3DXMATRIX matRot;

	D3DXMatrixTranslation( &matTrans, 0.0f, 0.0f, 0.f );

	D3DXMatrixRotationYawPitchRoll( &matRot,D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0) );

	matWorld = matRot * matTrans;
	pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	pD3DDevice->BeginScene();
	pD3DDevice->SetTexture( 0, texture);
	pD3DDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(Vertex) );
	pD3DDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  0, 2 );
	pD3DDevice->EndScene();

	pD3DDevice->Present( NULL, NULL, hWnd, NULL );
}

VOID CMoviePlayer::Render()
{
	if( m_bRelease )
	{
		Release();		
		return;
	}

	if( false == IsPlay() )
		return;

	// 새로운 프레임이 있는지 확인한다.
	if(m_BVL.CheckFrame())
	{
		HRESULT hr;
		IDirect3DTexture9* curframe;

		// 프레임 데이타를 가져온다.
		hr = m_BVL.GetLastFrame(&curframe);
		if( FAILED(hr) )
			return;

		// 화면 출력
		RenderD3D(curframe);

		// 작업 완료
		m_BVL.ReleaseLastFrame(curframe);
	}
}

RwInt32 CMoviePlayer::ReleaseHandler()
{
	if( IsPlay() )
		m_bRelease = TRUE;

	return 1;
}

VOID CMoviePlayer::Release()
{
	m_bRelease = FALSE;

	Stop();
	//StopMovie();
	//ReadyMovie(m_strMovieFileName.c_str(), m_eSize);
	//SetVolume(m_fVolume);
}

eMOVIE_HANDLE_EVENT_RESULT CMoviePlayer::HandleEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( hWnd != CNtlApplication::GetInstance()->GetHWnd() )
		return MOVIE_HANDLE_EVENT_OK;

	switch(message)
	{
	case WM_KEYDOWN:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			if ( IsPlay() )
			{
				Stop();
				return MOVIE_HANDLE_EVENT_STOP_MOVIE;
			}
		}
		break;

	case WM_ACTIVATE:
		{
			// Minimize 또는 Active 비활성화
			if ( LOWORD( wParam ) == WA_INACTIVE || ::IsIconic( hWnd ) )
			{
				if ( IsPlay() )
				{
					Stop();
					return MOVIE_HANDLE_EVENT_STOP_MOVIE;
				}
			}
		}
		break;
	}

	if ( WM_BVNOTIFY_EOF == message )
	{
		if ( Stop() )
		{
			return MOVIE_HANDLE_EVENT_STOP_MOVIE;
		}
	}

	return MOVIE_HANDLE_EVENT_OK;
}