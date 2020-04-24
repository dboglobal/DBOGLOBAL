#include "precomp_ntlpresentation.h"
#include "NtlAnimLayer.h"
#include "NtlCharacterData.h"

CNtlAnimLayer::CNtlAnimLayer() : m_pBaseHierarchy(NULL),
								 m_bBlendChange(FALSE),
								 m_iLastSendEventIndex(-1),
								 m_fLastSendEventTime(0.f)
{
	m_pCallBack = NULL;
}
  
RwBool CNtlAnimLayer::Create(RpHAnimHierarchy *pBaseHierarchy)
{
	NTL_FUNCTION("CNtlAnimLayer::Create()");
	NTL_ASSERTE(pBaseHierarchy != NULL);
	NTL_ASSERTE(pBaseHierarchy->currentAnim->pCurrentAnim != NULL);
	if(pBaseHierarchy == NULL || pBaseHierarchy->currentAnim->pCurrentAnim == NULL)
		NTL_RETURN(FALSE);

	m_pBaseHierarchy = pBaseHierarchy;
	for(RwInt32 i = 0; i < ANIM_CHANNEL_END; i++)
	{
		if(!m_AnimChannel[i].Create(pBaseHierarchy))
			NTL_RETURN(FALSE);
	}
	
	NTL_RETURN(TRUE);
}

void CNtlAnimLayer::Destroy()
{
	NTL_FUNCTION("CNtlAnimLayer::Destroy()");
	
	for(int i = 0; i < ANIM_CHANNEL_END; i++)
	{
		m_AnimChannel[i].Destroy();
	}
	
	if(m_pCallBack != NULL)
	{
		NTL_DELETE (m_pCallBack);
		m_pCallBack = NULL;
	}

	NTL_RETURNVOID();
}


static float Min(float x, float y)
{
	if(x < y)
		return x;
	else if(x > y)
		return y;
	else
		return x;
}

static float Max(float x, float y)
{
	if(x > y)
		return x;
	else if(x < y)
		return y;
	
	return x;
}


