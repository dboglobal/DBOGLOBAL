#include "stdafx.h"
#include "GameProcessor.h"
#include "GameMain.h"
#include "NtlPacketGU.h"
#include "NtlPacketGM.h"
#include "GameServer.h"
#include "trade.h"
#include "ScriptAlgoObjectManager.h"
#include "RankBattle.h"
#include "DynamicFieldSystemEvent.h"
#include "DragonballHunt.h"
#include "DragonballScramble.h"
#include "DungeonManager.h"
#include "PacketEvent.h"
#include "Party.h"
#include "DojoManager.h"
#include "BudokaiManager.h"
#include "ExpEvent.h"
#include "BusSystem.h"
#include "HoneyBeeEvent.h"



CGameProcessor::CGameProcessor()
{
	Init();
}

CGameProcessor::~CGameProcessor()
{
	m_dwLastTimeGameMainUpdated = 0;
	SAFE_DELETE(m_pEventHolder);
}


void CGameProcessor::Init()
{
	m_dwTickOld = GetTickCount();
	m_dwTickCount = m_dwTickOld;
	m_dwLastTimeGameMainUpdated = m_dwTickOld;
	m_fMultiple = 0.0f;
	m_dwLogTick = 0;
}


bool CGameProcessor::Create(DWORD dwTimeTick, CGameMain* pGameMain)
{
	m_dwTickTime = dwTimeTick;
	m_pEventHolder = new EventableObjectHolder(-1);
	m_pGameMainRef = pGameMain;
	m_pPacketEvent = new CPacketEvent;

	return true;
}


