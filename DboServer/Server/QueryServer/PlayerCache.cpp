#include "stdafx.h"
#include "PlayerCache.h"
#include "NtlAdmin.h"
#include "QueryServer.h"
#include "NtlPacketQG.h"
#include "NtlPacketQC.h"
#include "Utils.h"


bool sortByPlace(const sITEM_DATA *lhs, const sITEM_DATA *rhs) { return lhs->byPlace < rhs->byPlace; }


CAccountCache::CAccountCache(ACCOUNTID accountID):
	m_accountID(accountID)
{
	Init();
}

CAccountCache::~CAccountCache()
{
	Destroy();
}

void CAccountCache::Init()
{
	m_hServerSession = INVALID_HSESSION;
	m_bIsBankLoaded = false;
	m_byGameMasterLevel = ADMIN_LEVEL_NONE;
}

void CAccountCache::Destroy()
{
	m_setCharId.clear();

	for (std::map<QWORD, sCASHITEM_BRIEF*>::iterator it = m_mapCashItems.begin(); it != m_mapCashItems.end(); )
	{
		delete it->second;

		it = m_mapCashItems.erase(it);
	}

	m_mapShortcutInfo.clear();

}


void CAccountCache::OnLoadAccountInfo(QueryResultVector & results)
{
	QueryResult* qrAccountInfo = results[0].result;
	if (qrAccountInfo)
	{
		Field* f = qrAccountInfo->Fetch();

		m_dwCash = f[0].GetDWORD();
		m_byGameMasterLevel = f[1].GetBYTE();
		m_byPremiumSlots = f[2].GetBYTE();
		m_dwEventCoins = f[3].GetDWORD();
		m_dwWaguCoins = f[4].GetDWORD();
	}


	QueryResult* qrCashItemInfo = results[1].result;
	if (qrCashItemInfo)
	{
		do
		{
			Field* f = qrCashItemInfo->Fetch();

			sCASHITEM_BRIEF* pCashItem = new sCASHITEM_BRIEF;
			
			pCashItem->qwProductId = f[0].GetUInt64();
			pCashItem->HLSitemTblidx = f[2].GetUInt32();
			pCashItem->byStackCount = f[3].GetBYTE();
			wcscpy_s(pCashItem->wchSenderName, NTL_MAX_SIZE_CHAR_NAME + 1, s2ws(f[6].GetString()).c_str());
			pCashItem->tRegTime.year = f[7].GetWORD();
			pCashItem->tRegTime.month = f[8].GetBYTE();
			pCashItem->tRegTime.day = f[9].GetBYTE();
			pCashItem->tRegTime.hour = f[10].GetBYTE();
			pCashItem->tRegTime.minute = f[11].GetBYTE();
			pCashItem->tRegTime.second = f[12].GetBYTE();

			m_mapCashItems.insert(std::make_pair(pCashItem->qwProductId, pCashItem));

		}
		while (qrCashItemInfo->NextRow());
	}


	QueryResult* qrShortcutInfo = results[2].result;
	if (qrShortcutInfo)
	{
		do
		{
			Field* f = qrShortcutInfo->Fetch();

			m_mapShortcutInfo.insert(std::make_pair(f[0].GetWORD(), f[1].GetWORD())); //ActionID, wKey
		}
		while (qrShortcutInfo->NextRow());
	}


	CNtlPacket pQry(sizeof(sQC_CHARACTER_LOAD_RES));
	sQC_CHARACTER_LOAD_RES * qRes = (sQC_CHARACTER_LOAD_RES *)pQry.GetPacketData();
	qRes->wOpCode = QC_CHARACTER_LOAD_RES;
	qRes->accountId = this->m_accountID;
	pQry.SetPacketLen(sizeof(sQC_CHARACTER_LOAD_RES));
	g_pApp->Send(GetSession(), &pQry);
}

void CAccountCache::OnLoadEventReward(QueryResultVector & results, HOBJECT hHandle, CHARACTERID charId)
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packetRes(sizeof(sQG_EVENT_REWARD_LOAD_RES));
	sQG_EVENT_REWARD_LOAD_RES * res = (sQG_EVENT_REWARD_LOAD_RES *)packetRes.GetPacketData();
	res->wOpCode = QG_EVENT_REWARD_LOAD_RES;
	res->wResultCode = 500; // GAME_SUCCESS
	res->handle = hHandle;
	res->charId = charId;
	res->byCount = 0;

	QueryResult* qrEventReward = results[0].result;
	if (qrEventReward)
	{
		do
		{
			Field* f = qrEventReward->Fetch();

			res->asInfo[res->byCount].charId = f[1].GetUInt32();
			res->asInfo[res->byCount].eventTblidx = f[0].GetUInt32();

			const WCHAR* wchName = s2ws(f[2].GetString()).c_str();

			res->asNameInfo[res->byCount].charId = res->asInfo[res->byCount].charId;
			NTL_SAFE_WCSCPY(res->asNameInfo[res->byCount].awchName, wchName);

			m_mapEventReward.insert({ res->asInfo[res->byCount].eventTblidx, sEVENT_PC_NAME(res->asInfo[res->byCount].charId, wchName) });

			res->byCount += 1;

		} while (qrEventReward->NextRow());
	}
	
	app->Send(GetSession(), &packetRes);
}

void CAccountCache::SendShortcutLoadRes(CHARACTERID charId)
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_PC_SHORTCUT_LOAD_RES));
	sQG_PC_SHORTCUT_LOAD_RES * res = (sQG_PC_SHORTCUT_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_SHORTCUT_LOAD_RES;
	res->charID = charId;
	res->wResultCode = 500;

	for (std::map<WORD, WORD>::iterator it = m_mapShortcutInfo.begin(); it != m_mapShortcutInfo.end(); it++)
	{
		res->asShortcutData[res->byShortcutCount].wActionID = it->first;
		res->asShortcutData[res->byShortcutCount++].wKey = it->second;

		if (res->byShortcutCount == NTL_SHORTCUT_MAX_COUNT)
			break;
	}

	packet.SetPacketLen(sizeof(sQG_PC_SHORTCUT_LOAD_RES));
	app->Send(GetSession(), &packet);
}

void CAccountCache::SendCashshopLoadRes(CHARACTERID charId, HOBJECT hHandle)
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	int nTotal = (int)m_mapCashItems.size();
	if (nTotal == 0)
		return;

	int i = 0;

	CNtlPacket packet(sizeof(sQG_CASHITEM_INFO));
	sQG_CASHITEM_INFO * res = (sQG_CASHITEM_INFO *)packet.GetPacketData();
	res->wOpCode = QG_CASHITEM_INFO;
	res->charId = charId;
	res->handle = hHandle;

	for (std::map<QWORD, sCASHITEM_BRIEF*>::iterator it = m_mapCashItems.begin(); it != m_mapCashItems.end(); it++)
	{
		memcpy(&res->aInfo[res->byCount++], it->second, sizeof(sCASHITEM_BRIEF));
		i++;

		if (res->byCount == DBO_MAX_CASHITEM_COUNT_IN_PACKET_SERVER || i == nTotal)
		{
			packet.SetPacketLen(sizeof(sQG_CASHITEM_INFO));
			app->Send(GetSession(), &packet);

			res->byCount = 0;
			memset(res->aInfo, NULL, sizeof(res->aInfo));
		}
	}
}

bool CAccountCache::RemoveCashItem(QWORD qwProductId)
{
	std::map<QWORD, sCASHITEM_BRIEF*>::iterator it = m_mapCashItems.find(qwProductId);
	if (it != m_mapCashItems.end())
	{
		sCASHITEM_BRIEF* pBrief = it->second;
		delete pBrief;
		m_mapCashItems.erase(it);

		return true;
	}

	return false;
}

bool CAccountCache::CashItemDecreaseCount(QWORD qwProductId, bool& rbDelete)
{
	std::map<QWORD, sCASHITEM_BRIEF*>::iterator it = m_mapCashItems.find(qwProductId);
	if (it != m_mapCashItems.end())
	{
		sCASHITEM_BRIEF* pBrief = it->second;

		if (pBrief->byStackCount == 1)
		{
			delete pBrief;
			m_mapCashItems.erase(it);
			rbDelete = true;
		}
		else
		{
			pBrief->byStackCount -= 1;
			rbDelete = false;
		}

		return true;
	}

	return false;
}

void CAccountCache::InsertCashItem(sCASHITEM_BRIEF * pBrief)
{
	m_mapCashItems.insert(std::make_pair(pBrief->qwProductId, pBrief));
}

void CAccountCache::AddShortcut(WORD wActionID, WORD wKey)
{
	m_mapShortcutInfo.insert(std::make_pair(wActionID, wKey));
}

void CAccountCache::DeleteShortcut(WORD wActionID)
{
	m_mapShortcutInfo.erase(wActionID);
}

bool CAccountCache::UpdateShortcut(WORD wActionID, WORD wNewkey)
{
	std::map<WORD, WORD>::iterator it = m_mapShortcutInfo.find(wActionID);
	if (it != m_mapShortcutInfo.end())
	{
		it->second = wNewkey;
		return true;
	}

	return false;
}

void CAccountCache::AddBankItem(sITEM_DATA & rData)
{
	sITEM_DATA* pData = new sITEM_DATA;
	memcpy(pData, &rData, sizeof(sITEM_DATA));

	m_mapBank.insert(std::make_pair(pData->itemId, pData));
}

void CAccountCache::EraseBankItem(ITEMID itemId)
{
	m_mapBank.erase(itemId);
}

void CAccountCache::InsertBankItem(sITEM_DATA* pData)
{
	m_mapBank.insert(std::make_pair(pData->itemId, pData));
}

sITEM_DATA * CAccountCache::GetBankItem(ITEMID itemId, BYTE byPlace, BYTE byPos)
{
	std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapBank.find(itemId);
	if (it != m_mapBank.end())
	{
		if (it->second->byPlace == byPlace && it->second->byPosition == byPos)
			return it->second;
	}

	return NULL;
}

sITEM_DATA * CAccountCache::GetBankItem(TBLIDX tblidx)
{
	for (std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapBank.begin(); it != m_mapBank.end(); it++)
	{
		if (it->second->itemNo == tblidx)
			return it->second;
	}

	return nullptr;
}

void CAccountCache::SendBankItemLoad(CHARACTERID charId)
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_LOAD_PC_BANK_DATA));
	sQG_LOAD_PC_BANK_DATA * res = (sQG_LOAD_PC_BANK_DATA *)packet.GetPacketData();
	res->wOpCode = QG_LOAD_PC_BANK_DATA;
	res->charID = charId;
	res->byCurPacketCount = 0;
	res->byItemCount = 0;
	res->byTotalCount = (BYTE)m_mapBank.size();

	if (res->byTotalCount > 0)
	{
		/*
			When we send items to gameserver/client, then we have to sort the items by PLACE first.. Otherwise it can have fatal issues and client crashes. Client always must load inventory first, etc..
		*/

		std::vector<sITEM_DATA*> tempVec;

		//insert all items to temp vector
		std::for_each(m_mapBank.begin(), m_mapBank.end(),
			[&tempVec](const std::map<ITEMID, sITEM_DATA*>::value_type& p)
		{ tempVec.push_back(p.second); });

		//sort vector by place
		std::sort(tempVec.begin(), tempVec.end(), sortByPlace);

		for (std::vector<sITEM_DATA*>::iterator it = tempVec.begin(); it != tempVec.end(); it++)
		{
			sITEM_DATA* pItem = *it;

			memcpy(&res->asItemData[res->byItemCount++], pItem, sizeof(sITEM_DATA));

			res->byCurPacketCount++;

			if (res->byItemCount == NTL_MAX_BANK_ITEM_SLOT || res->byCurPacketCount == res->byTotalCount)
			{
				packet.SetPacketLen(sizeof(sQG_LOAD_PC_BANK_DATA));
				app->Send(GetSession(), &packet);

				::ZeroMemory(res->asItemData, sizeof(sITEM_DATA) * NTL_MAX_BANK_ITEM_SLOT);
				res->byItemCount = 0;
			}
		}
	}
}

bool CAccountCache::HasEventReward(TBLIDX rewardTblidx, CHARACTERID charId)
{
	std::map<TBLIDX, sEVENT_PC_NAME>::iterator it = m_mapEventReward.find(rewardTblidx);
	if (it != m_mapEventReward.end())
	{
		if (it->second.charId == 0 || it->second.charId == charId)
			return true;
	}

	return false;
}



CPlayerCache::CPlayerCache(ACCOUNTID accountID, CAccountCache* pAccountCache):
	m_accountID(accountID), m_pAccountCache(pAccountCache)
{
	Init();
}

CPlayerCache::~CPlayerCache()
{
	Destroy();
}

void CPlayerCache::Init()
{
	m_hServerSession = INVALID_HSESSION;
	m_bIsQuickTeleportLoaded = false;
	m_bIsBankLoaded = false;
	::ZeroMemory(&m_sPcData, sizeof(m_sPcData));
	m_sPcData.wJoinID = INVALID_JOINID;
	m_serverChangeInfo.Init();
	m_bTutorialFlag = false;
	::ZeroMemory(&m_sWarFogInfo, sizeof(m_sWarFogInfo));
	::ZeroMemory(&m_sMailBrief, sizeof(m_sMailBrief));
	::ZeroMemory(&m_sRankBattleScore, sizeof(m_sRankBattleScore));
	::ZeroMemory(m_abyTitleIndexFlag, sizeof(m_abyTitleIndexFlag));
	::ZeroMemory(&m_sQuestCompleteData, sizeof(m_sQuestCompleteData));

	m_tNextSkillReset = INVALID_DBOTIME;
}

