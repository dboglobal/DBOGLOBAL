#include "stdafx.h"
#include "ItemDrop.h"
#include "CPlayer.h"
#include "ItemManager.h"
#include "NtlBitFlagManager.h"
#include "item.h"
#include "NtlRandom.h"
#include "NtlPacketGU.h"
#include "NtlResultCode.h"
#include "GameMain.h"
#include "GameServer.h"
#include "TableContainerManager.h"
#include "ItemEnchantTable.h"


CItemDrop::CItemDrop(eOBJTYPE eObjType)
	:CSpawnObject(eObjType)
{
	m_hOwner = INVALID_HOBJECT;
	m_partyID = INVALID_PARTYID;
	m_dwDropTime = GetTickCount();

	m_tblidx = INVALID_TBLIDX;
	m_byBattleAttribute = INVALID_BYTE;
	m_byGrade = 0;
	m_byRank = ITEM_RANK_NORMAL;
	m_bNeedToIdentify = false;
	m_dwZeniDrop = 0;

	if (eObjType == OBJTYPE_DROPITEM)
		m_sOptionSet = new sITEM_OPTION_SET;
	else
		m_sOptionSet = NULL;

	m_bLocked = false;
}

CItemDrop::~CItemDrop()
{
	//printf("CItemDrop::~CItemDrop() \n");
	if(!m_bIsDeleted)
		g_pEventMgr->RemoveEvents(this);

	SAFE_DELETE(m_sOptionSet);
}


void CItemDrop::CopyToObjectInfo(sOBJECT_INFO *pObjectInfo, CHARACTERID playerCharID)
{
	pObjectInfo->objType = (BYTE)GetObjType();

	DWORD dwCurTick = GetTickCount();

	switch (GetObjType())
	{
	case OBJTYPE_DROPMONEY:
	{
		pObjectInfo->moneyBrief.dwZenny = m_dwZeniDrop;
		pObjectInfo->moneyState.bIsNew = m_dwDropTime + 5000 > dwCurTick;
		GetCurLoc().CopyTo(pObjectInfo->moneyState.vCurLoc);
	}
	break;
	case OBJTYPE_DROPITEM:
	{
		pObjectInfo->itemBrief.byBattleAttribute = m_byBattleAttribute;
		pObjectInfo->itemBrief.byGrade = m_byGrade;
		pObjectInfo->itemBrief.byRank = m_byRank;
		pObjectInfo->itemBrief.tblidx = m_tblidx;
		pObjectInfo->itemState.bIsNew = m_dwDropTime + 5000 > dwCurTick;
		pObjectInfo->itemState.bNeedToIdentify = m_bNeedToIdentify;
		pObjectInfo->itemState.dwDropTime = dwCurTick - m_dwDropTime;
		pObjectInfo->itemState.hOwner = m_hOwner;
		pObjectInfo->itemState.partyID = m_partyID;
		GetCurLoc().CopyTo(pObjectInfo->itemState.vCurLoc);

		pObjectInfo->itemOptionSet.Init();

		if(m_sOptionSet)
			memcpy(&pObjectInfo->itemOptionSet, m_sOptionSet, sizeof(sITEM_OPTION_SET));
	}
	break;

	default: ERR_LOG(LOG_GENERAL, "Object type %d not set", GetObjType());
	}
}


