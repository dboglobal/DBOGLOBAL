#include "stdafx.h"
#include "EndCondition_HimSelf_LPCheck.h"


CEndCondition_HimSelf_LPCheck::CEndCondition_HimSelf_LPCheck(CNpc* pBot, CAISNodeCondition_HimselfCheck::eAIS_HIMSELF_VALUETYPE eValueType, float fVal)
:CEndCondition(pBot, "CEndCondition_HimSelf_LPCheck")
{
	m_dwTime = 0;
	m_eValueType = eValueType;
	m_fVal = fVal;
}

CEndCondition_HimSelf_LPCheck::~CEndCondition_HimSelf_LPCheck()
{
}


int CEndCondition_HimSelf_LPCheck::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return 1;
}

void CEndCondition_HimSelf_LPCheck::OnExit()
{

}