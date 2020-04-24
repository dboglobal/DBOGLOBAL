/*****************************************************************************
*
* File			: NtlSobTriggerObjectAttr.h
 * Author		: HyungSuk, Jang
* Copyright		: (аж)NTL
* Date			: 2006. 11. 21	
* Abstract		: Simulation object trigger object attribute base class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_SOB_TRIGGEROBJECT_ATTR_H__
#define __NTL_SOB_TRIGGEROBJECT_ATTR_H__

#include "NtlSobAttr.h"

class CNtlSobTriggerObjectAttr : public CNtlSobLifeAttr
{
	DECLEAR_MEMORY_POOL( CNtlSobTriggerObjectAttr, NTL_DEFAULT_MEMORY_POOL )

public:

	sOBJECT_TBLDAT *m_pTriggerObjTbl;	/** trigger object table data pointer */

public:

	CNtlSobTriggerObjectAttr(); 

	virtual ~CNtlSobTriggerObjectAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_TRIGGER_OBJECT); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	void			SetTriggerObjectTbl(const sOBJECT_TBLDAT *pTriggerObjTbl);
	sOBJECT_TBLDAT* GetTriggerObjectTbl(void) const;

	RwBool			IsHaveOperating(void);
};




inline sOBJECT_TBLDAT* CNtlSobTriggerObjectAttr::GetTriggerObjectTbl(void) const
{
	return m_pTriggerObjTbl;
}

#endif