#include "precomp_dboclient.h"
#include "TBMatchTeamObRedGui.h"

// Core
#include "NtlDebug.h"

// Presentation
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlWorldConcept.h"
#include "NtlSobPlayer.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLEvent.h"

// Dbo
#include "DboGlobal.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBMatchTeamRedMemberGui
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CTBMatchTeamRedMemberGui::CTBMatchTeamRedMemberGui()
{
	m_bCreate = FALSE;
}

/**
* \brief Destruction
*/
CTBMatchTeamRedMemberGui::~CTBMatchTeamRedMemberGui()
{

}

/**
* \brief Create
*/
RwBool CTBMatchTeamRedMemberGui::Create( const WCHAR* pCharName, SERIAL_HANDLE hMemberSerial )
{
	NTL_FUNCTION( "CTBMatchTeamRedMemberGui::Create" );

	if( !CNtlPLGui::Create( "gui\\Observer.rsr", "gui\\TBMatchTeamObRedGui.srf", "gui\\TBMatchTeamObRedGui.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager());

	m_pThis			= (gui::CDialog*)GetComponent("dlgMain");
	m_pPnlClass		= (gui::CPanel*)GetComponent("pnlClass");
	m_pPnlLeader	= (gui::CPanel*)GetComponent("pnlLeader");
	m_pStbLevel		= (gui::CStaticBox*)GetComponent("stbLevel");
	m_pStbName		= (gui::CStaticBox*)GetComponent("stbName");
	m_pPgbLp		= (gui::CProgressBar*)GetComponent("pgbLp");
	m_pPgbEp		= (gui::CProgressBar*)GetComponent("pgbEp");

	// Name
	m_pStbName->SetText( pCharName );

	// Member Data Update
	UpdateMemberData( hMemberSerial );
	
	// 이름, 레벨 및 각종 Status 세팅
	CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>(GetNtlSobManager()->GetSobObject( hMemberSerial ));
	if( pPlayer == NULL )
	{
		DBO_FAIL( "CTBMatchIndiObBlueGui : must player handle is valid" << hMemberSerial );
		return FALSE;
	}

	RwUInt8 byClass = Logic_GetPlayerClass( pPlayer );
	m_pPnlClass->AddSurface( Logic_GetPCClassIconSurface( byClass, FALSE ) );

	// Link
	LinkMsg( g_EventSobInfoUpdate, 0 );

	GetNtlGuiManager()->AddUpdateFunc( this );
	m_bCreate = TRUE;

	Show( true );

	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
VOID CTBMatchTeamRedMemberGui::Destroy()
{
	NTL_FUNCTION( "CTBMatchTeamRedMemberGui::Destroy" );

	if( m_bCreate )
		GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventSobInfoUpdate );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

/**
* \brief Update
*/
VOID CTBMatchTeamRedMemberGui::Update( RwReal fElapsed )
{
	m_LpGauge.Update( fElapsed, m_pPgbLp );
	m_EpGauge.Update( fElapsed, m_pPgbEp );
}

/**
* \brief HandleEvents
*/
VOID CTBMatchTeamRedMemberGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast< SNtlEventSobInfoUpdate* >( msg.pData );

		if( m_hMemberSerial != pUpdate->hSerialId )
			return;

		if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR )
		{
			UpdateMemberData( pUpdate->hSerialId );
		}
	}
}

