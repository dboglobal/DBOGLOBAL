#include "gui_precomp.h"
#include "gui_renderer.h"
#include "gui_texture.h"
#include "gui_surface.h"
#include "gui_define.h"
#include "NtlProfiler.h"


gui::CGuiRenderer g_GuiRenderer;

START_GUI

#define MAX_GUI_RENDER_BUFFER		256
#define MAX_GUI_RENDER_TOP_BUFFER	64
#define GUI_RENDER_GROW_SIZE		128		
#define GUI_RENDER_TOP_GROW_SIZE	32


CGuiRenderer::CGuiRenderer()
{
	m_nBufferNum = MAX_GUI_RENDER_BUFFER;
	m_nRenderNum = 0;
	m_nTopBufferNum = MAX_GUI_RENDER_TOP_BUFFER;
	m_nRenderTopNum = 0;

	m_pRenderBuffer = NTL_NEW SRenderBuffer [m_nBufferNum];
	m_pRenderTopBuffer = NTL_NEW SRenderBuffer [m_nTopBufferNum];
}

CGuiRenderer::~CGuiRenderer()
{
	NTL_ARRAY_DELETE( m_pRenderBuffer );
	NTL_ARRAY_DELETE( m_pRenderTopBuffer );
}

VOID CGuiRenderer::ResetRender(VOID)
{
	m_nRenderNum = 0;
	m_nRenderTopNum = 0;
}


VOID CGuiRenderer::BufferGrow(VOID)
{
	int nTempBufferNum = m_nBufferNum+GUI_RENDER_GROW_SIZE;
	SRenderBuffer *pTempBuffer = NTL_NEW SRenderBuffer [nTempBufferNum];
    memcpy(pTempBuffer, m_pRenderBuffer, sizeof(SRenderBuffer)*m_nBufferNum);
	m_nBufferNum = nTempBufferNum;
	NTL_ARRAY_DELETE( m_pRenderBuffer );
	m_pRenderBuffer = pTempBuffer;
}

VOID CGuiRenderer::BufferTopGrow(VOID)
{
	int nTempBufferNum = m_nTopBufferNum+GUI_RENDER_TOP_GROW_SIZE;
	SRenderBuffer *pTempBuffer = NTL_NEW SRenderBuffer [nTempBufferNum];
    memcpy(pTempBuffer, m_pRenderTopBuffer, sizeof(SRenderBuffer)*m_nRenderTopNum);
	m_nTopBufferNum = nTempBufferNum;
	NTL_ARRAY_DELETE( m_pRenderTopBuffer );
	m_pRenderTopBuffer = pTempBuffer;
}

