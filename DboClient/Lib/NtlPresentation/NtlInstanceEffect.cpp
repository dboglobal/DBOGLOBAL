//***********************************************************************************
//	File		:	NtlInstanceEffect.h
//	Desc		:	
//	Begin		:	2005. 7.28
//                  2006. 8.17
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//                  HoDong
//	Update		:	
//***********************************************************************************
#include "precomp_ntlpresentation.h"
#include ".\ntlinstanceeffect.h"
#include "NtlWorldInterface.h"
#include "NtlInstanceParticleSystem.h"
#include "NtlInstanceMeshSystem.h"
#include "NtlInstanceBeamSystem.h"
#include "NtlPLEntityFreeList.h"
#include "NtlEffectSystemFreeList.h"
#include "NtlAtomic.h"
#include "NtlPLEntityRenderHelpers.h"
#include "NtlPLWorldEntity.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RWWorld.h"

#include "NtlPLCullingScheduling.h"

#define EFFECT_NOT_UPDATE_TIME_MAX 0.5f                     ///< Not Visible이거나 컬링될때 업데이트 주기

RwBool CNtlInstanceEffect::m_bLowSpec = FALSE;
RwBool CNtlInstanceEffect::m_bRenderMeshSystem = TRUE;
RwBool CNtlInstanceEffect::m_bRenderDecalSystem = TRUE;
RwReal CNtlInstanceEffect::m_fLowSpecRatio = 0.5f;

void* CNtlInstanceEffect::operator new(size_t size)
{
	NTL_FUNCTION("CNtlInstanceEffect::operator new");
    
	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_EFFECT));
}

void CNtlInstanceEffect::operator delete(void *pObj)
{
	CNtlPLEntityFreeList::Free(PLENTITY_EFFECT, pObj);
}

CNtlInstanceEffect::CNtlInstanceEffect(void)
{
	SetLayer(PLENTITY_LAYER_EFFECT);
	SetMinimapLayer(NTL_PLEFLAG_MINIMAP_LAYER_ONE);
	SetClassType(PLENTITY_EFFECT);

	NTL_ASSERTE(m_svSystem.empty());

	m_pResourceEffect	= NULL;
	m_fLifeTime			= 0.f;

	m_bAutoDelete		= TRUE;

	m_fAnimSpeed        = 1.f;

	RwMatrixSetIdentity(&m_matWorld);

	m_vModelScale.x = 1.f;
	m_vModelScale.y = 1.f;
	m_vModelScale.z = 1.f;

	m_vModelAngle.x = 0.f;
	m_vModelAngle.y = 0.f;
	m_vModelAngle.z = 0.f;

	m_vWorldPosition.x = 0.f;
	m_vWorldPosition.y = 0.f;
	m_vWorldPosition.z = 0.f;

	m_pVisibleBVClump = NULL;		
    m_bApplyScale = TRUE;
    m_bIsCulling = FALSE;
    m_fNotUpdateTime = 0.0f;    

    ZeroMemory(&m_BoundingSphere, sizeof(m_BoundingSphere));

#ifndef dNTL_WORLD_CULLING_NEW
	GetNtlPLCullingScheduler()->AddCulling(this);
#endif
}

CNtlInstanceEffect::~CNtlInstanceEffect(void)
{
#ifndef dNTL_WORLD_CULLING_NEW
	GetNtlPLCullingScheduler()->RemoveCulling(GetClassType(), this);
#endif
}

/**
 * Effect 생성
 */
RwBool CNtlInstanceEffect::Create(const SPLEntityCreateParam *pParam)
{
	//SetVisibleBoundingVolume(true);
	return TRUE;
}

/**
 * Effect Destroy
 */
void CNtlInstanceEffect::Destroy()
{
    m_PLSound.Destroy();

	svdef_InstanceComponentSystem::iterator it;
	for (it = m_svSystem.begin(); it != m_svSystem.end(); ++ it)
	{
		CNtlEffectSystemFreeList::DeleteSystem(*it);
	}
	m_svSystem.clear();

	//m_pVisibleBVClump
	if(m_pVisibleBVClump != NULL)
	{
		RpWorldRemoveClump(CNtlPLGlobal::m_pRpWorld, m_pVisibleBVClump->GetClump());
		CNtlPLResourceManager::GetInstance()->UnLoad(m_pVisibleBVClump);
	}
}

