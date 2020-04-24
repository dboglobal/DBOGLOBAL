#include "stdafx.h"
#include "ActionPatternSystem.h"


CActionPatternSystem::CActionPatternSystem()
{
}

CActionPatternSystem::~CActionPatternSystem()
{
	Destroy();
}


bool CActionPatternSystem::Create()
{
	return m_cContainerActionPatternSet.Create();
}

void CActionPatternSystem::Destroy()
{

}








