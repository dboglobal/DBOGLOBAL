#include "StdAfx.h"

#include "NtlDebug.h"
#include "NtlText.h"

#include "NtlPLSceneManagerFactory.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLApi.h"
#include "InputHandler.h"
#include "NtlPLCharacterParser.h"
#include "ModelToolApplication.h"
#include "NtlWorldMaterialPlugin.h"

#include "ClumpPane.h"
#include "MaterialPane.h"
#include "TreePCPane.h"
#include "AnimSetPane.h"
#include "AnimPlayPane.h"
#include "BoneEditPane.h"
#include "BBoxPane.h"
#include "LinkEffectPane.h"
#include "TreeMob.h"
#include "PropertyPane.h"
#include "TreeObjectPane.h"
#include "TreeItemPane.h"
#include "ItemViewPane.h"
#include "TreeNPCPane.h"
#include "TreeVehiclePane.h"
#include "NtlPLHelpers.h"
#include "NaviMeshPane.h"
#include "VehicleViewPane.h"

#include "NtlBugTrap.h"

#include "MTCharacterInfo.h" 
#include "NtlDefaultItemData.h"
#include "NtlSoundManager.h"
#include "NtlPLResourcePack.h"


CModelToolApplication::CModelToolApplication(void)
{
    m_pCamera	  	 = NULL;
    m_pCharset		 = NULL;    
    m_pVisualManager = NULL;	
    m_pLtAmbient	 = NULL;
    m_pLtDirectional = NULL;			
    m_pCharacter	 = NULL;        
    m_lpDD           = NULL;
    m_pObject        = NULL;    
    m_pItem          = NULL;    
    m_pItemPool      = NULL;
	m_pWorldEntity	 = NULL;
	m_pPLEventHandler = NULL;

    
    // Obstacle Mesh가 배경 색상과 같아서 안보이는 경우가 있다.
    // 배경 색상 설정 기능을 넣기 귀찮아서, 걍 랜덤으로 정해버린다. (그래픽팀과 협의 끝)
    srand( (unsigned)time( NULL ) );
    m_BackgroundColor.red	= (RwUInt8)NtlRandomNumber(0, 255);
    m_BackgroundColor.green = (RwUInt8)NtlRandomNumber(0, 255);
    m_BackgroundColor.blue	= (RwUInt8)NtlRandomNumber(0, 255);
    m_BackgroundColor.alpha = 255;

    m_ForegorundColor.red	= 0;
    m_ForegorundColor.green	= 0;
    m_ForegorundColor.blue	= 0;
    m_ForegorundColor.alpha	= 255;

    m_dwPrevLMouseDownTime  = 0;
    m_dwPrevRMouseDownTime	= 0;

    m_bViewWorld     = TRUE;
    m_bViewToon      = TRUE;
    m_bViewWire      = FALSE;
    m_bViewHierarchy = FALSE;
    m_bViewBBox      = FALSE;
    m_bHitEffect     = TRUE;
    m_bFrameFix      = FALSE;
    m_bEnableCameraDistAlpha = FALSE;
    m_bCameraShakeDisable = TRUE;

    GetCurrentDirectoryA(256, m_chCurrentPath);

    m_handleMouseMove  = CInputHandler::GetInstance()->LinkMouseMove(this, &CModelToolApplication::MouseMoveHandler);    

    m_v2FaceCameraPos.x = 0.0f;
    m_v2FaceCameraPos.y = 0.0f;
    m_v2FaceCameraSize.x = 128.0f;
    m_v2FaceCameraSize.y = 128.0f;    

    m_eAppMode = m_ePrevAppMode = MT_MODE_NONE;
}

CModelToolApplication::~CModelToolApplication(void)
{
    CInputHandler::GetInstance()->UnLinkMouseMove(m_handleMouseMove);
}

void CModelToolApplication::SetErrorReport() 
{
    // 버그트랩용 설정
    BT_SetAppName(MT_WINDOW_NAME);     
    BT_SetSupportServer(_T("10.0.0.73"), 9999);
}