/**
 * Property를 Effect에 적용을 한다.
 * \param pData Property Data
 */
RwBool CNtlInstanceEffect::SetProperty(const CNtlPLProperty *pData)
{
	CNtlResourceEffect* pResourceEffect = (CNtlResourceEffect*)pData;

	m_pResourceEffect	= pResourceEffect;
	m_fLifeTime			= 0.f;

	m_bReady			= TRUE;
	m_bShow				= TRUE;
	
	CreateSystem(pResourceEffect, m_matWorld);

	m_bAutoDelete = pResourceEffect->IsAutoDelete();

    // Sound를 Play한다.
	if(strlen(pResourceEffect->m_EffectSound.chSoundName) > 0)
	{
        RwInt32 nCnt = 1;
        std::string strSoundFileName;

        if(strlen(pResourceEffect->m_EffectSound.chSoundName4) > 0)
            nCnt = 4;
        else if(strlen(pResourceEffect->m_EffectSound.chSoundName3) > 0)
            nCnt = 3;
        else if(strlen(pResourceEffect->m_EffectSound.chSoundName2) > 0)
            nCnt = 2;

        switch(NtlRandomNumber(1, nCnt))
        {
        case 1:
            strSoundFileName = pResourceEffect->m_EffectSound.chSoundName;
            break;
        case 2:
            strSoundFileName = pResourceEffect->m_EffectSound.chSoundName2;
            break;
        case 3:
            strSoundFileName = pResourceEffect->m_EffectSound.chSoundName3;
            break;
        case 4:
            strSoundFileName = pResourceEffect->m_EffectSound.chSoundName4;
            break;
        }

        RwReal fSoundPitch = CNtlMath::Interpolation(pResourceEffect->m_EffectSound.fSoundPitchMin, pResourceEffect->m_EffectSound.fSoundPitchMax, NtlRandomNumber(0.0f, 1.0f));

		m_PLSound.PlayEffectSound((RwChar*)strSoundFileName.c_str(), 
                                  pResourceEffect->m_EffectSound.bLoop, 
                                  (RwInt32)pResourceEffect->m_EffectSound.fSoundVolume,
                                  pResourceEffect->m_EffectSound.fSoundDist,
                                  pResourceEffect->m_EffectSound.fSoundDecayDist,
                                  fSoundPitch);
	}

    // Bounding sphere calculation
    CalcBoundingSphere();

	return TRUE;
}

/**
 * Effect를 생성을 한다.
 * \param matRender: Effect Local Matrix
 */
