#include "rpplugin.h"

#include "StdAfx.h"
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <zmouse.h>
#include "NtlDebug.h"
#include "NtlText.h"
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
#include <direct.h>
#include <io.h>
#include <RpMatFx.h>
#include <RpUVAnim.h>
#include <rppvs.h>
#include <rpltmap.h>
#include <rprandom.h>

#include "NtlCoreUtil.h"

#include "NtlWEApplication.h"
#include "NtlProfiler.h"
#include "TableContainer.h"
#include "NtlPLSceneManagerFactory.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLApi.h"
#include "InputHandler.h"
#include "NtlWorldFieldManager.h"
#include <ntlworldinterface.h>
#include <ntlworldplugin.h>
#include <ntlplworldentity.h>
#include <NtlWorldMaterialPlugin.h>
#include <NtlPLSceneManager.h>
#include "ntlatomic.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlCoreUtil.h"
#include "NtlPLMaterialExtWrapper.h"
#include "NtlPLEventHandler.h"
#include "NtlPLEventGenerator.h"
#include "NtlSoundManager.h"
#include "NtlCameraTop.h"
#include "NtlWe.h"
#include "NtlWEFrm.h"
#include "CameraAttrDlg.h"
#include "NtlPLObject.h"
#include "NtlWeControlUi.h"
#include "NtlWeAvatarController.h"
#include "NtlCoreApi.h"
#include "NtlSLApi.h"
#include "MiniMapManager.h"
#include "SpawnManager.h"
#include "gui_guimanager.h"
#include "gui_staticbox.h"
#include "gui_fontmanager.h"
#include "NtlWorldSectorPVS.h"
#include "NtlPLGuiManager.h"
#include "NtlBugTrap.h"
#include "NtlPLWeatherHandler.h"
#include "NtlPLResourceManager.h"
#include "WorldViewDlg.h"
#include "NtlWEDoc.h"
#include "NtlWorldLTManager.h"
#include "NtlWorldRpLightManager.h"
#include "BookmarkDlg.h"
#include "NtlSoundMoodManager.h"
#include "NtlPLResourcePack.h"


CNtlWEApplication::CNtlWEApplication()
{
	m_BackgroundColor.red   = 150;
	m_BackgroundColor.green = 150;
	m_BackgroundColor.blue  = 150;
	m_BackgroundColor.alpha = 0;

	m_ForegroundColor.red   = 0;
	m_ForegroundColor.green = 0;
	m_ForegroundColor.blue  = 0;
	m_ForegroundColor.alpha = 255;
	
	m_pVisualManager	= NULL;
	m_pCamera			= NULL;
	m_pCameraTop		= NULL;
	m_pWorldEntity		= NULL;
	m_pEventHandler		= NULL;	

	// do only once
	_getcwd(dGET_WORLD_PARAM()->CurWorkingFolderName, 256);
	strcpy(dGET_WORLD_PARAM()->CurDefaultTexPath, dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcat(dGET_WORLD_PARAM()->CurDefaultTexPath, "\\texture\\ntlwe\\");

	m_handleRestore	= CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRestore(this, &CNtlWEApplication::RestoreHandler);

	CTime time = CTime::GetCurrentTime(); 
	m_LogFilePath = dGET_WORLD_PARAM()->CurWorkingFolderName;
	m_LogFilePath += "\\TOOL\\SABER\\LOG\\";
	m_LogFilePath += time.Format("%y-%m-%d-%H-%M-%S");
	m_LogFilePath += "L.log";
	::NtlLogFilePath(m_LogFilePath.GetBuffer());
	m_TraFilePath = dGET_WORLD_PARAM()->CurWorkingFolderName;
	m_TraFilePath += "\\TOOL\\SABER\\LOG\\";
	m_TraFilePath += time.Format("%y-%m-%d-%H-%M-%S");
	m_TraFilePath += "T.log";
	::NtlTraceInitFilePath(m_TraFilePath.GetBuffer());

	::NtlLogFilePrint(" === < SABER > Log Start === ");
	::NtlTraceFilePrint(" === < SABER > Trace Start === ");
}

CNtlWEApplication::~CNtlWEApplication()
{
	::NtlLogFilePrint(" === < SABER > Log End === ");
	::NtlTraceFilePrint(" === < SABER > Trace End === ");

	CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRestore(m_handleRestore);
}

RwBool CNtlWEApplication::RestoreHandler()
{
	switch (m_pWorldEntity->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		{
			if (m_pEventHandler)
			{
				m_pEventHandler->Restore();

				// force 2 free camera
				CNtlWeControlUi::GetInstance().SetActiveWeController(m_pCamera);
				dGETFRM()->m_pCameraPortal->SetCurCamMode(0);
			}
		}
		break;
		// 인도어
	case AW_RWWORLD:
		{
			if (m_pEventHandler)
			{
				m_pEventHandler->Restore();

				// force 2 free camera
				CNtlWeControlUi::GetInstance().SetActiveWeController(m_pCamera);
				dGETFRM()->m_pCameraPortal->SetCurCamMode(0);
			}
		}
	}
	

	//gui::CGUIManager::SetViewSize(static_cast<INT>(GetScreenWidth()), static_cast<INT>(GetScreenHeight()));

	return TRUE;
}

void CNtlWEApplication::Switch(eAPP_ENTITY OptionID)
{
	switch(OptionID)
	{
	case eAO_FOG:
		{
			m_pVisualManager->GetFog()->Switch();
			break;
		}
	case eAO_BLOCK_GUIDE_LINE:
		{
			dGET_WORLD_PARAM()->BlockGuideLine = !dGET_WORLD_PARAM()->BlockGuideLine;
			break;
		}
	case eAO_FIELD_GUIDE_LINE:
		{
			dGET_WORLD_PARAM()->FieldGuideLine = !dGET_WORLD_PARAM()->FieldGuideLine;
			break;
		}
	case eAO_SECTOR_GUIDE_LINE:
		{
			dGET_WORLD_PARAM()->SectorGuideLine = !dGET_WORLD_PARAM()->SectorGuideLine;
			break;
		}
	default:
		{
			break;
		}
	}
}

RwBool CNtlWEApplication::IsWorking(eAPP_ENTITY OptionID)
{
	switch(OptionID)
	{
	case eAO_FOG:
		{
			return m_pVisualManager->GetFog()->IsWorking();
		}
	case eAO_FIELD_GUIDE_LINE:
		{
			return dGET_WORLD_PARAM()->FieldGuideLine;
		}
	case eAO_SECTOR_GUIDE_LINE:
		{
			return dGET_WORLD_PARAM()->SectorGuideLine;
		}
	}

	return FALSE;
}

bool CNtlWEApplication::IsWorking()
{
	if(!m_pWorldEntity)
		return false;

	return m_pWorldEntity->IsWorking();
}

bool CNtlWEApplication::CreateWorldFromFolder(string& FullPathFileName)
{
	return CreateWorldFromFolderEx(FullPathFileName);
	// path engine collision mesh
	CNtlPLGlobal::m_bCollObjVisible = FALSE;
	//080716 : Cz
	//CNtlPLObject::m_bLoadCollisionMesh = TRUE;
	CNtlPLGlobal::m_bCharacterUpdateVisible = TRUE;

	// destroy property container
	m_PropContainter.GetInstance()->Destroy();

	// destroy camera
	if(m_pCamera)
	{
		m_pCamera->Destroy();
		NTL_DELETE(m_pCamera);
		m_pCamera = NULL;
	}

	if(m_pCameraTop)
	{
		//m_pCamera->Destroy();
		NTL_DELETE(m_pCamera);
		m_pCamera = NULL;
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	m_pWorldEntity->Destroy();

	// set resource folder structures
	strcpy(dGET_WORLD_PARAM()->WorldProjectFolderName, FullPathFileName.c_str());
	strcat(dGET_WORLD_PARAM()->WorldProjectFolderName, "\\");
	if (_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName) == -1)
	{
		::AfxMessageBox("WorldProjectFolderName Error", MB_OK);
		return FALSE;
	}

	RwChar cTemp[512];
	sprintf_s(cTemp, 512, "SABER(%s-%s)-%s : Nothing is more despicable than respect based on fear-Albert Camus.", dNTL_WORLD_VERSION, dNTL_WORLD_TOOL_MODE, dGET_WORLD_PARAM()->WorldProjectFolderName);
	dGETFRM()->SetWindowText(cTemp);

	// create a global world param
	if(FILE* pFile = fopen("#######.gwp", "rb"))
	{
		LoadFileWorldState(dGET_WORLD_PARAM(), pFile);
		fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	// create world
	RwV3d SPos = {0.0f, 0.0f, 0.0f};
	m_pWorldEntity->CreateWorld(SPos);

	// free camera
	m_pCamera = NTL_NEW CNtlGameCamera;
 	CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_HDR);
	m_pCamera->Create(CNtlApplication::GetScreenWidth(), CNtlApplication::GetScreenHeight(), TRUE, dGET_WORLD_PARAM()->WorldNearPlane, dGET_WORLD_PARAM()->WorldFarPlane);

	RwV3d Eye = {0.0f, 8.0f, 0.0f};
	RwV3d LookAt = {0.0f, 0.0f, 0.0f};

	RwFrame *pCameraFrame = RwCameraGetFrame(m_pCamera->GetCamera());

	RwMatrix *CamMat = RwFrameGetMatrix(pCameraFrame);
	LookAt = *RwMatrixGetAt(CamMat);
	RwFrameTranslate(pCameraFrame, &Eye, rwCOMBINEPOSTCONCAT);

	CNtlPLGlobal::SetActiveCamera(m_pCamera->GetCamera());

	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);
	

	// top view camera
	m_pCameraTop = NTL_NEW CNtlCameraTop;
	m_pCameraTop->SetCamera(m_pCamera->GetCamera());

	// tool event handler
	// 인도어
	//switch (m_pWorldEntity->GetActiveWorldType())
	//{
	//case AW_HEGITHFIELD:
		m_pEventHandler = NTL_NEW CWorldEditorEventHander(GetScreenWidth(), GetScreenHeight());
		m_pEventHandler->Create();
		m_pEventHandler->LoadConfig();
	//	break;
	//case AW_RWWORLD:
	//	m_pEventHandlerIndoor = NTL_NEW CWorldEditorEventHanderIndoor();
	//	break;
	//}

	// sound
	//GetSoundManager()->Init(".\\sound\\", 1.0f, 1.0f, 1.0f, 0.5f);

#ifdef WE_CHARACTER_VIEW

	//Character View(By HoDong)
	CNtlWeAvatarController::GetInstance().DestroyControl();
	CNtlWeAvatarController::GetInstance().CreateControl(m_pCamera, m_pVisualManager);
	CNtlWeControlUi::GetInstance().SetActiveWeController(m_pCamera);

	dGET_WORLD_PARAM()->WorldCamSpeedPerSec = 100.0f;
	CNtlWeControlUi::GetInstance().SetFov(60.f);
	CNtlWeControlUi::GetInstance().SetSpeed(dGET_WORLD_PARAM()->WorldCamSpeedPerSec);
	CNtlWeControlUi::GetInstance().SetZoomDist(15.0f);

#endif

	return true;
}

