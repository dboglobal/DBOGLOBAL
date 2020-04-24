#include "precomp_dboclient.h"
#include "TBMinorDirectionGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlWorldConceptTB.h"
#include "NtlCameraManager.h"
#include "NtlSLLuaGlue.h"

// client
#include "DboGlobal.h"
#include "DiceManager.h"
#include "TBGui.h"

#define BUDOKAI_MINOR_DICE_START_TIME	3
#define BUDOKAI_MINOR_DICE_NOTIFY_TIME	5		// 관전자의 경우, Flash연출시간(현재1.2초)를 고려한 시간.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stENDINFO

VOID stENDINFO::Create( gui::CFlash* pEnd )
{
	if( pEnd )
	{
		bValid = TRUE;
		pflaEnd = pEnd;
		rtOriginalEnd = pflaEnd->GetPosition();

		pflaEnd->Show( false );
	}
}

VOID stENDINFO::Destroy(VOID)
{
	pflaEnd->SigMovieEnd().Disconnect( slotMovieEnd );

	bValid = FALSE;
	pflaEnd = NULL;
}

VOID stENDINFO::Update( RwReal fElapsedTime )
{
	if( bValid )
		pflaEnd->Update( fElapsedTime );
}

VOID stENDINFO::StartMovie(VOID)
{
	if( bValid )
	{
		pflaEnd->Show( true );
		pflaEnd->RestartMovie();
	}
}

VOID stENDINFO::EndMovie(VOID)
{
	if( bValid )
	{
		pflaEnd->PlayMovie( FALSE );
		pflaEnd->Show( false );
	}
}

VOID stENDINFO::ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	if( IsValid() )
		SizePosCalculation( pflaEnd, rtOriginalEnd, nScreenWidth, nScreenHeight );	

}

VOID stENDINFO::OnEndMovieEnd( gui::CComponent* pComponent )
{
	EndMovie();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stWINNERINFO

VOID stWINNERINFO::Create( gui::CFlash* pWinner, gui::CStaticBox* pWinnerName )
{
	if( pWinner && pWinnerName )
	{
		bValid = TRUE;
		pflaWinner = pWinner;
		pstbWinnerName = pWinnerName;
		rtOriginalWinner = pflaWinner->GetPosition();
		
		slotWinnerMovieEnd = pflaWinner->SigMovieEnd().Connect( this, &stWINNERINFO::OnWinnerMovieEnd );
		slotWinnerFSCallBack = pflaWinner->SigFSCallBack().Connect( this, &stWINNERINFO::OnWinnerFSCallBack );

		pflaWinner->Show( false );
		pstbWinnerName->Show( false );
	}
}

VOID stWINNERINFO::Destroy(VOID)
{
	pflaWinner->SigMovieEnd().Disconnect( slotWinnerMovieEnd );
	pflaWinner->SigFSCallBack().Disconnect( slotWinnerFSCallBack );

	bValid = FALSE;
	pflaWinner = NULL;
	pstbWinnerName = NULL;
}

VOID stWINNERINFO::Update( RwReal fElapsedTime )
{
	if( bValid )
		pflaWinner->Update( fElapsedTime );
}

VOID stWINNERINFO::SetWinnerName( const WCHAR* wszWinner )
{
	if( bValid )
		pstbWinnerName->SetText( wszWinner );
}

VOID stWINNERINFO::StartMovie(VOID)
{
	if( bValid )
	{
		pflaWinner->Show( true );
		pflaWinner->RestartMovie();
	}
}

VOID stWINNERINFO::EndMovie(VOID)
{
	if( bValid )
	{
		pflaWinner->PlayMovie( FALSE );
		pflaWinner->Show( false );
	}
}

VOID stWINNERINFO::ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	if( IsValid() )
		SizePosCalculation( pflaWinner, rtOriginalWinner, nScreenWidth, nScreenHeight );	
}

VOID stWINNERINFO::OnWinnerMovieEnd( gui::CComponent* pComponent )
{
	EndMovie();
}