RwBool CModelToolApplication::CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
{
    NTL_FUNCTION(__FUNCTION__);

	CNtlApplication::CreateSubSystem(iWidth, iHeight, zBuffer);

    // BugTrap
    SetErrorReport();


	// Open the event handler module	
	RWS::CEventHandler::Open();
	RWS::CEventHandler::RegisterStreamChunkHandlers();

	API_CoreInit();
    API_PLInit();		// API_PLInit;

	GetNtlResourcePackManager()->LoadPackHeader();
	GetNtlResourcePackManager()->SetActiveTypeFlags(NTL_PACK_TYPE_FLAG_TEXTURE | NTL_PACK_TYPE_FLAG_OBJECT |
		NTL_PACK_TYPE_FLAG_TERRAIN | NTL_PACK_TYPE_FLAG_GUI |
		NTL_PACK_TYPE_FLAG_FLASH | NTL_PACK_TYPE_FLAG_LANGUAGE |
	NTL_PACK_TYPE_FLAG_TABLE | NTL_PACK_TYPE_FLAG_SCRIPT | NTL_PACK_TYPE_FLAG_PROPERTY);


	m_pPLEventHandler = NTL_NEW CNtlPLEventHandler;
	if (!m_pPLEventHandler->Create())
	{
		NTL_ASSERTFAIL("CNtlPLEventHandler fail");
		NTL_RETURN(FALSE);
	}

	LinkTextureLoad(ResourcPackTextureLoad);
	//RwBool bLoadPropertyContainer = CNtlPLPropertyContainer::GetInstance()->Load();
	RwBool bLoadPropertyContainer = bLoadPropertyContainer = CNtlPLPropertyContainer::GetInstance()->LoadSerialize();

	if (!bLoadPropertyContainer)
	{
		NTL_ASSERTFAIL("Property container load fail");
		NTL_RETURN(FALSE);
	}


    // Camera 생성
    m_pCamera = NTL_NEW CNtlGameCamera();

    if(!m_pCamera->Create(iWidth, iHeight, zBuffer, 1.0f, 500.0f))
    {
		NTL_ASSERTFAIL("Create camera fail");
        NTL_RETURN(FALSE);
    }

    // Active Camera로 세팅한다.
    CNtlPLGlobal::SetActiveCamera(m_pCamera->GetCamera());

    RwRGBA colorTextForeGround = {255, 255, 255, 255};
    RwRGBA colorTextBackGround = {0, 0, 0, 0};

    m_pCharset = RtCharsetCreate(&colorTextForeGround, &colorTextBackGround);
	if (!m_pCharset)
	{
		NTL_ASSERTFAIL("RtCharsetCreate fail");
		NTL_RETURN(FALSE);
	}

    // Visual Manager를 생성한다.
    m_pVisualManager = NTL_NEW CNtlPLVisualManager();
    if(!m_pVisualManager->Create())
    {
		NTL_ASSERTFAIL("CNtlPLVisualManager fail");
        NTL_RETURN(FALSE);
    }

    // Active Scene Manager를 setting 한다.
    CNtlPLSceneManagerFactory::ActiveSceneManager(m_pVisualManager);

    // Property Container를 생성하고, 데이터를 Load 한다.	
   /* if(!m_PropContainer.Load())
    {
        NTL_ASSERTFAIL("Property Container Load Fail!");
        NTL_RETURN(FALSE);
    }*/

	// Create a background.
	CreateWorld();

    // dummy world.
    //m_pDummyWorld = (CNtlPLDummyWorld*)m_pVisualManager->CreateEntity(PLENTITY_DUMMY_WORLD, "NULL", NULL);

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

    // 메모리 사용내용을 구하기 위해서 DirectX7 개체를 생성한다.
    DirectDrawCreateEx(NULL, (void**)&m_lpDD, IID_IDirectDraw7, NULL);

    // Texture Path를 설정한다
    SetTexturePath();

    // Toon Data를 생성한다
    CMTClump::CreateCartoon();    

    // Sound Manager를 생성한다.
    GetSoundManager()->Init(".\\Sound\\");
    GetSoundManager()->SetListenerPosition(0.0f, 0.0f, 0.0f);

    // Face Camera를 생성한다.
    m_faceCamera.Create(FACE_CAMERA_SIZE, FACE_CAMERA_SIZE, 128, 128);
    
    // Item Pool을 생성한다.
    m_pItemPool = new CMTItemPool();
    
    // 스케쥴 로딩(멀티 스레드)를 변경한다.
    GetNtlResourceManager()->SetLoadScheduling(FALSE);

    NTL_RETURN(TRUE);
}

