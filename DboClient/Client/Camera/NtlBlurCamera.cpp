//***********************************************************************************
//	File		:	NtlBlurCamera.cpp
//	Desc		:	
//	Begin		:	2006. 2.13
//	Copyright	:	ⓒ 2006 by Mahwang CO., Ltd
//	Author		:	Mahwang
//	Update		:	
//***********************************************************************************

#include "NtlBlurCamera.h"
#include "NtlText.h"


CNtlBlurCamera::CNtlBlurCamera(void)
{
	m_pSourceTexture		= NULL;


	m_nBlurTextureWidth		= 128;;
	m_nBlurTextureHeight	= 128;

	m_pBlurCamera			= NULL;

	m_pTempCamera			= NULL;
	m_pTempTexture			= NULL;

	m_pGaussianCamera		= NULL;
	m_pGaussianTexture		= NULL;

	m_pPowerCamera			= NULL;
	m_pPowerTexture			= NULL;

	m_pReductionCamera		= NULL;
	m_pReductionTexture		= NULL;

	m_pFakeHDRCamera		= NULL;
	m_pFakeHDRTexture		= NULL;

	m_lpDevice				= NULL;

	m_lpEffect				= NULL;
	m_hTechnique			= NULL;

	m_hMapWidth				= NULL;
	m_hMapHeight			= NULL;

	m_hGaussainWeight		= NULL;

	m_hPowerHeight			= NULL;
	m_hBloomHeight			= NULL;

	m_bDrawTexture			= TRUE;
	m_nRenderType			= LAST_RENDER_NONE;
	m_nPowerType			= POWER_FILLTER_NONE;
	m_nBlurType				= BLUR_FILLTER_NONE;

	m_fPowerHeight			= 32.f;

	m_fGaussainDispersion_sq= 1.0f;
	UpdateGaussainWeightTable(m_fGaussainDispersion_sq);

	m_nFakeHDRBlurCount		= 1;
	m_fBloomHeightLight		= 0.5f;
	m_nTextureReductionSize		= 128;

	m_handleKeyDown    = CInputHandler::GetInstance()->LinkKeyDown(this, &CNtlBlurCamera::KeyboardDownHandler);
}


CNtlBlurCamera::~CNtlBlurCamera(void)
{
	CInputHandler::GetInstance()->UnLinkKeyDown(m_handleKeyDown);
}


//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Destroy()
{
	if (m_lpEffect != NULL)
	{
		m_lpEffect->Release();
		m_lpEffect = NULL;

		m_hTechnique			= NULL;
	}

	// Delete Camera
	DestroyBlurCamera(m_pGaussianCamera, m_pGaussianTexture);
	DestroyBlurCamera(m_pPowerCamera, m_pPowerTexture);

	DestroyBlurCamera(m_pReductionCamera, m_pReductionTexture);

	DestroyBlurCamera(m_pFakeHDRCamera, m_pFakeHDRTexture);

	DestroyBlurCamera(m_pTempCamera, m_pTempTexture);

	DestroyBlurCamera(m_pRwCamera, m_pSourceTexture);

	m_pRwCamera = m_pBlurCamera;
	m_pBlurCamera = NULL;


	CNtlCamera::Destroy();
}

void CNtlBlurCamera::DestroyBlurCamera(RwCamera*& pCamera, RwTexture*& pTexture)
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

