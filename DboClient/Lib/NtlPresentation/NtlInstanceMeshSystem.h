//***********************************************************************************
//	File		:	NtlInstanceMeshSystem.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once


#include ".\ntlinstancecomponentsystem.h"
#include "NtlResourceMeshSystem.h"

#include "NtlPLResourceManager.h"
#include "NtlPLEntityRenderHelpers.h"

#include "NtlInstanceTrailSystem.h"

#include <rpuvanim.h>
#include <rtdict.h>

class CNtlInstanceMeshSystem : public CNtlInstanceComponentSystem
{
public:
	CNtlInstanceMeshSystem(void);
	virtual ~CNtlInstanceMeshSystem(void);

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual void	Init();
	virtual void	Reset();

	virtual void	Delete();
	virtual RwBool	Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld);

	virtual RwBool	Update(RwReal fElapsedTime);
	virtual RwBool	Render();

	RwBool		    Finish();                                                       ///< 이펙트의 삭제 작업을 처리한다.

	virtual void	SetWorldMatrix(const RwMatrix& matWorld);
	virtual void    SetAlpha(RwUInt8 byValue);                                      ///< 이펙트의 Alpha값을 적용한다.
    virtual void	SetScale(RwReal fScale);

    static void    CreateToonData(RpClump* pClump, SToonData* pToonData);
protected:
    RtDict* UVAnimDictLoad(const RwChar* strFullName);
    void	BuildEmitterStandard(SNtlMeshEmitterStandard* pEmitterStandard);
    void	UpdateSize(RwReal fDeltaTime);
    void	UpdateColor(RwReal fDeltaTime);
    void	UpdateRotate(RwReal fDeltaTime);
    virtual RwBool  UpdateFinish(RwReal fElapsedTime);          ///< Finish 작업을 처리한다.
    void    CalcBoundingSphere();
    

protected:
    CNtlResourceMeshSystem*			m_pResourceMeshSystem;	
    CNtlPLResource*					m_pClumpResource;
    CNtlPLResource*					m_pAnimationResource;

    RtDict*							m_pUvAnimDict;
    RwSList*						m_pAnimationMaterialList;

    RwRGBA							m_Color;   

    RwInt32							m_nSizeSeed;

    RwBool							m_bDefaultRotate;

    RwBool							m_bUpdateRenderMatrix;

    RwMatrix*                       m_pMatEmitter;                   ///< Attach Option이 False 상태일때 처음 Emitter의 Matrix
    RwMatrix                        m_matInvScale;                   ///< Scale 행렬의 역행렬. Spline 계산시 사용된다.
    RpHAnimHierarchy*               m_pAnimhierarchy;                ///< Clump의 Hierarchy (Animation이 있을때만 사용)

    CNtlInstanceTrailSystem*        m_pTrailSystem;                  ///< Trail System (Trail Action)
    SToonData*                      m_pToonData;                     ///< 툰이 적용된 메시를 위한 툰 정보
};
