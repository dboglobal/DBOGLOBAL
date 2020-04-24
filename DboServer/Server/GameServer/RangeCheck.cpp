#include "stdafx.h"
#include "RangeCheck.h"
#include "char.h"


CRangeCheck::CRangeCheck(CCharacter* pChar, CRangeCheck::eRANGECHECK_TYPE eRangeCheckType)
{
	m_pParent = pChar;
	m_eRangeCheckType = eRangeCheckType;
}

CRangeCheck::~CRangeCheck()
{
}



void CRangeCheck::AddBot(CCharacter* pChar)
{
	if (pChar)
	{
		if (m_eRangeCheckType == eSINGLENESS)
		{
			AddSinglenessBot(pChar);
		}
		else if (m_eRangeCheckType == eSEVERAL)
		{
			AddSeveralBot(pChar);
		}
		else ERR_LOG(LOG_SYSTEM, "fail : m_eRangeCheckType(%d)", m_eRangeCheckType);
	}
	else ERR_LOG(LOG_SYSTEM, "fail : NULL == pBot");
}


CCharacter* CRangeCheck::ResultSinglenessBot()
{
	return this->m_sRangeCheckData_Singleness.pChar;
}


void CRangeCheck::ResultSeveralBot(std::vector<CCharacter*>& rBotList, int nContainerSize)
{
	rBotList.clear();

	for (RANGECHECK_MAP::iterator it = m_mapRangeCheck.begin(); it != m_mapRangeCheck.end(); it++)
	{
		if (rBotList.size() >= nContainerSize)
			break;

		rBotList.push_back(it->second.pChar);
	}
}


bool CRangeCheck::IsEmptySeveral()
{
	if (m_eRangeCheckType != eSEVERAL)
		ERR_LOG(LOG_SYSTEM, "fail : eSEVERAL != m_eRangeCheckType(%d)", m_eRangeCheckType);

	return m_mapRangeCheck.empty();
}


void CRangeCheck::AddSinglenessBot(CCharacter* pChar)
{
	if (pChar)
	{
		CNtlVector vTemp(pChar->GetCurLoc().operator-(m_pParent->GetCurLoc()));
		vTemp.y = 0.0f;
		float fRangeDiff = vTemp.Length();
		if (m_sRangeCheckData_Singleness.pChar)
		{
			if (m_sRangeCheckData_Singleness.fRangeDiff > fRangeDiff)
			{
				m_sRangeCheckData_Singleness.pChar = pChar;
				m_sRangeCheckData_Singleness.fRangeDiff = fRangeDiff;
			}
		}
		else
		{
			m_sRangeCheckData_Singleness.pChar = pChar;
			m_sRangeCheckData_Singleness.fRangeDiff = fRangeDiff;
		}
	}
	else ERR_LOG(LOG_SYSTEM, "fail : NULL == pBot");
}


void CRangeCheck::AddSeveralBot(CCharacter* pChar)
{
	if (pChar)
	{
		CNtlVector vTemp(pChar->GetCurLoc().operator-(m_pParent->GetCurLoc()));
		vTemp.y = 0.0f;
		float fRangeDiff = vTemp.Length();

		sRANGECHECK_DATA sRangeData(pChar, fRangeDiff);
		m_mapRangeCheck.insert(RANGECHECK_VALUE(fRangeDiff, sRangeData));
	}
	else ERR_LOG(LOG_SYSTEM, "fail : NULL == pBot");
}
