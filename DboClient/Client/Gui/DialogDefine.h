/******************************************************************************
* File			: DialogDefine.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 8. 11
* Abstract		: 
*****************************************************************************
* Desc			: Dialog 관련 정의
*****************************************************************************/

#pragma once

enum eDialogVisible
{
	DIALOGVISIBLE_FALSE,					///< 다이얼로그를 닫는다.
	DIALOGVISIBLE_TRUE						///< 다이얼로그를 연다.
};

// Be careful not to overlap the SideDilaog index
enum eDialogType
{
	DIALOG_FIRST,

	// GM Dialog (not affected by ESC key)

	DIALOG_GM_FIRST = DIALOG_FIRST,
	
	DIALOG_GM_QUICKSLOT = DIALOG_GM_FIRST,

	DIALOG_GM_LAST = DIALOG_GM_QUICKSLOT,


	// Default dialog (default dialog to be displayed during game, not affected by ESC key)

	DIALOG_DEFAULT_FIRST,

	DIALOG_HP = DIALOG_DEFAULT_FIRST,		///< HP 윈도우
	DIALOG_EXP,								///< EXP 창(경험치)
	DIALOG_CHAT,							///< 채팅창
	DIALOG_QUICKSLOT,						///< 퀵슬롯 창	
	DIALOG_BAGSLOT,							///< 캡슐 슬롯	
	DIALOG_MINIMAP,							///< 미니맵	
	DIALOG_MAINMENUBUTTON,					///< 메인 메뉴 버튼
	DIALOG_SIDEICON,						///< 사이드 아이콘
	DIALOG_SIDEDIALOG_MANAGER,				///< 사이드 다이얼로그 매니저	
	DIALOG_SIDEDIALOG_CONTROLLER,			///< Side dialog controller
	DIALOG_PARTYMENU,						///< 파티 메뉴(파티 맴버창을 자식으로 갖는다)
	DIALOG_GM_CHATTING,						///< GM 채팅
	DIALOG_GROUP_CHAT_MANAGER,				///< Group 채팅 매니저
	DIALOG_DICE_MANAGER,					///< 주사위 매니저
	DIALOG_VEHICLE,							///< 탈 것 조작창

	DIALOG_DEFAULT_LAST = DIALOG_VEHICLE,
	

	// Event Dialog (Basically it is not affected by ESC key)

	DIALOG_EVENT_FIRST,

	DIALOG_TARGER_UI = DIALOG_EVENT_FIRST,	///< 타겟 UI	// 19 in TW client
	DIALOG_SCOUTER_MEASURE,					///< 스카우터 전투력 측정
	DIALOG_SHOPING_CART,					///< 상점의 카트
	DIALOG_TRADECART,						///< 트레이드 카트
	DIALOG_PRIVATESHOP_CART,				///< 개인상점 카트
	DIALOG_NETPYSHOP_TRADE,					///< NetPy Shop Trade Cart
	DIALOG_WAREHOUSE_1,						///< 1번 창고
	DIALOG_WAREHOUSE_2,						///< 2번 창고
	DIALOG_WAREHOUSE_3,						///< 3번 창고
	DIALOG_WAREHOUSE_COMMON,				///< 공유 창고
	DIALOG_DBC_REWARD,						///< 드래곤볼 보상 GUI
	DIALOG_DBC_DRAGON_DIALOG,				///< 용신의 대화 말풍선 GUI
	DIALOG_DROPITEM_INFO,					///< 드롭아이템 정보창
	DIALOG_TMQ_REWARD,						///< 타임머쉰 퀘스트 보상창
	DIALOG_BACKBOARD,						///< 화면전체를 가리는 백보드창	
	DIALOG_FLASH_NOTIFY,					///< Flash를 이용한 Notify 알림창
	DIALOG_INTERFACE_SHAKE,					///< Window to speed up recovery by manipulating the interface at the time of knockdown
    DIALOG_NET_CONNECT_BOX,                 ///< 서버 연결 실패시 상태를 표시하는 다이얼로그 박스
    DIALOG_TELECAST,                        ///< Broadcasting window // by daneos: is this required?? 
	DIALOG_TIME_NOTIFY,						///< 시간 알림 표시창
	DIALOG_GUILD_WAREHOUSE_1,				///< 길드 1번 창고
	DIALOG_GUILD_WAREHOUSE_2,				///< 길드 2번 창고
	DIALOG_GUILD_WAREHOUSE_3,				///< 길드 3번 창고
	DIALOG_TUTORIAL_DIALOG_FOCUS,			///< 튜토리얼 다이얼로그 포커스창
	DIALOG_TUTORIAL_EXAMPLE,				///< 튜토리얼 예시창
	DIALOG_TUTORIAL_MESSAGE,				///< 튜토리얼 메세지
	DIALOG_TUTORIAL_CONFIRM,				///< 튜토리얼 확인 메세지
	DIALOG_PETITION_SATISFACTION,			///< GM 상담 만족도 조사창
	DIALOG_BROADCAST,
	DIALOG_PET_STATUSBAR,					///< 펫 상태 창    
	DIALOG_PET_SKILL_SLOT,                ///< 펫을 소환했을때 나타나는 스킬 슬롯창

