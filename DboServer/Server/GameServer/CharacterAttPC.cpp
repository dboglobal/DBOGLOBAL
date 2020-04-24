#include "stdafx.h"
#include "CharacterAttPC.h"
#include "CPlayer.h"
#include "item.h"
#include "VehicleTable.h"
#include "ItemOptionTable.h"
#include "TableContainerManager.h"
#include "CharTitleTable.h"
#include "calcs.h"
#include "StatusTransformTable.h"
#include "CommonConfigTable.h"
#include "FormulaTable.h"
#include "GameServer.h"


CCharacterAttPC::CCharacterAttPC()
{
}

CCharacterAttPC::~CCharacterAttPC()
{
}


bool CCharacterAttPC::Create(CPlayer* pPlayer)
{
	if (CCharacterAtt::Create(pPlayer))
	{
		m_pPlayerRef = pPlayer;
		return true;
	}

	return false;
}


void CCharacterAttPC::CalculateAtt()
{
	sCHAR_TBLDAT* charTbldat = m_pPlayerRef->GetTbldat();
	sPC_TBLDAT* pcTbldat = m_pPlayerRef->GetTbldat();
	float byLevel = (float)m_pPlayerRef->GetLevel();

	m_pAttribute.MaxLp = (int)charTbldat->dwBasic_LP;
	m_pAttribute.wMaxEP = charTbldat->wBasic_EP;
	m_pAttribute.wPhysicalDefence = charTbldat->wBasic_Physical_Defence;
	m_pAttribute.wEnergyDefence = charTbldat->wBasic_Energy_Defence;
	m_pAttribute.wAttackSpeedRate = charTbldat->wAttack_Speed_Rate;
	m_pAttribute.fAttackRange = charTbldat->fAttack_Range;
	m_pAttribute.wAttackRate = charTbldat->wAttack_Rate;
	m_pAttribute.wDodgeRate = charTbldat->wDodge_Rate;
	m_pAttribute.wBlockRate = charTbldat->wBlock_Rate;
	m_pAttribute.wCurseSuccessRate = charTbldat->wCurse_Success_Rate;
	m_pAttribute.wCurseToleranceRate = charTbldat->wCurse_Tolerance_Rate;

	m_pAttribute.Str = (WORD)(charTbldat->wBasicStr + pcTbldat->fLevel_Up_Str * byLevel);
	m_pAttribute.Con = (WORD)(charTbldat->wBasicCon + pcTbldat->fLevel_Up_Con * byLevel);
	m_pAttribute.Foc = (WORD)(charTbldat->wBasicFoc + pcTbldat->fLevel_Up_Foc * byLevel);
	m_pAttribute.Dex = (WORD)(charTbldat->wBasicDex + pcTbldat->fLevel_Up_Dex * byLevel);
	m_pAttribute.Sol = (WORD)(charTbldat->wBasicSol + pcTbldat->fLevel_Up_Sol * byLevel);
	m_pAttribute.Eng = (WORD)(charTbldat->wBasicEng + pcTbldat->fLevel_Up_Eng * byLevel);

	m_pAttribute.fSkillAnimationSpeedModifier = 100.0f;
	m_pAttribute.dwWeightLimit = pcTbldat->dwWeightLimit;
	m_pAttribute.MaxAp = DBO_CHAR_DEFAULT_AP;
	m_pAttribute.wMaxRP = WORD(CFormulaTable::m_afRate[2300][1] + byLevel * CFormulaTable::m_afRate[2300][2]);
	m_pAttribute.wRpRegen = 25;
	m_pAttribute.wRpDimimutionRate = 6;
	m_pAttribute.wApRegen = 5000;
	m_pAttribute.wApSitdownRegen = 10000;
	m_pAttribute.wApDegen = 5000;

	if (m_pPlayerRef->IsAdult())
	{
		m_pAttribute.fRunSpeed = pcTbldat->fAdult_Run_Speed;
		m_pAttribute.fFlySpeed = pcTbldat->fAdult_Fly_Speed;
		m_pAttribute.fFlyDashSpeed = pcTbldat->fAdult_Dash_Speed;
		m_pAttribute.fFlyAccelSpeed = pcTbldat->fAdult_Accel_Speed;
	}
	else
	{
		m_pAttribute.fRunSpeed = pcTbldat->fChild_Run_Speed;
		m_pAttribute.fFlySpeed = pcTbldat->fChild_Fly_Speed;
		m_pAttribute.fFlyDashSpeed = pcTbldat->fChild_Dash_Speed;
		m_pAttribute.fFlyAccelSpeed = pcTbldat->fChild_Accel_Speed;
	}

	m_pAttribute.MaxLp += int(CFormulaTable::m_afRate[200 + pcTbldat->byClass + 1][1] + m_pAttribute.Con * CFormulaTable::m_afRate[200 + pcTbldat->byClass + 1][2]);
	m_pAttribute.wMaxEP += WORD(CFormulaTable::m_afRate[1300 + pcTbldat->byClass + 1][1] + m_pAttribute.Eng * CFormulaTable::m_afRate[1300 + pcTbldat->byClass + 1][2]);
	m_pAttribute.wAttackRate += WORD(m_pAttribute.Foc / CFormulaTable::m_afRate[600][1] * CFormulaTable::m_afRate[600][2]);
	m_pAttribute.wDodgeRate += WORD(m_pAttribute.Dex / CFormulaTable::m_afRate[800][1] * CFormulaTable::m_afRate[800][2]);

	m_pAttribute.wPhysicalOffence += WORD(byLevel * CFormulaTable::m_afRate[100 + pcTbldat->byClass + 1][1] + m_pAttribute.Str * CFormulaTable::m_afRate[100 + pcTbldat->byClass + 1][2]);
	m_pAttribute.wPhysicalOffence += WORD(m_pAttribute.Dex * CFormulaTable::m_afRate[100 + pcTbldat->byClass + 1][3]);

	m_pAttribute.wBlockRate += WORD(CFormulaTable::m_afRate[900][1] + m_pAttribute.Dex * CFormulaTable::m_afRate[900][2] + m_pAttribute.Con * CFormulaTable::m_afRate[900][2]);
	m_pAttribute.wGuardRate += WORD(CFormulaTable::m_afRate[1000][1] + m_pAttribute.Dex * CFormulaTable::m_afRate[1000][2]);


	m_pAttribute.wEnergyOffence += WORD(byLevel * CFormulaTable::m_afRate[1100 + pcTbldat->byClass + 1][1] + m_pAttribute.Sol * CFormulaTable::m_afRate[1100 + pcTbldat->byClass + 1][2]);
	m_pAttribute.wEnergyOffence += WORD(m_pAttribute.Foc * CFormulaTable::m_afRate[1100 + pcTbldat->byClass + 1][3]);

	m_pAttribute.wLpRegen += WORD(CFormulaTable::m_afRate[300][1] + m_pAttribute.Con * CFormulaTable::m_afRate[300][2]);
	m_pAttribute.wLpSitdownRegen += WORD(m_pAttribute.wLpRegen * CFormulaTable::m_afRate[400][1]);
	m_pAttribute.wLpBattleRegen = WORD(m_pAttribute.wLpRegen * CFormulaTable::m_afRate[500][1]);
	m_pAttribute.wEpRegen += WORD(CFormulaTable::m_afRate[1400][1] + m_pAttribute.Eng * CFormulaTable::m_afRate[1400][2]);
	m_pAttribute.wEpSitdownRegen += WORD(m_pAttribute.wEpRegen * CFormulaTable::m_afRate[1500][1]);
	m_pAttribute.wEpBattleRegen = WORD(m_pAttribute.wLpRegen * CFormulaTable::m_afRate[1600][1]);

	m_pAttribute.wPhysicalCriticalRate += WORD(CFormulaTable::m_afRate[1900 + pcTbldat->byClass + 1][1] + m_pAttribute.Dex / CFormulaTable::m_afRate[1900 + pcTbldat->byClass + 1][2]);
	m_pAttribute.wEnergyCriticalRate += WORD(CFormulaTable::m_afRate[2100 + pcTbldat->byClass + 1][1] + m_pAttribute.Foc / CFormulaTable::m_afRate[2100 + pcTbldat->byClass + 1][2]);
	m_pAttribute.wCurseSuccessRate += WORD(m_pAttribute.Foc / CFormulaTable::m_afRate[700][1] * CFormulaTable::m_afRate[700][2]);
	m_pAttribute.wCurseToleranceRate += WORD(m_pAttribute.Dex / CFormulaTable::m_afRate[1200][1] * CFormulaTable::m_afRate[1200][2]);

	m_pAttribute.fPhysicalCriticalDamageRate += CFormulaTable::m_afRate[2000 + pcTbldat->byClass + 1][1] + m_pAttribute.Dex / CFormulaTable::m_afRate[2000 + pcTbldat->byClass + 1][2];
	m_pAttribute.fEnergyCriticalDamageRate += CFormulaTable::m_afRate[2200 + pcTbldat->byClass + 1][1] + m_pAttribute.Foc / CFormulaTable::m_afRate[2200 + pcTbldat->byClass + 1][2];

	//default equipment stats added to base
	m_pPlayerRef->GetPlayerItemContainer()->CopyBaseItemAttributesTo(m_pAttribute);

	//apply passive skills attributes
	m_pPlayerRef->GetSkillManager()->CopyPassiveAttributesTo(this);


	m_pPlayerRef->GetPlayerItemContainer()->CopyItemAttributesTo(this);

	//set char title attribute
	if (m_pPlayerRef->GetCharTitleID() != INVALID_TBLIDX)
	{
		sCHARTITLE_TBLDAT *charTitleTbldat = (sCHARTITLE_TBLDAT *)g_pTableContainer->GetCharTitleTable()->FindData(m_pPlayerRef->GetCharTitleID());
		if (charTitleTbldat)
		{
			for (BYTE i = 0; i < NTL_MAX_CHAR_TITLE_EFFECT; i++)
			{
				eSYSTEM_EFFECT_CODE effectcode = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(charTitleTbldat->atblSystem_Effect_Index[i]);
				if (effectcode != INVALID_SYSTEM_EFFECT_CODE)
				{
					Dbo_SetAvatarAttributeValue(this, effectcode, (float)charTitleTbldat->abySystem_Effect_Value[i], charTitleTbldat->abySystem_Effect_Type[i]);
				}
			}
		}
	}

	//buff attributes
	m_pPlayerRef->GetBuffManager()->CopyBuffAttributesTo(this);

	//transformation attributes
	if (m_pPlayerRef->GetTransformationTbldat())
	{
		sSTATUS_TRANSFORM_TBLDAT* pTransformationTbldat = m_pPlayerRef->GetTransformationTbldat();

		m_pAttribute.MaxLp = (int)(m_pAttribute.MaxLp * pTransformationTbldat->fLP_Transform);
		m_pAttribute.wMaxEP = (WORD)(m_pAttribute.wMaxEP * pTransformationTbldat->fEP_Transform);
		m_pAttribute.wPhysicalOffence = (WORD)(m_pAttribute.wPhysicalOffence * pTransformationTbldat->fPhysical_Offence_Transform);
		m_pAttribute.wEnergyOffence = (WORD)(m_pAttribute.wEnergyOffence * pTransformationTbldat->fEnergy_Offence_Transform);
		m_pAttribute.wPhysicalDefence = (WORD)(m_pAttribute.wPhysicalDefence * pTransformationTbldat->fPhysical_Defence_Transform);
		m_pAttribute.wEnergyDefence = (WORD)(m_pAttribute.wEnergyDefence * pTransformationTbldat->fEnergy_Defence_Transform);
		m_pAttribute.fRunSpeed = m_pAttribute.fRunSpeed * pTransformationTbldat->fRun_Speed_Transform;

		if (m_pPlayerRef->GetAspectStateId() == ASPECTSTATE_KAIOKEN)
		{
			float fSpeedBonus = 3.0f;

			if(m_pPlayerRef->GetTransformGrade() == 2)
				fSpeedBonus = 5.0f;
			else if (m_pPlayerRef->GetTransformGrade() >= 3)
				fSpeedBonus = 7.0f;

			CalculateAttackSpeedRate(fSpeedBonus, SYSTEM_EFFECT_APPLY_TYPE_PERCENT, false);
		}
		else
		{
			if (pTransformationTbldat->fAttack_Speed_Transform < 1.0f)
				CalculateAttackSpeedRate(100.f - pTransformationTbldat->fAttack_Speed_Transform * 100.f, SYSTEM_EFFECT_APPLY_TYPE_PERCENT, false); // * 100 because the value is like 0.75 = 0.75 * 100 = 75% and then - 100 to get the real %
			else
				CalculateAttackSpeedRate(pTransformationTbldat->fAttack_Speed_Transform * 100.f - 100.f, SYSTEM_EFFECT_APPLY_TYPE_PERCENT, true);
		}

		if (m_pPlayerRef->GetAspectStateId() == ASPECTSTATE_SUPER_SAIYAN)
			m_pAttribute.fSkillAnimationSpeedModifier += 20.f; // make skill animation faster
		

		m_pAttribute.wAttackRate = (WORD)(m_pAttribute.wAttackRate * pTransformationTbldat->fAttack_Rate_Transform);
		m_pAttribute.wDodgeRate = (WORD)(m_pAttribute.wDodgeRate * pTransformationTbldat->fDodge_Rate_Transform);
		m_pAttribute.wBlockRate = (WORD)(m_pAttribute.wBlockRate * pTransformationTbldat->fBlock_Rate_Transform);
		m_pAttribute.wCurseSuccessRate = (WORD)(m_pAttribute.wCurseSuccessRate * pTransformationTbldat->fCurse_Success_Transform);
		m_pAttribute.wCurseToleranceRate = (WORD)(m_pAttribute.wCurseToleranceRate * pTransformationTbldat->fCurse_Tolerance_Transform);
		m_pAttribute.fAttackRange = pTransformationTbldat->fAttack_Range_Change;

		if (m_pPlayerRef->GetTransformGrade() > 1)
		{
			m_pAttribute.wPhysicalCriticalRate += WORD((float)m_pAttribute.Dex * (float)m_pPlayerRef->GetTransformGrade() / 100.0f);
			m_pAttribute.wEnergyCriticalRate += WORD((float)m_pAttribute.Foc * (float)m_pPlayerRef->GetTransformGrade() / 100.0f);
		}

	}

	//add percent values
	CalculatePercentValues();

	// -- prop stats
	// offence
	if (GetBattleAttributeOffence() != BATTLE_ATTRIBUTE_NONE)
	{
		Dbo_SetAvatarAttributeValue(this, GetBattleAttributeEffectCode(GetBattleAttributeOffence()), 5.0f, GetBattleAttributeEffectApplyType(GetBattleAttributeOffence()));
	}

	//vehicle -- MUST BE THE LAST
	if (m_pPlayerRef->GetVehicleTblidx() != INVALID_TBLIDX)
	{
		sVEHICLE_TBLDAT* pVehicleTbl = (sVEHICLE_TBLDAT*)g_pTableContainer->GetVehicleTable()->FindData(m_pPlayerRef->GetVehicleTblidx());
		if (pVehicleTbl)
		{
			m_pAttribute.fRunSpeed = (float)pVehicleTbl->bySpeed;

			if (m_pPlayerRef->GetVehicleFuelId() != INVALID_HOBJECT)
			{
				CItem* pFuel = m_pPlayerRef->GetPlayerItemContainer()->GetItem(m_pPlayerRef->GetVehicleFuelId());
				if (pFuel && pFuel->IsExpired() == false)
				{
					sITEM_TBLDAT* fuelTbldat = pFuel->GetTbldat();
					if (fuelTbldat)
					{
						if (fuelTbldat->Item_Option_Tblidx != INVALID_TBLIDX)
						{
							sITEM_OPTION_TBLDAT* optionTbldat = (sITEM_OPTION_TBLDAT*)g_pTableContainer->GetItemOptionTable()->FindData(fuelTbldat->Item_Option_Tblidx);
							if (optionTbldat)
							{
								m_pAttribute.fRunSpeed += m_pAttribute.fRunSpeed * (float)optionTbldat->nValue[0] / 100.f;
							}
						}
					}
				}
			}
		}
	}

	CGameServer* app = (CGameServer*)g_pApp;

	if (m_pPlayerRef->GetDragonballScramble() && app->GetGsChannel() == 0)
		m_pAttribute.MaxLp += 5000;

	CalcSecondWeaponOffence();

	//m_pAttribute.fCoolTimeChangePercent = -100.f; // TEMP

	CCharacterAtt::CalculateAtt();
}


