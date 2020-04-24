#pragma once

#include "NtlPacketCommon.h"
#include "NtlItem.h"
#include "NtlSkill.h"

#include "NtlCharacter.h"

enum eOPCODE_CQ
{
	CQ_OPCODE_BEGIN = 32000,

	CQ_NOTIFY_SERVER_BEGIN = CQ_OPCODE_BEGIN,

	CQ_CHECK_AUTH_KEY_REQ,

	CQ_CHARACTER_INFO_REQ,
	CQ_CHARACTER_ADD_REQ,
	CQ_CHARACTER_DEL_REQ,

	CQ_CHARACTER_EXIT,
	CQ_CHARACTER_LOAD_REQ,
	CQ_CHAR_SERVER_RESET_REQ,

	
	CQ_CONNECT_WAIT_CHECK_REQ,
	CQ_CONNECT_WAIT_CANCEL_REQ,

	CQ_CHARACTER_RENAME_REQ,

	CQ_UPDATE_SERVERFARMID_REQ,

	CQ_CHARACTER_CAN_DEL_REQ,
	CQ_CHARACTER_DEL_CANCEL_REQ,

	CQ_OPCODE_DUMMY,
	CQ_OPCODE_END = CQ_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_CQ(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(CQ_NOTIFY_SERVER_BEGIN)
	BYTE				byServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CQ_CHECK_AUTH_KEY_REQ)
	ACCOUNTID		accountId;
	//char			achAuthKey[NTL_MAX_SIZE_AUTH_KEY];
	SERVERFARMID		serverFarmId;
	SERVERFARMID		lastServerFarmId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CQ_CHARACTER_INFO_REQ)
	ACCOUNTID		accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CQ_CHARACTER_ADD_REQ)
	ACCOUNTID		accountId;
//	WCHAR			awchUserID[NTL_MAX_SIZE_USERID + 1];
	WCHAR			awchCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	DWORD			dwCodePage;
	BYTE			byRace;
	BYTE			byClass;
	BYTE			byGender;
	BYTE			byFace;
	BYTE			byHair;
	BYTE			byHairColor;
	BYTE			bySkinColor;
	BYTE			byBlood;
	sVECTOR3		vSpawn_Loc;
	sVECTOR3		vSpawn_Dir;
	BYTE			byBindType;		// eDBO_BIND_TYPE
	WORLDID			bindWorldId;
	sVECTOR3		vBind_Loc;
	sVECTOR3		vBind_Dir;
	TBLIDX			mapNameTblidx;
	BYTE			byItemCount;
	sITEM_DATA		sItem[NTL_MAX_NEWBIE_ITEM];	
	BYTE			bySkillCount;
	TBLIDX			aSkill[NTL_MAX_NEWBIE_SKILL];	
//	BYTE			byQuickSlotCount;
//	sNEWBIE_QUICKSLOT_DATA asQuickSlotData[NTL_MAX_NEWBIE_QUICKSLOT_COUNT];
	PORTALID		defaultPortalId[3];
//	sQUESTITEM_DATA	sQItem;
	BYTE			bySuperiorType;
	BYTE			bySuperiorValue;
	SERVERFARMID	serverId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CQ_CHARACTER_DEL_REQ)
ACCOUNTID		accountID;
CHARACTERID		charID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CQ_CHARACTER_EXIT)
	ACCOUNTID		accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CQ_CHARACTER_LOAD_REQ)
	ACCOUNTID		accountId;
	SERVERFARMID	serverFarmId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CQ_CHAR_SERVER_RESET_REQ)
	ACCOUNTID		accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( CQ_CONNECT_WAIT_CHECK_REQ )
	ACCOUNTID		accountId;
	SERVERFARMID	ServerFarmId;
	BYTE			byChannelIdx;
	BYTE			byGameServerIndex;
	BYTE			byCharacterServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( CQ_CONNECT_WAIT_CANCEL_REQ )
	ACCOUNTID		accountId;
	SERVERFARMID	ServerFarmId;
	BYTE			byChannelIdx;
	BYTE			byGameServerIndex;
	BYTE			byCharacterServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( CQ_CHARACTER_RENAME_REQ )
	ACCOUNTID		accountId;
	CHARACTERID		charId;
	WCHAR			awchCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( CQ_UPDATE_SERVERFARMID_REQ )
	ACCOUNTID		accountId;
	SERVERFARMID	newServerfarmId;
END_PROTOCOL()
//------------------------------------------------------------------

BEGIN_PROTOCOL(CQ_CHARACTER_CAN_DEL_REQ)
ACCOUNTID		accountId;
CHARACTERID		charId;
SERVERFARMID	serverFarmID;
__int64			deleteDate;
bool			bIsNeedDelChar;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CQ_CHARACTER_DEL_CANCEL_REQ)
ACCOUNTID		accountId;
CHARACTERID		charId;
SERVERFARMID	serverFarmID;
WCHAR			sDelPass[50+1];
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()