//***********************************************************************************
//
//	File		:	TableContainer.h
//
//	Begin		:	2007-01-31
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "SpawnTable.h"

class CNtlFileSerializer;
class CNtlBitFlagManager;

class CExpTable;
class CHelpTable;
class CMerchantTable;
class CMobTable;
class CNewbieTable;
class CNPCTable;
class CPCTable;
class CStatusTransformTable;

class CItemOptionTable;
class CItemTable;
class CUseItemTable;
class CDragonBallTable;
class CDragonBallRewardTable;
class CDragonBallReturnPointTable;

class CActionTable;
class CChatCommandTable;
class CDirectionLinkTable;
class CFormulaTable;
class CServerConfigTable;

class CCharmTable;

class CQuestDropTable;
class CQuestItemTable;
class CQuestProbabilityTable;
class CQuestTextDataTable;
class CQuestRewardTable;

class CHTBSetTable;
class CSkillTable;
class CSystemEffectTable;

class CTextAllTable;
class CChattingFilterTable;

class CObjectTable;
class CSpawnTable;
class CWayPointTable;
class CWorldTable;
class CWorldZoneTable;
class CWorldPlayTable;
class CWorldMapTable;
class CLandMarkTable;
class CModelToolCharDataTable;
class CModelToolObjDataTable;
class CWorldPathTable;

class CTimeQuestTable;
class CRankBattleTable;
class CBudokaiTable;
class CDungeonTable;

class CTableFileNameList;

class CGuideHintTable;
class CPortalTable;
class CNpcSpeechTable;
class CSetItemTable;
class CScriptLinkTable;
class CQuestNarrationTable;

class CVehicleTable;
class CItemRecipeTable;


class CDynamicObjectTable;
class CMobMovePatternTable;
class CDojoTable;
class CItemMixMachineTable;

class CHLSItemTable;

//new
class CCharTitleTable;
class CDwcTable;
class CDwcMissionTable;
class CItemBagListTable;
class CItemEnchantTable;
class CItemGroupListTable;
class CMascotTable;
class CQuestRewardSelectTable;
class CMascotGradeTable;
class CMascotStatusTable;
class CItemMixExpTable;
class CItemUpgradeRateNewTable;
class CAirCostumeTable;
class CCommonConfigTable;
class CSlotMachineTable;
class CHlsSlotMachineItemTable;
class CItemDisassembleTable;
class CNpcServerTable;
class CMobServerTable;
class CEventSystemTable;
class CDynamicFieldSystemTable;



class CTableContainer
{
public:
	enum eTABLE
	{
		// Character
		TABLE_EXP = 0,
		TABLE_HELP,
		TABLE_MERCHANT,
		TABLE_MOB,
		TABLE_NEWBIE,
		TABLE_NPC,
		TABLE_PC,
		TABLE_STATUS_TRANSFORM,
		TABLE_GUIDE_HINT,

		// Item
		TABLE_ITEM_OPTION,
		TABLE_ITEM,
		TABLE_USE_ITEM,
		TABLE_SET_ITEM,
		TABLE_DRAGONBALL,
		TABLE_DRAGONBALL_REWARD,
		TABLE_DRAGONBALL_RETURN_POINT, //new


		// Misc
		TABLE_ACTION,
		TABLE_CHAT_COMMAND,
		TABLE_DIRECTION_LINK,
		TABLE_FORMULA,
		TABLE_SERVERCONFIG,

		// Party
		TABLE_CHARM,

		// Quest
		TABLE_QUEST_DROP,
		TABLE_QUEST_ITEM,
		TABLE_QUEST_PROBABILITY,
		TABLE_QUEST_TEXT_DATA,
		TABLE_QUEST_REWARD,
		TABLE_QUEST_REWARD_SELECT, //new

		// Rule
		TABLE_RANKBATTLE,
		TABLE_TIMEQUEST,
		TABLE_BUDOKAI,

