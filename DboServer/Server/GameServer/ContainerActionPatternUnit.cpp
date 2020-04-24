#include "stdafx.h"
#include "ContainerActionPatternUnit.h"


CContainerActionPatternUnit::CContainerActionPatternUnit()
{
	m_bIsDestroy = false;
	m_vecActionPattern.clear();
	m_actionpatternTblIdx = INVALID_TBLIDX;
}

CContainerActionPatternUnit::~CContainerActionPatternUnit()
{
	if (!m_bIsDestroy)
		Destroy();
}


bool CContainerActionPatternUnit::Create(TBLIDX actionpatternTblID, ACTIONPATTERN_FIELDSIZE nActionPatternFieldSize)
{
	if (actionpatternTblID == INVALID_TBLIDX)
	{
		ERR_LOG(LOG_BOTAI, "fail : INVALID_TBLIDX == actionpatternTblID");
	}
	else if (nActionPatternFieldSize < MAX_SAVECOUNT_ACTIONPATTERN_FIELD)
	{
		m_actionpatternTblIdx = actionpatternTblID;
		m_vecActionPattern.reserve(nActionPatternFieldSize);
		return true;
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "fail : MAX_SAVECOUNT_ACTIONPATTERN_FIELD(%d) <= nActionPatternFieldSize(%d), actionpatternTblID = %u", MAX_SAVECOUNT_ACTIONPATTERN_FIELD, nActionPatternFieldSize, actionpatternTblID);
	}

	return false;
}

void CContainerActionPatternUnit::Destroy()
{
	m_bIsDestroy = true;
	AllDeleteActionPattern();
}

bool CContainerActionPatternUnit::AddActionPattern(CActionPatternUnit* pActionUnit)
{
	if (pActionUnit)
	{
		if (m_vecActionPattern.size() < MAX_SAVECOUNT_ACTIONPATTERN_FIELD)
		{
			m_vecActionPattern.push_back(pActionUnit);
			return true;
		}
		else
		{
			ERR_LOG(LOG_BOTAI, "fail : MAX_SAVECOUNT_ACTIONPATTERN_FIELD(%d) <= m_vecActionPattern.size(%d), m_actionpatternTblIdx = %u", MAX_SAVECOUNT_ACTIONPATTERN_FIELD, m_vecActionPattern.size(), m_actionpatternTblIdx);
		}
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "fail : NULL == pActionUnit");
	}

	return false;
}

CActionPatternUnit* CContainerActionPatternUnit::GetActionPattern(ACTIONPATTERN_FIELD apField)
{
	if (apField == INVALID_ACTIONPATTERN_FIELD)
	{
		ERR_LOG(LOG_BOTAI, "fail : INVALID_ACTIONPATTERN_FIELD(%d) == apField", apField);
	}
	else if (apField < MAX_SAVECOUNT_ACTIONPATTERN_FIELD)
	{
		if (m_vecActionPattern.size() > apField)
			return m_vecActionPattern[apField];
		else
			ERR_LOG(LOG_BOTAI, "fail : m_vecActionPattern.size()(%u) <= apField(%u), m_actionpatternTblIdx = %u", m_vecActionPattern.size(), apField, m_actionpatternTblIdx);
	}
	else
		ERR_LOG(LOG_BOTAI, "fail : MAX_SAVECOUNT_ACTIONPATTERN_FIELD(%d) <= apField(%u), m_actionpatternTblIdx = %u", MAX_SAVECOUNT_ACTIONPATTERN_FIELD, apField, m_actionpatternTblIdx);

	return NULL;
}

bool CContainerActionPatternUnit::DeleteActionPattern(ACTIONPATTERN_FIELD apField)
{
	if (apField == INVALID_ACTIONPATTERN_FIELD)
	{
		ERR_LOG(LOG_BOTAI, "fail : INVALID_ACTIONPATTERN_FIELD(%d) == apField", apField);
	}
	else if (m_vecActionPattern.size() > apField)
	{
		CActionPatternUnit* pUnit = m_vecActionPattern[apField];
		if (pUnit)
		{
			delete pUnit;
		}
		m_vecActionPattern[apField] = NULL;

		return true;
	}
	else
		ERR_LOG(LOG_BOTAI, "fail : m_vecActionPattern.size()(%u) <= apField(%u), m_actionpatternTblIdx = %u", m_vecActionPattern.size(), apField, m_actionpatternTblIdx);

	return false;
}

void CContainerActionPatternUnit::AllDeleteActionPattern()
{
	ACTIONPATTERN_ITER it = m_vecActionPattern.begin();
	for (; it != m_vecActionPattern.end(); it++)
	{
		CActionPatternUnit* pUnit = *it;
		if (pUnit)
			delete pUnit;
	}

	m_vecActionPattern.clear();
}

int CContainerActionPatternUnit::GetCount()
{
	return (int)m_vecActionPattern.size();
}

TBLIDX CContainerActionPatternUnit::GetTblIdx()
{
	return this->m_actionpatternTblIdx;
}