float CCharacterAttPC::GetWalkSpeed()
{
	return GetRunSpeed();	//player walk and run speed is the same..
}


void CCharacterAttPC::CalculateStr(float fValue, BYTE byApplyType, bool bIsPlus)
{
	WORD wCurStr = m_pAttribute.Str;

	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.Str = UnsignedSafeIncrease<WORD>(m_pAttribute.Str, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fSTR += fValue;
		}

		m_pAttribute.wPhysicalOffence = UnsignedSafeIncrease<WORD>(m_pAttribute.wPhysicalOffence, WORD((m_pAttribute.Str - wCurStr)  * CFormulaTable::m_afRate[100 + m_pPlayerRef->GetClass() + 1][2]));
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.Str = UnsignedSafeDecrease<WORD>(m_pAttribute.Str, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fSTRNegative -= fValue;
		}

		m_pAttribute.wPhysicalOffence = UnsignedSafeDecrease<WORD>(m_pAttribute.wPhysicalOffence, WORD((wCurStr - m_pAttribute.Str)  * CFormulaTable::m_afRate[100 + m_pPlayerRef->GetClass() + 1][2]));
	}
}

void CCharacterAttPC::CalculateCon(float fValue, BYTE byApplyType, bool bIsPlus)
{
	WORD wCurCon = m_pAttribute.Con;
	WORD wCurLpRegen = m_pAttribute.wLpRegen;

	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.Con = UnsignedSafeIncrease<WORD>(m_pAttribute.Con, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fCON += fValue;
		}

		m_pAttribute.MaxLp += int(float(m_pAttribute.Con - wCurCon) * CFormulaTable::m_afRate[200 + m_pPlayerRef->GetClass() + 1][2]);
		m_pAttribute.wLpRegen += WORD(float(m_pAttribute.Con - wCurCon) * CFormulaTable::m_afRate[300][2]);
		m_pAttribute.wLpSitdownRegen += WORD(float(m_pAttribute.wLpRegen - wCurLpRegen) * CFormulaTable::m_afRate[400][1]);

		m_pAttribute.wBlockRate += WORD(float(m_pAttribute.Con - wCurCon) * CFormulaTable::m_afRate[900][2]);
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.Con = UnsignedSafeDecrease<WORD>(m_pAttribute.Con, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fCONNegative -= fValue;
		}

		m_pAttribute.MaxLp = UnsignedSafeDecrease<WORD>(m_pAttribute.MaxLp, WORD(float(wCurCon - m_pAttribute.Con) * CFormulaTable::m_afRate[200 + m_pPlayerRef->GetClass() + 1][2]));
		m_pAttribute.wLpRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLpRegen, WORD(float(wCurCon - m_pAttribute.Con) * CFormulaTable::m_afRate[300][2]));
		m_pAttribute.wLpSitdownRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLpSitdownRegen, WORD(float(wCurLpRegen - m_pAttribute.wLpRegen) * CFormulaTable::m_afRate[400][1]));

		m_pAttribute.wBlockRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wBlockRate, WORD(float(wCurCon - m_pAttribute.Con) * CFormulaTable::m_afRate[900][2]));
	}
}

