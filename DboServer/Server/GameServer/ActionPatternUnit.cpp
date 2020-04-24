#include "stdafx.h"
#include "ActionPatternUnit.h"


CActionPatternUnit::CActionPatternUnit(eACTIONPATTERNID eActionPatternID)
{
	this->m_eActionPatternID = eActionPatternID;
	this->m_dwAniPlayTime = 0;
}

CActionPatternUnit::~CActionPatternUnit()
{
	
}

