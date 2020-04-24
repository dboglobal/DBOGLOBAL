#ifndef __CHAR_BUFF__
#define __CHAR_BUFF__

#include "NtlSkill.h"

class CCharacter;
enum eGAMERULE_TYPE;

class CBuff
{
public:

	enum BUFF_REMOVAL_REASON
	{
		BUFF_REMOVAL_REASON_BY_ITSELF,
		BUFF_REMOVAL_REASON_REPLACED,
		BUFF_REMOVAL_REASON_ETC,

		BUFF_REMOVAL_REASON_COUNT,
		BUFF_REMOVAL_REASON_FIRST = BUFF_REMOVAL_REASON_BY_ITSELF,
		BUFF_REMOVAL_REASON_LAST = BUFF_REMOVAL_REASON_COUNT - 1,
	};

public:

	CBuff();
	virtual ~CBuff();

public:

	bool						Create(CCharacter *pOwnerRef, sBUFF_INFO* pBuffInfo, eSYSTEM_EFFECT_CODE* effectCode, HOBJECT hCaster, eBUFF_TYPE buffType, BYTE byBuffGroup, BYTE* abyEffectType);

	void						Destroy();

protected:

	void						Init();

public:

	virtual bool				OnRegistered(bool bIsLoadedBuff, bool bNeedToDisplayMessage);

	virtual bool				OnRemoved(BUFF_REMOVAL_REASON nReason);

	virtual bool				IsValid();

	virtual void				TickProcess(DWORD dwTickDiff);


public:

	bool						IsBlessBuff();

	bool						IsCurseBuff();

	bool						IsSubBuff();

	bool						IsRestrictionRule(eGAMERULE_TYPE eRuleType);

	void						OnEffectActive(bool bUpdateStats = true); //called when effect begins

	bool						OnEffect(DWORD dwTickDiff);

	void						OnEffectInActive(bool bRecalculateStats = false); //called when effect ends

	void						ActivateBuff(bool bFlag);

public:

	TBLIDX						GetSourceTblidx() { return m_sBuffInfo.sourceTblidx; }

	BYTE						GetSourceType() { return m_sBuffInfo.bySourceType; }

	BYTE						GetBuffIndex() { return m_sBuffInfo.buffIndex; }
	void						SetBuffIndex(BYTE byIndex) { m_sBuffInfo.buffIndex = byIndex; }

	eSYSTEM_EFFECT_CODE			GetSystemEffectCode(BYTE byEffectSlot);

	BYTE						GetSystemEffectCodeSlot(eSYSTEM_EFFECT_CODE effectCode);

	HOBJECT						GetCasterHandle() { return m_hCaster; }

	bool						IsActive() { return m_sBuffInfo.bActive; }
	void						SetActive(bool bFlag) { m_sBuffInfo.bActive = bFlag; }

	sBUFF_INFO*					GetBuffInfo() { return &m_sBuffInfo; }

	BYTE						GetBuffGroup() { return m_byBuffGroup; }

	BYTE						GetSystemEffectType(BYTE byEffectSlot);

	float						GetSystemEffectParameter(eSYSTEM_EFFECT_CODE effectCode);

	eBUFF_TYPE					GetBuffType() { return m_eBuffType; }

	bool						RemoveSubBuffOnDeath();

	DWORD						GetRemainTime(int i) { return m_dwRemainTime[i]; }

protected:

	sBUFF_INFO					m_sBuffInfo;

	eSYSTEM_EFFECT_CODE			m_aeEffectCode[NTL_MAX_EFFECT_IN_SKILL];

	CCharacter *				m_pOwnerRef;
	
	HOBJECT						m_hCaster;

	eBUFF_TYPE					m_eBuffType;

	BYTE						m_byBuffGroup;

	BYTE						m_abyEffectType[NTL_MAX_EFFECT_IN_SKILL];

	DWORD						m_dwRemainTime[NTL_MAX_EFFECT_IN_SKILL];
};



#endif