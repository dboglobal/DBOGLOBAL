#include "stdafx.h"
#include "ControlStateEx.h"


CControlStateEx::CControlStateEx(int controlStateID, const char* lpszControlStateName)
:CControlState(controlStateID, lpszControlStateName)
{
	m_bRemovable = true;
	m_bRemoving = false;
}

CControlStateEx::~CControlStateEx()
{
	this->RemoveAllSubControl();
}



int CControlStateEx::OnUpdate(DWORD dwTickTime, float fMultiple)
{
	int nRet = -1;

	this->UpdateSubControlList(dwTickTime, fMultiple);
	if (nRet != CHANGED)
		nRet = this->UpdateSubControlQueue(dwTickTime, fMultiple);

	return nRet;
}


int CControlStateEx::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	for (CControlStateEx* pLinkObject = (CControlStateEx*)m_subControlList.GetFirst();
		pLinkObject;
		pLinkObject = (CControlStateEx*)m_subControlList.GetFirst())
	{
		if (pLinkObject->GetStatus() == ACTIVATED || pLinkObject->GetStatus() == 5)
		{
			pLinkObject->OnObjectMsg(pObjectMsg);
		}
	}

	CControlState* j = (CControlState*)m_subControlQueue.GetFirst();
	if (j)
	{
		if (j->GetStatus() == ACTIVATED || j->GetStatus() == 5)
			j->OnObjectMsg(pObjectMsg);
	}

	return m_status;
}


bool CControlStateEx::AddSubControlList(CControlState * state, bool bFront)
{
	if (state)
	{
		if (state->GetPrev() || state->GetNext())
		{
			ERR_LOG(LOG_SCRIPT, "state->GetPrev() || state->GetNext()");
			return false;
		}
		else
		{
			this->m_subControlList.Push(state, bFront);
			return true;
		}
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "state == NULL");
	}

	return false;
}

bool CControlStateEx::AddSubControlQueue(CControlState * state, bool bFront)
{
	if (state)
	{
		if (state->GetPrev() || state->GetNext())
		{
			ERR_LOG(LOG_SCRIPT, "state->GetPrev() || state->GetNext()");
			return false;
		}
		else
		{
			this->m_subControlQueue.Push(state, bFront);
			return true;
		}
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "state == NULL");
	}

	return false;
}

void CControlStateEx::RemoveAllSubControl()
{
	this->RemoveAllSubControlList();
	this->RemoveAllSubControlQueue();
}

CControlStateEx* CControlStateEx::GetSubControlQueue(int id)
{
	for (CControlStateEx* pLinkObject = (CControlStateEx *)m_subControlQueue.GetFirst();
		pLinkObject;
		pLinkObject = (CControlStateEx *)m_subControlQueue.GetFirst())
	{
		if (pLinkObject->GetControlStateID() == id)
			return pLinkObject;
	}

	return NULL;
}

void CControlStateEx::RemoveAllSubControlList()
{
	for (CControlStateEx* pLinkObject = (CControlStateEx*)m_subControlList.GetFirst();
		pLinkObject;
		pLinkObject = (CControlStateEx*)m_subControlList.GetFirst())
	{
		pLinkObject->OnExit();
		m_subControlList.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);
	}
}

void CControlStateEx::RemoveAllSubControlQueue()
{
	for (CControlStateEx* pLinkObject = (CControlStateEx*)m_subControlQueue.GetFirst();
		pLinkObject;
		pLinkObject = (CControlStateEx*)m_subControlQueue.GetFirst())
	{
		pLinkObject->OnExit();
		m_subControlQueue.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);
	}
}

int CControlStateEx::UpdateSubControlList(DWORD dwTickTime, float fMultiple)
{
	return 0;
}

int CControlStateEx::UpdateSubControlQueue(DWORD dwTickTime, float fMultiple)
{
	return 0;
}

void CControlStateEx::ClearRemovableList()
{
	CControlStateEx* pLinkObject = (CControlStateEx*)m_subControlList.GetFirst();
	CControlStateEx* pNextLinkObject = NULL;
	while (pLinkObject)
	{
		pNextLinkObject = (CControlStateEx*)pLinkObject->GetNext();

		this->SetRemoving(true);
		pLinkObject->ClearRemovableList();
		pLinkObject->ClearRemovableQueue();

		if (pLinkObject->IsEmpty())
		{
			if (pLinkObject->IsRemovable() == true || pLinkObject->IsStatus(ACTIVATED) != true)
			{
				pLinkObject->OnExit();
				m_subControlList.Remove(pLinkObject);
				
				SAFE_DELETE(pLinkObject);
			}
		}

		pLinkObject = pNextLinkObject;
	}
}

void CControlStateEx::ClearRemovableQueue()
{
	CControlStateEx* pLinkObject = (CControlStateEx*)m_subControlQueue.GetFirst();
	CControlStateEx* pNextLinkObject = NULL;
	while (pLinkObject)
	{
		pNextLinkObject = (CControlStateEx*)pLinkObject->GetNext();

		this->SetRemoving(true);
		pLinkObject->ClearRemovableList();
		pLinkObject->ClearRemovableQueue();

		if (pLinkObject->IsEmpty())
		{
			if (pLinkObject->IsRemovable() == true || pLinkObject->IsStatus(ACTIVATED) != true)
			{
				pLinkObject->OnExit();
				m_subControlQueue.Remove(pLinkObject);
				
				SAFE_DELETE(pLinkObject);
			}
		}

		pLinkObject = pNextLinkObject;
	}
}













