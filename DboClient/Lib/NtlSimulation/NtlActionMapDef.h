 /*****************************************************************************
 *
 * File			: NtlActionMapDef.h
 * Author		: Haesung, Cho
 * Copyright	: (주)NTL
 * Date			: 2008. 1. 15	
 * Abstract		: Simulation ActionMap definition
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_ACTIONMAP_DEF_H__
#define __NTL_ACTIONMAP_DEF_H__

///////////////////////////////////////////////////////////////////////////////
// NTL Key Definition
///////////////////////////////////////////////////////////////////////////////
#define NTL_KEY_BACKSPACE	   0x08
#define NTL_KEY_TAB			   0x09
#define NTL_KEY_RETURN		   0x0D
#define NTL_KEY_NUMPAD0        0x60
#define NTL_KEY_NUMPAD1        0x61
#define NTL_KEY_NUMPAD2        0x62
#define NTL_KEY_NUMPAD3        0x63
#define NTL_KEY_NUMPAD4        0x64
#define NTL_KEY_NUMPAD5        0x65
#define NTL_KEY_NUMPAD6        0x66
#define NTL_KEY_NUMPAD7        0x67
#define NTL_KEY_NUMPAD8        0x68
#define NTL_KEY_NUMPAD9        0x69
#define NTL_KEY_MULTIPLY       0x6A
#define NTL_KEY_ADD            0x6B
#define NTL_KEY_SEPARATOR      0x6C
#define NTL_KEY_SUBTRACT       0x6D
#define NTL_KEY_DECIMAL        0x6E
#define NTL_KEY_DIVIDE         0x6F
#define NTL_KEY_F1             0x70
#define NTL_KEY_F2             0x71
#define NTL_KEY_F3             0x72
#define NTL_KEY_F4             0x73
#define NTL_KEY_F5             0x74
#define NTL_KEY_F6             0x75
#define NTL_KEY_F7             0x76
#define NTL_KEY_F8             0x77
#define NTL_KEY_F9             0x78
#define NTL_KEY_F10            0x79
#define NTL_KEY_F11            0x7A
#define NTL_KEY_F12            0x7B

#define NTL_KEY_SHIFT          0x10
#define NTL_KEY_CONTROL        0x11
#define NTL_KEY_MENU           0x12
#define NTL_KEY_PAUSE          0x13
#define NTL_KEY_CAPITAL        0x14
#define NTL_KEY_TOCHINESE	   0x19
#define NTL_KEY_ESCAPE         0x1B
#define NTL_KEY_CONVERT        0x1C
#define NTL_KEY_NONCONVERT     0x1D
#define NTL_KEY_ACCEPT         0x1E
#define NTL_KEY_MODECHANGE     0x1F
#define NTL_KEY_SPACE          0x20
#define NTL_KEY_PRIOR          0x21
#define NTL_KEY_NEXT           0x22
#define NTL_KEY_END            0x23
#define NTL_KEY_HOME           0x24
#define NTL_KEY_LEFT           0x25
#define NTL_KEY_UP             0x26
#define NTL_KEY_RIGHT          0x27
#define NTL_KEY_DOWN           0x28
#define NTL_KEY_SELECT         0x29
#define NTL_KEY_PRINT          0x2A
#define NTL_KEY_EXECUTE        0x2B
#define NTL_KEY_SNAPSHOT       0x2C
#define NTL_KEY_INSERT         0x2D
#define NTL_KEY_DELETE         0x2E
#define NTL_KEY_HELP           0x2F
#define NTL_KEY_LEFTWINDOW	   0x5b
#define NTL_KEY_RIGHTWINDOW    0x5c
#define NTL_KEY_NUMLOCK        0x90
#define NTL_KEY_SCROLL         0x91
#define NTL_KEY_COMMA		   0xBC		// ','
#define NTL_KEY_DASH		   0xBD
#define NTL_KEY_PERIOD		   0xBE		// '.'
#define NTL_KEY_OEM_2		   0xBF		// '/'
#define NTL_KEY_EQUAL		   0xBB
#define NTL_KEY_WAVE		   0xC0		// '~'
#define NTL_KEY_WON			   0xDC		// '\'
#define NTL_KEY_LBRACKET		0xDB	// '['
#define NTL_KEY_RBRACKET		0xDD	// ']'

///////////////////////////////////////////////////////////////////////////////
// DragonBall Online Action
///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
// ActionMap Key 상태
///////////////////////////////////////////////////////////////////////////////

#define NTL_ALT_BIT   0x0100
#define NTL_CTRL_BIT  0x0200			// 조합키
#define NTL_SHIFT_BIT 0x0400

#define NTL_KEY_UPDATE 0x4000
#define NTL_KEY_EMPTY 0x8000

///////////////////////////////////////////////////////////////////////////////
// ETC
///////////////////////////////////////////////////////////////////////////////

enum eACTION
{
	// 0 ~ 19 까지 아바타 행동
	ACTION_AVATAR_FORWARD = 0,		///< 전진
	ACTION_AVATAR_BACKWARD,			///< 후진
	ACTION_AVATAR_LEFTTURN,			///< 좌회전
	ACTION_AVATAR_RIGHTTURN,		///< 우회전
	ACTION_AVATAR_SITDOWN,			///< 앉기
	ACTION_AVATAR_JUMP,				///< 점프
	ACTION_AVATAR_AUTORUN,			///< 자동달리기
	ACTION_AVATAR_SCOUTER,			///< 스카우터 사용하기
	ACTION_AVATAR_CHARGE,			///< 기 모으기
	ACTION_AVATAR_LOOTING,			///< 아이템 줍기
	ACTION_AVATAR_BLOCKING,			///< 아바타 블록킹
	ACTION_AVATAR_LEFTSIDE,
	ACTION_AVATAR_RIGHTSIDE,
	ACTION_AVATAR_FREEDOWN,

	// 20 ~ 39 까지 타겟 행동
	ACTION_TARGET_SELF = 20,		///< 자신 선택
	ACTION_TARGET_1STPARTY,			///< 파티원1 선택
	ACTION_TARGET_2NDPARTY,			///< 파티원2 선택
	ACTION_TARGET_3RDPARTY,			///< 파티원3 선택
	ACTION_TARGET_4THPARTY,			///< 파티원4 선택
	ACTION_TARGET_MEMBER,
	ACTION_TARGET_AUTO,				///< 가까운 적 선택
	ACTION_TARGET_AUTOATK,			///< 자동공격
	ACTION_TARGET_AUTOFOLLOW,		///< 자동 따라가기
	ACTION_TARGET_MARKING_1,		///< 공유 타겟1 지정
	ACTION_TARGET_MARKING_2,		///< 공유 타겟2 지정
	ACTION_TARGET_MARKING_3,		///< 공유 타겟3 지정
	ACTION_TARGET_MARKING_4,		///< 공유 타겟4 지정
	ACTION_TARGET_MARKING_5,		///< 공유 타겟5 지정
	ACTION_TARGET_SELECTING_1,		///< 공유 타겟1 선택
	ACTION_TARGET_SELECTING_2,		///< 공유 타겟2 선택
	ACTION_TARGET_SELECTING_3,		///< 공유 타겟3 선택
	ACTION_TARGET_SELECTING_4,		///< 공유 타겟4 선택
	ACTION_TARGET_SELECTING_5,		///< 공유 타겟5 선택
	ACTION_TARGET_AUTOTARGET_PARTY,	///< 파티원 자동 선택

	// 40 ~ 59 까지 퀵 행동
	ACTION_QUICK_1 = 40,			///< 퀵슬롯1번
	ACTION_QUICK_2,					///< 퀵슬롯2번
	ACTION_QUICK_3,					///< 퀵슬롯3번
	ACTION_QUICK_4,					///< 퀵슬롯4번
	ACTION_QUICK_5,					///< 퀵슬롯5번
	ACTION_QUICK_6,					///< 퀵슬롯6번
	ACTION_QUICK_7,					///< 퀵슬롯7번
	ACTION_QUICK_8,					///< 퀵슬롯8번
	ACTION_QUICK_9,					///< 퀵슬롯9번
	ACTION_QUICK_0,					///< 퀵슬롯0번
	ACTION_QUICK_MIN,				///< 퀵슬롯-번
	ACTION_QUICK_PLUS,				///< 퀵슬롯+번
	ACTION_QUICK_PREV,				///< 이전 퀵슬롯
	ACTION_QUICK_NEXT,				///< 다음 퀵슬롯
	ACTION_QUICK_2NDCAP,			///< 2번 캡슐키트
	ACTION_QUICK_3RDCAP,			///< 3번 캡슐키트
	ACTION_QUICK_4THCAP,			///< 4번 캡슐키트
	ACTION_QUICK_5THCAP,			///< 5번 캡슐키트

	// 60 ~ 79 까지 윈도우 행동
	ACTION_WINDOW_PROFILE = 60,			///< 상세정보 윈도우
	ACTION_WINDOW_SKILL,			///< 스킬정보 윈도우
	ACTION_WINDOW_QUEST,			///< 퀘스트 윈도우
	ACTION_WINDOW_MINIMAP,			///< 미니맵
	ACTION_WINDOW_WORLDMAP,			///< 월드맵
	ACTION_WINDOW_MAINCAP,			///< 메인 캡슐키트
	ACTION_WINDOW_PARTY,			///< 파티정보					///< 기획 단계에서 사라짐
	ACTION_WINDOW_GUILD,			///< 길드정보
	ACTION_WINDOW_FRIEND,			///< 친구정보
	ACTION_WINDOW_OPTION,			///< 옵션
	ACTION_WINDOW_HELP,				///< 도움말
	ACTION_WINDOW_MAIN,				///< 메인메뉴
	ACTION_WINDOW_RANKBOARD,		///< 랭크보드
	ACTION_WINDOW_HOIPOIMIX,
	ACTION_WINDOW_MASCOT,
	ACTION_WINDOW_DWC,

	// 80 ~ 159 까지 빈자리 예약
	ACTION_QUICK_1_EX = 80,				///< 퀵슬롯 첫번째 확장 1번
	ACTION_QUICK_2_EX,					///< 퀵슬롯2번
	ACTION_QUICK_3_EX,					///< 퀵슬롯3번
	ACTION_QUICK_4_EX,					///< 퀵슬롯4번
	ACTION_QUICK_5_EX,					///< 퀵슬롯5번
	ACTION_QUICK_6_EX,					///< 퀵슬롯6번
	ACTION_QUICK_7_EX,					///< 퀵슬롯7번
	ACTION_QUICK_8_EX,					///< 퀵슬롯8번
	ACTION_QUICK_9_EX,					///< 퀵슬롯9번
	ACTION_QUICK_0_EX,					///< 퀵슬롯0번
	ACTION_QUICK_MIN_EX,				///< 퀵슬롯-번
	ACTION_QUICK_PLUS_EX,				///< 퀵슬롯+번
	ACTION_QUICK_1_EX2,					///< 퀵슬롯 첫번째 확장 1번
	ACTION_QUICK_2_EX2,					///< 퀵슬롯2번
	ACTION_QUICK_3_EX2,					///< 퀵슬롯3번
	ACTION_QUICK_4_EX2,					///< 퀵슬롯4번
	ACTION_QUICK_5_EX2,					///< 퀵슬롯5번
	ACTION_QUICK_6_EX2,					///< 퀵슬롯6번
	ACTION_QUICK_7_EX2,					///< 퀵슬롯7번
	ACTION_QUICK_8_EX2,					///< 퀵슬롯8번
	ACTION_QUICK_9_EX2,					///< 퀵슬롯9번
	ACTION_QUICK_0_EX2,					///< 퀵슬롯0번
	ACTION_QUICK_MIN_EX2,				///< 퀵슬롯-번
	ACTION_QUICK_PLUS_EX2,				///< 퀵슬롯+번
	ACTION_QUICK_SUMMON,
	ACTION_QUICK_SUMMON2,

	// 160 ~ 169 까지 미니맵 관련
	ACTION_MINIMAP_ZOOMOUT = 160,			///< 미니맵 축소
	ACTION_MINIMAP_ZOOMIN,					///< 미니맵 확대

	// 170 ~ 179 까지 채팅창 관련
	ACTION_CHAT_PGUP = 170,				///< 채팅창 이전 페이지
	ACTION_CHAT_PGDN,					///< 채팅창 다음 페이지

	// 180 ~ 199 까지 전체 명령 관련
	ACTION_GLOBAL_CANCLE = 180,			///< 취소(Targeting, Windows, Main Menu)
	ACTION_GLOBAL_SNAPSHOT,				///< 스크린샷
	ACTION_GLOBAL_CHAT,					///< 채팅입력
	ACTION_GLOBAL_RENDERGUI,

	ACTION_NUMS = 255,
	ACTION_INVALID = ACTION_NUMS, 				///< 액션 ID의 끝번호
};

#define dNTL_KEY_EMPTY		0x8000		// 정의된 키가 없다.
#define dNTL_KEY_INVALID	0xffff		// 키 값이 INVALID.

/**
* \breif NTL의 키 타입
* NTL Define:
* 0 0 0 0 0 0 0 0    00000000
* E           C A    Key Data
* 상위 8비트 : C = CTRL, A = ALT
* 하위 8비트 : Key Data
*/
typedef union _uNtlKey
{
	unsigned short key;			// 16Bit

	/**
	* key를 상위 비트와 하위 비트로 나뉘어서 사용하는 공용 구조체
	*/
	struct _BIT
	{
		unsigned short ori:8;	// 오리지널 Key에 사용
		unsigned short bit8:1;	// 조합키로 사용되는 Bit flags
		unsigned short bit7:1;
		unsigned short bit6:1;
		unsigned short bit5:1;
		unsigned short bit4:1;
		unsigned short bit3:1;
		unsigned short bit2:1;
		unsigned short bit1:1;
	} BIT;
} uNtlKey;

