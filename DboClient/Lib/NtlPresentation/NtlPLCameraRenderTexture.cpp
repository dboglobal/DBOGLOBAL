#include "precomp_ntlpresentation.h"
#include "NtlMath.h"
#include "NtlPLEntity.h"
#include "NtlPLApi.h"
#include "NtlPLCameraRenderTexture.h"

/**
 * 객체 생성
 * \param nWidth: Texture의 가로 사이즈
 * \param nHeight; Texture의 세로 사이즈
 */
void CNtlPLCameraRenderTexture::Create( RwInt32 nTexWidth, RwInt32 nTexHeight, RwInt32 nRealWidth, RwInt32 nRealHeight, RwReal fFov, RwBool _IsCamInWorld)
{
	NTL_FUNCTION("CNtlPLCameraRenderTexture::Create()");

	m_pCamera = RwCameraCreate();
	if(m_pCamera)
	{
		RwCameraSetFrame(m_pCamera, RwFrameCreate());

		RwCameraSetRaster(m_pCamera, RwRasterCreate(nTexWidth, nTexHeight, 0, rwRASTERTYPECAMERATEXTURE | rwRASTERFORMAT8888 ));
		RwCameraSetZRaster(m_pCamera, RwRasterCreate(nTexWidth, nTexHeight, 0, rwRASTERTYPEZBUFFER));
		
		RwCameraSetNearClipPlane (m_pCamera, 0.001f);
		RwCameraSetFarClipPlane (m_pCamera, 10.f);
		
		SetCalcFov(fFov, nRealWidth, nRealHeight);
		RwRaster *pRaster = RwCameraGetRaster(m_pCamera);
		m_pRenderTexture = RwTextureCreate(pRaster);
		RwTextureSetFilterMode(m_pRenderTexture, rwFILTERLINEAR);
	}

	m_IsCamInWorld = _IsCamInWorld;
	if(m_IsCamInWorld)
	{
		RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, m_pCamera);
	}

	NTL_RETURNVOID();
}

/**
 * 객체 소멸
 */
void CNtlPLCameraRenderTexture::Destroy()
{
	NTL_FUNCTION("CNtlPLCameraRenderTexture::Destroy()");

	if(m_IsCamInWorld)
	{
		RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, m_pCamera);
	}

	if(m_pCamera)
	{
		RwRaster *pRaster;
		RwFrame *pFrame;

		pFrame = RwCameraGetFrame(m_pCamera);
		if(pFrame)
		{
			RwCameraSetFrame(m_pCamera, NULL);
			RwFrameDestroy(pFrame);
		}

		pRaster = RwCameraGetRaster(m_pCamera);
		if(pRaster)
		{
			//RwRasterDestroy(pRaster);
			RwCameraSetRaster(m_pCamera, NULL);
		}

		pRaster = RwCameraGetZRaster(m_pCamera);
		if(pRaster)
		{
			RwRasterDestroy(pRaster);
			RwCameraSetZRaster(m_pCamera, NULL);
		}

		RwCameraDestroy(m_pCamera);

		m_pCamera = NULL;

		RwTextureDestroy(m_pRenderTexture);
		m_pRenderTexture = NULL;
	}
	
	NTL_RETURNVOID();
}

void CNtlPLCameraRenderTexture::SetNear(RwReal _Near)
{
	RwCameraSetNearClipPlane (m_pCamera, _Near);
}

void CNtlPLCameraRenderTexture::SetFar(RwReal _Far)
{
	RwCameraSetFarClipPlane (m_pCamera, _Far);
}

/**
 * Fov를 구하기 위한 함수
 * \param fFov: Fov
 */
void CNtlPLCameraRenderTexture::SetCalcFov(RwReal fFov, RwInt32 nWidth, RwInt32 nHeight)
{
	NTL_FUNCTION("CNtlPLCameraRenderTexture::SetCalcFov()");
	RwReal fViewWindow = RwTan(NTL_DEG2RAD(fFov * 0.5f));
	RwV2d vw;
	
	vw.x = fViewWindow;
    vw.y = ((float)nHeight * fViewWindow) / (float)nWidth;

    RwCameraSetViewWindow(m_pCamera, &vw);

	NTL_RETURNVOID();
}

/**
 * Texture에 Entity를 Rendering을 한다.
 * \param pEntity Rendering을 할 Entity
 */
