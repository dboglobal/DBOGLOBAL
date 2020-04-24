#pragma once

#include "NtlPacketCommon.h"
#include "NtlCharacter.h"
#include "NtlVector.h"
#include "NtlGuild.h"
#include "NtlBudokai.h"
#include "NtlTeleport.h"
#include "NtlDojo.h"
#include "NtlServer.h"
#include "NtlChat.h"


enum eOPCODE_GT
{
	GT_OPCODE_BEGIN = 40000,

	GT_GAME_INFO,

	GT_WORLD_CREATED_NFY,
	GT_WORLD_DESTROYED_NFY,
	GT_USER_ENTER_GAME,
	GT_USER_LEAVE_GAME,
	GT_USER_AUTH_KEY_CREATED_NFY,

	GT_UPDATE_PLAYER_POSITION,
	GT_UPDATE_PLAYER_CLASS,
	GT_UPDATE_PLAYER_LEVEL,
	GT_UPDATE_PLAYER_REPUTATION,
	GT_UPDATE_PUNISH,

	GT_PARTY_CREATED,
	GT_PARTY_DISBANDED,
	GT_PARTY_MEMBER_JOINED,
	GT_PARTY_MEMBER_LEFT,
	GT_PARTY_LEADER_CHANGED,

	GT_GUILD_CREATE,
	GT_GUILD_DELETE,
	GT_GUILD_INVITE_REQ,

	GT_SYSTEM_DISPLAY_TEXT,

	GT_GUILD_FUNCTION_ADD_REQ,
	GT_GUILD_GIVE_ZENNY_REQ,
	GT_GUILD_BANK_START_REQ,
	GT_GUILD_BANK_END_NFY,
	GT_GUILD_CREATE_MARK_REQ,							// GuildMark Create
	GT_GUILD_CHANGE_MARK_REQ,							// GuildMark Change
	GT_GUILD_CHANGE_NAME_REQ,

	GT_BUDOKAI_NOTICE_NFY,
	GT_BUDOKAI_CHANNEL_STATE_INFO_NFY,
	GT_BUDOKAI_CHANNEL_UPDATE_STATE,
	GT_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE,

	GT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES,
	GT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES,
	GT_BUDOKAI_TOURNAMENT_TEAM_LIST_RES,
	GT_BUDOKAI_TOURNAMENT_TEAM_INFO_RES,

	GT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY,			// 예선전 출전 리스트(천하제일무도회 서버 -> 일반 게임서버)
	GT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY,				// 예선전 출전 리스트(천하제일무도회 서버 -> 일반 게임서버)

	GT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ,			// 예선전 텔레포트 정보 (일반 게임서버 -> 천하제일무도회 서버)
	GT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES,			// 예선전 텔레포트 정보 (천하제일무도회 서버 -> 일반 게임서버)
	GT_BUDOKAI_PROGRESS_MESSAGE_NFY,					// 천하제일무도회 진행 알림, 예선참가자,(천하제일무도회 서버 -> 일반 게임서버)

	GT_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_REQ,				// 천하제일무도회 서버로 텔레포트 요청
	GT_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES,				// 천하제일무도회 서버로 텔레포트 요청을 한 응답.

	GT_CHAR_MARKING_UPDATE,								// 모든 게임서버로 

	GT_CHAR_SERVER_TELEPORT,							// 타 채널에서 Player 초대(소환)

	GT_DOJO_CREATE_REQ,
	GT_DOJO_DELETE_REQ,
	GT_DOJO_FUNCTION_ADD_REQ,
	GT_DOGI_CREATE_REQ,
	GT_DOGI_CHANGE_REQ,

	GT_CONNECT_VALID_CHECK_REQ,

	GT_DOJO_SCRAMBLE_REQ,								// 쟁탈전 신청
	GT_DOJO_SCRAMBLE_RESPONSE,							// 쟁탈전에 응답
	GT_DOJO_SCRAMBLE_STATE_CHANGE,						// 쟁탈전 상태변경	
	GT_DOJO_UPDATE_REQ,

	GT_CONNECT_CHANNEL_CHANGE_REQ,
	GT_DOJO_SCRAMBLE_RESET,

