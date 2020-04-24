#include "precomp_dboclient.h"
#include "CommunityGuild.h"

// Share
#include "ItemTable.h"
#include "NtlDebug.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// Presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLApi.h"
#include "NtlSLGuild.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"

// Dbo
#include "InfoWndManager.h"
#include "IconMoveManager.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "ChatGui.h"
#include "CommunityGui.h"
#include "DisplayStringManager.h"
#include "GuildEmblemMakerGui.h"
#include "GuildNoticeGui.h"


#define dMAGIN								3	
#define dLIST_GAP							23
#define dLIST_START_Y						152
#define dLIST_MARK_X						18
#define dMARK_SIZE							16
#define dTRIBUTARYSIMBOL_ADJUST_X			2
#define dTRIBUTARYSIMBOL_ADJUST_Y			0




//////////////////////////////////////////////////////////////////////////
//	CCommunityGuild
//////////////////////////////////////////////////////////////////////////

CGuildMemberGui::CGuildMemberGui(CNtlPLGui* pParent)
:m_charID(INVALID_CHARACTERID)
{
	CRectangle rect;

	m_pParent = pParent;

	// 이름
	rect.SetRectWH(dMAGIN, dMAGIN, 109, 13);
	m_pName = NTL_NEW gui::CStaticBox( rect, pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pName->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pName->Show(false);
	m_pName->Enable(false);

	// 레벨
	rect.SetRectWH(dMAGIN + 117, dMAGIN, 43, 13);
	m_pLevel = NTL_NEW gui::CStaticBox( rect, pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pLevel->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pLevel->Show(false);
	m_pLevel->Enable(false);

	// 지역
	rect.SetRectWH(dMAGIN + 176, dMAGIN, 73, 13);
	m_pArea = NTL_NEW gui::CStaticBox( rect, pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pArea->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pArea->Show(false);
	m_pArea->Enable(false);

	// 맴버 메뉴 버튼
	rect.SetRectWH(273, 151, 20, 20);
	m_pMemberMenuButton = (gui::CButton*) NTL_NEW gui::CButton(rect, "",
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "gui\\Community.srf", "srfDownArrowBtnUp" ),
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "gui\\Community.srf", "srfDownArrowBtnDown" ),
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "gui\\Community.srf", "srfDownArrowBtnUp" ),
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "gui\\Community.srf", "srfDownArrowBtnFoc" ),
		NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
		GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pMemberMenuButton->Show(false);
	m_slotMemberMenuButton = m_pMemberMenuButton->SigClicked().Connect(this, &CGuildMemberGui::ClickedMemberButton);

	// 클래스 서페이스
	rect.SetRectWH(0, 0, dMARK_SIZE, dMARK_SIZE);
	pTributarySimbol = NTL_NEW gui::CPanel( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager() );
}

CGuildMemberGui::~CGuildMemberGui()
{
	Destory();
}

VOID CGuildMemberGui::Destory()
{
	NTL_DELETE( m_pName );
	NTL_DELETE( m_pLevel );
	NTL_DELETE( m_pArea );
	NTL_DELETE( m_pMemberMenuButton );
}

VOID CGuildMemberGui::Clear()
{
	std::list<gui::CSurface>* pSurfaceList = pTributarySimbol->GetSurface();
	CRectangle rtRect = pTributarySimbol->GetPosition();

	pSurfaceList->clear();
	pTributarySimbol->SetPosition(rtRect.left, rtRect.top);
	pTributarySimbol->DeleteToolTip();
	
	m_charID				= INVALID_CHARACTERID;
	m_pName	->Clear();
	m_pLevel->Clear();
	m_pArea	->Clear();

	m_pMemberMenuButton->Show(false);
}

VOID CGuildMemberGui::SetPosition(RwInt32 iParentiX, RwInt32 iParentiY, RwInt32 iX, RwInt32 iY)
{	
	RwInt32 iWidth = 0;
	CRectangle rtScreen = m_pParent->GetPosition();

	m_rtPos.SetRectWH(iX, iY, 275, 19);

	pTributarySimbol->SetPosition(iX + dTRIBUTARYSIMBOL_ADJUST_X, iY + dTRIBUTARYSIMBOL_ADJUST_Y);
	iWidth += pTributarySimbol->GetWidth();

	m_pName->SetPosition(iX + iWidth + dMAGIN + 5, iY + dMAGIN);
	iWidth += m_pName->GetWidth();

	m_pLevel->SetPosition(iX + 117, iY + dMAGIN);
	iWidth += m_pLevel->GetWidth();

	m_pArea->SetPosition(iX + 176, iY + dMAGIN);
	iWidth += m_pArea->GetWidth();

	m_pMemberMenuButton->SetPosition(273, iY + 1);
}

VOID CGuildMemberGui::GetPosition(CRectangle& rect)
{	
	rect = m_rtPos;
}

