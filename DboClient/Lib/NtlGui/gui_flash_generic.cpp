#include "gui_precomp.h"
#include "gui_flash_generic.h"
#include "gui_componentoption.h"
#include "gui_componenttokenizer.h"

#include "NtlFlasherManager.h"

#include "rprandom.h"

START_GUI

CFlash_Generic::CFlash_Generic( CFlash* pSelf, CONST CHAR* pFileName, BOOL bUseFont, BOOL bOriginalSize, BOOL bScissor )
: m_pFlash( pSelf ), m_pFlashObject( NULL )
{
	m_stMovieEndData.Clear();
	m_vecFSCallBackData.clear();
	
	m_SlotMove = m_pFlash->SigMove().Connect(this, &CFlash_Generic::OnMove );
	m_slotResize = m_pFlash->SigResize().Connect( this, &CFlash_Generic::OnResize );
	m_SlotSetOption = m_pFlash->SigSetOptions().Connect( this, &CFlash_Generic::OnSetOptions );
	m_SlotPaint = m_pFlash->SigPaint().Connect( this, &CFlash_Generic::OnPaint );
	
	m_SlotMouseDown = m_pFlash->SigMouseDown().Connect( this, &CFlash_Generic::OnMouseDown );
	m_SlotMouseUp = m_pFlash->SigMouseUp().Connect( this, &CFlash_Generic::OnMouseUp );
	m_SlotMouseMove = m_pFlash->SigMouseMove().Connect( this, &CFlash_Generic::OnMouseMove );
	m_SlotMouseWheel = m_pFlash->SigMouseWheel().Connect( this, &CFlash_Generic::OnMouseWheel );
	m_SlotKeyDown = m_pFlash->SigKeyDown().Connect( this, &CFlash_Generic::OnKeyDown );
	m_SlotKeyUp = m_pFlash->SigKeyUp().Connect( this, &CFlash_Generic::OnKeyUp );
	m_SlotMouseLeave = m_pFlash->SigMouseLeave().Connect( this, &CFlash_Generic::OnMouseLeave );

	m_SlotSetAlpha = m_pFlash->SigSetAlpha().Connect( this, &CFlash_Generic::OnSetAlpha );
	m_SlotSetColor = m_pFlash->SigSetColor().Connect( this, &CFlash_Generic::OnSetColor );

#ifdef SCALEFORM_GFX
	m_pFlashObject = NTL_NEW CNtlFlasher;
	if( pFileName )
	{
		m_pFlashObject->SetUseFont( bUseFont );
		Load( pFileName, bOriginalSize );	
		
	}
#endif	

#ifdef FMOD_SOUND_ATTACHED_TO_GUI
	m_hSound = INVALID_SOUND_HANDLE;
	m_hBGM = INVALID_SOUND_HANDLE;
#endif
}

CFlash_Generic::~CFlash_Generic(VOID)
{
#ifdef SCALEFORM_GFX
	m_vecFSCallBackData.clear();

	if( m_pFlashObject )
	{
		m_pFlashObject->Destroy();

		NTL_DELETE( m_pFlashObject );
	}	
#endif
}

BOOL CFlash_Generic::Load( CONST CHAR* pFileName, BOOL bOriginalSize )
{
#ifdef SCALEFORM_GFX
	if( m_pFlashObject->GetMovie() || m_pFlashObject->GetMovieDef() )
		Unload();

	std::string strFilename = "flash\\";
	strFilename += pFileName;

	int nID = GetFlasherManager()->GenerateUniqueID();

	GetFlasherManager()->GetCommandHandler()->LinkFSCallBack(nID, this, &CFlash_Generic::OnFSCallBack); // link callback
	
	BOOL bRes =	m_pFlashObject->Create( strFilename.c_str(), nID);

	if( !bRes )
	{
		GetFlasherManager()->GetCommandHandler()->UnLinkFSCallBack(nID); // unlink callback
		return FALSE;
	}

	if( bOriginalSize )
		m_pFlash->SetSize( m_pFlashObject->GetResolutionWidth(), m_pFlashObject->GetResolutionHeight() );		

	SetFlashPosition();	

	return bRes;
#endif
	return TRUE;
}

