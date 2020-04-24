/*****************************************************************************
*
* File			: NtlPLFadeController.h
* Author		: agebreak
* Copyright		: (주)NTL
* Date			: 2007. 11. 21	
* Abstract		: Fade In/Out용 값을 위한 컨트롤러
*****************************************************************************
* Desc         : Entity에는 AlphaWeight라는 컨트롤러가 있지만, Entity외에는
*                적용할 수 없기 때문에 새로 만든다.
*
*****************************************************************************/

#pragma once

/// Fade 값 계산용 컨트롤러
class CNtlPLFadeController
{
public:
    CNtlPLFadeController(void);
    virtual ~CNtlPLFadeController(void);

    void Set(RwReal fOrgValue, RwReal fDestValue, RwReal fFadeTime);
    void Update(RwReal fElapsedTime);
    
    RwReal GetValue() {return m_fValue;}    ///< 현재의 Fade 값을 반환한다.
    RwBool IsEnd() {return m_bEnd;}         ///< Fade가 끝났는지를 반환한다.

protected:
    RwReal m_fOrgValue;                     ///< 원본값
    RwReal m_fDestValue;                    ///< 끝값
    RwReal m_fValue;                        ///< 현재값
    RwReal m_fFadeTime;                     ///< Fade Time
    RwReal m_fTime;                         ///< 현재 Time
    RwBool m_bEnd;                          ///< Fade가 끝났는지 여부
};
