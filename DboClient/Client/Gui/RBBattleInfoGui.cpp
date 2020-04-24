#include "precomp_dboclient.h"
#include "RBBattleInfoGui.h"

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

#define RBENEMY_PARTYINFO_CLASSICON_X	-24		// 각 파티인포아이템 기준 좌표.
#define RBENEMY_PARTYINFO_CLASSICON_Y	0	

#define RB_BATTLEINFO_KAMESCORE_X		127
#define RB_BATTLEINFO_TSURUSCORE_X		38
#define RB_BATTLEINFO_SCORE_Y			62

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// RBBattleInfoItem

CRBBattleEnemyInfoItem::CRBBattleEnemyInfoItem(VOID)
: m_pName( NULL ), m_hSerial( INVALID_SERIAL_ID )
{

}

CRBBattleEnemyInfoItem::~CRBBattleEnemyInfoItem(VOID)
{

}

VOID CRBBattleEnemyInfoItem::SetUI( gui::CStaticBox* pName )
{
	m_pName = pName;	
}

VOID CRBBattleEnemyInfoItem::SetData( RwUInt32 hSerial, RwUInt8 byClass, std::wstring& wstrName )
{
	m_hSerial = hSerial;
	
	m_surClass.SetSurface( Logic_GetPCClassIconSurface( byClass, FALSE ) );
	m_surClass.SetPositionfromParent( RBENEMY_PARTYINFO_CLASSICON_X, RBENEMY_PARTYINFO_CLASSICON_Y );
	CRectangle rtScreen = m_pName->GetScreenRect();
	m_surClass.SetPositionbyParent( rtScreen.left, rtScreen.top );
	m_surClass.Show( TRUE );

	m_pName->SetText( wstrName.c_str() );
}

VOID CRBBattleEnemyInfoItem::SetState( RwUInt8 byState )
{
	switch( byState )
	{
		case RANKBATTLE_MEMBER_STATE_NONE:
		case RANKBATTLE_MEMBER_STATE_NORMAL:
		case RANKBATTLE_MEMBER_STATE_ATTACKABLE:
			SetEnable( TRUE );
		break;
		case RANKBATTLE_MEMBER_STATE_OUTOFAREA:
		case RANKBATTLE_MEMBER_STATE_FAINT:
		case RANKBATTLE_MEMBER_STATE_GIVEUP:
			SetEnable(FALSE);
		break;
		case RANKBATTLE_MEMBER_STATE_NOAPPLY:
			InitData();
			Show( FALSE );
		break;
	}
}

VOID CRBBattleEnemyInfoItem::InitData(VOID)
{
	m_hSerial = INVALID_SERIAL_ID;
	m_pName->Clear();
	m_surClass.UnsetTexture();
	m_surClass.Show( FALSE );	
}

VOID CRBBattleEnemyInfoItem::SetEnable( RwBool bEnable )
{
	if( bEnable )
		m_pName->SetTextColor(SIDE_DIALOG_FONT_GREEN_COLOR, TRUE );
	else
		m_pName->SetTextColor(INFOCOLOR_5, TRUE );
}

VOID CRBBattleEnemyInfoItem::Show( RwBool bShow )
{
	if( bShow )
	{
		m_pName->Show( true );		
	}
	else
	{
		m_pName->Show( false );		
	}	
}

VOID CRBBattleEnemyInfoItem::OnPaint(VOID)
{
	m_surClass.Render();	
}

VOID CRBBattleEnemyInfoItem::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_pName->GetScreenRect();

	m_surClass.SetPositionbyParent( rtScreen.left, rtScreen.top );	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRBBattleInfoGui

CRBBattleInfoGui::CRBBattleInfoGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	
}

CRBBattleInfoGui::~CRBBattleInfoGui(VOID)
{

}

