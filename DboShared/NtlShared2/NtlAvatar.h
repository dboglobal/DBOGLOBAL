#pragma once

enum eATTRIBUTE_TO_UPDATE
{
	ATTRIBUTE_TO_UPDATE_STR_LAST,
	ATTRIBUTE_TO_UPDATE_CON_LAST,
	ATTRIBUTE_TO_UPDATE_FOC_LAST,
	ATTRIBUTE_TO_UPDATE_DEX_LAST,
	ATTRIBUTE_TO_UPDATE_SOL_LAST,
	ATTRIBUTE_TO_UPDATE_ENG_LAST,

	ATTRIBUTE_TO_UPDATE_MAX_LP_LAST,
	ATTRIBUTE_TO_UPDATE_MAX_EP_LAST,
	ATTRIBUTE_TO_UPDATE_MAX_AP_LAST,//new
	ATTRIBUTE_TO_UPDATE_MAX_RP_LAST,

	ATTRIBUTE_TO_UPDATE_LP_REGENERATION_LAST,
	ATTRIBUTE_TO_UPDATE_LP_SITDOWN_REGENERATION_LAST,
	ATTRIBUTE_TO_UPDATE_LP_BATTLE_REGENERATION_LAST,

	ATTRIBUTE_TO_UPDATE_EP_REGENERATION_LAST,
	ATTRIBUTE_TO_UPDATE_EP_SITDOWN_REGENERATION_LAST,
	ATTRIBUTE_TO_UPDATE_EP_BATTLE_REGENERATION_LAST,

	ATTRIBUTE_TO_UPDATE_AP_REGENERATION_LAST,
	ATTRIBUTE_TO_UPDATE_AP_SITDOWN_REGENERATION_LAST,
	ATTRIBUTE_TO_UPDATE_AP_DEGEN,

	ATTRIBUTE_TO_UPDATE_RP_CHARGE_SPEED_LAST,
	ATTRIBUTE_TO_UPDATE_RP_DIMIMUTION,

	ATTRIBUTE_TO_UPDATE_PHYSICAL_OFFENCE_LAST,
	ATTRIBUTE_TO_UPDATE_PHYSICAL_DEFENCE_LAST,

	ATTRIBUTE_TO_UPDATE_ENERGY_OFFENCE_LAST,
	ATTRIBUTE_TO_UPDATE_ENERGY_DEFENCE_LAST,

	ATTRIBUTE_TO_UPDATE_ATTACK_RATE_LAST,
	ATTRIBUTE_TO_UPDATE_DODGE_RATE_LAST,

	ATTRIBUTE_TO_UPDATE_BLOCK_RATE_LAST,
	ATTRIBUTE_TO_UPDATE_BLOCK_DAMAGE_RATE_LAST,//new

	ATTRIBUTE_TO_UPDATE_CURSE_SUCCESS_LAST,
	ATTRIBUTE_TO_UPDATE_CURSE_TOLERANCE_LAST,

	ATTRIBUTE_TO_UPDATE_PHYSICAL_CRITICAL_LAST,
	ATTRIBUTE_TO_UPDATE_ENERGY_CRITICAL_LAST,

	ATTRIBUTE_TO_UPDATE_PHYSICAL_CRITICAL_DAMAGE_RATE_LAST,//new
	ATTRIBUTE_TO_UPDATE_ENERGY_CRITICAL_DAMAGE_RATE_LAST, //new

	ATTRIBUTE_TO_UPDATE_RUN_SPEED_LAST,//new
	ATTRIBUTE_TO_UPDATE_FLY_SPEED_LAST,//new
	ATTRIBUTE_TO_UPDATE_FLY_DASH_SPEED_LAST,//new
	ATTRIBUTE_TO_UPDATE_FLY_ACCEL_SPEED_LAST,//new

	ATTRIBUTE_TO_UPDATE_ATTACK_SPEED_RATE_LAST,
	ATTRIBUTE_TO_UPDATE_ATTACK_RANGE_LAST,

	ATTRIBUTE_TO_UPDATE_CASTING_TIME_CHANGE_PERCENT,
	ATTRIBUTE_TO_UPDATE_COOL_TIME_CHANGE_PERCENT,
	ATTRIBUTE_TO_UPDATE_KEEP_TIME_CHANGE_PERCENT,
	ATTRIBUTE_TO_UPDATE_DOT_TIME_CHANGE_ABSOLUTE,
	ATTRIBUTE_TO_UPDATE_REQUIRED_EP_CHANGE_PERCENT,

