#include "precomp_dboclient.h"
#include "PartyMenuGui.h"
// core
#include "NtlDebug.h"

// shared
#include "WorldTable.h"
#include "TableContainer.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"

// dbo
#include "DboGlobal.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DboPacketGenerator.h"
#include "DisplayStringManager.h"
#include "ChatGui.h"
#include "PartyMemberGui.h"
#include "AlarmManager.h"
#include "SideDialogManager.h"
#include "PopupManager.h"

namespace
{
	#define dMENU_GAP					26
	#define dMEMBER_GAP					38
	#define dBUTTON_GAP					2
	#define dBUFF_GAP					8

	#define dUPDATE_TIME_OTHER_WORLD	10.f
};


//////////////////////////////////////////////////////////////////////////
//	CPartyMenu
//////////////////////////////////////////////////////////////////////////
CPartyMenu::CPartyMenu(const RwChar* pName)
:CNtlPLGui(pName)
,m_fUpdateTimeAbsentee(0.f)
{
}

CPartyMenu::~CPartyMenu()
{
}

RwBool CPartyMenu::Create()
{
	NTL_FUNCTION( "CPartyMenu::Create" );

	if(!CNtlPLGui::Create("", "gui\\PartyMenu.srf", "gui\\PartyMenu.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_PARTY_MENU);

	SetPosition(dDIALOG_CLEINT_EDGE_GAP, 228);

	CRectangle rtScreen = m_pThis->GetPosition();

	// 파티 이름
	m_pPartyName = (gui::CStaticBox*)GetComponent("stbPartyName");

	// 파티원 관리 버튼
	m_pMemberMenuButton = (gui::CButton*)GetComponent("btnMainMenu");
	m_pMemberMenuButton->SetToolTip(GetDisplayStringManager()->GetString("DST_PARTYGUI_MEMBER"));
	m_slotMemberMenuButton = m_pMemberMenuButton->SigClicked().Connect(this, &CPartyMenu::OnClick_MemberMenuButton);

	// 파티 메뉴/맴버 창의 switch 버튼
	m_pSwitchButton = (gui::CButton*)GetComponent("btnSwitchMenu");
	m_pSwitchButton->SetToolTip(GetDisplayStringManager()->GetString("DST_PARTYGUI_SWITCH"));
	m_slotSwitchButton = m_pSwitchButton->SigClicked().Connect(this, &CPartyMenu::OnClick_SwitchButton);

	// 제니 분배 방식 버튼
	m_pDivideZennyButton = (gui::CButton*)GetComponent("btnDivideZenny");
	m_pDivideZennyButton->SetToolTip(GetDisplayStringManager()->GetString("DST_PARTYGUI_ZENNY_LOOTING"));
	m_slotDivideZennyButton = m_pDivideZennyButton->SigClicked().Connect(this, &CPartyMenu::OnClick_DevideZennyButton);

	// 아이템 분배 방식 버튼
	m_pDivideItemButton = (gui::CButton*)GetComponent("btnDivideItem");
	m_pDivideItemButton->SetToolTip(GetDisplayStringManager()->GetString("DST_PARTYGUI_ITEM_LOOTING"));
	m_slotDivideItemButton = m_pDivideItemButton->SigClicked().Connect(this, &CPartyMenu::OnClick_DevideItemButton);

	// 던전 버튼
	m_pDungeonButton = (gui::CButton*)GetComponent("btnDungeon");
	m_pDungeonButton->SetToolTip(GetDisplayStringManager()->GetString("DST_PARTYGUI_DUNGEON"));
	m_slotDungeonButton = m_pDungeonButton->SigClicked().Connect(this, &CPartyMenu::OnClick_DungeonButton);

	// 메뉴 펼치기 버튼
	m_pSpreadMenuButton = (gui::CButton*)GetComponent("btnSpreadMenu");
	m_pSpreadMenuButton->SetToolTip(GetDisplayStringManager()->GetString("DST_PARTYGUI_SPREAD"));
	m_slotSpreadMenuButton = m_pSpreadMenuButton->SigClicked().Connect(this, &CPartyMenu::OnClick_SpreadMenuButton);
	m_pSpreadMenuButton->Show(false);

	// 메뉴 접기 버튼
	m_pFoldMenuButton = (gui::CButton*)GetComponent("btnFoldMenu");
	m_pFoldMenuButton->SetToolTip(GetDisplayStringManager()->GetString("DST_PARTYGUI_FOLD"));
	m_slotFoldMenuButton = m_pFoldMenuButton->SigClicked().Connect(this, &CPartyMenu::OnClick_FoldMenuButton);

	// 파티 이름 배경
	m_srfPartyNameBackground.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyMenu.srf", "srfPartyNameBackground" ) );
	m_srfPartyNameBackground.SetPositionfromParent(0, -(m_srfPartyNameBackground.GetHeight() + 2));	

	// default value
	m_MenuShape.bShowBuff	= TRUE;
	m_MenuShape.bShowMember	= TRUE;

	SpreadButtons();

	// 파티를 만들기 전에는 보여주지 않는다
	m_pPartyName->Show(false);

	MemberButton_UpdateTooltip();
	NoPartyButtons();

	// for default position
	OnMove(0, 0);

	// sig
	m_slotMove			= m_pThis->SigMove().Connect(this, &CPartyMenu::OnMove );
	m_slotPaint			= m_pThis->SigPaint().Connect(this, &CPartyMenu::OnPaint );

	LinkMsg(g_EventPartyUpdateValue);
	LinkMsg(g_EventChangeWorldConceptState);
	LinkMsg(g_EventIconPopupResult);	

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CPartyMenu::Destroy()
{
	NTL_FUNCTION("CPartyMenu::Destroy");

	Clear();	

	UnLinkMsg(g_EventPartyUpdateValue);
	UnLinkMsg(g_EventChangeWorldConceptState);
	UnLinkMsg(g_EventIconPopupResult);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CPartyMenu::Clear()
{
	CheckPopup();
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	for each( CPartyMemberGui* pPartyMemberGui in m_listPartyMember )
	{
		pPartyMemberGui->Destroy();
		NTL_DELETE(pPartyMemberGui);
	}
	m_listPartyMember.clear();
}

VOID CPartyMenu::ResetPartyMenu()
{
	NoPartyButtons();
	Clear();
}

VOID CPartyMenu::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();	
	if( pParty->GetMemberCount() <= 1 )
		return;


	PARTYMEMBER_ITER it = m_listPartyMember.begin();
	for( ; it != m_listPartyMember.end() ; ++it )
	{
		(*it)->Update(fElapsed);
	}

	m_fUpdateTimeAbsentee += fElapsed;

	if( m_fUpdateTimeAbsentee >= dUPDATE_TIME_OTHER_WORLD )
	{
		m_fUpdateTimeAbsentee = 0.f;
		SetOtherWorld();
	}
}

VOID CPartyMenu::AddMember(sPartyMember* pMember)
{
	// 파티맴버와 자신까지 합한 수가 최대 파티원 수보다 크면 리턴
	if( m_listPartyMember.size() + 1 > NTL_MAX_MEMBER_IN_PARTY )
		return;

	CRectangle rect = GetPosition();
	CPartyMemberGui* pPartyMember = NTL_NEW CPartyMemberGui;
	pPartyMember->Create(pMember->hSerial, pMember->wszMemberName, pMember->byClass);
	if( !pPartyMember )
	{
		DBO_FAIL("Can not create PartyMemberGui");
		pPartyMember->Destroy();
		NTL_DELETE(pPartyMember);
		return;
	}	

	pPartyMember->SetLevel(pMember->byLevel);	
	pPartyMember->SetEP(pMember->wCurEP, pMember->wMaxEP);	
	pPartyMember->SetLP(pMember->wCurLP, pMember->wMaxLP);		
	
	RwInt32 iHeight = rect.bottom + dMENU_GAP + m_listPartyMember.size() * (pPartyMember->GetHeight() + dMEMBER_GAP);
	pPartyMember->SetPosition(dDIALOG_CLEINT_EDGE_GAP, iHeight);
	pPartyMember->Show(m_MenuShape.bShowMember);
	pPartyMember->ShowBuff(m_MenuShape.bShowBuff);

	m_listPartyMember.push_back(pPartyMember);
}

VOID CPartyMenu::DelMember(SERIAL_HANDLE hSerial)
{
	CPartyMemberGui* pPartyMemberGui;
	PARTYMEMBER_ITER it = m_listPartyMember.begin();
	for( ; it != m_listPartyMember.end() ; ++it )
	{
		pPartyMemberGui = *it;
		if( pPartyMemberGui->GetHandle() == hSerial )
		{
			pPartyMemberGui->Destroy();
			NTL_DELETE(pPartyMemberGui);
			it = m_listPartyMember.erase(it);

			break;
		}
	}

	// 삭제된 맴버ui 밑의 ui를 한단계씩 위로 올린다.
	for( ; it != m_listPartyMember.end() ; ++it )
	{
		pPartyMemberGui = *it;
		CRectangle rect = pPartyMemberGui->GetPosition();
		pPartyMemberGui->SetPosition(dDIALOG_CLEINT_EDGE_GAP, rect.top - pPartyMemberGui->GetHeight() - 40);
	}
}

CPartyMemberGui* CPartyMenu::GetPartyMemberGui(SERIAL_HANDLE hSerial)
{
	PARTYMEMBER_ITER it = m_listPartyMember.begin();
	for( ; it != m_listPartyMember.end() ; ++it )
	{
		CPartyMemberGui* pPartyMemberGui = *it;

		if(pPartyMemberGui->GetHandle() == hSerial)
		{
			return pPartyMemberGui;
		}
	}

	return NULL;
}

VOID CPartyMenu::SetOtherWorld()
{	
	CWorldTable *pWorldTable = API_GetTableContainer()->GetWorldTable();
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlParty* pParty = pAvatar->GetParty();	
	sPartyMember* pAvatarMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pAvatar->GetSerialID()) );
	sWORLD_TBLDAT* pAvatarWorldTblData = reinterpret_cast<sWORLD_TBLDAT*>( pWorldTable->FindData( Logic_GetActiveWorldTableId() ) );

	if( !pAvatarWorldTblData )
	{
		DBO_FAIL("invalid world table index : " << Logic_GetActiveWorldTableId() );
		return;
	}	

	// 아바타와는 다른 세상에 있는 맴버를 찾아내어 표시한다(TMQ, 랭크배틀 등...)
	PARTYMEMBER_ITER it = m_listPartyMember.begin();
	for( ; it != m_listPartyMember.end() ; ++it )
	{
		CPartyMemberGui* pMemberGui = *it;
		sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pMemberGui->GetHandle()) );
		if( !pMember )
		{
			DBO_FAIL("Not exist member data : " << pMemberGui->GetHandle());
			continue;
		}		
		
		sWORLD_TBLDAT* pWorldTblData = reinterpret_cast<sWORLD_TBLDAT*>( pWorldTable->FindData(pMember->uiWorldTbl) );
		if( !pWorldTblData )
		{
			DBO_FAIL("invalid world table index : " << pMember->uiWorldTbl);
			continue;
		}		

		if( pAvatarMember->uiWorldID != pMember->uiWorldID )			
		{
			if( pWorldTblData->bDynamic || pAvatarWorldTblData->bDynamic)
				pMemberGui->SetOtherWorld(TRUE);
			else
				pMemberGui->SetOtherWorld(FALSE);
		}
		else
		{
			pMemberGui->SetOtherWorld(FALSE);
		}
	}
}

