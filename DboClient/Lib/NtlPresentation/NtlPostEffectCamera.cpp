//***********************************************************************************
//	File		:	NtlBlurCamera.cpp
//	Desc		:	
//	Begin		:	2006. 2.13
//	Copyright	:	ⓒ 2006 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlpresentation.h"
#include "NtlPostEffectCamera.h"
#include "NtlText.h"
#include "NtlPLRenderState.h"
#include "NtlEffectDefine.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLVisualManager.h"
#include "NtlProfiler.h"
#include "NtlPLHeatHaze.h"

#include "NtlPLResourcePack.h"
#include "NtlPLResourceManager.h"


CNtlPostEffectCamera* CNtlPostEffectCamera::m_pInstance = NULL;
RwUInt8				  CNtlPostEffectCamera::m_byPEFilters = 0;


CNtlPostEffectCamera::CNtlPostEffectCamera(void)
{
	m_pInstance				= this;

	m_bEnableCamera			= FALSE;
	m_strShaderName			= ".\\script\\Blur.fx";

	m_BackgroundColor.red	= 0;
	m_BackgroundColor.green	= 0;
	m_BackgroundColor.blue	= 0;
	m_BackgroundColor.alpha	= 0;

	m_pSourceTexture		= NULL;

	m_nBlurTextureWidth		= 128;
	m_nBlurTextureHeight	= 128;

	m_pMainCamera			= NULL;

	m_pTempCamera			= NULL;
	m_pTempTexture			= NULL;

	m_pReductionCamera		= NULL;
	m_pReductionTexture		= NULL;

	m_pFakeHDRCamera		= NULL;
	m_pFakeHDRTexture		= NULL;

	m_lpDevice				= NULL;

	m_lpEffect				= NULL;
	m_hTechnique			= NULL;

	m_hMapWidth				= NULL;
	m_hMapHeight			= NULL;

	m_hMonoPowerHeight		= NULL;
	m_hRGB2Y				= NULL;

	m_fMonoPowerHeight		= dMONO_POWER_DEFAULT;

	m_vRGB2Y				= D3DXVECTOR4(0.29900f, 0.4f, 0.11400f, 0.f);
//	m_vRGB2Y				= D3DXVECTOR4(0.29900f, 0.58700f, 0.11400f, 0.f);

	m_nTextureReductionSize	= 128;

    m_pMotionBlurCamera     = NULL;
    m_pMotionBlurTexture    = NULL;

    m_bEnableBlur           = FALSE;    
    m_fBlurAlpha            = EFFECT_BLUR_ALPHA;
    m_fBlurFadeVelocity     = 0.0f;

	m_handleRelease			= CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRelease(this, &CNtlPostEffectCamera::ReleaseHandler);
	m_handleRestore			= CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRestore(this, &CNtlPostEffectCamera::RestoreHandler);
	
	m_pHeatHazeCamera = NULL;
	m_pHeatHazeTexture = NULL;
	m_pHeatHaze = NULL;

	SetPostEffectFilters(POST_EFFECT_FILTER_NONE);
}


CNtlPostEffectCamera::~CNtlPostEffectCamera(void)
{
	CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRelease(m_handleRelease);
	CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRestore(m_handleRestore);

	m_pInstance		= NULL;
}

CNtlPostEffectCamera* CNtlPostEffectCamera::GetInstance(void)
{
	return m_pInstance;
}

//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::Destroy()
{
	if (m_lpEffect != NULL)
	{
		m_lpEffect->Release();
		m_lpEffect = NULL;

		m_hTechnique			= NULL;
	}

	// Delete Camera
    DestroyBlurCamera(m_pMotionBlurCamera, m_pMotionBlurTexture);

	DestroyBlurCamera(m_pReductionCamera, m_pReductionTexture);

	DestroyBlurCamera(m_pFakeHDRCamera, m_pFakeHDRTexture);

	DestroyBlurCamera(m_pTempCamera, m_pTempTexture);

	DestroyBlurCamera(m_pCamera, m_pSourceTexture);

	DestroyBlurCamera(m_pHeatHazeCamera, m_pHeatHazeTexture);

	m_pCamera = m_pMainCamera;
	m_pMainCamera = NULL;

	CNtlCamera::Destroy();

	if(m_pHeatHaze)
	{
		NTL_DELETE(m_pHeatHaze);
	}
}

void CNtlPostEffectCamera::DestroyBlurCamera(RwCamera*& pCamera, RwTexture*& pTexture)
{
	if(pCamera != NULL)
	{
		RwFrame *frame;
		RwRaster *raster;

		frame = RwCameraGetFrame(pCamera);
		RwCameraSetFrame(pCamera, NULL);
		RwFrameDestroy(frame);

		raster = RwCameraGetRaster(pCamera);
		RwCameraSetRaster(pCamera, NULL);
		RwRasterDestroy(raster);

		raster = RwCameraGetZRaster(pCamera);
		RwCameraSetZRaster(pCamera, NULL);
		RwRasterDestroy(raster);

		RwCameraDestroy(pCamera);
		pCamera = NULL;

		if (pTexture != NULL)
		{
			RwTextureSetRaster(pTexture, NULL);
			RwTextureDestroy(pTexture);
			pTexture = NULL;
		}
	}
}

