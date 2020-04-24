/*****************************************************************************
 *
 * File			: NtlSobNpc.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 11. 30	
 * Abstract		: Simulation object npc attribute base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOBNPC_ATTR_H__
#define __NTL_SOBNPC_ATTR_H__

#include "NtlSobAttr.h"

class CNtlSobNpcAttr : public CNtlSobBattleAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobNpcAttr, NTL_DEFAULT_MEMORY_POOL)
	
public:

	sNPC_TBLDAT *m_pNpcTbl;		/** mob table data pointer */
		
public:

	CNtlSobNpcAttr();

	virtual ~CNtlSobNpcAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_NPC); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	sNPC_TBLDAT* GetNpcTbl(void) const;
};

#endif
