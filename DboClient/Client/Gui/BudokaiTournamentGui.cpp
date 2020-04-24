#include "precomp_dboclient.h"
#include "BudokaiTournamentGui.h"

// core
#include "NtlDebug.h"

// shared
#include "NtlBudokai.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// client
#include "DboGlobal.h"
#include "DboPacketGenerator.h"
#include "DboEvent.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "DialogManager.h"
#include "TBTournamentMatchInfoGui.h"

std::map<RwUInt16,sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA> CBudokaiTournamentGui::m_mapIndividualEntryData;
std::map<RwUInt16,sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA> CBudokaiTournamentGui::m_mapTeamEntryData;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBudokaiTournamentEntry

#define BUDOKAI_TOURNAMENT_ENTRY_MARK_1_LEFT	43
#define BUDOKAI_TOURNAMENT_ENTRY_MARK_1_TOP		481
#define BUDOKAI_TOURNAMENT_ENTRY_MARK_WIDTH		31
#define BUDOKAI_TOURNAMENT_ENTRY_MARK_HEIGHT	31
#define BUDOKAI_TOURNAMENT_ENTRY_MARK_OFFSET	45

CBudokaiTournamentEntry::CBudokaiTournamentEntry( gui::CComponent* pParent, RwUInt8 byPos, 
												  RwInt32 nMark1Left, RwInt32 nMark1Top, RwInt32 nWidth, RwInt32 nHeight, RwInt32 nOffset )
: m_byPos( byPos ), m_wJoinID( INVALID_WORD )
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	// 포지션 위치정보와 서피스를 선택.
	CRectangle rtMark;
	rtMark.SetRectWH( nMark1Left + byPos * nOffset, nMark1Top, nWidth, nHeight );					  

	CHAR buf[256];
	sprintf_s( buf, 256, "srfEntryMark%d", byPos + 1 );
	m_ppnlMark = NTL_NEW gui::CPanel( rtMark, pParent, pSurfaceManager, pSurfaceManager->GetSurface( "BudokaiTournament.srf", buf ) );
	CRectangle rtScreen = m_ppnlMark->GetScreenRect();

	m_surFocus.SetSurface( pSurfaceManager->GetSurface( "BudokaiTournament.srf", "srfEntryMarkFocus" ) );
	m_surFocus.SetRect( rtScreen );
	m_surFocus.Show( FALSE );

	m_slotMouseEnter = m_ppnlMark->SigMouseEnter().Connect( this, &CBudokaiTournamentEntry::OnMouseEnter );
	m_slotMouseLeave = m_ppnlMark->SigMouseLeave().Connect( this, &CBudokaiTournamentEntry::OnMouseLeave );
	m_slotMove = m_ppnlMark->SigMove().Connect( this, &CBudokaiTournamentEntry::OnMove );
	m_slotPaint = m_ppnlMark->SigPaint().Connect( this, &CBudokaiTournamentEntry::OnPaint );
}

CBudokaiTournamentEntry::~CBudokaiTournamentEntry(VOID)
{
	NTL_DELETE( m_ppnlMark );	
}

VOID CBudokaiTournamentEntry::SetEntry( const WCHAR* pName, RwUInt16 wJoinID )
{
	// 이름 세팅
	m_wstrName = pName;
	m_wJoinID = wJoinID;
}

VOID CBudokaiTournamentEntry::OnMouseEnter( gui::CComponent* pComponent )
{
	CRectangle rect =  m_ppnlMark->GetScreenRect();

	// Focus
	m_surFocus.Show( TRUE );

	// 팝업
	if( m_wstrName.size() )
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, rect.right, rect.bottom, (VOID*)m_wstrName.c_str(), DIALOG_BUDOKAI_TOURNAMENT );
}

VOID CBudokaiTournamentEntry::OnMouseLeave( gui::CComponent* pComponent )
{
	// Focus 종료
	m_surFocus.Show( FALSE );

	// 팝업 종료
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_BUDOKAI_TOURNAMENT )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CBudokaiTournamentEntry::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_ppnlMark->GetScreenRect();
	m_surFocus.SetRect( rtScreen );
}