RwBool CNtlAnimLayer::Update(float fElapsed)
{
	NTL_FUNCTION("CNtlAnimLayer::Update()");
	NTL_ASSERT(fElapsed >= 0.f, "fElapsed is minus\n");

	RpHAnimHierarchy *pCurrentHierarchy;
	RpHAnimHierarchy *pNextHierarchy;
	pCurrentHierarchy = m_AnimChannel[ANIM_CHANNEL_CURRENT].GetHierarchy();
	pNextHierarchy    = m_AnimChannel[ANIM_CHANNEL_NEXT].GetHierarchy();

	RwReal fOldTime = pCurrentHierarchy->currentAnim->currentTime;
	RwReal fUpdateElapsedTime = pCurrentHierarchy->currentAnim->currentTime + fElapsed;
	if( m_pCallBack &&
		!m_bBlendChange && 
		!m_AnimChannel[ANIM_CHANNEL_CURRENT].GetAnimEnd())
	{
		SInstanceAnimData *pCurInstanceAnimData = m_AnimChannel[ANIM_CHANNEL_CURRENT].GetAnimData();

        // 이벤트 콜백
		RwUInt32 uiEventSize = pCurInstanceAnimData->pTypeAnimData->vecAnimEvent.size();
		if(uiEventSize)
		{
			//Model Tool에서는 Sorting이 안되어 있을 수도 있다.
			for(RwUInt32 i = 0; i < uiEventSize; i++)
			{
				RwReal fTime = pCurInstanceAnimData->pTypeAnimData->vecAnimEvent[i]->fTime;

				// 0은 Event를 안보낸다. 0을 보낼 수 있도록 수정 후 Test (by HoDong)
				if( fOldTime < fTime && fTime <= fUpdateElapsedTime)
				{
					m_pCallBack->Call(pCurInstanceAnimData->pTypeAnimData->vecAnimEvent[i]);
				}
			}
		}

		//Animation의 End까지 Play가 될 경우 AnimEnd Event를 보낸다.
		//Loop가 아닐경우에만 Event를 보낸다.
		if( fUpdateElapsedTime >= pCurrentHierarchy->currentAnim->pCurrentAnim->duration && 
			!m_AnimChannel[ANIM_CHANNEL_CURRENT].GetLoop())
		{
			SEventAnimEnd	sEventAnimEnd;
			sEventAnimEnd.uiAnimKey = pCurInstanceAnimData->pTypeAnimData->uiAnimKey;
			fElapsed = fUpdateElapsedTime - pCurrentHierarchy->currentAnim->pCurrentAnim->duration;
			RpHAnimHierarchySetCurrentAnimTime(pCurrentHierarchy, pCurrentHierarchy->currentAnim->pCurrentAnim->duration);
			m_pCallBack->Call(&sEventAnimEnd);
			m_AnimChannel[ANIM_CHANNEL_CURRENT].SetAnimEnd(TRUE);

			RpHAnimHierarchyCopy(m_pBaseHierarchy, pCurrentHierarchy);																																																																																																																																																															
			NTL_RETURN(TRUE);
		}
	}
	
	if( !m_AnimChannel[ANIM_CHANNEL_CURRENT].GetLoop() && 
		 m_AnimChannel[ANIM_CHANNEL_CURRENT].GetAnimEnd() &&
	    !m_bBlendChange)
	{
		//보간값이 갱신이 되어 있어야 Scale 적용시 문제가 안생긴다.
		RpHAnimHierarchyCopy(m_pBaseHierarchy, pCurrentHierarchy);
		NTL_RETURN(TRUE);
	}


	if(!m_bBlendChange || m_CurBlendData.eBlendType == BLEND_MIX)
		m_AnimChannel[ANIM_CHANNEL_CURRENT].Update(fElapsed);
	
	if(m_bBlendChange && m_CurBlendData.eBlendType == BLEND_MIX)
		m_AnimChannel[ANIM_CHANNEL_NEXT].Update(fElapsed);
	
    // 애니메이션 블렌딩
	if(m_bBlendChange)
	{
		RpHAnimHierarchyBlend(m_pBaseHierarchy, pCurrentHierarchy, pNextHierarchy, m_CurBlendData.fBlendAlpha);

		float timeDelta = Min( 1.0f,  fElapsed / m_CurBlendData.fBlendTargetInterval);
		float alphaDelta = (m_CurBlendData.fBlendTargetAlpha - m_CurBlendData.fBlendAlpha) * timeDelta;

		m_CurBlendData.fBlendAlpha += alphaDelta;

		m_CurBlendData.fBlendTargetInterval -= fElapsed;
		m_CurBlendData.fBlendTargetInterval = Max(m_CurBlendData.fBlendTargetInterval, 0.f);

		if(m_CurBlendData.fBlendTargetInterval == 0.f)
		{
			m_bBlendChange = FALSE;

			m_AnimChannel[ANIM_CHANNEL_CURRENT].SetAnimData(m_AnimChannel[ANIM_CHANNEL_NEXT].GetAnimData());
			m_AnimChannel[ANIM_CHANNEL_CURRENT].SetLoop(m_AnimChannel[ANIM_CHANNEL_NEXT].GetLoop());
			m_AnimChannel[ANIM_CHANNEL_CURRENT].SetAnimEnd(m_AnimChannel[ANIM_CHANNEL_NEXT].GetAnimEnd());

			RpHAnimHierarchySetCurrentAnimTime(pCurrentHierarchy, pNextHierarchy->currentAnim->currentTime);
			RpHAnimHierarchyCopy(m_pBaseHierarchy, pCurrentHierarchy);
		}
	}
	else
	{
		RpHAnimHierarchyCopy(m_pBaseHierarchy, m_AnimChannel[ANIM_CHANNEL_CURRENT].GetHierarchy());
	}
	NTL_RETURN(TRUE);
}

