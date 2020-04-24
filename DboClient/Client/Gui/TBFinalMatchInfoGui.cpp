#include "precomp_dboclient.h"
#include "TBFinalMatchInfoGui.h"
#
// Shared
#include "PCTable.h"

// Presentation
#include "NtlPLGuiManager.h"

// sound
#include "GUISoundDefine.h"

// Simulation
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlWorldConceptTB.h"
#include "NtlSLLogic.h"

// Dbo Client
#include "SideDialogManager.h"
#include "DboLogic.h"
#include "DisplayStringManager.h"
#include "DboEventGenerator.h"

#define TB_FINAL_ENEMY_PARTYINFO_CLASSICON_X	-24		// 각 파티인포아이템 기준 좌표.
#define TB_FINAL_ENEMY_PARTYINFO_CLASSICON_Y	0	

#define TB_FINAL_BATTLEINFO_BLUESCORE_X		127
#define TB_FINAL_BATTLEINFO_REDSCORE_X		38
#define TB_FINAL_BATTLEINFO_SCORE_Y			62	

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TBFinalMatchInfoItem

CTBFinalMatchEnemyInfoItem::CTBFinalMatchEnemyInfoItem(VOID)
: m_pName( NULL ), m_hSerial( INVALID_SERIAL_ID )
{

}

CTBFinalMatchEnemyInfoItem::~CTBFinalMatchEnemyInfoItem(VOID)
{

}

VOID CTBFinalMatchEnemyInfoItem::SetUI( gui::CStaticBox* pName )
{
	m_pName = pName;	
}

VOID CTBFinalMatchEnemyInfoItem::SetData( RwUInt32 hSerial, RwUInt8 byClass, std::wstring& wstrName )
{
	m_hSerial = hSerial;

	m_surClass.SetSurface( Logic_GetPCClassIconSurface( byClass, FALSE ) );
	m_surClass.SetPositionfromParent( TB_FINAL_ENEMY_PARTYINFO_CLASSICON_X, TB_FINAL_ENEMY_PARTYINFO_CLASSICON_Y );
	CRectangle rtScreen = m_pName->GetScreenRect();
	m_surClass.SetPositionbyParent( rtScreen.left, rtScreen.top );
	m_surClass.Show( TRUE );

	m_pName->SetText( wstrName.c_str() );
}

VOID CTBFinalMatchEnemyInfoItem::SetState( RwUInt8 byState )
{
	switch( byState )
	{
	case MATCH_MEMBER_STATE_NONE:
	case MATCH_MEMBER_STATE_NORMAL:
		SetEnable( TRUE );
		break;
	case MATCH_MEMBER_STATE_OUTOFAREA:
	case MATCH_MEMBER_STATE_FAINT:
	case MATCH_MEMBER_STATE_GIVEUP:
	case MATCH_MEMBER_STATE_RESCUE:
		SetEnable( FALSE );
		break;
	case MATCH_MEMBER_STATE_NOAPPLY:
		InitData();
		Show( FALSE );
		break;
	}
}

VOID CTBFinalMatchEnemyInfoItem::InitData(VOID)
{
	m_hSerial = INVALID_SERIAL_ID;
	m_pName->Clear();
	m_surClass.UnsetTexture();
	m_surClass.Show( FALSE );	
}

VOID CTBFinalMatchEnemyInfoItem::SetEnable( RwBool bEnable )
{
	if( bEnable )
		m_pName->SetTextColor( SIDE_DIALOG_FONT_GREEN_COLOR, TRUE );
	else
		m_pName->SetTextColor( INFOCOLOR_5, TRUE );	
}

VOID CTBFinalMatchEnemyInfoItem::Show( RwBool bShow )
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

VOID CTBFinalMatchEnemyInfoItem::OnPaint(VOID)
{
	if( m_pName->IsVisible() )
		m_surClass.Render();	
}

VOID CTBFinalMatchEnemyInfoItem::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_pName->GetScreenRect();

	m_surClass.SetPositionbyParent( rtScreen.left, rtScreen.top );	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBFinalMatchInfoGui

CTBFinalMatchInfoGui::CTBFinalMatchInfoGui( const RwChar* pName )
: CNtlPLGui( pName )
{

}

CTBFinalMatchInfoGui::~CTBFinalMatchInfoGui(VOID)
{

}

RwBool CTBFinalMatchInfoGui::Create(VOID)
{
	NTL_FUNCTION( "CTBFinalMatchInfoGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\BudokaiMatchInfo.srf", "gui\\BudokaiFinalMatchInfo.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );


	gui::CStaticBox* pName = NULL;
	gui::CPanel* pNameBack = NULL;
	
	pName = (gui::CStaticBox*)GetComponent( "stbName1" );
	pNameBack = (gui::CPanel*)GetComponent( "pnlNameBack1" );
	m_EnemyInfoArray[0].SetUI( pName );
	m_EnemyInfoArray[0].Show( FALSE );

	pName = (gui::CStaticBox*)GetComponent( "stbName2" );
	pNameBack = (gui::CPanel*)GetComponent( "pnlNameBack2" );
	m_EnemyInfoArray[1].SetUI( pName );
	m_EnemyInfoArray[1].Show( FALSE );

	pName = (gui::CStaticBox*)GetComponent( "stbName3" );
	pNameBack = (gui::CPanel*)GetComponent( "pnlNameBack3" );
	m_EnemyInfoArray[2].SetUI( pName );
	m_EnemyInfoArray[2].Show( FALSE );

	pName = (gui::CStaticBox*)GetComponent( "stbName4" );
	pNameBack = (gui::CPanel*)GetComponent( "pnlNameBack4" );
	m_EnemyInfoArray[3].SetUI( pName );
	m_EnemyInfoArray[3].Show( FALSE );

	pName = (gui::CStaticBox*)GetComponent( "stbName5" );
	pNameBack = (gui::CPanel*)GetComponent( "pnlNameBack5" );
	m_EnemyInfoArray[4].SetUI( pName );
	m_EnemyInfoArray[4].Show( FALSE );

	// TBFinalMatchInfo
	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_pstbBlueTeamName = (gui::CStaticBox*)GetComponent( "stbBlueTeamName" );
	m_pstbRedTeamName = (gui::CStaticBox*)GetComponent( "stbRedTeamName" );
	m_pstbVS = (gui::CStaticBox*)GetComponent( "stbVS" );
	m_pbtnMinimize = (gui::CButton*)GetComponent( "btnMinimize" );
	m_pstbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pstbEnemyMember = (gui::CStaticBox*)GetComponent( "stbEnemyMember" );

	m_pstbVS->SetText( GetDisplayStringManager()->GetString( "DST_RB_VS" ) );
	m_pstbBlueTeamName->SetText( GetDisplayStringManager()->GetString( "DST_RB_BLUE_TEAM" ) );
	m_pstbRedTeamName->SetText( GetDisplayStringManager()->GetString( "DST_RB_RED_TEAM" ) );
	m_pstbEnemyMember->SetText( GetDisplayStringManager()->GetString( "DST_RB_ENEMY_MEMBER" ) );
	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_REQUEST_FINAL" ) );

	m_numBlueWinCount.Create( ND_CENTER );
	m_numBlueWinCount.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_0" ) );
	m_numBlueWinCount.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_1" ) );
	m_numBlueWinCount.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_2" ) );
	m_numBlueWinCount.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_3" ) );
	m_numBlueWinCount.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_4" ) );
	m_numBlueWinCount.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_5" ) );
	m_numBlueWinCount.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_6" ) );
	m_numBlueWinCount.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_7" ) );
	m_numBlueWinCount.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_8" ) );
	m_numBlueWinCount.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_9" ) );
	m_numBlueWinCount.SetParentPos( TB_FINAL_BATTLEINFO_BLUESCORE_X, TB_FINAL_BATTLEINFO_SCORE_Y );
	m_numBlueWinCount.SetPosition( rtScreen.left, rtScreen.top );
	m_numBlueWinCount.SetNumGap(-5);

	m_numRedWinCount.Create( ND_CENTER );
	m_numRedWinCount.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_0" ) );
	m_numRedWinCount.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_1" ) );
	m_numRedWinCount.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_2" ) );
	m_numRedWinCount.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_3" ) );
	m_numRedWinCount.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_4" ) );
	m_numRedWinCount.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_5" ) );
	m_numRedWinCount.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_6" ) );
	m_numRedWinCount.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_7" ) );
	m_numRedWinCount.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_8" ) );
	m_numRedWinCount.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_9" ) );
	m_numRedWinCount.SetParentPos( TB_FINAL_BATTLEINFO_REDSCORE_X, TB_FINAL_BATTLEINFO_SCORE_Y );
	m_numRedWinCount.SetPosition( rtScreen.left, rtScreen.top );
	m_numRedWinCount.SetNumGap(-5);

	m_slotMove = m_pThis->SigMove().Connect( this, &CTBFinalMatchInfoGui::OnMove );
	m_slotPostPaint = m_pstbVS->SigPaint().Connect( this, &CTBFinalMatchInfoGui::OnPostPaint );
	m_slotPaint = m_pThis->SigPaint().Connect( this, &CTBFinalMatchInfoGui::OnPaint );
	m_slotClickMinimize = m_pbtnMinimize->SigClicked().Connect( this, &CTBFinalMatchInfoGui::OnClickMinimize );

	Show( false );

	LinkMsg( g_EventFinalMatchPlayerState, NULL, 0x7000 );
	LinkMsg( g_EventFinalMatchStateUpdate, NULL, 0x7000 );
	LinkMsg( g_EventFinalMatchStageFinish, NULL, 0x7000 );

	NTL_RETURN( TRUE );
}

VOID CTBFinalMatchInfoGui::Destroy(VOID)
{
	UnLinkMsg( g_EventFinalMatchPlayerState );
	UnLinkMsg( g_EventFinalMatchStateUpdate );
	UnLinkMsg( g_EventFinalMatchStageFinish );	

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CTBFinalMatchInfoGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );	

		if( !pTBWorldConcept->GetMyTeam() )
			return -1;

		if( pTBWorldConcept->GetMatchState() == BUDOKAI_FINALMATCH_STATE_STAGE_READY ||
			pTBWorldConcept->GetMatchState() == BUDOKAI_FINALMATCH_STATE_STAGE_RUN ||
			pTBWorldConcept->GetMatchState() == BUDOKAI_FINALMATCH_STATE_STAGE_FINISH )
			Show( true );
		else
			return -1;
	}
	else
		Show( false );
	
	return 1;
}

VOID CTBFinalMatchInfoGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventFinalMatchPlayerState )
	{
		SNtlEventFinalMatchPlayerState* pPlayerState = reinterpret_cast<SNtlEventFinalMatchPlayerState*>( msg.pData );
		CTBFinalMatchEnemyInfoItem* pEnemyInfo = FindEnemyInfo( pPlayerState->hPlayer );

		if( pEnemyInfo )
			pEnemyInfo->SetState( pPlayerState->byPcState );		
	}
	else if( msg.Id == g_EventFinalMatchStateUpdate )
	{
		SNtlEventFinalMatchStateUpdate* pData = reinterpret_cast<SNtlEventFinalMatchStateUpdate*>( msg.pData );
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );	

		if( pData->bEnterState )
		{
			// 현재 자신의 경기가 아니라면
			if( !pTBWorldConcept->GetMyTeam() )
				return;

			// 참가자가 아니라면
			if( !pTBWorldConcept->GetMyTeam() )
				return;

			if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_READY )
			{
				if( pData->byStage == 0 )
				{
					for( RwInt32 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
					{
						m_EnemyInfoArray[i].Show( FALSE );
					}

					m_numBlueWinCount.SetNumber( 0 );
					m_numRedWinCount.SetNumber( 0 );				

					if( pTBWorldConcept->GetMyTeam() )
					{
						if( pTBWorldConcept->MyTeamIsBlue() )
						{
							m_pstbBlueTeamName->SetTextColor( RANKBATTLE_COLOR_YELLOW, TRUE );
							m_pstbRedTeamName->SetTextColor( RANKBATTLE_COLOR_GREEN, TRUE );
						}
						else
						{
							m_pstbBlueTeamName->SetTextColor( RANKBATTLE_COLOR_GREEN, TRUE );
							m_pstbRedTeamName->SetTextColor( RANKBATTLE_COLOR_YELLOW, TRUE );
						}

						stTBudokaiTeam* pEnemyTeam = pTBWorldConcept->GetEnemyTeam();

						if( pEnemyTeam )
						{
							VEC_TBTEAMMEMBER* pEnemyTeamMember = &pEnemyTeam->vecpMember;
							VEC_TBTEAMMEMBER_ITER it;
							RwInt32 idx = 0;

							for( it = pEnemyTeamMember->begin() ; it != pEnemyTeamMember->end() ; ++it )
							{
								stTBudokaiMember* pMember = (*it);
								if( pMember->byState != MATCH_MEMBER_STATE_NOAPPLY )
								{
									m_EnemyInfoArray[idx].SetData( pMember->hSerial, pMember->byClass, pMember->wstrName );
									m_EnemyInfoArray[idx].SetState( pMember->byState );
									m_EnemyInfoArray[idx].Show( TRUE );
									++idx;
								}
							}			
						}					

						m_pstbEnemyMember->Show( true );
					}
					else
					{
						m_pstbBlueTeamName->SetTextColor( RANKBATTLE_COLOR_GREEN, TRUE );
						m_pstbRedTeamName->SetTextColor( RANKBATTLE_COLOR_GREEN, TRUE );

						m_pstbEnemyMember->Show( false );
					}

					GetSideDialogManager()->OpenDialog( SDIALOG_BUDOKAI_FINALMATCH_INFO );
				}			
			}		
			else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_MATCH_FINISH )
			{
				GetSideDialogManager()->CloseDialog( SDIALOG_BUDOKAI_FINALMATCH_INFO );
			}
		}		
	}
	else if( msg.Id == g_EventFinalMatchStageFinish )
	{
		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		SNtlEventFinalMatchStageFinish* pStageFinish = reinterpret_cast<SNtlEventFinalMatchStageFinish*>( msg.pData );

		if( !pStageFinish->bIsObserver )
		{
			m_numBlueWinCount.SetNumber( pStageFinish->pStageScore->byTeam1 );
			m_numRedWinCount.SetNumber( pStageFinish->pStageScore->byTeam2 );		
		}
	}	
}

CTBFinalMatchEnemyInfoItem* CTBFinalMatchInfoGui::FindEnemyInfo( RwUInt32 hSerial )
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

VOID CTBFinalMatchInfoGui::OnPaint(VOID)
{
	m_numBlueWinCount.Render();
	m_numRedWinCount.Render();
}	

VOID CTBFinalMatchInfoGui::OnPostPaint(VOID)
{
	for( RwInt32 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		m_EnemyInfoArray[i].OnPaint();
	}
}

VOID CTBFinalMatchInfoGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	for( RwInt32 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		m_EnemyInfoArray[i].OnMove( nOldX, nOldY );
	}

	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_numBlueWinCount.SetPosition( rtScreen.left, rtScreen.top );
	m_numRedWinCount.SetPosition( rtScreen.left, rtScreen.top );
}

VOID CTBFinalMatchInfoGui::OnClickMinimize( gui::CComponent* pComponent )
{
	CDboEventGenerator::SideDialogEvent( SDIALOG_MESSAGE_MINIMIZE, SDIALOG_BUDOKAI_FINALMATCH_INFO, DIALOG_SIDEDIALOG_CONTROLLER );
}