VOID CGuildMemberGui::SetMember(sGuildMember* pGuildMember)
{
	if( !pGuildMember )
	{
		DBO_FAIL("Invalid pointer");
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlGuild* pGuild = pAvatar->GetGuild();
	CRectangle rect;

	rect = pTributarySimbol->GetPosition();
	std::list<gui::CSurface>* pSurfaceList = pTributarySimbol->GetSurface();
	pSurfaceList->clear();

	const WCHAR* pwcClassName = Logic_GetClassName(pGuildMember->byClass);

	if( pGuild->IsGuildMaster(pGuildMember->charID) )
	{
		std::wstring wstrTooltip = GetDisplayStringManager()->GetString("DST_GUILD_MASTER");
		wstrTooltip.append(L" / ");
		wstrTooltip.append(pwcClassName);

		pTributarySimbol->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGuildMasterMark" ) );
		pTributarySimbol->SetToolTip( wstrTooltip );
	}
	else if( pGuild->IsSecondGuildMaster(pGuildMember->charID) )
	{
		std::wstring wstrTooltip = GetDisplayStringManager()->GetString("DST_GUILD_SECOND_MASTER");
		wstrTooltip.append(L" / ");
		wstrTooltip.append(pwcClassName);

		pTributarySimbol->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSecondGuildMasterMark" ) );
		pTributarySimbol->SetToolTip( wstrTooltip );
	}
	else
	{
		pTributarySimbol->AddSurface( Logic_GetPCClassIconSurface(pGuildMember->byClass, false) );
		pTributarySimbol->SetToolTip( pwcClassName );
	}
	
	pTributarySimbol->SetPosition(rect.left, rect.top);

		
	if( pGuild->IsGuildMaster(pGuildMember->charID) )
	{
		SetMasterColor();
	}
	else if( pGuild->IsSecondGuildMaster(pGuildMember->charID) )
	{
		SetSecondMasterColor();
	}
	else
	{
		SetNormalColor();
	}


	if( !pGuildMember->bOnline )
		SetOffLineColor();


	m_charID = pGuildMember->charID;
	m_pName->SetText(pGuildMember->wszMemberName);
	m_pLevel->SetText(pGuildMember->byLevel);
	m_pArea->SetText(pGuildMember->wstrArea.c_str());
}

VOID CGuildMemberGui::SetMasterColor()
{
	m_pName	->SetTextColor( RGB(247, 148, 28), true );
	m_pLevel->SetTextColor( RGB(247, 148, 28), true );
	m_pArea	->SetTextColor( RGB(247, 148, 28), true );
}

VOID CGuildMemberGui::SetSecondMasterColor()
{
	m_pName	->SetTextColor( RGB(81, 223, 240), true );
	m_pLevel->SetTextColor( RGB(81, 223, 240), true );
	m_pArea	->SetTextColor( RGB(81, 223, 240), true );
}

VOID CGuildMemberGui::SetNormalColor()
{
	m_pName	->SetTextColor( RGB(255, 255, 255), true );
	m_pLevel->SetTextColor( RGB(255, 255, 255), true );
	m_pArea	->SetTextColor( RGB(255, 255, 255), true );
}

VOID CGuildMemberGui::SetOffLineColor()
{
	m_pName	->SetTextColor( RGB(149, 149, 149), true );
	m_pLevel->SetTextColor( RGB(149, 149, 149), true );
	m_pArea	->SetTextColor( RGB(149, 149, 149), true );
}

CHARACTERID  CGuildMemberGui::GetCharID()
{
	return m_charID;
}

VOID CGuildMemberGui::Show(bool bShow)
{
	pTributarySimbol->Show(bShow);
	m_pName			->Show(bShow);
	m_pLevel		->Show(bShow);
	m_pArea			->Show(bShow);

	if( m_charID == INVALID_CHARACTERID )
		m_pMemberMenuButton->Show(false);
	else
		m_pMemberMenuButton->Show(bShow);
}

RwBool CGuildMemberGui::PtInRect(RwInt32 iXPos, RwInt32 iYPos)
{
	return m_rtPos.PtInRect(iXPos, iYPos);
}

VOID CGuildMemberGui::ClickedMemberButton(gui::CComponent* pComponent)
{
	// 맴버 버튼 On/Off
	CRectangle rect = m_pParent->GetPosition();
	CDboEventGenerator::IconPopupShow( TRUE, m_charID, PLACE_SUB_GUILD_DIALOG, 0,
										rect.left + m_pMemberMenuButton->GetPosition().left,
										rect.top + m_rtPos.top );
}

//////////////////////////////////////////////////////////////////////////
//	CCommunityGuild
//////////////////////////////////////////////////////////////////////////


CCommunityGuild::CCommunityGuild()
:m_bNameAscendingSort(FALSE)
,m_bLevelAscendingSort(FALSE)
,m_bAreaAscendingSort(FALSE)
,m_iOnlineMember(0)
,m_SelectedCharID(INVALID_SERIAL_ID)
,m_pEmblemMakerGui(NULL)
,m_pNoticeGui(NULL)
,m_byInfoMemberIndex(INVALID_BYTE)
,m_pEmblemButtonImage(NULL)
{
}

CCommunityGuild::~CCommunityGuild()
{
	Destroy();
}

RwBool CCommunityGuild::Create(CNtlPLGui* pParent)
{
	NTL_FUNCTION( "CCommunityGuild::Create" );

	m_pParent = pParent;

	CRectangle rect;
	CRectangle rtParent = m_pParent->GetPosition();

	// 길드 문장 버튼
	m_pEmblemButton = (gui::CButton*)m_pParent->GetComponent("GuildEmblemButton");
	m_pEmblemButton->SetToolTip( GetDisplayStringManager()->GetString("DST_GUILD_EMBLEM_INFO") );
	m_slotEmblemButton = m_pEmblemButton->SigClicked().Connect(this, &CCommunityGuild::ClickedEmblemButton);	

	// 길드 문장 수정 버튼
	m_pEmblemModifyButton = (gui::CButton*)m_pParent->GetComponent("GuildEmblemModifyButton");
	m_pEmblemModifyButton->SetToolTip( GetDisplayStringManager()->GetString("DST_GUILD_EMBLEM_MODIFY_INFO") );
	m_slotEmblemModifyButton = m_pEmblemModifyButton->SigClicked().Connect(this, &CCommunityGuild::ClickedEmblemButton);	

	// 길드 이름
	rect.SetRectWH(78, 71, 130, 13);
	m_pGuildName = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pGuildName->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	
	m_pGuildName->SetTextColor(RGB(255, 210, 0));
	m_pGuildName->Clear();
	m_pGuildName->Enable(false);
	m_pGuildName->Show(false);

	// max guild points
	rect.SetRectWH(78, 93, 130, 13);
	m_pMaxGuildPointText = NTL_NEW gui::CStaticBox(rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pMaxGuildPointText->CreateFontStd("detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pMaxGuildPointText->SetTextColor(RGB(255, 210, 0));
	m_pMaxGuildPointText->Clear();
	m_pMaxGuildPointText->Enable(false);
	m_pMaxGuildPointText->Show(false);
	m_pMaxGuildPointText->SetText(GetDisplayStringManager()->GetString("DST_GUILD_MAX_POINT"));

	rect.SetRectWH(215, 93, 240, 13);
	m_pMaxGuildPoint = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pMaxGuildPoint->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pMaxGuildPoint->SetTextColor(RGB(255, 210, 0));
	m_pMaxGuildPoint->Clear();
	m_pMaxGuildPoint->Enable(false);
	m_pMaxGuildPoint->Show(false);

	// cur guild points
	rect.SetRectWH(78, 108, 130, 13);
	m_pCurGuildPointText = NTL_NEW gui::CStaticBox(rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pCurGuildPointText->CreateFontStd("detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pCurGuildPointText->SetTextColor(RGB(255, 210, 0));
	m_pCurGuildPointText->Clear();
	m_pCurGuildPointText->Enable(false);
	m_pCurGuildPointText->Show(false);
	m_pCurGuildPointText->SetText(GetDisplayStringManager()->GetString("DST_GUILD_CURRNET_POINT"));

	rect.SetRectWH(215, 108, 240, 13);
	m_pCurGuildPoint = NTL_NEW gui::CStaticBox(rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pCurGuildPoint->CreateFontStd("detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pCurGuildPoint->SetTextColor(RGB(255, 210, 0));
	m_pCurGuildPoint->Clear();
	m_pCurGuildPoint->Enable(false);
	m_pCurGuildPoint->Show(false);

	// 길드 탈퇴 버튼
	m_pLeaveButton = (gui::CButton*)m_pParent->GetComponent("GuildLeaveButton");
	m_pLeaveButton->SetToolTip( GetDisplayStringManager()->GetString("DST_GUILD_LEAVE") );
	m_slotLeaveButton = m_pLeaveButton->SigClicked().Connect(this, &CCommunityGuild::ClickedLeaveButton);

	// 길드 도장 버튼
	m_pGhymButton = (gui::CButton*)m_pParent->GetComponent("GuildGhymButton");
	m_pGhymButton->SetToolTip( GetDisplayStringManager()->GetString("DST_GUILD_GYM") );
	m_slotGhymButton = m_pGhymButton->SigClicked().Connect(this, &CCommunityGuild::ClickedGhymButton);



	// 이름 정렬 버튼
	m_pNameButton = (gui::CButton*)m_pParent->GetComponent("GuildNameButton");
	m_pNameButton->SetTextStyle(COMP_TEXT_CENTER);
	m_pNameButton->SetTextUpColor(NTL_BUTTON_UP_COLOR);
	m_pNameButton->SetTextDownColor(NTL_BUTTON_DOWN_COLOR);
	m_pNameButton->SetTextFont( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pNameButton->SetText( GetDisplayStringManager()->GetString("DST_GUILD_MEMBER") );
	m_pNameButton->ApplyText();
	m_slotNameButton = m_pNameButton->SigClicked().Connect(this, &CCommunityGuild::ClickedNameButton);

	// 레벨 정렬 버튼
	m_pLevelButton = (gui::CButton*)m_pParent->GetComponent("GuildLevelButton");
	m_pLevelButton->SetTextStyle(COMP_TEXT_CENTER);
	m_pLevelButton->SetTextUpColor(NTL_BUTTON_UP_COLOR);
	m_pLevelButton->SetTextDownColor(NTL_BUTTON_DOWN_COLOR);
	m_pLevelButton->SetTextFont( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pLevelButton->SetText( GetDisplayStringManager()->GetString("DST_GUILD_LEVEL") );	
	m_pLevelButton->ApplyText();
	m_slotLevelButton = m_pLevelButton->SigClicked().Connect(this, &CCommunityGuild::ClickedLevelButton);

	// 지역 정렬 버튼
	m_pAreaButton = (gui::CButton*)m_pParent->GetComponent("GuildAreaButton");
	m_pAreaButton->SetTextStyle(COMP_TEXT_CENTER);
	m_pAreaButton->SetTextUpColor(NTL_BUTTON_UP_COLOR);
	m_pAreaButton->SetTextDownColor(NTL_BUTTON_DOWN_COLOR);
	m_pAreaButton->SetTextFont( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pAreaButton->SetText( GetDisplayStringManager()->GetString("DST_GUILD_CUR_POS") );
	m_pAreaButton->ApplyText();
	m_slotAreaButton = m_pAreaButton->SigClicked().Connect(this, &CCommunityGuild::ClickedAreaButton);

	// 길드 맴버 배경
	m_GuildMemBackSurface.SetSurface(0,  GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "srfMemberPanelLeft" ) );
	m_GuildMemBackSurface.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "srfMemberPanelCenter" ) );
	m_GuildMemBackSurface.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "srfMemberPanelRight" ) );
	m_GuildMemBackSurface.SetSize(275, 180);
	m_GuildMemBackSurface.SetPositionfromParent(17, 152);



	// 사람 모양
	m_srfHumanShapeSurface.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "srfHumanShape" ) );
	m_srfHumanShapeSurface.SetPositionfromParent(280, 336);

	// 길드 인원
	rect.SetRectWH(203, 336, 65, 13);
	m_pGuildNum = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_pGuildNum->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	
	m_pGuildNum->SetTextColor(RGB(186, 186, 186));
	m_pGuildNum->SetText(L"0/0");
	m_pGuildNum->Enable(false);
	m_pGuildNum->Show(false);




	// 길드 공지 바
	m_srfNoticeBar.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "PartyHeadLine" ) );
	m_srfNoticeBar.SetPositionfromParent(10, 346);

	// 길드 공지 배경
	m_NoticeSurface.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "srfNoticePanelLeft" ) );
	m_NoticeSurface.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "srfNoticePanelCenter" ) );
	m_NoticeSurface.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Community.srf", "srfNoticePanelRight" ) );
	m_NoticeSurface.SetSize(290, 90);
	m_NoticeSurface.SetPositionfromParent(16, 365);

	// 길드 공지 버튼
	m_pNoticeButton = (gui::CButton*)m_pParent->GetComponent("GuildNoticeButton");
	m_pNoticeButton->SetToolTip( GetDisplayStringManager()->GetString("DST_GUILD_NOTICE") );
	m_slotNoticeButton = m_pNoticeButton->SigClicked().Connect(this, &CCommunityGuild::ClickedNoticeButton);

	// '길드 공지'
	rect.SetRectWH(31, 348, 123, 13);
	m_pGuildNoticeStatic = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pGuildNoticeStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	
	m_pGuildNoticeStatic->SetText(GetDisplayStringManager()->GetString("DST_GUILD_NOTICE"));
	m_pGuildNoticeStatic->Enable(false);
	m_pGuildNoticeStatic->Show(false);

	// 공지 날짜
	rect.SetRectWH(31, 371, 140, 13);
	m_pGuildNoticeTime = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pGuildNoticeTime->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	
	m_pGuildNoticeTime->SetTextColor(RGB(123, 201, 223));
	m_pGuildNoticeTime->Clear();
	m_pGuildNoticeTime->Enable(false);
	m_pGuildNoticeTime->Show(false);

	// 공지자
	rect.SetRectWH(164, 371, 101, 13);
	m_pGuildNoticer = NTL_NEW gui::CStaticBox( rect, m_pParent->GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pGuildNoticer->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	
	m_pGuildNoticer->SetTextColor(RGB(123, 201, 223));
	m_pGuildNoticer->Clear();
	m_pGuildNoticer->Enable(false);
	m_pGuildNoticer->Show(false);

	// 공지 내용
	m_pNotice = (gui::COutputBox*)m_pParent->GetComponent("NoticeContent");
	m_pNotice->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	
	m_pNotice->SetLineSpace(3);
	m_pNotice->SetTextColor(RGB(255, 210, 0));
	m_pNotice->Show(false);
	
	// 스크롤
	m_pScrollbar = (gui::CScrollBar*)m_pParent->GetComponent("scbScroll");
	m_pScrollbar->SetMaxValue(0);

	for(RwUInt8 i = 0 ; i < dVISIBLE_LIST_COUNT ; ++i)
	{
		m_aGuildMemberGui[i] = NTL_NEW CGuildMemberGui(pParent);
		m_aGuildMemberGui[i]->Show(false);
	}

	// 엠블렘 버튼 이미지
	CRectangle rtEmblemModifyButton = m_pEmblemButton->GetPosition();
	m_pEmblemButtonImage = NTL_NEW CRwTextureGui;
	m_pEmblemButtonImage->SetPositionfromParent(rtEmblemModifyButton.left, rtEmblemModifyButton.top);
	m_pEmblemButtonImage->SetSize(rtEmblemModifyButton.GetWidth(), rtEmblemModifyButton.GetHeight());


	// sig
	m_slotServerScrollChanged		= m_pScrollbar->SigValueChanged().Connect( this, &CCommunityGuild::OnScrollChanged );
	m_slotServerScrollSliderMoved	= m_pScrollbar->SigSliderMoved().Connect( this, &CCommunityGuild::OnScrollChanged );
	m_slotCaptureWheelMove			= GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CCommunityGuild::OnCaptureWheelMove );
	m_slotCaptureMouseDown			= GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CCommunityGuild::OnCaptureMouseDown );

	NTL_RETURN(TRUE);
}

