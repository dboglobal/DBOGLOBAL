#ifndef __NTL_STORAGE_DEFINE_H__
#define __NTL_STORAGE_DEFINE_H__

// 적용 하는 Flags
enum eNTL_STORAGE_APPLY
{
	eNTL_STORAGE_APPLY_PRESENTATION				= 0x00000001,
	eNTL_STORAGE_APPLY_SIMULATION				= 0x00000010,
	eNTL_STORAGE_APPLY_CLIENT					= 0x00000100,
	eNTL_STORAGE_APPLY_PRESENTATION_INITONCE	= 0x00001000,
	eNTL_STORAGE_APPLY_ALL						= 0x11110111
};

/**
* \brief NTL_STORAGE UNIT들의 GROUP TYPE( 하나의 File 과 1:1 매칭된다. )
*/
enum eNTL_STORAGE_GROUP_TYPE
{
	eNTL_STORAGE_GROUP_ACCOUNT,		///< 계정 정보
	eNTL_STORAGE_GROUP_SYSTEM,		///< 시스템
	eNTL_STORAGE_GROUP_GAMEINFO,	///< 게임 정보
	eNTL_STORAGE_GROUP_SCOUTER,		///< 캐릭터 스카우터 정보
	eNTL_STORAGE_GROUP_QUEST,		///< 퀘스트 인디케이터 정보
	eNTL_STORAGE_GROUP_CHARACTER,	///< 캐릭터 게임 정보

	eNTL_STORAGE_GROUP_NUMS,
	eNTL_STORAGE_GROUP_INVALID = 0xFF
};

/**
* \brief NtlStorageUnit 들의 Type
*/
enum eNTL_STORAGE_TYPE
{
	eNTL_STORAGE_ACCOUNT,			///< 계정
	eNTL_STORAGE_GRAPHIC,			///< 그래픽
	eNTL_STORAGE_SOUND,				///< 소리
	eNTL_STORAGE_GAMEINFO,			///< 게임 정보
	eNTL_STORAGE_ETC,				///< 기타
	eNTL_STORAGE_CHAT,				///< 채팅
	eNTL_STORAGE_SCOUTER,			///< 몬스터 스카우터 및 캐릭터 스카우터 정보(전투력 측정)
	eNTL_STORAGE_QUEST,				///< 퀘스트
	eNTL_STORAGE_CHARACTER,			///< 캐릭터별 세팅
	eNTL_STORAGE_CHARACTER_ETC,
	eNTL_STORAGE_CHARACTER_SCOUTER,
	eNTL_STORAGE_SYSTEM_ETC,

	eNTL_STORAGE_NUMS,
	eNTL_STORAGE_INVALID = 0xFF
};

// eNTL_STORAGE_TYPE과 일치하여야 함
extern const char* g_acNtlStorageTypeString[eNTL_STORAGE_NUMS];

/**
* \brief NtlStorageValue가 가지는 Data Type들
*/
enum eNTL_STORAGE_VARIABLE_TYPE
{
	eINT,			// INT 형
	eFLOAT,			// FLOAT 형
	eBOOL,			// bool 형
	eSTRING,		// 문자열 형
	eUSER,			// 사용자 정의 형

	eNTL_STORAGE_VARIABLE_NUMS,
	eNTL_STORAGE_VARIABLE_INVALID = 0xFF
};

// Check 함수 포인터 ( 키와 연결된 값의 범위체크에 사용된다. )
typedef bool (*NtlStorageUnitCheckFunc)(unsigned int uiKey);

// Apply 함수 포인터 ( 유닛과 연결되어 유닛 단위로 Apply 한다. )
class CNtlStorageUnit;
typedef bool (*NtlStorageUnitApplyFunc)(CNtlStorageUnit* pUnit, unsigned int uiFlags);

// NTL_STORAGE 키 네임의 길이
#define dNTL_STORAGE_MAX_KEY_NAME_LENGTH	(32+1)

// NTL_STORAGE VALUE 의 길이
#define dNTL_STORAGE_MAX_VALUE_LENGTH		(128+1)

// 매핑 테이블
struct SNtlStorageMappingTableData
{
	unsigned int		uiKey;
	unsigned char		byVarType;
	unsigned char		byCategoryType;
	char				acKeyName[dNTL_STORAGE_MAX_KEY_NAME_LENGTH];
	
	// 이하부터만 변경 가능
	NtlStorageUnitCheckFunc		ntlStorageFunc;		///< Check 함수 포인터
	char						acDefaultValue[dNTL_STORAGE_MAX_VALUE_LENGTH];
};

