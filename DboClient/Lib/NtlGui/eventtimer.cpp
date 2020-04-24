#include "gui_precomp.h"
#include "eventtimer.h"

std::list<CEventTimerSlot*> CEventTimer::m_CallbackTimer;
std::list<CTimerHandle> CEventTimer::m_RemoveTimer;