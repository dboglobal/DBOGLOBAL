#include "precomp_ntlpresentation.h"

#include "NtlAtomic.h"
#include "NtlTimer.h"

#include "NtlPLEntityFreeList.h"
#include "NtlPLResourceManager.h"
#include "NtlPLProperty.h"
#include "NtlWorldInterface.h"
#include "NtlPLCharacterParser.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLEventGenerator.h"
#include "NtlPLSceneManager.h"
#include "NtlInstanceEffect.h"
#include "NtlPLMaterialExtWrapper.h"
#include "NtlSoundMessage.h"
#include "NtlSoundManager.h"
#include "NtlPLEntityBlend.h"
#include "NtlPLRenderState.h"
#include "NtlPLHelpers.h"
#include "NtlPLCharMeshRenderPipe.h"
#include "NtlPLCharMeshRenderPipe00.h"
#include "NtlPLCharMeshRenderPipe01.h"
#include "NtlPLCharMeshRenderPipeFarAway.h"
#include "ntlworlddi.h"
#include "NtlProfiler.h"
#include "NtlPLOccluder_Base.h"
#include "NtlPLCullingScheduling.h"
 
#include "NtlPLCharacter.h"
 
RwBool CNtlPLCharacter::m_SkipAnimUpdate		= TRUE;
RwBool CNtlPLCharacter::m_SkipSimpleMaterial	= TRUE;
RwBool CNtlPLCharacter::m_SkipAdge				= FALSE;

void* CNtlPLCharacter::operator new(size_t size)
{
	NTL_FUNCTION("CNtlPLCharacter::operator new");

	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_CHARACTER));
}

void CNtlPLCharacter::operator delete(void *pObj)
{
	CNtlPLEntityFreeList::Free(PLENTITY_CHARACTER, pObj);
}


CNtlPLCharacter::CNtlPLCharacter() : m_pProperty(NULL),
m_pResourceClump(NULL),
m_bAnimUpdate(FALSE),
m_pBaseHierarchy(NULL),
m_fAnimSpeed(1.f),
m_pEquipItem(NULL),
m_bRunTimeAlpha(FALSE),
m_SkipAnimUpdateTime(0.0f),
m_SkipAnimUpdateElapsedTime(0.0f),
m_SimpleMaterial(FALSE),
m_PetrifySkillActivated(FALSE)
{
	SetLayer(PLENTITY_LAYER_RWWORLD);
	SetClassType(PLENTITY_CHARACTER);
	SetFlags(NTL_PLEFLAG_ATTACH | NTL_PLEFLAG_PICKING);

	m_vCurPos.x = 0.f;
	m_vCurPos.y = 0.f;
	m_vCurPos.z = 0.f;

	m_vScale.x = 1.f;
	m_vScale.y = 1.f;
	m_vScale.z = 1.f;

	m_vAngle.x = 0.f;
	m_vAngle.y = 0.f;
	m_vAngle.z = 0.f;

	m_sSkinColor.red   = 255;
	m_sSkinColor.green = 255;
	m_sSkinColor.blue  = 255;
	m_sSkinColor.alpha = 255;

	m_sHeadColor.red   = 255;
	m_sHeadColor.green = 255;
	m_sHeadColor.blue  = 255;
	m_sHeadColor.alpha = 255;

	m_sColor.red   = 255;
	m_sColor.green = 255;
	m_sColor.blue  = 255;
	m_sColor.alpha = 255;

	m_sAddColor.red		= 0;
	m_sAddColor.green	= 0;
	m_sAddColor.blue	= 0;
	m_sAddColor.alpha	= 255;

	m_sInkColor.red   = 0;
	m_sInkColor.green = 0;
	m_sInkColor.blue  = 0;
	m_sInkColor.alpha = 255;

	m_fFastMaxExtent = 1.0f;

    m_vBoneStretchTargetPos = ZeroAxis;
    ZeroMemory(m_szBoneStretchAxisBone, sizeof(m_szBoneStretchAxisBone));

	m_vecAtomicList.reserve(10);

	m_ToonEdgeVisibility = TRUE;

	m_pSEMTex = NULL;

#ifndef dNTL_WORLD_CULLING_NEW
	GetNtlPLCullingScheduler()->AddCulling(this);
#endif
}

CNtlPLCharacter::~CNtlPLCharacter()
{
#ifndef dNTL_WORLD_CULLING_NEW
	GetNtlPLCullingScheduler()->RemoveCulling(GetClassType(), this);
#endif

    if(m_sScheduleResInfo.bPCFlag && (m_sScheduleResInfo.uiRace == RACE_NAMEK || m_sScheduleResInfo.uiRace == RACE_MAJIN))
    {
        NTL_DELETE(m_pTypeBoneData);
    }
}

RwBool CNtlPLCharacter::SetProperty(const CNtlPLProperty *pData)
{
	return SetThreadSafeProperty(pData);
}

/**
* Character 생성시 호출이 된다.
* \param pParam Character 생성시 필요한(Head, Hair, SkinColor, HairColor값을 받아야 한다. Pc, Mob, Npc에 대한 구분도)
* \return 실패 유무
*/
RwBool CNtlPLCharacter::Create(const SPLEntityCreateParam *pParam)
{
	NTL_ASSERTE(CNtlPLGlobal::m_pRpWorld);
	if(CNtlPLGlobal::m_pRpWorld == NULL)
		return TRUE;

	m_pEquipItem = NTL_NEW CNtlPLEquipItem;
	m_pEquipItem->SetOwner(this);  
	
	if(pParam)
	{
		SPLCharacterCreateParam *pCharParam = (SPLCharacterCreateParam *)pParam;
		SetSerialID(pCharParam->uiSerialId);
		m_sScheduleResInfo.bPCFlag			= pCharParam->bPcFlag;
		m_sScheduleResInfo.uiClass			= pCharParam->uiClass;
		m_sScheduleResInfo.uiRace			= pCharParam->uiRace;
		m_sScheduleResInfo.uiGender			= pCharParam->uiGender;
		m_sScheduleResInfo.uiHeadType		= pCharParam->uiHeadType;
		m_sScheduleResInfo.uiFaceType		= pCharParam->uiFaceType;
		m_sScheduleResInfo.uiSkinColorType	= pCharParam->uiSkinColorType;
		m_sScheduleResInfo.uiHeadColorType	= pCharParam->uiHeadColorType;
		m_sScheduleResInfo.uiSerialId		= pCharParam->uiSerialId;
		m_sScheduleResInfo.bNotShadingFlag	= pCharParam->bNotShadingFlag;
        m_sScheduleResInfo.bIsAdult         = pCharParam->bIsAdult;

		if( GetCharScheduleResInfo()->bPCFlag )
		{
			m_pEquipItem->Create( m_sScheduleResInfo.uiClass, 
				m_sScheduleResInfo.uiRace, 
				m_sScheduleResInfo.uiGender, 
				m_sScheduleResInfo.uiHeadType,
				m_sScheduleResInfo.uiFaceType, 
				m_sScheduleResInfo.uiSkinColorType,
				m_sScheduleResInfo.uiHeadColorType,
                m_sScheduleResInfo.bIsAdult);
		}
	}

	// Namek has a skill that increases the bones. If you share Bone's information, other Namekes will also increase their patterns.
	// So Namekian does not share the Bone, but has a separate copy. (by agebreak October 20, 2008)
    if(pParam && m_sScheduleResInfo.bPCFlag && (m_sScheduleResInfo.uiRace == RACE_NAMEK || m_sScheduleResInfo.uiRace == RACE_MAJIN))
    {
        m_pTypeBoneData = NTL_NEW STypeBoneData();
        memcpy(m_pTypeBoneData, m_pProperty->GetBoneScaleData(), sizeof(STypeBoneData));
    }
    else
    {
        m_pTypeBoneData = m_pProperty->GetBoneScaleData();
    }    

	if(!CreateScheduling(pParam))
		return FALSE;

	return TRUE;
}

void CNtlPLCharacter::CallSchedulingResource(CNtlPLResource *pResource)
{
	m_pResourceClump = pResource;
	NTL_ASSERTE(m_pResourceClump);

	if( m_pResourceClump == NULL )
		return;

	CreateThreadSafe();
}

RwBool CNtlPLCharacter::CreateThreadSafe(void)
{
	NTL_ASSERTE( m_pResourceClump );
    
    m_pBaseHierarchy = Helper_GetHierarchyClump(m_pResourceClump->GetClump());
    NTL_ASSERTE(m_pBaseHierarchy);
    if(m_pBaseHierarchy == NULL)
        return FALSE;


	// In case of PC, remove all Atomic.
 	if( GetCharScheduleResInfo()->bPCFlag )
	{
		Helper_DestroyAllAtomics(&m_vecAtomicList);
 		Helper_RemoveClumpAllAtomics(m_pResourceClump->GetClump());
	}

	NTL_ASSERTE(m_pProperty);
	m_InstanceAnimTable.Create(m_pProperty->GetAnimTable());

	if( m_sScheduleResInfo.uiAnimKey == NML_IDLE_LOOP )
		m_sScheduleResInfo.bAnimLoop = TRUE;

	if(!CreateAnim(m_sScheduleResInfo.uiAnimKey, m_sScheduleResInfo.fAnimElapsedTime, m_sScheduleResInfo.bAnimLoop))
		return FALSE;

	if(GetCharScheduleResInfo()->bNotShadingFlag)
		//CreateToonData("smooth_white.png", "texture/toon/");
		CreateToonData("smooth.png", "texture/toon/");
	else
		CreateToonData("smooth.png", "texture/toon/");

	Helper_SetToonClump(m_pResourceClump->GetClump(), &m_ToonData);
	Helper_GetBoneList(m_pResourceClump->GetClump(), &m_mapFrame);

	m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].SetCallBack(this, &CNtlPLCharacter::CallBackBaseAnim);
	m_nBoneCount = m_pBaseHierarchy->numNodes;

	if( GetCharScheduleResInfo()->bPCFlag)
		RpNtlClumpSetData( m_pResourceClump->GetClump(), this);
	else
	{
		Helper_SetClumpAllAtomics( m_pResourceClump->GetClump(), &m_vecAtomicList);

		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RpAtomic *pAtomic = m_vecAtomicList[i]->pData;

			RpNtlAtomicSetRenderCallBack( pAtomic, RpAtomicGetRenderCallBack(pAtomic) );
			RpAtomicSetRenderCallBack(pAtomic, CNtlPLCharacter::RenderCallBack);

			RpNtlAtomicSetData(pAtomic, this);
		}		

		// NPC, MOB, VEHICLE spherical enviroment mapping
		RwUInt16 EnvTexName = 0;

		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			EnvTexName = RpNtlAtomicGetEnvTexName(m_vecAtomicList[i]->pData);
			if(EnvTexName)
				break;
		}		

		if(EnvTexName)
		{
			char szTmp[32] = {0,};
			_itoa_s(EnvTexName, szTmp, 10);

			m_pSEMTex = GetNtlResourceManager()->LoadTexture(szTmp, ".\\Texture\\EnvMap\\");
		}
	}	

	m_sScheduleResInfo.bLoadComplete = TRUE;

	// Set the Link Effect (must be set after loadComplete is set).
	for(UINT i = 0; i < m_pProperty->m_vLinkEffect.size(); ++i)
	{
		SEventLinkEffect* pEventLinkEffect = m_pProperty->m_vLinkEffect[i];        
		m_LinkEffectInstance.AttachLinkEffect(this, pEventLinkEffect);
	}

	SetAnimUpdate(TRUE);

	SetPosition(&m_vCurPos);
	SetAngleY(m_vAngle.y);
	SetScale(m_vScale.x);

	SetColor(m_sColor.red, m_sColor.green, m_sColor.blue);
	SetAlpha(m_sColor.alpha);
	SetWeightAlpha( GetWeightAlpha() );

	SetSerialID( GetCharScheduleResInfo()->uiSerialId);
	SetVisible( GetCharScheduleResInfo()->bVisible );

	SetBaseAnimation( m_sScheduleResInfo.uiAnimKey, m_sScheduleResInfo.fAnimElapsedTime, m_sScheduleResInfo.bAnimLoop );

	CreateOccluderProxy();

	AddWorld();    

	return TRUE;
}

RwBool CNtlPLCharacter::CreateOccluderProxy()
{
#ifdef dNTL_WORLD_CULLING_NEW
	return COccluderProxy::CreateOccluderProxy(0.5f, m_vecAtomicList.size() + 1);
#else
	return COccluderProxy::CreateOccluderProxy(0.5f, m_vecAtomicList.size());
#endif
}

RwBool CNtlPLCharacter::SetThreadSafeProperty(const CNtlPLProperty *pData)
{
	NTL_ASSERTE( pData );
	if(pData == NULL)
		return FALSE;

	m_pProperty = (CNtlPLCharacterProperty *)pData;

	return TRUE;
}

RwBool CNtlPLCharacter::CreateScheduling(const SPLEntityCreateParam * pParam)
{
	NTL_ASSERT( pParam != NULL, RWSTRING( "NULL Param" ) );
	if( pParam == NULL)
		return FALSE;

	std::string strClumpFileName = m_pProperty->GetBaseMeshFilePath();
	strClumpFileName += m_pProperty->GetBaseMeshFileName();

	std::string strTexPath = "Texture\\character\\;";
	strTexPath += "Texture\\item\\";

	CNtlPLResourceManager::GetInstance()->LoadSchedulingClump( strClumpFileName.c_str(), strTexPath.c_str(), this );   

	return TRUE;
}

RwReal CNtlPLCharacter::GetMaxExtent()
{
	RwReal fWidth	= GetWidth();
	RwReal fHeight	= GetHeight();
	RwReal fDepth	= GetDepth();

	if(fWidth > fHeight && fWidth > fDepth)
	{
		return GetWidth();
	}

	if(fHeight > fWidth && fHeight > fDepth)
	{
		return GetHeight();
	}

	if(fDepth > fHeight && fDepth > fWidth)
	{
		return fDepth;
	}

	return fWidth;
}

RwReal CNtlPLCharacter::GetFastMaxExtent()
{
	return m_fFastMaxExtent;
}

