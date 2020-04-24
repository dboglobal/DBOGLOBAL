#include "precomp_dboclient.h"
#include "RBDirectionGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlSLRankBattle.h"

// client
#include "DboGlobal.h"

#define RB_DIRECT_BASIS_WIDTH	1024
#define RB_DIRECT_BASIS_HEIGHT	768

CRBDirectionGui::CRBDirectionGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	m_bFirstStageReady = TRUE;
}

CRBDirectionGui::~CRBDirectionGui(VOID)
{

}

RwBool CRBDirectionGui::Create(VOID)
{
	NTL_FUNCTION( "CRBBoardGui::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\RBDirection.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_pdlgDirection = (gui::CDialog*)GetComponent( "dlgDirection" );
	m_pflaTitle = (gui::CFlash*)GetComponent( "flaTitle" );
	m_pflaKameTeam = (gui::CFlash*)GetComponent( "flaKameTeam" );
	m_pflaTsuruTeam = (gui::CFlash*)GetComponent( "flaTsuruTeam" );
	m_pflaVS = (gui::CFlash*)GetComponent( "flaVS" );

	m_stScore.Create((gui::CFlash*)GetComponent( "flaScore" ) );

	m_rtOriginalTitle = m_pflaTitle->GetPosition();
	m_rtOriginalKameTeam = m_pflaKameTeam->GetPosition();
	m_rtOriginalTsuruTeam = m_pflaTsuruTeam->GetPosition();
	m_rtOriginalVS = m_pflaVS->GetPosition();

	m_slotTitleMovieEnd = m_pflaTitle->SigMovieEnd().Connect( this, &CRBDirectionGui::OnTitleMovieEnd );
	m_slotKameTeamMovieEnd = m_pflaKameTeam->SigMovieEnd().Connect( this, &CRBDirectionGui::OnKameTeamMovieEnd );
	m_slotTsuruTeamMovieEnd = m_pflaTsuruTeam->SigMovieEnd().Connect( this, &CRBDirectionGui::OnTsuruTeamMovieEnd );
	
	RwChar buf[256];
	for( RwInt32 i = 0 ; i < MAX_TEAM_MEMBER ; ++i )
	{
		sprintf_s( buf, 256, "flaKameTeamMember%d", i + 1 );
		m_pflaKameTeamMember[i] = (gui::CFlash*)GetComponent( buf );
		m_rtOriginalKameTeamMember[i] = m_pflaKameTeamMember[i]->GetPosition();
		m_slotKameTeamMemberMovieEnd[i] = m_pflaKameTeamMember[i]->SigMovieEnd().Connect( this, &CRBDirectionGui::OnKameTeamMemberMovieEnd );

		sprintf_s( buf, 256, "flaTsuruTeamMember%d", i + 1 );
		m_pflaTsuruTeamMember[i] = (gui::CFlash*)GetComponent( buf );
		m_rtOriginalTsuruTeamMember[i] = m_pflaTsuruTeamMember[i]->GetPosition();
		m_slotTsuruTeamMemberMovieEnd[i] = m_pflaTsuruTeamMember[i]->SigMovieEnd().Connect( this, &CRBDirectionGui::OnTsuruTeamMemberMovieEnd );
	}

	LinkMsg( g_EventRBBattleStateUpdate );
	LinkMsg( g_EventRBBattleStageFinish );
	LinkMsg( g_EventResize );
	
	Show( true );
	m_pdlgDirection->Show( false );

	GetNtlGuiManager()->AddUpdateFunc(this);

	NTL_RETURN( TRUE );
}

VOID CRBDirectionGui::Destroy(VOID)
{
	m_stScore.Destroy();

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	for (int i = 0; i < MAX_TEAM_MEMBER; ++i)
	{
		m_pflaKameTeamMember[i]->Unload();
		m_pflaTsuruTeamMember[i]->Unload();
	}

	UnLinkMsg( g_EventRBBattleStateUpdate );
	UnLinkMsg( g_EventRBBattleStageFinish );
	UnLinkMsg( g_EventResize );
	
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CRBDirectionGui::Update( RwReal fElapsed )
{
	if (m_pdlgDirection->IsVisible())
	{
		if(m_pflaTitle->IsVisible())
			m_pflaTitle->Update(fElapsed);

		if (m_pflaKameTeam->IsVisible())
			m_pflaKameTeam->Update(fElapsed);

		if (m_pflaTsuruTeam->IsVisible())
			m_pflaTsuruTeam->Update(fElapsed);

		if (m_pflaVS->IsVisible())
			m_pflaVS->Update(fElapsed);


		for (RwInt32 i = 0; i < MAX_TEAM_MEMBER; ++i)
		{
			if (m_pflaKameTeamMember[i]->IsVisible())
				m_pflaKameTeamMember[i]->Update(fElapsed);

			if (m_pflaTsuruTeamMember[i]->IsVisible())
				m_pflaTsuruTeamMember[i]->Update(fElapsed);
		}
	}

	m_stScore.Update(fElapsed);
}

VOID CRBDirectionGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventRBBattleStateUpdate )
	{
		SNtlEventRBBattleStateUpdate* pData = reinterpret_cast<SNtlEventRBBattleStateUpdate*>( msg.pData );

		if( pData->byBattleState == RANKBATTLE_BATTLESTATE_DIRECTION )
		{
			// Set position
			ReCalcPosition(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

			CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
			CNtlRankBattle::MAP_RBMEMBER_ITER it;
			RwInt32 i;

			for (i = 0, it = pRankBattle->GetBlueTeamMap()->begin(); it != pRankBattle->GetBlueTeamMap()->end(); ++i, ++it)
			{
				CNtlRankBattle::stRankBattleMember* pData = &(*it).second;
				m_pflaKameTeamMember[i]->SetVariable("Text_Box01.text", pData->wstrName.c_str());
			}

			for (i = 0, it = pRankBattle->GetRedTeamMap()->begin(); it != pRankBattle->GetRedTeamMap()->end(); ++i, ++it)
			{
				CNtlRankBattle::stRankBattleMember* pData = &(*it).second;
				m_pflaTsuruTeamMember[i]->SetVariable("Text_Box01.text", pData->wstrName.c_str());
			}

			// Direction Show
			m_pdlgDirection->Show(true);

			m_pflaKameTeam->Show(false);
			m_pflaTsuruTeam->Show(false);
			m_pflaVS->Show(false);

			m_pflaKameTeam->PlayMovie(FALSE);
			m_pflaTsuruTeam->PlayMovie(FALSE);
			m_pflaVS->PlayMovie(FALSE);

			for (RwInt32 i = 0; i < MAX_TEAM_MEMBER; ++i)
			{
				m_pflaKameTeamMember[i]->Show(false);
				m_pflaTsuruTeamMember[i]->Show(false);
				m_pflaKameTeamMember[i]->PlayMovie(FALSE);
				m_pflaTsuruTeamMember[i]->PlayMovie(FALSE);
			}

			m_pflaTitle->Show(true);
			m_pflaTitle->PlayMovie(TRUE);
		}
		else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_PREPARE)
		{
			if( m_bFirstStageReady )
			{
				m_pdlgDirection->Show( false );
				m_bFirstStageReady = FALSE;
			}
			else
			{
				m_stScore.EndMovie();
			}
		}
		else if (pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_READY)
		{
			if (m_bFirstStageReady)
			{
				m_pdlgDirection->Show(false);
				m_bFirstStageReady = FALSE;
			}
			else
			{
				m_stScore.EndMovie();
			}
		}
		else if (pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_RUN)
		{
		}
		else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_MATCH_FINISH)
		{
			m_stScore.EndMovie();
		}
	}
	else if( msg.Id == g_EventRBBattleStageFinish )
	{
		SNtlEventRBBattleStageFinish* pData = reinterpret_cast<SNtlEventRBBattleStageFinish*>( msg.pData );
		sRANKBATTLE_SCORE* pScore = reinterpret_cast<sRANKBATTLE_SCORE*>( pData->pStageScoreData );

		m_stScore.StartMovie();
		m_stScore.SetScore(pScore->byOwner, pScore->byChallenger);
		
	}
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		ReCalcPosition( pData->iWidht, pData->iHeight );		
	}
}

