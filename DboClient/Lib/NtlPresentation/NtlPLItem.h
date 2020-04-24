/*****************************************************************************
 *
 * File			: NtlPLItem.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 2.	
 * Abstract		: NTL CNtlPLItem
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef _CNTL_PL_ITEM_
#define _CNTL_PL_ITEM_

#include "NtlDebug.h"
#include "NtlPLAttach.h"
#include "NtlPLResource.h"
#include "NtlPLItemProperty.h"
#include "NtlItem.h"
#include "NtlPLEntityRenderHelpers.h"
#include "NtlPLLinkEffect.h"
#include "NtlInstanceTraceSystem.h"
#include "NtlAnimLayer.h"
#include "NtlInstanceUpgradeEffectSystem.h"


struct SPLItemCreateParam : public SPLEntityCreateParam
{
	EItemEquipSlotType	eItemEquipSlotType;
	RwUInt8				byUpgrade;

	SPLItemCreateParam() : eItemEquipSlotType(ITEM_EQUIP_NONE), byUpgrade(0) {}
};


class CNtlPLCharacter;
class CNtlPLItemEMUVController;


class CNtlPLItem : public CNtlPLAttach
{
public:
	CNtlPLItem();
	virtual ~CNtlPLItem();

protected:
	CNtlPLItemProperty		*m_pProperty;							///< Item Property
	CNtlPLResource			*m_pResourceClump;						///< Item Resource
	
	RwMatrix				m_matWorld;								///< Item의 World Matrix

	FRAME_MAP				m_mapFrame;								///< Bone Info
	ENTITY_ATOMIC_VEC		m_vecAtomicList;						///< Atomic List
	
	CNtlPLCharacter			*m_pOwner;								///< Equip Item일 경우 소유자
	
	SItemScheduleResInfo	m_sScheduleResInfo;						///< Schedule Loading 관련 Resource 정보들	
	EItemEquipSlotType		m_eInstanceEquipSlotType;				///< Instance Equip Slot Type
	

	RwTexture				*m_pBaseTexture;						///< Base Texture
	RwTexture				*m_pMultiTexture;						///< Multi Texture

	RwRGBA					m_sColor;								///< Color
	RwRGBA					m_sAddColor;
	RwBool					m_bRunTimeAlpha;

	SToonData				m_ToonData;								///< Toon Ink, Toon Paint (Item에서는 기본으로 생성이 되지 않는다.)
																	///< Character에만 적용이 되므로 Character Equip 될 때 생성이 된다.

	CNtlPLLinkEffect        m_LinkEffectInstance;					///< LinkEffect Instance들을 관리하는 객체
	RwBool                  m_bEnableTrace;                         ///< 궤적 이펙트 표현 유무    
    RwBool                  m_bEnableUpgradeEffect;                 ///< Upgrade Effect 표현 유무 플래그

    // Item의 Animation 관련
    RpHAnimHierarchy*       m_pBaseHierarchy;                       ///< Item의 Hierarchy
    CNtlAnimLayer*          m_pAnimLayer;                           ///< 애니메이션 레이어
    CNtlInstanceAnimTable*  m_pInstanceAnimTable;                   ///< Animation Resource Instance Table
    RwUInt32                m_uiCurAnimKey;                         ///< 현재 적용되고 있는 AnimKey
    RwBool                  m_bAnimUpdate;                          ///< Animation Update 유무
	
	RwBool                  m_bVisualEffect;                        ///< VisualEffect 부착 유무

    RwInt32                 m_nGrade;                               ///< Item의 Grade    
    std::vector<CNtlInstanceEffect*> m_pUpgradeEffects;             ///< 생성된 Upgrade Effect들의 벡터

	RwTexture				*m_pUVTexture;							///< U,V Texture
	RwMatrix				*m_pUVMatrix;							///< U,V Texture Matrix

	// fake sphere enviroment uv ani effect controller for each upgrade level
	CNtlPLItemEMUVController* m_pEMUVController;

	// DOGI emblem textures
	RwTexture* m_pDogiEmblemTex;

	// DOGI emblem color
	RwRGBA m_DogiBeltClr;

	// DOGI editing color
	RwRGBA m_DogiEditedColor;

	// default render callback
	RpAtomicCallBackRender	m_fnDefRenderCallBack;

	RwBool m_SimpleMaterial;

	RwBool m_PetrifyMaterial;

protected:
	RwBool  CreateAnimData(RwUInt32 uiAnimKey);														///< Animation 관련 데이터를 설정한다.

	RwBool	SetThreadSafeProperty(const CNtlPLProperty *pData);
	RwBool	CreateScheduling(const SPLEntityCreateParam * pParam);
	RwBool	CreateThreadSafe(void);
	void	SetApplyRunTimeAlpha();

	// set enviroment map & uv ani
	VOID SetEMUV();

public:
	void* operator new(size_t size);
	void operator delete(void *pObj);
	
    // 오버라이딩 메소드
	virtual RwBool Create(const SPLEntityCreateParam *pParam = NULL);
	virtual void   Destroy(void);
	virtual RwBool Update(RwReal fElapsed);
	virtual RwBool Render(void);

#ifdef dNTL_WORLD_CULLING_NEW
	virtual RwBool	CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame);
#endif

	virtual void CallSchedulingResource(CNtlPLResource *pResource);

    virtual void   SetVisible(RwBool bVisible);
	
	SItemScheduleResInfo *GetItemScheduleResInfo() { return &m_sScheduleResInfo; }
    virtual RwBool IsSchedulingLoadingComplete() {return m_sScheduleResInfo.bLoadComplete;}

	RwBool	RenderToTexture();

	virtual RwFrame* GetFrame(void);

	virtual VOID SetEMUVController(RwBool _Activation);
	//virtual CNtlPLResource* GetResourceClump() { return m_pResourceClump; }
	//virtual CNtlPLItemEMUVController* GetEMUVController() { return m_pResourceClump; }
	const virtual CNtlPLResource* GetResource(void) const {return m_pResourceClump; }

	virtual RwBool SetProperty(const CNtlPLProperty *pProperty);
    virtual CNtlPLItemProperty* GetProperty() {return m_pProperty;}

	virtual void AddWorld(void);
	virtual void RemoveWorld(void);
	RwBool	IsExistWorld();

	void SetPosition(const RwV3d *pPos);
	void SetMatrix(RwMatrix& matWorld);
	virtual RwMatrix& GetMatrix() {return m_matWorld;}
	
	RwMatrix* GetBoneMatrix(const RwChar *pszBoneName);
	RwFrame *GetBoneByName(const RwChar *pszBoneName);
	
	EItemResType GetResType() { NTL_ASSERTE(m_pProperty); return m_pProperty->GetResType();}
	
	void SetEquipSlotType(EItemEquipSlotType eEquipSlotType) { m_eInstanceEquipSlotType = eEquipSlotType; }
	EItemEquipSlotType GetEquipSlotType() { return m_eInstanceEquipSlotType;  }
	
	void	SetBaseTexture(RwTexture *pTexture);
	void	SetMultiTexture(RwTexture *pTexture);

	RwTexture *GetBaseTexture() { return m_pBaseTexture; }
	RwTexture *GetMultiTexture() { return m_pMultiTexture; }
	
	RwTexture *GetUVTexture() { return m_pUVTexture; }
	RwMatrix *GetUVMatrix() { return m_pUVMatrix; }

	void	SetAlpha(RwUInt8 byValue);
	void	SetWeightAlpha(RwReal fWeightValue);

	void	SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	void	SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);

	RwRGBA	*GetColor() { return &m_sColor; }
	RwRGBA	*GetAddColor() { return &m_sAddColor; }

	SToonData	*GetToonData();

	virtual void	SetChangeEquip(CNtlPLCharacter *pOwner, RwBool bApplyAttach = TRUE);
	virtual void	SetRemoveEquip();

	virtual void	SetApplyEquipData();
	virtual void	SetApplyEquipDataOwner(RpAtomic *pAtomic, CNtlPLCharacter *pOwner);

	CNtlPLCharacter *GetOwner() { return m_pOwner; }

    RwBool          GetEnableVisualEffect() {return m_bVisualEffect;}                               ///< Visual Effect 부착 유무를 반환한다.
    RwBool          AttachVisualEffect( CNtlInstanceEffect* pInstanceEffect, 
		                                SEventVisualEffect* pEventVisualEffect);
    
    VecLinkEffect*      GetVecLinkEffect() { return &(m_LinkEffectInstance.m_vecLinkEffect); }	    ///< LinkEffect Vector를 반환한다.
    CNtlInstanceEffect* AttachLinkEffect( SEventLinkEffect* pEventLinkEffect );                     ///< Link Effect를 Attach 시킨다.
    RwBool              DetachLinkEffect( CNtlInstanceEffect* pLinkEffect );     

    void                SetEnableTrace(RwBool bEnable) {m_bEnableTrace = bEnable; }					///< 궤적 이펙트 표현유무를 설정한다.
    RwBool              GetEnableTrace() { return m_bEnableTrace; }									///< 궤적 이펙트 표현유무를 반환한다.
    RwBool              ActiveTraceEffect( SEventTrace* pEventTrace );								///< 궤적 이펙트를 표현한다.

    RwBool              SetUpgradeEffect( ENtlPLItemGrade eGrade );									///< Item의 Grade Effect를 설정한다.
    RwInt32             GetUpgradeEffect() {return m_nGrade;}
    void                SetEnableUpgradeEffect( RwBool bEnable );									///< Item의 Grade Effect 사용 유무를 설정한다.
	
	RwV3d	GetSubWeaponOffset() { return m_pProperty->GetAttachOffset();}

    RwBool	SetAnimation(RwUInt32 uiAnimKey, RwReal fStartTime = 0.0f, RwBool bLoop = TRUE);		///< Animation을 적용한다.
    virtual int CallBackBaseAnim(void* pEventData);												///< Animation Event의 CallBack 함수    
    
	void    SetAnimUpdate(RwBool bAnimUpdate) {m_bAnimUpdate = bAnimUpdate;}						///< Animation의 업데이트 유무를 설정한다.
    RwBool  GetAnimUpdate() {return m_bAnimUpdate;}													///< Animation의 업데이트 유무를 반환한다.
    RwReal  GetBaseDurationAnimTime();																///< Animation의 Play Time을 반환한다.
    RwBool  IsHaveAnimData();                                                                       ///< 애니메이션 데이터가 세팅되어 있는지 반환한다.

	// DOGI interfaces
	RwTexture*	GetDogiEmblem() { return m_pDogiEmblemTex; }
	RwRGBA*		GetDogiBeltColor() { return &m_DogiBeltClr; }
	RwBool		SetDogiBeltColor(RwRGBA& _BeltRGBA);
	VOID		SetDogiEditedColor(RwRGBA& _DogiEditedColor);
	RwRGBA*		GetDogiEditedColor() { return &m_DogiEditedColor; }
	RwBool		AttachDogiEmblem(RwTexture* _pTexture);
	RwBool		DetachDogiEmblem();
	
	VOID	SetToonEdgeVisibility4CurEquipItem(RwBool _Visibility);

	void	SetSimpleMaterial(RwBool _SimpleMaterial) { m_SimpleMaterial = _SimpleMaterial; }
	RwBool	GetSimpleMaterial() { return m_SimpleMaterial; }
	void	CreateSimpleMaterial();
	void	RestoreSimpleMaterial();

	VOID	SetPetrifyMaterial(RwBool _PetrifyFlag) { m_PetrifyMaterial = _PetrifyFlag; }
	RwBool	GetPetrifyMaterial() { return m_PetrifyMaterial; }
	void	CreatePetrifyMaterial();
	void	RestorePetrifyMaterial();

	RwBool	GetMeshFilePrefixName(string& _MeshFileName);
};

#endif
/*
#pragma once


using std::map;
using std::vector;
#include <map>
#include <vector>


enum eTIME_LINE_LIST_ID
{
	eTLLI_ITEM_UPGRADE_LVL_3 = 0,
	eTLLI_ITEM_UPGRADE_LVL_4,
};


struct sTIME_LINE_BLOCK
{
	// allocated time for this time block
	RwReal _Time;

	// activation switch flag
	RwBool _Switch;

	sTIME_LINE_BLOCK::sTIME_LINE_BLOCK()
	{
		_Time	= 0.0f;
		_Switch	= FALSE;
	}
};

class CTimeLineActor
{
public:
	CTimeLineActor();
	virtual ~CTimeLineActor();

private:
	RwInt32 m_CurTLBIdx;
	RwReal	m_CurTLBAccumulatedTime;
};

class CNtlTimeLine
{
public:
	CNtlTimeLineContainer();
	virtual ~CNtlTimeLineContainer();

private:
	// time line block vector
	typedef std::vector<sTIME_LINE_BLOCK>	tTLB_VEC;
	typedef tTLB_VEC::iterator				tTLB_VEC_ITER;

	// time line map
	typedef std::map<eTIME_LINE_LIST_ID, tTLB_VEC>	tTLL_MAP;
	typedef tTLL_MAP::iterator						tTLL_MAP_ITER;

private:
	VOID RegisterTLL();

public:
	VOID Create();
	VOID Destroy();
};*/