void CNtlPLCharacter::SetSkipAnimUpdate(RwBool _Skip/* = TRUE*/)
{
	m_SkipAnimUpdate = _Skip;
}

void CNtlPLCharacter::SetSkipSimpleMaterial(RwBool _Skip/* = TRUE*/)
{
	m_SkipSimpleMaterial = _Skip;
}

RwBool CNtlPLCharacter::GetSkipSimpleMaterial(void)
{
	return m_SkipSimpleMaterial;
}

void CNtlPLCharacter::SetSkipEdge(RwBool _Skip /*= FALSE*/)
{
	m_SkipAdge = _Skip;
}

RwBool CNtlPLCharacter::GetSkipEdge(void)
{
	return m_SkipAdge;
}

RwBool CNtlPLCharacter::Update(float fElapsed)
{
	if ( 0.f == fElapsed )
	{
		return TRUE;
	}
/*
	static RwBool woody = FALSE;
	if(dOKEY(DIK_I))
	{
		woody = !woody;
		SetPetrifyMaterial(woody);
		SetAnimUpdate(!woody);
	}
*/
	m_fFastMaxExtent = GetMaxExtent();

	RwBool bVisible = IsVisible();
	// If you do not see it, do not update it. (Map tool only)
	if( !bVisible && CNtlPLGlobal::m_bCharacterUpdateVisible)
		return TRUE;

	if( GetCharScheduleResInfo()->bLoadComplete)
	{
		// DISABLED BY DANEOS. IF WE DONT LOOK AT PLAYER, IT WILL FUCK THE ANIMATION
		/*if(!bVisible)
		{
			m_SkipAnimUpdateElapsedTime += fElapsed;
			m_SkipAnimUpdateTime = CNtlTimer::AdjustBestFpsToSec();
			if(m_SkipAnimUpdateElapsedTime < m_SkipAnimUpdateTime)
				return TRUE;

			fElapsed = m_SkipAnimUpdateElapsedTime;
		}*/

		if(m_pEquipItem)
		{
			m_pEquipItem->Update(fElapsed);
		}

		// get the distance from a camera to me
		RwV3d		vPos	= GetPosition();
		RwMatrix*	pMatrix = RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));

		RwV3d vDelta;
		RwV3dSubMacro(&vDelta, RwMatrixGetPos(pMatrix), &vPos);
		RwReal	fLen			= RwV3dLength(&vDelta);
		RwReal	MaxExtent		= GetFastMaxExtent();
		RwBool  CharException	= GetFlags() & NTL_PLEFLAG_DUMMY;

		// simplify material when the distance is far enough away
		if(fLen > 70.0f && MaxExtent < 4.0f && !CharException)
		{
			SetSimpleMaterial(TRUE);
		}
		else
		{
			SetSimpleMaterial(FALSE);
		}

		if(m_SkipAnimUpdate)
		{
			// Guard band clip test
			/*
			RwSphere sphere;
			sphere.center = vPos;
			sphere.radius = GetFastMaxExtent() * 0.5f;
			sphere.center.y += (GetHeight() * 0.5f);

			// Big entities exceptions; dinosaurs
			RwFrustumTestResult ret = RwCameraFrustumTestSphere(CNtlPLGlobal::m_RwCamera, &sphere);
			*/
			if(/*ret == rwSPHEREOUTSIDE ||*/m_uiCullFlags)
			{
				// one second delay 4 invisible chars
				m_SkipAnimUpdateTime = 1.0f;
			}
			else
			{
				// 4 meters diameter 4 the big entities
				if(MaxExtent > 4.0f)
				{
					// 200 meters 4 distance visibility
					// 0.1 sec 4 big entities
					m_SkipAnimUpdateTime = fLen * 0.1f / 200.0f;
				}
				else
				{
					// 0.2 sec 4 the entities smaller than the big ones
					m_SkipAnimUpdateTime = fLen * 0.2f / 200.0f;
				}
			}		

			// GUI characters exceptions
			if(CharException)
			{
				m_SkipAnimUpdateTime = 0.001f;
			}
		}
		else
		{
			m_SkipAnimUpdateTime = 0.001f;
		}

		m_SkipAnimUpdateElapsedTime += fElapsed;

		if(m_SkipAnimUpdateElapsedTime > m_SkipAnimUpdateTime)
		{
			if(m_bAnimUpdate)
			{
				float fFinalAnimSpeed = 0.f;
				if(m_pProperty != NULL)
				{
					if(m_sBaseAnimPlayInfo.m_uiAnimKey == RUN_FRONT )
						fFinalAnimSpeed = m_fAnimSpeed * m_pProperty->GetInitRunFrontAnimSpeed();
					else if(m_sBaseAnimPlayInfo.m_uiAnimKey == WALK_FRONT )
						fFinalAnimSpeed = m_fAnimSpeed * m_pProperty->GetInitWalkFrontAnimSpeed();
					else
						fFinalAnimSpeed = m_fAnimSpeed;
				}
				else
					fFinalAnimSpeed = m_fAnimSpeed;

				m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].Update(m_SkipAnimUpdateElapsedTime * fFinalAnimSpeed);
				//m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].Update(fElapsed * fFinalAnimSpeed);

				if( m_pTypeBoneData && m_pTypeBoneData->bBoneScale)
					UpdatePreBoneScale();

				RpHAnimHierarchyUpdateMatrices(m_pBaseHierarchy);

				if( m_pTypeBoneData && m_pTypeBoneData->bBoneScale)
					UpdatePostBoneScale();
			}
			else
			{
				RwReal fCurTime = m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].GetCurrentAnimTime();
				m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].SetCurrentAnimTime(fCurTime);

				if( m_pTypeBoneData && m_pTypeBoneData->bBoneScale)
					UpdatePreBoneScale();

				RpHAnimHierarchyUpdateMatrices(m_pBaseHierarchy);

				if( m_pTypeBoneData && m_pTypeBoneData->bBoneScale)
					UpdatePostBoneScale();
			}

			//CNtlPLAttach::Update(fElapsed);
			//CNtlPLAttach::Update(m_SkipAnimUpdateElapsedTime);

			m_SkipAnimUpdateElapsedTime = 0;
		}
		else
		{
			if(GetCharScheduleResInfo()->bPCFlag)
			{
				RwReal fCurTime = m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].GetCurrentAnimTime();
				m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].SetCurrentAnimTime(fCurTime);

				if( m_pTypeBoneData && m_pTypeBoneData->bBoneScale)
					UpdatePreBoneScale();

				RpHAnimHierarchyUpdateMatrices(m_pBaseHierarchy);

				if( m_pTypeBoneData && m_pTypeBoneData->bBoneScale)
					UpdatePostBoneScale();
			}
			else
			{
				RpHAnimHierarchyUpdateMatrices(m_pBaseHierarchy);
			}
		}

		CNtlPLAttach::Update(fElapsed);
		/*
		if(m_pEquipItem)
		{
		m_pEquipItem->Update(fElapsed);
		}

		// Guard band clip test
		RwSphere sphere;
		sphere.center = vPos;
		sphere.radius = GetFastMaxExtent() * 0.5f;
		sphere.center.y += (GetHeight() * 0.5f);

		// Big entities exceptions; dinosaurs
		RwFrustumTestResult ret = RwCameraFrustumTestSphere(CNtlPLGlobal::m_RwCamera, &sphere);

		if(m_bAnimUpdate)
		{
		float fFinalAnimSpeed = 0.f;
		if(m_pProperty != NULL)
		{
		if(m_sBaseAnimPlayInfo.m_uiAnimKey == RUN_FRONT )
		fFinalAnimSpeed = m_fAnimSpeed * m_pProperty->GetInitRunFrontAnimSpeed();
		else if(m_sBaseAnimPlayInfo.m_uiAnimKey == WALK_FRONT )
		fFinalAnimSpeed = m_fAnimSpeed * m_pProperty->GetInitWalkFrontAnimSpeed();
		else
		fFinalAnimSpeed = m_fAnimSpeed;
		}
		else
		fFinalAnimSpeed = m_fAnimSpeed;

		m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].Update(fElapsed * fFinalAnimSpeed);

		if( m_pTypeBoneData && m_pTypeBoneData->bBoneScale)
		UpdatePreBoneScale();

		RpHAnimHierarchyUpdateMatrices(m_pBaseHierarchy);

		if( m_pTypeBoneData && m_pTypeBoneData->bBoneScale)
		UpdatePostBoneScale();
		}
		else
		{
		RwReal fCurTime = m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].GetCurrentAnimTime();
		m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].SetCurrentAnimTime(fCurTime);

		if( m_pTypeBoneData && m_pTypeBoneData->bBoneScale)
		UpdatePreBoneScale();

		RpHAnimHierarchyUpdateMatrices(m_pBaseHierarchy);

		if( m_pTypeBoneData && m_pTypeBoneData->bBoneScale)
		UpdatePostBoneScale();
		}
		CNtlPLAttach::Update(fElapsed); */
	}
	else
	{
		m_sScheduleResInfo.fAnimElapsedTime += fElapsed;
	}

	return TRUE;
}

/**
* Visula Manager에서 Render를 강제로 호출하게 했다.
* CallBack 형식으로 변경을 해야 할듯 하다.
* \return 실패 유무
*/
RwBool CNtlPLCharacter::Render(void)
{	
	return TRUE;
}

static RpAtomic *RenderToTextureAtomic(RpAtomic *pAtomic, void *pData)
{
	RwUInt32 uiRenderFlags = RpNtlAtomicGetFlag(pAtomic);

	if(uiRenderFlags & NTL_ALPHATEST)
		BeginNtlAotmicAlphaTestState();
	if(uiRenderFlags & NTL_TWOSIDE)
		BeginNtlAtomicTwoSideState();

	if ( uiRenderFlags & NTL_ALPHA)		UpdateAlphaState(TRUE);

	pAtomic->renderCallBack(pAtomic);

	if ( uiRenderFlags & NTL_ALPHA)		UpdateAlphaState(FALSE);

	if(uiRenderFlags & NTL_TWOSIDE)
		EndNtlAtomicTwoSideState();
	if(uiRenderFlags & NTL_ALPHATEST)
		EndNtlAtomicAlphaTestState();

	return pAtomic;
}

/**
* Texture에 Rendering을 할 경우 쓰는 함수
* \return 실패 유무
*/
RwBool CNtlPLCharacter::RenderToTexture()
{
	if( !GetCharScheduleResInfo()->bLoadComplete ||
		m_pResourceClump == NULL)
		return FALSE;

	if(m_pResourceClump->GetClump())
	{
		RpClumpForAllAtomics(m_pResourceClump->GetClump(), RenderToTextureAtomic, NULL);

		//RpClumpRender(m_pResourceClump->GetClump());
	}

	if(m_pEquipItem)
	{
		m_pEquipItem->RenderToTexture();
	}

	return TRUE;
}

void CNtlPLCharacter::UpdatePreBoneScale()
{
	int nTotalBone = m_pBaseHierarchy->numNodes;

	RwUInt32	stack[MAX_NUM_SCALE_BONE>>1];
	RwUInt32	nStack = 0;
	RwUInt32	parentIndex = 0;
	RwUInt32	currentIndex;
    RwBool      bTargetPosApply = FALSE;
    RwUInt32    uiBone1 = 0;
    RwMatrix*   pMatClavicle = NULL;
    
    if(strlen(m_szBoneStretchAxisBone) > 0)
    {
        uiBone1 = GetBoneIndex(m_szBoneStretchAxisBone);   
        bTargetPosApply = RwV3dLength(&m_vBoneStretchTargetPos) > 0.0f ? TRUE : FALSE;
        pMatClavicle = GetBoneMatrix(m_szBoneStretchAxisBone);
    }

	for (currentIndex=1; currentIndex < (RwUInt32)nTotalBone; currentIndex++)
	{
		RwUInt32            flags;
		RpHAnimInterpFrame *iFrame = (RpHAnimInterpFrame *) 
			rtANIMGETINTERPFRAME(m_pBaseHierarchy->currentAnim, currentIndex);

		iFrame->t.x *= m_pTypeBoneData->BoneScale[parentIndex].fLength;
		iFrame->t.y *= m_pTypeBoneData->BoneScale[parentIndex].fWidth;
		iFrame->t.z *= m_pTypeBoneData->BoneScale[parentIndex].fWidth;

        if(bTargetPosApply && parentIndex == uiBone1)
        {
            // 어깨 본으로부터 타겟의 각도를 계산해서 변형한다. 
            // 타겟 방향으로 본을 늘리기 위해서다. 나메크 팔늘리기 스킬용 (by agebreak 08-10-23)
            RwV3d vBonePos = pMatClavicle->pos;            
            vBonePos.x = GetPosition().x;
            vBonePos.z = GetPosition().z;
            RwV3d vDir = m_vBoneStretchTargetPos - GetPosition();
            vDir.y = 0.0f;
            RwV3dNormalize(&vDir, &vDir);

            RwV3d vTargetDir = m_vBoneStretchTargetPos - vBonePos;
            RwV3dNormalize(&vTargetDir, &vTargetDir);            
            RwReal fAngle = acos(RwV3dDotProduct(&vDir, &vTargetDir));

            // 왼쪽, 오른쪽 어깨에 따라서 휘어지는 각도가 다르다.
            if(strstr(m_szBoneStretchAxisBone, " L ") > 0)
            {
                fAngle = vBonePos.y <= m_vBoneStretchTargetPos.y ? fAngle : -fAngle;
            }
            else if(strstr(m_szBoneStretchAxisBone, " R ") > 0)
            {
                fAngle = vBonePos.y <= m_vBoneStretchTargetPos.y ? -fAngle : fAngle;
            }
            else
            {
                fAngle = 0.0f;
            }
            

            RwMatrix matRot, matAnim;                
            RwMatrixSetIdentity(&matRot);
            RwMatrixSetIdentity(&matAnim);
            RtQuatConvertToMatrix(&iFrame->q, &matAnim);            
            RwMatrixRotate(&matRot, &ZAxis, NTL_RAD2DEG(fAngle), rwCOMBINEPOSTCONCAT);
            RwMatrixMultiply(&matAnim, &matRot, &matAnim);
            RtQuatConvertFromMatrix(&iFrame->q, &matAnim);            
        }

		flags = m_pBaseHierarchy->pNodeInfo[currentIndex].flags;

		if (flags & rpHANIMPUSHPARENTMATRIX)
		{
			stack[++nStack] = parentIndex;
		}

		if (flags & rpHANIMPOPPARENTMATRIX)
		{
			parentIndex = stack[nStack--];            
            nStack = max(nStack, 0);
		}
		else
		{
			parentIndex = currentIndex;
		}
	}
}