RwBool CRBBattleInfoGui::Create(VOID)
{
	NTL_FUNCTION( "CRBBattleInfoGui::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\RankBattleInfo.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	// RBEnemyInfo
	gui::CStaticBox* pName = NULL;
	gui::CPanel* pNameBack = NULL;
	gui::CStaticBox* pstbBattleOrder = NULL;
	
	pName = (gui::CStaticBox*)GetComponent( "stbName1" );
	pstbBattleOrder = (gui::CStaticBox*)GetComponent("stbBattleOrder1");
	pNameBack = (gui::CPanel*)GetComponent( "pnlNameBack1" );
	m_EnemyInfoArray[0].SetUI( pName );
	m_EnemyInfoArray[0].Show( FALSE );

	pName = (gui::CStaticBox*)GetComponent( "stbName2" );
	pstbBattleOrder = (gui::CStaticBox*)GetComponent("stbBattleOrder2");
	pNameBack = (gui::CPanel*)GetComponent( "pnlNameBack2" );
	m_EnemyInfoArray[1].SetUI( pName );
	m_EnemyInfoArray[1].Show( FALSE );

	pName = (gui::CStaticBox*)GetComponent( "stbName3" );
	pstbBattleOrder = (gui::CStaticBox*)GetComponent("stbBattleOrder3");
	pNameBack = (gui::CPanel*)GetComponent( "pnlNameBack3" );
	m_EnemyInfoArray[2].SetUI( pName );
	m_EnemyInfoArray[2].Show( FALSE );

	pName = (gui::CStaticBox*)GetComponent( "stbName4" );
	pstbBattleOrder = (gui::CStaticBox*)GetComponent("stbBattleOrder4");
	pNameBack = (gui::CPanel*)GetComponent( "pnlNameBack4" );
	m_EnemyInfoArray[3].SetUI( pName );
	m_EnemyInfoArray[3].Show( FALSE );

	pName = (gui::CStaticBox*)GetComponent( "stbName5" );
	pstbBattleOrder = (gui::CStaticBox*)GetComponent("stbBattleOrder5");
	pNameBack = (gui::CPanel*)GetComponent( "pnlNameBack5" );
	m_EnemyInfoArray[4].SetUI( pName );
	m_EnemyInfoArray[4].Show( FALSE );

	// RBBattleInfo
	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_pstbKameTeamName = (gui::CStaticBox*)GetComponent( "stbMyTeamName" );
	m_pstbTsuruTeamName = (gui::CStaticBox*)GetComponent( "stbEnemyTeamName" );
	m_pstbVS = (gui::CStaticBox*)GetComponent( "stbVS" );
	m_pmdbStraightWinCount = (gui::CMDStaticBox*)GetComponent( "mdbStraightWinCount" );
	m_pmdbPerfectWinCount = (gui::CMDStaticBox*)GetComponent( "mdbPerfectWinCount" );
	m_pbtnMinimize = (gui::CButton*)GetComponent( "btnMinimize" );
	m_pstbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pstbEnemyMember = (gui::CStaticBox*)GetComponent( "stbEnemyMember" );

	// 색설정
	m_pstbKameTeamName->SetTextColor( NTL_BLUE_TEAM_COLOR );
	m_pstbTsuruTeamName->SetTextColor( NTL_RED_TEAM_COLOR );
	m_pstbKameTeamName->SetEffectColor( RANKBATTLE_COLOR_GREEN );
	m_pstbTsuruTeamName->SetEffectColor( RANKBATTLE_COLOR_GREEN );

	m_pstbVS->SetText( GetDisplayStringManager()->GetString( "DST_RB_VS" ) );
	m_pstbKameTeamName->SetText( GetDisplayStringManager()->GetString( "DST_RB_BLUE_TEAM" ) );
	m_pstbTsuruTeamName->SetText( GetDisplayStringManager()->GetString( "DST_RB_RED_TEAM" ) );
	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( "DST_SIDEDLG_TITLE_RANKBATTLEINFO" ) );
	m_pstbEnemyMember->SetText( GetDisplayStringManager()->GetString( "DST_RB_ENEMY_MEMBER" ) );

	m_numKameWinCount.Create( ND_CENTER );
	m_numKameWinCount.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_0" ) );
	m_numKameWinCount.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_1" ) );
	m_numKameWinCount.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_2" ) );
	m_numKameWinCount.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_3" ) );
	m_numKameWinCount.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_4" ) );
	m_numKameWinCount.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_5" ) );
	m_numKameWinCount.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_6" ) );
	m_numKameWinCount.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_7" ) );
	m_numKameWinCount.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_8" ) );
	m_numKameWinCount.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_9" ) );
	m_numKameWinCount.SetParentPos( RB_BATTLEINFO_KAMESCORE_X, RB_BATTLEINFO_SCORE_Y );
	m_numKameWinCount.SetPosition( rtScreen.left, rtScreen.top );
	m_numKameWinCount.SetNumGap(-5);

	m_numTsuruWinCount.Create( ND_CENTER );
	m_numTsuruWinCount.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_0" ) );
	m_numTsuruWinCount.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_1" ) );
	m_numTsuruWinCount.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_2" ) );
	m_numTsuruWinCount.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_3" ) );
	m_numTsuruWinCount.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_4" ) );
	m_numTsuruWinCount.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_5" ) );
	m_numTsuruWinCount.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_6" ) );
	m_numTsuruWinCount.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_7" ) );
	m_numTsuruWinCount.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_8" ) );
	m_numTsuruWinCount.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_9" ) );
	m_numTsuruWinCount.SetParentPos( RB_BATTLEINFO_TSURUSCORE_X, RB_BATTLEINFO_SCORE_Y );
	m_numTsuruWinCount.SetPosition( rtScreen.left, rtScreen.top );
	m_numTsuruWinCount.SetNumGap(-5);

	m_slotMove = m_pThis->SigMove().Connect( this, &CRBBattleInfoGui::OnMove );
	m_slotPostPaint = m_pstbVS->SigPaint().Connect( this, &CRBBattleInfoGui::OnPostPaint );
	m_slotPaint = m_pThis->SigPaint().Connect( this, &CRBBattleInfoGui::OnPaint );
	m_slotClickMinimize = m_pbtnMinimize->SigClicked().Connect( this, &CRBBattleInfoGui::OnClickMinimize );

	Show( false );

	//LinkMsg( g_EventRBBattleTeamInfo );	// WorldConceptChange이전에 날라오는 이벤트, 여기에서는 생성되기 전에 호출되므로 불용.
	LinkMsg( g_EventRBBattlePlayerState );
	LinkMsg( g_EventRBBattleStateUpdate );
	LinkMsg( g_EventRBBattleStageFinish );
	LinkMsg( g_EventRBBattleMatchFinish );

	NTL_RETURN( TRUE );
}