void CPlayerCache::Destroy()
{
	for (std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapItems.begin(); it != m_mapItems.end(); )
	{
		delete it->second;
		it = m_mapItems.erase(it);
	}

	for (std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapBank.begin(); it != m_mapBank.end(); )
	{
		delete it->second;
		it = m_mapBank.erase(it);
	}

	for (std::map<BYTE, sSKILL_DATA*>::iterator it = m_mapSkills.begin(); it != m_mapSkills.end(); )
	{
		delete it->second;
		it = m_mapSkills.erase(it);
	}

	for (std::map<BYTE, sBUFF_DATA*>::iterator it = m_mapBuffs.begin(); it != m_mapBuffs.end(); )
	{
		delete it->second;
		it = m_mapBuffs.erase(it);
	}

	for (std::map<TBLIDX, sHTB_SKILL_DATA*>::iterator it = m_mapHtbSkills.begin(); it != m_mapHtbSkills.end(); )
	{
		delete it->second;
		it = m_mapHtbSkills.erase(it);
	}

	for (std::map<BYTE, sQUESTITEM_DATA*>::iterator it = m_mapQuestItems.begin(); it != m_mapQuestItems.end(); )
	{
		delete it->second;
		it = m_mapQuestItems.erase(it);
	}

	for (std::map<QUESTID, sPROGRESS_QUEST_INFO*>::iterator it = m_mapQuestProgressInfo.begin(); it != m_mapQuestProgressInfo.end(); )
	{
		delete it->second;
		it = m_mapQuestProgressInfo.erase(it);
	}

	m_mapRecipes.clear();

	for (std::map<BYTE, sQUICK_TELEPORT_INFO*>::iterator it = m_mapQuickTeleport.begin(); it != m_mapQuickTeleport.end(); )
	{
		delete it->second;
		it = m_mapQuickTeleport.erase(it);
	}

	for (std::map<BYTE, sMASCOT_DATA_EX*>::iterator it = m_mapMascots.begin(); it != m_mapMascots.end(); )
	{
		delete it->second;
		it = m_mapMascots.erase(it);
	}

	m_mapItemCoolTime.clear();

	for (std::map<TBLIDX, sEVENT_REWARD_INFO*>::iterator it = m_mapEventRewardInfo.begin(); it != m_mapEventRewardInfo.end(); )
	{
		delete it->second;
		it = m_mapEventRewardInfo.erase(it);
	}

	for (std::map<BYTE, sQUICK_SLOT_DATA*>::iterator it = m_mapQuickSlotInfo.begin(); it != m_mapQuickSlotInfo.end(); )
	{
		delete it->second;
		it = m_mapQuickSlotInfo.erase(it);
	}

	m_setPortalPoints.clear();

	for (std::map<MAILID, sMAIL_DATA*>::iterator it = m_mapMail.begin(); it != m_mapMail.end(); )
	{
		delete it->second;
		it = m_mapMail.erase(it);
	}


	m_pAccountCache = NULL;
}


void CPlayerCache::SendPcDataLoadRes()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	if (m_sPcData.bIsScrambleJoinFlag == true && app->GetDragonballScrambleBegin() == false)
	{
		m_sPcData.bIsScrambleJoinFlag = false;
		m_sPcData.bIsScrambleRewardedFlag = false;
	}

	CNtlPacket packet(sizeof(sQG_PC_DATA_LOAD_RES));
	sQG_PC_DATA_LOAD_RES * res = (sQG_PC_DATA_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_DATA_LOAD_RES;
	res->wResultCode = 500;

	res->accountId = m_accountID;
	memcpy(&res->sPcData, &m_sPcData, sizeof(sPC_DATA));
	memcpy(&res->serverChangeInfo, &m_serverChangeInfo, sizeof(sDBO_SERVER_CHANGE_INFO));
	res->bTutorialFlag = m_bTutorialFlag;
	memcpy(&res->sWarFogInfo, &m_sWarFogInfo, sizeof(sCHAR_WAR_FOG_FLAG));
	memcpy(&res->sMailBrief, &m_sMailBrief, sizeof(sMAIL_NEW_BREIF));
	memcpy(&res->sRankBattleScore, &m_sRankBattleScore, sizeof(sRANKBATTLE_SCORE_INFO));
	memcpy(res->TitleIndexFlag, m_abyTitleIndexFlag, sizeof(m_abyTitleIndexFlag));
	res->wWaguCoins = (WORD)m_pAccountCache->GetWaguCoin();

	packet.SetPacketLen(sizeof(sQG_PC_DATA_LOAD_RES));
	app->Send(GetSession(), &packet);
}


void CPlayerCache::SendSkillLoadRes()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_PC_SKILL_LOAD_RES));
	sQG_PC_SKILL_LOAD_RES * res = (sQG_PC_SKILL_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_SKILL_LOAD_RES;
	res->charID = m_sPcData.charId;
	res->wResultCode = 500;

	for (std::map<BYTE, sSKILL_DATA*>::iterator it = m_mapSkills.begin(); it != m_mapSkills.end(); it++)
	{
		memcpy(&res->asSkill[res->bySkillCount++], it->second, sizeof(sSKILL_DATA));

		if (res->bySkillCount == NTL_MAX_PC_HAVE_SKILL)
			break;
	}

	packet.SetPacketLen(sizeof(sQG_PC_SKILL_LOAD_RES));
	app->Send(GetSession(), &packet);
}

void CPlayerCache::SendBuffLoadRes()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_PC_BUFF_LOAD_RES));
	sQG_PC_BUFF_LOAD_RES * res = (sQG_PC_BUFF_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_BUFF_LOAD_RES;
	res->charID = m_sPcData.charId;
	res->wResultCode = 500;

	for (std::map<BYTE, sBUFF_DATA*>::iterator it = m_mapBuffs.begin(); it != m_mapBuffs.end(); it++)
	{
		memcpy(&res->asBuff[res->byBuffCount++], it->second, sizeof(sBUFF_DATA));

		if (res->byBuffCount == DBO_MAX_BUFF_CHARACTER_HAS)
			break;
	}

	packet.SetPacketLen(sizeof(sQG_PC_BUFF_LOAD_RES));
	app->Send(GetSession(), &packet);
}

void CPlayerCache::SendHtbSkillLoadRes()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_PC_HTB_LOAD_RES));
	sQG_PC_HTB_LOAD_RES * res = (sQG_PC_HTB_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_HTB_LOAD_RES;
	res->charID = m_sPcData.charId;
	res->wResultCode = 500;

	for (std::map<TBLIDX, sHTB_SKILL_DATA*>::iterator it = m_mapHtbSkills.begin(); it != m_mapHtbSkills.end(); it++)
	{
		memcpy(&res->asHTBSkill[res->byHTBSkillCount++], it->second, sizeof(sHTB_SKILL_DATA));

		if (res->byHTBSkillCount == NTL_HTB_MAX_PC_HAVE_HTB_SKILL)
			break;
	}

	packet.SetPacketLen(sizeof(sQG_PC_HTB_LOAD_RES));
	app->Send(GetSession(), &packet);
}

void CPlayerCache::SendItemLoadRes()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_PC_ITEM_LOAD_RES));
	sQG_PC_ITEM_LOAD_RES * res = (sQG_PC_ITEM_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_ITEM_LOAD_RES;
	res->charID = m_sPcData.charId;
	res->wResultCode = 500;
	res->bHaveNetPosition = false;
	res->wTotalCount = (WORD)m_mapItems.size();
	res->wTotalPacketCount = 0;
	res->wCurPacketCount = 0;
	res->byItemCount = 0;

	if (res->wTotalCount == 0)
	{
		res->wTotalPacketCount = 1;
		res->wCurPacketCount = 1;

		packet.SetPacketLen(sizeof(sQG_PC_ITEM_LOAD_RES));
		app->Send(GetSession(), &packet);
	}
	else
	{
		/*
			When we send items to gameserver/client, then we have to sort the items by PLACE first.. Otherwise it can have fatal issues and client crashes. Client always must load inventory first, etc..
		*/

		std::vector<const sITEM_DATA*> tempVec;

		//insert all items to temp vector
		std::for_each(m_mapItems.begin(), m_mapItems.end(),
			[&tempVec](const std::map<ITEMID, const sITEM_DATA*>::value_type& p)
		{ tempVec.push_back(p.second); });

		//sort vector by place
		std::sort(tempVec.begin(), tempVec.end(), sortByPlace);

		for (std::vector<const sITEM_DATA*>::iterator it = tempVec.begin(); it != tempVec.end(); it++)
		{
			const sITEM_DATA* pItem = *it;

			if (pItem->byPlace > CONTAINER_TYPE_INVEN_LAST) //dont load bank items.
			{
				if (res->byItemCount > 0)
				{
					packet.SetPacketLen(sizeof(sQG_PC_ITEM_LOAD_RES));
					app->Send(GetSession(), &packet);
				}

				break;
			}

			memcpy(&res->asItemData[res->byItemCount++], pItem, sizeof(sITEM_DATA));

			res->wCurPacketCount++;

			if (res->byItemCount == DBO_ITEM_IN_BAG_COUNT_PER_PACKET || res->wCurPacketCount == res->wTotalCount)
			{
				res->wTotalPacketCount++;

				packet.SetPacketLen(sizeof(sQG_PC_ITEM_LOAD_RES));
				app->Send(GetSession(), &packet);

				::ZeroMemory(res->asItemData, sizeof(sITEM_DATA) * DBO_ITEM_IN_BAG_COUNT_PER_PACKET);
				res->byItemCount = 0;
			}
		}
	}
}

void CPlayerCache::SendQuestItemLoadRes()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_PC_QUEST_ITEM_LOAD_RES));
	sQG_PC_QUEST_ITEM_LOAD_RES * res = (sQG_PC_QUEST_ITEM_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_QUEST_ITEM_LOAD_RES;
	res->charID = m_sPcData.charId;
	res->wResultCode = 500;

	for (std::map<BYTE, sQUESTITEM_DATA*>::iterator it = m_mapQuestItems.begin(); it != m_mapQuestItems.end(); it++)
	{
		memcpy(&res->asQuestInventory[res->byQuestInventoryCount++], it->second, sizeof(sQUESTITEM_DATA));

		if (res->byQuestInventoryCount == NTL_QUEST_INVENTORY_SLOT_COUNT)
			break;
	}

	packet.SetPacketLen(sizeof(sQG_PC_QUEST_ITEM_LOAD_RES));
	app->Send(GetSession(), &packet);
}

void CPlayerCache::SendQuestCompleteLoadRes()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_PC_QUEST_COMPLETE_LOAD_RES));
	sQG_PC_QUEST_COMPLETE_LOAD_RES * res = (sQG_PC_QUEST_COMPLETE_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_QUEST_COMPLETE_LOAD_RES;
	res->charID = m_sPcData.charId;
	res->wResultCode = 500;
	memcpy(&res->sQuestCompleteData, &m_sQuestCompleteData, sizeof(m_sQuestCompleteData));
	app->Send(GetSession(), &packet);
}

void CPlayerCache::SendQuestProgressLoadRes()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_PC_QUEST_PROGRESS_LOAD_RES));
	sQG_PC_QUEST_PROGRESS_LOAD_RES * res = (sQG_PC_QUEST_PROGRESS_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_QUEST_PROGRESS_LOAD_RES;
	res->charID = m_sPcData.charId;
	res->wResultCode = 500;

	for (std::map<QUESTID, sPROGRESS_QUEST_INFO*>::iterator it = m_mapQuestProgressInfo.begin(); it != m_mapQuestProgressInfo.end(); it++)
	{
		memcpy(&res->asQuestProgress[res->byQuestProgressCount++], it->second, sizeof(sPROGRESS_QUEST_INFO));

		if (res->byQuestProgressCount == eMAX_CAN_PROGRESS_QUEST_NUM)
			break;
	}

	packet.SetPacketLen(sizeof(sQG_PC_QUEST_PROGRESS_LOAD_RES));
	app->Send(GetSession(), &packet);
}

void CPlayerCache::SendQuickSlotLoadRes()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_PC_QUICK_SLOT_LOAD_RES));
	sQG_PC_QUICK_SLOT_LOAD_RES * res = (sQG_PC_QUICK_SLOT_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_QUICK_SLOT_LOAD_RES;
	res->charID = m_sPcData.charId;
	res->wResultCode = 500;
	res->byQuickSlotCount = 0;

	for (std::map<BYTE, sQUICK_SLOT_DATA*>::iterator it = m_mapQuickSlotInfo.begin(); it != m_mapQuickSlotInfo.end(); it++)
	{
		memcpy(&res->asQuickSlotData[res->byQuickSlotCount++], it->second, sizeof(sQUICK_SLOT_DATA));

		if (res->byQuickSlotCount == NTL_CHAR_QUICK_SLOT_MAX_COUNT)
			break;
	}

	packet.SetPacketLen(sizeof(sQG_PC_QUICK_SLOT_LOAD_RES));
	app->Send(GetSession(), &packet);
}

void CPlayerCache::SendItemRecipeLoadRes()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_PC_ITEM_RECIPE_LOAD_RES));
	sQG_PC_ITEM_RECIPE_LOAD_RES * res = (sQG_PC_ITEM_RECIPE_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_ITEM_RECIPE_LOAD_RES;
	res->charID = m_sPcData.charId;
	res->wResultCode = 500;

	for (std::map<TBLIDX, BYTE>::iterator it = m_mapRecipes.begin(); it != m_mapRecipes.end(); it++)
	{
		res->asRecipeData[res->wRecipeCount].byRecipeType = it->second;
		res->asRecipeData[res->wRecipeCount++].recipeTblidx = it->first;

		if (res->wRecipeCount == NTL_ITEM_RECIPE_MAX_COUNT)
			break;
	}

	packet.SetPacketLen(sizeof(sQG_PC_ITEM_RECIPE_LOAD_RES));
	app->Send(GetSession(), &packet);
}

