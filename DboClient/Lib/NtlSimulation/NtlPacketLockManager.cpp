#include "precomp_ntlsimulation.h"
#include "NtlPacketLockManager.h"

#include "NtlDebug.h"
#include "NtlPacketGU.h"
#include "NtlPacketTU.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"


#include "NtlSLLogic.h"


namespace
{
	#define dWAITING_TIME_FOR_UNLOCK		10.f
};
	
WCHAR sBuf[256];


CNtlPacketLockManager::CNtlPacketLockManager(VOID)
{
	m_mapLockPacket[GU_ITEM_MOVE_RES]									= LT_ITEM;
	m_mapLockPacket[GU_ITEM_MOVE_STACK_RES]								= LT_ITEM;
	m_mapLockPacket[GU_ITEM_DELETE_RES]									= LT_ITEM;
	m_mapLockPacket[GU_ITEM_PICK_RES]									= LT_ITEM;
	m_mapLockPacket[GU_ITEM_IDENTIFY_RES]								= LT_ITEM;
	m_mapLockPacket[GU_QUEST_ITEM_MOVE_RES]								= LT_ITEM;
	m_mapLockPacket[GU_QUEST_ITEM_DELETE_RES]							= LT_ITEM;
	m_mapLockPacket[GU_ITEM_UPGRADE_RES]								= LT_ITEM;
	m_mapLockPacket[GU_ITEM_CHANGE_BATTLE_ATTRIBUTE_RES]				= LT_ITEM;
	m_mapLockPacket[GU_ITEM_USE_RES]									= LT_ITEM;
	m_mapLockPacket[GU_ITEM_SEAL_RES] = LT_ITEM;
	m_mapLockPacket[GU_ITEM_SEAL_EXTRACT_RES]							= LT_ITEM;

	m_mapLockPacket[GU_QUICK_TELEPORT_LOAD_RES]							= LT_ITEM;
	m_mapLockPacket[GU_QUICK_TELEPORT_UPDATE_RES] = LT_ITEM;
	m_mapLockPacket[GU_QUICK_TELEPORT_DEL_RES] = LT_ITEM;
	m_mapLockPacket[GU_QUICK_TELEPORT_USE_RES] = LT_ITEM;

	m_mapLockPacket[GU_ULTIMATE_DUNGEON_ENTER_RES] = LT_WORLDCONCEPT;

	m_mapLockPacket[GU_BATTLE_DUNGEON_ENTER_RES] = LT_ITEM | LT_WORLDCONCEPT;
	m_mapLockPacket[GU_BATTLE_DUNGEON_LEAVE_RES] = LT_WORLDCONCEPT;

	m_mapLockPacket[GU_ZENNY_PICK_RES]									= LT_ZENNY;		
	
	m_mapLockPacket[GU_SKILL_LEARN_RES]									= LT_SKILL;
	m_mapLockPacket[GU_SKILL_LEARN_BY_ITEM_RES] = LT_SKILL;
	m_mapLockPacket[GU_SKILL_UPGRADE_RES]								= LT_SKILL;

	// Tutorial
	m_mapLockPacket[GU_TUTORIAL_PLAY_QUIT_RES]							= LT_WORLDCONCEPT;	

	// Rp Bonus Auto Setting
	m_mapLockPacket[GU_SKILL_RP_BONUS_SETTING_RES]						= LT_SKILL;
	
	m_mapLockPacket[GU_ITEM_REPAIR_RES]									= LT_ITEM | LT_ZENNY;
	m_mapLockPacket[GU_ITEM_EQUIP_REPAIR_RES]							= LT_ITEM | LT_ZENNY;	
	m_mapLockPacket[GU_DRAGONBALL_REWARD_RES]							= LT_ITEM | LT_ZENNY;

	m_mapLockPacket[GU_FREEBATTLE_CHALLENGE_RES]						= LT_WORLDCONCEPT;		
	m_mapLockPacket[GU_DRAGONBALL_CHECK_RES]							= LT_WORLDCONCEPT;

	m_mapLockPacket[GU_CHAR_CRESCENT_POPO_REVIVAL_RES] = LT_ITEM | LT_WORLDCONCEPT;

	// 상점
	m_mapLockPacket[GU_SHOP_START_RES]									= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_SHOP_END_RES]									= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_SHOP_BUY_RES]									= LT_ITEM | LT_ZENNY;
	m_mapLockPacket[GU_SHOP_SELL_RES]									= LT_ITEM | LT_ZENNY;	

	m_mapLockPacket[GU_SHOP_EVENTITEM_START_RES]						= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_SHOP_EVENTITEM_END_RES]							= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_SHOP_EVENTITEM_BUY_RES]							= LT_ITEM | LT_ZENNY;

	m_mapLockPacket[GU_SHOP_ITEM_IDENTIFY_RES]							= LT_ITEM | LT_ZENNY;	

	// 파티
	m_mapLockPacket[GU_PARTY_CREATE_RES]								= LT_WORLDCONCEPT;	
	m_mapLockPacket[GU_PARTY_DISBAND_RES]								= LT_WORLDCONCEPT;	
	m_mapLockPacket[GU_PARTY_INVITE_RES]								= LT_WORLDCONCEPT;	
	m_mapLockPacket[GU_PARTY_RESPONSE_INVITATION_RES]					= LT_WORLDCONCEPT;	
	m_mapLockPacket[GU_PARTY_LEAVE_RES]									= LT_WORLDCONCEPT;	
	m_mapLockPacket[GU_PARTY_KICK_OUT_RES]								= LT_WORLDCONCEPT;	
	m_mapLockPacket[GU_PARTY_CHANGE_LEADER_RES]							= LT_WORLDCONCEPT;	
	m_mapLockPacket[GU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES]			= LT_ZENNY;
	m_mapLockPacket[GU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES]			= LT_ITEM;	
	//m_mapLockPacket[GU_PARTY_OPEN_CHARM_SLOT_RES]						= LT_ITEM | LT_ZENNY;
	//m_mapLockPacket[GU_PARTY_CLOSE_CHARM_SLOT_RES]						= LT_ITEM | LT_ZENNY;
	//m_mapLockPacket[GU_PARTY_REGISTER_VICTIM_ITEM_RES]					= LT_ITEM;
	//m_mapLockPacket[GU_PARTY_UNREGISTER_VICTIM_ITEM_RES]				= LT_ITEM;
	//m_mapLockPacket[GU_PARTY_DECLARE_ZENNY_RES]							= LT_ZENNY;
	//m_mapLockPacket[GU_PARTY_ACTIVATE_CHARM_RES]						= LT_WORLDCONCEPT;	
	//m_mapLockPacket[GU_PARTY_DEACTIVATE_CHARM_RES]						= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_PARTY_DUNGEON_DIFF_RES]							= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_PARTY_DUNGEON_INIT_RES]							= LT_WORLDCONCEPT;

	//m_mapLockPacket[GU_PARTY_INVEN_ITEM_RANK_SET_RES]					= LT_ITEM;	

	//m_mapLockPacket[GU_PARTY_CHARM_DICE_ROLLED_RES]						= LT_ITEM | LT_ZENNY;
	//m_mapLockPacket[GU_PARTY_INVEST_ZENNY_RES]							= LT_ITEM | LT_ZENNY;
	//m_mapLockPacket[GU_PARTY_INVEN_LOCK_RES]							= LT_ITEM | LT_ZENNY;
	//m_mapLockPacket[GU_PARTY_INVEN_RESET_RES]							= LT_ITEM | LT_ZENNY;
	//m_mapLockPacket[GU_PARTY_INVEN_ACCEPTED_RES]						= LT_ITEM | LT_ZENNY;
	//m_mapLockPacket[GU_PARTY_INVEN_GIVE_RES]							= LT_ITEM | LT_ZENNY;
	//m_mapLockPacket[GU_PARTY_INVEN_DICE_START_RES]						= LT_ITEM | LT_ZENNY;
	//m_mapLockPacket[GU_PARTY_INVEN_DICE_ROLL_RES]						= LT_ITEM | LT_ZENNY;	

	// 은행 	
	m_mapLockPacket[GU_BANK_LOAD_RES]									= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_BANK_START_RES]									= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_BANK_END_RES]									= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_BANK_MOVE_RES]									= LT_ITEM;
	m_mapLockPacket[GU_BANK_MOVE_STACK_RES]								= LT_ITEM;
	m_mapLockPacket[GU_BANK_ITEM_DELETE_RES]							= LT_ITEM;
	m_mapLockPacket[GU_BANK_BUY_RES]									= LT_ITEM | LT_ZENNY;	
	m_mapLockPacket[GU_BANK_ZENNY_RES]									= LT_ZENNY;	

	// 유저 트레이드
	m_mapLockPacket[GU_TRADE_START_RES]									= LT_ITEM | LT_ZENNY | LT_WORLDCONCEPT;
	m_mapLockPacket[GU_TRADE_END_RES]									= LT_ITEM | LT_ZENNY | LT_WORLDCONCEPT;
	m_mapLockPacket[GU_TRADE_CANCEL_RES]								= LT_ITEM | LT_ZENNY | LT_WORLDCONCEPT;
	m_mapLockPacket[GU_TRADE_DENY_RES]									= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_TRADE_ADD_RES]									= LT_ITEM;
	m_mapLockPacket[GU_TRADE_DEL_RES]									= LT_ITEM;
	m_mapLockPacket[GU_TRADE_MODIFY_RES]								= LT_ITEM;
	m_mapLockPacket[GU_TRADE_ZENNY_UPDATE_RES]							= LT_ZENNY;	

	// 개인상점
	m_mapLockPacket[GU_PRIVATESHOP_ITEM_INSERT_RES]						= LT_ITEM;
	m_mapLockPacket[GU_PRIVATESHOP_ITEM_DELETE_RES]						= LT_ITEM;
	m_mapLockPacket[GU_PRIVATESHOP_ITEM_BUYING_RES]						= LT_ITEM | LT_ZENNY;
	
	m_mapLockPacket[GU_PRIVATESHOP_CREATE_RES]							= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_PRIVATESHOP_EXIT_RES]							= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_PRIVATESHOP_OPEN_RES]							= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_PRIVATESHOP_CLOSE_RES]							= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_PRIVATESHOP_ENTER_RES]							= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_PRIVATESHOP_LEAVE_RES]							= LT_WORLDCONCEPT;

	// tmq
	m_mapLockPacket[GU_TIMEQUEST_ENTER_RES] = LT_ITEM | LT_WORLDCONCEPT;

	// RankBattle 게시판 관련 
	//m_mapLockPacket[GU_RANKBATTLE_ROOM_LIST_RES]						= LT_WORLDCONCEPT;
	//m_mapLockPacket[GU_RANKBATTLE_ROOM_INFO_RES]						= LT_WORLDCONCEPT;
	//m_mapLockPacket[GU_RANKBATTLE_ROOM_CREATE_RES]					= LT_WORLDCONCEPT;
	//m_mapLockPacket[GU_RANKBATTLE_ROOM_LEAVE_RES]						= LT_WORLDCONCEPT;
	//m_mapLockPacket[GU_RANKBATTLE_ROOM_CHALLENGE_RES]					= LT_WORLDCONCEPT;
	//m_mapLockPacket[GU_RANKBATTLE_ROOM_MATCH_CANCEL_RES]				= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_RANKBATTLE_INFO_RES]								= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_RANKBATTLE_JOIN_RES]								= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_RANKBATTLE_LEAVE_RES]							= LT_WORLDCONCEPT;

	// Ranking Board
	m_mapLockPacket[TU_RANKBATTLE_RANK_LIST_RES]						= LT_COMMUNITY;
	m_mapLockPacket[TU_RANKBATTLE_RANK_FIND_CHARACTER_RES]				= LT_COMMUNITY;
	m_mapLockPacket[TU_RANKBATTLE_RANK_COMPARE_DAY_RES]					= LT_COMMUNITY;
	m_mapLockPacket[TU_TMQ_RECORD_LIST_RES]								= LT_COMMUNITY;
	m_mapLockPacket[TU_TMQ_MEMBER_LIST_RES]								= LT_COMMUNITY;

	// WarFog
	m_mapLockPacket[GU_WAR_FOG_UPDATE_RES]								= LT_WORLDCONCEPT;
	
	m_mapLockPacket[GU_MAIL_START_RES]									= LT_MAIL;
	m_mapLockPacket[GU_MAIL_SEND_RES]									= LT_ITEM | LT_ZENNY | LT_MAIL;
	m_mapLockPacket[GU_MAIL_READ_RES]									= LT_MAIL;
	m_mapLockPacket[GU_MAIL_DEL_RES]									= LT_MAIL;
	m_mapLockPacket[GU_MAIL_RETURN_RES]									= LT_MAIL;
	m_mapLockPacket[GU_MAIL_RELOAD_RES]									= LT_MAIL;
	m_mapLockPacket[GU_MAIL_LOAD_RES]									= LT_MAIL;
	m_mapLockPacket[GU_MAIL_ITEM_RECEIVE_RES]							= LT_ITEM | LT_ZENNY | LT_MAIL;
	m_mapLockPacket[GU_MAIL_LOCK_RES]									= LT_MAIL;
	m_mapLockPacket[GU_CHAR_AWAY_RES]									= LT_MAIL;
	m_mapLockPacket[GU_MAIL_MULTI_DEL_RES]								= LT_MAIL;

	m_mapLockPacket[GU_TENKAICHIDAISIJYOU_LIST_RES] = LT_AUCTIONHOUSE;
	m_mapLockPacket[GU_TENKAICHIDAISIJYOU_SELL_RES]	= LT_ITEM | LT_ZENNY | LT_AUCTIONHOUSE;
	m_mapLockPacket[GU_TENKAICHIDAISIJYOU_SELL_CANCEL_RES] = LT_MAIL | LT_AUCTIONHOUSE;
	m_mapLockPacket[GU_TENKAICHIDAISIJYOU_BUY_RES] = LT_MAIL | LT_AUCTIONHOUSE;

    // Portal 
    m_mapLockPacket[GU_PORTAL_INFO]										= LT_WORLDCONCEPT;
    m_mapLockPacket[GU_PORTAL_ADD_RES]                                  = LT_WORLDCONCEPT;
    m_mapLockPacket[GU_PORTAL_RES]                                      = LT_WORLDCONCEPT;

	// 길드
	m_mapLockPacket[GU_GUILD_CREATE_RES]								= LT_COMMUNITY;
	m_mapLockPacket[GU_GUILD_INVITE_RES]								= LT_COMMUNITY;
	m_mapLockPacket[TU_GUILD_DISBAND_RES]                               = LT_COMMUNITY;
	m_mapLockPacket[TU_GUILD_DISBAND_CANCEL_RES]                        = LT_COMMUNITY;
	m_mapLockPacket[TU_GUILD_RESPONSE_INVITATION_RES]					= LT_COMMUNITY;
	m_mapLockPacket[TU_GUILD_LEAVE_RES]                                 = LT_COMMUNITY;
	m_mapLockPacket[TU_GUILD_KICK_OUT_RES]                              = LT_COMMUNITY;
	m_mapLockPacket[TU_GUILD_APPOINT_SECOND_MASTER_RES]                 = LT_COMMUNITY;
	m_mapLockPacket[TU_GUILD_DISMISS_SECOND_MASTER_RES]                 = LT_COMMUNITY;
	m_mapLockPacket[TU_GUILD_CHANGE_GUILD_MASTER_RES]                   = LT_COMMUNITY;
	m_mapLockPacket[TU_GUILD_CHANGE_NOTICE_RES]					        = LT_COMMUNITY;
	m_mapLockPacket[GU_GUILD_FUNCTION_ADD_RES]					        = LT_WORLDCONCEPT;
	m_mapLockPacket[GU_GUILD_CHANGE_NAME_RES]					        = LT_WORLDCONCEPT | LT_ITEM;
	m_mapLockPacket[GU_GUILD_GIVE_ZENNY_RES]							= LT_ZENNY;
	m_mapLockPacket[GU_GUILD_BANK_START_RES]							= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_GUILD_BANK_MOVE_RES]								= LT_ITEM;
	m_mapLockPacket[GU_GUILD_BANK_MOVE_STACK_RES]						= LT_ITEM;
	m_mapLockPacket[GU_GUILD_BANK_ZENNY_RES]							= LT_ZENNY;
	m_mapLockPacket[GU_GUILD_BANK_END_RES]								= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_GUILD_CREATE_MARK_RES]							= LT_COMMUNITY;
	m_mapLockPacket[GU_GUILD_CHANGE_MARK_RES]							= LT_COMMUNITY;

	// 도장
	m_mapLockPacket[GU_DOJO_CREATE_RES]							        = LT_ITEM | LT_ZENNY | LT_WORLDCONCEPT;
	m_mapLockPacket[GU_DOJO_FUNCTION_ADD_RES]					        = LT_ZENNY;
	m_mapLockPacket[GU_DOJO_SCRAMBLE_RES]						        = LT_WORLDCONCEPT;
	m_mapLockPacket[GU_DOJO_SCRAMBLE_RESPONSE_RES]				        = LT_WORLDCONCEPT;
	m_mapLockPacket[GU_DOJO_NPC_INFO_RES]							    = LT_WORLDCONCEPT;
	m_mapLockPacket[TU_DOJO_BUDOKAI_SEED_ADD_RES]				        = LT_COMMUNITY;
	m_mapLockPacket[TU_DOJO_NOTICE_CHANGE_RES]					        = LT_COMMUNITY;

	m_mapLockPacket[GU_CHARACTER_RENAME_RES] = LT_COMMUNITY | LT_ITEM;
	
	// 도복
	m_mapLockPacket[GU_DOGI_CREATE_RES]									= LT_ITEM | LT_ZENNY;
	m_mapLockPacket[GU_DOGI_CHANGE_RES]									= LT_ITEM | LT_ZENNY;
	m_mapLockPacket[GU_GUILD_DOGI_CREATE_RES]							= LT_ITEM | LT_ZENNY;
	m_mapLockPacket[GU_GUILD_DOGI_CHANGE_RES]							= LT_ITEM | LT_ZENNY;

    // Friend
    m_mapLockPacket[TU_FRIEND_ADD_RES]                                  = LT_COMMUNITY;
    m_mapLockPacket[TU_FRIEND_DEL_RES]                                  = LT_COMMUNITY;
    m_mapLockPacket[TU_FRIEND_MOVE_RES]                                 = LT_COMMUNITY;
    m_mapLockPacket[TU_FRIEND_BLACK_ADD_RES]                            = LT_COMMUNITY;
    m_mapLockPacket[TU_FRIEND_BLACK_DEL_RES]                            = LT_COMMUNITY;

    // 버스 및 탈것
    m_mapLockPacket[GU_RIDE_ON_BUS_RES]                                 = LT_COMMUNITY | LT_ZENNY;
    m_mapLockPacket[GU_RIDE_OFF_BUS_RES]                                = LT_COMMUNITY;

	// 천하제일 무도회
	m_mapLockPacket[GU_BUDOKAI_JOIN_INDIVIDUAL_RES]						= LT_COMMUNITY;
	m_mapLockPacket[GU_BUDOKAI_LEAVE_INDIVIDUAL_RES]					= LT_COMMUNITY;
	m_mapLockPacket[GU_BUDOKAI_JOIN_TEAM_INFO_RES]						= LT_COMMUNITY;
	m_mapLockPacket[GU_BUDOKAI_JOIN_TEAM_RES]							= LT_COMMUNITY;
	m_mapLockPacket[GU_BUDOKAI_LEAVE_TEAM_RES]							= LT_COMMUNITY;
	m_mapLockPacket[GU_BUDOKAI_LEAVE_TEAM_MEMBER_RES]					= LT_COMMUNITY;
	m_mapLockPacket[GU_BUDOKAI_JOIN_INFO_RES]							= LT_COMMUNITY;		
	m_mapLockPacket[GU_BUDOKAI_MUDOSA_INFO_RES]							= LT_COMMUNITY;		
	m_mapLockPacket[GU_BUDOKAI_MUDOSA_TELEPORT_RES]						= LT_COMMUNITY;		
	m_mapLockPacket[GU_BUDOKAI_JOIN_STATE_RES]							= LT_COMMUNITY;

	m_mapLockPacket[TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES]			= LT_COMMUNITY;
	m_mapLockPacket[TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES]			= LT_COMMUNITY;
	m_mapLockPacket[TU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES]				= LT_COMMUNITY;
	m_mapLockPacket[TU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES]				= LT_COMMUNITY;

	m_mapLockPacket[GU_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES]				= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_BUDOKAI_GM_BUDOKAI_SERVER_LEAVE_RES]				= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_BUDOKAI_GM_MATCH_PROGRESS_INFO_RES]				= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_BUDOKAI_GM_MATCH_ARENA_ENTER_RES]				= LT_WORLDCONCEPT;
	m_mapLockPacket[GU_BUDOKAI_GM_MATCH_ARENA_LEAVE_RES]				= LT_WORLDCONCEPT;

	// 주사위
	m_mapLockPacket[GU_DICE_ROLL_RES]									= LT_WORLDCONCEPT;

	// Gamble
	m_mapLockPacket[GU_SHOP_GAMBLE_BUY_RES]								= LT_ITEM;

	// Petition
	m_mapLockPacket[TU_PETITION_USER_INSERT_RES]						= LT_COMMUNITY;
	m_mapLockPacket[TU_PETITION_CONTENT_MODIFY_RES]						= LT_COMMUNITY;
	m_mapLockPacket[TU_PETITION_USER_CANCEL_RES]						= LT_COMMUNITY;

	// Teleport
	m_mapLockPacket[GU_TELEPORT_CONFIRM_RES]							= LT_WORLDCONCEPT;

	// 스킬초기화
	m_mapLockPacket[GU_SKILL_INIT_RES]									= LT_SKILL;
	m_mapLockPacket[GU_SKILL_RESET_PLUS_RES]							= LT_SKILL | LT_ITEM;

	// Hoipoimix
	m_mapLockPacket[GU_HOIPOIMIX_ITEM_CREATE_RES]						= LT_ITEM;

	// Vechicle
	m_mapLockPacket[GU_VEHICLE_END_RES]									= LT_ITEM | LT_WORLDCONCEPT;
	m_mapLockPacket[GU_VEHICLE_FUEL_INSERT_RES] = LT_ITEM | LT_WORLDCONCEPT;

	// NetPyShop
	m_mapLockPacket[GU_SHOP_NETPYITEM_START_RES]						= LT_ITEM | LT_WORLDCONCEPT;
	m_mapLockPacket[GU_SHOP_NETPYITEM_BUY_RES]							= LT_ITEM | LT_WORLDCONCEPT;
	m_mapLockPacket[GU_SHOP_NETPYITEM_END_RES]							= LT_ITEM | LT_WORLDCONCEPT;

	// CommercialExtend
	m_mapLockPacket[GU_DURATION_ITEM_BUY_RES]							= LT_ITEM;
	m_mapLockPacket[GU_DURATION_RENEW_RES]								= LT_ITEM;

	// HLS, Yardrat
	m_mapLockPacket[GU_CASHITEM_HLSHOP_START_RES]						= LT_HLSHOP;
	m_mapLockPacket[GU_CASHITEM_HLSHOP_END_RES]							= LT_HLSHOP;
	m_mapLockPacket[GU_CASHITEM_HLSHOP_REFRESH_RES]						= LT_HLSHOP;
	m_mapLockPacket[GU_CASHITEM_BUY_RES] = LT_HLSHOP;
	m_mapLockPacket[GU_CASHITEM_SEND_GIFT_RES] = LT_HLSHOP;
	m_mapLockPacket[GU_CASHITEM_MOVE_RES]								= LT_ITEM;
	m_mapLockPacket[GU_CASHITEM_USE_RES]								= LT_ITEM;

	m_mapLockPacket[GU_ITEM_DISASSEMBLE_RES] = LT_ITEM;

	m_mapLockPacket[GU_ITEM_SOCKET_DESTROY_BEAD_RES] = LT_ITEM;
	m_mapLockPacket[GU_ITEM_SOCKET_INSERT_BEAD_RES] = LT_ITEM;

	m_mapLockPacket[GU_ITEM_EXCHANGE_RES] = LT_ITEM;

	// Gmt
	m_mapLockPacket[GU_GMT_UPDATE_RES]									= LT_COMMUNITY;

	m_mapLockPacket[GU_CHARTITLE_SELECT_RES] = LT_WORLDCONCEPT;

	m_mapLockPacket[GU_PARTY_SELECT_STATE_RES] = LT_WORLDCONCEPT;



	m_listLockedTime.push_back( sLockedPacket(LT_ITEM) );
	m_listLockedTime.push_back( sLockedPacket(LT_ZENNY) );
	m_listLockedTime.push_back( sLockedPacket(LT_WORLDCONCEPT) );
	m_listLockedTime.push_back( sLockedPacket(LT_MAIL) );
	m_listLockedTime.push_back( sLockedPacket(LT_SKILL) );
	m_listLockedTime.push_back( sLockedPacket(LT_COMMUNITY) );
	m_listLockedTime.push_back(sLockedPacket(LT_AUCTIONHOUSE));
	m_listLockedTime.push_back(sLockedPacket(LT_HLSHOP));
}