	DIALOG_ITEM_EXCHANGE_CART,

	DIALOG_SCRAMBLE_NOTIFY,					///< 도장 쟁탈전 진행 알림창
	DIALOG_CCBD_NOTIFY,
	DIALOG_CCBD_REWARD,

	DIALOG_EVENT_LAST = DIALOG_CCBD_REWARD,

	
	// A dialog drawn on the screen by the user's operation

	DIALOG_NORMAL_FIRST, 

	DIALOG_MAINMENU = DIALOG_NORMAL_FIRST,	///< 메인 메뉴	
	DIALOG_ESCMENU,							///< ESC Menu			// 83 IN TW
	DIALOG_STATUS,							///< 스테이터스 윈도우			
	DIALOG_SKILL,							///< 스킬창
	DIALOG_CHAT_LOG,						///< 채팅로그창
    DIALOG_COMMUNITY,						///< 커뮤니티 다이얼로그
    DIALOG_FRIEND_LIST,                     ///< 친구 리스트/블랙 리스트
	DIALOG_WORLDMAP,						///< 월드맵
	DIALOG_OPTIONWND,						///< 옵션 윈도우
	DIALOG_HELPWND,							///< 헬프 윈도우	
	DIALOG_PRIVATESHOP,						///< 개인상점
	DIALOG_MAILSYSTEM,						///< 메일 시스템
	DIALOG_MAILSYSTEM_READ,					///< 메일 시스템 : 메일 읽기
	DIALOG_DBC_ALTAR,						///< 드래곤볼 컬렉션 제단 윈도우
    DIALOG_COMMU_TARGET,                    ///< 커뮤니티 타겟창

	DIALOG_HOIPOIMIX_RECIPE,
	DIALOG_HOIPOIMIX_CRAFT,

	DIALOG_CAPSULE_1,						///< 가방
	DIALOG_CAPSULE_2,
	DIALOG_CAPSULE_3,
	DIALOG_CAPSULE_4,
	DIALOG_CAPSULE_5,
	DIALOG_CAPSULE_NETPY,					///< 

	//DIALOG_MSGBOX,							///< Message box (center alignment)
	
	DIALOG_QUESTLIST,						///< 퀘스트리스트 
	DIALOG_QUESTPROGRESS,					///< 퀘스트진행	
	
	DIALOG_SKILL_RPBONUS,					///< RP BONUS 스킬 사용 GUI
	DIALOG_SKILL_RPBONUS_AUTO,

	DIALOG_RANKBOARD,						///< Ranking Board
	DIALOG_BUDOKAI_NEWS,					///< 천하제일 무도회 소식지
	DIALOG_BUDOKAI_REQUEST,					///< 천하제일 무도회 신청서
	DIALOG_BUDOKAI_TOURNAMENT,
	DIALOG_BUDOKAI_TOURNAMENT_MATCHINFO,	///< 토너먼트 MatchInfo