/**
* \brief ACTIONMAP의 Action과 Key를 저장하는 데이타형
* KEY		: unsigned short
* DATA		: unsigned short 
*/
typedef std::map< unsigned short, unsigned short > ACTIONMAP;

/**
* \brief 액션을 저장하고 있기 위한 벡터
*/
typedef std::vector< unsigned short > ACTIONVEC;

/**
* \brief 액션맵을 Setting 하는데의 Result
*/
enum eACTIONMAP_SET_RESULT
{
	SET_RESULT_FAILED = 0,					// 알수 없는 에러
	SET_RESULT_FIXED_KEY,					// 키가 고정되어 있어서 변경하지 못한다.
	SET_RESULT_FIXED_ACTION,				// 액션이 변경하지 못하는 액션이다.
	SET_RESULT_NOT_COMBINE_ACTION,			// 조합키를 사용하지 못하는 액션이다.
	SET_RESULT_ALREADY_KEY,					// Key가 이미 존재함		(기존에 있던 Key는 잃어버림)
	SET_RESULT_ALREADY_ACTION,				// Action이 이미 존재함		(기존에 있던 Action은 잃어버림)
	SET_RESULT_SAME_KEY,					// 같은 Key가 세팅되어 있었음
	SET_RESULT_NEGATIVE_MODE,				// 불가능한 모드
	SET_RESULT_OK,							// 정상적으로 성공
	SET_RESULT_NUMS,						// RESULT 갯수
	SET_RESULT_INVALID = SET_RESULT_NUMS
};

