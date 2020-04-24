#pragma once

#include "NtlPacketCommon.h"
#include "NtlParty.h"
#include "NtlGuild.h"
#include "NtlFriend.h"
#include "NtlRankBattle.h"
#include "NtlTimeQuest.h"
#include "NtlDojo.h"
#include "NtlHlsMachine.h"

enum eOPCODE_QT
{
	QT_OPCODE_BEGIN = 56000,

	QT_GUILD_CREATE_RES = QT_OPCODE_BEGIN,
	QT_GUILD_DATA,
	QT_GUILD_MEMBER_DATA,
	QT_GUILD_MEMBER_LEFT_NFY,
	QT_GUILD_DISBAND_RES,
	QT_GUILD_DISBAND_CANCEL_RES,

	QT_GUILD_INVITE_RES,
	QT_GUILD_LEAVE_RES,
	QT_GUILD_KICK_OUT_RES,

	QT_GUILD_APPOINT_SECOND_MASTER_RES,
	QT_GUILD_DISMISS_SECOND_MASTER_RES,
	QT_GUILD_CHANGE_GUILD_MASTER_RES,

	QT_FRIEND_ADD_RES,		// 친구 추가
	QT_FRIEND_DEL_RES,		// 친구 삭제
	QT_FRIEND_MOVE_RES,		// 친구 -> 블랙리스트로 이동
	QT_FRIEND_LIST_LOAD_RES, // 친구 리스트 로드
	QT_FRIEND_BLACK_ADD_RES,		// 친구 블랙 리스트 추가
	QT_FRIEND_BLACK_DEL_RES,		// 친구 블랙 리스트 삭제

	QT_RANKBATTLE_RANK_LIST_RES,
	QT_RANKBATTLE_RANK_FIND_CHARACTER_RES,
	QT_RANKBATTLE_RANK_DATA_UPDATE_NFY,		// RankBattle의 순위 정보가 바뀌었음을 알림

	QT_GUILD_FUNCTION_ADD_RES,
	QT_GUILD_GIVE_ZENNY_RES,
	QT_GUILD_CHANGE_NOTICE_RES,
	QT_GUILD_CREATE_MARK_RES,
	QT_GUILD_CHANGE_MARK_RES,
	QT_GUILD_CHANGE_NAME_RES,

	QT_TMQ_RECORD_LIST_RES,
	QT_TMQ_MEMBER_LIST_RES,
	
	QT_DOJO_DATA_RES,
	QT_DOJO_CREATE_RES,
	QT_DOJO_UPDATE_RES,
	QT_DOJO_DEL_RES,
	QT_DOJO_FUNCTION_ADD_RES,
	QT_DOGI_CREATE_RES,
	QT_DOGI_CHANGE_RES,

	QT_CONNECT_WAIT_CHECK_REQ,
	QT_CONNECT_WAIT_CANCEL_REQ,

	QT_DOJO_SCRAMBLE_RES,
	QT_DOJO_SCRAMBLE_REJECT_RES,
	QT_CONNECT_WAIT_CHARACTER_SERVER_DOWN_NFY,
	QT_DOJO_BUDOKAI_SEED_ADD_RES,
	QT_DOJO_BUDOKAI_SEED_DEL_RES,
	QT_DOJO_SCRAMBLE_REWARD_RES,
	QT_DOJO_CHANGE_NOTICE_RES,

	QT_TENKAICHIDAISIJYOU_SELL_RES,
	QT_TENKAICHIDAISIJYOU_SELL_CANCEL_RES,
	QT_TENKAICHIDAISIJYOU_BUY_RES,
	QT_TENKAICHIDAISIJYOU_SERVERDATA_RES,
	QT_TENKAICHIDAISIJYOU_PERIODEND_RES,

	QT_HLS_SLOT_MACHINE_EXTRACT_RES,

