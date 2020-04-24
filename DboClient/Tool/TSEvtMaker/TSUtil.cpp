#include "stdafx.h"
#include "TSUtil.h"
#include "TableContainer.h"
#include "TableFileNameList.h"
#include "NtlXMLDoc.h"
#include "NtlBitFlagManager.h"


CTableContainer* CreateTableContainer( const char* pPath, CTable::eLOADING_METHOD eLoadingMethod )
{
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
	flagManager.Set(CTableContainer::TABLE_PC);					
	flagManager.Set(CTableContainer::TABLE_MOB);				
	flagManager.Set(CTableContainer::TABLE_NPC);				
	flagManager.Set(CTableContainer::TABLE_ITEM);				
	flagManager.Set(CTableContainer::TABLE_ITEM_OPTION);		
	flagManager.Set(CTableContainer::TABLE_SKILL);				
	flagManager.Set(CTableContainer::TABLE_SYSTEM_EFFECT);		
	flagManager.Set(CTableContainer::TABLE_NEWBIE);				
	flagManager.Set(CTableContainer::TABLE_MERCHANT);			
	flagManager.Set(CTableContainer::TABLE_HTB_SET);			
	flagManager.Set(CTableContainer::TABLE_USE_ITEM);			
	flagManager.Set(CTableContainer::TABLE_SET_ITEM);			
	flagManager.Set(CTableContainer::TABLE_CHARM);				
	flagManager.Set(CTableContainer::TABLE_ACTION);				
	flagManager.Set(CTableContainer::TABLE_CHAT_COMMAND);		
	flagManager.Set(CTableContainer::TABLE_QUEST_ITEM);			
	flagManager.Set(CTableContainer::TABLE_QUEST_TEXT_DATA);	
	flagManager.Set(CTableContainer::TABLE_TEXT_ALL);			
	flagManager.Set(CTableContainer::TABLE_OBJECT);			
	flagManager.Set(CTableContainer::TABLE_WORLD_MAP);			
	flagManager.Set(CTableContainer::TABLE_LAND_MARK);			
	flagManager.Set(CTableContainer::TABLE_HELP);				
	flagManager.Set(CTableContainer::TABLE_GUIDE_HINT);			
	flagManager.Set(CTableContainer::TABLE_DRAGONBALL);			
	flagManager.Set(CTableContainer::TABLE_DRAGONBALL_REWARD);	
	flagManager.Set(CTableContainer::TABLE_TIMEQUEST);			
	flagManager.Set(CTableContainer::TABLE_BUDOKAI);			
	flagManager.Set(CTableContainer::TABLE_RANKBATTLE);			
	flagManager.Set(CTableContainer::TABLE_DIRECTION_LINK);		
	flagManager.Set(CTableContainer::TABLE_CHATTING_FILTER);	
	flagManager.Set(CTableContainer::TABLE_PORTAL);				
	flagManager.Set(CTableContainer::TABLE_SPEECH);				
	flagManager.Set(CTableContainer::TABLE_SCRIPT_LINK);		
	flagManager.Set(CTableContainer::TABLE_QUEST_NARRATION);	
	flagManager.Set(CTableContainer::TABLE_VEHICLE);			
	flagManager.Set(CTableContainer::TABLE_DUNGEON);			
	flagManager.Set(CTableContainer::TABLE_MOB_MOVE_PATTERN);	
	flagManager.Set(CTableContainer::TABLE_DYNAMIC_OBJECT);		
	flagManager.Set(CTableContainer::TABLE_ITEM_RECIPE);		
	flagManager.Set(CTableContainer::TABLE_ITEM_UPGRADE);		
	flagManager.Set(CTableContainer::TABLE_MIX_MACHINE);		
	flagManager.Set(CTableContainer::TABLE_DOJO);				
	flagManager.Set(CTableContainer::TABLE_QUEST_REWARD);		
	flagManager.Set(CTableContainer::TABLE_WORLD_ZONE);
	flagManager.Set(CTableContainer::TABLE_NPC_SPAWN);
	flagManager.Set(CTableContainer::TABLE_FORMULA);
	flagManager.Set(CTableContainer::TABLE_GAME_MANIA_TIME);

	fileNameList.SetFileName(CTableContainer::TABLE_WORLD,					"Table_World_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_PC,						"Table_PC_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_MOB,					"Table_MOB_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_NPC,					"Table_NPC_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM,					"Table_Item_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_OPTION,			"Table_Item_Option_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_SKILL,					"Table_Skill_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_SYSTEM_EFFECT,			"Table_System_Effect_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_NEWBIE,					"Table_Newbie_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_MERCHANT,				"Table_Merchant_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_HTB_SET,				"Table_HTB_Set_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_USE_ITEM,				"Table_Use_Item_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_SET_ITEM,				"Table_Set_Item_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_CHARM,					"Table_Charm_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_ACTION,					"Table_Action_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_CHAT_COMMAND,			"Table_Chat_Command_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_ITEM,				"Table_Quest_Item_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_TEXT_DATA,		"Table_Quest_Text_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_TEXT_ALL,				"Table_Text_All_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_OBJECT,					"Table_Object");
	fileNameList.SetFileName(CTableContainer::TABLE_WORLD_MAP,				"Table_Worldmap_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_LAND_MARK,				"Table_Landmark_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_HELP,					"Table_Help_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_GUIDE_HINT,				"Table_Guide_Hint_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_DRAGONBALL,				"Table_Dragon_Ball_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_DRAGONBALL_REWARD,		"Table_DB_Reward_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_TIMEQUEST,				"Table_TMQ_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_BUDOKAI,				"Table_Tenkaichibudokai_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_RANKBATTLE,				"Table_RankBattle_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_DIRECTION_LINK,			"Table_Direction_Link_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_CHATTING_FILTER,		"Table_Chatting_Filter_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_PORTAL,					"Table_Portal_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_SPEECH,					"Table_NPC_Speech_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_SCRIPT_LINK,			"Table_Script_Link_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_NARRATION,		"Table_Quest_Narration_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_VEHICLE,				"Table_Vehicle_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_DUNGEON,				"Table_Dungeon_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_MOB_MOVE_PATTERN,		"Table_Mob_Move_Pattern_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_DYNAMIC_OBJECT,			"Table_Dynamic_Object_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_RECIPE,			"Table_Item_Recipe_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_UPGRADE,			"Table_Item_Upgrade_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_MIX_MACHINE,			"Table_Item_Mix_Machine_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_DOJO,					"Table_Dojo_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_REWARD,			"Table_Quest_Reward_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_WORLD_ZONE,				"Table_World_Zone_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_FORMULA,				"TD_Formula");
	fileNameList.SetFileName(CTableContainer::TABLE_GAME_MANIA_TIME,		"Table_GameManiaTime_Data");

	CTableContainer* pTblContainer = new CTableContainer;

	if ( !pTblContainer->Create( flagManager, (char*)pPath, &fileNameList, eLoadingMethod, GetACP(), NULL) )
	{
		delete pTblContainer;

		return NULL;
	}

	return pTblContainer;
}

void DeleteTableContainer( CTableContainer*& pTblContainer )
{
	if ( pTblContainer )
	{
		pTblContainer->Destroy();
		delete pTblContainer;
		pTblContainer = NULL;
	}
}