int CNtlBlurCamera::KeyboardDownHandler(unsigned int uiKeyData)
{
	SKeyData *pData = reinterpret_cast<SKeyData*>(uiKeyData);
	
	if(pData->uiRepCount == 1)
	{
		if(pData->uiChar == VK_F5)
		{
			m_nRenderType = LAST_RENDER_NONE;
		}
		if(pData->uiChar == VK_F6)
		{
			m_nRenderType = LAST_RENDER_POWER;
		}
		if(pData->uiChar == VK_F7)
		{
			m_nRenderType = LAST_RENDER_FILLTER;
		}
		if(pData->uiChar == VK_F8)
		{
			m_bDrawTexture = !m_bDrawTexture;
		}

		if(pData->uiChar == VK_F9)
		{
			m_nTextureReductionSize = m_nTextureReductionSize << 1;
			if (m_nTextureReductionSize > 512)
			{
				m_nTextureReductionSize = 64;
			}
			CreateReductionTexture(m_nTextureReductionSize);
		}

		// keyboard input check
		if(pData->uiChar == VK_INSERT)
		{
			++ m_nPowerType;
			if (m_nPowerType >= POWER_FILLTER_MAX_COUNT)
			{
				m_nPowerType = POWER_FILLTER_NONE;
			}
		}
		// keyboard input check
		if(pData->uiChar == VK_HOME)
		{
			-- m_nPowerType;
			if (m_nPowerType < 0)
			{
				m_nPowerType = POWER_FILLTER_MONO_POWER;
			}
		}
		// keyboard input check
		if(pData->uiChar == VK_PRIOR)
		{
		}


		// keyboard input check
		if(pData->uiChar == VK_DELETE)
		{
			++ m_nBlurType;
			if (m_nBlurType >= BLUR_FILLTER_MAX_COUNT)
			{
				m_nBlurType = BLUR_FILLTER_NONE;
			}
		}
		// keyboard input check
		if(pData->uiChar == VK_END)
		{
			-- m_nBlurType;
			if (m_nBlurType < 0)
			{
				m_nBlurType = BLUR_FILLTER_REDUCTION;
			}
		}
		// keyboard input check
		if(pData->uiChar == VK_NEXT)
		{
		}

		// keyboard input check
		if(pData->uiChar == VK_LEFT)
		{
			-- m_nFakeHDRBlurCount;
			if (m_nFakeHDRBlurCount < 0)
			{
				m_nFakeHDRBlurCount = 0;
			}
		}
		// keyboard input check
		if(pData->uiChar == VK_RIGHT)
		{
			++ m_nFakeHDRBlurCount;
			if (m_nFakeHDRBlurCount > 10)
			{
				m_nFakeHDRBlurCount = 10;
			}
		}

		if(pData->uiChar == 'o' || pData->uiChar == 'O')
		{
			m_lpEffect->SetFloat(m_hPowerHeight, m_fPowerHeight);
			m_fPowerHeight -= 5.f;
		}
		if(pData->uiChar == 'p' || pData->uiChar == 'P')
		{
			m_lpEffect->SetFloat(m_hPowerHeight, m_fPowerHeight);
			m_fPowerHeight += 5.f;
		}


		if(pData->uiChar == 'k' || pData->uiChar == 'K')
		{
			m_fGaussainDispersion_sq -= 0.1f;
			UpdateGaussainWeightTable(m_fGaussainDispersion_sq);
			m_lpEffect->SetFloatArray(m_hGaussainWeight, m_fGaussainWeightTable, 7);
		}
		if(pData->uiChar == 'l' || pData->uiChar == 'L')
		{
			m_fGaussainDispersion_sq += .1f;
			UpdateGaussainWeightTable(m_fGaussainDispersion_sq);
			m_lpEffect->SetFloatArray(m_hGaussainWeight, m_fGaussainWeightTable, 7);
		}
		if(pData->uiChar == 'n' || pData->uiChar == 'N')
		{
			m_fBloomHeightLight -= 0.1;
			m_lpEffect->SetFloat(m_hBloomHeight, m_fBloomHeightLight);
		}
		if(pData->uiChar == 'm' || pData->uiChar == 'M')
		{
			m_fBloomHeightLight += 0.1;
			m_lpEffect->SetFloat(m_hBloomHeight, m_fBloomHeightLight);
		}
	}

	return 1;
}