VOID CGuiRenderer::RenderBuffer(const PlaneSnapShot *pSnapShot, CTexture *pTexture, FLOAT fZRot)
{
	NTL_SPROFILE("CGuiRenderer::RenderBuffer")

#define GUI_UV_START	0.01f
#define GUI_UV_END		0.99f

	RwReal fOffset = -0.5f;
		
	if( fZRot == 0.0f )
	{
		RwIm2DVertexSetScreenX(&m_2dVertices[0], (RwReal)pSnapShot->rtRect.left + fOffset);
		RwIm2DVertexSetScreenY(&m_2dVertices[0], (RwReal)pSnapShot->rtRect.top + fOffset);

		RwIm2DVertexSetScreenX(&m_2dVertices[1], (RwReal)pSnapShot->rtRect.left + fOffset);
		RwIm2DVertexSetScreenY(&m_2dVertices[1], (RwReal)pSnapShot->rtRect.bottom + fOffset);

		RwIm2DVertexSetScreenX(&m_2dVertices[2], (RwReal)pSnapShot->rtRect.right + fOffset);
		RwIm2DVertexSetScreenY(&m_2dVertices[2], (RwReal)pSnapShot->rtRect.top + fOffset);

		RwIm2DVertexSetScreenX(&m_2dVertices[3], (RwReal)pSnapShot->rtRect.right + fOffset);
		RwIm2DVertexSetScreenY(&m_2dVertices[3], (RwReal)pSnapShot->rtRect.bottom + fOffset);
	}
	else
	{
		FLOAT fRadian = GUI_DEG2RAD( fZRot );
		FLOAT fDelta[2];
		FLOAT fPoint[4];
		FLOAT fVertex[8];
		FLOAT fSinTheta = sin( fRadian );
		FLOAT fCosTheta = cos( fRadian );

		fPoint[0] = (FLOAT)pSnapShot->rtRect.left;
		fPoint[1] = (FLOAT)pSnapShot->rtRect.top;
		fPoint[2] = (FLOAT)pSnapShot->rtRect.right;
		fPoint[3] = (FLOAT)pSnapShot->rtRect.bottom;

		fDelta[0] = fPoint[0] + ( fPoint[2] - fPoint[0] ) / 2; // x축
		fDelta[1] = fPoint[1] + ( fPoint[3] - fPoint[1] ) / 2;  // y축

		fPoint[0] -= fDelta[0];
		fPoint[1] -= fDelta[1];
		fPoint[2] -= fDelta[0];
		fPoint[3] -= fDelta[1];

		fVertex[0] = fPoint[0] * fCosTheta + fPoint[1] * fSinTheta + fDelta[0];
		fVertex[1] = fPoint[1] * fCosTheta - fPoint[0] * fSinTheta + fDelta[1];
		fVertex[2] = fPoint[0] * fCosTheta + fPoint[3] * fSinTheta + fDelta[0];
		fVertex[3] = fPoint[3] * fCosTheta - fPoint[0] * fSinTheta + fDelta[1];
		fVertex[4] = fPoint[2] * fCosTheta + fPoint[1] * fSinTheta + fDelta[0];
		fVertex[5] = fPoint[1] * fCosTheta - fPoint[2] * fSinTheta + fDelta[1];
		fVertex[6] = fPoint[2] * fCosTheta + fPoint[3] * fSinTheta + fDelta[0];
		fVertex[7] = fPoint[3] * fCosTheta - fPoint[2] * fSinTheta + fDelta[1];

		// 회전
		RwIm2DVertexSetScreenX(&m_2dVertices[0], fVertex[0] + fOffset);
		RwIm2DVertexSetScreenY(&m_2dVertices[0], fVertex[1] + fOffset);

		RwIm2DVertexSetScreenX(&m_2dVertices[1], fVertex[2] + fOffset);
		RwIm2DVertexSetScreenY(&m_2dVertices[1], fVertex[3] + fOffset);

		RwIm2DVertexSetScreenX(&m_2dVertices[2], fVertex[4] + fOffset);
		RwIm2DVertexSetScreenY(&m_2dVertices[2], fVertex[5] + fOffset);

		RwIm2DVertexSetScreenX(&m_2dVertices[3], fVertex[6] + fOffset);
		RwIm2DVertexSetScreenY(&m_2dVertices[3], fVertex[7] + fOffset);		
	}

	RwIm2DVertexSetScreenZ(&m_2dVertices[0], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_2dVertices[0], 1.0f);
	RwIm2DVertexSetU (&m_2dVertices[0], pSnapShot->UVs[0], GUI_UV_END);
	RwIm2DVertexSetV (&m_2dVertices[0], pSnapShot->UVs[1], GUI_UV_END);

	RwIm2DVertexSetScreenZ(&m_2dVertices[1], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_2dVertices[1], 1.0f);
	RwIm2DVertexSetU (&m_2dVertices[1], pSnapShot->UVs[0], GUI_UV_END);
	RwIm2DVertexSetV (&m_2dVertices[1], pSnapShot->UVs[3], GUI_UV_END);

	RwIm2DVertexSetScreenZ(&m_2dVertices[2], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_2dVertices[2], 1.0f);
	RwIm2DVertexSetU (&m_2dVertices[2], pSnapShot->UVs[2], GUI_UV_END);
	RwIm2DVertexSetV (&m_2dVertices[2], pSnapShot->UVs[1], GUI_UV_END);

	RwIm2DVertexSetScreenZ(&m_2dVertices[3], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_2dVertices[3], 1.0f);
	RwIm2DVertexSetU (&m_2dVertices[3], pSnapShot->UVs[2], GUI_UV_END);
	RwIm2DVertexSetV (&m_2dVertices[3], pSnapShot->UVs[3], GUI_UV_END);

	for(RwInt32 i = 0; i < 4; i++)
	{
		RwIm2DVertexSetIntRGBA(&m_2dVertices[i], pSnapShot->uRed, pSnapShot->uGreen, pSnapShot->uBlue, pSnapShot->uAlpha);
	}
	
	if(pTexture)
	{
		RwD3D9SetTexture(pTexture->GetTexture(), 0);
	}
	else
	{
		RwD3D9SetTexture(NULL, 0);
	}

	RwIm2DRenderPrimitiveMacro(rwPRIMTYPETRISTRIP, m_2dVertices, 4);

	NTL_EPROFILE()
}

