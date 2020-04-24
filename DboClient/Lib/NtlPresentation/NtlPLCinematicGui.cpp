#include "precomp_ntlpresentation.h"
#include "NtlPLCinematicGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"
#include "NtlPLGuiManager.h"


CNtlPLLuaLinkedFlash::CNtlPLLuaLinkedFlash( CRectangle& rtRect, RwChar* pFilename, gui::CComponent* pParent, VOID* pEchoData, RwBool bUseTime, RwReal fLifeTime, RwReal fBeginAlphaTime /* = 0.0f */, RwReal fEndAlphaTime /* = 0.0f */, RwUInt32 flagPlayTimeType /* = 0  */)
: CNtlPLCinematicFlash( rtRect, pParent, pFilename )
{
	m_bUseTime = bUseTime;
	m_fLifeTime = fLifeTime;
	m_fFlashMovieTime = 0.0f;
	m_fBeginAlphaTime = fBeginAlphaTime;
	m_fEndAlphaTime = fEndAlphaTime;
	m_flagPlayTimeType = flagPlayTimeType;
	m_pEchoData = pEchoData;

	if( !m_bUseTime )
	{
		//m_slotMovieEnd = m_pFlash->SigMovieEnd().Connect( this, &CNtlPLLuaLinkedFlash::OnEndMovie );
		m_fFlashMovieTime = 1.0f / m_pFlash->GetFrameRate() * m_pFlash->GetFrameCount();
		m_fPlayTime = m_fFlashMovieTime;
	}	
	else
		m_fPlayTime = m_fLifeTime;
	
	if( m_flagPlayTimeType & SNtlEventCinematicFlash::FLAG_BEGIN )
		m_fPlayTime -= m_fBeginAlphaTime;	
	
	if( m_flagPlayTimeType & SNtlEventCinematicFlash::FLAG_END )
		m_fPlayTime -= m_fEndAlphaTime;
	
	m_eState = BEGIN;
	m_fCurrentTime = 0.0f;
}

CNtlPLLuaLinkedFlash::~CNtlPLLuaLinkedFlash(VOID)
{

}

RwBool CNtlPLLuaLinkedFlash::Update( RwReal fElapsedTime )
{
	m_fCurrentTime += fElapsedTime;

	switch( m_eState )
	{
		case BEGIN: BeginStateUpdate( fElapsedTime ); break;
		case PLAY: PlayStateUpdate( fElapsedTime ); break;
		case END: EndStateUpdate( fElapsedTime ); break;
		case NONE: return FALSE;
	}

	return TRUE;
}

VOID CNtlPLLuaLinkedFlash::BeginStateUpdate( RwReal fElapsedTime )
{
	if( m_fBeginAlphaTime >= m_fCurrentTime )
	{
		m_pFlash->SetAlpha( (RwUInt8)gui::GetResultLineInterpolation( m_fCurrentTime, m_fBeginAlphaTime, 0.0f, 255.0f ) );
	}
	else
	{
		m_pFlash->SetAlpha( 255 );

		m_eState = PLAY;
		m_fCurrentTime = 0.0f;
	}

	if( m_flagPlayTimeType & SNtlEventCinematicFlash::FLAG_BEGIN )		
		CNtlPLCinematicFlash::Update( fElapsedTime );	
}

VOID CNtlPLLuaLinkedFlash::PlayStateUpdate( RwReal fElapsedTime )
{
	//if( m_bUseTime )
	{
		if( m_fPlayTime < m_fCurrentTime )
		{
			OnEndMovie( NULL );
		}
	}
	
	CNtlPLCinematicFlash::Update( fElapsedTime );	
}

VOID CNtlPLLuaLinkedFlash::EndStateUpdate( RwReal fElapsedTime )
{
	if( m_fEndAlphaTime >= m_fCurrentTime )
	{
		m_pFlash->SetAlpha( (RwUInt8)gui::GetResultLineInterpolation( m_fCurrentTime, m_fEndAlphaTime, 255.0f, 0.0f ) );
	}
	else
	{
		m_pFlash->SetAlpha( 0 );

		m_eState = NONE;
		m_fCurrentTime = 0.0f;				
	}

	if( m_flagPlayTimeType & SNtlEventCinematicFlash::FLAG_END )
		CNtlPLCinematicFlash::Update( fElapsedTime );	
}

