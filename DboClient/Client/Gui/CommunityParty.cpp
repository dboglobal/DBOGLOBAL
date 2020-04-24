#include "precomp_dboclient.h"
#include "CommunityParty.h"

// shared
#include "ItemTable.h"
#include "NtlDebug.h"
#include "NtlStringUtil.h"
#include "NtlResultCode.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLGlobal.h"
#include "InputActionMap.h"
#include "NtlSobAvatar.h"
#include "NtlSLLogic.h"
#include "NtlSLEventFunc.h"

// dbo
#include "InfoWndManager.h"
#include "IconMoveManager.h"
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "ChatGui.h"
#include "PartyCharmGui.h"
#include "CommunityGui.h"
#include "SideDialogManager.h"


#define dMEMBER_GAP					23

#define dCOLOR_NORMAL_DIVISION		RGB(255, 192, 0)
#define dCOLOR_SELECT_DIVISION		RGB(176, 239, 65)

#define dCOLOR_PARTY_LEADER			NTL_PARTY_NAME_COLOR
#define dCOLOR_PARTY_MEMBER			RGB(255, 255, 255)


CCommunityParty::CCommunityParty()
:m_pParent(NULL)
,m_byItemDivisionType(NTL_PARTY_ITEM_LOOTING_GREEDILY)
,m_byZennyDivisionType(NTL_PARTY_ZENNY_LOOTING_GREEDILY)
,m_bySelectedItemDivisionType(NTL_PARTY_ITEM_LOOTING_GREEDILY)
,m_bySelectedZennyDivisionType(NTL_PARTY_ZENNY_LOOTING_GREEDILY)
,m_iLeaderIndex(0)
,m_byInfoWindowIndex(INVALID_INDEX)
{

}

CCommunityParty::~CCommunityParty()
{
	Destroy();
}

