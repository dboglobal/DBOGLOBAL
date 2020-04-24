#include "precomp_dboclient.h"
#include "TBTournamentGui.h"

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

std::map<RwUInt16,sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO> CTBTournamentGui::m_mapIndividualEntryInfo;
std::map<RwUInt16,sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO> CTBTournamentGui::m_mapTeamEntryInfo;
std::map<RwUInt16,sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA> CTBTournamentGui::m_mapIndividualEntryData;
std::map<RwUInt16,sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA> CTBTournamentGui::m_mapTeamEntryData;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBTournamentFightInfo

CTBTournamentFightInfo::CTBTournamentFightInfo( CTBTournamentFightInfo* pAboveNode, RwUInt8 byMatchNumber, RwUInt8 byMatchLevel, RwBool bParty )
: m_byMatchLevel( byMatchLevel ), m_byMatchNumber( byMatchNumber ), m_nEndOfBelowAnimation( 0 ), m_wWinnerTeamJoinID( INVALID_WORD ), m_pAboveFightInfo( pAboveNode ), m_bParty( bParty ), m_byMatchResult( INVALID_MATCH_RESULT )
{

}

CTBTournamentFightInfo::~CTBTournamentFightInfo(VOID)
{

}

VOID CTBTournamentFightInfo::SetFightResult( RwUInt8 byMatchResult, RwUInt16 wWinnerJoinID, RwUInt16 wJoinID1, RwUInt16 wJoinID2, RwUInt8 byScore1, RwUInt8 byScore2 )
{
	m_byMatchResult = byMatchResult;
	m_wWinnerTeamJoinID = wWinnerJoinID;
	m_stEntry[0].wJoinID = wJoinID1;
	m_stEntry[1].wJoinID = wJoinID2;
	m_stEntry[0].byScore = byScore1;
	m_stEntry[1].byScore = byScore2;

	if( MATCH_RESULT_WIN != byMatchResult && MATCH_RESULT_UNEARNED_WIN != byMatchResult )		
		EndOfAnimation();
}

VOID CTBTournamentFightInfo::SetEntryData( RwUInt8 byIdx, RwUInt16 wJoinID )
{
	NTL_ASSERT( byIdx >= 0 && byIdx < 2, "CTBTournamentFightInfo::SetEntryData : Invalid Index" );

	m_stEntry[byIdx].wJoinID = wJoinID;	
	m_stEntry[byIdx].bEntry = TRUE;	
}

VOID CTBTournamentFightInfo::ClickFightInfo(VOID)
{
	RwUInt16 wLeftJoinID = INVALID_WORD;
	RwUInt16 wRightJoinID = INVALID_WORD;

	CTBTournamentMatchInfoGui* pMatchInfoGui = reinterpret_cast<CTBTournamentMatchInfoGui*>( GetDialogManager()->GetDialog( DIALOG_BUDOKAI_TOURNAMENT_MATCHINFO ) );
	pMatchInfoGui->SetMatchData( m_byMatchLevel, m_byMatchNumber, m_stEntry[MATCH_TEAM_TYPE_TEAM1].byScore, m_stEntry[MATCH_TEAM_TYPE_TEAM2].byScore );
	GetDialogManager()->OpenDialog( DIALOG_BUDOKAI_TOURNAMENT_MATCHINFO );

	if( m_bParty )
	{
		sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* pLeftData = NULL;
		sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* pRightData = NULL;

		if( m_stEntry[MATCH_TEAM_TYPE_TEAM1].wJoinID != INVALID_WORD )
		{
			pLeftData =	CTBTournamentGui::FindTeamEntryData( m_stEntry[MATCH_TEAM_TYPE_TEAM1].wJoinID );

			if( !pLeftData )
				wLeftJoinID = m_stEntry[MATCH_TEAM_TYPE_TEAM1].wJoinID;
			else
				pMatchInfoGui->SetTeamData( pLeftData, MATCH_TEAM_TYPE_TEAM1 );				
		}

		if( m_stEntry[MATCH_TEAM_TYPE_TEAM2].wJoinID != INVALID_WORD )
		{
			pRightData = CTBTournamentGui::FindTeamEntryData( m_stEntry[MATCH_TEAM_TYPE_TEAM2].wJoinID );

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
			pLeftData =	CTBTournamentGui::FindIndividualEntryData( m_stEntry[MATCH_TEAM_TYPE_TEAM1].wJoinID );

			if( !pLeftData )
				wLeftJoinID = m_stEntry[MATCH_TEAM_TYPE_TEAM1].wJoinID;
			else
				pMatchInfoGui->SetIndividualData( pLeftData, MATCH_TEAM_TYPE_TEAM1 );				
		}

		if( m_stEntry[MATCH_TEAM_TYPE_TEAM2].wJoinID != INVALID_WORD )
		{
			pRightData = CTBTournamentGui::FindIndividualEntryData( m_stEntry[MATCH_TEAM_TYPE_TEAM2].wJoinID );

			if( !pRightData )
				wRightJoinID = m_stEntry[MATCH_TEAM_TYPE_TEAM2].wJoinID;
			else
				pMatchInfoGui->SetIndividualData( pRightData, MATCH_TEAM_TYPE_TEAM2 );					
		}

		if( wLeftJoinID != INVALID_WORD || wRightJoinID != INVALID_WORD )
			GetDboGlobal()->GetChatPacketGenerator()->SendBudokaiTournamentIndividualInfoReq( m_stEntry[0].wJoinID, m_stEntry[1].wJoinID );
	}	
}

VOID CTBTournamentFightInfo::ShowNameToInfoWnd( RwUInt8 byTeamPos )
{
	sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO* pData = NULL;

	if( m_bParty )
	{
		pData = CTBTournamentGui::FindTeamEntryInfo( m_stEntry[byTeamPos].wJoinID );		
	}
	else
	{
		pData = CTBTournamentGui::FindIndividualEntryInfo( m_stEntry[byTeamPos].wJoinID );	
	}

	if( pData )	
	{
		if( pData->wszTeamName && wcslen( pData->wszTeamName ) )
		{
			if( pData->bDojoRecommender )
			{
				WCHAR buf[GUI_TEXT_BUFFER_SIZE];
				swprintf_s( buf, GUI_TEXT_BUFFER_SIZE, L"%s\\n%s", GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_DOJO_RECOMMAND" ), pData->wszTeamName );
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, CMouse::GetX(), CMouse::GetY(), (VOID*)buf, DIALOG_BUDOKAI_TOURNAMENT );	
			}
			else
			{
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, CMouse::GetX(), CMouse::GetY(), (VOID*)pData->wszTeamName, DIALOG_BUDOKAI_TOURNAMENT );	
			}
		}		
	}
}

