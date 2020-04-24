#include "stdafx.h"

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

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
#include <rpUVAnim.h>
#include <rppvs.h>
#include <rpltmap.h>
#include <rprandom.h>
#include <rtcharse.h>

#include "NtlAtomic.h"
#include "NtlWorldPlugin.h"
#include "NtlWorldMaterialPlugin.h"

#include "NtlSoundManager.h"

#include "NtlPLApi.h"
#include "NtlPLMaterialExtWrapper.h"
#include "NtlPLPropertyContainer.h"

#include "NtlPLEventHandler.h"
#include "NtlPLEventGenerator.h"

#include "NtlPLSceneManager.h"
#include "NtlPLVisualManager.h"

#include "NtlPLWorldEntity.h"

#include "NtlPostEffectCamera.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RwWorld.h"
#include "NtlWorldLTManager.h"

#include "NtlWPPApplication.h"

CNtlWPPApplication::CNtlWPPApplication()
:m_pNtlPLEventHandler(NULL)
,m_pNtlPLVisualManager(NULL)
,m_pNtlPLPropertyContainer(NULL)
,m_pNtlPostEffectCamera(NULL)
,m_pNtlPLWorldEntity(NULL)
{
	// do only once
	_getcwd(dGET_WORLD_PARAM()->CurWorkingFolderName, 256);
	strcpy(dGET_WORLD_PARAM()->CurDefaultTexPath, dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcat(dGET_WORLD_PARAM()->CurDefaultTexPath, "\\texture\\ntlwe\\");

	CTime time = CTime::GetCurrentTime(); 
	m_strLogFilePath = dGET_WORLD_PARAM()->CurWorkingFolderName;
	m_strLogFilePath += "\\TOOL\\SABER\\LOG\\";
	m_strLogFilePath += time.Format("%y-%m-%d-%H-%M-%S");
	m_strLogFilePath += "L.log";
	::NtlLogFilePath(const_cast<RwChar*>(m_strLogFilePath.c_str()));
	m_strTraceFilePath = dGET_WORLD_PARAM()->CurWorkingFolderName;
	m_strTraceFilePath += "\\TOOL\\SABER\\LOG\\";
	m_strTraceFilePath += time.Format("%y-%m-%d-%H-%M-%S");
	m_strTraceFilePath += "T.log";
	::NtlTraceInitFilePath(const_cast<RwChar*>(m_strTraceFilePath.c_str()));

	::NtlLogFilePrint(" === < SABER > Log Start === ");
	::NtlTraceFilePrint(" === < SABER > Trace Start === ");

	InitPatchLog();
}

CNtlWPPApplication::~CNtlWPPApplication()
{
	::NtlLogFilePrint(" === < SABER > Log End === ");
	::NtlTraceFilePrint(" === < SABER > Trace End === ");
}

RwBool CNtlWPPApplication::AttachPlugin()
{
	NTL_FUNCTION("CNtlWPPApplication::AttachPlugin");

	if ( !RpWorldPluginAttach() )
		NTL_RETURN(FALSE);

	if ( !RpPVSPluginAttach() )
		NTL_RETURN(FALSE);

	if ( !RpRandomPluginAttach() )
		NTL_RETURN(FALSE);

	if ( !RpLtMapPluginAttach() )
		NTL_RETURN(FALSE);

	if ( !RpCollisionPluginAttach() )
		NTL_RETURN(FALSE);

	if ( !RtAnimInitialize() )
		NTL_RETURN(FALSE);

	if ( !RpHAnimPluginAttach() )
		NTL_RETURN(FALSE);

	if ( !RpSkinPluginAttach() )
		NTL_RETURN(FALSE);

	if ( !RpToonPluginAttach() )
		NTL_RETURN(FALSE);

	if ( !RpUserDataPluginAttach() )
		NTL_RETURN(FALSE);

	if ( !RpIdPluginAttach() )
		NTL_RETURN(FALSE);

	if ( !RpMatFXPluginAttach() )
		NTL_RETURN(FALSE);

	if (!RpUVAnimPluginAttach())
		NTL_RETURN(FALSE);

	if (!RpPTankPluginAttach())
		NTL_RETURN(FALSE);

	if (!RpPrtStdPluginAttach())
		NTL_RETURN(FALSE);

	if (!RpPrtAdvPluginAttach())
		NTL_RETURN(FALSE);

	if (!RpNtlAtomicPluginAttach())
		NTL_RETURN(FALSE);

	if (!NtlMatExtPluginAttach())
		NTL_RETURN(FALSE);

	if ( !RpNtlWorldAttach() )
		NTL_RETURN(FALSE);

	if ( !RpNtlWorldMaterialPluginAttach() )
		NTL_RETURN(FALSE);

	NTL_RETURN(TRUE);
}

RwBool CNtlWPPApplication::CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
{
	// profile
	NTL_FUNCTION("CNtlWPPApplication::CreateSubSystem" );

	CNtlApplication::CreateSubSystem(iWidth, iHeight, zBuffer);

	// open the event handler module
	RWS::CEventHandler::Open();
	RWS::CEventHandler::RegisterStreamChunkHandlers();

	// global initialization
	API_PLInit();

	// PL event handler
	m_pNtlPLEventHandler = NTL_NEW CNtlPLEventHandler;
	if(!m_pNtlPLEventHandler->Create())
	{
		NTL_RETURN(FALSE);
	}

	// visual manager
	m_pNtlPLVisualManager = NTL_NEW CNtlPLVisualManager;
	if(!m_pNtlPLVisualManager->Create())
		NTL_RETURN(FALSE);

	// active scene manager setting
	CNtlPLSceneManagerFactory::ActiveSceneManager(m_pNtlPLVisualManager);

	GetSoundManager()->Init(".\\sound\\", 0.0f, 1.0f, 1.0f, 0.5f);

	m_pNtlPLPropertyContainer = NTL_NEW CNtlPLPropertyContainer;
	if (!m_pNtlPLPropertyContainer->GetInstance()->Load())
		NTL_RETURN(FALSE);

	NTL_RETURN(TRUE);
}

void CNtlWPPApplication::Destroy()
{
	NTL_FUNCTION("CNtlWEApplication::Destroy" );

	if (m_pNtlPLPropertyContainer)
	{
		m_pNtlPLPropertyContainer->GetInstance()->Destroy();
		NTL_DELETE(m_pNtlPLPropertyContainer);
		m_pNtlPLPropertyContainer = NULL;
	}	

	GetSoundManager()->Release();

	if(m_pNtlPLVisualManager)
	{
		m_pNtlPLVisualManager->Destroy();
		NTL_DELETE(m_pNtlPLVisualManager);
		m_pNtlPLVisualManager = NULL;
	}

	// PL event handler
	if(m_pNtlPLEventHandler)
	{
		m_pNtlPLEventHandler->Destroy();
		NTL_DELETE(m_pNtlPLEventHandler);
	}

	// API_PLInit
	API_PLTerminate();

	// Close the event system
	RWS::CEventHandler::UnRegisterStreamChunkHandlers();
	RWS::CEventHandler::Close();

	CNtlApplication::Destroy();

	NTL_RETURNVOID();
}

RwBool CNtlWPPApplication::Update(RwReal fTime, RwReal fElapsedTime)
{
	return TRUE;
}

RwBool CNtlWPPApplication::TestWorldProject(const char* pcWorldFullPath, sNtlWorldParam* pNtlWorldParam)
{
	sNtlWorldParam ntlWorldParam;

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcpy(ntlWorldParam.WorldProjectFolderName, pcWorldFullPath);
	strcat(ntlWorldParam.WorldProjectFolderName, "\\");	
	if (_chdir(ntlWorldParam.WorldProjectFolderName) == -1)
	{
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		DBO_ASSERT(FALSE, "chagne directory failed.");
		return FALSE;
	}

	FILE* pFileWorldInfo = NULL;
	if (fopen_s(&pFileWorldInfo, "#######.gwp", "rb") != 0)
	{
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		DBO_ASSERT(FALSE, "chagne directory failed.");
		return FALSE;
	}
	
	LoadFileWorldState(&ntlWorldParam, pFileWorldInfo);
	fclose(pFileWorldInfo);

	if (pNtlWorldParam)
	{
		*pNtlWorldParam = ntlWorldParam;
	}

	if (strcmp(ntlWorldParam.WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		return FALSE;
	}

	return TRUE;
}

RwInt32 CNtlWPPApplication::GetWorldProjectFieldNum(const char* pcWorldFullPath)
{
	sNtlWorldParam ntlWorldParam;

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcpy(ntlWorldParam.WorldProjectFolderName, pcWorldFullPath);
	strcat(ntlWorldParam.WorldProjectFolderName, "\\");	
	if (_chdir(ntlWorldParam.WorldProjectFolderName) == -1)
	{
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		DBO_ASSERT(FALSE, "chagne directory failed.");
		return 0;
	}

	FILE* pFileWorldInfo = NULL;
	if (fopen_s(&pFileWorldInfo, "#######.gwp", "rb") != 0)
	{
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		DBO_ASSERT(FALSE, "chagne directory failed.");
		return 0;
	}

	LoadFileWorldState(&ntlWorldParam, pFileWorldInfo);
	fclose(pFileWorldInfo);

	if (strcmp(ntlWorldParam.WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		return 0;
	}

	return ntlWorldParam.WorldFieldNum;
}

RwBool CNtlWPPApplication::CreateWorldProject(const char* pcWorldFullPath, char* pcLoadVersion, char* pcSaveVersion)
{
	DestroyWorldProject();

	// chagne directory world project
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcpy(dGET_WORLD_PARAM()->WorldProjectFolderName, pcWorldFullPath);
	strcat(dGET_WORLD_PARAM()->WorldProjectFolderName, "\\");	
	if (_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName) == -1)
	{
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		DBO_ASSERT(FALSE, "chagne directory failed.");
		return FALSE;
	}

	// world infomation read
	FILE* pFileWorldInfo = NULL;
	if (fopen_s(&pFileWorldInfo, "#######.gwp", "rb") != 0)
	{
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		DBO_ASSERT(FALSE, "chagne directory failed.");
		return FALSE;
	}

	LoadFileWorldState(dGET_WORLD_PARAM(), pFileWorldInfo);
	fclose(pFileWorldInfo);

	if (strcmp(dGET_WORLD_PARAM()->WorldVer, pcLoadVersion))
	{
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}

	strcpy_s(dGET_WORLD_PARAM()->WorldLoadVer, 10, pcLoadVersion);
	strcpy_s(dGET_WORLD_PARAM()->WorldSaveVer, 10, pcSaveVersion);

	// chagne directroy working
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	// create world entity
	m_pNtlPLWorldEntity = static_cast<CNtlPLWorldEntity*>(m_pNtlPLVisualManager->CreateEntity(PLENTITY_WORLD, "NtlWorld"));
	m_pNtlPLWorldEntity->Destroy();
	m_pNtlPLWorldEntity->CreateWorld(CNtlPLGlobal::m_vZeroV3);

	// create camera
	m_pNtlPostEffectCamera = NTL_NEW CNtlPostEffectCamera;
	m_pNtlPostEffectCamera->SetPostEffectFilters(POST_EFFECT_FILTER_HDR);
	m_pNtlPostEffectCamera->Create(CNtlApplication::GetScreenWidth(), CNtlApplication::GetScreenHeight(), TRUE, dGET_WORLD_PARAM()->WorldNearPlane, dGET_WORLD_PARAM()->WorldFarPlane);

	RwMatrixSetIdentity(RwFrameGetMatrix(RwCameraGetFrame(m_pNtlPostEffectCamera->GetCamera())));
	RwFrameUpdateObjects(RwCameraGetFrame(m_pNtlPostEffectCamera->GetCamera()));

	CNtlPLGlobal::SetActiveCamera(m_pNtlPostEffectCamera->GetCamera());
	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

	if(m_pNtlPLWorldEntity->GetActiveWorldType() == AW_RWWORLD)
	{
		CNtlWorldLTManager::CreateInstance();
		CNtlWorldLTManager::GetInstance()->Create();
	}

	return TRUE;
}

void CNtlWPPApplication::DestroyWorldProject()
{
	if(m_pNtlPostEffectCamera)
	{
		RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);
		CNtlPLGlobal::SetActiveCamera(NULL);

		m_pNtlPostEffectCamera->Destroy();
		NTL_DELETE(m_pNtlPostEffectCamera);
		m_pNtlPostEffectCamera = NULL;
	}

	if (m_pNtlPLWorldEntity)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLWorldEntity);
		m_pNtlPLWorldEntity = NULL;
	}
}

void CNtlWPPApplication::InitPatchLog()
{
	CTime time = CTime::GetCurrentTime(); 
	m_strPatchFilePath = dGET_WORLD_PARAM()->CurWorkingFolderName;
	m_strPatchFilePath += "\\TOOL\\SABER\\LOG\\";
	m_strPatchFilePath += time.Format("%y-%m-%d-%H-%M-%S");
	m_strPatchFilePath += "P.log";
}

void CNtlWPPApplication::PrintPatchLog(RwChar *format, ...)
{
	DWORD dwAttrs = ::GetFileAttributes(m_strPatchFilePath.c_str());
	if(dwAttrs != INVALID_FILE_ATTRIBUTES)
	{
		if( dwAttrs & FILE_ATTRIBUTE_READONLY) 
		{ 
			dwAttrs &= ~FILE_ATTRIBUTE_READONLY;
			::SetFileAttributes(m_strPatchFilePath.c_str(), dwAttrs); 
		}
	}

	FILE* pFile = NULL;
	if (fopen_s(&pFile, m_strPatchFilePath.c_str(), "a+") != 0)
	{
		return;
	}

	va_list args;
	va_start(args, format);
	vfprintf_s(pFile, format, args);
	va_end(args);

	fclose(pFile);
}


