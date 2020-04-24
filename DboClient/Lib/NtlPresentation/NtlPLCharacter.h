/*****************************************************************************
*
* File			: NTLPLCharacter.h
* Author		: HongHoDong
* Copyright	: (주)NTL
* Date			: 2005. 8. 20.
* Abstract		: NTL PLCharacter
*****************************************************************************
* Desc         : 
*
*****************************************************************************/
#ifndef __NTL_PL_CHARACTER_H__
#define __NTL_PL_CHARACTER_H__

#include "NtlDebug.h"
#include "NtlPLAttach.h"
#include "NtlPLResource.h"
#include "NtlPLCharacterProperty.h"
#include "NtlPLItem.h"
#include "ceventhandler.h"
#include "NtlPLEntityRenderHelpers.h"
#include "NtlDefaultItemData.h"
#include "NtlPLLinkEffect.h"
#include "NtlAnimLayer.h"
#include "NtlInstanceAnimData.h"
#include "NtlPLEquipItem.h"
#include "OccluderProxy.h"


#define dCOSTUME_BITSET_INVALID		(0xffffffff)


struct SPLCharacterCreateParam : public SPLEntityCreateParam
{
	RwUInt32 uiSerialId;

	RwBool   bPcFlag;
	RwUInt32 uiClass;
	RwUInt32 uiRace;
	RwUInt32 uiGender;
	RwUInt32 uiHeadType;
	RwUInt32 uiFaceType;
	RwUInt32 uiHeadColorType;
	RwUInt32 uiSkinColorType;
	RwBool	bNotShadingFlag;
    RwBool  bIsAdult;
	SPLCharacterCreateParam() : bNotShadingFlag(FALSE), bIsAdult(FALSE) {}
};

enum ECharacterAnimLayer
{
	CHARACTER_ANIM_LAYER_BASE = 0,
	CHARACTER_ANIM_LAYER_UPPER,
	CHARACTER_ANIM_LAYER_END,
};

/*
* Play이 되고 있는 Animation의 정보
*/
struct SAnimPlayInfo
{
	RwUInt32	m_uiAnimKey;
	std::vector<SEventAnimHit *> vecAnimHit;

	SAnimPlayInfo() : m_uiAnimKey(0) {}
};


/**
* \ingroup NtlPresentation
* \brief Character 관련 클래스
* \date 2006-08-21
* \author Hodong
*/

class CNtlPLCharacter : public CNtlPLAttach, public COccluderProxy
{
public:
	CNtlPLCharacter();
	virtual ~CNtlPLCharacter();

protected:
//	RwSphere				m_BSphere;								///< Position이 연산되지 않은 BoundSphere
	RwSphere				m_BSphereCur;							///< Position이 연산된 BoundingSphere : GetBoundingSphere를 호출하게 되면 업데이트 된다.
	CNtlPLCharacterProperty	*m_pProperty;							///< Character Property
	CNtlPLResource			*m_pResourceClump;						///< Target Clump Resource Reference
	ENTITY_ATOMIC_VEC	    m_vecAtomicList;                        ///< 현재 Clump를 구성하고 있는 Atomic들의 리스트

	SCharScheduleResInfo	m_sScheduleResInfo;
	RwBool					m_bRunTimeAlpha;

	RpHAnimHierarchy		*m_pBaseHierarchy;						///< Base Hierarchy
    
	CNtlAnimLayer			m_AnimLayer[CHARACTER_ANIM_LAYER_END];	///< AnimationLayer(상, 하체)
	CNtlInstanceAnimTable	m_InstanceAnimTable;					///< Animation Resource InstanceData

	SAnimPlayInfo			m_sBaseAnimPlayInfo;					///< 현재 Play중인 Animation의 정보들
	FRAME_MAP				m_mapFrame;								///< Bone Info
	SToonData				m_ToonData;								///< Toon Ink, Toon Paint, Toon Resource
	
    STypeBoneData			*m_pTypeBoneData;						///< Bone Data Reference
	RwInt32				    m_nBoneCount;							///< Bone의 갯수
    RwV3d                   m_vBoneStretchTargetPos;                ///< Bone Stretch Target 위치
    RwChar                  m_szBoneStretchAxisBone[MAX_DEFAULT_NAME];                ///< Bone Stretch 이벤트에서 회전축이 되는 본


	RwV3d					m_vCurPos;								///< Character의 현재 위치
	RwV3d					m_vScale;								///< Character의 Scale를 변경하고자 할때(Instance)
	RwV3d					m_vAngle;								///< Angle					

