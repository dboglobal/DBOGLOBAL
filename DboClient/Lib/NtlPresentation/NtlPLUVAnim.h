#pragma once


/**
 * \ingroup NtlPresentation
 * \brief UVAnim을 처리하는 클래스
 * \date 2006-07-11
 * \author agebreak
 */
class CNtlPLUVAnim
{
public:
    CNtlPLUVAnim(void);
    virtual ~CNtlPLUVAnim(void);

    RwBool Create(const char* szUVFileName);
    RwBool SetClump(RpClump* pClump);
    RwBool Create(const char* szUVFileName, RpClump* pClump);       ///< UVAnim을 생성한다.
    void   Destory();                                               ///< 할당된 메모리를 해제한다.
    void   Update(RwReal fElapsedTime);                             ///< UVAim을 Update한다.
    void   SetUVAnimSpeed(RwReal fSpeed) {m_fUVAnimTime = fSpeed;}  ///< UVAnim의 속도를 지정한다

protected:
    RtDict* UVAnimDictLoad(const RwChar* strFullName);              ///< *.uva 파일을 로드한다.

protected:
    RtDict*							m_pUvAnimDict;                 ///< UVAnim dict 객체
    RwSList*						m_pAnimationMaterialList;      ///< UVAnim을 적용할 Material list
    RwReal                          m_fUVAnimTime;                 ///< UVAnim Speed
    
    
};
