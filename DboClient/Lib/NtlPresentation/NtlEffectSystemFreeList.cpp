//***********************************************************************************
//	File		:	NtlEffectSystemFreeList.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************
#include "precomp_ntlpresentation.h"
#include ".\ntleffectsystemfreelist.h"

#include ".\ntlresourcecomponentsystem.h"

#include ".\ntlinstancemeshsystem.h"
#include ".\ntlinstanceparticlesystem.h"
#include ".\ntlinstancebeamsystem.h"
#include ".\ntlinstancehurricanesystem.h"
#include ".\ntlinstanceLightningsystem.h"
#include ".\ntlinstanceDecalsystem.h"
#include ".\ntlinstancePostEffectsystem.h"
#include "NtlInstanceTraceSystem.h"
#include "NtlInstanceLineSystem.h"
#include "NtlInstanceUpgradeEffectSystem.h"
#include "NtlInstanceTrailSystem.h"


RwFreeList*	CNtlEffectSystemFreeList::m_pParticleSystemFreeList = NULL;
RwFreeList*	CNtlEffectSystemFreeList::m_pMeshSystemFreeList = NULL;
RwFreeList*	CNtlEffectSystemFreeList::m_pBeamSystemFreeList = NULL;
RwFreeList*	CNtlEffectSystemFreeList::m_pHurricaneSystemFreeList = NULL;
RwFreeList*	CNtlEffectSystemFreeList::m_pLightningSystemFreeList = NULL;
RwFreeList*	CNtlEffectSystemFreeList::m_pDecalSystemFreeList = NULL;
RwFreeList*	CNtlEffectSystemFreeList::m_pPostEffectSystemFreeList = NULL;
RwFreeList* CNtlEffectSystemFreeList::m_pTraceSystemFreeList = NULL;
RwFreeList* CNtlEffectSystemFreeList::m_pLineSystemFreeList = NULL;
RwFreeList* CNtlEffectSystemFreeList::m_pTrailSystemFreeList = NULL;
RwFreeList* CNtlEffectSystemFreeList::m_pVertex1024 = NULL;
RwFreeList* CNtlEffectSystemFreeList::m_pVertex5120 = NULL;

std::string					CNtlEffectSystemFreeList::m_strClumpTexturePath;
CNtlResourceFileManager		CNtlEffectSystemFreeList::m_ClumpFileManager;
CNtlResourceFileManager		CNtlEffectSystemFreeList::m_AnimationFileManager;
CNtlResourceFileManager		CNtlEffectSystemFreeList::m_UvAnimationFileManager;
CNtlResourceFileManager		CNtlEffectSystemFreeList::m_TextureFileManager;
CNtlResourceFileManager     CNtlEffectSystemFreeList::m_SoundFileManager;

//CNtlEffectSystemFreeList::smdef_PLResource			CNtlEffectSystemFreeList::m_smClumpResource;
//CNtlEffectSystemFreeList::smdef_PLResource			CNtlEffectSystemFreeList::m_smAnimationResource;
//
//CNtlEffectSystemFreeList::smdef_Texture				CNtlEffectSystemFreeList::m_smTextureResource;

