#include "precomp_ntlpresentation.h"
#include "NtlPLApi.h"

#include "NtlPLWorldState.h"

#include "NtlPLVisualManager.h"
#include "NtlPLResourceManager.h"
#include "NtlPLOptionManager.h"
#include "NtlSpecularManager.h"


#define SPECULAR_FILENAME		"specular"
#define SPECULAR_PATH			".\\Texture\\ntlwe\\Water\\"

#define SPECULAR_TEXTURE_SIZE	512
#define	SPECULAR_DISTANCE		512
#define	SPECULAR_RADIUS			0.5f

CNtlSpecularManager* CNtlSpecularManager::g_pSpecularManager = NULL;

CNtlSpecularManager::CNtlSpecularManager()
{
	m_pCamera			= NULL;
	m_pFrame			= NULL;

	m_pCameraFilter		= NULL;
	m_pFrameFilter		= NULL;
	m_pRasterFilter		= NULL;
	m_pTextureFilter	= NULL;
	m_pTextureReflection= NULL;

	m_uiAlphaOrigin		= 0;
	m_uiAlphaPresent	= 0;
	m_uiAlphaFade		= 0;
	m_fFadeTime			= 0.0f;
	m_fFadeElapsed		= 0.0f;
	m_bFading			= FALSE;

	m_fOffsetY			= 0.0f;
	m_fOffsetNear		= 0.0f;
	m_hDeviceRestore	= NULL;
}

CNtlSpecularManager::~CNtlSpecularManager()
{
}

void CNtlSpecularManager::CreateInstance()
{
	if (!g_pSpecularManager)
	{
		g_pSpecularManager						= NTL_NEW CNtlSpecularManager;
		g_pSpecularManager->m_hDeviceRestore	= CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRestore(g_pSpecularManager, &CNtlSpecularManager::OnDeviceRestore);
	}
}

void CNtlSpecularManager::DestroyInstance()
{
	if (g_pSpecularManager)
	{
		CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRestore(g_pSpecularManager->m_hDeviceRestore);
		NTL_DELETE(g_pSpecularManager);
		g_pSpecularManager = NULL;
	}
}

int	CNtlSpecularManager::OnDeviceRestore()
{
  	RunableFade(m_uiAlphaFade, 1.0f);
	return 1;
}

RwBool CNtlSpecularManager::Create()
{
	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED, 0);

	if (CreateCamera() && CreateCameraFilter())
	{
		SetIm2DVertexRect(0.0f, 0.0f, (RwReal)RwRasterGetWidth(m_pRasterFilter), (RwReal)RwRasterGetHeight(m_pRasterFilter), m_pCameraFilter);
		SetIm2DVertexRGBA(255, 255, 255, 255);

		return TRUE;
	}
	Destroy();
	return FALSE;
}

void CNtlSpecularManager::Destroy()
{
	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED);

	DestroyCameraFilter();
	DestoryCamera();
}

RwBool CNtlSpecularManager::CreateCamera()
{
	m_pCamera = RwCameraCreate();
	if (!m_pCamera)
	{
		return FALSE;
	}

	RwCameraSetProjection(m_pCamera, rwPARALLEL);
	RwCameraSetNearClipPlane(m_pCamera, 0.001f);
	RwCameraSetFarClipPlane(m_pCamera, 2048.0f);

	m_pFrame = RwFrameCreate();
	if (!m_pFrame)
	{
		return FALSE;
	}
	RwCameraSetFrame(m_pCamera, m_pFrame);

	return TRUE;
}

void CNtlSpecularManager::DestoryCamera()
{
	if (m_pCamera)
	{
		RwCameraSetFrame(m_pCamera, NULL);
		RwCameraDestroy(m_pCamera);
		m_pCamera			= NULL;
	}
	if (m_pFrame)
	{
		RwFrameDestroy(m_pFrame);
		m_pFrame = NULL;
	}
}

