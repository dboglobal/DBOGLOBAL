#include "stdafx.h"

#include "NtlWE.h"
#include "NtlWEFrm.h"
#include "NtlWEDoc.h"

#include "NtlWorldFieldManager.h"

#include "NtlPLRenderState.h"

#include "NtlPLWorldEntity.h"

#include "CameraAttrDlg.h"

#include "UpdateEventCompass.h"

CUpdateEventCompass::CUpdateEventCompass()
{
	m_pCompassCam		= NULL;
	m_pCompassTex		= NULL;
	m_pCompassTexR2S	= NULL;
}

CUpdateEventCompass::~CUpdateEventCompass()
{
	if(m_pCompassCam)
	{
		RwFrame *frame;
		RwRaster *raster;

		RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, m_pCompassCam);

		frame = RwCameraGetFrame(m_pCompassCam);
		RwCameraSetFrame(m_pCompassCam, NULL);
		RwFrameDestroy(frame);

		raster = RwCameraGetRaster(m_pCompassCam);
		RwCameraSetRaster(m_pCompassCam, NULL);
		RwRasterDestroy(raster);

		raster = RwCameraGetZRaster(m_pCompassCam);
		RwCameraSetZRaster(m_pCompassCam, NULL);
		RwRasterDestroy(raster);

		RwCameraDestroy(m_pCompassCam);

		RwTextureSetRaster(m_pCompassTexR2S, NULL);
		RwTextureDestroy(m_pCompassTexR2S);

		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pCompassTex);
	}
}

