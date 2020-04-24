#include "precomp_ntlflasher.h"
#include "NtlMath.h"

#include "NtlFlasher.h"
#include "NtlFlasherManager.h"



CNtlFlasher::CNtlFlasher(VOID)
{
	m_nScreenX = 0;
	m_nScreenY = 0;
	m_nWidth = 1;
	m_nHeight = 1;
	m_nResolutionWidth = 0;
	m_nResolutionHeight = 0;

	m_bPlay = FALSE;
	m_bAutoRewind = FALSE;
	m_bFrameCallback = FALSE;
	m_bUseFont = FALSE;
	m_bOutofScreen = FALSE;
	m_eScaleModeType = GFxMovieView::SM_NoScale;

	m_pImageCreator = *NTL_NEW GFxImageCreator( true );

	m_strFileName = "Unknown";
	m_id = 0;
}

CNtlFlasher::~CNtlFlasher(VOID)
{
	m_pImageCreator = NULL;

	if (m_id > 0)
	{
		GetFlasherManager()->GetCommandHandler()->UnLinkFSCallBack(m_id); // unlink (do this in destructor and not in Destroy() cost we might call Destroy() in other functions)
	}

	Destroy();
}

BOOL CNtlFlasher::Create( CONST CHAR* szFileName, int nID)
{
	GFxLoader* loader = GetFlasherManager()->GetLoader();
	
	loader->SetImageCreator( m_pImageCreator );

	m_strFileName = szFileName;
	m_id = nID;

	if( m_bUseFont )
	{
		loader->SetFontMap( GetFlasherManager()->GetFontMap() );
		loader->SetFontLib( GetFlasherManager()->GetFontLib() );
	}

	std::string strSwfFileName = szFileName;
	std::string strPath = szFileName;
	std::string strName;
	std::string strGfxtFileName;

	if( !strSwfFileName.size() )
		return FALSE;

	std::string::size_type pos = strPath.rfind( ".swf" );

	if( pos == std::string::npos )
		strSwfFileName += ".swf";
	else
		strPath.erase( pos );
	
	strName = strPath;
	pos = strName.rfind( "\\" );

	if( pos != std::string::npos )
		strName.erase( 0, pos + 1 );	

	strGfxtFileName = strPath + "\\" + strName + ".swf";
	
	if( !( m_pMovieDef = *loader->CreateMovie(strGfxtFileName.c_str(), GFxLoader::LoadWaitCompletion) ) )
	{
		Destroy();

		DBO_WARNING_MESSAGE(strGfxtFileName.c_str() << " Flash Load Fail");

		return FALSE;		
	}

	if( !( m_pMovie = *m_pMovieDef->CreateInstance(/*false*/) ) ) // by daneos: Dont enable "false" here, or some flash might not work. As example "you win" and "you lose" in ranking.
	{
		Destroy();
		NTL_ASSERTFAIL(szFileName << " Movie Create Fail" );
		return FALSE;
	}

	m_pMovie->SetUserData((void*)nID);

	m_pRenderConfig = *NTL_NEW GFxRenderConfig( GetFlasherManager()->GetFlashRenderer(), GFxRenderConfig::RF_EdgeAA );
	loader->SetRenderConfig( m_pRenderConfig );
	m_pMovie->SetBackgroundAlpha( 0.0f );

	// ResizeMode 설정
	SetResizeType( m_eScaleModeType );

	// Resolution Size저장.
	GRectF rect = m_pMovie->GetVisibleFrameRect();
	m_nResolutionWidth = (INT)rect.Width();
	m_nResolutionHeight = (INT)rect.Height();

	return TRUE;
}

VOID CNtlFlasher::Destroy(VOID)
{
	m_nScreenX = 0;
	m_nScreenY = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nResolutionWidth = 0;
	m_nResolutionHeight = 0;
	
	m_pRenderConfig = NULL;
	m_pMovieDef = NULL;
	m_pMovie = NULL;
}

