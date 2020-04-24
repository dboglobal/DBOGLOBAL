//***********************************************************************************
//	File		:	VenusEventEnemy.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once


#include "NtlPLResource.h"

class CVenusEventEnemy
{
public:
	std::string			m_strClumpName;
	std::string			m_strIdleAnimationName;
	std::string			m_strDamageAnimationName;

	std::string			m_strEffectName;


	CNtlPLResource*		m_pClumpResource;
	CNtlPLResource*		m_pIdleAnimationResource;
	CNtlPLResource*		m_pDamageAnimationResource;

	RtAnimAnimation*	m_pCurrentAnimation;

	RwReal				m_fCurrentTime;


public:
	CVenusEventEnemy(void);
	~CVenusEventEnemy(void);

	void	Delete();

	void	DeleteResource();
	void	CreateResource();

	RwBool	SetDamage();
	RwBool	SetPosition(RwV3d& vPosition);

	RwBool	Update(RwReal fElapsedTime);
	RwBool	Render();

	RwBool	Save(FILE* pFile);
	RwBool	Load(FILE* pFile);
};
