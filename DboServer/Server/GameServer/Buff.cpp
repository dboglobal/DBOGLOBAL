#include "stdafx.h"
#include "Buff.h"
#include "NtlPacketGU.h"
#include "CPlayer.h"
#include "ItemPet.h"
#include "TableContainerManager.h"
#include "SystemEffectTable.h"
#include "CommonConfigTable.h"
#include "NtlNavi.h"
#include "battle.h"
#include "CharState.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CBuff::CBuff()
{
	Init();
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CBuff::~CBuff()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuff::Create(CCharacter *pOwnerRef, sBUFF_INFO* pBuffInfo, eSYSTEM_EFFECT_CODE* effectCode, HOBJECT hCaster, eBUFF_TYPE buffType, BYTE byBuffGroup, BYTE* abyEffectType)
{
	if (pOwnerRef)
	{
		memcpy(&m_sBuffInfo, pBuffInfo, sizeof(sBUFF_INFO));

		m_dwRemainTime[0] = m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainTime;
		m_dwRemainTime[1] = m_sBuffInfo.aBuffParameter[1].buffParameter.dwRemainTime;

		if(m_sBuffInfo.aBuffParameter[0].byBuffParameterType != DBO_BUFF_PARAMETER_TYPE_DEFAULT && m_sBuffInfo.aBuffParameter[0].byBuffParameterType != DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_AUTO_RECOVER)
			m_dwRemainTime[0] = UnsignedSafeDecrease<DWORD>(m_sBuffInfo.dwTimeRemaining, NTL_EFFECT_APPLY_INTERVAL);
		if (m_sBuffInfo.aBuffParameter[1].byBuffParameterType != DBO_BUFF_PARAMETER_TYPE_DEFAULT && m_sBuffInfo.aBuffParameter[1].byBuffParameterType != DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_AUTO_RECOVER)
			m_dwRemainTime[1] = UnsignedSafeDecrease<DWORD>(m_sBuffInfo.dwTimeRemaining, NTL_EFFECT_APPLY_INTERVAL);

		if (std::isnan(m_sBuffInfo.aBuffParameter[0].buffParameter.fParameter) || std::isinf(m_sBuffInfo.aBuffParameter[0].buffParameter.fParameter))
			m_sBuffInfo.aBuffParameter[0].buffParameter.fParameter = 0.0f;
		if (std::isnan(m_sBuffInfo.aBuffParameter[1].buffParameter.fParameter) || std::isinf(m_sBuffInfo.aBuffParameter[1].buffParameter.fParameter))
			m_sBuffInfo.aBuffParameter[1].buffParameter.fParameter = 0.0f;

		m_sBuffInfo.bActive = true;
		m_aeEffectCode[0] = effectCode[0];
		m_aeEffectCode[1] = effectCode[1];
		m_pOwnerRef = pOwnerRef;
		m_hCaster = hCaster;
		m_eBuffType = buffType;
		m_byBuffGroup = byBuffGroup;

		m_abyEffectType[0] = abyEffectType[0];
		m_abyEffectType[1] = abyEffectType[1];

		return true;
	}

	Destroy();
	return false;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBuff::Destroy()
{
	m_sBuffInfo.buffIndex = INVALID_BYTE;
	m_pOwnerRef = NULL;
	m_hCaster = INVALID_HOBJECT;
	m_aeEffectCode[0] = INVALID_SYSTEM_EFFECT_CODE;
	m_aeEffectCode[1] = INVALID_SYSTEM_EFFECT_CODE;
	m_eBuffType = BUFF_TYPE_INVALID;
	m_byBuffGroup = INVALID_BYTE;
	m_abyEffectType[0] = INVALID_BYTE;
	m_abyEffectType[1] = INVALID_BYTE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBuff::Init()
{
	m_pOwnerRef = NULL;
	m_hCaster = INVALID_HOBJECT;
	m_aeEffectCode[0] = INVALID_SYSTEM_EFFECT_CODE;
	m_aeEffectCode[1] = INVALID_SYSTEM_EFFECT_CODE;
	m_eBuffType = BUFF_TYPE_INVALID;
	m_byBuffGroup = INVALID_BYTE;
	m_sBuffInfo.bActive = false;
	m_abyEffectType[0] = INVALID_BYTE;
	m_abyEffectType[1] = INVALID_BYTE;

	m_dwRemainTime[0] = INVALID_DWORD;
	m_dwRemainTime[1] = INVALID_DWORD;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuff::OnRegistered(bool bIsLoadedBuff, bool bNeedToDisplayMessage)
{
	if (!bIsLoadedBuff)
	{
		CNtlPacket packet(sizeof(sGU_BUFF_REGISTERED));
		sGU_BUFF_REGISTERED * res = (sGU_BUFF_REGISTERED*)packet.GetPacketData();
		res->wOpCode = GU_BUFF_REGISTERED;
		res->hHandle = m_pOwnerRef->GetID();
		res->bNeedToDisplayMessage = bNeedToDisplayMessage;
		memcpy(&res->buffInfo, &m_sBuffInfo, sizeof(sBUFF_INFO));
		packet.SetPacketLen(sizeof(sGU_BUFF_REGISTERED));
		m_pOwnerRef->Broadcast(&packet);

		OnEffectActive();

		// Check if its a curse buff and if have curse reflecton buff
		if (GetCasterHandle() != INVALID_HOBJECT && IsCurseBuff())
		{
			if (CBuff* pBuff = m_pOwnerRef->GetBuffManager()->FindBuff(ACTIVE_REFLECT_CURSE))
			{
				CCharacter* pCaster = g_pObjectManager->GetChar(GetCasterHandle());
				if (pCaster && pCaster->IsInitialized() && !pCaster->IsFainting())
				{
					if (pCaster->GetStateManager()->IsCharCondition(CHARCOND_INVINCIBLE) == false)
					{
						float fDmg = pBuff->GetSystemEffectParameter(ACTIVE_REFLECT_CURSE);

						pCaster->OnBuffDamage(m_pOwnerRef->GetID(), fDmg);

						pCaster->SendEffectAffected(g_pTableContainer->GetSystemEffectTable()->GetEffectTblidx(ACTIVE_REFLECT_CURSE), pBuff->GetSourceType(), pBuff->GetSourceTblidx(),
							fDmg, INVALID_FLOAT, m_pOwnerRef->GetID());
					}
				}
			}
		}
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuff::OnRemoved(BUFF_REMOVAL_REASON nReason)
{
	CNtlPacket packet(sizeof(sGU_BUFF_DROPPED));
	sGU_BUFF_DROPPED * res = (sGU_BUFF_DROPPED*)packet.GetPacketData();
	res->wOpCode = GU_BUFF_DROPPED;
	res->hHandle = m_pOwnerRef->GetID();
	res->buffIndex = GetBuffIndex();
	res->bySourceType = GetSourceType();

	if (nReason == BUFF_REMOVAL_REASON_REPLACED)
		res->bIsByReplacing = true;
	else
		res->bIsByReplacing = false;

	packet.SetPacketLen(sizeof(sGU_BUFF_DROPPED));
	m_pOwnerRef->Broadcast(&packet);

	OnEffectInActive(nReason != BUFF_REMOVAL_REASON_ETC);

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuff::IsValid()
{
	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBuff::TickProcess(DWORD dwTickDiff)
{
	bool bDoEffect = false;
	int nEffectCnt = 0;

	if (!m_sBuffInfo.bActive)
		return;

	m_sBuffInfo.dwTimeRemaining = UnsignedSafeDecrease<DWORD>(m_sBuffInfo.dwTimeRemaining, dwTickDiff);

	for (int i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
	{
		switch (m_sBuffInfo.aBuffParameter[i].byBuffParameterType)
		{
			case DBO_BUFF_PARAMETER_TYPE_DOT:
			case DBO_BUFF_PARAMETER_TYPE_HOT:
			case DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_STEAL_OVER_TIME:
			{
				if (m_dwRemainTime[i] > m_sBuffInfo.dwTimeRemaining) //check if dot/hot should be applied
				{
					m_dwRemainTime[i] = UnsignedSafeDecrease<DWORD>(m_dwRemainTime[i], NTL_EFFECT_APPLY_INTERVAL); //decrease time by 2 seconds because hot/dot only apply every 2 seconds

					bDoEffect = true;
				}
			}
			break;

			case DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_AUTO_RECOVER:
			{
				m_dwRemainTime[0] = UnsignedSafeDecrease<DWORD>(m_dwRemainTime[0], dwTickDiff);
				m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainTime = m_dwRemainTime[0];

				if (m_dwRemainTime[0] == 0)
					bDoEffect = true;
			}
			break;

			default:
			{
				//++nEffectCnt;
				//if(nEffectCnt == 2) // only do this if both effects are not dot, hot etc
				//	bDoEffect = true;
			}
			break;
		}
	}


	if (bDoEffect)
	{
		if (OnEffect(dwTickDiff)) //Object can die while "OnEffect" and at death all buffs will be removed.
			return;
	}

	if (
		(m_sBuffInfo.dwTimeRemaining == 0 && m_sBuffInfo.aBuffParameter[0].byBuffParameterType != DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_AUTO_RECOVER) //check if normal buff end
		|| (m_sBuffInfo.aBuffParameter[0].byBuffParameterType == DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_AUTO_RECOVER && m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue == 0) //check if auto recover buff end
		)
	{
		m_pOwnerRef->GetBuffManager()->RemoveBuff(GetBuffIndex(), m_eBuffType, BUFF_REMOVAL_REASON_BY_ITSELF);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuff::IsBlessBuff()
{
	return m_eBuffType == BUFF_TYPE_BLESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuff::IsCurseBuff()
{
	return m_eBuffType == BUFF_TYPE_CURSE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuff::IsSubBuff()
{
	return m_eBuffType == BUFF_TYPE_SUB;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuff::IsRestrictionRule(eGAMERULE_TYPE eRuleType)
{
	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBuff::OnEffectActive(bool bUpdateStats/* = true*/)
{
	//update stats
	if(bUpdateStats)
		m_pOwnerRef->GetCharAtt()->CalculateAll();

	for (int i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
	{
		switch (m_aeEffectCode[i])
		{
			case ACTIVE_BLEED:				m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_BLEEDING, NULL, true);			break;
			case ACTIVE_POISON:				m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_POISON, NULL, true);				break;
			case ACTIVE_STOMACHACHE:		m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_STOMACHACHE, NULL, true);		break;
			case ACTIVE_BURN:				m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_BURN, NULL, true);				break;
			case ACTIVE_BARRIER:			m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_BARRIER, NULL, true);			break;
			case ACTIVE_SPINNING_ATTACK:	m_pOwnerRef->SetSpecialSkillTblidx(m_sBuffInfo.sourceTblidx); m_pOwnerRef->SendAspectStateSpinningAttack();		break;
			case ACTIVE_FAKE_DEATH:			m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_FAKE_DEATH, NULL, true);	m_pOwnerRef->GetTargetListManager()->ReleaseAllTargets();	break;
			case ACTIVE_CANT_BE_TARGETTED:	m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_CANT_BE_TARGETTED, NULL, true);	break;
			case ACTIVE_LP_AUTO_RECOVER:	m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_LP_AUTO_RECOVER, NULL, true);	break;
			case ACTIVE_EP_AUTO_RECOVER:	m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_EP_AUTO_RECOVER, NULL, true);	break;
			case ACTIVE_RABIES:				m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_RABIES, NULL, true);				break;
			case ACTIVE_DRUNK:				m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_DRUNK, NULL, true);				break;
			case ACTIVE_ROLLING_ATTACK:		m_pOwnerRef->SetSpecialSkillTblidx(m_sBuffInfo.sourceTblidx); m_pOwnerRef->SendAspectStateRollingAttack();		break;;
			case ACTIVE_EXCITATION_MALE:	m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_EXCITATION_MALE, NULL, true);	break;
			case ACTIVE_INVINCIBLE:			m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_INVINCIBLE, NULL, true);			break;
			case ACTIVE_EXCITATION_FEMALE:	m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_EXCITATION_FEMALE, NULL, true);	break;
			case ACTIVE_TAUNT:				m_pOwnerRef->SetTauntTarget(m_hCaster);														break;
			case ACTIVE_PARALYZE:			m_pOwnerRef->SendCharStateParalyzed();														break;
			case ACTIVE_YELL:
			case ACTIVE_CONFUSE:
			{
				if (sSKILL_TBLDAT* pSkill = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(m_sBuffInfo.sourceTblidx))
				{
					m_pOwnerRef->SetCheckTargetInConfusion(pSkill->bKeep_Effect);
					
					m_pOwnerRef->EnterConfusedState(m_hCaster);
				}
			}
			break;
			case ACTIVE_TERROR:				m_pOwnerRef->EnterTerrorState(m_hCaster); 													break;
			case ACTIVE_STUN:				m_pOwnerRef->SendCharStateStunned(); 														break;
			case ACTIVE_SLEEP:				m_pOwnerRef->SendCharStateSleeping(); 														break;
			case ACTIVE_STONE:				m_pOwnerRef->SendCharStateStunned(DBO_STUN_TYPE_STONE);										break;
			case ACTIVE_CANDY:				m_pOwnerRef->StartCandy();																	break;
			case ACTIVE_TIED_UP:			m_pOwnerRef->SendCharStateStunned(DBO_STUN_TYPE_TIED);										break;
			case ACTIVE_FREEZE:
			{
				if (sSKILL_TBLDAT* pSkill = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(m_sBuffInfo.sourceTblidx))
				{
					m_pOwnerRef->SendCharStateStunned(DBO_STUN_TYPE_FROZEN, m_hCaster, pSkill->bKeep_Effect);
				}
			}
			break;
			case ACTIVE_BAN_CHARGE:			m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_CHARGING_BLOCKED, NULL, true);	break;
			case ACTIVE_WARP_STUN:			m_pOwnerRef->SendCharStateStunned();														break;
			case ACTIVE_TAIYOU_KEN:			m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_TAIYOU_KEN, NULL, true);			break;
			case ACTIVE_BATTLE_INABILITY:	m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_BATTLE_INABILITY, NULL, true);	break;
			case ACTIVE_SKILL_INABILITY:	m_pOwnerRef->GetStateManager()->AddConditionState(CHARCOND_SKILL_INABILITY, NULL, true);	break;
			

			default: break;
		}
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuff::OnEffect(DWORD dwTickDiff)
{
	for (int i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
	{
		switch (m_aeEffectCode[i])
		{
			case ACTIVE_BLEED:
			case ACTIVE_POISON:
			case ACTIVE_STOMACHACHE:
			case ACTIVE_BURN:
			{
				if (m_pOwnerRef->GetStateManager()->IsCharCondition(CHARCOND_INVINCIBLE) == false)
				{
					if (m_pOwnerRef->IsPC() && m_pOwnerRef->GetCurWorld() && m_pOwnerRef->GetCurWorld()->GetNaviInstanceHandle())
					{
						CPlayer* pPc = (CPlayer*)m_pOwnerRef;

						if (GetNaviEngine()->IsBasicAttributeSet(pPc->GetCurWorld()->GetNaviInstanceHandle(), pPc->GetCurLoc().x, pPc->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE) && pPc->IsPvpZone() == false)
							continue;
					}

					m_pOwnerRef->SendEffectAffected(g_pTableContainer->GetSystemEffectTable()->GetEffectTblidx(m_aeEffectCode[i]), GetSourceType(), GetSourceTblidx(),
						m_sBuffInfo.aBuffParameter[i].buffParameter.fParameter, 0.0f, GetCasterHandle());

					if (m_pOwnerRef->OnBuffDamage(m_hCaster, m_sBuffInfo.aBuffParameter[i].buffParameter.fParameter))
						return true; //return true on death

					//recover lp/ep on hit
					m_pOwnerRef->LpEpRecoverOnHit(m_hCaster, m_sBuffInfo.aBuffParameter[i].buffParameter.fParameter);
				}
			}
			break;

			case ACTIVE_HEAL_OVER_TIME:
			{
				m_pOwnerRef->UpdateCurLP((int)m_sBuffInfo.aBuffParameter[i].buffParameter.fParameter, true, false);

				m_pOwnerRef->SendEffectAffected(g_pTableContainer->GetSystemEffectTable()->GetEffectTblidx(m_aeEffectCode[i]), GetSourceType(), GetSourceTblidx(),
					m_sBuffInfo.aBuffParameter[i].buffParameter.fParameter, 0.0f, GetCasterHandle());
			}
			break;

			case ACTIVE_EP_OVER_TIME:
			{
				m_pOwnerRef->UpdateCurEP((WORD)m_sBuffInfo.aBuffParameter[i].buffParameter.fParameter, true, false);

				m_pOwnerRef->SendEffectAffected(g_pTableContainer->GetSystemEffectTable()->GetEffectTblidx(m_aeEffectCode[i]), GetSourceType(), GetSourceTblidx(),
					m_sBuffInfo.aBuffParameter[i].buffParameter.fParameter, 0.0f, GetCasterHandle());
			}
			break;

			case ACTIVE_LP_AUTO_RECOVER:
			{
				//only process if lp 30% or bellow
				if (((CPlayer*)m_pOwnerRef)->GetCombatMode() && m_pOwnerRef->GetCurLP() > 0 && m_pOwnerRef->ConsiderLPLow(30.0f) ) //check lp
				{
				//	sCOMMONCONFIG_VALUE_DATA* commonConfig = g_pTableContainer->GetCommonConfigTable()->FindCommonConfig(m_sBuffInfo.aBuffParameter[0].buffParameter.commonConfigTblidx);

					int recover = m_pOwnerRef->GetMaxLP() - m_pOwnerRef->GetCurLP();

					if (m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue >= (DWORD)recover)
					{
						m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue -= (DWORD)recover;
					}
					else
					{
						recover = (int)m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue;
						m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue = 0;
					}

					m_dwRemainTime[0] = m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainTime = m_sBuffInfo.aBuffParameter[1].buffParameter.dwRemainTime; //set time for next usage possibility

					m_pOwnerRef->UpdateCurLP(recover, true, true); //update lp
					m_pOwnerRef->SendAutoRecoverNfy(m_sBuffInfo.sourceTblidx, m_sBuffInfo.buffIndex, m_sBuffInfo.bySourceType, m_sBuffInfo.aBuffParameter[0].buffParameter.commonConfigTblidx, m_dwRemainTime[0], m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue, recover); //send packet nfy
				}
			}
			break;

			case ACTIVE_EP_AUTO_RECOVER:
			{
				if (m_pOwnerRef->GetCurEP() > 0 && m_pOwnerRef->ConsiderEPLow(30.0f)) //check lp
				{
				//	sCOMMONCONFIG_VALUE_DATA* commonConfig = g_pTableContainer->GetCommonConfigTable()->FindCommonConfig(m_sBuffInfo.aBuffParameter[0].buffParameter.commonConfigTblidx);

					WORD recover = m_pOwnerRef->GetMaxEP() - m_pOwnerRef->GetCurEP();

					if (m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue >= (DWORD)recover)
					{
						m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue -= (DWORD)recover;
					}
					else
					{
						recover = (int)m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue;
						m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue = 0;
					}

					m_dwRemainTime[0] = m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainTime = m_sBuffInfo.aBuffParameter[1].buffParameter.dwRemainTime; //set time for next usage possibility

					m_pOwnerRef->UpdateCurEP(recover, true, true); //update ep
					m_pOwnerRef->SendAutoRecoverNfy(m_sBuffInfo.sourceTblidx, m_sBuffInfo.buffIndex, m_sBuffInfo.bySourceType, m_sBuffInfo.aBuffParameter[0].buffParameter.commonConfigTblidx, m_dwRemainTime[0], m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue, recover); //send packet nfy
				}
			}
			break;

			case ACTIVE_EXCITATION_MALE:
			{
				if (m_pOwnerRef->GetStateManager()->IsCharCondition(CHARCOND_INVINCIBLE) == false)
				{
					sCOMMONCONFIG_VALUE_DATA* commonConfig = g_pTableContainer->GetCommonConfigTable()->FindCommonConfig(m_sBuffInfo.aBuffParameter[i].buffParameter.commonConfigTblidx);
					if (commonConfig)
					{
						if (m_pOwnerRef->GetCurLP() <= (int)commonConfig->adwValue[2]) //if current lp lower or same as deduction value then return
							return true;

						//	CLIENT CRASH WHEN SENDING THE FOLLOWING PACKET
						///	m_pOwnerRef->SendEffectAffected(g_pTableContainer->GetSystemEffectTable()->GetEffectTblidx(m_aeEffectCode[i]), GetSourceType(), GetSourceTblidx(), (float)commonConfig->adwValue[2], INVALID_FLOAT, GetCasterHandle());

						if (m_pOwnerRef->OnBuffDamage(m_hCaster, (float)commonConfig->adwValue[2]))
							return true; //return true on death
					}
				}
			}
			break;
			case ACTIVE_EXCITATION_FEMALE:
			{
				sCOMMONCONFIG_VALUE_DATA* commonConfig = g_pTableContainer->GetCommonConfigTable()->FindCommonConfig(m_sBuffInfo.aBuffParameter[i].buffParameter.commonConfigTblidx);
				if (commonConfig)
				{
				//	CLIENT CRASH WHEN SENDING THE FOLLOWING PACKET
				///	m_pOwnerRef->SendEffectAffected(g_pTableContainer->GetSystemEffectTable()->GetEffectTblidx(m_aeEffectCode[i]), GetSourceType(), GetSourceTblidx(), (float)commonConfig->adwValue[2], INVALID_FLOAT, GetCasterHandle());

					m_pOwnerRef->UpdateCurEP((WORD)commonConfig->adwValue[2], false, false); //update ep
				}
			}
			break;

			case ACTIVE_LP_STEAL_OVER_TIME:
			{
				if (m_pOwnerRef->GetStateManager()->IsCharCondition(CHARCOND_INVINCIBLE) == false)
				{
					if (m_pOwnerRef->IsPC() && m_pOwnerRef->GetCurWorld() && m_pOwnerRef->GetCurWorld()->GetNaviInstanceHandle())
					{
						CPlayer* pPc = (CPlayer*)m_pOwnerRef;

						if (GetNaviEngine()->IsBasicAttributeSet(pPc->GetCurWorld()->GetNaviInstanceHandle(), pPc->GetCurLoc().x, pPc->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE) && pPc->IsPvpZone() == false)
							continue;
					}

					float fDmg = m_sBuffInfo.aBuffParameter[i].buffParameter.fParameter;
					int nRecover = (int)fDmg;
					bool bPercent = m_sBuffInfo.aBuffParameter[i].buffParameter.ACTIVE_LP_EP_STEAL_OVER_TIME_bInPercent;

					if (bPercent == true)
					{
						fDmg = (float)m_pOwnerRef->GetCurLP() * fDmg / 100.f;
						nRecover = (int)fDmg;
					}

					if (nRecover > m_pOwnerRef->GetCurLP())
						nRecover = m_pOwnerRef->GetCurLP();

					if (nRecover > 0)
					{
						if (GetCasterHandle() != INVALID_HOBJECT)
						{
							CCharacter* pCaster = g_pObjectManager->GetChar(GetCasterHandle());
							if (pCaster && pCaster->IsInitialized() && !pCaster->IsFainting())
							{
								if (bPercent == false)
								{
									sSKILL_TBLDAT* pSkillTbldat = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(m_sBuffInfo.sourceTblidx);
									if (pSkillTbldat)
										CalcLifeStealDamage(pCaster, m_pOwnerRef, pSkillTbldat, i, (float)pSkillTbldat->aSkill_Effect_Value[i], fDmg);
								}

								pCaster->UpdateCurLP(nRecover, true, false);
							}

							m_pOwnerRef->SendEffectAffected(g_pTableContainer->GetSystemEffectTable()->GetEffectTblidx(m_aeEffectCode[i]), GetSourceType(), GetSourceTblidx(),
								fDmg, (float)nRecover, GetCasterHandle());
						}

						if (m_pOwnerRef->OnBuffDamage(m_hCaster, fDmg))
							return true; //return true on death
					}
				}
			}
			break;
			case ACTIVE_EP_STEAL_OVER_TIME:
			{
				if (m_pOwnerRef->GetStateManager()->IsCharCondition(CHARCOND_INVINCIBLE) == false)
				{
					if (m_pOwnerRef->IsPC() && m_pOwnerRef->GetCurWorld() && m_pOwnerRef->GetCurWorld()->GetNaviInstanceHandle())
					{
						CPlayer* pPc = (CPlayer*)m_pOwnerRef;

						if (GetNaviEngine()->IsBasicAttributeSet(pPc->GetCurWorld()->GetNaviInstanceHandle(), pPc->GetCurLoc().x, pPc->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE) && pPc->IsPvpZone() == false)
							continue;
					}

					float fDmg = m_sBuffInfo.aBuffParameter[i].buffParameter.fParameter;
					int nRecover = (int)fDmg;
					bool bPercent = m_sBuffInfo.aBuffParameter[i].buffParameter.ACTIVE_LP_EP_STEAL_OVER_TIME_bInPercent;

					if (bPercent == true)
					{
						fDmg = (float)m_pOwnerRef->GetCurEP() * fDmg / 100.f;
						nRecover = (int)fDmg;
					}

					if (nRecover > (int)m_pOwnerRef->GetCurEP())
						nRecover = (int)m_pOwnerRef->GetCurEP();

					if (nRecover > 0)
					{
						m_pOwnerRef->SendEffectAffected(g_pTableContainer->GetSystemEffectTable()->GetEffectTblidx(m_aeEffectCode[i]), GetSourceType(), GetSourceTblidx(),
							fDmg, (float)nRecover, GetCasterHandle());

						if (GetCasterHandle() != INVALID_HOBJECT)
						{
							CCharacter* pCaster = g_pObjectManager->GetChar(GetCasterHandle());
							if (pCaster && pCaster->IsInitialized() && !pCaster->IsFainting())
							{
								pCaster->UpdateCurEP((WORD)nRecover, true, false);
							}
						}

						m_pOwnerRef->UpdateCurEP((WORD)nRecover, false, true);
					}
				}
			}
			break;

			case ACTIVE_MASCOT_VP_AUTO_RECOVERY:
			{
				m_dwRemainTime[0] = m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainTime = m_sBuffInfo.aBuffParameter[1].buffParameter.dwRemainTime; //set time for next usage possibility

				CPlayer* pPlayer = (CPlayer*)m_pOwnerRef;
				if (CItemPet* pMascot = pPlayer->GetCurrentMascot())
				{
					DWORD recover = pMascot->GetMaxVP() - pMascot->GetCurVP();
					if (recover > 0 && pMascot->ConsiderVPLow(30.0f))
					{
						if (m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue >= recover)
						{
							m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue -= recover;
						}
						else
						{
							recover = (int)m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue;
							m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue = 0;
						}

						pMascot->UpdateVP(recover);
					}
				}
			}
			break;

			case ACTIVE_MASCOT_EXP_AUTO_PORTION:
			{
				m_dwRemainTime[0] = m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainTime = m_sBuffInfo.aBuffParameter[1].buffParameter.dwRemainTime; //set time for next usage possibility

				CPlayer* pPlayer = (CPlayer*)m_pOwnerRef;
				if (CItemPet* pMascot = pPlayer->GetCurrentMascot())
				{
					if (pMascot->GetTbldat()->nextMascotTblidx != INVALID_TBLIDX && pMascot->GetExp() < pMascot->GetNeedExp())
					{
						sCOMMONCONFIG_VALUE_DATA* commonConfig = g_pTableContainer->GetCommonConfigTable()->FindCommonConfig(m_sBuffInfo.aBuffParameter[0].buffParameter.commonConfigTblidx);

						m_sBuffInfo.aBuffParameter[0].buffParameter.dwRemainValue -= commonConfig->adwValue[1];

						pMascot->UpdateExp(commonConfig->adwValue[1]);
					}
				}
			}
			break;

			default: break;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBuff::OnEffectInActive(bool bRecalculateStats/* = false*/)
{
	for (int i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
	{
		switch (m_aeEffectCode[i])
		{
			case ACTIVE_BLEED:				m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_BLEEDING, NULL, true);			break;
			case ACTIVE_POISON:				m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_POISON, NULL, true);				break;
			case ACTIVE_STOMACHACHE:		m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_STOMACHACHE, NULL, true);			break;
			case ACTIVE_BURN:				m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_BURN, NULL, true);				break;
			case ACTIVE_BARRIER:			m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_BARRIER, NULL, true);				break;
			case ACTIVE_SPINNING_ATTACK:	m_pOwnerRef->SetSpecialSkillTblidx(INVALID_TBLIDX); m_pOwnerRef->SendAspectStateInvalid();		break;
			case ACTIVE_FAKE_DEATH:			m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_FAKE_DEATH, NULL, false);			break;
			case ACTIVE_CANT_BE_TARGETTED:	m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_CANT_BE_TARGETTED, NULL, true);	break;
			case ACTIVE_LP_AUTO_RECOVER:	m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_LP_AUTO_RECOVER, NULL, true);		break;
			case ACTIVE_EP_AUTO_RECOVER:	m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_EP_AUTO_RECOVER, NULL, true);		break;
			case ACTIVE_RABIES:				m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_RABIES, NULL, true);				break;
			case ACTIVE_DRUNK:				m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_DRUNK, NULL, true);				break;
			case ACTIVE_ROLLING_ATTACK:		m_pOwnerRef->SetSpecialSkillTblidx(INVALID_TBLIDX); m_pOwnerRef->SendAspectStateInvalid();		break;
			case ACTIVE_EXCITATION_MALE:	m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_EXCITATION_MALE, NULL, true);		break;
			case ACTIVE_INVINCIBLE:			m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_INVINCIBLE, NULL, true);			break;
			case ACTIVE_EXCITATION_FEMALE:	m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_EXCITATION_FEMALE, NULL, true);	break;
			case ACTIVE_TAUNT:				m_pOwnerRef->SetTauntTarget(INVALID_HOBJECT);													break;
			case ACTIVE_PARALYZE:			if (m_pOwnerRef->GetCharStateID() == CHARSTATE_PARALYZED) m_pOwnerRef->StandUpIfPossible(CHARSTATE_PARALYZED, ACTIVE_PARALYZE);	break;
			case ACTIVE_YELL:
			case ACTIVE_CONFUSE:
			{
				m_pOwnerRef->SetTargetInConfusion(INVALID_HOBJECT);
				m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_CONFUSED, NULL, true);
			}
			break;
			case ACTIVE_TERROR:				m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_TERROR, NULL, true); 				break;
			case ACTIVE_STUN:				if (m_pOwnerRef->GetCharStateID() == CHARSTATE_STUNNED) m_pOwnerRef->StandUpIfPossible(CHARSTATE_STUNNED, ACTIVE_STUN);	break;
			case ACTIVE_SLEEP:				if (m_pOwnerRef->GetCharStateID() == CHARSTATE_SLEEPING) m_pOwnerRef->StandUpIfPossible(CHARSTATE_SLEEPING);	break;
			case ACTIVE_STONE:
			case ACTIVE_CANDY:				m_pOwnerRef->EndCandy();																		break;
			case ACTIVE_TIED_UP:
			case ACTIVE_FREEZE:
			case ACTIVE_WARP_STUN:			if (m_pOwnerRef->GetCharStateID() == CHARSTATE_STUNNED) m_pOwnerRef->StandUpIfPossible(CHARSTATE_STUNNED, ACTIVE_WARP_STUN);	break;
			case ACTIVE_BAN_CHARGE:			m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_CHARGING_BLOCKED, NULL, true);	break;
			case ACTIVE_TAIYOU_KEN:			m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_TAIYOU_KEN, NULL, true);			break;
			case ACTIVE_BATTLE_INABILITY:	m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_BATTLE_INABILITY, NULL, true);	break;
			case ACTIVE_SKILL_INABILITY:	m_pOwnerRef->GetStateManager()->RemoveConditionState(CHARCOND_SKILL_INABILITY, NULL, true);		break;
			case ACTIVE_AIR_MOVE:
			case ACTIVE_AIR_DASH:
			case ACTIVE_AIR_ACCEL:
			case ACTIVE_AIR_MOVE_DASH_ACCEL: if(m_pOwnerRef->GetAirState() == AIR_STATE_ON) m_pOwnerRef->SendCharStateFalling(NTL_MOVE_NONE);	break;
			default: break;
		}
	}

	if (m_sBuffInfo.bySourceType == DBO_OBJECT_SOURCE_SKILL)
	{
		if (sSKILL_TBLDAT* pSkill = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(m_sBuffInfo.sourceTblidx))
		{
			if (m_pOwnerRef->IsPC() && Dbo_IsHarmfulEffectType(pSkill->bySkill_Active_Type) && pSkill->byApply_Target != DBO_SKILL_APPLY_TARGET_PARTY) // check if debuff
			{
				if(((CPlayer*)m_pOwnerRef)->GetCombatMode())
					((CPlayer*)m_pOwnerRef)->UpdateBattleCombatMode(true); //reset combat mode
			}

			if (pSkill->bKeep_Effect)
			{
				if (m_hCaster != m_pOwnerRef->GetID()) // check if char is not caster.
				{
					CCharacter* pCaster = g_pObjectManager->GetChar(m_hCaster);
					if (pCaster && pCaster->GetCharStateID() == CHARSTATE_KEEPING_EFFECT) // check if caster is in keep_effect state
					{
						pCaster->EraseKeepEffectTarget(m_pOwnerRef->GetID()); // remove char from casters keep_effect map
					}


					if (m_pOwnerRef->GetCharStateID() != CHARSTATE_KEEPING_EFFECT) // check if player is in keeping_effect. If true, then we dont need to try and put him to standing state.
					{
						// go standing if possible
						m_pOwnerRef->StandUpIfPossible(m_pOwnerRef->GetCharStateID());
					}
				}
				else
				{
					if(m_pOwnerRef->GetStateManager()->GetCharState() && m_pOwnerRef->GetStateManager()->GetCharState()->GetNextStateID() != CHARSTATE_KEEPING_EFFECT)
						m_pOwnerRef->StandUpIfPossible(CHARSTATE_KEEPING_EFFECT);
				}
			}
		}
	}

	if (bRecalculateStats)
	{
		m_pOwnerRef->GetCharAtt()->CalculateAll();
	}
}