VOID CNtlPLLuaLinkedFlash::OnEndMovie( gui::CComponent* pComponent )
{
	m_eState = END;
	m_fCurrentTime = 0.0f;			
	
	CNtlPLEventGenerator::CinematicEcho( m_pEchoData );
}

CNtlPLCinematicGui::CNtlPLCinematicGui(VOID)
{
	m_eCinematicState = CINEMATIC_STATE_NONE;
	m_eBalloonState = BALLOON_STATE_NONE;

	m_fBalloonTime = 0.0f;
}

CNtlPLCinematicGui::~CNtlPLCinematicGui(VOID)
{

}

RwBool CNtlPLCinematicGui::Create(VOID)
{
	m_pCinematicObject = GetNtlPLCinematicObject();

	m_slotOnMouseDown = m_pCinematicObject->GetDialog()->SigMouseDown().Connect( this, &CNtlPLCinematicGui::OnMouseDown );		

	LinkMsg( g_EventCinematicViewShow );
	LinkMsg( g_EventCinematicBalloon );
	LinkMsg( g_EventCinematicFlash );
	LinkMsg( g_EventResetCinematic );
	LinkMsg( g_EventCinematicBreak );
	LinkMsg(g_EventDirectPlayCancel);

	NTL_RETURN( TRUE );
}

VOID CNtlPLCinematicGui::Destroy(VOID)
{
	UnLinkMsg( g_EventCinematicViewShow );
	UnLinkMsg( g_EventCinematicBalloon );
	UnLinkMsg( g_EventCinematicFlash );
	UnLinkMsg( g_EventResetCinematic );
	UnLinkMsg( g_EventCinematicBreak );
	UnLinkMsg(g_EventDirectPlayCancel);
}

INT CNtlPLCinematicGui::StateUpdate( RwUInt32 uiElapsedTime )
{
	RwReal fElapsedTime = uiElapsedTime * 0.001f;
	
	CinematicDirection();
	BalloonDirection( fElapsedTime );

	return 1;
}

VOID CNtlPLCinematicGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventCinematicViewShow )
	{
		SNtlEventCinematicViewShow* pData = reinterpret_cast<SNtlEventCinematicViewShow*>( msg.pData );
		CNtlPLEventGenerator::ResetCinematic( SNtlEventResetCinematic::CINEMATIC );
		SetCinematicViewShow( pData->bShow );
	}
	else if( msg.Id == g_EventCinematicBalloon )
	{
		SetCinematicBalloon( msg.pData );
	}
	else if( msg.Id == g_EventCinematicFlash )
	{
		SetCinematicFlash( msg.pData );
	}
	else if( msg.Id == g_EventResetCinematic )
	{
		SNtlEventResetCinematic* pData = reinterpret_cast<SNtlEventResetCinematic*>( msg.pData );

		if( pData->eDemandResetObject != SNtlEventResetCinematic::CINEMATIC )
		{
			CloseCinematic();
		}		
	}	
	else if( msg.Id == g_EventCinematicBreak )
	{
		CloseCinematic();
	}
	else if (msg.Id == g_EventDirectPlayCancel)
	{
		UINT32 hPc = reinterpret_cast<UINT32>(msg.pData);

		SkipCinematics(hPc);
	}
}

VOID CNtlPLCinematicGui::SetCinematicViewShow( RwBool bShow )
{
	if( bShow )
	{
		SetCinematicState( CINEMATIC_STATE_BEGIN );
	}
	else
	{
		if( m_eCinematicState != CINEMATIC_STATE_NONE )
			SetCinematicState( CINEMATIC_STATE_END );			
	}
}

