#include "precomp_dboclient.h"
#include "SurfaceCamera.h"

#include "NtlDebug.h"
#include "NtlMath.h"

#include "NtlPLApi.h"

CSurfaceCamera::CSurfaceCamera()
:m_pCamera(NULL)
,m_pRenderTexture(NULL)
{

}

CSurfaceCamera::~CSurfaceCamera()
{

}

VOID CSurfaceCamera::Create(RwInt32 iWidth, RwInt32 iHeight)
{
	NTL_FUNCTION("CSurfaceCamera::Create");

	// Create raster camera
	m_pCamera = RwCameraCreate();

	// create camera
	RwCameraSetProjection(m_pCamera, rwPARALLEL);
	RwCameraSetNearClipPlane (m_pCamera, 0.001f);
	RwCameraSetFarClipPlane (m_pCamera, 10.f);

	RwCameraSetFrame(m_pCamera, RwFrameCreate());
	RwCameraSetRaster(m_pCamera, RwRasterCreate(iWidth, iHeight, 0, rwRASTERTYPECAMERATEXTURE | rwRASTERFORMAT8888 ));

	m_pRenderTexture = RwTextureCreate(m_pCamera->frameBuffer);
	RwTextureSetFilterMode(m_pRenderTexture, rwFILTERLINEAR);

	RwReal fViewWindow = RwTan(NTL_DEG2RAD(45.f * 0.5f));
	RwV2d vw;

	vw.y = vw.x = fViewWindow;
	vw.y = ((float)iHeight * fViewWindow) / (float)iWidth;

	RwCameraSetViewWindow(m_pCamera, &vw);


	// delete Cz
	// set look, up, right 
	/*
	RwV3d vCameraPos, vLookAt;

	vCameraPos.x = 0.0f;
	vCameraPos.y = 0.0f;
	vCameraPos.z = 0.0f;

	vLookAt.x = 0.0f;
	vLookAt.y = 0.0f;
	vLookAt.z = 0.0f;

	API_PL_CameraLookAt(m_pCamera, &vCameraPos, &vLookAt);
	*/

	NTL_RETURNVOID();
}

VOID CSurfaceCamera::Destroy()
{
	NTL_FUNCTION("CSurfaceCamera::Destroy");

	if( m_pCamera )
	{
		RwFrame* pFrame = RwCameraGetFrame(m_pCamera);
		if( pFrame )
		{
			RwCameraSetFrame(m_pCamera, NULL);
			RwFrameDestroy(pFrame);
		}	

		RwRaster* pRaster = RwCameraGetRaster(m_pCamera);
		if( pRaster )
		{
			RwCameraSetRaster(m_pCamera, NULL);
			RwRasterDestroy(pRaster);
		}

		RwCameraDestroy(m_pCamera);
		m_pCamera = NULL;
	}
	
	if( m_pRenderTexture )
	{
		RwTextureSetRaster(m_pRenderTexture, NULL);
		RwTextureDestroy(m_pRenderTexture);
		m_pRenderTexture = NULL;
	}

	NTL_RETURNVOID();
}

RwBool CSurfaceCamera::CameraBeginUpdate()
{
	RwRGBA camTexColor = {0, 0, 0, 255};
	if (RwCameraClear(m_pCamera, &camTexColor, rwCAMERACLEARIMAGE))
	{
		if( RwCameraBeginUpdate(m_pCamera) )
		{
			return TRUE;
		}
	}
	return FALSE;
}

VOID CSurfaceCamera::CameraEndUpdate()
{
	NTL_ASSERT(m_pCamera == RwCameraGetCurrentCamera(), "Don't EndUpdate different camera");
	RwCameraEndUpdate(m_pCamera);
}

VOID CSurfaceCamera::CreateTexture()
{
	m_GUITexture.Load(m_pRenderTexture);
}

gui::CTexture* CSurfaceCamera::GetTexture()
{
	return &m_GUITexture;
}