#include "precomp_ntlsimulation.h"
#include "NtlSLApi.h"

// shared
#include "TableContainer.h"
#include "TableFileNameList.h"
#include "NtlXMLDoc.h"
#include "NtlBitFlagManager.h"

// presentation
#include "NtlPLDamageBox.h"
#include "NtlPLResourcePack.h"
#include "NtlPLHelpers.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLGlobal.h"
#include "TableContainer.h"
#include "NtlDTProp.h"
#include "InputActionMap.h"
#include "NtlSobManager.h"
#include "NtlFSMLookAtManager.h"
#include "NtlSLEventHandler.h"
#include "NtlNetSender.h"
#include "NtlSLPacketGenerator.h"
#include "NtlPacketLockManager.h"
#include "NtlTriggerDirectSync.h"
#include "NtlSLLuaGlue.h"
#include "NtlCameraManager.h"
#include "NtlSobCharProxy.h"
#include "NtlSobAvatar.h"
#include "DboTSCQAgency.h"
#include "DboTSCTAgency.h"
#include "NtlClock.h"
#include "NtlWorldConcept.h"
#include "NtlSobIcon.h"
#include "NtlDTCinematicManager.h"
#include "NtlSobElapsedController.h"
#include "NtlSoundMoodManager.h"
#include "NtlSLLogic.h"


MEMORYSTATUS				g_memStaus;

RwUInt32					g_uiSystemMouseClickInterval = (RwUInt32)MAX_MOUSE_CLICK_INTERVAL;
RwReal						g_fCurMouseClickInterval	= 0.f;

SDevMasterInfo				*g_pDevMasterInfo		= NULL;

CTableContainer				*g_pTableContainer		= NULL;

CNtlDTContainer				*g_pDTContainer			= NULL;
CNtlNetSender				*g_pNetSender			= NULL;					
CNtlSLPacketGenerator		*g_pSLPacketGenerator	= NULL;	
CNtlTriggerSyncManager		*g_pTriggerSyncManager	= NULL;	
CNtlPacketLockManager		*g_pPacketLocker		= NULL;		

CInputActionMap				*g_pActionMap			= NULL;
CNtlSobManager				*g_pSobMgr				= NULL;
CNtlFSMLookAtManager		*g_pFSMLookAtManager	= NULL;

CNtlSLGlobal				*g_pSLGlobal			= NULL;
CNtlSLEventHandler			*g_pSLEventHander		= NULL;
CNtlGameCameraManager		*g_pGameCameraManager	= NULL;

CNtlWorldConcept			*g_pWorldConcept		= NULL;
CNtlClock					*g_pGameClock			= NULL;

CNtlDTCinematicManager		*g_pDTCinematicManager	= NULL;

CNtlCamera					*g_pCamera				= NULL;


MEMORYSTATUS* API_GetGlobalMemoryStatus(void)
{
	return &g_memStaus;
}

