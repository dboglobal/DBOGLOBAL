/*****************************************************************************
 *
 * File			: NtlSobProxy.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 4. 26	
 * Abstract		: Simulation object proxy base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOB_PROXY_H__
#define __NTL_SOB_PROXY_H__

#include "ceventhandler.h"
#include "NtlMath.h"
#include "NtlSobProxyDef.h" 


class CNtlSobProxy : public RWS::CEventHandler
{
public:

	typedef std::list<CNtlPLEntity*>						ListPLEntity;
	typedef std::list<CNtlSobProxySystemEffect*>		    ListProxySystemEffect;	

protected:

	RwUInt32						m_uiSystemFlags;
	RwUInt32						m_uiCompType;
	CNtlSob							*m_pSobObj;
	ListPLEntity					m_listPLChild;
	
	RwUInt8							m_byAlpha;
	CNtlSobProxySystemEffectColor	*m_pWorldLight;
	ListProxySystemEffect			m_listProxySystemEffect;	
    
    RwBool                          m_bEnableCameraAlpha;
    CNtlSobProxySystemEffect*       m_pCameraAlphaEffect;

	bool							m_bDisableAniChange;

protected:

	void							ResetRemoveSystemEffectAlpha(void);

	void							UpdateSystemEffectColor(RwReal fElapsed);
	void							UpdateSystemEffectAlpha(RwReal fElapsed);	
    void							UpdateSystemEffectScale(RwReal fElapsed);

public:

	CNtlSobProxy();
	virtual ~CNtlSobProxy();

	virtual RwBool	Create(RwUInt32 uiCompType);
	virtual void	Destroy(void);
	virtual void	Update(RwReal fElapsed);
	virtual void	HandleEvents(RWS::CMsg &pMsg) {}

	
	
public:

	void			SetDisableAniChange(bool bFlag) { m_bDisableAniChange = bFlag; }
	bool			GetDisableAniChange() { return m_bDisableAniChange; }

	//---------------------------------------------------
	// proxy system flag 설정에 관한 함수.
	//---------------------------------------------------

	virtual	void	EnableProxySystemVisible(RwBool bVisible);
	RwBool			IsProxySystemVisible(void);

	virtual void	EnableProxySystemGUILPVisible(RwBool bVisible);
	RwBool			IsProxySystemGUILPVisible(void);

	//---------------------------------------------------
	// owner simulation object  설정에 관한 함수.
	//---------------------------------------------------

	void			SetSobObj(CNtlSob *pSobObj);
	void			SetCompType(RwUInt32 uiCompType);
	RwUInt32		GetCompType(void) const;

	virtual void	NotifyStateEnter(RwUInt32 uiStateId)	{}
	virtual void	NotifyStateExit(RwUInt32 uiOldState, RwUInt32 uiNewState)		{}

	//---------------------------------------------------
	// presentation entity 생성 함수.                                                                      
	//---------------------------------------------------

	// attach 시키지 않고 effect 생성.
	CNtlPLEntity*			CreatePLEffect(const RwChar *pKey, const RwV3d *pPos = NULL, RwBool bIgnoreVisible = FALSE );

	// attach 시키지 않고 effect 생성
	// child list에 추가한다.
	CNtlPLEntity*			CreatePLChildEffect(const RwChar *pKey, RwBool bIgnoreVisible = FALSE);
	
	// bone attach시키면서 effect 생성.
    virtual CNtlPLEntity*	CreatePLChildEffect(const RwChar *pKey, const RwChar *pAttachBoneName, RwV3d vOffset = ZeroAxis, RwBool bIgnoreVisible = FALSE) {return NULL;}

	// offset position에 attach시키면서 effect 생성.
	virtual CNtlPLEntity*	CreatePLChildEffect(const RwChar *pKey, RwV3d vOffset, RwBool bIgnoreVisible = FALSE, RwBool bApplyRotate = FALSE)  { return NULL; }

	// offset position에 attach시키면서 effect 생성.
	CNtlPLEntity*			CreatePLDamageBox(RwV3d vPos, RwUInt32 uiDamage, RwBool bHealing = FALSE);

	// child로 생성된 effect를 삭제한다.
	virtual void			DeletePLChildEffect(CNtlPLEntity *pPLEntity);

	//---------------------------------------------------
	// main presentation entity에 관련된 함수 
	//---------------------------------------------------

	/**
	* Functions related to add and remove in world.
	*/

	virtual void			AddWorld(void)		{}
	virtual void			RemoveWorld(void)	{}

	/**
	* transform에 관현된 함수.
	*/

	virtual void			SetPosition(const RwV3d *pPos)	{}
	virtual RwV3d			GetPosition(void);

	virtual void			SetDirection(const RwV3d *pDir) {}
	virtual RwV3d			GetDirection(void);

	virtual void			SetAngleY(RwReal fAngle)		{}
	virtual void			SetDeltaAngleY(RwReal fAngle)	{}
	virtual void			SetAngleX(RwReal fAngle) {}
	virtual void			SetDeltaAngleX(RwReal fAngle) {}

	virtual void			SetScale(RwReal fScale)			{}

	/**
	* material에 관현된 함수.
	*/

	virtual void			SetAlpha(RwUInt8 byAlpha);
	
	RwUInt8					GetAlpha(void);

	void					AddAlpha(RwUInt8 byAddValue);

	virtual void			SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue) {} 

	void					EnableWorldLight(RwBool bEnable, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);

	virtual	void			SetInkThicknessWeight(RwReal fWeightValue = 1.0f) {}

	virtual void			SetInkThickness(RwReal fValue = NTL_INK_THICKNESS) {}

	virtual void			SetInkColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue) {}
	
	/**
	* animation에 관련된 함수.
	*/
	
	// animation setting
	virtual void			SetBaseAnimation(RwUInt32 uiAnimKey, RwBool bLoop = TRUE, RwReal fStartTime = 0.0f) {}

	virtual RwBool			IsExistBaseAnimation(RwUInt32 uiAnimKey) { return FALSE; }

	// 현재 play 하는 animation key를 얻어온다.
	virtual RwUInt32		GetBaseAnimationKey(void)		{ return 0; }

	virtual RwBool			IsBaseAnimationEnd(void)		{ return TRUE; }

	virtual void			SetAnimSpeed(RwReal fAniSpeed)	{}
    virtual RwReal          GetAnimSpeed()					{return 1.0f;}

	virtual void			SetAnimBlendNoneOneTime(void)	{}

	/**
	* Decoration에 관련된 함수
	*/

	// Entity 상단의 이름 색상을 변경한다
	virtual void			SetNameColor(const WCHAR* pwcName, COLORREF color) {}

	virtual void			SetNickNameColor(const WCHAR* pwcNickName, COLORREF nickNameColor) {}

	virtual void			SetEmblemFactor(RwUInt8 byTypeA, RwUInt8 byTypeAColor,	
											RwUInt8 byTypeB, RwUInt8 byTypeBColor, 
											RwUInt8 byTypeC, RwUInt8 byTypeCColor) {}

	virtual void			SetDecorationProxyAlpha(RwUInt8 byAlpha) {}
	virtual void			EnableDecorationProxyVisible(RwBool bShow) {}

	/**
	* entity 정보에 관한 함수.
	*/

	// main entity를 얻어오는 함수.
	virtual CNtlPLEntity*	GetPLMainEntity(void) { return NULL; }

	// bone position을 얻어온다.
	virtual RwV3d			GetBonePosition(const RwChar *pBoneName);

	// 장착한 무기의 bone position
	virtual RwV3d			GetWeaponBonePosition(const RwChar *pBoneName);

	// 장착한 sub weapon의 bone position
	virtual RwV3d			GetSubWeaponBonePosition(const RwChar *pBoneName);

	// pl entity의 폭을 얻어온다.
	virtual RwReal			GetPLEntityWidth(void); 

	// pl entity의 높이를 얻어온다.
	virtual RwReal			GetPLEntityHeight(void);

	// pl entity의 depth을 얻어온다.
	virtual RwReal			GetPLEntityDepth(void); 

	// pl entity의 base scale을 얻어온다.
	virtual RwReal			GetPLEntityBaseScale(void);

    // 임의로 조정된 Scale을 얻어온다.
    virtual RwReal          GetScale();

	/**
	* child presentation entity에 관련된 함수  
	*/

	// child entity 추가.
	void					AddPLChildEntity(CNtlPLEntity *pPLEntity);

	// child entity 갯수 얻어오기.
	RwInt32					GetPLChildEntityCount(void) const;

	// child entity transform.
	void					TransformPLChildEntity(RwMatrix& mat);

	/** 
	* dynamic effect control.
	*/
	CNtlSobProxySystemEffect*	AddVisualSystemEffectAlphaBlend(RwReal fWeightAlpha, RwReal fLifeTime, RwBool bLoop, RwBool bExceptDecorationProxy = FALSE);	
	CNtlSobProxySystemEffect*	AddVisualSystemEffectAlphaBlend(RwReal fStartWeightAlpha, RwReal fEndWeightAlpha, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop, RwBool bExceptDecorationProxy = FALSE);	
	void						RemoveVisualSystemEffectAlpha(CNtlSobProxySystemEffect *pSystemEffect);	

    CNtlSobProxySystemEffect*   AddVisualSystemEffectColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwBool bAddColor = FALSE);
    void                        RemoveVisualSystemEffectColor(CNtlSobProxySystemEffect* pSystemEffect);

	CNtlSobProxySystemEffect*	AddVisualSystemEffectColorFlickering(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fFlickerTime);
	void						RemoveVisualSystemEffectColorFlickering(CNtlSobProxySystemEffect *pSystemEffect);	

    CNtlSobProxySystemEffect*   AddVisualSystemEffectScale(RwReal fEndScale, RwReal fTime);    

    RwBool                      IsExitSystemEffect(CNtlSobProxySystemEffect* pSystemEffect);
	
    //////////////////////////////////////////////////////////////////////////
	virtual void EnableVisible(RwBool bEnable);
	virtual void EnablePicking(RwBool bEnable) {}
	virtual void EnableShadowRendering(RwBool bEnable) {}
	virtual void EnableHighlight(RwBool bEnable) {}	
	virtual void CreateElapsedController(RwReal fCtrlTime, RwReal fWeightValue);
	virtual void DeleteElapsedController(void);
    virtual void EnableCameraAlpha(RwBool bEnable);             ///< 카메라 거리에 따른 알파를 적용한다.

