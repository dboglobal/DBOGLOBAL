#include "precomp_ntlcore.h"
#include "NtlDebug.h"

#include "NtlSchedule.h"

CScheduler::CScheduler()
{
	m_fElapsedTime		= 0.0f;
}

CScheduler::~CScheduler()
{
	for (LIST_ELE_IT it = m_listScheduleElement.begin(); it != m_listScheduleElement.end(); ++it)
	{
		CScheduleElement* pScheduleElement = *it;
		NTL_DELETE(pScheduleElement);
	}
	m_listScheduleElement.clear();	
}

RwBool CScheduler::Scheduling(RwReal fElapsedTime)
{
	if (!m_listScheduleElement.empty())
	{
		m_fElapsedTime += fElapsedTime;
		while (m_fElapsedTime > 0.0f)
		{
			if (GetFirstScedule())
			{
				RwReal fTime = m_fElapsedTime - GetFirstScedule()->GetUsedTime();
				if (fTime > 0.0f)
				{
					if (SchedulingProcess())
					{
						m_fElapsedTime = fTime;
						continue;
					}
				}
			}
			break;
		}
		return TRUE;
	}
	m_fElapsedTime = 0.0f;
	return FALSE;
}

RwBool CScheduler::SchedulingProcess()
{
	CScheduleElement* pScheduleElement = NULL;
	if (m_listScheduleElement.empty())
	{
		return FALSE;
	}
	
	pScheduleElement = m_listScheduleElement.front();
	if (pScheduleElement)
	{
		switch (pScheduleElement->m_eScheduleStep)
		{
			case ESCHEDULE_STEP_NONE:
				pScheduleElement->m_eScheduleStep = ESCHEDULE_STEP_BEGIN;
			case ESCHEDULE_STEP_BEGIN:
				if (!pScheduleElement->Begin())
				{
					RemovePresentSchedule();
					break;
				}
				pScheduleElement->m_eScheduleStep = ESCHEDULE_STEP_SCHEDULING;
			case ESCHEDULE_STEP_SCHEDULING:
				if (pScheduleElement->Scheduling())
				{
					break;
				}
				pScheduleElement->m_eScheduleStep = ESCHEDULE_STEP_END;
			case ESCHEDULE_STEP_END:
				pScheduleElement->End();
				RemovePresentSchedule();
				break;
		}
	}
	return TRUE;
}

void CScheduler::RemovePresentSchedule()
{
	if (!m_listScheduleElement.empty())
	{
		CScheduleElement* pScheduleElement = m_listScheduleElement.front();
		NTL_DELETE(pScheduleElement);
		m_listScheduleElement.pop_front();
	}
}

void CScheduler::AddSchedule(CScheduleElement* pNewScheduleElement)
{
	m_listScheduleElement.push_back(pNewScheduleElement);
}

void CScheduler::RemoveSchedule(RwInt32 iPrimaryKey)
{
	for (LIST_ELE_IT it = m_listScheduleElement.begin(); it != m_listScheduleElement.end(); ++it)
	{
		CScheduleElement* pScheduleElement = *it;
		if (iPrimaryKey == pScheduleElement->GetPrimaryKey())
		{
			NTL_DELETE(pScheduleElement);
			m_listScheduleElement.erase(it);
			break;
		}
	}
}

void CScheduler::ClearSchedule()
{
	for (LIST_ELE_IT it = m_listScheduleElement.begin(); it != m_listScheduleElement.end(); ++it)
	{
		CScheduleElement* pScheduleElement = *it;
		NTL_DELETE(pScheduleElement);
	}
	m_listScheduleElement.clear();
}

CScheduleElement* CScheduler::GetFirstScedule()
{
	if (m_listScheduleElement.empty())
	{
		return NULL;
	}
	return m_listScheduleElement.front();
}

//////////////////////////////////////////////////////////////////////////

CScheduleElement::CScheduleElement(RwInt32 iPrimaryKey, RwReal fUsedTime)
:m_iPrimaryKey(iPrimaryKey)
,m_fUsedTime(fUsedTime)
,m_eScheduleStep(ESCHEDULE_STEP_NONE)
{
}

CScheduleElement::~CScheduleElement()
{
}

//////////////////////////////////////////////////////////////////////////