VOID CCommunityGuild::Destroy()
{
	NTL_FUNCTION("CCommunityGuild::Destroy");

	Clear();	
	ClearEmblemMaker();
	ClearNoticeGui();
	EmblemButton_Reset();
	NTL_DELETE(m_pEmblemButtonImage);

	for(RwUInt8 i = 0 ; i < dVISIBLE_LIST_COUNT ; ++i)
	{
		if(m_aGuildMemberGui[i])
		{
			m_aGuildMemberGui[i]->Destory();
			NTL_DELETE(m_aGuildMemberGui[i]);
		}
	}

	NTL_RETURNVOID();
}

VOID CCommunityGuild::Clear()
{
	m_pGuildName		->Clear();
	m_pCurGuildPointText->Clear();
	m_pMaxGuildPointText->Clear();
	m_pCurGuildPoint->Clear();
	m_pMaxGuildPoint->Clear();
	m_pGuildNum			->SetText(L"0/0");
	m_pGuildNoticer		->Clear();
	m_pNotice			->Clear();

	m_bNameAscendingSort	= FALSE;
	m_bLevelAscendingSort	= FALSE;
	m_bAreaAscendingSort	= FALSE;

	m_iOnlineMember = 0;

	for(RwUInt8 i = 0 ; i < dVISIBLE_LIST_COUNT ; ++i)
	{
		if(m_aGuildMemberGui[i])
			m_aGuildMemberGui[i]->Clear();
	}

	m_pScrollbar->SetMaxValue(0);
}

