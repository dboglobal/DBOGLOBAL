#include "stdafx.h"
#include "EventMgr.h"


TimedEvent * TimedEvent::Allocate(void* object, CallbackBase* callback, unsigned int type, DWORD time, DWORD repeat, unsigned int flags)
{
	return new TimedEvent(object, callback, type, time, repeat, flags);
}
