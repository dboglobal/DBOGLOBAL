#include "precomp_ntlpresentation.h"
#include "NtlPLEntityBlend.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLEntity.h"


CNtlPLEntityAlphaWeightBlend::CNtlPLEntityAlphaWeightBlend()
{
	m_byBlendType		= PLEAW_END;
	m_fCurrTime			= 0.0f;
	m_fBlendTime		= 1.0f;	
	m_fLifeTime			= 0.0f;    
	m_bLoop				= FALSE;
	m_bCalcWeightValue	= TRUE;
	m_bFinish			= FALSE;

    m_fWeightValue = m_fStartWeightValue = m_fEndWeightValue = 1.0f;
}

CNtlPLEntityAlphaWeightBlend::~CNtlPLEntityAlphaWeightBlend()
{
}

void CNtlPLEntityAlphaWeightBlend::Update(RwReal fElapsed)
{
	m_fCurrTime += fElapsed;
    
    if(!m_bLoop && m_fCurrTime >= m_fLifeTime)
    {
        m_fWeightValue = m_fEndWeightValue;
        m_byBlendType = PLEAW_END;
        m_bFinish = TRUE;            
    }

    if(m_byBlendType == PLEAW_END)      // 블렌드가 끝났거나 블렌드 알파가 아닐때
    {
        if(m_fStartWeightValue != m_fWeightValue)               // 맨 처음 한번만 계산 플래그를 켜기 위한 비교
        {
            m_bCalcWeightValue = TRUE;
            m_fStartWeightValue = m_fWeightValue;
        }
        else
        {
            m_bCalcWeightValue = TRUE;
        }        
    }
    else if(m_byBlendType == PLEAW_BLEND)   // 블렌드 중일때
    {
        if(m_fCurrTime >= m_fBlendTime)
        {
            m_fWeightValue = m_fEndWeightValue;
            m_byBlendType = PLEAW_END;
        }
        else
        {
            m_fWeightValue = CNtlMath::Interpolation(m_fStartWeightValue, m_fEndWeightValue, m_fCurrTime / m_fBlendTime); 
            m_fWeightValue = CNtlMath::Range(m_fWeightValue, m_fStartWeightValue, m_fEndWeightValue);
        }
    }
}

RwBool CNtlPLEntityAlphaWeightBlend::IsCalcWeightValue(void)
{
	return m_bCalcWeightValue;
}

RwBool CNtlPLEntityAlphaWeightBlend::IsFinish(void)
{
	return m_bFinish;
}


void CNtlPLEntityAlphaWeightBlend::SetWeight(RwReal fWeightValue, RwReal fLifeTime, RwBool bLoop)
{
	m_byBlendType	= PLEAW_END;	
    m_fStartWeightValue = -1.0f;            // 처음 한번만 계산하기 위해서 비교값을 넣어준다.
	m_fWeightValue	= fWeightValue;
	m_fLifeTime		= fLifeTime;
	m_bLoop			= bLoop;
}

