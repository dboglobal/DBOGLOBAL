#include "stdafx.h"
#include "TQSNodeAction_ChangeObjectState.h"
#include "ControlScriptNodeParam_Number.h"
#include "NtlQuest.h"


CTQSNodeAction_ChangeObjectState::CTQSNodeAction_ChangeObjectState(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_CHANGE_OBJECT_STATE, lpszName)
{
	Init();
}

CTQSNodeAction_ChangeObjectState::~CTQSNodeAction_ChangeObjectState()
{
}

CTQSNodeAction_ChangeObjectState::sPARAMETER_INFO* CTQSNodeAction_ChangeObjectState::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_ChangeObjectState::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "index") == 0)
	{
		m_objectTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "state") == 0)
	{
		m_mainState = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "substate show") == 0)
	{
		if (pNode->GetValue() == 0.0f)
			BIT_FLAG_SET(m_subStateUnset, TOBJECT_SUBSTATE_FLAG_SHOW);
		else
			BIT_FLAG_SET(m_subStateSet, TOBJECT_SUBSTATE_FLAG_SHOW);

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "substate unselect") == 0)
	{
		if (pNode->GetValue() == 0.0f)
			BIT_FLAG_SET(m_subStateUnset, TOBJECT_SUBSTATE_FLAG_UNSELECT);
		else
			BIT_FLAG_SET(m_subStateSet, TOBJECT_SUBSTATE_FLAG_UNSELECT);

		return CControlScriptNode::AddParam(pNode);
	}


	printf("CTQSNodeAction_ChangeObjectState: [%s] not exist \n", name);
	return false;
}

void CTQSNodeAction_ChangeObjectState::Init()
{
	m_objectTblidx = INVALID_TBLIDX;
	m_mainState = TOBJECT_STATE_NONE;
	m_subStateSet = 0;
	m_subStateUnset = 0;
}
