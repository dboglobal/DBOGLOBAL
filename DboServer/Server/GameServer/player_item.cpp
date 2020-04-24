#include "stdafx.h"
#include "GameServer.h"
#include "CPlayer.h"
#include "TableContainerManager.h"
#include "VehicleTable.h"
#include "item.h"
#include "ItemDrop.h"
#include "NtlPacketGU.h"
#include "NtlResultCode.h"
#include "NtlPacketGQ.h"
#include "GameMain.h"
#include "NtlNavi.h"



///////////////////
// Equip item
bool CPlayer::EquipItem(CItem* item, BYTE byDestpos)
{
	sITEM_DATA* pItemData = &item->GetItemData();

	CNtlPacket packet(sizeof(sGU_UPDATE_ITEM_EQUIP));
	sGU_UPDATE_ITEM_EQUIP * res = (sGU_UPDATE_ITEM_EQUIP *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_ITEM_EQUIP;
	res->handle = GetID();
	res->byPos = byDestpos;
	res->sItemBrief.tblidx = pItemData->itemNo;
	res->sItemBrief.byGrade = pItemData->byGrade;
	res->sItemBrief.byBattleAttribute = pItemData->byBattleAttribute;
	packet.SetPacketLen(sizeof(sGU_UPDATE_ITEM_EQUIP));
	BroadcastToNeighbor(&packet); // no need to send to myself

	GetPlayerItemContainer()->SetItemBrief(byDestpos, pItemData);

	item->SetEquipped(true);

	return true;
}

///////////////////
// Unequip item
bool CPlayer::UnequipItem(CItem* item)
{
	if (!item->IsEquipped())
		return false;

	CNtlPacket packet(sizeof(sGU_UPDATE_ITEM_EQUIP));
	sGU_UPDATE_ITEM_EQUIP * res = (sGU_UPDATE_ITEM_EQUIP *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_ITEM_EQUIP;
	res->handle = GetID();
	res->byPos = item->GetPos();
	memset(&res->sItemBrief, -1, sizeof(res->sItemBrief));
	packet.SetPacketLen(sizeof(sGU_UPDATE_ITEM_EQUIP));
	BroadcastToNeighbor(&packet); // no need to send to myself

	GetPlayerItemContainer()->UnsetItemBrief(item->GetPos());

	item->SetEquipped(false);

	return true;
}

///////////////////
// set item cool-down (load from db)
void CPlayer::SetItemCooldown(sDBO_ITEM_COOL_TIME& cooltimedata)
{
	m_ItemCooldown[cooltimedata.byItemCoolTimeGroupIndex] = cooltimedata;
}
///////////////////
// check if item is on cooldown
bool CPlayer::IsItemCooldown(DWORD CoolTimeBitFlag)
{
	for (int i = 0; i < ITEM_COOL_TIME_GROUP_COUNT; i++)
	{
		if (MAKE_BIT_FLAG(i) == CoolTimeBitFlag)
		{
			if (m_ItemCooldown[i].dwItemCoolTimeRemaining > 0)
				return true;
			else
				return false;
		}
	}
	return false;
}
///////////////////
// begin item cool down
void CPlayer::BeginItemCooldown(DWORD CoolTimeBitFlag, DWORD CoolTime, DWORD RemainTime)
{
	for (int i = 0; i < ITEM_COOL_TIME_GROUP_COUNT; i++)
	{
		if (MAKE_BIT_FLAG(i) == CoolTimeBitFlag)
		{
			m_ItemCooldown[i].byItemCoolTimeGroupIndex = i;
			m_ItemCooldown[i].dwInitialItemCoolTime = CoolTime;
			m_ItemCooldown[i].dwItemCoolTimeRemaining = RemainTime;

			break;
		}
	}
}
///////////////////
// process item cool down time
void CPlayer::ItemCooltimeProcess(DWORD dwTime)
{
	for (int i = 0; i < ITEM_COOL_TIME_GROUP_COUNT; i++)
	{
		m_ItemCooldown[i].dwItemCoolTimeRemaining = UnsignedSafeDecrease<DWORD>(m_ItemCooldown[i].dwItemCoolTimeRemaining, dwTime);
	}

	//process item
	GetPlayerItemContainer()->ProcessItemWithDuration(dwTime);
}



//--------------------------------------------------------------------------------------//
//		START VEHICLE
//--------------------------------------------------------------------------------------//
bool CPlayer::StartVehicle(TBLIDX vehicleIdx, HOBJECT hVehicleId)
{
	sVEHICLE_TBLDAT* pVehicleTbl = (sVEHICLE_TBLDAT*)g_pTableContainer->GetVehicleTable()->FindData(vehicleIdx);
	if (pVehicleTbl)
	{
		SetVehicleAniPlay(true); // set that we play animation (cant move during play animation)

		// Go standing state, so we dont "run" while use vehicle capsule
		SendCharStateStanding();

		CNtlPacket pPacket(sizeof(sGU_VEHICLE_START_NFY));
		sGU_VEHICLE_START_NFY* pRes = (sGU_VEHICLE_START_NFY*)pPacket.GetPacketData();
		pRes->wOpCode = GU_VEHICLE_START_NFY;
		pRes->hDriverHandle = GetID();
		pRes->hVehicleItem = hVehicleId;
		pRes->idVehicleItemTblidx = vehicleIdx;
		pRes->idFuelItemTblidx = INVALID_TBLIDX;
		if (GetVehicleFuelId() != INVALID_HOBJECT)
		{
			CItem* pFuel = GetPlayerItemContainer()->GetItem(GetVehicleFuelId());
			if (pFuel)
			{
				pRes->idFuelItemTblidx = pFuel->GetTblidx();
			}
			else
			{
				ERR_LOG(LOG_USER, "Player %u GetVehicleFuelId() = %u. pFuel == NULL !", GetVehicleFuelId());
			}
		}
		Broadcast(&pPacket);

		/*
			byVehicleType 0 = get off from vehicle on attack
			byVehicleType 1 = stay on vehicle on attack(only auto attack)
		*/
		SetVehicle(vehicleIdx, hVehicleId, pVehicleTbl->byVehicleType == 0);

		CNtlPacket pPacket2(sizeof(sGU_UPDATE_CHAR_ASPECT_STATE));
		sGU_UPDATE_CHAR_ASPECT_STATE * pRes2 = (sGU_UPDATE_CHAR_ASPECT_STATE*)pPacket2.GetPacketData();
		pRes2->handle = GetID();
		pRes2->wOpCode = GU_UPDATE_CHAR_ASPECT_STATE;
		pRes2->aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_VEHICLE;
		pRes2->aspectState.sAspectStateDetail.sVehicle.idVehicleTblidx = vehicleIdx;
		pRes2->aspectState.sAspectStateDetail.sVehicle.idVehicleItemHandle = hVehicleId;
		Broadcast(&pPacket2);

		GetStateManager()->CopyAspectFrom(&pRes2->aspectState);

		GetCharAtt()->CalculateAll();

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		END VEHICLE
//--------------------------------------------------------------------------------------//
void CPlayer::EndVehicle(WORD wReasonCode)
{
	// send packet
	CNtlPacket packet(sizeof(sGU_VEHICLE_END_RES));
	sGU_VEHICLE_END_RES * res = (sGU_VEHICLE_END_RES *)packet.GetPacketData();
	res->wOpCode = GU_VEHICLE_END_RES;
	res->wResultCode = wReasonCode;
	packet.SetPacketLen(sizeof(sGU_VEHICLE_END_RES));
	SendPacket(&packet);

	// remove fuel
	if (GetVehicleFuelId() != INVALID_HOBJECT)
	{
		CItem* pFuel = GetPlayerItemContainer()->GetItem(GetVehicleFuelId());
		if (pFuel)
		{
			pFuel->SetLocked(false);
		}
		else
		{
			ERR_LOG(LOG_USER, "Player %u GetVehicleFuelId() = %u. pFuel == NULL !", GetCharID(), GetVehicleFuelId());
		}
	}

	// init vehicle data
	m_vehicle.Init();

	CNtlPacket packet4(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res4 = (sGU_UPDATE_CHAR_STATE *)packet4.GetPacketData();
	res4->wOpCode = GU_UPDATE_CHAR_STATE;
	res4->handle = GetID();
	res4->sCharState.sCharStateBase.byStateID = CHARSTATE_RIDEON;
	GetCurLoc().CopyTo(res4->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res4->sCharState.sCharStateBase.vCurDir);
	res4->sCharState.sCharStateBase.eAirState = GetAirState();
	res4->sCharState.sCharStateBase.bFightMode = GetFightMode();
	res4->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res4->sCharState.sCharStateBase.dwStateTime = 0;
	res4->sCharState.sCharStateBase.aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_VEHICLE;
	res4->sCharState.sCharStateBase.aspectState.sAspectStateDetail.sVehicle.idVehicleTblidx = INVALID_TBLIDX;
	res4->sCharState.sCharStateBase.aspectState.sAspectStateDetail.sVehicle.idVehicleItemHandle = INVALID_HOBJECT;
	packet4.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));
	Broadcast(&packet4);
	
	GetStateManager()->CopyFrom(&res4->sCharState);

	SendAspectStateInvalid();
	SendCharStateStanding();

	GetCharAtt()->CalculateAll();
}


//--------------------------------------------------------------------------------------//
//		UPDATE VEHICLE FUEL
//--------------------------------------------------------------------------------------//
void CPlayer::UpdateVehicleFuel(bool bInsert, BYTE byItemPlace/* = INVALID_BYTE*/, BYTE byItemPos/* = INVALID_BYTE*/)
{
	if (bInsert)
	{
		CNtlPacket packet(sizeof(sGU_VEHICLE_FUEL_INSERT_RES));
		sGU_VEHICLE_FUEL_INSERT_RES * res = (sGU_VEHICLE_FUEL_INSERT_RES *)packet.GetPacketData();
		res->wOpCode = GU_VEHICLE_FUEL_INSERT_RES;
		res->wResultCode = GAME_VEHICLE_FAIL_TO_USE_FUEL_ITEM;	//by default we set fail

		if (IsInvenContainer(byItemPlace))
		{
			CItem* item = GetPlayerItemContainer()->GetItem(byItemPlace, byItemPos);
			if (item && item->IsExpired() == false)
			{
				if (item->GetTbldat()->byItem_Type == ITEM_TYPE_FUEL)
				{
					// check if already have a fuel registered
					if (GetVehicleFuelId() != INVALID_HOBJECT)
					{
						CItem* pFuel = GetPlayerItemContainer()->GetItem(GetVehicleFuelId());
						if (pFuel)
						{
							pFuel->SetLocked(false);
						}
					}

					res->wResultCode = GAME_SUCCESS;
					res->hItem = item->GetID();

					SetVehicleFuelId(item->GetID());
					item->SetLocked(true);
				}
				else res->wResultCode = GAME_VEHICLE_INVALID_FUEL_ITEM;
			}
		}

		packet.SetPacketLen(sizeof(sGU_VEHICLE_FUEL_INSERT_RES));
		SendPacket(&packet);

		if (res->wResultCode == GAME_SUCCESS)
			GetCharAtt()->CalculateAll();
	}
	else
	{
		bool bSuccess = false;
		if (GetVehicleFuelId() != INVALID_HOBJECT)
		{
			CItem* pFuel = GetPlayerItemContainer()->GetItem(GetVehicleFuelId());
			if (pFuel)
			{
				SetVehicleFuelId(INVALID_HOBJECT);
				pFuel->SetLocked(false);

				bSuccess = true;
			}
			else
			{
				ERR_LOG(LOG_USER, "Player %u GetVehicleFuelId() = %u. pFuel == NULL !", GetCharID(), GetVehicleFuelId());
			}
		}

		if(bSuccess && GetAspectStateId() == ASPECTSTATE_VEHICLE)
			GetCharAtt()->CalculateAll();
	}
}

void CPlayer::SetVehicle(TBLIDX vehicleTblidx, HOBJECT hVehicleId, bool bGetOffOnAttack)
{
	m_vehicle.vehicleTblidx = vehicleTblidx;
	m_vehicle.hVehicleId = hVehicleId;
	m_vehicle.bGetOffOnAttack = bGetOffOnAttack;
}

/////////////////////////////////////////////////////////////////
// HOI POI CRAFTING

///////////////////
// clear recipes
void CPlayer::ClearRecipes()
{
	std::map<TBLIDX, sRECIPE_DATA*>::iterator it = p_HoiPoiRecipeData.begin();
	for (; it != p_HoiPoiRecipeData.end(); it++)
	{
		sRECIPE_DATA* pInfo = it->second;
		SAFE_DELETE(pInfo);
	}

	p_HoiPoiRecipeData.clear();
}


///////////////////
// add recipe
void CPlayer::AddHoiPoiRecipe(TBLIDX recipetblidx, BYTE byRecipeType)
{
	sRECIPE_DATA* recipe = new sRECIPE_DATA;

	recipe->byRecipeType = recipetblidx;
	recipe->recipeTblidx = byRecipeType;

	p_HoiPoiRecipeData.insert(std::make_pair(recipetblidx, recipe));
}
///////////////////
// get recipe
sRECIPE_DATA* CPlayer::GetHoiPoiRecipe(TBLIDX recipetblidx)
{
	std::map<TBLIDX, sRECIPE_DATA*>::iterator it = p_HoiPoiRecipeData.find(recipetblidx);

	if (p_HoiPoiRecipeData.end() == it)
		return NULL;

	return it->second;
}

//--------------------------------------------------------------------------------------//
//		QUICK TELEPORT ITEM
//--------------------------------------------------------------------------------------//
void CPlayer::AddQuickTeleport(sQUICK_TELEPORT_INFO* data, BYTE bySlot, bool bQuery/* = true*/)
{
	tQuickTeleportMap::iterator it = m_mapQuickTeleport.find(bySlot);
	if (it != m_mapQuickTeleport.end()) //slot already exist -> update slot
	{
		sQUICK_TELEPORT_INFO* cur = it->second;
		memcpy(cur, data, sizeof(sQUICK_TELEPORT_INFO));
	}
	else //slot is empty. add new
	{
		sQUICK_TELEPORT_INFO* info = new sQUICK_TELEPORT_INFO;
		memcpy(info, data, sizeof(sQUICK_TELEPORT_INFO));

		m_mapQuickTeleport.insert(std::make_pair(bySlot, info));
	}

	if (bQuery)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		CNtlPacket pQry(sizeof(sGQ_QUICK_TELEPORT_UPDATE_REQ));
		sGQ_QUICK_TELEPORT_UPDATE_REQ * qRes = (sGQ_QUICK_TELEPORT_UPDATE_REQ *)pQry.GetPacketData();
		qRes->wOpCode = GQ_QUICK_TELEPORT_UPDATE_REQ;
		qRes->charId = GetCharID();
		memcpy(&qRes->asData, data, sizeof(sQUICK_TELEPORT_INFO));
		pQry.SetPacketLen(sizeof(sGQ_QUICK_TELEPORT_UPDATE_REQ));
		app->SendTo(app->GetQueryServerSession(), &pQry);
	}
}

sQUICK_TELEPORT_INFO* CPlayer::GetQuickTeleportData(BYTE bySlot)
{
	tQuickTeleportMap::iterator it = m_mapQuickTeleport.find(bySlot);
	if (it != m_mapQuickTeleport.end())
		return it->second;
	else
		return NULL;
}

bool CPlayer::DelQuickTeleportData(BYTE bySlot)
{
	tQuickTeleportMap::iterator it = m_mapQuickTeleport.find(bySlot);
	if (it != m_mapQuickTeleport.end()) //slot already exist -> update slot
	{
		sQUICK_TELEPORT_INFO* cur = it->second;
		SAFE_DELETE(cur);
		m_mapQuickTeleport.erase(bySlot);

		return true;
	}
	else return false;
}

void CPlayer::LoadAddQuickTeleport(sQUICK_TELEPORT_INFO & rInfo)
{
	sQUICK_TELEPORT_INFO* pData = new sQUICK_TELEPORT_INFO;
	memcpy(pData, &rInfo, sizeof(sQUICK_TELEPORT_INFO));

	m_mapQuickTeleport.insert(std::make_pair(rInfo.bySlotNum, pData));
}


void CPlayer::AddDeathQuickTeleport()
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (GetCurWorld())
	{
		sQUICK_TELEPORT_INFO aData;
		aData.bySlotNum = 0;
		aData.worldTblidx = GetWorldTblidx();
		aData.mapNameTblidx = GetNaviEngine()->GetTextAllIndex(GetCurWorld()->GetNaviInstanceHandle(), GetCurLoc().x, GetCurLoc().z);
		GetCurLoc().CopyTo(aData.vLoc);

		SYSTEMTIME ti;
		GetLocalTime(&ti);
		aData.tSaveTime.day = (BYTE)ti.wDay;
		aData.tSaveTime.hour = (BYTE)ti.wHour;
		aData.tSaveTime.minute = (BYTE)ti.wMinute;
		aData.tSaveTime.month = (BYTE)ti.wMonth;
		aData.tSaveTime.second = (BYTE)ti.wSecond;
		aData.tSaveTime.year = ti.wYear;

		AddQuickTeleport(&aData, 0, false);
	}
}


void CPlayer::LoadQuickTeleportFromDB()
{
	ERR_LOG(LOG_USER, "Player %u load quick teleport from db.", GetCharID());

	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket pQry(sizeof(sGQ_QUICK_TELEPORT_INFO_LOAD_REQ));
	sGQ_QUICK_TELEPORT_INFO_LOAD_REQ * qRes = (sGQ_QUICK_TELEPORT_INFO_LOAD_REQ *)pQry.GetPacketData();
	qRes->wOpCode = GQ_QUICK_TELEPORT_INFO_LOAD_REQ;
	qRes->charId = GetCharID();
	pQry.SetPacketLen(sizeof(sGQ_QUICK_TELEPORT_INFO_LOAD_REQ));
	app->SendTo(app->GetQueryServerSession(), &pQry);
}

void CPlayer::LoadQuickTeleport()
{
	CNtlPacket packet(sizeof(sGU_QUICK_TELEPORT_LOAD_RES));
	sGU_QUICK_TELEPORT_LOAD_RES * res = (sGU_QUICK_TELEPORT_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = GU_QUICK_TELEPORT_LOAD_RES;
	res->wResultCode = GAME_SUCCESS;

	for (tQuickTeleportMap::iterator it = m_mapQuickTeleport.begin(); it != m_mapQuickTeleport.end(); it++)
	{
		res->asData[res->byCount++] = *it->second;
	}

	packet.SetPacketLen(sizeof(sGU_QUICK_TELEPORT_LOAD_RES));
	SendPacket(&packet);
}


//--------------------------------------------------------------------------------------//
//		DECREASE ITEM DURABILITY. USED AFTER DEATH
//--------------------------------------------------------------------------------------//
void CPlayer::DecreaseEquipmentDurability()
{
	CGameServer* app = (CGameServer*)g_pApp;
	BYTE byApplyCount = 0;

	CNtlPacket packet(sizeof(sGQ_ITEM_DUR_DOWN));
	sGQ_ITEM_DUR_DOWN * res = (sGQ_ITEM_DUR_DOWN *)packet.GetPacketData();
	res->wOpCode = GQ_ITEM_DUR_DOWN;
	res->handle = GetID();
	res->charId = GetCharID();

	memset(res->byDur, INVALID_BYTE, sizeof(res->byDur));

	GetPlayerItemContainer()->OnDecreaseEquipmentDurability(res->byDur, byApplyCount);

	if (byApplyCount > 0)
	{
		packet.SetPacketLen(sizeof(sGQ_ITEM_DUR_DOWN));
		app->SendTo(app->GetQueryServerSession(), &packet);

		CNtlPacket packet2(sizeof(sGU_ITEM_DUR_DOWN));
		sGU_ITEM_DUR_DOWN * res2 = (sGU_ITEM_DUR_DOWN *)packet2.GetPacketData();
		res2->wOpCode = GU_ITEM_DUR_DOWN;
		memcpy(res2->byDur, res->byDur, sizeof(res->byDur));
		packet2.SetPacketLen(sizeof(sGU_ITEM_DUR_DOWN));
		SendPacket(&packet2);
	}
}

