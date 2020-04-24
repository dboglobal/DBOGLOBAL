#include "precomp_dboclient.h"
#include "RwTextureGui.h"

CRwTextureGui::CRwTextureGui()
:m_pTexture(NULL)
,m_iParentX(0)
,m_iParentY(0)
,m_iX_fromParent(0)
,m_iY_fromParent(0)
,m_iWidth(0)
,m_iHeight(0)
,m_byAlpha(255)
,m_bShow(TRUE)
,m_bSelfCreatedTexture(FALSE)
{
	ZeroMemory(v2dVertices, sizeof(v2dVertices));

#define GUI_UV_END		0.99f

	RwIm2DVertexSetScreenZ(&v2dVertices[0], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&v2dVertices[0], 1.0f);
	RwIm2DVertexSetU (&v2dVertices[0], 0.f, GUI_UV_END);
	RwIm2DVertexSetV (&v2dVertices[0], 0.f, GUI_UV_END);

	RwIm2DVertexSetScreenZ(&v2dVertices[1], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&v2dVertices[1], 1.0f);
	RwIm2DVertexSetU (&v2dVertices[1], 0.f, GUI_UV_END);
	RwIm2DVertexSetV (&v2dVertices[1], 1.f, GUI_UV_END);

	RwIm2DVertexSetScreenZ(&v2dVertices[2], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&v2dVertices[2], 1.0f);
	RwIm2DVertexSetU (&v2dVertices[2], 1.f, GUI_UV_END);
	RwIm2DVertexSetV (&v2dVertices[2], 0.f, GUI_UV_END);

	RwIm2DVertexSetScreenZ(&v2dVertices[3], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&v2dVertices[3], 1.0f);
	RwIm2DVertexSetU (&v2dVertices[3], 1.f, GUI_UV_END);
	RwIm2DVertexSetV (&v2dVertices[3], 1.f, GUI_UV_END);
}

CRwTextureGui::~CRwTextureGui()
{
	UnsetTexture();
}

VOID CRwTextureGui::SetTexture(RwTexture* pTexture)
{
	m_pTexture = pTexture;
}

VOID CRwTextureGui::SetTexture(RwChar* pcFile)
{
	if( !pcFile || strlen(pcFile) == 0 )
		return;

	RwImageSetPath(".\\texture\\gui\\");

	m_pTexture = RwTextureRead(pcFile, NULL);
	if( !m_pTexture )
		return;

	m_bSelfCreatedTexture = TRUE;
}

VOID CRwTextureGui::UnsetTexture()
{
	if( m_bSelfCreatedTexture )
		RwTextureDestroy(m_pTexture);

	m_pTexture				= NULL;
	m_bSelfCreatedTexture	= FALSE;
}

VOID CRwTextureGui::SetAlpha(RwUInt8 byAlpha)
{
	m_byAlpha = byAlpha;
}

VOID CRwTextureGui::SetPositionfromParent(RwInt32 iPosX, RwInt32 iPosY)
{
	m_iX_fromParent	= iPosX;
	m_iY_fromParent	= iPosY;

	CalcVertex();
}

VOID CRwTextureGui::SetPositionbyParent(RwInt32 iParentPosX, RwInt32 iParentPosY)
{
	m_iParentX		= iParentPosX;
	m_iParentY		= iParentPosY;

	CalcVertex();
}

VOID CRwTextureGui::SetSize(RwInt32 iWidth, RwInt32 iHeight)
{
	m_iWidth		= iWidth;
	m_iHeight		= iHeight;

	CalcVertex();
}

RwInt32 CRwTextureGui::GetWidth()
{
	return m_iWidth;
}

RwInt32 CRwTextureGui::GetHeight()
{
	return m_iHeight;
}

RwInt32 CRwTextureGui::GetPositionX()
{
	return m_iParentX;
}

RwInt32 CRwTextureGui::GetPositionY()
{
	return m_iParentY;
}

CRectangle CRwTextureGui::GetPosition()
{
	CRectangle rtPosition;

	rtPosition.left		= m_iParentX + m_iX_fromParent;
	rtPosition.top		= m_iParentY + m_iY_fromParent;
	rtPosition.right	= rtPosition.left + m_iWidth;
	rtPosition.bottom	= rtPosition.top  + m_iHeight;

	return rtPosition;
}

RwTexture* CRwTextureGui::GetTexture()
{
	return m_pTexture;
}

VOID CRwTextureGui::Show(bool bShow)
{
	m_bShow = bShow;
}

VOID CRwTextureGui::Render()
{
	if( !m_bShow )
		return;

	if( !m_pTexture )
		return;

	for(RwInt32 i = 0; i < 4; i++)
	{
		RwIm2DVertexSetIntRGBA(&v2dVertices[i], 255, 255, 255, m_byAlpha);
	}

	RwD3D9SetTexture( m_pTexture, 0 );
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, v2dVertices, 4);
}

RwBool CRwTextureGui::PtInRect(int iScreenX, int iScreenY)
{
	CRectangle rtRect = GetPosition();

	return rtRect.PtInRect(iScreenX, iScreenY);
}

RwBool CRwTextureGui::IsSelfCreatedTexture()
{
	return m_bSelfCreatedTexture;
}

VOID CRwTextureGui::CalcVertex()
{
#define dOFFSET			(-0.5f)

	CRectangle rtRect = GetPosition();

	RwIm2DVertexSetScreenX(&v2dVertices[0], (RwReal)rtRect.left		+ dOFFSET);
	RwIm2DVertexSetScreenY(&v2dVertices[0], (RwReal)rtRect.top		+ dOFFSET);

	RwIm2DVertexSetScreenX(&v2dVertices[1], (RwReal)rtRect.left		+ dOFFSET);
	RwIm2DVertexSetScreenY(&v2dVertices[1], (RwReal)rtRect.bottom	+ dOFFSET);

	RwIm2DVertexSetScreenX(&v2dVertices[2], (RwReal)rtRect.right	+ dOFFSET);
	RwIm2DVertexSetScreenY(&v2dVertices[2], (RwReal)rtRect.top		+ dOFFSET);

	RwIm2DVertexSetScreenX(&v2dVertices[3], (RwReal)rtRect.right	+ dOFFSET);
	RwIm2DVertexSetScreenY(&v2dVertices[3], (RwReal)rtRect.bottom	+ dOFFSET);
}