void CUpdateEventCompass::CreateCompass()
{
	RwReal	recipCameraZ	= 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);
	RwInt32	nStartX			= 0;
	RwInt32 nStartY			= 0;
	RwInt32 width			= 128;
	RwInt32 height			= 128;

	RwIm2DVertexSetScreenX(&m_CompassVert[0], nStartX-0.5f);
	RwIm2DVertexSetScreenY(&m_CompassVert[0], nStartY-0.5f);
	RwIm2DVertexSetScreenZ(&m_CompassVert[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_CompassVert[0], recipCameraZ);
	RwIm2DVertexSetU(&m_CompassVert[0], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_CompassVert[0], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_CompassVert[0], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&m_CompassVert[1], nStartX-0.5f);
	RwIm2DVertexSetScreenY(&m_CompassVert[1], nStartY+height-0.5f);
	RwIm2DVertexSetScreenZ(&m_CompassVert[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_CompassVert[1], recipCameraZ);
	RwIm2DVertexSetU(&m_CompassVert[1], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_CompassVert[1], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_CompassVert[1], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&m_CompassVert[2], nStartX+width-0.5f);
	RwIm2DVertexSetScreenY(&m_CompassVert[2], nStartY-0.5f);
	RwIm2DVertexSetScreenZ(&m_CompassVert[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_CompassVert[2], recipCameraZ);
	RwIm2DVertexSetU(&m_CompassVert[2], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_CompassVert[2], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_CompassVert[2], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&m_CompassVert[3], nStartX+width-0.5f);
	RwIm2DVertexSetScreenY(&m_CompassVert[3], nStartY+height-0.5f);
	RwIm2DVertexSetScreenZ(&m_CompassVert[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_CompassVert[3], recipCameraZ);
	RwIm2DVertexSetU(&m_CompassVert[3], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_CompassVert[3], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_CompassVert[3], 255, 255, 255, 255);

	char szOldDir[NTL_MAX_DIR_PATH] = {0,};
	char szNewDir[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldDir);
	::strcpy(szNewDir, "Tool\\SABER\\Texture\\UI\\");

	:: SetCurrentDirectory(szOldDir);

	m_pCompassTex = CNtlPLResourceManager::GetInstance()->LoadTexture("Compass", szNewDir);
	DBO_ASSERT(m_pCompassTex, "Texture load failed.");

	m_pCompassCam = CreateTextureCamera(CNtlPLGlobal::m_pRpWorld);

	RwIm3DVertexSetU(&m_CompassGeom[0], 0.0f);		
	RwIm3DVertexSetV(&m_CompassGeom[0], 0.0f);
	RwIm3DVertexSetU(&m_CompassGeom[1], 0.0f);		
	RwIm3DVertexSetV(&m_CompassGeom[1], 1.0f);
	RwIm3DVertexSetU(&m_CompassGeom[2], 1.0f);		
	RwIm3DVertexSetV(&m_CompassGeom[2], 0.0f);
	RwIm3DVertexSetU(&m_CompassGeom[3], 1.0f);		
	RwIm3DVertexSetV(&m_CompassGeom[3], 1.0f);
	RwIm3DVertexSetRGBA(&m_CompassGeom[0], 255, 255, 255, 255);
	RwIm3DVertexSetRGBA(&m_CompassGeom[1], 255, 255, 255, 255);
	RwIm3DVertexSetRGBA(&m_CompassGeom[2], 255, 255, 255, 255);
	RwIm3DVertexSetRGBA(&m_CompassGeom[3], 255, 255, 255, 255);
	RwReal Size = 10.0f;
	RwIm3DVertexSetPos(&m_CompassGeom[0], Size/2.f, Size/2.f, 0.0f);
	RwIm3DVertexSetPos(&m_CompassGeom[1], Size/2.f, -Size/2.f, 0.0f);
	RwIm3DVertexSetPos(&m_CompassGeom[2], -Size/2.f, Size/2.f, 0.0f);
	RwIm3DVertexSetPos(&m_CompassGeom[3], -Size/2.f, -Size/2.f, 0.0f);
}

void CUpdateEventCompass::UpdateCompass()
{
	RwRGBA Color = {255, 255, 255, 0};
	if (RwCameraClear(m_pCompassCam, &Color, rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
	{
		if(RwCameraBeginUpdate(m_pCompassCam))
		{
			if(RwIm3DTransform(m_CompassGeom, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
			{
				D3DXMATRIXA16 mTex, mTemp;
				D3DXMatrixIdentity(&mTex);

				D3DXMatrixTranslation(&mTemp, -0.5f, -0.5f, 0);
				D3DXMatrixMultiply(&mTex, &mTex, &mTemp);

				RwReal Radian;
				RwFrame *pCameraFrame	= RwCameraGetFrame(dGETAPP().GetNtlGameCamera()->GetCamera());
				RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
				RwV3d *pFront			= RwMatrixGetAt(pM);
				RwV3d  vZ				= {0.0f, 0.0f, 1.0f};
				RwV3d vFront; 

				CNtlMath::MathRwV3dAssign(&vFront, pFront->x, pFront->y, pFront->z);

				vFront.y = 0.0f;
				RwV3dNormalize(&vFront, &vFront);

				if(pFront->x < 0)
				{
					Radian = RwV3dDotProduct(&vZ, &vFront);
					if(Radian < -1.0f)
					{
						Radian = -1.0f;
					}
					else if(Radian > 1.0f)
					{ 
						Radian = 1.0f;
					}

					Radian = ::acosf(Radian);
				}
				else
				{
					Radian = RwV3dDotProduct(&vFront, &vZ);
					if(Radian < -1.0f)
					{
						Radian = -1.0f;
					}
					else if(Radian > 1.0f)
					{
						Radian = 1.0f;
					}

					Radian = -::acosf(Radian);
				}

				D3DXMatrixRotationZ(&mTemp, Radian);
				D3DXMatrixMultiply(&mTex, &mTex, &mTemp);
				D3DXMatrixTranslation(&mTemp, 0.5f, 0.5f, 0);
				D3DXMatrixMultiply(&mTex, &mTex, &mTemp);
				mTex.m[2][0] = mTex.m[3][0];
				mTex.m[2][1] = mTex.m[3][1];
				mTex.m[3][0] = 0.0f;
				mTex.m[3][1] = 0.0f;

				UpdateAlphaState(TRUE);
				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

				RwD3D9SetTexture(m_pCompassTex, 0);
				RwD3D9SetTransform(D3DTS_TEXTURE0, &mTex);
				RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

				RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);
				RwIm3DEnd();

				UpdateAlphaState(FALSE);
				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);

				RwD3D9SetTexture(NULL, 0);

				D3DXMATRIXA16 mIdentity;
				D3DXMatrixIdentity(&mIdentity);
				RwD3D9SetTransform(D3DTS_TEXTURE0, mIdentity);

				RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			}

			RwCameraEndUpdate(m_pCompassCam);
		}
	}
}

void CUpdateEventCompass::RenderCompass()
{
	if(dGETFRM()->m_pCameraPortal->GetCurCamMode() != 2)
	{
		CNtlWorldFieldManager *pMgr = dGETMGR();
		if(pMgr && pMgr->GetWorldReady() && m_pCompassTexR2S && dGET_WORLD_PARAM()->FlgObjInfoShow)
		{
			UpdateAlphaState(TRUE);

			RwD3D9SetTexture(m_pCompassTexR2S, 0);
			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_CompassVert, 4);	

			UpdateAlphaState(FALSE);
		}
	}
}

void CUpdateEventCompass::RestoreCompass()
{
	RwReal	recipCameraZ	= 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);
	RwInt32	nStartX			= 0;
	RwInt32 nStartY			= 0;
	RwInt32 width			= 128;
	RwInt32 height			= 128;

	RwIm2DVertexSetScreenX(&m_CompassVert[0], nStartX-0.5f);
	RwIm2DVertexSetScreenY(&m_CompassVert[0], nStartY-0.5f);
	RwIm2DVertexSetScreenZ(&m_CompassVert[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_CompassVert[0], recipCameraZ);
	RwIm2DVertexSetU(&m_CompassVert[0], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_CompassVert[0], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_CompassVert[0], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&m_CompassVert[1], nStartX-0.5f);
	RwIm2DVertexSetScreenY(&m_CompassVert[1], nStartY+height-0.5f);
	RwIm2DVertexSetScreenZ(&m_CompassVert[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_CompassVert[1], recipCameraZ);
	RwIm2DVertexSetU(&m_CompassVert[1], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_CompassVert[1], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_CompassVert[1], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&m_CompassVert[2], nStartX+width-0.5f);
	RwIm2DVertexSetScreenY(&m_CompassVert[2], nStartY-0.5f);
	RwIm2DVertexSetScreenZ(&m_CompassVert[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_CompassVert[2], recipCameraZ);
	RwIm2DVertexSetU(&m_CompassVert[2], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_CompassVert[2], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_CompassVert[2], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&m_CompassVert[3], nStartX+width-0.5f);
	RwIm2DVertexSetScreenY(&m_CompassVert[3], nStartY+height-0.5f);
	RwIm2DVertexSetScreenZ(&m_CompassVert[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_CompassVert[3], recipCameraZ);
	RwIm2DVertexSetU(&m_CompassVert[3], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_CompassVert[3], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_CompassVert[3], 255, 255, 255, 255);
}

RwCamera* CUpdateEventCompass::CreateTextureCamera(RpWorld *world)
{
	RwRaster *raster;

	RwInt32 TextureSize = 128;

	raster = RwRasterCreate(TextureSize, TextureSize, 0, rwRASTERTYPECAMERATEXTURE  | rwRASTERFORMAT8888);

	if( raster )
	{
		RwRaster *zRaster;

		zRaster = RwRasterCreate(TextureSize, TextureSize, 0, rwRASTERTYPEZBUFFER);

		if( zRaster )
		{
			RwFrame *frame;

			RwV3d Pos = {0.0, 0.0, -7.0f};
			frame = RwFrameCreate();
			RwFrameSetIdentity(frame);
			RwFrameTranslate(frame, &Pos, rwCOMBINEREPLACE);

			if( frame )
			{
				RwCamera *camera;

				camera = RwCameraCreate();
				if(camera)
				{
					RwV2d vw;

					RwCameraSetRaster(camera, raster);
					RwCameraSetZRaster(camera, zRaster);

					RwCameraSetFrame(camera, frame);

					RwCameraSetNearClipPlane(camera, 0.1f);
					RwCameraSetFarClipPlane(camera, 250.0f);

					vw.x = vw.y = 0.4f;
					RwCameraSetViewWindow(camera, &vw);

					RpWorldAddCamera(world, camera);

					m_pCompassTexR2S = RwTextureCreate(raster);
					DBO_ASSERT(m_pCompassTexR2S, "Texture create failed.");

					RwTextureSetFilterMode(m_pCompassTexR2S, rwFILTERLINEAR);
					RwTextureSetAddressing(m_pCompassTexR2S, rwTEXTUREADDRESSWRAP);

					return camera;
				}

				RwFrameDestroy(frame);
			}

			RwRasterDestroy(zRaster);
		}

		RwRasterDestroy(raster);
	}

	return NULL;
}