//------------------------------------------------------------------
//	FuncName	: CreateBlurCamera
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlPostEffectCamera::CreateBlurCamera(RwCamera*& pCamera, RwTexture*& pTexture, RwInt32 nWidth, RwInt32 nHeight, RwReal fNearClipPlane, RwReal fFarClipPlane)
{
	RwRaster *raster = RwRasterCreate(nWidth, nHeight, 0, rwRASTERTYPECAMERATEXTURE);
	if(raster != NULL)
	{
		RwRaster* zRaster = RwRasterCreate(nWidth, nHeight, 0, rwRASTERTYPEZBUFFER);
		if(zRaster != NULL)
		{
			RwFrame *frame = RwFrameCreate();
			if(frame != NULL)
			{
				pCamera = RwCameraCreate();
				if(pCamera != NULL)
				{
					//RwV2d vw;

					RwCameraSetRaster(pCamera, raster);
					RwCameraSetZRaster(pCamera, zRaster);

					RwCameraSetFrame(pCamera, frame);

					RwCameraSetNearClipPlane(pCamera, fNearClipPlane);
					RwCameraSetFarClipPlane(pCamera, fFarClipPlane);

					/*
					vw.x = vw.y = 0.4f;
					RwCameraSetViewWindow(pCamera, &vw);
					*/
                    CNtlCamera::SetCalcFov(pCamera, GetFov());
					
					pTexture = RwTextureCreate(raster);
					DBO_ASSERT(pTexture, "Texture create failed.");

					RwTextureSetFilterMode(pTexture, rwFILTERLINEAR);
					RwTextureSetAddressing(pTexture, rwTEXTUREADDRESSCLAMP);

					return TRUE;
				}
				RwFrameDestroy(frame);
			}
			RwRasterDestroy(zRaster);
		}
		RwRasterDestroy(raster);
	}
	return FALSE;
}


//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwCamera* CNtlPostEffectCamera::Create(RwInt32 width, 
							 RwInt32 height, 
							 RwBool zBuffer, 
							 RwReal fNearClipPlane, 
							 RwReal fFarClipPlane)
{
	m_bEnableCamera = TRUE;
	m_nBlurTextureWidth = width;
	m_nBlurTextureHeight = height;

	m_lpDevice = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();

	if (!CreateEffect())
	{
		DBO_WARNING_MESSAGE("CreateEffect FAIL");
		return NULL;
	}

	m_pMainCamera = CNtlCamera::Create(width, height, zBuffer, fNearClipPlane, fFarClipPlane);
	if (m_pMainCamera == NULL)
	{
		DBO_WARNING_MESSAGE("CNtlCamera::Create m_pMainCamera FAIL");
		return NULL;
	}

	if (!CreateBlurCamera(m_pCamera, m_pSourceTexture, width, height, fNearClipPlane, fFarClipPlane))
	{
		DBO_WARNING_MESSAGE("CreateBlurCamera m_pCamera FAIL");
		return NULL;
	}
	if (!CreateBlurCamera(m_pTempCamera, m_pTempTexture, width, height, fNearClipPlane, fFarClipPlane))
	{
		DBO_WARNING_MESSAGE("CreateBlurCamera m_pTempCamera FAIL");
		return NULL;
	}
    if(!CreateBlurCamera(m_pMotionBlurCamera, m_pMotionBlurTexture, EFFECT_BLUR_TEXTURE_WIDTH, EFFECT_BLUR_TEXTURE_HEIGHT, fNearClipPlane, fFarClipPlane))
    {
		DBO_WARNING_MESSAGE("CreateBlurCamera m_pMotionBlurCamera FAIL");
        return NULL;
    }
	if(!CreateBlurCamera(m_pHeatHazeCamera, m_pHeatHazeTexture, width, height, fNearClipPlane, fFarClipPlane))
	{
		DBO_WARNING_MESSAGE("m_pHeatHazeCamera m_pMotionBlurCamera FAIL");
		return NULL;
	}

	CreateReductionTexture(m_nTextureReductionSize);

	BuildVertex(width, height);

	m_pHeatHaze = NTL_NEW CNtlPLHeatHaze(width, height);

	return m_pCamera;
}

void Set2DVertex(RwIm2DVertex* pVertex, RwReal fX, RwReal fY, RwReal fZ, RwReal fRecipZ, RwReal fU, RwReal fV)
{
    RwIm2DVertexSetScreenX(pVertex, fX);
    RwIm2DVertexSetScreenY(pVertex, fY);
    RwIm2DVertexSetScreenZ(pVertex, fZ);
    RwIm2DVertexSetRecipCameraZ(pVertex, fRecipZ);
    RwIm2DVertexSetU(pVertex, fU, NULL);
    RwIm2DVertexSetV(pVertex, fV, NULL);
    RwIm2DVertexSetIntRGBA(pVertex, 255, 255, 255, 255); 
}