void CItemDrop::GenerateRank(sITEM_TBLDAT* table, bool bDungeon, BYTE byMobGrade, DWORD dwSuperior, DWORD dwExcellent, DWORD dwRare, DWORD dwLegendary)
{
	if (table->byEquip_Type != EQUIP_TYPE_MAIN_WEAPON && table->byEquip_Type != EQUIP_TYPE_SUB_WEAPON && table->byEquip_Type != EQUIP_TYPE_ARMOR && table->byEquip_Type != EQUIP_TYPE_ACCESSORY)
		return;
	else if (table->bIsCanHaveOption && table->Item_Option_Tblidx != INVALID_TBLIDX)
		return;

	//printf("Generate rank for item ix %u \n", table->tblidx);

	/*Superior = 4%, Excellent = 3%, Rare = 2%, Legendary = 1%*/

	DWORD dwRank = dwSuperior + dwExcellent + dwRare + dwLegendary;

	float fSuperiorRate = 4.f;
	float fExcellentRate = 2.f;
	float fRareRate = 1.f;
	float fLegendaryRate = 0.5f;

	if (dwRank > 0)
	{
		if (dwSuperior > 0)
			fSuperiorRate = 1000.f - (float)dwSuperior / 10.f;
		else
			fSuperiorRate = 0.0f;

		if (dwExcellent > 0)
			fExcellentRate = 1000.f - (float)dwExcellent / 10.f;
		else
			fExcellentRate = 0.0f;

		if (dwRare > 0)
			fRareRate = 1000.f - (float)dwRare / 10.f;
		else
			fRareRate = 0.0f;

		if(dwLegendary > 0)
			fLegendaryRate = 1000.f - (float)dwLegendary / 10.f;
		else
			fLegendaryRate = 0.0f;
	}
	else
	{
		if (bDungeon)
		{
			switch (byMobGrade)
			{
				case MOB_GRADE_NORMAL:
				{
					fSuperiorRate *= 3.5f;
				}
				break;
				case MOB_GRADE_SUPER:
				{
					fSuperiorRate *= 6.f;
					fExcellentRate *= 2.f;
				}
				break;
				case MOB_GRADE_ULTRA:
				{
					fSuperiorRate *= 15.f;
					fExcellentRate *= 14.f;
					fRareRate *= 13.f;
					fLegendaryRate *= 12.f;
				}
				break;
				case MOB_GRADE_BOSS:
				{
					fSuperiorRate *= 50.f;
					fExcellentRate *= 50.f;
					fRareRate *= 35.f;
					fLegendaryRate *= 35.f;
				}
				break;
				case MOB_GRADE_HERO:
				{
					fSuperiorRate *= 50.f;
					fExcellentRate *= 50.f;
					fRareRate *= 40.f;
					fLegendaryRate *= 40.f;
				}
				break;
				default: break;
			}
		}

		//printf("dwSuperiorRate %f, dwExcellentRate %f, dwRareRate %f, dwLegendaryRate %f \n", fSuperiorRate, fExcellentRate, fRareRate, fLegendaryRate);
	}

	std::vector<BYTE> vecAvailableRank;


	if (Dbo_CheckProbabilityF(fSuperiorRate))
	{
		vecAvailableRank.push_back(ITEM_RANK_SUPERIOR);
	}

	if (Dbo_CheckProbabilityF(fExcellentRate))
	{
		vecAvailableRank.push_back(ITEM_RANK_EXCELLENT);
	}

	if (Dbo_CheckProbabilityF(fRareRate))
	{
		vecAvailableRank.push_back(ITEM_RANK_RARE);
	}

	if (Dbo_CheckProbabilityF(fLegendaryRate))
	{
		vecAvailableRank.push_back(ITEM_RANK_LEGENDARY);
	}

	BYTE byNewRank;

	if (vecAvailableRank.size() > 0)
	{
		byNewRank = vecAvailableRank[RandomRange(0, (int)vecAvailableRank.size() - 1)];

		if (byNewRank > table->byRank)
			m_byRank = byNewRank;
	}

	//printf("set random rank %u \n", m_byRank);
}

void CItemDrop::SetTbldat(sITEM_TBLDAT* table, bool bCanUnidentified/* = true*/)
{
	m_tblidx = table->tblidx;
	m_byBattleAttribute = table->byBattle_Attribute;
	m_byRank = table->byRank;
	//printf("default m_byRank %u \n", m_byRank);
	m_byGrade = 0;

	if (bCanUnidentified && table->byItem_Type != ITEM_TYPE_DRAGONBALL && Dbo_CheckProbability(NTL_MAX_IDENTIFY_DROP_RATE))
		m_bNeedToIdentify = true;
}

