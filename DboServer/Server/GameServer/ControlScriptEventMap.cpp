#include "stdafx.h"
#include "ControlScriptEventMap.h"


CControlScriptEventMap::CControlScriptEventMap()
{
}

CControlScriptEventMap::~CControlScriptEventMap()
{
}



void CControlScriptEventMap::SetScriptEvent(DWORD eventId)
{
	SCRIPTEVENT_MAP::iterator it = m_mapScriptEvent.find(eventId);
	if (it == m_mapScriptEvent.end())
	{
		sEVENT_DATA sEventData(eventId, 0);
		m_mapScriptEvent.insert(SCRIPTEVENT_VAL(eventId, sEventData));
	}
}


bool CControlScriptEventMap::IsReceived(DWORD eventId)
{
	SCRIPTEVENT_MAP::iterator it = m_mapScriptEvent.find(eventId);
	if (it != m_mapScriptEvent.end())
	{
		//printf("it->second.wCount: %u \n", it->second.wCount);
		if (it->second.wCount > 0)
			return true;
	}

	return false;
}

bool CControlScriptEventMap::IsReceived(DWORD eventId, WORD wCount)
{
	SCRIPTEVENT_MAP::iterator it = m_mapScriptEvent.find(eventId);
	if (it != m_mapScriptEvent.end())
	{
		if (it->second.wCount >= wCount)
			return true;
	}

	return false;
}

void CControlScriptEventMap::Clear()
{
	m_mapScriptEvent.clear();
}

void CControlScriptEventMap::ClearScriptEvent(DWORD eventId)
{
	SCRIPTEVENT_MAP::iterator it = m_mapScriptEvent.find(eventId);
	if (it != m_mapScriptEvent.end())
	{
		m_mapScriptEvent.erase(it);
	}
}

void CControlScriptEventMap::OnEventReceived(DWORD eventId)
{
	SCRIPTEVENT_MAP::iterator it = m_mapScriptEvent.find(eventId);
	if (it == m_mapScriptEvent.end())
	{
		sEVENT_DATA sEventData(eventId, 1);
		m_mapScriptEvent.insert(SCRIPTEVENT_VAL(eventId, sEventData));
	}
	else
	{
		OnUpdateEventStatus(it->second.keyEventStatus, ++it->second.wCount);
	}
}


void CControlScriptEventMap::OnUpdateEventStatus(DWORD keyEventStatus, WORD wStatus)
{
}