// 굴절 이펙트를 구현하기 위해서 View의 Vertex의 수를 늘리는 함수
void SetViewVertex(RwIm2DVertex* pVertex, RwInt32 nWidht, RwInt32 nHeight, RwCamera* pCamera)
{
    int nCount = 0;
    RwReal fXUnit = (RwReal)nWidht / RES_MESH_CNT;
    RwReal fYUnit = (RwReal)nHeight / RES_MESH_CNT;    
    RwReal fNearScreenZ = RwIm2DGetNearScreenZ();
    RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(pCamera);
    RwReal fPos[4], fUV[4];

    for(int nY = 0; nY < RES_MESH_CNT; ++nY)
    {
        for(int nX = 0; nX < RES_MESH_CNT; ++nX)
        {
            fPos[0] = fXUnit * nX;
            fPos[1] = fXUnit * (nX + 1);
            fPos[2] = fYUnit * nY;
            fPos[3] = fYUnit * (nY + 1);            

            fUV[0] = fPos[0] / nWidht;
            fUV[1] = fPos[1] / nWidht;
            fUV[2] = fPos[2] / nHeight;
            fUV[3] = fPos[3] / nHeight;

            Set2DVertex(&pVertex[nCount++], fPos[0], fPos[2], fNearScreenZ, recipCameraZ, fUV[0], fUV[2]);
            Set2DVertex(&pVertex[nCount++], fPos[0], fPos[3], fNearScreenZ, recipCameraZ, fUV[0], fUV[3]);
            Set2DVertex(&pVertex[nCount++], fPos[1], fPos[2], fNearScreenZ, recipCameraZ, fUV[1], fUV[2]);

            Set2DVertex(&pVertex[nCount++], fPos[1], fPos[2], fNearScreenZ, recipCameraZ, fUV[1], fUV[2]);
            Set2DVertex(&pVertex[nCount++], fPos[0], fPos[3], fNearScreenZ, recipCameraZ, fUV[0], fUV[3]);
            Set2DVertex(&pVertex[nCount++], fPos[1], fPos[3], fNearScreenZ, recipCameraZ, fUV[1], fUV[3]);
        }        
    }
}

void CNtlPostEffectCamera::BuildVertex(RwInt32 width, RwInt32 height)
{
	RwIm3DVertexSetPos(&m_3dVertex[0], -1, -1, 0.f);	RwIm3DVertexSetRGBA (&m_3dVertex[0], 255, 255, 255, 0);	RwIm3DVertexSetU(&m_3dVertex[0], 0);	RwIm3DVertexSetV(&m_3dVertex[0], 1);  
	RwIm3DVertexSetPos(&m_3dVertex[1], -1,  1, 0.f);	RwIm3DVertexSetRGBA (&m_3dVertex[1], 255, 255, 255, 0);	RwIm3DVertexSetU(&m_3dVertex[1], 0);  RwIm3DVertexSetV(&m_3dVertex[1], 0);  
	RwIm3DVertexSetPos(&m_3dVertex[2],  1, -1, 0.f);	RwIm3DVertexSetRGBA (&m_3dVertex[2], 255, 255, 255, 0);	RwIm3DVertexSetU(&m_3dVertex[2], 1);  RwIm3DVertexSetV(&m_3dVertex[2], 1);  
	RwIm3DVertexSetPos(&m_3dVertex[3],  1,  1, 0.f);	RwIm3DVertexSetRGBA (&m_3dVertex[3], 255, 255, 255, 0);	RwIm3DVertexSetU(&m_3dVertex[3], 1);  RwIm3DVertexSetV(&m_3dVertex[3], 0);  

	SetRhwVertex(m_RhwVertex, 0, 0, width, height);

	width /= 3;
	height /= 3;

	SetRhwVertex(m_SourceVertex, 0, 0, width, height);
	SetRhwVertex(m_PowerVertex, 0, height+10, width, height);
	SetRhwVertex(m_FinalVertex, 0, height+10+height+10, width, height);
    SetRhwVertex(m_RhwBlurVertex, 0, 0, EFFECT_BLUR_TEXTURE_WIDTH, EFFECT_BLUR_TEXTURE_HEIGHT);
}

void CNtlPostEffectCamera::SetRhwVertex(RwIm2DVertex* pVertex, RwInt32 nStartX, RwInt32 nStartY, RwInt32 width, RwInt32 height)
{
	if (m_pCamera == NULL) return;
	RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(m_pCamera);

    Set2DVertex(&pVertex[0], nStartX - 0.5f, nStartY - 0.5f, RwIm2DGetNearScreenZ(), recipCameraZ, 0.0f, 0.0f);
    Set2DVertex(&pVertex[1], nStartX - 0.5f, nStartY + height - 0.5f, RwIm2DGetNearScreenZ(), recipCameraZ, 0.0f, 1.0f);
    Set2DVertex(&pVertex[2], nStartX + width - 0.5f, nStartY - 0.5f, RwIm2DGetNearScreenZ(), recipCameraZ, 1.0f, 0.0f);
    Set2DVertex(&pVertex[3], nStartX + width - 0.5f, nStartY + height - 0.5f, RwIm2DGetNearScreenZ(), recipCameraZ, 1.0f, 1.0f);
}

