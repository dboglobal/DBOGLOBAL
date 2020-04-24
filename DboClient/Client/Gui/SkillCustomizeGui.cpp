#include "precomp_dboclient.h"
#include "SkillCustomizeGui.h"

// core
#include "NtlDebug.h"

// table
#include "TableContainer.h"
#include "TextAllTable.h"
#include "PCTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlSobSkill.h"
#include "NtlSobSkillAttr.h"
#include "NtlSobHTBSkill.h"
#include "NtlSobHTBSkillAttr.h"
#include "NtlSobActionSkill.h"
#include "NtlSobActionSkillAttr.h"
#include "NtlSLEventFunc.h"

// dbo
#include "DboLogic.h"
#include "DboEventHandler.h"
#include "DboEventGenerator.h"
#include "SkillCustomizeItem.h"
#include "SkillCustomizeParser.h"
#include "DisplayStringManager.h"
#include "DialogManager.h"
#include "InfoWndManager.h"


CSkillCustomizeGui::CSkillCustomizeGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	for( RwInt32 i = 0 ; i < NUM_OF_CLASS ; ++i )
		m_pSkillGroup[i] = NULL;

	m_byType = FIRST_CLASS;

	m_pScrollBar = NULL;
	m_pTabBtn = NULL;
	m_pBtnClose = NULL;
	m_pTitle = NULL;
}

CSkillCustomizeGui::~CSkillCustomizeGui(VOID)
{
	
}

RwBool CSkillCustomizeGui::Create(VOID)
{
	NTL_FUNCTION( "CSkillWindowGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\SkillCustomize.srf", "gui\\SkillCustomize.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_pTitle = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbTitle" ) );
	m_pScrollBar = reinterpret_cast<gui::CScrollBar*>( GetComponent( "scbScroll" ) );
	m_pTabBtn = reinterpret_cast<gui::CTabButton*>( GetComponent( "tabSkill" ) );
	m_pBtnClose =  reinterpret_cast<gui::CButton*>( GetComponent( "btnClose" ) );	
	m_pSP = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbSP" ) );
	m_pSkillDlg = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgSkill" ) );

	m_pBtnHelp = (gui::CButton*)GetComponent( "btnHelp" );
	
	m_slotTabChanged			= m_pTabBtn->SigSelectChanged().Connect( this, &CSkillCustomizeGui::OnTabChanged );
	m_slotCloseButtonClicked	= m_pBtnClose->SigClicked().Connect( this, &CSkillCustomizeGui::OnCloseButtonClicked );
	m_slotScrollChanged			= m_pScrollBar->SigValueChanged().Connect( this, &CSkillCustomizeGui::OnScrollChanged );
	m_slotScrollSliderMoved		= m_pScrollBar->SigSliderMoved().Connect( this, &CSkillCustomizeGui::OnScrollChanged );
	m_slotHelpButtonClicked		= m_pBtnHelp->SigClicked().Connect( this, &CSkillCustomizeGui::OnHelpButtonClicked );

	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CSkillCustomizeGui::OnCaptureWheelMove );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CSkillCustomizeGui::OnCaptureMouseDown );

	// Tab Setting
	m_pTitle->SetText( GetDisplayStringManager()->GetString( "DST_SKILL_WINDOW_TITLE" ) );

	// 탭 세팅은 나중에....
	m_pTabBtn->AddTab( std::wstring( L"Skill1" ) );// GetDisplayStringManager()->GetString( DST_SKILL_TAB_BATTLE_SKILL ) ) );
	m_pTabBtn->AddTab( std::wstring( L"Skill2_1" ) );// GetDisplayStringManager()->GetString( DST_SKILL_TAB_SUB_SKILL ) ) );
	m_pTabBtn->AddTab( std::wstring( L"Skill2_2" ) );// GetDisplayStringManager()->GetString( DST_SKILL_TAB_ACTION_SKILL ) ) );
	m_pTabBtn->AddTab( std::wstring( GetDisplayStringManager()->GetString( "DST_SKILL_TAB_ACTION_SKILL" ) ) );

	// Parser 
	CTableContainer* pTableContainer = API_GetTableContainer();
	CSkillCustomizeParser::SetCommonData( pTableContainer->GetSkillTable(), pTableContainer->GetHTBSetTable(), pTableContainer->GetActionTable(), pTableContainer->GetTextAllTable(), GetNtlGuiManager()->GetSurfaceManager() );

	// Item Group 생성	
	for( RwInt32 i = 0 ; i < NUM_OF_CLASS ; ++i )
		m_pSkillGroup[i] = NTL_NEW CSkillCustomizeItemGroup( m_pSkillDlg, m_pScrollBar );	

	// Initial Setting
	m_pThis->Show( false );

	// serialize 
	EnableSerialize( TRUE );

	// Event연결
	LinkMsg( g_EventSobInfoUpdate );
	LinkMsg( g_EventSobCooling );
	LinkMsg( g_EventPickedUpHide );
	LinkMsg( g_EventSobConvertClass, 0, 0x7000 );
	LinkMsg( g_EventTLNotifyLockUnlock );	
	LinkMsg( g_EventChangeWorldConceptState );
	LinkMsg( g_EventGameServerChangeOut );
	LinkMsg( g_EventChangeClassAuthorityChangedNfy );
	LinkMsg(g_EventSkillSelectMode);

	NTL_RETURN( TRUE );
}