RwBool CCommunityParty::Create(CNtlPLGui* pParent)
{
	NTL_FUNCTION( "CCommunityParty::Create" );
	CRectangle rect;
	gui::CSurface tempSurface;
	CNtlOtherParam* pOtherParam = GetNtlSLGlobal()->GetSobAvatar()->GetOtherParam();

	m_pParent = pParent;

	// '파티이름'
	rect.SetRectWH(26, 65, 50, 14);
	m_pPartyName = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pPartyName->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	
	m_pPartyName->SetText(GetDisplayStringManager()->GetString(DST_PARTYGUI_PARTY_NAME));
	m_pPartyName->Enable(false);

	// 파티 이름 백보드
	m_PartyNameBack.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "PartyNameInputBack" ) );
	m_PartyNameBack.SetPositionfromParent(86, 63);

	// 현재 파티 이름
	rect.SetRectWH(100, 63, 100, 19);
	m_pMyPartyNameStatic = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pMyPartyNameStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pMyPartyNameStatic->SetTextColor(RGB(186, 186, 186));
	m_pMyPartyNameStatic->Clear();
	m_pMyPartyNameStatic->Enable(false);

	// 파티 이름 입력란
	m_pPartNameInput = (gui::CInputBox*)m_pParent->GetComponent( "PartyNameInput" );
	m_pPartNameInput->SetMaxLength(NTL_MAX_SIZE_PARTY_NAME_IN_UNICODE);
	m_pPartNameInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);
	m_slotReturnInput = m_pPartNameInput->SigReturnPressed().Connect(this, &CCommunityParty::OnReturn_PartyNameInput);


	// 파티 생성 버튼
	m_pPartyButtonCreate = (gui::CButton*)m_pParent->GetComponent("PartyButtonCreate");		
	m_pPartyButtonCreate->SetTextStyle(COMP_TEXT_CENTER);
	m_pPartyButtonCreate->SetTextUpColor(NTL_BUTTON_UP_COLOR);
	m_pPartyButtonCreate->SetTextDownColor(NTL_BUTTON_DOWN_COLOR);
	m_pPartyButtonCreate->SetTextDisableColor(RGB(100, 100, 100));
	m_pPartyButtonCreate->SetTextFont( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pPartyButtonCreate->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_PARTY_CREATE) );
	m_pPartyButtonCreate->ApplyText();
	m_slotPartyCreateButton = m_pPartyButtonCreate->SigClicked().Connect(this, &CCommunityParty::ClickedPartyCreateButton);


	// 'Zenny' 서페이스
	rect = m_pParent->GetPosition();
	m_ZennySurface.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Community.srf", "srfZennySurface") );
	m_ZennySurface.SetPositionfromParent(26, 89);

	// 제니 분배 방식	
	m_ZennyDivSurface[ZDT_PICKUP_PERSON].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Community.srf", "srfZennyDivPickup") );
	m_ZennyDivSurface[ZDT_PICKUP_PERSON].SetPositionfromParent(23, 101);
	m_ZennyDivSurface[ZDT_EQUAL].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Community.srf", "srfZennyDivNumPeople") );
	m_ZennyDivSurface[ZDT_EQUAL].SetPositionfromParent(23, 101);

	// 제니 분배 방식 변경 버튼
	m_pSelectZennyDivButton = (gui::CButton*)m_pParent->GetComponent("btnSelectZennyDivButton");		
	m_pSelectZennyDivButton->SetToolTip(GetDisplayStringManager()->GetString(DST_PARTYGUI_ZENNY_LOOTING));
	m_slotSelectZennyDivButton = m_pSelectZennyDivButton->SigClicked().Connect(this, &CCommunityParty::ClickedSelectDivButton);

	// 제니 분배 설명 배경
	rect = m_pParent->GetPosition();
	m_ZennyExplainPanel.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Community.srf", "srfExplainPanel") );
	m_ZennyExplainPanel.SetPositionfromParent(96, 94);

	// 제니 분배 설명
	rect.SetRectWH(104, 98, 146, 31);
	m_pZennyDivExplain = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pZennyDivExplain->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pZennyDivExplain->SetTextColor(dCOLOR_NORMAL_DIVISION);
	m_pZennyDivExplain->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_ZENNY_LOOTING_GREEDILY) );
	m_pZennyDivExplain->Enable(false);

	// 제니 분배 잠금 버튼
	m_pZennyLockButton = (gui::CButton*)m_pParent->GetComponent("btnZennyLockButton");		
	m_slotZennyLockButton = m_pZennyLockButton->SigClicked().Connect(this, &CCommunityParty::ClickedLockButton);



	// 'Item' 서페이스
	rect = m_pParent->GetPosition();
	m_ItemSurface.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Community.srf", "srfItemSurface") );
	m_ItemSurface.SetPositionfromParent(31, 133);

	// 아이템 분배 방식	
	m_ItemDivSurface[IDT_PICKUP_PERSON].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Community.srf", "srfItemDivPickup") );
	m_ItemDivSurface[IDT_PICKUP_PERSON].SetPositionfromParent(23, 144);
	m_ItemDivSurface[IDT_ORDER].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Community.srf", "srfItemDivOrder") );
	m_ItemDivSurface[IDT_ORDER].SetPositionfromParent(23, 144);
	m_ItemDivSurface[IDT_GRADE_ITEM].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Community.srf", "srfItemDivGrade") );
	m_ItemDivSurface[IDT_GRADE_ITEM].SetPositionfromParent(23, 144);
	m_ItemDivSurface[IDT_CONTIBUTION].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Community.srf", "srfItemDivPoint") );
	m_ItemDivSurface[IDT_CONTIBUTION].SetPositionfromParent(23, 144);

	// 아이템 분배 방식 변경 버튼
	m_pSelectItemDivButton = (gui::CButton*)m_pParent->GetComponent("btnSelectItemDivButton");		
	m_pSelectItemDivButton->SetToolTip(GetDisplayStringManager()->GetString(DST_PARTYGUI_ITEM_LOOTING));
	m_slotSelectItemDivButton = m_pSelectItemDivButton->SigClicked().Connect(this, &CCommunityParty::ClickedSelectDivButton);

	// 아이템 분배 설명 배경
	rect = m_pParent->GetPosition();
	m_ItemExplainPanel.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Community.srf", "srfExplainPanel") );
	m_ItemExplainPanel.SetPositionfromParent(96, 137);

	// 아이템 분배 설명
	rect.SetRectWH(104, 141, 146, 31);
	m_pItemDivExplain = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pItemDivExplain->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pItemDivExplain->SetTextColor(dCOLOR_NORMAL_DIVISION);
	m_pItemDivExplain->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_ITEM_LOOTING_GREEDILY) );
	m_pItemDivExplain->Enable(false);

	// 아이템 분배 잠금 버튼
	m_pItemLockButton = (gui::CButton*)m_pParent->GetComponent("btnItemLockButton");		
	m_slotItemLockButton = m_pItemLockButton->SigClicked().Connect(this, &CCommunityParty::ClickedLockButton);



	// '파티 맴버 해드라인'
	m_MemberHeadLine.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "PartyHeadLine" ) );
	m_MemberHeadLine.SetPositionfromParent(11, 189);

	///< '파티 맴버'
	rect.SetRectWH(29, 191, 122, 13);
	m_MemberStatic = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_MemberStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_MemberStatic->SetText(GetDisplayStringManager()->GetString(DST_PARTYGUI_MEMBER));
	m_MemberStatic->Enable(false);

	///< '기여점수'
	rect.SetRectWH(237, 195, 62, 13);
	m_ScoreStatic = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_ScoreStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_ScoreStatic->SetText(GetDisplayStringManager()->GetString(DST_PARTYGUI_SCORE));
	m_ScoreStatic->Enable(false);

	// 파티장 마크
	m_LeaderMark.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "srfPartyLeaderMark" ) );

	// 맴버 리스트 배경
	m_srfMemberboard.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "Memberboard" ) );
	m_srfMemberboard.SetPositionfromParent(37, 212);

	// 파티원 이름, 기여도
	RwInt32 iYPos = 212;
	for( RwInt32 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		rect.SetRectWH(60, iYPos + 3, 154, 14);
		m_PartyMemberInfo[i].pName = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
		m_PartyMemberInfo[i].pName->CreateFontStd("detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_PartyMemberInfo[i].pName->SetTextColor( dCOLOR_PARTY_MEMBER );
		m_PartyMemberInfo[i].pName->Enable(false);		
		m_PartyMemberInfo[i].pName->Clear();

		rect.SetRectWH(244, iYPos + 4, 48, 12);
		m_PartyMemberInfo[i].pScore = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
		m_PartyMemberInfo[i].pScore->CreateFontStd("detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_PartyMemberInfo[i].pScore->SetTextColor( dCOLOR_PARTY_MEMBER );
		m_PartyMemberInfo[i].pScore->Enable(false);		
		m_PartyMemberInfo[i].pScore->Clear();

		rect.SetRectWH(29, iYPos, 17, 18);
		m_PartyMemberInfo[i].pTributarySimbol = NTL_NEW gui::CPanel( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager() );

		iYPos += dMEMBER_GAP;
	}


	// 인벤 버튼
	m_PartyInvenButton = (gui::CButton*)m_pParent->GetComponent("InvenButton");
	m_PartyInvenButton->SetToolTip(GetDisplayStringManager()->GetString(DST_PARTYGUI_INVEN));
	m_slotInvenButton = m_PartyInvenButton->SigClicked().Connect(this, &CCommunityParty::OnClicked_InvenButton);

	// 부적 버튼
	m_pCharmButton = (gui::CButton*)m_pParent->GetComponent("CharmButton");
	m_pCharmButton->SetTextStyle(COMP_TEXT_CENTER);
	m_pCharmButton->SetTextUpColor(NTL_BUTTON_UP_COLOR);
	m_pCharmButton->SetTextDownColor(NTL_BUTTON_DOWN_COLOR);
	m_pCharmButton->SetTextFont( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	
	m_pCharmButton->SetToolTip(GetDisplayStringManager()->GetString(DST_PARTYGUI_CHARM));
	m_pCharmButton->ApplyText();
	m_slotCharmButton = m_pCharmButton->SigClicked().Connect(this, &CCommunityParty::OnClicked_CharmButton);

	// 부적 패널
	m_CharmBackpanel.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "CharmBackpanel" ) );
	m_CharmBackpanel.SetPositionfromParent(218, 328);

	// 부적 슬롯
	m_PartyCharmSlot[0].Create(m_pParent->GetDialog(), DIALOG_COMMUNITY, ISST_ITEM_TABLE_SERIAL);
	m_PartyCharmSlot[0].SetPosition(220, 330);

	m_PartyCharmSlot[1].Create(m_pParent->GetDialog(), DIALOG_COMMUNITY, ISST_ITEM_TABLE_SERIAL);
	m_PartyCharmSlot[1].SetPosition(267, 330);



	// 'CP 해드라인'
	m_CPHeadLine.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "PartyHeadLine" ) );
	m_CPHeadLine.SetPositionfromParent(11, 372);

	// 'CP 변환'
	rect.SetRectWH(29, 374, 122, 13);
	m_CPStatic = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_CPStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_CPStatic->SetText(GetDisplayStringManager()->GetString(DST_PARTYGUI_CP));
	m_CPStatic->Enable(false);

	// 실제 보유 CP
	rect.SetRectWH(31, 398, 100, 25);
	m_CPPoint = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_CPPoint->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_CPPoint->SetText( pOtherParam->GetCharmPoint() );
	m_CPPoint->Enable(false);

	// CP 설명
	rect.SetRectWH(20, 425, 217, 31);
	m_CPExplain = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_CPExplain->CreateFontStd( "detail", 80, DEFAULT_FONT_ATTR);
	m_CPExplain->SetTextColor( RGB(192, 192, 192) );
	m_CPExplain->SetText(GetDisplayStringManager()->GetString(DST_PARTYGUI_CP_EXPLAIN));
	m_CPExplain->Enable(false);

	// CP 버튼
	m_pCPButton = (gui::CButton*)m_pParent->GetComponent("CPButton");
	m_slotCPButton = m_pCPButton->SigClicked().Connect(this, &CCommunityParty::OnClicked_CPButton);

	// 아이템을 CP로 전환 버튼
	m_pItemtoCPButton = (gui::CButton*)m_pParent->GetComponent("ItemtoCPButton");
	m_slotItemtoCPButton = m_pItemtoCPButton->SigClicked().Connect(this, &CCommunityParty::OnClicked_ItemtoCPButton);

	LockZennyButton(true);
	LockItemButton(true);

	// Sig
	m_slotGotFocus	= m_pPartNameInput->SigGotFocus().Connect( this, &CCommunityParty::OnGotFocus );
	m_slotLostFocus = m_pPartNameInput->SigLostFocus().Connect( this, &CCommunityParty::OnLostFocus );

	CalcSurfacePosition();

	NTL_RETURN(TRUE);
}