//pick up item
void CItemDrop::PickUpItem(CPlayer* pPlayer)
{
	if (!pPlayer || !pPlayer->IsInitialized())
		return;

	WORD resultcode = GAME_SUCCESS;

	bool bPartyDice = false;
	bool bInOrder = false;
	bool bByPartyHunting = false;

	//decide if its greedy or not
	if (pPlayer->GetDragonballScramble() == false && pPlayer->GetParty() && pPlayer->GetParty()->GetZeniLootingMethod() != NTL_PARTY_ITEM_LOOTING_GREEDILY && pPlayer->GetParty()->GetPartyMemberCount() > 1)
	{
		switch (pPlayer->GetParty()->GetItemLootingMethod())
		{
			case NTL_PARTY_ITEM_LOOTING_IN_ORDER:
			{
				if (pPlayer->GetParty()->GetItemLootingMethodRank() <= m_byRank)
					bInOrder = true;
			}
			break;

			case NTL_PARTY_ITEM_LOOTING_DICE:
			case NTL_PARTY_ITEM_LOOTING_DICE_BY_EQUIPED:
			{
				if (pPlayer->GetParty()->GetItemLootingMethodRank() <= m_byRank)
					bPartyDice = true;
			}
			break;

			default: break;
		}
	}

	if (pPlayer->GetDragonballScramble() == false && m_tblidx >= 200041 && m_tblidx <= 200047)
	{
		resultcode = SCRAMBLE_CANNOT_DO_WHILE_NOT_JOINED;
	}
	else
	{
		if (bPartyDice == false)
		{
			if (bInOrder == false)
			{
				if (g_pItemManager->CreateItem(pPlayer, this) == false)
					resultcode = GAME_ITEM_INVEN_FULL;
				else
					g_pItemManager->DestroyItemDropOverTime(this);
			}
			else
			{
				if (pPlayer->GetParty()->ShareItemDropInOrder(this))
					g_pItemManager->DestroyItemDropOverTime(this);
				else
					resultcode = GAME_PARTY_NOBODY_CANT_RECEIVE_ITEM_RIGHT_NOW;
			}
		}
		else
		{
			//here we do not delete item drop. Because we do that when finish dice because item stays on ground while dicing
			if (pPlayer->GetParty()->CreatePartyInventoryItem(this) == false)
				resultcode = GAME_PARTY_NO_EMPTY_SPACE_IN_PARTY_INVENTORY;
			else
				bByPartyHunting = true;
		}
	}

	CNtlPacket packet3(sizeof(sGU_ITEM_PICK_RES));
	sGU_ITEM_PICK_RES * res3 = (sGU_ITEM_PICK_RES *)packet3.GetPacketData();
	res3->wOpCode = GU_ITEM_PICK_RES;
	res3->wResultCode = resultcode;
	res3->bByPartyHunting = bByPartyHunting;
	res3->bPartyDice = bPartyDice;
	res3->itemTblidx = m_tblidx;
	packet3.SetPacketLen(sizeof(sGU_ITEM_PICK_RES));
	pPlayer->SendPacket(&packet3);
}

void CItemDrop::PickUpZeni(CPlayer * pPlayer)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (pPlayer->GetParty() && pPlayer->GetParty()->GetZeniLootingMethod() == NTL_PARTY_ZENNY_LOOTING_SHARED && pPlayer->GetParty()->GetPartyMemberCount() > 1)
	{
		pPlayer->GetParty()->ShareZeniDropInParty(pPlayer, GetZeni());

		//destroy item <async>
		g_pItemManager->DestroyItemDropOverTime(this);
	}
	else
	{
		DWORD dwZeniBonus = GetZeni() * app->GetZeniBonusRate() / 100;

		CNtlPacket packet(sizeof(sGU_ZENNY_PICK_RES));
		sGU_ZENNY_PICK_RES * res = (sGU_ZENNY_PICK_RES *)packet.GetPacketData();
		res->wOpCode = GU_ZENNY_PICK_RES;
		res->wResultCode = GAME_ZENNY_OVER;
		res->bSharedInParty = false;
		res->dwBonusZenny = dwZeniBonus;
		res->dwOriginalZenny = GetZeni();
		res->dwZenny = res->dwOriginalZenny + dwZeniBonus;
		res->dwAcquisitionZenny = res->dwOriginalZenny + dwZeniBonus;

		if (pPlayer->CanReceiveZeni(res->dwAcquisitionZenny))
		{
			//ERR_LOG(LOG_USER, "Player: %u receive %u zeni from zeni pick up", pPlayer->GetCharID(), res->dwAcquisitionZenny);

			res->wResultCode = GAME_SUCCESS;
			pPlayer->UpdateZeni(ZENNY_CHANGE_TYPE_PICK, res->dwAcquisitionZenny, true, true);

			//destroy item <async>
			g_pItemManager->DestroyItemDropOverTime(this);
		}

		packet.SetPacketLen(sizeof(sGU_ZENNY_PICK_RES));
		pPlayer->SendPacket(&packet);

		if (pPlayer->GetParty() && res->wResultCode == GAME_SUCCESS)
		{
			CNtlPacket packet2(sizeof(sGU_PARTY_MEMBER_GAINED_ZENNY_NFY));
			sGU_PARTY_MEMBER_GAINED_ZENNY_NFY * res2 = (sGU_PARTY_MEMBER_GAINED_ZENNY_NFY *)packet2.GetPacketData();
			res2->wOpCode = GU_PARTY_MEMBER_GAINED_ZENNY_NFY;
			res2->hMember = pPlayer->GetID();
			res2->bIsShared = false;
			res2->dwBonusZenny = dwZeniBonus;
			res2->dwOriginalZenny = GetZeni();
			res2->dwAcquisitionZenny = res2->dwOriginalZenny + dwZeniBonus;
			res2->dwZenny = res2->dwAcquisitionZenny + dwZeniBonus;
			packet2.SetPacketLen(sizeof(sGU_PARTY_MEMBER_GAINED_ZENNY_NFY));
			pPlayer->GetParty()->SendMsgToPartyExceptOne(&packet2, pPlayer->GetID());
		}
	}
}

