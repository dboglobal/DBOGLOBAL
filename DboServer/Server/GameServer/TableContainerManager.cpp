#include "stdafx.h"
#include "TableContainerManager.h"
#include "NtlBitFlagManager.h"
#include "TableFileNameList.h"
#include "GameServer.h"

CTableContainerManager::CTableContainerManager()
{
}

CTableContainerManager::~CTableContainerManager()
{
}



//-----------------------------------------------------------------------------------
//		Loading Tables
//-----------------------------------------------------------------------------------
bool CTableContainerManager::CreateTableContainer(BYTE LoadTableFormat, char* pszPath)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlBitFlagManager flagManager;
    if (false == flagManager.Create(CTableContainer::TABLE_COUNT))
    {
        return false;
    }

    CTableFileNameList fileNameList;
    if (false == fileNameList.Create())
    {
        return false;
    }

	flagManager.Set(CTableContainer::TABLE_WORLD);
	fileNameList.SetFileName(CTableContainer::TABLE_WORLD,					"Table_World_Data");
	flagManager.Set(CTableContainer::TABLE_WORLD_MAP);
	fileNameList.SetFileName(CTableContainer::TABLE_WORLD_MAP,				"Table_Worldmap_Data");
	flagManager.Set(CTableContainer::TABLE_WORLD_ZONE);
	fileNameList.SetFileName(CTableContainer::TABLE_WORLD_ZONE,				"Table_World_Zone_Data");
	flagManager.Set(CTableContainer::TABLE_WORLD_PLAY);//
	fileNameList.SetFileName(CTableContainer::TABLE_WORLD_PLAY,				"Table_World_Play_Data");

	flagManager.Set(CTableContainer::TABLE_OBJECT);
	flagManager.Set(CTableContainer::TABLE_NPC_SPAWN);
	flagManager.Set(CTableContainer::TABLE_MOB_SPAWN);

	flagManager.Set(CTableContainer::TABLE_PC);
	fileNameList.SetFileName(CTableContainer::TABLE_PC,						"Table_PC_Data");
	flagManager.Set(CTableContainer::TABLE_NEWBIE);
	fileNameList.SetFileName(CTableContainer::TABLE_NEWBIE,					"Table_Newbie_Data");

	flagManager.Set(CTableContainer::TABLE_MOB);
	fileNameList.SetFileName(CTableContainer::TABLE_MOB,					"Table_MOB_Data");
	flagManager.Set(CTableContainer::TABLE_NPC);
	fileNameList.SetFileName(CTableContainer::TABLE_NPC,					"Table_NPC_Data");
	flagManager.Set(CTableContainer::TABLE_ITEM);
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM,					"Table_Item_Data");
	flagManager.Set(CTableContainer::TABLE_ITEM_OPTION);
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_OPTION,			"Table_Item_Option_Data");
	
	flagManager.Set(CTableContainer::TABLE_SKILL);
	fileNameList.SetFileName(CTableContainer::TABLE_SKILL,					"Table_Skill_Data");
	flagManager.Set(CTableContainer::TABLE_SYSTEM_EFFECT);
	fileNameList.SetFileName(CTableContainer::TABLE_SYSTEM_EFFECT,			"Table_System_Effect_Data");
	flagManager.Set(CTableContainer::TABLE_TEXT_ALL);
	fileNameList.SetFileName(CTableContainer::TABLE_TEXT_ALL,				"table_text_all_data");
	flagManager.Set(CTableContainer::TABLE_MERCHANT); 
	fileNameList.SetFileName(CTableContainer::TABLE_MERCHANT,				"Table_Merchant_Data");
	flagManager.Set(CTableContainer::TABLE_HTB_SET);
	fileNameList.SetFileName(CTableContainer::TABLE_HTB_SET,				"Table_HTB_Set_Data");
	flagManager.Set(CTableContainer::TABLE_USE_ITEM);
	fileNameList.SetFileName(CTableContainer::TABLE_USE_ITEM,				"Table_Use_Item_Data");
	flagManager.Set(CTableContainer::TABLE_SET_ITEM);
	fileNameList.SetFileName(CTableContainer::TABLE_SET_ITEM,				"Table_Set_Item_Data");
	flagManager.Set(CTableContainer::TABLE_CHARM);
	fileNameList.SetFileName(CTableContainer::TABLE_CHARM,					"Table_Charm_Data");
	flagManager.Set(CTableContainer::TABLE_ACTION);
	fileNameList.SetFileName(CTableContainer::TABLE_ACTION,					"Table_Action_Data");
	flagManager.Set(CTableContainer::TABLE_QUEST_ITEM);
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_ITEM,				"Table_Quest_Item_Data");
	
	flagManager.Set(CTableContainer::TABLE_DRAGONBALL);
	fileNameList.SetFileName(CTableContainer::TABLE_DRAGONBALL,				"Table_Dragon_Ball_Data");
	flagManager.Set(CTableContainer::TABLE_DRAGONBALL_REWARD);
	fileNameList.SetFileName(CTableContainer::TABLE_DRAGONBALL_REWARD,		"Table_DB_Reward_Data");
	flagManager.Set(CTableContainer::TABLE_DRAGONBALL_RETURN_POINT);
	fileNameList.SetFileName(CTableContainer::TABLE_DRAGONBALL_RETURN_POINT,"table_db_returnpoint");
	flagManager.Set(CTableContainer::TABLE_TIMEQUEST);
	fileNameList.SetFileName(CTableContainer::TABLE_TIMEQUEST,				"Table_TMQ_Data");
	flagManager.Set(CTableContainer::TABLE_DIRECTION_LINK);
	fileNameList.SetFileName(CTableContainer::TABLE_DIRECTION_LINK,			"Table_Direction_Link_Data");
	flagManager.Set(CTableContainer::TABLE_PORTAL);
	fileNameList.SetFileName(CTableContainer::TABLE_PORTAL, "Table_Portal_Data");

	flagManager.Set(CTableContainer::TABLE_SPEECH);
	fileNameList.SetFileName(CTableContainer::TABLE_SPEECH, "Table_NPC_Speech_Data");
	flagManager.Set(CTableContainer::TABLE_VEHICLE); 
	fileNameList.SetFileName(CTableContainer::TABLE_VEHICLE,				"Table_Vehicle_Data");
	flagManager.Set(CTableContainer::TABLE_DUNGEON); 
	fileNameList.SetFileName(CTableContainer::TABLE_DUNGEON,				"Table_Dungeon_Data");
	flagManager.Set(CTableContainer::TABLE_DYNAMIC_OBJECT); 
	fileNameList.SetFileName(CTableContainer::TABLE_DYNAMIC_OBJECT,			"Table_Dynamic_Object_Data");
	flagManager.Set(CTableContainer::TABLE_ITEM_RECIPE); 
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_RECIPE,			"Table_Item_Recipe_Data");
	flagManager.Set(CTableContainer::TABLE_MIX_MACHINE);
	fileNameList.SetFileName(CTableContainer::TABLE_MIX_MACHINE,			"Table_Item_Mix_Machine_Data");
	flagManager.Set(CTableContainer::TABLE_DOJO);
	fileNameList.SetFileName(CTableContainer::TABLE_DOJO,					"Table_Dojo_Data");
	flagManager.Set(CTableContainer::TABLE_QUEST_REWARD);
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_REWARD,			"Table_Quest_Reward_Data");
	flagManager.Set(CTableContainer::TABLE_FORMULA);
	fileNameList.SetFileName(CTableContainer::TABLE_FORMULA,				"TD_Formula");
	flagManager.Set(CTableContainer::TABLE_BUDOKAI); 
	fileNameList.SetFileName(CTableContainer::TABLE_BUDOKAI,				"Table_Tenkaichibudokai_Data");
	flagManager.Set(CTableContainer::TABLE_QUEST_DROP);
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_DROP,				"table_quest_drop_data");
	flagManager.Set(CTableContainer::TABLE_QUEST_PROBABILITY);
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_PROBABILITY,		"table_quest_probability_data");
	flagManager.Set(CTableContainer::TABLE_EXP);	
	fileNameList.SetFileName(CTableContainer::TABLE_EXP,					"Table_EXP_Data");
	flagManager.Set(CTableContainer::TABLE_STATUS_TRANSFORM);
	fileNameList.SetFileName(CTableContainer::TABLE_STATUS_TRANSFORM,		"Table_Status_Transform_Data");
	flagManager.Set(CTableContainer::TABLE_RANKBATTLE);
	fileNameList.SetFileName(CTableContainer::TABLE_RANKBATTLE,				"Table_RankBattle_Data");
	
	flagManager.Set(CTableContainer::TABLE_HLS_ITEM);
	fileNameList.SetFileName(CTableContainer::TABLE_HLS_ITEM,				"table_hls_item_data");
	

	flagManager.Set(CTableContainer::TABLE_CHARTITLE); 
	fileNameList.SetFileName(CTableContainer::TABLE_CHARTITLE,					"Table_CharTitle_Data");
	flagManager.Set(CTableContainer::TABLE_DWC);
	fileNameList.SetFileName(CTableContainer::TABLE_DWC,						"Table_DWC_Data");
	flagManager.Set(CTableContainer::TABLE_DWCMISSION);
	fileNameList.SetFileName(CTableContainer::TABLE_DWCMISSION,					"Table_DWC_Mission_Data");
	flagManager.Set(CTableContainer::TABLE_ITEM_ENCHANT);
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_ENCHANT,				"Table_Item_Enchant_Data");
	flagManager.Set(CTableContainer::TABLE_QUEST_REWARD_SELECT);
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_REWARD_SELECT,			"Table_Quest_Reward_Select_Data");
	flagManager.Set(CTableContainer::TABLE_MOB_MOVE_PATTERN);
	fileNameList.SetFileName(CTableContainer::TABLE_MOB_MOVE_PATTERN,					"Table_Mob_Move_Pattern_Data");
	flagManager.Set(CTableContainer::TABLE_ITEM_UPGRADE_RATE_NEW); 
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_UPGRADE_RATE_NEW,			"Table_Item_Upgrade_NewRate_Data");
	flagManager.Set(CTableContainer::TABLE_ITEM_BAG_LIST);
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_BAG_LIST,				"Table_Item_Bag_List_Data");
	flagManager.Set(CTableContainer::TABLE_ITEM_GROUP_LIST);
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_GROUP_LIST,			"Table_Item_Group_List_Data");

	flagManager.Set(CTableContainer::TABLE_AIR_COSTUME); //WORK BUT FIELDS (VALUES) ARE UNKNOWN
	fileNameList.SetFileName(CTableContainer::TABLE_AIR_COSTUME,					"Table_Aircostume_Data");
	flagManager.Set(CTableContainer::TABLE_MASCOT);
	fileNameList.SetFileName(CTableContainer::TABLE_MASCOT,						"Table_Mascot_Data");
	flagManager.Set(CTableContainer::TABLE_MASCOT_GRADE); //WORK BUT FIELDS (VALUES) ARE UNKNOWN
	fileNameList.SetFileName(CTableContainer::TABLE_MASCOT_GRADE,					"Table_Mascot_Grade_Data");
	flagManager.Set(CTableContainer::TABLE_MASCOT_STATUS); //WORK BUT FIELDS (VALUES) ARE UNKNOWN
	fileNameList.SetFileName(CTableContainer::TABLE_MASCOT_STATUS,					"Table_Mascot_Status_Data");
	flagManager.Set(CTableContainer::TABLE_ITEM_MIX_EXP); //WORK BUT FIELDS (VALUES) ARE UNKNOWN
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_MIX_EXP,			"Table_Item_Mix_Exp_Data");
	flagManager.Set(CTableContainer::TABLE_HLS_SLOT_MACHINE); //WORK
	fileNameList.SetFileName(CTableContainer::TABLE_HLS_SLOT_MACHINE,			"table_slot_machine_data");

	flagManager.Set(CTableContainer::TABLE_ITEM_DISASSEMBLE); //WORK
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_DISASSEMBLE,			"table_item_disassemble_data");

	flagManager.Set(CTableContainer::TABLE_NPC_SERVER);
	fileNameList.SetFileName(CTableContainer::TABLE_NPC_SERVER,				"table_npc_data_server");
	flagManager.Set(CTableContainer::TABLE_MOB_SERVER);
	fileNameList.SetFileName(CTableContainer::TABLE_MOB_SERVER,				"table_mob_data_server");

	flagManager.Set(CTableContainer::TABLE_COMMON_CONFIG);
	fileNameList.SetFileName(CTableContainer::TABLE_COMMON_CONFIG,				"Table_Common_Config_Data");


	flagManager.Set(CTableContainer::TABLE_PASS_POINT);		//dont have all path (.pth) files
	fileNameList.SetFileName(CTableContainer::TABLE_PASS_POINT,				".\\resource\\server_data\\world_data\\Path");

	flagManager.Set(CTableContainer::TABLE_SERVERCONFIG);
	fileNameList.SetFileName(CTableContainer::TABLE_SERVERCONFIG,		"table_server_config_data");

	flagManager.Set(CTableContainer::TABLE_EVENT_SYSTEM);
	fileNameList.SetFileName(CTableContainer::TABLE_EVENT_SYSTEM, "Table_Event_System_Data");
	flagManager.Set(CTableContainer::TABLE_DYNAMIC_FIELD_SYSTEM);
	fileNameList.SetFileName(CTableContainer::TABLE_DYNAMIC_FIELD_SYSTEM, "Table_Event_System_Dynamic_Data");

	flagManager.Set(CTableContainer::TABLE_MODEL_TOOL_CHAR);
	fileNameList.SetFileName(CTableContainer::TABLE_MODEL_TOOL_CHAR, ".\\resource\\property");
	//flagManager.Set(CTableContainer::TABLE_MODEL_TOOL_OBJ);
	//fileNameList.SetFileName(CTableContainer::TABLE_MODEL_TOOL_OBJ, ".\\resource\\property");

	//load tables
	Create(flagManager, pszPath, &fileNameList, (CTable::eLOADING_METHOD)LoadTableFormat, GetACP(), NULL);


	return TRUE;
}