// NtlStorage Bool Define
#define dSTORAGE_TRUE		"true"
#define dSTORAGE_FALSE		"false"

// 스카우터 측정 정보
typedef struct _SMemoryScouterData
{
	_SMemoryScouterData::_SMemoryScouterData(VOID)
	{
		uiPower = 0;
	}

	unsigned int uiPower;				// 전투력

}SMemoryScouterData;

// 퀘스트 정보
typedef struct _SMemoryQuestData
{
	_SMemoryQuestData::_SMemoryQuestData(VOID)
	{
		bIsNew = FALSE;
		bIsIndicated = FALSE;
	}

	RwBool	bIsNew;
	RwBool	bIsIndicated;
}SMemoryQuestData;

// Key ID Defines
// Rule: The KEY ID is incremented in sequence or greater than the last Defined ID.
// Do not add inserts in the middle of an absolute deletion. DEFINE, which is not used, is just a comment.
//----------------------------------------------------------------------------------------

#define dSTORAGE_CATEGORY						0

// Account
#define dSTORAGE_ACCOUNT_LASTCHAR				1000		// 마지막으로 플레이 한 캐릭터 이름

// Graphic
#define dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH		2000		// 해상도 WIDTH
#define dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT		2001
#define dSTORAGE_GRAPHIC_SELECT_VIDEODEPTH		2002		// 색상
#define dSTORAGE_GRAPHIC_GAMMA					2003		// 밝기
#define dSTORAGE_GRAPHIC_WINDOW_MODE			2004		// 창모드
#define dSTORAGE_GRAPHIC_PRESET					2005		// 프리셋 레벨 0~3, 4 = 사용자 설정
#define dSTORAGE_GRAPHIC_TERRAIN_RANGE			2006		// 지형 시야거리
#define dSTORAGE_GRAPHIC_OBJECT_RANGE			2007		// 사물 시야거리
#define dSTORAGE_GRAPHIC_TERRAIN_SHADOW			2010		// 지형 그림자
#define dSTORAGE_GRAPHIC_WATER_EFFECT			2011		// 물효과
#define dSTORAGE_GRAPHIC_CHARACTER_RANGE		2012		// 캐릭터 시야거리
#define dSTORAGE_GRAPHIC_CHARACTER_EFFECT		2013		// 캐릭터 업그레이드 효과 
#define dSTORAGE_GRAPHIC_CHARACTER_SHADOW		2014		// 캐릭터 그림자
#define dSTORAGE_GRAPHIC_MAGIC_EFFECT			2015		// 마법 세부효과
#define dSTORAGE_GRAPHIC_WEATHER_EFFECT			2016		// 날시 효과
#define dSTORAGE_GRAPHIC_TEXTURE_LEVEL			2017		// 텍스쳐 해상도
#define dSTORAGE_GRAPHIC_SHADER_HDR				2018		// HDR 효과
#define dSTORAGE_GRAPHIC_CHARACTER_EDGE			2019		// 카툰 외곽선
#define dSTORAGE_GRAPHIC_SHOW_FPS				2020
#define dSTORAGE_GRAPHIC_FPS					2021

// Sound
#define dSTORAGE_SOUND_MAIN_VOLUME				4000	// 메인 볼륨
#define dSTORAGE_SOUND_BACK_VOLUME				4001	// 배경음 볼륨
#define dSTORAGE_SOUND_EFFECT_VOLUME			4002	// 효과음 볼륨
#define dSTORAGE_SOUND_ENV_VOLUME				4003	// 환경음 볼륨

// GameInfo
#define dSTORAGE_GAMEINFO_GUILD_EMBLEM			5000	// 유파 엠블렘
#define dSTORAGE_GAMEINFO_DOJO_MARK				5001	// 도장 마크
#define dSTORAGE_GAMEINFO_SCOUTER_VISIBLE		5002	// 스카우터 표시

// Etc
#define dSTORAGE_ETC_FRIEND_NOTIFY				6000	// 친구 접속 알림
#define dSTORAGE_ETC_GUILD_MEMBER_NOTIFY		6001	// 문하생 접속 알림
#define dSTORAGE_ETC_QUESTTEXT_EFFECT			6002	// 퀘스트 텍스트 효과
#define dSTORAGE_ETC_USER_TRADE					6003	// 거래
#define dSTORAGE_ETC_MOVEABLEWINDOW				6004	// 윈도우 이동 가능 여부
#define dSTORAGE_ETC_MOUSE_DASH					6005	// 마우스 대시 간격
#define dSTORAGE_ETC_MOUSE_FOV					6006
#define dSTORAGE_ETC_MOUSE_VIEW_POINT			6007

