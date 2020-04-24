#include "stdafx.h"
#include "ExpEvent.h"
#include "GameObject.h"
#include "GameServer.h"
#include "NtlPacketGU.h"
#include "ObjectManager.h"
#include "NtlAdmin.h"
#include "NtlPacketGT.h"
#include "NtlStringW.h"


CExpEvent::CExpEvent()
{
	Init();
}


CExpEvent::~CExpEvent()
{
	Destroy();
}


void CExpEvent::Init()
{
	m_bOnOff = false;
	m_dwWaitTickCount = 10000;
}


void CExpEvent::Destroy()
{
}


void CExpEvent::Create()
{
}


void CExpEvent::TickProcess(DWORD dwTickDiff)
{
	m_dwWaitTickCount = UnsignedSafeDecrease<DWORD>(m_dwWaitTickCount, dwTickDiff);
	if (m_dwWaitTickCount > 0)
		return;

	CGameServer* app = (CGameServer*)g_pApp;

	DBOTIME curTime = app->GetTime();

	tm timeStruct = {};
	localtime_s(&timeStruct, &curTime);

	if (m_bOnOff == false)
	{
		if (timeStruct.tm_wday == 6 || timeStruct.tm_wday == 0) //check if its saturday or sunday -> start event
		{
			m_bOnOff = true;

			StartEvent(false);
		}
	}
	else
	{
		if (timeStruct.tm_wday == 1) // check if its monday -> end event
		{
			EndEvent();
		}
	}

	m_dwWaitTickCount = 300000; //update again in 5 minutes
}


void CExpEvent::StartEvent(bool bStartByCommand/* = true*/)
{
	CGameServer* app = (CGameServer*)g_pApp;

	ERR_LOG(LOG_GENERAL, "<Weekend EXP Event> Started");

	CNtlStringW msg;

	CNtlPacket packet(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT * res = (sGU_SYSTEM_DISPLAY_TEXT*)packet.GetPacketData();
	res->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	res->wMessageLengthInUnicode = (WORD)msg.Format(L"Weekend EXP Event Started! Bonus Exp = %i Percent", EXP_BONUS);
	res->byDisplayType = SERVER_TEXT_EMERGENCY;
	NTL_SAFE_WCSCPY(res->awchMessage, msg.c_str());
	packet.SetPacketLen(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	g_pObjectManager->SendPacketToAll(&packet);
}


void CExpEvent::EndEvent()
{
	if (m_bOnOff == false)
		return;

	CGameServer* app = (CGameServer*)g_pApp;

	ERR_LOG(LOG_GENERAL, "<Weekend EXP Event> End.");

	WCHAR* wMsg = L"Weekend EXP Event End!";

	Init();

	CNtlPacket packet(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT * res = (sGU_SYSTEM_DISPLAY_TEXT*)packet.GetPacketData();
	res->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	res->wMessageLengthInUnicode = (WORD)wcslen(wMsg);
	res->byDisplayType = SERVER_TEXT_EMERGENCY;
	NTL_SAFE_WCSCPY(res->awchMessage, wMsg);
	packet.SetPacketLen(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	g_pObjectManager->SendPacketToAll(&packet);
}

void CExpEvent::LoadEvent(CGameObject * pChar)
{
	if (m_bOnOff == false)
		return;

	CNtlStringW msg;

	CNtlPacket packetMsg(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT * resMsg = (sGU_SYSTEM_DISPLAY_TEXT *)packetMsg.GetPacketData();
	resMsg->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	resMsg->byDisplayType = SERVER_TEXT_EMERGENCY;
	resMsg->wMessageLengthInUnicode = (WORD)msg.Format(L"Weekend EXP Event is Live! Bonus Exp = %i Percent", EXP_BONUS);
	NTL_SAFE_WCSCPY(resMsg->awchMessage, msg.c_str());
	pChar->SendPacket(&packetMsg);
}

float CExpEvent::GetExpBonus()
{
	if (m_bOnOff)
		return (float)EXP_BONUS;

	return 0.0f;
}