	GT_DOJO_SCRAMBLE_POINT_NFY,
	GT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY,

	GT_SEND_SERVER_RELAY_DATA,							// 다른 게임서버로 data 전달

	GT_DOJO_SCRAMBLE_REWARD_REQ,
	GT_DOJO_NPC_INFO_REQ,
	GT_DOJO_SEND_TELEPORT_PROPOSAL,
	GT_DOJO_COMMAND,

	GT_CHAR_READY_FOR_COMMUNITY_SERVER_NFY,

	GT_DOJO_USERCOUNT_CHANGE,

	GT_TENKAICHIDAISIJYOU_SELL_REQ,
	GT_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ,
	GT_TENKAICHIDAISIJYOU_PRICE_INFO_REQ,
	GT_TENKAICHIDAISIJYOU_BUY_REQ,
	GT_TENKAICHIDAISIJYOU_LIST_REQ,
	GT_TENKAICHIDAISIJYOU_PERIODEND_TEXT,

	GT_CHAR_NAME_CHANGE_NFY,

	GT_BROADCASTING_SYSTEM_NFY,

	GT_WAGUCOIN_INCREASE_NFY,
	GT_EVENTCOIN_INCREASE_NFY,

	GT_OPCODE_END_DUMMY,
	GT_OPCODE_END = GT_OPCODE_END_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_GT(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_GAME_INFO)
	float				fSayMessageRadius;
	float				fShoutMessageRadius;
	SERVERCHANNELID		serverChannelId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_WORLD_CREATED_NFY)
	TBLIDX				worldTblidx;
	WORLDID				worldId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_WORLD_DESTROYED_NFY)
	WORLDID				worldId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_USER_ENTER_GAME)
	ACCOUNTID			accountId;
	CHARACTERID			charId;
	HOBJECT				hHandle;
	BYTE				byRace;
	BYTE				byClass;
	BYTE				byLevel;
	DWORD				dwReputation;
	WORLDID				worldId;
	sVECTOR3			vCurrentPosition;
	WCHAR				awchName[NTL_MAX_SIZE_CHAR_NAME + 1];
	GUILDID				guildId;
	DWORD				dwPunishBitFlag;
	GMGROUPID			gmGroupId;
	TBLIDX				mapNameTblidx;
	WCHAR				awchUserID[NTL_MAX_SIZE_USERID + 1];		// User ID
	bool				bForSyncCommunity;
	WORD				wWaguWaguCoin;
	WORD				wEventCoin;
	SERVERCHANNELID		channelId;
	DWORD				dwWaguPoints;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_USER_LEAVE_GAME)
	ACCOUNTID			accountId;
	eCHARLEAVING_TYPE	eCharLeavingType;
	SERVERCHANNELID		destGameServerChannelID;
	SERVERINDEX			destGameServerIndex;
	bool				bIsKickOut;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_USER_AUTH_KEY_CREATED_NFY)
	ACCOUNTID			accountId;
	BYTE				abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_UPDATE_PLAYER_POSITION)
	ACCOUNTID			accountId;
	WORLDID				worldId;
	sVECTOR3			vCurrentPosition;
	TBLIDX				mapNameTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_UPDATE_PLAYER_CLASS)
	ACCOUNTID			accountId;
	BYTE				byNewClass;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_UPDATE_PLAYER_LEVEL)
	ACCOUNTID			accountId;
	BYTE				byNewLevel;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_UPDATE_PLAYER_REPUTATION)
	ACCOUNTID			accountId;
	DWORD				dwNewReputation;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_UPDATE_PUNISH)
ACCOUNTID				accountId;
WCHAR					awchGmCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
WCHAR					awchCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
WCHAR					wchReason[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];
DWORD					dwDurationInMinute;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_PARTY_CREATED)
	PARTYID				partyId;
	CHARACTERID			leaderCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_PARTY_DISBANDED)
	PARTYID				partyId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_PARTY_MEMBER_JOINED)
	PARTYID				partyId;
	CHARACTERID			memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_PARTY_MEMBER_LEFT)
	PARTYID				partyId;
	CHARACTERID			memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_PARTY_LEADER_CHANGED)
	PARTYID				partyId;
	CHARACTERID			newLeaderCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_GUILD_CREATE)
	CHARACTERID			creatorCharId;
	WCHAR				wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_GUILD_DELETE)
