#include "stdafx.h"
#include "BotAiAction_Calculate.h"
#include "SPSNodeAction_Calculate.h"
#include "SvrScrFormulaCalc.h"
#include "SvrScrVariableMap.h"


CBotAiAction_Calculate::CBotAiAction_Calculate(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_CALCULATE, "BOTCONTROL_ACTION_CALCULATE")
{
}


CBotAiAction_Calculate::~CBotAiAction_Calculate()
{
}


bool CBotAiAction_Calculate::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_Calculate* pAction = dynamic_cast<CSPSNodeAction_Calculate*>(pControlScriptNode);
	if (pAction)
	{
		m_sDeclareList = pAction->m_sDeclareList;
		m_sFormula = pAction->m_sFormula;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_Calculate", pControlScriptNode);
	return false;
}

void CBotAiAction_Calculate::OnEnter()
{
}

void CBotAiAction_Calculate::OnExit()
{
}


int CBotAiAction_Calculate::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (!m_sDeclareList.size())
	{
		m_status = COMPLETED;
		return m_status;
	}

	CSvrScrVariableMap* pMap = GetValidVariableMap();
	if (pMap)
	{
		for (auto it = m_sDeclareList.begin(); it != m_sDeclareList.end(); it++)
		{
			sSSD_FORMULA_DECLARE& declare = *it;

			if (pMap->Declare(declare.sName.str(), declare.sVariable))
				break;
		}

		if (m_sFormula.vecEntities.size())
		{
			CSvrScrFormulaCalc calc(this);

			if (!calc.Calculate(m_sFormula))
			{
				ERR_LOG(LOG_BOTAI, "calculate fail. ObjType = %u", GetBot()->GetObjType());
				m_status = FAILED;
				return m_status;
			}
		}
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "fail : GetValidVariableMap() == NULL. ObjType = %u", GetBot()->GetObjType());
		m_status = FAILED;
		return m_status;
	}

	m_status = COMPLETED;
	return m_status;
}