void CBuff::ActivateBuff(bool bFlag)
{
	CNtlPacket packet(sizeof(sGU_BUFF_ACTIVATE));
	sGU_BUFF_ACTIVATE * res = (sGU_BUFF_ACTIVATE*)packet.GetPacketData();
	res->wOpCode = GU_BUFF_ACTIVATE;
	res->handle = m_pOwnerRef->GetID();

	if (bFlag == false)
	{
		res->sBuffInfo.bActive = bFlag;
		res->sBuffInfo.buffIndex = GetBuffIndex();
		res->sBuffInfo.sourceTblidx = GetSourceTblidx();
		res->sBuffInfo.bySourceType = GetSourceType();
	}
	else
	{
		memcpy(&res->sBuffInfo, GetBuffInfo(), sizeof(sBUFF_INFO));
		res->sBuffInfo.bActive = bFlag;
	}
	
	packet.SetPacketLen(sizeof(sGU_BUFF_ACTIVATE));
	m_pOwnerRef->SendPacket(&packet);

	SetActive(bFlag);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
eSYSTEM_EFFECT_CODE CBuff::GetSystemEffectCode(BYTE byEffectSlot)
{
	if (byEffectSlot < NTL_MAX_EFFECT_IN_SKILL)
		return m_aeEffectCode[byEffectSlot];

	return INVALID_SYSTEM_EFFECT_CODE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BYTE CBuff::GetSystemEffectCodeSlot(eSYSTEM_EFFECT_CODE effectCode)
{
	for (unsigned int i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
	{
		if (m_aeEffectCode[i] == effectCode)
			return (BYTE)i;
	}

	return INVALID_BYTE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BYTE CBuff::GetSystemEffectType(BYTE byEffectSlot)
{
	if (byEffectSlot < NTL_MAX_EFFECT_IN_SKILL)
		return m_abyEffectType[byEffectSlot];

	return INVALID_BYTE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
float CBuff::GetSystemEffectParameter(eSYSTEM_EFFECT_CODE effectCode)
{
	for (unsigned int i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
	{
		if (m_aeEffectCode[i] == effectCode)
			return m_sBuffInfo.aBuffParameter[i].buffParameter.fParameter;
	}

	return 0.0f;
}

bool CBuff::RemoveSubBuffOnDeath()
{
	for (unsigned int i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
	{
		switch (m_aeEffectCode[i])
		{
			case ACTIVE_RABIES:
			case ACTIVE_DRUNK:
			case ACTIVE_EXCITATION_MALE:
			case ACTIVE_EXCITATION_FEMALE:
				return true;

			default: break;
		}
	}

	return false;
}
















