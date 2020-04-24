#pragma once

#include "NtlPLAttach.h"
#include "NtlAnimEventData.h"

typedef std::vector<CNtlInstanceEffect*> VecLinkEffect;
/**
 * \ingroup NtlPresentation
 * \brief Link Effect 클래스 (Model에 계속 붙어있는 이펙트) 
 * \date 2006-07-14
 * \author agebreak
 */
class CNtlPLLinkEffect
{
public:
    CNtlPLLinkEffect(void);
    virtual ~CNtlPLLinkEffect(void);
    
    CNtlInstanceEffect* AttachLinkEffect(CNtlPLAttach* pAttach, SEventLinkEffect* pEventLinkEffect);    
    RwBool DetachLinkEffect(CNtlPLAttach* pAttach, CNtlInstanceEffect* pLinkEffect);
    CNtlInstanceEffect* GetLinkEffectFromName(RwChar* szName);          ///< 이름으로 LinkEffect Instance를 검색하여 반환한다. (모델툴에서만 사용)
    void SetVisible(BOOL bVisible);         ///< 모든 LinkEffect의 Visible을 설정한다. 

public:
    VecLinkEffect m_vecLinkEffect;          ///< Link Effect Instance들의 벡터

};
