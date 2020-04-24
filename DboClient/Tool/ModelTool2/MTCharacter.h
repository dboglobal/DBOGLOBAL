#pragma once

#include "MTDef.h"
#include "NtlPLCharacter.h"
#include "MTCharacterInfo.h"
#include "NtlInstanceTraceSystem.h"

class CNtlPLEntityAlphaWeightBlend;

typedef std::map<int, std::string>      BONENAME_MAP;


/*!
 * \brief
 * Model Tool Character 관련 클래스
 * 
 * CNtlPLCharacter를 상속받아서 필요한 부분을 오버라이딩하여 사용한다.
 * 
 * \remarks
 * Write remarks for CMTCharacter here.
 * 
 * \see
 * CNtlPLCharacter
 */
class CMTCharacter : public CNtlPLCharacter
{
public:

	// CNtlPLCharacter 클래스의 메모리풀에 문제가 있어서. 강제로 오버라이딩 헀다.-_-a
	void* operator new(size_t size);
	void operator delete(void *pObj);

	CMTCharacter(void);
	virtual ~CMTCharacter(void);
	
	//--- 오버라이딩
	virtual RwBool Create(const SPLEntityCreateParam *pParam  = NULL );		///< 초기화 작업을 해준다.
    virtual void   Destroy();
	virtual RwBool Update(RwReal fElapsed);
	virtual RwBool Render(void);        
    virtual RwBool	CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame) {return !IsVisible();}
    virtual RwBool	CullingTest(RwCamera* pRwCamera) {return !IsVisible();}
	//--------------------------------------------------------------------

	RwBool  LoadClump(RwChar* filename);				                	///< clump 파일을 로딩한다.	    
    RwBool  SaveClump(RwChar* fileName);                                    ///< Clump 파일을 저장한다.    	
    void    UpdateClumpInfo();                                              ///< Clump의 정보를 새로 갱신한다.
    
    void    DisplayInfo(RtCharset* pCharSet);                               ///< 화면에 정보를 표시한다.
    CMTClump* GetClumpInfo() {return &m_charInfo;}                           ///< CharInfo 객체를 반환한다.    
    ENTITY_ATOMIC_VEC   GetAtomicList() {return m_vecAtomicList;}           ///< Atomic List를 반환한다.      (Alpha Event에서 Atomic Index 순서를 맞추기 위해서 추가)

    // Clump 속성 관련
    char*   GetClumpName() {return m_strClumpPathName;};                     ///< Clump의 Name을 반환한다. (전체 경로)    
    void    SetVisible(RwBool bVisible);                                    ///< 화면에 모델의 표시 유무를 설정한다.
    void    SetAlpha(RwUInt8 byValue);                                      ///< 알파를 설정한다.
    void    SetWeightAlpha(RwReal fWeightValue);							///< Weight Alpha

    // 바운딩 박스 관련
    void    RenderAnimBBox();                                               ///< 캐릭터의 Animation BBox를 렌더링한다.
    RwBBox  CreateDefaultAnimBBox();                                        ///< 캐릭터의 기본 Animation BBox를 만든다.
    
    // WireFrame 렌더링 관련
    void              RenderWireFrame(void);                                ///< WireFrame을 렌더링 한다.    

    // Hierarchy 렌더링 관련
    void             RenderHierarchy(void);                                 ///< Hierarchy를 렌더링 한다.
    static RpAtomic* AtomicRenderSkeleton(RpAtomic *atomic, void * data);
    static RwFrame*  HierarchyRender(RwFrame *frame, void * data);
    static void      SkeletonRender(RwMatrix *LTM, RpHAnimHierarchy *hier);

    // Bone 관련
    RwInt32          GetBoneNum() {return m_nBoneCount;};                      ///< Bone의 개수를 반환한다.        
    RwBool           SetSelectBone(int nBoneIndex);                             ///< 렌더링할 Bone을 선택한다.
    void             SetRenderBone(RwBool bRender) {m_bRenderBone = bRender;}; ///< Bone의 렌더링 유무를 결정한다.
    
    // Animation 관련   
    RwBool          SetAnim(RwUInt32 uiKey);                                 ///< 애니메이션을 모델에 적용한다.    
    RwUInt32        GetCurrentAnimKey() {return m_uiCurrentAnimKey;}         ///< 현재 Play되고 있는 Anim Key를 반환한다,.

    // 스크립트 관련
    void            ResetProperty();                                        ///< 캐릭터 프로퍼티의 모든 내용을 리셋한다.
    CNtlPLCharacterProperty* GetProperty() {return m_pProperty;};           ///< 캐릭터의 프로퍼티의 포인터를 반환한다.

    // Item 관련    
    CNtlPLItem*	    SetChangeEquipItemForTool(CNtlPLItem *pItem);           ///< Item을 장착한다.
    virtual RwBool	SetRemoveEquipItem(CNtlPLItem *pItem);                  ///< Item을 해제한다.

    // Link Effect 관련
    CNtlInstanceEffect* GetLinkEffectFromName(RwChar* szName);              ///< Name에 해당하는 LinkEffect를 반환한다.

    // Hissidian 렌더링 관련
    void            SetRenderHissidian(SEventAnimHit* pEventHissidan) {m_pEventHissidianDir = pEventHissidan;}      ///< Hissidan을  렌더링하기 위한 이벤트를 설정한다.

    // Color 설정
    virtual void SetSkinColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);							///< Skin Color
    virtual void SetHeadColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);							///< Head Color

	// 모델툴에서 임시적으로 환경맵을 보여주기 위한 method
	void SetEnvMap(RwChar* _pName);