RwUInt8	API_LoadSLDevMasterInformation(void)
{
	NTL_FUNCTION("API_LoadSLDevMasterInformation");
	
	CNtlXMLDoc doc;
	doc.Create();

	// xml doc load
	if(doc.Load( (char*)"devmaster.xml" ) == false)
	{
		NTL_RETURN(DEVMASTER_NOT_FOUND_FILE);
	}

	char chBuffer[1024];
    
	// master operation을 얻어온다.
	IXMLDOMNode* pNode = doc.SelectSingleNode((char*)"/master_options/op");
	
	if(!doc.GetTextWithAttributeName(pNode, "key", chBuffer, 1024))
	{
		NTL_RETURN(DEVMASTER_INVALID_SERIAL);
	}

	if(strcmp(chBuffer, DEVMASTER_KEY) != 0)
	{
		NTL_RETURN(DEVMASTER_INVALID_SERIAL);
	}

	pNode->Release(); 
	
	g_pDevMasterInfo = NTL_NEW SDevMasterInfo;
	memset(g_pDevMasterInfo, 0, sizeof(SDevMasterInfo));
	g_pDevMasterInfo->bPackFileUse = TRUE;
	g_pDevMasterInfo->bThreadLoading = TRUE;

	// master cheat을 얻어온다.

	pNode = doc.SelectSingleNode((char*)"/master_options/cheat");

	if(!doc.GetTextWithAttributeName(pNode, "cool_time_factor", chBuffer, 1024))
	{
		NTL_RETURN(DEVMASTER_READ_ERROR);
	}

	g_pDevMasterInfo->fCoolingTimeFactor = (RwReal)atof(chBuffer);

	if(!doc.GetTextWithAttributeName(pNode, "data_file_xml", chBuffer, 1024))
	{
		NTL_RETURN(DEVMASTER_READ_ERROR);
	}

	g_pDevMasterInfo->bLoadDataXml = (RwBool)atoi(chBuffer);

	if(!doc.GetTextWithAttributeName(pNode, "pack_file_use", chBuffer, 1024))
	{
		NTL_RETURN(DEVMASTER_READ_ERROR);
	}

	g_pDevMasterInfo->bPackFileUse = (RwBool)atoi(chBuffer);

	if(doc.GetTextWithAttributeName(pNode, "thread_load", chBuffer, 1024))
	{
		g_pDevMasterInfo->bThreadLoading = (RwBool)atoi(chBuffer);
	}
	
	pNode->Release(); 

	// master camera을 얻어온다.
	
	pNode = doc.SelectSingleNode((char*)"/master_options/camera");
	if(!doc.GetTextWithAttributeName(pNode, "fov", chBuffer, 1024))
	{
		NTL_RETURN(DEVMASTER_READ_ERROR);
	}

	g_pDevMasterInfo->fCameraFov = (RwReal)atof(chBuffer);

	if(!doc.GetTextWithAttributeName(pNode, "near", chBuffer, 1024))
	{
		NTL_RETURN(DEVMASTER_READ_ERROR);
	}

	g_pDevMasterInfo->fCameraNear = (RwReal)atof(chBuffer);

	if(!doc.GetTextWithAttributeName(pNode, "far", chBuffer, 1024))
	{
		NTL_RETURN(DEVMASTER_READ_ERROR);
	}

	g_pDevMasterInfo->fCameraFar = (RwReal)atof(chBuffer);

	if(!doc.GetTextWithAttributeName(pNode, "zoom_min", chBuffer, 1024))
	{
		NTL_RETURN(DEVMASTER_READ_ERROR);
	}

	g_pDevMasterInfo->fCameraZoomMin = (RwReal)atof(chBuffer);

	if(!doc.GetTextWithAttributeName(pNode, "zoom_max", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}

	g_pDevMasterInfo->fCameraZoomMax = (RwReal)atof(chBuffer);

	pNode->Release(); 


	// master screen을 얻어온다.

	pNode = doc.SelectSingleNode((char*)"/master_options/screen");
	if(!doc.GetTextWithAttributeName(pNode, "modechange", chBuffer, 1024))
	{
		NTL_RETURN(DEVMASTER_READ_ERROR);
	}

	g_pDevMasterInfo->bScreenChangeMode = (RwBool)atoi(chBuffer);

	pNode->Release(); 

	CNtlSobIcon::SetCoolingUpdateFactor(g_pDevMasterInfo->fCoolingTimeFactor);

	g_pDevMasterInfo->bDevUser = TRUE;
	
	NTL_RETURN(DEVMASTER_SUCCESS);
}

SDevMasterInfo*	API_GetSLDevMasterInformation(void)
{
	return g_pDevMasterInfo;
}


RwBool CreateTableContainer(RwUInt8 byLoadMethod)
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

	flagManager.Set(CTableContainer::TABLE_EXP);
	flagManager.Set(CTableContainer::TABLE_WORLD);
	flagManager.Set(CTableContainer::TABLE_WORLD_MAP);
	flagManager.Set(CTableContainer::TABLE_LAND_MARK);
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
	flagManager.Set(CTableContainer::TABLE_CHARTITLE);
	flagManager.Set(CTableContainer::TABLE_ACTION);
	flagManager.Set(CTableContainer::TABLE_AIR_COSTUME);
	flagManager.Set(CTableContainer::TABLE_CHAT_COMMAND);		
	flagManager.Set(CTableContainer::TABLE_QUEST_ITEM);
	flagManager.Set(CTableContainer::TABLE_QUEST_TEXT_DATA);
	flagManager.Set(CTableContainer::TABLE_TEXT_ALL);
	flagManager.Set(CTableContainer::TABLE_OBJECT);
	flagManager.Set(CTableContainer::TABLE_HELP);
	flagManager.Set(CTableContainer::TABLE_GUIDE_HINT);		
	flagManager.Set(CTableContainer::TABLE_HLS_ITEM);
	flagManager.Set(CTableContainer::TABLE_ITEM_DISASSEMBLE);
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
	flagManager.Set(CTableContainer::TABLE_MIX_MACHINE);		
	flagManager.Set(CTableContainer::TABLE_DOJO);				
	flagManager.Set(CTableContainer::TABLE_QUEST_REWARD);
	flagManager.Set(CTableContainer::TABLE_QUEST_REWARD_SELECT);
	flagManager.Set(CTableContainer::TABLE_WORLD_ZONE);
	flagManager.Set(CTableContainer::TABLE_NPC_SPAWN);
	flagManager.Set(CTableContainer::TABLE_FORMULA);
	flagManager.Set(CTableContainer::TABLE_ITEM_MIX_EXP);
	flagManager.Set(CTableContainer::TABLE_COMMON_CONFIG);
	flagManager.Set(CTableContainer::TABLE_DWC);
	flagManager.Set(CTableContainer::TABLE_ITEM_ENCHANT);
	flagManager.Set(CTableContainer::TABLE_NPC_SERVER);


	fileNameList.SetFileName(CTableContainer::TABLE_EXP,					"table_exp_data");
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
	fileNameList.SetFileName(CTableContainer::TABLE_CHARTITLE,				"Table_CharTitle_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_ACTION,					"Table_Action_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_AIR_COSTUME,			"Table_Aircostume_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_CHAT_COMMAND,			"Table_Chat_Command_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_ITEM,				"Table_Quest_Item_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_TEXT_DATA,		"Table_Quest_Text_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_TEXT_ALL,				"Table_Text_All_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_WORLD_MAP,				"Table_Worldmap_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_LAND_MARK,				"Table_Landmark_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_HELP,					"Table_Help_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_GUIDE_HINT,				"Table_Guide_Hint_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_HLS_ITEM,				"table_hls_item_data");
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_DISASSEMBLE,		"table_item_disassemble_data");
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
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_MIX_EXP,			"Table_Item_Mix_Exp_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_MIX_MACHINE,			"Table_Item_Mix_Machine_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_DOJO,					"Table_Dojo_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_REWARD,			"Table_Quest_Reward_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_QUEST_REWARD_SELECT,	"Table_Quest_Reward_Select_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_WORLD_ZONE,				"Table_World_Zone_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_FORMULA,				"TD_Formula");
	fileNameList.SetFileName(CTableContainer::TABLE_COMMON_CONFIG, "table_common_config_data");
	fileNameList.SetFileName(CTableContainer::TABLE_DWC,					"table_dwc_data");
	fileNameList.SetFileName(CTableContainer::TABLE_ITEM_ENCHANT, "table_item_enchant_data");
	fileNameList.SetFileName(CTableContainer::TABLE_NPC_SERVER,				"table_npc_data_server"); // required for mini map so we dont show npcs that are not spawned

	g_pTableContainer = NTL_NEW CTableContainer;
	
	std::string str;
	CTable::eLOADING_METHOD eLoadMethod = (CTable::eLOADING_METHOD)byLoadMethod;
	if(eLoadMethod == CTable::LOADING_METHOD_XML)
		str = NTL_GAME_TABLE_DATA_XML_FOLDER;
	else if(eLoadMethod == CTable::LOADING_METHOD_BINARY)
		str = NTL_GAME_TABLE_DATA_BIN_FOLDER;
	else
		str = NTL_GAME_TABLE_DATA_ENC_FOLDER;

    RwBool bResult = FALSE;
    if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TABLE)
    {
        CTablePackLoad tableLoad;
        bResult = g_pTableContainer->Create(flagManager, (char*)str.c_str(), &fileNameList, eLoadMethod, GetACP(), &tableLoad);
    }
    else
    {
        bResult= g_pTableContainer->Create(flagManager, (char*)str.c_str(), &fileNameList, eLoadMethod, GetACP(), NULL);
    }	

	return bResult;
}


