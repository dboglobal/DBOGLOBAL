#include "StdAfx.h"

#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <zmouse.h>
#include "DboApplication.h"
#include "NtlDebug.h"
#include "NtlText.h"

#include "NtlPLSceneManagerFactory.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLApi.h"
#include "InputHandler.h"
#include ".\dbocharacterapplication.h"
#include "NtlPLCharacterParser.h"


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

CDboCharacterApplication::CDboCharacterApplication()
{
	m_BackgroundColor.red   = 0;
	m_BackgroundColor.green = 0;
	m_BackgroundColor.blue  = 255;
	m_BackgroundColor.alpha = 255;

	m_ForegroundColor.red   = 0;
	m_ForegroundColor.green = 0;
	m_ForegroundColor.blue  = 0;
	m_ForegroundColor.alpha = 255;
	
	m_pVisualManager	= NULL;
	m_pCamera			= NULL;

	m_pPLCharacter      = NULL;
}

CDboCharacterApplication::~CDboCharacterApplication()
{

}

RwBool CDboCharacterApplication::CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
{
	NTL_FUNCTION("CDboCharacterApplication::CreateSubSystem" );

	CNtlApplication::CreateSubSystem(iWidth, iHeight, zBuffer);
	
	// API_PLInit
	API_PLInit();
	
	// Open the event handler module
	RWS::CEventHandler::Open ();
    RWS::CEventHandler::RegisterStreamChunkHandlers();
  
	m_pCamera = NTL_NEW CNtlGameCamera;

	if(!m_pCamera->Create(iWidth, iHeight, zBuffer, 1.f, 500.f))
	{
		NTL_RETURN(FALSE);
	}
	
	// active renderware camera setting 한다.
	CNtlPLGlobal::SetActiveCamera(m_pCamera->GetCamera()); 
	
	
	m_pCharset = RtCharsetCreate(&m_ForegroundColor, &m_BackgroundColor);
    if( m_pCharset == NULL )
	{
		NTL_RETURN(FALSE);
	}
	
	m_pVisualManager = NTL_NEW CNtlPLVisualManager;
	if(!m_pVisualManager->Create())
	{
		NTL_RETURN(FALSE);
	}
 	
	 

	// active scene manager를 setting 한다.
	CNtlPLSceneManagerFactory::ActiveSceneManager(m_pVisualManager);

	// property container 생성 및 load 한다.
	if(!CNtlPLPropertyContainer::GetInstance()->Load(""))
	{
		NTL_ASSERTFAIL("Property container load fail");
		NTL_RETURN(FALSE);
	}
	
	RwBBox bb;
	bb.inf.x = -1000.f;	bb.inf.y = -1000.f;	bb.inf.z = -1000.f;
	bb.sup.x = 1000.f;	bb.sup.y = 1000.f;	bb.sup.z = 1000.f;
	m_pWorld = RpWorldCreate(&bb);
	
	CNtlPLGlobal::SetActiveWorld(m_pWorld); 
	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

	// ambient light create
	m_pLtAmbient = RpLightCreate(rpLIGHTAMBIENT);
	RwRGBAReal color = { 0.9f, 0.9f, 0.9f, 1.f};
	RpLightSetColor(m_pLtAmbient, &color);
	RpWorldAddLight(CNtlPLGlobal::m_pRpWorld, m_pLtAmbient);

	// directional light create
	m_pLtDirectional = RpLightCreate(rpLIGHTDIRECTIONAL);
	if( m_pLtDirectional)
    {
		RwFrame *pFrame = (RwFrame *)NULL;
        RpLightSetColor(m_pLtDirectional, &color);
	
        pFrame = RwFrameCreate();
        if( pFrame )
        {
			RwFrameRotate(pFrame, &CNtlPLGlobal::m_vXAxisV3, 135.0f, rwCOMBINEREPLACE);
			RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, -45.0f, rwCOMBINEPOSTCONCAT);

	//		RpLightSetFlags(m_pLtDirectional, rpLIGHTLIGHTATOMICS);
			RpLightSetFrame(m_pLtDirectional, pFrame);			
			RpWorldAddLight(CNtlPLGlobal::m_pRpWorld, m_pLtDirectional);
        }       
    }
	NTL_RETURN(TRUE);
}

RwBool CDboCharacterApplication::AttachPlugin()
{
	NTL_FUNCTION( "CDboCharacterApplication::AttachPlugin" );

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
	
	if (!RpNtlAtomicPluginAttach())
		NTL_RETURN(FALSE);

	if( !RpNtlWorldAttach() )
		NTL_RETURN(FALSE);

	

	//if( !RpToonMultiTexPluginAttach() )
		//NTL_RETURN(FALSE);

	NTL_RETURN(TRUE);
}

