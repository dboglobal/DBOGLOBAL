/*****************************************************************************
*
* File			: DboTLConditionCheckState.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 4. 29
* Abstract		: 
*****************************************************************************
* Desc          : 튜토리얼중 유저의 연습 상태의 상태를 관리한다
*
*****************************************************************************/

#ifndef __NTL_TL_CONDITION__CHECK_STATE_H__
#define __NTL_TL_CONDITION__CHECK_STATE_H__

// core
#include "ceventhandler.h"

// simulation
#include "NtlSLDef.h"


class CDboConditionCheckBase
{
public:
	virtual VOID	Update(RwReal fElapsed) {}
	virtual VOID	HandleEvents(RWS::CMsg &pMsg) {}
};


class CDboConditionCheck_MouseDrag : public CDboConditionCheckBase
{
public:
	CDboConditionCheck_MouseDrag() : m_fYaw(0.f)
	{}

	virtual VOID		Update(RwReal fElapsed);

protected:
	RwReal			m_fYaw;
};

class CDboConditionCheck_CheckPoint : public CDboConditionCheckBase
{
public:
	virtual VOID		HandleEvents(RWS::CMsg &pMsg);
};

class CDboConditionCheck_Mob_Target : public CDboConditionCheckBase
{
public:
	virtual VOID		HandleEvents(RWS::CMsg &pMsg);
};

class CDboConditionCheck_Mob_NormalAttack : public CDboConditionCheckBase
{
public:
	virtual VOID		HandleEvents(RWS::CMsg &pMsg);
};

class CDboConditionCheck_Mob_SkillAttack : public CDboConditionCheckBase
{
public:
	virtual VOID		HandleEvents(RWS::CMsg &pMsg);
};

#endif