VOID CBudokaiTournamentEntry::OnPaint(VOID)
{
	m_surFocus.Render();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBudokaiTournamentFightInfo

#define BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_ORIGIN_LEFT			67
#define BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_ORIGIN_TOP			432
#define BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_WIDTH				28
#define BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_HEIGHT				28
#define BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_VERTICAL_OFFSET		51
#define BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_HORIZONTAL_OFFSET	45
#define BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_END_Y			481

#define BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_WIDTH			2
#define BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_HEIGHT			2
#define BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_ANIMATION_SPEED		100		// pixel / second
#define BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_CLICK_WAIT_TIME		1000

CBudokaiTournamentFightInfo::CBudokaiTournamentFightInfo( CBudokaiTournamentFightInfo* pAboveNode, gui::CComponent* pParent, RwUInt8 byMatchNumber, RwUInt8 byMatchLevel, RwUInt8 byServerMatchNumber, RwUInt8 byServerMatchLevel, RwBool bParty, 
														  RwInt32 nOriginLeft, RwInt32 nOriginTop , RwInt32 nWidth, RwInt32 nHeight, RwInt32 nVerticalOffset, RwInt32 nHorizontalOffset, RwInt32 nLineEndY )
: m_fCurrentTime( 0.0f ), m_byMatchNumber( byMatchNumber ), m_byMatchLevel( byMatchLevel ), m_byServerMatchLevel( byServerMatchLevel ), m_byServerMatchNumber( byServerMatchNumber ), 
  m_nEndOfBelowAnimation( 0 ), m_uiWaitTime( 0 ), m_wWinnerTeamJoinID( INVALID_WORD ), m_eAnimationState( NONE ), m_pAboveFightInfo( pAboveNode ), m_pdlgParent( pParent ), m_bParty( bParty ),
  m_nHorizontalOffset( nHorizontalOffset ), m_nOriginLeft( nOriginLeft )
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	// 포지션 위치정보와 서피스를 선택.
	CRectangle rtMark;
	rtMark.SetRectWH( GetHorizontalOrigin() + byMatchNumber * GetHorizontalOffset(), nOriginTop - ( byMatchLevel * nVerticalOffset ), nWidth, nHeight );
		
	m_pbtnBattleInfo = NTL_NEW gui::CButton( rtMark, pParent, pSurfaceManager );
	m_pbtnBattleInfo->AddSurfaceFocus( pSurfaceManager->GetSurface( "BudokaiTournament.srf", "srfFightInfoFocus" ) );
	m_pbtnBattleInfo->AddSurfaceDown( pSurfaceManager->GetSurface( "BudokaiTournament.srf", "srfFightInfoDown" ) );
	m_pbtnBattleInfo->AddSurfaceUp( pSurfaceManager->GetSurface( "BudokaiTournament.srf", "srfFightInfoUp" ) );

	// Bar 위치 저장
	RwInt32 nMarkCenterX, nMarkCenterY;
	gui::CSurface surLine = pSurfaceManager->GetSurface( "BudokaiTournament.srf", "srfRedLine" );
	rtMark.GetCenterPosition( &nMarkCenterX, &nMarkCenterY );

	// 맨 밑단에는 Line이 하나 더 있음.
	if( byMatchLevel == 0 )
	{
		m_surLeftInitVerticalLine.SetSurface( surLine );
		m_rtLeftInitVerticalLine.SetRect( nMarkCenterX - GetHorizontalOffset() / 4 - BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_WIDTH / 2,
								 	      nMarkCenterY - BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_HEIGHT / 2 + BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_HEIGHT,
										  nMarkCenterX - GetHorizontalOffset() / 4 + BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_WIDTH / 2, nLineEndY );										  
						
		m_surRightInitVerticalLine.SetSurface( surLine );
		m_rtRightInitVerticalLine.SetRect( nMarkCenterX + GetHorizontalOffset() / 4 - BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_WIDTH / 2,
										   nMarkCenterY - BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_HEIGHT / 2 + BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_HEIGHT,
										   nMarkCenterX + GetHorizontalOffset() / 4 + BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_WIDTH / 2, nLineEndY );										   

		m_fMoveTimeInitVertical = (RwReal)m_rtLeftInitVerticalLine.GetHeight() / BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_ANIMATION_SPEED;
	}
	m_surLeftInitVerticalLine.Show( FALSE );
	m_surRightInitVerticalLine.Show( FALSE );

	m_surLeftHorizontalLine.Show( FALSE );
	m_surLeftHorizontalLine.SetSurface( surLine );
	m_rtLeftHorizontalLine.SetRect( nMarkCenterX - GetHorizontalOffset() / 4 - BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_WIDTH / 2,
									nMarkCenterY - BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_HEIGHT / 2,
									nMarkCenterX, 
									nMarkCenterY + BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_HEIGHT / 2 );

	m_surRightHorizontalLine.Show( FALSE );
	m_surRightHorizontalLine.SetSurface( surLine );
	m_rtRightHorizontalLine.SetRect( nMarkCenterX, 
									 nMarkCenterY - BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_HEIGHT / 2,
									 nMarkCenterX + GetHorizontalOffset() / 4 + BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_WIDTH / 2,
									 nMarkCenterY + BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_HEIGHT / 2 );

	m_fMoveTimeHorizontal = (RwReal)m_rtLeftHorizontalLine.GetWidth() / BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_ANIMATION_SPEED;
	
	m_surCenterVerticalLine.Show( FALSE );
	m_surCenterVerticalLine.SetSurface( surLine );
	m_rtCenterVerticalLine.SetRect( nMarkCenterX - BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_WIDTH / 2,
									nMarkCenterY - nVerticalOffset,
									nMarkCenterX + BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_WIDTH / 2,
									nMarkCenterY );

	m_fMoveTimeVertical = (RwReal)m_rtCenterVerticalLine.GetHeight() / BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_ANIMATION_SPEED;

	m_slotClickInfoBtn = m_pbtnBattleInfo->SigClicked().Connect( this, &CBudokaiTournamentFightInfo::OnClickInfoBtn );
	m_slotMove = m_pdlgParent->SigMove().Connect( this, &CBudokaiTournamentFightInfo::OnMove );
	m_slotPaint = m_pdlgParent->SigPaint().Connect( this, &CBudokaiTournamentFightInfo::OnPaint );
}

CBudokaiTournamentFightInfo::~CBudokaiTournamentFightInfo(VOID)
{
	NTL_DELETE( m_pbtnBattleInfo );
}

VOID CBudokaiTournamentFightInfo::Update( RwReal fElapsedTime )
{
	if( m_stEntry[0].bEntry && m_stEntry[1].bEntry )
	{	
		switch( m_eAnimationState )
		{
		case NONE : 
		case END : return;
		case INIT_VERTICAL : UpdateInitVertical( fElapsedTime ); break;
		case HORIZONTAL : UpdateHorizontal( fElapsedTime ); break;
		case VERTICAL : UpdateVertical( fElapsedTime ); break;
		}	
	}	
}

VOID CBudokaiTournamentFightInfo::UpdateInitVertical( RwReal fElapsedTime )
{
	CRectangle rtScreen = m_pdlgParent->GetScreenRect();
	m_fCurrentTime += fElapsedTime;
	
	if( m_fCurrentTime < m_fMoveTimeInitVertical )
	{
		RwInt32 nYPos = (RwInt32)gui::GetResultLineInterpolation( m_fCurrentTime, m_fMoveTimeInitVertical, (RwReal)m_rtLeftInitVerticalLine.bottom, (RwReal)m_rtLeftInitVerticalLine.top );
	
		if( m_surLeftInitVerticalLine.IsShow() )
			m_surLeftInitVerticalLine.SetRect( m_rtLeftInitVerticalLine.left + rtScreen.left, nYPos + rtScreen.top, m_rtLeftInitVerticalLine.right + rtScreen.left, m_rtLeftInitVerticalLine.bottom + rtScreen.top );

		if( m_surRightInitVerticalLine.IsShow() )
			m_surRightInitVerticalLine.SetRect( m_rtRightInitVerticalLine.left + rtScreen.left, nYPos + rtScreen.top, m_rtRightInitVerticalLine.right + rtScreen.left, m_rtRightInitVerticalLine.bottom + rtScreen.top );
	}	
	else
	{
		if( m_surLeftInitVerticalLine.IsShow() )
			m_surLeftInitVerticalLine.SetRect( m_rtLeftInitVerticalLine.left + rtScreen.left, m_rtLeftInitVerticalLine.top + rtScreen.top, m_rtLeftInitVerticalLine.right + rtScreen.left, m_rtLeftInitVerticalLine.bottom + rtScreen.top );
		
		if( m_surRightInitVerticalLine.IsShow() )
			m_surRightInitVerticalLine.SetRect( m_rtRightInitVerticalLine.left + rtScreen.left, m_rtRightInitVerticalLine.top + rtScreen.top, m_rtRightInitVerticalLine.right + rtScreen.left, m_rtRightInitVerticalLine.bottom + rtScreen.top );

		SetAnimationState( HORIZONTAL );
	}
}

