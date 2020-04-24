#include "precomp_dboclient.h"
#include "TBTournamentMatchInfoGui.h"

// shared


// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// client
#include "DboEvent.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"

#define TEAM_INFO_ITEM_X		12
#define TEAM_INFO_ITEM_Y		143
#define TEAM_INFO_ITEM_Y2		163
#define TEAM_INFO_ITEM_Y_OFFSET	41
#define TEAM_INFO_ITEM_WIDTH	124
#define TEAM_INFO_ITEM_HEIGHT	18

CTBTournamentMatchInfoGui::CTBTournamentMatchInfoGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	
}

CTBTournamentMatchInfoGui::~CTBTournamentMatchInfoGui(VOID)
{

}

RwBool CTBTournamentMatchInfoGui::Create(VOID)
{
	NTL_FUNCTION( "CTBTournamentMatchInfoGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\BudokaiTournamentMatchInfo.srf", "gui\\BudokaiTournamentMatchInfo.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pThis->SetPriority(dDIALOGPRIORITY_TB_TOURNEMENT);

	m_pstbTitle = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbTitle" ) );
	m_pstbMatch = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbMatch" ) );
	m_pbtnClose = reinterpret_cast<gui::CButton*>( GetComponent( "btnClose" ) );

	RwChar buf[256];
	CRectangle rect;
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < 2 ; ++i )
	{
		sprintf_s( buf, 256, "stbTeam%dName", i + 1 );
		m_stTeam[i].m_pstbName = reinterpret_cast<gui::CStaticBox*>( GetComponent( buf ) );

		sprintf_s( buf, 256, "pnlDojoMark%d", i + 1 );
		m_stTeam[i].m_ppnlDojoEmblem = reinterpret_cast<gui::CStaticBox*>( GetComponent( buf ) );

		m_stTeam[i].m_numScore.Create( ND_RIGHT );
		m_stTeam[i].m_numScore.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_0" ) );
		m_stTeam[i].m_numScore.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_1" ) );
		m_stTeam[i].m_numScore.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_2" ) );
		m_stTeam[i].m_numScore.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_3" ) );
		m_stTeam[i].m_numScore.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_4" ) );
		m_stTeam[i].m_numScore.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_5" ) );
		m_stTeam[i].m_numScore.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_6" ) );
		m_stTeam[i].m_numScore.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_7" ) );
		m_stTeam[i].m_numScore.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_8" ) );
		m_stTeam[i].m_numScore.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_9" ) );
		m_stTeam[i].m_numScore.SetParentPos( 25 + i * 230, 104 );
		m_stTeam[i].m_numScore.SetPosition( rtScreen.left, rtScreen.top );
		m_stTeam[i].m_numScore.SetRate( 0.8f );

		RwUInt32 dwCompType, uiWidth;
		if( !i )
		{
			dwCompType = COMP_TEXT_LEFT;
			uiWidth = TEAM_INFO_ITEM_X;
		}
		else
		{
			dwCompType = COMP_TEXT_RIGHT;
			uiWidth = GetWidth() - TEAM_INFO_ITEM_WIDTH - TEAM_INFO_ITEM_X;
		}
				
		for( RwInt32 j = 0 ; j < NUM_INFO ; ++j )
		{
			m_stTeam[i].m_pstbLine1[j] = NTL_NEW gui::CStaticBox( m_pThis, GetNtlGuiManager()->GetSurfaceManager(), dwCompType );
			rect.SetRectWH( uiWidth, TEAM_INFO_ITEM_Y + ( TEAM_INFO_ITEM_Y_OFFSET * j ), TEAM_INFO_ITEM_WIDTH, TEAM_INFO_ITEM_HEIGHT );
			m_stTeam[i].m_pstbLine1[j]->SetPosition( rect );
			m_stTeam[i].m_pstbLine1[j]->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );

			m_stTeam[i].m_pstbLine2[j] = NTL_NEW gui::CStaticBox( m_pThis, GetNtlGuiManager()->GetSurfaceManager(), dwCompType );
			rect.SetRectWH( uiWidth, TEAM_INFO_ITEM_Y2 + ( TEAM_INFO_ITEM_Y_OFFSET * j ), TEAM_INFO_ITEM_WIDTH, TEAM_INFO_ITEM_HEIGHT );
			m_stTeam[i].m_pstbLine2[j]->SetPosition( rect );
			m_stTeam[i].m_pstbLine2[j]->SetTextColor( RANKBATTLE_COLOR_YELLOW );			
			m_stTeam[i].m_pstbLine2[j]->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
		}		
	}
	
	m_slotClickClose = m_pbtnClose->SigClicked().Connect( this, &CTBTournamentMatchInfoGui::OnClickClose );
	m_slotMove = m_pThis->SigMove().Connect( this, &CTBTournamentMatchInfoGui::OnMove );
	m_slotPaint = m_pThis->SigPaint().Connect( this, &CTBTournamentMatchInfoGui::OnPaint );

	// Initial Setting
	m_pThis->Show( false );

	// Event¿¬°á
	LinkMsg( g_EventResize );
	
	
	NTL_RETURN( TRUE );
}