void CNtlPLCharacter::UpdatePostBoneScale()
{
	int nTotalBone = m_pBaseHierarchy->numNodes;
    RwUInt32 uiBone1 = GetBoneIndex("Bip01 L UpperArm");

	RwMatrix    *matrix;
	matrix = RpHAnimHierarchyGetMatrixArray(m_pBaseHierarchy);

	for (int i=0; i < nTotalBone ; i++)
	{
		RwV3d       vScale;        

		vScale.x = m_pTypeBoneData->BoneScale[i].fLength;
		vScale.y = vScale.z = m_pTypeBoneData->BoneScale[i].fWidth;
		RwMatrixScale(matrix, &vScale, rwCOMBINEPRECONCAT);
        
        //if(uiBone1 == i)
        //{
        //    RwMatrixRotate(matrix, &XAxis, -45.0f, rwCOMBINEPOSTCONCAT);
        //}

		matrix++;
	}
}

/**
* Toon Data를 생성을 한다.
*/
void CNtlPLCharacter::CreateToonData(const RwChar *szTexName, const RwChar *szTexPath)
{
	NTL_FUNCTION("CNtlPLCharacter::CreateToonData()");

	m_ToonData.pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(szTexName, szTexPath);
	m_ToonData.pToonInk = RpToonInkCreate();
	//RwTextureSetFilterMode(m_ToonData.pTexture, rwFILTERLINEARMIPLINEAR); 

	SetInkThickness(DEFAULT_INK_THICKNESS);
	RpToonInkSetColor(m_ToonData.pToonInk, m_sInkColor);
	RpToonInkSetName(m_ToonData.pToonInk, "silhouette" );

	m_ToonData.pToonPaint = RpToonPaintCreate();
	RpToonPaintSetGradientTexture(m_ToonData.pToonPaint, m_ToonData.pTexture);

	NTL_RETURNVOID();
}

/**
* Animation에 필요한 세팅을 한다.
* \param pAnim RtAnimAnimation Data
* \return 실패 유무
*/
RwBool CNtlPLCharacter::CreateAnim(RwUInt32 uiAnimKey, RwReal fStartTime, RwBool bLoop )
{
	NTL_ASSERTE(m_pResourceClump != NULL);

	SInstanceAnimData *pInstanceAnimData = m_InstanceAnimTable.Get(uiAnimKey);

	// CHECK IF ANIMATION EXIST
	if(pInstanceAnimData == NULL)
	{
		// LOGGING

		std::string *pStrAnimName = CNtlPLCharacterParser::GetInstance().GetPCMatchTable()->GetString(uiAnimKey);

		if(pStrAnimName == NULL)
			pStrAnimName = CNtlPLCharacterParser::GetInstance().GetMobMatchTable()->GetString(uiAnimKey);

		if(pStrAnimName == NULL)
			pStrAnimName = CNtlPLCharacterParser::GetInstance().GetNPCMatchTable()->GetString(uiAnimKey);

		if(pStrAnimName != NULL)
			DBO_WARNING_MESSAGE("Find not animation: " << "ModelName: " << GetName() << " AnimName: " << pStrAnimName->c_str() << " AnimID: " << uiAnimKey );

		if(pStrAnimName == NULL)
			DBO_WARNING_MESSAGE("Find not animation: " << "ModelName: " << GetName() << "AnimID: " << uiAnimKey );
	}

	if((pInstanceAnimData == NULL) || (pInstanceAnimData->pResource == NULL) )
		return FALSE;
	

	RpHAnimHierarchySetCurrentAnim(m_pBaseHierarchy, pInstanceAnimData->pResource->GetAnimation());
    
    Helper_SetHierarchyClumpSkin(m_pResourceClump->GetClump(), m_pBaseHierarchy);

	RpHAnimHierarchySetFlags( m_pBaseHierarchy,
		(RpHAnimHierarchyFlag)(RpHAnimHierarchyGetFlags(m_pBaseHierarchy) | 
		rpHANIMHIERARCHYUPDATELTMS | 
		rpHANIMHIERARCHYUPDATEMODELLINGMATRICES));

	RpHAnimHierarchyAttach(m_pBaseHierarchy);
	RpHAnimUpdateHierarchyMatrices(m_pBaseHierarchy);

	if(!m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].Create(m_pBaseHierarchy))
		return FALSE;

	m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].SetAllAnimation(pInstanceAnimData, fStartTime, bLoop);
	m_sBaseAnimPlayInfo.m_uiAnimKey = uiAnimKey;

	return TRUE;
}

RwBool CNtlPLCharacter::IsExistAnim(RwUInt32 uiAnimKey)
{
	STypeAnimData *pTypeAnimData = m_pProperty->GetAnimTable()->Get(uiAnimKey);
	if(pTypeAnimData == NULL)
	{
		return FALSE;
	}
	return TRUE;
}


/**
* Animation Change를 한다. RUN_FRONT, RUN_BACK, ROTATE는 현재 Animation이 있을 경우 변경하지 않는다.
* \param uiAnimKey		AnimationKey
* \param fStartTime	Animation 시작시간
* \param bLoop			Loop 유무
* \return 실패 유무
*/
RwBool CNtlPLCharacter::SetBaseAnimation(RwUInt32 uiAnimKey, RwReal fStartTime, RwBool bLoop)
{
	if( !GetCharScheduleResInfo()->bLoadComplete )
	{
		m_sScheduleResInfo.uiAnimKey = uiAnimKey;		
		m_sScheduleResInfo.fAnimElapsedTime = 0.f;
		m_sScheduleResInfo.bAnimLoop = bLoop;
		return FALSE;
	}

	/*if (!(*(int(**)(void))(*(_DWORD *)a1 + 20))() || !(*(int(__thiscall **)(int))(*(_DWORD *)v5 + 24))(v5))
		return 0;*/

	// When the animation changes, all LoopEffects are destroyed.
	if(m_sBaseAnimPlayInfo.m_uiAnimKey != uiAnimKey)
	{
		ClearLoopEffect();
		ClearLoopSound();
	}

	SInstanceAnimData *pInstanceAnimData = m_InstanceAnimTable.Get(uiAnimKey);
	if(pInstanceAnimData == NULL)
	{
		std::string *pStrAnimName = CNtlPLCharacterParser::GetInstance().GetPCMatchTable()->GetString(uiAnimKey);

		if(pStrAnimName == NULL)
			pStrAnimName = CNtlPLCharacterParser::GetInstance().GetMobMatchTable()->GetString(uiAnimKey);

		if(pStrAnimName == NULL)
			pStrAnimName = CNtlPLCharacterParser::GetInstance().GetNPCMatchTable()->GetString(uiAnimKey);

		if(pStrAnimName != NULL)
			DBO_WARNING_MESSAGE("Find not animation: " << "ModelName: " << GetName() << " AnimName: " << pStrAnimName->c_str() << " AnimID: " << uiAnimKey);

		if(pStrAnimName == NULL)
			DBO_WARNING_MESSAGE("Find not animation : ModelName("<< GetName() << "), AnimID(" << uiAnimKey <<")");

		pInstanceAnimData = m_InstanceAnimTable.Get(NML_IDLE_LOOP);
	}

	if( (pInstanceAnimData == NULL) || 
		(pInstanceAnimData->pResource == NULL) )
		return FALSE;

	// to do
	/*if (!**(_DWORD **)((*(int(__thiscall **)(int))(*(_DWORD *)v5 + 24))(v5) + 32))
	{
		v18 = *(_DWORD **)(*v8 + 36);
		v19 = (*(int(__thiscall **)(int))(*(_DWORD *)v5 + 24))(v5);
		sub_E88530(*(_DWORD *)(v19 + 32), v18);
		v20 = (int *)(*(int(__thiscall **)(int))(*(_DWORD *)v5 + 24))(v5);
		sub_E895E0(v20);
	}*/

	m_sBaseAnimPlayInfo.m_uiAnimKey = uiAnimKey;

	m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].SetAnimation(pInstanceAnimData, fStartTime, bLoop);

	return TRUE;
}

/**
* Animation Blend를 하지 않고 Pre, Next 모두 Animation를 바꾼다. 
* \param uiAnimKey		AnimationKey
* \param fStartTime	Animation 시작시간
* \param bLoop			Loop 유무
* \return 실패 유무
*/
RwBool CNtlPLCharacter::SetAllAnimation(RwUInt32 uiAnimKey, RwReal fStartTime, RwBool bLoop)
{
	/*if (!(*(int(**)(void))(*(_DWORD *)a1 + 20))() || !(*(int(__thiscall **)(int))(*(_DWORD *)v5 + 24))(v5))
		return 0;*/

	// When the animation changes, all LoopEffects are destroyed.
	if (m_sBaseAnimPlayInfo.m_uiAnimKey != uiAnimKey)
	{
		ClearLoopEffect();
		ClearLoopSound();
	}

	SInstanceAnimData *pInstanceAnimData = m_InstanceAnimTable.Get(uiAnimKey);
	if (pInstanceAnimData == NULL)
	{
		std::string *pStrAnimName = CNtlPLCharacterParser::GetInstance().GetPCMatchTable()->GetString(uiAnimKey);

		if (pStrAnimName == NULL)
			pStrAnimName = CNtlPLCharacterParser::GetInstance().GetMobMatchTable()->GetString(uiAnimKey);

		if (pStrAnimName == NULL)
			pStrAnimName = CNtlPLCharacterParser::GetInstance().GetNPCMatchTable()->GetString(uiAnimKey);

		if(pStrAnimName != NULL)
			DBO_WARNING_MESSAGE("Find not animation: " << "ModelName: " << GetName() << " AnimName: " << pStrAnimName->c_str() << " AnimID: " << uiAnimKey);

		if (pStrAnimName == NULL)
			DBO_WARNING_MESSAGE("Find not animation : ModelName(" << GetName() << "), AnimID(" << uiAnimKey << ")");

		pInstanceAnimData = m_InstanceAnimTable.Get(NML_IDLE_LOOP);
	}

	if ((pInstanceAnimData == NULL) ||
		(pInstanceAnimData->pResource == NULL))
		return FALSE;


	// to do
	/*if (!**(_DWORD **)((*(int(__thiscall **)(int))(*(_DWORD *)v5 + 24))(v5) + 32))
	{
		v18 = *(_DWORD **)(*v8 + 36);
		v19 = (*(int(__thiscall **)(int))(*(_DWORD *)v5 + 24))(v5);
		sub_E88530(*(_DWORD *)(v19 + 32), v18);
		v20 = (int *)(*(int(__thiscall **)(int))(*(_DWORD *)v5 + 24))(v5);
		sub_E895E0(v20);
	}*/

	m_sBaseAnimPlayInfo.m_uiAnimKey = uiAnimKey;
	m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].SetAllAnimation(pInstanceAnimData, fStartTime, bLoop);

	return TRUE;
}

RwBool CNtlPLCharacter::SetBlend(EBlendType eBlendType, RwReal fBlendAlpha, RwReal fBlendInterval)
{
	NTL_FUNCTION("CNtlPLCharacter::SetBlend()");

	m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].SetBlend(eBlendType, fBlendAlpha, fBlendInterval);

	NTL_RETURN(TRUE);
}

/**
* Animation에서의 AnimHit
* \return SAnimHitParam
*/
SAnimPlayInfo *CNtlPLCharacter::GetBaseAnimPlayInfo()
{
	NTL_FUNCTION("CNtlPLCharacter::GetBaseAnimPlayInfo()");

	if( !GetCharScheduleResInfo()->bLoadComplete )
		return NULL;

	m_sBaseAnimPlayInfo.vecAnimHit.clear();

	SInstanceAnimData *pInstanceAnimData = m_InstanceAnimTable.Get(m_sBaseAnimPlayInfo.m_uiAnimKey);
	if (!pInstanceAnimData)
		return NULL;

	RwUInt32 uiCount = pInstanceAnimData->pTypeAnimData->vecAnimEvent.size();

	for(RwUInt32 i = 0; i < uiCount; i++)
	{
		if(pInstanceAnimData->pTypeAnimData->vecAnimEvent[i]->eEventID == EVENT_ANIM_HIT)
		{
			m_sBaseAnimPlayInfo.vecAnimHit.push_back( (SEventAnimHit *)pInstanceAnimData->pTypeAnimData->vecAnimEvent[i] );
		}
	}

	NTL_RETURN(&m_sBaseAnimPlayInfo);
}

RwReal CNtlPLCharacter::GetDurationAnimTime(RwUInt32 uiAnimKey)
{
	if( !GetCharScheduleResInfo()->bLoadComplete )
		return 0.f;

	SInstanceAnimData *pInstanceAnimData = m_InstanceAnimTable.Get(uiAnimKey);
	if (!pInstanceAnimData)
		return 0.f;

	NTL_ASSERTE(pInstanceAnimData->pResource);

	return pInstanceAnimData->pResource->GetAnimation()->duration;
}

RwReal CNtlPLCharacter::GetWidth()
{
	RwBBox *pbbBoxAnim = m_pProperty->GetAnimBBox();
	RwReal fWidth = pbbBoxAnim->sup.x - pbbBoxAnim->inf.x;

	fWidth *= m_vScale.x;
	return fWidth;
}

RwReal CNtlPLCharacter::GetHeight()
{
	RwBBox *pbbBoxAnim = m_pProperty->GetAnimBBox();
	RwReal fHeight = pbbBoxAnim->sup.y - pbbBoxAnim->inf.y;

	fHeight *= m_vScale.y;
	return fHeight;
}

