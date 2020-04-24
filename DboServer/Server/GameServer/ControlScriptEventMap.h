#ifndef __SERVER_CONTROLSCRIPT_EVENTMAP__
#define __SERVER_CONTROLSCRIPT_EVENTMAP__

#include "NtlSharedType.h"
#include <map>

class CControlScriptEventMap
{
	struct sEVENT_DATA
	{
		sEVENT_DATA::sEVENT_DATA(DWORD _eventId, WORD _wCount)
		{
			eventId = _eventId;
			wCount = _wCount;
			keyEventStatus = INVALID_DWORD;
		}

		DWORD eventId;
		WORD wCount;
		DWORD keyEventStatus;
	};

	typedef std::map<DWORD, sEVENT_DATA> SCRIPTEVENT_MAP;
	typedef SCRIPTEVENT_MAP::value_type SCRIPTEVENT_VAL;

public:
	CControlScriptEventMap();
	virtual ~CControlScriptEventMap();


public:

	void SetScriptEvent(DWORD eventId);
	bool IsReceived(DWORD eventId);
	bool IsReceived(DWORD eventId, WORD wCount);
	void Clear();
	void ClearScriptEvent(DWORD eventId);
	void OnEventReceived(DWORD eventId);


protected:

	virtual void OnUpdateEventStatus(DWORD keyEventStatus, WORD wStatus);


private:

	SCRIPTEVENT_MAP m_mapScriptEvent;

};


#endif