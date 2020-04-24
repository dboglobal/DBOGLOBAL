#include "gui_precomp.h"
#include "gui_staticbox_generic.h"
#include "gui_componentoption.h"
#include "gui_texture.h"
#include "gui_texturemanager.h"
#include "gui_font.h"
#include "gui_fontmanager.h"
#include "gui_renderer.h"

START_GUI

/////////////////////////////////////////////////////////////////////////////
// Construction:

CStaticBox_Generic::CStaticBox_Generic(	CStaticBox *pSelf, DWORD dwStyle, BOOL bDynamic )
: m_pStaticBox( pSelf ),m_dwStyle( dwStyle ), m_bDynamic( bDynamic )
{
	m_nFontH = DEFAULT_FONT_SIZE;
	m_strFont = DEFAULT_FONT;
	m_nFontAttribute = DEFAULT_FONT_ATTR;

	m_pFont = NULL;

	m_Color			= DEFAULT_TEXTCOLOR;
	m_BkColor		= DEFAULT_BKCOLOR;
	m_nBkMode		= DEFAULT_BKMODE;
	m_effectColor	= DEFAULT_TEXTEFFECTCOLOR;
	m_nEffectValue	= 0;

	m_nMaxCX		= m_pStaticBox->GetScreenRect().GetWidth();
	m_nMaxCY		= m_pStaticBox->GetScreenRect().GetHeight();
	m_nMinCX		= 0;
	m_nMinCY		= 0;
	m_nTextureWidth	= 0;
	m_nTextureHeight= 0;
	m_nMarginX		= 0;	
	m_nMarginY		= 0;
	m_nLineSpace	= 5;

	m_ucAlpha = DEFAULT_ALPHA;
	m_ucTexturePosOffset = 0;
	m_ucTextureSizeOffset = 0;

	m_pBuffer = NTL_NEW CUniBuffer(0);
	m_pBuffer->SetSizeCalcEnable();

	m_SlotSetOptions = m_pStaticBox->SigSetOptions().Connect( this, &CStaticBox_Generic::OnSetOptions );
	m_SlotMove = m_pStaticBox->SigMove().Connect( this, &CStaticBox_Generic::OnMove );
	m_SlotResize = m_pStaticBox->SigResize().Connect( this, &CStaticBox_Generic::OnResize );
	m_SlotSetAlpha = m_pStaticBox->SigSetAlpha().Connect( this, &CStaticBox_Generic::OnSetAlpha );
	m_SlotSetColor = m_pStaticBox->SigSetColor().Connect( this, &CStaticBox_Generic::OnSetColor );

	SetEffectMode( DEFAULT_TEXTEFFECTMODE );
	SetEffectValue( 0 );
}


CStaticBox_Generic::~CStaticBox_Generic(VOID)
{
	Clear();

	NTL_DELETE( m_pBuffer );

	if( m_TextSurface.m_pTexture ) 
	{
		g_TextureMgr.DeleteTexture( m_TextSurface.m_pTexture );
		m_TextSurface.m_pTexture = NULL;
	}

	if( m_pFont ) 
	{
		g_FontMgr.DeleteGuiFont( m_pFont );
		m_pFont = NULL;
	}
}


VOID CStaticBox_Generic::ItemClear(VOID)
{
	std::list<CStaticBox_Item*>::iterator it;
	for( it = m_listItem.begin() ; it != m_listItem.end() ; ++it )
	{
		NTL_DELETE( *it );
	}

	m_listItem.clear(); 
} 

VOID CStaticBox_Generic::Clear(VOID)
{
	ItemClear();

	if(m_pBuffer)
	{
		m_pBuffer->Clear(); 
	}

	if( m_TextSurface.m_pTexture )
		m_TextSurface.m_pTexture->FillClear(); 
}

VOID CStaticBox_Generic::CreateFont(char *pFaceName, INT nHeight, INT nAttributes /*= 0*/)
{
	if(m_pFont) 
	{
		g_FontMgr.DeleteGuiFont(m_pFont);
		m_pFont = NULL;
	}

	m_pFont = g_FontMgr.CreateGuiFont(pFaceName, nHeight, nAttributes);
	if(m_pFont == NULL)
		return;

	CRectangle rcScreen = m_pStaticBox->GetScreenRect();
	SetTextSurface( rcScreen );

	m_pBuffer->SetFont( m_pFont ); 
}