VOID CPartyMenu::NoPartyButtons()
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	CRectangle rtRect;

	m_pSpreadMenuButton		->Show(false);
	m_pFoldMenuButton		->Show(true);

	m_pFoldMenuButton		->ClickEnable(false);

	m_pMemberMenuButton		->Show(true);	
	m_pDivideZennyButton	->Show(false);
	m_pDivideItemButton		->Show(false);
	m_pDungeonButton		->Show(false);
	//m_pPartyCharmButton		->Show(false);
	m_pSwitchButton			->Show(false);


	rtRect = m_pFoldMenuButton->GetPosition();
	m_pMemberMenuButton->SetPosition(rtRect.right + dBUTTON_GAP, 0);

	rtRect = m_pMemberMenuButton->GetScreenRect();

	m_pThis->SetWidth( rtRect.right - rtScreen.left );
}

VOID CPartyMenu::SpreadButtons()
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	CRectangle rtRect;

	m_pSpreadMenuButton		->Show(false);
	m_pFoldMenuButton		->Show(true);

	m_pFoldMenuButton		->ClickEnable(true);

	m_pMemberMenuButton		->Show(true);	
	m_pDivideZennyButton	->Show(true);
	m_pDivideItemButton		->Show(true);
	m_pDungeonButton		->Show(true);
	//m_pPartyCharmButton		->Show(true);
	m_pSwitchButton			->Show(true);
	

	rtRect = m_pFoldMenuButton->GetPosition();
	m_pMemberMenuButton->SetPosition(rtRect.right + dBUTTON_GAP, 0);

	rtRect = m_pMemberMenuButton->GetPosition();
	m_pDivideZennyButton->SetPosition(rtRect.right + dBUTTON_GAP, 0);	

	rtRect = m_pDivideZennyButton->GetPosition();
	m_pDivideItemButton->SetPosition(rtRect.right + dBUTTON_GAP, 0);

	rtRect = m_pDivideItemButton->GetPosition();
	m_pDungeonButton->SetPosition(rtRect.right + dBUTTON_GAP, 0);

	rtRect = m_pDungeonButton->GetPosition();	
	//m_pPartyCharmButton->SetPosition(rtRect.right + dBUTTON_GAP, 0);

	//rtRect = m_pPartyCharmButton->GetPosition();
	m_pSwitchButton->SetPosition(rtRect.right + dBUTTON_GAP, 0);

	rtRect = m_pSwitchButton->GetScreenRect();

	m_pThis->SetWidth( rtRect.right - rtScreen.left );

	CheckPopup();
}