VOID CCommunityParty::Destroy()
{
	NTL_FUNCTION("CCommunityParty::Destroy");	

	for(RwInt32 i = 0 ; i < NTL_PARTY_CHARM_INVENTORY_COUNT ; ++i )
	{
		m_PartyCharmSlot[i].Destroy();
	}

	NTL_RETURNVOID();
}

VOID CCommunityParty::Clear()
{
	m_iLeaderIndex = 0;

	m_pPartNameInput->Clear();

	for( RwInt32 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		DelMember(i);
	}

	m_bySelectedItemDivisionType = NTL_PARTY_ITEM_LOOTING_GREEDILY;
	m_bySelectedZennyDivisionType = NTL_PARTY_ZENNY_LOOTING_GREEDILY;

	SetZennyDivision(NTL_PARTY_ITEM_LOOTING_GREEDILY);
	SetItemDivision(NTL_PARTY_ZENNY_LOOTING_GREEDILY);

	LockZennyButton(true);
	LockItemButton(true);

	CRectangle rectParent = m_pParent->GetPosition();	
	m_PartyNameBack.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "PartyNameInputBack" ) );
	m_PartyNameBack.SetPositionfromParent(86, 63);
	m_PartyNameBack.SetPositionbyParent(rectParent.left, rectParent.top);
}