VOID CBudokaiTournamentFightInfo::UpdateHorizontal( RwReal fElapsedTime )
{
	CRectangle rtScreen = m_pdlgParent->GetScreenRect();
	m_fCurrentTime += fElapsedTime;

	if( m_fCurrentTime < m_fMoveTimeHorizontal )
	{
		if( m_surLeftHorizontalLine.IsShow() )
		{
			RwInt32 nLeftXPos = (RwInt32)gui::GetResultLineInterpolation( m_fCurrentTime, m_fMoveTimeHorizontal, (RwReal)m_rtLeftHorizontalLine.left, (RwReal)m_rtLeftHorizontalLine.right );
			m_surLeftHorizontalLine.SetRect( m_rtLeftHorizontalLine.left + rtScreen.left, m_rtLeftHorizontalLine.top + rtScreen.top, nLeftXPos + rtScreen.left, m_rtLeftHorizontalLine.bottom + rtScreen.top );
		}
		
		if( m_surRightHorizontalLine.IsShow() )
		{
			RwInt32 nRightXPos = (RwInt32)gui::GetResultLineInterpolation( m_fCurrentTime, m_fMoveTimeHorizontal, (RwReal)m_rtRightHorizontalLine.right, (RwReal)m_rtRightHorizontalLine.left );
			m_surRightHorizontalLine.SetRect( nRightXPos + rtScreen.left, m_rtRightHorizontalLine.top + rtScreen.top, m_rtRightHorizontalLine.right + rtScreen.left,  m_rtRightHorizontalLine.bottom + rtScreen.top );
		}		
	}
	else
	{
		if( m_surLeftHorizontalLine.IsShow() )
			m_surLeftHorizontalLine.SetRect( m_rtLeftHorizontalLine.left + rtScreen.left, m_rtLeftHorizontalLine.top + rtScreen.top, m_rtLeftHorizontalLine.right + rtScreen.left, m_rtLeftHorizontalLine.bottom + rtScreen.top );

		if( m_surRightHorizontalLine.IsShow() )
			m_surRightHorizontalLine.SetRect( m_rtRightHorizontalLine.left + rtScreen.left, m_rtRightHorizontalLine.top + rtScreen.top, m_rtRightHorizontalLine.right + rtScreen.left,  m_rtRightHorizontalLine.bottom + rtScreen.top );

		SetAnimationState( VERTICAL );
	}
}

VOID CBudokaiTournamentFightInfo::UpdateVertical( RwReal fElapsedTime )
{
	CRectangle rtscreen = m_pdlgParent->GetScreenRect();
	m_fCurrentTime += fElapsedTime;

	if( m_fCurrentTime < m_fMoveTimeVertical )
	{
		RwInt32 nYPos = (RwInt32)gui::GetResultLineInterpolation( m_fCurrentTime, m_fMoveTimeVertical, (RwReal)m_rtCenterVerticalLine.bottom, (RwReal)m_rtCenterVerticalLine.top );

		m_surCenterVerticalLine.SetRect( m_rtCenterVerticalLine.left + rtscreen.left, nYPos + rtscreen.top, m_rtCenterVerticalLine.right + rtscreen.left, m_rtCenterVerticalLine.bottom + rtscreen.top );
	}
	else
	{
		m_surCenterVerticalLine.SetRect( m_rtCenterVerticalLine.left + rtscreen.left, m_rtCenterVerticalLine.top + rtscreen.top, m_rtCenterVerticalLine.right + rtscreen.left, m_rtCenterVerticalLine.bottom + rtscreen.top );

		SetAnimationState( END );
	}
}

VOID CBudokaiTournamentFightInfo::SetFightResult( RwUInt16 wWinnerJoinID, RwUInt16 wJoinID1, RwUInt16 wJoinID2, RwUInt8 byScore1, RwUInt8 byScore2 )
{
	if( m_eAnimationState == END )
		return;

	m_wWinnerTeamJoinID = wWinnerJoinID;
	m_stEntry[0].wJoinID = wJoinID1;
	m_stEntry[1].wJoinID = wJoinID2;
	m_stEntry[0].byScore = byScore1;
	m_stEntry[1].byScore = byScore2;

	if( m_wWinnerTeamJoinID == m_stEntry[0].wJoinID )
		m_wLoserTeamJoinID = m_stEntry[1].wJoinID;
	else if( m_wWinnerTeamJoinID == m_stEntry[1].wJoinID )
		m_wLoserTeamJoinID = m_stEntry[0].wJoinID;
	
	if( m_byMatchLevel == 0 )
		SetAnimationState( INIT_VERTICAL );
	else
		SetAnimationState( HORIZONTAL );	
}

VOID CBudokaiTournamentFightInfo::SetEntryData( RwUInt8 byIdx, RwUInt16 wJoinID )
{
	NTL_ASSERT( byIdx >= 0 && byIdx < 2, "CBudokaiTournamentFightInfo::SetEntryData : Invalid Index" );

	m_stEntry[byIdx].wJoinID = wJoinID;	
	m_stEntry[byIdx].bEntry = TRUE;
}

VOID CBudokaiTournamentFightInfo::SetAnimationState( ANIMATIONSTATE eAnimationState )
{
	m_fCurrentTime = 0.0f;
	m_eAnimationState = eAnimationState;

	switch( m_eAnimationState )
	{
	case INIT_VERTICAL :
		{
			if( m_wWinnerTeamJoinID != INVALID_WORD )
			{
				if( m_wWinnerTeamJoinID == m_stEntry[0].wJoinID )
					m_surLeftInitVerticalLine.Show( TRUE );
				else if( m_wWinnerTeamJoinID == m_stEntry[1].wJoinID )
					m_surRightInitVerticalLine.Show( TRUE ); 			
			}			
		}
		break;
	case HORIZONTAL :
		{
			if( m_wWinnerTeamJoinID != INVALID_WORD )
			{
				if( m_wWinnerTeamJoinID == m_stEntry[0].wJoinID )
					m_surLeftHorizontalLine.Show( TRUE );
				else if( m_wWinnerTeamJoinID == m_stEntry[1].wJoinID )
					m_surRightHorizontalLine.Show( TRUE ); 			
			}			
		}
		break;
	case VERTICAL :
		{
			if( m_wWinnerTeamJoinID != INVALID_WORD )
				m_surCenterVerticalLine.Show( TRUE ); 
		}
		break;
	case END : 
		{
			RwUInt8 nIdx;

			if( IsLeftNode() )
				nIdx = 0;
			else
				nIdx = 1;
			
			if( m_pAboveFightInfo )
				m_pAboveFightInfo->SetEntryData( nIdx, m_wWinnerTeamJoinID );
		}		
		break;	
	}	
}

RwInt32 CBudokaiTournamentFightInfo::GetHorizontalOrigin(VOID)
{
	RwInt32 nOrigin = m_nOriginLeft;	

	for( RwInt32 i = 0 ; i < m_byMatchLevel ; ++i )
	{
		nOrigin += (RwInt32)pow( (RwReal)2, i ) * m_nHorizontalOffset;
	}

	return nOrigin;
}

RwInt32 CBudokaiTournamentFightInfo::GetHorizontalOffset(VOID)
{
	return (RwInt32)pow( (RwReal)2, m_byMatchLevel + 1 ) * m_nHorizontalOffset;
}

RwBool CBudokaiTournamentFightInfo::IsLeftNode(VOID)
{
	return !( m_byMatchNumber % 2 );
}

