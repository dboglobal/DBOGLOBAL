#include "stdafx.h"
#include "Npc.h"
#include "BuffBot.h"
#include "TableContainerManager.h"
#include "SystemEffectTable.h"


CBuffManagerBot::CBuffManagerBot()
{
	m_pBotRef = NULL;
}

CBuffManagerBot::~CBuffManagerBot()
{
	m_pBotRef = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuffManagerBot::Create(CNpc *pOwnerRef)
{
	if (CBuffManager::Create(pOwnerRef))
	{
		m_pBotRef = pOwnerRef;
		return true;
	}

	return false;
}


bool CBuffManagerBot::RegisterBuff(DWORD& rdwKeepTime, eSYSTEM_EFFECT_CODE* effectCode, sDBO_BUFF_PARAMETER * paBuffParameter, HOBJECT hCaster, eBUFF_TYPE buffType, sSKILL_TBLDAT* pSkillTbldat, BYTE* prBuffIndex)
{
	if (CBuffManager::RegisterBuff(rdwKeepTime, effectCode, paBuffParameter, hCaster, buffType, pSkillTbldat, prBuffIndex))
	{
		if (rdwKeepTime > 0)
		{
			CBuffBot* pBuff = new CBuffBot;

			sBUFF_INFO buffInfo;
			buffInfo.buffIndex = INVALID_BYTE;
			buffInfo.sourceTblidx = pSkillTbldat->tblidx;
			buffInfo.dwTimeRemaining = rdwKeepTime;
			buffInfo.dwInitialDuration = rdwKeepTime;
			buffInfo.bySourceType = DBO_OBJECT_SOURCE_SKILL;
			memcpy(&buffInfo.aBuffParameter[0], &paBuffParameter[0], sizeof(sDBO_BUFF_PARAMETER));
			memcpy(&buffInfo.aBuffParameter[1], &paBuffParameter[1], sizeof(sDBO_BUFF_PARAMETER));

			if (pBuff->Create(m_pBotRef, &buffInfo, effectCode, hCaster, buffType, pSkillTbldat->byBuff_Group, pSkillTbldat->bySkill_Effect_Type))
			{
				if (!AddBuff(pBuff, true))
				{
					SAFE_DELETE(pBuff);
					return false;
				}

				if (prBuffIndex)
					*prBuffIndex = pBuff->GetBuffIndex();
			}
			else
			{
				SAFE_DELETE(pBuff);
				return false;
			}
		}

		return true;
	}

	return false;
}