	DIALOG_TBGAMBLERESULT,					///< 겜블 캡슐 확인 창
	DIALOG_CHANNGEL_CHANGE,					///< 게임중 채널 변경 창		// 137 in tw
	DIALOG_PETITION,						///< GM 상담 요청창

	DIALOG_SKILLABILITYSET,					///< Skill Ability 설정 GUI
	DIALOG_SKILLABILITYUSE,

	DIALOG_SKILL_HTB_RPUSE,					///< HTB Skill Rp Use Gui

	DIALOG_ITEM_OPTIONRESTRUCTURE,
	DIALOG_QUICK_TELEPORT,
	DIALOG_PLAYER_TITLE,

	DIALOG_NORMAL_LAST = DIALOG_PLAYER_TITLE,


	//NPC-related dialog (Basically, it is not affected by ESC key)

	DIALOG_NPCDIALOG_FIRST,

	DIALOG_MULTIDIALOG = DIALOG_NPCDIALOG_FIRST,///< NPC 멀티다이얼로그
	
	DIALOG_QUESTMESSAGE,					///< 퀘스트메세지	
	DIALOG_QUESTPROPOSAL,					///< 퀘스트제안	
	DIALOG_QUESTREWARD,						///< 퀘스트보상
	DIALOG_ITEMUPGRADE,						///< 아이템 업그레이드	
	DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE,
	DIALOG_RBBOARD,							///< 랭크배틀 게시판
	DIALOG_RBCHALLENGE,						///< 랭크배틀 도전정보
	DIALOG_NPCSHOP,							///< NPC 상점	
	DIALOG_TMQBOARD,						///< 타임머신 퀘스트 게시판	
	DIALOG_ULTIMATEDUNGEON,
	DIALOG_WAREHOUSEBAR,					///< 창고 컨트롤 바	
    DIALOG_PORTAL,                          ///< 포탈 NPC 
	DIALOG_OBJECT_CLICK,					///< 오브젝트 클릭
	DIALOG_GUILD_WAREHOUSEBAR,				///< 길드 창고 컨트롤 바
	DIALOG_DOJO_INFO,						///< 도장 정보창
	DIALOG_DOJO_ACCOUNT,					///< 도장 창고 사용내역
	DIALOG_DOGI,							///< 도복 다이얼로그
	DIALOG_DOJO_UPGRADE,					///< Painting Upgrade Dialog

	DIALOG_BIND_SEALING,
	DIALOG_ITEM_EXCHANGE_SHOP,				//buy items with items

	DIALOG_NETPYSHOP,
	DIALOG_DURATION_EXTEND_CONFIRM,			///< Extension Dialog

	DIALOG_HLSHOP,
	DIALOG_HLSHOP_WAREHOUSE,
	DIALOG_HLSHOP_BUY,
	DIALOG_HLSHOP_BUY_CONFIRM,
	DIALOG_HLSHOP_GIFT,
	DIALOG_HLSHOP_GIFT_CONFIRM,
	DIALOG_AUCTION_HOUSE,

	DIALOG_CCBD_BOARD,

	DIALOG_NPCDIALOG_LAST = DIALOG_CCBD_BOARD,

	DIALOG_NUM,

	DIALOG_LAST = DIALOG_NPCDIALOG_LAST,

	DIALOG_UNKNOWN = 0xFF,					///< 잘못된 다이얼로그 인덱스
};

enum eDialogMode
{	
	DIALOGMODE_ITEM_REPAIR,					///< 아이템 수리 모드
	DIALOGMODE_ITEM_IDENTIFICATION,			///< 미확인 아이템 감정
	DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION,	///< 상점에서 미확인 아이템 감정
	DIALOGMODE_NARRATION,					///< 내레이션 모드

	DIALOGMODE_CHATTING_RESIZE_HORI,		///< 채팅 디스플레이창 수직 리사이즈
	DIALOGMODE_CHATTING_RESIZE_RIGHTUP,		///< 채팅 디스플레이창 우상단 리사이즈
	DIALOGMODE_CHATTING_RESIZE_RIGHTDOWN,	///< 채팅 디스플레이창 우하단 리사이즈
	DIALOGMODE_CHATTING_RESIZE_VERT,		///< 채팅 디스플레이창 수평 리사이즈

