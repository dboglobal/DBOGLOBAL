#pragma once

#include "NtlPacketCommon.h"
#include "NtlGuild.h"
#include "NtlFriend.h"
#include "NtlRankBattle.h"
#include "NtlTimeQuest.h"
#include "NtlBudokai.h"
#include "NtlPetition.h"
#include "NtlDojo.h"
#include "NtlHlsMachine.h"
#include "NtlChat.h"


enum eOPCODE_TU
{
	TU_OPCODE_BEGIN = 7000,

	TU_ENTER_CHAT_RES,
	TU_DISCONNECT,
	TU_DISCONNECTED_NFY,

	TU_HEARTBEAT,

	TU_SYSTEM_DISPLAY_TEXT,

	TU_CHAT_MESSAGE_SAY,
	TU_CHAT_MESSAGE_SHOUT,
	TU_CHAT_MESSAGE_WHISPER,
	TU_CHAT_MESSAGE_WHISPER_FAILED_NFY,
	TU_CHAT_MESSAGE_PARTY,
	TU_CHAT_MESSAGE_GUILD,
	TU_CHAT_MESSAGE_TRADE,
	TU_CHAT_MESSAGE_FIND_PARTY,

	TU_GUILD_CREATED_NFY,
	TU_GUILD_INFO,
	TU_GUILD_MEMBER_INFO,
	TU_GUILD_DISBAND_RES,
	TU_GUILD_BEING_DISBANDED_NFY,
	TU_GUILD_DISBAND_CANCEL_RES,
	TU_GUILD_DISBAND_CANCELED_NFY,
	TU_GUILD_DISBANDED_NFY,
	TU_GUILD_INVITED_NFY,
	TU_GUILD_RESPONSE_INVITATION_RES,
	TU_GUILD_MEMBER_JOINED_NFY,
	TU_GUILD_INVITATION_DECLINED_NFY,
	TU_GUILD_INVITATION_EXPIRED_NFY,
	TU_GUILD_LEAVE_RES,
	TU_GUILD_MEMBER_LEFT_NFY,
	TU_GUILD_KICK_OUT_RES,
	TU_GUILD_MEMBER_KICKED_OUT_NFY,
	TU_GUILD_APPOINT_SECOND_MASTER_RES,
	TU_GUILD_SECOND_MASTER_APPOINTED_NFY,
	TU_GUILD_DISMISS_SECOND_MASTER_RES,
	TU_GUILD_SECOND_MASTER_DISMISSED_NFY,
	TU_GUILD_CHANGE_GUILD_MASTER_RES,
	TU_GUILD_GUILD_MASTER_CHANGED_NFY,
	TU_GUILD_MEMBER_POSITION_CHANGED_NFY,
	TU_GUILD_MEMBER_CLASS_CHANGED_NFY,
	TU_GUILD_MEMBER_LEVEL_CHANGED_NFY,
	TU_GUILD_MEMBER_REPUTATION_CHANGED_NFY,
	TU_GUILD_MEMBER_ONLINE_NFY,
	TU_GUILD_MEMBER_OFFLINE_NFY,

	TU_FRIEND_ADD_RES,
	TU_FRIEND_DEL_RES,
	TU_FRIEND_MOVE_RES,
	TU_FRIEND_LIST_INFO,
	TU_FRIEND_BLACK_ADD_RES,
	TU_FRIEND_BLACK_DEL_RES,
	TU_FRIEND_INFO,
	TU_FRIEND_INFO_CHANGE,

	TU_RANKBATTLE_RANK_LIST_RES,
	TU_RANKBATTLE_RANK_FIND_CHARACTER_RES,
	TU_RANKBATTLE_RANK_COMPARE_DAY_RES,

	TU_GUILD_REPUTATION_CHANGE_NFY,
	TU_GUILD_FUNCTION_CHANGE_NFY,
	TU_GUILD_CHANGE_NOTICE_NFY,
	TU_GUILD_CHANGE_NOTICE_RES,
	TU_GUILD_GIVE_ZENNY_NFY,

	TU_TMQ_RECORD_LIST_RES,
	TU_TMQ_MEMBER_LIST_RES,

	TU_BUDOKAI_NOTICE_NFY,
	TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES,
	TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES,
	TU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES,
	TU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES,

