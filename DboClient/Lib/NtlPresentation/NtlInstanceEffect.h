//***********************************************************************************
//	File		:	NtlInstanceEffect.h
//	Desc		:	
//	Begin		:	2005. 7.28
//                  2006. 8.17
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//                  HoDong
//	Update		:	
//***********************************************************************************
#ifndef __NTL_INSTANCE_EFFECT__
#define __NTL_INSTANCE_EFFECT__

#include "rwcore.h"
#include "NtlPLAttach.h"
#include "NtlEffectDefine.h"
#include "NtlResourceEffect.h"
#include "NtlInstanceComponentSystem.h"
#include "NtlPLResourceManager.h"
#include "NtlPLSound.h"

/**
 * \ingroup Effect
 * Effect Entity Class(NtlInstanceComponentSystem들의 Container Class 이다.)
 * 
 */
class CNtlInstanceEffect : public CNtlPLAttach
{
public:
	typedef std::vector<CNtlInstanceComponentSystem*>	svdef_InstanceComponentSystem;
	svdef_InstanceComponentSystem						m_svSystem;

	CNtlResourceEffect* m_pResourceEffect;

	RwMatrix			m_matWorld;						///< Effect World Matrix (m_vModelScale * m_vModelAngle * m_vWorldPosition)
														///< SetMatrix()로 Matrix 값을 직접 입력시 m_vWorldPosition 값만 혼용해서 쓸수가 있다.
														///< Rotation과 Scale은 Matrix에 입력된 값과 충돌이 생길 수도 있다.

	RwV3d				m_vModelScale;					///< Model Scale
	RwV3d				m_vModelAngle;					///< Model Angle (Degree - 360 - 360)
	RwV3d				m_vWorldPosition;				///< World Position    

	RwReal				m_fLifeTime;
	RwBool				m_bReady;
	RwBool				m_bShow;
	RwBool				m_bAutoDelete;
    RwBool              m_bApplyScale;

    // Sound 관련 인스턴스
    CNtlPLSound         m_PLSound;                      ///< Sound 객체   

protected:
    RwReal						m_fAnimSpeed;			///< Anim Speed
    CNtlPLResource *			m_pVisibleBVClump;		///< Bounding Volume Clump //Tool에서 Picking을 하기 위한 용도로만	
    RwSphere                    m_BoundingSphere;       ///< Bounding Sphere
    RwBool                      m_bIsCulling;           ///< 바운딩 스피어에 의해서 컬링되었는지 유무 플래그
    RwReal                      m_fNotUpdateTime;       ///< 컬링되거나 Not Visible되어서 업데이트 안된 시간    
    
    static  RwBool              m_bLowSpec;             ///< 이펙트 로우 스펙 적용 유무 플래그
    static  RwReal              m_fLowSpecRatio;        ///< 이펙트 로우 스펙용 비율
    static  RwBool              m_bRenderMeshSystem;    ///< 메시 시스템 렌더링 On/Off 플래그
    static  RwBool              m_bRenderDecalSystem;   ///< 데칼 시스템 렌더링 On/Off 플래그

public:
	CNtlInstanceEffect(void);
	~CNtlInstanceEffect(void);

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual RwBool	Create(const SPLEntityCreateParam *pParam);
	virtual void	Destroy();
    virtual void    Finish();                                   ///< 이펙트를 바로 종료하지 않고, 종료작업을 호출할때 사용하는 메소드 	    
	virtual RwBool	Update(RwReal fElapsedTime);
	virtual RwBool	Render();
	virtual RwBool  RenderToTexture(void);
    virtual RwBool	SetProperty(const CNtlPLProperty *pData);
    virtual void    SetAttachParent(CNtlPLAttach *pPLAttachEntity);

    void    AddComponentSystem(CNtlInstanceComponentSystem* pComponentSystem);

	virtual RwBool IsAutoDelete()	{ return m_bAutoDelete; }						///< 자동으로 삭제가 되는 Effect 인지 아닌지
	
	void	SetMatrix(RwMatrix& matWorld);											///< Effect의 World Matrix를 설정한다.
	RwMatrix &GetMatrix(void);														///< Effect의 World Matrix를 얻는다.
	
