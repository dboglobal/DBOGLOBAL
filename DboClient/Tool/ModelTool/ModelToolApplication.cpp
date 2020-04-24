#include "StdAfx.h"
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <zmouse.h>
#include "NtlDebug.h"
#include "NtlText.h"

#include "NtlPLSceneManagerFactory.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLApi.h"
#include "InputHandler.h"
#include "NtlPLCharacterParser.h"
#include "ModelToolApplication.h"
#include "NtlWorldMaterialPlugin.h"

#include <rtintsec.h> 
#include <rpcollis.h>
#include <rtquat.h> 
#include <rtanim.h>
#include <rphanim.h>
#include <rptoon.h>
#include <rpskin.h>
#include <rpusrdat.h>
#include <rpid.h>
#include <rpmatfx.h>


#include <mmsystem.h>

#include "AnimToolView.h"

CModelToolApplication::CModelToolApplication(void)
{
	m_pCamera	  	 = NULL;
	m_pCharset		 = NULL;
    m_pCharsetHitTime = NULL;
	m_pVisualManager = NULL;
	m_pWorld		 = NULL;
	m_pLtAmbient	 = NULL;
	m_pLtDirectional = NULL;			
	m_pCharacter	 = NULL;    
    m_pResWorld      = NULL;    
    m_pResSky        = NULL;    
    m_pAnimEditChar  = NULL;
    m_lpDD           = NULL;
    

	m_BackgroundColor.red	= 0;
	m_BackgroundColor.green = 0;
	m_BackgroundColor.blue	= 255;
	m_BackgroundColor.alpha = 255;

	m_ForegorundColor.red	= 0;
	m_ForegorundColor.green	= 0;
	m_ForegorundColor.blue	= 0;
	m_ForegorundColor.alpha	= 255;

	m_dwPrevLMouseDownTime  = 0;
	m_dwPrevRMouseDownTime	= 0;

    m_bViewWorld     = TRUE;
    m_bViewWire      = FALSE;
    m_bViewHierarchy = FALSE;
}

CModelToolApplication::~CModelToolApplication(void)
{
}

