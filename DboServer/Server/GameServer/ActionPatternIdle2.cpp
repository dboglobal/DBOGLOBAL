#include "stdafx.h"
#include "ActionPatternIdle2.h"


CActionPatternIdle2::CActionPatternIdle2()
	:CActionPatternUnit(BOTAP_IDLE2)
{
}

CActionPatternIdle2::~CActionPatternIdle2()
{
}

bool CActionPatternIdle2::Create(CCharacter* pBot)
{
	m_dwAniPlayTime = 2500;
	return true;
}