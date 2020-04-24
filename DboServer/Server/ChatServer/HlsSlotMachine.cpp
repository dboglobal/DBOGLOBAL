#include "stdafx.h"
#include "HlsSlotMachine.h"
#include "TableContainerManager.h"
#include "HLSItemTable.h"
#include "SlotMachineTable.h"
#include "NtlRandom.h"
#include "Player.h"
#include <time.h>
#include "NtlPacketTU.h"


CHlsSlotMachine::CHlsSlotMachine()
{
	//init winners
	memset(m_aWinnerIndex, 0, sizeof(m_aWinnerIndex));

	Init();
}

CHlsSlotMachine::~CHlsSlotMachine()
{
}


void CHlsSlotMachine::Init()
{
	//delete items
	for (SLOTMACHINEGROUP::iterator it = m_slotMachineGroup.begin(); it != m_slotMachineGroup.end();)
	{
		delete it->second;
		it = m_slotMachineGroup.erase(it);
	}

	//delete slot machine
	for (std::map<TBLIDX, sSLOT_MACHINE*>::iterator it = m_mapSlotMachine.begin(); it != m_mapSlotMachine.end();)
	{
		delete it->second;
		it = m_mapSlotMachine.erase(it);
	}


	CHlsSlotMachineItemTable* pItemTable = g_pTableContainer->GetSlotMachineItemTable();
	for (CTable::TABLEIT it = pItemTable->Begin(); it != pItemTable->End(); it++)
	{
		sHLS_SLOT_MACHINE_ITEM_TBLDAT* pHlsSlotItem = (sHLS_SLOT_MACHINE_ITEM_TBLDAT*)it->second;

		if (pHlsSlotItem->bActive == false)
			continue;

		sHLS_ITEM_TBLDAT* pHlsItem = (sHLS_ITEM_TBLDAT*)g_pTableContainer->GetHLSItemTable()->FindData(pHlsSlotItem->cashItemTblidx);
		if(pHlsItem)
		{
			sHLS_SLOT_ITEM* pSlot = new sHLS_SLOT_ITEM;

			pSlot->wCountLeft = (pHlsSlotItem->byStackCount > 0) ? pHlsSlotItem->byStackCount : INVALID_WORD;
			pSlot->byRank = 0;
			pSlot->fPercent = (float)pHlsSlotItem->byPercent;
			pSlot->pHlsItem = pHlsItem;
			pSlot->pSlotItem = pHlsSlotItem;

			m_slotMachineGroup.insert(SLOTMACHINEGROUP_VAL(pHlsSlotItem->slotMachineTblidx, pSlot));
		}
	}

	//loop the slots to get the top 10 items
	CSlotMachineTable* pSlotTable = g_pTableContainer->GetSlotMachineTable();
	for (CTable::TABLEIT it = pSlotTable->Begin(); it != pSlotTable->End(); it++)
	{
		sHLS_SLOT_MACHINE_TBLDAT* pHlsMachineTbldat = (sHLS_SLOT_MACHINE_TBLDAT*)it->second;

		if (pHlsMachineTbldat->bOnOff == false)
			continue;

		//insert slot machine
		sSLOT_MACHINE* pSlotMachine = new sSLOT_MACHINE;
		pSlotMachine->pTbldat = pHlsMachineTbldat;
		pSlotMachine->wCurrentCapsule = 800;
		m_mapSlotMachine.insert({ pHlsMachineTbldat->tblidx, pSlotMachine });

		for (BYTE i = 0; i < DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS; i++)
		{
			if (pHlsMachineTbldat->aItemTblidx[i] == INVALID_TBLIDX)
				break;

			sHLS_ITEM_TBLDAT* pHlsItem = (sHLS_ITEM_TBLDAT*)g_pTableContainer->GetHLSItemTable()->FindData(pHlsMachineTbldat->aItemTblidx[i]);
			if (pHlsItem)
			{
				sHLS_SLOT_MACHINE_ITEM_TBLDAT* pHlsSlotItem = (sHLS_SLOT_MACHINE_ITEM_TBLDAT*)g_pTableContainer->GetSlotMachineItemTable()->FindData(pHlsMachineTbldat->aItemTblidx[i]);
				if (pHlsSlotItem)
				{
					sHLS_SLOT_ITEM* pSlot = new sHLS_SLOT_ITEM;

					pSlot->wCountLeft = 1;
					pSlot->byRank = i + 1;
					pSlot->fPercent = 0.0f;
					pSlot->pHlsItem = pHlsItem;
					pSlot->pSlotItem = pHlsSlotItem;

					m_slotMachineGroup.insert(SLOTMACHINEGROUP_VAL(pHlsSlotItem->slotMachineTblidx, pSlot));
				}
				else printf("pHlsMachineTbldat->aItemTblidx[i] %u not found in  table_slot_machine_item_data", pHlsMachineTbldat->aItemTblidx[i]);
			}
		}
	}
}


