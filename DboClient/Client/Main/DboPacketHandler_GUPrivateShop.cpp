/*****************************************************************************
* File			: DboPackethandler_GUPrivateShop.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2007. 1. 16
* Abstract		: 개인상점 관련 패킷 핸들
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// simulation
#include "NtlNetSender.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"
#include "NtlItem.h"
#include "NtlWorldConceptPrivateShop.h"
#include "NtlCameraController.h"

// dbo
#include "IconMoveManager.h"


//PrivateShop
void PacketHandler_GUPrivateShop_Create_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_PRIVATESHOP_CREATE_RES);

	sGU_PRIVATESHOP_CREATE_RES* pResult = (sGU_PRIVATESHOP_CREATE_RES*)pPacket;
	
	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	GetNtlWorldConcept()->AddWorldPlayConcept(WORLD_PLAY_PRIVATESHOP);

	CNtlWorldConceptController*		pController				= GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_PRIVATESHOP);
	CNtlWorldConceptPrivateShop*	pPrivateShopController	= reinterpret_cast<CNtlWorldConceptPrivateShop*>(pController);

	if (pPrivateShopController)
	{
		pPrivateShopController->SetPrivateShopState(PRIVATESHOP_STATE_CLOSE);
	}
	
	sPRIVATESHOP_SHOP_DATA sShopData;
	sShopData.hOwner					= GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();
	sShopData.waittimeCashShopStart		= pResult->sPrivateShopData.waittimeCashShopStart;
	sShopData.waittimeCashShopEnd		= pResult->sPrivateShopData.waittimeCashShopEnd;
	sShopData.sSummaryPrivateShopData	= pResult->sPrivateShopData.sSummaryPrivateShopData;
	
	memcpy_s(sShopData.wcNotice, sizeof(sShopData.wcNotice), pResult->sPrivateShopData.wcNotice, pResult->sPrivateShopData.byNoticeSize);

	CNtlSLEventGenerator::PrivateShopState(PRIVATESHOP_EVENT_CREATE, PRIVATESHOP_STATE_CLOSE, &sShopData, TRUE);
}

/**
* \brief 주변에 개인상점이 열렸을 경우 브로드 캐스팅을 받는다.
*/
void PacketHandler_GUPrivateShop_Create_Nfy(void *pPacket)
{
	sGU_PRIVATESHOP_CREATE_NFY* pNotify = (sGU_PRIVATESHOP_CREATE_NFY*)pPacket;

	sPRIVATESHOP_SHOP_DATA sShopData;
	sShopData.hOwner = pNotify->hOwner;
	sShopData.waittimeCashShopStart = 0;
	sShopData.waittimeCashShopEnd = 0;
	sShopData.sSummaryPrivateShopData = pNotify->sSummaryPrivateShopData;

	swprintf_s( sShopData.wcNotice, NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1, L"%s", L"Data Loading..." );

	CNtlSLEventGenerator::PrivateShopState(PRIVATESHOP_EVENT_CREATE, PRIVATESHOP_STATE_CLOSE, &sShopData, FALSE);
}

void PacketHandler_GUPrivateShop_Exit_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_PRIVATESHOP_EXIT_RES);

	sGU_PRIVATESHOP_EXIT_RES* pResult = (sGU_PRIVATESHOP_EXIT_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}	
	
	CNtlWorldConceptController*		pController				= GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_PRIVATESHOP);
	CNtlWorldConceptPrivateShop*	pPrivateShopController	= reinterpret_cast<CNtlWorldConceptPrivateShop*>(pController);

	if (pPrivateShopController)
	{
		pPrivateShopController->SetPrivateShopState(PRIVATESHOP_STATE_NONE);
	}
	GetNtlWorldConcept()->RemoveWorldPlayConcept(WORLD_PLAY_PRIVATESHOP);

	sPRIVATESHOP_SHOP_DATA sShopData;
	sShopData.hOwner					= GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();

	CNtlSLEventGenerator::PrivateShopState(PRIVATESHOP_EVENT_EXIT, PRIVATESHOP_STATE_NONE, &sShopData, TRUE);
}

void PacketHandler_GUPrivateShop_Open_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_PRIVATESHOP_OPEN_RES);

	sGU_PRIVATESHOP_OPEN_RES* pResult = (sGU_PRIVATESHOP_OPEN_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlWorldConceptController*		pController				= GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_PRIVATESHOP);
	CNtlWorldConceptPrivateShop*	pPrivateShopController	= reinterpret_cast<CNtlWorldConceptPrivateShop*>(pController);

	if (pPrivateShopController)
	{
		pPrivateShopController->SetPrivateShopState(PRIVATESHOP_STATE_OPEN);
	}


	sPRIVATESHOP_SHOP_DATA sShopData;
	sShopData.hOwner					= GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();
	sShopData.sSummaryPrivateShopData	= pResult->sSummaryPrivateShopData;

	memcpy_s(sShopData.wcNotice, sizeof(sShopData.wcNotice), pResult->wcNotice, pResult->byNoticeSize);

	CNtlSLEventGenerator::PrivateShopState(PRIVATESHOP_EVENT_OPEN, PRIVATESHOP_STATE_OPEN, &sShopData, TRUE);
}

