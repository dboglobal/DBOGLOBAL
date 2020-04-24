/*****************************************************************************
 *
 * File			: NtlSobPetAtt.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 11. 30	
 * Abstract		: Simulation object player attribute base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOBPET_ATTR_H__
#define __NTL_SOBPET_ATTR_H__

#include "NtlSharedDef.h"
#include "NtlSobAttr.h"


class CNtlSobPetAttr : public CNtlSobBattleAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobPetAttr, NTL_DEFAULT_MEMORY_POOL)
	
public:

	RwUInt8		m_bySummonSourceType;	/** skill or item */	
	RwUInt32	m_uiSourceTblId;		/** skill or item table id */
	sNPC_TBLDAT *m_pNpcTbl;				/** mob table data pointer */
	
public:

	CNtlSobPetAttr(); 

	virtual ~CNtlSobPetAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_PET); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	sNPC_TBLDAT* GetNpcTbl(void) const;
};

#endif