RwBool CNtlAnimLayer::SetAllAnimation(SInstanceAnimData *pInstanceAnimData, RwReal fStartTime, RwBool bLoop)
{
	NTL_FUNCTION("CNtlAnimLayer::SetAllAnimation()");
	NTL_ASSERTE(pInstanceAnimData);
	NTL_ASSERTE(fStartTime > 0.f || fStartTime < 1.f);

	if(pInstanceAnimData == NULL)
		NTL_RETURN(FALSE);

	m_AnimChannel[ANIM_CHANNEL_CURRENT].SetAnimData(pInstanceAnimData);
	m_AnimChannel[ANIM_CHANNEL_NEXT].SetAnimData(pInstanceAnimData);
	
	m_AnimChannel[ANIM_CHANNEL_CURRENT].SetLoop(bLoop);
	m_AnimChannel[ANIM_CHANNEL_CURRENT].SetAnimEnd(FALSE);

	m_AnimChannel[ANIM_CHANNEL_NEXT].SetLoop(bLoop);
	m_AnimChannel[ANIM_CHANNEL_NEXT].SetAnimEnd(FALSE);

	RpHAnimHierarchy *pHierarchy = m_AnimChannel[ANIM_CHANNEL_CURRENT].GetHierarchy();
	RwReal fDurationTime = pHierarchy->currentAnim->pCurrentAnim->duration;
	RpHAnimHierarchySetCurrentAnimTime(pHierarchy, fDurationTime * fStartTime);

	pHierarchy = m_AnimChannel[ANIM_CHANNEL_NEXT].GetHierarchy();
	fDurationTime = pHierarchy->currentAnim->pCurrentAnim->duration;
	RpHAnimHierarchySetCurrentAnimTime(pHierarchy, fDurationTime * fStartTime);
	
	RpHAnimHierarchySetCurrentAnim(m_pBaseHierarchy, pInstanceAnimData->pResource->GetAnimation());
	fDurationTime = m_pBaseHierarchy->currentAnim->pCurrentAnim->duration;
	RpHAnimHierarchySetCurrentAnimTime(m_pBaseHierarchy, fDurationTime * fStartTime);
	
	m_iLastSendEventIndex = -1;
	m_fLastSendEventTime = 0.f;
	NTL_RETURN(TRUE);
}

RwBool CNtlAnimLayer::SetAnimation(SInstanceAnimData *pInstanceAnimData, RwReal fStartTime /* = 1.f */, RwBool bLoop /* = TRUE */, RwBool bBlending /* = TRUE */)
{
	NTL_FUNCTION("CNtlAnimLayer::SetBaseAnimation()");
	NTL_ASSERTE(pInstanceAnimData);
	NTL_ASSERTE(fStartTime > 0.f || fStartTime < 1.f);

	if(pInstanceAnimData == NULL)
		NTL_RETURN(FALSE);
	
	if( m_DefaultBlendData.fBlendTargetInterval > 0.f)
	{
		m_bBlendChange = bBlending;
		m_CurBlendData = m_DefaultBlendData;
	}

    if(m_bBlendChange)
    {
	    m_AnimChannel[ANIM_CHANNEL_NEXT].SetAnimData(pInstanceAnimData);
	    m_AnimChannel[ANIM_CHANNEL_NEXT].SetLoop(bLoop);
	    m_AnimChannel[ANIM_CHANNEL_NEXT].SetAnimEnd(FALSE);

        RpHAnimHierarchy *pHierarchy = m_AnimChannel[ANIM_CHANNEL_NEXT].GetHierarchy();
        RwReal fDurationTime = pHierarchy->currentAnim->pCurrentAnim->duration;
        RpHAnimHierarchySetCurrentAnimTime(pHierarchy, fDurationTime * fStartTime);
    }
    else        
    {
        m_AnimChannel[ANIM_CHANNEL_CURRENT].SetAnimData(pInstanceAnimData);
        m_AnimChannel[ANIM_CHANNEL_CURRENT].SetLoop(bLoop);
        m_AnimChannel[ANIM_CHANNEL_CURRENT].SetAnimEnd(FALSE);

        RpHAnimHierarchy *pHierarchy = m_AnimChannel[ANIM_CHANNEL_CURRENT].GetHierarchy();
        RwReal fDurationTime = pHierarchy->currentAnim->pCurrentAnim->duration;
        RpHAnimHierarchySetCurrentAnimTime(pHierarchy, fDurationTime * fStartTime);
		RpHAnimHierarchyCopy(m_pBaseHierarchy, m_AnimChannel[ANIM_CHANNEL_CURRENT].GetHierarchy());
    }                
	
	m_iLastSendEventIndex = -1;
	m_fLastSendEventTime = 0.f;

	NTL_RETURN(TRUE);
}