void CPlayerCache::SendMascotLoadRes()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_PC_MASCOT_LOAD_RES));
	sQG_PC_MASCOT_LOAD_RES * res = (sQG_PC_MASCOT_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_MASCOT_LOAD_RES;
	res->charID = m_sPcData.charId;
	res->wResultCode = 500;

	for (std::map<BYTE, sMASCOT_DATA_EX*>::iterator it = m_mapMascots.begin(); it != m_mapMascots.end(); it++)
	{
		memcpy(&res->asMascotData[res->byMascotCount++], it->second, sizeof(sMASCOT_DATA_EX));

		if (res->byMascotCount == GMT_MAX_PC_HAVE_MASCOT)
			break;
	}

	packet.SetPacketLen(sizeof(sQG_PC_MASCOT_LOAD_RES));
	app->Send(GetSession(), &packet);
}

void CPlayerCache::SendItemCoolTimeLoadRes()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_PC_ITEM_COOL_TIME_LOAD_RES));
	sQG_PC_ITEM_COOL_TIME_LOAD_RES * res = (sQG_PC_ITEM_COOL_TIME_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_ITEM_COOL_TIME_LOAD_RES;
	res->charID = m_sPcData.charId;
	res->wResultCode = 500;

	for (std::map<BYTE, sDBO_ITEM_COOL_TIME>::iterator it = m_mapItemCoolTime.begin(); it != m_mapItemCoolTime.end(); it++)
	{
		memcpy(&res->asItemCoolTimeData[res->byItemCoolTimeCount++], &it->second, sizeof(sDBO_ITEM_COOL_TIME));

		if (res->byItemCoolTimeCount == NTL_MAX_ITEM_COOL_DOWN)
			break;
	}

	packet.SetPacketLen(sizeof(sQG_PC_ITEM_COOL_TIME_LOAD_RES));
	app->Send(GetSession(), &packet);
}

void CPlayerCache::SendPortalLoadRes()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_PORTAL_START_RES));
	sQG_PORTAL_START_RES * res = (sQG_PORTAL_START_RES *)packet.GetPacketData();
	res->wOpCode = QG_PORTAL_START_RES;
	res->charID = m_sPcData.charId;
	res->wResultCode = 500;

	for (std::set<PORTALID>::iterator it = m_setPortalPoints.begin(); it != m_setPortalPoints.end(); it++)
	{
		res->aPortalID[res->byCount++] = *it;

		if (res->byCount == NTL_PORTAL_MAX_COUNT)
			break;
	}

	packet.SetPacketLen(sizeof(sQG_PORTAL_START_RES));
	app->Send(GetSession(), &packet);
}

void CPlayerCache::SendBankItemLoad()
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_LOAD_PC_BANK_DATA));
	sQG_LOAD_PC_BANK_DATA * res = (sQG_LOAD_PC_BANK_DATA *)packet.GetPacketData();
	res->wOpCode = QG_LOAD_PC_BANK_DATA;
	res->charID = m_sPcData.charId;
	res->byCurPacketCount = 0;
	res->byItemCount = 0;
	res->byTotalCount = (BYTE)m_mapBank.size();

	if (res->byTotalCount == 0)
	{
		res->byCurPacketCount = 1;

		packet.SetPacketLen(sizeof(sQG_LOAD_PC_BANK_DATA));
		app->Send(GetSession(), &packet);
	}
	else
	{
		/*
		When we send items to gameserver/client, then we have to sort the items by PLACE first.. Otherwise it can have fatal issues and client crashes. Client always must load inventory first, etc..
		*/

		std::vector<sITEM_DATA*> tempVec;

		//insert all items to temp vector
		std::for_each(m_mapBank.begin(), m_mapBank.end(),
			[&tempVec](const std::map<ITEMID, sITEM_DATA*>::value_type& p)
		{ tempVec.push_back(p.second); });

		//sort vector by place
		std::sort(tempVec.begin(), tempVec.end(), sortByPlace);

		for (std::vector<sITEM_DATA*>::iterator it = tempVec.begin(); it != tempVec.end(); it++)
		{
			sITEM_DATA* pItem = *it;

			memcpy(&res->asItemData[res->byItemCount++], pItem, sizeof(sITEM_DATA));

			res->byCurPacketCount++;

			if (res->byItemCount == NTL_MAX_BANK_ITEM_SLOT || res->byCurPacketCount == res->byTotalCount)
			{
				packet.SetPacketLen(sizeof(sQG_LOAD_PC_BANK_DATA));
				app->Send(GetSession(), &packet);

				::ZeroMemory(res->asItemData, sizeof(sITEM_DATA) * NTL_MAX_BANK_ITEM_SLOT);
				res->byItemCount = 0;
			}
		}
	}
}


void CPlayerCache::OnLoadPcData(QueryResultVector & results)
{
	//CHARACTER INFO
	QueryResult* qrCharacterInfo = results[0].result;
	if (qrCharacterInfo)
	{
		Field* f = qrCharacterInfo->Fetch();

		m_sPcData.charId = f[0].GetUInt32();
		wcscpy_s(m_sPcData.awchName, NTL_MAX_SIZE_CHAR_NAME + 1, s2ws(f[1].GetString()).c_str());
		m_sPcData.byRace = f[5].GetBYTE();
		m_sPcData.byClass = f[6].GetBYTE();
		m_sPcData.bIsAdult = f[9].GetBool();

		m_sPcData.byGender = f[7].GetBYTE();

		m_sPcData.fPositionX = f[14].GetFloat();
		m_sPcData.fPositionY = f[15].GetFloat();
		m_sPcData.fPositionZ = f[16].GetFloat();
		m_sPcData.fDirX = f[17].GetFloat();
		m_sPcData.fDirY = f[18].GetFloat();
		m_sPcData.fDirZ = f[19].GetFloat();

		m_sPcData.worldTblidx = f[21].GetUInt32();
		m_sPcData.worldId = f[20].GetUInt32();

		m_sPcData.dwEXP = f[4].GetDWORD();
		m_sPcData.byLevel = f[3].GetBYTE();

		if (m_sPcData.byLevel >= 30)
			m_sPcData.bChangeClass = true;

		m_sPcData.charLp = f[34].GetINT();
		m_sPcData.wEP = f[35].GetWORD();
		m_sPcData.wRP = f[36].GetWORD();

		m_sPcData.byCurRPBall = f[48].GetBYTE();
		m_sPcData.charAP = f[37].GetINT();

		m_sPcData.byFace = f[8].GetBYTE();
		m_sPcData.byHair = f[10].GetBYTE();
		m_sPcData.byHairColor = f[11].GetBYTE();
		m_sPcData.bySkinColor = f[12].GetBYTE();

		m_sPcData.dwMoney = f[23].GetDWORD();

		m_sPcData.guildId = f[32].GetUInt32();
		wcscpy_s(m_sPcData.awchGuildName, NTL_MAX_SIZE_GUILD_NAME + 1, s2ws(f[33].GetString()).c_str());

		m_sPcData.dwTutorialHint = f[26].GetDWORD();
		m_sPcData.dwMapInfoIndex = f[22].GetDWORD();

		m_sPcData.dwReputation = f[28].GetDWORD();
		m_sPcData.dwMudosaPoint = f[29].GetDWORD();
		m_sPcData.dwSpPoint = f[30].GetDWORD();

		m_sPcData.charTitle = f[46].GetUInt32();

		m_sPcData.sMixData.byLevel = f[44].GetBYTE();
		m_sPcData.sMixData.dwExp = f[45].GetDWORD();

		m_sPcData.dwWaguWaguPoints = f[50].GetDWORD();

		m_sPcData.byRankBattleRemainCount = 0;
		m_sPcData.byTmqLimitCount = 0;
		m_sPcData.byTmqLimitPlusCount = 0;
		m_sPcData.byDWCLimitCount = 0;
		m_sPcData.byDWCLimitPlusCount = 0;
		m_sPcData.bIsScrambleJoinFlag = false;
		m_sPcData.bIsScrambleRewardedFlag = false;

		m_sPcData.mascotTblidx = f[47].GetUInt32();
		m_sPcData.revivalAftereffectToGet = INVALID_TBLIDX;
		m_sPcData.eAirState = f[52].GetBYTE();

		m_sPcData.bInvisibleCostume = f[53].GetBool();
		m_sPcData.bInvisibleTitle = false; // TO DO
		m_sPcData.bIsGameMaster = f[31].GetBool();
		m_sPcData.byAdminLevel = m_pAccountCache->GetGmLevel();
		m_sPcData.bIsMailAway = f[38].GetBool();


		if (f[25].GetINT() == FALSE) //FALSE = CAN DO TUTORIAL : TRUE = CANT DO TUTORIAL
		{
			m_bTutorialFlag = true;
		}
		else
		{
			m_bTutorialFlag = false;
		}

	}

	//BIND INFO
	QueryResult* qrBindInfo = results[1].result;
	if (qrBindInfo)
	{
		Field* f = qrBindInfo->Fetch();

		m_sPcData.byBindType = f[9].GetBYTE();
		m_sPcData.bindWorldId = f[1].GetUInt32();
		m_sPcData.bindObjectTblidx = f[2].GetUInt32();
		m_sPcData.vBindLoc.x = f[3].GetFloat();
		m_sPcData.vBindLoc.y = f[4].GetFloat();
		m_sPcData.vBindLoc.z = f[5].GetFloat();
		m_sPcData.vBindDir.x = f[6].GetFloat();
		m_sPcData.vBindDir.y = f[7].GetFloat();
		m_sPcData.vBindDir.z = f[8].GetFloat();
	}

	//MAIL INFO
	QueryResult* qrMailInfo = results[2].result;
	if (qrMailInfo)
	{
		do
		{
			Field* f = qrMailInfo->Fetch();

			BYTE sendertype = f[1].GetBYTE();

			m_sMailBrief.byMailCount++;

			if (f[2].GetBool() == false) //check if UnRead
			{
				if (sendertype == eMAIL_SENDER_TYPE_BASIC || sendertype == eMAIL_SENDER_TYPE_REPLY || sendertype == eMAIL_SENDER_TYPE_RETURN)
					m_sMailBrief.byUnReadNormal++;

				if (sendertype == eMAIL_SENDER_TYPE_SYSTEM || sendertype == eMAIL_SENDER_TYPE_GM || sendertype == eMAIL_SENDER_TYPE_QUEST)
					m_sMailBrief.byUnReadManager++;
			}
		}
		while (qrMailInfo->NextRow());
	}

	//RANK BATTLE INFO
	QueryResult* qrRankBattle = results[3].result;
	if (qrRankBattle)
	{
		Field* f = qrRankBattle->Fetch();

		m_sRankBattleScore.dwWin = f[1].GetDWORD();
		m_sRankBattleScore.dwDraw = f[2].GetDWORD();
		m_sRankBattleScore.dwLose = f[3].GetDWORD();
		m_sRankBattleScore.wStraightKOWin = f[4].GetWORD();
		m_sRankBattleScore.wMaxStraightKOWin = f[5].GetWORD();
		m_sRankBattleScore.wMaxStraightWin = f[6].GetWORD();
		m_sRankBattleScore.wStraightWin = f[7].GetWORD();
		m_sRankBattleScore.fPoint = f[8].GetFloat();
	}

	//TITLE INFO
	QueryResult* qrTitleInfo = results[4].result;
	if (qrTitleInfo)
	{
		Field* f = qrTitleInfo->Fetch();

		do
		{
			TBLIDX titleid = f[0].GetUInt32() - 1;
			char cCurBit = (titleid % 8) * 1;

			m_abyTitleIndexFlag[titleid / 8] |= 0x01 << cCurBit;
		}
		while (qrTitleInfo->NextRow());
	}

	//WARFOG INFO
	QueryResult* qrWarFogInfo = results[5].result;
	if (qrWarFogInfo)
	{
		Field* f = qrWarFogInfo->Fetch();

		do
		{
			TBLIDX contentid = f[0].GetUInt32();
			unsigned char& uiArrayPos = m_sWarFogInfo.achWarFogFlag[contentid / 8];
			char byCurBit = (contentid % 8) * 1;
			uiArrayPos |= 0x01 << byCurBit;
		}
		while (qrWarFogInfo->NextRow());
	}


	//send pc data result
	SendPcDataLoadRes();
}

