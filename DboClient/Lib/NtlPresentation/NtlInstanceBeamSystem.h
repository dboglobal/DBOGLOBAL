//***********************************************************************************
//	File		:	NtlInstanceBeamSystem.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlinstanceImVertexsystem.h"
#include ".\ntlresourcebeamsystem.h"

class CNtlInstanceBeamSystem : public CNtlInstanceImVertexSystem
{
public:
    CNtlResourceBeamSystem*			m_pResourceBeamSystem;
    CNtlInstanceComponentSystem*	m_pHeadSystem;

protected:

	enum COUNT
	{
		VERTEX_COUNT = 12,
		INDEX_COUNT = 18
	};
	static RwImVertexIndex			indices[INDEX_COUNT];

	RwReal							m_fStopTime;
	RwV3d							m_vStartPoint;
	RwV3d							m_vEndPoint;    
    
public:
	CNtlInstanceBeamSystem(void);
	virtual ~CNtlInstanceBeamSystem(void);

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual void	Init();
	virtual void	Reset();

	virtual void	Delete();
	virtual RwBool	Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld);

	virtual void	Stop();

			void	SetVertexUv();

	virtual RwBool	Update(RwReal fElapsedTime);
	virtual RwBool	Render();

protected:
    void	BuildEmitterStandard(SNtlBeamEmitterStandard* pEmitterStandard);
    void	UpdateVertices();
    void	UpdateVertices(RwReal fElapsedTime, RwReal fLifeTime);
    virtual RwBool  UpdateFinish(RwReal fElapsedTime);                  ///< Finish 작업을 처리한다.
};