VOID CFlash_Generic::Unload(VOID)
{
#ifdef  SCALEFORM_GFX
	m_pFlashObject->Destroy();
	m_hSound = INVALID_SOUND_HANDLE;

	if( INVALID_SOUND_HANDLE != m_hBGM )
	{
		GetSoundManager()->Stop(m_hBGM);
		m_hBGM = INVALID_SOUND_HANDLE;
	}
#endif	
}

VOID CFlash_Generic::Update( FLOAT fElapsedTime )
{
#ifdef SCALEFORM_GFX	
	if (!m_pFlashObject->Update(fElapsedTime))
	{
		m_stMovieEndData.SendMovieEndCallBack();
	}

	if( m_stMovieEndData.DoSendMovieEndCallBack() )
	{
		m_sigMovieEnd( m_pFlash );	
		m_stMovieEndData.Clear();
	}

	if( !m_vecFSCallBackData.empty() )
	{
		std::vector<stFSCallBackData>::iterator it;
		for( it = m_vecFSCallBackData.begin() ; it != m_vecFSCallBackData.end() ; ++it )
		{
			stFSCallBackData* pData = &(*it);
			m_sigFSCallBack( pData->m_strCommand.c_str(), pData->m_strArgs.c_str() );		
		}

		m_vecFSCallBackData.clear();
	}
#endif
}

BOOL CFlash_Generic::SetVariable( const CHAR* path, const CHAR* pValue )
{
#ifdef SCALEFORM_GFX
	GFxMovieView* pMovie = m_pFlashObject->GetMovie();
	if( pMovie )
		return pMovie->SetVariable( path, pValue );
#endif
	return FALSE;
}

BOOL CFlash_Generic::SetVariable( const CHAR* path, const WCHAR* pValue )
{
#ifdef SCALEFORM_GFX
	GFxMovieView* pMovie = m_pFlashObject->GetMovie();
	if( pMovie )
		return pMovie->SetVariable( path, pValue );
#endif 
	return FALSE;
}

BOOL CFlash_Generic::SetVariableFloat( const CHAR* path, FLOAT fValue )
{ 
#ifdef SCALEFORM_GFX
	GFxMovieView* pMovie = m_pFlashObject->GetMovie();
	if( pMovie )
		return pMovie->SetVariableDouble( path, fValue );	
#endif
	return FALSE;
}

BOOL CFlash_Generic::SetVariableArray( INT type, const CHAR* path, DWORD index, const VOID* pData, DWORD count )
{
#ifdef SCALEFORM_GFX 
	

	GFxMovieView* pMovie = m_pFlashObject->GetMovie();
	if( pMovie )
		return pMovie->SetVariableArray( (GFxMovie::SetArrayType)type, path, index, pData, count );
#endif
	return FALSE;
}

const CHAR*	CFlash_Generic::GetVariable( const CHAR* path )
{
#ifdef SCALEFORM_GFX 
	GFxMovieView* pMovie = m_pFlashObject->GetMovie();
	if( pMovie )
		return pMovie->GetVariable( path );
#endif
	return NULL;
}

const WCHAR* CFlash_Generic::GetVariableWideStr( const CHAR* path )
{
#ifdef SCALEFORM_GFX 
	GFxMovieView* pMovie = m_pFlashObject->GetMovie();
	if( pMovie )
		return pMovie->GetVariableStringW( path );	
#endif
	return NULL;
}

FLOAT CFlash_Generic::GetVariableFloat( const CHAR* path )
{
#ifdef SCALEFORM_GFX
	GFxMovieView* pMovie = m_pFlashObject->GetMovie();
	if( pMovie )
		return (FLOAT)pMovie->GetVariableDouble( path );	
#endif 
	return 0.0f;
}

BOOL CFlash_Generic::GetVariableArray( INT type, const CHAR* path, DWORD index, VOID* pData, DWORD count )
{
#ifdef SCALEFORM_GFX
	GFxMovieView* pMovie = m_pFlashObject->GetMovie();
	if( pMovie )
		return pMovie->GetVariableArray( (GFxMovie::SetArrayType)type, path, index, pData, count );
#endif 
	return FALSE;
}

const CHAR* CFlash_Generic::Invoke( const CHAR* path, const CHAR* parameter, va_list args )
{
#ifdef SCALEFORM_GFX
	GFxMovieView* pMovie = m_pFlashObject->GetMovie();
	if( pMovie )
	{
		const CHAR* pRes = pMovie->InvokeArgs( path, parameter, args );
		return pRes;
	}	
#endif
	return NULL;
}