CNtlPacketLockManager::~CNtlPacketLockManager(VOID)
{
	m_mapLockPacket.clear();
	m_setLockPacket.clear();
	m_listLockedTime.clear();
}

VOID CNtlPacketLockManager::Reset(VOID)
{
	m_setLockPacket.clear();
	m_listLockedTime.clear();
}

VOID CNtlPacketLockManager::Update(RwReal fElapsed)
{
	LIST_LOCKED_TIME_ITER it_time = m_listLockedTime.begin();
	for( ; it_time != m_listLockedTime.end() ; ++it_time )
	{
		sLockedPacket& lockedPacket = *it_time;

		if( lockedPacket.uiPacket != 0 )
		{
			lockedPacket.fElapsed += fElapsed;

#ifdef NDEBUG
			if( lockedPacket.fElapsed >= dWAITING_TIME_FOR_UNLOCK )
			{
				lockedPacket.fElapsed = 0.f;
				lockedPacket.uiPacket = 0;
			}
#endif
		}		
	}
}

RwBool CNtlPacketLockManager::Lock( RwInt32 eLockPacket )
{
	MAP_LOCKPACKET::iterator it = m_mapLockPacket.find( eLockPacket );

	if( it == m_mapLockPacket.end() )
	{
		DBO_FAIL("None Register lock packet " << eLockPacket );
		return FALSE;
	}

	LIST_LOCKED_TIME_ITER it_time = m_listLockedTime.begin();
	for( ; it_time != m_listLockedTime.end() ; ++it_time )
	{
		sLockedPacket& lockedPacket = *it_time;

		if( lockedPacket.eType & it->second )
		{
			lockedPacket.uiPacket = eLockPacket;
			lockedPacket.fElapsed = 0.f;

//#ifndef NDEBUG
//			swprintf_s( sBuf, 256, L"Packet lock : %d", lockedPacket.uiPacket );			
//			CNtlSLEventGenerator::SysMsg( sBuf );
//#endif
		}
	}

	return TRUE;
}

