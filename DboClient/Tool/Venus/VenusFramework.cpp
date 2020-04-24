//***********************************************************************************
//	File		:	VenusFramework.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by NTL-inc CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\venusframework.h"

#include "NtlEffectSystemFreeList.h"

#include "NtlEventQueue.h"

#include "StatusBarDialog.h"

#include "VenusCamera.h"
#include "VenusConfig.h"

#include ".\venuspropertycontainer.h"
#include ".\venusvisualmanager.h"
#include ".\stringmanager.h"

#include "VenusPlayerEventManager.h"

#include "VenusAPI.h"
#include ".\venusgrid.h"

#include "VenusFrame.h"
#include "MeshLayer.h"
#include "EventLayer.h"
#include "TextureLayer.h"

#include <direct.h>
#include "NtlDebug.h"
#include "NtlText.h"
#include "VenusDefine.h"
#include "NtlSoundManager.h"
#include "NtlPLEntityFreeList.h"
#include "ntlworldplugin.h"
#include "NtlWorldMaterialPlugin.h"
#include "NtlCoreApi.h"
#include "NtlPLObject.h"
#include "NtlPlSun.h"
#include "NtlPLResourceScheduling.h"
#include "NtlInstanceMeshSystem.h"
#include "Manipulator.h"

#include "NtlPLResourcePack.h"


CVenusFramework::CVenusFramework(void)
{
	m_fAutoSaveTime = 0.f;

	m_BackgroundColor.red	=150;
	m_BackgroundColor.green	=150;
	m_BackgroundColor.blue	=150;
	m_BackgroundColor.alpha	=255;

	m_ForegroundColor.red   = 255;
	m_ForegroundColor.green = 255;
	m_ForegroundColor.blue  = 0;
	m_ForegroundColor.alpha = 255;

	m_pCharset = NULL;

	m_pWorld = NULL;
	m_pWorldEntity = NULL;
	m_pAmbientLight = NULL;
	m_pMainLight = NULL;
	
	m_pEffectClump		= NULL;
	m_pDecalTestObject	= NULL;

	m_bSelectMode = FALSE;
    m_bFrameFix = FALSE;
    m_fTimeFrameFix = 0.0f;	

	m_pSun = NULL;
}

CVenusFramework::~CVenusFramework(void)
{
}

//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
CVenusFramework& CVenusFramework::GetInstance(void)
{
	static CVenusFramework framework;
	return framework;
}

//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusFramework::Destroy()
{
//	m_LightningController.DeleteData();

    // Sound Manager
    GetSoundManager()->Release();

    if(m_pToonData)
    {
        RpToonPaintDestroy(m_pToonData->pToonPaint);
        RpToonInkDestroy(m_pToonData->pToonInk);
        CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pToonData->pTexture);
        NTL_DELETE(m_pToonData);        
    }

	if(m_pCharset)
    {
		RtCharsetClose();
		RwRasterDestroy(m_pCharset);
		m_pCharset = NULL;
    }

	if (m_pEffectClump != NULL)
	{
		RpClumpDestroy(m_pEffectClump);
		m_pEffectClump = NULL;
	}

	if (m_pAmbientLight != NULL)
	{
		RpWorldRemoveLight(m_pWorld, m_pAmbientLight);
		RpLightDestroy(m_pAmbientLight);
		m_pAmbientLight = NULL;
	}

	if (m_pMainLight != NULL)
	{
		RpWorldRemoveLight(m_pWorld, m_pMainLight);

		RwFrame* pFrame = RpLightGetFrame(m_pMainLight);
        RpLightSetFrame(m_pMainLight, NULL);
        RwFrameDestroy(pFrame);

		RpLightDestroy(m_pMainLight);
		m_pMainLight = NULL;
	}

	CVenusGrid::GetInstance().Destroy();
	CVenusCamera::GetInstance().Destroy();


	CStringManager::GetInstance().Delete();

	if(m_pWorldEntity)
	{
		GetSceneManager()->DeleteEntity(m_pWorldEntity);
		m_pWorldEntity = NULL;
	}

	CVenusVisualManager::GetInstance().Destroy();
	CVenusPropertyContainer::GetInstance().Destroy();

	VENUS_PLTerminate();

	if(m_pPLEventHandler)
	{
		m_pPLEventHandler->Destroy();
		NTL_DELETE(m_pPLEventHandler);
	}

	GetSoundManager()->Release();

	API_CoreTerminate();

	// Close the event system
	RWS::CEventHandler::UnRegisterStreamChunkHandlers();
	RWS::CEventHandler::Close ();

	CNtlApplication::Destroy();

}