VOID CBudokaiTournamentFightInfo::OnClickInfoBtn( gui::CComponent* pComponent )
{
	// 연속클릭방지 시간 저장 및 시간 정보 유효 확인
	RwUInt32 uiCurrentTime = GetTickCount();

	if( m_uiWaitTime - uiCurrentTime > BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_CLICK_WAIT_TIME )
	{
		RwUInt16 wLeftJoinID = INVALID_WORD;
		RwUInt16 wRightJoinID = INVALID_WORD;
		m_uiWaitTime = uiCurrentTime;		

		CTBTournamentMatchInfoGui* pMatchInfoGui = reinterpret_cast<CTBTournamentMatchInfoGui*>( GetDialogManager()->GetDialog( DIALOG_BUDOKAI_TOURNAMENT_MATCHINFO ) );
		pMatchInfoGui->SetMatchData( m_byServerMatchLevel, m_byServerMatchNumber, m_stEntry[MATCH_TEAM_TYPE_TEAM1].byScore, m_stEntry[MATCH_TEAM_TYPE_TEAM2].byScore );
		GetDialogManager()->OpenDialog( DIALOG_BUDOKAI_TOURNAMENT_MATCHINFO );

		if( m_bParty )
		{
			sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* pLeftData = NULL;
			sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* pRightData = NULL;
						
			if( m_stEntry[MATCH_TEAM_TYPE_TEAM1].wJoinID != INVALID_WORD )
			{
				pLeftData =	CBudokaiTournamentGui::FindTeamEntryData( m_stEntry[MATCH_TEAM_TYPE_TEAM1].wJoinID );

				if( !pLeftData )
					wLeftJoinID = m_stEntry[MATCH_TEAM_TYPE_TEAM1].wJoinID;
				else
					pMatchInfoGui->SetTeamData( pLeftData, MATCH_TEAM_TYPE_TEAM1 );				
			}			

			if( m_stEntry[MATCH_TEAM_TYPE_TEAM2].wJoinID != INVALID_WORD )
			{
				pRightData = CBudokaiTournamentGui::FindTeamEntryData( m_stEntry[MATCH_TEAM_TYPE_TEAM2].wJoinID );

				if( !pRightData )
					wRightJoinID = m_stEntry[MATCH_TEAM_TYPE_TEAM2].wJoinID;
				else
					pMatchInfoGui->SetTeamData( pRightData, MATCH_TEAM_TYPE_TEAM2 );					
			}
			
			if( wLeftJoinID != INVALID_WORD || wRightJoinID != INVALID_WORD )
				GetDboGlobal()->GetChatPacketGenerator()->SendBudokaiTournamentTeamInfoReq( wLeftJoinID, wRightJoinID );
		}
		else
		{
			sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA* pLeftData = NULL;
			sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA* pRightData = NULL;
			
			if( m_stEntry[MATCH_TEAM_TYPE_TEAM1].wJoinID != INVALID_WORD )
			{
				pLeftData =	CBudokaiTournamentGui::FindIndividualEntryData( m_stEntry[MATCH_TEAM_TYPE_TEAM1].wJoinID );

				if( !pLeftData )
					wLeftJoinID = m_stEntry[MATCH_TEAM_TYPE_TEAM1].wJoinID;
				else
					pMatchInfoGui->SetIndividualData( pLeftData, MATCH_TEAM_TYPE_TEAM1 );				
			}

			if( m_stEntry[MATCH_TEAM_TYPE_TEAM2].wJoinID != INVALID_WORD )
			{
				pRightData = CBudokaiTournamentGui::FindIndividualEntryData( m_stEntry[MATCH_TEAM_TYPE_TEAM2].wJoinID );

				if( !pRightData )
					wRightJoinID = m_stEntry[MATCH_TEAM_TYPE_TEAM2].wJoinID;
				else
					pMatchInfoGui->SetIndividualData( pRightData, MATCH_TEAM_TYPE_TEAM2 );					
			}

			if( wLeftJoinID != INVALID_WORD || wRightJoinID != INVALID_WORD )
				GetDboGlobal()->GetChatPacketGenerator()->SendBudokaiTournamentIndividualInfoReq( wLeftJoinID, wRightJoinID );
		}
	}
}

VOID CBudokaiTournamentFightInfo::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_pdlgParent->GetScreenRect();
	RwInt32 nOffsetX = rtScreen.left - nOldX;
	RwInt32 nOffsetY = rtScreen.top - nOldY;

	m_surLeftInitVerticalLine.GetSurface()->m_SnapShot.rtRect.Offset( nOffsetX, nOffsetY );
	m_surRightInitVerticalLine.GetSurface()->m_SnapShot.rtRect.Offset( nOffsetX, nOffsetY );
	m_surLeftHorizontalLine.GetSurface()->m_SnapShot.rtRect.Offset( nOffsetX, nOffsetY );
	m_surRightHorizontalLine.GetSurface()->m_SnapShot.rtRect.Offset( nOffsetX, nOffsetY );
	m_surCenterVerticalLine.GetSurface()->m_SnapShot.rtRect.Offset( nOffsetX, nOffsetY );
}