RwBool CNtlSpecularManager::CreateCameraFilter()
{
	m_pCameraFilter = RwCameraCreate();
	if (!m_pCameraFilter)
	{
		return FALSE;
	}
	RwCameraSetProjection(m_pCameraFilter, rwPARALLEL);

	m_pFrameFilter = RwFrameCreate();
	if (!m_pFrameFilter)
	{
		return FALSE;
	}
	RwCameraSetFrame(m_pCameraFilter, m_pFrameFilter);

	m_pRasterFilter = RwRasterCreate(SPECULAR_TEXTURE_SIZE, SPECULAR_TEXTURE_SIZE, 0, rwRASTERFORMAT8888 | rwRASTERTYPECAMERATEXTURE);
	if (!m_pRasterFilter)
	{
		return FALSE;
	}
	RwCameraSetRaster(m_pCameraFilter, m_pRasterFilter);

	m_pTextureFilter = RwTextureCreate(NULL);
	DBO_ASSERT(m_pTextureFilter, "Texture create failed.");
	if (!m_pTextureFilter)
	{
		return FALSE;
	}
	RwTextureSetRaster(m_pTextureFilter, m_pRasterFilter);
	RwTextureSetFilterMode(m_pTextureFilter, rwFILTERLINEAR);
	RwTextureSetAddressing(m_pTextureFilter, rwTEXTUREADDRESSCLAMP);

	m_pTextureReflection = GetNtlResourceManager()->LoadTexture(SPECULAR_FILENAME, SPECULAR_PATH);
	DBO_ASSERT(m_pTextureReflection, "Texture load failed.");

	if (!m_pTextureReflection)
	{
		return FALSE;
	}

	return TRUE;
}

void CNtlSpecularManager::DestroyCameraFilter()
{
	if (m_pTextureReflection)
	{
		GetNtlResourceManager()->UnLoadTexture(m_pTextureReflection);
		m_pTextureReflection = NULL;
	}
	if (m_pTextureFilter)
	{
		RwTextureSetRaster(m_pTextureFilter, NULL);
		RwTextureDestroy(m_pTextureFilter);
		m_pTextureFilter = NULL;
	}
	if (m_pCameraFilter)
	{
		RwCameraSetFrame(m_pCameraFilter, NULL);
		RwCameraSetRaster(m_pCameraFilter, NULL);
		RwCameraDestroy(m_pCameraFilter);
		m_pCameraFilter = NULL;
	}
	if (m_pFrameFilter)
	{
		RwFrameDestroy(m_pFrameFilter);
		m_pFrameFilter = NULL;
	}
	if (m_pRasterFilter)
	{
		RwRasterDestroy(m_pRasterFilter);
		m_pRasterFilter = NULL;
	}
}

void CNtlSpecularManager::HandleEvents(RWS::CMsg &pMsg)
{
	if (CheckEventsWorldEffectChanged(pMsg))
	{
		sNTL_FIELD_PROP* pNtlFieldProp = reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);
		SetSpecular(pNtlFieldProp->_NtlSpecular);
	}
}

void CNtlSpecularManager::Update(RwReal fElapsed)
{
	if (!CNtlPLOptionManager::GetInstance()->GetWaterSpecular())
	{
		return;
	}

	if (m_sNtlSpecularNew._SpecularExistence)
	{
 		if (!m_bFading && !m_uiAlphaPresent)
		{
			SetSpecular(m_sNtlSpecularNew, TRUE, TRUE);
		}
	}

	if (m_bFading)
	{
		UpdateFade(fElapsed);
		RenderToTexture();
	}

	if (m_sNtlSpecular._SpecularExistence)
	{
		UpdateCamera(fElapsed);
	}
}

void CNtlSpecularManager::UpdateCamera(RwReal fElapsed)
{
	CalcCameraFrustum();
	CalcMatrixTexture();
}

void CNtlSpecularManager::CalcCameraFrustum()
{
 	RwMatrix*	pmatLight	= RwFrameGetMatrix(m_pFrame);
	RwV3d		vAt			= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwV3d		vPos;

	RwV3dScale(&vPos, &m_vDirection, SPECULAR_DISTANCE + m_fOffsetNear);
 	RwV3dIncrementScaled(&vAt, &m_vDirection, m_fOffsetNear);
	vPos.x	= vAt.x + vPos.x;
	vPos.y	= vAt.y - m_fOffsetY;
	vPos.z	= vAt.z + vPos.z;
	API_PL_CameraMatrixLookAt(pmatLight, &vPos, &vAt);
	RwFrameUpdateObjects(m_pFrame);	
}

