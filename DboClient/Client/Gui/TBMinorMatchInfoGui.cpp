#include "precomp_dboclient.h"
#include "TBMinorMatchInfoGui.h"

// Shared
#include "PCTable.h"

// Presentation
#include "NtlPLGuiManager.h"

// sound
#include "GUISoundDefine.h"

// Simulation
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSLRankBattle.h"
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"

// Dbo Client
#include "SideDialogManager.h"
#include "DboLogic.h"
#include "DisplayStringManager.h"
#include "DboEventGenerator.h"

#define TB_MINOR_ENEMY_PARTYINFO_CLASSICON_X	-24		// 각 파티인포아이템 기준 좌표.
#define TB_MINOR_ENEMY_PARTYINFO_CLASSICON_Y	0	

#define TB_MINOR_BATTLEINFO_ALIVESCORE_X	38
#define TB_MINOR_BATTLEINFO_KILLSCORE_X		127
#define TB_MINOR_BATTLEINFO_SCORE_Y			68

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TBMinorMatchInfoItem

CTBMinorMatchEnemyInfoItem::CTBMinorMatchEnemyInfoItem(VOID)
{
	
}

CTBMinorMatchEnemyInfoItem::~CTBMinorMatchEnemyInfoItem(VOID)
{
	
}

VOID CTBMinorMatchEnemyInfoItem::Clear(VOID)
{
	m_pName->Clear();
	m_pKillCount->Clear();
	m_pTeamMember->SetRange( 0, NTL_MAX_MEMBER_IN_PARTY );
	m_pTeamMember->SetPos( 0 );
	m_pKilledTeamMember->SetRange( 0, NTL_MAX_MEMBER_IN_PARTY );
	m_pKilledTeamMember->SetPos( 0 );
}

VOID CTBMinorMatchEnemyInfoItem::SetUI( gui::CStaticBox* pName, gui::CStaticBox* pKillCount, gui::CProgressBar* pTeamMember, gui::CProgressBar* pKilledTeamMember )
{
	m_pName = pName;	
	m_pKillCount = pKillCount;
	m_pTeamMember = pTeamMember;
	m_pKilledTeamMember = pKilledTeamMember;

	Clear();
}

VOID CTBMinorMatchEnemyInfoItem::SetData( RwUInt16 wTeamType, const WCHAR* szName, RwInt8 byKillCount )
{
	m_pName->SetText( szName );
	SetMemberState( wTeamType );
	SetKillCount( byKillCount );

	m_wTeamType = wTeamType;
}

VOID CTBMinorMatchEnemyInfoItem::SetKillCount( RwUInt8 byKillCount )
{
	m_pKillCount->SetText( byKillCount );
}

VOID CTBMinorMatchEnemyInfoItem::SetMemberState( RwUInt16 wTeamType )
{
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

	stTBudokaiTeam* pBudokaiTeam = pTBWorldConcept->FindTeam( wTeamType );
	std::vector<stTBudokaiMember*>::iterator iter;

	RwInt32 nTeamCount = 0;
	RwInt32 nKilledCount = 0;	

	if( !pBudokaiTeam )
		return;
	
	for( iter = pBudokaiTeam->vecpMember.begin() ; iter != pBudokaiTeam->vecpMember.end() ; ++iter )
	{
		stTBudokaiMember* pBudokaiMember = (*iter);

		switch( pBudokaiMember->byState )
		{
		case MATCH_MEMBER_STATE_NORMAL: 
			++nTeamCount; 
			break;
		case MATCH_MEMBER_STATE_FAINT:
		case MATCH_MEMBER_STATE_OUTOFAREA:
		case MATCH_MEMBER_STATE_RESCUE:
		case MATCH_MEMBER_STATE_GIVEUP:
			++nTeamCount;
			++nKilledCount;
			break;
		}				
	}

	m_pTeamMember->SetPos( nTeamCount );
	m_pKilledTeamMember->SetPos( nKilledCount );

	if( nTeamCount - nKilledCount <= 0 )
	{
		m_pName->SetTextColor( INFOCOLOR_5, TRUE );
		m_pKillCount->SetTextColor( INFOCOLOR_5, TRUE );
		m_pTeamMember->SetAlpha( 128 );
		m_pKilledTeamMember->SetAlpha( 128 );
	}
	else
	{
		m_pName->SetTextColor( INFOCOLOR_0, TRUE );
		m_pKillCount->SetTextColor( INFOCOLOR_0, TRUE );
		m_pTeamMember->SetAlpha( 255 );
		m_pKilledTeamMember->SetAlpha( 255 );
	}
}