VOID CTBMatchTeamRedMemberGui::UpdateMemberData( SERIAL_HANDLE hSerial )
{
	// 이름, 레벨 및 각종 Status 세팅
	CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>(GetNtlSobManager()->GetSobObject( hSerial ));
	if( pPlayer == NULL )
	{
		DBO_FAIL( "CTBMatchIndiObRedGui : must player handle is valid" << hSerial );
		return;
	}

	m_hMemberSerial	= hSerial;

	RwUInt32	uiLp		= Logic_GetLp( pPlayer );
	RwUInt32	uiMaxLp		= Logic_GetMaxLp( pPlayer );
	RwUInt32	uiEp		= Logic_GetEp( pPlayer );
	RwUInt32	uiMaxEp		= Logic_GetMaxEp( pPlayer );
	RwUInt8		byLevel		= Logic_GetLevel( pPlayer );

	m_pStbLevel->SetText( byLevel );
	m_pPgbLp->SetRange(0, uiMaxLp );
	m_pPgbEp->SetRange(0, uiMaxEp );
	
	// Gauge Animation
	m_LpGauge.SetValue( uiLp, uiMaxLp, m_pPgbLp );
	m_EpGauge.SetValue( uiEp, uiMaxEp, m_pPgbEp );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBMatch
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CTBMatchTeamObRedGui::CTBMatchTeamObRedGui( const RwChar* pName )
: CNtlPLGui( pName )
, m_pMemberGui(NULL)
, m_byMemberNums(0)
, m_bCreate(FALSE)
{

}

/**
* \brief Destruction
*/
CTBMatchTeamObRedGui::~CTBMatchTeamObRedGui()
{

}

/**
* \brief Create
*/
RwBool CTBMatchTeamObRedGui::Create()
{
	NTL_FUNCTION( "CTBMatchTeamObRedGui::Create" );

	// Dummy Dialog
	CRectangle rect;
	rect.SetRectWH( 0, 0, 1, 1 );
	m_pThis = NTL_NEW gui::CDialog( &rect, GetNtlGuiManager()->GetGuiManager(), GetNtlGuiManager()->GetSurfaceManager() );

	LinkMsg( g_EventMajorMatchStateUpdate, 0 );
	LinkMsg( g_EventFinalMatchStateUpdate, 0 );

	Show( false );

	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
VOID CTBMatchTeamObRedGui::Destroy()
{
	DestroyMemberGui();

	NTL_DELETE( m_pThis );

	UnLinkMsg( g_EventMajorMatchStateUpdate );
	UnLinkMsg( g_EventFinalMatchStateUpdate );
}

VOID CTBMatchTeamObRedGui::CreateMemberGui()
{
	DestroyMemberGui();

	// 레드팀의 인원을 읽어온다.
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	DBO_ASSERT( pTBWorldConcept, "CTBMatchIndiObRedGui : must World concept is valid" );		

	stTBudokaiTeam* pRedTeam = pTBWorldConcept->GetRedTeam();
	DBO_ASSERT( pRedTeam, "CTBmatchIndiObRedGui : must Budokai red team is valid" );

	// Size
	m_byMemberNums = (RwUInt8)pRedTeam->vecpMember.size();
	if( m_byMemberNums == 0 )
		return;

	// Size수에 맞춰서 생성
	m_pMemberGui = NTL_NEW CTBMatchTeamRedMemberGui[m_byMemberNums];

	for(RwInt32 i=0; i < m_byMemberNums; ++i )
	{
		m_pMemberGui[i].Create( pRedTeam->vecpMember[i]->wstrName.c_str(), pRedTeam->vecpMember[i]->hSerial );
	}

	// 전체화면에서 중간에 맞추기 위하여
	int nHeight = (m_byMemberNums * dTEAMMEMBER_GUI_OFFSET_Y) - (dTEAMMEMBER_GUI_OFFSET_Y - dTEAMMEMBER_GUI_HEIGHT);
	int nStartPosY = (GetDboGlobal()->GetScreenHeight()/2) - (nHeight/2);
	for(RwInt32 i=0; i < m_byMemberNums; ++i )
	{
		m_pMemberGui[i].SetPosition( 0, nStartPosY + ( i * dTEAMMEMBER_GUI_OFFSET_Y ) );
	}

	m_bCreate = TRUE;
}

VOID CTBMatchTeamObRedGui::DestroyMemberGui()
{
	if( m_pMemberGui )
	{
		for(RwInt32 i=0; i < m_byMemberNums; ++i )
		{
			m_pMemberGui[i].Destroy();
		}
		NTL_ARRAY_DELETE( m_pMemberGui );
	}
	m_byMemberNums = 0;

	m_bCreate = FALSE;
}

/**
* \brief Update
* \param fElapsed	(RwReal) 이전 업데이트에서 경과한 시간
*/
VOID CTBMatchTeamObRedGui::Update( RwReal fElapsed )
{
	if( m_bCreate )
	{
		for(RwInt32 i=0; i< m_byMemberNums; ++i )
		{
			m_pMemberGui[i].Update( fElapsed );
		}
	}
}

/**
* \brief HandleEvents
*/
VOID CTBMatchTeamObRedGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventMajorMatchStateUpdate )
	{
		SNtlEventMajorMatchStateUpdate* pData = reinterpret_cast<SNtlEventMajorMatchStateUpdate*>( msg.pData );
		CNtlWorldConceptTB*	pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		DBO_ASSERT( pTBWorldConcept, "CTBMatchTeamObBlueGui::HandleEvents : must World concept is valid" );

		if( pData->bEnterState )
		{
			// READY, RUN, FINISH 상태가 아니라면 해제한다.
			if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_READY ||
				pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_RUN	||
				pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH )
			{
				if( !m_bCreate )
				{
					// 참가자라면 리턴				
					stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
					if( pMember )
						return;

					// 싱글 플레이 중이라면 리턴
					if( !pTBWorldConcept->IsPartyBattle() )
						return;

					// Team Info가 들어오면 생성한다.
					CreateMemberGui();
				}

			}
			else
			{
				if( m_bCreate )
					DestroyMemberGui();
			}
		}		
	}
	else if( msg.Id == g_EventFinalMatchStateUpdate )
	{
		SNtlEventFinalMatchStateUpdate* pData = reinterpret_cast<SNtlEventFinalMatchStateUpdate*>( msg.pData );
		CNtlWorldConceptTB*	pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		DBO_ASSERT( pTBWorldConcept, "CTBMatchTeamObBlueGui::HandleEvents : must World concept is valid" );

		if( pData->bEnterState )
		{
			// READY, RUN, FINISH 상태가 아니라면 해제한다.
			if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_READY ||
				pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_RUN	||
				pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_FINISH )
			{
				if( !m_bCreate )
				{
					// 참가자라면 리턴				
					stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
					if( pMember )
						return;

					// 싱글 플레이 중이라면 리턴
					if( !pTBWorldConcept->IsPartyBattle() )
						return;

					// Team Info가 들어오면 생성한다.
					CreateMemberGui();
				}
			}
			else
			{
				if( m_bCreate )
					DestroyMemberGui();
			}
		}		
	}
}