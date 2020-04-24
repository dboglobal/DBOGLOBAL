#include "precomp_dboclient.h"
#include "DboPacketGenerator.h"

// shared
#include "NtlClientNet.h"
#include "NtlPacketUA.h"
#include "NtlPacketUC.h"
#include "NtlPacketUG.h"
#include "NtlPacketUT.h"
#include "NtlPacketTU.h"

// simulation.h"
#include "NtlSLGlobal.h"
#include "NtlNetSender.h"
#include "NtlPacketLockManager.h"
#include "NtlInventory.h"
#include "NtlSobAvatar.h"
#include "NtlDebug.h"
#include "NtlSobManager.h"
#include "NtlSLGlobal.h"
#include "NtlSobItem.h"
#include "NtlSLApi.h"
#include "NtlSobWorldItem.h"

#include "md5.h"

// framework
#include "NtlTimer.h"

// Dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboVirtualServer.h"
#include "IconMoveManager.h"
#include "GameStageState.h"



CLoginPacketGenerator::CLoginPacketGenerator(CNtlNetSender *pNetSender)
:CNtlPacketGenerator(pNetSender)
{
}

bool CLoginPacketGenerator::SendLonInReq(const WCHAR *pUserId, const WCHAR *pPassword, RwUInt16 wLVer, RwUInt16 wRVer, const BYTE* pMacAddress)
{
	sUA_LOGIN_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UA_LOGIN_REQ;
	memcpy(sPacket.awchUserId, pUserId, sizeof(WCHAR)*wcslen(pUserId));

	char* password = Ntl_WC2MB(pPassword);

	MD5 md;
	strcpy_s((char*)sPacket.achPasswd, NTL_MAX_SIZE_USERPW_ENCRYPT + 1, md.digestString(password)); //encrypt password

	// free memory
	Ntl_CleanUpHeapString(password);

	sPacket.wLVersion = wLVer;
	sPacket.wRVersion = wRVer;
	memcpy(sPacket.abyMacAddress, pMacAddress, sizeof(sPacket.abyMacAddress));

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CLoginPacketGenerator::SendLogOutReq(const WCHAR *pUserId, bool bIsEnteringCharacterServer)
{
	sUA_LOGIN_DISCONNECT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UA_LOGIN_DISCONNECT_REQ;
	memcpy(sPacket.awchUserId, pUserId, sizeof(WCHAR)*wcslen(pUserId)); 
	sPacket.bIsEnteringCharacterServer = bIsEnteringCharacterServer;
	
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


CLobbyPacketGenerator::CLobbyPacketGenerator(CNtlNetSender *pNetSender)
:CNtlPacketGenerator(pNetSender)
{	
}


bool CLobbyPacketGenerator::SendCharLogInReq(RwUInt32 uiAccountId, const RwChar *pAuthKey, RwUInt8 byServerFarmID)
{
	sUC_LOGIN_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UC_LOGIN_REQ;
	sPacket.accountId = uiAccountId;
	sPacket.serverID = byServerFarmID;
	memcpy((char*)sPacket.abyAuthKey, pAuthKey, strlen(pAuthKey)); 
	
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CLobbyPacketGenerator::SendCharServerListReq()
{
	sUC_CHARACTER_SERVERLIST_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UC_CHARACTER_SERVERLIST_REQ;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket); 
}

bool CLobbyPacketGenerator::SendCharServerListOneReq()
{
	sUC_CHARACTER_SERVERLIST_ONE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UC_CHARACTER_SERVERLIST_ONE_REQ;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket); 
}

bool CLobbyPacketGenerator::SendCharLoadReq(RwUInt32 uiAccountID, RwUInt8 byServerFarmID)
{
	sUC_CHARACTER_LOAD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UC_CHARACTER_LOAD_REQ;
	sPacket.accountId = uiAccountID;
	sPacket.serverFarmId = byServerFarmID;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket); 
}

