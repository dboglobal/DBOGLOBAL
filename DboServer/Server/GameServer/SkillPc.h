#ifndef __PC_SKILL__
#define __PC_SKILL__

#include "Skill.h"

class CPlayer;
class CSkillCondition;

class CSkillPc : public CSkill
{

public:

	CSkillPc();
	virtual ~CSkillPc();

public:

	virtual bool						Create(sSKILL_TBLDAT *pSkillDataRef, CPlayer *pOwnerRef, BYTE skillIndex, DWORD dwTimeRemaining, int nExp, BYTE byRpBonusType, bool bIsRpBonusAuto);

	virtual void						Destroy();

protected:

	void								Init();

public:

	virtual void						TickProcess(DWORD dwTickDiff);

	virtual bool						UseSkill(BYTE byRpBonusType, HOBJECT hAppointTargetHandle, CNtlVector& rvFinalSubjectLoc, CNtlVector& rvFinalLoc, BYTE byApplyTargetCount, HOBJECT* ahApplyTarget, WORD & rwResultCode);

	virtual void						CastSkill(HOBJECT hAppointTargetHandle, BYTE byApplyTargetCount, HOBJECT* ahApplyTarget, bool bSetData = true);


public:

	void								OnLearnSkill();

	void								UpdateRpSetting(BYTE byRpBonusType, bool bIsRpBonusAuto, WORD & rwResultCode);

	bool								UpgradeSkill(WORD & rwResultCode, sSKILL_TBLDAT* pNewSkillTbldat);

	bool								CanResetSkill();

public:

	int									CopyToData(sSKILL_DATA * pSkillData);
	int									CopyToInfo(sSKILL_INFO * pSkillInfo);

	bool								IsRpBonusAuto() { return m_bIsRpBonusAuto; }
	virtual BYTE						GetRpBonusType() { return m_byRpBonusType; }

	BYTE								GetUsedRpBonus() { return m_byUseRpBonusType; }

private:

	int									m_nExp;

	BYTE								m_byRpBonusType;

	bool								m_bIsRpBonusAuto;

	CPlayer*							m_pPlayerRef;
};


#endif