void CGameProcessor::Run(DWORD dwTickCount)
{
	CGameServer* app = (CGameServer*)g_pApp;

	m_dwTickCount = dwTickCount;
	m_dwTickDiff = m_dwTickCount - m_dwTickOld;

	m_dwTickOld = m_dwTickCount;

	if (m_dwTickDiff > 0)
		m_pEventHolder->Update(m_dwTickDiff);

	m_pPacketEvent->ProcessEventQueue(); //process packets

	
	if (m_dwTickCount - m_dwLastTimeGameMainUpdated >= m_dwTickTime)
	{
	//	ERR_LOG(LOG_FIRST, "m_dwTickCount %u - m_dwLastTimeGameMainUpdated %u >= m_dwTickTime %u\n", m_dwTickCount, m_dwLastTimeGameMainUpdated, m_dwTickTime);
		LARGE_INTEGER m_freq, rStart, rEvent, rWorld, rObject, rEnd, rSpawn, rItem, rShenron, rTrade, rDungeon, rScript, rRankBattle, rDynamicEvent, rDbHunt, rParty;

		float fMultiple = (float)m_dwTickDiff / (float)m_dwTickTime;
		DWORD dwTickDiff = m_dwTickCount - m_dwLastTimeGameMainUpdated;

		m_dwLastTimeGameMainUpdated = m_dwTickCount;

		QueryPerformanceFrequency(&m_freq);
		QueryPerformanceCounter(&rStart);

		QueryPerformanceCounter(&rEvent);

		g_pItemManager->TickProcess(dwTickDiff, fMultiple);
		QueryPerformanceCounter(&rItem);

		g_pShenronManager->TickProcess(dwTickDiff, fMultiple);
		QueryPerformanceCounter(&rShenron);

		g_pTradeManager->TickProcess(m_dwTickCount);
		QueryPerformanceCounter(&rTrade);

		m_pGameMainRef->TickProcess(dwTickDiff, fMultiple, &rWorld, &rObject, &rSpawn);

		if (app->IsDojoChannel())
		{
			g_pDojoManager->TickProcess(dwTickDiff);
			g_pBudokaiManager->TickProcess(dwTickDiff);
		}
		else
		{
			if (app->m_config.byChannel == 0)
				g_pDragonballScramble->TickProcess(m_dwTickCount);

			g_pBusSystem->TickProcess(dwTickDiff);

			g_pDungeonManager->TickProcess(dwTickDiff);
			QueryPerformanceCounter(&rDungeon);

			g_pScriptAlgoManager->TickProcess(dwTickDiff);
			QueryPerformanceCounter(&rScript);

			g_pRankbattleManager->TickProcess(dwTickDiff);	// for some reason it lags the server.. need to be checked
			QueryPerformanceCounter(&rRankBattle);

			g_pDynamicFieldSystemEvent->TickProcess(dwTickDiff);
			QueryPerformanceCounter(&rDynamicEvent);

			g_pDragonballHuntEvent->TickProcess(m_dwTickCount);
			QueryPerformanceCounter(&rDbHunt);

			g_pExpEvent->TickProcess(dwTickDiff);
			g_pHoneyBeeEvent->TickProcess(m_dwTickCount);
		}

		g_pPartyManager->TickProcess(dwTickDiff);
		QueryPerformanceCounter(&rParty);

		QueryPerformanceCounter(&rEnd);

		m_dwLogTick += dwTickDiff;

	//	if (m_dwLogTick >= 1000) //log every 10 seconds
		{
			float fTimeDif = ((float)(rEnd.QuadPart - rStart.QuadPart)) * 1000.f / ((float)m_freq.QuadPart);
			if (fTimeDif > 200.f)
			{
				ERR_LOG(LOG_SYSTEM, "LOG-TICK: fTimeDif = %f, Event %f, Item %f, Shenron %f, Trade %f, World %f, Object %f, Spawn %f, Dungeon %f, Script %f, RankBattle %f, DynamicEvent %f, DbHunt %f, rParty %f",
					fTimeDif,
					((float)(rEvent.QuadPart - rStart.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
					((float)(rItem.QuadPart - rEvent.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
					((float)(rShenron.QuadPart - rItem.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
					((float)(rTrade.QuadPart - rShenron.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
					((float)(rWorld.QuadPart - rTrade.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
					((float)(rObject.QuadPart - rWorld.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
					((float)(rSpawn.QuadPart - rObject.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
					((float)(rDungeon.QuadPart - rSpawn.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
					((float)(rScript.QuadPart - rDungeon.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
					((float)(rRankBattle.QuadPart - rScript.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
					((float)(rDynamicEvent.QuadPart - rRankBattle.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
					((float)(rDbHunt.QuadPart - rDynamicEvent.QuadPart)) * 1000.f / ((float)m_freq.QuadPart),
					((float)(rParty.QuadPart - rDbHunt.QuadPart)) * 1000.f / ((float)m_freq.QuadPart)
				);
			}

			m_dwLogTick = 0;
		}
		
	}
}

void CGameProcessor::PostClientPacketEvent(CPacketEventObj * pEvent)
{
	m_pPacketEvent->PostEvent(pEvent);
}


void CGameProcessor::StartServerShutdownEvent()
{
	if (g_pEventMgr->HasEvent(this, EVENT_SERVER_SHUT_DOWN))
	{
		EndServerShutdownEvent();
		return;
	}

	CNtlPacket packet(sizeof(sGU_SHUTDOWN_COUNT_DOWN_START_NFY));
	sGU_SHUTDOWN_COUNT_DOWN_START_NFY * res = (sGU_SHUTDOWN_COUNT_DOWN_START_NFY *)packet.GetPacketData();
	res->wOpCode = GU_SHUTDOWN_COUNT_DOWN_START_NFY;
	res->byMsgType = 1;
	packet.SetPacketLen(sizeof(sGU_SHUTDOWN_COUNT_DOWN_START_NFY));
	g_pObjectManager->SendPacketToAll(&packet);


	//Send packet to master server so no new people can connect.
	CGameServer* app = (CGameServer*)g_pApp;
	CNtlPacket packet2(sizeof(sGM_SERVER_SHUT_DOWN));
	sGM_SERVER_SHUT_DOWN * res2 = (sGM_SERVER_SHUT_DOWN *)packet2.GetPacketData();
	res2->wOpCode = GM_SERVER_SHUT_DOWN;
	packet2.SetPacketLen(sizeof(sGM_SERVER_SHUT_DOWN));
	app->SendTo(app->GetMasterServerSession(), &packet2);

	g_pEventMgr->AddEvent(this, &CGameProcessor::OnEvent_ServerShutdown, EVENT_SERVER_SHUT_DOWN, 30000, 0xFFFFFFFF, 0);
}

void CGameProcessor::EndServerShutdownEvent()
{
	g_pEventMgr->RemoveEvents(this, EVENT_SERVER_SHUT_DOWN);
}

void CGameProcessor::OnEvent_ServerShutdown()
{
	/*
	*	OnEvent_ServerShutdown is always called multiple times until no more async queries in queue. On first call we dc everyone.
		Second we let all queries finish and then shut down the server to avoid people lose items etc.
	*/

	g_pObjectManager->DisconnectAll();
}
