#include "precomp_ntlsimulation.h"
#include "NtlSobAvatarAttr.h"

// shared
#include "PCTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSLApi.h"
#include "NtlSLLogicDef.h"



DEFINITION_MEMORY_POOL(CNtlSobAvatarAttr)

CNtlSobAvatarAttr::CNtlSobAvatarAttr() 
{
	m_uiExp = 1;
	m_uiMaxExp = 1;

	m_uiZenny = 0;
	m_uiSp = 0;

	m_uiHonorPoint = 0;
	m_uiMudosaPoint = 0;			///< 무도사 포인트

    // PC방 관련
    m_dwNetPy = 0;
	m_dwHlsCash = 0;

	///
	
	m_wApRegen = 0;
	m_wApSitdownRegen = 0;
	m_wApDegen = 0;

	m_wRpRegen = 0;
	m_wRpDimimutionRate = 0;

	m_dwWeightLimit = 0;
	m_fItemUpgradeBonusRate = 0.;
	m_byExpBooster = 0;
	m_byQuestDropRate = 0;

}

void CNtlSobAvatarAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SAvatarInfo *pAvatarInfo	= GetNtlSLGlobal()->GetAvatarInfo();
		CPCTable *pPcTbl			= API_GetTableContainer()->GetPcTable();
		m_pPcTbl					= (sPC_TBLDAT*)pPcTbl->FindData(pAvatarInfo->sCharPf.tblidx);

		SetLevel(pAvatarInfo->sCharPf.byLevel);
		SetLp(pAvatarInfo->sCharPf.curLp);
		SetMaxLp((RwInt32)pAvatarInfo->sCharPf.avatarAttribute.MaxLp);
		SetEp((RwInt32)pAvatarInfo->sCharPf.wCurEP);
		SetMaxEp((RwInt32)pAvatarInfo->sCharPf.avatarAttribute.wMaxEP);
		SetAp(pAvatarInfo->sCharPf.curAP);
		SetMaxAp((RwInt32)pAvatarInfo->sCharPf.avatarAttribute.MaxAp);
		SetMaxRp((RwInt32)pAvatarInfo->sCharPf.avatarAttribute.wMaxRP);
		SetRadius(m_pPcTbl->fRadius);

        SetAdult(pAvatarInfo->sCharPf.bIsAdult);
        SetModelName(pAvatarInfo->sCharPf.bIsAdult ? m_pPcTbl->szModel_Adult : m_pPcTbl->szModel_Child);

		SetName(pAvatarInfo->sCharPf.awchName);
		SetNameColor(NTL_PLAYER_NAME_COLOR);

		SetRace(m_pPcTbl->byRace);
		SetRaceFlag(m_pPcTbl->dwClass_Bit_Flag);
		SetClass(m_pPcTbl->byClass);
		SetGender(m_pPcTbl->byGender);
		SetFace(pAvatarInfo->sCharPf.sPcShape.byFace);
		SetHair(pAvatarInfo->sCharPf.sPcShape.byHair);
		SetHairColor(pAvatarInfo->sCharPf.sPcShape.byHairColor);
		SetSkinColor(pAvatarInfo->sCharPf.sPcShape.bySkinColor);

		SetReputation(pAvatarInfo->sCharPf.dwReputation);

		m_uiZenny = pAvatarInfo->sCharPf.dwZenny;
		m_uiSp = pAvatarInfo->sCharPf.dwSpPoint;
		m_uiMudosaPoint = pAvatarInfo->sCharPf.dwMudosaPoint;

		m_bCanChangeClass = pAvatarInfo->sCharPf.bChangeClass;
				
		m_uiExp			= pAvatarInfo->sCharPf.dwCurExp; 
		m_uiMaxExp		= pAvatarInfo->sCharPf.dwMaxExpInThisLevel;

		
		m_Str = pAvatarInfo->sCharPf.avatarAttribute.Str;
		m_Con = pAvatarInfo->sCharPf.avatarAttribute.Con;
		m_Foc = pAvatarInfo->sCharPf.avatarAttribute.Foc;
		m_Dex = pAvatarInfo->sCharPf.avatarAttribute.Dex;
		m_Sol = pAvatarInfo->sCharPf.avatarAttribute.Sol;
		m_Eng = pAvatarInfo->sCharPf.avatarAttribute.Eng;

		m_wLpRegen = pAvatarInfo->sCharPf.avatarAttribute.wLpRegen;
		m_wLpSitdownRegen = pAvatarInfo->sCharPf.avatarAttribute.wLpSitdownRegen;
		m_wLpBattleRegen = pAvatarInfo->sCharPf.avatarAttribute.wLpBattleRegen;
		m_wEpRegen = pAvatarInfo->sCharPf.avatarAttribute.wEpRegen;
		m_wEpSitdownRegen = pAvatarInfo->sCharPf.avatarAttribute.wEpSitdownRegen;
		m_wEpBattleRegen = pAvatarInfo->sCharPf.avatarAttribute.wEpBattleRegen;

		m_wPhysicalOffence = pAvatarInfo->sCharPf.avatarAttribute.wPhysicalOffence;
		m_wPhysicalDefence = pAvatarInfo->sCharPf.avatarAttribute.wPhysicalDefence;
		m_wEnergyOffence = pAvatarInfo->sCharPf.avatarAttribute.wEnergyOffence;
		m_wEnergyDefence = pAvatarInfo->sCharPf.avatarAttribute.wEnergyDefence;
		m_wAttackRate = pAvatarInfo->sCharPf.avatarAttribute.wAttackRate;
		m_wDodgeRate = pAvatarInfo->sCharPf.avatarAttribute.wDodgeRate;
		m_wBlockRate = pAvatarInfo->sCharPf.avatarAttribute.wBlockRate;
		m_wBlockDamageRate = pAvatarInfo->sCharPf.avatarAttribute.wBlockDamageRate;
		m_wCurseSuccessRate = pAvatarInfo->sCharPf.avatarAttribute.wCurseSuccessRate;
		m_wCurseToleranceRate = pAvatarInfo->sCharPf.avatarAttribute.wCurseToleranceRate;
		m_wPhysicalCriticalRate = pAvatarInfo->sCharPf.avatarAttribute.wPhysicalCriticalRate;
		m_wEnergyCriticalRate = pAvatarInfo->sCharPf.avatarAttribute.wEnergyCriticalRate;
		m_fPhysicalCriticalDamageRate = pAvatarInfo->sCharPf.avatarAttribute.fPhysicalCriticalDamageRate;
		m_fEnergyCriticalDamageRate = pAvatarInfo->sCharPf.avatarAttribute.fEnergyCriticalDamageRate;

		SetRunSpeed(pAvatarInfo->sCharPf.avatarAttribute.fRunSpeed);
		SetFlySpeed(pAvatarInfo->sCharPf.avatarAttribute.fFlySpeed);
		SetFlyDashSpeed(pAvatarInfo->sCharPf.avatarAttribute.fFlyDashSpeed);
		SetFlyAccelSpeed(pAvatarInfo->sCharPf.avatarAttribute.fFlyAccelSpeed);
		SetAttackRange(pAvatarInfo->sCharPf.avatarAttribute.fAttackRange);
		SetCastingTimeModifier(pAvatarInfo->sCharPf.avatarAttribute.fCastingTimeChangePercent);
		SetCoolingTimeModifier(pAvatarInfo->sCharPf.avatarAttribute.fCoolTimeChangePercent);
		SetKeepingTimeModifier(pAvatarInfo->sCharPf.avatarAttribute.fKeepTimeChangePercent);
		SetDOTValueModifier(pAvatarInfo->sCharPf.avatarAttribute.fDotValueChangePercent);
		SetDOTTimeModifier(pAvatarInfo->sCharPf.avatarAttribute.fDotTimeChangeAbsolute);
		SetRequiredEPModifier(pAvatarInfo->sCharPf.avatarAttribute.fRequiredEpChangePercent);

		m_wAttackSpeedRate = pAvatarInfo->sCharPf.avatarAttribute.wAttackSpeedRate;

		if(m_wAttackSpeedRate > 0)
			SetAttackAnimSpeed(1000.f/(float)m_wAttackSpeedRate);

        if(IsAdult())
        {
            SetDefaultRunSpeed(m_pPcTbl->fAdult_Run_Speed);            
            SetRunAnimSpeed(m_pPcTbl->fAdult_Run_Speed / m_pPcTbl->fAdult_Run_Speed_Origin);           
        }
        else
        {
            SetDefaultRunSpeed(m_pPcTbl->fChild_Run_Speed);            
            SetRunAnimSpeed(m_pPcTbl->fChild_Run_Speed / m_pPcTbl->fChild_Run_Speed_Origin);
        }
		

		m_fPhysicalReflection = pAvatarInfo->sCharPf.avatarAttribute.fPhysicalReflection;
		m_fEnergyReflection = pAvatarInfo->sCharPf.avatarAttribute.fEnergyReflection;
		m_wParalyzeToleranceRate = pAvatarInfo->sCharPf.avatarAttribute.wParalyzeToleranceRate;
		m_wTerrorToleranceRate = pAvatarInfo->sCharPf.avatarAttribute.wTerrorToleranceRate;
		m_wConfuseToleranceRate = pAvatarInfo->sCharPf.avatarAttribute.wConfuseToleranceRate;
		m_wStoneToleranceRate = pAvatarInfo->sCharPf.avatarAttribute.wStoneToleranceRate;
		m_wCandyToleranceRate = pAvatarInfo->sCharPf.avatarAttribute.wCandyToleranceRate;
		m_fParalyzeKeepTimeDown = pAvatarInfo->sCharPf.avatarAttribute.fParalyzeKeepTimeDown;
		m_fTerrorKeepTimeDown = pAvatarInfo->sCharPf.avatarAttribute.fTerrorKeepTimeDown;
		m_fConfuseKeepTimeDown = pAvatarInfo->sCharPf.avatarAttribute.fConfuseKeepTimeDown;
		m_fStoneKeepTimeDown = pAvatarInfo->sCharPf.avatarAttribute.fStoneKeepTimeDown;
		m_fCandyKeepTimeDown = pAvatarInfo->sCharPf.avatarAttribute.fCandyKeepTimeDown;
		m_fBleedingKeepTimeDown = pAvatarInfo->sCharPf.avatarAttribute.fBleedingKeepTimeDown;
		m_fPoisonKeepTimeDown = pAvatarInfo->sCharPf.avatarAttribute.fPoisonKeepTimeDown;
		m_fStomachacheKeepTimeDown = pAvatarInfo->sCharPf.avatarAttribute.fStomachacheKeepTimeDown;
		m_fCriticalBlockSuccessRate = pAvatarInfo->sCharPf.avatarAttribute.fCriticalBlockSuccessRate;
		m_dwLpRecoveryWhenHit = pAvatarInfo->sCharPf.avatarAttribute.dwLpRecoveryWhenHit;
		m_fLpRecoveryWhenHitInPercent = pAvatarInfo->sCharPf.avatarAttribute.fLpRecoveryWhenHitInPercent;
		m_dwEpRecoveryWhenHit = pAvatarInfo->sCharPf.avatarAttribute.dwEpRecoveryWhenHit;
		m_fEpRecoveryWhenHitInPercent = pAvatarInfo->sCharPf.avatarAttribute.fEpRecoveryWhenHitInPercent;
		m_wStomachacheDefence = pAvatarInfo->sCharPf.avatarAttribute.wStomachacheDefence;
		m_wPoisonDefence = pAvatarInfo->sCharPf.avatarAttribute.wPoisonDefence;
		m_wBleedDefence = pAvatarInfo->sCharPf.avatarAttribute.wBleedDefence;
		m_wBurnDefence = pAvatarInfo->sCharPf.avatarAttribute.wBurnDefence;
		m_fMindCurseImmunity = pAvatarInfo->sCharPf.avatarAttribute.fMindCurseImmunity;
		m_fBodyCurseImmunity = pAvatarInfo->sCharPf.avatarAttribute.fBodyCurseImmunity;
		m_fChangeCurseImmunity = pAvatarInfo->sCharPf.avatarAttribute.fChangeCurseImmunity;
		SetSkillAnimationSpeedModifier(pAvatarInfo->sCharPf.avatarAttribute.fSkillAnimationSpeedModifier);
		m_fSkillAggroBonus = pAvatarInfo->sCharPf.avatarAttribute.fSkillAggroBonus;
		m_fSkillAggroBonusInPercent = pAvatarInfo->sCharPf.avatarAttribute.fSkillAggroBonusInPercent;
		m_fDirectHealPowerBonus = pAvatarInfo->sCharPf.avatarAttribute.fDirectHealPowerBonus;
		m_fDirectHealPowerBonusInPercent = pAvatarInfo->sCharPf.avatarAttribute.fDirectHealPowerBonusInPercent;
		m_fHotPowerBonus = pAvatarInfo->sCharPf.avatarAttribute.fHotPowerBonus;
		m_fHotPowerBonusInPercent = pAvatarInfo->sCharPf.avatarAttribute.fHotPowerBonusInPercent;
		m_fDotValueChangePercent = pAvatarInfo->sCharPf.avatarAttribute.fDotValueChangePercent;
		m_wGuardRate = pAvatarInfo->sCharPf.avatarAttribute.wGuardRate;
		m_fSkillDamageBlockModeSuccessRate = pAvatarInfo->sCharPf.avatarAttribute.fSkillDamageBlockModeSuccessRate;
		m_fCurseBlockModeSuccessRate = pAvatarInfo->sCharPf.avatarAttribute.fCurseBlockModeSuccessRate;
		m_fKnockdownBlockModeSuccessRate = pAvatarInfo->sCharPf.avatarAttribute.fKnockdownBlockModeSuccessRate;
		m_fHtbBlockModeSuccessRate = pAvatarInfo->sCharPf.avatarAttribute.fHtbBlockModeSuccessRate;

		m_wRpRegen = pAvatarInfo->sCharPf.avatarAttribute.wRpRegen;
		m_wRpDimimutionRate = pAvatarInfo->sCharPf.avatarAttribute.wRpDimimutionRate;

		m_wApRegen = pAvatarInfo->sCharPf.avatarAttribute.wApRegen;
		m_wApSitdownRegen = pAvatarInfo->sCharPf.avatarAttribute.wApSitdownRegen;
		m_wApDegen = pAvatarInfo->sCharPf.avatarAttribute.wApDegen;

		m_dwWeightLimit = pAvatarInfo->sCharPf.avatarAttribute.dwWeightLimit;
		m_fItemUpgradeBonusRate = pAvatarInfo->sCharPf.avatarAttribute.fItemUpgradeBonusRate;
		m_byExpBooster = pAvatarInfo->sCharPf.avatarAttribute.byExpBooster;
		m_byQuestDropRate = pAvatarInfo->sCharPf.avatarAttribute.byQuestDropRate;

		m_fPhysicalArmorPenRate = pAvatarInfo->sCharPf.avatarAttribute.fPhysicalArmorPenRate;
		m_fEnergyArmorPenRate = pAvatarInfo->sCharPf.avatarAttribute.fEnergyArmorPenRate;

		m_fPhysicalCriticalDefenceRate = pAvatarInfo->sCharPf.avatarAttribute.fPhysicalCriticalDefenceRate;
		m_fEnergyCriticalDefenceRate = pAvatarInfo->sCharPf.avatarAttribute.fEnergyCriticalDefenceRate;

		return;
	}

	CNtlSobPlayerAttr::HandleEvents(pMsg);
}

RwUInt32 CNtlSobAvatarAttr::GetAP(void)
{
	return Dbo_CalculatePowerLevel(	m_wPhysicalOffence, m_wPhysicalDefence, m_wEnergyOffence, m_wEnergyDefence,
									m_wAttackRate, m_wDodgeRate, m_wCurseSuccessRate, m_wCurseToleranceRate,
									m_wPhysicalCriticalRate, m_wEnergyCriticalRate, m_wAttackSpeedRate,
									(WORD)GetMaxLp(), (WORD)GetMaxEp(), GetLevel(), 0 );
}