	QT_OPCODE_END_DUMMY,
	QT_OPCODE_END = QT_OPCODE_END_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_QT(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_CREATE_RES)
	BYTE						byServerChannelIndex;
	WORD						wResultCode;
	sDBO_GUILD_DATA				guildData;
	DWORD						dwSpentZenny;
	ACCOUNTID					creatorAccountId;
	PARTYID						partyId;
	BYTE						byMemberCount;
	sDBO_GUILD_MEMBER_DATA		aMemberData[NTL_MAX_MEMBER_IN_PARTY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_DATA)
sDBO_GUILD_DATA				guildData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_MEMBER_DATA)
GUILDID						guildId;
sDBO_GUILD_MEMBER_DATA		guildMemberData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_MEMBER_LEFT_NFY)
	CHARACTERID					memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_DISBAND_RES)
	WORD						wResultCode;
	GUILDID						guildId;
	time_t						timeToDisband;
	bool						bByUser;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_DISBAND_CANCEL_RES)
	WORD						wResultCode;
	GUILDID						guildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_INVITE_RES)
	WORD						wResultCode;
	CHARACTERID					inviteeCharId;
	GUILDID						guildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_LEAVE_RES)
	WORD						wResultCode;
	CHARACTERID					leavingMemberCharId;
	GUILDID						guildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_KICK_OUT_RES)
	WORD						wResultCode;
	CHARACTERID					kickedOutMemberCharId;
	GUILDID						guildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_APPOINT_SECOND_MASTER_RES)
	WORD						wResultCode;
	GUILDID						guildId;
	CHARACTERID					targetMemberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_DISMISS_SECOND_MASTER_RES)
	WORD						wResultCode;
	GUILDID						guildId;
	CHARACTERID					targetMemberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_CHANGE_GUILD_MASTER_RES)
	WORD						wResultCode;
	GUILDID						guildId;
	CHARACTERID					newMasterCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_FRIEND_ADD_RES)
	WORD						wResultCode;
	WCHAR						wchName[NTL_MAX_SIZE_CHAR_NAME + 1];
	CHARACTERID					charID;
	ACCOUNTID					accountID;
	CHARACTERID					targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_FRIEND_DEL_RES)
	WORD						wResultCode;
	WCHAR						wchFriendName[NTL_MAX_SIZE_CHAR_NAME + 1];
	CHARACTERID					charID;
	ACCOUNTID					accountID;
	CHARACTERID					targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_FRIEND_MOVE_RES)
	WORD						wResultCode;
	CHARACTERID					charID;
	ACCOUNTID					accountID;
	CHARACTERID					targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_FRIEND_LIST_LOAD_RES)
	ACCOUNTID					accountID;
	BYTE						byCount;
	sFRIEND_FULL_DATA			asInfo[NTL_MAX_COUNT_FRIEND_PACKET];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_FRIEND_BLACK_ADD_RES)
	WORD						wResultCode;
	CHARACTERID					charID;
	ACCOUNTID					accountID;
	WCHAR						wchName[NTL_MAX_SIZE_CHAR_NAME + 1];
	CHARACTERID					targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_FRIEND_BLACK_DEL_RES)
	WORD						wResultCode;
	CHARACTERID					charID;
	CHARACTERID					targetID;
	ACCOUNTID					accountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_RANKBATTLE_RANK_LIST_RES)
	WORD						wResultCode;

	CHARACTERID					charId;					// -- 커뮤니티 서버에서 사용
	BYTE						byCompareDay;			// 비교일 -- 커뮤니티 서버에서 사용
	bool						bIsCompareRankReq;		// CompareRank 변경을 위한 요청인가? -- 커뮤니티 서버에서 사용

	DWORD						dwPage;

	BYTE						byCount;
	sRANKBATTLE_RANK_DATA		asRankList[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_RANKBATTLE_RANK_FIND_CHARACTER_RES)
	WORD						wResultCode;

	CHARACTERID					charId;					// 요청한 character -- 커뮤니티 서버에서 사용
	BYTE						byCompareDay;			// 비교일 -- 커뮤니티 서버에서 사용

	WCHAR						wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];// 찾은 character

	DWORD						dwPage;

	BYTE						byCount;
	sRANKBATTLE_RANK_DATA		asRankList[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_RANKBATTLE_RANK_DATA_UPDATE_NFY)
	DWORD						dwBeforeRank;			// 변경 전 Rank
	DWORD						dwAfterRank;			// 변경 후 Rank
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_FUNCTION_ADD_RES)
	BYTE						byServerChannelIndex;
	BYTE						byServerIndex;
	WORD						wResultCode;
	GUILDID						guildId;
	CHARACTERID					masterCharId;
	DWORD						dwZenny;
	DWORD64						qwGuildFunctionFlag;
	BYTE						byFunction;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_GIVE_ZENNY_RES)
	BYTE						byServerChannelIndex;
	BYTE						byServerIndex;
	WORD						wResultCode;
	GUILDID						guildId;
	CHARACTERID					charId;
	DWORD						dwZenny;
	DWORD						dwAddReputaion;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_GUILD_CHANGE_NOTICE_RES)
