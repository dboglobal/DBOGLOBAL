#include "precomp_dboclient.h"
#include "resource.h"

//#include "toonmultiTex.h"
#include "DboApplication.h"

// core
#include "NtlAtomic.h"
#include "NtlPLMaterialExtWrapper.h"
#include "NtlDebug.h"
#include "NtlCharsetPrint.h"
#include "NtlText.h"
#include "NtlEventQueue.h"
#include "NtlCoreUtil.h"
#include "NtlCoreApi.h"
#include "NtlLoadingController.h"
#include "NtlProfiler.h"

// shared
#include "NtlClientNet.h"
#include "table.h"

// frame
#include "NtlStage.h"

// sound
#include "NtlSoundManager.h"

// gui
#include "gui_precomp.h"
#include "guicore_precomp.h"
#include "gui_componenttokenizer.h"
#include "gui_htmlbox_generic.h"
#include "gui_htmltokenizer.h"
#include "gui_inputbox_generic.h"
#include "StringLocalConverter.h"

// flash
#include "NtlFlasherManager.h"

// presentation
#include "NtlPLEventGenerator.h"
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
#include "NtlPLEmblemStuffList.h"
#include "NtlPLPalette.h"

// simulation
#include "InputHandler.h"
#include "InputActionMap.h"
#include "NtlPacketLockManager.h"
#include "NtlCameraManager.h"
#include "NtlSLApi.h"
#include "StageManager.h"
#include "NtlPLVisualManager.h"
#include "InputActionMap.h"
#include "NtlSLLogic.h"
#include "NtlStorageManager.h"
#include "NtlSLTBCrowdManager.h"
#include "NtlStorageManager.h"
#include "NtlSoundLogic.h"

// dbo
#include "PacketProc.h"
#include "DisplayParser.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "DboDef.h"
#include "DboEventGenerator.h"
#include "DboEventHandler.h"
#include "DboVirtualServer.h"
#include "DumpCommand.h"
#include "CursorManager.h"
#include "DboLogic.h"
#include "Msjexhnd.h"
#include "DboExceptionHandler.h"
#include "SayFilter.h"
#include "NtlBugTrap.h"
#include "MoviePlayer.h"
#include "DboAddinManager.h"
#include "LinkItemTrasformList.h"
#include "MrPoPoHandler.h"
#include "PetitionManager.h"
#include "MsgBoxManager.h"
#include "DboExtraErrorReport.h"
#include "TeleportProposalManager.h"
#include "Thread.h"
#include "PacketTracer.h"
#include "LobbyManager.h"


// discord
#ifdef USE_DISCORD
#include "Discord.h"
#endif

// lleo52 - 추후 삭제 필요
#include "NtlMovieManager.h"
#include <fstream>

// lleo52 - 추후 삭제 필요
std::vector< NTL_MOVIE_HANDLE > g_vecdefMovieList;


CDboApplication::CDboApplication() : m_pCharset(NULL)
{
	m_pInstance				= this;

	m_BackgroundColor.red   = 0;
	m_BackgroundColor.green = 0;
	m_BackgroundColor.blue  = 0;
	m_BackgroundColor.alpha = 0;

	m_ForegroundColor.red   = 255;
	m_ForegroundColor.green = 255;
	m_ForegroundColor.blue  = 255;
	m_ForegroundColor.alpha = 255;

	m_pStageMgr				= NULL;
	m_pVisualManager		= NULL;
	m_pCamera				= NULL;
	m_pDboEventHandler		= NULL;
	m_pDumpManager			= NULL;
	m_pGamma				= NULL;

	ZeroMemory( m_OutputText[0], sizeof( RwChar ) * 256 );
	ZeroMemory( m_OutputText[1], sizeof( RwChar ) * 256 );

	m_bMouseBackgroundCapture = FALSE;

	CNtlTimer::ActiveInterpolation(true);

	m_fLastClientInfoUpdate = 0.0f;
	m_lastClientInfoSent = 0;
}

CDboApplication::~CDboApplication()
{
	m_pInstance = NULL;
}

void CDboApplication::SetErrorReport()
{
    CDboExtraErrorReport::SetDXDiagLog();

	// Settings for bugtrap
	char szVer[32] = {0,};
	sprintf_s(szVer, "%d.%d", CLIENT_LVER, CLIENT_RVER);

    BT_SetAppName(_T(m_DboGlobal.GetConfigData()->strBugTrapFolder.c_str()));         
	BT_SetAppVersion(szVer);    
	BT_SetFlags(BTF_DETAILEDMODE | BTF_SCREENCAPTURE);
	//BT_SetFlags(BTF_NONE);
    BT_SetSupportServer(GetDboGlobal()->GetConfigData()->strBugTrapServerIP.c_str(), (SHORT)GetDboGlobal()->GetConfigData()->dwBugTrapServerPort);	
	
	char buf[NTL_MAX_DIR_PATH] = {0,};
    ::GetCurrentDirectory(NTL_MAX_DIR_PATH, buf);

    // Additional information (game ID)
	std::string strAddLogFile = buf;
	strAddLogFile += "\\";
	strAddLogFile += ERROR_REPORT_FILE_NAME;
	BT_AddLogFile(strAddLogFile.c_str());    

    // DBO Log
	strAddLogFile = buf;
	strAddLogFile += "\\dbolog.txt";		
	BT_AddLogFile(strAddLogFile.c_str());

    // DX information file
    strAddLogFile = buf;
    strAddLogFile += "\\DxLog.txt";
    BT_AddLogFile(strAddLogFile.c_str());
}

