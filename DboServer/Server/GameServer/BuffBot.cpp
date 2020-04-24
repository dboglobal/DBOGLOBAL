#include "stdafx.h"
#include "BuffBot.h"
#include "Npc.h"


CBuffBot::CBuffBot()
{
	m_pBotRef = NULL;
}

CBuffBot::~CBuffBot()
{
	m_pBotRef = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuffBot::Create(CNpc *pOwnerRef, sBUFF_INFO* pBuffInfo, eSYSTEM_EFFECT_CODE* effectCode, HOBJECT hCaster, eBUFF_TYPE buffType, BYTE byBuffGroup, BYTE* abyEffectType)
{
	if (CBuff::Create(pOwnerRef, pBuffInfo, effectCode, hCaster, buffType, byBuffGroup, abyEffectType))
	{
		m_pBotRef = pOwnerRef;
		return true;
	}

	return false;
}
