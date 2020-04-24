/*****************************************************************************
*
* File			: NtlPLObject.h
* Author		: HyungSuk Jang
* Copyright	: (주)NTL
* Date			: 2005. 8. 11	
* Abstract		: Presentation layer object entity class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_PLOBJECT_H__
#define __NTL_PLOBJECT_H__

// 형석 잠시 주석처리.
// #define FADE_OBJECT_TIME        2.0f                ///< Object가 Fade In/Out에 걸리는 시간 
#define FADE_EFFECT_GAP_TIME    1.0f                ///< Effect가 표시된후 얼마후에 Object를 표시할지의 Gap

#include "NtlPLAttach.h"
#include "NtlPLGlobal.h"
#include "NtlAtomic.h"
#include "NtlPLResource.h"
#include "NtlPLObjectProperty.h"
#include "NtlPLUVAnim.h"
#include "NtlPLLinkEffect.h"
#include "NtlAnimLayer.h"
#include "NtlPLEntityRenderHelpers.h"
#include "OccluderProxy.h"
#include "NtlPLObjectType.h"

// #include <vector>
// using std::vector;

typedef union
{
	RwTexCoords uv;
	RwUInt32    intUV[2];
}
LtMapIntUV_;

struct sSCHEDULING_LM_ATOM_PROP
{
	sSCHEDULING_LM_ATOM_PROP()
	{
		_pTexCoords = NULL;
	}

	~sSCHEDULING_LM_ATOM_PROP()
	{
		NTL_ARRAY_DELETE(_pTexCoords);
	}

	RwChar			_szAtomName[64];
	RwChar			_szTexName[64];
	RwInt32			_UVCnt;
	RwTexCoords*	_pTexCoords;
};

struct sSCHEDULING_LM_PROP
{
	RwUInt32	_uiLtMapObjectFlags;
	RwUInt32	_uiLtMapMaterialFlags;

	vector<sSCHEDULING_LM_ATOM_PROP*> _vecSchedulingLMAtomProp;
};

/// 스케쥴링 로딩시 필요한 정보를 담는 구조체
struct SObjectSchedulingInfo
{
    RwBool      bLoadComplete;
    RwUInt32    uiAnimKey;
    RwReal      fAnimStartTime;
    RwBool      bAnimLoop;

    SObjectSchedulingInfo()
    {
        bLoadComplete = FALSE;
        uiAnimKey = 0;
        fAnimStartTime = 0.0f;
        bAnimLoop = FALSE;
    }
};

class CNtlPLEntityAlphaWeightBlend;

struct SPLObjectCreateParam : public SPLEntityCreateParam
{
	RwBool bLoadMap;            // 맵을 로드할때(Fade가 적용되지 않아야 할때)는 True, 아니면 False
};

struct SCollisionInfo;
struct RpHAnimHierarchy;
struct RwFrame;
struct SPLLightObjectCreateParam;

class CNtlPLResource;
class CNtlWorldShadow;

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

class CNtlPLObject : public CNtlPLAttach, public COccluderProxy
{
public:
	/// Fade System에 사용될 Flag값
	enum EObjectFadeState
	{
		FADE_VISIBLE,
		FADE_IN_OBJECT,
		FADE_IN_EFFECT,
		FADE_OUT_OBJECT,
		FADE_OUT_EFFECT,
		FADE_NOT_VISIBLE,
	};

public:
	void* operator new(size_t size);
	void operator delete(void *pObj);

	CNtlPLObject();
	virtual ~CNtlPLObject();

	virtual RwBool Create( const SPLEntityCreateParam * pParam = NULL );
	virtual void Destroy(void);
	virtual RwBool Update(RwReal fElapsed);

	virtual RwBool Render(void);
	virtual RwBool RenderToTexture();

	// Occluder Proxy 생성 관련.
	virtual RwBool CreateOccluderProxy();

#ifdef dNTL_WORLD_CULLING_NEW
	virtual RwBool CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame);
#else
	virtual RwBool CullingTest(RwCamera* pRwCamera);
#endif
	virtual RwBool  IsCullingTestAllAtomic();                                       ///< 현재 설정된 애니메이션이 Culling Atomic 체크인지 반환한다.

	virtual RwBool SetProperty(const CNtlPLProperty *pData);
	virtual CNtlPLObjectProperty* GetProperty() {return m_pProperty;}

	virtual void AddWorld(void);
	virtual void RemoveWorld(void);

	virtual void CallPreSchedulingResource(void);
	virtual void CallSchedulingResource(CNtlPLResource *pResource);
	virtual void CallSchedulingResourceOnly();

	virtual void SetPosition(const RwV3d *pPos);
	virtual RwV3d GetPosition(void);

	virtual void SetScale( const RwV3d * pScale );
	virtual const RwV3d * GetScale( void ) const;
	virtual void SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle);
	virtual const RwV3d* GetRotate(void) const;    
	virtual RwV3d GetDirection();

	virtual RwReal GetVisibleCullingDistance(void);

	virtual void  SetVisible(RwBool bVisible);													///< 오브젝트의 표현 유무를 설정한다.
	void SetEffectVisible(RwBool bVisible);														///< 이펙트의 표현 유무를 설정한다.

	void SetAlpha(RwUInt8 byValue);																///< 오브젝트의 Alpha를 설정한다.
	void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);								///< 오브젝트에 Color를 설정한다.
	RwRGBA	*GetColor() { return &m_sColor; }													///< 오브젝트의 Color를 얻는다.

	void SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	RwRGBA	*GetAddColor() { return &m_sAddColor; }

	virtual RwFrame*	GetFrame(void);

	void				SetMatrix( RwMatrix & matWorld );
	RwMatrix&			GetMatrix(void);
	RwMatrix*			GetBoneMatrix(const RwChar *pBoneName);

	void				GetBoneName(std::vector<std::string>& vecBoneName);

	virtual RwSphere*	GetBoundingSphere();
	virtual void		CalcBoundingSphere();

	RwBool			IsActiveUpdate( void ) { return m_bActiveUpdate; }
	RwBool          IsSchedulingLoadingComplete() {return m_sScheduleInfo.bLoadComplete;}       ///< 스케쥴 로딩이 완료

	RpClump *GetClump( void ) const;

	RwBool          SetUVAnim(const RwChar* szUVAnimFileName);									///< UVAnim을 설정한다.
	CNtlPLUVAnim*   GetUVAnim() {return m_pUVAnim;}												///< UVAnim 객체를 반환한다.

	RwBool          SetAnimation(const RwChar* szAnimFileName);									///< 애니메이션을 적용한다.
	virtual RwBool	SetTriggerAnimation(RwUInt32 uiAnimKey, RwReal fStartTime = 0.0f, RwBool bLoop = TRUE);				///< Trigger Animation을 재생한다.
	virtual int		CallBackBaseAnim(void* pEventData);										///< Animation Event의 CallBack 함수    
	void            SetAnimUpdate(RwBool bPlay) {m_bAnimPlay = bPlay;}								///< 애니메이션 플레이/스톱을 적용한다.
	RwBool			GetPlayAnim() { return m_bAnimPlay; }										///< 애니메이션이 플레이가 되는지
	RwBool			SetPlayAnimTime(RwReal fStartAnimTime);										///< Animation Play 시작 시간 (0.f - 1.f)
	RwBool			SetPlayAnimSpeed(RwReal fAnimSpeed);										///< Animaiton Speed (1.f: Default )
	RwReal			GetBaseCurrentAnimTime();															///< 현재 Base Layer Animation의 시간을 얻는 함수
	RwReal			GetAnimPlayTime(RwUInt32 uiAnimKey);										///< Key에 해당하는 Anim의 Play Time을 반환한다.
	RwReal			GetBaseDurationAnimTime();													///< Animation의 Play Time을 반환한다.
	void			SetBaseCurrentAnimTime(RwReal fCurrTime);								    		///< 시간을 강제로 넣는 함수도 필요

	RwBool			GetEnableShadow();															///< Shadow를 만들것인지

	// Doodads PSM
	RwBool				GetEnableGenShadowMap();													///< ShadowMap을 생성을 해야 하는지
	RwBool				AreThereObjPSMap();
	VOID				DeleteDoodadShadow();
	ENTITY_ATOMIC_VEC*	GetAtomicList();															///< Atomic List Return을 한다.	
	VOID				SetAtomicPSMap();

	virtual const RwBBox* GetBoundingBox(void) {return &m_bbox;}								///< Bounding Box를 반환한다.

	// Link Effect 관련
	VecLinkEffect*      GetVecLinkEffect() {return &(m_LinkEffectInstance.m_vecLinkEffect);}	///< LinkEffect Vector를 반환한다.
	CNtlInstanceEffect* AttachLinkEffect(SEventLinkEffect* pEventLinkEffect);					///< Link Effect를 Attach 시킨다.
	RwBool              DetachLinkEffect(CNtlInstanceEffect* pLinkEffect);						///< Link Effect를 Detach 시킨다.

	// World Editor를 위한 함수
	virtual RwBBox  GetTriggerAABBInfo();														///< 월드에디터의 트리거 정보에 사용하기 위한 AABB의 정보를 반환한다.
	virtual RwBBox  GetTriggerAABBInfo( const RwV3d& vPos, 
		const RwV3d& vRotate, 
		const RwV3d& vScale);	

	// Fade System 관련
	RwBool          UpdateFadeSystem();                                                         ///< 설정된 Culling Distance에 따라서 Object를 Fade 시킨다.
	void            SetFadeEnable(RwBool bEnable);                                              ///< Fade Enable 유무를 설정한다

	void    SetTransform(void);

	// Milepost 관련 프로퍼티
	RwUInt32        GetMilepostID() {return m_uiMilepostTextID;}                                ///< Milepost Text ID를 반환한다.
	void            SetMilepostID(RwUInt32 uiMilepostTextID) {m_uiMilepostTextID = uiMilepostTextID;} ///< Milepost Text ID를 설정한다.

	// Toon 관련
	void            CheckToonData();                                                            ///< Toon 적용이 가능한지 확인한다.
	SToonData*      GetToonData() {return m_pToonData;}

	void		ResetUV();
	RwBool		CheckUVsSetUpForLightMapping();
	RwUInt32	GetObjectSerialID()								{ return m_uiObjectSerialID; }
	void		SetObjectSerialID(RwUInt32 uiObjectSerialID) { m_uiObjectSerialID = uiObjectSerialID; }
	void		DeleteLightmapFile();
	void		CheckLightmapFlags();

	// attach light
	virtual void		AttachLightObject();	

	RwBBox				GetDefaultAABB();

	// object type
	RwUInt32			GetObjectType();
	void				SetObjectType(RwUInt32 uiObjectType);
	CNtlPLObjectType*	GetObjectTypePtr() { return m_pObjectType; }

protected:

	RwBool	SetThreadSafeProperty(const CNtlPLProperty *pData);
	RwBool	CreateScheduling(const SPLEntityCreateParam * pParam);
	RwBool	CreateThreadSafe(void);

	RwBool  CreateAnim();																		///< Anim 관련 정보및 객체를 생성한다.
	RwBBox  CreateDefaultAABB();																///< Property에 AABB정보가 없을때 기본 AABB정보를 생성한다	
	RwBool  UpdateFading(RwReal fElapsedTime);                                                  ///< 페이딩한다.

	// Loop Effect 관련
	void    AddLoopEffect(SLoopEffect* pLoopEffect) {m_listLoopEffect.push_back(pLoopEffect);} ///< LoopEffect List에 LoopEffect를 추가한다.
	void    ClearLoopEffect();                                                                 ///< LoopEffect List에 들어있는 EffectInstance들을 소멸시킨다.
	RwBool  IsExistLoopEffect(const RwChar* szEffectName, const RwChar* szBoneName);           ///< LoopEffect List안에 이름-Bone 쌍의 LoopEffect가 있는지 확인한다.

	// Loop Sound 관련
	void    AddLoopSound(SOUND_HANDLE hSound) {m_listLoopSound.push_back(hSound);}             ///< LoopSound List에 SoundHandle을 추가한다.
	void    ClearLoopSound();                                                                  ///< LoopSound List에 들어있는 LoopSound들을 소멸한다.
	RwBool  IsExistLoopSound(RwChar* szSoundName);                                             ///< LoopSound List에 동일한 SoundHandle이 있는지 확인한다.
	void    UpdateLoopSoundPos();                                                              ///< LoopSound 위치를 업데이트 한다.

	// Animation Event 관련
	virtual void OnEventAnimEnd(SEventAnimEnd* pEventAnimEnd);
	virtual void OnEventVisualSound(SEventSound* pEventSound);									
	virtual void OnEventVisualEffect(SEventVisualEffect* pEventVisualEffect);    
	virtual void OnEventAlphaFade(SEventAlpha* pEventAlpha);
	virtual void OnEventTMQ(SEventAnimCinematic* pEventTMQ);
	virtual void OnEventExplosion(SEventExplosion* pEventExplosion);

	void    SetAtomicWeightAlpha(const RwChar *pAtomicName, RwReal fWeightValue);               ///< Atomic Weight Alpha Value를 세팅한다.

	void	AddSceneUpdate();
	void	RemoveSceneUpdate();

public:
	void			SetLightMapMaterialFlags(RwUInt32 uiFlags);
	void			SetLightMapObjectFlags(RwUInt32 uiFlags);
	RwUInt32		GetLightMapMaterialFlags();
	RwUInt32		GetLightMapObjectFlags();
	void			FreeSchedulingLTProp();
	void			AllocSchedulingLTProp();
	RwBool			IsRpWorldSectorList(RpWorldSector* pRpWorldSector);
	void			UpdateRpSectorOBBCheck();

	// attach dogi gym. emblem texture. careful for Destroy()
	RwBool			SetAtomicEmblemMap(RwTexture* _pEmblemMap);
	RwBool			GetEmblemMarkEnabled();
	void			DestroyEmblemMap();

	// dojo	
	CNtlPLEntity*	GetDojo()							{ return m_pDojoEntity; }
	void			SetDojo(CNtlPLEntity* pDojoEntity)	{ m_pDojoEntity = pDojoEntity; }

public:
	static RpAtomic *RenderCallBack(RpAtomic *pAtomic);

public:
	//static void					SetFarPlane(RwReal fFarDist);

protected:
	//static RwPlane				g_planeFar;

#ifdef dNTL_WORLD_TOOL_MODE
public:
	static void			SaveSwapFile(RwReal x, RwReal y, RwReal z);
	static void			LoadSwapFile(RwReal x, RwReal y, RwReal z);
#endif

protected:
	sSCHEDULING_LM_PROP*		m_pSchedulingLMProp;
	SObjectSchedulingInfo       m_sScheduleInfo;        ///< 스케쥴링 로딩 관련 정보 
	RwUInt32					m_uiObjectSerialID;

	CNtlPLObjectType*			m_pObjectType;

	RwBool						m_bActiveUpdate;
	RwBool						m_bLoadMap;

	CNtlPLResource *			m_pClumpResource;
	ENTITY_ATOMIC_VEC	        m_vecAtomicList;         ///< 현재 Clump를 구성하고 있는 Atomic들의 리스트
	FRAME_MAP				    m_mapFrame;				 ///< Bone Info

	RwV3d						m_vModelAngle;
	RwV3d						m_vModelScale;
	RwV3d						m_vWorldPosition;

	CNtlPLObjectProperty*       m_pProperty;            ///< 오브젝트 프로퍼티 객체	

	CNtlPLUVAnim*               m_pUVAnim;              ///< UVAnim 파일 객체                
	RpHAnimHierarchy*           m_pBaseHierarchy;		///< Base Hierarchy
	CNtlPLResource*             m_pAnimResource;        ///< Anim Resource
	RwBool                      m_bAnimPlay;            ///< Anim Play 유무
	RwBool						m_bHaveAnim;			///< Anim 존재 유무.

	RwReal						m_fAnimSpeed;			///< Anim Speed

	RwRGBA						m_sColor;				///< Color				
	RwRGBA						m_sAddColor;			///< AddColor

	RwSphere					m_BSphere;				///< Position이 연산되지 않은 BoundSphere
	RwSphere					m_BSphereCur;			///< Position이 연산된 BoundingSphere : GetBoundingSphere를 호출하게 되면 업데이트 된다.

	CNtlPLLinkEffect            m_LinkEffectInstance;   ///< LinkEffect Instance들을 관리하는 객체

	RwBBox                      m_bbox;                 ///< Object의 Bounding Box

	// Fade System 관련
	EObjectFadeState            m_eFadeState;           ///< 현재 Object의 Fade State
	RwReal                      m_fPrevCameraDistance;  ///< 이전의 카메라와의 거리
	RwReal                      m_fFadeTime;            ///< Fade Time    
	CNtlPLEntityAlphaWeightBlend* m_pFadeBlend;         ///< Fade Blend 설정용 알파 블렌딩 객체

	// Trigger Object의 Animation 관련
	CNtlAnimLayer*				m_pAnimLayer;			///< 애니메이션 레이어
	CNtlInstanceAnimTable*		m_pInstanceAnimTable;	///< Animation Resource Instance Table
	RwUInt32					m_uiCurAnimKey;			///< 현재 적용되고 있는 AnimKey

	ListSoundHandle				m_listLoopSound;        ///< LoopSound들의 HANDLE List
	ListLoopEffect				m_listLoopEffect;       ///< LoopEffect의 list

	// 지형 오브젝트 그림자 관련
	vector<CNtlWorldShadow*>    m_vecNtlWorldShadow;

	RwUInt32                    m_uiMilepostTextID;      ///< 이정표로 사용될때 연결될 텍스트 테이블의 ID

	// 툰관련
	SToonData*				    m_pToonData;			///< Toon Ink, Toon Paint, Toon Resource

	// 인도어
	CNtlOBB						m_OBB;					///< 인도어에서 사용되며, RpWorldSector를 검색하기 위해 사용.
	vector<RpWorldSector*>		m_vecRpWorldSector;		///< 인도어에서 사용되며, 포함되어 있는 RpWorldSectorlist

	// Dojo
	CNtlPLEntity*				m_pDojoEntity;

	// attach light
	struct SLightObjectAttachData
	{
		RwV3d						vPos;
		RwChar						chBoneName[MAX_ATTR_BONE_NAME];
		SPLLightObjectCreateParam*	pPLLightObjectCreateParam;
	};
	std::vector<SLightObjectAttachData*> m_vecAttachLightParam;

public:
	virtual RwBool		LoadFromFile(FILE* pFile, EActiveWorldType eActiveWorldType);
	virtual RwBool		SaveIntoFile(FILE* pFile, EActiveWorldType eActiveWorldType);
	static	RwInt32		SkipToFile(FILE* pFile, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType);

	static	BYTE*		SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType);
	static	BYTE*		SaveIntoFileFromFileMemVisibilityDistance(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwReal fVD, RwReal fMinVD, RwReal fMaxVD);
	static	BYTE*		SkipToFileMem(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType);
	static	BYTE*		SkipToFileMemGetEnabledTrigger(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwBool* pResult);
	static	BYTE*		SkipToFileMemGetEnabledPEC(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwBool* pResult);

	// Projection Shadow
	virtual RwBool		SavePSMap(FILE* pFile);
	virtual RwBool		LoadPSMap(FILE* pFile);
	static RwInt32		SkipPSMap(FILE* pFile);

	static	BYTE*		SavePSMapFromFileMem(FILE* pFile, BYTE* pFileMem);
	static	BYTE*		SkipPSMapToFileMem(BYTE* pFileMem);

	// lightmap 관련
	virtual RwBool		SaveLightmap(FILE* pFile);
	virtual RwBool		LoadLightmap(FILE* pFile);
	static  RwInt32		SkipLightmap(FILE* pFile);

	static  BYTE*		SaveLightmapFromFileMem(FILE* pFile, BYTE* pFileMem);
	static  BYTE*		SkipLightmapToFileMem(BYTE* pFileMem);

	virtual RwBool		LoadSchedulingLightmapProp(FILE* pFile);

	// Attach LightObject
	virtual RwBool		LoadLightObjectAttachData(FILE* pFile);	
	virtual RwBool		SaveLightObjectAttachData(FILE* pFile);
	static  RwInt32		SkipLightObjectAttachData(FILE* pFile);

	static  BYTE*		SaveLightObjectAttachDataFromFileMem(FILE* pFile, BYTE* pFileMem);
	static  BYTE*		SkipLightObjectAttachDataToFileMem(BYTE* pFileMem);
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

class CNtlPLObject : public CNtlPLAttach, public COccluderProxy
{
public:
	/// Fade System에 사용될 Flag값
	enum EObjectFadeState
	{
		FADE_VISIBLE,
		FADE_IN_OBJECT,
		FADE_IN_EFFECT,
		FADE_OUT_OBJECT,
		FADE_OUT_EFFECT,
		FADE_NOT_VISIBLE,
	};

public:
	void* operator new(size_t size);
	void operator delete(void *pObj);

	CNtlPLObject();
	virtual ~CNtlPLObject();

	virtual RwBool Create( const SPLEntityCreateParam * pParam = NULL );
	virtual void Destroy(void);
	virtual RwBool Update(RwReal fElapsed);

	virtual RwBool Render(void);
	virtual RwBool RenderToTexture();

	// Occluder Proxy 생성 관련.
	virtual RwBool CreateOccluderProxy();

#ifdef dNTL_WORLD_CULLING_NEW
	virtual RwBool CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame);
#else
	virtual RwBool CullingTest(RwCamera* pRwCamera);
#endif
    virtual RwBool  IsCullingTestAllAtomic();                                       ///< 현재 설정된 애니메이션이 Culling Atomic 체크인지 반환한다.

	virtual RwBool SetProperty(const CNtlPLProperty *pData);
	virtual CNtlPLObjectProperty* GetProperty() {return m_pProperty;}

	virtual void AddWorld(void);
	virtual void RemoveWorld(void);

	virtual void CallPreSchedulingResource(void);
	virtual void CallSchedulingResource(CNtlPLResource *pResource);
	virtual void CallSchedulingResourceOnly();

	virtual void SetPosition(const RwV3d *pPos);
	virtual RwV3d GetPosition(void);

	virtual void SetScale( const RwV3d * pScale );
	virtual const RwV3d * GetScale( void ) const;
	virtual void SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle);
	virtual const RwV3d* GetRotate(void) const;    
	virtual RwV3d GetDirection();

	virtual RwReal GetVisibleCullingDistance(void);

	virtual void  SetVisible(RwBool bVisible);													///< 오브젝트의 표현 유무를 설정한다.
	void SetEffectVisible(RwBool bVisible);														///< 이펙트의 표현 유무를 설정한다.

	void SetAlpha(RwUInt8 byValue);																///< 오브젝트의 Alpha를 설정한다.
	void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);								///< 오브젝트에 Color를 설정한다.
	RwRGBA	*GetColor() { return &m_sColor; }													///< 오브젝트의 Color를 얻는다.

	void SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	RwRGBA	*GetAddColor() { return &m_sAddColor; }

	virtual RwFrame*	GetFrame(void);

	void				SetMatrix( RwMatrix & matWorld );
	RwMatrix&			GetMatrix(void);
	RwMatrix*			GetBoneMatrix(const RwChar *pBoneName);

	void				GetBoneName(std::vector<std::string>& vecBoneName);

	virtual RwSphere*	GetBoundingSphere();
	virtual void		CalcBoundingSphere();

	RwBool			IsActiveUpdate( void ) { return m_bActiveUpdate; }
    RwBool          IsSchedulingLoadingComplete() {return m_sScheduleInfo.bLoadComplete;}       ///< 스케쥴 로딩이 완료

	RpClump *GetClump( void ) const;

	RwBool          SetUVAnim(const RwChar* szUVAnimFileName);									///< UVAnim을 설정한다.
	CNtlPLUVAnim*   GetUVAnim() {return m_pUVAnim;}												///< UVAnim 객체를 반환한다.

	RwBool          SetAnimation(const RwChar* szAnimFileName);									///< 애니메이션을 적용한다.
	virtual RwBool	SetTriggerAnimation(RwUInt32 uiAnimKey, RwReal fStartTime = 0.0f, RwBool bLoop = TRUE);				///< Trigger Animation을 재생한다.
	virtual int		CallBackBaseAnim(void* pEventData);										///< Animation Event의 CallBack 함수    
	void            SetAnimUpdate(RwBool bPlay) {m_bAnimPlay = bPlay;}								///< 애니메이션 플레이/스톱을 적용한다.
	RwBool			GetPlayAnim() { return m_bAnimPlay; }										///< 애니메이션이 플레이가 되는지
	RwBool			SetPlayAnimTime(RwReal fStartAnimTime);										///< Animation Play 시작 시간 (0.f - 1.f)
	RwBool			SetPlayAnimSpeed(RwReal fAnimSpeed);										///< Animaiton Speed (1.f: Default )
	RwReal			GetBaseCurrentAnimTime();															///< 현재 Base Layer Animation의 시간을 얻는 함수
	RwReal			GetAnimPlayTime(RwUInt32 uiAnimKey);										///< Key에 해당하는 Anim의 Play Time을 반환한다.
	RwReal			GetBaseDurationAnimTime();													///< Animation의 Play Time을 반환한다.
	void			SetBaseCurrentAnimTime(RwReal fCurrTime);								    		///< 시간을 강제로 넣는 함수도 필요

	RwBool			GetEnableShadow();															///< Shadow를 만들것인지

	// Doodads PSM
	RwBool				GetEnableGenShadowMap();													///< ShadowMap을 생성을 해야 하는지
	RwBool				AreThereObjPSMap();
	VOID				DeleteDoodadShadow();
	ENTITY_ATOMIC_VEC*	GetAtomicList();															///< Atomic List Return을 한다.
	VOID				SavePSMap(FILE* _pFile);
	VOID				LoadPSMap(FILE* _pFile);
	VOID				SetAtomicPSMap();

	virtual const RwBBox* GetBoundingBox(void) {return &m_bbox;}								///< Bounding Box를 반환한다.

	// Link Effect 관련
	VecLinkEffect*      GetVecLinkEffect() {return &(m_LinkEffectInstance.m_vecLinkEffect);}	///< LinkEffect Vector를 반환한다.
	CNtlInstanceEffect* AttachLinkEffect(SEventLinkEffect* pEventLinkEffect);					///< Link Effect를 Attach 시킨다.
	RwBool              DetachLinkEffect(CNtlInstanceEffect* pLinkEffect);						///< Link Effect를 Detach 시킨다.

	// World Editor를 위한 함수
	virtual RwBBox  GetTriggerAABBInfo();														///< 월드에디터의 트리거 정보에 사용하기 위한 AABB의 정보를 반환한다.
	virtual RwBBox  GetTriggerAABBInfo( const RwV3d& vPos, 
		const RwV3d& vRotate, 
		const RwV3d& vScale);	

	// Fade System 관련
	RwBool          UpdateFadeSystem();                                                         ///< 설정된 Culling Distance에 따라서 Object를 Fade 시킨다.
	void            SetFadeEnable(RwBool bEnable);                                              ///< Fade Enable 유무를 설정한다

	void    SetTransform(void);

	// Milepost 관련 프로퍼티
	RwUInt32        GetMilepostID() {return m_uiMilepostTextID;}                                ///< Milepost Text ID를 반환한다.
	void            SetMilepostID(RwUInt32 uiMilepostTextID) {m_uiMilepostTextID = uiMilepostTextID;} ///< Milepost Text ID를 설정한다.

    // Toon 관련
    void            CheckToonData();                                                            ///< Toon 적용이 가능한지 확인한다.
    SToonData*      GetToonData() {return m_pToonData;}

	// lightmap 관련
	virtual RwBool		SaveLightmap(FILE* pFile);
	virtual RwBool		LoadLightmap(FILE* pFile);
	static  RwInt32		SkipLightmap(FILE* pFile);

	static  BYTE*		SaveLightmapFromFileMem(FILE* pFile, BYTE* pFileMem);
	static  BYTE*		SkipLightmapToFileMem(BYTE* pFileMem);

	virtual RwBool		LoadSchedulingLightmapProp(FILE* pFile);

	void		ResetUV();
	RwBool		CheckUVsSetUpForLightMapping();
	RwUInt32	GetObjectSerialID()								{ return m_uiObjectSerialID; }
    void		SetObjectSerialID(RwUInt32 uiObjectSerialID) { m_uiObjectSerialID = uiObjectSerialID; }
	void		DeleteLightmapFile();
	void		CheckLightmapFlags();

	// attach light
	virtual void		AttachLightObject();
	virtual RwBool		LoadLightObjectAttachData(FILE* pFile);	
	virtual RwBool		SaveLightObjectAttachData(FILE* pFile);
	static  RwInt32		SkipLightObjectAttachData(FILE* pFile);
		
	static  BYTE*		SaveLightObjectAttachDataFromFileMem(FILE* pFile, BYTE* pFileMem);
	static  BYTE*		SkipLightObjectAttachDataToFileMem(BYTE* pFileMem);

	RwBBox		GetDefaultAABB();

	// object type
	RwUInt32			GetObjectType();
	void				SetObjectType(RwUInt32 uiObjectType);
	CNtlPLObjectType*	GetObjectTypePtr() { return m_pObjectType; }

protected:

	RwBool	SetThreadSafeProperty(const CNtlPLProperty *pData);
	RwBool	CreateScheduling(const SPLEntityCreateParam * pParam);
	RwBool	CreateThreadSafe(void);

	RwBool  CreateAnim();																		///< Anim 관련 정보및 객체를 생성한다.
	RwBBox  CreateDefaultAABB();																///< Property에 AABB정보가 없을때 기본 AABB정보를 생성한다	
	RwBool  UpdateFading(RwReal fElapsedTime);                                                  ///< 페이딩한다.

	// Loop Effect 관련
	void    AddLoopEffect(SLoopEffect* pLoopEffect) {m_listLoopEffect.push_back(pLoopEffect);} ///< LoopEffect List에 LoopEffect를 추가한다.
	void    ClearLoopEffect();                                                                 ///< LoopEffect List에 들어있는 EffectInstance들을 소멸시킨다.
	RwBool  IsExistLoopEffect(const RwChar* szEffectName, const RwChar* szBoneName);           ///< LoopEffect List안에 이름-Bone 쌍의 LoopEffect가 있는지 확인한다.

	// Loop Sound 관련
	void    AddLoopSound(SOUND_HANDLE hSound) {m_listLoopSound.push_back(hSound);}             ///< LoopSound List에 SoundHandle을 추가한다.
	void    ClearLoopSound();                                                                  ///< LoopSound List에 들어있는 LoopSound들을 소멸한다.
	RwBool  IsExistLoopSound(RwChar* szSoundName);                                             ///< LoopSound List에 동일한 SoundHandle이 있는지 확인한다.
	void    UpdateLoopSoundPos();                                                              ///< LoopSound 위치를 업데이트 한다.

	// Animation Event 관련
	virtual void OnEventAnimEnd(SEventAnimEnd* pEventAnimEnd);
	virtual void OnEventVisualSound(SEventSound* pEventSound);									
	virtual void OnEventVisualEffect(SEventVisualEffect* pEventVisualEffect);    
	virtual void OnEventAlphaFade(SEventAlpha* pEventAlpha);
	virtual void OnEventTMQ(SEventAnimCinematic* pEventTMQ);
	virtual void OnEventExplosion(SEventExplosion* pEventExplosion);

	void    SetAtomicWeightAlpha(const RwChar *pAtomicName, RwReal fWeightValue);               ///< Atomic Weight Alpha Value를 세팅한다.

	void	AddSceneUpdate();
	void	RemoveSceneUpdate();

public:
	void			SetLightMapMaterialFlags(RwUInt32 uiFlags);
	void			SetLightMapObjectFlags(RwUInt32 uiFlags);
	RwUInt32		GetLightMapMaterialFlags();
	RwUInt32		GetLightMapObjectFlags();
	void			FreeSchedulingLTProp();
	void			AllocSchedulingLTProp();
	RwBool			IsRpWorldSectorList(RpWorldSector* pRpWorldSector);
	void			UpdateRpSectorOBBCheck();

	// attach dogi gym. emblem texture. careful for Destroy()
	RwBool			SetAtomicEmblemMap(RwTexture* _pEmblemMap);
	RwBool			GetEmblemMarkEnabled();
	void			DestroyEmblemMap();

	// dojo	
	CNtlPLEntity*	GetDojo()							{ return m_pDojoEntity; }
	void			SetDojo(CNtlPLEntity* pDojoEntity)	{ m_pDojoEntity = pDojoEntity; }

public:
	static RpAtomic *RenderCallBack(RpAtomic *pAtomic);

public:
	//static void					SetFarPlane(RwReal fFarDist);

protected:
	//static RwPlane				g_planeFar;

#ifdef dNTL_WORLD_TOOL_MODE
public:
	static void			SaveSwapFile(RwReal x, RwReal y, RwReal z);
	static void			LoadSwapFile(RwReal x, RwReal y, RwReal z);
#endif
	
protected:
	sSCHEDULING_LM_PROP*		m_pSchedulingLMProp;
    SObjectSchedulingInfo       m_sScheduleInfo;        ///< 스케쥴링 로딩 관련 정보 
	RwUInt32					m_uiObjectSerialID;

	CNtlPLObjectType*			m_pObjectType;

	RwBool						m_bActiveUpdate;
	RwBool						m_bLoadMap;

	CNtlPLResource *			m_pClumpResource;
	ENTITY_ATOMIC_VEC	        m_vecAtomicList;         ///< 현재 Clump를 구성하고 있는 Atomic들의 리스트
	FRAME_MAP				    m_mapFrame;				 ///< Bone Info

	RwV3d						m_vModelAngle;
	RwV3d						m_vModelScale;
	RwV3d						m_vWorldPosition;

	CNtlPLObjectProperty*       m_pProperty;            ///< 오브젝트 프로퍼티 객체	

	CNtlPLUVAnim*               m_pUVAnim;              ///< UVAnim 파일 객체                
	RpHAnimHierarchy*           m_pBaseHierarchy;		///< Base Hierarchy
	CNtlPLResource*             m_pAnimResource;        ///< Anim Resource
	RwBool                      m_bAnimPlay;            ///< Anim Play 유무
	RwBool						m_bHaveAnim;			///< Anim 존재 유무.

	RwReal						m_fAnimSpeed;			///< Anim Speed

	RwRGBA						m_sColor;				///< Color				
	RwRGBA						m_sAddColor;			///< AddColor

	RwSphere					m_BSphere;				///< Position이 연산되지 않은 BoundSphere
	RwSphere					m_BSphereCur;			///< Position이 연산된 BoundingSphere : GetBoundingSphere를 호출하게 되면 업데이트 된다.

	CNtlPLLinkEffect            m_LinkEffectInstance;   ///< LinkEffect Instance들을 관리하는 객체

	RwBBox                      m_bbox;                 ///< Object의 Bounding Box

	// Fade System 관련
	EObjectFadeState            m_eFadeState;           ///< 현재 Object의 Fade State
	RwReal                      m_fPrevCameraDistance;  ///< 이전의 카메라와의 거리
	RwReal                      m_fFadeTime;            ///< Fade Time    
	CNtlPLEntityAlphaWeightBlend* m_pFadeBlend;         ///< Fade Blend 설정용 알파 블렌딩 객체

	// Trigger Object의 Animation 관련
	CNtlAnimLayer*				m_pAnimLayer;			///< 애니메이션 레이어
	CNtlInstanceAnimTable*		m_pInstanceAnimTable;	///< Animation Resource Instance Table
	RwUInt32					m_uiCurAnimKey;			///< 현재 적용되고 있는 AnimKey

	ListSoundHandle				m_listLoopSound;        ///< LoopSound들의 HANDLE List
	ListLoopEffect				m_listLoopEffect;       ///< LoopEffect의 list

	// 지형 오브젝트 그림자 관련
	vector<CNtlWorldShadow*>    m_vecNtlWorldShadow;

	RwUInt32                    m_uiMilepostTextID;      ///< 이정표로 사용될때 연결될 텍스트 테이블의 ID

    // 툰관련
    SToonData*				    m_pToonData;			///< Toon Ink, Toon Paint, Toon Resource

	// 인도어
	CNtlOBB						m_OBB;					///< 인도어에서 사용되며, RpWorldSector를 검색하기 위해 사용.
	vector<RpWorldSector*>		m_vecRpWorldSector;		///< 인도어에서 사용되며, 포함되어 있는 RpWorldSectorlist

	// Dojo
	CNtlPLEntity*				m_pDojoEntity;

	// attach light
	struct SLightObjectAttachData
	{
		RwV3d						vPos;
		RwChar						chBoneName[MAX_ATTR_BONE_NAME];
		SPLLightObjectCreateParam*	pPLLightObjectCreateParam;
	};
	std::vector<SLightObjectAttachData*> m_vecAttachLightParam;

public:
	virtual RwBool	LoadFromFile(FILE* pFile, EActiveWorldType eActiveWorldType);
	virtual RwBool	SaveIntoFile(FILE* pFile, EActiveWorldType eActiveWorldType);
	static	RwInt32	SkipToFile(FILE* pFile, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType);

	static	BYTE*	SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType);
	static	BYTE*	SaveIntoFileFromFileMemVisibilityDistance(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwReal fVD, RwReal fMinVD, RwReal fMaxVD);
	static	BYTE*	SkipToFileMem(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType);
	static	BYTE*	SkipToFileMemGetEnabledTrigger(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwBool* pResult);
	static	BYTE*	SkipToFileMemGetEnabledPEC(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwBool* pResult);
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif

#endif