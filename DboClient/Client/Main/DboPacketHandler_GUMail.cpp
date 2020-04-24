/*****************************************************************************
* File			: DboPackethandler_GUTMail.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2007. 1. 16
* Abstract		: 메일 관련 패킷 핸들
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// sound
#include "GUISoundDefine.h"

// simulation
#include "NtlNetSender.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"


// MailSystem
void PacketHandler_GUMailStartRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_MAIL_START_RES);

	sGU_MAIL_START_RES* pResult = (sGU_MAIL_START_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		CNtlSLEventGenerator::MailStart(pResult->hObject, pResult->bIsAway, FALSE);
		// Mail의 경우 자체적인 Dialog 오픈 실패 상황 처리 알고리즘이 존재
		// CDboEventGenerator::DialogEvent( DIALOGEVENT_OPEN_FAIL_NPC_DIALOG, PLACE_NONE, PLACE_NONE, DIALOG_MAILSYSTEM );
		return;
	}

	CNtlSLEventGenerator::MailStart(pResult->hObject, pResult->bIsAway, TRUE);
}

void PacketHandler_GUMailLoadInfo(void* pPacket)
{
	sGU_MAIL_LOAD_INFO* pResult = (sGU_MAIL_LOAD_INFO*)pPacket;

	sMAIL_PROFILE sMailProfile;
	sMailProfile.mailID			= pResult->sData.mailID;
	sMailProfile.bIsRead		= pResult->sData.bIsRead;
	sMailProfile.bIsAccept		= pResult->sData.bIsAccept;
	sMailProfile.bIsLock		= pResult->sData.bIsLock;
	sMailProfile.byMailType		= pResult->sData.byMailType;
	sMailProfile.bySenderType	= pResult->sData.bySenderType;
	sMailProfile.dwZenny		= pResult->sData.dwZenny;
	sMailProfile.tCreateTime	= pResult->sData.tCreateTime;
	sMailProfile.endTime		= pResult->sData.endTime;

	memcpy(&sMailProfile.sItemProfile, &pResult->sData.sItemProfile, sizeof(sITEM_PROFILE));
	wcscpy_s(sMailProfile.wszFromName, NTL_MAX_SIZE_CHAR_NAME + 1, pResult->sData.wszFromName);
	memset(sMailProfile.wszText, NULL, sizeof(sMailProfile.wszText));
	memcpy_s(sMailProfile.wszText, sizeof(sMailProfile.wszText), pResult->wszText, sizeof(WCHAR) * pResult->byTextSize);

	CNtlSLEventGenerator::MailLoadInfo(&sMailProfile);
}

void PacketHandler_GUMailLoadData(void* pPacket)
{
	sGU_MAIL_LOAD_DATA* pResult = (sGU_MAIL_LOAD_DATA*)pPacket;

	sMAIL_PROFILE sMailProfile;
	sMailProfile.mailID			= pResult->sData.mailID;
	sMailProfile.bIsRead		= pResult->sData.bIsRead;
	sMailProfile.bIsAccept		= pResult->sData.bIsAccept;
	sMailProfile.bIsLock		= pResult->sData.bIsLock;
	sMailProfile.byMailType		= pResult->sData.byMailType;
	sMailProfile.bySenderType	= pResult->sData.bySenderType;
	sMailProfile.dwZenny		= pResult->sData.dwZenny;
	sMailProfile.tCreateTime	= pResult->sData.tCreateTime;
	sMailProfile.endTime		= pResult->sData.endTime;

	memcpy(&sMailProfile.sItemProfile, &pResult->sData.sItemProfile, sizeof(sITEM_PROFILE));
	wcscpy_s(sMailProfile.wszFromName, NTL_MAX_SIZE_CHAR_NAME + 1, pResult->sData.wszFromName);
	memset(sMailProfile.wszText, NULL, sizeof(sMailProfile.wszText));
	memcpy_s(sMailProfile.wszText, sizeof(sMailProfile.wszText), pResult->wszText, sizeof(WCHAR) * pResult->byTextSize);

	CNtlSLEventGenerator::MailLoadData(&sMailProfile);
}

void PacketHandler_GUMailSendRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_MAIL_SEND_RES);
	
	sGU_MAIL_SEND_RES* pResult = (sGU_MAIL_SEND_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::MailSend(pResult->hObject, pResult->wszTargetName);

	Logic_PlayGUISound( GSD_SYSTEM_MAIL_SEND );

	// 메일 퀘스트
	CNtlSLEventGenerator::TSUseMail();
}

void PacketHandler_GUMailReadRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_MAIL_READ_RES);

	sGU_MAIL_READ_RES* pResult = (sGU_MAIL_READ_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	Logic_PlayGUISound( GSD_SYSTEM_NEW_MAIL_ALRAM );

	CNtlSLEventGenerator::MailRead(pResult->hObject, pResult->mailID, pResult->byRemainDay);
}

void PacketHandler_GUMailDelRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_MAIL_DEL_RES);

	sGU_MAIL_DEL_RES* pResult = (sGU_MAIL_DEL_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::MailDel(pResult->hObject, pResult->mailID);
}

void PacketHandler_GUMailReturnRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_MAIL_RETURN_RES);

	sGU_MAIL_RETURN_RES* pResult = (sGU_MAIL_RETURN_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::MailReturn(pResult->hObject, pResult->mailID);
}

void PacketHandler_GUMailReloadRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_MAIL_RELOAD_RES);

	sGU_MAIL_RELOAD_RES* pResult = (sGU_MAIL_RELOAD_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::MailReload(pResult->hObject, pResult->aMailID, pResult->byMailCount, pResult->byNormalCount, pResult->byManagerCount);
}

void PacketHandler_GUMailReloadNfy(void* pPacket)
{
	sGU_MAIL_RELOAD_NFY* pResult = (sGU_MAIL_RELOAD_NFY*)pPacket;

	CNtlSLEventGenerator::MailReloadNfy(pResult->aMailID, pResult->byMailCount, pResult->byNormalCount, pResult->byManagerCount);
}

void PacketHandler_GUMailLoadRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_MAIL_LOAD_RES);

	sGU_MAIL_LOAD_RES* pResult = (sGU_MAIL_LOAD_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::MailLoad(pResult->hObject);
}

void PacketHandler_GUMailItemReceiveRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_MAIL_ITEM_RECEIVE_RES);

	sGU_MAIL_ITEM_RECEIVE_RES* pResult = (sGU_MAIL_ITEM_RECEIVE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::MailItemReceive(pResult->hObject, pResult->mailID);
}

void PacketHandler_GUMailLockRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_MAIL_LOCK_RES);

	sGU_MAIL_LOCK_RES* pResult = (sGU_MAIL_LOCK_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::MailLock(pResult->hObject, pResult->mailID, pResult->bIsLock, pResult->endTime);
}

void PacketHandler_GUMailCloseNfy(void* pPacket)
{
	CNtlSLEventGenerator::MailCloseNfy();
}

void PacketHandler_GUMailMultiDelRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_MAIL_MULTI_DEL_RES);

	sGU_MAIL_MULTI_DEL_RES* pResult = (sGU_MAIL_MULTI_DEL_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::MailMultiDel( pResult->hObject, pResult->byCount, pResult->aMailID );
}