RwBool CNtlWEApplication::OnIdle()
{ 
	if(!m_pWorldEntity || !m_pWorldEntity->GetWorldFieldMgr())
	{
		return CNtlApplication::Update();
	}

#ifdef dNTL_WORLD_TOOL_MODE

		if (m_pEventHandler)
		{
			m_pEventHandler->Update();
		}		
	
#endif

	return CNtlApplication::Update();
}

VOID CNtlWEApplication::CreateDefaultPVSFile()
{
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);

	RwBool					PVSFlag	= FALSE;
	CNtlWorldFieldManager*	pFMgr	= m_pWorldEntity->GetWorldFieldMgr();

	if(FILE* pFile = fopen("-------", "wb"))
	{
		BYTE PVSFlag = 0;
		for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum; ++i)
		{
			fwrite(&PVSFlag, sizeof(BYTE), 1, pFile);
		}

		fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
}

bool CNtlWEApplication::CreateWorld(string& FullPathFileName)
{
	return CreateWorldEx(FullPathFileName);

	CNtlPLGlobal::m_bCollObjVisible = FALSE;
	//080716 : Cz
	//CNtlPLObject::m_bLoadCollisionMesh = TRUE;
	CNtlPLGlobal::m_bCharacterUpdateVisible = TRUE;

	// destroy property container
	m_PropContainter.GetInstance()->Destroy();

	// destroy camera
	if(m_pCamera)
	{
		m_pCamera->Destroy();
		NTL_DELETE(m_pCamera);
		m_pCamera = NULL;
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	if(m_pCamera)
	{
		m_pCamera->Destroy();
		NTL_DELETE(m_pCamera);
		m_pCamera = NULL;
	}

	m_pWorldEntity->Destroy();

	// set resource folder structures
	strcpy(dGET_WORLD_PARAM()->WorldProjectFolderName, FullPathFileName.c_str());
	strcat(dGET_WORLD_PARAM()->WorldProjectFolderName, "\\");
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);

	_mkdir("fields");
	_mkdir("swap");

	// create a global world param
	if(FILE* pFile = fopen("#######.gwp", "wb"))
	{
		// default zero
		dGET_WORLD_PARAM()->WorldObjectSerialID = 1;
		dGET_WORLD_PARAM()->WorldTriggerSerialID = 1;
		SaveFileWorldState(dGET_WORLD_PARAM(), pFile);
		fclose(pFile);
	}

	if(FILE* pFile = fopen("-------", "wb"))
	{
		BYTE PVSFlag = 0;
		for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum; ++i)
		{
			fwrite(&PVSFlag, sizeof(BYTE), 1, pFile);
		}

		fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	RwV3d SPos = {0.0f, 0.0f, 0.0f};
	m_pWorldEntity->CreateWorld(SPos);

	// free camera
	m_pCamera = NTL_NEW CNtlGameCamera;
	CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_HDR);
	m_pCamera->Create(CNtlApplication::GetScreenWidth(), CNtlApplication::GetScreenHeight(), TRUE, dGET_WORLD_PARAM()->WorldNearPlane, dGET_WORLD_PARAM()->WorldFarPlane);
	
	RwV3d Eye = {0.0f, 8.0f, 0.0f};
	RwV3d LookAt = {0.0f, 0.0f, 0.0f};

	RwFrame *pCameraFrame = RwCameraGetFrame(m_pCamera->GetCamera());

	RwMatrix *CamMat = RwFrameGetMatrix(pCameraFrame);
	LookAt = *RwMatrixGetAt(CamMat);
	RwFrameTranslate(pCameraFrame, &Eye, rwCOMBINEPOSTCONCAT);

	CNtlPLGlobal::SetActiveCamera(m_pCamera->GetCamera()); 

	switch (m_pWorldEntity->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		CNtlPLGlobal::SetActiveWorld(m_pWorldEntity->GetWorld()); 
		break;
	}

	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

	// top view camera
	m_pCameraTop = NTL_NEW CNtlCameraTop;
	m_pCameraTop->SetCamera(m_pCamera->GetCamera());

	// tool event handler
	// 인도어