	RwBool					m_bAnimUpdate;							///< Animation를 Update를 하는지
	RwReal					m_fAnimSpeed;							///< Anim Speed
	RwReal					m_fFastMaxExtent;						///< fast max extent

	CNtlPLEquipItem			*m_pEquipItem;							///< Equip Item

	RwRGBA					m_sHeadColor;							///< Head Color
	RwRGBA					m_sSkinColor;							///< Skin Color

	RwRGBA					m_sColor;								///< Base Color
	RwRGBA					m_sAddColor;							///< Add Color

	RwRGBA					m_sInkColor;							///< Ink Color;

	CNtlPLLinkEffect        m_LinkEffectInstance;                   ///< LinkEffect Instance들을 관리하는 객체
	ListLoopEffect          m_listLoopEffect;                       ///< LoopEffect의 list
	ListSoundHandle         m_listLoopSound;                        ///< LoopSound들의 HANDLE List

	RwBool					m_ToonEdgeVisibility;

	// NPC나 MOB의 경우 m_pResourceClump를 쓰는데 이경우 PLItem에만 해당하는 SimpleMaterial를 쓸수가 없다.
	RwBool m_SimpleMaterial;

	// Character가 멀리 있을때 performance 증진을 위해 animation update을 skip한다.
	RwReal m_SkipAnimUpdateTime;
	RwReal m_SkipAnimUpdateElapsedTime;

	static RwBool m_SkipAnimUpdate;
	static RwBool m_SkipSimpleMaterial;
	static RwBool m_SkipAdge;

	// For NPC, MOB, VEHICLE spherical enviroment mapping texture
	RwTexture* m_pSEMTex;

	RwBool m_PetrifySkillActivated;

protected:
	void	CreateToonData(const RwChar *szTexName, const RwChar *szTexPath);
	RwBool	CreateAnim(RwUInt32 uiAnimKey, RwReal fStartTime = 0.f, RwBool	bLoop = TRUE);

	RwBool	SetThreadSafeProperty(const CNtlPLProperty *pData);
	RwBool	CreateScheduling(const SPLEntityCreateParam * pParam);
	RwBool	CreateThreadSafe(void);

	void	SetApplyRunTimeAlpha(RwBool bFlag);

 	virtual RwSphere*	GetBoundingSphere();
// 	virtual void		CalcBoundingSphere();

	void	UpdatePreBoneScale();
	void	UpdatePostBoneScale();

	// Loop Effect 관련
	void    AddLoopEffect(SLoopEffect* pLoopEffect) {m_listLoopEffect.push_back(pLoopEffect);} ///< LoopEffect List에 LoopEffect를 추가한다.
	void    ClearLoopEffect();                                                                 ///< LoopEffect List에 들어있는 EffectInstance들을 소멸시킨다.
	RwBool  IsExistLoopEffect(const RwChar* szEffectName, const RwChar* szBoneName);           ///< LoopEffect List안에 이름-Bone 쌍의 LoopEffect가 있는지 확인한다.

	// Loop Sound 관련
	void    AddLoopSound(SOUND_HANDLE hSound) {m_listLoopSound.push_back(hSound);}             ///< LoopSound List에 SoundHandle을 추가한다.
	void    ClearLoopSound();                                                                  ///< LoopSound List에 들어있는 LoopSound들을 소멸한다.
	RwBool  IsExistLoopSound(RwChar* szSoundName);                                             ///< LoopSound List에 동일한 SoundHandle이 있는지 확인한다.

