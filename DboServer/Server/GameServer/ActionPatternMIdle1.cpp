#include "stdafx.h"
#include "ActionPatternMIdle1.h"
#include "char.h"


CActionPatternMIdle1::CActionPatternMIdle1()
	:CActionPatternUnit(BOTAP_MIDLE1)
{
}

CActionPatternMIdle1::~CActionPatternMIdle1()
{
}

bool CActionPatternMIdle1::Create(CCharacter* pBot)
{
	if(pBot)
		m_dwAniPlayTime = (DWORD)pBot->GetAniTbldat()->GetAnimationLength(BOTAP_MIDLE1);
	else
	{
		ERR_LOG(LOG_BOTAI, "pBot == NULL");
		return false;
	}

	return true;
}