VOID CTBTournamentFightInfo::EndOfAnimation(VOID)
{
	if( m_pAboveFightInfo )
		m_pAboveFightInfo->SetEntryData( (RwUInt8)( m_byMatchNumber % 2 ), m_wWinnerTeamJoinID );	
}

RwUInt8 CTBTournamentFightInfo::GetWinnerPosition(VOID)
{
	if( m_byMatchResult == MATCH_RESULT_WIN ||
		m_byMatchResult == MATCH_RESULT_UNEARNED_WIN )
	{
		for( RwUInt8 i = 0 ; i < MAX_MATCH_TEAM_TYPE_COUNT ; ++i )
		{
			if( m_stEntry[i].wJoinID == m_wWinnerTeamJoinID )
				return i;
		}
	}

	return INVALID_BYTE;
}

//RwInt32 CTBTournamentFightInfo::GetHorizontalOrigin(VOID)
//{
//	RwInt32 nOrigin = m_nOriginLeft;
//	RwInt32 nOffsetValue = 0;
//
//	for( RwInt32 i = 0 ; i < m_byMatchLevel ; ++i )
//	{
//		nOrigin += (RwInt32)pow( (RwReal)2, i ) * m_nHorizontalOffset;
//	}
//
//	return nOrigin;
//}
//
//RwInt32 CTBTournamentFightInfo::GetHorizontalOffset(VOID)
//{
//	return (RwInt32)pow( (RwReal)2, m_byMatchLevel + 1 ) * m_nHorizontalOffset;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBTournamentInfoGroup

CTBTournamentInfoGroup::CTBTournamentInfoGroup( RwUInt8 byPosOffset, RwInt32 nEntryCount, RwUInt8 byTopOfDepth, RwUInt8 byEndOfDepth, RwBool bParty, gui::CFlash* pFlash, gui::CStaticBox* pWinnerName, gui::CStaticBox* pWinnerEmblem )
: m_byPosOffset( byPosOffset ), m_nEntryCount( nEntryCount ), m_nFightInfoCount( nEntryCount - 1 ), m_pFlash( pFlash ), m_pWinnerName( pWinnerName ), m_pWinnerEmblem( pWinnerEmblem ), m_bParty( bParty ), m_byTopOfDepth( byTopOfDepth ), m_byEndOfDepth( byEndOfDepth )
{
	m_ppFightInfo = NTL_NEW CTBTournamentFightInfo*[m_nFightInfoCount];	
	for( RwInt32 i = 0 ; i < m_nFightInfoCount ; ++i )
	{
		m_ppFightInfo[i] = NULL;
	}

	m_slotFSCallBack = pFlash->SigFSCallBack().Connect( this, &CTBTournamentInfoGroup::OnFSCallBack );
}

CTBTournamentInfoGroup::~CTBTournamentInfoGroup(VOID)
{
	RwInt32 i;

	for( i = 0 ; i < m_nFightInfoCount ; ++i )
		NTL_DELETE( m_ppFightInfo[i] );

	NTL_ARRAY_DELETE( m_ppFightInfo );
}

VOID CTBTournamentInfoGroup::Update( RwReal fElapsed )
{
	if( m_pFlash->IsVisible() )
		m_pFlash->Update( fElapsed );
}

VOID CTBTournamentInfoGroup::Show( bool bShow )
{
	m_pFlash->Show( bShow );
}

VOID CTBTournamentInfoGroup::CreateFightData( RwUInt8 byIndex, CTBTournamentFightInfo* pAboveFightInfo, RwUInt8 byMatchLevel, RwUInt8 byMatchNumber )
{
	NTL_ASSERT( byIndex >= 0 && byIndex < m_nFightInfoCount, "CTBTournamentInfoGroup::CreateFightData : Invalid Index" );
	NTL_ASSERT( m_ppFightInfo[byIndex] == NULL, "CTBTournamentInfoGroup::CreateFightData : No Allowed Complexed Index" );

	m_ppFightInfo[byIndex] = NTL_NEW CTBTournamentFightInfo( pAboveFightInfo, byMatchNumber, byMatchLevel, m_bParty );	
}

VOID CTBTournamentInfoGroup::SetEntryData( RwUInt8 byIndex, RwUInt16 wJoinID )
{
	RwUInt8 byDepth = m_byEndOfDepth;
	RwUInt8 byNumber = byIndex / 2;
	RwUInt8 byPos = byIndex % 2;
	CTBTournamentFightInfo* pFightInfo = FindFightInfo( byDepth, byNumber );

	if( pFightInfo )
	{		
		pFightInfo->SetEntryData( byPos, wJoinID );

		RwUInt16 wMyJoinID = INVALID_WORD;

		if( m_bParty )
			wMyJoinID = GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiJoinInfo()->sTeamInfo.wJoinId;
		else
			wMyJoinID = GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiJoinInfo()->sIndividualInfo.wJoinId;

		if( wMyJoinID == INVALID_WORD )
			return;

		if( wMyJoinID == wJoinID )
			FocusMyEntry( byDepth, byNumber, byPos );
	}
}

RwBool CTBTournamentInfoGroup::SetFightResultData( RwUInt8 byMatchLevel, RwUInt8 byMatchNumber, RwUInt16 wJoinID1, RwUInt16 wJoinID2, 
												  RwUInt8 byMatchReasult, RwUInt16 wWinnerTeam, RwUInt8 byScore1, RwUInt8 byScore2 )
{
	CTBTournamentFightInfo* pFightInfo = FindFightInfo( byMatchLevel, byMatchNumber );

	if( !pFightInfo )
		return FALSE;

	pFightInfo->SetFightResult( byMatchReasult, wWinnerTeam, wJoinID1, wJoinID2, byScore1, byScore2 );

	if( pFightInfo->IsEnableToStartAnimation() )
		DrawLine( byMatchLevel, byMatchNumber, pFightInfo->GetWinnerPosition() );		

	return TRUE;
}

CTBTournamentFightInfo* CTBTournamentInfoGroup::FindFightInfo( RwUInt8 byMatchLevel, RwUInt8 byMatchNumber )
{
	for( RwInt32 i = 0 ; i < m_nFightInfoCount ; ++i )
	{
		if( m_ppFightInfo[i] )
		{
			if( m_ppFightInfo[i]->GetMatchLevel() == byMatchLevel &&
				m_ppFightInfo[i]->GetMatchNumber() == byMatchNumber )				
				return m_ppFightInfo[i];
		}
	}

	return NULL;
}

