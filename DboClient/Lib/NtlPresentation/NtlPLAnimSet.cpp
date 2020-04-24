#include "precomp_ntlpresentation.h"
#include "NtlPLAnimSet.h"
#include "NtlDebug.h"
#include "NtlPLGlobal.h"
#include "NtlDebug.h"
#include "NtlPLAnimEvent.h"

#include "NtlPLCharacter.h"

CNtlPLAnimSet::CNtlPLAnimSet() : m_pTargetClump(NULL),
								 m_pMainHierarchy(NULL),
								 m_bActiveFlag(FALSE),
								 m_fOldAnimTime(0.f)
{

}

CNtlPLAnimSet::~CNtlPLAnimSet()
{

}

/**
 * Frame의 Hierarchy를 얻어 온다.
 * \param *frame 현재 frame
 * \param *data return를 할 Hierarchy
 */
static RwFrame *GetChildFrameHierarchy(RwFrame *frame, void *data)
{    
    RpHAnimHierarchy *hierarchy = *(RpHAnimHierarchy **)data;

	hierarchy = RpHAnimFrameGetHierarchy(frame);
    if( hierarchy == NULL )
    {
        RwFrameForAllChildren(frame, GetChildFrameHierarchy, data);

        return frame;
    }

    *(void **)data = (void *)hierarchy;

    return NULL;
}

/**
 * Skin에 Hierarchy를 Setting을 한다.
 * \param *atomic 현재 atomic
 * \param *data Hierarchy 
 */
static RpAtomic *SetHierarchyForSkinAtomic(RpAtomic *atomic, void *data)
{
    RpSkinAtomicSetHAnimHierarchy(atomic, (RpHAnimHierarchy *)data);
	return atomic;
}

float Min(float x, float y)
{
	if(x < y)
		return x;
	else if(x > y)
		return y;
	else
		return x;
}

float Max(float x, float y)
{
	if(x > y)
		return x;
	else if(x < y)
		return y;
	
	return x;
}

/**
 * AnimSet를 생성을 한다.
 * \param *pClump Target Clump
 * \param *pDefaultAnim Default Animation(Idle): Hierarchy를 생성하기 위해서 필요
 */
void CNtlPLAnimSet::Create(RpClump *pClump, RtAnimAnimation *pDefaultAnim, CNtlPLCharacter *pOwner)
{
	NTL_FUNCTION("CNtlPLAnimSet::Create(RpClump *pClump, RtAnimAnimation *pDefaultAnim)");
	
	NTL_ASSERT(pClump != NULL, "pClump = NULL");
	NTL_ASSERT(pDefaultAnim != NULL, "pDefaultAnim = NULL");

	m_pTargetClump = pClump;
	m_pOwner = pOwner;

	CreateAnimation(pDefaultAnim);

	NTL_RETURNVOID();
}

void CNtlPLAnimSet::CreateAnimation(RtAnimAnimation *pDefaultAnim)
{
	NTL_FUNCTION("CNtlPLAnimSet::CreatAnimation(RtAnimAnimaiton *pDefaultAnim)");
	NTL_ASSERT(m_pTargetClump != NULL, "m_pTargetClump = NULL");
	NTL_ASSERT(pDefaultAnim != NULL, "pDefaultAnim = NULL");

	//Hierarchy를 얻어온다.
	RwFrameForAllChildren( RpClumpGetFrame(m_pTargetClump), 
		                   GetChildFrameHierarchy, 
						   (void *)&m_pMainHierarchy);

	//Skin에 Hierarchy 정보를 준다.
	RpClumpForAllAtomics( m_pTargetClump, SetHierarchyForSkinAtomic, (void *)m_pMainHierarchy);

	//Default Animation를 세팅한다.
	RpHAnimHierarchySetCurrentAnim(m_pMainHierarchy, pDefaultAnim);
	
	
	RpHAnimHierarchySetFlags( m_pMainHierarchy,
                              (RpHAnimHierarchyFlag)(RpHAnimHierarchyGetFlags(m_pMainHierarchy) | 
                              rpHANIMHIERARCHYUPDATELTMS | 
						      rpHANIMHIERARCHYUPDATEMODELLINGMATRICES));
	

	RpHAnimHierarchyAttach(m_pMainHierarchy);
	RpHAnimUpdateHierarchyMatrices(m_pMainHierarchy);
	
	for(int i = 0; i < ANIM_CHANNEL_END; i++)
	{
		m_Channel[i].pHierarchy      = RpHAnimHierarchyCreateFromHierarchy(m_pMainHierarchy, 
			RpHAnimHierarchyGetFlags(m_pMainHierarchy),
			m_pMainHierarchy->currentAnim->maxInterpKeyFrameSize);
		
		//Default Anim이 없으면 AddTogether 연산시 에러가 난다.
		RpHAnimHierarchySetCurrentAnim(m_Channel[i].pHierarchy,      pDefaultAnim);
	}
	m_eBlendType    = BLEND_TWEEN;
	m_bAnimChanging = FALSE;

	m_fDefaultBlendAlpha = 0.f;
	m_fDefaultBlendTargetAlpha = 1.f;
	m_fDefaultBlendTargetInterval = 0.1f;

	m_bActiveFlag = TRUE;
	NTL_RETURNVOID();
}

