#include "stdafx.h"
#include "DojoWar.h"
#include "Dojo.h"


CDojoWar::CDojoWar()
{
	Init();
}

CDojoWar::~CDojoWar()
{
	Destroy();
}


void CDojoWar::Init()
{
	m_set_dojoWar.clear();
}

void CDojoWar::Destroy()
{
	m_set_dojoWar.clear();
}


void CDojoWar::AddDojo(CDojo * pDojo)
{
	m_set_dojoWar.insert(pDojo);
}

void CDojoWar::EraseDojo(TBLIDX dojoIdx)
{
	for (std::set<CDojo*>::iterator it = m_set_dojoWar.begin(); it != m_set_dojoWar.end(); it++)
	{
		CDojo* pDojo = *it;

		if (pDojo->GetDojoData()->dojoTblidx == dojoIdx)
		{
			m_set_dojoWar.erase(it);
			return;
		}
	}
}

CDojo * CDojoWar::GetDojo(GUILDID guildId)
{
	for (std::set<CDojo*>::iterator it = m_set_dojoWar.begin(); it != m_set_dojoWar.end(); it++)
	{
		CDojo* pDojo = *it;

		if (pDojo->GetDojoData()->guildId == guildId || pDojo->GetDojoData()->challengeGuildId == guildId)
			return pDojo;
	}

	return nullptr;
}

void CDojoWar::SendDojoWarState(HSESSION hSession)
{
	for (std::set<CDojo*>::iterator it = m_set_dojoWar.begin(); it != m_set_dojoWar.end(); it++)
	{
		CDojo* pDojo = *it;

		if (pDojo->GetDojoWarStatus() == eDBO_DOJO_STATUS_RECEIVE)
		{
			pDojo->SendDojoState(hSession);
		}
	}
}
