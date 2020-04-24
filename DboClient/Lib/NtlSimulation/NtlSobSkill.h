/*****************************************************************************
 *
 * File			: NtlSobSkill.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 4. 24	
 * Abstract		: Simulation skill object
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_SKILL_H__
#define __NTL_SOB_SKILL_H__

#include "NtlSob.h"
#include "NtlDebug.h"

class CNtlSobSkillIcon;

class CNtlSobSkill : public CNtlSob
{
	DECLEAR_MEMORY_POOL(CNtlSobSkill, NTL_DEFAULT_MEMORY_POOL)
	
private:

	RwInt32 m_bySlotIdx;

	CNtlSobSkillIcon *m_pIcon;

public:

	CNtlSobSkill();
	virtual ~CNtlSobSkill();

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

inline void CNtlSobSkill::SetSlotIdx(RwUInt8 bySoltIdx)
{
	m_bySlotIdx = bySoltIdx;
}

inline RwUInt8 CNtlSobSkill::GetSlotIdx(void) const
{
	return (RwUInt8)m_bySlotIdx;
}


#endif