	// Animation Event 관련    
	virtual void   OnEventAnimEnd(SEventAnimEnd* pEventAnimEnd);                               ///< Anim End Event를 처리한다.
	virtual void   OnEventHit(SEventAnimHit* pEventHit);    
	virtual void   OnEventWeightTime(SEventWeightTime* pEventTime);
	virtual void   OnEventVisualSound(SEventSound* pEventSound);
	virtual void   OnEventTraceEffect(SEventTrace* pEventTrace);                                ///< Trace Effect를 생성한다.
	virtual void   OnEventVisualEffect(SEventVisualEffect* pEventVisualEffect);                 ///< Visual Effect Event를 처리하고, Visual Effect를 생성한다.
	virtual void   OnEventSubWeapon(SEventSubWeapon* pEventSubWeapon);                          ///< SubWeapon Event를 처리한다.
	virtual void   OnEventPostEffect(SEventPostEffect* pEventPostEffect);						///< Post Effect Event를 처리한다.
	virtual void   OnEventSummonPet(SEventSummonPet* pEventSummonPet);							///< Summon Pet Event를 처리한다.
	virtual void   OnEventAlphaFade(SEventAlpha* pEventAlpha);                                  ///< Alpha Fade Event를 처리한다.
	virtual void   OnEventFootStep(SEventFootStep* pEventFootStep);
	virtual void   OnEventDirect(SEventDirect* pEventDirect);                                   ///< 연출용 이벤트
    virtual void   OnEventColorChange(SEventColorChange* pEventColorChange);                    ///< 색상 변경 이벤트 처리
    virtual void   OnEventStretch(SEventStretch* pEventStretch);                                ///< 본 늘리기 이벤트
    virtual void   OnEventTrigger(SEventTrigger* pEventTrigger);                                ///< 트리거 이벤트
    virtual void   OnEventSkillCancel(SEventSkillCancel* pEventSkillCancel);                    ///< 스킬 캔슬 이벤트

public:
	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual RwBool	Create(const SPLEntityCreateParam *pParam = NULL);
	virtual RwBool	Update(RwReal fElapsed);
	virtual RwBool	Render();
	virtual RwBool	RenderToTexture();
	

	// Occluder Proxy 생성 관련.
	virtual RwBool CreateOccluderProxy();

#ifdef dNTL_WORLD_CULLING_NEW
	virtual RwBool	CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame);
#else
	virtual RwBool	CullingTest(RwCamera* pRwCamera);