VOID CSkillCustomizeGui::Destroy(VOID)
{
	for( RwInt32 i = 0 ; i < NUM_OF_CLASS ; ++i )
		NTL_DELETE( m_pSkillGroup[i] );	

	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventSobCooling );
	UnLinkMsg( g_EventPickedUpHide );
	UnLinkMsg( g_EventSobConvertClass );
	UnLinkMsg( g_EventTLNotifyLockUnlock );
	UnLinkMsg( g_EventChangeWorldConceptState );
	UnLinkMsg( g_EventGameServerChangeOut );
	UnLinkMsg( g_EventChangeClassAuthorityChangedNfy );
	UnLinkMsg(g_EventSkillSelectMode);

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CSkillCustomizeGui::Update( RwReal fElapsed )
{
	NTL_ASSERT( m_byType >= FIRST_CLASS && m_byType < NUM_OF_CLASS, "CSkillCustomizeGui::Update : Invalid GroupType" );
	m_pSkillGroup[m_byType]->Update( fElapsed );
}

RwInt32 CSkillCustomizeGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( bOpen )
	{
		// Examine the current position and print out
		CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );
		RwUInt32 flagClass = pAvatarAttr->GetPcTbl()->dwClass_Bit_Flag;
		
		if( m_pSkillGroup[SECOND_CLASS_1]->GetClassFlag() & flagClass )
			m_pTabBtn->SelectTab( SECOND_CLASS_1, TRUE );
		else if( m_pSkillGroup[SECOND_CLASS_2]->GetClassFlag() & flagClass )
			m_pTabBtn->SelectTab( SECOND_CLASS_2, TRUE );
		else
			m_pTabBtn->SelectTab( FIRST_CLASS, TRUE );

		RaiseTop();		
		Logic_SendTutorialCondition( ETL_CONDITION_TYPE_SKILL_OPEN );
		
		// Update connection
		GetNtlGuiManager()->AddUpdateFunc( this );
	}
	else
	{
		GetDialogManager()->CloseDialog( DIALOG_SKILLABILITYSET );

		if( GetInfoWndManager()->GetRequestGui() == DIALOG_SKILL )
			GetInfoWndManager()->ShowInfoWindow( FALSE );
		
		Logic_SendTutorialCondition( ETL_CONDITION_TYPE_SKILL_CLOSE );
		GetNtlGuiManager()->RemoveUpdateFunc( this );
	}	

	return 1;
}