	ATTRIBUTE_TO_UPDATE_PHYSICAL_REFLECTION,//new
	ATTRIBUTE_TO_UPDATE_ENERGY_REFLECTION,

	ATTRIBUTE_TO_UPDATE_PARALYZE_TOLERANCE_RATE,
	ATTRIBUTE_TO_UPDATE_TERROR_TOLERANCE_RATE,
	ATTRIBUTE_TO_UPDATE_CONFUSE_TOLERANCE_RATE,
	ATTRIBUTE_TO_UPDATE_STONE_TOLERANCE_RATE,
	ATTRIBUTE_TO_UPDATE_CANDY_TOLERANCE_RATE,

	ATTRIBUTE_TO_UPDATE_PARALYZE_KEEP_TIME_DOWN,
	ATTRIBUTE_TO_UPDATE_TERROR_KEEP_TIME_DOWN,
	ATTRIBUTE_TO_UPDATE_CONFUSE_KEEP_TIME_DOWN,
	ATTRIBUTE_TO_UPDATE_STONE_KEEP_TIME_DOWN,
	ATTRIBUTE_TO_UPDATE_CANDY_KEEP_TIME_DOWN,
	ATTRIBUTE_TO_UPDATE_BLEEDING_KEEP_TIME_DOWN,
	ATTRIBUTE_TO_UPDATE_POISON_KEEP_TIME_DOWN,
	ATTRIBUTE_TO_UPDATE_STOMACHACHE_KEEP_TIME_DOWN,

	ATTRIBUTE_TO_UPDATE_CRITICAL_BLOCK_SUCCESS_RATE,

	ATTRIBUTE_TO_UPDATE_LP_RECOVERY_WHEN_HIT,//new
	ATTRIBUTE_TO_UPDATE_LP_RECOVERY_WHEN_HIT_IN_PERCENT,//new
	ATTRIBUTE_TO_UPDATE_EP_RECOVERY_WHEN_HIT,//new
	ATTRIBUTE_TO_UPDATE_EP_RECOVERY_WHEN_HIT_IN_PERCENT,//new

	ATTRIBUTE_TO_UPDATE_STOMACHACHE_DEFENCE_LAST,//new
	ATTRIBUTE_TO_UPDATE_POISON_DEFENCE_LAST,//new
	ATTRIBUTE_TO_UPDATE_BLEED_DEFENCE_LAST,//new
	ATTRIBUTE_TO_UPDATE_BURN_DEFENCE_LAST,//new
	ATTRIBUTE_TO_UPDATE_MIND_CURSE_IMMUNITY_LAST,//new
	ATTRIBUTE_TO_UPDATE_BODY_CURSE_IMMUNITY_LAST,//new
	ATTRIBUTE_TO_UPDATE_CHANGE_CURSE_IMMUNITY_LAST,//new
	ATTRIBUTE_TO_UPDATE_SKILL_ANIMATION_SPEED_MODIFIER_LAST,//new
	ATTRIBUTE_TO_UPDATE_WEIGHT_LIMIT_LAST,//new
	ATTRIBUTE_TO_UPDATE_SKILL_AGGRO_BONUS,//new
	ATTRIBUTE_TO_UPDATE_SKILL_AGGRO_BONUS_IN_PERCENT,//new

	ATTRIBUTE_TO_UPDATE_DIRECT_HEAL_POWER_BONUS,//new
	ATTRIBUTE_TO_UPDATE_DIRECT_HEAL_POWER_BONUS_IN_PERCENT,//new
	ATTRIBUTE_TO_UPDATE_HOT_POWER_BONUS,//new
	ATTRIBUTE_TO_UPDATE_HOT_POWER_BONUS_IN_PERCENT,//new
	ATTRIBUTE_TO_UPDATE_DOT_VALUE_CHANGE_PERCENT,//new
	ATTRIBUTE_TO_UPDATE_PHYSICAL_CRITICAL_DEFENCE_RATE,//new
	ATTRIBUTE_TO_UPDATE_ENERGY_CRITICAL_DEFENCE_RATE,//new

	ATTRIBUTE_TO_UPDATE_GUARD_RATE_LAST,//new