CHARACTERID			gmCharId;
WCHAR				wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_GUILD_INVITE_REQ)
	CHARACTERID			invitorCharId;
	CHARACTERID			targetCharId;
	WCHAR				wszTargetName[NTL_MAX_SIZE_CHAR_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_SYSTEM_DISPLAY_TEXT)
	SERVERCHANNELID		serverChannelId;

	WCHAR				wszGmCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE				byDisplayType;
	WCHAR				wszMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_GUILD_FUNCTION_ADD_REQ)
	CHARACTERID			masterCharId;
	BYTE				byFunction;
	DWORD				dwZenny;		// 필요금액	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_GUILD_GIVE_ZENNY_REQ)
	CHARACTERID			charId;
	DWORD				dwZenny;		// 현재 소지금
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_GUILD_BANK_START_REQ)
	CHARACTERID			charId;
	HOBJECT				hNpcHandle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_GUILD_BANK_END_NFY)
	CHARACTERID			charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_GUILD_CREATE_MARK_REQ )
	CHARACTERID			charId;
	sDBO_GUILD_MARK		sMark;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_GUILD_CHANGE_MARK_REQ )
	CHARACTERID			charId;
	sDBO_GUILD_MARK		sMark;
	DWORD				dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_GUILD_CHANGE_NAME_REQ )
	CHARACTERID			charId;
	WCHAR				wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
	BYTE				byPlace;
	BYTE				byPos;
	ITEMID				itemId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_BUDOKAI_NOTICE_NFY)
	CHARACTERID		charId;
	BYTE			byNoticeType;		// eBUDOKAI_NOTICE_TYPE

	union
	{
		// byNoticeType == BUDOKAI_NOTICE_SYSTEM 사용
		TBLIDX		tblidxNotice;		

		// byNoticeType == BUDOKAI_NOTICE_GM 사용
		struct sGM_NOTICE
		{
			BYTE		byNoticeLength;
			WCHAR		wszNotice[BUDOKAI_MAX_NOTICE_LENGTH + 1];
		};
		sGM_NOTICE	sGmNotice;
	};
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_CHANNEL_STATE_INFO_NFY )
	BUDOKAITIME							tmDefaultOpenTime;
	bool								bRankbattleAllow;
	sBUDOKAI_UPDATE_STATE_INFO			sStateInfo;
	sBUDOKAI_UPDATE_MATCH_STATE_INFO	aMatchStateInfo[MAX_BUDOKAI_MATCH_TYPE];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_CHANNEL_UPDATE_STATE )
	sBUDOKAI_UPDATE_STATE_INFO	sStateInfo;
	BUDOKAITIME					tmDefaultOpenTime;
	bool						bRankbattleAllow;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE )
	BYTE								byMatchType;	// eBUDOKAI_MATCH_TYPE
	sBUDOKAI_UPDATE_MATCH_STATE_INFO	sStateInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES )
	CHARACTERID							charId;
	WORD								wResultCode;
	bool								bSetEntryTeam;
	sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO	aEntryTeam[BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER];
	BYTE								byMatchDataCount;
	sBUDOKAI_TOURNAMENT_MATCH_DATA		aMatchData[BUDOKAI_MAX_TOURNAMENT_INDIVIDUAL_MATCH_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES )
	CHARACTERID									charId;
	WORD										wResultCode;
	sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA	asEntryData[2];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_TOURNAMENT_TEAM_LIST_RES )
	CHARACTERID							charId;
	WORD								wResultCode;
	bool								bSetEntryTeam;
	sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO	aEntryTeam[BUDOKAI_MAJOR_MATCH_TEAM_NUMBER];
	BYTE								byMatchDataCount;
	sBUDOKAI_TOURNAMENT_MATCH_DATA		aMatchData[BUDOKAI_MAX_TOURNAMENT_TEAM_MATCH_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_TOURNAMENT_TEAM_INFO_RES )
	CHARACTERID							charId;
	WORD								wResultCode;
	sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA	asEntryData[2];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY )
	BYTE									byServerChannelId;
	BYTE									byServerIndex;
	sMINORMATCH_TELEPORT_INDIVIDUAL_DATA	aTeleportData[BUDOKAI_MAX_INDIVIDUAL_MATCH_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY )
	BYTE								byServerChannelId;
	BYTE								byServerIndex;
	sMINORMATCH_TELEPORT_TEAM_DATA		aTeleportData[BUDOKAI_MAX_TEAM_MATCH_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ )
	HOBJECT								handle;
	CHARACTERID							charId;
	BYTE								byMatchType; // eBUDOKAI_MATCH_TYPE
	JOINID								joinId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES )
	BYTE								byServerChannelId;
	BYTE								byServerIndex;
	HOBJECT								handle;
	CHARACTERID							charId;
	BYTE								byMatchType; //eBUDOKAI_MATCH_TYPE
	JOINID								joinId;

	WORD								wResultCode;

	// DestInfo
	BYTE								byTeleportType;	// eTELEPORT_TYPE
	BYTE								byDestServerChannelId;
	BYTE								byDestServerIndex;
	TBLIDX								worldTblidx;
	WORLDID								worldId;
	sVECTOR3							vLoc;
	sVECTOR3							vDir;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_PROGRESS_MESSAGE_NFY )
	BYTE								byMatchType;
	BYTE								byMsgId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_REQ )
	HOBJECT								hPc;
	CHARACTERID							charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES )
	WORD								wResultCode;

	SERVERCHANNELID						svrChannelId;
	SERVERINDEX							svrIndex;
	HOBJECT								hPc;
	CHARACTERID							charId;

	sSERVERTELEPORT_INFO				sTeleportInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_CHAR_MARKING_UPDATE )
	CHARACTERID							charId;
