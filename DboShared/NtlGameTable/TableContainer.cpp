//***********************************************************************************
//
//	File		:	TableContainer.cpp
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

#include "StdAfx.h"
#include "TableContainer.h"
#include "TableFileNameList.h"

#include "ExpTable.h"
#include "HelpTable.h"
#include "MerchantTable.h"
#include "MobTable.h"
#include "NewbieTable.h"
#include "NPCTable.h"
#include "PCTable.h"
#include "StatusTransformTable.h"
#include "GuideHintTable.h"

#include "ItemOptionTable.h"
#include "ItemTable.h"
#include "UseItemTable.h"
#include "SetItemTable.h"

#include "DragonBallTable.h"
#include "DragonBallRewardTable.h"
#include "DragonBallReturnPointTable.h"

#include "ActionTable.h"
#include "ChatCommandTable.h"
#include "DirectionLinkTable.h"
#include "FormulaTable.h"
#include "ServerConfigTable.h"

#include "CharmTable.h"

#include "QuestDropTable.h"
#include "QuestItemTable.h"
#include "QuestProbabilityTable.h"
#include "QuestTextDataTable.h"
#include "QuestRewardTable.h"

#include "HTBSetTable.h"
#include "SkillTable.h"
#include "SystemEffectTable.h"

#include "TextAllTable.h"
#include "ChattingFilterTable.h"

#include "ObjectTable.h"
#include "SpawnTable.h"
#include "WorldTable.h"
#include "WorldZoneTable.h"
#include "WorldMapTable.h"
#include "LandMarkTable.h"
#include "WorldPathTable.h"
#include "WorldPlayTable.h"
#include "PortalTable.h"
#include "NpcSpeechTable.h"
#include "SetItemTable.h"

#include "TimeQuestTable.h"
#include "RankBattleTable.h"
#include "BudokaiTable.h"
#include "ScriptLinkTable.h"
#include "DungeonTable.h"

#include "ModelToolCharDataTable.h"
#include "ModelToolObjDataTable.h"

#include "QuestNarrationTable.h"

#include "VehicleTable.h"

#include "ItemRecipeTable.h"

#include "DynamicObjectTable.h"
#include "MobMovePatternTable.h"

#include "DojoTable.h"
#include "ItemMixMachineTable.h"

#include "HLSItemTable.h"

#include "NtlFileSerializer.h"
#include "NtlStringHandler.h"
#include "NtlBitFlagManager.h"


//new
#include "CharTitleTable.h"
#include "DwcTable.h"
#include "DwcMissionTable.h"
#include "ItemBagListTable.h"
#include "ItemEnchantTable.h"
#include "ItemGroupListTable.h"
#include "MascotTable.h"
#include "QuestRewardSelectTable.h"
#include "MascotGradeTable.h"
#include "MascotStatusTable.h"
#include "ItemMixExpTable.h"
#include "ItemUpgradeRateNewTable.h"
#include "AirCostumeTable.h"
#include "CommonConfigTable.h"
#include "SlotMachineTable.h"
#include "HlsSlotMachineItemTable.h"
#include "ItemDisassembleTable.h"
#include "ContentsOnOffTable.h"
#include "NpcServerTable.h"
#include "MobServerTable.h"
#include "MobServerTable.h"
#include "EventSystemTable.h"
#include "DynamicFieldSystemTable.h"