CTBTournamentFightInfo*	CTBTournamentInfoGroup::GetDepthNumberFromFlashArgs( const RwChar* szArgs, RwUInt8* pDepth, RwUInt8* pNumber )
{
	std::string strArgs = szArgs;
	std::string::size_type pos;
	pos = strArgs.find( ',' );
	NTL_ASSERT( pos != std::string::npos, "CTBTournamentFightInfo::OnFSCallBack : Flash Argument is wrong" );

	*pDepth = (RwUInt8)atoi( strArgs.c_str() );		
	strArgs.erase( 0, pos + 1 );

	if( m_bParty )
		*pDepth -= 1;

	*pNumber = (RwUInt8)atoi( strArgs.c_str() );		

	if( m_byPosOffset )
	{
		RwUInt8 byTotalNumberOfDepth = (RwUInt8)pow( 2.0f, (RwReal)( (*pDepth) ) );
		*pNumber += byTotalNumberOfDepth / 2;
	}

	CTBTournamentFightInfo* pFightInfo = FindFightInfo( *pDepth, *pNumber );

	return pFightInfo;
}

CTBTournamentFightInfo*	CTBTournamentInfoGroup::GetDepthNumberTeamFromFlashArgs( const RwChar* szArgs, RwUInt8* pDepth, RwUInt8* pNumber, RwUInt8* pTeam )
{
	std::string strArgs = szArgs;
	std::string::size_type pos;
	pos = strArgs.find( ',' );
	NTL_ASSERT( pos != std::string::npos, "CTBTournamentFightInfo::OnFSCallBack : Flash Argument is wrong" );

	*pDepth = (RwUInt8)atoi( strArgs.c_str() );		
	strArgs.erase( 0, pos + 1 );

	if( m_bParty )
		*pDepth -= 1;

	*pNumber = (RwUInt8)atoi( strArgs.c_str() );		
	strArgs.erase( 0, pos + 1 );

	if( m_byPosOffset )
	{
		RwUInt8 byTotalNumberOfDepth = (RwUInt8)pow( 2.0f, (RwReal)( (*pDepth) ) );
		*pNumber += byTotalNumberOfDepth / 2;
	}

	*pTeam = (RwUInt8)atoi( strArgs.c_str() );

	NTL_ASSERT( *pTeam >= 0 && *pTeam < MAX_MATCH_TEAM_TYPE_COUNT, "CTBTournamentInfoGroup::GetDepthNumberTeamFromFlashArgs : Invalid Team" );

	CTBTournamentFightInfo* pFightInfo = FindFightInfo( *pDepth, *pNumber );

	return pFightInfo;
}

RwBool CTBTournamentInfoGroup::IsMyTeamInThisGroup( RwUInt16 wjoinID )
{
	if( wjoinID == INVALID_WORD )
		return FALSE;

	for( RwInt32 i = 0 ; i < m_nFightInfoCount ; ++i )
	{
		if( m_ppFightInfo[i] )
		{
			if( m_ppFightInfo[i]->GetLeftEntryID() == wjoinID ||
				m_ppFightInfo[i]->GetRightEntryID() == wjoinID )
				return TRUE;
		}
	}

	return FALSE;
}

VOID CTBTournamentInfoGroup::DrawLine( RwUInt8 byDepth, RwUInt8 byNumber, RwUInt8 byPos )
{
	if( byPos != MATCH_TEAM_TYPE_TEAM1 && byPos != MATCH_TEAM_TYPE_TEAM2 )
		return;

	if( m_bParty )
		byDepth += 1;

	if( m_byPosOffset )
	{
		RwUInt8 byTotalNumberOfDepth = (RwUInt8)pow( 2.0f, (RwReal)( byDepth ) );
		byNumber -= byTotalNumberOfDepth / 2;
	}	

	m_pFlash->Invoke( "Draw_Line", "%d,%d,%d", byDepth, byNumber, byPos );
}

VOID CTBTournamentInfoGroup::FocusMyEntry( RwUInt8 byDepth, RwUInt8 byNumber, RwUInt8 byPos )
{
	if( m_bParty )
		byDepth += 1;

	if( m_byPosOffset )
	{
		RwUInt8 byTotalNumberOfDepth = (RwUInt8)pow( 2.0f, (RwReal)( byDepth ) );
		byNumber -= byTotalNumberOfDepth / 2;
	}	

	m_pFlash->Invoke( "Select_Focus", "%d,%d,%d", byDepth, byNumber, byPos );
}