VOID CRBDirectionGui::SizePosCalculation( gui::CComponent* pComponent, CRectangle rtOriginal, RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	CRectangle rtRect;

	rtRect.SetRectWH( nScreenWidth * rtOriginal.left / RB_DIRECT_BASIS_WIDTH,
					  nScreenHeight * rtOriginal.top / RB_DIRECT_BASIS_HEIGHT,
					  nScreenWidth * rtOriginal.GetWidth() / RB_DIRECT_BASIS_WIDTH,
					  nScreenHeight * rtOriginal.GetHeight() / RB_DIRECT_BASIS_HEIGHT );

	pComponent->SetPosition( rtRect );
}

VOID CRBDirectionGui::ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	SizePosCalculation( m_pflaTitle, m_rtOriginalTitle, nScreenWidth, nScreenHeight );
	SizePosCalculation( m_pflaKameTeam, m_rtOriginalKameTeam, nScreenWidth, nScreenHeight );
	SizePosCalculation( m_pflaTsuruTeam, m_rtOriginalTsuruTeam, nScreenWidth, nScreenHeight );
	SizePosCalculation( m_pflaVS, m_rtOriginalVS, nScreenWidth, nScreenHeight );

	for( RwInt32 i = 0 ; i < MAX_TEAM_MEMBER ; ++i )
	{
		SizePosCalculation( m_pflaKameTeamMember[i], m_rtOriginalKameTeamMember[i], nScreenWidth, nScreenHeight );
		SizePosCalculation( m_pflaTsuruTeamMember[i], m_rtOriginalTsuruTeamMember[i], nScreenWidth, nScreenHeight );
	}

	m_stScore.ReCalcPosition( nScreenWidth, nScreenHeight );
	m_pThis->SetSize( nScreenWidth, nScreenHeight );
}