void PacketHandler_GUPrivateShop_Open_Nfy(void *pPacket)
{
	sGU_PRIVATESHOP_OPEN_NFY* pResult = (sGU_PRIVATESHOP_OPEN_NFY*)pPacket;

	/*if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, pResult->wResultCode);
		return;
	}*/

	sPRIVATESHOP_SHOP_DATA sShopData;
	sShopData.hOwner					= pResult->hOwner;
	memcpy(&sShopData.sSummaryPrivateShopData, &pResult->sSummaryPrivateShopData, sizeof(sSUMMARY_PRIVATESHOP_SHOP_DATA));

	CNtlSLEventGenerator::PrivateShopState(PRIVATESHOP_EVENT_OPEN, PRIVATESHOP_STATE_OPEN, &sShopData, FALSE);
}

void PacketHandler_GUPrivateShop_Close_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_PRIVATESHOP_CLOSE_RES);

	sGU_PRIVATESHOP_CLOSE_RES* pResult = (sGU_PRIVATESHOP_CLOSE_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlWorldConceptController*		pController				= GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_PRIVATESHOP);
	CNtlWorldConceptPrivateShop*	pPrivateShopController	= reinterpret_cast<CNtlWorldConceptPrivateShop*>(pController);

	if (pPrivateShopController)
	{
		pPrivateShopController->SetPrivateShopState(PRIVATESHOP_STATE_CLOSE);
	}

	sPRIVATESHOP_SHOP_DATA sShopData;
	sShopData.hOwner					= GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();

	CNtlSLEventGenerator::PrivateShopState(PRIVATESHOP_EVENT_CLOSE, PRIVATESHOP_STATE_CLOSE, &sShopData, TRUE);
}

void PacketHandler_GUPrivateShop_Close_Nfy(void *pPacket)
{
	sGU_PRIVATESHOP_CLOSE_NFY* pResult = (sGU_PRIVATESHOP_CLOSE_NFY*)pPacket;

	/*if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, pResult->wResultCode);
		return;
	}*/

	sPRIVATESHOP_SHOP_DATA sShopData;
	sShopData.hOwner = pResult->hOwner;

	CNtlSLEventGenerator::PrivateShopState(PRIVATESHOP_EVENT_CLOSE, PRIVATESHOP_STATE_CLOSE, &sShopData, FALSE);
}

void PacketHandler_GUPrivateShop_Enter_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_PRIVATESHOP_ENTER_RES);

	sGU_PRIVATESHOP_ENTER_RES* pResult = (sGU_PRIVATESHOP_ENTER_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::CameraNpc(pResult->sPrivateShopData.hOwner);
	GetNtlWorldConcept()->AddWorldPlayConcept(WORLD_PLAY_PRIVATESHOP_VISITOR);

	sPRIVATESHOP_SHOP_DATA sShopData;
	sShopData.hOwner					= pResult->sPrivateShopData.hOwner;
	sShopData.waittimeCashShopStart		= pResult->sPrivateShopData.waittimeCashShopStart;
	sShopData.waittimeCashShopEnd		= pResult->sPrivateShopData.waittimeCashShopEnd;
	
	memcpy(&sShopData.sSummaryPrivateShopData, &pResult->sPrivateShopData.sSummaryPrivateShopData, sizeof(sSUMMARY_PRIVATESHOP_SHOP_DATA));
	memcpy_s(sShopData.wcNotice, sizeof(sShopData.wcNotice), pResult->sPrivateShopData.wcNotice, pResult->sPrivateShopData.byNoticeSize);
	
	CNtlSLEventGenerator::PrivateShopStateVisitor(PRIVATESHOP_EVENT_ENTER, &sShopData);
}

void PacketHandler_GUPrivateShop_Leave_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_PRIVATESHOP_LEAVE_RES);

	sGU_PRIVATESHOP_LEAVE_RES* pResult = (sGU_PRIVATESHOP_LEAVE_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_NPC);
	GetNtlWorldConcept()->RemoveWorldPlayConcept(WORLD_PLAY_PRIVATESHOP_VISITOR);

	CNtlSLEventGenerator::PrivateShopStateVisitor(PRIVATESHOP_EVENT_LEAVE, NULL);
}

void PacketHandler_GUPrivateShop_Leave_Nfy(void *pPacket)
{
	//sGU_PRIVATESHOP_LEAVE_NFY* pResult = (sGU_PRIVATESHOP_LEAVE_NFY*)pPacket;

	/*if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, pResult->wResultCode);
		return;
	}*/

	CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_NPC);
	GetNtlWorldConcept()->RemoveWorldPlayConcept(WORLD_PLAY_PRIVATESHOP_VISITOR);
	CNtlSLEventGenerator::PrivateShopStateVisitor(PRIVATESHOP_EVENT_LEAVE, NULL);
}