VOID CCommunityGuild::GuildLeave()
{
	Clear();
	ClearEmblemMaker();
	ClearNoticeGui();
	EmblemButton_Reset();

	CCommunityGui* pCommunityGui = reinterpret_cast<CCommunityGui*>( GetDialogManager()->GetDialog(DIALOG_COMMUNITY) );
	if( pCommunityGui->GetPageType() == CCommunityGui::PAGETYPE_GUILD )
	{
		m_pEmblemButton->Show(true);
		m_pEmblemModifyButton->Show(false);
	}
}

VOID CCommunityGuild::ClearEmblemMaker()
{
	if( m_pEmblemMakerGui )
	{
		m_pNoticeButton->ClickEnable(true);

		m_pParent->UnLink( m_pEmblemMakerGui->GetDialog() );
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pEmblemMakerGui);

		m_pEmblemMakerGui->Destroy();
		NTL_DELETE(m_pEmblemMakerGui);
	}
}

VOID CCommunityGuild::ClearNoticeGui()
{
	if( m_pNoticeGui )
	{
		m_pEmblemButton->ClickEnable(true);
		m_pEmblemModifyButton->ClickEnable(true);

		m_pParent->UnLink( m_pNoticeGui->GetDialog() );
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pNoticeGui);

		m_pNoticeGui->Destroy();
		NTL_DELETE(m_pNoticeGui);
	}
}