		// Skill
		TABLE_HTB_SET,
		TABLE_SKILL,
		TABLE_SYSTEM_EFFECT,

		// Text
		TABLE_TEXT_ALL,
		TABLE_CHATTING_FILTER,
		TABLE_TEXT_SERVER,

		// World
		TABLE_LAND_MARK,
		TABLE_OBJECT,
		TABLE_NPC_SPAWN,
		TABLE_MOB_SPAWN,
		TABLE_WORLD_MAP,
		TABLE_WORLD,
		TABLE_WORLD_ZONE,
		TABLE_WORLD_PLAY,

		// GraphicData
		TABLE_MODEL_TOOL_CHAR,
		TABLE_MODEL_TOOL_OBJ,

		// World Data
		TABLE_PASS_POINT,

		// Portal Data
		TABLE_PORTAL,

		TABLE_SPEECH,

		TABLE_SCRIPT_LINK,
		TABLE_DUNGEON,
		
		TABLE_QUEST_NARRATION,

		TABLE_VEHICLE,
		TABLE_ITEM_RECIPE,
		TABLE_ITEM_RECIPE_NEW, //new
		TABLE_DYNAMIC_OBJECT,
		TABLE_MOB_MOVE_PATTERN,
		TABLE_DOJO,
		TABLE_ITEM_UPGRADE_RATE_NEW, //new
		TABLE_MIX_MACHINE,
		TABLE_ITEM_MIX_EXP, //new
		
		TABLE_HLS_ITEM,
		TABLE_HLS_SLOT_MACHINE,
		TABLE_HLS_SLOT_MACHINE_ITEM,

		TABLE_NPC_SERVER, //new
		TABLE_MOB_SERVER, //new
		TABLE_ITEM_ENCHANT, //new
		TABLE_ITEM_DISASSEMBLE,

		//mascot
		TABLE_MASCOT, //new
		TABLE_MASCOT_GRADE, //new
		TABLE_MASCOT_STATUS, //new

		TABLE_COMMON_CONFIG, //new
		TABLE_DWC, //new
		TABLE_DWCMISSION, //new
		TABLE_CHARTITLE, //new
		TABLE_EVENT_SYSTEM, //new
		TABLE_DYNAMIC_FIELD_SYSTEM, //new
		TABLE_ITEM_BAG_LIST, //new
		TABLE_ITEM_GROUP_LIST, //new
		TABLE_AIR_COSTUME,//new

		TABLE_COUNT
	};

public:
	CTableContainer(void);

	virtual ~CTableContainer(void);

public:

	typedef std::map<TBLIDX, CSpawnTable*> SPAWNTABLEMAP;
	typedef SPAWNTABLEMAP::iterator SPAWNTABLEIT;
	typedef SPAWNTABLEMAP::value_type SPAWNTABLEVAL;

	typedef std::map<TBLIDX, CObjectTable*> OBJTABLEMAP;
	typedef OBJTABLEMAP::iterator OBJTABLEIT;
	typedef OBJTABLEMAP::value_type OBJTABLEVAL;


public:

	class ICallBack
	{
	public:
		virtual bool Call( const char* pfilename, CNtlFileSerializer* pSeralize, const char* pszCryptPassword ) = 0;
	};

	// Create 사용 시 pCall 이 NULL 일 경우 이전의 방식과 동일하게 동작한다. pCall -> Client 사용용 
	// Create pCall used in this way, the behavior is the same as the previous case NULL. pCall -> Client for use
	bool							Create(CNtlBitFlagManager& rTableFlag, char* pszPath, CTableFileNameList* pFileNameList, CTable::eLOADING_METHOD eLoadingMethod, DWORD dwCodePage, ICallBack* pCall );	

	bool							Create(CNtlBitFlagManager& rTableFlag, WCHAR* pwszPath, CTableFileNameList* pFileNameList, CTable::eLOADING_METHOD eLoadingMethod, DWORD dwCodePage, ICallBack* pCall );