/**
* \brief 액션맵의 에러 코드를 담고 있는 벡터
*/
typedef std::vector< BYTE > RESULTVEC;

/**
* \brief 액션맵의 속성 ( KEY와 ACTION 둘다 가능 )
* 추가될 수 있다.
*/
// #define TYPE_NONE			0x00000000		// 아무 속성이 없다.
#define TYPE_FIXED			0x00000001		// 고정된 타입, 변경하지 못한다.
#define TYPE_COMBINE		0x00000002		// 조합키로 사용되는지의 여부
#define TYPE_EXCLUSIVE		0x00000004		// 조합키로는 사용하지 못하는 액션

/**
* \brief 키나 액션의 속성을 저장하고 있는 맵
*/
typedef std::map< unsigned short, int >		TYPEMAP;

/**
* \brief 키의 이름을 보관하고 있는 맵
*/
typedef std::map<unsigned short, std::wstring> KEYNAMEMAP;

/**
* \brief ACTIONMAP을 사용하는 모드를 열거한다.
*/
enum eACTIONMAP_MODE
{
	ACTIONMAP_MODE_USERSETTING = 0,
	ACTIONMAP_MODE_DEFAULT,

	ACTIONMAP_MODE_NUMS,
	ACTIONMAP_MODE_INVALID = ACTIONMAP_MODE_NUMS
};

// 키가 무제한으로 무시되어야 할 때 사용되는 Count Define
#define ACTIONMAP_IGNORE_KEY_UNLIMITED_IGNORE	0xFF

#endif//__NTL_ACTIONMAP_DEF_H__