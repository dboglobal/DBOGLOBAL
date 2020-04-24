/*****************************************************************************
*
* File			: NtlSobHTBSkillAttr.h
* Author		: Peessi
* Copyright		: (주)NTL
* Date			: 2006. 8. 1	
* Abstract		: Simulation object skill attribute base class
*****************************************************************************
* Desc          : NtlSobSkillAttr에서 분리 
*
*****************************************************************************/

#ifndef __NTL_SOB_HTBSKILL_ATTR_H__
#define __NTL_SOB_HTBSKILL_ATTR_H__

#include "NtlSobAttr.h"

class CNtlSobHTBSkillAttr : public CNtlSobAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobHTBSkillAttr, NTL_DEFAULT_MEMORY_POOL)

public:

	sHTB_SET_TBLDAT *m_pHTBSkillTbl;	/** skill table data pointer */

public:

	CNtlSobHTBSkillAttr() 
	{ 
		m_pHTBSkillTbl = NULL;
	}

	virtual ~CNtlSobHTBSkillAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_HTBSKILL); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	void SetHTBSkillTbl(const sHTB_SET_TBLDAT *pSkillTbl);
	sHTB_SET_TBLDAT* GetHTBSkillTbl(void) const;
};

inline void CNtlSobHTBSkillAttr::SetHTBSkillTbl(const sHTB_SET_TBLDAT *pHTBSkillTbl)
{
	m_pHTBSkillTbl = const_cast<sHTB_SET_TBLDAT*>(pHTBSkillTbl);
}

inline sHTB_SET_TBLDAT* CNtlSobHTBSkillAttr::GetHTBSkillTbl(void) const
{
	return m_pHTBSkillTbl;
}


#endif