VOID CNtlPLCinematicGui::SetCinematicBalloon( void* pData )
{
	SNtlEventCinematicBalloon* pBalloonData = reinterpret_cast<SNtlEventCinematicBalloon*>( pData );
	
	if( pBalloonData->bShow )
	{
		if( m_eBalloonState == BALLOON_STATE_TEXTAPPEAR || m_eBalloonState == BALLOON_STATE_IDLE )
		{
			SetBalloonState( BALLOON_STATE_TEXTAPPEAR );
		}
		else
		{
			SetBalloonState( BALLOON_STATE_BEGIN );			
		}

		m_pCinematicObject->ShowImage( TRUE );
		m_pCinematicObject->SetImageTexture( "3511101_1.png" );
		m_pCinematicObject->SetName( L"Dummy" );

		std::wstring wstrTextData = L"In order to display all information, you must inherit the class";
		m_pCinematicObject->GetHtmlText()->SetHtmlFromMemory( wstrTextData.c_str(), wstrTextData.size() );		
	}
	else
	{
		if( m_eBalloonState != BALLOON_STATE_NONE &&
			m_eBalloonState != BALLOON_STATE_END )			
			SetBalloonState( BALLOON_STATE_END );
	}
}

VOID CNtlPLCinematicGui::SetCinematicFlash( void* pData )
{
	SNtlEventCinematicFlash* pFlashData = reinterpret_cast<SNtlEventCinematicFlash*>( pData );

	if( pFlashData->bShow )
	{
		CRectangle rect( 0, 0, m_pCinematicObject->GetWidth(), m_pCinematicObject->GetHeight() );

		CNtlPLLuaLinkedFlash* pFlash = NTL_NEW CNtlPLLuaLinkedFlash( rect, pFlashData->pFilename, m_pCinematicObject->GetImageFrame(),
																	 pFlashData->pEchoData, pFlashData->bUseTime, pFlashData->fLifeTime,
																	 pFlashData->fBeginAlphaTime, pFlashData->fEndAlphaTime, pFlashData->flagPlayTimeType );

		if( IsStateNone( ALL ) )
		{
			m_pCinematicObject->CinematicFadeOut( TRUE, TRUE );
			m_pCinematicObject->InitShowSetting();
			m_pCinematicObject->GetImageFrame()->Show( false );
			m_pCinematicObject->GetText()->Show( false );
			m_pCinematicObject->GetHtmlText()->Show( false );
			m_pCinematicObject->ShowImage( FALSE );			

			m_pCinematicObject->SwitchDialog( true );	
			m_pCinematicObject->LinkUpdateCallBack( this, &CNtlPLCinematicGui::StateUpdate );
			GetNtlGuiManager()->AddUpdateFunc( m_pCinematicObject );
		}

		m_pCinematicObject->AddFlashObject( pFlash );		
	}		
	else 
	{
		// Flash는 다 지워진 후에 Flash 삭제가 날아온다. 
		if( IsStateNone( ALL ) )
		{
			m_pCinematicObject->SwitchDialog( false );
			m_pCinematicObject->UnLinkUpdateCallBack();
			GetNtlGuiManager()->RemoveUpdateFunc( m_pCinematicObject );
			//OutputDebugString( "\nFlash UnLinkUpdateCallBack()\n\n" );
		}		
	}
}

VOID CNtlPLCinematicGui::SetCinematicState( CINEMATICSTATE eState )
{
	switch( eState )
	{
	case CINEMATIC_STATE_NONE:
		if( IsStateNone( BALLOON | FLASH ) )
		{
			GetNtlPLCinematicObject()->SwitchDialog( false );
			m_pCinematicObject->UnLinkUpdateCallBack();	
			GetNtlGuiManager()->RemoveUpdateFunc( m_pCinematicObject );
			//OutputDebugString( "\nCinematic UnLinkUpdateCallBack()\n\n" );
		}	
		
		break;
	case CINEMATIC_STATE_BEGIN:
		if( IsStateNone( ALL ) )
		{
			m_pCinematicObject->SwitchDialog( true );
			m_pCinematicObject->InitShowSetting();
			m_pCinematicObject->GetImageFrame()->Show( false );
			m_pCinematicObject->GetText()->Show( false );
			m_pCinematicObject->GetHtmlText()->Show( false );
			m_pCinematicObject->ShowImage( FALSE );			
			m_pCinematicObject->CinematicBackFadeOut( TRUE );
			m_pCinematicObject->LinkUpdateCallBack( this, &CNtlPLCinematicGui::StateUpdate );
			GetNtlGuiManager()->AddUpdateFunc( m_pCinematicObject );
			//OutputDebugString( "\nCinematic LinkUpdateCallBack()\n\n" );
		}	
		
		m_pCinematicObject->CinematicFadeOut( TRUE, TRUE );
		m_pCinematicObject->ResetCinematicCurrentTime();
		break;
	case CINEMATIC_STATE_END:
		m_pCinematicObject->ResetCinematicCurrentTime();
		break;
	case CINEMATIC_STATE_IDLE:
		break;
	}	

	m_eCinematicState = eState;
}