void CNtlSpecularManager::CalcMatrixTexture()
{
	RwMatrix    matCamera;
	RwMatrix    matInvLight;
	RwV3d       vScaling;
	RwV3d		vTranslation;

	RwMatrixCopy(&matCamera, RwFrameGetLTM(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	matCamera.right.x = -matCamera.right.x;
	matCamera.right.y = -matCamera.right.y;
	matCamera.right.z = -matCamera.right.z;

	RwMatrixInvert(&matInvLight, RwFrameGetMatrix(m_pFrame));

	vScaling.x = -0.5f / m_pCamera->viewWindow.x;
	vScaling.y = -0.5f / m_pCamera->viewWindow.y;
	vScaling.z = 0.0f;								// 1.0f / (m_pCamera->farPlane - m_pCamera->nearPlane)

	RwMatrixScale(&matInvLight, &vScaling, rwCOMBINEPOSTCONCAT);

	vTranslation.x = vTranslation.y = 0.5f;	vTranslation.z = 0.0f;
	RwMatrixTranslate(&matInvLight, &vTranslation, rwCOMBINEPOSTCONCAT);

	RwMatrixMultiply(&m_matTexture, &matCamera, &matInvLight);

	((D3DMATRIX*)&m_matTexture)->_14 = 0.0f;
	((D3DMATRIX*)&m_matTexture)->_24 = 0.0f;
	((D3DMATRIX*)&m_matTexture)->_34 = 0.0f;
	((D3DMATRIX*)&m_matTexture)->_44 = 1.0f;
}

RwTexture* CNtlSpecularManager::GetTexture()
{
	return m_pTextureFilter;
}

RwMatrix CNtlSpecularManager::GetMatrixTexture()
{
	return m_matTexture;
}

RwBool	CNtlSpecularManager::IsInSpecualr(const RwSphere* pSphere)
{
	if (m_pCamera && m_pFrame && m_sNtlSpecular._SpecularExistence && m_uiAlphaPresent)
	{
		if (RwCameraFrustumTestSphere(m_pCamera, pSphere))
		{
			return TRUE;
		} 
	} 
	return FALSE;
}

void CNtlSpecularManager::RenderToTexture()
{ 
	RwCamera* pOldCamera = RwCameraGetCurrentCamera();
	
	if (pOldCamera)
	{
		RwCameraEndUpdate(pOldCamera);
	}

	static RwRGBA bgColor = {0, 0, 0, 0};
	if (RwCameraClear(m_pCameraFilter, &bgColor, rwCAMERACLEARIMAGE))
	{
		if (RwCameraBeginUpdate(m_pCameraFilter))
		{
			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE); 
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);

			RwD3D9SetTexture(m_pTextureReflection, 0);

			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_aIm2DBuffer, 4);

			RwD3D9SetTexture(NULL, 0);

			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
			RwCameraEndUpdate(m_pCameraFilter);
		}
	}

	if (pOldCamera)
	{
		RwCameraBeginUpdate(pOldCamera);
	}	
}

void CNtlSpecularManager::SetIm2DVertexRect(RwReal fLeft, RwReal fTop, RwReal fRight, RwReal fBottom, RwCamera* pCamera)
{
	RwReal fRecipZ	= 1.0f / RwCameraGetNearClipPlane(pCamera);
	RwReal fNearZ	= RwIm2DGetNearScreenZ();

	RwIm2DVertexSetScreenX(&m_aIm2DBuffer[0], fLeft);
	RwIm2DVertexSetScreenY(&m_aIm2DBuffer[0], fTop);
	RwIm2DVertexSetScreenZ(&m_aIm2DBuffer[0], fNearZ);
	RwIm2DVertexSetRecipCameraZ(&m_aIm2DBuffer[0], fRecipZ);
	RwIm2DVertexSetU(&m_aIm2DBuffer[0], 0.0f, fRecipZ);
	RwIm2DVertexSetV(&m_aIm2DBuffer[0], 0.0f, fRecipZ);

	RwIm2DVertexSetScreenX(&m_aIm2DBuffer[1], fLeft);
	RwIm2DVertexSetScreenY(&m_aIm2DBuffer[1], fBottom);
	RwIm2DVertexSetScreenZ(&m_aIm2DBuffer[1], fNearZ);
	RwIm2DVertexSetRecipCameraZ(&m_aIm2DBuffer[1], fRecipZ);
	RwIm2DVertexSetU(&m_aIm2DBuffer[1], 0.0f, fRecipZ);
	RwIm2DVertexSetV(&m_aIm2DBuffer[1], 1.0f, fRecipZ);

	RwIm2DVertexSetScreenX(&m_aIm2DBuffer[2], fRight);
	RwIm2DVertexSetScreenY(&m_aIm2DBuffer[2], fTop);
	RwIm2DVertexSetScreenZ(&m_aIm2DBuffer[2], fNearZ);
	RwIm2DVertexSetRecipCameraZ(&m_aIm2DBuffer[2], fRecipZ);
	RwIm2DVertexSetU(&m_aIm2DBuffer[2], 1.0f, fRecipZ);
	RwIm2DVertexSetV(&m_aIm2DBuffer[2], 0.0f, fRecipZ);

	RwIm2DVertexSetScreenX(&m_aIm2DBuffer[3], fRight);
	RwIm2DVertexSetScreenY(&m_aIm2DBuffer[3], fBottom);
	RwIm2DVertexSetScreenZ(&m_aIm2DBuffer[3], fNearZ);
	RwIm2DVertexSetRecipCameraZ(&m_aIm2DBuffer[3], fRecipZ);
	RwIm2DVertexSetU(&m_aIm2DBuffer[3], 1.0f, fRecipZ);
	RwIm2DVertexSetV(&m_aIm2DBuffer[3], 1.0f, fRecipZ);
}

