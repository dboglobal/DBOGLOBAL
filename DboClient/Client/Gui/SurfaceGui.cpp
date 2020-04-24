#include "precomp_dboclient.h"
#include "SurfaceGui.h"
#include "gui_guimanager.h"
#include "gui_texturemanager.h"
#include "gui_texture.h"
#include "gui_renderer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSurfaceGui

CSurfaceGui::CSurfaceGui()
{
	m_bCreateTexture = FALSE;
	m_bShow = TRUE;
	m_Pos_from_Parent.x = 0;
	m_Pos_from_Parent.y = 0;
	m_bClipping = FALSE;
}

CSurfaceGui::~CSurfaceGui()
{
	UnsetTexture();
}

void CSurfaceGui::SetRect(RwInt32 iLeft, RwInt32 iTop, RwInt32 iRight, RwInt32 iBottom)
{
	m_surPaint.m_SnapShot.rtRect.left	= iLeft;
	m_surPaint.m_SnapShot.rtRect.top	= iTop;
	m_surPaint.m_SnapShot.rtRect.right	= iRight;
	m_surPaint.m_SnapShot.rtRect.bottom	= iBottom;
}

void CSurfaceGui::SetRect(CRectangle& rect)
{
	m_surPaint.m_SnapShot.rtRect.left	= rect.left;
	m_surPaint.m_SnapShot.rtRect.top	= rect.top;
	m_surPaint.m_SnapShot.rtRect.right	= rect.right;
	m_surPaint.m_SnapShot.rtRect.bottom	= rect.bottom;
}
void CSurfaceGui::SetRectWH(RwInt32 iLeft, RwInt32 iTop, RwInt32 iWidth, RwInt32 iHeight)
{
	m_surPaint.m_SnapShot.rtRect.left	= iLeft;
	m_surPaint.m_SnapShot.rtRect.top	= iTop;
	m_surPaint.m_SnapShot.rtRect.right	= iLeft + iWidth;
	m_surPaint.m_SnapShot.rtRect.bottom = iTop + iHeight;
}

void CSurfaceGui::GetRect(CRectangle& rect)
{
	rect = m_surPaint.m_SnapShot.rtRect;
}

CPos CSurfaceGui::GetPosfromParent()
{
	return m_Pos_from_Parent;
}

RwInt32 CSurfaceGui::GetWidth()
{
	return m_surPaint.m_SnapShot.rtRect.right - m_surPaint.m_SnapShot.rtRect.left;
}

RwInt32 CSurfaceGui::GetHeight()
{
	return m_surPaint.m_SnapShot.rtRect.bottom - m_surPaint.m_SnapShot.rtRect.top;
}

RwBool CSurfaceGui::PtInRect(int iX, int iY)
{
	if( m_surPaint.m_SnapShot.rtRect.left <= iX && iX <= m_surPaint.m_SnapShot.rtRect.right &&
		m_surPaint.m_SnapShot.rtRect.top <= iY && iY <= m_surPaint.m_SnapShot.rtRect.bottom )
		return TRUE;

	return FALSE;
}

void CSurfaceGui::SetSize(RwInt32 iWidth, RwInt32 iHeight)
{
	m_surPaint.m_SnapShot.rtRect.right = m_surPaint.m_SnapShot.rtRect.left + iWidth;
	m_surPaint.m_SnapShot.rtRect.bottom = m_surPaint.m_SnapShot.rtRect.top + iHeight;
}

void CSurfaceGui::SetPosition(RwInt32 iPosX, RwInt32 iPosY)
{
	RwInt32 iWidth = m_surPaint.m_SnapShot.rtRect.GetWidth();
	RwInt32 iHeight = m_surPaint.m_SnapShot.rtRect.GetHeight();

	m_surPaint.m_SnapShot.rtRect.left = m_surPaint.m_Original.rtRect.left + iPosX;
	m_surPaint.m_SnapShot.rtRect.top = m_surPaint.m_Original.rtRect.top + iPosY;
	m_surPaint.m_SnapShot.rtRect.right = m_surPaint.m_SnapShot.rtRect.left + iWidth;
	m_surPaint.m_SnapShot.rtRect.bottom = m_surPaint.m_SnapShot.rtRect.top + iHeight;
}