//------------------------------------------------------------------------------------------------------------
// event에 관련된 함수.  
//------------------------------------------------------------------------------------------------------------

public:

	
	// mouse focus를 받았을 경우.
	virtual void SetFocus(void) {}

	// mouse focus를 읽어 버렸을 경우.
	virtual void ReleaseFocus(void) {}

};

inline RwBool CNtlSobProxy::IsProxySystemVisible(void)
{
	if(m_uiSystemFlags & NTL_PROXYSYSTEM_NOT_VISIBLE)
		return FALSE;

	return TRUE;
}

inline RwBool CNtlSobProxy::IsProxySystemGUILPVisible(void)
{
	if(m_uiSystemFlags & NTL_PROXYSYSTEM_LP_VISIBLE)
		return FALSE;

	return TRUE;
}

inline void CNtlSobProxy::SetSobObj(CNtlSob *pSobObj)
{
	m_pSobObj = pSobObj;
}

inline void CNtlSobProxy::SetCompType(RwUInt32 uiCompType)
{
	m_uiCompType = uiCompType;
}

inline RwUInt32 CNtlSobProxy::GetCompType(void) const
{
	return m_uiCompType;
}

inline RwInt32 CNtlSobProxy::GetPLChildEntityCount(void) const
{
	return m_listPLChild.size();
}

// pl entity의 폭을 얻어온다.
inline RwReal CNtlSobProxy::GetPLEntityWidth(void)
{
	return 1.0f;
}

inline RwReal CNtlSobProxy::GetPLEntityHeight(void)
{
	return 2.0f;
}

// Retrieves the depth of pl entity.
inline RwReal CNtlSobProxy::GetPLEntityDepth(void)
{
	return 1.0f;
}

inline RwReal CNtlSobProxy::GetPLEntityBaseScale(void)
{
	return 1.0f;
}
inline RwReal CNtlSobProxy::GetScale()
{
    return 1.0f;
}


#endif