void CNtlInstanceEffect::CreateSystem(CNtlResourceEffect* pResourceEffect, const RwMatrix& matRender)
{
    if(!pResourceEffect)
        return;

	if (!pResourceEffect->m_svResourceSystem.empty())
	{
		CNtlResourceEffect::svdef_ResourceComponentSystem::iterator it;

		for ( it = pResourceEffect->m_svResourceSystem.begin(); it != pResourceEffect->m_svResourceSystem.end(); ++it)
		{
			CNtlResourceComponentSystem* pResourceComponentSystem = (*it);

			if (!pResourceComponentSystem->m_bSubSystem)
			{
				CNtlInstanceComponentSystem* pInstanceComponentSystem = CNtlEffectSystemFreeList::CreateSystem(pResourceComponentSystem->GetSystemType());

				NTL_ASSERTE(pInstanceComponentSystem);

				if (!pInstanceComponentSystem->Create(pResourceEffect, pResourceComponentSystem, matRender))
				{
					pInstanceComponentSystem->Reset();
					CNtlEffectSystemFreeList::DeleteSystem(pInstanceComponentSystem);
					continue;
				}

				m_svSystem.push_back(pInstanceComponentSystem);
			}
		}

		{
			// FadeOut System 연결 하기
			svdef_InstanceComponentSystem::iterator it;
			for (it = m_svSystem.begin(); it != m_svSystem.end(); ++it)
			{
				CNtlInstanceComponentSystem* pSystem = (*it);
				if (pSystem->m_pResourceComponentSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFADEOUT))
				{
					svdef_InstanceComponentSystem::iterator jt;
					for (jt = m_svSystem.begin(); jt != m_svSystem.end(); ++jt)
					{
						CNtlInstanceComponentSystem* pSrcSystem = (*jt);
						if (_stricmp(pSystem->m_pResourceComponentSystem->m_EmitterPrtFadeOut.strFollowSystem, pSrcSystem->m_pResourceComponentSystem->GetSystemName()) == 0)
						{
							pSystem->m_pFadeOutSystem = pSrcSystem;
							break;
						}
					}
				}

				if (pSystem->GetSystemType() == CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM)
				{
					CNtlInstanceBeamSystem* pBeamSystem = (CNtlInstanceBeamSystem*)pSystem;
					if (strlen(pBeamSystem->m_pResourceBeamSystem->m_EmitterStandard.strHeadSystem) > 0)
					{
						svdef_InstanceComponentSystem::iterator jt;
						for (jt = m_svSystem.begin(); jt != m_svSystem.end(); ++jt)
						{
							CNtlInstanceComponentSystem* pSrcSystem = (*jt);
							if (_stricmp(pBeamSystem->m_pResourceBeamSystem->m_EmitterStandard.strHeadSystem, pSrcSystem->m_pResourceComponentSystem->GetSystemName()) == 0)
							{
								pBeamSystem->m_pHeadSystem = pSrcSystem;
								break;
							}
						}
					}
				}
			}
		}
	}
}
/**
* Update the effect.
* \ param fElapsedTime The time from the previous frame to the current time
 */
RwBool CNtlInstanceEffect::Update(RwReal fElapsedTime)
{
    m_fLifeTime += fElapsedTime;

	std::string strname = GetName();

	if(m_bShow == FALSE || IsCulling())
    {
        // NOTE: If it is not visible or culled, it should be updated regularly.
        m_fNotUpdateTime += fElapsedTime;
        if(m_fNotUpdateTime < EFFECT_NOT_UPDATE_TIME_MAX)
        {
            return TRUE;            
        }
        else
        {
            fElapsedTime = m_fNotUpdateTime;
            m_fNotUpdateTime = 0.0f;
        }
    }

	RwBool bRunning = FALSE;
	svdef_InstanceComponentSystem::iterator it;
	for (it = m_svSystem.begin(); it != m_svSystem.end(); ++ it)
	{
        // The ComponentSystem should return False, but the Effect returns False.
		bRunning |= (*it)->Update(fElapsedTime * m_fAnimSpeed);
	}

	return bRunning;
}

RwBool CNtlInstanceEffect::RenderToTexture(void)
{
	Render();

	return CNtlPLAttach::RenderToTexture();
}

/**
 * Rendering the effect.
 */
RwBool CNtlInstanceEffect::Render()
{
	if (!m_pResourceEffect)
	{
		return TRUE;
	}

	if (!m_bShow || m_bIsCulling)
	{
		return TRUE;
	}

#ifdef dNTL_WORLD_CULLING_NEW
	RwReal fVisibleDistance = GetVisibleCullingDistance();
	if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST))
	{
		fVisibleDistance = 512.0f;
	}
	if (RpNtlCameraFrustumTestSphere(RwCameraGetCurrentCamera(), &m_BoundingSphere, &fVisibleDistance) == rwSPHEREOUTSIDE)   
	{
		return TRUE;
	}
#endif

	svdef_InstanceComponentSystem::iterator it;
	for (it = m_svSystem.begin(); it != m_svSystem.end(); ++ it)
	{

		if ((m_pResourceEffect && (*it)->m_pResourceComponentSystem->m_bShow) || !m_pResourceEffect)
		{
			(*it)->Render();
		}
	}

    // Test
	//RenderBoundingSphere();

	return TRUE;
}

/**
 * Effect World에 위치 시킨다.
 * Matrix가 적용 되고 Position이 넘어오는 경우가 있다.
 * Matrix에서 Positon은 크게 문제가 없으나 Scale 하고 Rotation
 * Matirx와 임시 변수(Scale, Angle)과 어떻게 동기화를 시킬까?
 * \param matWorld Effect를 World에 위치 시키는 Model Matrix
 */