RwBool API_SLInit(RwBool bVirtualNet, CNtlCamera *pCamera, RwUInt8 byLoadMethod, RwBool bDTDataLoad /*= FALSE*/)
{
	NTL_FUNCTION("API_SLInit");

	// system memory check
	::GlobalMemoryStatus(&g_memStaus);
	Logic_CalcLimitMemory(g_memStaus.dwTotalPhys);
    	
	// simulation event handler
	g_pSLEventHander = NTL_NEW CNtlSLEventHandler;
	if(!g_pSLEventHander->Create())
	{
		NTL_RETURN(FALSE);
	}

	if(!CreateTableContainer(byLoadMethod))
	{
		NTL_ASSERTFAIL("API_SLInit => Game Table load fail !!!");
		NTL_RETURN(FALSE);
	}

	g_pDTContainer = NTL_NEW CNtlDTContainer;
	if(!g_pDTContainer->Create(NTL_GAME_DT_DATA_FILE))
	{
		NTL_RETURN(FALSE);
	}

	if(bDTDataLoad)
		g_pDTContainer->Load(NTL_GAME_DT_DATA_FILE);

	if(bVirtualNet)
	{
		g_pNetSender = NTL_NEW CNtlVirtualNetSender;
	}
	else
	{
		g_pNetSender = NTL_NEW CNtlNetSender;
	}

	g_pSLPacketGenerator = NTL_NEW CNtlSLPacketGenerator(g_pNetSender);

	g_pTriggerSyncManager = NTL_NEW CNtlTriggerSyncManager;

	if(!g_pTriggerSyncManager->Create())
	{
		NTL_RETURN(FALSE);
	}

	g_pPacketLocker = NTL_NEW CNtlPacketLockManager;


	// input action mapping create
	g_pActionMap = NTL_NEW CInputActionMap;
	if(!g_pActionMap->Create())
	{
		NTL_RETURN(FALSE);
	}

	g_pSobMgr = NTL_NEW CNtlSobManager;
	if(!g_pSobMgr->Create())
	{
		NTL_RETURN(FALSE);
	}

	g_pFSMLookAtManager = NTL_NEW CNtlFSMLookAtManager;
	if(!g_pFSMLookAtManager->Create())
	{
		NTL_RETURN(FALSE);
	}

	SLLua_Setup();

	g_pGameCameraManager = NTL_NEW CNtlGameCameraManager;
	g_pGameCameraManager->Create();
	g_pGameCameraManager->SetActiveCamera(pCamera);
	g_pCamera = pCamera;

	// event handler 등록.
	CNtlSobCharProxy::RegisterEventHandler();

	g_pSLGlobal = NTL_NEW CNtlSLGlobal;
	if(!g_pSLGlobal->Create(TRUE))
	{
		NTL_RETURN(FALSE);
	}

	// world concept
	g_pWorldConcept = NTL_NEW CNtlWorldConcept;
	if(!g_pWorldConcept->Create())
	{
		NTL_RETURN(FALSE);
	}

	// Game clock 등록.
	g_pGameClock = NTL_NEW CNtlClock;

	g_pDTCinematicManager = NTL_NEW CNtlDTCinematicManager;
	if(!g_pDTCinematicManager->Create())
	{
		NTL_RETURN(FALSE);
	}

	if( false == CNtlSoundMoodManager::CreateInstance() )
	{
		NTL_RETURN(FALSE);
	}


	// Callback 등록
	LinkCallback_GetDogiColorIndex(Logic_GetSobPlayerDogiColor);


	return TRUE;
}