void CHlsSlotMachine::GetSlotItems(TBLIDX slotIdx, std::vector<sHLS_SLOT_ITEM*>* pVec)
{
	sSLOT_MACHINE* pSlot = GetSlotMachine(slotIdx);
	if (pSlot == NULL)
		return;

	std::multimap<TBLIDX, sHLS_SLOT_ITEM*>::iterator itLow = m_slotMachineGroup.lower_bound(slotIdx);
	std::multimap<TBLIDX, sHLS_SLOT_ITEM*>::iterator itUp = m_slotMachineGroup.upper_bound(slotIdx);

	while (itLow != itUp)
	{
		sHLS_SLOT_ITEM* pTbldat = itLow->second;

		if (pTbldat->wCountLeft > 0)
		{
			if (pTbldat->byRank == 0)
			{
				if (Dbo_CheckProbabilityF(pTbldat->fPercent))
					pVec->push_back(pTbldat);
			}
			else
			{
				//if (Dbo_CheckProbabilityF((float)pTbldat->byRank + float(pSlot->wMaxCapsule / pSlot->wCurrentCapsule)))
					pVec->push_back(pTbldat);
			}
		}

		++itLow;
	}
}

void CHlsSlotMachine::AddWinner(TBLIDX slotId, TBLIDX itemTblidx, CPlayer * pPlayer)
{
	m_aWinnerIndex[slotId - 1] += 1;

	sHLS_SLOT_WINNER_INFO* pWinner = new sHLS_SLOT_WINNER_INFO;
	pWinner->nExtractTime = time(NULL);
	NTL_SAFE_WCSCPY(pWinner->wszPlayer, pPlayer->GetCharName());
	pWinner->wWinCount = pPlayer->GetSlotMachineCount();
	pWinner->winnerIndex = m_aWinnerIndex[slotId - 1];

	m_slotWinnerInfo[slotId - 1].push_back(pWinner);

	if (m_slotWinnerInfo[slotId - 1].size() > 3)
	{
		sHLS_SLOT_WINNER_INFO* pInfo = m_slotWinnerInfo[slotId - 1].front();
		SAFE_DELETE(pInfo);
		m_slotWinnerInfo[slotId - 1].pop_front();
	}
}

void CHlsSlotMachine::GetWinnerInfo(WORD wSlot, CPlayer * pPlayer)
{
	if (wSlot > 1)
		return;

	CNtlPacket packet(sizeof(sTU_HLS_SLOT_MACHINE_WINNER_INFO_RES));
	sTU_HLS_SLOT_MACHINE_WINNER_INFO_RES* res = (sTU_HLS_SLOT_MACHINE_WINNER_INFO_RES*)packet.GetPacketData();
	res->wOpCode = TU_HLS_SLOT_MACHINE_WINNER_INFO_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->wMachineIndex = wSlot;
	res->byInfoCount = 0;

	for (std::list<sHLS_SLOT_WINNER_INFO*>::iterator it = m_slotWinnerInfo[wSlot - 1].begin(); it != m_slotWinnerInfo[wSlot - 1].end(); it++)
	{
		sHLS_SLOT_WINNER_INFO* pInfo = *it;

		NTL_SAFE_WCSCPY(res->wszPlayer[res->byInfoCount], pInfo->wszPlayer);
		res->wWinCount[res->byInfoCount] = pInfo->wWinCount;
		res->nExtractTime[res->byInfoCount] = pInfo->nExtractTime;
		res->nWinnerIndex[res->byInfoCount] = (WORD)pInfo->winnerIndex;

		if (++res->byInfoCount == DBO_MAX_HLS_SLOT_MACHINES_MAX_WINNERS)
			break;
	}

	packet.SetPacketLen(sizeof(sTU_HLS_SLOT_MACHINE_WINNER_INFO_RES));
	pPlayer->SendPacket(&packet);
}

void CHlsSlotMachine::LoadSlotMachines(CPlayer* pPlayer, BYTE byType)
{
	BYTE i = 0;

	CNtlPacket packet(sizeof(sTU_HLS_SLOT_MACHINE_INFO_RES));
	sTU_HLS_SLOT_MACHINE_INFO_RES* res = (sTU_HLS_SLOT_MACHINE_INFO_RES*)packet.GetPacketData();
	res->wOpCode = TU_HLS_SLOT_MACHINE_INFO_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->byType = byType;

	for (std::map<TBLIDX, sSLOT_MACHINE*>::iterator it = m_mapSlotMachine.begin(); it != m_mapSlotMachine.end(); it++)
	{
		sSLOT_MACHINE* pSlotMachine = (sSLOT_MACHINE*)it->second;

		if (pSlotMachine->pTbldat->byType == byType)
		{
			res->wMachineIndex[i] = pSlotMachine->pTbldat->tblidx;
			res->byCoin[i] = (BYTE)pSlotMachine->pTbldat->byCoin;
			res->bOnOff[i] = pSlotMachine->pTbldat->bOnOff;
			memcpy(res->ItemTblidx[i], pSlotMachine->pTbldat->aItemTblidx, sizeof(pSlotMachine->pTbldat->aItemTblidx));
			res->wWaitingTime[i] = 0;
			res->wCurrentCapsule[i] = pSlotMachine->wCurrentCapsule;
			res->wMaxCapsule[i] = pSlotMachine->wMaxCapsule;

			i++;
		}
	}

	res->byMachineCount = i;
	packet.SetPacketLen(sizeof(sTU_HLS_SLOT_MACHINE_INFO_RES));
	pPlayer->SendPacket(&packet);
}

sSLOT_MACHINE * CHlsSlotMachine::GetSlotMachine(TBLIDX tblidx)
{
	std::map<TBLIDX, sSLOT_MACHINE*>::iterator it = m_mapSlotMachine.find(tblidx);
	if (it != m_mapSlotMachine.end())
		return it->second;

	return nullptr;
}