VOID CTBTournamentInfoGroup::OnFSCallBack( const RwChar* szCommand, const RwChar* szArgs )
{
	if( !strcmp( szCommand, "InfoButtonPress" ) )
	{
		RwUInt8 byDepth = INVALID_BYTE;
		RwUInt8 byNumber = INVALID_BYTE;

		CTBTournamentFightInfo* pFightInfo = GetDepthNumberFromFlashArgs( szArgs, &byDepth, &byNumber );

		if( pFightInfo )
			pFightInfo->ClickFightInfo();
	}
	else if( !strcmp( szCommand, "DrawEnd" ) )
	{
		RwUInt8 byDepth = INVALID_BYTE;
		RwUInt8 byNumber = INVALID_BYTE;
		RwUInt8 byTeam = INVALID_BYTE;

		CTBTournamentFightInfo* pFightInfo = GetDepthNumberTeamFromFlashArgs( szArgs, &byDepth, &byNumber, &byTeam );

		if( pFightInfo )
		{
			pFightInfo->EndOfAnimation();

			// 최상위 라인인 경우 이름을 출력
			if( byDepth == m_byTopOfDepth )
			{
				sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO* pData = NULL;

				if( m_bParty )
					pData = CTBTournamentGui::FindTeamEntryInfo( pFightInfo->GetWinnerID() );
				else
					pData = CTBTournamentGui::FindIndividualEntryInfo( pFightInfo->GetWinnerID() );

				if( pData )
				{
					m_pWinnerName->SetText( pData->wszTeamName );			

					if( pData->bDojoRecommender )
					{
						m_pWinnerEmblem->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_DOJO_RECOMMAND" ) );						
					}
					else
					{
						m_pWinnerEmblem->Clear();
					}
				}				
			}

			// 상위단의 모든 차일드의 애니메이션이 종료되었으면 상위단의 애니메이션 시작
			CTBTournamentFightInfo* pAboveFightInfo = pFightInfo->GetAboveFightInfo();
			if( pAboveFightInfo && pAboveFightInfo->IsEnableToStartAnimation() )
				DrawLine( pAboveFightInfo->GetMatchLevel(), pAboveFightInfo->GetMatchNumber(), pAboveFightInfo->GetWinnerPosition() );				
		}		
	}
	else if( !strcmp( szCommand, "EntryInfoRollOver" ) )
	{
		RwUInt8 byDepth = INVALID_BYTE;
		RwUInt8 byNumber = INVALID_BYTE;
		RwUInt8 byTeam = INVALID_BYTE;

		CTBTournamentFightInfo* pFightInfo = GetDepthNumberTeamFromFlashArgs( szArgs, &byDepth, &byNumber, &byTeam );

		if( pFightInfo )
			pFightInfo->ShowNameToInfoWnd( byTeam );
	}
	else if( !strcmp( szCommand, "EntryInfoRollOut" ) )
	{
		if( GetInfoWndManager()->GetRequestGui() == DIALOG_BUDOKAI_TOURNAMENT )
			GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBTournamentFinalInfoGroup

CTBTournamentFinalInfoGroup::CTBTournamentFinalInfoGroup( RwUInt8 byPosOffset, RwInt32 nEntryCount, RwUInt8 byTopOfDepth, RwUInt8 byEndOfDepth, RwBool bParty, gui::CFlash* pFlash, gui::CStaticBox* pWinnerName, gui::CStaticBox* pWinnerEmblem )
: CTBTournamentInfoGroup( byPosOffset, nEntryCount, byTopOfDepth, byEndOfDepth, bParty, pFlash, pWinnerName, pWinnerEmblem )
{
	CRectangle rect[NUM_NAME];
	rect[SEMIFINAL1_LEFT].SetRectWH( 25, 341, 155, 36 );
	rect[SEMIFINAL1_RIGHT].SetRectWH( 211, 341, 155, 36 );
	rect[SEMIFINAL2_LEFT].SetRectWH( 397, 341, 155, 36 );
	rect[SEMIFINAL2_RIGHT].SetRectWH( 583, 341, 155, 36 );
	rect[FINAL_LEFT].SetRectWH( 126, 197, 155, 36 );
	rect[FINAL_RIGHT].SetRectWH( 485, 197, 155, 36 );

	for( RwInt32 i = 0 ; i < NUM_NAME ; ++i )
	{
		m_pName[i] = NTL_NEW gui::CStaticBox( rect[i], m_pFlash, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
		m_pName[i]->CreateFontStd( DEFAULT_FONT, 120, DEFAULT_FONT_ATTR );
		m_pName[i]->Show( false );

		CRectangle rectEmblem;
		rectEmblem.SetRectWH( rect[i].left, rect[i].top - 12, rect[i].GetWidth(), 12 );
		m_pEmblem[i] = NTL_NEW gui::CStaticBox( rectEmblem, m_pFlash, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
		m_pEmblem[i]->CreateFontStd( DEFAULT_FONT, 90, DEFAULT_FONT_ATTR );
		m_pEmblem[i]->Show( false );
	}
	m_pWinnerName->Show( false );
	m_pWinnerEmblem->Show( false );
	
	CRectangle rectWinner;
	rectWinner.SetRectWH( 306, 72, 155, 36 );
	m_pWinnerName->SetPosition( rectWinner );

	m_slotFinalFSCallBack = m_pFlash->SigFSCallBack().Connect( this, &CTBTournamentFinalInfoGroup::OnFinalFSCallBack );
}

CTBTournamentFinalInfoGroup::~CTBTournamentFinalInfoGroup(VOID)
{

}

VOID CTBTournamentFinalInfoGroup::CreateFightData( RwUInt8 byIndex, CTBTournamentFightInfo *pAboveFightInfo, RwUInt8 byMatchLevel, RwUInt8 byMatchNumber )
{
	CTBTournamentInfoGroup::CreateFightData( byIndex, pAboveFightInfo, byMatchLevel, byMatchNumber );

	// 맨 밑단의 녀석은 Entry True로 만들어주기 위해. 결승전 전용.
	if( byMatchLevel == m_byEndOfDepth )
	{
		m_ppFightInfo[byIndex]->SetEntryData( MATCH_TEAM_TYPE_TEAM1, INVALID_WORD );
		m_ppFightInfo[byIndex]->SetEntryData( MATCH_TEAM_TYPE_TEAM2, INVALID_WORD );
	}
}

RwBool CTBTournamentFinalInfoGroup::SetFightResultData( RwUInt8 byMatchLevel, RwUInt8 byMatchNumber, RwUInt16 wJoinID1, RwUInt16 wJoinID2, 
													   RwUInt8 byMatchResult, RwUInt16 wWinnerTeam, RwUInt8 byScore1, RwUInt8 byScore2 )
{
	if( byMatchLevel - 1 == m_byEndOfDepth )
	{
		sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO* pData = NULL;

		if( m_bParty )
			pData = CTBTournamentGui::FindTeamEntryInfo( wWinnerTeam );			
		else
			pData = CTBTournamentGui::FindIndividualEntryInfo( wWinnerTeam );

		if( pData )
		{
			m_pName[byMatchNumber]->SetText( pData->wszTeamName );						

			if( pData->bDojoRecommender )
			{
				m_pEmblem[byMatchNumber]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_DOJO_RECOMMAND" ) );				
			}
			else
			{
				m_pEmblem[byMatchNumber]->Clear();
			}
		}		

		RwUInt8 byEndMatchNumber = byMatchNumber / 2;
		CTBTournamentFightInfo* pFightInfo = FindFightInfo( m_byEndOfDepth, byEndMatchNumber );

		if( pFightInfo )
			pFightInfo->SetEntryData( byMatchNumber % 2 , wWinnerTeam );							
	}
	else if( byMatchLevel <= m_byEndOfDepth )
		return CTBTournamentInfoGroup::SetFightResultData( byMatchLevel, byMatchNumber, wJoinID1, wJoinID2, byMatchResult, wWinnerTeam, byScore1, byScore2 );

	return TRUE;
}

VOID CTBTournamentFinalInfoGroup::OnFinalFSCallBack( const RwChar* szCommand, const RwChar* szArgs )
{
	if( !strcmp( szCommand, "DrawEnd" ) )
	{	
		RwUInt8 byDepth = INVALID_BYTE;
		RwUInt8 byNumber = INVALID_BYTE;
		RwUInt8 byTeam = INVALID_BYTE;

		CTBTournamentFightInfo* pFightInfo = GetDepthNumberTeamFromFlashArgs( szArgs, &byDepth, &byNumber, &byTeam );

		// 애니메이션이 끝나면 해당하는 선수 이름 출력
		if( pFightInfo && byDepth == BUDOKAI_MATCH_DEPTH_4 )
		{
			sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO* pData = NULL;

			if( m_bParty )
				pData = CTBTournamentGui::FindTeamEntryInfo( pFightInfo->GetWinnerID() );
			else
				pData = CTBTournamentGui::FindIndividualEntryInfo( pFightInfo->GetWinnerID() );

			if( pData )
			{
				m_pName[FINAL_LEFT+byNumber]->SetText( pData->wszTeamName );			

				if( pData->bDojoRecommender )
				{
					m_pEmblem[FINAL_LEFT+byNumber]->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_DOJO_RECOMMAND" ) );
				}
				else
				{
					m_pEmblem[FINAL_LEFT+byNumber]->Clear();
				}
			}			
		}	
	}	
	else if( !strcmp( szCommand, "TextStart" ) )
	{
		for( RwInt32 i = 0 ; i < NUM_NAME ; ++i )
		{
			m_pName[i]->Show( true );
			m_pEmblem[i]->Show( true );
		}

		m_pWinnerName->Show( true );
		m_pWinnerEmblem->Show( true );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBTournamentGui

CTBTournamentGui::CTBTournamentGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CTBTournamentGui::~CTBTournamentGui(VOID)
{

}

VOID CTBTournamentGui::Init(VOID)
{
	for( RwInt32 i = 0 ; i < NUM_PAGE ; ++i )
	{		
		m_pInfoGroup[i] = NULL;
	}	

	m_byMaxEntry = 0;

	m_eCurrentPage = TOURNAMENT_INDIVIDUAL_A;
	m_byIndividualFightInfoInputDoneCount = INVALID_BYTE;
	m_byTeamFightInfoInputDoneCount = INVALID_BYTE;

	m_pbtnRightArrow = NULL;
	m_pbtnLeftArrow = NULL;
	m_pstbIndividualGroupType = NULL;

	m_pbtnParty = NULL;
	m_pbtnIndividual = NULL;
	m_pbtnNextInfomation = NULL;

	m_pstbTitle = NULL;
	m_pstbInfomation = NULL;
}

RwBool CTBTournamentGui::Create(VOID)
{
	NTL_FUNCTION( "CTBTournamentGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\BudokaiTournament.srf", "gui\\BudokaiTournament.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain" ) );
	m_pThis->SetPriority(dDIALOGPRIORITY_TB_TOURNEMENT);

	m_pbtnRightArrow = reinterpret_cast<gui::CButton*>( GetComponent( "btnRight" ) );
	m_pbtnLeftArrow = reinterpret_cast<gui::CButton*>( GetComponent( "btnLeft" ) );
	m_pstbIndividualGroupType = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbType" ) );

	m_pbtnParty = reinterpret_cast<gui::CButton*>( GetComponent( "btnParty" )	);
	m_pbtnIndividual = reinterpret_cast<gui::CButton*>( GetComponent( "btnIndivisual" )	);

	m_pstbTitle = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbTitle" ) );
	m_pstbInfomation = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbInfo" ) );
	m_pbtnNextInfomation = reinterpret_cast<gui::CButton*>( GetComponent( "btnNextInfo" )	);

	m_pbtnClose = reinterpret_cast<gui::CButton*>( GetComponent( "btnClose" ) );

	// peessiTODO: 실제 회차에 맞게 수정. 실제 Infomation루틴 작성.
	m_pstbTitle->Format( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_TITLE" ), 1 );
	m_pstbInfomation->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TOURNAMENT_INFO_1" ) );

	m_pbtnIndividual->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_SOLO" ) );
	m_pbtnParty->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_PARTY" ) );

	// Callback
	m_slotClickClose = m_pbtnClose->SigClicked().Connect( this, &CTBTournamentGui::OnClickClose );
	m_slotClickRightArrow = m_pbtnRightArrow->SigClicked().Connect( this, &CTBTournamentGui::OnClickRightArrow );
	m_slotClickLeftArrow = m_pbtnLeftArrow->SigClicked().Connect( this, &CTBTournamentGui::OnClickLeftArrow );
	m_slotClickPartyBtn = m_pbtnParty->SigClicked().Connect( this, &CTBTournamentGui::OnClickPartyBtn );
	m_slotClickIndividualBtn = m_pbtnIndividual->SigClicked().Connect( this, &CTBTournamentGui::OnClickIndividualBtn );

	// 토너먼트 컴포넌트 생성.
	CreateFightDataGroup();

	Show( false );

	// Event
	LinkMsg( g_EventBudokaiTournamentIndividualList );
	LinkMsg( g_EventBudokaiTournamentIndividualInfo );
	LinkMsg( g_EventBudokaiTournamentTeamList );
	LinkMsg( g_EventBudokaiTournamentTeamInfo );
	LinkMsg( g_EventFinalMatchStateUpdate );
	LinkMsg( g_EventResize );

	NTL_RETURN( TRUE );
}

VOID CTBTournamentGui::Destroy(VOID)
{
	GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_TOURNAMENT );

	// 토너먼트 컴포넌트 해제
	DeleteFightDataGroup();

	m_mapIndividualEntryInfo.clear();
	m_mapTeamEntryInfo.clear();
	m_mapIndividualEntryData.clear();
	m_mapTeamEntryData.clear();

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventBudokaiTournamentIndividualList );
	UnLinkMsg( g_EventBudokaiTournamentIndividualInfo );
	UnLinkMsg( g_EventBudokaiTournamentTeamList );
	UnLinkMsg( g_EventBudokaiTournamentTeamInfo );
	UnLinkMsg( g_EventFinalMatchStateUpdate );
	UnLinkMsg( g_EventResize );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CTBTournamentGui::Update( RwReal fElapsed )
{
	for( RwInt32 i = 0 ; i < NUM_PAGE ; ++i )
	{
		m_pInfoGroup[i]->Update( fElapsed );
	}	
}

RwInt32 CTBTournamentGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( bOpen )
	{
		//SetPage( m_eCurrentPage );
		CNtlTBudokai* pBudokai = GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai();

		if( pBudokai->GetBudokaiJoinInfo()->byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL )
		{
			if( m_pInfoGroup[TOURNAMENT_INDIVIDUAL_FINAL]->IsMyTeamInThisGroup( pBudokai->GetBudokaiJoinInfo()->sIndividualInfo.wJoinId ) )
				SetPage( TOURNAMENT_INDIVIDUAL_FINAL );
			else if( m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->IsMyTeamInThisGroup( pBudokai->GetBudokaiJoinInfo()->sIndividualInfo.wJoinId ) )
				SetPage( TOURNAMENT_INDIVIDUAL_A );
			else if( m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->IsMyTeamInThisGroup( pBudokai->GetBudokaiJoinInfo()->sIndividualInfo.wJoinId ) )
				SetPage( TOURNAMENT_INDIVIDUAL_B );
			else
				SetPage( m_eCurrentPage );				
		}
		else if( pBudokai->GetBudokaiJoinInfo()->byMatchType == BUDOKAI_MATCH_TYPE_TEAM )
			SetPage( TOURNAMENT_PARTY );		
		else
			SetPage( m_eCurrentPage );


		GetNtlGuiManager()->AddUpdateFunc( this );		
	}
	else
	{
		if( GetInfoWndManager()->GetRequestGui() == DIALOG_BUDOKAI_TOURNAMENT )
			GetInfoWndManager()->ShowInfoWindow( FALSE );

		GetNtlGuiManager()->RemoveUpdateFunc( this );

		GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_TOURNAMENT_MATCHINFO );
	}

	return 1;	
}

VOID CTBTournamentGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventFinalMatchStateUpdate )
	{
		SNtlEventFinalMatchStateUpdate* pData = reinterpret_cast<SNtlEventFinalMatchStateUpdate*>( msg.pData );

		if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_MATCH_FINISH && !pData->bEnterState )
			GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_TOURNAMENT );			
	}
	else if( msg.Id == g_EventBudokaiTournamentIndividualList )
	{
		SDboEventBudokaiTournamentIndividualList* pData = reinterpret_cast<SDboEventBudokaiTournamentIndividualList*>( msg.pData );		

		if( pData->wEntryTeam_var != INVALID_WORD )
		{
			sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR* pEntryData = reinterpret_cast<sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR*>( pData->pData->Read( pData->wEntryTeam_var ) );
			
			for( RwUInt8 i = 0 ; i < BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER ; ++i )
			{
				sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO data;
				memset( &data, 0, sizeof( data ) );

				RwUInt16 wTeamNameIndex = pEntryData[i].wTeamName;
				RwUInt16 wJoinID = pEntryData[i].wJoinId;
				
				data.wJoinId = wJoinID;
				data.bDojoRecommender = pEntryData[i].bDojoRecommender;
				memcpy( data.wszTeamName, pData->pData->Read( wTeamNameIndex ), pData->pData->GetSize( wTeamNameIndex ) );

				m_mapIndividualEntryInfo[wJoinID] = data;

				m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->SetEntryData( i, wJoinID );
				m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->SetEntryData( i, wJoinID );				
			}

			m_byIndividualFightInfoInputDoneCount = 0;
		}

		for( RwInt32 i = 0 ; i < pData->byMatchDataCount ; ++i )
		{
			sBUDOKAI_TOURNAMENT_MATCH_DATA* pMatchData = reinterpret_cast<sBUDOKAI_TOURNAMENT_MATCH_DATA*>( pData->pData->Read( pData->wMatchData_var ) );
			sBUDOKAI_TOURNAMENT_MATCH_DATA* pCurrentMatchData = &pMatchData[i];

			m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->SetFightResultData( pCurrentMatchData->byDepth, pCurrentMatchData->byMatchNumber, pCurrentMatchData->wJoinId1, pCurrentMatchData->wJoinId2,
				pCurrentMatchData->byMatchResult, pCurrentMatchData->wWinnerTeam, pCurrentMatchData->byScore1, pCurrentMatchData->byScore2 );
			m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->SetFightResultData( pCurrentMatchData->byDepth, pCurrentMatchData->byMatchNumber, pCurrentMatchData->wJoinId1, pCurrentMatchData->wJoinId2,
				pCurrentMatchData->byMatchResult, pCurrentMatchData->wWinnerTeam, pCurrentMatchData->byScore1, pCurrentMatchData->byScore2 );
			m_pInfoGroup[TOURNAMENT_INDIVIDUAL_FINAL]->SetFightResultData( pCurrentMatchData->byDepth, pCurrentMatchData->byMatchNumber, pCurrentMatchData->wJoinId1, pCurrentMatchData->wJoinId2,
				pCurrentMatchData->byMatchResult, pCurrentMatchData->wWinnerTeam, pCurrentMatchData->byScore1, pCurrentMatchData->byScore2 );					
		}

		m_byIndividualFightInfoInputDoneCount = (RwUInt8)(m_byIndividualFightInfoInputDoneCount + pData->byMatchDataCount);		
	}
	else if( msg.Id == g_EventBudokaiTournamentTeamList )
	{
		SDboEventBudokaiTournamentTeamList* pData = reinterpret_cast<SDboEventBudokaiTournamentTeamList*>( msg.pData );

		if( pData->wEntryTeam_var != INVALID_WORD )
		{
			sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR* pEntryData = reinterpret_cast<sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR*>( pData->pData->Read( pData->wEntryTeam_var ) );
			
			for( RwUInt8 i = 0 ; i < BUDOKAI_MAJOR_MATCH_TEAM_NUMBER ; ++i )
			{
				sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO data;
				memset( &data, 0, sizeof( data ) );

				RwUInt16 wTeamNameIndex = pEntryData[i].wTeamName;
				RwUInt16 wJoinID = pEntryData[i].wJoinId;

				data.wJoinId = wJoinID;
				data.bDojoRecommender = pEntryData[i].bDojoRecommender;
				memcpy( data.wszTeamName, pData->pData->Read( wTeamNameIndex ), pData->pData->GetSize( wTeamNameIndex ) );

				m_mapTeamEntryInfo[wJoinID] = data;

				m_pInfoGroup[TOURNAMENT_PARTY]->SetEntryData( i, wJoinID );
			}

			m_byTeamFightInfoInputDoneCount = 0;
		}

		for( RwInt32 i = 0 ; i < pData->byMatchDataCount ; ++i )
		{
			sBUDOKAI_TOURNAMENT_MATCH_DATA* pMatchData = reinterpret_cast<sBUDOKAI_TOURNAMENT_MATCH_DATA*>( pData->pData->Read( pData->wMatchData_var ) );
			sBUDOKAI_TOURNAMENT_MATCH_DATA* pCurrentMatchData = &pMatchData[i];

			m_pInfoGroup[TOURNAMENT_PARTY]->SetFightResultData( pCurrentMatchData->byDepth, pCurrentMatchData->byMatchNumber, pCurrentMatchData->wJoinId1, pCurrentMatchData->wJoinId2,
				pCurrentMatchData->byMatchResult, pCurrentMatchData->wWinnerTeam, pCurrentMatchData->byScore1, pCurrentMatchData->byScore2 );
		}

		m_byTeamFightInfoInputDoneCount = (RwUInt8)(m_byTeamFightInfoInputDoneCount + pData->byMatchDataCount);
	}
	else if( msg.Id == g_EventBudokaiTournamentIndividualInfo )
	{
		SDboEventBudokaiTournamentIndividualInfo* pData = reinterpret_cast<SDboEventBudokaiTournamentIndividualInfo*>( msg.pData );		

		for( RwUInt16 i = 0 ; i < 2 ; ++i )
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
			data.bDojoRecommender = pVarData->bDojoRecommender;
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

		for( RwUInt16 i = 0 ; i < 2 ; ++i )
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

VOID CTBTournamentGui::CreateFightDataGroup(VOID)
{
	gui::CFlash* pFlash[NUM_PAGE] = { NULL, };
	gui::CStaticBox* pWinnerName[NUM_PAGE] = { NULL, };
	gui::CStaticBox* pWinnerEmblem[NUM_PAGE] = { NULL, };

	CRectangle rect;
	rect.SetRectWH( 19, 110, 1, 1 );
	pFlash[TOURNAMENT_INDIVIDUAL_A] = NTL_NEW gui::CFlash( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), "TB_Tournament_Table.swf", FALSE, TRUE, FALSE );		
	pFlash[TOURNAMENT_INDIVIDUAL_B] = NTL_NEW gui::CFlash( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), "TB_Tournament_Table.swf", FALSE, TRUE, FALSE );		
	pFlash[TOURNAMENT_INDIVIDUAL_FINAL] = NTL_NEW gui::CFlash( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), "TB_SemiFinal_Tournament_Table.swf", FALSE, TRUE, FALSE );		
	pFlash[TOURNAMENT_PARTY] = NTL_NEW gui::CFlash( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), "TB_Tournament_Table.swf", FALSE, TRUE, FALSE );		

	for( RwInt32 i = 0 ; i < NUM_PAGE ; ++i )
	{
		rect.SetRectWH( 273, 67, 218, 46 );
		pWinnerName[i] = NTL_NEW gui::CStaticBox( rect, pFlash[i], GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
		pWinnerName[i]->CreateFontStd( DEFAULT_FONT, 120, DEFAULT_FONT_ATTR );		

		rect.SetRectWH( 273, 55, 218, 12 );
		pWinnerEmblem[i] = NTL_NEW gui::CStaticBox( rect, pFlash[i], GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
		pWinnerEmblem[i]->CreateFontStd( DEFAULT_FONT, 90, DEFAULT_FONT_ATTR );	

		pFlash[i]->SetPriority( 0x5000 );
		pFlash[i]->PlayMovie( TRUE );
	}

	rect.SetRectWH( 306, 72, 155, 36 );
	pWinnerName[TOURNAMENT_INDIVIDUAL_FINAL]->SetPosition( rect );

	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A] = NTL_NEW CTBTournamentInfoGroup( 0, BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER / 2, BUDOKAI_MATCH_DEPTH_4, BUDOKAI_MATCH_DEPTH_32, FALSE, pFlash[TOURNAMENT_INDIVIDUAL_A], pWinnerName[TOURNAMENT_INDIVIDUAL_A], pWinnerEmblem[TOURNAMENT_INDIVIDUAL_A] );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B] = NTL_NEW CTBTournamentInfoGroup( BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER / 2, BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER / 2, BUDOKAI_MATCH_DEPTH_4, BUDOKAI_MATCH_DEPTH_32, FALSE, pFlash[TOURNAMENT_INDIVIDUAL_B], pWinnerName[TOURNAMENT_INDIVIDUAL_B], pWinnerEmblem[TOURNAMENT_INDIVIDUAL_B] );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_FINAL] = NTL_NEW CTBTournamentFinalInfoGroup( 0, 4, BUDOKAI_MATCH_DEPTH_2, BUDOKAI_MATCH_DEPTH_4, FALSE, pFlash[TOURNAMENT_INDIVIDUAL_FINAL], pWinnerName[TOURNAMENT_INDIVIDUAL_FINAL], pWinnerEmblem[TOURNAMENT_INDIVIDUAL_FINAL] );
	m_pInfoGroup[TOURNAMENT_PARTY] = NTL_NEW CTBTournamentInfoGroup( 0, BUDOKAI_MAJOR_MATCH_TEAM_NUMBER, BUDOKAI_MATCH_DEPTH_2, BUDOKAI_MATCH_DEPTH_16, TRUE, pFlash[TOURNAMENT_PARTY], pWinnerName[TOURNAMENT_PARTY], pWinnerEmblem[TOURNAMENT_PARTY] );

	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData( 0, NULL, 1, 0 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData( 1, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 1, 0 ), 2, 0 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData( 2, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 1, 0 ), 2, 1 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData( 3, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 2, 0 ), 3, 0 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData( 4, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 2, 0 ), 3, 1 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData( 5, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 2, 1 ), 3, 2 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData( 6, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 2, 1 ), 3, 3 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData( 7, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 0 ), 4, 0 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData( 8, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 0 ), 4, 1 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData( 9, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 1 ), 4, 2 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData(10, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 1 ), 4, 3 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData(11, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 2 ), 4, 4 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData(12, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 2 ), 4, 5 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData(13, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 3 ), 4, 6 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->CreateFightData(14, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->FindFightInfo( 3, 3 ), 4, 7 );

	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData( 0, NULL, 1, 1 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData( 1, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 1, 1 ), 2, 2 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData( 2, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 1, 1 ), 2, 3 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData( 3, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 2, 2 ), 3, 4 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData( 4, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 2, 2 ), 3, 5 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData( 5, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 2, 3 ), 3, 6 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData( 6, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 2, 3 ), 3, 7 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData( 7, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 4 ), 4, 8 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData( 8, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 4 ), 4, 9 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData( 9, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 5 ), 4,10 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData(10, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 5 ), 4,11 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData(11, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 6 ), 4,12 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData(12, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 6 ), 4,13 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData(13, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 7 ), 4,14 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->CreateFightData(14, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->FindFightInfo( 3, 7 ), 4,15 );

	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_FINAL]->CreateFightData( 0, NULL, 0, 0 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_FINAL]->CreateFightData( 1, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_FINAL]->FindFightInfo( 0, 0 ), 1, 0 );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_FINAL]->CreateFightData( 2, m_pInfoGroup[TOURNAMENT_INDIVIDUAL_FINAL]->FindFightInfo( 0, 0 ), 1, 1 );

	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData( 0, NULL, 0, 0 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData( 1, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 0, 0 ), 1, 0 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData( 2, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 0, 0 ), 1, 1 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData( 3, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 1, 0 ), 2, 0 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData( 4, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 1, 0 ), 2, 1 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData( 5, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 1, 1 ), 2, 2 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData( 6, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 1, 1 ), 2, 3 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData( 7, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 0 ), 3, 0 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData( 8, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 0 ), 3, 1 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData( 9, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 1 ), 3, 2 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData(10, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 1 ), 3, 3 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData(11, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 2 ), 3, 4 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData(12, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 2 ), 3, 5 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData(13, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 3 ), 3, 6 );
	m_pInfoGroup[TOURNAMENT_PARTY]->CreateFightData(14, m_pInfoGroup[TOURNAMENT_PARTY]->FindFightInfo( 2, 3 ), 3, 7 );
}

