#pragma once

#include "NtlPacketCommon.h"
#include "NtlCharacter.h"
#include "NtlGuild.h"
#include "NtlParty.h"
#include "NtlBudokai.h"
#include "NtlTeleport.h"
#include "NtlDojo.h"
#include "NtlServer.h"
#include "NtlHlsItem.h"


enum eOPCODE_TG
{
	TG_OPCODE_BEGIN = 41000,

	TG_HEARTBEAT = TG_OPCODE_BEGIN,
	TG_USER_ENTER_GAME_ACK,
	TG_USER_LEAVE_GAME_ACK,
	TG_USER_AUTH_KEY_CREATED_ACK,

	TG_PARTY_CREATED_ACK,
	TG_PARTY_DISBANDED_ACK,
	TG_PARTY_MEMBER_JOINED_ACK,
	TG_PARTY_MEMBER_LEFT_ACK,
	TG_PARTY_LEADER_CHANGED_ACK,

	TG_GUILD_CREATE_RES,
	TG_GUILD_INVITE_RES,
	TG_GUILD_CREATED_NFY,
	TG_GUILD_DESTROYED_NFY,
	TG_GUILD_INFO_REFRESHED_NFY,
	TG_GUILD_PLAYER_GUILDID_CHANGED_NFY,

	TG_GUILD_FUNCTION_ADD_RES,
	TG_GUILD_GIVE_ZENNY_RES,
	TG_GUILD_BANK_START_RES,
	TG_GUILD_CREATE_MARK_RES,							// GuildMark Create
	TG_GUILD_CHANGE_MARK_RES,							// GuildMark Change
	TG_GUILD_CHANGE_NAME_RES,
	TG_GUILD_BANK_END_NFY,

	TG_BUDOKAI_CHANNEL_STATE_INFO_NFY,
	TG_BUDOKAI_CHANNEL_UPDATE_STATE,
	TG_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE,

	TG_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ,
	TG_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ,
	TG_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ,
	TG_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ,

	TG_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY,			// 예선전 출전 리스트(천하제일무도회 서버 -> 일반 게임서버)
	TG_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY,				// 예선전 출전 리스트(천하제일무도회 서버 -> 일반 게임서버)

	TG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ,			// 예선전 텔레포트 정보 (일반 게임서버 -> 천하제일무도회 서버)
	TG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES,			// 예선전 텔레포트 정보 (천하제일무도회 서버 -> 일반 게임서버)
	TG_BUDOKAI_PROGRESS_MESSAGE_NFY,					// 진행 메시지,(예선전만)(천하제일무도회 서버 -> 일반 게임서버)

	TG_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_REQ,
	TG_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES,

	TG_CHAR_MARKING_UPDATE_NFY,							// 칭호 업데이트

	TG_CHAR_SERVER_TELEPORT,							// Teleport player to other channel

	TG_DOJO_CREATED_NFY,
	TG_DOJO_DESTROYED_NFY,
	TG_DOJO_SERVER_NFY,
	TG_DOJO_CREATE_RES,
	TG_DOJO_DELETE_RES,
	TG_DOJO_FUNCTION_ADD_RES,
	TG_DOGI_CREATE_RES,
	TG_DOGI_CHANGE_RES,

	TG_CONNECT_VALID_CHECK_RES,

	TG_DOJO_SCRAMBLE_RES,
	TG_DOJO_SCRAMBLE_RESPONSE,
	TG_CONNECT_CHANNEL_CHANGE_RES,
	TG_DOJO_UPDATE_RES,
	TG_SEND_SERVER_RELAY_DATA,
	TG_DOJO_SCRAMBLE_STATE_CHANGE,

	TG_DOJO_SEND_ATT_GUILD_ID,

	TG_DOJO_NPC_INFO_RES,
	TG_DOJO_SEND_TELEPORT_PROPOSAL_NFY,
	TG_DOJO_COMMAND,
	TG_DOJO_USERCOUNT_CHANGE,

	TG_RANKBATTLE_REMAIN_COUNT_FLUSH_NFY,
	TG_DAILY_LIMIT_COUNT_RESET_NFY,
	TG_MAIL_RECV_SERVER_NFY,
//	TG_GUILD_DOGI_DYE_RES,

