// SaberView1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "SaberView1.h"


// CSaberView1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSaberView1, CDialog)

CSaberView1::CSaberView1(CWnd* pParent /*=NULL*/)
	: CDialog(CSaberView1::IDD, pParent)
{

}

CSaberView1::~CSaberView1()
{
}

void CSaberView1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSaberView1, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSaberView1 메시지 처리기입니다.

/*
void CWorldViewDlg::CreateWorldViewResource()
{
	if (dGET_WORLD_PARAM()->WorldSizeHalf == 0 && m_pRpWorld)
	{
		return;
	}

	CRect rtView;
	GetDlgItem(IDC_VIEW)->GetClientRect(rtView);

	RwBBox BBox;
	BBox.inf.x = BBox.inf.y = BBox.inf.z = static_cast<RwReal>(-dGET_WORLD_PARAM()->WorldSizeHalf);
	BBox.sup.x = BBox.sup.y = BBox.sup.z = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf);

	if (m_pRpWorld = RpWorldCreate(&BBox))
	{
		if (m_pRwCamera	= RwCameraCreate())
		{
			RwCameraSetFrame(m_pRwCamera, RwFrameCreate());
			RwCameraSetRaster(m_pRwCamera, RwRasterCreate(rtView.Width(), rtView.Height(), 0, rwRASTERTYPECAMERA));
			RwCameraSetZRaster(m_pRwCamera, RwRasterCreate(rtView.Width(), rtView.Height(), 0, rwRASTERTYPEZBUFFER));

			RwCameraSetProjection(m_pRwCamera, rwPERSPECTIVE);

			RwCameraSetFarClipPlane(m_pRwCamera, static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize + 1));
			RwCameraSetNearClipPlane(m_pRwCamera, 0.001f);

			RwV2d vw;
			vw.x = 0.5f;//static_cast<RwReal>(rtView.Width()) * 0.5f / (static_cast<RwReal>(rtView.Width()) + static_cast<RwReal>(rtView.Height()));
			vw.y = 0.5f;//static_cast<RwReal>(rtView.Height()) * 0.5f / (static_cast<RwReal>(rtView.Width()) + static_cast<RwReal>(rtView.Height())); 
			RwCameraSetViewWindow(m_pRwCamera, &vw);

			RwD3D9CameraAttachWindow(m_pRwCamera, GetDlgItem(IDC_VIEW)->GetSafeHwnd());

			RpWorldAddCamera(m_pRpWorld, m_pRwCamera);
		}
		RpWorldAddLight(m_pRpWorld, RpLightCreate(rpLIGHTAMBIENT));

		// 3+--------------+1
		//  |			   |
		//  |			   |
		//  |			   |
		//  |			   |
		//  |              |
		// 2+--------------+0
		// 

		RwIm3DVertexSetPos(&m_aRwIm3DVertex[0], BBox.inf.x,  0.0f, BBox.inf.z);
		RwIm3DVertexSetRGBA(&m_aRwIm3DVertex[0], 255, 255, 255, 255);
		RwIm3DVertexSetU(&m_aRwIm3DVertex[0], 1.0f);
		RwIm3DVertexSetV(&m_aRwIm3DVertex[0], 1.0f);  
		RwIm3DVertexSetPos(&m_aRwIm3DVertex[1], BBox.inf.x, 0.0f, BBox.sup.z);
		RwIm3DVertexSetRGBA(&m_aRwIm3DVertex[1], 255, 255, 255, 255);
		RwIm3DVertexSetU(&m_aRwIm3DVertex[1], 1.0f);
		RwIm3DVertexSetV(&m_aRwIm3DVertex[1], 0.0f);  
		RwIm3DVertexSetPos(&m_aRwIm3DVertex[2], BBox.sup.x, 0.0f, BBox.inf.z);
		RwIm3DVertexSetRGBA(&m_aRwIm3DVertex[2], 255, 255, 255, 255);
		RwIm3DVertexSetU(&m_aRwIm3DVertex[2], 0.0f);
		RwIm3DVertexSetV(&m_aRwIm3DVertex[2], 1.0f);  
		RwIm3DVertexSetPos(&m_aRwIm3DVertex[3], BBox.sup.x, 0.0f, BBox.sup.z);
		RwIm3DVertexSetRGBA(&m_aRwIm3DVertex[3], 255, 255, 255, 255);
		RwIm3DVertexSetU(&m_aRwIm3DVertex[3], 0.0f);
		RwIm3DVertexSetV(&m_aRwIm3DVertex[3], 0.0f); 

		m_pRwTexWorldTile = CNtlPLResourceManager::GetInstance()->LoadTexture("Grid8x8", "Tool\\SABER\\Texture\\UI\\");
		RwTextureSetFilterMode(m_pRwTexWorldTile, rwFILTERLINEARMIPLINEAR);
	}

	OnSize(0, 512, 512 + dWORLD_VIEW_TOP_MARGIN);
}
void CWorldViewDlg::Render()
{
	if (IsWindowVisible() && m_pRpWorld && m_pRwCamera)
	{
		RwMatrixSetIdentity(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)));
		RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = 1.0f;
		RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = 0.0f;
		RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = 0.0f;
		RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = 0.0f;
		RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = 0.0f;
		RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = 1.0f;
		RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = 0.0f;
		RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = -1.0f;
		RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = 0.0f;
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)))->x;
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize);
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)))->z;
		RwMatrixUpdate(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)));
		RwFrameUpdateObjects(RwCameraGetFrame(m_pRwCamera));

		static RwRGBA rgbaBackGround = { 150, 150, 150, 255 };
		RwCameraClear(m_pRwCamera, &rgbaBackGround, rwCAMERACLEARZ | rwCAMERACLEARIMAGE);
		if (RwCameraBeginUpdate(m_pRwCamera))
		{
			RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
			RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
			RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
			//RwRenderStateSet(rwRENDERSTATECULLMODE, (void *)rwCULLMODECULLNONE);

			if (m_pRwTexWorldMap)
			{			
				RwD3D9SetTexture(m_pRwTexWorldMap, 0);
				if (RwIm3DTransform(m_aRwIm3DVertex, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV | rwIM3D_VERTEXRGBA))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
					RwIm3DEnd(); 
				}
			}

			D3DXMATRIXA16 matTemp;
			D3DXMatrixIdentity(&matTemp);
			matTemp._11 = 1.0f / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldNum);
			matTemp._32 = 1.0f / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldNum);
			matTemp._41 = 0.5f;
			matTemp._42 = 0.5f;

			RwD3D9SetTexture(m_pRwTexWorldTile, 0);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &matTemp);
			if (RwIm3DTransform(m_aRwIm3DVertex, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV | rwIM3D_VERTEXRGBA))
			{
				RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
				RwIm3DEnd(); 
			}

			RwD3D9SetTexture(NULL, 0);
			//RwRenderStateSet(rwRENDERSTATECULLMODE, (void *)rwCULLMODECULLBACK);
			RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
			RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
			RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);

			RwCameraEndUpdate(m_pRwCamera);
		}
		RwCameraShowRaster(m_pRwCamera, GetDlgItem(IDC_VIEW)->GetSafeHwnd(), NULL);
	}
}

void CWorldViewDlg::Render()
{
	if (IsWindowVisible() && m_pRpWorld && m_pRwCamera)
	{
		RwMatrixSetIdentity(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)));
		RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = 1.0f;
		RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = 0.0f;
		RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = 0.0f;
		RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = 0.0f;
		RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = 0.0f;
		RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = 1.0f;
		RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = 0.0f;
		RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = -1.0f;
		RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = 0.0f;
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)))->x;
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize);
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)))->z;
		RwMatrixUpdate(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)));
		RwFrameUpdateObjects(RwCameraGetFrame(m_pRwCamera));

		static RwRGBA rgbaBackGround = { 150, 150, 150, 255 };
		RwCameraClear(m_pRwCamera, &rgbaBackGround, rwCAMERACLEARZ | rwCAMERACLEARIMAGE);
		if (RwCameraBeginUpdate(m_pRwCamera))
		{
			RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
			RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
			RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
			//RwRenderStateSet(rwRENDERSTATECULLMODE, (void *)rwCULLMODECULLNONE);

			if (m_pRwTexWorldMap)
			{			
				RwD3D9SetTexture(m_pRwTexWorldMap, 0);
				if (RwIm3DTransform(m_aRwIm3DVertex, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV | rwIM3D_VERTEXRGBA))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
					RwIm3DEnd(); 
				}
			}

			D3DXMATRIXA16 matTemp;
			D3DXMatrixIdentity(&matTemp);
			matTemp._11 = 1.0f / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldNum);
			matTemp._32 = 1.0f / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldNum);
			matTemp._41 = 0.5f;
			matTemp._42 = 0.5f;

			RwD3D9SetTexture(m_pRwTexWorldTile, 0);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &matTemp);
			if (RwIm3DTransform(m_aRwIm3DVertex, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV | rwIM3D_VERTEXRGBA))
			{
				RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
				RwIm3DEnd(); 
			}

			RwD3D9SetTexture(NULL, 0);
			//RwRenderStateSet(rwRENDERSTATECULLMODE, (void *)rwCULLMODECULLBACK);
			RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
			RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
			RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);

			RwCameraEndUpdate(m_pRwCamera);
		}
		RwCameraShowRaster(m_pRwCamera, GetDlgItem(IDC_VIEW)->GetSafeHwnd(), NULL);
	}
}
*/
void CSaberView1::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	/*
	CRect rtView;
	GetClientRect(&rtView);

	if(::IsWindow(GetDlgItem(IDC_SABER_VIEW1_HIDE)->GetSafeHwnd()) && m_pRpWorld && m_pRwCamera)
	{
		GetDlgItem(IDC_SABER_VIEW1_HIDE)->SetWindowPos(NULL, 0, dWORLD_VIEW_TOP_MARGIN, rtView.Width(), rtView.Height() - dWORLD_VIEW_TOP_MARGIN, NULL);	
		GetDlgItem(IDC_SABER_VIEW1_HIDE)->GetClientRect(&rtView);

		RwRaster *pRwRaster		= RwCameraGetRaster(m_pRwCamera);
		RwRaster *pRwRasterZ	= RwCameraGetZRaster(m_pRwCamera);
		pRwRasterZ->width		= pRwRaster->width = rtView.Width();
		pRwRasterZ->height		= pRwRaster->height= rtView.Height();

		RwV2d vw;
		vw.x = 0.5f;//static_cast<RwReal>(rtView.Width()) * 0.5f / (static_cast<RwReal>(rtView.Width()) + static_cast<RwReal>(rtView.Height()));
		vw.y = 0.5f;//static_cast<RwReal>(rtView.Height()) * 0.5f / (static_cast<RwReal>(rtView.Width()) + static_cast<RwReal>(rtView.Height())); 
		RwCameraSetViewWindow(m_pRwCamera, &vw);
	}
	*/
}