VOID CPartyMenu::FoldButtons()
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	CRectangle rtRect;

	m_pSpreadMenuButton		->Show(true);
	m_pFoldMenuButton		->Show(false);

	m_pFoldMenuButton		->ClickEnable(true);

	m_pMemberMenuButton		->Show(true);
	m_pDivideZennyButton	->Show(false);
	m_pDivideItemButton		->Show(false);
	m_pDungeonButton		->Show(false);
	//m_pPartyCharmButton		->Show(false);
	m_pSwitchButton			->Show(true);


	rtRect = m_pFoldMenuButton->GetPosition();
	m_pMemberMenuButton->SetPosition(rtRect.right + dBUTTON_GAP, 0);

	rtRect = m_pMemberMenuButton->GetPosition();
	m_pSwitchButton->SetPosition(rtRect.right + dBUTTON_GAP, 0);	

	rtRect = m_pSwitchButton->GetScreenRect();

	m_pThis->SetWidth( rtRect.right - rtScreen.left );
		
	CheckPopup();
}

VOID CPartyMenu::SwitchMemberGui(bool bOpen)
{
	m_MenuShape.bShowMember = bOpen;

	PARTYMEMBER_ITER it = m_listPartyMember.begin();
	for( ; it != m_listPartyMember.end() ; ++it )
	{
		(*it)->Show(m_MenuShape.bShowMember);
	}

	// 파티 버프
	SwitchMemberBuff(m_MenuShape.bShowMember);
}

