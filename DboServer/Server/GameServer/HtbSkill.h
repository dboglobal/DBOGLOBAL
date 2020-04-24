#ifndef __PC_HTB_SKILL__
#define __PC_HTB_SKILL__

#include "HTBSetTable.h"

class CPlayer;

class CHtbSkill
{

public:

	CHtbSkill();
	virtual ~CHtbSkill();

public:

	bool						Create(sHTB_SET_TBLDAT *pHtbDataRef, CPlayer *pOwnerRef, BYTE skillIndex, DWORD dwTimeRemaining);

	void						Destroy();

	void						Init();

public:

	void						TickProcess(DWORD dwTickDiff);

	void						OnLearnHtbSkill();

public:

	TBLIDX						GetHtbID() { return m_pHtbDataRef->tblidx; }
	DWORD						GetCoolTimeRemaining() { return m_dwTimeRemaining; }
	void						SetCoolTimeRemaining(DWORD dwTime) { m_dwTimeRemaining = dwTime; }
	sHTB_SET_TBLDAT*			GetTbldat() { return m_pHtbDataRef; }

	void						SetSkillIndex(BYTE byIndex) { m_byHtbSlot = byIndex; }
	BYTE						GetSkillIndex() { return m_byHtbSlot; }

public:

	int							CopyToInfo(sHTB_SKILL_INFO * pHtbInfo);
	int							CopyToData(sHTB_SKILL_DATA * pHtbData);

public:

	void						SetCurrentStep(BYTE byStep) { m_byStep = byStep; }
	BYTE						GetCurrentStep() { return m_byStep; }

	void						SetHitCount(BYTE byCount) { m_byHitCount = byCount; }
	BYTE						GetHitCount() { return m_byHitCount; }

	void						SetTarget(HOBJECT hTarget) { m_hTarget = hTarget; }
	HOBJECT						GetTarget() { return m_hTarget; }

	void						SetAttackFailed(bool bFlag) { m_bAtkFailed = bFlag; }
	bool						GetAttackFailed() { return m_bAtkFailed; }

private:

	BYTE						m_byHtbSlot;

	DWORD						m_dwTimeRemaining;

	sHTB_SET_TBLDAT *			m_pHtbDataRef;

	CPlayer*					m_pPlayerRef;

private:

	HOBJECT						m_hTarget;

	BYTE						m_byStep;

	BYTE						m_byHitCount;

	bool						m_bAtkFailed;

};


#endif