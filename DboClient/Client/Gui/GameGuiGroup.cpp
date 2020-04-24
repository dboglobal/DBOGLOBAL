#include "precomp_dboclient.h"
#include "GameGuiGroup.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLDef.h"
#include "NtlSLEvent.h"

// simulation
#include "NtlSobAvatar.h"
#include "NtlSLTMQ.h"
#include "NtlSLGlobal.h"
#include "NtlSobItemAttr.h"

// dbo
#include "DboGlobal.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "IconMoveManager.h"
#include "PopupManager.h"
#include "InfoWndManager.h"
#include "DboEvent.h"

#include "HpGui.h"
#include "ChatGui.h"
#include "QuickSlotGui.h"
#include "ExpGui.h"
#include "MainMenuGui.h"
#include "ESCMenuGui.h"
#include "MinimapGui.h"
#include "TargetHpGui.h"
#include "StatusWindowGui.h"
#include "BagGui.h"
#include "SkillCustomizeGui.h"
#include "PopupMenuGui.h"
#include "TradeCart.h"
#include "NPCShop.h"
#include "ScouterMeasureDlgGui.h"
#include "ScouterPCInfoViewer.h"
#include "ChatBalloonManager.h"
#include "SubGaugeGui.h"
#include "CommunityGui.h"
#include "PartyMenuGui.h"
#include "QuestGui.h"
#include "ItemChangeBattleAttributeGui.h"
#include "ItemUpgradeGui.h"
#include "WareHouseGui.h"
#include "CautionNotifyGui.h"
#include "TerritoryNotifyGui.h"
#include "PublicNotifyGui.h"
#include "ActionNotifyGui.h"
#include "GroupNotifyGui.h"
#include "NameNotifyGui.h"
#include "WorldMap.h"
#include "OptionWindowGui.h"
#include "HelpWindowGui.h"
#include "DropItemInfoGui.h"
#include "SideIconGui.h"
#include "AltarGui.h"
#include "DBCRewardGUI.h"
#include "PrivateShopGui.h"
#include "PrivateShopCartGui.h"
#include "TMQBoardGui.h"
#include "DBCNarrationGui.h"
#include "RBBoardGui.h"
#include "RBChallengeGui.h"
#include "MailSystemGui.h"
#include "CinematicGui.h"
#include "BackboardGui.h"
#include "TMQRewardGui.h"
#include "TMQStatusGui.h"
#include "PetStatusBarGui.h"
#include "PetSkillWindowGui.h"
#include "PetStatusBarGui.h"
#include "PetSkillWindowGui.h"
#include "RBGui.h"
#include "TBGui.h"
#include "PortalGui.h"
#include "ObjectClickGui.h"
#include "FlashNotifyGui.h"
#include "FriendListGui.h"
#include "InterfaceShakeGui.h"
#include "RankBoardGui.h"
#include "CommuTargetGui.h"
#include "TimeNotifyGui.h"
#include "SideDialogControlGui.h"
#include "GuildWarehouseGui.h"
#include "TutorialDialogFocusGui.h"
#include "TutorialExampleGui.h"
#include "TutorialMessageGui.h"
#include "TutorialConfirmGui.h"
#include "DiceManager.h"
#include "TBGambleResult.h"
#include "ChannelChangeGui.h"
#include "GMChatGui.h"
#include "PetitionGui.h"
#include "PetitionSatisfactionGui.h"
#include "BroadCastGui.h"
#include "SkillAbilitySetGui.h"
#include "SkillAbilityUseGui.h"
#include "SkillHTBRpResult.h"
#include "SkillHTBRpUse.h"
#include "VehicleGui.h"
#include "DeveloperPreviewGui.h"
#include "DojoInfoGui.h"
#include "DogiGui.h"
#include "DojoUpgradeGui.h"
#include "ScrambleNotifyGui.h"
#include "ScrambleStatusGui.h"
#include "NetPyShopGui.h"
#include "NetPyShopCartGui.h"
#include "DeliberationRankGui.h"
#include "DeliberationRankGui.h"
#include "CommercialExtendGui.h"

#include "GMGuiGroup.h"
#include "GroupChatManager.h"
#include "DialogManager.h"
#include "SideDialogManager.h"
#include "PetitionManager.h"
#include "DBCDragonDialogGui.h"

#include "ItemBindSealingGui.h"
#include "HLShopGui.h"
#include "YardratWarehouseGui.h"
#include "HLShopBuyGui.h"
#include "HLShopBuyConfirmGui.h"
#include "HLShopGiftGui.h"
#include "HLShopGiftConfirmGui.h"
#include "ItemSwapGui.h"
#include "ItemSwapCartGui.h"
#include "AuctionHouseGui.h"
#include "ItemOptionRestructureGui.h"
#include "PlayerTitleGui.h"
#include "CCBDBoardGui.h"
#include "CCBDStatusGui.h"
#include "CCBDNotifyGui.h"
#include "CCBDRewardGui.h"
#include "UltimateDungeonGui.h"
#include "HoiPoiMixCraftGui.h"
#include "HoiPoiMixRecipeGui.h"
#include "QuickTeleportGui.h"



// 2007.11.17 by agebreak
// A macro function that creates a dialog and registers it in the dialog manager.
#define AddDialog(pGui, classname, szName, id) \
	{ \
            pGui = NTL_NEW classname(szName);\
            if(!pGui->Create())    \
            {                      \
                pGui->Destroy();   \
                NTL_DELETE(pGui);  \
                NTL_RETURN(FALSE); \
            }                      \
            CNtlPLGuiManager::GetInstance()->AddGui(pGui); \
            GetDialogManager()->RegistDialog(id, pGui, &classname::SwitchDialog); \
	}

#define AddDialog_no_return(pGui, classname, szName, id) \
	{ \
			pGui = NTL_NEW classname(szName);\
			if(!pGui->Create())    \
			{                      \
				pGui->Destroy();   \
				NTL_DELETE(pGui);  \
			}                      \
			CNtlPLGuiManager::GetInstance()->AddGui(pGui); \
			GetDialogManager()->RegistDialog(id, pGui, &classname::SwitchDialog); \
	}

// A macro function that creates a dialog and registers it with the side dialog manager.
#define AddSideDialog(pGui, classname, szName, id) \
	{ \
		pGui = NTL_NEW classname(szName);\
		if(!pGui->Create())    \
		{                      \
			pGui->Destroy();   \
			NTL_DELETE(pGui);  \
			NTL_RETURN(FALSE); \
		}                      \
		CNtlPLGuiManager::GetInstance()->AddGui(pGui); \
		GetSideDialogManager()->RegistDialog(id, pGui, &classname::SwitchDialog); \
	}

#define AddSideDialog_no_return(pGui, classname, szName, id) \
	{ \
		pGui = NTL_NEW classname(szName);\
		if(!pGui->Create())    \
		{                      \
			pGui->Destroy();   \
			NTL_DELETE(pGui);  \
		}                      \
		CNtlPLGuiManager::GetInstance()->AddGui(pGui); \
		GetSideDialogManager()->RegistDialog(id, pGui, &classname::SwitchDialog); \
	}

// Macro function to remove dialog
#define RemoveDialog(pGui) \
	{ \
            if(pGui)       \
            {              \
                CNtlPLGuiManager::GetInstance()->RemoveGui(pGui);     \
                pGui->Destroy();                                    \
                NTL_DELETE(pGui);                                   \
            } \
	}


