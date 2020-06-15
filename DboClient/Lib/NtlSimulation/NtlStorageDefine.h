#ifndef __NTL_STORAGE_DEFINE_H__
#define __NTL_STORAGE_DEFINE_H__

// ���� �ϴ� Flags
enum eNTL_STORAGE_APPLY
{
	eNTL_STORAGE_APPLY_PRESENTATION				= 0x00000001,
	eNTL_STORAGE_APPLY_SIMULATION				= 0x00000010,
	eNTL_STORAGE_APPLY_CLIENT					= 0x00000100,
	eNTL_STORAGE_APPLY_PRESENTATION_INITONCE	= 0x00001000,
	eNTL_STORAGE_APPLY_ALL						= 0x11110111
};

/**
* \brief NTL_STORAGE UNIT���� GROUP TYPE( �ϳ��� File �� 1:1 ��Ī�ȴ�. )
*/
enum eNTL_STORAGE_GROUP_TYPE
{
	eNTL_STORAGE_GROUP_ACCOUNT,		///< ���� ����
	eNTL_STORAGE_GROUP_SYSTEM,		///< �ý���
	eNTL_STORAGE_GROUP_GAMEINFO,	///< ���� ����
	eNTL_STORAGE_GROUP_SCOUTER,		///< ĳ���� ��ī���� ����
	eNTL_STORAGE_GROUP_QUEST,		///< ����Ʈ �ε������� ����
	eNTL_STORAGE_GROUP_CHARACTER,	///< ĳ���� ���� ����

	eNTL_STORAGE_GROUP_NUMS,
	eNTL_STORAGE_GROUP_INVALID = 0xFF
};

/**
* \brief NtlStorageUnit ���� Type
*/
enum eNTL_STORAGE_TYPE
{
	eNTL_STORAGE_ACCOUNT,			///< ����
	eNTL_STORAGE_GRAPHIC,			///< �׷���
	eNTL_STORAGE_SOUND,				///< �Ҹ�
	eNTL_STORAGE_GAMEINFO,			///< ���� ����
	eNTL_STORAGE_ETC,				///< ��Ÿ
	eNTL_STORAGE_CHAT,				///< ä��
	eNTL_STORAGE_SCOUTER,			///< ���� ��ī���� �� ĳ���� ��ī���� ����(������ ����)
	eNTL_STORAGE_QUEST,				///< ����Ʈ
	eNTL_STORAGE_CHARACTER,			///< ĳ���ͺ� ����
	eNTL_STORAGE_CHARACTER_ETC,
	eNTL_STORAGE_CHARACTER_SCOUTER,
	eNTL_STORAGE_SYSTEM_ETC,

	eNTL_STORAGE_NUMS,
	eNTL_STORAGE_INVALID = 0xFF
};

// eNTL_STORAGE_TYPE�� ��ġ�Ͽ��� ��
extern const char* g_acNtlStorageTypeString[eNTL_STORAGE_NUMS];

/**
* \brief NtlStorageValue�� ������ Data Type��
*/
enum eNTL_STORAGE_VARIABLE_TYPE
{
	eINT,			// INT ��
	eFLOAT,			// FLOAT ��
	eBOOL,			// bool ��
	eSTRING,		// ���ڿ� ��
	eUSER,			// ����� ���� ��

	eNTL_STORAGE_VARIABLE_NUMS,
	eNTL_STORAGE_VARIABLE_INVALID = 0xFF
};

// Check �Լ� ������ ( Ű�� ����� ���� ����üũ�� ���ȴ�. )
typedef bool (*NtlStorageUnitCheckFunc)(unsigned int uiKey);

// Apply �Լ� ������ ( ���ְ� ����Ǿ� ���� ������ Apply �Ѵ�. )
class CNtlStorageUnit;
typedef bool (*NtlStorageUnitApplyFunc)(CNtlStorageUnit* pUnit, unsigned int uiFlags);

// NTL_STORAGE Ű ������ ����
#define dNTL_STORAGE_MAX_KEY_NAME_LENGTH	(32+1)

