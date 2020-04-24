#pragma once

class CNtlSob;
class CNtlPLEntity;

/**
 * \ingroup NtlSimulation
 * \brief 공유 타겟 마크를 관리하는 클래스
 *
 * \date 2009-08-13
 * \author agebreak
 */
class CNtlShareTargetMark
{
public:
    // 공유 타겟 타입
    enum EShareTargetType
    {
        SHARE_TARGET_NORMAL,        
        SHARE_TARGET_TARGET,        
        SHARE_TARGET_ATTACK,
        SHARE_TARGET_NONE,
    };

    CNtlShareTargetMark(CNtlSob* pSobObject);
    virtual ~CNtlShareTargetMark();

    void    CreateShareTargetMark(RwUInt8 bySlot, RwInt32 type);               ///< 공유타겟 마크를 생성한다.
    void    DeleteShareTargetMark();                                            ///< 공유타겟 마크를 해제한다.
    RwBool  IsShareTargeting();                                                 ///< 공유타겟이 설정되어있는지를 반환한다.
    RwUInt8 GetSlot() {return m_bySlot;}                                        ///< 현재 설정된 슬롯을 반환한다.    

protected:

protected:
    CNtlSob*            m_pSobObj;                  ///< 공유타겟이 붙을 메인 엔티티
    CNtlPLEntity*       m_pShareTargetMark;         ///< 공유타겟 이펙트
    RwUInt8             m_bySlot;                   ///< 공유타겟 넘버(슬롯)
    EShareTargetType    m_type;                     ///< 공유타겟 타입  
    
};