	void							Destroy();


protected:

	void							Init();


public:

	bool							Reload(CNtlBitFlagManager& rTableFlag, CTableFileNameList* pFileNameList);

	//update current tables with the ones from localize folder (required for merge tool)
	bool							Update(CNtlBitFlagManager& rTableFlag, CTableFileNameList* pFileNameList);

	bool							SaveToFile(CNtlBitFlagManager& rTableFlag, CTableFileNameList* pFileNameList, bool bNeedToEncrypt);

	void							SetPath(char* pszPath);

	void							SetPath(WCHAR* pwszPath) { m_wstrPath = pwszPath; }

protected:

	bool							InitializeTable(CTable* pTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializeTable(CTable* pTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializeTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializeTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializePackTable(CTable* pTable, CNtlFileSerializer& serializer, WCHAR* pszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializePackTable(CTable* pTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializePackTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, WCHAR* pszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializePackTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension, ICallBack* pCall);

	bool							ReloadTable(CTable* pTable, CNtlFileSerializer& serializer, char* pwszFileNameWithoutExtension);
	bool							ReloadTable(CTable* pTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension);

	//Update tables with data from localize folder
	bool							UpdateTable(CTable* pTable, CNtlFileSerializer& serializer, char* pwszFileNameWithoutExtension);
	bool							UpdateTable(CTable* pTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension);

	bool							UpdateTextAllTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, char* pwszFileNameWithoutExtension);
	bool							UpdateTextAllTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension);

private:

	template<class T>
	void DboCreateTable(const char *szClassName, CNtlFileSerializer & serializer, wchar_t * pwszFileName, T **pTable, CTableContainer::ICallBack *pCall)
	{
		if (!*pTable)
		{
			T* pNewTable = new T;

			if (false == pNewTable->Create(m_dwCodePage))
			{
				CTable::CallErrorCallbackFunction("%s::Create() failed.(Table : %s)", szClassName);
				delete pNewTable;
			}

			*pTable = pNewTable;
		}

		if (NULL == pCall)
		{
			if (false == InitializeTable(*pTable, serializer, pwszFileName, NULL))
			{
				CTable::CallErrorCallbackFunction("InitializeTable() failed.(Table : %s)", szClassName);
				delete pTable;
			}
		}
		else
		{
			if (false == InitializePackTable(*pTable, serializer, pwszFileName, pCall))
			{
				CTable::CallErrorCallbackFunction("InitializeTable() failed.(Table : %s)", szClassName);
				delete pTable;
			}
		}
	}

public:

// Character
	CExpTable*						GetExpTable() { return m_pExpTable; }

	CGuideHintTable*				GetGuideHintTable() { return m_pGuideHintTable; }

	CHelpTable*						GetHelpTable() { return m_pHelpTable; }

	CMerchantTable*					GetMerchantTable() { return m_pMerchantTable; }

	CMobTable*						GetMobTable() { return m_pMobTable; }

	CNewbieTable*					GetNewbieTable() { return m_pNewbieTable; }

	CNPCTable*						GetNpcTable() { return m_pNpcTable; }

	CPCTable*						GetPcTable() { return m_pPcTable; }

	CStatusTransformTable*			GetStatusTransformTable() { return m_pStatusTransformTable; }

	//new
	CCharTitleTable*				GetCharTitleTable() { return m_pCharTitleTable; }
	CDwcTable*						GetDwcTable()		{ return m_pDwcTable;	}
	CDwcMissionTable*				GetDwcMissionTable()		{ return m_pDwcMissionTable;	}
	CItemBagListTable*				GetItemBagListTable()	{ return m_pItemBagListTable; }
	CItemEnchantTable*				GetItemEnchantTable()	{ return m_pItemEnchantTable; }
	CItemGroupListTable*			GetItemGroupListTable()	{ return m_pItemGroupListTable;	}
	CMascotTable*					GetMascotTable()	{ return m_pMascotTable;	}
	CQuestRewardSelectTable*		GetQuestRewardSelectTable()	{ return m_pQuestRewardSelectTable; }
	CMascotGradeTable*				GetMascotGradeTable()	{ return m_pMascotGradeTable; }
	CMascotStatusTable*				GetMascotStatusTable()	{ return m_pMascotStatusTable; }
	CItemMixExpTable*				GetItemMixExpTable()	{ return m_pItemMixExpTable; }
	CItemUpgradeRateNewTable*		GetItemUpgradeRateNewTable()	{ return m_pItemUpgradeRateNewTable;	}
	CAirCostumeTable*				GetAirCostumeTable()	{ return m_pAirCostumeTable;	}
	CCommonConfigTable*				GetCommonConfigTable()	{ return m_pCommonConfigTable;	}
	CNpcServerTable*				GetNpcServerTable()	{ return m_pNpcServerTable; }
	CMobServerTable*				GetMobServerTable()	{ return m_pMobServerTable; }

// Item
	CDragonBallRewardTable*			GetDragonBallRewardTable() { return m_pDragonBallRewardTable; }
	CDragonBallTable*				GetDragonBallTable() { return m_pDragonBallTable; }
	CDragonBallReturnPointTable*	GetDragonBallReturnPointTable() { return m_pDragonBallReturnPointTable; }

	CItemOptionTable*				GetItemOptionTable() { return m_pItemOptionTable; }

	CItemTable*						GetItemTable() { return m_pItemTable; }

	CUseItemTable*					GetUseItemTable() { return m_pUseItemTable; }

	CSetItemTable*					GetSetItemTable() { return m_pSetItemTable; }

	CItemDisassembleTable*			GetItemDisassembleTable() { return m_pItemDisassembleTable; }
	
// Misc
	CActionTable*					GetActionTable() { return m_pActionTable; }

	CChatCommandTable*				GetChatCommandTable() { return m_pChatCommandTable; }

	CDirectionLinkTable *			GetDirectionLinkTable() { return m_pDirectionLinkTable; }

	CFormulaTable*					GetFormulaTable() { return m_pFormulaTable; }

	CServerConfigTable*				GetServerConfigTable() { return m_pServerConfigTable; }

// Party
	CCharmTable*					GetCharmTable() { return m_pCharmTable; }

// Quest
	CQuestDropTable*				GetQuestDropTable() { return m_pQuestDropTable; }

	CQuestItemTable*				GetQuestItemTable() { return m_pQuestItemTable; }

	CQuestProbabilityTable*			GetQuestProbabilityTable() { return m_pQuestProbalityTable; }

	CQuestTextDataTable*			GetQuestTextDataTable() { return m_pQuestTextDataTable; }

	CQuestRewardTable*				GetQuestRewardTable() { return m_pQuestRewardTable; }

// Rule
	CRankBattleTable *				GetRankBattleTable() { return m_pRankBattleTable; }

	CTimeQuestTable *				GetTimeQuestTable() { return m_pTimeQuestTable; }

	CBudokaiTable *					GetBudokaiTable() { return m_pBudokaiTable;}

	CDungeonTable *					GetDungeonTable() { return m_pDungeonTable; }
// Skill
	CHTBSetTable*					GetHTBSetTable() { return m_pHTBSetTable; }

	CSkillTable*					GetSkillTable() { return m_pSkillTable; }

	CSystemEffectTable*				GetSystemEffectTable() { return m_pSystemEffectTable; }

// Text
	CTextAllTable*					GetTextAllTable() { return m_pTextAllTable; }
	CChattingFilterTable*			GetChattingFilterTable() { return m_pChattingFilterTable; }

// World
	CLandMarkTable*					GetLandMarkTable() { return m_pLandMarkTable; } 

	CObjectTable*					GetObjectTable(TBLIDX worldTblidx);

	CSpawnTable*					GetNpcSpawnTable(TBLIDX worldTblidx);

	CSpawnTable*					GetMobSpawnTable(TBLIDX worldTblidx);

	CWorldMapTable*					GetWorldMapTable() { return m_pWorldMapTable; }

	CWorldTable*					GetWorldTable() { return m_pWorldTable; }

	CWorldZoneTable*				GetWorldZoneTable() { return m_pWorldZoneTable; }

// Graphic Data
	CModelToolCharDataTable *		GetModelToolCharDataTable() { return m_pCharDataAnimTable; }

	CModelToolObjDataTable *		GetModelToolObjDataTable() { return m_pObjDataAnimTable; }

// World Data
	CWorldPathTable*				GetWorldPathTable() { return m_pWorldPathTable; }

	CWorldPlayTable*				GetWorldPlayTable() { return m_pWorldPlayTable; }

// Portal Data
	CPortalTable*					GetPortalTable() { return m_pPortalTable; }

	CNpcSpeechTable*				GetNpcSpeechTable() { return m_pNpcSpeechTable; } 
	
	CScriptLinkTable*				GetScriptLinkTable() { return m_pScriptLinkTable; }

	CQuestNarrationTable*			GetQuestNarrationTable() { return m_pQuestNarrationTable; }

	CVehicleTable*					GetVehicleTable() { return m_pVehicleTable; }

	CItemRecipeTable*				GetItemRecipeTable() { return m_pItemRecipeTable; }

	CDynamicObjectTable*			GetDynamicObjectTable() { return m_pDynamicObjectTable; }

	CMobMovePatternTable*			GetMobMovePatternTable() { return m_pMobMovePatternTable; }

	CDojoTable*						GetDojoTable() { return m_pDojoTable; }

	CItemMixMachineTable*			GetItemMixMachineTable() { return m_pItemMixMachineTable; }

	CHLSItemTable*					GetHLSItemTable() { return m_pHLSItemTable; }
	
	CSlotMachineTable*				GetSlotMachineTable() { return m_pSlotMachineTable; }

	CHlsSlotMachineItemTable*		GetSlotMachineItemTable() { return m_pHlsSlotMachineItemTable; }

	CEventSystemTable*				GetEventSystemTable() { return m_pEventSystemTable; }
	CDynamicFieldSystemTable*		GetDynamicFieldSystemTable() { return m_pDynamicFieldSystemTable; }


public:

	OBJTABLEIT						BeginObjectTable();

	OBJTABLEIT						EndObjectTable();

	SPAWNTABLEIT					BeginNpcSpawnTable();

	SPAWNTABLEIT					EndNpcSpawnTable();

	SPAWNTABLEIT					BeginMobSpawnTable();

	SPAWNTABLEIT					EndMobSpawnTable();


protected:

	std::wstring					m_wstrPath;

	CTable::eLOADING_METHOD			m_eLoadingMethod;

	DWORD							m_dwCodePage;

// Character
	CExpTable*						m_pExpTable;

	CGuideHintTable*				m_pGuideHintTable;

	CHelpTable *					m_pHelpTable;

	CMerchantTable*					m_pMerchantTable;

	CMobTable*						m_pMobTable;

	CNewbieTable*					m_pNewbieTable;

	CNPCTable*						m_pNpcTable;

	CPCTable*						m_pPcTable;

	CStatusTransformTable*			m_pStatusTransformTable;

	//new
	CCharTitleTable*				m_pCharTitleTable;
	CDwcTable*						m_pDwcTable;
	CDwcMissionTable*				m_pDwcMissionTable;
	CItemBagListTable*				m_pItemBagListTable;
	CItemEnchantTable*				m_pItemEnchantTable;
	CItemGroupListTable*			m_pItemGroupListTable;
	CMascotTable*					m_pMascotTable;
	CQuestRewardSelectTable*		m_pQuestRewardSelectTable;
	CMascotGradeTable*				m_pMascotGradeTable;
	CMascotStatusTable*				m_pMascotStatusTable;
	CItemMixExpTable*				m_pItemMixExpTable;
	CItemUpgradeRateNewTable*		m_pItemUpgradeRateNewTable;
	CAirCostumeTable*				m_pAirCostumeTable;
	CCommonConfigTable*				m_pCommonConfigTable;
	CNpcServerTable*				m_pNpcServerTable;
	CMobServerTable*				m_pMobServerTable;
	CEventSystemTable*				m_pEventSystemTable;
	CDynamicFieldSystemTable*		m_pDynamicFieldSystemTable;

// Item
	CDragonBallRewardTable*			m_pDragonBallRewardTable;
	CDragonBallTable*				m_pDragonBallTable;
	CDragonBallReturnPointTable*	m_pDragonBallReturnPointTable;

	CItemOptionTable*				m_pItemOptionTable;

	CItemTable*						m_pItemTable;

	CUseItemTable*					m_pUseItemTable;

	CSetItemTable*					m_pSetItemTable;

	CItemDisassembleTable*			m_pItemDisassembleTable;
	
// Misc
	CActionTable*					m_pActionTable;

	CChatCommandTable*				m_pChatCommandTable;

	CDirectionLinkTable *			m_pDirectionLinkTable;

	CFormulaTable*					m_pFormulaTable;

	CServerConfigTable*				m_pServerConfigTable;

// Party
	CCharmTable*					m_pCharmTable;

// Quest
	CQuestDropTable*				m_pQuestDropTable;		// 퀘스트 드롭 아이템 테이블

	CQuestItemTable*				m_pQuestItemTable;		// 퀘스트 아이템 테이블

	CQuestProbabilityTable*			m_pQuestProbalityTable;

	CQuestTextDataTable*			m_pQuestTextDataTable;

	CQuestRewardTable*				m_pQuestRewardTable;

// Rule
	CRankBattleTable *				m_pRankBattleTable;

	CTimeQuestTable *				m_pTimeQuestTable;

	CBudokaiTable *					m_pBudokaiTable;

	CDungeonTable *					m_pDungeonTable;
// Skill
	CHTBSetTable*					m_pHTBSetTable;

	CSkillTable*					m_pSkillTable;

	CSystemEffectTable*				m_pSystemEffectTable;

// Text
	CTextAllTable*					m_pTextAllTable;
	CChattingFilterTable*			m_pChattingFilterTable;

// World
	CLandMarkTable*					m_pLandMarkTable;

	OBJTABLEMAP						m_mapObjectTable;

	SPAWNTABLEMAP					m_mapNpcSpawnTable;

	SPAWNTABLEMAP					m_mapMobSpawnTable;

	CWorldMapTable*					m_pWorldMapTable;

	CWorldTable*					m_pWorldTable;

	CWorldZoneTable*				m_pWorldZoneTable;

	CWorldPlayTable*				m_pWorldPlayTable;

// Graphic Data
	CModelToolCharDataTable	*		m_pCharDataAnimTable;

	CModelToolObjDataTable *		m_pObjDataAnimTable;

// World Data
	CWorldPathTable*				m_pWorldPathTable;

// Portal Data
	CPortalTable*					m_pPortalTable;

	CNpcSpeechTable*				m_pNpcSpeechTable;	

	CScriptLinkTable*				m_pScriptLinkTable;

	CQuestNarrationTable*			m_pQuestNarrationTable;

	CVehicleTable*					m_pVehicleTable; 

	CItemRecipeTable*				m_pItemRecipeTable;

	CDynamicObjectTable*			m_pDynamicObjectTable;

	CMobMovePatternTable*			m_pMobMovePatternTable;

	CDojoTable*						m_pDojoTable;

	CItemMixMachineTable*			m_pItemMixMachineTable;

	CHLSItemTable*					m_pHLSItemTable; 

	CSlotMachineTable*				m_pSlotMachineTable;

	CHlsSlotMachineItemTable*		m_pHlsSlotMachineItemTable;
};