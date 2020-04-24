#include "precomp_dboclient.h"
#include "windows.h"

// shared
#include "NtlParty.h"
#include "NtlItem.h"
#include "NtlWareHouse.h"
#include "ObjectTable.h"

// core
#include "NtlDebug.h"
#include "NtlSysEvent.h"

// sound
#include "NtlSoundManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSlLogic.h"
#include "NtlSLPacketGenerator.h"
#include "NtlNetSender.h"
#include "NtlFSMUtil.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobTriggerObjectAttr.h"
#include "NtlStorageManager.h"

// dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"
#include "DboEventHandler.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "PacketProc.h" 
#include "AlarmManager.h"
#include "ChatGui.h"


CDboEventHandler::CDboEventHandler()
{
}

CDboEventHandler::~CDboEventHandler()
{
}


RwBool CDboEventHandler::Create(void)
{
	NTL_FUNCTION("CDboEventHandler::Create");

	RegisterMsg(g_EventVirtualServerPacket, "g_EventVirtualServerPacketStr", "SDboEventVirtualServerPacket");
	RegisterMsg(g_EventLogInServerConnect, "g_EventLogInServerConnectStr", "SDboLogInServerConnect");
	RegisterMsg(g_EventLobbyServerConnect, "g_EventLobbyServerConnectStr", "SDboLobbyServerConnect");
	RegisterMsg(g_EventGameServerConnect, "g_EventGameServerConnectStr", "SDboGameServerConnect");

	RegisterMsg(g_EventLogInStageStateEnter, "g_EventLogInStageStateEnterStr", "SDboLogInStageStateEnter");
	RegisterMsg(g_EventLogInStageStateExit, "g_EventLogInStageStateExitStr", "SDboLogInStageStateExit");
	RegisterMsg(g_EventLogInStageTimeOut, "g_EventLogInStageTimeOutStr", "SDboLogInStageTimeOut");

	RegisterMsg(g_EventCharStageStateEnter, "g_EventCharStageStateEnterStr", "SDboCharStageStateEnter");
	RegisterMsg(g_EventCharStageStateExit, "g_EventCharStageStateExitStr", "SDboCharStageStateExit");
	RegisterMsg(g_EventLoginMessage, "g_EventLoginMessageStr", "g_EventLoginMessage");
	RegisterMsg(g_EventLobbyMessage, "g_EventLobbyMessageStr", "g_EventLobbyMessage");
	RegisterMsg(g_EventCharMake, "g_EventCharMakeStr", "g_EventCharMake");	

	RegisterMsg(g_EventDebug_for_Developer, "g_EventDebug_for_DeveloperStr", "g_EventDebug_for_Developer");
	RegisterMsg(g_EventLobbyPacketHelper, "g_EventLobbyPacketHelperStr", "g_EventLobbyPacketHelper");
	RegisterMsg(g_EventLoginGuiEnable, "g_EventLoginGuiEnableStr", "SDboEventLoginGuiEnable");
	RegisterMsg(g_EventCharSelectGuiEnable, "g_EventCharSelectGuiEnableStr", "SDboEventCharSelectGuiEnable");
    RegisterMsg(g_EventMsgBoxShow, "g_EventMsgBoxShowStr", "SDboEventMsgBoxShow");
	RegisterMsg(g_EventMsgBoxResult, "g_EventMsgBoxResultStr", "SDboEventMsgBoxResult");
	RegisterMsg(g_EventChatGuiMode, "g_EventChatGuiModeStr", "SDboEventChatGuiMode");
	RegisterMsg(g_EventSayMessage, "g_EventSayMessageStr", "SDboEventSayMessage");
	RegisterMsg(g_EventSetTextToChatInput, "g_EventSetTextToChatInputStr", " " );
	RegisterMsg(g_EventTimeFixedNotify, "g_EventTimeFixedNotifyStr", "SDboEventTimeFixedNotify");
	RegisterMsg(g_EventCautionSideNotify, "g_EventCautionSideNotify", "SDboEventCautionSideNotify");
	RegisterMsg(g_EventNotify, "g_EventdNotifyStr", "SDboEventNotify");
	RegisterMsg(g_EventFlashNotify, "g_EventFlashNotify", "SDboEventFlashNotify");	
	RegisterMsg(g_EventFlashNotifyString, "g_EventFlashNotifyString", "SDboEventFlashNotifyString");	
	RegisterMsg(g_EventFlashFinishNotify, "g_EventFlashFinishNotify", "SDboEventFlashFinishNotify");	
	RegisterMsg(g_EventFlashNotifyFinish, "g_EventFlashNotifyFinish", "SDboEventFlashNotifyFinish2");	
	RegisterMsg(g_EventGroupNotify, "g_EventGroupNotify", "SDboEventGroupNotify");
	RegisterMsg(g_EventNameNotify, "g_EventNameNotify", "SDboEventNameNotify");
	RegisterMsg(g_EventIconMoveClick, "g_EventIconMoveClick", " ");
	RegisterMsg(g_EventIconPopupShow, "g_EventPopupMenuShow", "SDboEventPopupMenuShow");
	RegisterMsg(g_EventIconPopupResult, "g_EventPopupMenuResult", "SDboEventPopupMenuResult");
	RegisterMsg(g_EventCalcPopupShow, "g_EventCalcPopupShow", "SDboEventCalcPopupShow");
	RegisterMsg(g_EventCalcPopupResult, "g_EventCalcPopupResult", "SDboEventCalcPopupResult");
	RegisterMsg(g_EventPickedUpHide, "g_EventPickedUpHide", " ");
	RegisterMsg(g_EventDialog, "g_EventDialog", " ");
	RegisterMsg(g_EventPostDialog, "g_EventPostDialog", " ");
	RegisterMsg(g_EventShop, "g_EventShopStr", " ");
	RegisterMsg(g_EventUpdateRP, "g_EventUpdateRP", "SDboEventUpdateRP");
    RegisterMsg(g_EventUpdateRpStock, "g_EventUpdateRpStock", "SDboEventUpdateRpStock");
	RegisterMsg(g_EventUpdateExp, "g_EventUpdateExp", "SDboEventUpdateExp");
	RegisterMsg(g_EventCharObjDelete, "g_EventCharObjDelete", " ");
	RegisterMsg(g_EventItemUpgradeResult, "g_EventItemUpgradeResult", "SDboItemUpgradeResult" );
	RegisterMsg(g_EventItemChangeOptionConfirmation, "g_EventItemChangeOptionConfirmation", " ");
	RegisterMsg(g_EventItemChangeOptionResult, "g_EventItemChangeOptionResult", "SDboItemChangeOptionResult");
	RegisterMsg(g_EventItemChangeBattleAttributeResult, "g_EventItemChangeBattleAttributeResult", "SDboItemChangeBattleAttributeResult");
	RegisterMsg(g_EventRegisterItemUpgrade, "g_EventRegisterItemUpgrade", "SDboRegisterItemUpgrade" );
	RegisterMsg(g_EventRegisterItemChangeBattleAttribute, "g_EventRegisterItemChangeBattleAttribute", "SDboRegisterItemUpgrade");

	RegisterMsg(g_EventCharTitleSelectRes, "g_EventCharTitleSelectRes", "SDboEventCharTitleSelectRes");
	RegisterMsg(g_EventCharTitleUpdate, "g_EventCharTitleUpdate", "SDboEventCharTitleUpdate");

	RegisterMsg(g_EventQuickSlotInfo, "g_EventQuickSlotInfo", " ");
	RegisterMsg(g_EventQuickSlotDelNfy, "g_EventQuickSlotDelNfy", "SDboEventQuickSlotDelNfy");
	RegisterMsg(g_EventNPCDialogOpen, "g_EventNPCDialogOpen", "SDboNPCDialogOpen");
	RegisterMsg(g_EventEndterWorld, "g_EventEndterWorldStr", "g_EventEndterWorld");
	RegisterMsg(g_EventMap, "g_EventMapStr", "g_EventMap");
	RegisterMsg(g_EventUserTrade, "g_EventUserTradeStr", "g_EventUserTrade");
	RegisterMsg(g_EventServerCharDialog, "g_EventServerCharDialogStr", "SDboEventServerCharDialog");
	RegisterMsg(g_EventOpenHelpWindow, "g_EventOpenHelpWindow", " ");
	RegisterMsg(g_EventOpenHelpContent, "g_EventOpenHelpContent", "SDboEventOpenHelpContent" );
	RegisterMsg(g_EventEnableItemIcon, "g_EventEnableItemIconStr", "SDboEventEnableItemIcon" );
	RegisterMsg(g_EventCapsuleLockItem, "g_EventCapsuleLockItem", "SDboEventCapsuleLockItem" );
	RegisterMsg(g_EventCapsuleLockItemWithoutBag, "g_EventCapsuleLockItemWithoutBag", "SDboEventCapsuleLockWithoutBag");
	RegisterMsg(g_EventDirectMoveIcon, "g_EventDIrectMoveIcon", "SDboEventDirectMoveIcon" );
	RegisterMsg(g_EventPrivateShopSelectNfy, "g_EventPrivateShopSelectNfy", "SDboEventPrivateShopSelectNfy" );
	RegisterMsg(g_EventRegPrivateShopCartItem, "g_EventRegPrivateShopCartItem", "SDboEventRegPrivateShopCartItem" );
	RegisterMsg(g_EventRegPrivateShopItem, "g_EventRegPrivateShopItem", "SDboEventRegPrivateShopItem");
	RegisterMsg(g_EventPrivateShopBusinessStart, "g_EventPrivateShopBusinessStart", "SDboEventPrivateShopBusinessStart");
	RegisterMsg(g_EventPrivateShopBusinessFluctuations, "g_EventPrivateShopBusinessFluctuations", "SDboEventPrivateShopBusinessFluctuations" );
	RegisterMsg(g_EventPrivateShopBusinessCloseWindow, "g_EventPrivateShopBusinessCloseWindow", "SDboEventPrivateShopBusinessCloseWindow" );
	RegisterMsg(g_EventBackboard, "g_EventBackboard", "SDboEventBackboard");
	RegisterMsg(g_EventSummonPet, "g_EventSummonPet", "SDboEventSummonPet");
	RegisterMsg(g_EventOpenSendMail, "g_EventOpenSendMail", "SDboEventOpenSendMail");	
	RegisterMsg(g_EventReadingMail, "g_EventReadingMail", "SDboEventReadingMail");	
	RegisterMsg(g_EventMailSlotUpdate, "g_EventMailSlotUpdate", "SDboEventMailSlotUpdate" );
	RegisterMsg(g_EventQuestMessage, "g_EventQuestMessage", " ");
	RegisterMsg(g_EventQuestMessage2, "g_EventQuestMessage2", "SDboEventQuestMessage2");
	RegisterMsg(g_EventRpBonusSetup, "g_EventRpBonusSetup", "SDboEventRpBonusSetup");
	RegisterMsg(g_EventRpBonusSelect, "g_EventRpBonusSelect", " ");
	RegisterMsg(g_EventRpBonusAuto, "g_EventRpBonusAuto", " ");
	RegisterMsg(g_EventTargetStatusRefresh, "g_EventTargetStatusRefresh", " ");
	RegisterMsg(g_EventRankBattleRankListRes, "g_EventRankBattleRankListRes", "SDboEventRankBattleRankListRes");
	RegisterMsg(g_EventRankBattleRankFindCharacterRes, "g_EventRankBattleRankFindCharacterRes", "SDboEventRankBattleRankFindCharacterRes");
	RegisterMsg(g_EventRankBattleRankCompareDayRes, "g_EventRankBattleRankCompareDayRes", "SDboEventRankBattleRankCompareDayRes");
	RegisterMsg(g_EventRankBoardDisable, "g_EventRankBoardDisable", " ");
	RegisterMsg(g_EventZennyLootingEffect, "g_EventZennyLootingEffect", "SDboEventZennyLootingEffect" );
	RegisterMsg(g_EventItemCreatedEffect, "g_EventItemCreatedEffect", "SDboEventItemCreatedEffect" );
	RegisterMsg(g_EventSideDialog, "g_EventSideDialog", "SDboEventSideDialog" );
	RegisterMsg(g_EventTMQRecordListRes, "g_EventTMQRecordListRes", "SDboTMQRecordListRes" );
	RegisterMsg(g_EventTMQMemberListRes, "g_EventTMQMemberListRes", "SDboTMQMemberListRes" );
	RegisterMsg(g_EventBudokaiNews, "g_EventBudokaiNews", "SDboEventBudokaiNews" );
	RegisterMsg(g_EventBudokaiPrizeWinnerName, "g_EventBudokaiPrizeWinnerName", "SDboEventBudokaiPrizeWinnerName" );
	RegisterMsg(g_EventScriptLinkNotify, "g_EventScriptLinkNotify", "SDboEventScriptLinkNotify" );
	RegisterMsg(g_EventBudokaiTournamentIndividualList, "g_EventBudokaiTournamentIndividualList", "SDboEventTournamentIndividualList" );
	RegisterMsg(g_EventBudokaiTournamentIndividualInfo, "g_EventBudokaiTournamentIndividualInfo", "SDboEventTournamentIndividualInfo" );
	RegisterMsg(g_EventBudokaiTournamentTeamList, "g_EventBudokaiTournamentTeamList", "SDboEventTournamentTeamList" );
	RegisterMsg(g_EventBudokaiTournamentTeamInfo, "g_EventBudokaiTournamentTeamInfo", "SDboEventTournamentTeamInfo" );
	RegisterMsg(g_EventDiceResult, "g_EventDiceResult", "SDboEventDiceResult" );
	RegisterMsg(g_EventDiceResultShow, "g_EventDiceResultShow", "SDboEventDiceResultShow" );
	RegisterMsg(g_EventDiceRollReqtoServer, "g_EventDiceRollReqtoServer", "SDboEventDiceRollReqtoServer" );
	RegisterMsg(g_EventDiceStart, "g_EventDiceStart", "SDboEventDiceStart" );
	RegisterMsg(g_EventShopGambleBuyRes, "g_EventShopGambleRes", "SDboEventShopGambleRes" );
	RegisterMsg(g_EventGambleUseEffect, "g_EventGambleUseEffect", "SDboEventGambleUseEffect" );
	RegisterMsg(g_EventRegMailAttachItem, "g_EventRegMailAttachItem", "SDboEventRegMailAttachItem" );

	RegisterMsg(g_EventRegBindSealingAttachItem, "g_EventRegBindSealingAttachItem", "SDboEventRegMailAttachItem");
	RegisterMsg(g_EventRegHoiPoiMixMaterialItem, "g_EventRegHoiPoiMixMaterialItem", "SDboEventRegMailAttachItem");
	RegisterMsg(g_EventRegCCBDCouponItem, "g_EventRegCCBDCouponItem", "SDboEventRegMailAttachItem");
	RegisterMsg(g_EventRegOptionReconstructItem, "g_EventRegOptionReconstructItem", "SDboEventRegMailAttachItem");
	RegisterMsg(g_EventOpenQuickTeleport, "g_EventOpenQuickTeleport", "SDboEventRegMailAttachItem");
	RegisterMsg(g_EventQuickTeleportLoad, "g_EventQuickTeleportLoad", "SDboEventQuickTeleportLoad");
	RegisterMsg(g_EventQuickTeleportUpdate, "g_EventQuickTeleportUpdate", "SDboEventQuickTeleportUpdate");
	RegisterMsg(g_EventQuickTeleportDelete, "g_EventQuickTeleportDelete", "SDboEventQuickTeleportDeleteAndMove");
	RegisterMsg(g_EventQuickTeleportMove, "g_EventQuickTeleportMove", "SDboEventQuickTeleportDeleteAndMove");

	RegisterMsg(g_EventRegAuctionHouseItem, "g_EventRegAuctionHouseItem", "SDboEventRegMailAttachItem");

	RegisterMsg(g_EventPetition, "g_EventPetition", "SDboEventPetition" );
	RegisterMsg(g_EventTeleportProposalNfy, "g_EventTeleportProposalNfy", "SDboEventTeleportProposalNfy" );
	RegisterMsg(g_EventTeleportConfirmRes, "g_EventTeleportConfirmRes", "SDboEventTeleportConfirmRes" );
	RegisterMsg(g_EventObserverMenu, "g_EventObserverMenu", "SDboEventObserver" );
	RegisterMsg(g_EventRPCharge, "g_EventRPCharge", "SDboEventRPCharge" );
	RegisterMsg(g_EventBattleCombatMode, "g_EventBattleCombatModeStr", "SDboEventBattleCombatMode");
	RegisterMsg(g_EventItemIdentifyEffect, "g_EventItemIdentifyEffect", "SDboEventItemIdentifyEffect" );
	
	RegisterMsg( g_EventHoipoiMixItemMakeRes, "g_EventHoipoiMixItemMakeRes", "SDboEventHoipoiMixItemMakeRes" );
	RegisterMsg( g_EventHoipoiMixCreateStart, "g_EventHoipoiMixCreateStart", "SDboEventHoipoiMixStartCreate" );

	RegisterMsg( g_EventHintViewClosed, "g_EventHintViewClosed", "SDboEventLoading" );

	RegisterMsg(g_EventLoading, "g_EventLoading", "SDboNetPyShopEvent" );

    RegisterMsg(g_EventOpenBagGui, "g_EventOpenBagGui", NULL);
    RegisterMsg(g_EventOpenScouterBackgroundGui, "g_EventOpenScouterBackgroundGui", NULL);

    // 한국 PC방 관련
    RegisterMsg(g_EventUpdateNetPy, "g_EventUpdateNetPy", "SDboEventUpdateNetPy");
    RegisterMsg(g_EventNetMarbleMemberShipNfy, "g_EventNetMarbleMemberShipNfy", NULL);
	RegisterMsg(g_EventNetPyShopEvent, "g_EventNetPyShopEvent", "SDboNetPyShopEvent" );

	// CommercialExtend
	RegisterMsg(g_EventCommercialExtendCommand, "g_EventCommercialExtendCommand", "SDboEventCommercialExtendCommand" );

	//hl shop
	RegisterMsg(g_EventHLShopEvent, "g_EventHLShopEvent", "SDboEventHLShopEvent");
	RegisterMsg(g_EventHLShopEventBuy, "g_EventHLShopEventBuy", "SDboEventHLShopEventBuy");
	RegisterMsg(g_EventHLShopEventGift, "g_EventHLShopEventGift", "SDboEventHLShopEventGift");
	RegisterMsg(g_EventHLShopEventItemInfo, "g_EventHLShopEventItemInfo", "SDboEventHLShopEventItemInfo");
	RegisterMsg(g_EventHLShopEventItemInfoRes, "g_EventHLShopEventItemInfoRes", "");
	RegisterMsg(g_EventHLShopEventItemBuyRes, "g_EventHLShopEventItemBuyRes", "SDboEventHLShopEventItemBuyRes");
	RegisterMsg(g_EventHLShopEventItemMoveRes, "g_EventHLShopEventItemMoveRes", "SDboEventHLShopEventItemMoveRes");
	RegisterMsg(g_EventHLShopEventItemUseRes, "g_EventHLShopEventItemUseRes", "SDboEventHLShopEventItemMoveRes");
	RegisterMsg(g_EventHLShopEventItemAddNfy, "g_EventHLShopEventItemAddNfy", "SDboEventHLShopEventItemAddNfy");
	RegisterMsg(g_EventHLShopEventItemDelNfy, "g_EventHLShopEventItemDelNfy", "SDboEventHLShopEventItemDelNfy");
	RegisterMsg(g_EventHLShopEventItemGiftRes, "g_EventHLShopEventItemGiftRes", "SDboEventHLShopEventItemGiftRes");
	
	//test		
	RegisterMsg(g_EventSCS, "g_EventSCS", "" );
	
	LinkMsg(g_EventVirtualServerPacket, 0);
	LinkMsg(g_EventNetSendError, 0);

	// SL Event Link
	LinkMsg( g_EventSysMsg, 0 );
	LinkMsg( g_EventSysStringMsg, 0 );
	LinkMsg( g_EventFormatSysMsg, 0 );
	LinkMsg( g_EventFormatSysStringMsg );
	LinkMsg( g_EventSobNpcCommunity, 0 );
	LinkMsg( g_EventSobTriggerObjCommunity, 0 );
	LinkMsg( g_EventCreateStage, 0 );
	LinkMsg( g_EventTLDisableDialogMoveOption, 0 );	
		
	NTL_RETURN(TRUE);
}