#define DBO_CREATE_TABLE(table_class_name, serializer, file_name_without_extension, table_pointer_variable, table_call_back)		\
	{																											\
		if (NULL != table_pointer_variable)																		\
		{																										\
			CTable::CallErrorCallbackFunction("The table is already created.(Table : %s)", #table_class_name);	\
			delete table_pointer_variable;																		\
		}																										\
		table_class_name* pTable = new table_class_name;														\
		if (NULL == pTable)																						\
		{																										\
			CTable::CallErrorCallbackFunction("new operation failed.(Table : %s)", #table_class_name);			\
			Destroy();																							\
			return false;																						\
		}																										\
		if (false == pTable->Create(m_dwCodePage))																\
		{																										\
			CTable::CallErrorCallbackFunction("%s::Create() failed.(Table : %s)", #table_class_name);			\
			delete pTable;																						\
																												\
			Destroy();																							\
			return false;																						\
		}																										\
		if (NULL == table_call_back)																			\
		{																										\
			if (false == InitializeTable(pTable, serializer, file_name_without_extension, table_call_back))		\
			{																									\
				CTable::CallErrorCallbackFunction("InitializeTable() failed.(Table : %s)", #table_class_name);	\
				delete pTable;																					\
																												\
				Destroy();																						\
				return false;																					\
			}																									\
		}																										\
		else																									\
		{																										\
			if (false == InitializePackTable(pTable, serializer, file_name_without_extension, table_call_back))	\
			{																									\
				CTable::CallErrorCallbackFunction("InitializeTable() failed.(Table : %s)", #table_class_name);	\
				delete pTable;																					\
																												\
				Destroy();																						\
				return false;																					\
			}																									\
		}																										\
		table_pointer_variable = pTable;																		\
	}																											\

#define DBO_RELOAD_TABLE(table_class_name, serializer, file_name_without_extension, table_pointer_variable)		\
	{																											\
		if (NULL == table_pointer_variable)																		\
		{																										\
			CTable::CallErrorCallbackFunction("The table is not created.(Table : %s)", #table_class_name);		\
			return false;																						\
		}																										\
		if ( false == ReloadTable( table_pointer_variable, serializer, file_name_without_extension ) )			\
		{																										\
			CTable::CallErrorCallbackFunction("%s::ReloadTable() failed.(Table : %s)", #table_class_name);		\
			return false;																						\
		}																										\
	}


#define DBO_UPDATE_TABLE(table_class_name, serializer, file_name_without_extension, table_pointer_variable)		\
	{																											\
		if (NULL == table_pointer_variable)																		\
		{																										\
			CTable::CallErrorCallbackFunction("The table is not created.(Table : %s)", #table_class_name);		\
			return false;																						\
		}																										\
		if ( false == UpdateTable( table_pointer_variable, serializer, file_name_without_extension ) )			\
		{																										\
			CTable::CallErrorCallbackFunction("%s::UpdateTable() failed.(Table : %s)", #table_class_name);		\
			return false;																						\
		}																										\
	}

#define DBO_CREATE_AND_REGISTER_TABLE(table_class_name, serializer, file_name_without_extension, table_map, tblidx, table_call_back)		\
	{																													\
		table_class_name* pNewTable = NULL;																				\
		DBO_CREATE_TABLE(table_class_name, serializer, file_name_without_extension, pNewTable, table_call_back);							\
																														\
		if (false == table_map.insert(std::pair<TBLIDX, table_class_name*>(tblidx, pNewTable)).second)					\
		{																												\
			_ASSERTE( !"table_map.insert Fail");																		\
			delete pNewTable;																							\
																														\
			Destroy();																									\
			return false;																								\
		}																												\
	}


#define DBO_EXPORT_TABLE(table_object_pointer, serializer, file_name_without_extension, need_to_encrypt)		\
	{																											\
		serializer.Refresh();																					\
																												\
		std::wstring wstrFullPath;																				\
																												\
		wstrFullPath = m_wstrPath;																				\
		wstrFullPath += L"\\";																					\
		wstrFullPath += file_name_without_extension;															\
		if (false == need_to_encrypt)																			\
		{																										\
			wstrFullPath += L".rdf";																			\
																												\
			table_object_pointer->SaveToBinary(serializer);														\
																												\
			serializer.SaveFile((WCHAR*)(wstrFullPath.c_str()), false);											\
		}																										\
		else																									\
		{																										\
			wstrFullPath += L".edf";																			\
																												\
			CNtlFileSerializer dataSerializer;																	\
			table_object_pointer->SaveToBinary(dataSerializer);													\
																												\
			int nDataSize = dataSerializer.GetDataSize();														\
			serializer << nDataSize;																			\
			serializer.In(dataSerializer.GetData(), nDataSize);													\
																												\
			serializer.SaveFile((WCHAR*)(wstrFullPath.c_str()), true, L"KEY_FOR_GAME_DATA_TABLE");			\
		}																										\
	}

#define DBO_EXPORT_REGISTERED_TABLE(table_class_name, serializer, file_name_without_extension, need_to_encrypt, table_map, tblidx)		\
	{																																	\
		std::map<TBLIDX, table_class_name*>::iterator iter;																				\
		iter = table_map.find(tblidx);																									\
																																		\
		if (table_map.end() != iter)																									\
		{																																\
			table_class_name* pTable = iter->second;																					\
			if (NULL != pTable)																											\
			{																															\
				DBO_EXPORT_TABLE(pTable, serializer, file_name_without_extension, need_to_encrypt);										\
			}																															\
		}																																\
	}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CTableContainer::CTableContainer(void)
{
	Init();	
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CTableContainer::~CTableContainer(void)
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CTableContainer::Init()
{
	m_pExpTable = NULL;
	m_pHelpTable = NULL;
	m_pMerchantTable = NULL;
	m_pMobTable = NULL;
	m_pNewbieTable = NULL;
	m_pNpcTable = NULL;
	m_pPcTable = NULL;
	m_pStatusTransformTable = NULL;
	m_pGuideHintTable = NULL;

	//new
	m_pCharTitleTable = NULL;
	m_pDwcTable = NULL;
	m_pDwcMissionTable = NULL;
	m_pItemBagListTable = NULL;
	m_pItemEnchantTable = NULL;
	m_pItemGroupListTable = NULL;
	m_pMascotTable = NULL;
	m_pQuestRewardSelectTable = NULL;
	m_pMascotGradeTable = NULL;
	m_pMascotStatusTable = NULL;
	m_pItemMixExpTable = NULL;
	m_pItemUpgradeRateNewTable = NULL;
	m_pAirCostumeTable = NULL;
	m_pCommonConfigTable = NULL;
	m_pNpcServerTable = NULL;
	m_pMobServerTable = NULL;
	m_pEventSystemTable = NULL;
	m_pDynamicFieldSystemTable = NULL;

	m_pItemOptionTable = NULL;
	m_pItemTable = NULL;

	m_pUseItemTable = NULL;
	m_pSetItemTable = NULL;

	m_pDragonBallTable = NULL;
	m_pDragonBallRewardTable = NULL;
	m_pDragonBallReturnPointTable = NULL;

	m_pActionTable = NULL;
	m_pChatCommandTable = NULL;
	m_pDirectionLinkTable = NULL;
	m_pFormulaTable = NULL;
	m_pServerConfigTable = NULL;

	m_pCharmTable = NULL;

	m_pQuestDropTable = NULL;
	m_pQuestItemTable = NULL;
	m_pQuestProbalityTable = NULL;
	m_pQuestTextDataTable = NULL;
	m_pQuestRewardTable = NULL;

	m_pHTBSetTable = NULL;
	m_pSkillTable = NULL;
	m_pSystemEffectTable = NULL;

	m_pTextAllTable = NULL;
	m_pChattingFilterTable = NULL;

	m_mapObjectTable.clear();
	m_mapNpcSpawnTable.clear();
	m_mapMobSpawnTable.clear();
	m_pWorldTable = NULL;
	m_pWorldZoneTable = NULL;
	m_pWorldPlayTable = NULL;

	m_pWorldMapTable = NULL;
	m_pLandMarkTable = NULL;

	m_pTimeQuestTable = NULL;
	m_pRankBattleTable = NULL;
	m_pBudokaiTable = NULL;

	m_pCharDataAnimTable = NULL;
	m_pObjDataAnimTable = NULL;

	m_pWorldPathTable = NULL;

	m_pPortalTable = NULL;

	m_pNpcSpeechTable = NULL;

	m_pScriptLinkTable = NULL;
	m_pItemDisassembleTable = NULL;
	m_pDungeonTable = NULL;

	m_pQuestNarrationTable = NULL;

	m_pVehicleTable = NULL;

	m_pItemRecipeTable = NULL;

	m_pDynamicObjectTable = NULL;

	m_pMobMovePatternTable = NULL;

	m_pDojoTable = NULL;

	m_pItemMixMachineTable = NULL;
	
	m_pHLSItemTable = NULL;
	m_pSlotMachineTable = NULL;
	m_pHlsSlotMachineItemTable = NULL;
}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::Create(CNtlBitFlagManager& rTableFlag, char* pszPath, CTableFileNameList* pFileNameList, CTable::eLOADING_METHOD eLoadingMethod, DWORD dwCodePage, ICallBack* pCall)
{
	WCHAR* pwszPath = Ntl_MB2WC(pszPath);
	if (NULL == pwszPath)
	{
		return false;
	}

	bool bResult = Create(rTableFlag, pwszPath, pFileNameList, eLoadingMethod, dwCodePage, pCall);

	Ntl_CleanUpHeapStringW(pwszPath);

	return bResult;
}

void CTableContainer::SetPath(char* pszPath)
{
	WCHAR* pwszPath = Ntl_MB2WC(pszPath);
	if (NULL == pwszPath)
	{
		return;
	}
	SetPath(pwszPath);
	Ntl_CleanUpHeapStringW(pwszPath);
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::Create(CNtlBitFlagManager& rTableFlag, WCHAR* pwszPath, CTableFileNameList* pFileNameList, CTable::eLOADING_METHOD eLoadingMethod, DWORD dwCodePage, ICallBack* pCall)
{
	m_wstrPath = pwszPath;
	m_eLoadingMethod = eLoadingMethod;
	m_dwCodePage = dwCodePage;

	CNtlFileSerializer serializer;

	//--------------------------------------------------------------------------------
	// Character
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_EXP))
	{
		DBO_CREATE_TABLE(CExpTable, serializer, pFileNameList->GetFileNameW(TABLE_EXP), m_pExpTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_HELP))
	{
		DBO_CREATE_TABLE(CHelpTable, serializer, pFileNameList->GetFileNameW(TABLE_HELP), m_pHelpTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_MERCHANT))
	{
		DBO_CREATE_TABLE(CMerchantTable, serializer, pFileNameList->GetFileNameW(TABLE_MERCHANT), m_pMerchantTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_MOB))
	{
		DBO_CREATE_TABLE(CMobTable, serializer, pFileNameList->GetFileNameW(TABLE_MOB), m_pMobTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_NEWBIE))
	{
		DBO_CREATE_TABLE(CNewbieTable, serializer, pFileNameList->GetFileNameW(TABLE_NEWBIE), m_pNewbieTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_NPC))
	{
		DBO_CREATE_TABLE(CNPCTable, serializer, pFileNameList->GetFileNameW(TABLE_NPC), m_pNpcTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_PC))
	{
		DBO_CREATE_TABLE(CPCTable, serializer, pFileNameList->GetFileNameW(TABLE_PC), m_pPcTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_STATUS_TRANSFORM))
	{
		DBO_CREATE_TABLE(CStatusTransformTable, serializer, pFileNameList->GetFileNameW(TABLE_STATUS_TRANSFORM), m_pStatusTransformTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_GUIDE_HINT))
	{
		DBO_CREATE_TABLE(CGuideHintTable, serializer, pFileNameList->GetFileNameW(TABLE_GUIDE_HINT), m_pGuideHintTable, pCall);
	}
	//-NEW----------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_CHARTITLE))
	{
		DBO_CREATE_TABLE(CCharTitleTable, serializer, pFileNameList->GetFileNameW(TABLE_CHARTITLE), m_pCharTitleTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_DWC))
	{
		DBO_CREATE_TABLE(CDwcTable, serializer, pFileNameList->GetFileNameW(TABLE_DWC), m_pDwcTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_DWCMISSION))
	{
		DBO_CREATE_TABLE(CDwcMissionTable, serializer, pFileNameList->GetFileNameW(TABLE_DWCMISSION), m_pDwcMissionTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_ITEM_BAG_LIST))
	{
		DBO_CREATE_TABLE(CItemBagListTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_BAG_LIST), m_pItemBagListTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_ITEM_ENCHANT))
	{
		DBO_CREATE_TABLE(CItemEnchantTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_ENCHANT), m_pItemEnchantTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_ITEM_GROUP_LIST))
	{
		DBO_CREATE_TABLE(CItemGroupListTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_GROUP_LIST), m_pItemGroupListTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_ITEM_UPGRADE_RATE_NEW))
	{
		DBO_CREATE_TABLE(CItemUpgradeRateNewTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_UPGRADE_RATE_NEW), m_pItemUpgradeRateNewTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_MASCOT))
	{
		DBO_CREATE_TABLE(CMascotTable, serializer, pFileNameList->GetFileNameW(TABLE_MASCOT), m_pMascotTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_QUEST_REWARD_SELECT))
	{
		DBO_CREATE_TABLE(CQuestRewardSelectTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_REWARD_SELECT), m_pQuestRewardSelectTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_MASCOT_GRADE))
	{
		DBO_CREATE_TABLE(CMascotGradeTable, serializer, pFileNameList->GetFileNameW(TABLE_MASCOT_GRADE), m_pMascotGradeTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_MASCOT_STATUS))
	{
		DBO_CREATE_TABLE(CMascotStatusTable, serializer, pFileNameList->GetFileNameW(TABLE_MASCOT_STATUS), m_pMascotStatusTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_ITEM_MIX_EXP))
	{
		DBO_CREATE_TABLE(CItemMixExpTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_MIX_EXP), m_pItemMixExpTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_AIR_COSTUME))
	{
		DBO_CREATE_TABLE(CAirCostumeTable, serializer, pFileNameList->GetFileNameW(TABLE_AIR_COSTUME), m_pAirCostumeTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_COMMON_CONFIG))
	{
		DBO_CREATE_TABLE(CCommonConfigTable, serializer, pFileNameList->GetFileNameW(TABLE_COMMON_CONFIG), m_pCommonConfigTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_NPC_SERVER))
	{
		DBO_CREATE_TABLE(CNpcServerTable, serializer, pFileNameList->GetFileNameW(TABLE_NPC_SERVER), m_pNpcServerTable, pCall);
	}
#ifdef _IS_SERVER_
	if (false != rTableFlag.IsSet(TABLE_MOB_SERVER))
	{
		DBO_CREATE_TABLE(CMobServerTable, serializer, pFileNameList->GetFileNameW(TABLE_MOB_SERVER), m_pMobServerTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_EVENT_SYSTEM))
	{
		DBO_CREATE_TABLE(CEventSystemTable, serializer, pFileNameList->GetFileNameW(TABLE_EVENT_SYSTEM), m_pEventSystemTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_DYNAMIC_FIELD_SYSTEM))
	{
		DBO_CREATE_TABLE(CDynamicFieldSystemTable, serializer, pFileNameList->GetFileNameW(TABLE_DYNAMIC_FIELD_SYSTEM), m_pDynamicFieldSystemTable, pCall);
	}
#endif
	//--------------------------------------------------------------------------------



	//--------------------------------------------------------------------------------
	// Item
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_ITEM_OPTION))
	{
		DBO_CREATE_TABLE(CItemOptionTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_OPTION), m_pItemOptionTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_ITEM))
	{
		DBO_CREATE_TABLE(CItemTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM), m_pItemTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_USE_ITEM))
	{
		DBO_CREATE_TABLE(CUseItemTable, serializer, pFileNameList->GetFileNameW(TABLE_USE_ITEM), m_pUseItemTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_SET_ITEM))
	{
		DBO_CREATE_TABLE(CSetItemTable, serializer, pFileNameList->GetFileNameW(TABLE_SET_ITEM), m_pSetItemTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_DRAGONBALL))
	{
		DBO_CREATE_TABLE(CDragonBallTable, serializer, pFileNameList->GetFileNameW(TABLE_DRAGONBALL), m_pDragonBallTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_DRAGONBALL_REWARD))
	{
		DBO_CREATE_TABLE(CDragonBallRewardTable, serializer, pFileNameList->GetFileNameW(TABLE_DRAGONBALL_REWARD), m_pDragonBallRewardTable, pCall);
	}
#ifdef _IS_SERVER_
	if (false != rTableFlag.IsSet(TABLE_DRAGONBALL_RETURN_POINT))
	{
		DBO_CREATE_TABLE(CDragonBallReturnPointTable, serializer, pFileNameList->GetFileNameW(TABLE_DRAGONBALL_RETURN_POINT), m_pDragonBallReturnPointTable, pCall);
	}
#endif
	if (false != rTableFlag.IsSet(TABLE_ITEM_DISASSEMBLE))
	{
		DBO_CREATE_TABLE(CItemDisassembleTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_DISASSEMBLE), m_pItemDisassembleTable, pCall);
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Misc
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_ACTION))
	{
		DBO_CREATE_TABLE(CActionTable, serializer, pFileNameList->GetFileNameW(TABLE_ACTION), m_pActionTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_CHAT_COMMAND))
	{
		DBO_CREATE_TABLE(CChatCommandTable, serializer, pFileNameList->GetFileNameW(TABLE_CHAT_COMMAND), m_pChatCommandTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_DIRECTION_LINK))
	{
		DBO_CREATE_TABLE(CDirectionLinkTable, serializer, pFileNameList->GetFileNameW(TABLE_DIRECTION_LINK), m_pDirectionLinkTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_FORMULA))
	{
		DBO_CREATE_TABLE(CFormulaTable, serializer, pFileNameList->GetFileNameW(TABLE_FORMULA), m_pFormulaTable, pCall);
	}
#ifdef _IS_SERVER_
	if (false != rTableFlag.IsSet(TABLE_SERVERCONFIG))
	{
		DBO_CREATE_TABLE(CServerConfigTable, serializer, pFileNameList->GetFileNameW(TABLE_SERVERCONFIG), m_pServerConfigTable, pCall);
	}
#endif
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Party
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_CHARM))
	{
		DBO_CREATE_TABLE(CCharmTable, serializer, pFileNameList->GetFileNameW(TABLE_CHARM), m_pCharmTable, pCall);
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Quest
	//--------------------------------------------------------------------------------
#ifdef _IS_SERVER_
	if (false != rTableFlag.IsSet(TABLE_QUEST_DROP))
	{
		DBO_CREATE_TABLE(CQuestDropTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_DROP), m_pQuestDropTable, pCall);
	}
