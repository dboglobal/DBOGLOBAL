/*****************************************************************************
*
* File			: NtlSobAtt.h
* Author		: HyungSuk, Jang
* Copyright	: (주)NTL
* Date			: 2005. 11. 30	
* Abstract		: Simulation object attribute base class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_SOBCHAR_PROXY_H__
#define __NTL_SOBCHAR_PROXY_H__

// Core
#include "NtlMath.h"

// shared
#include "NtlItem.h"

// SL
#include "NtlSobProxy.h"

class CNtlPLCharacter;
class CNtlPLPlayerName;
class CNtlPLDecal;
class CNtlPLItem;
class CNtlPLObject;
class CNtlPLCameraRenderTexture;
class CNtlSobItem;
class CDBODirectorIndicate;
class CNtlSobCharDecorationProxy;
class CNtlSobCharEquipProxy;
class CNtlSobActor;
struct SEquipItem;

class CNtlSubWeaponController
{
public:

	enum SLOT_SUBWEAPON_STATE
	{
		SSWS_NONE,
		SSWS_IDLE,
		SSWS_MOVE,
		SSWS_ATTACK,
	};

    CNtlSobActor*       m_pActor;
	CNtlPLCharacter		*m_pPLCharacter;
	CNtlPLItem			*m_pPLItem;
	RwMatrix			*m_pMatSubWeaponBone;
	RwMatrix			m_matWorld;

	RwBool				m_bResLoaded;
	RwBool				m_bResLoadComplete;
	RwUInt8				m_bySWState;			/** sub weapon state */

private:

	void	UpdateCreate(RwReal fElapsed);
	void	UpdateIdle(RwReal fElapsed);
	void	UpdateMove(RwReal fElapsed);
	void	UpdateAttack(RwReal fElapsed);

public:

	CNtlSubWeaponController();
	~CNtlSubWeaponController();

	void	SetData(CNtlSobActor* pActor, CNtlPLCharacter *pPLCharacter, CNtlPLItem *pPLItem, RwBool bResLoadComplete);
	void	SetChangeItem(void);
	void	RemoveChangeItem(void);

	void	SetState(RwUInt8 byState);
	void	Update(RwReal fElapsed);

	void	NotifyOwnerStateEnter(RwUInt32 uiStateId);
	void	NotifyOwnerStateExit(RwUInt32 uiStateId);

	void	ResourceLoadComplete(RwBool bVisible);
	RwBool	IsResourceLoaded(void);
};
//////////////////////////////////////////////////////////////////////////

class CNtlSobCharPrivateShopProxy
{
private:

	CNtlPLEntity	*m_pPLShop;
	RwReal			m_fWidth;
	RwReal			m_fHeight;
	RwReal			m_fDepth;

public:

	CNtlSobCharPrivateShopProxy();
	~CNtlSobCharPrivateShopProxy();

	void	SetData(CNtlSob *pSobObj, CNtlPLCharacter *pPLCharacter);
	void	Update(RwReal fElapsed);

	RwReal	GetHeight(void);
};

//////////////////////////////////////////////////////////////////////////

class CNtlSobCharPitchAngleProxy
{
private:
	
	RwReal			m_fAngleX;
	RwReal			m_fUpdateAngleTime;
	RwBool			m_bCalcAngle;
	CNtlSob			*m_pSobObj;
	CNtlPLCharacter *m_pPLCharacter;

private:

	void	CalcAngle(void);

public:

	CNtlSobCharPitchAngleProxy();

	void	ReCalcAngle(CNtlSob *pSobObj, CNtlPLCharacter *pPLCharacter);
	void	Update(RwReal fElapsed);