VOID CStaticBox_Generic::SetTextSurface( CRectangle rcScreen )
{
	if( rcScreen.GetWidth() == 0 || rcScreen.GetHeight() == 0 )
		return;

	// Magical number to print period.
#define AUTO_PERIOD_SIZE 7

	INT nWidth, nHeight;

	if( m_bDynamic )
	{
		nWidth = m_nMaxCX - 2 * m_nMarginX + AUTO_PERIOD_SIZE + m_ucTextureSizeOffset;
		nHeight = m_nMaxCY - 2 * m_nMarginY + m_ucTextureSizeOffset;
	}
	else
	{
		nWidth = rcScreen.GetWidth() - 2 * m_nMarginX + AUTO_PERIOD_SIZE + m_ucTextureSizeOffset;
		nHeight = rcScreen.GetHeight() - 2 * m_nMarginY + m_ucTextureSizeOffset;
	}	

	if( nWidth < 0 )
		nWidth = 1;

	if( nHeight < 0 )
		nHeight = 1;

	INT	nTextureWidth = g_TextureMgr.GetSortSize( nWidth );
	INT nTextureHeight = g_TextureMgr.GetSortSize( nHeight );

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

	// surface setting
	m_TextSurface.m_pTexture = g_TextureMgr.CreateTexture( nWidth, nHeight );
	m_TextSurface.m_SnapShot.uRed = 255;
	m_TextSurface.m_SnapShot.uGreen = 255;
	m_TextSurface.m_SnapShot.uBlue = 255;
	m_TextSurface.m_SnapShot.uAlpha = m_ucAlpha;
	m_TextSurface.m_SnapShot.UVs[0] = 0;
	m_TextSurface.m_SnapShot.UVs[1] = 0;
	m_TextSurface.m_SnapShot.UVs[2] = 1;
	m_TextSurface.m_SnapShot.UVs[3] = 1;

	SetTextTexturePos( rcScreen );	
}

INT CStaticBox_Generic::GetFitWidthToString(VOID)
{
	INT	nCurCX = 0;			
	INT nXPos, nNextXPos, nPrevXPos = 0;	
	INT	nTextSize = m_pBuffer->GetTextSize();

	for( INT i = 0 ; i < nTextSize ; i++ )
	{
		m_pBuffer->CPtoX(i, FALSE, &nXPos); 
		m_pBuffer->CPtoX(i+1, FALSE, &nNextXPos); 

		// Check LineFeed
		if( i < nTextSize - 1 )
		{
			WCHAR wCurentChar = (*m_pBuffer)[i];
			WCHAR wNextChar	  = (*m_pBuffer)[i+1];

			if( wCurentChar == L'\\' && wNextChar == L'n' )
			{
				m_pBuffer->CPtoX( i + 2, FALSE, &nPrevXPos );
				++i;
				continue;
			}
		}

		if( nCurCX < nNextXPos - nPrevXPos )
			nCurCX = nNextXPos - nPrevXPos;
	}

	return ( nCurCX + 2 * m_nMarginX );	
}

INT CStaticBox_Generic::GetFitHeightToString( VOID )
{
	INT nLine = 1;	
	INT	nCurCX = 0;			
	INT	nCurCY = 0;
	INT nXPos, nNextXPos, nPrevXPos = 0;	
	INT	nTextSize = m_pBuffer->GetTextSize();

	INT nMaxCX = m_nMaxCX - 2 * m_nMarginX;
	INT nMaxCY = m_nMaxCY - 2 * m_nMarginY;
	INT nMinCX = m_nMinCX - 2 * m_nMarginX;
	INT nMinCY = m_nMinCY - 2 * m_nMarginY;

	for( INT i = 0 ; i < nTextSize ; i++ )
	{
		m_pBuffer->CPtoX( i, FALSE, &nXPos ); 
		m_pBuffer->CPtoX( i + 1, FALSE, &nNextXPos ); 

		// Check LineFeed
		if( i < nTextSize - 1 )
		{
			WCHAR wCurentChar = (*m_pBuffer)[i];
			WCHAR wNextChar	  = (*m_pBuffer)[i+1];

			if( wCurentChar == L'\\' && wNextChar == L'n' )
			{
				m_pBuffer->CPtoX( i + 2, FALSE, &nPrevXPos );
				++nLine;
				++i;
				continue;
			}
		}

		if( nNextXPos - nPrevXPos > nMaxCX )
		{
			nPrevXPos = nXPos;
			++nLine;
			continue;
		}

		if( nCurCX < nNextXPos - nPrevXPos )
			nCurCX = nNextXPos - nPrevXPos;
	}

	// Line으로 높이 구하기.
	INT nFontHeight = m_pFont->GetHeight(); 
	INT nFontInternal = m_pFont->GetInternal();

	nCurCY = nFontHeight * nLine + nFontInternal + ( nLine - 1 ) * m_nLineSpace;

	return nCurCY;
}