// 	switch (m_pWorldEntity->GetActiveWorldType())
// 	{
// 	case AW_HEGITHFIELD:
		m_pEventHandler = NTL_NEW CWorldEditorEventHander(GetScreenWidth(), GetScreenHeight());
		m_pEventHandler->Create();
//		break;
// 	case AW_RWWORLD:
// 		m_pEventHandlerIndoor = NTL_NEW CWorldEditorEventHanderIndoor();
// 		break;
// 	}

	//GetSoundManager()->Init(".\\sound\\", 1.0f, 1.0f, 1.0f, 0.5f);

	// create a new PVS data
	//CreateDefaultPVSFile();

#ifdef WE_CHARACTER_VIEW
	//Character View(By HoDong)
	CNtlWeAvatarController::GetInstance().DestroyControl();
	CNtlWeAvatarController::GetInstance().CreateControl(m_pCamera, m_pVisualManager);
	CNtlWeControlUi::GetInstance().SetActiveWeController(m_pCamera);

	dGET_WORLD_PARAM()->WorldCamSpeedPerSec = 100.0f;
	CNtlWeControlUi::GetInstance().SetFov(60.f);
	CNtlWeControlUi::GetInstance().SetSpeed(dGET_WORLD_PARAM()->WorldCamSpeedPerSec);
	CNtlWeControlUi::GetInstance().SetZoomDist(15.0f);

#endif
	return true;
}

void CNtlWEApplication::CreateWorldDefaultParam()
{
	strcpy_s(dGET_WORLD_PARAM()->WorldVer,		10, dNTL_WORLD_VERSION);
	strcpy_s(dGET_WORLD_PARAM()->WorldSaveVer,	10, dNTL_WORLD_VERSION);
	strcpy_s(dGET_WORLD_PARAM()->WorldLoadVer,	10, dNTL_WORLD_VERSION);

	dGET_WORLD_PARAM()->WorldHeightfieldNum = 512;
	dGET_WORLD_PARAM()->WorldSectorTileSize = 4;
	dGET_WORLD_PARAM()->WorldSectorTileNum	= 32;
	dGET_WORLD_PARAM()->WorldFieldSectorNum = 2;
	dGET_WORLD_PARAM()->WorldPolyCnt		= 0;
	dGET_WORLD_PARAM()->WorldSectorVertNum	= dGET_WORLD_PARAM()->WorldSectorTileNum + 1;
	dGET_WORLD_PARAM()->WorldSectorNum		= dGET_WORLD_PARAM()->WorldHeightfieldNum / dGET_WORLD_PARAM()->WorldSectorTileNum;
	dGET_WORLD_PARAM()->WorldSectorSize		= dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileSize;
	dGET_WORLD_PARAM()->WorldSectorHalfNum	= dGET_WORLD_PARAM()->WorldSectorNum / 2;
	dGET_WORLD_PARAM()->WorldSectorPolyNum	= dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum * 2;
	dGET_WORLD_PARAM()->WorldSize			= dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorSize;
	dGET_WORLD_PARAM()->WorldSizeHalf		= dGET_WORLD_PARAM()->WorldSize / 2;
	dGET_WORLD_PARAM()->WorldValueMax		= +(dGET_WORLD_PARAM()->WorldSizeHalf);
	dGET_WORLD_PARAM()->WorldValueMin		= -(dGET_WORLD_PARAM()->WorldSizeHalf);
	dGET_WORLD_PARAM()->WorldFieldSize		= dGET_WORLD_PARAM()->WorldSectorSize * dGET_WORLD_PARAM()->WorldFieldSectorNum;
	dGET_WORLD_PARAM()->WorldFieldNum		= dGET_WORLD_PARAM()->WorldSize / dGET_WORLD_PARAM()->WorldFieldSize;
	dGET_WORLD_PARAM()->WorldFieldHalfNum	= dGET_WORLD_PARAM()->WorldFieldNum / 2;
	dGET_WORLD_PARAM()->WorldTexAddr		= 32;
	dGET_WORLD_PARAM()->WorldNearPlane		= 0.2f;
	dGET_WORLD_PARAM()->WorldFarPlane		= 512.0f;
	dGET_WORLD_PARAM()->WorldCamSpeedPerSec = 150.0f;
	dGET_WORLD_PARAM()->IdxSkyType			= static_cast<RwInt32>(eSTI_HEMISPHERE);

	dGET_WORLD_PARAM()->LODEnable				= FALSE;		
	dGET_WORLD_PARAM()->NumBrushRadiusInTile	= 3.0f;

	dGET_WORLD_PARAM()->ValHeightfieldSensitivity		= 0.1f;
	dGET_WORLD_PARAM()->ValTextureSensitivity			= 0.2f;
	dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity	= 0.3f;

	dGET_WORLD_PARAM()->BlockGuideLine			= FALSE;
	dGET_WORLD_PARAM()->FieldGuideLine			= FALSE;
	dGET_WORLD_PARAM()->SectorGuideLine			= FALSE;
	dGET_WORLD_PARAM()->FlgHeightValueShow		= FALSE;
	dGET_WORLD_PARAM()->FlgObjInfoShow			= TRUE;
	dGET_WORLD_PARAM()->ClrDayAndNight			= 0x00000000;
	dGET_WORLD_PARAM()->ClrDiffuserPicker		= 0x00000000;
	dGET_WORLD_PARAM()->ClrBaseAmbient[0]		= 100;
	dGET_WORLD_PARAM()->ClrBaseAmbient[1]		= 100;
	dGET_WORLD_PARAM()->ClrBaseAmbient[2]		= 100;
	dGET_WORLD_PARAM()->ClrShadow				= 0xff777777;

	// 인도어
	dGET_WORLD_PARAM()->WorldBlockSize			= 64;
	dGET_WORLD_PARAM()->WorldBlockNum			= dGET_WORLD_PARAM()->WorldSize / dGET_WORLD_PARAM()->WorldBlockSize;
	dGET_WORLD_PARAM()->WorldType				= 0;
	
}

