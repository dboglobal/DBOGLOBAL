#include "gui_precomp.h"
#include "gui_listbox_generic.h"
#include "gui_listbox.h"
#include "gui_listbox_item.h"
#include "gui_scrollbar.h"
#include "gui_component.h"
#include "gui_componentoption.h"
#include "gui_renderer.h"

START_GUI

#define DEFAULT_MASK_COLOR_RED		100
#define DEFAULT_MASK_COLOR_GREEN	100
#define DEFAULT_MASK_COLOR_BLUE		255
#define DEFAULT_MASK_COLOR_ALPHA	100

////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor:

CListBox_Generic::CListBox_Generic( CListBox* pSelf, BOOL bDynamicScrollBar, INT nMaxVisibleItems /* = LISTBOX_DEFAULT_VISIBLEITEM  */ )
: m_pListBox(pSelf), m_nVisibleRowCount( nMaxVisibleItems ), m_bDynamicScrollBar( bDynamicScrollBar )
{
	m_pFont = NULL;
	m_dwScrollStyle = 0;
	m_dwWidthSortStyle = 0;

	m_nMaxRowCount = 0;
	
	m_nMarginX = 0;
	m_nMarginY = 0;

	m_nOffsetRow = 0;

	m_strFont = DEFAULT_FONT;
	m_nFontH = DEFAULT_FONT_SIZE;			
	m_nFontAttributes = DEFAULT_FONT_ATTR;
	
	m_SelTextColor  = DEFAULT_SELECTCOLOR;
	m_TextColor		= DEFAULT_TEXTCOLOR;
	m_BkColor		= DEFAULT_BKCOLOR;
	m_nBkMode		= DEFAULT_BKMODE;
	m_EffectColor	= DEFAULT_TEXTEFFECTCOLOR;
	m_nEffectMode	= DEFAULT_TEXTEFFECTMODE;
	m_nEffectValue	= 0;

	SetSelectedMaskColor( DEFAULT_MASK_COLOR_RED, DEFAULT_MASK_COLOR_GREEN, DEFAULT_MASK_COLOR_BLUE, DEFAULT_MASK_COLOR_ALPHA );
		
	m_bMultiSelection = false;

	m_nRowWidth  = 0;
	CalcRowHeight();

	m_SlotPaint = m_pListBox->SigPaint().Connect( this, &CListBox_Generic::OnPaint );
	m_SlotSetOptions = m_pListBox->SigSetOptions().Connect( this, &CListBox_Generic::OnSetOptions );
	m_SlotMouseDown = m_pListBox->SigMouseDown().Connect(	this, &CListBox_Generic::OnMouseDown );
	m_SlotMouseMove = m_pListBox->SigMouseMove().Connect( this, &CListBox_Generic::OnMouseMove );
	m_SlotMouseLeave = m_pListBox->SigMouseLeave().Connect( this, &CListBox_Generic::OnMouseLeave );
	m_SlotScrollChange = m_pListBox->m_pScrollBar->SigValueChanged().Connect(	this, &CListBox_Generic::OnScrollChange );
	m_SlotWheelMove = m_pListBox->SigMouseWheel().Connect( this, &CListBox_Generic::OnWheelMove );
	m_SlotSliderMove = m_pListBox->m_pScrollBar->SigSliderMoved().Connect( this, &CListBox_Generic::OnScrollChange );
	m_SlotMove = m_pListBox->SigMove().Connect( this, &CListBox_Generic::OnMove );
	//m_SlotResize = m_pScrollBar->SigResize().Connect( this, &CListBox_Generic::OnResize );

	if( m_bDynamicScrollBar )
		m_pListBox->m_pScrollBar->Show( false );
	else
		m_pListBox->m_pScrollBar->Show( true );
}