RwBool CDboApplication::CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
{
	NTL_FUNCTION("CDboApplication::CreateSubSystem" );

	 // Load keyboard accelerators
    m_hAccel = LoadAccelerators( NULL, MAKEINTRESOURCE(IDR_MAIN_ACCEL) );

	// dev master 정보를 읽는다.
	RwUInt8 byMasterRes = API_LoadSLDevMasterInformation();
	RwBool bDevUser = Logic_IsDevUser();
	SDevMasterInfo *pDevMasterInfo = API_GetSLDevMasterInformation();

	if(byMasterRes == DEVMASTER_INVALID_SERIAL)
	{
		MessageBox(GetHWnd(), "Develoment master file is invalid key\n master debug can not use !!!\n Please contact to programmer (Mr. Jang - hyung suk)", "Error", MB_OK);
	}

#ifdef NDEBUG
	if( !bDevUser )
	{
		NtlTraceEnable_inRelease(false);
	}
#endif

	// FrameWork Layer
	CNtlApplication::CreateSubSystem(iWidth, iHeight, zBuffer);


#ifdef COLLSION_VIEW
	//080716 : Cz
	//CNtlPLObject::m_bLoadCollisionMesh = TRUE;
	CNtlPLGlobal::m_bCollObjVisible = TRUE;
#endif

	// Open the event handler module
	RWS::CEventHandler::Open();
	RWS::CEventHandler::RegisterStreamChunkHandlers();

	// dbo global create
	if(!m_DboGlobal.Create(GetHWnd()))
	{
		NTL_ASSERTFAIL("dbo global create fail");
		NTL_RETURN(FALSE);
	}

    // Bug Trap
    SetErrorReport();    

	// core layer
	//-------------------------------------------------------------------------------------

	if(!API_CoreInit())
	{
		NTL_RETURN(FALSE);
	}

	m_DboGlobal.SetScreenSize(GetScreenWidth(), GetScreenHeight()); 

	// Gamma Controller
	m_pGamma = NTL_NEW CNtlGammaController();
	if( m_pGamma )
		m_pGamma->Create( GetNtlStorageManager()->GetFloatData( dSTORAGE_GRAPHIC_GAMMA ) );

	// set Option Apply func
	GetNtlStorageManager()->SetCommonApplyFunc( Logic_ApplyNtlStorageUnit );

	//----------------------------------------------------------------------
	// presentation layer
	//----------------------------------------------------------------------

	// API_PLInit
	API_PLInit();

	// Presentation layer option apply
	GetNtlStorageManager()->ApplyStorageType( eNTL_STORAGE_GRAPHIC, eNTL_STORAGE_APPLY_PRESENTATION_INITONCE );

	// resourec pack
#ifdef USE_PACK_DATA

	if( !bDevUser || (bDevUser && pDevMasterInfo->bPackFileUse) )
	{
//		LinkTextureLoad(ResourcPackTextureLoad, TextureLoadLevelPath);
//		GetNtlResourceManager()->SetTextureLoadLevel(CNtlPLResourceManager::RES_TEX_TERRAIN_DICT, 3);
//	GetNtlResourceManager()->SetTextureLoadLevel(CNtlPLResourceManager::RES_TEX_OBJECT_DICT, 3);
//		GetNtlResourceManager()->SetTextureLoadLevel(CNtlPLResourceManager::RES_TEX_CHARACTER_DICT, 3);
//		GetNtlResourceManager()->SetTextureLoadLevel(CNtlPLResourceManager::RES_TEX_EFFECT_DICT, 2);

		LinkTextureLoad(ResourcPackTextureLoad);
		LinkGuiScriptLoad(GuiPackLoad);
		LinkFlashResourceLoad(FlashPackLoad);
		LinkLoadSound_from_Memory(SoundPackLoad);
		Link_IsExistSoundFile(IsExistSoundFile);
		LinkLoadEmblemStuff(ScriptPackLoad);
		LinkHtmlScriptLoad(ScriptPackLoad);
		LinkLoadContract(ScriptPackLoad);

		GetNtlResourcePackManager()->LoadPackHeader();
		GetNtlResourcePackManager()->SetActiveTypeFlags(NTL_PACK_TYPE_FLAG_TEXTURE | NTL_PACK_TYPE_FLAG_OBJECT | 
														NTL_PACK_TYPE_FLAG_TERRAIN | NTL_PACK_TYPE_FLAG_GUI |
														NTL_PACK_TYPE_FLAG_FLASH | NTL_PACK_TYPE_FLAG_LANGUAGE|
														NTL_PACK_TYPE_FLAG_TABLE | NTL_PACK_TYPE_FLAG_SCRIPT |
                                                        NTL_PACK_TYPE_FLAG_PROPERTY);
	}

#endif

	// by daneos. Removed from API_PLInit because it needs to be called after loading pack data
	CNtlPLPalette::CreateInstance();
	CNtlPLEmblemMaker::CreateInstance();

	if ( !GetDboGlobal()->GetDBOUIConfig()->Load() )
	{
		NTL_RETURN(FALSE);
	}

	// display string load

	SConfigData *pConfigData = m_DboGlobal.GetConfigData(); 
	CDisplayParser parser(".\\language\\local_data.dat");
	GetDisplayStringManager()->InsertOptions(parser);
	GetAlarmManager()->ParseData( parser, ".\\language\\local_msg_type.alm", FALSE );

	CDisplayParser syncparser(".\\language\\local_sync_data.dat");
	GetDisplayStringManager()->InsertOptions(syncparser, CDisplayStringManager::SERVER_SYNC_RESULT_MSG);
	GetAlarmManager()->ParseData( syncparser, ".\\language\\local_sync_msg_type.alm", TRUE );

	//-------------------------------------------------------------------------------------


	// event handler
	m_pPLEventHandler = NTL_NEW CNtlPLEventHandler;
	if(!m_pPLEventHandler->Create())
	{
		NTL_RETURN(FALSE);
	}

	CPetitionManager::CreateInstance();

	// property container 생성 및 load 한다.

	RwBool bLoadPropertyContainer;

	if(Logic_IsDevUser())
	{
		if(pDevMasterInfo->bLoadDataXml)
			bLoadPropertyContainer = CNtlPLPropertyContainer::GetInstance()->Load();
		else
		{
			if(LOAD_DATA_MODE == DATA_MODE_XML)
				bLoadPropertyContainer = CNtlPLPropertyContainer::GetInstance()->Load();
			else //if(LOAD_DATA_MODE == DATA_MODE_BIN)
				bLoadPropertyContainer = CNtlPLPropertyContainer::GetInstance()->LoadSerialize();
		//	else if(LOAD_DATA_MODE == DATA_MODE_ENC)
		//		bLoadPropertyContainer = CNtlPLPropertyContainer::GetInstance()->LoadSerialize(TRUE, ENC_DATA_PASSWORD);
		//	else
		//		bLoadPropertyContainer = CNtlPLPropertyContainer::GetInstance()->Load();
		}
	}
	else
	{
		if(LOAD_DATA_MODE == DATA_MODE_XML)
			bLoadPropertyContainer = CNtlPLPropertyContainer::GetInstance()->Load();
		else //if(LOAD_DATA_MODE == DATA_MODE_BIN)
			bLoadPropertyContainer = CNtlPLPropertyContainer::GetInstance()->LoadSerialize();
	//	else if(LOAD_DATA_MODE == DATA_MODE_ENC)
	//		bLoadPropertyContainer = CNtlPLPropertyContainer::GetInstance()->LoadSerialize(TRUE, ENC_DATA_PASSWORD);
	//	else
	//		bLoadPropertyContainer = CNtlPLPropertyContainer::GetInstance()->Load();
	}

	if(!bLoadPropertyContainer)
	{
		NTL_ASSERTFAIL("Property container load fail");
		NTL_RETURN(FALSE);
	}

	// camera 
	m_pCamera = NTL_NEW CNtlPostEffectCamera;
	// disable b daneos
	/*if(bDevUser)
	{
		SDevMasterInfo *pDevMasterInfo = API_GetSLDevMasterInformation();
		if(!m_pCamera->Create(iWidth, iHeight, zBuffer, pDevMasterInfo->fCameraNear , pDevMasterInfo->fCameraFar))
		{
			NTL_RETURN(FALSE);
		}
	}
	else*/
	{
		if(!m_pCamera->Create(iWidth, iHeight, zBuffer, DEFAULT_CAMERA_NEAR , DEFAULT_CAMERA_FAR))
		{
			NTL_RETURN(FALSE);
		}
	}

	// active renderware camera setting 한다.
	CNtlPLGlobal::SetActiveCamera(m_pCamera->GetCamera()); 

	// character set
	m_pCharset = RtCharsetCreate(&m_ForegroundColor, &m_BackgroundColor);
	if( m_pCharset == NULL )
	{
		NTL_RETURN(FALSE);
	}	

	// visual manager create
	m_pVisualManager = NTL_NEW CNtlPLVisualManager;
	if(!m_pVisualManager->Create())
	{
		NTL_RETURN(FALSE);
	}
	// active scene manager를 setting 한다.
	CNtlPLSceneManagerFactory::ActiveSceneManager(m_pVisualManager);

	// gui setup
	if( !gui::CSetupGUI::Init( GetHWnd(), m_iScreenWidth, m_iScreenHeight, GetNtlGuiManager()->GetGuiManager(), 
							   ".\\gui\\Font\\GuiFont.def", ".\\script\\metatag.htm", ".\\gui\\Font\\FlashFont.def",
							   ".\\gui\\particle\\DBO_2DParticle.2ps" ) )
	{
		DBO_WARNING_MESSAGE("FAILED TO INIT GUI");
		NTL_RETURN(FALSE);
	}

	// Particle Surface 로드

	// GlobalGui 생성_ 해제는 역순으로.
	gui::CGlobalComponent::GetInstance()->CreateComponent( "gui\\GlobalGui.rsr", "gui\\GlobalGui.srf", "gui\\GlobalGui.frm", 
		GetNtlGuiManager()->GetReourceManager(), GetNtlGuiManager()->GetSurfaceManager(),
		GetNtlGuiManager()->GetComponentManager(), GetNtlGuiManager()->GetGuiManager() );
	gui::CGlobalComponent::GetInstance()->SetToolTip( "tooltipcomponent" );

	// sound 생성
	GetSoundManager()->Init(".\\sound\\", 1.0f, 1.0f, 1.0f, 0.5f);

	//----------------------------------------------------------------------
	// simulation layer
	//----------------------------------------------------------------------

	CTable::eLOADING_METHOD eLoadMethod;
	if(LOAD_DATA_MODE == DATA_MODE_XML)
		eLoadMethod = CTable::LOADING_METHOD_XML;
	else //if(LOAD_DATA_MODE == DATA_MODE_BIN)
		eLoadMethod = CTable::LOADING_METHOD_BINARY;
	//else
	//	eLoadMethod = CTable::LOADING_METHOD_SECURED_BINARY;

	LinkCallback_GetNPCJobName(Logic_GetNPCJobName);

#ifdef NET_SERVER_SYNC

	if(!API_SLInit(FALSE, m_pCamera, (RwUInt8)eLoadMethod, TRUE))
	{
		NTL_RETURN(FALSE);
	}

#else

	if(!API_SLInit(TRUE, m_pCamera, (RwUInt8)eLoadMethod))
	{
		NTL_RETURN(FALSE);
	}

#endif

	if ( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_PROPERTY )
	{
		void* pBuffer = NULL;
		RwInt32 iSize;

		GetNtlResourcePackManager()->LoadProperty( dDIRECTION_GAME_DATA_FILE, (void**)&pBuffer, &iSize );

		if( NULL == pBuffer )
		{
			DBO_FAIL( "Can not load scprit file of name : " << dDIRECTION_GAME_DATA_FILE );
			NTL_RETURN( FALSE );
		}

		CNtlFileSerializer nsl;
		nsl.In( pBuffer, iSize );

		if ( !GetNtlSLCrowdManager()->Load(nsl) )
		{
			NTL_ARRAY_DELETE( pBuffer );
			NTL_RETURN( FALSE );
		}

		NTL_ARRAY_DELETE( pBuffer );
	}
	else
	{
		if ( !GetNtlSLCrowdManager()->Load( dDIRECTION_GAME_DATA_FILE ) )
		{
			DBO_FAIL( "Can not load scprit file of name : " << dDIRECTION_GAME_DATA_FILE );
			NTL_RETURN( FALSE );
		}
	}

	// Ntl Storage에 Apply 함수를 지정하고 Scouter와 Quest에 저장을 하는 함수를 지정한다.
	

	// 볼륨이 너무 큰 사운드 그룹의 최대 볼륨 조정
	float fDefaultVolume = GetNtlStorageManager()->GetFloatData( dSTORAGE_SOUND_BACK_VOLUME );
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_JINGLE_MUSIC,	fDefaultVolume);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_FLASH_MUSIC,	fDefaultVolume);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_BGM,			fDefaultVolume);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_OBJECT_MUSIC,	fDefaultVolume);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_AMBIENT_MUSIC,	fDefaultVolume);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_WEATHER_MUSIC,	fDefaultVolume);

	//----------------------------------------------------------------------
	// client
	//----------------------------------------------------------------------

	if( FALSE == CLobbyManager::CreateInstance() )
		NTL_RETURN(FALSE);

	m_pDboEventHandler = NTL_NEW CDboEventHandler;
	if(!m_pDboEventHandler->Create())
	{
		NTL_RETURN(FALSE);
	}

	// msg box
	CMsgBoxManager::CreateInstance();
	CTeleportProposalManager::CreateInstance();

	// stage manager create
	m_pStageMgr = NTL_NEW CStageManager;
	if(!m_pStageMgr->Create())
	{
		NTL_RETURN(FALSE);
	}

	// debug console command manager
	m_pDumpManager = NTL_NEW CDumpCmdManager;
	if (Logic_IsDevUser())
	{
		GetDumpCmdManager()->ActiveDumpTarget(CDumpCmdManager::DUMP_TARGET_CONSOLE, TRUE);
	}

	GetCursorManager()->CreateInstance();
	GetCursorManager()->InitLobbyCursor();

	// Connect Exception Handler
	g_MSJExceptionHandler.SetApplicationFilter( DboExceptionHandler );

	// gui html doc callback function
	gui::CHtmlDoc::LinkUserTag(Logic_CallbackHtmlUserTag);
	gui::CHtmlBox_Generic::LinkHtmlLinkTag(Logic_CallbackHtmlLinkTag);

	// evnet Occur(stage create).
	GetDboGlobal()->LoadingContractInfo();
	CDboEventGenerator::StageCreate(LOGIN_STAGE_NAME);

	// 필터링 테이블과 텍스트 올 테이블에서 데이터를 읽여들여 채팅 메시지 필터링 데이터를 생성
	GetChattingFilter()->Create();

	if(bDevUser)
	{
		DragAcceptFiles(m_hWnd, TRUE);
	}

    // AddIn
    CDBOAddinManager::Init();

	// Init mouse double click time
	API_InitDoubleClickInterval();

	CMoviePlayer::CreateInstance();

	if( false == gui::CStringLocalConverter::CreateInstance() )
	{
		NTL_RETURN(FALSE);
	}

	CPacketTracer::CreateInstance();			/// woosungs_test 20090804

	// lleo52 - 추후 제거 필요
	CNtlMovieManager::CreateInstance( CDboApplication::GetInstance()->GetHWnd() );