//------------------------------------------------------------------
//	FuncName	: CreateSubSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusFramework::AttachPlugin()
{

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

	// woody1019
	if( !RpNtlWorldAttach() )
		NTL_RETURN(FALSE);

	if( !RpNtlWorldMaterialPluginAttach() )
		NTL_RETURN(FALSE);


	return TRUE;
}

void CVenusFramework::SetErrorReport() 
{
    // 버그트랩용 설정
    BT_SetAppName(VERSION_VENUS);         
    BT_SetSupportServer(_T("211.169.249.71"), 9999);
    BT_SetFlags(BTF_DETAILEDMODE);
}

RwBool CVenusFramework::CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool bZBuffer)
{
    // Bug Trap Setting
    SetErrorReport();

	CNtlApplication::CreateSubSystem(iWidth, iHeight, bZBuffer);

	CStringManager::GetInstance().Create("TOOL\\VenusData\\String.txt");

	// Open the event handler module
	RWS::CEventHandler::Open();
	RWS::CEventHandler::RegisterStreamChunkHandlers();

	API_CoreInit();
	
	VENUS_PLInit();

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

	// Active Scene Manager를 Setting한다.
	CNtlPLSceneManagerFactory::ActiveSceneManager(&CVenusVisualManager::GetInstance());

	// Property Container를 생성하고, 데이터를 Load 한다.
	if(!CVenusPropertyContainer::GetInstance().LoadSerialize())
	{
		NTL_ASSERTFAIL("Property Container Load Fail!");
		NTL_RETURN(FALSE);
	}
	
	// World를 생성한다.
	CreateWorld();
	ShowTerrain(CVenusConfig::GetInstance().m_bShowTerrain);	

	// Light
    m_pAmbientLight = RpLightCreate(rpLIGHTAMBIENT);
    if(m_pAmbientLight)
    {
		UpdateAmbient();
		RpWorldAddLight(m_pWorld, m_pAmbientLight);
    }
	// Light
	CreateMainLight();

	CVenusCamera::GetInstance().Create(iWidth, iHeight, bZBuffer);
	CVenusCamera::GetInstance().InitViewMatrix();
	CVenusGrid::GetInstance().Create();

	m_pCharset = RtCharsetCreate(&m_ForegroundColor, &m_BackgroundColor);
    if(m_pCharset == NULL)
	{
		return FALSE;
	}

	// rectangle vertex setting
	RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CVenusCamera::GetInstance().GetCamera());
	for (RwInt32 i = 0; i < RECTANGLE_VERTEX_COUNT; ++ i)
	{
        RwIm2DVertexSetScreenZ(&m_RectangleVertex[i], RwIm2DGetNearScreenZ());
        RwIm2DVertexSetRecipCameraZ(&m_RectangleVertex[i], recipCameraZ);
		m_RectangleVertex[i].emissiveColor = D3DXCOLOR(1.f, 1.f, 0, 1.f);
	}

	SetPauseVertex();

    // Sound Manager를 생성한다.
    //std::string strSoundPath = ".\\";
    //strSoundPath += CVenusConfig::GetInstance().m_strSoundPath;
    //strSoundPath += "\\";

    GetSoundManager()->Init(".\\Sound\\");      // 강제 세팅
    GetSoundManager()->SetListenerPosition(0.0f, 0.0f, 0.0f);

    CString strTitleName;
    strTitleName.Format( "%s [ %s ]", CVenusConfig::GetInstance().m_strAppName, "Noname");    
    AfxGetMainWnd()->SetWindowText(strTitleName);
	
	return TRUE; 
}