void CCharacterAttPC::CalculateFoc(float fValue, BYTE byApplyType, bool bIsPlus)
{
	WORD wCurFoc = m_pAttribute.Foc;

	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.Foc = UnsignedSafeIncrease<WORD>(m_pAttribute.Foc, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFOC += fValue;
		}

		m_pAttribute.wAttackRate += WORD(float(m_pAttribute.Foc - wCurFoc) / CFormulaTable::m_afRate[600][1] * CFormulaTable::m_afRate[600][2]);
		m_pAttribute.wCurseSuccessRate += WORD(float(m_pAttribute.Foc - wCurFoc) / CFormulaTable::m_afRate[700][1] * CFormulaTable::m_afRate[700][2]);
		m_pAttribute.wEnergyCriticalRate += WORD(float(m_pAttribute.Foc - wCurFoc) / CFormulaTable::m_afRate[2100 + m_pPlayerRef->GetClass() + 1][2]);
		m_pAttribute.fEnergyCriticalDamageRate += float(m_pAttribute.Foc - wCurFoc) / CFormulaTable::m_afRate[2200 + m_pPlayerRef->GetClass() + 1][2];
		m_pAttribute.wEnergyOffence += WORD(float(m_pAttribute.Foc - wCurFoc) * CFormulaTable::m_afRate[1100 + m_pPlayerRef->GetClass() + 1][3]);
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.Foc = UnsignedSafeDecrease<WORD>(m_pAttribute.Foc, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFOCNegative -= fValue;
		}

		m_pAttribute.wAttackRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wAttackRate, WORD(float(wCurFoc - m_pAttribute.Foc) / CFormulaTable::m_afRate[600][1] * CFormulaTable::m_afRate[600][2]));
		m_pAttribute.wCurseSuccessRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wCurseSuccessRate, WORD(float(wCurFoc - m_pAttribute.Foc) / CFormulaTable::m_afRate[700][1] * CFormulaTable::m_afRate[700][2]));
		m_pAttribute.wEnergyCriticalRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wEnergyCriticalRate, WORD(float(wCurFoc - m_pAttribute.Foc) / CFormulaTable::m_afRate[2100 + m_pPlayerRef->GetClass() + 1][2]));
		m_pAttribute.fEnergyCriticalDamageRate = UnsignedSafeDecrease<float>(m_pAttribute.fEnergyCriticalDamageRate, float(wCurFoc - m_pAttribute.Foc) / CFormulaTable::m_afRate[2200 + m_pPlayerRef->GetClass() + 1][2]);
		m_pAttribute.wEnergyOffence = UnsignedSafeDecrease<WORD>(m_pAttribute.wEnergyOffence, WORD(float(wCurFoc - m_pAttribute.Foc) * CFormulaTable::m_afRate[1100 + m_pPlayerRef->GetClass() + 1][3]));
	}
}

