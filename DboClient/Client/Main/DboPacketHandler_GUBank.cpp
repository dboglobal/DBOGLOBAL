/*****************************************************************************
* File			: DboPackethandler_GUBank.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2007. 1. 16
* Abstract		: 은행 관련 패킷 핸들
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// shared
#include "ItemTable.h"
#include "TextAllTable.h"

// simulation
#include "NtlNetSender.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"
#include "NtlWorldConceptNPCCommu.h"

// dbo
#include "IconMoveManager.h"

void PacketHandler_GSBankLoadReq(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_BANK_LOAD_RES );

	sGU_BANK_LOAD_RES* pResult = (sGU_BANK_LOAD_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BANK_LOAD_RES"), TRUE);
		return;
	}

	CNtlSLEventGenerator::SobWarehouseUpdate(NESWUT_RECIEVE_DATA, pResult->handle);
}

void PacketHandler_GSBankStartRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_BANK_START_RES );

	// 창고를 처음 여는 동작의 결과
	sGU_BANK_START_RES *pResult = (sGU_BANK_START_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CDboEventGenerator::DialogEvent(DIALOGEVENT_OPEN_FAIL_NPC_DIALOG, PLACE_NONE, DIALOG_WAREHOUSEBAR);
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BANK_START_RES"), TRUE);
		return;
	}
	
	CNtlWorldConceptNPCCOMMU* pWorldConceptController = reinterpret_cast<CNtlWorldConceptNPCCOMMU*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_NPC_COMMU) );
	pWorldConceptController->ChangeState(WORLD_NPC_WAREHOUSE);

	CNtlSLEventGenerator::SobWarehouseUpdate(NESWUT_WAREHOUSE_START, pResult->handle);
}

void PacketHandler_GSBankMoveRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_BANK_MOVE_RES );

	// 창고에 아이템을 넣거나 꺼냈다
	sGU_BANK_MOVE_RES *pResult = (sGU_BANK_MOVE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BANK_MOVE_RES"), TRUE);
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	CNtlSLEventGenerator::SobWarehouseItemMove(pSobAvatar->GetSerialID(), pResult->hSrcItem, pResult->bySrcPlace, pResult->bySrcPos,
		pResult->hDstItem, pResult->byDestPlace, pResult->byDestPos);
}

void PacketHandler_GSBankStackRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_BANK_MOVE_STACK_RES );

	// 창고에 아이템을 1개보다 많이 넣거나 꺼냈다
	sGU_BANK_MOVE_STACK_RES *pResult = (sGU_BANK_MOVE_STACK_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BANK_MOVE_STACK_RES"), TRUE);
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	CNtlSLEventGenerator::SobWarehouseItemMoveStack(pSobAvatar->GetSerialID(), pResult->hSrcItem, pResult->bySrcPlace, pResult->bySrcPos, pResult->byStackCount1,
		pResult->hDestItem, pResult->byDestPlace, pResult->byDestPos, pResult->byStackCount2);
}

void PacketHandler_GSBankEndRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_BANK_END_RES );

	// 창고를 닫았다. 메세지만 뿌린다
	sGU_BANK_END_RES *pResult = (sGU_BANK_END_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BANK_END_RES"), TRUE);
		return;
	}

	CNtlSLEventGenerator::SobWarehouseUpdate(NESWUT_CLOSE, INVALID_SERIAL_ID);
}

void PacketHandler_GSBankItemInfo(void *pPacket)
{
	// 처음으로 창고를 열어서 패킷을 받았다
	sGU_BANK_ITEM_INFO* pResult = (sGU_BANK_ITEM_INFO*)pPacket;

	CNtlSLEventGenerator::SobWarehouseItemCreate(pResult->byItemCount, pResult->aBankProfile);	
}

void PacketHandler_GSBankZennyInfo(void* pPacket)
{
	sGU_BANK_ZENNY_INFO* pResult = (sGU_BANK_ZENNY_INFO*)pPacket;

	// 제니
	CNtlSLEventGenerator::SobWarehouseUpdate(NESWUT_ADD_ZENNY, INVALID_SERIAL_ID, pResult->dwZenny);
}

void PacketHandler_GSBankZennyRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_BANK_ZENNY_RES );

	// 창고의 제니에 변화
	sGU_BANK_ZENNY_RES *pResult = (sGU_BANK_ZENNY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BANK_ZENNY_RES"), TRUE);
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	if(pResult->bIsSave)
		// 저금
		CNtlSLEventGenerator::SobWarehouseUpdate(NESWUT_ADD_ZENNY, pResult->handle, pResult->dwZenny);
	else
		// 인출
		CNtlSLEventGenerator::SobWarehouseUpdate(NESWUT_SUB_ZENNY, pResult->handle, pResult->dwZenny);
}

void PacketHandler_GSBankBuyRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_BANK_BUY_RES );

	// 창고 슬롯을 늘렸다
	sGU_BANK_BUY_RES *pResult = (sGU_BANK_BUY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BANK_BUY_RES"), TRUE);
		return;
	}

	sITEM_PROFILE itemProfile;
	itemProfile.handle = pResult->hItemhandle;
	itemProfile.tblidx = pResult->sData.itemNo;
	itemProfile.byPlace = pResult->sData.byPlace;
	itemProfile.byPos = pResult->sData.byPosition;
	itemProfile.byStackcount = pResult->sData.byStackcount;
	itemProfile.byRank = pResult->sData.byRank;
	itemProfile.byCurDur = pResult->sData.byCurrentDurability;
	itemProfile.bNeedToIdentify = pResult->sData.bNeedToIdentify;
	itemProfile.byGrade = pResult->sData.byGrade;
	
	memcpy(&itemProfile.sOptionSet, &pResult->sData.sOptionSet, sizeof(sITEM_OPTION_SET));

	CNtlSLEventGenerator::SobWarehouseItemCreate(1, &itemProfile);

	// 창고를 구입하였습니다
	GetAlarmManager()->AlarmMessage("DST_WAREHOUSE_SUCCESS_BUY");
}

void PacketHandler_GSBankItemDeleteRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_BANK_ITEM_DELETE_RES );

	// 창고 안의 아이템을 지운 결과를 알려준다
	// 실제로 지우는 것은 GU_ITEM_DELETE
	sGU_BANK_ITEM_DELETE_RES* pResult = (sGU_BANK_ITEM_DELETE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BANK_ITEM_DELETE_RES"), TRUE);
		return;
	}
}