RwBool CNtlEffectSystemFreeList::Create(void)
{
	NTL_FUNCTION("CNtlEffectSystemFreeList::Create");

	RwUInt32 iBlockSize = 50;

    m_pParticleSystemFreeList = RwFreeListCreate(sizeof(CNtlInstanceParticleSystem), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pParticleSystemFreeList, "particle system failed to allocate free list");

	iBlockSize = 10;
    m_pMeshSystemFreeList = RwFreeListCreate(sizeof(CNtlInstanceMeshSystem), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pMeshSystemFreeList, "mesh system failed to allocate free list");

    m_pBeamSystemFreeList = RwFreeListCreate(sizeof(CNtlInstanceBeamSystem), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pBeamSystemFreeList, "beam system failed to allocate free list");

    m_pHurricaneSystemFreeList = RwFreeListCreate(sizeof(CNtlInstanceHurricaneSystem), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pHurricaneSystemFreeList, "hurricane system failed to allocate free list");

    m_pLightningSystemFreeList = RwFreeListCreate(sizeof(CNtlInstanceLightningSystem), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pLightningSystemFreeList, "Lightning system failed to allocate free list");

    m_pDecalSystemFreeList = RwFreeListCreate(sizeof(CNtlInstanceDecalSystem), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pDecalSystemFreeList, "Decal system failed to allocate free list");

    m_pPostEffectSystemFreeList = RwFreeListCreate(sizeof(CNtlInstancePostEffectSystem), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pPostEffectSystemFreeList, "PostEffect system failed to allocate free list");

    m_pTraceSystemFreeList = RwFreeListCreate(sizeof(CNtlInstanceTraceSystem), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pTraceSystemFreeList, "Trace System failed to allocate free list");

    m_pLineSystemFreeList = RwFreeListCreate(sizeof(CNtlInstanceLineSystem), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pLineSystemFreeList, "Line System failed to allocate free list");

    m_pTrailSystemFreeList = RwFreeListCreate(sizeof(CNtlInstanceTrailSystem), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pTrailSystemFreeList, "Trail System Failed to allocate free list");

    m_pVertex1024 = RwFreeListCreate(sizeof(RwIm3DVertex) * 1024, iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    m_pVertex5120 = RwFreeListCreate(sizeof(RwIm3DVertex) * 5120, 4, 16, rwMEMHINTDUR_GLOBAL);

    NTL_RETURN(TRUE);
}

void CNtlEffectSystemFreeList::Destroy(void)
{
	NTL_FUNCTION("CNtlEffectSystemFreeList::Destroy");

    if (m_pParticleSystemFreeList)
    {
        RwFreeListDestroy(m_pParticleSystemFreeList);
        m_pParticleSystemFreeList = NULL;
    }

	if (m_pMeshSystemFreeList)
    {
        RwFreeListDestroy(m_pMeshSystemFreeList);
        m_pMeshSystemFreeList = NULL;
    }

	if (m_pBeamSystemFreeList)
    {
        RwFreeListDestroy(m_pBeamSystemFreeList);
        m_pBeamSystemFreeList = NULL;
    }

	if (m_pHurricaneSystemFreeList)
    {
        RwFreeListDestroy(m_pHurricaneSystemFreeList);
        m_pHurricaneSystemFreeList = NULL;
    }

	if (m_pLightningSystemFreeList)
    {
        RwFreeListDestroy(m_pLightningSystemFreeList);
        m_pLightningSystemFreeList = NULL;
    }

	if (m_pDecalSystemFreeList)
    {
        RwFreeListDestroy(m_pDecalSystemFreeList);
        m_pDecalSystemFreeList = NULL;
    }

	if (m_pPostEffectSystemFreeList)
    {
        RwFreeListDestroy(m_pPostEffectSystemFreeList);
        m_pPostEffectSystemFreeList = NULL;
    }

    if(m_pTraceSystemFreeList)
    {
        RwFreeListDestroy(m_pTraceSystemFreeList);
        m_pTraceSystemFreeList = NULL;
    }

    if(m_pLineSystemFreeList)
    {
        RwFreeListDestroy(m_pLineSystemFreeList);
        m_pLineSystemFreeList = NULL;
    }

    if(m_pVertex1024)
    {
        RwFreeListDestroy(m_pVertex1024);
        m_pVertex1024 = NULL;
    }

    if(m_pVertex5120)
    {
        RwFreeListDestroy(m_pVertex5120);
        m_pVertex5120 = NULL;
    }

	m_AnimationFileManager.Destroy();
	m_UvAnimationFileManager.Destroy();
	m_ClumpFileManager.Destroy();
	m_TextureFileManager.Destroy();
    m_SoundFileManager.Destroy();

	NTL_RETURNVOID();
}

void* CNtlEffectSystemFreeList::Alloc(const RwInt32 type)
{
	NTL_FUNCTION("CNtlEffectSystemFreeList::Alloc");

	switch(type)
	{
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE:
		NTL_RETURN(RwFreeListAlloc(m_pParticleSystemFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_MESH:
		NTL_RETURN(RwFreeListAlloc(m_pMeshSystemFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM:
		NTL_RETURN(RwFreeListAlloc(m_pBeamSystemFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_HURRICANE:
		NTL_RETURN(RwFreeListAlloc(m_pHurricaneSystemFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING:
		NTL_RETURN(RwFreeListAlloc(m_pLightningSystemFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_DECAL:
		NTL_RETURN(RwFreeListAlloc(m_pDecalSystemFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT:
		NTL_RETURN(RwFreeListAlloc(m_pPostEffectSystemFreeList, rwMEMHINTDUR_GLOBAL));
		break;
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRACE:
        NTL_RETURN(RwFreeListAlloc(m_pTraceSystemFreeList, rwMEMHINTDUR_GLOBAL));
        break;
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE:
        NTL_RETURN(RwFreeListAlloc(m_pLineSystemFreeList, rwMEMHINTDUR_GLOBAL));
        break;    
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRAIL:
        NTL_RETURN(RwFreeListAlloc(m_pTrailSystemFreeList, rwMEMHINTDUR_GLOBAL));
        break;
    case CNtlResourceComponentSystem::RESOURCE_VERTEX_1024:
        NTL_RETURN(RwFreeListAlloc(m_pVertex1024, rwMEMHINTDUR_GLOBAL));
        break;
    case CNtlResourceComponentSystem::RESOURCE_VERTEX_5120:
        NTL_RETURN(RwFreeListAlloc(m_pVertex5120, rwMEMHINTDUR_GLOBAL));
        break;
	default:
		NTL_ASSERTE(!"default");
		break;
	}
	
	NTL_RETURN(NULL);
}

void CNtlEffectSystemFreeList::Free(const RwInt32 type, void *pObj)
{
	NTL_FUNCTION("CNtlEffectSystemFreeList::Free");

	switch(type)
	{
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE:
		RwFreeListFree(m_pParticleSystemFreeList, (CNtlInstanceParticleSystem*)pObj);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_MESH:
		 RwFreeListFree(m_pMeshSystemFreeList, (CNtlInstanceMeshSystem*)pObj);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM:
		 RwFreeListFree(m_pBeamSystemFreeList, (CNtlInstanceBeamSystem*)pObj);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_HURRICANE:
		 RwFreeListFree(m_pHurricaneSystemFreeList, (CNtlInstanceHurricaneSystem*)pObj);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING:
		 RwFreeListFree(m_pLightningSystemFreeList, (CNtlInstanceLightningSystem*)pObj);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_DECAL:
		 RwFreeListFree(m_pDecalSystemFreeList, (CNtlInstanceDecalSystem*)pObj);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT:
		 RwFreeListFree(m_pPostEffectSystemFreeList, (CNtlInstancePostEffectSystem*)pObj);
		break;
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRACE:
        RwFreeListFree(m_pTraceSystemFreeList, (CNtlInstanceTraceSystem*)pObj);
        break;
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE:
        RwFreeListFree(m_pLineSystemFreeList, (CNtlInstanceLineSystem*)pObj);
        break;    
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRAIL:
        RwFreeListFree(m_pTrailSystemFreeList, (CNtlInstanceTrailSystem*)pObj);
        break;
    case CNtlResourceComponentSystem::RESOURCE_VERTEX_1024:
        RwFreeListFree(m_pVertex1024, (RwIm3DVertex*)pObj);
        break;
    case CNtlResourceComponentSystem::RESOURCE_VERTEX_5120:
        RwFreeListFree(m_pVertex5120, (RwIm3DVertex*)pObj);
        break;
	default:
		NTL_ASSERTE(!"default");
		break;
	}
	NTL_RETURNVOID();
}


CNtlInstanceComponentSystem* CNtlEffectSystemFreeList::CreateSystem(const RwInt32 type)
{
	NTL_FUNCTION("CNtlEffectSystemFreeList::CreateSystem");

	CNtlInstanceComponentSystem* pSystem = NULL;
	switch(type)
	{
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE:
		pSystem = NTL_NEW CNtlInstanceParticleSystem;
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_MESH:
		pSystem = NTL_NEW CNtlInstanceMeshSystem;
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM:
		pSystem = NTL_NEW CNtlInstanceBeamSystem;
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_HURRICANE:
		pSystem = NTL_NEW CNtlInstanceHurricaneSystem;
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING:
		pSystem = NTL_NEW CNtlInstanceLightningSystem;
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_DECAL:
		pSystem = NTL_NEW CNtlInstanceDecalSystem;
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT:
		pSystem = NTL_NEW CNtlInstancePostEffectSystem;
		break;
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRACE:
        pSystem = NTL_NEW CNtlInstanceTraceSystem;
        break;
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE:
        pSystem = NTL_NEW CNtlInstanceLineSystem;
        break;
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRAIL:
        pSystem  = NTL_NEW CNtlInstanceTrailSystem;
        break;
	default:
		NTL_ASSERTE(!"default");
		break;
	}
	
	NTL_RETURN(pSystem);
}

void CNtlEffectSystemFreeList::DeleteSystem(CNtlInstanceComponentSystem* pSystem)
{
	NTL_FUNCTION("CNtlEffectSystemFreeList::DeleteSystem");

	NTL_PRE(pSystem);

	NTL_DELETE( pSystem );

	NTL_RETURNVOID();
}


CNtlPLResource* CNtlEffectSystemFreeList::FindClump(const RwChar* strName)
{
	std::string strPath;
	std::string strTexturePath;
	
	const RwChar* szPath = m_ClumpFileManager.GetPathName(strName);		
	if(szPath)
	{
		strPath = szPath;
		strTexturePath = m_strClumpTexturePath;
	}
	else
	{
		strPath = PATH_EFFECT_RESOURCE;
		strPath += strName;
		
		strTexturePath = PATH_EFFECT_TEXTURE;
	}

	CNtlPLResource* pPLResource = CNtlPLResourceManager::GetInstance()->LoadClump(strPath.c_str(), strTexturePath.c_str());
	if(!pPLResource)
		return NULL;
	
	return pPLResource;
	
}

CNtlPLResource* CNtlEffectSystemFreeList::FindAnimation(const RwChar* strName)
{
	std::string strPath;
	const RwChar* szPathName = m_AnimationFileManager.GetPathName(strName);		
	if(szPathName)
	{
		strPath = szPathName;
	}
	else
	{
		strPath = PATH_EFFECT_RESOURCE;
		strPath += strName;
	}

	CNtlPLResource* pPLResource = CNtlPLResourceManager::GetInstance()->LoadAnimation(strPath.c_str());
	if(!pPLResource)
		return NULL;
	
	return pPLResource;
}

RwTexture* CNtlEffectSystemFreeList::FindTexture(const RwChar* strName)
{
	std::string strPath;
	
	const RwChar* szPathName = m_TextureFileManager.GetPathName(strName);
	if(szPathName)
	{
		strPath = szPathName;
		int nLen = strPath.length() - strlen(strName);
		strPath = strPath.substr(0, nLen);
	}
	else
	{
		strPath = PATH_EFFECT_TEXTURE;
	}	

	RwTexture* pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(strName, strPath.c_str());
	if(!pTexture)
		return NULL;
		
	RwTextureSetFilterMode (pTexture, rwFILTERLINEAR);

	return pTexture;
}