VOID CPartyMenu::SwitchMemberBuff(bool bOpen)
{
	m_MenuShape.bShowBuff = bOpen;

	PARTYMEMBER_ITER it = m_listPartyMember.begin();
	for( ; it != m_listPartyMember.end() ; ++it )
	{
		CPartyMemberGui* pPartyMemberGui = *it;
		pPartyMemberGui->ShowBuff(m_MenuShape.bShowBuff);
	}
}

VOID CPartyMenu::CheckPopup()
{
	if( GetPopupManager()->IsVisiblePopupGui( PLACE_SUB_PARTY_MENU_MEMBER ) ||
		GetPopupManager()->IsVisiblePopupGui( PLACE_SUB_PARTY_MENU_DIVIDE_ZENNY ) ||
		GetPopupManager()->IsVisiblePopupGui( PLACE_SUB_PARTY_MENU_DIVIDE_ITEM ) ||
		GetPopupManager()->IsVisiblePopupGui( PLACE_SUB_PARTY_MENU_DUNGEON ) ||
		GetPopupManager()->IsVisiblePopupGui( PLACE_SUB_PARTY_MENU_SWITCH ) )
		GetPopupManager()->AllPopupClose();
}

VOID CPartyMenu::MemberButton_UpdateTooltip()
{
	RwBool bEnableButton = FALSE;

	CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
	if( !pWorldConcept )
	{
		bEnableButton = TRUE;		
	}
	else
	{
		if(pWorldConcept->GetConceptType() == WORLD_PLAY_DOJO_SCRAMBLE || 
			pWorldConcept->GetConceptType() == WORLD_PLAY_PARTY_DUNGEON)
		{
			bEnableButton = TRUE;
		}
		else if(pWorldConcept->GetState() == WORLD_STATE_EXIT ||
				 pWorldConcept->GetState() == WORLD_STATE_NONE)
		{
			bEnableButton = TRUE;
		}
	}


	if( bEnableButton )
	{
		m_pMemberMenuButton->ClickEnable(true);
		m_pMemberMenuButton->SetToolTip( GetDisplayStringManager()->GetString("DST_PARTYGUI_MEMBER") );
	}
	else
	{
		m_pMemberMenuButton->ClickEnable(false);

		std::string stringIndex = Logic_GetWarnningWorldConceptActionMessageIndex(WORLD_CONCEPT_FIRST_GRADE);
		if(stringIndex != "" )
			m_pMemberMenuButton->SetToolTip( GetDisplayStringManager()->GetString(stringIndex) );
	}	
}

