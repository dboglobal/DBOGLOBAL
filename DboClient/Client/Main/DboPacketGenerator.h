/*****************************************************************************
 *
 * File			: DboPacketGenerator.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 9. 27	
 * Abstract		: DBO packet generator
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DBO_PACKET_GENERATOR_H__
#define __DBO_PACKET_GENERATOR_H__

#include "NtlPacketGenerator.h"
#include "NtlCharacter.h"
#include "NtlItem.h"
#include <list>

struct sSHOP_BUY_CART;

class CLoginPacketGenerator : public CNtlPacketGenerator
{
public:

	CLoginPacketGenerator(CNtlNetSender *pNetSender);

	bool SendLonInReq(const WCHAR *pUserId, const WCHAR *pPassword, RwUInt16 wLVer, RwUInt16 wRVer, const BYTE* pMacAddress);
	bool SendLogOutReq(const WCHAR *pUserId, bool bIsEnteringCharacterServer);
};


class CLobbyPacketGenerator : public CNtlPacketGenerator
{
public:

	CLobbyPacketGenerator(CNtlNetSender *pNetSender);

	bool SendCharLogInReq(RwUInt32 uiAccountId, const RwChar *pAuthKey, RwUInt8 byServerFarmID);
	bool SendCharServerListReq();
	bool SendCharServerListOneReq();
	bool SendCharLoadReq(RwUInt32 uiAccountID, RwUInt8 byServerFarmID);	
	bool SendCharAddReq(const WCHAR *pUserName, RwUInt8 byRace, RwUInt8 byClassId, RwUInt8 byGender, 
						RwUInt8 byFace, RwUInt8 byHair, RwUInt8 byHairColor, RwUInt8 bySkinColor,
						RwUInt8 byBlood);
	bool SendCharDelReq(RwUInt32 uiCharId, const WCHAR* pwcText);
	bool SendCharDelCancelReq(RwUInt32 uiCharId, const WCHAR* pwcText);
	bool SendCharRenameReq(RwUInt32 uiCharID, DWORD dwCodePage, WCHAR* pwcCharName);
	bool SendCharConnectGameServer_WaitCheckReq(RwUInt8 byServerChannelIndex);
	bool SendCharConnectGameServer_WaitCancelReq(RwUInt8 byServerChannelIndex);
	bool SendCharSelectReq(RwUInt32 uiCharId, RwUInt8 byServerChannelIdx);
	bool SendCharDisconnectReq(BYTE byMoveType);
};

class CGamePacketGenerator : public CNtlPacketGenerator
{
public:

	CGamePacketGenerator(CNtlNetSender *pNetSender);
	virtual ~CGamePacketGenerator();

	bool SendGameEnterReq(RwUInt32 uiAccoundId, RwUInt32 uiCharId, const RwChar *pAuthKey, bool bTutorial);
	bool SendEnterWorld(void);
	bool SendAuthKeyCommunityServerReq(void);
	bool SendCharReadyForCommunityServerNfy(void);
	bool SendGameLeaveReq(void);
	bool SendCharServerExitReq(void);
	bool SendServerChangeReq(void);
	bool SendChannelChangeReq(RwUInt8 byChannelID);

	// item..
	bool SendItemMoveReq(RwUInt8 bySrcPlace, RwUInt8 bySrcSlotPos, RwUInt8 byDestPlace, RwUInt8 byDestSlostPos);
	bool SendItemStackMoveReq(RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwUInt8 byDestPlace, RwUInt8 byDestPos, RwUInt8 byStackCount);
	bool SendItemDeleteReq(RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwBool* pPacketLock );
	bool SendItemUpgradeReq(RwUInt32 hNPCSerialId, RwUInt8 byItemPlace, RwUInt8 byItemPos, RwUInt8 byStonePlace, RwUInt8 byStonePos, RwUInt8 byCorePlace, RwUInt8 byCorePos);

	bool SendItemChangeOptionReq(RwUInt8 byItemPlace, RwUInt8 byItemPos, RwUInt8 byKitPlace, RwUInt8 byKitPos, WORD wOptionIndex);

	bool SendQuickTeleportLoadReq(RwUInt8 byItemPlace, RwUInt8 byItemPos);
	bool SendQuickTeleportUpdateReq(HOBJECT hHandle, BYTE bySlot);
	bool SendQuickTeleportDelReq(HOBJECT hHandle, BYTE bySlot);
	bool SendQuickTeleportUseReq(HOBJECT hHandle, BYTE bySlot);

	bool SendItemChangeBattleAttributeReq(RwUInt32 hNPCSerialId, RwUInt8 byItemPlace, RwUInt8 byItemPos, RwUInt8 byAdditialItemPlace, RwUInt8 byAdditialItemPos, RwUInt8 byAdditionalAttribute);
		
	bool SendPickUpWorldItemReq(RwUInt32 hTargetSerialId);
	bool SendPickUpWorldZennyReq(RwUInt32 hTargetSerialId);

	bool SendQuestItemMoveReq(RwUInt8 bySrcSlotIdx, RwUInt8 byDestSlotIdx);
	bool SendQuestItemDeleteReq(RwUInt8 bySrcSlotIdx, RwBool* pPacketLock);

	bool SendSetIconToQuickSlot( RwUInt32 tblidx, RwUInt8 bySlotID, RwUInt8 byType, RwUInt8 byPlace, RwUInt8 byPos );

	bool SendUnsetIconFromQuickSlot( RwUInt8 bySlotID );

	bool SendItemSealReq(BYTE byPlace, BYTE byPos, BYTE bySealPlace, BYTE bySealPos);
	bool SendItemSealExtractReq(BYTE byPlace, BYTE byPos);

	// party select
	bool SendPartySelectStateReq(BYTE bySelectState);

	// ud
	bool SendUltimateDungeonEnterReq(HOBJECT hNpc, BYTE byDifficulty);

	// ccbd
	bool SendBattleDungeonEnterReq(HOBJECT hNpc, HOBJECT hItem);
	bool SendBattleDungeonLeaveReq();

	//
	bool SendCharRevivalReq(RwUInt8 byType);
	bool SendCharCrescentPopoRevivalReq(BYTE byPlace, BYTE byPos);

	// Skill Shop
	bool SendSkillUpgradeReq( RwUInt8 ucSlotIdx );
	bool SendSkillLearnReq( RwUInt32 uiTblIdx );
	bool SendSkillLearnByItemReq(BYTE byRequiredItemPlace, BYTE byRequiredItemPos);
	bool SendHTBLearnReq( RwUInt32 hTrainer, RwUInt32 uiTblIdx );

	// Buff
	bool SendBuffDropReq(BYTE byBuffIndex, BYTE bySourceType, RwUInt32 uiTblIdx );

	// Skill Rp Setting
	bool SendSkillRpBonusSettingReq( RwUInt8 bySlotIndex, RwUInt8 byRpBonusType , RwBool bIsRpBonusAuto);
	
	// server cheat command
	bool SendServerCommand(const WCHAR *pCmd);

	// NPC Shop
	bool SendNPCShopStartReq(RwUInt32 uiSerial, BYTE byType, RwBool* bPacketLock);
	bool SendNPCShopBuy(RwUInt32 uiNPCSerial, sSHOP_BUY_CART* pShopBuyCart[NTL_MAX_BUY_SHOPPING_CART]);
	bool SendNPCShopSell(RwInt32 uiNPCSerial, sSHOP_SELL_CART* pShopSellCart[NTL_MAX_SELL_SHOPPING_CART]);
	bool SendNPCShopClose(void);

	bool SendEventItemShopStartReq(RwInt32 uiNPCSerial, RwBool* bPacketLock);
	bool SendEventItemShopBuyReq(RwInt32 uiNPCSerial, sSHOP_BUY_CART* pShopBuyCart[NTL_MAX_BUY_SHOPPING_CART]);
	bool SendEventItemShopEndReq();

	bool SendItemRepair(RwInt32 uiNPCSerial, RwUInt8 iPlace, RwUInt8 iPos);
	bool SendItemAllRepair(RwInt32 uiNPCSerial);
	bool SendItemIdentifyReq(RwInt32 uiNPCSerial, RwUInt8 iPlace, RwUInt8 iPos);
	bool SendItemDisassembleReq(RwUInt8 iPlace, RwUInt8 iPos);

	bool SendItemSocketInsertBeadReq(RwUInt8 iPlace, RwUInt8 iPos, RwUInt8 iBeadPlace, RwUInt8 iBeadPos);
	bool SendItemSocketDestroyBeadReq(RwUInt8 iPlace, RwUInt8 iPos);

	bool SendItemExchangeReq(RwUInt32 uiNPCSerial, RwUInt8 byItemPos, RwUInt8 byMerchantTab, RwUInt8 byStack);

	// scouter & parts
	bool SendScouterMeasure(RwUInt32 uiTargetSerial);
	bool SendItemIdentification(RwUInt8 byPlace, RwUInt8 byPos);
	bool SendPCInfoView(RwUInt32 uiSerial);

	// Party
	bool SendPartyCreate(const WCHAR* pcText);
	bool SendPartyDisband();
	bool SendPartyInvite(RwUInt32 uiSerial);
	bool SendPartyInvite_CharID(RwUInt32 uiCharID);
	bool SendPartyInvite_Name(const WCHAR* pcName);
	bool SendPartyResponseInvite(RwUInt32 byResponse);
	bool SendPartyLeave();
	bool SendPartyKickOut(RwUInt32 uiSerial);
	bool SendPartyChangeLeader(RwUInt32 uiSerial);

	bool SendPartyZennyDivision(RwUInt8 byDivision);
	bool SendPartyItemDivision(RwUInt8 byDivision);

	bool SendPartyShareTargetReq(RwUInt8 bySlot, RwUInt32 hTarget);                ///< 파티 공유 타겟 선택을 서버에 요청한다.

	bool SendPartyDungeonChangeReq(RwUInt32 uiPartyDungeonState);
	bool SendPartyDungeonInitReq(void);

	// Ware House
	bool SendBankLoadReq(RwUInt32 uiNPCSerial);
	bool SendBankStart(RwUInt32 uiNPCSerial, RwBool* pSendResult);
	bool SendBankMove(RwUInt32 uiNPCSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwUInt8 byDestPlace, RwUInt8 byDestPos);
	bool SendBankMoveStack(RwUInt32 uiNPCSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwUInt8 byDestPlace, RwUInt8 byDestPos, RwUInt8 byStackCount);
	bool SendBankEnd();
	bool SendBankZenny(RwUInt32 uiNPCSerial, RwUInt32 uiZenny, bool bIsSave);
	bool SendBankBuy(RwUInt32 uiNPCSerial, RwUInt8 byMerchantTab, RwUInt8 byPos);
	bool SendBankDeleteItem(RwUInt8 byPlace, RwUInt8 byPos, RwBool* pPacketLock);

	// free pvp
	bool SendFreeBattleChallengeReq(RwUInt32 uiTargetSerial);	
	bool SendFreeBattleAcceptRes(RwUInt8 byAccept);

	// user trade
	bool SendTradeStartReq(RwUInt32 uiSerial);
	bool SendTradeOkReq(RwUInt32 uiSerial, RwUInt8 byOK);
	bool SendTradeAddReq(RwUInt32 uiTarget, RwUInt32 uiItem, RwUInt8 byCount);
	bool SendTradeDelReq(RwUInt32 uiTarget, RwUInt32 uiItem);
	bool SendTradeUpdateItem(RwUInt32 uiTarget, RwUInt32 uiItem, RwUInt8 byCount);
	bool SendTradeZennyUpdateReq(RwUInt32 uiTarget, RwUInt32 uiZenny);
	bool SendTradeEndReq(RwUInt32 uiTarget, RwUInt32 uiPacketCount, bool bLock);
	bool SendTradeCancelReq(RwUInt32 uiTarget);
	bool SendTradeDeclineReq(bool bDecline);

	bool SendCharRenameReq(const WCHAR* pwcCharName, BYTE byPlace, BYTE byPos);

	// guild
	bool SendGuildCreateReq(RwUInt32 uiNPCSerial, const WCHAR* pcGuildName);
	bool SendGuildInviteReq(RwUInt32 uiTargetSerial);
	bool SendGuildFunctionAddReq(RwUInt32 uiNPCSerial, RwUInt8 byFunction);
	bool SendGuildChangeNameReq(const WCHAR* pwcGuildName, BYTE byPlace, BYTE byPos);
	bool SendGuildCreateEmblemReq(RwUInt8 byTypeA, RwUInt8 byTypeAColor, RwUInt8 byTypeB, RwUInt8 byTypeBColor, RwUInt8 byTypeC, RwUInt8 byTypeCColor);
	bool SendGuildChangeEmblemReq(RwUInt8 byTypeA, RwUInt8 byTypeAColor, RwUInt8 byTypeB, RwUInt8 byTypeBColor, RwUInt8 byTypeC, RwUInt8 byTypeCColor);
	bool SendGuildGiveZenny(RwUInt32 uiNPCSerial, RwUInt32 uiZenny);

	bool SendGuildWarehouseStart_Req(RwUInt32 uiNPCSerial);
	bool SendGuildWarehouseMoveReq(RwUInt32 uiNPCSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwUInt8 byDestPlace, RwUInt8 byDestPos);
	bool SendGuildWarehouseMoveStackReq(RwUInt32 uiNPCSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwUInt8 byDestPlace, RwUInt8 byDestPos, RwUInt8 byStackCount);
	bool SendGuildWarehouseZennyReq(RwUInt32 uiNPCSerial, RwUInt32 uiZenny, RwBool bSave);
	bool SendGuildWarehouseEnd();

	// 도장
	bool SendDojo_CreateReq(RwUInt32 uiNPCHandle);
	bool SendDojo_FunctionAddReq(RwUInt32 uiGuildManagerHandle, RwUInt8 byFunction, RwUInt8 byPlace, RwUInt8 byPos);
	bool SendDojo_ScrambleReq(RwUInt32 uiNPCHandle);
	bool SendDojo_ScrambleResponseReq(bool bAccept);
	bool SendDojo_NPCInfoReq(RwUInt32 uiNPCSerial);

	// 도복
	bool SendDogi_CreateReq(RwUInt32 uiHandle, void* pDogiData);
	bool SendDogi_ChagneReq(RwUInt32 uiHandle, void* pDogiData);
	bool SendGuildDogi_CreateReq(RwUInt32 uiHandle, void* pDogiData);
	bool SendGuildDogi_ChagneReq(RwUInt32 uiHandle, void* pDogiData);

	// helpsystem & tutorial
	bool SendTutorialHintUpdateReq(DWORD dwHelpHint);
	bool SendTutorialWaitCancelReq();
	bool SendTutorialPlayQuitReq();

	//tmq
	bool SendTimeQuestEnterReq(HOBJECT hNpc, BYTE byDifficulty);

	// Rank Battle 
	bool SendRBInfoReq( RwUInt8 byBattleMode );
	bool SendRBJoinReq( RwUInt32 hObject, RwUInt32 tblidx );
	bool SendRBLeaveReq( RwUInt32 tblidx );
	//bool SendRBRoomListReq(RwUInt32 uiHandle, RwUInt16 wPage);
	//bool SendRBRoomInfoReq(RwUInt32 uiHandle, RwUInt32 uiRoomID);
	//bool SendRBRoomCreateReq(RwUInt32 uiHandle);
	//bool SendRBRoomLeaveReq(void);
	//bool SendRBChallengeReq(RwUInt32 uiHandle, RwUInt32 uiRoomID);
	//bool SendRBMatchCancelReq(void);
	
	// PrivateShop
	//bool SendPrivateShopCreateReq(); // SL로 이동
	bool SendPrivateShopExitReq();
	bool SendPrivateShopOpenReq(const WCHAR* pwcPrivateShopName, const WCHAR* pwcNotice, bool bIsOwnerEmpty);
	bool SendPrivateShopCloseReq();
	bool SendPrivateShopItemInsertReq(RwUInt8 uiPrivateShopPos, RwUInt8 uiInventoryPlace, RwUInt8 uiInventoryPos);
	bool SendPrivateShopItemUpdateReq(RwUInt8 uiPrivateShopPos, RwUInt32 uiZenney);
	bool SendPrivateShopItemDeleteReq(RwUInt8 uiPrivateShopPos);
	bool SendPrivateShopEnterReq(RwUInt32 uiOwnerSerial);
	bool SendPrivateShopLeaveReq(RwUInt32 uiOwnerSerial);
	bool SendPrivateShopItemSelectReq(RwUInt32 uiOwnerSerial, RwUInt8 uiPrivateShopPos, bool bSelect);
	bool SendPrivateShopItemBuyingReq(RwUInt32 uiOwnerSerial, RwUInt8 auiPrivateShopPos[NTL_MAX_BUY_SHOPPING_CART]);

	// DragonBall Collection	
	bool SendDBCCheckReq(RwUInt32 uiAltarHandle, sITEM_POSITION_DATA sData[NTL_ITEM_MAX_DRAGONBALL]);
	bool SendDBCRewardReq(RwUInt32 uiAltarHandle, TBLIDX tblIdx);

	// MailSystem
	bool SendMailStartReq(RwUInt32 hSerialID/*Mail Object or Scouter Part*/);
	bool SendMailSendReq(RwUInt32 hSerialID, const WCHAR* pwcTargetName, const WCHAR* pwcText, RwUInt8 uiMailType, sITEM_POSITION_DATA* pItemData, RwUInt32 uiZenny, RwUInt8 uiDay);
	bool SendMailReadReq(RwUInt32 hSerialID, MAILID mailID);
	bool SendMailDelReq(RwUInt32 hSerialID, MAILID mailID);
	bool SendMailReturnReq(RwUInt32 hSerialID, MAILID mailID);
	bool SendMailReloadReq(RwUInt32 hSerialID);
	//bool SendMailLoadReq(RwUInt32 hSerialID, MAILID mailID); // SL 로 이동
	bool SendMailItemReceiveReq(RwUInt32 hSerialID, MAILID mailID);
	bool SendMailLockReq(RwUInt32 hSerialID, MAILID mailID, bool bIsLock);
	bool SendCharAwayReq(bool bIsAway);
	bool SendMailMultiDelReq(RwUInt32 hObject, RwUInt8 byCount, MAILID* pMailID );

	// auction house
	bool	SendAuctionHouseListReq(CHARACTERID charId, const WCHAR* awchItemName, BYTE byClassType, BYTE byTabType, BYTE byItemType, BYTE byMinLevel, BYTE byMaxLevel, BYTE byRank, RwUInt32 uiPage, BYTE bySortType, BYTE byPage);
	bool	SendAuctionHouseSellReq(DWORD dwPrice, BYTE byPlace, BYTE byPosition, BYTE byCount, BYTE byTime);
	bool	SendAuctionHouseSellCancelReq(ITEMID nItem);
	bool	SendAuctionHouseBuyReq(ITEMID nItem);

    // Portal System
    bool SendPortalStartReq(RwUInt32 hSerialID, RwBool* pPacketLock);                    ///< 포탈 NPC를 클릭했다는 패킷을 보낸다.
    bool SendPortalAddReq(RwUInt32 hSerialID);                      ///< 포탈 NPC에 설정된 포인트의 등록을 요청한다.
    bool SendPortalReq(RwUInt32 hSerialID, BYTE byPoint);           ///< 포탈 이동을 요청한다.

	// WarFog
	bool SendWarFogUpdateReq(RwUInt32 uiTriggerObjectIndex);

	// Block Mode
	bool SendBlockModeReq(bool bIsOn);

	// KnockDown Recovery
	bool SendCharKnockDownReleaseNfy();								///< 넉다운을 회복한다고 요청한다.

    // 변신    
    bool SendTransformCancelReq();                              ///< 변신 취소를 요청한다.

	// 천하제일 무도회 신청 관련
	bool SendBudokaiJoinIndividualReq();
	bool SendBudokaiLeaveIndividualReq();
	bool SendBudokaiJoinTeamInfoReq();
	bool SendBudokaiJoinTeamReq(WCHAR* pwcTeamName);
	bool SendBudokaiLeaveTeamReq();
	bool SendBudokaiLeaveTeamMemberReq();
	bool SendBudokaiJoinInfoReq();
	bool SendBudokaiMudosaInfoReq();
	bool SendBudokaiPrizeWinnerNameReq();
	bool SendBudokaiPrizeTeamWinnerNameReq();

	// 주사위
	bool SendRoolReq();
	bool SendItemDiceReq(HOBJECT hItemHandle, BYTE byDice);

	// Gamble Buying
	bool SendShopGambleBuyReq(RwUInt32 hSerialID);

	// Teleport
	bool SendTeleportConfirmReq(bool bTeleport, RwUInt8 byTeleportIndex);

	// 스킬초기화
	bool SendSkillInitReq(RwUInt32 hSerialID);
	bool SendSkillResetPlusReq(BYTE byPlace, BYTE byPos);

	// GM
	bool SendGMBudokaiServerEnterReq();
	bool SendGMBudokaiServerLeaveReq();
	bool SendGMBudokaiMatchProgressInfoReq(RwUInt8 byMatchType);
	bool SendGMBudokaiMatchArenaEnterReq(RwUInt8 byMatchType, RwUInt8 byMatchWorldType,	RwUInt8 byMatchDepth, RwUInt8 byMatchIndex);
	bool SendGMBudokaiMatchArenaLeaveReq();

	// Hoipoi
	bool SendHoipoiMixItemCreateReq(HOBJECT hObject, TBLIDX idxRecipe, BYTE byMaterialCount, void* aMaterialSlot);

	// bus
	bool SendBusWorldMapStatus(bool bIsWorldMapOpen);

	// NetPyShop
	bool SendShopNetPyItemStartReq();
	bool SendShopNetPyItemBuyReq( RwUInt8 byBuyCount, sSHOP_BUY_CART* pBuyData );
	bool SendShopNetPyItemEndReq();

	// CommercialExtend (Duration Item)
	bool SendDurationItemBuyReq( RwUInt32 uiMerchantTblidx, BYTE bySlotPos );
	bool SendDurationRenewReq( HOBJECT hItemHandle, TBLIDX itemIdx );

	// HLS, Yardrat
	bool SendCashItemHLShopStartReq();
	bool SendCashItemHLShopEndReq();
	bool SendCashItemHLShopRefreshReq();
	bool SendCashItemHLShopBuyReq(TBLIDX uiHlsItemTblidx);
	bool SendCashItemMoveReq( RwUInt64 uiProductId );
	bool SendCashItemUseReq(RwUInt64 uiProductId);
	bool SendCashItemHLShopGiftReq(TBLIDX uiHlsItemTblidx, const WCHAR* wchTargetName);

	bool SendCharTitleSelectReq(TBLIDX tblIndex);

	bool	SendClientInfoNfy(bool bSpeedHack);
};

