#include "stdafx.h"
#include "item.h"
#include "GameServer.h"
#include "GameMain.h"
#include "CPlayer.h"
#include "Monster.h"
#include "UseItemTable.h"
#include "NtlBitFlagManager.h"
#include "NtlRandom.h"
#include "DojoManager.h"
#include "ItemPet.h"
#include "NtlPacketGQ.h"
#include "NtlPacketGU.h"
#include "NtlPacketGT.h"
#include "NtlResultCode.h"
#include "TableContainerManager.h"
#include "ItemEnchantTable.h"
#include "SystemEffectTable.h"
#include "QuestProbabilityTable.h"
#include "ItemRecipeTable.h"
#include "CommonConfigTable.h"
#include "ScriptAlgoObjectManager.h"
#include "TimeLeapDungeon.h"
#include "TriggerObject.h"
#include "WpsAlgoObject.h"
#include "NtlNavi.h"
#include "TriggerManager.h"
#include "DojoWar.h"
#include "SpellAreaChecker.h"
#include "ItemOptionTable.h"


CItem::CItem()
{
	hHandle = INVALID_HOBJECT;
	item_data.itemNo = 0;
	item_data.itemId = 0;
	ZeroMemory(item_data.awchMaker, NTL_MAX_SIZE_CHAR_NAME + 1);
	m_pOwner = NULL;
	memset(&item_data, NULL, sizeof(sITEM_DATA));
	
	m_pProto = NULL;

	item_data.itemId = INVALID_ITEMID;

	m_bEquipped = false;
	m_bTrading = false;
	m_bIsLocked = false;

	m_byKeyPlace = INVALID_BYTE;
	m_byKeyPos = INVALID_BYTE;
	m_byApplyTargetCount = INVALID_BYTE;
	memset(m_ahApplyTarget, INVALID_HOBJECT, sizeof(m_ahApplyTarget));
	m_pUseItemTbldat = NULL;
}

CItem::~CItem()
{
	SetLocked(true);
	if (!IsTrading() && GetID() != INVALID_HOBJECT)
	{
		g_pObjectManager->DeleteUID(GetID());
	}
}


//set item tbldat
void CItem::SetTbldat(sITEM_TBLDAT* table)
{
	m_pProto = table;
}

//trade item
bool CItem::TradeItem(CPlayer* newowner)
{
	CGameServer* app = (CGameServer*)g_pApp;

	std::pair<BYTE, BYTE> inv = newowner->GetPlayerItemContainer()->GetEmptyInventory();

	if (inv.first != INVALID_BYTE)
	{
		SetID(g_pObjectManager->CreateUID());
		g_pItemManager->AddItem(this);
		SetTrading(true);

		SetPlace(inv.first);
		SetPos(inv.second);
		
		newowner->GetPlayerItemContainer()->AddReservedInventory(inv.first, inv.second);

		return true;
	}
	else
		ERR_LOG(LOG_USER, "Couldnt create item for player %u. No free slot found", newowner->GetCharID());

	return false;
}


//set count
bool CItem::SetCount(BYTE byCount, bool bIsNew/* = false*/, bool bQuery/* = false*/, bool bSendPacket/* = true*/)
{
	CGameServer* app = (CGameServer*)g_pApp;
	BYTE bymaxstack = m_pProto->byMax_Stack;

	item_data.byStackcount = byCount < bymaxstack ? byCount : bymaxstack;

	if (item_data.byStackcount == 0)
	{
		if (m_pOwner)
		{
			if (IsBagContainer(GetPlace()) && IsBag())
				m_pOwner->GetPlayerItemContainer()->RemoveActiveBag(GetPos());

			//del from map
			m_pOwner->GetPlayerItemContainer()->RemoveItem(GetPlace(), GetID());

			//query
			if (bQuery)
			{
				CNtlPacket packet2(sizeof(sGQ_ITEM_DELETE_REQ));
				sGQ_ITEM_DELETE_REQ * res2 = (sGQ_ITEM_DELETE_REQ *)packet2.GetPacketData();
				res2->wOpCode = GQ_ITEM_DELETE_REQ;
				res2->charId = m_pOwner->GetCharID();
				res2->itemId = GetItemID();
				res2->handle = GetID();
				res2->byPlace = GetPlace();
				res2->byPos = GetPos();
				packet2.SetPacketLen(sizeof(sGQ_ITEM_DELETE_REQ));
				app->SendTo(app->GetQueryServerSession(), &packet2);
			}

			if (bSendPacket)
			{
				//delete item packet
				CNtlPacket packet(sizeof(sGU_ITEM_DELETE));
				sGU_ITEM_DELETE * res = (sGU_ITEM_DELETE *)packet.GetPacketData();
				res->wOpCode = GU_ITEM_DELETE;
				res->hSrcItem = GetID();
				res->bySrcPlace = GetPlace();
				res->bySrcPos = GetPos();
				packet.SetPacketLen(sizeof(sGU_ITEM_DELETE));
				app->Send(m_pOwner->GetClientSessionID(), &packet);
			}
		}

		//destroy item
		g_pItemManager->DestroyItem(this);

		return true;
	}
	else if (m_pOwner)
	{
		if (bSendPacket)
		{
			//send item update packet
			CNtlPacket packet(sizeof(sGU_ITEM_STACK_UPDATE));
			sGU_ITEM_STACK_UPDATE * res = (sGU_ITEM_STACK_UPDATE *)packet.GetPacketData();
			res->wOpCode = GU_ITEM_STACK_UPDATE;
			res->bIsNew = bIsNew;
			res->byStack = item_data.byStackcount;
			res->hItemHandle = GetID();
			packet.SetPacketLen(sizeof(sGU_ITEM_STACK_UPDATE));
			app->Send(m_pOwner->GetClientSessionID(), &packet);
		}

		//query
		if (bQuery)
		{
			CNtlPacket packet2(sizeof(sGQ_ITEM_STACK_UPDATE_REQ));
			sGQ_ITEM_STACK_UPDATE_REQ * res2 = (sGQ_ITEM_STACK_UPDATE_REQ *)packet2.GetPacketData();
			res2->wOpCode = GQ_ITEM_STACK_UPDATE_REQ;
			res2->handle = m_pOwner->GetID();
			res2->charId = m_pOwner->GetCharID();
			res2->itemId = GetItemID();
			res2->hItem = GetID();
			res2->byPlace = GetPlace();
			res2->byPos = GetPos();
			res2->byStackCount = item_data.byStackcount;
			packet2.SetPacketLen(sizeof(sGQ_ITEM_STACK_UPDATE_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet2);
		}

		return true;
	}

	return false;
}
//remove from character
CItem* CItem::RemoveFromCharacter()
{
	if (!m_pOwner)
	{
		ERR_LOG(LOG_USER, "owner null");
		return (this);
	}

	CNtlPacket packet(sizeof(sGU_ITEM_DELETE));
	sGU_ITEM_DELETE * res = (sGU_ITEM_DELETE *)packet.GetPacketData();
	res->wOpCode = GU_ITEM_DELETE;
	res->hSrcItem = GetID();
	res->bySrcPlace = GetPlace();
	res->bySrcPos = GetPos();
	packet.SetPacketLen(sizeof(sGU_ITEM_DELETE));
	g_pApp->Send(m_pOwner->GetClientSessionID(), &packet);

	//del from map
	m_pOwner->GetPlayerItemContainer()->RemoveItem(GetPlace(), GetID());

	m_pOwner = NULL;
	item_data.byPlace = item_data.byPosition = 0;

	return (this);
}

//add to character
bool CItem::AddToCharacter(CPlayer* pPlayer)
{
	m_pOwner = pPlayer;
	item_data.charId = pPlayer->GetCharID();
	
	pPlayer->GetPlayerItemContainer()->AddItem(this);

	return true;
}

//set durability
void CItem::UpdateDurability(BYTE byDur)
{
	CGameServer* app = (CGameServer*)g_pApp;

	item_data.byCurrentDurability = byDur;

	CNtlPacket packet(sizeof(sGU_ITEM_DUR_UPDATE));
	sGU_ITEM_DUR_UPDATE * res = (sGU_ITEM_DUR_UPDATE *)packet.GetPacketData();
	res->wOpCode = GU_ITEM_DUR_UPDATE;
	res->byDur = byDur;
	res->hItemHandle = GetID();
	packet.SetPacketLen( sizeof(sGU_ITEM_DUR_UPDATE) );
	app->Send(m_pOwner->GetClientSessionID() , &packet );
}

bool CItem::IsExpired()
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (item_data.byDurationType != eDURATIONTYPE_FLATSUM)
		return false;

	return app->GetTime() >= item_data.nUseEndTime;
}

bool CItem::IsLocked(bool bCheckEquipped/* = true*/)
{
	if (m_bIsLocked || m_bTrading)
		return true;

	if (bCheckEquipped && m_bEquipped)
		return true;

	return false;
}

void CItem::SetItemData(sSHOP_BUY_INVEN * pData, ITEMID itemId)
{
	NTL_SAFE_WCSCPY(item_data.awchMaker, pData->awchMaker);
	item_data.bNeedToIdentify = false;
	item_data.byBattleAttribute = BATTLE_ATTRIBUTE_NONE;
	item_data.byCurrentDurability = pData->byCurrentDurability;
	item_data.byDurationType = pData->byDurationType;
	item_data.byGrade = pData->byGrade;
	item_data.byPlace = pData->byPlace;
	item_data.byPosition = pData->byPos;
	item_data.byRank = pData->byRank;
	item_data.byRestrictState = pData->byRestrictState;
	item_data.byStackcount = pData->byStack;
	//item_data.charId = //this is set inside function AddToCharacter
	item_data.itemId = itemId;
	item_data.itemNo = pData->tblItem;
	item_data.nUseEndTime = pData->nUseEndTime;
	item_data.nUseStartTime = pData->nUseStartTime;
	memcpy(&item_data.sOptionSet, &pData->sOptionSet, sizeof(pData->sOptionSet));
} 

