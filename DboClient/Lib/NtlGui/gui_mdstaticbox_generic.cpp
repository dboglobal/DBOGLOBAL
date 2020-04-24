#include "gui_precomp.h"
#include "gui_mdstaticbox_generic.h"
#include "gui_componentoption.h"
#include "gui_renderer.h"

#define FIXVALUE	0

START_GUI

#define MDSTATICBOX_TEXTURE_EFFECT_MARGIN	4

/////////////////////////////////////////////////////////////////////////////
// Construction:

CMDStaticBox_Generic::CMDStaticBox_Generic( CMDStaticBox *pSelf )
: m_pMDStaticBox( pSelf ), m_uniBuffer( 0 )
{
	m_dwStyle	= 0;
	m_colorText	= DEFAULT_TEXTCOLOR;	
	m_colorBK	= DEFAULT_BKCOLOR;
	m_nBkMode	= DEFAULT_BKMODE;
	m_colorEffect = DEFAULT_TEXTEFFECTCOLOR;
	m_nEffectMode = DEFAULT_TEXTEFFECTMODE;
	m_nEffectValue = 0;
	
	m_nMinCX = 0;
	m_nMinCY = 0;
	m_nMarginX	= 0;
	m_nMarginY	= 0;
	m_nInterval	= 5;
	m_dwDefaultFont = 0;

	m_nCursorX	= 0;	
	m_nCursorY	= 0;		
	m_nMaxWidth = m_pMDStaticBox->GetWidth();
	m_nMaxHeight= m_pMDStaticBox->GetHeight();
	m_nTotalItemCY = 0;

	m_nTextureWidth = 0;
	m_nTextureHeight = 0;

	m_nLineCount = 0;

	m_uniBuffer.SetSizeCalcEnable();

	m_SlotSetOptions = m_pMDStaticBox->SigSetOptions().Connect( this, &CMDStaticBox_Generic::OnSetOptions );
	m_SlotMove = m_pMDStaticBox->SigMove().Connect( this, &CMDStaticBox_Generic::OnMove );
	m_SlotPaint = m_pMDStaticBox->SigPaint().Connect( this, &CMDStaticBox_Generic::OnPaint );
	m_SlotSetAlpha = m_pMDStaticBox->SigSetAlpha().Connect( this, &CMDStaticBox_Generic::OnSetAlpha );
	m_SlotSetColor = m_pMDStaticBox->SigSetColor().Connect( this, &CMDStaticBox_Generic::OnSetColor );
}