WORD						wResultCode;
GUILDID						guildId;
CHARACTERID					charId;
WCHAR						awchCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
WORD						wNoticeLengthInUnicode;	
WCHAR						awchNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QT_GUILD_CREATE_MARK_RES )
	BYTE						byServerChannelIndex;
	BYTE						byServerIndex;
	WORD						wResultCode;
	GUILDID						guildId;
	CHARACTERID					charId;
	sDBO_GUILD_MARK				sMark;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QT_GUILD_CHANGE_MARK_RES )
	BYTE						byServerChannelIndex;
	BYTE						byServerIndex;
	WORD						wResultCode;
	GUILDID						guildId;
	CHARACTERID					charId;
	DWORD						dwZenny;
	sDBO_GUILD_MARK				sMark;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QT_GUILD_CHANGE_NAME_RES )
BYTE						byServerChannelIndex;
BYTE						byServerIndex;
WORD						wResultCode;
GUILDID						guildId;
CHARACTERID					charId;
WCHAR						wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
BYTE						byPlace;
BYTE						byPos;
ITEMID						itemId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QT_TMQ_RECORD_LIST_RES )
	WORD						wResultCode;
	CHARACTERID					charId;
	TBLIDX						tmqTblidx;
	BYTE						byDifficult;
	BYTE						byTeamCount;
	sTIMEQUEST_TEAM_RANK_DATA	asTeam[DBO_TIMEQUEST_DEFAULT_TOTAL_RANK_COUNT];		// 용량 주의
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QT_TMQ_MEMBER_LIST_RES )
	WORD						wResultCode;
	CHARACTERID					charId;	
	TBLIDX						tmqTblidx;
	BYTE						byDifficult;
	BYTE						byRank;
	sTIMEQUEST_TEAM_MEMBER_DATA	sTeam;		
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_DOJO_DATA_RES)
BYTE						byCount;
sDBO_DOJO_DATA				asDojoData[DBO_MAX_COUNT_DOJO_IN_WORLD];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_DOJO_CREATE_RES)
WORD						wResultCode;
BYTE						byServerChannelId;
GUILDID						guildId;
CHARACTERID					charId;
TBLIDX						dojoTblidx;
BYTE						byLevel;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_DOJO_UPDATE_RES)
	WORD						wResultCode;
	BYTE						byServerChannelId;
	sDBO_DOJO_DATA				sDojoData;
	GUILDID						guildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_DOJO_DEL_RES)
	WORD						wResultCode;
	BYTE						byServerChannelId;
	GUILDID						guildId;
	CHARACTERID					charId;
	TBLIDX						dojoTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_DOJO_FUNCTION_ADD_RES)
	BYTE						byServerChannelIndex;
	BYTE						byServerIndex;
	WORD						wResultCode;
	GUILDID						guildId;
	CHARACTERID					masterCharId;
	DWORD						dwZenny;
	DWORD64						qwGuildFunctionFlag;
	BYTE						byFunction;
	BYTE						byPlace;
	BYTE						byPos;
	BYTE						byStack;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QT_DOGI_CREATE_RES )
	BYTE						byServerChannelIndex;
	BYTE						byServerIndex;
	WORD						wResultCode;
	CHARACTERID					charId;
	sDBO_DOGI_DATA				sDogi;
	bool						bIsDojo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QT_DOGI_CHANGE_RES )
	BYTE						byServerChannelIndex;
	BYTE						byServerIndex;
	WORD						wResultCode;
	CHARACTERID					charId;
	sDBO_DOGI_DATA				sDogi;
	DWORD						dwZenny;
	bool						bIsDojo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QT_CONNECT_WAIT_CHECK_REQ )
	ACCOUNTID		accountId;
	SERVERCHANNELID gameServerChannelID;
	SERVERINDEX		gameServerIndex;
	SERVERCHANNELID characterServerChannelID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QT_CONNECT_WAIT_CANCEL_REQ )
	ACCOUNTID		accountId;
	SERVERCHANNELID gameServerChannelID;
	SERVERINDEX		gameServerIndex;
	SERVERCHANNELID characterServerChannelID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_DOJO_SCRAMBLE_RES)
	WORD						wResultCode;
	BYTE						byServerChannelIndex;
	BYTE						byServerIndex;
	GUILDID						guildId;
	CHARACTERID					charId;
	TBLIDX						dojoTblidx;
	DWORD						dwReqZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_DOJO_SCRAMBLE_REJECT_RES)
	WORD						wResultCode;
	BYTE						byServerChannelIndex;
	BYTE						byServerIndex;
	GUILDID						guildId;
	TBLIDX						dojoTblidx;
	DWORD						dwReqZenny;
	CHARACTERID					leader;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QT_CONNECT_WAIT_CHARACTER_SERVER_DOWN_NFY )
	SERVERINDEX characterServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_DOJO_BUDOKAI_SEED_ADD_RES)
	WORD						wResultCode;
	CHARACTERID					charId;
	TBLIDX						dojoTblidx;
	WCHAR						wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];// 등록할 character
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_DOJO_BUDOKAI_SEED_DEL_RES)	
	WORD						wResultCode;
	CHARACTERID					charId;
	TBLIDX						dojoTblidx;
	WCHAR						wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];// 삭제할 character
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QT_DOJO_SCRAMBLE_REWARD_RES )			
	WORD						wResultCode;
	TBLIDX						dojoTblidx;
	GUILDID						guildId;// winguild
	DWORD						dwAddGuildReputation;
	DWORD64						qwGuildFunctionFlag;
	BYTE						byLevel;
	TBLIDX						itemTblidx;
	BYTE						byCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_DOJO_CHANGE_NOTICE_RES)