VOID CSkillCustomizeGui::UpdatePopoNotifySkillAlarm()				/// woosungs_test 20090730
{
	RwBool bNotify = false;

	for( RwInt32 i = 0 ; i <= SECOND_CLASS_2 ; ++i )
	{
		bNotify = m_pSkillGroup[i]->IsExistUpgradebleSkill();
		if(bNotify)
		{	
			CNtlSLEventGenerator::PopoNotify(GetDisplayStringManager()->GetString("DST_SKILL_NOTYFY_UPGRADE_TITLE"),
				GetDisplayStringManager()->GetString("DST_SKILL_NOTYFY_UPGRADE_ALARM"), 
				TRUE, 30.0f);
			break;
		}
	}

	for( RwInt32 i = 0 ; i <= SECOND_CLASS_2 ; ++i )
	{
		bNotify = m_pSkillGroup[i]->IsExistTrainableSkill();
		if(bNotify)
		{	
			CNtlSLEventGenerator::PopoNotify(GetDisplayStringManager()->GetString("DST_SKILL_NOTYFY_LEARN_MASTER_SKILL_TITLE"),
				GetDisplayStringManager()->GetString("DST_SKILL_NOTYFY_LEARN_MASTER_SKILL_ALARM"), 
				TRUE, 30.0f);
			break;
		}
	}

	for( RwInt32 i = 0 ; i <= SECOND_CLASS_2 ; ++i )
	{
		bNotify = m_pSkillGroup[i]->IsExistSelfTrainableSkill();
		if(bNotify)
		{	
			CNtlSLEventGenerator::PopoNotify(GetDisplayStringManager()->GetString("DST_SKILL_NOTYFY_LEARN_SELF_SKILL_TITLE"),
				GetDisplayStringManager()->GetString("DST_SKILL_NOTYFY_LEARN_SELF_SKILL_ALARM"), 
				TRUE, 30.0f);
			break;
		}
	}
}

VOID CSkillCustomizeGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );
		RwUInt32 nSlot = pUpdate->uiParam1;

		if( pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;

		if( pUpdate->uiUpdateType & EVENT_AIUT_SKILL_SP )
		{
			SetSP();
			CheckUpgradebleSkill();
		}
		else if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR_LEVELUP )
		{
			CheckUpgradebleSkill();
			UpdatePopoNotifySkillAlarm();							/// woosungs_test 20090730
		}
		else if( pUpdate->uiUpdateType & ( EVENT_AIUT_SKILL_LEVELUP | EVENT_AIUT_SKILL_LEARN ) )
		{
			SetSkillItem( nSlot );
			CheckUpgradebleSkill();
		}		
		else if( pUpdate->uiUpdateType & EVENT_AIUT_SKILL )
		{
			for( RwInt32 i = 0 ; i < NTL_MAX_PC_HAVE_SKILL ; ++i )
			{
				SetSkillItem( i );
			}
			for( RwInt32 i = 0 ; i < NTL_HTB_MAX_PC_HAVE_HTB_SKILL ; ++i )
			{
				SetHTBItem( i );
			}
			for( RwInt32 i = 0 ; i < NTL_MAX_PC_HAVE_ACTION_SKILL ; ++i )
			{
				SetActionSkillItem( i );
			}

			SetSP();
			CheckUpgradebleSkill();
		}
		else if( pUpdate->uiUpdateType & EVENT_AIUT_HTBSKILL_LEARN )
		{
			SetHTBItem( nSlot );
			CheckUpgradebleSkill();
		}
		else if( pUpdate->uiUpdateType & EVENT_AIUT_SKILL_UI_INIT )
		{
			GenerateSkillItems();
		}
		else if( pUpdate->uiUpdateType & EVENT_AIUT_SKILL_RPBONUS )
		{
			SetRPType( nSlot );
		}
		else if( pUpdate->uiUpdateType & EVENT_AIUT_SKILL_INIT )
		{
			SetInitSkill();
			UpdatePopoNotifySkillAlarm();							/// woosungs_test 20090730
		}
	}
	else if( msg.Id == g_EventSobCooling )
	{
		SNtlEventSobCooling* pData = reinterpret_cast<SNtlEventSobCooling*>( msg.pData );

		CoolingEffectProc( pData->hSerialId, pData->bStartOrEnd );
	}
	else if( msg.Id == g_EventPickedUpHide )
	{
		RwInt32 nSlotIdx = (RwInt32)msg.pData;
		
		if( nSlotIdx != PLACE_SKILL )
			return;

		for( RwInt32 i = 0 ; i < NUM_OF_CLASS ; ++i )
			m_pSkillGroup[i]->HidePickedUp();					
	}
	else if( msg.Id == g_EventSobConvertClass )
	{
		SNtlEventSobConvertClass* pData = reinterpret_cast<SNtlEventSobConvertClass*>( msg.pData );

		if( pData->hSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
		{
			CheckUpgradebleSkill();
		}
	}
	else if( msg.Id == g_EventTLNotifyLockUnlock )
	{
		SNtlEventTLLockUnlcok* pEvent = reinterpret_cast<SNtlEventTLLockUnlcok*>( msg.pData );

		if( pEvent->byLockType != TLLOCK_MOUSE )
			return;

		if( pEvent->byIndex != ETL_MOUSE_INPUT_TYPE_SKILL_WND_TAB && 
			pEvent->byIndex != ETL_MOUSE_INPUT_TYPE_ALL )
			return;

		for( RwUInt8 i = 0 ; i < m_pTabBtn->GetButtonCount() ; ++i )
			m_pTabBtn->EnableTab( i, !pEvent->bLock );
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pEvent->eWorldConcept == WORLD_PLAY_TUTORIAL )
		{
			if( pEvent->uiState == WORLD_STATE_EXIT )
			{
				for( RwUInt8 i = 0 ; i < m_pTabBtn->GetButtonCount() ; ++i )
					m_pTabBtn->EnableTab(i, true);				
			}
		}
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		ClearSkillItems();		
	}
	else if( msg.Id == g_EventChangeClassAuthorityChangedNfy )
	{
		CheckUpgradebleSkill();
	}
	else if (msg.Id == g_EventSkillSelectMode)
	{
		SNtlEventSkillSelectMode* pData = reinterpret_cast<SNtlEventSkillSelectMode*>(msg.pData);

		SelectEffectProc(pData->hSerialId, pData->bSelect);
	}
}

