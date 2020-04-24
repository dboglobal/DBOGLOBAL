#ifndef _MODEL_TOOL_APPLICATION_
#define _MODEL_TOOL_APPLICATION_

#include <rwcore.h>
#include <rpworld.h>
#include <rphanim.h>
#include <rptoon.h>
#include <rpskin.h>
#include <rpUsrdat.h>
#include <rpid.h>
#include <ddraw.h>

#include "Resource.h"
#include "MiniDump.h"
#include "NtlApplication.h"
#include "NtlPLApi.h"

#include "NtlPLVisualManager.h"
#include "MTPropertyContainer.h"
#include "NtlPLDecal.h"
#include "NtlDecalManager.h"

#include "NtlPostEffectCamera.h"
#include "ToolCamera.h"

#include "NtlPLCharacter.h"
#include "NtlPLCharacterParser.h"


#include "NtlAtomic.h"
#include "ntlworldplugin.h"

#include "MTCharacter.h"
#include "MTObject.h"
#include "MTItem.h"
#include "NtlPLMaterialExtWrapper.h"
#include "NtlPLDummyWorld.h"
#include "NtlPLCameraRenderTexture.h"

#include "NtlPLResourceManager.h"
#include "MTItemPool.h"
#include "NtlPLWorldEntity.h"

#define GET_KEY_REPEAT(lParam) (lParam & 0x0000ffff)

/*!
 * \brief
 * Model Tool의 App 클래스
 * 
 * NtlApplication 클래스에서 상속받아서 Render Ware의 Application Class가 된다.
 * 
 * \remarks
 * Write remarks for CModelToolApplication here.
 * 
 * \see
 * CNtlApplication
 */
class CModelToolApplication : public CNtlApplication
{
public:    

public:
	CModelToolApplication(void);
	virtual ~CModelToolApplication(void);

	virtual void Destroy();											///< 생성된 객체들을 파괴한다.
	RwBool	Update(RwReal fTime, RwReal fElapsedTime);				///< Update Method
	RwBool	OnIdle() {return CNtlApplication::Update();}			///< Idle Update 
    RwBool  Resize(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer); ///< 화면 크기를 조절할때 호출된다.

	// Path 관련
    char*	GetWorkDir() {return m_chCurrentPath;};					///< 현재 파일의 경로를 반환한다.
    char*   GetTexturePath();                                       ///< Texture Path를 반환한다.
	
	/// 싱글톤 호출 함수
	static	CModelToolApplication* GetInstance() {return (CModelToolApplication*)m_pInstance; }
	CMTCharacter* GetCharcter() {return m_pCharacter;};				///< 캐릭터 객체를 반환한다.

    // 플래그 관련
    void SetWorldView(RwBool bView);        ///< 지형 표현 유무를 선택한다.
    BOOL GetWorldView() {return m_bViewWorld;};                     ///< 지형 표현 유무를 반환한다.

    void SetWireView(RwBool bView) {m_bViewWire = bView;};          ///< Wireframe 표현 유무를 선택한다.
    BOOL GetWireView() {return m_bViewWire;};                       ///< Wireframe 표현 유무를 반환한다.

    void SetHierarchyView(RwBool bView) {m_bViewHierarchy = bView;}; ///< Hierarchy 표현 유무를 선택한다.
    BOOL GetHierarchyView() {return m_bViewHierarchy;};              ///< Hierarchy 표현 유무를 반환한다.

    void SetBBoxView(RwBool bView) {m_bViewBBox = bView;};          ///< 캐릭터의 BBox 표현 유무를 선택한다.
    BOOL GetBBoxView() {return m_bViewBBox;};                       ///< 캐릭터의 BBox 표현 유무를 반환한다.

    void SetToonView(RwBool bView);                                 ///< Toon 적용의 유무를 설정한다.
    BOOL GetToonView() {return m_bViewToon;}                        ///< Toon 적용 유무를 반환한다.