void API_SLReset(void)
{
	if(g_pSLGlobal)
	{
		g_pSLGlobal->Destroy();
		NTL_DELETE(g_pSLGlobal);
	}

	if(g_pGameCameraManager)
	{
		g_pGameCameraManager->Destroy();
		NTL_DELETE(g_pGameCameraManager);
	}
	
	if(g_pDTCinematicManager)
	{
		g_pDTCinematicManager->Destroy();
		NTL_DELETE(g_pDTCinematicManager);
	}

	CNtlSoundMoodManager::DestroyInstance();

	SLLua_Release();

	if(g_pFSMLookAtManager)
	{
		g_pFSMLookAtManager->Destroy();
		NTL_DELETE(g_pFSMLookAtManager);
	}

	if(g_pSobMgr)
	{
		g_pSobMgr->Destroy();
		NTL_DELETE(g_pSobMgr);
	}
	
	if(g_pActionMap)
	{
		g_pActionMap->Destroy();
		NTL_DELETE(g_pActionMap);
	}

	if(g_pPacketLocker)
	{
		NTL_DELETE(g_pPacketLocker);
	}

	if(g_pTriggerSyncManager)
	{
		g_pTriggerSyncManager->Destroy();
		NTL_DELETE(g_pTriggerSyncManager);
	}

	if(g_pWorldConcept)
	{
		g_pWorldConcept->Destroy();
		NTL_DELETE(g_pWorldConcept);
	}

	if(g_pGameClock)
	{
		NTL_DELETE(g_pGameClock);
	}

	// 다시 생성

	g_pTriggerSyncManager = NTL_NEW CNtlTriggerSyncManager;
	g_pTriggerSyncManager->Create();

	g_pPacketLocker = NTL_NEW CNtlPacketLockManager;

	// input action mapping create
	g_pActionMap = NTL_NEW CInputActionMap;
	g_pActionMap->Create();

	g_pSobMgr = NTL_NEW CNtlSobManager;
	g_pSobMgr->Create();

	g_pFSMLookAtManager = NTL_NEW CNtlFSMLookAtManager;
	g_pFSMLookAtManager->Create();

	SLLua_Setup();

	g_pGameCameraManager = NTL_NEW CNtlGameCameraManager;
	g_pGameCameraManager->Create();
	g_pGameCameraManager->SetActiveCamera(g_pCamera);
	
	g_pSLGlobal = NTL_NEW CNtlSLGlobal;
	g_pSLGlobal->Create(TRUE);

	// world concept
	g_pWorldConcept = NTL_NEW CNtlWorldConcept;
	g_pWorldConcept->Create();
	
	// Game clock 등록.
	g_pGameClock = NTL_NEW CNtlClock;

	g_pDTCinematicManager = NTL_NEW CNtlDTCinematicManager;
	g_pDTCinematicManager->Create();

	CNtlSoundMoodManager::CreateInstance();

}