class CChatPacketGenerator : public CNtlPacketGenerator
{
public:

	CChatPacketGenerator(CNtlNetSender *pNetSender);

	bool SendChatEnterReq(void);
	bool SendChatMsgSay(const WCHAR *pMsg);
	bool SendChatMsgShout(const WCHAR *pMsg);
	bool SendChatMsgWhisper(const WCHAR *pMsg, const WCHAR *pReceiverName);
	bool SendChatMsgParty(RwUInt8 byChatType, const WCHAR *pMsg);
	bool SendChatMsgGuild(RwUInt8 byChatType, const WCHAR *pMsg);
	bool SendChatMsgTrade(const WCHAR *pMsg);
	bool SendChatMsgFindParty(const WCHAR *pMsg);

	bool SendPetitionUserInsertReq(const WCHAR* pwcCharName, const WCHAR* pwcPetition, RwInt32 iCategory, RwInt32 iCategory2);
	bool SendPetitionModifyReq(RwUInt32 uiPetitionID, const WCHAR* pwcPetition, RwInt32 iCategory, RwInt32 iCategory2);
	bool SendPetitionUserCancelReq(RwUInt32 uiPetitionID);

	bool SendPetitionChatStartRes(RwUInt16 wResultCode, RwUInt32 uiGMAccountID);
	bool SendPetitionChatGMSayRes(RwUInt16 wResultCode, RwUInt32 uiGMAccountID);
	bool SendPetitionChatUserSayReq(RwUInt32 uiGMAccountID, const WCHAR* pwcMessage);
	bool SendPetitionChatEndNfy(RwUInt32 uiGMAccountID);
	bool SendPetitionSatisfaction(RwUInt32 uiPetitionID, RwUInt8 bySatisfactionRate);

