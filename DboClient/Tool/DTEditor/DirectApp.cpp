#include "stdafx.h"

#include "DirectApp.h"

#include <direct.h>

// renderware
#include <rwcore.h>
#include <rpworld.h>
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

#include <rpuvanim.h>
#include <rpprtstd.h>
#include <rpprtadv.h>
#include <string>

// shared
#include "NtlClientNet.h"
#include "PCTable.h"
#include "WorldTable.h"
#include "MobTable.h"

// core
#include "NtlAtomic.h"
#include "NtlPLMaterialExtWrapper.h"
#include "NtlDebug.h"
#include "NtlCharsetPrint.h"
#include "NtlText.h"
#include "NtlEventQueue.h"
#include "NtlCoreUtil.h"
#include "NtlCoreApi.h"

// gui
#include "gui_precomp.h"
#include "guicore_precomp.h"
#include "gui_componenttokenizer.h"

// sound
#include "NtlSoundManager.h"
#include "NtlSoundLogic.h"

// presentation
#include "NtlPLSceneManagerFactory.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLGlobal.h"
#include "NtlPLApi.h"
#include "NtlPLGuiManager.h"
#include "NtlResourceEffect.h"
#include "NtlEffectSystemFreeList.h"
#include "guicore_precomp.h"
#include "gui_precomp.h"
#include "ntlworldinterface.h"
#include "ntlworldplugin.h"
#include "ntlplworldentity.h"
#include "NtlWorldMaterialPlugin.h"
#include "NtlPLSceneManager.h"
#include "NtlPostEffectCamera.h"
#include "NtlPLObject.h"
#include "NtlPLResourcePack.h"
#include "NtlDefaultItemData.h"
#include "NtlPLEventGenerator.h"


// simulation
#include "NtlCameraManager.h"
#include "NtlSLApi.h"
#include "TableContainer.h"
#include "NtlSLEventFunc.h"
#include "NtlDTProp.h"
#include "NtlSobProxy.h"
#include "NtlSLLogic.h"
#include "NtlNetSender.h"
#include "NtlSLTBCrowdManager.h"
#include "NtlPLEmblemStuffList.h"
#include "NtlDTCinematicManager.h"

// DTEditor
#include "DTEditorGlobal.h"
#include "DTServer.h"
#include "CinematicGui.h"
#include "ViewControlDlg.h"


CDirectApplication::CDirectApplication() : m_pCharset(NULL)
{
	
	m_pInstance = this;

	m_pCharset = NULL;
	m_BackgroundColor.red   = 100;
	m_BackgroundColor.green = 100;
	m_BackgroundColor.blue  = 100;
	m_BackgroundColor.alpha = 0;

	m_ForegroundColor.red   = 255;
	m_ForegroundColor.green = 255;
	m_ForegroundColor.blue  = 255;
	m_ForegroundColor.alpha = 255;

	m_pVisualManager	= NULL;
	m_pPropContainter	= NULL;
	m_pPLEventHandler	= NULL;

	ZeroMemory( m_OutputText[0], sizeof( RwChar ) * 256 );
	ZeroMemory( m_OutputText[1], sizeof( RwChar ) * 256 );

	m_pWorldEntity		= NULL;
	m_pAvatar			= NULL;
	m_pCamera			= NULL;

	m_bFirstInWorld		= TRUE;
	m_uiMoveFlags		= 0;
	CNtlMath::MathRwV3dAssign(&m_vAvatarInitPos, 0.0f, 0.0f, 0.0f);

	m_pCinematicGui		= NULL;
}

CDirectApplication::~CDirectApplication()
{
	m_pInstance = NULL;
}


