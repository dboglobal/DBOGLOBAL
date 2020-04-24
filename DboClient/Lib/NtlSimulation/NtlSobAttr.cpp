#include "precomp_ntlsimulation.h"
#include "NtlSobAttr.h"

//core
#include "NtlDebug.h"

// CNtlSLAttrFactory* CNtlSLAttrFactory::m_pFactory[MAX_SLCLASS] = { NULL, };

CNtlSobLifeAttr::CNtlSobLifeAttr()
{
	m_byLevel	= 1;
	m_iLp		= 0;
	m_iMaxLp	= 1;
	m_iEp		= 0;
	m_iMaxEp	= 1;
	m_iMaxRp	= 1;
	m_fRadius	= 0.0f;

	m_wstrName	= L"NONE";
	m_NameColor	= RGB(255, 255, 255);
	m_NickNameColor = RGB(255, 255, 255);

	m_bNameCreate = TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

CNtlSobMoveableAttr::CNtlSobMoveableAttr()
{
	m_fDefaultWalkSpeed				= 7.0f;
	m_fDefaultRunSpeed				= 7.0f;
	m_fWalkSpeed					= 7.0f;
	m_fRunSpeed						= 7.0f;
	m_fFlySpeed						= 7.0f;
	m_fFlyDashSpeed					= 7.0f;
	m_fFlyAccelSpeed				= 7.0f;
	m_fSkillAnimationSpeedModifier	= 100.0f;
	m_fAttackAnimSpeed				= 1.0f;    
    m_fRunAnimSpeed					= 1.0f;
	m_fFlyHeight					= 0.0f;
	m_fScale						= 1.0f;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

CNtlSobBattleAttr::CNtlSobBattleAttr()
{
	m_Str = 0;
	m_Con = 0;
	m_Foc = 0;
	m_Dex = 0;
	m_Sol = 0;
	m_Eng = 0;

	m_wLpRegen = 0;
	m_wLpSitdownRegen = 0;
	m_wLpBattleRegen = 0;
	m_wEpRegen = 0;
	m_wEpSitdownRegen = 0;
	m_wEpBattleRegen = 0;

	m_wPhysicalOffence = 0;
	m_wPhysicalDefence = 0;
	m_wEnergyOffence = 0;
	m_wEnergyDefence = 0;
	m_wAttackRate = 0;
	m_wDodgeRate = 0;
	m_wBlockRate = 0;
	m_wBlockDamageRate = 0;
	m_wCurseSuccessRate = 0;
	m_wCurseToleranceRate = 0;
	m_wPhysicalCriticalRate = 0;
	m_wEnergyCriticalRate = 0;
	m_fPhysicalCriticalDamageRate = 0;
	m_fEnergyCriticalDamageRate = 0;
	m_fPhysicalReflection = 0;
	m_fEnergyReflection = 0;
	m_wParalyzeToleranceRate = 0;
	m_wTerrorToleranceRate = 0;
	m_wConfuseToleranceRate = 0;
	m_wStoneToleranceRate = 0;
	m_wCandyToleranceRate = 0;
	m_fParalyzeKeepTimeDown = 0;
	m_fTerrorKeepTimeDown = 0;
	m_fConfuseKeepTimeDown = 0;
	m_fStoneKeepTimeDown = 0;
	m_fCandyKeepTimeDown = 0;
	m_fBleedingKeepTimeDown = 0;
	m_fPoisonKeepTimeDown = 0;
	m_fStomachacheKeepTimeDown = 0;
	m_fCriticalBlockSuccessRate = 0;
	m_dwLpRecoveryWhenHit = 0;
	m_fLpRecoveryWhenHitInPercent = 0;
	m_dwEpRecoveryWhenHit = 0;
	m_fEpRecoveryWhenHitInPercent = 0;
	m_wStomachacheDefence = 0;
	m_wPoisonDefence = 0;
	m_wBleedDefence = 0;
	m_wBurnDefence = 0;
	m_fMindCurseImmunity = 0;
	m_fBodyCurseImmunity = 0;
	m_fChangeCurseImmunity = 0;

	m_fSkillAggroBonus = 0;
	m_fSkillAggroBonusInPercent = 0;
	m_fDirectHealPowerBonus = 0;
	m_fDirectHealPowerBonusInPercent = 0;
	m_fHotPowerBonus = 0;
	m_fHotPowerBonusInPercent = 0;
	m_fDotValueChangePercent = 0;
	m_wGuardRate = 0;
	m_fSkillDamageBlockModeSuccessRate = 0;
	m_fCurseBlockModeSuccessRate = 0;
	m_fKnockdownBlockModeSuccessRate = 0;
	m_fHtbBlockModeSuccessRate = 0;

	m_wAttackSpeedRate = 0;

	m_fPhysicalArmorPenRate = 0.0f;
	m_fEnergyArmorPenRate = 0.0f;

	m_fPhysicalCriticalDefenceRate = 0.0f;
	m_fEnergyCriticalDefenceRate = 0.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

CNtlSobSocialAttr::CNtlSobSocialAttr()
{
	m_byRace		= 0;
	m_uiRaceFlag	= 0;
	m_byGender		= 0;
	m_byClass		= 0;
	m_byGender		= 0;
    m_bIsAdult      = FALSE;

	m_byFace		= 0;
	m_byHair		= 0;
	m_byHairColor	= 0;
	m_bySkinColor	= 0;

	m_uiReputation	= 0;
}
