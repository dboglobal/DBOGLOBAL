/*****************************************************************************
*
* File			: NtlSobActionSkillAttr.h
* Author		: Peessi
* Copyright		: (주)NTL
* Date			: 2006. 8. 30	
* Abstract		: Simulation object skill attribute base class
*****************************************************************************
* Desc          : NtlSobSkillAttr에서 분리 
*
*****************************************************************************/

#ifndef __NTL_SOB_ACTIONSKILL_ATTR_H__
#define __NTL_SOB_ACTIONSKILL_ATTR_H__

#include "NtlSobAttr.h"

class CNtlSobActionSkillAttr : public CNtlSobAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobActionSkillAttr, NTL_DEFAULT_MEMORY_POOL)

public:

	sACTION_TBLDAT *m_pActionSkillTbl;	/** skill table data pointer */

public:

	CNtlSobActionSkillAttr() 
	{ 
		m_pActionSkillTbl = NULL;
	}

	virtual ~CNtlSobActionSkillAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_ACTIONSKILL); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	void SetActionSkillTbl(const sACTION_TBLDAT *pSkillTbl);
	sACTION_TBLDAT* GetActionSkillTbl(void) const;
};

inline void CNtlSobActionSkillAttr::SetActionSkillTbl(const sACTION_TBLDAT *pActionSkillTbl)
{
	m_pActionSkillTbl = const_cast<sACTION_TBLDAT*>(pActionSkillTbl);
}

inline sACTION_TBLDAT* CNtlSobActionSkillAttr::GetActionSkillTbl(void) const
{
	return m_pActionSkillTbl;
}


#endif