RwBool CDirectApplication::CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
{
	NTL_FUNCTION("CDirectApplication::CreateSubSystem" );
	
	CNtlApplication::CreateSubSystem(iWidth, iHeight, zBuffer);

	RwUInt8 byMasterRes = API_LoadSLDevMasterInformation();
	RwBool bDevUser = Logic_IsDevUser();
	SDevMasterInfo *pDevMasterInfo = API_GetSLDevMasterInformation();

	if(byMasterRes == DEVMASTER_INVALID_SERIAL)
	{
		MessageBox(GetHWnd(), "Develoment master file is invalid key\n master debug can not use !!!\n Please contact to programmer (Mr. Jang - hyung suk)", "Error", MB_OK);
	}

	RwBool bUsePackData = FALSE;
	if(::AfxMessageBox("Would you like use the pack data?", MB_YESNO) == IDYES)
	{
		bUsePackData = TRUE;
	}

	// Open the event handler module
	RWS::CEventHandler::Open();
    RWS::CEventHandler::RegisterStreamChunkHandlers();

	//----------------------------------------------------
	// core layer initial
	//----------------------------------------------------

	if(!API_CoreInit())
	{
		NTL_RETURN(FALSE);
	}

	//----------------------------------------------------
	// framework layer initial
	//----------------------------------------------------

	// camera
	// active renderware camera setting 한다.
	m_pCamera = NTL_NEW CNtlPostEffectCamera;
	if(!m_pCamera->Create(iWidth, iHeight, zBuffer, 0.5f , 500.0f))
	{
		NTL_RETURN(FALSE);
	}

	CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_HDR);

	CNtlPLGlobal::SetActiveCamera(m_pCamera->GetCamera()); 

	//----------------------------------------------------
	// presentation layer initial
	//----------------------------------------------------

	API_PLInit();

	m_pPLEventHandler = NTL_NEW CNtlPLEventHandler;
	if(!m_pPLEventHandler->Create())
	{
		NTL_RETURN(FALSE);
	}

	// character set
	m_pCharset = RtCharsetCreate(&m_ForegroundColor, &m_BackgroundColor);
    if( m_pCharset == NULL )
	{
		NTL_RETURN(FALSE);
	}	

//	Pack 사용
	if( bUsePackData )
	{
		LinkTextureLoad(ResourcPackTextureLoad);
		LinkGuiScriptLoad(GuiPackLoad);
		LinkLoadSound_from_Memory(SoundPackLoad);
		Link_IsExistSoundFile(IsExistSoundFile);
		LinkLoadEmblemStuff(ScriptPackLoad);

		GetNtlResourcePackManager()->LoadPackHeader();
		GetNtlResourcePackManager()->SetActiveTypeFlags(NTL_PACK_TYPE_FLAG_TEXTURE | NTL_PACK_TYPE_FLAG_OBJECT |
			NTL_PACK_TYPE_FLAG_TERRAIN | NTL_PACK_TYPE_FLAG_GUI |
			NTL_PACK_TYPE_FLAG_FLASH | NTL_PACK_TYPE_FLAG_LANGUAGE |
			NTL_PACK_TYPE_FLAG_TABLE | NTL_PACK_TYPE_FLAG_SCRIPT | NTL_PACK_TYPE_FLAG_PROPERTY);
	}
	
	// property container 생성 및 load 한다.
	m_pPropContainter = NTL_NEW CNtlPLPropertyContainer;

	if(!m_pPropContainter->LoadSerialize())
	{
		NTL_ASSERTFAIL("Property container load fail");
		NTL_RETURN(FALSE);
	}

	// visual manager create
	// active scene manager를 setting 한다.
	m_pVisualManager = NTL_NEW CNtlPLVisualManager;
	if(!m_pVisualManager->Create())
	{
		NTL_RETURN(FALSE);
	}
	CNtlPLSceneManagerFactory::ActiveSceneManager(m_pVisualManager);

	// gui setup
	if( !gui::CSetupGUI::Init( GetHWnd(), m_iScreenWidth, m_iScreenHeight, GetNtlGuiManager()->GetGuiManager(), 
								".\\gui\\Font\\GuiFont.def", ".\\script\\metatag.htm", ".\\gui\\Font\\FlashFont.def",
								".\\gui\\particle\\DBO_2DParticle.2ps" ) )
	{
		NTL_RETURN(FALSE);
	}

	CNtlPLGuiManager::GetInstance()->GetReourceManager()->AddPage("gui\\Game.rsr");
	CNtlPLGuiManager::GetInstance()->GetSurfaceManager()->AddPage("gui\\GameCommon.srf");

	GetNtlGuiManager()->CreatePLGuiGroup();
	
	// active scene manager를 setting 한다.
	CNtlPLSceneManagerFactory::ActiveSceneManager(m_pVisualManager);

	// sound 생성
	GetSoundManager()->Init(".\\sound\\", 1.0f, 1.0f, 1.0f, 0.5f);

	//----------------------------------------------------
	// simulaltion layer initial
	//----------------------------------------------------
	if(!API_SLInit(TRUE, m_pCamera, CTable::LOADING_METHOD_BINARY))
	{
		NTL_RETURN(FALSE);
	}

	if(!GetNtlSLCrowdManager()->Load(dDIRECTION_GAME_DATA_FILE))
	{
		NTL_RETURN(FALSE);
	}

	CNtlVirtualNetSender::SetVirtualNetSenderFunction(DTServerPacketProc); 

	// world table setting
	CViewControlDlg dlg;
	dlg.m_eType = EWORLD_CREATE;
	if( dlg.DoModal() != IDOK )
	{
		NTL_RETURN(FALSE);
	}

	const sWORLD_TBLDAT* pWorldTblData = GetActiveWorldTableData();
	
	// avatar 생성.
	CreteAvatarInfo(m_vAvatarInitPos);
	CreateWorld(m_vAvatarInitPos, pWorldTblData->szName);

	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

	// camera zoom in/out 
	GetNtlGameCameraManager()->SetMaxDist(10000.0f);
	
	GetInputActionMap()->LinkKeyboardMove(this, &CDirectApplication::ActionMapKeyboardMove);

	CreateDTEditorGlobal();

	//
	m_pCinematicGui = NTL_NEW CCinematicGui();
	if( !m_pCinematicGui->Create() )
	{
		m_pCinematicGui->Destroy();
		NTL_DELETE( m_pCinematicGui );
		NTL_RETURN( FALSE );
	}


	NTL_RETURN(TRUE);
}	

