//***********************************************************************************
//	File		:	NtlBlurCamera.cpp
//	Desc		:	
//	Begin		:	2006. 2.13
//	Copyright	:	ⓒ 2006 by Mahwang CO., Ltd
//	Author		:	Mahwang
//	Update		:	
//***********************************************************************************

#include "precomp_dboclient.h"

#include "NtlPostEffectCamera.h"
#include "NtlText.h"

#include "NtlPLRenderState.h"

CNtlPostEffectCamera* CNtlPostEffectCamera::m_pInstance = NULL;

CNtlPostEffectCamera::CNtlPostEffectCamera(void)
{
	m_pInstance				= this;

	m_pSourceTexture		= NULL;

	m_nBlurTextureWidth		= 128;;
	m_nBlurTextureHeight	= 128;

	m_pBlurCamera			= NULL;

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

	m_bUsePostEffect		= FALSE;

	m_fMonoPowerHeight		= 2.4f;

	m_vRGB2Y				= D3DXVECTOR4(0.29900f, 0.4f, 0.11400f, 0.f);
//	m_vRGB2Y				= D3DXVECTOR4(0.29900f, 0.58700f, 0.11400f, 0.f);

	m_nTextureReductionSize	= 64;

	m_pCominTexture			= NULL;
	m_nComicState			= COMIC_STATE_IDLE;
	m_nComicAlpha			= 0;

	m_handleKeyDown			= CInputHandler::GetInstance()->LinkKeyDown(this, &CNtlPostEffectCamera::KeyboardDownHandler);
	m_handleRestore			= CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRestore(this, &CNtlPostEffectCamera::RestoreHandler);
}