RwBool CModelToolApplication::CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
{
	GetCurrentDirectoryA(256, m_chCurrentPath);

	NTL_FUNCTION(__FUNCTION__);
	
	CNtlApplication::CreateSubSystem(iWidth, iHeight, zBuffer);

	API_PLInit();		// API_PLInit;

	// Open the event handler module
	RWS::CEventHandler::Open();
	RWS::CEventHandler::RegisterStreamChunkHandlers();

	m_pCamera = NTL_NEW CNtlGameCamera();
    
	
	if(!m_pCamera->Create(iWidth, iHeight, zBuffer, 1.0f, 500.0f))
	{
		NTL_RETURN(FALSE);
	}
    
	// Active Camera로 세팅한다.
	CNtlPLGlobal::SetActiveCamera(m_pCamera->GetCamera());

    RwRGBA colorTextForeGround = {255, 255, 255, 255};
    RwRGBA colorTextBackGround = {0, 0, 0, 0};
	m_pCharset = RtCharsetCreate(&colorTextForeGround, &colorTextBackGround);
	if(!m_pCharset)
		NTL_RETURN(FALSE);

    // Hit Time 표시용 Charset을 생성한다.
    RwRGBA colorHitForeGround = {255, 255, 255, 255};
    RwRGBA colorHitBackGround = {255, 0, 0, 255};
    m_pCharsetHitTime = RtCharsetCreate(&colorHitForeGround, &colorHitBackGround);

	// Visual Manager를 생성한다.
	m_pVisualManager = NTL_NEW CNtlPLVisualManager();
	if(!m_pVisualManager->Create())
	{
		NTL_RETURN(FALSE);
	}

	// Active Scene Manager를 setting 한다.
	CNtlPLSceneManagerFactory::ActiveSceneManager(m_pVisualManager);

	// Property Container를 생성하고, 데이터를 Load 한다.	
	//if(!m_PropContainer.Load(""))
	//{
	//	NTL_ASSERTFAIL("Property Container Load Fail!");
	//	NTL_RETURN(FALSE);
	//}

	// RpWorld를 Create 한다.
	RwBBox bb;
	bb.inf.x = -1000.0f; bb.inf.y = -1000.0f; bb.inf.z = -1000.0f;
	bb.sup.x = bb.sup.y = bb.sup.z = 1000.0f;
	m_pWorld = RpWorldCreate(&bb);
	CNtlPLGlobal::SetActiveWorld(m_pWorld);

	// RpWorld에 Camea를 Add한다.
	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

	// Ambient Light Create
	m_pLtAmbient = RpLightCreate(rpLIGHTAMBIENT);
	RwRGBAReal color = {0.9f, 0.9f, 0.9f, 1.0f};
	RpLightSetColor(m_pLtAmbient, &color);
	RpWorldAddLight(CNtlPLGlobal::m_pRpWorld, m_pLtAmbient);

	// Directional Light Create
	m_pLtDirectional = RpLightCreate(rpLIGHTDIRECTIONAL);
	if(m_pLtDirectional)
	{
		RwFrame* pFrame = NULL;		// Directional Light를 붙일 Frame
		RpLightSetColor(m_pLtDirectional, &color);

		pFrame = RwFrameCreate();
		if(pFrame)
		{
			RwFrameRotate(pFrame, &CNtlPLGlobal::m_vXAxisV3, 135.0f, rwCOMBINEREPLACE);
			RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, -45.0f, rwCOMBINEPOSTCONCAT);

			RpLightSetFrame(m_pLtDirectional, pFrame);
			RpWorldAddLight(CNtlPLGlobal::m_pRpWorld, m_pLtDirectional);
		}
	}

	m_ResoureManager.Crate();	
	m_pCharacter = new CMTCharacter;
	m_pCharacter->Create();

    // 배경을 생성한다.
    CreateWorld();

    // 메모리 사용내용을 구하기 위해서 DirectX7 개체를 생성한다.
    DirectDrawCreateEx(NULL, (void**)&m_lpDD, IID_IDirectDraw7, NULL);
    
	NTL_RETURN(TRUE);
}

void CModelToolApplication::Destroy()
{
	NTL_FUNCTION(__FUNCTION__);

    if(m_lpDD)
        m_lpDD->Release();

    // World Destroy
    if(m_pResWorld && m_pResWorld->GetClump())
    {
        CNtlPLResourceManager::GetInstance()->UnLoad(m_pResWorld);
        m_pResWorld = NULL;
    }

    if(m_pResSky && m_pResSky->GetClump())
    {
        CNtlPLResourceManager::GetInstance()->UnLoad(m_pResSky);
        m_pResSky = NULL;
    }


	if(m_pCharacter)
	{
		m_pCharacter->Destroy();
		NTL_DELETE(m_pCharacter);
		m_pCharacter = 0;
	}

	m_ResoureManager.Destroy();

	// Light destroy (todo. Light도 Class Wrapping하면 안될까?)
	RpWorldRemoveLight(CNtlPLGlobal::m_pRpWorld, m_pLtAmbient);
	RpWorldRemoveLight(CNtlPLGlobal::m_pRpWorld, m_pLtDirectional);

	RwFrame* frame = RpLightGetFrame(m_pLtDirectional);

	RpLightSetFrame(m_pLtDirectional, NULL);
	RwFrameDestroy(frame);

	RpLightDestroy(m_pLtDirectional);
	RpLightDestroy(m_pLtAmbient);

	m_PropContainer.Destroy();

	// visual Manager Destroy
	if(m_pVisualManager)
	{
		m_pVisualManager->Destroy();
		NTL_DELETE(m_pVisualManager);
		m_pVisualManager = NULL;
	}

	// Character set Destroy
	if(m_pCharset)
	{
		RtCharsetClose();
		RwRasterDestroy(m_pCharset);
        RwRasterDestroy(m_pCharsetHitTime);
		m_pCharset = NULL;
        m_pCharsetHitTime = NULL;
	}

	// Camera Destroy
	if(m_pCamera)
	{
		m_pCamera->Destroy();
		NTL_DELETE(m_pCamera);
		m_pCamera = NULL;
	}

	// World Destroy
	RpWorldDestroy(CNtlPLGlobal::m_pRpWorld);

	// Close the event System
	RWS::CEventHandler::UnRegisterStreamChunkHandlers();
	RWS::CEventHandler::Close();

	API_PLTerminate();

	CNtlApplication::Destroy();
	
	NTL_RETURNVOID();
}

