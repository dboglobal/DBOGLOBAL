//***********************************************************************************
//	File		:	NtlInstanceHurricaneSystem.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlinstanceImVertexsystem.h"
#include ".\ntlresourceHurricanesystem.h"

class CNtlInstanceHurricaneSystem : public CNtlInstanceImVertexSystem
{
public:
    RwInt32							m_nStartPoint;
    RwInt32							m_nCurrentPoint;

protected:
    CNtlResourceHurricaneSystem*	m_pResourceHurricaneSystem;

    RwReal							m_fHurricaneAngle;
    RwReal							m_fDeltaAngle;

    RwReal							m_fDeltaAlpha;
    RwReal                          m_fTemp;
    std::vector<RwV3d>              m_vPoint;
    RwV3d                           m_vPrev;

public:
	CNtlInstanceHurricaneSystem(void);
	virtual ~CNtlInstanceHurricaneSystem(void);

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual void	Init();
	virtual void	Reset();

	virtual void	Delete();
	virtual RwBool	Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld);

	virtual void	SetVertexColor(RwRGBA& color);
    virtual void    SetAlpha(RwUInt8 byValue);                     ///< 알파값을 설정한다.

	virtual RwBool	Update(RwReal fElapsedTime);
	virtual RwBool	Render();

			
protected:
    void	BuildEmitterStandard(SNtlHurricaneEmitterStandard* pEmitterStandard);
	void	UpdateVertices(RwReal fElapsedTime, RwReal fLifeTime);
    void    UpdateVanish(RwReal fElapsedTime);                      ///< 알파를 없애서 소멸효과를 만든다.
    virtual RwBool  UpdateFinish(RwReal fElapsedTime);              ///< Finish 효과를 처리한다.

};