VOID CTBMinorMatchEnemyInfoItem::Show( RwBool bShow )
{
	if( bShow )
	{
		m_pName->Show( true );		
		m_pKillCount->Show( true );
		m_pTeamMember->Show( true );
		m_pKilledTeamMember->Show( true );

		
	}
	else
	{
		m_pName->Show( false );	
		m_pKillCount->Show( false );
		m_pTeamMember->Show( false );
		m_pKilledTeamMember->Show( false );
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBMinorMatchInfoGui

CTBMinorMatchInfoGui::CTBMinorMatchInfoGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	m_byInfoType = INFO_ENTRANCE;
}

CTBMinorMatchInfoGui::~CTBMinorMatchInfoGui(VOID)
{

}

RwBool CTBMinorMatchInfoGui::Create(VOID)
{
	NTL_FUNCTION( "CTBMinorMatchInfoGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\BudokaiMatchInfo.srf", "gui\\BudokaiMinorMatchInfo.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	// RBEnemyInfo
	gui::CStaticBox* pName = NULL;
	gui::CStaticBox* pKillCount = NULL;
	gui::CProgressBar* pTeamMember = NULL;
	gui::CProgressBar* pKilledMember = NULL;
		
	pName = (gui::CStaticBox*)GetComponent( "stbName1" );
	pKillCount = (gui::CStaticBox*)GetComponent( "stbKillCount1" );
	pTeamMember = (gui::CProgressBar*)GetComponent( "pgbTeamMember1" );
	pKilledMember = (gui::CProgressBar*)GetComponent( "pgbKilledTeamMember1" );
	m_EnemyInfoArray[0].SetUI( pName, pKillCount, pTeamMember, pKilledMember );
	m_EnemyInfoArray[0].Show( FALSE );

	pName = (gui::CStaticBox*)GetComponent( "stbName2" );
	pKillCount = (gui::CStaticBox*)GetComponent( "stbKillCount2" );
	pTeamMember = (gui::CProgressBar*)GetComponent( "pgbTeamMember2" );
	pKilledMember = (gui::CProgressBar*)GetComponent( "pgbKilledTeamMember2" );
	m_EnemyInfoArray[1].SetUI( pName, pKillCount, pTeamMember, pKilledMember );
	m_EnemyInfoArray[1].Show( FALSE );

	pName = (gui::CStaticBox*)GetComponent( "stbName3" );
	pKillCount = (gui::CStaticBox*)GetComponent( "stbKillCount3" );
	pTeamMember = (gui::CProgressBar*)GetComponent( "pgbTeamMember3" );
	pKilledMember = (gui::CProgressBar*)GetComponent( "pgbKilledTeamMember3" );
	m_EnemyInfoArray[2].SetUI( pName, pKillCount, pTeamMember, pKilledMember );
	m_EnemyInfoArray[2].Show( FALSE );

	pName = (gui::CStaticBox*)GetComponent( "stbName4" );
	pKillCount = (gui::CStaticBox*)GetComponent( "stbKillCount4" );
	pTeamMember = (gui::CProgressBar*)GetComponent( "pgbTeamMember4" );
	pKilledMember = (gui::CProgressBar*)GetComponent( "pgbKilledTeamMember4" );
	m_EnemyInfoArray[3].SetUI( pName, pKillCount, pTeamMember, pKilledMember );
	m_EnemyInfoArray[3].Show( FALSE );

	pName = (gui::CStaticBox*)GetComponent( "stbName5" );
	pKillCount = (gui::CStaticBox*)GetComponent( "stbKillCount5" );
	pTeamMember = (gui::CProgressBar*)GetComponent( "pgbTeamMember5" );
	pKilledMember = (gui::CProgressBar*)GetComponent( "pgbKilledTeamMember5" );
	m_EnemyInfoArray[4].SetUI( pName, pKillCount, pTeamMember, pKilledMember );
	m_EnemyInfoArray[4].Show( FALSE );

	// TBMinorMatchInfo
	m_pstbNameTitle = (gui::CStaticBox*)GetComponent( "stbEnemyMemberTitle" );
	m_pstbKOTitle = (gui::CStaticBox*)GetComponent( "stbKOCountTitle" );
	m_pstbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pbtnMinimize = (gui::CButton*)GetComponent( "btnMinimize" );

	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_MINORMATCH_INFO_TITLE" ) );
	m_pstbNameTitle->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_MINORMATCH_ENEMY_NAME_TITLE" ) );
	m_pstbKOTitle->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_MINORMATCH_KOCOUNTER_TITLE" ) );

	// TBMinor Observer
	m_pPnlSurvivedEnemy = (gui::CPanel*)GetComponent( "pnlSurvivedEnemy" );
	m_pPnlKilledEnemy	= (gui::CPanel*)GetComponent( "pnlKilledEnemy" );
	m_pStbSurvivedPlayer= (gui::CStaticBox*)GetComponent( "stbSurvivedPlayer" );
	m_pStbAllPlayer		= (gui::CStaticBox*)GetComponent( "stbAllPlayer" );

	m_pStbSurvivedPlayer->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_MINORMATCH_ALIVE_TITLE" ) );
	m_pStbAllPlayer->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_MINORMATCH_TOTAL_TITLE" ) );
	
	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_numAliveCount.Create( ND_CENTER );
	m_numAliveCount.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_0" ) );
	m_numAliveCount.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_1" ) );
	m_numAliveCount.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_2" ) );
	m_numAliveCount.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_3" ) );
	m_numAliveCount.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_4" ) );
	m_numAliveCount.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_5" ) );
	m_numAliveCount.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_6" ) );
	m_numAliveCount.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_7" ) );
	m_numAliveCount.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_8" ) );
	m_numAliveCount.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_9" ) );
	m_numAliveCount.SetParentPos( TB_MINOR_BATTLEINFO_ALIVESCORE_X, TB_MINOR_BATTLEINFO_SCORE_Y );
	m_numAliveCount.SetPosition( rtScreen.left, rtScreen.top );
	m_numKillCount.Create( ND_CENTER );
	m_numKillCount.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_0" ) );
	m_numKillCount.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_1" ) );
	m_numKillCount.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_2" ) );
	m_numKillCount.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_3" ) );
	m_numKillCount.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_4" ) );
	m_numKillCount.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_5" ) );
	m_numKillCount.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_6" ) );
	m_numKillCount.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_7" ) );
	m_numKillCount.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_8" ) );
	m_numKillCount.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_9" ) );
	m_numKillCount.SetParentPos( TB_MINOR_BATTLEINFO_KILLSCORE_X, TB_MINOR_BATTLEINFO_SCORE_Y );
	m_numKillCount.SetPosition( rtScreen.left, rtScreen.top );
	
	m_slotMove = m_pThis->SigMove().Connect( this, &CTBMinorMatchInfoGui::OnMove );
	m_slotPaint = m_pThis->SigPaint().Connect( this, &CTBMinorMatchInfoGui::OnPaint );
	m_slotClickMinimize = m_pbtnMinimize->SigClicked().Connect( this, &CTBMinorMatchInfoGui::OnClickMinimize );

	Show( false );

	LinkMsg( g_EventMinorMatchUpdateScore, NULL, 0x7000 );
	LinkMsg( g_EventMinorMatchPlayerState, NULL, 0x7000 );
	LinkMsg( g_EventMinorMatchStateUpdate, NULL, 0x7000 );
	LinkMsg( g_EventMinorMatchTeamInfo );
	
	NTL_RETURN( TRUE );
}

VOID CTBMinorMatchInfoGui::Destroy(VOID)
{
	UnLinkMsg( g_EventMinorMatchUpdateScore );
	UnLinkMsg( g_EventMinorMatchPlayerState );
	UnLinkMsg( g_EventMinorMatchStateUpdate );
	UnLinkMsg( g_EventMinorMatchTeamInfo );
	
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CTBMinorMatchInfoGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );	

		if( pTBWorldConcept->GetMatchState() == BUDOKAI_MINORMATCH_STATE_STAGE_READY ||
			pTBWorldConcept->GetMatchState() == BUDOKAI_MINORMATCH_STATE_STAGE_RUN ||
			pTBWorldConcept->GetMatchState() == BUDOKAI_MINORMATCH_STATE_STAGE_FINISH )
		{
			Show( true );
		}
		else
			return -1;
	}
	else
	{
		Show( false );
	}

	return 1;
}

VOID CTBMinorMatchInfoGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventMinorMatchPlayerState )
	{
		if( IsEntrance() )
			HandleEventEntranceMinorMatchPlayerState( msg );
		else
			HandleEventObserverMinorMatchPlayerState( msg );	
	}
	else if( msg.Id == g_EventMinorMatchUpdateScore )
	{
		// 참가자인지 관전자인지에 따라서 다른 UI Update 방식을 사용한다.
		if( IsEntrance() )
			HandleEventEntranceUpdateScore( msg );
		else
			HandleEventObserverUpdateScore( msg );
	}
	else if( msg.Id == g_EventMinorMatchStateUpdate )
	{
		if( IsEntrance() )
			HandleEventEntranceMatchStateUpdate( msg );
		else
			HandleEventObserverMatchStateUpdate( msg );
	}	
	else if( msg.Id == g_EventMinorMatchTeamInfo )
	{
		// 참가자인지 관전자인지 확인한다.
		UpdateInfoType();
		SetInfoFrame();
		SetInfoOnce();
	}
}

VOID CTBMinorMatchInfoGui::HandleEventEntranceMinorMatchPlayerState( RWS::CMsg& msg )
{
	SNtlEventMinorMatchPlayerState* pPlayerState = reinterpret_cast<SNtlEventMinorMatchPlayerState*>( msg.pData );

	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

	stTBudokaiMember* pBudokaiMember = pTBWorldConcept->FindMember( pPlayerState->hPlayer );
	if( pBudokaiMember )
	{
		CTBMinorMatchEnemyInfoItem* pEnemyInfoItem = FindEnemyInfoItem( pBudokaiMember->pTeam->wTeamType );

		if( pEnemyInfoItem )
		{
			pEnemyInfoItem->SetMemberState( pBudokaiMember->pTeam->wTeamType );
		}

		if( pTBWorldConcept->GetMatchState() >= BUDOKAI_MINORMATCH_STATE_STAGE_READY )
		{
			m_numAliveCount.SetNumber( GetAliveEnemyCount() );
		}
	}	
}