VOID CCommunityParty::PartyLeave()
{
	Clear();

	CCommunityGui* pGui = reinterpret_cast<CCommunityGui*>( GetDialogManager()->GetDialog(DIALOG_COMMUNITY) );
	if( pGui->GetPageType() == CCommunityGui::PAGETYPE_PARTY )
		ActiveTab(true);
	else
		ActiveTab(false);

	m_pPartyButtonCreate->ClickEnable(true);

	for(RwInt32 i = 0 ; i < NTL_PARTY_CHARM_INVENTORY_COUNT ; ++i )
		m_PartyCharmSlot[i].Clear();
}

VOID CCommunityParty::Update(RwReal fElapsed)
{
	if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_CHARM_BUFF ||
		GetInfoWndManager()->GetRequestGui() == DIALOG_PARTYMENU )
	{
		CRectangle rtScreen = m_pParent->GetDialog()->GetScreenRect();		
		RwInt32 iX = CMouse::m_nX - rtScreen.left;
		RwInt32 iY = CMouse::m_nY - rtScreen.top;

		if( m_PartyCharmSlot[m_byInfoWindowIndex].PtInRect(iX, iY) )
		{
			if( m_PartyCharmSlot[m_byInfoWindowIndex].GetSerial() != INVALID_SERIAL_ID )
			{
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_CHARM_BUFF,
					rtScreen.left + m_PartyCharmSlot[m_byInfoWindowIndex].GetXPosition(),
					rtScreen.top + m_PartyCharmSlot[m_byInfoWindowIndex].GetYPosition(),
					m_PartyCharmSlot[m_byInfoWindowIndex].GetItemTable(), DIALOG_COMMUNITY );
			}

			return;
		}
	}
}

VOID CCommunityParty::ActiveTab(bool bActive)
{
	m_pPartyName->Show(bActive);	

	CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
	if( bActive )
	{
		if( pParty->IsHaveGroup() )
		{
			m_pPartNameInput->Show(false);
			m_pMyPartyNameStatic->Show(true);
		}
		else
		{
			m_pPartNameInput->Show(true);
			m_pMyPartyNameStatic->Show(false);
		}
	}
	else
	{
		m_pMyPartyNameStatic->Show(false);
		m_pPartNameInput->Show(false);

		CheckInfoWindow();
	}

	m_pPartyButtonCreate->Show(bActive);

	m_pSelectZennyDivButton->Show(bActive);
	m_pZennyDivExplain->Show(bActive);
	m_pZennyLockButton->Show(bActive);

	m_pSelectItemDivButton->Show(bActive);
	m_pItemDivExplain->Show(bActive);
	m_pItemLockButton->Show(bActive);

	m_MemberStatic->Show(bActive);
	m_ScoreStatic->Show(bActive);

	for(RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i)
	{
		m_PartyMemberInfo[i].pName->Show(bActive);
		m_PartyMemberInfo[i].pScore->Show(bActive);
		m_PartyMemberInfo[i].pTributarySimbol->Show(bActive);
	}

	m_CPStatic->Show(bActive);
	m_CPPoint->Show(bActive);
	m_CPExplain->Show(bActive);
	m_pCharmButton->Show(bActive);
	m_pCPButton->Show(bActive);
	m_pItemtoCPButton->Show(bActive);

	m_PartyInvenButton->Show(bActive);	
}

VOID CCommunityParty::AdjustSurface(gui::CSurface& surface)
{
	surface.m_Original.rtRect.right -= surface.m_Original.rtRect.left;
	surface.m_Original.rtRect.left = 0;	
	surface.m_Original.rtRect.bottom -= surface.m_Original.rtRect.top;
	surface.m_Original.rtRect.top = 0;

	surface.m_SnapShot = surface.m_Original;
}

VOID CCommunityParty::OnReturn_PartyNameInput()
{
	ClickedPartyCreateButton(NULL);
	GetInputActionMap()->SetActive(TRUE);
}

VOID CCommunityParty::ClickedPartyCreateButton(gui::CComponent* pComponent)
{
	if( GetNtlSLGlobal()->GetSobAvatar()->GetParty()->IsHaveGroup() )
	{
		m_pPartNameInput->Clear();
		return;
	}

	const WCHAR* pcPartyName = m_pPartNameInput->GetText();
	if(!pcPartyName)
	{
		m_pPartNameInput->Clear();
		return;
	}
	else if( GetChattingFilter()->IsSlang(pcPartyName) )
	{
		// 사용할 수 없는 단어가 있습니다
		GetAlarmManager()->AlarmMessage(DST_LOBBY_CAN_NOT_USE_THE_WORD);
		return;
	}

	RwInt32 iNumByte = GetByte(pcPartyName);
	if( NTL_MIN_SIZE_PARTY_NAME > iNumByte || iNumByte > NTL_MAX_SIZE_PARTY_NAME )
	{
		m_pPartNameInput->Clear();
		GetAlarmManager()->AlarmMessage(GAME_PARTY_NOT_PROPER_PARTY_NAME_LENGTH, TRUE);
		return;
	}

	CGamePacketGenerator *pGamePacketGenerator = GetDboGlobal()->GetGamePacketGenerator();
	pGamePacketGenerator->SendPartyCreate(pcPartyName);
}

VOID CCommunityParty::ClickedSelectDivButton(gui::CComponent* pComponent)
{
	if( !Logic_I_am_PartyLeader() )
	{
		// DST_PARTYGUI_ONLY_LEADER
		GetAlarmManager()->AlarmMessage(DST_PARTYGUI_ONLY_LEADER);
		return;
	}

	if( pComponent == m_pSelectItemDivButton )
	{
		SetItemDivision(m_byItemDivisionType + 1);
	}
	else if( pComponent == m_pSelectZennyDivButton )
	{
		SetZennyDivision(m_byZennyDivisionType + 1);
	}
}

