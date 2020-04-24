#include "stdafx.h"
#include "EndCondition.h"


CEndCondition::CEndCondition(CNpc* pBot, const char* lpszControlName)
{
	if (pBot)
	{
		m_pBot = pBot;
		m_lpszEndConditionName = lpszControlName;
	}
	else
		ERR_LOG(LOG_BOTAI, "fail : NULL == m_pBot. ControlName %s", lpszControlName);
}

CEndCondition::~CEndCondition()
{
}