VOID CGuiRenderer::RenderBuffer( const TriSnapShot* pSnapShot, CTexture* pTexture, CRectangle* pClippingRect /* = NULL  */ ) 
{
	NTL_SPROFILE("CGuiRenderer::RenderBuffer")

	RwReal fOffset = -0.5f;

	RwIm2DVertexSetScreenX(&m_2dVertices[0], (RwReal)pSnapShot->ptPos[0].x + fOffset);
	RwIm2DVertexSetScreenY(&m_2dVertices[0], (RwReal)pSnapShot->ptPos[0].y + fOffset);

	RwIm2DVertexSetScreenX(&m_2dVertices[1], (RwReal)pSnapShot->ptPos[1].x + fOffset);
	RwIm2DVertexSetScreenY(&m_2dVertices[1], (RwReal)pSnapShot->ptPos[1].y + fOffset);

	RwIm2DVertexSetScreenX(&m_2dVertices[2], (RwReal)pSnapShot->ptPos[2].x + fOffset);
	RwIm2DVertexSetScreenY(&m_2dVertices[2], (RwReal)pSnapShot->ptPos[2].y + fOffset);

	RwIm2DVertexSetScreenZ(&m_2dVertices[0], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_2dVertices[0], 1.0f);
	RwIm2DVertexSetU (&m_2dVertices[0], pSnapShot->UVs[0], GUI_UV_END);
	RwIm2DVertexSetV (&m_2dVertices[0], pSnapShot->UVs[1], GUI_UV_END);

	RwIm2DVertexSetScreenZ(&m_2dVertices[1], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_2dVertices[1], 1.0f);
	RwIm2DVertexSetU (&m_2dVertices[1], pSnapShot->UVs[2], GUI_UV_END);
	RwIm2DVertexSetV (&m_2dVertices[1], pSnapShot->UVs[3], GUI_UV_END);

	RwIm2DVertexSetScreenZ(&m_2dVertices[2], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_2dVertices[2], 1.0f);
	RwIm2DVertexSetU (&m_2dVertices[2], pSnapShot->UVs[4], GUI_UV_END);
	RwIm2DVertexSetV (&m_2dVertices[2], pSnapShot->UVs[5], GUI_UV_END);

	for(RwInt32 i = 0; i < 3; i++)
	{
		RwIm2DVertexSetIntRGBA(&m_2dVertices[i], pSnapShot->uRed, pSnapShot->uGreen, pSnapShot->uBlue, pSnapShot->uAlpha);
	}

	if(pTexture)
	{
		RwD3D9SetTexture(pTexture->GetTexture(), 0);
	}
	else
	{
		RwD3D9SetTexture(NULL, 0);
	}

	if( pClippingRect )
	{
		LPDIRECT3DDEVICE9 pD3DDev = static_cast<LPDIRECT3DDEVICE9>( RwD3D9GetCurrentD3DDevice() );
		pD3DDev->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
		pD3DDev->SetScissorRect( pClippingRect );
		RwIm2DRenderPrimitiveMacro(rwPRIMTYPETRILIST, m_2dVertices, 3);
		pD3DDev->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
	}
	else
	{
		RwIm2DRenderPrimitiveMacro(rwPRIMTYPETRILIST, m_2dVertices, 3);
	}

	NTL_EPROFILE()
}