void CItem::GenerateOptionSet(bool bEnchantAble)
{
	item_data.sOptionSet.Init();

	if (bEnchantAble)
	{
		if (GetRank() < ITEM_RANK_SUPERIOR || GetRank() > ITEM_RANK_LEGENDARY)
			return;
		if (GetTbldat()->byItem_Type > ITEM_TYPE_RING)
			return;

		//CREATE RANDOM OPTION
		float fRemainingWorth = Dbo_GetItemWorth(GetRank(), m_pProto->byNeed_Min_Level);
		CNtlBitFlagManager flag;
		flag.Create(256);

		for (int i = 0; i < NTL_MAX_SYSTEM_EFFECT_COUNT; i++)
		{
			//	printf("%u | item worth %f GetRank() %u %u\n", GetTblidx(), fRemainingWorth, GetRank(), m_pProto.byRank);
			if (!g_pTableContainer->GetItemEnchantTable()->ProcessRandomOption(GetTbldat(), item_data.byRank, i == 0, i == NTL_MAX_SYSTEM_EFFECT_COUNT - 1, &flag, &fRemainingWorth, item_data.sOptionSet.aRandomOption[i]))
				break;
		}
	}
	else
	{
		//SET DEFINED OPTION
		if (CanHaveOption() && GetOptionTblidx() != INVALID_TBLIDX)
			item_data.sOptionSet.aOptionTblidx[0] = GetOptionTblidx();
	}
}

void CItem::GenerateOptionSet(bool bEnchantAble, sITEM_TBLDAT* pTbldat, sITEM_DATA* pItemData)
{
	pItemData->sOptionSet.Init();

	if (bEnchantAble)
	{
		if (CanChangeOption(pTbldat->byEquip_Type, pItemData->byRank, pItemData->byRestrictState, pTbldat->byNeed_Min_Level) == false)
			return;

		//CREATE RANDOM OPTION
		float fRemainingWorth = Dbo_GetItemWorth(pItemData->byRank, pTbldat->byNeed_Min_Level);
		CNtlBitFlagManager flag;
		flag.Create(256);

		for (int i = 0; i < NTL_MAX_SYSTEM_EFFECT_COUNT; i++)
		{
			//	printf("%u | item worth %f GetRank() %u %u\n", GetTblidx(), fRemainingWorth, GetRank(), m_pProto.byRank);
			if (!g_pTableContainer->GetItemEnchantTable()->ProcessRandomOption(pTbldat, pItemData->byRank, i == 0, i == NTL_MAX_SYSTEM_EFFECT_COUNT - 1, &flag, &fRemainingWorth, pItemData->sOptionSet.aRandomOption[i]))
				break;
		}
	}
	else
	{
		//SET DEFINED OPTION
		if (pTbldat->bIsCanHaveOption && pTbldat->Item_Option_Tblidx != INVALID_TBLIDX)
			pItemData->sOptionSet.aOptionTblidx[0] = pTbldat->Item_Option_Tblidx;
	}
}

void CItem::SetOptionSet(sITEM_OPTION_SET & rSet)
{
	memcpy(&item_data.sOptionSet, &rSet, sizeof(sITEM_OPTION_SET));
}

bool CItem::ChangeOption(WORD wOptionIndex, sITEM_TBLDAT* pTbldat, BYTE byRank, sITEM_OPTION_SET* sOptionSet)
{
	float fRemainingWorth = Dbo_GetItemWorth(byRank, pTbldat->byNeed_Min_Level);
	
	if (fRemainingWorth == 0.0f)
		return false;

	CNtlBitFlagManager flag;
	flag.Create(256);

	if (wOptionIndex == INVALID_WORD) //brown box. Re-roll all options
	{
		sOptionSet->Init(); //reset all

		for (int i = 0; i < NTL_MAX_SYSTEM_EFFECT_COUNT; i++)
		{
			g_pTableContainer->GetItemEnchantTable()->ProcessRandomOption(pTbldat, byRank, i == 0, i == NTL_MAX_SYSTEM_EFFECT_COUNT - 1, &flag, &fRemainingWorth, sOptionSet->aRandomOption[i]);
		}
	}
	else //silver box. Keep the selected attrbute but random stat
	{
		sOptionSet->Init(); //reset all
		
		sOptionSet->aRandomOption[0].optionValue = 0;
		sOptionSet->aRandomOption[0].wOptionIndex = wOptionIndex;

		for (int i = 0; i < NTL_MAX_SYSTEM_EFFECT_COUNT; i++)
		{
			g_pTableContainer->GetItemEnchantTable()->ProcessRandomOption(pTbldat, byRank, i == 0, i == NTL_MAX_SYSTEM_EFFECT_COUNT - 1, &flag, &fRemainingWorth, sOptionSet->aRandomOption[i]);
		}
	}

	return true;
}

bool CItem::CanTrade()
{
	if (IsLocked())
		return false;

	if (NeedToIdentify() == false && GetRestrictState() != ITEM_RESTRICT_STATE_TYPE_SEAL)
	{
		if (BIT_FLAG_TEST(GetTbldat()->wFunction_Bit_Flag, ITEM_FUNC_BIT_FLAG_TRADE_FORBIDDEN))
		{
			return false;
		}
		else if (GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_LIMIT || GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_CHARACTER_GET
			|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_ACCOUNT_GET || GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_BATTLE_DRAGONBALL
			|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_QUEST || GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_INSERT_BEAD)
			return false;

		else if (GetDurationtype() != eDURATIONTYPE_NORMAL)
			return false;
	}

	if(GetPlace() < CONTAINER_TYPE_BAG1 || GetPlace() > CONTAINER_TYPE_BAG5)
		return false;

	return true;
}

bool CItem::CanSell()
{
	if (IsLocked())
		return false;

	if (GetGrade() > 0)
		return false;

	if (GetTbldat()->byEquip_Type == EQUIP_TYPE_COSTUME)
		return false;

	if (!NeedToIdentify())
	{
		if (BIT_FLAG_TEST(GetTbldat()->wFunction_Bit_Flag, ITEM_FUNC_BIT_FLAG_SELL_FORBIDDEN))
			return false;

		else if (GetDurationtype() != eDURATIONTYPE_NORMAL)
			return false;
	}

	return true;
}

bool CItem::CanAuctionhouse()
{
	if (NeedToIdentify())
		return false;

	return CanTrade(); //if cant trade then we cant put into auctionhouse !
}

bool CItem::CanMail()
{
	return CanTrade(); //if cant trade then we cant put into mail !
}

bool CItem::CanPrivateshop()
{
	return CanTrade(); //if cant trade then we cant put into private shop !
}

bool CItem::CanDelete()
{
	if (IsLocked() || BIT_FLAG_TEST(GetTbldat()->wFunction_Bit_Flag, ITEM_FUNC_BIT_FLAG_DELETE_FORBIDDEN))
		return false;

	return true;
}

bool CItem::CanWarehouse(bool bIsSharedBank)
{
	if (IsLocked())
		return false;

	else if (!NeedToIdentify())
	{
		if (BIT_FLAG_TEST(GetTbldat()->wFunction_Bit_Flag, ITEM_FUNC_BIT_FLAG_STORE_FORBIDDEN))
			return false;

		else if (GetDurationtype() != eDURATIONTYPE_NORMAL)
			return false;

		else if (GetTblidx() >= 200041 && GetTblidx() <= 200048) // 1-7 Star Event Dragon Ball
			return false;

		if (bIsSharedBank)
		{
			if (GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_LIMIT
				|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_CHARACTER_GET
				|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_USE
				|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_DRAGONBALL
				|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_BATTLE_DRAGONBALL
				|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_QUEST
				|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_INSERT_BEAD)
				return false;
		}
	}

	if (GetPlace() == CONTAINER_TYPE_BAGSLOT)
		return false;

	return true;
}

bool CItem::CanDojowarehouse()
{
	if (GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_LIMIT
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_CHARACTER_GET
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_ACCOUNT_GET
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_USE
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_DRAGONBALL
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_BATTLE_DRAGONBALL
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_QUEST
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_INSERT_BEAD)
		return false;

	return CanWarehouse(false); //if cant put in normal warehouse then cant put into dojo warehouse
}

bool CItem::CanGuildWarehouse()
{
	if (GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_LIMIT
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_CHARACTER_GET
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_ACCOUNT_GET
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_USE
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_DRAGONBALL
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_BATTLE_DRAGONBALL
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_QUEST
		|| GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_INSERT_BEAD)
		return false;

	return CanWarehouse(false); //if cant put in normal warehouse then cant put into dojo warehouse
}


void CItem::UseItem(BYTE byKeyPlace, BYTE byKeyPos, BYTE byApplyTargetCount, HOBJECT* ahApplyTarget, sUSE_ITEM_TBLDAT* pUseItemTbldat, HOBJECT hTarget)
{
	m_byKeyPlace = byKeyPlace;
	m_byKeyPos = byKeyPos;

	m_pUseItemTbldat = pUseItemTbldat;

	UseItemCheckTarget(hTarget, byApplyTargetCount, ahApplyTarget);

	if (pUseItemTbldat->dwCastingTimeInMilliSecs > 0)
	{
		SetLocked(true);

		float fCastingTime = (float)pUseItemTbldat->dwCastingTimeInMilliSecs;
		
		if (m_pOwner->GetCharAtt()->GetCastingTimeChangePercent() != 0.0f)
			fCastingTime += fCastingTime * m_pOwner->GetCharAtt()->GetCastingTimeChangePercent() / 100.0f;

		m_pOwner->SendCharStateItemCasting((DWORD)fCastingTime, (DWORD)fCastingTime, GetTblidx(), hTarget);
	}
	else //if no casting then use immidiently
	{
		Cast();
	}
}