//	sMARKING							sMarking;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_CHAR_SERVER_TELEPORT )
CHARACTERID							charId;
sSERVERTELEPORT_INFO				sTeleportInfo;		
BYTE								abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOJO_CREATE_REQ )
CHARACTERID							charId;
TBLIDX								dojoTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOJO_DELETE_REQ )
	CHARACTERID							charId;
	TBLIDX								dojoTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_DOJO_FUNCTION_ADD_REQ)
	CHARACTERID							masterCharId;
	BYTE								byFunction;
	DWORD								dwZenny;		// 필요금액	
	BYTE								byPlace;
	BYTE								byPos;
	BYTE								byStack;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOGI_CREATE_REQ )
	CHARACTERID							charId;
	sDBO_DOGI_DATA						sData;
	bool								bIsDojo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOGI_CHANGE_REQ )
	CHARACTERID							charId;
	sDBO_DOGI_DATA						sData;
	DWORD								dwZenny;	
	bool								bIsDojo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_CONNECT_VALID_CHECK_REQ )
	ACCOUNTID		accountID;
	SERVERCHANNELID gameServerChannelID;
	SERVERINDEX		gameServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOJO_SCRAMBLE_REQ )
	HOBJECT								charId;		// 신청자
	HOBJECT								dojoTblidx;	// 도장
	DWORD								dwReqZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOJO_SCRAMBLE_RESPONSE )
	HOBJECT								charId;		// 신청자
	bool								bIsAccept;	// 도장
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOJO_SCRAMBLE_STATE_CHANGE )
	TBLIDX								dojoTblidx;
	BYTE								byState;	// 도장쟁탈전 상태
	DOJOTIME							tmNextStepTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOJO_UPDATE_REQ )
	CHARACTERID							charId;
	TBLIDX								dojoTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_CONNECT_CHANNEL_CHANGE_REQ )
	HOBJECT			hObject;
	CHARACTERID		characterID;
	ACCOUNTID		accountID;
	SERVERCHANNELID gameServerChannelID;
	SERVERINDEX		gameServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOJO_SCRAMBLE_RESET )
	TBLIDX								dojoTblidx;
	BYTE								byState;	// 도장쟁탈전 상태
	DOJOTIME							tmNextStepTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOJO_SCRAMBLE_POINT_NFY )
	sDOJO_SCRAMBLE_POINT sData;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( GT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY )
	TBLIDX								dojoTblidx;
	TBLIDX								objectTblidx;
	GUILDID								guildId;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( GT_SEND_SERVER_RELAY_DATA )
	SERVERCHANNELID						channelId;
	SERVERINDEX							serverIndex;

	WORD								wDataSize;
	BYTE								aRelayData[1];
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( GT_DOJO_SCRAMBLE_REWARD_REQ )				
	TBLIDX								dojoTblidx;
	GUILDID								guildId;// winguild
	DWORD								dwAddGuildReputation;
	sITEM_DATA							sItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOJO_NPC_INFO_REQ )
	CHARACTERID							charId;
	TBLIDX								dojoTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOJO_SEND_TELEPORT_PROPOSAL )
	GUILDID								defGuildId;
	GUILDID								attGuildId;
	BYTE								byTeleportType; 
	WORD								wWaitTime;
	WORLDID								destWorldId;
	TBLIDX								destWorldTblidx;
	CNtlVector							vDestDefLoc;
	CNtlVector							vDestAttLoc;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOJO_COMMAND )
	BYTE								byCommand;
	TBLIDX								dojoTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_CHAR_READY_FOR_COMMUNITY_SERVER_NFY )
	CHARACTERID							charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GT_DOJO_USERCOUNT_CHANGE )
	TBLIDX								dojoTblidx;
	BYTE								byDefCount;
	BYTE								byAttCount;