    void SetHitEffect(RwBool bView) {m_bHitEffect = bView;}         ///< Hit Effect 표시유무를 설정한다.
    BOOL GetHitEffect() {return m_bHitEffect;}                      ///< Hit Effect 표시유무를 반환한다.

    void    SetObjectCollisionMesh(RwBool bView) {CNtlPLGlobal::m_bCollObjVisible = bView;}    ///< Object의 Collision Mesh의 표시 유무를 설정한다.
    RwBool  GetObjectCollisionMesh() {return CNtlPLGlobal::m_bCollObjVisible;}                 ///< Objcec의 Collision Mesh의 표시 유무를 반환한다.

    void    SetEnableGlowFilter(RwBool bEnable);                    ///< Glow Filter 적용유무를 설정한다
    RwBool  GetEnableGlowFilter();                                  ///< Glow Filter 적용유무를 반환한다.

    void    SetEnableBlurFilter(RwBool bEnable);                    ///< Blur Filter 적용 유무를 설정한다.
    RwBool  GetEnableBlurFilter();                                  ///< Blur Filter 적용 유무를 반환한다.

    void    SetEnableFrameFix(RwBool bEnable);                      ///< Frame을 30프레임으로 고정시킨다.
    RwBool  GetEnableFrameFix();                                    ///< Frame Fix 유무

    void    SetEnableCameraDistAlpha(RwBool bEnable) {m_bEnableCameraDistAlpha = bEnable;}
    RwBool  GetEnableCameraDistAlpha() {return m_bEnableCameraDistAlpha;}

    void    SetCameraShakeDisable(RwBool bDisable) {m_bCameraShakeDisable = bDisable;}
    RwBool  GetCameraShakeDisable() {return m_bCameraShakeDisable;}

    void SetItemGrade(ENtlPLItemGrade eGrade);                      ///< 현재 화면에 나타나거나, 캐릭터에 설정된 Item에 Grade Effect를 부착한다.

    // 데이터 설정 관련
    void SetEditChar(CMTCharacter* pCharacter);                     ///< 현재 화면에 보이는 캐릭터를 설정한다.
    CMTCharacter* GetEditChar() {return m_pCharacter;}             ///< 현재 화면에 보이는 캐릭터를 반환한다.

    void SetEditObject(CMTObject* pObject);                          ///< 현재 화면에 나타날 오브젝트를 설정한다.
    CMTObject* GetEditObject() {return m_pObject;}                   ///< 현재 화면에 나타나는 오브젝트를 반환한다.

    void SetEditItem(CMTItem* pItem);                               ///< 현재 사용중인 아이템 객체를 설정한다.
    CMTItem* GetEditItem() {return m_pItem;}                        ///< 현재 사용중인 아이템 객체를 반환한다.

    // 현재 툴의 상태 관련
    void SetAppMode(EModelToolMode eMode) {m_ePrevAppMode = m_eAppMode; m_eAppMode = eMode;}
    EModelToolMode GetAppMode() {return m_eAppMode;}
    EModelToolMode GetAppPrevMode() {return m_ePrevAppMode;}
    RwBool IsNotChangeMode() {return m_eAppMode == m_ePrevAppMode;}
    void SetDataChanged();                                           ///< 데이터가 변경되었을때 호출한다.

    // 카메라 View 모드 관련
    CNtlGameCamera* GetCamera() {return m_pCamera;}   
    void UpdateCameraDistance(CMTClump* pMTClump);                   ///< Model의 크기에 맞게 Camera 거리를 조절한다.
    void UpdateFaceCamera(const RwV3d& vPos, const RwV3d& vLookAt);  ///< Face 카메라의 위치를 갱신한다.

	// 바이너리 컨버팅 관련
	CMTPropertyContainer* GetPropertyContainer() {return &m_PropContainer;}	///< 프로퍼티 컨테이너를 반환한다.	

	virtual LRESULT WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);		///< 윈도우 프로시져 함수

