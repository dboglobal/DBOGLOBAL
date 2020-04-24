#include "stdafx.h"
#include "BotAiBase_FirstProcList.h"


CBotAiBase_FirstProcList::CBotAiBase_FirstProcList(CNpc* pBot)
:CBotAiBase(pBot, BOTCONTROL_FIRSTPROC_LIST, "BOTCONTROL_FIRSTPROC_LIST")
{
	m_status = ACTIVATED;
}

CBotAiBase_FirstProcList::~CBotAiBase_FirstProcList()
{
}



int CBotAiBase_FirstProcList::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	int status = UpdateSubControlList(dwTickDiff, fMultiple);
	if (status != CHANGED && status != CLEARED)
	{
		status = UpdateSubControlQueue(dwTickDiff, fMultiple);
	}

	return status;
}
