#include "stdafx.h"
#include "BotAiController.h"

#include "BotAiState_Enter.h"
#include "BotAiState_Spawn.h"
#include "BotAiState_Idle.h"
#include "BotAiState_Despawn.h"
#include "BotAiState_Return.h"
#include "BotAiState_Fight.h"
#include "BotAiState_BattleScene.h"
#include "BotAiState_Teleporting.h"
#include "BotAiState_Leave.h"
#include "BotAiState_Faint.h"


CBotAiController::CBotAiController()
{
	m_dwSumDelayTick = 0;
	Init();
}

CBotAiController::~CBotAiController()
{
	Destroy();
}


bool CBotAiController::Create(CNpc* pBot)
{
	m_pControlObject = pBot;
	return m_cCheckupEndCondition.Create(pBot);
}

void CBotAiController::Destroy()
{
	SAFE_DELETE(m_pNextAiState);
	DestroyAllControlState();

	m_cCheckupEndCondition.Destroy();
}


void CBotAiController::Init()
{
	m_pControlObject = NULL;
	m_bControlBlock = false;
	m_pNextAiState = NULL;
}


int CBotAiController::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	DWORD dwTickTime = dwTickDiff;

	if (!m_pControlObject->HasNearbyPlayer(false)) //false in npcserver
	{
		m_dwSumDelayTick += dwTickDiff;
		if (m_dwSumDelayTick < 2000)
			return CControlState::ACTIVATED;

		dwTickTime = m_dwSumDelayTick;
		m_dwSumDelayTick = 0;
	}

	if (IsControlStateBlocked())
		return CControlState::INACTIVATED;

	CBotAiState* pCurAiState = NULL;

	GetCheckupEndCondition()->OnUpdate(dwTickTime, fMultiple);

	if (m_pNextAiState)
	{
		pCurAiState = m_pNextAiState;
		m_pNextAiState = NULL;
		ChangeAiState(pCurAiState);
	}
	else if (m_cAddStateList.GetCount())
	{
		pCurAiState = (CBotAiState*)m_controlStateStack.GetFirst();
		if (pCurAiState)
		{
			m_controlStateStack.PopFront();
			pCurAiState->Stop();
			SAFE_DELETE(pCurAiState);

			while (m_cAddStateList.GetCount())
			{
				CBotAiState* pLinkObject = (CBotAiState*)m_cAddStateList.GetFirst();
				m_cAddStateList.PopFront();

				m_controlStateStack.PushFront(pLinkObject);
			}

			pCurAiState = (CBotAiState*)m_controlStateStack.GetFirst();
			if (!pCurAiState)
			{
				ERR_LOG(LOG_BOTAI, "pCurAiState == NULL");
				return CControlState::SYSTEMERROR;
			}

			pCurAiState->Start();
		}
	}

	for (pCurAiState = (CBotAiState *)m_controlStateStack.GetFirst();
		pCurAiState && (pCurAiState->IsStatus(CControlState::COMPLETED) || pCurAiState->IsStatus(CControlState::FAILED));
		pCurAiState = (CBotAiState *)m_controlStateStack.GetFirst())
	{
		pCurAiState->Stop();
		m_controlStateStack.Remove(pCurAiState);
		SAFE_DELETE(pCurAiState);
	}

	if (pCurAiState == NULL)
		return CControlState::COMPLETED;

	pCurAiState->Start(); //printf("start cur ai state %s \n", pCurAiState->GetControlStateName());

	int controlStatus = pCurAiState->GetStatus();
	if (controlStatus == CControlState::FAILED)
	{
		ERR_LOG(LOG_BOTAI, "fail : CControlState::FAILED == controlStatus, pCurAiState->GetControlStateName() = %s", pCurAiState->GetControlStateName());
	}
	else
	{
		controlStatus = pCurAiState->OnUpdate(dwTickTime, fMultiple);
	//	ERR_LOG(LOG_BOTAI, "OnUpdate Botstate %s controlStatus %u Owner(id:%u,tblidx:%u,world:%u)", pCurAiState->GetControlStateName(), controlStatus, GetContolObject()->GetID(), GetContolObject()->GetTblidx(), GetContolObject()->GetWorldTblidx());
	}

	if (controlStatus == CControlState::COMPLETED && m_controlStateStack.GetCount() > 1)
		return CControlState::ACTIVATED;

	if (controlStatus == CControlState::CLEARED)
	{
		pCurAiState->Stop();
		m_controlStateStack.PopFront();
		SAFE_DELETE(pCurAiState);

		while (m_cAddStateList.GetCount())
		{
			CBotAiState* pLinkObject = (CBotAiState*)m_cAddStateList.GetFirst();
			m_cAddStateList.PopFront();
			m_controlStateStack.PushFront(pLinkObject);
		}

		pCurAiState = (CBotAiState*)m_controlStateStack.GetFirst();
		if (!pCurAiState)
		{
			ERR_LOG(LOG_BOTAI, "pCurAiState == NULL");
			return CControlState::SYSTEMERROR;
		}

		pCurAiState->Start();
		return controlStatus;
	}

	if (controlStatus != CControlState::CHANGED)
		return controlStatus;

	if (m_pNextAiState)
	{
		pCurAiState = m_pNextAiState;
		m_pNextAiState = NULL;
		ChangeAiState(pCurAiState);

		return controlStatus;
	}

	if (m_cAddStateList.GetCount() == 0)
		return controlStatus;

	if (pCurAiState->IsStatus(CControlState::COMPLETED) || pCurAiState->IsStatus(CControlState::FAILED))
	{
		pCurAiState->Stop();
		m_controlStateStack.PopFront();
		SAFE_DELETE(pCurAiState);
	}
	else
	{
		pCurAiState->Pause();
	}

	while (m_cAddStateList.GetCount())
	{
		CBotAiState* pLinkObject = (CBotAiState*)m_cAddStateList.GetFirst();
		m_cAddStateList.PopFront();
		m_controlStateStack.PushFront(pLinkObject);
	}

	pCurAiState = (CBotAiState*)m_controlStateStack.GetFirst();
	if (pCurAiState)
	{
		pCurAiState->Start();
		return controlStatus;
	}

	ERR_LOG(LOG_BOTAI, "pCurAiState == NULL");
	return CControlState::SYSTEMERROR;
}