VOID CRBBattleInfoGui::Destroy(VOID)
{
	//UnLinkMsg( g_EventRBBattleTeamInfo );
	UnLinkMsg( g_EventRBBattlePlayerState );
	UnLinkMsg( g_EventRBBattleStateUpdate );
	UnLinkMsg( g_EventRBBattleStageFinish );
	UnLinkMsg( g_EventRBBattleMatchFinish );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CRBBattleInfoGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );
	return 1;
}

VOID CRBBattleInfoGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventRBBattlePlayerState )
	{
		SNtlEventRBBattlePlayerState* pPlayerState = reinterpret_cast<SNtlEventRBBattlePlayerState*>( msg.pData );
		CRBBattleEnemyInfoItem* pEnemyInfo = FindEnemyInfo( pPlayerState->hSerial );
		if( pEnemyInfo )
			pEnemyInfo->SetState( pPlayerState->byState );		
	}
	else if( msg.Id == g_EventRBBattleStateUpdate )
	{
		SNtlEventRBBattleStateUpdate* pData = reinterpret_cast<SNtlEventRBBattleStateUpdate*>( msg.pData );
		CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();

		if( pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_PREPARE)
		{
			if( pData->byStage == 0 )
			{
				m_numKameWinCount.SetNumber( 0 );
				m_numTsuruWinCount.SetNumber( 0 );				

				if( pRankBattle->IsRedTeam() )
				{
					m_pstbTsuruTeamName->SetEffectMode( TE_OUTLINE );
					m_pstbTsuruTeamName->SetEffectValue( 2, TRUE );

					m_pstbKameTeamName->SetEffectValue( TE_NONE, TRUE );

					//SetStraightWinScore( 0 );
					//SetPerfectWinScore( 0 );
				}
				else
				{
					m_pstbKameTeamName->SetEffectMode( TE_OUTLINE );
					m_pstbKameTeamName->SetEffectValue( 2, TRUE );

					m_pstbTsuruTeamName->SetEffectValue( TE_NONE, TRUE );

					//SetStraightWinScore( pRankBattle->GetBlueTeamStraightWinScore() );
					//SetPerfectWinScore( pRankBattle->GetBlueTeamPerfectWinScore() );
				}

				//if( pRankBattle->IsRoomOwner() )
				//{
				//	m_pstbKameTeamName->SetTextColor( RANKBATTLE_COLOR_YELLOW, TRUE );
				//	m_pstbTsuruTeamName->SetTextColor( RANKBATTLE_COLOR_GREEN, TRUE );
				//}
				//else
				//{
				//	m_pstbKameTeamName->SetTextColor( RANKBATTLE_COLOR_GREEN, TRUE );
				//	m_pstbTsuruTeamName->SetTextColor( RANKBATTLE_COLOR_YELLOW, TRUE );
				//}
								
				GetSideDialogManager()->OpenDialog( SDIALOG_RANKBATTLE_INFO );
			}
			
			CNtlRankBattle::MAP_RBMEMBER* pMap = pRankBattle->GetEnemyTeamMap();
			CNtlRankBattle::MAP_RBMEMBER_ITER it;
			RwInt32 idx = 0;

			for( it = pMap->begin(), idx = 0 ; it != pMap->end() ; ++it )
			{
				CNtlRankBattle::stRankBattleMember* pMember = &(*it).second;
				if( pMember->byState != RANKBATTLE_MEMBER_STATE_NOAPPLY )
				{
					m_EnemyInfoArray[idx].SetData( pMember->hSerial, pMember->byClass, pMember->wstrName );
					m_EnemyInfoArray[idx].SetState( pMember->byState );
					m_EnemyInfoArray[idx].Show( TRUE );
					++idx;
				}
			}			
		}
		//else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_RUN )
		//{
		//	for( RwInt32 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
		//	{
		//		m_EnemyInfoArray[i].Show( FALSE );
		//		m_EnemyInfoArray[i].InitData();
		//	}
		//}		
	}
	else if( msg.Id == g_EventRBBattleStageFinish )
	{
		CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();

		if( pRankBattle->IsRoomOwner() )
		{
			m_numKameWinCount.SetNumber( pRankBattle->GetMyTeamWinScore() );
			m_numTsuruWinCount.SetNumber( pRankBattle->GetEnemyTeamWinScore() );
		}
		else
		{
			m_numKameWinCount.SetNumber( pRankBattle->GetEnemyTeamWinScore() );
			m_numTsuruWinCount.SetNumber( pRankBattle->GetMyTeamWinScore() );
		}
		
	}
	else if( msg.Id == g_EventRBBattleMatchFinish )
	{
		//SNtlEventRBBattleMatchFinish* pMatchFinishInfo = reinterpret_cast<SNtlEventRBBattleMatchFinish*>( msg.pData );
		//sRANKBATTLE_MATCH_FINISH_INFO* pFinishInfo = reinterpret_cast<sRANKBATTLE_MATCH_FINISH_INFO*>( pMatchFinishInfo->pMatchResult );
		CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();

		if( pRankBattle->GetResult() == CNtlRankBattle::WIN )
		{
			//SetStraightWinScore( pFinishInfo->sWinerRecord.wStraightWinCount );
			//SetPerfectWinScore( pFinishInfo->sWinerRecord.wStraightKOWinCount );
		}
		else
		{
			//SetStraightWinScore( 0 );
			//SetPerfectWinScore( 0 );
		}		
	}	
	//else if( msg.Id == g_EventRBBattleTeamInfo )
	//{
	//	SNtlEventRBBattleTeamInfo* pData = reinterpret_cast<SNtlEventRBBattleTeamInfo*>( msg.pData );
	//	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();

	//	if( pRankBattle->IsRoomOwner() )
	//	{
	//		SetStraightWinScore( pData->wStraightWinCount );
	//		SetPerfectWinScore( pData->wPerfectWinCount );
	//	}
	//	else
	//	{
	//		SetStraightWinScore( 0 );
	//		SetPerfectWinScore( 0 );
	//	}

	//	if( pRankBattle->IsRedTeam() )
	//	{
	//		m_pstbTsuruTeamName->SetEffectMode( TE_OUTLINE );
	//		m_pstbTsuruTeamName->SetEffectValue( 2, TRUE );

	//		m_pstbKameTeamName->SetEffectValue( TE_NONE, TRUE );
	//	}
	//	else
	//	{
	//		m_pstbKameTeamName->SetEffectMode( TE_OUTLINE );
	//		m_pstbKameTeamName->SetEffectValue( 2, TRUE );

	//		m_pstbTsuruTeamName->SetEffectValue( TE_NONE, TRUE );
	//	}
	//}
}

