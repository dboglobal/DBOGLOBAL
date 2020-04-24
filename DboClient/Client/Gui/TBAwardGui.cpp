#include "precomp_dboclient.h"
#include "TBAwardGui.h"

// core
#include "NtlDebug.h"
#include "NtlLuaState.h"

// share

// table
#include "ItemTable.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlWorldConceptTB.h"
#include "NtlSLLuaGlue.h"

// client
#include "DboGlobal.h"
#include "DboEvent.h"
#include "TBGui.h"
#include "InfoWndManager.h"
#include "TBCommonDirectionGui.h"
#include "ResultNarrationGui.h"
#include "CounterGui.h"
#include "DialogPriority.h"

#define BUDOKAI_AWARD_NAME_ORIGIN_X		332
#define BUDOKAI_AWARD_NAME_ORIGIN_Y		165
#define BUDOKAI_AWARD_NAME_WIDTH		208
#define BUDOKAI_AWARD_NAME_HEIGHT		69
#define BUDOKAI_AWARD_POINT_ORIGIN_X	667
#define BUDOKAI_AWARD_POINT_ORIGIN_Y	189
#define BUDOKAI_AWARD_OFFSET_Y			78

#define BUDOKAI_AWARD_SLOT_X			846
#define BUDOKAI_AWARD_SLOT_Y_1			183
#define BUDOKAI_AWARD_SLOT_Y_2			263
#define BUDOKAI_AWARD_SLOT_Y_3_1		341
#define BUDOKAI_AWARD_SLOT_Y_3_2		418

CTBAwardResultGui::CTBAwardResultGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CTBAwardResultGui::~CTBAwardResultGui(VOID)
{

}

VOID CTBAwardResultGui::Init(VOID)
{
	m_pflaAward = NULL;
	m_nMouseOnIndex = -1;
	m_fWaitTime = 0.0f;	
	
	for( RwInt32 i = 0 ; i < MAX_FINALMATCH_RESULT ; ++i )
	{
		m_stAwardItem[i].m_pName = NULL;
		m_stAwardItem[i].m_pRewardItem = NULL;
		m_stAwardItem[i].m_pstbStackCount = NULL;
	}
}

