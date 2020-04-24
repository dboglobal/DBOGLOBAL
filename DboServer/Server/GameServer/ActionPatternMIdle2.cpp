#include "stdafx.h"
#include "ActionPatternMIdle2.h"
#include "char.h"


CActionPatternMIdle2::CActionPatternMIdle2()
	:CActionPatternUnit(BOTAP_MIDLE2)
{
}

CActionPatternMIdle2::~CActionPatternMIdle2()
{
}

bool CActionPatternMIdle2::Create(CCharacter* pBot)
{
	if (pBot)
		m_dwAniPlayTime = pBot->GetAniTbldat()->GetAnimationLength(BOTAP_MIDLE1);
	else
	{
		ERR_LOG(LOG_BOTAI, "pBot == NULL");
		return false;
	}

	return true;
}