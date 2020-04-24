#include "precomp_dboclient.h"
#include "DBCNarrationGui.h"

// core
#include "NtlDebug.h"

// shared

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"
#include "NtlPLCinematicObject.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "DisplayStringManager.h"

#include "GUISoundDefine.h"

CDBCNarrationGui* CDBCNarrationGui::m_pInstance = NULL;

CDBCNarrationGui::CDBCNarrationGui(VOID)
{
	m_eUIState = STATE_NONE;	
}

CDBCNarrationGui::~CDBCNarrationGui(VOID)
{

}

RwBool CDBCNarrationGui::Create(VOID)
{
	m_pCinematicObject = GetNtlPLCinematicObject();

	m_slotOnMouseDown = m_pCinematicObject->GetDialog()->SigMouseDown().Connect( this, &CDBCNarrationGui::OnMouseDown );
	
	m_pCinematicObject->SetBalloonType( CNtlPLCinematicObject::BALLOON_TYPE_NORMAL );
	
	LinkMsg( g_EventDBCNarration );
	LinkMsg( g_EventESCinNarrationMode );
	LinkMsg( g_EventResetCinematic );

	NTL_RETURN( TRUE );
}

VOID CDBCNarrationGui::Destroy(VOID)
{
	UnLinkMsg( g_EventDBCNarration );
	UnLinkMsg( g_EventESCinNarrationMode );
	UnLinkMsg( g_EventResetCinematic );
}

INT CDBCNarrationGui::StateUpdate( RwUInt32 uiElapsedTime )
{
	switch( m_eUIState )
	{
	case STATE_BEGIN: BeginStateDirection(); break;
	case STATE_ING: IngStateDirection(); break;
	case STATE_END: EndStateDirection(); break;
	}

	return 1;
}

VOID CDBCNarrationGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventDBCNarration )
	{
		SNtlEventDBCNarration* pData = reinterpret_cast<SNtlEventDBCNarration*>( msg.pData );

		if( pData->eNarrationState == SNtlEventDBCNarration::DBC_NARRATION_START )
			CNtlPLEventGenerator::ResetCinematic( SNtlEventResetCinematic::DBCNARRATION );

		SetUIData( pData->pszTextureName, pData->pwszText, pData->eNarrationState );
	}
	else if( msg.Id == g_EventESCinNarrationMode )
	{
		if( m_eUIState == STATE_ING )
		{
			SetState( STATE_END );
		}			
	}
	else if( msg.Id == g_EventResetCinematic )
	{
		SNtlEventResetCinematic* pData = reinterpret_cast<SNtlEventResetCinematic*>( msg.pData );

		if( pData->eDemandResetObject != SNtlEventResetCinematic::DBCNARRATION )
		{
			if( m_eUIState != STATE_NONE )
			{
				SetState( STATE_NONE );
				CNtlSLEventGenerator::DBCNarrationEnd();
			}
		}		
	}
}

VOID CDBCNarrationGui::CreateInstance(VOID)
{
	if( !m_pInstance )
	{
		m_pInstance = NTL_NEW CDBCNarrationGui();
		if( !m_pInstance->Create() )
		{
			m_pInstance->Destroy();
			NTL_DELETE( m_pInstance );
		}		
	}
}

VOID CDBCNarrationGui::DeleteInstance(VOID)
{
	if( m_pInstance )
	{
		m_pInstance->Destroy();
		NTL_DELETE( m_pInstance );
	}
}

VOID CDBCNarrationGui::SetUIData( const RwChar* szTextureName, const WCHAR* wszText, RwInt32 eNarrationState )
{
	m_eNarrationState = eNarrationState;

	if( eNarrationState == SNtlEventDBCNarration::DBC_NARRATION_START )
	{
		SetState( STATE_BEGIN );
	}
	else if( eNarrationState == SNtlEventDBCNarration::DBC_NARRATION_END_ATONCE )
	{
		SetState( STATE_END );	
		return;
	}
	else
	{
		SetState( STATE_ING );
	}

	// 이미지 로드 및 이름 출력
	if( szTextureName )
	{
		m_pCinematicObject->SetImageTexture( const_cast<RwChar*>( szTextureName ) );
		m_pCinematicObject->ShowImage( TRUE );
		
		m_pCinematicObject->SetName( const_cast<WCHAR*>( GetDisplayStringManager()->GetString( "DST_SHENRON" ) ) );		
	}
	else
	{
		m_pCinematicObject->ShowImage( FALSE );
	}

	if( wszText )
	{
		m_pCinematicObject->GetBalloon()->Show( true );
		m_pCinematicObject->SetStaticText( wszText );
	}
	else
	{
		m_pCinematicObject->GetBalloon()->Show( false );				
	}
}