protected:
	//--- 오버라이딩	
	virtual RwBool CreateMesh(const char *szDffName);		///< Mesh를 로딩한다
    virtual RwBool SetBaseAnimation(RwUInt32 uiAnimKey, RwReal fStartTime = 0.f , RwBool bLoop = TRUE );
	//----------------------------------------------------------------------

    // Bone 관련    
    void   RenderSelectedBoneScale(RpHAnimHierarchy *pHierarchy, RwInt32 nCurrentBone);    ///< Bone을 렌더링 한다.
    
	virtual int	   CallBackBaseAnim(void* pEventData);

    // Event 처리 관련 오버라이딩
    virtual void   OnEventHit(SEventAnimHit* pEventHit);    
    virtual void   OnEventWeightTime(SEventWeightTime* pEventTime);
    virtual void   OnEventAlphaFade(SEventAlpha* pEventAlpha);    
    virtual void   OnEventPostEffect(SEventPostEffect* pEventPostEffect);
    virtual void   OnEventColorChange(SEventColorChange* pEventColorChange);                    ///< 색상 변경 이벤트 처리
    virtual void   OnEventStretch(SEventStretch* pEventStretch);                                ///< 팔 늘어나는 이벤트 처리

    void   RenderHissidainDir();                        ///< Hissidan 방향을 렌더링한다.
    void   UpdateAtomicsColor();                        ///< Color 관련
    void   UpdateStretch(RwReal fElapsed);              ///< Stretch 이벤트 관련
    void   UpdateCameraShake(RwReal fElapse);           ///< 카메라 셰이크 이벤트를 처리한다. 

protected:
	CMTClump		    m_charInfo;							///< 캐릭터 리소스 정보를 관리하는 객체	
    
    bool                m_bAnim;                            ///< 애니메이션의 적용 유무
    RtAnimAnimation*    m_pAnimation;                       ///< 캐릭터에 적용하는 애니메이션
	RwUInt32			m_uiCurBaseAnimKey;

    RwBool              m_bRemoveColorChange;               ///< 컬러변경 이벤트를 없앨지의 플래그    

    // Bone 관련    
    RwInt32             m_nCurrentSelectBoneIndex;          ///< 현재 선택된 Bone의 Index;
    RwBool              m_bRenderBone;                      ///< Bone의 렌더링 유무        
    char                m_strClumpPathName[1024];            ///< Clump의 전체 PathName
    RwUInt32            m_uiCurrentAnimKey;                  ///< 현재 Play 되고 있는 Anim Key

    SEventAnimHit*      m_pEventHissidianDir;               ///< Hissidian의 방향을 렌더링하기 위한 이벤트 포인터

    CNtlPLEntityAlphaWeightBlend* m_pAlphaBlend;              ///< 카메라 거리체크 알파 적용을 위한 객체

    // Stretch Event 관련
    RwBool              m_bStretchUpdate;                   ///< Stretch 이벤트를 위한 플래그
    RwBool              m_bStretchPulling;                  ///< Stretch 이벤트를 위한 플래그
    SEventStretch*      m_pStretchEvent;                    ///< Stretch 이벤트
    RwReal              m_fStretchPullingSpeed;             ///< Stretch 이벤트
    RwBool              m_bOrgBoneScale;                    ///< 원래의 본 스케일 플래그
    RwReal              m_fOrgBoneLength[3];                ///< 원래의 본 길이
    RwReal              m_fOrgBoneWidth[3];                 ///< 원래의 본 두께

    // 카메라 셰이크 관련
    RwReal              m_fShakeFactor;                     ///< 카메라 셰이크 팩터
    RwReal              m_fShakeMaxHeight;                  ///< 카메라 셰이크 최대값 팩터
    RwReal              m_fShakeElapsedTime;                ///< 카메라 셰이크용 ElapsedTime    
};