RwBool CNtlPacketLockManager::Unlock( RwInt32 eLockPacket )
{
	MAP_LOCKPACKET::iterator it = m_mapLockPacket.find( eLockPacket );

	if( it == m_mapLockPacket.end() )
	{
		DBO_FAIL("None Register lock packet " << eLockPacket );
		return FALSE;
	}

	LIST_LOCKED_TIME_ITER it_time = m_listLockedTime.begin();
	for( ; it_time != m_listLockedTime.end() ; ++it_time )
	{
		sLockedPacket& lockedPacket = *it_time;

		if( lockedPacket.eType & it->second )
		{
			lockedPacket.uiPacket = 0;
			lockedPacket.fElapsed = 0.f;

//#ifndef NDEBUG
//			swprintf_s( sBuf, 256, L"Packet Unlock : %d", lockedPacket.uiPacket );		
//			CNtlSLEventGenerator::SysMsg( sBuf );
//#endif
		}
	}

	return TRUE;
}

RwBool CNtlPacketLockManager::IsLock( RwInt32 eLockPacket )
{
	MAP_LOCKPACKET::iterator it = m_mapLockPacket.find( eLockPacket );

	if( it == m_mapLockPacket.end() )
	{
		DBO_FAIL("None Register lock packet " << eLockPacket );
		return FALSE;
	}

	LIST_LOCKED_TIME_ITER it_time = m_listLockedTime.begin();
	for( ; it_time != m_listLockedTime.end() ; ++it_time )
	{
		sLockedPacket& lockedPacket = *it_time;

		if( lockedPacket.eType & it->second )
		{
			if( lockedPacket.uiPacket != 0 )
			{
#ifndef NDEBUG
				swprintf_s( sBuf, 256, L"%d Packet IsLock", lockedPacket.uiPacket );		
				CNtlSLEventGenerator::SysMsg( sBuf );
#else 
				if(g_bDispPacketLockMsg)
				{
					swprintf_s( sBuf, 256, L"%d Packet IsLock",lockedPacket.uiPacket );		
					CNtlSLEventGenerator::SysMsg( sBuf );
				}
#endif
				return TRUE;
			}
		}
	}

	return FALSE;
}

