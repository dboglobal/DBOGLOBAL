//***********************************************************************************
//	File		:	NtlResourceImVertexSystem.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlresourcecomponentsystem.h"

class CNtlResourceImVertexSystem : public CNtlResourceComponentSystem
{
public:

	std::string						m_strTextureName;

	RpPrtStdEmitterPrtColor			m_EmitterPrtCol;
	SNtlPrtStdEmitterPrtSize		m_EmitterPrtSize;

	RpPrtAdvPrtMultiColorEmitter	m_EmitterPrtMultiCol;
	SNtlAdvMultiSizeEmitter			m_EmitterPrtMultiSize;

    SNtlPrtStdEmitterPrtFinish      m_EmitterPrtFinish;             ///< Finish 연출에 사용되는 프로퍼티

public:
	CNtlResourceImVertexSystem(const RwChar* strName);
	virtual ~CNtlResourceImVertexSystem(void);

	virtual void	Destroy();

	virtual RwBool	Load(FILE* pFile);
};