VOID CStaticBox_Generic::Redraw(VOID)
{
	ItemClear();

	if( m_bDynamic )
		AdjustRectToString();

	StringDetect();

	DrawItem();
}

VOID CStaticBox_Generic::CreateFont(VOID)
{
	CreateFont( (CHAR*)m_strFont.data(), m_nFontH, m_nFontAttribute );	
}

VOID CStaticBox_Generic::SetText( const CHAR* text )
{
	INT nLen = (INT)strlen( text );
	WCHAR pBuffer[GUI_TEXT_BUFFER_SIZE];

	::MultiByteToWideChar(GetACP(), 0, text, -1, pBuffer, nLen+1);

	SetText(pBuffer);	
}

VOID CStaticBox_Generic::SetText( const WCHAR* text )  
{
	ItemClear();

	if( m_pBuffer )
		m_pBuffer->Clear(); 

	if( text == NULL || m_pStaticBox == NULL || m_pFont == NULL )
		return;

	m_pBuffer->SetText( text ); 

	if( m_bDynamic )
		AdjustRectToString();

	StringDetect();

	DrawItem();
}

VOID CStaticBox_Generic::AddText( const CHAR* text )
{
	INT nLen = (INT)strlen( text );
	WCHAR pBuffer[GUI_TEXT_BUFFER_SIZE];

	::MultiByteToWideChar(GetACP(), 0, text, -1, pBuffer, nLen+1);

	AddText(pBuffer);
}		

VOID CStaticBox_Generic::AddText( const WCHAR* text )
{
	ItemClear();

	if( text == NULL || m_pStaticBox == NULL ||m_pFont == NULL )
		return;

	if( m_pBuffer )
	{
		// peessi : If the unibuffer-related CPToXSize is modified, return it to its original state.
		INT nTextSize = m_pBuffer->GetTextSize();
		m_pBuffer->InsertString(nTextSize, text);

		//// peessi : Delete this part
		/*WCHAR pBuffer[GUI_TEXT_BUFFER_SIZE];
		swprintf_s( pBuffer, GUI_TEXT_BUFFER_SIZE, L"%s%s", m_pBuffer->GetBuffer(), text );
		m_pBuffer->SetText( pBuffer );*/
		////

		if( m_bDynamic )
			AdjustRectToString();

		StringDetect();

		DrawItem();
	}
}

VOID CStaticBox_Generic::SetMargin( INT nMarginX, INT nMarginY )
{
	CRectangle rcScreen = m_pStaticBox->GetScreenRect();

	m_nMarginX = nMarginX;
	m_nMarginY = nMarginY;

	SetTextTexturePos( rcScreen );	
}

VOID CStaticBox_Generic::SetEffectMode( INT nEffectMode, BOOL bSurfaceChange /* = TRUE  */ )
{
	m_nEffectMode = nEffectMode;
	SetEffectValue( m_nEffectValue, bSurfaceChange );
}

VOID CStaticBox_Generic::SetEffectValue( INT nEffectValue, BOOL bSurfaceChange /* = TRUE  */ )
{
	switch( m_nEffectMode )
	{
	case TE_NONE: m_ucTextureSizeOffset = 0; m_ucTexturePosOffset = 0; break;
	case TE_SHADOW: m_ucTexturePosOffset = 0; m_ucTextureSizeOffset = nEffectValue; break;
	case TE_OUTLINE: 
		{
			switch( nEffectValue )
			{
			case 0: m_ucTexturePosOffset = 1; m_ucTextureSizeOffset = 2; break;
			case 1: m_ucTexturePosOffset = 2; m_ucTextureSizeOffset = 4; break;
			case 2: m_ucTexturePosOffset = 3; m_ucTextureSizeOffset = 6; break;
			default: m_ucTexturePosOffset = 4; m_ucTextureSizeOffset = 8; break;
			}
		}
	}

	if( bSurfaceChange )
		SetTextSurface( m_pStaticBox->GetScreenRect() );

	m_nEffectValue = nEffectValue;

}

