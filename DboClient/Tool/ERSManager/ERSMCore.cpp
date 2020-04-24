#include "StdAfx.h"
#include "ERSMCore.h"

ERSMCore* ERSMCore::m_pInstance = NULL;

ERSMCore::ERSMCore(void)
{
    m_pInstance = this;
}

ERSMCore::~ERSMCore(void)
{
}