VOID CSkillCustomizeGui::SetType( RwUInt8 byType )
{
	m_byType = byType;

	for( RwInt32 i = 0 ; i < NUM_OF_CLASS ; ++i )
	{
		if( i == byType )
		{
			m_pSkillGroup[i]->Show( true );
			m_pSkillGroup[i]->CalcScrollBar();
		}
		else
			m_pSkillGroup[i]->Show( false );
	}
}

VOID CSkillCustomizeGui::GenerateSkillItems(VOID)
{
	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );
	RwUInt32 flagClass = pAvatarAttr->GetPcTbl()->dwClass_Bit_Flag;

	if( flagClass & PC_CLASS_FLAG_HUMAN_FIGHTER )
	{
		m_pSkillGroup[FIRST_CLASS]->GenerateSkillItem( "HFI_SKILL.scr", PC_CLASS_FLAG_HUMAN_FIGHTER );
		m_pSkillGroup[SECOND_CLASS_1]->GenerateSkillItem( "HSF_SKILL.scr", (0x01 << PC_CLASS_STREET_FIGHTER) );
		m_pSkillGroup[SECOND_CLASS_2]->GenerateSkillItem( "HSM_SKILL.scr", (0x01 << PC_CLASS_SWORD_MASTER) );
		m_pTabBtn->ChangeTabText( FIRST_CLASS, std::wstring( GetDisplayStringManager()->GetString( "DST_HUMAN_FIGHTER" ) ) );
		m_pTabBtn->ChangeTabText( SECOND_CLASS_1, std::wstring( GetDisplayStringManager()->GetString( "DST_STREET_FIGHTER" ) ) );
		m_pTabBtn->ChangeTabText( SECOND_CLASS_2, std::wstring( GetDisplayStringManager()->GetString( "DST_SWORD_MASTER" ) ) );
	}
	else if( flagClass & PC_CLASS_FLAG_HUMAN_MYSTIC )
	{
		m_pSkillGroup[FIRST_CLASS]->GenerateSkillItem( "HMY_SKILL.scr", PC_CLASS_FLAG_HUMAN_MYSTIC );
		m_pSkillGroup[SECOND_CLASS_1]->GenerateSkillItem( "HCR_SKILL.scr", (0x01 << PC_CLASS_CRANE_ROSHI) );
		m_pSkillGroup[SECOND_CLASS_2]->GenerateSkillItem( "HTR_SKILL.scr", (0x01 << PC_CLASS_TURTLE_ROSHI) );
		m_pTabBtn->ChangeTabText( FIRST_CLASS, std::wstring( GetDisplayStringManager()->GetString( "DST_HUMAN_MYSTIC" ) ) );
		m_pTabBtn->ChangeTabText( SECOND_CLASS_1, std::wstring( GetDisplayStringManager()->GetString( "DST_CRANE_ROSHI" ) ) );
		m_pTabBtn->ChangeTabText( SECOND_CLASS_2, std::wstring( GetDisplayStringManager()->GetString( "DST_TURTLE_ROSHI" ) ) );
	}
	else if( flagClass & PC_CLASS_FLAG_HUMAN_ENGINEER )
	{

	}
	else if( flagClass & PC_CLASS_FLAG_NAMEK_FIGHTER )
	{
		m_pSkillGroup[FIRST_CLASS]->GenerateSkillItem( "NFI_SKILL.scr", PC_CLASS_FLAG_NAMEK_FIGHTER );
		m_pSkillGroup[SECOND_CLASS_1]->GenerateSkillItem( "NDW_SKILL.scr", (0x01 << PC_CLASS_DARK_WARRIOR) );
		m_pSkillGroup[SECOND_CLASS_2]->GenerateSkillItem( "NSK_SKILL.scr", (0x01 << PC_CLASS_SHADOW_KNIGHT) );			
		m_pTabBtn->ChangeTabText( FIRST_CLASS, std::wstring( GetDisplayStringManager()->GetString( "DST_NAMEK_FIGHTER" ) ) );
		m_pTabBtn->ChangeTabText( SECOND_CLASS_1, std::wstring( GetDisplayStringManager()->GetString( "DST_DARK_WARRIOR" ) ) );
		m_pTabBtn->ChangeTabText( SECOND_CLASS_2, std::wstring( GetDisplayStringManager()->GetString( "DST_SHADOW_KNIGHT" ) ) );
	}
	else if( flagClass & PC_CLASS_FLAG_NAMEK_MYSTIC )
	{
		m_pSkillGroup[FIRST_CLASS]->GenerateSkillItem( "NMY_SKILL.scr", PC_CLASS_FLAG_NAMEK_MYSTIC );
		m_pSkillGroup[SECOND_CLASS_1]->GenerateSkillItem( "NDH_SKILL.scr", (0x01 << PC_CLASS_DENDEN_HEALER) );
		m_pSkillGroup[SECOND_CLASS_2]->GenerateSkillItem( "NPS_SKILL.scr", (0x01 << PC_CLASS_POCO_SUMMONER) );
		m_pTabBtn->ChangeTabText( FIRST_CLASS, std::wstring( GetDisplayStringManager()->GetString( "DST_NAMEK_MYSTIC" ) ) );
		m_pTabBtn->ChangeTabText( SECOND_CLASS_1, std::wstring( GetDisplayStringManager()->GetString( "DST_DENDEN_HEALER" ) ) );
		m_pTabBtn->ChangeTabText( SECOND_CLASS_2, std::wstring( GetDisplayStringManager()->GetString( "DST_POCO_SUMMONER" ) ) );
	}
	else if( flagClass & PC_CLASS_FLAG_MIGHTY_MAJIN )
	{
		m_pSkillGroup[FIRST_CLASS]->GenerateSkillItem( "MMI_SKILL.scr", PC_CLASS_FLAG_MIGHTY_MAJIN );
		m_pSkillGroup[SECOND_CLASS_1]->GenerateSkillItem( "MUL_SKILL.scr", (0x01 << PC_CLASS_ULTI_MA) );		
		m_pSkillGroup[SECOND_CLASS_2]->GenerateSkillItem( "MGR_SKILL.scr", (0x01 << PC_CLASS_GRAND_MA)  );		
		m_pTabBtn->ChangeTabText( FIRST_CLASS, std::wstring( GetDisplayStringManager()->GetString( "DST_MIGHTY_MAJIN" ) ) );
		m_pTabBtn->ChangeTabText( SECOND_CLASS_1, std::wstring( GetDisplayStringManager()->GetString( "DST_ULTI_MA" ) ) );
		m_pTabBtn->ChangeTabText( SECOND_CLASS_2, std::wstring( GetDisplayStringManager()->GetString( "DST_GRAND_MA" ) ) );		
	}
	else if( flagClass & PC_CLASS_FLAG_WONDER_MAJIN )
	{
		m_pSkillGroup[FIRST_CLASS]->GenerateSkillItem( "MWO_SKILL.scr", PC_CLASS_FLAG_WONDER_MAJIN  );
		m_pSkillGroup[SECOND_CLASS_1]->GenerateSkillItem( "MPL_SKILL.scr", (0x01 << PC_CLASS_PLAS_MA) );
		m_pSkillGroup[SECOND_CLASS_2]->GenerateSkillItem( "MKR_SKILL.scr", (0x01 << PC_CLASS_KAR_MA) );
		m_pTabBtn->ChangeTabText( FIRST_CLASS, std::wstring( GetDisplayStringManager()->GetString( "DST_WONDER_MAJIN" ) ) );
		m_pTabBtn->ChangeTabText( SECOND_CLASS_1, std::wstring( GetDisplayStringManager()->GetString( "DST_PLAS_MA" ) ) );
		m_pTabBtn->ChangeTabText( SECOND_CLASS_2, std::wstring( GetDisplayStringManager()->GetString( "DST_KAR_MA" ) ) );
	}

	m_pSkillGroup[SOCIAL_ACTION]->GenerateSkillItem( "ACTION_SKILL.scr", 0 );
}