void CCharacterAttPC::CalculateDex(float fValue, BYTE byApplyType, bool bIsPlus)
{
	WORD wCurDex = m_pAttribute.Dex;

	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.Dex = UnsignedSafeIncrease<WORD>(m_pAttribute.Dex, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fDEX += fValue;
		}

		m_pAttribute.wDodgeRate += WORD(float(m_pAttribute.Dex - wCurDex) / CFormulaTable::m_afRate[800][1] * CFormulaTable::m_afRate[800][2]);
		m_pAttribute.wCurseToleranceRate += WORD(float(m_pAttribute.Dex - wCurDex) / CFormulaTable::m_afRate[1200][1] * CFormulaTable::m_afRate[1200][2]);
		m_pAttribute.wPhysicalCriticalRate += WORD(float(m_pAttribute.Dex - wCurDex) / CFormulaTable::m_afRate[1900 + m_pPlayerRef->GetClass() + 1][2]);
		m_pAttribute.fPhysicalCriticalDamageRate += float(m_pAttribute.Dex - wCurDex) / CFormulaTable::m_afRate[2000 + m_pPlayerRef->GetClass() + 1][2];
		m_pAttribute.wPhysicalOffence += WORD(float(m_pAttribute.Dex - wCurDex)  * CFormulaTable::m_afRate[100 + m_pPlayerRef->GetClass() + 1][3]);
		m_pAttribute.wBlockRate += WORD(float(m_pAttribute.Dex - wCurDex)  * CFormulaTable::m_afRate[900][2]);
		m_pAttribute.wGuardRate += WORD(float(m_pAttribute.Dex - wCurDex)  * CFormulaTable::m_afRate[1000][2]);
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.Dex = UnsignedSafeDecrease<WORD>(m_pAttribute.Dex, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fDEXNegative -= fValue;
		}

		m_pAttribute.wDodgeRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wDodgeRate, WORD(float(wCurDex - m_pAttribute.Dex) / CFormulaTable::m_afRate[800][1] * CFormulaTable::m_afRate[800][2]));
		m_pAttribute.wCurseToleranceRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wCurseToleranceRate, WORD(float(wCurDex - m_pAttribute.Dex) / CFormulaTable::m_afRate[1200][1] * CFormulaTable::m_afRate[1200][2]));
		m_pAttribute.wPhysicalCriticalRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wPhysicalCriticalRate, WORD(float(wCurDex - m_pAttribute.Dex) / CFormulaTable::m_afRate[1900 + m_pPlayerRef->GetClass() + 1][2]));
		m_pAttribute.fPhysicalCriticalDamageRate = UnsignedSafeDecrease<float>(m_pAttribute.fPhysicalCriticalDamageRate, float(wCurDex - m_pAttribute.Dex) / CFormulaTable::m_afRate[2000 + m_pPlayerRef->GetClass() + 1][2]);
		m_pAttribute.wPhysicalOffence = UnsignedSafeDecrease<WORD>(m_pAttribute.wPhysicalOffence, WORD(float(wCurDex - m_pAttribute.Dex) / CFormulaTable::m_afRate[100 + m_pPlayerRef->GetClass() + 1][3]));
		m_pAttribute.wBlockRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wBlockRate, WORD(float(wCurDex - m_pAttribute.Dex) * CFormulaTable::m_afRate[900][2]));
		m_pAttribute.wGuardRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wGuardRate, WORD(float(wCurDex - m_pAttribute.Dex) * CFormulaTable::m_afRate[1000][2]));
	}
}