VOID CTBMinorMatchInfoGui::HandleEventObserverMinorMatchPlayerState( RWS::CMsg& msg )
{
	SNtlEventMinorMatchPlayerState* pPlayerState = reinterpret_cast<SNtlEventMinorMatchPlayerState*>( msg.pData );

	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

	stTBudokaiMember* pBudokaiMember = pTBWorldConcept->FindMember( pPlayerState->hPlayer );
	if( pBudokaiMember )
	{
		CTBMinorMatchEnemyInfoItem* pEnemyInfoItem = FindEnemyInfoItem( pBudokaiMember->pTeam->wTeamType );

		if( pEnemyInfoItem )
		{
			pEnemyInfoItem->SetMemberState( pBudokaiMember->pTeam->wTeamType );
		}

		// 현재 매치 STATE가 READY보다 높다면 살아 있는 카운트를 세어서 적용한다.
		if( pTBWorldConcept->GetMatchState() >= BUDOKAI_MINORMATCH_STATE_STAGE_READY )
		{
			m_numAliveCount.SetNumber( GetAliveCount() );
		}
	}
}

VOID CTBMinorMatchInfoGui::HandleEventEntranceUpdateScore( RWS::CMsg& msg )
{
	SNtlEventMinorMatchUpdateScore* pData = reinterpret_cast<SNtlEventMinorMatchUpdateScore*>( msg.pData );
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

	stTBudokaiTeam* pMyTeam = pTBWorldConcept->GetMyTeam();

	if( !pMyTeam )
		return;

	if( pData->wTeamType == pMyTeam->wTeamType )
	{
		m_numKillCount.SetNumber( pData->byScore );				
	}
	else
	{
		m_listTeamSortByKillCount.sort( EnemyTeamSortByKillCount );

		RwInt32 idx = 0;
		std::list<stTBudokaiTeam*>::iterator iter;
		for( iter = m_listTeamSortByKillCount.begin() ; iter != m_listTeamSortByKillCount.end() ; ++iter )
		{
			if( idx >= MAX_TB_MINOR_ENEMYINFO )
				break;

			stTBudokaiTeam* pTeam = (*iter);
			if( pTeam->wTeamType == m_EnemyInfoArray[idx].GetTeamType() )
				m_EnemyInfoArray[idx].SetKillCount( pTeam->byTeamScore );
			else
				m_EnemyInfoArray[idx].SetData( pTeam->wTeamType, pTeam->wstrTeamName.c_str(), pTeam->byTeamScore );

			++idx;
		}
	}
}

