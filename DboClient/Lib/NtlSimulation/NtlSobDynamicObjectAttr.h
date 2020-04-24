/*****************************************************************************
*
* File			: NtlSobDynamicObjectAttr.h
 * Author		: Haesung, Cho
* Copyright		: (аж)NTL
* Date			: 2008. 12. 15	
* Abstract		: Simulation Dynamic object attribute base class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_SOB_DYNAMICOBJECT_ATTR_H__
#define __NTL_SOB_DYNAMICOBJECT_ATTR_H__

#include "NtlSobAttr.h"
#include "DynamicObjectTable.h"

class CNtlSobDynamicObjectAttr : public CNtlSobLifeAttr
{
	DECLEAR_MEMORY_POOL( CNtlSobDynamicObjectAttr, NTL_DEFAULT_MEMORY_POOL )

public:
	sDYNAMIC_OBJECT_TBLDAT	*m_pDynamicObjTbl;		///< Dynamic object table pointer

	// Dynamic Object Type
	RwUInt8					m_byDynamicObjectType;
	
	// Dynamic Object Table Index
	TBLIDX					m_idxTableForType;

public:
	CNtlSobDynamicObjectAttr();

	virtual ~CNtlSobDynamicObjectAttr() {}

	virtual RwBool	Create(void) { SetClassID(SLCLASS_DYNAMIC_OBJECT); return TRUE; }

	virtual void	Destroy(void) {}

	virtual void	HandleEvents(RWS::CMsg& pMsg);

public:
	void			SetDynamicObjectTbl(const sDYNAMIC_OBJECT_TBLDAT* pDynamicObjTbl);

	sDYNAMIC_OBJECT_TBLDAT*	GetDynamicObjectTbl(void) const;

	RwUInt8			GetDynamicObjectType();

	TBLIDX			GetTableIndexForType();
};

inline sDYNAMIC_OBJECT_TBLDAT* CNtlSobDynamicObjectAttr::GetDynamicObjectTbl(void) const
{
	return m_pDynamicObjTbl;
}

#endif