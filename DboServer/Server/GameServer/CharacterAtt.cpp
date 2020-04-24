#include "stdafx.h"
#include "NtlBitFlagManager.h"
#include "NtlPacketGU.h"
#include "GameServer.h"
#include "char.h"
#include "SystemEffectTable.h"

#include "calcs.h"



CCharacterAtt::CCharacterAtt()
{
	Init();
}

CCharacterAtt::~CCharacterAtt()
{
}


bool CCharacterAtt::Create(CCharacter* pChar)
{
	m_pOwnerRef = pChar;
	return true;
}


void CCharacterAtt::Init()
{
	m_pOwnerRef = NULL;
	memset(&m_pAttribute, 0, sizeof(sAVATAR_ATTRIBUTE));
	m_byBattle_Attribute = BATTLE_ATTRIBUTE_NONE;

	m_wSubWeaponPhysicalOffence = 0;
	m_wSubWeaponEnergyOffence = 0;
}

void CCharacterAtt::CalculateAll()
{
	//set backup
	m_fRunSpeedBackup = m_pAttribute.fRunSpeed;
	m_wAttackSpeedBackup = m_pAttribute.wAttackSpeedRate;
	m_fCurLpPercent = m_pOwnerRef->GetCurLpInPercent();
	m_fCurEpPercent = m_pOwnerRef->GetCurEpInPercent();
	m_fCurApPercent = m_pOwnerRef->GetCurApInPercent();

	//set to zero to avoid bug that stats keep increasing instead
	memset(&m_percentValue, 0, sizeof(m_percentValue));

	//set to zero to avoid bug that stats keep increasing instead
	memset(&m_pAttribute, 0, sizeof(sAVATAR_ATTRIBUTE));

	//calculate the stats
	CalculateAtt();


	//check if our LP changed
	if (m_fCurLpPercent > 0.0f && m_fCurLpPercent < 101.0f && m_fCurLpPercent != m_pOwnerRef->GetCurLpInPercent())
	{
		m_pOwnerRef->SetCurLP(int((float)m_pAttribute.MaxLp * m_fCurLpPercent / 100.f));

		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_LP));
		sGU_UPDATE_CHAR_LP * res = (sGU_UPDATE_CHAR_LP *)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_LP;
		res->handle = m_pOwnerRef->GetID();
		res->dwLpEpEventId = INVALID_DWORD;
		res->curLp = m_pOwnerRef->GetCurLP();
		res->maxLp = m_pAttribute.MaxLp;
		m_pOwnerRef->Broadcast(&packet);
	}

	//check if our EP changed
	if (m_fCurEpPercent > 0.0f && m_fCurEpPercent < 101.0f && m_fCurEpPercent != m_pOwnerRef->GetCurEpInPercent())
	{
		m_pOwnerRef->SetCurEP(WORD((float)m_pAttribute.wMaxEP * m_fCurEpPercent / 100.f));

		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_EP));
		sGU_UPDATE_CHAR_EP * res = (sGU_UPDATE_CHAR_EP *)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_EP;
		res->handle = m_pOwnerRef->GetID();
		res->dwLpEpEventId = INVALID_DWORD;
		res->wCurEP = m_pOwnerRef->GetCurEP();
		res->wMaxEP = m_pAttribute.wMaxEP;
		m_pOwnerRef->Broadcast(&packet);
	}

	//check if our AP changed
	if (m_fCurApPercent > 0.0f && m_fCurApPercent < 101.0f && m_fCurApPercent != m_pOwnerRef->GetCurApInPercent())
	{
		m_pOwnerRef->SetCurAP(int((float)m_pAttribute.MaxAp * m_fCurApPercent / 100.f));

		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_AP));
		sGU_UPDATE_CHAR_AP * res = (sGU_UPDATE_CHAR_AP *)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_AP;
		res->handle = m_pOwnerRef->GetID();
		res->curAP = m_pOwnerRef->GetCurAP();
		res->maxAP = m_pAttribute.MaxAp;
		m_pOwnerRef->SendPacket(&packet);
	}
}

