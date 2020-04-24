//***********************************************************************************
//	File		:	VenusEventBall.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include "NtlPLEntity.h"

class CVenusEventBall
{
public:
	RwReal				m_fLifeTime;
	CNtlPLEntity*		m_pEntity;

	RwV3d				m_vTargetPosition;
public:
	CVenusEventBall(CNtlPLEntity* pEntity, RwV3d& vTargetPosition);
	~CVenusEventBall(void);

	RwBool	Update(RwReal fElapsedTime);
	RwBool	Render();

public:
	RwMatrix		GetMatrix()			{ return m_pEntity->GetMatrix(); }
};