//------------------------------------------------------------------
//	FuncName	: CreateEffect
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlPostEffectCamera::CreateEffect()
{
	const D3DCAPS9* d3dCaps = (const D3DCAPS9 *)RwD3D9GetCaps();
    if (((d3dCaps->VertexShaderVersion & 0xffff) >= 0x0101) && ((d3dCaps->PixelShaderVersion & 0xffff) >= 0x0200))
	{
		LPD3DXBUFFER pErr=NULL;
		if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT)
		{
			BYTE*	pPackBuffer	= NULL;
			RwInt32	iPackSize	= 0;

			GetNtlResourcePackManager()->LoadScript(m_strShaderName.c_str(), (void**)&pPackBuffer, &iPackSize);
			if (pPackBuffer)
			{
				BYTE* pTempBuffer = NTL_NEW BYTE [iPackSize + 1];
				memcpy(pTempBuffer, pPackBuffer, iPackSize);
				pTempBuffer[iPackSize] = '\0';

				if (FAILED(D3DXCreateEffect(m_lpDevice, pTempBuffer, iPackSize + 1, NULL, NULL, 0, NULL, &m_lpEffect, &pErr)))
				{
					RwChar* Error = (RwChar*)pErr->GetBufferPointer();
					DBO_WARNING_MESSAGE("Error " << Error);
					pErr->Release();
					NTL_ARRAY_DELETE(pPackBuffer);
					NTL_ARRAY_DELETE(pTempBuffer);
					return FALSE;
                }

				NTL_ARRAY_DELETE(pPackBuffer);
				NTL_ARRAY_DELETE(pTempBuffer);
			}
			else
			{
				DBO_WARNING_MESSAGE("pPackBuffer == NULL");
			}
		}
		else
		{
			if (FAILED(D3DXCreateEffectFromFile(m_lpDevice, m_strShaderName.c_str(), NULL, NULL, 0, NULL, &m_lpEffect, &pErr)))
			{
				RwChar* Error = (RwChar*)pErr->GetBufferPointer();
				DBO_WARNING_MESSAGE("Error " << Error);
				pErr->Release();
				return FALSE;
			}
		}

		if ( NULL == m_lpEffect )
		{
			DBO_WARNING_MESSAGE("m_lpEffect == NULL");
			return FALSE;
		}

		m_hTechnique		= m_lpEffect->GetTechniqueByName("TShader");					NTL_ASSERTE(m_hTechnique);

		m_hMapWidth			= m_lpEffect->GetParameterByName(NULL, "MAP_WIDTH");			NTL_ASSERTE(m_hMapWidth);
		m_hMapHeight		= m_lpEffect->GetParameterByName(NULL, "MAP_HEIGHT");			NTL_ASSERTE(m_hMapHeight);

		m_hMonoPowerHeight	= m_lpEffect->GetParameterByName(NULL, "mono_power_height");	NTL_ASSERTE(m_hMonoPowerHeight);
		m_hRGB2Y			= m_lpEffect->GetParameterByName(NULL, "RGB2Y");				NTL_ASSERTE(m_hRGB2Y);

		m_lpEffect->SetFloat(m_hMapWidth, (RwReal)m_nBlurTextureWidth);
		m_lpEffect->SetFloat(m_hMapHeight, (RwReal)m_nBlurTextureHeight);

		m_lpEffect->SetFloat(m_hMonoPowerHeight, m_fMonoPowerHeight);

		m_lpEffect->SetVector(m_hRGB2Y, &m_vRGB2Y);
	}

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: UpdateBlurFilter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::UpdateBlurFilter()
{
	if(!IsEnablePostEffect())
		return;

	m_lpEffect->SetTechnique(m_hTechnique);
	m_lpEffect->Begin(NULL,0);	

	Update_Power_MonoPower();
	Update_FakeHDRFiltering();

	m_lpEffect->End();
}

//------------------------------------------------------------------
//	FuncName	: Update_Power_MonoPower
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::Update_Power_MonoPower()
{
	// apply current bloom effect mono factor
	m_fMonoPowerHeight = GetSceneManager()->GetActiveBloomFactor();

	if (SUCCEEDED(m_lpEffect->BeginPass(EFFECT_PASS_MONO_POWER)))
	{
		m_lpEffect->SetFloat(m_hMonoPowerHeight, m_fMonoPowerHeight);
		m_lpEffect->SetVector(m_hRGB2Y, &m_vRGB2Y);
		m_lpEffect->CommitChanges();

		if (RwCameraClear(m_pTempCamera, &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			if(RwCameraBeginUpdate(m_pTempCamera))
			{
				BeginPostEffectState(FALSE);

				RwD3D9SetTexture(m_pSourceTexture, 0);
				RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);

				EndPostEffectState(FALSE);

				RwCameraEndUpdate(m_pTempCamera);
			}
		}

		m_lpEffect->EndPass();
	}
}

