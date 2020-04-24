#include "stdafx.h"
#include "CPlayer.h"
#include "BuffPc.h"
#include "NtlResultCode.h"
#include "NtlPacketGQ.h"
#include "TableContainerManager.h"
#include "SystemEffectTable.h"
#include "ItemTable.h"
#include "UseItemTable.h"
#include "BuffLinkedObject.h"
#include "GameServer.h"


CBuffManagerPc::CBuffManagerPc()
{
	m_pPlayerRef = NULL;
}

CBuffManagerPc::~CBuffManagerPc()
{
	m_pPlayerRef = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuffManagerPc::Create(CPlayer *pOwnerRef)
{
	if (CBuffManager::Create(pOwnerRef))
	{
		m_pPlayerRef = pOwnerRef;
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	: Register buff from database
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuffManagerPc::RegisterLoadBuff(sBUFF_INFO & rBuffInfo, BYTE byBuffGroup)
{
	CBuffPc* pBuff = new CBuffPc;

	eBUFF_TYPE buffType = BUFF_TYPE_INVALID;
	eSYSTEM_EFFECT_CODE aeEffectCode[NTL_MAX_EFFECT_IN_SKILL];
	BYTE abyEffectType[NTL_MAX_EFFECT_IN_SKILL];

	if (rBuffInfo.bySourceType == DBO_OBJECT_SOURCE_SKILL)
	{
		sSKILL_TBLDAT* pSkillTbldat = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(rBuffInfo.sourceTblidx);
		if (pSkillTbldat)
		{
			aeEffectCode[0] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pSkillTbldat->skill_Effect[0]);
			aeEffectCode[1] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pSkillTbldat->skill_Effect[1]);

			if (Dbo_IsForBlessBuff(pSkillTbldat->bySkill_Active_Type))
				buffType = BUFF_TYPE_BLESS;
			else
				buffType = BUFF_TYPE_CURSE;

			abyEffectType[0] = pSkillTbldat->bySkill_Effect_Type[0];
			abyEffectType[1] = pSkillTbldat->bySkill_Effect_Type[1];
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "COULD NOT FIND SKILL %u !!!!", rBuffInfo.sourceTblidx);
			return false;
		}
	}
	else
	{
		buffType = BUFF_TYPE_SUB;

		sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(rBuffInfo.sourceTblidx);
		if (pItemTbldat)
		{
			sUSE_ITEM_TBLDAT* pUseItemTbldat = (sUSE_ITEM_TBLDAT*)g_pTableContainer->GetUseItemTable()->FindData(pItemTbldat->Use_Item_Tblidx);
			if (pUseItemTbldat)
			{
				aeEffectCode[0] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pUseItemTbldat->aSystem_Effect[0]);
				aeEffectCode[1] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pUseItemTbldat->aSystem_Effect[1]);

				abyEffectType[0] = pUseItemTbldat->abySystem_Effect_Type[0];
				abyEffectType[1] = pUseItemTbldat->abySystem_Effect_Type[1];
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "COULD NOT FIND USE-ITEM %u !!!!", rBuffInfo.sourceTblidx);
				return false;
			}
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "COULD NOT FIND ITEM %u !!!!", rBuffInfo.sourceTblidx);
			return false;
		}
	}

	//set parameter type
	for (int i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
	{
		if (aeEffectCode[i] == ACTIVE_HEAL_OVER_TIME || aeEffectCode[i] == ACTIVE_EP_OVER_TIME)
		{
			rBuffInfo.aBuffParameter[i].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_HOT;
		}
		else if (aeEffectCode[i] >= ACTIVE_BLEED && aeEffectCode[i] <= ACTIVE_BURN)
		{
			rBuffInfo.aBuffParameter[i].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DOT;
		}
		else if (aeEffectCode[i] == ACTIVE_LP_AUTO_RECOVER || aeEffectCode[i] == ACTIVE_EP_AUTO_RECOVER || 
			aeEffectCode[i] == ACTIVE_MASCOT_VP_AUTO_RECOVERY || aeEffectCode[i] == ACTIVE_MASCOT_EXP_AUTO_PORTION)
		{
			rBuffInfo.aBuffParameter[i].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_AUTO_RECOVER;
		}
		else if (aeEffectCode[i] == ACTIVE_LP_STEAL_OVER_TIME || aeEffectCode[i] == ACTIVE_EP_STEAL_OVER_TIME)
		{
			rBuffInfo.aBuffParameter[i].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_STEAL_OVER_TIME;
		}
		else if (aeEffectCode[i] == ACTIVE_EXCITATION_MALE || aeEffectCode[i] == ACTIVE_EXCITATION_FEMALE)
		{
			rBuffInfo.aBuffParameter[i].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DOT;
		}
		else
			rBuffInfo.aBuffParameter[i].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DEFAULT;
	}

	if (pBuff->Create(m_pPlayerRef, &rBuffInfo, aeEffectCode, INVALID_HOBJECT, buffType, byBuffGroup, abyEffectType))
	{
		if (!LoadBuff(pBuff))
		{
			SAFE_DELETE(pBuff);
			return false;
		}

		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	: Register a new Item buff
//		Return	: true/false
//-----------------------------------------------------------------------------------
bool CBuffManagerPc::RegisterSubBuff(sBUFF_INFO* pBuffInfo, eSYSTEM_EFFECT_CODE* effectCode, HOBJECT hCaster, BYTE byBuffGroup, WORD & rwResultCode, BYTE* abyEffectType)
{
	if (CBuffManager::RegisterSubBuff(pBuffInfo, effectCode, hCaster, byBuffGroup, rwResultCode, abyEffectType))
	{
		CBuffPc* pBuff = new CBuffPc;
		if (pBuff->Create(m_pPlayerRef, pBuffInfo, effectCode, hCaster, BUFF_TYPE_SUB, byBuffGroup, abyEffectType))
		{
			if (!AddBuff(pBuff, true))
			{
				rwResultCode = GAME_FAIL;
				SAFE_DELETE(pBuff);
				return false;
			}
		}
		else
		{
			rwResultCode = GAME_FAIL;
			SAFE_DELETE(pBuff);
			return false;
		}

		return true;
	}

	return false;
}

bool CBuffManagerPc::RegisterBuff(DWORD& rdwKeepTime, eSYSTEM_EFFECT_CODE * effectCode, sDBO_BUFF_PARAMETER * paBuffParameter, HOBJECT hCaster, eBUFF_TYPE buffType, sSKILL_TBLDAT * pSkillTbldat, BYTE* prBuffIndex)
{
	if (CBuffManager::RegisterBuff(rdwKeepTime, effectCode, paBuffParameter, hCaster, buffType, pSkillTbldat, prBuffIndex))
	{
		if (rdwKeepTime > 0)
		{
			CBuffPc* pBuff = new CBuffPc;

			sBUFF_INFO buffInfo;
			buffInfo.buffIndex = INVALID_BYTE;
			buffInfo.sourceTblidx = pSkillTbldat->tblidx;
			buffInfo.dwTimeRemaining = rdwKeepTime;
			buffInfo.dwInitialDuration = rdwKeepTime;
			buffInfo.bySourceType = DBO_OBJECT_SOURCE_SKILL;
			memcpy(&buffInfo.aBuffParameter[0], &paBuffParameter[0], sizeof(sDBO_BUFF_PARAMETER));
			memcpy(&buffInfo.aBuffParameter[1], &paBuffParameter[1], sizeof(sDBO_BUFF_PARAMETER));

			if (pBuff->Create(m_pPlayerRef, &buffInfo, effectCode, hCaster, buffType, pSkillTbldat->byBuff_Group, pSkillTbldat->bySkill_Effect_Type))
			{
				if (!AddBuff(pBuff, true))
				{
					SAFE_DELETE(pBuff);
					return false;
				}

				if(buffType == BUFF_TYPE_CURSE && hCaster != INVALID_HOBJECT)
					m_pPlayerRef->UpdateBattleCombatMode(true);//Start/Reset combat event

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


//-----------------------------------------------------------------------------------
//		Purpose	: Register a new Item buff
//		Return	: true/false
//-----------------------------------------------------------------------------------
void CBuffManagerPc::_SaveBuffCooldowns()
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGQ_SAVE_BUFF_DATA_REQ));
	sGQ_SAVE_BUFF_DATA_REQ* res = (sGQ_SAVE_BUFF_DATA_REQ*)packet.GetPacketData();
	res->wOpCode = GQ_SAVE_BUFF_DATA_REQ;
	res->handle = m_pPlayerRef->GetID();
	res->charId = m_pPlayerRef->GetCharID();
	res->byBuffCount = 0;

	for (CBuffLinkedObject* pBuffLink = (CBuffLinkedObject*)m_listBuffOrderedByRegisteredTime.GetFirst();
		pBuffLink;
		pBuffLink = (CBuffLinkedObject*)pBuffLink->GetNext())
	{
		CBuff* pBuff = pBuffLink->GetBuffRef();
		if (pBuff)
		{
			bool bStore = true;

			if (pBuff->GetSourceType() == DBO_OBJECT_SOURCE_SKILL)
			{
				if (sSKILL_TBLDAT* pSkill = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(pBuff->GetSourceTblidx()))
				{
					if (pSkill->bKeep_Effect) //dont store buffs which keep effect like "playful doll" in db 
					{
						bStore = false;
					}
				}
			}

			if (bStore)
			{
				sBUFF_INFO* info = pBuff->GetBuffInfo();

				res->asBuff[res->byBuffCount].sourceTblidx = pBuff->GetSourceTblidx();
				res->asBuff[res->byBuffCount].buffIndex = pBuff->GetBuffIndex();
				res->asBuff[res->byBuffCount].bySourceType = pBuff->GetSourceType();
				res->asBuff[res->byBuffCount].dwInitialDuration = info->dwInitialDuration;
				res->asBuff[res->byBuffCount].dwTimeRemaining = info->dwTimeRemaining;
				res->asBuff[res->byBuffCount].anArgument1[0] = info->aBuffParameter[0].buffParameter.commonConfigTblidx;
				res->asBuff[res->byBuffCount].anArgument2[0] = info->aBuffParameter[1].buffParameter.commonConfigTblidx;
				res->asBuff[res->byBuffCount].anArgument1[1] = pBuff->GetRemainTime(0);
				res->asBuff[res->byBuffCount].anArgument2[1] = pBuff->GetRemainTime(1);
				res->asBuff[res->byBuffCount].anArgument1[2] = info->aBuffParameter[0].buffParameter.dwRemainValue;
				res->asBuff[res->byBuffCount].anArgument2[2] = info->aBuffParameter[1].buffParameter.dwRemainValue;
				res->asBuff[res->byBuffCount].effectValue1 = info->aBuffParameter[0].buffParameter.fParameter;
				res->asBuff[res->byBuffCount].effectValue1 = info->aBuffParameter[1].buffParameter.fParameter;
				res->asBuff[res->byBuffCount].byBuffGroup = pBuff->GetBuffGroup();
				res->asBuff[res->byBuffCount++].bIsDBUpdateFiltering = false;
			}
		}
	}

	if (res->byBuffCount > 0)
	{
		packet.SetPacketLen(sizeof(sGQ_SAVE_BUFF_DATA_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);
	}
}

void CBuffManagerPc::CheckSubBuffs(BYTE byWorldRuleType)
{
	unsigned int pos = m_subBuffList.Begin();
	while (pos != m_subBuffList.End())
	{
		CBuff* pBuff = m_subBuffList.GetAt(pos);
		if (pBuff)
		{
			sITEM_TBLDAT * pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(pBuff->GetSourceTblidx());
			if (pItemTbldat)
			{
				sUSE_ITEM_TBLDAT * pUseItemTbldat = (sUSE_ITEM_TBLDAT*)g_pTableContainer->GetUseItemTable()->FindData(pItemTbldat->Use_Item_Tblidx);
				if (pUseItemTbldat)
				{
					if (pBuff->GetBuffInfo()->bActive) //check if buff is active
					{
						if (byWorldRuleType == GAMERULE_CCBATTLEDUNGEON && BIT_FLAG_TEST(pUseItemTbldat->dwUse_Restriction_Rule_Bit_Flag, MAKE_BIT_FLAG(GAMERULE_NORMAL)) == false)
							pBuff->ActivateBuff(false);
						else if (pUseItemTbldat->dwUse_Restriction_Rule_Bit_Flag > 0 && BIT_FLAG_TEST(pUseItemTbldat->dwUse_Restriction_Rule_Bit_Flag, MAKE_BIT_FLAG(byWorldRuleType))) //check if buff has to be disabled
						{
							pBuff->ActivateBuff(false);
						}
					}
					else if (pBuff->GetSystemEffectCode(0) != ACTIVE_LP_AUTO_RECOVER && pBuff->GetSystemEffectCode(0) != ACTIVE_EP_AUTO_RECOVER)
					{

						if (pUseItemTbldat->dwUse_Restriction_Rule_Bit_Flag == 0 || BIT_FLAG_TEST(pUseItemTbldat->dwUse_Restriction_Rule_Bit_Flag, MAKE_BIT_FLAG(byWorldRuleType) == false)) //check if buff has to be activated
						{
							pBuff->ActivateBuff(true);
						}
					}
				}
			}
		}

		pos = m_subBuffList.Next(pos);
	}
}

void CBuffManagerPc::RemoveBlessBuffsFromCaster(HOBJECT hCaster)
{
	int nRemoveCount = 0;

	unsigned int pos = m_blessBuffList.Begin();
	while (pos != m_blessBuffList.End())
	{
		CBuff* pBuff = m_blessBuffList.GetAt(pos);
		if (pBuff && pBuff->GetCasterHandle() == hCaster)
		{
			pos = RemoveBuff_Internal(CBuff::BUFF_REMOVAL_REASON_ETC, &m_blessBuffList, pos);

			++nRemoveCount;
		}
		else
			pos = m_blessBuffList.Next(pos);
	}

	if (nRemoveCount > 0)
		m_pPlayerRef->GetCharAtt()->CalculateAll();
}
