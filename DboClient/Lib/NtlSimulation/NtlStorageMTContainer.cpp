#include "precomp_ntlsimulation.h"
#include "NtlStorageMTContainer.h"

#include "NtlStorageDefine.h"


/**
* \brief NTL Storage Define Mapping Table
* 1번은 카테고리로 현재의 값이 카테고리임을 알린다.
*/
SNtlStorageMappingTableData CNtlStorageMTContainer::m_StorageMappingTable[] = {
// Table rule
// { KEY ID, Variable type, eNTL_STORAGE_TYPE, KEY STRING, Check Func, Default Value }

// Table Start ----------------------------------------------------------------------------
{dSTORAGE_CATEGORY,					eUSER,		eNTL_STORAGE_INVALID,	"CATEGORY",			NULL,	NULL},

{dSTORAGE_ACCOUNT_LASTCHAR,	eSTRING,	eNTL_STORAGE_ACCOUNT,	"LASTCHAR",			NULL,	NULL},

{dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH,	eSTRING,	eNTL_STORAGE_GRAPHIC,	"VIDEO_WIDTH",		NULL,	"1280"},
{dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT,	eSTRING,	eNTL_STORAGE_GRAPHIC,	"VIDEO_HEIGHT",		NULL,	"1024"},
{dSTORAGE_GRAPHIC_SELECT_VIDEODEPTH, eSTRING,	eNTL_STORAGE_GRAPHIC,	"VIDEO_DEPTH",		NULL,	"32"},
{dSTORAGE_GRAPHIC_GAMMA,			eFLOAT,		eNTL_STORAGE_GRAPHIC,	"VIDEO_GAMMA",		NULL,	"1.0"},
{dSTORAGE_GRAPHIC_WINDOW_MODE,		eBOOL,		eNTL_STORAGE_GRAPHIC,	"WINDOW_MODE",		NULL,	"false"},
{dSTORAGE_GRAPHIC_PRESET,			eINT,		eNTL_STORAGE_GRAPHIC,	"PRESET",			NULL,	"2"},
{dSTORAGE_GRAPHIC_TERRAIN_RANGE,	eINT,		eNTL_STORAGE_GRAPHIC,	"TERRAIN_RANGE",	NULL,	"3"},
{dSTORAGE_GRAPHIC_OBJECT_RANGE,		eINT,		eNTL_STORAGE_GRAPHIC,	"OBJECT_RANGE",		NULL,	"3"},
{dSTORAGE_GRAPHIC_TERRAIN_SHADOW,	eBOOL,		eNTL_STORAGE_GRAPHIC,	"TERRAIN_SHADOW",	NULL,	"true"},
{dSTORAGE_GRAPHIC_WATER_EFFECT,		eBOOL,		eNTL_STORAGE_GRAPHIC,	"WATER_EFFECT",		NULL,	"true"},
{dSTORAGE_GRAPHIC_CHARACTER_RANGE,	eINT,		eNTL_STORAGE_GRAPHIC,	"CHARACTER_RANGE",	NULL,	"3"},
{dSTORAGE_GRAPHIC_CHARACTER_EFFECT, eBOOL,		eNTL_STORAGE_GRAPHIC,	"CHARACTER_EFFECT", NULL,	"true"},
{dSTORAGE_GRAPHIC_CHARACTER_SHADOW, eBOOL,		eNTL_STORAGE_GRAPHIC,	"CHARACTER_SHADOW", NULL,	"true"},
{dSTORAGE_GRAPHIC_MAGIC_EFFECT,		eINT,		eNTL_STORAGE_GRAPHIC,	"MAGIC_EFFECT",		NULL,	"3"},
{dSTORAGE_GRAPHIC_WEATHER_EFFECT,	eINT,		eNTL_STORAGE_GRAPHIC,	"WEATHER_EFFECT",	NULL,	"0"},
{dSTORAGE_GRAPHIC_TEXTURE_LEVEL,	eINT,		eNTL_STORAGE_GRAPHIC,	"TEXTURE_LEVEL",	NULL,	"2"},
{dSTORAGE_GRAPHIC_SHADER_HDR,		eBOOL,		eNTL_STORAGE_GRAPHIC,	"HDR_SHADER",		NULL,	"true"},
{dSTORAGE_GRAPHIC_CHARACTER_EDGE,	eBOOL,		eNTL_STORAGE_GRAPHIC,	"CHARACTER_EDGE",	NULL,	"true"},
{dSTORAGE_GRAPHIC_SHOW_FPS,			eBOOL,		eNTL_STORAGE_GRAPHIC,	"SHOW_FPS",			NULL,	"true"},
{dSTORAGE_GRAPHIC_FPS,				eINT,		eNTL_STORAGE_GRAPHIC,	"FPS",				NULL,	"60"},

{dSTORAGE_SOUND_MAIN_VOLUME,		eFLOAT,		eNTL_STORAGE_SOUND,		"MAIN_VOLUME",		NULL,	"1.0"},
{dSTORAGE_SOUND_BACK_VOLUME,		eFLOAT,		eNTL_STORAGE_SOUND,		"BACK_VOLUME",		NULL,	"0.5"},
{dSTORAGE_SOUND_EFFECT_VOLUME,		eFLOAT,		eNTL_STORAGE_SOUND,		"EFFECT_VOLUME",	NULL,	"1.0"},
{dSTORAGE_SOUND_ENV_VOLUME,			eFLOAT,		eNTL_STORAGE_SOUND,		"ENV_VOLUME",		NULL,	"1.0"},

{dSTORAGE_GAMEINFO_GUILD_EMBLEM,	eBOOL,		eNTL_STORAGE_GAMEINFO,	"GUILD_EMBLEM",		NULL,	"true"},
{dSTORAGE_GAMEINFO_DOJO_MARK,		eBOOL,		eNTL_STORAGE_GAMEINFO,	"DOJO_MARK",		NULL,	"true"},
{dSTORAGE_GAMEINFO_SCOUTER_VISIBLE,	eBOOL,		eNTL_STORAGE_GAMEINFO,	"SCOUTER_VISIBLE",	NULL,	"true"},

{dSTORAGE_ETC_FRIEND_NOTIFY	,		eBOOL,		eNTL_STORAGE_ETC,		"FRIEND_NOTIFY",	NULL,	"true"},
{dSTORAGE_ETC_GUILD_MEMBER_NOTIFY,	eBOOL,		eNTL_STORAGE_ETC,		"GUILD_NOTIFY",		NULL,	"true"},
{dSTORAGE_ETC_QUESTTEXT_EFFECT,		eBOOL,		eNTL_STORAGE_ETC,		"QUEST_TEXT_EFFECT", NULL,	"true"},
{dSTORAGE_ETC_USER_TRADE,			eBOOL,		eNTL_STORAGE_ETC,		"USER_TRADE",		NULL,	"true"},
{dSTORAGE_ETC_MOVEABLEWINDOW,		eBOOL,		eNTL_STORAGE_ETC,		"MOVEABLE_WINDOW",	NULL,	"true"},
{dSTORAGE_ETC_MOUSE_DASH,			eFLOAT,		eNTL_STORAGE_ETC,		"MOUSE_DASH",		NULL,	"1.0"},
{dSTORAGE_ETC_MOUSE_FOV,			eFLOAT,		eNTL_STORAGE_ETC,		"MOUSE_FOV",		NULL,	"0"},
{dSTORAGE_ETC_MOUSE_VIEW_POINT,		eFLOAT,		eNTL_STORAGE_ETC,		"MOUSE_VIEW_POINT",	NULL,	"0"},

{dSTORAGE_CHAT_INPUTMODE,			eBOOL,		eNTL_STORAGE_CHAT,		"CHAT_INPUT",		NULL,	"false"},
{dSTORAGE_CHAT_WINDOW_LOCK,			eBOOL,		eNTL_STORAGE_CHAT,		"CHAT_LOCK",		NULL,	"false"},
{dSTORAGE_CHAT_MOUSE_SCROLL,		eBOOL,		eNTL_STORAGE_CHAT,		"MOUSE_SCROLL",		NULL,	"true"},
{dSTORAGE_CHAT_EXTEND_WINDOW,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND_WINDOW",	NULL,	"true"},
{dSTORAGE_CHAT_EXTEND2_WINDOW,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND2_WINDOW",	NULL,	"false"},
{dSTORAGE_CHAT_WINDOW_OPACITY,		eINT,		eNTL_STORAGE_CHAT,		"WINDOW_OPACITY",	NULL,	"60"},
{dSTORAGE_CHAT_BASIC_NORMAL,		eBOOL,		eNTL_STORAGE_CHAT,		"BASIC_NORMAL",		NULL,	"true"},
{dSTORAGE_CHAT_BASIC_WISPHER,		eBOOL,		eNTL_STORAGE_CHAT,		"BASIC_WISPHER",	NULL,	"true"},
{dSTORAGE_CHAT_BASIC_PARTY,			eBOOL,		eNTL_STORAGE_CHAT,		"BASIC_PARTY",		NULL,	"true"},
{dSTORAGE_CHAT_BASIC_FIND_PARTY,	eBOOL,		eNTL_STORAGE_CHAT,		"BASIC_FIND_PARTY",	NULL,	"true"},
{dSTORAGE_CHAT_BASIC_GUILD,			eBOOL,		eNTL_STORAGE_CHAT,		"BASIC_GUILD",		NULL,	"true"},
{dSTORAGE_CHAT_BASIC_TRADE,			eBOOL,		eNTL_STORAGE_CHAT,		"BASIC_TRADE",		NULL,	"true"},
{dSTORAGE_CHAT_BASIC_SHOUT,			eBOOL,		eNTL_STORAGE_CHAT,		"BASIC_SHOUT",		NULL,	"true"},
{dSTORAGE_CHAT_BASIC_SYSTEM,		eBOOL,		eNTL_STORAGE_CHAT,		"BASIC_SYSTEM",		NULL,	"false"},
{dSTORAGE_CHAT_EXTEND_NORMAL,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND_NORMAL",	NULL,	"false"},
{dSTORAGE_CHAT_EXTEND_WISPHER,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND_WISPHER",	NULL,	"false"},
{dSTORAGE_CHAT_EXTEND_PARTY,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND_PARTY",		NULL,	"false"},
{dSTORAGE_CHAT_EXTEND_FIND_PARTY,	eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND_FIND_PARTY",NULL,	"false"},
{dSTORAGE_CHAT_EXTEND_GUILD,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND_GUILD",		NULL,	"false"},
{dSTORAGE_CHAT_EXTEND_TRADE,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND_TRADE",		NULL,	"false"},
{dSTORAGE_CHAT_EXTEND_SHOUT,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND_SHOUT",		NULL,	"false"},
{dSTORAGE_CHAT_EXTEND_SYSTEM,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND_SYSTEM",	NULL,	"true"},
{dSTORAGE_CHAT_EXTEND2_NORMAL,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND2_NORMAL",	NULL,	"false"},
{dSTORAGE_CHAT_EXTEND2_WISPHER,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND2_WISPHER",	NULL,	"false"},
{dSTORAGE_CHAT_EXTEND2_PARTY,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND2_PARTY",	NULL,	"false"},
{dSTORAGE_CHAT_EXTEND2_FIND_PARTY,	eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND2_FIND_PARTY",NULL,	"false"},
{dSTORAGE_CHAT_EXTEND2_GUILD,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND2_GUILD",	NULL,	"false"},
{dSTORAGE_CHAT_EXTEND2_TRADE,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND2_TRADE",	NULL,	"false"},
{dSTORAGE_CHAT_EXTEND2_SHOUT,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND2_SHOUT",	NULL,	"false"},
{dSTORAGE_CHAT_EXTEND2_SYSTEM,		eBOOL,		eNTL_STORAGE_CHAT,		"EXTEND2_SYSTEM",	NULL,	"true"},

{dSTORAGE_SCOUTER_MONSTER_SCOUTER_MEMORY_INFO,	eUSER, eNTL_STORAGE_ETC,	"MONSTER_SCOUTER_MEMEORY_INFO", NULL, NULL},
{dSTORAGE_SCOUTER_PLAYER_SCOUTER_MEMORY_INFO,	eUSER, eNTL_STORAGE_ETC,	"PLAYER_SCOUTER_MEMORY_INFO", NULL, NULL},
{dSTORAGE_SCOUTER_INFO_TBLID,					eUSER, eNTL_STORAGE_ETC,	"SCOUTER_INFO_TBLID",	NULL, NULL},
{dSTORAGE_SCOUTER_INFO_POWER,					eUSER, eNTL_STORAGE_ETC,	"SCOUTER_INFO_NEW",	NULL, NULL},

{dSTORAGE_QUEST_MEMORY_INFO,		eUSER,		eNTL_STORAGE_QUEST,		"QUEST_MEMORY_INFO",	NULL, NULL},
{dSTORAGE_QUEST_INFO_ID,			eUSER,		eNTL_STORAGE_QUEST,		"QUEST_INFO_ID",		NULL, NULL},
{dSTORAGE_QUEST_INFO_NEW,			eUSER,		eNTL_STORAGE_QUEST,		"QUEST_INFO_NEW",		NULL, NULL},
{dSTORAGE_QUEST_INFO_INDICATE,		eUSER,		eNTL_STORAGE_QUEST,		"QUEST_INFO_INDICATE",	NULL, NULL},

{dSTORAGE_ETC_EX_QUICKSLOT1,		eBOOL,		eNTL_STORAGE_CHARACTER,		"EX_QUICKSLOT1",	NULL,	"true"},
{dSTORAGE_ETC_EX_QUICKSLOT2,		eBOOL,		eNTL_STORAGE_CHARACTER,		"EX_QUICKSLOT2",	NULL,	"false"},
{dSTORAGE_ETC_QUICKSLOT_LOCK,		eBOOL,		eNTL_STORAGE_CHARACTER,		"QUICKSLOT_LOCK",	NULL,	"false"},

{ dSTORAGE_CHARACTER_ETC_DISCORD_HIDE_NAME,	eBOOL,		eNTL_STORAGE_CHARACTER_ETC,		"DISCORD_HIDE_NAME",	NULL,	"false" },

{ dSTORAGE_CHARACTER_SCOUTER_ON,	eBOOL,		eNTL_STORAGE_CHARACTER_SCOUTER,		"ON",	NULL,	"true" }, // by default, scouter is turned on


{ dSTORAGE_SYSTEM_ETC_DISCORD,			eBOOL,		eNTL_STORAGE_SYSTEM_ETC,		"DISCORD",			NULL,	"true" },
{ dSTORAGE_SYSTEM_ETC_CONTRACT,			eINT,		eNTL_STORAGE_SYSTEM_ETC,		"CONTRACT",			NULL,	"0" },

{dSTORAGE_EOF,						eUSER,		eNTL_STORAGE_INVALID,	"END",					NULL, NULL},


// You have to insert the table data after the current line. 
// 반드시 현재 라인 다음에 추가하여야 합니다.

// Table End ----------------------------------------------------------------------------
};

CNtlStorageMTContainer::CNtlStorageMTContainer(void)
: m_bSorted(false)
, m_uiCount(0)
{
	SortTable();
}

CNtlStorageMTContainer::~CNtlStorageMTContainer(void)
{
}

/**
* \brief 미리 테이블을 한번 순회하여 카테고리별, ID별 포인터를 저장하고 있는다.
*/
bool CNtlStorageMTContainer::SortTable()
{
	// 테이블 갯수를 읽어들여서 저장해놓는다.
	m_uiCount = _countof(m_StorageMappingTable);

	for( unsigned int i=0; i < m_uiCount; ++i )
	{
		// 카테고리별로 등록
		m_mmapCategory.insert(
			std::make_pair( m_StorageMappingTable[i].byCategoryType,
			&m_StorageMappingTable[i] ));

		// KEY ID를 Key로 하여 등록
		m_mapTable.insert(
			std::make_pair( m_StorageMappingTable[i].uiKey,
			&m_StorageMappingTable[i] ));

		// STRING을 Key로 등록
		std::string strKey;
		strKey.append(m_StorageMappingTable[i].acKeyName);
		
		m_mapString.insert(
			std::make_pair( strKey,
			&m_StorageMappingTable[i] ));
		
	}

	m_bSorted = true;
	return true;
}

bool CNtlStorageMTContainer::SetUnitCheckFunc( unsigned int uiKey, NtlStorageUnitCheckFunc funcApply)
{
	TABLEMAP::iterator it = m_mapTable.find( uiKey );
	if( it == m_mapTable.end() )
	{
		// 테이블에 등록되어 있는 KEY가 없다.

		return false;
	}

	// 새로운 함수 포인터 설정
	SNtlStorageMappingTableData* pData = it->second;
	pData->ntlStorageFunc = funcApply;

	return true;
}

NtlStorageUnitCheckFunc CNtlStorageMTContainer::GetUnitCheckFunc( unsigned int uiKey )
{
	TABLEMAP::iterator it = m_mapTable.find( uiKey );
	if( it == m_mapTable.end() )
	{
		// 테이블에 등록되어 있는 KEY가 없다.

		return NULL;
	}

	return it->second->ntlStorageFunc;
}

unsigned int CNtlStorageMTContainer::GetCount()
{
	return m_uiCount;
}

eNTL_STORAGE_VARIABLE_TYPE CNtlStorageMTContainer::GetVariableType( unsigned int uiKey )
{
	TABLEMAP::iterator it = m_mapTable.find( uiKey );
	if( it == m_mapTable.end() )
	{
		// 테이블에 등록되어 있는 KEY가 없다.

		return eNTL_STORAGE_VARIABLE_INVALID;
	}

	return (eNTL_STORAGE_VARIABLE_TYPE)it->second->byVarType;
}

eNTL_STORAGE_TYPE CNtlStorageMTContainer::GetStorageType( unsigned int uiKey )
{
	TABLEMAP::iterator it = m_mapTable.find( uiKey );
	if( it == m_mapTable.end() )
	{
		// 테이블에 등록되어 있는 KEY가 없다.

		return eNTL_STORAGE_INVALID;
	}

	return (eNTL_STORAGE_TYPE)it->second->byCategoryType;
}

const char* CNtlStorageMTContainer::GetString( unsigned int uiKey )
{
	TABLEMAP::iterator it = m_mapTable.find( uiKey );
	if( it == m_mapTable.end() )
	{
		// 테이블에 등록되어 있는 KEY가 없다.

		return NULL;
	}

	return it->second->acKeyName;
}

const char* CNtlStorageMTContainer::GetDefaultString( unsigned int uiKey )
{
	TABLEMAP::iterator it = m_mapTable.find( uiKey );
	if( it == m_mapTable.end() )
	{
		// 테이블에 등록되어 있는 KEY가 없다.

		return NULL;
	}

	return it->second->acDefaultValue;
}

bool CNtlStorageMTContainer::GetDefaultBool( unsigned int uiKey )
{
	TABLEMAP::iterator it = m_mapTable.find( uiKey );
	if( it == m_mapTable.end() )
	{
		// 테이블에 등록되어 있는 KEY가 없다.

		return false;
	}

	if( strcmp( it->second->acDefaultValue, dSTORAGE_TRUE ) == 0 )
	{
		return true;
	}
	else
		return false;
}

int CNtlStorageMTContainer::GetDefaultInt( unsigned int uiKey )
{
	TABLEMAP::iterator it = m_mapTable.find( uiKey );
	if( it == m_mapTable.end() )
	{
		// 테이블에 등록되어 있는 KEY가 없다.

		return 0xFFFFFFFF;
	}

	return atoi( it->second->acDefaultValue );
}

float CNtlStorageMTContainer::GetDefaultFloat( unsigned int uiKey )
{
	TABLEMAP::iterator it = m_mapTable.find( uiKey );
	if( it == m_mapTable.end() )
	{
		// 테이블에 등록되어 있는 KEY가 없다.

		return 0.0f;
	}

	return (float)atof( it->second->acDefaultValue );
}

bool CNtlStorageMTContainer::GetKeyFromString( std::string& strKeyName, unsigned int& uiKey )
{
	STRINGMAP::iterator it = m_mapString.find( strKeyName );
	if( it == m_mapString.end() )
	{
		// There is no registered key in the table.

		return false;
	}

	uiKey = it->second->uiKey;
	return true;
}