VOID stWINNERINFO::OnWinnerFSCallBack( const RwChar* szCommand, const RwChar* szArgs )
{
	if( !strcmp( szCommand, "TextStart" )  )
	{
		pstbWinnerName->Show( true );
	}
	else if( !strcmp( szCommand, "TextEnd" ) )
	{
		pstbWinnerName->Show( false );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBMinorDirectionGui

CTBMinorDirectionGui::CTBMinorDirectionGui( const RwChar* pName )
: CTBCommonDirectionGui( pName )
{

}

CTBMinorDirectionGui::~CTBMinorDirectionGui(VOID)
{

}

RwBool CTBMinorDirectionGui::Create(VOID)
{
	NTL_FUNCTION( "CTBMinorDirectionGui::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\BudokaiMinorDirection.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain" ) );

	m_stOneLine.Create( reinterpret_cast<gui::CFlash*>( GetComponent( "flaOne" ) ) );
	m_stTwoLine.Create( reinterpret_cast<gui::CFlash*>( GetComponent( "flaFirst" ) ), reinterpret_cast<gui::CFlash*>( GetComponent( "flaSecond" ) ) ); 
	//m_stScroll.Create( reinterpret_cast<gui::CFlash*>( GetComponent( "flaScroll" ) ), reinterpret_cast<gui::CHtmlBox*>( GetComponent( "hbxText" ) ) );
	m_stEnd.Create( reinterpret_cast<gui::CFlash*>( GetComponent( "flaEnd" ) ) );
	m_stWinner.Create( reinterpret_cast<gui::CFlash*>( GetComponent( "flaWinner" ) ), reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbWinner" ) ) );

	m_slotWinnerMovieEnd = m_stWinner.pflaWinner->SigMovieEnd().Connect( this, &CTBMinorDirectionGui::OnWinnerMovieEnd );
	
	GetNtlGuiManager()->AddUpdateFunc( this );

	LinkMsg( g_EventMinorMatchStageFinish );
	LinkMsg( g_EventMinorMatchMatchFinish );
	LinkMsg( g_EventMinorMatchStateUpdate );
	LinkMsg( g_EventMinorMatchSelection );
	LinkMsg( g_EventResize );

	ReCalcPosition( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );
	Show( true );

	NTL_RETURN( TRUE );
}

VOID CTBMinorDirectionGui::Destroy(VOID)
{
	m_stOneLine.Destroy();
	m_stTwoLine.Destroy();
	//m_stScroll.Destroy();
	m_stEnd.Destroy();
	m_stWinner.Destroy();

	GetNtlGuiManager()->RemoveUpdateFunc( this );
	
	CEventTimer::RemoveTimer( m_hDiceWinnerSelectTimer );
	CEventTimer::RemoveTimer( m_hDiceWinnerShowTimer );

	UnLinkMsg( g_EventMinorMatchStageFinish );
	UnLinkMsg( g_EventMinorMatchMatchFinish );
	UnLinkMsg( g_EventMinorMatchStateUpdate );
	UnLinkMsg( g_EventMinorMatchSelection );
	UnLinkMsg( g_EventResize );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CTBMinorDirectionGui::Update( RwReal fElapsed )
{
	CTBCommonDirectionGui::Update( fElapsed );

	m_stEnd.Update( fElapsed );
	m_stWinner.Update( fElapsed );	
}

VOID CTBMinorDirectionGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventMinorMatchStateUpdate )
	{
		SNtlEventMinorMatchStateUpdate* pData = reinterpret_cast<SNtlEventMinorMatchStateUpdate*>( msg.pData );
		
		if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_WAIT )
		{
			if( pData->bEnterState )
			{
				m_stOneLine.LoadFlash( GetTBFlashName()->GetWait(), WAIT );
				m_stOneLine.StartMovie();
			}
			else
			{
				m_stOneLine.EndMovie();
			}
		}
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_MATCH_READY )
		{
			if( pData->bEnterState )
			{
				m_stOneLine.LoadFlash( GetTBFlashName()->GetPreliminary(), TITLE );
				m_stOneLine.StartMovie();
			}			
			else
			{
				m_stOneLine.EndMovie();
			}
		}
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_DIRECTION )
		{
			
		}
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_READY )
		{
			if( pData->bEnterState )
			{
				m_stTwoLine.LoadFlash( GetTBFlashName()->GetReady(), GetTBFlashName()->GetStart(), READYSTART, FALSE );
				m_stTwoLine.StartFirstMovie();
			}			
		}
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_RUN )
		{
			if( pData->bEnterState )
				m_stTwoLine.StartSecondMovie();
		}				
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_FINISH )
		{
			if( pData->bEnterState )
				m_stEnd.StartMovie();
		}
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_SELECTION )
		{
			if( !pData->bEnterState )
			{
				GetDiceManager()->Close( DICE_WORKD_ID_BUDOKAI_TENKAICHI_SELECT_WINNER );
				CTBGui::GetInstance()->HideCounterTime();
			}
		}
	}
	else if( msg.Id == g_EventMinorMatchMatchFinish )
	{
		SNtlEventMinorMatchMatchFinish* pData = reinterpret_cast<SNtlEventMinorMatchMatchFinish*>( msg.pData );
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CTBMinorDirectionGui::HandleEvents : must World concept is valid" );		

		stTBudokaiTeam* pWinnerTeam = pTBWorldConcept->FindTeam( pData->wMatchWinner );
		if( pWinnerTeam )
			m_stWinner.SetWinnerName( pWinnerTeam->wstrTeamName.c_str() );
		else
			m_stWinner.SetWinnerName( NULL );				

		m_stWinner.StartMovie();
	}
	else if( msg.Id == g_EventMinorMatchSelection )
	{
		SNtlEventMinorMatchSelection* pData = reinterpret_cast<SNtlEventMinorMatchSelection*>( msg.pData );

		// 데이터 준비
		m_WinnerTeam = pData->wWinnerTeam;
		m_bySelectionCount = pData->byTeamCount;
		memcpy( m_aSelectionData, pData->aSelection, sizeof( sMINORMATCH_SELECTION_DATA ) * pData->byTeamCount );

		// 연출 준비
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CTBMinorDirectionGui::HandleEvents : must World concept is valid" );	

		stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
		if( pMember )
		{
			GetAlarmManager()->FormattedAlarmMessage( "DST_BUDOKAI_MINOR_DICE_READY_NOTIFY", FALSE, NULL, BUDOKAI_MINOR_DICE_START_TIME );					
			m_hDiceWinnerSelectTimer = CEventTimer::AddTimer( BUDOKAI_MINOR_DICE_START_TIME * 1000, this, &CTBMinorDirectionGui::OnStartDice );
			CTBGui::GetInstance()->SetCounterTime( BUDOKAI_MINOR_DICE_START_TIME );
			CDboEventGenerator::DiceStart( DICE_WORKD_ID_BUDOKAI_TENKAICHI_SELECT_WINNER );
		}
		else
		{
			GetAlarmManager()->AlarmMessage( "DST_BUDOKAI_MINOR_WAIT_FOR_DICE_NOTIFY" );										
		}				
		m_hDiceWinnerShowTimer = CEventTimer::AddTimer( BUDOKAI_MINOR_DICE_NOTIFY_TIME * 1000, this, &CTBMinorDirectionGui::OnShowDiceResult );		
	}
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		ReCalcPosition( pData->iWidht, pData->iHeight );
	}
}

