#include "precomp_ntlpresentation.h"
#include "NtlPLSceneManager.h"
#include "NtlDebug.h"
#include "NtlPLLinkEffect.h"

CNtlPLLinkEffect::CNtlPLLinkEffect(void)
{
}

CNtlPLLinkEffect::~CNtlPLLinkEffect(void)
{
    for(size_t i = 0; i < m_vecLinkEffect.size(); ++i)
    {
        if(m_vecLinkEffect[i])
        {
            m_vecLinkEffect[i]->SetVisible(FALSE);
            GetSceneManager()->DeleteEntity(m_vecLinkEffect[i]);        
            m_vecLinkEffect[i] = NULL;
        }        
    }

    m_vecLinkEffect.clear();
}

/**
 * LinkEffect를 생성하여 부착한다.
 * \param pAttach 부착할 부모 객체
 * \param pEventLinkEffect 이펙트를 생성할 프로퍼티 객체
 * \param vLinkEffect Instance Effect를 생성할 vector
 * return 생성된 Effect 객체, 실패시엔 NULL을 반환
 */
CNtlInstanceEffect* CNtlPLLinkEffect::AttachLinkEffect(CNtlPLAttach* pAttach, SEventLinkEffect* pEventLinkEffect)
{
    if(!pAttach || !pEventLinkEffect)
        return NULL;

    // Add an effect to the screen
    CNtlInstanceEffect* pEffect = NULL;
    if(pEffect == NULL)
    {
	//	DBO_WARNING_MESSAGE("pEventLinkEffect->chEffectName: " << pEventLinkEffect->chEffectName);
        pEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pEventLinkEffect->chEffectName);
		if (!pEffect)
		{
			return NULL;
		}

        // Auto Delete 이펙트는 사용할수 없다.
        if(pEffect->IsAutoDelete())
        {
            pEffect->SetVisible(FALSE);
            GetSceneManager()->DeleteEntity(pEffect);
            return NULL;
        }
    }

    // Attach 한다.
    RwBool bSuccess = FALSE;
    if(pEventLinkEffect->bAttachBone)
    {
        SPLAttachAttr attr;
        attr.vOffsetPos = pEventLinkEffect->vOffsetPos;        
         bSuccess = pAttach->AttachBone(pEffect, pEventLinkEffect->chBoneName, &attr);
    }
    else
    {
		bSuccess = pAttach->AttachWorldPosAndRotate(pEffect, pEventLinkEffect->vOffsetPos);
    }

    if(!bSuccess)       // Attach가 정상적으로 되지 않았을때 (본이 없다던가..)
    {
        GetSceneManager()->DeleteEntity(pEffect);
        return NULL;
    }

    // 부모의 Visible 속성을 따른다.
    pEffect->SetVisible(pAttach->IsVisible());

    m_vecLinkEffect.push_back(pEffect);    

    return pEffect;
}

/**
* LinkEffect를 제거한다.
* \param pAttach LinkEffect를 제거할 부모 객체
* \param pEventLinkEffect 제거할 LinkEffect
* \param vLinkEffect Instance Effect를 제거할 vector
* return 성공 유무
*/
RwBool CNtlPLLinkEffect::DetachLinkEffect(CNtlPLAttach* pAttach, CNtlInstanceEffect* pLinkEffect)
{
    if(!pAttach || !pLinkEffect)
        return FALSE;

    pAttach->Detach(pLinkEffect);
    pLinkEffect->SetVisible(FALSE);
    GetSceneManager()->DeleteEntity(pLinkEffect);    

    // Intance들의 Vector에서 제거한다.    
    VecLinkEffect::iterator it = m_vecLinkEffect.begin();
    for(; it != m_vecLinkEffect.end(); ++it)
    {
        CNtlInstanceEffect* pLinkEfffectInstance = *it;
        if(pLinkEffect == pLinkEfffectInstance)
        {
            m_vecLinkEffect.erase(it);
            break;
        }
    }
    
    return TRUE;
}


/**
 * Effect 인스턴트 리스트중에서 이름에 해당하는 인스턴스 이펙트를 반환한다.
 * \param szName 찾을 이펙트의 이름
 * return 찾은 인스턴스 이펙트 객체. 찾지못하면 NULL을 반환한다.
 */
CNtlInstanceEffect* CNtlPLLinkEffect::GetLinkEffectFromName(RwChar* szName)
{
    if(!szName)
        return NULL;

    for(size_t i = 0; i < m_vecLinkEffect.size(); ++i)
    {
        if(m_vecLinkEffect[i])
        {
            if(strcmp(m_vecLinkEffect[i]->GetName(), szName) == 0)
                return m_vecLinkEffect[i];
        }        
    }

    return NULL;
}

void CNtlPLLinkEffect::SetVisible(BOOL bVisible)
{
    for(size_t i = 0; i < m_vecLinkEffect.size(); ++i)
    {
        if(m_vecLinkEffect[i])
        {
            m_vecLinkEffect[i]->SetVisible(bVisible);
        }
    }
}