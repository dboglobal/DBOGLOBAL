#pragma once

#include "SkillTable.h"

class CCharacter;

class CSkill
{

public:

	CSkill();
	virtual ~CSkill();

public:

	virtual bool						Create(sSKILL_TBLDAT *pSkillDataRef, CCharacter *pOwnerRef, BYTE skillIndex);

	virtual void						Destroy();

	virtual void						TickProcess(DWORD dwTickDiff);

	virtual bool						UseSkill(BYTE byRpBonusType, HOBJECT hAppointTargetHandle, CNtlVector& rvFinalSubjectLoc, CNtlVector& rvFinalLoc, BYTE byApplyTargetCount, HOBJECT* ahApplyTarget, WORD & rwResultCode);

	bool								CheckCasting();
	virtual void						CastSkill(HOBJECT hAppointTargetHandle, BYTE byApplyTargetCount, HOBJECT* ahApplyTarget, bool bSetData = true); 

	virtual void						OnAffected();

	void								OnAffectingEnd();

	void								OnAffectedCanceled();

	virtual bool						IsForBlessBuff();

	virtual bool						IsForCurseBuff();

protected:

	void								Init();
	void								InitSkillUse();

public:

	TBLIDX								GetSkillId() { return m_pSkillDataRef->tblidx; }

	DWORD								GetCoolTimeRemaining() { return m_dwCoolTimeRemaining; }
	void								SetCoolTimeRemaining(DWORD dwTime) { m_dwCoolTimeRemaining = dwTime; }
	void								DecreaseCoolTimeRemaining(DWORD dwTickDiff);

	DWORD								GetAffectingTimeRemaining() { return m_dwAffectingTimeRemaining; }
	void								DecreaseAffectingTimeRemaining(DWORD dwTickDiff);
	void								SetAffectingTimeRemaining(DWORD dwTime) { m_dwAffectingTimeRemaining = dwTime; }

	bool								IsForBuff();

	sSKILL_TBLDAT*						GetOriginalTableData() { return m_pSkillDataRef; }

	void								ChangeOriginalTableData(sSKILL_TBLDAT *pSkillDataRef);

	CCharacter*							GetOwner() { return m_pOwnerRef; }

	BYTE								GetSkillIndex() { return m_skillIndex; }

	void								SetSkillIndex(BYTE skillIndex) { m_skillIndex = skillIndex; }

	eSYSTEM_EFFECT_CODE					GetEffectCode(BYTE byPos) { return m_aeEffectCode[byPos]; }

	BYTE								GetUseRpBonusType() { return m_byUseRpBonusType; }

protected:

	sSKILL_TBLDAT *						m_pSkillDataRef;

	CCharacter *						m_pOwnerRef;

	BYTE								m_skillIndex;

	DWORD								m_dwCoolTimeRemaining;;

	DWORD								m_dwAffectingTimeRemaining;

	eSYSTEM_EFFECT_CODE					m_aeEffectCode[NTL_MAX_EFFECT_IN_SKILL];


protected:

	//skill usage
	HOBJECT								m_hUseAppointedTarget;
	BYTE								m_byUseRpBonusType;
	BYTE								m_byApplyTargetCount;
	HOBJECT								m_ahApplyTarget[NTL_MAX_NUMBER_OF_SKILL_TARGET];
	CNtlVector							m_vFinalLoc;
	CNtlVector							m_vFinalSubjectLoc;


	struct ACTION_SKILL
	{
		ACTION_SKILL()
		{
			bCanTargetRecoverOnHit = true;
			bCanWakeUpTarget = false;
		}

		bool					bIsSkillHarmful;
		BYTE					bySkillResultCount;
		sSKILL_RESULT			aSkillResult[NTL_MAX_NUMBER_OF_SKILL_TARGET];

		bool					bCanTargetRecoverOnHit;
		bool					bCanWakeUpTarget;

		BYTE					bySkillResultBuffCount;
		sDBO_BUFF_PARAMETER		aBuffParameter[NTL_MAX_NUMBER_OF_SKILL_TARGET][NTL_MAX_EFFECT_IN_SKILL];
		DWORD					dwKeepTime[NTL_MAX_NUMBER_OF_SKILL_TARGET];

	}m_actionSkill;

};