	TU_PETITION_CHAT_START_REQ,
	TU_PETITION_CHAT_GM_SAY_REQ,
	TU_PETITION_CHAT_USER_SAY_RES,
	TU_PETITION_CHAT_GM_END_NFY,
	TU_PETITION_USER_INSERT_RES,
	TU_PETITION_CONTENT_MODIFY_RES,
	TU_PETITION_USER_CANCEL_RES,
	TU_PETITION_FINISH_NFY,

	TU_DOJO_CREATED_NFY,
	TU_DOJO_DESTROYED_NFY,
	TU_DOJO_BRIEF_NFY,
	TU_DOJO_SCRAMBLE_NFY,
	TU_DOJO_SCRAMBLE_INFORM_NFY,
	TU_DOJO_SCRAMBLE_REJECT_NFY,
	TU_DOJO_BUDOKAI_SEED_ADD_RES,
	TU_DOJO_BUDOKAI_SEED_DEL_RES,
	TU_DOJO_SCRAMBLE_POINT_NFY,
	TU_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY,
	TU_DOJO_NOTICE_CHANGE_RES,
	TU_DOJO_NOTICE_CHANGE_NFY,
	TU_DOJO_SCRAMBLE_RECEIVE_NFY,
	TU_DOJO_SCRAMBLE_RECEIVE_INFO_NFY,
	TU_DOJO_SCRAMBLE_REWARD_NFY,
	TU_DOJO_SCRAMBLE_RESULT_NFY,

	TU_CHAR_NAME_CHANGED_NFY,

	TU_HLS_SLOT_MACHINE_INFO_RES,
	TU_HLS_SLOT_MACHINE_EXTRACT_RES,
	TU_HLS_SLOT_MACHINE_WINNER_INFO_RES,

	TU_BROADCASTING_SYSTEM_NFY,

	TU_WAGUWAGUCOIN_UPDATE_INFO,
	TU_EVENTCOIN_UPDATE_INFO,

	TU_OPCODE_END_DUMMY,
	TU_OPCODE_END = TU_OPCODE_END_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_TU(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_ENTER_CHAT_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_DISCONNECT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_DISCONNECTED_NFY)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_SYSTEM_DISPLAY_TEXT)
	WCHAR				wszGmCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE				byDisplayType;
	WORD				wMessageLengthInUnicode;
	WCHAR				wszMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_SAY)
	HOBJECT				hSubject;
	WCHAR				awchSenderCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_SHOUT)
	HOBJECT				hSubject;
	WCHAR				awchSenderCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_WHISPER)
	bool				bIsMessageFromYou;

	union UNIONWHISPERMSG
	{
		WCHAR wszMessageSenderCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
		WCHAR wszMessageReceiverCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	};
	UNIONWHISPERMSG		uCharName;

	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_WHISPER_FAILED_NFY)
	WCHAR				awchReceiverCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_PARTY)
	HOBJECT				hSubject;
	WCHAR				awchSenderCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE				byChattingType;		// eDBO_CHATTING_TYPE
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_GUILD)
	WCHAR				wszSenderCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE				byChattingType;		// eDBO_CHATTING_TYPE
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_TRADE)
	HOBJECT				hSubject;
	SERVERCHANNELID		serverChannelId;
	WCHAR				awchSenderCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_FIND_PARTY)
HOBJECT				hSubject;
SERVERCHANNELID		serverChannelId;
WCHAR				awchSenderCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
WORD				wMessageLengthInUnicode;
WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_CREATED_NFY)
WCHAR				wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_INFO)
sDBO_GUILD_INFO				guildInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_INFO)
sDBO_GUILD_MEMBER_INFO		guildMemberInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_DISBAND_RES)
WORD						wResultCode;
time_t						timeToDisband;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_BEING_DISBANDED_NFY)
	time_t						timeToDisband;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_DISBAND_CANCEL_RES)
	WORD						wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_DISBAND_CANCELED_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_DISBANDED_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_INVITED_NFY)
	WCHAR						wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_RESPONSE_INVITATION_RES)
	WORD						wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_JOINED_NFY)
	sDBO_GUILD_MEMBER_INFO		memberInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_INVITATION_DECLINED_NFY)
	WCHAR						wszTargetName[NTL_MAX_SIZE_CHAR_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_INVITATION_EXPIRED_NFY)
	WCHAR						wszTargetName[NTL_MAX_SIZE_CHAR_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_LEAVE_RES)
	WORD						wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_LEFT_NFY)
	CHARACTERID					memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_KICK_OUT_RES)
	WORD						wResultCode;
	CHARACTERID					kickedOutMemberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_KICKED_OUT_NFY)