// Function called only in Dynamic.
VOID CStaticBox_Generic::AdjustRectToString(VOID)
{
	INT nLine = 1;	
	INT	nCurCX = 0;			
	INT	nCurCY = 0;
	INT nXPos, nNextXPos, nPrevXPos = 0;	
	INT	nTextSize = m_pBuffer->GetTextSize();

	INT nMaxCX = m_nMaxCX - 2 * m_nMarginX;
	INT nMaxCY = m_nMaxCY - 2 * m_nMarginY;
	INT nMinCX = m_nMinCX - 2 * m_nMarginX;
	INT nMinCY = m_nMinCY - 2 * m_nMarginY;

	for( INT i = 0 ; i < nTextSize ; i++ )
	{
		m_pBuffer->CPtoX( i, FALSE, &nXPos ); 
		m_pBuffer->CPtoX( i + 1, FALSE, &nNextXPos ); 

		// Check LineFeed
		if( i < nTextSize - 1 )
		{
			WCHAR wCurentChar = (*m_pBuffer)[i];
			WCHAR wNextChar	  = (*m_pBuffer)[i+1];

			if( wCurentChar == L'\\' && wNextChar == L'n' )
			{
				m_pBuffer->CPtoX( i + 2, FALSE, &nPrevXPos );
				++nLine;
				++i;
				continue;
			}
		}

		if( nNextXPos - nPrevXPos > nMaxCX )
		{
			nPrevXPos = nXPos;
			++nLine;
			continue;
		}

		if( nCurCX < nNextXPos - nPrevXPos )
			nCurCX = nNextXPos - nPrevXPos;
	}

	// Line으로 높이 구하기.
	INT nFontHeight = m_pFont->GetHeight(); 
	INT nFontInternal = m_pFont->GetInternal();

	nCurCY = nFontHeight * nLine + nFontInternal + ( nLine - 1 ) * m_nLineSpace;

	if( nCurCY > nMaxCY )
		nCurCY = nMaxCY;

	if( nCurCX < nMinCX )
		nCurCX = nMinCX;

	if( nCurCY < nMinCY )
		nCurCY = nMinCY;

	m_pStaticBox->SetSize( nCurCX + 2 * m_nMarginX, nCurCY + 2 * m_nMarginY );
}