CRectangle CFlash_Generic::GetFrameResolution(VOID)
{
	CRectangle rect;
#ifdef SCALEFORM_GFX
	rect.right = m_pFlashObject->GetResolutionWidth();
	rect.bottom = m_pFlashObject->GetResolutionHeight();
#endif
	return rect;		
}

INT CFlash_Generic::GetFrameCount(VOID)
{
	INT nFrameCount = 0;
#ifdef SCALEFORM_GFX
	nFrameCount = m_pFlashObject->GetFrameCount();
#endif
	return nFrameCount;
}

FLOAT CFlash_Generic::GetFrameRate(VOID)
{
	FLOAT fFrameRate = 0.0f;
#ifdef SCALEFORM_GFX
	fFrameRate = m_pFlashObject->GetFrameRate();
#endif
	return fFrameRate;
}

BOOL CFlash_Generic::IsPlayMovie(VOID)
{
#ifdef SCALEFORM_GFX
	return m_pFlashObject->IsPlay();
#else
	return FALSE;
#endif
}

VOID CFlash_Generic::SetFrameCallback( BOOL bSet )
{
#ifdef SCALEFORM_GFX
	m_pFlashObject->SetFrameCallback( bSet );
#endif
}

BOOL CFlash_Generic::HasFrameCallback(VOID)
{
#ifdef SCALEFORM_GFX
	return m_pFlashObject->HasFrameCallback();
#else
	return FALSE;
#endif 
}

DWORD CFlash_Generic::GetCurrentFrame(VOID)
{
#ifdef SCALEFORM_GFX
	return m_pFlashObject->GetCurrentFrame();
#else
	return 0;
#endif
}

VOID CFlash_Generic::GotoFrame( DWORD dwFrame )
{
#ifdef SCALEFORM_GFX
	return m_pFlashObject->GotoFrame( dwFrame );
#endif 
}

VOID CFlash_Generic::SetResizeType( INT eResizeType )
{
#ifdef SCALEFORM_GFX
	m_pFlashObject->SetResizeType( (GFxMovieView::ScaleModeType)eResizeType );	
#endif
}

VOID CFlash_Generic::SetFlashPosition(VOID)
{
#ifdef SCALEFORM_GFX
	CRectangle rtScreen = m_pFlash->GetScreenRect();	
	CRectangle rtClipping;
	
	if( m_pFlash->IsClipped() )
		rtClipping = m_pFlash->GetClippingRect();

	m_pFlashObject->SetPosition( rtScreen.left, rtScreen.top, rtScreen.GetWidth(), rtScreen.GetHeight(),
								 rtClipping.left, rtClipping.top, rtClipping.GetWidth(), rtClipping.GetHeight(), m_pFlash->IsClipped() );		
#endif
}

VOID CFlash_Generic::OnSetOptions( const CComponentOptions& options )
{
#ifdef SCALEFORM_GFX
	if( options.Exists( "usefont" ) )
	{
		m_pFlashObject->SetUseFont( options.GetValueAsBool( "usefont" ) );
	}

	BOOL bOriginalSize = FALSE;
	if( options.Exists( "originalsize" ) )
	{
		bOriginalSize = options.GetValueAsBool( "originalsize" );
	}

	if( options.Exists( "filename" ) )
	{
		Load( options.GetValue( "filename" ).c_str(), bOriginalSize );
	}
	
	if( options.Exists( "initplay" ) )
	{
		m_pFlashObject->Play( options.GetValueAsBool( "initplay" ) );
	}

	if( options.Exists( "autorewind" ) )
	{
		m_pFlashObject->AutoRewind( options.GetValueAsBool( "autorewind" ) );	
	}

	if( options.Exists( "framecallback" ) )
	{
		m_pFlashObject->SetFrameCallback( options.GetValueAsBool( "framecallback" ) );
	}

	if( options.Exists( "resizetype" ) )
	{
		std::string strType = options.GetValue( "resizetype" );
		if( strType == "noscale" )
			m_pFlash->SetResizeType( CFlash::NOSCALE );
		else if( strType == "showall" )
			m_pFlash->SetResizeType( CFlash::SHOWALL );
		else if( strType == "exactfit" )
			m_pFlash->SetResizeType( CFlash::EXACTFIT );
	}	
#endif
}

