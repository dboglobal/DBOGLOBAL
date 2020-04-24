//***********************************************************************************
//	File		:	NtlResourceBeamSystem.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlresourceImVertexsystem.h"

class CNtlResourceBeamSystem : public CNtlResourceImVertexSystem
{
public:
	SNtlBeamEmitterStandard			m_EmitterStandard;

public:
	CNtlResourceBeamSystem(const RwChar* strName);
	virtual ~CNtlResourceBeamSystem(void);

	virtual void	Destroy();

	virtual RwBool	Load(FILE* pFile);
};
