#include "stdafx.h"
#include "BotAiBase.h"


CBotAiBase::CBotAiBase(CNpc* pBot, eBOTCONTROLID controlID, const char* lpszControlName)
:CComplexState(controlID, lpszControlName)
{
	m_pBotNpc = pBot;
}

CBotAiBase::~CBotAiBase()
{
}


bool CBotAiBase::AddSubControlQueue(CBotAiBase* pAiBase, bool bFront)
{
	if (m_subControlQueue.GetFirst())
	{
		((CComplexState*)m_subControlQueue.GetFirst())->Pause();
	}

	return CComplexState::AddSubControlQueue(pAiBase, bFront);
}