void CNtlWEApplication::SetErrorReport()
{
	// Dx에서 PC관련 정보를 가져와서 txt파일을 만든다.
	//ShellExecute(NULL, "open", "dxdiag.exe", "/t DxLog.txt", NULL, SW_SHOWNORMAL);

	// 버그트랩용 설정    
	char szVer[32] = {0,};
	//sprintf_s(szVer, "%d.%d", CLIENT_LVER, CLIENT_RVER);
	sprintf_s(szVer, "1.0");

	BT_SetAppName(_T("SABER"));         
	BT_SetAppVersion(szVer);    
	BT_SetFlags(BTF_DETAILEDMODE | BTF_SCREENCAPTURE);
	BT_SetSupportServer("211.169.249.71", 9999);
	
	BT_AddLogFile(m_LogFilePath.GetBuffer());
	BT_AddLogFile(m_TraFilePath.GetBuffer());
}

RwBool CNtlWEApplication::CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
{
	// profile
	NTL_FUNCTION("CNtlWEApplication::CreateSubSystem" );

	// Bug Trap
	SetErrorReport();

	// implement base class

	CNtlApplication::CreateSubSystem(iWidth, iHeight, zBuffer);
	
	// global initialization
	API_PLInit();
	
	// open the event handler module
	RWS::CEventHandler::Open();
	RWS::CEventHandler::RegisterStreamChunkHandlers();

	LinkTextureLoad(ResourcPackTextureLoad);
	GetNtlResourcePackManager()->LoadPackHeader();
	GetNtlResourcePackManager()->SetActiveTypeFlags(NTL_PACK_TYPE_FLAG_TEXTURE | NTL_PACK_TYPE_FLAG_OBJECT |
		/*NTL_PACK_TYPE_FLAG_TERRAIN |*/ NTL_PACK_TYPE_FLAG_GUI |
		NTL_PACK_TYPE_FLAG_FLASH | NTL_PACK_TYPE_FLAG_LANGUAGE |
		NTL_PACK_TYPE_FLAG_TABLE | NTL_PACK_TYPE_FLAG_SCRIPT | NTL_PACK_TYPE_FLAG_PROPERTY);

	// PL event handler
	m_pPLEventHandler = NTL_NEW CNtlPLEventHandler;
	if(!m_pPLEventHandler->Create())
	{
		NTL_RETURN(FALSE);
	}

	// character set
	m_pCharset = RtCharsetCreate(&m_ForegroundColor, &m_BackgroundColor);
    if(!m_pCharset)
		NTL_RETURN(FALSE);


	// visual manager
	m_pVisualManager = NTL_NEW CNtlPLVisualManager;
	if(!m_pVisualManager->Create())
		NTL_RETURN(FALSE);

	// active scene manager setting
	CNtlPLSceneManagerFactory::ActiveSceneManager(m_pVisualManager);

	// create world terrain
	CreateWorldDefaultParam();

	m_pWorldEntity = static_cast<CNtlPLWorldEntity*>(m_pVisualManager->CreateEntity(PLENTITY_WORLD, "NtlWorld"));
	DBO_ASSERT(m_pWorldEntity, "Entity create failed.");

	// free camera
	m_pCamera = NTL_NEW CNtlGameCamera;
	if(!m_pCamera->Create(iWidth, iHeight, zBuffer, dGET_WORLD_PARAM()->WorldNearPlane, dGET_WORLD_PARAM()->WorldFarPlane))
		NTL_RETURN(FALSE);

	RwV3d Eye = {0.0f, 4.0f, 0.0f};
	RwV3d LookAt = {0.0f, 0.0f, 0.0f};

	RwFrame *pCameraFrame = RwCameraGetFrame(m_pCamera->GetCamera());

	RwMatrix *CamMat = RwFrameGetMatrix(pCameraFrame);
	LookAt = *RwMatrixGetAt(CamMat);
	RwFrameTranslate(pCameraFrame, &Eye, rwCOMBINEPOSTCONCAT);

	// active renderware camera setting
	CNtlPLGlobal::SetActiveCamera(m_pCamera->GetCamera()); 

	// activate current world
	switch (m_pWorldEntity->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		CNtlPLGlobal::SetActiveWorld(m_pWorldEntity->GetWorld()); 
		break;
	}

	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

	// gui setup
	if( !gui::CSetupGUI::Init( GetHWnd(), m_iScreenWidth, m_iScreenHeight, GetNtlGuiManager()->GetGuiManager(), 
		".\\gui\\Font\\GuiFont.def", ".\\script\\metatag.htm", ".\\gui\\Font\\FlashFont.def",
		".\\gui\\particle\\DBO_2DParticle.2ps" ) )
	{
		NTL_RETURN(FALSE);
	}

	GetSoundManager()->Init(".\\sound\\", 0.0f, 1.0f, 1.0f, 0.5f);

#ifdef _NTL_PROFILE_ENABLE
	// gui group (profile)
	GetNtlGuiManager()->CreatePLGuiGroup();
#endif


#ifdef WE_CHARACTER_VIEW
	//Character View(By HoDong) 
	if(!API_CoreInit())
	{
		NTL_RETURN(FALSE);
	}

	if(!API_SLInit(TRUE, NULL, (RwUInt8)CTable::LOADING_METHOD_BINARY))
		NTL_RETURN(FALSE);

#endif

	// Spawn manager
	GetSpawnInstance()->Create(eSPAWN_MOB);

	NTL_RETURN(TRUE);
}

RwBool CNtlWEApplication::AttachPlugin()
{
	NTL_FUNCTION( "CNtlWEApplication::AttachPlugin" );

	if( !RpWorldPluginAttach() )
		NTL_RETURN(FALSE);

	if( !RpPVSPluginAttach() )
		NTL_RETURN(FALSE);

	if( !RpRandomPluginAttach() )
		NTL_RETURN(FALSE);

	if( !RpLtMapPluginAttach() )
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

	
	NTL_RETURN(TRUE);
}

