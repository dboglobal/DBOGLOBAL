#include "precomp_dboclient.h"
#include "CharSelecterGui.h"
/*
// core
#include "NtlDebug.h"
#include "NtlMath.h"
#include "NtlStringUtil.h"

// sound
#include "GUISoundDefine.h"

// share
#include "NtlCharacter.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DisplayStringDef.h"
#include "DisplayStringManager.h"
#include "CharStageState.h"

namespace
{
	#define dDEFAULTWIDTH			1024
	#define dDEFAULTHEIGHT			768

	#define ATTIRIBUTEHAEIGHT		66
};
/*
CCharCreateGuiGui::CCharCreateGuiGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_byRotationState(ER_STOP)
{
}

CCharCreateGuiGui::~CCharCreateGuiGui()
{

}

RwBool CCharCreateGuiGui::Create()
{
	NTL_FUNCTION( "CCharCreateGuiGui::Create" );

	if(!CNtlPLGui::Create("gui\\CharCreate.rsr", "gui\\CharCreate.srf", "gui\\CharCreate.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// Race 배경
	m_RaceWin.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "info_1_Left" ));
	m_RaceWin.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "info_1_Center" ));
	m_RaceWin.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "info_1_Right" ));

	// Race under line
	m_RaceUnderLine.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "underLine" ));
	m_RaceUnderLine.SetPosition(15, 91);

	// Class 배경
	m_ClassWin.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "info_1_Left" ));
	m_ClassWin.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "info_1_Center" ));
	m_ClassWin.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "info_1_Right" ));
	m_ClassWin.SetPosition(m_RaceWin.GetPositionX(), 372);

	// Class under line
	m_ClassUnderLine.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "underLine" ));
	m_ClassUnderLine.SetPosition(15, 404);

	// Make 배경
	m_MakeWin.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "make_1_Left" ));
	m_MakeWin.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "make_1_Center" ));
	m_MakeWin.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "make_1_Right" ));
	m_MakeWin.SetPositionfromParent(793, 59);

	m_MakeWin2.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "make_2_Left" ));
	m_MakeWin2.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "make_2_Center" ));
	m_MakeWin2.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "make_2_Right" ));
	m_MakeWin2.SetPositionfromParent(793, 571);

	// Make under line
	m_MakeUnderLine.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "underLine" ));
	m_MakeUnderLine.SetPositionfromParent(800, 91);

	// Race text
	m_RaceText.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "race_text" ));

	// Class text
	m_ClassText.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "Class_text" ));

	// Make text
	m_MakeText.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "make_text" ));
	m_MakeText.SetPositionfromParent(806, 70);

	// 이름 입력란 배경
	m_NameInputBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "name_input" ));
	m_NameInputBack.SetPositionfromParent(400, 685);

	// 이름 입력란
	m_pNameInput = (gui::CInputBox*)GetComponent( "NameInput" );
	m_pNameInput->SetText(L"");

	// 종족 이름	
	rect.SetRectWH(90, 67, 150, 30);
	m_pRaceName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );	
	m_pRaceName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pRaceName->SetText(L"");
	m_pRaceName->Enable(false);

	// 클래스 이름	
	rect.SetRectWH(90, 380, 150, 30);
	m_pClassName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );	
	m_pClassName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pClassName->SetText(L"");
	m_pClassName->Enable(false);

	// 종족 설명
	rect.SetRectWH(19, 117, 201, 250);
	m_pRaceInfo = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT | COMP_TEXT_UP);	
	m_pRaceInfo->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pRaceInfo->SetText(L"");
	m_pRaceInfo->Enable(false);

	// 클래스 설명
	rect.SetRectWH(19, 431, 201, 250);
	m_pClassInfo = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT | COMP_TEXT_UP);	
	m_pClassInfo->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pClassInfo->SetText(L"");
	m_pClassInfo->Enable(false);

	// '캐릭터 만들기' 스태틱	
	rect.SetRectWH(900, 67, 150, 30);
	m_pCharMake =  NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pCharMake->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pCharMake->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_CHARACTER_MAKE));
	m_pCharMake->Enable(false);

	// '이름' 스태틱
	rect.SetRectWH(413, 704, 30, 20);
	m_pNameInputStatic =  NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_pNameInputStatic->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pNameInputStatic->SetText(GetDisplayStringManager()->GetString(DST_STATUS_NAME));
	m_pNameInputStatic->Enable(false);

	// 왼쪽 회전 버튼
	m_pLeftRotButton = (gui::CButton*)GetComponent("LeftRotButton");
	m_slotLeftRotButton = m_pLeftRotButton->SigClicked().Connect( this, &CCharCreateGuiGui::ClickRotButton );

	// 오른쪽 회전 버튼
	m_pRightRotButton = (gui::CButton*)GetComponent("RightRotButton");
	m_slotRightRotButton = m_pRightRotButton->SigClicked().Connect( this, &CCharCreateGuiGui::ClickRotButton );

	// 정지 버튼
	m_pStopButton = (gui::CButton*)GetComponent("StopButton");
	m_slotStopButton = m_pStopButton->SigClicked().Connect( this, &CCharCreateGuiGui::ClickRotButton );

	// 랜덤 버튼
	m_pRandomButton = (gui::CButton*)GetComponent("RandomButton");
	m_pRandomButton->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_RANDOM_CHOICE));
	m_slotRandomButton = m_pRandomButton->SigClicked().Connect( this, &CCharCreateGuiGui::ClickedRandomButton );

	// 만들기
	m_pMakeButton = (gui::CButton*)GetComponent("MakeButton");
	m_pMakeButton->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_MAKE));
	m_slotMakeButton = m_pMakeButton->SigClicked().Connect( this, &CCharCreateGuiGui::ClickedMakeButton );

	// 돌아가기 버튼
	m_pReturnButton = (gui::CButton*)GetComponent("ReturnButton");
	m_pReturnButton->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_RETURN));
	m_slotReturnButton = m_pReturnButton->SigClicked().Connect( this, &CCharCreateGuiGui::ClickedReturnButton);

	// 각 속성
	std::string fullName = "";
	char acLeft[] = "LeftButton_";
	char acRight[] = "RightButton_";
	char acStaticName[] = "Combo_Title_";
	char acNum[3] = "";

	gui::CSurface comboSurface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "combolist" );
	std::string aIconName[ATTIRIBUTE_NUM] = {"char_mak_ico01.png", "char_mak_ico07.png", "char_mak_ico02.png", "char_mak_ico03.png", "char_mak_ico04.png",
												"char_mak_ico05.png", "char_mak_ico06.png"};
	for(RwInt8 i = 0 ; i < ATTIRIBUTE_NUM ; ++i)
	{		
		// 슬롯 배경
		m_AttibuteSlot[i].Backboard.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "make_slot" ));
		m_AttibuteSlot[i].Backboard.SetPosition(806, 115 + i*ATTIRIBUTEHAEIGHT);

		// Text backboard
		m_AttibuteSlot[i].Textboard.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "TextBoard" ));
		m_AttibuteSlot[i].Textboard.SetPosition(855, 149+ i*ATTIRIBUTEHAEIGHT);

		// 슬롯 아이콘
		m_AttibuteSlot[i].Icon.Create(m_pThis, DIALOG_UNKNOWN, ISST_NORMATL_ICON);
		m_AttibuteSlot[i].Icon.SetPosition(818, 133 + i*ATTIRIBUTEHAEIGHT);
		m_AttibuteSlot[i].Icon.SetIcon(aIconName[i].c_str());

		// 슬롯 왼쪽 버튼
		_itoa_s(i+1, acNum, sizeof(acNum), 10);

		fullName = acLeft;
		fullName += acNum;
		m_AttibuteSlot[i].pLeftButton = (gui::CButton*)GetComponent(fullName.c_str());
		m_slotLeft[i] = m_AttibuteSlot[i].pLeftButton->SigClicked().Connect( this, &CCharCreateGuiGui::ClickLeftButton);

		// 슬롯 오른쪽 버튼
		fullName = acRight;
		fullName += acNum;
		m_AttibuteSlot[i].pRightButton = (gui::CButton*)GetComponent(fullName.c_str());
		m_slotRight[i] = m_AttibuteSlot[i].pRightButton->SigClicked().Connect( this, &CCharCreateGuiGui::ClickRightButton);

		// 슬롯 타이틀
		fullName = acStaticName;
		fullName += acNum;
		m_AttibuteSlot[i].pTitle = (gui::CStaticBox*)GetComponent(fullName.c_str());

		// 선택된 항목
		rect.SetRectWH(855, 151 + i*ATTIRIBUTEHAEIGHT, 140, 17);
		m_AttibuteSlot[i].pAttribute = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
		m_AttibuteSlot[i].pAttribute->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_AttibuteSlot[i].pAttribute->SetText(L"");
		m_AttibuteSlot[i].pAttribute->Enable(false);
				
		// 현재 선택된 인덱스
		m_AttibuteSlot[i].byValue = 0;
	}	

	// 종   족
	m_AttibuteSlot[ATTIRIBUTE_RACE].pTitle->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_RACE));
	// 성   별
	m_AttibuteSlot[ATTIRIBUTE_GENDER].pTitle->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_GENDER));
	// 직   업
	m_AttibuteSlot[ATTIRIBUTE_CLASS].pTitle->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_CLASS));
	// 머리모양
	m_AttibuteSlot[ATTIRIBUTE_HAIR].pTitle->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_HAIR));
	// 머리색
	m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].pTitle->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_HAIRCOLOR));
	// 얼   굴
	m_AttibuteSlot[ATTIRIBUTE_FACE].pTitle->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_FACE));
	// 피부색
	m_AttibuteSlot[ATTIRIBUTE_SKIN].pTitle->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_SKIN));	

	// sig	
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CCharCreateGuiGui::OnPaint );

	// 종족은 고정	
	m_AttibuteSlot[ATTIRIBUTE_RACE].listContent.push_back(GetDisplayStringManager()->GetString(DST_HUMAN));
	m_AttibuteSlot[ATTIRIBUTE_RACE].listContent.push_back(GetDisplayStringManager()->GetString(DST_NAMEK));
	//m_AttibuteSlot[ATTIRIBUTE_RACE].pTitle->SetText(GetDisplayStringManager()->GetString( DST_MAJIN ));

	// default
	m_AttibuteSlot[ATTIRIBUTE_RACE].SelectIndex(0);
	m_TempAttribute.listContent.clear();
	m_TempAttribute.byRaceValue = RACE_UNKNOWN;
	m_TempAttribute.byHair = 0xFF;

	// 현재 종족에 따라 선택할 수 있는 캐릭터 속성 리스트 표시
	UpdateComboBox();	

	// Locate Component
	RwInt32 iScreenWidth = GetDboGlobal()->GetScreenWidth();
	RwInt32 iScreenHeight = GetDboGlobal()->GetScreenHeight();

	if( iScreenWidth != m_pThis->GetWidth() ||  iScreenHeight != m_pThis->GetHeight() )
		LocateComponent(iScreenWidth, iScreenHeight);

	// sig
	m_slotAbsoluteWheelMove = GetNtlGuiManager()->GetGuiManager()->SigAbsoluteWheelMove().Connect( this, &CCharCreateGuiGui::OnAbsoluteWheelMove );

	Show(false);

	LinkMsg(g_EventCharStageStateEnter);
	LinkMsg(g_EventCharStageStateExit);	
	LinkMsg(g_EventResize);

	NTL_RETURN(TRUE);
}

VOID CCharCreateGuiGui::Destroy()
{
	NTL_FUNCTION("CCharCreateGuiGui::Destroy");

	UnLinkMsg(g_EventCharStageStateEnter);
	UnLinkMsg(g_EventCharStageStateExit);
	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
} 

VOID CCharCreateGuiGui::Clear()
{
	m_pRaceName->SetText(L"");
	m_pClassName->SetText(L"");
	m_pRaceInfo->SetText(L"");
	m_pClassInfo->SetText(L"");
	m_pNameInput->SetText(L"");

	for(RwInt8 i = 0 ; i < ATTIRIBUTE_NUM ; ++i)
	{		
		m_AttibuteSlot[i].byValue = 0;
	}
}

VOID CCharCreateGuiGui::UpdateComboBox()
{	
	m_AttibuteSlot[ATTIRIBUTE_GENDER].ClearText();
	m_AttibuteSlot[ATTIRIBUTE_CLASS].ClearText();
	m_AttibuteSlot[ATTIRIBUTE_HAIR].ClearText();
	m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].ClearText();
	m_AttibuteSlot[ATTIRIBUTE_FACE].ClearText();
	m_AttibuteSlot[ATTIRIBUTE_SKIN].ClearText();

	switch(m_AttibuteSlot[ATTIRIBUTE_RACE].byValue)
	{
	case RACE_HUMAN:
		{
			// 종족
			m_pRaceName->SetText(GetDisplayStringManager()->GetString( DST_HUMAN ));
			m_pRaceInfo->SetText(GetDisplayStringManager()->GetString( DST_LOBBY_HUMAN_EXPLAIN ));

			// 클래스
			m_AttibuteSlot[ATTIRIBUTE_CLASS].listContent.push_back(GetDisplayStringManager()->GetString( DST_HUMAN_FIGHTER ));
			m_AttibuteSlot[ATTIRIBUTE_CLASS].listContent.push_back(GetDisplayStringManager()->GetString( DST_HUMAN_MYSTIC ));
			m_pClassName->SetText(GetDisplayStringManager()->GetString( DST_HUMAN_FIGHTER ));
			m_pClassInfo->SetText(GetDisplayStringManager()->GetString( DST_LOBBY_HUMAN_FIGHTER ));

			// 성별
			m_AttibuteSlot[ATTIRIBUTE_GENDER].listContent.push_back(GetDisplayStringManager()->GetString( DST_AVATAR_SEX_MALE ));
			m_AttibuteSlot[ATTIRIBUTE_GENDER].listContent.push_back(GetDisplayStringManager()->GetString( DST_AVATAR_SEX_FEMALE ));

			if( m_AttibuteSlot[ATTIRIBUTE_GENDER].byValue == GENDER_MALE )
			{
				// 머리모양
				for(RwInt32 i = 0 ; i < DEFAULT_HEAD_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_HAIR].listContent.push_back(GetDisplayStringManager()->GetString( DST_HUMAN_MALE_HAIRTYPE1 + i ));

				// 머리색
				for(RwInt32 i = 0 ; i < DEFAULT_HEAD_COLOR_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].listContent.push_back(GetDisplayStringManager()->GetString( DST_HUMAN_MALE_HAIRCOLOR1 + i ));

				// 얼굴
				for(RwInt32 i = 0 ; i < DEFAULT_FACE_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_FACE].listContent.push_back(GetDisplayStringManager()->GetString( DST_HUMAN_MALE_FACE1 + i ));

				// 피부
				for(RwInt32 i = 0 ; i < DEFAULT_SKIN_COLOR_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_SKIN].listContent.push_back(GetDisplayStringManager()->GetString( DST_HUMAN_MALE_SKIN1 + i ));
			}
			else
			{				
				// 머리모양
				for(RwInt32 i = 0 ; i < DEFAULT_HEAD_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_HAIR].listContent.push_back(GetDisplayStringManager()->GetString( DST_HUMAN_FEMALE_HAIRTYPE1 + i ));

				// 머리색
				for(RwInt32 i = 0 ; i < DEFAULT_HEAD_COLOR_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].listContent.push_back(GetDisplayStringManager()->GetString( DST_HUMAN_FEMALE_HAIRCOLOR1 + i ));

				// 얼굴
				for(RwInt32 i = 0 ; i < DEFAULT_FACE_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_FACE].listContent.push_back(GetDisplayStringManager()->GetString( DST_HUMAN_FEMALE_FACE1 + i ));

				// 피부
				for(RwInt32 i = 0 ; i < DEFAULT_SKIN_COLOR_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_SKIN].listContent.push_back(GetDisplayStringManager()->GetString( DST_HUMAN_FEMALE_SKIN1 + i ));
			}
			break;
		}
	case RACE_NAMEK:
		{	
			// 종족
			m_pRaceName->SetText(GetDisplayStringManager()->GetString( DST_NAMEK ));
			m_pRaceInfo->SetText(GetDisplayStringManager()->GetString( DST_LOBBY_NAMEK_EXPLAIN ));

			// 클래스
			m_AttibuteSlot[ATTIRIBUTE_CLASS].listContent.push_back(GetDisplayStringManager()->GetString( DST_NAMEK_FIGHTER ));
			m_AttibuteSlot[ATTIRIBUTE_CLASS].listContent.push_back(GetDisplayStringManager()->GetString( DST_NAMEK_MYSTIC ));
			m_pClassName->SetText(GetDisplayStringManager()->GetString( DST_NAMEK_FIGHTER ));
			m_pClassInfo->SetText(GetDisplayStringManager()->GetString( DST_LOBBY_NAMEK_FIGHTER ));

			// 성별
			m_AttibuteSlot[ATTIRIBUTE_GENDER].listContent.push_back(GetDisplayStringManager()->GetString( DST_AVATAR_SEX_NOT_EXIST ));

			// 머리모양
			for(RwInt32 i = 0 ; i < DEFAULT_HEAD_NUM ; ++i )
				m_AttibuteSlot[ATTIRIBUTE_HAIR].listContent.push_back(GetDisplayStringManager()->GetString( DST_NAMEKAN_HAIRTYPE1 + i ));

			// 머리색
			for(RwInt32 i = 0 ; i < DEFAULT_HEAD_COLOR_NUM ; ++i )
				m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].listContent.push_back(GetDisplayStringManager()->GetString( DST_NAMEKAN_HAIRCOLOR1 + i ));

			// 얼굴
			for(RwInt32 i = 0 ; i < DEFAULT_FACE_NUM ; ++i )
				m_AttibuteSlot[ATTIRIBUTE_FACE].listContent.push_back(GetDisplayStringManager()->GetString( DST_NAMEKAN_FACE1 + i ));

			// 피부
			for(RwInt32 i = 0 ; i < DEFAULT_SKIN_COLOR_NUM ; ++i )
				m_AttibuteSlot[ATTIRIBUTE_SKIN].listContent.push_back(GetDisplayStringManager()->GetString( DST_NAMEKAN_SKIN1 + i ));

			break;
		}
	case RACE_MAJIN:
		{
			// 종족
			m_pRaceName->SetText(GetDisplayStringManager()->GetString( DST_MAJIN ));
			m_pRaceInfo->SetText(GetDisplayStringManager()->GetString( DST_LOBBY_MAJIN_EXPLAIN ));

			// 클래스
			m_AttibuteSlot[ATTIRIBUTE_CLASS].listContent.push_back(GetDisplayStringManager()->GetString( DST_MIGHTY_MAJIN ));
			m_AttibuteSlot[ATTIRIBUTE_CLASS].listContent.push_back(GetDisplayStringManager()->GetString( DST_WONDER_MAJIN ));
			m_pClassName->SetText(GetDisplayStringManager()->GetString( DST_MIGHTY_MAJIN ));
			m_pClassInfo->SetText(GetDisplayStringManager()->GetString( DST_LOBBY_MIGHTY_MAJIN ));

			// 성별
			m_AttibuteSlot[ATTIRIBUTE_GENDER].listContent.push_back(GetDisplayStringManager()->GetString( DST_AVATAR_SEX_MALE ));
			m_AttibuteSlot[ATTIRIBUTE_GENDER].listContent.push_back(GetDisplayStringManager()->GetString( DST_AVATAR_SEX_FEMALE ));

			if( m_AttibuteSlot[ATTIRIBUTE_GENDER].byValue == GENDER_MALE )
			{
				// 머리모양
				for(RwInt32 i = 0 ; i < DEFAULT_HEAD_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_HAIR].listContent.push_back(GetDisplayStringManager()->GetString( DST_MAJIN_MALE_HAIRTYPE1 + i ));

				// 머리색
				for(RwInt32 i = 0 ; i < DEFAULT_HEAD_COLOR_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].listContent.push_back(GetDisplayStringManager()->GetString( DST_MAJIN_MALE_HAIRCOLOR1 + i ));

				// 얼굴
				for(RwInt32 i = 0 ; i < DEFAULT_FACE_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_FACE].listContent.push_back(GetDisplayStringManager()->GetString( DST_MAJIN_MALE_FACE1 + i ));

				// 피부
				for(RwInt32 i = 0 ; i < DEFAULT_SKIN_COLOR_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_SKIN].listContent.push_back(GetDisplayStringManager()->GetString( DST_MAJIN_MALE_SKIN1 + i ));
			}
			else
			{
				// 머리모양
				for(RwInt32 i = 0 ; i < DEFAULT_HEAD_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_HAIR].listContent.push_back(GetDisplayStringManager()->GetString( DST_MAJIN_FEMALE_HAIRTYPE1 + i ));

				// 머리색
				for(RwInt32 i = 0 ; i < DEFAULT_HEAD_COLOR_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].listContent.push_back(GetDisplayStringManager()->GetString( DST_MAJIN_FEMALE_HAIRCOLOR1 + i ));

				// 얼굴
				for(RwInt32 i = 0 ; i < DEFAULT_FACE_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_FACE].listContent.push_back(GetDisplayStringManager()->GetString( DST_MAJIN_FEMALE_FACE1 + i ));

				// 피부
				for(RwInt32 i = 0 ; i < DEFAULT_SKIN_COLOR_NUM ; ++i )
					m_AttibuteSlot[ATTIRIBUTE_SKIN].listContent.push_back(GetDisplayStringManager()->GetString( DST_MAJIN_FEMALE_SKIN1 + i ));
			}
			break;
		}
	}
	
	m_AttibuteSlot[ATTIRIBUTE_GENDER].SelectIndex(0);
	m_AttibuteSlot[ATTIRIBUTE_CLASS].SelectIndex(0);
	m_AttibuteSlot[ATTIRIBUTE_HAIR].SelectIndex(0);
	m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].SelectIndex(0);
	m_AttibuteSlot[ATTIRIBUTE_FACE].SelectIndex(0);
	m_AttibuteSlot[ATTIRIBUTE_SKIN].SelectIndex(0);

	// 리스트가 바뀌면 모델은 정지한 상태로 정면을 바라보나
	CDboEventGenerator::LobbyMessage(LMT_ROTATION_FOREWARD);
}

VOID CCharCreateGuiGui::ClickLeftButton(gui::CComponent* pComponent)
{
	GetSoundManager()->Play(NULL, SOUND_UI, GSD_SYSTEM_BUTTON_CLICK);

	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	for( RwUInt8 i = 0 ; i < ATTIRIBUTE_NUM ; ++i )
	{
		if( m_AttibuteSlot[i].pLeftButton == pComponent )
		{
			if( m_AttibuteSlot[i].listContent.size() == 0 )
				return;

			if( m_AttibuteSlot[i].byValue > 0 )
				UpdateSlot(false, i);
		}
	}	
}

VOID CCharCreateGuiGui::ClickRightButton(gui::CComponent* pComponent)
{
	GetSoundManager()->Play(NULL, SOUND_UI, GSD_SYSTEM_BUTTON_CLICK);

	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	for( RwUInt8 i = 0 ; i < ATTIRIBUTE_NUM ; ++i )
	{
		if( m_AttibuteSlot[i].pRightButton == pComponent )
		{
			if( m_AttibuteSlot[i].listContent.size() == 0 )
				return;

			if( m_AttibuteSlot[i].byValue < m_AttibuteSlot[i].listContent.size() - 1 )
				UpdateSlot(true, i);
		}
	}	
}

VOID CCharCreateGuiGui::UpdateSlot(RwBool bRight, RwUInt8 byIndex)
{
	// 인덱스 변경
	if( bRight )
		m_AttibuteSlot[byIndex].SelectIndex(m_AttibuteSlot[byIndex].byValue + 1);
	else
		m_AttibuteSlot[byIndex].SelectIndex(m_AttibuteSlot[byIndex].byValue - 1);

	if( byIndex == ATTIRIBUTE_RACE )
	{
		// 종족이 바뀌었다.
		UpdateComboBox();

		ChangeModel(true);
	}
	else if( byIndex == ATTIRIBUTE_CLASS )
	{
		// 클래스가 바뀌었다
		if( m_AttibuteSlot[ATTIRIBUTE_RACE].byValue == RACE_HUMAN )
		{
			m_pClassName->SetText(GetDisplayStringManager()->GetString(DST_HUMAN_FIGHTER  + m_AttibuteSlot[byIndex].byValue));
			m_pClassInfo->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_HUMAN_FIGHTER  + m_AttibuteSlot[byIndex].byValue));
		}
		else if( m_AttibuteSlot[ATTIRIBUTE_RACE].byValue == RACE_NAMEK )
		{
			m_pClassName->SetText(GetDisplayStringManager()->GetString(DST_NAMEK_FIGHTER  + m_AttibuteSlot[byIndex].byValue));
			m_pClassInfo->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_NAMEK_FIGHTER  + m_AttibuteSlot[byIndex].byValue));
		}
		else if( m_AttibuteSlot[ATTIRIBUTE_RACE].byValue == RACE_MAJIN )
		{
			m_pClassName->SetText(GetDisplayStringManager()->GetString(DST_MIGHTY_MAJIN  + m_AttibuteSlot[byIndex].byValue));
			m_pClassInfo->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_MIGHTY_MAJIN  + m_AttibuteSlot[byIndex].byValue));
		}

		ChangeModel(false);
	}	
	else
	{
		ChangeModel(false);
	}
	
	// 버튼 사용여부 판단
	CheckEnableButton();
}

VOID CCharCreateGuiGui::CheckEnableButton()
{
	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	// 휴먼 대머리의 경우 머리색을 고를 수 없다	
	if( m_AttibuteSlot[ATTIRIBUTE_RACE].byValue == RACE_HUMAN )
	{
		// 대머리
		if( m_AttibuteSlot[ATTIRIBUTE_HAIR].byValue == 1 &&
			m_AttibuteSlot[ATTIRIBUTE_GENDER].byValue != GENDER_FEMALE )
		{
			if(m_TempAttribute.byHair != 1)
			{
				m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].pLeftButton->Enable(false);
				m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].pRightButton->Enable(false);

				// 정보 저장
				m_TempAttribute.listContent = m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].listContent;
				m_TempAttribute.byRaceValue = m_AttibuteSlot[ATTIRIBUTE_RACE].byValue;
				m_TempAttribute.byHair = m_AttibuteSlot[ATTIRIBUTE_HAIR].byValue;

				m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].ClearText();
				// 선택불가
				m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].pAttribute->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_CANNOT_CHOICE));
			}			
		}
		else
		{
			m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].pLeftButton->Enable(true);
			m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].pRightButton->Enable(true);

			if(m_TempAttribute.byRaceValue == RACE_HUMAN &&
			   m_TempAttribute.byHair == 1)
			{
				m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].listContent = m_TempAttribute.listContent;
				m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].SelectIndex(m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].byValue);

				m_TempAttribute.byRaceValue = RACE_UNKNOWN;
				m_TempAttribute.byHair = 0xFF;
				m_TempAttribute.listContent.clear();
			}
		}
	}
	// 나메크인의 경우 머리색을 고를 수 없다
	else if(m_AttibuteSlot[ATTIRIBUTE_RACE].byValue == RACE_NAMEK )
	{
		m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].pLeftButton->Enable(false);
		m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].pRightButton->Enable(false);

		// 정보 저장
		m_TempAttribute.listContent = m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].listContent;
		m_TempAttribute.byRaceValue = m_AttibuteSlot[ATTIRIBUTE_RACE].byValue;
		m_TempAttribute.byHair = m_AttibuteSlot[ATTIRIBUTE_HAIR].byValue;

		m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].ClearText();
		m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].pAttribute->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_CANNOT_CHOICE));
	}
}

VOID CCharCreateGuiGui::LocateComponent(RwInt32 iWidth, RwInt32 iHeight)
{
	CRectangle rect;
	CPos pos_from_Parent;

	rect.left = 0;
	rect.top = 0;
	rect.right = iWidth;
	rect.bottom = iHeight;

	m_pThis->SetPosition(rect);	

	pos_from_Parent = m_MakeWin.GetPosfromParent();
	m_MakeWin.SetPosition(iWidth - (dDEFAULTWIDTH - pos_from_Parent.x), m_MakeWin.GetPositionY());

	pos_from_Parent = m_MakeWin2.GetPosfromParent();
	m_MakeWin2.SetPosition(iWidth - (dDEFAULTWIDTH - pos_from_Parent.x), m_MakeWin2.GetPositionY());

	pos_from_Parent = m_MakeText.GetPosfromParent();
	m_MakeText.SetPosition(iWidth - (dDEFAULTWIDTH - pos_from_Parent.x), pos_from_Parent.y);

	pos_from_Parent = m_MakeUnderLine.GetPosfromParent();
	m_MakeUnderLine.SetPosition(iWidth - (dDEFAULTWIDTH - pos_from_Parent.x), pos_from_Parent.y);

	rect = m_pCharMake->GetPosition();
	m_pCharMake->SetPosition(iWidth - 124, rect.top);
	
	for(RwUInt8 i = 0 ; i < ATTIRIBUTE_NUM ; ++i)
	{
		m_AttibuteSlot[i].SetPosition(iWidth - 218,  115 + i*ATTIRIBUTEHAEIGHT);
	}

	pos_from_Parent = m_NameInputBack.GetPosfromParent();
	m_NameInputBack.SetPosition((RwInt32)((RwReal)pos_from_Parent.x/dDEFAULTWIDTH*iWidth), iHeight - 83);
	m_NameInputBack.GetRect(rect);
	
	m_pNameInput->SetPosition(rect.left + 64, rect.top + 22);

	m_pNameInputStatic->SetPosition(rect.left + 13, rect.top + 19);

	m_pLeftRotButton->SetPosition((RwInt32)(300.f/dDEFAULTWIDTH*iWidth), iHeight - 194);
	m_pRightRotButton->SetPosition((RwInt32)(663.f/dDEFAULTWIDTH*iWidth), iHeight - 194);
	m_pStopButton->SetPosition((RwInt32)(486.f/dDEFAULTWIDTH*iWidth), iHeight - 164);

	rect = m_pRandomButton->GetPosition();
	m_pRandomButton->SetPosition(iWidth - 208, 582);

	rect = m_pMakeButton->GetPosition();
	m_pMakeButton->SetPosition(iWidth - 208, 613);

	m_pReturnButton->SetPosition(iWidth - 208, 663);
}

VOID CCharCreateGuiGui::ChangeModel(RwBool bSetDefaultCamera)
{
	RwUInt8 byGender, byClass;

	if( m_AttibuteSlot[ATTIRIBUTE_RACE].byValue == RACE_NAMEK )
	{
		// 나메크인의 피부색과 머리색은 같다
		m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].byValue = m_AttibuteSlot[ATTIRIBUTE_SKIN].byValue;
	}
	
	byClass = ConverIndexToClass(m_AttibuteSlot[ATTIRIBUTE_CLASS].byValue);
	byGender = ConverIndexToGenderbyRace(m_AttibuteSlot[ATTIRIBUTE_RACE].byValue);

	// +1 : 모델 인덱스가 1부터 시작하는 것도 있다
	CDboEventGenerator::CharMaking(bSetDefaultCamera,
									m_AttibuteSlot[ATTIRIBUTE_RACE].byValue,
									byClass,
									m_AttibuteSlot[ATTIRIBUTE_HAIR].byValue + 1,
									m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].byValue + 1,
									m_AttibuteSlot[ATTIRIBUTE_FACE].byValue + 1,
									m_AttibuteSlot[ATTIRIBUTE_SKIN].byValue + 1,
									byGender);
}

VOID CCharCreateGuiGui::ClickRotButton(gui::CComponent* pComponent)
{
	GetSoundManager()->Play(NULL, SOUND_UI, GSD_SYSTEM_BUTTON_CLICK);

	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	if( pComponent == m_pLeftRotButton )
	{
		if( m_byRotationState == ER_TURN_LEFT )
		{
			m_byRotationState = ER_STOP;
			CDboEventGenerator::LobbyMessage(LMT_ROTATION_STOP);
		}
		else
		{
			m_byRotationState = ER_TURN_LEFT;
			CDboEventGenerator::LobbyMessage(LMT_ROTATION_LEFT, 100.0f);
		}
	}
	else if( pComponent == m_pRightRotButton )
	{
		if( m_byRotationState == ER_TURN_RIGHT )
		{
			m_byRotationState = ER_STOP;
			CDboEventGenerator::LobbyMessage(LMT_ROTATION_STOP);
		}
		else
		{
			m_byRotationState = ER_TURN_RIGHT;
			CDboEventGenerator::LobbyMessage(LMT_ROTATION_RIGHT, 100.0f);
		}		
	}
	else
	{
		m_byRotationState = ER_STOP;
		CDboEventGenerator::LobbyMessage(LMT_ROTATION_FOREWARD);
	}
}

VOID CCharCreateGuiGui::ClickedRandomButton(gui::CComponent* pComponent)
{
	GetSoundManager()->Play(NULL, SOUND_UI, GSD_SYSTEM_BUTTON_CLICK);

	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	// 랜덤값 얻어오기
	m_AttibuteSlot[ATTIRIBUTE_HAIR].SelectIndex(NtlRandomNumber(0, DEFAULT_HEAD_NUM - 1));
	m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].SelectIndex(NtlRandomNumber(0, DEFAULT_HEAD_COLOR_NUM - 1));
	m_AttibuteSlot[ATTIRIBUTE_FACE].SelectIndex(NtlRandomNumber(0, DEFAULT_FACE_NUM - 1));
	m_AttibuteSlot[ATTIRIBUTE_SKIN].SelectIndex(NtlRandomNumber(0, DEFAULT_SKIN_COLOR_NUM - 1));

	CheckEnableButton();

	// 실제 모델 바꾸기
	ChangeModel(false);
}

VOID CCharCreateGuiGui::ClickedMakeButton(gui::CComponent* pComponent)
{
	GetSoundManager()->Play(NULL, SOUND_UI, GSD_SYSTEM_BUTTON_CLICK);

	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	RwInt32 iNumByte = GetByte(m_pNameInput->GetText());

	if( iNumByte == 0 )
	{
		// 이름을 입력하여 주십시요
		CDboEventGenerator::LobbyMsgBox(LMBT_OK, LMBW_NONE, GetDisplayStringManager()->GetString(DST_LOBBY_EMPTYNAME));
		return;
	}
	else if( iNumByte < NTL_MIN_SIZE_CHAR_NAME )
	{		
		// 이름이 너무 짧습니다
		CDboEventGenerator::LobbyMsgBox(LMBT_OK, LMBW_NONE, GetDisplayStringManager()->GetString(DST_LOBBY_SHORTNAME));
		return;
	}
	else if( iNumByte > NTL_MAX_SIZE_CHAR_NAME )
	{
		// 이름이 너무 깊니다
		CDboEventGenerator::LobbyMsgBox(LMBT_OK, LMBW_NONE, GetDisplayStringManager()->GetString(DST_LOBBY_LONGNAME));
		return;
	}
	else if( GetChattingFilter()->IsSlang((WCHAR*)m_pNameInput->GetText()) )
	{
		// 사용할 수 없는 단어가 있습니다
		CDboEventGenerator::LobbyMsgBox(LMBT_OK, LMBW_NONE, GetDisplayStringManager()->GetString(DST_LOBBY_CAN_NOT_USE_THE_WORD));
		return;
	}

	RwUInt8 byGender, byClass;

	byClass = ConverIndexToClass(m_AttibuteSlot[ATTIRIBUTE_CLASS].byValue);
	byGender = ConverIndexToGenderbyRace(m_AttibuteSlot[ATTIRIBUTE_RACE].byValue);

	// +1 : 모델 인덱스가 1부터 시작하는 것도 있다
	GetDboGlobal()->GetLobbyPacketGenerator()->SendCharAddReq(m_pNameInput->GetText(),
						m_AttibuteSlot[ATTIRIBUTE_RACE].byValue, byClass, 
						byGender,  m_AttibuteSlot[ATTIRIBUTE_FACE].byValue + 1,
						m_AttibuteSlot[ATTIRIBUTE_HAIR].byValue + 1, m_AttibuteSlot[ATTIRIBUTE_HAIR_COLOR].byValue + 1, 
						m_AttibuteSlot[ATTIRIBUTE_SKIN].byValue + 1, 0);

	GetCharStageState()->ChangeState(CHAR_STATE_MAKE_REQUEST);
}

VOID CCharCreateGuiGui::ClickedReturnButton(gui::CComponent* pComponent)
{
	GetSoundManager()->Play(NULL, SOUND_UI, GSD_SYSTEM_BUTTON_CANCEL);

	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	GetCharStageState()->ChangeState(CHAR_STATE_MAKE_EXIT);
}

VOID CCharCreateGuiGui::OnAbsoluteWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	if( !IsShow() )
		return;

	if(sDelta < 0)
		CDboEventGenerator::LobbyMessage(LMT_ZOOM_OUT);
	else
		CDboEventGenerator::LobbyMessage(LMT_ZOOM_IN);
}

VOID CCharCreateGuiGui::OnPaint()
{
	m_RaceWin.Render();
	m_ClassWin.Render();
	m_MakeWin.Render();
	m_MakeWin2.Render();

	m_RaceUnderLine.Render();
	m_ClassUnderLine.Render();
	m_MakeUnderLine.Render();

	m_RaceText.Render();
	m_ClassText.Render();
	m_MakeText.Render();
	m_NameInputBack.Render();

	for(RwInt8 i = 0 ; i < ATTIRIBUTE_NUM ; ++i)
	{
		m_AttibuteSlot[i].Backboard.Render();
		m_AttibuteSlot[i].Textboard.Render();
		m_AttibuteSlot[i].Icon.Paint();
	}
}

RwUInt8 CCharCreateGuiGui::ConverIndexToClass(RwInt32 iIndex)
{
	switch(m_AttibuteSlot[ATTIRIBUTE_RACE].byValue)
	{
	case RACE_HUMAN:
		{
			switch(iIndex)
			{
			case 0:		return PC_CLASS_HUMAN_FIGHTER;
			case 1:		return PC_CLASS_HUMAN_MYSTIC;
			case 2:		return PC_CLASS_HUMAN_ENGINEER;
			}
		}
	case RACE_NAMEK:
		{
			switch(iIndex)
			{
			case 0:		return PC_CLASS_NAMEK_FIGHTER;
			case 1:		return PC_CLASS_NAMEK_MYSTIC;
			}
		}
	case RACE_MAJIN:
		{
			switch(iIndex)
			{
			case 0:		return PC_CLASS_MIGHTY_MAJIN;
			case 1:		return PC_CLASS_WONDER_MAJIN;
			}
		}
	}

	NTL_ASSERT(false, "Unknown class");
	return RACE_UNKNOWN;
}

RwUInt8 CCharCreateGuiGui::ConverIndexToGenderbyRace(RwInt32 iRace)
{
	RwUInt8 byGender = INVALID_INDEX;

	if( iRace == RACE_NAMEK )
	{
		byGender = GENDER_ONE_SEX;
	}
	else if( iRace == RACE_HUMAN )
	{
		byGender = m_AttibuteSlot[ATTIRIBUTE_GENDER].byValue;
	}

	NTL_ASSERT(byGender != INVALID_INDEX, "Unknown gender " << iRace);
	return byGender;
}

VOID CCharCreateGuiGui::SetAlpha(RwUInt8 byAlpha)
{
	GetDialog()->SetAlpha(byAlpha);

	m_RaceWin.SetAlpha(byAlpha);
	m_ClassWin.SetAlpha(byAlpha);
	m_MakeWin.SetAlpha(byAlpha);
	m_MakeWin2.SetAlpha(byAlpha);
	m_RaceText.SetAlpha(byAlpha);
	m_ClassText.SetAlpha(byAlpha);
	m_RaceUnderLine.SetAlpha(byAlpha);
	m_ClassUnderLine.SetAlpha(byAlpha);
	m_MakeText.SetAlpha(byAlpha);
	m_MakeUnderLine.SetAlpha(byAlpha);
	m_NameInputBack.SetAlpha(byAlpha);

	for(RwUInt8 i = 0 ; i < ATTIRIBUTE_NUM ; ++i )
	{
		m_AttibuteSlot[i].Icon.SetAlpha(byAlpha);
		m_AttibuteSlot[i].Backboard.SetAlpha(byAlpha);
		m_AttibuteSlot[i].Textboard.SetAlpha(byAlpha);
	}
}

VOID CCharCreateGuiGui::EnableButtons(bool bEnable)
{
	m_pLeftRotButton->Enable(bEnable);
	m_pRightRotButton->Enable(bEnable);
	m_pStopButton->Enable(bEnable);
	m_pRandomButton->Enable(bEnable);
	m_pMakeButton->Enable(bEnable);
	m_pReturnButton->Enable(bEnable);
}

VOID CCharCreateGuiGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CCharCreateGuiGui::HandleEvents");

	if( msg.Id == g_EventCharStageStateEnter )
	{
		SDboEventCharStageStateEnter* pPacket = reinterpret_cast<SDboEventCharStageStateEnter*>( msg.pData );

		if( pPacket->byState == CHAR_STATE_MAKE_IDLE )
			EnableButtons(true);
		else
			EnableButtons(false);

		if( pPacket->byState == CHAR_STATE_MAKE_ENTER )
		{
			Clear();

			m_AttibuteSlot[ATTIRIBUTE_RACE].SelectIndex(0);
			UpdateComboBox();

			ChangeModel(true);

			Show(true);
		}
	}
	else if( msg.Id == g_EventCharStageStateExit )
	{
		SDboEventCharStageStateExit* pPacket = reinterpret_cast<SDboEventCharStageStateExit*>( msg.pData );
		if( pPacket->byState == CHAR_STATE_MAKE_EXIT )
		{
			m_byRotationState = ER_STOP;
			CDboEventGenerator::LobbyMessage(LMT_ROTATION_FOREWARD);

			if( GetCharStageState()->GetReservateState() == CHAR_STATE_RETURN_LOGIN )
			{
				CDboEventGenerator::LobbyMessage(LMT_DESTORY_LOBBY_AVATAR);
				CDboEventGenerator::LobbyMessage(LMT_DESTORY_LOBBY_WORLD);
			}			

			Show(false);
		}
	}
	else if(msg.Id == g_EventResize)
	{
		SDboEventResize* pPacket = reinterpret_cast<SDboEventResize*>( msg.pData );		
		LocateComponent(pPacket->iWidht, pPacket->iHeight);	
	}
	
	NTL_RETURNVOID();
}

/*****************************************************************************
* CRaceExplainGui
*****************************************************************************/
/*
CRaceExplainGui::CRaceExplainGui(const RwChar* pName)
:CNtlPLGui(pName)
{

}

CRaceExplainGui::~CRaceExplainGui()
{

}

RwBool CRaceExplainGui::Create()
{
	NTL_FUNCTION( "CRaceExplainGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\CharCreate_Explain.srf", "gui\\CharCreate_Race.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// 배경
	m_Background.SetType(CWindowby3::WT_VERTICAL);
	m_Background.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfBackgroundLeft" ));
	m_Background.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfBackgroundCenter" ));
	m_Background.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfBackgroundRight" ));	

	// Race text surface
	m_srfRaceText.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfRaceText" ));

	// 종족 이름 배경
	m_srtRaceNameBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfNameBar" ));

	// 종족 이름
	rect.SetRectWH(48, 70, 169, 14);
	m_pRaceName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pRaceName->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pRaceName->SetText("인간");
	m_pRaceName->Enable(false);

	// sig
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CRaceExplainGui::OnPaint );

	NTL_RETURN(TRUE);
}

VOID CRaceExplainGui::Destroy()
{
	NTL_FUNCTION("CRaceExplainGui::Destroy");

	NTL_RETURNVOID();
}

VOID CRaceExplainGui::SetRace(RwUInt8 byRace)
{
}

VOID CRaceExplainGui::SetPosition(RwInt32 iXPos, RwInt32 iYPos)
{
	m_pThis->SetPosition(iXPos, iYPos);

	RwInt32 iStartX = iXPos + 20;
	RwInt32 iStartY = iYPos + 20;

	m_Background.SetPosition(iStartX, iStartY);
	m_srfRaceText.SetPosition(iStartX + 49, iStartY + 9);
	m_srtRaceNameBack.SetPosition(iStartX + 30, iStartY + 47);
}

VOID CRaceExplainGui::OnPaint()
{
	m_Background.Render();
	m_srfRaceText.Render();
	m_srtRaceNameBack.Render();
}

/*****************************************************************************
* CRaceExplainGui
*****************************************************************************/
/*
CClassExplainGui::CClassExplainGui(const RwChar* pName)
:CNtlPLGui(pName)
{

}

CClassExplainGui::~CClassExplainGui()
{

}

RwBool CClassExplainGui::Create()
{
	NTL_FUNCTION( "CClassExplainGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\CharCreate_Explain.srf", "gui\\CharCreate_Class.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// 배경
	m_Background.SetType(CWindowby3::WT_VERTICAL);
	m_Background.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfBackgroundLeft" ));
	m_Background.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfBackgroundCenter" ));
	m_Background.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfBackgroundRight" ));	

	// Class text surface
	m_srfClassText.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfRaceText" ));

	// 클래스 이름 배경
	m_srtClassNameBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfNameBar" ));

	// 종족 이름
	rect.SetRectWH(48, 70, 169, 14);
	m_pClassName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pClassName->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pClassName->SetText("무도가");
	m_pClassName->Enable(false);

	// sig
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CClassExplainGui::OnPaint );

	NTL_RETURN(TRUE);
}

VOID CClassExplainGui::Destroy()
{
	NTL_FUNCTION("CClassExplainGui::Destroy");

	NTL_RETURNVOID();
}

VOID CClassExplainGui::SetClass(RwUInt8 byRace)
{
}

VOID CClassExplainGui::SetPosition(RwInt32 iXPos, RwInt32 iYPos)
{
	m_pThis->SetPosition(iXPos, iYPos);

	RwInt32 iStartX = iXPos + 20;
	RwInt32 iStartY = iYPos + 20;

	m_Background.SetPosition(iStartX, iStartY);
	m_srfClassText.SetPosition(iStartX + 49, iStartY + 9);
	m_srtClassNameBack.SetPosition(iStartX + 30, iStartY + 47);
}

VOID CClassExplainGui::OnPaint()
{
	m_Background.Render();
	m_srfClassText.Render();
	m_srtClassNameBack.Render();
}

/*****************************************************************************
* CCharCreateGuiGui
*****************************************************************************/
/*
CCharCreateGuiGui::CCharCreateGuiGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_byRotationState(ER_STOP)
{
}

CCharCreateGuiGui::~CCharCreateGuiGui()
{

}

RwBool CCharCreateGuiGui::Create()
{
	NTL_FUNCTION( "CCharCreateGuiGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\CharCreate2.srf", "gui\\CharCreate2.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// 왼쪽 회전 버튼
	m_pLeftRotButton = (gui::CButton*)GetComponent("LeftRotButton");
	m_slotLeftRotButton = m_pLeftRotButton->SigClicked().Connect( this, &CCharCreateGuiGui::OnClickLeftTurnButton );

	// 오른쪽 회전 버튼
	m_pRightRotButton = (gui::CButton*)GetComponent("RightRotButton");
	m_slotRightRotButton = m_pRightRotButton->SigClicked().Connect( this, &CCharCreateGuiGui::OnClickRightTurnButton );

	// 정지 버튼
	m_pStopButton = (gui::CButton*)GetComponent("StopButton");
	m_slotStopButton = m_pStopButton->SigClicked().Connect( this, &CCharCreateGuiGui::OnClickStopButton );

	// 종족 설명 Gui
	m_pRaceExplainGui = NTL_NEW CRaceExplainGui("RaceExplain");
	m_pRaceExplainGui->Create();
	m_pRaceExplainGui->SetPosition(4, 53);

	// 클래스 설명 Gui
	m_pClassExplainGui = NTL_NEW CClassExplainGui("ClassExplain");
	m_pClassExplainGui->Create();
	m_pClassExplainGui->SetPosition(4, 364);

	//Show(false);

	// sig	
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CCharCreateGuiGui::OnPaint );

	LinkMsg(g_EventCharStageStateEnter);
	LinkMsg(g_EventCharStageStateExit);	
	LinkMsg(g_EventResize);

	NTL_RETURN(TRUE);
}

VOID CCharCreateGuiGui::Destroy()
{
	NTL_FUNCTION("CCharCreateGuiGui::Destroy");

	if( m_pRaceExplainGui )
	{
		m_pRaceExplainGui->Destroy();
		NTL_DELETE(m_pRaceExplainGui);
	}

	if( m_pClassExplainGui )
	{
		m_pClassExplainGui->Destroy();
		NTL_DELETE(m_pClassExplainGui);
	}

	UnLinkMsg(g_EventCharStageStateEnter);
	UnLinkMsg(g_EventCharStageStateExit);
	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CCharCreateGuiGui::SetAlpha(RwUInt8 byAlpha)
{

}

VOID CCharCreateGuiGui::LocateComponent(RwInt32 IWidth, RwInt32 IHeight)
{	
}

VOID CCharCreateGuiGui::OnClickLeftTurnButton(gui::CComponent* pComponent)
{
	GetSoundManager()->Play(NULL, SOUND_UI, GSD_SYSTEM_BUTTON_CLICK);

	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	if( m_byRotationState == ER_TURN_LEFT )
	{
		m_byRotationState = ER_STOP;
		CDboEventGenerator::LobbyMessage(LMT_ROTATION_STOP);
	}
	else
	{
		m_byRotationState = ER_TURN_LEFT;
		CDboEventGenerator::LobbyMessage(LMT_ROTATION_LEFT, 100.0f);
	}
}

VOID CCharCreateGuiGui::OnClickRightTurnButton(gui::CComponent* pComponent)
{
	GetSoundManager()->Play(NULL, SOUND_UI, GSD_SYSTEM_BUTTON_CLICK);

	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	if( m_byRotationState == ER_TURN_RIGHT )
	{
		m_byRotationState = ER_STOP;
		CDboEventGenerator::LobbyMessage(LMT_ROTATION_STOP);
	}
	else
	{
		m_byRotationState = ER_TURN_RIGHT;
		CDboEventGenerator::LobbyMessage(LMT_ROTATION_RIGHT, 100.0f);
	}
}

VOID CCharCreateGuiGui::OnClickStopButton(gui::CComponent* pComponent)
{
	GetSoundManager()->Play(NULL, SOUND_UI, GSD_SYSTEM_BUTTON_CLICK);

	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	m_byRotationState = ER_STOP;
	CDboEventGenerator::LobbyMessage(LMT_ROTATION_FOREWARD);
}

VOID CCharCreateGuiGui::OnPaint()
{

}

VOID CCharCreateGuiGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CCharCreateGuiGui::HandleEvents");


	if( msg.Id == g_EventCharStageStateEnter )
	{
		Show(true);
	}
	else if(msg.Id == g_EventResize)
	{
		SDboEventResize* pPacket = reinterpret_cast<SDboEventResize*>( msg.pData );		
		LocateComponent(pPacket->iWidht, pPacket->iHeight);	
	}

	NTL_RETURNVOID();
}*/