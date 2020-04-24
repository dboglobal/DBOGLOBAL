#include "stdafx.h"
#include "ScriptAlgoAction_ChangeObjectState.h"
#include "WPSNodeAction_ChangeObjectState.h"
#include "TriggerObject.h"
#include "NtlPacketGU.h"
#include "CPlayer.h"


CWpsAlgoAction_ChangeObjectState::CWpsAlgoAction_ChangeObjectState(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CHANGE_OBJECT_STATE, "SCRIPTCONTROL_ACTION_CHANGE_OBJECT_STATE")
{
}


CWpsAlgoAction_ChangeObjectState::~CWpsAlgoAction_ChangeObjectState()
{
}


bool CWpsAlgoAction_ChangeObjectState::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_ChangeObjectState* pAction = dynamic_cast<CWPSNodeAction_ChangeObjectState*>(pControlScriptNode);
	if (pAction)
	{
		m_objectTblidx = pAction->m_objectTblidx;
		m_mainState = pAction->m_mainState;
		m_subStateSet = pAction->m_subStateSet;
		m_subStateUnset = pAction->m_subStateUnset;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_ChangeNpcAttribute", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_ChangeObjectState::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetOwner()->GetWorld() == NULL)
	{
		ERR_LOG(LOG_SCRIPT, "Has no world. Script ID %u", GetOwner()->GetScriptID());
		m_status = SYSTEMERROR;
		return m_status;
	}

	CTriggerObject* pObj = GetOwner()->GetWorld()->FindStaticObjectByIdx(m_objectTblidx);
	if (pObj == NULL)
	{
		ERR_LOG(LOG_SCRIPT, "World %u has no trigger object %u. Script ID %u", GetOwner()->GetWorld()->GetIdx(), m_objectTblidx, GetOwner()->GetScriptID());
		m_status = SYSTEMERROR;
		return m_status;
	}

	//printf("obj %u change m_mainState %u m_subStateUnset %u m_subStateSet %u. Script %u \n", m_objectTblidx, m_mainState, m_subStateUnset, m_subStateSet, GetOwner()->GetScriptID());

	BYTE byNewSubState = pObj->GetSubState();

	if (m_subStateUnset)
	{
		for (int i = 0; i < MAX_TOBJECT_SUBSTATE; i++)
		{
			BYTE byFlag = MAKE_BIT_FLAG(i);
			if (BIT_FLAG_TEST(m_subStateUnset, byFlag)) //check if flag set
			{
				if (BIT_FLAG_TEST(byNewSubState, byFlag)) //check if obj have flag
				{
					BIT_FLAG_UNSET(byNewSubState, byFlag); //remove flag
				}
			}
		}
	}

	if(m_subStateSet)
	{
		for (int i = 0; i < MAX_TOBJECT_SUBSTATE; i++)
		{
			BYTE byFlag = MAKE_BIT_FLAG(i);
			if (BIT_FLAG_TEST(m_subStateSet, byFlag)) //check if flag set
			{
				if (BIT_FLAG_TEST(byNewSubState, byFlag) == false) //check if obj dont have flag
				{
					BIT_FLAG_SET(byNewSubState, byFlag); //add flag
				}
			}
		}
	}

	BYTE byMainState = pObj->GetMainState();
	if (m_mainState != INVALID_BYTE)
	{
		byMainState = m_mainState;
	}

	pObj->UpdateToObjectState(byMainState, byNewSubState, GetTickCount());
	m_status = COMPLETED;

	return m_status;
}