RwBool CNtlWEApplication::Resize(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer, RwBool bRestore_from_Minimize)
{
	NTL_FUNCTION("CNtlWEApplication::Resize" );

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
				rt.x = 0;
				rt.y = 0;
				rt.w = iWidth;
				rt.h = iHeight;
				//m_pCamera->SetCameraResize(m_pCamera->GetCamera(), &rt);
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

RwBool CNtlWEApplication::OnSize(RwUInt32 iType, RwUInt32 iWidth, RwUInt32 iHeight)
{
	RwBool bRestore_from_Minimize = FALSE;
	/*if (iType == SIZE_MINIMIZED)
	{
		m_bMinimized = TRUE;
	}
	else if(iType == SIZE_RESTORED)
	{
		if(m_bMinimized)
		{
			m_bMinimized = FALSE;
			if (strlen(dGET_WORLD_PARAM()->WorldProjectFolderName) > 0)
			{
				bRestore_from_Minimize = TRUE;
			}
		}
	}*/

	return Resize(iWidth, iHeight, TRUE, bRestore_from_Minimize);
}

void CNtlWEApplication::Destroy()
{
	NTL_FUNCTION("CNtlWEApplication::Destroy" );

#ifdef WE_CHARACTER_VIEW
	CNtlWeControlUi::GetInstance().Destroy();

	CNtlWeAvatarController::GetInstance().DestroyControl();
#endif

	if (m_pEventHandler)
	{
		m_pEventHandler->SaveConfig();
		NTL_DELETE(m_pEventHandler);
		m_pEventHandler = NULL;
	}

// 	if (m_pEventHandlerIndoor)
// 	{
// 		NTL_DELETE(m_pEventHandlerIndoor);
// 		m_pEventHandlerIndoor = NULL;
// 	}

	if(m_pCamera)
	{
		RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

		m_pCamera->Destroy();
		NTL_DELETE(m_pCamera);
		m_pCamera = NULL;
	}

	if(m_pCameraTop)
	{
		NTL_DELETE(m_pCameraTop);
		m_pCameraTop = NULL;
	}

	if (m_pWorldEntity)
	{
		GetSceneManager()->DeleteEntity(m_pWorldEntity);
		m_pWorldEntity = NULL;
	}

	m_PropContainter.GetInstance()->Destroy();

	// destory brush entity
	if(dGET_BRUSH_ENTITY())
	{
		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
		dGET_BRUSH_ENTITY() = NULL;
	}

#ifdef WE_CHARACTER_VIEW
	//Character View(By HoDong)
	API_SLTerminate();
#endif

#ifdef _NTL_PROFILE_ENABLE
	// gui group (profile)
	GetNtlGuiManager()->DestroyPLGuiGroup();
#endif

	gui::CSetupGUI::Deinit();

	// Spawn manager
	DestroySpawnInstance();

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

	// PL event handler
	if(m_pPLEventHandler)
	{
		m_pPLEventHandler->Destroy();
		NTL_DELETE(m_pPLEventHandler);
	}

	GetSoundManager()->Release();

	// API_PLInit
	API_PLTerminate();

#ifdef WE_CHARACTER_VIEW	
	//Character View(By HoDong);
	API_CoreTerminate();
#endif

	// Close the event system
	RWS::CEventHandler::UnRegisterStreamChunkHandlers();
	RWS::CEventHandler::Close();

	CNtlApplication::Destroy();

	NTL_RETURNVOID();
}

void CNtlWEApplication::DisplayInfo()
{
	//if(m_pEventHandler)
	{
		if(!dGET_WORLD_PARAM()->FlgObjInfoShow)
			{
			return;
			}

		RwChar		caption[256];
		RwUInt32	iHeight = 0;

		RwRGBA rgba;
		rgba.alpha	= 255;
		rgba.red	= 0;
		rgba.green	= 0;
		rgba.blue	= 0;

		RtCharsetSetColors(m_pCharset, &rgba, &m_BackgroundColor);

		RsSprintf(caption, RWSTRING("FPS: %4.2f"), GetFps());
		RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);

		RsSprintf(caption, RWSTRING("Window Width : %d, Height : %d"), GetScreenWidth(), GetScreenHeight());
		RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);

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
		RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);

		RsSprintf(caption, RWSTRING("Virtual : %I64u/%I64uMB"), (dwTotalVirtual - dwAvailVirtual) / dwDataPack, dwTotalVirtual / dwDataPack);
		RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);

		RsSprintf(caption, RWSTRING("PageFile : %I64u/%I64uMB"), (dwTotalPageFile - dwAvailPageFile) / dwDataPack, dwTotalPageFile / dwDataPack);
		RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);

		RsSprintf(caption, RWSTRING("Video : %u/%uMB"), (dwTotalVideo - dwAvailVideo) / dwDataPack, dwTotalVideo / dwDataPack);
		RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);

		RwV3d Pos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
		RsSprintf(caption, RWSTRING("%.2f, %.2f, %.2f"), Pos.x, Pos.y, Pos.z);
		RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);

		RsSprintf(caption, RWSTRING("%.2f mps"), dGET_WORLD_PARAM()->WorldCamSpeedPerSec);
		RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);

		RsSprintf(caption, RWSTRING("%d polys"), dGET_WORLD_PARAM()->WorldPolyCnt);
		RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);

		RsSprintf(caption, RWSTRING("CullTestCnt:%u"), CNtlPLGlobal::m_uiCullTestCnt);
		RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);

		RsSprintf(caption, RWSTRING("CullTestAtomicCnt:%u"), CNtlPLGlobal::m_uiCullTestAtomicCnt);
		RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);

		RsSprintf(caption, RWSTRING("RenderAtomicCnt:%u"), CNtlPLGlobal::m_uiRenderAtomicCnt);
		RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);

		// display field index, sector index of the current camera position
		if(m_pCamera && m_pWorldEntity->IsWorking() && m_pWorldEntity->GetActiveWorldType() == AW_HEGITHFIELD)
		{
			RwInt32 CurSecIdx = m_pWorldEntity->GetWorldFieldMgr()->GetSectorIdx(Pos);
			RsSprintf(caption, RWSTRING("Field(%d), Sector(%d), PVS(%d)"), m_pWorldEntity->GetWorldFieldMgr()->GetFieldIdx(Pos), CurSecIdx, m_pWorldEntity->GetWorldFieldMgr()->m_pSectors[CurSecIdx].m_pNtlWorldSectorPVS->GetPVSAppliedNeighborSectorNum(CurSecIdx));
			RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);
		}

		// display current z vector normalized direction; unit front vector
		if(m_pCamera && m_pWorldEntity->IsWorking())
		{
			RwFrame*	pFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
			RwMatrix*	pMatrix = RwFrameGetMatrix(pFrame);
			RwV3d		vFront	= *RwMatrixGetAt(pMatrix);

			//vFront.y = 0.0f;
			RwV3dNormalize(&vFront, &vFront);
			RsSprintf(caption, RWSTRING("%.2f, %.2f, %.2f"), vFront.x, vFront.y, vFront.z);
			RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);
		}

		if(m_pEventHandler)
		{
			RsSprintf(caption, RWSTRING("OBJ %d(%d) / EFF %d(%d) / WAT %d(%d)"),	m_pEventHandler->m_CntDoodad[0], m_pEventHandler->m_CntDoodad[1],
				m_pEventHandler->m_CntEffect[0], m_pEventHandler->m_CntEffect[1],
				m_pEventHandler->m_CntWater[0], m_pEventHandler->m_CntWater[1]);
			RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);
		}

		if(m_pWorldEntity->GetWorld() && m_pWorldEntity->GetWorldFieldMgr() && m_pWorldEntity->GetWorldFieldMgr()->GetWeatherHandler())
		{
			RsSprintf(caption, RWSTRING("Weather Particle : %d(%d)"),
				m_pWorldEntity->GetWorldFieldMgr()->GetWeatherHandler()->GetNumRender(),
				m_pWorldEntity->GetWorldFieldMgr()->GetWeatherHandler()->GetNumList());
			RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);
		}
		if(GetSceneManager()->GetActiveWorldType() == AW_RWWORLD && CNtlWorldRpLightManager::GetInstance())
		{
			RsSprintf(caption, RWSTRING("RealLight(%d,%d) DummyLight(%d,%d)"),
				CNtlWorldRpLightManager::GetInstance()->GetLightNumStaticRealLoaded(),
				CNtlWorldRpLightManager::GetInstance()->GetLightNumRealLoaded(),
				CNtlWorldRpLightManager::GetInstance()->GetLightNumStaticDummyLoaded(),
				CNtlWorldRpLightManager::GetInstance()->GetLightNumDummyLoaded());
			RsCharsetPrint(m_pCharset, caption, 0, iHeight++, rsPRINTPOSTOPRIGHT);
		}

		if(dGET_WORLD_PARAM()->FlgHeightValueShow)
		{
			RwV3d v2dPoint;
			RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
			RwV3dTransformPoints(&v2dPoint, &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, 1, pMatView);
			v2dPoint.x = (v2dPoint.x / v2dPoint.z) * GetScreenWidth() + 10.0f;
			v2dPoint.y = (v2dPoint.y / v2dPoint.z) * GetScreenHeight();

			RsSprintf(caption, RWSTRING("%.2f"), dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y);
			RsCharsetPrint(m_pCharset, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);
		}

		if(CNtlWorldLTManager::GetInstance())
			CNtlWorldLTManager::GetInstance()->RenderUIText(m_pCharset, GetScreenWidth(), GetScreenHeight());

		if (m_pEventHandler)
		{
			m_pEventHandler->RenderDisplayInfo(m_pCharset, GetScreenWidth(), GetScreenHeight());
		}
	}
}