	ATTRIBUTE_TO_UPDATE_SKILL_DAMAGE_BLOCK_MODE_SUCCESS_RATE_LAST,
	ATTRIBUTE_TO_UPDATE_CURSE_BLOCK_MODE_SUCCESS_RATE_LAST,
	ATTRIBUTE_TO_UPDATE_KNOCKDOWN_BLOCK_MODE_SUCCESS_RATE_LAST,
	ATTRIBUTE_TO_UPDATE_HTB_BLOCK_MODE_SUCCESS_RATE_LAST,

	ATTRIBUTE_TO_UPDATE_ITEM_UPGRADE_RATE_UP,
	ATTRIBUTE_TO_UPDATE_EXP_BOOSTER,
	ATTRIBUTE_TO_UPDATE_QUEST_DROP_UP,

	ATTRIBUTE_TO_UPDATE_PHYSICAL_ARMOR_PEN_RATE,
	ATTRIBUTE_TO_UPDATE_ENERGY_ARMOR_PEN_RATE,

	ATTRIBUTE_TO_UPDATE_COUNT,
	ATTRIBUTE_TO_UPDATE_UNKNOWN	= 0xFF,

	ATTRIBUTE_TO_UPDATE_FIRST = ATTRIBUTE_TO_UPDATE_STR_LAST,
	ATTRIBUTE_TO_UPDATE_LAST = ATTRIBUTE_TO_UPDATE_COUNT - 1,
};

#pragma pack(push, 1)

struct sAVATAR_ATTRIBUTE
{
	WORD Str; //absolute
	WORD Con; //absolute
	WORD Foc; //absolute
	WORD Dex; //absolute
	WORD Sol; //absolute
	WORD Eng; //absolute

	int MaxLp; //absolute
	WORD wMaxEP; //absolute
	int MaxAp; //absolute
	WORD wMaxRP; //absolute

	WORD wLpRegen; //absolute
	WORD wLpSitdownRegen; //absolute
	WORD wLpBattleRegen; //absolute

	WORD wEpRegen; //absolute
	WORD wEpSitdownRegen; //absolute
	WORD wEpBattleRegen; //absolute

	WORD wApRegen; //absolute
	WORD wApSitdownRegen; //absolute
	WORD wApDegen; //absolute

	WORD wRpRegen; //absolute		RP rate increase. (/ Second)
	WORD wRpDimimutionRate;			//RP 감소 속도.(/second)

	WORD wPhysicalOffence; //absolute
	WORD wPhysicalDefence; //absolute

	WORD wEnergyOffence; //absolute
	WORD wEnergyDefence; //absolute

	WORD wAttackRate; //percent
	WORD wDodgeRate; //percent

	WORD wBlockRate; //percent
	WORD wBlockDamageRate; //percent

	WORD wCurseSuccessRate; //percent
	WORD wCurseToleranceRate; //percent

	WORD wPhysicalCriticalRate; //percent
	WORD wEnergyCriticalRate; //percent

	float fPhysicalCriticalDamageRate; //percent
	float fEnergyCriticalDamageRate; //percent

	float fRunSpeed; //absolute
	float fFlySpeed; //absolute
	float fFlyDashSpeed; //absolute
	float fFlyAccelSpeed; //absolute

	WORD wAttackSpeedRate; //absolute
	float fAttackRange; //absolute

	float fCastingTimeChangePercent; //percent
	float fCoolTimeChangePercent; //percent
	float fKeepTimeChangePercent; //percent
	float fDotTimeChangeAbsolute; //absolute
	float fRequiredEpChangePercent; //percent
	float fPhysicalReflection; //absolute
	float fEnergyReflection; //absolute