RwReal CNtlPLCharacter::GetDepth()
{
	RwBBox *pbbBoxAnim = m_pProperty->GetAnimBBox();
	RwReal fDepth = pbbBoxAnim->sup.z - pbbBoxAnim->inf.z;

	fDepth *= m_vScale.z;
	return fDepth;
}

void CNtlPLCharacter::Destroy()
{
	RemoveWorld();
	for each(SPLAttachAttr* pPLAttachAttr in m_listAttachAttr)
	{
		if(!pPLAttachAttr->pPLEntity)
			continue;

		if(strcmp(pPLAttachAttr->pPLEntity->GetName(), EFFECT_NAME_TRACE) == 0)
		{
			pPLAttachAttr->pPLEntity->SetAttachParent(NULL);
			GetSceneManager()->DeleteEntity(pPLAttachAttr->pPLEntity);
			pPLAttachAttr->pPLEntity = NULL;
		}
	}

	if(m_pEquipItem)
	{
		m_pEquipItem->Destroy();
		NTL_DELETE(m_pEquipItem);
		m_pEquipItem = NULL;
	}

	ClearLoopEffect();    
	ClearLoopSound();

	if(m_ToonData.pToonPaint)
		RpToonPaintDestroy(m_ToonData.pToonPaint);
	if(m_ToonData.pToonInk)
		RpToonInkDestroy(m_ToonData.pToonInk);
	if(m_ToonData.pTexture)
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_ToonData.pTexture);

	if(m_pBaseHierarchy != NULL)
		RpHAnimHierarchyDestroy(m_pBaseHierarchy);

	CNtlPLResourceManager::GetInstance()->UnLoadScheduling(this, m_pResourceClump );
	m_pResourceClump = NULL;

	m_mapFrame.clear();
	m_InstanceAnimTable.Destroy();
	m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].Destroy();
	m_AnimLayer[CHARACTER_ANIM_LAYER_UPPER].Destroy();
	Helper_DestroyAllAtomics(&m_vecAtomicList);

	DestroyOccluderProxy();
}


void CNtlPLCharacter::AddWorld(void)
{
	if( m_pResourceClump == NULL ||
		CNtlPLGlobal::m_pRpWorld == NULL)
		return;

	if(RpClumpGetWorld( m_pResourceClump->GetClump() ) == NULL)
		RpWorldAddClump(CNtlPLGlobal::m_pRpWorld, m_pResourceClump->GetClump()); 
}

void CNtlPLCharacter::RemoveWorld(void)
{
	if( m_pResourceClump == NULL ||
		CNtlPLGlobal::m_pRpWorld == NULL)
		return;

	if(RpClumpGetWorld( m_pResourceClump->GetClump() ) != NULL)
		RpWorldRemoveClump(CNtlPLGlobal::m_pRpWorld, m_pResourceClump->GetClump());
}

RwBool CNtlPLCharacter::IsExistWorld()
{
	if( m_pResourceClump == NULL )
		return FALSE;

	if( RpClumpGetWorld( m_pResourceClump->GetClump() ) != NULL)
		return TRUE;

	return FALSE;
}

RpClump	*CNtlPLCharacter::GetClump() 
{ 
	if( m_pResourceClump == NULL) 
		return NULL; 

	return m_pResourceClump->GetClump();
}

RpHAnimHierarchy *CNtlPLCharacter::GetBaseHierarchy() 
{ 
	return m_pBaseHierarchy;
}


RpAtomic* CNtlPLCharacter::GetAtomic( const std::string& strName ) 
{   
    std::string strAtomicName;
    for each(SEntityAtomicData* pAtomicData in m_vecAtomicList)
    {
        strAtomicName = Helper_AtomicName(pAtomicData->pData);
        if(strName == strAtomicName)
            return pAtomicData->pData;
    }

    return NULL;
}

/**
* pPos위치에 Character를 위치 시킨다.
* \param pPos World에서의 위치
*/
void CNtlPLCharacter::SetPosition(const RwV3d *pPos)
{
	memcpy(&m_vCurPos, pPos, sizeof(RwV3d));

	if( !GetCharScheduleResInfo()->bLoadComplete )
		return;

	NTL_ASSERTE(m_pResourceClump);
	if(m_pResourceClump == NULL)
		return;

	RwFrame* pFrame;
	pFrame = RpClumpGetFrame(m_pResourceClump->GetClump());

	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vXAxisV3, m_vAngle.x,rwCOMBINEREPLACE);
	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, m_vAngle.y,rwCOMBINEPOSTCONCAT);
	if(m_pTypeBoneData)
	{
		if(m_pTypeBoneData->bBaseScale)
			RwFrameScale(pFrame, &m_pTypeBoneData->vBaseScale, rwCOMBINEPOSTCONCAT);

		RwFrameScale(pFrame, &m_vScale, rwCOMBINEPOSTCONCAT);

		if(m_pTypeBoneData->bBoneScale)
			RwFrameTranslate(pFrame, &m_pTypeBoneData->vPosOffset, rwCOMBINEPOSTCONCAT);
	}

	RwFrameTranslate(pFrame, &m_vCurPos, rwCOMBINEPOSTCONCAT);

	RwFrameUpdateObjects( pFrame );

	// Sound의 위치도 변경한다.
	ListSoundHandle::iterator it = m_listLoopSound.begin();
	for(; it != m_listLoopSound.end(); ++it)
	{
		SOUND_HANDLE hSound = *it;
		GetSoundManager()->SetSoundPosition(hSound, m_vCurPos.x, m_vCurPos.y, m_vCurPos.z);
	}

	// Position이 변경이 될 경우에 위치는 변경이 되지 않는다.(이거 Test를 좀 해봐야 할 듯 하다 by HoDong)
	//CNtlPLAttach::Update(0.f);
}


/**
* Base Scale를 조정을 한다.
* \param fScale Default값은 1.f 이다.
*/
void CNtlPLCharacter::SetScale(RwReal fScale)
{
	m_vScale.x = fScale;
	m_vScale.y = fScale;
	m_vScale.z = fScale;

	if( !GetCharScheduleResInfo()->bLoadComplete )
		return;

	NTL_ASSERTE(m_pResourceClump);
	if(m_pResourceClump == NULL)
		return;

	RwFrame* pFrame;
	pFrame = RpClumpGetFrame(m_pResourceClump->GetClump());

	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vXAxisV3, m_vAngle.x,rwCOMBINEREPLACE);
	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, m_vAngle.y,rwCOMBINEPOSTCONCAT);
	if(m_pTypeBoneData)
	{
		if(m_pTypeBoneData->bBaseScale)
			RwFrameScale(pFrame, &m_pTypeBoneData->vBaseScale, rwCOMBINEPOSTCONCAT);

		RwFrameScale(pFrame, &m_vScale, rwCOMBINEPOSTCONCAT);

		if(m_pTypeBoneData->bBoneScale)
			RwFrameTranslate(pFrame, &m_pTypeBoneData->vPosOffset, rwCOMBINEPOSTCONCAT);
	}
	RwFrameTranslate(pFrame, &m_vCurPos, rwCOMBINEPOSTCONCAT);
}

RwReal CNtlPLCharacter::GetScale() 
{   
	return m_vScale.x;
}


/**
* Base Scale를 조정을 한다.
* \param fScale Default값은 1.f 이다.
*/
void CNtlPLCharacter::SetBaseScale(RwReal fBaseScale)
{
	if(m_pTypeBoneData)
	{
		m_pTypeBoneData->vBaseScale.x = fBaseScale;
		m_pTypeBoneData->vBaseScale.y = fBaseScale;
		m_pTypeBoneData->vBaseScale.z = fBaseScale;
	}

	if( !GetCharScheduleResInfo()->bLoadComplete )
		return;

	NTL_ASSERTE(m_pResourceClump);
	if( m_pResourceClump == NULL)
		return;

	RwFrame* pFrame;
	pFrame = RpClumpGetFrame(m_pResourceClump->GetClump());

	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vXAxisV3, m_vAngle.x,rwCOMBINEREPLACE);
	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, m_vAngle.y,rwCOMBINEPOSTCONCAT);
	if(m_pTypeBoneData)
	{
		if(m_pTypeBoneData->bBaseScale)
			RwFrameScale(pFrame, &m_pTypeBoneData->vBaseScale, rwCOMBINEPOSTCONCAT);

		RwFrameScale(pFrame, &m_vScale, rwCOMBINEPOSTCONCAT);

		if(m_pTypeBoneData->bBoneScale)
			RwFrameTranslate(pFrame, &m_pTypeBoneData->vPosOffset, rwCOMBINEPOSTCONCAT);
	}
	RwFrameTranslate(pFrame, &m_vCurPos, rwCOMBINEPOSTCONCAT);
}

/**
* 프로퍼티에 설정된 BaseScale을 반환한다.
*/
RwReal CNtlPLCharacter::GetBaseScale()
{
	if(m_pTypeBoneData)
	{
		return m_pTypeBoneData->vBaseScale.x;
	}
	else
	{
		return 1.0f;
	}
}

/**
* 발이 땅에서 떨어질 경우 위치값이다.
* \param pPos 이동시킬 위치이다.
*/
void CNtlPLCharacter::SetPosOffset(RwV3d *pPos)
{
	NTL_ASSERTE(pPos);
	NTL_ASSERTE(m_pTypeBoneData);

	m_pTypeBoneData->vPosOffset.x = pPos->x;
	m_pTypeBoneData->vPosOffset.y = pPos->y;
	m_pTypeBoneData->vPosOffset.z = pPos->z;

	RwFrame* pFrame;
	pFrame = RpClumpGetFrame(m_pResourceClump->GetClump());

	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vXAxisV3, m_vAngle.x,rwCOMBINEREPLACE);
	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, m_vAngle.y,rwCOMBINEPOSTCONCAT);
	if(m_pTypeBoneData)
	{
		if(m_pTypeBoneData->bBaseScale)
			RwFrameScale(pFrame, &m_pTypeBoneData->vBaseScale, rwCOMBINEPOSTCONCAT);

		RwFrameScale(pFrame, &m_vScale, rwCOMBINEPOSTCONCAT);

		if(m_pTypeBoneData->bBoneScale)
			RwFrameTranslate(pFrame, &m_pTypeBoneData->vPosOffset, rwCOMBINEPOSTCONCAT);
	}

	RwFrameTranslate(pFrame, &m_vCurPos, rwCOMBINEPOSTCONCAT);

	int test = 0;
}

/**
* Character의 회전
* \param fAngleY 회전값
*/
void CNtlPLCharacter::SetAngleY(const RwReal fAngleY)
{
	m_vAngle.y = fAngleY;

	if( !GetCharScheduleResInfo()->bLoadComplete )
		return;

	NTL_ASSERTE(m_pResourceClump);
	if(m_pResourceClump == NULL)
		return;

	RwFrame* pFrame;
	pFrame = RpClumpGetFrame(m_pResourceClump->GetClump());
	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vXAxisV3, m_vAngle.x,rwCOMBINEREPLACE);
	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, m_vAngle.y,rwCOMBINEPOSTCONCAT);

	if(m_pTypeBoneData)
	{
		if(m_pTypeBoneData->bBaseScale)
			RwFrameScale(pFrame, &m_pTypeBoneData->vBaseScale, rwCOMBINEPOSTCONCAT);

		RwFrameScale(pFrame, &m_vScale, rwCOMBINEPOSTCONCAT);

		if(m_pTypeBoneData->bBoneScale)
			RwFrameTranslate(pFrame, &m_pTypeBoneData->vPosOffset, rwCOMBINEPOSTCONCAT);
	}

	RwFrameTranslate(pFrame, &m_vCurPos, rwCOMBINEPOSTCONCAT);
}

void CNtlPLCharacter::SetAngleX(const RwReal fAngleX)
{
	m_vAngle.x = fAngleX;
	if( !GetCharScheduleResInfo()->bLoadComplete )
		return;

	NTL_ASSERTE(m_pResourceClump);
	if(m_pResourceClump == NULL)
		return;

	RwFrame* pFrame;
	pFrame = RpClumpGetFrame(m_pResourceClump->GetClump());
	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vXAxisV3, m_vAngle.x,rwCOMBINEREPLACE);
	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, m_vAngle.y,rwCOMBINEPOSTCONCAT);


	if(m_pTypeBoneData)
	{
		if(m_pTypeBoneData->bBaseScale)
			RwFrameScale(pFrame, &m_pTypeBoneData->vBaseScale, rwCOMBINEPOSTCONCAT);

		RwFrameScale(pFrame, &m_vScale, rwCOMBINEPOSTCONCAT);

		if(m_pTypeBoneData->bBoneScale)
			RwFrameTranslate(pFrame, &m_pTypeBoneData->vPosOffset, rwCOMBINEPOSTCONCAT);
	}

	RwFrameTranslate(pFrame, &m_vCurPos, rwCOMBINEPOSTCONCAT);
}

/**
* 현재 Angle에서 fAngleDeltaY값 만큼을 더한다.
* \param fAngleDeltaY Delta 회전값
*/
void CNtlPLCharacter::SetAngleDelta(const RwReal fAngleDeltaY)
{
	m_vAngle.y = m_vAngle.y + fAngleDeltaY;
	if( !GetCharScheduleResInfo()->bLoadComplete )
		return;
	SetAngleY(m_vAngle.y);
}

/**
* 특정 Bone의 Matrix 값을 얻는다.
* \param pBoneName Bone의 이름
*/
RwMatrix* CNtlPLCharacter::GetBoneMatrix(const RwChar *pBoneName)
{
	if( !GetCharScheduleResInfo()->bLoadComplete )
		return NULL;

	RwFrame *pFrame = GetBoneByName(pBoneName);
	if(pFrame != NULL)
		return &pFrame->ltm;

	return NULL;
}

RwFrame *CNtlPLCharacter::GetBoneByName(const RwChar *pBoneName)
{
	if( !GetCharScheduleResInfo()->bLoadComplete )
		return NULL;

	if(m_mapFrame.find( pBoneName ) == m_mapFrame.end())
		return NULL;
	RwFrame *pFrame = m_mapFrame[pBoneName];

	if(pFrame != NULL)
		return pFrame;

	return NULL;
}