RwBool CNtlAnimLayer::SetBlend(EBlendType eBlendType, RwReal fBlendAlpha, RwReal fBlendInterval)
{
	NTL_FUNCTION("CNtlAnimLayer::SetBlend()");

	//m_bBlendChange = TRUE;

	m_DefaultBlendData.eBlendType  = eBlendType;
	m_DefaultBlendData.fBlendAlpha = fBlendAlpha;
	m_DefaultBlendData.fBlendTargetInterval = fBlendInterval;
	
	//m_CurBlendData = m_DefaultBlendData;

	NTL_RETURN(TRUE);
}

RwReal CNtlAnimLayer::GetCurrentAnimTime()
{
	NTL_FUNCTION("CNtlAnimLayer::GetCurrentAnimTime()");
	RpHAnimHierarchy *pHierarchy = m_AnimChannel[ANIM_CHANNEL_CURRENT].GetHierarchy();
	NTL_ASSERTE(pHierarchy);
	
	if( pHierarchy != NULL && pHierarchy->currentAnim != NULL &&
		pHierarchy->currentAnim->pCurrentAnim)
	{
		NTL_RETURN(pHierarchy->currentAnim->currentTime);
	}
	
	NTL_RETURN(0.f);
}

RwReal CNtlAnimLayer::GetDurationAnimTime()
{
	NTL_FUNCTION("CNtlAnimLayer::GetDurationAnimTime()");
	RpHAnimHierarchy *pHierarchy = m_AnimChannel[ANIM_CHANNEL_CURRENT].GetHierarchy();
	NTL_ASSERTE(pHierarchy);
	
	if( pHierarchy != NULL && pHierarchy->currentAnim != NULL &&
		pHierarchy->currentAnim->pCurrentAnim)
	{
		NTL_RETURN(pHierarchy->currentAnim->pCurrentAnim->duration);
	}
	
	NTL_RETURN(0.f);
}

void CNtlAnimLayer::SetCurrentAnimTime(RwReal fCurrTime)
{
    NTL_FUNCTION(__FUNCTION__);

    RpHAnimHierarchy *pHierarchy = m_AnimChannel[ANIM_CHANNEL_CURRENT].GetHierarchy();
    NTL_ASSERTE(pHierarchy);

    if( pHierarchy != NULL && pHierarchy->currentAnim != NULL &&
        pHierarchy->currentAnim->pCurrentAnim)
    {
        RpHAnimHierarchySetCurrentAnimTime(pHierarchy, fCurrTime);
		RpHAnimHierarchyCopy(m_pBaseHierarchy, pHierarchy);
    }
    
    NTL_RETURNVOID();
}
	

RwBool CNtlAnimLayer::GetCurrentAnimEnd()
{
	return m_AnimChannel[ANIM_CHANNEL_CURRENT].GetAnimEnd();
}

RwBool CNtlAnimLayer::IsBlendEnable(void)
{
	if(m_CurBlendData.fBlendTargetInterval > NTL_EPSILON)
		return TRUE;

	return FALSE;
}

void CNtlAnimLayer::OnAfterAnimEventRun() 
{
    // When a skill cancel event is triggered, all the events in the back are processed at once.
    // VE, Sound except
    if(m_pCallBack && !m_AnimChannel[ANIM_CHANNEL_CURRENT].GetAnimEnd())
    {
        SInstanceAnimData* pCurInstanceAnimData = m_AnimChannel[ANIM_CHANNEL_CURRENT].GetAnimData();        
        RwReal fAnimTime = GetCurrentAnimTime();
        for each(SEventAnim* pEventAnim in pCurInstanceAnimData->pTypeAnimData->vecAnimEvent)
        {
            if(pEventAnim->eEventID != EVENT_ANIM_VISUAL_EFFECT && 
               pEventAnim->eEventID != EVENT_ANIM_VISUAL_SOUND &&
               pEventAnim->fTime >= fAnimTime)
            {
                m_pCallBack->Call(pEventAnim);
            }            
        }        
    }
}