// peessi : The characters used are not adapted to different language versions.
VOID CStaticBox_Generic::StringDetect(VOID)
{
	if( m_pStaticBox == NULL ||m_pFont == NULL )
		return;

	// The size of the space in the Component.
	INT nWidth = GetTextRegionWidth();
	INT nHeight = GetTextRegionHeight();

	INT nTextSize = m_pBuffer->GetTextSize(); 
	INT nXPos, nNextXPos = 0, nPrevXPos = 0, nStartX = 0, nAllocSize;
	INT i = 0;

	// Height Check 1
	// If you narrow it after checking height, return immediately.
	// by daneos - disable this so text is shown even if its bigger than the text-box
	/*if (nHeight < CalcCurrentHeight(1))
	{
		DBO_WARNING_MESSAGE("nHeight < CalcCurrentHeight(1)");
		return;
	}*/

	WCHAR InsertBuffer[GUI_TEXT_BUFFER_SIZE];

	for(i = 0; i < nTextSize; i++)
	{
		BOOL bLineFeed = FALSE;
		m_pBuffer->CPtoX(i, FALSE, &nXPos); 
		m_pBuffer->CPtoX(i+1, FALSE, &nNextXPos); 
		WCHAR wCurentChar = STATICBOX_NULL_STRING;
		WCHAR wNextChar = STATICBOX_NULL_STRING;

		// Check LineFeed
		if( i < nTextSize - 1 )
		{
			wCurentChar	= (*m_pBuffer)[i];
			wNextChar	= (*m_pBuffer)[i+1];

			if( wCurentChar == L'\\' && wNextChar == L'n' )
				bLineFeed = TRUE;
		}

		// Width, LineFeed Check
		if( nNextXPos - nPrevXPos > nWidth || bLineFeed )
		{
			// Height Check 2
			// If height does not exist after check height of next block, modify current string and return after input.
			INT nNextLineCount = (INT)m_listItem.size() + 2;
			if( nHeight < CalcCurrentHeight( nNextLineCount ) )
			{
				CUniBuffer pEllipsisBuffer( 0 );
				pEllipsisBuffer.SetFont( m_pFont );
				pEllipsisBuffer.SetText( STATICBOX_SYMBOL_STRING_ELLIPSIS );
				const WCHAR* pEllipsis = pEllipsisBuffer.GetBuffer();

				INT nEllipsisWidth = 0;
				pEllipsisBuffer.CPtoX( pEllipsisBuffer.GetTextSize(), FALSE, &nEllipsisWidth );

				if( nWidth - ( nXPos - nPrevXPos ) >= nEllipsisWidth )
					nAllocSize = i - nStartX;
				else
				{
					INT nPrevChar = i;
					INT nPrevCharPosX = 0;

					while( nPrevChar >= 0 )
					{
						--nPrevChar;
						m_pBuffer->CPtoX( nPrevChar, FALSE, &nPrevCharPosX );

						if( nWidth - ( nPrevCharPosX - nPrevXPos ) > nEllipsisWidth )
							break;
					}

					nAllocSize = nPrevChar - nStartX;
				}

				if( nAllocSize <= 0 )
				{
					memset( InsertBuffer, 0, sizeof( WCHAR ) * wcslen( pEllipsis ) + 1 );
					memcpy( InsertBuffer, pEllipsis, sizeof( WCHAR ) * wcslen( pEllipsis ) );
				}
				else
				{
					memset( InsertBuffer, 0, sizeof( WCHAR ) * ( nAllocSize + 1 ) );
					memcpy( InsertBuffer, m_pBuffer->GetBuffer() + nStartX, sizeof( WCHAR ) * nAllocSize );
					wcscat( InsertBuffer, pEllipsis );				
				}

				CStaticBox_Item* pItem = NTL_NEW CStaticBox_Item( InsertBuffer, m_pFont );
				m_listItem.push_back( pItem );
				return;
			}

			// . Check
			// If the first character after the command is. I change it to carry out next.
			if( !bLineFeed )
			{
				if( wCurentChar == STATICBOX_SYMBOL_CHARACTER_PERIOD )
					++i;	
			}			

			nAllocSize = i - nStartX;
			memset( InsertBuffer, 0, sizeof( WCHAR ) * ( nAllocSize + 1 ) );
			memcpy( InsertBuffer, m_pBuffer->GetBuffer() + nStartX, sizeof( WCHAR ) * nAllocSize );
			CStaticBox_Item* pItem = NTL_NEW CStaticBox_Item( InsertBuffer, m_pFont );
			m_listItem.push_back( pItem ); 

			if( bLineFeed )
			{
				INT nNextPos = i + 2;	// The position where the text is skipped.
				nStartX = nNextPos;
				if( nNextPos < nTextSize )
					m_pBuffer->CPtoX( nNextPos, FALSE, &nPrevXPos );
				++i;// Reduce unnecessary loops.
			}
			else
			{
				nStartX = i;
				nPrevXPos = nXPos;
			}			
		}
	}

	if(nStartX < i)
	{
		nAllocSize = i - nStartX;
		memset( InsertBuffer, 0, sizeof( WCHAR ) * ( nAllocSize + 1 ) );
		memcpy( InsertBuffer, m_pBuffer->GetBuffer() + nStartX, sizeof( WCHAR ) * nAllocSize );
		CStaticBox_Item* pItem = NTL_NEW CStaticBox_Item( InsertBuffer, m_pFont );
		m_listItem.push_back( pItem ); 		
	}	
}


INT CStaticBox_Generic::GetStartPosX( CUniBuffer* pBuffer )
{
	if( ENABLE_COMPONENT_STYLE( m_dwStyle, COMP_TEXT_LEFT ) )
	{
		return m_ucTexturePosOffset;
	}
	else if( ENABLE_COMPONENT_STYLE( m_dwStyle, COMP_TEXT_CENTER ) )
	{
		INT nWidth = m_pStaticBox->GetScreenRect().GetWidth() - 2 * m_nMarginX;
		INT nXPos;
		pBuffer->CPtoX( pBuffer->GetTextSize(), FALSE, &nXPos );

		if( nWidth >= nXPos )
			return ( nWidth - nXPos ) / 2 + m_ucTexturePosOffset; 		
	}
	else if( ENABLE_COMPONENT_STYLE( m_dwStyle, COMP_TEXT_RIGHT ) )
	{
		INT nWidth = m_pStaticBox->GetScreenRect().GetWidth() - 2 * m_nMarginX;
		INT nXPos;
		pBuffer->CPtoX( pBuffer->GetTextSize(), FALSE, &nXPos );

		if( nWidth >= nXPos )
			return ( nWidth - nXPos ) + m_ucTexturePosOffset; 		
	}

	return m_ucTexturePosOffset;
}