void  CNtlInstanceEffect::SetMatrix(RwMatrix& matWorld)
{
	RwMatrixCopy(&m_matWorld, &matWorld);

	// NOTE: Attached Mesh Systems receive a matrix of scaled bones for each update,
	// False, we need to convert it to the original scale.
    if(!m_bApplyScale)
    {
        RwMatrixScale(&m_matWorld, &CNtlMath::RwV3dInverse(m_vModelScale), rwCOMBINEPRECONCAT);
    }

	svdef_InstanceComponentSystem::iterator it;
	for (it = m_svSystem.begin(); it != m_svSystem.end(); ++ it)
	{
		(*it)->SetWorldMatrix(m_matWorld);
	}

	m_vWorldPosition = m_matWorld.pos;
    m_BoundingSphere.center = m_vWorldPosition;
    m_PLSound.SetPosition(&m_vWorldPosition);
}

RwMatrix &CNtlInstanceEffect::GetMatrix()
{
	return m_matWorld;
}

/**
 * ModelScale, ModelAngle, WorldPostion으로 Matrix을 구한다.
 */
void CNtlInstanceEffect::SetTransform()
{
	RwMatrixScale( &m_matWorld, &m_vModelScale, rwCOMBINEREPLACE );
	
	RwMatrixRotate( &m_matWorld, &CNtlPLGlobal::m_vXAxisV3, m_vModelAngle.x, rwCOMBINEPOSTCONCAT );
	RwMatrixRotate( &m_matWorld, &CNtlPLGlobal::m_vYAxisV3, m_vModelAngle.y, rwCOMBINEPOSTCONCAT );
	RwMatrixRotate( &m_matWorld, &CNtlPLGlobal::m_vZAxisV3, m_vModelAngle.z, rwCOMBINEPOSTCONCAT );

	RwMatrixTranslate( &m_matWorld, &m_vWorldPosition, rwCOMBINEPOSTCONCAT );

	svdef_InstanceComponentSystem::iterator it;
	for (it = m_svSystem.begin(); it != m_svSystem.end(); ++ it)
	{
		(*it)->SetWorldMatrix(m_matWorld);
	}
}

/**
 * Effect World에 위치 시킨다. m_matWorld에 위치 좌표를 넣는다.
 * 그리고 임시 변수인 m_vWorldPosition도 값을 갱신 시킨다.
 * \param pPos Effect가 위치 할 World의 좌표
 */
 void CNtlInstanceEffect::SetPosition(const RwV3d *pPos)
{
	RwV3d *pvOldPos = RwMatrixGetPos(&m_matWorld);
	RwV3d vSubDist;
	RwV3dSubMacro(&vSubDist, pPos, pvOldPos);
	RwMatrixTranslate(&m_matWorld, &vSubDist, rwCOMBINEPOSTCONCAT);
	
	svdef_InstanceComponentSystem::iterator it;
	for (it = m_svSystem.begin(); it != m_svSystem.end(); ++ it)
	{
		(*it)->SetWorldMatrix(m_matWorld);
	}


	m_vWorldPosition = *pPos;
	
    //Visible Bounding Volume
	if(m_pVisibleBVClump)
	{
		RpClump *pClump = m_pVisibleBVClump->GetClump();

		RwFrame *pFrame = RpClumpGetFrame(pClump);

		RwFrameTranslate (pFrame, &m_vWorldPosition, rwCOMBINEREPLACE);
	}

    m_PLSound.SetPosition(pPos);

    // Bounding Sphere
    m_BoundingSphere.center = m_vWorldPosition;
}

/**
 * Effect의 World에서의 위치를 얻는다.
 * \return RwV3d World에서의 위치
 */
RwV3d CNtlInstanceEffect::GetPosition(void)
{ 
	return m_matWorld.pos;
}

/**
 * Effect를 Model 공간에서 회전을 시킨다.
 * \param fXAngle X축 각도, Y축 각도, Z축 각도 모두(Degree -360 - 360)
 */