VOID CTBTournamentGui::DeleteFightDataGroup(VOID)
{
	for( RwInt32 i = 0 ; i < NUM_PAGE ; ++i )
	{
		NTL_DELETE( m_pInfoGroup[i] );
	}	
}

VOID CTBTournamentGui::SetPage( ePage ePageType )
{
	m_pInfoGroup[TOURNAMENT_PARTY]->Show( false );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->Show( false );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->Show( false );
	m_pInfoGroup[TOURNAMENT_INDIVIDUAL_FINAL]->Show( false );

	if( ePageType == TOURNAMENT_PARTY )
	{
		m_pbtnRightArrow->Show( false );
		m_pbtnLeftArrow->Show( false );
		m_pstbIndividualGroupType->Show( false );		
		m_pInfoGroup[TOURNAMENT_PARTY]->Show( true );

		// 버튼 세팅
		m_pbtnParty->ClickEnable( FALSE );
		m_pbtnIndividual->ClickEnable( TRUE );

		GetDboGlobal()->GetChatPacketGenerator()->SendBudokaiTournamentTeamListReq( m_byTeamFightInfoInputDoneCount );		
	}
	else if( ePageType == TOURNAMENT_INDIVIDUAL_A || ePageType == TOURNAMENT_INDIVIDUAL_B || ePageType == TOURNAMENT_INDIVIDUAL_FINAL )
	{
		m_pbtnRightArrow->Show( true );
		m_pbtnLeftArrow->Show( true );
		m_pstbIndividualGroupType->Show( true );

		if( ePageType == TOURNAMENT_INDIVIDUAL_A )
		{
			m_pstbIndividualGroupType->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TOURNAMENT_A_GROUP" ) );
			m_pInfoGroup[TOURNAMENT_INDIVIDUAL_A]->Show( true );
		}
		else if( ePageType == TOURNAMENT_INDIVIDUAL_B )
		{
			m_pstbIndividualGroupType->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TOURNAMENT_B_GROUP" ) );
			m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->Show( true );
		}
		else if( ePageType == TOURNAMENT_INDIVIDUAL_FINAL )
		{
			m_pstbIndividualGroupType->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TOURNAMENT_FINAL" ) );
			m_pInfoGroup[TOURNAMENT_INDIVIDUAL_FINAL]->Show( true );
		}

		// 버튼 세팅
		m_pbtnIndividual->ClickEnable( FALSE );
		m_pbtnParty->ClickEnable( TRUE );

		GetDboGlobal()->GetChatPacketGenerator()->SendBudokaiTournamentIndividualListReq( m_byIndividualFightInfoInputDoneCount );
	}

	m_eCurrentPage = ePageType;
}

