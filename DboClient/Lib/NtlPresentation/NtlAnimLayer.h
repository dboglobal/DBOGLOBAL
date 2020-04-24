/*****************************************************************************
 *
 * File			: NTLAnimLayer.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2006. 4. 11	
 * Abstract		: NTL NtlAnimLayer
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PL_ANIM_LAYER_H__
#define __NTL_PL_ANIM_LAYER_H__

#include "NtlInstanceAnimData.h"
#include "NtlAnimChannel.h"
#include "NtlCallbackWidget.h"

enum EAnimChannel
{
	ANIM_CHANNEL_CURRENT = 0,
	ANIM_CHANNEL_NEXT,
	ANIM_CHANNEL_END,
};

enum EBlendType
{
	BLEND_MIX,							///< Current를 Play를 하고 Next를 Play를 하면서 보간을 한다.
	BLEND_TWEEN							///< Current를 Stop하고 Next의 Start까지 보간을 한다.
};

struct SBlendData
{
	RwReal		fBlendAlpha;			///< Current Blend값(0.f - 1.f)
	RwReal		fBlendTargetAlpha;		///< Target Blend값
	RwReal		fBlendTargetInterval;	///< 변화량
	EBlendType	eBlendType;				///< BlendType

	SBlendData() : fBlendAlpha(0.f), fBlendTargetAlpha(1.f), fBlendTargetInterval(0.1f),
	               eBlendType(BLEND_TWEEN) {}
};

/*!
 * \Animation
 * Animation의 Blending을 처리하기 위한 Class	
 * 
 */
class CNtlAnimLayer
{
public:
	CNtlAnimLayer();
	virtual ~CNtlAnimLayer() {};

protected:
	RpHAnimHierarchy	*m_pBaseHierarchy;					///< Base Hierarchy Reference
	CNtlAnimChannel		m_AnimChannel[ANIM_CHANNEL_END];	///< Blending은 최대 2개
	
	SBlendData			m_CurBlendData;						///< 현재 Blend 값
	SBlendData			m_DefaultBlendData;					///< Default Blend 값
	RwBool				m_bBlendChange; 					///< Blending 중인지

	RwInt32				m_iLastSendEventIndex;				///< 마지막으로 보낸 Event Index
	RwReal				m_fLastSendEventTime;				///< 마지막으로 보낸 Event를 보낸 시간

	CNtlCallbackVPParam1	*m_pCallBack;

public:
	RwBool	Create(RpHAnimHierarchy *pBaseHierarchy);
	RwBool	Update(float fElapsed);
	void	Destroy();
	
	RwBool	SetAllAnimation(SInstanceAnimData *pInstanceAnimData, RwReal fStartTime = 1.f, RwBool bLoop = TRUE);
	RwBool	SetAnimation(SInstanceAnimData *pInstanceAnimData, RwReal fStartTime = 1.f, RwBool bLoop = TRUE, RwBool bBlending = TRUE);
	RwBool	SetBlend(EBlendType eBlendType, RwReal fBlendAlpha = 0.f, RwReal fBlendInterval = 0.1f);
	
	RwReal	GetCurrentAnimTime();
    void    SetCurrentAnimTime(RwReal fCurrTime);				///< 애니메이션의 현재 플레이 타임을 설정한다. (By agebreak 2006.05.01)
	RwReal	GetDurationAnimTime();								///< 애미메이션의 전체 시간

	RwBool	GetCurrentAnimEnd();

	RwBool	IsBlendEnable(void);

    void    OnAfterAnimEventRun();                              ///< 현재 시간 이후 Anim Event를 모두 실행시킨다.
	
	template <class Callbackclass>
	void SetCallBack(Callbackclass *cbclass,int (Callbackclass::*callback)(void* EventData))
	{
		m_pCallBack = NTL_NEW CNtlCallbackVPWidget1<Callbackclass> (cbclass,callback);
	}

};

#endif
