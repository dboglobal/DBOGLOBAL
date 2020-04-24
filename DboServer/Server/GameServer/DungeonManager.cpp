#include "Stdafx.h"
#include "DungeonManager.h"
#include "TimeQuest.h"
#include "GameServer.h"
#include "TimeLeapDungeon.h"
#include "CPlayer.h"
#include "TableContainerManager.h"
#include "NtlResultCode.h"


CDungeonManager::CDungeonManager()
{
}

CDungeonManager::~CDungeonManager()
{

}


void CDungeonManager::TickProcess(DWORD dwTickDiff)
{
	for (std::map<PARTYID, CTimeQuest*>::iterator it = m_mapTimeQuest.begin(); it != m_mapTimeQuest.end(); )
	{
		CTimeQuest* pDungeon = it->second;

		if (pDungeon->TickProcess(dwTickDiff))
		{
			it = m_mapTimeQuest.erase(it);
			SAFE_DELETE(pDungeon);
		}
		else
		{
			++it;
		}
	}
}




WORD CDungeonManager::CreateTimeQuest(CPlayer* player, TBLIDX id, BYTE byDifficulty, BYTE byMode)
{
	sTIMEQUEST_TBLDAT* pTimeQuestTbldat = (sTIMEQUEST_TBLDAT*)g_pTableContainer->GetTimeQuestTable()->FindData(id);
	if (pTimeQuestTbldat == NULL)
	{
		printf("Could not find pTimeQuestTbldat. Tblidx %u\n", id);
		return GAME_TIMEQUEST_ERROR;
	}

	PARTYID partyID = (byMode == TIMEQUEST_MODE_PARTY) ? player->GetPartyID() : 50000 + player->GetCharID(); //party id can never be 12000 or higher

	if (FindTimeQuest(partyID)) //check if party already exist
	{
		return GAME_TIMEQUEST_ALREADY_JOINED;
	}

	CItem* pItem = NULL;

	if (byMode == TIMEQUEST_MODE_PARTY)
	{
		CParty* pParty = player->GetParty();

		pItem = player->GetPlayerItemContainer()->GetItemByIdx(pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].needItemTblidx);

		if (pItem == NULL)
			return GAME_TIMEQUEST_MUST_HAVE_NEED_ITEM;
		else if (pParty == NULL)
			return GAME_PARTY_YOU_ARE_NOT_IN_PARTY;
		else if (player->GetZeni() < pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].dwNeedZenny)
			return GAME_TIMEQUEST_MUST_HAVE_NEED_ZENNY;
		else if (pParty->GetPartyMemberCount() > pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].byMaxMemberCount) //check max member count
			return GAME_TIMEQUEST_EXCEED_MAX_MEMBER_COUNT;
		else if (pParty->GetPartyMemberCount() < pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].byMinMemberCount) //check min member count
			return GAME_TIMEQUEST_NEED_MORE_MEMBER;
		else
		{
			for (int i = 0; i < pParty->GetPartyMemberCount(); i++)
			{
				CPlayer* pMember = g_pObjectManager->GetPC(pParty->GetMemberInfo(i).hHandle);
				if (pMember && pMember->IsInitialized())
				{
					if (pMember->GetLevel() < pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].byMinMemberLevel) //check min level
						return GAME_TOO_LOW_MEMBER_LEVEL;
					else if (pMember->GetLevel() > pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].byMaxMemberLevel) //check max level
						return GAME_TOO_HIGHT_MEMBER_LEVEL;
				}
			}
		}
	}

	CTimeQuest* pTimeQuest = new CTimeQuest(partyID, pTimeQuestTbldat, byDifficulty, byMode);

	if (pTimeQuest->Create(player))
	{
		m_mapTimeQuest.insert({ partyID, pTimeQuest });

		//remove required zeni
		if (pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].dwNeedZenny > 0)
			player->UpdateZeni(ZENNY_CHANGE_TYPE_ITEM_BUY, pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].dwNeedZenny, false, true);

		//remove required item
		if(pItem && pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].needItemTblidx != INVALID_TBLIDX)
			pItem->SetCount(pItem->GetCount() - 1, false, true);
	}
	else
	{
		SAFE_DELETE(pTimeQuest);
		return GAME_TIMEQUEST_ERROR;
	}

	return GAME_SUCCESS;
}


CTimeQuest* CDungeonManager::FindTimeQuest(PARTYID partyid)
{
	std::map<ROOMID, CTimeQuest*>::iterator it = m_mapTimeQuest.find(partyid);

	if (m_mapTimeQuest.end() == it){
		return NULL;
	}

	return it->second;
}