RpLight* CVenusFramework::CreateMainLight()
{
	if (m_pMainLight != NULL)
	{
		RpWorldRemoveLight(m_pWorld, m_pMainLight);

		RwFrame* pFrame = RpLightGetFrame(m_pMainLight);
        RpLightSetFrame(m_pMainLight, NULL);
        RwFrameDestroy(pFrame);

		RpLightDestroy(m_pMainLight);
		m_pMainLight = NULL;
	}

	m_pMainLight = RpLightCreate(CVenusConfig::GetInstance().m_nLightType);
	UpdateMainLight();

	RwFrame *pFrame = RwFrameCreate();
	NTL_ASSERTE(pFrame);

	RwFrameRotate(pFrame, &VEC_X, 90, rwCOMBINEREPLACE);

	RwV3d pos = { 0, 20, 0 };
	RwFrameTranslate(pFrame, &pos, rwCOMBINEPOSTCONCAT);

	RpLightSetFrame(m_pMainLight, pFrame);
	RpWorldAddLight(m_pWorld, m_pMainLight);

	return m_pMainLight;
}


RpClump* CVenusFramework::LoadDummyClump(const RwChar* strFileName, RwV3d v, RwV3d s)
{
	RpClump* pClump = NULL;
	RwStream* pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, strFileName);
	if(pStream)
	{
		RwStreamFindChunk(pStream, rwID_CLUMP, NULL, NULL);
		pClump = RpClumpStreamRead(pStream);
		RwStreamClose(pStream, NULL);

		RwFrameScale(RpClumpGetFrame(pClump), &s, rwCOMBINEREPLACE);
		RwFrameTranslate(RpClumpGetFrame(pClump), &v, rwCOMBINEPOSTCONCAT);
	}

    CNtlInstanceMeshSystem::CreateToonData(pClump, m_pToonData);
	return pClump;
}

//------------------------------------------------------------------
//	FuncName	: CreateEffectClump
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusFramework::CreateEffectClump(const RwChar* strPathName)
{
	if (m_pEffectClump != NULL)
	{
		RpClumpDestroy(m_pEffectClump);
		m_pEffectClump = NULL;
	}
	if (strPathName == NULL) return;

	RwV3d v = {0.f, 0.f, 0.f};
	RwV3d s = {0.01f, 0.01f, 0.01f};

	RwImageSetPath(CNtlEffectSystemFreeList::m_strClumpTexturePath.c_str());
	m_pEffectClump	= LoadDummyClump(strPathName,	v, s);
}

//------------------------------------------------------------------
//	FuncName	: UpdateAmbient
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusFramework::UpdateAmbient()
{
	if (m_pAmbientLight != NULL)
		RpLightSetColor(m_pAmbientLight, &CVenusConfig::GetInstance().m_AmbientColor);
}

//------------------------------------------------------------------
//	FuncName	: UpdateLight
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusFramework::UpdateMainLight()
{
	if (m_pMainLight != NULL)
	{
		RpLightSetColor(m_pMainLight, &CVenusConfig::GetInstance().m_LightColor);
		RpLightSetRadius(m_pMainLight, CVenusConfig::GetInstance().m_fLightRadius);
        RpLightSetConeAngle(m_pMainLight, CVenusConfig::GetInstance().m_fLightConeAngle / 180.0f * rwPI);
	}
}

//------------------------------------------------------------------
//	FuncName	: ShowTerrain
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusFramework::ShowTerrain(RwBool bShow)
{
	// 지형과 스카이를 같이 없앤다.
	CNtlPLGlobal::m_bWorldTerrainVisible = bShow;
	CNtlPLGlobal::m_bWorldSkyVisible = bShow;
}