	// guild
	bool SendGuild_Disband_Req();
	bool SendGuild_Disband_Cancle_Req();
	bool SendGuild_Response_Invitation(RwUInt8 byResponse);
	bool SendGuild_Leave_Req();
	bool SendGuild_Kick_Out_Req(RwUInt32 uiTargetSerial);
	bool SendGuild_Appoint_Second_Master_Req(RwUInt32 uiTargetSerial);
	bool SendGuild_DIsmiss_Second_Master_Req(RwUInt32 uiTargetSerial);
	bool SendGuild_Change_Guild_Master_Req(RwUInt32 uiTargetSerial);
	bool SendGuild_Notice_Req(RwUInt16 wNoticeLength, const WCHAR* pwcNotice);

    // Friend System
    bool SendFriend_Add_Req(const WCHAR* pName);              ///< 친구 추가를 요청한다.
    bool SendFriend_Del_Req(RwUInt32 uiTargetID, const WCHAR* pName);             ///< 친구 삭제를 요청한다.
    bool SendFriend_Move_Req(RwUInt32 uiTargetID, const WCHAR* pName);            ///< 친구 리스트->블랙리스트 이동을 요청한다.
    bool SendFriend_Black_Add_Req(const WCHAR* pName);        ///< 블랙 리스트 추가를 요청한다.
    bool SendFriend_Black_Del_Req(RwUInt32 uiTargetID, const WCHAR* pName);       ///< 블랙 리스트 삭제를 요청한다.