void CNtlInstanceEffect::SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle)
{
	m_vModelAngle.x = fXAngle;
	m_vModelAngle.y = fYAngle;	
	m_vModelAngle.z = fZAngle;

	SetTransform();

	//Visible Bounding Volume
	if(m_pVisibleBVClump)
	{
		RpClump*	pClump	= m_pVisibleBVClump->GetClump();
		RwFrame*	pFrame	= RpClumpGetFrame(pClump);
		RwMatrix*	pMatrix	= RwFrameGetMatrix(pFrame);
		RwMatrix    Matrix;

		RwMatrixSetIdentity(&Matrix);
		RwMatrixRotate(&Matrix, &CNtlPLGlobal::m_vYAxisV3, m_vModelAngle.y, rwCOMBINEREPLACE);
		RwMatrixTranslate(&Matrix, &m_vWorldPosition, rwCOMBINEPOSTCONCAT);

		*pMatrix = Matrix;
		RwFrameUpdateObjects(pFrame);
	}
}

/**
 * Effect를 Model 공간에서의 회전값을 얻는다.
 * \return RwV3d 각축에 대한 회전값(Degree -360 - 360)
 */
const RwV3d* CNtlInstanceEffect::GetRotate(void) const
{
	return &m_vModelAngle;
}

/**
 * Effect의 Model 공간에서의 Scale을 적용한다.
 */
void CNtlInstanceEffect::SetScale( RwReal fScale)
{
	m_vModelScale.x = fScale;
	m_vModelScale.y = fScale;
	m_vModelScale.z = fScale;

    m_BoundingSphere.radius *= fScale;

    if(m_bApplyScale)
    {
	    svdef_InstanceComponentSystem::iterator it;
	    for (it = m_svSystem.begin(); it != m_svSystem.end(); ++ it)
	    {
		    // Sets the scale of each effect system, which is overridden only in the Particle System.
			// Other effect systems can be scaled only by setting the Matrix of SetTransform (). (2007.7.19 by agebreak)
		    (*it)->SetScale(fScale);
	    }
    }
}

/**
 * Effect의 Model 공간에서의 Scale 값을 얻는다.
 */
RwReal CNtlInstanceEffect::GetScale( void )
{
	return m_vModelScale.x;
}

/**
 * Effect AnimSpeed를 조정한다.
 */
RwBool CNtlInstanceEffect::SetPlayAnimSpeed(RwReal fAnimSpeed)
{
	if(fAnimSpeed < 0.f)
		return FALSE;
	m_fAnimSpeed = fAnimSpeed;

	return TRUE;
}

void CNtlInstanceEffect::SetVisibleBoundingVolume(RwBool bVisible)
{
	if(bVisible)
	{
		if(m_pVisibleBVClump == NULL)
		{
			m_pVisibleBVClump = CNtlPLResourceManager::GetInstance()->LoadClump("Tool/SABER/Mesh/Effect_Dummy.dff", NULL);
			if(m_pVisibleBVClump)
			{
				RpNtlClumpSetData( m_pVisibleBVClump->GetClump(), this );
				RpWorldAddClump(CNtlPLGlobal::m_pRpWorld, m_pVisibleBVClump->GetClump());


				if(m_pVisibleBVClump)
				{
					RpClump *pClump = m_pVisibleBVClump->GetClump();

					RwFrame *pFrame = RpClumpGetFrame(pClump);

					RwFrameTranslate (pFrame, &m_vWorldPosition, rwCOMBINEREPLACE);
				}

			}
		}
	}
	else
	{
		if(m_pVisibleBVClump)
		{
			RpWorldRemoveClump(CNtlPLGlobal::m_pRpWorld, m_pVisibleBVClump->GetClump());
			CNtlPLResourceManager::GetInstance()->UnLoad(m_pVisibleBVClump);
			m_pVisibleBVClump = NULL;
        }
	}
}

void CNtlInstanceEffect::SetAlpha( RwUInt8 byValue )
{
    svdef_InstanceComponentSystem::iterator it;
    for (it = m_svSystem.begin(); it != m_svSystem.end(); ++ it)
    {
       (*it)->SetAlpha(byValue);
    }    
}