void CSurfaceGui::SetCenterPosition(RwInt32 iPosX, RwInt32 iPosY)
{
	RwInt32 iWidth = m_surPaint.m_SnapShot.rtRect.GetWidth();
	RwInt32 iHeight = m_surPaint.m_SnapShot.rtRect.GetHeight();

	m_surPaint.m_SnapShot.rtRect.left = m_surPaint.m_Original.rtRect.left + iPosX - iWidth/2;
	m_surPaint.m_SnapShot.rtRect.top = m_surPaint.m_Original.rtRect.top + iPosY - iHeight/2;
	m_surPaint.m_SnapShot.rtRect.right = m_surPaint.m_SnapShot.rtRect.left + iWidth;
	m_surPaint.m_SnapShot.rtRect.bottom = m_surPaint.m_SnapShot.rtRect.top + iHeight;
}

void CSurfaceGui::SetPositionfromParent(RwInt32 iPosX_from_Parent, RwInt32 iPosY_from_Parent)
{
	m_Pos_from_Parent.x = iPosX_from_Parent;
	m_Pos_from_Parent.y = iPosY_from_Parent;
}

void CSurfaceGui::SetPositionbyParent(RwInt32 iParentPosX, RwInt32 iParentPosY)
{
	RwInt32 iWidth = m_surPaint.m_SnapShot.rtRect.GetWidth();
	RwInt32 iHeight = m_surPaint.m_SnapShot.rtRect.GetHeight();

	SetRectWH(m_Pos_from_Parent.x + iParentPosX, m_Pos_from_Parent.y + iParentPosY, iWidth, iHeight);
}

void CSurfaceGui::SetClippingRect( CRectangle& rtClipping )
{
	m_rtClipping = rtClipping;
}

void CSurfaceGui::SetClippingMode( RwBool bClipping )
{
	m_bClipping = bClipping;
}

void CSurfaceGui::SetWidth(RwInt32 iWidth)
{
	m_surPaint.m_SnapShot.rtRect.right = m_surPaint.m_SnapShot.rtRect.left + iWidth;
}

void CSurfaceGui::SetHeight(RwInt32 iHeight)
{
	m_surPaint.m_SnapShot.rtRect.bottom = m_surPaint.m_SnapShot.rtRect.top + iHeight;
}

void CSurfaceGui::SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byAlpha)
{
	m_surPaint.m_SnapShot.uRed = byRed;
	m_surPaint.m_SnapShot.uGreen = byGreen;
	m_surPaint.m_SnapShot.uBlue = byBlue;
	m_surPaint.m_SnapShot.uAlpha = byAlpha;
}

void CSurfaceGui::SetColorOnly(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_surPaint.m_SnapShot.uRed = byRed;
	m_surPaint.m_SnapShot.uGreen = byGreen;
	m_surPaint.m_SnapShot.uBlue = byBlue;
}

void CSurfaceGui::SetAlpha(RwUInt8 byAlpha)
{
	m_surPaint.m_SnapShot.uAlpha = byAlpha;
}

RwBool CSurfaceGui::SetTexture(const RwChar *pName)
{
	m_bCreateTexture = TRUE;

	if(m_surPaint.m_pTexture)
	{
		g_TextureMgr.DeleteTexture(m_surPaint.m_pTexture);
		m_surPaint.m_pTexture = NULL;
	}

	m_surPaint.m_pTexture = g_TextureMgr.CreateTexture(pName);

	if( m_surPaint.m_pTexture == NULL )
		return FALSE;

	return TRUE;
}

void CSurfaceGui::SetZRot(RwReal fTheta)
{
	m_surPaint.m_fAngle = fTheta;
}

RwReal CSurfaceGui::GetZRot()
{
	return m_surPaint.m_fAngle;
}

void CSurfaceGui::SetTexture(const gui::CTexture *pTexture,RwBool bCreated /* = FALSE */)
{
	m_surPaint.m_pTexture = (gui::CTexture*)pTexture;
	m_bCreateTexture = bCreated;
}

void CSurfaceGui::UnsetTexture(VOID)
{
	if( m_bCreateTexture )
	{
		g_TextureMgr.DeleteTexture( m_surPaint.m_pTexture );
		m_bCreateTexture = FALSE;
	}

	m_surPaint.m_pTexture = NULL;
}

void CSurfaceGui::SetSurface(gui::CSurface& surface)
{
	m_surPaint = surface;
}