VOID CSkillCustomizeGui::ClearSkillItems(VOID)
{
	for( RwInt32 i = 0 ; i < NUM_OF_CLASS ; ++i )
		m_pSkillGroup[i]->ClearSkillItem();
}

VOID CSkillCustomizeGui::SetInitSkill(VOID)
{
	// Initialize the skill (except for ACTION)
	for( RwInt32 i = 0 ; i < SECOND_CLASS_2 ; ++i )
	{
		m_pSkillGroup[i]->SetInitSkill();
	}
}

VOID CSkillCustomizeGui::SetSkillItem( RwInt32 nSlot )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSkillContainer* pSkillContainer = pAvatar->GetSkillContainer();
	CNtlSobSkill* pSobSkill = pSkillContainer->GetSkill( nSlot );

	if( pSobSkill )
	{
		CNtlSobSkillAttr* pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobSkill->GetSobAttr() );
		CNtlSobSkillIcon* pSobSkillIcon = reinterpret_cast<CNtlSobSkillIcon*>( pSobSkill->GetIcon() );
		sSKILL_TBLDAT* pSkillTable = pSobSkillAttr->GetSkillTbl();
		
		for( RwInt32 i = 0 ; i <= SECOND_CLASS_2 ; ++i )
		{
			if( m_pSkillGroup[i]->GetClassFlag() & pSkillTable->dwPC_Class_Bit_Flag )
				m_pSkillGroup[i]->SetSkillItem( API_GetTableContainer()->GetSkillTable()->FindBasicSkillTblidx( pSkillTable->tblidx ), (CNtlSobIcon*)pSobSkillIcon, pSkillTable );
		}
	}
}