	void	SetPosition(const RwV3d *pPos);											///< Effect를 World의 위치로 이동 시킨다.
	RwV3d	GetPosition(void);														///< Effect의 World의 위치를 얻는다.
	
	void	SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle);				///< Effect의 Model 공간 회전값(degree)
	const	RwV3d* GetRotate(void) const;											///< Effect의 Model 공간 회전값 얻기

	void	SetScale( RwReal fScale );												///< Effect의 Model 공간 Scale값
	RwReal	GetScale(void);															///< Effect의 Model 공간 Scale값 얻어오기
    void    SetApplyScale(RwBool bApply);                                           ///< 스케일 적용 유무를 설정한다.

    inline RwSphere* GetBoundingSphere();                                           ///< 이펙트의 바운딩 스피어를 반환한다. (바운딩 스피어가 없으면 NULL을 반환)
    void			 RenderBoundingSphere();                                        ///< Renders the shape of the bounding sphere. (Venus only)

    virtual void	SetAlpha(RwUInt8 byValue);                                      ///< Effect의 Alpha값을 설정한다.
    virtual void	SetVisible(RwBool bVisible);                                    ///< Effect의 Visible 유무를 설정한다
    virtual RwBool	IsVisible(RwUInt32 uiFlags = 0xFFFFFFFF);                       ///< Effect의 Visible 유무를 반환한다.

	RwBool			SetPlayAnimSpeed(RwReal fAnimSpeed);									///< Animaiton Speed (1.f: Default )
	void			SetVisibleBoundingVolume(RwBool bVisible);								///< World Editor용 Picking Bounding Volume
    void			SetPostEffect_CenterFixEnable(RwBool bEnable);                          ///< 포함된 Post Effect의 Center Fix기능을 설정한다.
    void			SetCulling(RwBool bCulling) {m_bIsCulling = bCulling;}                  ///< 강제로 컬링 유무를 설정한다.
    RwBool			IsCulling() {return m_bIsCulling;}                                      ///< 컬링 유무 플래그를 반환한다.
    RwUInt32        GetMemoryUseSize();                                                     ///< 이펙트의 메모리 사용량을 반환한다.
    
#ifndef dNTL_WORLD_CULLING_NEW
	virtual RwBool  CullingTest(RwCamera* pCamera);                                         ///< 컬링 유무를 검사한다.
#endif

    //-- Flag settings for Low Spec
    static void     SetLowSpecEnable(RwBool bEnable) {m_bLowSpec = bEnable;}        ///< Set whether low specification is applied or not.
    static RwBool   GetLowSpecEnable() {return m_bLowSpec;}
    static void     SetLowSpecRatio(RwReal fRatio) {m_fLowSpecRatio = fRatio;}      ///< Apply the low spec ratio.   
    static RwReal   GetLowSpecRatio() {return m_fLowSpecRatio;}
    static void     SetRenderMeshSystem(RwBool bEnable) {m_bRenderMeshSystem = bEnable;}    ///< Mesh System Rendering On / Off
    static RwBool   GetRenderMeshSystem() {return m_bRenderMeshSystem;}
    static void     SetRenderDecalSystem(RwBool bEnable) {m_bRenderDecalSystem = bEnable;}  ///< Decal System Rendering On / Off
    static RwBool   GetRenderDecalSystem() {return m_bRenderDecalSystem;}

protected:
    void	SetTransform();
    void	CreateSystem(CNtlResourceEffect* pResourceEffect, const RwMatrix& matRender);
    void    CalcBoundingSphere();                                                   ///< Calculate the bounding sphere.

#ifdef dNTL_WORLD_TOOL_MODE
public:
	static void			SaveSwapFile(RwReal x, RwReal y, RwReal z);
	static void			LoadSwapFile(RwReal x, RwReal y, RwReal z);
#endif

public:
	virtual RwBool	LoadFromFile(FILE* pFile);
	virtual RwBool	SaveIntoFile(FILE* pFile);
	static	RwInt32 SkipToFile(FILE* pFile);

	static	BYTE*	SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	static	BYTE*	SkipToFileMem(BYTE* pFileMem);

};

RwSphere* CNtlInstanceEffect::GetBoundingSphere() 
{
    if(m_BoundingSphere.radius != 0.0f)
        return &m_BoundingSphere;

    return NULL;
}

#endif