RwBool CTBAwardResultGui::Create()
{
	NTL_FUNCTION( "CTBAwardResultGui::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\BudokaiAward.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain" ) );
	m_pflaAward = reinterpret_cast<gui::CFlash*>( GetComponent( "flaAward" ) );
	m_pflaEffect = reinterpret_cast<gui::CFlash*>( GetComponent( "flaEffect" ) );
	m_pflaAward->PlayMovie( FALSE );
	m_pflaEffect->PlayMovie( FALSE );

	m_pThis->SetPriority( dDIALOGPRIORITY_RESULT );

	CRectangle rtScreen = m_pThis->GetScreenRect();
	CRectangle rtRect;

	m_stAwardItem[FINALMATCH_RESULT_WINNER].m_rtRewardItem.SetRectWH( BUDOKAI_AWARD_SLOT_X, BUDOKAI_AWARD_SLOT_Y_1, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_stAwardItem[FINALMATCH_RESULT_SEMIWINNER].m_rtRewardItem.SetRectWH( BUDOKAI_AWARD_SLOT_X, BUDOKAI_AWARD_SLOT_Y_2, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_stAwardItem[FINALMATCH_RESULT_MATCH1_LOSER].m_rtRewardItem.SetRectWH( BUDOKAI_AWARD_SLOT_X, BUDOKAI_AWARD_SLOT_Y_3_1, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_stAwardItem[FINALMATCH_RESULT_MATCH2_LOSER].m_rtRewardItem.SetRectWH( BUDOKAI_AWARD_SLOT_X, BUDOKAI_AWARD_SLOT_Y_3_2, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );

	for( RwInt32 i = 0 ; i < MAX_FINALMATCH_RESULT ; ++i )
	{
		rtRect.SetRectWH( BUDOKAI_AWARD_NAME_ORIGIN_X, BUDOKAI_AWARD_NAME_ORIGIN_Y + BUDOKAI_AWARD_OFFSET_Y * i, BUDOKAI_AWARD_NAME_WIDTH, BUDOKAI_AWARD_NAME_HEIGHT );
		m_stAwardItem[i].m_pName = NTL_NEW gui::CStaticBox( rtRect, m_pflaAward, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
		m_stAwardItem[i].m_pName->CreateFontStd( DEFAULT_FONT, 120, DEFAULT_FONT_ATTR );
		m_stAwardItem[i].m_pName->Show( false );

		m_stAwardItem[i].m_numPoint.Create( ND_CENTER, NVD_TOP, 0, dNCFLAG_PLUS );
		m_stAwardItem[i].m_numPoint.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_0" ) );
		m_stAwardItem[i].m_numPoint.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_1" ) );
		m_stAwardItem[i].m_numPoint.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_2" ) );
		m_stAwardItem[i].m_numPoint.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_3" ) );
		m_stAwardItem[i].m_numPoint.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_4" ) );
		m_stAwardItem[i].m_numPoint.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_5" ) );
		m_stAwardItem[i].m_numPoint.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_6" ) );
		m_stAwardItem[i].m_numPoint.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_7" ) );
		m_stAwardItem[i].m_numPoint.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_8" ) );
		m_stAwardItem[i].m_numPoint.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_9" ) );
		m_stAwardItem[i].m_numPoint.SetSurface( NC_PLUS, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTB_Plus" ) );
		m_stAwardItem[i].m_numPoint.SetParentPos( BUDOKAI_AWARD_POINT_ORIGIN_X, BUDOKAI_AWARD_POINT_ORIGIN_Y + BUDOKAI_AWARD_OFFSET_Y * i );
		m_stAwardItem[i].m_numPoint.SetPosition( rtScreen.left, rtScreen.top );
		m_stAwardItem[i].m_numPoint.Show( FALSE );

		m_stAwardItem[i].m_surIcon.Show( FALSE );
		m_stAwardItem[i].m_surIcon.SetRect( m_stAwardItem[i].m_rtRewardItem );
	}
	
	m_surFocusSlot.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_surFocusSlot.SetSize( DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_surFocusSlot.Show( FALSE );

	m_slotPaint = m_pflaAward->SigPaint().Connect( this, &CTBAwardResultGui::OnPaint );
	m_slotMove = m_pflaAward->SigMove().Connect( this, &CTBAwardResultGui::OnMove );
	m_slotMouseMove = m_pflaAward->SigMouseMove().Connect( this, &CTBAwardResultGui::OnMouseMove );
	m_slotMouseLeave = m_pflaAward->SigMouseLeave().Connect( this, &CTBAwardResultGui::OnMouseLeave );
	m_slotFSCallBack = m_pflaAward->SigFSCallBack().Connect( this, &CTBAwardResultGui::OnFSCallBack );

	CNtlPLGui::Show( false );
	GetNtlGuiManager()->AddUpdateFunc( this );

	LinkMsg( g_EventResize );	
	LinkMsg( g_EventCinematicGuiCommand );
	LinkMsg( g_EventAwardInfo );
	
	NTL_RETURN( TRUE );
}

VOID CTBAwardResultGui::Destroy(VOID)
{
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	OnMouseLeave( NULL ); // 인포윈도우 닫기.

	DeleteAllRewardItem(); // 아이템 아이콘 삭제.

	UnLinkMsg( g_EventResize );
	UnLinkMsg( g_EventCinematicGuiCommand );
	UnLinkMsg( g_EventAwardInfo );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CTBAwardResultGui::Update( RwReal fElapsed )
{
	if( m_pThis->IsVisible() )
	{
		m_pflaAward->Update( fElapsed );
		m_pflaEffect->Update( fElapsed );
		
		for( RwInt32 i = 0 ; i < MAX_FINALMATCH_RESULT ; ++i )
		{
			m_stAwardItem[i].m_numPoint.Update( fElapsed );
		}
	}	

	if( m_fWaitTime > 0.0f )
	{
		m_fWaitTime -= fElapsed;

		if( m_fWaitTime <= TB_MATCH_RESULT_REMAIN_TIME && !CTBGui::GetInstance()->m_pCounter->IsShow() )
			CTBGui::GetInstance()->SetCounterTime( m_fWaitTime );		
	}
	else if( IsShow() )
		Show( false );	
}

VOID CTBAwardResultGui::Show( bool bOpen )
{
	CNtlPLGui::Show( bOpen );

	if( bOpen )
	{
		CenterAlign( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );		
		m_pflaAward->PlayMovie( TRUE );
		m_pflaEffect->PlayMovie( TRUE );
		CTBGui::GetInstance()->m_pResultNarration->SetData( "TB_MatchFinish_npc.swf", GetDisplayStringManager()->GetString( "DST_BUDOKAI_RESULT_MESSAGE" ) );				
	}
	else
	{
		CTBGui::GetInstance()->m_pResultNarration->Show( false );
		CTBGui::GetInstance()->HideCounterTime();
		m_fWaitTime = 0.0f;		
	}
}

VOID CTBAwardResultGui::CalcResize( RwUInt32 uiWidth, RwUInt32 uiHeight )
{
	m_pThis->SetSize( uiWidth, uiHeight );

	m_pflaAward->SetPosition( ( uiWidth - m_pflaAward->GetWidth() ) / 2, ( uiHeight - m_pflaAward->GetHeight() ) / 2 );
	m_pflaEffect->SetSize( uiWidth, uiHeight );
}

VOID CTBAwardResultGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventCinematicGuiCommand )
	{
		SNtlEventCinematicGuiCommand* pData = reinterpret_cast<SNtlEventCinematicGuiCommand*>( msg.pData );

		if( !strcmp( pData->szCommand, GetTBFlashName()->GetLuaAwardInfoCallBack() ) )
		{
			if( pData->bShow )
				Show( true );			
			else
				Show( false );
		}
	}
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );

		CalcResize( pData->iWidht, pData->iHeight );
	}
	else if( msg.Id == g_EventAwardInfo )
	{
		SNtlEventAwardInfo* pData = reinterpret_cast<SNtlEventAwardInfo*>( msg.pData );

		SetResultData( pData->paTeamNameIndex, pData->pData );
	}
}

VOID CTBAwardResultGui::SetResultData( RwUInt16* paTeanNameIndex, VOID* pData )
{
	CNtlTBudokai* pTBudokai = GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai();
	sBUDOKAI_TBLINFO* pBudokaiData = pTBudokai->GetBudokaiTblData();
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		
	WCHAR wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
	sVARIABLE_DATA* pVarData = reinterpret_cast<sVARIABLE_DATA*>( pData );

	for( RwUInt8 i = 0 ; i < MAX_FINALMATCH_RESULT ; ++i )
	{
		memset( wszTeamName, 0, sizeof( wszTeamName ) );
		memcpy( wszTeamName, pVarData->Read( paTeanNameIndex[i] ), pVarData->GetSize( paTeanNameIndex[i] ) );

		m_stAwardItem[i].m_pName->SetText( wszTeamName );
		
		sAWARDING_REWARD::sREWARD* pReward = NULL; 

		if( pTBWorldConcept->IsPartyBattle() )
		{
			if( GetBudokaiType( GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiMainState() ) == BUDOKAI_TYPE_JUNIOR )
			{
				switch( i )
				{
				case FINALMATCH_RESULT_WINNER: pReward = &pBudokaiData->sJuniorTeamReward.sAwarding.sWinner; break;
				case FINALMATCH_RESULT_SEMIWINNER: pReward = &pBudokaiData->sJuniorTeamReward.sAwarding.sSemiWinner; break;
				case FINALMATCH_RESULT_MATCH1_LOSER: 
				case FINALMATCH_RESULT_MATCH2_LOSER: pReward = &pBudokaiData->sJuniorTeamReward.sAwarding.sOther;	break;			
				}
			}
			else
			{
				switch( i )
				{
				case FINALMATCH_RESULT_WINNER: pReward = &pBudokaiData->sTeamReward.sAwarding.sWinner; break;
				case FINALMATCH_RESULT_SEMIWINNER: pReward = &pBudokaiData->sTeamReward.sAwarding.sSemiWinner; break;
				case FINALMATCH_RESULT_MATCH1_LOSER: 
				case FINALMATCH_RESULT_MATCH2_LOSER: pReward = &pBudokaiData->sTeamReward.sAwarding.sOther;	break;			
				}
			}
		}
		else
		{
			if( GetBudokaiType( GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiMainState() ) == BUDOKAI_TYPE_JUNIOR )
			{
				switch( i )
				{
				case FINALMATCH_RESULT_WINNER: pReward = &pBudokaiData->sJuniorIndividualReward.sAwarding.sWinner; break;
				case FINALMATCH_RESULT_SEMIWINNER: pReward = &pBudokaiData->sJuniorIndividualReward.sAwarding.sSemiWinner; break;
				case FINALMATCH_RESULT_MATCH1_LOSER: 
				case FINALMATCH_RESULT_MATCH2_LOSER: pReward = &pBudokaiData->sJuniorIndividualReward.sAwarding.sOther;	break;			
				}
			}
			else
			{
				switch( i )
				{
				case FINALMATCH_RESULT_WINNER: pReward = &pBudokaiData->sIndividualReward.sAwarding.sWinner; break;
				case FINALMATCH_RESULT_SEMIWINNER: pReward = &pBudokaiData->sIndividualReward.sAwarding.sSemiWinner; break;
				case FINALMATCH_RESULT_MATCH1_LOSER: 
				case FINALMATCH_RESULT_MATCH2_LOSER: pReward = &pBudokaiData->sIndividualReward.sAwarding.sOther; break;			
				}
			}
		}

		if( pReward )
		{
			SetRewardItem( i, pReward->itemTblidx, pReward->byStackCount );		
			m_stAwardItem[i].m_numPoint.SetNumber( pReward->dwMudosaPoint );
		}		
		else
		{
			DeleteRewardItem( i );
			m_stAwardItem[i].m_numPoint.SetNumber( 0 );
		}
	}

	m_fWaitTime = pTBudokai->GetBudokaiTblData()->dwAwardingTime + 0.99f;	
}

VOID CTBAwardResultGui::SetRewardItem( RwUInt8 bySlotIdx, RwUInt32 tblidx, RwUInt8 byStackCount )
{
	CItemTable* pItemTable = API_GetTableContainer()->GetItemTable();
	sITEM_TBLDAT* pTableData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData( tblidx ) );

	if( pTableData )
	{
		m_stAwardItem[bySlotIdx].m_surIcon.SetTexture( Logic_CreateTexture( const_cast<RwChar*>( pTableData->szIcon_Name ) ) );
		if( pTableData->byMax_Stack > 1 )
			CreateStackNumber( bySlotIdx, byStackCount );			

		m_stAwardItem[bySlotIdx].m_pRewardItem = pTableData;		
	}
}

VOID CTBAwardResultGui::DeleteRewardItem( RwUInt8 bySlotIdx )
{
	m_stAwardItem[bySlotIdx].m_pRewardItem = NULL;
	m_stAwardItem[bySlotIdx].m_surIcon.Show( FALSE );
	Logic_DeleteTexture( m_stAwardItem[bySlotIdx].m_surIcon.GetTexture() );
	m_stAwardItem[bySlotIdx].m_surIcon.UnsetTexture();
	DeleteStackNumber( bySlotIdx );	
}

VOID CTBAwardResultGui::DeleteAllRewardItem(VOID)
{
	for( RwUInt8 i = 0 ; i < MAX_FINALMATCH_RESULT ; ++i )
	{
		DeleteRewardItem( i );
	}
}

RwBool CTBAwardResultGui::CreateStackNumber( RwUInt8 bySlotIdx, RwUInt8 uiStackCount )
{
	NTL_FUNCTION( "CTBAwardResultGui::CreateStackNubmer" );

	CRectangle rect;
	rect.SetRect( m_stAwardItem[bySlotIdx].m_rtRewardItem.left, m_stAwardItem[bySlotIdx].m_rtRewardItem.bottom - DBOGUI_STACKNUM_HEIGHT,
				  m_stAwardItem[bySlotIdx].m_rtRewardItem.left + DBOGUI_STACKNUM_WIDTH, m_stAwardItem[bySlotIdx].m_rtRewardItem.bottom );
	m_stAwardItem[bySlotIdx].m_pstbStackCount = NTL_NEW gui::CStaticBox( rect, m_pflaAward, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN );
	if( !m_stAwardItem[bySlotIdx].m_pstbStackCount )
		NTL_RETURN( FALSE );

	m_stAwardItem[bySlotIdx].m_pstbStackCount->CreateFontStd( DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR );
	m_stAwardItem[bySlotIdx].m_pstbStackCount->SetEffectMode( DBOGUI_STACKNUM_FONTEFFECTMODE );
	m_stAwardItem[bySlotIdx].m_pstbStackCount->SetText( uiStackCount );
	m_stAwardItem[bySlotIdx].m_pstbStackCount->Enable( false );
	m_stAwardItem[bySlotIdx].m_pstbStackCount->Show( false );

	NTL_RETURN( TRUE );
}

VOID CTBAwardResultGui::DeleteStackNumber( RwUInt8 bySlotIdx )
{
	if( m_stAwardItem[bySlotIdx].m_pstbStackCount )
		NTL_DELETE( m_stAwardItem[bySlotIdx].m_pstbStackCount );
}

VOID CTBAwardResultGui::SetMouseOnIndex( RwInt32 nIndex )
{
	NTL_ASSERT( nIndex < MAX_FINALMATCH_RESULT, "CTBAwardResultGui::SetMouseOnIndex : Invalid Array Index" );

	m_nMouseOnIndex = nIndex;

	if( nIndex >= 0 )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		m_surFocusSlot.SetPosition( rtScreen.left + m_stAwardItem[nIndex].m_rtRewardItem.left, rtScreen.top + m_stAwardItem[nIndex].m_rtRewardItem.top );
		m_surFocusSlot.Show( TRUE );
	}
	else
		m_surFocusSlot.Show( FALSE );

}

