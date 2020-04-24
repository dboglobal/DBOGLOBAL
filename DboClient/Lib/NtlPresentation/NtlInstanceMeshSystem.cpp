//***********************************************************************************
//	File		:	NtlInstanceEffect.h
//	Desc		:	
//	Begin		:	2005. 7.28
//                  2006. 8.17
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************
#include "precomp_ntlpresentation.h"
#include ".\ntlinstancemeshsystem.h"
#include ".\ntleffectsystemfreelist.h"
#include "NtlPLRenderState.h"
#include "NtlEffectAPI.h"
#include "NtlMath.h"
#include "NtlPLApi.h"
#include "NtlPLResourcePack.h"
#include "NtlPLEntityRenderHelpers.h"
#include "NtlAtomic.h"
#include "NtlMaterialExt.h"
#include "NtlPLCharacter.h"

// 이유는 알수 없으나 rpPRTSTDEMITTERDATAFLAGPRTSIZE 가 정의 되지 않았다고 나와서 할수 없이 include 한다
#include "rpprtstd.h"

void* CNtlInstanceMeshSystem::operator new(size_t size)
{
	NTL_FUNCTION("CNtlInstanceEffect::operator new");
    
	NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_MESH));
}

void CNtlInstanceMeshSystem::operator delete(void *pObj)
{
	CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_MESH, pObj);
}

CNtlInstanceMeshSystem::CNtlInstanceMeshSystem(void)
{
	Init();
}

CNtlInstanceMeshSystem::~CNtlInstanceMeshSystem(void)
{
	Delete();
}

/**
 * MeshSystem 초기화
 */
void CNtlInstanceMeshSystem::Init()
{
	CNtlInstanceComponentSystem::Init();

	m_pClumpResource			= NULL;
	m_pAnimationResource		= NULL;

	m_pUvAnimDict				= NULL;
	m_pAnimationMaterialList	= NULL;

	m_Color.alpha = m_Color.red = m_Color.green = m_Color.blue = 255;

	m_nSizeSeed					= 0;

	m_bDefaultRotate			= FALSE;
	m_pResourceMeshSystem		= NULL;

	m_bUpdateRenderMatrix		= FALSE;
    m_pMatEmitter               = NULL;  
    m_pTrailSystem              = NULL;
    m_pAnimhierarchy            = NULL;    
    m_pToonData                 = NULL;
    m_bFinish                   = FALSE;    

    RwMatrixSetIdentity(&m_matInvScale);

    m_uiMemoryUseSize = sizeof(CNtlInstanceMeshSystem);
}

/**
 * 
 */
void CNtlInstanceMeshSystem::Reset()
{
	Delete();
}

/**
 * 
 */
void CNtlInstanceMeshSystem::Delete()
{
    NTL_DELETE(m_pTrailSystem);
    NTL_DELETE(m_pMatEmitter);

    if(m_pToonData)
    {
        RpToonPaintDestroy(m_pToonData->pToonPaint);
        RpToonInkDestroy(m_pToonData->pToonInk);
        CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pToonData->pTexture);
        NTL_DELETE(m_pToonData);        
    }
    
	if (m_pAnimationMaterialList != NULL)
	{
		rwSListDestroy(m_pAnimationMaterialList);
		m_pAnimationMaterialList = NULL;
	}
	if (m_pClumpResource != NULL)
	{
		if (m_pUvAnimDict == NULL)
		{
			CNtlPLResourceManager::GetInstance()->UnLoad(m_pClumpResource);
			m_pClumpResource = NULL;
		}
		else
		{
			RpClumpDestroy(m_pClumpResource->GetClump());
			NTL_DELETE(m_pClumpResource);
			m_pClumpResource = NULL;
		}
	}

	if (m_pUvAnimDict != NULL)
	{
		RtDictDestroy(m_pUvAnimDict);
		m_pUvAnimDict = NULL;
	}

	if (m_pAnimationResource != NULL)
	{
		CNtlPLResourceManager::GetInstance()->UnLoad(m_pAnimationResource);
		m_pAnimationResource = NULL;
	}

	CNtlInstanceComponentSystem::Delete();
}