void CCharacterAtt::CalculateAtt()
{
	if (m_pOwnerRef->IsPC())
	{
		sAVATAR_ATTRIBUTE_LINK& newavt = CNtlAvatar::GetInstance()->ConvertAVATAR_ATTRIBUTE(&m_pAttribute);
		CNtlBitFlagManager flagMgr;

		if (false == flagMgr.Create(&m_pAttribute, ATTRIBUTE_TO_UPDATE_COUNT))
		{
			ERR_LOG(LOG_USER, "CalculateAtt: flagMgr.Create == FALSE.");
			return;
		}

		for (DWORD dwIndex = ATTRIBUTE_TO_UPDATE_FIRST; dwIndex < ATTRIBUTE_TO_UPDATE_COUNT; dwIndex++)
			flagMgr.Set(dwIndex);

		DWORD datasize = 0;

		CNtlPacket packet(sizeof(sGU_AVATAR_ATTRIBUTE_UPDATE));
		sGU_AVATAR_ATTRIBUTE_UPDATE * res = (sGU_AVATAR_ATTRIBUTE_UPDATE *)packet.GetPacketData();
		res->wOpCode = GU_AVATAR_ATTRIBUTE_UPDATE;
		res->hHandle = m_pOwnerRef->GetID();
		res->byAttributeTotalCount = ATTRIBUTE_TO_UPDATE_COUNT;

		if (CNtlAvatar::GetInstance()->SaveAvatarAttribute(&flagMgr, &newavt, res->abyFlexibleField, &datasize) == false)
		{
			ERR_LOG(LOG_USER, "CalculateAtt: save avatar attribute failed ");
			return;
		}

		packet.SetPacketLen(sizeof(sGU_AVATAR_ATTRIBUTE_UPDATE));
		m_pOwnerRef->Broadcast(&packet);
	}

	if (m_fRunSpeedBackup != m_pAttribute.fRunSpeed)
	{
		m_pOwnerRef->UpdateMoveSpeed(GetWalkSpeed(), m_pAttribute.fRunSpeed);

		if (m_pAttribute.fRunSpeed <= 0.0f)
		{
			if(m_pOwnerRef->GetCharStateID() == CHARSTATE_DASH_PASSIVE)
			{
				m_pOwnerRef->SendCharStateStanding();
			}
			else if (m_pOwnerRef->GetCharStateID() == CHARSTATE_SKILL_AFFECTING && m_pOwnerRef->GetMoveFlag() == NTL_MOVE_FLAG_DASH)
			{
				m_pOwnerRef->SendCharStateStanding();
			}
		}
	}

	if (m_wAttackSpeedBackup != m_pAttribute.wAttackSpeedRate)
	{
		m_pOwnerRef->UpdateAttackSpeed(m_pAttribute.wAttackSpeedRate);
	}
}

