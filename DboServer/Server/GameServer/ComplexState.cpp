#include "stdafx.h"
#include "ComplexState.h"
#include "BotAiBase.h"


CComplexState::CComplexState(int controlStateID, const char* lpszControlStateName)
: CControlState(controlStateID, lpszControlStateName)
{

}

CComplexState::~CComplexState()
{
	DestroyAllSubControl();
}


void CComplexState::Start()
{
	if (m_status == INACTIVATED)
	{
		m_status = ACTIVATED;
		OnEnter();
	}
	else if (m_status == PAUSED)
	{
		m_status = ACTIVATED;
		OnContinue();
	}
}

void CComplexState::Pause()
{
	if (m_status == ACTIVATED)
	{
		CComplexState* pLinkObject = (CComplexState*)m_subControlList.GetFirst();
		CComplexState* pLinkNext = NULL;
		while (pLinkObject)
		{
			pLinkNext = (CComplexState*)pLinkObject->GetNext();
			pLinkObject->Pause();

			if (pLinkObject->IsStatus(COMPLETED))
			{
				pLinkObject->Stop();
				m_subControlList.Remove(pLinkObject);
				SAFE_DELETE(pLinkObject);
			}

			pLinkObject = pLinkNext;
		}

		if (m_subControlQueue.GetFirst())
		{
			pLinkObject = (CComplexState*)m_subControlQueue.GetFirst();
			pLinkObject->Pause();

			if (pLinkObject->IsStatus(COMPLETED))
			{
				pLinkObject->Stop();
				m_subControlQueue.Remove(pLinkObject);
				SAFE_DELETE(pLinkObject);
			}
		}

		OnPause();

		if (m_status == ACTIVATED)
			m_status = PAUSED;
	}

}

void CComplexState::Stop()
{
	if (m_status != INACTIVATED)
	{
		CComplexState* pState = NULL;
		
		for (pState = (CComplexState*)m_subControlList.GetFirst(); pState; pState = (CComplexState*)pState->GetNext())
			pState->Stop();

		if (m_subControlQueue.GetFirst())
		{
			pState = (CComplexState*)m_subControlQueue.GetFirst();
			pState->Stop();
		}

		OnExit();
		m_status = INACTIVATED;
	}
}

bool CComplexState::AddSubControlList(CControlState * controlstate, bool bFront)
{
	if (controlstate)
	{
		if (controlstate->GetPrev() || controlstate->GetNext())
		{
			ERR_LOG(LOG_SCRIPT, "pControlState->GetPrev() || pControlState->GetNext()");
			return false;
		}
		else
		{
			controlstate->SetParent(this);
			m_subControlList.Push(controlstate, bFront);
			return true;
		}
	}

	ERR_LOG(LOG_SCRIPT, "controlstate == NULL");
	return false;
}

bool CComplexState::AddSubControlQueue(CControlState * controlstate, bool bFront)
{
	if (controlstate)
	{
		if (controlstate->GetPrev() || controlstate->GetNext())
		{
			ERR_LOG(LOG_SCRIPT, "pControlState->GetPrev() || pControlState->GetNext()");
			return false;
		}
		else
		{
			if (m_subControlQueue.GetFirst())
			{
				((CComplexState*)m_subControlQueue.GetFirst())->Pause();
			}

			controlstate->SetParent(this);
			m_subControlQueue.Push(controlstate, bFront);
			return true;
		}
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "controlstate == NULL");
		return false;
	}

	return false;
}

void CComplexState::RemoveAllSubControl()
{
	RemoveAllSubControlList();
	RemoveAllSubControlQueue();
}

void CComplexState::DestroyAllSubControl()
{
	DestroyAllSubControlList();
	DestroyAllSubControlQueue();
}

void CComplexState::AllSubControlStop()
{
	CComplexState * j = NULL;

	//list
	for (j = (CComplexState *)m_subControlList.GetFirst(); j; j = (CComplexState*)j->GetNext())
		j->Stop();
	//queue
	for (j = (CComplexState *)m_subControlQueue.GetFirst(); j; j = (CComplexState*)j->GetNext())
		j->Stop();
}

bool CComplexState::MoveSubControl(CComplexState * pTo)
{
	if (pTo)
	{
		CComplexState * pLinkObject = NULL;

		for (pLinkObject = (CComplexState *)m_subControlQueue.GetFirst();
			pLinkObject;
			pLinkObject = (CComplexState *)m_subControlQueue.GetFirst())
		{
			m_subControlQueue.Remove(pLinkObject);

			if (!pTo->AddSubControlQueue(pLinkObject, true))
				return false;
		}

		pLinkObject = NULL;

		for (pLinkObject = (CComplexState *)m_subControlList.GetFirst();
			pLinkObject;
			pLinkObject = (CComplexState *)m_subControlList.GetFirst())
		{
			m_subControlList.Remove(pLinkObject);

			if (!pTo->AddSubControlList(pLinkObject, true))
				return false;
		}

		return true;
	}
	
	printf("CComplexState::MoveSubControl: pTo is null \n");

	return false;
}