VOID CSkillCustomizeGui::SetHTBItem( RwInt32 nSlot )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSkillContainer* pSkillContainer = pAvatar->GetSkillContainer();
	CNtlSobHTBSkill* pSobHTBSkill = pSkillContainer->GetHTBSkill( nSlot );

	if( pSobHTBSkill )
	{
		CNtlSobHTBSkillAttr* pSobHTBSkillAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>( pSobHTBSkill->GetSobAttr() );
		CNtlSobHTBSkillIcon* pSobHTBSkillIcon = reinterpret_cast<CNtlSobHTBSkillIcon*>( pSobHTBSkill->GetIcon() );
		sHTB_SET_TBLDAT* pHTBTable = pSobHTBSkillAttr->GetHTBSkillTbl();

		for( RwInt32 i = 0 ; i <= SECOND_CLASS_2 ; ++i )
		{
			if( m_pSkillGroup[i]->GetClassFlag() & pHTBTable->dwPC_Class_Bit_Flag )
				m_pSkillGroup[i]->SetSkillItem( pHTBTable->tblidx, (CNtlSobIcon*)pSobHTBSkillIcon, pHTBTable );
		}
	}
}

VOID CSkillCustomizeGui::SetActionSkillItem( RwInt32 nSlot )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSkillContainer* pSkillContainer = pAvatar->GetSkillContainer();
	CNtlSobActionSkill* pSobActionSkill = pSkillContainer->GetActionSkill( nSlot );

	if( pSobActionSkill )
	{
		CNtlSobActionSkillAttr* pSobActionSkillAttr = reinterpret_cast<CNtlSobActionSkillAttr*>( pSobActionSkill->GetSobAttr() );
		CNtlSobActionSkillIcon* pSobActionSkillIcon = reinterpret_cast<CNtlSobActionSkillIcon*>( pSobActionSkill->GetIcon() );
		sACTION_TBLDAT* pActionTable = pSobActionSkillAttr->GetActionSkillTbl();

		m_pSkillGroup[SOCIAL_ACTION]->SetSkillItem( pActionTable->tblidx, (CNtlSobIcon*)pSobActionSkillIcon, pActionTable );				
	}
}