void CPlayerCache::OnLoadPcData2(QueryResultVector & results)
{
	if (QueryResult* qrItemInfo = results[0].result)
	{
		do
		{
			Field* f = qrItemInfo->Fetch();

			sITEM_DATA* data = new sITEM_DATA;

			data->itemId = f[0].GetUInt64();
			data->itemNo = f[1].GetUInt32();
			data->byPlace = f[3].GetBYTE();
			data->byPosition = f[4].GetBYTE();
			data->byStackcount = f[5].GetBYTE();
			data->byRank = f[6].GetBYTE();
			data->byCurrentDurability = f[7].GetBYTE();
			data->byGrade = f[8].GetBYTE();
			data->bNeedToIdentify = f[9].GetBool();
			data->byBattleAttribute = f[10].GetBYTE();
			wcscpy_s(data->awchMaker, NTL_MAX_SIZE_CHAR_NAME + 1, s2ws(f[11].GetString()).c_str());
			data->sOptionSet.aOptionTblidx[0] = f[12].GetUInt32();
			data->sOptionSet.aOptionTblidx[1] = f[13].GetUInt32();
			data->sOptionSet.aRandomOption[0].wOptionIndex = f[14].GetWORD();
			data->sOptionSet.aRandomOption[0].optionValue = f[15].GetINT();
			data->sOptionSet.aRandomOption[1].wOptionIndex = f[16].GetWORD();
			data->sOptionSet.aRandomOption[1].optionValue = f[17].GetINT();
			data->sOptionSet.aRandomOption[2].wOptionIndex = f[18].GetWORD();
			data->sOptionSet.aRandomOption[2].optionValue = f[19].GetINT();
			data->sOptionSet.aRandomOption[3].wOptionIndex = f[20].GetWORD();
			data->sOptionSet.aRandomOption[3].optionValue = f[21].GetINT();
			data->sOptionSet.aRandomOption[4].wOptionIndex = f[22].GetWORD();
			data->sOptionSet.aRandomOption[4].optionValue = f[23].GetINT();
			data->sOptionSet.aRandomOption[5].wOptionIndex = f[24].GetWORD();
			data->sOptionSet.aRandomOption[5].optionValue = f[25].GetINT();
			data->sOptionSet.aRandomOption[6].wOptionIndex = f[26].GetWORD();
			data->sOptionSet.aRandomOption[6].optionValue = f[27].GetINT();
			data->sOptionSet.aRandomOption[7].wOptionIndex = f[28].GetWORD();
			data->sOptionSet.aRandomOption[7].optionValue = f[29].GetINT();
			data->nUseStartTime = (DBOTIME)f[30].GetUInt64();
			data->nUseEndTime = (DBOTIME)f[31].GetUInt64();
			data->byRestrictState = f[32].GetBYTE();
			data->byDurationType = f[33].GetBYTE();

			m_mapItems.insert(std::make_pair(data->itemId, data));

		} while (qrItemInfo->NextRow());
	}


	if (QueryResult* qrSkillData = results[1].result)
	{
		do
		{
			Field* f = qrSkillData->Fetch();

			AddSkillData(f[4].GetBYTE(), f[0].GetUInt32(), f[3].GetBYTE(), f[2].GetBool());

		} while (qrSkillData->NextRow());
	}


	if (QueryResult* qrHtbSkillData = results[2].result)
	{
		do
		{
			Field* f = qrHtbSkillData->Fetch();

			AddHtbSkillData(f[0].GetUInt32(), f[3].GetDWORD());

		} while (qrHtbSkillData->NextRow());
	}


	if (QueryResult* qrBuffData = results[3].result)
	{
		do
		{
			Field* f = qrBuffData->Fetch();

			sBUFF_DATA* data = new sBUFF_DATA;

			data->sourceTblidx = f[1].GetUInt32();
			data->bySourceType = f[2].GetBYTE();
			data->buffIndex = f[3].GetBYTE();
			data->byBuffGroup = f[4].GetBYTE();
			data->dwInitialDuration = f[5].GetDWORD();
			data->dwTimeRemaining = f[6].GetDWORD();
			data->effectValue1 = f[7].GetFloat();
			data->effectValue1 = f[8].GetFloat();

			data->anArgument1[0] = f[9].GetINT(); // commonConfigTblidx
			data->anArgument1[1] = f[10].GetINT(); // left dwRemainTime
			data->anArgument1[2] = f[11].GetINT(); // left dwRemainValue
			data->anArgument2[0] = f[12].GetINT(); // default commonConfigTblidx
			data->anArgument2[1] = f[13].GetINT(); // default dwRemainTime
			data->anArgument2[2] = f[14].GetINT(); // default dwRemainValue

			m_mapBuffs.insert(std::make_pair(data->buffIndex, data));

		} while (qrBuffData->NextRow());
	}


	if (QueryResult* qrQuestItems = results[4].result)
	{
		do
		{
			Field* f = qrQuestItems->Fetch();

			AddQuestItem(f[1].GetUInt32(), f[3].GetBYTE(), f[2].GetBYTE());

		} while (qrQuestItems->NextRow());
	}


	if (QueryResult* qrQuests = results[5].result)
	{
		BYTE bySlot = 0;
		do
		{
			Field* f = qrQuests->Fetch();

			//Quest in progress
			if (f[16].GetWORD() != INVALID_WORD)
			{
				sPROGRESS_QUEST_INFO* data = new sPROGRESS_QUEST_INFO;

				data->byVer = 0;
				data->tId = f[1].GetWORD();
				data->bySlotId = bySlot++;
				data->uData.sQInfoV0.wQState = f[16].GetBYTE();
				data->uData.sQInfoV0.sMainTSP.tcCurId = f[6].GetBYTE();
				data->uData.sQInfoV0.sMainTSP.tcPreId = f[5].GetBYTE();
				//	data->uData.sQInfoV0.sSSM.auiSSM[0] = 
				data->uData.sQInfoV0.sETSlot.asExceptTimer[0].tcId = f[13].GetBYTE();
				data->uData.sQInfoV0.sETSlot.asExceptTimer[0].taId = f[14].GetBYTE();
				data->uData.sQInfoV0.sETSlot.asExceptTimer[0].uiRemainTime = f[15].GetUInt32();
				data->uData.sQInfoV0.sETSlot.asExceptTimer[1].tcId = NTL_TS_TC_ID_INVALID;
				data->uData.sQInfoV0.sETSlot.asExceptTimer[1].taId = NTL_TS_TA_ID_INVALID;
				data->uData.sQInfoV0.sETSlot.asExceptTimer[1].uiRemainTime = 0;
				data->uData.sQInfoV0.sETSlot.asExceptTimer[2].tcId = NTL_TS_TC_ID_INVALID;
				data->uData.sQInfoV0.sETSlot.asExceptTimer[2].taId = NTL_TS_TA_ID_INVALID;
				data->uData.sQInfoV0.sETSlot.asExceptTimer[2].uiRemainTime = 0;
				data->uData.sQInfoV0.sETSlot.asExceptTimer[3].tcId = NTL_TS_TC_ID_INVALID;
				data->uData.sQInfoV0.sETSlot.asExceptTimer[3].taId = NTL_TS_TA_ID_INVALID;
				data->uData.sQInfoV0.sETSlot.asExceptTimer[3].uiRemainTime = 0;
				data->uData.sQInfoV0.sSToCEvtData.m_aUserData[0] = f[9].GetUInt32();
				data->uData.sQInfoV0.sSToCEvtData.m_aUserData[1] = f[10].GetUInt32();
				data->uData.sQInfoV0.sSToCEvtData.m_aUserData[2] = f[11].GetUInt32();
				data->uData.sQInfoV0.sSToCEvtData.m_aUserData[3] = f[12].GetUInt32();
				data->uData.sQInfoV0.sSToCEvtData.tcId = f[7].GetBYTE();
				data->uData.sQInfoV0.sSToCEvtData.taId = f[8].GetBYTE();
				data->uData.sQInfoV0.tcQuestInfo = f[2].GetBYTE();
				data->uData.sQInfoV0.taQuestInfo = f[3].GetBYTE();
				data->uData.sQInfoV0.tgExcCGroup = f[4].GetBYTE();

				m_mapQuestProgressInfo.insert(std::make_pair(data->tId, data));
			}
			else
			{
				//Quest complete info
				WORD questid = f[1].GetWORD();
				int nShift = (questid % eCOMPLETE_QUEST_QUEST_PER_BYTE) * eCOMPLETE_QUEST_STATE_MAX;

				BIT_FLAG_SET(m_sQuestCompleteData.abyQCInfo[questid / eCOMPLETE_QUEST_QUEST_PER_BYTE], eCOMPLETE_QUEST_STATE_CLEAR << nShift);
			}

		} while (qrQuests->NextRow());
	}


	if (QueryResult* qrQuickSlot = results[6].result)
	{
		do
		{
			Field* f = qrQuickSlot->Fetch();

			sQUICK_SLOT_DATA* data = new sQUICK_SLOT_DATA;

			data->tblidx = f[1].GetUInt32();
			data->bySlot = f[2].GetBYTE();
			data->byType = f[3].GetBYTE();
			data->itemID = f[4].GetUInt64();
			data->hItem = INVALID_HOBJECT;

			m_mapQuickSlotInfo.insert(std::make_pair(data->bySlot, data));

		} while (qrQuickSlot->NextRow());
	}


	if (QueryResult* qrMascotData = results[7].result)
	{
		do
		{
			Field* f = qrMascotData->Fetch();

			sMASCOT_DATA_EX* data = new sMASCOT_DATA_EX;

			data->byIndex = f[1].GetBYTE();
			data->tblidx = f[2].GetUInt32();
			data->itemTblidx = f[2].GetUInt32();
			data->dwCurVP = f[3].GetDWORD();
			data->dwMaxVP = f[4].GetDWORD();
			data->dwCurExp = f[5].GetDWORD();
			data->skillTblidx[0] = f[6].GetUInt32();
			data->skillTblidx[1] = f[7].GetUInt32();
			data->skillTblidx[2] = f[8].GetUInt32();
			data->skillTblidx[3] = f[9].GetUInt32();
			data->byItemRank = 0;

			m_mapMascots.insert(std::make_pair(data->byIndex, data));

		} while (qrMascotData->NextRow());
	}


	if (QueryResult* qrRecipeData = results[8].result)
	{
		do
		{
			Field* f = qrRecipeData->Fetch();

			RegisterRecipe(f[1].GetUInt32(), f[2].GetBYTE());

		} while (qrRecipeData->NextRow());
	}


	if (QueryResult* qrItemCoolDown = results[9].result)
	{
		do
		{
			Field* f = qrItemCoolDown->Fetch();

			sDBO_ITEM_COOL_TIME data;

			data.byItemCoolTimeGroupIndex = f[1].GetBYTE();
			data.dwInitialItemCoolTime = f[2].GetDWORD();
			data.dwItemCoolTimeRemaining = f[3].GetDWORD();

			m_mapItemCoolTime.insert(std::make_pair(data.byItemCoolTimeGroupIndex, data));

		} while (qrItemCoolDown->NextRow());
	}


	if (QueryResult* qrPortal = results[10].result)
	{
		do
		{
			Field* f = qrPortal->Fetch();

			m_setPortalPoints.insert(f[0].GetBYTE());

		} while (qrPortal->NextRow());
	}


	SendSkillLoadRes();
	SendBuffLoadRes();
	SendHtbSkillLoadRes();
	SendItemLoadRes();
	SendQuestItemLoadRes();
	SendQuestCompleteLoadRes();
	SendQuestProgressLoadRes();
	SendQuickSlotLoadRes();
	SendItemRecipeLoadRes();
	SendMascotLoadRes();
	SendPortalLoadRes();
	SendItemCoolTimeLoadRes(); //must be last

	m_pAccountCache->SendShortcutLoadRes(m_sPcData.charId);
}

