#include "stdafx.h"
#include "CharacterAttBot.h"
#include "Monster.h"
#include "FormulaTable.h"
#include "calcs.h"


CCharacterAttBot::CCharacterAttBot()
{
	m_pBotRef = NULL;
}

CCharacterAttBot::~CCharacterAttBot()
{
}


bool CCharacterAttBot::Create(CNpc* pBot)
{
	if (CCharacterAtt::Create(pBot))
	{
		m_pBotRef = pBot;
		return true;
	}

	return false;
}


void CCharacterAttBot::CalculateAtt()
{
	sBOT_TBLDAT* pBotTbldat = reinterpret_cast<sBOT_TBLDAT*>(m_pBotRef->GetTbldat());
	sCHAR_TBLDAT* charTbldat = reinterpret_cast<sCHAR_TBLDAT*>(m_pBotRef->GetTbldat());

	BYTE byGrade = pBotTbldat->byGrade;
	BYTE byLv = pBotTbldat->byLevel;
	BYTE byEfLv = m_pBotRef->GetEffectiveLevel();

	m_pAttribute.MaxLp = (int)charTbldat->dwBasic_LP;
	m_pAttribute.MaxLp += (byEfLv - byLv) * ((m_pAttribute.MaxLp / byLv) * (byGrade + 1));

	m_pAttribute.wMaxEP = charTbldat->wBasic_EP;
	m_pAttribute.wMaxEP += (byEfLv - byLv) * ((m_pAttribute.wMaxEP / byLv) * (byGrade + 1));

	m_pAttribute.wPhysicalDefence = charTbldat->wBasic_Physical_Defence;
	m_pAttribute.wPhysicalDefence += (byEfLv - byLv) * ((m_pAttribute.wPhysicalDefence / byLv) * (byGrade + 1));

	m_pAttribute.wEnergyDefence = charTbldat->wBasic_Energy_Defence;
	m_pAttribute.wEnergyDefence += (byEfLv - byLv) * ((m_pAttribute.wEnergyDefence / byLv) * (byGrade + 1));

	m_pAttribute.wPhysicalOffence = pBotTbldat->wBasic_Physical_Offence;
	m_pAttribute.wPhysicalOffence += (byEfLv - byLv) * ((m_pAttribute.wPhysicalOffence / byLv) * (byGrade + 1));

	m_pAttribute.wEnergyOffence = pBotTbldat->wBasic_Energy_Offence;
	m_pAttribute.wEnergyOffence += (byEfLv - byLv) * ((m_pAttribute.wEnergyOffence / byLv) * (byGrade + 1));

	m_pAttribute.Str = charTbldat->wBasicStr;
	m_pAttribute.Con = charTbldat->wBasicCon;
	m_pAttribute.Foc = charTbldat->wBasicFoc;
	m_pAttribute.Dex = charTbldat->wBasicDex;
	m_pAttribute.Sol = charTbldat->wBasicSol;
	m_pAttribute.Eng = charTbldat->wBasicEng;
	m_pAttribute.wAttackSpeedRate = charTbldat->wAttack_Speed_Rate;
	m_pAttribute.fAttackRange = charTbldat->fAttack_Range;

	//m_pAttribute.wBlockRate = charTbldat->wBlock_Rate;
	//m_pAttribute.wCurseSuccessRate = charTbldat->wCurse_Success_Rate;
	//m_pAttribute.wCurseToleranceRate = charTbldat->wCurse_Tolerance_Rate;

	m_pAttribute.wEpRegen = pBotTbldat->wEP_Regeneration;
	m_pAttribute.wEpSitdownRegen = m_pAttribute.wMaxEP * 20 / 100;
	m_pAttribute.wEpBattleRegen = pBotTbldat->wEP_Regeneration;
	m_byBattle_Attribute = pBotTbldat->byBattle_Attribute;

	m_pAttribute.fRunSpeed = pBotTbldat->fRun_Speed;
	m_pAttribute.fFlySpeed = pBotTbldat->fRun_Speed;
	m_pAttribute.fFlyDashSpeed = pBotTbldat->fRun_Speed;
	m_pAttribute.fFlyAccelSpeed = pBotTbldat->fRun_Speed;

	m_pAttribute.wStomachacheDefence = pBotTbldat->wStomachacheDefence;
	m_pAttribute.wPoisonDefence = pBotTbldat->wPoisonDefence;
	m_pAttribute.wBleedDefence = pBotTbldat->wBleedDefence;
	m_pAttribute.wBurnDefence = pBotTbldat->wBurnDefence;

	m_pAttribute.fSkillAnimationSpeedModifier = 100.0f;

	m_pAttribute.wAttackRate = WORD(m_pAttribute.Foc / CFormulaTable::m_afRate[600][1] * CFormulaTable::m_afRate[600][2]);
	m_pAttribute.wDodgeRate = WORD(m_pAttribute.Dex / CFormulaTable::m_afRate[800][1] * CFormulaTable::m_afRate[800][2]);

	if (m_pAttribute.wAttackRate == INVALID_WORD)
	{
		ERR_LOG(LOG_BOTAI, "0Attack Rate is invalid. Foc: %u, formula 1: %f, formula 2: %f", m_pAttribute.Foc, CFormulaTable::m_afRate[600][1], CFormulaTable::m_afRate[600][2]);
	}
	if (m_pAttribute.wDodgeRate == INVALID_WORD)
	{
		ERR_LOG(LOG_BOTAI, "0Dodge Rate is invalid. Dex: %u, formula 1: %f, formula 2: %f", m_pAttribute.Dex, CFormulaTable::m_afRate[800][1], CFormulaTable::m_afRate[800][2]);
	}

	m_pAttribute.wBlockRate = WORD(CFormulaTable::m_afRate[919][1] + m_pAttribute.Dex * CFormulaTable::m_afRate[919][2] + m_pAttribute.Con * CFormulaTable::m_afRate[919][2]);
	m_pAttribute.wGuardRate = WORD(CFormulaTable::m_afRate[1000][1] + m_pAttribute.Dex * CFormulaTable::m_afRate[1000][2]);

	m_pAttribute.wCurseSuccessRate = WORD(m_pAttribute.Foc / CFormulaTable::m_afRate[700][1] * CFormulaTable::m_afRate[700][2]);
	m_pAttribute.wCurseToleranceRate = WORD(m_pAttribute.Dex / CFormulaTable::m_afRate[1200][1] * CFormulaTable::m_afRate[1200][2]);

	m_pAttribute.wLpRegen = pBotTbldat->wLP_Regeneration;
	m_pAttribute.wLpSitdownRegen = WORD(m_pAttribute.MaxLp) * 20 / 100;
	m_pAttribute.wLpBattleRegen = m_pAttribute.wLpRegen;

	m_pOwnerRef->GetBuffManager()->CopyBuffAttributesTo(this);

	if (m_pAttribute.wAttackRate == INVALID_WORD)
	{
		ERR_LOG(LOG_BOTAI, "1Attack Rate is invalid. Foc: %u, formula 1: %f, formula 2: %f", m_pAttribute.Foc, CFormulaTable::m_afRate[600][1], CFormulaTable::m_afRate[600][2]);
	}
	if (m_pAttribute.wDodgeRate == INVALID_WORD)
	{
		ERR_LOG(LOG_BOTAI, "1Dodge Rate is invalid. Dex: %u, formula 1: %f, formula 2: %f", m_pAttribute.Dex, CFormulaTable::m_afRate[800][1], CFormulaTable::m_afRate[800][2]);
	}

	if (m_pAttribute.MaxLp == 0)
	{
		ERR_LOG(LOG_BOTAI, "1 MAXLP is 0. dwBasic_LP: %u, byEfLv: %u, byLv: %u", charTbldat->dwBasic_LP, byEfLv, byLv);
	}

	CalculatePercentValues();

	if (m_pAttribute.MaxLp == 0)
	{
		ERR_LOG(LOG_BOTAI, "2 MAXLP is 0. dwBasic_LP: %u, byEfLv: %u, byLv: %u", charTbldat->dwBasic_LP, byEfLv, byLv);
	}

	if (m_pAttribute.wAttackRate == INVALID_WORD)
	{
		ERR_LOG(LOG_BOTAI, "2Attack Rate is invalid. Foc: %u, formula 1: %f, formula 2: %f, Percentfoc: %f, PercentFoc Negative: %f", m_pAttribute.Foc, CFormulaTable::m_afRate[600][1], CFormulaTable::m_afRate[600][2], m_percentValue.m_fAttackRate, m_percentValue.m_fAttackRateNegative);
	}
	if (m_pAttribute.wDodgeRate == INVALID_WORD)
	{
		ERR_LOG(LOG_BOTAI, "2Dodge Rate is invalid. Dex: %u, formula 1: %f, formula 2: %f, Percentfoc: %f, PercentFoc Negative: %f", m_pAttribute.Dex, CFormulaTable::m_afRate[800][1], CFormulaTable::m_afRate[800][2], m_percentValue.m_fDodgeRate, m_percentValue.m_fDodgeRateNegative);
	}

	CCharacterAtt::CalculateAtt();
}

float CCharacterAttBot::GetWalkSpeed()
{
	return m_pBotRef->GetTbldat()->fWalk_Speed;
}
