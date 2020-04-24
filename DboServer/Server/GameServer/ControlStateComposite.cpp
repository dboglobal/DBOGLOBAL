#include "stdafx.h"
#include "ControlAction.h" //#include "ControlStateComposite.h"


CControlStateComposite::CControlStateComposite(int controlStateID, const char* lpszControlStateName)
: CControlState(controlStateID, lpszControlStateName)
{

}

CControlStateComposite::~CControlStateComposite()
{
	//ERR_LOG(LOG_GENERAL, "DESTROY %s %u status %u\n", GetControlStateName(), GetControlStateID(), GetStatus());
	RemoveAllSubControl();
}



int CControlStateComposite::OnUpdate(DWORD dwTickTime, float fMultiple)
{
	UpdateSubControlList(dwTickTime, fMultiple);
	UpdateSubControlQueue(dwTickTime, fMultiple);

	return m_status;
}


int CControlStateComposite::OnObjectMsg(CObjectMsg * objmsg)
{
	int subControlStatus = 0;
	printf("E2X CControlStateComposite::OnObjectMsg \n");
	for (CControlStateComposite* pSubControlState = (CControlStateComposite*)m_subControlList.GetFirst();
		pSubControlState;
		pSubControlState = (CControlStateComposite*)pSubControlState->GetNext())
	{
		if (pSubControlState->IsStatus(COMPLETED) || pSubControlState->IsStatus(FAILED))
		{
			pSubControlState->Stop();
			m_subControlList.Remove(pSubControlState);
			SAFE_DELETE(pSubControlState);
		}
		else
		{
			subControlStatus = pSubControlState->OnObjectMsg(objmsg);
			if (subControlStatus == COMPLETED || subControlStatus == FAILED)
			{
				pSubControlState->Stop();
				m_subControlList.Remove(pSubControlState);
				SAFE_DELETE(pSubControlState);
			}
		}
	}

	CControlStateComposite* pSubControlState = (CControlStateComposite*)m_subControlQueue.GetFirst();
	if (pSubControlState)
	{
		subControlStatus = pSubControlState->OnObjectMsg(objmsg);
		if (subControlStatus == COMPLETED || subControlStatus == FAILED)
		{
			pSubControlState->Stop();
			m_subControlQueue.Remove(pSubControlState);
			SAFE_DELETE(pSubControlState);
		}
	}

	return m_status;
}


bool CControlStateComposite::AddSubControlList(CControlState * pControlState, bool bFront)
{
	if (pControlState)
	{
		if (pControlState->GetPrev() || pControlState->GetNext())
		{
			printf("CControlStateComposite::AddSubControlList:: pControlState->GetPrev() || pControlState->GetNext() \n");
			return false;
		}
		else
		{
			m_subControlList.Push(pControlState, bFront);
			return true;
		}
	}

	printf("CControlStateComposite::AddSubControlList:: pControlState is NULL \n");

	return false;
}

bool CControlStateComposite::AddSubControlQueue(CControlState * pControlState, bool bFront)
{
	if (pControlState)
	{
		if (pControlState->GetPrev() || pControlState->GetNext())
		{
			printf("CControlStateComposite::AddSubControlQueue:: pControlState->GetPrev() || pControlState->GetNext() \n");
			return false;
		}
		else
		{
			m_subControlQueue.Push(pControlState, bFront);
			return true;
		}
	}

	printf("CControlStateComposite::AddSubControlQueue:: pControlState is NULL \n");

	return false;
}

void CControlStateComposite::RemoveAllSubControl()
{
	RemoveAllSubControlList();
	RemoveAllSubControlQueue();
}

void CControlStateComposite::DestroyAllSubControl()
{
	DestroyAllSubControlList();
	DestroyAllSubControlQueue();
}

void CControlStateComposite::AllSubControlStop()
{
	CControlStateComposite * j = NULL;

	//list
	for (j = (CControlStateComposite *)m_subControlList.GetFirst(); j; j = (CControlStateComposite*)j->GetNext())
		j->Stop();
	//queue
	for (j = (CControlStateComposite *)m_subControlQueue.GetFirst(); j; j = (CControlStateComposite*)j->GetNext())
		j->Stop();
}