void CNtlSpecularManager::SetIm2DVertexRGBA(RwUInt8 uiRed, RwUInt8 uiGreen, RwUInt8 uiBlue, RwUInt8 uiAlpha)
{
	for (int i = 0; i < 4; ++i)
	{
		RwIm2DVertexSetIntRGBA(&m_aIm2DBuffer[i], uiRed, uiGreen, uiBlue, uiAlpha);
	}
}

void CNtlSpecularManager::RunableFade(RwUInt8 uiAlpha, RwReal fTime)
{
	m_uiAlphaOrigin = m_uiAlphaPresent;
	m_uiAlphaFade	= uiAlpha;
	m_fFadeTime		= fTime;
	m_fFadeElapsed	= 0.0f;
	m_bFading		= TRUE;
}

void CNtlSpecularManager::UpdateFade(RwReal fElapsed)
{
	m_fFadeElapsed += fElapsed;

	RwReal fDelta = m_fFadeElapsed / m_fFadeTime;
	if(fDelta > 1.0f)
	{
		m_bFading	= FALSE;
		fDelta		= 1.0f;
	}

	m_uiAlphaPresent = RwInt32FromRealMacro((RwReal)m_uiAlphaOrigin + (fDelta * (RwReal)(m_uiAlphaFade - m_uiAlphaOrigin)));
	SetIm2DVertexRGBA(255, 255, 255, m_uiAlphaPresent);
}

void CNtlSpecularManager::SetSpecular(const sNTL_SPECULAR& sNtlSpecular, RwBool bCompareCheck, RwBool bActive)
{
	if (bActive)
	{
		if (bCompareCheck)
		{
			if (m_sNtlSpecular.Compare(sNtlSpecular) && m_uiAlphaPresent == sNtlSpecular._Power)
			{
				return;
			}
		}
		m_sNtlSpecular = sNtlSpecular;

		RwV2d vw;
		vw.x = SPECULAR_RADIUS * m_sNtlSpecular._Scale.x;
		vw.y = SPECULAR_RADIUS * m_sNtlSpecular._Scale.y;
		RwCameraSetViewWindow(m_pCamera, &vw);	

		m_vDirection	= m_sNtlSpecular._Direction;
 		m_vDirection.y	= 0.0f;
		m_fOffsetNear	= m_pCamera->viewWindow.y;
		m_fOffsetY		= m_sNtlSpecular._Direction.y;

		RunableFade(m_sNtlSpecular._Power, 1.0f);
	}
	else
	{
		if (bCompareCheck)
		{
			if (m_sNtlSpecularNew.Compare(sNtlSpecular))
			{
				return;
			}
		}

		m_sNtlSpecularNew = sNtlSpecular;

		RunableFade(0, 1.0f);
	}
}