RwInt32 CTBAwardResultGui::GetRewardSlotIndex( RwInt32 nX, RwInt32 nY )
{
	for( RwInt32 i = 0 ; i < MAX_FINALMATCH_RESULT ; ++i )
	{
		if( m_stAwardItem[i].m_pRewardItem &&
			m_stAwardItem[i].m_rtRewardItem.PtInRect( nX, nY ) )
			return i;
	}

	return -1;
}

VOID CTBAwardResultGui::ShowAwardItem( RwUInt8 bySlotIdx )
{
	m_stAwardItem[bySlotIdx].m_pName->Show( true );
	m_stAwardItem[bySlotIdx].m_surIcon.Show( TRUE );
	m_stAwardItem[bySlotIdx].m_numPoint.Show( TRUE );	
	if( m_stAwardItem[bySlotIdx].m_pstbStackCount )
		m_stAwardItem[bySlotIdx].m_pstbStackCount->Show( true );
}

VOID CTBAwardResultGui::OnFSCallBack( const RwChar* szCommand, const RwChar* szArgs )
{
	if( !strcmp( szCommand, "TextEnd1" ) )	// 3,4위
	{
		ShowAwardItem( FINALMATCH_RESULT_MATCH1_LOSER );
		ShowAwardItem( FINALMATCH_RESULT_MATCH2_LOSER );
	}
	else if( !strcmp( szCommand, "TextEnd2" ) ) // 2위
	{
		ShowAwardItem( FINALMATCH_RESULT_SEMIWINNER );
	}
	else if( !strcmp( szCommand, "TextEnd3" ) )	// 1위
	{
		ShowAwardItem( FINALMATCH_RESULT_WINNER );
	}
}

