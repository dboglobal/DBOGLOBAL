#pragma once

#include "NtlSLEvent.h"

class RWS::CEventHandler;
class CNtlSobActor;
class CNtlPLCharacter;
class CNtlPLItem;
class CNtlInstanceEffect;
class CNtlSobProxySystemEffect;
struct SEquipItem;



/**
* \ingroup NtlSimulation
* \brief 변신관련 비쥬얼을 처리하는 클래스
*
* \date 2008-10-07
* \author agebreak
*/
class CNtlSobCharProxyTransform : public RWS::CEventHandler
{
public:
    CNtlSobCharProxyTransform(CNtlSobActor* pActor, CNtlPLCharacter* pPLCharacter);
    virtual ~CNtlSobCharProxyTransform(void);

    void	        Update(RwReal fElapsed);
    void	        HandleEvents(RWS::CMsg &pMsg);
    void            Destory();

    void            SetActor(CNtlSobActor* pActor, CNtlPLCharacter* pPLCharacter);          ///< 캐릭터를 설정한다. (포인터가 변경된 경우)        
    void            PostChangeAdult();                                                      ///< 어른/아이로 변경후 호출되는 함수
    RwBool          IsTransform()   {return m_bIsTransform;}        ///< 변신했는지 여부를 반환한다.    

    // 월드 진입/삭제시의 이벤트 함수
    void            AddWorld();
    void            RemoveWorld();

protected:
    // Event
    void            EventSobTransform(SNtlEventTransform* pData);     ///< 변신 이벤트 처리
    void            EventSobSkillAction(RWS::CMsg& pMsg);             ///< 스킬 사용 이벤트를 처리한다.
    void            EventSobChangeAdult(RWS::CMsg& pMsg);             ///< 성인으로 변신을 처리한다.

    // Transform
    void            OnTransform_SuperSaiyan(RwBool bTransform);     ///< 초사이어인 변신 처리
    void            OnTransform_KaiOuKen(RwBool bTransform, BYTE byLevel);        ///< 계왕권 변신 처리
    void            OnTransform_BigNamek(RwBool bTransform);        ///< 거대 나메크 변신 처리
    void            OnTransform_PureMajin(RwBool bTransform);       ///< 순수 마인 변신 처리
    void            OnTransform_Mob(RwUInt32 tblIdx);               ///< 몹 변신 처리
    void            OnTransform_Candy(RwBool bTransform);           ///< 캔디 스킬 변신 처리
    void            DetachEffect();                                 ///< 변신시의 부착 이펙트 해제

	void            OnTransform_CinematicScale(float fScale, float fTime);

    // Super Saiyan
    void            AttachEffectSuperSaiyan(CNtlPLCharacter* pPLCharacter);                      ///< 초사이어인 이펙트 부착
    CNtlPLItem*     GetTransformItem();                             ///< 초사이어인 변신 아이템을 반환한다.

    // 계왕권
    void            AttachEffectKaiouKen(CNtlPLCharacter* pPLCharacter, BYTE byLevel);    ///< 계왕권 이펙트 생성    

    // 거대 나멕
    void            DetachWeapon();                         ///< 원래 장착하고 있는 무기를 해제한다.
    void            AttachWeapon();                         ///< 원래 장착하고 있던 무기를 장착한다.
    void            ChangeWeapon(RWS::CMsg& pMsg);          ///< 변신 상태에서 무기를 변경하였을때

    // 순수 마인
    CNtlPLCharacter* CreatePureMajinModel();                ///< 순수 마인 모델을 생성하고 반환한다.

    // 캔디 변신
    CNtlPLCharacter* CreateCandyModel();                    ///< 캔디 모델을 생성하고 반환한다.

protected:
    CNtlSobActor*       m_pActor;                           ///< Actor 객체
    CNtlPLCharacter*    m_pPLCharacter;                     ///< Actor의 PLEntity
    SNtlEventTransform  m_transFormData;                    ///< 변신 정보 구조체
    CNtlPLCharacter*    m_pPLCharacterUI;                   ///< UI에 표현되는 캐릭터 (아바타만 사용)
	CNtlPLCharacter*    m_pPLCharacterUI_Dogi;              ///< Dogi UI에 표현되는 캐릭터 (아바타만 사용)
    RwBool              m_bIsTransform;                     ///< 변신했는지 여부 플래그    
    RwUInt32            m_uiTransformId;                    ///< 무엇으로 변신했는지의 ID
    RwBool              m_bReadyTransform;                  ///< 변신 스킬을 사용했다
    RwBool              m_bScheduleLoadingCheck;            ///< 스케쥴 로딩 체크 플래그
    BYTE                m_byTransformLevel;                 ///< 변신 레벨    
    std::vector<CNtlInstanceEffect*> m_vecEffect;           ///< 변신시의 이펙트    

    // 초사이야인
    CNtlPLItem*         m_pItemTransform;                   ///< 초사이야인 변신시의 헤어
	CNtlPLItem*         m_pItemTransformUI;                 ///< UI용 아이템
    CNtlPLItem*         m_pItemTransformUI_Dogi;            ///< Dogi UI용 아이템
    RwTexture*          m_pFaceTransform;                   ///< 초사이야인 변신 얼굴 텍스쳐
    CNtlSobProxySystemEffect* m_pSystemEffectColor[2];      ///< 초사이어인 변신시 Effect Color 컨트롤러

    // 계왕권
    CNtlSobProxySystemEffect* m_pKaiOuKenColor[2];          ///< 계왕권 변신시 컬러 컨트롤러

    // 거대 나멕
    SEquipItem*         m_pItemOrgWeapon;                   ///< 나메크 변신시에 원래 차고 있던 무기 
    SEquipItem*         m_pItemOrgSubWeapon;                ///< 나메크 변신시에 원래 차고 있던 서브 웨폰
    RwReal              m_fOrgAttackRange;                  ///< 변신전의 원래 공격 범위

    // 순수 마인    
    CNtlPLCharacter*    m_pMajinOrgModel;                   ///< 원래 마인 모델
    CNtlPLCharacter*    m_pMajinOrgModelUI;                 ///< UI용 원래 마인 모델
	CNtlPLCharacter*    m_pMajinOrgModelUI_Dogi;            ///< Dogi UI용 원래 마인 모델
    CNtlPLCharacter*    m_pMajinPureModel;                  ///< 변신한 순수 마인 모델    
    CNtlPLCharacter*    m_pMajinPureModelUI;                ///< UI용 순수 마인 모델    
	CNtlPLCharacter*    m_pMajinPureModelUI_Dogi;           ///< Dogi UI용 순수 마인 모델    

    // 캔디 변신
    CNtlPLCharacter*    m_pCandyOrgModel;                   ///< 캔디로 변신하기전 원래 모델
    CNtlPLCharacter*    m_pCandyOrgModelUI;                 ///< UI용 원래 모델
	CNtlPLCharacter*    m_pCandyOrgModelUI_Dogi;            ///< Dogi UI용 원래 모델
    CNtlPLCharacter*    m_pCandyModel;                      ///< 캔디 변신 모델
	CNtlPLCharacter*    m_pCandyModelUI;                    ///< UI용 캔디 모델
	CNtlPLCharacter*    m_pCandyModelUI_Dogi;               ///< Dogi UI용 캔디 모델
};