VOID CTBMinorMatchInfoGui::HandleEventObserverUpdateScore( RWS::CMsg& msg )
{
	//SNtlEventMinorMatchUpdateScore* pData = reinterpret_cast<SNtlEventMinorMatchUpdateScore*>( msg.pData );
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CTBMinorMatchInfoGui::HandleEventObserverUpdateScore : must World concept is valid" );		


	m_listTeamSortByKillCount.sort( EnemyTeamSortByKillCount );

	RwInt32 idx = 0;
	std::list<stTBudokaiTeam*>::iterator iter;
	for( iter = m_listTeamSortByKillCount.begin() ; iter != m_listTeamSortByKillCount.end() ; ++iter )
	{
		if( idx >= MAX_TB_MINOR_ENEMYINFO )
			break;

		stTBudokaiTeam* pTeam = (*iter);
		if( pTeam->wTeamType == m_EnemyInfoArray[idx].GetTeamType() )
			m_EnemyInfoArray[idx].SetKillCount( pTeam->byTeamScore );
		else
			m_EnemyInfoArray[idx].SetData( pTeam->wTeamType, pTeam->wstrTeamName.c_str(), pTeam->byTeamScore );

		++idx;
	}
}

VOID CTBMinorMatchInfoGui::HandleEventEntranceMatchStateUpdate( RWS::CMsg& msg )
{
	SNtlEventMinorMatchStateUpdate* pData = reinterpret_cast<SNtlEventMinorMatchStateUpdate*>( msg.pData );

	if( pData->bEnterState )
	{
		if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_READY )
		{
			for( RwInt32 i = 0 ; i < MAX_TB_MINOR_ENEMYINFO ; ++i )
			{
				m_EnemyInfoArray[i].Show( FALSE );
			}

			CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
			NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

			m_numAliveCount.SetNumber( GetAliveEnemyCount() );
			m_numKillCount.SetNumber( 0 );				

			// 상대팀 정보 입력
			RwInt32 idx = 0;
			MAP_TBTEAM* pTeams = pTBWorldConcept->GetTeams();
			stTBudokaiTeam* pMyTeam = pTBWorldConcept->GetMyTeam();
			MAP_TBTEAM_ITER iter;			

			if( !pMyTeam )
				return;

			for( iter = pTeams->begin() ; iter != pTeams->end() ; ++iter )
			{
				stTBudokaiTeam* pTeam = (*iter).second;

				if( pTeam->wTeamType == pMyTeam->wTeamType )
					continue;

				if( idx < MAX_TB_MINOR_ENEMYINFO )
				{
					m_EnemyInfoArray[idx].SetData( pTeam->wTeamType, pTeam->wstrTeamName.c_str(), pTeam->byTeamScore );
					m_EnemyInfoArray[idx].Show( TRUE );
					++idx;				
				}				

				m_listTeamSortByKillCount.push_back( pTeam );
			}			

			GetSideDialogManager()->OpenDialog( SDIALOG_BUDOKAI_MINORMATCH_INFO );
		}		
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_MATCH_FINISH )
		{
			GetSideDialogManager()->CloseDialog( SDIALOG_BUDOKAI_MINORMATCH_INFO );
		}
	}	
}