VOID CRBDirectionGui::OnTitleMovieEnd(gui::CComponent* pComponent)
{
	m_pflaTsuruTeam->Show(true);
	m_pflaTsuruTeam->PlayMovie(TRUE);
}

VOID CRBDirectionGui::OnKameTeamMovieEnd( gui::CComponent* pComponent )
{
	m_pflaKameTeamMember[0]->Show(true);
	m_pflaKameTeamMember[0]->PlayMovie(TRUE);
}

VOID CRBDirectionGui::OnTsuruTeamMovieEnd( gui::CComponent* pComponent )
{
	m_pflaTsuruTeamMember[0]->Show(true);
	m_pflaTsuruTeamMember[0]->PlayMovie(TRUE);
}

VOID CRBDirectionGui::OnKameTeamMemberMovieEnd( gui::CComponent* pComponent )
{
	RwUInt32 i;
	for (i = 0; i < MAX_TEAM_MEMBER; ++i)
	{
		if (pComponent == m_pflaKameTeamMember[i])
		{
			++i;		// i는 다음에 플레이 되는 인덱스
			break;
		}
	}

	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();

	if (pRankBattle->GetBlueTeamMap()->size() <= i)
	{
		m_pflaVS->Show(true);
		m_pflaVS->PlayMovie(TRUE);
	}
	else
	{
		m_pflaKameTeamMember[i]->Show(true);
		m_pflaKameTeamMember[i]->PlayMovie(TRUE);
	}
}

VOID CRBDirectionGui::OnTsuruTeamMemberMovieEnd( gui::CComponent* pComponent )
{
	RwUInt32 i;
	for (i = 0; i < MAX_TEAM_MEMBER; ++i)
	{
		if (pComponent == m_pflaTsuruTeamMember[i])
		{
			++i;		// i는 다음에 플레이 되는 인덱스
			break;
		}
	}

	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();

	if (pRankBattle->GetRedTeamMap()->size() <= i)
	{
		m_pflaKameTeam->Show(true);
		m_pflaKameTeam->PlayMovie(TRUE);
	}
	else
	{
		m_pflaTsuruTeamMember[i]->Show(true);
		m_pflaTsuruTeamMember[i]->PlayMovie(TRUE);
	}
}