VOID CTBAwardResultGui::OnPaint(VOID)
{
	for( RwInt32 i = 0 ; i < MAX_FINALMATCH_RESULT ; ++i )
	{
		m_stAwardItem[i].m_surIcon.Render();
		m_stAwardItem[i].m_numPoint.Render();		
	}

	m_surFocusSlot.Render();
}

VOID CTBAwardResultGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_pflaAward->GetScreenRect();

	for( RwInt32 i = 0 ; i < MAX_FINALMATCH_RESULT ; ++i )
	{
		m_stAwardItem[i].m_numPoint.SetPosition( rtScreen.left, rtScreen.top );
		m_stAwardItem[i].m_surIcon.SetPosition( rtScreen.left + m_stAwardItem[i].m_rtRewardItem.left, rtScreen.top + m_stAwardItem[i].m_rtRewardItem.top );
	}

	SetMouseOnIndex( m_nMouseOnIndex );
}

VOID CTBAwardResultGui::OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY )
{
	RwInt32 nMouseOnIdx = GetRewardSlotIndex( nX, nY );

	if( nMouseOnIdx >= 0 )
	{
		if( m_nMouseOnIndex != nMouseOnIdx )
		{
			CRectangle rtScreen = m_pThis->GetScreenRect();
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_TABLE_ITEM, m_stAwardItem[nMouseOnIdx].m_rtRewardItem.left + rtScreen.left, m_stAwardItem[nMouseOnIdx].m_rtRewardItem.top + rtScreen.top, m_stAwardItem[nMouseOnIdx].m_pRewardItem, DIALOG_UNKNOWN );
			SetMouseOnIndex( nMouseOnIdx );
		}
	}
	else
	{
		if( m_nMouseOnIndex >= 0 )
		{
			GetInfoWndManager()->ShowInfoWindow( FALSE );
			SetMouseOnIndex( -1 );
		}
	}
}

VOID CTBAwardResultGui::OnMouseLeave( gui::CComponent* pComponent )
{
	if( m_nMouseOnIndex >= 0 )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		SetMouseOnIndex( -1 );
	}
}