/*****************************************************************************
 *
 * File			: NtlSobAtt.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 30	
 * Abstract		: Simulation object attribute base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOB_ATTR_H__
#define __NTL_SOB_ATTR_H__

#include "NtlSLDef.h"
#include "ceventhandler.h"
#include "NtlSLServerDeclear.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// simualtion object factory macro
////////////////////////////////////////////////////////////////////////////////////////////////////

/*

class CNtlSobAttr;

class CNtlSLAttrFactory
{
public:

	static CNtlSLAttrFactory *m_pFactory[MAX_SLCLASS];

public:

	CNtlSLAttrFactory() {} 
	virtual CNtlSobAttr* CreateFactory(void) const { return NULL; } 
};
	


#define DECLEAR_SLSOB_ATTR_FACTORY(class_name, classid) \
protected: \
	class class_name##Factory : public CNtlSLAttrFactory \
	{ \
	public: \
		class_name##Factory() \
		{ \
			m_pFactory[classid] = this; \
		} \
		CNtlSobAttr* CreateFactory(void) const { return NTL_NEW class_name; } \
	};\
public:\
	static class_name##Factory m_##class_name##Factory; 



#define DEFINITION_SLSOB_ATTR_FACTORY(class_name)\
	class_name::class_name##Factory class_name::m_##class_name##Factory;

*/

class CNtlSobAttr : public RWS::CEventHandler
{
protected:

	RwUInt32 m_uiClassID;

	CNtlSobAttr() { m_uiClassID = INVALID_SLCLASS_ID; }

public:

	virtual ~CNtlSobAttr() {}
	virtual RwBool Create(void) { return TRUE; }
	virtual void Destroy(void) {}
	virtual void HandleEvents(RWS::CMsg &pMsg) {}

public:

	void SetClassID(const RwUInt32 uiClassID);
	RwUInt32 GetClassID(void) const;
};

inline void CNtlSobAttr::SetClassID(const RwUInt32 uiClassID)
{
	m_uiClassID = uiClassID;
}

