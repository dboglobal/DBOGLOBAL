#include "stdafx.h"
#include "ContainerMeHater.h"


CContainerMeHater::CContainerMeHater()
{
}

CContainerMeHater::~CContainerMeHater()
{
}


bool CContainerMeHater::Create(CCharacter* pChar, CMeHaterManager* pMeHaterManager)
{
	if (pChar)
	{
		if (pMeHaterManager)
		{
			m_pChar = pChar;
			m_pMeHaterManager = pMeHaterManager;
			return true;
		}
	}

	return false;
}


void CContainerMeHater::Destroy()
{
	AllDeleteMeHater();
}


bool CContainerMeHater::AddMeHater(HOBJECT hMeHater)
{
	if (hMeHater == INVALID_HOBJECT)
	{
		ERR_LOG(LOG_SYSTEM, "fail : INVALID_HOBJECT == hMeHater");
	}
	else
	{
		MEHATER_MAP::iterator it = m_mapMeHater.find(hMeHater);
		if (it == m_mapMeHater.end())
		{
			m_mapMeHater.insert(MEHATER_VALUE(hMeHater, hMeHater));
			return true;
		}
	}

	return false;
}


bool CContainerMeHater::ModefiMeHater(HOBJECT hMeHater, HOBJECT hModifyObj)
{
	DeleteMeHater(hMeHater);
	return AddMeHater(hModifyObj);
}


void CContainerMeHater::DeleteMeHater(HOBJECT hMeHater)
{
	MEHATER_MAP::iterator it = m_mapMeHater.find(hMeHater);
	if (it != m_mapMeHater.end())
	{
		m_mapMeHater.erase(hMeHater);
	}
}


void CContainerMeHater::AllDeleteMeHater()
{
	m_mapMeHater.clear();
}


bool CContainerMeHater::IsMeHater()
{
	return m_mapMeHater.empty() == false;
}


int CContainerMeHater::GetMeHaterCount()
{
	return (int)m_mapMeHater.size();
}