#ifdef USE_DISCORD

	CDiscord::CreateInstance();

	GetDiscordManager()->SetUseDiscord(GetNtlStorageManager()->GetBoolData(dSTORAGE_SYSTEM_ETC_DISCORD));

#endif




	NTL_RETURN(TRUE);
}	

RwBool CDboApplication::AttachPlugin()
{
	NTL_FUNCTION( "CDboApplication::AttachPlugin" );

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

	// 1OLD TERRAIN
	//	if (!CNtlPLTerrain::RpWorldSectorExPlugInAttach())
	//		NTL_RETURN(FALSE);

	//if (!RpToonMultiTexPluginAttach())
	//NTL_RETURN(FALSE);

	NTL_RETURN(TRUE);
}

RwBool CDboApplication::Resize(RwInt32 iWidth, RwInt32 iHeight, RwBool zBuffer, RwBool bRestore_from_Minimize)
{
	NTL_FUNCTION("CDboApplication::Resize" );

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
				
				m_DboGlobal.SetScreenSize(iWidth, iHeight); 
				gui::CSetupGUI::Resize(iWidth, iHeight);
				CNtlPLEventGenerator::Resize(iWidth, iHeight, bRestore_from_Minimize);
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

void CDboApplication::SetApplicationMinMax(LPMINMAXINFO lpmmi)
{
	NTL_FUNCTION("CDboApplication::SetApplicationMinMax" );

	lpmmi->ptMinTrackSize.x = DEFAULT_SCREEN_WIDTH;
	lpmmi->ptMinTrackSize.y = DEFAULT_SCREEN_HEIGHT;
	
	NTL_RETURNVOID();
}

LRESULT CDboApplication::PreTranslateMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( GetMoviePlayer() && MOVIE_HANDLE_EVENT_OK != GetMoviePlayer()->HandleEvent(window, message, wParam, lParam) )
		return 0;
	
	CDboGlobal *pDboGlobal = GetDboGlobal();
	if(pDboGlobal == NULL)
		return 0;

	// avooo's comment : (08. 10.21) 다른 방법으로 인풋이 제어되고 있다. 이 부분은
	// 필요가 없을 것 같아 한동안 주석으로 처리하고 결과를 지켜본다
	// input이 active 안되어 있으면.
	//if(!pDboGlobal->GetGameData()->bInputActive)
	//	return 0;

	// window command message 조사.
	switch(message)
	{
		case WM_DROPFILES:
		{
			//if( Logic_IsDevUser() )
			//{
			//	POINT pt;
			//	char buffer[_MAX_PATH] = {0,};

			//	// 어느 위치에 드롭되었는지 그 항목을 알아낸다.
			//	if (DragQueryPoint((HDROP)wParam, &pt)) 
			//	{
			//		UINT i = 0;
			//		// 모두 몇 개의 파일이 드롭되었는지 알아낸다.
			//		// 만일 폴더가 드롭되었다면 폴더의 이름만 넘어온다.
			//		UINT uCount = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL ,0);

			//		for(i = 0;i < uCount;i++)
			//		{
			//			// 드롭된 파일의 이름을 알아온다.
			//			DragQueryFile((HDROP)wParam, i, buffer ,_MAX_PATH);

			//			// 드롭된 파일 이름을 출력해본다.
			//			//MessageBox(m_hWnd, buffer, "File Name", MB_OK);

			//			string	strCmp		= buffer;
			//			RwInt32 SequenceIdx = strCmp.find_last_of('\\');
			//			strCmp.erase(strCmp.begin(), strCmp.begin() + SequenceIdx + 1);

			//			if(!stricmp("LinkItemTrasformList.xml", strCmp.c_str()))
			//			{
			//				CDboEventGenerator::NotifyMessage(SDboEventNotify::ACTION, L"Link Mesh Transform Script이 갱신되었습니다.");
			//				CLinkItemTrasformList::GetInstance().Refresh();
			//			}
			//		}
			//	}
			//	// drag and drop 작업을 끝낸다.
			//	DragFinish((HDROP)wParam);
			//	break;
			//}
			
			break;
		}

		case WM_COPYDATA:
		{
			/*if( Logic_IsDevUser() )
			{
				char acBuffer[256] = "";
				COPYDATASTRUCT* pCDS = (COPYDATASTRUCT*)lParam;

				memset(acBuffer, 0, sizeof(acBuffer));

				if( pCDS->lpData )
					memcpy(acBuffer, (const char*)pCDS->lpData, pCDS->cbData);

				GetDumpCmdManager()->CleintConsoleCommand(acBuffer);
			}*/
			
			break;
		}
		case WM_COMMAND:
			{
				switch( LOWORD(wParam) )
				{
				case ID_TOGGLE_FULLSCREEN:
					if(Logic_IsDevUser()) 
							ToggleFullMode();						
					return 0;
				}
			}
		break;      
		//case MSG_FROM_MRPOPO:
		//     GetMrPoPoHandler()->HandleEvent(wParam, lParam);
		//break;
		case WM_CLOSE:
			{
				if ( !CanExitApp() )
				{
					return 1;
				}

				SUserData*		pUserData		= GetDboGlobal()->GetUserData();
				SConnectData*	pConnectData	= GetDboGlobal()->GetConnectData();

				if( pUserData && pConnectData )
				{
					if( INVALID_HSERVER != pConnectData->sAuthCon.hSocket )
					{
						CLoginPacketGenerator* pLoginPacketGenerator = GetDboGlobal()->GetLoginPacketGenerator(); 
						if( pLoginPacketGenerator )
						{
							if( wcslen(pUserData->wchUserID) > 0 )
								pLoginPacketGenerator->SendLogOutReq(pUserData->wchUserID, false);
						}
					}
				}

				return 0;
			}
	}

	// chatting mode 검사. 
	RwBool bChatMode = FALSE;
	SGameData *pGameData = pDboGlobal->GetGameData();
	bChatMode = pGameData->bChatMode;

	// gui window message 처리.
	LRESULT ret = 0;
	gui::CGUIManager *pGuiMgr = CNtlPLGuiManager::GetInstance()->GetGuiManager();
	if( pGuiMgr && !m_bMouseBackgroundCapture )
		ret = pGuiMgr->Win32EventProc(window, message, wParam, lParam);

	using namespace gui;
#define GET_KEY_REPEAT(lParam)	(lParam & 0x0000ffff)

	// window focus를 빠져나갔을 때.
	if(message == WM_ACTIVATE)
	{
		// Minimize 또는 Active 비활성화
		if(LOWORD(wParam) == WA_INACTIVE || ::IsIconic(window))
		{
			if(GetInputActionMap())
				GetInputActionMap()->Reset();

			if( m_pGamma )
				m_pGamma->RestoreBackGroundGamma();
		}
		// 활성화
		else if( LOWORD(wParam) == WA_ACTIVE )
		{
			if( m_pGamma )
				m_pGamma->SetCurrentGamma();
		}
	}

	
	if( CNtlPLGuiManager::GetInstance()->IsGuiFocus() )
	{
		switch(message)
		{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			// 한글 입력 모드라면 VK_PROCESSKEY 가 전달 된다. 실제 눌러진 가상키는 ImmGetVirtualKey() 함수를 이용하여 한글/영문 입력 모드에 상관없이 
			// 눌러진 가상 키 코드를 조사할 수 있다.
			if( VK_PROCESSKEY == wParam )
			{
				unsigned int uiKey = MapVirtualKey( LOBYTE(HIWORD( lParam ) ), 1 );
				CInputHandler::GetInstance()->KeyDownHandler(uiKey,  (unsigned int)GET_KEY_REPEAT(lParam));
				break;
			}

			CInputHandler::GetInstance()->KeyDownHandler((unsigned int)wParam,  (unsigned int)GET_KEY_REPEAT(lParam));
			break;
		case WM_SYSKEYUP:
		case WM_KEYUP:
			{
				if( VK_PROCESSKEY == wParam )
				{
					unsigned int uiKey = MapVirtualKey( LOBYTE(HIWORD( lParam ) ), 1 );
					CInputHandler::GetInstance()->KeyDownHandler(uiKey,  (unsigned int)GET_KEY_REPEAT(lParam));
					break;
				}

				CInputHandler::GetInstance()->KeyUpHandler((unsigned int)wParam,  (unsigned int)GET_KEY_REPEAT(lParam));
				break;
			}
			break;
		case WM_LBUTTONUP:
			if(m_bMouseBackgroundCapture)
			{
				CInputHandler::GetInstance()->MouseUpHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_LBTN, timeGetTime() - m_dwPrevLMouseDownTime);
				ReleaseCapture();
				m_bMouseBackgroundCapture = FALSE;
			}
			break;
		case WM_RBUTTONUP:
			if(m_bMouseBackgroundCapture)
			{
				CInputHandler::GetInstance()->MouseUpHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_RBTN, timeGetTime() - m_dwPrevRMouseDownTime);
				ReleaseCapture();
				m_bMouseBackgroundCapture = FALSE;
			}
			break;
		case WM_MOUSEWHEEL:
			if(m_bMouseBackgroundCapture)
			{
				CInputHandler::GetInstance()->MouseWheelHandler(LOWORD(wParam), HIWORD(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			break;
		case WM_MOUSEMOVE:
			if(m_bMouseBackgroundCapture)
			{
				CInputHandler::GetInstance()->MouseMoveHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			break;
		}
	}
	else
	{
		switch(message)
		{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			// 한글 입력 모드라면 VK_PROCESSKEY 가 전달 된다. 실제 눌러진 가상키는 ImmGetVirtualKey() 함수를 이용하여 한글/영문 입력 모드에 상관없이 
			// 눌러진 가상 키 코드를 조사할 수 있다.
			if( VK_PROCESSKEY == wParam )
			{
				unsigned int uiKey = MapVirtualKey( LOBYTE(HIWORD( lParam ) ), 1 );
				CInputHandler::GetInstance()->KeyDownHandler(uiKey,  (unsigned int)GET_KEY_REPEAT(lParam));
				break;
			}

			CInputBox::SetEnglishMode();
			CInputHandler::GetInstance()->KeyDownHandler((unsigned int)wParam,  (unsigned int)GET_KEY_REPEAT(lParam));
			break;
		case WM_SYSKEYUP:
		case WM_KEYUP:
			// 한글 입력 모드라면 VK_PROCESSKEY 가 전달 된다. 실제 눌러진 가상키는 ImmGetVirtualKey() 함수를 이용하여 한글/영문 입력 모드에 상관없이 
			// 눌러진 가상 키 코드를 조사할 수 있다.
			if( VK_PROCESSKEY == wParam )
			{
				unsigned int uiKey = MapVirtualKey( LOBYTE(HIWORD( lParam ) ), 1 );
				CInputHandler::GetInstance()->KeyDownHandler(uiKey,  (unsigned int)GET_KEY_REPEAT(lParam));
				break;
			}

			CInputHandler::GetInstance()->KeyUpHandler((unsigned int)wParam,  (unsigned int)GET_KEY_REPEAT(lParam));
			break;
		case WM_LBUTTONDOWN:
			m_bMouseBackgroundCapture = TRUE;
			SetCapture(window);
			CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_LBTN, false ); 
			m_dwPrevLMouseDownTime = timeGetTime();
			break;
		case WM_LBUTTONUP:
			CInputHandler::GetInstance()->MouseUpHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_LBTN, timeGetTime() - m_dwPrevLMouseDownTime);
			ReleaseCapture();
			m_bMouseBackgroundCapture = FALSE;
			break;
		case WM_RBUTTONDOWN:
			m_bMouseBackgroundCapture = TRUE;
			SetCapture(window);
			CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_RBTN, false); 
			m_dwPrevRMouseDownTime = timeGetTime();
			break;
		case WM_RBUTTONUP:
			CInputHandler::GetInstance()->MouseUpHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_RBTN, timeGetTime() - m_dwPrevRMouseDownTime);
			ReleaseCapture();
			m_bMouseBackgroundCapture = FALSE;
			break;
		case WM_LBUTTONDBLCLK:
			CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_LBTN, true); 
			m_dwPrevLMouseDownTime = timeGetTime();
			break;
		case WM_RBUTTONDBLCLK:
			CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_RBTN, true); 
			break;
		case WM_MOUSEWHEEL:
			CInputHandler::GetInstance()->MouseWheelHandler(LOWORD(wParam), HIWORD(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_MOUSEMOVE:
			/*if (m_bMouseBackgroundCapture)
			{
				CInputHandler::GetInstance()->MouseMoveHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}*/
			break;
		default:
			break;
		}
	}

	// chatting mode 검사.
	if(!bChatMode && pGameData->bChatMode)
	{
		if(GetInputActionMap())
			GetInputActionMap()->Reset();
	}

	// lleo52 - 추후 삭제 필요
	{
		struct sMovieInfo
		{
			bool bCanCreate;
			int nX;
			int nY;
			int nWidth;
			int nHeight;
			bool bSync;
			bool bRepeat;
			char szFileName[4096];
		};

		static sMovieInfo sInfo = { false, 0, 0, 200, 200, true, true, ".\\movie\\opening.avi" };

		if ( message == WM_KEYDOWN && wParam == VK_F1 )
		{
			std::ifstream file;

			static char buff[4096];

			file.open( "movieinfo.txt" );

			if ( file.is_open() )
			{
				sInfo.bCanCreate = true;

				file.getline( buff, 4096 );
				sInfo.nX = atoi( buff );

				file.getline( buff, 4096 );
				sInfo.nY = atoi( buff );

				file.getline( buff, 4096 );
				sInfo.nWidth = atoi( buff );

				file.getline( buff, 4096 );
				sInfo.nHeight = atoi( buff );

				file.getline( buff, 4096 );
				sInfo.bSync = atoi( buff ) ? true : false;

				file.getline( buff, 4096 );
				sInfo.bRepeat = atoi( buff ) ? true : false;

				file.getline( buff, 4096 );
				strcpy( sInfo.szFileName, buff );
			}
			else
			{
				sInfo.bCanCreate = false;
			}
		}

		if ( message == WM_KEYDOWN && wParam == VK_F2 )
		{
			if ( sInfo.bCanCreate )
			{
				NTL_MOVIE_HANDLE hHandle = CNtlMovieManager::GetInstance()->CreateMovie( sInfo.bSync, sInfo.nX, sInfo.nY, sInfo.nWidth, sInfo.nHeight );

				CNtlMovieManager::GetInstance()->Open( hHandle, sInfo.szFileName );

				CNtlMovieManager::GetInstance()->Play( hHandle, sInfo.bRepeat );

				g_vecdefMovieList.push_back( hHandle );
			}
		}

		if ( message == WM_KEYDOWN && wParam == VK_F3 )
		{
			if ( !g_vecdefMovieList.empty() )
			{
				std::vector< NTL_MOVIE_HANDLE >::iterator it = --g_vecdefMovieList.end();
				CNtlMovieManager::GetInstance()->DeleteMovie( *it );
				g_vecdefMovieList.erase( it );
			}
		}
	}

	return ret;
}