INT CStaticBox_Generic::GetStartPosY(INT nTotLine, INT nCurrLine)
{
	if( nTotLine == 0 )
		return -1;

	if( nCurrLine >= nTotLine )
		return -1;

	INT nFontHeight = m_pFont->GetHeight(); 
	INT nFontInternal = m_pFont->GetInternal();

	INT nStartY = 0;
	INT nHeight = GetTextRegionHeight();

	INT nCurrentYPos = nFontHeight * nCurrLine + m_nLineSpace * nCurrLine;
	INT nTotalTextHeight = CalcCurrentHeight( nTotLine );

	if( ENABLE_COMPONENT_STYLE( m_dwStyle, COMP_TEXT_UP ) )
	{									   
		nStartY = nFontInternal;

		if( nStartY + nCurrentYPos > nHeight - nFontHeight )
			return -1;
	}
	else if( ENABLE_COMPONENT_STYLE( m_dwStyle, COMP_TEXT_DOWN ) )
	{
		nStartY = ( nHeight - nTotalTextHeight );

		if( nStartY < 0 )
		{
			nStartY = nFontInternal;

			if( nStartY + nCurrentYPos > nHeight - nFontHeight )
				return -1;
		}
	}
	else
	{
		nStartY = ( nHeight - nTotalTextHeight ) / 2;

		if( nStartY < 0 )
		{
			nStartY = nFontInternal;

			if (nStartY + nCurrentYPos > nHeight - nFontHeight)
			{
			//	DBO_WARNING_MESSAGE("CStaticBox_Generic::GetStartPosY = FontSize StaticBoxSize Over"); // this says if font bigger than text box
				return 0;
				//return -1;
			}
		}	
	}

	return ( nStartY + nCurrentYPos ) + m_ucTexturePosOffset;
}

VOID CStaticBox_Generic::DrawItem(VOID)
{
	if( m_TextSurface.m_pTexture )
		m_TextSurface.m_pTexture->FillClear(); 
	else
		return;

	if (m_listItem.size() == 0)
	{
	//	DBO_WARNING_MESSAGE("m_listItem.size() == 0");
		return;
	}

	INT nItemSize = (INT)m_listItem.size();
	INT nIdx = 0;

	std::list<CStaticBox_Item*>::iterator it;
	for( it = m_listItem.begin() ; it != m_listItem.end() ; ++it )
	{
		CStaticBox_Item *pItem = (*it);

		CRectangle rect = m_pFont->TextOutW( pItem->m_pBuffer->GetBuffer() );   
		INT nPosX = GetStartPosX( pItem->m_pBuffer );
		INT nPosY = GetStartPosY( nItemSize, nIdx );

		if( nPosY >= 0 && nPosX >= 0 )
		{
			CRectangle rtBound;
			rtBound.SetRect( nPosX, nPosY, nPosX+rect.GetWidth(), nPosY+rect.GetHeight() );   
			g_FontMgr.BitmapToTexture( m_TextSurface.m_pTexture, rtBound, m_Color, m_nEffectMode, m_effectColor, m_nEffectValue, m_nBkMode, m_BkColor );
		}

		nIdx++;
	}
}

VOID CStaticBox_Generic::Render(VOID)
{
	CRectangle* pClippingRect = NULL;
	if( m_pStaticBox->IsClipped() )	
	{
		pClippingRect = m_pStaticBox->GetClippingRect();
	}

	g_GuiRenderer.RenderQueue( &m_TextSurface.m_SnapShot, m_TextSurface.m_pTexture, m_pStaticBox->GetRenderTop(), 0.0f, pClippingRect );
}

INT CStaticBox_Generic::CalcCurrentHeight( INT nTotalLine )
{
	if( nTotalLine <= 0 )
		return 0;

	INT nFontHeight = m_pFont->GetHeight(); 
	INT nFontInternal = m_pFont->GetInternal();

	return nFontHeight * nTotalLine + m_nLineSpace * ( nTotalLine - 1 ) + nFontInternal;	
}

