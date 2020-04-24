#include "stdafx.h"
#include "DragonballHunt.h"
#include "GameServer.h"
#include "NtlPacketGU.h"
#include "NtlAdmin.h"
#include "NtlPacketGT.h"
#include "NtlStringW.h"
#include "Monster.h"


CDragonballHunt::CDragonballHunt()
{
	m_dwNextUpdateTick = GetTickCount();

	Init();
}


CDragonballHunt::~CDragonballHunt()
{
	Destroy();
}


void CDragonballHunt::Init()
{
	m_bOnOff = false;
	m_timeStart = INVALID_DBOTIME;
	m_timeEnd = INVALID_DBOTIME;
	m_dwDragonballDropCount = 0;
}


void CDragonballHunt::Destroy()
{
}


void CDragonballHunt::Create()
{
}


void CDragonballHunt::TickProcess(DWORD dwTick)
{
	if (dwTick < m_dwNextUpdateTick)
		return;

	CGameServer* app = (CGameServer*)g_pApp;

	DBOTIME curTime = app->GetTime();

	if (m_bOnOff == false)
	{
		tm timeStruct = {};
		localtime_s(&timeStruct, &curTime);

		switch (timeStruct.tm_wday)
		{
			case 0: //sunday
			{
				if (timeStruct.tm_hour >= 8 && timeStruct.tm_hour <= 11)
				{
					m_bOnOff = true;
					m_timeStart = curTime;

					tm timeEndStruct = timeStruct;
					timeEndStruct.tm_sec = 0;
					timeEndStruct.tm_min = 0;
					timeEndStruct.tm_hour = 12;

					m_timeEnd = mktime(&timeEndStruct);

					StartEvent(false);
				}
			}
			break;
			case 1: //monday
			{
				if (timeStruct.tm_hour >= 12 && timeStruct.tm_hour <= 15)
				{
					m_bOnOff = true;
					m_timeStart = curTime;

					tm timeEndStruct = timeStruct;
					timeEndStruct.tm_sec = 0;
					timeEndStruct.tm_min = 0;
					timeEndStruct.tm_hour = 16;

					m_timeEnd = mktime(&timeEndStruct);

					StartEvent(false);
				}
			}
			break;
			case 2: //tuesday
			{
				if (timeStruct.tm_hour >= 16 && timeStruct.tm_hour <= 19)
				{
					m_bOnOff = true;
					m_timeStart = curTime;

					tm timeEndStruct = timeStruct;
					timeEndStruct.tm_sec = 0;
					timeEndStruct.tm_min = 0;
					timeEndStruct.tm_hour = 20;

					m_timeEnd = mktime(&timeEndStruct);

					StartEvent(false);
				}
			}
			break;
			case 3: //wednesday
			{
				if (timeStruct.tm_hour >= 20 && timeStruct.tm_hour <= 23)
				{
					m_bOnOff = true;
					m_timeStart = curTime;

					tm timeEndStruct = timeStruct;
					timeEndStruct.tm_sec = 59;
					timeEndStruct.tm_min = 59;
					timeEndStruct.tm_hour = 23;

					m_timeEnd = mktime(&timeEndStruct);

					StartEvent(false);
				}
			}
			break;
			case 5: //friday
			{
				if (timeStruct.tm_hour >= 0 && timeStruct.tm_hour <= 3)
				{
					m_bOnOff = true;
					m_timeStart = curTime;

					tm timeEndStruct = timeStruct;
					timeEndStruct.tm_sec = 0;
					timeEndStruct.tm_min = 0;
					timeEndStruct.tm_hour = 4;

					m_timeEnd = mktime(&timeEndStruct);

					StartEvent(false);
				}
			}
			break;
			case 6: //saturday
			{
				if (timeStruct.tm_hour >= 4 && timeStruct.tm_hour <= 7)
				{
					m_bOnOff = true;
					m_timeStart = curTime;

					tm timeEndStruct = timeStruct;
					timeEndStruct.tm_sec = 0;
					timeEndStruct.tm_min = 0;
					timeEndStruct.tm_hour = 8;

					m_timeEnd = mktime(&timeEndStruct);

					StartEvent(false);
				}
			}
			break;

			default: break;
		}
	}
	else
	{
		if (curTime >= m_timeEnd)
		{
			EndEvent();
		}
	}

	m_dwNextUpdateTick = dwTick + 300000; //update again in 5 minutes
}