WORD						wResultCode;
TBLIDX						dojoTblidx;
CHARACTERID					charId;
WCHAR						awchCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
WORD						wNoticeLengthInUnicode;	
WCHAR						awchNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE + 1];
GUILDID				guildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_TENKAICHIDAISIJYOU_SELL_RES)
SERVERCHANNELID					byServerChannelIndex;
SERVERINDEX						byServerIndex;
CHARACTERID						charId;
WORD							wResultCode;
DWORD							dwFee;
BYTE							byPlace;
BYTE							byPosition;
sTENKAICHIDAISIJYOU_DATA		sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_TENKAICHIDAISIJYOU_SELL_CANCEL_RES)
CHARACTERID						charId;
WORD							wResultCode;
ITEMID							nItem;
MAILID							mailId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_TENKAICHIDAISIJYOU_BUY_RES)
CHARACTERID						charId;
WORD							wResultCode;
ITEMID							nItem;
DWORD							dwMoney;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_TENKAICHIDAISIJYOU_SERVERDATA_RES)
BYTE							byCurPacketCount;
bool							bEndList;
BYTE							byListNum;
sTENKAICHIDAISIJYOU_DATA		sData[10];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_TENKAICHIDAISIJYOU_PERIODEND_RES)
WORD							wResultCode;
ITEMID							nItem;
MAILID							mailId;
bool							bList;
CHARACTERID						charId;
ITEMID							itemId;
DWORD							dwPrice;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QT_HLS_SLOT_MACHINE_EXTRACT_RES)
WORD			wResultCode;
CHARACTERID		charId;
ACCOUNTID		accountId;
BYTE			byHlsMachineType;
WORD			wMachineIndex;
WORD			wCoin;
BYTE			byRanking[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
TBLIDX			ItemTblidx[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
BYTE			byStackCount[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
BYTE			bySetCount[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
BYTE			byExtractCount;
ITEMID			aProductId[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
END_PROTOCOL()
//------------------------------------------------------------------



#pragma pack()