/*
#define SPECULAR_DISTANCE		512
#define	SPECULAR_TEXTURE_SIZE	512
//#define dSPECULARMANAGER_USE_PROJ_PERSPECTIVE // 원근투영모드로 변환시 제대로 동작하지 않음. 정보 차원으로 소스 삭제 안함.
// DEFALT		: 평행투영
// PERSPECTIVE	: 거리에 따른 스케일										
// PARALLEL		: 크기에 따른 스케일

CNtlSpecularManager::CNtlSpecularManager()
{
	m_pCamera			= NULL;
	m_pFrame			= NULL;

	m_pCameraFilter		= NULL;
	m_pFrameFilter		= NULL;
	m_pRasterFilter		= NULL;
	m_pTextureFilter	= NULL;
	m_pTextureReflection= NULL;

	m_pActiveSun		= NULL;
	
	m_fWidth			= 0.0f;
	m_fHeight			= 0.0f;

	m_uiAlphaOrigin		= 0;
	m_uiAlphaPresent	= 0;
	m_uiAlphaFade		= 0;
	m_fFadeTime			= 0.0f;
	m_fFadeElapsed		= 0.0f;
	m_bFading			= FALSE;
}

CNtlSpecularManager::~CNtlSpecularManager()
{
}

RwBool CNtlSpecularManager::Create()
{
	if (CreateCamera() && CreateCameraFilter())
	{
		SetIm2DVertexRect(0.0f, 0.0f, (RwReal)RwRasterGetWidth(m_pRasterFilter), (RwReal)RwRasterGetHeight(m_pRasterFilter), m_pCameraFilter);
		SetIm2DVertexRGBA(255, 255, 255, 255);
		return TRUE;
	}
	Destroy();
	return FALSE;
}

void CNtlSpecularManager::Destroy()
{
	DestroyCameraFilter();
	DestoryCamera();
}

RwBool CNtlSpecularManager::CreateCamera()
{
	m_pCamera = RwCameraCreate();
	if (!m_pCamera)
	{
		return FALSE;
	}

#ifdef dSPECULARMANAGER_USE_PROJ_PERSPECTIVE
	RwCameraSetProjection(m_pCamera, rwPERSPECTIVE);	
#else
	RwCameraSetProjection(m_pCamera, rwPARALLEL);
#endif
	RwCameraSetNearClipPlane(m_pCamera, 0.001f);
	RwCameraSetFarClipPlane(m_pCamera, 2000.0f);

	m_pFrame = RwFrameCreate();
	if (!m_pFrame)
	{
		return FALSE;
	}
	RwCameraSetFrame(m_pCamera, m_pFrame);
	
	return TRUE;
}

void CNtlSpecularManager::DestoryCamera()
{
	if (m_pCamera)
	{
		RwCameraSetFrame(m_pCamera, NULL);
		RwCameraDestroy(m_pCamera);
		m_pCamera			= NULL;
	}
	if (m_pFrame)
	{
		RwFrameDestroy(m_pFrame);
		m_pFrame = NULL;
	}
}

RwBool CNtlSpecularManager::CreateCameraFilter()
{
	m_pCameraFilter = RwCameraCreate();
	if (!m_pCameraFilter)
	{
		return FALSE;
	}
	RwCameraSetProjection(m_pCameraFilter, rwPARALLEL);

	m_pFrameFilter = RwFrameCreate();
	if (!m_pFrameFilter)
	{
		return FALSE;
	}
	RwCameraSetFrame(m_pCameraFilter, m_pFrameFilter);

	m_pRasterFilter = RwRasterCreate(SPECULAR_TEXTURE_SIZE, SPECULAR_TEXTURE_SIZE, 0, rwRASTERFORMAT8888 | rwRASTERTYPECAMERATEXTURE);
	if (!m_pRasterFilter)
	{
		return FALSE;
	}
	RwCameraSetRaster(m_pCameraFilter, m_pRasterFilter);

	m_pTextureFilter = RwTextureCreate(NULL);
	if (!m_pTextureFilter)
	{
		return FALSE;
	}
	RwTextureSetRaster(m_pTextureFilter, m_pRasterFilter);
	RwTextureSetAddressing(m_pTextureFilter, rwTEXTUREADDRESSCLAMP);

	m_pTextureReflection = GetNtlResourceManager()->LoadTexture("TEST_reflection", PATH_EFFECT_TEXTURE);
	
	return TRUE;
}

void CNtlSpecularManager::DestroyCameraFilter()
{
	if (m_pTextureReflection)
	{
		GetNtlResourceManager()->UnLoadTexture(m_pTextureReflection);
		m_pTextureReflection = NULL;
	}
	if (m_pTextureFilter)
	{
		RwTextureSetRaster(m_pTextureFilter, NULL);
		RwTextureDestroy(m_pTextureFilter);
		m_pTextureFilter = NULL;
	}
	if (m_pCameraFilter)
	{
		RwCameraSetFrame(m_pCameraFilter, NULL);
		RwCameraSetZRaster(m_pCameraFilter, NULL);
		RwCameraSetRaster(m_pCameraFilter, NULL);
		RwCameraDestroy(m_pCameraFilter);
		m_pCameraFilter = NULL;
	}
	if (m_pFrameFilter)
	{
		RwFrameDestroy(m_pFrameFilter);
		m_pFrameFilter = NULL;
	}
	if (m_pRasterFilter)
	{
		RwRasterDestroy(m_pRasterFilter);
		m_pRasterFilter = NULL;
	}
}


void CNtlSpecularManager::Update(RwReal fElapsed)
{
	CNtlPLSun*				pActiveSun		= NULL;
	CNtlPLVisualManager*	pVisualManager	= (CNtlPLVisualManager*)GetSceneManager();
	CNtlPLRenderGroup*		pSunGroup		= pVisualManager->FindInstanceGroup(PLENTITY_SUN);
	if (pSunGroup)
	{ 
		CNtlPLRenderGroup::MapEntity::iterator	it;
		CNtlPLRenderGroup::MapEntity*			pmapSun = pSunGroup->GetEntities();
		for (it = pmapSun->begin(); it != pmapSun->end(); ++it)
		{
			CNtlPLSun* pSun = (CNtlPLSun*)(*it).second;
			if (pSun->UseLensFlare())
			{
				pActiveSun = pSun;
				break;
			}
		}
	}

	if (pActiveSun && !m_bFading && !m_uiAlphaPresent)
	{
		SetActiveSun(pActiveSun);
		RunableFade(1.0f, 1.0f);
	}
	else if (pActiveSun != m_pActiveSun && m_uiAlphaPresent)
	{
		RunableFade(0.0f, 1.0f);
	}

	if (m_bFading)
	{
		UpdateFade(fElapsed);
		RenderToTexture();
	}
	UpdateCamera(fElapsed);
}

void CNtlSpecularManager::SetActiveSun(CNtlPLSun* pSun)
{
	m_pActiveSun = pSun;
	if (m_pActiveSun)
	{
		m_fWidth		= dSUN_RADIUS * m_pActiveSun->GetScale();
		m_fHeight		= dSUN_RADIUS * m_pActiveSun->GetScale();
		m_vPosSun		= m_pActiveSun->GetPosition();

		RwV2d vw;
#ifdef dSPECULARMANAGER_USE_PROJ_PERSPECTIVE
		vw.x = m_fWidth		* 0.5f / (m_fWidth + m_fHeight); // (m_fWidth + m_fHeight) = NearPlane
		vw.y = m_fHeight	* 0.5f / (m_fWidth + m_fHeight); 
#else
		vw.x = m_fWidth;
		vw.y = m_fHeight;
#endif
		RwCameraSetViewWindow(m_pCamera, &vw);		
	}
}

void CNtlSpecularManager::UpdateCamera(RwReal fElapsed)
{
	CalcCameraFrustum();
	CalcMatrixTexture();
}

#ifdef dSPECULARMANAGER_USE_PROJ_PERSPECTIVE
void CNtlSpecularManager::CalcCameraFrustum()
{
	RwMatrix*	pmatLight	= RwFrameGetMatrix(m_pFrame);
	RwMatrix*	pmatCamera	= RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
	RwV3d		vAt			= *RwMatrixGetPos(pmatCamera);
	RwV3d		vPos;

	vPos.x	= vAt.x + m_vPosSun.x;
	vPos.y	= vAt.y - m_vPosSun.y;
	vPos.z	= vAt.z + m_vPosSun.z;
	API_PL_CameraMatrixLookAt(pmatLight, &vPos, RwMatrixGetPos(pmatCamera));
	RwV3dIncrementScaled(RwMatrixGetPos(pmatLight), RwMatrixGetAt(pmatLight), -SPECULAR_DISTANCE);

	RwMatrixUpdate(pmatLight);
	RwFrameUpdateObjects(m_pFrame);	
}

void CNtlSpecularManager::CalcMatrixTexture()
{
	RwMatrix    matCamera;
	RwMatrix    matInvLight;
	RwV3d       vScaling;
	RwV3d		vTranslation;

	RwMatrixCopy(&matCamera, RwFrameGetLTM(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	matCamera.right.x = -matCamera.right.x;
	matCamera.right.y = -matCamera.right.y;
	matCamera.right.z = -matCamera.right.z;

	RwMatrixInvert(&matInvLight, RwFrameGetMatrix(m_pFrame));
	 
 	RwReal fViewWindowX = m_pCamera->viewWindow.x * (m_pCamera->farPlane - (m_fWidth + m_fHeight));
	RwReal fViewWindowY = m_pCamera->viewWindow.y * (m_pCamera->farPlane - (m_fWidth + m_fHeight));
   
	vScaling.x = -0.5f / fViewWindowX;
	vScaling.y = -0.5f / fViewWindowY;
	vScaling.z = (m_fWidth + m_fHeight) / (m_pCamera->farPlane - (m_fWidth + m_fHeight));

	RwMatrixScale(&matInvLight, &vScaling, rwCOMBINEPOSTCONCAT);

	vTranslation.x = vTranslation.y = 0.5f;	vTranslation.z = 0.0f;
	RwMatrixTranslate(&matInvLight, &vTranslation, rwCOMBINEPOSTCONCAT);

	RwMatrixMultiply(&m_matTexture, &matCamera, &matInvLight);

	((D3DMATRIX*)&m_matTexture)->_14 = 0.0f;
	((D3DMATRIX*)&m_matTexture)->_24 = 0.0f;
	((D3DMATRIX*)&m_matTexture)->_34 = 0.0f;
	((D3DMATRIX*)&m_matTexture)->_44 = 1.0f;
}
#else
//void CNtlSpecularManager::CalcCameraFrustum()
//{
//	RwMatrix*	pmatLight	= RwFrameGetMatrix(m_pFrame);
//	RwMatrix*	pmatCamera	= RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
//	RwV3d		vAt			= *RwMatrixGetPos(pmatCamera);
//	RwV3d		vPos;
//
//	vPos.x	= vAt.x + m_vPosSun.x;
//	vPos.y	= vAt.y - m_vPosSun.y;
//	vPos.z	= vAt.z + m_vPosSun.z;
//	API_PL_CameraMatrixLookAt(pmatLight, &vPos, &vAt);
//
//	RwMatrixUpdate(pmatLight);
//	RwFrameUpdateObjects(m_pFrame);	
//}

void CNtlSpecularManager::CalcCameraFrustum() //높이 고정 투영
{
	RwMatrix*	pmatLight	= RwFrameGetMatrix(m_pFrame);
	RwV3d		vAt			= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwV3d		vPos;

	vPos.x	= vAt.x + m_vPosSun.x;
	vPos.y	= vAt.y;
	vPos.z	= vAt.z + m_vPosSun.z;

	RwV3d		vTemp;
	RwV3dSubMacro(&vTemp, &vAt, &vPos);
	RwV3dNormalize(&vTemp, &vTemp);
	RwV3dScale(&vPos, &vTemp, -SPECULAR_DISTANCE);
	vPos.x	= vAt.x + vPos.x;
	vPos.y	= vAt.y - m_vPosSun.y;
	vPos.z	= vAt.z + vPos.z;
	RwV3dIncrementScaled(&vAt, &vTemp, -m_fHeight);

	API_PL_CameraMatrixLookAt(pmatLight, &vPos, &vAt);
	
	RwMatrixUpdate(pmatLight); // 이거 안해도 된다.c
	RwFrameUpdateObjects(m_pFrame);	
}

void CNtlSpecularManager::CalcMatrixTexture()
{
	RwMatrix    matCamera;
	RwMatrix    matInvLight;
	RwV3d       vScaling;
	RwV3d		vTranslation;

	RwMatrixCopy(&matCamera, RwFrameGetLTM(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	matCamera.right.x = -matCamera.right.x;
	matCamera.right.y = -matCamera.right.y;
	matCamera.right.z = -matCamera.right.z;

	RwMatrixInvert(&matInvLight, RwFrameGetMatrix(m_pFrame));

	RwReal fViewWindowX = m_pCamera->viewWindow.x;
	RwReal fViewWindowY = m_pCamera->viewWindow.y;

	vScaling.x = -0.5f / fViewWindowX;
	vScaling.y = -0.5f / fViewWindowY;
	vScaling.z = 1.0f / (m_pCamera->farPlane - m_pCamera->nearPlane);

	RwMatrixScale(&matInvLight, &vScaling, rwCOMBINEPOSTCONCAT);

	vTranslation.x = vTranslation.y = 0.5f;	vTranslation.z = 0.0f;
	RwMatrixTranslate(&matInvLight, &vTranslation, rwCOMBINEPOSTCONCAT);

	RwMatrixMultiply(&m_matTexture, &matCamera, &matInvLight);

	((D3DMATRIX*)&m_matTexture)->_14 = 0.0f;
	((D3DMATRIX*)&m_matTexture)->_24 = 0.0f;
	((D3DMATRIX*)&m_matTexture)->_34 = 0.0f;
	((D3DMATRIX*)&m_matTexture)->_44 = 1.0f;
}
#endif

RwTexture* CNtlSpecularManager::GetTexture()
{
	if (m_pActiveSun && m_pCamera && m_pFrame)
	{
		if (m_uiAlphaPresent)
		{
			return m_pTextureFilter;
		}
	}
	return NULL;
}

RwMatrix CNtlSpecularManager::GetMatrixTexture()
{
	return m_matTexture;
}

RwBool	CNtlSpecularManager::CanRenderSpecualr(const RwSphere* pSphere)
{
	if (m_pActiveSun && m_pCamera && m_pFrame && m_uiAlphaPresent)
	{
		if (RwCameraFrustumTestSphere(m_pCamera, pSphere))
		{
			return TRUE;
		} 
	} 
	return FALSE;
}

void CNtlSpecularManager::RenderToTexture()
{

	RwCamera* pOldCamera = CNtlPLGlobal::m_RwCamera;
	RwCameraEndUpdate(pOldCamera);

	static RwRGBA bgColor = {0, 0, 0, 0};
	if (RwCameraClear(m_pCameraFilter, &bgColor, rwCAMERACLEARIMAGE))
	{
	if (RwCameraBeginUpdate(m_pCameraFilter))
	{
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE); 
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);

	RwD3D9SetTexture(m_pTextureReflection, 0);

	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_aIm2DBuffer, 4);

	RwD3D9SetTexture(NULL, 0);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);

	RwCameraEndUpdate(m_pCameraFilter);
	}
	}
	if (!RwCameraBeginUpdate(pOldCamera))
	{
		NTL_ASSERT(false, "BeginUpdate FAILED");
	}
}

void CNtlSpecularManager::SetIm2DVertexRect(RwReal fLeft, RwReal fTop, RwReal fRight, RwReal fBottom, RwCamera* pCamera)
{
	RwReal fRecipZ	= 1.0f / RwCameraGetNearClipPlane(pCamera);
	RwReal fNearZ	= RwIm2DGetNearScreenZ();

	RwIm2DVertexSetScreenX(&m_aIm2DBuffer[0], fLeft);
	RwIm2DVertexSetScreenY(&m_aIm2DBuffer[0], fTop);
	RwIm2DVertexSetScreenZ(&m_aIm2DBuffer[0], fNearZ);
	RwIm2DVertexSetRecipCameraZ(&m_aIm2DBuffer[0], fRecipZ);
	RwIm2DVertexSetU(&m_aIm2DBuffer[0], 0.0f, fRecipZ);
	RwIm2DVertexSetV(&m_aIm2DBuffer[0], 0.0f, fRecipZ);

	RwIm2DVertexSetScreenX(&m_aIm2DBuffer[1], fLeft);
	RwIm2DVertexSetScreenY(&m_aIm2DBuffer[1], fBottom);
	RwIm2DVertexSetScreenZ(&m_aIm2DBuffer[1], fNearZ);
	RwIm2DVertexSetRecipCameraZ(&m_aIm2DBuffer[1], fRecipZ);
	RwIm2DVertexSetU(&m_aIm2DBuffer[1], 0.0f, fRecipZ);
	RwIm2DVertexSetV(&m_aIm2DBuffer[1], 1.0f, fRecipZ);

	RwIm2DVertexSetScreenX(&m_aIm2DBuffer[2], fRight);
	RwIm2DVertexSetScreenY(&m_aIm2DBuffer[2], fTop);
	RwIm2DVertexSetScreenZ(&m_aIm2DBuffer[2], fNearZ);
	RwIm2DVertexSetRecipCameraZ(&m_aIm2DBuffer[2], fRecipZ);
	RwIm2DVertexSetU(&m_aIm2DBuffer[2], 1.0f, fRecipZ);
	RwIm2DVertexSetV(&m_aIm2DBuffer[2], 0.0f, fRecipZ);

	RwIm2DVertexSetScreenX(&m_aIm2DBuffer[3], fRight);
	RwIm2DVertexSetScreenY(&m_aIm2DBuffer[3], fBottom);
	RwIm2DVertexSetScreenZ(&m_aIm2DBuffer[3], fNearZ);
	RwIm2DVertexSetRecipCameraZ(&m_aIm2DBuffer[3], fRecipZ);
	RwIm2DVertexSetU(&m_aIm2DBuffer[3], 1.0f, fRecipZ);
	RwIm2DVertexSetV(&m_aIm2DBuffer[3], 1.0f, fRecipZ);
}

void CNtlSpecularManager::SetIm2DVertexRGBA(RwUInt8 uiRed, RwUInt8 uiGreen, RwUInt8 uiBlue, RwUInt8 uiAlpha)
{
	for (int i = 0; i < 4; ++i)
	{
		RwIm2DVertexSetIntRGBA(&m_aIm2DBuffer[i], uiRed, uiGreen, uiBlue, uiAlpha);
	}
}

void CNtlSpecularManager::RunableFade(RwReal fDelta, RwReal fTime)
{
	m_uiAlphaOrigin = m_uiAlphaPresent;
	m_uiAlphaFade	= (RwUInt8)(fDelta * 255.0f);
	m_fFadeTime		= fTime;
	m_fFadeElapsed	= 0.0f;
	m_bFading		= TRUE;
}

void CNtlSpecularManager::UpdateFade(RwReal fElapsed)
{
	m_fFadeElapsed += fElapsed;

	RwReal fDelta = m_fFadeElapsed / m_fFadeTime;
	if(fDelta > 1.0f)
	{
		m_bFading	= FALSE;
		fDelta		= 1.0f;
	}

	m_uiAlphaPresent = RwInt32FromRealMacro((RwReal)m_uiAlphaOrigin + (fDelta * (RwReal)(m_uiAlphaFade - m_uiAlphaOrigin)));
	SetIm2DVertexRGBA(255, 255, 255, m_uiAlphaPresent);
}
*/