	DIALOGMODE_ITEM_DISASSEMBLE,
	DIALOGMODE_ITEM_BEAD,
	DIALOGMODE_SKILL_SELECT,

	DIALOGMODE_UNKNOWN,

	DIALOGMODE_FIRST = DIALOGMODE_ITEM_REPAIR,
	DIALOGMODE_LAST  = DIALOGMODE_NARRATION
};


// Regular dialog attribute
#define dRDA_NORMAL							0x00000000
#define dRDA_EXCLUSIVE						0x00000001	///< Do not yield when another regular dialog opens
#define dRDA_HALFSIZE						0x00000002	///< 다른 레귤러 다이얼로그보다 절반 정도의 크기로 아래쪽 정렬된다
#define dRDA_NOT_ESC						0x00000004	///< It is not affected by the ESC key

// Entire focusing dialog attribute
#define dEFDA_NORMAL						0x00000000
#define dEFDA_BACKBOARD						0x00000001


// GUI Common Define
#define dDIALOG_CLEINT_EDGE_GAP				0

#define DBOGUI_DIALOG_TITLE_X				30
#define DBOGUI_DIALOG_TITLE_Y				2

#define DBOGUI_STACKNUM_WIDTH				32
#define DBOGUI_STACKNUM_HEIGHT				32
#define DBOGUI_STACKNUM_FONT				"detail"
#define DBOGUI_STACKNUM_FONTHEIGHT			75
#define DBOGUI_STACKNUM_FONTEFFECTMODE		TE_OUTLINE
#define DBOGUI_STACKNUM_FONTATTR			0
#define DBOGUI_STACKNUM_ALIGN				COMP_TEXT_LEFT | COMP_TEXT_DOWN

#define DBOGUI_SLOT_KEYNAME_FONT			"detail"
#define DBOGUI_SLOT_KEYNAME_FONTHEIGHT		75
#define DBOGUI_SLOT_KEYNAME_FONTEFFECTMODE	TE_OUTLINE
#define DBOGUI_SLOT_KEYNAME_FONTATTR		0
#define DBOGUI_SLOT_KEYNAME_ALIGN			COMP_TEXT_LEFT | COMP_TEXT_UP

#define DBOGUI_QUESTBALLOON_FONTHEIGHT		105

#define DBOGUI_ICON_SIZE					32
#define DBOGUI_ICON_SIZEDIFF				2

#define NTL_LINKED_DIALOG_GAP				5

#define dINPUTBOX_CARET_WIDTH				2
#define dINPUTBOX_CARET_HEIGHT				12


// Text Color
#define INFOCOLOR_0		RGB( 254, 254, 254 ) // white #fefefe
#define INFOCOLOR_1		RGB( 148, 191, 234 ) // blue #94bfea
#define INFOCOLOR_2		RGB(  38, 158, 255 ) // blue #269eff
#define INFOCOLOR_3		RGB( 172, 250,  80 ) // yellow/green #acfa50
#define INFOCOLOR_4		RGB( 176, 254,  90 ) // yellow/green #acfa50
#define INFOCOLOR_5		RGB( 243, 106, 124 ) // pink/red #f3677c
#define INFOCOLOR_6		RGB( 255, 168,  68 ) // orange #ffa844
#define INFOCOLOR_7		RGB( 255, 218,  75 ) // yellow #ffda4b
#define INFOCOLOR_8		RGB( 231, 121, 210 ) // purple #e779d2
#define INFOCOLOR_9		RGB( 180, 180, 180 ) // grey
#define INFOCOLOR_13	RGB( 167, 184, 193 ) // grey
#define INFOCOLOR_14	RGB( 35, 255, 60 ) // green #23ff3c

#define INFOCOLOR_LOBBY_FOC		RGB( 255, 255, 255 )
#define INFOCOLOR_LOBBY_DOWN	RGB( 193, 193, 193 )