RwBool CDirectApplication::AttachPlugin()
{
	NTL_FUNCTION( "CDirectApplication::AttachPlugin" );

	if( !RpWorldPluginAttach() )
		NTL_RETURN(FALSE);

	if( !RpPVSPluginAttach() )
		NTL_RETURN(FALSE);

	if( !RpCollisionPluginAttach() )
		NTL_RETURN(FALSE);

	if( !RtAnimInitialize() )
		NTL_RETURN(FALSE);

	if( !RpLtMapPluginAttach() )
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

	// woody1019
	if( !RpNtlWorldAttach() )
		NTL_RETURN(FALSE);

	if( !RpNtlWorldMaterialPluginAttach() )
		NTL_RETURN(FALSE);

	NTL_RETURN(TRUE);
}

RwBool CDirectApplication::Resize(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer, RwBool bRestore_from_Minimize)
{
	NTL_FUNCTION("CDirectApplication::Resize" );

	CNtlApplication::Resize(iWidth, iHeight, zBuffer, bRestore_from_Minimize);

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
				rt.x = 0; rt.y = 0; rt.w = iWidth; rt.h = iHeight;
				m_pCamera->SetCameraSize(&rt); 
				CNtlPLEventGenerator::Resize(iWidth, iHeight, bRestore_from_Minimize);
				gui::CSetupGUI::Resize(iWidth, iHeight);
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

LRESULT CDirectApplication::PreTranslateMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;

	switch(message)
	{
	case WM_KEYDOWN:
		break;
	}

	/*

#define GET_KEY_REPEAT(lParam)	(lParam & 0x0000ffff)

	switch(message)
	{
	case WM_KEYDOWN:
		CInputHandler::GetInstance()->KeyDownHandler((unsigned int)wParam,  (unsigned int)GET_KEY_REPEAT(lParam));
		break;
	case WM_KEYUP:
		CInputHandler::GetInstance()->KeyUpHandler((unsigned int)wParam,  (unsigned int)GET_KEY_REPEAT(lParam));
		break;
	case WM_LBUTTONDOWN:
		CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_LBTN, false ); 
		break;
	case WM_LBUTTONUP:
		CInputHandler::GetInstance()->MouseUpHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_LBTN, timeGetTime() - m_dwPrevLMouseDownTime);
		break;
	case WM_RBUTTONDOWN:
		CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_RBTN, false); 
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
	*/

	return ret;
}

void CDirectApplication::OnIdle(void)
{
	
	CNtlApplication::Update();
}