bool CControlStateComposite::MoveSubControl(CControlStateComposite* pTo)
{
	if (pTo)
	{
		CControlStateComposite * pLinkObject = NULL;

		for (pLinkObject = (CControlStateComposite *)m_subControlQueue.GetFirst();
			pLinkObject;
			pLinkObject = (CControlStateComposite *)m_subControlQueue.GetFirst())
		{
			m_subControlQueue.Remove(pLinkObject);

			if (!pTo->AddSubControlQueue(pLinkObject, true))
				return false;
		}

		pLinkObject = NULL;

		for (pLinkObject = (CControlStateComposite *)m_subControlList.GetFirst();
			pLinkObject;
			pLinkObject = (CControlStateComposite *)m_subControlList.GetFirst())
		{
			m_subControlList.Remove(pLinkObject);

			if (!pTo->AddSubControlList(pLinkObject, true))
				return false;
		}

		return true;
	}

	printf("CControlStateComposite::MoveSubControl: pTo is null \n");

	return false;
}

bool CControlStateComposite::CopySubControl(CControlStateComposite * pTo, CControlStateFactory* pControlFactory)
{
	if (pTo)
	{
		if (pControlFactory)
		{
			CControlStateComposite * j = NULL;

			for (j = (CControlStateComposite *)m_subControlQueue.GetFirst();
				j;
				j = (CControlStateComposite *)j->GetNext())
			{
				CControlState* pSubControlState = pControlFactory->CreateNodeControlState(j->GetControlStateID(), j);


				if (!pSubControlState)
				{
					printf("CControlStateComposite::CopySubControl:: CreateNodeControlState failed \n");
					return false;
				}

				if (!pTo->AddSubControlList(pSubControlState, true))
					return false;
			}

			j = NULL;

			for (j = (CControlStateComposite *)m_subControlList.GetFirst();
				;
				j = (CControlStateComposite *)j->GetNext())
			{
				if (!j)
					return true;

				CControlStateComposite* pToa = (CControlStateComposite*)pControlFactory->CreateNodeControlState(j->GetControlStateID(), j);

				if (!pToa)
					break;

				if (!j->CopySubControl(pToa, pControlFactory))
					return 0;

				if (!pTo->AddSubControlList(pToa, true))
					return false;
			}
		}
		else
		{
			printf("CControlStateComposite::CopySubControl:: pControlFactory is null \n");
			return false;
		}
	}

	printf("CControlStateComposite::CopySubControl:: pTo is null \n");

	return false;
}

bool CControlStateComposite::RemoveSubControlList(int controlStateID)
{
	for (CControlState* j = (CControlState *)m_subControlList.GetFirst();
		j;
		j = (CControlState *)j->GetNext())
	{
		if (j->GetControlStateID() == controlStateID)
		{
			j->Stop();
			m_subControlList.Remove(j);
			SAFE_DELETE(j);

			return true;
		}
	}

	return false;
}

bool CControlStateComposite::RemoveSubControlQueue(int controlStateID)
{
	for (CControlState* j = (CControlState *)m_subControlQueue.GetFirst();
		j;
		j = (CControlState *)j->GetNext())
	{
		if (j->GetControlStateID() == controlStateID)
		{
			j->Stop();
			m_subControlQueue.Remove(j);
			SAFE_DELETE(j);

			return true;
		}
	}

	return false;
}

void CControlStateComposite::RemoveAllSubControlList()
{
	for (CControlStateComposite* pLinkObject = (CControlStateComposite *)m_subControlList.GetFirst();
		pLinkObject;
		pLinkObject = (CControlStateComposite *)m_subControlList.GetFirst())
	{
		pLinkObject->Stop();
		m_subControlList.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);
	}
}

void CControlStateComposite::RemoveAllSubControlQueue()
{
	for (CControlStateComposite* pLinkObject = (CControlStateComposite *)m_subControlQueue.GetFirst();
		pLinkObject;
		pLinkObject = (CControlStateComposite *)m_subControlQueue.GetFirst())
	{
		pLinkObject->Stop();
		m_subControlQueue.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);
	}
}

