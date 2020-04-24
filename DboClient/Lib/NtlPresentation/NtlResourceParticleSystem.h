//***********************************************************************************
//	File		:	NtlResourceParticleSystem.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlresourcecomponentsystem.h"


class CNtlResourceParticleSystem : public CNtlResourceComponentSystem
{
public:
	RwUInt32						m_nPointEmitterDataFlag;

	RwInt32							m_nParticleBatchMaxCount;

	RpPrtStdEmitterStandard			m_EmitterStandard;
	std::string						m_strTextureName;

	RpPrtStdEmitterPrtColor			m_EmitterPrtCol;
	RpPrtStdEmitterPrtTexCoords		m_EmitterPrtTexCoords;
	RpPrtStdEmitterPrt2DRotate		m_EmitterPrtRotate;
	RpPrtStdEmitterPrtSize			m_EmitterPrtSize;
	RpPrtStdEmitterPrtMatrix		m_EmitterPrtMatrix;

	SNtlPrtStdEmitterPrtOrbitPoint	m_EmitterPrtOrbitPoint;
	SNtlPrtStdEmitterPrtHurricane	m_EmitterPrtHurricane;

	RpPrtAdvPrtEmitterEmitter		m_PrtEmitterEmitter;
	RpPrtAdvPrtMultiColorEmitter	m_EmitterPrtMultiCol;
	RpPrtAdvEmtPrtMultiTexCoords	m_EmitterPrtMultiTexCoords;
	RpPrtAdvPrtMultiSizeEmitter		m_EmitterPrtMultiSize;

	RpPrtAdvPointListEmitter		m_EmitterPtList;
	RpPrtAdvSphereEmitter			m_EmitterPtrSphere;
    RpPrtAdvCircleEmitter			m_EmitterPtrCircle;

	std::string						m_strSubSystem;

    SNtlPrtStdEmitterTrail          m_EmitterTrail;             ///< Trail Action 관련 프로퍼티
	RwReal							m_fDepthBias;				///< Depth Bias 값

public:
	CNtlResourceParticleSystem(const RwChar* strName);
	virtual ~CNtlResourceParticleSystem(void);

	virtual void	Destroy();

			RwInt32	GetEmitterDataFlag();
			RwInt32	GetParticleDataFlag();

			RwBool	CreateParticleClass(RpPrtStdEmitterClass*& pEmitterClass, RpPrtStdParticleClass*& pParticleClass);
			RwBool	ResourceDataCopy(RpPrtStdEmitterClass* pEmitterClass, RpPrtStdEmitter* pEmitter);

	virtual RwBool	Load(FILE* pFile);

public:
	RwBool	IsPointEmitterDataFlag(RwUInt32 flag)		{ return (m_nPointEmitterDataFlag == flag) ? TRUE : FALSE; }
    
    void    SetFollowFlag(BOOL bFollow);                ///< 파티클이 생성위치를 따라다닐지 유무를 설정한다.
};