VOID CNtlPLCinematicGui::SetBalloonState( BALLOONSTATE eState )
{
	switch( eState )
	{
	case BALLOON_STATE_NONE:
		if( IsStateNone( CINEMATIC | FLASH ) )
		{
			GetNtlPLCinematicObject()->SwitchDialog( false );
			m_pCinematicObject->UnLinkUpdateCallBack();			
			GetNtlGuiManager()->RemoveUpdateFunc( m_pCinematicObject );			
			//OutputDebugString( "\nBalloon UnLinkUpdateCallBack()\n\n" );
		}		
		break;
	case BALLOON_STATE_BEGIN:
		if( IsStateNone( ALL ) )
		{
			m_pCinematicObject->SwitchDialog( true );
			m_pCinematicObject->InitShowSetting();
			m_pCinematicObject->CinematicFadeOut( TRUE, TRUE );
			m_pCinematicObject->LinkUpdateCallBack( this, &CNtlPLCinematicGui::StateUpdate );			
			GetNtlGuiManager()->AddUpdateFunc( m_pCinematicObject );
			//OutputDebugString( "\nBalloon LinkUpdateCallBack()\n\n" );
		}	

		m_pCinematicObject->CinematicBackFadeOut( TRUE );
		m_pCinematicObject->GetImageFrame()->Show( true );
		m_pCinematicObject->GetImageFrame()->SetAlpha( 0 );
		m_pCinematicObject->SetImageAlpha( 0 );
		m_pCinematicObject->ShowImage( FALSE );
		m_pCinematicObject->GetText()->Show( false );
		m_pCinematicObject->GetHtmlText()->Show( false );		
		m_pCinematicObject->SetFocusOnNPCBar( TRUE );

		m_fBalloonTime = 0.0f;
		break;
	case BALLOON_STATE_TEXTAPPEAR:
		m_pCinematicObject->GetHtmlText()->Show( true );
		m_pCinematicObject->GetHtmlText()->SetAlpha( 0 );
		m_fBalloonTime = 0.0f;
		break;
	case BALLOON_STATE_END:
		m_fBalloonTime = 0.0f;
		break;
	case BALLOON_STATE_IDLE:
		break;
	}	

	m_eBalloonState = eState;
}

VOID CNtlPLCinematicGui::CinematicDirection(VOID)
{
	RwReal fCurrentTime = m_pCinematicObject->GetCinematicCurrentTime();

	if( m_eCinematicState == CINEMATIC_STATE_BEGIN )
	{
		if( fCurrentTime < CINEMATIC_MOVETIME )
		{
			m_pCinematicObject->CinematicFadeIn( TRUE, FALSE );
		}
		else
		{
			m_pCinematicObject->CinematicFadeIn( TRUE, TRUE );
			SetCinematicState( CINEMATIC_STATE_IDLE );
		}
	}		
	else if( m_eCinematicState == CINEMATIC_STATE_END )
	{
		if( fCurrentTime < CINEMATIC_MOVETIME )
		{
			m_pCinematicObject->CinematicFadeOut( TRUE, FALSE );
		}
		else
		{
			m_pCinematicObject->CinematicFadeOut( TRUE, TRUE );
			SetCinematicState( CINEMATIC_STATE_NONE );
		}
	}
}