VOID CCommunityGuild::ActiveTab(bool bActive)
{
	if(!bActive)
		CheckInfoWindow();

	if( bActive )
	{
		CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();

		if( pGuild->IsHaveEmblem() )
		{
			m_pEmblemButton->Show(false);
			m_pEmblemModifyButton->Show(true);
		}
		else
		{
			m_pEmblemButton->Show(true);
			m_pEmblemModifyButton->Show(false);
		}
	}
	else
	{
		m_pEmblemButton->Show(false);
		m_pEmblemModifyButton->Show(false);
	}
	
	m_pLeaveButton				->Show(bActive);
	m_pGhymButton				->Show(bActive);

	m_pNameButton				->Show(bActive);
	m_pLevelButton				->Show(bActive);
	m_pAreaButton				->Show(bActive);
	m_pNoticeButton				->Show(bActive);
	
	m_pGuildName				->Show(bActive);
	m_pCurGuildPointText->Show(bActive);
	m_pMaxGuildPointText->Show(bActive);
	m_pCurGuildPoint->Show(bActive);
	m_pMaxGuildPoint->Show(bActive);
	m_pGuildNum					->Show(bActive);
	m_pGuildNoticeStatic		->Show(bActive);
	m_pGuildNoticeTime			->Show(bActive);
	m_pGuildNoticer				->Show(bActive);
	m_pNotice					->Show(bActive);

	m_pScrollbar				->Show(bActive);

	for(RwInt8 i = 0 ; i < dVISIBLE_LIST_COUNT ; ++i )
	{
		m_aGuildMemberGui[i]->Show(bActive);
	}

	if( m_pEmblemMakerGui )
		m_pEmblemMakerGui->Active(bActive);

	if( m_pNoticeGui )
		m_pNoticeGui->Active(bActive);
}

VOID CCommunityGuild::AddMember(CHARACTERID charID)
{
	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();

	// 스크롤 최대값 조정
	m_pScrollbar->SetMaxValue( pGuild->GetMemberCount() - dVISIBLE_LIST_COUNT );

	RefreshList(m_iVisibleStart);

	SetOnlineMemberCount(pGuild);
}

VOID CCommunityGuild::DelMember(CHARACTERID charID)
{
	RwInt32 iDelIndex = -1;
	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();

	for(RwInt8 i = 0 ; i < dVISIBLE_LIST_COUNT ; ++i)
	{
		if( m_aGuildMemberGui[i]->GetCharID() == charID )
		{
			iDelIndex = i;
			break;
		}
	}

	// 스크롤 최대값 조정
	m_pScrollbar->SetMaxValue( pGuild->GetMemberCount() - dVISIBLE_LIST_COUNT );

	if( iDelIndex < m_pScrollbar->GetValue() + dVISIBLE_LIST_COUNT )
		RefreshList(m_iVisibleStart);

	SetOnlineMemberCount(pGuild);
}

VOID CCommunityGuild::SetOnlineMemberCount(CNtlGuild* pGuild)
{
	if( !pGuild )
	{
		DBO_FAIL("Invalid pointer");
		return;
	}

	m_pGuildNum->Format(L"%d/%d", pGuild->GetOnlineMemberCount(), pGuild->GetMemberCount());
}

VOID CCommunityGuild::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_COMMUNITY )
	{
		CCommunityGui* pGui = reinterpret_cast<CCommunityGui*>( GetDialogManager()->GetDialog(DIALOG_COMMUNITY) );
		if( pGui->GetPageType() == CCommunityGui::PAGETYPE_GUILD )
		{
			if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_JUST_WTEXT )
				GetInfoWndManager()->ShowInfoWindow( FALSE );
		}
	}

    m_byInfoMemberIndex = INVALID_BYTE;
}

VOID CCommunityGuild::OnScrollChanged(RwInt32 iOffset)
{
	RefreshList(iOffset);
}

VOID CCommunityGuild::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
{
	if( !m_pParent->IsShow() )
		return;

	if( m_pParent->GetDialog()->GetParent()->GetChildComponentReverseAt( pos.x, pos.y ) != m_pParent->GetDialog() )
		return;

	if( m_pParent->GetDialog()->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;

	if( m_GuildMemBackSurface.PtInRect(pos.x, pos.y) )
	{
		RwInt32 iValue = m_pScrollbar->GetValue();
		RwInt32 iMaxValue = m_pScrollbar->GetMaxValue();

		if( sDelta < 0 )
			++iValue;
		else
			--iValue;

		if( iValue < 0 )
			iValue = 0;
		else if( iValue > iMaxValue )
			iValue = iMaxValue;

		m_pScrollbar->SetValue( iValue );
	}
	else if( m_NoticeSurface.PtInRect(pos.x, pos.y) )
	{
		if( sDelta < 0 )
			m_pNotice->NextLine();
		else
			m_pNotice->PrevLine();
	}
}

VOID CCommunityGuild::OnCaptureMouseDown(const CKey& key)
{
	if( m_pParent->GetDialog()->GetParent()->GetChildComponentReverseAt((RwInt32)key.m_fX, (RwInt32)key.m_fY)
		!= m_pParent->GetDialog() )
		return;
	
	CRectangle rect = m_pParent->GetDialog()->GetScreenRect();
	if( rect.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY))
	{		
		GetDialogManager()->RaiseDialogbyUser(DIALOG_COMMUNITY);
		m_pParent->RaiseLinked();
	}
}

VOID CCommunityGuild::ClickedEmblemButton(gui::CComponent* pComponent)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlGuild* pGuild = pAvatar->GetGuild();

	if( pGuild->IsHaveGroup() == FALSE )
	{
		GetAlarmManager()->AlarmMessage("DST_GUILD_NOT_JOIN");
		return;
	}

	if( !Logic_I_am_GuildLeader() )
	{
		GetAlarmManager()->AlarmMessage("COMMUNITY_GUILD_FUNCTION_NOT_FOUND", TRUE);
		return;
	}

	if( pGuild->IsHaveEmblem() )
	{
		if( pGuild->IsExistFunction(DBO_GUILD_FUNCTION_CAN_CHANGE_EMBLEM) == FALSE )
		{
			GetAlarmManager()->AlarmMessage("COMMUNITY_GUILD_FUNCTION_NOT_FOUND", TRUE);
			return;
		}
	}
	else if( pGuild->IsExistFunction(DBO_GUILD_FUNCTION_CAN_SET_EMBLEM) == FALSE )
	{
		GetAlarmManager()->AlarmMessage("COMMUNITY_GUILD_FUNCTION_NOT_FOUND", TRUE);
		return;
	}

	// 엠블렘 메이커 GUI
	if( !m_pEmblemMakerGui )
	{
		CRectangle rtScreen = m_pParent->GetPosition();

		m_pEmblemMakerGui = NTL_NEW CGuildEmblemMakerGui("GuildEmblemMakerGui");
		m_pEmblemMakerGui->Create();
		m_pEmblemMakerGui->SetPosition(rtScreen.left + (rtScreen.GetWidth() - m_pEmblemMakerGui->GetWidth())/2,
			rtScreen.top + 128 );

		m_pParent->Link( m_pEmblemMakerGui->GetDialog() );
		CNtlPLGuiManager::GetInstance()->AddGui(m_pEmblemMakerGui);

		m_pNoticeButton->ClickEnable(false);
	}
}

