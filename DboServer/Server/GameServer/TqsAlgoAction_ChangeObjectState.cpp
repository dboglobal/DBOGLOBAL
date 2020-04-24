#include "stdafx.h"
#include "TqsAlgoAction_ChangeObjectState.h"
#include "TQSNodeAction_ChangeObjectState.h"
#include "TriggerObject.h"
#include "NtlPacketGU.h"
#include "World.h"


CTqsAlgoAction_ChangeObjectState::CTqsAlgoAction_ChangeObjectState(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_CHANGE_OBJECT_STATE, "TQS_ALGOCONTROLID_ACTION_CHANGE_OBJECT_STATE")
{
}


CTqsAlgoAction_ChangeObjectState::~CTqsAlgoAction_ChangeObjectState()
{
}


bool CTqsAlgoAction_ChangeObjectState::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_ChangeObjectState* pAction = dynamic_cast<CTQSNodeAction_ChangeObjectState*>(pControlScriptNode);
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

int CTqsAlgoAction_ChangeObjectState::OnUpdate(DWORD dwTickDiff, float fMultiple)
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
