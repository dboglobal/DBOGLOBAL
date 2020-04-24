#include "gui_precomp.h"

#include "gui_line_renderer.h"

START_GUI

CGuiLine::CGuiLine()
: m_p2dVertices(NULL)
, m_nVerticesNum(0)
{
}

CGuiLine::~CGuiLine()
{
	Destroy();
}

VOID CGuiLine::SetLine( SLineItem* pLineItem, int nItemNum )
{
	Destroy();

	m_nVerticesNum = nItemNum;

	m_p2dVertices = NTL_NEW RwIm2DVertex[nItemNum];

	RwReal fOffset = -0.5f;

	for( int i=0; i< nItemNum; ++i )
	{
		RwIm2DVertexSetScreenX(&m_p2dVertices[i], pLineItem[i].x + fOffset);
		RwIm2DVertexSetScreenY(&m_p2dVertices[i], pLineItem[i].y + fOffset);
		RwIm2DVertexSetScreenZ(&m_p2dVertices[i], 0.0f);
		RwIm2DVertexSetRecipCameraZ(&m_p2dVertices[i], 1.0f);
		RwIm2DVertexSetIntRGBA(&m_p2dVertices[i], 255, 255, 255, 255 );
	}
}

VOID CGuiLine::SetLine( RwReal fPosX1, RwReal fPosY1, RwReal fPosX2, RwReal fPosY2 )
{
	Destroy();

	m_nVerticesNum = 2;

	m_p2dVertices = NTL_NEW RwIm2DVertex[m_nVerticesNum];

	RwIm2DVertexSetScreenX(&m_p2dVertices[0], fPosX1);
	RwIm2DVertexSetScreenY(&m_p2dVertices[0], fPosY1);
	RwIm2DVertexSetScreenZ(&m_p2dVertices[0], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_p2dVertices[0], 1.0f);
	RwIm2DVertexSetIntRGBA(&m_p2dVertices[0], 255, 255, 255, 255 );

	RwIm2DVertexSetScreenX(&m_p2dVertices[1], fPosX2);
	RwIm2DVertexSetScreenY(&m_p2dVertices[1], fPosY2);
	RwIm2DVertexSetScreenZ(&m_p2dVertices[1], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_p2dVertices[1], 1.0f);
	RwIm2DVertexSetIntRGBA(&m_p2dVertices[1], 255, 255, 255, 255 );
}

VOID CGuiLine::SetLineAt( RwUInt32 uiIndex, RwReal fPosX, RwReal fPosY )
{
	if( uiIndex >= (unsigned int)m_nVerticesNum )
	{
		assert( 0 && "CGuiLine::SetLineAt - Buffer overflow");
		return;
	}

	RwIm2DVertexSetScreenX(&m_p2dVertices[uiIndex], fPosX);
	RwIm2DVertexSetScreenY(&m_p2dVertices[uiIndex], fPosY);
	RwIm2DVertexSetScreenZ(&m_p2dVertices[uiIndex], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_p2dVertices[uiIndex], 1.0f);
}

VOID CGuiLine::SetColor( COLORREF color )
{
	for( int i=0; i < m_nVerticesNum; ++i )
	{
		RwIm2DVertexSetIntRGBA(&m_p2dVertices[i], GetRValue( color ), GetGValue( color ), GetBValue( color ), 255 );
	}
}

VOID CGuiLine::Destroy()
{
	NTL_ARRAY_DELETE( m_p2dVertices );

	m_nVerticesNum = 0;
}

VOID CGuiLine::Render( VOID )
{
	if( m_p2dVertices == NULL )
		return;
	
	RwD3D9SetTexture(NULL, 0);
	

	RwIm2DRenderPrimitiveMacro(rwPRIMTYPEPOLYLINE, m_p2dVertices, m_nVerticesNum);

}

VOID CGuiLine::SetLineMove( RwReal fX, RwReal fY )
{
	RwReal fOffset = -0.5f;

	for( int i=0; i< m_nVerticesNum; ++i )
	{
		RwIm2DVertexSetScreenX(&m_p2dVertices[i], m_p2dVertices[i].x + fX + fOffset);
		RwIm2DVertexSetScreenY(&m_p2dVertices[i], m_p2dVertices[i].y + fY + fOffset);
		RwIm2DVertexSetScreenZ(&m_p2dVertices[i], 0.0f);
	}
}

VOID CGuiLine::SetAlpha( RwUInt8 byAlpha )
{
	for( int i=0; i < m_nVerticesNum; ++i )
	{
		m_p2dVertices[i].emissiveColor &= ( ((DWORD)byAlpha)<<24);
	}
}


END_GUI