	TG_TENKAICHIDAISIJYOU_SELL_RES,
	TG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES,
	TG_TENKAICHIDAISIJYOU_PRICE_INFO_RES,
	TG_TENKAICHIDAISIJYOU_BUY_RES,
	TG_TENKAICHIDAISIJYOU_LIST_DATA,
	TG_TENKAICHIDAISIJYOU_LIST_RES,
	TG_TENKAICHIDAISIJYOU_PERIODEND_RES,
	TG_TENKAICHIDAISIJYOU_PERIODEND_TEXT,

	TG_WAGUCOIN_DECREASE_NFY,
	//TG_EVENTCOIN_DECREASE_NFY,
	TG_WAGUWAGUMACHINE_UPDATE_CASHITEM_INFO,
	//TG_CHARTITLE_SELECT_NFY,
	//TG_CHARTITLE_ADD_NFY,
	//TG_CHARTITLE_DELETE_NFY,
	//TG_TRADE_BLACK_OK_FIND,
	//TG_TRADE_BLACK_END_FIND,

	TG_OPCODE_END_DUMMY,	
	TG_OPCODE_END = TG_OPCODE_END_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_TG(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_USER_ENTER_GAME_ACK)
	WORD				wResultCode;
	ACCOUNTID			accountId;
	bool				bForSyncCommunity; //true if chat-server restart.
	BYTE				abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_USER_LEAVE_GAME_ACK)
	ACCOUNTID			accountId;
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_USER_AUTH_KEY_CREATED_ACK)
	WORD				wResultCode;
	ACCOUNTID			accountId;
	BYTE				abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_PARTY_CREATED_ACK)
	WORD				wResultCode;
	PARTYID				partyId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_PARTY_DISBANDED_ACK)
	WORD				wResultCode;
	PARTYID				partyId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_PARTY_MEMBER_JOINED_ACK)
	WORD				wResultCode;
	PARTYID				partyId;
	CHARACTERID			memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_PARTY_MEMBER_LEFT_ACK)
	WORD				wResultCode;
	PARTYID				partyId;
	CHARACTERID			memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_PARTY_LEADER_CHANGED_ACK)
	WORD				wResultCode;
	PARTYID				partyId;
	CHARACTERID			newLeaderCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_GUILD_CREATE_RES)
	WORD				wResultCode;
	CHARACTERID			creatorCharId;
	DWORD				dwSpentZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_GUILD_INVITE_RES)
	WORD				wResultCode;
	CHARACTERID			invitorCharId;
	WCHAR				wszTargetName[NTL_MAX_SIZE_CHAR_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_GUILD_CREATED_NFY)
	GUILDID				guildId;
	WCHAR				wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
	sDBO_GUILD_MARK		guildMark;
	sDBO_DOGI_DATA		sDogi;
	DWORD64				qwGuildFunctionFlag;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_GUILD_DESTROYED_NFY)
GUILDID					guildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_GUILD_INFO_REFRESHED_NFY)
GUILDID				guildId;
WCHAR				wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
sDBO_GUILD_MARK		sMark;
sDBO_DOGI_DATA		sDogi;
DWORD64				qwGuildFunctionFlag;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_GUILD_PLAYER_GUILDID_CHANGED_NFY)
CHARACTERID			charId;
GUILDID				newGuildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_GUILD_FUNCTION_ADD_RES)
	WORD				wResultCode;
	CHARACTERID			masterCharId;
	DWORD				dwZenny;
	DWORD64				qwGuildFunctionFlag;
	BYTE				byPlace;
	BYTE				byPos;
	BYTE				byStack;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_GUILD_GIVE_ZENNY_RES)
	WORD				wResultCode;
	CHARACTERID			charId;
	DWORD				dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_GUILD_BANK_START_RES)
	WORD				wResultCode;
	CHARACTERID			charId;
	HOBJECT				hNpcHandle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_GUILD_CREATE_MARK_RES )
	GUILDID				guildId;
	CHARACTERID			charId;
	WORD				wResultCode;
	sDBO_GUILD_MARK		sMark;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_GUILD_CHANGE_MARK_RES )
	GUILDID				guildId;
	CHARACTERID			charId;
	WORD				wResultCode;
	sDBO_GUILD_MARK		sMark;
	DWORD				dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_GUILD_CHANGE_NAME_RES )
	GUILDID				guildId;
	CHARACTERID			charId;
	WORD				wResultCode;
	WCHAR				wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
	BYTE				byPlace;
	BYTE				byPos;
	ITEMID				itemId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_GUILD_BANK_END_NFY)
	WORD				wResultCode;
	CHARACTERID			charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_BUDOKAI_CHANNEL_STATE_INFO_NFY )
	BUDOKAITIME							tmDefaultOpenTime;
	bool								bRankbattleAllow;
	sBUDOKAI_UPDATE_STATE_INFO			sStateInfo;
	sBUDOKAI_UPDATE_MATCH_STATE_INFO	aMatchStateInfo[MAX_BUDOKAI_MATCH_TYPE];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_BUDOKAI_CHANNEL_UPDATE_STATE )
	sBUDOKAI_UPDATE_STATE_INFO	sStateInfo;
	BUDOKAITIME					tmDefaultOpenTime;
	bool						bRankbattleAllow;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE )
	BYTE								byMatchType;	// eBUDOKAI_MATCH_TYPE
	sBUDOKAI_UPDATE_MATCH_STATE_INFO	sStateInfo;