//remove from ground
void CItemDrop::RemoveFromGround()
{
	CGameServer* app = (CGameServer*)g_pApp;

	m_hOwner = INVALID_HOBJECT;
	m_partyID = INVALID_PARTYID;

	SetInitialized(false);

	app->GetGameMain()->GetWorldManager()->LeaveObject(this);
}

//add to ground
bool CItemDrop::AddToGround(WORLDID Worldid, sVECTOR3& pos)
{
	if (Worldid == 0 || Worldid == INVALID_WORLDID)
	{
		ERR_LOG(LOG_USER, "wrong map index argument: %d", Worldid);
		return false;
	}

	if (GetCurWorld())
	{
		ERR_LOG(LOG_USER, "Already on world");
		return false;
	}

	CGameServer* app = (CGameServer*)g_pApp;

	SetWorldID(Worldid);
	SetCurLoc(pos);

	SetInitialized(true);

	app->GetGameMain()->GetWorldManager()->EnterObject(this, m_worldID);

	return true;
}

bool CItemDrop::IsOwnership(CPlayer* ch)
{
	if (g_pEventMgr->HasEvent(this, EVENT_ITEM_OWNERSHIP) == false)
		return true;

	return ( m_hOwner == ch->GetID() || (m_partyID != INVALID_PARTYID && m_partyID == ch->GetPartyID() )) ? true : false;
}


void CItemDrop::GenerateOptionSet(sITEM_TBLDAT* table, bool bEnchantAble)
{
	m_sOptionSet->Init();

	if (bEnchantAble)
	{
		if (CanChangeOption(table->byEquip_Type, m_byRank, ITEM_RESTRICT_STATE_TYPE_NONE, table->byNeed_Min_Level) == false)
			return;

		//CREATE RANDOM OPTION
		float fRemainingWorth = Dbo_GetItemWorth(m_byRank, table->byNeed_Min_Level);
		CNtlBitFlagManager flag;
		flag.Create(256);

		for (int i = 0; i < NTL_MAX_SYSTEM_EFFECT_COUNT; i++)
		{
			//	printf("%u | item worth %f GetRank() %u %u\n", GetTblidx(), fRemainingWorth, GetRank(), m_pProto.byRank);
			if (!g_pTableContainer->GetItemEnchantTable()->ProcessRandomOption(table, m_byRank, i == 0, i == NTL_MAX_SYSTEM_EFFECT_COUNT - 1, &flag, &fRemainingWorth, m_sOptionSet->aRandomOption[i]))
				break;
		}
	}
	else
	{
		//SET DEFINED OPTION
		if (table->bIsCanHaveOption && table->Item_Option_Tblidx != INVALID_TBLIDX)
			m_sOptionSet->aOptionTblidx[0] = table->Item_Option_Tblidx;
	}
}




//--------------------------------------------------------------------------------------//
//			===			TIMED EVENT FUNCTIONS		===
//--------------------------------------------------------------------------------------//
void CItemDrop::event_DestroyEvent()
{
	if (GetLocked() == false)
	{
		g_pItemManager->DestroyItemDropOverTime(this);
	}
}
void CItemDrop::StartDestroyEvent()
{
	if (g_pEventMgr->HasEvent(this, EVENT_ITEM_DESTROY))
		return;

	g_pEventMgr->AddEvent(this, &CItemDrop::event_DestroyEvent, EVENT_ITEM_DESTROY, NTL_TICK_OF_DROP_DISAPPEAR, -1, 0);
}


void CItemDrop::event_Ownership()
{
	g_pEventMgr->RemoveEvents(this, EVENT_ITEM_OWNERSHIP);
}
void CItemDrop::SetOwnership(HOBJECT hOwner, PARTYID partyID)
{
	m_hOwner = partyID == INVALID_PARTYID ? hOwner : INVALID_HOBJECT; // only set if party id is invalid
	m_partyID = partyID;

	g_pEventMgr->AddEvent(this, &CItemDrop::event_Ownership, EVENT_ITEM_OWNERSHIP, NTL_TICK_OF_DROP_OWNNER, 1, 0);
}