void CSurfaceGui::Render(bool bRenderTop /*= false*/)
{
	CRectangle* pClippingRect = NULL;
	if( m_bClipping )
		pClippingRect = &m_rtClipping;

	if( m_bShow )
		g_GuiRenderer.RenderQueue(&m_surPaint.m_SnapShot, m_surPaint.m_pTexture, bRenderTop, m_surPaint.m_fAngle, pClippingRect );
}

RwUInt8 CSurfaceGui::GetAlpha()
{
	return m_surPaint.m_SnapShot.uAlpha;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFilteredSurfaceGui

CFilteredSurfaceGui::CFilteredSurfaceGui(VOID) : m_ptexFilter( NULL )
{

}

CFilteredSurfaceGui::~CFilteredSurfaceGui(VOID)
{
	UnsetFilterTexture();
}

void CFilteredSurfaceGui::SetFilterTexture( const gui::CTexture* ptexFilter )
{
	m_ptexFilter = const_cast<gui::CTexture*>( ptexFilter );
}

void CFilteredSurfaceGui::UnsetFilterTexture(VOID)
{
	m_ptexFilter = NULL;
}

VOID CFilteredSurfaceGui::Render( bool bRenderTop /* = false */ )
{
#define GUI_UV_START	0.01f
#define GUI_UV_END		0.99f

	if( !m_bShow )
		return;

	PlaneSnapShot*	pSnapShot = &m_surPaint.m_SnapShot;
	//PlaneSnapShot*  pFilterShot = &m_surPaint.m_SnapShot;
	gui::CTexture*	pTexture = m_surPaint.m_pTexture;
	gui::CTexture*	pFilterTexture = m_ptexFilter;

	RwReal fOffset = -0.5f;

	RwIm2DVertexSetScreenX(&m_2dVertices[0], (RwReal)pSnapShot->rtRect.left + fOffset);
	RwIm2DVertexSetScreenY(&m_2dVertices[0], (RwReal)pSnapShot->rtRect.top + fOffset);

	RwIm2DVertexSetScreenX(&m_2dVertices[1], (RwReal)pSnapShot->rtRect.left + fOffset);
	RwIm2DVertexSetScreenY(&m_2dVertices[1], (RwReal)pSnapShot->rtRect.bottom + fOffset);

	RwIm2DVertexSetScreenX(&m_2dVertices[2], (RwReal)pSnapShot->rtRect.right + fOffset);
	RwIm2DVertexSetScreenY(&m_2dVertices[2], (RwReal)pSnapShot->rtRect.top + fOffset);

	RwIm2DVertexSetScreenX(&m_2dVertices[3], (RwReal)pSnapShot->rtRect.right + fOffset);
	RwIm2DVertexSetScreenY(&m_2dVertices[3], (RwReal)pSnapShot->rtRect.bottom + fOffset);

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

	if( pTexture && pFilterTexture )
	{
		BeginFilterRenderState();

		RwD3D9SetTexture(pTexture->GetTexture(), 0);
		RwD3D9SetTexture(pFilterTexture->GetTexture(), 1);

		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_2dVertices, 4);

		EndFilterRenderState();
	}
	else if( pTexture )
	{
		RwD3D9SetTexture(pTexture->GetTexture(), 0);

		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_2dVertices, 4);
	}
	else
	{
		RwD3D9SetTexture(NULL, 0);

		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_2dVertices, 4);
	}	
}

VOID CFilteredSurfaceGui::BeginFilterRenderState(VOID)
{
	// [CodeInDeviceState : Setting] 
	RwD3D9SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	RwD3D9SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 ); 
	RwD3D9SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ); 
	RwD3D9SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE ); 
	

	RwD3D9SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );
	RwD3D9SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_SELECTARG1 ); 
	RwD3D9SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT ); 

	RwD3D9SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_MODULATE ); 
	RwD3D9SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_CURRENT );
	RwD3D9SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
}