void CDirectApplication::OnResize(RwUInt32 uiWidth, RwUInt32 uiHeight, RwBool zBuffer)
{
	Resize(uiWidth, uiHeight, zBuffer, FALSE);
}

void CDirectApplication::OnTeleport(RwReal fTeleportX, RwReal fTeleportZ)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	RwV3d vPos = pSobAvatar->GetPosition();

	vPos.x = fTeleportX;
	vPos.z = fTeleportZ;

	SWorldHeightStuff sStuff;
	Logic_GetWorldHeight(&vPos, sStuff);

	vPos.y = sStuff.fFinialHeight;

	pSobAvatar->SetPosition(&vPos);

	m_pWorldEntity->SetPortalPosition(vPos);

	m_bFirstInWorld = TRUE;
}

void CDirectApplication::OnWorldChange(void)
{
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

	const sWORLD_TBLDAT *pWorldTblData = GetActiveWorldTableData();
	
	pAvatarInfo->sWorldInfo.tblidx	= pWorldTblData->tblidx;
	pAvatarInfo->sWorldInfo.worldID = 1;

	CNtlMath::MathRwV3dAssign(&m_vAvatarInitPos, pWorldTblData->vDefaultLoc.x, pWorldTblData->vDefaultLoc.y, pWorldTblData->vDefaultLoc.z);

	m_pAvatar->SetPosition(&m_vAvatarInitPos);

	m_pAvatar->GetSobProxy()->RemoveWorld();
	RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

	m_pVisualManager->DeleteEntity(m_pWorldEntity);
	m_pWorldEntity = NULL;

	CreateWorld(m_vAvatarInitPos, pWorldTblData->szName);

	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

	m_pAvatar->GetSobProxy()->AddWorld();

	GetSceneManager()->SetVisiblePlanet(TRUE);

	m_bFirstInWorld = TRUE;
}

void CDirectApplication::OnAvatarChange(void)
{
	RwV3d vPos = m_pAvatar->GetPosition();
	DeleteAvatar();
	CreteAvatarInfo(vPos);
	CreateAvatar();
}

void CDirectApplication::OnMobCreate(void)
{
	RwV3d vPos = m_pAvatar->GetPosition();
	RwV3d vDir = m_pAvatar->GetDirection();

	vPos.x += vDir.x * 2.0f;
	vPos.y += vDir.y * 2.0f;
	vPos.z += vDir.z * 2.0f;

	const sMOB_TBLDAT *pMobTblData = GetActiveMonsterTableData();

	sMOB_BRIEF sMobBrief;

	sMobBrief.tblidx = pMobTblData->tblidx;
	sMobBrief.curLp = pMobTblData->dwBasic_LP;
	sMobBrief.maxLp = pMobTblData->dwBasic_LP;
	sMobBrief.fLastWalkingSpeed = pMobTblData->fWalk_Speed;
	sMobBrief.fLastRunningSpeed = pMobTblData->fRun_Speed;
	sMobBrief.wAttackSpeedRate	= pMobTblData->wAttack_Speed_Rate;

	sCHARSTATE sCharState;
	sCharState.sCharStateBase.byStateID										= CHARSTATE_SPAWNING;
	sCharState.sCharStateBase.dwStateTime									= 0;
	sCharState.sCharStateBase.dwConditionFlag								= 0;
	sCharState.sCharStateBase.aspectState.sAspectStateBase.byAspectStateId	= 0;
	sCharState.sCharStateBase.bFightMode									= false;

	sCharState.sCharStateBase.vCurLoc.x										= vPos.x;
	sCharState.sCharStateBase.vCurLoc.y										= vPos.y;
	sCharState.sCharStateBase.vCurLoc.z										= vPos.z;

	sCharState.sCharStateBase.vCurDir.x										= vDir.x;
	sCharState.sCharStateBase.vCurDir.y										= vDir.y;
	sCharState.sCharStateBase.vCurDir.z										= vDir.z;

	CNtlSLEventGenerator::SobMonsterCreate(SLCLASS_MONSTER, AcquireSerialId(), vPos, vDir, &sMobBrief, &sCharState);
}