CHARACTERID					memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_APPOINT_SECOND_MASTER_RES)
	WORD						wResultCode;
	CHARACTERID					secondMasterCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_SECOND_MASTER_APPOINTED_NFY)
	CHARACTERID					memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_DISMISS_SECOND_MASTER_RES)
	WORD						wResultCode;
	CHARACTERID					secondMasterCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_SECOND_MASTER_DISMISSED_NFY)
	CHARACTERID					memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_CHANGE_GUILD_MASTER_RES)
	WORD						wResultCode;
	CHARACTERID					newMasterCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_GUILD_MASTER_CHANGED_NFY)
	CHARACTERID					memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_POSITION_CHANGED_NFY)
	CHARACTERID					memberCharId;
	TBLIDX						newMapNameTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_CLASS_CHANGED_NFY)
	CHARACTERID					memberCharId;
	BYTE						byNewClass;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_LEVEL_CHANGED_NFY)
	CHARACTERID					memberCharId;
	BYTE						byNewLevel;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_REPUTATION_CHANGED_NFY)
	CHARACTERID					memberCharId;
	DWORD						dwNewReputation;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_ONLINE_NFY)
	CHARACTERID					memberCharId;
	TBLIDX						newMapNameTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_OFFLINE_NFY)
	CHARACTERID					memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_ADD_RES)
	WORD						wResultCode;
	WCHAR						wchName[NTL_MAX_SIZE_CHAR_NAME + 1];
	CHARACTERID					targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_DEL_RES)
	WORD						wResultCode;
	CHARACTERID					targetID;
	WCHAR						wchName[NTL_MAX_SIZE_CHAR_NAME + 1];//new
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_MOVE_RES)
	WORD						wResultCode;
	CHARACTERID					targetID;
	WCHAR						wchName[NTL_MAX_SIZE_CHAR_NAME + 1];//new
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_LIST_INFO)
	BYTE						byCount;
	sFRIEND_FULL_INFO			asInfo[NTL_MAX_COUNT_FRIEND_PACKET];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_BLACK_ADD_RES)
	WORD						wResultCode;
	WCHAR						wchName[NTL_MAX_SIZE_CHAR_NAME + 1];
	CHARACTERID					targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_BLACK_DEL_RES)
	WORD						wResultCode;
	CHARACTERID					targetID;
	WCHAR						wchName[NTL_MAX_SIZE_CHAR_NAME + 1];//new
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_INFO)
	sFRIEND_INFO				sInfo;
	bool						bIsFirst;		// true : dont show "x has logged in". false = show "x has logged in"
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_INFO_CHANGE)
	CHARACTERID					targetID;
	BYTE						byChangeType;	// NtlFriend.h eFRIEND_CHANGE_TYPE
	DWORD						dwChangeValue;	// 1:Off, 2:On , Class , Level , Channel, PositionMapTblidx
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_RANKBATTLE_RANK_LIST_RES)
	WORD						wResultCode;
	DWORD						dwPage;
	BYTE						byCompareDay;
	BYTE						byRankInfoCount;
	WORD						wRankInfo;		// sRANKBATTLE_RANK_INFO []
	sVARIABLE_DATA				sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_RANKBATTLE_RANK_FIND_CHARACTER_RES)
	WORD						wResultCode;
	DWORD						dwPage;
	BYTE						byCompareDay;
	WCHAR						wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE						byRankInfoCount;
	WORD						wRankInfo;		// sRANKBATTLE_RANK_INFO asRankInfo []
	sVARIABLE_DATA				sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_RANKBATTLE_RANK_COMPARE_DAY_RES)
	WORD						wResultCode;
	DWORD						dwPage;
	BYTE						byCompareDay;
	bool						bIsFullList;	// true = wRankInfo 참조 | false = wCompareInfo 참조
	BYTE						byInfoCount;
	union
	{
		WORD					wRankInfo;		// sRANKBATTLE_RANK_INFO asRankInfo []
		WORD					wCompareInfo;	// DWORD adwCompareRank []
	};

	sVARIABLE_DATA				sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_REPUTATION_CHANGE_NFY)
	DWORD						dwReputation;
	DWORD						dwMaxGuildPointEver;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_FUNCTION_CHANGE_NFY)
	DWORD64						qwGuildFunctionFlag;
	BYTE						byNewFunc;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_CHANGE_NOTICE_NFY)
	WCHAR						awchCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WORD						wNoticeLengthInUnicode;	
	WCHAR						awchNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_CHANGE_NOTICE_RES)
	WORD						wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_GIVE_ZENNY_NFY)
	CHARACTERID					charId; 
	DWORD						dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_TMQ_RECORD_LIST_RES )
	WORD						wResultCode;
	TBLIDX						tmqTblidx;
	BYTE						byDifficult;
	BYTE						byTeamCount;
	sTIMEQUEST_TEAM_RANK_DATA	asTeam[DBO_TIMEQUEST_DEFAULT_TOTAL_RANK_COUNT];		// 용량 주의
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_TMQ_MEMBER_LIST_RES )
	WORD						wResultCode;
	TBLIDX						tmqTblidx;
	BYTE						byDifficult;
	BYTE						byRank;
	sTIMEQUEST_TEAM_MEMBER_DATA	sTeam;		
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_BUDOKAI_NOTICE_NFY)
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
BEGIN_PROTOCOL( TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES )
	WORD									wResultCode;
	WORD									wEntryTeam;			// sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR	asEntryTeam[BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER];
	BYTE									byMatchDataCount;
	WORD									wMatchData;			// sBUDOKAI_TOURNAMENT_MATCH_DATA aMatchData[BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER * 2 - 1];
	sVARIABLE_DATA							sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES )
	WORD										wResultCode;
	TEAMTYPE									awTeamType[2];		// sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA_VAR asEntryData[2];
	sVARIABLE_DATA								sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES )
	WORD									wResultCode;
	WORD									wEntryTeam;			// sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR	asEntryTeam[BUDOKAI_MAJOR_MATCH_TEAM_NUMBER];
	BYTE									byMatchDataCount;
	WORD									wMatchData;			// sBUDOKAI_TOURNAMENT_MATCH_DATA aMatchData[BUDOKAI_MAJOR_MATCH_TEAM_NUMBER * 2 - 1];
	sVARIABLE_DATA							sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES )
	WORD								wResultCode;
	TEAMTYPE							awTeamType[2];			//	sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA_VAR	asEntryData[2];
	sVARIABLE_DATA						sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_CHAT_START_REQ )
	PETITIONID			petitionID;
	bool				bNeedSatisfactionRate;
	ACCOUNTID			gmAccountID;
	WCHAR				awchGMCharName_Consult[NTL_MAX_SIZE_CHAR_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_CHAT_GM_SAY_REQ )
	ACCOUNTID			gmAccountID;
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_CHAT_USER_SAY_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_CHAT_GM_END_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_USER_INSERT_RES )
	WORD				wResultCode;
	PETITIONID			petitionID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_CONTENT_MODIFY_RES )
	WORD				wResultCode;