#endif
    virtual RwBool  IsCullingTestAllAtomic();                                       ///< 현재 설정된 애니메이션이 Culling Atomic 체크인지 반환한다.

	virtual RwBool	SetProperty(const CNtlPLProperty *pData);
	virtual void	Destroy();

	virtual void CallSchedulingResource(CNtlPLResource *pResource);
	SCharScheduleResInfo *GetCharScheduleResInfo() { return &m_sScheduleResInfo; }
	virtual RwBool IsSchedulingLoadingComplete() {return m_sScheduleResInfo.bLoadComplete;}

	virtual void AddWorld(void);
	virtual void RemoveWorld(void);
	RwBool	IsExistWorld();

	virtual void SetVisible(RwBool bVisible);    
	virtual void SetPicking(RwBool bPicking);

	virtual void SetCullFlags(RwUInt32 uiFlag, RwBool bCulled);
	virtual void SetCullFlags(RwUInt32 uiFlag);

	virtual RwFrame*    GetFrame();
	virtual RwMatrix&   GetMatrix();
	virtual void        SetMatrix(RwMatrix& matWorld);    
    RpClump*            GetClump();
    RpAtomic*           GetAtomic(const std::string& strName);
    RpHAnimHierarchy*   GetBaseHierarchy();
    void                UpdateMaterialSkinInfo();                                               ///< Atomic의 Material 정보를 갱신한다. (순수마인시 사용)

    // 본 관련
    RwMatrix*       GetBoneMatrix(const RwChar *pBoneName);
    RwFrame *       GetBoneByName(const RwChar *pBoneName);
    RwUInt32        GetBoneIndex(const RwChar* pBoneName);
    STypeBoneData*  GetBoneData() {return m_pTypeBoneData;}
    RwChar*         GetBoneName(int nBoneIndex);                                                ///< Bone의 이름을 반환한다.
    void            SetSBoneStretchTargetPos(const RwV3d& vTargetPos, const RwChar* szAxisBone);///< Bone Stretch이벤트의 타겟을 설정한다

	virtual void SetPosition(const RwV3d *pPos);
	RwV3d	GetPosition();

	virtual void SetDirection(const RwV3d *pDir);
	RwV3d	GetDirection();
	RwV3d	GetRight();    
	RwV3d   GetUp();

	void	SetAngleDelta(const RwReal fAngleDeltaY);

	void	SetAngleY(const RwReal fAngleY);
	RwReal	GetAngleY() { return m_vAngle.y; }

	void    SetAngleX(const RwReal fAngleX);
	RwReal	GetAngleX() { return m_vAngle.x; }

	virtual void   SetScale(RwReal fScale);														///< 임의로 Scale를 변경(Instance)
	virtual RwReal GetScale();                                                                  ///< 임의로 조정한 Scale을 반환

	void	SetBaseScale(RwReal fBaseScale);													///< BaseScale 변경
	RwReal  GetBaseScale();                                                                     ///< 설정된 BaseScale을 반환한다.
	void	SetPosOffset(RwV3d *pPos);															///< Bone Scale 했을경우 발이 떨어지면 보정하는 함수

	RwBool	SetAllAnimation(RwUInt32 uiAnimKey, RwReal fStartTime = 0.f, RwBool	bLoop = TRUE);  ///< Set All Animation
	RwBool	SetBaseAnimation(RwUInt32 uiAnimKey, RwReal fStartTime = 0.f, RwBool bLoop = TRUE); ///< Set Base Animation(Client 전용)
	RwUInt32 GetCurBaseAnimKey() {return m_sBaseAnimPlayInfo.m_uiAnimKey;};                     ///< 현재 플레이되고 있는 Anim Key를 반환한다.    
	RwBool	IsExistAnim(RwUInt32 uiAnimKey);													///< uiAnimKey의 Animation이 있는지
	RwBool	SetBlend(EBlendType eBlendType, RwReal fBlendAllpha = 0.f, RwReal fBlendInterval = 0.1f);///< Animation Blend (fBlendAlpha = 1.f 이면 Blend가 안된다.)
	void	SetAnimSpeed(RwReal fSpeed);														///< Animation의 Speed Default(1.f)
	RwReal  GetAnimSpeed();                                                                     ///< Animation의 Speed를 반환한다.
	void	SetAnimUpdate(RwBool bAnimUpdate);													///< Animation를 Update를 할지 말지
	RwBool	GetAnimUpdate();																	///< Animation를 Update를 하는지 검사하는 함수
	RwReal	GetBaseCurrentAnimTime();															///< 현재 Base Layer Animation의 시간을 얻는 함수
	RwReal	GetBaseDurationAnimTime();															///< 현재 Base Layer Animation의 전체 시간을 얻는 함수
	void    SetBaseCurrentAnimTime(RwReal fCurrTime);								    		///< 시간을 강제로 넣는 함수도 필요
	RwBool	GetBaseCurrentAnimEnd();
	SAnimPlayInfo	*GetBaseAnimPlayInfo();														///< 현재 Animation의 간략 정보를 얻는다.
	RwReal  GetDurationAnimTime(RwUInt32 uiAnimKey);											///< uiAnimKey에 해당한는 Animation의 시간을 얻는다.
    void    OnAfterAnimEventRun();                                                              ///< 현재 시간 이후의 모든 Anim Event를 실행시킨다.

	RwReal	GetWidth();																			///< Character Width (x)
	RwReal	GetHeight();																		///< Character Height (y)
	RwReal	GetDepth();																			///< Character Depth (z)	
	RwReal	GetMaxExtent();
	RwReal	GetFastMaxExtent();

	RwBool SetChangeEquipCItem(CNtlPLItem* _pItem, RwBool _bAttach, RwUInt32 _CBitSet);
	RwBool SetRemoveEquipCItem(CNtlPLItem* _pItem, RwUInt32 _CBitSet);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1. pItem		: Change 또는 Remove 하려는 Item 이다.
	// 2. bAttach	: Subweapon 관련 Flag, Subweapon 의 경우 이 Flag이 FALSE여서 실제로는 Attach 되지 않는다는 것을 의미한다.
	// 3. _CBitSet	: Costume 관련 BitFlag Set 이며 반드시 Costume 인 경우에만 입력한다.
	//				  실제 BitFlag Set 값이 0xFFFFFFFF 인 경우는 기존의 방식을 따르며 0xFFFFFFFF가 아닌 경우는 무언가 Attach 또는 Detach 된다는 의미이다.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// < 주의 >
	// 현재 Interface를 사용할때 주의해야 할 사항은 다음과 같다.
	// BitSet에 해당하는 모든메쉬는 단순히 Lib내부적으로 attach, detach된다. 때문에 이 함수를 항상 마지막에 실행시켜야 한다.
	// 이 함수를 먼저 실행한 후 기존방식으로 item을 입거나 벗게되면 다시 그 item때문에 default item부분이 입혀지거나 벗겨지기 때문이다.
	// 항상 Costume관련된 입기 interface는 마지막에 실행한다. 반대로 벗을때는 항상 먼저 실행한다. 동일한 이유이다.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual RwBool	SetChangeEquipItem(CNtlPLItem *pItem, RwBool bAttach = TRUE, RwUInt32 _CBitSet = dCOSTUME_BITSET_INVALID);
	virtual RwBool	SetRemoveEquipItem(CNtlPLItem *pItem, RwUInt32 _CBitSet = dCOSTUME_BITSET_INVALID);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Toon edge visibiltity를 결정하는 함수
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 호출하면 Toon edge rendering을 끄거나 켤수 있다.
	// 이 함수를 호출하게 되면 내부적으로 Atomic에 Flag를 갖게 되므로 유지보수에 주의하여야 한다.
	// Edge를 끈 상태에서 서버로 부터 부분 mesh를 입는경우, 벗는경우등등 캐릭터의 외관상태변화에 따라 이 함수가 호출되어야 할것이다.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VOID SetToonEdgeVisibility4CurEquipItem(RwBool _Visibility);

	void UpdateCharacterAtomicList();

	CNtlPLEquipItem* GetEquipItem() {return m_pEquipItem;}
	RwBool  SetItemGradeEffect(ENtlPLItemGrade eGrade);                                         ///< 현재 장착하고 있는 무기에 Grade Effect를 적용한다.

	SToonData	*GetToonData() { return &m_ToonData; }											///< Toon Data를 얻는다.							

	void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);								///< Base Color
	RwRGBA	*GetColor() { return &m_sColor; }

	void SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	RwRGBA *GetAddColor() { return &m_sAddColor; }

	virtual void SetAlpha(RwUInt8 byValue);																///< Base Alpha
	virtual void SetWeightAlpha(RwReal fWeightValue);													///< Weight Alpha
	virtual void SetAtomicWeightAlpha(const RwChar *pAtomicName, RwReal fWeightValue);                  ///< Atomic Weight Alpha Value를 세팅한다.

	virtual void SetSkinColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);					///< Skin Color
	RwRGBA	*GetSkinColor() { return &m_sSkinColor; }		

	virtual void SetHeadColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);					///< Head Color
	RwRGBA *GetHeadColor() { return &m_sHeadColor; }

	virtual void SetInkColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);					///< Ink Color
	virtual void SetInkThickness(RwReal fThickness = DEFAULT_INK_THICKNESS);					///< Ink Thickness

	virtual int	CallBackBaseAnim(void* pAnimEvent);											///< Animation Event를 받는 Callback 함수
	static RpAtomic *RenderCallBack(RpAtomic *pAtomic);

	// Link Effect 관련
	VecLinkEffect*      GetVecLinkEffect() {return &(m_LinkEffectInstance.m_vecLinkEffect);}	///< LinkEffect Vector를 반환한다.
    CNtlInstanceEffect* AttachLinkEffect(SEventLinkEffect* pEventLinkEffect);					///< Link Effect를 Attach 시킨다.
	RwBool              DetachLinkEffect(CNtlInstanceEffect* pLinkEffect);                      ///< Link Effect를 Detach 시킨다.

	RwV3d  GetFaceCameraPos() { return m_pProperty->GetFaceCameraPos();}							///< Get Face Camera Position
	RwV3d  GetFaceCameraLookAt() { return m_pProperty->GetFaceCameraLookAt();}					///< Set Face Camera LookAt Height 
    RwReal GetAlphaDistance();                              

	RwBool IsApplyGroundPitchAngle(void);

	static void fpRenderCB(void* _pParam);

	// Animation update를 skip할때 문제가 발생할 소지가 있다. ex) 튜토리얼에서 월드 트랜스폼이 이루어지지 못하고 char animation으로만 처리할때등등
	// 구간 구간 문제가 생기는 부분에 설정하고 해제한다; Renderstate설정및 복원과 같은 개념으로 사용하면 된다.
	static void SetSkipAnimUpdate(RwBool _Skip = TRUE);

	// Simple material부분의 game option check에 따른 유무를 판단하기 위함 함수이다.
	static void SetSkipSimpleMaterial(RwBool _Skip = TRUE);
	static RwBool GetSkipSimpleMaterial(void);

	// edge를 skip 시킨다.(형석)
	static void SetSkipEdge(RwBool _Skip = FALSE);
	static RwBool GetSkipEdge(void);

	void	SetDXT1AlphaCheck(RwBool _DXT1AlphaCheck);

	// simple material; simplify textures
	RwBool	GetSimpleMaterial() { return m_SimpleMaterial; }
	VOID	SetSimpleMaterial(RwBool _SimpleMaterial);

	// Petrify
	RwBool	GetPetrifyMaterial() { return m_PetrifySkillActivated; }
	VOID	SetPetrifyMaterial(RwBool _PetrifyFlag);

	// Spherical Enviroment Map
	RwTexture* GetSEM() { return m_pSEMTex; }

	// enviroment map uv ani toggling
	static VOID ToggleEMUVAni(RwBool _Visibility);
};

#endif