void CPlayerCache::OnLoadBank(QueryResultVector & results, CAccountCache * pAccountCache, bool bLoadSharedBankFromCache, HOBJECT hObject, HOBJECT hNpcHandle)
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	if (QueryResult* result = results[0].result)
	{
		BYTE i = 0;
		int cpc = 0;
		UINT32 totalcount = result->GetRowCount();

		CNtlPacket packet(sizeof(sQG_LOAD_PC_BANK_DATA));
		sQG_LOAD_PC_BANK_DATA * res = (sQG_LOAD_PC_BANK_DATA *)packet.GetPacketData();
		res->wOpCode = QG_LOAD_PC_BANK_DATA;
		res->charID = GetCharID();

		do
		{
			Field* f = result->Fetch();

			res->asItemData[i].itemId = f[0].GetUInt64();
			res->asItemData[i].itemNo = f[1].GetUInt32();
			res->asItemData[i].charId = res->charID;
			res->asItemData[i].byPlace = f[3].GetBYTE();
			res->asItemData[i].byPosition = f[4].GetBYTE();
			res->asItemData[i].byStackcount = f[5].GetBYTE();
			res->asItemData[i].byRank = f[6].GetBYTE();
			res->asItemData[i].byCurrentDurability = f[7].GetBYTE();
			res->asItemData[i].byGrade = f[8].GetBYTE();
			res->asItemData[i].bNeedToIdentify = f[9].GetBool();
			res->asItemData[i].byBattleAttribute = f[10].GetBYTE();
			wcscpy_s(res->asItemData[i].awchMaker, NTL_MAX_SIZE_CHAR_NAME + 1, s2ws(f[11].GetString()).c_str());
			res->asItemData[i].sOptionSet.aOptionTblidx[0] = f[12].GetUInt32();
			res->asItemData[i].sOptionSet.aOptionTblidx[1] = f[13].GetUInt32();
			res->asItemData[i].sOptionSet.aRandomOption[0].wOptionIndex = f[14].GetWORD();
			res->asItemData[i].sOptionSet.aRandomOption[0].optionValue = f[15].GetINT();
			res->asItemData[i].sOptionSet.aRandomOption[1].wOptionIndex = f[16].GetWORD();
			res->asItemData[i].sOptionSet.aRandomOption[1].optionValue = f[17].GetINT();
			res->asItemData[i].sOptionSet.aRandomOption[2].wOptionIndex = f[18].GetWORD();
			res->asItemData[i].sOptionSet.aRandomOption[2].optionValue = f[19].GetINT();
			res->asItemData[i].sOptionSet.aRandomOption[3].wOptionIndex = f[20].GetWORD();
			res->asItemData[i].sOptionSet.aRandomOption[3].optionValue = f[21].GetINT();
			res->asItemData[i].sOptionSet.aRandomOption[4].wOptionIndex = f[22].GetWORD();
			res->asItemData[i].sOptionSet.aRandomOption[4].optionValue = f[23].GetINT();
			res->asItemData[i].sOptionSet.aRandomOption[5].wOptionIndex = f[24].GetWORD();
			res->asItemData[i].sOptionSet.aRandomOption[5].optionValue = f[25].GetINT();
			res->asItemData[i].sOptionSet.aRandomOption[6].wOptionIndex = f[26].GetWORD();
			res->asItemData[i].sOptionSet.aRandomOption[6].optionValue = f[27].GetINT();
			res->asItemData[i].sOptionSet.aRandomOption[7].wOptionIndex = f[28].GetWORD();
			res->asItemData[i].sOptionSet.aRandomOption[7].optionValue = f[29].GetINT();
			res->asItemData[i].nUseStartTime = (DBOTIME)f[30].GetDWORD();
			res->asItemData[i].nUseEndTime = (DBOTIME)f[31].GetDWORD();
			res->asItemData[i].byRestrictState = f[32].GetBYTE();
			res->asItemData[i].byDurationType = f[33].GetBYTE();

			AddBankItem(res->asItemData[i]);

			i++; cpc++;

			res->byTotalCount = (BYTE)totalcount;
			res->byItemCount = i;
			res->byCurPacketCount = (BYTE)cpc - i;

			if ((int)i == NTL_MAX_BANK_ITEM_SLOT || cpc == (int)totalcount)
			{
				packet.SetPacketLen(sizeof(sQG_LOAD_PC_BANK_DATA));
				app->Send(GetSession(), &packet);

				::ZeroMemory(res->asItemData, sizeof(sITEM_DATA) * NTL_MAX_BANK_ITEM_SLOT);
				i = 0;
			}
		} while (result->NextRow());
	}

	if (bLoadSharedBankFromCache == false)
	{
		if (QueryResult* result = results[1].result)
		{
			BYTE i = 0;
			int cpc = 0;
			UINT32 totalcount = result->GetRowCount();

			CNtlPacket packet(sizeof(sQG_LOAD_PC_BANK_DATA));
			sQG_LOAD_PC_BANK_DATA * res = (sQG_LOAD_PC_BANK_DATA *)packet.GetPacketData();
			res->wOpCode = QG_LOAD_PC_BANK_DATA;
			res->charID = GetCharID();

			do
			{
				Field* f = result->Fetch();

				res->asItemData[i].itemId = f[0].GetUInt64();
				res->asItemData[i].itemNo = f[1].GetUInt32();
				res->asItemData[i].charId = res->charID;
				res->asItemData[i].byPlace = f[3].GetBYTE();
				res->asItemData[i].byPosition = f[4].GetBYTE();
				res->asItemData[i].byStackcount = f[5].GetBYTE();
				res->asItemData[i].byRank = f[6].GetBYTE();
				res->asItemData[i].byCurrentDurability = f[7].GetBYTE();
				res->asItemData[i].byGrade = f[8].GetBYTE();
				res->asItemData[i].bNeedToIdentify = f[9].GetBool();
				res->asItemData[i].byBattleAttribute = f[10].GetBYTE();
				wcscpy_s(res->asItemData[i].awchMaker, NTL_MAX_SIZE_CHAR_NAME + 1, s2ws(f[11].GetString()).c_str());
				res->asItemData[i].sOptionSet.aOptionTblidx[0] = f[12].GetUInt32();
				res->asItemData[i].sOptionSet.aOptionTblidx[1] = f[13].GetUInt32();
				res->asItemData[i].sOptionSet.aRandomOption[0].wOptionIndex = f[14].GetWORD();
				res->asItemData[i].sOptionSet.aRandomOption[0].optionValue = f[15].GetINT();
				res->asItemData[i].sOptionSet.aRandomOption[1].wOptionIndex = f[16].GetWORD();
				res->asItemData[i].sOptionSet.aRandomOption[1].optionValue = f[17].GetINT();
				res->asItemData[i].sOptionSet.aRandomOption[2].wOptionIndex = f[18].GetWORD();
				res->asItemData[i].sOptionSet.aRandomOption[2].optionValue = f[19].GetINT();
				res->asItemData[i].sOptionSet.aRandomOption[3].wOptionIndex = f[20].GetWORD();
				res->asItemData[i].sOptionSet.aRandomOption[3].optionValue = f[21].GetINT();
				res->asItemData[i].sOptionSet.aRandomOption[4].wOptionIndex = f[22].GetWORD();
				res->asItemData[i].sOptionSet.aRandomOption[4].optionValue = f[23].GetINT();
				res->asItemData[i].sOptionSet.aRandomOption[5].wOptionIndex = f[24].GetWORD();
				res->asItemData[i].sOptionSet.aRandomOption[5].optionValue = f[25].GetINT();
				res->asItemData[i].sOptionSet.aRandomOption[6].wOptionIndex = f[26].GetWORD();
				res->asItemData[i].sOptionSet.aRandomOption[6].optionValue = f[27].GetINT();
				res->asItemData[i].sOptionSet.aRandomOption[7].wOptionIndex = f[28].GetWORD();
				res->asItemData[i].sOptionSet.aRandomOption[7].optionValue = f[29].GetINT();
				res->asItemData[i].nUseStartTime = (DBOTIME)f[30].GetDWORD();
				res->asItemData[i].nUseEndTime = (DBOTIME)f[31].GetDWORD();
				res->asItemData[i].byRestrictState = f[32].GetBYTE();
				res->asItemData[i].byDurationType = f[33].GetBYTE();

				pAccountCache->AddBankItem(res->asItemData[i]);

				i++; cpc++;

				res->byTotalCount = (BYTE)totalcount;
				res->byItemCount = i;
				res->byCurPacketCount = (BYTE)cpc - i;

				if ((int)i == NTL_MAX_BANK_ITEM_SLOT || cpc == (int)totalcount)
				{
					packet.SetPacketLen(sizeof(sQG_LOAD_PC_BANK_DATA));
					app->Send(GetSession(), &packet);

					::ZeroMemory(res->asItemData, sizeof(sITEM_DATA) * NTL_MAX_BANK_ITEM_SLOT);
					i = 0;
				}
			} while (result->NextRow());
		}
	}
	else
	{
		pAccountCache->SendBankItemLoad(GetCharID());
	}

	CNtlPacket packet2(sizeof(sQG_LOAD_PC_BANK_DATA_RES));
	sQG_LOAD_PC_BANK_DATA_RES * res2 = (sQG_LOAD_PC_BANK_DATA_RES *)packet2.GetPacketData();
	res2->wOpCode = QG_LOAD_PC_BANK_DATA_RES;
	res2->charID = GetCharID();
	res2->dwZenny = 0;
	res2->npchandle = hNpcHandle;
	res2->wResultCode = 500;
	res2->handle = hObject;
	packet2.SetPacketLen(sizeof(sQG_LOAD_PC_BANK_DATA_RES));
	app->Send(GetSession(), &packet2);

	SetBankLoaded(true);
	pAccountCache->SetBankLoaded(true);
}

void CPlayerCache::SetGuild(GUILDID guildId, WCHAR * wszGuildName)
{
	m_sPcData.guildId = guildId;

	if (guildId == 0)
	{
		::ZeroMemory(m_sPcData.awchGuildName, NTL_MAX_SIZE_GUILD_NAME + 1);
	}
	else
	{
		NTL_SAFE_WCSCPY(m_sPcData.awchGuildName, wszGuildName);
	}
}

void CPlayerCache::SetSkillPoints(DWORD dwPoints)
{
	m_sPcData.dwSpPoint = dwPoints;
}

void CPlayerCache::SetZeni(DWORD dwZeni)
{
	m_sPcData.dwMoney = dwZeni;
}

void CPlayerCache::SetMudusaPoints(DWORD dwPoints)
{
	m_sPcData.dwMudosaPoint = dwPoints;
}


void CPlayerCache::StoreRunTimeData(DWORD dwExp, TBLIDX worldIdx, WORLDID worldId, sVECTOR3 & rLoc, sVECTOR3 & rDir)
{
	m_sPcData.dwEXP = dwExp;
	m_sPcData.worldTblidx = worldIdx;
	m_sPcData.worldId = worldId;
	m_sPcData.fPositionX = rLoc.x;
	m_sPcData.fPositionY = rLoc.y;
	m_sPcData.fPositionZ = rLoc.z;
	m_sPcData.fDirX = rDir.x;
	m_sPcData.fDirY = rDir.y;
	m_sPcData.fDirZ = rDir.z;
}

void CPlayerCache::SavePcData(const sPC_DATA & rPcData)
{
	m_sPcData.dwEXP = rPcData.dwEXP;
	m_sPcData.worldTblidx = rPcData.worldTblidx;
	m_sPcData.worldId = rPcData.worldId;
	m_sPcData.fPositionX = rPcData.fPositionX;
	m_sPcData.fPositionY = rPcData.fPositionY;
	m_sPcData.fPositionZ = rPcData.fPositionZ;
	m_sPcData.fDirX = rPcData.fDirX;
	m_sPcData.fDirY = rPcData.fDirY;
	m_sPcData.fDirZ = rPcData.fDirZ;

	m_sPcData.charLp = rPcData.charLp;
	m_sPcData.wEP = rPcData.wEP;
	m_sPcData.wRP = rPcData.wRP;
	m_sPcData.byCurRPBall = rPcData.byCurRPBall;
	m_sPcData.charAP = rPcData.charAP;
	m_sPcData.dwMapInfoIndex = rPcData.dwMapInfoIndex;
	m_sPcData.eAirState = rPcData.eAirState;
	m_sPcData.mascotTblidx = rPcData.mascotTblidx;
	m_sPcData.charTitle = rPcData.charTitle;
	m_sPcData.dwTutorialHint = rPcData.dwTutorialHint;
	m_sPcData.bIsScrambleJoinFlag = rPcData.bIsScrambleJoinFlag;
	m_sPcData.bIsScrambleRewardedFlag = rPcData.bIsScrambleRewardedFlag;
}

void CPlayerCache::SaveServerChangeInfo(const sDBO_SERVER_CHANGE_INFO & rInfo)
{
	memcpy(&m_serverChangeInfo, &rInfo, sizeof(sDBO_SERVER_CHANGE_INFO));
}

bool CPlayerCache::IsChannelChanged()
{
	if (m_serverChangeInfo.prevServerChannelId != INVALID_SERVERCHANNELID)
	{
		m_serverChangeInfo.Init();
		return true;
	}

	return false;
}

void CPlayerCache::UpdateWarFog(BYTE * pbyWarFogFlag)
{
	memcpy(m_sWarFogInfo.achWarFogFlag, pbyWarFogFlag, sizeof(m_sWarFogInfo.achWarFogFlag));
}

void CPlayerCache::UpdateSelectedTitle(TBLIDX titleIdx)
{
	m_sPcData.charTitle = titleIdx;
}

void CPlayerCache::UpdateTitle(BYTE * pbyTitleIndexFlag)
{
	memcpy(m_abyTitleIndexFlag, pbyTitleIndexFlag, sizeof(m_abyTitleIndexFlag));
}

void CPlayerCache::RegisterRecipe(TBLIDX recipeIdx, BYTE byRecipeType)
{
	m_mapRecipes.insert(std::make_pair(recipeIdx, byRecipeType));
}

void CPlayerCache::RegisterMascot(BYTE byIndex, TBLIDX mascotTblidx, BYTE byRank, DWORD dwVP)
{
	sMASCOT_DATA_EX* pData = new sMASCOT_DATA_EX;

	pData->tblidx = mascotTblidx;
	pData->itemTblidx = mascotTblidx;
	pData->byIndex = byIndex;
	pData->dwCurVP = dwVP;
	pData->dwMaxVP = dwVP;
	pData->dwCurExp = 0;
	pData->byItemRank = byRank;
	pData->skillTblidx[0] = INVALID_TBLIDX;
	pData->skillTblidx[1] = INVALID_TBLIDX;
	pData->skillTblidx[2] = INVALID_TBLIDX;
	pData->skillTblidx[3] = INVALID_TBLIDX;

	m_mapMascots.insert(std::make_pair(byIndex, pData));
}

void CPlayerCache::DeleteMascot(BYTE byIndex)
{
	std::map<BYTE, sMASCOT_DATA_EX*>::iterator it = m_mapMascots.find(byIndex);
	if (it != m_mapMascots.end())
	{
		delete it->second;
		m_mapMascots.erase(it);
	}
}

void CPlayerCache::MascotSkillAdd(BYTE byMascotIndex, BYTE bySlot, TBLIDX skillTblidx)
{
	std::map<BYTE, sMASCOT_DATA_EX*>::iterator it = m_mapMascots.find(byMascotIndex);
	if (it != m_mapMascots.end())
	{
		sMASCOT_DATA_EX* pData = it->second;

		pData->skillTblidx[bySlot] = skillTblidx;
	}
}

sMASCOT_DATA_EX * CPlayerCache::GetMascotData(BYTE byIndex)
{
	std::map<BYTE, sMASCOT_DATA_EX*>::iterator it = m_mapMascots.find(byIndex);
	if (it != m_mapMascots.end())
	{
		return it->second;
	}

	return nullptr;
}

void CPlayerCache::EraseMascot(BYTE byIndex)
{
	m_mapMascots.erase(byIndex);
}

void CPlayerCache::AddPortal(PORTALID portalID)
{
	m_setPortalPoints.insert(portalID);
}

void CPlayerCache::UpdateBind(WORLDID worldId, TBLIDX objectTblidx, BYTE byType, sVECTOR3 & vLoc, sVECTOR3 & vDir)
{
	m_sPcData.bindWorldId = worldId;
	m_sPcData.bindObjectTblidx = objectTblidx;
	m_sPcData.byBindType = byType;
	m_sPcData.vBindLoc.x = vLoc.x;
	m_sPcData.vBindLoc.y = vLoc.y;
	m_sPcData.vBindLoc.z = vLoc.z;
	m_sPcData.vBindDir.x = vDir.x;
	m_sPcData.vBindDir.y = vDir.y;
	m_sPcData.vBindDir.z = vDir.z;
}

void CPlayerCache::SetChildAdult(bool bIsAdult)
{
	m_sPcData.bIsAdult = bIsAdult;
}

void CPlayerCache::SetClass(BYTE byClass)
{
	m_sPcData.byClass = byClass;
}

void CPlayerCache::SetGender(BYTE byGender)
{
	m_sPcData.byGender = byGender;
}

