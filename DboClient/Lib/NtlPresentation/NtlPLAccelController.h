/*****************************************************************************
*
* File			: NtlPLAccelController
* Author		: agebreak
* Copyright		: (주)NTL
* Date			: 2007. 12. 29	
* Abstract		: 가속도 계산용 컨트롤러
*****************************************************************************
* Desc         : UI의 슬라이드등에서 사용되는 가속도 계산용 컨트롤러
*
*****************************************************************************/

#pragma once

/// 가속도 계산 컨트롤러
class CNtlPLAccelController
{
public:
    CNtlPLAccelController(void);
    ~CNtlPLAccelController(void);
	
    void    SetAccel(RwReal fStartVal, RwReal fEndVal, RwReal fStartVelocity, RwReal fAccel);       /// 값을 설정한다.
    RwBool  Update(RwReal fElapsed);                           ///< 업데이트
	void	Stop();
    RwReal  GetCurrentValue() {return m_fCurrentVal;}          ///< 현재값
    RwReal  GetCurrentVelocity() {return m_fCurrVel;}          ///< 현재 속도
	RwReal	GetCurrentProgress();								///< 0.0(시작) ~ 1.0(끝) 기준의 현재 진행

protected:
    RwReal  m_fStartVal;                      ///< 시작값
    RwReal  m_fEndVal;                        ///< 끝값
    RwReal  m_fCurrentVal;                    ///< 현재값
    RwReal  m_fStartVelocity;                 ///< 시작 속도
    RwReal  m_fAccel;                         ///< 가속도
    RwReal  m_fCurrVel;                       ///< 현재 속도
	RwReal	m_fCurrProgress;				  ///< 현재 진행치 ( 0.0f ~ 1.0f )
    RwBool  m_bUpdate;                        ///< 업데이트 유무 플래그
};