void CDboApplication::SetRenderEnable(RwBool bRender)
{
	CNtlApplication::SetRenderEnable( bRender );

	if( m_pCamera )
	{
		if (m_pCamera->CameraClear(&m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			if(m_pCamera->MainCameraBeginUpdate(0.f))
			{
				m_pCamera->MainCameraEndUpdate();
			}
			m_pCamera->CameraShowRaster(GetHWnd(), 0);
		}
	}	
}

RwBool CDboApplication::Update(RwReal fTime, RwReal fElapsedTime)
{
	// network process
	// Putting input later due to picking problems.
	// Because of the fast picking process, the atomic that is culled in the rendering is put in the pick atomic container.
	// If the network is processed before the input, the atomic may be deleted if the server deletes the spawn object.

	fElapsedTime = fElapsedTime * g_fElapsedWeightValue;

#ifdef NET_SERVER_SYNC
	CPacketProc::GetInstance()->RecvPacketProc();
#else
	CDboVirtualServer::GetInstance()->Update(fElapsedTime);
#endif

//	EventQueueCurrentProcessFilterMsg(

	API_CoreUpdate(fElapsedTime);

	GetLobbyManager()->Update(fElapsedTime);

	CNtlLoadingController::GetInstance()->Lock();

#ifdef _NTL_PROFILE_ENABLE
	CProfileManager::Increment_Frame_Counter();
#endif

	NTL_SPROFILE("GAME LOGIC")

	m_pVisualManager->UpdateBeforeCamera(fElapsedTime);
	m_pVisualManager->Update(fElapsedTime);

	if(GetRenderEnable())
	{
		if (m_pCamera->CameraClear(&m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			if( m_pCamera->PostCameraBeginUpdate(fElapsedTime) )
			{
				if(!IsIconic(m_hWnd))
				{
					m_pVisualManager->Render(); 
				}

				RwD3D9SetVertexShader(NULL);
				RwD3D9SetPixelShader(NULL);

				m_pCamera->PostCameraEndUpdate();
			}
			else 
			{
				GetNtlGuiManager()->Update( fElapsedTime );
			}

			m_pCamera->UpdatePostEffectFilter(); 
			m_pCamera->UpdateHeatHaze(fElapsedTime);
			m_pCamera->UpdateMotionBlur(fElapsedTime);

			if( m_pCamera->MainCameraBeginUpdate(fElapsedTime) )
			{
				if(!IsIconic(m_hWnd))
				{
					m_pVisualManager->PostRender();
					m_pStageMgr->PostRender();
					DisplayInfo();

					DisplayFps();
				}			

				m_pCamera->MainCameraEndUpdate();
			}

			// lleo52 - 추후 제거 필요
			std::vector< NTL_MOVIE_HANDLE >::iterator it = g_vecdefMovieList.begin();
			for ( ; it != g_vecdefMovieList.end(); ++it )
			{
				CNtlMovieManager::GetInstance()->Render( *it );
			}

			m_pCamera->CameraShowRaster(GetHWnd(), 0);
		}
	}

	API_GetSLPacketLockManager()->Update(fElapsedTime);	

	if( GetMoviePlayer() )
		GetMoviePlayer()->Render();

	CNtlLoadingController::GetInstance()->Unlock();

	//프로파일
	//CProfileIterator* pThe = CProfileManager::Get_Iterator();
	//CProfileManager::Release_Iterator(pThe);

	// update discord
#ifdef USE_DISCORD
	GetDiscordManager()->UpdateDiscordConnection();
#endif // USE_DISCORD

	m_fLastClientInfoUpdate += fElapsedTime;
	if (m_fLastClientInfoUpdate >= 110.f) // send every 110 seconds
	{
		m_fLastClientInfoUpdate = 0.f;

		DBOTIME curTime = time(NULL);

		bool bSpeedHack = false;
		if (m_lastClientInfoSent != 0)
		{
 		//	DBO_WARNING_MESSAGE("curTime: " << curTime << " m_lastClientInfoSent: " << m_lastClientInfoSent << " total: " << curTime - m_lastClientInfoSent);
			if (curTime - m_lastClientInfoSent < 108) // check if we send packet faster than every 108 seconds (we are supposed to send it every 110 seconds)
				bSpeedHack = true;
		}

		m_lastClientInfoSent = curTime;

		SConnectData* pConnectData = GetDboGlobal()->GetConnectData();
		if (pConnectData->sGameCon.hSocket != INVALID_SOCKET)
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendClientInfoNfy(bSpeedHack); // send packet
		}
	}



	NTL_RPROFILE(TRUE)
}

void CDboApplication::Destroy()
{
	NTL_FUNCTION("CDboApplication::Destroy" );	

	// lleo52 - 추후 제거 필요
	CNtlMovieManager::DeleteInstance();

	gui::CStringLocalConverter::DestroyInstance();

#ifdef USE_DISCORD
	CDiscord::DestroyInstance();
#endif

	CPacketTracer::DeleteInstance();			/// woosungs_test 20090804

	CMoviePlayer::DestoryInstance();

	// Destroy mouse double click time
	API_DestroyDoubleClickInterval();

	//------------------------------------------------------------------------------------
	// client layer
	//------------------------------------------------------------------------------------

    CDBOAddinManager::ShutDown();

	// debug console command manager
	if(m_pDumpManager)
	{
		NTL_DELETE(m_pDumpManager);
		m_pDumpManager = NULL;
	}

	// stage manager destroy
	if(m_pStageMgr)
	{
		m_pStageMgr->Destroy();
		NTL_DELETE(m_pStageMgr);
		m_pStageMgr = NULL;
	}

	CTeleportProposalManager::DeleteInstance();
	CMsgBoxManager::DeleteInstance();

	// dbo global destroy
	m_DboGlobal.Destroy();

	// cursor destroy
	GetCursorManager()->DeleteInstance();

	// dbo event handler destroy
	if(m_pDboEventHandler)
	{
		m_pDboEventHandler->Destroy(); 
		NTL_DELETE(m_pDboEventHandler);
	}

	CLobbyManager::DestoryInstance();

	//------------------------------------------------------------------------------------
	// simulation layer
	//------------------------------------------------------------------------------------

	DeleteNtlSLCrowdManager();

	API_SLTerminate();

	//------------------------------------------------------------------------------------
	// presentation layer
	//------------------------------------------------------------------------------------

	// GlobalGui Destroy
	gui::CGlobalComponent::GetInstance()->DeleteComponent();

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

	// property를 destroy 한다.
	CNtlPLPropertyContainer::GetInstance()->Destroy();

	// API_PLInit
	UnLinkTextureLoad();

#ifdef USE_PACK_DATA
	UnLinkGuiScriptLoad();
	UnLinkFlashResourceLoad();
	UnLinkLoadSound_from_Memory();
	UnLink_IsExistSoundFile();
	UnLinkLoadEmblemStuff();
	UnLinkHtmlScriptLoad();
	UnLinkLoadContract();
#endif

	API_PLTerminate();

	if(m_pPLEventHandler)
	{
		m_pPLEventHandler->Destroy();
		NTL_DELETE(m_pPLEventHandler);
	}

	// gui
	gui::CSetupGUI::Deinit();

	// sound 해제
	GetSoundManager()->Release();

	// Gamma Controller
	if( m_pGamma )
	{
		m_pGamma->Destroy();
		NTL_DELETE( m_pGamma );
	}

	//------------------------------------------------------------------------------------
	// core layer
	//------------------------------------------------------------------------------------
	API_CoreTerminate();

	CPetitionManager::DestroyInstance();

	// Close the event system
	RWS::CEventHandler::UnRegisterStreamChunkHandlers();
	RWS::CEventHandler::Close ();

	CNtlApplication::Destroy();

	NTL_RETURNVOID();
}

void CDboApplication::DisplayInfo()
{
	NTL_SPROFILE("CDboApplication::DisplayInfo")

	if(!Logic_IsDevUser())
	{
		NTL_RPROFILE_VOID()
	}

	if(!GetDboGlobal()->IsDebugDisplay())
	{
		NTL_RPROFILE_VOID()
	}

	RwChar caption[256];
	RwInt32 iIdx = 0;
	//fps
	RsSprintf(caption, RWSTRING("FPS: %4.2f"), GetFps());
	RsCharsetPrint(m_pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

	//window size
	RsSprintf(caption, RWSTRING("Window Width : %d, Height : %d"), GetScreenWidth(), GetScreenHeight());
	RsCharsetPrint(m_pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

	static unsigned __int64	dwAvailPhys, dwTotalPhys, dwAvailVirtual,dwTotalVirtual, dwAvailPageFile, dwTotalPageFile;
	static DWORD			dwAvailVideo, dwTotalVideo;
	static DWORD			dwDataPack = (1024 * 1024); // MB
	static RwReal			fMemoryCheck = 0.0f;

	fMemoryCheck -= g_GetElapsedTime();
	if (fMemoryCheck < 0.0f)
	{
		GetSystemMemorySize(&dwAvailPhys, &dwTotalPhys, &dwAvailVirtual, &dwTotalVirtual, &dwAvailPageFile, &dwTotalPageFile);
		GetVideoMemorySize(&dwAvailVideo, &dwTotalVideo);
		fMemoryCheck = 0.5f;		
	}

	RsSprintf(caption, RWSTRING("Phys : %I64u/%I64uMB"), (dwTotalPhys - dwAvailPhys) / dwDataPack, dwTotalPhys / dwDataPack);
	RsCharsetPrint(m_pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

	RsSprintf(caption, RWSTRING("Virtual : %I64u/%I64uMB"), (dwTotalVirtual - dwAvailVirtual) / dwDataPack, dwTotalVirtual / dwDataPack);
	RsCharsetPrint(m_pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

	RsSprintf(caption, RWSTRING("PageFile : %I64u/%I64uMB"), (dwTotalPageFile - dwAvailPageFile) / dwDataPack, dwTotalPageFile / dwDataPack);
	RsCharsetPrint(m_pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

	RsSprintf(caption, RWSTRING("Video : %u/%uMB"), (dwTotalVideo - dwAvailVideo) / dwDataPack, dwTotalVideo / dwDataPack);
	RsCharsetPrint(m_pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

// 	RsSprintf(caption, RWSTRING("CullTestCnt:%u"), CNtlPLGlobal::m_uiCullTestCnt);
// 	RsCharsetPrint(m_pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);
// 
// 	RsSprintf(caption, RWSTRING("CullTestAtomicCnt:%u"), CNtlPLGlobal::m_uiCullTestAtomicCnt);
// 	RsCharsetPrint(m_pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);
// 
// 	RsSprintf(caption, RWSTRING("RenderAtomicCnt:%u"), CNtlPLGlobal::m_uiRenderAtomicCnt);
// 	RsCharsetPrint(m_pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

	CNtlCharsetPrint *pCharsetPrint = CNtlCharsetPrint::GetInstance(); 
	RwInt32 iSetCount = pCharsetPrint->GetCount(); 
	for(RwInt32 i = 0; i < iSetCount; i++)
	{
		if(pCharsetPrint->IsValid(i))
		{
			RsCharsetPrint(m_pCharset, pCharsetPrint->GetString(i), 0, iIdx++, rsPRINTPOSTOPRIGHT);
		}
	}

	NTL_EPROFILE()
}

void CDboApplication::DisplayFps()
{
	if (IsShowFPS() == false)
	{
		return;
	}

	RwChar caption[256];

	//fps
	RsSprintf(caption, RWSTRING("FPS: %4.2f"), GetFps());
	RsCharsetPrint(m_pCharset, caption, -22, 0, rsPRINTPOSTOPRIGHT);
}

// 아이콘 사용 때문에 오버라이딩 한다. (by agebreak)
bool CDboApplication::RegisterMainWindowClass( HANDLE processInstance ) 
{
	WNDCLASS windowClass;

#ifdef UNDER_CE
	windowClass.style = CS_DBLCLKS;
#else
	windowClass.style = CS_BYTEALIGNWINDOW | CS_DBLCLKS;
#endif
	windowClass.lpfnWndProc = (WNDPROC)MainWndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = (HINSTANCE)processInstance;
	windowClass.hIcon = LoadIcon((HINSTANCE)processInstance, MAKEINTRESOURCE(IDI_ICON1));
	windowClass.hCursor = LoadCursor(0, IDC_ARROW);
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = kMainWindowClassName;

	return ( RegisterClass(&windowClass) != 0 );
}

void CDboApplication::SetGammaRamp( RwReal fGamma )
{
	if( m_pGamma )
		m_pGamma->SetGamma( fGamma );
}

RwBool CDboApplication::CanExitApp( void )
{
	// Work thread
	// - loading thread 가 존재하면 아직 Thread가 데이터를 로딩중이므로 로딩을
	//   완료후 종료 시킨다
	return g_clWorkThreadMgr.GetThreadCnt() == 0 ? TRUE : FALSE;
}

RwBool CDboApplication::Create( HINSTANCE hInstance, RwInt32 posX, RwInt32 posY, RwInt32 sizeX, RwInt32 sizeY, RwBool bFullScreen )
{
	// Application을 세팅하기 위하여 System의 Option을 읽어들인다.
	Logic_LoadSystemOption();

	// Debug mode creates a window by referring to the bFullScreen variable that calls this function in WinMain.
#ifndef NDEBUG
	RwInt32 nPosX = 0;
	RwInt32 nPosY = 0;

	RwInt32 nSystemWidth = GetSystemMetrics( SM_CXSCREEN );
	RwInt32 nSystemHeight = GetSystemMetrics( SM_CYSCREEN );
	if( nSystemWidth > sizeX )
		nPosX = ( nSystemWidth - sizeX ) / 2 ;

	if( nSystemHeight > sizeY )
		nPosY = ( nSystemHeight - sizeY ) / 2 ;

	return CNtlApplication::Create( hInstance, nPosX, nPosY, sizeX, sizeY, bFullScreen );
#else
	// Release In the version, the option is read and applied on the user's computer.
	
	RwBool bUserScreen = GetNtlStorageManager()->GetBoolData( dSTORAGE_GRAPHIC_WINDOW_MODE );
	RwInt32 nScreenWidth = GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH );
	RwInt32 nScreenHeight = GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT );
	RwInt32 nScreenDepth = GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_SELECT_VIDEODEPTH );

	// 창 모드라면 화면을 중간으로 세팅한다.
	RwInt32 nPosX = 0;
	RwInt32 nPosY = 0;
	if( bUserScreen )
	{
		RwInt32 nSystemWidth = GetSystemMetrics( SM_CXSCREEN );
		RwInt32 nSystemHeight = GetSystemMetrics( SM_CYSCREEN );
		if( nSystemWidth > nScreenWidth )
			nPosX = ( nSystemWidth - nScreenWidth ) / 2 ;

		if( nSystemHeight > nScreenHeight )
			nPosY = ( nSystemHeight - nScreenHeight ) / 2 ;
	}

	return CNtlApplication::Create( hInstance, nPosX, nPosY, nScreenWidth, nScreenHeight, !bUserScreen );
#endif
}