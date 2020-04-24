#include "stdafx.h"
#include "NtlBitFlag.h"
#include "NtlAvatar.h"

#include "NtlBitFlagManager.h"

CNtlAvatar::CNtlAvatar(void)
{
	Init();
}

CNtlAvatar::~CNtlAvatar(void)
{
}

void CNtlAvatar::Init()
{
	InitializeAttributeLink();
}

void CNtlAvatar::InitializeAttributeLink()
{
}

CNtlAvatar* CNtlAvatar::GetInstance()
{
	static CNtlAvatar avatar;
	return &avatar;
}

bool CNtlAvatar::UpdateAvatarAttribute(BYTE byAttributeTotalCount, void* pvRawData, sAVATAR_ATTRIBUTE* pAttributeData)
{
	BYTE* pbyCurrentPosition = (BYTE*)pvRawData;
	BYTE* pbyAttributeData = (BYTE*)pAttributeData;

	CNtlBitFlagManager changedFlag;
	if (false == changedFlag.Create(pvRawData, byAttributeTotalCount))
	{
		return false;
	}

	pbyCurrentPosition += changedFlag.GetBytesUsed();

	for (BYTE byIndex = ATTRIBUTE_TO_UPDATE_FIRST ; byIndex <= ATTRIBUTE_TO_UPDATE_LAST ; byIndex++)
	{
		if (false != changedFlag.IsSet(byIndex))
		{
			DWORD dwDataSize = m_attributeLogic[byIndex].pCopyAttributeFunction(pbyCurrentPosition, pbyAttributeData + m_attributeLogic[byIndex].dwFieldOffset);
			if (0 == dwDataSize)
			{
				return false;
			}
			else
			{
				pbyCurrentPosition += dwDataSize;
			}
		}
	}

	return true;
}

bool CNtlAvatar::SaveAvatarAttribute(CNtlBitFlagManager* pChangedFlag, sAVATAR_ATTRIBUTE_LINK* pAttributeDataLink, void* pvBuffer, DWORD* pwdDataSize)
{
	BYTE* pbyBuffer = (BYTE*)pvBuffer;
	BYTE* pbyAttributeDataLink = (BYTE*)pAttributeDataLink;

	*pwdDataSize = 0;

	::CopyMemory(pbyBuffer, pChangedFlag->GetRawData(), pChangedFlag->GetBytesUsed());

	pbyBuffer += pChangedFlag->GetBytesUsed();
	*pwdDataSize += pChangedFlag->GetBytesUsed();

	for (BYTE byIndex = ATTRIBUTE_TO_UPDATE_FIRST; byIndex <= ATTRIBUTE_TO_UPDATE_LAST; byIndex++)
	{
		if (false != pChangedFlag->IsSet(byIndex))
		{
			void* pvAttributeFieldLink = *((void**)(pbyAttributeDataLink + byIndex * sizeof(void*)));
			if (NULL == pvAttributeFieldLink)
			{
				continue;
			}

			DWORD dwDataSize = m_attributeLogic[byIndex].pCopyAttributeFunction(pvAttributeFieldLink, pbyBuffer);
			if (0 == dwDataSize)
			{
				return false;
			}
			else
			{
				pbyBuffer += dwDataSize;
				*pwdDataSize += dwDataSize;
			}
		}
	}

	return true;
}