void CNtlWEApplication::RenderText(RwV3d vPos, const RwChar* pPrintString, RwUInt8 r, RwUInt8 g, RwUInt8 b, RwBool bBackCulling)
{
	RenderText(vPos, pPrintString, GetScreenWidth(), GetScreenHeight(), r,g, b, bBackCulling);
}

void CNtlWEApplication::RenderText(RwV3d vPos, const RwChar* pPrintString, RwUInt32 uiWidth, RwUInt32 uiHeight, RwUInt8 r, RwUInt8 g, RwUInt8 b, RwBool bBackCulling)
{
	if (RwCameraGetCurrentCamera())
	{
		RwV3d v3;
		RwV3dTransformPoint(&v3, &vPos, RwCameraGetViewMatrix(RwCameraGetCurrentCamera()));

		if (RwCameraGetProjection(RwCameraGetCurrentCamera()) == rwPERSPECTIVE)
		{
			if (bBackCulling)
			{
				if (v3.z <= 0.0f)
				{
					return;
				}
			}

			v3.x = v3.x / v3.z;
			v3.y = v3.y / v3.z;
			v3.z = v3.z / v3.z;
		}		

		RwV2d v2;
		v2.x = v3.x * static_cast<RwReal>(uiWidth);
		v2.y = v3.y * static_cast<RwReal>(uiHeight);

		RwRGBA rgba;
		rgba.alpha	= 255;
		rgba.red	= r;
		rgba.green	= g;
		rgba.blue	= b;

		RtCharsetSetColors(m_pCharset, &rgba, &m_BackgroundColor);
		RsCharsetPrint(m_pCharset, pPrintString, static_cast<RwInt32>(v2.x), static_cast<RwInt32>(v2.y));		
	}
}

CNtlWeController* CNtlWEApplication::GetNtlCamController(RwInt32 IdxCamMode)
{
	if(IdxCamMode == 0)
	{
		return static_cast<CNtlWeController*>(m_pCamera);
	}
	else if(IdxCamMode == 1)
	{
		return static_cast<CNtlWeController*>(&CNtlWeAvatarController::GetInstance());
	}
	else if(IdxCamMode == 2)
	{
		return static_cast<CNtlWeController*>(m_pCameraTop);
	}

	return NULL;
}

VOID CNtlWEApplication::PickSC()
{
	if(dOKEY(DIK_NUMPADENTER))
	{
		if(m_pCamera)
		{
			CreateDirectory(".\\ScreenShot",NULL);
			SYSTEMTIME systemTime;
			GetSystemTime(&systemTime);

			RwChar szBuffer[256];
			RwInt32 nYear   = (RwInt32)systemTime.wYear;
			RwInt32 nMonth  = (RwInt32)systemTime.wMonth;
			RwInt32 nDay	= (RwInt32)systemTime.wDay;
			RwInt32 nHour	= (RwInt32)systemTime.wHour;
			RwInt32 nSecond = (RwInt32)systemTime.wSecond;
			RwInt32 nMinute = (RwInt32)systemTime.wMinute;
			RwReal fMilliseconds = (RwReal)systemTime.wMilliseconds/1000.f;
			sprintf_s(szBuffer, 256, ".\\ScreenShot\\%d-%d-%d-%d-%d-%d-%4.2f.bmp", nYear, nMonth, nDay, nHour, nSecond, nMinute, fMilliseconds);

			RwCamera *pCamera = m_pCamera->GetCamera();
			if(pCamera && strlen(szBuffer) > 0)
			{
				RwRaster *pRaster = RwCameraGetRaster(pCamera);		

				API_PL_RasterSaveImage(pRaster, szBuffer);
			}
		}
	}
}

