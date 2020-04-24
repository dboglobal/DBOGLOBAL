#include "stdafx.h"
#include "ActionPatternMove.h"


CActionPatternMove::CActionPatternMove()
:CActionPatternUnit(BOTAP_MOVE)
{
}

CActionPatternMove::~CActionPatternMove()
{
}

bool CActionPatternMove::Create(CCharacter* pBot)
{
	m_dwAniPlayTime = 0;
	return true;
}