bool CNtlAvatar::SaveAvatarAttribute(CNtlBitFlagManager* pChangedFlag, sAVATAR_ATTRIBUTE* pAttributeData, void* pvBuffer, DWORD* pwdDataSize)
{
	BYTE* pbyBuffer = (BYTE*)pvBuffer;
	BYTE* pbyAttributeData = (BYTE*)pAttributeData;

	*pwdDataSize = 0;

	::CopyMemory(pbyBuffer, pChangedFlag->GetRawData(), pChangedFlag->GetBytesUsed());

	pbyBuffer += pChangedFlag->GetBytesUsed();
	*pwdDataSize += pChangedFlag->GetBytesUsed();

	for (BYTE byIndex = ATTRIBUTE_TO_UPDATE_FIRST; byIndex <= ATTRIBUTE_TO_UPDATE_LAST; byIndex++)
	{
		if (false != pChangedFlag->IsSet(byIndex))
		{
			void* pvAttributeFieldLink = *((void**)(pbyAttributeData + byIndex * sizeof(void*)));
			if (NULL == pvAttributeFieldLink)
			{
				continue;
			}

			DWORD dwDataSize = m_attributeLogic[byIndex].pCopyAttributeFunction(pvAttributeFieldLink, pbyBuffer);
			if (0 == dwDataSize)
			{
				return false;
			}
			else
			{
				pbyBuffer += dwDataSize;
				*pwdDataSize += dwDataSize;
			}
		}
	}

	return true;
}

bool CNtlAvatar::FillAvatarAttribute(sAVATAR_ATTRIBUTE_LINK* pAttributeDataLink, sAVATAR_ATTRIBUTE* pAttributeData)
{
	BYTE* pbyAttributeDataLink = (BYTE*)pAttributeDataLink;
	BYTE* pbyAttributeData = (BYTE*)pAttributeData;

	for (BYTE byIndex = ATTRIBUTE_TO_UPDATE_FIRST; byIndex <= ATTRIBUTE_TO_UPDATE_LAST; byIndex++)
	{
		void* pvAttributeFieldLink = *((void**)(pbyAttributeDataLink + byIndex * sizeof(void*)));
		if (NULL == pvAttributeFieldLink)
		{
			continue;
		}

		DWORD dwDataSize = m_attributeLogic[byIndex].pCopyAttributeFunction(
			pvAttributeFieldLink,
			pbyAttributeData + m_attributeLogic[byIndex].dwFieldOffset);
		if (0 == dwDataSize)
		{
			return false;
		}
	}

	return true;
}

#ifndef ATTRIBUTE_LOGIC_DEFINE
#define ATTRIBUTE_LOGIC_DEFINE(field_name, type)					\
	{																\
		PtrToUlong(&(((sAVATAR_ATTRIBUTE*)NULL)->field_name)),		\
		CopyValueByType_##type										\
	}
#endif