VOID CFilteredSurfaceGui::EndFilterRenderState(VOID)
{
	// [CodeInTextrueStageState : Restore]
	RwD3D9SetTexture(NULL, 1);

	// stage 0
	RwD3D9SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
	RwD3D9SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_CURRENT); 
	
	RwD3D9SetTextureStageState (0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); 
	RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG2, D3DTA_CURRENT); 

	// stage 1
	RwD3D9SetTextureStageState (1, D3DTSS_TEXCOORDINDEX, 1);
	RwD3D9SetTextureStageState (1, D3DTSS_COLOROP, D3DTOP_DISABLE); 
	RwD3D9SetTextureStageState (1, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState (1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
	
	RwD3D9SetTextureStageState (1, D3DTSS_ALPHAOP, D3DTOP_DISABLE); 
	RwD3D9SetTextureStageState (1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState (1, D3DTSS_ALPHAARG2, D3DTA_CURRENT); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCameraRenderTargetSurface
VOID CCameraRenderTargetSurface::Render( bool bRenderTop /* = false */ )
{
#define GUI_UV_START	0.01f
#define GUI_UV_END		0.99f

	RwReal fOffset = -0.5f;
	
	RwIm2DVertexSetScreenX(&m_2dVertices[0], (RwReal)m_surPaint.m_SnapShot.rtRect.left + fOffset);
	RwIm2DVertexSetScreenY(&m_2dVertices[0], (RwReal)m_surPaint.m_SnapShot.rtRect.top + fOffset);

	RwIm2DVertexSetScreenX(&m_2dVertices[1], (RwReal)m_surPaint.m_SnapShot.rtRect.left + fOffset);
	RwIm2DVertexSetScreenY(&m_2dVertices[1], (RwReal)m_surPaint.m_SnapShot.rtRect.bottom + fOffset);

	RwIm2DVertexSetScreenX(&m_2dVertices[2], (RwReal)m_surPaint.m_SnapShot.rtRect.right + fOffset);
	RwIm2DVertexSetScreenY(&m_2dVertices[2], (RwReal)m_surPaint.m_SnapShot.rtRect.top + fOffset);

	RwIm2DVertexSetScreenX(&m_2dVertices[3], (RwReal)m_surPaint.m_SnapShot.rtRect.right + fOffset);
	RwIm2DVertexSetScreenY(&m_2dVertices[3], (RwReal)m_surPaint.m_SnapShot.rtRect.bottom + fOffset);

	RwIm2DVertexSetScreenZ(&m_2dVertices[0], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_2dVertices[0], 1.0f);
	RwIm2DVertexSetU (&m_2dVertices[0], m_surPaint.m_SnapShot.UVs[0], GUI_UV_END);
	RwIm2DVertexSetV (&m_2dVertices[0], m_surPaint.m_SnapShot.UVs[1], GUI_UV_END);

	RwIm2DVertexSetScreenZ(&m_2dVertices[1], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_2dVertices[1], 1.0f);
	RwIm2DVertexSetU (&m_2dVertices[1], m_surPaint.m_SnapShot.UVs[0], GUI_UV_END);
	RwIm2DVertexSetV (&m_2dVertices[1], m_surPaint.m_SnapShot.UVs[3], GUI_UV_END);

	RwIm2DVertexSetScreenZ(&m_2dVertices[2], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_2dVertices[2], 1.0f);
	RwIm2DVertexSetU (&m_2dVertices[2], m_surPaint.m_SnapShot.UVs[2], GUI_UV_END);
	RwIm2DVertexSetV (&m_2dVertices[2], m_surPaint.m_SnapShot.UVs[1], GUI_UV_END);

	RwIm2DVertexSetScreenZ(&m_2dVertices[3], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_2dVertices[3], 1.0f);
	RwIm2DVertexSetU (&m_2dVertices[3], m_surPaint.m_SnapShot.UVs[2], GUI_UV_END);
	RwIm2DVertexSetV (&m_2dVertices[3], m_surPaint.m_SnapShot.UVs[3], GUI_UV_END);

	for(RwInt32 i = 0; i < 4; i++)
	{
		RwIm2DVertexSetIntRGBA(&m_2dVertices[i], m_surPaint.m_SnapShot.uRed, m_surPaint.m_SnapShot.uGreen, m_surPaint.m_SnapShot.uBlue, m_surPaint.m_SnapShot.uAlpha);
	}

	if(m_surPaint.m_pTexture)
	{
		RwD3D9SetTexture(m_surPaint.m_pTexture->GetTexture(), 0);
	}
	else
	{
		RwD3D9SetTexture(NULL, 0);
	}

	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_2dVertices, 4);
}