void API_SLTerminate(void)
{
	NTL_FUNCTION("API_SLTerminate");    

	if(g_pDevMasterInfo)
	{
		NTL_DELETE(g_pDevMasterInfo);
	}

	if(g_pSLGlobal)
	{
		g_pSLGlobal->Destroy();
		NTL_DELETE(g_pSLGlobal);
	}

	if(g_pGameCameraManager)
	{
		g_pGameCameraManager->Destroy();
		NTL_DELETE(g_pGameCameraManager);
	}
	
	if(g_pDTCinematicManager)
	{
		g_pDTCinematicManager->Destroy();
		NTL_DELETE(g_pDTCinematicManager);
	}

	CNtlSoundMoodManager::DestroyInstance();
	
	SLLua_Release();

	if(g_pFSMLookAtManager)
	{
		g_pFSMLookAtManager->Destroy();
		NTL_DELETE(g_pFSMLookAtManager);
	}

	if(g_pSobMgr)
	{
		g_pSobMgr->Destroy();
		NTL_DELETE(g_pSobMgr);
	}
	
	if(g_pActionMap)
	{
		g_pActionMap->Destroy();
		NTL_DELETE(g_pActionMap);
	}

	if(g_pPacketLocker)
	{
		NTL_DELETE(g_pPacketLocker);
	}

	if(g_pTriggerSyncManager)
	{
		g_pTriggerSyncManager->Destroy();
		NTL_DELETE(g_pTriggerSyncManager);
	}

	if(g_pSLPacketGenerator)
	{
		NTL_DELETE(g_pSLPacketGenerator);
	}

	if(g_pNetSender)
	{
		NTL_DELETE(g_pNetSender);
	}

	if(g_pTableContainer)
	{
		NTL_DELETE(g_pTableContainer);
	}

	if(g_pDTContainer)
	{
		g_pDTContainer->Destroy();
		NTL_DELETE(g_pDTContainer);
	}

	if(g_pWorldConcept)
	{
		g_pWorldConcept->Destroy();
		NTL_DELETE(g_pWorldConcept);
	}

	if(g_pSLEventHander)
	{
		g_pSLEventHander->Destroy();
		NTL_DELETE(g_pSLEventHander);
	}

	if(g_pGameClock)
	{
		NTL_DELETE(g_pGameClock);
	}    

	NTL_RETURNVOID();
}