VOID CCommunityGuild::ClickedLeaveButton(gui::CComponent* pComponent)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlGuild* pGuild = pAvatar->GetGuild();

	if( pGuild->IsHaveGroup() )
	{
		// 길드를 탈퇴한다
		//const WCHAR* pwcText = GetDisplayStringManager()->GetString(DST_GUILD_LEAVE_ASK);
		//CDboEventGenerator::MsgBoxShow(pwcText, MBW_GUILD_LEAVE, MBTF_OK | MBTF_CANCEL);	
		GetAlarmManager()->AlarmMessage( "DST_GUILD_LEAVE_ASK" );
	}
	else
	{
		// 길드에 가입하지 않았습니다
		GetAlarmManager()->AlarmMessage("DST_GUILD_NOT_JOIN");
	}
}

VOID CCommunityGuild::ClickedGhymButton(gui::CComponent* pComponent)
{
	// 길드 도장 GUI를 연다
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlGuild* pGuild = pAvatar->GetGuild();

	if( pGuild->IsHaveGroup() == FALSE )
	{
		GetAlarmManager()->AlarmMessage("DST_GUILD_NOT_JOIN");
		return;
	}
}

VOID CCommunityGuild::ClickedNameButton(gui::CComponent* pComponent)
{
	// 길드 리스트를 이름별로 정렬한다
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlGuild* pGuild = pAvatar->GetGuild();

	m_bNameAscendingSort = !m_bNameAscendingSort;	
	pGuild->Sort(COMMUNITY_DATA_NAME, m_bNameAscendingSort);

	RefreshList(0);
}
VOID CCommunityGuild::ClickedLevelButton(gui::CComponent* pComponent)
{
	// 길드 리스트를 레벨별로 정렬한다
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlGuild* pGuild = pAvatar->GetGuild();

	m_bLevelAscendingSort = !m_bLevelAscendingSort;	
	pGuild->Sort(COMMUNITY_GUILD_LEVEL, m_bLevelAscendingSort);

	RefreshList(0);
}
VOID CCommunityGuild::ClickedAreaButton(gui::CComponent* pComponent)
{
	// 길드 리스트를 지역별로 정렬한다
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlGuild* pGuild = pAvatar->GetGuild();

	m_bAreaAscendingSort = !m_bAreaAscendingSort;	
	pGuild->Sort(COMMUNITY_GUILD_AREA, m_bAreaAscendingSort);

	RefreshList(0);
}

VOID CCommunityGuild::RefreshList(RwInt32 iOffSet)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	CCommunityGui*	pGui	= reinterpret_cast<CCommunityGui*>( GetDialogManager()->GetDialog(DIALOG_COMMUNITY) );
	bool bShow = FALSE;

	if( pGui->GetPageType() == CCommunityGui::PAGETYPE_GUILD )	
		bShow = TRUE;

	RwUInt32 uiCount = 0;
	RwUInt8 byMemberGuiIndex = 0;
	COMMUNITY_ITER it = pGuild->Begin();
	COMMUNITY_ITER it_end = pGuild->End();

	m_iVisibleStart = iOffSet;

	for( ; it != it_end ; ++it, ++uiCount )
	{
		if( (RwUInt8)m_iVisibleStart <= uiCount && uiCount < (RwUInt32)m_iVisibleStart + dVISIBLE_LIST_COUNT )
		{
			m_aGuildMemberGui[byMemberGuiIndex]->SetMember( reinterpret_cast<sGuildMember*>(*it) );
			m_aGuildMemberGui[byMemberGuiIndex]->Show(bShow);

			++byMemberGuiIndex;
			continue;
		}
	}

	for(RwUInt8 i = byMemberGuiIndex ; i < dVISIBLE_LIST_COUNT ; ++i)
	{
		m_aGuildMemberGui[i]->Clear();
		m_aGuildMemberGui[i]->Show(false);
	}
}

VOID CCommunityGuild::EmblemButton_Change()
{
	EmblemButton_Reset();

	sEmblemFactor emblemFactor;
	if( FALSE == Logic_GetEmblemFactor(&emblemFactor, GetNtlSLGlobal()->GetSobAvatar()) )
		return;
	
	// 길드 문장 버튼의 아래에 깔릴 문장 이미지
	if( !m_pEmblemButtonImage )
		return;

	m_pEmblemButtonImage->SetTexture( GetEmblemMaker()->CreateEmblem(&emblemFactor) );

	CCommunityGui* pCommunityGui = reinterpret_cast<CCommunityGui*>( GetDialogManager()->GetDialog(DIALOG_COMMUNITY) );
	if( pCommunityGui->GetPageType() == CCommunityGui::PAGETYPE_GUILD )
		ActiveTab(true);
}

VOID CCommunityGuild::EmblemButton_Reset()
{
	if( !m_pEmblemButtonImage )
		return;

	if( m_pEmblemButtonImage->GetTexture() )
	{
		GetEmblemMaker()->DestoyEmblem( m_pEmblemButtonImage->GetTexture() );
		m_pEmblemButtonImage->UnsetTexture();
	}
}

SERIAL_HANDLE CCommunityGuild::GetSeletedID()
{
	return m_SelectedCharID;
}

