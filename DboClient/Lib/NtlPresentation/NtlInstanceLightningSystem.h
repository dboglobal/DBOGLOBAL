//***********************************************************************************
//	File		:	NtlInstanceLightningSystem.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlinstanceImVertexsystem.h"
#include ".\ntlresourcelightningsystem.h"

class CNtlInstanceLightningSystem : public CNtlInstanceImVertexSystem
{
public:
	CNtlResourceLightningSystem*	m_pResourceLightningSystem;
    
	RwInt32							m_nLevelCount;    
	RwV3d*							m_pLocation;

	RwMatrix						m_matLightning;



	RwReal							m_fUpdateTime;

protected:
    RwV3d							m_vStartPoint;
    RwV3d							m_vEndPoint;
    RwV3d                           m_vFinishPoint;                 ///< Finish에 사용될 임시 변수

public:
	CNtlInstanceLightningSystem(void);
	virtual ~CNtlInstanceLightningSystem(void);

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual void	Init();
	virtual void	Reset();

	virtual void	Delete();
	virtual RwBool	Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld);

			void	SetVertexUv();

	virtual RwBool	Update(RwReal fElapsedTime);
	virtual RwBool	Render();

			

			
			RwReal	GetLineWidth(RwReal fRatio);
protected:
    void	BuildEmitterStandard(SNtlLightningEmitterStandard* pEmitterStandard);
    void	UpdateVertices();    
    void	CalculateMatrix(RwMatrix& matWorld, const RwV3d& vStart, const RwV3d& vEnd);
    void	CalculatePoints(const RwV3d& vStart, const RwV3d& vEnd, RwReal fAmp, RwReal fRough);
    virtual RwBool  UpdateFinish(RwReal fElapsedTime);                  ///< Finish 작업을 처리한다.
};