VOID CSkillCustomizeGui::SetRPType( RwInt32 nSlot )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSkillContainer* pSkillContainer = pAvatar->GetSkillContainer();
	CNtlSobSkill* pSobSkill = pSkillContainer->GetSkill( nSlot );

	if( pSobSkill )
	{
		CNtlSobSkillAttr* pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobSkill->GetSobAttr() );
		sSKILL_TBLDAT* pSkillTable = pSobSkillAttr->GetSkillTbl();

		for( RwInt32 i = 0 ; i <= SECOND_CLASS_2 ; ++i )
		{
			if( m_pSkillGroup[i]->GetClassFlag() & pSkillTable->dwPC_Class_Bit_Flag )
				m_pSkillGroup[i]->SetRPType( API_GetTableContainer()->GetSkillTable()->FindBasicSkillTblidx( pSkillTable->tblidx ), pSobSkillAttr );
		}
	}
}

VOID CSkillCustomizeGui::SetSP(VOID)
{
	m_pSP->Format( GetDisplayStringManager()->GetString( "DST_SKILL_SP" ), Logic_GetSp() );
}

VOID CSkillCustomizeGui::CheckUpgradebleSkill(VOID)
{
	for( RwInt32 i = 0 ; i <= SECOND_CLASS_2 ; ++i )
	{
		m_pSkillGroup[i]->CheckUpgradebleSkill();
	}
}

VOID CSkillCustomizeGui::CoolingEffectProc( RwUInt32 hSerial, RwBool bStart )
{
	CNtlSob* pSob = GetNtlSobManager()->GetSobObject( hSerial );
	NTL_ASSERT( pSob, "CSkillCustomizeGui::CoolingEffectProc : Invalid Serial ID" );
	RwUInt32 uiBaseTblIndex = INVALID_TBLIDX;
	RwUInt32 flagClass = 0;
	
	if( pSob )
	{
		switch( pSob->GetClassID() )
		{
		case SLCLASS_SKILL:
			{
				CNtlSobSkillAttr* pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSob->GetSobAttr() );
				uiBaseTblIndex = API_GetTableContainer()->GetSkillTable()->FindBasicSkillTblidx( pSobSkillAttr->GetSkillTbl()->tblidx );			
				flagClass = pSobSkillAttr->GetSkillTbl()->dwPC_Class_Bit_Flag;
			}
			break;
		case SLCLASS_HTBSKILL:
			{
				CNtlSobHTBSkillAttr* pSobHTBAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>( pSob->GetSobAttr() );
				uiBaseTblIndex = pSobHTBAttr->GetHTBSkillTbl()->tblidx;
				flagClass = pSobHTBAttr->GetHTBSkillTbl()->dwPC_Class_Bit_Flag;
			}
			break;
		case SLCLASS_ACTIONSKILL:
			{
				CNtlSobActionSkillAttr* pSobAction = reinterpret_cast<CNtlSobActionSkillAttr*>( pSob->GetSobAttr() );
				uiBaseTblIndex = pSobAction->GetActionSkillTbl()->tblidx;

				m_pSkillGroup[SOCIAL_ACTION]->CoolingEffectProc( uiBaseTblIndex, bStart );
			}
		default:
			return;
		}		

		for( RwInt32 i = 0 ; i <= SECOND_CLASS_2 ; ++i )
		{
			if( m_pSkillGroup[i]->GetClassFlag() & flagClass )
				m_pSkillGroup[i]->CoolingEffectProc( uiBaseTblIndex, bStart );
		}
	}
}