RwBool CNtlWEApplication::Update(RwReal fTime, RwReal fElapsedTime)
{
	if(m_pCamera && m_pWorldEntity->IsWorking())
	{

#ifdef	WE_CHARACTER_VIEW
		//By HoDong
		CNtlWeControlUi::GetInstance().Update(fElapsedTime);
#endif
		
#ifndef WE_CHARACTER_VIEW
		m_pCamera->UpdateControl(fElapsedTime);
#endif
		GetNtlSoundMoodManager()->Update(fElapsedTime);
	}

	m_pVisualManager->UpdateBeforeCamera(fElapsedTime);
	
	if (m_pCamera->CameraClear(&m_BackgroundColor, rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
	{

		if(m_pCamera->PostCameraBeginUpdate(fElapsedTime) )
		{
			m_pVisualManager->Update(fElapsedTime);
			m_pVisualManager->Render(); 

			RwD3D9SetVertexShader(NULL);
			RwD3D9SetPixelShader(NULL);

			if (m_pEventHandler)
			{
				m_pEventHandler->RenderBrush();
			}
			m_pWorldEntity->Render();
			GetSpawnInstance()->Render();

			if (m_pEventHandler)
			{
				m_pEventHandler->Render();
			}			
			m_pCamera->PostCameraEndUpdate();
		}

		m_pCamera->UpdatePostEffectFilter(); 
		m_pCamera->UpdateHeatHaze(fElapsedTime);
		m_pCamera->UpdateMotionBlur(fElapsedTime);

		if( m_pCamera->MainCameraBeginUpdate(fElapsedTime) )
		{
			m_pVisualManager->PostRender();

			DisplayInfo();

			m_pCamera->MainCameraEndUpdate();
		}

		m_pCamera->CameraShowRaster(GetHWnd(), 0);
	}

	dGETFRM()->m_pWorldViewDlg->Render();
	
	// 2009. 3. 23. - Cz
	// Print Screen Key로 Client에서 Screenshot을 찍기 때문에 똑같은 기능을 Tool에서 빼기로함.
	//PickSC();

	return TRUE;
}

LRESULT CNtlWEApplication::WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _NTL_PROFILE_ENABLE
	gui::CGUIManager *pGuiMgr = CNtlPLGuiManager::GetInstance()->GetGuiManager();
	if(pGuiMgr)
	{
		pGuiMgr->Win32EventProc(window, message, wParam, lParam);
	}
#endif

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
		CInputHandler::GetInstance()->MouseUpHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_LBTN, 0);
		break;
	case WM_RBUTTONDOWN:
		CInputHandler::GetInstance()->MouseDownHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_RBTN, false); 
		break;
	case WM_RBUTTONUP:
		CInputHandler::GetInstance()->MouseUpHandler((unsigned int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (char)MOUSE_RBTN, 0);
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
	case WM_SIZE:
		{
			int a = 0;
		}
		break;

	default:
		break;
	}

	return TRUE;
	//return CNtlApplication::WndProc(window, message, wParam, lParam);
}

bool CNtlWEApplication::CreateWorldEx(string& FullPathFileName)
{
	CNtlPLGlobal::m_bCollObjVisible = FALSE;
	CNtlPLGlobal::m_bCharacterUpdateVisible = TRUE;

	DestroyWorldEx();

	*dGET_WORLD_PARAM() = dGETDOC()->m_NtlWorldParam;

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	// create property container
	m_PropContainter.GetInstance()->Load();

	CString strPath	= dGET_WORLD_PARAM()->CurWorkingFolderName;
	CString strTemp	= "\\Tool\\SABER\\cript\\Property.ini";
	strPath			= strPath + strTemp;

	m_WorldPropToolData.LoadIni(strPath.GetBuffer());

	// set resource folder structures
	strcpy(dGET_WORLD_PARAM()->WorldProjectFolderName, FullPathFileName.c_str());
	strcat(dGET_WORLD_PARAM()->WorldProjectFolderName, "\\");
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);

	_mkdir("fields");
	_mkdir("swap");

	// create a global world param
	if(FILE* pFile = fopen("#######.gwp", "wb"))
	{
		// default zero
		
		dGET_WORLD_PARAM()->WorldObjectSerialID = 1;
		dGET_WORLD_PARAM()->WorldTriggerSerialID = 1;
		SaveFileWorldState(dGET_WORLD_PARAM(), pFile);
		fclose(pFile);
	}

	if(FILE* pFile = fopen("-------", "wb"))
	{
		BYTE PVSFlag = 0;
		for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum; ++i)
		{
			fwrite(&PVSFlag, sizeof(BYTE), 1, pFile);
		}

		fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	// create world
	RwV3d SPos = {0.0f, 0.0f, 0.0f};

	m_pWorldEntity = static_cast<CNtlPLWorldEntity*>(m_pVisualManager->CreateEntity(PLENTITY_WORLD, "NtlWorld"));
	DBO_ASSERT(m_pWorldEntity, "Entity create failed.");

	m_pWorldEntity->Destroy();
	m_pWorldEntity->CreateWorld(SPos);

	// free camera
	m_pCamera = NTL_NEW CNtlGameCamera;
	CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_HDR);
	m_pCamera->Create(CNtlApplication::GetScreenWidth(), CNtlApplication::GetScreenHeight(), TRUE, dGET_WORLD_PARAM()->WorldNearPlane, dGET_WORLD_PARAM()->WorldFarPlane);

	RwV3d Eye = {0.0f, 8.0f, 0.0f};
	RwV3d LookAt = {0.0f, 0.0f, 0.0f};

	RwFrame *pCameraFrame = RwCameraGetFrame(m_pCamera->GetCamera());

	RwMatrix *CamMat = RwFrameGetMatrix(pCameraFrame);
	LookAt = *RwMatrixGetAt(CamMat);
	RwFrameTranslate(pCameraFrame, &Eye, rwCOMBINEPOSTCONCAT);

	CNtlPLGlobal::SetActiveCamera(m_pCamera->GetCamera()); 

	switch (m_pWorldEntity->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		CNtlPLGlobal::SetActiveWorld(m_pWorldEntity->GetWorld()); 
		break;
	}

	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

	// top view camera
	m_pCameraTop = NTL_NEW CNtlCameraTop;
	m_pCameraTop->SetCamera(m_pCamera->GetCamera());

	// tool event handler
	// 인도어
// 	switch (m_pWorldEntity->GetActiveWorldType())
// 	{
// 	case AW_HEGITHFIELD:
		m_pEventHandler = NTL_NEW CWorldEditorEventHander(GetScreenWidth(), GetScreenHeight());
		m_pEventHandler->Create();
// 		break;
// 	case AW_RWWORLD:
// 		m_pEventHandlerIndoor = NTL_NEW CWorldEditorEventHanderIndoor();
// 		break;
// 	}

	//GetSoundManager()->Init(".\\sound\\", 1.0f, 1.0f, 1.0f, 0.5f);

	// create a new PVS data
	//CreateDefaultPVSFile();