//------------------------------------------------------------------
//	FuncName	: CreateBlurCamera
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlBlurCamera::CreateBlurCamera(RwCamera*& pCamera, RwTexture*& pTexture, RwInt32 nWidth, RwInt32 nHeight, RwReal fNearClipPlane, RwReal fFarClipPlane)
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
					RwV2d vw;

					RwCameraSetRaster(pCamera, raster);
					RwCameraSetZRaster(pCamera, zRaster);

					RwCameraSetFrame(pCamera, frame);

					RwCameraSetNearClipPlane(pCamera, fNearClipPlane);
					RwCameraSetFarClipPlane(pCamera, fFarClipPlane);

					vw.x = vw.y = 0.4f;
					RwCameraSetViewWindow(pCamera, &vw);

					pTexture = RwTextureCreate(raster);
					RwTextureSetFilterMode(pTexture, rwFILTERLINEAR);

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
RwCamera* CNtlBlurCamera::Create(RwInt32 width, 
							 RwInt32 height, 
							 RwBool zBuffer, 
							 RwReal fNearClipPlane, 
							 RwReal fFarClipPlane)
{
	m_nBlurTextureWidth = width;
	m_nBlurTextureHeight = height;
	m_pBlurCamera = CNtlCamera::Create(width, height, zBuffer, fNearClipPlane, fFarClipPlane);

	if (!CreateBlurCamera(m_pRwCamera, m_pSourceTexture, width, height, fNearClipPlane, fFarClipPlane))
	{
		return NULL;
	}

	if (!CreateBlurCamera(m_pGaussianCamera, m_pGaussianTexture, width, height, fNearClipPlane, fFarClipPlane))
	{
		return NULL;
	}
	if (!CreateBlurCamera(m_pPowerCamera, m_pPowerTexture, width, height, fNearClipPlane, fFarClipPlane))
	{
		return NULL;
	}
	if (!CreateBlurCamera(m_pReductionCamera, m_pReductionTexture, m_nTextureReductionSize, m_nTextureReductionSize, fNearClipPlane, fFarClipPlane))
	{
		return NULL;
	}
	if (!CreateBlurCamera(m_pFakeHDRCamera, m_pFakeHDRTexture, m_nTextureReductionSize, m_nTextureReductionSize, fNearClipPlane, fFarClipPlane))
	{
		return NULL;
	}

	if (!CreateBlurCamera(m_pTempCamera, m_pTempTexture, width, height, fNearClipPlane, fFarClipPlane))
	{
		return NULL;
	}

	m_Vertex[0].x = -1;		m_Vertex[0].y =	-1;		m_Vertex[0].z = 0.1f;	m_Vertex[0].u = 0.f;	m_Vertex[0].v = 1.f;
	m_Vertex[1].x = -1;		m_Vertex[1].y =	1;		m_Vertex[1].z = 0.1f;	m_Vertex[1].u = 0.f;	m_Vertex[1].v = 0.f;
	m_Vertex[2].x = 1;		m_Vertex[2].y =	-1;		m_Vertex[2].z = 0.1f;	m_Vertex[2].u = 1.f;	m_Vertex[2].v = 1.f;
	m_Vertex[3].x = 1;		m_Vertex[3].y =	1;		m_Vertex[3].z = 0.1f;	m_Vertex[3].u = 1.f;	m_Vertex[3].v = 0.f;

	m_RhwVertex[0].x = 0-0.5;			m_RhwVertex[0].y =	height-0.5; m_RhwVertex[0].z = 0;	m_RhwVertex[0].w = 1;	m_RhwVertex[0].u = 0.f;	m_RhwVertex[0].v = 1.f;
	m_RhwVertex[1].x = 0-0.5;			m_RhwVertex[1].y =	0-0.5;		m_RhwVertex[1].z = 0;	m_RhwVertex[1].w = 1;	m_RhwVertex[1].u = 0.f;	m_RhwVertex[1].v = 0.f;
	m_RhwVertex[2].x = width-0.5;		m_RhwVertex[2].y =	height-0.5; m_RhwVertex[2].z = 0;	m_RhwVertex[2].w = 1;	m_RhwVertex[2].u = 1.f;	m_RhwVertex[2].v = 1.f;
	m_RhwVertex[3].x = width-0.5;		m_RhwVertex[3].y =	0-0.5;		m_RhwVertex[3].z = 0;	m_RhwVertex[3].w = 1;	m_RhwVertex[3].u = 1.f;	m_RhwVertex[3].v = 0.f;

	CreateReductionTexture(m_nTextureReductionSize);

	width /= 3.f;
	height /= 3.f;

	m_SourceVertex[0].x = 0-0.5;			m_SourceVertex[0].y =	height-0.5; m_SourceVertex[0].z = 0;	m_SourceVertex[0].w = 1;	m_SourceVertex[0].u = 0.f;	m_SourceVertex[0].v = 1.f;
	m_SourceVertex[1].x = 0-0.5;			m_SourceVertex[1].y =	0-0.5;		m_SourceVertex[1].z = 0;	m_SourceVertex[1].w = 1;	m_SourceVertex[1].u = 0.f;	m_SourceVertex[1].v = 0.f;
	m_SourceVertex[2].x = width-0.5;		m_SourceVertex[2].y =	height-0.5; m_SourceVertex[2].z = 0;	m_SourceVertex[2].w = 1;	m_SourceVertex[2].u = 1.f;	m_SourceVertex[2].v = 1.f;
	m_SourceVertex[3].x = width-0.5;		m_SourceVertex[3].y =	0-0.5;		m_SourceVertex[3].z = 0;	m_SourceVertex[3].w = 1;	m_SourceVertex[3].u = 1.f;	m_SourceVertex[3].v = 0.f;

	m_PowerVertex[0].x = 0-0.5;			m_PowerVertex[0].y =	height+10+height-0.5;	m_PowerVertex[0].z = 0;	m_PowerVertex[0].w = 1;	m_PowerVertex[0].u = 0.f;	m_PowerVertex[0].v = 1.f;
	m_PowerVertex[1].x = 0-0.5;			m_PowerVertex[1].y =	height+10+0-0.5;		m_PowerVertex[1].z = 0;	m_PowerVertex[1].w = 1;	m_PowerVertex[1].u = 0.f;	m_PowerVertex[1].v = 0.f;
	m_PowerVertex[2].x = width-0.5;		m_PowerVertex[2].y =	height+10+height-0.5;	m_PowerVertex[2].z = 0;	m_PowerVertex[2].w = 1;	m_PowerVertex[2].u = 1.f;	m_PowerVertex[2].v = 1.f;
	m_PowerVertex[3].x = width-0.5;		m_PowerVertex[3].y =	height+10+0-0.5;		m_PowerVertex[3].z = 0;	m_PowerVertex[3].w = 1;	m_PowerVertex[3].u = 1.f;	m_PowerVertex[3].v = 0.f;

	m_FinalVertex[0].x = 0-0.5;			m_FinalVertex[0].y =	height+10+height+10+height-0.5;	m_FinalVertex[0].z = 0;	m_FinalVertex[0].w = 1;	m_FinalVertex[0].u = 0.f;	m_FinalVertex[0].v = 1.f;
	m_FinalVertex[1].x = 0-0.5;			m_FinalVertex[1].y =	height+10+height+10+0-0.5;		m_FinalVertex[1].z = 0;	m_FinalVertex[1].w = 1;	m_FinalVertex[1].u = 0.f;	m_FinalVertex[1].v = 0.f;
	m_FinalVertex[2].x = width-0.5;		m_FinalVertex[2].y =	height+10+height+10+height-0.5;	m_FinalVertex[2].z = 0;	m_FinalVertex[2].w = 1;	m_FinalVertex[2].u = 1.f;	m_FinalVertex[2].v = 1.f;
	m_FinalVertex[3].x = width-0.5;		m_FinalVertex[3].y =	height+10+height+10+0-0.5;		m_FinalVertex[3].z = 0;	m_FinalVertex[3].w = 1;	m_FinalVertex[3].u = 1.f;	m_FinalVertex[3].v = 0.f;

	m_lpDevice = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();

	CreateEffect();


	return m_pRwCamera;
}