VOID CTBTournamentGui::OnClickRightArrow( gui::CComponent* pComponent )
{
	if( m_eCurrentPage == TOURNAMENT_INDIVIDUAL_A )
		SetPage( TOURNAMENT_INDIVIDUAL_B );
	else if( m_eCurrentPage == TOURNAMENT_INDIVIDUAL_B )
		SetPage( TOURNAMENT_INDIVIDUAL_FINAL );
	else
		SetPage( TOURNAMENT_INDIVIDUAL_A );
}

VOID CTBTournamentGui::OnClickLeftArrow( gui::CComponent* pComponent )
{
	if( m_eCurrentPage == TOURNAMENT_INDIVIDUAL_A )
		SetPage( TOURNAMENT_INDIVIDUAL_FINAL );
	else if( m_eCurrentPage == TOURNAMENT_INDIVIDUAL_B )
		SetPage( TOURNAMENT_INDIVIDUAL_A );
	else
		SetPage( TOURNAMENT_INDIVIDUAL_B );
}

VOID CTBTournamentGui::OnClickPartyBtn( gui::CComponent* pComponent )
{
	SetPage( TOURNAMENT_PARTY );	
}

VOID CTBTournamentGui::OnClickIndividualBtn( gui::CComponent* pComponent )
{
	CNtlTBudokai* pBudokai = GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai();

	if( m_pInfoGroup[TOURNAMENT_INDIVIDUAL_FINAL]->IsMyTeamInThisGroup( pBudokai->GetBudokaiJoinInfo()->sIndividualInfo.wJoinId ) )
		SetPage( TOURNAMENT_INDIVIDUAL_FINAL );
	else if( m_pInfoGroup[TOURNAMENT_INDIVIDUAL_B]->IsMyTeamInThisGroup( pBudokai->GetBudokaiJoinInfo()->sIndividualInfo.wJoinId ) )
		SetPage( TOURNAMENT_INDIVIDUAL_B );	

	SetPage( TOURNAMENT_INDIVIDUAL_A );	
}