INT CStaticBox_Generic::GetTextRegionWidth(VOID)
{
	return m_pStaticBox->GetScreenRect().GetWidth() - 2 * m_nMarginX;
}

INT CStaticBox_Generic::GetTextRegionHeight(VOID)
{
	return m_pStaticBox->GetScreenRect().GetHeight() - 2 * m_nMarginY;
}

VOID CStaticBox_Generic::SetTextTexturePos( CRectangle& rect )
{
	m_TextSurface.m_SnapShot.rtRect.left = rect.left + m_nMarginX - m_ucTexturePosOffset;
	m_TextSurface.m_SnapShot.rtRect.right = m_TextSurface.m_SnapShot.rtRect.left + m_TextSurface.m_pTexture->GetWidth();
	m_TextSurface.m_SnapShot.rtRect.top = rect.top + m_nMarginY - m_ucTexturePosOffset;
	m_TextSurface.m_SnapShot.rtRect.bottom = m_TextSurface.m_SnapShot.rtRect.top + m_TextSurface.m_pTexture->GetHeight();
}

/////////////////////////////////////////////////////////////////////////////
// callback function

VOID CStaticBox_Generic::OnSetOptions(const CComponentOptions &options)
{
	if(options.Exists("text_style"))
	{
		INT nCount = options.Count( "text_style" );
		for( INT i = 0 ; i < nCount ; ++i )
		{
			std::string style = options.GetValue( "text_style", i ); 

			if(style == "left")
			{
				m_dwStyle |= COMP_TEXT_LEFT;
			}
			if(style == "center")
			{
				m_dwStyle |= COMP_TEXT_CENTER;
			}
			if(style == "right")
			{
				m_dwStyle |= COMP_TEXT_RIGHT;
			}
			if( style == "up" )
			{
				m_dwStyle |= COMP_TEXT_UP;
			}
			if( style == "down" )
			{
				m_dwStyle |= COMP_TEXT_DOWN;
			}
			if( style == "verticalcenter" )
			{
				m_dwStyle |= COMP_TEXT_VERTICAL_CENTER;
			}
		}
	}

	if( options.Exists( "linespace" ) )
		m_nLineSpace = options.GetValueAsInt( "linespace" );

	if( options.Exists( "margin_x" ) )
		m_nMarginX = options.GetValueAsInt( "margin_x" );

	if( options.Exists( "margin_y" ) )
		m_nMarginY = options.GetValueAsInt( "margin_y" );


	BYTE byRed = DEFAULT_TEXTCOLOR_RED;
	BYTE byGreen = DEFAULT_TEXTCOLOR_GREEN;
	BYTE byBlue = DEFAULT_TEXTCOLOR_BLUE;

	if(options.Exists("text_color_red"))
		byRed = options.GetValueAsInt("text_color_red");
	if(options.Exists("text_color_green"))
		byGreen = options.GetValueAsInt("text_color_green");
	if(options.Exists("text_color_blue"))
		byBlue = options.GetValueAsInt("text_color_blue");

	m_Color= RGB(byRed,byGreen,byBlue);

	byRed = DEFAULT_BKCOLOR_RED;
	byGreen = DEFAULT_BKCOLOR_GREEN;
	byBlue = DEFAULT_BKCOLOR_BLUE;

	if(options.Exists("text_bkcolor_red"))
		byRed = options.GetValueAsInt("text_bkcolor_red");
	if(options.Exists("text_bkcolor_green"))
		byGreen = options.GetValueAsInt("text_bkcolor_green");
	if(options.Exists("text_bkcolor_blue"))
		byBlue = options.GetValueAsInt("text_bkcolor_blue");

	m_BkColor = RGB(byRed,byGreen,byBlue);

	if( options.Exists( "text_bk_mode" ) )
		m_nBkMode = options.GetValueAsBool( "text_bk_mode" );

	byRed = DEFAULT_TEXTEFFECTCOLOR_RED;
	byGreen = DEFAULT_TEXTEFFECTCOLOR_GREEN;
	byBlue = DEFAULT_TEXTEFFECTCOLOR_BLUE;

	if( options.Exists( "text_effectcolor_red" ) )
		byRed = options.GetValueAsInt( "text_effectcolor_red" );
	if( options.Exists( "text_effectcolor_green" ) )
		byGreen = options.GetValueAsInt( "text_effectcolor_green" );
	if( options.Exists( "text_effectcolor_blue" ) )
		byBlue = options.GetValueAsInt( "text_effectcolor_blue" );

	if( options.Exists( "text_effect_mode" ) )
	{
		std::string effectmode = options.GetValue( "text_effect_mode" );
		if( effectmode == "shadow" )
		{
			SetEffectMode( TE_SHADOW, FALSE );
			SetEffectValue( DEFAULT_SHADOW_EFFECT_VALUE, FALSE );			
		}	
		else if( effectmode == "outline" )
		{
			SetEffectMode( TE_OUTLINE, FALSE );
			SetEffectValue( DEFAULT_SHADOW_EFFECT_VALUE, FALSE );						
		}
	}
	if( options.Exists( "text_effect_value" ) )
	{
		SetEffectValue( options.GetValueAsInt( "text_effect_value" ), FALSE ); 		
	}

	m_effectColor = RGB( byRed,byGreen,byBlue );

	if( options.Exists( "dynamic" ) )
	{
		m_bDynamic = options.GetValueAsBool( "dynamic" );

		// Default Value;
		m_nMaxCX = m_pStaticBox->GetScreenRect().GetWidth();
		m_nMaxCY = m_pStaticBox->GetScreenRect().GetHeight();		

		if( options.Exists( "max_cx" ) )
			m_nMaxCX = options.GetValueAsInt( "max_cx" );
		if( options.Exists( "max_cy" ) )
			m_nMaxCY = options.GetValueAsInt( "max_cy" );
		if( options.Exists( "min_cx" ) )
			m_nMinCX = options.GetValueAsInt( "min_cx" );
		if( options.Exists( "min_cy" ) )
			m_nMinCY = options.GetValueAsInt( "min_cy" );
	}

	if(options.Exists("font_name"))
		m_strFont = options.GetValue("font_name"); 

	if(options.Exists("font_height"))
		m_nFontH = options.GetValueAsInt("font_height" );

	if(options.Exists("font_attribute"))
	{
		INT nCount = options.Count("font_attribute");
		for(INT i=0 ; i<nCount ; i++)
			m_nFontAttribute |= options.GetValueAsInt("font_attribute",i);
	}

	CreateFont();
}