CTimeLeapDungeon* CDungeonManager::CreateTimeLeapDungeon(CPlayer* pOwner, TBLIDX dungeonTblidx)
{
	sDUNGEON_TBLDAT* pDungeon = (sDUNGEON_TBLDAT*)g_pTableContainer->GetDungeonTable()->FindData(dungeonTblidx);
	if (pDungeon == NULL)
	{
		printf("Could not find dungeon tblidx %u \n", dungeonTblidx);
		return NULL;
	}

	//TODO: Checks if player has valid loc and can enter dungeon... etc..

	CTimeLeapDungeon* pTLD = new CTimeLeapDungeon(pOwner, pDungeon);

	if (pTLD->Create())
	{
		m_mapTimeLeapDungeon.insert({ pOwner->GetCharID(), pTLD });

		return pTLD;
	}

	SAFE_DELETE(pTLD);

	return NULL;
}


CTimeLeapDungeon* CDungeonManager::FindTimeLeapDungeon(CHARACTERID charID)
{
	std::map<CHARACTERID, CTimeLeapDungeon*>::iterator it = m_mapTimeLeapDungeon.find(charID);

	if (m_mapTimeLeapDungeon.end() == it){
		return NULL;
	}

	return it->second;
}


void CDungeonManager::DestroyTimeLeapDungeon(CHARACTERID charID, CTimeLeapDungeon* pDungeon)
{
	m_mapTimeLeapDungeon.erase(charID);
	SAFE_DELETE(pDungeon);
}




CUltimateDungeon* CDungeonManager::CreateUltimateDungeon(CPlayer* pOwner, GROUPID groupid, BYTE byDifficulty)
{
	sDUNGEON_TBLDAT* pDungeon = (sDUNGEON_TBLDAT*)g_pTableContainer->GetDungeonTable()->FindDataByGroupIdx(groupid, byDifficulty);
	if (pDungeon == NULL)
	{
		printf("Could not find dungeon groupid %u byDifficulty %u\n", groupid, byDifficulty);
		return NULL;
	}

	if (pOwner->GetLevel() < pDungeon->byMinLevel || pOwner->GetLevel() > pDungeon->byMaxLevel)
		return NULL;

	if (FindUltimateDungeon(pOwner->GetPartyID()) != NULL)
		return NULL;

	CUltimateDungeon* pUD = new CUltimateDungeon(pOwner->GetPartyID(), pDungeon);

	if (pUD->Create(pOwner))
	{
		m_mapUltimateDungeon.insert({ pOwner->GetPartyID(), pUD });

		return pUD;
	}

	SAFE_DELETE(pUD);

	return NULL;
}


CUltimateDungeon* CDungeonManager::FindUltimateDungeon(PARTYID partyID)
{
	std::map<PARTYID, CUltimateDungeon*>::iterator it = m_mapUltimateDungeon.find(partyID);

	if (m_mapUltimateDungeon.end() == it){
		return NULL;
	}

	return it->second;
}


void CDungeonManager::DestroyUltimateDungeon(CUltimateDungeon* pDungeon)
{
	m_mapUltimateDungeon.erase(pDungeon->GetPartyID());
	SAFE_DELETE(pDungeon);
}

CBattleDungeon * CDungeonManager::CreateBattleDungeon(CPlayer * pOwner, WORD & rwResultcode, BYTE byBeginStage)
{
	if (FindBattleDungeon(pOwner->GetPartyID()) != NULL)
	{
		rwResultcode = GAME_BATTLE_DUNGEON_FAIL;
		return NULL;
	}

	CBattleDungeon* pCCBD = new CBattleDungeon(pOwner->GetPartyID());

	if (pCCBD->Create(pOwner, byBeginStage))
	{
		m_mapBattleDungeon.insert({ pOwner->GetPartyID(), pCCBD });

		return pCCBD;
	}

	SAFE_DELETE(pCCBD);

	return NULL;
}

CBattleDungeon * CDungeonManager::FindBattleDungeon(PARTYID partyID)
{
	std::map<PARTYID, CBattleDungeon*>::iterator it = m_mapBattleDungeon.find(partyID);

	if (m_mapBattleDungeon.end() == it) {
		return NULL;
	}

	return it->second;
}

void CDungeonManager::DestroyBattleDungeon(CBattleDungeon * pDungeon)
{
	m_mapBattleDungeon.erase(pDungeon->GetPartyID());
	SAFE_DELETE(pDungeon);
}