void CDboEventHandler::Destroy(void)
{
	NTL_FUNCTION("CDboEventHandler::Destroy");

	UnLinkMsg( g_EventSysMsg );
	UnLinkMsg( g_EventSysStringMsg );
	UnLinkMsg( g_EventFormatSysMsg );
	UnLinkMsg( g_EventFormatSysStringMsg );
	UnLinkMsg( g_EventSobNpcCommunity );
	UnLinkMsg(g_EventVirtualServerPacket);
	UnLinkMsg(g_EventNetSendError);
	UnLinkMsg( g_EventSobTriggerObjCommunity );
	UnLinkMsg( g_EventCreateStage);
	UnLinkMsg( g_EventTLDisableDialogMoveOption);

	UnRegisterMsg(g_EventLogInStageStateEnter);
	UnRegisterMsg(g_EventLogInStageStateExit);
	UnRegisterMsg(g_EventLogInStageTimeOut);

	UnRegisterMsg(g_EventCharStageStateEnter);
	UnRegisterMsg(g_EventCharStageStateExit);
	UnRegisterMsg(g_EventLoginMessage);
	UnRegisterMsg(g_EventLobbyMessage);	
	UnRegisterMsg(g_EventCharMake);
	
	UnRegisterMsg(g_EventVirtualServerPacket);
	UnRegisterMsg(g_EventLogInServerConnect);
	UnRegisterMsg(g_EventLobbyServerConnect);
	UnRegisterMsg(g_EventGameServerConnect);

	UnRegisterMsg(g_EventDebug_for_Developer);
	UnRegisterMsg(g_EventLobbyPacketHelper);
	UnRegisterMsg(g_EventLoginGuiEnable);
	UnRegisterMsg(g_EventCharSelectGuiEnable);
	UnRegisterMsg(g_EventMsgBoxShow);
	UnRegisterMsg(g_EventMsgBoxResult);
	UnRegisterMsg(g_EventChatGuiMode);
	UnRegisterMsg(g_EventSayMessage);
	UnRegisterMsg(g_EventSetTextToChatInput);
	UnRegisterMsg(g_EventTimeFixedNotify);
	UnRegisterMsg(g_EventCautionSideNotify);
	UnRegisterMsg(g_EventNotify);
	UnRegisterMsg(g_EventFlashNotify);
	UnRegisterMsg(g_EventFlashNotifyString);
	UnRegisterMsg(g_EventFlashFinishNotify);
	UnRegisterMsg(g_EventFlashNotifyFinish);
	UnRegisterMsg(g_EventGroupNotify);	
	UnRegisterMsg(g_EventNameNotify);
	
	UnRegisterMsg(g_EventIconMoveClick);
	UnRegisterMsg(g_EventIconPopupShow);
	UnRegisterMsg(g_EventIconPopupResult);
	UnRegisterMsg(g_EventCalcPopupShow);
	UnRegisterMsg(g_EventCalcPopupResult);
	UnRegisterMsg(g_EventPickedUpHide);
	UnRegisterMsg(g_EventDialog);
	UnRegisterMsg(g_EventPostDialog);	
	UnRegisterMsg(g_EventShop);
	UnRegisterMsg(g_EventUpdateRP);	
    UnRegisterMsg(g_EventUpdateRpStock);
	UnRegisterMsg(g_EventUpdateExp);
	UnRegisterMsg(g_EventCharObjDelete);
	UnRegisterMsg(g_EventItemUpgradeResult);
	UnRegisterMsg(g_EventItemChangeOptionConfirmation);
	UnRegisterMsg(g_EventItemChangeOptionResult);
	UnRegisterMsg(g_EventItemChangeBattleAttributeResult);
	UnRegisterMsg(g_EventRegisterItemUpgrade);
	UnRegisterMsg(g_EventRegisterItemChangeBattleAttribute);
	UnRegisterMsg(g_EventCharTitleSelectRes);
	UnRegisterMsg(g_EventCharTitleUpdate);
	UnRegisterMsg(g_EventQuickSlotInfo);
	UnRegisterMsg(g_EventQuickSlotDelNfy);
	UnRegisterMsg(g_EventNPCDialogOpen);
	UnRegisterMsg(g_EventEndterWorld);
	UnRegisterMsg(g_EventMap);
	UnRegisterMsg(g_EventUserTrade);
	UnRegisterMsg(g_EventServerCharDialog);
	UnRegisterMsg(g_EventOpenHelpWindow);
	UnRegisterMsg(g_EventOpenHelpContent);
	UnRegisterMsg(g_EventEnableItemIcon);
	UnRegisterMsg(g_EventCapsuleLockItem);
	UnRegisterMsg(g_EventCapsuleLockItemWithoutBag);
	UnRegisterMsg(g_EventDirectMoveIcon);
	UnRegisterMsg(g_EventPrivateShopSelectNfy);
	UnRegisterMsg(g_EventRegPrivateShopCartItem);
	UnRegisterMsg(g_EventRegPrivateShopItem);
	UnRegisterMsg(g_EventPrivateShopBusinessStart);
	UnRegisterMsg(g_EventPrivateShopBusinessFluctuations);
	UnRegisterMsg(g_EventPrivateShopBusinessCloseWindow);
	UnRegisterMsg(g_EventBackboard);
	UnRegisterMsg(g_EventSummonPet);
	UnRegisterMsg(g_EventOpenSendMail);
	UnRegisterMsg(g_EventReadingMail);
	UnRegisterMsg(g_EventMailSlotUpdate);
	UnRegisterMsg(g_EventQuestMessage);
	UnRegisterMsg(g_EventQuestMessage2);	
	UnRegisterMsg(g_EventRpBonusSetup);
	UnRegisterMsg(g_EventRpBonusSelect);
	UnRegisterMsg(g_EventRpBonusAuto);
	UnRegisterMsg(g_EventTargetStatusRefresh);
	UnRegisterMsg(g_EventRankBattleRankListRes);
	UnRegisterMsg(g_EventRankBattleRankFindCharacterRes);
	UnRegisterMsg(g_EventRankBattleRankCompareDayRes);
	UnRegisterMsg(g_EventRankBoardDisable);
	UnRegisterMsg(g_EventZennyLootingEffect);
	UnRegisterMsg(g_EventItemCreatedEffect);
	UnRegisterMsg(g_EventSideDialog);
	UnRegisterMsg(g_EventTMQRecordListRes);
	UnRegisterMsg(g_EventTMQMemberListRes);
	UnRegisterMsg(g_EventBudokaiNews);
	UnRegisterMsg(g_EventBudokaiPrizeWinnerName);
	UnRegisterMsg(g_EventScriptLinkNotify);
	UnRegisterMsg(g_EventBudokaiTournamentIndividualList);
	UnRegisterMsg(g_EventBudokaiTournamentIndividualInfo);
	UnRegisterMsg(g_EventBudokaiTournamentTeamList);
	UnRegisterMsg(g_EventBudokaiTournamentTeamInfo);
	UnRegisterMsg(g_EventDiceResult);
	UnRegisterMsg(g_EventDiceResultShow);
	UnRegisterMsg(g_EventDiceRollReqtoServer);	
	UnRegisterMsg(g_EventDiceStart);	
	UnRegisterMsg(g_EventShopGambleBuyRes);
	UnRegisterMsg(g_EventGambleUseEffect);
	UnRegisterMsg(g_EventRegMailAttachItem);
	UnRegisterMsg(g_EventPetition);	
	UnRegisterMsg(g_EventTeleportProposalNfy);
	UnRegisterMsg(g_EventTeleportConfirmRes);
	UnRegisterMsg(g_EventObserverMenu);	
	UnRegisterMsg(g_EventRPCharge);
	UnRegisterMsg(g_EventBattleCombatMode);
	UnRegisterMsg(g_EventItemIdentifyEffect);

	UnRegisterMsg( g_EventHoipoiMixItemMakeRes );
	UnRegisterMsg( g_EventHoipoiMixCreateStart );

	UnRegisterMsg( g_EventLoading );

	UnRegisterMsg( g_EventHintViewClosed );

	UnRegisterMsg( g_EventNetPyShopEvent );

    UnRegisterMsg( g_EventOpenBagGui );
    UnRegisterMsg( g_EventOpenScouterBackgroundGui );
    //////////////////////////////////////////////////////////////////////////
    // 한국 PC방 관련
    UnRegisterMsg(g_EventUpdateNetPy);
    UnRegisterMsg(g_EventNetMarbleMemberShipNfy);

	// CommercialExtend
    UnRegisterMsg(g_EventCommercialExtendCommand);

	//hl shop
	UnRegisterMsg(g_EventHLShopEvent);
	UnRegisterMsg(g_EventHLShopEventBuy);
	UnRegisterMsg(g_EventHLShopEventGift);
	UnRegisterMsg(g_EventHLShopEventItemInfo);
	UnRegisterMsg(g_EventHLShopEventItemInfoRes);
	UnRegisterMsg(g_EventHLShopEventItemBuyRes);
	UnRegisterMsg(g_EventHLShopEventItemMoveRes);
	UnRegisterMsg(g_EventHLShopEventItemUseRes);
	UnRegisterMsg(g_EventHLShopEventItemAddNfy);
	UnRegisterMsg(g_EventHLShopEventItemDelNfy);
	UnRegisterMsg(g_EventHLShopEventItemGiftRes);
			
	UnRegisterMsg(g_EventSCS);		
	NTL_RETURNVOID();
}