VOID CTBTournamentMatchInfoGui::Destroy(VOID)
{
	UnLinkMsg( g_EventResize );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CTBTournamentMatchInfoGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( bOpen )
	{
		gui::CGUIManager* pGuiManager = GetNtlGuiManager()->GetGuiManager();
		SetCenterPosition( pGuiManager->GetWidth(), pGuiManager->GetHeight() );
	}

	return 1;
}

VOID CTBTournamentMatchInfoGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		SetCenterPosition( pData->iWidht, pData->iHeight );
	}
}

VOID CTBTournamentMatchInfoGui::SetIndividualData( sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA* pData, RwUInt16 wTeamType )
{
	m_stTeam[wTeamType].m_pstbLine1[CLASS]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_CLASS" ) );	
	m_stTeam[wTeamType].m_pstbLine1[LEVEL]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_LEVEL" ) );	
	m_stTeam[wTeamType].m_pstbLine1[BATTLESCORE]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_RECORD" ) );	
	m_stTeam[wTeamType].m_pstbLine1[RANKPOINT]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_RANKING_POINT" ) );	
	m_stTeam[wTeamType].m_pstbLine1[GUILDNAME]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_GUILD_NAME" ) );	

	if( !pData )
		return;

	m_stTeam[wTeamType].m_pstbName->SetText( pData->wszCharName );
	m_stTeam[wTeamType].m_pstbLine2[CLASS]->SetText( Logic_GetClassName( pData->byClass ) );
	m_stTeam[wTeamType].m_pstbLine2[LEVEL]->SetText( pData->byLevel );
	m_stTeam[wTeamType].m_pstbLine2[BATTLESCORE]->Format( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_RECORD_DATA" ), 
														  pData->wRankBattleDrawCount + pData->wRankBattleLoseCount + pData->wRankBattleWinCount,
														  pData->wRankBattleWinCount, pData->wRankBattleLoseCount );
	m_stTeam[wTeamType].m_pstbLine2[RANKPOINT]->SetText( (RwInt32)pData->fRankPoint );
	m_stTeam[wTeamType].m_pstbLine2[GUILDNAME]->SetText( pData->wszGuildName );

	if( pData->bDojoRecommender )
	{
		m_stTeam[wTeamType].m_ppnlDojoEmblem->Show( true );
		m_stTeam[wTeamType].m_ppnlDojoEmblem->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_DOJO_RECOMMAND" ) );
	}
	else
		m_stTeam[wTeamType].m_ppnlDojoEmblem->Show( false );
}