VOID CTBMinorDirectionGui::ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	CTBCommonDirectionGui::ReCalcPosition( nScreenWidth, nScreenHeight );

	m_stEnd.ReCalcPosition( nScreenWidth, nScreenHeight );
	m_stWinner.ReCalcPosition( nScreenWidth, nScreenHeight );	
}

VOID CTBMinorDirectionGui::OnWinnerMovieEnd( gui::CComponent* pComponent )
{
	Show( false );
	CDboEventGenerator::FlashFinishNotify( 0, 0, 0, "MinorMatchResult", NULL );
}

//VOID CTBMinorDirectionGui::OnTitleMovieEnd( gui::CComponent* pComponent )
//{
//	//m_stScroll.SetHtmlText( "" );
//	//m_stScroll.StartMovie();	
//}

VOID CTBMinorDirectionGui::OnStartDice(VOID)
{
	CEventTimer::RemoveTimer( m_hDiceWinnerSelectTimer );
	CTBGui::GetInstance()->HideCounterTime();

	// 연출 준비
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CTBMinorDirectionGui::HandleEvents : must World concept is valid" );	
	SERIAL_HANDLE hAvatar = Logic_GetAvatarHandle();
	stTBudokaiMember* pMember = pTBWorldConcept->FindMember( hAvatar );
	RwUInt8 byResult = 0;

	if( pMember )
	{
		for( RwUInt8 i = 0 ; i < m_bySelectionCount ; ++i )
		{
			if( pMember->pTeam->wTeamType == m_aSelectionData[i].teamType )
			{
				byResult = m_aSelectionData[i].byNumber;
				break;
			}
		}		

		CDboEventGenerator::DiceResult( hAvatar, DICE_WORKD_ID_BUDOKAI_TENKAICHI_SELECT_WINNER, byResult );
	}	
}

VOID CTBMinorDirectionGui::OnShowDiceResult(VOID)
{
	CEventTimer::RemoveTimer( m_hDiceWinnerShowTimer );
	
	// 각 캐릭터의 주사위결과를 표현.
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CTBMinorDirectionGui::HandleEvents : must World concept is valid" );	

	RwUInt8 byWinnerScore = 0;
	stTBudokaiTeam* pWinnerTeam = NULL;
	SERIAL_HANDLE hAvatar = Logic_GetAvatarHandle();

	for( RwUInt8 i = 0 ; i < m_bySelectionCount ; ++i )
	{
		stTBudokaiTeam* pTeam = pTBWorldConcept->FindTeam( m_aSelectionData[i].teamType );

		if( pTeam )
		{
			if( m_aSelectionData[i].teamType == m_WinnerTeam )
			{
				pWinnerTeam = pTeam;
				byWinnerScore = m_aSelectionData[i].byNumber;
			}			

			VEC_TBTEAMMEMBER_ITER iter;
			for( iter = pTeam->vecpMember.begin() ; iter != pTeam->vecpMember.end() ; ++iter )
			{
				stTBudokaiMember* pMember = (*iter);

				if( pMember->hSerial != hAvatar )
					CDboEventGenerator::DiceResultShow( pMember->hSerial, DICE_WORKD_ID_BUDOKAI_TENKAICHI_SELECT_WINNER, m_aSelectionData[i].byNumber );
			}			

			GetAlarmManager()->FormattedAlarmMessage( "DST_BUDOKAI_MINOR_DICE_SCORE_NOTIFY", FALSE, NULL, pTeam->wstrTeamName.c_str(), m_aSelectionData[i].byNumber );
		}		
	}
	
	if( pWinnerTeam )
	{
		GetAlarmManager()->FormattedAlarmMessage( "DST_BUDOKAI_MINOR_DICE_WIN_NOTIFY", FALSE, NULL, pWinnerTeam->wstrTeamName.c_str(), byWinnerScore );
	}
}