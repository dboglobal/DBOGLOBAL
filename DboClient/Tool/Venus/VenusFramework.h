//***********************************************************************************
//	File		:	VenusFramework.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include "d3d9types.h"
#include "NtlApplication.h"

#include "rpworld.h"
#include "rtcharse.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLEventHandler.h"
#include "NtlPlSun.h"
#include "NtlPLEntityRenderHelpers.h"

#include ".\ntllightingcontroller.h"

/**
 * \ingroup Venus
 * \brief Venus의 Main FrameWork
 * \date 2006-07-31
 * \author agebreak
 */
class CVenusFramework : public CNtlApplication
{
public:
	CVenusFramework(void);
	virtual ~CVenusFramework(void);

	virtual void	Destroy();
	virtual RwBool	Update(RwReal fTime, RwReal fElapsedTime);
			void	OnIdle()	{ CNtlApplication::Update(); }

            void	SetSelectRectangle(POINT& OldPoint, POINT& CurPoint);  ///< 선택영역 사각형을 설정한다.
            void	CreateEffectClump(const RwChar* strPathName);          ///< Effect Clump를 생성한다.
    
            void	UpdateAmbient();                                       ///< Ambient Light를 업데이트한다.
            void	UpdateMainLight();                                     ///< Main Light를 업데이트한다.
            RpLight* CreateMainLight();                                    ///< Main Light를 생성한다.

            void	ShowTerrain(RwBool bShow);                             ///< 지형 표시 유무를 설정한다.
            void	ShowObject(RwBool bShow);                              ///< Decal Test를 위한 Object 표시 유무를 설정한다.

            /// Get/Set Method
            RwBool  GetSelectMode() {return m_bSelectMode;}                ///< Select Mode를 설정한다,.
            void    SetSelectMode(RwBool bSelectMode) {m_bSelectMode = bSelectMode;} ///< Select Mode를 반환한다.

            RwRGBA  GetBackGroundColor() {return m_BackgroundColor;}       ///< Background Color를 반환한다.
            void    SetBackGroundColor(const RwRGBA& color) {m_BackgroundColor = color;} ///< BackGround Color를 설정한다.

            void    SetFrameFix(RwBool bEnable) {m_bFrameFix = bEnable;}           ///< Frame Fix 유무를 선택한다. (현재는 30 fps로 고정한다.)                    
            RwBool  GetFrameFix() {return m_bFrameFix;}                            ///< Frame Fix 유무를 반환한다. 

			RwBool	CreateWorld();												   ///< VENUS에서 사용할 World를 생성한다.

	static	CVenusFramework&	GetInstance(void);

protected:
    virtual RwBool  AttachPlugin();                                        ///< Plugin들을 Attach 한다.
    virtual RwBool	CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool bZBuffer); ///< SubSystem을 생성한다.

    RpClump* LoadDummyClump(const RwChar* strFileName, RwV3d v, RwV3d s);  ///< Dummy Clump를 생성한다.

    void	SetPauseVertex();                                              ///< Pause시 나타나는 화면 테두리 버텍스를 설정한다.
    void	DisplayInfo();
    
    void	RenderSelectRectangle();                                       ///< 선택 영역을 화면에 렌더링 한다.
    void	AutoTempFileSave();                                            ///< 임시 파일을 자동으로 저장한다.
    void    SetErrorReport();                                              ///< BugTrap을 이용한 에러 리포트 설정을 한다.
	RwBool	VENUS_PLInit();												   ///< API_PLInit의 내용을 Venus에 맞게 수정했다.
	void	VENUS_PLTerminate();										   ///< API_PLTerminate의 내용을 Venus에 맞게 수정했다.

protected:
    RtCharset*						m_pCharset;             ///< Display 정보를 표시하기 위한 객체
    RpWorld*						m_pWorld;               ///< World 객체
	CNtlPLWorldEntity*				m_pWorldEntity;			///< World Entity 객체
    RpLight*						m_pAmbientLight;        ///< Ambient Light 객체
    RpLight*						m_pMainLight;           ///< Main Light 객체

    RpClump*						m_pEffectClump;         ///< Effect Clump
	CNtlPLEntity*					m_pDecalTestObject;		///< Decal Test용 Object 객체

    RwRGBA							m_BackgroundColor;      ///< BackGround Color
    RwRGBA							m_ForegroundColor;      ///< ForeGround Color

    RwBool							m_bSelectMode;          ///< 선택 Mode Flag
	CNtlPLEventHandler		       *m_pPLEventHandler;

    // 선택 사각형 버텍스
    enum RECTANGLE_VERTEX
    {
        RECTANGLE_VERTEX_COUNT = 5
    };
    RwIm2DVertex					m_RectangleVertex[RECTANGLE_VERTEX_COUNT];      ///< Select 사각형 버텍스

    CNtlLightingController			m_LightningController;		///< 번개 관리자.

    RwIm2DVertex					m_PauseVertex[16];          ///< Pause 버텍스

    RwReal							m_fAutoSaveTime;            ///< Auto Save될 Time 값

    RwBool                          m_bFrameFix;                ///< FrameFix 유무를 선택한다.
    RwReal                          m_fTimeFrameFix;            ///< FrameFix시 사용할 Time값을 저장하고 있는 변수
    SToonData*                      m_pToonData;                     ///< 툰이 적용된 메시를 위한 툰 정보

public:
	CNtlPLSun*						m_pSun;						///< LensFlare Test를 위한 객체

};