CGameGuiGroup::CGameGuiGroup()
:m_pHp(NULL)
,m_pChat(NULL)
,m_pQuickSlot(NULL)
,m_pExp(NULL)
,m_pMainMenu(NULL)
,m_pESCMenu(NULL)
,m_pMinimap(NULL)
,m_pTargetHp(NULL)
,m_pStatusWindow(NULL)
,m_pBagSlot(NULL)
,m_pSkillWindow(NULL)
, m_pPopupMenu(NULL)
, m_pCalcPopup(NULL)
, m_pTradeCart(NULL)
, m_pNPCShop(NULL)
, m_pScouterMeasureDlg(NULL)
, m_pScouterPCInfoViewer(NULL)
, m_pSubGauge(NULL)
, m_pCommunity(NULL)
, m_pPlayerTitle(NULL)
, m_pQuest(NULL)
, m_pItemChangeBattleAttribute(NULL)
, m_pItemUpgrade(NULL)
, m_pWarehouseBar(NULL)
, m_pWarehouse_1(NULL)
, m_pWarehouse_2(NULL)
, m_pWarehouse_3(NULL)
, m_pWarehouseCommon(NULL)
, m_pCautionNotify(NULL)
, m_pTerritoryNotify(NULL)
, m_pPublicNotify(NULL)
, m_pActionNotify(NULL)
, m_pGroupNotify(NULL)
, m_pNameNotify(NULL)
, m_pWorldMap(NULL)
, m_pOptionWindowGui(NULL)
, m_pHelpWindowGui(NULL)
, m_pDropItemInfoGui(NULL)
, m_pSideIconGui(NULL)
, m_pPrivateShopGui(NULL)
, m_pPrivateShopCartGui(NULL)
, m_pRBBoardGui(NULL)
//,m_pRBChallengeGui(NULL)
, m_pTMQBoard(NULL)
, m_pUDBoard(NULL)
, m_pMailSystemGui(NULL)
, m_pMailSystemReadGui(NULL)
, m_pBackboardGui(NULL)
, m_pPortalGui(NULL)
, m_pObjectClickGui(NULL)
, m_pFlashNotify(NULL)
, m_pFriendListGui(NULL)
, m_pInterfaceShakeGui(NULL)
, m_pRankBoardGui(NULL)
, m_pCommuTargetGui(NULL)
, m_pTimeNotifyGui(NULL)
, m_pCinematicGui(NULL)
//,m_pNetConnectBox(NULL)
, m_pSideDialogControl(NULL)
, m_pTutorialDialogFocus(NULL)
, m_pTutorialExample(NULL)
, m_pTutorialMessage(NULL)
, m_pToturialConfirm(NULL)
, m_pTBGambleResult(NULL)
, m_pChannelChange(NULL)
, m_pGroupChatManager(NULL)
, m_pGMChatting(NULL)
, m_pPetition(NULL)
, m_pPetitionSatisfaction(NULL)
, m_pBroadCastGui(NULL)
, m_pPartyMenu(NULL)
, m_pSkillAbilitySetGui(NULL)
, m_pSkillAbilityUseGui(NULL)
, m_pHTBRpResult(NULL)
, m_pHTBRpUse(NULL)
, m_pVehicle(NULL)
, m_pGMGuiGroup(NULL)
, m_pDeveloperPreviewGui(NULL)
, m_pDojoInfoGui(NULL)
, m_pDogiGui(NULL)
, m_pDojoUpgrade(NULL)
, m_pScrambleStatusGui(NULL)
, m_pScrambleNotify(NULL)
, m_pNetMablePCBangGui(NULL)
, m_pNetPyShopGui(NULL)
, m_pNetPyShopCartGui(NULL)
, m_pDeliverationRankContainer(NULL)
, m_pCommercialExtendGui(NULL)
, m_pItemBindSealingGui(NULL)
, m_pHoiPoiMixRecipeGui(NULL)
, m_pHoiPoiMixCraftGui(NULL)
, m_pHLShopGui(NULL)
, m_pYardratWarehouseGui(NULL)
, m_pHLShopBuyGui(NULL)
, m_pHLShopBuyConfirmGui(NULL)
, m_pHLShopGiftGui(NULL)
, m_pHLShopGiftConfirmGui(NULL)
, m_pItemSwapGui(NULL)
, m_pItemSwapCartGui(NULL)
, m_pAuctionHouseGui(NULL)
, m_pItemOptionRestructureGui(NULL)
, m_pQuickTeleportGui(NULL)
, m_pCCBDBoardGui(NULL)
, m_pCCBDNotifyGui(NULL)
, m_pCCBDRewardGui(NULL)
{	
}

CGameGuiGroup::~CGameGuiGroup()
{
}

