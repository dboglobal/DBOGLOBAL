#include "precomp_dboclient.h"
#include "TBMatchIndiObBlueGui.h"

// Core
#include "NtlDebug.h"

// Presentation
#include "NtlPLGuiManager.h"

// Simulaion
#include "NtlWorldConcept.h"
#include "NtlSobPlayer.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLEvent.h"

// Dbo
#include "DboGlobal.h"

/**
* \brief Construction
* \param pName	(const RwChar*) GUI 이름
*/
CTBMatchIndiObBlueGui::CTBMatchIndiObBlueGui( const RwChar* pName )
: CNtlPLGui( pName )
, m_bSetData(FALSE)
{

}

/**
* \brief Destruction
*/
CTBMatchIndiObBlueGui::~CTBMatchIndiObBlueGui()
{

}


/**
* \brief Create
* 천하제일 무도회 개인전 관전자 RED 팀의 StatusBar에 관련된 GUI를 생성한다.
*/
RwBool CTBMatchIndiObBlueGui::Create()
{
	NTL_FUNCTION("CTBMatchIndiObBlueGui::Create");

	if( !CNtlPLGui::Create("gui\\Observer.rsr", "gui\\TBMatchIndiObBlueGui.srf", "gui\\TBMatchIndiObBlueGui.frm") )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis		= (gui::CDialog*)GetComponent("dlgMain");
	m_pStbName	= (gui::CStaticBox*)GetComponent("stbName");
	m_pPgbLp	= (gui::CProgressBar*)GetComponent("pgbLP");
	m_pPgbEp	= (gui::CProgressBar*)GetComponent("pgbEP");
	m_pStbLp	= (gui::CStaticBox*)GetComponent("stbLP");
	m_pStbEp	= (gui::CStaticBox*)GetComponent("stbEP");
	m_pStbLevel	= (gui::CStaticBox*)GetComponent("stbLevel");

	// Link
	LinkMsg( g_EventSobInfoUpdate, 0 );
	LinkMsg( g_EventMajorMatchStateUpdate, 0 );
	LinkMsg( g_EventFinalMatchStateUpdate, 0 );

	// Add Update
	GetNtlGuiManager()->AddUpdateFunc( this );

	Show( false );

	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
VOID CTBMatchIndiObBlueGui::Destroy()
{
	NTL_FUNCTION("CTBMatchIndiObBlueGui::Destroy");

	// Remove Update
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventMajorMatchStateUpdate );
	UnLinkMsg( g_EventFinalMatchStateUpdate );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

/**
* \brief Update
* \param fElapsed	(RwReal) 이전 프레임에서 경과된 시간
*/
VOID CTBMatchIndiObBlueGui::Update( RwReal fElapsed )
{
	m_LpGauge.Update( fElapsed, m_pPgbLp );
	m_EpGauge.Update( fElapsed, m_pPgbEp );
}

/**
* \brief HandleEvents
* \param msg		(RWS::CMsg&) Event 메시지 구조체
*/
VOID CTBMatchIndiObBlueGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventSobInfoUpdate )
	{
		if( !m_bSetData || !IsShow() )
			return;

		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast< SNtlEventSobInfoUpdate* >( msg.pData );

		stTBudokaiMember* pMember = GetBlueTeamUser();

		// 현재 블루팀의 플레이어만 업데이트 해준다.
		if( pMember->hSerial != pUpdate->hSerialId )
			return;

		// Attribute 관련 Update
		if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR )
		{
			UpdateUserData( pUpdate->hSerialId );
		}
	}
	else if( msg.Id == g_EventMajorMatchStateUpdate )
	{
		SNtlEventMajorMatchStateUpdate* pData = reinterpret_cast<SNtlEventMajorMatchStateUpdate*>( msg.pData );

		if( pData->bEnterState )
		{
			if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_READY ||
				pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_RUN	||
				pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH )
			{
				if( !IsShow() )
				{
					// 참가자라면 리턴
					CNtlWorldConceptTB*	pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
					DBO_ASSERT( pTBWorldConcept, "CTBMatchIndiObBlueGui::HandleEvents : must World concept is valid" );

					stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
					if( pMember )
						return;

					// 파티 플레이라면 리턴
					if( pTBWorldConcept->IsPartyBattle() )
						return;

					// Team 정보가 오면 유저의 Data를 입력한다.
					SetUserData();

					// SetPosition : Blue
					SetPosition( GetDboGlobal()->GetScreenWidth() - m_pThis->GetScreenRect().GetWidth(), 0 );
					Show( true );
				}
			}
			else
			{
				if( IsShow() )
					Show( false );
			}
		}		
	}
	else if( msg.Id == g_EventFinalMatchStateUpdate )
	{
		SNtlEventFinalMatchStateUpdate* pData = reinterpret_cast<SNtlEventFinalMatchStateUpdate*>( msg.pData );

		if( pData->bEnterState )
		{
			if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_READY ||
				pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_RUN	||
				pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_FINISH )
			{
				if( !IsShow() )
				{
					// 참가자라면 리턴
					CNtlWorldConceptTB*	pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
					DBO_ASSERT( pTBWorldConcept, "CTBMatchIndiObBlueGui::HandleEvents : must World concept is valid" );

					stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
					if( pMember )
						return;

					// 파티 플레이라면 리턴
					if( pTBWorldConcept->IsPartyBattle() )
						return;

					pMember = GetBlueTeamUser();
					if( !pMember )
						return;

					// Team 정보가 오면 유저의 Data를 입력한다.
					SetUserData();

					// SetPosition : Blue
					SetPosition( GetDboGlobal()->GetScreenWidth() - m_pThis->GetScreenRect().GetWidth(), 0 );
					Show( true );
				}
			}
			else
			{
				if( IsShow() )
					Show( false );
			}
		}
	}
}