void CModelToolApplication::Destroy()
{
    NTL_FUNCTION(__FUNCTION__);
    
    m_faceCamera.Destroy();

    CMTClump::DestroyCartoon();

    // Sound Manager
    GetSoundManager()->Release();

    if(m_lpDD)
        m_lpDD->Release();

    NTL_DELETE(m_pItemPool);

    // Camera Destroy
    if(m_pCamera)
    {
        m_pCamera->Destroy();
        NTL_DELETE(m_pCamera);        
    }

    RwFrame* frame = RpLightGetFrame(m_pLtDirectional);

    RpLightSetFrame(m_pLtDirectional, NULL);
    RwFrameDestroy(frame);

    RpWorldRemoveLight(CNtlPLGlobal::m_pRpWorld, m_pLtDirectional);
    RpWorldRemoveLight(CNtlPLGlobal::m_pRpWorld, m_pLtAmbient);
    RpLightDestroy(m_pLtDirectional);
    RpLightDestroy(m_pLtAmbient);


	if(m_pWorldEntity)
	{
		GetSceneManager()->DeleteEntity(m_pWorldEntity);
		m_pWorldEntity = NULL;
	}

    m_ResoureManager.Destroy();   


    // visual Manager Destroy
    if(m_pVisualManager)
    {
        m_pVisualManager->Destroy();
        NTL_DELETE(m_pVisualManager);        
    }

    m_PropContainer.Destroy();
    

    // Character set Destroy
    if(m_pCharset)
    {
        RtCharsetClose();
        RwRasterDestroy(m_pCharset);        
        m_pCharset = NULL;        
    }

    API_PLTerminate();
	
	if(m_pPLEventHandler)
	{
		m_pPLEventHandler->Destroy();
		NTL_DELETE(m_pPLEventHandler);
	}

	API_CoreTerminate();

	// Close the event System
	RWS::CEventHandler::UnRegisterStreamChunkHandlers();
	RWS::CEventHandler::Close();

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

    if (!NtlMatExtPluginAttach())
        NTL_RETURN(FALSE);

    if( !RpNtlWorldAttach() )
        NTL_RETURN(FALSE);

    if( !RpNtlWorldMaterialPluginAttach() )
        NTL_RETURN(FALSE);

    /* 
    * Attach Normal Maps plug-in...
    */
    if( !RpNormMapPluginAttach() )
    {
        return FALSE;
    }


    NTL_RETURN(TRUE);
}