void CDboEventHandler::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CDboEventHandler::HandleEvents");

	if(pMsg.Id == g_EventVirtualServerPacket)
	{
		CPacketProc::GetInstance()->RecvVirualPackProc(pMsg.pData); 
	}
	else if(pMsg.Id == g_EventNetSendError)
	{
		SNtlEventNetSendError *pNetSendError = reinterpret_cast<SNtlEventNetSendError*>( pMsg.pData );
		CNtlNetSender *pNetSender = (CNtlNetSender*)pNetSendError->pNetSender;
		HSERVER hServer = pNetSender->GetSocketHandle();

		SConnectData *pConnectData = GetDboGlobal()->GetConnectData();
		if(hServer == pConnectData->sAuthCon.hSocket)
		{
			if(pConnectData->sAuthCon.byConnState == SConnectAtomic::SOCKET_STATE_CONNECT)
			{
				CDboEventGenerator::LogInServerConnect(SERVER_CONNECT_TYPE_CONNENCT_DISCONNECT);
				pConnectData->sAuthCon.byConnState = SConnectAtomic::SOCKET_STATE_DISCONNECT;
				pConnectData->sAuthCon.hSocket = INVALID_HSERVER;
			}
		}
		else if(hServer == pConnectData->sCharCon.hSocket)
		{
			if(pConnectData->sCharCon.byConnState == SConnectAtomic::SOCKET_STATE_CONNECT)
			{
				CDboEventGenerator::LobbyServerConnect(SERVER_CONNECT_TYPE_CONNENCT_DISCONNECT);
				pConnectData->sCharCon.byConnState = SConnectAtomic::SOCKET_STATE_DISCONNECT;
				pConnectData->sCharCon.hSocket = INVALID_HSERVER;
			}
		}
		else if(hServer == pConnectData->sGameCon.hSocket)
		{
			if(pConnectData->sGameCon.byConnState == SConnectAtomic::SOCKET_STATE_CONNECT)
			{
				CDboEventGenerator::GameServerConnect(SERVER_CONNECT_TYPE_CONNENCT_DISCONNECT);
				pConnectData->sGameCon.byConnState = SConnectAtomic::SOCKET_STATE_DISCONNECT;
				pConnectData->sGameCon.hSocket = INVALID_HSERVER;
			}
		}
		else
		{
			/*
			if(pConnectData->sGameCon.byConnState == SConnectAtomic::SOCKET_STATE_CONNECT)
			{
				CDboEventGenerator::LobbyServerConnect(FALSE);
				pConnectData->sCharCon.byConnState = SConnectAtomic::SOCKET_STATE_DISCONNECT;
				pConnectData->sCharCon.hSocket = INVALID_HSERVER;
			}
			*/
		}
	}
	else if( pMsg.Id == g_EventSysMsg )
	{
		SNtlEventSysMsg* pData = reinterpret_cast<SNtlEventSysMsg*>( pMsg.pData );

		GetAlarmManager()->AlarmMessage(pData->strMsg, pData->bServerMsg );
	}
	else if( pMsg.Id == g_EventSysStringMsg )
	{
		SNtlEventSysStringMsg* pData = reinterpret_cast<SNtlEventSysStringMsg*>( pMsg.pData );

		GetAlarmManager()->AlarmMessage( pData->szMessage, pData->eType );		
	}
	else if( pMsg.Id == g_EventFormatSysMsg )
	{
		SNtlEventFormatSysMsg* pData = reinterpret_cast<SNtlEventFormatSysMsg*>( pMsg.pData );

		GetAlarmManager()->vFormattedAlarmMessage(pData->strMsg, pData->bServerMsg, NULL, pData->args );
	}
	else if( pMsg.Id == g_EventFormatSysStringMsg )
	{
		SNtlEventFormatSysStringMsg* pData = reinterpret_cast<SNtlEventFormatSysStringMsg*>( pMsg.pData );

		GetAlarmManager()->vFormattedAlarmMessage( pData->szMessage, pData->args );
	}
	else if( pMsg.Id == g_EventSobNpcCommunity )
	{
		SNtlEventSobNpcCommunity* pData = reinterpret_cast<SNtlEventSobNpcCommunity*>( pMsg.pData );
		CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pData->hSerialId) );
		DBO_ASSERT(pActor, "CDboEventHandler::HandleEvents, g_EventSobNpcCommunity, Not exist sob object of handle : " << pData->hSerialId);

		if( Logic_IsBus(pActor) )
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

			if( FSMUtil_IsOnBus( pAvatar->GetCurrentState() ) )
			{
				CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pAvatar->GetBehaviorData());
				SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

				if( pActor->GetSerialID() == pCtrlStuff->sRide.hTargetSerialId )
				{
					// 내가 타고 있는 버스이다
					//const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_BUS_ASK_GET_OFF");
					//CDboEventGenerator::MsgBoxShow(pwcText, MBW_GET_OFF_BUS_ASK, MBTF_OK | MBTF_CANCEL);
					GetAlarmManager()->AlarmMessage( "DST_BUS_ASK_GET_OFF" );
				}
			}
			else
			{
                API_GetSLPacketGenerator()->SendRideOnBus(pActor->GetSerialID());				
			}
		}
	}
	else if( pMsg.Id == g_EventSobTriggerObjCommunity )
	{
		SNtlEventSobTriggerObjCommunity* pEvent = reinterpret_cast<SNtlEventSobTriggerObjCommunity*>( pMsg.pData );
		CNtlSobTriggerObject* pSobTriggerObject = reinterpret_cast<CNtlSobTriggerObject*>(GetNtlSobManager()->GetSobObject(pEvent->hSerialId));
		DBO_ASSERT(pSobTriggerObject, "CDboEventHandler::HandleEvents, Not exist trigger object of handle : " << pEvent->hSerialId);
		CNtlSobTriggerObjectAttr* pSobObjectAttr = reinterpret_cast<CNtlSobTriggerObjectAttr*>(pSobTriggerObject->GetSobAttr());
		sOBJECT_TBLDAT* pOBJECT_TBLDAT = reinterpret_cast<sOBJECT_TBLDAT*>(pSobObjectAttr->GetTriggerObjectTbl());
		DBO_ASSERT(pOBJECT_TBLDAT, "CDboEventHandler::HandleEvents, Not exist object table ");

		if( pOBJECT_TBLDAT->dwFunction & eDBO_TRIGGER_OBJECT_FUNC_NAMEKAN_SIGN )
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendWarFogUpdateReq(pEvent->hSerialId);
		}
	}
	else if( pMsg.Id == g_EventCreateStage )
	{
		SNtlEventStageData* pEvent = static_cast<SNtlEventStageData*>( pMsg.pData );
		if( strcmp(pEvent->chStageName, CHARACTER_STAGE_NAME) == 0 )
		{
			
		}
	}
	else if( pMsg.Id == g_EventTLDisableDialogMoveOption )
	{
		SNtlEventTLDisableDialogMoveOption* pEvent = static_cast<SNtlEventTLDisableDialogMoveOption*>( pMsg.pData );

		if( pEvent->bDiableDialogMoveOption )
		{
			CNtlSLEventGenerator::DialogEvent(TRM_DIALOG_DIALOG_DISMOVABLE);			
		}
		else
		{
			if( GetNtlStorageManager()->GetBoolData( dSTORAGE_ETC_MOVEABLEWINDOW ) )
			{
				CNtlSLEventGenerator::DialogEvent(TRM_DIALOG_DIALOG_MOVABLE);
			}
			else
			{
				CNtlSLEventGenerator::DialogEvent(TRM_DIALOG_DIALOG_DISMOVABLE);
			}
		}
	}
 
	NTL_RETURNVOID();
} 