void CNtlPLEntityAlphaWeightBlend::SetWeight(RwReal fStartWeightValue, RwReal fEndWeightValue, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop)
{
	m_byBlendType		= PLEAW_BLEND;	
	m_fWeightValue		= fStartWeightValue;
	m_fStartWeightValue	= fStartWeightValue;
	m_fEndWeightValue	= fEndWeightValue;
    m_fBlendTime        = fBlendTime;
	m_fLifeTime			= fLifeTime;
	m_bLoop				= bLoop;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPLAtomicAlphaWeightBlend::CNtlPLAtomicAlphaWeightBlend()
{
}

CNtlPLAtomicAlphaWeightBlend::~CNtlPLAtomicAlphaWeightBlend()
{
}

void CNtlPLAtomicAlphaWeightBlend::Update(RwReal fElapsed)
{
	CNtlPLEntityAlphaWeightBlend::Update(fElapsed);
}

void CNtlPLAtomicAlphaWeightBlend::SetWeight(const RwChar *pAtomicName, RwReal fWeightValue, RwReal fLifeTime /*= 0.0f*/, RwBool bLoop /*= TRUE*/)
{
	m_strName = pAtomicName;

	CNtlPLEntityAlphaWeightBlend::SetWeight(fWeightValue, fLifeTime, bLoop);
}

void CNtlPLAtomicAlphaWeightBlend::SetWeight(const RwChar *pAtomicName, RwReal fStartWeightValue, RwReal fEndWeightValue, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop)
{
	m_strName = pAtomicName;

	CNtlPLEntityAlphaWeightBlend::SetWeight(fStartWeightValue, fEndWeightValue, fBlendTime, fLifeTime, bLoop);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CNtlPLEntityBlendController::CNtlPLEntityBlendController(CNtlPLEntity *pPLEntity)
{
	m_pPLEntity = pPLEntity;
}


CNtlPLEntityBlendController::~CNtlPLEntityBlendController()
{
	ListAlphaWeightBlend::iterator it;
	for(it = m_listAlphaBlend.begin(); it != m_listAlphaBlend.end(); it++)
	{
		NTL_DELETE((*it));
	}

	m_listAlphaBlend.clear();


	ListAtomicAlphaWeightBlend::iterator itatomic;
	for(itatomic = m_listAtomicAlphaBlend.begin(); itatomic != m_listAtomicAlphaBlend.end(); itatomic++)
	{
		NTL_DELETE((*itatomic));
	}

	m_listAtomicAlphaBlend.clear();
}

void CNtlPLEntityBlendController::UpdateEntityAlpha(RwReal fElapsed)
{
    if(m_listAlphaBlend.empty())
        return;

	RwBool bCalcWeightValue = FALSE;
	CNtlPLEntityAlphaWeightBlend *pAlphaWeight;
	ListAlphaWeightBlend::iterator it;

	for(it = m_listAlphaBlend.begin(); it != m_listAlphaBlend.end(); )
	{
		pAlphaWeight = (*it);
		pAlphaWeight->Update(fElapsed);

		if(pAlphaWeight->IsCalcWeightValue())
			bCalcWeightValue = TRUE;

		if(pAlphaWeight->IsFinish())
		{
			bCalcWeightValue = TRUE;
			NTL_DELETE(pAlphaWeight);
			it = m_listAlphaBlend.erase(it);
		}
		else
		{
			++it;
		}
	}

	if(bCalcWeightValue)
	{
		RwReal fWeightAlpha = 1.0f;

		for(it = m_listAlphaBlend.begin(); it != m_listAlphaBlend.end(); it++)
		{
			fWeightAlpha = fWeightAlpha * (*it)->GetWeight();
		}

		m_pPLEntity->SetWeightAlpha(fWeightAlpha);
	}
}

void CNtlPLEntityBlendController::UpdateAtomicAlpha(RwReal fElapsed)
{
	if(m_listAtomicAlphaBlend.empty())
        return;

	RwBool bCalcWeightValue = FALSE;
	CNtlPLAtomicAlphaWeightBlend *pAlphaWeight;
	ListAtomicAlphaWeightBlend::iterator it;

	for(it = m_listAtomicAlphaBlend.begin(); it != m_listAtomicAlphaBlend.end(); )
	{
		pAlphaWeight = (*it);
		pAlphaWeight->Update(fElapsed);

		if(pAlphaWeight->IsFinish())
		{
            // 1.0f setting.
            m_pPLEntity->SetAtomicWeightAlpha(pAlphaWeight->GetAtomicName().c_str(), 1.0f);

            it = m_listAtomicAlphaBlend.erase(it);
			NTL_DELETE(pAlphaWeight);
		}
		else
		{
			if(pAlphaWeight->IsCalcWeightValue())
			{
				m_pPLEntity->SetAtomicWeightAlpha(pAlphaWeight->GetAtomicName().c_str(), pAlphaWeight->GetWeight());
			}

			it++;
		}
	}
}


void CNtlPLEntityBlendController::Update(RwReal fElapsed)
{
	UpdateEntityAlpha(fElapsed);
	UpdateAtomicAlpha(fElapsed);
}


CNtlPLAtomicAlphaWeightBlend* CNtlPLEntityBlendController::FindAtomicAlphaBlend(const RwChar *pAtomicName)
{
	ListAtomicAlphaWeightBlend::iterator it;
	for(it = m_listAtomicAlphaBlend.begin(); it != m_listAtomicAlphaBlend.end(); it++)
	{
		std::string& strName = (*it)->GetAtomicName();
		if(strName == pAtomicName)
		{
			return (*it);
		}
	}

	return NULL;
}

// 일반 알파 블렌딩을 추가한다.
// 새로운 알파 블렌딩 객체를 리스트에 추가하고, 그 객체를 반환한다.
CNtlPLEntityAlphaWeightBlend* CNtlPLEntityBlendController::AddAlpha(RwReal fWeightAlpha, RwReal fLifeTime, RwBool bLoop)
{
	CNtlPLEntityAlphaWeightBlend *pAlphaBlend = NTL_NEW CNtlPLEntityAlphaWeightBlend;
	pAlphaBlend->SetWeight(fWeightAlpha, fLifeTime, bLoop);

	m_listAlphaBlend.push_back(pAlphaBlend);

    return pAlphaBlend;
}

// time에 따라 Fade 되는 알파 블렌딩을 추가한다.
// 새로운 알파 블렌딩 객체를 리스트에 추가하고, 그 객체를 반환한다.
CNtlPLEntityAlphaWeightBlend* CNtlPLEntityBlendController::AddAlpha(RwReal fStartWeightAlpha, RwReal fEndWeightAlpha, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop)
{
	CNtlPLEntityAlphaWeightBlend *pAlphaBlend = NTL_NEW CNtlPLEntityAlphaWeightBlend;
	pAlphaBlend->SetWeight(fStartWeightAlpha, fEndWeightAlpha, fBlendTime, fLifeTime, bLoop);

	m_listAlphaBlend.push_back(pAlphaBlend);

    return pAlphaBlend;
}

CNtlPLAtomicAlphaWeightBlend* CNtlPLEntityBlendController::AddAtomicAlpha(const RwChar *pAtomicName, RwReal fWeightAlpha, RwReal fLifeTime /*= 0.0f*/, RwBool bLoop /*= TRUE*/)
{
    // 한 Atomic에는 하나의 Alpha Blend 객체만 적용된다.
    if(FindAtomicAlphaBlend(pAtomicName))
        return NULL;

	CNtlPLAtomicAlphaWeightBlend *pAlphaBlend = NTL_NEW CNtlPLAtomicAlphaWeightBlend;
	pAlphaBlend->SetWeight(pAtomicName, fWeightAlpha, fLifeTime, bLoop);

	m_listAtomicAlphaBlend.push_back(pAlphaBlend);

    return pAlphaBlend;
}

CNtlPLAtomicAlphaWeightBlend*  CNtlPLEntityBlendController::AddAtomicAlpha(const RwChar *pAtomicName, RwReal fStartWeightAlpha, RwReal fEndWeightAlpha, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop)
{
    // 한 Atomic에는 하나의 Alpha Blend 객체만 적용된다.
    if(FindAtomicAlphaBlend(pAtomicName))
        return NULL;

	CNtlPLAtomicAlphaWeightBlend *pAlphaBlend = NTL_NEW CNtlPLAtomicAlphaWeightBlend;
	pAlphaBlend->SetWeight(pAtomicName, fStartWeightAlpha, fEndWeightAlpha, fBlendTime, fLifeTime, bLoop);

	m_listAtomicAlphaBlend.push_back(pAlphaBlend);

    return pAlphaBlend;
}


void CNtlPLEntityBlendController::AddAlphaBlend( CNtlPLEntityAlphaWeightBlend* pAlphaBlend ) 
{
    NTL_ASSERT(pAlphaBlend, "Is NULL");
    if(!pAlphaBlend)
        return;

    m_listAlphaBlend.push_back(pAlphaBlend);
}


void CNtlPLEntityBlendController::RemoveAlpha(CNtlPLEntityAlphaWeightBlend *pAlphaBlend)
{
	ListAlphaWeightBlend::iterator it;
	for(it = m_listAlphaBlend.begin(); it != m_listAlphaBlend.end(); it++)
	{
		if( (*it) == pAlphaBlend)
		{
			NTL_DELETE((*it));
			m_listAlphaBlend.erase(it);
			return;
		}
	}
}

void CNtlPLEntityBlendController::RemoveAtomicAlpha(CNtlPLAtomicAlphaWeightBlend *pAtomicAlphaBlend)
{
	ListAtomicAlphaWeightBlend::iterator it;
	for(it = m_listAtomicAlphaBlend.begin(); it != m_listAtomicAlphaBlend.end(); it++)
	{
		if( (*it) == pAtomicAlphaBlend)
		{
			NTL_DELETE((*it));
			m_listAtomicAlphaBlend.erase(it);
			return;
		}
	}
}