RwBool CNtlPacketLockManager::IsLockType( RwInt32 eLockedType )
{
	LIST_LOCKED_TIME_ITER it_time = m_listLockedTime.begin();
	for( ; it_time != m_listLockedTime.end() ; ++it_time )
	{
		sLockedPacket& lockedPacket = *it_time;

		if( lockedPacket.eType & eLockedType )
		{
			if( lockedPacket.uiPacket != 0 )
			{
#ifndef NDEBUG
				swprintf_s( sBuf, 256, L"%d Packet",lockedPacket.uiPacket );		
				CNtlSLEventGenerator::SysMsg( sBuf );
#else 
				if(g_bDispPacketLockMsg)
				{
					swprintf_s( sBuf, 256, L"%d Packet",lockedPacket.uiPacket );		
					CNtlSLEventGenerator::SysMsg( sBuf );
				}
#endif
				return TRUE;
			}
		}
	}

	return FALSE;
}

RwBool CNtlPacketLockManager::IsLock_withoutMessage( RwInt32 eLockPacket )
{
	MAP_LOCKPACKET::iterator it = m_mapLockPacket.find( eLockPacket );

	if( it == m_mapLockPacket.end() )
	{
		DBO_FAIL("None Register lock packet " << eLockPacket );
		return FALSE;
	}

	LIST_LOCKED_TIME_ITER it_time = m_listLockedTime.begin();
	for( ; it_time != m_listLockedTime.end() ; ++it_time )
	{
		sLockedPacket& lockedPacket = *it_time;

		if( lockedPacket.eType & it->second )
		{
			if( lockedPacket.uiPacket != 0 )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

RwBool CNtlPacketLockManager::IsLockType_withoutMessage( RwInt32 eLockedType )
{
	LIST_LOCKED_TIME_ITER it_time = m_listLockedTime.begin();
	for( ; it_time != m_listLockedTime.end() ; ++it_time )
	{
		sLockedPacket& lockedPacket = *it_time;

		if( lockedPacket.eType & eLockedType )
		{
			if( lockedPacket.uiPacket != 0 )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

RwBool CNtlPacketLockManager::IndividualLock( RwInt32 eLockPacket )
{
	SET_LOCKPACKET::iterator it = m_setLockPacket.find( eLockPacket );

	if( it != m_setLockPacket.end() )
	{
		DBO_FAIL( "This packet is already individual lock!" << eLockPacket );
		return FALSE;
	}

	m_setLockPacket.insert( eLockPacket );

	return TRUE;
}

RwBool CNtlPacketLockManager::IndividualUnlock( RwInt32 eLockPacket )
{
	SET_LOCKPACKET::iterator it = m_setLockPacket.find( eLockPacket );

	if( it == m_setLockPacket.end() )
	{
		DBO_FAIL("This packet isn't lock yet");
		return FALSE;
	}

	m_setLockPacket.erase( it );

	return TRUE;
}

RwBool CNtlPacketLockManager::IsIndividualLock( RwInt32 eLockPacket )
{
	SET_LOCKPACKET::iterator it = m_setLockPacket.find( eLockPacket );

	if( it != m_setLockPacket.end() )
	{
#ifndef NDEBUG
		swprintf_s( sBuf, 256, L"%d Packet IsIndividualLock",eLockPacket);		
		CNtlSLEventGenerator::SysMsg( sBuf );
#else 
		if(g_bDispPacketLockMsg)
		{
			swprintf_s( sBuf, 256, L"%d Packet IsIndividualLock",eLockPacket);		
			CNtlSLEventGenerator::SysMsg( sBuf );
		}
#endif

		return TRUE;
	}

	return FALSE;
}