#define RANKBATTLE_COLOR_YELLOW	RGB( 255, 254, 0 )
#define RANKBATTLE_COLOR_RED	RGB( 255, 0, 0 )
#define RANKBATTLE_COLOR_WHITE	RGB( 254, 254, 254 )
#define RANKBATTLE_COLOR_GRAY	RGB( 127, 127, 127 )
#define RANKBATTLE_COLOR_GREEN	RGB( 131, 255, 143 )

// button defalut color
#define NTL_BUTTON_UP_COLOR				RGB(255, 255, 255)
#define NTL_BUTTON_DOWN_COLOR			RGB(255, 210, 29)
#define NTL_BUTTON_FOCUS_COLOR			RGB(255, 255, 255)
#define NTL_BUTTON_DISABLE_COLOR		RGB(180, 180, 180)

// Party button color
#define NTL_PARTY_BUTTON_UP_COLOR			RGB(255, 210, 29)
#define NTL_PARTY_BUTTON_DOWN_COLOR			RGB(255, 210, 29)
#define NTL_PARTY_BUTTON_FOCUS_COLOR		RGB(255, 255, 255)
#define NTL_PARTY_BUTTON_DISABLE_COLOR		RGB(180, 180, 180)

// Team Color
#define NTL_RED_TEAM_COLOR		RGB( 255, 0, 0 )
#define NTL_BLUE_TEAM_COLOR		RGB( 0, 135, 255 )

#define NTL_BUTTON_CLICK_DIFFX			2 
#define NTL_BUTTON_CLICK_DIFFY			3 

// RPGauge 
#define RPGAUGE_TIME_SERVER		1.0f
#define RPGAUGE_TIME_EPSILON	0.0f

// LP Warning ramp rate
#define dLPWARNING_RATE			0.3f

// RP Bonus Apply KeyTime
#define SKILL_RPBONUS_KEYTIME	0.5f

// Item Duration warning
#define ITEM_DURATION_WARNING	0.3f

// Dialog를 클릭했을 시 해당에 Link된 Dialog와 자신을 최상단에 그리기 위한 매크로
#define CAPTURE_MOUSEDOWN_RAISE(eDialog, PosX, PosY)	\
	if( m_pThis->GetParent()->GetChildComponentReverseAt((RwInt32)PosX, (RwInt32)PosY) != m_pThis ) \
	return; \
	\
	CRectangle rect = m_pThis->GetScreenRect(); \
	\
	if( rect.PtInRect((RwInt32)PosX, (RwInt32)PosY)) \
	{ \
		RaiseLinked(); \
		GetDialogManager()->RaiseDialogbyUser(eDialog); \
	}


// Dialog를 클릭했을 시 해당 Dialog를 Link하고 있는 Dialog와 그 Dialog에 Link된 Dialog들을
// 최상단에 그리기 위한 매크로
#define CAPTURE_MOUSEDOWN_RAISE_TOP_LINKED(TopLinkedDialog, PosX, PosY)	\
	if( m_pThis->GetParent()->GetChildComponentReverseAt((RwInt32)PosX, (RwInt32)PosY) != m_pThis ) \
		return; \
	\
	CRectangle rect = m_pThis->GetScreenRect(); \
	\
	if( rect.PtInRect((RwInt32)PosX, (RwInt32)PosY)) \
		GetDialogManager()->GetDialog(TopLinkedDialog)->RaiseLinked();


// MouseWheelMove Macro
#define CAPTURE_MOUSEWHEELMOVE_SCROLLBAR( bShow, pDialog, pComponent, pScrollBar, sDelta, pos )\
	if( !bShow ) return;\
	if( pDialog->GetParent()->GetChildComponentReverseAt( pos.x - pDialog->GetParent()->GetScreenRect().left, pos.y - pDialog->GetParent()->GetScreenRect().top ) != pDialog ) return;\
	if( pComponent->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT ) return;\
	\
	RwInt32 nValue = pScrollBar->GetValue();\
	RwInt32 nMaxValue = pScrollBar->GetMaxValue();\
	RwInt32 nDelta = nValue - sDelta / GUI_MOUSE_WHEEL_DELTA;\
	\
	if( nDelta < 0 ) nDelta = 0;\
	else if( nDelta > nMaxValue ) nDelta = nMaxValue;\
	pScrollBar->SetValue( nDelta );