#endif
	if (false != rTableFlag.IsSet(TABLE_QUEST_ITEM))
	{
		DBO_CREATE_TABLE(CQuestItemTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_ITEM), m_pQuestItemTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_QUEST_PROBABILITY))
	{
		DBO_CREATE_TABLE(CQuestProbabilityTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_PROBABILITY), m_pQuestProbalityTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_QUEST_TEXT_DATA))
	{
		DBO_CREATE_TABLE(CQuestTextDataTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_TEXT_DATA), m_pQuestTextDataTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_QUEST_REWARD))
	{
		DBO_CREATE_TABLE(CQuestRewardTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_REWARD), m_pQuestRewardTable, pCall);
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Skill
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_HTB_SET))
	{
		DBO_CREATE_TABLE(CHTBSetTable, serializer, pFileNameList->GetFileNameW(TABLE_HTB_SET), m_pHTBSetTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_SKILL))
	{
		DBO_CREATE_TABLE(CSkillTable, serializer, pFileNameList->GetFileNameW(TABLE_SKILL), m_pSkillTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_SYSTEM_EFFECT))
	{
		DBO_CREATE_TABLE(CSystemEffectTable, serializer, pFileNameList->GetFileNameW(TABLE_SYSTEM_EFFECT), m_pSystemEffectTable, pCall);
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Text
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_TEXT_ALL))
	{
		DBO_CREATE_TABLE(CTextAllTable, serializer, pFileNameList->GetFileNameW(TABLE_TEXT_ALL), m_pTextAllTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_CHATTING_FILTER))
	{
		DBO_CREATE_TABLE(CChattingFilterTable, serializer, pFileNameList->GetFileNameW(TABLE_CHATTING_FILTER), m_pChattingFilterTable, pCall);
	}

	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// World
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_WORLD))
	{
		DBO_CREATE_TABLE(CWorldTable, serializer, pFileNameList->GetFileNameW(TABLE_WORLD), m_pWorldTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_WORLD_ZONE))
	{
		DBO_CREATE_TABLE(CWorldZoneTable, serializer, pFileNameList->GetFileNameW(TABLE_WORLD_ZONE), m_pWorldZoneTable, pCall);
	}
#ifdef _IS_SERVER_
	if (false != rTableFlag.IsSet(TABLE_WORLD_PLAY))
	{
		DBO_CREATE_TABLE(CWorldPlayTable, serializer, pFileNameList->GetFileNameW(TABLE_WORLD_PLAY), m_pWorldPlayTable, pCall);
	}
#endif
	if (false != rTableFlag.IsSet(TABLE_OBJECT))
	{
		for (CTable::TABLEIT iter = m_pWorldTable->Begin(); m_pWorldTable->End() != iter; iter++)
		{
			sWORLD_TBLDAT* pWorldTableData = (sWORLD_TBLDAT*)(iter->second);

			if(wcslen(pWorldTableData->wszObjSpawn_Table_Name) > 0)
				DBO_CREATE_AND_REGISTER_TABLE(CObjectTable, serializer, pWorldTableData->wszObjSpawn_Table_Name, m_mapObjectTable, pWorldTableData->tblidx, pCall);
		}
	}
	if (false != rTableFlag.IsSet(TABLE_NPC_SPAWN))
	{
		for (CTable::TABLEIT iter = m_pWorldTable->Begin(); m_pWorldTable->End() != iter; iter++)
		{
			sWORLD_TBLDAT* pWorldTableData = (sWORLD_TBLDAT*)(iter->second);

			if (wcslen(pWorldTableData->wszNpcSpawn_Table_Name) > 0)
				DBO_CREATE_AND_REGISTER_TABLE(CSpawnTable, serializer, pWorldTableData->wszNpcSpawn_Table_Name, m_mapNpcSpawnTable, pWorldTableData->tblidx, pCall);
		}
	}
	if (false != rTableFlag.IsSet(TABLE_MOB_SPAWN))
	{
		for (CTable::TABLEIT iter = m_pWorldTable->Begin(); m_pWorldTable->End() != iter; iter++)
		{
			sWORLD_TBLDAT* pWorldTableData = (sWORLD_TBLDAT*)(iter->second);

			if (wcslen(pWorldTableData->wszMobSpawn_Table_Name) > 0)
				DBO_CREATE_AND_REGISTER_TABLE(CSpawnTable, serializer, pWorldTableData->wszMobSpawn_Table_Name, m_mapMobSpawnTable, pWorldTableData->tblidx, pCall);
		}
	}
	if (false != rTableFlag.IsSet(TABLE_WORLD_MAP))
	{
		DBO_CREATE_TABLE(CWorldMapTable, serializer, pFileNameList->GetFileNameW(TABLE_WORLD_MAP), m_pWorldMapTable, pCall);
	}
	if (false != rTableFlag.IsSet(TABLE_LAND_MARK))
	{
		DBO_CREATE_TABLE(CLandMarkTable, serializer, pFileNameList->GetFileNameW(TABLE_LAND_MARK), m_pLandMarkTable, pCall);
	}

	//--------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_TIMEQUEST))
	{
		DBO_CREATE_TABLE(CTimeQuestTable, serializer, pFileNameList->GetFileNameW(TABLE_TIMEQUEST), m_pTimeQuestTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_RANKBATTLE))
	{
		DBO_CREATE_TABLE(CRankBattleTable, serializer, pFileNameList->GetFileNameW(TABLE_RANKBATTLE), m_pRankBattleTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_BUDOKAI))
	{
		DBO_CREATE_TABLE(CBudokaiTable, serializer, pFileNameList->GetFileNameW(TABLE_BUDOKAI), m_pBudokaiTable, pCall);
	}

#ifdef _IS_SERVER_
	if (false != rTableFlag.IsSet(TABLE_PASS_POINT))
	{
		if (NULL == m_pWorldTable)
		{
			Destroy();
			return false;
		}

		CWorldPathTable * pTable = new CWorldPathTable;
		if (NULL == pTable)
		{
			Destroy();
			return false;
		}

		if (false == pTable->Create(pFileNameList->GetFileName(TABLE_PASS_POINT)))
		{
			printf("pTable->Create CWorldPathTable failed \n");
			delete pTable;
			pTable = NULL;

			Destroy();
			return false;
		}


		for (CTable::TABLEIT iter = m_pWorldTable->Begin(); m_pWorldTable->End() != iter; iter++)
		{
			sWORLD_TBLDAT * pWorldTableData = (sWORLD_TBLDAT*)(iter->second);

			if (false == pTable->CreateWorldPath(pWorldTableData->tblidx, pWorldTableData->szName))
			{
				//	printf("pTable->CreateWorldPath failed \n");
				//delete pTable;
				//pTable = NULL;

				//Destroy();
				//return false;
			}
		}

		m_pWorldPathTable = pTable;
	}

	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// GraphicData
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_MODEL_TOOL_CHAR))
	{
		if (NULL == m_pPcTable)
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}

		CModelToolCharDataTable* pTable = new CModelToolCharDataTable;
		if (NULL == pTable)
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}
		if (false == pTable->Create(pFileNameList->GetFileName(TABLE_MODEL_TOOL_CHAR), false, NULL))
		{
			delete pTable;

			//- yoshiki : To log system!
			Destroy();
			return false;
		}

		m_pCharDataAnimTable = pTable;
	}
	if (false != rTableFlag.IsSet(TABLE_MODEL_TOOL_OBJ))
	{
		if (NULL == m_pPcTable)
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}

		CModelToolObjDataTable* pTable = new CModelToolObjDataTable;
		if (NULL == pTable)
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}
		if (false == pTable->Create(pFileNameList->GetFileName(TABLE_MODEL_TOOL_OBJ)))
		{
			delete pTable;

			//- yoshiki : To log system!
			Destroy();
			return false;
		}

		m_pObjDataAnimTable = pTable;
	}

#endif

	if (false != rTableFlag.IsSet(TABLE_PORTAL))
	{
		DBO_CREATE_TABLE(CPortalTable, serializer, pFileNameList->GetFileNameW(TABLE_PORTAL), m_pPortalTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_SPEECH))
	{
		DBO_CREATE_TABLE(CNpcSpeechTable, serializer, pFileNameList->GetFileNameW(TABLE_SPEECH), m_pNpcSpeechTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_SCRIPT_LINK))
	{
		DBO_CREATE_TABLE(CScriptLinkTable, serializer, pFileNameList->GetFileNameW(TABLE_SCRIPT_LINK), m_pScriptLinkTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_DUNGEON))
	{
		DBO_CREATE_TABLE(CDungeonTable, serializer, pFileNameList->GetFileNameW(TABLE_DUNGEON), m_pDungeonTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_QUEST_NARRATION))
	{
		DBO_CREATE_TABLE(CQuestNarrationTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_NARRATION), m_pQuestNarrationTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_VEHICLE))
	{
		DBO_CREATE_TABLE(CVehicleTable, serializer, pFileNameList->GetFileNameW(TABLE_VEHICLE), m_pVehicleTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_ITEM_RECIPE))
	{
		DBO_CREATE_TABLE(CItemRecipeTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_RECIPE), m_pItemRecipeTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_DYNAMIC_OBJECT))
	{
		DBO_CREATE_TABLE(CDynamicObjectTable, serializer, pFileNameList->GetFileNameW(TABLE_DYNAMIC_OBJECT), m_pDynamicObjectTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_MOB_MOVE_PATTERN))
	{
		DBO_CREATE_TABLE(CMobMovePatternTable, serializer, pFileNameList->GetFileNameW(TABLE_MOB_MOVE_PATTERN), m_pMobMovePatternTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_DOJO))
	{
		DBO_CREATE_TABLE(CDojoTable, serializer, pFileNameList->GetFileNameW(TABLE_DOJO), m_pDojoTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_MIX_MACHINE))
	{
		DBO_CREATE_TABLE(CItemMixMachineTable, serializer, pFileNameList->GetFileNameW(TABLE_MIX_MACHINE), m_pItemMixMachineTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_HLS_ITEM))
	{
		DBO_CREATE_TABLE(CHLSItemTable, serializer, pFileNameList->GetFileNameW(TABLE_HLS_ITEM), m_pHLSItemTable, pCall);
	}

	if (false != rTableFlag.IsSet(TABLE_HLS_SLOT_MACHINE))
	{
		DBO_CREATE_TABLE(CSlotMachineTable, serializer, pFileNameList->GetFileNameW(TABLE_HLS_SLOT_MACHINE), m_pSlotMachineTable, pCall);
	}
#ifdef _IS_SERVER_
	if (false != rTableFlag.IsSet(TABLE_HLS_SLOT_MACHINE_ITEM))
	{
		DBO_CREATE_TABLE(CHlsSlotMachineItemTable, serializer, pFileNameList->GetFileNameW(TABLE_HLS_SLOT_MACHINE_ITEM), m_pHlsSlotMachineItemTable, pCall);
	}