BOOL CNtlFlasher::Update( FLOAT fElapsed )
{
	if( m_pMovie && GetFlasherManager()->IsEnableRender() )
	{
		if( fElapsed >= 1.0f )
			fElapsed = 1.0f;

		m_pMovie->Advance( fElapsed, 1 );

		if (HasFrameCallback())
		{
			UINT nCurrentFrame = m_pMovie->GetCurrentFrame();
			UINT nTotalFrame = m_pMovie->GetFrameCount();
			GFxMovie::PlayState ePlayState = m_pMovie->GetPlayState();

			if (nCurrentFrame + 1 >= nTotalFrame)
			{
				if (m_bAutoRewind)
				{
					RestartMovie();
				}
				else
				{
					Play(FALSE);

					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

VOID CNtlFlasher::Render(VOID)
{
	if( m_pMovie && GetFlasherManager()->IsEnableRender() && !m_bOutofScreen )
	{
		GetFlasherManager()->GetStateBlock()->Capture();

		LPDIRECT3DDEVICE9 pD3DDev = static_cast<LPDIRECT3DDEVICE9>( RwD3D9GetCurrentD3DDevice() );

		pD3DDev->EndScene();

		m_pMovie->Display();		
		
		pD3DDev->BeginScene();

		GetFlasherManager()->GetStateBlock()->Apply();
	}	
}

INT CNtlFlasher::GetFrameCount(VOID)
{
	if( m_pMovie )
		return m_pMovie->GetFrameCount();

	return 0;
}

FLOAT CNtlFlasher::GetFrameRate(VOID)
{
	if( m_pMovie )
		return m_pMovie->GetFrameRate();

	return 0.0f;
}

DWORD CNtlFlasher::GetCurrentFrame(VOID)
{
	if( m_pMovie )
		return m_pMovie->GetCurrentFrame();

	return 0;
}

VOID CNtlFlasher::GotoFrame( DWORD dwFrame )
{
	if( m_pMovie )
		return m_pMovie->GotoFrame( dwFrame );
}

VOID CNtlFlasher::SetPosition( INT nScreenX, INT nScreenY, INT nWidth, INT nHeight, INT nClippingX, INT nClippingY, INT nClippingWidth, INT nClippingHeight, BOOL bClipping /* = FALSE  */)
{
	m_nScreenX = nScreenX;
	m_nScreenY = nScreenY;
	m_nWidth = nWidth;	
	m_nHeight = nHeight;
	
	SetViewPortRect( m_nScreenX, m_nScreenY, m_nWidth, m_nHeight, nClippingX, nClippingY, nClippingWidth, nClippingHeight, bClipping );
}

VOID CNtlFlasher::SetRotate( INT nDegree )
{
	if( m_pMovie )
	{
		float fAngle = NTL_DEG2RAD( nDegree );
		float fCenterX = m_nWidth / 2.0f;
		float fCenterY = m_nHeight / 2.0f;
		float fX = ( cos( fAngle ) * fCenterX ) + ( ( -sin( fAngle ) * fCenterY ) );
		float fY = ( sin( fAngle ) * fCenterX ) + ( cos( fAngle ) * fCenterY );
		char buf[32];

		sprintf_s( buf, 32, "%d", nDegree );
		m_pMovie->SetVariable( "_root._rotation", buf );
		sprintf_s( buf, 32, "%d", (INT)( fCenterX - fX ) );
		m_pMovie->SetVariable( "_root._x", buf );
		sprintf_s( buf, 32, "%d", (INT)( fCenterY - fY ) );
		m_pMovie->SetVariable( "_root._y", buf );
	}
}

VOID CNtlFlasher::SetAlpha( CHAR ucAlpha )
{
	if( m_pMovie )
	{
		char buf[32];
		sprintf_s( buf, 32, "%u", ucAlpha );
		m_pMovie->SetVariable( "_root._alpha", buf );
	}
}

BOOL CNtlFlasher::IsPlay(VOID)
{
	if( m_pMovie )
	{
		if (m_pMovie->GetPlayState() == GFxMovie::Playing)
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

VOID CNtlFlasher::Play( BOOL bPlay )
{
	if( m_pMovie )
	{
		if( bPlay )
			m_pMovie->SetPlayState( GFxMovie::Playing );
		else
			m_pMovie->SetPlayState( GFxMovie::Stopped );
	}	
}

VOID CNtlFlasher::SetResizeType( GFxMovieView::ScaleModeType eResizeType )
{
	m_eScaleModeType = eResizeType;
	
	if( m_pMovie )
	{
		m_pMovie->SetViewScaleMode( eResizeType );		
	}
}

VOID CNtlFlasher::RestartMovie(VOID)
{
	if( m_pMovie )
	{
		Play( TRUE );
		m_pMovie->Restart();	
	}
}

VOID CNtlFlasher::OnMouseMove( INT nX, INT nY )
{
	if( m_pMovie )
	{
		GFxMouseEvent MouseEvent( GFxEvent::MouseMove, 0, (SInt)nX, (SInt)nY );
		m_pMovie->HandleEvent( MouseEvent );
	}
}

VOID CNtlFlasher::OnMouseButton( UINT button, BOOL bDown, INT nX, INT nY )
{
	if( m_pMovie )
	{
		GFxEvent::EventType eEventDown = ( bDown ) ? GFxEvent::MouseDown : GFxEvent::MouseUp ;
		GFxMouseEvent MouseEvent( eEventDown, button, (SInt)nX, (SInt)nY );
		m_pMovie->HandleEvent( MouseEvent );
	}
}

VOID CNtlFlasher::OnMouseWheel( INT nDelta, INT nX, INT nY )
{
	if( m_pMovie )
	{
		GFxMouseEvent MouseEvent( GFxEvent::MouseWheel, 0, (SInt)nX, (SInt)nY, (FLOAT)( nDelta / WHEEL_DELTA ) * 3 );
		m_pMovie->HandleEvent( MouseEvent );
	}
}

VOID CNtlFlasher::OnKeyEvent( UINT key, UINT info, BOOL bDown )
{
	GFxKey::Code FxKey( (GFxKey::Code)key );
	
	// 키들이 매치되지 않는 경우가 많으므로 다시 한번 맞춰본다.
	if( key >= 'A' && key <= 'Z')
	{
		FxKey = (GFxKey::Code)( ( key - 'A' ) + GFxKey::A );
	}
	else if( key >= VK_F1 && key <= VK_F15 )
	{
		FxKey = (GFxKey::Code)( ( key - VK_F1 ) + GFxKey::F1 );
	}
	else if( key >= VK_NUMPAD0 && key <= VK_NUMPAD9 )
	{
		FxKey = (GFxKey::Code)( ( key - VK_NUMPAD0 ) + GFxKey::KP_0 );
	}
	else
	{
		static struct 
		{
			INT	         vk;
			GFxKey::Code FxKey;
		} table[] =
		{
			{ VK_RETURN,    GFxKey::Return },
			{ VK_ESCAPE,    GFxKey::Escape },
			{ VK_LEFT,      GFxKey::Left },
			{ VK_UP,        GFxKey::Up },
			{ VK_RIGHT,     GFxKey::Right },
			{ VK_DOWN,      GFxKey::Down },

			// 안맞는 키는 위로 계속 채워 넣자.
			{ 0, GFxKey::VoidSymbol }
		};

		for ( INT i = 0 ; table[i].vk != 0 ; ++i )
		{
			if ( key == (UInt)table[i].vk )
			{
				FxKey = table[i].FxKey;
				break;
			}
		}
	}

	if ( FxKey != GFxKey::VoidSymbol )
	{
		if ( m_pMovie )
		{
			// ASCII 코드를 얻는다.
			UByte asciiCode = 0;
			UINT uScanCode = (info >> 16) & 0xFF; // fetch the scancode
			BYTE ks[256];
			WORD charCode;

			// 현재 키보드 상태
			::GetKeyboardState(ks);

			if ( ::ToAscii( key, uScanCode, ks, &charCode, 0 ) > 0 ) 
			{
				asciiCode = LOBYTE (charCode);
			}

			GFxKeyEvent KeyEvent( ( bDown ) ? GFxEvent::KeyDown : GFxKeyEvent::KeyUp, FxKey, asciiCode );			
			KeyEvent.SpecialKeysState.SetShiftPressed( ( ::GetKeyState( VK_SHIFT ) & 0x8000 ) ? 1 : 0 );
			KeyEvent.SpecialKeysState.SetCtrlPressed( ( ::GetKeyState( VK_CONTROL ) & 0x8000 ) ? 1 : 0 );
			KeyEvent.SpecialKeysState.SetAltPressed( ( ::GetKeyState( VK_MENU ) & 0x8000 ) ? 1 : 0 );
			KeyEvent.SpecialKeysState.SetNumToggled( ( ::GetKeyState( VK_NUMLOCK ) & 0x1 ) ? 1 : 0 );
			KeyEvent.SpecialKeysState.SetCapsToggled( ( ::GetKeyState( VK_CAPITAL ) & 0x1 ) ? 1 : 0 );
			KeyEvent.SpecialKeysState.SetScrollToggled( ( ::GetKeyState( VK_SCROLL ) & 0x1 ) ? 1 : 0 );
			m_pMovie->HandleEvent( KeyEvent );
		}
	}
}

VOID CNtlFlasher::SetViewPortRect( INT nX, INT nY, INT nWidth, INT nHeight, INT nClippingX, INT nClippingY, INT nClippingWidth, INT nClippingHeight, BOOL bClipping )
{
	RwInt32 nManagerWidth = GetFlasherManager()->GetScreenWidth();
	RwInt32 nManagerHeight = GetFlasherManager()->GetScreenHeight();

	if( nX >= nManagerWidth || nY >= nManagerHeight || nX + nWidth < 0 || nY + nHeight < 0 || nWidth <= 0 || nHeight <= 0 )
	{
		m_bOutofScreen = TRUE;
		return; 
	}
	else
	{
		m_bOutofScreen = FALSE;
	}

	if( m_pMovie )
	{
		if( bClipping )
		{
			GViewport viewport( nManagerWidth, nManagerHeight, nX, nY, nWidth, nHeight, nClippingX, nClippingY, nClippingWidth, nClippingHeight );
			m_pMovie->SetViewport( viewport );
		}
		else
		{
			m_pMovie->SetViewport( nManagerWidth, nManagerHeight, nX, nY, nWidth, nHeight );
		}		
	}	
}
