#include "precomp_ntlpresentation.h"
#include "NtlPLAccelController.h"

CNtlPLAccelController::CNtlPLAccelController(void)
{
    m_fStartVal     = 0.0f;
    m_fEndVal       = 0.0f;
    m_fCurrentVal   = 0.0f;
    m_fStartVelocity = 0.0f;
    m_fAccel        = 0.0f;
    m_fCurrVel      = 0.0f;
	m_fCurrProgress = 0.0f;
    m_bUpdate       = FALSE;
}

CNtlPLAccelController::~CNtlPLAccelController(void)
{

}

// fStartVal : 시작값
// fEndVal : 목표값
// fStartVelocity : 속도 (증가값/초)
// fAccel : 가속도 (증가속도/초)
void CNtlPLAccelController::SetAccel( RwReal fStartVal, RwReal fEndVal, RwReal fStartVelocity, RwReal fAccel ) 
{
    m_fStartVal = m_fCurrentVal =  fStartVal;
    m_fEndVal = fEndVal;
    m_fStartVelocity = m_fCurrVel = fStartVelocity;
    m_fAccel = fAccel;                 ///< 가속도는 Update시마다 계산하는것을 줄이기 위해서 미리 제곱해둔다.
    m_bUpdate = TRUE;
}

RwBool CNtlPLAccelController::Update( RwReal fElapsed ) 
{
    if(!m_bUpdate)
        return FALSE;

    // 속도 증가
    m_fCurrVel += m_fAccel * fElapsed;

    // 값 증가
    if(m_fEndVal >= m_fStartVal)
    {
        m_fCurrentVal += m_fCurrVel * fElapsed;
    }
    else
    {
        m_fCurrentVal -= m_fCurrVel * fElapsed;
    }

    if((m_fEndVal >= m_fStartVal && m_fCurrentVal >= m_fEndVal) ||
       (m_fEndVal < m_fStartVal && m_fCurrentVal <= m_fEndVal))
    {
        m_fCurrentVal = m_fEndVal;        
        m_bUpdate = FALSE;        
    }

	// Current Progress
	m_fCurrProgress = RwFabs(( m_fStartVal-m_fCurrentVal)/(m_fStartVal-m_fEndVal));

    return m_bUpdate;
}

void CNtlPLAccelController::Stop()
{
	m_bUpdate = false;
}

RwReal CNtlPLAccelController::GetCurrentProgress()
{
	return m_fCurrProgress;
}