CRBBattleEnemyInfoItem* CRBBattleInfoGui::FindEnemyInfo( RwUInt32 hSerial )
{
	if( hSerial == INVALID_SERIAL_ID )
		return NULL;

	for( RwInt32 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		if( m_EnemyInfoArray[i].GetSerial() == hSerial )
			return &m_EnemyInfoArray[i];
	}

	return NULL;
}

VOID CRBBattleInfoGui::SetStraightWinScore( RwInt32 nStraightWinScore )
{
	RwInt32 nAlign = COMP_TEXT_LEFT;
	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
	if( !pRankBattle->IsRoomOwner() )
		nAlign = COMP_TEXT_RIGHT;

	const WCHAR* pString = GetDisplayStringManager()->GetString( "DST_RB_STRAIGHT_WIN" );
	m_pmdbStraightWinCount->SetItem( nStraightWinScore, "straightwincount", 0, nAlign, RANKBATTLE_COLOR_YELLOW );
	m_pmdbStraightWinCount->SetItem( pString, "straightwin", 0, COMP_TEXT_LEFT, SIDE_DIALOG_FONT_GREEN_COLOR, 5, TRUE );
	m_pmdbStraightWinCount->DrawItem();
}

VOID CRBBattleInfoGui::SetPerfectWinScore( RwInt32 nPerfectWinScore )
{
	RwInt32 nAlign = COMP_TEXT_LEFT;
	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
	if( !pRankBattle->IsRoomOwner() )
		nAlign = COMP_TEXT_RIGHT;

	const WCHAR* pString = GetDisplayStringManager()->GetString( "DST_RB_PERFECT_WIN" );
	m_pmdbPerfectWinCount->SetItem( nPerfectWinScore, "perfectwincount", 0, nAlign, RANKBATTLE_COLOR_YELLOW );
	m_pmdbPerfectWinCount->SetItem( pString, "perfectwin", 0, COMP_TEXT_LEFT, SIDE_DIALOG_FONT_GREEN_COLOR, 5, TRUE );
	m_pmdbPerfectWinCount->DrawItem();
}

VOID CRBBattleInfoGui::OnPaint(VOID)
{
	m_numKameWinCount.Render();
	m_numTsuruWinCount.Render();
}	

VOID CRBBattleInfoGui::OnPostPaint(VOID)
{
	for( RwInt32 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		m_EnemyInfoArray[i].OnPaint();
	}
}

VOID CRBBattleInfoGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	for( RwInt32 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		m_EnemyInfoArray[i].OnMove( nOldX, nOldY );
	}

	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_numKameWinCount.SetPosition( rtScreen.left, rtScreen.top );
	m_numTsuruWinCount.SetPosition( rtScreen.left, rtScreen.top );
}

VOID CRBBattleInfoGui::OnClickMinimize( gui::CComponent* pComponent )
{
	CDboEventGenerator::SideDialogEvent( SDIALOG_MESSAGE_MINIMIZE, SDIALOG_RANKBATTLE_INFO, DIALOG_SIDEDIALOG_CONTROLLER );
}