void CDragonballHunt::StartEvent(bool bStartByCommand/* = true*/)
{
	if (bStartByCommand)
	{
		if (m_bOnOff)
			return;

		CGameServer* app = (CGameServer*)g_pApp;

		m_bOnOff = true;
		m_timeStart = app->GetTime();
		m_timeEnd = m_timeStart + 7200;
	}

	ERR_LOG(LOG_GENERAL, "<Dragonball Hunt Event> Start %I64u, End %I64u, Duration in seconds %I64u", m_timeStart, m_timeEnd, m_timeEnd - m_timeStart);

	CNtlPacket packet(sizeof(sGU_DRAGONBALL_SCHEDULE_INFO));
	sGU_DRAGONBALL_SCHEDULE_INFO * res = (sGU_DRAGONBALL_SCHEDULE_INFO *)packet.GetPacketData();
	res->wOpCode = GU_DRAGONBALL_SCHEDULE_INFO;
	res->bIsAlive = true;
	res->byEventType = SCHEDULE_EVENT_TYPE_EVENT_DRAGONBALL;
	res->nStartTime = m_timeStart;
	res->nEndTime = m_timeEnd;
	g_pObjectManager->SendPacketToAll(&packet);

	WCHAR* wMsg = L"Dragonball Hunt Event Started!";

	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packetMsg(sizeof(sGT_SYSTEM_DISPLAY_TEXT));
	sGT_SYSTEM_DISPLAY_TEXT * resMsg = (sGT_SYSTEM_DISPLAY_TEXT*)packetMsg.GetPacketData();
	resMsg->wOpCode = GT_SYSTEM_DISPLAY_TEXT;
	resMsg->serverChannelId = app->GetGsChannel();
	resMsg->byDisplayType = SERVER_TEXT_EMERGENCY;
	NTL_SAFE_WCSCPY(resMsg->wszMessage, wMsg);
	packetMsg.SetPacketLen(sizeof(sGT_SYSTEM_DISPLAY_TEXT));
	app->SendTo(app->GetChatServerSession(), &packetMsg);
}


void CDragonballHunt::EndEvent()
{
	if (m_bOnOff == false)
		return;

	ERR_LOG(LOG_GENERAL, "<Dragonball Hunt Event> End. Total Dragonballs dropped: %u", m_dwDragonballDropCount);

	CNtlStringW msg;
	msg.Format(L"Dragonball Hunt Event End! Dragonballs dropped: %u", m_dwDragonballDropCount);

	Init();

	std::set<HOBJECT>::iterator it;
	while (m_setMonsters.size() > 0)
	{
		it = m_setMonsters.begin();

		CMonster* pMob = g_pObjectManager->GetMob(*it);
		if (pMob)
		{
			pMob->ResetDragonball();
		}

		m_setMonsters.erase(it);
	}

	CNtlPacket packet(sizeof(sGU_DRAGONBALL_SCHEDULE_INFO));
	sGU_DRAGONBALL_SCHEDULE_INFO * res = (sGU_DRAGONBALL_SCHEDULE_INFO *)packet.GetPacketData();
	res->wOpCode = GU_DRAGONBALL_SCHEDULE_INFO;
	res->bIsAlive = false;
	res->byEventType = SCHEDULE_EVENT_TYPE_EVENT_DRAGONBALL;
	g_pObjectManager->SendPacketToAll(&packet);

	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packetMsg(sizeof(sGT_SYSTEM_DISPLAY_TEXT));
	sGT_SYSTEM_DISPLAY_TEXT * resMsg = (sGT_SYSTEM_DISPLAY_TEXT*)packetMsg.GetPacketData();
	resMsg->wOpCode = GT_SYSTEM_DISPLAY_TEXT;
	resMsg->serverChannelId = app->GetGsChannel();
	resMsg->byDisplayType = SERVER_TEXT_EMERGENCY;
	NTL_SAFE_WCSCPY(resMsg->wszMessage, msg.c_str());
	packetMsg.SetPacketLen(sizeof(sGT_SYSTEM_DISPLAY_TEXT));
	app->SendTo(app->GetChatServerSession(), &packetMsg);
}

void CDragonballHunt::LoadEvent(CGameObject * pChar)
{
	if (m_bOnOff == false)
		return;

	CNtlPacket packet(sizeof(sGU_DRAGONBALL_SCHEDULE_INFO));
	sGU_DRAGONBALL_SCHEDULE_INFO * res = (sGU_DRAGONBALL_SCHEDULE_INFO *)packet.GetPacketData();
	res->wOpCode = GU_DRAGONBALL_SCHEDULE_INFO;
	res->bIsAlive = m_bOnOff;
	res->byEventType = SCHEDULE_EVENT_TYPE_EVENT_DRAGONBALL;
	res->nStartTime = m_timeStart;
	res->nEndTime = m_timeEnd;
	pChar->SendPacket(&packet);
}