VOID CPartyMenu::OnClick_SpreadMenuButton(gui::CComponent* pComponent)
{
	SpreadButtons();
}

VOID CPartyMenu::OnClick_FoldMenuButton(gui::CComponent* pComponent)
{
	FoldButtons();
}

VOID CPartyMenu::OnClick_MemberMenuButton(gui::CComponent* pComponent)
{
	CRectangle rtRect = m_pThis->GetScreenRect();
	CDboEventGenerator::IconPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_PARTY_MENU_MEMBER, 0,
									  rtRect.left, rtRect.top);
}

VOID CPartyMenu::OnClick_DevideZennyButton(gui::CComponent* pComponent)
{
	CRectangle rtRect = m_pThis->GetScreenRect();
	CDboEventGenerator::IconPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_PARTY_MENU_DIVIDE_ZENNY, 0,
									  rtRect.left, rtRect.top);
}

VOID CPartyMenu::OnClick_DevideItemButton(gui::CComponent* pComponent)
{
	CRectangle rtRect = m_pThis->GetScreenRect();
	CDboEventGenerator::IconPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_PARTY_MENU_DIVIDE_ITEM, 0,
									  rtRect.left, rtRect.top);
}

VOID CPartyMenu::OnClick_DungeonButton(gui::CComponent* pComponent)
{
	if( Logic_I_am_PartyLeader() == FALSE )
	{
		GetAlarmManager()->AlarmMessage( "GAME_PARTY_ONLY_ALLOWED_TO_PARTY_LEADER", TRUE );
		return;
	}

	CRectangle rtRect = m_pThis->GetScreenRect();
	CDboEventGenerator::IconPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_PARTY_MENU_DUNGEON, 0,
									  rtRect.left, rtRect.top);
}