void CItem::UseItemCheckTarget(HOBJECT hAppointTargetHandle, BYTE byApplyTargetCount, HOBJECT * ahApplyTarget)
{
	m_byApplyTargetCount = byApplyTargetCount;
	
	if (m_byApplyTargetCount == 0 && m_pUseItemTbldat->byApply_Target != DBO_SKILL_APPLY_TARGET_SELF) // required for item to resurrect a single party member
	{
		m_byApplyTargetCount = 1;
		ahApplyTarget[0] = hAppointTargetHandle;
	}
	//printf("m_byApplyTargetCount:%u, byApplyTargetCount:%u, byApply_Target:%u \n", m_byApplyTargetCount, byApplyTargetCount, m_pUseItemTbldat->byApply_Target);
	CSpellAreaChecker rSpellAreaChecker;
	rSpellAreaChecker.Create();

	CSpawnObject *pAppointTarget = NULL;
	//printf("m_pUseItemTbldat->byAppoint_Target: %u \n", m_pUseItemTbldat->byAppoint_Target);
	if (m_pUseItemTbldat->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_SELF)
	{
		if (m_byApplyTargetCount <= 1)
		{
			m_byApplyTargetCount = 1;
			m_ahApplyTarget[0] = m_pOwner->GetID();

			return;
		}
	}

	if (m_pUseItemTbldat->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY) // trigger object
	{
		pAppointTarget = m_pOwner->GetCurWorld()->FindStaticObject(hAppointTargetHandle);

		if (pAppointTarget)
		{
			m_byApplyTargetCount = 1;
			m_ahApplyTarget[0] = hAppointTargetHandle;

			return;
		}
	}
	else
	{
		pAppointTarget = (hAppointTargetHandle == INVALID_HOBJECT) ? m_pOwner : g_pObjectManager->GetChar(hAppointTargetHandle);
	}

	if (pAppointTarget == NULL)
	{
		m_byApplyTargetCount = 0;
		return;
	}

	rSpellAreaChecker.PrepareForSelection(m_pOwner, pAppointTarget, m_pUseItemTbldat->byApply_Range, (float)m_pUseItemTbldat->byApply_Area_Size_1, (float)m_pUseItemTbldat->byApply_Area_Size_2);

	sSKILL_TARGET_LIST targetList;

	for (BYTE i = 0; i < m_byApplyTargetCount; i++)
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(ahApplyTarget[i]);
		if (!pTarget || !pTarget->IsInitialized() || pTarget->GetCurWorld() == NULL)
			continue;

		switch (m_pUseItemTbldat->byApply_Target)
		{
			case DBO_SKILL_APPLY_TARGET_SELF:
			{
				targetList.AddTarget(m_pOwner->GetID());
			}
			break;
			case DBO_SKILL_APPLY_TARGET_ENEMY:
			{
				if (rSpellAreaChecker.IsObjectInApplyRange(pTarget, NULL) && m_pOwner->IsInRange(pTarget, m_pUseItemTbldat->fUse_Range_Max + 10.f) == true)
				{
					targetList.AddTarget(ahApplyTarget[i]);
				}
			}
			break;

			case DBO_SKILL_APPLY_TARGET_ALLIANCE: 
			{
				if (rSpellAreaChecker.IsObjectInApplyRange(pTarget, NULL) && m_pOwner->IsInRange(pTarget, m_pUseItemTbldat->fUse_Range_Max + 10.f) == true)
				{
					if (pTarget->IsNPC())
					{
						targetList.AddTarget(ahApplyTarget[i]);
					}
					else if (pTarget->IsPC())
					{
						CPlayer* pPcTarget = (CPlayer*)pTarget;

						if(pPcTarget->GetPcIsFreeBattle() == false && pPcTarget->IsPvpZone() == false)
							targetList.AddTarget(ahApplyTarget[i]);
					}
				}
			}
			break;

			case DBO_SKILL_APPLY_TARGET_PARTY:
			{
				if (rSpellAreaChecker.IsObjectInApplyRange(pTarget, NULL) && m_pOwner->IsInRange(pTarget, m_pUseItemTbldat->fUse_Range_Max + 10.f) == true)
				{
					if(pTarget->IsPC() && pTarget->GetPartyID() != INVALID_PARTYID && pTarget->GetPartyID() == m_pOwner->GetPartyID())
					{
						targetList.AddTarget(ahApplyTarget[i]);
					}
				}
			}
			break;

			case DBO_SKILL_APPLY_TARGET_MOB_PARTY:
			{
				ERR_LOG(LOG_SYSTEM, "DBO_SKILL_APPLY_TARGET_MOB_PARTY dont know. GetTblidx %u", GetTblidx());
			}
			break;

			case DBO_SKILL_APPLY_TARGET_SUMMON:
			{
				if (pTarget->IsSummonPet())
					targetList.AddTarget(ahApplyTarget[i]);
			}
			break;

			case DBO_SKILL_APPLY_TARGET_ANY_NPC:
			{
				if (rSpellAreaChecker.IsObjectInApplyRange(pTarget, NULL) && m_pOwner->IsInRange(pTarget, m_pUseItemTbldat->fUse_Range_Max + 10.f) == true)
					if (pTarget->IsNPC())
						targetList.AddTarget(ahApplyTarget[i]);
			}
			break;

			case DBO_SKILL_APPLY_TARGET_ANY_MOB:
			{
				if (rSpellAreaChecker.IsObjectInApplyRange(pTarget, NULL) && m_pOwner->IsInRange(pTarget, m_pUseItemTbldat->fUse_Range_Max + 10.f) == true)
					if (pTarget->IsMonster())
						targetList.AddTarget(ahApplyTarget[i]);
			}
			break;

			case DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE:
			{
				ERR_LOG(LOG_SYSTEM, "DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE dont know. Skill id %u", GetTblidx());
			}
			break;

			default:
			{
				ERR_LOG(LOG_SYSTEM, "fail : switch( m_pUseItemTbldat->byApply_Target(%u) )", m_pUseItemTbldat->byApply_Target);
			}
			break;
		}
	}

	m_byApplyTargetCount = targetList.byTargetCount;
	//printf("targetList.byTargetCount: %u \n", targetList.byTargetCount);
	memcpy(m_ahApplyTarget, targetList.ahTarget, sizeof(m_ahApplyTarget));
}