RwBool CGameGuiGroup::Create(void)
{
	NTL_FUNCTION("CGameGuiGroup::Create");

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

	// Dialog manager 
	GetDialogManager()->CreateInstance( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	// 종족별 리소스 폴더 지정
	LoadCommonScript();	

	// PLGuiGroup
	GetNtlGuiManager()->CreatePLGuiGroup();

	// Popup
	CPopupManager::CreateInstance();

	// InfoWindow
	CInfoWndManager::CreateInstance();

	// Icon Move Manager
	CIconMoveManager::CreateInstance( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	// Balloon Manager
	CBalloonManager::CreateInstance();

	// RankBattle UI Manager.
	CRBGui::CreateInstance();

	// Budokai UI Manager.
	CTBGui::CreateInstance();
	GetTBGuiInstance()->CreateBudokaiNewsOnce();

	// Dice Manager
	CDiceManager::CreateInstance();

	// DropItemInfo
	m_pDropItemInfoGui = NTL_NEW CDropItemInfoGui( "DropItemInfoGui" );
	if( !m_pDropItemInfoGui->Create() )
	{
		m_pDropItemInfoGui->Destroy();
		NTL_DELETE( m_pDropItemInfoGui );
		NTL_RETURN( FALSE );
	}
	CNtlPLGuiManager::GetInstance()->AddGui( m_pDropItemInfoGui );
	GetDialogManager()->RegistDialog( DIALOG_DROPITEM_INFO, m_pDropItemInfoGui, &CDropItemInfoGui::SwitchDialog );

	AddDialog(m_pScouterMeasureDlg, CScouterMeasureDlgGui, "ScouterMeasure", DIALOG_SCOUTER_MEASURE);

	// rp cast gauge
	m_pSubGauge = NTL_NEW CSubGaugeGui( "SubGaugeGui" );
	if( !m_pSubGauge->Create() )
	{
		m_pSubGauge->Destroy();
		NTL_DELETE( m_pSubGauge );
		NTL_RETURN( FALSE );
	}
	CNtlPLGuiManager::GetInstance()->AddGui( m_pSubGauge );

	// hp/mp/cp
	AddDialog(m_pHp, CHpGui, "HpGui", DIALOG_HP);

	// chatting 
	m_pChat = NTL_NEW CChatGui("ChatGui");
	if(!m_pChat->Create())
	{
		m_pChat->Destroy(); 
		NTL_DELETE(m_pChat);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pChat);
	GetDialogManager()->RegistDialog( DIALOG_CHAT, m_pChat, &CChatGui::SwitchDialog );    

	// quick slot
	m_pQuickSlot = NTL_NEW CQuickSlotGui("QuickSlotGui");
	if(!m_pQuickSlot->Create())
	{
		m_pQuickSlot->Destroy();
		NTL_DELETE(m_pQuickSlot);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pQuickSlot); 
	GetDialogManager()->RegistDialog( DIALOG_QUICKSLOT, m_pQuickSlot, &CQuickSlotGui::SwitchDialog );

	// exp
	m_pExp = NTL_NEW CExpGui("ExpGui");
	if(!m_pExp->Create())
	{
		m_pExp->Destroy();
		NTL_DELETE(m_pExp);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pExp); 
	GetDialogManager()->RegistDialog( DIALOG_EXP, m_pExp, &CExpGui::SwitchDialog );

	// main menu
	m_pMainMenu = NTL_NEW CMainMenuGroup();
	if(!m_pMainMenu->Create())
	{
		m_pMainMenu->Destroy();
		NTL_DELETE(m_pMainMenu);
		NTL_RETURN(FALSE);
	}

	AddDialog( m_pESCMenu, CESCMenuGui, "ESCMenu", DIALOG_ESCMENU );

	AddDialog(m_pMinimap, CMinimapGui, "Minimap", DIALOG_MINIMAP);    

	// target hp
	AddDialog(m_pTargetHp, CTargetHpGui, "TargetHp", DIALOG_TARGER_UI);


	// StatusWindow
	AddDialog(m_pStatusWindow, CStatusWindowGui, "StatusWindowGui", DIALOG_STATUS);


	// BagSlot
	m_pBagSlot = NTL_NEW CBagSlotGui( "BagSlot" );
	if( !m_pBagSlot->Create() )
	{
		m_pBagSlot->Destroy();
		NTL_DELETE( m_pBagSlot );
		NTL_RETURN( FALSE );
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pBagSlot);
	GetDialogManager()->RegistDialog(DIALOG_BAGSLOT, m_pBagSlot, &CBagSlotGui::SwitchDialog);

	// SkillWindow
	//m_pSkillWindow = NTL_NEW CSkillWindowGui( "SkillWindow" );
	//if( !m_pSkillWindow->Create() )
	//{
	//	m_pSkillWindow->Destroy();
	//	NTL_DELETE( m_pSkillWindow );
	//	NTL_RETURN( FALSE );
	//}
	//CNtlPLGuiManager::GetInstance()->AddGui(m_pSkillWindow);
	//GetDialogManager()->RegistDialog( DIALOG_SKILL, m_pSkillWindow, &CSkillWindowGui::SwitchDialog );
	m_pSkillWindow = NTL_NEW CSkillCustomizeGui( "SkillWindow" );
	if( !m_pSkillWindow->Create() )
	{
		m_pSkillWindow->Destroy();
		NTL_DELETE( m_pSkillWindow );
		NTL_RETURN( FALSE );
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pSkillWindow);
	GetDialogManager()->RegistDialog( DIALOG_SKILL, m_pSkillWindow, &CSkillCustomizeGui::SwitchDialog );

	AddDialog(m_pNPCShop, CNPCShop, "NPCShop", DIALOG_NPCSHOP);

	// avooo's comment: I am usually registered as a cart in a shop,
	// Release and register with user trade cart
	AddDialog(m_pTradeCart, CTradeCart, "TradeCart", DIALOG_SHOPING_CART);

	// scouter
	AddSideDialog(m_pScouterPCInfoViewer, CScouterPCInfoViewerGui, "ScouterPCInfoViewer", SDIALOG_SCOUTER_PC_VIEWER);

	AddDialog(m_pCommunity, CCommunityGui, "Community", DIALOG_COMMUNITY);

	// Change Item battle attribute
	AddDialog(m_pItemChangeBattleAttribute, CItemChangeBattleAttributeGui, "ItemChangeBattleAttribute", DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE);

	// Upgrade item
	AddDialog(m_pItemUpgrade, CItemUpgradeGui, "ItemUpgrade", DIALOG_ITEMUPGRADE);

	// Quest Group
	m_pQuest = NTL_NEW CQuestGui();
	if( !m_pQuest->Create() )
	{
		m_pQuest->Destroy();
		NTL_DELETE(m_pQuest);
		NTL_RETURN(FALSE);
	}

	AddDialog(m_pWarehouseBar, CWarehouseBarGui, "WarehouseBarGui", DIALOG_WAREHOUSEBAR);

	// 창고 1
	m_pWarehouse_1 = NTL_NEW CWarehouseGui("WarehouseGui_1");
	m_pWarehouse_1->RegisterWarehouseIndex(0);
	if(!m_pWarehouse_1->Create())
	{
		m_pWarehouse_1->Destroy();
		NTL_DELETE(m_pWarehouse_1);
		NTL_RETURN(FALSE);
	}
	m_pWarehouseBar->RegisterWarehouse(0, m_pWarehouse_1);
	CNtlPLGuiManager::GetInstance()->AddGui(m_pWarehouse_1);
	GetDialogManager()->RegistDialog(DIALOG_WAREHOUSE_1, m_pWarehouse_1, &CWarehouseGui::SwitchDialog);

	// 창고 2
	m_pWarehouse_2 = NTL_NEW CWarehouseGui("WarehouseGui_2");
	m_pWarehouse_2->RegisterWarehouseIndex(1);
	if(!m_pWarehouse_2->Create())
	{
		m_pWarehouse_2->Destroy();
		NTL_DELETE(m_pWarehouse_2);
		NTL_RETURN(FALSE);
	}
	m_pWarehouseBar->RegisterWarehouse(1, m_pWarehouse_2);
	CNtlPLGuiManager::GetInstance()->AddGui(m_pWarehouse_2);
	GetDialogManager()->RegistDialog(DIALOG_WAREHOUSE_2, m_pWarehouse_2, &CWarehouseGui::SwitchDialog);

	// 창고 3
	m_pWarehouse_3 = NTL_NEW CWarehouseGui("WarehouseGui_3");
	m_pWarehouse_3->RegisterWarehouseIndex(2);
	if(!m_pWarehouse_3->Create())
	{
		m_pWarehouse_3->Destroy();
		NTL_DELETE(m_pWarehouse_3);
		NTL_RETURN(FALSE);
	}
	m_pWarehouseBar->RegisterWarehouse(2, m_pWarehouse_3);
	CNtlPLGuiManager::GetInstance()->AddGui(m_pWarehouse_3);
	GetDialogManager()->RegistDialog(DIALOG_WAREHOUSE_3, m_pWarehouse_3, &CWarehouseGui::SwitchDialog);

	// 공유창고
	m_pWarehouseCommon = NTL_NEW CWarehouseCommonGui("WarehouseCommonGui");
	m_pWarehouseCommon->RegisterWarehouseIndex(3);
	if(!m_pWarehouseCommon->Create())
	{
		m_pWarehouseCommon->Destroy();
		NTL_DELETE(m_pWarehouseCommon);
		NTL_RETURN(FALSE);
	}
	m_pWarehouseBar->RegisterWarehouse(3, m_pWarehouseCommon);
	CNtlPLGuiManager::GetInstance()->AddGui(m_pWarehouseCommon);
	GetDialogManager()->RegistDialog(DIALOG_WAREHOUSE_COMMON, m_pWarehouseCommon, &CWarehouseCommonGui::SwitchDialog);

	// caution notify
	m_pCautionNotify = NTL_NEW CCautionNotifyGui("CautionNotifyGui");
	if( !m_pCautionNotify->Create() )
	{
		m_pCautionNotify->Destroy();
		NTL_DELETE(m_pCautionNotify);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pCautionNotify);

	// Territory notify
	m_pTerritoryNotify = NTL_NEW CTerritoryNotifyGui("TerritoryNotifyGui");
	if( !m_pTerritoryNotify->Create() )
	{
		m_pTerritoryNotify->Destroy();
		NTL_DELETE(m_pTerritoryNotify);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pTerritoryNotify);

	// Public Notify
	m_pPublicNotify = NTL_NEW CPublicNotifyGui("PublicNotifyGui");
	if( !m_pPublicNotify->Create() )
	{
		m_pPublicNotify->Destroy();
		NTL_DELETE(m_pPublicNotify);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pPublicNotify);

	// Action Notify
	m_pActionNotify = NTL_NEW CActionNotifyGui("ActionNotifyGui");
	if( !m_pActionNotify->Create() )
	{
		m_pActionNotify->Destroy();
		NTL_DELETE(m_pActionNotify);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pActionNotify);

	// Group Notify
	m_pGroupNotify = NTL_NEW CGroupNotifyGui("GroupNotifyGui");
	if( !m_pGroupNotify->Create() )
	{
		m_pGroupNotify->Destroy();
		NTL_DELETE(m_pGroupNotify);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pGroupNotify);

	// Name Notify
	m_pNameNotify = NTL_NEW CNameNotifyGui("NameNotifyGui");
	if( !m_pNameNotify->Create() )
	{
		m_pNameNotify->Destroy();
		NTL_DELETE(m_pNameNotify);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pNameNotify);

	// HTB RP Result
	m_pHTBRpResult = NTL_NEW CSkillHTBRpResult("SkillHTBRpResult");
	if( !m_pHTBRpResult->Create() )
	{
		m_pHTBRpResult->Destroy();
		NTL_DELETE(m_pHTBRpResult);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pHTBRpResult);

	AddDialog(m_pWorldMap, CWorldMapGui, "CWorldMapGui", DIALOG_WORLDMAP);

	// Option Window, Help Window, Side Icon
	AddDialog( m_pOptionWindowGui, COptionWindowGui, "OptionWndGui", DIALOG_OPTIONWND );

	AddDialog( m_pHelpWindowGui, CHelpWindowGui, "HelpWndGui", DIALOG_HELPWND );

	AddDialog( m_pSideIconGui, CSideIconGui, "SideIconGui", DIALOG_SIDEICON );

	// Private Shop
	AddDialog( m_pPrivateShopGui, CPrivateShopGui, "PrivateShopGui", DIALOG_PRIVATESHOP ); 
	AddDialog( m_pPrivateShopCartGui, CPrivateShopCartGui, "PrivateShopCartGui", DIALOG_PRIVATESHOP_CART );

    AddDialog(m_pRBBoardGui, CRBBoardGui, "RBBoardGui", DIALOG_RBBOARD);

    /*AddDialog(m_pRBChallengeGui, CRBChallengeGui, "RBChallengeGui", DIALOG_RBCHALLENGE);*/

	AddDialog(m_pTMQBoard, CTMQBoardGui, "TMQBoardGui", DIALOG_TMQBOARD);

	AddDialog(m_pUDBoard, CUltimateDungeonGui, "UltimateDungeonGui", DIALOG_ULTIMATEDUNGEON);

	// Post System Gui (Mail)
	AddDialog( m_pMailSystemGui, CMailSystemGui, "MailSystemGui", DIALOG_MAILSYSTEM );

	AddDialog( m_pMailSystemReadGui, CMailSystemReadGui, "MailSystemReadGui", DIALOG_MAILSYSTEM_READ );

	// CinematicGui
	m_pCinematicGui = NTL_NEW CCinematicGui();
	if( !m_pCinematicGui->Create() )
	{
		m_pCinematicGui->Destroy();
		NTL_DELETE( m_pCinematicGui );
		NTL_RETURN( FALSE );
	}

	AddDialog(m_pBackboardGui, CBackboardGui, "Backboard", DIALOG_BACKBOARD);	

	AddDialog(m_pPortalGui, CPortalGui, "PortalGui", DIALOG_PORTAL);

    AddDialog(m_pObjectClickGui, CObjectClickGui, "ObjectClickGui", DIALOG_OBJECT_CLICK);

	AddDialog(m_pFlashNotify, CFlashNotifyGui, "FlashNotifyGui", DIALOG_FLASH_NOTIFY);

    AddDialog(m_pFriendListGui, CFriendListGui, "FriendListGui", DIALOG_FRIEND_LIST);

    AddDialog(m_pCommuTargetGui, CCommuTargetGui, "CommuTargetGui", DIALOG_COMMU_TARGET);

	AddDialog(m_pTimeNotifyGui, CTimeNotifyGui, "TimeNotifyGui", DIALOG_TIME_NOTIFY);	

	// Interface Shake
	AddDialog(m_pInterfaceShakeGui, CInterfaceShakeGui, "InterfaceShakeGui", DIALOG_INTERFACE_SHAKE );

	// RankBoard Gui
	AddDialog(m_pRankBoardGui, CRankBoardGui, "RankBoardGui", DIALOG_RANKBOARD );

	AddDialog(m_pTBGambleResult, CTBGambleResultGui, "TBGambleResultGui", DIALOG_TBGAMBLERESULT );

	AddDialog(m_pChannelChange, CChannelChangeGui, "ChannelChangeGui", DIALOG_CHANNGEL_CHANGE );	

	//AddDialog(m_pGroupChatManager, CGroupChatManager, "GroupChatManager", DIALOG_GROUP_CHAT_MANAGER );	

	// party
	AddDialog(m_pPartyMenu, CPartyMenu, "PartyMenuGui", DIALOG_PARTYMENU );	

	// 사이드 다이얼로그 컨트롤
	AddDialog(m_pSideDialogControl, CSideDialogControlGui, "SideDialogControlGui", DIALOG_SIDEDIALOG_CONTROLLER );

	// BroadCast Gui
	AddDialog(m_pBroadCastGui, CBroadCastGui, "BroadCastGui", DIALOG_BROADCAST );

	AddDialog(m_pSkillAbilitySetGui, CSkillAbilitySetGui, "SkillAbilitySetGui", DIALOG_SKILLABILITYSET );

	AddDialog(m_pSkillAbilityUseGui, CSkillAbilityUseGui, "SkillAbilityUseGui", DIALOG_SKILLABILITYUSE );

	AddDialog(m_pHTBRpUse, CSkillHtbRpUse, "SkillHtbRpUse", DIALOG_SKILL_HTB_RPUSE );

	// GM Gui Group
	if( pAvatarInfo->sCharPf.bIsGameMaster )
	{
		m_pGMGuiGroup = NTL_NEW CGMGuiGroup;
		if( !m_pGMGuiGroup->Create() )
		{
			m_pGMGuiGroup->Destroy();
			NTL_DELETE(m_pGMGuiGroup);
		}
	}	

	// NetPyShop
	AddDialog(m_pNetPyShopGui, CNetPyShopGui, "NetPyShopGui", DIALOG_NETPYSHOP);
	AddDialog(m_pNetPyShopCartGui, CNetPyShopCartGui, "NetPyShopCartGui", DIALOG_NETPYSHOP_TRADE);	

    // Link Move
	m_pNPCShop->Link( m_pTradeCart->GetDialog() );
	m_pNetPyShopGui->Link( m_pNetPyShopCartGui->GetDialog() );
	m_pWarehouseBar->Link( m_pWarehouse_1->GetDialog() );
	m_pWarehouseBar->Link( m_pWarehouse_2->GetDialog() );
	m_pWarehouseBar->Link( m_pWarehouse_3->GetDialog() );
	m_pWarehouseBar->Link( m_pWarehouseCommon->GetDialog() );


	//Show rating
	//if( eDBO_SERVICE_PUBLISHER_CJI_KOR == eService ) // by daneos
	{
		m_pDeliverationRankContainer = NTL_NEW CDeliberationRankContainer;
		if(!m_pDeliverationRankContainer->Create())
		{
			m_pDeliverationRankContainer->Destroy();
			NTL_DELETE(m_pDeliverationRankContainer);
			NTL_RETURN(FALSE);
		}
	}

	// Commercial extend confirm dialog
	AddDialog(m_pCommercialExtendGui, CCommercialExtendGui, "CommercialExtendGui", DIALOG_DURATION_EXTEND_CONFIRM);


	// by daneos
	AddDialog(m_pHoiPoiMixRecipeGui, CHoiPoiMixRecipeGui, "CHoiPoiMixRecipeGui", DIALOG_HOIPOIMIX_RECIPE);
	AddDialog(m_pHoiPoiMixCraftGui, CHoiPoiMixCraftGui, "CHoiPoiMixCraftGui", DIALOG_HOIPOIMIX_CRAFT);
	m_pHoiPoiMixRecipeGui->Link(m_pHoiPoiMixCraftGui->GetDialog());

	AddDialog(m_pItemBindSealingGui, CItemBindSealingGui, "ItemBindSealingGui", DIALOG_BIND_SEALING);

	AddDialog(m_pHLShopGui, CHLShopGui, "HLShopGui", DIALOG_HLSHOP);
	AddDialog(m_pYardratWarehouseGui, CYardratWarehouseGui, "CYardratWarehouseGui", DIALOG_HLSHOP_WAREHOUSE);
	m_pHLShopGui->Link(m_pYardratWarehouseGui->GetDialog());
	AddDialog(m_pHLShopBuyGui, CHLShopBuyGui, "HLShopBuyGui", DIALOG_HLSHOP_BUY);
	AddDialog(m_pHLShopBuyConfirmGui, CHLShopBuyConfirmGui, "HLShopBuyConfirmGui", DIALOG_HLSHOP_BUY_CONFIRM);
	AddDialog(m_pHLShopGiftGui, CHLShopGiftGui, "HLShopGiftGui", DIALOG_HLSHOP_GIFT);
	AddDialog(m_pHLShopGiftConfirmGui, CHLShopGiftConfirmGui, "HLShopGiftConfirmGui", DIALOG_HLSHOP_GIFT_CONFIRM);

	AddDialog(m_pItemSwapGui, CItemSwapGui, "ItemSwapGui", DIALOG_ITEM_EXCHANGE_SHOP);
	AddDialog(m_pItemSwapCartGui, CItemSwapCartGui, "ItemSwapCartGui", DIALOG_ITEM_EXCHANGE_CART);
	m_pItemSwapGui->Link(m_pItemSwapCartGui->GetDialog());

	AddDialog(m_pAuctionHouseGui, CAuctionHouseGui, "AuctionHouseGui", DIALOG_AUCTION_HOUSE);

	AddDialog(m_pItemOptionRestructureGui, CItemOptionRestructureGui, "CItemOptionRestructureGui", DIALOG_ITEM_OPTIONRESTRUCTURE);
	AddDialog(m_pQuickTeleportGui, CQuickTeleportGui, "CQuickTeleportGui", DIALOG_QUICK_TELEPORT);

	AddDialog(m_pPlayerTitle, CPlayerTitleGui, "PlayerTitleGui", DIALOG_PLAYER_TITLE);

	AddDialog(m_pCCBDBoardGui, CCCBDBoardGui, "CCBDBoardGui", DIALOG_CCBD_BOARD);



	// gui manager signal
	gui::CGUIManager *pGuiMgr = CNtlPLGuiManager::GetInstance()->GetGuiManager();  
	m_slotPaint = pGuiMgr->SigPaint().Connect(this, &CGameGuiGroup::OnPaint);

	LinkMsg(g_EventSobInfoUpdate);
	LinkMsg(g_EventSummonPet);
	LinkMsg(g_EventChangeWorldConceptState);
	LinkMsg(g_EventTMQCleintState);
	LinkMsg(g_EventPartyUpdateValue);
	LinkMsg(g_EventGameServerChangeOut);
	LinkMsg(g_EventGuildWarehouseNotify);
	LinkMsg(g_EventNotifyGuild);
    LinkMsg(g_EventTransform);
	LinkMsg(g_EventPetition);
	LinkMsg(g_EventSobVehicleEngine);
	LinkMsg(g_EventDebug_for_Developer);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventDojoStateNotify);

	GetDialogManager()->OpenDefaultDialog();

	NTL_RETURN(TRUE);
}

void CGameGuiGroup::Destroy(void)
{
	NTL_FUNCTION("CGameGuiGroup::Destroy");

	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventSummonPet);
	UnLinkMsg(g_EventChangeWorldConceptState);
	UnLinkMsg(g_EventTMQCleintState);
	UnLinkMsg(g_EventPartyUpdateValue);
	UnLinkMsg(g_EventGameServerChangeOut);
	UnLinkMsg(g_EventGuildWarehouseNotify);
	UnLinkMsg(g_EventNotifyGuild);	
    UnLinkMsg(g_EventTransform);
	UnLinkMsg(g_EventPetition);
	UnLinkMsg(g_EventSobVehicleEngine);
	UnLinkMsg(g_EventDebug_for_Developer);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventDojoStateNotify);

	CTBGui::DeleteInstance();
	CRBGui::DeleteInstance();

	CDBCNarrationGui::DeleteInstance();
	CBalloonManager::DeleteInstance();
	CPetStatusBarGui::DeleteInstance();
	CPetSkillWindowGui::DeleteInstance();

	RemoveDialog(m_pItemOptionRestructureGui);
	RemoveDialog(m_pQuickTeleportGui);
	
	CTMQRewardGui::DestroyInstance();
	CTMQStatusGui::DestroyInstance();

	CCCBDStatusGui::DestroyInstance();

	CGuildWarehouseBar::DestroyInstance();

    RemoveDialog(m_pDropItemInfoGui);
    RemoveDialog(m_pSubGauge);
    RemoveDialog(m_pHp);
    RemoveDialog(m_pChat);
    RemoveDialog(m_pQuickSlot);
    RemoveDialog(m_pExp);

	if(m_pMainMenu)
	{
		m_pMainMenu->Destroy();
		NTL_DELETE(m_pMainMenu);
	}

	RemoveDialog(m_pESCMenu);
	RemoveDialog(m_pMinimap);
    RemoveDialog(m_pTargetHp);
    RemoveDialog(m_pStatusWindow);
	RemoveDialog(m_pBagSlot);
    RemoveDialog(m_pSkillWindow);
	RemoveDialog(m_pNPCShop);
	RemoveDialog(m_pTradeCart);
	RemoveDialog(m_pScouterMeasureDlg);
	RemoveDialog(m_pScouterPCInfoViewer);
	RemoveDialog(m_pCommunity);
	RemoveDialog(m_pPlayerTitle);
    RemoveDialog(m_pItemChangeBattleAttribute);
	RemoveDialog(m_pItemUpgrade);

	if( m_pQuest )
	{
		m_pQuest->Destroy();
		NTL_DELETE( m_pQuest );
	}

	RemoveDialog(m_pWarehouseBar);
	RemoveDialog(m_pWarehouse_1);
	RemoveDialog(m_pWarehouse_2);
	RemoveDialog(m_pWarehouse_3);
	RemoveDialog(m_pWarehouseCommon);
    RemoveDialog(m_pCautionNotify);
    RemoveDialog(m_pTerritoryNotify);
    RemoveDialog(m_pPublicNotify);
    RemoveDialog(m_pActionNotify);
	RemoveDialog(m_pGroupNotify);
	RemoveDialog(m_pNameNotify);
	RemoveDialog(m_pHTBRpResult);
	RemoveDialog(m_pWorldMap);
	RemoveDialog(m_pOptionWindowGui);
	RemoveDialog(m_pSideIconGui);
	RemoveDialog(m_pHelpWindowGui);
	RemoveDialog(m_pPrivateShopGui);
	RemoveDialog(m_pPrivateShopCartGui);
    RemoveDialog(m_pRBBoardGui);
    /*RemoveDialog(m_pRBChallengeGui);*/
	RemoveDialog(m_pTMQBoard);
	RemoveDialog(m_pUDBoard);
	RemoveDialog(m_pMailSystemGui);
	RemoveDialog(m_pMailSystemReadGui);
    RemoveDialog(m_pBackboardGui);
    RemoveDialog(m_pPortalGui);
    RemoveDialog(m_pObjectClickGui);
    RemoveDialog(m_pFlashNotify);
    RemoveDialog(m_pFriendListGui);
	RemoveDialog(m_pInterfaceShakeGui);
	RemoveDialog(m_pRankBoardGui);
    RemoveDialog(m_pCommuTargetGui);
	RemoveDialog(m_pTimeNotifyGui);
    //RemoveDialog(m_pNetConnectBox);
	RemoveDialog(m_pSideDialogControl);
	RemoveDialog(m_pTutorialDialogFocus);
	RemoveDialog(m_pTutorialExample);
	RemoveDialog(m_pTutorialMessage);
	RemoveDialog(m_pToturialConfirm);
	RemoveDialog(m_pTBGambleResult);
	RemoveDialog(m_pChannelChange);
	RemoveDialog(m_pGroupChatManager);	
	RemoveDialog(m_pGMChatting);
	RemoveDialog(m_pPetition);
	RemoveDialog(m_pPetitionSatisfaction);	
	RemoveDialog(m_pBroadCastGui);
	RemoveDialog(m_pPartyMenu);
	RemoveDialog(m_pSkillAbilitySetGui);
	RemoveDialog(m_pSkillAbilityUseGui);
	RemoveDialog(m_pHTBRpUse);
	RemoveDialog(m_pVehicle);
	RemoveDialog(m_pDeveloperPreviewGui);
	RemoveDialog(m_pDojoInfoGui);
	RemoveDialog(m_pDogiGui);
	RemoveDialog(m_pDojoUpgrade);
	RemoveDialog(m_pScrambleStatusGui);
	RemoveDialog(m_pScrambleNotify);
	RemoveDialog(m_pNetPyShopGui);
	RemoveDialog(m_pNetPyShopCartGui);	
	RemoveDialog(m_pCommercialExtendGui);
	RemoveDialog(m_pItemBindSealingGui);
	RemoveDialog(m_pHoiPoiMixRecipeGui);
	RemoveDialog(m_pHoiPoiMixCraftGui);
	RemoveDialog(m_pHLShopGui);
	RemoveDialog(m_pYardratWarehouseGui);
	RemoveDialog(m_pHLShopBuyGui);
	RemoveDialog(m_pHLShopBuyConfirmGui);
	RemoveDialog(m_pHLShopGiftGui);
	RemoveDialog(m_pHLShopGiftConfirmGui);
	RemoveDialog(m_pItemSwapGui);
	RemoveDialog(m_pItemSwapCartGui);
	RemoveDialog(m_pAuctionHouseGui);
	RemoveDialog(m_pCCBDBoardGui);
	RemoveDialog(m_pCCBDNotifyGui);
	RemoveDialog(m_pCCBDRewardGui);
			
	if( m_pCinematicGui )
	{
		m_pCinematicGui->Destroy();
		NTL_DELETE( m_pCinematicGui );
	}


	if( m_pDeliverationRankContainer )
	{
		m_pDeliverationRankContainer->Destroy();
		NTL_DELETE(m_pDeliverationRankContainer);
	}
	

	if( m_pGMGuiGroup )
	{
		m_pGMGuiGroup->Destroy();
		NTL_DELETE(m_pGMGuiGroup);
	}	

	CDiceManager::DeleteInstance();

	CPopupManager::DeleteInstance();

	CInfoWndManager::DeleteInstance();

	CIconMoveManager::DeleteInstance();

	CAltarGui::DeleteInstance();
	CDBCRewardGUI::DeleteInstance();
    CDBCDragonDialogGUI::DeleteInstance();

	GetNtlGuiManager()->DestroyPLGuiGroup();

	GetDialogManager()->DestoryInstance();
	// disconnect gui manager signal
	gui::CGUIManager *pGuiMgr = CNtlPLGuiManager::GetInstance()->GetGuiManager();  

	pGuiMgr->SigPaint().Disconnect(m_slotPaint);

	UnloadCommonScript();
	
	NTL_RETURNVOID();
}

