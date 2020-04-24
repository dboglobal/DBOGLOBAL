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
#include "NtlPLPropertyContainer.h"

#include "NtlPostEffectCamera.h"
#include "ToolCamera.h"

#include "NtlPLCharacter.h"
#include "NtlPLCharacterParser.h"


#include "NtlAtomic.h"
#include "ntlworldplugin.h"

#include "RpClump.h"
#include "MTCharacter.h"
#include "NtlToonMaterial.h"

#include "NtlPLResourceManager.h"


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
    enum EModelToolMode
    {
        MODE_CLUMP,
        MODE_PC,
        MODE_MOB,
        MODE_ITEM,
        MODE_OBJECT,
    };

public:
	CModelToolApplication(void);
	virtual ~CModelToolApplication(void);

	virtual void Destroy();											///< 생성된 객체들을 파괴한다.
	RwBool	Update(RwReal fTime, RwReal fElapsedTime);				///< Update Method
	RwBool	OnIdle() {return CNtlApplication::Update();}			///< Idle Update 
    RwBool  Resize(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer); ///< 화면 크기를 조절할때 호출된다.

	RwBool	LoadClump(RwChar * filename );							///< Clump 파일을 Loading한다.
	char*	GetWorkDir() {return m_chCurrentPath;};					///< 현재 파일의 경로를 반환한다.
	
	/// 싱글톤 호출 함수
	static	CModelToolApplication* GetInstance() {return (CModelToolApplication*)m_pInstance; }
	CMTCharacter* GetCharcter() {return m_pCharacter;};				///< 캐릭터 객체를 반환한다.

    // 플래그 관련
    void SetWorldView(RwBool bView) {m_bViewWorld = bView;};        ///< 지형 표현 유무를 선택한다.
    BOOL GetWorldView() {return m_bViewWorld;};                     ///< 지형 표현 유무를 반환한다.

    void SetWireView(RwBool bView) {m_bViewWire = bView;};          ///< Wireframe 표현 유무를 선택한다.
    BOOL GetWireView() {return m_bViewWire;};                       ///< Wireframe 표현 유무를 반환한다.

    void SetHierarchyView(RwBool bView) {m_bViewHierarchy = bView;}; ///< Hierarchy 표현 유무를 선택한다.
    BOOL GetHierarchyView() {return m_bViewHierarchy;};              ///< Hierarchy 표현 유무를 반환한다.

    void SetEditChar(CMTCharacter* pCharacter) {m_pAnimEditChar = pCharacter;}; 

    // 현재 툴의 상태 관련
    void SetAppMode(EModelToolMode eMode) {m_eAppMode = eMode;}
    EModelToolMode GetAppMode() {return m_eAppMode;}

	virtual LRESULT WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);		///< 윈도우 프로시져 함수

protected:
	RwBool CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer);  ///< 엔진이 시동된후에 SubSystem들을 생성한다.
	RwBool AttachPlugin();														///< 필요한 PlugIn들을 Attach 한다.
    RwBool CreateWorld();                                                       ///< 배경으로 쓰일 지형을 만든다.                                                    
    void   DisplayInfo();                                                       ///< View 정보를 표시한다.
    void   DisplayHitTime();                                                    ///< Hit Time을 화면에 표시한다

protected:
    CNtlGameCamera*	        m_pCamera;								///< Viewer에 사용될 Camera
	RtCharset*				m_pCharset;	
    RtCharset*              m_pCharsetHitTime;                      ///< HitTime 표시용 CharSet
	RwRGBA					m_BackgroundColor;						///< Viewer의 Background Color
    RwRGBA					m_ForegorundColor;						///< Viewer의 Foreground Color

	CNtlPLVisualManager*	m_pVisualManager;						///< Visual Manager (?)	

	RpWorld*				m_pWorld;								///< World 객체
	RpLight*				m_pLtAmbient;							///< Ambient Light
	RpLight*				m_pLtDirectional;						///< Directional Light 

	CNtlPLPropertyContainer	m_PropContainer;						///< 프로퍼티 Container 싱글톤 객체
	CNtlPLResourceManager   m_ResoureManager;						///< Resource Manager 싱글톤 객체

	
	CMTCharacter*			m_pCharacter;							///< 캐릭터 객체
    CMTCharacter*           m_pAnimEditChar;                        ///< Anim Edit를 위한 Char 인스턴스
	
	char					m_chCurrentPath[256];					///< 현재 디렉토리를 저장할 변수

	DWORD					m_dwPrevLMouseDownTime;
	DWORD					m_dwPrevRMouseDownTime;

    CNtlPLResource*         m_pResWorld;                            ///< 지형 리소스
    CNtlPLResource*         m_pResSky;                              ///< 하늘 리소스    

    // View 관련 Flag
    RwBool                  m_bViewWorld;                           ///< 지형 렌더링 유무
    RwBool                  m_bViewWire;                            ///< WireFrame 표현 유무
    RwBool                  m_bViewHierarchy;                       ///< Hierarchy 표현 유무

    // 메모리 내역 관련
    LPDIRECTDRAW7           m_lpDD;                                 ///< 메모리를 표시하기 위한 DirectX7 객체

    EModelToolMode          m_eAppMode;                             ///< 현재 모델툴의 상태 플래그

   
};

#endif