bool CComplexState::CopySubControl(CComplexState * pTo, CControlStateFactory * pControlFactory)
{
	if (pTo)
	{
		if (pControlFactory)
		{
			CComplexState * j = NULL;

			for (j = (CComplexState *)m_subControlQueue.GetFirst(); 
				j; 
				j = (CComplexState *)j->GetNext())
			{
				CControlState* pSubControlState = pControlFactory->CreateNodeControlState(j->GetControlStateID(), j);

				if (!pSubControlState)
				{
					printf("CComplexState::CopySubControl:: CreateNodeControlState failed \n");
					return false;
				}

				if (!pTo->AddSubControlList(pSubControlState, true))
					return false;
			}


			for (j = (CComplexState *)m_subControlList.GetFirst();
				j; 
				j = (CComplexState *)j->GetNext())
			{
				CComplexState* pToa = (CComplexState*)pControlFactory->CreateNodeControlState(j->GetControlStateID(), j);

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
			printf("CComplexState::CopySubControl:: pControlFactory is null \n");
			return false;
		}
	}

	printf("CComplexState::CopySubControl:: pTo is null \n");

	return false;
}

bool CComplexState::RemoveSubControlList(int controlStateID)
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

bool CComplexState::RemoveSubControlQueue(int controlStateID)
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

void CComplexState::RemoveAllSubControlList()
{
	for (CComplexState* j = (CComplexState *)m_subControlList.GetFirst();
		j;
		j = (CComplexState *)m_subControlList.GetFirst())
	{
		j->RemoveAllSubControl();
		j->Stop();
		m_subControlList.Remove(j);
		SAFE_DELETE(j);
	}
}

void CComplexState::RemoveAllSubControlQueue()
{
	for (CComplexState* j = (CComplexState *)m_subControlQueue.GetFirst();
		j;
		j = (CComplexState *)m_subControlQueue.GetFirst())
	{
		j->RemoveAllSubControl();
		j->Stop();
		m_subControlQueue.Remove(j);
		SAFE_DELETE(j);
	}
}

void CComplexState::DestroyAllSubControlList()
{
	for (CComplexState* j = (CComplexState *)m_subControlList.GetFirst();
		j;
		j = (CComplexState *)m_subControlList.GetFirst())
	{
		j->DestroyAllSubControl();
		m_subControlList.Remove(j);
		SAFE_DELETE(j);
	}
}

void CComplexState::DestroyAllSubControlQueue()
{
	for (CComplexState* j = (CComplexState *)m_subControlQueue.GetFirst();
		j;
		j = (CComplexState *)m_subControlQueue.GetFirst())
	{
		j->DestroyAllSubControl();
		m_subControlQueue.Remove(j);
		SAFE_DELETE(j);
	}
}


int CComplexState::UpdateSubControlList(DWORD dwTickTime, float fMultiple)
{
	CBotAiBase* pLinkObject = (CBotAiBase*)m_subControlList.GetFirst();

	int status = 0;

	if (pLinkObject)
	{
		while (pLinkObject)
		{
			//ERR_LOG(LOG_BOTAI, "update list %s %u status %u \n", pLinkObject->GetControlStateName(), pLinkObject->GetControlStateID(), pLinkObject->GetStatus());
			CBotAiBase* pNextState = (CBotAiBase*)pLinkObject->GetNext();
			if (pLinkObject->IsStatus(COMPLETED) || pLinkObject->IsStatus(FAILED))
			{
				pLinkObject->Stop();
				m_subControlList.Remove(pLinkObject);
				SAFE_DELETE(pLinkObject);
			}
			else
			{
				pLinkObject->Start();
				status = pLinkObject->GetStatus();
				switch (status)
				{
					case ACTIVATED:
					{
						int iUpdateRes = pLinkObject->OnUpdate(dwTickTime, fMultiple);
						switch (iUpdateRes)
						{
							case ACTIVATED:
							case BLOCKED:
							case SYSTEMERROR:
							{
								goto LABEL_7; 
							}
							break;

							case COMPLETED:
							case FAILED:
							{
								pLinkObject->Stop();
								m_subControlList.Remove(pLinkObject);
								SAFE_DELETE(pLinkObject);
								goto LABEL_7;
							}
							break;

							case CHANGED:
							{
								if (pLinkObject->IsStatus(COMPLETED) || pLinkObject->IsStatus(FAILED))
								{
									pLinkObject->Stop();
									m_subControlList.Remove(pLinkObject);
									SAFE_DELETE(pLinkObject);
								}

								return CHANGED;

							}
							break;

							case CLEARED: return CLEARED; break;

							default:
							{
								ERR_LOG(LOG_SCRIPT, "invalid status. [%u]", iUpdateRes);
							}
							break;
						}

					}break;
					
					case COMPLETED:
					case FAILED:
					{
						pLinkObject->Stop();
						m_subControlList.Remove(pLinkObject);
						SAFE_DELETE(pLinkObject);
					}
					break;

					case CHANGED:
					{
						if (pLinkObject->IsStatus(COMPLETED) || pLinkObject->IsStatus(FAILED))
						{
							pLinkObject->Stop();
							m_subControlList.Remove(pLinkObject);
							SAFE_DELETE(pLinkObject);
						}

						return CHANGED;
					}

					case CLEARED: return status; break;

					case BLOCKED:
					case SYSTEMERROR:
						break;

					default: printf("UpdateSubControlList: unknown status [%i] \n", status); break;
				}
			}


		LABEL_7:
			pLinkObject = pNextState;
		}
	}
	else
	{
		status = COMPLETED;
	}

	return status;
}

int CComplexState::UpdateSubControlQueue(DWORD dwTickTime, float fMultiple)
{
	CBotAiBase* pLinkObject = NULL;

	for (pLinkObject = (CBotAiBase *)m_subControlQueue.GetFirst();
		pLinkObject && (pLinkObject->IsStatus(COMPLETED) || pLinkObject->IsStatus(FAILED));
		pLinkObject = (CBotAiBase *)m_subControlQueue.GetFirst())
	{
		pLinkObject->Stop();
		m_subControlQueue.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);
	}

	int result = ACTIVATED;

	if (pLinkObject)
	{
		//ERR_LOG(LOG_BOTAI, "update queue %s %u status %u \n", pLinkObject->GetControlStateName(), pLinkObject->GetControlStateID(), pLinkObject->GetStatus());
		pLinkObject->Start();
		int nStatus = pLinkObject->GetStatus();
		switch (nStatus)
		{
			case ACTIVATED:
			{
				int iUpdateRes = pLinkObject->OnUpdate(dwTickTime, fMultiple); 
				switch (iUpdateRes)
				{
					case ACTIVATED:
					case BLOCKED:
					case SYSTEMERROR:
					case PAUSED:
					{
						result = iUpdateRes; //maybe nStatus must be changed to iUpdateRes ???
					}break;

					case COMPLETED:
					case FAILED:
					{
						pLinkObject->Stop();
						m_subControlQueue.Remove(pLinkObject);
						SAFE_DELETE(pLinkObject);

						if (m_subControlQueue.GetCount() <= 0)
						{
							result = iUpdateRes; //maybe nStatus must be changed to iUpdateRes ???
						}
						else
							result = ACTIVATED;
					}
					break;

					case CHANGED:
					{
						if (pLinkObject->IsStatus(COMPLETED) || pLinkObject->IsStatus(FAILED))
						{
							pLinkObject->Stop();
							m_subControlQueue.Remove(pLinkObject);
							SAFE_DELETE(pLinkObject);
						}

						result = CHANGED;
					}
					break;

					case CLEARED:
					{
						result = CLEARED;
					}
					break;

					default:
					{
						ERR_LOG(LOG_SCRIPT, "invalid status [%i] \n", iUpdateRes);
						result = SYSTEMERROR;
					}break;
				}
			}break;

			case COMPLETED:
			case FAILED:
			{
				pLinkObject->Stop();
				m_subControlQueue.Remove(pLinkObject);
				SAFE_DELETE(pLinkObject);

				if (m_subControlQueue.GetCount() <= 0)
					result = nStatus;
				else
					result = ACTIVATED;

			}break;

			case CHANGED:
			{
				if (pLinkObject->IsStatus(COMPLETED) || pLinkObject->IsStatus(FAILED))
				{
					pLinkObject->Stop();
					m_subControlQueue.Remove(pLinkObject);
					SAFE_DELETE(pLinkObject);
				}
				result = CHANGED;
			}
			break;
			case CLEARED:
			{
				result = CLEARED;
			}
			break;
			case BLOCKED:
			case SYSTEMERROR:
			{
				result = nStatus;
			}break;

			default:
			{
				ERR_LOG(LOG_SCRIPT, "invalid status [%i] \n", nStatus);
				result = SYSTEMERROR;
			}break;
		}
	}
	else
	{
		result = COMPLETED;
	}

	return result;
}


CComplexState* CComplexState::GetCurrentAction()
{
	return (CComplexState*)m_subControlQueue.GetFirst();
}


int CComplexState::OnUpdate(DWORD dwTickTime, float fMultiple)
{
	int status = UpdateSubControlList(dwTickTime, fMultiple);
	if (status != CHANGED && status != CLEARED)
	{
		status = UpdateSubControlQueue(dwTickTime, fMultiple);
	}

	return status;
}


int CComplexState::OnObjectMsg(CObjectMsg * objmsg)
{
	for (CControlState* j = (CControlState*)m_subControlList.GetFirst(); j; j = (CControlState*)j->GetNext())
	{
		if (j->GetStatus() == ACTIVATED || j->GetStatus() == BLOCKED)
			j->OnObjectMsg(objmsg);
	}

	CControlState* j = (CControlState*)m_subControlQueue.GetFirst();
	if(j)
	{
		if (j->GetStatus() == ACTIVATED || j->GetStatus() == BLOCKED)
			j->OnObjectMsg(objmsg);
	}

	return m_status;
}