void CControlStateComposite::DestroyAllSubControlList()
{
	for (CControlStateComposite* pLinkObject = (CControlStateComposite *)m_subControlList.GetFirst();
		pLinkObject;
		pLinkObject = (CControlStateComposite *)m_subControlList.GetFirst())
	{
		m_subControlList.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);
	}
}

void CControlStateComposite::DestroyAllSubControlQueue()
{
	for (CControlStateComposite* pLinkObject = (CControlStateComposite *)m_subControlQueue.GetFirst();
		pLinkObject;
		pLinkObject = (CControlStateComposite *)m_subControlQueue.GetFirst())
	{
		m_subControlQueue.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);
	}
}

int CControlStateComposite::UpdateSubControlList(DWORD dwTickTime, float fMultiple)
{
	int status = ACTIVATED;

	CControlStateComposite* pLinkObject = (CControlStateComposite*)m_subControlList.GetFirst();
	if (pLinkObject)
	{
		while (pLinkObject)
		{
			CControlStateComposite* pNextState = (CControlStateComposite*)pLinkObject->GetNext();

			if (pLinkObject->IsStatus(COMPLETED) || pLinkObject->IsStatus(FAILED))
			{
				pLinkObject->Stop();
				m_subControlList.Remove(pLinkObject);
				SAFE_DELETE(pLinkObject);
			}
			else
			{
				pLinkObject->Start();

				if (pLinkObject->GetStatus() == FAILED)
				{
					ERR_LOG(LOG_SCRIPT, "CControlState::FAILED == ControlStatus");
				}
				else
				{
				//	ERR_LOG(LOG_GENERAL, "update list %s %u status %u\n", pLinkObject->GetControlStateName(), pLinkObject->GetControlStateID(), status);
					status = pLinkObject->OnUpdate(dwTickTime, fMultiple);
				}

				if (status != COMPLETED && status != FAILED)
				{
					if (status == CHANGED)
					{
						pLinkObject->Stop();
						break;
					}
				}
				else
				{
					pLinkObject->Stop();
					m_subControlList.Remove(pLinkObject);
					SAFE_DELETE(pLinkObject);
				}
				
			}

			pLinkObject = pNextState;
		}
	}
	else
	{
		status = COMPLETED;
	}

	return status;
}

int CControlStateComposite::UpdateSubControlQueue(DWORD dwTickTime, float fMultiple)
{
	CControlStateComposite* pLinkObject = NULL;

	for (pLinkObject = (CControlStateComposite *)m_subControlQueue.GetFirst();
		pLinkObject && (pLinkObject->IsStatus(COMPLETED) || pLinkObject->IsStatus(FAILED));
		pLinkObject = (CControlStateComposite *)m_subControlQueue.GetFirst())
	{
		pLinkObject->Stop();

		m_subControlQueue.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);
	}

	if (!pLinkObject)
		return COMPLETED;

	pLinkObject->Start();
	int status = pLinkObject->GetStatus();

	if (status == FAILED)
	{
		ERR_LOG(LOG_BOTAI, "CControlState::FAILED == subControlStatus");
	}
	else
	{
	//	ERR_LOG(LOG_GENERAL, "update queue %s %u status %u\n", pLinkObject->GetControlStateName(), pLinkObject->GetControlStateID(), status);
		status = pLinkObject->OnUpdate(dwTickTime, fMultiple);
	}

	if (status != COMPLETED && status != FAILED)
	{
		if (status == CHANGED)
		{
			pLinkObject->Stop();
		}

	}
	else
	{
		pLinkObject->Stop();

		m_subControlQueue.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);

		if (m_subControlQueue.GetCount() > 0)
			return ACTIVATED;
	}

	return status;
}

CControlAction* CControlStateComposite::GetCurrentAction()
{
	return (CControlAction*)m_subControlQueue.GetFirst();
}

CControlAction* CControlStateComposite::GetNextAction()
{
	CControlAction* result = (CControlAction*)m_subControlQueue.GetFirst();
	if (result)
		return (CControlAction*)result->GetNext();

	return NULL;
}