VOID CDBCNarrationGui::SetState( STATE eState )
{
	if( m_eUIState == eState )
		return;

	switch( eState )
	{
	case STATE_NONE:
		GetDialogManager()->OffMode();
		GetNtlPLCinematicObject()->SwitchDialog( false );
		GetDialogManager()->OpenGMDialog();
		GetDialogManager()->OpenDefaultDialog();
		GetNtlGuiManager()->RemoveUpdateFunc( m_pCinematicObject );
		GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_DIRECT );
		m_pCinematicObject->UnLinkUpdateCallBack();
		break;
	case STATE_BEGIN:
		if( m_eUIState == STATE_BEGIN )
			return;

		if( !GetDialogManager()->IsMode( DIALOGMODE_NARRATION ) )
		{
			GetDialogManager()->OnMode( DIALOGMODE_NARRATION );		
			GetNtlWorldConcept()->AddWorldPlayConcept( WORLD_PLAY_DIRECT );
			GetNtlPLCinematicObject()->SwitchDialog( true );
		}
		m_pCinematicObject->InitShowSetting();
		m_pCinematicObject->ShowImage( FALSE );
		m_pCinematicObject->GetText()->Show( false );
		m_pCinematicObject->GetHtmlText()->Show( false );
		m_pCinematicObject->CinematicFadeOut( FALSE, TRUE );	
		m_pCinematicObject->CinematicBackFadeOut( TRUE );
		m_pCinematicObject->GetDialog()->SetAlpha( 0 );
		m_pCinematicObject->SetImageAlpha( 0 );
		m_pCinematicObject->ResetCinematicCurrentTime();		
		m_pCinematicObject->SetFocusOnNPCBar( TRUE );
		GetNtlGuiManager()->AddUpdateFunc( m_pCinematicObject );
		m_pCinematicObject->LinkUpdateCallBack( this, &CDBCNarrationGui::StateUpdate );
		break;
	case STATE_ING:
		m_pCinematicObject->GetText()->Show( true );
		m_pCinematicObject->GetText()->SetAlpha( 0 );
		m_pCinematicObject->ResetCinematicCurrentTime();		
		break;
	case STATE_END:
		if( m_eUIState == STATE_END )
			return;

		m_pCinematicObject->CinematicFadeIn( FALSE, TRUE );		
		m_pCinematicObject->CinematicBackFadeIn( TRUE );
		m_pCinematicObject->GetDialog()->SetAlpha( 255 );
		m_pCinematicObject->SetImageAlpha( 255 );
		m_pCinematicObject->ResetCinematicCurrentTime();
		break;	
	}

	m_eUIState = eState;
}

VOID CDBCNarrationGui::BeginStateDirection(VOID)
{
	RwReal fCurrentTime = m_pCinematicObject->GetCinematicCurrentTime();

	if( fCurrentTime < CINEMATIC_MOVETIME )
	{
		m_pCinematicObject->CinematicFadeIn( TRUE, FALSE );
		m_pCinematicObject->CinematicBackFadeIn( FALSE, fCurrentTime );
		
		RwUInt8 ucAlpha = (RwUInt8)gui::GetResultLineInterpolation( fCurrentTime, CINEMATIC_MOVETIME, 0.0f, 255.0f );
		m_pCinematicObject->GetImageFrame()->SetAlpha( ucAlpha );
		m_pCinematicObject->SetImageAlpha( ucAlpha );
	}
	else
	{
		m_pCinematicObject->CinematicFadeIn( TRUE, TRUE );
		m_pCinematicObject->CinematicBackFadeIn( TRUE );

		m_pCinematicObject->GetImageFrame()->SetAlpha( 255 );
		m_pCinematicObject->SetImageAlpha( 255 );

		SetState( STATE_ING );		
	}
}

VOID CDBCNarrationGui::IngStateDirection(VOID)
{
	RwReal fCurrentTime = m_pCinematicObject->GetCinematicCurrentTime();

	if( fCurrentTime < CINEMATIC_TEXT_APPEARTIME )
	{
		m_pCinematicObject->GetText()->SetAlpha( (RwUInt8)gui::GetResultLineInterpolation( fCurrentTime, CINEMATIC_TEXT_APPEARTIME, 0.0f, 255.0f ) );
	}
	else
	{
		m_pCinematicObject->GetText()->SetAlpha( 255 );	
		SetState( STATE_IDLE );
	}
}

VOID CDBCNarrationGui::EndStateDirection(VOID)
{
	RwReal fCurrentTime = m_pCinematicObject->GetCinematicCurrentTime();

	if( fCurrentTime < CINEMATIC_MOVETIME )
	{
		m_pCinematicObject->CinematicFadeOut( TRUE, FALSE );
		m_pCinematicObject->CinematicBackFadeOut( FALSE, fCurrentTime );

		RwUInt8 ucAlpha = (RwUInt8)gui::GetResultLineInterpolation( fCurrentTime, CINEMATIC_MOVETIME, 255.0f, 0.0f );
		m_pCinematicObject->GetImageFrame()->SetAlpha( ucAlpha );
		m_pCinematicObject->SetImageAlpha( ucAlpha );
	}
	else
	{
		m_pCinematicObject->CinematicFadeOut( TRUE, TRUE );
		m_pCinematicObject->CinematicBackFadeOut( TRUE );

		m_pCinematicObject->GetImageFrame()->SetAlpha( 0 );
		m_pCinematicObject->SetImageAlpha( 0 );		

		SetState( STATE_NONE );	
		CNtlSLEventGenerator::DBCNarrationEnd();
	}
}

VOID CDBCNarrationGui::OnMouseDown( const CKey& key )
{
	if( m_eUIState == STATE_ING || m_eUIState == STATE_IDLE )
	{
		RwReal fCurrentTime = m_pCinematicObject->GetCinematicCurrentTime();

		if( fCurrentTime < CINEMATIC_TEXT_APPEARTIME )
		{
			m_pCinematicObject->SetCinematicCurrentTime( CINEMATIC_TEXT_APPEARTIME );
		}		
		else if( m_eNarrationState == SNtlEventDBCNarration::DBC_NARRATION_END )
		{
			SetState( STATE_END );
		}
	}
}