VOID CBudokaiTournamentFightInfo::OnPaint(VOID)
{
	m_surLeftInitVerticalLine.Render();
	m_surRightInitVerticalLine.Render();
	m_surLeftHorizontalLine.Render();
	m_surRightHorizontalLine.Render();
	m_surCenterVerticalLine.Render();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBudokaiTournamentEntryInfoGroup

CBudokaiTournamentEntryGroup::CBudokaiTournamentEntryGroup( RwInt32 nPosOffset, RwInt32 nEntryCount, RwInt32 nOriginX, RwInt32 nOriginY, RwInt32 nOffset, gui::CDialog* pDialog )
{
	m_pDialog = pDialog;

	m_nEntryCount = nEntryCount;
	m_nPosOffset = nPosOffset;
	
	m_pEntry = NTL_NEW CBudokaiTournamentEntry*[nEntryCount];
	for( RwUInt8 i = 0 ; i < nEntryCount ; ++i )
	{
		m_pEntry[i] = NTL_NEW CBudokaiTournamentEntry( m_pDialog, i, nOriginX, nOriginY, BUDOKAI_TOURNAMENT_ENTRY_MARK_WIDTH, BUDOKAI_TOURNAMENT_ENTRY_MARK_HEIGHT, nOffset );
	}	
}

CBudokaiTournamentEntryGroup::~CBudokaiTournamentEntryGroup(VOID)
{
	for( RwInt32 i = 0 ; i < m_nEntryCount ; ++i )
		NTL_DELETE( m_pEntry[i] );

	NTL_ARRAY_DELETE( m_pEntry );
	NTL_DELETE( m_pDialog );
}

VOID CBudokaiTournamentEntryGroup::Show( bool bShow )
{
	m_pDialog->Show( bShow );
}

VOID CBudokaiTournamentEntryGroup::SetData( sVARIABLE_DATA* pData, WORD wEntryTeam )
{
	sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR* pEntryData = reinterpret_cast<sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR*>( pData->Read( wEntryTeam ) );
	WCHAR wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];

	for( RwInt32 i = 0 ; i < m_nEntryCount ; ++i )
	{
		RwUInt16 wTeamNameIndex = pEntryData[m_nPosOffset+i].wTeamName;
		RwUInt16 wJoinID = pEntryData[m_nPosOffset+i].wJoinId;

		memset( wszTeamName, 0, sizeof( wszTeamName ) );
		if( wJoinID != INVALID_WORD )
			memcpy( wszTeamName, pData->Read( wTeamNameIndex ), pData->GetSize( wTeamNameIndex ) );

		m_pEntry[i]->SetEntry( wszTeamName, wJoinID );	
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBudokaiTournamentFightInfoGroup

CBudokaiTournamentFightInfoGroup::CBudokaiTournamentFightInfoGroup( RwInt32 nPosOffset, RwInt32 nEntryCount, RwInt32 nOriginX, RwInt32 nOriginY, RwInt32 nVerticalOffset, RwInt32 nHorizontalOffset, RwInt32 nEndLineY, gui::CDialog* pDialog )
{
	m_pDialog = pDialog;
	m_posOrigin.x = nOriginX;
	m_posOrigin.y = nOriginY;
	m_nVerticalOffset = nVerticalOffset;
	m_nHorizontalOffset = nHorizontalOffset;
	m_nEndLineY = nEndLineY;

	m_nFightInfoCount = nEntryCount - 1;
	m_nPosOffset = nPosOffset;

	m_ppFightInfo = NTL_NEW CBudokaiTournamentFightInfo*[m_nFightInfoCount];	
	for( RwInt32 i = 0 ; i < m_nFightInfoCount ; ++i )
		m_ppFightInfo[i] = NULL;
}

CBudokaiTournamentFightInfoGroup::~CBudokaiTournamentFightInfoGroup(VOID)
{
	for( RwInt32 i = 0 ; i < m_nFightInfoCount ; ++i )
		NTL_DELETE( m_ppFightInfo[i] );

	NTL_ARRAY_DELETE( m_ppFightInfo );
	NTL_DELETE( m_pDialog );
}

VOID CBudokaiTournamentFightInfoGroup::CreateData( RwUInt8 byIndex, CBudokaiTournamentFightInfo* pAboveFightInfo, RwUInt8 byMatchLevel, RwUInt8 byMatchNumber, RwUInt8 byServerMatchLevel, RwUInt8 byServerMatchNumber, RwBool bParty )
{
	NTL_ASSERT( byIndex >= 0 && byIndex < m_nFightInfoCount, "CBudokaiTournamentFightInfoGroup::CreateData : Invalid Index" );
	NTL_ASSERT( m_ppFightInfo[byIndex] == NULL, "CBudokaiTournamentFightInfoGroup::CreateData : No Allowed Complexed Index" );

	m_ppFightInfo[byIndex] = NTL_NEW CBudokaiTournamentFightInfo( pAboveFightInfo, m_pDialog, byMatchNumber, byMatchLevel, byServerMatchNumber, byServerMatchLevel, bParty,
																  m_posOrigin.x, m_posOrigin.y, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_WIDTH, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_HEIGHT, m_nVerticalOffset, m_nHorizontalOffset, m_nEndLineY );																  
}

CBudokaiTournamentFightInfo* CBudokaiTournamentFightInfoGroup::FindFightInfo( RwUInt8 byServerMatchLevel, RwUInt8 byServerMatchNumber )
{
	for( RwInt32 i = 0 ; i < m_nFightInfoCount ; ++i )
	{
		if( m_ppFightInfo[i] )
		{
			if( m_ppFightInfo[i]->GetServerMatchLevel() == byServerMatchLevel &&
				m_ppFightInfo[i]->GetServerMatchNumber() == byServerMatchNumber )				
				return m_ppFightInfo[i];
		}
	}
	
	return NULL;
}

VOID CBudokaiTournamentFightInfoGroup::Update( RwReal fElapsed )
{
	for( RwInt32 i = 0 ; i < m_nFightInfoCount ; ++i )
		m_ppFightInfo[i]->Update( fElapsed );
}

VOID CBudokaiTournamentFightInfoGroup::Show( bool bShow )
{
	m_pDialog->Show( bShow );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBudokaiTournamentGui

CBudokaiTournamentGui::CBudokaiTournamentGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CBudokaiTournamentGui::~CBudokaiTournamentGui(VOID)
{

}

VOID CBudokaiTournamentGui::Init(VOID)
{
	for( RwInt32 i = 0 ; i < NUM_PAGE ; ++i )
	{
		m_pPageDialog[i] = NULL;
		m_pEntryGroup[i] = NULL;
		m_pFightInfoGroup[i] = NULL;
	}

	m_byMaxEntry = 0;

	m_eCurrenctPage = INVALID_PAGE;
	m_byIndividualFightInfoInputDoneCount = INVALID_BYTE;
	m_byTeamFightInfoInputDoneCount = INVALID_BYTE;

	m_pbtnRightArrow = NULL;
	m_pbtnLeftArrow = NULL;
	m_pstbIndividualGroupType = NULL;

	m_pbtnParty = NULL;
	m_pbtnIndividual = NULL;
	m_ppnlLineBack = NULL;
	
	m_pstbTitle = NULL;
	m_pstbInfomation = NULL;
}

RwBool CBudokaiTournamentGui::Create(VOID)
{
	NTL_FUNCTION( "CBudokaiTournamentGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\BudokaiTournament.srf", "gui\\BudokaiTournament.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain" ) );

	m_pbtnRightArrow = reinterpret_cast<gui::CButton*>( GetComponent( "btnRight" ) );
	m_pbtnLeftArrow = reinterpret_cast<gui::CButton*>( GetComponent( "btnLeft" ) );
	m_pstbIndividualGroupType = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbType" ) );
	
	m_pbtnParty = reinterpret_cast<gui::CButton*>( GetComponent( "btnParty" )	);
	m_pbtnIndividual = reinterpret_cast<gui::CButton*>( GetComponent( "btnIndivisual" )	);
	
	m_ppnlLineBack = reinterpret_cast<gui::CPanel*>( GetComponent( "pnlLineBack" ) );
	m_ppnlQuestionMark = reinterpret_cast<gui::CPanel*>( GetComponent( "pnlQuestionMark" ) );

	m_pstbTitle = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbTitle" ) );
	m_pstbInfomation = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbInfo" ) );

	m_pbtnClose = reinterpret_cast<gui::CButton*>( GetComponent( "btnClose" ) );

	// peessitemp:
	m_pstbInfomation->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TOURNAMENT_INFO_1" ) );

	m_pbtnIndividual->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_SOLO" ) );
	m_pbtnParty->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_PARTY" ) );

	// Callback
	m_slotClickClose = m_pbtnClose->SigClicked().Connect( this, &CBudokaiTournamentGui::OnClickClose );
	m_slotClickRightArrow = m_pbtnRightArrow->SigClicked().Connect( this, &CBudokaiTournamentGui::OnClickRightArrow );
	m_slotClickLeftArrow = m_pbtnLeftArrow->SigClicked().Connect( this, &CBudokaiTournamentGui::OnClickLeftArrow );
	m_slotClickPartyBtn = m_pbtnParty->SigToggled().Connect( this, &CBudokaiTournamentGui::OnToggledPartyBtn );
	m_slotClickIndividualBtn = m_pbtnIndividual->SigToggled().Connect( this, &CBudokaiTournamentGui::OnToggledIndividualBtn );

	// 토너먼트 컴포넌트 생성.
	CreateDataGroup();

	Show( false );

	// Event
	LinkMsg( g_EventBudokaiTournamentIndividualList );
	LinkMsg( g_EventBudokaiTournamentIndividualInfo );
	LinkMsg( g_EventBudokaiTournamentTeamList );
	LinkMsg( g_EventBudokaiTournamentTeamInfo );
	LinkMsg( g_EventResize );

	NTL_RETURN( TRUE );
}

VOID CBudokaiTournamentGui::Destroy(VOID)
{
	// 혹시나.
	m_mapIndividualEntryData.clear();
	m_mapTeamEntryData.clear();

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventBudokaiTournamentIndividualList );
	UnLinkMsg( g_EventBudokaiTournamentIndividualInfo );
	UnLinkMsg( g_EventBudokaiTournamentTeamList );
	UnLinkMsg( g_EventBudokaiTournamentTeamInfo );
	UnLinkMsg( g_EventResize );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CBudokaiTournamentGui::Update( RwReal fElapsed )
{
	if( m_pPageDialog[TOURNAMENT_INDIVIDUAL_A]->IsVisible() )
		m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->Update( fElapsed );
	else if( m_pPageDialog[TOURNAMENT_INDIVIDUAL_B]->IsVisible() )
		m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->Update( fElapsed );
	else if( m_pPageDialog[TOURNAMENT_PARTY]->IsVisible() )
		m_pFightInfoGroup[TOURNAMENT_PARTY]->Update( fElapsed );	
}

RwInt32 CBudokaiTournamentGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( bOpen )
	{
		m_pstbTitle->Format( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_TITLE" ), GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetSeasonCount() );
		GetNtlGuiManager()->AddUpdateFunc( this );
		SetPage( TOURNAMENT_INDIVIDUAL_A );
	}
	else
	{
		GetNtlGuiManager()->RemoveUpdateFunc( this );
	}

	return 1;	
}

VOID CBudokaiTournamentGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventBudokaiTournamentIndividualList )
	{
		SDboEventBudokaiTournamentIndividualList* pData = reinterpret_cast<SDboEventBudokaiTournamentIndividualList*>( msg.pData );		

		if( pData->wEntryTeam_var != INVALID_WORD )
		{
			m_pEntryGroup[TOURNAMENT_INDIVIDUAL_A]->SetData( pData->pData, pData->wEntryTeam_var );
			m_pEntryGroup[TOURNAMENT_INDIVIDUAL_B]->SetData( pData->pData, pData->wEntryTeam_var );			
		}

		for( RwInt32 i = 0 ; i < pData->byMatchDataCount ; ++i )
		{
			sBUDOKAI_TOURNAMENT_MATCH_DATA* pMatchData = reinterpret_cast<sBUDOKAI_TOURNAMENT_MATCH_DATA*>( pData->pData->Read( pData->wMatchData_var ) );
			sBUDOKAI_TOURNAMENT_MATCH_DATA* pCurrentMatchData = &pMatchData[i];
			
			CBudokaiTournamentFightInfo* pFightInfo = NULL;
			pFightInfo = m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( pCurrentMatchData->byDepth, pCurrentMatchData->byMatchNumber );
			if( !pFightInfo )	
			{
				pFightInfo = m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( pCurrentMatchData->byDepth, pCurrentMatchData->byMatchNumber );

				if( !pFightInfo )
					continue;
			}
			
			// 맨 밑단에만 일단 ID를 전해준다.
			RwUInt8 byDepth = (RwUInt8)( log( (RwReal)( BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER ) ) / log( 2.0f ) ) - 1;
			if( pCurrentMatchData->byDepth == byDepth )
			{
				pFightInfo->SetEntryData( 0, pCurrentMatchData->wJoinId1 );
				pFightInfo->SetEntryData( 1, pCurrentMatchData->wJoinId2 );
			}
			
			pFightInfo->SetFightResult( pCurrentMatchData->wWinnerTeam, pCurrentMatchData->wJoinId1, pCurrentMatchData->wJoinId2, pCurrentMatchData->byScore1, pCurrentMatchData->byScore2 );
		}

		m_byIndividualFightInfoInputDoneCount = (RwUInt8)(m_byIndividualFightInfoInputDoneCount + pData->byMatchDataCount);		
	}
	else if( msg.Id == g_EventBudokaiTournamentTeamList )
	{
		SDboEventBudokaiTournamentTeamList* pData = reinterpret_cast<SDboEventBudokaiTournamentTeamList*>( msg.pData );

		if( pData->wEntryTeam_var != INVALID_WORD )
		{
			m_pEntryGroup[TOURNAMENT_PARTY]->SetData( pData->pData, pData->wEntryTeam_var );
		}

		for( RwInt32 i = 0 ; i < pData->byMatchDataCount ; ++i )
		{
			sBUDOKAI_TOURNAMENT_MATCH_DATA* pMatchData = reinterpret_cast<sBUDOKAI_TOURNAMENT_MATCH_DATA*>( pData->pData->Read( pData->wMatchData_var ) );
			sBUDOKAI_TOURNAMENT_MATCH_DATA* pCurrentMatchData = &pMatchData[i];

			CBudokaiTournamentFightInfo* pFightInfo = NULL;
			pFightInfo = m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( pCurrentMatchData->byDepth, pCurrentMatchData->byMatchNumber );
			if( !pFightInfo )
				continue;

			// 맨 밑단에만 일단 ID를 전해준다.
			RwUInt8 byDepth = (RwUInt8)( log( (RwReal)( BUDOKAI_MAJOR_MATCH_TEAM_NUMBER ) ) / log( 2.0f ) ) - 1;
			if( pCurrentMatchData->byDepth == byDepth )
			{
				pFightInfo->SetEntryData( 0, pCurrentMatchData->wJoinId1 );
				pFightInfo->SetEntryData( 1, pCurrentMatchData->wJoinId2 );
			}

			pFightInfo->SetFightResult( pCurrentMatchData->wWinnerTeam, pCurrentMatchData->wJoinId1, pCurrentMatchData->wJoinId2, pCurrentMatchData->byScore1, pCurrentMatchData->byScore2 );
		}

		m_byTeamFightInfoInputDoneCount = (RwUInt8)(m_byTeamFightInfoInputDoneCount + pData->byMatchDataCount);
	}
	else if( msg.Id == g_EventBudokaiTournamentIndividualInfo )
	{
		SDboEventBudokaiTournamentIndividualInfo* pData = reinterpret_cast<SDboEventBudokaiTournamentIndividualInfo*>( msg.pData );		
	
		for( RwInt16 i = 0 ; i < 2 ; ++i )
		{
			if( pData->wTeamType[i] == INVALID_WORD )
				continue;

			sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA data;
			memset( &data, 0, sizeof( data ) );

			sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA_VAR* pVarData = reinterpret_cast<sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA_VAR*>( pData->pData->Read( pData->wTeamType[i] ) );
			
			data.wJoinId = pVarData->wJoinId;
			data.byMinorMatch_MatchIndex = pVarData->byMinorMatch_MatchIndex;
			data.charId = pVarData->charId;
			data.byClass = pVarData->byClass;
			data.byLevel = pVarData->byLevel;
			data.fRankPoint = pVarData->fRankPoint;
			data.wRankBattleWinCount = pVarData->wRankBattleWinCount;
			data.wRankBattleLoseCount = pVarData->wRankBattleLoseCount;
			data.wRankBattleDrawCount = pVarData->wRankBattleDrawCount;
			memcpy( data.wszCharName, pData->pData->Read( pVarData->wCharName ), pData->pData->GetSize( pVarData->wCharName ) );
			memcpy( data.wszGuildName, pData->pData->Read( pVarData->wGuildName ), pData->pData->GetSize( pVarData->wGuildName ) );

			m_mapIndividualEntryData[pVarData->wJoinId] = data;

			CTBTournamentMatchInfoGui* pMatchInfoGui = reinterpret_cast<CTBTournamentMatchInfoGui*>( GetDialogManager()->GetDialog( DIALOG_BUDOKAI_TOURNAMENT_MATCHINFO ) );
			pMatchInfoGui->SetIndividualData( &data, i );
		}
	}
	else if( msg.Id == g_EventBudokaiTournamentTeamInfo )
	{
		SDboEventBudokaiTournamentTeamInfo* pData = reinterpret_cast<SDboEventBudokaiTournamentTeamInfo*>( msg.pData );
		
		for( RwInt16 i = 0 ; i < 2 ; ++i )
		{
			if( pData->wTeamType[i] == INVALID_WORD )
				continue;

			sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA data;
			memset( &data, 0, sizeof( data ) );

			sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA_VAR* pVarData = reinterpret_cast<sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA_VAR*>( pData->pData->Read( pData->wTeamType[i] ) );

			data.wJoinId = pVarData->wJoinId;
			data.byMemberCount = pVarData->byMemberCount;
			data.byMinorMatch_MatchIndex = pVarData->byMinorMatch_MatchIndex;			
			memcpy( data.wszTeamName, pData->pData->Read( pVarData->wTeamName ), pData->pData->GetSize( pVarData->wTeamName ) );

			sBUDOKAI_TOURNAMENT_TEAM_ENTRY_MEMBER_DATA_VAR* pMemberVar = reinterpret_cast<sBUDOKAI_TOURNAMENT_TEAM_ENTRY_MEMBER_DATA_VAR*>( pData->pData->Read( pVarData->wMembers ) );
			
			for( RwInt32 j = 0 ; j < pVarData->byMemberCount ; ++j )
			{
				data.aMembers[j].charId = pMemberVar[j].charId;
				data.aMembers[j].byClass = pMemberVar[j].byClass;
				data.aMembers[j].byLevel = pMemberVar[j].byLevel;
				data.aMembers[j].fRankPoint = pMemberVar[j].fRankPoint;
				memcpy( data.aMembers[j].wszCharName, pData->pData->Read( pMemberVar[j].wCharName ), pData->pData->GetSize( pMemberVar[j].wCharName ) );
			}

			m_mapTeamEntryData[pVarData->wJoinId] = data;

			CTBTournamentMatchInfoGui* pMatchInfoGui = reinterpret_cast<CTBTournamentMatchInfoGui*>( GetDialogManager()->GetDialog( DIALOG_BUDOKAI_TOURNAMENT_MATCHINFO ) );
			pMatchInfoGui->SetTeamData( &data, i );
		}
	}
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		SetPosition( (RwInt32)( ( pData->iWidht - GetWidth() ) / 2 ), (RwInt32)( ( pData->iHeight - GetHeight() ) / 2 ) );
	}
}

