#include "precomp_ntlsimulation.h"
#include "NtlSobPetAttr.h"

// shared
#include "NPCTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogicDef.h"


DEFINITION_MEMORY_POOL(CNtlSobPetAttr)

CNtlSobPetAttr::CNtlSobPetAttr()
{
	m_bySummonSourceType	= 0;
	m_uiSourceTblId			= 0;
	m_pNpcTbl				= NULL;
}

void CNtlSobPetAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobPetCreate *pSobPetCreate = (SNtlEventSobPetCreate*)pMsg.pData;

		CNPCTable *pNpcTbl		= API_GetTableContainer()->GetNpcTable();
		CTextTable *pTextTable	= API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();

		if(pSobPetCreate->bOwnerAvatar)
		{
			sSUMMON_PET_PROFILE *pPetProfile = pSobPetCreate->uPetBrief.pPetProfile;
			sAVATAR_ATTRIBUTE *pAttrData = &pPetProfile->attribute;

			m_pNpcTbl = (sNPC_TBLDAT*)pNpcTbl->FindData(pPetProfile->npcTblidx);

			m_bySummonSourceType = pPetProfile->bySourceType;
			m_uiSourceTblId = pPetProfile->sourceTblidx;
			
			SetLevel(m_pNpcTbl->byLevel);
			SetFlyHeight(m_pNpcTbl->fFly_Height);
			SetScale(m_pNpcTbl->fScale);
			SetAttackRange(m_pNpcTbl->fAttack_Range);
			SetRadius(m_pNpcTbl->fRadius);
			SetDefaultWalkSpeed(m_pNpcTbl->fWalk_Speed);
			SetDefaultRunSpeed(m_pNpcTbl->fRun_Speed);

			SetLp(pPetProfile->curLp);
			SetEp(pPetProfile->wCurEP);
			SetMaxLp(pAttrData->MaxLp);
			SetMaxEp((RwInt32)pAttrData->wMaxEP);

			
			m_Str = pAttrData->Str;
			m_Con = pAttrData->Con;
			m_Foc = pAttrData->Foc;
			m_Dex = pAttrData->Dex;
			m_Sol = pAttrData->Sol;
			m_Eng = pAttrData->Eng;

			m_wLpRegen = pAttrData->wLpRegen;
			m_wLpSitdownRegen = pAttrData->wLpSitdownRegen;
			m_wLpBattleRegen = pAttrData->wLpBattleRegen;
			m_wEpRegen = pAttrData->wEpRegen;
			m_wEpSitdownRegen = pAttrData->wEpSitdownRegen;
			m_wEpBattleRegen = pAttrData->wEpBattleRegen;

			m_wPhysicalOffence = pAttrData->wPhysicalOffence;
			m_wPhysicalDefence = pAttrData->wPhysicalDefence;
			m_wEnergyOffence = pAttrData->wEnergyOffence;
			m_wEnergyDefence = pAttrData->wEnergyDefence;
			m_wAttackRate = pAttrData->wAttackRate;
			m_wDodgeRate = pAttrData->wDodgeRate;
			m_wBlockRate = pAttrData->wBlockRate;
			m_wBlockDamageRate = pAttrData->wBlockDamageRate;
			m_wCurseSuccessRate = pAttrData->wCurseSuccessRate;
			m_wCurseToleranceRate = pAttrData->wCurseToleranceRate;
			m_wPhysicalCriticalRate = pAttrData->wPhysicalCriticalRate;
			m_wEnergyCriticalRate = pAttrData->wEnergyCriticalRate;
			m_fPhysicalCriticalDamageRate = pAttrData->fPhysicalCriticalDamageRate;
			m_fEnergyCriticalDamageRate = pAttrData->fEnergyCriticalDamageRate;

			SetRunSpeed(pAttrData->fRunSpeed);
			SetFlySpeed(pAttrData->fFlySpeed);
			SetFlyDashSpeed(pAttrData->fFlyDashSpeed);
			SetFlyAccelSpeed(pAttrData->fFlyAccelSpeed);
			SetAttackRange(pAttrData->fAttackRange);
			SetCastingTimeModifier(pAttrData->fCastingTimeChangePercent);
			SetCoolingTimeModifier(pAttrData->fCoolTimeChangePercent);
			SetKeepingTimeModifier(pAttrData->fKeepTimeChangePercent);
			SetDOTValueModifier(pAttrData->fDotValueChangePercent);
			SetDOTTimeModifier(pAttrData->fDotTimeChangeAbsolute);
			SetRequiredEPModifier(pAttrData->fRequiredEpChangePercent);

			m_wAttackSpeedRate = pAttrData->wAttackSpeedRate;

			if (m_wAttackSpeedRate > 0)
				SetAttackAnimSpeed((RwReal)1000 / (RwReal)m_wAttackSpeedRate);


			m_fPhysicalReflection = pAttrData->fPhysicalReflection;
			m_fEnergyReflection = pAttrData->fEnergyReflection;
			m_wParalyzeToleranceRate = pAttrData->wParalyzeToleranceRate;
			m_wTerrorToleranceRate = pAttrData->wTerrorToleranceRate;
			m_wConfuseToleranceRate = pAttrData->wConfuseToleranceRate;
			m_wStoneToleranceRate = pAttrData->wStoneToleranceRate;
			m_wCandyToleranceRate = pAttrData->wCandyToleranceRate;
			m_fParalyzeKeepTimeDown = pAttrData->fParalyzeKeepTimeDown;
			m_fTerrorKeepTimeDown = pAttrData->fTerrorKeepTimeDown;
			m_fConfuseKeepTimeDown = pAttrData->fConfuseKeepTimeDown;
			m_fStoneKeepTimeDown = pAttrData->fStoneKeepTimeDown;
			m_fCandyKeepTimeDown = pAttrData->fCandyKeepTimeDown;
			m_fBleedingKeepTimeDown = pAttrData->fBleedingKeepTimeDown;
			m_fPoisonKeepTimeDown = pAttrData->fPoisonKeepTimeDown;
			m_fStomachacheKeepTimeDown = pAttrData->fStomachacheKeepTimeDown;
			m_fCriticalBlockSuccessRate = pAttrData->fCriticalBlockSuccessRate;
			m_dwLpRecoveryWhenHit = pAttrData->dwLpRecoveryWhenHit;
			m_fLpRecoveryWhenHitInPercent = pAttrData->fLpRecoveryWhenHitInPercent;
			m_dwEpRecoveryWhenHit = pAttrData->dwEpRecoveryWhenHit;
			m_fEpRecoveryWhenHitInPercent = pAttrData->fEpRecoveryWhenHitInPercent;
			m_wStomachacheDefence = pAttrData->wStomachacheDefence;
			m_wPoisonDefence = pAttrData->wPoisonDefence;
			m_wBleedDefence = pAttrData->wBleedDefence;
			m_wBurnDefence = pAttrData->wBurnDefence;
			m_fMindCurseImmunity = pAttrData->fMindCurseImmunity;
			m_fBodyCurseImmunity = pAttrData->fBodyCurseImmunity;
			m_fChangeCurseImmunity = pAttrData->fChangeCurseImmunity;
			SetSkillAnimationSpeedModifier(pAttrData->fSkillAnimationSpeedModifier);
			m_fSkillAggroBonus = pAttrData->fSkillAggroBonus;
			m_fSkillAggroBonusInPercent = pAttrData->fSkillAggroBonusInPercent;
			m_fDirectHealPowerBonus = pAttrData->fDirectHealPowerBonus;
			m_fDirectHealPowerBonusInPercent = pAttrData->fDirectHealPowerBonusInPercent;
			m_fHotPowerBonus = pAttrData->fHotPowerBonus;
			m_fHotPowerBonusInPercent = pAttrData->fHotPowerBonusInPercent;
			m_fDotValueChangePercent = pAttrData->fDotValueChangePercent;
			m_wGuardRate = pAttrData->wGuardRate;
			m_fSkillDamageBlockModeSuccessRate = pAttrData->fSkillDamageBlockModeSuccessRate;
			m_fCurseBlockModeSuccessRate = pAttrData->fCurseBlockModeSuccessRate;
			m_fKnockdownBlockModeSuccessRate = pAttrData->fKnockdownBlockModeSuccessRate;
			m_fHtbBlockModeSuccessRate = pAttrData->fHtbBlockModeSuccessRate;

			m_fPhysicalArmorPenRate = pAttrData->fPhysicalArmorPenRate;
			m_fEnergyArmorPenRate = pAttrData->fEnergyArmorPenRate;

			m_fPhysicalCriticalDefenceRate = pAttrData->fPhysicalCriticalDefenceRate;
			m_fEnergyCriticalDefenceRate = pAttrData->fEnergyCriticalDefenceRate;
		}
		else
		{
			sSUMMON_PET_BRIEF *pPetBrief = pSobPetCreate->uPetBrief.pPetBrief;

			m_pNpcTbl	= (sNPC_TBLDAT*)pNpcTbl->FindData(pPetBrief->npcTblidx);
			SetLevel(m_pNpcTbl->byLevel);
			SetLp((RwInt32)pPetBrief->curLp);
			SetMaxLp((RwInt32)pPetBrief->maxLp);
			SetEp(pPetBrief->wCurEP);
			SetMaxEp(pPetBrief->wMaxEP);
			SetRadius(m_pNpcTbl->fRadius);

			SetDefaultWalkSpeed(m_pNpcTbl->fWalk_Speed);
			SetDefaultRunSpeed(m_pNpcTbl->fRun_Speed);
			SetWalkSpeed(m_pNpcTbl->fWalk_Speed);
			SetRunSpeed(pPetBrief->fLastRunningSpeed);
			if(pPetBrief->wAttackSpeedRate > 0)
				SetAttackAnimSpeed((RwReal)1000/(RwReal)pPetBrief->wAttackSpeedRate);
			SetSkillAnimationSpeedModifier(pPetBrief->fSkillAnimationSpeedModifier);
			SetFlyHeight(m_pNpcTbl->fFly_Height);
			SetScale(m_pNpcTbl->fScale);
			SetAttackRange(m_pNpcTbl->fAttack_Range);


			m_bySummonSourceType	= pPetBrief->bySourceType;
			m_uiSourceTblId			= pPetBrief->sourceTblidx;
		}

		
		SetModelName(m_pNpcTbl->szModel);

		sTEXT_TBLDAT* pTextTblData = reinterpret_cast<sTEXT_TBLDAT*>( pTextTable->FindData( m_pNpcTbl->Name ) );
		if(pTextTblData)
			SetName(pTextTblData->wstrText.c_str());

		SetNameColor(NTL_NPC_NAME_COLOR);
	}
}


sNPC_TBLDAT* CNtlSobPetAttr::GetNpcTbl(void) const
{
	return m_pNpcTbl;
}
