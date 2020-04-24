#include "stdafx.h"
#include "HoneyBeeEvent.h"
#include "GameServer.h"
#include "NtlPacketGU.h"
#include "NtlStringW.h"
#include "ObjectManager.h"
#include "NtlAdmin.h"
#include "Monster.h"
#include "NtlRandom.h"
#include "TableContainerManager.h"
#include "ItemManager.h"
#include "ItemDrop.h"


CHoneyBeeEvent::CHoneyBeeEvent()
{
	Init();
}

CHoneyBeeEvent::~CHoneyBeeEvent()
{
}

void CHoneyBeeEvent::Init()
{
	m_bOn = false;
	m_timeStart = 0;
	m_timeEnd = 0;
	m_dwNextUpdateTick = 0;
	m_nMonsterSummoned = 0;
}


void CHoneyBeeEvent::StartEvent(BYTE byHours/* = 3*/)
{
	if (m_bOn)
		return;

	CGameServer* app = (CGameServer*)g_pApp;

	m_bOn = true;
	m_timeStart = app->GetTime();
	m_timeEnd = m_timeStart + (byHours * 3600);

	CNtlStringW msg;

	CNtlPacket packet(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT * res = (sGU_SYSTEM_DISPLAY_TEXT*)packet.GetPacketData();
	res->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	res->wMessageLengthInUnicode = (WORD)msg.Format(L"HoneyBee Event Started. Kill Monsters to summon Bees. You have %u hours time to farm", byHours);
	res->byDisplayType = SERVER_TEXT_EMERGENCY;
	NTL_SAFE_WCSCPY(res->awchMessage, msg.c_str());
	packet.SetPacketLen(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	g_pObjectManager->SendPacketToAll(&packet);
}

void CHoneyBeeEvent::TickProcess(DWORD dwTick)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (dwTick < m_dwNextUpdateTick)
		return;

	if (m_bOn)
	{
		if (app->GetTime() >= m_timeEnd)
		{
			EndEvent();
		}
	}

	m_dwNextUpdateTick = dwTick + 5000; //update again in 5 seconds
}

void CHoneyBeeEvent::Update(CMonster * pMob, CCharacter * pPlayer)
{
	if (m_bOn == false)
		return;

	if (pPlayer->GetCurWorld() == NULL)
		return;
	if (pPlayer->GetCurWorld()->GetRuleType() != GAMERULE_NORMAL)
		return;

	if (pMob->GetTblidx() == 2741101)
	{
		CItemDrop* pDrop = g_pItemManager->CreateSingleDrop(70.f, 11160033);
		if (pDrop)
		{
			sVECTOR3 pos;
			pos.x = pMob->GetCurLoc().x + RandomRangeF(-2.0f, 2.0f);
			pos.y = pMob->GetCurLoc().y;
			pos.z = pMob->GetCurLoc().z + RandomRangeF(-2.0f, 2.0f);

			pDrop->SetNeedToIdentify(false);
			pDrop->SetOwnership(pPlayer->GetID(), pPlayer->GetPartyID());
			pDrop->StartDestroyEvent();
			pDrop->AddToGround(pMob->GetWorldID(), pos);
		}

		return;
	}
	else if (pMob->GetTblidx() == 2741102)
	{
		CItemDrop* pDrop = g_pItemManager->CreateSingleDrop(70.f, 11160034);
		if (pDrop)
		{
			sVECTOR3 pos;
			pos.x = pMob->GetCurLoc().x + RandomRangeF(-2.0f, 2.0f);
			pos.y = pMob->GetCurLoc().y;
			pos.z = pMob->GetCurLoc().z + RandomRangeF(-2.0f, 2.0f);

			pDrop->SetNeedToIdentify(false);
			pDrop->SetOwnership(pPlayer->GetID(), pPlayer->GetPartyID());
			pDrop->StartDestroyEvent();
			pDrop->AddToGround(pMob->GetWorldID(), pos);
		}

		return;
	}
	else if (pMob->GetTblidx() == 7441101)
	{
		CItemDrop* pDrop = g_pItemManager->CreateSingleDrop(70.f, 11160035);
		if (pDrop)
		{
			sVECTOR3 pos;
			pos.x = pMob->GetCurLoc().x + RandomRangeF(-2.0f, 2.0f);
			pos.y = pMob->GetCurLoc().y;
			pos.z = pMob->GetCurLoc().z + RandomRangeF(-2.0f, 2.0f);

			pDrop->SetNeedToIdentify(false);
			pDrop->SetOwnership(pPlayer->GetID(), pPlayer->GetPartyID());
			pDrop->StartDestroyEvent();
			pDrop->AddToGround(pMob->GetWorldID(), pos);
		}

		return;
	}

	if (Dbo_CheckProbability(5) == false)
		return;

	if (pMob->GetLevel() >= 1 && pMob->GetLevel() < 30)
	{
		sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(2741101);
		if (pTbldat)
		{
			sMOB_DATA data;
			InitMobData(data);

			data.worldID = pMob->GetWorldID();
			data.worldtblidx = pMob->GetWorldTblidx();

			data.tblidx = pTbldat->tblidx;

			pMob->GetCurLoc().CopyTo(data.vCurLoc);
			pMob->GetCurLoc().CopyTo(data.vSpawnLoc);
			pMob->GetCurDir().CopyTo(data.vCurDir);
			pMob->GetCurDir().CopyTo(data.vSpawnDir);

			data.actionpatternTblIdx = 1;

			CMonster* pNewMob = (CMonster*)g_pObjectManager->CreateCharacter(OBJTYPE_MOB);
			if (pNewMob)
			{
				if (pNewMob->CreateDataAndSpawn(data, pTbldat))
				{
					pNewMob->SetStandAlone(true);
				}
			}
		}
	}
	else if (pMob->GetLevel() >= 30 && pMob->GetLevel() < 50)
	{
		sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(2741102);
		if (pTbldat)
		{
			sMOB_DATA data;
			InitMobData(data);

			data.worldID = pMob->GetWorldID();
			data.worldtblidx = pMob->GetWorldTblidx();

			data.tblidx = pTbldat->tblidx;

			pMob->GetCurLoc().CopyTo(data.vCurLoc);
			pMob->GetCurLoc().CopyTo(data.vSpawnLoc);
			pMob->GetCurDir().CopyTo(data.vCurDir);
			pMob->GetCurDir().CopyTo(data.vSpawnDir);

			data.actionpatternTblIdx = 1;

			CMonster* pNewMob = (CMonster*)g_pObjectManager->CreateCharacter(OBJTYPE_MOB);
			if (pNewMob)
			{
				if (pNewMob->CreateDataAndSpawn(data, pTbldat))
				{
					pNewMob->SetStandAlone(true);
				}
			}
		}
	}
	else
	{
		sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(7441101);
		if (pTbldat)
		{
			sMOB_DATA data;
			InitMobData(data);

			data.worldID = pMob->GetWorldID();
			data.worldtblidx = pMob->GetWorldTblidx();

			data.tblidx = pTbldat->tblidx;

			pMob->GetCurLoc().CopyTo(data.vCurLoc);
			pMob->GetCurLoc().CopyTo(data.vSpawnLoc);
			pMob->GetCurDir().CopyTo(data.vCurDir);
			pMob->GetCurDir().CopyTo(data.vSpawnDir);

			data.actionpatternTblIdx = 1;

			CMonster* pNewMob = (CMonster*)g_pObjectManager->CreateCharacter(OBJTYPE_MOB);
			if (pNewMob)
			{
				if (pNewMob->CreateDataAndSpawn(data, pTbldat))
				{
					pNewMob->SetStandAlone(true);
				}
			}
		}
	}
}

void CHoneyBeeEvent::EndEvent()
{
	if (m_bOn == false)
		return;

	Init();

	CNtlStringW msg;

	CNtlPacket packet(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT * res = (sGU_SYSTEM_DISPLAY_TEXT*)packet.GetPacketData();
	res->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	res->wMessageLengthInUnicode = (WORD)msg.Format(L"HoneyBee Event Ended");
	res->byDisplayType = SERVER_TEXT_EMERGENCY;
	NTL_SAFE_WCSCPY(res->awchMessage, msg.c_str());
	packet.SetPacketLen(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	g_pObjectManager->SendPacketToAll(&packet);
}

void CHoneyBeeEvent::LoadEvent(HSESSION hSession)
{
	if (m_bOn == false)
		return;

	CNtlStringW msg;

	CNtlPacket packetMsg(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT * resMsg = (sGU_SYSTEM_DISPLAY_TEXT *)packetMsg.GetPacketData();
	resMsg->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	resMsg->byDisplayType = SERVER_TEXT_EMERGENCY;
	resMsg->wMessageLengthInUnicode = (WORD)msg.Format(L"HoneyBee Event is currently running!");
	NTL_SAFE_WCSCPY(resMsg->awchMessage, msg.c_str());
	g_pApp->Send(hSession, &packetMsg);
}