CMDStaticBox_Generic::~CMDStaticBox_Generic()
{
	if( m_TextSurface.m_pTexture )
	{
		g_TextureMgr.DeleteTexture( m_TextSurface.m_pTexture );
		m_TextSurface.m_pTexture = NULL;
	}
	
	std::vector<CGuiFont*>::iterator it;
	
	for( it = m_vecFont.begin() ; it != m_vecFont.end() ; ++it )
	{
		if( (*it) )
		{
			g_FontMgr.DeleteGuiFont( (*it) );
			(*it) = NULL;
		}
	}
	m_vecFont.clear();

	ClearItem();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

BOOL CMDStaticBox_Generic::IsFontAvailable( DWORD dwIndex )
{
	DWORD nFontCount = (DWORD)m_vecFont.size();

	if( dwIndex >= nFontCount || !nFontCount )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Operation:

VOID CMDStaticBox_Generic::SetItem( const WCHAR* szText, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, 
								    COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue, 
									INT nX, BOOL bAttachPrevLine )
{
	std::string strName = szName;
	std::list<CMDStaticBox_Item*>::iterator it = FindItem( strName, m_listItem );
	CMDStaticBox_Item* pItem = NULL;
	CMDStaticBox_Item* pPrevItem = FindLastItem( m_listItem );
	
	if( !IsFontAvailable( dwFontIndex ) )
		return;

	if( it == m_listItem.end() )
	{
		pItem = NTL_NEW CMDStaticBox_Item( szText, szName, nX, 0, 0, 0, dwFontIndex, colorText, colorEffect, nEffectMode, nEffectValue, dwStyle, bAttachPrevLine );
		m_listItem.push_back( pItem );
	}	
	else
	{
		pItem = *it;
		FixMaxWidthHeight( pItem );
		pItem->ClearSubItem();
		pItem->SetValue( szText, dwFontIndex, colorText, colorEffect, nEffectMode, nEffectValue, dwStyle, bAttachPrevLine );
		pItem->SetPos( nX, 0 );
		pItem->SetSize( 0, 0 );
	}

	DetectLineFeed( pItem, pPrevItem, FALSE );
}

VOID CMDStaticBox_Generic::SetItemXY( const WCHAR* szText, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, 
									  COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue )
{
	std::string strName = szName;
	std::list<CMDStaticBox_Item*>::iterator it = FindItem( strName, m_listXYItem );
	CMDStaticBox_Item* pItem = NULL;

	if( !IsFontAvailable( dwFontIndex ) )
		return;

	if( it == m_listXYItem.end() )
	{
		pItem = NTL_NEW CMDStaticBox_Item( szText, szName, nX, nY, 0, 0, dwFontIndex, colorText, colorEffect, nEffectMode, nEffectValue, dwStyle );
		m_listXYItem.push_back( pItem );
	}	
	else
	{
		pItem = *it;
		pItem->ClearSubItem();
		pItem->SetValue( szText, dwFontIndex, colorText, colorEffect, nEffectMode, nEffectValue, dwStyle );
		pItem->SetPos( nX, nY );
		pItem->SetSize( 0, 0 );
	}

	DetectLineFeed( pItem, NULL, TRUE );
}

VOID CMDStaticBox_Generic::SetBlankLine( INT nCY /* = -1  */)
{
	if( nCY < 0 )
	{
		if( !IsFontAvailable( m_dwDefaultFont ) )
			return;
		
		m_uniBuffer.SetFont( m_vecFont[m_dwDefaultFont] );
		nCY = m_vecFont[m_dwDefaultFont]->GetHeight();
	}

	m_listItem.push_back( NTL_NEW CMDStaticBox_Item( L"", "", 0, 0, 0, nCY, m_dwDefaultFont, m_colorText, m_colorEffect, m_nEffectMode, m_nEffectValue, m_dwStyle ) );
	CalcMaxWidthHeight( 0, nCY, 0, FALSE );
}

VOID CMDStaticBox_Generic::DeleteItem( const CHAR* szName )
{
	std::list<CMDStaticBox_Item*>::iterator it;
	std::string strname = szName;

	for( it = m_listItem.begin() ; it != m_listItem.end() ; ++it )
	{
		if( (*it)->m_strName == strname )
		{
			m_nTotalItemCY -= (*it)->m_nCY;

			NTL_DELETE( *it );

			m_listItem.erase( it );

			return;
		}
	}

	for( it = m_listXYItem.begin() ; it != m_listXYItem.end() ; ++it )
	{
		if( (*it)->m_strName == strname )
		{
			NTL_DELETE( *it );

			m_listXYItem.erase( it );

			return;
		}
	}
}

VOID CMDStaticBox_Generic::ClearItem(VOID)
{
	m_nLineCount = 0;
	m_nTotalItemCY = 0;
	m_nMaxWidth = 0;
	m_nMaxHeight = 0;

	std::list<CMDStaticBox_Item*>::iterator it;
	for( it = m_listXYItem.begin() ; it != m_listXYItem.end() ; ++it )
	{
		NTL_DELETE( *it );
	}
	m_listXYItem.clear();

	for( it = m_listItem.begin() ; it != m_listItem.end() ; ++it )
	{
		NTL_DELETE( *it );
	}
	m_listItem.clear();
} 

VOID CMDStaticBox_Generic::DrawText(VOID)
{
	CRectangle rtPrint;
	CRectangle rtBound;
	CMDStaticBox_Item* pItem = NULL;
	CMDStaticBox_Item* pPrevItem = NULL;

	m_nCursorX = 0;
	m_nCursorY = 0;
		
	CreateTexture();
	RecalcPosition();

	if( m_TextSurface.m_pTexture )
		m_TextSurface.m_pTexture->FillClear();
	
	std::list<CMDStaticBox_Item*>::iterator it;

	for( it = m_listItem.begin() ; it != m_listItem.end() ; ++it )
	{
		pItem = (*it);
		DrawItem( pItem, pPrevItem );
		
		std::list<CMDStaticBox_Item*>::iterator itt;
		CMDStaticBox_Item* pChildItem = NULL;

		for( itt = pItem->m_listChildItem.begin() ; itt != pItem->m_listChildItem.end() ; ++itt )
		{
			pChildItem = (*itt);
			DrawItem( pChildItem, NULL );
		}

		pPrevItem = pItem;
	}

	for( it = m_listXYItem.begin() ; it != m_listXYItem.end() ; ++it )
	{
		pItem = (*it);
		DrawXYItem( pItem, NULL );

		std::list<CMDStaticBox_Item*>::iterator itt;
		CMDStaticBox_Item* pChildItem = NULL;
		CMDStaticBox_Item* pChildPrevItem = pItem;

		for( itt = pItem->m_listChildItem.begin() ; itt != pItem->m_listChildItem.end() ; ++itt )
		{
			pChildItem = (*itt);
			DrawXYItem( pChildItem, pChildPrevItem );
			pChildPrevItem = pChildItem;
		}		
	}
}

INT CMDStaticBox_Generic::AddFont( const CHAR* pFontName, INT nHeight, INT nAttributes )
{
	CGuiFont* pFont = g_FontMgr.CreateGuiFont( pFontName, nHeight, nAttributes );
	
	if( pFont )
		m_vecFont.push_back( pFont );

	return (INT)( m_vecFont.size() - 1 );
}

VOID CMDStaticBox_Generic::AddSurface( CSurface surface )
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = m_pMDStaticBox->GetScreenRect(); 

	surface.m_SnapShot.rtRect.left = rtScreen.left + rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top + rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left + rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top + rtSnap.GetHeight();

	m_listSurface.push_back( surface );
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

VOID CMDStaticBox_Generic::CalcMaxWidthHeight( INT nCX, INT nCY, INT nY, BOOL bXY )
{
	INT nMaxHeight = 0;
	
	if( nCY )
		++m_nLineCount;
		
	if( bXY )
	{
		nMaxHeight = nY + nCY;
	}
	else
	{
		m_nTotalItemCY += nCY;

		if( m_nLineCount > 1 )
			nMaxHeight = m_nTotalItemCY + m_nInterval * ( m_nLineCount - 1 );
		else
			nMaxHeight = m_nTotalItemCY;
	}
	
	m_nMaxWidth = max( m_nMaxWidth, nCX );
	m_nMaxHeight = max( m_nMaxHeight, nMaxHeight );
}

VOID CMDStaticBox_Generic::FixMaxWidthHeight( CMDStaticBox_Item* pItem )
{
	INT nLine = (INT)pItem->m_listChildItem.size() + 1;

	if( pItem->m_bAttachPrevLine )
		--nLine;

	m_nLineCount   -= nLine;
	m_nTotalItemCY -= nLine * pItem->m_nCY;
	m_nMaxHeight   = m_nTotalItemCY + m_nInterval * ( m_nLineCount - 1 );
}

VOID CMDStaticBox_Generic::DetectLineFeed( CMDStaticBox_Item* pItem, CMDStaticBox_Item* pPrevItem, BOOL bXY )
{
	BOOL bFirstLineFeed = TRUE;
	INT nCX = 0, nCY = 0;
	INT nPos = 0, nPointerPos = 0, nPointer = 0;
	
	// string detect
	m_uniBuffer.SetFont( m_vecFont[pItem->m_dwFontIndex] );
	m_uniBuffer.SetText( pItem->m_wstrText.c_str() );

	INT nTextSize = m_uniBuffer.GetTextSize();
	nCY = m_vecFont[pItem->m_dwFontIndex]->GetHeight();

	INT nTempCY = nCY;
	INT nTempCX = 0;

	if( pItem->m_bAttachPrevLine )
	{
		nTempCY = 0;
		
		if( pPrevItem )
			nTempCX = pPrevItem->m_nPosX + pPrevItem->m_nCX;
	}

	WCHAR pInsertBuffer[GUI_TEXT_BUFFER_SIZE];
	
	for( INT i = 0 ; i < nTextSize - 1 ; ++i )
	{
		WCHAR wCurentChar = m_uniBuffer[i];
		WCHAR wNextChar	  = m_uniBuffer[i+1];

		if( wCurentChar == L'\\' && wNextChar == L'n' )
		{
			m_uniBuffer.CPtoX( i , FALSE, &nPos );
			nCX = nPos - nPointerPos;

			INT nAllocSize = i - nPointer;
			memset( pInsertBuffer, 0, sizeof( WCHAR ) * ( nAllocSize + 1 ) );
			memcpy( pInsertBuffer, m_uniBuffer.GetBuffer() + nPointer, sizeof( WCHAR ) * nAllocSize );

			if( bFirstLineFeed )
			{
				pItem->m_wstrText = pInsertBuffer;
				pItem->SetSize( nCX, nCY );
				bFirstLineFeed = FALSE;
				CalcMaxWidthHeight( nCX + pItem->m_nPosX + nTempCX, nTempCY, pItem->m_nPosY, bXY );
			}
			else
			{
				INT nTempY = pItem->m_nPosY + ( nCY + m_nInterval ) * ( (INT)pItem->m_listChildItem.size() + 1 );
				CMDStaticBox_Item* pChildItem = NTL_NEW CMDStaticBox_Item( pInsertBuffer, "", pItem->m_nPosX, 0, nCX, nCY, pItem->m_dwFontIndex, 
																	   pItem->m_colorText, pItem->m_colorEffect, pItem->m_nEffectMode, pItem->m_nEffectValue, 
																	   pItem->m_dwStyle );
				pItem->AddItem( pChildItem );
				CalcMaxWidthHeight( nCX + pChildItem->m_nPosX, nCY, pChildItem->m_nPosY, bXY );
			}

			nPointer = i + 2;
			m_uniBuffer.CPtoX( i + 2, FALSE, &nPointerPos );
			++i;
		}
	}

	if ( bFirstLineFeed )
	{
		m_uniBuffer.CPtoX( nTextSize, FALSE, &nCX );
		++nCX;
		pItem->SetSize( nCX, nCY );
		CalcMaxWidthHeight( nCX + pItem->m_nPosX + nTempCX, nTempCY, pItem->m_nPosY, bXY );
	}
	else
	{
		m_uniBuffer.CPtoX( nTextSize , FALSE, &nPos );
		nCX = nPos - nPointerPos + 1;

		INT nAllocSize = nTextSize - nPointer;
		memset( pInsertBuffer, 0, sizeof( WCHAR ) * ( nAllocSize + 1 ) );
		memcpy( pInsertBuffer, m_uniBuffer.GetBuffer() + nPointer, sizeof( WCHAR ) * nAllocSize );
		
		INT nTempY = pItem->m_nPosY + ( nCY + m_nInterval ) * ( (INT)pItem->m_listChildItem.size() + 1 );
		CMDStaticBox_Item* pChildItem = NTL_NEW CMDStaticBox_Item( pInsertBuffer, "", pItem->m_nPosX, nTempY, nCX, nCY, pItem->m_dwFontIndex, 
															   pItem->m_colorText, pItem->m_colorEffect, pItem->m_nEffectMode, pItem->m_nEffectValue,
															   pItem->m_dwStyle );
		pItem->AddItem( pChildItem );
		CalcMaxWidthHeight( nCX + pChildItem->m_nPosX, nCY, pChildItem->m_nPosY, bXY );
	}
}


VOID CMDStaticBox_Generic::DrawItem( CMDStaticBox_Item* pItem, CMDStaticBox_Item* pPrevItem )
{
	CRectangle rtPrint;
	CRectangle rtBound;

	if( pPrevItem && pItem->m_bAttachPrevLine )
	{
		m_nCursorX = pPrevItem->m_nPosX + pPrevItem->m_nCX;
		m_nCursorY = pPrevItem->m_nPosY;
	}

	if( ENABLE_COMPONENT_STYLE( pItem->m_dwStyle, COMP_TEXT_CENTER ) )
	{
		pItem->m_nPosX = ( m_nMaxWidth - pItem->m_nCX ) / 2;
		if( pItem->m_nPosX < 0 )
			pItem->m_nPosX = 0;
	} 
	else if( ENABLE_COMPONENT_STYLE( pItem->m_dwStyle, COMP_TEXT_RIGHT ) )
	{
		pItem->m_nPosX = m_nMaxWidth - pItem->m_nCX;
		if( pItem->m_nPosX < 0 )
			pItem->m_nPosX = 0;
	}
	else
	{
		pItem->m_nPosX += m_nCursorX;
	}

	pItem->m_nPosY = m_nCursorY;

	if( pItem->m_nCX > 1 && pItem->m_nCY > 0 )
	{
		rtPrint = m_vecFont[pItem->m_dwFontIndex]->TextOutW( pItem->m_wstrText.c_str() );
		rtBound.SetRectWH( pItem->m_nPosX + MDSTATICBOX_TEXTURE_EFFECT_MARGIN, pItem->m_nPosY + MDSTATICBOX_TEXTURE_EFFECT_MARGIN, rtPrint.GetWidth(), rtPrint.GetHeight() );
		g_FontMgr.BitmapToTexture( m_TextSurface.m_pTexture, rtBound, pItem->m_colorText, pItem->m_nEffectMode, pItem->m_colorEffect, pItem->m_nEffectValue, m_nBkMode, m_colorBK );
	}
	
	m_nCursorX = 0;			
	m_nCursorY += pItem->m_nCY + m_nInterval;
}

VOID CMDStaticBox_Generic::DrawXYItem( CMDStaticBox_Item* pItem, CMDStaticBox_Item* pPrevChildItem /* = NULL  */)
{
	CRectangle rtPrint;
	CRectangle rtBound;

	if( pItem->m_nCX > 1 && pItem->m_nCY > 0 )
	{
		if( ENABLE_COMPONENT_STYLE( pItem->m_dwStyle, COMP_TEXT_CENTER ) )
		{
			pItem->m_nPosX = ( m_nMaxWidth - pItem->m_nCX ) / 2;
			if( pItem->m_nPosX < 0 )
				pItem->m_nPosX = 0;
		} 
		else if( ENABLE_COMPONENT_STYLE( pItem->m_dwStyle, COMP_TEXT_RIGHT ) )
		{
			pItem->m_nPosX = m_nMaxWidth - pItem->m_nCX;
			if( pItem->m_nPosX < 0 )
				pItem->m_nPosX = 0;
		}
		
		if( pPrevChildItem )
		{
			pItem->m_nPosY = pPrevChildItem->m_nPosY + pPrevChildItem->m_nCY + m_nInterval;
		}

		rtPrint = m_vecFont[pItem->m_dwFontIndex]->TextOutW( pItem->m_wstrText.c_str() );
		rtBound.SetRectWH( pItem->m_nPosX + MDSTATICBOX_TEXTURE_EFFECT_MARGIN, pItem->m_nPosY + MDSTATICBOX_TEXTURE_EFFECT_MARGIN, rtPrint.GetWidth(), rtPrint.GetHeight() );
		g_FontMgr.BitmapToTexture( m_TextSurface.m_pTexture, rtBound, pItem->m_colorText, pItem->m_nEffectMode, pItem->m_colorEffect, pItem->m_nEffectValue, m_nBkMode, m_colorBK );
	}
	else
	{
		if( pPrevChildItem )
		{
			pItem->m_nPosY = pPrevChildItem->m_nPosY + pPrevChildItem->m_nCY + m_nInterval;
		}
	}
}

VOID CMDStaticBox_Generic::SetTextTexturePos( CRectangle& rect )
{
	m_TextSurface.m_SnapShot.rtRect.left = rect.left + m_nMarginX - MDSTATICBOX_TEXTURE_EFFECT_MARGIN;
	m_TextSurface.m_SnapShot.rtRect.right = m_TextSurface.m_SnapShot.rtRect.left + m_TextSurface.m_pTexture->GetWidth();
	m_TextSurface.m_SnapShot.rtRect.top = rect.top + m_nMarginY - MDSTATICBOX_TEXTURE_EFFECT_MARGIN;
	m_TextSurface.m_SnapShot.rtRect.bottom = m_TextSurface.m_SnapShot.rtRect.top + m_TextSurface.m_pTexture->GetHeight();
}

VOID CMDStaticBox_Generic::CreateTexture(VOID)
{
	if( m_nMaxWidth == 0 || m_nMaxHeight == 0 )
		return;

	INT nTextureWidth = g_TextureMgr.GetSortSize( m_nMaxWidth + 2 * MDSTATICBOX_TEXTURE_EFFECT_MARGIN );
	INT nTextureHeight= g_TextureMgr.GetSortSize( m_nMaxHeight + 2 * MDSTATICBOX_TEXTURE_EFFECT_MARGIN );

	CRectangle rcScreen = m_pMDStaticBox->GetScreenRect();

	if( m_nTextureWidth == nTextureWidth && m_nTextureHeight == nTextureHeight )
	{
		SetTextTexturePos( rcScreen );
		return;
	}
		
	m_nTextureWidth = nTextureWidth;
	m_nTextureHeight = nTextureHeight;

	if( m_TextSurface.m_pTexture )
	{
		g_TextureMgr.DeleteTexture( m_TextSurface.m_pTexture );
		m_TextSurface.m_pTexture = NULL;
	}

	m_TextSurface.m_pTexture = g_TextureMgr.CreateTexture( nTextureWidth, nTextureHeight );
	m_TextSurface.m_SnapShot.uRed = 255;
	m_TextSurface.m_SnapShot.uGreen = 255;
	m_TextSurface.m_SnapShot.uBlue = 255;
	m_TextSurface.m_SnapShot.uAlpha = 255;
	m_TextSurface.m_SnapShot.UVs[0] = 0;
	m_TextSurface.m_SnapShot.UVs[1] = 0;
	m_TextSurface.m_SnapShot.UVs[2] = 1;
	m_TextSurface.m_SnapShot.UVs[3] = 1;	

	SetTextTexturePos( rcScreen );	
}

VOID CMDStaticBox_Generic::RecalcPosition(VOID)
{
	CRectangle rect = m_pMDStaticBox->GetPosition();
	
	INT nWidth = m_nMaxWidth + 2 * m_nMarginX;
	INT nHeight = m_nMaxHeight + 2 * m_nMarginY;
	
	if( nWidth < m_nMinCX )
		nWidth = m_nMinCX;

	if( nHeight < m_nMinCY )
		nHeight	= m_nMinCY;

	rect.right = rect.left + nWidth;
	rect.bottom= rect.top + nHeight;

	m_pMDStaticBox->SetPosition( rect );
}

std::list<CMDStaticBox_Item*>::iterator CMDStaticBox_Generic::FindItem( std::string& strName, std::list<CMDStaticBox_Item*>& stdList )
{
	std::list<CMDStaticBox_Item*>::iterator it;

	for( it = stdList.begin() ; it != stdList.end() ; ++it )
	{
		if( (*it)->m_strName == strName )
			return it;
	}

	return it;
}

CMDStaticBox_Item* CMDStaticBox_Generic::FindLastItem( std::list<CMDStaticBox_Item*>& stdList )
{
	if( stdList.empty() )
		return NULL;
	else
		return stdList.back();	
}

/////////////////////////////////////////////////////////////////////////////
// callback function

// setoption 관련 대폭 수정해야함. 기능 미비.
VOID CMDStaticBox_Generic::OnSetOptions( const CComponentOptions& options )
{
	if( options.Exists( "min_cx" ) )
		m_nMinCX = options.GetValueAsInt( "min_cx" );
	if( options.Exists( "min_cy" ) )
		m_nMinCY = options.GetValueAsInt( "min_cy" );
	if( options.Exists( "margin_x" ) )
		m_nMarginX = options.GetValueAsInt( "margin_x" );
	if( options.Exists( "margin_y" ) )
		m_nMarginY = options.GetValueAsInt( "margin_y" );
	if( options.Exists( "width" ) )
		m_nMaxWidth = options.GetValueAsInt( "width" ) - 2 * m_nMarginX;
	if( options.Exists( "height" ) )
		m_nMaxHeight = options.GetValueAsInt( "height" ) - 2 * m_nMarginY;
	if( options.Exists( "interval" ) )
		m_nInterval = options.GetValueAsInt( "interval" );
	if( options.Exists( "defaultfont_index" ) )
		m_dwDefaultFont = options.GetValueAsInt( "defaultfont_index" );
	if( options.Exists( "font_name" ) )
	{
		INT nCount = options.Count( "font_name" );
		for( INT i = 0 ; i < nCount ; ++i )
		{
			AddFont( options.GetValue( "font_name", i ).c_str(), 
					 options.GetValueAsInt( "font_height", i ), 
					 options.GetValueAsInt( "font_attribute", i ) );
		}
	}
	else
	{
		AddFont( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	}

	if( options.Exists( "text" ) )
	{
		INT nCount = options.Count( "text" );
		for( INT i = 0 ; i < nCount ; ++i )
		{
			BOOL bAttach = options.GetValueAsBool( "text_attachprevline", i );
			BOOL bForceXY = options.GetValueAsBool( "text_forcexy", i );
			INT nFontIndex = options.GetValueAsInt( "font_index", i );
			DWORD dwStyle = 0;
			std::string style = options.GetValue( "text_style", i );
			if(style == "left")
			{
				dwStyle |= COMP_TEXT_LEFT;
			}
			if(style == "center")
			{
				dwStyle |= COMP_TEXT_CENTER;
			}
			if(style == "right")
			{
				dwStyle |= COMP_TEXT_RIGHT;
			}
			DWORD colorText = RGB( options.GetValueAsInt( "text_color_red", i ), 
								   options.GetValueAsInt( "text_color_green", i ),
								   options.GetValueAsInt( "text_color_blue", i ) );
			
			if( !bForceXY )
			{
				m_pMDStaticBox->SetItem( options.GetValue( "text", i ).c_str(), options.GetValue( "text_name", i ).c_str(),
										 nFontIndex, dwStyle, colorText, 0, bAttach );
									
			}
			else
			{
				INT nX = options.GetValueAsInt( "text_x", i );
				INT nY = options.GetValueAsInt( "text_y", i );

				m_pMDStaticBox->SetItemXY( options.GetValue( "text", i ).c_str(), options.GetValue( "text_name", i ).c_str(),
										   nFontIndex, nX, nY, dwStyle, colorText );
			}
		}
	}

	if(options.Exists("surface_file"))
	{
		std::string file = options.GetValue( "surface_file" ); 
		CSurfaceManager* pSurfaceManager = m_pMDStaticBox->GetSurfaceManager();
		if( pSurfaceManager )
		{
			if( options.Exists( "surface" ) )
			{
				INT nCount = options.Count( "surface" );
				for( INT i = 0 ; i < nCount ; ++i )
					AddSurface( pSurfaceManager->GetSurface( file,options.GetValue( "surface", i ) ) ); 
			}
		}
	}

	DrawText();
}

VOID CMDStaticBox_Generic::OnMove(int nX,int nY)
{
	if( m_pMDStaticBox == NULL )
		return;

	CRectangle rtScreen = m_pMDStaticBox->GetScreenRect();

	if( m_TextSurface.m_pTexture )
		SetTextTexturePos( rtScreen );
	
	CRectangle rtSnap;

	CSurface* pSurface;
	std::list<CSurface>::iterator the;

	for( the = m_listSurface.begin() ; the != m_listSurface.end() ; ++the )
	{
		pSurface = &(*the);
		rtSnap = pSurface->m_Original.rtRect;

		pSurface->m_SnapShot.rtRect.left  = rtScreen.left + rtSnap.left;
		pSurface->m_SnapShot.rtRect.top	  = rtScreen.top + rtSnap.top;
		pSurface->m_SnapShot.rtRect.right = pSurface->m_SnapShot.rtRect.left + rtSnap.GetWidth();
		pSurface->m_SnapShot.rtRect.bottom= pSurface->m_SnapShot.rtRect.top + rtSnap.GetHeight();
	}
}

VOID CMDStaticBox_Generic::OnPaint(VOID)
{
	std::list<CSurface>::iterator it;

	for( it = m_listSurface.begin() ; it != m_listSurface.end() ; ++it )
	{
		g_GuiRenderer.RenderQueue( &(*it).m_SnapShot, (*it).m_pTexture, m_pMDStaticBox->GetRenderTop() );
	}

	g_GuiRenderer.RenderQueue( &m_TextSurface.m_SnapShot, m_TextSurface.m_pTexture, m_pMDStaticBox->GetRenderTop() );
}

VOID CMDStaticBox_Generic::OnSetAlpha( RwUInt8 ucAlpha )
{
	m_TextSurface.m_SnapShot.uAlpha = ucAlpha;

	CSurface* pSurface;
	std::list<CSurface>::iterator it;

	for( it = m_listSurface.begin() ; it != m_listSurface.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uAlpha = ucAlpha;
	}
}

VOID CMDStaticBox_Generic::OnSetColor( RwUInt8 ucRed, RwUInt8 ucGreen, RwUInt8 ucBlue )
{
	m_TextSurface.m_SnapShot.uRed = ucRed;
	m_TextSurface.m_SnapShot.uGreen = ucGreen;
	m_TextSurface.m_SnapShot.uBlue = ucBlue;

	CSurface* pSurface;
	std::list<CSurface>::iterator it;

	for( it = m_listSurface.begin() ; it != m_listSurface.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uRed = ucRed;
		pSurface->m_SnapShot.uGreen = ucGreen;
		pSurface->m_SnapShot.uBlue = ucBlue;
	}
}				  



END_GUI
