/*****************************************************************************
*
* File			: NtlSobCharDecorationProxy.h
* Author		: agebreak
* Copyright	: (��)NTL
* Date			: 2005. 11. 30	
* Abstract		: Simulation object attribute base class
*****************************************************************************
* Desc         : ĳ������ ����Ʈ, �׸��ڵ��� ���ڷ��̼� ���� Ŭ����
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

/// ĳ������ ����Ʈ, �׸��ڵ��� ���ڷ��̼� ���� Ŭ����
class CNtlSobCharDecorationProxy
{
public:

	static RwBool				m_bShadowCreate;			/** shadow�� ���� on/off flag Ư�� character shadow ������ ����*/
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
    RwInt32                     m_nPrevSectorIdx;           // ������ ���� ���� �ε��� (�׸��� �Ǻ��� ���)    

    CNtlPLDecal					*m_pShadowDecal;			/** shadow decal */
    CNtlPLPlayerName			*m_pPlayerName;				/** presentation player name entity */
    CNtlPLEntity				*m_pTargetMark;				/** character target mark */
    CNtlPLEntity                *m_pTargetMark02;           // ��Į Ÿ�� ��ũ
    CNtlPLEntity				*m_pAttackMark;				/** character attack mark */
    CNtlPLEntity                *m_pAttackMark02;           // ��Į ���� ��ũ
    CNtlPLEntity				*m_pQuestMark;				/**	character quest mark */        
    CNtlPLEntity                *m_pTenkaichiMark;          ///< õ������ ����ȸ Īȣ ��ŷ ����Ʈ
    CNtlPLEntity                *m_pTargetMarkingMark;      /// Ÿ�� ��ŷ ��ũ (�ް� ������)
    CDBODirectorIndicate        *m_pDBODirectorIndicate;    /** TMQ/Quest Directore Indicate */    
	sEmblemFactor				*m_pEmblemFactor;			/** Emblem ���� ��� */
    CNtlShareTargetMark         *m_pShareTargetMark;        ///< ���� Ÿ�� ��ũ ���� ��ü

    std::vector<CNtlInstanceEffect*> m_vecRPBonusEffect;    ///< ������ RPBonus�� ����Ʈ�� �����ϱ� ���� ����

    CNtlSobCharProxyTransform   *m_pProxyTransform;         ///< ���� ���� ���Ͻ� ��ü    

	CNtlPLEntity*				m_pTitleEffect;
	CNtlPLEntity*				m_pGuardEffect;
	CNtlPLEntity*				m_pRpChargeEffect;

protected:

    /** 
    * shadow decal ����
    */
    void CreatePLShadowDecal(void);
    void DeletePLShadowDecal(void);
    void ChangePLShadowDecal();             // �׸��� ������ �����Ѵ�.

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
    RwBool IsNotCreateDecalMark();                                          ///< Ÿ�� ��ũ�� �ٴ� ��Į�� ǥ������ �ʴ� ��ý���� Ȯ��

    /** 
    * character quest mark
    */ 
    void CreatePLQuestMark(const RwChar *pKey);
    void DeletePLQuestMark(void);

    /// ���� Ÿ�� ��ũ
    void CreateShareTargetMark(RwUInt8 bySlot, RwUInt32 type);
    void DeleteShareTargetMark();

    // õ������ ����ȸ Īȣ �̺�Ʈ
    void CreateTenkaichiMark(BYTE byType);
    void DeleteTenkaichiMark();

    // Ÿ�� ��ŷ �̺�Ʈ (�ް� ������)
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
    void    SobShareTargetSelectHandler(RWS::CMsg& pMsg);                   ///< ���� Ÿ�� ��ũ ���� �̺�Ʈ ó��
    void    SobShareTargetReleaseHandler(RWS::CMsg& pMsg);                  ///< ���� Ÿ�� ��ũ ���� �̺�Ʈ ó��        
    void    SobTenkaichiMarkingHandler(RWS::CMsg& pMsg);                    ///< õ������ ����ȸ Īȣ �̺�Ʈ ó��    

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
    RwBool AttachRPBonusEffect();             ///< RP Bonus Effect�� �����Ѵ�.
    RwBool DetachRPBonusEffect();             ///< RP Bonus Effect�� �����Ѵ�.

	 // Guard Skill
	void CreateGuardEffect(RwChar *pKey);
	void DeleteGuardEffect();

	// rp charging Skill
	void CreateRpChargeEffect(RwChar *pKey);
	void DeleteRpChargeEffect();

    // Transform
    void						SobChangeAdult(CNtlPLCharacter* pPLCharacter);                    ///< UI Model�� ü���� �Ǿ�����
    CNtlSobCharProxyTransform*  GetProxyTransform() {return m_pProxyTransform;}    
};