END_PROTOCOL()

//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ)
	CHARACTERID					charId;
	BYTE						byReceivedListCount;		// 클라이언트에 저장하고 있는 리스트(경기 정보)의 갯수
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ)
	CHARACTERID					charId;
	WORD						awJoinId[2];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ)
	CHARACTERID					charId;
	BYTE						byReceivedListCount;		// 클라이언트에 저장하고 있는 리스트(경기 정보)의 갯수
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ)
	CHARACTERID					charId;
	WORD						awJoinId[2];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY )
	BYTE									byServerChannelId;
	BYTE									byServerIndex;
	sMINORMATCH_TELEPORT_INDIVIDUAL_DATA	aTeleportData[BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY )
	BYTE								byServerChannelId;
	BYTE								byServerIndex;
	sMINORMATCH_TELEPORT_TEAM_DATA		aTeleportData[BUDOKAI_MAJOR_MATCH_TEAM_NUMBER];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ )
	BYTE								byServerChannelId;
	BYTE								byServerIndex;
	HOBJECT								handle;
	CHARACTERID							charId;
	BYTE								byMatchType; //eBUDOKAI_MATCH_TYPE
	JOINID								joinId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES )
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
BEGIN_PROTOCOL( TG_BUDOKAI_PROGRESS_MESSAGE_NFY )
	BYTE								byMatchType;
	BYTE								byMsgId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_REQ )
	SERVERCHANNELID						svrChannelId;
	SERVERINDEX							svrIndex;
	HOBJECT								hPc;
	CHARACTERID							charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES )
	WORD								wResultCode;

	HOBJECT								hPc;
	CHARACTERID							charId;

	sSERVERTELEPORT_INFO				sTeleportInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_CHAR_MARKING_UPDATE_NFY)
	CHARACTERID							charId;
//	sMARKING							sMarking;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_CHAR_SERVER_TELEPORT )
CHARACTERID							charId;
sSERVERTELEPORT_INFO				sTeleportInfo;	
BYTE								abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOJO_CREATED_NFY )
	sDBO_DOJO_DATA						sDojoData;		
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_DOJO_DESTROYED_NFY)
	GUILDID								guildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOJO_SERVER_NFY )
	SERVERCHANNELID						svrChannelId;
	SERVERINDEX							svrIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOJO_CREATE_RES )
	WORD								wResultCode;
	CHARACTERID							charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOJO_DELETE_RES )
	WORD								wResultCode;
	CHARACTERID							charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOJO_FUNCTION_ADD_RES )