VOID CTBMinorMatchInfoGui::HandleEventObserverMatchStateUpdate( RWS::CMsg& msg )
{
	SNtlEventMinorMatchStateUpdate* pData = reinterpret_cast<SNtlEventMinorMatchStateUpdate*>( msg.pData );

	if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_READY ||
		pData->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_RUN	||
		pData->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_FINISH )
	{
		for( RwInt32 i = 0 ; i < MAX_TB_MINOR_ENEMYINFO ; ++i )
		{
			m_EnemyInfoArray[i].Show( FALSE );
		}

		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		m_numAliveCount.SetNumber( GetAliveCount() );
		m_numKillCount.SetNumber( GetTotalMemberCount() );				

		// 현재 인원 정보 입력
		RwInt32 idx = 0;
		MAP_TBTEAM* pTeams = pTBWorldConcept->GetTeams();
		MAP_TBTEAM_ITER iter;			

		for( iter = pTeams->begin() ; iter != pTeams->end() ; ++iter )
		{
			stTBudokaiTeam* pTeam = (*iter).second;

			if( idx < MAX_TB_MINOR_ENEMYINFO )
			{
				m_EnemyInfoArray[idx].SetData( pTeam->wTeamType, pTeam->wstrTeamName.c_str(), pTeam->byTeamScore );
				m_EnemyInfoArray[idx].Show( TRUE );
				++idx;				
			}				

			m_listTeamSortByKillCount.push_back( pTeam );
		}			

		GetSideDialogManager()->OpenDialog( SDIALOG_BUDOKAI_MINORMATCH_INFO );
	}		
	else
	{
		GetSideDialogManager()->CloseDialog( SDIALOG_BUDOKAI_MINORMATCH_INFO );
	}
}

CTBMinorMatchEnemyInfoItem* CTBMinorMatchInfoGui::FindEnemyInfoItem( RwUInt16 wTeamType )
{
	for( RwInt32 i = 0 ; i < MAX_TB_MINOR_ENEMYINFO ; ++i )
	{
		if( m_EnemyInfoArray[i].GetTeamType() == wTeamType )
			return &m_EnemyInfoArray[i];
	}

	return NULL;
}

/**
* \brief 남아있는 적 인원수를 체크한다.
*/
RwUInt32 CTBMinorMatchInfoGui::GetAliveEnemyCount(VOID)
{
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

	MAP_TBMEMBER_ITER iterMember;
	MAP_TBMEMBER* pMembers = pTBWorldConcept->GetMembers();
	stTBudokaiTeam* pMyTeam = pTBWorldConcept->GetMyTeam();
	RwUInt32 uiAliveEnemyCount = 0;

	for( iterMember = pMembers->begin() ; iterMember != pMembers->end() ; ++iterMember )
	{
		stTBudokaiMember* pMember = (*iterMember).second;

		if( !pMyTeam )
		{
			++uiAliveEnemyCount;
		}
		else if( pMember->byState == MATCH_MEMBER_STATE_NORMAL && pMember->pTeam->wTeamType != pMyTeam->wTeamType )
		{
			++uiAliveEnemyCount;
		}
	}

	return uiAliveEnemyCount;
}