// NTL_STORAGE VALUE �� ����
#define dNTL_STORAGE_MAX_VALUE_LENGTH		(128+1)

// ���� ���̺�
struct SNtlStorageMappingTableData
{
	unsigned int		uiKey;
	unsigned char		byVarType;
	unsigned char		byCategoryType;
	char				acKeyName[dNTL_STORAGE_MAX_KEY_NAME_LENGTH];
	
	// ���Ϻ��͸� ���� ����
	NtlStorageUnitCheckFunc		ntlStorageFunc;		///< Check �Լ� ������
	char						acDefaultValue[dNTL_STORAGE_MAX_VALUE_LENGTH];
};

// NtlStorage Bool Define
#define dSTORAGE_TRUE		"true"
#define dSTORAGE_FALSE		"false"

// ��ī���� ���� ����
typedef struct _SMemoryScouterData
{
	_SMemoryScouterData::_SMemoryScouterData(VOID)
	{
		uiPower = 0;
	}

	unsigned int uiPower;				// ������

}SMemoryScouterData;

// ����Ʈ ����
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
#define dSTORAGE_ACCOUNT_LASTCHAR				1000		// ���������� �÷��� �� ĳ���� �̸�

// Graphic
#define dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH		2000		// �ػ� WIDTH
#define dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT		2001
#define dSTORAGE_GRAPHIC_SELECT_VIDEODEPTH		2002		// ����
#define dSTORAGE_GRAPHIC_GAMMA					2003		// ���
#define dSTORAGE_GRAPHIC_WINDOW_MODE			2004		// â���
#define dSTORAGE_GRAPHIC_PRESET					2005		// ������ ���� 0~3, 4 = ����� ����
#define dSTORAGE_GRAPHIC_TERRAIN_RANGE			2006		// ���� �þ߰Ÿ�
#define dSTORAGE_GRAPHIC_OBJECT_RANGE			2007		// �繰 �þ߰Ÿ�
#define dSTORAGE_GRAPHIC_TERRAIN_SHADOW			2010		// ���� �׸���
#define dSTORAGE_GRAPHIC_WATER_EFFECT			2011		// ��ȿ��
#define dSTORAGE_GRAPHIC_CHARACTER_RANGE		2012		// ĳ���� �þ߰Ÿ�
#define dSTORAGE_GRAPHIC_CHARACTER_EFFECT		2013		// ĳ���� ���׷��̵� ȿ�� 
#define dSTORAGE_GRAPHIC_CHARACTER_SHADOW		2014		// ĳ���� �׸���
#define dSTORAGE_GRAPHIC_MAGIC_EFFECT			2015		// ���� ����ȿ��
#define dSTORAGE_GRAPHIC_WEATHER_EFFECT			2016		// ���� ȿ��
#define dSTORAGE_GRAPHIC_TEXTURE_LEVEL			2017		// �ؽ��� �ػ�
#define dSTORAGE_GRAPHIC_SHADER_HDR				2018		// HDR ȿ��
#define dSTORAGE_GRAPHIC_CHARACTER_EDGE			2019		// ī�� �ܰ���
#define dSTORAGE_GRAPHIC_SHOW_FPS				2020
#define dSTORAGE_GRAPHIC_FPS					2021

// Sound
#define dSTORAGE_SOUND_MAIN_VOLUME				4000	// ���� ����
#define dSTORAGE_SOUND_BACK_VOLUME				4001	// ����� ����
#define dSTORAGE_SOUND_EFFECT_VOLUME			4002	// ȿ���� ����
#define dSTORAGE_SOUND_ENV_VOLUME				4003	// ȯ���� ����

// GameInfo
#define dSTORAGE_GAMEINFO_GUILD_EMBLEM			5000	// ���� ������
#define dSTORAGE_GAMEINFO_DOJO_MARK				5001	// ���� ��ũ
#define dSTORAGE_GAMEINFO_SCOUTER_VISIBLE		5002	// ��ī���� ǥ��
#define dSTORAGE_GAMEINFO_ITEM_DROP				5003	// 스카우터 표시