RwBool CModelToolApplication::AttachPlugin()
{
	NTL_FUNCTION(__FUNCTION__);

	if( !RpWorldPluginAttach() )
		NTL_RETURN(FALSE);
	
	if( !RpCollisionPluginAttach() )
		NTL_RETURN(FALSE);
	
	if( !RtAnimInitialize() )
		NTL_RETURN(FALSE);
	
	if( !RpHAnimPluginAttach() )
		NTL_RETURN(FALSE);
	
	if( !RpSkinPluginAttach() )
		NTL_RETURN(FALSE);

	if ( !RpToonPluginAttach() )
		NTL_RETURN(FALSE);

	if( !RpUserDataPluginAttach() )
		NTL_RETURN(FALSE);

	if( !RpIdPluginAttach() )
		NTL_RETURN(FALSE);
	
	if( !RpMatFXPluginAttach() )
		NTL_RETURN(FALSE);

	if (!RpUVAnimPluginAttach())
		NTL_RETURN(FALSE);

	if(!RpPTankPluginAttach())
		NTL_RETURN(FALSE);
	
	if (!RpPrtStdPluginAttach())
		NTL_RETURN(FALSE);
	
	if (!RpPrtAdvPluginAttach())
		NTL_RETURN(FALSE);
	
	if (!RpNtlAtomicPluginAttach())
		NTL_RETURN(FALSE);

	if (!RpNtlToonMaterialPluginAttach())
		NTL_RETURN(FALSE);

	// woody1019
	if( !RpNtlWorldAttach() )
		NTL_RETURN(FALSE);

	if( !RpNtlWorldMaterialPluginAttach() )
		NTL_RETURN(FALSE);

	NTL_RETURN(TRUE);
}