#ifdef WE_CHARACTER_VIEW
	//Character View(By HoDong)
	CNtlWeAvatarController::GetInstance().CreateControl(m_pCamera, m_pVisualManager);

	CNtlWeControlUi::GetInstance().Create();
	CNtlWeControlUi::GetInstance().SetActiveWeController(m_pCamera);
	CNtlWeControlUi::GetInstance().SetFov(60.f);
	CNtlWeControlUi::GetInstance().SetZoomDist(15.0f);

	dGET_WORLD_PARAM()->WorldCamSpeedPerSec = 100.0f;
	CNtlWeControlUi::GetInstance().SetSpeed(dGET_WORLD_PARAM()->WorldCamSpeedPerSec);

#endif

	if(m_pWorldEntity->GetActiveWorldType() == AW_RWWORLD)
	{
		CNtlWorldLTManager::CreateInstance();
		CNtlWorldLTManager::GetInstance()->Create();
	}

	dGETFRM()->m_pBookmarkDlg->CreateBM();

	return true;
}

bool CNtlWEApplication::CreateWorldFromFolderEx(string& FullPathFileName)
{
	CNtlPLGlobal::m_bCollObjVisible = FALSE;
	CNtlPLGlobal::m_bCharacterUpdateVisible = TRUE;

	DestroyWorldEx();

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	m_PropContainter.GetInstance()->Load();

	CString strPath	= dGET_WORLD_PARAM()->CurWorkingFolderName;
	CString strTemp	= "\\Tool\\SABER\\Script\\Property.ini";
	strPath			= strPath + strTemp;

	m_WorldPropToolData.LoadIni(strPath.GetBuffer());

	strcpy(dGET_WORLD_PARAM()->WorldProjectFolderName, FullPathFileName.c_str());
	strcat(dGET_WORLD_PARAM()->WorldProjectFolderName, "\\");
	if (_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName) == -1)
	{
		::AfxMessageBox("WorldProjectFolderName Error", MB_OK);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}

	RwChar cTemp[512];
	sprintf_s(cTemp, 512, "SABER(%s-%s)-%s : Nothing is more despicable than respect based on fear-Albert Camus.", dNTL_WORLD_VERSION, dNTL_WORLD_TOOL_MODE, dGET_WORLD_PARAM()->WorldProjectFolderName);
	dGETFRM()->SetWindowText(cTemp);

	// create a global world param
	if(FILE* pFile = fopen("#######.gwp", "rb"))
	{
		LoadFileWorldState(dGET_WORLD_PARAM(), pFile);
		fclose(pFile);
	}

	if(FILE* pFile = fopen("-------", "rb"))
	{
		fclose(pFile);
	}
	else if(::AfxMessageBox("not exist pvs data. create now?", MB_YESNO) == IDYES)
	{		
		if(FILE* pFile = fopen("-------", "wb"))
		{
			BYTE PVSFlag = 0;
			for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum; ++i)
			{
				fwrite(&PVSFlag, sizeof(BYTE), 1, pFile);
			}

			fclose(pFile);
		}
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	// create world
	RwV3d SPos = {0.0f, 0.0f, 0.0f};

	m_pWorldEntity = static_cast<CNtlPLWorldEntity*>(m_pVisualManager->CreateEntity(PLENTITY_WORLD, "NtlWorld"));
	DBO_ASSERT(m_pWorldEntity, "Entity create failed.");

	//m_pWorldEntity->Destroy();
	m_pWorldEntity->CreateWorld(SPos);

	// free camera
	m_pCamera = NTL_NEW CNtlGameCamera;
	CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_HDR);
	m_pCamera->Create(CNtlApplication::GetScreenWidth(), CNtlApplication::GetScreenHeight(), TRUE, dGET_WORLD_PARAM()->WorldNearPlane, dGET_WORLD_PARAM()->WorldFarPlane);

	RwV3d Eye = {0.0f, 8.0f, 0.0f};
	RwV3d LookAt = {0.0f, 0.0f, 0.0f};

	RwFrame *pCameraFrame = RwCameraGetFrame(m_pCamera->GetCamera());

	RwMatrix *CamMat = RwFrameGetMatrix(pCameraFrame);
	LookAt = *RwMatrixGetAt(CamMat);
	RwFrameTranslate(pCameraFrame, &Eye, rwCOMBINEPOSTCONCAT);

	CNtlPLGlobal::SetActiveCamera(m_pCamera->GetCamera());

	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

	// top view camera
	m_pCameraTop = NTL_NEW CNtlCameraTop;
	m_pCameraTop->SetCamera(m_pCamera->GetCamera());

	m_pEventHandler = NTL_NEW CWorldEditorEventHander(GetScreenWidth(), GetScreenHeight());
	m_pEventHandler->Create();
	m_pEventHandler->LoadConfig();

#ifdef WE_CHARACTER_VIEW

	//Character View(By HoDong)
	CNtlWeAvatarController::GetInstance().CreateControl(m_pCamera, m_pVisualManager);

	CNtlWeControlUi::GetInstance().Create();
	CNtlWeControlUi::GetInstance().SetActiveWeController(m_pCamera);
	CNtlWeControlUi::GetInstance().SetFov(60.f);
	CNtlWeControlUi::GetInstance().SetZoomDist(15.0f);

	dGET_WORLD_PARAM()->WorldCamSpeedPerSec = 100.0f;
	CNtlWeControlUi::GetInstance().SetSpeed(dGET_WORLD_PARAM()->WorldCamSpeedPerSec);
#endif

	if(m_pWorldEntity->GetActiveWorldType() == AW_RWWORLD)
	{
		CNtlWorldLTManager::CreateInstance();
		CNtlWorldLTManager::GetInstance()->Create();
	}

	dGETFRM()->m_pBookmarkDlg->CreateBM();
	return true;
}

void CNtlWEApplication::DestroyWorldEx()
{
#ifdef WE_CHARACTER_VIEW
	CNtlWeAvatarController::GetInstance().DestroyControl();

	CNtlWeControlUi::GetInstance().Destroy();
#endif

	if (m_pEventHandler)
	{
		NTL_DELETE(m_pEventHandler);
		m_pEventHandler = NULL;
	}

	if(m_pCamera)
	{
		RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

		m_pCamera->Destroy();
		NTL_DELETE(m_pCamera);
		m_pCamera = NULL;
	}

	if(m_pCameraTop)
	{
		NTL_DELETE(m_pCameraTop);
		m_pCameraTop = NULL;
	}

	if (m_pWorldEntity)
	{
		GetSceneManager()->DeleteEntity(m_pWorldEntity);
		m_pWorldEntity = NULL;
	}

	m_PropContainter.GetInstance()->Destroy();
}