/**
 * AnimSet를 Destory 한다.
 */
void CNtlPLAnimSet::Destroy()
{
	NTL_FUNCTION("CNtlPLAnimSet::Destroy");
	
	for(int i = 0; i < ANIM_CHANNEL_END; i++)
	{
		if(m_Channel[i].pHierarchy)
			RpHAnimHierarchyDestroy(m_Channel[i].pHierarchy);
	}
	
	m_pTargetClump = NULL;
	NTL_RETURNVOID();
}

void CNtlPLAnimSet::SendAnimEnd()
{
	SAnimMsg msg;
	SEventAnimEnd animEnd;
	msg.id = EVENT_ANIM_END;
	msg.pEventData = &animEnd;
	animEnd.uiAnimKey = m_Channel[ANIM_CHANNEL_CURRENT].uiAnimKey;
	m_pOwner->HandleEvent(&msg);
}

void CNtlPLAnimSet::SendAnimHit()
{
	SAnimMsg msg;
	SEventAnimHit animHit;
	msg.id = EVENT_ANIM_HIT;
	msg.pEventData = &animHit;
	m_pOwner->HandleEvent(&msg);
}

void CNtlPLAnimSet::SendFootStep()
{
	SAnimMsg msg;
	msg.id = EVENT_ANIM_FOOT_STEP;
	msg.pEventData = NULL;
	m_pOwner->HandleEvent(&msg);
}

/*
 * AnimSet를 Update를 하는 함수이며 2개의 Animation
 * 간의 Blend를 처리를 한다.
 * \param fElapsed 이전 시간에서 경과된 시간
 */