RwBool API_SLResourceInit(void)
{
	CNtlPLDamageBox::CreateResource();

	return TRUE;
}

void API_SLResourceTerminate(void)
{
	CNtlPLDamageBox::DestroyResource();
}

CTableContainer* API_GetTableContainer(void)
{
	return g_pTableContainer;
}

void API_SetTableContainer(CTableContainer* pTblContainer)
{
	g_pTableContainer = pTblContainer;
}

CNtlSLPacketGenerator* API_GetSLPacketGenerator(void)
{
	return g_pSLPacketGenerator;
}

CNtlPacketLockManager* API_GetSLPacketLockManager(void)
{
	return g_pPacketLocker;
}

CDboTSCQAgency* API_GetQuestAgency(void)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pSobAvatar == NULL)
		return NULL;

	return pSobAvatar->GetQuestAgency();
}

CDboTSCTAgency* API_GetTriggerAgency(void)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pSobAvatar == NULL)
		return NULL;

	return pSobAvatar->GetPCTriggerAgency();
}

CNtlClock* API_GetGameClock(void)
{
	return g_pGameClock;
}

CNtlDTContainer* API_GetDTContainter(void)
{
	return g_pDTContainer;
}

void API_InitDoubleClickInterval(void)
{
	g_uiSystemMouseClickInterval = GetDoubleClickTime();

	g_fCurMouseClickInterval = (DEF_MOUSE_CLICK_INTERVAL - MIN_MOUSE_CLICK_INTERVAL) / (MAX_MOUSE_CLICK_INTERVAL - MIN_MOUSE_CLICK_INTERVAL);

	API_SetDoubleClickInterval( g_fCurMouseClickInterval );
}

void API_DestroyDoubleClickInterval(void)
{
	SetDoubleClickTime( g_uiSystemMouseClickInterval );
}

RwReal API_GetDoubleClickInterval(void)
{
	return g_fCurMouseClickInterval;
}

void API_SetDoubleClickInterval( RwReal fIntervalRatio /*= 0.f*/ )
{
	g_fCurMouseClickInterval = fIntervalRatio;

	g_fCurMouseClickInterval = g_fCurMouseClickInterval <= 0.f ? 0.f : g_fCurMouseClickInterval;
	g_fCurMouseClickInterval = g_fCurMouseClickInterval >= 1.f ? 1.f : g_fCurMouseClickInterval;

	RwUInt32 uiInterval = (RwUInt32)((MAX_MOUSE_CLICK_INTERVAL - MIN_MOUSE_CLICK_INTERVAL) * g_fCurMouseClickInterval + MIN_MOUSE_CLICK_INTERVAL);

	SetDoubleClickTime( uiInterval );
}