//------------------------------------------------------------------
//	FuncName	: Update_FakeHDRFiltering
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::Update_FakeHDRFiltering()
{
	// ==========================================
	// 화면 축소하기
	// ==========================================
	if (SUCCEEDED(m_lpEffect->BeginPass(EFFECT_PASS_NONE)))
	{
		m_lpEffect->CommitChanges();
		if (RwCameraClear(m_pReductionCamera, &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			if(RwCameraBeginUpdate(m_pReductionCamera))
			{
				// camera를 update 한후 render state를 먹인다.
				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

				BeginPostEffectState(FALSE);

				RwD3D9SetTexture(m_pTempTexture, 0);
				RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwReductionVertex, 4);

				EndPostEffectState(FALSE);

				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);

				RwCameraEndUpdate(m_pReductionCamera);
			}
		}
		m_lpEffect->EndPass();	
	}

	// ==========================================
	// 축소화면 블러 먹이기
	// ==========================================
	if (SUCCEEDED(m_lpEffect->BeginPass(EFFECT_PASS_16BOX)))
	{
		m_lpEffect->SetFloat(m_hMapWidth, (RwReal)m_nTextureReductionSize);
		m_lpEffect->SetFloat(m_hMapHeight, (RwReal)m_nTextureReductionSize);
		m_lpEffect->CommitChanges();
		if (RwCameraClear(m_pFakeHDRCamera, &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			if(RwCameraBeginUpdate(m_pFakeHDRCamera))
			{
				// camera를 update 한후 render state를 먹인다.
				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

				//programmable vertex Pipeline인 경우 TextureState의 TexCoordIndex가 Stage 명과 같아야 한다.(by HoDong 2007. 4. 12)
				//Test를 1주일 정도 보다가 정리를 한다.

				// CzStateChange : Start
				// CzStateChange : Old
				//RwD3D9SetTextureStageState (0, D3DTSS_TEXCOORDINDEX, 0);
				//RwD3D9SetTextureStageState (1, D3DTSS_TEXCOORDINDEX, 1);

				// CzStateChange : New
				RwD3D9SetTextureStageState (0, D3DTSS_TEXCOORDINDEX, 0);
				RwD3D9SetTextureStageState (1, D3DTSS_TEXCOORDINDEX, 1);
				RwD3D9SetTextureStageState (2, D3DTSS_TEXCOORDINDEX, 2);
				RwD3D9SetTextureStageState (3, D3DTSS_TEXCOORDINDEX, 3);
				// CzStateChange : End

				RwD3D9SetTexture(m_pReductionTexture, 0);
				if(RwIm3DTransform(m_3dVertex, 4, NULL, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXUV))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
					RwIm3DEnd();
				}

				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);

				RwCameraEndUpdate(m_pFakeHDRCamera);
			}
		}
		m_lpEffect->EndPass();
	}
	
	if (SUCCEEDED(m_lpEffect->BeginPass(EFFECT_PASS_16BOX)))
	{
		m_lpEffect->SetFloat(m_hMapWidth, (RwReal)m_nTextureReductionSize);
		m_lpEffect->SetFloat(m_hMapHeight, (RwReal)m_nTextureReductionSize);
		m_lpEffect->CommitChanges();
		if (RwCameraClear(m_pReductionCamera, &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			if(RwCameraBeginUpdate(m_pReductionCamera))
			{
				// camera를 update 한후 render state를 먹인다.
				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

				// programmable vertex Pipeline인 경우 TextureState의 TexCoordIndex가 Stage 명과 같아야 한다.(by HoDong 2007. 4. 12)
				// Test를 1주일 정도 보다가 정리를 한다.

				// CzStateChange : Start
				// CzStateChange : Old
				//RwD3D9SetTextureStageState (0, D3DTSS_TEXCOORDINDEX, 0);
				//RwD3D9SetTextureStageState (1, D3DTSS_TEXCOORDINDEX, 1);

				// CzStateChange : New
				RwD3D9SetTextureStageState (0, D3DTSS_TEXCOORDINDEX, 0);
				RwD3D9SetTextureStageState (1, D3DTSS_TEXCOORDINDEX, 1);
				RwD3D9SetTextureStageState (2, D3DTSS_TEXCOORDINDEX, 2);
				RwD3D9SetTextureStageState (3, D3DTSS_TEXCOORDINDEX, 3);
				// CzStateChange : End

				RwD3D9SetTexture(m_pFakeHDRTexture, 0);
				if(RwIm3DTransform(m_3dVertex, 4, NULL, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXUV))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
					RwIm3DEnd();
				}

				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);

				RwCameraEndUpdate(m_pReductionCamera);
			}
		}
		m_lpEffect->EndPass();
	}
	
	// ==========================================
	// 화면 확대하기
	// ==========================================
	if (SUCCEEDED(m_lpEffect->BeginPass(EFFECT_PASS_NONE)))
	{
		m_lpEffect->CommitChanges();
		if (RwCameraClear(m_pTempCamera, &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			if(RwCameraBeginUpdate(m_pTempCamera))
			{
				// camera를 update 한후 render state를 먹인다.
				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

				BeginPostEffectState(FALSE);

				RwD3D9SetTexture(m_pReductionTexture, 0);
				RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);

				EndPostEffectState(FALSE);

				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);

				RwCameraEndUpdate(m_pTempCamera);
			}
		}
		m_lpEffect->EndPass();
	}
}	