VOID CCommunityGuild::ClickedNoticeButton(gui::CComponent* pComponent)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlGuild* pGuild = pAvatar->GetGuild();

	if( pGuild->IsHaveGroup() == FALSE )
	{
		GetAlarmManager()->AlarmMessage("DST_GUILD_NOT_JOIN");
		return;
	}

	if( !Logic_I_am_GuildLeader() )
	{
		GetAlarmManager()->AlarmMessage("COMMUNITY_GUILD_FUNCTION_NOT_FOUND", TRUE);
		return;
	}

	// 공지를 띄운다
	if( !m_pNoticeGui )
	{
		CRectangle rtScreen = m_pParent->GetPosition();

		m_pNoticeGui = NTL_NEW CGuildNoticeGui("GuildNoticeGui");
		m_pNoticeGui->Create(CGuildNoticeGui::NOTICE_TYPE_GUILD);
		m_pNoticeGui->SetPosition(rtScreen.left + (rtScreen.GetWidth() - m_pNoticeGui->GetWidth())/2,
			rtScreen.top + 128 );

		m_pParent->Link( m_pNoticeGui->GetDialog() );
		CNtlPLGuiManager::GetInstance()->AddGui(m_pNoticeGui);

		m_pNoticeGui->SetInputFocus();
		m_pEmblemButton->ClickEnable(false);
		m_pEmblemModifyButton->ClickEnable(false);
	}
}

VOID CCommunityGuild::OnMouseDown(const CKey& key)
{
}

VOID CCommunityGuild::OnMouseUp(const CKey& key)
{	
}

VOID CCommunityGuild::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pParent->GetPosition();

	m_GuildMemBackSurface		.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfHumanShapeSurface		.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfNoticeBar				.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_NoticeSurface				.SetPositionbyParent(rtScreen.left, rtScreen.top);

	RwInt32 iHeight = dLIST_START_Y;
	for(RwInt8 i = 0  ; i < dVISIBLE_LIST_COUNT ; ++i )
	{
		m_aGuildMemberGui[i]->SetPosition(rtScreen.left, rtScreen.top, dLIST_MARK_X, iHeight);
		iHeight += dLIST_GAP;
	}
	
	if( m_pEmblemButtonImage )
		m_pEmblemButtonImage->SetPositionbyParent(rtScreen.left, rtScreen.top);	

	CheckInfoWindow();
}

VOID CCommunityGuild::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{	
	CRectangle rect = m_pParent->GetPosition();

	for( RwUInt8 i = 0 ; i < dVISIBLE_LIST_COUNT ; ++i )
	{
		if( m_aGuildMemberGui[i]->GetCharID() == INVALID_CHARACTERID )
			continue;

		if( m_aGuildMemberGui[i]->PtInRect(nX, nY) )
		{
			if( GetInfoWndManager()->GetInfoWndState() != CInfoWndManager::INFOWND_JUST_WTEXT &&
				GetInfoWndManager()->GetRequestGui() != DIALOG_COMMUNITY &&
				m_byInfoMemberIndex != i )
			{
				CRectangle rtMember;
				CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();
				sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(m_aGuildMemberGui[i]->GetCharID()) );
				if( pMember )
				{
					if( pMember->wstrArea.empty() )
						break;

					m_aGuildMemberGui[i]->GetPosition(rtMember);

					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,	
										rect.left + 20, rect.top + rtMember.top, (VOID*)pMember->wstrArea.c_str(), DIALOG_COMMUNITY );

					m_byInfoMemberIndex = i;	
				}
				else
				{
					DBO_FAIL("Not exist guild member of charID : " << m_aGuildMemberGui[i]->GetCharID());
				}				
			}

			return;
		}
	}

	GetInfoWndManager()->ShowInfoWindow( FALSE );
	m_byInfoMemberIndex = INVALID_BYTE;
}

VOID CCommunityGuild::OnMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
	m_byInfoMemberIndex = INVALID_BYTE;
}

VOID CCommunityGuild::OnPaint()
{
#define GUI_UV_END		0.99f

	if( m_pEmblemModifyButton->IsVisible() )
	{
		if( m_pEmblemButtonImage )
			m_pEmblemButtonImage->Render();		
	}


	m_GuildMemBackSurface	.Render();	
	m_srfHumanShapeSurface	.Render();
	m_srfNoticeBar			.Render();
	m_NoticeSurface			.Render();
}

VOID CCommunityGuild::SwitchDialog(bool bOpen)
{
	for(RwInt8 i = 0 ; i < dVISIBLE_LIST_COUNT ; ++i )
		m_aGuildMemberGui[i]->Show(bOpen);

	if( bOpen )
	{
		CCommunityGui* pGui = reinterpret_cast<CCommunityGui*>( GetDialogManager()->GetDialog(DIALOG_COMMUNITY) );
		if( pGui->GetPageType() == CCommunityGui::PAGETYPE_GUILD )
		{
			if( m_pEmblemMakerGui )
				m_pEmblemMakerGui->Active(true);

			if( m_pNoticeGui )
				m_pNoticeGui->Active(true);
		}
	}
	else
	{
		CheckInfoWindow();

		if( m_pEmblemMakerGui )
			m_pEmblemMakerGui->Active(false);

		if( m_pNoticeGui )
			m_pNoticeGui->Active(false);
	}
}

