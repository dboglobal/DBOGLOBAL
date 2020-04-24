#ifndef __INC_DBOG_CHARATTRIBUTE_BASE_H__
#define __INC_DBOG_CHARATTRIBUTE_BASE_H__


class CCharacter;
#include "NtlAvatar.h"

class CCharacterAtt
{

public:
	CCharacterAtt();
	virtual	~CCharacterAtt();

public:

	sAVATAR_ATTRIBUTE*		GetAvatarAttributePointer() { return &m_pAttribute; }
	sAVATAR_ATTRIBUTE&		GetAvatarAttribute() { return m_pAttribute; }

public:

	bool					Create(CCharacter* pChar);

	void					Init();

//CALCULATION

	virtual void	CalculateAll();
	virtual void	CalculateAtt();

	void			CalculatePercentValues();

public:


	virtual void	CalculateStr(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCon(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateFoc(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateDex(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateSol(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEng(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateMaxLP(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateMaxEP(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateMaxRP(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateMaxAP(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateLpRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateLpSitdownRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateLpBattleRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEpRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEpSitdownRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEpBattleRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateApRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateApSitdownRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateApDegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateRpRegen(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculatePhysicalOffence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyOffence(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculatePhysicalDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyDefence(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateAttackRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateDodgeRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateBlockRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateBlockDamageRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCurseSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCurseToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculatePhysicalCriticalRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyCriticalRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculatePhysicalCriticalDamageRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyCriticalDamageRate(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateRunSpeed(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateFlySpeed(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateFlyDashSpeed(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateFlyAccelSpeed(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateAttackSpeedRate(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateAttackRange(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateCastingTimeChangePercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCoolTimeChangePercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateKeepTimeChangePercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateDotTimeChangeAbsolute(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateRequiredEpChangePercent(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateBattleAttribute(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateHonestOffence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateHonestDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateStrangeOffence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateStrangeDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateWildOffence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateWildDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEleganceOffence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEleganceDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateFunnyOffence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateFunnyDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculatePhysicalReflection(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyReflection(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateParalyzeToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateTerrorToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateConfuseToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateStoneToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCandyToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateParalyzeKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateTerrorKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateConfuseKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateStoneKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCandyKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateBleedingKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculatePoisonKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateStomachacheKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCriticalBlockSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateLpRecoveryWhenHit(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateLpRecoveryWhenHitInPercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEpRecoveryWhenHit(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEpRecoveryWhenHitInPercent(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateStomachacheDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculatePoisonDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateBleedDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateBurnDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateMindCurseImmunity(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateBodyCurseImmunity(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateChangeCurseImmunity(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateSkillAnimationSpeedModifier(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateWeightLimit(float fValue, BYTE byApplyType, bool bIsPlus) {}
	virtual void	CalculateSkillAggroBonus(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateSkillAggroBonusInPercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateDirectHealPowerBonus(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateDirectHealPowerBonusInPercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateHotPowerBonus(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateHotPowerBonusInPercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateDotValueChangePercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculatePhysicalCriticalDefenceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyCriticalDefenceRate(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateGuardRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateSkillDamageBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCurseBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateKnockdownBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateHtbBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus);
	//virtual void	CalculateItemUpgradeBonusRate() {}
	//virtual void	CalculateItemUpgradeBreakBonusRate() {}
	virtual void	CalculateExpBooster(float fValue, BYTE byApplyType, bool bIsPlus);
	//virtual void	CalculateQuestDropRate() {}

	virtual void	CalculatePhysicalArmorPen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyArmorPen(float fValue, BYTE byApplyType, bool bIsPlus);

//GETTER

	int		GetStr() { return m_pAttribute.Str; }
	int		GetCon() { return m_pAttribute.Con; }
	int		GetFoc() { return m_pAttribute.Foc; }
	int		GetDex() { return m_pAttribute.Dex; }
	int		GetSol() { return m_pAttribute.Sol; }
	int		GetEng() { return m_pAttribute.Eng; }

	int		GetMaxLP() { return m_pAttribute.MaxLp; }
	WORD	GetMaxEP() { return m_pAttribute.wMaxEP; }
	int		GetMaxAP() { return m_pAttribute.MaxAp; }
	WORD	GetMaxRP() { return m_pAttribute.wMaxRP; }

	WORD	GetLPRegen() { return m_pAttribute.wLpRegen; }
	WORD	GetLPSitdownRegen() { return m_pAttribute.wLpSitdownRegen; }
	WORD	GetLPBattleRegen() { return m_pAttribute.wLpBattleRegen; }
	WORD	GetEPRegen() { return m_pAttribute.wEpRegen; }
	WORD	GetEPSitdownRegen() { return m_pAttribute.wEpSitdownRegen; }
	WORD	GetEPBattleRegen() { return m_pAttribute.wEpBattleRegen; }
	WORD	GetAPRegen() { return m_pAttribute.wApRegen; }
	WORD	GetAPSitdownRegen() { return m_pAttribute.wApSitdownRegen; }
	WORD	GetAPDegen() { return m_pAttribute.wApDegen; }
	WORD	GetRPRegen() { return m_pAttribute.wRpRegen; }
	WORD	GetRPDimimutionRate() { return m_pAttribute.wRpDimimutionRate; }
	WORD	GetPhysicalOffence() { return m_pAttribute.wPhysicalOffence; }
	WORD	GetPhysicalDefence() { return m_pAttribute.wPhysicalDefence; }
	void	SetPhysicalDefence(WORD wDef) { m_pAttribute.wPhysicalDefence = wDef; }
	WORD	GetEnergyOffence() { return m_pAttribute.wEnergyOffence; }
	WORD	GetEnergyDefence() { return m_pAttribute.wEnergyDefence; }
	WORD	GetAttackRate() { return m_pAttribute.wAttackRate; }
	WORD	GetDodgeRate() { return m_pAttribute.wDodgeRate; }
	WORD	GetBlockRate() { return m_pAttribute.wBlockRate; }
	WORD	GetBlockDamageRate() { return m_pAttribute.wBlockDamageRate; }
	WORD	GetCurseSuccessRate() { return m_pAttribute.wCurseSuccessRate; }
	WORD	GetCurseToleranceRate() { return m_pAttribute.wCurseToleranceRate; }
	WORD	GetPhysicalCriticalRate() { return m_pAttribute.wPhysicalCriticalRate; }
	WORD	GetEnergyCriticalRate() { return m_pAttribute.wEnergyCriticalRate; }
	float	GetPhysicalCriticalDamageRate() { return m_pAttribute.fPhysicalCriticalDamageRate; }
	float	GetEnergyCriticalDamageRate() { return m_pAttribute.fEnergyCriticalDamageRate; }

	virtual float	GetWalkSpeed() = 0;
	float	GetRunSpeed() { return m_pAttribute.fRunSpeed; }
	float	GetFlySpeed() { return m_pAttribute.fFlySpeed; }
	float	GetFlyDashSpeed() { return m_pAttribute.fFlyDashSpeed; }
	float	GetFlyAccelSpeed() { return m_pAttribute.fFlyAccelSpeed; }

	WORD	GetAttackSpeedRate() { return m_pAttribute.wAttackSpeedRate; }

	float	GetAttackRange() { return m_pAttribute.fAttackRange; }

	float	GetCastingTimeChangePercent() { return m_pAttribute.fCastingTimeChangePercent; }
	float	GetCoolTimeChangePercent() { return m_pAttribute.fCoolTimeChangePercent; }
	float	GetKeepTimeChangePercent() { return m_pAttribute.fKeepTimeChangePercent; }
	float	GetDotTimeChangeAbsolute() { return m_pAttribute.fDotTimeChangeAbsolute; }
	float	GetRequiredEpChangePercent() { return m_pAttribute.fRequiredEpChangePercent; }

	float	GetPhysicalReflection() { return m_pAttribute.fPhysicalReflection; }
	float	GetEnergyReflection() { return m_pAttribute.fEnergyReflection; }

	WORD	GetParalyzeToleranceRate() { return m_pAttribute.wParalyzeToleranceRate; }
	WORD	GetTerrorToleranceRate() { return m_pAttribute.wTerrorToleranceRate; }
	WORD	GetConfuseToleranceRate() { return m_pAttribute.wConfuseToleranceRate; }
	WORD	GetStoneToleranceRate() { return m_pAttribute.wStoneToleranceRate; }
	WORD	GetCandyToleranceRate() { return m_pAttribute.wCandyToleranceRate; }

	float	GetParalyzeKeepTimeDown() { return m_pAttribute.fParalyzeKeepTimeDown; }
	float	GetTerrorKeepTimeDown() { return m_pAttribute.fTerrorKeepTimeDown; }
	float	GetConfuseKeepTimeDown() { return m_pAttribute.fConfuseKeepTimeDown; }
	float	GetStoneKeepTimeDown() { return m_pAttribute.fStoneKeepTimeDown; }
	float	GetCandyKeepTimeDown() { return m_pAttribute.fCandyKeepTimeDown; }
	float	GetBleedingKeepTimeDown() { return m_pAttribute.fBleedingKeepTimeDown; }
	float	GetPoisonKeepTimeDown() { return m_pAttribute.fPoisonKeepTimeDown; }
	float	GetStomachacheKeepTimeDown() { return m_pAttribute.fStomachacheKeepTimeDown; }

	float	GetCriticalBlockSuccessRate() { return m_pAttribute.fCriticalBlockSuccessRate; }
	DWORD	GetLpRecoveryWhenHit() { return m_pAttribute.dwLpRecoveryWhenHit; }
	float	GetLpRecoveryWhenHitInPercent() { return m_pAttribute.fLpRecoveryWhenHitInPercent; }
	DWORD	GetEpRecoveryWhenHit() { return m_pAttribute.dwEpRecoveryWhenHit; }
	float	GetEpRecoveryWhenHitInPercent() { return m_pAttribute.fEpRecoveryWhenHitInPercent; }

	WORD	GetStomachacheDefence() { return m_pAttribute.wStomachacheDefence; }
	WORD	GetPoisonDefence() { return m_pAttribute.wPoisonDefence; }
	WORD	GetBleedDefence() { return m_pAttribute.wBleedDefence; }
	WORD	GetBurnDefence() { return m_pAttribute.wBurnDefence; }
	float	GetMindCurseImmunity() { return m_pAttribute.fMindCurseImmunity; }
	float	GetBodyCurseImmunity() { return m_pAttribute.fBodyCurseImmunity; }
	float	GetChangeCurseImmunity() { return m_pAttribute.fChangeCurseImmunity; }
	float	GetSkillAnimationSpeedModifier() { return m_pAttribute.fSkillAnimationSpeedModifier; }
	DWORD	GetWeightLimit() { return m_pAttribute.dwWeightLimit; }
	float	GetSkillAggroBonus() { return m_pAttribute.fSkillAggroBonus; }
	float	GetSkillAggroBonusInPercent() { return m_pAttribute.fSkillAggroBonusInPercent; }
	float	GetDirectHealPowerBonus() { return m_pAttribute.fDirectHealPowerBonus; }
	float	GetDirectHealPowerBonusInPercent() { return m_pAttribute.fDirectHealPowerBonusInPercent; }
	float	GetHotPowerBonus() { return m_pAttribute.fHotPowerBonus; }
	float	GetHotPowerBonusInPercent() { return m_pAttribute.fHotPowerBonusInPercent; }
	float	GetDotValueChangePercent() { return m_pAttribute.fDotValueChangePercent; }
	BYTE	GetBattleAttributeOffence() { return m_byBattle_Attribute; }
	float	GetPhysicalCriticalDefenceRate() { return m_pAttribute.fPhysicalCriticalDefenceRate; }
	float	GetEnergyCriticalDefenceRate() { return m_pAttribute.fEnergyCriticalDefenceRate; }

	float	GetSkillDamageBlockModeSuccessRate() { return m_pAttribute.fSkillDamageBlockModeSuccessRate; }
	float	GetCurseBlockModeSuccessRate() { return m_pAttribute.fCurseBlockModeSuccessRate; }
	float	GetKnockDownBlockModeSuccessRate() { return m_pAttribute.fKnockdownBlockModeSuccessRate; }
	float	GetHtbKnockDownBlockModeSuccessRate() { return m_pAttribute.fHtbBlockModeSuccessRate; }

	WORD	GetGuardRate() { return m_pAttribute.wGuardRate; }

	BYTE	GetExpBoost() { return m_pAttribute.byExpBooster; }

	float	GetPhysicalArmorPenRate() { return m_pAttribute.fPhysicalArmorPenRate; }
	float	GetEnergyArmorPenRate() { return m_pAttribute.fEnergyArmorPenRate; }

//SETTER
	void	SetMaxLP(int nMaxLP) { m_pAttribute.MaxLp = nMaxLP; }
	void	SetMaxEP(WORD wMaxEP) { m_pAttribute.wMaxEP = wMaxEP; }

	void	SetRunSpeed(float fSpeed) { m_pAttribute.fRunSpeed = fSpeed; }
	void	SetFlySpeed(float fSpeed) { m_pAttribute.fFlySpeed = fSpeed; }
	void	SetFlyDashSpeed(float fSpeed) { m_pAttribute.fFlyDashSpeed = fSpeed; }
	void	SetFlyAccelSpeed(float fSpeed) { m_pAttribute.fFlyAccelSpeed = fSpeed; }

	void	SetAttackSpeedRate(WORD wSpeed) { m_pAttribute.wAttackSpeedRate = wSpeed; }

	void	SetAttackRange(float fRange) { m_pAttribute.fAttackRange = fRange; }

	void	SetSkillAnimationSpeedModifier(float fSpeed) { m_pAttribute.fSkillAnimationSpeedModifier = fSpeed; }

	void	SetBattleAttributeOffence(BYTE byAttribute) { m_byBattle_Attribute = byAttribute; }

	void	SetPhysicalOffence(WORD wAttack) { m_pAttribute.wPhysicalOffence = wAttack; }
	void	SetEnergyOffence(WORD wAttack) { m_pAttribute.wEnergyOffence = wAttack; }

	void	SetSubOffence(WORD wOffence) { m_wSubWeaponPhysicalOffence = wOffence; m_wSubWeaponEnergyOffence = wOffence; }
	WORD	GetSubWeaponPhysicalOffence() { return m_wSubWeaponPhysicalOffence; }
	WORD	GetSubWeaponEnergyOffence() { return m_wSubWeaponEnergyOffence; }

protected:

	CCharacter*				m_pOwnerRef;
	sAVATAR_ATTRIBUTE		m_pAttribute;
	BYTE					m_byBattle_Attribute;		//eBATTLE_ATTRIBUTE //[0] Offence (main)

	float					m_fRunSpeedBackup;
	WORD					m_wAttackSpeedBackup;

	WORD					m_wSubWeaponPhysicalOffence;
	WORD					m_wSubWeaponEnergyOffence;

	float					m_fCurLpPercent;
	float					m_fCurEpPercent;
	float					m_fCurApPercent;

	// percent values
	struct sPERCENT_VALUES
	{
		float					m_fSTR;
		float					m_fCON;
		float					m_fFOC;
		float					m_fDEX;
		float					m_fSOL;
		float					m_fENG;
		float					m_fLP;
		float					m_fEP;
		float					m_fAP;
		float					m_fRP;
		float					m_fLpRegen;
		float					m_fLpSitdownRegen;
		float					m_fLpBattleRegen;
		float					m_fEpRegen;
		float					m_fEpSitdownRegen;
		float					m_fEpBattleRegen;
		float					m_fApRegen;
		float					m_fApSitdownRegen;
		float					m_fApDegen;
		float					m_fRpRegen;
		float					m_fRpDimimutionRate;
		float					m_fPhysicalOffence;
		float					m_fPhysicalDefence;
		float					m_fEnergyOffence;
		float					m_fEnergyDefence;
		float					m_fAttackRate;
		float					m_fDodgeRate;
		float					m_fBlockRate;
		float					m_fBlockDamageRate;
		float					m_fCurseSuccessRate;
		float					m_fCurseToleranceRate;
		float					m_fPhysicalCriticalRate;
		float					m_fEnergyCriticalRate;
		float					m_fPhysicalCriticalDamageRate;
		float					m_fEnergyCriticalDamageRate;
		float					m_fRunSpeed;
		float					m_fFlySpeed;
		float					m_fFlyDashSpeed;
		float					m_fFlyAccelSpeed;
		float					m_fAttackSpeedRate;
		float					m_fAttackRange;
		float					m_fStomachacheDef;
		float					m_fPoisonDef;
		float					m_fBleedDef;
		float					m_fBurnDef;
		float					m_fMindCurseImmunity;
		float					m_fBodyCurseImmunity;
		float					m_fChangeCurseImmunity;
		float					m_fSkillAnimationSpeedModifier;
		float					m_fGuardRate;

		float					m_fSTRNegative;
		float					m_fCONNegative;
		float					m_fFOCNegative;
		float					m_fDEXNegative;
		float					m_fSOLNegative;
		float					m_fENGNegative;
		float					m_fLPNegative;
		float					m_fEPNegative;
		float					m_fAPNegative;
		float					m_fRPNegative;
		float					m_fLpRegenNegative;
		float					m_fLpSitdownRegenNegative;
		float					m_fLpBattleRegenNegative;
		float					m_fEpRegenNegative;
		float					m_fEpSitdownRegenNegative;
		float					m_fEpBattleRegenNegative;
		float					m_fApRegenNegative;
		float					m_fApSitdownRegenNegative;
		float					m_fApBattleRegenNegative;
		float					m_fApDegenNegative;
		float					m_fApBattleDegenNegative;
		float					m_fRpRegenNegative;
		float					m_fRpDimimutionRateNegative;
		float					m_fPhysicalOffenceNegative;
		float					m_fPhysicalDefenceNegative;
		float					m_fEnergyOffenceNegative;
		float					m_fEnergyDefenceNegative;
		float					m_fAttackRateNegative;
		float					m_fDodgeRateNegative;
		float					m_fBlockRateNegative;
		float					m_fBlockDamageRateNegative;
		float					m_fCurseSuccessRateNegative;
		float					m_fCurseToleranceRateNegative;
		float					m_fPhysicalCriticalRateNegative;
		float					m_fEnergyCriticalRateNegative;
		float					m_fPhysicalCriticalDamageRateNegative;
		float					m_fEnergyCriticalDamageRateNegative;
		float					m_fRunSpeedNegative;
		float					m_fFlySpeedNegative;
		float					m_fFlyDashSpeedNegative;
		float					m_fFlyAccelSpeedNegative;
		float					m_fAttackSpeedRateNegative;
		float					m_fAttackRangeNegative;
		float					m_fStomachacheDefNegative;
		float					m_fPoisonDefNegative;
		float					m_fBleedDefNegative;
		float					m_fBurnDefNegative;
		float					m_fMindCurseImmunityNegative;
		float					m_fBodyCurseImmunityNegative;
		float					m_fChangeCurseImmunityNegative;
		float					m_fSkillAnimationSpeedModifierNegative;
		float					m_fGuardRateNegative;
	}
	m_percentValue;
};

#endif