VOID CTBTournamentGui::OnClickClose( gui::CComponent* pComponent )
{
	GetDialogManager()->CloseDialog( DIALOG_BUDOKAI_TOURNAMENT );
}

sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO* CTBTournamentGui::FindIndividualEntryInfo( RwUInt16 wJoinID )
{
	std::map<RwUInt16,sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO>::iterator iter;

	iter = m_mapIndividualEntryInfo.find( wJoinID );
	if( iter != m_mapIndividualEntryInfo.end() )
		return &( iter->second ) ;

	return NULL;
}

sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO* CTBTournamentGui::FindTeamEntryInfo( RwUInt16 wJoinID )
{
	std::map<RwUInt16,sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO>::iterator iter;

	iter = m_mapTeamEntryInfo.find( wJoinID );
	if( iter != m_mapTeamEntryInfo.end() )
		return &( iter->second );

	return NULL;
}

sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA* CTBTournamentGui::FindIndividualEntryData( RwUInt16 wJoinID )
{
	std::map<RwUInt16,sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA>::iterator iter;
	sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA* pData = NULL;

	iter = m_mapIndividualEntryData.find( wJoinID );
	if( iter != m_mapIndividualEntryData.end() )
		pData = &( iter->second );

	return pData;
}

sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* CTBTournamentGui::FindTeamEntryData( RwUInt16 wJoinID )
{
	std::map<RwUInt16,sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA>::iterator iter;
	sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* pData = NULL;

	iter = m_mapTeamEntryData.find( wJoinID );
	if( iter != m_mapTeamEntryData.end() )
		pData = &( iter->second );

	return pData;
}
