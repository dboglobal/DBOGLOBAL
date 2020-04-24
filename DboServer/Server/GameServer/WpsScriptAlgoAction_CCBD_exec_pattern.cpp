#include "stdafx.h"
#include "WpsScriptAlgoAction_CCBD_exec_pattern.h"
#include "WpsNodeAction_CCBD_exec_pattern.h"
#include "WpsAlgoStage.h"
#include "NtlRandom.h"
#include "WpsScriptAlgoCondition_CCBD_pattern.h"



CWpsScriptAlgoAction_CCBD_exec_pattern::CWpsScriptAlgoAction_CCBD_exec_pattern(CWpsAlgoObject* pObject) :
	CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CCBD_EXEC_PATTERN, "SCRIPTCONTROL_ACTION_CCBD_EXEC_PATTERN")
{
	m_bySelectedPattern = INVALID_BYTE;
	m_dwWait = 0;
}


CWpsScriptAlgoAction_CCBD_exec_pattern::~CWpsScriptAlgoAction_CCBD_exec_pattern()
{
}


bool CWpsScriptAlgoAction_CCBD_exec_pattern::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWpsNodeAction_CCBD_exec_pattern* pAction = dynamic_cast<CWpsNodeAction_CCBD_exec_pattern*>(pControlScriptNode);
	if (pAction)
	{
		std::vector<CNtlVector>::iterator it = pAction->m_vecPatterns.begin();

		std::vector<BYTE> randPattern;

		for(std::vector<CNtlVector>::iterator it = pAction->m_vecPatterns.begin(); it != pAction->m_vecPatterns.end(); it++)
		{
			CNtlVector & rVec = *it;

			if (Dbo_CheckProbabilityF(rVec.y))
			{
				randPattern.push_back((BYTE)rVec.x);
			}
		}

		if (randPattern.size() <= 1 )
			m_bySelectedPattern = (BYTE)pAction->m_vecPatterns[0].x;
		else
		{
			m_bySelectedPattern = randPattern[RandomRange(0, (int)(randPattern.size() - 1))];
		}

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWpsNodeAction_CCBD_exec_pattern", pControlScriptNode);
	return false;
}

int CWpsScriptAlgoAction_CCBD_exec_pattern::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_bySelectedPattern == INVALID_BYTE)
	{
		printf("m_bySelectedPattern == INVALID_BYTE \n");
		m_status = FAILED;
		return m_status;
	}

	if (m_dwWait > 0)
	{
		m_dwWait = UnsignedSafeDecrease<DWORD>(m_dwWait, dwTickDiff);

		//When finish normal stage then wait 5 seconds and teleport to standard loc

		if (m_dwWait == 0)
		{
			m_status = COMPLETED;
		}
	}
	else
	{
		CWpsAlgoStage* pStage = dynamic_cast<CWpsAlgoStage*>(GetOwner()->GetController()->GetCurrentStage());
		if (pStage)
		{
			CWpsScriptAlgoCondition_CCBD_pattern* pPattern = (CWpsScriptAlgoCondition_CCBD_pattern*)pStage->GetCCBDPattern(m_bySelectedPattern);

			if (pPattern)
			{
				if (pPattern->Update(dwTickDiff, fMultiple) == COMPLETED)
				{
					m_dwWait = 5000;//wait 5 seconds when finish NON-BOSS stage
				}
			}
			else
			{
				printf("pPattern == NULL \n");
				m_status = FAILED;
				return m_status;
			}
		}
		else
		{
			printf("GetOwner()->GetController()->GetCurrentStage() != CWpsAlgoStage \n");
			m_status = FAILED;
			return m_status;
		}
	}

	return m_status;
}