#endif

	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CTableContainer::Destroy()
{
	m_wstrPath.clear();
	m_eLoadingMethod = CTable::LOADING_METHOD_INVALID;
	m_dwCodePage = 0;

	//--------------------------------------------------------------------------------
	// Character
	//--------------------------------------------------------------------------------
	if (NULL != m_pExpTable)
	{
		delete m_pExpTable;
		m_pExpTable = NULL;
	}
	if (NULL != m_pHelpTable)
	{
		delete m_pHelpTable;
		m_pHelpTable = NULL;
	}
	if (NULL != m_pMerchantTable)
	{
		delete m_pMerchantTable;
		m_pMerchantTable = NULL;
	}
	if (NULL != m_pMobTable)
	{
		delete m_pMobTable;
		m_pMobTable = NULL;
	}
	if (NULL != m_pNewbieTable)
	{
		delete m_pNewbieTable;
		m_pNewbieTable = NULL;
	}
	if (NULL != m_pNpcTable)
	{
		delete m_pNpcTable;
		m_pNpcTable = NULL;
	}
	if (NULL != m_pPcTable)
	{
		delete m_pPcTable;
		m_pPcTable = NULL;
	}
	if (NULL != m_pStatusTransformTable)
	{
		delete m_pStatusTransformTable;
		m_pStatusTransformTable = NULL;
	}
	if (NULL != m_pGuideHintTable)
	{
		delete m_pGuideHintTable;
		m_pGuideHintTable = NULL;
	}

	//-NEW----------------------------------------------------------------------------
	if (NULL != m_pCharTitleTable)
	{
		delete m_pCharTitleTable;
		m_pCharTitleTable = NULL;
	}
	if (NULL != m_pDwcTable)
	{
		delete m_pDwcTable;
		m_pDwcTable = NULL;
	}
	if (NULL != m_pDwcMissionTable)
	{
		delete m_pDwcMissionTable;
		m_pDwcMissionTable = NULL;
	}
	if (NULL != m_pItemBagListTable)
	{
		delete m_pItemBagListTable;
		m_pItemBagListTable = NULL;
	}
	if (NULL != m_pItemEnchantTable)
	{
		delete m_pItemEnchantTable;
		m_pItemEnchantTable = NULL;
	}
	if (NULL != m_pItemGroupListTable)
	{
		delete m_pItemGroupListTable;
		m_pItemGroupListTable = NULL;
	}
	if (NULL != m_pItemUpgradeRateNewTable)
	{
		delete m_pItemUpgradeRateNewTable;
		m_pItemUpgradeRateNewTable = NULL;
	}
	if (NULL != m_pMascotTable)
	{
		delete m_pMascotTable;
		m_pMascotTable = NULL;
	}
	if (NULL != m_pQuestRewardSelectTable)
	{
		delete m_pQuestRewardSelectTable;
		m_pQuestRewardSelectTable = NULL;
	}
	if (NULL != m_pMascotGradeTable)
	{
		delete m_pMascotGradeTable;
		m_pMascotGradeTable = NULL;
	}
	if (NULL != m_pMascotStatusTable)
	{
		delete m_pMascotStatusTable;
		m_pMascotStatusTable = NULL;
	}
	if (NULL != m_pItemMixExpTable)
	{
		delete m_pItemMixExpTable;
		m_pItemMixExpTable = NULL;
	}
	if (NULL != m_pAirCostumeTable)
	{
		delete m_pAirCostumeTable;
		m_pAirCostumeTable = NULL;
	}
	if (NULL != m_pCommonConfigTable)
	{
		delete m_pCommonConfigTable;
		m_pCommonConfigTable = NULL;
	}
	if (NULL != m_pNpcServerTable)
	{
		delete m_pNpcServerTable;
		m_pNpcServerTable = NULL;
	}
	if (NULL != m_pMobServerTable)
	{
		delete m_pMobServerTable;
		m_pMobServerTable = NULL;
	}
	if (NULL != m_pEventSystemTable)
	{
		delete m_pEventSystemTable;
		m_pEventSystemTable = NULL;
	}
	if (NULL != m_pDynamicFieldSystemTable)
	{
		delete m_pDynamicFieldSystemTable;
		m_pDynamicFieldSystemTable = NULL;
	}

	//--------------------------------------------------------------------------------
	
	//--------------------------------------------------------------------------------
	// Item
	//--------------------------------------------------------------------------------
	if (NULL != m_pItemOptionTable)
	{
		delete m_pItemOptionTable;
		m_pItemOptionTable = NULL;
	}
	if (NULL != m_pItemTable)
	{
		delete m_pItemTable;
		m_pItemTable = NULL;
	}
	if (NULL != m_pUseItemTable)
	{
		delete m_pUseItemTable;
		m_pUseItemTable = NULL;
	}
	if (NULL != m_pSetItemTable)
	{
		delete m_pSetItemTable;
		m_pSetItemTable = NULL;
	}
	if (NULL != m_pDragonBallTable)
	{
		delete m_pDragonBallTable;
		m_pDragonBallTable = NULL;
	}
	if (NULL != m_pDragonBallRewardTable)
	{
		delete m_pDragonBallRewardTable;
		m_pDragonBallRewardTable = NULL;
	}
	if (NULL != m_pDragonBallReturnPointTable)
	{
		delete m_pDragonBallReturnPointTable;
		m_pDragonBallReturnPointTable = NULL;
	}
	if (NULL != m_pItemDisassembleTable)
	{
		delete m_pItemDisassembleTable;
		m_pItemDisassembleTable = NULL;
	}

	//--------------------------------------------------------------------------------
	
	//--------------------------------------------------------------------------------
	// Misc
	//--------------------------------------------------------------------------------
	if (NULL != m_pActionTable)
	{
		delete m_pActionTable;
		m_pActionTable = NULL;
	}
	if (NULL != m_pChatCommandTable)
	{
		delete m_pChatCommandTable;
		m_pChatCommandTable = NULL;
	}
	if (NULL != m_pDirectionLinkTable)
	{
		delete m_pDirectionLinkTable;
		m_pDirectionLinkTable = NULL;
	}
	if (NULL != m_pFormulaTable)
	{
		delete m_pFormulaTable;
		m_pFormulaTable = NULL;
	}
	if (NULL != m_pServerConfigTable)
	{
		delete m_pServerConfigTable;
		m_pServerConfigTable = NULL;
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Party
	//--------------------------------------------------------------------------------
	if (NULL != m_pCharmTable)
	{
		delete m_pCharmTable;
		m_pCharmTable = NULL;
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Quest
	//--------------------------------------------------------------------------------
	if (NULL != m_pQuestDropTable)
	{
		delete m_pQuestDropTable;
		m_pQuestDropTable = NULL;
	}
	if (NULL != m_pQuestItemTable)
	{
		delete m_pQuestItemTable;
		m_pQuestItemTable = NULL;
	}
	if(NULL != m_pQuestProbalityTable)
	{
		delete m_pQuestProbalityTable;
		m_pQuestProbalityTable = NULL;
	}
	if (NULL != m_pQuestTextDataTable)
	{
		delete m_pQuestTextDataTable;
		m_pQuestTextDataTable = NULL;
	}
	if ( NULL != m_pQuestRewardTable )
	{
		delete m_pQuestRewardTable;
		m_pQuestRewardTable = NULL;
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Skill
	//--------------------------------------------------------------------------------
	if (NULL != m_pHTBSetTable)
	{
		delete m_pHTBSetTable;
		m_pHTBSetTable = NULL;
	}
	if (NULL != m_pSkillTable)
	{
		delete m_pSkillTable;
		m_pSkillTable = NULL;
	}
	if (NULL != m_pSystemEffectTable)
	{
		delete m_pSystemEffectTable;
		m_pSystemEffectTable = NULL;
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Text
	//--------------------------------------------------------------------------------
	if (NULL != m_pTextAllTable)
	{
		delete m_pTextAllTable;
		m_pTextAllTable = NULL;
	}
	if (NULL != m_pChattingFilterTable)
	{
		delete m_pChattingFilterTable;
		m_pChattingFilterTable = NULL;
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// World
	//--------------------------------------------------------------------------------
	if (NULL != m_pWorldPathTable)
	{
		delete m_pWorldPathTable;
		m_pWorldPathTable = NULL;
	}
	if (NULL != m_pWorldTable)
	{
		delete m_pWorldTable;
		m_pWorldTable = NULL;
	}
	if (NULL != m_pWorldZoneTable)
	{
		delete m_pWorldZoneTable;
		m_pWorldZoneTable = NULL;
	}

	for (OBJTABLEIT it = m_mapObjectTable.begin() ; it != m_mapObjectTable.end() ; it++)
	{
		delete it->second;
	}
	m_mapObjectTable.clear();

	for (SPAWNTABLEIT it = m_mapNpcSpawnTable.begin() ; it != m_mapNpcSpawnTable.end() ; it++)
	{
		delete it->second;
	}
	m_mapNpcSpawnTable.clear();

	for (SPAWNTABLEIT it = m_mapMobSpawnTable.begin() ; it != m_mapMobSpawnTable.end() ; it++)
	{
		delete it->second;
	}
	m_mapMobSpawnTable.clear();

	if (NULL != m_pWorldMapTable)
	{
		delete m_pWorldMapTable;
		m_pWorldMapTable = NULL;
	}
	if (NULL != m_pLandMarkTable)
	{
		delete m_pLandMarkTable;
		m_pLandMarkTable = NULL;
	}
	if( NULL != m_pWorldPlayTable )
	{
		delete m_pWorldPlayTable;
		m_pWorldPlayTable = NULL;
	}
	//--------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------
	// Rule
	//--------------------------------------------------------------------------------
	if (NULL != m_pTimeQuestTable)
	{
		delete m_pTimeQuestTable;
		m_pTimeQuestTable = NULL;
	}

	if(NULL != m_pRankBattleTable)
	{
		delete m_pRankBattleTable;
		m_pRankBattleTable = NULL;
	}

	if(NULL != m_pBudokaiTable)
	{
		delete m_pBudokaiTable;
		m_pBudokaiTable = NULL;
	}

	if(NULL != m_pDungeonTable)
	{
		delete m_pDungeonTable;
		m_pDungeonTable = NULL;
	}
	//--------------------------------------------------------------------------------
	// GraphicData
	//--------------------------------------------------------------------------------
	if (NULL != m_pCharDataAnimTable)
	{
		delete m_pCharDataAnimTable;
		m_pCharDataAnimTable = NULL;
	}
	if (NULL != m_pObjDataAnimTable)
	{
		delete m_pObjDataAnimTable;
		m_pObjDataAnimTable = NULL;
	}
	//--------------------------------------------------------------------------------
	if (NULL != m_pPortalTable)
	{
		delete m_pPortalTable;
		m_pPortalTable = NULL;
	}
	//--------------------------------------------------------------------------------
	if (NULL != m_pNpcSpeechTable)
	{
		delete m_pNpcSpeechTable;
		m_pNpcSpeechTable = NULL;
	}
	//--------------------------------------------------------------------------------
	if (NULL != m_pScriptLinkTable)
	{
		delete m_pScriptLinkTable;
		m_pScriptLinkTable = NULL;
	}
	//--------------------------------------------------------------------------------
	if (NULL != m_pQuestNarrationTable)
	{
		delete m_pQuestNarrationTable;
		m_pQuestNarrationTable = NULL;
	}
	//--------------------------------------------------------------------------------
	if (NULL != m_pVehicleTable)
	{
		delete m_pVehicleTable;
		m_pVehicleTable = NULL;
	}
	//--------------------------------------------------------------------------------
	if (NULL != m_pItemRecipeTable)
	{
		delete m_pItemRecipeTable;
		m_pItemRecipeTable = NULL;
	}
	//--------------------------------------------------------------------------------
	if (NULL != m_pDynamicObjectTable)
	{
		delete m_pDynamicObjectTable;
		m_pDynamicObjectTable = NULL;
	}
	//--------------------------------------------------------------------------------
	if (NULL != m_pMobMovePatternTable)
	{
		delete m_pMobMovePatternTable;
		m_pMobMovePatternTable = NULL;
	}
	//--------------------------------------------------------------------------------
	if (NULL != m_pDojoTable)
	{
		delete m_pDojoTable;
		m_pDojoTable = NULL;
	}
	//--------------------------------------------------------------------------------
	if (NULL != m_pItemMixMachineTable)
	{
		delete m_pItemMixMachineTable;
		m_pItemMixMachineTable = NULL;
	}
	//--------------------------------------------------------------------------------
	if (NULL != m_pHLSItemTable)
	{
		delete m_pHLSItemTable;
		m_pHLSItemTable = NULL;
	}
	//--------------------------------------------------------------------------------	
	if (NULL != m_pSlotMachineTable)
	{
		delete m_pSlotMachineTable;
		m_pSlotMachineTable = NULL;
	}
	//--------------------------------------------------------------------------------	

}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::Reload(CNtlBitFlagManager& rTableFlag, CTableFileNameList* pFileNameList)
{
	CNtlFileSerializer serializer;

	// Mob
	if (false != rTableFlag.IsSet(TABLE_MOB))
	{
		DBO_RELOAD_TABLE( CNPCTable, serializer, pFileNameList->GetFileNameW(TABLE_MOB), m_pMobTable );
	}

	// Npc
	if (false != rTableFlag.IsSet(TABLE_NPC))
	{
		DBO_RELOAD_TABLE( CNPCTable, serializer, pFileNameList->GetFileNameW(TABLE_NPC), m_pNpcTable );
	}

	// Item
	if (false != rTableFlag.IsSet(TABLE_ITEM))
	{
		DBO_RELOAD_TABLE( CItemTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM), m_pItemTable );
	}

	// ItemOption
	if (false != rTableFlag.IsSet(TABLE_ITEM_OPTION))
	{
		DBO_RELOAD_TABLE( CItemOptionTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_OPTION), m_pItemOptionTable );
	}

	// UseItem
	if (false != rTableFlag.IsSet(TABLE_USE_ITEM))
	{
		DBO_RELOAD_TABLE( CUseItemTable, serializer, pFileNameList->GetFileNameW(TABLE_USE_ITEM), m_pUseItemTable );
	}

	// SetItem
	if (false != rTableFlag.IsSet(TABLE_SET_ITEM))
	{
		DBO_RELOAD_TABLE( CSetItemTable, serializer, pFileNameList->GetFileNameW(TABLE_SET_ITEM), m_pSetItemTable );
	}

	// Merchant
	if (false != rTableFlag.IsSet(TABLE_MERCHANT))
	{
		DBO_RELOAD_TABLE( CMerchantTable, serializer, pFileNameList->GetFileNameW(TABLE_MERCHANT), m_pMerchantTable );
	}

	// Skill
	if (false != rTableFlag.IsSet(TABLE_SKILL))
	{
		DBO_RELOAD_TABLE( CSkillTable, serializer, pFileNameList->GetFileNameW(TABLE_SKILL), m_pSkillTable );
	}

	// Object
	if (false != rTableFlag.IsSet(TABLE_OBJECT))
	{
		for (CTable::TABLEIT iter = m_pWorldTable->Begin() ; m_pWorldTable->End() != iter ; iter++)
		{
			sWORLD_TBLDAT * pWorldTableData = (sWORLD_TBLDAT*) iter->second;

			CObjectTable * pObjectTable = GetObjectTable( pWorldTableData->tblidx );
			if( pObjectTable )
			{
				DBO_RELOAD_TABLE( CObjectTable, serializer, pWorldTableData->wszObjSpawn_Table_Name, pObjectTable );
			}
		}
	}


	// DirectionLienk
	if (false != rTableFlag.IsSet(TABLE_DIRECTION_LINK))
	{
		DBO_RELOAD_TABLE( CDirectionLinkTable, serializer, pFileNameList->GetFileNameW(TABLE_DIRECTION_LINK), m_pDirectionLinkTable );
	}

	// Formula
	if (false != rTableFlag.IsSet(TABLE_FORMULA))
	{
		DBO_RELOAD_TABLE( CFormulaTable, serializer, pFileNameList->GetFileNameW(TABLE_FORMULA), m_pFormulaTable );
	}

	if (false != rTableFlag.IsSet(TABLE_SCRIPT_LINK))
	{
		DBO_RELOAD_TABLE( CScriptLinkTable, serializer, pFileNameList->GetFileNameW(TABLE_SCRIPT_LINK), m_pScriptLinkTable );
	}

	if (false != rTableFlag.IsSet(TABLE_DUNGEON))
	{
		DBO_RELOAD_TABLE( CDungeonTable, serializer, pFileNameList->GetFileNameW(TABLE_DUNGEON), m_pDungeonTable );
	}

	if (false != rTableFlag.IsSet(TABLE_QUEST_NARRATION))
	{
		DBO_RELOAD_TABLE( CQuestNarrationTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_NARRATION), m_pQuestNarrationTable );
	}

	if (false != rTableFlag.IsSet(TABLE_VEHICLE))
	{
		DBO_RELOAD_TABLE( CVehicleTable, serializer, pFileNameList->GetFileNameW(TABLE_VEHICLE), m_pVehicleTable );
	}

	if (false != rTableFlag.IsSet(TABLE_ITEM_RECIPE))
	{
		DBO_RELOAD_TABLE( CItemRecipeTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_RECIPE), m_pItemRecipeTable );
	}

	if (false != rTableFlag.IsSet(TABLE_DYNAMIC_OBJECT))
	{
		DBO_RELOAD_TABLE( CDynamicObjectTable, serializer, pFileNameList->GetFileNameW(TABLE_DYNAMIC_OBJECT), m_pDynamicObjectTable );
	}

	if (false != rTableFlag.IsSet(TABLE_MOB_MOVE_PATTERN))
	{
		DBO_RELOAD_TABLE( CMobMovePatternTable, serializer, pFileNameList->GetFileNameW(TABLE_MOB_MOVE_PATTERN), m_pMobMovePatternTable );
	}

	if (false != rTableFlag.IsSet(TABLE_DOJO))
	{
		DBO_RELOAD_TABLE( CDojoTable, serializer, pFileNameList->GetFileNameW(TABLE_DOJO), m_pDojoTable);
	}

	if (false != rTableFlag.IsSet(TABLE_WORLD_MAP))
	{
		DBO_RELOAD_TABLE( CWorldMapTable, serializer, pFileNameList->GetFileNameW(TABLE_WORLD_MAP), m_pWorldMapTable);
	}

	if (false != rTableFlag.IsSet(TABLE_LAND_MARK))
	{
		DBO_RELOAD_TABLE( CLandMarkTable, serializer, pFileNameList->GetFileNameW(TABLE_LAND_MARK), m_pLandMarkTable);
	}

	if (false != rTableFlag.IsSet(TABLE_MIX_MACHINE))
	{
		DBO_RELOAD_TABLE( CItemMixMachineTable, serializer, pFileNameList->GetFileNameW(TABLE_MIX_MACHINE), m_pItemMixMachineTable );
	}
	if (false != rTableFlag.IsSet(TABLE_HLS_ITEM))
	{
		DBO_RELOAD_TABLE( CHLSItemTable, serializer, pFileNameList->GetFileNameW(TABLE_HLS_ITEM), m_pHLSItemTable);
	}

	if (false != rTableFlag.IsSet(TABLE_HLS_SLOT_MACHINE_ITEM))
	{
		DBO_RELOAD_TABLE(CHlsSlotMachineTableItem, serializer, pFileNameList->GetFileNameW(TABLE_HLS_SLOT_MACHINE_ITEM), m_pHlsSlotMachineItemTable);
	}

	return true;
}


bool CTableContainer::Update(CNtlBitFlagManager& rTableFlag, CTableFileNameList* pFileNameList)
{
	CNtlFileSerializer serializer;

	// Mob
	if (false != rTableFlag.IsSet(TABLE_MOB))
	{
		DBO_UPDATE_TABLE(CNPCTable, serializer, pFileNameList->GetFileNameW(TABLE_MOB), m_pMobTable);
	}
	// Npc
	if (false != rTableFlag.IsSet(TABLE_NPC))
	{
		DBO_UPDATE_TABLE(CNPCTable, serializer, pFileNameList->GetFileNameW(TABLE_NPC), m_pNpcTable);
	}

	// Item
	if (false != rTableFlag.IsSet(TABLE_ITEM))
	{
		DBO_UPDATE_TABLE(CItemTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM), m_pItemTable);
	}

	// ItemOption
	if (false != rTableFlag.IsSet(TABLE_ITEM_OPTION))
	{
		DBO_UPDATE_TABLE(CItemOptionTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_OPTION), m_pItemOptionTable);
	}

	// UseItem
	if (false != rTableFlag.IsSet(TABLE_USE_ITEM))
	{
		DBO_UPDATE_TABLE(CUseItemTable, serializer, pFileNameList->GetFileNameW(TABLE_USE_ITEM), m_pUseItemTable);
	}

	// Merchant
	if (false != rTableFlag.IsSet(TABLE_MERCHANT))
	{
		DBO_UPDATE_TABLE(CMerchantTable, serializer, pFileNameList->GetFileNameW(TABLE_MERCHANT), m_pMerchantTable);
	}

	// Item recipe
	if (false != rTableFlag.IsSet(TABLE_ITEM_RECIPE))
	{
		DBO_UPDATE_TABLE(CItemRecipeTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_RECIPE), m_pItemRecipeTable);
	}

	// quest probability
	if (false != rTableFlag.IsSet(TABLE_QUEST_PROBABILITY))
	{
		DBO_UPDATE_TABLE(CQuestProbabilityTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_PROBABILITY), m_pQuestProbalityTable);
	}

	// text all
	if (false != rTableFlag.IsSet(TABLE_TEXT_ALL))
	{
		UpdateTextAllTable(m_pTextAllTable, serializer, pFileNameList->GetFileNameW(TABLE_TEXT_ALL));
	}

	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::SaveToFile(CNtlBitFlagManager& rTableFlag, CTableFileNameList* pFileNameList, bool bNeedToEncrypt)
{
	CNtlFileSerializer serializer;

	//--------------------------------------------------------------------------------
	// Character
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_EXP))
	{
		DBO_EXPORT_TABLE(m_pExpTable, serializer, pFileNameList->GetFileNameW(TABLE_EXP), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_HELP))
	{
		DBO_EXPORT_TABLE(m_pHelpTable, serializer, pFileNameList->GetFileNameW(TABLE_HELP), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_MERCHANT))
	{
		DBO_EXPORT_TABLE(m_pMerchantTable, serializer, pFileNameList->GetFileNameW(TABLE_MERCHANT), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_MOB))
	{
		DBO_EXPORT_TABLE(m_pMobTable, serializer, pFileNameList->GetFileNameW(TABLE_MOB), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_NEWBIE))
	{
		DBO_EXPORT_TABLE(m_pNewbieTable, serializer, pFileNameList->GetFileNameW(TABLE_NEWBIE), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_NPC))
	{
		DBO_EXPORT_TABLE(m_pNpcTable, serializer, pFileNameList->GetFileNameW(TABLE_NPC), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_PC))
	{
		DBO_EXPORT_TABLE(m_pPcTable, serializer, pFileNameList->GetFileNameW(TABLE_PC), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_STATUS_TRANSFORM))
	{
		DBO_EXPORT_TABLE(m_pStatusTransformTable, serializer, pFileNameList->GetFileNameW(TABLE_STATUS_TRANSFORM), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_GUIDE_HINT))
	{
		DBO_EXPORT_TABLE(m_pGuideHintTable, serializer, pFileNameList->GetFileNameW(TABLE_GUIDE_HINT), bNeedToEncrypt);
	}

	//-NEW----------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_CHARTITLE))
	{
		DBO_EXPORT_TABLE(m_pCharTitleTable, serializer, pFileNameList->GetFileNameW(TABLE_CHARTITLE), bNeedToEncrypt);
	}

	if (false != rTableFlag.IsSet(TABLE_DWC))
	{
		DBO_EXPORT_TABLE(m_pDwcTable, serializer, pFileNameList->GetFileNameW(TABLE_DWC), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_DWCMISSION))
	{
		DBO_EXPORT_TABLE(m_pDwcMissionTable, serializer, pFileNameList->GetFileNameW(TABLE_DWCMISSION), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_ITEM_BAG_LIST))
	{
		DBO_EXPORT_TABLE(m_pItemBagListTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_BAG_LIST), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_ITEM_ENCHANT))
	{
		DBO_EXPORT_TABLE(m_pItemEnchantTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_ENCHANT), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_ITEM_GROUP_LIST))
	{
		DBO_EXPORT_TABLE(m_pItemGroupListTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_GROUP_LIST), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_ITEM_UPGRADE_RATE_NEW))
	{
		DBO_EXPORT_TABLE(m_pItemUpgradeRateNewTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_UPGRADE_RATE_NEW), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_MASCOT))
	{
		DBO_EXPORT_TABLE(m_pMascotTable, serializer, pFileNameList->GetFileNameW(TABLE_MASCOT), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_QUEST_REWARD_SELECT))
	{
		DBO_EXPORT_TABLE(m_pQuestRewardSelectTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_REWARD_SELECT), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_MASCOT_GRADE))
	{
		DBO_EXPORT_TABLE(m_pMascotGradeTable, serializer, pFileNameList->GetFileNameW(TABLE_MASCOT_GRADE), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_MASCOT_STATUS))
	{
		DBO_EXPORT_TABLE(m_pMascotStatusTable, serializer, pFileNameList->GetFileNameW(TABLE_MASCOT_STATUS), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_ITEM_MIX_EXP))
	{
		DBO_EXPORT_TABLE(m_pItemMixExpTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_MIX_EXP), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_AIR_COSTUME))
	{
		DBO_EXPORT_TABLE(m_pAirCostumeTable, serializer, pFileNameList->GetFileNameW(TABLE_AIR_COSTUME), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_COMMON_CONFIG))
	{
		DBO_EXPORT_TABLE(m_pCommonConfigTable, serializer, pFileNameList->GetFileNameW(TABLE_COMMON_CONFIG), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_NPC_SERVER))
	{
		DBO_EXPORT_TABLE(m_pNpcServerTable, serializer, pFileNameList->GetFileNameW(TABLE_NPC_SERVER), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_MOB_SERVER))
	{
		DBO_EXPORT_TABLE(m_pMobServerTable, serializer, pFileNameList->GetFileNameW(TABLE_MOB_SERVER), bNeedToEncrypt);
	}
	//--------------------------------------------------------------------------------
	
	
	
	//--------------------------------------------------------------------------------
	// Item
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_ITEM_OPTION))
	{
		DBO_EXPORT_TABLE(m_pItemOptionTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_OPTION), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_ITEM))
	{
		DBO_EXPORT_TABLE(m_pItemTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_USE_ITEM))
	{
		DBO_EXPORT_TABLE(m_pUseItemTable, serializer, pFileNameList->GetFileNameW(TABLE_USE_ITEM), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_SET_ITEM))
	{
		DBO_EXPORT_TABLE(m_pSetItemTable, serializer, pFileNameList->GetFileNameW(TABLE_SET_ITEM), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_DRAGONBALL))
	{
		DBO_EXPORT_TABLE(m_pDragonBallTable, serializer, pFileNameList->GetFileNameW(TABLE_DRAGONBALL), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_DRAGONBALL_REWARD))
	{
		DBO_EXPORT_TABLE(m_pDragonBallRewardTable, serializer, pFileNameList->GetFileNameW(TABLE_DRAGONBALL_REWARD), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_DRAGONBALL_RETURN_POINT))
	{
		DBO_EXPORT_TABLE(m_pDragonBallReturnPointTable, serializer, pFileNameList->GetFileNameW(TABLE_DRAGONBALL_RETURN_POINT), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_ITEM_DISASSEMBLE))
	{
		DBO_EXPORT_TABLE(m_pItemDisassembleTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_DISASSEMBLE), bNeedToEncrypt);
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Misc
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_ACTION))
	{
		DBO_EXPORT_TABLE(m_pActionTable, serializer, pFileNameList->GetFileNameW(TABLE_ACTION), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_CHAT_COMMAND))
	{
		DBO_EXPORT_TABLE(m_pChatCommandTable, serializer, pFileNameList->GetFileNameW(TABLE_CHAT_COMMAND), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_DIRECTION_LINK))
	{
		DBO_EXPORT_TABLE(m_pDirectionLinkTable, serializer, pFileNameList->GetFileNameW(TABLE_DIRECTION_LINK), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_FORMULA))
	{
		DBO_EXPORT_TABLE(m_pFormulaTable, serializer, pFileNameList->GetFileNameW(TABLE_FORMULA), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_SERVERCONFIG))
	{
		DBO_EXPORT_TABLE(m_pServerConfigTable, serializer, pFileNameList->GetFileNameW(TABLE_SERVERCONFIG), bNeedToEncrypt);
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Party
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_CHARM))
	{
		DBO_EXPORT_TABLE(m_pCharmTable, serializer, pFileNameList->GetFileNameW(TABLE_CHARM), bNeedToEncrypt);
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Quest
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_QUEST_DROP))
	{
		DBO_EXPORT_TABLE(m_pQuestDropTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_DROP), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_QUEST_ITEM))
	{
		DBO_EXPORT_TABLE(m_pQuestItemTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_ITEM), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_QUEST_PROBABILITY))
	{
		DBO_EXPORT_TABLE(m_pQuestProbalityTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_PROBABILITY), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_QUEST_TEXT_DATA))
	{
		DBO_EXPORT_TABLE(m_pQuestTextDataTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_TEXT_DATA), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_QUEST_REWARD))
	{
		DBO_EXPORT_TABLE(m_pQuestRewardTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_REWARD), bNeedToEncrypt);
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Skill
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_HTB_SET))
	{
		DBO_EXPORT_TABLE(m_pHTBSetTable, serializer, pFileNameList->GetFileNameW(TABLE_HTB_SET), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_SKILL))
	{
		DBO_EXPORT_TABLE(m_pSkillTable, serializer, pFileNameList->GetFileNameW(TABLE_SKILL), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_SYSTEM_EFFECT))
	{
		DBO_EXPORT_TABLE(m_pSystemEffectTable, serializer, pFileNameList->GetFileNameW(TABLE_SYSTEM_EFFECT), bNeedToEncrypt);
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// Text
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_TEXT_ALL))
	{
		DBO_EXPORT_TABLE(m_pTextAllTable, serializer, pFileNameList->GetFileNameW(TABLE_TEXT_ALL), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_CHATTING_FILTER))
	{
		DBO_EXPORT_TABLE(m_pChattingFilterTable, serializer, pFileNameList->GetFileNameW(TABLE_CHATTING_FILTER), bNeedToEncrypt);
	}
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	// World
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_WORLD))
	{
		DBO_EXPORT_TABLE(m_pWorldTable, serializer, pFileNameList->GetFileNameW(TABLE_WORLD), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_WORLD_ZONE))
	{
		DBO_EXPORT_TABLE(m_pWorldZoneTable, serializer, pFileNameList->GetFileNameW(TABLE_WORLD_ZONE), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_OBJECT))
	{
		for (CTable::TABLEIT iter = m_pWorldTable->Begin() ; m_pWorldTable->End() != iter ; iter++)
		{
			sWORLD_TBLDAT* pWorldTableData = (sWORLD_TBLDAT*)(iter->second);

			DBO_EXPORT_REGISTERED_TABLE(CObjectTable, serializer, pWorldTableData->wszObjSpawn_Table_Name, bNeedToEncrypt, m_mapObjectTable, pWorldTableData->tblidx);
		}
	}
	if (false != rTableFlag.IsSet(TABLE_NPC_SPAWN))
	{
		for (CTable::TABLEIT iter = m_pWorldTable->Begin() ; m_pWorldTable->End() != iter ; iter++)
		{
			sWORLD_TBLDAT* pWorldTableData = (sWORLD_TBLDAT*)(iter->second);
			
			DBO_EXPORT_REGISTERED_TABLE(CSpawnTable, serializer, pWorldTableData->wszNpcSpawn_Table_Name, bNeedToEncrypt, m_mapNpcSpawnTable, pWorldTableData->tblidx);
		}
	}
	if (false != rTableFlag.IsSet(TABLE_MOB_SPAWN))
	{
		for (CTable::TABLEIT it = m_pWorldTable->Begin() ; m_pWorldTable->End() != it ; ++it)
		{
			sWORLD_TBLDAT* pWorldTableData = (sWORLD_TBLDAT*)(it->second);

			DBO_EXPORT_REGISTERED_TABLE(CSpawnTable, serializer, pWorldTableData->wszMobSpawn_Table_Name, bNeedToEncrypt, m_mapMobSpawnTable, pWorldTableData->tblidx);
		}
	}
	if (false != rTableFlag.IsSet(TABLE_WORLD_MAP))
	{
		DBO_EXPORT_TABLE(m_pWorldMapTable, serializer, pFileNameList->GetFileNameW(TABLE_WORLD_MAP), bNeedToEncrypt);
	}
	if (false != rTableFlag.IsSet(TABLE_LAND_MARK))
	{
		DBO_EXPORT_TABLE(m_pLandMarkTable, serializer, pFileNameList->GetFileNameW(TABLE_LAND_MARK), bNeedToEncrypt);
	}
	//--------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------
	if (false != rTableFlag.IsSet(TABLE_TIMEQUEST))
	{
		DBO_EXPORT_TABLE(m_pTimeQuestTable, serializer, pFileNameList->GetFileNameW(TABLE_TIMEQUEST), bNeedToEncrypt);
	}

	if (false != rTableFlag.IsSet(TABLE_RANKBATTLE))
	{
		DBO_EXPORT_TABLE(m_pRankBattleTable, serializer, pFileNameList->GetFileNameW(TABLE_RANKBATTLE), bNeedToEncrypt);
	}

	if (false != rTableFlag.IsSet(TABLE_BUDOKAI))
	{
		DBO_EXPORT_TABLE(m_pBudokaiTable, serializer, pFileNameList->GetFileNameW(TABLE_BUDOKAI), bNeedToEncrypt);
	}


	//--------------------------------------------------------------------------------
	// GraphicData
	//--------------------------------------------------------------------------------
	// 그래픽 데이타는 TableContainer로 export하지 않습니다.
	// Graphic data is not supposed to be exported via TableContainer.
	// by YOSHIKI(2007-04-05)

	if (false != rTableFlag.IsSet(TABLE_PORTAL))
	{
		DBO_EXPORT_TABLE(m_pPortalTable, serializer, pFileNameList->GetFileNameW(TABLE_PORTAL), bNeedToEncrypt);
	}

	if (false != rTableFlag.IsSet(TABLE_SPEECH))
	{
		DBO_EXPORT_TABLE(m_pNpcSpeechTable, serializer, pFileNameList->GetFileNameW(TABLE_SPEECH), bNeedToEncrypt);
	}

	if (false != rTableFlag.IsSet(TABLE_WORLD_PLAY))
	{
		DBO_EXPORT_TABLE(m_pWorldPlayTable, serializer, pFileNameList->GetFileNameW(TABLE_WORLD_PLAY), bNeedToEncrypt);
	}

	if (false != rTableFlag.IsSet(TABLE_SCRIPT_LINK))
	{
		DBO_EXPORT_TABLE( m_pScriptLinkTable, serializer, pFileNameList->GetFileNameW(TABLE_SCRIPT_LINK), bNeedToEncrypt );
	}

	if (false != rTableFlag.IsSet(TABLE_DUNGEON))
	{
		DBO_EXPORT_TABLE( m_pDungeonTable, serializer, pFileNameList->GetFileNameW(TABLE_DUNGEON), bNeedToEncrypt );
	}

	if (false != rTableFlag.IsSet(TABLE_QUEST_NARRATION))
	{
		DBO_EXPORT_TABLE( m_pQuestNarrationTable, serializer, pFileNameList->GetFileNameW(TABLE_QUEST_NARRATION), bNeedToEncrypt );
	}

	if (false != rTableFlag.IsSet(TABLE_VEHICLE))
	{
		DBO_EXPORT_TABLE( m_pVehicleTable, serializer, pFileNameList->GetFileNameW(TABLE_VEHICLE), bNeedToEncrypt );
	}

	if (false != rTableFlag.IsSet(TABLE_ITEM_RECIPE))
	{
		DBO_EXPORT_TABLE( m_pItemRecipeTable, serializer, pFileNameList->GetFileNameW(TABLE_ITEM_RECIPE), bNeedToEncrypt );
	}

	if (false != rTableFlag.IsSet(TABLE_DYNAMIC_OBJECT))
	{
		DBO_EXPORT_TABLE( m_pDynamicObjectTable, serializer, pFileNameList->GetFileNameW(TABLE_DYNAMIC_OBJECT), bNeedToEncrypt );
	}

	if (false != rTableFlag.IsSet(TABLE_MOB_MOVE_PATTERN))
	{
		DBO_EXPORT_TABLE( m_pMobMovePatternTable, serializer, pFileNameList->GetFileNameW(TABLE_MOB_MOVE_PATTERN), bNeedToEncrypt );
	}

	if (false != rTableFlag.IsSet(TABLE_DOJO))
	{
		DBO_EXPORT_TABLE( m_pDojoTable, serializer, pFileNameList->GetFileNameW(TABLE_DOJO), bNeedToEncrypt);
	}

	if (false != rTableFlag.IsSet(TABLE_MIX_MACHINE))
	{
		DBO_EXPORT_TABLE( m_pItemMixMachineTable, serializer, pFileNameList->GetFileNameW(TABLE_MIX_MACHINE), bNeedToEncrypt );
	}

	if (false != rTableFlag.IsSet(TABLE_HLS_ITEM))
	{
		DBO_EXPORT_TABLE( m_pHLSItemTable, serializer, pFileNameList->GetFileNameW(TABLE_HLS_ITEM), bNeedToEncrypt);
	}

	if (false != rTableFlag.IsSet(TABLE_HLS_SLOT_MACHINE))
	{
		DBO_EXPORT_TABLE( m_pSlotMachineTable, serializer, pFileNameList->GetFileNameW(TABLE_HLS_SLOT_MACHINE), bNeedToEncrypt);
	}
	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::ReloadTable(CTable* pTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension)
{
	if (NULL == pszFileNameWithoutExtension)
	{
		return false;
	}

	int iRequiredChars = ::MultiByteToWideChar(m_dwCodePage, 0, pszFileNameWithoutExtension, -1, NULL, 0);
	WCHAR* pwszUnicodeFileName = new WCHAR[iRequiredChars];
	if (NULL == pwszUnicodeFileName)
	{
		return false;
	}

	::MultiByteToWideChar(m_dwCodePage, 0, pszFileNameWithoutExtension, -1, pwszUnicodeFileName, iRequiredChars);

	bool bResult = ReloadTable(pTable, serializer, pwszUnicodeFileName);

	delete [] pwszUnicodeFileName;

	return bResult;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::ReloadTable(CTable* pTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension)
{
	std::wstring wstrFullPath( m_wstrPath );

	wstrFullPath += L"\\";
	wstrFullPath += pwszFileNameWithoutExtension;

	switch ( m_eLoadingMethod )
	{
	case CTable::LOADING_METHOD_XML:
		{
			wstrFullPath += L".xml";

			if ( false == pTable->LoadFromXml( (WCHAR*) wstrFullPath.c_str(), true, false ) )
			{
				return false;
			}
		}
		break;

	case CTable::LOADING_METHOD_BINARY:
		{
			wstrFullPath += L".rdf";
			
			serializer.LoadFile( (WCHAR*) wstrFullPath.c_str(), false );

			if ( false == pTable->LoadFromBinary(serializer, true, false ) )
			{
				return false;
			}
			
		}
		break;

	case CTable::LOADING_METHOD_SECURED_BINARY:
		{
			wstrFullPath += L".edf";

			serializer.LoadFile( (WCHAR*) wstrFullPath.c_str(), true, L"KEY_FOR_GAME_DATA_TABLE" );
			
			int nDataSize = 0;
			serializer >> nDataSize;

			CNtlFileSerializer dataSerializer;
			serializer.Out(dataSerializer, nDataSize);

			if ( false == pTable->LoadFromBinary( dataSerializer, true, false ) )
			{
				return false;
			}
		}
		break;

	default:
		{
			return false;
		}
		break;
	}

	return true;
}



//-----------------------------------------------------------------------------------
//		Purpose	: Update tables from localize folder
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::UpdateTable(CTable* pTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension)
{
	if (NULL == pszFileNameWithoutExtension)
	{
		return false;
	}

	int iRequiredChars = ::MultiByteToWideChar(m_dwCodePage, 0, pszFileNameWithoutExtension, -1, NULL, 0);
	WCHAR* pwszUnicodeFileName = new WCHAR[iRequiredChars];
	if (NULL == pwszUnicodeFileName)
	{
		return false;
	}

	::MultiByteToWideChar(m_dwCodePage, 0, pszFileNameWithoutExtension, -1, pwszUnicodeFileName, iRequiredChars);

	bool bResult = UpdateTable(pTable, serializer, pwszUnicodeFileName);

	delete [] pwszUnicodeFileName;

	return bResult;
}
//-----------------------------------------------------------------------------------
//		Purpose	: Update tables from localize folder
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::UpdateTable(CTable* pTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension)
{
	std::wstring wstrFullPath( m_wstrPath );

	wstrFullPath += L"\\";
	wstrFullPath += pwszFileNameWithoutExtension;

	switch ( m_eLoadingMethod )
	{
	case CTable::LOADING_METHOD_XML:
		{
			wstrFullPath += L".xml";

			if ( false == pTable->LoadFromXml( (WCHAR*) wstrFullPath.c_str(), false, true ) )
			{
				return false;
			}
		}
		break;

	case CTable::LOADING_METHOD_BINARY:
		{
			wstrFullPath += L".rdf";
			
			serializer.LoadFile( (WCHAR*) wstrFullPath.c_str(), false );

			if ( false == pTable->LoadFromBinary(serializer, false, true ) )
			{
				return false;
			}
			
		}
		break;

	case CTable::LOADING_METHOD_SECURED_BINARY:
		{
			wstrFullPath += L".edf";

			serializer.LoadFile( (WCHAR*) wstrFullPath.c_str(), true, L"KEY_FOR_GAME_DATA_TABLE" );
			
			int nDataSize = 0;
			serializer >> nDataSize;

			CNtlFileSerializer dataSerializer;
			serializer.Out(dataSerializer, nDataSize);

			if ( false == pTable->LoadFromBinary( dataSerializer, false, true ) )
			{
				return false;
			}
		}
		break;

	default:
		{
			return false;
		}
		break;
	}

	return true;
}

bool CTableContainer::UpdateTextAllTable(CTextAllTable * pTextAllTable, CNtlFileSerializer & serializer, char * pwszFileNameWithoutExtension)
{
	if (NULL == pwszFileNameWithoutExtension)
	{
		return false;
	}

	int iRequiredChars = ::MultiByteToWideChar(m_dwCodePage, 0, pwszFileNameWithoutExtension, -1, NULL, 0);
	WCHAR* pwszUnicodeFileName = new WCHAR[iRequiredChars];
	if (NULL == pwszUnicodeFileName)
	{
		return false;
	}

	::MultiByteToWideChar(m_dwCodePage, 0, pwszFileNameWithoutExtension, -1, pwszUnicodeFileName, iRequiredChars);

	bool bResult = UpdateTextAllTable(pTextAllTable, serializer, pwszUnicodeFileName);

	delete[] pwszUnicodeFileName;

	return bResult;
}

bool CTableContainer::UpdateTextAllTable(CTextAllTable * pTextAllTable, CNtlFileSerializer & serializer, WCHAR * pwszFileNameWithoutExtension)
{
	std::wstring wstrFullPath;

	wstrFullPath = m_wstrPath;
	wstrFullPath += L"\\";
	wstrFullPath += pwszFileNameWithoutExtension;

	switch (m_eLoadingMethod)
	{
	case CTable::LOADING_METHOD_XML:
	{
		wstrFullPath += L".xml";

		if (false == pTextAllTable->LoadFromXml((WCHAR*)(wstrFullPath.c_str())))
		{
			return false;
		}
	}
	break;

	case CTable::LOADING_METHOD_BINARY:
	{
		wstrFullPath += L".rdf";

		serializer.LoadFile((WCHAR*)(wstrFullPath.c_str()), false);

		if (false == pTextAllTable->LoadFromBinary(serializer, false, true))
		{
			return false;
		}
	}
	break;

	case CTable::LOADING_METHOD_SECURED_BINARY:
	{
		wstrFullPath += L".edf";

		serializer.LoadFile((WCHAR*)(wstrFullPath.c_str()), true, L"KEY_FOR_GAME_DATA_TABLE");

		int nDataSize = 0;
		serializer >> nDataSize;

		CNtlFileSerializer dataSerializer;
		serializer.Out(dataSerializer, nDataSize);

		if (false == pTextAllTable->LoadFromBinary(dataSerializer, false, true))
		{
			return false;
		}
	}
	break;

	default:
	{
		//- yoshiki : To log system!
		return false;
	}
	break;
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::InitializeTable(CTable* pTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension, ICallBack* pCall)
{
	if (NULL == pszFileNameWithoutExtension)
	{
		return false;
	}

	int iRequiredChars = ::MultiByteToWideChar(m_dwCodePage, 0, pszFileNameWithoutExtension, -1, NULL, 0);
	WCHAR* pwszUnicodeFileName = new WCHAR[iRequiredChars];
	if (NULL == pwszUnicodeFileName)
	{
		return false;
	}

	::MultiByteToWideChar(m_dwCodePage, 0, pszFileNameWithoutExtension, -1, pwszUnicodeFileName, iRequiredChars);

	bool bResult = InitializeTable(pTable, serializer, pwszUnicodeFileName, pCall);
	
	delete [] pwszUnicodeFileName;

	return bResult;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::InitializeTable(CTable* pTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension, ICallBack* pCall)
{
	UNREFERENCED_PARAMETER(pCall);
	std::wstring wstrFullPath;

	wstrFullPath = m_wstrPath;
	wstrFullPath += L"\\";
	wstrFullPath += pwszFileNameWithoutExtension;
	
	switch (m_eLoadingMethod)
	{
	case CTable::LOADING_METHOD_XML:
		{
			wstrFullPath += L".xml";

			if ( false == pTable->LoadFromXml( (WCHAR*) wstrFullPath.c_str(), false, false ) )
			{
				return false;
			}
		}
		break;

	case CTable::LOADING_METHOD_BINARY:
		{
			wstrFullPath += L".rdf";

			serializer.LoadFile( (WCHAR*) wstrFullPath.c_str() );

			if (false == pTable->LoadFromBinary( serializer, false, false ) )
			{
				return false;
			}
		}
		break;

	case CTable::LOADING_METHOD_SECURED_BINARY:
		{
			wstrFullPath += L".edf";

			serializer.LoadFile((WCHAR*) wstrFullPath.c_str(), true, L"KEY_FOR_GAME_DATA_TABLE");

			int nDataSize = 0;
			serializer >> nDataSize;

			CNtlFileSerializer dataSerializer;
			serializer.Out(dataSerializer, nDataSize);
		
			if ( false == pTable->LoadFromBinary( dataSerializer, false, false ) )
			{
				printf("failed to load edf \n");
				return false;
			}
		}
		break;

	default:
		{
			//- yoshiki : To log system!
			return false;
		}
		break;
	}

	return true;
}
//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::InitializePackTable(CTable* pTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension, ICallBack* pCall)
{
	if (NULL == pszFileNameWithoutExtension)
	{
		return false;
	}

	int iRequiredChars = ::MultiByteToWideChar(m_dwCodePage, 0, pszFileNameWithoutExtension, -1, NULL, 0);
	WCHAR* pwszUnicodeFileName = new WCHAR[iRequiredChars];
	if (NULL == pwszUnicodeFileName)
	{
		return false;
	}

	::MultiByteToWideChar(m_dwCodePage, 0, pszFileNameWithoutExtension, -1, pwszUnicodeFileName, iRequiredChars);

	bool bResult = InitializePackTable(pTable, serializer, pwszUnicodeFileName, pCall);
	
	delete [] pwszUnicodeFileName;

	return bResult;
}
//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::InitializePackTable(CTable* pTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension, ICallBack* pCall)
{
	std::wstring wstrFullPath;

	wstrFullPath = m_wstrPath;
	wstrFullPath += L"\\";
	wstrFullPath += pwszFileNameWithoutExtension;

	char* pchFileName = Ntl_WC2MB((WCHAR*)wstrFullPath.c_str());
	char* pszCryptPassword = NULL;

	std::string strFullFileName = pchFileName;

	switch ( m_eLoadingMethod )
	{
	case CTable::LOADING_METHOD_XML:
		{
			strFullFileName += ".xml";

			if ( false == pCall->Call(strFullFileName.c_str(), &serializer, pszCryptPassword) )
			{
				return false;
			}

			Ntl_CleanUpHeapString(pchFileName);

			if (false == pTable->LoadFromBinary( serializer, false, false ) )
			{
				return false;
			}
		}
		break;

	case CTable::LOADING_METHOD_BINARY:
		{
			strFullFileName += ".rdf";

			if ( false == pCall->Call(strFullFileName.c_str(), &serializer, pszCryptPassword) )
			{
				return false;
			}

			Ntl_CleanUpHeapString(pchFileName);

			if (false == pTable->LoadFromBinary( serializer, false, false ) )
			{
				return false;
			}
		}
		break;

	case CTable::LOADING_METHOD_SECURED_BINARY:
		{
			strFullFileName += ".edf";
			pszCryptPassword = "KEY_FOR_GAME_DATA_TABLE";

			if (false == pCall->Call(strFullFileName.c_str(), &serializer, pszCryptPassword))
			{
				return false;
			}

			Ntl_CleanUpHeapString(pchFileName);

			if (false == pTable->LoadFromBinary(serializer, false, false))
			{
				return false;
			}
		}
		break;

	default:
		{
			return false;
		}
		break;
	}

	serializer.Refresh(); //this is important or there will be issues when loading multiple tables

	return true;
}
//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::InitializeTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension, ICallBack* pCall)
{
	if (NULL == pszFileNameWithoutExtension)
	{
		return false;
	}

	int iRequiredChars = ::MultiByteToWideChar(m_dwCodePage, 0, pszFileNameWithoutExtension, -1, NULL, 0);
	WCHAR* pwszUnicodeFileName = new WCHAR[iRequiredChars];
	if (NULL == pwszUnicodeFileName)
	{
		return false;
	}

	::MultiByteToWideChar(m_dwCodePage, 0, pszFileNameWithoutExtension, -1, pwszUnicodeFileName, iRequiredChars);

	bool bResult = InitializeTable(pTextAllTable, serializer, pwszUnicodeFileName, pCall);
	
	delete [] pwszUnicodeFileName;

	return bResult;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::InitializeTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension, ICallBack* pCall)
{
	UNREFERENCED_PARAMETER(pCall);
	std::wstring wstrFullPath;

	wstrFullPath = m_wstrPath;
	wstrFullPath += L"\\";
	wstrFullPath += pwszFileNameWithoutExtension;

	switch (m_eLoadingMethod)
	{
	case CTable::LOADING_METHOD_XML:
		{
			wstrFullPath += L".xml";

			if (false == pTextAllTable->LoadFromXml((WCHAR*)(wstrFullPath.c_str())))
			{
				return false;
			}
		}
		break;

	case CTable::LOADING_METHOD_BINARY:
		{
			wstrFullPath += L".rdf";

			serializer.LoadFile((WCHAR*)(wstrFullPath.c_str()), false);

			if (false == pTextAllTable->LoadFromBinary(serializer, false, false))
			{
				return false;
			}
		}
		break;

	case CTable::LOADING_METHOD_SECURED_BINARY:
		{
			wstrFullPath += L".edf";

			serializer.LoadFile((WCHAR*)(wstrFullPath.c_str()), true, L"KEY_FOR_GAME_DATA_TABLE");

			int nDataSize = 0;
			serializer >> nDataSize;
			
			CNtlFileSerializer dataSerializer;
			serializer.Out(dataSerializer, nDataSize);

			if (false == pTextAllTable->LoadFromBinary(dataSerializer, false, false))
			{
				return false;
			}
		}
		break;

	default:
		{
			//- yoshiki : To log system!
			return false;
		}
		break;
	}

	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::InitializePackTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension, ICallBack* pCall)
{
	if (NULL == pszFileNameWithoutExtension)
	{
		return false;
	}

	int iRequiredChars = ::MultiByteToWideChar(m_dwCodePage, 0, pszFileNameWithoutExtension, -1, NULL, 0);
	WCHAR* pwszUnicodeFileName = new WCHAR[iRequiredChars];
	if (NULL == pwszUnicodeFileName)
	{
		return false;
	}

	::MultiByteToWideChar(m_dwCodePage, 0, pszFileNameWithoutExtension, -1, pwszUnicodeFileName, iRequiredChars);

	bool bResult = InitializePackTable(pTextAllTable, serializer, pwszUnicodeFileName, pCall);
	
	delete [] pwszUnicodeFileName;

	return bResult;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableContainer::InitializePackTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension, ICallBack* pCall)
{
	std::wstring wstrFullPath;

	wstrFullPath = m_wstrPath;
	wstrFullPath += L"\\";
	wstrFullPath += pwszFileNameWithoutExtension;

	char* pchFileName = Ntl_WC2MB((WCHAR*)wstrFullPath.c_str());
	char* pszCryptPassword = NULL;

	std::string strFullFileName = pchFileName;

	switch ( m_eLoadingMethod )
	{
	case CTable::LOADING_METHOD_XML:
		{
			strFullFileName += ".xml";

			if ( false == pCall->Call(strFullFileName.c_str(), &serializer, pszCryptPassword) )
			{
				return false;
			}

			Ntl_CleanUpHeapString(pchFileName);

			if (false == pTextAllTable->LoadFromBinary( serializer, false, false) )
			{
				return false;
			}
		}
		break;

	case CTable::LOADING_METHOD_BINARY:
		{
			strFullFileName += ".rdf";

			if ( false == pCall->Call(strFullFileName.c_str(), &serializer, pszCryptPassword) )
			{
				return false;
			}

			Ntl_CleanUpHeapString(pchFileName);

			if (false == pTextAllTable->LoadFromBinary( serializer, false, false ) )
			{
				CTable::CallErrorCallbackFunction("CCTableContainer::InitializePackTable LoadFromBinary fail");
				return false;
			}
		}
		break;

	case CTable::LOADING_METHOD_SECURED_BINARY:
		{
			strFullFileName += ".edf";
			pszCryptPassword = "KEY_FOR_GAME_DATA_TABLE";
			
			if ( false == pCall->Call(strFullFileName.c_str(), &serializer, pszCryptPassword) )
			{
				return false;
			}

			int nDataSize = 0;
			serializer >> nDataSize;

			CNtlFileSerializer dataSerializer;
			serializer.Out(dataSerializer, nDataSize);

			Ntl_CleanUpHeapString(pchFileName);

			if (false == pTextAllTable->LoadFromBinary( dataSerializer, false, false) )
			{
				return false;
			}
		}
		break;

	default:
		{
			return false;
		}
		break;
	}

	serializer.Refresh(); //this is important or there will be issues when loading multiple tables

	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CSpawnTable* CTableContainer::GetNpcSpawnTable(TBLIDX worldTblidx)
{
	SPAWNTABLEIT it = m_mapNpcSpawnTable.find(worldTblidx);
	if (it == m_mapNpcSpawnTable.end())
	{
		return NULL;
	}

	return it->second;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CSpawnTable* CTableContainer::GetMobSpawnTable(TBLIDX worldTblidx)
{
	SPAWNTABLEIT it = m_mapMobSpawnTable.find(worldTblidx);
	if (it == m_mapMobSpawnTable.end())
	{
		return NULL;
	}

	return it->second;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CObjectTable* CTableContainer::GetObjectTable(TBLIDX worldTblidx)
{
	OBJTABLEIT it = m_mapObjectTable.find(worldTblidx);
	if (it == m_mapObjectTable.end())
	{
		return NULL;
	}

	return it->second;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CTableContainer::OBJTABLEIT CTableContainer::BeginObjectTable()
{
	return m_mapObjectTable.begin();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CTableContainer::OBJTABLEIT CTableContainer::EndObjectTable()
{
	return m_mapObjectTable.end();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CTableContainer::SPAWNTABLEIT CTableContainer::BeginNpcSpawnTable()
{
	return m_mapNpcSpawnTable.begin();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CTableContainer::SPAWNTABLEIT CTableContainer::EndNpcSpawnTable()
{
	return m_mapNpcSpawnTable.end();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CTableContainer::SPAWNTABLEIT CTableContainer::BeginMobSpawnTable()
{
	return m_mapMobSpawnTable.begin();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CTableContainer::SPAWNTABLEIT CTableContainer::EndMobSpawnTable()
{
	return m_mapMobSpawnTable.end();
}