void CItem::Cast()
{
	CGameServer* app = (CGameServer*)g_pApp;

	m_pOwner->SetItemCast(INVALID_HOBJECT);
	SetLocked(false);

	CNtlPacket packet(sizeof(sGU_CHAR_ACTION_ITEM));
	sGU_CHAR_ACTION_ITEM * res = (sGU_CHAR_ACTION_ITEM *)packet.GetPacketData();
	res->wOpCode = GU_CHAR_ACTION_ITEM;
	res->handle = m_pOwner->GetID();
	res->wResultCode = GAME_SUCCESS;
	res->dwLpEpEventId = g_pObjectManager->AcquireLpEpEventID();
	res->itemTblidx = GetTblidx();

	CItem* pKeyItem = NULL;
	if (m_byKeyPlace != INVALID_BYTE && m_byKeyPos != INVALID_BYTE)
		pKeyItem = m_pOwner->GetPlayerItemContainer()->GetItem(m_byKeyPlace, m_byKeyPos);


	bool bAddBuff = true;
	int i = 0;
	BYTE bySkillResultCount = 0;

	eSYSTEM_EFFECT_CODE effectCode[NTL_MAX_EFFECT_IN_ITEM];
	effectCode[0] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(m_pUseItemTbldat->aSystem_Effect[0]);
	effectCode[1] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(m_pUseItemTbldat->aSystem_Effect[1]);

	sBUFF_INFO buffInfo;
	buffInfo.buffIndex = INVALID_BYTE;
	buffInfo.sourceTblidx = GetTblidx();
	buffInfo.dwTimeRemaining = m_pUseItemTbldat->dwKeepTimeInMilliSecs;
	buffInfo.dwInitialDuration = m_pUseItemTbldat->dwKeepTimeInMilliSecs;
	buffInfo.bySourceType = DBO_OBJECT_SOURCE_ITEM;

	for (; i < m_byApplyTargetCount; i++)
	{
		bool bSuccess = true;

		for (int e = 0; e < NTL_MAX_EFFECT_IN_SKILL; e++)
		{
			if (effectCode[e] == INVALID_SYSTEM_EFFECT_CODE)
				break;
		//	printf("CItem::Cast(): effectCode[e] %u \n", effectCode[e]);
			switch (effectCode[e])
			{
				case ACTIVE_DIRECT_HEAL:
				{
					CCharacter* pTarget = g_pObjectManager->GetPC(m_ahApplyTarget[i]);
					if (pTarget && pTarget->IsInitialized())
					{
						pTarget->SetLpEpEventID(res->dwLpEpEventId);

						res->aSkillResult[bySkillResultCount].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DH_HOT;

						if (m_pUseItemTbldat->abySystem_Effect_Type[e] == SYSTEM_EFFECT_APPLY_TYPE_MAX_LP)
						{
							res->aSkillResult[bySkillResultCount].effectResult[e].DH_HOT_fHealAmount_Lp = (float)(pTarget->GetMaxLP() * m_pUseItemTbldat->aSystem_Effect_Value[e] / 100);
							m_pOwner->UpdateCurLP((int)res->aSkillResult[bySkillResultCount].effectResult[e].DH_HOT_fHealAmount_Lp, true, false);
						}
						else
						{
							res->aSkillResult[bySkillResultCount].effectResult[e].DH_HOT_fHealAmount_Lp = (float)(m_pUseItemTbldat->aSystem_Effect_Value[e]);
							m_pOwner->UpdateCurLP((int)m_pUseItemTbldat->aSystem_Effect_Value[e], true, false);
						}
					}
				}
				break;

				case ACTIVE_DIRECT_EP_HEAL:
				{
					CCharacter* pTarget = g_pObjectManager->GetPC(m_ahApplyTarget[i]);
					if (pTarget && pTarget->IsInitialized())
					{
						pTarget->SetLpEpEventID(res->dwLpEpEventId);

						res->aSkillResult[bySkillResultCount].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DH_HOT;

						if (m_pUseItemTbldat->abySystem_Effect_Type[e] == SYSTEM_EFFECT_APPLY_TYPE_MAX_EP)
						{
							res->aSkillResult[bySkillResultCount].effectResult[e].DH_HOT_fHealAmount_Ep = (float)(pTarget->GetMaxEP() * m_pUseItemTbldat->aSystem_Effect_Value[e] / 100);
							m_pOwner->UpdateCurEP((int)res->aSkillResult[bySkillResultCount].effectResult[e].DH_HOT_fHealAmount_Ep, true, false);
						}
						else
						{
							res->aSkillResult[bySkillResultCount].effectResult[e].DH_HOT_fHealAmount_Ep = (float)m_pUseItemTbldat->aSystem_Effect_Value[e];
							m_pOwner->UpdateCurEP((WORD)m_pUseItemTbldat->aSystem_Effect_Value[e], true, false);
						}
					}
				}
				break;

				case ACTIVE_HEAL_OVER_TIME:
				{
					CCharacter* pTarget = g_pObjectManager->GetPC(m_ahApplyTarget[i]);
					if (pTarget && pTarget->IsInitialized())
					{
						pTarget->SetLpEpEventID(res->dwLpEpEventId);

						res->aSkillResult[bySkillResultCount].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DH_HOT;

						buffInfo.aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_HOT;
						buffInfo.aBuffParameter[e].buffParameter.dwRemainTime = m_pUseItemTbldat->dwKeepTimeInMilliSecs;

						res->aSkillResult[bySkillResultCount].effectResult[e].DH_HOT_fHealAmount_Lp = (float)m_pUseItemTbldat->aSystem_Effect_Value[e];

						if (!m_pOwner->IsSitting()) //Bug fix. When player use emote and then HOT item the client dont send sUG_CHAR_TOGG_SITDOWN packet to server.
						{
							m_pOwner->GetStateManager()->ChangeCharState(CHARSTATE_SITTING, NULL, true);
						}

						buffInfo.aBuffParameter[e].buffParameter.fParameter = (float)m_pUseItemTbldat->aSystem_Effect_Value[e];
					}
				}
				break;

				case ACTIVE_EP_OVER_TIME:
				{
					CCharacter* pTarget = g_pObjectManager->GetPC(m_ahApplyTarget[i]);
					if (pTarget && pTarget->IsInitialized())
					{
						pTarget->SetLpEpEventID(res->dwLpEpEventId);
						res->aSkillResult[bySkillResultCount].effectResult[e].eResultType = DBO_SYSTEM_EFFECT_RESULT_TYPE_DH_HOT;

						buffInfo.aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_HOT;
						buffInfo.aBuffParameter[e].buffParameter.dwRemainTime = m_pUseItemTbldat->dwKeepTimeInMilliSecs;

						res->aSkillResult[bySkillResultCount].effectResult[e].DH_HOT_fHealAmount_Ep = (float)m_pUseItemTbldat->aSystem_Effect_Value[e];

						if (!m_pOwner->IsSitting()) //Bug fix. When player use emote and then HOT item the client dont send sUG_CHAR_TOGG_SITDOWN packet to server.
						{
							m_pOwner->GetStateManager()->ChangeCharState(CHARSTATE_SITTING, NULL, true);
						}

						buffInfo.aBuffParameter[e].buffParameter.fParameter = (float)m_pUseItemTbldat->aSystem_Effect_Value[e];
					}
				}
				break;

				case ACTIVE_TELEPORT_BIND:
				{
					if (m_pOwner->GetCurWorld() != NULL && m_pOwner->GetCurWorld()->GetRuleType() == GAMERULE_NORMAL)
					{
						if (m_pOwner->GetDragonballScrambleBallFlag() == 0)
						{
							CNtlVector vBindLoc(m_pOwner->GetBindLoc());
							m_pOwner->StartTeleport(vBindLoc, m_pOwner->GetCurDir(), m_pOwner->GetBindWorldID(), TELEPORT_TYPE_TELEPOPO);
						}
						else res->wResultCode = GAME_CAN_NOT_TELEPORT;
					}
					else res->wResultCode = GAME_ITEM_CANT_USE_NOW;
				}
				break;

				case ACTIVE_POISON_CURE:
				{
					if (m_pOwner->GetStateManager()->IsCharCondition(CHARCOND_POISON))
					{
						if (Dbo_CheckProbabilityF((float)m_pUseItemTbldat->aSystem_Effect_Value[e]))
							m_pOwner->GetBuffManager()->EndBuff(ACTIVE_POISON);
					}
				}
				break;

				case ACTIVE_STOMACHACHE_CURE:
				{
					if (m_pOwner->GetStateManager()->IsCharCondition(CHARCOND_STOMACHACHE))
					{
						if (Dbo_CheckProbabilityF((float)m_pUseItemTbldat->aSystem_Effect_Value[e]))
							m_pOwner->GetBuffManager()->EndBuff(ACTIVE_STOMACHACHE);
					}
				}
				break;

				case ACTIVE_PARALYZE_CURE:
				{
					if (m_pOwner->GetCharStateID() == CHARSTATE_PARALYZED)
					{
						if (Dbo_CheckProbabilityF((float)m_pUseItemTbldat->aSystem_Effect_Value[e]))
							m_pOwner->GetBuffManager()->EndBuff(ACTIVE_PARALYZE);
					}
						
				}
				break;

				case ACTIVE_SUPER_SAIYAN:
				case ACTIVE_PURE_MAJIN:
				case ACTIVE_GREAT_NAMEK:
				{
					if (m_pOwner->CanTransform(effectCode[e], (TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[e], false))
					{
						buffInfo.aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DEFAULT;
						buffInfo.aBuffParameter[e].buffParameter.fParameter = (float)m_pUseItemTbldat->aSystem_Effect_Value[e];

						int nOldMaxLP = m_pOwner->GetMaxLP();
						WORD wOldMaxEP = m_pOwner->GetMaxEP();

						m_pOwner->GetBuffManager()->RegisterSubBuff(&buffInfo, effectCode, m_pOwner->GetID(), m_pUseItemTbldat->byBuff_Group, res->wResultCode, m_pUseItemTbldat->abySystem_Effect_Type);

						if (m_pOwner->GetMaxLP() - nOldMaxLP > 0)
							m_pOwner->UpdateCurLP(m_pOwner->GetMaxLP() - nOldMaxLP, true, false);

						if (m_pOwner->GetMaxEP() - wOldMaxEP > 0)
							m_pOwner->UpdateCurEP(m_pOwner->GetMaxEP() - wOldMaxEP, true, false);
						
						res->aSkillResult[bySkillResultCount].effectResult[e].fResultValue = buffInfo.aBuffParameter[e].buffParameter.fParameter;
					}
					else res->wResultCode = GAME_SKILL_ALREADY_TRANSFORMED;
				}
				break;

				case ACTIVE_RP_UP:
				{
					res->aSkillResult[bySkillResultCount].effectResult[e].fResultValue = (float)m_pUseItemTbldat->aSystem_Effect_Value[e];
					m_pOwner->UpdateCurRP((WORD)m_pUseItemTbldat->aSystem_Effect_Value[e], true, false);
				}
				break;

				case ACTIVE_RESCUE:
				{
					CCharacter* pTarget = g_pObjectManager->GetPC(m_ahApplyTarget[i]);
					if (pTarget && pTarget->IsInitialized())
					{
						pTarget->SetLpEpEventID(res->dwLpEpEventId);

						if (pTarget->GetCharStateID() == CHARSTATE_FAINTING)
						{
							if (pTarget->IsPC())
							{
								if (pTarget->GetCurWorld() && pTarget->GetCurWorld()->GetRuleType() != GAMERULE_RANKBATTLE && pTarget->GetCurWorld()->GetRuleType() != GAMERULE_MINORMATCH && pTarget->GetCurWorld()->GetRuleType() != GAMERULE_MAJORMATCH && pTarget->GetCurWorld()->GetRuleType() != GAMERULE_FINALMATCH && pTarget->GetCurWorld()->GetRuleType() != GAMERULE_CCBATTLEDUNGEON)
								{
									pTarget->Revival(pTarget->GetCurLoc(), pTarget->GetWorldID(), REVIVAL_TYPE_RESCUED);
									pTarget->UpdateCurLP((int)m_pUseItemTbldat->aSystem_Effect_Value[e], true, false);
								}
								else res->wResultCode = GAME_ITEM_CANT_USE_INVALID_WORLD;
							}
							else res->wResultCode = GAME_ITEM_USE_ONLY_PLYAER;
						}
						else res->wResultCode = GAME_ITEM_USEFUL_ONLY_WHEN_FAINTING;
					}
					else res->wResultCode = GAME_FAIL;
				}
				break;

				case ACTIVE_QUEST_TRIGGER:
				{
					CCharacter* pTarget = g_pObjectManager->GetPC(m_ahApplyTarget[i]);
					if (pTarget && pTarget->IsInitialized())
					{
						pTarget->SetLpEpEventID(res->dwLpEpEventId);

						QUESTID qId = (QUESTID)m_pUseItemTbldat->aSystem_Effect_Value[e];

						if (g_pTriggerManager->FindQuestFromTS(qId) == NULL)
							res->wResultCode = GAME_QUEST_NOT_EXIST;
						else if (m_pOwner->GetQuests()->CheckQuestCounter(qId) == false)
							res->wResultCode = GAME_QUEST_COUNT_OVER;
						else if (m_pOwner->GetQuests()->GetQuestProgress(qId) != NULL)
							res->wResultCode = GAME_QUEST_ALREADY_EXIST;
						else
						{
							CNtlPacket itemUsePacket(sizeof(sGU_TS_EXCUTE_TRIGGER));
							sGU_TS_EXCUTE_TRIGGER* iuRes = (sGU_TS_EXCUTE_TRIGGER*)itemUsePacket.GetPacketData();
							iuRes->wOpCode = GU_TS_EXCUTE_TRIGGER;
							iuRes->hExcuteItem = GetID();
							iuRes->tId = (NTL_TS_T_ID)m_pUseItemTbldat->aSystem_Effect_Value[e];
							iuRes->byTsType = TS_TYPE_QUEST_CS;
							itemUsePacket.SetPacketLen(sizeof(sGU_TS_EXCUTE_TRIGGER));
							pTarget->SendPacket(&itemUsePacket);
						}
					}
					else res->wResultCode = GAME_ITEM_CANNOT_USE_INVALID_TARGET;
				}
				break;

				case ACTIVE_OBJECT_TRIGGER:
				{	
					CTriggerObject* obj = m_pOwner->GetCurWorld()->FindStaticObject(m_ahApplyTarget[i]);
					if (obj && BIT_FLAG_TEST(obj->GetSubState(), TOBJECT_SUBSTATE_FLAG_SHOW))
					{
						if (obj->IsBeingExecuted() == false)
						{
							if (m_pOwner->IsInRange(obj->GetCurLoc(), 10.f))
							{
								if (m_pOwner->GetAspectStateId() == ASPECTSTATE_VEHICLE)
									m_pOwner->EndVehicle(GAME_VEHICLE_END_BY_FORCED);

								m_pOwner->SetUseItemToExcuteTriggerObject(true);

								if(obj->StartExecuting(m_pOwner, true, (QUESTID)m_pUseItemTbldat->aSystem_Effect_Value[e]) == false)
									res->wResultCode = GAME_FAIL;

								m_pOwner->SetUseItemToExcuteTriggerObject(false);
							}
							else res->wResultCode = GAME_TARGET_TOO_FAR;
						}
						else res->wResultCode = GAME_ITEM_CANNOT_USE_INVALID_TARGET;
					}
					else res->wResultCode = GAME_ITEM_CANNOT_USE_INVALID_TARGET;
				}
				break;

				case ACTIVE_GAMBLE_ITEM:
				{
					//printf("(TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[0] %u \n", (TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[0]);
					sQUEST_PROBABILITY_TBLDAT * qProbTbl = (sQUEST_PROBABILITY_TBLDAT*)g_pTableContainer->GetQuestProbabilityTable()->FindData((TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[0]);
					if (qProbTbl)
					{
						//printf("qProbTbl->eUseType %u \n", qProbTbl->eUseType);
						switch (qProbTbl->eUseType)
						{
							case PROBABILITY_USE_TYPE_GAMBLE_SHOP: //PLAYER ONLY RECEIVE 1 REWARD
							{
								if (m_pOwner->GetPlayerItemContainer()->CountEmptyInventory() == 0)
									res->wResultCode = GAME_ITEM_INVEN_FULL;
								else
								{
									bool bRewardReceived = false;
									int nCount = 0;
									int nReward[NTL_QUEST_PROBABILITY_MAX_COUNT];
									memset(nReward, NULL, sizeof(nReward));

									REPEAT_LOOP2:
									for (int c = 0; c < qProbTbl->byCount; c++)
									{
										if (nCount >= NTL_QUEST_PROBABILITY_MAX_COUNT)
											nCount = NTL_QUEST_PROBABILITY_MAX_COUNT - 1;

										if (qProbTbl->asProbabilityData[c].byType == eREWARD_TYPE_NORMAL_ITEM)
										{
											if (qProbTbl->asProbabilityData[c].tblidx != INVALID_TBLIDX)
											{
												if (Dbo_CheckProbability(qProbTbl->asProbabilityData[c].dwRate / 10000))
												{
													nReward[nCount++] = c;
													bRewardReceived = true;
													continue;
												}
											}
											else
											{
												ERR_LOG(LOG_GENERAL, "ERROR type %u not set yet. Item tblidx %u", qProbTbl->asProbabilityData[c].byType, GetTblidx());
												res->wResultCode = GAME_FAIL;
												break;
											}
										}
										else
										{
											ERR_LOG(LOG_GENERAL, "ERROR type %u not set yet. Item tblidx %u", qProbTbl->asProbabilityData[c].byType, GetTblidx());
											res->wResultCode = GAME_FAIL;
											break;
										}

										if (c == qProbTbl->byCount - 1 && !bRewardReceived)
										{
											ERR_LOG(LOG_GENERAL, "Repeat loop. Tblidx %u", GetTblidx());
											goto REPEAT_LOOP2;
										}
									}
									
									if (res->wResultCode == GAME_SUCCESS && bRewardReceived == true)
									{
										int nRandomReward = RandomRange(0, nCount-1);

										if (nRandomReward >= NTL_QUEST_PROBABILITY_MAX_COUNT)
											nRandomReward = NTL_QUEST_PROBABILITY_MAX_COUNT - 1;

										if (&qProbTbl->asProbabilityData[nReward[nRandomReward]])
										{
											int byItemCount = RandomRange((int)qProbTbl->asProbabilityData[nReward[nRandomReward]].dwMinValue, (int)qProbTbl->asProbabilityData[nReward[nRandomReward]].dwMaxValue);

											if (g_pItemManager->CreateItem(m_pOwner, qProbTbl->asProbabilityData[nReward[nRandomReward]].tblidx, (BYTE)byItemCount) == false)
												res->wResultCode = GAME_FAIL;
										}
										else
										{
											ERR_LOG(LOG_GENERAL, "ERROR!!! &qProbTbl->asProbabilityData[nReward[nRandomReward]] is null !!! Item Tblidx %u nRandomReward %u", GetTblidx(), nRandomReward);
											res->wResultCode = GAME_FAIL;
										}
									}

								}
							}
							break;
							case PROBABILITY_USE_TYPE_GAMBLE_ITEM:  //PLAYER WILL RECEIVE MULTIPLE REWARDS
							{
								int nReward[NTL_QUEST_PROBABILITY_MAX_COUNT];
								int ii = 0;

							REPEAT_PROBABILITY_USE_TYPE_GAMBLE_ITEM:

								for (int c = 0; c < qProbTbl->byCount; c++)
								{
									if (qProbTbl->asProbabilityData[c].byType == eREWARD_TYPE_NORMAL_ITEM)
									{
										if (qProbTbl->asProbabilityData[c].tblidx != INVALID_TBLIDX)
										{
											if (Dbo_CheckProbability(qProbTbl->asProbabilityData[c].dwRate / 10000) == true)
											{
												nReward[ii++] = c;
											}
										}
									}
									else
									{
										ERR_LOG(LOG_GENERAL, "ERROR: qProbTbl->asProbabilityData[c].byType %u does not exist", qProbTbl->asProbabilityData[c].byType);
									}
								}

								if (ii == 0 && qProbTbl->byCount > 0)
								{
									printf("repeat loop x1041 \n");
									goto REPEAT_PROBABILITY_USE_TYPE_GAMBLE_ITEM;
								}

								//printf("reward count %i \n", i);
								if (m_pOwner->GetPlayerItemContainer()->CountEmptyInventory() < ii)
								{
									res->wResultCode = GAME_ITEM_INVEN_FULL;
									break;
								}
								else
								{
									for (int c = 0; c < ii; c++)
									{
									//	printf("reward idx %u chance %u/%u \n", qProbTbl->asProbabilityData[nReward[c]].tblidx, qProbTbl->asProbabilityData[nReward[c]].dwMinValue, qProbTbl->asProbabilityData[nReward[c]].dwMaxValue);
										BYTE byItemCount = RandomRange(qProbTbl->asProbabilityData[nReward[c]].dwMinValue, qProbTbl->asProbabilityData[nReward[c]].dwMaxValue);
										g_pItemManager->CreateItem(m_pOwner, qProbTbl->asProbabilityData[nReward[c]].tblidx, byItemCount);
									}
								}
							}
							break;

							default:{ ERR_LOG(LOG_SYSTEM, "qProbTbl->eUseType %u not set ", qProbTbl->eUseType); res->wResultCode = GAME_FAIL; }break;
						}
					}
					else res->wResultCode = GAME_FAIL;
				}
				break;

				case ACTIVE_VEHICLE:
				{
					if (m_pOwner->GetAspectStateId() != ASPECTSTATE_INVALID)
						res->wResultCode = GAME_SKILL_CANT_USE_VEHICLE_WHEN_TRANSFORMED;
					else if (m_pOwner->GetDragonballScramble() == true && app->GetGsChannel() == 0) //do not allow to ride vehicle while in scramble channel AND joined scramble
						res->wResultCode = SCRAMBLE_CANNOT_DO_WHILE_JOINED;
					else
					{
						//	if (GetNaviEngine()->IsBasicAttributeSet(m_pOwner->GetCurWorld()->GetNaviInstanceHandle(), m_pOwner->GetCurLoc().x, m_pOwner->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_FORBID_VEHICLE) == false)
						//	{
						if (!m_pOwner->StartVehicle(m_pUseItemTbldat->tblidx, GetID()))
							res->wResultCode = GAME_FAIL;
						//	}
						//	else res->wResultCode = GAME_VEHICLE_END_BY_FORBIDDEN_ZONE;
					}
				}
				break;

				case ACTIVE_RECIPE_UPDATE:
				{
					if (m_pOwner->GetHoiPoiRecipe(GetTblidx()) == NULL)
					{
						sITEM_RECIPE_TBLDAT * recipeTbldat = (sITEM_RECIPE_TBLDAT*)g_pTableContainer->GetItemRecipeTable()->FindData(GetTblidx());
						if (recipeTbldat)
						{
							if (recipeTbldat->bValidityAble == true)
							{
								if (m_pOwner->GetLevel() >= recipeTbldat->byNeedMixLevel)
								{
									m_pOwner->AddHoiPoiRecipe(recipeTbldat->tblidx, recipeTbldat->byRecipeType);

									CNtlPacket pRecipe(sizeof(sGU_HOIPOIMIX_RECIPE_REG_NFY));
									sGU_HOIPOIMIX_RECIPE_REG_NFY* rRecipe = (sGU_HOIPOIMIX_RECIPE_REG_NFY*)pRecipe.GetPacketData();
									rRecipe->wOpCode = GU_HOIPOIMIX_RECIPE_REG_NFY;
									rRecipe->recipeTblidx = recipeTbldat->tblidx;
									pRecipe.SetPacketLen(sizeof(sGU_HOIPOIMIX_RECIPE_REG_NFY));
									m_pOwner->SendPacket(&pRecipe);

									CNtlPacket packetQry(sizeof(sGQ_RECIPE_REG_REQ));
									sGQ_RECIPE_REG_REQ * resQry = (sGQ_RECIPE_REG_REQ*)packetQry.GetPacketData();
									resQry->wOpCode = GQ_RECIPE_REG_REQ;
									resQry->handle = m_pOwner->GetID();
									resQry->charId = m_pOwner->GetCharID();
									resQry->byPlace = GetPlace();
									resQry->byPos = GetPos();
									resQry->itemId = GetItemID();
									resQry->recipeTblidx = recipeTbldat->tblidx;
									resQry->byRecipeType = recipeTbldat->byRecipeType;
									packetQry.SetPacketLen(sizeof(sGQ_RECIPE_REG_REQ));
									app->SendTo(app->GetQueryServerSession(), &packetQry);

								}
								else res->wResultCode = GAME_ITEM_RECIPE_CANNOT_SET_YOU_NEED_HIGHER_LEVEL;
							}
							else res->wResultCode = GAME_ITEM_RECIPE_REGISTER_FAIL;
						}
						else res->wResultCode = GAME_ITEM_RECIPE_NOT_FOUND;
					}
					else res->wResultCode = GAME_ITEM_RECIPE_NORMAL_RECIPE_ALREADY_STARTED;
				}
				break;

				case ACTIVE_SCRIPT_SIGNAL_SPS:
				{
					CNpc* pNpc = g_pObjectManager->GetNpc(m_ahApplyTarget[i]);
					if (pNpc && pNpc->IsInitialized() && pNpc->GetCurWorld() && pNpc->IsNPC() && pNpc->GetPlayScript() != INVALID_TBLIDX && pNpc->IsPlayScriptEnd() == false)
					{
						pNpc->SetLinkPc(m_pOwner->GetCharID(), m_pOwner->GetID());

						CObjMsg_SendItemEventToSps pMsg;
						pMsg.itemTblidx = GetTblidx();
						pNpc->OnObjectMsg(&pMsg);
					}
					else res->wResultCode = GAME_TARGET_IS_WRONG_STATE;
				}
				break;

				case ACTIVE_SCRIPT_EXECUTE_WPS_LOC:
				{
				//	printf("->tblidx %u, GetTblidx() %u, ->useWorldTblidx %u, RequiredQuestID %u \n", 
				//		m_pUseItemTbldat->tblidx, GetTblidx(), m_pUseItemTbldat->useWorldTblidx, m_pUseItemTbldat->RequiredQuestID);
					TBLIDX scriptId = (TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[0];

					if (GetOwner()->GetScript(scriptId) == NULL)
					{
						if (GetOwner()->GetCurWorld())
						{
							bool rbStart = false;
							CWpsAlgoObject* pObj = g_pScriptAlgoManager->CreateWpsObject(scriptId, rbStart);
							if (pObj)
							{
								pObj->AddPlayer(GetOwner()->GetID());
								pObj->SetWorld(GetOwner()->GetCurWorld());

								GetOwner()->AddScript(pObj);

								if (rbStart)
									pObj->Start();
							}
							else res->wResultCode = GAME_FAIL;
						}
						else res->wResultCode = GAME_FAIL;
					}
					else res->wResultCode = GAME_FAIL;
				}
				break;

				case ACTIVE_SHIFT_SEAL_STATE:
				{
					CDojo* pDojo = g_pDojoWarManager->GetDojo(m_pOwner->GetGuildID());
					if (pDojo)
					{
						CTriggerObject* obj = m_pOwner->GetCurWorld()->FindStaticObject(m_ahApplyTarget[i]);
						if (obj && BIT_FLAG_TEST(obj->GetSubState(), TOBJECT_SUBSTATE_FLAG_SHOW))
						{
							BYTE byState = obj->GetMainState();

							if (pDojo->UpdateSeal(m_pOwner->GetCharName(), m_pOwner->GetGuildID(), obj->GetTblidx(), byState))
							{
								obj->UpdateToObjectState(byState, TOBJECT_SUBSTATE_FLAG_SHOW, app->GetCurTickCount());
							}
						}
						else res->wResultCode = GAME_FAIL;
					}
					else res->wResultCode = GAME_FAIL;
				}
				break;

				case ACTIVE_TELEPORT_DOJO:
				{
					bool bCanEnter = true;

					if (CDojo* pDojoWar = g_pDojoWarManager->GetDojo(m_pOwner->GetGuildID()))
					{
						if (pDojoWar->GetWarState() != eDBO_DOJO_STATUS_READY && pDojoWar->GetWarState() != eDBO_DOJO_STATUS_START)
						{
							bCanEnter = false;
						}
					}

					if (bCanEnter)
					{
						sDBO_DOJO_BRIEF* dojobrief = g_pDojoManager->GetDojoBrief(m_pOwner->GetGuildID());
						if (dojobrief)
						{
							sDOJO_TBLDAT* dojoTbl = (sDOJO_TBLDAT*)g_pTableContainer->GetDojoTable()->FindData(dojobrief->dojoTblidx);
							if (dojoTbl)
							{
								CWorld* pWorld = app->GetGameMain()->GetWorldManager()->FindWorld(dojoTbl->zoneTblidx);
								if (pWorld)
								{
									if (m_pOwner->GetCurWorld() != NULL && m_pOwner->GetCurWorld()->GetRuleType() == GAMERULE_NORMAL)
										m_pOwner->TeleportAnotherServer(pWorld->GetTbldat()->vStart2Loc, pWorld->GetTbldat()->vStart2Dir, pWorld->GetIdx(), pWorld->GetID(), TELEPORT_TYPE_DOJO, DOJO_CHANNEL_INDEX, 0, false);
									else
										res->wResultCode = GAME_ITEM_CANT_USE_NOW;
								}
								else res->wResultCode = GAME_WORLD_NOT_FOUND;
							}
							else res->wResultCode = GAME_GUILD_DOJO_NOT_FOUND_TABLE;
						}
						else res->wResultCode = GAME_GUILD_DOJO_NOT_FOUND;
					}
					else res->wResultCode = GAME_FAIL;
				}
				break;

				case ACTIVE_LP_AUTO_RECOVER:
				case ACTIVE_EP_AUTO_RECOVER:
				case ACTIVE_MASCOT_VP_AUTO_RECOVERY:
				{
					buffInfo.dwTimeRemaining = INVALID_DWORD;
					buffInfo.dwInitialDuration = INVALID_DWORD;
					buffInfo.aBuffParameter[0].buffParameter.fParameter = 0.0f;
					buffInfo.aBuffParameter[1].buffParameter.fParameter = 0.0f;
					buffInfo.aBuffParameter[0].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_AUTO_RECOVER;
					sCOMMONCONFIG_VALUE_DATA* commonConfig = g_pTableContainer->GetCommonConfigTable()->FindCommonConfig((TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[e]);
					buffInfo.aBuffParameter[0].buffParameter.commonConfigTblidx = (TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[0];
					buffInfo.aBuffParameter[0].buffParameter.dwRemainValue = commonConfig->adwValue[2]; //remain value
					buffInfo.aBuffParameter[1].buffParameter.dwRemainValue = commonConfig->adwValue[2]; //max value
					buffInfo.aBuffParameter[0].buffParameter.dwRemainTime = 0; //remain time
					buffInfo.aBuffParameter[1].buffParameter.dwRemainTime = commonConfig->adwValue[3] * 1000; //max time
				}
				break;

				case ACTIVE_DRUNK:
				{
					buffInfo.aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DEFAULT;
					buffInfo.aBuffParameter[e].buffParameter.fParameter = (float)m_pUseItemTbldat->aSystem_Effect_Value[e];
					buffInfo.aBuffParameter[e].buffParameter.commonConfigTblidx = (TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[e];
				}
				break;

				case ACTIVE_EXCITATION_MALE:
				{
					CPlayer* pTarget = g_pObjectManager->GetPC(m_ahApplyTarget[i]);
					if (pTarget && pTarget->IsInitialized())
					{
						if (pTarget->IsPC())
						{
							if (pTarget->GetGender() == GENDER_MALE) //only males
							{
								pTarget->SetLpEpEventID(res->dwLpEpEventId);

								buffInfo.aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DOT;
								buffInfo.aBuffParameter[e].buffParameter.fParameter = (float)m_pUseItemTbldat->aSystem_Effect_Value[e];
								buffInfo.aBuffParameter[e].buffParameter.commonConfigTblidx = (TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[e];
								buffInfo.aBuffParameter[e].buffParameter.dwRemainTime = m_pUseItemTbldat->dwKeepTimeInMilliSecs;
							}
						}
						else res->wResultCode = GAME_ITEM_CANNOT_USE_INVALID_TARGET;
					}
					else res->wResultCode = GAME_ITEM_CANNOT_USE_INVALID_TARGET;
				}
				break;

				case ACTIVE_EXCITATION_FEMALE:
				{
					CPlayer* pTarget = g_pObjectManager->GetPC(m_ahApplyTarget[i]);
					if (pTarget && pTarget->IsInitialized())
					{
						if (pTarget->IsPC())
						{
							if (pTarget->GetGender() != GENDER_MALE) //only nemkian and female
							{
								pTarget->SetLpEpEventID(res->dwLpEpEventId);

								sCOMMONCONFIG_VALUE_DATA* commonConfig = g_pTableContainer->GetCommonConfigTable()->FindCommonConfig((TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[e]);

								buffInfo.aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DOT;
								buffInfo.aBuffParameter[e].buffParameter.fParameter = (float)m_pUseItemTbldat->aSystem_Effect_Value[e];
								buffInfo.aBuffParameter[e].buffParameter.commonConfigTblidx = (TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[e];
								buffInfo.aBuffParameter[e].buffParameter.dwRemainTime = m_pUseItemTbldat->dwKeepTimeInMilliSecs;

								if (pTarget->GetBuffManager()->RegisterSubBuff(&buffInfo, effectCode, m_pOwner->GetID(), m_pUseItemTbldat->byBuff_Group, res->wResultCode, m_pUseItemTbldat->abySystem_Effect_Type))
									pTarget->UpdateCurRP((WORD)commonConfig->adwValue[1], true, false);

								bAddBuff = false;
							}
						}
						else res->wResultCode = GAME_ITEM_CANNOT_USE_INVALID_TARGET;
					}
					else res->wResultCode = GAME_ITEM_CANNOT_USE_INVALID_TARGET;
				}
				break;

				case ACTIVE_PARTY_POPO:
				{
					if (m_pOwner->GetParty())
					{
						if (m_pOwner->GetCurWorld() && m_pOwner->GetCurWorld()->GetTbldat()->bDynamic == false)
						{
							m_pOwner->GetParty()->StartPartyTeleport(m_pOwner);
						}
						else res->wResultCode = GAME_ITEM_CANT_USE_INVALID_WORLD;
					}
					else res->wResultCode = GAME_COMMON_YOU_ARE_NOT_IN_A_PARTY;
				}
				break;

				case ACTIVE_SEX_LOOKS_CHANGE:
				{
					if (m_pOwner->GetGender() != GENDER_ONE_SEX) //only male and female
					{
						BYTE byNewGender = (m_pOwner->GetGender() == GENDER_FEMALE) ? GENDER_MALE : GENDER_FEMALE;

						if (m_pOwner->GetPlayerItemContainer()->WearGenderRequiredItem(byNewGender) == false)
						{
							CNtlPacket packetQry(sizeof(sGQ_CHAR_CONVERT_GENDER_NFY));
							sGQ_CHAR_CONVERT_GENDER_NFY* resQry = (sGQ_CHAR_CONVERT_GENDER_NFY*)packetQry.GetPacketData();
							resQry->wOpCode = GQ_CHAR_CONVERT_GENDER_NFY;
							resQry->charId = m_pOwner->GetCharID();
							resQry->byGender = byNewGender;
							packetQry.SetPacketLen(sizeof(sGQ_CHAR_CONVERT_GENDER_NFY));
							app->SendTo(app->GetQueryServerSession(), &packetQry);
						}
						else res->wResultCode = GAME_ITEM_GENDER_DOESNT_MATCH;
					}
					else res->wResultCode = GAME_ITEM_GENDER_DOESNT_MATCH;
				}
				break;

				case ACTIVE_MASCOT_REGIST:
				{
					if (m_pOwner->RegisterMascot(GetTblidx()) == false)
						res->wResultCode = MASCOT_EXCEED_HAVE_COUNT;
				}
				break;

				case ACTIVE_ADVANCED_RESCUE: //revive party members and heal 100%
				{
					if (m_pOwner->GetParty() == NULL)
					{
						res->wResultCode = GAME_PARTY_YOU_ARE_NOT_IN_PARTY;
						goto END;
					}

					CCharacter* pTarget = g_pObjectManager->GetPC(m_ahApplyTarget[i]);
					if (pTarget && pTarget->IsInitialized())
					{
						if (pTarget->GetID() != m_pOwner->GetID() && pTarget->IsFainting())
						{
							if (pTarget->GetCurWorld() && pTarget->GetCurWorld()->GetRuleType() != GAMERULE_RANKBATTLE && pTarget->GetCurWorld()->GetRuleType() != GAMERULE_MINORMATCH && pTarget->GetCurWorld()->GetRuleType() != GAMERULE_MAJORMATCH && pTarget->GetCurWorld()->GetRuleType() != GAMERULE_FINALMATCH && pTarget->GetCurWorld()->GetRuleType() != GAMERULE_CCBATTLEDUNGEON)
							{
								pTarget->SetLpEpEventID(res->dwLpEpEventId);

								pTarget->Revival(pTarget->GetCurLoc(), pTarget->GetWorldID(), REVIVAL_TYPE_RESCUED);
								pTarget->UpdateCurLP(pTarget->GetMaxLP() * 100 / 100, true, false);
								pTarget->UpdateCurEP(pTarget->GetMaxEP() * 100 / 100, true, false);

								pTarget->GetCurLoc().CopyTo(res->aSkillResult[bySkillResultCount].vFinalSubjectLoc);
							}
							else bSuccess = false;
						}
						else bSuccess = false;
					}
					else bSuccess = false;
				}
				break;

				case ACTIVE_EVENT_COIN_PLUS:
				{
					if (app->GetChatServerSession())
					{
						m_pOwner->SetEventMachineCoin(m_pOwner->GetEventMachineCoin() + (DWORD)m_pUseItemTbldat->aSystem_Effect_Value[0]);

						//send packet to chat server that waguwagu has increased
						CNtlPacket packetGT(sizeof(sGT_EVENTCOIN_INCREASE_NFY));
						sGT_EVENTCOIN_INCREASE_NFY* resGT = (sGT_EVENTCOIN_INCREASE_NFY*)packetGT.GetPacketData();
						resGT->wOpCode = GT_EVENTCOIN_INCREASE_NFY;
						resGT->charId = m_pOwner->GetCharID();
						resGT->dwEventCoin = m_pOwner->GetEventMachineCoin();
						resGT->accountId = m_pOwner->GetAccountID();
						packetGT.SetPacketLen(sizeof(sGT_EVENTCOIN_INCREASE_NFY));
						app->SendTo(app->GetChatServerSession(), &packetGT);


						CNtlPacket packetQry(sizeof(sGQ_EVENT_COIN_ADD_REQ));
						sGQ_EVENT_COIN_ADD_REQ* resQry = (sGQ_EVENT_COIN_ADD_REQ*)packetQry.GetPacketData();
						resQry->wOpCode = GQ_EVENT_COIN_ADD_REQ;
						resQry->hPc = m_pOwner->GetID();
						resQry->accountId = m_pOwner->GetAccountID();
						resQry->charId = m_pOwner->GetCharID();
						resQry->byIncreaseCoin = (BYTE)m_pUseItemTbldat->aSystem_Effect_Value[0];
						resQry->byCoinType = 0;
						packetQry.SetPacketLen(sizeof(sGQ_EVENT_COIN_ADD_REQ));
						app->SendTo(app->GetQueryServerSession(), &packetQry);

					}
					else res->wResultCode = GAME_COMMON_CANT_DO_THAT_FOR_SOME_REASON;
				}
				break;

				case ACTIVE_WAGU_COIN_PLUS:
				{
					if (app->GetChatServerSession())
					{
						CNtlPacket packetQry(sizeof(sGQ_WAGUWAGUMACHINE_COIN_INCREASE_REQ));
						sGQ_WAGUWAGUMACHINE_COIN_INCREASE_REQ* resQry = (sGQ_WAGUWAGUMACHINE_COIN_INCREASE_REQ*)packetQry.GetPacketData();
						resQry->wOpCode = GQ_WAGUWAGUMACHINE_COIN_INCREASE_REQ;
						resQry->handle = m_pOwner->GetID();
						resQry->accountId = m_pOwner->GetAccountID();
						resQry->characterId = m_pOwner->GetCharID();
						resQry->wWaguCoin = (WORD)m_pUseItemTbldat->aSystem_Effect_Value[0];
						resQry->qwProductId = INVALID_QWORD;
						packetQry.SetPacketLen(sizeof(sGQ_WAGUWAGUMACHINE_COIN_INCREASE_REQ));
						app->SendTo(app->GetQueryServerSession(), &packetQry);
					}
					else res->wResultCode = GAME_COMMON_CANT_DO_THAT_FOR_SOME_REASON;
				}
				break;

				case ACTIVE_CHAR_TITLE:
				{
					if (m_pOwner->CheckCharTitle((TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[e] - 1) == false)
					{
						m_pOwner->AddCharTitle((TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[e] - 1);
					}
					else res->wResultCode = CHARTITLE_YOU_ALREADY_HAVE_THE_TITLE;
				}
				break;

				case ACTIVE_CHILD_ADULT_TRANSFORM:
				{
					if (m_pOwner->IsAdult())
						m_pOwner->UpdateAdult(false);
					else
						m_pOwner->UpdateAdult(true);
				}
				break;

				case ACTIVE_MAX_LP_UP:
				case ACTIVE_MAX_EP_UP:
				case ACTIVE_PHYSICAL_DEFENCE_UP:
				case ACTIVE_ENERGY_DEFENCE_UP:
				case ACTIVE_STR_UP:
				case ACTIVE_CON_UP:
				case ACTIVE_FOC_UP:
				case ACTIVE_DEX_UP:
				case ACTIVE_SOL_UP:
				case ACTIVE_ENG_UP:
				case ACTIVE_MOVE_SPEED_UP:
				case ACTIVE_LP_REGENERATION:
				case ACTIVE_EP_REGENERATION:
				case ACTIVE_AIR_MOVE:
				case ACTIVE_AIR_DASH:
				case ACTIVE_AIR_ACCEL:
				case ACTIVE_AIR_MOVE_DASH_ACCEL:
				case ACTIVE_MAX_AP_UP:
				case ACTIVE_AP_REGENERATION:
				case ACTIVE_EXP_BOOSTER:
				{
					buffInfo.aBuffParameter[e].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DEFAULT;
					buffInfo.aBuffParameter[e].buffParameter.fParameter = (float)m_pUseItemTbldat->aSystem_Effect_Value[e];
					res->aSkillResult[bySkillResultCount].effectResult[e].fResultValue = buffInfo.aBuffParameter[e].buffParameter.fParameter;
				}
				break;

				case ACTIVE_SET_LEVEL:
				{
					CCharacter* pTarget = g_pObjectManager->GetPC(m_ahApplyTarget[i]);
					if (pTarget && pTarget->IsInitialized())
					{
						pTarget->SetLpEpEventID(res->dwLpEpEventId);
						if (m_pOwner->GetLevel() < (BYTE)m_pUseItemTbldat->aSystem_Effect_Value[e])
							m_pOwner->LevelUp(pTarget->GetExp(), (BYTE)m_pUseItemTbldat->aSystem_Effect_Value[e] - pTarget->GetLevel());
						else
							res->wResultCode = GAME_ITEM_NEED_LESS_LEVEL;
					}
				}
				break;

				case ACTIVE_MASCOT_VP_RECOVERY:
				{
					if (m_pOwner->GetCurrentMascot())
						res->wResultCode = m_pOwner->GetCurrentMascot()->UpdateVP((DWORD)m_pUseItemTbldat->aSystem_Effect_Value[e]);
					else
						res->wResultCode = MASCOT_FAIL;
				}
				break;

				case ACTIVE_MASCOT_EXP_AUTO_PORTION:
				{
					buffInfo.dwTimeRemaining = INVALID_DWORD;
					buffInfo.dwInitialDuration = INVALID_DWORD;
					buffInfo.aBuffParameter[0].buffParameter.fParameter = 0.0f;
					buffInfo.aBuffParameter[1].buffParameter.fParameter = 0.0f;
					buffInfo.aBuffParameter[0].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_AUTO_RECOVER;
					sCOMMONCONFIG_VALUE_DATA* commonConfig = g_pTableContainer->GetCommonConfigTable()->FindCommonConfig((TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[e]);
					buffInfo.aBuffParameter[0].buffParameter.commonConfigTblidx = (TBLIDX)m_pUseItemTbldat->aSystem_Effect_Value[e];
					buffInfo.aBuffParameter[0].buffParameter.dwRemainValue = commonConfig->adwValue[2]; //remain value
					buffInfo.aBuffParameter[1].buffParameter.dwRemainValue = commonConfig->adwValue[2]; //max value
					buffInfo.aBuffParameter[0].buffParameter.dwRemainTime = 0; //remain time
					buffInfo.aBuffParameter[1].buffParameter.dwRemainTime = commonConfig->adwValue[0]; //max time
				}
				break;

				default:
				{
					ERR_LOG(LOG_GENERAL, "Item Tblidx %u. Use-Item effectcode %s not setup yet.", GetTblidx() , NtlGetSystemEffectString(effectCode[e]));
					res->wResultCode = GAME_ITEM_CANT_USE_NOW;
				}
				break;
			}
		}

		if (Dbo_IsForBlessBuff(m_pUseItemTbldat->byUse_Item_Active_Type) && res->wResultCode == GAME_SUCCESS && bAddBuff)
		{
			CCharacter* pTarget = g_pObjectManager->GetPC(m_ahApplyTarget[i]);
			if (pTarget && pTarget->IsInitialized())
			{
				pTarget->GetBuffManager()->RegisterSubBuff(&buffInfo, effectCode, m_pOwner->GetID(), m_pUseItemTbldat->byBuff_Group, res->wResultCode, m_pUseItemTbldat->abySystem_Effect_Type);
			}
		}

		if (res->wResultCode == GAME_SUCCESS && bSuccess == true)
		{
			res->aSkillResult[bySkillResultCount].hTarget = m_ahApplyTarget[i];
			res->bySkillResultCount += 1;

			++bySkillResultCount;
		}
	}

	END:

	packet.SetPacketLen(sizeof(sGU_CHAR_ACTION_ITEM));

	if (res->wResultCode == GAME_SUCCESS && res->bySkillResultCount > 0)
	{
		m_pOwner->Broadcast(&packet); //broadcast to all

		m_pOwner->BeginItemCooldown(m_pUseItemTbldat->dwCool_Time_Bit_Flag, m_pUseItemTbldat->dwCoolTimeInMilliSecs, m_pUseItemTbldat->dwCoolTimeInMilliSecs);

		if (pKeyItem)
			pKeyItem->SetCount(pKeyItem->GetCount() - 1, false, true);
	}
	else
	{
		m_pOwner->SendPacket(&packet); //send only to player the error message
	}

	if (BIT_FLAG_TEST(m_pUseItemTbldat->wFunction_Bit_Flag, MAKE_BIT_FLAG(USE_ITEM_FUNCTION_NON_CONSUMABLE)) == false && res->bySkillResultCount > 0)
		SetCount(GetCount() - 1, false, true);
}


void CItem::CancelCasting()
{
	m_pOwner->SetItemCast(INVALID_HOBJECT);
	SetLocked(false);

	CNtlPacket packet(sizeof(sGU_ITEM_CASTING_CANCELED));
	sGU_ITEM_CASTING_CANCELED * res = (sGU_ITEM_CASTING_CANCELED *)packet.GetPacketData();
	res->wOpCode = GU_ITEM_CASTING_CANCELED;
	res->handle = m_pOwner->GetID();
	res->wResultCode = GAME_SUCCESS;
	packet.SetPacketLen(sizeof(sGU_ITEM_CASTING_CANCELED));
	m_pOwner->Broadcast(&packet);
}


WORD CItem::InsertSocketBead(CItem* pBeadItem)
{
	CGameServer* app = (CGameServer*)g_pApp;

	float fRemainingWorth = Dbo_GetItemWorth(pBeadItem->GetRank(), 25);

	CNtlBitFlagManager flag;
	flag.Create(256);

	//printf("default item worth %f \n", fRemainingWorth);
	
	sITEM_RANDOM_OPTION aRandomOption[2];
	aRandomOption[0].optionValue = 0;
	aRandomOption[0].wOptionIndex = INVALID_WORD;
	aRandomOption[1].optionValue = 0;
	aRandomOption[1].wOptionIndex = INVALID_WORD;

	if (!g_pTableContainer->GetItemEnchantTable()->ProcessRandomOption(GetTbldat(), pBeadItem->GetRank(), false, false, &flag, &fRemainingWorth, aRandomOption[0]))
		return GAME_ITEM_INVALID_BEAD_OPTION;

	//printf("2 item worth %f \n", fRemainingWorth);
	g_pTableContainer->GetItemEnchantTable()->ProcessRandomOption(GetTbldat(), pBeadItem->GetRank(), false, true, &flag, &fRemainingWorth, aRandomOption[1]);


	CNtlPacket packet(sizeof(sGQ_ITEM_SOCKET_INSERT_BEAD_REQ));
	sGQ_ITEM_SOCKET_INSERT_BEAD_REQ * res = (sGQ_ITEM_SOCKET_INSERT_BEAD_REQ *)packet.GetPacketData();
	res->wOpCode = GQ_ITEM_SOCKET_INSERT_BEAD_REQ;
	res->handle = m_pOwner->GetID();
	res->charId = m_pOwner->GetCharID();
	res->byItemPlace = GetPlace();
	res->byItemPos = GetPos();
	res->ItemId = GetItemID();
	res->byRestrictState = ITEM_RESTRICT_STATE_TYPE_INSERT_BEAD; 
	res->byDurationType = GetTbldat()->byDurationType;
	res->byBeadPlace = pBeadItem->GetPlace();
	res->byBeadPos = pBeadItem->GetPos();
	res->BeadItemId = pBeadItem->GetItemID();
	res->byBeadRemainStack = UnsignedSafeDecrease<BYTE>(pBeadItem->GetCount(), 1);
	res->sItemRandomOption[0].optionValue = aRandomOption[0].optionValue;
	res->sItemRandomOption[0].wOptionIndex = aRandomOption[0].wOptionIndex;
	res->sItemRandomOption[1].optionValue = aRandomOption[1].optionValue;
	res->sItemRandomOption[1].wOptionIndex = aRandomOption[1].wOptionIndex;
	res->nUseStartTime = 0;
	res->nUseEndTime = 0;
	packet.SetPacketLen(sizeof(sGQ_ITEM_SOCKET_INSERT_BEAD_REQ));
	app->SendTo(app->GetQueryServerSession(), &packet);

	pBeadItem->SetCount(res->byBeadRemainStack, false, false);

	return GAME_SUCCESS;
}


void CItem::DestroySocketBead(bool bTimeOut)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGQ_ITEM_SOCKET_DESTROY_BEAD_REQ));
	sGQ_ITEM_SOCKET_DESTROY_BEAD_REQ * res = (sGQ_ITEM_SOCKET_DESTROY_BEAD_REQ *)packet.GetPacketData();
	res->wOpCode = GQ_ITEM_SOCKET_DESTROY_BEAD_REQ;
	res->handle = m_pOwner->GetID();
	res->charId = m_pOwner->GetCharID();
	res->byItemPlace = GetPlace();
	res->byItemPos = GetPos();
	res->ItemId = GetItemID();
	res->byRestrictState = GetTbldat()->byRestrictType; //set default restriction
	res->byDurationType = GetTbldat()->byDurationType; //set default duration type
	res->bTimeOut = bTimeOut;
	packet.SetPacketLen(sizeof(sGQ_ITEM_SOCKET_DESTROY_BEAD_REQ));
	app->SendTo(app->GetQueryServerSession(), &packet);
}


void CItem::SetSocketBead(BYTE byRestrictState, BYTE byDurationType, sITEM_RANDOM_OPTION* pRandomOption, DBOTIME nUseStartTime, DBOTIME nUseEndTime)
{
	item_data.byRestrictState = byRestrictState;
	item_data.byDurationType = byDurationType;

	if (pRandomOption)
	{
		item_data.sOptionSet.aRandomOption[6].optionValue = pRandomOption[0].optionValue;
		item_data.sOptionSet.aRandomOption[6].wOptionIndex = pRandomOption[0].wOptionIndex;
		item_data.sOptionSet.aRandomOption[7].optionValue = pRandomOption[1].optionValue;
		item_data.sOptionSet.aRandomOption[7].wOptionIndex = pRandomOption[1].wOptionIndex;
	}
	else
	{
		item_data.sOptionSet.aRandomOption[6].optionValue = 0;
		item_data.sOptionSet.aRandomOption[6].wOptionIndex = INVALID_WORD;
		item_data.sOptionSet.aRandomOption[7].optionValue = 0;
		item_data.sOptionSet.aRandomOption[7].wOptionIndex = INVALID_WORD;
	}

	item_data.nUseStartTime = nUseStartTime;
	item_data.nUseEndTime = nUseEndTime;
}

bool CItem::HasScouterEffect(int nEffectCode)
{
	sITEM_OPTION_TBLDAT* pOptionTbldat = (sITEM_OPTION_TBLDAT*)g_pTableContainer->GetItemOptionTable()->FindData(m_pProto->Item_Option_Tblidx);
	if (!pOptionTbldat)
		return false;

	for (int i = 0; i < NTL_MAX_SYSTEM_EFFECT_COUNT; i++)
	{
		eSYSTEM_EFFECT_CODE effectcode = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pOptionTbldat->system_Effect[i]);
		if (effectcode == INVALID_SYSTEM_EFFECT_CODE)
			return false;

		if (effectcode == nEffectCode)
			return true;
	}

	return false;
}