RwBool CDboCharacterApplication::Resize(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
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
				m_pCamera->CameraSize(m_pCamera->GetCamera(), &rt); 
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

void CDboCharacterApplication::Destroy()
{
	NTL_FUNCTION("CDboCharacterApplication::Destroy" );
	if(m_pPLCharacter)
	{
		m_pVisualManager->DeleteEntity(m_pPLCharacter);
		m_pPLCharacter = NULL;
	}
	
	// light destroy
	RpWorldRemoveLight (CNtlPLGlobal::m_pRpWorld, m_pLtAmbient);
	RpWorldRemoveLight (CNtlPLGlobal::m_pRpWorld, m_pLtDirectional);

	RwFrame *frame = RpLightGetFrame(m_pLtDirectional);

    RpLightSetFrame(m_pLtDirectional, NULL);
    RwFrameDestroy(frame);

    RpLightDestroy(m_pLtDirectional);

	
	// property를 destroy 한다.
	m_PropContainter.GetInstance()->Destroy();

	

	// visual manager destroy
	if(m_pVisualManager)
	{
		m_pVisualManager->Destroy();
		NTL_DELETE(m_pVisualManager);
		m_pVisualManager = NULL;
	}

	
	// character set destroy
	if(m_pCharset)
    {
		RtCharsetClose();
		RwRasterDestroy(m_pCharset);
		m_pCharset = NULL;
    }

	
	// camera destroy
	if(m_pCamera)
	{
		m_pCamera->Destroy();
		NTL_DELETE(m_pCamera);
		m_pCamera = NULL;
	}
	RpWorldDestroy(CNtlPLGlobal::m_pRpWorld);

	// Close the event system
	RWS::CEventHandler::UnRegisterStreamChunkHandlers();
//    CLinearMemoryManager::DestroyLinearMemoryManager();
    RWS::CEventHandler::Close ();

	//CNtlEventQueue::GetInstance()->Destroy();
	
	// API_PLInit
	API_PLTerminate();
	
	CNtlApplication::Destroy();
	
	NTL_RETURNVOID();
}

void CDboCharacterApplication::DisplayInfo()
{
	RwChar caption[256];
	//fps
	RsSprintf(caption, RWSTRING("FPS: %4.2f"), GetFps());
	RsCharsetPrint(m_pCharset, caption, 0, 0, rsPRINTPOSTOPRIGHT);

	RsSprintf(caption, RWSTRING("Window Width : %d, Height : %d"), GetWindowWidth(), GetWindowHeight());
	RsCharsetPrint(m_pCharset, caption, 0, 1, rsPRINTPOSTOPRIGHT);
}

RwBool	CDboCharacterApplication::Update(RwReal fTime, RwReal fElapsedTime)
{
	NTL_FUNCTION("CDboCharacterApplication::Update" );
	RwCameraClear(m_pCamera->GetCamera(), &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	
	if(m_pCamera)
		m_pCamera->Update(fElapsedTime);

	//Ready to render scene.
	if( RwCameraBeginUpdate(m_pCamera->GetCamera()) )
    {
		if(m_pVisualManager)
		{
			m_pVisualManager->Update(fElapsedTime);
			m_pVisualManager->Render();
		}
		
		DisplayInfo();
		//Render complete
		RwCameraEndUpdate(RwCameraGetCurrentCamera());
    }
	RwCameraShowRaster(m_pCamera->GetCamera(), GetHWnd(), 0);
	
	NTL_RETURN(TRUE);
}




LRESULT CDboCharacterApplication::WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
#define GET_KEY_REPEAT(lParam)	(lParam & 0x0000ffff)

	switch(message)
	{
	case WM_KEYDOWN:
		CInputHandler::GetInstance()->KeyDownHandler((unsigned int)wParam,  GET_KEY_REPEAT(lParam));
		break;
	case WM_KEYUP:
		CInputHandler::GetInstance()->KeyUpHandler((unsigned int)wParam,  GET_KEY_REPEAT(lParam));
		break;
	case WM_LBUTTONDOWN:
		CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_LBTN, false ); 
		m_dwPrevLMouseDownTime = timeGetTime();
		break;
	case WM_LBUTTONUP:
		CInputHandler::GetInstance()->MouseUpHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_LBTN, timeGetTime() - m_dwPrevLMouseDownTime);
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
	//return CNtlApplication::WndProc(window, message, wParam, lParam);
}

RwBool CDboCharacterApplication::CreatePLCharacter(const char *chScriptFileName)
{
	if(m_pPLCharacter)
	{
		m_pVisualManager->DeleteEntity(m_pPLCharacter);
		m_pPLCharacter = NULL;
		m_pPLCharacter = static_cast<CNtlPLCharacter*>(m_pVisualManager->CreateEntity(PLENTITY_CHARACTER, "toolcharacter"));
		m_pPLCharacter->CreateNew(chScriptFileName);
	}
	else
	{
		m_PropContainter.GetInstance()->Create();
		m_PropContainter.GetInstance()->Load(NULL);
		m_pPLCharacter = static_cast<CNtlPLCharacter*>(m_pVisualManager->CreateEntity(PLENTITY_CHARACTER, "toolcharacter"));
		m_pPLCharacter->CreateNew(chScriptFileName);
	}
	return TRUE;
}

RwBool CDboCharacterApplication::SetAllChannelChangeAnimation(unsigned int uiAnimKey)
{
	if(m_pPLCharacter == NULL)
		return FALSE;
	
	m_pPLCharacter->SetAllChannelChangeAnimation(uiAnimKey);
	return TRUE;
}

RwBool CDboCharacterApplication::SavePLCharacter(const char *szSaveFileName)
{
	//return m_pPLCharacter->Save(szSaveFileName);
	return TRUE;
}

RwBool CDboCharacterApplication::LoadPLCharacter(const char *szLoadFileName)
{
	/*
	if(m_pPLCharacter)
	{	
		m_pVisualManager->DeleteEntity(m_pPLCharacter);
		m_pPLCharacter = NULL;
	}
	else
	{
		m_PropContainter.GetInstance()->Create();
		m_PropContainter.GetInstance()->Load(NULL);
	}

	m_pPLCharacter = static_cast<CNtlPLCharacter*>(m_pVisualManager->CreateEntity(PLENTITY_CHARACTER, "character"));

	if(!m_pPLCharacter->Load(szLoadFileName))
		return FALSE;
	*/

	return TRUE;
}