//------------------------------------------------------------------
//	FuncName	: CreateEffect
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlBlurCamera::CreateEffect()
{
    RwImageSetPath("shader/");

	m_pTestTexture = RwTextureRead("3dwa12.png", NULL);

	LPD3DXBUFFER pErr=NULL;
	if (FAILED(D3DXCreateEffectFromFile(m_lpDevice,
									".\\Shader\\Blur.fx",
									NULL,
									NULL,
									0,
									NULL,
									&m_lpEffect,
									&pErr)))
	{
		RwChar* Error = (RwChar*)pErr->GetBufferPointer();
		pErr->Release();
		return FALSE;
	}

	m_hTechnique		= m_lpEffect->GetTechniqueByName("TShader");			NTL_ASSERTE(m_hTechnique);

	m_hMapWidth			= m_lpEffect->GetParameterByName(NULL, "MAP_WIDTH");	NTL_ASSERTE(m_hMapWidth);
	m_hMapHeight		= m_lpEffect->GetParameterByName(NULL, "MAP_HEIGHT");	NTL_ASSERTE(m_hMapHeight);

	m_hGaussainWeight	= m_lpEffect->GetParameterByName(NULL, "weight");		NTL_ASSERTE(m_hGaussainWeight);
	m_hPowerHeight		= m_lpEffect->GetParameterByName(NULL, "power_height");	NTL_ASSERTE(m_hPowerHeight);
	m_hBloomHeight		= m_lpEffect->GetParameterByName(NULL, "bloom_height");	NTL_ASSERTE(m_hBloomHeight);

	m_lpEffect->SetFloat(m_hMapWidth, m_nBlurTextureWidth);
	m_lpEffect->SetFloat(m_hMapHeight, m_nBlurTextureHeight);

	m_lpEffect->SetFloatArray(m_hGaussainWeight, m_fGaussainWeightTable, 7);

	m_lpEffect->SetFloat(m_hPowerHeight, m_fPowerHeight);
	m_lpEffect->SetFloat(m_hBloomHeight, m_fBloomHeightLight);

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Update(RwRGBA& color, HWND hWnd)
{
	RwUInt32 nFvf;
	m_lpDevice->GetFVF((DWORD*)&nFvf);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void *)rwCULLMODECULLNONE);

	m_lpEffect->SetTechnique(m_hTechnique);
	m_lpEffect->Begin(NULL,0);	

	switch(m_nPowerType)
	{
	case POWER_FILLTER_NONE:		Update_Power_None(color, hWnd);			break;
	case POWER_FILLTER_BLOOM:		Update_Power_Bloom(color, hWnd);		break;
	case POWER_FILLTER_POWER:		Update_Power_Power(color, hWnd);		break;
	case POWER_FILLTER_MONO:		Update_Power_Mono(color, hWnd);			break;
	case POWER_FILLTER_MONO_POWER:	Update_Power_MonoPower(color, hWnd);	break;
	}

	if (m_nPowerType != POWER_FILLTER_NONE)
	{
		Update_Power(color, hWnd);

		switch(m_nBlurType)
		{
		case BLUR_FILLTER_GAUSSIAN:		Update_GaussainFiltering(color, hWnd);	break;
		case BLUR_FILLTER_REDUCTION:	Update_ReductionFiltering(color, hWnd);	break;
		case BLUR_FILLTER_FAKE_HDR:		Update_FakeHDRFiltering(color, hWnd);	break;
		case BLUR_FILLTER_16BOX:		Update_16BoxFiltering(color, hWnd);		break;
		case BLUR_FILLTER_64BOX:		Update_64BoxFiltering(color, hWnd);		break;
		}

		Update_Last(color, hWnd);
	}

	m_lpEffect->End();

	RwRenderStateSet(rwRENDERSTATECULLMODE,				(void *)rwCULLMODECULLBACK);
	m_lpDevice->SetFVF(nFvf);

	RwCameraShowRaster(GetCamera(), hWnd, 0);
}

//------------------------------------------------------------------
//	FuncName	: Update_Power_None
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Update_Power_None(RwRGBA& color, HWND hWnd)
{
	m_lpEffect->BeginPass(EFFECT_PASS_NONE);

	RwCameraClear(m_pBlurCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pBlurCamera))
	{
		RwD3D9SetTexture(m_pSourceTexture, 0);

		m_lpDevice->SetFVF(D3DFVF_RHW_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_RhwVertex, sizeof(D3D_RHW_VERTEX));

		RwCameraEndUpdate(m_pBlurCamera);
	}

	m_lpEffect->EndPass();
}

//------------------------------------------------------------------
//	FuncName	: Update_Power_Refract
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Update_Power_Power(RwRGBA& color, HWND hWnd)
{
	m_lpEffect->BeginPass(EFFECT_PASS_POWER);
	m_lpEffect->SetFloat(m_hPowerHeight, m_fPowerHeight);

	RwCameraClear(m_pTempCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pTempCamera))
	{
		RwD3D9SetTexture(m_pSourceTexture, 0);

		m_lpDevice->SetTextureStageState(0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1);
		m_lpDevice->SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);

		m_lpDevice->SetFVF(D3DFVF_RHW_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_RhwVertex, sizeof(D3D_RHW_VERTEX));

		m_lpDevice->SetTextureStageState(0, D3DTSS_COLOROP,		D3DTOP_MODULATE);
		m_lpDevice->SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);

		RwCameraEndUpdate(m_pTempCamera);
	}
	m_lpEffect->EndPass();
}


