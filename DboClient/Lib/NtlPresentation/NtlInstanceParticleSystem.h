//***********************************************************************************
//	File		:	NtlInstanceParticleSystem.h
//	Desc		:	
//	Begin		:	2006. 7.28
//	Copyright	:	ⓒ 2006 by NTL CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlinstancecomponentsystem.h"

#include ".\ntlresourceparticlesystem.h"

#include "NtlInstanceTrailSystem.h"
/**
 * \ingroup NtlPresentation
 * \brief Particle Instance를 구성하는 클래스
 * \date 2006-08-28
 * \author agebreak
 */
class CNtlInstanceParticleSystem : public CNtlInstanceComponentSystem
{
public:
	CNtlResourceParticleSystem* m_pResourceParticleSystem;

	RpPrtStdEmitterClass*		m_pEmitterClass;
	RpPrtStdParticleClass*		m_pParticleClass;

	RpPrtStdEmitter*			m_pEmitter;
	RpPrtStdEmitterStandard*	m_pEmitterStandard;    

	RpAtomic*					m_pAtomic;
	RpPrtStdEmitterPTank*		m_pTankProperties;

	RwTexture*					m_pStandardTexture;

	RwInt32						m_nVelocityOffset;
	// hurricane
	RwReal						m_fHurricaneLifeTime;
	RwReal						m_fHurricaneDeltaTime;
    
    // Trail System (Trail Action)
    CNtlInstanceTrailSystem*    m_pTrailSystem;

protected:
    // Matrix Action이 적용되었을때 Rotation 관련
    RpPrtStdEmitterPrtMatrix*   m_pEmitterPrtMatrix;
    RwV3d                       m_vOrgAt;           ///< 원래 설정된 At값
    RwV3d                       m_vOrgUp;           ///< 원래 설정된 Up값    

    // Follow Particle 관련 (파티클 생성객체를 따라다니는 파티클)
    RwV3d                       m_vPosPrev;         ///< 생성 객체의 이전 위치
    RwV3d                       m_vPosDelta;        ///< 이전 위치로 부터 변경된 위치    

public:
	CNtlInstanceParticleSystem(void);
	virtual ~CNtlInstanceParticleSystem(void);

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual void	Init();
	virtual void	Reset();

	virtual void	Delete();
	virtual RwBool	Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld);
	virtual void	Stop();    

			RpPrtStdEmitter*	SubEmitterCreate(CNtlResourceParticleSystem* pResourceSystem);

	virtual RwBool	Update(RwReal fElapsedTime);
	virtual RwBool	Render();

	virtual void	SetWorldMatrix(const RwMatrix& matWorld);
	virtual void	SetScale(RwReal fScale);					///< 파티클의 Scale을 설정한다.
    virtual void    SetAlpha(RwUInt8 byValue);                  ///< 파티클의 Alpha를 설정한다.

			void	BuildHurricanePoint(RwV3d& vPosition, RwReal fLifeTime);

protected:
            void    UpdateFollow();                             ///< Follow 파티클의 위치를 업데이트한다.    
            void    UpdateBillboard();                          ///< 빌보드가 설정된경우 빌보드를 적용한다.
            void    UpdateRotate();                             ///< Emitter의 Rotate값을 적용한다.
};