void CCharacterAttPC::CalculateSol(float fValue, BYTE byApplyType, bool bIsPlus)
{
	WORD wCurSol = m_pAttribute.Sol;

	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.Sol = UnsignedSafeIncrease<WORD>(m_pAttribute.Sol, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fSOL += fValue;
		}

		m_pAttribute.wEnergyOffence += WORD(float(m_pAttribute.Sol - wCurSol) * CFormulaTable::m_afRate[1100 + m_pPlayerRef->GetClass() + 1][2]);
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.Sol = UnsignedSafeDecrease<WORD>(m_pAttribute.Sol, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fSOLNegative -= fValue;
		}

		m_pAttribute.wEnergyOffence = UnsignedSafeDecrease<WORD>(m_pAttribute.wEnergyOffence, WORD(float(wCurSol - m_pAttribute.Sol) * CFormulaTable::m_afRate[1100 + m_pPlayerRef->GetClass() + 1][2]));
	}
}

void CCharacterAttPC::CalculateEng(float fValue, BYTE byApplyType, bool bIsPlus)
{
	WORD wCurEng = m_pAttribute.Eng;
	WORD wCurEpRegen = m_pAttribute.wEpRegen;

	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.Eng = UnsignedSafeIncrease<WORD>(m_pAttribute.Eng, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fENG += fValue;
		}

		m_pAttribute.wMaxEP += (WORD)((float)(m_pAttribute.Eng - wCurEng) * CFormulaTable::m_afRate[1300 + m_pPlayerRef->GetClass() + 1][2]);
		m_pAttribute.wEpRegen += (WORD)((float)(m_pAttribute.Eng - wCurEng) * CFormulaTable::m_afRate[1400][2]);
		m_pAttribute.wEpSitdownRegen += (WORD)(float(m_pAttribute.wEpRegen - wCurEpRegen) * CFormulaTable::m_afRate[1500][1]);
		m_pAttribute.wEpBattleRegen += (WORD)((float)(m_pAttribute.wEpRegen - wCurEpRegen) * CFormulaTable::m_afRate[1600][1]);
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.Eng = UnsignedSafeDecrease<WORD>(m_pAttribute.Eng, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fENGNegative -= fValue;
		}

		m_pAttribute.wMaxEP = UnsignedSafeDecrease<WORD>(m_pAttribute.wMaxEP, WORD(float(wCurEng - m_pAttribute.Eng) * CFormulaTable::m_afRate[1300 + m_pPlayerRef->GetClass() + 1][2]));
		m_pAttribute.wEpRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wEpRegen, WORD(float(wCurEng - m_pAttribute.Eng) * CFormulaTable::m_afRate[1400][2]));
		m_pAttribute.wEpSitdownRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wEpSitdownRegen, WORD(float(wCurEpRegen - m_pAttribute.wEpRegen) * CFormulaTable::m_afRate[1500][1]));
		m_pAttribute.wEpBattleRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wEpBattleRegen, WORD(float(wCurEpRegen - m_pAttribute.wEpRegen) * CFormulaTable::m_afRate[1600][1]));
	}
}