	WORD wParalyzeToleranceRate;	//percent
	WORD wTerrorToleranceRate;	//percent
	WORD wConfuseToleranceRate;	//percent
	WORD wStoneToleranceRate;	//percent
	WORD wCandyToleranceRate;	//percent
	float fParalyzeKeepTimeDown;	//percent
	float fTerrorKeepTimeDown;	//percent
	float fConfuseKeepTimeDown;	//percent
	float fStoneKeepTimeDown;	//percent
	float fCandyKeepTimeDown;	//percent
	float fBleedingKeepTimeDown;	//percent
	float fPoisonKeepTimeDown;	//percent
	float fStomachacheKeepTimeDown;	//percent
	float fCriticalBlockSuccessRate;	//percent
	DWORD dwLpRecoveryWhenHit; //absolute
	float fLpRecoveryWhenHitInPercent;	//percent
	DWORD dwEpRecoveryWhenHit; //absolute
	float fEpRecoveryWhenHitInPercent;	//percent
	WORD wStomachacheDefence;
	WORD wPoisonDefence;
	WORD wBleedDefence;
	WORD wBurnDefence;
	float fMindCurseImmunity;
	float fBodyCurseImmunity;
	float fChangeCurseImmunity;
	float fSkillAnimationSpeedModifier;
	DWORD dwWeightLimit;
	float fSkillAggroBonus;
	float fSkillAggroBonusInPercent;
	float fDirectHealPowerBonus;
	float fDirectHealPowerBonusInPercent;
	float fHotPowerBonus;
	float fHotPowerBonusInPercent;
	float fDotValueChangePercent;
	float fPhysicalCriticalDefenceRate; // decreases critical dmg bonus
	float fEnergyCriticalDefenceRate;	// decreases critical dmg bonus
	WORD wGuardRate;
	float fSkillDamageBlockModeSuccessRate;
	float fCurseBlockModeSuccessRate;
	float fKnockdownBlockModeSuccessRate;
	float fHtbBlockModeSuccessRate;
	float fItemUpgradeBonusRate;
	BYTE byExpBooster;
	BYTE byQuestDropRate;
	float fPhysicalArmorPenRate; // Decreases x% of target physical def. Note: stat should never be below 0
	float fEnergyArmorPenRate; //  Decreases x% of target energy def. Note: stat should never be below 0
};

struct sAVATAR_ATTRIBUTE_LINK
{
	WORD *pStr;
	WORD *pCon;
	WORD *pFoc;
	WORD *pDex;
	WORD *pSol;
	WORD *pEng;

	int *pMaxLp;
	WORD *pwMaxEP;
	int *pMaxAp;
	WORD *pwMaxRP;

	WORD *pwLpRegen;
	WORD *pwLpSitdownRegen;
	WORD *pwLpBattleRegen;

	WORD *pwEpRegen;
	WORD *pwEpSitdownRegen;
	WORD *pwEpBattleRegen;

	WORD *pwApRegen;
	WORD *pwApSitdownRegen;
	WORD *pwApDegen;

	WORD *pwRpRegen;
	WORD *pwRpDimimutionRate;

	WORD *pwPhysicalOffence;
	WORD *pwPhysicalDefence;

	WORD *pwEnergyOffence;
	WORD *pwEnergyDefence;

	WORD *pwAttackRate;
	WORD *pwDodgeRate;

	WORD *pwBlockRate;
	WORD *pwBlockDamageRate;

	WORD *pwCurseSuccessRate;
	WORD *pwCurseToleranceRate;

	WORD *pwPhysicalCriticalRate;
	WORD *pwEnergyCriticalRate;

	float *pfPhysicalCriticalDamageRate;
	float *pfEnergyCriticalDamageRate;

	float *pfRunSpeed;
	float *pfFlySpeed;
	float *pfFlyDashSpeed;
	float *pfFlyAccelSpeed;

	WORD *pwAttackSpeedRate;
	float *pfAttackRange;

	float *pfCastingTimeChangePercent;
	float *pfCoolTimeChangePercent;
	float *pfKeepTimeChangePercent;
	float *pfDotTimeChangeAbsolute;
	float *pfRequiredEpChangePercent;
	float *pfPhysicalReflection;
	float *pfEnergyReflection;