int CBotAiController::OnObjectMsg(CObjectMsg * pObjMsg)
{
	switch (pObjMsg->GetID())
	{
		case OBJMSG_TRIGGER_EVENT:
		{
			CObjMsg_TriggerEvent* pMsg = dynamic_cast<CObjMsg_TriggerEvent*>(pObjMsg);
			if (pMsg)
			{
				GetContolObject()->GetSpsEventMap()->OnEventReceived(pMsg->teid);
				return TRUE;
			}
		}
		break;
		case OBJMSG_TQS_EVENT:
		{
			CObjMsg_TqsEvent* pMsg = dynamic_cast<CObjMsg_TqsEvent*>(pObjMsg);
			if (pMsg)
			{
				GetContolObject()->GetSpsEventMap()->OnEventReceived(pMsg->eventID);
				return TRUE;
			}
		}
		break;
		case OBJMSG_WPS_EVENT:
		{
			CObjMsg_WpsEvent* pMsg = dynamic_cast<CObjMsg_WpsEvent*>(pObjMsg);
			if (pMsg)
			{
				GetContolObject()->GetSpsEventMap()->OnEventReceived(pMsg->eventID);
				return TRUE;
			}
		}
		break;
		case OBJMSG_SEND_WPS_EVENT_TO_SPS:
		{
			CObjMsg_SendWpsEventToSps* pMsg = dynamic_cast<CObjMsg_SendWpsEventToSps*>(pObjMsg);
			if (pMsg)
			{
				GetContolObject()->GetSpsEventMap()->OnEventReceived(pMsg->eventID);
				return TRUE;
			}
		}
		break;
		case OBJMSG_SEND_ITEM_EVENT_TO_SPS:
		case OBJMSG_SEND_WPS_EVENT_TO_AIS:
		{
			for (CControlState* pCurControlState = (CControlState*)m_controlStateStack.GetFirst();
				pCurControlState;
				pCurControlState = (CControlState*)pCurControlState->GetNext())
			{
				pCurControlState->OnObjectMsg(pObjMsg);
				return TRUE;
			}
		}
		break;

		default:
		{
			CControlState* pCurControlState = (CControlState*)m_controlStateStack.GetFirst();
			if (pCurControlState)
				return pCurControlState->OnObjectMsg(pObjMsg);
		}
		break;
	}

	return NTL_SUCCESS;
}


bool CBotAiController::ChangeAiState(CBotAiState* pAiState)
{
	if (pAiState)
	{
		RemoveAllControlState();

		pAiState->Start();
		m_controlStateStack.PushFront(pAiState);
		return true;
	}

	ERR_LOG(LOG_BOTAI, "pAiState == NULL");
	return false;
}

bool CBotAiController::AddAiState(CBotAiState* pAiState)
{
	if (pAiState)
	{
		m_cAddStateList.PushBack(pAiState);
		return true;
	}

	ERR_LOG(LOG_BOTAI, "pAiState == NULL");
	return false;
}

void CBotAiController::RemoveAllControlState()
{
	for (CBotAiState* pLinkObject = (CBotAiState*)m_controlStateStack.GetFirst();
		pLinkObject;
		pLinkObject = (CBotAiState*)m_controlStateStack.GetFirst())
	{
		pLinkObject->Stop();
		m_controlStateStack.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);
	}
}

void CBotAiController::DestroyAllControlState()
{
	for (CNtlLinkObject* pLinkObject = m_controlStateStack.GetFirst();
		pLinkObject;
		pLinkObject = m_controlStateStack.GetFirst())
	{
		m_controlStateStack.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);
	}
}

void CBotAiController::AllStop()
{
	for (CBotAiState* j = (CBotAiState*)m_controlStateStack.GetFirst(); j; j = (CBotAiState*)j->GetNext())
		j->Stop();
}

