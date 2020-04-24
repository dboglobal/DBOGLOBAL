#include "precomp_ntlpresentation.h"
#include "NtlPLCameraRT.h"

#include "NtlDebug.h"
#include "NtlMath.h"

#include "NtlPLApi.h"

CNtlPLCameraRT::CNtlPLCameraRT()
:m_pCamera(NULL)
,m_pRenderTexture(NULL)
{

}

CNtlPLCameraRT::~CNtlPLCameraRT()
{

}

VOID CNtlPLCameraRT::Create(RwInt32 iWidth, RwInt32 iHeigth)
{
	NTL_FUNCTION("CNtlPLCameraRT::Create");

	// Create raster camera
	m_pCamera = RwCameraCreate();

	m_iWidth	= iWidth;
	m_iHeigth	= iHeigth;

	// create camera
	RwCameraSetProjection(m_pCamera, rwPARALLEL);
	RwCameraSetNearClipPlane (m_pCamera, 0.001f);
	RwCameraSetFarClipPlane (m_pCamera, 10.f);

	RwCameraSetFrame(m_pCamera, RwFrameCreate());

	RwCameraSetRaster(m_pCamera, RwRasterCreate(m_iWidth, m_iHeigth, 0, rwRASTERTYPECAMERATEXTURE | rwRASTERFORMAT8888 ));
	m_pRenderTexture = RwTextureCreate(m_pCamera->frameBuffer);
	RwTextureSetFilterMode(m_pRenderTexture, rwFILTERLINEAR);

	RwReal fViewWindow = RwTan(NTL_DEG2RAD(45.f * 0.5f));
	RwV2d vw;

	vw.y = vw.x = fViewWindow;
	vw.y = ((float)iHeigth * fViewWindow) / (float)iWidth;

	RwCameraSetViewWindow(m_pCamera, &vw);

	NTL_RETURNVOID();
}

VOID CNtlPLCameraRT::Destroy()
{
	NTL_FUNCTION("CNtlPLCameraRT::Destroy");

	RwTextureDestroy(m_pRenderTexture);

	RwFrame* pFrame = RwCameraGetFrame(m_pCamera);
	if(pFrame)
	{
		RwCameraSetFrame(m_pCamera, NULL);
		RwFrameDestroy(pFrame);
	}

	RwCameraDestroy(m_pCamera);
	m_pCamera = NULL;

	NTL_RETURNVOID();
}

RwBool CNtlPLCameraRT::CameraBeginUpdate()
{
	RwRGBA camTexColor = {0, 0, 0, 0};
	if (RwCameraClear(m_pCamera, &camTexColor, rwCAMERACLEARIMAGE))
	{
		if( RwCameraBeginUpdate(m_pCamera) )
		{
			return true;
		}		
	}
	return false;
}

VOID CNtlPLCameraRT::CameraEndUpdate()
{
	NTL_ASSERT(m_pCamera == RwCameraGetCurrentCamera(), "Don't EndUpdate different camera");
	RwCameraEndUpdate(m_pCamera);
}

RwTexture* CNtlPLCameraRT::GetTexture()
{
	return m_pRenderTexture;
}