//------------------------------------------------------------------
//	FuncName	: Update_Power_Bloom
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Update_Power_Bloom(RwRGBA& color, HWND hWnd)
{
	m_lpEffect->BeginPass(EFFECT_PASS_BLOOM);
	m_lpEffect->SetFloat(m_hBloomHeight, m_fBloomHeightLight);

	RwCameraClear(m_pTempCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pTempCamera))
	{
		RwD3D9SetTexture(m_pSourceTexture, 0);

		m_lpDevice->SetTextureStageState(0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1);
		m_lpDevice->SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);

		m_lpDevice->SetFVF(D3DFVF_RHW_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_RhwVertex, sizeof(D3D_RHW_VERTEX));

		m_lpDevice->SetTextureStageState(0, D3DTSS_COLOROP,		D3DTOP_MODULATE);
		m_lpDevice->SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);

		RwCameraEndUpdate(m_pTempCamera);
	}

	m_lpEffect->EndPass();
}

//------------------------------------------------------------------
//	FuncName	: Update_Power_Mono
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Update_Power_Mono(RwRGBA& color, HWND hWnd)
{
	m_lpEffect->BeginPass(EFFECT_PASS_MONO);
	m_lpEffect->SetFloat(m_hPowerHeight, m_fPowerHeight);

	RwCameraClear(m_pTempCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pTempCamera))
	{
		RwD3D9SetTexture(m_pSourceTexture, 0);

		m_lpDevice->SetTextureStageState(0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1);
		m_lpDevice->SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);

		m_lpDevice->SetFVF(D3DFVF_RHW_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_RhwVertex, sizeof(D3D_RHW_VERTEX));

		m_lpDevice->SetTextureStageState(0, D3DTSS_COLOROP,		D3DTOP_MODULATE);
		m_lpDevice->SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);

		RwCameraEndUpdate(m_pTempCamera);
	}
	m_lpEffect->EndPass();
}

//------------------------------------------------------------------
//	FuncName	: Update_Power_Mono
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Update_Power_MonoPower(RwRGBA& color, HWND hWnd)
{
	m_lpEffect->BeginPass(EFFECT_PASS_MONO_POWER);
	m_lpEffect->SetFloat(m_hPowerHeight, m_fPowerHeight);

	RwCameraClear(m_pTempCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pTempCamera))
	{
		RwD3D9SetTexture(m_pSourceTexture, 0);

		m_lpDevice->SetTextureStageState(0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1);
		m_lpDevice->SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);

		m_lpDevice->SetFVF(D3DFVF_RHW_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_RhwVertex, sizeof(D3D_RHW_VERTEX));

		m_lpDevice->SetTextureStageState(0, D3DTSS_COLOROP,		D3DTOP_MODULATE);
		m_lpDevice->SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);

		RwCameraEndUpdate(m_pTempCamera);
	}

	m_lpEffect->EndPass();
}

//------------------------------------------------------------------
//	FuncName	: Update_Gaussain
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Update_GaussainFiltering(RwRGBA& color, HWND hWnd)
{
	m_lpEffect->BeginPass(EFFECT_PASS_GAUSSAIN_X);
	m_lpEffect->SetFloatArray(m_hGaussainWeight, m_fGaussainWeightTable, 7);
	m_lpEffect->SetFloat(m_hMapWidth, m_nBlurTextureWidth);
	m_lpEffect->SetFloat(m_hMapHeight, m_nBlurTextureHeight);

	// ==========================================
	// 가로 방향 가우시안 필터
	// ==========================================
	RwCameraClear(m_pGaussianCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pGaussianCamera))
	{
		RwD3D9SetTexture(m_pTempTexture, 0);

		m_lpDevice->SetFVF(D3DFVF_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(D3D_VERTEX));

		RwCameraEndUpdate(m_pGaussianCamera);
	}

	m_lpEffect->EndPass();

	// ==========================================
	// 가로 방향 가우시안 필터
	// ==========================================
	m_lpEffect->BeginPass(EFFECT_PASS_GAUSSAIN_Y);

	RwCameraClear(m_pTempCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pTempCamera))
	{
		RwD3D9SetTexture(m_pGaussianTexture, 0);

		m_lpDevice->SetFVF(D3DFVF_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(D3D_VERTEX));

		RwCameraEndUpdate(m_pTempCamera);
	}
	m_lpEffect->EndPass();
}