CBotAiState* CBotAiController::GetCurrentState()
{
	return (CBotAiState*)m_controlStateStack.GetFirst();
}

int CBotAiController::GetCurrentStateID()
{
	int iRes = INVALID_BOTCONTROLID;

	CBotAiState* aiState = (CBotAiState*)m_controlStateStack.GetFirst();
	if (aiState)
		iRes = aiState->GetControlStateID();

	return iRes;
}

bool CBotAiController::IsCurrentStateID(int id)
{
	bool bRes = false;

	CBotAiState* aiState = (CBotAiState*)m_controlStateStack.GetFirst();
	if (aiState)
		bRes = aiState->GetControlStateID() == id;

	return bRes;
}

void CBotAiController::SetControlBlock(bool bBlock)
{
	m_bControlBlock = bBlock;
}

bool CBotAiController::IsControlStateBlocked()
{
	return m_bControlBlock;
}

void CBotAiController::SetNextAiState(CBotAiState* pNextAiState)
{
	if (pNextAiState)
	{
		SAFE_DELETE(m_pNextAiState);

		m_pNextAiState = pNextAiState;
	}
}

bool CBotAiController::IsEmptyNextControlState()
{
	return m_pNextAiState == NULL;
}

bool CBotAiController::ChangeControlState_Idle()
{
	CBotAiState_Idle* pState = new CBotAiState_Idle(GetContolObject());
	if (pState)
	{
		SetNextAiState(pState);
		return true;
	}

	ERR_LOG(LOG_BOTAI, "pState == NULL");
	return false;
}

bool CBotAiController::ChangeControlState_Enter()
{
	CBotAiState_Enter* pState = new CBotAiState_Enter(GetContolObject());
	if (pState)
	{
		SetNextAiState(pState);
		return true;
	}

	ERR_LOG(LOG_BOTAI, "pState == NULL");
	return false;
}

bool CBotAiController::ChangeControlState_Spawn()
{
	CBotAiState_Spawn* pState = new CBotAiState_Spawn(GetContolObject());
	if (pState)
	{
		SetNextAiState(pState);
		return true;
	}

	ERR_LOG(LOG_BOTAI, "pState == NULL");
	return false;
}

bool CBotAiController::ChangeControlState_Leave()
{
	for (CControlState* j = (CControlState*)m_controlStateStack.GetFirst();
		j;
		j = (CControlState*)j->GetNext())
	{
		if (j->GetControlStateID() == BOTCONTROL_LEAVE)
		{
			ERR_LOG(LOG_GENERAL, "j->GetControlStateID() == BOTCONTROL_LEAVE \n");
			return true;
		}
	}

	CBotAiState_Leave* pState = new CBotAiState_Leave(GetContolObject());
	SetNextAiState(pState);
	return true;
}

bool CBotAiController::ChangeControlState_Faint()
{
	CBotAiState_Faint* pState = new CBotAiState_Faint(GetContolObject());
	SetNextAiState(pState);
	return true;
}

bool CBotAiController::ChangeControlState_Despawn()
{
	CBotAiState_Despawn* pDespawn = new CBotAiState_Despawn(GetContolObject());
	SetNextAiState(pDespawn);
	return true;
}

bool CBotAiController::ChangeControlState_Teleporting(WORLDID worldId, TBLIDX worldTblidx, CNtlVector& rvNewLoc, CNtlVector& rvNewDir)
{
	CBotAiState_Teleporting* pState = new CBotAiState_Teleporting(GetContolObject(), worldId, worldTblidx, rvNewLoc, rvNewDir);
	SetNextAiState(pState);
	return true;
}

bool CBotAiController::AddControlState_Fight(HOBJECT hTarget)
{
	if (m_pControlObject->IsNPC() || m_pControlObject->IsMonster())
	{
		CBotAiState_Return* pStateReturn = new CBotAiState_Return(m_pControlObject, BOT_RETURNMODE_FASTRUN, m_pControlObject->GetFirstBattleLoc(), m_pControlObject->GetFirstBattleDir());
		if (!AddAiState(pStateReturn))
			return false;
	}

	CBotAiState_Fight* pStateFight = new CBotAiState_Fight(m_pControlObject, hTarget);
	if (!AddAiState(pStateFight))
		return false;

	return true;
}

bool CBotAiController::AddControlState_BattleScene(TBLIDX tblidxBattleScript, DWORD dwBSFlowNodeIndex)
{
	CBotAiState_BattleScene* pBattleScene = new CBotAiState_BattleScene(GetContolObject(), tblidxBattleScript, dwBSFlowNodeIndex);

	return AddAiState(pBattleScene);
}

CBotAiState_Fight* CBotAiController::GetFightState()
{
	CControlState* pState = (CControlState*)m_controlStateStack.GetFirst();
	if (pState)
	{
		if (pState->GetControlStateID() == BOTCONTROL_FIGHT)
			return (CBotAiState_Fight*)pState;
	}

	return NULL;
}

