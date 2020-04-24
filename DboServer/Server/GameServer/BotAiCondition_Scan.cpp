#include "stdafx.h"
#include "BotAiCondition_Scan.h"
#include "AISNodeCondition_Scan.h"
#include "AIScriptControlFactoy.h"


CBotAiCondition_Scan::CBotAiCondition_Scan(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_SCAN, "BOTCONTROL_CONDITION_SCAN")
{
	m_eObjType = INVALID_OBJTYPE;
	m_tblidx = INVALID_TBLIDX;
	m_wRange = 255;
	m_dwTime = 0;
	m_scriptnodeID = INVALID_DWORD;
}

CBotAiCondition_Scan::~CBotAiCondition_Scan()
{
}

bool CBotAiCondition_Scan::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CAISNodeCondition_Scan* pCond = dynamic_cast<CAISNodeCondition_Scan*>(pControlScriptNode);
	if (pCond)
	{
		m_eObjType = pCond->m_eObjType;
		m_tblidx = pCond->m_tblidx;
		m_wRange = pCond->m_wRange;
		m_scriptnodeID = pControlScriptNode->GetNodeIndex();

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CAISNodeCondition_Scan", pControlScriptNode);
	return false;
}

void CBotAiCondition_Scan::CopyTo(CControlState* pTo)
{
	CBotAiCondition_Scan* pCond = dynamic_cast<CBotAiCondition_Scan*>(pTo);
	if (pCond)
	{
		pCond->m_eObjType = m_eObjType;
		pCond->m_tblidx = m_tblidx;
		pCond->m_wRange = m_wRange;
		pCond->m_dwTime = m_dwTime;
		pCond->m_scriptnodeID = m_scriptnodeID;
	}
}

int CBotAiCondition_Scan::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);
	if (m_dwTime < 1000)
		return m_status;

	m_dwTime = 0;

	if (m_eObjType == OBJTYPE_PC)
	{
		if (GetBot()->ConsiderScanTarget(m_wRange) == INVALID_HOBJECT)
			return m_status;
	}
	else if (m_eObjType == OBJTYPE_NPC || m_eObjType == OBJTYPE_MOB)
	{
		if (GetBot()->ConsiderScanObject(m_tblidx, m_wRange) == INVALID_HOBJECT)
			return m_status;
	}
	else
	{
		m_status = FAILED;
		return m_status;
	}

	CAIScriptControlFactoy mFactory(GetBot());

	if (!GetBot()->GetControlStateManager()->CopySubControl(m_scriptnodeID, this, (CComplexState *)GetBot()->GetBotController()->GetCurrentState(), &mFactory))
	{
		m_status = FAILED;
		return m_status;
	}

	return m_status;
}