void CVenusFramework::ShowObject(RwBool bShow)
{
	//if(m_pDecalTestObject)
	//{
	//	m_pDecalTestObject->SetVisible(bShow);
	//}	

	// NOTE : 테스트용으로 임시로 LensFlare Refresh 작업을 한다.
	//if(m_pSun)
	//{
	//	m_pSun->ReFreshLensFlareData();
	//}
}

//------------------------------------------------------------------
//	FuncName	: SetPauseVertex
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusFramework::SetPauseVertex()
{
	RwReal fWidth = (RwReal)GetScreenWidth() - 1;
	RwReal fHeight = (RwReal)GetScreenHeight() - 1;

	RwIm2DVertexSetScreenX(&m_PauseVertex[0], 0.f);
	RwIm2DVertexSetScreenY(&m_PauseVertex[0], 0.f);
	RwIm2DVertexSetScreenZ(&m_PauseVertex[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetIntRGBA(&m_PauseVertex[0], 255, 0, 0, 255);

	RwIm2DVertexSetScreenX(&m_PauseVertex[1], fWidth);
	RwIm2DVertexSetScreenY(&m_PauseVertex[1], 0.f);
	RwIm2DVertexSetScreenZ(&m_PauseVertex[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetIntRGBA(&m_PauseVertex[1], 255, 0, 0, 255);


	m_PauseVertex[2] = m_PauseVertex[1];
	RwIm2DVertexSetScreenX(&m_PauseVertex[3], fWidth);
	RwIm2DVertexSetScreenY(&m_PauseVertex[3], fHeight);
	RwIm2DVertexSetScreenZ(&m_PauseVertex[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetIntRGBA(&m_PauseVertex[3], 255, 0, 0, 255);

	m_PauseVertex[4] = m_PauseVertex[3];
	RwIm2DVertexSetScreenX(&m_PauseVertex[5], 0.f);
	RwIm2DVertexSetScreenY(&m_PauseVertex[5], fHeight);
	RwIm2DVertexSetScreenZ(&m_PauseVertex[5], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetIntRGBA(&m_PauseVertex[5], 255, 0, 0, 255);

	m_PauseVertex[6] = m_PauseVertex[5];
	m_PauseVertex[7] = m_PauseVertex[0];


	fWidth = (RwReal)GetScreenWidth() - 2;
	fHeight = (RwReal)GetScreenHeight() - 2;

	RwIm2DVertexSetScreenX(&m_PauseVertex[8], 1.f);
	RwIm2DVertexSetScreenY(&m_PauseVertex[8], 1.f);
	RwIm2DVertexSetScreenZ(&m_PauseVertex[8], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetIntRGBA(&m_PauseVertex[8], 255, 0, 0, 255);

	RwIm2DVertexSetScreenX(&m_PauseVertex[9], fWidth);
	RwIm2DVertexSetScreenY(&m_PauseVertex[9], 1.f);
	RwIm2DVertexSetScreenZ(&m_PauseVertex[9], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetIntRGBA(&m_PauseVertex[9], 255, 0, 0, 255);


	m_PauseVertex[10] = m_PauseVertex[9];
	RwIm2DVertexSetScreenX(&m_PauseVertex[11], fWidth);
	RwIm2DVertexSetScreenY(&m_PauseVertex[11], fHeight);
	RwIm2DVertexSetScreenZ(&m_PauseVertex[11], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetIntRGBA(&m_PauseVertex[11], 255, 0, 0, 255);

	m_PauseVertex[12] = m_PauseVertex[11];
	RwIm2DVertexSetScreenX(&m_PauseVertex[13], 1.f);
	RwIm2DVertexSetScreenY(&m_PauseVertex[13], fHeight);
	RwIm2DVertexSetScreenZ(&m_PauseVertex[13], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetIntRGBA(&m_PauseVertex[13], 255, 0, 0, 255);

	m_PauseVertex[14] = m_PauseVertex[13];
	m_PauseVertex[15] = m_PauseVertex[8];
}

//------------------------------------------------------------------
//	FuncName	: Update
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusFramework::Update(RwReal fTime, RwReal fElapsedTime)
{
    // Frame 고정을 적용한다.
    if(m_bFrameFix)
    {
        m_fTimeFrameFix += fElapsedTime;

        if(m_fTimeFrameFix < (1.0f / FRAME_FIX_FPS))
        {            
            return FALSE;        
        }
        else
        {
            fElapsedTime += m_fTimeFrameFix;
            m_fTimeFrameFix = 0.0f;
        }
    }

	// input process
	API_CoreUpdate(fElapsedTime);

	// World Update
	RwReal fHeight = 0.0f;
	GetSceneManager()->GetWorldHeight(&ZeroAxis, fHeight, NULL);
	RwV3d vPos;
	vPos.x = 0.0f;
	vPos.y = fHeight;
	vPos.z = 0.0f;

	m_pWorldEntity->SetPlayerPosition(vPos);		
	//m_pWorldEntity->Update(fElapsedTime);

    // 자동 저장
	m_fAutoSaveTime += fElapsedTime;
	if (m_fAutoSaveTime > CVenusConfig::GetInstance().m_fAutoSaveMaxTime)
	{
		AutoTempFileSave();
		m_fAutoSaveTime = 0.f;
	}

	// Effect Clump 
	if (m_pEffectClump != NULL)
	{
		RwMatrix matClump, matScale, matRotate, matWorld, matRender;
		CVenusCamera::GetInstance().GetClumpMatrix(matClump);

		RwMatrixRotate(&matRotate, &VEC_Y, fTime * 100.f, rwCOMBINEREPLACE);

		RwSphere sphere;
		API_CalculationShpere(sphere, m_pEffectClump);

		RwV3d s = {0.05f / sphere.radius, 0.05f / sphere.radius, 0.05f / sphere.radius};
		RwMatrixScale(&matScale, &s, rwCOMBINEREPLACE);

		RwMatrixMultiply(&matWorld, &matScale, &matRotate);

		RwMatrixMultiply(&matRender, &matWorld, &matClump);

		RwFrameTransform(RpClumpGetFrame(m_pEffectClump), &matRender, rwCOMBINEREPLACE);
	}

	if (!CVenusConfig::GetInstance().m_bTimePause)
	{
		if (CVenusConfig::GetInstance().m_nCurrentLayer != EDIT_LAYER_EVENT)
		{
			CVenusVisualManager::GetInstance().Update(fElapsedTime * CVenusConfig::GetInstance().m_fTimeSpeed);
		}
		else
		{
			CVenusPlayerEventManager::GetInstance().Update(fElapsedTime * CVenusConfig::GetInstance().m_fTimeSpeed);
		}
	}

	if (RwCameraClear(CVenusCamera::GetInstance().GetCamera(), &m_BackgroundColor, rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
	{
		if(RwCameraBeginUpdate(CVenusCamera::GetInstance().GetCamera()))
		{
			if (m_pEffectClump != NULL)
			{
				RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLNONE);
				RpClumpRender(m_pEffectClump);
				RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLBACK);
			}

			if (CVenusConfig::GetInstance().m_bShowGrid)
			{
				CVenusGrid::GetInstance().Render();
			}

			if (CVenusConfig::GetInstance().m_nCurrentLayer != EDIT_LAYER_EVENT)
			{
				CVenusVisualManager::GetInstance().RenderEffect(TRUE);
				CVenusVisualManager::GetInstance().Render();
				CVenusPropertyContainer::GetInstance().Render();
			}
			else
			{
				// 이벤트 상태일때는 일반 이펙트는 렌더링하지 않는다.
				CVenusVisualManager::GetInstance().RenderEffect(FALSE);
				CVenusVisualManager::GetInstance().Render();
				CVenusPlayerEventManager::GetInstance().Render();
			}


			if (m_bSelectMode)
			{
				RenderSelectRectangle();
			}

			if (CVenusConfig::GetInstance().m_bTimePause)
			{
				SetPauseVertex();
				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);
				RwIm2DRenderPrimitive(rwPRIMTYPELINELIST, m_PauseVertex, 16);
			}

			DisplayInfo();

			RwCameraEndUpdate(CVenusCamera::GetInstance().GetCamera());
		}

		// by agebreak
		CVenusCamera::GetInstance().UpdateBlurFilter();
		CVenusCamera::GetInstance().UpdateMotionBlur(fElapsedTime);
		if (CVenusCamera::GetInstance().MainCameraBeginUpdate(fElapsedTime))
		{

			// 필터 효과가 먹지 않아야 할것들은 여기서 그려라
			CVenusCamera::GetInstance().MainCameraEndUpdate();
		}

		CVenusCamera::GetInstance().CameraShowRaster(GetHWnd(), 0);
	}

    Sleep(1);

	return FALSE;
}

//------------------------------------------------------------------
//	FuncName	: DisplayInfo
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusFramework::DisplayInfo()
{
	NTL_FUNCTION( "CVenusFramework::DisplayInfo" );

    if(m_bFrameFix)
    {
        CStatusBarDialog::GetInstance().UpdateFps(FRAME_FIX_FPS);
    }
    else
    {
	    CStatusBarDialog::GetInstance().UpdateFps(GetFps());
    }

	CStatusBarDialog::GetInstance().UpdateCount(CVenusVisualManager::GetInstance().GetRenderingEffectCount(), CVenusVisualManager::GetInstance().GetRenderingPolygonCount());

    RwChar string[1024] = {0,};
	GetCurrentDirectory(1024, string);
	RsCharsetPrint(m_pCharset, string, 0, 0, rsPRINTPOSTOPRIGHT);

	RsSprintf(string, RWSTRING( "Intersection Effect : %d" ), CVenusVisualManager::GetInstance().m_svSelectInstanceEffect.size());
	RsCharsetPrint(m_pCharset, string, 0, 1, rsPRINTPOSTOPRIGHT);

	RsSprintf(string, RWSTRING( "Manipulator type: %d" ), CManipulator::GetInstance().m_nArrowType);
	RsCharsetPrint(m_pCharset, string, 0, 2, rsPRINTPOSTOPRIGHT);

	rwsprintf( string, RWSTRING( "Time: %6.2f" ), GetCurrentTime() );
	RsCharsetPrint (m_pCharset, string, 0, 3, rsPRINTPOSTOPRIGHT );

	RsSprintf(string, RWSTRING("Copy Efffect : %s"), CVenusPropertyContainer::GetInstance().m_pCopyResourceEffect ? CVenusPropertyContainer::GetInstance().m_pCopyResourceEffect->GetName(): "NONE");
	RsCharsetPrint(m_pCharset, string, 0, 4, rsPRINTPOSTOPRIGHT);

	RsSprintf(string, RWSTRING("Copy System : %s"), CVenusPropertyContainer::GetInstance().m_pCopyResourceSystem ? CVenusPropertyContainer::GetInstance().m_pCopyResourceSystem->GetSystemName() : "NONE");
	RsCharsetPrint(m_pCharset, string, 0, 5, rsPRINTPOSTOPRIGHT);

	RsSprintf(string, RWSTRING("Copy DataFlag : %d"), CVenusPropertyContainer::GetInstance().m_nCopySystemDataFlag);
	RsCharsetPrint(m_pCharset, string, 0, 6, rsPRINTPOSTOPRIGHT);

    RsSprintf(string, RWSTRING("Used Memory : %dKb"), CVenusVisualManager::GetInstance().GetRenderingEffectUsedMemory() / 1024);
    RsCharsetPrint(m_pCharset, string, 0, 6, rsPRINTPOSTOPRIGHT);
	

	NTL_RETURNVOID();
}

void CVenusFramework::SetSelectRectangle(POINT& OldPoint, POINT& CurPoint)
{
	RwIm2DVertexSetScreenX(&m_RectangleVertex[0], (RwReal)OldPoint.x);		RwIm2DVertexSetScreenY(&m_RectangleVertex[0], (RwReal)OldPoint.y);
	RwIm2DVertexSetScreenX(&m_RectangleVertex[1], (RwReal)OldPoint.x);		RwIm2DVertexSetScreenY(&m_RectangleVertex[1], (RwReal)CurPoint.y);
	RwIm2DVertexSetScreenX(&m_RectangleVertex[2], (RwReal)CurPoint.x);		RwIm2DVertexSetScreenY(&m_RectangleVertex[2], (RwReal)CurPoint.y);
	RwIm2DVertexSetScreenX(&m_RectangleVertex[3], (RwReal)CurPoint.x);		RwIm2DVertexSetScreenY(&m_RectangleVertex[3], (RwReal)OldPoint.y);
	RwIm2DVertexSetScreenX(&m_RectangleVertex[4], (RwReal)OldPoint.x);		RwIm2DVertexSetScreenY(&m_RectangleVertex[4], (RwReal)OldPoint.y);
}

//------------------------------------------------------------------
//	FuncName	: RenderSelectPatch
//	Desc		: 렌더웨어에서 제공하는 버텍스 구조를 그때로 쓰기 때문에 fvf 등을 따로 해주지 않아도 된다.
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusFramework::RenderSelectRectangle()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	RwD3D9SetTransform(D3DTS_WORLD, matWorld);

	RwD3D9SetTexture(NULL, 0);

	RwIm2DRenderPrimitive(rwPRIMTYPEPOLYLINE, m_RectangleVertex, 5);
}

//------------------------------------------------------------------
//	FuncName	: AutoTempFileSave
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusFramework::AutoTempFileSave()
{
	if (CVenusPropertyContainer::GetInstance().m_bSave)
	{
		SetCurrentDirectory(CVenusConfig::GetInstance().m_strMainPath.c_str());

		std::string strPathName = CVenusConfig::GetInstance().m_strMainPath.c_str();
		strPathName += "\\";
		if (CVenusConfig::GetInstance().m_strFileName.IsEmpty())
		{
			strPathName += "Venus.bak";
		}
		else
		{
			RwChar strTitle[MAX_PATH];
			strcpy(strTitle, CVenusConfig::GetInstance().m_strFileName.GetBuffer(0));

			strTitle[CVenusConfig::GetInstance().m_strFileName.GetLength() - 4] = '\0';
			
			strPathName += strTitle;
			strPathName += ".bak";
		}

		CVenusPropertyContainer::GetInstance().Save(strPathName.c_str());

		// Auto Save 로 Backup 파일을 만든다고 해서 CVenusPropertyContainer::GetInstance().m_bSave 로 해서는 안된다.
		// 이유는 알아서..
		CVenusPropertyContainer::GetInstance().m_bSave = TRUE;
	}
}

RwBool CVenusFramework::CreateWorld() 
{
	if(CVenusConfig::GetInstance().m_strWorldPath.empty())
		return FALSE;

	// set texture folder path
	_getcwd(dGET_WORLD_PARAM()->CurWorkingFolderName, 256);
	strcpy(dGET_WORLD_PARAM()->CurDefaultTexPath, dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcat(dGET_WORLD_PARAM()->CurDefaultTexPath, "\\texture\\ntlwe\\");

	// set mesh folder path	
	strcpy(dGET_WORLD_PARAM()->WorldProjectFolderName, dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcat(dGET_WORLD_PARAM()->WorldProjectFolderName, CVenusConfig::GetInstance().m_strWorldPath.c_str());
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);


	// create a global world param
	if (FILE* pFile = fopen("#######.gwp", "rb"))
	{
		LoadFileWorldState(dGET_WORLD_PARAM(), pFile);
		fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	m_pWorldEntity = static_cast<CNtlPLWorldEntity*>(GetSceneManager()->CreateEntity(PLENTITY_WORLD, "NtlWorld"));	
	if(!m_pWorldEntity)
		return FALSE;

	m_pWorldEntity->CreateWorld((RwV3d)ZeroAxis);

	m_pWorld = m_pWorldEntity->GetWorld();

	return TRUE;	
}

RwBool CVenusFramework::VENUS_PLInit() 
{
	CNtlPLGlobal::Init();
	if(!CNtlPLEntityFreeList::AllocFreeList())
		return FALSE;

	if(!CNtlPLAttach::AllocFreeList())
		return FALSE;

	if(!CNtlResourceScheduleManager::AllocFreeList())
		return FALSE;

	//----- 여기서부터 Venus에 맞게 수정한 내용

	CNtlEffectSystemFreeList::m_strClumpTexturePath = CVenusConfig::GetInstance().m_strClumpTexturePath.c_str();

	CNtlEffectSystemFreeList::m_ClumpFileManager.AddExt(".dff");
	CNtlEffectSystemFreeList::m_ClumpFileManager.LoadPath(CVenusConfig::GetInstance().m_strClumpPath.c_str());

	CNtlEffectSystemFreeList::m_AnimationFileManager.AddExt(".anm");
	CNtlEffectSystemFreeList::m_AnimationFileManager.LoadPath(CVenusConfig::GetInstance().m_strAnimationPath.c_str());

	CNtlEffectSystemFreeList::m_UvAnimationFileManager.AddExt(".uva");
	CNtlEffectSystemFreeList::m_UvAnimationFileManager.LoadPath(CVenusConfig::GetInstance().m_strUvAnimationPath.c_str());

	CNtlEffectSystemFreeList::m_TextureFileManager.AddExt(".bmp");
	CNtlEffectSystemFreeList::m_TextureFileManager.AddExt(".tga");
	CNtlEffectSystemFreeList::m_TextureFileManager.AddExt(".png");
	CNtlEffectSystemFreeList::m_TextureFileManager.AddExt(".dds");
	CNtlEffectSystemFreeList::m_TextureFileManager.LoadPath(CVenusConfig::GetInstance().m_strEffectTexturePath.c_str());

	// Sound
	CNtlEffectSystemFreeList::m_SoundFileManager.AddExt(".mp3");
	CNtlEffectSystemFreeList::m_SoundFileManager.AddExt(".wav");
	CNtlEffectSystemFreeList::m_SoundFileManager.AddExt(".ogg");
	CNtlEffectSystemFreeList::m_SoundFileManager.LoadPath(CVenusConfig::GetInstance().m_strSoundPath.c_str());

	CNtlEffectSystemFreeList::Create();

	CVenusPropertyContainer::GetInstance().Create();
	CVenusVisualManager::GetInstance().Create();	
	CVenusPlayerEventManager::GetInstance().Create();
	((CMeshLayer*)gGetEditLayer(EDIT_LAYER_MESH))->InitializeList();

	((CTextureLayer*)gGetEditLayer(EDIT_LAYER_TEXTURE))->BuildTextureList();

	((CEventLayer*)gGetEditLayer(EDIT_LAYER_EVENT))->ResetPropertiesEnemy();

	// 데이타를 다 읽고 난 다음에는 기본 디렉토리로 되돌려준다
	SetCurrentDirectory(CVenusConfig::GetInstance().m_strMainPath.c_str());

    // 바운딩 스피어 업데이트 True
    CNtlResourceEffect::m_bUpdateBoundingSphere = TRUE;

	return TRUE;
}

void CVenusFramework::VENUS_PLTerminate() 
{
	CVenusPlayerEventManager::GetInstance().Delete();

	CNtlPLResourceManager::GetInstance()->Destroy();
	
	//---- 수정한 내용	
	CNtlEffectSystemFreeList::Destroy();	
	//////////////////////////////////////////////////////////////////////////

	CNtlPLEntityFreeList::DeAllocFreeList();
	CNtlPLAttach::DeAllocFreeList();
	CNtlPLGlobal::Terminate();
	CNtlResourceScheduleManager::DeAllocFreeList();

}