//***********************************************************************************
//	File		:	NtlEffectSystemFreeList.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	Agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include <map>

#include <rwcore.h>
#include <rpworld.h>
#include "NtlPLDef.h"

#include "NtlResourceFileManager.h"

#include "NtlPLResource.h"

class CNtlInstanceComponentSystem;

class CNtlEffectSystemFreeList
{
private:
	static	RwFreeList*		m_pParticleSystemFreeList;
	static	RwFreeList*		m_pMeshSystemFreeList;
	static	RwFreeList*		m_pBeamSystemFreeList;
	static	RwFreeList*		m_pHurricaneSystemFreeList;
	static	RwFreeList*		m_pLightningSystemFreeList;
	static	RwFreeList*		m_pDecalSystemFreeList;
	static	RwFreeList*		m_pPostEffectSystemFreeList;
    static  RwFreeList*     m_pTraceSystemFreeList;
    static  RwFreeList*     m_pLineSystemFreeList;    
    static  RwFreeList*     m_pTrailSystemFreeList;    
    static  RwFreeList*     m_pVertex1024;                          ///< 버텍스(1024) 메모리 풀
    static  RwFreeList*     m_pVertex5120;                          ///< 버텍스(5120) 메모리 풀

public:
	static	std::string					m_strClumpTexturePath;
	static	CNtlResourceFileManager		m_ClumpFileManager;
	static	CNtlResourceFileManager		m_AnimationFileManager;
	static	CNtlResourceFileManager		m_UvAnimationFileManager;
	static	CNtlResourceFileManager		m_TextureFileManager;
    static  CNtlResourceFileManager     m_SoundFileManager;                 ///< Sound File들의 리스트 (현재는 Venus에서만 사용한다)

	//typedef std::map<std::string, CNtlPLResource*, STRING_COMPARE_NOCASE>	smdef_PLResource;
	//static	smdef_PLResource		m_smClumpResource;
	//static	smdef_PLResource		m_smAnimationResource;	

	//typedef std::map<std::string, RwTexture*, STRING_COMPARE_NOCASE>	smdef_Texture;
	//static	smdef_Texture			m_smTextureResource;

public:

	static RwBool	Create(void);
	static void		Destroy(void);
	static void*	Alloc(const RwInt32 type);
	static void		Free(const RwInt32 type, void *pObj);

	static CNtlInstanceComponentSystem* CreateSystem(const RwInt32 type);
	static void							DeleteSystem(CNtlInstanceComponentSystem* pSystem);

	static CNtlPLResource*		FindClump(const RwChar* strName);
	static CNtlPLResource*		FindAnimation(const RwChar* strName);
	static RwTexture*			FindTexture(const RwChar* strName);

};
