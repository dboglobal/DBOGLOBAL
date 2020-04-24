#include "stdafx.h"
#include "ScriptAlgoAction_ChangeNpcAttribute.h"
#include "WPSNodeAction_ChangeNpcAttribute.h"


CWpsAlgoAction_ChangeNpcAttribute::CWpsAlgoAction_ChangeNpcAttribute(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CHANGE_NPC_ATTRIBUTE, "SCRIPTCONTROL_ACTION_CHANGE_NPC_ATTRIBUTE")
{
}


CWpsAlgoAction_ChangeNpcAttribute::~CWpsAlgoAction_ChangeNpcAttribute()
{
}


bool CWpsAlgoAction_ChangeNpcAttribute::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_ChangeNpcAttribute* pAction = dynamic_cast<CWPSNodeAction_ChangeNpcAttribute*>(pControlScriptNode);
	if (pAction)
	{
		m_targetTblidx = pAction->m_targetTblidx;
		m_bApplyRelationPc = pAction->m_bApplyRelationPc;
		m_eRelationPc = pAction->m_eRelationPc;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_ChangeNpcAttribute", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_ChangeNpcAttribute::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