//------------------------------------------------------------------
//	FuncName	: Update_Reduction
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Update_ReductionFiltering(RwRGBA& color, HWND hWnd)
{
	m_lpEffect->BeginPass(EFFECT_PASS_NONE);

	// ==========================================
	// 블러 화면 축소하기
	// ==========================================
	RwCameraClear(m_pReductionCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pReductionCamera))
	{
		RwD3D9SetTexture(m_pTempTexture, 0);

		m_lpDevice->SetFVF(D3DFVF_RHW_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_ReductionVertex, sizeof(D3D_RHW_VERTEX));

		RwCameraEndUpdate(m_pReductionCamera);
	}
	m_lpEffect->EndPass();


	// ==========================================
	// 블러 화면 확대하기
	// ==========================================
	m_lpEffect->BeginPass(EFFECT_PASS_NONE);

	RwCameraClear(m_pTempCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pTempCamera))
	{
		RwD3D9SetTexture(m_pReductionTexture, 0);
		m_lpDevice->SetFVF(D3DFVF_RHW_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_RhwVertex, sizeof(D3D_RHW_VERTEX));

		RwD3D9SetTexture(NULL, 0);

		RwCameraEndUpdate(m_pTempCamera);
	}
	m_lpEffect->EndPass();
}

//------------------------------------------------------------------
//	FuncName	: Update_FakeHDRFiltering
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Update_FakeHDRFiltering(RwRGBA& color, HWND hWnd)
{
	// ==========================================
	// 화면 축소하기
	// ==========================================
	m_lpEffect->BeginPass(EFFECT_PASS_NONE);
	RwCameraClear(m_pReductionCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pReductionCamera))
	{
		RwD3D9SetTexture(m_pTempTexture, 0);

		m_lpDevice->SetFVF(D3DFVF_RHW_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_ReductionVertex, sizeof(D3D_RHW_VERTEX));

		RwCameraEndUpdate(m_pReductionCamera);
	}
	m_lpEffect->EndPass();


	// ==========================================
	// 축소화면 블러 먹이기
	// ==========================================
	for (RwInt32 i = 0; i < m_nFakeHDRBlurCount; ++ i)
	{
		m_lpEffect->BeginPass(EFFECT_PASS_16BOX);
		m_lpEffect->SetFloat(m_hMapWidth, (RwReal)m_nTextureReductionSize);
		m_lpEffect->SetFloat(m_hMapHeight, (RwReal)m_nTextureReductionSize);

		RwCameraClear(m_pFakeHDRCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
		if(RwCameraBeginUpdate(m_pFakeHDRCamera))
		{
			RwD3D9SetTexture(m_pReductionTexture, 0);

			m_lpDevice->SetFVF(D3DFVF_VERTEX);
			m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(D3D_VERTEX));

			RwCameraEndUpdate(m_pFakeHDRCamera);
		}
		m_lpEffect->EndPass();

		m_lpEffect->BeginPass(EFFECT_PASS_16BOX);
		m_lpEffect->SetFloat(m_hMapWidth, (RwReal)m_nTextureReductionSize);
		m_lpEffect->SetFloat(m_hMapHeight, (RwReal)m_nTextureReductionSize);
		RwCameraClear(m_pReductionCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
		if(RwCameraBeginUpdate(m_pReductionCamera))
		{
			RwD3D9SetTexture(m_pFakeHDRTexture, 0);

			m_lpDevice->SetFVF(D3DFVF_VERTEX);
			m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(D3D_VERTEX));

			RwCameraEndUpdate(m_pReductionCamera);
		}
		m_lpEffect->EndPass();
	}


	// ==========================================
	// 화면 확대하기
	// ==========================================
	m_lpEffect->BeginPass(EFFECT_PASS_NONE);
	RwCameraClear(m_pTempCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pTempCamera))
	{
		RwD3D9SetTexture(m_pReductionTexture, 0);
		m_lpDevice->SetFVF(D3DFVF_RHW_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_RhwVertex, sizeof(D3D_RHW_VERTEX));

		RwD3D9SetTexture(NULL, 0);

		RwCameraEndUpdate(m_pTempCamera);
	}
	m_lpEffect->EndPass();
}

//------------------------------------------------------------------
//	FuncName	: Update_16BoxFiltering
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Update_16BoxFiltering(RwRGBA& color, HWND hWnd)
{
	m_lpEffect->BeginPass(EFFECT_PASS_16BOX);
	m_lpEffect->SetFloat(m_hMapWidth, (RwReal)m_nBlurTextureWidth);
	m_lpEffect->SetFloat(m_hMapHeight, (RwReal)m_nBlurTextureHeight);

	RwCameraClear(m_pGaussianCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pGaussianCamera))
	{
		RwD3D9SetTexture(m_pTempTexture, 0);
		
		m_lpDevice->SetFVF(D3DFVF_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(D3D_VERTEX));

		RwCameraEndUpdate(m_pGaussianCamera);
	}
	m_lpEffect->EndPass();
}

//------------------------------------------------------------------
//	FuncName	: Update_64BoxFiltering
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Update_64BoxFiltering(RwRGBA& color, HWND hWnd)
{
	m_lpEffect->BeginPass(EFFECT_PASS_64BOX);
	m_lpEffect->SetFloat(m_hMapWidth, (RwReal)m_nBlurTextureWidth);
	m_lpEffect->SetFloat(m_hMapHeight, (RwReal)m_nBlurTextureHeight);

	RwCameraClear(m_pGaussianCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pGaussianCamera))
	{
		RwD3D9SetTexture(m_pTempTexture, 0);

		m_lpDevice->SetFVF(D3DFVF_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(D3D_VERTEX));

		RwCameraEndUpdate(m_pGaussianCamera);
	}
	m_lpEffect->EndPass();
}