VOID CFlash_Generic::OnPaint(VOID)
{
#ifdef SCALEFORM_GFX
	m_pFlashObject->Render();		
#endif	
}

VOID CFlash_Generic::OnMove( INT nX, INT nY )
{	
#ifdef  SCALEFORM_GFX
	SetFlashPosition();
#endif	
}

VOID CFlash_Generic::OnResize( INT nOldCX, INT nOldCY )
{
#ifdef  SCALEFORM_GFX
	SetFlashPosition();
#endif
}

VOID CFlash_Generic::OnMouseDown( const CKey& key )
{
#ifdef SCALEFORM_GFX
	// key ID 0 : Leftbutton, 1: Rightbutton
	m_pFlashObject->OnMouseButton( key.m_nID, TRUE, (INT)key.m_fX, (INT)key.m_fY );	
	m_pFlash->CaptureMouse();
#endif
}

VOID CFlash_Generic::OnMouseUp( const CKey& key )
{
#ifdef SCALEFORM_GFX
	// key ID 0 : Leftbutton, 1: Rightbutton
	m_pFlashObject->OnMouseButton( key.m_nID, FALSE, (INT)key.m_fX, (INT)key.m_fY );			
	m_pFlash->ReleaseMouse();
#endif
}

VOID CFlash_Generic::OnMouseMove( INT nFlags, INT nX, INT nY )
{
#ifdef SCALEFORM_GFX
	m_pFlashObject->OnMouseMove( nX, nY );
#endif	
}

VOID CFlash_Generic::OnKeyDown( gui::CComponent *pComponent, CInputDevice *pInputDevice, const CKey &key )
{
#ifdef SCALEFORM_GFX
	m_pFlashObject->OnKeyEvent( key.m_dwVKey, key.m_nID, TRUE );
#endif
}

VOID CFlash_Generic::OnKeyUp( CComponent* pComponent, CInputDevice* pInputDevice, const CKey& key )
{
#ifdef SCALEFORM_GFX
	m_pFlashObject->OnKeyEvent( key.m_dwVKey, key.m_nID, FALSE );
#endif
}

VOID CFlash_Generic::OnMouseLeave( gui::CComponent* pComponent )
{
#ifdef SCALEFORM_GFX
	m_pFlashObject->OnMouseMove( -1, -1 );
#endif
}

VOID CFlash_Generic::OnMouseWheel( INT nFalgs, short shDelta, CPos& ptPos )
{
#ifdef SCALEFORM_GFX
	m_pFlashObject->OnMouseWheel( shDelta, ptPos.x, ptPos.y );
#endif
}

VOID CFlash_Generic::OnSetAlpha( BYTE ucAlpha )
{
#ifdef SCALEFORM_GFX
	m_pFlashObject->SetAlpha( (RwUInt8)( ucAlpha / 255.0f * 100.0f ) );
#endif
}

VOID CFlash_Generic::OnSetColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
	
}

INT CFlash_Generic::OnFSCallBack( VOID* szCommand, VOID* szArgs)
{
	const CHAR* pCommand = reinterpret_cast<const CHAR*>( szCommand );
	const CHAR* pArgs = reinterpret_cast<const CHAR*>( szArgs );

	//DBO_WARNING_MESSAGE("OnFSCallBack name match nID " << nID << " cur ID: " << m_pFlashObject->GetUniqueID() << " cur file: " << m_pFlashObject->GetFileName().c_str());

	if (!strcmp(pCommand, "MovieEnd"))
	{
		m_stMovieEndData.SendMovieEndCallBack();
	}
#ifdef FMOD_SOUND_ATTACHED_TO_GUI
	else if( !strcmp( pCommand, "Sound" ) )
	{
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_UI_SOUND;
		tSoundParam.pcFileName		= reinterpret_cast<char*>( szArgs );

		GetSoundManager()->Play( &tSoundParam );
		m_hSound = tSoundParam.hHandle;
	}
	else if( !strcmp( pCommand, "Sound_Loop" ) )
	{
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_UI_SOUND;
		tSoundParam.pcFileName		= reinterpret_cast<char*>( szArgs );
		tSoundParam.bLoop			= true;

		GetSoundManager()->Play( &tSoundParam );
		m_hSound = tSoundParam.hHandle;
	}
	else if( !strcmp( pCommand, "Sound_Stop" ) )
	{
		GetSoundManager()->Stop( m_hSound );
	}
	else if( strcmp("TMQ_PROLOG_BGM_START", pCommand) == 0 )
	{
		if( INVALID_SOUND_HANDLE == m_hBGM )
		{
			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
			tSoundParam.pcFileName		= (char*)pArgs;
			tSoundParam.eBGM_Type		= BGM_TYPE_CINEMATIC;
			tSoundParam.bLoop			= true;
			GetSoundManager()->Play(&tSoundParam);

			m_hBGM = tSoundParam.hHandle;
		}
	}
	else if( strcmp("BGM_STOP", pCommand) == 0 )
	{
		if( INVALID_SOUND_HANDLE != m_hBGM )
			GetSoundManager()->Stop(m_hBGM);
	}
#endif
	else
	{
		m_vecFSCallBackData.push_back(stFSCallBackData(pCommand, pArgs));
	}
	
	return 1;
}

