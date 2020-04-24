#include "stdafx.h"

#include "NtlWE.h"
#include "NtlWEFrm.h"
#include "NtlWEDoc.h"

#include "NtlWorldPaletteDefine.h"

#include "NtlPLRenderState.h"

#include "UpdateEventThumbnail.h"

CUpdateEventThumbnail::CUpdateEventThumbnail()
{
	m_pThumbnailTex	= NULL;
}

CUpdateEventThumbnail::~CUpdateEventThumbnail()
{
	RwTextureDestroy(m_pThumbnailTex);
}

void CUpdateEventThumbnail::CreateThumbnail()
{
	RwReal	recipCameraZ	= 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);
	RwInt32	nStartX			= 128;
	RwInt32 nStartY			= 0;
	RwInt32 width			= 128;
	RwInt32 height			= 128;

	RwIm2DVertexSetScreenX(&m_ThumbnailVert[0], nStartX-0.5f);
	RwIm2DVertexSetScreenY(&m_ThumbnailVert[0], nStartY-0.5f);
	RwIm2DVertexSetScreenZ(&m_ThumbnailVert[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_ThumbnailVert[0], recipCameraZ);
	RwIm2DVertexSetU(&m_ThumbnailVert[0], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_ThumbnailVert[0], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_ThumbnailVert[0], 255, 255, 255, 100); 

	RwIm2DVertexSetScreenX(&m_ThumbnailVert[1], nStartX-0.5f);
	RwIm2DVertexSetScreenY(&m_ThumbnailVert[1], nStartY+height-0.5f);
	RwIm2DVertexSetScreenZ(&m_ThumbnailVert[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_ThumbnailVert[1], recipCameraZ);
	RwIm2DVertexSetU(&m_ThumbnailVert[1], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_ThumbnailVert[1], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_ThumbnailVert[1], 255, 255, 255, 100); 

	RwIm2DVertexSetScreenX(&m_ThumbnailVert[2], nStartX+width-0.5f);
	RwIm2DVertexSetScreenY(&m_ThumbnailVert[2], nStartY-0.5f);
	RwIm2DVertexSetScreenZ(&m_ThumbnailVert[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_ThumbnailVert[2], recipCameraZ);
	RwIm2DVertexSetU(&m_ThumbnailVert[2], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_ThumbnailVert[2], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_ThumbnailVert[2], 255, 255, 255, 100); 

	RwIm2DVertexSetScreenX(&m_ThumbnailVert[3], nStartX+width-0.5f);
	RwIm2DVertexSetScreenY(&m_ThumbnailVert[3], nStartY+height-0.5f);
	RwIm2DVertexSetScreenZ(&m_ThumbnailVert[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_ThumbnailVert[3], recipCameraZ);
	RwIm2DVertexSetU(&m_ThumbnailVert[3], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_ThumbnailVert[3], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_ThumbnailVert[3], 255, 255, 255, 100);

	LoadThumbnailTex("not registered");
}

void CUpdateEventThumbnail::RenderThumbnail()
{
	if(dGET_CUR_PALETTE() == ePM_TEXTURE)
	{
		if(m_pThumbnailTex)
		{
			UpdateAlphaState(TRUE);

			RwD3D9SetTexture(m_pThumbnailTex, 0);
			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_ThumbnailVert, 4);

			UpdateAlphaState(FALSE);
		}
	}
}

void CUpdateEventThumbnail::RestoreThumbnail()
{
	RwReal	recipCameraZ	= 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);
	RwInt32	nStartX			= 128;
	RwInt32 nStartY			= 0;
	RwInt32 width			= 128;
	RwInt32 height			= 128;

	RwIm2DVertexSetScreenX(&m_ThumbnailVert[0], nStartX-0.5f);
	RwIm2DVertexSetScreenY(&m_ThumbnailVert[0], nStartY-0.5f);
	RwIm2DVertexSetScreenZ(&m_ThumbnailVert[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_ThumbnailVert[0], recipCameraZ);
	RwIm2DVertexSetU(&m_ThumbnailVert[0], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_ThumbnailVert[0], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_ThumbnailVert[0], 255, 255, 255, 100); 

	RwIm2DVertexSetScreenX(&m_ThumbnailVert[1], nStartX-0.5f);
	RwIm2DVertexSetScreenY(&m_ThumbnailVert[1], nStartY+height-0.5f);
	RwIm2DVertexSetScreenZ(&m_ThumbnailVert[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_ThumbnailVert[1], recipCameraZ);
	RwIm2DVertexSetU(&m_ThumbnailVert[1], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_ThumbnailVert[1], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_ThumbnailVert[1], 255, 255, 255, 100); 

	RwIm2DVertexSetScreenX(&m_ThumbnailVert[2], nStartX+width-0.5f);
	RwIm2DVertexSetScreenY(&m_ThumbnailVert[2], nStartY-0.5f);
	RwIm2DVertexSetScreenZ(&m_ThumbnailVert[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_ThumbnailVert[2], recipCameraZ);
	RwIm2DVertexSetU(&m_ThumbnailVert[2], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_ThumbnailVert[2], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_ThumbnailVert[2], 255, 255, 255, 100); 

	RwIm2DVertexSetScreenX(&m_ThumbnailVert[3], nStartX+width-0.5f);
	RwIm2DVertexSetScreenY(&m_ThumbnailVert[3], nStartY+height-0.5f);
	RwIm2DVertexSetScreenZ(&m_ThumbnailVert[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_ThumbnailVert[3], recipCameraZ);
	RwIm2DVertexSetU(&m_ThumbnailVert[3], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_ThumbnailVert[3], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_ThumbnailVert[3], 255, 255, 255, 100);
}

void CUpdateEventThumbnail::LoadThumbnailTex(const char* pName)
{
	if(!::strcmp(pName, "not registered"))
	{
		m_pThumbnailTex = CNtlPLResourceManager::GetInstance()->LoadTexture("NTL", dGET_WORLD_PARAM()->CurDefaultTexPath);
		DBO_ASSERT(m_pThumbnailTex, "Texture load failed.");
		return;
	}

	if(m_pThumbnailTex)
	{
		RwTextureDestroy(m_pThumbnailTex);
	}

	m_pThumbnailTex = CNtlPLResourceManager::GetInstance()->LoadTexture(pName, dGET_WORLD_PARAM()->CurDefaultTexPath); 
	DBO_ASSERT(m_pThumbnailTex, "Texture load failed.");
}