RwBool CModelToolApplication::Resize(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
{
    NTL_FUNCTION("CDboCharacterApplication::Resize" );

    CNtlApplication::Resize(iWidth, iHeight, zBuffer,FALSE);

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
				m_pCamera->SetCameraSize(&rt);                
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
    // Clump 정보 표시
    GetSafeInstance(CClumpPane)->DisplayInfo(m_pCharset);

    RwChar caption[256] = {0,};

    // FPS
    if(m_bFrameFix)
    {
        RsSprintf(caption, RWSTRING("FPS : 30.0"));
    }
    else
    {
        RsSprintf(caption, RWSTRING("FPS : %4.2f"), GetFps());
    }
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

    // 카메라와 현재 모델 위치(0,0,0)과의 거리를 표시한다.
    if(m_pCamera)
    {
        RwFrame* pFrame = RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
        RwV3d vPosCamera = *RwMatrixGetPos(RwFrameGetMatrix(pFrame));
        RwV3d vDist = vPosCamera - ZeroAxis;
        RwReal fDist = RwV3dLength(&vDist);
        RsSprintf(caption, RWSTRING("Camera Distance : %.2f"), fDist);
        RsCharsetPrint(m_pCharset, caption, 0, 1, rsPRINTPOSBOTTOMLEFT);
    }    
}

RwBool CModelToolApplication::Update(RwReal fTime, RwReal fElapsedTime)
{
    NTL_FUNCTION(__FUNCTION__);

    // Frame 고정을 적용한다.
    static RwReal fTimeFrameFix = 0.0f;

    if(m_bFrameFix)
    {
        fTimeFrameFix += fElapsedTime;

        if(fTimeFrameFix < (1.0f / FRAME_FIX_FPS))
        {
            return FALSE;
        }
        else
        {
            fElapsedTime += fTimeFrameFix;
            fTimeFrameFix = 0.0f;
        }
    }

	// input process
	API_CoreUpdate(fElapsedTime);

	// World Update
	if(m_pWorldEntity)
	{
		RwReal fHeight = 0.0f;
		GetSceneManager()->GetWorldHeight(&ZeroAxis, fHeight, NULL);
		RwV3d vPos;
		vPos.x = 0.0f; vPos.y = fHeight; vPos.z = 0.0f;
		m_pWorldEntity->SetPlayerPosition(vPos);
	}


	if (RwCameraClear(m_pCamera->GetCamera(), &m_BackgroundColor, rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
	{
		if(m_pCamera)
			m_pCamera->Update(fElapsedTime);

		//Ready to Render Scene
		if(RwCameraBeginUpdate(m_pCamera->GetCamera()))
		{
			if(m_pVisualManager)
			{
				m_pVisualManager->Update(fElapsedTime);
				m_pVisualManager->Render();            
			}

			if(m_eAppMode == MT_MODE_OBJECT)
			{
				if(m_pObject)
				{
					m_pObject->Update(fElapsedTime);                

					m_pObject->UpdateFadeSystem();

					// Atomic BB 
					GetSafeInstance(CClumpPane)->Render();

					// Render BBox
					if(m_bViewBBox)
					{
						m_pObject->RenderBBox();
					}

					if(m_bViewWire)
					{
						m_pObject->RenderWireFrame();
					}

					GetSafeInstance(CNaviMeshPane)->Render();
				}            
			}       
			else if(m_eAppMode == MT_MODE_ITEM)
			{
				// Atomic BB 
				GetSafeInstance(CClumpPane)->Render();
			}
			else if(m_eAppMode == MT_MODE_PC || m_eAppMode == MT_MODE_MOB || m_eAppMode == MT_MODE_NPC || m_eAppMode == MT_MODE_VEHICLE)
			{

				if(m_pCharacter)
				{
					// SlowTime Effect를 위해서 Time을 조절한다.
					RwReal fWeightElapsed = fElapsedTime;

					if(m_pCharacter->GetFlags() & NTL_PLEFLAG_WEIGHT_ELAPSED_TIME)
					{
						fWeightElapsed = m_pCharacter->GetWeightElapsedTime() * fElapsedTime;
					}

					m_pCharacter->Update(fWeightElapsed);
					m_pCharacter->Render();

					// Wireframe 표시
					if(m_bViewWire)
					{
						m_pCharacter->RenderWireFrame();
					}

					// Hierarchy 표시
					if(m_bViewHierarchy)
					{
						m_pCharacter->RenderHierarchy();            
					}

					// 캐릭터 BBox 표시
					if(m_bViewBBox)
					{
						m_pCharacter->RenderAnimBBox();
					}

					// Atomic BB 
					GetSafeInstance(CClumpPane)->Render();

					if(m_eAppMode == MT_MODE_PC)
					{
						// Face Camera 렌더링
						m_faceCamera.Render(m_pCamera->GetCamera(), m_v2FaceCameraPos, m_v2FaceCameraSize);
					}                

					m_pCharacter->DisplayInfo(m_pCharset);                
				}
			}

			// Render Complete     
			RwCameraEndUpdate(m_pCamera->GetCamera());
		}  

		// Camera Filter Update    
		m_pCamera->UpdateBlurFilter();
		m_pCamera->UpdateMotionBlur(fElapsedTime);

		if(m_pCamera->MainCameraBeginUpdate(fElapsedTime))
		{
			// 필터 효과가 안먹는 것은 여기에서 그린다.

			// 화면에 정보 표시        
			DisplayInfo();

			m_pCamera->MainCameraEndUpdate();
		}

		m_pCamera->CameraShowRaster(GetHWnd(), 0);
	}


    // Face Camera Render
    if(m_eAppMode == MT_MODE_PC)
    {
        if(m_pCharacter && m_pCharacter->GetProperty())
        {
            // Face Camera Update
            m_faceCamera.RenderTexture(m_pCharacter);
        }
    }

    // 애니메이션 툴뷰 업데이트
    GetSafeInstance(CAnimPlayPane)->Update();    

    NTL_RETURN(TRUE);
}

LRESULT CModelToolApplication::WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_KEYDOWN:
        if(wParam == VK_SPACE)
        {
            GetSafeInstance(CAnimPlayPane)->ChangeAnimPlay();
            //m_pMovie->Restart();
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
        SetCapture(window);
        CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_RBTN, false); 
        m_dwPrevRMouseDownTime = timeGetTime();
        break;
    case WM_RBUTTONUP:
        CInputHandler::GetInstance()->MouseUpHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_RBTN, timeGetTime() - m_dwPrevRMouseDownTime);
        ReleaseCapture();
        break;
    case WM_LBUTTONDBLCLK:
        CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_LBTN, true); 
        break;
    case WM_RBUTTONDBLCLK:
        CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_RBTN, true); 
        break;
    case WM_MBUTTONDOWN:
        SetCapture(window);
        CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_MBTN, false);         
        m_dwPrevRMouseDownTime = timeGetTime();
        break;
    case WM_MBUTTONUP:
        CInputHandler::GetInstance()->MouseUpHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_MBTN, timeGetTime() - m_dwPrevRMouseDownTime);
        ReleaseCapture();
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

    // Bone Edit를 위한 부분
    MSG msg;
    msg.hwnd = window;
    msg.message = message;
    msg.lParam = lParam;
    msg.wParam = wParam;

    GetSafeInstance(CBoneEditPane)->PreTranslateMessage(&msg);

    return TRUE;
}