void CSkillCustomizeGui::SelectEffectProc(RwUInt32 hSerial, bool bStart)
{
	CNtlSob* pSob = GetNtlSobManager()->GetSobObject(hSerial);
	NTL_ASSERT(pSob, "CSkillCustomizeGui::SelectEffectProc : Invalid Serial ID");
	RwUInt32 uiBaseTblIndex = INVALID_TBLIDX;
	RwUInt32 flagClass = 0;

	if (pSob)
	{
		switch (pSob->GetClassID())
		{
		case SLCLASS_SKILL:
		{
			CNtlSobSkillAttr* pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>(pSob->GetSobAttr());
			uiBaseTblIndex = API_GetTableContainer()->GetSkillTable()->FindBasicSkillTblidx(pSobSkillAttr->GetSkillTbl()->tblidx);
			flagClass = pSobSkillAttr->GetSkillTbl()->dwPC_Class_Bit_Flag;
		}
		break;
		case SLCLASS_HTBSKILL:
		{
			CNtlSobHTBSkillAttr* pSobHTBAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>(pSob->GetSobAttr());
			uiBaseTblIndex = pSobHTBAttr->GetHTBSkillTbl()->tblidx;
			flagClass = pSobHTBAttr->GetHTBSkillTbl()->dwPC_Class_Bit_Flag;
		}
		break;
		case SLCLASS_ACTIONSKILL:
		{
			CNtlSobActionSkillAttr* pSobAction = reinterpret_cast<CNtlSobActionSkillAttr*>(pSob->GetSobAttr());
			uiBaseTblIndex = pSobAction->GetActionSkillTbl()->tblidx;

			m_pSkillGroup[SOCIAL_ACTION]->SelectEffectProc(uiBaseTblIndex, bStart);
		}
		default:
			return;
		}

		for (RwInt32 i = 0; i <= SECOND_CLASS_2; ++i)
		{
			if (m_pSkillGroup[i]->GetClassFlag() & flagClass)
				m_pSkillGroup[i]->SelectEffectProc(uiBaseTblIndex, bStart);
		}
	}
}

VOID CSkillCustomizeGui::OnTabChanged( RwInt32 nIndex, RwInt32 nOldIndex )
{
	SetType( (RwUInt8)nIndex );
}

VOID CSkillCustomizeGui::OnCloseButtonClicked( gui::CComponent* pComponent )
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_SKILL_WND_CLOSE_BTN ) )
		return;

	GetDialogManager()->CloseDialog( DIALOG_SKILL );			
}

VOID CSkillCustomizeGui::OnScrollChanged( RwInt32 nNewOffset )
{
	NTL_ASSERT( m_byType >= FIRST_CLASS && m_byType < NUM_OF_CLASS, "CSkillCustomizeGui::OnScrollChanged : Invalid GroupType" );
	m_pSkillGroup[m_byType]->SetOffset( nNewOffset );
}

VOID CSkillCustomizeGui::OnHelpButtonClicked( gui::CComponent* pComponent )
{
	CDboEventGenerator::OpenHelpContent( DIALOG_SKILL );
}

VOID CSkillCustomizeGui::OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
{
	CAPTURE_MOUSEWHEELMOVE_SCROLLBAR( IsShow(), m_pSkillDlg->GetParent(), m_pSkillDlg, m_pScrollBar, sDelta * 12, pos );

	//if( !IsShow() )
	//	return;

	//if( m_pSkillDlg->GetParent()->GetParent()->GetChildComponentReverseAt( pos.x - m_pSkillDlg->GetParent()->GetParent()->GetScreenRect().left, pos.y - m_pSkillDlg->GetParent()->GetParent()->GetScreenRect().top ) != m_pSkillDlg->GetParent() ) 
	//	return;

	//if( m_pSkillDlg->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT ) 
	//	return;

	//RwInt32 nValue = m_pScrollBar->GetValue();
	//RwInt32 nMaxValue = m_pScrollBar->GetMaxValue();
	//RwInt32 nDelta = nValue - sDelta / 10;

	//if( nDelta < 0 ) 
	//	nDelta = 0;
	//else if( nDelta > nMaxValue ) 
	//	nDelta = nMaxValue;

	//m_pScrollBar->SetValue( nDelta );	
}

VOID CSkillCustomizeGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_SKILL, key.m_fX, key.m_fY );
}
