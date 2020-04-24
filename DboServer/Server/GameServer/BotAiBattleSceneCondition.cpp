#include "stdafx.h"
#include "BotAiBattleSceneCondition.h"
#include "SPSNodeCondition_BSCondition.h"



CBotAiBattleSceneCondition::CBotAiBattleSceneCondition(CNpc* pBot, eBOTCONTROLID controlID, const char *lpszControlName)
:CBotAiCondition(pBot, controlID, lpszControlName)
{
	m_bOperatorAnd = true;
}

CBotAiBattleSceneCondition::~CBotAiBattleSceneCondition()
{
}


int CBotAiBattleSceneCondition::UpdateSubControlList(DWORD dwTickDiff, float fMultiple)
{
	int nStatus = 9;
	int nLoopCount2 = 0;

	CControlState* pLinkObject = (CControlState*)m_subControlList.GetFirst();
	if (pLinkObject == NULL)
	{
		return nStatus;
	}

	CControlState* pNextState = NULL;
	while (pLinkObject)
	{
		pNextState = (CControlState*)pLinkObject->GetNext();

		pLinkObject->Start();

		if (pLinkObject->IsStatus(ACTIVATED))
		{
			nStatus = pLinkObject->OnUpdate(dwTickDiff, fMultiple);
			if (nStatus == 9)
			{
				if (m_bOperatorAnd == false) //if false then only update one. Otherwise update all
					return nStatus;
			}
			else return nStatus;
		}

		++nLoopCount2;
		if (nLoopCount2 > 100)
		{
			ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
			break;
		}

		pLinkObject = pNextState;
	}

	return nStatus;
}