VOID CTBTournamentMatchInfoGui::SetTeamData( sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* pData, RwUInt16 wTeamType )
{
	if( !pData )
		return;

	m_stTeam[wTeamType].m_pstbName->SetText( pData->wszTeamName );
	m_stTeam[wTeamType].m_ppnlDojoEmblem->Show( false );

	for( RwInt32 i = 0 ; i < pData->byMemberCount ; ++i )
	{
		m_stTeam[wTeamType].m_pstbLine1[i]->SetText( pData->aMembers[i].wszCharName );
		m_stTeam[wTeamType].m_pstbLine2[i]->Format( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_LEVEL_CLASS" ),
													pData->aMembers[i].byLevel, Logic_GetClassName( pData->aMembers[i].byClass ) );
	}
}

VOID CTBTournamentMatchInfoGui::SetMatchData( RwUInt8 byDepth, RwUInt8 byIndex, RwUInt8 byLeftScore, RwUInt8 byRightScore )
{
	if( byDepth == BUDOKAI_MATCH_DEPTH_2 )
	{
		m_pstbMatch->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_MATCH_NAME_FINAL" ) );
	}
	else if( byDepth == BUDOKAI_MATCH_DEPTH_4 )
	{
		m_pstbMatch->Format( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_MATCH_NAME_SEMIFINAL" ), byIndex + 1 );
	}
	else if( byDepth != INVALID_BYTE )
	{
		m_pstbMatch->Format( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_MATCH_NAME_COMMON" ), (RwInt32)pow( (RwReal)2, byDepth ) * 2, byIndex + 1 );
	}
	else
	{
		m_pstbMatch->Clear();
	}

	if( byLeftScore == INVALID_BYTE )
	{
		m_stTeam[MATCH_TEAM_TYPE_TEAM1].m_numScore.Show( FALSE );
	}
	else
	{
		m_stTeam[MATCH_TEAM_TYPE_TEAM1].m_numScore.SetNumber( byLeftScore );
		m_stTeam[MATCH_TEAM_TYPE_TEAM1].m_numScore.Show( TRUE );
	}

	if( byRightScore == INVALID_BYTE )
	{
		m_stTeam[MATCH_TEAM_TYPE_TEAM2].m_numScore.Show( FALSE );
	}
	else
	{
		m_stTeam[MATCH_TEAM_TYPE_TEAM2].m_numScore.SetNumber( byRightScore );
		m_stTeam[MATCH_TEAM_TYPE_TEAM2].m_numScore.Show( TRUE );
	}

	ClearData( MATCH_TEAM_TYPE_TEAM1 );
	ClearData( MATCH_TEAM_TYPE_TEAM2 );
}

VOID CTBTournamentMatchInfoGui::SetCenterPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	SetPosition( (RwInt32)( ( nScreenWidth - GetWidth() ) / 2 ), (RwInt32)( ( nScreenHeight - GetHeight() ) / 2 ) );
}

VOID CTBTournamentMatchInfoGui::ClearData( RwUInt16 wTeamType )
{
	m_stTeam[wTeamType].m_pstbName->Clear();
	m_stTeam[wTeamType].m_ppnlDojoEmblem->Show( false );
	
	for( RwInt32 i = 0 ; i < NUM_INFO ; ++i )
	{
		m_stTeam[wTeamType].m_pstbLine1[i]->Clear();
		m_stTeam[wTeamType].m_pstbLine2[i]->Clear();		
	}
}

VOID CTBTournamentMatchInfoGui::OnClickClose( gui::CComponent* pComponent )
{
	GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_TOURNAMENT_MATCHINFO );
}

VOID CTBTournamentMatchInfoGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	
	for( RwInt32 i = 0 ; i < 2 ; ++i )
	{
		m_stTeam[i].m_numScore.SetPosition( rtScreen.left, rtScreen.top );
	}
}

VOID CTBTournamentMatchInfoGui::OnPaint(VOID)
{
	for( RwInt32 i = 0 ; i < 2 ; ++i )
	{
		m_stTeam[i].m_numScore.Render();
	}
}