// Chatting
#define dSTORAGE_CHAT_INPUTMODE					7000	// 채팅창 입력모드
#define dSTORAGE_CHAT_WINDOW_LOCK				7001	// 채팅창 잠금
#define dSTORAGE_CHAT_MOUSE_SCROLL				7002	// 마우스 스크롤
#define dSTORAGE_CHAT_EXTEND_WINDOW				7003	// 확장 채팅창
#define dSTORAGE_CHAT_WINDOW_OPACITY			7004	// 윈도우 투명도
#define dSTORAGE_CHAT_BASIC_NORMAL				7005	// 기본 윈도우 : 일반대화
#define dSTORAGE_CHAT_BASIC_WISPHER				7006	// 기본 윈도우 : 귓속말
#define dSTORAGE_CHAT_BASIC_PARTY				7007	// 기본 윈도우 : 파티
#define dSTORAGE_CHAT_BASIC_GUILD				7008	// 기본 윈도우 : 유파
#define dSTORAGE_CHAT_BASIC_TRADE				7009	// 기본 윈도우 : 거래
#define dSTORAGE_CHAT_BASIC_SHOUT				7010	// 기본 윈도우 : 외치기
#define dSTORAGE_CHAT_BASIC_SYSTEM				7011	// 기본 윈도우 : 시스템
#define dSTORAGE_CHAT_EXTEND_NORMAL				7012	// 확장 윈도우 : 일반대화
#define dSTORAGE_CHAT_EXTEND_WISPHER			7013	// 확장 윈도우 : 귓속말
#define dSTORAGE_CHAT_EXTEND_PARTY				7014	// 확장 윈도우 : 파티
#define dSTORAGE_CHAT_EXTEND_GUILD				7015	// 확장 윈도우 : 유파
#define dSTORAGE_CHAT_EXTEND_TRADE				7016	// 확장 윈도우 : 거래
#define dSTORAGE_CHAT_EXTEND_SHOUT				7017	// 확장 윈도우 : 외치기
#define dSTORAGE_CHAT_EXTEND_SYSTEM				7018	// 확장 윈도우 : 시스템
#define dSTORAGE_CHAT_BASIC_FIND_PARTY			7019
#define dSTORAGE_CHAT_EXTEND_FIND_PARTY			7020
#define dSTORAGE_CHAT_EXTEND2_NORMAL			7021
#define dSTORAGE_CHAT_EXTEND2_WISPHER			7022
#define dSTORAGE_CHAT_EXTEND2_PARTY				7023
#define dSTORAGE_CHAT_EXTEND2_GUILD				7024
#define dSTORAGE_CHAT_EXTEND2_TRADE				7025
#define dSTORAGE_CHAT_EXTEND2_SHOUT				7026
#define dSTORAGE_CHAT_EXTEND2_SYSTEM			7027
#define dSTORAGE_CHAT_EXTEND2_FIND_PARTY		7028

#define dSTORAGE_CHAT_EXTEND2_WINDOW			7029

// Scouter
#define dSTORAGE_SCOUTER_MONSTER_SCOUTER_MEMORY_INFO	6000
#define dSTORAGE_SCOUTER_PLAYER_SCOUTER_MEMORY_INFO		6001
#define dSTORAGE_SCOUTER_INFO_TBLID						6002
#define dSTORAGE_SCOUTER_INFO_POWER						6003

// Quest Indicator
#define	dSTORAGE_QUEST_MEMORY_INFO						7000
#define dSTORAGE_QUEST_INFO_ID							7001
#define dSTORAGE_QUEST_INFO_NEW							7002
#define dSTORAGE_QUEST_INFO_INDICATE					7003

// Character Info
#define dSTORAGE_ETC_EX_QUICKSLOT1						8000	// 추가 퀵슬롯 1
#define dSTORAGE_ETC_EX_QUICKSLOT2						8001	// 추가 퀵슬롯 2
#define dSTORAGE_ETC_QUICKSLOT_LOCK						8002	// 퀵슬롯 잠그기

#define dSTORAGE_CHARACTER_ETC_DISCORD_HIDE_NAME		9000	// show character name in discord 

#define dSTORAGE_CHARACTER_SCOUTER_ON					9010	// scouter status

#define dSTORAGE_SYSTEM_ETC_DISCORD						10000	// Enable/disable discorrd
#define dSTORAGE_SYSTEM_ETC_CONTRACT					10001	// contract/game policy

#define dSTORAGE_EOF									0xFFFFFFFF

#endif//__NTL_STORAGE_DEFINE_H__