void CDirectApplication::UpdateWorld(void)
{
	if(m_pWorldEntity && m_pWorldEntity->GetWorldReady())
	{
		if(m_pAvatar == NULL)
			CreateAvatar();

		RwV3d vPos = m_pAvatar->GetPosition();
		
		SetSplineStandardPosition(vPos);

		if(m_bFirstInWorld)
		{
			SWorldHeightStuff sStuff;
			Logic_GetWorldHeight(&vPos, sStuff);

			vPos.y = sStuff.fFinialHeight;

			m_pAvatar->SetPosition(&vPos);

			m_bFirstInWorld = FALSE;
		}
		

		if(!GetNtlSLGlobal()->IsWorldUpdateAvaterPosition() || GetEditModeType() == EDIT_MODE_FREE_CAMERA)
		{
			RwV3dAssign(&vPos, GetNtlGameCameraManager()->GetCameraPos());
		}

		m_pWorldEntity->SetPlayerPosition(vPos);
	}
}

void CDirectApplication::UpdateSoundListener(void)
{
	if(m_pAvatar)
	{
		// 시네마틱중이면 카메라로 세팅한다.
		if(GetNtlDTCinematicManager()->IsRunning())
		{
			GetSceneManager()->SetSoundListener(NULL);
			return;
		}

		GetSceneManager()->SetSoundListener(m_pAvatar->GetSobProxy()->GetPLMainEntity());
	}
	else
	{
		GetSceneManager()->SetSoundListener(NULL);
	}
}