	// Ranking Board
	bool SendRankBattle_Rank_List_Req( RwInt32 dwPage, RwInt8 byCompareDay );	// 랭킹 보드 리스트를 요청한다.
	bool SendRankBattle_Rank_Find_Character_Req( RwInt32 dwPage, RwInt8 byCompareDay, const WCHAR *pCharName );	// 검색할 캐릭터가 있는 리스트를 요청한다.
	bool SendRankBattle_Rank_Compare_Day_Req( RwInt32 dwPage, RwInt8 byCompareDay );	// 비교날짜에 따른 리스트를 요청한다.

	bool SendTMQ_Record_List_Req( RwUInt32 tmqTblidx, RwUInt8 byDifficult );
	bool SendTMQ_Member_List_Req( RwUInt32 tmqTblidx, RwUInt8 byDifficult, RwUInt8 byRank );

	// Tenkaichi Budokai
	bool SendBudokaiTournamentIndividualListReq( RwUInt8 byReceivedListCount );
	bool SendBudokaiTournamentIndividualInfoReq( RwUInt16 wJoinID1, RwUInt16 wJoinID2 );
	bool SendBudokaiTournamentTeamListReq( RwUInt8 byReceivedListCount );
	bool SendBudokaiTournamentTeamInfoReq( RwUInt16 wJoinID1, RwUInt16 wJoinID2 );	

	// 도장
	bool SendDojo_BudokaiSeedAddReq(const WCHAR* pwcCharName);
	bool SendDojo_NoticeChangeReq(RwUInt16 wNoticeLengthInUnicode, WCHAR* awcNotice);

};

#endif