/**
* \brief Blue 팀의 Plyaer Data 업데이트
*/
VOID CTBMatchIndiObBlueGui::SetUserData()
{
	stTBudokaiMember* pMember = GetBlueTeamUser();
	DBO_ASSERT( pMember, "CTBMatchIndiObBlueGui : must Budokai member is valid" );

	// User를 검사해서 현재의 유저가 NOAPPLY또는 GIVEUP일 경우 업데이트를 하지 않는다.
	if( pMember->byState == MATCH_MEMBER_STATE_NOAPPLY ||
		pMember->byState == MATCH_MEMBER_STATE_GIVEUP )
	{
		return;
	}

	m_bSetData = TRUE;

	// 이름
	m_pStbName->SetText( pMember->wstrName.c_str() );

	UpdateUserData( pMember->hSerial );
}

VOID CTBMatchIndiObBlueGui::UpdateUserData( SERIAL_HANDLE hSerial )
{
	// 이름, 레벨 및 각종 Status 세팅
	CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>(GetNtlSobManager()->GetSobObject( hSerial ));
	DBO_ASSERT( pPlayer, "CTBMatchIndiObBlueGui : must player handle is valid" );

	RwUInt32	uiLp		= Logic_GetLp( pPlayer );
	RwUInt32	uiMaxLp		= Logic_GetMaxLp( pPlayer );
	RwUInt32	uiEp		= Logic_GetEp( pPlayer );
	RwUInt32	uiMaxEp		= Logic_GetMaxEp( pPlayer );
	RwUInt8		byLevel		= Logic_GetLevel( pPlayer );

	m_pStbLevel->SetText( byLevel );
	m_pPgbLp->SetRange(0, uiMaxLp );
	m_pPgbEp->SetRange(0, uiMaxEp );
	m_pStbLp->Format( "%u / %u", uiLp, uiMaxLp );
	m_pStbEp->Format( "%u / %u", uiEp, uiMaxEp );

	// Gauge Animation
	m_LpGauge.SetValue( uiLp, uiMaxLp, m_pPgbLp );
	m_EpGauge.SetValue( uiEp, uiMaxEp, m_pPgbEp );
}

/**
* \brief Blue 팀의 User Data를 리턴
*/
stTBudokaiMember* CTBMatchIndiObBlueGui::GetBlueTeamUser()
{
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	DBO_ASSERT( pTBWorldConcept, "CTBMatchIndiObBlueGui : must World concept is valid" );		

	stTBudokaiTeam* pRedTeam = pTBWorldConcept->GetBlueTeam();
	if( !pRedTeam )
	{
		DBO_ASSERT( pRedTeam, "CTBmatchIndiObBlueGui : must Budokai red team is valid" );
		return NULL;
	}

	// 팀 정보가 없다.
	if( pRedTeam->vecpMember.size() <= 0 )
		return NULL;

	VEC_TBTEAMMEMBER* pMembers = &pRedTeam->vecpMember;
	stTBudokaiMember* pMember = reinterpret_cast<stTBudokaiMember*>(pMembers->front());

	return pMember;
}