VOID CNtlPLCinematicGui::BalloonDirection( RwReal fElapsedTime )
{
	m_fBalloonTime += fElapsedTime;

	if( m_eBalloonState == BALLOON_STATE_BEGIN )
	{
		if( m_fBalloonTime < CINEMATIC_MOVETIME )
		{
			RwUInt8 byAlpha = (RwUInt8)gui::GetResultLineInterpolation( m_fBalloonTime, CINEMATIC_MOVETIME, 0.0f, 255.0f );
			m_pCinematicObject->GetImageFrame()->SetAlpha( byAlpha );
			m_pCinematicObject->SetImageAlpha( byAlpha );
			m_pCinematicObject->CinematicBackFadeIn( FALSE, m_fBalloonTime );
		}
		else
		{	
			m_pCinematicObject->GetImageFrame()->SetAlpha( 255 );
			m_pCinematicObject->SetImageAlpha( 255 );
			m_pCinematicObject->CinematicBackFadeIn( TRUE );
			SetBalloonState( BALLOON_STATE_TEXTAPPEAR );
		}
	}
	else if( m_eBalloonState == BALLOON_STATE_END )
	{
		if( m_fBalloonTime < CINEMATIC_MOVETIME )
		{
			RwUInt8 byAlpha = (RwUInt8)gui::GetResultLineInterpolation( m_fBalloonTime, CINEMATIC_MOVETIME, 255.0f, 0.0f );
			m_pCinematicObject->GetImageFrame()->SetAlpha( byAlpha );
			m_pCinematicObject->SetImageAlpha( byAlpha );
			m_pCinematicObject->CinematicBackFadeOut( FALSE, m_fBalloonTime );
		}
		else
		{	
			m_pCinematicObject->GetImageFrame()->SetAlpha( 0 );
			m_pCinematicObject->SetImageAlpha( 0 );
			m_pCinematicObject->CinematicBackFadeOut( TRUE );
			SetBalloonState( BALLOON_STATE_NONE );
		}
	}
	else if( m_eBalloonState == BALLOON_STATE_TEXTAPPEAR )
	{
		if( m_fBalloonTime < CINEMATIC_TEXT_APPEARTIME )
		{
			m_pCinematicObject->GetHtmlText()->SetAlpha( (RwUInt8)gui::GetResultLineInterpolation( m_fBalloonTime, CINEMATIC_TEXT_APPEARTIME, 0.0f, 255.0f ) );
		}
		else
		{
			m_pCinematicObject->GetHtmlText()->SetAlpha( 255 );	
			SetBalloonState( BALLOON_STATE_IDLE );			
		}
	}
}

RwBool CNtlPLCinematicGui::IsStateNone( RwUInt32 flagObject )
{
	RwBool bAllStateNone = TRUE;
	
	if( flagObject & CINEMATIC )
	{
		if( m_eCinematicState != CINEMATIC_STATE_NONE )
			bAllStateNone = FALSE;
	}

	if( flagObject & BALLOON )
	{
		if( m_eBalloonState != BALLOON_STATE_NONE )
			bAllStateNone = FALSE;
	}

	if( flagObject & FLASH )
	{
		if( m_pCinematicObject->IsFlashObjectUpdated() )
			bAllStateNone = FALSE;
	}

	return bAllStateNone;
}

VOID CNtlPLCinematicGui::CloseCinematic(VOID)
{
	if( m_pCinematicObject->IsFlashObjectUpdated() )
	{
		m_pCinematicObject->DeleteAllFlashObject();
	}
	if( m_eCinematicState != CINEMATIC_STATE_NONE )
	{
		SetCinematicState( CINEMATIC_STATE_NONE );			
	}
	if( m_eBalloonState != BALLOON_STATE_NONE )
	{
		SetBalloonState( BALLOON_STATE_NONE );	
	}		
}

VOID CNtlPLCinematicGui::OnMouseDown( const CKey& key )
{
	if( m_eBalloonState == BALLOON_STATE_TEXTAPPEAR )
	{
		if( m_fBalloonTime < CINEMATIC_TEXT_APPEARTIME )
		{
			m_fBalloonTime = CINEMATIC_TEXT_APPEARTIME;			
		}				
	}
}









