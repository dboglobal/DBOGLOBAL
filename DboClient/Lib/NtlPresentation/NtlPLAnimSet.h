/*****************************************************************************
 *
 * File			: NTLAnimSet.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 20	
 * Abstract		: NTL PLAnimSet
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef _CNTL_PL_ANIM_SET_
#define _CNTL_PL_ANIM_SET_

/*
#include <rwcore.h>
#include <rtanim.h>
#include <rpworld.h> 
#include <rtquat.h> 
#include <rtanim.h>
#include <rphanim.h>
#include <vector>
*/

#include "NtlDebug.h"
#include "NtlPLCharacterAnimTable.h"

/**
 * \ingroup Client
 * Animation 관련 처리하는 Class 이다.
 * 
 */

enum EAnimChannel
{
	ANIM_CHANNEL_CURRENT = 0,
	ANIM_CHANNEL_NEXT,
	ANIM_CHANNEL_END,
};

enum EBlendType
{
	BLEND_MIX,		//이전의 Animation를 Play를 하고, 현재의 Animation도 Play를 하면서 보간을 한다.
	BLEND_TWEEN		//이전의 Animation은 멈추고, 현재의 Animation의 Start까지 보간을 한다.
};


struct SAnimChannel
{
	RpHAnimHierarchy	*pHierarchy;
	RwBool				bLoop;				//loop로 작동을 해야 하는 Animation 인지
	RwBool				bNotLoopEnd;		//loop가 아닌 Animation이 끝났는지
	RwReal				fAnimSpeed;         //Animation Speed
	unsigned int		uiAnimKey;

	SAnimData			*pAnimData;

	SAnimChannel() 
	{ 
		pHierarchy      = NULL;
		bLoop			= FALSE;
		pAnimData		= NULL;
		bNotLoopEnd     = FALSE;
		fAnimSpeed      = 1.f;
	};
};

class CNtlPLCharacter;
class CNtlPLAnimSet
{
public:
	CNtlPLAnimSet();
	~CNtlPLAnimSet();
private:
	RpClump	*m_pTargetClump;						 //Target Clump
	CNtlPLCharacter *m_pOwner;                       //현재 Class를 소유하는 Class

	RpHAnimHierarchy *m_pMainHierarchy;				 //Character Main Hierarchy
	
	
	SAnimChannel		m_Channel[ANIM_CHANNEL_END]; //AnimChannel
	EBlendType			m_eBlendType;

	RwReal	m_fBlendAlpha;							 // Blend 값 (0.. 1)
	RwReal	m_fBlendTargetAlpha;					 // Target Blend
	RwReal	m_fBlendTargetInterval;					 // Blend 될 시간(초)

	RwBool	m_bAnimChanging;
	
	RwReal	m_fDefaultBlendAlpha;
	RwReal  m_fDefaultBlendTargetAlpha;
	RwReal	m_fDefaultBlendTargetInterval;
	
	RwBool	m_bActiveFlag;
	
	RwReal	m_fOldAnimTime;
private:
	void SendAnimEnd();
	void SendAnimHit();
	void SendFootStep();

public:
	void Create(RpClump *pClump, RtAnimAnimation *pDefaultAnim, CNtlPLCharacter *pOwner);
	void CreateAnimation(RtAnimAnimation *pDefaultAnim);


	void Destroy();
	void Update(float fElapsed);
	
	void	SetChangeAnimation(SAnimData *pAnimData, RwBool bLoop, unsigned int uiAnimKey);
	void	SetChangeChannelAnimation(EAnimChannel eChannel, RtAnimAnimation *pAnim);

	RwBool	SetChangeBlend(EBlendType blendType, RwReal fBlendTargetAlpha = 1.f, RwReal fBlendTargetInterval = 0.1f);
	RwBool	SetChangeAnimSpeed(RwReal fSpeed = 1.f);

	RpHAnimHierarchy *GetHierarchy() { NTL_ASSERTE(m_pMainHierarchy != NULL); return m_pMainHierarchy; }
};

#endif