RwBool CModelToolApplication::CreateWorld()
{
	// set texture folder path
	_getcwd(dGET_WORLD_PARAM()->CurWorkingFolderName, 256);
	strcpy(dGET_WORLD_PARAM()->CurDefaultTexPath, dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcat(dGET_WORLD_PARAM()->CurDefaultTexPath, "\\texture\\ntlwe\\");

	// set mesh folder path	
	strcpy(dGET_WORLD_PARAM()->WorldProjectFolderName, dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcat(dGET_WORLD_PARAM()->WorldProjectFolderName, "\\world\\dbo_select_character\\");
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);


	// create a global world param
	if(FILE* pFile = fopen("#######.gwp", "rb"))
	{
		LoadFileWorldState(dGET_WORLD_PARAM(), pFile);
		fclose(pFile);

		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
		{
		}
		else
		{
			char acTemp[256];
			sprintf_s(acTemp, 256, "unsupported version : world(%s), tool(%s)", dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION);
			::AfxMessageBox(acTemp, MB_OK);
		}
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	m_pWorldEntity = static_cast<CNtlPLWorldEntity*>(GetSceneManager()->CreateEntity(PLENTITY_WORLD, "NtlWorld"));	
	if(!m_pWorldEntity)
		return FALSE;

    RwV3d vWorldPos = ZeroAxis;
	m_pWorldEntity->CreateWorld(vWorldPos);
	
	//m_pWorld = m_pWorldEntity->GetWorld();	
	GetSceneManager()->GetFog()->Switch();

	return TRUE;	
}

void CModelToolApplication::SetEditChar(CMTCharacter* pCharacter)
{
    if(m_pCharacter && m_pCharacter != pCharacter)
    {
        m_pCharacter->SetVisible(FALSE);
    }

    if(pCharacter)
    {
        pCharacter->SetVisible(TRUE);
        if(m_pObject)
        {
            m_pObject->SetVisible(FALSE);
        }

        if(m_pItem)
        {
            m_pItem->SetVisible(FALSE);
        }
    }

    m_pCharacter = pCharacter;

    switch(m_eAppMode)
    {
    case MT_MODE_CLUMP:      
        if(pCharacter)
        {
            GetSafeInstance(CMaterialPane)->SetClump(pCharacter->GetClumpInfo());
        }
        else
        {
            GetSafeInstance(CMaterialPane)->SetClump(NULL);
        }

        GetSafeInstance(CTreePCPane)->SetModel(NULL);        
        GetSafeInstance(CTreeMobPane)->SetModel(NULL);
        GetSafeInstance(CTreeNPCPane)->SetModel(NULL);
        GetSafeInstance(CAnimSetPane)->SetModel(NULL);
        GetSafeInstance(CBoneEditPane)->SetModel(NULL);
        GetSafeInstance(CBBoxPane)->SetModel(NULL);
        GetSafeInstance(CAnimPlayPane)->SetAnimData(NULL, NULL);
        GetSafeInstance(CPropertyPane)->SetEvent(NULL, NULL);
        GetSafeInstance(CTreeObjectPane)->SetObject(NULL);
        GetSafeInstance(CTreeItemPane)->SetItem(NULL);
        GetSafeInstance(CLinkEffectPane)->SetCharacter(NULL);
        GetSafeInstance(CItemViewPane)->SetCharacter(NULL);
        GetSafeInstance(CNaviMeshPane)->SetObject(NULL);
        break;
    case MT_MODE_VEHICLE:
        GetSafeInstance(CVehicleViewPane)->SetVehicle(pCharacter);
    case MT_MODE_PC:
    case MT_MODE_MOB:
    case MT_MODE_NPC:    
        if(pCharacter)
        {
            GetSafeInstance(CClumpPane)->SetClump(pCharacter->GetClumpInfo());
            GetSafeInstance(CMaterialPane)->SetClump(pCharacter->GetClumpInfo());            
        }
        else
        {
            GetSafeInstance(CClumpPane)->SetClump(NULL);
            GetSafeInstance(CMaterialPane)->SetClump(NULL);            
        }
        GetSafeInstance(CAnimSetPane)->SetModel(pCharacter);
        GetSafeInstance(CBoneEditPane)->SetModel(pCharacter);
        GetSafeInstance(CBBoxPane)->SetModel(pCharacter);    
        GetSafeInstance(CPropertyPane)->SetEvent(NULL);
        GetSafeInstance(CPropertyPane)->SetCharacter(pCharacter);
        GetSafeInstance(CTreeObjectPane)->SetObject(NULL);
        GetSafeInstance(CTreeItemPane)->SetItem(NULL);
        GetSafeInstance(CLinkEffectPane)->SetCharacter(pCharacter);
        GetSafeInstance(CItemViewPane)->SetCharacter(pCharacter);
        GetSafeInstance(CNaviMeshPane)->SetObject(NULL);
        break;    
    }

    // Camera Distance
    if(pCharacter)
    {
        UpdateCameraDistance(pCharacter->GetClumpInfo());


        // Face Camera Update
        if(pCharacter->GetProperty())
        {
            UpdateFaceCamera(pCharacter->GetProperty()->GetFaceCameraPos(),
                pCharacter->GetProperty()->GetFaceCameraLookAt());
        }
    }
}

void CModelToolApplication::SetEditObject(CMTObject* pObject)
{

    if(m_pObject && m_pObject != pObject)
    {
        m_pObject->SetVisible(FALSE);
    }

    if(pObject)
    {
        pObject->SetVisible(TRUE);

        if(m_pCharacter)
        {
            m_pCharacter->SetVisible(FALSE);
        }

        if(m_pItem)
        {
            m_pItem->SetVisible(FALSE);
        }
    }

    m_pObject = pObject;

    switch(m_eAppMode)
    {
    case MT_MODE_OBJECT:
        if(pObject)
        {
            GetSafeInstance(CClumpPane)->SetClump(pObject->GetClumpInfo());
            GetSafeInstance(CMaterialPane)->SetClump(pObject->GetClumpInfo());            
        }
        else
        {
            GetSafeInstance(CClumpPane)->SetClump(NULL);
            GetSafeInstance(CMaterialPane)->SetClump(NULL);
        }

        GetSafeInstance(CTreePCPane)->SetModel(NULL);        
        GetSafeInstance(CTreeMobPane)->SetModel(NULL);
        GetSafeInstance(CTreeItemPane)->SetItem(NULL);
        GetSafeInstance(CAnimSetPane)->SetModel(pObject);
        GetSafeInstance(CBoneEditPane)->SetModel(NULL);
        GetSafeInstance(CBBoxPane)->SetModel(NULL);
        GetSafeInstance(CAnimPlayPane)->SetAnimData(NULL, NULL);
        GetSafeInstance(CPropertyPane)->SetEvent(NULL);
		GetSafeInstance(CPropertyPane)->SetObject(pObject);
        GetSafeInstance(CLinkEffectPane)->SetObject(pObject);
        GetSafeInstance(CItemViewPane)->SetCharacter(NULL);
        GetSafeInstance(CNaviMeshPane)->SetObject(pObject);

        break;
    }

    // Camera Distance
    if(pObject)
    {
        UpdateCameraDistance(pObject->GetClumpInfo());
    }
}

void CModelToolApplication::SetEditItem(CMTItem* pItem)
{
    if(m_pItem && m_pItem != pItem)
    {
        m_pItem->SetVisible(FALSE);      
    }

    m_pItem = pItem;

    switch(m_eAppMode)
    {
    case MT_MODE_ITEM:
        if(pItem)
        {
            GetSafeInstance(CClumpPane)->SetClump(pItem->GetClumpInfo());
            GetSafeInstance(CMaterialPane)->SetClump(pItem->GetClumpInfo());
        }
        else
        {
            GetSafeInstance(CClumpPane)->SetClump(NULL);
            GetSafeInstance(CMaterialPane)->SetClump(NULL);
        }

        GetSafeInstance(CTreePCPane)->SetModel(NULL);        
        GetSafeInstance(CTreeMobPane)->SetModel(NULL);
        GetSafeInstance(CTreeObjectPane)->SetObject(NULL);
        GetSafeInstance(CAnimSetPane)->SetModel(pItem);
        GetSafeInstance(CBoneEditPane)->SetModel(NULL);
        GetSafeInstance(CBBoxPane)->SetModel(NULL);
        GetSafeInstance(CAnimPlayPane)->SetAnimData(NULL, NULL);
        GetSafeInstance(CPropertyPane)->SetEvent(NULL);
        GetSafeInstance(CPropertyPane)->SetItem(pItem);
        GetSafeInstance(CLinkEffectPane)->SetItem(pItem);
        GetSafeInstance(CItemViewPane)->SetCharacter(NULL);
        GetSafeInstance(CNaviMeshPane)->SetObject(NULL);
        break;
    }

    if(pItem)
    {
        if(m_pCharacter)
        {
            m_pCharacter->SetVisible(FALSE);
            m_pCharacter = NULL;
        }

        if(m_pObject)
        {
            m_pObject->SetVisible(FALSE);
            m_pObject = NULL;
        }

        pItem->SetVisible(TRUE);        
    }


    // Camera Distance
    if(pItem)
    {
        UpdateCameraDistance(pItem->GetClumpInfo());
    }    
}

void CModelToolApplication::SetDataChanged()
{
    switch(m_eAppMode)
    {
    case MT_MODE_PC:
        if(!m_pCharacter)
            return;
        GetSafeInstance(CTreePCPane)->OnDataChanged();        
        break;
	case MT_MODE_NPC:
		if(!m_pCharacter)
			return;
		GetSafeInstance(CTreeNPCPane)->OnDataChanged();
		break;
    case MT_MODE_MOB:
        if(!m_pCharacter)
            return;
        GetSafeInstance(CTreeMobPane)->OnDataChanged();
        break;
    case MT_MODE_ITEM:
        if(!m_pItem)
            return;
        GetSafeInstance(CTreeItemPane)->OnDataChanged();
        break;
    case MT_MODE_OBJECT:
        if(!m_pObject)
            return;
        GetSafeInstance(CTreeObjectPane)->OnDataChanged();
        break;
    case MT_MODE_VEHICLE:
        if(!m_pCharacter) return;
        GetSafeInstance(CTreeVehiclePane)->OnDataChanged();
        break;
    }
}

int CModelToolApplication::MouseMoveHandler(unsigned int uiMouseData)
{
    SMouseData *pData = (SMouseData *)uiMouseData;
    if(pData->uiFlags & MK_RBUTTON)
    {
        if(m_eAppMode == MT_MODE_PC || m_eAppMode == MT_MODE_NPC || m_eAppMode == MT_MODE_MOB || m_eAppMode == MT_MODE_VEHICLE)
        {
			if(!m_pCharacter)
				return TRUE;

            RwReal fOffset = 5.0f;
            if(pData->iPosX > m_vOldMousePos.x) 
            {
                RwReal fAngleY = m_pCharacter->GetAngleY();
                fAngleY += fOffset;
                m_pCharacter->SetAngleY(fAngleY);
            }
            else if(pData->iPosX < m_vOldMousePos.x)
            {
                RwReal fAngleY = m_pCharacter->GetAngleY();
                fAngleY -= fOffset;
                m_pCharacter->SetAngleY(fAngleY);

            }

            m_vOldMousePos.x = (RwReal)pData->iPosX;
            m_vOldMousePos.y = (RwReal)pData->iPosY;
        }
		else if(m_eAppMode == MT_MODE_OBJECT)
		{
			if(!m_pObject)
				return TRUE;

			RwV3d vRot = *m_pObject->GetRotate();
			if(pData->iPosX > m_vOldMousePos.x)
			{
				vRot.y += 8.0f;
				m_pObject->SetRotate(vRot.x, vRot.y, vRot.z);
			}
			else if(pData->iPosX < m_vOldMousePos.x)
			{
				vRot.y -= 8.0f;
				m_pObject->SetRotate(vRot.x, vRot.y, vRot.z);
			}
			m_vOldMousePos.x = (RwReal)pData->iPosX;
			m_vOldMousePos.y = (RwReal)pData->iPosY;
		}
    }
    return TRUE;
}

int CModelToolApplication::MouseDownHandler( unsigned int uiMouseData ) 
{
	return TRUE;
}	

int CModelToolApplication::MouseUpHandler( unsigned int uiMouseData ) 
{
	return TRUE;
}

static RpAtomic *SilhouetteOnAtomicCallBack(RpAtomic *atomic, void *data __RWUNUSED__)
{
    RpToonGeo* toonGeo = RpToonGeometryGetToonGeo(RpAtomicGetGeometry(atomic));
    if(toonGeo)
    {
        RpToonAtomicEnable(atomic);
    }

    return atomic;
}

static RpAtomic *SilhouetteAtomicSetRenderCallBack(RpAtomic *atomic, void *data)
{
    RpAtomicCallBackRender renderCallback = (RpAtomicCallBackRender)data;

    RpAtomicSetRenderCallBack(atomic, renderCallback);

    return atomic;
}

static RpClump *SilhouetteOnClumpCallBack(RpClump *clump, void *data __RWUNUSED__)
{
    RpClumpForAllAtomics(clump, SilhouetteOnAtomicCallBack, NULL);

    return clump;
}

static RpClump *SilhouetteClumpSetRenderCallBack(RpClump *clump, void *data)
{
    RpClumpForAllAtomics(clump, SilhouetteAtomicSetRenderCallBack, data);

    return clump;
}

void CModelToolApplication::SetToonView(RwBool bView)
{
    if(m_bViewToon == bView || !CNtlPLGlobal::m_pRpWorld)
        return;

    m_bViewToon = bView;

    switch(m_eAppMode)
    {
    case MT_MODE_PC:                
    case MT_MODE_MOB:
    case MT_MODE_NPC:
    case MT_MODE_VEHICLE:
        if(m_pCharacter)
        {
            m_pCharacter->GetClumpInfo()->SetViewEdge(bView);
        }
        break;
    case MT_MODE_OBJECT:
        break;
    case MT_MODE_CLUMP:
        break;
    }
}

void CModelToolApplication::SetTexturePath() 
{
    ZeroMemory(m_strTexturePath, sizeof(m_strTexturePath));

    strcat_s(m_strTexturePath, ".\\Texture\\Character\\;");
    strcat_s(m_strTexturePath, ".\\Texture\\Object\\;");
    strcat_s(m_strTexturePath, ".\\Texture\\Item\\;");
    strcat_s(m_strTexturePath, ".\\tool\\ToolShareData\\NaviTexture\\;");

}

char* CModelToolApplication::GetTexturePath() 
{
    ::SetCurrentDirectoryA(m_chCurrentPath);

    return m_strTexturePath;
}

void CModelToolApplication::UpdateCameraDistance(CMTClump* pMTClump)
{
    if(!pMTClump || !m_pCamera)
        return;

    RwReal fDistance = __max(pMTClump->GetMaxWidthHeight() * 2.0f, 2.5f);
    m_pCamera->SetDistance(fDistance);    
}

void CModelToolApplication::UpdateFaceCamera(const RwV3d& vPos, const RwV3d& vLookAt)
{
    m_faceCamera.SetCameraPosition(&vPos, &vLookAt);

}

// File loader callback.
//GFile* GFxLoader_FileOpenCallback(const char* ppathOrUrl)
//{
//    return new GSysFile(ppathOrUrl);
//}




void CModelToolApplication::SetItemGrade( ENtlPLItemGrade eGrade ) 
{
    if(m_eAppMode == MT_MODE_ITEM)
    {
        if(m_pItem)
        {
            m_pItem->SetUpgradeEffect(eGrade);
        }
    }
    else if(m_eAppMode == MT_MODE_PC)
    {
        if(m_pCharacter)
        {
            m_pCharacter->SetItemGradeEffect(eGrade);
        }
    }
}
void CModelToolApplication::SetEnableGlowFilter( RwBool bEnable ) 
{
    if(!m_pCamera)
        return;

    if(bEnable)
    {   
        m_pCamera->SetPostEffectFilters(POST_EFFECT_FILTER_HDR);
    }
    else
    {
        m_pCamera->SetPostEffectFilters(POST_EFFECT_FILTER_NONE);
        
    }
}

RwBool CModelToolApplication::GetEnableGlowFilter() 
{
    if(!m_pCamera)
        return FALSE;

    RwInt8 byCameraFilter = m_pCamera->GetPostEffectFilters();
    if(byCameraFilter == POST_EFFECT_FILTER_HDR)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void CModelToolApplication::SetEnableBlurFilter( RwBool bEnable ) 
{
    if(!m_pCamera)
        return;

    m_pCamera->SetEnableBlur(bEnable);
}

RwBool CModelToolApplication::GetEnableBlurFilter() 
{
    if(!m_pCamera)
        return FALSE;

    return m_pCamera->GetEnableBlur();   
}

void CModelToolApplication::SetEnableFrameFix( RwBool bEnable ) 
{
    m_bFrameFix = bEnable;
}

RwBool CModelToolApplication::GetEnableFrameFix() 
{
    return m_bFrameFix;
}

void CModelToolApplication::SetWorldView( RwBool bView ) 
{
	m_bViewWorld = bView;

	// 지형과 스카이를 같이 없앤다.
	CNtlPLGlobal::m_bWorldTerrainVisible = bView;
	CNtlPLGlobal::m_bWorldSkyVisible = bView;
}