void CNtlPLCameraRenderTexture::RenderTexture(CNtlPLEntity *pEntity)
{
	NTL_FUNCTION("CNtlPLCameraRenderTexture::RenderTexture(CNtlPLEntity *pEntity)");
	RwRGBA camTexColor = { 0, 0, 0, 0};
	if (RwCameraClear(m_pCamera, &camTexColor, rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
	{
		if( RwCameraBeginUpdate(m_pCamera))
		{
			if(pEntity)
			{
				pEntity->RenderToTexture();
			}
			RwCameraEndUpdate(m_pCamera);
		}
	}
	
	NTL_RETURNVOID();
}


/**
 * Texture에 Entity를 Rendering을 한다.
 * \param pEntity Rendering을 할 Entity
 */
void CNtlPLCameraRenderTexture::RenderTexture(std::list<CNtlPLEntity*>& listEntity)
{
	NTL_FUNCTION("CNtlPLCameraRenderTexture::RenderTexture(std::list<CNtlPLEntity*>& listEntity)");
	RwRGBA camTexColor = { 0, 0, 0, 0};
	if (RwCameraClear(m_pCamera, &camTexColor, rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
	{
		std::list<CNtlPLEntity*>::iterator it;

		if( RwCameraBeginUpdate(m_pCamera))
		{
			for(it = listEntity.begin(); it != listEntity.end(); it++)
			{
				(*it)->RenderToTexture();
			}
			RwCameraEndUpdate(m_pCamera);
		}
	}
	
	NTL_RETURNVOID();
}

/**
 * Camera Transforam
 * \param pCameraPos Camera Position
 * \param pLookAt Camera Target Position
 */
void CNtlPLCameraRenderTexture::SetCameraPosition(const RwV3d *pCameraPos, const RwV3d *pLookAt)
{
	NTL_FUNCTION("CNtlPLCameraRenderTexture::SetCameraPosition()");

	NTL_ASSERTE(m_pCamera);
	NTL_ASSERTE(pCameraPos);
	NTL_ASSERTE(pLookAt);

	API_PL_CameraLookAt(m_pCamera, pCameraPos, pLookAt);

	NTL_RETURNVOID();
}


/**
 * Target 카메라에 Rendered Texture를 Render한다.
 * \param pRenderTarget Texture을 렌더링할 카메라
 * \param fPos Texture가 표시될 위치
 * \param fSize Texture의 Size 
 */
void CNtlPLCameraRenderTexture::Render(RwCamera *pRenderTarget, RwV2d fPos, RwV2d fSize)
{
	NTL_FUNCTION("CNtlPLCameraRenderTexture::Render()");

	RwIm2DVertex	pVertex[4];
	RwReal			recipCameraZ = 1.0f / RwCameraGetNearClipPlane(pRenderTarget);
	
	RwIm2DVertexSetScreenX(&pVertex[0], fPos.x);
	RwIm2DVertexSetScreenY(&pVertex[0], fPos.y);
	RwIm2DVertexSetScreenZ(&pVertex[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&pVertex[0], recipCameraZ);
	RwIm2DVertexSetU(&pVertex[0], 0, recipCameraZ);
	RwIm2DVertexSetV(&pVertex[0], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&pVertex[0], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&pVertex[1], fPos.x);
	RwIm2DVertexSetScreenY(&pVertex[1], fPos.y + fSize.y);
	RwIm2DVertexSetScreenZ(&pVertex[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&pVertex[1], recipCameraZ);
	RwIm2DVertexSetU(&pVertex[1], 0, recipCameraZ);
	RwIm2DVertexSetV(&pVertex[1], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&pVertex[1], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&pVertex[2], fPos.x + fSize.x);
	RwIm2DVertexSetScreenY(&pVertex[2], fPos.y);
	RwIm2DVertexSetScreenZ(&pVertex[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&pVertex[2], recipCameraZ);
	RwIm2DVertexSetU(&pVertex[2], 1, recipCameraZ);
	RwIm2DVertexSetV(&pVertex[2], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&pVertex[2], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&pVertex[3], fPos.x + fSize.x);
	RwIm2DVertexSetScreenY(&pVertex[3], fPos.y + fSize.y);
	RwIm2DVertexSetScreenZ(&pVertex[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&pVertex[3], recipCameraZ);
	RwIm2DVertexSetU(&pVertex[3], 1, recipCameraZ);
	RwIm2DVertexSetV(&pVertex[3], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&pVertex[3], 255, 255, 255, 255); 

	//if( RwCameraBeginUpdate(pRenderTarget) )
    //{
		RwD3D9SetTexture(NULL, 0);
		RwD3D9SetTexture(NULL, 1);
	
		RwD3D9SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
		RwD3D9SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
		RwD3D9SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_DIFFUSE); 

		// stage 0 alpha : nada 
		RwD3D9SetTextureStageState (0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
		RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE); 

		RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEGOURAUD);
		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) FALSE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *) NULL);
		
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *) FALSE);
		RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
	
		RwD3D9SetTexture(m_pRenderTexture, 0);
		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, pVertex, 4);
		RwD3D9SetTexture(NULL, 0);
		RwD3D9SetTexture(NULL, 1);

		RwD3D9SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
		RwD3D9SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
		RwD3D9SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_CURRENT); 

		RwD3D9SetTextureStageState (0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); 
		RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
		RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG2, D3DTA_CURRENT); 

		RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *) rwSHADEMODEGOURAUD);
		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) TRUE);
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *) NULL);

		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *) FALSE);
		RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
		
		//RwCameraEndUpdate(pRenderTarget);
	//}

	NTL_RETURNVOID();
}