/*****************************************************************************
*
* File			: NtlSobActionSkill.h
* Author		: Peessi
* Copyright		: (¡÷)NTL
* Date			: 2006. 8. 30	
* Abstract		: Simulation Action skill object
*****************************************************************************
* Desc          : Action Skill ∫–∏Æ. 
*
*****************************************************************************/

#ifndef __NTL_SOB_ACTION_SKILL_H__
#define __NTL_SOB_ACTION_SKILL_H__

#include "NtlSob.h"
#include "NtlDebug.h"

class CNtlSobActionSkillIcon;

class CNtlSobActionSkill : public CNtlSob
{
	DECLEAR_MEMORY_POOL(CNtlSobActionSkill, NTL_DEFAULT_MEMORY_POOL)

private:

	RwUInt8 m_bySlotIdx;

	CNtlSobActionSkillIcon *m_pIcon;

public:

	CNtlSobActionSkill();
	virtual ~CNtlSobActionSkill();

	virtual RwBool Create(void);

	virtual void Destroy(void);

	virtual void Update(RwReal fElapsed);

	virtual void HandleEvents(RWS::CMsg &pMsg);

	// item attribute function

public:

	CNtlSobIcon* GetIcon(void) const;

public:

	void SetSlotIdx(RwUInt8 bySoltIdx);

	RwUInt8 GetSlotIdx(void) const;
};

inline void CNtlSobActionSkill::SetSlotIdx(RwUInt8 bySoltIdx)
{
	m_bySlotIdx = bySoltIdx;
}

inline RwUInt8 CNtlSobActionSkill::GetSlotIdx(void) const
{
	return m_bySlotIdx;
}

#endif