WORD				wResultCode;
CHARACTERID			masterCharId;
DWORD				dwZenny;
DWORD64				qwGuildFunctionFlag;
BYTE				byFunction;
GUILDID				guildId;
END_PROTOCOL()
//------------------------------------------------------------------	
BEGIN_PROTOCOL( TG_DOGI_CREATE_RES )
	WORD								wResultCode;
	CHARACTERID							charId;
	sDBO_DOGI_DATA						sData;
	bool								bIsDojo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOGI_CHANGE_RES )
	WORD								wResultCode;
	CHARACTERID							charId;
	sDBO_DOGI_DATA						sData;
	DWORD								dwZenny;
	bool								bIsDojo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_CONNECT_VALID_CHECK_RES )
	WORD			wResultCode;
	ACCOUNTID		accountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOJO_SCRAMBLE_RES )
	HOBJECT								charId;		// 신청자
	WORD								wResultCode;
	DWORD								dwReqZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOJO_SCRAMBLE_RESPONSE )
	HOBJECT								charId;		// 도장리더
	bool								bIsAccept;	
	WORD								wResultCode;
	bool								bIsRetry;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_CONNECT_CHANNEL_CHANGE_RES )
	WORD			wResultCode;
	ACCOUNTID		accountID;
	HOBJECT			hObject;
	CHARACTERID		characterID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOJO_UPDATE_RES )
	WORD								wResultCode;
	GUILDID								guildId;
	GUILDID								newGuildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_SEND_SERVER_RELAY_DATA )
	SERVERCHANNELID						fromChannelId;
	SERVERINDEX							fromServerIndex;

	WORD								wDataSize;
	BYTE								aData[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_DOJO_SEND_ATT_GUILD_ID)
TBLIDX								dojoTblidx;
GUILDID								defGuildId;
GUILDID								attGuildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOJO_SCRAMBLE_STATE_CHANGE )
	TBLIDX								dojoTblidx;
	BYTE								byState;	// 도장쟁탈전 상태
	DOJOTIME							tmNextStepTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOJO_NPC_INFO_RES )
	CHARACTERID							charId;
	WORD								wResultCode;
	sDBO_DOJO_NPC_INFO					sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOJO_SEND_TELEPORT_PROPOSAL_NFY )
	BYTE								byTeleportType; 
	WORD								wWaitTime;
	WORLDID								destWorldId;
	TBLIDX								destWorldTblidx;
	CNtlVector							vDestLoc;
	SERVERCHANNELID						destServerChannelId;
	SERVERINDEX							destServerIndex;
	BYTE								byCount;
	CHARACTERID							aMemberId[DBO_MAX_MEMBER_IN_GUILD];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOJO_COMMAND )
	BYTE								byCommand;
	TBLIDX								dojoTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TG_DOJO_USERCOUNT_CHANGE )
	TBLIDX								dojoTblidx;
	BYTE								byDefCount;
	BYTE								byAttCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_RANKBATTLE_REMAIN_COUNT_FLUSH_NFY)
sDBO_TIME							time;
BYTE								byInitCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_DAILY_LIMIT_COUNT_RESET_NFY)
DBOTIME								nProcTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_MAIL_RECV_SERVER_NFY)
CHARACTERID								charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_TENKAICHIDAISIJYOU_SELL_RES)
CHARACTERID					charId;
WORD						wResultCode;
BYTE						byPlace;
BYTE						byPosition;
BYTE						byCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES)
CHARACTERID					charId;
WORD						wResultCode;
ITEMID						nItem;
MAILID						mailID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_TENKAICHIDAISIJYOU_PRICE_INFO_RES)
CHARACTERID					charId;
ITEMID						nItem;
DWORD						dwPrice;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_TENKAICHIDAISIJYOU_BUY_RES)
CHARACTERID					charId;
WORD						wResultCode;
ITEMID						nItem;
DWORD						dwMoney;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_TENKAICHIDAISIJYOU_LIST_DATA)
CHARACTERID						charId;
sTENKAICHIDAISIJYOU_DATA_CLIENT	sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_TENKAICHIDAISIJYOU_LIST_RES)
CHARACTERID					charId;
WORD						wResultCode;
unsigned int				uiPage;
unsigned int				uiMaxPage;
BYTE						byListNum;
BYTE						byPage;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_TENKAICHIDAISIJYOU_PERIODEND_RES)
CHARACTERID					charId;
MAILID						mailId;
ITEMID						nItem;
ITEMID						itemID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_TENKAICHIDAISIJYOU_PERIODEND_TEXT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_WAGUCOIN_DECREASE_NFY)
CHARACTERID			charId;
WORD				wWaguWaguCoin;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TG_WAGUWAGUMACHINE_UPDATE_CASHITEM_INFO)
	CHARACTERID		charId;
	BYTE			bySetCount;
	sCASHITEM_BRIEF	sInfo;
END_PROTOCOL()
//------------------------------------------------------------------


#pragma pack()