VOID CStaticBox_Generic::OnMove(INT nOldX,INT nOldY)
{
	if(m_pStaticBox == NULL)
		return;
	if(m_TextSurface.m_pTexture == NULL)
		return;
	if(m_pFont == NULL)
		return;

	CRectangle rect = m_pStaticBox->GetScreenRect();
	SetTextTexturePos( rect );
}

VOID CStaticBox_Generic::OnResize( INT nOldCX, INT nOldCY )
{
	// Dynamic 의 경우는 Resize에서 TextSurface를 조정하지 않는다.
	if( m_bDynamic )	
		return;
	if( m_pStaticBox == NULL )
		return;
	if( m_TextSurface.m_pTexture == NULL )
		return;
	if( m_pFont == NULL )
		return;

	SetTextSurface( m_pStaticBox->GetScreenRect() );

	Redraw();
}

VOID CStaticBox_Generic::OnSetAlpha( BYTE ucAlpha )
{
	for( LIST_SURFACE_IT it = m_stlSurface.begin() ; it != m_stlSurface.end() ; ++it )
	{
		CSurface* pSurface = &(*it);

		pSurface->m_SnapShot.uAlpha = ucAlpha;
	}

	m_TextSurface.m_SnapShot.uAlpha = ucAlpha;

	m_ucAlpha = ucAlpha;
}

VOID CStaticBox_Generic::OnSetColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
	for( LIST_SURFACE_IT it = m_stlSurface.begin() ; it != m_stlSurface.end() ; ++it )
	{
		CSurface* pSurface = &(*it);

		pSurface->m_SnapShot.uRed = ucRed;
		pSurface->m_SnapShot.uGreen = ucGreen;
		pSurface->m_SnapShot.uBlue = ucBlue;
	}

	m_TextSurface.m_SnapShot.uRed = ucRed;
	m_TextSurface.m_SnapShot.uGreen = ucGreen;
	m_TextSurface.m_SnapShot.uBlue = ucBlue;
}
END_GUI
