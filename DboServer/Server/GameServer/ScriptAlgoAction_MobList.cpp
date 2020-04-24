#include "stdafx.h"
#include "ScriptAlgoAction_MobList.h"
#include "WPSNodeAction_MobList.h"
#include "WpsAlgoStage.h"


CWpsAlgoAction_MobList::CWpsAlgoAction_MobList(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_MOB_LIST, "SCRIPTCONTROL_ACTION_MOB_LIST")
{
	m_pNode = NULL;
}


CWpsAlgoAction_MobList::~CWpsAlgoAction_MobList()
{
}


bool CWpsAlgoAction_MobList::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_MobList* pAction = dynamic_cast<CWPSNodeAction_MobList*>(pControlScriptNode);
	if (pAction)
	{
		m_nIndex = pAction->m_nIndex - 1;

		m_pNode = pAction;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_MobList", pControlScriptNode);
	return false;
}


void CWpsAlgoAction_MobList::OnEnter()
{
}


int CWpsAlgoAction_MobList::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CWpsAlgoStage* pStage = dynamic_cast<CWpsAlgoStage*>(GetOwner()->GetController()->GetCurrentStage());
	if (pStage)
	{
		pStage->SetMobList(m_pNode, m_nIndex);
	}
	else printf("CWpsAlgoAction_MobList: dynamic_cast m_pParent -> CWpsAlgoStage fail\n");

	m_status = COMPLETED;
	return m_status;
}