BOOL CFlash_Generic::CreateFlashManager( HWND hWnd )
{
#ifdef SCALEFORM_GFX
	if( GetFlasherManager()->CreateInstance( hWnd ) )
		return TRUE;
	
	return FALSE;
#endif
	return TRUE;
}

VOID CFlash_Generic::DeleteFlashManager(VOID)
{
#ifdef  SCALEFORM_GFX
	GetFlasherManager()->DeleteInstance();
#endif
}

VOID CFlash_Generic::ResetFlashRenderer(VOID)
{
#ifdef SCALEFORM_GFX
	GetFlasherManager()->ResetRenderer();
#endif
}

BOOL CFlash_Generic::ParseFlashFont( const CHAR* pFilename )
{
#ifdef SCALEFORM_GFX

	std::string strFIleName = pFilename;
	CComponentTokenizer lexer( strFIleName );

	if( !lexer.IsSuccess() )
		return FALSE;

	enum EParseState
	{
		PARSE_STATE_NONE,
		PARSE_STATE_FONTLIB,
		PARSE_STATE_FONTMAPNAME,
		PARSE_STATE_FONTNAME,
		PARSE_STATE_FONTSTYLE,
	};

	EParseState eState = PARSE_STATE_NONE;
	std::string strLib, strMapName, strFontName, strFontStyle;

	while( TRUE )
	{
		const std::string& token = lexer.PeekNextToken();

		if( token.empty() )
			break;

		switch( eState )
		{
		case PARSE_STATE_NONE:
			{
				if( token == "fontlib" )
				{
					eState = PARSE_STATE_FONTLIB;
				}
				else if( token == "map" )
				{
					eState = PARSE_STATE_FONTMAPNAME;
				}
			}
			break;	
		case PARSE_STATE_FONTLIB:
			{
				if( token == ";" )
				{
					std::string strFilename = "flash\\";
					strFilename += strLib;

					GetFlasherManager()->SetFontLibData( strFilename );
					eState = PARSE_STATE_NONE;
				}				
				else
				{
					strLib = token;			
				}
			}
			break;
		case PARSE_STATE_FONTMAPNAME:
			{
				if( token == "=" )
				{
					eState = PARSE_STATE_FONTNAME;
				}
				else
				{
					strMapName = token;
				}
			}
			break;
		case PARSE_STATE_FONTNAME:
			{
				if( token == "," )
				{
					eState = PARSE_STATE_FONTSTYLE;
				}
				else if( token == ";" )
				{
					GetFlasherManager()->SetFontMapData( strMapName, strFontName, std::string( "normal" ) );
					eState = PARSE_STATE_NONE;
				}
				else
				{
					strFontName = token;
				}				
			}
			break;
		case PARSE_STATE_FONTSTYLE:
			{
				if( token == ";" )
				{
					GetFlasherManager()->SetFontMapData( strMapName, strFontName, strFontStyle );
					eState = PARSE_STATE_NONE;
				}
				else
				{
					strFontStyle = token;
				}
			}
			break;
		}

		lexer.PopToPeek();
	}	
#endif

	return TRUE;
}

VOID CFlash_Generic::SetScreenSize( INT nWidth, INT nHeight )
{
#ifdef SCALEFORM_GFX
	GetFlasherManager()->SetScreenSize( nWidth, nHeight );
#endif
}

END_GUI