VOID CBudokaiTournamentGui::CreateDataGroup(VOID)
{
	for( RwInt32 i = 0 ; i < NUM_PAGE ; ++i )
	{
		m_pPageDialog[i] = NTL_NEW gui::CDialog( CRectangle( 0, 0, m_pThis->GetWidth(), m_pThis->GetHeight() ), m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
		m_pPageDialog[i]->SetPriority( 0x7000 );	// 낮은 Priority		
	}	

	m_pEntryGroup[TOURNAMENT_INDIVIDUAL_A] = NTL_NEW CBudokaiTournamentEntryGroup( 0, BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER / 2, BUDOKAI_TOURNAMENT_ENTRY_MARK_1_LEFT, BUDOKAI_TOURNAMENT_ENTRY_MARK_1_TOP, BUDOKAI_TOURNAMENT_ENTRY_MARK_OFFSET, m_pPageDialog[TOURNAMENT_INDIVIDUAL_A] );
	m_pEntryGroup[TOURNAMENT_INDIVIDUAL_B] = NTL_NEW CBudokaiTournamentEntryGroup( BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER / 2, BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER / 2, BUDOKAI_TOURNAMENT_ENTRY_MARK_1_LEFT, BUDOKAI_TOURNAMENT_ENTRY_MARK_1_TOP, BUDOKAI_TOURNAMENT_ENTRY_MARK_OFFSET, m_pPageDialog[TOURNAMENT_INDIVIDUAL_B] );
	m_pEntryGroup[TOURNAMENT_PARTY] = NTL_NEW CBudokaiTournamentEntryGroup( 0, BUDOKAI_MAJOR_MATCH_TEAM_NUMBER, BUDOKAI_TOURNAMENT_ENTRY_MARK_1_LEFT, BUDOKAI_TOURNAMENT_ENTRY_MARK_1_TOP, BUDOKAI_TOURNAMENT_ENTRY_MARK_OFFSET, m_pPageDialog[TOURNAMENT_PARTY] );

	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A] = NTL_NEW CBudokaiTournamentFightInfoGroup( 0, BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER / 2, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_ORIGIN_LEFT, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_ORIGIN_TOP, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_VERTICAL_OFFSET, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_HORIZONTAL_OFFSET, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_END_Y, m_pPageDialog[TOURNAMENT_INDIVIDUAL_A] );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B] = NTL_NEW CBudokaiTournamentFightInfoGroup( BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER / 2, BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER / 2, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_ORIGIN_LEFT, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_ORIGIN_TOP, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_VERTICAL_OFFSET, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_HORIZONTAL_OFFSET, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_END_Y, m_pPageDialog[TOURNAMENT_INDIVIDUAL_B] );
	m_pFightInfoGroup[TOURNAMENT_PARTY] = NTL_NEW CBudokaiTournamentFightInfoGroup( 0, BUDOKAI_MAJOR_MATCH_TEAM_NUMBER, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_ORIGIN_LEFT, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_ORIGIN_TOP, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_VERTICAL_OFFSET, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_HORIZONTAL_OFFSET, BUDOKAI_TOURNAMENT_ENTRY_FIGHTINFO_LINE_END_Y, m_pPageDialog[TOURNAMENT_PARTY] );

	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData( 0, NULL, 3, 0, 1, 0, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData( 1, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 1, 0 ), 2, 0, 2, 0, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData( 2, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 1, 0 ), 2, 1, 2, 1, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData( 3, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 2, 0 ), 1, 0, 3, 0, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData( 4, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 2, 0 ), 1, 1, 3, 1, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData( 5, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 2, 1 ), 1, 2, 3, 2, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData( 6, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 2, 1 ), 1, 3, 3, 3, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData( 7, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 0 ), 0, 0, 4, 0, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData( 8, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 0 ), 0, 1, 4, 1, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData( 9, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 1 ), 0, 2, 4, 2, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData(10, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 1 ), 0, 3, 4, 3, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData(11, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 2 ), 0, 4, 4, 4, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData(12, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 2 ), 0, 5, 4, 5, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData(13, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 3 ), 0, 6, 4, 6, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateData(14, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 3 ), 0, 7, 4, 7, FALSE );

	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData( 0, NULL, 3, 0, 1, 1, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData( 1, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 1, 1 ), 2, 0, 2, 2, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData( 2, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 1, 1 ), 2, 1, 2, 3, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData( 3, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 2, 2 ), 1, 0, 3, 4, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData( 4, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 2, 2 ), 1, 1, 3, 5, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData( 5, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 2, 3 ), 1, 2, 3, 6, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData( 6, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 2, 3 ), 1, 3, 3, 7, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData( 7, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 4 ), 0, 0, 4, 8, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData( 8, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 4 ), 0, 1, 4, 9, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData( 9, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 5 ), 0, 2, 4,10, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData(10, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 5 ), 0, 3, 4,11, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData(11, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 6 ), 0, 4, 4,12, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData(12, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 6 ), 0, 5, 4,13, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData(13, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 7 ), 0, 6, 4,14, FALSE );
	m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateData(14, m_pFightInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 7 ), 0, 7, 4,15, FALSE );

	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData( 0, NULL, 3, 0, 0, 0, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData( 1, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 0, 0 ), 2, 0, 1, 0, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData( 2, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 0, 0 ), 2, 1, 1, 1, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData( 3, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 1, 0 ), 1, 0, 2, 0, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData( 4, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 1, 0 ), 1, 1, 2, 1, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData( 5, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 1, 1 ), 1, 2, 2, 2, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData( 6, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 1, 1 ), 1, 3, 2, 3, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData( 7, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 0 ), 0, 0, 3, 0, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData( 8, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 0 ), 0, 1, 3, 1, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData( 9, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 1 ), 0, 2, 3, 2, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData(10, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 1 ), 0, 3, 3, 3, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData(11, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 2 ), 0, 4, 3, 4, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData(12, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 2 ), 0, 5, 3, 5, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData(13, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 3 ), 0, 6, 3, 6, TRUE );
	m_pFightInfoGroup[TOURNAMENT_PARTY]->CreateData(14, m_pFightInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 3 ), 0, 7, 3, 7, TRUE );
}