	RwReal	GetAngleX(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
class CNtlSobCharProxy : public CNtlSobProxy
{
public:
	typedef struct _SUICharacter
	{
		CNtlPLCharacter				*pPLCharacter;
		CNtlSobCharEquipProxy		*pUIEquipProxy;
		CNtlPLCameraRenderTexture	*pUICamera;				/** UI render camera */
	}SUICharacter;

private:
	struct SUIPcStatusRotate
	{
		RwBool	bLeftRot;
		RwBool	bRightRot;
        RwReal  fAngle;        
	};

	CNtlPLCharacter				*m_pCharacter;				/** character resource */
	CNtlSobCharDecorationProxy	*m_pDecorationProxy;		/** character decoration proxy */
	CNtlSobCharEquipProxy		*m_pEquipProxy;				/** character equip proxy */
	CNtlSobCharPrivateShopProxy *m_pPrivateShopProxy;		/** private shop */
	CNtlSobCharPitchAngleProxy	*m_pPitchAngleProxy;		/** pitch angle proxy */
	
	RwReal						m_fInkThickness;
	RwReal						m_fWeightInk;
	RwReal						m_fCurrHalfOneSecTime;
	RwBool						m_bEnableHighlight;
    CNtlSobProxySystemEffect*   m_pFocusColorEffect;       ///< 픽킹시 하이라이트 이펙트
	RwBool						m_bResLoaded;
	RwUInt8						m_byAnimBlendNoneOneTime;	/** animation blending을 한번만 하지 않는다 */
    

	SUICharacter				*m_pUIPcStatusWnd;
	SUIPcStatusRotate			*m_pUIPcStatusRotate;
	CNtlPLCameraRenderTexture	*m_pUIPcStatusBarCamera;

	SUICharacter				*m_pUIPcDogiWnd;
	SUIPcStatusRotate			*m_pUIPcDogiRotate;


    RwBool                      m_bRemoveColorChangeEffect;  ///< 색상 변경 이펙트 삭제 플래그
    CNtlSobProxySystemEffect*   m_ppColorChangeEffect[2];    ///< 색상 변경 이벤트에 사용되는 시스템 이펙트

//-------------------------------------------------------------------------------
// event handler
//-------------------------------------------------------------------------------

public:

	typedef void (CNtlSobCharProxy::*FuncEventHander)(RWS::CMsg &pMsg);
	typedef std::map<const RWS::CRegisteredMsgs*, FuncEventHander> MapFuncEventHandler;
	static MapFuncEventHandler m_mapEventFunc;

	static void RegisterEventHandler(void);

	// event handler function
	void SobCreateEventHandler(RWS::CMsg &pMsg);
	void SobEquipChangeEventHandler(RWS::CMsg &pMsg);
	void SobGotFocusEventHandler(RWS::CMsg &pMsg);
	void SobLostFocusEventHandler(RWS::CMsg &pMsg);
	void SobQuestAcceptProposalNfy(RWS::CMsg &pMsg);
	void SobQuestAcceptRewardNfy(RWS::CMsg &pMsg);
	void SobSubWeaponActiveEventHandler(RWS::CMsg &pMsg);
	void SobSubWeaponDeActiveEventHandler(RWS::CMsg &pMsg);
	void SobConvertClass(RWS::CMsg &pMsg);
    void SobChangeAdult(RWS::CMsg &pMsg);
    void SobDecorationNotify(RWS::CMsg &pMsg);
    void SobPostEffectEventHandler(RWS::CMsg& pMsg);
    void SobColorChangeEventHandler(RWS::CMsg& pMsg);
    
	
protected:

	/** 
	* character 생성
	*/
	CNtlPLCharacter* CreatePLCharacter(RwBool bNotShading);
	void DeletePLCharacter(void);

	/**
	* equipment item
	*/

	void CreateEquipProxy(RWS::CMsg &pMsg);
	void DeleteEquipProxy(void);

	void CreateDecorationProxy(RWS::CMsg &pMsg);
	void DeleteDecorationProxy(void);
	
	/**
	* UI pc status window rendering equipment item
	*/

	void CreateUIPcStatusWndCharacter(void);
	void DeleteUIPcStatusWndCharacter(void);
	
	void CreateUIPcStatusWndEquipPLItem(void);
	void DeleteUIPcStatusWndEquipPLItem(void);

	void CreateUIPcStatusWndCamera(void);
	void DeleteUIPcStatusWndCamera(void);

	void CreateUIPcDogiCharacter(void);
	void DeleteUIPcDogiCharacter(void);

	void CreateUIPcDogiEquipPLItem(void);
	void DeleteUIPcDogiEquipPLItem(void);

	void CreateUIPcDogiCamera(void);
	void DeleteUIPcDogiCamera(void);

	/**
	* Update
	*/

	void UpdateHalfOneSecTick(RwReal fElapsed);
	void UpdateInkThicknessCheck(RwReal fElapsed);
	void UpdateDecoration(RwReal fElapsed);
	void UpdateResourceLoadCheck(RwReal fElapsed);
	void UpdateGUIUpdateCheck(RwReal fElapsed);
	
	/**
	* resource
	*/

	void ResourceLoadComplete(RwBool bVisible);

public:

	CNtlSobCharProxy();
	virtual ~CNtlSobCharProxy();

	virtual RwBool Create(RwUInt32 uiCompType);
	virtual void Destroy(void);
	virtual void Update(RwReal fElapsed);
	virtual void HandleEvents(RWS::CMsg &pMsg);

	//---------------------------------------------------
	// proxy system flag 설정에 관한 함수.
	//---------------------------------------------------

	void EnableProxySystemVisible(RwBool bVisible);

	//---------------------------------------------------
	// character 속성에 관련된 함수.
	//---------------------------------------------------
	void EnablePlayerNameVisible(RwBool bVisible);
	RwBool GetActiveSubWeapon(void);

	//---------------------------------------------------
	// Inventory UI 에 관련된 함수.
	//---------------------------------------------------
	RwTexture* UIPcStatusWndRender(void);
	RwTexture* UIPcStatusBarRender(void);
	RwTexture* UIPcDogiWndRender(void);
	SUICharacter* GetUICharacter() {return m_pUIPcStatusWnd;}		///< UI화면상의 캐릭터 객체를 반환한다.
	SUICharacter* GetUIDogiCharacter() {return m_pUIPcDogiWnd;}		///< UI화면상의 캐릭터 객체를 반환한다.

	//---------------------------------------------------
	// owner simulation object  설정에 관한 함수.
	//---------------------------------------------------
	void	NotifyStateEnter(RwUInt32 uiStateId);	
	void	NotifyStateExit(RwUInt32 uiOldState, RwUInt32 uiNewState);

	//---------------------------------------------------
	// presentation entity 생성 함수.                                                                      
	//---------------------------------------------------

	// bone attach시키면서 effect 생성.
	virtual CNtlPLEntity* CreatePLChildEffect(const RwChar *pKey, const RwChar *pAttachBoneName, RwV3d vOffset = ZeroAxis, RwBool bIgnoreVisible = FALSE);

	// offset position에 attach시키면서 effect 생성.
	virtual CNtlPLEntity*	CreatePLChildEffect(const RwChar *pKey, RwV3d vOffset, RwBool bIgnoreVisible = FALSE, RwBool bApplyRotate = FALSE);

	//---------------------------------------------------
	// main presentation entity에 관련된 함수 
	//---------------------------------------------------

	/**
	* world에 add 및 remove 관련된 함수.
	*/

	virtual void			AddWorld(void);		
	virtual void			RemoveWorld(void);

	/**
	* transform에 관현된 함수.
	*/

	virtual void SetPosition(const RwV3d *pPos);
	virtual void SetDirection(const RwV3d *pDir);
	virtual void SetAngleY(RwReal fAngle);
	virtual void SetDeltaAngleY(RwReal fAngle);
	virtual void SetAngleX(RwReal fAngle);
	virtual void SetDeltaAngleX(RwReal fAngle);
	virtual void SetScale(RwReal fScale);

	/**
	* material에 관현된 함수.
	*/

	void SetAlpha(RwUInt8 byAlpha);
	void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	void SetInkThicknessWeight(RwReal fWeightValue = 1.0f); 
	void SetInkThickness(RwReal fValue = NTL_INK_THICKNESS);
	void SetInkColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
    void DetachColorChangeEffect();
    void SetStoneMaterial(RwBool bApply);

	/**
	* animation에 관련된 함수.
	*/

	// animation
	void		SetBaseAnimation(RwUInt32 uiAnimKey, RwBool bLoop = TRUE, RwReal fStartTime = 0.0f);
	RwBool		IsExistBaseAnimation(RwUInt32 uiAnimKey);
	RwUInt32	GetBaseAnimationKey(void);
	RwBool		IsBaseAnimationEnd(void);
	void		SetAnimSpeed(RwReal fAniSpeed);
    RwReal      GetAnimSpeed();
	void		SetAnimBlendNoneOneTime(void);
    void        SetSkillCancel();

	/**
	* entity 정보에 관한 함수.
	*/
	CNtlPLEntity* GetPLMainEntity(void);    // presentation main entity
	virtual RwV3d GetBonePosition(const RwChar *pBoneName); 	// bone position을 구한다.
	virtual RwV3d GetWeaponBonePosition(const RwChar *pBoneName);   // 장착한 무기의 bone position
	virtual RwV3d GetSubWeaponBonePosition(const RwChar *pBoneName);    // 장착한 sub weapon의 bone position

	virtual RwReal GetPLEntityHeight(void); // pl entity의 높이를 얻어온다.
	virtual RwReal GetPLEntityWidth(void);  // pl entity의 폭을 얻어온다.
	virtual RwReal GetPLEntityDepth(void);  // pl entity의 깊이을 얻어온다.
	virtual RwReal GetPLEntityBaseScale(void);  // pl entity의 base scale을 얻어온다.
    virtual RwReal GetScale();
    void           ChangeModel(CNtlPLCharacter* pCharacter, RwBool bEquipItem, RwBool bDeleteOrgModel = FALSE);        ///< 메인 Entity 모델을 변경한다.
	void           ChangeUIModel(CNtlPLCharacter* pCharacter);                         ///< UI창에 렌더링될 모델을 변경한다.
	void           ChangeUIModel_Dogi(CNtlPLCharacter* pCharacter);                         ///< UI창에 렌더링될 모델을 변경한다.

    // Item 관련 함수
    SEquipItem*    GetEquipItem(RwUInt8 bySlotIdx);                         ///< 장착하고 있는 Item을 반환한다.
    void           CreateEquipItem(RwUInt32 uiTblId, RwUInt8 bySlotIdx, RwUInt8 byGrade);           ///< Item을 장착한다.
    void           DeleteEquipItem(RwUInt8 bySlotIdx);    
    void           SetVisibleEquipItem(RwBool bVisible, RwUInt8 bySlotIdx);                  ///< 장착하고 있는 Item의 Visible을 설정한다.   

	/**
	* Decoration에 관련된 함수
	*/

    // Rp Bonus Skill
    void AttachRPBonusEffect();             ///< RP Bonus Effect를 생성한다.
    void DetachRPBonusEffect();             ///< RP Bonus Effect를 제거한다.

	// Guard Skill
	void				CreateGuardEffect();
	void				DeleteGuardEffect();

	// rp charging Skill
	void				CreateRpChargeEffect();
	void				DeleteRpChargeEffect();

	// Entity 상단의 이름 색상을 변경한다
	virtual void SetNameColor(const WCHAR* pwcName, COLORREF nameColor,
							  const WCHAR* pwcGuildName, COLORREF guildColor,
							  const WCHAR* pwcTitleName, COLORREF titlecolor);

	virtual void SetNickNameColor(const WCHAR* pwcNickName, COLORREF nickNameColor);

	virtual void SetEmblemFactor(RwUInt8 byTypeA, RwUInt8 byTypeAColor,	
								RwUInt8 byTypeB, RwUInt8 byTypeBColor, 
								RwUInt8 byTypeC, RwUInt8 byTypeCColor);


	virtual void SetDecorationProxyAlpha(RwUInt8 byAlpha);
	virtual void EnableDecorationProxyVisible(RwBool bShow);

	/**
	* pc status window rotate
	*/
	void	PcStatusRotateLeft(void);
	void	PcStatusRotateRight(void);
	void	PcStatusRotateStop(void);
	void	PcStatusRotateReset(void);
	void	PcStatusRotate(RwReal fDelta);

	void	PcDogiRotateLeft(void);
	void	PcDogiRotateRight(void);
	void	PcDogiRotateStop(void);
	void	PcDogiRotateReset(void);
	void	PcDogiRotate(RwReal fDelta);

	virtual void EnableVisible(RwBool bEnable);    
	virtual void EnablePicking(RwBool bEnable);
	virtual void EnableShadowRendering(RwBool bEnable);
	virtual void EnableHighlight(RwBool bEnable);
	virtual void CreateElapsedController(RwReal fCtrlTime, RwReal fWeightValue);
	virtual void DeleteElapsedController(void);

	//---------------------------------------------------
	// event에 관련된 함수.  
	//---------------------------------------------------

	// mouse focus를 받았을 경우.
	virtual void SetFocus(void);

    // mouse focus를 읽어 버렸을 경우.
	virtual void ReleaseFocus(void);

    CNtlSobCharDecorationProxy* GetDecorationProxy() {return m_pDecorationProxy;}
	CNtlSobCharEquipProxy* GetEuqipProxy() {return m_pEquipProxy;}
};

#endif