bool CLobbyPacketGenerator::SendCharAddReq(const WCHAR *pUserName, RwUInt8 byRace, RwUInt8 byClassId, RwUInt8 byGender, 
											RwUInt8 byFace, RwUInt8 byHair, RwUInt8 byHairColor, RwUInt8 bySkinColor, 
											RwUInt8 byBlood)
{
	sUC_CHARACTER_ADD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UC_CHARACTER_ADD_REQ;
	memcpy(sPacket.awchCharName, pUserName, sizeof(WCHAR)*wcslen(pUserName)); 

	sPacket.byRace = byRace;
	sPacket.byClass = byClassId;
	sPacket.byGender = byGender;
	sPacket.byFace = byFace;
	sPacket.byHair = byHair;
	sPacket.byHairColor = byHairColor;
	sPacket.bySkinColor = bySkinColor;
	sPacket.byBlood = byBlood;
		
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CLobbyPacketGenerator::SendCharDelReq(RwUInt32 uiCharId, const WCHAR* pwcText)
{
	sUC_CHARACTER_DEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UC_CHARACTER_DEL_REQ;
	sPacket.charId = uiCharId;

	char* password = Ntl_WC2MB(pwcText);

	MD5 md;
	strcpy_s((char*)sPacket.achPasswd, NTL_MAX_SIZE_USERPW_ENCRYPT + 1, md.digestString(password)); //encrypt password

	// free memory
	Ntl_CleanUpHeapString(password);
	
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CLobbyPacketGenerator::SendCharDelCancelReq(RwUInt32 uiCharId, const WCHAR* pwcText)
{
	sUC_CHARACTER_DEL_CANCEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UC_CHARACTER_DEL_CANCEL_REQ;
	sPacket.charId			= uiCharId;
	
	char* password = Ntl_WC2MB(pwcText);

	MD5 md;
	strcpy_s((char*)sPacket.achPasswd, NTL_MAX_SIZE_USERPW_ENCRYPT + 1, md.digestString(password)); //encrypt password

	// free memory
	Ntl_CleanUpHeapString(password);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CLobbyPacketGenerator::SendCharRenameReq(RwUInt32 uiCharID, DWORD dwCodePage, WCHAR* pwcCharName)
{
	if( !pwcCharName )
		return true;

	RwInt32 iNameLength = wcslen(pwcCharName);
	if( iNameLength > NTL_MAX_SIZE_CHAR_NAME )
		return true;

	sUC_CHARACTER_RENAME_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode			= UC_CHARACTER_RENAME_REQ;
	sPacket.charId			= uiCharID;
	memcpy(sPacket.awchCharName, pwcCharName, sizeof(WCHAR)*iNameLength); 

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CLobbyPacketGenerator::SendCharConnectGameServer_WaitCheckReq(RwUInt8 byServerChannelIndex)
{
	sUC_CONNECT_WAIT_CHECK_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UC_CONNECT_WAIT_CHECK_REQ;
	sPacket.serverChannelId = byServerChannelIndex;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CLobbyPacketGenerator::SendCharConnectGameServer_WaitCancelReq(RwUInt8 byServerChannelIndex)
{
	sUC_CONNECT_WAIT_CANCEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UC_CONNECT_WAIT_CANCEL_REQ;
	sPacket.serverChannelId = byServerChannelIndex;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CLobbyPacketGenerator::SendCharSelectReq(RwUInt32 uiCharId, RwUInt8 byServerChannelIdx)
{
	sUC_CHARACTER_SELECT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UC_CHARACTER_SELECT_REQ;
	sPacket.charId = uiCharId;
	sPacket.serverChannelId = byServerChannelIdx;
	
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CLobbyPacketGenerator::SendCharDisconnectReq(BYTE byMoveType)
{
	sUC_CHARACTER_EXIT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode		= UC_CHARACTER_EXIT_REQ;
	sPacket.byMoveType = byMoveType;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGamePacketGenerator::CGamePacketGenerator(CNtlNetSender *pNetSender)
:CNtlPacketGenerator(pNetSender)
{
	
}

CGamePacketGenerator::~CGamePacketGenerator()
{
	
}


bool CGamePacketGenerator::SendGameEnterReq(RwUInt32 uiAccoundId, RwUInt32 uiCharId, const RwChar *pAuthKey, bool bTutorial)
{
	sUG_GAME_ENTER_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_GAME_ENTER_REQ;
	sPacket.accountId = uiAccoundId;
	sPacket.charId = uiCharId;
	sPacket.bTutorialMode = bTutorial;
	memcpy(sPacket.abyAuthKey, pAuthKey, NTL_MAX_SIZE_AUTH_KEY);
		
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CGamePacketGenerator::SendEnterWorld(void)
{
	sUG_ENTER_WORLD sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ENTER_WORLD;
			
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}


bool CGamePacketGenerator::SendAuthKeyCommunityServerReq(void)
{
	sUG_AUTH_KEY_FOR_COMMUNITY_SERVER_REQ sPacket;

	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_AUTH_KEY_FOR_COMMUNITY_SERVER_REQ;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CGamePacketGenerator::SendCharReadyForCommunityServerNfy(void)
{
	sUG_CHAR_READY_FOR_COMMUNITY_SERVER_NFY sPacket;

	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CHAR_READY_FOR_COMMUNITY_SERVER_NFY;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CGamePacketGenerator::SendGameLeaveReq(void)
{
	sUG_GAME_LEAVE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_GAME_LEAVE_REQ;
			
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CGamePacketGenerator::SendCharServerExitReq(void)
{
	// TO DO BY DANEOS 
	// THIS CRASHES THE GAME. I think it is because of "DeleteNetConnectBox".
	// Need to change the way of exit game anyway so server can decide if he can exit or not.
	//GetGameStageState()->ChangeState(GAME_STATE_CHAR_EXIT);

	sUG_CHAR_EXIT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CHAR_EXIT_REQ;
			
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CGamePacketGenerator::SendServerChangeReq(void)
{
	sUG_CHAR_SERVER_CHANGE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CHAR_SERVER_CHANGE_REQ;
			
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CGamePacketGenerator::SendChannelChangeReq(RwUInt8 byChannelID)
{
	sUG_CHAR_CHANNEL_CHANGE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CHAR_CHANNEL_CHANGE_REQ;
	sPacket.destServerChannelId = byChannelID;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CGamePacketGenerator::SendItemMoveReq(RwUInt8 bySrcPlace, RwUInt8 bySrcSlotPos, RwUInt8 byDestPlace, RwUInt8 byDestSlostPos)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_ITEM_MOVE_RES ) )
		return true;

	sUG_ITEM_MOVE_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UG_ITEM_MOVE_REQ;
	sPacket.bySrcPlace	= bySrcPlace;
	sPacket.bySrcPos	= bySrcSlotPos;
	sPacket.byDestPlace	= byDestPlace;
	sPacket.byDestPos	= byDestSlostPos;

	API_GetSLPacketLockManager()->Lock( GU_ITEM_MOVE_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendItemStackMoveReq(RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwUInt8 byDestPlace, RwUInt8 byDestPos, RwUInt8 byStackCount)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_ITEM_MOVE_STACK_RES ) )
		return true;

	sUG_ITEM_MOVE_STACK_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UG_ITEM_MOVE_STACK_REQ;
	sPacket.bySrcPlace	= bySrcPlace;
	sPacket.bySrcPos	= bySrcPos;
	sPacket.byDestPlace = byDestPlace;
	sPacket.byDestPos	= byDestPos;
	sPacket.byStackCount= byStackCount;

	API_GetSLPacketLockManager()->Lock( GU_ITEM_MOVE_STACK_RES );	
	
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}	

bool CGamePacketGenerator::SendItemDeleteReq( RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwBool* pPacketLock )
{
	if( API_GetSLPacketLockManager()->IsLock( GU_ITEM_DELETE_RES ) )
	{
		*pPacketLock = TRUE;
		return true;
	}
	else
		*pPacketLock = FALSE;

	sUG_ITEM_DELETE_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UG_ITEM_DELETE_REQ;
	sPacket.bySrcPlace = bySrcPlace;
	sPacket.bySrcPos = bySrcPos;

	API_GetSLPacketLockManager()->Lock( GU_ITEM_DELETE_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendItemUpgradeReq(RwUInt32 hNPCSerialId, RwUInt8 byItemPlace, RwUInt8 byItemPos, RwUInt8 byStonePlace, RwUInt8 byStonePos, RwUInt8 byCorePlace, RwUInt8 byCorePos)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_ITEM_UPGRADE_RES ) )
		return true;

	sUG_ITEM_UPGRADE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_UPGRADE_REQ;
	sPacket.handle = hNPCSerialId;
	sPacket.byItemPlace = byItemPlace;
	sPacket.byItemPos = byItemPos;
	sPacket.byStonePlace = byStonePlace;
	sPacket.byStonePos = byStonePos;
	sPacket.byCorePlace = byCorePlace;
	sPacket.byCorePos = byCorePos;

	API_GetSLPacketLockManager()->Lock( GU_ITEM_UPGRADE_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendItemChangeOptionReq(RwUInt8 byItemPlace, RwUInt8 byItemPos, RwUInt8 byKitPlace, RwUInt8 byKitPos, WORD wOptionIndex)
{
	sUG_ITEM_CHANGE_OPTION_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_CHANGE_OPTION_REQ;
	sPacket.byItemPlace = byItemPlace;
	sPacket.byItemPos = byItemPos;
	sPacket.byKitPlace = byKitPlace;
	sPacket.byKitPos = byKitPos;
	sPacket.wOptionIndex = wOptionIndex;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendQuickTeleportLoadReq(RwUInt8 byItemPlace, RwUInt8 byItemPos)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_QUICK_TELEPORT_LOAD_RES))
		return true;

	sUG_QUICK_TELEPORT_LOAD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_QUICK_TELEPORT_LOAD_REQ;
	sPacket.byPlace = byItemPlace;
	sPacket.byPos = byItemPos;

	API_GetSLPacketLockManager()->Lock(GU_QUICK_TELEPORT_LOAD_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendQuickTeleportUpdateReq(HOBJECT hHandle, BYTE bySlot)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_QUICK_TELEPORT_UPDATE_RES))
		return true;

	sUG_QUICK_TELEPORT_UPDATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_QUICK_TELEPORT_UPDATE_REQ;
	sPacket.hHandle = hHandle;
	sPacket.bySlot = bySlot;

	API_GetSLPacketLockManager()->Lock(GU_QUICK_TELEPORT_UPDATE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendQuickTeleportDelReq(HOBJECT hHandle, BYTE bySlot)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_QUICK_TELEPORT_DEL_RES))
		return true;

	sUG_QUICK_TELEPORT_DEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_QUICK_TELEPORT_DEL_REQ;
	sPacket.hHandle = hHandle;
	sPacket.bySlot = bySlot;

	API_GetSLPacketLockManager()->Lock(GU_QUICK_TELEPORT_DEL_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendQuickTeleportUseReq(HOBJECT hHandle, BYTE bySlot)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_QUICK_TELEPORT_USE_RES))
		return true;

	sUG_QUICK_TELEPORT_USE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_QUICK_TELEPORT_USE_REQ;
	sPacket.hHandle = hHandle;
	sPacket.bySlot = bySlot;

	API_GetSLPacketLockManager()->Lock(GU_QUICK_TELEPORT_USE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendItemChangeBattleAttributeReq(RwUInt32 hNPCSerialId, RwUInt8 byItemPlace, RwUInt8 byItemPos, RwUInt8 byAdditialItemPlace, RwUInt8 byAdditialItemPos, RwUInt8 byAdditionalAttribute)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_ITEM_CHANGE_BATTLE_ATTRIBUTE_RES))
		return true;

	sUG_ITEM_CHAGE_BATTLE_ATTRIBUTE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_CHAGE_BATTLE_ATTRIBUTE_REQ;
	sPacket.npcHandle = hNPCSerialId;
	sPacket.byItemPlace = byItemPlace;
	sPacket.byItemPos = byItemPos;
	sPacket.byAdditialItemPlace = byAdditialItemPlace;
	sPacket.byAdditialItemPos = byAdditialItemPos;
	sPacket.byAdditionalAttribute = byAdditionalAttribute;

	API_GetSLPacketLockManager()->Lock(GU_ITEM_CHANGE_BATTLE_ATTRIBUTE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPickUpWorldItemReq(RwUInt32 hTargetSerialId)
{
// I unlocked the pack lock to pick up fast items. (Jang Hyung Suk)
//	if( API_GetSLPacketLockManager()->IsLock( GU_ITEM_PICK_RES ) )
//		return true;

	CNtlSobWorldItem *pSobObj = reinterpret_cast<CNtlSobWorldItem*>( GetNtlSobManager()->GetSobObject( hTargetSerialId ) );
	if(pSobObj)
		pSobObj->SetLootSend(TRUE);

	sUG_ITEM_PICK_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_PICK_REQ;

	sPacket.byAvatarType = DBO_AVATAR_TYPE_AVATAR;
	sPacket.handle = hTargetSerialId;

// I unlocked the pack lock to pick up fast items. (Jang Hyung Suk)
//	API_GetSLPacketLockManager()->Lock( GU_ITEM_PICK_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);	
}

bool CGamePacketGenerator::SendPickUpWorldZennyReq(RwUInt32 hTargetSerialId)
{
// I unlocked the pack lock to pick up fast items. (Jang Hyung Suk)
//	if( API_GetSLPacketLockManager()->IsLock( GU_ZENNY_PICK_RES ) )
//		return true;

	CNtlSobWorldItem *pSobObj = reinterpret_cast<CNtlSobWorldItem*>( GetNtlSobManager()->GetSobObject(hTargetSerialId) );
	if(pSobObj)
		pSobObj->SetLootSend(TRUE);

	sUG_ZENNY_PICK_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ZENNY_PICK_REQ;

	sPacket.byAvatarType = DBO_AVATAR_TYPE_AVATAR;
	sPacket.handle = hTargetSerialId;

// I unlocked the pack lock to pick up fast items. (Jang Hyung Suk)
//	API_GetSLPacketLockManager()->Lock( GU_ZENNY_PICK_RES );
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendQuestItemMoveReq(RwUInt8 bySrcSlotIdx, RwUInt8 byDestSlotIdx)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_QUEST_ITEM_MOVE_RES ) )
		return true;

	sUG_QUEST_ITEM_MOVE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_QUEST_ITEM_MOVE_REQ;
	sPacket.bySrcPos = bySrcSlotIdx;
	sPacket.byDestPos = byDestSlotIdx;

	API_GetSLPacketLockManager()->Lock( GU_QUEST_ITEM_MOVE_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendQuestItemDeleteReq(RwUInt8 bySrcSlotIdx, RwBool* pPacketLock)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_QUEST_ITEM_DELETE_RES ) )
	{
		*pPacketLock = TRUE;
		return true;
	}
	else
		*pPacketLock = FALSE;

	sUG_QUEST_ITEM_DELETE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_QUEST_ITEM_DELETE_REQ;
	sPacket.byDeletePos = bySrcSlotIdx;

	API_GetSLPacketLockManager()->Lock( GU_QUEST_ITEM_DELETE_RES );
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}	

bool CGamePacketGenerator::SendSetIconToQuickSlot(RwUInt32 tblidx, RwUInt8 bySlotID, RwUInt8 byType, RwUInt8 byPlace, RwUInt8 byPos)
{
	NTL_ASSERT( bySlotID >= 0 && bySlotID < 48, "CGamePacketGenerator::SendSetIconToQuickSlot : SlotID Error!(" << bySlotID << ")" );

	sUG_QUICK_SLOT_UPDATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_QUICK_SLOT_UPDATE_REQ;
	sPacket.tblidx = tblidx;
	sPacket.bySlotID = bySlotID;
	sPacket.byPlace = byPlace;
	sPacket.byPos = byPos;
	sPacket.byType = byType;

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendUnsetIconFromQuickSlot(RwUInt8 bySlotID)
{
	sUG_QUICK_SLOT_DEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_QUICK_SLOT_DEL_REQ;
	sPacket.bySlotID = bySlotID;

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendItemSealReq(BYTE byPlace, BYTE byPos, BYTE bySealPlace, BYTE bySealPos)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_ITEM_SEAL_RES))
		return true;

	sUG_ITEM_SEAL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_SEAL_REQ;
	sPacket.byItemPlace = byPlace;
	sPacket.byItemPos = byPos;
	sPacket.bySealPlace = bySealPlace;
	sPacket.bySealPos = bySealPos;

	API_GetSLPacketLockManager()->Lock(GU_ITEM_SEAL_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendItemSealExtractReq(BYTE byPlace, BYTE byPos)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_ITEM_SEAL_EXTRACT_RES))
		return true;

	sUG_ITEM_SEAL_EXTRACT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_SEAL_EXTRACT_REQ;
	sPacket.byItemPlace = byPlace;
	sPacket.byItemPos = byPos;

	API_GetSLPacketLockManager()->Lock(GU_ITEM_SEAL_EXTRACT_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartySelectStateReq(BYTE bySelectState)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_PARTY_SELECT_STATE_RES))
		return true;

	sUG_PARTY_SELECT_STATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PARTY_SELECT_STATE_REQ;
	sPacket.bySelectState = bySelectState;

	API_GetSLPacketLockManager()->Lock(GU_PARTY_SELECT_STATE_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendUltimateDungeonEnterReq(HOBJECT hNpc, BYTE byDifficulty)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_ULTIMATE_DUNGEON_ENTER_RES))
		return true;

	sUG_ULTIMATE_DUNGEON_ENTER_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ULTIMATE_DUNGEON_ENTER_REQ;
	sPacket.hRequestNpc = hNpc;
	sPacket.byDifficulty = byDifficulty;

	API_GetSLPacketLockManager()->Lock(GU_ULTIMATE_DUNGEON_ENTER_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBattleDungeonEnterReq(HOBJECT hNpc, HOBJECT hItem)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_BATTLE_DUNGEON_ENTER_RES))
		return true;

	sUG_BATTLE_DUNGEON_ENTER_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BATTLE_DUNGEON_ENTER_REQ;
	sPacket.hNpc = hNpc;
	sPacket.hItem = hItem;

	API_GetSLPacketLockManager()->Lock(GU_BATTLE_DUNGEON_ENTER_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBattleDungeonLeaveReq()
{
	if (API_GetSLPacketLockManager()->IsLock(GU_BATTLE_DUNGEON_LEAVE_RES))
		return true;

	sUG_BATTLE_DUNGEON_LEAVE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BATTLE_DUNGEON_LEAVE_REQ;

	API_GetSLPacketLockManager()->Lock(GU_BATTLE_DUNGEON_LEAVE_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}


bool CGamePacketGenerator::SendServerCommand(const WCHAR *pCmd)
{
	sUG_SERVER_COMMAND sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UG_SERVER_COMMAND;
	sPacket.wMessageLengthInUnicode = (WORD)wcslen(pCmd);
	wcscpy_s(sPacket.awchCommand, NTL_MAX_LENGTH_OF_CHAT_MESSAGE, pCmd);

	RwInt32 iLen = sizeof( sPacket ) - sizeof(WCHAR) * (NTL_MAX_LENGTH_OF_CHAT_MESSAGE - sPacket.wMessageLengthInUnicode);

	return m_pNetSender->SendPacket( iLen, &sPacket ); 
}

bool CGamePacketGenerator::SendSkillLearnReq( RwUInt32 uiTblIdx )
{
	if( API_GetSLPacketLockManager()->IsLock( GU_SKILL_LEARN_RES ) )
		return true;

	sUG_SKILL_LEARN_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UG_SKILL_LEARN_REQ;
	sPacket.skillTblidx = uiTblIdx;

	API_GetSLPacketLockManager()->Lock( GU_SKILL_LEARN_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendSkillLearnByItemReq(BYTE byRequiredItemPlace, BYTE byRequiredItemPos)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_SKILL_LEARN_BY_ITEM_RES))
		return true;

	sUG_SKILL_LEARN_BY_ITEM_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SKILL_LEARN_BY_ITEM_REQ;
	sPacket.byRequiredItemPlace = byRequiredItemPlace;
	sPacket.byRequiredItemPos = byRequiredItemPos;

	API_GetSLPacketLockManager()->Lock(GU_SKILL_LEARN_BY_ITEM_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendSkillUpgradeReq( RwUInt8 ucSlotIdx )
{
	if( API_GetSLPacketLockManager()->IsLock( GU_SKILL_UPGRADE_RES ) )
		return true;

	sUG_SKILL_UPGRADE_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UG_SKILL_UPGRADE_REQ;
	sPacket.skillIndex = ucSlotIdx;

	API_GetSLPacketLockManager()->Lock( GU_SKILL_UPGRADE_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendHTBLearnReq( RwUInt32 hTrainer, RwUInt32 uiTblIdx )
{
	sUG_HTB_LEARN_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UG_HTB_LEARN_REQ;
	sPacket.hTrainer = hTrainer;
	sPacket.skillId = uiTblIdx;	
	
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendBuffDropReq(BYTE byBuffIndex, BYTE bySourceType, RwUInt32 uiTblIdx )
{
	sUG_BUFF_DROP_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UG_BUFF_DROP_REQ;
	sPacket.bySourceType = bySourceType;
	sPacket.tblidx = uiTblIdx;
	sPacket.buffIndex = byBuffIndex;

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendSkillRpBonusSettingReq(  RwUInt8 bySlotIndex, RwUInt8 byRpBonusType , RwBool bIsRpBonusAuto ) 
{
	if( API_GetSLPacketLockManager()->IsLock( GU_SKILL_RP_BONUS_SETTING_RES ) )
		return true;

	sUG_SKILL_RP_BONUS_SETTING_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UG_SKILL_RP_BONUS_SETTING_REQ;
	sPacket.skillIndex = bySlotIndex;
	sPacket.byRpBonusType = byRpBonusType;
	sPacket.bIsRpBonusAuto = B2b(bIsRpBonusAuto);

	API_GetSLPacketLockManager()->Lock( GU_SKILL_RP_BONUS_SETTING_RES );
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendCharRevivalReq(RwUInt8 byType)
{
	sUG_CHAR_REVIVAL_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UG_CHAR_REVIVAL_REQ;
	sPacket.byRevivalRequestType = byType;

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendCharCrescentPopoRevivalReq(BYTE byPlace, BYTE byPos)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_CHAR_CRESCENT_POPO_REVIVAL_RES))
		return false;

	sUG_CHAR_CRESCENT_POPO_REVIVAL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CHAR_CRESCENT_POPO_REVIVAL_REQ;
	sPacket.byPlace = byPlace;
	sPacket.byPos = byPos;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendNPCShopStartReq(RwUInt32 uiSerial, BYTE byType, RwBool* bPacketLock)
{
	if( !bPacketLock )
		return true;

	*bPacketLock = API_GetSLPacketLockManager()->IsLock( GU_SHOP_START_RES );

	if( *bPacketLock )
		return true;

	sUG_SHOP_START_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SHOP_START_REQ;
	sPacket.handle = uiSerial;
	sPacket.byType = byType;

	API_GetSLPacketLockManager()->Lock(GU_SHOP_START_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendNPCShopBuy(RwUInt32 uiNPCSerial, sSHOP_BUY_CART* pShopBuyCart[NTL_MAX_BUY_SHOPPING_CART])
{	
	if( API_GetSLPacketLockManager()->IsLock(GU_SHOP_BUY_RES) )
		return true;

	RwInt32 iLength;

	sUG_SHOP_BUY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SHOP_BUY_REQ;
	sPacket.handle = uiNPCSerial;

	BYTE iEmptySlot = 0;
	for( int i = 0 ; i < NTL_MAX_BUY_SHOPPING_CART ; ++i )
	{
		if( pShopBuyCart[i] )
		{
			if( pShopBuyCart[i]->byStack > 0 )
			{
				memcpy((void*)&sPacket.sBuyData[iEmptySlot], (void*)pShopBuyCart[i], sizeof(sSHOP_BUY_CART) );
				++iEmptySlot;
			}
		}
	}	

	if(iEmptySlot == 0)
		return true;

	sPacket.byBuyCount = iEmptySlot;

	iLength = sizeof(sPacket.wOpCode) + sizeof(sPacket.handle) + sizeof(sPacket.byBuyCount)
				+ ( sizeof(sSHOP_BUY_CART) * sPacket.byBuyCount );

	API_GetSLPacketLockManager()->Lock(GU_SHOP_BUY_RES);
	return m_pNetSender->SendPacket( iLength, &sPacket );
}

bool CGamePacketGenerator::SendNPCShopSell(RwInt32 uiNPCSerial, sSHOP_SELL_CART* pShopSellCart[NTL_MAX_SELL_SHOPPING_CART])
{
	// 서버로부터 먼저보낸 메세지가 있는데 아직 안왔다면 false 리턴
	if( API_GetSLPacketLockManager()->IsLock(GU_SHOP_SELL_RES) )
		return true;

	RwInt32 iLength;
	sUG_SHOP_SELL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SHOP_SELL_REQ;
	sPacket.handle = uiNPCSerial;
	
	BYTE iEmptySlot = 0;
	for( int i = 0 ; i < NTL_MAX_BUY_SHOPPING_CART ; ++i )
	{
		if( pShopSellCart[i] )
		{
			if( pShopSellCart[i]->byStack > 0 )
			{
				memcpy((void*)&sPacket.sSellData[iEmptySlot], (void*)pShopSellCart[i], sizeof(sSHOP_BUY_CART) );
				++iEmptySlot;
			}
		}
	}	

	if(iEmptySlot == 0)
		return true;

	sPacket.bySellCount = iEmptySlot;

	iLength = sizeof(sPacket.wOpCode) + sizeof(sPacket.handle) + sizeof(sPacket.bySellCount)
		+ ( sizeof(sSHOP_SELL_CART) * sPacket.bySellCount );

	// 서버로부터 응답을 기다리는 메세지 목록에 추가
	API_GetSLPacketLockManager()->Lock(GU_SHOP_SELL_RES);
	return m_pNetSender->SendPacket(iLength, &sPacket);
}

bool CGamePacketGenerator::SendNPCShopClose()
{
	if( API_GetSLPacketLockManager()->IsLock(GU_SHOP_END_RES) )
		return true;

	sUG_SHOP_END_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SHOP_END_REQ;

	API_GetSLPacketLockManager()->Lock(GU_SHOP_END_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendEventItemShopStartReq(RwInt32 uiNPCSerial, RwBool* bPacketLock)
{
	if( !bPacketLock )
		return true;

	*bPacketLock = API_GetSLPacketLockManager()->IsLock( GU_SHOP_EVENTITEM_START_RES );

	if( *bPacketLock )
		return true;

	sUG_SHOP_EVENTITEM_START_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SHOP_EVENTITEM_START_REQ;
	sPacket.handle = uiNPCSerial;

	API_GetSLPacketLockManager()->Lock(GU_SHOP_EVENTITEM_START_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendEventItemShopBuyReq(RwInt32 uiNPCSerial, sSHOP_BUY_CART* pShopBuyCart[NTL_MAX_BUY_SHOPPING_CART])
{
	if( API_GetSLPacketLockManager()->IsLock(GU_SHOP_EVENTITEM_BUY_RES) )
		return true;

	RwInt32 iLength;
	sUG_SHOP_EVENTITEM_BUY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode		= UG_SHOP_EVENTITEM_BUY_REQ;
	sPacket.handle		= uiNPCSerial;

	BYTE iEmptySlot = 0;
	for( int i = 0 ; i < NTL_MAX_BUY_SHOPPING_CART ; ++i )
	{
		if( pShopBuyCart[i] )
		{
			if( pShopBuyCart[i]->byStack > 0 )
			{
				memcpy((void*)&sPacket.sBuyData[iEmptySlot], (void*)pShopBuyCart[i], sizeof(sSHOP_BUY_CART) );
				++iEmptySlot;
			}
		}
	}	

	if(iEmptySlot == 0)
		return true;

	sPacket.byBuyCount = iEmptySlot;

	iLength = sizeof(sPacket.wOpCode) + sizeof(sPacket.handle) + sizeof(sPacket.byBuyCount)
		+ ( sizeof(sSHOP_BUY_CART) * sPacket.byBuyCount );

	API_GetSLPacketLockManager()->Lock(GU_SHOP_EVENTITEM_BUY_RES);
	return m_pNetSender->SendPacket( iLength, &sPacket );
}

bool CGamePacketGenerator::SendEventItemShopEndReq()
{
	if( API_GetSLPacketLockManager()->IsLock(GU_SHOP_EVENTITEM_END_RES) )
		return true;

	sUG_SHOP_EVENTITEM_END_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SHOP_EVENTITEM_END_REQ;

	API_GetSLPacketLockManager()->Lock(GU_SHOP_EVENTITEM_END_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendItemRepair(RwInt32 uiNPCSerial, RwUInt8 iPlace, RwUInt8 iPos)
{
	// 서버로부터 먼저보낸 메세지가 있는데 아직 안왔다면 false 리턴
	if( API_GetSLPacketLockManager()->IsLock(GU_ITEM_REPAIR_RES) )
		return true;
	
	sUG_ITEM_REPAIR_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_REPAIR_REQ;
	sPacket.handle = uiNPCSerial;
	sPacket.byPlace = iPlace;
	sPacket.byPos = iPos;

	// 서버로부터 응답을 기다리는 메세지 목록에 추가
	API_GetSLPacketLockManager()->Lock(GU_ITEM_REPAIR_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);	
}

bool CGamePacketGenerator::SendItemIdentification(RwUInt8 byPlace, RwUInt8 byPos)
{
	// 서버로부터 먼저보낸 메세지가 있는데 아직 안왔다면 false 리턴
	if( API_GetSLPacketLockManager()->IsLock(GU_ITEM_IDENTIFY_RES) )
		return true;

	sUG_ITEM_IDENTIFY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_IDENTIFY_REQ;
	sPacket.byPlace = byPlace;
	sPacket.byPos = byPos;

	// 서버로부터 응답을 기다리는 메세지 목록에 추가
	API_GetSLPacketLockManager()->Lock(GU_ITEM_IDENTIFY_RES);
	
	// Item Identify Effect
	/*CDboEventGenerator::ItemIdentifyEffect( FALSE, INVALID_SERIAL_ID, byPlace, byPos );*/

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);	
}

bool CGamePacketGenerator::SendPCInfoView(RwUInt32 uiSerial)
{
	sUG_SCOUTER_EQUIP_CHECK_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SCOUTER_EQUIP_CHECK_REQ;
	sPacket.hTarget = uiSerial;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);	
}

bool CGamePacketGenerator::SendItemAllRepair(RwInt32 uiNPCSerial)
{
	// 서버로부터 먼저보낸 메세지가 있는데 아직 안왔다면 false 리턴
	if( API_GetSLPacketLockManager()->IsLock(GU_ITEM_EQUIP_REPAIR_RES) )
		return true;

	sUG_ITEM_EQUIP_REPAIR_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_EQUIP_REPAIR_REQ;
	sPacket.handle = uiNPCSerial;

	// 서버로부터 응답을 기다리는 메세지 목록에 추가
	API_GetSLPacketLockManager()->Lock(GU_ITEM_EQUIP_REPAIR_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);	
}

bool CGamePacketGenerator::SendItemIdentifyReq(RwInt32 uiNPCSerial, RwUInt8 iPlace, RwUInt8 iPos)
{
	// If there is a message previously sent from the server but it has not been done yet, it returns false.
	if( API_GetSLPacketLockManager()->IsLock(GU_SHOP_ITEM_IDENTIFY_RES) )
		return true;

	sUG_SHOP_ITEM_IDENTIFY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SHOP_ITEM_IDENTIFY_REQ;
	sPacket.hNpchandle	= uiNPCSerial;
	sPacket.byPlace		= iPlace;
	sPacket.byPos		= iPos;

	// Add to the list of messages waiting for a response from the server
	API_GetSLPacketLockManager()->Lock(GU_SHOP_ITEM_IDENTIFY_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);	
}

bool CGamePacketGenerator::SendItemDisassembleReq(RwUInt8 iPlace, RwUInt8 iPos)
{
	// If there is a message previously sent from the server but it has not been done yet, it returns false.
	if (API_GetSLPacketLockManager()->IsLock(GU_ITEM_DISASSEMBLE_RES))
		return true;

	sUG_ITEM_DISASSEMBLE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_DISASSEMBLE_REQ;
	sPacket.byPlace = iPlace;
	sPacket.byPos = iPos;

	// Add to the list of messages waiting for a response from the server
	API_GetSLPacketLockManager()->Lock(GU_ITEM_DISASSEMBLE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendItemSocketInsertBeadReq(RwUInt8 iPlace, RwUInt8 iPos, RwUInt8 iBeadPlace, RwUInt8 iBeadPos)
{
	// If there is a message previously sent from the server but it has not been done yet, it returns false.
	if (API_GetSLPacketLockManager()->IsLock(GU_ITEM_SOCKET_INSERT_BEAD_RES))
		return true;

	sUG_ITEM_SOCKET_INSERT_BEAD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_SOCKET_INSERT_BEAD_REQ;
	sPacket.byItemPlace = iPlace;
	sPacket.byItemPos = iPos;
	sPacket.byBeadPlace = iBeadPlace;
	sPacket.byBeadPos = iBeadPos;

	// Add to the list of messages waiting for a response from the server
	API_GetSLPacketLockManager()->Lock(GU_ITEM_SOCKET_INSERT_BEAD_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendItemSocketDestroyBeadReq(RwUInt8 iPlace, RwUInt8 iPos)
{
	// If there is a message previously sent from the server but it has not been done yet, it returns false.
	if (API_GetSLPacketLockManager()->IsLock(GU_ITEM_SOCKET_DESTROY_BEAD_RES))
		return true;

	sUG_ITEM_SOCKET_DESTROY_BEAD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_SOCKET_DESTROY_BEAD_REQ;
	sPacket.byItemPlace = iPlace;
	sPacket.byItemPos = iPos;

	// Add to the list of messages waiting for a response from the server
	API_GetSLPacketLockManager()->Lock(GU_ITEM_SOCKET_DESTROY_BEAD_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendItemExchangeReq(RwUInt32 uiNPCSerial, RwUInt8 byItemPos, RwUInt8 byMerchantTab, RwUInt8 byStack)
{
	// If there is a message previously sent from the server but it has not been done yet, it returns false.
	if (API_GetSLPacketLockManager()->IsLock(GU_ITEM_EXCHANGE_RES))
		return true;

	sUG_ITEM_EXCHANGE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_EXCHANGE_REQ;
	sPacket.handle = uiNPCSerial;
	sPacket.sBuyData.byItemPos = byItemPos;
	sPacket.sBuyData.byMerchantTab = byMerchantTab;
	sPacket.sBuyData.byStack = byStack;


	// Add to the list of messages waiting for a response from the server
	API_GetSLPacketLockManager()->Lock(GU_ITEM_EXCHANGE_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendScouterMeasure(RwUInt32 uiTargetSerial)
{
	sUG_SCOUTER_INDICATOR_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SCOUTER_INDICATOR_REQ;
	sPacket.hTarget = uiTargetSerial;	

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyCreate(const WCHAR* pcText)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_PARTY_CREATE_RES) )
		return true;

	sUG_PARTY_CREATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));
	
	sPacket.wOpCode = UG_PARTY_CREATE_REQ;
	memcpy(sPacket.wszPartyName, pcText, sizeof(WCHAR)*wcslen(pcText));

	API_GetSLPacketLockManager()->Lock(GU_PARTY_CREATE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyDisband()
{
	if( API_GetSLPacketLockManager()->IsLock(GU_PARTY_DISBAND_RES) )
		return true;

	sUG_PARTY_DISBAND_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PARTY_DISBAND_REQ;

	API_GetSLPacketLockManager()->Lock(GU_PARTY_DISBAND_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyInvite(RwUInt32 uiSerial)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_PARTY_INVITE_RES) )
		return true;

	sUG_PARTY_INVITE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PARTY_INVITE_REQ;
	sPacket.hTarget = uiSerial;

	API_GetSLPacketLockManager()->Lock(GU_PARTY_INVITE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyInvite_CharID( RwUInt32 uiCharID ) 
{
	// 이 패킷의 응답으로는 UG_PARTY_INVITE_REQ 의 응답과 같은 것이 온다
	if( API_GetSLPacketLockManager()->IsLock(GU_PARTY_INVITE_RES) )
		return true;

    sUG_PARTY_INVITE_CHARID_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UG_PARTY_INVITE_CHARID_REQ;
    sPacket.targetCharId = uiCharID;

	API_GetSLPacketLockManager()->Lock(GU_PARTY_INVITE_RES);
    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyInvite_Name(const WCHAR* pcName)
{
	// 이 패킷의 응답으로는 UG_PARTY_INVITE_REQ 의 응답과 같은 것이 온다
	if( API_GetSLPacketLockManager()->IsLock(GU_PARTY_INVITE_RES) )
		return true;

	sUG_PARTY_INVITE_CHAR_NAME_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PARTY_INVITE_CHAR_NAME_REQ;
	swprintf_s(sPacket.wszTargetName, sizeof(sPacket.wszTargetName)/2, L"%s", pcName);

	API_GetSLPacketLockManager()->Lock(GU_PARTY_INVITE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyResponseInvite(RwUInt32 byResponse)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_PARTY_RESPONSE_INVITATION_RES) )
		return true;

	sUG_PARTY_RESPONSE_INVITATION sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PARTY_RESPONSE_INVITATION;
	sPacket.byResponse = (BYTE)byResponse;

	API_GetSLPacketLockManager()->Lock(GU_PARTY_RESPONSE_INVITATION_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyLeave()
{	
	if( API_GetSLPacketLockManager()->IsLock(GU_PARTY_LEAVE_RES) )
		return true;

	sUG_PARTY_LEAVE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PARTY_LEAVE_REQ;

	API_GetSLPacketLockManager()->Lock(GU_PARTY_LEAVE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyKickOut(RwUInt32 uiSerial)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_PARTY_KICK_OUT_RES) )
		return true;

	sUG_PARTY_KICK_OUT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PARTY_KICK_OUT_REQ;
	sPacket.hTargetMember = uiSerial;

	API_GetSLPacketLockManager()->Lock(GU_PARTY_KICK_OUT_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyChangeLeader(RwUInt32 uiSerial)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_PARTY_CHANGE_LEADER_RES) )
		return true;

	sUG_PARTY_CHANGE_LEADER_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PARTY_CHANGE_LEADER_REQ;
	sPacket.hTargetMember = uiSerial;

	API_GetSLPacketLockManager()->Lock(GU_PARTY_CHANGE_LEADER_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyZennyDivision(RwUInt8 byDivision)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES ) )
		return true;	

	sUG_PARTY_CHANGE_ZENNY_LOOTING_METHOD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PARTY_CHANGE_ZENNY_LOOTING_METHOD_REQ;
	sPacket.byLootingMethod = byDivision;

	API_GetSLPacketLockManager()->Lock( GU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES );
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyItemDivision(RwUInt8 byDivision)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES ) )
		return true;	

	sUG_PARTY_CHANGE_ITEM_LOOTING_METHOD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PARTY_CHANGE_ITEM_LOOTING_METHOD_REQ;
	sPacket.byLootingMethod = byDivision;

	API_GetSLPacketLockManager()->Lock( GU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES );
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBankLoadReq(RwUInt32 uiNPCSerial)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BANK_LOAD_RES ) )
		return true;

	sUG_BANK_LOAD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BANK_LOAD_REQ;
	sPacket.handle	= uiNPCSerial;

	API_GetSLPacketLockManager()->Lock(GU_BANK_LOAD_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBankStart(RwUInt32 uiNPCSerial, RwBool* pSendResult)
{
	if( !pSendResult )
		return true;

	*pSendResult = API_GetSLPacketLockManager()->IsLock( GU_BANK_START_RES );

	if( *pSendResult )
		return true;

	sUG_BANK_START_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BANK_START_REQ;
	sPacket.handle = uiNPCSerial;

	API_GetSLPacketLockManager()->Lock(GU_BANK_START_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBankMove(RwUInt32 uiNPCSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwUInt8 byDestPlace, RwUInt8 byDestPos)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BANK_MOVE_RES ) )
		return true;

	sUG_BANK_MOVE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BANK_MOVE_REQ;
	sPacket.handle = uiNPCSerial;
	sPacket.bySrcPlace = bySrcPlace;
	sPacket.bySrcPos = bySrcPos;
	sPacket.byDestPlace = byDestPlace;
	sPacket.byDestPos = byDestPos;

	// 서버로부터 응답을 기다리는 메세지 목록에 추가
	API_GetSLPacketLockManager()->Lock(GU_BANK_MOVE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBankMoveStack(RwUInt32 uiNPCSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwUInt8 byDestPlace, RwUInt8 byDestPos, RwUInt8 byStackCount)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BANK_MOVE_STACK_RES ) )
		return true;

	sUG_BANK_MOVE_STACK_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BANK_MOVE_STACK_REQ;
	sPacket.handle = uiNPCSerial;
	sPacket.bySrcPlace = bySrcPlace;
	sPacket.bySrcPos = bySrcPos;
	sPacket.byDestPlace = byDestPlace;
	sPacket.byDestPos = byDestPos;
	sPacket.byStackCount = byStackCount;

	// 서버로부터 응답을 기다리는 메세지 목록에 추가
	API_GetSLPacketLockManager()->Lock(GU_BANK_MOVE_STACK_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBankEnd()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BANK_END_RES ) )
		return true;

	sUG_BANK_END_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BANK_END_REQ;

	API_GetSLPacketLockManager()->Lock(GU_BANK_END_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBankZenny(RwUInt32 uiNPCSerial, RwUInt32 uiZenny, bool bIsSave)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BANK_ZENNY_RES ) )
		return true;

	sUG_BANK_ZENNY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BANK_ZENNY_REQ;
	sPacket.handle = uiNPCSerial;
	sPacket.dwZenny = uiZenny;
	sPacket.bIsSave = bIsSave;

	// 서버로부터 응답을 기다리는 메세지 목록에 추가
	API_GetSLPacketLockManager()->Lock(GU_BANK_ZENNY_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBankBuy(RwUInt32 uiNPCSerial, RwUInt8 byMerchantTab, RwUInt8 byPos)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BANK_BUY_RES ) )
		return true;

	sUG_BANK_BUY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BANK_BUY_REQ;
	sPacket.hNpchandle = uiNPCSerial;
	sPacket.byMerchantTab = byMerchantTab;
	sPacket.byPos = byPos;

	API_GetSLPacketLockManager()->Lock(GU_BANK_BUY_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBankDeleteItem(RwUInt8 byPlace, RwUInt8 byPos, RwBool* pPacketLock)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BANK_ITEM_DELETE_RES ) )
	{
		*pPacketLock = TRUE;
		return true;
	}
	else
		*pPacketLock = FALSE;

	sUG_BANK_ITEM_DELETE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BANK_ITEM_DELETE_REQ;
	sPacket.byPlace = byPlace;
	sPacket.byPos = byPos;

	// 서버로부터 응답을 기다리는 메세지 목록에 추가
	API_GetSLPacketLockManager()->Lock(GU_BANK_ITEM_DELETE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendFreeBattleChallengeReq(RwUInt32 uiTargetSerial)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_FREEBATTLE_CHALLENGE_RES ) )
		return true;

	sUG_FREEBATTLE_CHALLENGE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_FREEBATTLE_CHALLENGE_REQ;
	sPacket.hTarget = uiTargetSerial;

	API_GetSLPacketLockManager()->Lock(GU_FREEBATTLE_CHALLENGE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendFreeBattleAcceptRes(RwUInt8 byAccept)
{
	sUG_FREEBATTLE_ACCEPT_RES sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_FREEBATTLE_ACCEPT_RES;
	sPacket.byAccept = byAccept;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTradeStartReq(RwUInt32 uiSerial)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_TRADE_START_RES) )
		return true;

	sUG_TRADE_START_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TRADE_START_REQ;
	sPacket.hTarget = uiSerial;

	API_GetSLPacketLockManager()->Lock(GU_TRADE_START_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTradeOkReq(RwUInt32 uiSerial, RwUInt8 byOK)
{
	sUG_TRADE_OK_RES sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TRADE_OK_RES;
	sPacket.handle = uiSerial;
	sPacket.byOK = byOK;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTradeAddReq(RwUInt32 uiTarget, RwUInt32 uiItem, RwUInt8 byCount)
{
	// If there is a message previously sent from the server but it has not been done yet, it returns false.
	if( API_GetSLPacketLockManager()->IsLock(GU_TRADE_ADD_RES) )
		return true;

	sUG_TRADE_ADD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TRADE_ADD_REQ;
	sPacket.hTarget = uiTarget;
	sPacket.hItem = uiItem;
	sPacket.byCount = byCount;

	// Add to the list of messages waiting for a response from the server
	API_GetSLPacketLockManager()->Lock(GU_TRADE_ADD_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTradeDelReq(RwUInt32 uiTarget, RwUInt32 uiItem)
{
	// 서버로부터 먼저보낸 메세지가 있는데 아직 안왔다면 false 리턴
	if( API_GetSLPacketLockManager()->IsLock(GU_TRADE_DEL_RES) )
		return true;

	sUG_TRADE_DEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TRADE_DEL_REQ;
	sPacket.hTarget = uiTarget;
	sPacket.hItem = uiItem;

	// 서버로부터 응답을 기다리는 메세지 목록에 추가
	API_GetSLPacketLockManager()->Lock(GU_TRADE_DEL_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTradeUpdateItem(RwUInt32 uiTarget, RwUInt32 uiItem, RwUInt8 byCount)
{
	// 서버로부터 먼저보낸 메세지가 있는데 아직 안왔다면 false 리턴
	if( API_GetSLPacketLockManager()->IsLock(GU_TRADE_MODIFY_RES) )
		return true;

	sUG_TRADE_MODIFY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TRADE_MODIFY_REQ;
	sPacket.hTarget = uiTarget;
	sPacket.hItem = uiItem;
	sPacket.byCount = byCount;

	// 서버로부터 응답을 기다리는 메세지 목록에 추가
	API_GetSLPacketLockManager()->Lock(GU_TRADE_MODIFY_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTradeZennyUpdateReq(RwUInt32 uiTarget, RwUInt32 uiZenny)
{
	// 서버로부터 먼저보낸 메세지가 있는데 아직 안왔다면 false 리턴
	if( API_GetSLPacketLockManager()->IsLock(GU_TRADE_ZENNY_UPDATE_RES) )
		return true;

	sUG_TRADE_ZENNY_UPDATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TRADE_ZENNY_UPDATE_REQ;
	sPacket.hTarget = uiTarget;
	sPacket.dwZenny = uiZenny;

	// 서버로부터 응답을 기다리는 메세지 목록에 추가
	API_GetSLPacketLockManager()->Lock(GU_TRADE_ZENNY_UPDATE_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTradeEndReq(RwUInt32 uiTarget, RwUInt32 uiPacketCount, bool bLock)
{
	// 서버로부터 먼저보낸 메세지가 있는데 아직 안왔다면 false 리턴
	if( API_GetSLPacketLockManager()->IsLock(GU_TRADE_END_RES) )
		return true;

	sUG_TRADE_END_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TRADE_END_REQ;
	sPacket.hTarget = uiTarget;
	sPacket.dwPacketCount = uiPacketCount;
	sPacket.bIsSet = bLock;

	// 서버로부터 응답을 기다리는 메세지 목록에 추가
	API_GetSLPacketLockManager()->Lock(GU_TRADE_END_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTradeCancelReq(RwUInt32 uiTarget)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_TRADE_CANCEL_RES) )
		return true;

	sUG_TRADE_CANCEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TRADE_CANCEL_REQ;
	sPacket.hTarget = uiTarget;

	API_GetSLPacketLockManager()->Lock(GU_TRADE_CANCEL_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTradeDeclineReq(bool bDecline)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_TRADE_DENY_RES) )
		return true;

	sUG_TRADE_DENY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TRADE_DENY_REQ;
	sPacket.bIsDeny = bDecline;

	API_GetSLPacketLockManager()->Lock(GU_TRADE_DENY_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendCharRenameReq(const WCHAR * pwcCharName, BYTE byPlace, BYTE byPos)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_CHARACTER_RENAME_RES))
		return true;

	sUG_CHARACTER_RENAME_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CHARACTER_RENAME_REQ;
	memcpy(sPacket.awchCharName, pwcCharName, sizeof(WCHAR)*wcslen(pwcCharName));
	sPacket.byPlace = byPlace;
	sPacket.byPos = byPos;

	API_GetSLPacketLockManager()->Lock(GU_CHARACTER_RENAME_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGuildCreateReq(RwUInt32 uiNPCSerial, const WCHAR* pcGuildName)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_CREATE_RES) )
		return true;

	sUG_GUILD_CREATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_GUILD_CREATE_REQ;
	sPacket.hGuildManagerNpc = uiNPCSerial;
	memcpy(sPacket.wszGuildName, pcGuildName, sizeof(WCHAR)*wcslen(pcGuildName));

	API_GetSLPacketLockManager()->Lock(GU_GUILD_CREATE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGuildInviteReq(RwUInt32 uiTargetSerial)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_INVITE_RES) )
		return true;

	sUG_GUILD_INVITE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_GUILD_INVITE_REQ;
	sPacket.hTarget = uiTargetSerial;

	API_GetSLPacketLockManager()->Lock(GU_GUILD_INVITE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGuildFunctionAddReq(RwUInt32 uiNPCSerial, RwUInt8 byFunction)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_FUNCTION_ADD_RES) )
		return true;

	sUG_GUILD_FUNCTION_ADD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_GUILD_FUNCTION_ADD_REQ;
	sPacket.hGuildManagerNpc	= uiNPCSerial;
	sPacket.byFunction			= byFunction;

	API_GetSLPacketLockManager()->Lock(GU_GUILD_FUNCTION_ADD_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGuildChangeNameReq(const WCHAR* pwcGuildName, BYTE byPlace, BYTE byPos)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_CHANGE_NAME_RES) )
		return true;

	sUG_GUILD_CHANGE_NAME_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_GUILD_CHANGE_NAME_REQ;
	memcpy(sPacket.wszGuildName, pwcGuildName, sizeof(WCHAR)*wcslen(pwcGuildName));
	sPacket.byPlace = byPlace;
	sPacket.byPos = byPos;

	API_GetSLPacketLockManager()->Lock(GU_GUILD_CHANGE_NAME_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGuildCreateEmblemReq(RwUInt8 byTypeA, RwUInt8 byTypeAColor,
													RwUInt8 byTypeB, RwUInt8 byTypeBColor,
													RwUInt8 byTypeC, RwUInt8 byTypeCColor)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_CREATE_MARK_RES) )
		return true;

	sUG_GUILD_CREATE_MARK_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_GUILD_CREATE_MARK_REQ;
	sPacket.sMark.byMarkMain		= byTypeA;
	sPacket.sMark.byMarkMainColor	= byTypeAColor;
	sPacket.sMark.byMarkInLine		= byTypeB;
	sPacket.sMark.byMarkInColor		= byTypeBColor;
	sPacket.sMark.byMarkOutLine		= byTypeC;
	sPacket.sMark.byMarkOutColor	= byTypeCColor;

	API_GetSLPacketLockManager()->Lock(GU_GUILD_CREATE_MARK_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGuildChangeEmblemReq(RwUInt8 byTypeA, RwUInt8 byTypeAColor,
													RwUInt8 byTypeB, RwUInt8 byTypeBColor,
													RwUInt8 byTypeC, RwUInt8 byTypeCColor)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_CHANGE_MARK_RES) )
		return true;

	sUG_GUILD_CHANGE_MARK_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_GUILD_CHANGE_MARK_REQ;
	sPacket.sMark.byMarkMain		= byTypeA;
	sPacket.sMark.byMarkMainColor	= byTypeAColor;
	sPacket.sMark.byMarkInLine		= byTypeB;
	sPacket.sMark.byMarkInColor		= byTypeBColor;
	sPacket.sMark.byMarkOutLine		= byTypeC;
	sPacket.sMark.byMarkOutColor	= byTypeCColor;

	API_GetSLPacketLockManager()->Lock(GU_GUILD_CHANGE_MARK_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGuildGiveZenny(RwUInt32 uiNPCSerial, RwUInt32 uiZenny)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_GIVE_ZENNY_RES) )
		return true;

	sUG_GUILD_GIVE_ZENNY sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode				= UG_GUILD_GIVE_ZENNY;
	sPacket.hGuildManagerNpc	= uiNPCSerial;
	sPacket.dwZenny				= uiZenny;

	API_GetSLPacketLockManager()->Lock(GU_GUILD_GIVE_ZENNY_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGuildWarehouseStart_Req(RwUInt32 uiNPCSerial)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_BANK_START_RES) )
		return true;

	sUG_GUILD_BANK_START_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode				= UG_GUILD_BANK_START_REQ;
	sPacket.handle				= uiNPCSerial;

	API_GetSLPacketLockManager()->Lock(GU_GUILD_BANK_START_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGuildWarehouseMoveReq(RwUInt32 uiNPCSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwUInt8 byDestPlace, RwUInt8 byDestPos)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_BANK_MOVE_RES) )
		return true;

	sUG_GUILD_BANK_MOVE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode				= UG_GUILD_BANK_MOVE_REQ;
	sPacket.handle				= uiNPCSerial;
	sPacket.bySrcPlace			= bySrcPlace;
	sPacket.bySrcPos			= bySrcPos;
	sPacket.byDestPlace			= byDestPlace;
	sPacket.byDestPos			= byDestPos;

	API_GetSLPacketLockManager()->Lock(GU_GUILD_BANK_MOVE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGuildWarehouseMoveStackReq(RwUInt32 uiNPCSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwUInt8 byDestPlace, RwUInt8 byDestPos, RwUInt8 byStackCount)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_BANK_MOVE_STACK_RES) )
		return true;

	sUG_GUILD_BANK_MOVE_STACK_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode				= UG_GUILD_BANK_MOVE_STACK_REQ;
	sPacket.handle				= uiNPCSerial;
	sPacket.bySrcPlace			= bySrcPlace;
	sPacket.bySrcPos			= bySrcPos;
	sPacket.byDestPlace			= byDestPlace;
	sPacket.byDestPos			= byDestPos;
	sPacket.byStackCount		= byStackCount;

	API_GetSLPacketLockManager()->Lock(GU_GUILD_BANK_MOVE_STACK_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}
bool CGamePacketGenerator::SendGuildWarehouseZennyReq(RwUInt32 uiNPCSerial, RwUInt32 uiZenny, RwBool bSave)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_BANK_ZENNY_RES) )
		return true;

	sUG_GUILD_BANK_ZENNY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode				= UG_GUILD_BANK_ZENNY_REQ;
	sPacket.handle				= uiNPCSerial;
	sPacket.dwZenny				= uiZenny;
	sPacket.bIsSave				= B2b(bSave);

	API_GetSLPacketLockManager()->Lock(GU_GUILD_BANK_ZENNY_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGuildWarehouseEnd()
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_BANK_END_RES) )
		return true;

	sUG_GUILD_BANK_END_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_GUILD_BANK_END_REQ;

	API_GetSLPacketLockManager()->Lock(GU_GUILD_BANK_END_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendDojo_CreateReq(RwUInt32 uiNPCHandle)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_DOJO_CREATE_RES) )
		return true;

	sUG_DOJO_CREATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode		= UG_DOJO_CREATE_REQ;
	sPacket.hObject		= uiNPCHandle;

	API_GetSLPacketLockManager()->Lock(GU_DOJO_CREATE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendDojo_FunctionAddReq(RwUInt32 uiGuildManagerHandle, RwUInt8 byFunction,
												   RwUInt8 byPlace, RwUInt8 byPos)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_DOJO_FUNCTION_ADD_RES) )
		return true;

	sUG_DOJO_FUNCTION_ADD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode			= UG_DOJO_FUNCTION_ADD_REQ;
	sPacket.hGuildManagerNpc= uiGuildManagerHandle;
	sPacket.byFunction		= byFunction;
	sPacket.byPlace			= byPlace;
	sPacket.byPos			= byPos;

	API_GetSLPacketLockManager()->Lock(GU_DOJO_FUNCTION_ADD_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendDojo_ScrambleReq(RwUInt32 uiNPCHandle)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_DOJO_SCRAMBLE_RES) )
		return true;

	sUG_DOJO_SCRAMBLE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode		= UG_DOJO_SCRAMBLE_REQ;
	sPacket.hNpcHandle	= uiNPCHandle;

	API_GetSLPacketLockManager()->Lock(GU_DOJO_SCRAMBLE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendDojo_ScrambleResponseReq(bool bAccept)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_DOJO_SCRAMBLE_RESPONSE_RES) )
		return true;

	sUG_DOJO_SCRAMBLE_RESPONSE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode		= UG_DOJO_SCRAMBLE_RESPONSE_REQ;
	sPacket.bIsAccept	= bAccept;

	API_GetSLPacketLockManager()->Lock(GU_DOJO_SCRAMBLE_RESPONSE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendDojo_NPCInfoReq(RwUInt32 uiNPCSerial)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_DOJO_NPC_INFO_RES) )
		return true;

	sUG_DOJO_NPC_INFO_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode		= UG_DOJO_NPC_INFO_REQ;
	sPacket.hNpcHandle	= uiNPCSerial;

	API_GetSLPacketLockManager()->Lock(GU_DOJO_NPC_INFO_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendDogi_CreateReq(RwUInt32 uiHandle, void* pDogiData)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_DOGI_CREATE_RES) )
		return true;

	sUG_DOGI_CREATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_DOGI_CREATE_REQ;
	sPacket.hObject	= uiHandle;
	sPacket.sData	= *reinterpret_cast<sDBO_DOGI_DATA*>( pDogiData );

	API_GetSLPacketLockManager()->Lock(GU_DOGI_CREATE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendDogi_ChagneReq(RwUInt32 uiHandle, void* pDogiData)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_DOGI_CHANGE_RES) )
		return true;

	sUG_DOGI_CHANGE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_DOGI_CHANGE_REQ;
	sPacket.hObject	= uiHandle;
	sPacket.sData	= *reinterpret_cast<sDBO_DOGI_DATA*>( pDogiData );

	API_GetSLPacketLockManager()->Lock(GU_DOGI_CHANGE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGuildDogi_CreateReq(RwUInt32 uiHandle, void* pDogiData)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_DOGI_CREATE_RES) )
		return true;

	sUG_GUILD_DOGI_CREATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_GUILD_DOGI_CREATE_REQ;
	sPacket.hObject	= uiHandle;
	sPacket.sData	= *reinterpret_cast<sDBO_DOGI_DATA*>( pDogiData );

	API_GetSLPacketLockManager()->Lock(GU_GUILD_DOGI_CREATE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGuildDogi_ChagneReq(RwUInt32 uiHandle, void* pDogiData)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_GUILD_DOGI_CHANGE_RES) )
		return true;

	sUG_GUILD_DOGI_CHANGE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_GUILD_DOGI_CHANGE_REQ;
	sPacket.hObject	= uiHandle;
	sPacket.sData	= *reinterpret_cast<sDBO_DOGI_DATA*>( pDogiData );

	API_GetSLPacketLockManager()->Lock(GU_GUILD_DOGI_CHANGE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTutorialHintUpdateReq(DWORD dwHelpHint)
{
	sUG_TUTORIAL_HINT_UPDATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode			= UG_TUTORIAL_HINT_UPDATE_REQ;
	sPacket.dwTutorialHint	= dwHelpHint;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTutorialWaitCancelReq()
{
	sUG_TUTORIAL_WAIT_CANCEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode			= UG_TUTORIAL_WAIT_CANCEL_REQ;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTutorialPlayQuitReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_TUTORIAL_PLAY_QUIT_RES ) )
		return true;

	sUG_TUTORIAL_PLAY_QUIT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode			= UG_TUTORIAL_PLAY_QUIT_REQ;

	API_GetSLPacketLockManager()->Lock( GU_TUTORIAL_PLAY_QUIT_RES );
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTimeQuestEnterReq(HOBJECT hNpc, BYTE byDifficulty)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_TIMEQUEST_ENTER_RES))
		return true;

	sUG_TIMEQUEST_ENTER_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TIMEQUEST_ENTER_REQ;
	sPacket.hTimeQuestNpc = hNpc;
	sPacket.byDifficult = byDifficulty;

	API_GetSLPacketLockManager()->Lock(GU_TIMEQUEST_ENTER_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendRBInfoReq( RwUInt8 byBattleMode )
{
	if( API_GetSLPacketLockManager()->IsLock( GU_RANKBATTLE_INFO_RES ) )
		return true;

	sUG_RANKBATTLE_INFO_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_RANKBATTLE_INFO_REQ;
	sPacket.byBattleMode = byBattleMode;

	API_GetSLPacketLockManager()->Lock( GU_RANKBATTLE_INFO_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendRBJoinReq( RwUInt32 hObject, RwUInt32 tblidx )
{
	if( API_GetSLPacketLockManager()->IsLock( GU_RANKBATTLE_JOIN_RES ) )
		return true;

	sUG_RANKBATTLE_JOIN_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_RANKBATTLE_JOIN_REQ;
	sPacket.hBoardObject = hObject;
	sPacket.rankBattleTblidx = tblidx;

	API_GetSLPacketLockManager()->Lock( GU_RANKBATTLE_JOIN_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendRBLeaveReq( RwUInt32 tblidx )
{
	if( API_GetSLPacketLockManager()->IsLock( GU_RANKBATTLE_LEAVE_RES ) )
		return true;

	sUG_RANKBATTLE_LEAVE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_RANKBATTLE_LEAVE_REQ;
	sPacket.rankBattleTblidx = tblidx;

	API_GetSLPacketLockManager()->Lock( GU_RANKBATTLE_LEAVE_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

//bool CGamePacketGenerator::SendRBRoomListReq(RwUInt32 uiHandle, RwUInt16 wPage)
//{
//	if( API_GetSLPacketLockManager()->IsLock( GU_RANKBATTLE_ROOM_LIST_RES ) )
//		return true;
//
//	sUG_RANKBATTLE_ROOM_LIST_REQ sPacket;
//	memset(&sPacket, 0, sizeof(sPacket));
//
//	sPacket.wOpCode = UG_RANKBATTLE_ROOM_LIST_REQ;
//	sPacket.hRankBattleObject = uiHandle;
//	sPacket.wPage = wPage;
//
//	API_GetSLPacketLockManager()->Lock( GU_RANKBATTLE_ROOM_LIST_RES );
//
//	return m_pNetSender->SendPacket(sizeof(sPacket),&sPacket);
//}
//
//bool CGamePacketGenerator::SendRBRoomInfoReq(RwUInt32 uiHandle, RwUInt32 uiRoomID)
//{
//	if( API_GetSLPacketLockManager()->IsLock( GU_RANKBATTLE_ROOM_INFO_RES ) )
//		return true;
//
//	sUG_RANKBATTLE_ROOM_INFO_REQ sPacket;
//	memset(&sPacket, 0, sizeof(sPacket));
//
//	sPacket.wOpCode = UG_RANKBATTLE_ROOM_INFO_REQ;
//	sPacket.hRankBattleObject = uiHandle;
//	sPacket.roomId = uiRoomID;
//
//	API_GetSLPacketLockManager()->Lock( GU_RANKBATTLE_ROOM_INFO_RES );
//
//	return m_pNetSender->SendPacket(sizeof(sPacket),&sPacket);
//}
//
//bool CGamePacketGenerator::SendRBRoomCreateReq(RwUInt32 uiHandle)
//{
//	if( API_GetSLPacketLockManager()->IsLock( GU_RANKBATTLE_ROOM_CREATE_RES ) )
//		return true;
//
//	sUG_RANKBATTLE_ROOM_CREATE_REQ sPacket;
//	memset(&sPacket, 0, sizeof(sPacket));
//
//	sPacket.wOpCode = UG_RANKBATTLE_ROOM_CREATE_REQ;
//	sPacket.hRankBattleObject = uiHandle;
//
//	API_GetSLPacketLockManager()->Lock( GU_RANKBATTLE_ROOM_CREATE_RES );
//
//	return m_pNetSender->SendPacket(sizeof(sPacket),&sPacket);
//}
//
//bool CGamePacketGenerator::SendRBRoomLeaveReq(void)
//{
//	if( API_GetSLPacketLockManager()->IsLock( GU_RANKBATTLE_ROOM_LEAVE_RES ) )
//		return true;
//
//	sUG_RANKBATTLE_ROOM_LEAVE_REQ sPacket;
//	memset(&sPacket, 0, sizeof(sPacket));
//
//	sPacket.wOpCode = UG_RANKBATTLE_ROOM_LEAVE_REQ;	
//
//	API_GetSLPacketLockManager()->Lock( GU_RANKBATTLE_ROOM_LEAVE_RES );
//
//	return m_pNetSender->SendPacket(sizeof(sPacket),&sPacket);
//}
//
//bool CGamePacketGenerator::SendRBChallengeReq(RwUInt32 uiHandle, RwUInt32 uiRoomID)
//{
//	if( API_GetSLPacketLockManager()->IsLock( GU_RANKBATTLE_ROOM_CHALLENGE_RES ) )
//		return true;
//
//	sUG_RANKBATTLE_ROOM_CHALLENGE_REQ sPacket;
//	memset(&sPacket, 0, sizeof(sPacket));
//
//	sPacket.wOpCode = UG_RANKBATTLE_ROOM_CHALLENGE_REQ;
//	sPacket.hRankBattleObject = uiHandle;
//	sPacket.roomId = uiRoomID;
//
//	API_GetSLPacketLockManager()->Lock( GU_RANKBATTLE_ROOM_CHALLENGE_RES );
//
//	return m_pNetSender->SendPacket(sizeof(sPacket),&sPacket);
//}
//
//bool CGamePacketGenerator::SendRBMatchCancelReq(void)
//{
//	if( API_GetSLPacketLockManager()->IsLock( GU_RANKBATTLE_ROOM_MATCH_CANCEL_RES ) )
//		return true;
//
//	sUG_RANKBATTLE_ROOM_MATCH_CANCEL_REQ sPacket;
//	memset(&sPacket, 0, sizeof(sPacket));
//
//	sPacket.wOpCode = UG_RANKBATTLE_ROOM_MATCH_CANCEL_REQ;
//	
//	API_GetSLPacketLockManager()->Lock( GU_RANKBATTLE_ROOM_MATCH_CANCEL_RES );
//
//	return m_pNetSender->SendPacket(sizeof(sPacket),&sPacket);
//}

// PrivateShop
/*bool CGamePacketGenerator::SendPrivateShopCreateReq()
{
	if(API_GetSLPacketLockManager()->IsLock(GU_PRIVATESHOP_CREATE_RES))
		return false;

	sUG_PRIVATESHOP_CREATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));
	sPacket.wOpCode = UG_PRIVATESHOP_CREATE_REQ;

	API_GetSLPacketLockManager()->Lock(GU_PRIVATESHOP_CREATE_RES);
	
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}*/

bool CGamePacketGenerator::SendPrivateShopExitReq()
{
	if(API_GetSLPacketLockManager()->IsLock(GU_PRIVATESHOP_EXIT_RES))
		return false;

	sUG_PRIVATESHOP_EXIT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_PRIVATESHOP_EXIT_REQ;

	API_GetSLPacketLockManager()->Lock(GU_PRIVATESHOP_EXIT_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPrivateShopOpenReq(const WCHAR* pwcPrivateShopName, const WCHAR* pwcNotice, bool bIsOwnerEmpty)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_PRIVATESHOP_OPEN_RES))
		return false;

	sUG_PRIVATESHOP_OPEN_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode				= UG_PRIVATESHOP_OPEN_REQ;
	sPacket.bIsOwnerEmpty		= bIsOwnerEmpty;
	sPacket.byNoticeSize		= (BYTE)wcslen(pwcNotice) * sizeof(WCHAR);
	
	wcscpy_s(sPacket.wcPrivateShopName, NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1, pwcPrivateShopName);
	wcscpy_s(sPacket.wcNotice, NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1, pwcNotice);

	API_GetSLPacketLockManager()->Lock(GU_PRIVATESHOP_OPEN_RES);

	RwUInt32 uiTrashMemorySize = (NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1) * sizeof(WCHAR) - sPacket.byNoticeSize;
	return m_pNetSender->SendPacket(sizeof(sPacket) - uiTrashMemorySize, &sPacket);
}

bool CGamePacketGenerator::SendPrivateShopCloseReq()
{
	if(API_GetSLPacketLockManager()->IsLock(GU_PRIVATESHOP_CLOSE_RES))
		return false;

	sUG_PRIVATESHOP_CLOSE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_PRIVATESHOP_CLOSE_REQ;

	API_GetSLPacketLockManager()->Lock(GU_PRIVATESHOP_CLOSE_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPrivateShopEnterReq(RwUInt32 uiOwnerSerial)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_PRIVATESHOP_ENTER_RES))
		return false;

	sUG_PRIVATESHOP_ENTER_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode				= UG_PRIVATESHOP_ENTER_REQ;
	sPacket.hOwner				= uiOwnerSerial;

	API_GetSLPacketLockManager()->Lock(GU_PRIVATESHOP_ENTER_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPrivateShopLeaveReq(RwUInt32 uiOwnerSerial)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_PRIVATESHOP_LEAVE_RES))
		return false;

	sUG_PRIVATESHOP_LEAVE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode				= UG_PRIVATESHOP_LEAVE_REQ;
	sPacket.hOwner				= uiOwnerSerial;

	API_GetSLPacketLockManager()->Lock(GU_PRIVATESHOP_LEAVE_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPrivateShopItemInsertReq(RwUInt8 uiPrivateShopPos, RwUInt8 uiInventoryPlace, RwUInt8 uiInventoryPos)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_PRIVATESHOP_ITEM_INSERT_RES))
		return false;

	sUG_PRIVATESHOP_ITEM_INSERT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode				= UG_PRIVATESHOP_ITEM_INSERT_REQ;
	sPacket.byPrivateShopInventorySlotPos	= uiPrivateShopPos;
	sPacket.byInventoryPlace	= uiInventoryPlace; 
	sPacket.byInventoryPos		= uiInventoryPos;

	API_GetSLPacketLockManager()->Lock(GU_PRIVATESHOP_ITEM_INSERT_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPrivateShopItemUpdateReq(RwUInt8 uiPrivateShopPos, RwUInt32 uiZenney)
{
	sUG_PRIVATESHOP_ITEM_UPDATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode				= UG_PRIVATESHOP_ITEM_UPDATE_REQ;
	sPacket.byPrivateShopInventorySlotPos	= uiPrivateShopPos;
	sPacket.dwZenny				= uiZenney;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPrivateShopItemDeleteReq(RwUInt8 uiPrivateShopPos)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_PRIVATESHOP_ITEM_DELETE_RES))
		return false;

	sUG_PRIVATESHOP_ITEM_DELETE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode				= UG_PRIVATESHOP_ITEM_DELETE_REQ;
	sPacket.byPrivateShopInventorySlotPos	= uiPrivateShopPos;

	API_GetSLPacketLockManager()->Lock(GU_PRIVATESHOP_ITEM_DELETE_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPrivateShopItemSelectReq(RwUInt32 uiOwnerSerial, RwUInt8 uiPrivateShopPos, bool bSelect)
{
	sUG_PRIVATESHOP_ITEM_SELECT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));


    sPacket.wOpCode             = UG_PRIVATESHOP_ITEM_SELECT_REQ;
	sPacket.hOwner				= uiOwnerSerial;
	sPacket.byPrivateShopInventorySlotPos	= uiPrivateShopPos;
	sPacket.byItemState			= (BYTE)(bSelect ? sPRIVATESHOP_ITEM_DATA::PRIVATESHOP_VISITOR_SELECT_ITEM :
											 sPRIVATESHOP_ITEM_DATA::PRIVATESHOP_VISITOR_UNSELECT_ITEM);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPrivateShopItemBuyingReq(RwUInt32 uiOwnerSerial, RwUInt8 auiPrivateShopPos[NTL_MAX_BUY_SHOPPING_CART])
{
	if(API_GetSLPacketLockManager()->IsLock(GU_PRIVATESHOP_ITEM_BUYING_RES))
		return false;

	sUG_PRIVATESHOP_ITEM_BUYING_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UG_PRIVATESHOP_ITEM_BUYING_REQ;
	sPacket.hOwner = uiOwnerSerial;
	memcpy(sPacket.byPrivateShopInventorySlotPos, auiPrivateShopPos, sizeof(RwUInt8) * NTL_MAX_BUY_SHOPPING_CART);

	API_GetSLPacketLockManager()->Lock(GU_PRIVATESHOP_ITEM_BUYING_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendDBCCheckReq( RwUInt32 uiAltarHandle, sITEM_POSITION_DATA sData[NTL_ITEM_MAX_DRAGONBALL] ) 
{
	if(API_GetSLPacketLockManager()->IsLock(GU_DRAGONBALL_CHECK_RES))
		return false;

	sUG_DRAGONBALL_CHECK_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_DRAGONBALL_CHECK_REQ;
	sPacket.hObject = uiAltarHandle;
	for(int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; ++i)
	{
		sPacket.sData[i] = sData[i];
	}

	API_GetSLPacketLockManager()->Lock(GU_DRAGONBALL_CHECK_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendDBCRewardReq( RwUInt32 uiAltarHandle, TBLIDX tblIdx ) 
{
	if(API_GetSLPacketLockManager()->IsLock(GU_DRAGONBALL_REWARD_RES))
		return false;

	sUG_DRAGONBALL_REWARD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_DRAGONBALL_REWARD_REQ;
	sPacket.hObject = uiAltarHandle;
	sPacket.rewardTblidx = tblIdx;

	API_GetSLPacketLockManager()->Lock(GU_DRAGONBALL_REWARD_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

// MailSystem
bool CGamePacketGenerator::SendMailStartReq(RwUInt32 hSerialID)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_MAIL_START_RES))
		return false;

	sUG_MAIL_START_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_MAIL_START_REQ;
	sPacket.hObject	= hSerialID;

	API_GetSLPacketLockManager()->Lock(GU_MAIL_START_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendMailSendReq(RwUInt32 hSerialID, const WCHAR* pwcTargetName, const WCHAR* pwcText, RwUInt8 uiMailType, sITEM_POSITION_DATA* pItemData, RwUInt32 uiZenny, RwUInt8 uiDay)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_MAIL_SEND_RES))
		return false;

	sUG_MAIL_SEND_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode		= UG_MAIL_SEND_REQ;
	sPacket.hObject		= hSerialID;
	sPacket.byMailType	= uiMailType;
	sPacket.dwZenny		= uiZenny;
	sPacket.byDay		= uiDay;
	sPacket.byTextSize	= (BYTE)wcslen(pwcText);
	
	wcscpy_s(sPacket.wszTargetName, NTL_MAX_SIZE_CHAR_NAME + 1, pwcTargetName);
	wcscpy_s(sPacket.wszText, NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1, pwcText);

	if (pItemData)
	{
		memcpy(&sPacket.sItemData, pItemData, sizeof(sITEM_POSITION_DATA));
	}

	API_GetSLPacketLockManager()->Lock(GU_MAIL_SEND_RES);

	RwUInt32 uiTrashMemorySize = ((NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1) - sPacket.byTextSize) * sizeof(WCHAR);
	return m_pNetSender->SendPacket(sizeof(sPacket) - uiTrashMemorySize, &sPacket);
}

bool CGamePacketGenerator::SendMailReadReq(RwUInt32 hSerialID, MAILID mailID)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_MAIL_READ_RES))
		return false;

	sUG_MAIL_READ_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_MAIL_READ_REQ;
	sPacket.hObject	= hSerialID;
	sPacket.mailID	= mailID;

	API_GetSLPacketLockManager()->Lock(GU_MAIL_READ_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendMailDelReq(RwUInt32 hSerialID, MAILID mailID)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_MAIL_DEL_RES))
		return false;

	sUG_MAIL_DEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_MAIL_DEL_REQ;
	sPacket.hObject	= hSerialID;
	sPacket.mailID	= mailID;

	API_GetSLPacketLockManager()->Lock(GU_MAIL_DEL_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendMailReturnReq(RwUInt32 hSerialID, MAILID mailID)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_MAIL_RETURN_RES))
		return false;

	sUG_MAIL_RETURN_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_MAIL_RETURN_REQ;
	sPacket.hObject	= hSerialID;
	sPacket.mailID	= mailID;

	API_GetSLPacketLockManager()->Lock(GU_MAIL_RETURN_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendMailReloadReq(RwUInt32 hSerialID)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_MAIL_RELOAD_RES))
		return false;

	sUG_MAIL_RELOAD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_MAIL_RELOAD_REQ;
	sPacket.hObject	= hSerialID;

	API_GetSLPacketLockManager()->Lock(GU_MAIL_RELOAD_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

/*bool CGamePacketGenerator::SendMailLoadReq(RwUInt32 hSerialID, MAILID mailID)
{
	sUG_MAIL_LOAD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_MAIL_LOAD_REQ;
	sPacket.hObject	= hSerialID;
	sPacket.mailID	= mailID;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}*/

bool CGamePacketGenerator::SendMailItemReceiveReq(RwUInt32 hSerialID, MAILID mailID)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_MAIL_ITEM_RECEIVE_RES))
		return false;

	sUG_MAIL_ITEM_RECEIVE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_MAIL_ITEM_RECEIVE_REQ;
	sPacket.hObject	= hSerialID;
	sPacket.mailID	= mailID;

	API_GetSLPacketLockManager()->Lock(GU_MAIL_ITEM_RECEIVE_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendMailLockReq(RwUInt32 hSerialID, MAILID mailID, bool bIsLock)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_MAIL_LOCK_RES))
		return false;

	sUG_MAIL_LOCK_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_MAIL_LOCK_REQ;
	sPacket.hObject	= hSerialID;
	sPacket.mailID	= mailID;
	sPacket.bIsLock = bIsLock;

	API_GetSLPacketLockManager()->Lock(GU_MAIL_LOCK_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendCharAwayReq(bool bIsAway)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_CHAR_AWAY_RES))
		return false;

	sUG_CHAR_AWAY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_CHAR_AWAY_REQ;
	sPacket.bIsAway	= bIsAway;

	API_GetSLPacketLockManager()->Lock(GU_CHAR_AWAY_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendMailMultiDelReq( RwUInt32 hObject, RwUInt8 byCount, MAILID* pMailID )
{
	if( API_GetSLPacketLockManager()->IsLock( GU_MAIL_MULTI_DEL_RES ) )
		return false;

	sUG_MAIL_MULTI_DEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_MAIL_MULTI_DEL_REQ;
	sPacket.hObject = hObject;
	sPacket.byCount = byCount;
	memcpy( sPacket.aMailID, pMailID, sizeof(MAILID) * byCount );

	API_GetSLPacketLockManager()->Lock( GU_MAIL_MULTI_DEL_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendAuctionHouseListReq(CHARACTERID charId, const WCHAR* awchItemName, BYTE byClassType, BYTE byTabType, BYTE byItemType, BYTE byMinLevel, BYTE byMaxLevel, BYTE byRank, RwUInt32 uiPage, BYTE bySortType, BYTE byPage)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_TENKAICHIDAISIJYOU_LIST_RES))
		return false;

	sUG_TENKAICHIDAISIJYOU_LIST_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TENKAICHIDAISIJYOU_LIST_REQ;
	sPacket.charId = charId;
	wcscpy_s(sPacket.awchItemName, DBO_MAX_LENGTH_ITEM_NAME_TEXT + 1, awchItemName);
	sPacket.byClassType = byClassType;
	sPacket.byTabType = byTabType;
	sPacket.byItemType = byItemType;
	sPacket.byMinLevel = byMinLevel;
	sPacket.byMaxLevel = byMaxLevel;
	sPacket.byRank = byRank;
	sPacket.uiPage = uiPage;
	sPacket.bySortType = bySortType;
	sPacket.byPage = byPage;

	API_GetSLPacketLockManager()->Lock(GU_TENKAICHIDAISIJYOU_LIST_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendAuctionHouseSellReq(DWORD dwPrice, BYTE byPlace, BYTE byPosition, BYTE byCount, BYTE byTime)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_TENKAICHIDAISIJYOU_SELL_RES))
		return false;

	sUG_TENKAICHIDAISIJYOU_SELL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TENKAICHIDAISIJYOU_SELL_REQ;
	sPacket.dwPrice = dwPrice;
	sPacket.byPlace = byPlace;
	sPacket.byPosition = byPosition;
	sPacket.byCount = byCount;
	sPacket.byTime = byTime;

	API_GetSLPacketLockManager()->Lock(GU_TENKAICHIDAISIJYOU_SELL_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendAuctionHouseSellCancelReq(ITEMID nItem)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_TENKAICHIDAISIJYOU_SELL_CANCEL_RES))
		return false;

	sUG_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ;
	sPacket.nItem = nItem;

	API_GetSLPacketLockManager()->Lock(GU_TENKAICHIDAISIJYOU_SELL_CANCEL_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendAuctionHouseBuyReq(ITEMID nItem)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_TENKAICHIDAISIJYOU_BUY_RES))
		return false;

	sUG_TENKAICHIDAISIJYOU_BUY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_TENKAICHIDAISIJYOU_BUY_REQ;
	sPacket.nItem = nItem;

	API_GetSLPacketLockManager()->Lock(GU_TENKAICHIDAISIJYOU_BUY_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPortalStartReq( RwUInt32 hSerialID, RwBool* bPacketLock ) 
{
	/*if( !bPacketLock )
		return true;

	*bPacketLock = API_GetSLPacketLockManager()->IsLock( GU_PORTAL_START_RES );

	if( *bPacketLock )
		return true;


    sUG_PORTAL_START_REQ sPacket;    
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UG_PORTAL_START_REQ;
   sPacket.handle = hSerialID;

    API_GetSLPacketLockManager()->Lock(GU_PORTAL_START_RES);

    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);*/

	return true;
}

bool CGamePacketGenerator::SendPortalAddReq( RwUInt32 hSerialID ) 
{
    if(API_GetSLPacketLockManager()->IsLock(GU_PORTAL_ADD_RES))
        return false;

    sUG_PORTAL_ADD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UG_PORTAL_ADD_REQ;
	sPacket.handle = hSerialID;    

    API_GetSLPacketLockManager()->Lock(GU_PORTAL_ADD_RES);

    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPortalReq( RwUInt32 hSerialID, BYTE byPoint ) 
{
    if(API_GetSLPacketLockManager()->IsLock(GU_PORTAL_RES))
        return false;

    sUG_PORTAL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UG_PORTAL_REQ;
    sPacket.handle = hSerialID;
    sPacket.byPoint = byPoint;

    API_GetSLPacketLockManager()->Lock(GU_PORTAL_RES);

    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendWarFogUpdateReq(RwUInt32 uiTriggerObjectIndex)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_WAR_FOG_UPDATE_RES))
		return false;

	sUG_WAR_FOG_UPDATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UG_WAR_FOG_UPDATE_REQ;
	sPacket.hObject = uiTriggerObjectIndex;

	API_GetSLPacketLockManager()->Lock(GU_WAR_FOG_UPDATE_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBlockModeReq(bool bIsOn)
{
	sUG_CHAR_BLOCK_MODE sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UG_CHAR_BLOCK_MODE;
	sPacket.bIsOn = bIsOn;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendCharKnockDownReleaseNfy() 
{
	sUG_CHAR_KNOCKDOWN_RELEASE_NFY sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CHAR_KNOCKDOWN_RELEASE_NFY;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyShareTargetReq( RwUInt8 bySlot, RwUInt32 hTarget ) 
{
    sUG_PARTY_SHARETARGET_REQ sPacket;    
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UG_PARTY_SHARETARGET_REQ;
    sPacket.bySlot = bySlot;
    sPacket.hTarget = hTarget;

    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyDungeonChangeReq(RwUInt32 uiPartyDungeonState)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_PARTY_DUNGEON_DIFF_RES ) )
		return false;

	sUG_PARTY_DIFF_CHANGE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PARTY_DIFF_CHANGE_REQ;
	sPacket.eDiff	= (ePARTY_DUNGEON_STATE)uiPartyDungeonState;

	API_GetSLPacketLockManager()->Lock( GU_PARTY_DUNGEON_DIFF_RES );
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendPartyDungeonInitReq(void)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_PARTY_DUNGEON_INIT_RES ) )
		return false;

	sUG_PARTY_DUNGEON_INIT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PARTY_DUNGEON_INIT_REQ;

	API_GetSLPacketLockManager()->Lock( GU_PARTY_DUNGEON_INIT_RES );
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTransformCancelReq() 
{
    sUG_TRANSFORM_CANCEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UG_TRANSFORM_CANCEL_REQ;

    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBudokaiJoinIndividualReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_JOIN_INDIVIDUAL_RES ) )
		return false;

	sUG_BUDOKAI_JOIN_INDIVIDUAL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BUDOKAI_JOIN_INDIVIDUAL_REQ;

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_JOIN_INDIVIDUAL_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBudokaiLeaveIndividualReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_LEAVE_INDIVIDUAL_RES ) )
		return false;

	sUG_BUDOKAI_LEAVE_INDIVIDUAL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BUDOKAI_LEAVE_INDIVIDUAL_REQ;

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_LEAVE_INDIVIDUAL_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBudokaiJoinTeamInfoReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_JOIN_TEAM_INFO_RES ) )
		return false;

	sUG_BUDOKAI_JOIN_TEAM_INFO_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BUDOKAI_JOIN_TEAM_INFO_REQ;

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_JOIN_TEAM_INFO_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBudokaiJoinTeamReq(WCHAR* pwcTeamName)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_JOIN_TEAM_RES ) )
		return false;

	sUG_BUDOKAI_JOIN_TEAM_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BUDOKAI_JOIN_TEAM_REQ;
	::CopyMemory(&sPacket.wszTeamName, pwcTeamName, sizeof(WCHAR) * (NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE+1) );

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_JOIN_TEAM_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBudokaiLeaveTeamReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_LEAVE_TEAM_RES ) )
		return false;

	sUG_BUDOKAI_LEAVE_TEAM_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BUDOKAI_LEAVE_TEAM_REQ;

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_LEAVE_TEAM_RES );
	
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBudokaiLeaveTeamMemberReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_LEAVE_TEAM_RES ) )
		return false;

	sUG_BUDOKAI_LEAVE_TEAM_MEMBER_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BUDOKAI_LEAVE_TEAM_MEMBER_REQ;

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_LEAVE_TEAM_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBudokaiJoinInfoReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_JOIN_INFO_RES ) )
		return false;

	sUG_BUDOKAI_JOIN_INFO_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_BUDOKAI_JOIN_INFO_REQ;

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_JOIN_INFO_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBudokaiMudosaInfoReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_MUDOSA_INFO_RES ) )
		return false;

	sUG_BUDOKAI_MUDOSA_INFO_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_BUDOKAI_MUDOSA_INFO_REQ;

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_MUDOSA_INFO_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBudokaiPrizeWinnerNameReq()
{
	if( API_GetSLPacketLockManager()->IsIndividualLock( GU_BUDOKAI_PRIZEWINNER_NAME_RES ) )
		return false;

	sUG_BUDOKAI_PRIZEWINNER_NAME_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket) );

	sPacket.wOpCode = UG_BUDOKAI_PRIZEWINNER_NAME_REQ;

	API_GetSLPacketLockManager()->IndividualLock( GU_BUDOKAI_PRIZEWINNER_NAME_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendBudokaiPrizeTeamWinnerNameReq()
{
	if( API_GetSLPacketLockManager()->IsIndividualLock( GU_BUDOKAI_PRIZEWINNER_TEAM_NAME_RES ) )
		return false;

	sUG_BUDOKAI_PRIZEWINNER_TEAM_NAME_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket) );

	sPacket.wOpCode = UG_BUDOKAI_PRIZEWINNER_TEAM_NAME_REQ;

	API_GetSLPacketLockManager()->IndividualLock( GU_BUDOKAI_PRIZEWINNER_TEAM_NAME_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendRoolReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_DICE_ROLL_RES ) )
		return false;

	sUG_DICE_ROLL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode	= UG_DICE_ROLL_REQ;

	API_GetSLPacketLockManager()->Lock( GU_DICE_ROLL_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendItemDiceReq(HOBJECT hItemHandle, BYTE byDice)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_ITEM_DICE_RES))
		return false;

	sUG_ITEM_DICE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_ITEM_DICE_REQ;
	sPacket.byDice = byDice;
	sPacket.hItemHandle = hItemHandle;

	API_GetSLPacketLockManager()->Lock(GU_ITEM_DICE_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendShopGambleBuyReq( RwUInt32 hSerialID )
{
	if( API_GetSLPacketLockManager()->IsLock( GU_SHOP_GAMBLE_BUY_RES ) )
		return false;

	sUG_SHOP_GAMBLE_BUY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SHOP_GAMBLE_BUY_REQ;
	sPacket.handle = hSerialID;

	API_GetSLPacketLockManager()->Lock( GU_SHOP_GAMBLE_BUY_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendTeleportConfirmReq( bool bTeleport, RwUInt8 byTeleportIndex )
{
	if( API_GetSLPacketLockManager()->IsLock( GU_TELEPORT_CONFIRM_RES ) )
		return false;

	sUG_TELEPORT_CONFIRM_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UG_TELEPORT_CONFIRM_REQ;
	sPacket.bTeleport = bTeleport;
	sPacket.byTeleportIndex = byTeleportIndex;

	API_GetSLPacketLockManager()->Lock( GU_TELEPORT_CONFIRM_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendSkillInitReq( RwUInt32 hSerialID )
{
	if( API_GetSLPacketLockManager()->IsLock( GU_SKILL_INIT_RES ) )
		return false;

	sUG_SKILL_INIT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UG_SKILL_INIT_REQ;
	sPacket.hNpc = hSerialID;

	API_GetSLPacketLockManager()->Lock( GU_SKILL_INIT_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendSkillResetPlusReq(BYTE byPlace, BYTE byPos)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_SKILL_RESET_PLUS_RES))
		return false;

	sUG_SKILL_RESET_PLUS_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SKILL_RESET_PLUS_REQ;
	sPacket.byPlace = byPlace;
	sPacket.byPos = byPos;

	API_GetSLPacketLockManager()->Lock(GU_SKILL_RESET_PLUS_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendGMBudokaiServerEnterReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES ) )
		return false;

	sUG_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_REQ;

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendGMBudokaiServerLeaveReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_GM_BUDOKAI_SERVER_LEAVE_RES ) )
		return false;

	sUG_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BUDOKAI_GM_BUDOKAI_SERVER_LEAVE_REQ;

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_GM_BUDOKAI_SERVER_LEAVE_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendGMBudokaiMatchProgressInfoReq(RwUInt8 byMatchType)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_GM_MATCH_PROGRESS_INFO_RES ) )
		return false;

	sUG_BUDOKAI_GM_MATCH_PROGRESS_INFO_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BUDOKAI_GM_MATCH_PROGRESS_INFO_REQ;
	sPacket.byMatchType = byMatchType;

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_GM_MATCH_PROGRESS_INFO_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendGMBudokaiMatchArenaEnterReq(RwUInt8 byMatchType, RwUInt8 byMatchWorldType,
														   RwUInt8 byMatchDepth, RwUInt8 byMatchIndex)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_GM_MATCH_ARENA_ENTER_RES ) )
		return false;

	sUG_BUDOKAI_GM_MATCH_ARENA_ENTER_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BUDOKAI_GM_MATCH_ARENA_ENTER_REQ;
	sPacket.byMatchType			= byMatchType;
	sPacket.byMatchWorldType	= byMatchWorldType;
	sPacket.byMatchDepth		= byMatchDepth;
	sPacket.byMatchIndex		= byMatchIndex;

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_GM_MATCH_ARENA_ENTER_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendGMBudokaiMatchArenaLeaveReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_GM_MATCH_ARENA_LEAVE_RES ) )
		return false;

	sUG_BUDOKAI_GM_MATCH_ARENA_LEAVE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_BUDOKAI_GM_MATCH_ARENA_LEAVE_REQ;

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_GM_MATCH_ARENA_LEAVE_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendHoipoiMixItemCreateReq(HOBJECT hObject, TBLIDX idxRecipe, BYTE byMaterialCount, void* aMaterialSlot)
{
	if( API_GetSLPacketLockManager()->IsLock(GU_HOIPOIMIX_ITEM_CREATE_RES) )
		return false;

	sUG_HOIPOIMIX_ITEM_CREATE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UG_HOIPOIMIX_ITEM_CREATE_REQ;
	sPacket.objHandle = hObject;
	sPacket.recipeTblidx = idxRecipe;
	sPacket.byMaterialCount = byMaterialCount;
	memcpy(sPacket.aMaterialSlot, aMaterialSlot, sizeof(sINVEN_SLOT) * byMaterialCount);

	API_GetSLPacketLockManager()->Lock( GU_HOIPOIMIX_ITEM_CREATE_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendBusWorldMapStatus(bool bIsWorldMapOpen)
{
	sUG_WORLD_MAP_STATUS sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode				= UG_WORLD_MAP_STATUS;
	sPacket.bIsWorldMapOpen		= bIsWorldMapOpen;

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendShopNetPyItemStartReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_SHOP_NETPYITEM_START_RES ) )
		return false;

	sUG_SHOP_NETPYITEM_START_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SHOP_NETPYITEM_START_REQ;

	API_GetSLPacketLockManager()->Lock( GU_SHOP_NETPYITEM_START_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendShopNetPyItemBuyReq( RwUInt8 byBuyCount, sSHOP_BUY_CART* pBuyData )
{
	if( API_GetSLPacketLockManager()->IsLock( GU_SHOP_NETPYITEM_BUY_RES ) )
		return false;

	sUG_SHOP_NETPYITEM_BUY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SHOP_NETPYITEM_BUY_REQ;
	sPacket.byBuyCount = byBuyCount;
	memcpy( sPacket.sBuyData, pBuyData, sizeof( sSHOP_BUY_CART ) * byBuyCount );

	API_GetSLPacketLockManager()->Lock( GU_SHOP_NETPYITEM_BUY_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendShopNetPyItemEndReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_SHOP_NETPYITEM_END_RES ) )
		return false;

	sUG_SHOP_NETPYITEM_END_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_SHOP_NETPYITEM_END_REQ;

	API_GetSLPacketLockManager()->Lock( GU_SHOP_NETPYITEM_END_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendDurationItemBuyReq( RwUInt32 uiMerchantTblidx, BYTE bySlotPos )
{
	if( API_GetSLPacketLockManager()->IsLock( GU_DURATION_ITEM_BUY_RES ) )
		return false;

	sUG_DURATION_ITEM_BUY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_DURATION_ITEM_BUY_REQ;
	sPacket.merchantTblidx = uiMerchantTblidx;
	sPacket.byPos = bySlotPos;

	API_GetSLPacketLockManager()->Lock( GU_DURATION_ITEM_BUY_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendDurationRenewReq( HOBJECT hItemHandle, TBLIDX itemIdx)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_DURATION_RENEW_RES ) )
		return false;

	sUG_DURATION_RENEW_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_DURATION_RENEW_REQ;
	sPacket.hItemHandle = hItemHandle;
	sPacket.itemTblidx = itemIdx;

	API_GetSLPacketLockManager()->Lock( GU_DURATION_RENEW_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendCashItemHLShopStartReq()
{
	if (API_GetSLPacketLockManager()->IsLock(GU_CASHITEM_HLSHOP_START_RES))
		return false;

	sUG_CASHITEM_HLSHOP_START_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CASHITEM_HLSHOP_START_REQ;

	API_GetSLPacketLockManager()->Lock(GU_CASHITEM_HLSHOP_START_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendCashItemHLShopEndReq()
{
	if (API_GetSLPacketLockManager()->IsLock(GU_CASHITEM_HLSHOP_END_RES))
		return false;

	sUG_CASHITEM_HLSHOP_END_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CASHITEM_HLSHOP_END_REQ;

	API_GetSLPacketLockManager()->Lock(GU_CASHITEM_HLSHOP_END_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendCashItemHLShopRefreshReq()
{
	if (API_GetSLPacketLockManager()->IsLock(GU_CASHITEM_HLSHOP_REFRESH_RES))
		return false;

	sUG_CASHITEM_HLSHOP_REFRESH_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CASHITEM_HLSHOP_REFRESH_REQ;

	API_GetSLPacketLockManager()->Lock(GU_CASHITEM_HLSHOP_REFRESH_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendCashItemHLShopBuyReq(TBLIDX uiHlsItemTblidx)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_CASHITEM_BUY_RES))
		return false;

	sUG_CASHITEM_BUY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CASHITEM_BUY_REQ;
	sPacket.dwIdxHlsTable = uiHlsItemTblidx;

	API_GetSLPacketLockManager()->Lock(GU_CASHITEM_BUY_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendCashItemMoveReq( RwUInt64 uiProductId )
{
	if( API_GetSLPacketLockManager()->IsLock( GU_CASHITEM_MOVE_RES ) )
		return false;

	sUG_CASHITEM_MOVE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CASHITEM_MOVE_REQ;
	sPacket.qwProductId = uiProductId;

	API_GetSLPacketLockManager()->Lock( GU_CASHITEM_MOVE_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CGamePacketGenerator::SendCashItemUseReq(RwUInt64 uiProductId)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_CASHITEM_USE_RES))
		return false;

	sUG_CASHITEM_USE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CASHITEM_USE_REQ;
	sPacket.qwProductId = uiProductId;

	API_GetSLPacketLockManager()->Lock(GU_CASHITEM_USE_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendCashItemHLShopGiftReq(TBLIDX uiHlsItemTblidx, const WCHAR* wchTargetName)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_CASHITEM_SEND_GIFT_RES))
		return false;

	sUG_CASHITEM_SEND_GIFT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CASHITEM_SEND_GIFT_REQ;
	sPacket.dwIdxHlsTable = uiHlsItemTblidx;
	NTL_SAFE_WCSCPY(sPacket.wchName, wchTargetName);

	API_GetSLPacketLockManager()->Lock(GU_CASHITEM_SEND_GIFT_RES);
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendCharTitleSelectReq(TBLIDX tblIndex)
{
	if (API_GetSLPacketLockManager()->IsLock(GU_CHARTITLE_SELECT_RES))
		return false;

	sUG_CHARTITLE_SELECT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CHARTITLE_SELECT_REQ;
	sPacket.tblIndex = tblIndex;

	API_GetSLPacketLockManager()->Lock(GU_CHARTITLE_SELECT_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CGamePacketGenerator::SendClientInfoNfy(bool bSpeedHack)
{
	sUG_CLIENT_INFO_NFY sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CLIENT_INFO_NFY;
	sPacket.fAppTime = CNtlTimer::GetAppTime();
	sPacket.bSpeedHack = bSpeedHack;
	sPacket.dwCameraMoveCount = sm_cameraMoveCount;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


CChatPacketGenerator::CChatPacketGenerator(CNtlNetSender *pNetSender)
:CNtlPacketGenerator(pNetSender)
{
}

bool CChatPacketGenerator::SendChatEnterReq(void)
{
	sUT_ENTER_CHAT sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	SConnectData *pConnectData = GetDboGlobal()->GetConnectData();

	sPacket.wOpCode = UT_ENTER_CHAT;
	sPacket.accountId = pConnectData->uiAccountId;
	memcpy(sPacket.abyAuthKey, pConnectData->sChatCon.chAuthKey, NTL_MAX_SIZE_AUTH_KEY);

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket ); 
}

bool CChatPacketGenerator::SendChatMsgSay(const WCHAR *pMsg)
{
	sUT_CHAT_MESSAGE_SAY sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );
	
	sPacket.wOpCode = UT_CHAT_MESSAGE_SAY;
	sPacket.wMessageLengthInUnicode = (WORD)wcslen(pMsg);

	if( sPacket.wMessageLengthInUnicode > NTL_MAX_LENGTH_OF_CHAT_MESSAGE )
		sPacket.wMessageLengthInUnicode = NTL_MAX_LENGTH_OF_CHAT_MESSAGE;

	wcsncpy_s(sPacket.awchMessage, NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1, pMsg, sPacket.wMessageLengthInUnicode);

	RwInt32 iLen = sizeof( sPacket ) - sizeof(WCHAR) * (NTL_MAX_LENGTH_OF_CHAT_MESSAGE - sPacket.wMessageLengthInUnicode + 1);
	
	return m_pNetSender->SendPacket( iLen, &sPacket ); 
}

bool CChatPacketGenerator::SendChatMsgShout( const WCHAR *pMsg )
{
	sUT_CHAT_MESSAGE_SHOUT sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UT_CHAT_MESSAGE_SHOUT;
	sPacket.wMessageLengthInUnicode = (WORD)wcslen( pMsg );

	if( sPacket.wMessageLengthInUnicode > NTL_MAX_LENGTH_OF_CHAT_MESSAGE )
		sPacket.wMessageLengthInUnicode = NTL_MAX_LENGTH_OF_CHAT_MESSAGE;

	wcsncpy_s( sPacket.awchMessage, NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1, pMsg, sPacket.wMessageLengthInUnicode );

	RwInt32 iLen = sizeof( sPacket ) - sizeof( WCHAR ) * ( NTL_MAX_LENGTH_OF_CHAT_MESSAGE - sPacket.wMessageLengthInUnicode + 1);

	return m_pNetSender->SendPacket( iLen, &sPacket );
}

bool CChatPacketGenerator::SendChatMsgWhisper( const WCHAR *pMsg, const WCHAR *pReceiverName )
{
	sUT_CHAT_MESSAGE_WHISPER sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UT_CHAT_MESSAGE_WHISPER;
	sPacket.wMessageLengthInUnicode = (WORD)wcslen( pMsg );

	if( sPacket.wMessageLengthInUnicode > NTL_MAX_LENGTH_OF_CHAT_MESSAGE )
		sPacket.wMessageLengthInUnicode = NTL_MAX_LENGTH_OF_CHAT_MESSAGE;

	wcsncpy_s( sPacket.awchMessage, NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1, pMsg, sPacket.wMessageLengthInUnicode );
	wcscpy_s( sPacket.awchReceiverCharName, NTL_MAX_SIZE_CHAR_NAME + 1, pReceiverName );

	RwInt32 iLen = sizeof( sPacket ) - sizeof( WCHAR ) * ( NTL_MAX_LENGTH_OF_CHAT_MESSAGE - sPacket.wMessageLengthInUnicode + 1);

	return m_pNetSender->SendPacket( iLen, &sPacket );
}

bool CChatPacketGenerator::SendChatMsgParty( RwUInt8 byChatType, const WCHAR *pMsg )
{
	sUT_CHAT_MESSAGE_PARTY sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UT_CHAT_MESSAGE_PARTY;
	sPacket.byChattingType			= byChatType;
	sPacket.wMessageLengthInUnicode = (WORD)wcslen( pMsg );
	
	if( sPacket.wMessageLengthInUnicode > NTL_MAX_LENGTH_OF_CHAT_MESSAGE )
		sPacket.wMessageLengthInUnicode = NTL_MAX_LENGTH_OF_CHAT_MESSAGE;

	wcsncpy_s( sPacket.awchMessage, NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1, pMsg, sPacket.wMessageLengthInUnicode );

	RwInt32 iLen = sizeof( sPacket ) - sizeof( WCHAR ) * ( NTL_MAX_LENGTH_OF_CHAT_MESSAGE - sPacket.wMessageLengthInUnicode + 1);

	return m_pNetSender->SendPacket( iLen, &sPacket );
}

bool CChatPacketGenerator::SendChatMsgGuild(RwUInt8 byChatType, const WCHAR *pMsg)
{
	sUT_CHAT_MESSAGE_GUILD sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode					= UT_CHAT_MESSAGE_GUILD;
	sPacket.byChattingType			= byChatType;
	sPacket.wMessageLengthInUnicode	= (WORD)wcslen( pMsg );

	if( sPacket.wMessageLengthInUnicode > NTL_MAX_LENGTH_OF_CHAT_MESSAGE )
		sPacket.wMessageLengthInUnicode = NTL_MAX_LENGTH_OF_CHAT_MESSAGE;

	wcsncpy_s( sPacket.awchMessage, NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1, pMsg, sPacket.wMessageLengthInUnicode );

	RwInt32 iLen = sizeof( sPacket ) - sizeof( WCHAR ) * ( NTL_MAX_LENGTH_OF_CHAT_MESSAGE - sPacket.wMessageLengthInUnicode + 1);

	return m_pNetSender->SendPacket( iLen, &sPacket );
}

bool CChatPacketGenerator::SendChatMsgTrade(const WCHAR * pMsg)
{
	sUT_CHAT_MESSAGE_TRADE sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UT_CHAT_MESSAGE_TRADE;
	sPacket.wMessageLengthInUnicode = (WORD)wcslen(pMsg);

	if (sPacket.wMessageLengthInUnicode > NTL_MAX_LENGTH_OF_CHAT_MESSAGE)
		sPacket.wMessageLengthInUnicode = NTL_MAX_LENGTH_OF_CHAT_MESSAGE;

	wcsncpy_s(sPacket.awchMessage, NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1, pMsg, sPacket.wMessageLengthInUnicode);

	RwInt32 iLen = sizeof(sPacket) - sizeof(WCHAR) * (NTL_MAX_LENGTH_OF_CHAT_MESSAGE - sPacket.wMessageLengthInUnicode + 1);

	return m_pNetSender->SendPacket(iLen, &sPacket);
}

bool CChatPacketGenerator::SendChatMsgFindParty(const WCHAR * pMsg)
{
	sUT_CHAT_MESSAGE_FIND_PARTY sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UT_CHAT_MESSAGE_FIND_PARTY;
	sPacket.wMessageLengthInUnicode = (WORD)wcslen(pMsg);

	if (sPacket.wMessageLengthInUnicode > NTL_MAX_LENGTH_OF_CHAT_MESSAGE)
		sPacket.wMessageLengthInUnicode = NTL_MAX_LENGTH_OF_CHAT_MESSAGE;

	wcsncpy_s(sPacket.awchMessage, NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1, pMsg, sPacket.wMessageLengthInUnicode);

	RwInt32 iLen = sizeof(sPacket) - sizeof(WCHAR) * (NTL_MAX_LENGTH_OF_CHAT_MESSAGE - sPacket.wMessageLengthInUnicode + 1);

	return m_pNetSender->SendPacket(iLen, &sPacket);
}

bool CChatPacketGenerator::SendPetitionUserInsertReq(const WCHAR* pwcCharName, const WCHAR* pwcPetition, RwInt32 iCategory, RwInt32 iCategory2)
{
	if( API_GetSLPacketLockManager()->IsLock( TU_PETITION_USER_INSERT_RES ) )
		return false;

	if( !pwcCharName || !pwcPetition )
		return FALSE;

	if( wcslen(pwcCharName) == 0 || wcslen(pwcPetition) == 0 )
		return FALSE;

	sUT_PETITION_USER_INSERT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode			= UT_PETITION_USER_INSERT_REQ;
	wcscpy_s( sPacket.awchUserCharName, NTL_MAX_SIZE_CHAR_NAME + 1, pwcCharName );
	wcscpy_s( sPacket.awchQuestionContent, NTL_MAX_SIZE_QUESTION_CONTENT + 1, pwcPetition );
	sPacket.nCategory1		= iCategory;
	sPacket.nCategory2		= iCategory2;

	API_GetSLPacketLockManager()->Lock( TU_PETITION_USER_INSERT_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CChatPacketGenerator::SendPetitionModifyReq(RwUInt32 uiPetitionID, const WCHAR* pwcPetition, RwInt32 iCategory, RwInt32 iCategory2)
{
	if( API_GetSLPacketLockManager()->IsLock( TU_PETITION_CONTENT_MODIFY_RES ) )
		return false;

	if( !pwcPetition )
		return FALSE;

	if( wcslen(pwcPetition) == 0 )
		return FALSE;

	sUT_PETITION_CONTENT_MODIFY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode			= UT_PETITION_CONTENT_MODIFY_REQ;
	sPacket.petitionID		= uiPetitionID;
	sPacket.nCategory1		= iCategory;
	sPacket.nCategory2		= iCategory2;
	wcscpy_s( sPacket.awchQuestionContent, NTL_MAX_SIZE_QUESTION_CONTENT + 1, pwcPetition );

	API_GetSLPacketLockManager()->Lock( TU_PETITION_CONTENT_MODIFY_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CChatPacketGenerator::SendPetitionUserCancelReq(RwUInt32 uiPetitionID)
{
	if( API_GetSLPacketLockManager()->IsLock( TU_PETITION_USER_CANCEL_RES ) )
		return false;

	sUT_PETITION_USER_CANCEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode			= UT_PETITION_USER_CANCEL_REQ;
	sPacket.petitionID		= uiPetitionID;

	API_GetSLPacketLockManager()->Lock( TU_PETITION_USER_CANCEL_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CChatPacketGenerator::SendPetitionChatStartRes(RwUInt16 wResultCode, RwUInt32 uiGMAccountID)
{
	sUT_PETITION_CHAT_START_RES sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode			= UT_PETITION_CHAT_START_RES;
	sPacket.wResultCode		= wResultCode;
	sPacket.gmAccountID		= uiGMAccountID;

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CChatPacketGenerator::SendPetitionChatGMSayRes(RwUInt16 wResultCode, RwUInt32 uiGMAccountID)
{
	sUT_PETITION_CHAT_GM_SAY_RES sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode			= UT_PETITION_CHAT_GM_SAY_RES;
	sPacket.wResultCode		= wResultCode;
	sPacket.gmAccountID		= uiGMAccountID;

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CChatPacketGenerator::SendPetitionChatUserSayReq(RwUInt32 uiGMAccountID, const WCHAR* pwcMessage)
{
	if( !pwcMessage )
		return FALSE;

	sUT_PETITION_CHAT_USER_SAY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode			= UT_PETITION_CHAT_USER_SAY_REQ;
	sPacket.gmAccountID		= uiGMAccountID;
	sPacket.wMessageLengthInUnicode		= (WORD)wcslen(pwcMessage);
	wcscpy_s( sPacket.awchMessage, NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1, pwcMessage );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CChatPacketGenerator::SendPetitionChatEndNfy(RwUInt32 uiGMAccountID)
{
	sUT_PETITION_CHAT_USER_END_NFY sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode			= UT_PETITION_CHAT_USER_END_NFY;
	sPacket.gmAccountID		= uiGMAccountID;

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CChatPacketGenerator::SendPetitionSatisfaction(RwUInt32 uiPetitionID, RwUInt8 bySatisfactionRate)
{
	sUT_PETITION_SATISFACTION_NFY sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode			= UT_PETITION_SATISFACTION_NFY;
	sPacket.petitionID		= uiPetitionID;
	sPacket.bySatisfactionRate	= bySatisfactionRate;

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CChatPacketGenerator::SendGuild_Disband_Req()
{
	if( API_GetSLPacketLockManager()->IsLock( TU_GUILD_DISBAND_RES ) )
		return true;

	sUT_GUILD_DISBAND_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UT_GUILD_DISBAND_REQ;

	API_GetSLPacketLockManager()->Lock( TU_GUILD_DISBAND_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket ); 
}

bool CChatPacketGenerator::SendGuild_Disband_Cancle_Req()
{
	if(API_GetSLPacketLockManager()->IsLock(TU_GUILD_DISBAND_CANCEL_RES))
		return false;

	sUT_GUILD_DISBAND_CANCEL_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UT_GUILD_DISBAND_CANCEL_REQ;

	API_GetSLPacketLockManager()->Lock( TU_GUILD_DISBAND_CANCEL_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket ); 
}

bool CChatPacketGenerator::SendGuild_Response_Invitation(RwUInt8 byResponse)
{
	if(API_GetSLPacketLockManager()->IsLock(TU_GUILD_RESPONSE_INVITATION_RES))
		return false;

	sUT_GUILD_RESPONSE_INVITATION sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UT_GUILD_RESPONSE_INVITATION;
	sPacket.byResponse = byResponse;

	API_GetSLPacketLockManager()->Lock( TU_GUILD_RESPONSE_INVITATION_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket ); 
}

bool CChatPacketGenerator::SendGuild_Leave_Req()
{
	if(API_GetSLPacketLockManager()->IsLock(TU_GUILD_LEAVE_RES))
		return false;

	sUT_GUILD_LEAVE_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UT_GUILD_LEAVE_REQ;

	API_GetSLPacketLockManager()->Lock( TU_GUILD_LEAVE_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket ); 
}

bool CChatPacketGenerator::SendGuild_Kick_Out_Req(RwUInt32 uiTargetSerial)
{
	if(API_GetSLPacketLockManager()->IsLock(TU_GUILD_KICK_OUT_RES))
		return false;

	sUT_GUILD_KICK_OUT_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UT_GUILD_KICK_OUT_REQ;
	sPacket.targetMemberCharId = uiTargetSerial;

	API_GetSLPacketLockManager()->Lock( TU_GUILD_KICK_OUT_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket ); 
}

bool CChatPacketGenerator::SendGuild_Appoint_Second_Master_Req(RwUInt32 uiTargetSerial)
{
	if(API_GetSLPacketLockManager()->IsLock(TU_GUILD_APPOINT_SECOND_MASTER_RES))
		return false;

	sUT_GUILD_APPOINT_SECOND_MASTER_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UT_GUILD_APPOINT_SECOND_MASTER_REQ;
	sPacket.targetMemberCharId = uiTargetSerial;

	API_GetSLPacketLockManager()->Lock( TU_GUILD_APPOINT_SECOND_MASTER_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket ); 
}

bool CChatPacketGenerator::SendGuild_DIsmiss_Second_Master_Req(RwUInt32 uiTargetSerial)
{
	if(API_GetSLPacketLockManager()->IsLock(TU_GUILD_DISMISS_SECOND_MASTER_RES))
		return false;

	sUT_GUILD_DISMISS_SECOND_MASTER_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UT_GUILD_DISMISS_SECOND_MASTER_REQ;
	sPacket.targetMemberCharId = uiTargetSerial;

	API_GetSLPacketLockManager()->Lock( TU_GUILD_DISMISS_SECOND_MASTER_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket ); 
}

bool CChatPacketGenerator::SendGuild_Change_Guild_Master_Req(RwUInt32 uiTargetSerial)
{
	if(API_GetSLPacketLockManager()->IsLock(TU_GUILD_CHANGE_GUILD_MASTER_RES))
		return false;

	sUT_GUILD_CHANGE_GUILD_MASTER_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UT_GUILD_CHANGE_GUILD_MASTER_REQ;
	sPacket.targetMemberCharId = uiTargetSerial;

	API_GetSLPacketLockManager()->Lock( TU_GUILD_CHANGE_GUILD_MASTER_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket ); 
}

bool CChatPacketGenerator::SendGuild_Notice_Req(RwUInt16 wNoticeLength, const WCHAR* pwcNotice)
{
	if(API_GetSLPacketLockManager()->IsLock(TU_GUILD_CHANGE_NOTICE_RES))
		return false;

	sUT_GUILD_CHANGE_NOTICE_REQ sPacket;
	memset( &sPacket, 0, sizeof( sPacket ) );

	sPacket.wOpCode = UT_GUILD_CHANGE_NOTICE_REQ;
	sPacket.wNoticeLengthInUnicode = wNoticeLength;
	wcscpy_s(sPacket.awchNotice, NTL_MAX_LENGTH_OF_GUILD_NOTICE + 1, pwcNotice);

	API_GetSLPacketLockManager()->Lock( TU_GUILD_CHANGE_NOTICE_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket ); 
}

bool CChatPacketGenerator::SendFriend_Add_Req( const WCHAR* pName ) 
{
    if(API_GetSLPacketLockManager()->IsLock(TU_FRIEND_ADD_RES))
        return false;

    sUT_FRIEND_ADD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UT_FRIEND_ADD_REQ;
	wcscpy_s(sPacket.wchName, NTL_MAX_SIZE_CHAR_NAME + 1, pName);

    API_GetSLPacketLockManager()->Lock(TU_FRIEND_ADD_RES);

    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CChatPacketGenerator::SendFriend_Del_Req(RwUInt32 uiTargetID, const WCHAR* pName)
{
    if(API_GetSLPacketLockManager()->IsLock(TU_FRIEND_DEL_RES))
        return false;

    sUT_FRIEND_DEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UT_FRIEND_DEL_REQ;
    sPacket.targetID = uiTargetID;
	wcscpy_s(sPacket.wchName, NTL_MAX_SIZE_CHAR_NAME + 1, pName);

    API_GetSLPacketLockManager()->Lock(TU_FRIEND_DEL_RES);

    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CChatPacketGenerator::SendFriend_Move_Req(RwUInt32 uiTargetID, const WCHAR* pName)
{
    if(API_GetSLPacketLockManager()->IsLock(TU_FRIEND_MOVE_RES))
        return false;

    sUT_FRIEND_MOVE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UT_FRIEND_MOVE_REQ;
    sPacket.targetID = uiTargetID;
	wcscpy_s(sPacket.wchName, NTL_MAX_SIZE_CHAR_NAME + 1, pName);

    API_GetSLPacketLockManager()->Lock(TU_FRIEND_MOVE_RES);

    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CChatPacketGenerator::SendFriend_Black_Add_Req( const WCHAR* pName ) 
{
    if(API_GetSLPacketLockManager()->IsLock(TU_FRIEND_BLACK_ADD_RES))
        return false;
    
    sUT_FRIEND_BLACK_ADD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UT_FRIEND_BLACK_ADD_REQ;
	wcscpy_s(sPacket.awchName, NTL_MAX_SIZE_CHAR_NAME + 1, pName);

    API_GetSLPacketLockManager()->Lock(TU_FRIEND_BLACK_ADD_RES);

    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CChatPacketGenerator::SendFriend_Black_Del_Req(RwUInt32 uiTargetID, const WCHAR* pName)
{
    if(API_GetSLPacketLockManager()->IsLock(TU_FRIEND_BLACK_DEL_RES))
        return false;

    sUT_FRIEND_BLACK_DEL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

    sPacket.wOpCode = UT_FRIEND_BLACK_DEL_REQ;
    sPacket.targetID = uiTargetID;
	wcscpy_s(sPacket.wchName, NTL_MAX_SIZE_CHAR_NAME + 1, pName);

    API_GetSLPacketLockManager()->Lock(TU_FRIEND_BLACK_DEL_RES);

    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CChatPacketGenerator::SendRankBattle_Rank_List_Req( RwInt32 dwPage, RwInt8 byCompareDay ) 
{
	if( API_GetSLPacketLockManager()->IsLock( TU_RANKBATTLE_RANK_LIST_RES ) )
		return false;

	sUT_RANKBATTLE_RANK_LIST_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UT_RANKBATTLE_RANK_LIST_REQ;
	sPacket.dwPage = dwPage;
	sPacket.byCompareDay = byCompareDay;

	API_GetSLPacketLockManager()->Lock( TU_RANKBATTLE_RANK_LIST_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket );
}

bool CChatPacketGenerator::SendRankBattle_Rank_Find_Character_Req( RwInt32 dwPage, RwInt8 byCompareDay, const WCHAR *pCharName ) 
{
	if( API_GetSLPacketLockManager()->IsLock( TU_RANKBATTLE_RANK_FIND_CHARACTER_RES ) )
		return false;

	sUT_RANKBATTLE_RANK_FIND_CHARACTER_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UT_RANKBATTLE_RANK_FIND_CHARACTER_REQ;
	sPacket.dwPage = dwPage;
	sPacket.byCompareDay = byCompareDay;
	swprintf_s(sPacket.wszCharName, L"%s", pCharName);

	API_GetSLPacketLockManager()->Lock( TU_RANKBATTLE_RANK_FIND_CHARACTER_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CChatPacketGenerator::SendRankBattle_Rank_Compare_Day_Req( RwInt32 dwPage, RwInt8 byCompareDay ) 
{
	if( API_GetSLPacketLockManager()->IsLock( TU_RANKBATTLE_RANK_COMPARE_DAY_RES ) )
		return false;

	sUT_RANKBATTLE_RANK_COMPARE_DAY_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UT_RANKBATTLE_RANK_COMPARE_DAY_REQ;
	sPacket.dwPage = dwPage;
	sPacket.byCompareDay = byCompareDay;

	API_GetSLPacketLockManager()->Lock( TU_RANKBATTLE_RANK_COMPARE_DAY_RES );
	
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CChatPacketGenerator::SendTMQ_Record_List_Req( RwUInt32 tmqTblidx, RwUInt8 byDifficult )
{
	if( API_GetSLPacketLockManager()->IsLock( TU_TMQ_RECORD_LIST_RES ) )
		return false;

	sUT_TMQ_RECORD_LIST_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UT_TMQ_RECORD_LIST_REQ;
	sPacket.tmqTblidx = tmqTblidx;
	sPacket.byDifficult = byDifficult;

	API_GetSLPacketLockManager()->Lock( TU_TMQ_RECORD_LIST_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CChatPacketGenerator::SendTMQ_Member_List_Req( RwUInt32 tmqTblidx, RwUInt8 byDifficult, RwUInt8 byRank )
{
	if( API_GetSLPacketLockManager()->IsLock( TU_TMQ_MEMBER_LIST_RES ) )
		return false;

	sUT_TMQ_MEMBER_LIST_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UT_TMQ_MEMBER_LIST_REQ;
	sPacket.tmqTblidx = tmqTblidx;
	sPacket.byDifficult = byDifficult;
	sPacket.byRank = byRank;

	API_GetSLPacketLockManager()->Lock( TU_TMQ_MEMBER_LIST_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CChatPacketGenerator::SendBudokaiTournamentIndividualListReq( RwUInt8 byReceivedListCount )
{
	if( API_GetSLPacketLockManager()->IsLock( TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES ) )
		return false;

	sUT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ;
	sPacket.byReceivedListCount = byReceivedListCount;

	API_GetSLPacketLockManager()->Lock( TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CChatPacketGenerator::SendBudokaiTournamentIndividualInfoReq( RwUInt16 wJoinID1, RwUInt16 wJoinID2 )
{
	if( API_GetSLPacketLockManager()->IsLock( TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES ) )
		return false;

	sUT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ sPacket;	
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ;
	sPacket.awJoinId[0] = wJoinID1;
	sPacket.awJoinId[1] = wJoinID2;

	API_GetSLPacketLockManager()->Lock( TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CChatPacketGenerator::SendBudokaiTournamentTeamListReq( RwUInt8 byReceivedListCount )
{
	if( API_GetSLPacketLockManager()->IsLock( TU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES ) )
		return false;

	sUT_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UT_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ;
	sPacket.byReceivedListCount = byReceivedListCount;

	API_GetSLPacketLockManager()->Lock( TU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CChatPacketGenerator::SendBudokaiTournamentTeamInfoReq( RwUInt16 wJoinID1, RwUInt16 wJoinID2 )
{
	if( API_GetSLPacketLockManager()->IsLock( TU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES ) )
		return false;

	sUT_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UT_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ;
	sPacket.awJoinId[0] = wJoinID1;
	sPacket.awJoinId[1] = wJoinID2;

	API_GetSLPacketLockManager()->Lock( TU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES );

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CChatPacketGenerator::SendDojo_BudokaiSeedAddReq(const WCHAR* pwcCharName)
{
	if( API_GetSLPacketLockManager()->IsLock( TU_DOJO_BUDOKAI_SEED_ADD_RES ) )
		return false;

	if( !pwcCharName )
		return false;

	if( wcslen(pwcCharName) > NTL_MAX_SIZE_CHAR_NAME )
		return false;

	sUT_DOJO_BUDOKAI_SEED_ADD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UT_DOJO_BUDOKAI_SEED_ADD_REQ;
	swprintf_s(sPacket.wszCharName, NTL_MAX_SIZE_CHAR_NAME + 1, L"%s", pwcCharName);	

	API_GetSLPacketLockManager()->Lock( TU_DOJO_BUDOKAI_SEED_ADD_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CChatPacketGenerator::SendDojo_NoticeChangeReq(RwUInt16 wNoticeLengthInUnicode, WCHAR* pwcNotice)
{
	if( API_GetSLPacketLockManager()->IsLock( TU_DOJO_NOTICE_CHANGE_RES ) )
		return false;

	if( !pwcNotice )
		return false;

	if( wcslen(pwcNotice) > NTL_MAX_LENGTH_OF_GUILD_NOTICE )
		return false;

	sUT_DOJO_NOTICE_CHANGE_REQ sPacket;
	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UT_DOJO_NOTICE_CHANGE_REQ;
	sPacket.wNoticeLengthInUnicode = wNoticeLengthInUnicode;
	swprintf_s(sPacket.awchNotice, NTL_MAX_LENGTH_OF_GUILD_NOTICE + 1, L"%s", pwcNotice);	

	API_GetSLPacketLockManager()->Lock( TU_DOJO_NOTICE_CHANGE_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}