CNtlPostEffectCamera::~CNtlPostEffectCamera(void)
{
	CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRestore(m_handleRestore);

	CInputHandler::GetInstance()->UnLinkKeyDown(m_handleKeyDown);
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
	DestroyBlurCamera(m_pReductionCamera, m_pReductionTexture);

	DestroyBlurCamera(m_pFakeHDRCamera, m_pFakeHDRTexture);

	DestroyBlurCamera(m_pTempCamera, m_pTempTexture);

	DestroyBlurCamera(m_pRwCamera, m_pSourceTexture);

	m_pRwCamera = m_pBlurCamera;
	m_pBlurCamera = NULL;

	if (m_pCominTexture != NULL)
	{
		RwTextureDestroy(m_pCominTexture);
		m_pCominTexture = NULL;
	}

	CNtlCamera::Destroy();
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
RwCamera* CNtlPostEffectCamera::Create(RwInt32 width, 
							 RwInt32 height, 
							 RwBool zBuffer, 
							 RwReal fNearClipPlane, 
							 RwReal fFarClipPlane)
{
	m_nBlurTextureWidth = width;
	m_nBlurTextureHeight = height;

	m_lpDevice = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();

	if (!CreateEffect())
	{
		return NULL;
	}

	m_pBlurCamera = CNtlCamera::Create(width, height, zBuffer, fNearClipPlane, fFarClipPlane);

	if (!CreateBlurCamera(m_pRwCamera, m_pSourceTexture, width, height, fNearClipPlane, fFarClipPlane))
	{
		return NULL;
	}
	if (!CreateBlurCamera(m_pTempCamera, m_pTempTexture, width, height, fNearClipPlane, fFarClipPlane))
	{
		return NULL;
	}

	CreateReductionTexture(m_nTextureReductionSize);

	BuildVertex(width, height);

	SetComicVertex(width, height);


	RwImVertexIndex ComicIndex[] = {	0, 1, 2,		\
										0, 3, 4, 		\
										0, 5, 6, 		\
										0, 7, 8, 		\
										0, 9, 10, 		\
										0, 11, 12, 		\
										0, 13, 14, 		\
										0, 15, 16};
	memcpy(m_ComicIndex, ComicIndex, sizeof(RwImVertexIndex) * 24);

	RwImVertexIndex ComicIndex2[] = {	0, 1, 2,		\
										0, 2, 3, 		\
										0, 3, 4, 		\
										0, 4, 5, 		\
										0, 5, 6, 		\
										0, 6, 7, 		\
										0, 7, 8, 		\
										0, 8, 9, 		\
										0, 9, 10, 		\
										0, 10, 11, 		\
										0, 11, 12, 		\
										0, 12, 13, 		\
										0, 13, 14, 		\
										0, 14, 15, 		\
										0, 15, 16, 		\
										0, 16, 17};
	memcpy(m_ComicIndex2, ComicIndex2, sizeof(RwImVertexIndex) * 48);

	RwImVertexIndex ComicIndex3[] = { 9, 13, 1, 5 };
	memcpy(m_ComicIndex3, ComicIndex3, sizeof(RwImVertexIndex) * 4);
/*	RwIm2DVertexSetU(&m_ComicVertex[9], 1, 0);
	RwIm2DVertexSetV(&m_ComicVertex[9], 0, 0);

	RwIm2DVertexSetU(&m_ComicVertex[13], 0, 0);
	RwIm2DVertexSetV(&m_ComicVertex[13], 0, 0);

	RwIm2DVertexSetU(&m_ComicVertex[1], 0, 0);
	RwIm2DVertexSetV(&m_ComicVertex[1], 1, 0);

	RwIm2DVertexSetU(&m_ComicVertex[5], 1, 0);
	RwIm2DVertexSetV(&m_ComicVertex[5], 1, 0);
*/
    RwImageSetPath("texture\\effect\\");
	m_pCominTexture = RwTextureRead("Zoom.PNG", "Zoom.PNG");


	return m_pRwCamera;
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
}

void CNtlPostEffectCamera::SetRhwVertex(RwIm2DVertex* pVertex, RwInt32 nStartX, RwInt32 nStartY, RwInt32 width, RwInt32 height)
{
	RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(m_pRwCamera);

	RwIm2DVertexSetScreenX(&pVertex[0], nStartX-0.5f);
	RwIm2DVertexSetScreenY(&pVertex[0], nStartY-0.5f);
	RwIm2DVertexSetScreenZ(&pVertex[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&pVertex[0], recipCameraZ);
	RwIm2DVertexSetU(&pVertex[0], 0, recipCameraZ);
	RwIm2DVertexSetV(&pVertex[0], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&pVertex[0], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&pVertex[1], nStartX-0.5f);
	RwIm2DVertexSetScreenY(&pVertex[1], nStartY+height-0.5f);
	RwIm2DVertexSetScreenZ(&pVertex[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&pVertex[1], recipCameraZ);
	RwIm2DVertexSetU(&pVertex[1], 0, recipCameraZ);
	RwIm2DVertexSetV(&pVertex[1], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&pVertex[1], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&pVertex[2], nStartX+width-0.5f);
	RwIm2DVertexSetScreenY(&pVertex[2], nStartY-0.5f);
	RwIm2DVertexSetScreenZ(&pVertex[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&pVertex[2], recipCameraZ);
	RwIm2DVertexSetU(&pVertex[2], 1, recipCameraZ);
	RwIm2DVertexSetV(&pVertex[2], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&pVertex[2], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&pVertex[3], nStartX+width-0.5f);
	RwIm2DVertexSetScreenY(&pVertex[3], nStartY+height-0.5f);
	RwIm2DVertexSetScreenZ(&pVertex[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&pVertex[3], recipCameraZ);
	RwIm2DVertexSetU(&pVertex[3], 1, recipCameraZ);
	RwIm2DVertexSetV(&pVertex[3], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&pVertex[3], 255, 255, 255, 255); 
}

void CNtlPostEffectCamera::SetComicVertex(RwInt32 width, RwInt32 height)
{
	RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(m_pRwCamera);
	for (RwInt32 i = 0; i < 18; ++i)
	{
		RwIm2DVertexSetScreenZ(&m_ComicVertex[i], RwIm2DGetNearScreenZ());
		RwIm2DVertexSetRecipCameraZ(&m_ComicVertex[i], recipCameraZ);
		RwIm2DVertexSetIntRGBA(&m_ComicVertex[i], 255, 255, 255, 128); 
	}

	RwIm2DVertexSetScreenX(&m_ComicVertex[0], (width / 2.f));
	RwIm2DVertexSetScreenY(&m_ComicVertex[0], (height / 2.f));
	RwIm2DVertexSetU(&m_ComicVertex[0], 0.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[0], 0.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[0], (width / 2.f));
	RwIm2DVertexSetScreenY(&m_ComicVertex[0], (height / 2.f));
	RwIm2DVertexSetU(&m_ComicVertex[0], 0.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[0], 0.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[1], (width * (0.f / 4.f)));
	RwIm2DVertexSetScreenY(&m_ComicVertex[1], height);
	RwIm2DVertexSetU(&m_ComicVertex[1], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[1], 1.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[2], (width * (1.f / 4.f)));
	RwIm2DVertexSetScreenY(&m_ComicVertex[2], height);
	RwIm2DVertexSetU(&m_ComicVertex[2], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[2], 0.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[3], (width * (2.f / 4.f)));
	RwIm2DVertexSetScreenY(&m_ComicVertex[3], height);
	RwIm2DVertexSetU(&m_ComicVertex[3], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[3], 1.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[4], (width * (3.f / 4.f)));
	RwIm2DVertexSetScreenY(&m_ComicVertex[4], height);
	RwIm2DVertexSetU(&m_ComicVertex[4], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[4], 0.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[5], (width * (4.f / 4.f)));
	RwIm2DVertexSetScreenY(&m_ComicVertex[5], height);
	RwIm2DVertexSetU(&m_ComicVertex[5], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[5], 1.f, recipCameraZ);


	RwIm2DVertexSetScreenX(&m_ComicVertex[6], width);
	RwIm2DVertexSetScreenY(&m_ComicVertex[6], (height * (3.f / 4.f)));
	RwIm2DVertexSetU(&m_ComicVertex[6], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[6], 0.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[7], width);
	RwIm2DVertexSetScreenY(&m_ComicVertex[7], (height * (2.f / 4.f)));
	RwIm2DVertexSetU(&m_ComicVertex[7], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[7], 1.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[8], width);
	RwIm2DVertexSetScreenY(&m_ComicVertex[8], (height * (1.f / 4.f)));
	RwIm2DVertexSetU(&m_ComicVertex[8], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[8], 0.f, recipCameraZ);


	RwIm2DVertexSetScreenX(&m_ComicVertex[9], (width * (4.f / 4.f)));
	RwIm2DVertexSetScreenY(&m_ComicVertex[9], 0.f);
	RwIm2DVertexSetU(&m_ComicVertex[9], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[9], 1.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[10], (width * (3.f / 4.f)));
	RwIm2DVertexSetScreenY(&m_ComicVertex[10], 0.f);
	RwIm2DVertexSetU(&m_ComicVertex[10], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[10], 0.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[11], (width * (2.f / 4.f)));
	RwIm2DVertexSetScreenY(&m_ComicVertex[11], 0.f);
	RwIm2DVertexSetU(&m_ComicVertex[11], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[11], 1.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[12], (width * (1.f / 4.f)));
	RwIm2DVertexSetScreenY(&m_ComicVertex[12], 0.f);
	RwIm2DVertexSetU(&m_ComicVertex[12], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[12], 0.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[13], (width * (0.f / 4.f)));
	RwIm2DVertexSetScreenY(&m_ComicVertex[13], 0.f);
	RwIm2DVertexSetU(&m_ComicVertex[13], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[13], 1.f, recipCameraZ);


	RwIm2DVertexSetScreenX(&m_ComicVertex[14], 0.f);
	RwIm2DVertexSetScreenY(&m_ComicVertex[14], (height * (1.f / 4.f)));
	RwIm2DVertexSetU(&m_ComicVertex[14], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[14], 0.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[15], 0.f);
	RwIm2DVertexSetScreenY(&m_ComicVertex[15], (height * (2.f / 4.f)));
	RwIm2DVertexSetU(&m_ComicVertex[15], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[15], 1.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[16], 0.f);
	RwIm2DVertexSetScreenY(&m_ComicVertex[16], (height * (3.f / 4.f)));
	RwIm2DVertexSetU(&m_ComicVertex[16], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[16], 0.f, recipCameraZ);

	RwIm2DVertexSetScreenX(&m_ComicVertex[17], (width * (0.f / 4.f)));
	RwIm2DVertexSetScreenY(&m_ComicVertex[17], height);
	RwIm2DVertexSetU(&m_ComicVertex[17], 1.f, recipCameraZ);
	RwIm2DVertexSetV(&m_ComicVertex[17], 1.f, recipCameraZ);
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
void CNtlPostEffectCamera::UpdateBlurFilter(RwRGBA& color)
{
	if (m_bUsePostEffect && m_lpEffect != NULL)
	{
		m_lpEffect->SetTechnique(m_hTechnique);
		m_lpEffect->Begin(NULL,0);	

		Update_Power_MonoPower(color);
		Update_FakeHDRFiltering(color);

		m_lpEffect->End();
	}
}

//------------------------------------------------------------------
//	FuncName	: Update_Power_None
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::Update_Power_None(RwRGBA& color)
{
	RwCameraClear(m_pBlurCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pBlurCamera))
	{
		BeginPostEffectState(FALSE);

		RwD3D9SetTexture(m_pSourceTexture, 0);
		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);

		EndPostEffectState(FALSE);

		RwCameraEndUpdate(m_pBlurCamera);
	}
}


//------------------------------------------------------------------
//	FuncName	: Update_Power_MonoPower
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::Update_Power_MonoPower(RwRGBA& color)
{
	m_lpEffect->BeginPass(EFFECT_PASS_MONO_POWER);
	m_lpEffect->SetFloat(m_hMonoPowerHeight, m_fMonoPowerHeight);
	m_lpEffect->SetVector(m_hRGB2Y, &m_vRGB2Y);

	RwCameraClear(m_pTempCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pTempCamera))
	{
		BeginPostEffectState(FALSE);

		RwD3D9SetTexture(m_pSourceTexture, 0);
		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);

		EndPostEffectState(FALSE);

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
void CNtlPostEffectCamera::Update_FakeHDRFiltering(RwRGBA& color)
{
	RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

	// ==========================================
	// 화면 축소하기
	// ==========================================
	m_lpEffect->BeginPass(EFFECT_PASS_NONE);
	RwCameraClear(m_pReductionCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pReductionCamera))
	{
		BeginPostEffectState(FALSE);

		RwD3D9SetTexture(m_pTempTexture, 0);
	    RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwReductionVertex, 4);
		
		EndPostEffectState(FALSE);

		RwCameraEndUpdate(m_pReductionCamera);
	}
	m_lpEffect->EndPass();	


	// ==========================================
	// 축소화면 블러 먹이기
	// ==========================================
	m_lpEffect->BeginPass(EFFECT_PASS_16BOX);
	m_lpEffect->SetFloat(m_hMapWidth, (RwReal)m_nTextureReductionSize);
	m_lpEffect->SetFloat(m_hMapHeight, (RwReal)m_nTextureReductionSize);

	RwCameraClear(m_pFakeHDRCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pFakeHDRCamera))
	{
		RwD3D9SetTexture(m_pReductionTexture, 0);
		if(RwIm3DTransform(m_3dVertex, 4, NULL, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXUV))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
			RwIm3DEnd();
		}

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
		if(RwIm3DTransform(m_3dVertex, 4, NULL, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXUV))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
			RwIm3DEnd();
		}

		RwCameraEndUpdate(m_pReductionCamera);
	}
	m_lpEffect->EndPass();


	// ==========================================
	// 화면 확대하기
	// ==========================================
	m_lpEffect->BeginPass(EFFECT_PASS_NONE);
	RwCameraClear(m_pTempCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pTempCamera))
	{
		BeginPostEffectState(FALSE);

		RwD3D9SetTexture(m_pReductionTexture, 0);
	    RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);

		EndPostEffectState(FALSE);

		RwCameraEndUpdate(m_pTempCamera);
	}
	m_lpEffect->EndPass();

	RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);
}