//-------------------------------------------------------------
// Name: CreateReductionTexture()
// Desc: 가중치 계산
//-------------------------------------------------------------
void CNtlPostEffectCamera::CreateReductionTexture(RwInt32 nTextureSize)
{
	if (m_pReductionCamera != NULL)
	{
		DestroyBlurCamera(m_pReductionCamera, m_pReductionTexture);
	}
	if (m_pFakeHDRCamera != NULL)
	{
		DestroyBlurCamera(m_pFakeHDRCamera, m_pFakeHDRTexture);
	}

	SetRhwVertex(m_RhwReductionVertex, 0, 0, nTextureSize, nTextureSize);

	if (!CreateBlurCamera(m_pReductionCamera, m_pReductionTexture, nTextureSize, nTextureSize, 0.1f, 1000.f))
	{
		return;
	}
	if (!CreateBlurCamera(m_pFakeHDRCamera, m_pFakeHDRTexture, nTextureSize, nTextureSize, 0.1f, 1000.f))
	{
		return;
	}
}

//------------------------------------------------------------------
//	FuncName	: DrawTexture
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::DrawTexture()
{
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);

	// Source Texture
	RwD3D9SetTexture(m_pSourceTexture, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_SourceVertex, 4);

	// Source Texture
	RwD3D9SetTexture(m_pReductionTexture, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_PowerVertex, 4);

	// Source Texture
	RwD3D9SetTexture(m_pTempTexture, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_FinalVertex, 4);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
}

void CNtlPostEffectCamera::SetCameraSize(RwRect* rect)
{
	if (rect != NULL)
	{
		m_nBlurTextureWidth = rect->w;
		m_nBlurTextureHeight= rect->h;
	}

	if (m_pTempCamera != NULL)
		CNtlCamera::SetCameraResize(m_pTempCamera, rect);
	if (m_pMainCamera != NULL)
		CNtlCamera::SetCameraResize(m_pMainCamera, rect);
	if (m_pCamera != NULL)
		CNtlCamera::SetCameraResize(m_pCamera, rect);
	if (m_pHeatHazeCamera != NULL)
		CNtlCamera::SetCameraResize(m_pHeatHazeCamera, rect);


	BuildVertex(m_nBlurTextureWidth, m_nBlurTextureHeight);

	if(m_pHeatHaze)
	{
		m_pHeatHaze->SetScrSize(rect);
	}

	RestoreHandler();
}

int CNtlPostEffectCamera::ReleaseHandler()
{
	if (m_lpEffect != NULL)
	{
		m_lpEffect->Release();
		m_lpEffect = NULL;

		m_hTechnique = NULL;
	}

	if(m_pHeatHaze)
	{
		m_pHeatHaze->ReleaseHandler();
	}

	return TRUE;
}

int CNtlPostEffectCamera::RestoreHandler()
{
	if (!m_bEnableCamera) return TRUE;

	if (m_lpEffect != NULL)
	{
		m_lpEffect->Release();
		m_lpEffect = NULL;

		m_hTechnique = NULL;
	}

	m_lpDevice = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();

	if (!CreateEffect())
	{
		return NULL;
	}

	if(m_pHeatHaze)
	{
		m_pHeatHaze->RestoreHandler(m_pCamera);
	}

	return TRUE;
}

RwBool CNtlPostEffectCamera::IsEnablePostEffect(void)
{
	if(m_lpEffect == NULL)
		return FALSE;

	if(GetPostEffectFilters() == POST_EFFECT_FILTER_NONE)
		return FALSE;

	if(!GetSceneManager()->GetWorld())
		return FALSE;

	return TRUE;
}

RwCamera* CNtlPostEffectCamera::CameraClear(RwRGBA *rgba, RwInt32 uiClearMode)
{
	// image filteing camera
	if(m_pCamera)
		return RwCameraClear(m_pCamera, rgba, uiClearMode);
	return NULL;
}

void CNtlPostEffectCamera::CameraShowRaster(HWND hWnd, RwUInt32 uiFlags)
{
	// main
	RwCameraShowRaster(m_pMainCamera, hWnd, uiFlags);
}


RwCamera* CNtlPostEffectCamera::PostCameraBeginUpdate(RwReal fElapsed)
{
	if(m_pCamera)
		return RwCameraBeginUpdate(m_pCamera);
	else
		return RwCameraBeginUpdate(m_pMainCamera);
}