/**
 * MeshSystem 생성
 * \param pResourceEffect 리소스
 * \param pResourceComponentSystem 리소스 Container
 */
RwBool CNtlInstanceMeshSystem::Create( CNtlResourceEffect* pResourceEffect, 
									   CNtlResourceComponentSystem* pResourceComponentSystem, 
									   const RwMatrix& matWorld)
{
	CNtlInstanceComponentSystem::Create(pResourceEffect, pResourceComponentSystem, matWorld);

	m_pResourceMeshSystem = (CNtlResourceMeshSystem*) pResourceComponentSystem;

	if (m_pResourceMeshSystem->m_strMeshFileName.empty())
		return FALSE;
	
	if (!m_pResourceMeshSystem->m_strUvFileName.empty())
	{

		std::string strPathName;
		const RwChar* szPath = CNtlEffectSystemFreeList::m_UvAnimationFileManager.GetPathName(m_pResourceMeshSystem->m_strUvFileName.c_str());
		if(szPath)
		{
			strPathName = szPath;
		}
		else
		{
			strPathName = PATH_EFFECT_RESOURCE;
			strPathName += m_pResourceMeshSystem->m_strUvFileName;
		}

		m_pUvAnimDict = UVAnimDictLoad(strPathName.c_str());
		RtDictSchemaSetCurrentDict(RpUVAnimGetDictSchema(), m_pUvAnimDict);
	}

	std::string strPathName;
	std::string strTexturePath;
	if(CNtlEffectSystemFreeList::m_ClumpFileManager.empty())		// Client does not put file information in map (to speed up loading)
	{
		strPathName = PATH_EFFECT_RESOURCE;
		strPathName += m_pResourceMeshSystem->m_strMeshFileName;

		strTexturePath = PATH_EFFECT_TEXTURE;
	}
	else	// Venus puts the file path information in the map because you can freely set the folder.
	{
		const RwChar* szPathName = CNtlEffectSystemFreeList::m_ClumpFileManager.GetPathName(m_pResourceMeshSystem->m_strMeshFileName.c_str());
		if (szPathName == NULL) return FALSE;

		strPathName = szPathName;
		strTexturePath = CNtlEffectSystemFreeList::m_strClumpTexturePath;
	}

	// When we have a Uv animation, we create a Clump as Clone, and the Mesh System that uses the same Clump
	// Uv is applied without setting Uv animation. Currently, the CNtlPLResourceManager does not allow clone generation.
	// Because there is no option, I temporarily hardcoded here.2wq3eaytg
	if (m_pUvAnimDict == NULL)
	{
		m_pClumpResource = CNtlPLResourceManager::GetInstance()->LoadClump(strPathName.c_str(), strTexturePath.c_str());
		if (m_pClumpResource == NULL) 
			return FALSE;
	}
	else
	{
		m_pClumpResource = NTL_NEW CNtlPLResource();
        m_uiMemoryUseSize += sizeof(CNtlPLResource);
		RwImageSetPath(strTexturePath.c_str());
		m_pClumpResource->SetData(API_PL_LoadClump(strPathName.c_str()));
	}

	// If animation data exists, apply Animation to Clump.
	if (!m_pResourceMeshSystem->m_strAniFileName.empty())
	{
		m_pAnimationResource = CNtlEffectSystemFreeList::FindAnimation(m_pResourceMeshSystem->m_strAniFileName.c_str());
		if (m_pAnimationResource != NULL)
		{
			API_SetAnimation(m_pClumpResource->GetClump(), m_pAnimationResource->GetAnimation());
            RpClumpForAllAtomics(m_pClumpResource->GetClump(), API_GetSkinHierarchy, (void *)&m_pAnimhierarchy);
		}		
	}

	if (m_pUvAnimDict != NULL)
	{
		m_pAnimationMaterialList = rwSListCreate(sizeof(RpMaterial*), rwID_NAOBJECT);
		API_AddAnimatedMaterialsList(m_pAnimationMaterialList, m_pClumpResource->GetClump());
	}

	BuildEmitterStandard(&m_pResourceMeshSystem->m_EmitterStandard);
	m_nSizeSeed = rand();
	
	if (m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE) ||
		m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE) ||
		m_bDefaultRotate ||
		m_pSplinePath != NULL)
	{
		m_bUpdateRenderMatrix = TRUE;
	}

    // Toon application
	//CreateToonData(m_pClumpResource->GetClump(), m_pToonData); // did I disable this? by daneos

    // Trail Action
    if(m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))
    {
        m_pTrailSystem = NTL_NEW CNtlInstanceTrailSystem();
        m_pTrailSystem->Create(this, &m_pResourceMeshSystem->m_EmitterTrail);
        m_uiMemoryUseSize += m_pTrailSystem->GetMemoryUseSize();
    }

	return TRUE;
}