RwBool CDirectApplication::Update(RwReal fTime, RwReal fElapsedTime)
{
	NTL_FUNCTION("CDirectApplication::Update" );

	CDTServer::GetInstance()->Update(fElapsedTime);

	API_CoreUpdate(fElapsedTime);

	// world concept update
	GetNtlWorldConcept()->Update(fElapsedTime);

	DTEditorUpdate(fElapsedTime);

	UpdateWorld();

	UpdateSoundListener();
		
	m_pVisualManager->UpdateBeforeCamera(fElapsedTime);

	if (m_pCamera->CameraClear(&m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
	{
		//Ready to render scene.
		if( m_pCamera->PostCameraBeginUpdate(fElapsedTime) )
		{
			m_pVisualManager->Update(fElapsedTime);

			m_pVisualManager->Render(); 

			SplineRender();
			RenderGrid();

			m_pCamera->PostCameraEndUpdate();
		}

		m_pCamera->UpdatePostEffectFilter(); 
		m_pCamera->UpdateMotionBlur(fElapsedTime);

		if( m_pCamera->MainCameraBeginUpdate(fElapsedTime) )
		{
			m_pVisualManager->PostRender();

			DisplayInfo();

			m_pCamera->MainCameraEndUpdate();
		}

		m_pCamera->CameraShowRaster(GetHWnd(), 0);
	}


	// 다른 edit viewer rendering.
	EditViewerRendering(fElapsedTime);
	
	NTL_RETURN(TRUE);
}

void CDirectApplication::Destroy()
{
	NTL_FUNCTION("CDboApplication::Destroy" );

	//-----------------------------------------------
	// DT Editor layer release
	//-----------------------------------------------

	if( m_pCinematicGui )
	{
		m_pCinematicGui->Destroy();
		NTL_DELETE( m_pCinematicGui );
	}

	CNtlPLGuiManager::GetInstance()->GetSurfaceManager()->RemovePage("GameCommon.srf"); 
	CNtlPLGuiManager::GetInstance()->GetReourceManager()->RemovePage("Game.rsr");

	GetNtlGuiManager()->DestroyPLGuiGroup();
	gui::CSetupGUI::Deinit();
	DestroyDTEditorGlobal();

	//-----------------------------------------------
	// simulation layer release
	//-----------------------------------------------

	DeleteAvatar();

	DeleteNtlSLCrowdManager();

	API_SLTerminate();	

	//-----------------------------------------------
	// presentation layer release
	//-----------------------------------------------

	RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);
	m_pVisualManager->DeleteEntity(m_pWorldEntity);
	
	// visual manager destroy
	if(m_pVisualManager)
	{
		m_pVisualManager->Destroy();
		NTL_DELETE(m_pVisualManager);
		m_pVisualManager = NULL;
	}
	
	// property를 destroy 한다.
	if(m_pPropContainter)
	{
		m_pPropContainter->Destroy();
		NTL_DELETE(m_pPropContainter);
	}

	// character set destroy
	if(m_pCharset)
    {
		RtCharsetClose();
		RwRasterDestroy(m_pCharset);
		m_pCharset = NULL;
    }

	// sound 해제
	GetSoundManager()->Release();

	UnLinkTextureLoad();
	UnLinkGuiScriptLoad();
	UnLinkLoadSound_from_Memory();
	UnLink_IsExistSoundFile();
	UnLinkLoadEmblemStuff();

	// API_PLInit
	API_PLTerminate();

	//------------------------------------------------------
	// framework layer release
	//------------------------------------------------------

	// camera destroy
	if(m_pCamera)
	{
		m_pCamera->Destroy();
		NTL_DELETE(m_pCamera);
		m_pCamera = NULL;
	}

	if(m_pPLEventHandler)
	{
		m_pPLEventHandler->Destroy();
		NTL_DELETE(m_pPLEventHandler);
	}

	//------------------------------------------------------
	// core layer release
	//------------------------------------------------------

	API_CoreTerminate();

	// Close the event system
	RWS::CEventHandler::UnRegisterStreamChunkHandlers();
    RWS::CEventHandler::Close ();
	
	CNtlApplication::Destroy();
	
	NTL_RETURNVOID();
}

int CDirectApplication::ActionMapKeyboardMove(unsigned int uiMoveFlags)
{
	m_uiMoveFlags = uiMoveFlags;

//	if(GetEditModeType() == EDIT_MODE_VIEWER)
	{
		if(uiMoveFlags == 0)
		{
			CNtlSLEventGenerator::KeyboardMoveStop();
            GetNtlGameCameraManager()->FreeCameraMoveStop();
		}
		else 
		{
            // 프리 카메라가 활성화 되어 있는지 확인한다.
            if(GetNtlGameCameraManager()->IsActiveFreeCamera())
            {
                GetNtlGameCameraManager()->FreeCameraMove(uiMoveFlags, GetFreeCameraSpeed());
            }
            else
            {
                // 프리카메라가 없는 경우에만 아바타 이동
			    CNtlSLEventGenerator::KeyboardMove(uiMoveFlags, FALSE, 0.0f);
            }
		}
	}

	return 1;
}

// woody1019
void CDirectApplication::CreateWorld(RwV3d& vAvatarPos, const RwChar *pWorldName)
{
	// set texture folder path
	_getcwd(dGET_WORLD_PARAM()->CurWorkingFolderName, 256);
	strcpy(dGET_WORLD_PARAM()->CurDefaultTexPath, dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcat(dGET_WORLD_PARAM()->CurDefaultTexPath, "\\texture\\ntlwe\\");

	// set mesh folder path
	std::string str = "\\world\\";
	str += pWorldName;

	strcpy(dGET_WORLD_PARAM()->WorldProjectFolderName, dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcat(dGET_WORLD_PARAM()->WorldProjectFolderName, str.c_str());

	str += "\\";
	str += "#######.gwp";

	FILE* pFile = NULL;
	RwUInt32 uiOffset, uiSize;

	std::string strPackFileName = GetNtlResourcePackManager()->LoadTerrain(str.c_str(), uiOffset, uiSize);
	
	if(strPackFileName.size() > 0)
	{
		pFile = fopen(strPackFileName.c_str(), "rb");
		if(pFile)
			fseek(pFile, uiOffset, SEEK_SET);
	}
	else
	{
		std::string strWorldFileName = ".";
		strWorldFileName += str;
		pFile = fopen(strWorldFileName.c_str(), "rb");
	}
	
	// create a global world param
	if(pFile)
	{
		LoadFileWorldState(dGET_WORLD_PARAM(), pFile);
		fclose(pFile);
	}

	m_pWorldEntity = static_cast<CNtlPLWorldEntity*>(GetSceneManager()->CreateEntity(PLENTITY_WORLD, "NtlWorld"));
	m_pWorldEntity->CreateWorld(vAvatarPos);
}


void CDirectApplication::CreteAvatarInfo(RwV3d vPos)
{
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	pAvatarInfo->uiSerialId = AcquireSerialId();		

	CPCTable *pPcTable = API_GetTableContainer()->GetPcTable();

	RwUInt8 byRace		= GetAvatarRace();
	RwUInt8 byClass		= GetAvatarClass();
	RwUInt32 byGender	= GetAvatarGender();

	sPC_TBLDAT *pTblData = (sPC_TBLDAT*)pPcTable->GetPcTbldat(byRace, byClass, byGender);   
	pAvatarInfo->sCharPf.tblidx = pTblData->tblidx;	
	
	wcscpy_s(pAvatarInfo->sCharPf.awchName, NTL_MAX_SIZE_CHAR_NAME+ 1, L"Test");

	pAvatarInfo->sCharPf.sPcShape.byFace = DEF_ITEM_START;
	pAvatarInfo->sCharPf.sPcShape.byHair = DEF_ITEM_START;
	pAvatarInfo->sCharPf.sPcShape.byHairColor = DEF_ITEM_START;
	pAvatarInfo->sCharPf.sPcShape.bySkinColor = DEF_ITEM_START;
	
	pAvatarInfo->sCharPf.avatarAttribute.Str = 1;
	pAvatarInfo->sCharPf.avatarAttribute.Con = 1;
	pAvatarInfo->sCharPf.avatarAttribute.Foc = 1;
	pAvatarInfo->sCharPf.avatarAttribute.Dex = 1;
	pAvatarInfo->sCharPf.avatarAttribute.Sol = 1;
	pAvatarInfo->sCharPf.avatarAttribute.Eng = 1;

	pAvatarInfo->sCharPf.curLp = 10000;
	pAvatarInfo->sCharPf.avatarAttribute.MaxLp = 10000;
	pAvatarInfo->sCharPf.wCurEP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wMaxEP = 100;
	pAvatarInfo->sCharPf.wCurRP = 100;

		
	pAvatarInfo->sCharPf.avatarAttribute.fRunSpeed = 8.0f;
	pAvatarInfo->sCharPf.byLevel = 1;
	pAvatarInfo->sCharPf.dwCurExp = 1;
    pAvatarInfo->sCharPf.bIsAdult = TRUE;

	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x = vPos.x;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y = vPos.y;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z = vPos.z;

	pAvatarInfo->sCharState.sCharStateBase.vCurDir.x = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.y = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.z = 1.0f;  

	pAvatarInfo->wItemCount	= 0;
	pAvatarInfo->bySkillCount	= 0;
	pAvatarInfo->byHTBCount		= 0;

//	pAvatarInfo->sItemPf[0].handle			= 20000;
//	pAvatarInfo->sItemPf[0].tblidx			= 14051;
//	pAvatarInfo->sItemPf[0].byPlace			= CONTAINER_TYPE_EQUIP;	
//	pAvatarInfo->sItemPf[0].byPos			= EQUIP_SLOT_TYPE_JACKET;
//	pAvatarInfo->sItemPf[0].byStackcount	= 1;
//	pAvatarInfo->sItemPf[0].byRank			= 1;
//	pAvatarInfo->sItemPf[0].byCurDur		= 1;		// 내구도

	for(RwInt32 i = 0; i < NTL_MAX_OPTION_IN_ITEM; i++)
	{
		pAvatarInfo->sItemPf[i].sOptionSet.Init();
	}
	
	pAvatarInfo->sItemPf[0].bNeedToIdentify = false;
	pAvatarInfo->sItemPf[0].byGrade			= 1;

	// world table data setting...
	const sWORLD_TBLDAT *pWorldTblData				= GetActiveWorldTableData();
	pAvatarInfo->sWorldInfo.tblidx					= pWorldTblData->tblidx;
	pAvatarInfo->sWorldInfo.worldID					= 1;
	pAvatarInfo->sWorldInfo.hTriggerObjectOffset	= 10000;
}

void CDirectApplication::CreateAvatar(void)
{
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

	m_pAvatar = reinterpret_cast<CNtlSobAvatar*>(CNtlSLEventGenerator::SobAvatarCreate(SLCLASS_AVATAR, pAvatarInfo->uiSerialId, &pAvatarInfo->sCharState));

	m_pAvatar->GetSobProxy()->EnablePicking(TRUE);

	GetNtlGameCameraManager()->SetActiveActor(m_pAvatar);
}

void CDirectApplication::DeleteAvatar(void)
{
	if(m_pAvatar)
	{
		CNtlSLEventGenerator::SobDelete(m_pAvatar->GetSerialID());
		m_pAvatar = NULL;
		GetNtlSLGlobal()->SetSobAvatar(NULL);
	}
}

void CDirectApplication::SplineRender(void)
{
	CNtlDTSpline *pDTSpline = (CNtlDTSpline*)GetActiveDTProp();
	if(pDTSpline)
	{
		RwV3d vPos = GetSplineStandardPosition();
		if(pDTSpline->IsUseWorldPosition())
		{
			CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);
		}

		RwMatrix mat;
		RwMatrixSetIdentity(&mat);
		RwMatrixTranslate( &mat, &vPos, rwCOMBINEPOSTCONCAT );

		SetSplineEditPosition(vPos);
		
		RwInt32 iSelSx, iSelEx;
		GetActiveSelectControlPoint(iSelSx, iSelEx);

		CNtlDTRender::DTPropRender(pDTSpline, &mat, GetSplinePointBoxSize(), iSelSx, iSelEx);

		RwInt32 iFocusIdx = GetFocusControlPoint();

		CNtlDTRender::DTPropFocusPointRender(pDTSpline, &mat, iFocusIdx, GetSplinePointBoxSize() * 1.4f);

		if(IsCustomDataRender())
			CNtlDTRender::DTPropValidCustomDataRender(pDTSpline, &mat, GetSplinePointBoxSize() * 0.5f);  
	}
}


void CDirectApplication::RenderGrid(void)
{
	RwV3d vPos = GetSplineStandardPosition();

	CNtlDTSpline *pDTSpline = (CNtlDTSpline*)GetActiveDTProp();

	RwMatrix mat;
	RwMatrixSetIdentity(&mat);
	RwMatrixTranslate( &mat, &vPos, rwCOMBINEPOSTCONCAT );

	RwInt32 iLineCx	= GetGridLineCx();
	RwInt32 iLineCy	= GetGridLineCy();
	
	RwIm3DVertex *pLineXVertex = GetLineXVertex();
	RwIm3DVertex *pLineYVertex = GetLineYVertex();

	RwIm3DVertex *pAxisXVertex = GetLineAxisXVertex();
	RwIm3DVertex *pAxisZVertex = GetLineAxisZVertex();
	
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);

	if(RwIm3DTransform(pLineXVertex, iLineCx*2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
		RwIm3DEnd();
	}
	
	if(RwIm3DTransform(pLineYVertex, iLineCy*2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
		RwIm3DEnd();
	}

	if(RwIm3DTransform(pAxisXVertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
		RwIm3DEnd();
	}

	if(RwIm3DTransform(pAxisZVertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
		RwIm3DEnd();
	}
}

void CDirectApplication::DisplayInfo(void)
{
	RwChar caption[256];
	RwInt32 iIdx = 0;

	//fps
	RsSprintf(caption, RWSTRING("FPS: %4.2f"), GetFps());
	RsCharsetPrint(m_pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

	if(m_pAvatar)
	{
		RwV3d vPos = m_pAvatar->GetPosition();
		RsSprintf(caption, RWSTRING("Avatar Pos : %6.2f, %6.2f, %6.2f"), vPos.x, vPos.y, vPos.z);
		RsCharsetPrint(m_pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

		RwV3d vDir = m_pAvatar->GetDirection();
		RsSprintf(caption, RWSTRING("Avatar Dir : %6.2f, %6.2f, %6.2f"), vDir.x, vDir.y, vDir.z);
		RsCharsetPrint(m_pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);
	}

	CNtlCharsetPrint *pCharsetPrint = CNtlCharsetPrint::GetInstance(); 
	RwInt32 iSetCount = pCharsetPrint->GetCount(); 
	for(RwInt32 i = 0; i < iSetCount; i++)
	{
		if(pCharsetPrint->IsValid(i))
		{
			RsCharsetPrint(m_pCharset, pCharsetPrint->GetString(i), 0, iIdx++, rsPRINTPOSTOPRIGHT);
		}
	}
}