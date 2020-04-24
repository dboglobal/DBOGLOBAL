#include "stdafx.h"
#include "DojoWar.h"
#include "DojoManager.h"


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

CDojo * CDojoWar::GetDojo(GUILDID guildId)
{
	for (std::set<CDojo*>::iterator it = m_set_dojoWar.begin(); it != m_set_dojoWar.end(); it++)
	{
		CDojo* pDojo = *it;

		if (pDojo->GetGuildID() == guildId || pDojo->GetAttGuildID() == guildId)
			return pDojo;
	}

	return nullptr;
}

void CDojoWar::RemoveDojo(TBLIDX dojoTblidx)
{
	for (std::set<CDojo*>::iterator it = m_set_dojoWar.begin(); it != m_set_dojoWar.end(); it++)
	{
		CDojo* pDojo = *it;

		if (pDojo->GetDojoTblidx() == dojoTblidx)
		{
			m_set_dojoWar.erase(it);
			return;
		}
	}
}