	WORD *pwParalyzeToleranceRate;
	WORD *pwTerrorToleranceRate;
	WORD *pwConfuseToleranceRate;
	WORD *pwStoneToleranceRate;
	WORD *pwCandyToleranceRate;
	float *pfParalyzeKeepTimeDown;
	float *pfTerrorKeepTimeDown;
	float *pfConfuseKeepTimeDown;
	float *pfStoneKeepTimeDown;
	float *pfCandyKeepTimeDown;
	float *pfBleedingKeepTimeDown;
	float *pfPoisonKeepTimeDown;
	float *pfStomachacheKeepTimeDown;
	float *pfCriticalBlockSuccessRate;
	DWORD *pdwLpRecoveryWhenHit;
	float *pfLpRecoveryWhenHitInPercent;
	DWORD *pdwEpRecoveryWhenHit;
	float *pfEpRecoveryWhenHitInPercent;
	WORD *pwStomachacheDefence;
	WORD *pwPoisonDefence;
	WORD *pwBleedDefence;
	WORD *pwBurnDefence;
	float *pfMindCurseImmunity;
	float *pfBodyCurseImmunity;
	float *pfChangeCurseImmunity;
	float *pfSkillAnimationSpeedModifier;
	DWORD *pdwWeightLimit;
	float *pfSkillAggroBonus;
	float *pfSkillAggroBonusInPercent;
	float *pfDirectHealPowerBonus;
	float *pfDirectHealPowerBonusInPercent;
	float *pfHotPowerBonus;
	float *pfHotPowerBonusInPercent;
	float *pfDotValueChangePercent;
	float *pfPhysicalCriticalDefenceRate;
	float *pfEnergyCriticalDefenceRate;
	WORD *pwGuardRate;
	float *pfSkillDamageBlockModeSuccessRate;
	float *pfCurseBlockModeSuccessRate;
	float *pfKnockdownBlockModeSuccessRate;
	float *pfHtbBlockModeSuccessRate;
	float *pfItemUpgradeBonusRate;
	BYTE *pbyExpBooster;
	BYTE *pbyQuestDropRate;
	float *pfPhysicalArmorPenRate;
	float *pfEnergyArmorPenRate;
};

#pragma pack(pop)

class CNtlBitFlagManager;

class CNtlAvatar
{
public:
	struct sATTRIBUTE_LOGIC
	{
		DWORD dwFieldOffset;
		DWORD (*pCopyAttributeFunction)(void* pvBuffer, void* pvValue);
	};

protected:
	CNtlAvatar(void);
public:
	virtual ~CNtlAvatar(void);

protected:
	void Init();

	virtual void InitializeAttributeLink();

public:
	static CNtlAvatar* GetInstance();

public:

	// Updates sAVATAR_ATTRIBUTE structure with using raw data in a packet.(This function will be used mainly on client-side.)
	// (Packet -> Meta data + sAVATAR_ATTRIBUTE)
	static bool UpdateAvatarAttribute(BYTE byAttributeTotalCount, void* pvRawData, sAVATAR_ATTRIBUTE* pAttributeData);

	// Generates raw data in a packet with using attribute data.(This function will be used mainly on server-side.)
	// (Meta data + Attribute data -> Packet)
	static bool SaveAvatarAttribute(CNtlBitFlagManager* pChangedFlag, sAVATAR_ATTRIBUTE_LINK* pAttributeDataLink, void* pvBuffer, DWORD* pwdDataSize);
	static bool SaveAvatarAttribute(CNtlBitFlagManager* pChangedFlag, sAVATAR_ATTRIBUTE* pAttributeData, void* pvBuffer, DWORD* pwdDataSize);

	// Copies attribute data into sAVATAR_ATTRIBUTE structure.(This function will be used mainly on server-side.)
	// (Attribute data -> sAVATAR_ATTRIBUTE)
	static bool FillAvatarAttribute(sAVATAR_ATTRIBUTE_LINK* pAttributeDataLink, sAVATAR_ATTRIBUTE* pAttributeData);


	//NEW SELFMADE! ABOVE NOT USED
	// convert from sAVATAR_ATTRIBUTE to sAVATAR_ATTRIBUTE_LINK
	static bool FillAvatarAttributeNew(sAVATAR_ATTRIBUTE* pAttributeData, sAVATAR_ATTRIBUTE_LINK* pAttributeDataLink);
	// Very bad way to convert sAVATAR_ATTRIBUTE to sAVATAR_ATTRIBUTE_LINK
	static sAVATAR_ATTRIBUTE_LINK ConvertAVATAR_ATTRIBUTE(sAVATAR_ATTRIBUTE* avt);

public:
	static sATTRIBUTE_LOGIC m_attributeLogic[ATTRIBUTE_TO_UPDATE_COUNT];

	// needed to convert from sAVATAR_ATTRIBUTE to sAVATAR_ATTRIBUTE_LINK
	static sATTRIBUTE_LOGIC m_attributeLogicNew[ATTRIBUTE_TO_UPDATE_COUNT];

	static DWORD CopyValueByType_BYTE(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByType_WORD(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByType_float(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByType_DWORD(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByType_int(void* pvValue, void* pvBuffer);

	static DWORD CopyValueByTypeNew_BYTE(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByTypeNew_WORD(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByTypeNew_float(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByTypeNew_DWORD(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByTypeNew_int(void* pvValue, void* pvBuffer);
};