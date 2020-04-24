/*****************************************************************************
*
* File			: NtlSobHTBSkill.h
* Author		: Peessi
* Copyright		: (¡÷)NTL
* Date			: 2006. 8. 1	
* Abstract		: Simulation HTB skill object
*****************************************************************************
* Desc          : HTB Skill ∫–∏Æ. 
*
*****************************************************************************/

#ifndef __NTL_SOB_HTB_SKILL_H__
#define __NTL_SOB_HTB_SKILL_H__

#include "NtlSob.h"
#include "NtlDebug.h"

class CNtlSobHTBSkillIcon;

class CNtlSobHTBSkill : public CNtlSob
{
	DECLEAR_MEMORY_POOL(CNtlSobHTBSkill, NTL_DEFAULT_MEMORY_POOL)

private:

	RwInt32 m_bySlotIdx;

	CNtlSobHTBSkillIcon *m_pIcon;

public:

	CNtlSobHTBSkill();
	virtual ~CNtlSobHTBSkill();

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

inline void CNtlSobHTBSkill::SetSlotIdx(RwUInt8 bySoltIdx)
{
	m_bySlotIdx = bySoltIdx;
}

inline RwUInt8 CNtlSobHTBSkill::GetSlotIdx(void) const
{
	return (RwUInt8)m_bySlotIdx;
}


#endif