VOID CCommunityGuild::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CCommunityGuild::HandleEvents");

	if( msg.Id == g_EventNotifyGuild )
	{
		SNtlEventNotifyGuild* pEvent = reinterpret_cast<SNtlEventNotifyGuild*>( msg.pData );
		switch(pEvent->iMessage)
		{
		case SLGE_GUILD_INFO:
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
				{
					DBO_FAIL("Not exist avatar instance");
					break;
				}

				CNtlGuild* pGuild = pAvatar->GetGuild();

				m_pGuildName->SetText( pGuild->GetGuildName() );
				m_pMaxGuildPoint->SetText(pGuild->GetReputationEver() );
				m_pCurGuildPoint->SetText(pGuild->GetReputation());

				m_pScrollbar->SetMaxValue( pGuild->GetMemberCount() - dVISIBLE_LIST_COUNT );

				RefreshList(0);

				// 접속인원
				SetOnlineMemberCount(pGuild);

				// 해산 공지가 있다면
				if( pGuild->IsBeingDisband() )
				{
					sDetailTime detailTime;

					Logic_timeToDetailTime(pGuild->GetTimeToDisband(), detailTime);

					m_pNotice->Clear();
					m_pNotice->Format((WCHAR*)GetDisplayStringManager()->GetString("DST_GUILD_NOTICE_DISSOLVE"),
										detailTime.uiYear, detailTime.byMonth, detailTime.byDay, detailTime.strDay.c_str(), 
										detailTime.byHour, detailTime.byMinute);
					m_pNotice->FirstLine();
				}

				if( pGuild->IsHaveEmblem() )
					EmblemButton_Change();

				break;
			}      
		case SLGE_ADD_MEMBER:
			{
				AddMember(pEvent->uiParam);
				break;
			}
		case SLGE_DEL_MEMBER:
			{
				DelMember(pEvent->uiParam);
				break;
			}
		case SLGE_APPOINT_MASTER:
		case SLGE_APPOINT_SECOND_MASTER:
		case SLGE_DISMISS_SECOND_MASTER:
		case SLGE_APPOINT_MEMBER:
		case SLGE_MEM_POS:
		case SLGE_MEM_CLASS:
		case SLGE_MEM_LEVEL:
		case SLGE_MEM_NAME:
			{
				RefreshList(m_iVisibleStart);
				break;
			}
		case SLGE_MEM_ONLINE:
		case SLGE_MEM_OFFLINE:
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
				{
					DBO_FAIL("Not exist avatar instance");
					break;
				}

				CNtlGuild* pGuild = pAvatar->GetGuild();

				RefreshList(m_iVisibleStart);
				SetOnlineMemberCount(pGuild);
				break;
			}
		case SLGE_CHANGE_GUILD_NAME:
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
				{
					DBO_FAIL("Not exist avatar instance");
					break;
				}

				CNtlGuild* pGuild = pAvatar->GetGuild();

				m_pGuildName->SetText( pGuild->GetGuildName() );
				break;
			}
		case SLGE_CHANGE_GUILD_EMBLEM:
			{
				EmblemButton_Change();
				break;
			}
		case SLGE_BEING_DISBAND:
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
				{
					DBO_FAIL("Not exist avatar instance");
					break;
				}

				CNtlGuild*	pGuild = pAvatar->GetGuild();
				sDetailTime	detailTime;

				Logic_timeToDetailTime(pGuild->GetTimeToDisband(), detailTime);

				m_pNotice->Clear();
				m_pNotice->Format((WCHAR*)GetDisplayStringManager()->GetString("DST_GUILD_NOTICE_DISSOLVE"),
									detailTime.uiYear, detailTime.byMonth, detailTime.byDay, detailTime.strDay.c_str(), 
									detailTime.byHour, detailTime.byMinute);
				m_pNotice->FirstLine();
				break;
			}
		case SLGE_CANCLE_DISBAND:
			{
				m_pNotice->Clear();
				m_pNotice->Format(L"%s", GetDisplayStringManager()->GetString("DST_GUILD_CANCELED_DISSOLVE"));
				m_pNotice->FirstLine();
				break;
			}
		case SLGE_DISBAND:
			{
				GuildLeave();
				break;
			}
		case SLGE_CHANGE_REPUTATION:
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
				{
					DBO_FAIL("Not exist avatar instance");
					break;
				}

				CNtlGuild* pGuild = pAvatar->GetGuild();

				m_pMaxGuildPoint->SetText(pGuild->GetReputationEver());
				m_pCurGuildPoint->SetText(pGuild->GetReputation());

				break;
			}
		case SLGE_NOTICE:
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
				{
					DBO_FAIL("Not exist avatar instance");
					break;
				}

				CNtlGuild*		pGuild			= pAvatar->GetGuild();
				sGuildNotice*	pGuildNotice	= pGuild->GetNotice();

				m_pGuildNoticer->SetText(pGuildNotice->awcCharName);

				m_pNotice->Clear();
				m_pNotice->Format(L"%s", pGuildNotice->awcNotice);
				m_pNotice->FirstLine();
				break;
			}
		}
	}
	else if( msg.Id == g_EventIconPopupResult )
	{
		SDboEventIconPopupResult* pEvent = reinterpret_cast<SDboEventIconPopupResult*>( msg.pData );

		if( pEvent->nSrcPlace != PLACE_SUB_GUILD_DIALOG )
			NTL_RETURNVOID();

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
		{
			DBO_FAIL("Not exist avatar instance");
			return;
		}

		CNtlGuild* pGuild = pAvatar->GetGuild();

		m_SelectedCharID = pEvent->uiSerial;

		sGuildMember* pMA_Member = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(m_SelectedCharID) );
		if( !pMA_Member )
		{
			DBO_FAIL("g_EventIconPopupResult, Not exist guild member of charID : " << m_SelectedCharID);
			NTL_RETURNVOID();
		}


		if( pEvent->nWorkId == PMW_GUILD_CHANGE_MASTER )
		{
			GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_ENTRUST_MASTER", FALSE, NULL, pMA_Member->wszMemberName);
		}
		else if( pEvent->nWorkId == PMW_GUILD_APPOINT_SECOND_MASTER )
		{
			GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_APPOINT_SECOND_MASTER", FALSE, NULL, pMA_Member->wszMemberName);
		}
		else if( pEvent->nWorkId == PMW_GUILD_DISAPPOINT_SECOND_MASTER )
		{
			GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_DISMISS_SECOND_MASTER_REQ", FALSE, NULL, pMA_Member->wszMemberName);
		}
		else if( pEvent->nWorkId == PMW_GUILD_KICK_OUT )
		{
			GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_KICK_OUT", FALSE, NULL, pMA_Member->wszMemberName);
		}
		else if( pEvent->nWorkId == PMW_GUILD_POST )
		{
			sGuildMember* pMA_Member = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pEvent->uiSerial) );
			if( pMA_Member )
			{
				CDboEventGenerator::OpenSendMail(pMA_Member->wszMemberName);
			}
			else
			{
				DBO_FAIL("Not exist guild member of charID : " << pEvent->uiSerial);
			}			
		}		
	}
	else if( msg.Id == g_EventPostDialog )
	{
		SDboEventDialog* pEvent = (SDboEventDialog*)msg.pData;

		if( pEvent->iType == DIALOGEVENT_CLOSE_EMBLEM_MAKER )
			ClearEmblemMaker();
		else if( pEvent->iType == DIALOGEVENT_CLOSE_GUILD_NOTICE )
			ClearNoticeGui();
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		GuildLeave();
	}

	NTL_RETURNVOID();
}