void CNtlPostEffectCamera::PostCameraEndUpdate(void)
{
	if(m_pCamera)
		RwCameraEndUpdate(m_pCamera);
	else
		RwCameraEndUpdate(m_pMainCamera);
}

RwCamera* CNtlPostEffectCamera::MainCameraBeginUpdate(RwReal fElapsed)
{
	if(RwCameraBeginUpdate(m_pMainCamera))
	{
		if (IsEnablePostEffect())
		{
			
			//m_lpEffect->SetTechnique(m_hTechnique);
			//m_lpEffect->Begin(NULL,0);	
			//m_lpEffect->BeginPass(EFFECT_PASS_LASTDRAW);
			//m_lpEffect->CommitChanges();
			
			// Test를 1주일 정도 보다가 정리를 한다.(by HoDong 2007.4.11)
			RwD3D9SetTextureStageState (0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1);
			RwD3D9SetTextureStageState (0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
			
			RwD3D9SetTextureStageState (0, D3DTSS_ALPHAOP,		D3DTOP_SELECTARG1);
			RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
			
			RwD3D9SetTextureStageState (1, D3DTSS_COLOROP,		D3DTOP_ADD);
			RwD3D9SetTextureStageState (1, D3DTSS_COLORARG1,	D3DTA_CURRENT); 
			RwD3D9SetTextureStageState (1, D3DTSS_COLORARG2,	D3DTA_TEXTURE);

			RwD3D9SetTextureStageState (1, D3DTSS_ALPHAOP,		D3DTOP_ADD);
			RwD3D9SetTextureStageState (1, D3DTSS_ALPHAARG1,	D3DTA_CURRENT); 
			RwD3D9SetTextureStageState (1, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE);

			RwD3D9SetTextureStageState (1, D3DTSS_TEXCOORDINDEX, 0);

            RwD3D9SetTexture(m_pSourceTexture, 0);
			RwD3D9SetTexture(m_pTempTexture, 1);

			//BeginPostEffectState(TRUE);
	
			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);
			
			EndPostEffectState(TRUE);

			//RwD3D9SetTexture(NULL, 0);
			//RwD3D9SetTexture(NULL, 1);
			
			//m_lpEffect->EndPass();
			//m_lpEffect->End();
		}
		else
		{
			BeginPostEffectState(FALSE);

            RwD3D9SetTexture(m_pSourceTexture, 0);

			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);            

			EndPostEffectState(FALSE);
		}

		// LensFlare Render (by agebreak 2007.4.23)
		for each(CNtlPLLensFlare* pLensFlare in m_listLensFlare)
		{
			if(pLensFlare && pLensFlare->IsVisible())
			{
				pLensFlare->Render();
			}
		}		

		// Post Effect Render		
        for each(CNtlInstancePostEffectSystem* pPostEffectSystem in m_listPostEffectSystem)
        {
            if(pPostEffectSystem->GetComicState() != CNtlInstancePostEffectSystem::COMIC_STATE_IDLE)
            {
                pPostEffectSystem->RenderPostEffect();
            }
        }

		return m_pMainCamera;
	}

	return NULL;
}

void CNtlPostEffectCamera::MainCameraEndUpdate(void)
{
	RwCameraEndUpdate(m_pMainCamera);
}

void CNtlPostEffectCamera::UpdatePostEffectFilter(void)
{
	NTL_SPROFILE("CNtlPostEffectCamera::UpdatePostEffectFilter")

	UpdateBlurFilter();

	NTL_EPROFILE()
}

void CNtlPostEffectCamera::SetPostEffectFilters( RwUInt8 byPEFilters )
{
	m_byPEFilters = byPEFilters; 
}

RwUInt8	CNtlPostEffectCamera::GetPostEffectFilters(void)	
{ 
	return m_byPEFilters;
}

void CNtlPostEffectCamera::UpdateHeatHaze(RwReal fElapsedTime)
{
	if(!m_pHeatHaze || !m_pHeatHaze->GetEnabled())
	{
		return;
	}

	if (m_pHeatHaze->Update(fElapsedTime, m_pHeatHazeCamera))
	{
		m_pHeatHaze->Render(m_pSourceTexture, m_pHeatHazeCamera);
	
		// take the frame
		RwIm2DVertex rhwVertex[4];
		memcpy(rhwVertex, m_RhwVertex, sizeof(RwIm2DVertex) * 4);
		for(int i = 0; i < 4; ++i)
		{
			RwIm2DVertexSetRealRGBA(rhwVertex, 255, 255, 255, m_fBlurAlpha);
		}

		if(RwCameraBeginUpdate(m_pCamera))
		{
			RwD3D9SetTexture(m_pHeatHazeTexture, 0);
			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, rhwVertex, 4);

			EndBlurEffect();

			RwCameraEndUpdate(m_pCamera);       
		}
	}
}									