void CNtlInstanceEffect::AddComponentSystem( CNtlInstanceComponentSystem* pComponentSystem ) 
{
    if(!pComponentSystem)
        return;

    m_svSystem.push_back(pComponentSystem);
}

void CNtlInstanceEffect::Finish() 
{
    // 내부적으로는 ComponentSystem들의 Stop을 호출한다.	

    svdef_InstanceComponentSystem::iterator it;
    for (it = m_svSystem.begin(); it != m_svSystem.end(); ++it)
    {
		// 모두 False를 반환하면 설정된 Finish 동작이 없다는 것이다.
        (*it)->Finish();
    }
}

void CNtlInstanceEffect::SetVisible( RwBool bVisible ) 
{
    m_bShow = bVisible;
}

// 스케일 적용 유무를 설정한다
void CNtlInstanceEffect::SetApplyScale( RwBool bApply ) 
{
    m_bApplyScale = bApply;
}

void CNtlInstanceEffect::SetPostEffect_CenterFixEnable( RwBool bEnable ) 
{
    for each(CNtlInstanceComponentSystem* pSystem in m_svSystem)
    {
        if(pSystem && pSystem->GetSystemType() == CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT)
        {
            CNtlInstancePostEffectSystem* pPostEffect = (CNtlInstancePostEffectSystem*)pSystem;
            pPostEffect->SetCenterFixEnable(bEnable);
        }
    }
}

void CNtlInstanceEffect::SetAttachParent( CNtlPLAttach *pPLAttachEntity ) 
{
    for each(CNtlInstanceComponentSystem* pSystem in m_svSystem)
    {
        if(pSystem)
        {
            pSystem->m_pAttachParent = pPLAttachEntity;
        }
    }

    CNtlPLAttach::SetAttachParent(pPLAttachEntity);
}

void CNtlInstanceEffect::RenderBoundingSphere() 
{
    CalcBoundingSphere();
    Helper_ClumpWorldBoundingSphereRender(&m_BoundingSphere);
}

void CNtlInstanceEffect::CalcBoundingSphere() 
{
    m_BoundingSphere.center = GetPosition();

    if(m_pResourceEffect->m_EffectBoundingSphere.bDisableAuto)
    {
        // 정의된 바운딩 스피어 사용
        m_BoundingSphere.radius = m_pResourceEffect->m_EffectBoundingSphere.fRadius;
    }
    else
    {
        // Use auto-generated bounding sphere
        RwSphere* pBoundingSphere = NULL;        
        RwV3d vLength = ZeroAxis;    

        for each(CNtlInstanceComponentSystem* pSystem in m_svSystem)
        {
            pBoundingSphere = pSystem->GetBoundingSphere();
            if(!pBoundingSphere)
                continue;

            vLength = m_BoundingSphere.center - pBoundingSphere->center;
            m_BoundingSphere.radius = RwV3dLength(&vLength);
            m_BoundingSphere.radius += pBoundingSphere->radius;        
        }
    }
}

#ifndef dNTL_WORLD_CULLING_NEW
RwBool CNtlInstanceEffect::CullingTest(RwCamera* pCamera)
{
	CNtlPLCullingScheduling *pCullScheduling = GetNtlPLCullingScheduler();
	if(pCullScheduling)
	{
		pCullScheduling->IncAtomicOperator(PLENTITY_EFFECT);
	}

#ifdef dNTL_WORLD_TOOL_MODE
	if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST))
	{
		if (RwCameraFrustumTestSphere(pCamera, &m_BoundingSphere) == rwSPHEREOUTSIDE)
		{
			m_bIsCulling = TRUE;
		}
		else
		{
			m_bIsCulling = FALSE;
		}
		return m_bIsCulling;
	}
#endif
	if (RpNtlCameraFrustumTestSphere(pCamera, &m_BoundingSphere, &m_fVisibleCullingDistance) == rwSPHEREOUTSIDE)   
    {
        m_bIsCulling = TRUE;
    }
    else
    {
        m_bIsCulling = FALSE;
    }

    return m_bIsCulling;
}
#endif

