#include "precomp_dboclient.h"
#include "GMPlayerInfoGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlWorldConceptTB.h"
#include "NtlSLLogic.h"

// dbo
#include "DialogPriority.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"


CPlayerInfoGui::CPlayerInfoGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_hPlayer(INVALID_SERIAL_ID)
{

}

CPlayerInfoGui::~CPlayerInfoGui()
{

}

RwBool CPlayerInfoGui::Create()
{
	NTL_FUNCTION( "CPlayerInfoGui::Create" );

	if(!CNtlPLGui::Create("gui\\Observer.rsr", "gui\\GMPlayerInfo.srf", "gui\\GMPlayerInfo.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority( dDIALOGPRIORITY_QUICKSLOT );

	m_pExitButton = (gui::CButton*)GetComponent("btnExit");
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CPlayerInfoGui::OnClicked_CloseButton);

	m_pPlayerName		= (gui::CStaticBox*)GetComponent("stbPlayerName");
	m_pLevel			= (gui::CStaticBox*)GetComponent("stbLevel");
	m_pRace				= (gui::CStaticBox*)GetComponent("stbRace");
	m_pClass			= (gui::CStaticBox*)GetComponent("stbClass");
	m_pRecord			= (gui::CStaticBox*)GetComponent("stbRecord");
	m_pGuild			= (gui::CStaticBox*)GetComponent("stbGuild");
	m_pTeam				= (gui::CStaticBox*)GetComponent("stbTeam");
	m_pLevel_content	= (gui::CStaticBox*)GetComponent("stbLevel_content");
	m_pRace_content		= (gui::CStaticBox*)GetComponent("stbRace_content");
	m_pClass_content	= (gui::CStaticBox*)GetComponent("stbClass_content");
	m_pRecord_content	= (gui::CStaticBox*)GetComponent("stbRecord_content");
	m_pGuild_content	= (gui::CStaticBox*)GetComponent("stbGuild_content");
	m_pTeam_content		= (gui::CStaticBox*)GetComponent("stbTeam_content");

	m_pLevel->SetText( GetDisplayStringManager()->GetString("DST_LOBBY_CHAR_LEVEL") );
	m_pRace->SetText( GetDisplayStringManager()->GetString("DST_RACE") );
	m_pClass->SetText( GetDisplayStringManager()->GetString("DST_LOBBY_CHAR_CLASS") );
	m_pRecord->SetText( GetDisplayStringManager()->GetString("DST_RECORD") );
	m_pGuild->SetText( GetDisplayStringManager()->GetString("DST_GUILD") );
	m_pTeam->SetText( GetDisplayStringManager()->GetString("DST_TAB_PARTY") );

	LinkMsg(g_EventResize);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CPlayerInfoGui::Destroy()
{
	NTL_FUNCTION("CPlayerInfoGui::Destroy");

	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

RwBool CPlayerInfoGui::SetPlayerSerial(SERIAL_HANDLE hPlayer)
{
	if( hPlayer == INVALID_SERIAL_ID )
	{
		NTL_ASSERT(false, "CPlayerInfoGui::SetPlayerSerial, Invalid serial ID");
		return FALSE;
	}

	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_T_BUDOKAI) );
	if( !pTBWorldConcept )
		return FALSE;

	stTBudokaiMember* pBudokaiMember = pTBWorldConcept->FindMember(hPlayer);
	if( !pBudokaiMember )
	{
		NTL_ASSERT(false, "CPlayerInfoGui::SetPlayerSerial, Not exist player of handle : " << hPlayer);
		return FALSE;
	}

	m_hPlayer = hPlayer;

	m_pPlayerName->SetText( pBudokaiMember->wstrName.c_str() );
	m_pLevel_content->SetText( pBudokaiMember->byLevel );	
	m_pRace_content->SetText( Logic_GetPlayerRaceName( Logic_ConverClasstoRace(pBudokaiMember->byClass) ) );
	m_pClass_content->SetText( Logic_GetClassName(pBudokaiMember->byClass) );
	m_pRecord_content->Format( GetDisplayStringManager()->GetString("DST_OBSERVER_RECORD"),
							   pBudokaiMember->wWinCount + pBudokaiMember->wLoseCount + pBudokaiMember->wDrawCount,
							   pBudokaiMember->wWinCount,
							   pBudokaiMember->wLoseCount);


	if( pBudokaiMember->wstrGuildName.empty() )
		m_pGuild_content->SetText( GetDisplayStringManager()->GetString("DST_NOT_JOINED_GUILD") );
	else
		m_pGuild_content->SetText( pBudokaiMember->wstrGuildName.c_str() );


	if( pBudokaiMember->pTeam->wstrTeamName.empty() )
	{
		m_pTeam->Show(false);
		m_pTeam_content->Show(false);
	}
	else
	{
		m_pTeam->Show(true);
		m_pTeam_content->Show(true);
		m_pTeam_content->SetText( pBudokaiMember->pTeam->wstrTeamName.c_str() );
	}

	return TRUE;
}

VOID CPlayerInfoGui::OnClicked_CloseButton(gui::CComponent* pComponent)
{
	CDboEventGenerator::ObserverMenuPost(OBSERVER_REMOVE_PLAYERINFO_GUI, m_hPlayer);
}

RwInt32 CPlayerInfoGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

VOID CPlayerInfoGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CPlayerInfoGui::HandleEvents");

	if( msg.Id == g_EventResize )
	{
		Logic_LocateDialog_in_CleintRect(this, TRUE);
	}

	NTL_RETURNVOID();
}