// Etc
#define dSTORAGE_ETC_FRIEND_NOTIFY				6000	// ģ�� ���� �˸�
#define dSTORAGE_ETC_GUILD_MEMBER_NOTIFY		6001	// ���ϻ� ���� �˸�
#define dSTORAGE_ETC_QUESTTEXT_EFFECT			6002	// ����Ʈ �ؽ�Ʈ ȿ��
#define dSTORAGE_ETC_USER_TRADE					6003	// �ŷ�
#define dSTORAGE_ETC_MOVEABLEWINDOW				6004	// ������ �̵� ���� ����
#define dSTORAGE_ETC_MOUSE_DASH					6005	// ���콺 ��� ����
#define dSTORAGE_ETC_MOUSE_FOV					6006
#define dSTORAGE_ETC_MOUSE_VIEW_POINT			6007

// Chatting
#define dSTORAGE_CHAT_INPUTMODE					7000	// ä��â �Է¸��
#define dSTORAGE_CHAT_WINDOW_LOCK				7001	// ä��â ���
#define dSTORAGE_CHAT_MOUSE_SCROLL				7002	// ���콺 ��ũ��
#define dSTORAGE_CHAT_EXTEND_WINDOW				7003	// Ȯ�� ä��â
#define dSTORAGE_CHAT_WINDOW_OPACITY			7004	// ������ ������
#define dSTORAGE_CHAT_BASIC_NORMAL				7005	// �⺻ ������ : �Ϲݴ�ȭ
#define dSTORAGE_CHAT_BASIC_WISPHER				7006	// �⺻ ������ : �ӼӸ�
#define dSTORAGE_CHAT_BASIC_PARTY				7007	// �⺻ ������ : ��Ƽ
#define dSTORAGE_CHAT_BASIC_GUILD				7008	// �⺻ ������ : ����
#define dSTORAGE_CHAT_BASIC_TRADE				7009	// �⺻ ������ : �ŷ�
#define dSTORAGE_CHAT_BASIC_SHOUT				7010	// �⺻ ������ : ��ġ��
#define dSTORAGE_CHAT_BASIC_SYSTEM				7011	// �⺻ ������ : �ý���
#define dSTORAGE_CHAT_EXTEND_NORMAL				7012	// Ȯ�� ������ : �Ϲݴ�ȭ
#define dSTORAGE_CHAT_EXTEND_WISPHER			7013	// Ȯ�� ������ : �ӼӸ�
#define dSTORAGE_CHAT_EXTEND_PARTY				7014	// Ȯ�� ������ : ��Ƽ
#define dSTORAGE_CHAT_EXTEND_GUILD				7015	// Ȯ�� ������ : ����
#define dSTORAGE_CHAT_EXTEND_TRADE				7016	// Ȯ�� ������ : �ŷ�
#define dSTORAGE_CHAT_EXTEND_SHOUT				7017	// Ȯ�� ������ : ��ġ��
#define dSTORAGE_CHAT_EXTEND_SYSTEM				7018	// Ȯ�� ������ : �ý���
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
#define dSTORAGE_ETC_EX_QUICKSLOT1						8000	// �߰� ������ 1
#define dSTORAGE_ETC_EX_QUICKSLOT2						8001	// �߰� ������ 2
#define dSTORAGE_ETC_QUICKSLOT_LOCK						8002	// ������ ��ױ�

#define dSTORAGE_CHARACTER_ETC_DISCORD_HIDE_NAME		9000	// show character name in discord 

#define dSTORAGE_CHARACTER_SCOUTER_ON					9010	// scouter status

#define dSTORAGE_SYSTEM_ETC_DISCORD						10000	// Enable/disable discorrd
#define dSTORAGE_SYSTEM_ETC_CONTRACT					10001	// contract/game policy

#define dSTORAGE_EOF									0xFFFFFFFF

#endif//__NTL_STORAGE_DEFINE_H__