CNtlAvatar::sATTRIBUTE_LOGIC CNtlAvatar::m_attributeLogic[ATTRIBUTE_TO_UPDATE_COUNT] =
{
	ATTRIBUTE_LOGIC_DEFINE(Str, WORD),
	ATTRIBUTE_LOGIC_DEFINE(Con, WORD),
	ATTRIBUTE_LOGIC_DEFINE(Foc, WORD),
	ATTRIBUTE_LOGIC_DEFINE(Dex, WORD),
	ATTRIBUTE_LOGIC_DEFINE(Sol, WORD),
	ATTRIBUTE_LOGIC_DEFINE(Eng, WORD),
	ATTRIBUTE_LOGIC_DEFINE(MaxLp, int),
	ATTRIBUTE_LOGIC_DEFINE(wMaxEP, WORD),
	ATTRIBUTE_LOGIC_DEFINE(MaxAp, int),
	ATTRIBUTE_LOGIC_DEFINE(wMaxRP, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wLpRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wLpSitdownRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wLpBattleRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wEpRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wEpSitdownRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wEpBattleRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wApRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wApSitdownRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wApDegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wRpRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wRpDimimutionRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wPhysicalOffence, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wPhysicalDefence, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wEnergyOffence, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wEnergyDefence, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wAttackRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wDodgeRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wBlockRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wBlockDamageRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wCurseSuccessRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wCurseToleranceRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wPhysicalCriticalRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wEnergyCriticalRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(fPhysicalCriticalDamageRate, float),
	ATTRIBUTE_LOGIC_DEFINE(fEnergyCriticalDamageRate, float),
	ATTRIBUTE_LOGIC_DEFINE(fRunSpeed, float),
	ATTRIBUTE_LOGIC_DEFINE(fFlySpeed, float),
	ATTRIBUTE_LOGIC_DEFINE(fFlyDashSpeed, float),
	ATTRIBUTE_LOGIC_DEFINE(fFlyAccelSpeed, float),
	ATTRIBUTE_LOGIC_DEFINE(wAttackSpeedRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(fAttackRange, float),
	ATTRIBUTE_LOGIC_DEFINE(fCastingTimeChangePercent, float),
	ATTRIBUTE_LOGIC_DEFINE(fCoolTimeChangePercent, float),
	ATTRIBUTE_LOGIC_DEFINE(fKeepTimeChangePercent, float),
	ATTRIBUTE_LOGIC_DEFINE(fDotTimeChangeAbsolute, float),
	ATTRIBUTE_LOGIC_DEFINE(fRequiredEpChangePercent, float),
	ATTRIBUTE_LOGIC_DEFINE(fPhysicalReflection, float),
	ATTRIBUTE_LOGIC_DEFINE(fEnergyReflection, float),
	ATTRIBUTE_LOGIC_DEFINE(wParalyzeToleranceRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wTerrorToleranceRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wConfuseToleranceRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wStoneToleranceRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wCandyToleranceRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(fParalyzeKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE(fTerrorKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE(fConfuseKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE(fStoneKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE(fCandyKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE(fBleedingKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE(fPoisonKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE(fStomachacheKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE(fCriticalBlockSuccessRate, float),
	ATTRIBUTE_LOGIC_DEFINE(dwLpRecoveryWhenHit, DWORD),
	ATTRIBUTE_LOGIC_DEFINE(fLpRecoveryWhenHitInPercent, float),
	ATTRIBUTE_LOGIC_DEFINE(dwEpRecoveryWhenHit, DWORD),
	ATTRIBUTE_LOGIC_DEFINE(fEpRecoveryWhenHitInPercent, float),
	ATTRIBUTE_LOGIC_DEFINE(wStomachacheDefence, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wPoisonDefence, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wBleedDefence, WORD),
	ATTRIBUTE_LOGIC_DEFINE(wBurnDefence, WORD),
	ATTRIBUTE_LOGIC_DEFINE(fMindCurseImmunity, float),
	ATTRIBUTE_LOGIC_DEFINE(fBodyCurseImmunity, float),
	ATTRIBUTE_LOGIC_DEFINE(fChangeCurseImmunity, float),
	ATTRIBUTE_LOGIC_DEFINE(fSkillAnimationSpeedModifier, float),
	ATTRIBUTE_LOGIC_DEFINE(dwWeightLimit, DWORD),
	ATTRIBUTE_LOGIC_DEFINE(fSkillAggroBonus, float),
	ATTRIBUTE_LOGIC_DEFINE(fSkillAggroBonusInPercent, float),
	ATTRIBUTE_LOGIC_DEFINE(fDirectHealPowerBonus, float),
	ATTRIBUTE_LOGIC_DEFINE(fDirectHealPowerBonusInPercent, float),
	ATTRIBUTE_LOGIC_DEFINE(fHotPowerBonus, float),
	ATTRIBUTE_LOGIC_DEFINE(fHotPowerBonusInPercent, float),
	ATTRIBUTE_LOGIC_DEFINE(fDotValueChangePercent, float),
	ATTRIBUTE_LOGIC_DEFINE(fPhysicalCriticalDefenceRate, float),
	ATTRIBUTE_LOGIC_DEFINE(fEnergyCriticalDefenceRate, float),
	ATTRIBUTE_LOGIC_DEFINE(wGuardRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE(fSkillDamageBlockModeSuccessRate, float),
	ATTRIBUTE_LOGIC_DEFINE(fCurseBlockModeSuccessRate, float),
	ATTRIBUTE_LOGIC_DEFINE(fKnockdownBlockModeSuccessRate, float),
	ATTRIBUTE_LOGIC_DEFINE(fHtbBlockModeSuccessRate, float),
	ATTRIBUTE_LOGIC_DEFINE(fItemUpgradeBonusRate, float),
	ATTRIBUTE_LOGIC_DEFINE(byExpBooster, BYTE),
	ATTRIBUTE_LOGIC_DEFINE(byQuestDropRate, BYTE),
	ATTRIBUTE_LOGIC_DEFINE(fPhysicalArmorPenRate, float),
	ATTRIBUTE_LOGIC_DEFINE(fEnergyArmorPenRate, float),

};

#undef ATTRIBUTE_LOGIC_DEFINE

DWORD CNtlAvatar::CopyValueByType_BYTE(void* pvValue, void* pvBuffer)
{
	BYTE* pbyBuffer = (BYTE*)pvBuffer;
	*pbyBuffer = *((BYTE*)pvValue);
	return sizeof(BYTE);
}
DWORD CNtlAvatar::CopyValueByType_WORD(void* pvValue, void* pvBuffer)
{
	WORD* pbyBuffer = (WORD*)pvBuffer;
	*pbyBuffer = *((WORD*)pvValue);
	return sizeof(WORD);
}
DWORD CNtlAvatar::CopyValueByType_float(void* pvValue, void* pvBuffer)
{
	float* pbyBuffer = (float*)pvBuffer;
	*pbyBuffer = *((float*)pvValue);
	return sizeof(float);
}
DWORD CNtlAvatar::CopyValueByType_DWORD(void* pvValue, void* pvBuffer)
{
	DWORD* pbyBuffer = (DWORD*)pvBuffer;
	*pbyBuffer = *((DWORD*)pvValue);
	return sizeof(DWORD);
}
DWORD CNtlAvatar::CopyValueByType_int(void* pvValue, void* pvBuffer)
{
	int* pbyBuffer = (int*)pvBuffer;
	*pbyBuffer = *((int*)pvValue);
	return sizeof(int);
}



bool CNtlAvatar::FillAvatarAttributeNew(sAVATAR_ATTRIBUTE* pAttributeData, sAVATAR_ATTRIBUTE_LINK* pAttributeDataLink)
{
	BYTE* pbyAttributeDataLink = (BYTE*)pAttributeDataLink;
	BYTE* pbyAttributeData = (BYTE*)pAttributeData;

	for (BYTE byIndex = ATTRIBUTE_TO_UPDATE_FIRST ; byIndex <= ATTRIBUTE_TO_UPDATE_LAST ; byIndex++)
	{
		void* pvAttributeFieldLink = ((void*)(pbyAttributeData + byIndex * sizeof(void*)));
		if (NULL == pvAttributeFieldLink)
		{
			continue;
		}

		DWORD dwDataSize = m_attributeLogicNew[byIndex].pCopyAttributeFunction(
																			pvAttributeFieldLink,
																			pbyAttributeDataLink + m_attributeLogic[byIndex].dwFieldOffset);
		if (0 == dwDataSize)
		{
			return false;
		}
	}

	return true;
}



#ifndef ATTRIBUTE_LOGIC_DEFINE_NEW
#define ATTRIBUTE_LOGIC_DEFINE_NEW(field_name, type)						\
	{																		\
		PtrToUlong(&(((sAVATAR_ATTRIBUTE_LINK*)NULL)->field_name)),			\
		CopyValueByTypeNew_##type											\
	}
#endif

CNtlAvatar::sATTRIBUTE_LOGIC CNtlAvatar::m_attributeLogicNew[ATTRIBUTE_TO_UPDATE_COUNT] =
{
	ATTRIBUTE_LOGIC_DEFINE_NEW(pStr, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pCon, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pFoc, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pDex, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pSol, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pEng, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pMaxLp, int),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwMaxEP, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pMaxAp, int),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwMaxRP, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwLpRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwLpSitdownRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwLpBattleRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwEpRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwEpSitdownRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwEpBattleRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwApRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwApSitdownRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwApDegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwRpRegen, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwRpDimimutionRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwPhysicalOffence, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwPhysicalDefence, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwEnergyOffence, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwEnergyDefence, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwAttackRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwDodgeRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwBlockRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwBlockDamageRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwCurseSuccessRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwCurseToleranceRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwPhysicalCriticalRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwEnergyCriticalRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfPhysicalCriticalDamageRate, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfEnergyCriticalDamageRate, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfRunSpeed, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfFlySpeed, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfFlyDashSpeed, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfFlyAccelSpeed, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwAttackSpeedRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfAttackRange, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfCastingTimeChangePercent, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfCoolTimeChangePercent, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfKeepTimeChangePercent, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfDotTimeChangeAbsolute, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfRequiredEpChangePercent, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfPhysicalReflection, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfEnergyReflection, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwParalyzeToleranceRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwTerrorToleranceRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwConfuseToleranceRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwStoneToleranceRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwCandyToleranceRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfParalyzeKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfTerrorKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfConfuseKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfStoneKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfCandyKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfBleedingKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfPoisonKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfStomachacheKeepTimeDown, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfCriticalBlockSuccessRate, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pdwLpRecoveryWhenHit, DWORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfLpRecoveryWhenHitInPercent, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pdwEpRecoveryWhenHit, DWORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfEpRecoveryWhenHitInPercent, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwStomachacheDefence, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwPoisonDefence, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwBleedDefence, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwBurnDefence, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfMindCurseImmunity, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfBodyCurseImmunity, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfChangeCurseImmunity, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfSkillAnimationSpeedModifier, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pdwWeightLimit, DWORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfSkillAggroBonus, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfSkillAggroBonusInPercent, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfDirectHealPowerBonus, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfDirectHealPowerBonusInPercent, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfHotPowerBonus, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfHotPowerBonusInPercent, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfDotValueChangePercent, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfPhysicalCriticalDefenceRate, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfEnergyCriticalDefenceRate, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pwGuardRate, WORD),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfSkillDamageBlockModeSuccessRate, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfCurseBlockModeSuccessRate, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfKnockdownBlockModeSuccessRate, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfHtbBlockModeSuccessRate, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfItemUpgradeBonusRate, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pbyExpBooster, BYTE),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pbyQuestDropRate, BYTE),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfPhysicalArmorPenRate, float),
	ATTRIBUTE_LOGIC_DEFINE_NEW(pfEnergyArmorPenRate, float),
};

#undef ATTRIBUTE_LOGIC_DEFINE_NEW



DWORD CNtlAvatar::CopyValueByTypeNew_BYTE(void* pvValue, void* pvBuffer)
{
	BYTE* pbyBuffer = (BYTE*)pvBuffer;
	*pbyBuffer = *((BYTE*)pvValue);
	return sizeof(pvValue);
}
DWORD CNtlAvatar::CopyValueByTypeNew_WORD(void* pvValue, void* pvBuffer)
{
	WORD* pbyBuffer = (WORD*)pvBuffer;
	pbyBuffer = (WORD*)pvValue;
	return sizeof(WORD*);
}
DWORD CNtlAvatar::CopyValueByTypeNew_float(void* pvValue, void* pvBuffer)
{
	float* pbyBuffer = (float*)pvBuffer;
	pbyBuffer = (float*)pvValue;
	return sizeof(float*);
}
DWORD CNtlAvatar::CopyValueByTypeNew_DWORD(void* pvValue, void* pvBuffer)
{
	DWORD* pbyBuffer = (DWORD*)pvBuffer;
	*pbyBuffer = *((DWORD*)pvValue);
	return sizeof(DWORD);
}
DWORD CNtlAvatar::CopyValueByTypeNew_int(void* pvValue, void* pvBuffer)
{
	int* pbyBuffer = (int*)pvBuffer;
	*pbyBuffer = *((int*)pvValue);
	return sizeof(int);
}

sAVATAR_ATTRIBUTE_LINK CNtlAvatar::ConvertAVATAR_ATTRIBUTE(sAVATAR_ATTRIBUTE* avt)
{
	sAVATAR_ATTRIBUTE_LINK newavt;

	newavt.pStr = &avt->Str;
	newavt.pCon = &avt->Con;
	newavt.pFoc = &avt->Foc;
	newavt.pDex = &avt->Dex;
	newavt.pSol = &avt->Sol;
	newavt.pEng = &avt->Eng;
	newavt.pMaxLp = &avt->MaxLp;
	newavt.pwMaxEP = &avt->wMaxEP;
	newavt.pMaxAp = &avt->MaxAp;
	newavt.pwMaxRP = &avt->wMaxRP;
	newavt.pwLpRegen = &avt->wLpRegen;
	newavt.pwLpSitdownRegen = &avt->wLpSitdownRegen;
	newavt.pwLpBattleRegen = &avt->wLpBattleRegen;
	newavt.pwEpRegen = &avt->wEpRegen;
	newavt.pwEpSitdownRegen = &avt->wEpSitdownRegen;
	newavt.pwEpBattleRegen = &avt->wEpBattleRegen;
	newavt.pwApRegen = &avt->wApRegen;
	newavt.pwApSitdownRegen = &avt->wApSitdownRegen;
	newavt.pwApDegen = &avt->wApDegen;
	newavt.pwRpRegen = &avt->wRpRegen;
	newavt.pwRpDimimutionRate = &avt->wRpDimimutionRate;
	newavt.pwPhysicalOffence = &avt->wPhysicalOffence;
	newavt.pwPhysicalDefence = &avt->wPhysicalDefence;
	newavt.pwEnergyOffence = &avt->wEnergyOffence;
	newavt.pwEnergyDefence = &avt->wEnergyDefence;
	newavt.pwAttackRate = &avt->wAttackRate;
	newavt.pwDodgeRate = &avt->wDodgeRate;
	newavt.pwBlockRate = &avt->wBlockRate;
	newavt.pwBlockDamageRate = &avt->wBlockDamageRate;
	newavt.pwCurseSuccessRate = &avt->wCurseSuccessRate;
	newavt.pwCurseToleranceRate = &avt->wCurseToleranceRate;
	newavt.pwPhysicalCriticalRate = &avt->wPhysicalCriticalRate;
	newavt.pwEnergyCriticalRate = &avt->wEnergyCriticalRate;
	newavt.pfPhysicalCriticalDamageRate = &avt->fPhysicalCriticalDamageRate;
	newavt.pfEnergyCriticalDamageRate = &avt->fEnergyCriticalDamageRate;
	newavt.pfRunSpeed = &avt->fRunSpeed;
	newavt.pfFlySpeed = &avt->fFlySpeed;
	newavt.pfFlyDashSpeed = &avt->fFlyDashSpeed;
	newavt.pfFlyAccelSpeed = &avt->fFlyAccelSpeed;
	newavt.pwAttackSpeedRate = &avt->wAttackSpeedRate;
	newavt.pfAttackRange = &avt->fAttackRange;
	newavt.pfCastingTimeChangePercent = &avt->fCastingTimeChangePercent;
	newavt.pfCoolTimeChangePercent = &avt->fCoolTimeChangePercent;
	newavt.pfKeepTimeChangePercent = &avt->fKeepTimeChangePercent;
	newavt.pfDotTimeChangeAbsolute = &avt->fDotTimeChangeAbsolute;
	newavt.pfRequiredEpChangePercent = &avt->fRequiredEpChangePercent;
	newavt.pfPhysicalReflection = &avt->fPhysicalReflection;
	newavt.pfEnergyReflection = &avt->fEnergyReflection;
	newavt.pwParalyzeToleranceRate = &avt->wParalyzeToleranceRate;
	newavt.pwTerrorToleranceRate = &avt->wTerrorToleranceRate;
	newavt.pwConfuseToleranceRate = &avt->wConfuseToleranceRate;
	newavt.pwStoneToleranceRate = &avt->wStoneToleranceRate;
	newavt.pwCandyToleranceRate = &avt->wCandyToleranceRate;
	newavt.pfParalyzeKeepTimeDown = &avt->fParalyzeKeepTimeDown;
	newavt.pfTerrorKeepTimeDown = &avt->fTerrorKeepTimeDown;
	newavt.pfConfuseKeepTimeDown = &avt->fConfuseKeepTimeDown;
	newavt.pfStoneKeepTimeDown = &avt->fStoneKeepTimeDown;
	newavt.pfCandyKeepTimeDown = &avt->fCandyKeepTimeDown;
	newavt.pfBleedingKeepTimeDown = &avt->fBleedingKeepTimeDown;
	newavt.pfPoisonKeepTimeDown = &avt->fPoisonKeepTimeDown;
	newavt.pfStomachacheKeepTimeDown = &avt->fStomachacheKeepTimeDown;
	newavt.pfCriticalBlockSuccessRate = &avt->fCriticalBlockSuccessRate;
	newavt.pdwLpRecoveryWhenHit = &avt->dwLpRecoveryWhenHit;
	newavt.pfLpRecoveryWhenHitInPercent = &avt->fLpRecoveryWhenHitInPercent;
	newavt.pdwEpRecoveryWhenHit = &avt->dwEpRecoveryWhenHit;
	newavt.pfEpRecoveryWhenHitInPercent = &avt->fEpRecoveryWhenHitInPercent;
	newavt.pwStomachacheDefence = &avt->wStomachacheDefence;
	newavt.pwPoisonDefence = &avt->wPoisonDefence;
	newavt.pwBleedDefence = &avt->wBleedDefence;
	newavt.pwBurnDefence = &avt->wBurnDefence;
	newavt.pfMindCurseImmunity = &avt->fMindCurseImmunity;
	newavt.pfBodyCurseImmunity = &avt->fBodyCurseImmunity;
	newavt.pfChangeCurseImmunity = &avt->fChangeCurseImmunity;
	newavt.pfSkillAnimationSpeedModifier = &avt->fSkillAnimationSpeedModifier;
	newavt.pdwWeightLimit = &avt->dwWeightLimit;
	newavt.pfSkillAggroBonus = &avt->fSkillAggroBonus;
	newavt.pfSkillAggroBonusInPercent = &avt->fSkillAggroBonusInPercent;
	newavt.pfDirectHealPowerBonus = &avt->fDirectHealPowerBonus;
	newavt.pfDirectHealPowerBonusInPercent = &avt->fDirectHealPowerBonusInPercent;
	newavt.pfHotPowerBonus = &avt->fHotPowerBonus;
	newavt.pfHotPowerBonusInPercent = &avt->fHotPowerBonusInPercent;
	newavt.pfDotValueChangePercent = &avt->fDotValueChangePercent;
	newavt.pfPhysicalCriticalDefenceRate = &avt->fPhysicalCriticalDefenceRate;
	newavt.pfEnergyCriticalDefenceRate = &avt->fEnergyCriticalDefenceRate;
	newavt.pwGuardRate = &avt->wGuardRate;
	newavt.pfSkillDamageBlockModeSuccessRate = &avt->fSkillDamageBlockModeSuccessRate;
	newavt.pfCurseBlockModeSuccessRate = &avt->fCurseBlockModeSuccessRate;
	newavt.pfKnockdownBlockModeSuccessRate = &avt->fKnockdownBlockModeSuccessRate;
	newavt.pfHtbBlockModeSuccessRate = &avt->fHtbBlockModeSuccessRate;
	newavt.pfItemUpgradeBonusRate = &avt->fItemUpgradeBonusRate;
	newavt.pbyExpBooster = &avt->byExpBooster;
	newavt.pbyQuestDropRate = &avt->byQuestDropRate;
	newavt.pfPhysicalArmorPenRate = &avt->fPhysicalArmorPenRate;
	newavt.pfEnergyArmorPenRate = &avt->fEnergyArmorPenRate;

	return newavt;
}