void CNtlPLAnimSet::Update(float fElapsed)
{
	NTL_FUNCTION("CNtlPLAnimSet::Update");
	if(m_pTargetClump && m_bActiveFlag)
	{
		RwReal fCurrentElapsed = fElapsed * m_Channel[ANIM_CHANNEL_CURRENT].fAnimSpeed;
		RwReal fNextElapsed    = fElapsed * m_Channel[ANIM_CHANNEL_NEXT].fAnimSpeed;

		RwReal fCurrentAnimTime  = m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy->currentAnim->currentTime+fCurrentElapsed;
		RwReal fDurationAnimTime = m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy->currentAnim->pCurrentAnim->duration;

#ifndef _CHARACTER_TOOL_		
		SAnimData *pAnimData = m_Channel[ANIM_CHANNEL_CURRENT].pAnimData;
		if(pAnimData && !pAnimData->vecEvent.empty())
		{
			for(size_t i = 0; i < pAnimData->vecEvent.size(); i++)
			{
				//if( pAnimData->vecEvent[i].id == EVENT_ANIM_HIT &&
				if( pAnimData->vecEvent[i].fTime > m_fOldAnimTime &&
				    pAnimData->vecEvent[i].fTime < fCurrentAnimTime )
				{
					if( pAnimData->vecEvent[i].id == EVENT_ANIM_HIT)
						SendAnimHit();
					else if( pAnimData->vecEvent[i].id == EVENT_ANIM_FOOT_STEP)
						SendFootStep();
				}
			}
		}

		if(fCurrentAnimTime > fDurationAnimTime)
		{
			if( m_Channel[ANIM_CHANNEL_CURRENT].bLoop == FALSE )
			{
				RwReal fEndTimeRange = fDurationAnimTime - m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy->currentAnim->currentTime;
				if(fEndTimeRange >= 0.f && m_Channel[ANIM_CHANNEL_CURRENT].bNotLoopEnd == FALSE)
				{
					if(fEndTimeRange != 0.f)
					{
						if( !m_bAnimChanging ||
							(m_bAnimChanging && m_eBlendType == BLEND_MIX) )
							RpHAnimHierarchyAddAnimTime(m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy, fEndTimeRange);
						
						RpHAnimHierarchyUpdateMatrices(m_pMainHierarchy);
						m_fOldAnimTime = m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy->currentAnim->currentTime;
					}

					SendAnimEnd();

					m_Channel[ANIM_CHANNEL_CURRENT].bNotLoopEnd = TRUE;
				}
				if( !m_bAnimChanging)
					NTL_RETURNVOID();
			}
			else
			{
				//if( m_Channel[ANIM_CHANNEL_CURRENT].pAnimData->strAnimFileName == "A_HFI_M_Jump_start.anm")
					SendAnimEnd();
			}
		}
#endif		
		if( !m_bAnimChanging ||
			(m_bAnimChanging && m_eBlendType == BLEND_MIX) )
			RpHAnimHierarchyAddAnimTime(m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy, fCurrentElapsed);

		if(m_bAnimChanging && m_eBlendType == BLEND_MIX)
			RpHAnimHierarchyAddAnimTime(m_Channel[ANIM_CHANNEL_NEXT].pHierarchy, fNextElapsed);

		if(m_bAnimChanging)
		{
			RpHAnimHierarchyBlend(m_pMainHierarchy, 
				m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy,
				m_Channel[ANIM_CHANNEL_NEXT].pHierarchy, m_fBlendAlpha);

			float timeDelta = Min( 1.0f,  fElapsed / m_fBlendTargetInterval);
			float alphaDelta = (m_fBlendTargetAlpha - m_fBlendAlpha) * timeDelta;

			m_fBlendAlpha += alphaDelta;

			m_fBlendTargetInterval -= fElapsed;
			m_fBlendTargetInterval = Max(m_fBlendTargetInterval, 0.f);

			if(m_fBlendTargetInterval == 0.f)
			{
				m_bAnimChanging = FALSE;
				m_fBlendAlpha = 0.f;

				//Next Animation의 상태 및 Ani 정보를 Current로 옮긴다.
				RpHAnimHierarchySetCurrentAnim( m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy,
					                            RpHAnimHierarchyGetCurrentAnim(m_Channel[ANIM_CHANNEL_NEXT].pHierarchy) );
				float animTime = m_Channel[ANIM_CHANNEL_NEXT].pHierarchy->currentAnim->currentTime;
				RpHAnimHierarchySetCurrentAnimTime(m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy, animTime);

				RpHAnimHierarchyCopy(m_pMainHierarchy, m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy);
				m_Channel[ANIM_CHANNEL_CURRENT].bLoop       = m_Channel[ANIM_CHANNEL_NEXT].bLoop;
				m_Channel[ANIM_CHANNEL_CURRENT].bNotLoopEnd = m_Channel[ANIM_CHANNEL_NEXT].bNotLoopEnd;
				m_Channel[ANIM_CHANNEL_CURRENT].uiAnimKey   = m_Channel[ANIM_CHANNEL_NEXT].uiAnimKey;
				m_Channel[ANIM_CHANNEL_CURRENT].pAnimData   = m_Channel[ANIM_CHANNEL_NEXT].pAnimData;
				m_Channel[ANIM_CHANNEL_CURRENT].fAnimSpeed  = m_Channel[ANIM_CHANNEL_NEXT].fAnimSpeed;
			}
		}
		else
			RpHAnimHierarchyCopy(m_pMainHierarchy, m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy);
		
		RpHAnimHierarchyUpdateMatrices(m_pMainHierarchy);
		m_fOldAnimTime = m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy->currentAnim->currentTime;
	}
	NTL_RETURNVOID();
}
/**
 * Animation를 Change를 한다.
 * \param *pAnim 현재 Play를 할 RtAnimation
 * \param bLoop Loop Animation인가(이부분은 아직 코딩 안함)
 */