RwUInt32 CNtlPLCharacter::GetBoneIndex( const RwChar* pBoneName ) 
{
    std::string strBoneName = pBoneName;
    std::string strMapBoneName;
    for(int i = 0; i < m_nBoneCount; ++i)
    {
        strMapBoneName = GetBoneName(i);
        if(strMapBoneName == strBoneName)
            return i;
    }

    return 0;
}

RwV3d CNtlPLCharacter::GetPosition()  
{ 
	if( !GetCharScheduleResInfo()->bLoadComplete )
	{
		return m_vCurPos;
	}

	RwFrame *pFrame = RpClumpGetFrame(m_pResourceClump->GetClump());
	NTL_ASSERTE(pFrame);
	return pFrame->modelling.pos;
}

void CNtlPLCharacter::SetDirection(const RwV3d *pDir)
{
	if( !GetCharScheduleResInfo()->bLoadComplete )
		return;

	RwReal fAxisAngleX, fAxisAngleY;
	CNtlMath::LineToAngleXY(pDir, fAxisAngleX, fAxisAngleY);

	m_vAngle.x = fAxisAngleX + m_vAngle.x;
	m_vAngle.y = fAxisAngleY + m_vAngle.y;

	SetAngleX(m_vAngle.x);
	SetAngleY(m_vAngle.y);
}

RwV3d CNtlPLCharacter::GetDirection() 
{ 
	if( !GetCharScheduleResInfo()->bLoadComplete )
	{
		return m_vCurPos;
	}

	RwFrame *pFrame = RpClumpGetFrame(m_pResourceClump->GetClump()); 
	NTL_ASSERTE(pFrame);
	return pFrame->modelling.at;
}

RwV3d CNtlPLCharacter::GetRight()     
{ 
	if( !GetCharScheduleResInfo()->bLoadComplete )
	{
		return m_vCurPos;
	}

	RwFrame *pFrame = RpClumpGetFrame(m_pResourceClump->GetClump()); 
	NTL_ASSERTE(pFrame);
	return pFrame->modelling.right;
}

RwV3d CNtlPLCharacter::GetUp() 
{
	if( !GetCharScheduleResInfo()->bLoadComplete )
	{
		return YAxis;
	}

	RwFrame *pFrame = RpClumpGetFrame(m_pResourceClump->GetClump()); 
	NTL_ASSERTE(pFrame);
	return pFrame->modelling.up;
}

RwFrame *CNtlPLCharacter::GetFrame()
{

	//NTL_ASSERTE(m_pResourceClump);
	if(m_pResourceClump == NULL)
		return NULL;

	return RpClumpGetFrame(m_pResourceClump->GetClump());
}

RwMatrix& CNtlPLCharacter::GetMatrix()
{
	RwFrame *pFrame = GetFrame();

	if(pFrame == NULL)
	{
		return CNtlPLEntity::GetMatrix();
	}

	return *RwFrameGetMatrix(pFrame);
}

void CNtlPLCharacter::SetMatrix( RwMatrix& matWorld)
{
	RwFrame *pFrame = GetFrame();
    if(!pFrame)
        return;

    RwMatrix* pMatChar = RwFrameGetMatrix(pFrame);
    if(!pMatChar)
        return;

	RwMatrixCopy(pMatChar, &matWorld);
    pMatChar->pos = ZeroAxis;
    
    // Base Scale을 적용해줘야만 한다.
    RwFrameScale(pFrame, &m_pTypeBoneData->vBaseScale, rwCOMBINEPOSTCONCAT);
    RwFrameTranslate(pFrame, RwMatrixGetPos(&matWorld), rwCOMBINEPOSTCONCAT);

	RwFrameUpdateObjects(pFrame);
}


void CNtlPLCharacter::SetAnimSpeed(RwReal fSpeed)
{
	m_fAnimSpeed = fSpeed;   
}

RwReal CNtlPLCharacter::GetAnimSpeed()
{
	return m_fAnimSpeed;
}

void CNtlPLCharacter::SetAnimUpdate(RwBool bAnimUpdate)
{
	m_bAnimUpdate = bAnimUpdate;
}


RwBool CNtlPLCharacter::GetAnimUpdate()
{
	return m_bAnimUpdate;
}

RwReal CNtlPLCharacter::GetBaseCurrentAnimTime()
{
	return m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].GetCurrentAnimTime();
}

RwReal CNtlPLCharacter::GetBaseDurationAnimTime()
{
	return m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].GetDurationAnimTime();

}

void CNtlPLCharacter::SetBaseCurrentAnimTime(RwReal fCurrTime)
{
	m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].SetCurrentAnimTime(fCurrTime);
}

RwBool CNtlPLCharacter::GetBaseCurrentAnimEnd()
{
	return m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].GetCurrentAnimEnd();
}

RwBool CNtlPLCharacter::SetChangeEquipCItem(CNtlPLItem* _pItem, RwBool _bAttach, RwUInt32 _CBitSet)
{
	if(m_pEquipItem)
	{
		m_pEquipItem->AddCItem(_pItem, _bAttach, _CBitSet);

		// Cz : Character Atomic List
// 		DestroyOccluderProxy();
// 
// 		if(m_pResourceClump)
// 		{
// 			Helper_DestroyAllAtomics(&m_vecAtomicList);
// 			Helper_SetClumpAllAtomics(m_pResourceClump->GetClump(), &m_vecAtomicList);
// 		}
// 		CreateOccluderProxy();

		return TRUE;
	}

	return FALSE;
}

RwBool CNtlPLCharacter::SetRemoveEquipCItem(CNtlPLItem* _pItem, RwUInt32 _CBitSet)
{
	if(m_pEquipItem)
	{
		m_pEquipItem->RemoveCItem(_pItem, _CBitSet);

		// Cz : Character Atomic List
// 		DestroyOccluderProxy();
// 		if(m_pResourceClump)
// 		{
// 			Helper_DestroyAllAtomics( &m_vecAtomicList );
// 			Helper_SetClumpAllAtomics( m_pResourceClump->GetClump(), &m_vecAtomicList);
// 		}
// 		CreateOccluderProxy();

		return TRUE;
	}

	return FALSE;
}

RwBool CNtlPLCharacter::SetChangeEquipItem(CNtlPLItem *pItem, RwBool bAttach, RwUInt32 _CBitSet/* = dCOSTUME_BITSET_INVALID*/)
{
	if(_CBitSet != dCOSTUME_BITSET_INVALID)
	{
		SetChangeEquipCItem(pItem, bAttach, _CBitSet);

		return TRUE;
	}

	if(m_pEquipItem != NULL)
	{
		m_pEquipItem->AddItem(pItem, bAttach);

		// Cz : Character Atomic List
// 		DestroyOccluderProxy();
// 		if(m_pResourceClump)
// 		{
// 			Helper_DestroyAllAtomics( &m_vecAtomicList );
// 			Helper_SetClumpAllAtomics( m_pResourceClump->GetClump(), &m_vecAtomicList);
// 		}
// 		CreateOccluderProxy();
		
		return TRUE;
	}

	return FALSE;
}

RwBool CNtlPLCharacter::SetRemoveEquipItem(CNtlPLItem *pItem, RwUInt32 _CBitSet/* = dCOSTUME_BITSET_INVALID*/)
{
	if(_CBitSet != dCOSTUME_BITSET_INVALID)
	{
		SetRemoveEquipCItem(pItem, _CBitSet);

		return TRUE;
	}

	if(m_pEquipItem != NULL)
	{
		m_pEquipItem->RemoveItem(pItem);

		// Cz : Character Atomic List
// 		DestroyOccluderProxy();
// 		if(m_pResourceClump)
// 		{
// 			Helper_DestroyAllAtomics( &m_vecAtomicList );
// 			Helper_SetClumpAllAtomics( m_pResourceClump->GetClump(), &m_vecAtomicList);
// 		}
// 		CreateOccluderProxy();

		return TRUE;
	}

	return FALSE;
}

void CNtlPLCharacter::UpdateCharacterAtomicList()
{
	DestroyOccluderProxy();
	if(m_pResourceClump)
	{
		Helper_DestroyAllAtomics( &m_vecAtomicList );
		Helper_SetClumpAllAtomics( m_pResourceClump->GetClump(), &m_vecAtomicList);
	}
	CreateOccluderProxy();
}									  

VOID CNtlPLCharacter::SetToonEdgeVisibility4CurEquipItem(RwBool _Visibility)
{
	if(m_pEquipItem)
	{
		m_pEquipItem->SetToonEdgeVisibility4CurEquipItem(_Visibility);
	}
}

