#ifndef __CHAR_SKILL_MANAGER_BASE__
#define __CHAR_SKILL_MANAGER_BASE__

#include "Skill.h"
#include "NtlLinkArray.h"

class CCharacterObject;
struct sAVATAR_ATTRIBUTE;
class QueryBuffer;

class CSkillManager
{

public:

	CSkillManager();
	virtual ~CSkillManager();

public:

	virtual bool							Create(CCharacterObject * pOwnerRef, DWORD dwMaxNumberOfSkill);

	virtual void							Destroy();

	virtual void							TickProcess(DWORD dwTickDiff);

public:

	bool									AddSkill(CSkill * pSkill);

	bool									RemoveSkill(TBLIDX tblidx);

	virtual bool							RemoveSkillAll();

	CSkill*									FindSkill(TBLIDX tblidx);

	CSkill*									GetSkillWithSkillIndex(BYTE byIndex);

	bool									FindSkillWithSystemEffect(TBLIDX systemEffect, std::list<CSkill*> *rListSkill);

	CSkill*									FindSkillWithSystemEffectCode(eSYSTEM_EFFECT_CODE effectCode);

	DWORD									GetNumberOfSkill() { return (DWORD)m_skillList.GetSize(); }

	DWORD									GetMaxNumberOfSkill() { return m_dwMaxNumberOfSkill; }

	CCharacterObject*						GetOwner() { return m_pOwnerRef; }

	BYTE									AcquireSkillIndex(TBLIDX skillTblidxToGet);

	void									ReleaseSkillIndex(BYTE skillIndex);

public:

	TBLIDX									GetCurSkillTblidx() { return m_curSkillTblidx; }

	void									SetCurSkillTblidx(TBLIDX curSkillTblidx) { m_curSkillTblidx = curSkillTblidx; }

protected:

	virtual void							Init();

	bool									RemoveSkill_Internal(unsigned int iterList);

public:

	virtual void							FinishCasting() = 0;

	virtual void							CancelCasting();

protected:

	CNtlLinkArray_Static<CSkill *>			m_skillList;

	DWORD									m_dwMaxNumberOfSkill;

	CSkill **								m_ppSkill;

	TBLIDX *								m_paTblidxInTransaction;

	CCharacterObject *						m_pOwnerRef;

	//Skill Usage
	TBLIDX									m_curSkillTblidx;

};


#endif