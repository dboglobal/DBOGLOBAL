#include "stdafx.h"

#include "NtlPLEvent.h"
#include "NtlHistoryController.h"

CNtlHistoryData::CNtlHistoryData()
{
}

CNtlHistoryData::~CNtlHistoryData()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlHistoryAction::CNtlHistoryAction()
{
	m_uiState = EHISTROY_STATE_NONE;
}

CNtlHistoryAction::~CNtlHistoryAction()
{
	DeleteHistoryDataAll();
}

void CNtlHistoryAction::PushHistoryData(CNtlHistoryData* pHistoryData)
{
	m_vecHistoryDataList.push_back(pHistoryData);

	m_uiState = EHISTROY_STATE_REDO;
}

void CNtlHistoryAction::DeleteHistoryDataAll()
{
	VEC_HISTORY_DATA_IT	it		= m_vecHistoryDataList.begin();
	while (it != m_vecHistoryDataList.end())
	{
		CNtlHistoryData* pHistoryDataTemp = *it;
		NTL_DELETE(pHistoryDataTemp);
		it = m_vecHistoryDataList.erase(it);
	}
	m_uiState = EHISTROY_STATE_NONE;
}

void CNtlHistoryAction::Undo()
{
	m_uiState = EHISTROY_STATE_UNDO;

	VEC_HISTORY_DATA_RIT	rit		= m_vecHistoryDataList.rbegin();
	while (rit != m_vecHistoryDataList.rend())
	{
		CNtlHistoryData* pHistoryDataTemp = *rit;
		Undo(pHistoryDataTemp);
		++rit;
	}
}

void CNtlHistoryAction::Redo()
{
	m_uiState = EHISTROY_STATE_REDO;

	VEC_HISTORY_DATA_IT	it		= m_vecHistoryDataList.begin();
	while (it != m_vecHistoryDataList.end())
	{
		CNtlHistoryData* pHistoryDataTemp = *it;
		Redo(pHistoryDataTemp);
		++it;
	}
}

RwBool CNtlHistoryAction::CheckException()
{
	m_uiState = EHISTROY_STATE_REDO;

	VEC_HISTORY_DATA_IT	it		= m_vecHistoryDataList.begin();
	while (it != m_vecHistoryDataList.end())
	{
		CNtlHistoryData* pHistoryDataTemp = *it;
		if (CheckException(pHistoryDataTemp))
		{
			return TRUE;
		}
		++it;
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlHistoryController::CNtlHistoryController()
{
	m_uiCurrentIndex	= dNTL_HISTORY_INVALID_IDX;
	m_uiHistroyID		= dNTL_HISTORY_INVALID_IDX;

	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY, 0);
}

CNtlHistoryController::~CNtlHistoryController()
{
	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY);

	DeleteHistroyActionAll();
}

CNtlHistoryAction* CNtlHistoryController::PushHistoryAction(CNtlHistoryAction* pHistoryAction, RwUInt32 uiHistroyID)
{
	if (uiHistroyID != m_uiHistroyID)
	{
		DeleteHistroyActionAll();
		m_uiHistroyID = uiHistroyID;
	}

	if (m_uiCurrentIndex != dNTL_HISTORY_INVALID_IDX && !m_vecHistoryActionList.empty() && m_uiCurrentIndex != m_vecHistoryActionList.size())
	{
		DeleteHistroyActionAfter(m_uiCurrentIndex);
	}

	static RwUInt32 uiQSize = 3;
	if (m_vecHistoryActionList.size() >= uiQSize)
	{
		DeleteHistroyActionBefore((m_vecHistoryActionList.size()) - uiQSize + 2); // +2 는 INDEX가 1부터 시작하기 때문이다.
	}

	m_vecHistoryActionList.push_back(pHistoryAction);
	m_uiCurrentIndex = m_vecHistoryActionList.size();

	return NULL;
}

void CNtlHistoryController::DeleteHistroyActionAfter(RwUInt32 uiIndex)
{
	RwUInt32				uiCnt	= 0;
	VEC_HISTORY_ACTION_IT	it		= m_vecHistoryActionList.begin();
	while (it != m_vecHistoryActionList.end())
	{
		if (++uiCnt > uiIndex)
		{
			CNtlHistoryAction* pHistoryActionTemp = *it;
			NTL_DELETE(pHistoryActionTemp);
			it = m_vecHistoryActionList.erase(it);
		}
		else
		{
			++it;
		}
	}
	if (m_vecHistoryActionList.empty())
	{
		m_uiCurrentIndex = dNTL_HISTORY_INVALID_IDX;
	}
}

void CNtlHistoryController::DeleteHistroyActionBefore(RwUInt32 uiIndex)
{
	RwUInt32				uiCnt	= 0;
	VEC_HISTORY_ACTION_IT	it		= m_vecHistoryActionList.begin();
	while (it != m_vecHistoryActionList.end())
	{
		if (++uiCnt < uiIndex)
		{
			CNtlHistoryAction* pHistoryActionTemp = *it;
			NTL_DELETE(pHistoryActionTemp);
			it = m_vecHistoryActionList.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (m_vecHistoryActionList.empty())
	{
		m_uiCurrentIndex = dNTL_HISTORY_INVALID_IDX;
	}
}

void CNtlHistoryController::DeleteHistroyActionAll()
{
	VEC_HISTORY_ACTION_IT	it		= m_vecHistoryActionList.begin();
	while (it != m_vecHistoryActionList.end())
	{
		CNtlHistoryAction* pHistoryActionTemp = *it;
		NTL_DELETE(pHistoryActionTemp);
		it = m_vecHistoryActionList.erase(it);
	}
	m_uiCurrentIndex	= dNTL_HISTORY_INVALID_IDX;
	m_uiHistroyID		= dNTL_HISTORY_INVALID_IDX;
}

void CNtlHistoryController::Undo()
{
	SetHistroyActionPos(m_uiCurrentIndex - 1);
}

void CNtlHistoryController::Redo()
{
	SetHistroyActionPos(m_uiCurrentIndex + 1);
}

void CNtlHistoryController::SetHistroyActionPos(RwUInt32 uiIdx)
{
	if (m_vecHistoryActionList.empty())
	{
		return;
	}
	if (uiIdx > m_vecHistoryActionList.size())
	{
		return;
	}

	if (m_uiCurrentIndex < uiIdx)
	{
		while (m_uiCurrentIndex < uiIdx)
		{
			m_vecHistoryActionList[m_uiCurrentIndex++]->Redo();
		}
	}
	else if (m_uiCurrentIndex > uiIdx)
	{
		while (m_uiCurrentIndex > uiIdx) 
		{
			m_vecHistoryActionList[--m_uiCurrentIndex]->Undo();
		}
	}
}

void CNtlHistoryController::CheckException()
{
	RwUInt32				uiCnt	= 0;
	VEC_HISTORY_ACTION_IT	it		= m_vecHistoryActionList.begin();
	while (it != m_vecHistoryActionList.end())
	{
		++uiCnt;
		CNtlHistoryAction* pHistoryActionTemp = *it;
		if (pHistoryActionTemp->CheckException())
		{
			NTL_DELETE(pHistoryActionTemp);
			it = m_vecHistoryActionList.erase(it);
			if (uiCnt <= m_uiCurrentIndex)
			{
				--m_uiCurrentIndex;
				--uiCnt;
			}
		}
		else
		{
			++it;
		}
	}
}

void CNtlHistoryController::HandleEvents(RWS::CMsg &pMsg)
{
	if (pMsg.Id == NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY)
	{
		CheckException();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////