/**
* CallBack function that is called when character is rendered
* Equip Item also calls this function.
* \ param pAtomic:
*/
RpAtomic *CNtlPLCharacter::RenderCallBack(RpAtomic *pAtomic)
{
	CNtlPLEntity *pEntity = (CNtlPLEntity*)RpNtlAtomicGetData(pAtomic);
	if(pEntity)
	{
		if(pEntity->GetClassType() == PLENTITY_ITEM)
		{
			CNtlPLItem*	pItemEntity = (CNtlPLItem*)pEntity;

            // No scouting options
            if(CNtlPLGlobal::m_bRenderScouter == FALSE && pItemEntity->GetEquipSlotType() == ITEM_EQUIP_R_EYE)            
            {
                return pAtomic;
            }

			RwTexture*	pTexture	= pItemEntity->GetBaseTexture();
			RwTexture*	pUVTexture	= pItemEntity->GetUVTexture();
			
			pItemEntity->SetEMUVController(TRUE);
			pItemEntity->CreateSimpleMaterial(); 
			pItemEntity->CreatePetrifyMaterial();

			RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);
			if(pGeom)
			{
				RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);
				for(int i = 0; i < nMatCount; i++)
				{
					RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
					if(pMaterial)
					{
						CNtlPLCharacter *pCharEntity = pItemEntity->GetOwner();

						// get through the proper renderpipe
						RpNtlMaterialSetRenderCB(pMaterial, CNtlPLCharacter::fpRenderCB);

						if(pTexture)
							RpMaterialSetTexture(pMaterial, pTexture);

						if(pUVTexture)
						{
							RpNtlMaterialExtSetUVTransform(pMaterial, pItemEntity->GetUVMatrix() );
							RpNtlMaterialExtSetUVTexture(pMaterial, pUVTexture);
						}

						// DOGI emblem
						if((NtlMatExtGetFlag(pMaterial) & rpNTL_MATERIAL_EMBLEM) && !pCharEntity->GetPetrifyMaterial())
						{
							RpNtlMaterialExtSetEmblemTex(pMaterial, pItemEntity->GetDogiEmblem());
							RpNtlMaterialExtSetDogiColor(pMaterial, pItemEntity->GetDogiEditedColor());
						}

						// DOGI pants
						if((NtlMatExtGetFlag(pMaterial) & rpNTL_MATERIAL_DOGI_PANTS) && !pCharEntity->GetPetrifyMaterial())
						{
							RpNtlMaterialExtSetDogiColor(pMaterial, pItemEntity->GetDogiEditedColor());
						}

						// DOGI skirt
						if((NtlMatExtGetFlag(pMaterial) & rpNTL_MATERIAL_DOGI_SKIRT) && !pCharEntity->GetPetrifyMaterial())
						{
							RpNtlMaterialExtSetDogiColor(pMaterial, pItemEntity->GetDogiEditedColor());
						}

						// COSTUME EMBLEM ( TO DO )
						if ((NtlMatExtGetFlag(pMaterial) & rpNTL_MATERIAL_COSTUME_EMBLEM) && !pCharEntity->GetPetrifyMaterial())
						{
							
						}

						// color
						RwRGBAReal sColorReal;
						RwRGBARealFromRwRGBA(&sColorReal, pItemEntity->GetColor());						

						const D3DCAPS9* pD3DCaps = (const D3DCAPS9*)RwD3D9GetCaps();
						if(pD3DCaps->MaxSimultaneousTextures <= 4)
						{
							sColorReal.red		= 1.0f;
							sColorReal.green	= 1.0f;
							sColorReal.blue		= 1.0f;
						}

						if(pCharEntity != NULL)
						{
                            if(NtlMatExtGetFlag(pMaterial) & rpNTL_MATERIAL_SKIN_COLOR)
                            {
                                RwRGBAReal sSkinColorReal;
                                RwRGBARealFromRwRGBA(&sSkinColorReal, pCharEntity->GetSkinColor());

                                RwRGBA sColor;
                                sColor.red		= RwUInt8((sSkinColorReal.red) * 255.f);
                                sColor.green	= RwUInt8((sSkinColorReal.green) * 255.f);
                                sColor.blue		= RwUInt8((sSkinColorReal.blue) * 255.f);
                                sColor.alpha	= pCharEntity->GetColor()->alpha;
                                RpNtlMaterialExtSetSkinColor(pMaterial, &sColor);

                                sColorReal.alpha = pCharEntity->GetColor()->alpha / 255.0f;
                            }
							else if((NtlMatExtGetFlag(pMaterial) & rpNTL_MATERIAL_HEAD_COLOR) && !pCharEntity->GetPetrifyMaterial())
							{
								RwRGBAReal	sHeadColorReal;
								RwRGBARealFromRwRGBA(&sHeadColorReal, pCharEntity->GetHeadColor());

								sColorReal.red		= sColorReal.red	* sHeadColorReal.red;
								sColorReal.green	= sColorReal.green	* sHeadColorReal.green;
								sColorReal.blue		= sColorReal.blue	* sHeadColorReal.blue;
							}
							else if((NtlMatExtGetFlag(pMaterial) & rpNTL_MATERIAL_BELT_COLOR) && !pCharEntity->GetPetrifyMaterial())
							{
								RwRGBAReal DogiClrReal;
								RwRGBARealFromRwRGBA(&DogiClrReal, pItemEntity->GetDogiBeltColor());

								sColorReal.red		= sColorReal.red	* DogiClrReal.red;
								sColorReal.green	= sColorReal.green	* DogiClrReal.green;
								sColorReal.blue		= sColorReal.blue	* DogiClrReal.blue;
							}
							else
							{
								sColorReal.alpha = static_cast<RwReal>(pCharEntity->GetColor()->alpha / 255.0f);
							}

							// to highlight a character while selecting
							RpNtlMaterialExtSetAddColor(pMaterial, pCharEntity->GetAddColor());
						}
						else
						{
							// to highlight a character while selecting
							RpNtlMaterialExtSetAddColor(pMaterial, pItemEntity->GetAddColor());
						}

						RwRGBA sColor;
						sColor.red		= RwUInt8((sColorReal.red) * 255.f);
						sColor.green	= RwUInt8((sColorReal.green) * 255.f);
						sColor.blue		= RwUInt8((sColorReal.blue) * 255.f);
						sColor.alpha	= RwUInt8((sColorReal.alpha) * pItemEntity->GetWeightAlpha() * 255.f);
						RpMaterialSetColor(pMaterial, &sColor);
					}
				}
			}

			pTexture = pItemEntity->GetMultiTexture();
			if(pTexture)
			{
				if(pGeom)
				{
					RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, 0);

					if(pMaterial)
						NtlMatExtSetMultiTexture(pMaterial, pTexture);
				}
			}

			// 형석 coding
			if(m_SkipAdge)
			{
				AtomicDefaultRenderCallBack(pAtomic);
			}
			else
			{
				SToonData *pToonData = pItemEntity->GetToonData();
				if(pToonData->pToonInk)
				{
					Helper_SetToonAtomicRender( pAtomic, pToonData);
				}

				if(RpNtlAtomicGetFlag(pAtomic) & NTL_TOON_EDGE_OFF)
				{
					AtomicDefaultRenderCallBack(pAtomic);
				}
				else
				{
					RpNtlAtomicGetRenderCallBack(pAtomic)(pAtomic);
				}
			}

			pItemEntity->SetEMUVController(FALSE);
			pItemEntity->RestoreSimpleMaterial();
			pItemEntity->RestorePetrifyMaterial();
		}
		// render MOB, NPC; CNtlPLCharacter::m_pResourceClump
		else if(pEntity->GetClassType() == PLENTITY_CHARACTER)
		{
			CNtlPLCharacter*	pCharEntity = (CNtlPLCharacter *)pEntity;
			RpGeometry*			pGeom		= RpAtomicGetGeometry(pAtomic);
			RwBool				SEMActFlag	= RpNtlAtomicGetEnvTexName(pAtomic);

			if(pGeom)
			{
				RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);
				for(int i = 0; i < nMatCount; i++)
				{
					RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
					if(pMaterial)
					{				
						// For MOB, NPC, VEHECLE spherical enviroment mapping
						if(SEMActFlag)
							RpNtlMaterialExtSetEnvMapTexture(pMaterial, pCharEntity->GetSEM());

						// simple material
						if(pCharEntity->GetSimpleMaterial())
							RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) | rpNTL_MATERIAL_SIMPLEMATERIAL);
						else
							RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) & ~rpNTL_MATERIAL_SIMPLEMATERIAL);
						
						// petrify
						if(pCharEntity->GetPetrifyMaterial())
							RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) | rpNTL_MATERIAL_PETRIFY);
						else
							RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) & ~rpNTL_MATERIAL_PETRIFY);

						// get through the proper renderpipe
						RpNtlMaterialSetRenderCB(pMaterial, CNtlPLCharacter::fpRenderCB);

						RwRGBAReal	sColorReal;
						RwRGBARealFromRwRGBA(&sColorReal, pCharEntity->GetColor());

						// Atomic에 설정된 Alpha 값
						RwReal fAtomicAlpha = RpNtlAtomicGetAlpha(pAtomic) / 255.0f;

						RwRGBA sColor;
						sColor.red		= RwUInt8((sColorReal.red)*255.f);
						sColor.green	= RwUInt8((sColorReal.green)*255.f);
						sColor.blue		= RwUInt8((sColorReal.blue)*255.f);
						sColor.alpha	= RwUInt8((sColorReal.alpha)*255.f*fAtomicAlpha);

						RpMaterialSetColor(pMaterial, &sColor);
						RpNtlMaterialExtSetAddColor(pMaterial, pCharEntity->GetAddColor());

						pMaterial->color.alpha = sColor.alpha;

                        if(NtlMatExtGetFlag(pMaterial) & rpNTL_MATERIAL_SKIN_COLOR)
                        {
                            RwRGBAReal sSkinColorReal;
                            RwRGBARealFromRwRGBA(&sSkinColorReal, pCharEntity->GetSkinColor());

                            sColor.red		= RwUInt8((sSkinColorReal.red) * 255.f);
                            sColor.green	= RwUInt8((sSkinColorReal.green) * 255.f);
                            sColor.blue		= RwUInt8((sSkinColorReal.blue) * 255.f);
                            sColor.alpha	= pCharEntity->GetColor()->alpha;
                            RpNtlMaterialExtSetSkinColor(pMaterial, &sColor);                            
                        }
					}
				}
			}

			// 형석 coding
			if(m_SkipAdge)
			{
				AtomicDefaultRenderCallBack(pAtomic);
			}
			else
			{
				SToonData *pToonData = pCharEntity->GetToonData();
				if(pToonData->pToonInk)
				{
					Helper_SetToonAtomicRender( pAtomic, pToonData);
				}

				if(RpNtlAtomicGetFlag(pAtomic) & NTL_TOON_EDGE_OFF)
				{
					AtomicDefaultRenderCallBack(pAtomic);
				}
				else
				{
					RpNtlAtomicGetRenderCallBack(pAtomic)(pAtomic);
				}
			}

			// For MOB, NPC, VEHECLE spherical enviroment mapping
			if(SEMActFlag)
			{
				RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);
				for(int i = 0; i < nMatCount; i++)
				{
					RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
					if(pMaterial)
					{				
						RpNtlMaterialExtSetEnvMapTexture(pMaterial, NULL);
					}
				}
			}
		}
	}

	return pAtomic;
}

int CNtlPLCharacter::CallBackBaseAnim(void* pEventData)
{
	SEventAnim *pEventAnim = (SEventAnim *)pEventData;

	switch(pEventAnim->eEventID)
	{
	case EVENT_ANIM_END:
		OnEventAnimEnd((SEventAnimEnd*)pEventData);
		break;		
	case EVENT_ANIM_HIT:
		OnEventHit((SEventAnimHit*)pEventData);
		break;
	case EVENT_ANIM_WEIGHT_TIME:
		OnEventWeightTime((SEventWeightTime*)pEventData);
		break;
	case EVENT_ANIM_FOOT_STEP:
		OnEventFootStep((SEventFootStep*)pEventData);
		break;
	case EVENT_ANIM_VISUAL_EFFECT:
		OnEventVisualEffect((SEventVisualEffect*)pEventData);
		break;
	case EVENT_ANIM_VISUAL_SOUND:
		OnEventVisualSound((SEventSound*)pEventData);
		break;
	case EVENT_ANIM_TRACE_EFFECT:
		OnEventTraceEffect((SEventTrace*)pEventData);
		break;
	case EVENT_ANIM_SUB_WEAPON:
		OnEventSubWeapon((SEventSubWeapon*)pEventData);
		break;            
	case EVENT_ANIM_POST_EFFECT:
		OnEventPostEffect((SEventPostEffect*)pEventData);
		break;
	case EVENT_ANIM_SUMMON_PET:
		OnEventSummonPet((SEventSummonPet*)pEventData);
		break;
	case EVENT_ANIM_ALPHA:
		OnEventAlphaFade((SEventAlpha*)pEventData);
		break;
	case EVENT_ANIM_DIRECT:
		OnEventDirect((SEventDirect*)pEventData);
		break;
    case EVENT_ANIM_COLOR_CHANGE:
        OnEventColorChange((SEventColorChange*)pEventData);
        break;
    case EVENT_ANIM_STRETCH:
		if(m_sScheduleResInfo.uiRace == RACE_NAMEK) // by daneos: temp fix to avoid other than namek have pull skill.
			OnEventStretch((SEventStretch*)pEventData);
        break;
    case EVENT_ANIM_TRIGGER:
        OnEventTrigger((SEventTrigger*)pEventData);
        break;
    case EVENT_ANIM_SKILL_CANCEL:
        OnEventSkillCancel((SEventSkillCancel*)pEventData);
        break;
	default:
		NTL_ASSERT(TRUE, "Not EventID");
		break;        
	}
	return TRUE;
}

void CNtlPLCharacter::OnEventTraceEffect( SEventTrace* pEventTrace ) 
{
	if(!pEventTrace)
		return;

	if(pEventTrace->eAttachType == SEventTrace::CHARACTER_BONE)
	{
        CNtlInstanceTraceSystem* pTraceSystem = NTL_NEW CNtlInstanceTraceSystem();
        pTraceSystem->Create(pEventTrace, this);

		CNtlInstanceEffect* pEffect = NTL_NEW CNtlInstanceEffect();       
		pEffect->SetName(EFFECT_NAME_TRACE);
		pEffect->SetFlags(NTL_PLEFLAG_NOT_PROPERTY_USED);        
		pEffect->AddComponentSystem(pTraceSystem);
        Attach(pEffect);

		GetSceneManager()->AddPLEntity(pEffect);
		
	}
	else
	{
		for each(CNtlPLItem* pItem in *(m_pEquipItem->GetEquipItemList()))
		{
			if(!pItem)
				continue;

			pItem->ActiveTraceEffect(pEventTrace);                    
		}        
	}
}

void CNtlPLCharacter::OnEventVisualEffect( SEventVisualEffect* pEventVisualEffect ) 
{
	//Effect Name이 없는 경우는 무조건 Return을 한다.
	if( strlen(pEventVisualEffect->chEffectName) <= 0)
		return;

	if( strlen(pEventVisualEffect->chBoneName) <= 0 &&
		pEventVisualEffect->bAttachBone)
		return;

	if(!pEventVisualEffect->bAttach || pEventVisualEffect->eBoneType == BONE_CHARACTER)
	{
		// 만약 LoopEffect 리스트에 같은 이름,Bone이 있으면 새로 생성하지 않는다.
		if(IsExistLoopEffect(pEventVisualEffect->chEffectName, pEventVisualEffect->chBoneName))
			return;

		CNtlPLEntity *pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pEventVisualEffect->chEffectName);
		NTL_ASSERT(pPLEntity, __FUNCTION__ << "pPLEntity == NULL");
		if(pPLEntity == NULL)
			return;

		CNtlInstanceEffect *pInstanceEffect = (CNtlInstanceEffect *)pPLEntity;

		// 이펙트의 Scale 적용 유무 플래그를 설정한다.
		pInstanceEffect->SetApplyScale(pEventVisualEffect->bApplyScale);
		pPLEntity->SetScale(GetBaseScale() * m_vScale.x);            

		//Effect에 Character의 Serial ID를 넣는다.(Client에서 사용을 하기 위해서)
		pPLEntity->SetSerialID(GetSerialID());

		pInstanceEffect->SetPlayAnimSpeed(m_fAnimSpeed);

		// AutoDelete가 아니면 LoopEffect라고 간주하고 리스트에 추가한다              
		if(!pPLEntity->IsAutoDelete())
		{
			SLoopEffect* pLoopEffect = NTL_NEW SLoopEffect();
			pLoopEffect->pLoopEffectIntance = pInstanceEffect;
			pLoopEffect->szBoneName = pEventVisualEffect->chBoneName;

			AddLoopEffect(pLoopEffect);
		}

        if(!pEventVisualEffect->bAttach)
        {
            RwV3d vPos = GetPosition() + pEventVisualEffect->vOffSetPos;
            pInstanceEffect->SetPosition(&vPos);

            pInstanceEffect->SetVisible(IsVisible());
        }
		else if( pEventVisualEffect->bAttachBone)				
		{
			if(pEventVisualEffect->bProjectileType)
			{
				//  발사체 타입인경우에는 Attach 하지 않는다.
				RwMatrix* pMatBone = GetBoneMatrix(pEventVisualEffect->chBoneName);
				RwMatrix matEffect;
				RwMatrixSetIdentity(&matEffect);
				*RwMatrixGetPos(&matEffect) = *RwMatrixGetPos(pMatBone);
				*RwMatrixGetAt(&matEffect) = GetDirection();
				*RwMatrixGetUp(&matEffect) = GetUp();
				*RwMatrixGetRight(&matEffect) = GetRight();
				RwMatrixUpdate(&matEffect);

				pInstanceEffect->SetMatrix(matEffect);
			}
			else
			{
                SPLAttachAttr attachAttr;
                attachAttr.vOffsetPos = pEventVisualEffect->vOffSetPos;        

				AttachBone((CNtlPLAttach *)pPLEntity, pEventVisualEffect->chBoneName, &attachAttr);
			}
		}
		else
		{
			AttachWorldPosAndRotate((CNtlPLAttach *)pPLEntity, pEventVisualEffect->vOffSetPos);
		}
	}
	else if(pEventVisualEffect->eBoneType == BONE_WEAPON)
	{
		if(m_pEquipItem)
		{
			for each(CNtlPLItem* pItem in *(m_pEquipItem->GetEquipItemList()))
			{
				if(!pItem || pItem->GetProperty()->GetResType() != ITEM_RES_EQUIP_MAIN)
					continue;

				RwFrame* pFrame = pItem->GetBoneByName(pEventVisualEffect->chBoneName);                
				if(pFrame)
				{
					CNtlInstanceEffect* pInstanceEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pEventVisualEffect->chEffectName);
					if(!pInstanceEffect)
						return;

					pItem->AttachVisualEffect(pInstanceEffect, pEventVisualEffect);
					break;
				}
			}
		}
	}
	else if(pEventVisualEffect->eBoneType == BONE_SUB_WEAPON)
	{
		if(m_pEquipItem)
		{
			for each(CNtlPLItem* pItem in *(m_pEquipItem->GetEquipItemList()))
			{
				if(!pItem || pItem->GetProperty()->GetResType() != ITEM_RES_EQUIP_SUB)
					continue;

				RwFrame* pFrame = pItem->GetBoneByName(pEventVisualEffect->chBoneName);				
				if(pFrame)
				{
					CNtlInstanceEffect* pInstanceEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pEventVisualEffect->chEffectName);
					if(!pInstanceEffect)
						return;

					pItem->AttachVisualEffect(pInstanceEffect, pEventVisualEffect);
					break;
				}
			}
		}

	}
}

