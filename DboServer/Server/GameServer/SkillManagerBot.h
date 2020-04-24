#ifndef __BOT_SKILL_MANAGER__
#define __BOT_SKILL_MANAGER__

#include "SkillManager.h"
#include "SkillBot.h"


class CCharacterObject;
class CCharacter;
class CSkillCondition;
class CMonster;

class CSkillManagerBot : public CSkillManager
{

public:

	CSkillManagerBot();
	virtual ~CSkillManagerBot();

public:

	virtual bool							Create(CCharacterObject * pOwnerRef, DWORD dwMaxNumberOfSkill);

	virtual void							Destroy();

protected:

	virtual void							Init();

public:

	virtual bool							RemoveSkillAll();

public:

	bool									AddSkill(BYTE byCount, CNpc *pBot, CSkillBot *pSkill, TBLIDX Skill_Tblidx, BYTE byUse_Skill_Basis, WORD wUse_Skill_LP, WORD wUse_Skill_Time);

	CSkill*									GetSkill();

	CCharacter*								GetOwnerCharacter();

	CSkillCondition*						GetSkill(CSkillCondition **apSkillCondition, int nSkillConditionCount, DWORD dwTickTime);

	CSkillCondition*						GetSkill(DWORD dwTickTime);

	CSkillCondition*						FindSkillCondition(BYTE bySkillIdx);

	CSkillCondition*						FindSkillCondition(TBLIDX skillTblidx);

	CSkillCondition*						GetCurSkillCondition();

	void									SetCurSkillConditionIdx(BYTE byCurSkillConditionIdx);

	bool									IsSkillUseLock() { return m_bIsSkillUseLock; }

	void									SetSkillUse_Lock() { m_bIsSkillUseLock = true; }

	void									SetSkillUse_Unlock() { m_bIsSkillUseLock = false; }

	bool									IsSkill() { return m_bySkillAllCount > 0; }

	BYTE									FillPetSkillInfo(sSKILL_INFO_PET* pSkillInfo);

	void									SetCanUseSkill(TBLIDX skillIdx, bool bFlag);


public:

	virtual void							FinishCasting();

	virtual void							CancelCasting();


private:

	BYTE									m_bySkillAllCount;

	BYTE									m_bySkillCondition_LP;
	BYTE									m_bySkillCondition_Give;
	BYTE									m_bySkillCondition_Time;
	BYTE									m_bySkillCondition_RingRange;
	BYTE									m_bySkillCondition_OnlyLP;


	CSkillCondition *						m_apSkillCondition_All[NTL_MAX_NPC_HAVE_SKILL];
	CSkillCondition *						m_apSkillCondition_LP[NTL_MAX_NPC_HAVE_SKILL];
	CSkillCondition *						m_apSkillCondition_Give[NTL_MAX_NPC_HAVE_SKILL];
	CSkillCondition *						m_apSkillCondition_Time[NTL_MAX_NPC_HAVE_SKILL];
	CSkillCondition *						m_apSkillCondition_RingRange[NTL_MAX_NPC_HAVE_SKILL];
	CSkillCondition *						m_apSkillCondition_OnlyLP[NTL_MAX_NPC_HAVE_SKILL];

	BYTE									m_byCurSkillConditionIdx;

	bool									m_bIsSkillUseLock;

};


#endif