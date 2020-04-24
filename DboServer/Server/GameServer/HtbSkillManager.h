#ifndef __PC_HTB_SKILL_MANAGER__
#define __PC_HTB_SKILL_MANAGER__

#include "HtbSkill.h"
#include "NtlLinkArray.h"

class CPlayer;
class QueryBuffer;


class CHtbSkillManager
{

public:

	CHtbSkillManager();
	virtual ~CHtbSkillManager();

public:

	bool								Create(CPlayer * pOwnerRef);

	void								Destroy();

	void								Init();

public:

	void								TickProcess(DWORD dwTickDiff);

public:

	void								LearnHtbSkill(TBLIDX skillTblidx, WORD & rwResultCode);

	bool								AddHtbSkill(CHtbSkill * pHtbSkill);

	CHtbSkill*							GetHtbSkill(BYTE bySlot);

	CHtbSkill*							FindHtbSkill(TBLIDX tblidx);

	bool								CanLearnHtbSkill(TBLIDX tblidx, WORD & rwResultCode);

	void								LearnHtb(TBLIDX skillTblidx, WORD & rwResultCode);

public:

	int									CopyToInfo(sHTB_SKILL_INFO * pHtbInfo);
	int									CopyToData(sHTB_SKILL_DATA * pHtbData);

	void								_SaveHtbCooldowns();

private:

	CPlayer*							m_pPlayerRef;

	CHtbSkill **						m_ppHtbSkill;

	CNtlLinkArray_Static<CHtbSkill *>	m_htbList;

};


#endif