#include "stdafx.h"
#include "ScriptAlgoController.h"

#include "TqsAlgoStage_Stage.h"
#include "TqsAlgoStage_End.h"
#include "TqsAlgoStage_Begin.h"

#include "WpsAlgoStage.h"
#include "WpsAlgoStageFailed.h"
#include "WpsAlgoStagePrepare.h"

#include "ObjectMsg.h"


CScriptAlgoController::CScriptAlgoController()
{
	Init();
}

CScriptAlgoController::~CScriptAlgoController()
{
	Destroy();
}


void CScriptAlgoController::SetOwner(CScriptAlgoObject* pOwner)
{
	m_pOwner = pOwner;
}


void CScriptAlgoController::Init()
{
	m_pOwner = NULL;
	m_pNextStage = NULL;
}


void CScriptAlgoController::Destroy()
{
	SAFE_DELETE(m_pNextStage);
	DestroyAllControlStage();
}


int CScriptAlgoController::OnUpdate(DWORD dwTickDiff)
{
	DWORD dwTickTime = dwTickDiff;

	CControlStateComposite* pCurStage = NULL;

	if (m_pNextStage)
	{
		pCurStage = m_pNextStage;
		m_pNextStage = NULL;
		ChangeStage(pCurStage);
	}
	else if (m_cAddStageList.GetCount() > 0)
	{
		pCurStage = (CControlStateComposite*)m_controlStageStack.GetFirst();
		if (pCurStage)
		{
			m_controlStageStack.PopFront();
			pCurStage->Stop();
			SAFE_DELETE(pCurStage);

			while (m_cAddStageList.GetCount())
			{
				CNtlLinkObject* pLinkObject = m_cAddStageList.GetFirst();
				m_cAddStageList.PopFront();

				m_controlStageStack.PushFront(pLinkObject);
			}

			pCurStage = (CControlStateComposite*)m_controlStageStack.GetFirst();
			if (!pCurStage)
			{
				ERR_LOG(LOG_BOTAI, "pCurStage == NULL");
				return CControlState::SYSTEMERROR;
			}

			pCurStage->Start();
		}
	}

	for (pCurStage = (CControlStateComposite *)m_controlStageStack.GetFirst();
		pCurStage && (pCurStage->IsStatus(CControlState::COMPLETED) || pCurStage->IsStatus(CControlState::FAILED));
		pCurStage = (CControlStateComposite *)m_controlStageStack.GetFirst())
	{
		pCurStage->Stop();
		m_controlStageStack.Remove(pCurStage);
		SAFE_DELETE(pCurStage);
	}

	if (pCurStage == NULL)
		return CControlState::COMPLETED;

	pCurStage->Start();

	int controlStatus = pCurStage->GetStatus();
	if (controlStatus == CControlState::FAILED)
	{
		ERR_LOG(LOG_SCRIPT, "fail : CControlState::FAILED == controlStatus, pCurStage->GetControlStateName() = %s", pCurStage->GetControlStateName());
	}
	else
	{
	//	ERR_LOG(LOG_SCRIPT, "UPDATE STAGE %s %u", pCurStage->GetControlStateName(), pCurStage->GetControlStateID());
		controlStatus = pCurStage->OnUpdate(dwTickTime, 0.0f);
	}

	if (controlStatus == CControlState::COMPLETED && m_controlStageStack.GetCount() > 1)
		return CControlState::ACTIVATED;

	if (controlStatus != CControlState::CHANGED)
		return controlStatus;

	if (m_pNextStage)
	{
		pCurStage = m_pNextStage;
		m_pNextStage = NULL;
		ChangeStage(pCurStage);

		return controlStatus;
	}

	if (m_cAddStageList.GetCount() == 0)
		return controlStatus;

	if (pCurStage->IsStatus(CControlState::COMPLETED) || pCurStage->IsStatus(CControlState::FAILED))
	{
		pCurStage->Stop();
		m_controlStageStack.PopFront();
		SAFE_DELETE(pCurStage);
	}

	while (m_cAddStageList.GetCount())
	{
		CNtlLinkObject* pLinkObject = m_cAddStageList.GetFirst();
		m_cAddStageList.PopFront();
		m_controlStageStack.PushFront(pLinkObject);
	}

	pCurStage = (CControlStateComposite*)m_controlStageStack.GetFirst();
	if (pCurStage)
	{
		pCurStage->Start();
		return controlStatus;
	}

	ERR_LOG(LOG_BOTAI, "pCurStage == NULL");
	return CControlState::SYSTEMERROR;
}