//------------------------------------------------------------------
//	FuncName	: Update_Last
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Update_Last(RwRGBA& color, HWND hWnd)
{
	m_lpEffect->BeginPass(EFFECT_PASS_LASTDRAW);

	// ==========================================
	// 최종적으로 그리기
	// ==========================================
	RwCameraClear(m_pBlurCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pBlurCamera))
	{
		RwUInt32 nColorOp, nArg1, nArg2;
		RwD3D9GetTextureStageState (1, D3DTSS_COLOROP,	(DWORD*)&nColorOp);
		RwD3D9GetTextureStageState (1, D3DTSS_COLORARG1,	(DWORD*)&nArg1); 
		RwD3D9GetTextureStageState (1, D3DTSS_COLORARG2,	(DWORD*)&nArg2);

		m_lpDevice->SetTextureStageState (1, D3DTSS_COLOROP,	D3DTOP_ADD);
		m_lpDevice->SetTextureStageState (1, D3DTSS_COLORARG1,	D3DTA_CURRENT); 
		m_lpDevice->SetTextureStageState (1, D3DTSS_COLORARG2,	D3DTA_TEXTURE);

    RwD3D9SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    RwD3D9SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    RwD3D9SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		switch(m_nRenderType)
		{
		case LAST_RENDER_NONE:
			{
				m_lpDevice->SetTextureStageState (1, D3DTSS_COLOROP,	D3DTOP_ADD);

				RwD3D9SetTexture(m_pSourceTexture, 0);
				switch(m_nBlurType)
				{
				case BLUR_FILLTER_16BOX:
				case BLUR_FILLTER_64BOX:		RwD3D9SetTexture(m_pGaussianTexture, 1);break;
				default:						RwD3D9SetTexture(m_pTempTexture, 1);	break;
				}
			}
			break;
		case LAST_RENDER_POWER:
			{
				RwD3D9SetTexture(m_pPowerTexture, 0);
			}
			break;
		case LAST_RENDER_FILLTER:
			{
				RwD3D9SetTexture(m_pTempTexture, 0);
			}
			break;
		}

		m_lpDevice->SetFVF(D3DFVF_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(D3D_VERTEX));


    RwD3D9SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    RwD3D9SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    RwD3D9SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		m_lpDevice->SetTextureStageState (1, D3DTSS_COLOROP,	nColorOp); 
		m_lpDevice->SetTextureStageState (1, D3DTSS_COLORARG1,	nArg1); 
		m_lpDevice->SetTextureStageState (1, D3DTSS_COLORARG2,	nArg2); 

		RwD3D9SetTexture(NULL, 0);
		RwD3D9SetTexture(NULL, 1);

		if (m_bDrawTexture)
			DrawTexture();

		RwCameraEndUpdate(m_pBlurCamera);
	}
	m_lpEffect->EndPass();
}

//------------------------------------------------------------------
//	FuncName	: Update_Power
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::Update_Power(RwRGBA& color, HWND hWnd)
{
	m_lpEffect->BeginPass(EFFECT_PASS_NONE);

	RwCameraClear(m_pPowerCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pPowerCamera))
	{
		RwD3D9SetTexture(m_pTempTexture, 0);

		m_lpDevice->SetFVF(D3DFVF_RHW_VERTEX);
		m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_RhwVertex, sizeof(D3D_RHW_VERTEX));

		RwD3D9SetTexture(NULL, 0);

		RwCameraEndUpdate(m_pPowerCamera);
	}

	m_lpEffect->EndPass();
}

//------------------------------------------------------------------
//	FuncName	: DrawTexture
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::DrawTexture()
{
	// Source Texture
	RwD3D9SetTexture(m_pSourceTexture, 0);
	m_lpDevice->SetFVF(D3DFVF_RHW_VERTEX);
	m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_SourceVertex, sizeof(D3D_RHW_VERTEX));

	// Source Texture
	RwD3D9SetTexture(m_pPowerTexture, 0);
	m_lpDevice->SetFVF(D3DFVF_RHW_VERTEX);
	m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_PowerVertex, sizeof(D3D_RHW_VERTEX));

	// Source Texture
	switch(m_nBlurType)
	{
	case BLUR_FILLTER_16BOX:
	case BLUR_FILLTER_64BOX:		RwD3D9SetTexture(m_pGaussianTexture, 0);break;
	default:						RwD3D9SetTexture(m_pTempTexture, 0);	break;
	}
	m_lpDevice->SetFVF(D3DFVF_RHW_VERTEX);
	m_lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_FinalVertex, sizeof(D3D_RHW_VERTEX));
}