VOID CBudokaiTournamentGui::SetPage( ePage ePageType )
{
	m_pPageDialog[TOURNAMENT_PARTY]->Show( false );
	m_pPageDialog[TOURNAMENT_INDIVIDUAL_A]->Show( false );
	m_pPageDialog[TOURNAMENT_INDIVIDUAL_B]->Show( false );

	if( ePageType == TOURNAMENT_PARTY )
	{
		m_pbtnRightArrow->Show( false );
		m_pbtnLeftArrow->Show( false );
		m_pstbIndividualGroupType->Show( false );		
		m_pPageDialog[TOURNAMENT_PARTY]->Show( true );

		// 버튼 세팅
		m_pbtnParty->ClickEnable( FALSE );
		m_pbtnIndividual->ClickEnable( TRUE );
		m_pbtnIndividual->SetDown( false );	

		GetDboGlobal()->GetChatPacketGenerator()->SendBudokaiTournamentTeamListReq( m_byTeamFightInfoInputDoneCount );		
	}
	else if( ePageType == TOURNAMENT_INDIVIDUAL_A || ePageType == TOURNAMENT_INDIVIDUAL_B )
	{
		m_pbtnRightArrow->Show( true );
		m_pbtnLeftArrow->Show( true );
		m_pstbIndividualGroupType->Show( true );

		if( ePageType == TOURNAMENT_INDIVIDUAL_A )
		{
			m_pstbIndividualGroupType->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TOURNAMENT_A_GROUP" ) );
			m_pPageDialog[TOURNAMENT_INDIVIDUAL_A]->Show( true );
		}
		else // TOURNAMENT_INDIVIDUAL_B
		{
			m_pstbIndividualGroupType->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TOURNAMENT_B_GROUP" ) );
			m_pPageDialog[TOURNAMENT_INDIVIDUAL_B]->Show( true );
		}

		// 버튼 세팅
		m_pbtnIndividual->ClickEnable( FALSE );
		m_pbtnParty->ClickEnable( TRUE );
		m_pbtnParty->SetDown( false );

		GetDboGlobal()->GetChatPacketGenerator()->SendBudokaiTournamentIndividualListReq( m_byIndividualFightInfoInputDoneCount );
	}

	m_eCurrenctPage = ePageType;
}