void CNtlPostEffectCamera::UpdateMotionBlur(RwReal fElapsedTime)
{
	NTL_SPROFILE("CNtlPostEffectCamera::UpdateMotionBlur")

    if(!m_bEnableBlur || !m_pMotionBlurCamera || !m_pMotionBlurTexture)
	{
		NTL_RPROFILE_VOID()
	}

    // 알파값을 업데이트 한다.
    if(m_fBlurAlpha > EFFECT_BLUR_ALPHA)
    {
        m_fBlurAlpha -= (m_fBlurFadeVelocity * fElapsedTime);
        if(m_fBlurAlpha < EFFECT_BLUR_ALPHA)
        {
            m_fBlurAlpha = EFFECT_BLUR_ALPHA;
        }
    }

    // 현재 화면을 블러 누적 텍스쳐에 알파값을 주고 찍는다.
    for(int i = 0; i < 4; ++i)
    {
        RwIm2DVertexSetRealRGBA(&m_RhwBlurVertex[i], 255, 255, 255, m_fBlurAlpha);
    }

    if(RwCameraBeginUpdate(m_pMotionBlurCamera))
    {
        BeginBlurEffect();

        RwD3D9SetTexture(m_pSourceTexture, 0);
        RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwBlurVertex, 4);

        RwCameraEndUpdate(m_pMotionBlurCamera);
    }

    // 누적 블러 텍스쳐를 현재화면에 찍는다.
    RwIm2DVertex rhwVertex[4];
    memcpy(rhwVertex, m_RhwVertex, sizeof(RwIm2DVertex) * 4);
    for(int i = 0; i < 4; ++i)
    {
        RwIm2DVertexSetRealRGBA(rhwVertex, 255, 255, 255, m_fBlurAlpha);
    }

    if(RwCameraBeginUpdate(m_pCamera))
    {
        RwD3D9SetTexture(m_pMotionBlurTexture, 0);
        RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, rhwVertex, 4);

        EndBlurEffect();

        RwCameraEndUpdate(m_pCamera);       
    }

	NTL_EPROFILE()
}

void CNtlPostEffectCamera::SetEnableBlur(RwBool bEnable, RwReal fFadeTime /* = 0.0f */)
{
    if(fFadeTime == 0.0f)
    {
        m_fBlurAlpha = EFFECT_BLUR_ALPHA;
        m_fBlurFadeVelocity = 0.0f;
    }
    else
    {
        m_fBlurAlpha = 255.0f;
        m_fBlurFadeVelocity = (255.0f - EFFECT_BLUR_ALPHA) / fFadeTime;
    }

    if(!m_bEnableBlur && bEnable)
    {
		if (RwCameraClear(m_pMotionBlurCamera, &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
		{
			// 현재 화면을 블러 누적 텍스쳐에 알파값을 주고 찍는다.
			for(int i = 0; i < 4; ++i)
			{
				RwIm2DVertexSetRealRGBA(&m_RhwBlurVertex[i], 255, 255, 255, 255);
			}

			if(RwCameraBeginUpdate(m_pMotionBlurCamera))
			{
				BeginBlurEffect();

				RwD3D9SetTexture(m_pSourceTexture, 0);
				RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwBlurVertex, 4);

				EndBlurEffect();

				RwCameraEndUpdate(m_pMotionBlurCamera);
			}
		}
    }

    m_bEnableBlur = bEnable;
}

RwV2d CNtlPostEffectCamera::GetScreenSize() 
{
    RwV2d v2dSize;
    v2dSize.x = (RwReal)m_nBlurTextureWidth;
    v2dSize.y = (RwReal)m_nBlurTextureHeight;

    return v2dSize;
}

void CNtlPostEffectCamera::AttachPostEffectSystem( CNtlInstancePostEffectSystem* pPostEffectSystem ) 
{
	if(!pPostEffectSystem)
		return;

    m_listPostEffectSystem.push_back(pPostEffectSystem);
}

void CNtlPostEffectCamera::DetachPostEffectSystem( CNtlInstancePostEffectSystem* pPostEffectSystem ) 
{
	if(!pPostEffectSystem)
		return;

	m_listPostEffectSystem.remove(pPostEffectSystem);
}

void CNtlPostEffectCamera::AttachLensFlare( CNtlPLLensFlare* pLensFlare ) 
{
	if(!pLensFlare)
		return;

	m_listLensFlare.push_back(pLensFlare);
}

void CNtlPostEffectCamera::DetachLensFlare( CNtlPLLensFlare* pLensFlare ) 
{
	if(!pLensFlare)
		return;

	m_listLensFlare.remove(pLensFlare);
}

void CNtlPostEffectCamera::SetCalcFov( RwReal fFov ) 
{
    CNtlCamera::SetCalcFov(m_pCamera, fFov);
    CNtlCamera::SetCalcFov(m_pMainCamera, fFov);
    CNtlCamera::SetCalcFov(m_pTempCamera, fFov);
    CNtlCamera::SetCalcFov(m_pMotionBlurCamera, fFov);
}