//------------------------------------------------------------------
//	FuncName	: DrawInfo
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlBlurCamera::DrawInfo(RtCharset* pCharset, RwInt32 iIdx)
{
	RwChar caption[256];

	switch(m_nPowerType)
	{
	case POWER_FILLTER_NONE:
		RsSprintf(caption, RWSTRING("extraction Mode : %s"), "None");
		break;
	case POWER_FILLTER_BLOOM:
		RsSprintf(caption, RWSTRING("extraction Mode : %s"), "Bloom");
		break;
	case POWER_FILLTER_POWER:
		RsSprintf(caption, RWSTRING("extraction Mode : %s"), "Power");
		break;
	case POWER_FILLTER_MONO:
		RsSprintf(caption, RWSTRING("extraction Mode : %s"), "Mono");
		break;
	case POWER_FILLTER_MONO_POWER:
		RsSprintf(caption, RWSTRING("extraction Mode : %s"), "Mono + Power");
		break;
	}
	RsCharsetPrint(pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

	switch(m_nBlurType)
	{
	case BLUR_FILLTER_NONE:			RsSprintf(caption, RWSTRING("extraction Mode : %s"), "None");			break;
	case BLUR_FILLTER_GAUSSIAN:		RsSprintf(caption, RWSTRING("extraction Mode : %s"), "GAUSSIAN");		break;
	case BLUR_FILLTER_REDUCTION:	RsSprintf(caption, RWSTRING("extraction Mode : %s"), "REDUCTION");		break;
	case BLUR_FILLTER_FAKE_HDR:		RsSprintf(caption, RWSTRING("extraction Mode : %s"), "FAKE HDR");		break;
	case BLUR_FILLTER_16BOX:		RsSprintf(caption, RWSTRING("extraction Mode : %s"), "16BOX");			break;
	case BLUR_FILLTER_64BOX:		RsSprintf(caption, RWSTRING("extraction Mode : %s"), "64BOX");			break;
	}
	RsCharsetPrint(pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

	RsSprintf(caption, RWSTRING("BloomTexture Size(F9) : %d"), m_nTextureReductionSize);
	RsCharsetPrint(pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

	RsSprintf(caption, RWSTRING("Power Height(O, P) : %f"), m_fPowerHeight);
	RsCharsetPrint(pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

	RsSprintf(caption, RWSTRING("Bloom Height(N, M) : %f"), m_fBloomHeightLight);
	RsCharsetPrint(pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);

	RsSprintf(caption, RWSTRING("GaussianBlur Dispersion(K, L) : %f"), m_fGaussainDispersion_sq);
	RsCharsetPrint(pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);
}

//-------------------------------------------------------------
// Name: UpdateGaussainWeightTable()
// Desc: 가중치 계산
//-------------------------------------------------------------
void CNtlBlurCamera::UpdateGaussainWeightTable(FLOAT fGaussainDispersion_sq)
{
	m_fGaussainWeightTable[0] = 0.1f		* m_fGaussainDispersion_sq;
	m_fGaussainWeightTable[1] = 0.358f		* m_fGaussainDispersion_sq;
	m_fGaussainWeightTable[2] = 0.773f		* m_fGaussainDispersion_sq;
	m_fGaussainWeightTable[3] = 1.0f		* m_fGaussainDispersion_sq;
	m_fGaussainWeightTable[4] = 0.773f		* m_fGaussainDispersion_sq;
	m_fGaussainWeightTable[5] = 0.358f		* m_fGaussainDispersion_sq;
	m_fGaussainWeightTable[6] = 0.1f		* m_fGaussainDispersion_sq;
}

//-------------------------------------------------------------
// Name: CreateReductionTexture()
// Desc: 가중치 계산
//-------------------------------------------------------------
void CNtlBlurCamera::CreateReductionTexture(RwInt32 nTextureSize)
{
	if (m_pReductionCamera != NULL)
	{
		DestroyBlurCamera(m_pReductionCamera, m_pReductionTexture);
	}
	if (m_pFakeHDRCamera != NULL)
	{
		DestroyBlurCamera(m_pFakeHDRCamera, m_pFakeHDRTexture);
	}

	m_ReductionVertex[0].x = 0-0.5;				m_ReductionVertex[0].y =	nTextureSize-0.5;	m_ReductionVertex[0].z = 0;	m_ReductionVertex[0].w = 1;	m_ReductionVertex[0].u = 0.f;	m_ReductionVertex[0].v = 1.f;
	m_ReductionVertex[1].x = 0-0.5;				m_ReductionVertex[1].y =	0-0.5;				m_ReductionVertex[1].z = 0;	m_ReductionVertex[1].w = 1;	m_ReductionVertex[1].u = 0.f;	m_ReductionVertex[1].v = 0.f;
	m_ReductionVertex[2].x = nTextureSize-0.5;	m_ReductionVertex[2].y =	nTextureSize-0.5;	m_ReductionVertex[2].z = 0;	m_ReductionVertex[2].w = 1;	m_ReductionVertex[2].u = 1.f;	m_ReductionVertex[2].v = 1.f;
	m_ReductionVertex[3].x = nTextureSize-0.5;	m_ReductionVertex[3].y =	0-0.5;				m_ReductionVertex[3].z = 0;	m_ReductionVertex[3].w = 1;	m_ReductionVertex[3].u = 1.f;	m_ReductionVertex[3].v = 0.f;

	if (!CreateBlurCamera(m_pReductionCamera, m_pReductionTexture, nTextureSize, nTextureSize, 0.1f, 1000.f))
	{
		return;
	}
	if (!CreateBlurCamera(m_pFakeHDRCamera, m_pFakeHDRTexture, nTextureSize, nTextureSize, 0.1f, 1000.f))
	{
		return;
	}
}