RwBool CModelToolApplication::Resize(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
{
    NTL_FUNCTION("CDboCharacterApplication::Resize" );

    CNtlApplication::Resize(iWidth, iHeight, zBuffer);

    if(m_pCamera != NULL)
    {
        if (m_pCamera->GetCamera())
        {
            if (iWidth == 0 || iHeight == 0)
            {
            }
            else
            {
                RwRect rt;
                rt.x = 0;
                rt.y = 0;
                rt.w = iWidth;
                rt.h = iHeight;
                m_pCamera->SetCameraResize(m_pCamera->GetCamera(), &rt);
            }
            NTL_RETURN(TRUE);
        }
        else
        {
            NTL_RETURN(FALSE);
        }
    }

    NTL_RETURN(TRUE);
}

void CModelToolApplication::DisplayInfo()
{
    RwChar caption[256] = {0,};

    // FPS
    RsSprintf(caption, RWSTRING("FPS : %4.2f"), GetFps());
    RsCharsetPrint(m_pCharset, caption, 0, 0, rsPRINTPOSTOPRIGHT); 

    // 비디오 메모리를 구한다.    
    DDSCAPS2      ddsCaps2; 
    DWORD         dwTotal; 
    DWORD         dwFree;
    HRESULT       hr; 

    ZeroMemory(&ddsCaps2, sizeof(ddsCaps2));

    ddsCaps2.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM; 
    hr = m_lpDD->GetAvailableVidMem(&ddsCaps2, &dwTotal, &dwFree); 
    if (FAILED(hr))
        return;

    float fTotalVideoMem = (float)dwTotal / (1024.0f * 1024.0f);
    float fUseVideoMem = (float)(dwTotal - dwFree) / (1024.0f * 1024.0f);

    // 시스템 메모리를 구한다.
    MEMORYSTATUS MemStatus;
    MemStatus.dwLength=sizeof(MemStatus);
    GlobalMemoryStatus(&MemStatus);

    float fTotalLocalMem = (float)MemStatus.dwTotalPhys / (1024.0f * 1024.0f);
    float fUseLocalMem = (float)MemStatus.dwAvailPhys / (1024.0f * 1024.0f);

    RsSprintf(caption, RWSTRING("VIDEO MEM : %.2f/%.0f"), fUseVideoMem, fTotalVideoMem);
    RsCharsetPrint(m_pCharset, caption, 0 , 1, rsPRINTPOSTOPRIGHT);
    RsSprintf(caption, RWSTRING("LOCAL MEM : %.2f/%.0f"), fUseLocalMem, fTotalLocalMem);
    RsCharsetPrint(m_pCharset, caption, 0, 2, rsPRINTPOSTOPRIGHT);
}

void CModelToolApplication::DisplayHitTime()
{
    // 설정된 HitTime시에 HitTime을 화면에 표시한다.

    if(!m_pAnimEditChar || !m_pAnimEditChar->GetProperty())
        return;
    
    //RwUInt32 uiAniKey = m_pAnimEditChar->GetCurrentAnimKey();
    //if(uiAniKey == 0)
    //    return;

    //STypeAnimData* pAnimData = m_pAnimEditChar->GetProperty()->GetAnimTable()->Get(uiAniKey);
    //if(!pAnimData)
    //    return;

    //// 애니메이션의 현재 시간
    //RwReal fCurrentAnimTime = m_pAnimEditChar->GetBaseCurrentAnimTime();
    //RwBool bVisible = FALSE;

    //for(size_t i = 0; i < pAnimData->vecAnimEvent.size(); ++i)
    //{
    //   SEventAnim* pEventAnim = pAnimData->vecAnimEvent[i];
    //   if(pEventAnim)
    //   {
    //       if(pEventAnim->eEventID == EVENT_ANIM_HIT && pEventAnim->fTime != 0.00f)
    //       {
    //           // 이벤트 타임에서 0.5초 동안 표시한다.
    //           if(fCurrentAnimTime >= pEventAnim->fTime && fCurrentAnimTime < (pEventAnim->fTime + 0.5f))
    //           {
    //                bVisible = TRUE;
    //                break;
    //           }
    //       }
    //   }
    //}

    //if(bVisible)
    //{
    //    RwChar caption[256] = {0,};
    //    RsSprintf(caption, RWSTRING("HIT!"));
    //    RsCharsetPrint(m_pCharsetHitTime, caption, 0, 0, rsPRINTPOSBOTTOMRIGHT);
    //}
    //else
    //{

    //}
}

RwBool CModelToolApplication::Update(RwReal fTime, RwReal fElapsedTime)
{
	NTL_FUNCTION(__FUNCTION__);

	RwCameraClear(m_pCamera->GetCamera(), &m_BackgroundColor, rwCAMERACLEARZ | rwCAMERACLEARIMAGE);

	if(m_pCamera)
		m_pCamera->Update(fElapsedTime);

	// Ready to Render Scene
	if(RwCameraBeginUpdate(m_pCamera->GetCamera()))
	{
		if(m_pVisualManager)
		{
			m_pVisualManager->Update(fElapsedTime);
			m_pVisualManager->Render();            
		}

		//if(m_pCharacter)
		//{
  //          m_pCharacter->Update(fElapsedTime);
		//	m_pCharacter->Render();
		//}

        if(m_pAnimEditChar)
        {
            m_pAnimEditChar->Update(fElapsedTime);
            m_pAnimEditChar->Render();
        }


        // Wireframe 표시
        if(m_bViewWire)
        {
            if(m_pAnimEditChar)
                m_pAnimEditChar->RenderWireFrame();
        }

        // World 표시
        if(m_bViewWorld)
        {
            if(m_pResWorld)
                RpClumpRender(m_pResWorld->GetClump());
            if(m_pResSky)
                RpClumpRender(m_pResSky->GetClump());
        }

        // Hierarchy 표시
        if(m_bViewHierarchy)
        {
            if(m_pCharacter)
                m_pCharacter->RenderHierarchy();
            if(m_pAnimEditChar)
                m_pAnimEditChar->RenderHierarchy();
        }
        
        // 화면에 정보 표시
        DisplayInfo();
        if(m_pAnimEditChar)
        {
            m_pAnimEditChar->DisplayInfo(m_pCharset);
            DisplayHitTime();
        }        

        // Render Complete     
		RwCameraEndUpdate(RwCameraGetCurrentCamera());        
	}

	RwCameraShowRaster(m_pCamera->GetCamera(), GetHWnd(), 0);
    
    // 애니메이션 툴뷰 업데이트
    CAnimToolView::GetInstance()->Update();

	NTL_RETURN(TRUE);
}

RwBool CModelToolApplication::LoadClump(RwChar * filename )
{
	NTL_PRE(filename);
	NTL_PRE(m_pCharacter);
	NTL_PRE(m_pWorld);

	RwBool retBool = m_pCharacter->LoadClump(filename);
	if(!retBool)	
	{
		MessageBox(NULL, L"File Open Error", L"MODEL TOOL", MB_OK);
		return retBool;
	}
	
	return retBool;
}

LRESULT CModelToolApplication::WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_KEYDOWN:
        if(wParam == VK_SPACE)
        {
            CAnimToolView::GetInstance()->ChangeAnimPlay();
        }
        
		CInputHandler::GetInstance()->KeyDownHandler((unsigned int)wParam,  GET_KEY_REPEAT(lParam));        
        break;
	case WM_KEYUP:
		CInputHandler::GetInstance()->KeyUpHandler((unsigned int)wParam,  GET_KEY_REPEAT(lParam));
		break;
	case WM_LBUTTONDOWN:
		SetCapture(window);
		CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_LBTN, false ); 
		m_dwPrevLMouseDownTime = timeGetTime();
		break;
	case WM_LBUTTONUP:
		CInputHandler::GetInstance()->MouseUpHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_LBTN, timeGetTime() - m_dwPrevLMouseDownTime);
		ReleaseCapture();
		break;
	case WM_RBUTTONDOWN:
		CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_RBTN, false); 
		m_dwPrevRMouseDownTime = timeGetTime();
		break;
	case WM_RBUTTONUP:
		CInputHandler::GetInstance()->MouseUpHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_RBTN, timeGetTime() - m_dwPrevRMouseDownTime);
		break;
	case WM_LBUTTONDBLCLK:
		CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_LBTN, true); 
		break;
	case WM_RBUTTONDBLCLK:
		CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_RBTN, true); 
		break;
	case WM_MOUSEWHEEL:
		CInputHandler::GetInstance()->MouseWheelHandler(LOWORD(wParam), HIWORD(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MOUSEMOVE:
		CInputHandler::GetInstance()->MouseMoveHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;   

	default:
		break;
	}

	return TRUE;
}

RwBool CModelToolApplication::CreateWorld()
{
    std::string strWorldFileName = m_chCurrentPath;
    strWorldFileName += "\\Tool\\ModelTool\\modeltool_field.dff";
    std::string strSkyFileName = m_chCurrentPath;
    strSkyFileName += "\\Tool\\ModelTool\\sky.dff";
    std::string strTexPath = m_chCurrentPath;
    strTexPath += "\\Tool\\ModelTool\\";

    m_pResWorld = CNtlPLResourceManager::GetInstance()->LoadClump(strWorldFileName.c_str(), strTexPath.c_str());
    if(!m_pResWorld)
        return FALSE;

    //todo. 테스트용 임시 수치
    //RwV3d vTrans;
    //vTrans.x = 305.0f;
    //vTrans.y = 26.4f;
    //vTrans.z = 50.0f;    

    //RwFrame* pFrame = RpClumpGetFrame(m_pResWorld->GetClump());    
    //RwFrameTranslate(pFrame, &vTrans, rwCOMBINEREPLACE);

    m_pResSky   = CNtlPLResourceManager::GetInstance()->LoadClump(strSkyFileName.c_str(), strTexPath.c_str());
    if(!m_pResSky)
        return FALSE;

    return TRUE;
}

