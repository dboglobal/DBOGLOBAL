#include "precomp_ntlpresentation.h"
#include "NtlPLFadeController.h"
#include "NtlMath.h"

CNtlPLFadeController::CNtlPLFadeController(void)
:m_fOrgValue(1.0f)
,m_fDestValue(0.0f)
,m_fValue(0.0f)
,m_fFadeTime(0.0f)
,m_fTime(0.0f)
,m_bEnd(TRUE)
{
}

CNtlPLFadeController::~CNtlPLFadeController(void)
{
}

void CNtlPLFadeController::Set( RwReal fOrgValue, RwReal fDestValue, RwReal fFadeTime ) 
{
    m_fOrgValue = fOrgValue;
    m_fDestValue = fDestValue;
    m_fFadeTime = fFadeTime;
    m_fTime = 0.0f;
    m_bEnd = FALSE;
}

void CNtlPLFadeController::Update( RwReal fElapsedTime ) 
{
    if(m_bEnd)
        return;

    m_fTime += fElapsedTime;
    if(m_fTime >= m_fFadeTime)
    {
        m_fValue = m_fDestValue;
        m_bEnd = TRUE;
    }
    else
    {
        m_fValue = CNtlMath::Interpolation(m_fOrgValue, m_fDestValue, m_fTime / m_fFadeTime);
    }
}