RwUInt32 CTBMinorMatchInfoGui::GetAliveCount( VOID )
{
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

	MAP_TBMEMBER_ITER iterMember;
	MAP_TBMEMBER* pMembers = pTBWorldConcept->GetMembers();

	RwUInt32 uiAliveCount = 0;
	
	for( iterMember = pMembers->begin(); iterMember != pMembers->end(); ++iterMember )
	{
		stTBudokaiMember* pMember = (*iterMember).second;

		if( pMember->byState == MATCH_MEMBER_STATE_NORMAL )
		{
			++uiAliveCount;
		}
	}

	return uiAliveCount;
}

RwUInt32 CTBMinorMatchInfoGui::GetTotalMemberCount( VOID )
{
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

	// 죽은 사람들까지 판단한다.
	RwUInt32 uiTotalMember = 0;
	MAP_TBTEAM* pTeams = pTBWorldConcept->GetTeams();
	MAP_TBTEAM_ITER iter;
	for( iter = pTeams->begin(); iter != pTeams->end(); ++iter )
	{
		stTBudokaiTeam* pTeam = (stTBudokaiTeam*)iter->second;
		
		uiTotalMember += pTeam->vecpMember.size();
	}

	return uiTotalMember;
}

RwBool CTBMinorMatchInfoGui::IsEntrance()
{
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must world concept is valid" );

	// 천하제일 무도회에 자신이 없으면 참가자가 아니다
	stTBudokaiMember* pMember = pTBWorldConcept->FindMember( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
	if( pMember )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID CTBMinorMatchInfoGui::UpdateInfoType()
{
	if( IsEntrance() )
	{
		m_byInfoType = INFO_ENTRANCE;
	}
	else
	{
		m_byInfoType = INFO_OBSERVER;
	}
}

VOID CTBMinorMatchInfoGui::SetInfoFrame()
{
	// GUI INFO TYPE에 따라 다름
	if( m_byInfoType == INFO_ENTRANCE )
	{
		m_pStbAllPlayer->Show( false );
		m_pStbSurvivedPlayer->Show( false );
	}
	else if( m_byInfoType == INFO_OBSERVER )
	{
		m_pPnlKilledEnemy->Show( false );
		m_pPnlSurvivedEnemy->Show( false );
	}
	else
	{
		m_pStbAllPlayer->Show( false );
		m_pStbSurvivedPlayer->Show( false );
	}
}

VOID CTBMinorMatchInfoGui::SetInfoOnce()
{
	if( m_byInfoType == INFO_OBSERVER )
	{
		m_numAliveCount.SetNumber( GetAliveCount() );
		m_numKillCount.SetNumber( GetTotalMemberCount() );
	}
}

VOID CTBMinorMatchInfoGui::OnPaint(VOID)
{
	m_numAliveCount.Render();
	m_numKillCount.Render();
}	

VOID CTBMinorMatchInfoGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_numAliveCount.SetPosition( rtScreen.left, rtScreen.top );
	m_numKillCount.SetPosition( rtScreen.left, rtScreen.top );
}

VOID CTBMinorMatchInfoGui::OnClickMinimize( gui::CComponent* pComponent )
{
	CDboEventGenerator::SideDialogEvent( SDIALOG_MESSAGE_MINIMIZE, SDIALOG_BUDOKAI_MINORMATCH_INFO, DIALOG_SIDEDIALOG_CONTROLLER );
}

bool EnemyTeamSortByKillCount( const stTBudokaiTeam* lhs, const stTBudokaiTeam* rhs )
{
	return lhs->byTeamScore > rhs->byTeamScore;
}