VOID CPartyMenu::OnClick_SwitchButton(gui::CComponent* pComponent)
{
	CRectangle rtRect = m_pThis->GetScreenRect();
	CDboEventGenerator::IconPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_PARTY_MENU_SWITCH, 0,
									  rtRect.left, rtRect.top);
}

VOID CPartyMenu::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetPosition();
	m_srfPartyNameBackground.SetPositionbyParent(rtScreen.left, rtScreen.top);
}

VOID CPartyMenu::OnPaint()
{
	if( m_pPartyName->IsVisible() )
		m_srfPartyNameBackground.Render();
}

RwInt32 CPartyMenu::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
		SwitchMemberGui(true);
	}	
	else
	{
		Show(false);
		SwitchMemberGui(false);
		CheckPopup();
	}

	return 1;
}

VOID CPartyMenu::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CPartyMenu::HandleEvents");

	if(msg.Id == g_EventPartyUpdateValue)
	{
		SNtlEventPartyUpdate* pPacket = reinterpret_cast<SNtlEventPartyUpdate*>(msg.pData);

		switch(pPacket->iMessage)
		{
		case PMT_PARTY_CREATE:
			{
				GetNtlGuiManager()->AddUpdateFunc( this );
				SpreadButtons();

				CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
				m_pPartyName->SetText( pParty->GetPartyName() );
				m_pPartyName->Show(true);
				break;
			}
		case PMT_PARTY_LEAVE:
			{
				m_pPartyName->Show(false);
				ResetPartyMenu();				
				break;
			}
		case PMT_PARTY_MEMBER_ADD:
			{
				// 자기 자신은 처리하지 않는다.
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if(pAvatar->GetSerialID() == pPacket->hSerialId)
					return;

				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pAvatar->GetParty()->GetMemberbyKey(pPacket->hSerialId) );

				AddMember(pMember);

				break;
			}
		case PMT_PARTY_MEMBER_DEL:
			{				
				DelMember(pPacket->hSerialId);
				break;
			}
		case PMT_LP:
			{
				// LP 알람등이 작동하는 맴버창을 찾는다
				RwReal fEmergencyPulse = 0.f;
				RwBool bState = FALSE;
				PARTYMEMBER_ITER it = m_listPartyMember.begin();
				for( ; it != m_listPartyMember.end() ; ++it )
				{
					fEmergencyPulse = (*it)->GetCurEmergencyPulse();

					if( fEmergencyPulse > 0.f )
					{
						bState = (*it)->GetEmergencyState();
						break;
					}
				}

				// 모든 맴버창의 LP 알람등을 동기화
				it = m_listPartyMember.begin();
				for( ; it != m_listPartyMember.end() ; ++it )
				{
					if( fEmergencyPulse >= 0.f )
					{
						(*it)->SetCurEmergencyPulse(fEmergencyPulse);
						(*it)->SetEmergencyState(bState);
					}
				}
				break;
			}
		}
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		MemberButton_UpdateTooltip();
	}
	else if( msg.Id == g_EventIconPopupResult )
	{
		SDboEventIconPopupResult* pEvent = reinterpret_cast<SDboEventIconPopupResult*>(msg.pData);

		switch( pEvent->nSrcPlace )
		{
		case PLACE_SUB_PARTY_MENU_MEMBER:
			{
				if( pEvent->nWorkId == PMW_PARTY_CREATE )
				{
					// Would you like to make a party?
					GetAlarmManager()->AlarmMessage("DST_PARTY_CREATE");
				}
				else if( pEvent->nWorkId == PMW_PARTY_INVITE )
				{
					SERIAL_HANDLE hTarget = Logic_GetAvatarTargetHandle();
					if( hTarget == INVALID_SERIAL_ID )
					{
						// 타켓이 없습니다
						GetAlarmManager()->AlarmMessage("DST_TARGET_NONE");
						NTL_RETURNVOID();
					}

					if( Logic_I_am_PartyLeader() == FALSE )
					{
						GetAlarmManager()->AlarmMessage( "GAME_PARTY_ONLY_ALLOWED_TO_PARTY_LEADER", TRUE );
						NTL_RETURNVOID();
					}

					if( Logic_CanPartyInvite() == FALSE )
					{
						NTL_RETURNVOID();
					}

					GetDboGlobal()->GetGamePacketGenerator()->SendPartyInvite(hTarget);
				}
				else if( pEvent->nWorkId == PMW_PARTY_LEAVE )
				{
					if( Logic_CanPartyLeave() == FALSE )
					{
						NTL_RETURNVOID();
					}

					GetAlarmManager()->AlarmMessage( "DST_PARTY_LEAVE_ASK" );
				}				

				break;
			}
		case PLACE_SUB_PARTY_MENU_DIVIDE_ZENNY:
			{
				if( pEvent->nWorkId == PMW_PARTY_DIVIDE_PICKUP_PERSON )
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendPartyZennyDivision(NTL_PARTY_ZENNY_LOOTING_GREEDILY);
				}
				else if( pEvent->nWorkId == PMW_PARTY_DIVIDE_EQUAL )
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendPartyZennyDivision(NTL_PARTY_ZENNY_LOOTING_SHARED);
				}

				break;
			}
		case PLACE_SUB_PARTY_MENU_DIVIDE_ITEM:
			{
				if( pEvent->nWorkId == PMW_PARTY_DIVIDE_PICKUP_PERSON )
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendPartyItemDivision(NTL_PARTY_ITEM_LOOTING_GREEDILY);
				}
				else if( pEvent->nWorkId == PMW_PARTY_DIVIDE_ORDER )
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendPartyItemDivision(NTL_PARTY_ITEM_LOOTING_IN_ORDER);
				}
				else if( pEvent->nWorkId == PMW_PARTY_GRADE_ITEM )
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendPartyItemDivision(NTL_PARTY_ITEM_LOOTING_IN_ORDER_SEPERATELY);
				}
				else if( pEvent->nWorkId == PMW_PARTY_USE_INVEN )
				{
					// 기본 파티 인벤 분배 방식
					GetDboGlobal()->GetGamePacketGenerator()->SendPartyItemDivision(NTL_PARTY_ITEM_LOOTING_IN_LEADER);
				}

				break;
			}
		case PLACE_SUB_PARTY_MENU_DUNGEON:
			{
				if( Logic_I_am_PartyLeader() == FALSE )
				{
					GetAlarmManager()->AlarmMessage( "GAME_PARTY_ONLY_ALLOWED_TO_PARTY_LEADER", TRUE );
					NTL_RETURNVOID();
				}

				if( pEvent->nWorkId == PMW_PARTY_DUNGEON_INIT )
				{				
					GetDboGlobal()->GetGamePacketGenerator()->SendPartyDungeonInitReq();
				}
				else if( pEvent->nWorkId == PMW_PARTY_DUNGEON_NORMAL )
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendPartyDungeonChangeReq(PARTY_DUNGEON_STATE_NORMAL);
				}
				else if( pEvent->nWorkId == PMW_PARTY_DUNGEON_HARD )
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendPartyDungeonChangeReq(PARTY_DUNGEON_STATE_HARD);
				}

				break;
			}
		case PLACE_SUB_PARTY_MENU_SWITCH:
			{
				if( pEvent->nWorkId == PMW_PARTY_MENU_SWITCH_MEMBER )
				{
					SwitchMemberGui(!m_MenuShape.bShowMember);
				}
				else if( pEvent->nWorkId == PMW_PARTY_MENU_SWITCH_MEMBER_BUFF )
				{
					SwitchMemberBuff(!m_MenuShape.bShowBuff);				
				}

				break;
			}
		}
	}
	
	NTL_RETURNVOID();
}