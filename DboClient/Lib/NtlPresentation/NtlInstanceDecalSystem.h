//***********************************************************************************
//	File		:	NtlInstanceDecalSystem.h
//	Desc		:	
//	Begin		:	2007. 3.19
//	Copyright	:	
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlinstanceImVertexsystem.h"
#include ".\ntlresourcedecalsystem.h"
#include "NtlPLDecal.h"

struct DecalParam
{
	RwV3d vPos;
	RwV3d vSize;
	RwInt32 *pBufferCnt;
	RwIm3DVertex *pVertices;
};

class CNtlInstanceDecalSystem : public CNtlInstanceComponentSystem
{
	// Vertex를 사용하지만, CNtlDecal을 사용하기 때문에 ImvertexSystem에서 상속 받지 않는다.
public:
	CNtlResourceDecalSystem*		m_pResourceDecalSystem;			///< 프로퍼티 정보를 가지고 있는 리소스 객체
	RwBool							m_bUpdateVertex;
	CNtlPLDecal*					m_pDecalSystem;					///< 실제 Decal을 그리는 Decal 객체

public:
	CNtlInstanceDecalSystem(void);
	virtual ~CNtlInstanceDecalSystem(void);

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual void	Init();
	virtual void	Reset();

	virtual void	Delete();
	virtual RwBool	Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld);

			void	SetVertexUv();
    virtual void    SetAlpha(RwUInt8 byValue);
    virtual void	SetScale(RwReal fScale);									 ///< 이펙트의 Scale을 설정한다. (현재는 파티클에서만 유효하다)

	virtual RwBool	Update(RwReal fElapsedTime);
	virtual RwBool	Render();

			void	BuildEmitterStandard(SNtlDecalEmitterStandard* pEmitterStandard);

	virtual void	SetWorldMatrix(const RwMatrix& matWorld);

	RwInt32	GetPolyCount();											///< 현재 그려지고 있는 폴리곤의 개수를 반환한다. (Vertex 개수 / 3)
};

