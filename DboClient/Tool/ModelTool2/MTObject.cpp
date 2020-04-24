#include "StdAfx.h"
#include "MTObject.h"
#include "NtlSoundManager.h"
#include "ModelToolApplication.h"
#include "NtlPLEntityBlend.h"

CMTObject::CMTObject(void)
{
    m_pProperty = NTL_NEW CNtlPLObjectProperty();
    m_eFadeState = FADE_VISIBLE;
	m_bActiveUpdate = TRUE;
}

CMTObject::~CMTObject(void)
{
    NTL_DELETE(m_pProperty);    

    Destroy();
}


void* CMTObject::operator new(size_t size)
{
    return ::operator new(size);
}

void CMTObject::operator delete(void *pObj)
{
    ::operator delete(pObj);
}


RwBool CMTObject::LoadClump(RwChar* szFileName)
{
    if(m_pClumpResource)
    {
        Destroy();        
    }

    // 설정된 UVAnim이 있으면 적용한다.(Clump가 Load되기전에 Load 되어야 한다)
    if(m_pProperty && m_pProperty->m_strUVAnimFileName.size())
        SetUVAnim(m_pProperty->m_strUVAnimFileName.c_str());
    
    if(m_pClumpResource == NULL)
    {
        m_pClumpResource = CNtlPLResourceManager::GetInstance()->LoadClump(szFileName, CModelToolApplication::GetInstance()->GetTexturePath());
        if(!m_pClumpResource)
            return FALSE;
    }    

    if(!Create())
        return FALSE;

    m_MTClump.Create(GetClump(), szFileName);
    
    if(!m_pProperty)
        return FALSE;

    if(m_pClumpResource)
    {
        Helper_SetClumpAllAtomics(m_pClumpResource->GetClump(), &m_vecAtomicList, false);
        for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
        {
            RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
            RpNtlAtomicSetRenderCallBack( pAtomic, RpAtomicGetRenderCallBack(pAtomic) );
            RpAtomicSetRenderCallBack(pAtomic, CNtlPLObject::RenderCallBack);
            RpNtlAtomicSetData(pAtomic, this);
        }
    }

    Helper_GetBoneList(GetClump(), &m_mapFrame);            // Bone List를 저장해 둔다.

    // BBox가 없으면 데이터를 추가한다.
    RwBBox* pBBox = m_pProperty->GetBBox();
    if(pBBox->sup.x == 0.0f && pBBox->inf.x == 0.0f)
    {
        CalcBBox();
    }

    // UVAnim을 Clump에 적용한다.
    if(m_pUVAnim)
    {
        m_pUVAnim->SetClump(GetClump());
        m_pUVAnim->SetUVAnimSpeed(m_pProperty->GetUVAnimSpeed());
        if(GetFlags() & NTL_PLEFLAG_NOTUPDATE)
            SetFlags(GetFlags() ^ NTL_PLEFLAG_NOTUPDATE);
    }

    // Toon
    CheckToonData();

    //m_bCollision = m_pProperty->GetCollision();
    SetVisibleCullingDistance(m_pProperty->GetCullingDistance());

    // 블렌드 객체를 생성하고 리스트에 추가한다.
    m_pFadeBlend = GetAlphaBlendController()->AddAlpha(1.0f);

    // Link Effect를 설정한다.
    for(UINT i = 0; i < m_pProperty->m_vLinkEffect.size(); ++i)
    {
        SEventLinkEffect* pEventLinkEffect = m_pProperty->m_vLinkEffect[i];        
        AttachLinkEffect(pEventLinkEffect);
    }	    
    if(!m_pProperty->m_vLinkEffect.empty())
    {
        Update(0.0f);       // 링크이펙트가 있으면 최초에 업데이트를 한번 해줘야 한다. 
    }

    // Sound를 재생한다.
    if(strlen(m_pProperty->GetSoundProp()->chSoundName) > 0)
    {
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_OBJECT_MUSIC;
		tSoundParam.pcFileName		= m_pProperty->GetSoundProp()->chSoundName;
		tSoundParam.bLoop			= true;
		tSoundParam.fVolume			= m_pProperty->GetSoundProp()->fSoundVolume * 0.01f;
		tSoundParam.fPitch			= m_pProperty->GetSoundProp()->fSoundPitchMin;
		tSoundParam.fXPos			= m_vWorldPosition.x;
		tSoundParam.fYPos			= m_vWorldPosition.y;
		tSoundParam.fZPos			= m_vWorldPosition.z;
		tSoundParam.fMinDistance	= m_pProperty->GetSoundProp()->fSoundDist;
		tSoundParam.fMaxDistance	= m_pProperty->GetSoundProp()->fSoundDecayDist;

		GetSoundManager()->Play(&tSoundParam);

        AddLoopSound(tSoundParam.hHandle);
    }

    CalcBoundingSphere();  

    m_sScheduleInfo.bLoadComplete = TRUE;

    return TRUE;
}