int CScriptAlgoController::OnObjectMsg(CObjectMsg * pObjMsg)
{
	switch (pObjMsg->GetID())
	{
		case OBJMSG_TQS_EVENT:
		{
			CObjMsg_TqsEvent* pMsg = dynamic_cast<CObjMsg_TqsEvent*>(pObjMsg);
			if (pMsg)
			{
				GetContolObject()->GetEventMap()->OnEventReceived((DWORD)pMsg->eventID);
			}
		}
		break;
		case OBJMSG_SEND_TS_EVENT_TO_WPS:
		{
			CObjMsg_SendTsEventToWps* pMsg = dynamic_cast<CObjMsg_SendTsEventToWps*>(pObjMsg);
			if (pMsg)
			{
				GetContolObject()->GetEventMap()->OnEventReceived((DWORD)pMsg->teId);
			}
		}
		break;
		case OBJMSG_SEND_SPS_EVENT_TO_WPS:
		{
			CObjMsg_SendSpsEventToWps* pMsg = dynamic_cast<CObjMsg_SendSpsEventToWps*>(pObjMsg);
			if (pMsg)
			{
				GetContolObject()->GetEventMap()->OnEventReceived(pMsg->eventId);
			}
		}
		break;
		case OBJMSG_SEND_AI_EVENT_TO_WPS:
		{
			CObjMsg_SendAisEventToWps* pMsg = dynamic_cast<CObjMsg_SendAisEventToWps*>(pObjMsg);
			if (pMsg)
			{
				GetContolObject()->GetEventMap()->OnEventReceived(pMsg->eventId);
			}
		}
		break;
		case OBJMSG_SEND_ITEM_EVENT_TO_WPS:
		{
			CObjMsg_SendItemEventToWps* pMsg = dynamic_cast<CObjMsg_SendItemEventToWps*>(pObjMsg);
			if (pMsg)
			{
				GetContolObject()->GetEventMap()->OnEventReceived((DWORD)pMsg->itemTblidx);
			}
		}
		break;
		case OBJMSG_SEND_TQS_EVENT_TO_WPS:
		{
			CObjMsg_SendTqsEventToWps* pMsg = dynamic_cast<CObjMsg_SendTqsEventToWps*>(pObjMsg);
			if (pMsg)
			{
				GetContolObject()->GetEventMap()->OnEventReceived(pMsg->eventId);
			}
		}
		break;
		case OBJMSG_SEND_WPS_EVENT_TO_WPS:
		{
			CObjMsg_SendWpsEventToWps* pMsg = dynamic_cast<CObjMsg_SendWpsEventToWps*>(pObjMsg);
			if (pMsg)
			{
				GetContolObject()->GetEventMap()->OnEventReceived(pMsg->eventId);
			}
		}
		break;
		case OBJMSG_SEND_SPS_EVENT:
		{
			CObjMsg_SendSpsEvent* pMsg = dynamic_cast<CObjMsg_SendSpsEvent*>(pObjMsg);
			if (pMsg)
			{
				GetContolObject()->GetEventMap()->OnEventReceived(pMsg->eventId);
			}
		}
		break;


		default:
		{
			ERR_LOG(LOG_SCRIPT, "MSG ERROR. pObjMsg->GetID() = %u", pObjMsg->GetID());
		}
		break;
	}

	return NTL_SUCCESS;
}


bool CScriptAlgoController::ChangeStage(CControlStateComposite* pStage)
{
	if (pStage)
	{
		RemoveAllControlStage();

		pStage->Start();
		m_controlStageStack.PushFront(pStage);
		return true;
	}

	ERR_LOG(LOG_BOTAI, "pAiStage == NULL");
	return false;
}

bool CScriptAlgoController::AddStage(CControlStateComposite* pStage)
{
	if (pStage)
	{
		m_cAddStageList.PushBack(pStage);
		return true;
	}

	ERR_LOG(LOG_BOTAI, "pAiStage == NULL");
	return false;
}