void CCharacterAttPC::CalculateBattleAttribute(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CCharacterAtt::CalculateBattleAttribute(fValue, byApplyType, bIsPlus);
}


void CCharacterAttPC::CalcSecondWeaponOffence()
{
	m_wSubWeaponPhysicalOffence = m_pAttribute.wPhysicalOffence;
	m_wSubWeaponEnergyOffence = m_pAttribute.wEnergyOffence;

	//remove main weapon offence
	CItem* pMainWeapon = m_pPlayerRef->GetPlayerItemContainer()->GetItem(CONTAINER_TYPE_EQUIP, EQUIP_SLOT_TYPE_HAND);
	if (pMainWeapon)
	{
		m_wSubWeaponPhysicalOffence = UnsignedSafeDecrease<WORD>(m_wSubWeaponPhysicalOffence, Dbo_GetFinalOffence(pMainWeapon->GetTbldat()->wPhysical_Offence, pMainWeapon->GetGrade()));
		m_wSubWeaponEnergyOffence = UnsignedSafeDecrease<WORD>(m_wSubWeaponEnergyOffence, Dbo_GetFinalOffence(pMainWeapon->GetTbldat()->wEnergy_Offence, pMainWeapon->GetGrade()));
	}

	//add sub weapon offence
	CItem* pWeapon = m_pPlayerRef->GetPlayerItemContainer()->GetItem(CONTAINER_TYPE_EQUIP, EQUIP_SLOT_TYPE_SUB_WEAPON);
	if (pWeapon)
	{
		m_wSubWeaponPhysicalOffence = UnsignedSafeIncrease<WORD>(m_wSubWeaponPhysicalOffence, Dbo_GetFinalOffence(pWeapon->GetTbldat()->wPhysical_Offence, pWeapon->GetGrade()));
		m_wSubWeaponEnergyOffence = UnsignedSafeIncrease<WORD>(m_wSubWeaponEnergyOffence, Dbo_GetFinalOffence(pWeapon->GetTbldat()->wEnergy_Offence, pWeapon->GetGrade()));
	}
}