inline RwUInt32 CNtlSobAttr::GetClassID(void) const
{
	return m_uiClassID;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

class CNtlSobLifeAttr : public CNtlSobAttr
{
private:

	RwUInt8			m_byLevel;
	RwInt32			m_iLp;
	RwInt32			m_iMaxLp;
	RwInt32			m_iEp;
	RwInt32			m_iMaxEp;
	RwInt32			m_iAp;
	RwInt32			m_iMaxAp;
	RwInt32			m_iMaxRp;
	RwReal			m_fRadius;
	
	std::string		m_strModelName;
	std::wstring	m_wstrName;
	COLORREF		m_NameColor;
	std::wstring	m_wstrNickName;
	COLORREF		m_NickNameColor;

	RwBool			m_bNameCreate;

protected:

	CNtlSobLifeAttr();

public:

	void			SetLevel(RwUInt8 byLevel)	{ m_byLevel = byLevel; }
	RwUInt8			GetLevel(void) const		{ return m_byLevel; }

	void			SetLp(RwInt32 iLp)			{ m_iLp = iLp; }
	RwInt32			GetLp(void) const			{ return m_iLp; }
	void			SetMaxLp(RwInt32 iMaxLp)	{ m_iMaxLp = iMaxLp; }
	RwInt32			GetMaxLp(void) const		{ return m_iMaxLp; }

	void			SetAp(RwInt32 iAp) { m_iAp = iAp; }
	RwInt32			GetAp(void) const { return m_iAp; }
	void			SetMaxAp(RwInt32 iMaxAp) { m_iMaxAp = iMaxAp; }
	RwInt32			GetMaxAp(void) const { return m_iMaxAp; }

	void			SetEp(RwInt32 iEp)			{ m_iEp = iEp; }
	RwInt32			GetEp(void) const			{ return m_iEp; }

	void			SetMaxEp(RwInt32 iMaxEp)	{ m_iMaxEp = iMaxEp; }
	RwInt32			GetMaxEp(void) const		{ return m_iMaxEp; }

	void			SetMaxRp(RwInt32 iMaxRp)	{ m_iMaxRp = iMaxRp; }
	RwInt32			GetMaxRp(void) const		{ return m_iMaxRp; }

	void			SetRadius(RwReal fRadius)	{ m_fRadius = fRadius; }
	RwReal			GetRadius(void) const		{ return m_fRadius; } 

	void			SetModelName(const RwChar *pModelName)	{ m_strModelName = pModelName; }
	const RwChar*	GetModelName(void) const				{ return m_strModelName.c_str(); }

	void			SetName(const WCHAR *pwName)			{ m_wstrName = pwName; }
	const WCHAR*	GetName(void) const						{ return m_wstrName.c_str(); }

	void			SetNameColor(COLORREF color)			{ m_NameColor = color; }
	COLORREF		GetNameColor(void) const				{ return m_NameColor; }

	void			SetNickName(const WCHAR *pwNickName)	{ m_wstrNickName = pwNickName; }
	const WCHAR*	GetNickName(void) const					{ return m_wstrNickName.c_str(); }

	void			SetNickNameColor(COLORREF color)		{ m_NickNameColor = color; }
	COLORREF		GetNickNameColor(void) const			{ return m_NickNameColor; }

	void			SetNameCreate(RwBool bNameCreate)		{ m_bNameCreate = bNameCreate; }
	RwBool			IsNameCreate(void) const				{ return m_bNameCreate; }
};


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

class CNtlSobMoveableAttr : public CNtlSobLifeAttr
{
private:

	RwReal			m_fDefaultWalkSpeed;
	RwReal			m_fDefaultRunSpeed;
	RwReal			m_fWalkSpeed;
	RwReal			m_fRunSpeed;

	RwReal			m_fFlySpeed;
	RwReal			m_fFlyDashSpeed;
	RwReal			m_fFlyAccelSpeed;

	RwReal			m_fSkillAnimationSpeedModifier;

	RwReal			m_fAttackAnimSpeed;	    
    RwReal          m_fRunAnimSpeed;
	RwReal			m_fFlyHeight;
	RwReal			m_fScale;

protected:

	CNtlSobMoveableAttr();

public:

	void	SetDefaultWalkSpeed(RwReal fSpeed)		{ m_fDefaultWalkSpeed = fSpeed; }
	RwReal	GetDefaultWalkSpeed(void) const			{ return m_fDefaultWalkSpeed; }

	void	SetDefaultRunSpeed(RwReal fSpeed)		{ m_fDefaultRunSpeed = fSpeed; }
	RwReal	GetDefaultRunSpeed(void) const			{ return m_fDefaultRunSpeed; }

	void	SetWalkSpeed(RwReal fSpeed)				{ m_fWalkSpeed = fSpeed; }
	RwReal	GetWalkSpeed(void) const				{ return m_fWalkSpeed; }

	void	SetRunSpeed(RwReal fSpeed)				{ m_fRunSpeed = fSpeed; }
	RwReal	GetRunSpeed(void) const					{ return m_fRunSpeed; }

	void	SetFlySpeed(RwReal fSpeed) { m_fFlySpeed = fSpeed; }
	RwReal	GetFlySpeed(void) const { return m_fFlySpeed; }

	void	SetFlyDashSpeed(RwReal fSpeed) { m_fFlyDashSpeed = fSpeed; }
	RwReal	GetFlyDashSpeed(void) const { return m_fFlyDashSpeed; }

	void	SetFlyAccelSpeed(RwReal fSpeed) { m_fFlyAccelSpeed = fSpeed; }
	RwReal	GetFlyAccelSpeed(void) const { return m_fFlyAccelSpeed; }

	void	SetSkillAnimationSpeedModifier(RwReal fSpeed) { m_fSkillAnimationSpeedModifier = fSpeed; }
	RwReal	GetSkillAnimationSpeedModifier(void) const { return m_fSkillAnimationSpeedModifier; }

	void	SetAttackAnimSpeed(RwReal fAnimSpeed)	{ m_fAttackAnimSpeed = fAnimSpeed; }
	RwReal	GetAttackAnimSpeed(void) const			{ return m_fAttackAnimSpeed; }

    virtual void    SetRunAnimSpeed(RwReal fRunAnimSpeed)   {m_fRunAnimSpeed = fRunAnimSpeed;}
    virtual RwReal  GetRunAnimSpeed()                       {return m_fRunAnimSpeed;}

	void	SetFlyHeight(RwReal fHeight)			{ m_fFlyHeight = fHeight; }
	RwReal	GetFlyHeight(void) const				{ return m_fFlyHeight; }

	void	SetScale(RwReal fScale)					{ m_fScale = fScale; }
	RwReal	GetScale(void) const					{ return m_fScale; }
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

class CNtlSobBattleAttr : public CNtlSobMoveableAttr
{
public:

	RwInt32		m_Str;		// 근력
	RwInt32		m_Con;		// 건강
	RwInt32		m_Foc;		// 종족과 클래스에 따른 생성시의 집중을 기입
	RwInt32		m_Dex;		// 민첩
	RwInt32		m_Sol;		// 종족과 클래스에 따른 생성시의 정신을 기입
	RwInt32		m_Eng;		// 종족과 클래스에 따른 생성시의 원기를 기입

	RwUInt16	m_wLpRegen;
	RwUInt16	m_wLpSitdownRegen;
	RwUInt16	m_wLpBattleRegen;

	RwUInt16	m_wEpRegen;
	RwUInt16	m_wEpSitdownRegen;
	RwUInt16	m_wEpBattleRegen;

	RwUInt16 m_wPhysicalOffence;
	RwUInt16 m_wPhysicalDefence;

	RwUInt16 m_wEnergyOffence;
	RwUInt16 m_wEnergyDefence;

	RwUInt16 m_wAttackRate;
	RwUInt16 m_wDodgeRate;
	RwUInt16 m_wBlockRate;
	RwUInt16 m_wBlockDamageRate;

	RwUInt16 m_wCurseSuccessRate;
	RwUInt16 m_wCurseToleranceRate;

	RwUInt16 m_wPhysicalCriticalRate;
	RwUInt16 m_wEnergyCriticalRate;

	float m_fPhysicalCriticalDamageRate;
	float m_fEnergyCriticalDamageRate;

	RwUInt16 m_wAttackSpeedRate;

	float m_fPhysicalReflection;
	float m_fEnergyReflection;

	WORD m_wParalyzeToleranceRate;
	WORD m_wTerrorToleranceRate;
	WORD m_wConfuseToleranceRate;
	WORD m_wStoneToleranceRate;
	WORD m_wCandyToleranceRate;
	float m_fParalyzeKeepTimeDown;
	float m_fTerrorKeepTimeDown;
	float m_fConfuseKeepTimeDown;
	float m_fStoneKeepTimeDown;
	float m_fCandyKeepTimeDown;
	float m_fBleedingKeepTimeDown;
	float m_fPoisonKeepTimeDown;
	float m_fStomachacheKeepTimeDown;
	float m_fCriticalBlockSuccessRate;
	DWORD m_dwLpRecoveryWhenHit;
	float m_fLpRecoveryWhenHitInPercent;
	DWORD m_dwEpRecoveryWhenHit;
	float m_fEpRecoveryWhenHitInPercent;

	WORD m_wStomachacheDefence;
	WORD m_wPoisonDefence;
	WORD m_wBleedDefence;
	WORD m_wBurnDefence;

	float m_fPhysicalCriticalDefenceRate; 
	float m_fEnergyCriticalDefenceRate;

	float m_fMindCurseImmunity;
	float m_fBodyCurseImmunity;
	float m_fChangeCurseImmunity;

	float m_fSkillAggroBonus;
	float m_fSkillAggroBonusInPercent;
	float m_fDirectHealPowerBonus;
	float m_fDirectHealPowerBonusInPercent;
	float m_fHotPowerBonus;
	float m_fHotPowerBonusInPercent;
	float m_fDotValueChangePercent;

	WORD m_wGuardRate;
	float m_fSkillDamageBlockModeSuccessRate;
	float m_fCurseBlockModeSuccessRate;
	float m_fKnockdownBlockModeSuccessRate;
	float m_fHtbBlockModeSuccessRate;

	float m_fPhysicalArmorPenRate;
	float m_fEnergyArmorPenRate;

private:

	RwReal			m_fAttackRange;
	RwReal			m_fCastingTimeModifier;	// 캐스팅타임 Modifier (% value)
	RwReal			m_fCoolingTimeModifier;	// 쿨타임 Modifier (% value)
	RwReal			m_fKeepingTimeModifier; // 킵타임 Modifier (% value)
	RwReal			m_fDOTValueModifier;	// DOT Modifier (% value)
	RwReal			m_fDOTTimeModifier;		// DOT Time Modifier (absloute value)
	RwReal			m_fRequiredEPModifier;	// RequireEP Modifier(% value)

protected:

	CNtlSobBattleAttr();

public:

	void	SetAttackRange(RwReal fAttackRange)				{ m_fAttackRange = fAttackRange; }
	RwReal	GetAttackRange(void) const						{ return m_fAttackRange; }

	void	SetCastingTimeModifier(RwReal fCastingTimeModifier)	{ m_fCastingTimeModifier = fCastingTimeModifier; }
	RwReal	GetCastingTimeModifier(void) const					{ return m_fCastingTimeModifier; }

	void	SetCoolingTimeModifier(RwReal fCoolingTimeModifier) { m_fCoolingTimeModifier = fCoolingTimeModifier; }
	RwReal	GetCoolingTimeModifier(void) const					{ return m_fCoolingTimeModifier; }

	void	SetKeepingTimeModifier(RwReal fKeepingTimeModifier) { m_fKeepingTimeModifier = fKeepingTimeModifier; }
	RwReal	GetKeepingTimeModifier(void) const					{ return m_fKeepingTimeModifier; }

	void	SetDOTValueModifier(RwReal fDOTValueModifier)	{ m_fDOTValueModifier = fDOTValueModifier; }
	RwReal	GetDOTValueModifier(void) const					{ return m_fDOTValueModifier; }

	void	SetDOTTimeModifier(RwReal fDOTTimeModifier)		{ m_fDOTTimeModifier = fDOTTimeModifier; }
	RwReal	GetDOTTimeModifier(void) const					{ return m_fDOTTimeModifier; }

	void	SetRequiredEPModifier(RwReal fRequireEPModifier)	{ m_fRequiredEPModifier = fRequireEPModifier; }
	RwReal	GetRequiredEPModifier(void) const				{ return m_fRequiredEPModifier; }
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

class CNtlSobSocialAttr : public CNtlSobBattleAttr
{
private:

	RwUInt8		m_byRace;
	RwUInt32	m_uiRaceFlag;
	RwUInt8		m_byClass;
	RwUInt8		m_byGender;
    RwBool      m_bIsAdult;
	// shape
	RwUInt8		m_byFace;
	RwUInt8		m_byHair;
	RwUInt8		m_byHairColor;
	RwUInt8		m_bySkinColor;

	RwUInt32	m_uiReputation;	// 명성 point


protected:

	CNtlSobSocialAttr();

public:

	void		SetRace(RwUInt8 byRace)					{ m_byRace = byRace; }
	RwUInt8		GetRace(void) const						{ return m_byRace; }

	void		SetRaceFlag(RwUInt32 uiRaceFlag)		{ m_uiRaceFlag = uiRaceFlag; }
	RwUInt32	GetRaceFlag(void) const					{ return m_uiRaceFlag; }

	void		SetClass(RwUInt8 byClass)				{ m_byClass = byClass; }
	RwUInt8		GetClass(void) const					{ return m_byClass; }

	void		SetGender(RwUInt8 byGender)				{ m_byGender = byGender; }
	RwUInt8		GetGender(void) const					{ return m_byGender; }

	void		SetFace(RwUInt8 byFace)					{ m_byFace = byFace; }
	RwUInt8		GetFace(void) const						{ return m_byFace; }

	void		SetHair(RwUInt8 byHair)					{ m_byHair = byHair; }
	RwUInt8		GetHair(void) const						{ return m_byHair; }

	void		SetHairColor(RwUInt8 byHairColor)		{ m_byHairColor = byHairColor; }
	RwUInt8		GetHairColor(void) const				{ return m_byHairColor; }

	void		SetSkinColor(RwUInt8 bySkinColor)		{ m_bySkinColor = bySkinColor; }
	RwUInt8		GetSkinColor(void) const				{ return m_bySkinColor; }

	void		SetReputation(RwUInt32 uiReputation)	{ m_uiReputation = uiReputation; }
	RwUInt32	GetReputation(void) const				{ return m_uiReputation; }

    void        SetAdult(RwBool bIsAdult)               { m_bIsAdult = bIsAdult;}
    RwBool      IsAdult()                               { return m_bIsAdult;}
};


#endif