void CPlayerCache::SetLevel(BYTE byLevel)
{
	m_sPcData.byLevel = byLevel;
}

void CPlayerCache::SetExp(DWORD dwExp)
{
	m_sPcData.dwEXP = dwExp;
}

void CPlayerCache::SetInvisibleCostume(bool bFlag)
{
	m_sPcData.bInvisibleCostume = bFlag;
}

void CPlayerCache::SetCharName(WCHAR * wchName)
{
	NTL_SAFE_WCSCPY(m_sPcData.awchName, wchName);
}

void CPlayerCache::SetHoipoiMix(BYTE byLevel, DWORD dwExp)
{
	m_sPcData.sMixData.byLevel = byLevel;
	m_sPcData.sMixData.dwExp = dwExp;
}


void CPlayerCache::UpdateQuickSlot(BYTE bySlot, BYTE byType, TBLIDX tblidx, ITEMID itemid)
{
	std::map<BYTE, sQUICK_SLOT_DATA*>::iterator it = m_mapQuickSlotInfo.find(bySlot);
	if (it != m_mapQuickSlotInfo.end())
	{
		sQUICK_SLOT_DATA* pData = it->second;

		pData->byType = byType;
		//pData->hItem
		pData->itemID = itemid;
		pData->tblidx = tblidx;
	}
	else
	{
		sQUICK_SLOT_DATA* data = new sQUICK_SLOT_DATA;

		data->tblidx = tblidx;
		data->bySlot = bySlot;
		data->byType = byType;
		data->itemID = itemid;
		data->hItem = INVALID_HOBJECT;

		m_mapQuickSlotInfo.insert(std::make_pair(data->bySlot, data));
	}
}

void CPlayerCache::DeleteQuickSlot(BYTE bySlot)
{
	std::map<BYTE, sQUICK_SLOT_DATA*>::iterator it = m_mapQuickSlotInfo.find(bySlot);
	if (it != m_mapQuickSlotInfo.end())
	{
		sQUICK_SLOT_DATA* pData = it->second;
		delete pData;

		m_mapQuickSlotInfo.erase(it);
	}
}

void CPlayerCache::UpdateItemCoolTime(sDBO_ITEM_COOL_TIME& rItemCoolTime)
{
	m_mapItemCoolTime.insert(std::make_pair(rItemCoolTime.byItemCoolTimeGroupIndex, rItemCoolTime));
}

bool CPlayerCache::UpdateSkillTime(sSKILL_DATA& rData)
{
	std::map<BYTE, sSKILL_DATA*>::iterator it = m_mapSkills.find(rData.skillIndex);
	if (it != m_mapSkills.end())
	{
		sSKILL_DATA* pData = it->second;

		if (pData->nRemainSec != rData.nRemainSec)
		{
			pData->nRemainSec = rData.nRemainSec;
			return true;
		}
	}

	return false;
}


void CPlayerCache::UpdateSkillData(sSKILL_DATA & rData)
{
	std::map<BYTE, sSKILL_DATA*>::iterator it = m_mapSkills.find(rData.skillIndex);
	if (it != m_mapSkills.end())
	{
		sSKILL_DATA* pData = it->second;

		pData->bIsRpBonusAuto = rData.bIsRpBonusAuto;
		pData->byRpBonusType = rData.byRpBonusType;
		pData->nRemainSec = rData.nRemainSec;
		pData->skillId = rData.skillId;
	}
}

void CPlayerCache::UpdateSkillData(BYTE bySkillIndex, TBLIDX skillIdx)
{
	std::map<BYTE, sSKILL_DATA*>::iterator it = m_mapSkills.find(bySkillIndex);
	if (it != m_mapSkills.end())
	{
		sSKILL_DATA* pData = it->second;

		pData->skillId = skillIdx;
	}
}

void CPlayerCache::AddSkillData(BYTE bySkillIndex, TBLIDX skillIdx, BYTE byRpBonusType, bool bRpAuto)
{
	sSKILL_DATA* pData = new sSKILL_DATA;

	pData->skillIndex = bySkillIndex;
	pData->bIsRpBonusAuto = bRpAuto;
	pData->byRpBonusType = byRpBonusType;
	pData->nRemainSec = 0;
	pData->skillId = skillIdx;

	m_mapSkills.insert(std::make_pair(bySkillIndex, pData));
}

void CPlayerCache::InitSkills()
{
	std::map<BYTE, sSKILL_DATA*>::iterator it = m_mapSkills.begin();
	while (it != m_mapSkills.end())
	{
		sSKILL_DATA* pSkill = it->second;

		delete pSkill;
		it = m_mapSkills.erase(it);
	}
}

bool CPlayerCache::DeleteSkill(BYTE byIndex)
{
	std::map<BYTE, sSKILL_DATA*>::iterator it = m_mapSkills.find(byIndex);
	if (it != m_mapSkills.end())
	{
		sSKILL_DATA* pData = it->second;

		delete pData;
		m_mapSkills.erase(it);

		return true;
	}

	return false;
}

sSKILL_DATA * CPlayerCache::GetSkillData(BYTE byIndex)
{
	std::map<BYTE, sSKILL_DATA*>::iterator it = m_mapSkills.find(byIndex);
	if (it != m_mapSkills.end())
	{
		return it->second;
	}

	return NULL;
}

bool CPlayerCache::UpdateHtbSkillTime(sHTB_SKILL_DATA & rData)
{
	std::map<TBLIDX, sHTB_SKILL_DATA*>::iterator it = m_mapHtbSkills.find(rData.skillId);
	if (it != m_mapHtbSkills.end())
	{
		sHTB_SKILL_DATA* pData = it->second;

		if (pData->dwSkillTime != rData.dwSkillTime)
		{
			pData->dwSkillTime = rData.dwSkillTime;
			return true;
		}
	}

	return false;
}

void CPlayerCache::AddHtbSkillData(TBLIDX skillIdx)
{
	sHTB_SKILL_DATA* pData = new sHTB_SKILL_DATA;

	pData->dwSkillTime = 0;
	pData->skillId = skillIdx;

	m_mapHtbSkills.insert(std::make_pair(skillIdx, pData));
}

void CPlayerCache::AddHtbSkillData(TBLIDX skillIdx, DWORD dwSkillTime)
{
	sHTB_SKILL_DATA* pData = new sHTB_SKILL_DATA;

	pData->dwSkillTime = dwSkillTime;
	pData->skillId = skillIdx;

	m_mapHtbSkills.insert(std::make_pair(skillIdx, pData));
}

bool CPlayerCache::UpdateBuffTime(sBUFF_DATA & rData)
{
	std::map<BYTE, sBUFF_DATA*>::iterator it = m_mapBuffs.find(rData.buffIndex);
	if (it != m_mapBuffs.end())
	{
		sBUFF_DATA* pData = it->second;

		if (rData.dwTimeRemaining > 5000) //only update if bigger than 5 seconds
		{
			pData->dwTimeRemaining = rData.dwTimeRemaining;
			pData->anArgument1[1] = rData.anArgument1[1];
			pData->anArgument1[2] = rData.anArgument1[2];
			return true;
		}
		else
		{
			delete pData;
			m_mapBuffs.erase(it);
		}
	}

	return false;
}

bool CPlayerCache::AddBuffData(sBUFF_DATA & rData)
{
	std::map<BYTE, sBUFF_DATA*>::iterator it = m_mapBuffs.find(rData.buffIndex);
	if (it == m_mapBuffs.end())
	{
		sBUFF_DATA* pBuffData = new sBUFF_DATA;
		memcpy(pBuffData, &rData, sizeof(sBUFF_DATA));

		m_mapBuffs.insert(std::make_pair(pBuffData->buffIndex, pBuffData));

		return true;
	}
	else ERR_LOG(LOG_USER, "Buffindex %u already registered. Cannot add buff.", rData.buffIndex);

	return false;
}


bool CPlayerCache::DelBuffData(BYTE byBuffIndex)
{
	std::map<BYTE, sBUFF_DATA*>::iterator it = m_mapBuffs.find(byBuffIndex);
	if (it != m_mapBuffs.end())
	{
		sBUFF_DATA* pData = it->second;

		delete pData;
		m_mapBuffs.erase(it);

		return true;
	}

	return false;
}

void CPlayerCache::UpdateIdentifyItem(ITEMID itemId)
{
	std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapItems.find(itemId);
	if (it != m_mapItems.end())
	{
		sITEM_DATA* pData = it->second;

		pData->bNeedToIdentify = false;
	}
}

void CPlayerCache::UpdateItemDur(ITEMID itemId, BYTE byDur)
{
	std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapItems.find(itemId);
	if(it != m_mapItems.end())
	{
		sITEM_DATA* pData = it->second;

		pData->byCurrentDurability = byDur;
	}
}

bool CPlayerCache::UpdateEquipItemDur(BYTE byPos, BYTE byDur)
{
	for (std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapItems.begin(); it != m_mapItems.end(); it++)
	{
		sITEM_DATA* pData = it->second;

		if (pData->byPlace == CONTAINER_TYPE_EQUIP && pData->byPosition == byPos)
		{
			pData->byCurrentDurability = byDur;
			return true;
		}
	}

	return false;
}

void CPlayerCache::AddQuestItem(TBLIDX tblidx, BYTE byPos, BYTE byStack)
{
	sQUESTITEM_DATA* pData = new sQUESTITEM_DATA;

	pData->byCurStackCount = byStack;
	pData->byPos = byPos;
	pData->tblidx = tblidx;

	m_mapQuestItems.insert(std::make_pair(byPos, pData));
}

bool CPlayerCache::UpdateQuestItem(BYTE byPos, BYTE byCount)
{
	std::map<BYTE, sQUESTITEM_DATA*>::iterator it = m_mapQuestItems.find(byPos);
	if (it != m_mapQuestItems.end())
	{
		sQUESTITEM_DATA* pData = it->second;

		pData->byCurStackCount = byCount;
	}

	return false;
}

bool CPlayerCache::CreateQuestItem(TBLIDX tblidx, BYTE byPos, BYTE byCount)
{
	std::map<BYTE, sQUESTITEM_DATA*>::iterator it = m_mapQuestItems.find(byPos);
	if (it == m_mapQuestItems.end())
	{
		sQUESTITEM_DATA* pData = new sQUESTITEM_DATA;

		pData->byCurStackCount = byCount;
		pData->tblidx = tblidx;
		pData->byPos = byPos;

		m_mapQuestItems.insert(std::make_pair(byPos, pData));
		return true;
	}

	return false;
}

bool CPlayerCache::DeleteQuestItem(BYTE byPos)
{
	std::map<BYTE, sQUESTITEM_DATA*>::iterator it = m_mapQuestItems.find(byPos);
	if (it != m_mapQuestItems.end())
	{
		sQUESTITEM_DATA* pData = it->second;

		delete pData;
		m_mapQuestItems.erase(it);
		return true;
	}

	return false;
}

bool CPlayerCache::MoveQuestItem(BYTE byPos, BYTE byDestPos)
{
	std::map<BYTE, sQUESTITEM_DATA*>::iterator it = m_mapQuestItems.find(byPos);
	if (it != m_mapQuestItems.end())
	{
		sQUESTITEM_DATA* pData = it->second;

		std::map<BYTE, sQUESTITEM_DATA*>::iterator it2 = m_mapQuestItems.find(byDestPos); //check if dest pos is empty
		if (it2 != m_mapQuestItems.end())
			return false;

		m_mapQuestItems.erase(it);

		pData->byPos = byDestPos;
		m_mapQuestItems.insert(std::make_pair(byDestPos, pData));

		return true;
	}

	return false;
}

bool CPlayerCache::SwitchQuestItem(BYTE byPos, BYTE byDestPos)
{
	std::map<BYTE, sQUESTITEM_DATA*>::iterator itSrc = m_mapQuestItems.find(byPos);
	if (itSrc == m_mapQuestItems.end())
		return false;

	std::map<BYTE, sQUESTITEM_DATA*>::iterator itDst = m_mapQuestItems.find(byDestPos);
	if (itDst == m_mapQuestItems.end())
		return false;

	sQUESTITEM_DATA* pSrcData = itSrc->second;
	sQUESTITEM_DATA* pDestData = itDst->second;

	TBLIDX srcTblidx = pSrcData->tblidx;
	BYTE bySrcCount = pSrcData->byCurStackCount;

	//set src data
	pSrcData->byCurStackCount = pDestData->byCurStackCount;
	pSrcData->byPos = pDestData->byPos;
	pSrcData->tblidx = pDestData->tblidx;

	//set dest item to src pos
	pDestData->byPos = byPos;
	pDestData->byCurStackCount = bySrcCount;
	pDestData->tblidx = srcTblidx;

	return true;
}

bool CPlayerCache::DeleteQuestProgressData(QUESTID qId)
{
	std::map<QUESTID, sPROGRESS_QUEST_INFO*>::iterator it = m_mapQuestProgressInfo.find(qId);
	if (it != m_mapQuestProgressInfo.end())
	{
		sPROGRESS_QUEST_INFO* pData = it->second;

		delete pData;
		m_mapQuestProgressInfo.erase(it);
		return true;
	}

	return false;
}

bool CPlayerCache::CompleteQuest(QUESTID qId)
{
	/*
		//it might happen, that we dont have quest in progress map because quest data is not sent to query server after accepting.
	*/

	int nShift = (qId % eCOMPLETE_QUEST_QUEST_PER_BYTE) * eCOMPLETE_QUEST_STATE_MAX;

	if (BIT_FLAG_TEST(m_sQuestCompleteData.abyQCInfo[qId / eCOMPLETE_QUEST_QUEST_PER_BYTE], (eCOMPLETE_QUEST_STATE_CLEAR << nShift)) == false) //check if not set
	{
		m_sQuestCompleteData.abyQCInfo[qId / eCOMPLETE_QUEST_QUEST_PER_BYTE] |= (eCOMPLETE_QUEST_STATE_CLEAR << nShift); //set
	}

	std::map<QUESTID, sPROGRESS_QUEST_INFO*>::iterator it = m_mapQuestProgressInfo.find(qId);
	if (it != m_mapQuestProgressInfo.end())
	{
		sPROGRESS_QUEST_INFO* pData = it->second;

		delete pData;
		m_mapQuestProgressInfo.erase(it);

		return true;
	}

	return true;
}