VOID CCommunityParty::ClickedLockButton(gui::CComponent* pComponent)
{
	if( !Logic_I_am_PartyLeader() )
	{
		GetAlarmManager()->AlarmMessage(DST_PARTYGUI_ONLY_LEADER);
		return;
	}

	if( pComponent == m_pZennyLockButton )
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendPartyZennyDivision(m_byZennyDivisionType);
	}
	else if( pComponent == m_pItemLockButton )
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendPartyItemDivision(m_byItemDivisionType);
	}
}

VOID CCommunityParty::OnClicked_InvenButton(gui::CComponent* pComponent)
{
	if(GetNtlSLGlobal()->GetSobAvatar()->GetParty()->IsHaveGroup() == false)
	{
		GetAlarmManager()->AlarmMessage( GAME_PARTY_YOU_ARE_NOT_IN_PARTY, TRUE );
		return;
	}

	GetSideDialogManager()->SwitchDialog(SDIALOG_PARTY_INVEN);
}

VOID CCommunityParty::OnClicked_CharmButton(gui::CComponent* pComponent)
{
	// 파티장만이 열 수 있다.
	if( Logic_I_am_PartyLeader() == false )
	{
		GetAlarmManager()->AlarmMessage( GAME_PARTY_ONLY_ALLOWED_TO_PARTY_LEADER, TRUE );
		return;
	}

	if( GetSideDialogManager()->IsOpenDialog(SDIALOG_PARTY_CHRAM) )
		return;

	CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();

	for( RwInt8 i = 0 ; i < NTL_PARTY_CHARM_INVENTORY_COUNT ; ++i )
	{
		// 빈 슬롯만 열 수 있다
		const sPartyCharm& rPartyCharm = pParty->GetCharmbyIndex(i);

		if( rPartyCharm.pITEM_TBLDAT == NULL && 
			m_PartyCharmSlot[i].GetSerial() == INVALID_SERIAL_ID )
		{
			CGamePacketGenerator *pGamePacketGenerator = GetDboGlobal()->GetGamePacketGenerator();
			pGamePacketGenerator->SendPartyOpenCharmSlot(i);
			break;
		}
	}	
}

VOID CCommunityParty::OnClicked_CPButton(gui::CComponent* pComponent)
{
	CNtlOtherParam* pOtherParam = GetNtlSLGlobal()->GetSobAvatar()->GetOtherParam();		

	CRectangle rtCPButton = m_pCPButton->GetScreenRect();
	CDboEventGenerator::CalcPopupShow(true, INVALID_SERIAL_ID, PLACE_SUB_PARTY_CP, 
		rtCPButton.right, rtCPButton.bottom, pOtherParam->GetCharmPoint());
}

VOID CCommunityParty::OnClicked_ItemtoCPButton(gui::CComponent* pComponent)
{
	if( GetIconMoveManager()->IsActive() )
	{
		if( PLACE_BAG != (EPlace)GetIconMoveManager()->GetSrcPlace() )
			return;

		sDboItemDeleteInfo sInfo;
		sInfo.ePlace = (EPlace)GetIconMoveManager()->GetSrcPlace();
		sInfo.hSerial = GetIconMoveManager()->GetSrcSerial();
		sInfo.nSlotIdx = GetIconMoveManager()->GetSrcSlotIdx();

		sMsgBoxData data;
		data.sItemDeleteInfo = sInfo;

		// 아이템을 CP로 전환하시겠습니까?
		GetAlarmManager()->AlarmMessage( DST_PARTY_GET_CP_ASK, FALSE, &data );
		//CDboEventGenerator::MsgBoxShow( GetDisplayStringManager()->GetString( DST_PARTY_GET_CP_ASK ), MBW_GET_CP_FROM_ITEM, MBT_OKCANCEL, FALSE, 0.0f, &sInfo );
		GetIconMoveManager()->IconMoveEnd();

		// 가방의 아이템 락
		CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
		CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(sInfo.hSerial) );
		NTL_ASSERT(pSrcItem, "CCommunityParty::OnClicked_ItemtoCPButton, Not exist sob item of handle : " << sInfo.hSerial);
		RwInt32 nBagSlotIdx = pInventory->FindBagSlot( pSrcItem->GetParentItemSerial() );
		NTL_ASSERT(0 <= nBagSlotIdx, "CCommunityParty::OnClicked_ItemtoCPButton, Bag is not found in BagSlot");

		CDboEventGenerator::DialogEvent(TRM_BEGIN_TRADING_ITEM_IN_BAG, PLACE_SUB_PARTY_CP, PLACE_BAG, 
			nBagSlotIdx, sInfo.nSlotIdx, sInfo.hSerial);
	}
}

VOID CCommunityParty::LocateLeaderMark(RwInt32 iIndex)
{
	m_iLeaderIndex = iIndex;

	CRectangle rectParent = m_pParent->GetPosition();
	m_LeaderMark.SetPositionfromParent(34, 206 + dMEMBER_GAP*(m_iLeaderIndex-1));
	m_LeaderMark.SetPositionbyParent(rectParent.left, rectParent.top);
}