void CGameGuiGroup::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CGameGuiGroup::HandleEvents");

	if(pMsg.Id == g_EventSobInfoUpdate)
	{
		SNtlEventSobInfoUpdate *pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>(pMsg.pData); 
		if(pUpdate->uiUpdateType == EVENT_AIUT_DIE)
		{
			if(pUpdate->hSerialId == Logic_GetAvatarTargetHandle())
				Logic_SobTarget(INVALID_SERIAL_ID, INVALID_BYTE);

			// to do: check if regen box already shown
			Logic_ShowRegenBox();
		}
	}
    else if(pMsg.Id == g_EventSummonPet)		// 소환수가 소환되면 소환수 UI들의 객체를 생성하고 표시한다.
    {
        SDboEventSummonPet* pData = reinterpret_cast<SDboEventSummonPet*>(pMsg.pData);

        if(pData->bCreate)
        {
			GetDialogManager()->RegistDialog( DIALOG_PET_STATUSBAR, CPetStatusBarGui::GetInstance(), &CPetStatusBarGui::SwitchDialog );
            CPetStatusBarGui::GetInstance()->SetPetID(pData->uiSerialID);

			GetDialogManager()->RegistDialog( DIALOG_PET_SKILL_SLOT, CPetSkillWindowGui::GetInstance(), &CPetSkillWindowGui::SwitchDialog );
            CPetSkillWindowGui::GetInstance()->SetPetID(pData->uiSerialID, CPetSkillWindowGui::E_PET_TYPE_SUMMON);

            GetDialogManager()->LocationDialogs(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

			GetDialogManager()->OpenDialog(DIALOG_PET_STATUSBAR);
			GetDialogManager()->OpenDialog(DIALOG_PET_SKILL_SLOT);
        }		
    }
    else if(pMsg.Id == g_EventTransform)
    {
        GetDialogManager()->RegistDialog( DIALOG_PET_SKILL_SLOT, CPetSkillWindowGui::GetInstance(), &CPetSkillWindowGui::SwitchDialog );
        CPetSkillWindowGui::GetInstance()->HandleEvents(pMsg);

        GetDialogManager()->LocationDialogs(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
    }
	else if(pMsg.Id == g_EventChangeWorldConceptState)
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( pMsg.pData );

		if( pEvent->eWorldConcept == WORLD_PLAY_TIME_MACHINE )
		{
			if( pEvent->uiState == WORLD_STATE_ENTER )
			{
				CTMQRewardGui::CreateInstance();
				CTMQStatusGui::CreateInstance();

				GetDialogManager()->LocationDialogs(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
			}
			else if( pEvent->uiState == WORLD_STATE_EXIT )
			{
				GetDialogManager()->CloseDialog(DIALOG_TELECAST);

				CTMQRewardGui::DestroyInstance();
				CTMQStatusGui::DestroyInstance();
			}
		}
		else if( pEvent->eWorldConcept == WORLD_PLAY_TUTORIAL )
		{
			if( pEvent->uiState == WORLD_STATE_EXIT )
			{
				GetDialogManager()->UnRegistDialog(DIALOG_TUTORIAL_DIALOG_FOCUS);
				GetDialogManager()->UnRegistDialog(DIALOG_TUTORIAL_EXAMPLE);
				GetDialogManager()->UnRegistDialog(DIALOG_TUTORIAL_MESSAGE);
				GetDialogManager()->UnRegistDialog(DIALOG_TUTORIAL_CONFIRM);

				RemoveDialog(m_pTutorialDialogFocus);
				RemoveDialog(m_pTutorialExample);
				RemoveDialog(m_pTutorialMessage);
				RemoveDialog(m_pToturialConfirm);
			}
		}
		else if( pEvent->eWorldConcept == WORLD_PLAY_DOJO_SCRAMBLE )
		{
			if( pEvent->uiState == WORLD_STATE_ENTER )
			{
				AddDialog_no_return(m_pScrambleNotify, CScrambleNotifyGui, "CScrambleNotifyGui", DIALOG_SCRAMBLE_NOTIFY);
				GetDialogManager()->LocationDialogs(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
			}
			else if( pEvent->uiState == WORLD_STATE_EXIT )
			{
				GetDialogManager()->UnRegistDialog(DIALOG_SCRAMBLE_NOTIFY);
				RemoveDialog(m_pScrambleNotify);
			}
		}
		else if (pEvent->eWorldConcept == WORLD_PLAY_CCBD)
		{
			if (pEvent->uiState == WORLD_STATE_ENTER)
			{
				CCCBDStatusGui::CreateInstance();
				AddDialog_no_return(m_pCCBDNotifyGui, CCCBDNotifyGui, "CCCBDNotifyGui", DIALOG_CCBD_NOTIFY);
				AddDialog_no_return(m_pCCBDRewardGui, CCCBDRewardGui, "CCCBDRewardGui", DIALOG_CCBD_REWARD);

				GetDialogManager()->LocationDialogs(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
			}
			else if (pEvent->uiState == WORLD_STATE_EXIT)
			{
				GetDialogManager()->UnRegistDialog(DIALOG_CCBD_NOTIFY);
				RemoveDialog(m_pCCBDNotifyGui);

				GetDialogManager()->UnRegistDialog(DIALOG_CCBD_REWARD);
				RemoveDialog(m_pCCBDRewardGui);

				CCCBDStatusGui::DestroyInstance();
			}
		}
	}
	else if( pMsg.Id == g_EventTMQCleintState )
	{
		SNtlEventTMQServerState* pEvent = reinterpret_cast<SNtlEventTMQServerState*>( pMsg.pData );

		if( pEvent->eWorldConcept != WORLD_PLAY_TUTORIAL )
			NTL_RETURNVOID();

		if( pEvent->uiState == WORLD_STATE_TMQ_TERRAIN_READY )
		{
			AddDialog_no_return(m_pTutorialDialogFocus, CTutorialDialogFocus, "CTutorialDialogFocus", DIALOG_TUTORIAL_DIALOG_FOCUS);
			AddDialog_no_return(m_pTutorialExample, CTutorialExampleGui, "CTutorialExample", DIALOG_TUTORIAL_EXAMPLE);
			AddDialog_no_return(m_pTutorialMessage, CTutorialMessageGui, "CTutorialMessage", DIALOG_TUTORIAL_MESSAGE);
			AddDialog_no_return(m_pToturialConfirm, CTutorialConfirmGui, "CTutorialConfirm", DIALOG_TUTORIAL_CONFIRM);			

			GetDialogManager()->LocationDialogs(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
		}
	}
	else if( pMsg.Id == g_EventPartyUpdateValue )
	{
		SNtlEventPartyUpdate* pEvent = reinterpret_cast<SNtlEventPartyUpdate*>(pMsg.pData);

		if( pEvent->iMessage == PMT_PARTY_CREATE )
		{
			// 파티 생성시 파티 인벤이 TMQ 사이드 다이얼로그를 가리는 것을 막기 위해
			CSideDialogControlGui* pSideDialogControlGui = (CSideDialogControlGui*)GetDialogManager()->GetDialog(DIALOG_SIDEDIALOG_CONTROLLER);
			if( pSideDialogControlGui )
				pSideDialogControlGui->SelectButtonType(SDIALOG_TMQ_STATUS);
			else
				DBO_FAIL("Not exist side dialog control gui");
		}
		else if( pEvent->iMessage == PMT_PARTY_LEAVE )
		{
		}
	}
	else if( pMsg.Id == g_EventGameServerChangeOut )
	{
		// 길드 정보 삭제
		GetDialogManager()->CloseDialog(DIALOG_GUILD_WAREHOUSEBAR);
		GetDialogManager()->SwitchBag(FALSE);
		CGuildWarehouseBar::DestroyInstance();

		// 파티 정보 삭제
		m_pPartyMenu->ResetPartyMenu();
	}	
	else if( pMsg.Id == g_EventGuildWarehouseNotify )
	{
		SNtlGuildWarehouseNotify* pEvent = reinterpret_cast<SNtlGuildWarehouseNotify*>( pMsg.pData );

		if( pEvent->byMessage == GUILD_WAREHOUSE_EVENT_START )
		{
			CGuildWarehouseBar::CreateInstance();
			GetDialogManager()->SwitchBag(TRUE);
			GetDialogManager()->OpenDialog(DIALOG_GUILD_WAREHOUSEBAR, INVALID_SERIAL_ID, FALSE);
		}
		else if( pEvent->byMessage == GUILD_WAREHOUSE_EVENT_END )
		{
			GetDialogManager()->SwitchBag(FALSE);
			CGuildWarehouseBar::DestroyInstance();
		}
	}
	else if( pMsg.Id == g_EventNotifyGuild )
	{
		SNtlEventNotifyGuild* pEvent = reinterpret_cast<SNtlEventNotifyGuild*>( pMsg.pData );

		if( pEvent->iMessage == SLGE_DISBAND )
		{
			GetDialogManager()->CloseDialog(DIALOG_GUILD_WAREHOUSEBAR);
			GetDialogManager()->SwitchBag(FALSE);
			CGuildWarehouseBar::DestroyInstance();
		}
	}
	else if( pMsg.Id == g_EventPetition )
	{
		SDboEventPetition* pEvent = reinterpret_cast<SDboEventPetition*>( pMsg.pData );

		switch(pEvent->uiEventType)
		{
		case PETITON_OPEN_DILAOG:
			{
				if( m_pPetition == NULL )
				{
					AddDialog_no_return(m_pPetition, CPetitionGui, "Petition", DIALOG_PETITION );
				}
				
				GetDialogManager()->OpenDialog(DIALOG_PETITION);

				break;
			}
		case PETITON_CLOSE_DILAOG:
			{
				GetDialogManager()->UnRegistDialog(DIALOG_PETITION);
				RemoveDialog(m_pPetition);
				break;
			}
		case PETITON_CHATTING_START:
			{
				AddDialog_no_return(m_pGMChatting, CGMChatGui, "GMChatGui", DIALOG_GM_CHATTING );
				GetDialogManager()->OpenDialog(DIALOG_GM_CHATTING);
				break;
			}
		case PETITON_CHATTING_END:
			{
				GetDialogManager()->UnRegistDialog(DIALOG_GM_CHATTING);
				RemoveDialog(m_pGMChatting);

				if( GetPetitionManager()->IsNeedSatisfation() )
				{
					// 유저 진정 만족도 조사
					AddDialog_no_return(m_pPetitionSatisfaction, CPetitionSatisfaction, "PetitionSatisfaction", DIALOG_PETITION_SATISFACTION );
					GetDialogManager()->LocationDialogs(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
					GetDialogManager()->OpenDialog(DIALOG_PETITION_SATISFACTION);
				}

				break;
			}
		case PETITON_FINISH_RESEARCH_SATISFACTION:
			{
				GetDialogManager()->UnRegistDialog(DIALOG_PETITION_SATISFACTION);
				RemoveDialog(m_pPetitionSatisfaction);
				break;
			}
		}		
	}
	else if( pMsg.Id == g_EventSobVehicleEngine )
	{
		SNtlEventSobVehicleEngine* pEvent = reinterpret_cast<SNtlEventSobVehicleEngine*>( pMsg.pData );

		if(pEvent->hSerialId != Logic_GetAvatarHandle() )
			NTL_RETURNVOID();

		if( pEvent->byMessage == VEHICLE_READY )
		{
			CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(pEvent->uiParam) );
			if( !pSobItem )
			{
				DBO_FAIL("g_EventSobVehicleEngine, Not exist sob item of handle : " << pEvent->uiParam);
				NTL_RETURNVOID();
			}

			CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
			sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();

			if( pITEM_TBLDAT->byItem_Type != ITEM_TYPE_CAPSULE )
			{
				DBO_FAIL("g_EventSobVehicleEngine, Not vehicle capsule item of handle : " << pEvent->uiParam);
				NTL_RETURNVOID();
			}

			AddDialog_no_return(m_pVehicle, CVehicleGui, "Vehicle", DIALOG_VEHICLE );
            if(!m_pVehicle)
                NTL_RETURNVOID();

			if( FALSE == m_pVehicle->RegVehicleInfo(pSobItem) )
			{
				GetDialogManager()->UnRegistDialog(DIALOG_VEHICLE);
				RemoveDialog(m_pVehicle);
				NTL_RETURNVOID();
			}

			GetDialogManager()->LocationDialogs(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
			GetDialogManager()->OpenDialog(DIALOG_VEHICLE);
		}
		else if( pEvent->byMessage == VEHICLE_ENGINE_END )
		{
			GetDialogManager()->UnRegistDialog(DIALOG_VEHICLE);
			RemoveDialog(m_pVehicle);
		}
	}
	else if( pMsg.Id == g_EventDebug_for_Developer )
	{
		sDboDeveloperData* pEvent = reinterpret_cast<sDboDeveloperData*>( pMsg.pData );

		switch(pEvent->uiType)
		{
		case DEVELOPER_PREVIEW_OPEN:
			{
				if( m_pDeveloperPreviewGui == NULL )
				{
					m_pDeveloperPreviewGui = NTL_NEW CDeveloperPreviewGui("DeveloperPreview");
					if(!m_pDeveloperPreviewGui->Create())
					{
						m_pDeveloperPreviewGui->Destroy();
						NTL_DELETE(m_pDeveloperPreviewGui);
					}
					CNtlPLGuiManager::GetInstance()->AddGui(m_pDeveloperPreviewGui);
				}
				break;
			}
		case DEVELOPER_PREVIEW_CLOSE:
			{
				RemoveDialog(m_pDeveloperPreviewGui);
				break;
			}
		}
	}
	else if( pMsg.Id == g_EventDialog )
	{
		SDboEventDialog* pEvent = reinterpret_cast<SDboEventDialog*>( pMsg.pData );

		if( DIALOGEVENT_CREATE_DOJO_GUI == pEvent->iType )
		{
			// 게임중 이 GUI가 필요하지 않을 수도 빈번하게 호출할 수도 있기에
			// 처음 호출시 생성하고 종료할 때까지 보존한다
			if( m_pDojoInfoGui == NULL )
				AddDialog_no_return(m_pDojoInfoGui, CDojoInfoGui, "DojoInfo", DIALOG_DOJO_INFO);

			if( m_pDogiGui == NULL )
				AddDialog_no_return(m_pDogiGui, CDogiGui, "DogiGui", DIALOG_DOGI);

			if( m_pDojoUpgrade == NULL )
				AddDialog_no_return(m_pDojoUpgrade, CDojoUpradeGui, "DojoUpgrade", DIALOG_DOJO_UPGRADE);
		}
	}
	else if( pMsg.Id == g_EventDojoStateNotify )
	{
		SNtlEventDojoMessage* pEvent = reinterpret_cast<SNtlEventDojoMessage*>( pMsg.pData );

		if( eDBO_DOJO_STATUS_READY <= pEvent->byDojoState && pEvent->byDojoState <= eDBO_DOJO_STATUS_END )
		{
			if( !m_pScrambleStatusGui )
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
					NTL_RETURNVOID();

				CNtlDojo*		pDojo			= pAvatar->GetDojo();
				sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();
				if( !pSCRAMBLE_INFO )
					NTL_RETURNVOID();

				if( pSCRAMBLE_INFO->uiScrambleDojoTableIndex != pEvent->uiDojoTableIndex )
					NTL_RETURNVOID();


				AddSideDialog_no_return(m_pScrambleStatusGui, CScrambleStatusGui, "CScrambleStatusGui", SDIALOG_SCRAMBLE_STATUS);
				GetSideDialogManager()->OpenDialog(SDIALOG_SCRAMBLE_STATUS);
			}
		}
		else if( eDBO_DOJO_STATUS_NORMAL == pEvent->byDojoState )
		{
			GetSideDialogManager()->UnRegistDialog(SDIALOG_SCRAMBLE_STATUS);
			RemoveDialog(m_pScrambleStatusGui);
		}
	}

	NTL_RETURNVOID();
}

void CGameGuiGroup::OnPaint(void)
{
	GetIconMoveManager()->Render();
}

void CGameGuiGroup::LoadCommonScript()
{
	gui::CResourceManager* pResMgr = CNtlPLGuiManager::GetInstance()->GetReourceManager();
	gui::CSurfaceManager* pSurMgr = CNtlPLGuiManager::GetInstance()->GetSurfaceManager();

	pResMgr->AddPage("gui\\Game.rsr");
	pSurMgr->AddPage("gui\\GameCommon.srf");
	pSurMgr->AddPage("gui\\CommonNumber.srf");
	pSurMgr->AddPage("gui\\CommonNumber2.srf");
	pSurMgr->AddPage("gui\\CommonNumber3.srf");
	pSurMgr->AddPage("gui\\particle\\DBO_2DParticle.srf");
}

void CGameGuiGroup::UnloadCommonScript()
{
	gui::CResourceManager *pResMgr = CNtlPLGuiManager::GetInstance()->GetReourceManager();
	gui::CSurfaceManager *pSurMgr = CNtlPLGuiManager::GetInstance()->GetSurfaceManager();

	pResMgr->RemovePage("Game.rsr");
	pSurMgr->RemovePage("GameCommon.srf");
	pSurMgr->RemovePage("CommonNumber.srf");
	pSurMgr->RemovePage("CommonNumber2.srf");
	pSurMgr->RemovePage("CommonNumber3.srf");
	pSurMgr->RemovePage("DBO_2DParticle.srf" );
}