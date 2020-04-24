//***********************************************************************************
//	File		:	NtlResourcePostEffectSystem.h
//	Desc		:	
//	Begin		:	2006. 7.28
//	Copyright	:	¨Ï 2006 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include "NtlResourceComponentSystem.h"

class CNtlResourcePostEffectSystem : public CNtlResourceComponentSystem
{
public:
    SNtlPostEffectEmitterStandard	m_EmitterStandard;
    RpPrtStdEmitterPrtColor			m_EmitterPrtCol;
    RpPrtAdvPrtMultiColorEmitter	m_EmitterPrtMultiCol;
	
	std::string						m_strTextureName;

public:
	CNtlResourcePostEffectSystem(const RwChar* strName);
	virtual ~CNtlResourcePostEffectSystem(void);

	virtual void	Destroy();

	virtual RwBool	Load(FILE* pFile);
};