//------------------------------------------------------------------
//	FuncName	: Update_Last
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlPostEffectCamera::BeginCameraUpdate(RwRGBA& color)
{
	// ==========================================
	// 최종적으로 그리기
	// ==========================================
	RwCameraClear(m_pBlurCamera, &color, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);
	if(RwCameraBeginUpdate(m_pBlurCamera))
	{
		if (m_bUsePostEffect && m_lpEffect != NULL)
		{
			m_lpEffect->SetTechnique(m_hTechnique);
			m_lpEffect->Begin(NULL,0);	
			m_lpEffect->BeginPass(EFFECT_PASS_LASTDRAW);

			RwD3D9SetTexture(m_pSourceTexture, 0);
			RwD3D9SetTexture(m_pTempTexture, 1);

			BeginPostEffectState(TRUE);

			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);
		
			EndPostEffectState(TRUE);

			RwD3D9SetTexture(NULL, 0);
			RwD3D9SetTexture(NULL, 1);

//			DrawTexture();

			m_lpEffect->EndPass();
			m_lpEffect->End();
		}
		else
		{
			BeginPostEffectState(FALSE);

			RwD3D9SetTexture(m_pSourceTexture, 0);
			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_RhwVertex, 4);

			EndPostEffectState(FALSE);
		}

		if (m_nComicState != COMIC_STATE_IDLE)
		{
			if (m_nComicState == COMIC_STATE_FADE_OUT)
			{
				m_nComicAlpha += 10;
				if (m_nComicAlpha >= 255)
				{
					m_nComicAlpha = 255;
					m_nComicState = COMIC_STATE_FADE_IN;
				}
			}
			else
			{
				m_nComicAlpha -= 10;
				if (m_nComicAlpha <= 0)
				{
					m_nComicAlpha = 0; 
					m_nComicState = COMIC_STATE_IDLE;
				}
			}

			for (RwInt32 i = 0; i < 18; ++i)
			{
				RwIm2DVertexSetIntRGBA(&m_ComicVertex[i], 255, 255, 255, m_nComicAlpha); 
			}

			RwD3D9SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,	D3DTOP_MODULATE);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE);

			RwD3D9SetTexture(m_pCominTexture, 0);
			RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, m_ComicVertex, 18, m_ComicIndex, 24);