RwBool CMTObject::Create(const SPLEntityCreateParam * pParam )
{
    if(!GetClump())
        return FALSE;

    RpNtlClumpSetData( GetClump(), this );
    RpWorldAddClump( CNtlPLGlobal::m_pRpWorld, GetClump() );


    return TRUE;
}

RwBool CMTObject::Update( RwReal fElapsed ) 
{
    return CNtlPLObject::Update(fElapsed);
}

void CMTObject::SetVisible(BOOL bVisible)
{
    m_MTClump.SetVisible(bVisible);

    CNtlPLObject::SetVisible(bVisible);
}

void CMTObject::CalcBBox() 
{
    m_pProperty->SetBBox(m_MTClump.GetDefaultBBox());
}

CNtlInstanceEffect* CMTObject::GetLinkEffectFromName(RwChar* szName)
{
    return m_LinkEffectInstance.GetLinkEffectFromName(szName);
}

const RwBBox* CMTObject::GetBoundingBox( void ) 
{
	m_bbox = GetTriggerAABBInfo();
	return &m_bbox;
}

void CMTObject::RenderBBox() 
{
	m_MTClump.RenderBB(const_cast<RwBBox*>(GetBoundingBox()), 255, 0, 0);
}

void CMTObject::RenderWireFrame() 
{
	m_MTClump.RenderWireFrame();
}

int CMTObject::CallBackBaseAnim(void* pEventData ) 
{
    CNtlPLObject::CallBackBaseAnim(pEventData);

    // Model Tool에서만 따로 처리하는 부분만 처리한다.
    SEventAnim* pEventAnim = (SEventAnim*)pEventData;
    switch(pEventAnim->eEventID)
    {
		case EVENT_ANIM_ALPHA:
			OnEventAlphaFade((SEventAlpha*)pEventData);
		break;
    }

    return TRUE;
}

void CMTObject::OnEventAlphaFade( SEventAlpha* pEventAlpha ) 
{
    if(pEventAlpha->eAlphaEventType == SEventAlpha::E_ALPHA_EVENT_CLUMP)
    {
        GetAlphaBlendController()->AddAlpha(pEventAlpha->nStartAlpha / 255.0f, 
                                            pEventAlpha->nDestAlpha / 255.0f,
                                            pEventAlpha->fFadeTime,
                                            pEventAlpha->fLifeTime,
                                            FALSE);        
    }
    else
    {
        ::CNtlPLObject::OnEventAlphaFade(pEventAlpha);
    }
}

RwBool CMTObject::SetTriggerAnimation( RwUInt32 uiAnimKey, RwReal fStartTime /*= 0.0f*/, RwBool bLoop /*= TRUE*/ ) 
{
    SetAlpha(255);

    return CNtlPLObject::SetTriggerAnimation(uiAnimKey, fStartTime, bLoop);
}

void CMTObject::SetWeightAlpha( RwReal fWeightValue ) 
{
    CNtlPLObject::SetWeightAlpha(fWeightValue);

    SetAlpha((RwUInt8)(fWeightValue * 255.0f));
}

void CMTObject::ClearLoopSound() 
{
    CNtlPLObject::ClearLoopSound();
}