void CCharacterAtt::CalculatePercentValues()
{
	if (m_percentValue.m_fSTR != 0.f)
		CalculateStr(NtlRound((float)m_pAttribute.Str * m_percentValue.m_fSTR / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fSTRNegative != 0.f)
		CalculateStr(NtlRound(fabs((float)m_pAttribute.Str * m_percentValue.m_fSTRNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fCON != 0.f)
		CalculateCon(NtlRound((float)m_pAttribute.Con * m_percentValue.m_fCON / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fCONNegative != 0.f)
		CalculateCon(NtlRound(fabs((float)m_pAttribute.Con * m_percentValue.m_fCONNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fFOC != 0.f)
		CalculateFoc(NtlRound((float)m_pAttribute.Foc * m_percentValue.m_fFOC / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fFOCNegative != 0.f)
		CalculateFoc(NtlRound(fabs((float)m_pAttribute.Foc * m_percentValue.m_fFOCNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fDEX != 0.f)
		CalculateDex(NtlRound((float)m_pAttribute.Dex * m_percentValue.m_fDEX / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fDEXNegative != 0.f)
		CalculateDex(NtlRound(fabs((float)m_pAttribute.Dex * m_percentValue.m_fDEXNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fSOL != 0.f)
		CalculateSol(NtlRound((float)m_pAttribute.Sol * m_percentValue.m_fSOL / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fSOLNegative != 0.f)
		CalculateSol(NtlRound(fabs((float)m_pAttribute.Sol * m_percentValue.m_fSOLNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fENG != 0.f)
		CalculateEng(NtlRound((float)m_pAttribute.Eng * m_percentValue.m_fENG / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fENGNegative != 0.f)
		CalculateEng(NtlRound(fabs((float)m_pAttribute.Eng * m_percentValue.m_fENGNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fLP != 0.f)
		CalculateMaxLP(NtlRound((float)m_pAttribute.MaxLp * m_percentValue.m_fLP / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fLPNegative != 0.f)
		CalculateMaxLP(NtlRound(fabs((float)m_pAttribute.MaxLp * m_percentValue.m_fLPNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEP != 0.f)
		CalculateMaxEP(NtlRound((float)m_pAttribute.wMaxEP * m_percentValue.m_fEP / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEPNegative != 0.f)
		CalculateMaxEP(NtlRound(fabs((float)m_pAttribute.wMaxEP * m_percentValue.m_fEPNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fAP != 0.f)
		CalculateMaxAP(NtlRound((float)m_pAttribute.MaxAp * m_percentValue.m_fAP / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fAPNegative != 0.f)
		CalculateMaxAP(NtlRound(fabs((float)m_pAttribute.MaxAp * m_percentValue.m_fAPNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fRP != 0.f)
		CalculateMaxRP(NtlRound((float)m_pAttribute.wMaxRP * m_percentValue.m_fRP / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fRPNegative != 0.f)
		CalculateMaxRP(NtlRound(fabs((float)m_pAttribute.wMaxRP * m_percentValue.m_fRPNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fLpRegen != 0.f)
		CalculateLpRegen(NtlRound((float)m_pAttribute.wLpRegen * m_percentValue.m_fLpRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fLpRegenNegative != 0.f)
		CalculateLpRegen(NtlRound(fabs((float)m_pAttribute.wLpRegen * m_percentValue.m_fLpRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fLpSitdownRegen != 0.f)
		CalculateLpSitdownRegen(NtlRound((float)m_pAttribute.wLpSitdownRegen * m_percentValue.m_fLpSitdownRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fLpSitdownRegenNegative != 0.f)
		CalculateLpSitdownRegen(NtlRound(fabs((float)m_pAttribute.wLpSitdownRegen * m_percentValue.m_fLpSitdownRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fLpBattleRegen != 0.f)
		CalculateLpBattleRegen(NtlRound((float)m_pAttribute.wLpBattleRegen * m_percentValue.m_fLpBattleRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fLpBattleRegenNegative != 0.f)
		CalculateLpBattleRegen(NtlRound(fabs((float)m_pAttribute.wLpBattleRegen * m_percentValue.m_fLpBattleRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEpRegen != 0.f)
		CalculateEpRegen(NtlRound((float)m_pAttribute.wEpRegen * m_percentValue.m_fEpRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEpRegenNegative != 0.f)
		CalculateEpRegen(NtlRound(fabs((float)m_pAttribute.wEpRegen * m_percentValue.m_fEpRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEpSitdownRegen != 0.f)
		CalculateEpSitdownRegen(NtlRound((float)m_pAttribute.wEpSitdownRegen * m_percentValue.m_fEpSitdownRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEpSitdownRegenNegative != 0.f)
		CalculateEpSitdownRegen(NtlRound(fabs((float)m_pAttribute.wEpSitdownRegen * m_percentValue.m_fEpSitdownRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEpBattleRegen != 0.f)
		CalculateEpBattleRegen(NtlRound((float)m_pAttribute.wEpBattleRegen * m_percentValue.m_fEpBattleRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEpBattleRegenNegative != 0.f)
		CalculateEpBattleRegen(NtlRound(fabs((float)m_pAttribute.wEpBattleRegen * m_percentValue.m_fEpBattleRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fApRegen != 0.f)
		CalculateApRegen(NtlRound((float)m_pAttribute.wApRegen * m_percentValue.m_fApRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fApRegenNegative != 0.f)
		CalculateApRegen(NtlRound(fabs((float)m_pAttribute.wApRegen * m_percentValue.m_fApRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fApSitdownRegen != 0.f)
		CalculateApSitdownRegen(NtlRound((float)m_pAttribute.wApSitdownRegen * m_percentValue.m_fApSitdownRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fApSitdownRegenNegative != 0.f)
		CalculateApSitdownRegen(NtlRound(fabs((float)m_pAttribute.wApSitdownRegen * m_percentValue.m_fApSitdownRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fApDegen != 0.f)
		CalculateApDegen(NtlRound((float)m_pAttribute.wApDegen * m_percentValue.m_fApDegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fApDegenNegative != 0.f)
		CalculateApDegen(NtlRound(fabs((float)m_pAttribute.wApDegen * m_percentValue.m_fApDegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fRpRegen != 0.f)
		CalculateRpRegen(NtlRound((float)m_pAttribute.wRpRegen * m_percentValue.m_fRpRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fRpRegenNegative != 0.f)
		CalculateRpRegen(NtlRound(fabs((float)m_pAttribute.wRpRegen * m_percentValue.m_fRpRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	//if (m_percentValue.m_fRpDimimutionRate != 0.f)
	//	Calculaterpdim((float)m_pAttribute.wRpDimimutionRate * m_percentValue.m_fRpDimimutionRate / 100.f, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);

	if (m_percentValue.m_fPhysicalOffence != 0.f)
		CalculatePhysicalOffence(NtlRound((float)m_pAttribute.wPhysicalOffence * m_percentValue.m_fPhysicalOffence / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fPhysicalOffenceNegative != 0.f)
		CalculatePhysicalOffence(NtlRound(fabs((float)m_pAttribute.wPhysicalOffence * m_percentValue.m_fPhysicalOffenceNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fPhysicalDefence != 0.f)
		CalculatePhysicalDefence(NtlRound((float)m_pAttribute.wPhysicalDefence * m_percentValue.m_fPhysicalDefence / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fPhysicalDefenceNegative != 0.f)
		CalculatePhysicalDefence(NtlRound(fabs((float)m_pAttribute.wPhysicalDefence * m_percentValue.m_fPhysicalDefenceNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEnergyOffence != 0.f)
		CalculateEnergyOffence(NtlRound((float)m_pAttribute.wEnergyOffence * m_percentValue.m_fEnergyOffence / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEnergyOffenceNegative != 0.f)
		CalculateEnergyOffence(NtlRound(fabs((float)m_pAttribute.wEnergyOffence * m_percentValue.m_fEnergyOffenceNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEnergyDefence != 0.f)
		CalculateEnergyDefence(NtlRound((float)m_pAttribute.wEnergyDefence * m_percentValue.m_fEnergyDefence / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEnergyDefenceNegative != 0.f)
		CalculateEnergyDefence(NtlRound(fabs((float)m_pAttribute.wEnergyDefence * m_percentValue.m_fEnergyDefenceNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fAttackRate != 0.f)
		CalculateAttackRate(NtlRound((float)m_pAttribute.wAttackRate * m_percentValue.m_fAttackRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fAttackRateNegative != 0.f)
		CalculateAttackRate(NtlRound(fabs((float)m_pAttribute.wAttackRate * m_percentValue.m_fAttackRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fDodgeRate != 0.f)
		CalculateDodgeRate(NtlRound((float)m_pAttribute.wDodgeRate * m_percentValue.m_fDodgeRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fDodgeRateNegative != 0.f)
		CalculateDodgeRate(NtlRound(fabs((float)m_pAttribute.wDodgeRate * m_percentValue.m_fDodgeRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fBlockRate != 0.f)
		CalculateBlockRate(NtlRound((float)m_pAttribute.wBlockRate * m_percentValue.m_fBlockRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fBlockRateNegative != 0.f)
		CalculateBlockRate(NtlRound(fabs((float)m_pAttribute.wBlockRate * m_percentValue.m_fBlockRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fBlockDamageRate != 0.f)
		CalculateBlockDamageRate(NtlRound((float)m_pAttribute.wBlockDamageRate * m_percentValue.m_fBlockDamageRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fBlockDamageRateNegative != 0.f)
		CalculateBlockDamageRate(NtlRound(fabs((float)m_pAttribute.wBlockDamageRate * m_percentValue.m_fBlockDamageRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fCurseSuccessRate != 0.f)
		CalculateCurseSuccessRate(NtlRound((float)m_pAttribute.wCurseSuccessRate * m_percentValue.m_fCurseSuccessRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fCurseSuccessRateNegative != 0.f)
		CalculateCurseSuccessRate(NtlRound(fabs((float)m_pAttribute.wCurseSuccessRate * m_percentValue.m_fCurseSuccessRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fCurseToleranceRate != 0.f)
		CalculateCurseToleranceRate(NtlRound((float)m_pAttribute.wCurseToleranceRate * m_percentValue.m_fCurseToleranceRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fCurseToleranceRateNegative != 0.f)
		CalculateCurseToleranceRate(NtlRound(fabs((float)m_pAttribute.wCurseToleranceRate * m_percentValue.m_fCurseToleranceRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fPhysicalCriticalRate != 0.f)
		CalculatePhysicalCriticalRate(NtlRound((float)m_pAttribute.wPhysicalCriticalRate * m_percentValue.m_fPhysicalCriticalRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fPhysicalCriticalRateNegative != 0.f)
		CalculatePhysicalCriticalRate(NtlRound(fabs((float)m_pAttribute.wPhysicalCriticalRate * m_percentValue.m_fPhysicalCriticalRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEnergyCriticalRate != 0.f)
		CalculateEnergyCriticalRate(NtlRound((float)m_pAttribute.wEnergyCriticalRate * m_percentValue.m_fEnergyCriticalRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEnergyCriticalRateNegative != 0.f)
		CalculateEnergyCriticalRate(NtlRound(fabs((float)m_pAttribute.wEnergyCriticalRate * m_percentValue.m_fEnergyCriticalRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fPhysicalCriticalDamageRate != 0.f)
		CalculatePhysicalCriticalDamageRate(NtlRound((float)m_pAttribute.fPhysicalCriticalDamageRate * m_percentValue.m_fPhysicalCriticalDamageRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fPhysicalCriticalDamageRateNegative != 0.f)
		CalculatePhysicalCriticalDamageRate(NtlRound(fabs((float)m_pAttribute.fPhysicalCriticalDamageRate * m_percentValue.m_fPhysicalCriticalDamageRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEnergyCriticalDamageRate != 0.f)
		CalculateEnergyCriticalDamageRate(NtlRound((float)m_pAttribute.fEnergyCriticalDamageRate * m_percentValue.m_fEnergyCriticalDamageRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEnergyCriticalDamageRateNegative != 0.f)
		CalculateEnergyCriticalDamageRate(NtlRound(fabs((float)m_pAttribute.fEnergyCriticalDamageRate * m_percentValue.m_fEnergyCriticalDamageRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);


	m_pAttribute.fFlySpeed += m_pAttribute.fFlySpeed * m_percentValue.m_fFlySpeed / 100.f;
	m_pAttribute.fFlyDashSpeed += m_pAttribute.fFlyDashSpeed * m_percentValue.m_fFlyDashSpeed / 100.f;
	m_pAttribute.fFlyAccelSpeed += m_pAttribute.fFlyAccelSpeed * m_percentValue.m_fFlyAccelSpeed / 100.f;

	if (m_percentValue.m_fAttackSpeedRate != 0.f)
		CalculateAttackSpeedRate(NtlRound((float)m_pAttribute.wAttackSpeedRate * m_percentValue.m_fAttackSpeedRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fAttackSpeedRateNegative != 0.f)
		CalculateAttackSpeedRate(NtlRound(fabs((float)m_pAttribute.wAttackSpeedRate * m_percentValue.m_fAttackSpeedRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fAttackRange != 0.f)
		CalculateAttackRange(NtlRound((float)m_pAttribute.fAttackRange * m_percentValue.m_fAttackRange / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fAttackRangeNegative != 0.f)
		CalculateAttackRange(NtlRound(fabs((float)m_pAttribute.fAttackRange * m_percentValue.m_fAttackRangeNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fStomachacheDef != 0.f)
		CalculateStomachacheDefence(NtlRound((float)m_pAttribute.wStomachacheDefence * m_percentValue.m_fStomachacheDef / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fStomachacheDefNegative != 0.f)
		CalculateStomachacheDefence(NtlRound(fabs((float)m_pAttribute.wStomachacheDefence * m_percentValue.m_fStomachacheDefNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fPoisonDef != 0.f)
		CalculatePoisonDefence(NtlRound((float)m_pAttribute.wPoisonDefence * m_percentValue.m_fPoisonDef / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fPoisonDefNegative != 0.f)
		CalculatePoisonDefence(NtlRound(fabs((float)m_pAttribute.wPoisonDefence * m_percentValue.m_fPoisonDefNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fBleedDef != 0.f)
		CalculateBleedDefence(NtlRound((float)m_pAttribute.wBleedDefence * m_percentValue.m_fBleedDef / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fBleedDefNegative != 0.f)
		CalculateBleedDefence(NtlRound(fabs((float)m_pAttribute.wBleedDefence * m_percentValue.m_fBleedDefNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fBurnDef != 0.f)
		CalculateBurnDefence(NtlRound((float)m_pAttribute.wBurnDefence * m_percentValue.m_fBurnDef / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fBurnDefNegative != 0.f)
		CalculateBurnDefence(NtlRound(fabs((float)m_pAttribute.wBurnDefence * m_percentValue.m_fBurnDefNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	m_pAttribute.fMindCurseImmunity += m_pAttribute.fMindCurseImmunity * m_percentValue.m_fMindCurseImmunity / 100.f;
	m_pAttribute.fBodyCurseImmunity += m_pAttribute.fBodyCurseImmunity * m_percentValue.m_fBodyCurseImmunity / 100.f;
	m_pAttribute.fChangeCurseImmunity += m_pAttribute.fChangeCurseImmunity * m_percentValue.m_fChangeCurseImmunity / 100.f;
	m_pAttribute.fSkillAnimationSpeedModifier += m_pAttribute.fSkillAnimationSpeedModifier * m_percentValue.m_fSkillAnimationSpeedModifier / 100.f;

	if (m_percentValue.m_fGuardRate != 0.f)
		CalcAttributeW(m_pAttribute.wGuardRate, NtlRound((float)m_pAttribute.wGuardRate * m_percentValue.m_fGuardRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fGuardRateNegative != 0.f)
		CalcAttributeW(m_pAttribute.wGuardRate, NtlRound(fabs((float)m_pAttribute.wGuardRate * m_percentValue.m_fGuardRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

}


void CCharacterAtt::CalculateStr(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.Str, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateCon(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.Con, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateFoc(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.Foc, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateDex(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.Dex, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateSol(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.Sol, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateEng(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.Eng, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateMaxLP(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.MaxLp += (int)fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLP += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.MaxLp -= (int)fValue;

			if (m_pAttribute.MaxLp < 0)
				m_pAttribute.MaxLp = 0;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLPNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateMaxEP(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wMaxEP = UnsignedSafeIncrease<WORD>(m_pAttribute.wMaxEP, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEP += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wMaxEP = UnsignedSafeDecrease<WORD>(m_pAttribute.wMaxEP, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEPNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateMaxRP(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wMaxRP = UnsignedSafeIncrease<WORD>(m_pAttribute.wMaxRP, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fRP += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wMaxRP = UnsignedSafeDecrease<WORD>(m_pAttribute.wMaxRP, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fRPNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateMaxAP(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.MaxAp += (int)fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAP += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.MaxAp -= (int)fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAPNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateLpRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLpRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wLpRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLpRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLpRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLpRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLpRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateLpSitdownRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLpSitdownRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wLpSitdownRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLpSitdownRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLpSitdownRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLpSitdownRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLpSitdownRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateLpBattleRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLpBattleRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wLpBattleRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLpBattleRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLpBattleRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLpBattleRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLpBattleRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateEpRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wEpRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wEpRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEpRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wEpRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wEpRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEpRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateEpSitdownRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wEpSitdownRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wEpSitdownRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEpSitdownRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wEpSitdownRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wEpSitdownRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEpSitdownRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateEpBattleRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wEpBattleRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wEpBattleRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEpBattleRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wEpBattleRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wEpBattleRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEpBattleRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateApRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wApRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wApRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fApRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wApRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wApRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fApRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateApSitdownRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wApSitdownRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wApSitdownRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fApSitdownRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wApSitdownRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wApSitdownRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fApSitdownRegenNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateApDegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wApDegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wApDegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fApDegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wApDegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wApDegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fApDegenNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateRpRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wRpRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wRpRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fRpRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wRpRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wRpRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fRpRegenNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculatePhysicalOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wPhysicalOffence = UnsignedSafeIncrease<WORD>(m_pAttribute.wPhysicalOffence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalOffence += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wPhysicalOffence = UnsignedSafeDecrease<WORD>(m_pAttribute.wPhysicalOffence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalOffenceNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateEnergyOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wEnergyOffence = UnsignedSafeIncrease<WORD>(m_pAttribute.wEnergyOffence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyOffence += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wEnergyOffence = UnsignedSafeDecrease<WORD>(m_pAttribute.wEnergyOffence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyOffenceNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculatePhysicalDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wPhysicalDefence = UnsignedSafeIncrease<WORD>(m_pAttribute.wPhysicalDefence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalDefence += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wPhysicalDefence = UnsignedSafeDecrease<WORD>(m_pAttribute.wPhysicalDefence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalDefenceNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateEnergyDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wEnergyDefence = UnsignedSafeIncrease<WORD>(m_pAttribute.wEnergyDefence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyDefence += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wEnergyDefence = UnsignedSafeDecrease<WORD>(m_pAttribute.wEnergyDefence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyDefenceNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateAttackRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wAttackRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wAttackRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAttackRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wAttackRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wAttackRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAttackRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateDodgeRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wDodgeRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wDodgeRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fDodgeRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wDodgeRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wDodgeRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fDodgeRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateBlockRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wBlockRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wBlockRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBlockRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wBlockRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wBlockRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBlockRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateBlockDamageRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wBlockDamageRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wBlockDamageRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBlockDamageRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wBlockDamageRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wBlockDamageRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBlockDamageRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateCurseSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wCurseSuccessRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wCurseSuccessRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fCurseSuccessRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wCurseSuccessRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wCurseSuccessRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fCurseSuccessRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateCurseToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wCurseToleranceRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wCurseToleranceRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fCurseToleranceRate += fValue;

			if (m_percentValue.m_fCurseToleranceRate > 60.f)
				m_percentValue.m_fCurseToleranceRate = 60.f;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wCurseToleranceRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wCurseToleranceRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fCurseToleranceRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculatePhysicalCriticalRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wPhysicalCriticalRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wPhysicalCriticalRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalCriticalRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wPhysicalCriticalRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wPhysicalCriticalRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalCriticalRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateEnergyCriticalRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wEnergyCriticalRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wEnergyCriticalRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyCriticalRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wEnergyCriticalRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wEnergyCriticalRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyCriticalRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculatePhysicalCriticalDamageRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fPhysicalCriticalDamageRate += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalCriticalDamageRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fPhysicalCriticalDamageRate -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalCriticalDamageRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateEnergyCriticalDamageRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fEnergyCriticalDamageRate += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyCriticalDamageRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fEnergyCriticalDamageRate -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyCriticalDamageRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateRunSpeed(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fRunSpeed += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_pAttribute.fRunSpeed += m_pAttribute.fRunSpeed * fValue / 100.f;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fRunSpeed -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_pAttribute.fRunSpeed -= m_pAttribute.fRunSpeed * fValue / 100.f;
		}

		if (m_pAttribute.fRunSpeed < 0.f)
			m_pAttribute.fRunSpeed = 0.f;
	}
}

void CCharacterAtt::CalculateFlySpeed(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fFlySpeed += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFlySpeed += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fFlySpeed -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFlySpeedNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateFlyDashSpeed(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fFlyDashSpeed += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFlyDashSpeed += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fFlyDashSpeed -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFlyDashSpeedNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateFlyAccelSpeed(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fFlyAccelSpeed += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFlyAccelSpeed += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fFlyAccelSpeed -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFlyAccelSpeedNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateAttackSpeedRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	/*
		Increase = slower
		Decrease = faster
	*/

	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wAttackSpeedRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wAttackSpeedRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAttackSpeedRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wAttackSpeedRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wAttackSpeedRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAttackSpeedRateNegative -= fValue;
		}

		if(m_pAttribute.wAttackSpeedRate < 80)
			m_pAttribute.wAttackSpeedRate = 80;
	}
}

void CCharacterAtt::CalculateAttackRange(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fAttackRange += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAttackRange += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fAttackRange -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAttackRangeNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateCastingTimeChangePercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fCastingTimeChangePercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateCoolTimeChangePercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fCoolTimeChangePercent, fValue, byApplyType, bIsPlus, -50.f);
}

void CCharacterAtt::CalculateKeepTimeChangePercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fKeepTimeChangePercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateDotTimeChangeAbsolute(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fDotTimeChangeAbsolute, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateRequiredEpChangePercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fRequiredEpChangePercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateBattleAttribute(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalculateHonestOffence(fValue, byApplyType, bIsPlus);
	CalculateStrangeOffence(fValue, byApplyType, bIsPlus);
	CalculateWildOffence(fValue, byApplyType, bIsPlus);
	CalculateEleganceOffence(fValue, byApplyType, bIsPlus);
	CalculateFunnyOffence(fValue, byApplyType, bIsPlus);
	CalculateHonestDefence(fValue, byApplyType, bIsPlus);
	CalculateStrangeDefence(fValue, byApplyType, bIsPlus);
	CalculateWildDefence(fValue, byApplyType, bIsPlus);
	CalculateEleganceDefence(fValue, byApplyType, bIsPlus);
	CalculateFunnyDefence(fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateHonestOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalculateCriticalBlockSuccessRate(fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateHonestDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalculateCriticalBlockSuccessRate(fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateStrangeOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
//	CalculateMaxLP(fValue, byApplyType, bIsPlus); // TEST TO SEE IF THE ISSUE WITH MOBS BVUGGED LP IS FIXED
}

void CCharacterAtt::CalculateStrangeDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
//	CalculateMaxLP(fValue, byApplyType, bIsPlus); // TEST TO SEE IF THE ISSUE WITH MOBS BVUGGED LP IS FIXED
}

void CCharacterAtt::CalculateWildOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalculatePhysicalArmorPen(fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateWildDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalculatePhysicalArmorPen(fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateEleganceOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalculateEnergyArmorPen(fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateEleganceDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalculateEnergyArmorPen(fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateFunnyOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalculatePhysicalCriticalDefenceRate(fValue, byApplyType, bIsPlus);
	CalculateEnergyCriticalDefenceRate(fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateFunnyDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalculatePhysicalCriticalDefenceRate(fValue, byApplyType, bIsPlus);
	CalculateEnergyCriticalDefenceRate(fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculatePhysicalReflection(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fPhysicalReflection, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateEnergyReflection(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fEnergyReflection, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateParalyzeToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wParalyzeToleranceRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateTerrorToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wTerrorToleranceRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateConfuseToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wConfuseToleranceRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateStoneToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wStoneToleranceRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateCandyToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wCandyToleranceRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateParalyzeKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fParalyzeKeepTimeDown, fValue, byApplyType, bIsPlus, 99.9f);
}

void CCharacterAtt::CalculateTerrorKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fTerrorKeepTimeDown, fValue, byApplyType, bIsPlus, 99.9f);
}

void CCharacterAtt::CalculateConfuseKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fConfuseKeepTimeDown, fValue, byApplyType, bIsPlus, 99.9f);
}

void CCharacterAtt::CalculateStoneKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fStoneKeepTimeDown, fValue, byApplyType, bIsPlus, 99.9f);
}

void CCharacterAtt::CalculateCandyKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fCandyKeepTimeDown, fValue, byApplyType, bIsPlus, 99.9f);
}

void CCharacterAtt::CalculateBleedingKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fBleedingKeepTimeDown, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculatePoisonKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fPoisonKeepTimeDown, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateStomachacheKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fStomachacheKeepTimeDown, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateCriticalBlockSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fCriticalBlockSuccessRate, fValue, byApplyType, bIsPlus, DBO_MAX_CRITICAL_BLOCK_SUCCESS_RATE);
}

void CCharacterAtt::CalculateLpRecoveryWhenHit(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeDW(m_pAttribute.dwLpRecoveryWhenHit, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateLpRecoveryWhenHitInPercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fLpRecoveryWhenHitInPercent, fValue, byApplyType, bIsPlus, m_pOwnerRef->IsPC() == true ? 60.f : INVALID_FLOAT);
}

void CCharacterAtt::CalculateEpRecoveryWhenHit(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeDW(m_pAttribute.dwEpRecoveryWhenHit, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateEpRecoveryWhenHitInPercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fEpRecoveryWhenHitInPercent, fValue, byApplyType, bIsPlus, m_pOwnerRef->IsPC() == true ? 60.f : INVALID_FLOAT);
}

void CCharacterAtt::CalculateStomachacheDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wStomachacheDefence = UnsignedSafeIncrease<WORD>(m_pAttribute.wStomachacheDefence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fStomachacheDef += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wStomachacheDefence = UnsignedSafeDecrease<WORD>(m_pAttribute.wStomachacheDefence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fStomachacheDefNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculatePoisonDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wPoisonDefence = UnsignedSafeIncrease<WORD>(m_pAttribute.wPoisonDefence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPoisonDef += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wPoisonDefence = UnsignedSafeDecrease<WORD>(m_pAttribute.wPoisonDefence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPoisonDefNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateBleedDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wBleedDefence = UnsignedSafeIncrease<WORD>(m_pAttribute.wBleedDefence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBleedDef += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wBleedDefence = UnsignedSafeDecrease<WORD>(m_pAttribute.wBleedDefence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBleedDefNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateBurnDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wBurnDefence = UnsignedSafeIncrease<WORD>(m_pAttribute.wBurnDefence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBurnDef += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wBurnDefence = UnsignedSafeDecrease<WORD>(m_pAttribute.wBurnDefence, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBurnDefNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateMindCurseImmunity(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fMindCurseImmunity += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fMindCurseImmunity += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fMindCurseImmunity -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fMindCurseImmunityNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateBodyCurseImmunity(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fBodyCurseImmunity += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBodyCurseImmunity += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fBodyCurseImmunity -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBodyCurseImmunityNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateChangeCurseImmunity(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fChangeCurseImmunity += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fChangeCurseImmunity += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fChangeCurseImmunity -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fChangeCurseImmunityNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateSkillAnimationSpeedModifier(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fSkillAnimationSpeedModifier += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fSkillAnimationSpeedModifier += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fSkillAnimationSpeedModifier -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fSkillAnimationSpeedModifierNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateSkillAggroBonus(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fSkillAggroBonus, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateSkillAggroBonusInPercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fSkillAggroBonusInPercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateDirectHealPowerBonus(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fDirectHealPowerBonus, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateDirectHealPowerBonusInPercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fDirectHealPowerBonusInPercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateHotPowerBonus(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fHotPowerBonus, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateHotPowerBonusInPercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fHotPowerBonusInPercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateDotValueChangePercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fDotValueChangePercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculatePhysicalCriticalDefenceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fPhysicalCriticalDefenceRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateEnergyCriticalDefenceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fEnergyCriticalDefenceRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateGuardRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wGuardRate += (WORD)fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fGuardRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wGuardRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wGuardRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fGuardRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateSkillDamageBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fSkillDamageBlockModeSuccessRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateCurseBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fCurseBlockModeSuccessRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateKnockdownBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fKnockdownBlockModeSuccessRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateHtbBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fHtbBlockModeSuccessRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateExpBooster(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeBYTE(m_pAttribute.byExpBooster, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculatePhysicalArmorPen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fPhysicalArmorPenRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateEnergyArmorPen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fEnergyArmorPenRate, fValue, byApplyType, bIsPlus);
}
