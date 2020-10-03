/*****************************************************************************
*
* File			: NtlSobCharDecorationProxy.h
* Author		: agebreak
* Copyright	: (주)NTL
* Date			: 2005. 11. 30	
* Abstract		: Simulation object attribute base class
*****************************************************************************
* Desc         : 캐릭터의 이펙트, 그림자등의 데코레이션 관련 클래스
*
*****************************************************************************/

#pragma once

class CNtlPLCharacter;
class CNtlPLPlayerName;
class CNtlPLDecal;
class CNtlPLItem;
class CNtlPLCameraRenderTexture;
class CNtlSobItem;
class CDBODirectorIndicate;
class CNtlSobCharDecorationProxy;
class CNtlInstanceEffect;
class CNtlSobCharProxyTransform;
class CNtlShareTargetMark;

struct sEmblemFactor;

/// 캐릭터의 이펙트, 그림자등의 데코레이션 관련 클래스
class CNtlSobCharDecorationProxy
{
public:

	static RwBool				m_bShadowCreate;			/** shadow을 생성 on/off flag 특정 character shadow 생성을 제어*/
    static RwBool				m_bShadowRender;			/** shadow rendering on/off flag */

private:

    CNtlSob						*m_pSobObj;
    CNtlPLCharacter				*m_pPLCharacter;			/** character resource */

    /*RwBool						m_bNameVisible;*/
	RwReal						m_fNameHeightOffset;		/** name height offset */
	RwV3d						m_vNamePos;					/** name position */
    RwReal						m_fDefShadowScale;			/** shadow scale */
    RwReal						m_fTransShadowScale;		/** shadow scale */
    RwBool						m_bResLoaded;
    RwBool						m_bResLoadComplete;
    RwInt32                     m_nPrevSectorIdx;           // 이전의 월드 섹터 인덱스 (그림자 판별에 사용)    

    CNtlPLDecal					*m_pShadowDecal;			/** shadow decal */
    CNtlPLPlayerName			*m_pPlayerName;				/** presentation player name entity */
    CNtlPLEntity				*m_pTargetMark;				/** character target mark */
    CNtlPLEntity                *m_pTargetMark02;           // 데칼 타겟 마크
    CNtlPLEntity				*m_pAttackMark;				/** character attack mark */
    CNtlPLEntity                *m_pAttackMark02;           // 데칼 어택 마크
    CNtlPLEntity				*m_pQuestMark;				/**	character quest mark */        
    CNtlPLEntity                *m_pTenkaichiMark;          ///< 천하제일 무도회 칭호 마킹 이펙트
    CNtlPLEntity                *m_pTargetMarkingMark;      /// 타겟 마킹 마크 (메가 도동파)
    CDBODirectorIndicate        *m_pDBODirectorIndicate;    /** TMQ/Quest Directore Indicate */    
	sEmblemFactor				*m_pEmblemFactor;			/** Emblem 구성 요소 */
    CNtlShareTargetMark         *m_pShareTargetMark;        ///< 공유 타겟 마크 관리 객체

    std::vector<CNtlInstanceEffect*> m_vecRPBonusEffect;    ///< 생성된 RPBonus용 이펙트를 저장하기 위한 벡터

    CNtlSobCharProxyTransform   *m_pProxyTransform;         ///< 변신 관련 프록시 객체    

	CNtlPLEntity*				m_pTitleEffect;
	CNtlPLEntity*				m_pGuardEffect;
	CNtlPLEntity*				m_pRpChargeEffect;

protected:

    /** 
    * shadow decal 생성
    */
    void CreatePLShadowDecal(void);
    void DeletePLShadowDecal(void);
    void ChangePLShadowDecal();             // 그림자 색상을 변경한다.

	/** 
	* Player Name
	*/
    void CreatePLPlayerName(void);
    void DeletePLPlayerName(void);

	/**
	* player title effect
	*/
	void CreatePLPlayerTitle(const char *pEffectKey, const char *pBoneKey);
	void DeletePLPlayerTitle();

    /** 
    * character attack mark
    */ 
    void CreatePLAttackMark(void);
    void DeletePLAttackMark(void);

    
    // character target mark
    void CreatePLTargetMark(void);
    void DeletePLTargetMark(void);
    RwBool IsNotCreateDecalMark();                                          ///< 타겟 마크중 바닥 데칼을 표시하지 않는 객첵인지 확인