void CNtlPLAnimSet::SetChangeAnimation(SAnimData *pAnimData, RwBool bLoop, unsigned int uiAnimKey)
{
	NTL_FUNCTION("CNtlPLAnimSet::SetChangeAnimation");
	NTL_ASSERT(pAnimData != NULL, "pAnimData == NULL");

	if(m_pTargetClump && m_bActiveFlag)
	{
		if(m_bAnimChanging == FALSE)
		{
			RpHAnimHierarchySetCurrentAnim(m_Channel[ANIM_CHANNEL_NEXT].pHierarchy, 
				(RtAnimAnimation *)pAnimData->pAnimRes->GetData());
		}
		else
		{
			//Next Animation의 상태 및 Ani 정보를 Current로 옮긴다.
			RpHAnimHierarchySetCurrentAnim( m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy,
											RpHAnimHierarchyGetCurrentAnim(m_Channel[ANIM_CHANNEL_NEXT].pHierarchy) );
			
			float animTime = m_Channel[ANIM_CHANNEL_NEXT].pHierarchy->currentAnim->currentTime;
			RpHAnimHierarchySetCurrentAnimTime(m_Channel[ANIM_CHANNEL_CURRENT].pHierarchy, animTime);
			RpHAnimHierarchySetCurrentAnim(m_Channel[ANIM_CHANNEL_NEXT].pHierarchy, 
				(RtAnimAnimation *)pAnimData->pAnimRes->GetData());
		}
		
		m_Channel[ANIM_CHANNEL_NEXT].bLoop = bLoop;
		m_Channel[ANIM_CHANNEL_NEXT].uiAnimKey = uiAnimKey;
		m_Channel[ANIM_CHANNEL_NEXT].pAnimData = pAnimData;
		m_Channel[ANIM_CHANNEL_NEXT].bNotLoopEnd = FALSE;
		m_Channel[ANIM_CHANNEL_NEXT].fAnimSpeed = 1.f;
		m_fBlendAlpha = m_fDefaultBlendAlpha;	
		m_bAnimChanging = TRUE;
		m_eBlendType = BLEND_TWEEN;
		
		m_fBlendTargetAlpha    = m_fDefaultBlendTargetAlpha;
		m_fBlendTargetInterval = m_fDefaultBlendTargetInterval;
	}
	NTL_RETURNVOID();
}


void CNtlPLAnimSet::SetChangeChannelAnimation(EAnimChannel eChannel, RtAnimAnimation *pAnim)
{
	RpHAnimHierarchySetCurrentAnim(m_Channel[eChannel].pHierarchy, pAnim);
}

/**
 *
 * \param blendType BlendType
 * \param fBlendTargetAlpha    변해야 할 Blend의 값(0 - 1)
 * \param fBlendTargetInterval TargetAlpha까지 Blend가 되어질 시간(초)
 * \return 
 */
RwBool CNtlPLAnimSet::SetChangeBlend(EBlendType blendType, RwReal fBlendTargetAlpha, RwReal fBlendTargetInterval)
{
	NTL_FUNCTION("CNtlPLAnimSet::SetChangeBlend");
	if(m_bAnimChanging)
	{
		m_eBlendType = blendType;
		m_fBlendTargetAlpha = fBlendTargetAlpha;
		m_fBlendTargetInterval = fBlendTargetInterval;
	}
	else
		NTL_RETURN(FALSE);
	
	NTL_RETURN(TRUE);
}

RwBool	CNtlPLAnimSet::SetChangeAnimSpeed(RwReal fSpeed)
{
	NTL_FUNCTION("CNtlPLAnimSet::SetChangeAnimSpeed");
	NTL_ASSERT(fSpeed > 0.f, "fSpeed <= 0");
	if(m_bAnimChanging)
	{
		m_Channel[ANIM_CHANNEL_NEXT].fAnimSpeed = fSpeed;
	}
	else
		NTL_RETURN(FALSE);

	NTL_RETURN(TRUE);
}
