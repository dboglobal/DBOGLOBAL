#include "stdafx.h"
#include "MeHaterManager.h"


CMeHaterManager::CMeHaterManager()
{
}

CMeHaterManager::~CMeHaterManager()
{
}


bool CMeHaterManager::Create(CCharacter* pChar)
{
	if (pChar)
	{
		m_pChar = pChar;
		return m_cContainerMeHater.Create(pChar, this);
	}

	ERR_LOG(LOG_SYSTEM, "fail : NULL == pBot");
	return false;
}


void CMeHaterManager::Destroy()
{
	m_cContainerMeHater.Destroy();
}


void CMeHaterManager::Init()
{
	m_cContainerMeHater.Destroy();
}