END_PROTOCOL()
//------------------------------------------------------------------

BEGIN_PROTOCOL(GT_TENKAICHIDAISIJYOU_SELL_REQ)
CHARACTERID				charId;
ITEMID					itemId;
DWORD					dwPrice;
BYTE					byPlace;
BYTE					byPosition;
BYTE					byCount;
BYTE					byTabType;
BYTE					byItemType;
BYTE					byItemLevel;
WCHAR					awchSeller[NTL_MAX_SIZE_CHAR_NAME+1];
WCHAR					awchItemName[DBO_MAX_LENGTH_ITEM_NAME_TEXT + 1];
DWORD					dwTime;
DWORD					dwClassBitFlag;
DWORD					dwFee;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ)
CHARACTERID				charId;
ITEMID					nItem;
WCHAR					awchText[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_TENKAICHIDAISIJYOU_PRICE_INFO_REQ)
CHARACTERID				charId;
ITEMID					nItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_TENKAICHIDAISIJYOU_BUY_REQ)
CHARACTERID				charId;
ITEMID					nItem;
DWORD					dwMoney;
WCHAR					awchSystem[NTL_MAX_SIZE_CHAR_NAME+1];
WCHAR					awchSellText[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];
WCHAR					awchBuyText[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_TENKAICHIDAISIJYOU_LIST_REQ)
ACCOUNTID				accountId;
CHARACTERID				charId;
WCHAR					awchItemName[DBO_MAX_LENGTH_ITEM_NAME_TEXT + 1];
BYTE					byTabType;
BYTE					byItemType;
BYTE					byMinLevel;
BYTE					byMaxLevel;
BYTE					byRank;
unsigned int			uiPage;
BYTE					bySortType;
BYTE					byClass;
BYTE					byPage;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_TENKAICHIDAISIJYOU_PERIODEND_TEXT)
WCHAR					awchSystem[NTL_MAX_SIZE_CHAR_NAME+1];
WCHAR					awchText[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_CHAR_NAME_CHANGE_NFY)
CHARACTERID				charId;
WCHAR					wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_BROADCASTING_SYSTEM_NFY)
BYTE					byMsgType;
sMSG_BROADCAST_DATA		sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_WAGUCOIN_INCREASE_NFY)
CHARACTERID				charId;
ACCOUNTID				accountId;
DWORD					dwWaguCoin;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GT_EVENTCOIN_INCREASE_NFY)
CHARACTERID				charId;
ACCOUNTID				accountId;
DWORD					dwEventCoin;
END_PROTOCOL()
//------------------------------------------------------------------



#pragma pack()
