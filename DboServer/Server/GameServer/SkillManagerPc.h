#ifndef __PC_SKILL_MANAGER__
#define __PC_SKILL_MANAGER__

#include "SkillManager.h"
#include "SkillPc.h"

class CPlayer;
class QueryBuffer;


class CSkillManagerPc : public CSkillManager
{

public:

	CSkillManagerPc();
	virtual ~CSkillManagerPc();

public:

	virtual bool							Create(CPlayer * pOwnerRef, DWORD dwMaxNumberOfSkill);

	virtual void							Destroy();


protected:

	virtual void							Init();

public:

	void									LearnSkill(TBLIDX skillTblidx, WORD & rwResultCode, bool bRequireSP = true);

	void									UpdateRpSetting(BYTE skillIndex, BYTE byRpBonusType, bool bIsRpBonusAuto);

	void									UpgradeSkill(BYTE skillIndex);

	void									StartSkillCoolDown(BYTE skillIndex);

	bool									HasSkillLearned(TBLIDX skillTblidx);
	bool									HasBasicSkillLearned(TBLIDX skillTblidx);

	bool									HasRequiredSkillLearned(sSKILL_TBLDAT* pNewSkillTbldat);

	bool									HasSkillsInCoolDown();

	void									InitSkills();

	bool									CanResetSkill(CSkillPc* pResetSkill);

public:

	int										CopyToInfo(sSKILL_INFO * pSkillInfo);
	int										CopyToData(sSKILL_DATA * pSkillData);

	void									CopyPassiveAttributesTo(CCharacterAtt* pCharAtt);

	void									_SaveSkillData();

	bool									IsCastingFinished() { return m_bCastingFinished; }
	void									SetCastingFinished(bool bFlag) { m_bCastingFinished = bFlag; }


public:

	virtual void							FinishCasting();

	virtual void							CancelCasting();

private:

	CPlayer*								m_pPlayerRef;

	bool									m_bCastingFinished;

};


#endif