VOID CCommunityParty::SetZennyDivision(RwUInt8 byIndex)
{
	if( byIndex >= NTL_PARTY_ZENNY_LOOTING_COUNT )
		byIndex = NTL_PARTY_ZENNY_LOOTING_GREEDILY;

	CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();

	m_byZennyDivisionType = byIndex;
	m_pZennyDivExplain->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_ZENNY_LOOTING_GREEDILY + byIndex) );

	if( pParty->IsHaveGroup() && m_byZennyDivisionType == m_bySelectedZennyDivisionType )
		m_pZennyDivExplain->SetTextColor(dCOLOR_SELECT_DIVISION, true);
	else
		m_pZennyDivExplain->SetTextColor(dCOLOR_NORMAL_DIVISION, true);
}

VOID CCommunityParty::SetItemDivision(RwUInt8 byIndex)
{
	if( byIndex >=	NTL_PARTY_ITEM_LOOTING_COUNT )
		byIndex = NTL_PARTY_ITEM_LOOTING_GREEDILY;

	CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();

	m_byItemDivisionType = byIndex;
	m_pItemDivExplain->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_ITEM_LOOTING_GREEDILY + byIndex) );

	if( pParty->IsHaveGroup() && m_byItemDivisionType == m_bySelectedItemDivisionType )
		m_pItemDivExplain->SetTextColor(dCOLOR_SELECT_DIVISION, true);
	else
		m_pItemDivExplain->SetTextColor(dCOLOR_NORMAL_DIVISION, true);
}

VOID CCommunityParty::LockZennyButton(bool bLock)
{
	char acBuffer[256] = "";

	m_pZennyLockButton->ClickEnable(bLock);

	if( bLock )
	{
		WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString(DST_PARTY_NOTIFY_DIVISION_INFO),-1, acBuffer, 256, NULL, NULL);
		m_pZennyLockButton->SetToolTip(acBuffer);
	}
	else
	{
		WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString(DST_PARTY_NOTIFY_WAIT_DIVISION), -1, acBuffer, 256, NULL, NULL);
		m_pZennyLockButton->SetToolTip(acBuffer);
	}
}

VOID CCommunityParty::LockItemButton(bool bLock)
{	
	char acBuffer[256] = "";

	m_pItemLockButton->ClickEnable(bLock);

	if( bLock )
	{
		WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString(DST_PARTY_NOTIFY_DIVISION_INFO), -1, acBuffer, 256, NULL, NULL);
		m_pItemLockButton->SetToolTip(acBuffer);
	}
	else
	{
		WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString(DST_PARTY_NOTIFY_WAIT_DIVISION), -1, acBuffer, 256, NULL, NULL);
		m_pItemLockButton->SetToolTip(acBuffer);

	}	
}

VOID CCommunityParty::SetMember(sPartyMember* pMember, RwUInt8 byIndex)
{
	CRectangle rtRect = m_PartyMemberInfo[byIndex].pTributarySimbol->GetPosition();
	std::list<gui::CSurface>* pSurfaceList = m_PartyMemberInfo[byIndex].pTributarySimbol->GetSurface();
	pSurfaceList->clear();

	m_PartyMemberInfo[byIndex].hSerial = pMember->hSerial;

	m_PartyMemberInfo[byIndex].pTributarySimbol->AddSurface( Logic_GetPCClassIconSurface(pMember->byClass, false) );
	m_PartyMemberInfo[byIndex].pTributarySimbol->SetToolTip( Logic_GetClassName(pMember->byClass) );
	m_PartyMemberInfo[byIndex].pTributarySimbol->SetPosition(rtRect.left, rtRect.top);

	m_PartyMemberInfo[byIndex].pName->SetText(pMember->wszMemberName);
	m_PartyMemberInfo[byIndex].pScore->SetText(pMember->iScore);	
}

VOID CCommunityParty::DelMember(RwUInt8 byIndex)
{
	std::list<gui::CSurface>* pSurfaceList = m_PartyMemberInfo[byIndex].pTributarySimbol->GetSurface();
	CRectangle rtRect = m_PartyMemberInfo[byIndex].pTributarySimbol->GetPosition();

	m_PartyMemberInfo[byIndex].hSerial = INVALID_SERIAL_ID;

	pSurfaceList->clear();
	m_PartyMemberInfo[byIndex].pTributarySimbol->SetPosition(rtRect.left, rtRect.top);
	m_PartyMemberInfo[byIndex].pTributarySimbol->DeleteToolTip();

	m_PartyMemberInfo[byIndex].pName->Clear();
	m_PartyMemberInfo[byIndex].pScore->Clear();
}

VOID CCommunityParty::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_COMMUNITY )
	{
		CCommunityGui* pGui = reinterpret_cast<CCommunityGui*>( GetDialogManager()->GetDialog(DIALOG_COMMUNITY) );
		if( pGui->GetPageType() == CCommunityGui::PAGETYPE_PARTY )
		{
			if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_CHARM_BUFF ||
				GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_JUST_WTEXT )
			{
				m_byInfoWindowIndex = INVALID_INDEX;
				GetInfoWndManager()->ShowInfoWindow( FALSE );
			}
		}
	}
}

VOID CCommunityParty::OnMouseDown(const CKey& key)
{
}

VOID CCommunityParty::OnMouseUp(const CKey& key)
{

}

VOID CCommunityParty::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CalcSurfacePosition();

	CRectangle rtScreen = m_pParent->GetPosition();

	for( RwUInt8 i = 0 ; i < NUM_ZDT ; ++i )
		m_ZennyDivSurface[i].SetPositionbyParent(rtScreen.left, rtScreen.top);

	for( RwUInt8 i = 0 ; i < NUM_IDT ; ++i )
		m_ItemDivSurface[i].SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_ZennySurface.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_ItemSurface.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_ZennyExplainPanel.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_ItemExplainPanel.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_LeaderMark.SetPositionbyParent(rtScreen.left, rtScreen.top);

	CheckInfoWindow();
}