void PacketHandler_GUPrivateShop_Item_Insert_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_PRIVATESHOP_ITEM_INSERT_RES);

	sGU_PRIVATESHOP_ITEM_INSERT_RES* pResult = (sGU_PRIVATESHOP_ITEM_INSERT_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	GetIconMoveManager()->IconMoveEnd();
	CNtlSLEventGenerator::PrivateShopItem(PRIVATESHOP_EVENT_ITEM_INSERT, &pResult->sPrivateShopItemData);
}

void PacketHandler_GUPrivateShop_Item_Update_Res(void *pPacket)
{
	sGU_PRIVATESHOP_ITEM_UPDATE_RES* pResult = (sGU_PRIVATESHOP_ITEM_UPDATE_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}
	sPRIVATESHOP_ITEM_DATA sItemData;
	sItemData.byPrivateShopInventorySlotPos	= pResult->byPrivateShopInventorySlotPos; 
	sItemData.dwZenny			= pResult->dwZenny;

	CNtlSLEventGenerator::PrivateShopItem(PRIVATESHOP_EVENT_ITEM_UPDATE, &sItemData);
}

void PacketHandler_GUPrivateShop_Item_Delete_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_PRIVATESHOP_ITEM_DELETE_RES);
	
	sGU_PRIVATESHOP_ITEM_DELETE_RES* pResult = (sGU_PRIVATESHOP_ITEM_DELETE_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::PrivateShopItem(PRIVATESHOP_EVENT_ITEM_DELETE, &pResult->sPrivateShopItemData);
}

void PacketHandler_GUPrivateShop_Item_Data_Info(void *pPacket)
{
	sGU_PRIVATESHOP_ITEM_DATA_INFO* pResult = (sGU_PRIVATESHOP_ITEM_DATA_INFO*)pPacket;
	
	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::PrivateShopItemDataInfo(pResult->hOwnerID, &pResult->sPrivateShopItemData);
} 

void PacketHandler_GUPrivateShop_Item_Select_Res(void *pPacket)
{
	sGU_PRIVATESHOP_ITEM_SELECT_RES* pResult = (sGU_PRIVATESHOP_ITEM_SELECT_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}
	
	CNtlSLEventGenerator::PrivateShopItemSelect(pResult->byPrivateShopInventorySlotPos, pResult->byItemState);

	if( pResult->byItemState == sPRIVATESHOP_ITEM_DATA::PRIVATESHOP_VISITOR_SELECT_ITEM )
	{
		CDboEventGenerator::PrivateShopSelectNfy(pResult->byPrivateShopInventorySlotPos, pResult->byItemState);
	}
}

void PacketHandler_GUPrivateShop_Item_Select_Nfy(void *pPacket)
{
	sGU_PRIVATESHOP_ITEM_SELECT_NFY* pResult = (sGU_PRIVATESHOP_ITEM_SELECT_NFY*)pPacket;

	/*if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, pResult->wResultCode);
		return;
	}*/

	CNtlSLEventGenerator::PrivateShopItemSelect(pResult->byPrivateShopInventorySlotPos, pResult->byItemState);
}

void PacketHandler_GUPrivateShop_Item_Buying_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_PRIVATESHOP_ITEM_BUYING_RES);

	sGU_PRIVATESHOP_ITEM_BUYING_RES* pResult = (sGU_PRIVATESHOP_ITEM_BUYING_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::PrivateShopItemBuying(INVALID_SERIAL_ID,
												TRUE,
												pResult->sPrivateShopItemData,
												pResult->byBuyItemCount);
}

void PacketHandler_GUPrivateShop_Item_Buying_Nfy(void *pPacket)
{
	sGU_PRIVATESHOP_ITEM_BUYING_NFY* pResult = (sGU_PRIVATESHOP_ITEM_BUYING_NFY*)pPacket;

	/*if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, pResult->wResultCode);
		return;
	}*/

	CNtlSLEventGenerator::PrivateShopItemBuying(pResult->hPrivateShopOwner,
												FALSE,
												pResult->sPrivateShopItemData,
												pResult->byBuyItemCount);
}

void PacketHandler_GUPrivateShop_Item_State_Nfy(void *pPacket)
{
	sGU_PRIVATESHOP_ITEM_STATE_NFY* pResult = (sGU_PRIVATESHOP_ITEM_STATE_NFY*)pPacket;

	/*if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, pResult->wResultCode);
		return;
	}*/

	CNtlSLEventGenerator::PrivateShopItemState(pResult->abyPrivateShopItemState, pResult->nPrivateShopItemStateCount);
}
