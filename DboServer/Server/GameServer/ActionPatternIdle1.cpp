#include "stdafx.h"
#include "ActionPatternIdle1.h"


CActionPatternIdle1::CActionPatternIdle1()
	:CActionPatternUnit(BOTAP_IDLE1)
{
}

CActionPatternIdle1::~CActionPatternIdle1()
{
}

bool CActionPatternIdle1::Create(CCharacter* pBot)
{
	m_dwAniPlayTime = 1500;
	return true;
}