VOID CBudokaiTournamentGui::OnClickRightArrow( gui::CComponent* pComponent )
{
	if( m_eCurrenctPage == TOURNAMENT_INDIVIDUAL_A )
		SetPage( TOURNAMENT_INDIVIDUAL_B );
	else
		SetPage( TOURNAMENT_INDIVIDUAL_A );
}

VOID CBudokaiTournamentGui::OnClickLeftArrow( gui::CComponent* pComponent )
{
	if( m_eCurrenctPage == TOURNAMENT_INDIVIDUAL_A )
		SetPage( TOURNAMENT_INDIVIDUAL_B );
	else
		SetPage( TOURNAMENT_INDIVIDUAL_A );
}

VOID CBudokaiTournamentGui::OnToggledPartyBtn( gui::CComponent* pComponent, bool bToggleDown )
{
	if( bToggleDown )
	{
		SetPage( TOURNAMENT_PARTY );
	}
}

VOID CBudokaiTournamentGui::OnToggledIndividualBtn( gui::CComponent* pComponent, bool bToggleDown )
{
	if( bToggleDown )
	{
		SetPage( TOURNAMENT_INDIVIDUAL_A );
	}	
}

VOID CBudokaiTournamentGui::OnClickClose( gui::CComponent* pComponent )
{
	GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_TOURNAMENT );
}

sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA* CBudokaiTournamentGui::FindIndividualEntryData( RwUInt16 wJoinID )
{
	std::map<RwUInt16,sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA>::iterator iter;
	sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA* pData = NULL;

	iter = m_mapIndividualEntryData.find( wJoinID );
	if( iter != m_mapIndividualEntryData.end() )
		pData = &( iter->second );

	return pData;
}

sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* CBudokaiTournamentGui::FindTeamEntryData( RwUInt16 wJoinID )
{
	std::map<RwUInt16,sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA>::iterator iter;
	sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* pData = NULL;

	iter = m_mapTeamEntryData.find( wJoinID );
	if( iter != m_mapTeamEntryData.end() )
		pData = &( iter->second );

	return pData;
}