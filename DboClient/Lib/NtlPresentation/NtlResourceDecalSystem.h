//***********************************************************************************
//	File		:	NtlResourceDecalSystem.h
//	Desc		:	
//	Begin		:	2007. 3.21
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlresourceImVertexsystem.h"

class CNtlResourceDecalSystem : public CNtlResourceImVertexSystem
{
public:
	SNtlDecalEmitterStandard		m_EmitterStandard;
	RpPrtStdEmitterPrt2DRotate		m_EmitterPrtRotate;         ///< Rotate Emitter
	SNtlPrtStdEmitterPrtUVAnim		m_EmitterUVAnim;			///< UV Anim Emitter

public: 
	CNtlResourceDecalSystem(const RwChar* strName);
	virtual	~CNtlResourceDecalSystem(void);

	virtual void	Destroy();

	virtual RwBool	Load(FILE* pFile);
};