RwBool CNtlInstanceEffect::IsVisible( RwUInt32 uiFlags /*= 0xFFFFFFFF*/ ) 
{
    RwBool bReturn =  __super::IsVisible(uiFlags);

    if(uiFlags & NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT)
    {
        bReturn &= !IsCulling();
    }

    return bReturn;
}

RwUInt32 CNtlInstanceEffect::GetMemoryUseSize() 
{
    RwUInt32 uiMemoryUseSize = sizeof(CNtlInstanceEffect);

    for each(CNtlInstanceComponentSystem* pSystem in m_svSystem)
    {
        if(pSystem)
        {
            uiMemoryUseSize += pSystem->GetMemoryUseSize();
        }        
    }

    return uiMemoryUseSize;
}

#ifdef dNTL_WORLD_TOOL_MODE

void CNtlInstanceEffect::SaveSwapFile(RwReal x, RwReal y, RwReal z)
{
	RwV3d	vSectorPos;

	vSectorPos.x = x;
	vSectorPos.y = y;
	vSectorPos.z = z;

	RwInt32	iSectorIdx = GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectorIdx(vSectorPos);
	if (iSectorIdx != -1)
	{
		CNtlWorldSector* pSector = &GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_pSectors[iSectorIdx];

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		if(_chdir("swap") == -1)
		{
			_mkdir("swap");
			_chdir("swap");
		}		

		if(_chdir("eff") == -1)
		{
			_mkdir("eff");
			_chdir("eff");
		}

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.eff", iSectorIdx);

		FILE* pFile;
		if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		RwInt32 iNum = static_cast<RwInt32>(pSector->m_vecNtlPLEntityEffect.size());

		fwrite(&iNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < (int)iNum; ++i)
		{
			CNtlInstanceEffect* pEffect = static_cast<CNtlInstanceEffect*>(pSector->m_vecNtlPLEntityEffect.at(i)); 

			RwUInt32 uiMinimapLayer = pEffect->GetMinimapLayer();
			fwrite(&uiMinimapLayer, sizeof(RwUInt32), 1, pFile);
		}

		fclose(pFile);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}

void CNtlInstanceEffect::LoadSwapFile(RwReal x, RwReal y, RwReal z)
{
	RwV3d	vSectorPos;

	vSectorPos.x = x;
	vSectorPos.y = y;
	vSectorPos.z = z;

	RwInt32	iSectorIdx = GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectorIdx(vSectorPos);
	if (iSectorIdx != -1)
	{
		CNtlWorldSector* pSector = &GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_pSectors[iSectorIdx];

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("swap");
		_chdir("eff");

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.eff", iSectorIdx);

		FILE* pFile;
		if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		RwInt32 iNum;

		fread(&iNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < (int)iNum; ++i)
		{
			CNtlInstanceEffect* pEffect = static_cast<CNtlInstanceEffect*>(pSector->m_vecNtlPLEntityEffect.at(i)); 

			RwUInt32 uiMinimapLayer = 0x00000000;
			fread(&uiMinimapLayer, sizeof(RwUInt32), 1, pFile);
			pEffect->SetMinimapLayer(uiMinimapLayer);
		}

		fclose(pFile);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}
#endif

RwBool CNtlInstanceEffect::LoadFromFile(FILE* pFile)
{
	RwReal fVisibilityDistance;
	fread(&fVisibilityDistance, sizeof(RwReal), 1, pFile);
	SetVisibleCullingDistance(fVisibilityDistance);

	return TRUE;
}

RwBool CNtlInstanceEffect::SaveIntoFile(FILE* pFile)
{
	RwReal fVisibilityDistance = GetVisibleCullingDistance();
	fwrite(&fVisibilityDistance, sizeof(RwReal), 1, pFile);

	return TRUE;
}

RwInt32 CNtlInstanceEffect::SkipToFile(FILE* pFile)
{
	::fseek(pFile, sizeof(RwReal), SEEK_CUR); // visibility distance

	return ::ftell(pFile);
}
BYTE* CNtlInstanceEffect::SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	fwrite(pFileMem, sizeof(RwReal), 1, pFile);
	pFileMem += sizeof(RwReal);

	return pFileMem;
}

BYTE* CNtlInstanceEffect::SkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwReal);

	return pFileMem;
}