void CPlayerCache::StoreQuestProgress(sPROGRESS_QUEST_INFO & rQuest)
{
	std::map<QUESTID, sPROGRESS_QUEST_INFO*>::iterator it = m_mapQuestProgressInfo.find(rQuest.tId);
	if (it != m_mapQuestProgressInfo.end())
	{
		sPROGRESS_QUEST_INFO* pData = it->second;

		memcpy(pData, &rQuest, sizeof(sPROGRESS_QUEST_INFO));
	}
	else
	{
		sPROGRESS_QUEST_INFO* pData = new sPROGRESS_QUEST_INFO;

		memcpy(pData, &rQuest, sizeof(sPROGRESS_QUEST_INFO));

		m_mapQuestProgressInfo.insert(std::make_pair(rQuest.tId, pData));
	}
}

void CPlayerCache::AddQuickTeleport(BYTE bySlot, sQUICK_TELEPORT_INFO & rInfo)
{
	sQUICK_TELEPORT_INFO* pInfo = new sQUICK_TELEPORT_INFO;
	memcpy(pInfo, &rInfo, sizeof(sQUICK_TELEPORT_INFO));

	m_mapQuickTeleport.insert(std::make_pair(bySlot, pInfo));
}

void CPlayerCache::UpdateQuickTeleport(BYTE bySlot, sQUICK_TELEPORT_INFO & rInfo)
{
	std::map<BYTE, sQUICK_TELEPORT_INFO*>::iterator it = m_mapQuickTeleport.find(bySlot);
	if (it != m_mapQuickTeleport.end())
	{
		sQUICK_TELEPORT_INFO* pData = it->second;

		memcpy(pData, &rInfo, sizeof(sQUICK_TELEPORT_INFO));
	}
	else
	{
		AddQuickTeleport(bySlot, rInfo);
	}
}

bool CPlayerCache::DeleteQuickTeleport(BYTE bySlot)
{
	std::map<BYTE, sQUICK_TELEPORT_INFO*>::iterator it = m_mapQuickTeleport.find(bySlot);
	if (it != m_mapQuickTeleport.end())
	{
		sQUICK_TELEPORT_INFO* pData = it->second;

		delete pData;
		m_mapQuickTeleport.erase(it);

		return true;
	}

	return false;
}

void CPlayerCache::FillQuickTeleportData(sQUICK_TELEPORT_INFO * pInfo, BYTE & rbyCount)
{
	for (std::map<BYTE, sQUICK_TELEPORT_INFO*>::iterator it = m_mapQuickTeleport.begin(); it != m_mapQuickTeleport.end(); it++)
	{
		memcpy(&pInfo[rbyCount++], it->second, sizeof(sQUICK_TELEPORT_INFO));

		if (rbyCount == NTL_QUICK_PORTAL_MAX_COUNT)
			break;
	}
}

bool CPlayerCache::RemoveItem(ITEMID itemId)
{
	std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapItems.find(itemId);
	if (it != m_mapItems.end())
	{
		sITEM_DATA* pData = it->second;

		delete pData;
		m_mapItems.erase(it);

		return true;
	}

	return false;
}

bool CPlayerCache::SwitchItem(ITEMID itemId, CPlayerCache * pNewOwner, BYTE byNewPlace, BYTE byNewPos)
{
	std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapItems.find(itemId);
	if (it != m_mapItems.end())
	{
		sITEM_DATA* pData = it->second;

		m_mapItems.erase(it);

		pData->byPlace = byNewPlace;
		pData->byPosition = byNewPos;
		pNewOwner->InsertItem(pData);

		return true;
	}

	return false;
}

void CPlayerCache::InsertItem(sITEM_DATA * pItemData)
{
	m_mapItems.insert(std::make_pair(pItemData->itemId, pItemData));
}

void CPlayerCache::EraseItem(ITEMID itemId)
{
	m_mapItems.erase(itemId);
}

void CPlayerCache::AddItem(sITEM_DATA & rItemData)
{
	sITEM_DATA* pData = new sITEM_DATA;
	memcpy(pData, &rItemData, sizeof(sITEM_DATA));

	m_mapItems.insert(std::make_pair(pData->itemId, pData));
}

void CPlayerCache::AddItem(sSHOP_BUY_INVEN & rData, ITEMID itemId)
{
	sITEM_DATA* pData = new sITEM_DATA;

	NTL_SAFE_WCSCPY(pData->awchMaker, rData.awchMaker);
	pData->bNeedToIdentify = false;
	pData->byBattleAttribute = 0;
	pData->byCurrentDurability = rData.byCurrentDurability;
	pData->byDurationType = rData.byDurationType;
	pData->byGrade = rData.byGrade;
	pData->byPlace = rData.byPlace;
	pData->byPosition = rData.byPos;
	pData->byRank = rData.byRank;
	pData->byRestrictState = rData.byRestrictState;
	pData->byStackcount = rData.byStack;
	pData->byCurrentDurability = rData.byCurrentDurability;
	pData->charId = m_sPcData.charId;
	pData->itemId = itemId;
	pData->itemNo = rData.tblItem;
	pData->nUseEndTime = rData.nUseEndTime;
	pData->nUseStartTime = rData.nUseStartTime;

	memcpy(&pData->sOptionSet, &rData.sOptionSet, sizeof(rData.sOptionSet));

	m_mapItems.insert(std::make_pair(pData->itemId, pData));
}

bool CPlayerCache::DecreaseItemCount(ITEMID itemId, BYTE byCount, bool& rbDelete)
{
	std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapItems.find(itemId);
	if (it != m_mapItems.end())
	{
		sITEM_DATA* pData = it->second;

		pData->byStackcount = UnsignedSafeDecrease<BYTE>(pData->byStackcount, byCount);
		if (pData->byStackcount == 0)
		{
			rbDelete = true;

			delete pData;
			m_mapItems.erase(it);
		}

		return true;
	}

	return false;
}

void CPlayerCache::RenewItemDuration(DBOTIME endUseTime, ITEMID itemId)
{
	std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapItems.find(itemId);
	if (it != m_mapItems.end())
	{
		sITEM_DATA* pData = it->second;

		pData->nUseEndTime = endUseTime;
	}
}

void CPlayerCache::SetItemBattleAttribute(BYTE byBattleAttribute, ITEMID itemId)
{
	std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapItems.find(itemId);
	if (it != m_mapItems.end())
	{
		sITEM_DATA* pData = it->second;

		pData->byBattleAttribute = byBattleAttribute;
	}
}

sITEM_DATA * CPlayerCache::GetItemData(ITEMID itemId)
{
	std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapItems.find(itemId);
	if (it != m_mapItems.end())
	{
		return it->second;
	}

	return NULL;
}

sITEM_DATA * CPlayerCache::GetItemData(ITEMID itemId, BYTE byPlace, BYTE byPos)
{
	std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapItems.find(itemId);
	if (it != m_mapItems.end())
	{
		if (it->second->byPlace == byPlace && it->second->byPosition == byPos)
			return it->second;
	}

	return NULL;
}

void CPlayerCache::AddBankItem(sITEM_DATA & rData)
{
	sITEM_DATA* pData = new sITEM_DATA;
	memcpy(pData, &rData, sizeof(sITEM_DATA));

	m_mapBank.insert(std::make_pair(pData->itemId, pData));
}

void CPlayerCache::EraseBankItem(ITEMID itemId)
{
	m_mapBank.erase(itemId);
}

void CPlayerCache::InsertBankItem(sITEM_DATA* pData)
{
	m_mapBank.insert(std::make_pair(pData->itemId, pData));
}

sITEM_DATA * CPlayerCache::GetBankItem(ITEMID itemId, BYTE byPlace, BYTE byPos)
{
	std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapBank.find(itemId);
	if (it != m_mapBank.end())
	{
		if(it->second->byPlace == byPlace && it->second->byPosition == byPos)
			return it->second;
	}

	return NULL;
}

sITEM_DATA * CPlayerCache::GetBankItem(TBLIDX itemTblidx)
{
	for (std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapBank.begin(); it != m_mapBank.end(); it++)
	{
		if (it->second->itemNo == itemTblidx)
			return it->second;
	}

	return nullptr;
}

void CPlayerCache::SetRankBattleScoreInfo(sRANKBATTLE_SCORE_INFO & rNewInfo)
{
	memcpy(&m_sRankBattleScore, &rNewInfo, sizeof(sRANKBATTLE_SCORE_INFO));
}

bool CPlayerCache::HasMail(MAILID mailid)
{
	std::map<MAILID, sMAIL_DATA*>::iterator it = m_mapMail.find(mailid);
	if (it != m_mapMail.end())
	{
		return true;
	}

	return false;
}

void CPlayerCache::SetMailBrief(BYTE byMailCount, BYTE byUnReadManager, BYTE byUnReadNormal)
{
	m_sMailBrief.byMailCount = byMailCount;
	m_sMailBrief.byUnReadManager = byUnReadManager;
	m_sMailBrief.byUnReadNormal = byUnReadNormal;
}