void CNtlPLCharacter::OnEventSubWeapon( SEventSubWeapon* pEventSubWeapon ) 
{
	switch(pEventSubWeapon->eSubWeaponActiveFlag)
	{
	case SUB_WEAPON_ACTIVE:
		{
			CNtlPLEventGenerator::AnimEventSubWeaponActive(GetSerialID());
			break;
		}
	case SUB_WEAPON_DEACTIVE:
		{
			CNtlPLEventGenerator::AnimEventSubWeaponDeActive(GetSerialID());
			break;
		}
	}
}

void CNtlPLCharacter::OnEventVisualSound( SEventSound* pEventSound ) 
{
	// Sound를 Play한다.    

	if(strlen(pEventSound->chSoundName) <= 1)
		return ;

	// LoopSound이고 이미 기존에 Play되고 있다면 Play하지 않는다.
	if(pEventSound->bLoop && IsExistLoopSound(pEventSound->chSoundName))
		return ;


	// 파일이 여러개 세팅되어 있는경우에는 랜덤으로 플레이된다.
	std::string soundFileName;
	int nMax = 1;
	if(strlen(pEventSound->chSoundName4) > 0)
		nMax = 4;
	else if(strlen(pEventSound->chSoundName3) > 0)
		nMax = 3;
	else if(strlen(pEventSound->chSoundName2) > 0)
		nMax = 2;

	if(nMax > 1)
	{
		int nRand = NtlRandomNumber(1, nMax);
		switch(nRand)
		{
		case 1:
			soundFileName = pEventSound->chSoundName;
			break;
		case 2:            
			soundFileName = pEventSound->chSoundName2;
			break;
		case 3:            
			soundFileName = pEventSound->chSoundName3;
			break;
		case 4:
			soundFileName = pEventSound->chSoundName4;
			break;
		}
	}
	else
	{
		soundFileName = pEventSound->chSoundName;
	}

	// 피치를 랜덤으로 선택한다
	RwReal fSoundPitch = NtlRandomNumber(pEventSound->fSoundPitchMin, pEventSound->fSoundPitchMax);

	sNtlSoundPlayParameta tSoundParam;
	tSoundParam.iChannelGroup	= pEventSound->eSoundType;
	tSoundParam.pcFileName		= (char*)soundFileName.c_str();
	tSoundParam.bLoop			= pEventSound->bLoop;
	tSoundParam.fVolume			= pEventSound->fSoundVolume * 0.01f;
	tSoundParam.fPitch			= fSoundPitch;
	tSoundParam.fXPos			= m_vCurPos.x;
	tSoundParam.fYPos			= m_vCurPos.y;
	tSoundParam.fZPos			= m_vCurPos.z;
	tSoundParam.fMinDistance	= pEventSound->fSoundDist;
	tSoundParam.fMaxDistance	= pEventSound->fSoundDecayDist;

	// by daneos test
	int iRet = GetSoundManager()->Play(&tSoundParam);

	// Loop Sound면 리스트에 추가한다.
	if(iRet == SOUNDRESULT_OK && pEventSound->bLoop && tSoundParam.hHandle != INVALID_SOUND_HANDLE)
	{
		AddLoopSound(tSoundParam.hHandle);
	}
}

void CNtlPLCharacter::OnEventWeightTime( SEventWeightTime* pEventTime ) 
{
	CNtlPLEventGenerator::AnimWeightTime(GetSerialID(), pEventTime);
}

void CNtlPLCharacter::OnEventAnimEnd( SEventAnimEnd* pEventAnimEnd ) 
{
	if(m_sBaseAnimPlayInfo.m_uiAnimKey == NML_SPAWN)
	{
		SetBaseAnimation(NML_IDLE_LOOP);
	}

	CNtlPLEventGenerator::AnimEnd(GetSerialID(), pEventAnimEnd->uiAnimKey, INVALID_GRAPHIC_ANIMATION_ID);
}

void CNtlPLCharacter::OnEventHit( SEventAnimHit* pEventHit ) 
{
	CNtlPLEventGenerator::AnimHit(GetSerialID(), pEventHit); 
}

void CNtlPLCharacter::OnEventPostEffect( SEventPostEffect* pEventPostEffect ) 
{
	// 포스트 이펙트는 타겟을 판별하고, 자신 이외의 다른 캐릭터의 이펙트를 보지 않기 위해서 Simul 레이어에서 처리한다.
	CNtlPLEventGenerator::AnimEventPostEffect(GetSerialID(), pEventPostEffect);
}

void CNtlPLCharacter::OnEventSummonPet( SEventSummonPet* pEventSummonPet ) 
{
	CNtlPLEventGenerator::AnimEventSummonActive(GetSerialID());
}

void CNtlPLCharacter::OnEventAlphaFade( SEventAlpha* pEventAlpha ) 
{
	// Atomic Alpha에 관해서만 PL단에서 처리한다.
	if(pEventAlpha->eAlphaEventType != SEventAlpha::E_ALPHA_EVENT_ATOMIC)
	{
		CNtlPLEventGenerator::AnimEventAlpha(GetSerialID(), (void*)pEventAlpha);
		return;
	}

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		if(Helper_GetBitFlag(pEventAlpha->bfAtomicList, i))
		{
			RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);
			RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag | NTL_RUNTIME_ALPHA);

			std::string strAtomicName = Helper_AtomicName(m_vecAtomicList[i]->pData);
			GetAlphaBlendController()->AddAtomicAlpha(strAtomicName.c_str(), 
				pEventAlpha->nStartAlpha / 255.0f, 
				pEventAlpha->nDestAlpha / 255.0f, 
				pEventAlpha->fFadeTime, 
				pEventAlpha->fLifeTime, FALSE);
		}        
	}
}

void CNtlPLCharacter::OnEventFootStep( SEventFootStep* pEventFootStep ) 
{
	CNtlPLEventGenerator::AnimFootStep(GetSerialID(), pEventFootStep);
}

void CNtlPLCharacter::OnEventDirect( SEventDirect* pEventDirect ) 
{
	CNtlPLEventGenerator::AnimEventDirect(GetSerialID(), pEventDirect);
}

void CNtlPLCharacter::OnEventColorChange( SEventColorChange* pEventColorChange ) 
{
    CNtlPLEventGenerator::AnimEventColorChange(GetSerialID(), pEventColorChange);
}

void CNtlPLCharacter::OnEventStretch( SEventStretch* pEventStretch ) 
{
    CNtlPLEventGenerator::AnimEventStretch(GetSerialID(), pEventStretch);
}

void CNtlPLCharacter::OnEventTrigger( SEventTrigger* pEventTrigger ) 
{
    CNtlPLEventGenerator::AnimEventTrigger(GetSerialID(), pEventTrigger);
}

void CNtlPLCharacter::OnEventSkillCancel( SEventSkillCancel* pEventSkillCancel ) 
{
    CNtlPLEventGenerator::AnimEventSkillCancel(GetSerialID());
}

void CNtlPLCharacter::SetSkinColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_sSkinColor.red	= byRed;
	m_sSkinColor.green	= byGreen;
	m_sSkinColor.blue	= byBlue;
}

void CNtlPLCharacter::SetHeadColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_sHeadColor.red	= byRed;
	m_sHeadColor.green	= byGreen;
	m_sHeadColor.blue	= byBlue;
}

void CNtlPLCharacter::SetInkColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	if(m_ToonData.pToonInk)
	{
		m_sInkColor.red   = byRed;
		m_sInkColor.green = byGreen;
		m_sInkColor.blue  = byBlue;
	}

	if( !GetCharScheduleResInfo()->bLoadComplete )
		return;

	RpToonInkSetColor(m_ToonData.pToonInk, m_sInkColor);
}

void CNtlPLCharacter::SetInkThickness(RwReal fThickness)
{
	if( !GetCharScheduleResInfo()->bLoadComplete )
		return;

	RpToonInkSetOverallThickness(m_ToonData.pToonInk, fThickness);
}

void CNtlPLCharacter::ClearLoopEffect()
{
    for each(SLoopEffect* pLoopEffect in m_listLoopEffect)
    {
        if(pLoopEffect && pLoopEffect->pLoopEffectIntance)
        {
            // 삭제가 아니라 종료를 시킨다.
            pLoopEffect->pLoopEffectIntance->Finish();            
        }

        NTL_DELETE(pLoopEffect);
    }

	m_listLoopEffect.clear();
}

RwBool CNtlPLCharacter::IsExistLoopEffect(const RwChar* szEffectName, const RwChar* szBoneName)
{
	if(!szEffectName || !szBoneName)
		return FALSE;

	ListLoopEffect::iterator it = m_listLoopEffect.begin();
	for(;it != m_listLoopEffect.end(); ++it)
	{
		SLoopEffect* pLoopEffect = *it;
		if(pLoopEffect && pLoopEffect->pLoopEffectIntance)
		{
			if(strcmp(pLoopEffect->pLoopEffectIntance->GetName(), szEffectName) == 0 &&
				strcmp(pLoopEffect->szBoneName, szBoneName) == 0)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CNtlPLCharacter::ClearLoopSound()
{
	ListSoundHandle::iterator it = m_listLoopSound.begin();
	for(; it != m_listLoopSound.end(); ++it)
	{
		SOUND_HANDLE hSound = *it;
		GetSoundManager()->Stop(hSound);        
	}

	m_listLoopSound.clear();
}

RwBool CNtlPLCharacter::IsExistLoopSound(RwChar* szSoundName)
{
	ListSoundHandle::iterator it = m_listLoopSound.begin();
	for(; it != m_listLoopSound.end(); ++it)
	{
		SOUND_HANDLE hListSound = *it;
        if(hListSound == INVALID_SOUND_HANDLE)
            continue;

		if(_strcmpi(GetSoundManager()->GetSoundName(hListSound),szSoundName) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

CNtlInstanceEffect* CNtlPLCharacter::AttachLinkEffect(SEventLinkEffect* pEventLinkEffect)
{
	if(!pEventLinkEffect)
		return FALSE;

	return m_LinkEffectInstance.AttachLinkEffect(this, pEventLinkEffect);

}

RwBool CNtlPLCharacter::DetachLinkEffect(CNtlInstanceEffect* pLinkEffect)
{
	if(!pLinkEffect)
		return FALSE;

	return m_LinkEffectInstance.DetachLinkEffect(this, pLinkEffect);
}

void CNtlPLCharacter::SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_sColor.red	= byRed;
	m_sColor.green	= byGreen;
	m_sColor.blue	= byBlue;

	if( !GetCharScheduleResInfo()->bLoadComplete )
		return;

	if(m_pEquipItem)
		m_pEquipItem->SetDefItemColor( byRed, byGreen, byBlue );
}

void CNtlPLCharacter::SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_sAddColor.red = byRed;
	m_sAddColor.green = byGreen;
	m_sAddColor.blue  = byBlue;

	if( !GetCharScheduleResInfo()->bLoadComplete )
		return;

	if( m_pEquipItem )
		m_pEquipItem->SetDefItemAddColor( byRed, byGreen, byBlue );
}


void CNtlPLCharacter::SetAlpha(RwUInt8 byValue)
{
	m_sColor.alpha = byValue;

	if( !GetCharScheduleResInfo()->bLoadComplete )
		return;

	if(m_pEquipItem)
		m_pEquipItem->SetDefItemAlpha(byValue);

	if( !m_sScheduleResInfo.bPCFlag )
	{
		if( m_sColor.alpha >= 255 && GetWeightAlpha() >= 1.f)
		{
			if( !m_bRunTimeAlpha )
				return;
			m_bRunTimeAlpha = FALSE;
		}
		else
		{
			if( m_bRunTimeAlpha )
				return;
			m_bRunTimeAlpha = TRUE;
		}

		SetApplyRunTimeAlpha( m_bRunTimeAlpha );

		// RW's DXT1 Alpha Attr check
		SetDXT1AlphaCheck(m_sColor.alpha);
	}
}

void CNtlPLCharacter::SetWeightAlpha(RwReal fWeightValue)
{
	CNtlPLAttach::SetWeightAlpha(fWeightValue);

	if(m_pEquipItem)
		m_pEquipItem->SetDefItemWeightAlpha(fWeightValue);

	if( !m_sScheduleResInfo.bPCFlag )
	{
		if( m_sColor.alpha >= 255 && GetWeightAlpha() >= 1.f)
		{
			if( !m_bRunTimeAlpha )
				return;
			m_bRunTimeAlpha = FALSE;
		}
		else
		{
			if( m_bRunTimeAlpha )
				return;
			m_bRunTimeAlpha = TRUE;
		}

		SetApplyRunTimeAlpha( m_bRunTimeAlpha );
	}
}

void CNtlPLCharacter::SetAtomicWeightAlpha( const RwChar *pAtomicName, RwReal fWeightValue ) 
{
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		if(strcmp(Helper_AtomicName(m_vecAtomicList[i]->pData), pAtomicName) == 0)
		{
			RwUInt8 byAlpha = (RwUInt8)(fWeightValue * 255.0f);
			RpNtlAtomicSetAlpha(m_vecAtomicList[i]->pData, byAlpha);            
			break;
		}
	}
}

void CNtlPLCharacter::SetApplyRunTimeAlpha(RwBool bFlag)
{
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);

		if(bFlag)
			RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag | NTL_RUNTIME_ALPHA);
		else
			RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag & (~NTL_RUNTIME_ALPHA));
	}
}

