//***********************************************************************************
//	File		:	NtlInstanceImVertexSystem.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlinstancecomponentsystem.h"
#include ".\ntlresourceImVertexsystem.h"

class CNtlInstanceImVertexSystem : public CNtlInstanceComponentSystem
{
public:
	CNtlResourceImVertexSystem*		m_pResourceImVertexSystem;

	RwV3d							m_vImVertexSize;

	RwInt32							m_nVertexCount;
	RwIm3DVertex*					m_pVertices;
    RwRGBA                          m_sColor;

public:
	CNtlInstanceImVertexSystem(void);
	virtual ~CNtlInstanceImVertexSystem(void);

	virtual void	Init();
	virtual void	Reset();

	virtual void	Delete();
	virtual RwBool	Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld);
    virtual RwBool	Update(RwReal fElapsedTime);

	virtual void	SetVertexColor(RwRGBA& color);
    virtual void    SetAlpha(RwUInt8 byValue);                                         ///< Efffect의 Alpha값을 설정한다.

	virtual void	SetWorldMatrix(const RwMatrix& matWorld);

	virtual void	BeginRenderState(CNtlResourceComponentSystem* pResourceSystem);
	virtual void	EndRenderState(CNtlResourceComponentSystem* pResourceSystem);

protected:
    virtual	void	UpdateColor(RwReal fDeltaTime);
    virtual void	UpdateSize(RwReal fDeltaTime);
    virtual RwUInt8 UpdateFinishAlpha(RwUInt8 uiOriginAlpha, RwReal fElapsedTime, RwReal fFinishTime);  ///< Finish시의 Alpha값 처리를 한다.
};