CListBox_Generic::~CListBox_Generic()
{
	ClearAll();

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

/////////////////////////////////////////////////////////////////////////////
// Attributes:

INT CListBox_Generic::GetItemCount(VOID) 
{
	return (INT)m_vecItems.size();
}

const std::vector<CListBox_Item*>& CListBox_Generic::GetAllItems(VOID) const
{
	return m_vecItems;
}

std::vector<CListBox_Item*> CListBox_Generic::GetSelectedItems(VOID) 
{
	std::vector<CListBox_Item*> retval;
	std::vector<CListBox_Item*>::iterator it;

	for( it = m_vecItems.begin() ; it != m_vecItems.end() ; ++it )
	{
		if( (*it)->m_bSelected )
		{
			retval.push_back( (*it) );
		}
	}

	return retval;
}

std::vector<std::wstring> CListBox_Generic::GetSelectedItemsText(VOID)
{
	std::wstring buf;
	std::vector<std::wstring> retval;
	std::vector<CListBox_Item*>::iterator it;
	
	for( it = m_vecItems.begin() ; it != m_vecItems.end() ; ++it )
	{
		if( (*it)->m_bSelected )
		{
			buf = (*it)->m_pBuf->GetBuffer();
			retval.push_back( buf );
		}
	}
	
	return retval;
}

std::vector<INT> CListBox_Generic::GetSelectedItemsIndex(VOID)
{
	std::vector<INT> retval;
	std::vector<CListBox_Item*>::iterator it;
	INT nIndex;

	for( nIndex = 0, it = m_vecItems.begin() ; it != m_vecItems.end() ; ++it, ++nIndex )
	{
		if( (*it)->m_bSelected )
		{
			retval.push_back( nIndex );
		}
	}

	return retval;
}

CListBox_Item* CListBox_Generic::GetSelectedItem(VOID) 
{
	CListBox_Item* retval = NULL;
	std::vector<CListBox_Item*>::iterator it;

	for( it = m_vecItems.begin() ; it != m_vecItems.end() ; ++it )
	{
		if( (*it)->m_bSelected )
		{
			retval = (*it);
			break;
		}
	}

	return retval;
}

std::wstring CListBox_Generic::GetSelectedItemText(VOID)
{
	std::wstring retval;
	std::vector<CListBox_Item*>::iterator it;

	for( it = m_vecItems.begin() ; it != m_vecItems.end() ; ++it )
	{
		if( (*it)->m_bSelected )
		{
			retval = (*it)->m_pBuf->GetBuffer();
			break;
		}
	}

	return retval;
}

INT CListBox_Generic::GetSelectedItemIndex(VOID)
{
	INT retval = -1;
	std::vector<CListBox_Item*>::iterator it;
	INT nIndex;

	for( nIndex = 0, it = m_vecItems.begin() ; it != m_vecItems.end() ; ++it, ++nIndex )
	{
		if( (*it)->m_bSelected )
		{
			retval = nIndex;
			break;
		}
	}

	return retval;
}

CListBox_Item* CListBox_Generic::GetItem( INT nIndex )
{
	CListBox_Item* retval = NULL;
	
	if( nIndex >= 0 && nIndex < GetItemCount() )
		retval = m_vecItems[nIndex];

	return retval;
}

std::wstring CListBox_Generic::GetItemText( INT nIndex )
{
	std::wstring retval;
		
	if( nIndex >= 0 && nIndex < GetItemCount() )
		retval = m_vecItems[nIndex]->m_pBuf->GetBuffer();

	return retval;
}

bool CListBox_Generic::IsSelected( INT nIndex ) 
{
	if( nIndex >= 0 && nIndex < GetItemCount() )
		return ( m_vecItems[nIndex]->m_bSelected );

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

INT CListBox_Generic::AddItem( const CHAR* szText, DWORD dwData )
{
	INT nLen = (INT)strlen( szText );
	WCHAR *pBuffer = NTL_NEW WCHAR[nLen+1];

	::MultiByteToWideChar( GetACP(), 0, szText, -1, pBuffer, nLen+1 );

	AddItem( pBuffer, dwData );

	NTL_ARRAY_DELETE( pBuffer );

	return (INT)m_vecItems.size() - 1;
}

INT CListBox_Generic::AddItem( const WCHAR* szText, DWORD dwData )
{
	std::wstring strText = szText;

	if( strText.empty() )
		return (INT)m_vecItems.size() - 1;

	m_vecItems.push_back( NTL_NEW CListBox_Item( szText, m_pFont, dwData ) );
	
	ReviseItem();
	DrawItem();

	return (INT)m_vecItems.size() - 1;
}

INT CListBox_Generic::InsertItem( const CHAR* szText, INT nIndex, DWORD dwData )
{
	INT nLen = (INT)strlen( szText );
	WCHAR *pBuffer = NTL_NEW WCHAR[nLen+1];

	::MultiByteToWideChar( GetACP(), 0, szText, -1, pBuffer, nLen+1 );

	InsertItem( pBuffer, nIndex, dwData );

	NTL_ARRAY_DELETE( pBuffer );

	return (INT)m_vecItems.size() - 1;
}

INT CListBox_Generic::InsertItem( const WCHAR* szText, INT nIndex, DWORD dwData )
{
	std::wstring strText = szText;

	if( strText.empty() )
		return (INT)m_vecItems.size() - 1;
	
	if( nIndex < 0 || nIndex >= (INT)m_vecItems.size() )
		m_vecItems.push_back( NTL_NEW CListBox_Item( szText, m_pFont, dwData ) );
	else
	{
		std::vector<CListBox_Item*>::iterator it;
		INT i;
		for( i = 0, it = m_vecItems.begin() ; it != m_vecItems.end() ; ++it, ++i )
		{
			if( i = nIndex )
				break;
		}
		m_vecItems.insert( it, NTL_NEW CListBox_Item( szText, m_pFont, dwData ) );
	}

	ReviseItem();
	DrawItem();

	return (INT)m_vecItems.size() - 1;
}

VOID CListBox_Generic::ChangeItem( const CHAR* szText, INT nIndex, DWORD dwData )
{
	INT nLen = (INT)strlen( szText );
	WCHAR *pBuffer = NTL_NEW WCHAR[nLen+1];

	::MultiByteToWideChar( GetACP(), 0, szText, -1, pBuffer, nLen+1 );

	ChangeItem( pBuffer, nIndex, dwData );
}

VOID CListBox_Generic::ChangeItem( const WCHAR* szText, INT nIndex, DWORD dwData )
{
	if( nIndex < 0 || nIndex >= GetItemCount() )
		return;

	m_vecItems[nIndex]->m_pBuf->SetText( szText );
	m_vecItems[nIndex]->m_dwData = dwData;

	ReviseItem();
	DrawItem();
}

VOID CListBox_Generic::RemoveItem( INT nIndex )
{
	if( nIndex < 0 || nIndex >= GetItemCount() )
		return;

	std::vector<CListBox_Item*>::iterator it;
	INT i = 0;

	for(it = m_vecItems.begin(); it != m_vecItems.end(); ++it, ++i)
	{
		if( i == nIndex )
		{
			CListBox_Item* pItem = *it;

			m_vecItems.erase(it);
			
			delete pItem;

			break;
		}
	}

	if( GetItemCount() - m_nOffsetRow < m_nVisibleRowCount )
		m_nOffsetRow = 0;

	ReviseItem();
	DrawItem();
}

VOID CListBox_Generic::RemoveSelectedItems(VOID)
{
	std::vector<INT> vecIndexes = GetSelectedItemsIndex();
	std::vector<INT>::iterator itIndex;
	std::vector<CListBox_Item*>::iterator it;
	INT i;

	for( i = 0, it = m_vecItems.begin() ; it != m_vecItems.end() ; ++i, ++it )
	{
		for( itIndex = vecIndexes.begin() ; itIndex != vecIndexes.end() ; ++itIndex )
		{
			if( i == (*itIndex) )
			{
				NTL_DELETE( *it );
				m_vecItems.erase( it );
				vecIndexes.erase( itIndex );
				break;
			}
		}
	}

	if( GetItemCount() - m_nOffsetRow < m_nVisibleRowCount )
		m_nOffsetRow = 0;

	ReviseItem();
	DrawItem();
}

VOID CListBox_Generic::SelectItem( INT nIndex, bool bSelect )
{
	if( nIndex < 0 || nIndex >= GetItemCount() )
		return;

	if( !m_bMultiSelection )
		SelectAll( false );

	m_vecItems[nIndex]->m_bSelected = bSelect;

	if( IsVisibleIndex( nIndex ) )
	{
		ReviseItem();
		DrawItem();
	}
	else
	{
		// 현재 안보이는 인덱스를 선택했을 경우, 그 인덱스를 첫 인덱스로 하고 출력.
		if( bSelect )
			SetTopItem( nIndex );	
	}	
}

VOID CListBox_Generic::SelectAll( bool bSelect )
{
	if( bSelect && !m_bMultiSelection )
		return;

	std::vector<CListBox_Item*>::iterator it;
	INT i;

	for( i = 0, it = m_vecItems.begin() ; it != m_vecItems.end() ; ++it, ++i )
		(*it)->m_bSelected = bSelect;

}

VOID CListBox_Generic::InvertSelection(VOID)
{
	if( !m_bMultiSelection )
		return;

	std::vector<CListBox_Item*>::iterator it;
	INT i;

	for( i = 0, it = m_vecItems.begin() ; it != m_vecItems.end() ; ++it, ++i )
		(*it)->m_bSelected = !( (*it)->m_bSelected );

}

BOOL CListBox_Generic::IsVisibleIndex( INT nIndex )
{
	if( nIndex >= m_nOffsetRow && nIndex < m_nOffsetRow + m_nVisibleRowCount )
		return TRUE;

	return FALSE;
}

VOID CListBox_Generic::SetMultiSelection( bool bEnable )
{
	m_bMultiSelection = bEnable;
}

VOID CListBox_Generic::SetItemHeight( INT nHeight )
{
	INT nMargin = 2 * m_nMarginY;

	m_nRowHeight = nHeight;

	m_nVisibleRowCount = ( m_pListBox->GetHeight() - nMargin ) / m_nRowHeight;
}

VOID CListBox_Generic::SetMaxVisibleItems( INT nCount )
{
	m_nVisibleRowCount = nCount;

	CalcRowHeight();
}

VOID CListBox_Generic::SetTopItem( INT nIndex )
{
	INT maximum_offset = GetItemCount() - m_nVisibleRowCount;

	if( nIndex < 0 )
		nIndex = 0;

	if( maximum_offset < 0 )
		maximum_offset = 0;

	if( nIndex >= maximum_offset )
		m_nOffsetRow = maximum_offset;
	else
		m_nOffsetRow = nIndex;

	ReviseItem();
	DrawItem();
}

VOID CListBox_Generic::SetSelectedMaskColor( BYTE uRed, BYTE uGreen, BYTE uBlue, BYTE uAlpha )
{
	m_MaskSurface.m_SnapShot.uRed = uRed;
	m_MaskSurface.m_SnapShot.uGreen = uGreen;
	m_MaskSurface.m_SnapShot.uBlue = uBlue;
	m_MaskSurface.m_SnapShot.uAlpha = uAlpha;
}

VOID CListBox_Generic::SetSelectedTextColor( COLORREF color )
{
	m_SelTextColor = color;
}

VOID CListBox_Generic::SetTextColor( COLORREF color )
{
	m_TextColor = color;
}

VOID CListBox_Generic::SetBkColor( COLORREF color )
{
	m_BkColor = color;
}

VOID CListBox_Generic::SetBkMode( INT nBkMode )
{
	m_nBkMode = nBkMode;
}

VOID CListBox_Generic::Sort( bool bAscending )
{
	// 미구현.
}

VOID CListBox_Generic::ClearAll(VOID)
{
	ClearItems();
	ClearDrawItems();
	ClearSelectionMasks();
	
	m_nOffsetRow = 0;
	
	ReviseItem();
	DrawItem();
}

VOID CListBox_Generic::ClearSelection(VOID)
{
	SelectAll( false );
	ClearSelectionMasks();
}

VOID CListBox_Generic::CreateFont( CHAR* pFaceName, INT nHeight, INT nAttributes /* = 0  */)
{
	if( m_pFont ) 
	{
		g_FontMgr.DeleteGuiFont(m_pFont);
		m_pFont = NULL;
	}

	m_pFont = g_FontMgr.CreateGuiFont( pFaceName, nHeight, nAttributes );
	if( m_pFont == NULL )
		return;

	CRectangle rcScreen = m_pListBox->GetScreenRect();
	
	if( rcScreen.GetWidth() == 0 || rcScreen.GetHeight() == 0 )
		return;

	if( m_TextSurface.m_pTexture ) 
	{
		g_TextureMgr.DeleteTexture( m_TextSurface.m_pTexture );
		m_TextSurface.m_pTexture = NULL;
	}

	// surface setting
	m_TextSurface.m_pTexture = g_TextureMgr.CreateTexture( rcScreen.GetWidth() + m_nEffectValue, rcScreen.GetHeight() + m_nEffectValue );
	m_TextSurface.m_SnapShot.uRed	= 255;
	m_TextSurface.m_SnapShot.uGreen = 255;
	m_TextSurface.m_SnapShot.uBlue	= 255;
	m_TextSurface.m_SnapShot.uAlpha = 255;
	m_TextSurface.m_SnapShot.UVs[0] = 0;
	m_TextSurface.m_SnapShot.UVs[1] = 0;
	m_TextSurface.m_SnapShot.UVs[2] = 1;
	m_TextSurface.m_SnapShot.UVs[3] = 1;

	m_TextSurface.m_SnapShot.rtRect.left	= rcScreen.left;
	m_TextSurface.m_SnapShot.rtRect.right	= rcScreen.left+m_TextSurface.m_pTexture->GetWidth();
	m_TextSurface.m_SnapShot.rtRect.top		= rcScreen.top;
	m_TextSurface.m_SnapShot.rtRect.bottom	= rcScreen.top+m_TextSurface.m_pTexture->GetHeight();
}

VOID CListBox_Generic::CreateFont(VOID)
{
	CreateFont( (char*)m_strFont.data(), m_nFontH, m_nFontAttributes );
}

VOID CListBox_Generic::AddSurface( CSurface surface )
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = m_pListBox->GetScreenRect(); 

	surface.m_SnapShot.rtRect.left = rtScreen.left + rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top + rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left + rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top + rtSnap.GetHeight();

	m_listSurface.push_back( surface );
}

VOID CListBox_Generic::AddMaskSurface( CSurface& surface )
{
	m_MaskSurface = surface;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

// Select items to be output (add, delete, etc.), select, add Ellipsis
VOID CListBox_Generic::ReviseItem()
{
	ClearDrawItems();

	if( GetItemCount() == 0 )
		return;

	UpdateScrollbar();
	CalcRowWidth();

	std::vector<CListBox_Item*>::iterator it = m_vecItems.begin();
	INT i;
	
	for (i = 0; i < m_nOffsetRow; ++i)
	{
		++it;
	}

	for(i = 0; i < m_nVisibleRowCount ; i++, it++ )
	{
		if( it == m_vecItems.end() )
			break;

		WCHAR InsertBuffer[GUI_TEXT_BUFFER_SIZE];
		BOOL bEllipsis = FALSE;

		CListBox_Item* pItem = (*it);

		INT nTextLength = pItem->m_pBuf->GetTextSize();
		INT	nPos, j;

		for( j = 1 ; j <= nTextLength ; j++ )
		{
			pItem->m_pBuf->CPtoX( j, FALSE, &nPos );

			if( nPos > m_nRowWidth )
			{
				bEllipsis = TRUE;
				break;
			}
		}

		if( bEllipsis )
		{
			CUniBuffer pEllipsisBuffer( 0 );
			pEllipsisBuffer.SetSizeCalcEnable();
			pEllipsisBuffer.SetFont( m_pFont );
			pEllipsisBuffer.SetText( STATICBOX_SYMBOL_STRING_ELLIPSIS );
			const WCHAR* pEllipsis = pEllipsisBuffer.GetBuffer();
			
			INT nEllipsisWidth = 0;
			pEllipsisBuffer.CPtoX( pEllipsisBuffer.GetTextSize(), FALSE, &nEllipsisWidth );

			if( m_nRowWidth - nPos >= nEllipsisWidth )
				nTextLength = j;
			else
			{
				INT nPrevChar = j;
				INT nPrevCharPosX = 0;

				while( nPrevChar >= 0 )
				{
					--nPrevChar;
					pItem->m_pBuf->CPtoX( nPrevChar, FALSE, &nPrevCharPosX );

					if( m_nRowWidth - nPrevCharPosX > nEllipsisWidth )
						break;
				}

				nTextLength = nPrevChar;
			}

			if( nTextLength <= 0 )
			{
				memset( InsertBuffer, 0, sizeof( WCHAR ) * wcslen( pEllipsis ) + 1 );
				memcpy( InsertBuffer, pEllipsis, sizeof( WCHAR ) * wcslen( pEllipsis ) );
			}
			else
			{
				memset( InsertBuffer, 0, sizeof( WCHAR ) * ( nTextLength + 1 ) );
				memcpy( InsertBuffer, pItem->m_pBuf->GetBuffer(), sizeof( WCHAR ) * nTextLength );
				wcscat( InsertBuffer, pEllipsis );				
			}

		}
		else
		{
			nTextLength = j;
			ZeroMemory(InsertBuffer, sizeof(InsertBuffer));
			memcpy( InsertBuffer, pItem->m_pBuf->GetBuffer(), sizeof( WCHAR ) * ( j - 1 ) );
		}


		m_listDrawItems.push_back(new CListBox_Item(InsertBuffer, m_pFont, pItem->m_dwData, pItem->m_bSelected));
	}
}

// 하이라이트, 텍스트 출력관련.
VOID CListBox_Generic::DrawItem(VOID)
{
	ClearSelectionMasks();

	if( m_TextSurface.m_pTexture )
		m_TextSurface.m_pTexture->FillClear();

	if( m_vecItems.empty() )
		return;	

	std::list<CListBox_Item*>::iterator it;
	INT idx;

	for( idx = 0, it = m_listDrawItems.begin() ; it != m_listDrawItems.end() ; ++it, ++idx )
	{
		CListBox_Item* pItem = (*it);

		COLORREF bkcolor;
		if( m_nBkMode == BK_OPAQUE )
			bkcolor = m_BkColor;
		else
			bkcolor = 0;

		COLORREF textColor = m_TextColor;

		INT nPosX = GetStartX( pItem->m_pBuf);
		INT nPosY = GetStartY( idx );
	
		if( nPosY >= 0 )
		{
			CRectangle rtBound, rect;

			if( pItem->m_bSelected )
			{
				CRectangle* pRect = NTL_NEW CRectangle;
				pRect->left   = m_nMarginX;
				pRect->right  = pRect->left + m_nRowWidth;
				pRect->top    = m_nMarginY + ( m_nRowHeight * idx );
				pRect->bottom = pRect->top + m_nRowHeight;
				m_listMaskRect.push_back( pRect );

				rect = m_pFont->TextOutW( pItem->m_pBuf->GetBuffer() );
			}
			else if (idx == m_nHighlightRow)
			{
				rect = m_pFont->TextOutW(pItem->m_pBuf->GetBuffer());

				textColor = m_SelTextColor;
			}
			else
				rect = m_pFont->TextOutW( pItem->m_pBuf->GetBuffer() );

			rtBound.SetRect( nPosX, nPosY, nPosX + rect.GetWidth(), nPosY + rect.GetHeight() );
			g_FontMgr.BitmapToTexture( m_TextSurface.m_pTexture, rtBound, textColor, m_nEffectMode, m_EffectColor, m_nEffectValue, m_nBkMode, m_BkColor );
		}
	}
}

INT CListBox_Generic::GetStartX( CUniBuffer* pBuffer )
{
	INT nXPos;
	INT nMarginX = m_nMarginX * 2;
	CRectangle rect = m_pListBox->GetClientRect();
	pBuffer->CPtoX( pBuffer->GetTextSize(), FALSE, &nXPos );

	INT nDelta = 0;
	if( m_pListBox->m_pScrollBar->IsVisible() )
		nDelta = m_pListBox->m_pScrollBar->GetWidth();

	if( ENABLE_COMPONENT_STYLE( m_dwWidthSortStyle, COMP_TEXT_LEFT) )
	{
		return m_nMarginX + m_nTextMarginX;
	}
	else if( ENABLE_COMPONENT_STYLE( m_dwWidthSortStyle, COMP_TEXT_CENTER ) )
	{
		INT nValue = ( rect.GetWidth() - nDelta - nMarginX - nXPos ) / 2 + m_nMarginX; 
		if( nValue > 0 )
			return nValue;
		else
			return 0;
	}
	else if( ENABLE_COMPONENT_STYLE( m_dwWidthSortStyle, COMP_TEXT_RIGHT ) )
	{
		INT nValue = ( rect.GetWidth() - nDelta - nXPos ) - m_nMarginX; 
		if( nValue )
			return nValue;
		else
			return 0;
	}
	else
	{
		return m_nMarginX;
	}
}

INT CListBox_Generic::GetStartY( INT nIndex )
{
	INT nFontHeight = m_pFont->GetHeight();
	INT nSpace = ( m_nRowHeight - nFontHeight ) / 2;	

	if( ENABLE_COMPONENT_STYLE( m_dwWidthSortStyle, COMP_TEXT_UP) )
	{
		return ( nIndex * m_nRowHeight ) + m_nMarginY;
	}
	else if(ENABLE_COMPONENT_STYLE(m_dwWidthSortStyle,COMP_TEXT_DOWN))
	{
		INT nValue = ( nIndex * m_nRowHeight ) + m_nMarginY + m_nRowHeight - nFontHeight;

		if( nValue > 0 )
			return nValue;
		else
			return 0;
	}
	else
	{
		return ( nIndex * m_nRowHeight ) + m_nMarginY + nSpace;
	}
}

VOID CListBox_Generic::CalcRowWidth(VOID)
{
	CRectangle rtListbox = m_pListBox->GetClientRect();

	m_nRowWidth = rtListbox.GetWidth() - 2 * m_nMarginX;

	if( m_pListBox->m_pScrollBar->IsVisible() )
	{
		CRectangle rtScroll = m_pListBox->m_pScrollBar->GetClientRect();
		m_nRowWidth -= rtScroll.GetWidth();
	}
}

VOID CListBox_Generic::CalcRowHeight(VOID)
{
	m_nRowHeight = ( m_pListBox->GetClientRect().GetHeight() - 2 * m_nMarginY ) / m_nVisibleRowCount;
}

INT CListBox_Generic::MousePosToIndex( INT nYPos )
{
	INT nCount = GetItemCount();
	if( nCount <= 0 )
		return -1;

	if( nYPos < m_nMarginY || nYPos > m_pListBox->GetHeight() - m_nMarginY )
		return -1;

	INT nIndex = (INT)( ( nYPos - m_nMarginY + m_nOffsetRow * m_nRowHeight ) / m_nRowHeight );

	if( nIndex >= nCount )
		return -1;

	return nIndex;
}

VOID CListBox_Generic::UpdateScrollbar(VOID)
{	
	INT nItemCount = GetItemCount();

	if( nItemCount > m_nVisibleRowCount )
	{
		if( m_bDynamicScrollBar && !m_pListBox->m_pScrollBar->IsVisible() )		
		{
			m_pListBox->m_pScrollBar->Show( true );
		}

		m_pListBox->m_pScrollBar->SetRange( 0, nItemCount - m_nVisibleRowCount );
		m_pListBox->m_pScrollBar->SetValue( m_nOffsetRow, FALSE );
	}
	else
	{
		if( m_bDynamicScrollBar && m_pListBox->m_pScrollBar->IsVisible() )
		{
			m_pListBox->m_pScrollBar->Show( false );
		}

		m_pListBox->m_pScrollBar->SetValue( 0, FALSE );
	}
}

VOID CListBox_Generic::ClearItems(VOID)
{
	std::vector<CListBox_Item*>::iterator it;

	for( it = m_vecItems.begin() ; it != m_vecItems.end() ; ++it )
	{
		NTL_DELETE( *it );
	}

	m_vecItems.clear();
}

VOID CListBox_Generic::ClearSelectionMasks(VOID)
{
	std::list<CRectangle*>::iterator it;

	for( it = m_listMaskRect.begin() ; it != m_listMaskRect.end() ; ++it )
	{
		NTL_DELETE( *it );
	}

	m_listMaskRect.clear();
}

VOID CListBox_Generic::ClearDrawItems(VOID)
{
	std::list<CListBox_Item*>::iterator it;

	for(it = m_listDrawItems.begin() ; it != m_listDrawItems.end(); ++it)
	{
		NTL_DELETE(*it);
	}

	m_listDrawItems.clear();
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

VOID CListBox_Generic::OnPaint(VOID)
{
	std::list<CSurface>::iterator the;
	for( the = m_listSurface.begin() ; the != m_listSurface.end() ; ++the )
	{
		g_GuiRenderer.RenderQueue( &(*the).m_SnapShot, (*the).m_pTexture );
	}

	std::list<CRectangle*>::iterator it;
	CRectangle* pRect;
	CRectangle rtScreen = m_pListBox->GetScreenRect();

	for( it = m_listMaskRect.begin() ; it != m_listMaskRect.end() ; ++it )
	{
		pRect = (*it);
		m_MaskSurface.m_SnapShot.rtRect.left = rtScreen.left + pRect->left;
		m_MaskSurface.m_SnapShot.rtRect.top = rtScreen.top + pRect->top;
		m_MaskSurface.m_SnapShot.rtRect.right = rtScreen.left + pRect->right;
		m_MaskSurface.m_SnapShot.rtRect.bottom = rtScreen. top + pRect->bottom;
		g_GuiRenderer.RenderQueue( &m_MaskSurface.m_SnapShot, m_MaskSurface.m_pTexture );
	}

	g_GuiRenderer.RenderQueue(&m_TextSurface.m_SnapShot, m_TextSurface.m_pTexture);
}

VOID CListBox_Generic::OnSetOptions( const CComponentOptions& options )
{
	if( options.Exists( "surface_file" ) )
	{
		std::string file = options.GetValue( "surface_file" ); 
		CSurfaceManager *pSurfaceManager = m_pListBox->GetSurfaceManager();
		if( pSurfaceManager )
		{
			INT nCount = options.Count( "surface" );
			for( INT i=0 ; i < nCount ; ++i )
				AddSurface( pSurfaceManager->GetSurface( file, options.GetValue( "surface", i ) ) ); 

			if( options.Exists( "masksurface" ) )
				AddMaskSurface( pSurfaceManager->GetSurface( file, options.GetValue( "masksurface" ) ) );				
		}
	}

	if(options.Exists("text_style"))
	{
		INT nCount = options.Count( "text_style" );
		for( INT i = 0 ; i < nCount ; ++i )
		{
			std::string style = options.GetValue( "text_style", i ); 

			if(style == "left")
			{
				m_dwWidthSortStyle |= COMP_TEXT_LEFT;
			}
			else if(style == "center")
			{
				m_dwWidthSortStyle |= COMP_TEXT_CENTER;
			}
			else if(style == "right")
			{
				m_dwWidthSortStyle |= COMP_TEXT_RIGHT;
			}
			else if( style == "up" )
			{
				m_dwWidthSortStyle |= COMP_TEXT_UP;
			}
			else if( style == "down" )
			{
				m_dwWidthSortStyle |= COMP_TEXT_DOWN;
			}
			else if( style == "verticalcenter" )
			{
				m_dwWidthSortStyle |= COMP_TEXT_VERTICAL_CENTER;
			}
		}
	}

	if (options.Exists("text_margin_x"))
		m_nTextMarginX = options.GetValueAsInt("text_margin_x");

	if( options.Exists( "margin_x" ) )
		m_nMarginX = options.GetValueAsInt( "margin_x" );

	if( options.Exists( "margin_y" ) )
		m_nMarginY = options.GetValueAsInt( "margin_y" );

	if (options.Exists("scrollbar_margin_x"))
		m_nScrollbarMarginX = options.GetValueAsInt("scrollbar_margin_x");

	if(options.Exists("font_name"))
		m_strFont = options.GetValue("font_name"); 
		
	if(options.Exists("font_attribute"))
	{
		int nCount = options.Count("font_attribute");
		for(int i=0 ; i<nCount ; i++)
			m_nFontAttributes |= options.GetValueAsInt("font_attribute",i);
	}
	
	if(options.Exists("font_height"))
		m_nFontH = options.GetValueAsInt("font_height" );

	CreateFont();

	BYTE byRed = DEFAULT_MASK_COLOR_RED;
	BYTE byGreen = DEFAULT_MASK_COLOR_GREEN;
	BYTE byBlue = DEFAULT_MASK_COLOR_BLUE;
	BYTE byAlpha = DEFAULT_MASK_COLOR_ALPHA;

	if(options.Exists("mask_color_red"))
		byRed = options.GetValueAsInt("mask_color_red");
	if(options.Exists("mask_color_green"))
		byGreen = options.GetValueAsInt("mask_color_green");
	if(options.Exists("mask_color_blue"))
		byBlue = options.GetValueAsInt("mask_color_blue");
	if(options.Exists("mask_alpha"))
		byAlpha = options.GetValueAsInt("mask_alpha");

	SetSelectedMaskColor( byRed, byGreen, byBlue, byAlpha );

	byRed = DEFAULT_TEXTCOLOR_RED;
	byGreen = DEFAULT_TEXTCOLOR_GREEN;
	byBlue = DEFAULT_TEXTCOLOR_BLUE;

	if(options.Exists("text_color_red"))
		byRed = options.GetValueAsInt("text_color_red");
	if(options.Exists("text_color_green"))
		byGreen = options.GetValueAsInt("text_color_green");
	if(options.Exists("text_color_blue"))
		byBlue = options.GetValueAsInt("text_color_blue");

	m_TextColor = RGB( byRed, byGreen, byBlue );

	byRed = DEFAULT_TEXTCOLOR_RED;
	byGreen = DEFAULT_TEXTCOLOR_GREEN;
	byBlue = DEFAULT_TEXTCOLOR_BLUE;

	if(options.Exists("text_selcolor_red"))
		byRed = options.GetValueAsInt("text_selcolor_red");
	if(options.Exists("text_selcolor_green"))
		byGreen = options.GetValueAsInt("text_selcolor_green");
	if(options.Exists("text_selcolor_blue"))
		byBlue = options.GetValueAsInt("text_selcolor_blue");

	m_SelTextColor = RGB(byRed,byGreen,byBlue);

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
			m_nEffectMode = TE_SHADOW;
			m_nEffectValue = DEFAULT_SHADOW_EFFECT_VALUE;
		}
		else if( effectmode == "outline" )
		{
			m_nEffectMode = TE_OUTLINE;
			m_nEffectValue = DEFAULT_OUTLINE_EFFECT_VALUE;
		}
	}
	if( options.Exists( "text_effect_value" ) )
	{
		m_nEffectValue = options.GetValueAsInt( "text_effect_value" );
	}

	m_EffectColor = RGB( byRed,byGreen,byBlue );

	if( options.Exists( "visiblecount" ) )
		m_nVisibleRowCount = options.GetValueAsInt( "visiblecount" );
	else
		m_nVisibleRowCount = LISTBOX_DEFAULT_VISIBLEITEM;

	if( options.Exists( "scrollbar_dynamicshow" ) )
	{
		m_bDynamicScrollBar = options.GetValueAsBool( "scrollbar_dynamicshow" );

		if( m_bDynamicScrollBar )
			m_pListBox->m_pScrollBar->Show( false );
		else
			m_pListBox->m_pScrollBar->Show( true );
	}

	CalcRowHeight();
}

VOID CListBox_Generic::OnMouseDown( const CKey& key )
{
	if( key.m_nID != UD_LEFT_BUTTON )
		return;

	INT nIndex = MousePosToIndex( (INT)key.m_fY );

	if( nIndex < 0 )
		return;

	RwBool bChanged = FALSE;
	if( !IsSelected( nIndex ) )
		bChanged = TRUE;

	if( m_bMultiSelection )
		SelectItem( nIndex, !m_vecItems[nIndex]->m_bSelected );	
	else
		SelectItem( nIndex, true );			

	m_SigSelected( nIndex );	

	if( bChanged )
		m_SigSelectionChanged();
}

VOID CListBox_Generic::OnMouseMove( INT nkey, INT xPos, INT yPos )
{
	INT nIndex = MousePosToIndex( yPos );
	INT nVisibleIndex = nIndex - m_nOffsetRow;

	if( nIndex < 0 )
	{
		if( !m_pListBox->GetToolTip().empty() )
			m_pListBox->DeleteToolTip();

		return;
	}
	
	if( nVisibleIndex == m_nHighlightRow )
		return;

	m_nHighlightRow = nVisibleIndex;
	m_SigHighlighted( nIndex );

	CListBox_Item* pItem = m_vecItems[nIndex];
	INT nTextLength = pItem->m_pBuf->GetTextSize();
	INT	nTextWidth;
	pItem->m_pBuf->CPtoX( nTextLength, FALSE, &nTextWidth );
	if( nTextWidth > m_nRowWidth )
	{
		m_pListBox->SetToolTip( std::wstring( pItem->m_pBuf->GetBuffer() ) );
		m_pListBox->ShowToolTip( true );
	}
	else
	{
		m_pListBox->DeleteToolTip();		
	}

	DrawItem();
}

VOID CListBox_Generic::OnMouseLeave( CComponent* pComponent )
{
	m_nHighlightRow = -1;

	DrawItem();
}

VOID CListBox_Generic::OnScrollChange( INT nNewOffset )
{
	SetTopItem( nNewOffset );	
}

VOID CListBox_Generic::OnWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
{
	RwInt32 nValue = m_pListBox->m_pScrollBar->GetValue();
	RwInt32 nMaxValue = m_pListBox->m_pScrollBar->GetMaxValue();
	RwInt32 nDelta = nValue - sDelta / GUI_MOUSE_WHEEL_DELTA;

	if( nDelta < 0 )
		nDelta = 0;
	else if( nDelta > nMaxValue )
		nDelta = nMaxValue;

	m_pListBox->m_pScrollBar->SetValue( nDelta );	
}

VOID CListBox_Generic::OnMove( INT nOldX, INT nOldY )
{
	CRectangle rtScreen,rtSnap;
	CSurface* pSurface;
	std::list<CSurface>::iterator the;

	rtScreen = m_pListBox->GetScreenRect();

	for( the = m_listSurface.begin() ; the != m_listSurface.end() ; ++the)
	{
		pSurface = &(*the);
		
		rtSnap = pSurface->m_Original.rtRect;

		pSurface->m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
		pSurface->m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
		pSurface->m_SnapShot.rtRect.right = pSurface->m_SnapShot.rtRect.left+rtSnap.GetWidth();
		pSurface->m_SnapShot.rtRect.bottom = pSurface->m_SnapShot.rtRect.top+rtSnap.GetHeight();
	}

	if( !m_pListBox || !m_TextSurface.m_pTexture || !m_pFont )
		return;

	m_TextSurface.m_SnapShot.rtRect.left = rtScreen.left;
	m_TextSurface.m_SnapShot.rtRect.right = rtScreen.left+m_TextSurface.m_pTexture->GetWidth();  
	m_TextSurface.m_SnapShot.rtRect.top = rtScreen.top;
	m_TextSurface.m_SnapShot.rtRect.bottom = rtScreen.top+m_TextSurface.m_pTexture->GetHeight();  
}

VOID CListBox_Generic::OnResize(int nOldWidth, int nOldHeight)
{
	DrawItem();

	CRectangle rect = m_pListBox->GetScreenRect();
	std::list<CSurface>::iterator it;

	// ListBox의 배경은 늘어난다. 
	for( it = m_listSurface.begin() ; it != m_listSurface.end() ; ++it )
	{
		(*it).m_SnapShot.rtRect.left = rect.left;
		(*it).m_SnapShot.rtRect.top = rect.top;
		(*it).m_SnapShot.rtRect.right = rect.right;
		(*it).m_SnapShot.rtRect.bottom = rect.bottom;
	}	
}

END_GUI