protected:
	RwBool CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer);  ///< 엔진이 시동된후에 SubSystem들을 생성한다.
    void   SetTexturePath();                                                    ///< TexturePath를 설정한다.
	RwBool AttachPlugin();														///< 필요한 PlugIn들을 Attach 한다.
    RwBool CreateWorld();                                                       ///< 배경으로 쓰일 지형을 만든다.                                                        
    void   DisplayInfo();                                                       ///< View 정보를 표시한다.    

    int		MouseDownHandler(unsigned int uiMouseData);
    int		MouseUpHandler(unsigned int uiMouseData);
    int		MouseMoveHandler(unsigned int uiMouseData);
    
    void    SetErrorReport();                                                   ///< Error Report 정보를 설정한다.

protected:
    CNtlGameCamera*	        m_pCamera;								///< Viewer에 사용될 Camera
	RtCharset*				m_pCharset;	
    
	RwRGBA					m_BackgroundColor;						///< Viewer의 Background Color
    RwRGBA					m_ForegorundColor;						///< Viewer의 Foreground Color

	CNtlPLVisualManager*	m_pVisualManager;						///< Visual Manager 
	CNtlPLEventHandler*     m_pPLEventHandler;
	
	RpLight*				m_pLtAmbient;							///< Ambient Light
	RpLight*				m_pLtDirectional;						///< Directional Light 

	CMTPropertyContainer	m_PropContainer;						///< 프로퍼티 Container 싱글톤 객체
	CNtlPLResourceManager   m_ResoureManager;						///< Resource Manager 싱글톤 객체
	
	CMTCharacter*			m_pCharacter;							///< 캐릭터 객체    
    CMTObject*              m_pObject;                              ///< 화면에 나타날 오브젝트 객체
    CMTItem*                m_pItem;                                ///< 현재 사용중인 Item객체

    // ItemPool
    CMTItemPool*            m_pItemPool;                            ///< Item Pool
	
	char					m_chCurrentPath[256];					///< 현재 디렉토리를 저장할 변수
    RwChar                  m_strTexturePath[1024];                 ///< Texture Folders Path

	DWORD					m_dwPrevLMouseDownTime;
	DWORD					m_dwPrevRMouseDownTime;

	CNtlPLWorldEntity*		m_pWorldEntity;							///< World Entity 객체    

    // View 관련 Flag
    RwBool                  m_bViewWorld;                           ///< 지형 렌더링 유무
    RwBool                  m_bViewWire;                            ///< WireFrame 표현 유무
    RwBool                  m_bViewHierarchy;                       ///< Hierarchy 표현 유무
    RwBool                  m_bViewBBox;                            ///< 캐릭터의 BBox의 표현 유무
    RwBool                  m_bViewToon;                            ///< Toon 적용 유무
    RwBool                  m_bHitEffect;                           ///< HitEffect 적용 유무        
    RwBool                  m_bFrameFix;                            ///< Frame Fix 유무 (30프레임으로 고정 시킨다.)
    RwBool                  m_bEnableCameraDistAlpha;               ///< 카메라 거리에 따른 알파 처리 적용 유무 플래그
    RwBool                  m_bCameraShakeDisable;                  ///< 카메라 셰이크때 모델툴 뷰로 Shake 하는지 플래그

    // 메모리 내역 관련
    LPDIRECTDRAW7           m_lpDD;                                 ///< 메모리를 표시하기 위한 DirectX7 객체

    EModelToolMode          m_eAppMode;                             ///< 현재 모델툴의 상태 플래그
    EModelToolMode          m_ePrevAppMode;                         ///< 모델툴의 이전 상태 플래그

    // Input 관련
    INPUT_HANDLE	m_handleMouseMove;    
    RwV2d           m_vOldMousePos;

    // Face Camera
    CNtlPLCameraRenderTexture m_faceCamera;                         ///< Face Camera 객체
    RwV2d                   m_v2FaceCameraPos;                      ///< Face Camera Pos
    RwV2d                   m_v2FaceCameraSize;                     ///< Face Camera Size

};

#endif