//			RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, m_ComicVertex, 18, m_ComicIndex2, 48);
//			RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRIFAN, m_ComicVertex, 18, m_ComicIndex3, 4);

			RwD3D9SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
		}
		return TRUE;
	}
	return FALSE;
}


//------------------------------------------------------------------
//	FuncName	: Update_Last
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::EndCameraUpdate(RwRGBA& color)
{
	RwCameraEndUpdate(m_pBlurCamera);
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

int CNtlPostEffectCamera::KeyboardDownHandler(unsigned int uiKeyData)
{
	SKeyData *pData = reinterpret_cast<SKeyData*>(uiKeyData);
	
	if(pData->uiRepCount == 1)
	{
		if(pData->uiChar == VK_INSERT)
		{
			m_bUsePostEffect = !m_bUsePostEffect;
		}
		if(pData->uiChar == VK_DELETE)
		{
			m_nComicState = COMIC_STATE_FADE_OUT;

			RwReal constant = 2.0f * rwPI / 17;
			for(RwInt32 i = 0; i < 17; ++i)
			{
				RwInt32 rw = rand() / 120 - 60;
				RwInt32 rh = rand() / 120 - 60;

				RwIm2DVertexSetScreenX(&m_ComicVertex[1+i], (RwReal)RwCos(i * -constant) * 1000.f + (m_nBlurTextureWidth / 2.f) + rw);
				RwIm2DVertexSetScreenY(&m_ComicVertex[1+i], (RwReal)RwSin(i * -constant) * 1000.f + (m_nBlurTextureHeight / 2.f) + rh);

				if (m_ComicVertex[1+i].x < 0.f) m_ComicVertex[1+i].x = 0.f;
				if (m_ComicVertex[1+i].z < 0.f) m_ComicVertex[1+i].z = 0.f;

				if (m_ComicVertex[1+i].x > m_nBlurTextureWidth) m_ComicVertex[1+i].x = m_nBlurTextureWidth;
				if (m_ComicVertex[1+i].z > m_nBlurTextureHeight) m_ComicVertex[1+i].z = m_nBlurTextureHeight;
			}
			RwIm2DVertexSetScreenX(&m_ComicVertex[17], m_ComicVertex[1].x);
			RwIm2DVertexSetScreenY(&m_ComicVertex[17], m_ComicVertex[1].y);
		}
	}
	return 1;
}


//------------------------------------------------------------------
//	FuncName	: DrawTexture
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlPostEffectCamera::DrawTexture()
{
	RwD3D9SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// Source Texture
	RwD3D9SetTexture(m_pSourceTexture, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_SourceVertex, 4);

	// Source Texture
	RwD3D9SetTexture(m_pReductionTexture, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_PowerVertex, 4);

	// Source Texture
	RwD3D9SetTexture(m_pTempTexture, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_FinalVertex, 4);

	RwD3D9SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CNtlPostEffectCamera::SetCameraSize(RwRect* rect)
{
	if (rect != NULL)
	{
		m_nBlurTextureWidth = rect->w;
		m_nBlurTextureHeight= rect->h;
	}

	CNtlCamera::SetCameraResize(m_pTempCamera, rect);
	CNtlCamera::SetCameraResize(m_pBlurCamera, rect);
	CNtlCamera::SetCameraResize(m_pRwCamera, rect);

	BuildVertex(m_nBlurTextureWidth, m_nBlurTextureHeight);

	RestoreHandler();
}

int CNtlPostEffectCamera::RestoreHandler()
{
	if (m_lpEffect != NULL)
	{
		m_lpEffect->Release();
		m_lpEffect = NULL;

		m_hTechnique			= NULL;
	}

	m_lpDevice = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();

	if (!CreateEffect())
	{
		return NULL;
	}

	return TRUE;
}
