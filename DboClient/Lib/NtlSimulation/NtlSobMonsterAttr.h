/*****************************************************************************
 *
 * File			: NtlSobMonsterAtt.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 11. 30	
 * Abstract		: Simulation object monster attribute base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOBMONSTER_ATTR_H__
#define __NTL_SOBMONSTER_ATTR_H__

#include "NtlDragonBall.h"

#include "NtlSobAttr.h"

class CNtlSobMonsterAttr : public CNtlSobBattleAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobMonsterAttr, NTL_DEFAULT_MEMORY_POOL)
	
public:

	sMOB_TBLDAT				*m_pMobTbl;						/** mob table data pointer */
	sMOVE_PATTERN_TBLDAT	*m_pMovePatternTbl;

	RwUInt8					m_byMovePatternId;

public:
    
	CNtlSobMonsterAttr(); 

	virtual ~CNtlSobMonsterAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_MONSTER); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	sMOB_TBLDAT*			GetMobTbl(void) const;

	sMOVE_PATTERN_TBLDAT*	GetMovePatternTbl(void) const;

	void					SetMovePatternId(RwUInt8 byPatternId);

	RwUInt8					GetMovePatternId(void) const;

	RwBool					IsHaveDragonBall();

protected:

	eDRAGON_BALL_TYPE		m_eDragonBallType;
};

inline sMOB_TBLDAT* CNtlSobMonsterAttr::GetMobTbl(void) const
{
	return m_pMobTbl;
}

inline sMOVE_PATTERN_TBLDAT* CNtlSobMonsterAttr::GetMovePatternTbl(void) const
{
	return m_pMovePatternTbl;
}

#endif