    /** 
    * character quest mark
    */ 
    void CreatePLQuestMark(const RwChar *pKey);
    void DeletePLQuestMark(void);

    /// 공유 타겟 마크
    void CreateShareTargetMark(RwUInt8 bySlot, RwUInt32 type);
    void DeleteShareTargetMark();

    // 천하제일 무도회 칭호 이벤트
    void CreateTenkaichiMark(BYTE byType);
    void DeleteTenkaichiMark();

    // 타겟 마킹 이벤트 (메가 도동파)
    void CreateTargetMarkingMark();
    void DeleteTargetMarkingMark();
    
public:

    CNtlSobCharDecorationProxy();
    ~CNtlSobCharDecorationProxy();

    void	SetData(CNtlSob *pSobObj, CNtlPLCharacter *pPLCharacter);
    void	Update(RwReal fElapsed);
    void	HandleEvents(RWS::CMsg &pMsg);

    //-------------------------------------------------------------------------------
    // event handler
    //-------------------------------------------------------------------------------
public:

    void	SobFaintingEventHandler(RWS::CMsg &pMsg);
    void    SobStateTanslation(RWS::CMsg& pMsg);
    void	SobTargetSelectEventHandler(RWS::CMsg &pMsg);
    void	SobAttackSelectEventHandler(RWS::CMsg &pMsg);
    void	SobTargetSelectReleaseEventHandler(RWS::CMsg &pMsg);
    void	SobAttackMarkReleaseEventHandler(RWS::CMsg &pMsg);
    void	SobQuestMark(RWS::CMsg &pMsg);
    void    SobDirectionNfy(RWS::CMsg &pMsg);    
    void    SobShareTargetSelectHandler(RWS::CMsg& pMsg);                   ///< 공유 타겟 마크 설정 이벤트 처리
    void    SobShareTargetReleaseHandler(RWS::CMsg& pMsg);                  ///< 공유 타겟 마크 해제 이벤트 처리        
    void    SobTenkaichiMarkingHandler(RWS::CMsg& pMsg);                    ///< 천하제일 무도회 칭호 이벤트 처리    

	void    SobTitleEffectHandler(RWS::CMsg& pMsg);

    void AddWorld(void);
    void RemoveWorld(void);	

    void SetAlpha(RwUInt8 byAlpha);
    void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
    void SetVisible(RwBool bEnable);
    void SetScale(RwReal fScale);
    void SetShadowOnOff(RwBool bOnOff);
    void EnablePlayerNameVisible(RwBool bVisible);

    void CreateWeightElapsedController(RwReal fLifeTime, RwReal fWeightValue);
    void DeleteWeightElapsedController(void);

    void DetachConvertClassEquipItem(void);
    void AttachConvertClassEquipItem(CNtlPLCharacter *pPLCharacter);

	void SetNameColor(const WCHAR* pwcName, COLORREF nameColor,
					  const WCHAR* pwcGuildName, COLORREF guildColor, 
					  const WCHAR* pwcTitleName, COLORREF titlecolor);

	void SetNickNameColor(const WCHAR* pwcNickName, COLORREF nickNameColor);
    CNtlPLPlayerName* GetPLPlayerName();

	void UpdatePlayerNamePosition( RwV3d& vPos, float fNewHeight = 0.0f );

	const sEmblemFactor* GetEmblemFactor();
	void SetEmblemFactor(RwUInt8 byTypeA, RwUInt8 byTypeAColor,	
						 RwUInt8 byTypeB, RwUInt8 byTypeBColor, 
						 RwUInt8 byTypeC, RwUInt8 byTypeCColor);

    void ResourceLoadComplete(RwBool bVisible);

    // Director Indicate
    void CreateDirectorIndicate();
    void DeleteDirectorIndicate();

    // Rp Bonus Skill
    RwBool AttachRPBonusEffect();             ///< RP Bonus Effect를 생성한다.
    RwBool DetachRPBonusEffect();             ///< RP Bonus Effect를 제거한다.

	 // Guard Skill
	void CreateGuardEffect(RwChar *pKey);
	void DeleteGuardEffect();

	// rp charging Skill
	void CreateRpChargeEffect(RwChar *pKey);
	void DeleteRpChargeEffect();

    // Transform
    void						SobChangeAdult(CNtlPLCharacter* pPLCharacter);                    ///< UI Model이 체인지 되었을때
    CNtlSobCharProxyTransform*  GetProxyTransform() {return m_pProxyTransform;}    
};