VOID CCommunityParty::CalcSurfacePosition()
{
	CRectangle rectParent = m_pParent->GetPosition();	

	m_PartyNameBack.SetPositionbyParent(rectParent.left, rectParent.top);

	m_MemberHeadLine.SetPositionbyParent(rectParent.left, rectParent.top);
	m_CPHeadLine.SetPositionbyParent(rectParent.left, rectParent.top);

	m_srfMemberboard.SetPositionbyParent(rectParent.left, rectParent.top);

	m_CharmBackpanel.SetPositionbyParent(rectParent.left, rectParent.top);

	m_PartyCharmSlot[0].SetParentPosition(rectParent.left, rectParent.top);
	m_PartyCharmSlot[1].SetParentPosition(rectParent.left, rectParent.top);

	LocateLeaderMark(m_iLeaderIndex);	
}
VOID CCommunityParty::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	CRectangle rtScreen = m_pParent->GetDialog()->GetScreenRect();

	// 아이콘 영역으로 마우스를 이동시켜서 아이템 정보를 보일 필요가 있는지 검사
	for( RwInt32 i = 0 ; i < NTL_PARTY_CHARM_INVENTORY_COUNT ; ++i )
	{
		if( m_PartyCharmSlot[i].PtInRect(nX, nY) )
		{
			if( m_PartyCharmSlot[i].GetSerial() != INVALID_SERIAL_ID &&
				m_byInfoWindowIndex != i )
			{
				m_byInfoWindowIndex = i;
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_CHARM_BUFF,
					rtScreen.left + m_PartyCharmSlot[i].GetXPosition(),
					rtScreen.top + m_PartyCharmSlot[i].GetYPosition(),
					m_PartyCharmSlot[i].GetItemTable(), DIALOG_COMMUNITY );
			}

			return;
		}
	}

	m_byInfoWindowIndex = INVALID_INDEX;

	if( m_LeaderMark.PtInRect(rtScreen.left + nX, rtScreen.top + nY) )
	{
		CRectangle rtRect;
		m_LeaderMark.GetRect(rtRect);
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
			rtRect.left, rtRect.top, (VOID*)GetDisplayStringManager()->GetString(DST_PARTYGUI_PARTYLEADER), DIALOG_COMMUNITY );
		return;
	}

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CCommunityParty::OnMouseLeave(gui::CComponent* pComponent)
{
	m_byInfoWindowIndex = INVALID_INDEX;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CCommunityParty::OnGotFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = TRUE;
}

VOID CCommunityParty::OnLostFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = FALSE;
}

VOID CCommunityParty::OnPaint()
{
	m_PartyNameBack.Render();

	m_ZennyDivSurface[m_byZennyDivisionType].Render();
	m_ItemDivSurface[m_byItemDivisionType].Render();

	m_ZennySurface.Render();
	m_ItemSurface.Render();
	m_ZennyExplainPanel.Render();
	m_ItemExplainPanel.Render();

	m_MemberHeadLine.Render();

	m_CPHeadLine.Render();

	m_srfMemberboard.Render();

	m_CharmBackpanel.Render();

	if(m_iLeaderIndex > 0)
		m_LeaderMark.Render();

	for(RwInt32 i = 0 ; i < NTL_PARTY_CHARM_INVENTORY_COUNT ; ++i )
	{
		m_PartyCharmSlot[i].Paint();
	}	
}

VOID CCommunityParty::SwitchDialog(bool bOpen)
{
	if(bOpen)
	{
		m_byItemDivisionType = m_bySelectedItemDivisionType;
		m_byZennyDivisionType = m_bySelectedZennyDivisionType;

		m_pItemDivExplain->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_ITEM_LOOTING_GREEDILY + m_bySelectedItemDivisionType) );
		m_pZennyDivExplain->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_ZENNY_LOOTING_GREEDILY + m_bySelectedZennyDivisionType) );

		if( m_pPartyButtonCreate->IsClickEnable() )
			m_pPartNameInput->SetFocus();
	}
	else
	{
		CheckInfoWindow();		
	}
}