/**
 * U,V Animaiont Res Load
 */
RtDict* CNtlInstanceMeshSystem::UVAnimDictLoad(const RwChar* strFullName)
{
	return API_PL_LoadUVAnimDict(strFullName);
}

/**
 * MeshSystem을 Update를 한다.
 * \param fElapsedTime 이전 Frame에서 현재 Frame까지 흐른 시간
 */
RwBool CNtlInstanceMeshSystem::Update(RwReal fElapsedTime)
{
    CNtlInstanceComponentSystem::Update(fElapsedTime);

	m_fLifeTime += fElapsedTime;
	if (m_bStart)
	{
		//Start시에 각 변수 값들을 초기화 한다.
		if (m_pResourceMeshSystem->m_fStartTime < m_fLifeTime )
		{
			m_bStart	= FALSE;
			m_bReady	= TRUE;
			m_bUpdate	= TRUE;
			m_fLifeTime = fElapsedTime;
		}
		else
			return TRUE;
	}

	if ( m_bUpdate && 
		 m_pResourceMeshSystem->m_bEternity == FALSE && 
		 m_fLifeTime >= m_pResourceMeshSystem->m_fLifeTime )
	{
		Stop();
	}

    if (m_bReady && CNtlInstanceEffect::GetRenderMeshSystem())
	{
		RwReal fDeltaTime = m_fLifeTime / m_pResourceMeshSystem->m_fLifeTime;
		fDeltaTime = fmod(fDeltaTime, 1.f);

		if (m_pSplinePath != NULL)
		{
			m_pSplinePath->FrameAnimation(m_fLifeTime, m_matPathWorld, m_pResourceMeshSystem->m_EmitterPrtSplinePath.loop, m_pResourceMeshSystem->m_EmitterPrtSplinePath.useRotate, GetScale());
			m_matPathRender = m_matPathWorld * m_pResourceMeshSystem->m_matPathWorld;
		}

		if (m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
			UpdateSize(fDeltaTime);
		
		if (m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
		{
			UpdateRotate(fDeltaTime);
		}
		else if (m_bDefaultRotate)
		{
			RwMatrixRotate(&m_matRotate, &VEC_X, m_pResourceMeshSystem->m_EmitterStandard.prtRotate.x * fElapsedTime, rwCOMBINEPOSTCONCAT);
			RwMatrixRotate(&m_matRotate, &VEC_Y, m_pResourceMeshSystem->m_EmitterStandard.prtRotate.y * fElapsedTime, rwCOMBINEPOSTCONCAT);
			RwMatrixRotate(&m_matRotate, &VEC_Z, m_pResourceMeshSystem->m_EmitterStandard.prtRotate.z * fElapsedTime, rwCOMBINEPOSTCONCAT);
		}

		if (m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
		{
			UpdateColor(fDeltaTime);
		}

		// Animation
		if (m_pAnimationResource != NULL)
		{
			RpHAnimHierarchyAddAnimTime(m_pAnimhierarchy, fElapsedTime);
			RpHAnimHierarchyUpdateMatrices(m_pAnimhierarchy);
		}
		if (m_pAnimationMaterialList)
		{
			API_MaterialsInterpolatorsAddAnimTime(m_pAnimationMaterialList, fElapsedTime);
			API_MaterialsAnimApply(m_pAnimationMaterialList);
		}


		if (m_bUpdateRenderMatrix)
		{
			SetWorldMatrix(m_matWorld);
		}

        // Trail Aciton
        if(m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))
        {
            if(m_pTrailSystem)
            {
                m_pTrailSystem->Update(fElapsedTime);
            }
        }
	}

	if (!m_bUpdate || m_bFinish)
	{
        if(m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
        {
            m_bReady = UpdateFinish(fElapsedTime);
        }
        else
        {
            m_bReady = FALSE;
        }
	}

    // 바운딩 스피어를 계산한다.
    if(CNtlResourceEffect::m_bUpdateBoundingSphere)
    {
        CalcBoundingSphere();
    }

	return m_bReady;
}

//------------------------------------------------------------------
//	FuncName	: Render
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceMeshSystem::Render()
{
    if(!CNtlInstanceEffect::GetRenderMeshSystem())
        return TRUE;

	if (!m_bReady || !m_bVisible) return TRUE;

	BeginEffectMeshSystemState(m_pResourceMeshSystem->m_bAlphaBlend, m_pResourceMeshSystem->m_nSrcBlend, m_pResourceMeshSystem->m_nDestBlend, m_pResourceMeshSystem->m_EmitterStandard.bWriteEnable);	

	RpClumpForAllAtomics(m_pClumpResource->GetClump(), API_MeshColorUpdate, (void*)&m_Color);
	RpClumpRender(m_pClumpResource->GetClump());
	RpClumpForAllAtomics(m_pClumpResource->GetClump(), API_MeshColorUpdate, (void*)&RGBA_BASE);

	EndEffectMeshSystemState(m_pResourceMeshSystem->m_bAlphaBlend);

    // Trail Action Render
    if(m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))
    {
        if(m_pTrailSystem)
        {
            m_pTrailSystem->Render();
        }
    }

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: SetWorldMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceMeshSystem::SetWorldMatrix(const RwMatrix& matWorld)
{
    RwV3d vAt = matWorld.at;

    if(!m_pResourceMeshSystem || !m_pResourceMeshSystem->IsEmitterDataFlag(NTLrpPRTADVEMITTERDATAFLAGFOLLOW) || 
        (vAt.x == 0.0f && 
         vAt.y == 0.0f &&
         vAt.z == 1.0f))       
    {
	    CNtlInstanceComponentSystem::SetWorldMatrix(matWorld);
    }
    else
    {
        if(!m_pMatEmitter)        
        {
            m_pMatEmitter = NTL_NEW RwMatrix;
            m_uiMemoryUseSize += sizeof(RwMatrix);
            *m_pMatEmitter = matWorld;
        }

        CNtlInstanceComponentSystem::SetWorldMatrix(*m_pMatEmitter);
    }
	
	RwV3d vPosition = *RwMatrixGetPos(&m_matRender);
	*RwMatrixGetPos(&m_matRender) = ZeroAxis;	

    if (m_pSplinePath != NULL)
	{
		// Spline 행렬은 회전이 적용되어 있지 않다. WorldMatrix의 회전을 Spline 행렬에 적용한다.
		RwMatrix tempMatrix = matWorld;
		*RwMatrixGetPos(&tempMatrix) = ZeroAxis;

		m_matPathRender = m_matPathRender * tempMatrix;
		vPosition += *RwMatrixGetPos(&m_matPathRender);

        if(m_pResourceMeshSystem && m_pResourceMeshSystem->m_EmitterPrtSplinePath.useRotate)
        {
            *RwMatrixGetPos(&m_matPathRender) = ZeroAxis;
            m_matRender = m_matPathRender * m_matRender;
        }
	}

    // Rotate Action 이 있거나 Default Rotate 값이 있으면.
    if ( m_pResourceMeshSystem && 
        m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE) || 
        m_bDefaultRotate)
    {
        m_matRender = m_matRotate * m_matRender;
    }

    RwMatrixTranslate(&m_matRender, &vPosition, rwCOMBINEPOSTCONCAT);

    if (m_pClumpResource != NULL)
    {
        RwFrame* pFrame = RpClumpGetFrame(m_pClumpResource->GetClump());        
        RwFrameTransform(pFrame, &m_matRender, rwCOMBINEREPLACE);
    }
}

//------------------------------------------------------------------
//	FuncName	: BuildEmitterStandard
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceMeshSystem::BuildEmitterStandard(SNtlMeshEmitterStandard* pEmitterStandard)
{
	if (!m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		RwMatrixScale(&m_matScale, &pEmitterStandard->prtSize, rwCOMBINEREPLACE);        
		SetWorldMatrix(m_matWorld);
        m_v3dFinishSize = pEmitterStandard->prtSize;
	}
	if (!m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
	{
		if ( pEmitterStandard->prtRotate.x != 0.f || 
			 pEmitterStandard->prtRotate.y != 0.f || 
			 pEmitterStandard->prtRotate.z != 0.f)
		{
			m_bDefaultRotate = TRUE;
		}
	}
	if (!m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
	{
		m_Color = pEmitterStandard->prtColor;
	}
}


//------------------------------------------------------------------
//	FuncName	: UpdateSize
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceMeshSystem::UpdateSize(RwReal fDeltaTime)
{
	RwV3d vSize;
	if (m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		RebuildMultiSize( vSize, 
			              m_nSizeSeed, fDeltaTime, 
						  &m_pResourceMeshSystem->m_EmitterPrtSize, 
						  &m_pResourceMeshSystem->m_EmitterPrtMultiSize);
	}
	else
		RebuildSize(vSize, m_nSizeSeed, fDeltaTime, &m_pResourceMeshSystem->m_EmitterPrtSize);

    vSize *= m_fScale;
	RwMatrixScale(&m_matScale, &vSize, rwCOMBINEREPLACE);
}

//------------------------------------------------------------------
//	FuncName	: UpdateColor
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceMeshSystem::UpdateColor(RwReal fDeltaTime)
{
	if (m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
	{
		RebuildMultiColor(m_Color, m_nSizeSeed, fDeltaTime, &m_pResourceMeshSystem->m_EmitterPrtCol, &m_pResourceMeshSystem->m_EmitterPrtMultiCol);
	}
	else
	{
		RebuildColor(m_Color, m_nSizeSeed, fDeltaTime, &m_pResourceMeshSystem->m_EmitterPrtCol);
	}
//	RpClumpForAllAtomics(m_pClumpResource->GetClump(), EffectMeshColorUpdate, (void*)&color);
}

//------------------------------------------------------------------
//	FuncName	: UpdateSize
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceMeshSystem::UpdateRotate(RwReal fDeltaTime)
{
	RwV3d vRotate;
	if (m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTIROTATE))
	{
		RebuildMultiRotate(vRotate, m_nSizeSeed, fDeltaTime, &m_pResourceMeshSystem->m_EmitterPrtRotate, &m_pResourceMeshSystem->m_EmitterPrtMultiRotate);
	}
	else
	{
		RebuildRotate(vRotate, m_nSizeSeed, fDeltaTime, &m_pResourceMeshSystem->m_EmitterPrtRotate);
	}

	RwMatrixRotate(&m_matRotate, &VEC_X, vRotate.x, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&m_matRotate, &VEC_Y, vRotate.y, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&m_matRotate, &VEC_Z, vRotate.z, rwCOMBINEPOSTCONCAT);

//	SetWorldMatrix(m_matWorld);
}

RwBool CNtlInstanceMeshSystem::UpdateFinish( RwReal fElapsedTime ) 
{
    if(m_pResourceMeshSystem->m_EmitterPrtFinish.m_nFinishTypeFlag & FINISH_TYPE_ALPHA)
    {
        if(m_Color.alpha > 0)
        {
            RwUInt8 uiDeltaAlpha = UpdateFinishAlpha(m_pResourceMeshSystem->m_EmitterStandard.prtColor.alpha, fElapsedTime, m_pResourceMeshSystem->m_EmitterPrtFinish.m_fFinishTime);
            
            // UINT의 특성상 음수가 되면 다시 MAX값이 되버리기 때문에 음수 체크를 한다.
            if((RwInt32)m_Color.alpha - (RwInt32)uiDeltaAlpha < 0)
            {
                m_Color.alpha = 0;
            }
            else
            {
                m_Color.alpha -= uiDeltaAlpha;
            }                            
        }
    }

    if(m_pResourceMeshSystem->m_EmitterPrtFinish.m_nFinishTypeFlag & FINISH_TYPE_SIZE)
    {
        // 크기를 점점 줄인다.
        m_bUpdateRenderMatrix = TRUE;

        RwReal fDeltaSize = fElapsedTime / m_pResourceMeshSystem->m_EmitterPrtFinish.m_fFinishTime;

        m_v3dFinishSize -= m_pResourceMeshSystem->m_EmitterStandard.prtSize * fDeltaSize;

        RwMatrixScale(&m_matScale, &m_v3dFinishSize, rwCOMBINEREPLACE);                    

    }

    m_fFinishTime += fElapsedTime;

    if(m_fFinishTime >= m_pResourceMeshSystem->m_EmitterPrtFinish.m_fFinishTime)
    {
        return FALSE;
    }
    
    return TRUE;
}

RwBool CNtlInstanceMeshSystem::Finish() 
{
	CNtlInstanceComponentSystem::Finish();

	if(m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
		return TRUE;

	return FALSE;
}

void CNtlInstanceMeshSystem::SetAlpha( RwUInt8 byValue ) 
{
	m_Color.alpha = byValue;
}

void CNtlInstanceMeshSystem::SetScale( RwReal fScale ) 
{
    CNtlInstanceComponentSystem::SetScale(fScale);

    RwV3d vScale = UnitAxis;
    vScale = m_pResourceMeshSystem->m_EmitterStandard.prtSize * m_fScale;
    if (!m_pResourceMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
    {
        int a = 0;
    }

    RwMatrixScale(&m_matScale, &vScale, rwCOMBINEREPLACE);            
}

void CNtlInstanceMeshSystem::CalcBoundingSphere() 
{
    RwSphere spBounding;    
    Helper_ClumpCalcBoundingSphere(m_pClumpResource->GetClump(), &spBounding);    
    
    RwV3d vLength = m_matRender.pos - spBounding.center;
    RwReal fLength = RwV3dLength(&vLength);
    fLength += spBounding.radius;

    m_pResourceMeshSystem->SetBoundingSphereRadius(fLength);
}

void CNtlInstanceMeshSystem::CreateToonData(RpClump* pClump, SToonData* pToonData)
{
    // 카툰이 적용되어 있으면 적용한다.
    if(!Helper_IsToonClump(pClump))
        return;

    ENTITY_ATOMIC_VEC vecAtomic;
    Helper_SetClumpAllAtomics(pClump, &vecAtomic);        
    for each(SEntityAtomicData* pAtomic in vecAtomic)
    {
        Helper_SetAtomicAllMaterialSkinInfo(pAtomic->pData);
        RpGeometry* pGeom = RpAtomicGetGeometry(pAtomic->pData);
        RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);
        for(int i = 0; i < nMatCount; ++i)
        {
            RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
            if(pMaterial)
            {
                RwRGBA	sColor;
                sColor.red		= 255;
                sColor.green	= 255;
                sColor.blue		= 255;
                sColor.alpha	= 255;
                RpNtlMaterialExtSetSkinColor(pMaterial, &sColor);

                // get through the proper renderpipe
                RpNtlMaterialSetRenderCB(pMaterial, CNtlPLCharacter::fpRenderCB);
            }
        }
    }

    RpAtomic* pAtomic = NULL;
    RpToonGeo *pToonGeo = NULL;

    if(!pToonData)
    {
        pToonData = NTL_NEW SToonData();        
        pToonData->pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture("smooth.png", "texture/toon/");
        pToonData->pToonInk = RpToonInkCreate();
        RpToonInkSetOverallThickness(pToonData->pToonInk, DEFAULT_INK_THICKNESS);
        RwRGBA colInk;
        colInk.red = colInk.green = colInk.blue = 0;
        colInk.alpha = 255;
        RpToonInkSetColor(pToonData->pToonInk, colInk);
        RpToonInkSetName(pToonData->pToonInk, "silhouette");
        pToonData->pToonPaint = RpToonPaintCreate();
        RpToonPaintSetGradientTexture(pToonData->pToonPaint, pToonData->pTexture);
    }
    
    Helper_SetToonClump(pClump, pToonData);    
}

