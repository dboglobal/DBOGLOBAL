#include "stdafx.h"
#include "PacketEvent.h"
#include "NtlServerApp.h"
#include "NtlLog.h"
#include "NtlSession.h"


CPacketEvent::CPacketEvent()
{
	Init();
}

CPacketEvent::~CPacketEvent()
{
	Destroy();
}


void CPacketEvent::Init()
{
}


void CPacketEvent::Destroy()
{
	ReleaseEvent();
}


void CPacketEvent::ReleaseEvent()
{
	for (CPacketEventObj* j = (CPacketEventObj*)m_processQueue.PopFront(); j; j = (CPacketEventObj*)m_processQueue.PopFront())
		SAFE_DELETE(j);

	m_eventQueueMutex.Lock();

	for (CPacketEventObj* j = (CPacketEventObj*)m_eventQueue.PopFront(); j; j = (CPacketEventObj*)m_eventQueue.PopFront())
		SAFE_DELETE(j);

	m_eventQueueMutex.Unlock();
}


void CPacketEvent::ProcessEventQueue()
{
	m_eventQueueMutex.Lock();

	m_eventQueue.Move(&m_processQueue);

	m_eventQueueMutex.Unlock();

	ProcessEvent();
}


void CPacketEvent::PostEvent(CPacketEventObj* pEvent)
{
	m_eventQueueMutex.Lock();
	m_eventQueue.PushBack(pEvent);
	m_eventQueueMutex.Unlock();
}

#include "NtlPacketCommon.h"
#include "NtlPacketUtil.h"
void CPacketEvent::ProcessEvent()
{
	LARGE_INTEGER freq, start, end;
	int nPackets = m_processQueue.GetCount();

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	for (CPacketEventObj* pEvent = (CPacketEventObj*)m_processQueue.GetFirst();
		pEvent;
		pEvent = (CPacketEventObj*)m_processQueue.GetFirst())
	{
		//if this dont work fine then use version 2
		if (pEvent->GetSession()->GetUniqueHandle() != INVALID_HSESSION && pEvent->GetUniqueSessionID() == pEvent->GetSession()->GetUniqueHandle())
		{
		//	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pEvent->GetPacket()->GetPacketData();
		//	printf("pEvent->GetUniqueSessionID() %u, %s, wOpCode %u \n", pEvent->GetUniqueSessionID(), NtlGetPacketName(pHeader->wOpCode), pHeader->wOpCode);
			pEvent->Call();
		}

		m_processQueue.PopFront();
		SAFE_DELETE(pEvent);
	}

	QueryPerformanceCounter(&end);

	float fTimeDif = ((float)(end.QuadPart - start.QuadPart)) * 1000.f / ((float)freq.QuadPart);
	if (fTimeDif > 50.0f)
		ERR_LOG(LOG_SYSTEM, "INFO: Processing session packets took longer than 50 ms. Duration (ms) required: %f, packets %u", fTimeDif, nPackets);
}