void CNtlPLCharacter::SetVisible(RwBool bVisible)
{
	if( !GetCharScheduleResInfo()->bLoadComplete )
	{
		m_sScheduleResInfo.bVisible = bVisible;
		return;
	}
	
	if( m_pEquipItem )
	{
		m_pEquipItem->SetDefItemVisible( bVisible );

	}

    m_LinkEffectInstance.SetVisible(bVisible);
    
    for each(SLoopEffect* pEffect in m_listLoopEffect)
    {
        if(pEffect && pEffect->pLoopEffectIntance)
        {
            pEffect->pLoopEffectIntance->SetVisible(bVisible);
        }
    }

	CNtlPLAttach::SetVisible(bVisible);
}

void CNtlPLCharacter::SetPicking(RwBool bPicking)
{
	if(m_pEquipItem)
	{
		DEFAULT_ITEM_MAP *pmapDefItem = m_pEquipItem->GetDefaultItemList();
		DEFAULT_ITEM_MAP_ITER it;
		for(it = pmapDefItem->begin(); it != pmapDefItem->end(); it++)
		{
			(*it).second->SetPicking(bPicking);
		}

		for each(CNtlPLItem* pItem in *(m_pEquipItem->GetEquipItemList()))
		{
			if(!pItem)
				continue;

			pItem->SetPicking(bPicking);
		}
	}

	CNtlPLEntity::SetPicking(bPicking);
}

RwBool CNtlPLCharacter::SetItemGradeEffect( ENtlPLItemGrade eGrade ) 
{
	if(m_pEquipItem)
	{
		for each(CNtlPLItem* pItem in *(m_pEquipItem->GetEquipItemList()))
		{
			if(!pItem)
				continue;

			pItem->SetUpgradeEffect(eGrade);
		}
	}  

	return TRUE;
}

RwBool CNtlPLCharacter::IsApplyGroundPitchAngle(void)
{
	if(m_pProperty == NULL)
		return FALSE;

	return m_pProperty->IsApplyNormal();
}

// RwSphere* CNtlPLCharacter::GetBoundingSphere()
// {
// 	m_BSphereCur.center = GetPosition() + m_BSphere.center;
// 	m_BSphereCur.radius	= m_BSphere.radius;
// 
// 	return &m_BSphereCur;
// }
// 
// void CNtlPLCharacter::CalcBoundingSphere()
// {
// 	m_BSphere.center = GetPosition();
// 	m_BSphere.radius = 0.0f;
// 
// 	for (RwInt32 iAtomicCnt = 0; iAtomicCnt < (RwInt32)m_vecAtomicList.size(); ++iAtomicCnt)
// 	{
// 		RpAtomic*	pAtomic		= m_vecAtomicList.at(iAtomicCnt)->pData;
// 		RwSphere*	pCurSphere	= const_cast<RwSphere*>(RpAtomicGetWorldBoundingSphere(pAtomic));
// 
// 		m_BSphere.center = (m_BSphere.center + pCurSphere->center) * 0.5f;
// 		m_BSphere.radius = RwV3dLength(&((m_BSphere.center - pCurSphere->center) * 0.5f)) + (m_BSphere.radius < pCurSphere->radius ? pCurSphere->radius : m_BSphere.radius);
// 	}
// 
// 	m_BSphere.center = m_BSphere.center - GetPosition();
// }

RwSphere* CNtlPLCharacter::GetBoundingSphere()
{
	m_BSphereCur.center = GetPosition();
	m_BSphereCur.radius = GetMaxExtent() * 0.5f;
	m_BSphereCur.center.y += m_BSphereCur.radius;

	return &m_BSphereCur;
}

#ifdef dNTL_WORLD_CULLING_NEW
//#include "NtlPLRenderGeometry.h"
RwBool CNtlPLCharacter::CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame)
{
	if (m_uiRenderFrame != uiRenderFrame)
	{
		if (!CNtlPLEntity::CullingTest(pRwCamera, uiRenderFrame))
		{
			RwInt32 iNumAtomic		= static_cast<RwInt32>(m_vecAtomicList.size());
			RwInt32	iFrustumCheck	= 0;
			RwInt32	iOccluderCheck	= 0;

			if (GetCharScheduleResInfo()->bLoadComplete && iNumAtomic > 0)
			{				
				if (IsCullingTestAllAtomic())
				{
					// [m_vecAtomicList.size()]번 Occluder Proxy는 Bounding Sphere다.
					for (RwInt32 i = 0; i < iNumAtomic; ++i)
					{
#ifdef _DEBUG
						++CNtlPLGlobal::m_uiCullTestAtomicCnt;
#endif

						RpAtomic*		pAtomic = static_cast<RpAtomic*>(m_vecAtomicList.at(i)->pData);
						const RwSphere* pSphere = RpAtomicGetWorldBoundingSphere(pAtomic);

						if (!CNtlPLGlobal::m_bCollObjVisible && (RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE))
						{
							++iFrustumCheck;							
						}
						else if (RwCameraFrustumTestSphere(pRwCamera, pSphere) == rwSPHEREOUTSIDE)
						{
							++iFrustumCheck;
						}
						else if (OccluderQuery(EOCCLUDER_SPHERE, EPLOCCLUDER_FUNC_CHARACTER, (void*)pSphere, i))
						{ 
							++iOccluderCheck;
						}
						else
						{
							break;
						}
					}
				}
				else
				{
					RwSphere* pSphere = GetBoundingSphere();

// 					static RwRGBA	clr = { 255, 255, 255 ,255 };
// 					static RwMatrix	mat;
// 
// 					RwMatrixTranslate(&mat, &pSphere->center, rwCOMBINEREPLACE);
// 					RenderGeometryBillboardSphere(&mat, &clr, pSphere->radius, pRwCamera, TRUE);

#ifdef _DEBUG
					++CNtlPLGlobal::m_uiCullTestAtomicCnt;
#endif

					if (RwCameraFrustumTestSphere(pRwCamera, pSphere) == rwSPHEREOUTSIDE)
					{
						iFrustumCheck = iNumAtomic;
					}
					else if (OccluderQuery(EOCCLUDER_SPHERE, EPLOCCLUDER_FUNC_CHARACTER, (void*)pSphere, iNumAtomic))
					{	
						iOccluderCheck = iNumAtomic;
					}
				}
			}

			if (iFrustumCheck >= iNumAtomic)
			{
				m_uiCullFlags |= NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT;
			}
			else if (iFrustumCheck + iOccluderCheck >= iNumAtomic) 
			{
				// Frustum + Occluder 갯수가 Atomic 갯수보다 많다면
				// OCCLUDER Flag를 셋팅 한다. 단 OccluderCheck 갯수가 존재해야 하므로, iFrustumCheck >= iNumAtomic를 통과해야만 가능하다.
				m_uiCullFlags |= NTL_PLEFLAG_CULLED_OCCLUDER;
			}
		}
		SetCullFlags(m_uiCullFlags);
	}
	return !IsVisible();
}
#else
RwBool CNtlPLCharacter::CullingTest(RwCamera* pRwCamera)
{
	if (!CNtlPLEntity::CullingTest(pRwCamera))
	{
		RwInt32 iNumAtomic = static_cast<RwInt32>(m_vecAtomicList.size());

		if (GetCharScheduleResInfo()->bLoadComplete && iNumAtomic > 0)
		{
			RwInt32 iFrustumCheck = 0;
			RwInt32 iOccluderCheck = 0;
			for (RwInt32 i = 0; i < iNumAtomic; ++i)
			{
#ifdef _DEBUG
				++CNtlPLGlobal::m_uiCullTestAtomicCnt;
#endif

				CNtlPLCullingScheduling *pCullScheduling = GetNtlPLCullingScheduler();
				if(pCullScheduling)
				{
					pCullScheduling->IncAtomicOperator(PLENTITY_CHARACTER);
				}

				RpAtomic* pAtomic = static_cast<RpAtomic*>(m_vecAtomicList.at(i)->pData);
				RwSphere* pSphere = &pAtomic->worldBoundingSphere;

#ifdef dNTL_WORLD_TOOL_MODE
				if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST))
				{
					rwObjectSetFlags(pAtomic, rwObjectGetFlags(pAtomic) | rpATOMICRENDER);
					continue;
				}
#endif

				if (RwCameraFrustumTestSphere(pRwCamera, pSphere) == rwSPHEREOUTSIDE)
				{
					rwObjectSetFlags(pAtomic, rwObjectGetFlags(pAtomic) & (~rpATOMICRENDER));
					++iFrustumCheck;
				}
				else if (OccluderQuery(EOCCLUDER_SPHERE, EPLOCCLUDER_FUNC_CHARACTER, (void*)pSphere, i))
				{
					rwObjectSetFlags(pAtomic, rwObjectGetFlags(pAtomic) & (~rpATOMICRENDER));
					++iOccluderCheck;
				}
				else
				{
					rwObjectSetFlags(pAtomic, rwObjectGetFlags(pAtomic) | rpATOMICRENDER);
				}
			}
			if (iFrustumCheck >= iNumAtomic)
			{
				m_uiCullFlags |= NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT;
			}
			else if (iFrustumCheck + iOccluderCheck >= iNumAtomic) 
			{
				// Frustum + Occluder 갯수가 Atomic 갯수보다 많다면
				// OCCLUDER Flag를 셋팅 한다. 단 OccluderCheck 갯수가 존재해야 하므로, iFrustumCheck >= iNumAtomic를 통과해야만 가능하다.
				m_uiCullFlags |= NTL_PLEFLAG_CULLED_OCCLUDER;
			}
		}		
	}

	SetCullFlags(m_uiCullFlags);
	return static_cast<RwBool>(m_uiCullFlags);	
}
#endif

void CNtlPLCharacter::SetCullFlags(RwUInt32 uiFlag, RwBool bCulled)
{
	CNtlPLAttach::SetCullFlags(uiFlag, bCulled);
	if (m_pEquipItem)
	{
		m_pEquipItem->SetCullFlags(uiFlag, bCulled);
	}
}

void CNtlPLCharacter::SetCullFlags(RwUInt32 uiFlag)
{
	CNtlPLAttach::SetCullFlags(uiFlag);
	if(m_pEquipItem)
	{
		m_pEquipItem->SetCullFlags(uiFlag);
	}
}

RwReal CNtlPLCharacter::GetAlphaDistance() 
{
    // 설정이 안되어 있으면 높이의 1.5배를 반환한다.
    return m_pProperty->GetAlphaDistance() == 0.0f ? GetHeight() * 1.5f : m_pProperty->GetAlphaDistance();
}

void CNtlPLCharacter::SetDXT1AlphaCheck(RwBool _DXT1AlphaCheck)
{
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpGeometry *pGeom = RpAtomicGetGeometry(m_vecAtomicList[i]->pData);
		if(pGeom)
		{
			RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

			for(RwInt32 m = 0; m < nMatCount; m++)
			{
				RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, m);
				if(pMaterial)
				{
					if(_DXT1AlphaCheck)
						RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) | rpNTL_MATERIAL_DXT1_ALPHA_CHECK);
					else
						RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) & ~rpNTL_MATERIAL_DXT1_ALPHA_CHECK);
				}
			}
		}	
	}
}

VOID CNtlPLCharacter::SetPetrifyMaterial(RwBool _PetrifyFlag)
{
	// NPC, MOB
	m_PetrifySkillActivated = _PetrifyFlag;

	// Avatars
	if(m_pEquipItem)
		m_pEquipItem->SetPetrifyMaterial(_PetrifyFlag);

    // Link Effects
    m_LinkEffectInstance.SetVisible(!_PetrifyFlag);
}

void CNtlPLCharacter::SetSimpleMaterial(RwBool _SimpleMaterial)
{
	// NPC, MOB
	m_SimpleMaterial = _SimpleMaterial;

	// Avatars
	if(m_pEquipItem)
		m_pEquipItem->SetSimpleMaterial(_SimpleMaterial);
}  

void CNtlPLCharacter::fpRenderCB(void* _pParam)
{
	MatExtInfoTag* pParam = static_cast<MatExtInfoTag*>(_pParam);

	if(!m_SkipSimpleMaterial && (RpNtlMaterialExtGetFlag(pParam->pMaterial) & rpNTL_MATERIAL_SIMPLEMATERIAL))
	{
		CNtlPLCharMeshRenderPipeFarAway::GetInstance()->Execute(_pParam);
	}
	else
	{
		const D3DCAPS9* pD3DCaps = (const D3DCAPS9*)RwD3D9GetCaps();
		if(pD3DCaps->MaxSimultaneousTextures > 4)
		{
			CNtlPLCharMeshRenderPipe01::GetInstance()->Execute(_pParam);
		}
		else
		{
			CNtlPLCharMeshRenderPipe00::GetInstance()->Execute(_pParam);
		}
	}
}

RwChar* CNtlPLCharacter::GetBoneName( int nBoneIndex ) 
{
    if(nBoneIndex < 0)
        return NULL;

    RwFrame* pFrame = m_pBaseHierarchy->pNodeInfo[nBoneIndex].pFrame;
    if(!pFrame)
        return NULL;

    RwChar* sBoneName = Helper_BoneName(pFrame);

    return sBoneName;
}

void CNtlPLCharacter::SetSBoneStretchTargetPos( const RwV3d& vTargetPos, const RwChar* szAxisBone ) 
{
    m_vBoneStretchTargetPos = vTargetPos;    
    sprintf_s(m_szBoneStretchAxisBone, "%s", szAxisBone);
}

void CNtlPLCharacter::UpdateMaterialSkinInfo() 
{
    for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
    {
        RpAtomic *pAtomic = m_vecAtomicList[i]->pData;        
        Helper_SetAtomicAllMaterialSkinInfo(pAtomic);        
    }
}

void CNtlPLCharacter::OnAfterAnimEventRun() 
{
    m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].OnAfterAnimEventRun();    
}

VOID CNtlPLCharacter::ToggleEMUVAni(RwBool _Visibility)
{
	CNtlPLCharMeshRenderPipe::ToggleEMUVAni(_Visibility);
}

RwBool CNtlPLCharacter::IsCullingTestAllAtomic() 
{
    STypeAnimData *pTypeAnimData = m_pProperty->GetAnimTable()->Get(GetCurBaseAnimKey());
    if(!pTypeAnimData)
        return TRUE;

    return pTypeAnimData->IsCullTestAllAtomic();
}