void CPlayerCache::ScheduleReloadMailsResult(QueryResultVector & results, HOBJECT hHandle, bool bSchedule)
{
	CNtlPacket packet(sizeof(sQG_MAIL_RELOAD_RES));
	sQG_MAIL_RELOAD_RES * res = (sQG_MAIL_RELOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_MAIL_RELOAD_RES;
	res->handle = hHandle;
	res->hObject = INVALID_HOBJECT;
	res->charID = m_sPcData.charId;
	res->bIsSchedule = bSchedule;
	res->byCount = 0;
	res->wResultCode = 500;
	res->byTotalCount = 0;
	res->byNormalCount = 0;
	res->byManagerCount = 0;

	if (results[0].result)
	{
		do
		{
			Field* f = results[0].result->Fetch();

			BYTE sendertype = f[6].GetBYTE();

			res->asMailID[res->byCount] = f[0].GetUInt32();

			res->asMailInfo[res->byCount].byMailType = f[1].GetBYTE();
			res->asMailInfo[res->byCount].dwZenny = f[2].GetDWORD();
			res->asMailInfo[res->byCount].bIsRead = f[5].GetBool();
			res->asMailInfo[res->byCount].bIsAccept = f[3].GetBool();
			res->asMailInfo[res->byCount].bIsLock = f[4].GetBool();
			res->asMailInfo[res->byCount].bySenderType = sendertype;
			res->asMailInfo[res->byCount].endTime = f[7].GetUInt64();


			if (res->asMailInfo[res->byCount].bIsRead == false)
			{
				if (sendertype == eMAIL_SENDER_TYPE_BASIC || sendertype == eMAIL_SENDER_TYPE_REPLY || sendertype == eMAIL_SENDER_TYPE_RETURN)
					res->byNormalCount++;

				if (sendertype == eMAIL_SENDER_TYPE_SYSTEM || sendertype == eMAIL_SENDER_TYPE_GM || sendertype == eMAIL_SENDER_TYPE_QUEST)
					res->byManagerCount++;
			}

			res->byTotalCount++;
			res->byCount++;

		} while (results[0].result->NextRow());
	}

	SetMailBrief(res->byTotalCount, res->byNormalCount, res->byManagerCount);

	packet.SetPacketLen(sizeof(sQG_MAIL_RELOAD_RES));
	g_pApp->Send(GetSession(), &packet);
}

void CPlayerCache::StartMailResult(QueryResultVector & results, HOBJECT hHandle, HOBJECT hObject)
{
	if (results[0].result)
	{
		BYTE byTotalCount = (BYTE)results[0].result->GetRowCount();

		CNtlPacket packet(sizeof(sQG_MAIL_START_INFO));
		sQG_MAIL_START_INFO * res = (sQG_MAIL_START_INFO *)packet.GetPacketData();
		res->wOpCode = QG_MAIL_START_INFO;
		res->charID = m_sPcData.charId;
		res->byTotalCount = byTotalCount;

		do
		{
			Field* f = results[0].result->Fetch();

			if (sMAIL_DATA* pMailData = GetMailData(f[0].GetUInt32()))
			{
				memcpy(&res->asData[res->byCount], pMailData, sizeof(sMAIL_DATA));
			}
			else
			{
				res->asData[res->byCount].mailID = f[0].GetUInt32();
				res->asData[res->byCount].bySenderType = f[2].GetBYTE();
				res->asData[res->byCount].byMailType = f[3].GetBYTE();
				res->asData[res->byCount].byTextSize = f[4].GetBYTE();
				NTL_SAFE_WCSCPY(res->asData[res->byCount].wszText, s2ws(f[5].GetString()).c_str());
				res->asData[res->byCount].dwZenny = f[6].GetDWORD();
				wcscpy_s(res->asData[res->byCount].wszFromName, NTL_MAX_SIZE_CHAR_NAME + 1, s2ws(f[9].GetString()).c_str());
				res->asData[res->byCount].bIsAccept = f[10].GetBool();
				res->asData[res->byCount].bIsLock = f[11].GetBool();
				res->asData[res->byCount].bIsRead = f[12].GetBool();
				res->asData[res->byCount].endTime = f[14].GetUInt64();
				res->asData[res->byCount].byExpired = f[15].GetBYTE();
				res->asData[res->byCount].tCreateTime.year = f[16].GetWORD();
				res->asData[res->byCount].tCreateTime.month = f[17].GetBYTE();
				res->asData[res->byCount].tCreateTime.day = f[18].GetBYTE();
				res->asData[res->byCount].tCreateTime.hour = f[19].GetBYTE();
				res->asData[res->byCount].tCreateTime.minute = f[20].GetBYTE();
				res->asData[res->byCount].tCreateTime.second = f[21].GetBYTE();

				res->asData[res->byCount].sItemData.Init();

				ITEMID itemid = f[7].GetUInt64();

				if (itemid > 0)
				{
					smart_ptr<QueryResult> item = GetCharDB.Query("SELECT * FROM items WHERE id=%I64u LIMIT 1", itemid);
					if (item)
					{
						Field* i = item->Fetch();
						if (i)
						{
							res->asData[res->byCount].sItemData.itemId = itemid;
							res->asData[res->byCount].sItemData.itemNo = i[1].GetUInt32();
							res->asData[res->byCount].sItemData.charId = m_sPcData.charId;
							res->asData[res->byCount].sItemData.byPlace = i[3].GetBYTE();
							res->asData[res->byCount].sItemData.byPosition = i[4].GetBYTE();
							res->asData[res->byCount].sItemData.byStackcount = i[5].GetBYTE();
							res->asData[res->byCount].sItemData.byRank = i[6].GetBYTE();
							res->asData[res->byCount].sItemData.byCurrentDurability = i[7].GetBYTE();
							res->asData[res->byCount].sItemData.byGrade = i[8].GetBYTE();
							res->asData[res->byCount].sItemData.bNeedToIdentify = i[9].GetBool();
							res->asData[res->byCount].sItemData.byBattleAttribute = i[10].GetBYTE();
							wcscpy_s(res->asData[res->byCount].sItemData.awchMaker, NTL_MAX_SIZE_CHAR_NAME + 1, s2ws(i[11].GetString()).c_str());
							res->asData[res->byCount].sItemData.sOptionSet.aOptionTblidx[0] = i[12].GetUInt32();
							res->asData[res->byCount].sItemData.sOptionSet.aOptionTblidx[1] = i[13].GetUInt32();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[0].wOptionIndex = i[14].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[0].optionValue = i[15].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[1].wOptionIndex = i[16].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[1].optionValue = i[17].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[2].wOptionIndex = i[18].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[2].optionValue = i[19].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[3].wOptionIndex = i[20].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[3].optionValue = i[21].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[4].wOptionIndex = i[22].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[4].optionValue = i[23].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[5].wOptionIndex = i[24].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[5].optionValue = i[25].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[6].wOptionIndex = i[26].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[6].optionValue = i[27].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[7].wOptionIndex = i[28].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[7].optionValue = i[29].GetINT();
							res->asData[res->byCount].sItemData.nUseStartTime = (DBOTIME)i[30].GetDWORD();
							res->asData[res->byCount].sItemData.nUseEndTime = (DBOTIME)i[31].GetDWORD();
							res->asData[res->byCount].sItemData.byRestrictState = i[32].GetBYTE();
							res->asData[res->byCount].sItemData.byDurationType = i[33].GetBYTE();
						}
					}
				}

				AddMailData(res->asData[res->byCount]);
			}

			res->byCount++;
			res->byCurPacketCount++;

			if (res->byCurPacketCount == byTotalCount || res->byCount == NTL_MAX_LOAD_MAIL_PACKET)
			{
				packet.SetPacketLen(sizeof(sQG_MAIL_START_INFO));
				g_pApp->Send(GetSession(), &packet);

				res->byCount = 0;
			}

		} while (results[0].result->NextRow());
	}

	CNtlPacket packet2(sizeof(sQG_MAIL_START_RES));
	sQG_MAIL_START_RES * res2 = (sQG_MAIL_START_RES *)packet2.GetPacketData();
	res2->wOpCode = QG_MAIL_START_RES;
	res2->handle = hHandle;
	res2->hObject = hObject;
	res2->charID = GetCharID();
	res2->wResultCode = 500;
	res2->bIsAway = m_sPcData.bIsMailAway;
	packet2.SetPacketLen(sizeof(sQG_MAIL_START_RES));
	g_pApp->Send(GetSession(), &packet2);
}

void CPlayerCache::LoadMailResult(QueryResultVector & results, HOBJECT hHandle, HOBJECT hObject)
{
	if (results[0].result)
	{
		BYTE byTotalCount = (BYTE)results[0].result->GetRowCount();

		CNtlPacket packet(sizeof(sQG_MAIL_LOAD_INFO));
		sQG_MAIL_LOAD_INFO * res = (sQG_MAIL_LOAD_INFO *)packet.GetPacketData();
		res->wOpCode = QG_MAIL_LOAD_INFO;
		res->charID = m_sPcData.charId;
		res->byTotalCount = byTotalCount;

		do
		{
			Field* f = results[0].result->Fetch();

			if (sMAIL_DATA* pMailData = GetMailData(f[0].GetUInt32()))
			{
				memcpy(&res->asData[res->byCount], pMailData, sizeof(sMAIL_DATA));
			}
			else
			{
				res->asData[res->byCount].mailID = f[0].GetUInt32();
				res->asData[res->byCount].bySenderType = f[2].GetBYTE();
				res->asData[res->byCount].byMailType = f[3].GetBYTE();
				res->asData[res->byCount].byTextSize = f[4].GetBYTE();
				NTL_SAFE_WCSCPY(res->asData[res->byCount].wszText, s2ws(f[5].GetString()).c_str());
				res->asData[res->byCount].dwZenny = f[6].GetDWORD();
				wcscpy_s(res->asData[res->byCount].wszFromName, NTL_MAX_SIZE_CHAR_NAME + 1, s2ws(f[9].GetString()).c_str());
				res->asData[res->byCount].bIsAccept = f[10].GetBool();
				res->asData[res->byCount].bIsLock = f[11].GetBool();
				res->asData[res->byCount].bIsRead = f[12].GetBool();
				res->asData[res->byCount].endTime = f[14].GetUInt64();
				res->asData[res->byCount].byExpired = f[15].GetBYTE();
				res->asData[res->byCount].tCreateTime.year = f[16].GetWORD();
				res->asData[res->byCount].tCreateTime.month = f[17].GetBYTE();
				res->asData[res->byCount].tCreateTime.day = f[18].GetBYTE();
				res->asData[res->byCount].tCreateTime.hour = f[19].GetBYTE();
				res->asData[res->byCount].tCreateTime.minute = f[20].GetBYTE();
				res->asData[res->byCount].tCreateTime.second = f[21].GetBYTE();

				res->asData[res->byCount].sItemData.Init();

				ITEMID itemid = f[7].GetUInt64();

				if (itemid > 0)
				{
					smart_ptr<QueryResult> item = GetCharDB.Query("SELECT * FROM items WHERE id=%I64u LIMIT 1", itemid);
					if (item)
					{
						Field* i = item->Fetch();
						if (i)
						{
							res->asData[res->byCount].sItemData.itemId = itemid;
							res->asData[res->byCount].sItemData.itemNo = i[1].GetUInt32();
							res->asData[res->byCount].sItemData.charId = m_sPcData.charId;
							res->asData[res->byCount].sItemData.byPlace = i[3].GetBYTE();
							res->asData[res->byCount].sItemData.byPosition = i[4].GetBYTE();
							res->asData[res->byCount].sItemData.byStackcount = i[5].GetBYTE();
							res->asData[res->byCount].sItemData.byRank = i[6].GetBYTE();
							res->asData[res->byCount].sItemData.byCurrentDurability = i[7].GetBYTE();
							res->asData[res->byCount].sItemData.byGrade = i[8].GetBYTE();
							res->asData[res->byCount].sItemData.bNeedToIdentify = i[9].GetBool();
							res->asData[res->byCount].sItemData.byBattleAttribute = i[10].GetBYTE();
							wcscpy_s(res->asData[res->byCount].sItemData.awchMaker, NTL_MAX_SIZE_CHAR_NAME + 1, s2ws(i[11].GetString()).c_str());
							res->asData[res->byCount].sItemData.sOptionSet.aOptionTblidx[0] = i[12].GetUInt32();
							res->asData[res->byCount].sItemData.sOptionSet.aOptionTblidx[1] = i[13].GetUInt32();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[0].wOptionIndex = i[14].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[0].optionValue = i[15].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[1].wOptionIndex = i[16].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[1].optionValue = i[17].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[2].wOptionIndex = i[18].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[2].optionValue = i[19].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[3].wOptionIndex = i[20].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[3].optionValue = i[21].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[4].wOptionIndex = i[22].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[4].optionValue = i[23].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[5].wOptionIndex = i[24].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[5].optionValue = i[25].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[6].wOptionIndex = i[26].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[6].optionValue = i[27].GetINT();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[7].wOptionIndex = i[28].GetWORD();
							res->asData[res->byCount].sItemData.sOptionSet.aRandomOption[7].optionValue = i[29].GetINT();
							res->asData[res->byCount].sItemData.nUseStartTime = (DBOTIME)i[30].GetDWORD();
							res->asData[res->byCount].sItemData.nUseEndTime = (DBOTIME)i[31].GetDWORD();
							res->asData[res->byCount].sItemData.byRestrictState = i[32].GetBYTE();
							res->asData[res->byCount].sItemData.byDurationType = i[33].GetBYTE();
						}
					}
				}

				AddMailData(res->asData[res->byCount]);
			}

			res->byCount++;
			res->byCurPacketCount++;

			if (res->byCurPacketCount == byTotalCount || res->byCount == NTL_MAX_LOAD_MAIL_PACKET)
			{
				packet.SetPacketLen(sizeof(sQG_MAIL_LOAD_INFO));
				g_pApp->Send(GetSession(), &packet);

				res->byCount = 0;
			}

		} while (results[0].result->NextRow());
	}

	CNtlPacket packet2(sizeof(sQG_MAIL_LOAD_RES));
	sQG_MAIL_LOAD_RES * res2 = (sQG_MAIL_LOAD_RES *)packet2.GetPacketData();
	res2->wOpCode = QG_MAIL_LOAD_RES;
	res2->handle = hHandle;
	res2->hObject = hObject;
	res2->charID = m_sPcData.charId;
	res2->wResultCode = 500;
	packet2.SetPacketLen(sizeof(sQG_MAIL_LOAD_RES));
	g_pApp->Send(GetSession(), &packet2);
}

void CPlayerCache::AddMailData(sMAIL_DATA & rMailData)
{
	sMAIL_DATA* pMail = new sMAIL_DATA;
	memcpy(pMail, &rMailData, sizeof(sMAIL_DATA));

	m_mapMail.insert(std::make_pair(pMail->mailID, pMail));
}

sMAIL_DATA * CPlayerCache::GetMailData(MAILID mailId)
{
	std::map<MAILID, sMAIL_DATA*>::iterator it = m_mapMail.find(mailId);
	if (it != m_mapMail.end())
	{
		return it->second;
	}

	return NULL;
}

bool CPlayerCache::DeleteMail(MAILID mailId)
{
	std::map<MAILID, sMAIL_DATA*>::iterator it = m_mapMail.find(mailId);
	if (it != m_mapMail.end())
	{
		sMAIL_DATA* pData = it->second;

		delete pData;
		m_mapMail.erase(it);

		return true;
	}

	return false;
}

void CPlayerCache::DecreaseMailUnreadNormal()
{
	m_sMailBrief.byUnReadNormal = UnsignedSafeDecrease<BYTE>(m_sMailBrief.byUnReadNormal, 1);
}

void CPlayerCache::DecreaseMailUnreadManager()
{
	m_sMailBrief.byUnReadManager = UnsignedSafeDecrease<BYTE>(m_sMailBrief.byUnReadManager, 1);
}

bool CPlayerCache::CanInitSkills()
{
	DBOTIME curTime = time(0);

	if (m_tNextSkillReset == INVALID_DBOTIME)
	{
		m_tNextSkillReset = curTime + 3600 * 24 * 7; // 7 days

		return true;
	}
	else if (m_tNextSkillReset <= curTime)
	{
		m_tNextSkillReset = curTime + 3600 * 24 * 7; // 7 days

		return true;
	}

	return false;
}




CPlayerCacheManager::CPlayerCacheManager()
{
	Init();
}

CPlayerCacheManager::~CPlayerCacheManager()
{
	Destroy();
}

void CPlayerCacheManager::Init()
{

}

void CPlayerCacheManager::Destroy()
{
	for (auto it = m_mapAccounts.begin(); it != m_mapAccounts.end(); )
	{
		delete it->second;

		it = m_mapAccounts.erase(it);
	}

	for (auto it = m_mapCharacters.begin(); it != m_mapCharacters.end(); )
	{
		delete it->second;

		it = m_mapCharacters.erase(it);
	}
}

CAccountCache * CPlayerCacheManager::GetAccount(ACCOUNTID accountID)
{
	auto it = m_mapAccounts.find(accountID);
	if (it != m_mapAccounts.end())
	{
		return it->second;
	}

	return nullptr;
}

CPlayerCache * CPlayerCacheManager::GetCharacter(CHARACTERID charID)
{
	auto it = m_mapCharacters.find(charID);
	if (it != m_mapCharacters.end())
	{
		return it->second;
	}

	return nullptr;
}

void CPlayerCacheManager::InsertAccount(ACCOUNTID accountID, CAccountCache * pAccountCache)
{
	m_mapAccounts.insert(std::make_pair(accountID, pAccountCache));
}

void CPlayerCacheManager::InsertCharacter(CHARACTERID charID, CPlayerCache * pPlayerCache)
{
	m_mapCharacters.insert(std::make_pair(charID, pPlayerCache));
}

void CPlayerCacheManager::EraseCharacter(CHARACTERID charID)
{
	m_mapCharacters.erase(charID);
}