VOID CGuiRenderer::RenderQueue(const PlaneSnapShot *pSnapShot, CTexture *pTexture, bool bRenderTop /* = false */, FLOAT fZRot /* = 0.0f */, CRectangle* pClippingRect /* = NULL */)
{
	NTL_SPROFILE("CGuiRenderer::RenderQueue")

	// Clipping 영역 계산.
	PlaneSnapShot planeClipped = *pSnapShot;

	if( pClippingRect )
	{
		if( pSnapShot->rtRect.left < pClippingRect->left )
		{
			planeClipped.rtRect.left = pClippingRect->left;
			planeClipped.UVs[0] += ( pSnapShot->UVs[2] - pSnapShot->UVs[0] ) * ( pClippingRect->left - pSnapShot->rtRect.left ) / pSnapShot->rtRect.GetWidth();			
		}

		if( pSnapShot->rtRect.top < pClippingRect->top )
		{
			planeClipped.rtRect.top = pClippingRect->top;
			planeClipped.UVs[1] += ( pSnapShot->UVs[3] - pSnapShot->UVs[1] ) * ( pClippingRect->top - pSnapShot->rtRect.top ) / pSnapShot->rtRect.GetHeight();
		}

		if( pSnapShot->rtRect.right > pClippingRect->right )
		{
			planeClipped.rtRect.right = pClippingRect->right;
			planeClipped.UVs[2] -= ( pSnapShot->UVs[2] - pSnapShot->UVs[0] ) * ( pSnapShot->rtRect.right - pClippingRect->right ) / pSnapShot->rtRect.GetWidth();			
		}

		if( pSnapShot->rtRect.bottom > pClippingRect->bottom )
		{
			planeClipped.rtRect.bottom = pClippingRect->bottom;
			planeClipped.UVs[3] -= ( pSnapShot->UVs[3] - pSnapShot->UVs[1] ) * ( pSnapShot->rtRect.bottom - pClippingRect->bottom ) / pSnapShot->rtRect.GetHeight();			
		}

		if( planeClipped.rtRect.right - planeClipped.rtRect.left <= 0 || planeClipped.rtRect.bottom - planeClipped.rtRect.top <= 0 )
			return;
	}	
	// Clipping 영역 계산 끝.

	if(bRenderTop)
	{
		if(m_nRenderTopNum + 1 >= m_nTopBufferNum)
			BufferTopGrow();

		memcpy(&m_pRenderTopBuffer[m_nRenderTopNum].SnapShot, &planeClipped, sizeof(PlaneSnapShot));
		m_pRenderTopBuffer[m_nRenderTopNum].pTexture = pTexture;
		m_pRenderTopBuffer[m_nRenderTopNum].fZRot = fZRot;
		m_nRenderTopNum++;
	}
	else
	{
		RenderBuffer(&planeClipped, pTexture, fZRot);
	//	if(m_nRenderNum + 1 >= m_nBufferNum)
	//		BufferGrow();

//		memcpy(&m_pRenderBuffer[m_nRenderNum].SnapShot, pSnapShot, sizeof(PlaneSnapShot));
//		m_pRenderBuffer[m_nRenderNum].pTexture = pTexture;
//		m_nRenderNum++;
	}

	NTL_EPROFILE()
}

VOID CGuiRenderer::Render(VOID)
{
	NTL_SPROFILE("CGuiRenderer::Render")

	int i = 0;

	for(i = 0; i < m_nRenderNum; i++)
	{
		RenderBuffer(&m_pRenderBuffer[i].SnapShot, m_pRenderBuffer[i].pTexture, m_pRenderBuffer[i].fZRot);  
	}

	for(i = 0; i < m_nRenderTopNum; i++)
	{
		RenderBuffer(&m_pRenderTopBuffer[i].SnapShot, m_pRenderTopBuffer[i].pTexture, m_pRenderTopBuffer[i].fZRot);  
	}

	NTL_EPROFILE()
}

END_GUI