void CScriptAlgoController::RemoveAllControlStage()
{
	for (CControlStateComposite* pLinkObject = (CControlStateComposite*)m_controlStageStack.GetFirst();
		pLinkObject;
		pLinkObject = (CControlStateComposite*)m_controlStageStack.GetFirst())
	{
		pLinkObject->Stop();
		m_controlStageStack.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);
	}
}

void CScriptAlgoController::DestroyAllControlStage()
{
	for (CNtlLinkObject* pLinkObject = m_controlStageStack.GetFirst();
		pLinkObject;
		pLinkObject = m_controlStageStack.GetFirst())
	{
		m_controlStageStack.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);
	}
}

void CScriptAlgoController::AllStop()
{
	for (CControlStateComposite* j = (CControlStateComposite*)m_controlStageStack.GetFirst(); j; j = (CControlStateComposite*)j->GetNext())
		j->Stop();
}

CControlStateComposite* CScriptAlgoController::GetCurrentStage()
{
	return (CControlStateComposite*)m_controlStageStack.GetFirst();
}

int CScriptAlgoController::GetCurrentStageID()
{
	int iRes = INVALID_SCRIPTCONTROLID;

	CTqsAlgoStage* aiStage = (CTqsAlgoStage*)m_controlStageStack.GetFirst();
	if (aiStage)
		iRes = aiStage->GetControlStateID();

	return iRes;
}

bool CScriptAlgoController::IsCurrentStageID(int id)
{
	bool bRes = false;

	CControlStateComposite* aiStage = (CControlStateComposite*)m_controlStageStack.GetFirst();
	if (aiStage)
		bRes = aiStage->GetControlStateID() == id;

	return bRes;
}


void CScriptAlgoController::SetNextStage(CControlStateComposite* pNextStage)
{
	if (pNextStage)
	{
		SAFE_DELETE(m_pNextStage);

		m_pNextStage = pNextStage;
	}
}

bool CScriptAlgoController::IsEmptyNextControlStage()
{
	return m_pNextStage == NULL;
}


void CScriptAlgoController::ChangeControlStage_Stage(BYTE byStage, eSSD_SCRIPT_TYPE scriptType)
{
	if (scriptType == SSD_SCRIPT_TYPE_TQS)
	{
		CTqsAlgoStage_Stage* pStage = new CTqsAlgoStage_Stage(byStage, (CTqsAlgoObject*)GetContolObject());
		if (pStage)
		{
			SetNextStage(pStage);
			return;
		}
	}
	else if (scriptType == SSD_SCRIPT_TYPE_WPS)
	{
		CWpsAlgoStage* pStage = new CWpsAlgoStage(byStage, (CWpsAlgoObject*)GetContolObject());
		if (pStage)
		{
			SetNextStage(pStage);
			return;
		}
	}

	ERR_LOG(LOG_SCRIPT, "pStage == NULL");
}


void CScriptAlgoController::ChangeControlStage_Begin()
{
	CTqsAlgoStage_Begin* pStage = new CTqsAlgoStage_Begin((CTqsAlgoObject*)GetContolObject());
	if (pStage)
	{
		SetNextStage(pStage);
		return;
	}

	ERR_LOG(LOG_SCRIPT, "pStage == NULL");
}

void CScriptAlgoController::ChangeControlStage_End()
{
	CTqsAlgoStage_End* pStage = new CTqsAlgoStage_End((CTqsAlgoObject*)GetContolObject());
	if (pStage)
	{
		SetNextStage(pStage);
		return;
	}

	ERR_LOG(LOG_SCRIPT, "pStage == NULL");
}




void CScriptAlgoController::ChangeControlStage_Prepare()
{
	CWpsAlgoStagePrepare* pStage = new CWpsAlgoStagePrepare((CWpsAlgoObject*)GetContolObject());
	if (pStage)
	{
		SetNextStage(pStage);
		return;
	}

	ERR_LOG(LOG_SCRIPT, "pStage == NULL");
}

void CScriptAlgoController::ChangeControlStage_Failed()
{
	CWpsAlgoStageFailed* pStage = new CWpsAlgoStageFailed((CWpsAlgoObject*)GetContolObject());
	if (pStage)
	{
		SetNextStage(pStage);
		return;
	}

	ERR_LOG(LOG_SCRIPT, "pStage == NULL");
}