VOID CCommunityParty::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CCommunityParty::HandleEvents");

	if(msg.Id == g_EventPartyUpdateValue)
	{
		SNtlEventPartyUpdate* pPacket = reinterpret_cast<SNtlEventPartyUpdate*>(msg.pData);

		switch(pPacket->iMessage)
		{
		case PMT_PARTY_CREATE:
			{	
				const WCHAR* pwcText = GetNtlSLGlobal()->GetSobAvatar()->GetParty()->GetPartyName();

				m_pMyPartyNameStatic->SetText(pwcText);
				m_pPartyButtonCreate->ClickEnable(false);

				CCommunityGui* pGui = reinterpret_cast<CCommunityGui*>( GetDialogManager()->GetDialog(DIALOG_COMMUNITY) );
				if( pGui->GetPageType() == CCommunityGui::PAGETYPE_PARTY )
					ActiveTab(true);


				SetZennyDivision(m_bySelectedZennyDivisionType);
				SetItemDivision(m_bySelectedItemDivisionType);

				m_PartyNameBack.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "PartyNameBack" ) );
				m_PartyNameBack.SetPositionfromParent(86, 63);

				CalcSurfacePosition();

				break;
			}
		case PMT_PARTY_LEAVE:
			{
				PartyLeave();
				break;
			}
		case PMT_PARTY_MEMBER_ADD:
			{
				// 가장 마지막에 추가된 맴버 정보를 등록				
				CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
				RwInt32 iLastMemberIndex = pParty->GetMemberCount();
				sPartyMember* pPartyMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey( pPacket->hSerialId ) );
				NTL_ASSERT(pPartyMember, "CCommunityParty::HandleEvents, Not exist party member of handle : " << pPacket->hSerialId);

				SetMember(pPartyMember, iLastMemberIndex - 1);
				break;	
			}
		case PMT_PARTY_MEMBER_DEL:
			{				
				CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();

				for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
				{		
					if( m_PartyMemberInfo[i].hSerial == pPacket->hSerialId )
					{
						DelMember(i);

						for( RwUInt8 k = i ; k < NTL_MAX_MEMBER_IN_PARTY ; ++k )
						{
							sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyIndex(k) );

							if( pMember )
								SetMember(pMember, k);								
							else
								DelMember(k);
						}

						break;
					}					
				}

				break;
			}
		case PMT_PARTY_LEADER_CHANGE:
			{				
				CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
				SERIAL_HANDLE hLeaderSerial = pParty->GetLeaderHandle();
				COMMUNITY_ITER it = pParty->Begin();

				for( RwUInt8 i = 0 ; it != pParty->End() ; ++i, ++it)
				{		
					sPartyMember* pMember = reinterpret_cast<sPartyMember*>( *it );

					// 파티 리더라면 마크 표시
					if( hLeaderSerial == pMember->hSerial )
					{
						LocateLeaderMark(i+1);
						m_PartyMemberInfo[i].pName->SetTextColor(dCOLOR_PARTY_LEADER, true);
					}
					else
					{
						m_PartyMemberInfo[i].pName->SetTextColor(dCOLOR_PARTY_MEMBER, true);
					}
				}

				break;
			}
		case PMT_ZENNY_DIVISION:
			{
				m_bySelectedZennyDivisionType = pPacket->uiValue;
				SetZennyDivision(pPacket->uiValue);
				break;
			}
		case PMT_ZENNY_LOCK:
			{
				LockZennyButton(false);				
				break;
			}
		case PMT_ZENNY_UNLOCK:
			{
				LockZennyButton(true);
				break;
			}
		case PMT_ITEM_DIVISION:
			{
				m_bySelectedItemDivisionType = pPacket->uiValue;
				SetItemDivision(pPacket->uiValue);
				break;
			}
		case PMT_ITEM_LOCK:
			{
				LockItemButton(false);

				break;
			}		
		case PMT_ITEM_UNLOCK:
			{
				LockItemButton(true);
				break;
			}
		case PMT_SCORE:
			{				
				CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
				COMMUNITY_ITER it = pParty->Begin();

				for( RwUInt8 byCount = 0 ; byCount < pParty->GetMemberCount() ; ++byCount, ++it )
				{		
					sPartyMember* pMember = reinterpret_cast<sPartyMember*>( *it );

					if(!pMember)
						break;

					if( pPacket->hSerialId == pMember->hSerial )
					{
						m_PartyMemberInfo[byCount].pScore->SetText(pPacket->uiValue);
					}
				}

				break;
			}
		}
	}
	else if(msg.Id == g_EventCharm)
	{
		SDboEventPartyCharm* pPacket = reinterpret_cast<SDboEventPartyCharm*>(msg.pData);

		switch(pPacket->iType)
		{
		case PCET_DISPLAY_CHARM_BUFF:
			{
				CRectangle rectParent = m_pParent->GetPosition();
				m_PartyCharmSlot[pPacket->iValue].SetParentPosition(rectParent.left, rectParent.top);
				m_PartyCharmSlot[pPacket->iValue].SetIcon(pPacket->uiValue2);

				break;
			}
		case PCET_UNDISPLAY_CHARM_BUFF:
			{
				m_PartyCharmSlot[pPacket->iValue].Clear();
				CheckInfoWindow();
				break;
			}
		}	
	}
	else if(msg.Id == g_EventCharmPointNotify)
	{
		SNtlEventCharmPoint* pEvent = reinterpret_cast<SNtlEventCharmPoint*>( msg.pData );
		m_CPPoint->SetText(pEvent->uiCharmPoint);
	}
	else if(msg.Id == g_EventCalcPopupResult)
	{
		SDboEventCalcPopupResult* pEvent = reinterpret_cast<SDboEventCalcPopupResult*>( msg.pData );

		if( pEvent->nSrcPlace == PLACE_SUB_PARTY_CP )
		{
			CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
			if( pParty->IsOpenCharm() )
				GetDboGlobal()->GetGamePacketGenerator()->SendPartyDeclareCharmPointReq(pEvent->uiValue);
		}
	}
	else if(msg.Id == g_EventGameServerChangeOut)
	{
		PartyLeave();

		CCommunityGui* pGui = reinterpret_cast<CCommunityGui*>( GetDialogManager()->GetDialog(DIALOG_COMMUNITY) );
		if( pGui->GetPageType() == CCommunityGui::PAGETYPE_PARTY )
		{
			m_pPartNameInput->Show(true);
			m_pMyPartyNameStatic->Show(false);
		}
		else
		{
			m_pMyPartyNameStatic->Show(false);
			m_pPartNameInput->Show(false);
		}

		CheckInfoWindow();
	}

	NTL_RETURNVOID();
}