END_PROTOCOL()
////----------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_USER_CANCEL_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_PETITION_FINISH_NFY )
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_CREATED_NFY )
	sDBO_DOJO_DATA		sDojoData;	
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_DESTROYED_NFY )
	TBLIDX				dojoTblidx;
	WCHAR				wszName[NTL_MAX_SIZE_GUILD_NAME + 1]; // new
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_BRIEF_NFY )
	sDBO_DOJO_BRIEF		sDojoData;	
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_NFY )			// The guild leader has been out or informed when a response is needed.
	WCHAR				wszName[NTL_MAX_SIZE_GUILD_NAME + 1]; // Application Guild Name
END_PROTOCOL()
//------------------------------------------------------------------	
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_INFORM_NFY )
	TBLIDX						dojoTblidx;
	BYTE						byState;	// Contest stalemate
	DOJOTIME					tmNextStepTime;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_REJECT_NFY )   // When the war you applied for refused
END_PROTOCOL()
//------------------------------------------------------------------	
BEGIN_PROTOCOL(TU_DOJO_BUDOKAI_SEED_ADD_RES)
	WORD				wResultCode;
	TBLIDX				dojoTblidx;
	WCHAR				wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];// 등록할 character
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_DOJO_BUDOKAI_SEED_DEL_RES)	
	WORD				wResultCode;
	WCHAR				wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];// 삭제할 character
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_POINT_NFY )
	sDOJO_SCRAMBLE_POINT sData;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY )
	TBLIDX								dojoTblidx;
	TBLIDX								objectTblidx;
	GUILDID								guildId;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_NOTICE_CHANGE_RES )
	WORD						wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_DOJO_NOTICE_CHANGE_NFY )
	TBLIDX						dojoTblidx;
	WCHAR						awchCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WORD						wNoticeLengthInUnicode;	
	WCHAR						awchNotice[NTL_MAX_LENGTH_OF_DOJO_NOTICE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_RECEIVE_NFY )
	TBLIDX								dojoTblidx;
	GUILDID								guildId;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_RECEIVE_INFO_NFY )
	TBLIDX								dojoTblidx;
	GUILDID								defGuildId;
	GUILDID								attGuildId;
	WCHAR								wszDefGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
	WCHAR								wszAttGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
	sDBO_GUILD_MARK						sDefMark;
	sDBO_GUILD_MARK						sAttMark;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_REWARD_NFY )
	DWORD						dwAddGuildReputation;
	TBLIDX						itemTblidx;
	BYTE						byCount;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_RESULT_NFY )
	GUILDID						winGuildId;	
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_CHAR_NAME_CHANGED_NFY )
	CHARACTERID					targetID;
	WCHAR						wszOldCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WCHAR						wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_BROADCASTING_SYSTEM_NFY )
	BYTE					byMsgType;
	sMSG_BROADCAST_DATA		sData;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_WAGUWAGUCOIN_UPDATE_INFO )
	WORD					wWaguWaguCoin;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_EVENTCOIN_UPDATE_INFO )
	WORD					wEventCoin;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_HLS_SLOT_MACHINE_INFO_RES )
	WORD			wResultCode;
	BYTE			byType;
	BYTE			byMachineCount;
	WORD			wMachineIndex[DBO_MAX_HLS_SLOT_MACHINES_PER_PAGE];
	BYTE			byCoin[DBO_MAX_HLS_SLOT_MACHINES_PER_PAGE];
	int				bOnOff[DBO_MAX_HLS_SLOT_MACHINES_PER_PAGE];
	TBLIDX			ItemTblidx[DBO_MAX_HLS_SLOT_MACHINES_PER_PAGE][DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
	WORD			wWaitingTime[DBO_MAX_HLS_SLOT_MACHINES_PER_PAGE];
	WORD			wCurrentCapsule[DBO_MAX_HLS_SLOT_MACHINES_PER_PAGE];
	WORD			wMaxCapsule[DBO_MAX_HLS_SLOT_MACHINES_PER_PAGE];
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_HLS_SLOT_MACHINE_EXTRACT_RES )
	WORD			wResultCode;
	TBLIDX			wMachineIndex;
	WORD			wNewWaguWaguPoints;
	BYTE			byRanking[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
	TBLIDX			ItemTblidx[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
	BYTE			byStackCount[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
	BYTE			bySetCount[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
	BYTE			byReallyExtractCount;	//how many times did we use the slot machine?
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_HLS_SLOT_MACHINE_WINNER_INFO_RES )
	WORD			wResultCode;
	WORD			wMachineIndex;
	BYTE			byInfoCount;
	WCHAR			wszPlayer[DBO_MAX_HLS_SLOT_MACHINES_MAX_WINNERS][NTL_MAX_SIZE_CHAR_NAME + 1];
	WORD			wWinCount[DBO_MAX_HLS_SLOT_MACHINES_MAX_WINNERS];
	DBOTIME			nExtractTime[DBO_MAX_HLS_SLOT_MACHINES_MAX_WINNERS];
	QWORD			nWinnerIndex[DBO_MAX_HLS_SLOT_MACHINES_MAX_WINNERS];
END_PROTOCOL()
//------------------------------------------------------------------ 

#pragma pack()
