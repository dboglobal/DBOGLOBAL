#include "gui_precomp.h"
#include "gui_outputbox_generic.h"
#include "gui_componentoption.h"
#include "gui_texture.h"
#include "gui_texturemanager.h"
#include "gui_font.h"
#include "gui_fontmanager.h"
#include "gui_renderer.h"

START_GUI

/**
* \brief Construction
* \param pSelf				(COutputBox*) 인터페이스로 사용될 COutputBox의 객체
* \param nMaxLine			(INT) 현재의 컴포넌트가 저장하는 최대 줄 수
* \param nMarginX			(INT) 컴포넌트내에서 시작 X 좌표
* \param nMarginY			(INT) 컴포넌트내에서 시작 Y 좌표
* \param bDynamicScrollBar	(bool) 스크롤바가 동적으로 작동하는지 여부
* \param bScrollBarLeft		(bool) 스크롤바의 좌/우 여부
* \param dwStyle			(DWORD) 
*/
COutputBox_Generic::COutputBox_Generic( COutputBox *pSelf, INT nMaxLine, INT nMarginX, INT nMarginY,
										bool bDynamicScrollBar, bool bScrollBarLeft, DWORD dwStyle )
:m_pOutputBox( pSelf ), m_nMaxLine( nMaxLine ), m_nMarginX( nMarginX ), m_nMarginY( nMarginY ),
 m_bDynamicScrollBar( bDynamicScrollBar ), m_bScrollbarLeftLine( bScrollBarLeft ), m_dwStyle( dwStyle )
{
	m_pFont			= NULL;
	m_nScrollLine	= 0;
	m_nRowLimit		= 0;
	m_nFontH		= DEFAULT_FONT_SIZE;
	m_strFont		= DEFAULT_FONT;
	m_dwTextColor	= DEFAULT_TEXTCOLOR;
	m_dwBkColor		= DEFAULT_BKCOLOR;
	m_nBkMode		= DEFAULT_BKMODE;
	m_dwTextEffectColor = DEFAULT_TEXTEFFECTCOLOR;
	m_nTextEffectMode	= DEFAULT_TEXTEFFECTMODE;
	m_nTextEffectValue	= 0;
	m_nInterval		= 0;
	m_nDrawItemNum	= 0;
	m_itDrawItem	= m_listDrawItem.begin();
	m_bNeedDrawItem = false;
	m_nScrollBarWidth = OUTPUTBOX_SCROLLBAR_SIZE;		// Scrollbar Width
	m_nScrollBarMarginTop = 0;							// Scrollbar MarginTop
	m_nScrollBarMarginBottom = 0;						// Scrollbar MarginBottom
	m_nDrawOffset = 0;

	CScrollBar* pScrollBar = m_pOutputBox->m_pSBVer;
	
	m_SlotSetOptions = m_pOutputBox->SigSetOptions().Connect( this, &COutputBox_Generic::OnSetOptions );
	m_SlotScrollChange = pScrollBar->SigValueChanged().Connect( this, &COutputBox_Generic::OnScrollChange );
	m_SlotScrollMove = pScrollBar->SigSliderMoved().Connect( this, &COutputBox_Generic::OnScrollChange );
	m_SlotMove = m_pOutputBox->SigMove().Connect( this, &COutputBox_Generic::OnMove );
	m_SlotPaint = m_pOutputBox->SigPaint().Connect( this, &COutputBox_Generic::OnPaint );
	m_SlotResize = m_pOutputBox->SigResize().Connect( this, &COutputBox_Generic::OnResize );
}

/**
* \brief Destruction
*/
COutputBox_Generic::~COutputBox_Generic()
{
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

	std::list<COutputBox_Item*>::iterator it;
	for( it = m_listDrawItem.begin() ; it != m_listDrawItem.end() ; ++it )
	{
		NTL_DELETE( *it );
	}
	m_listDrawItem.clear();
	m_itDrawItem = m_listDrawItem.begin();
	m_nDrawItemNum = 0;

	for( it = m_listCrudeItem.begin() ; it != m_listCrudeItem.end() ; ++it )	
	{
		NTL_DELETE( *it );
	}
	m_listCrudeItem.clear();

	for( it = m_listOriginalItem.begin() ; it != m_listOriginalItem.end() ; ++it )	
	{
		NTL_DELETE( *it );
	}
	m_listOriginalItem.clear();

	if( m_TextSurface.m_pTexture )
		m_TextSurface.m_pTexture->FillClear(); 

	m_nScrollLine = 0;
}

/**
* \brief 가지고 있는 아이템들을 모두 초기화한다.
*/
VOID COutputBox_Generic::Clear(VOID)
{
	std::list<COutputBox_Item*>::iterator it;
	for( it = m_listDrawItem.begin() ; it != m_listDrawItem.end() ; ++it )
	{
		COutputBox_Item* pOutputBox = (COutputBox_Item*)(*it);
		NTL_DELETE( pOutputBox );
	}
	m_listDrawItem.clear();

	if( m_pFont )
	{
		for( int i = 0; i<m_nMaxLine; ++i )
		{
			COutputBox_Item* pItem = NTL_NEW COutputBox_Item(m_pFont);
			m_listDrawItem.push_back( pItem );
		}
	}

	m_itDrawItem = m_listDrawItem.begin();
	m_nDrawItemNum = 0;

	for( it = m_listCrudeItem.begin() ; it != m_listCrudeItem.end() ; ++it )	
	{
		COutputBox_Item* pOutputBox = (COutputBox_Item*)(*it);
		NTL_DELETE( pOutputBox );
	}
	m_listCrudeItem.clear();

	for( it = m_listOriginalItem.begin() ; it != m_listOriginalItem.end() ; ++it )	
	{
		COutputBox_Item* pOutputBox = (COutputBox_Item*)(*it);
		NTL_DELETE( pOutputBox );
	}
	m_listOriginalItem.clear();

	if( m_TextSurface.m_pTexture )
		m_TextSurface.m_pTexture->FillClear(); 

	m_nScrollLine = 0;
}

/**
* \brief 현재의 컴포넌트에서 사용할 폰트를 생성한다.
* \param pFaceName		(CHAR*) 폰트의 이름
* \param nHeight		(INT) 폰트의 크기
* \param nAttributes	(INT) 폰트의 속성
*/
VOID COutputBox_Generic::CreateFont( CHAR* pFaceName, INT nHeight, INT nAttributes /*= 0*/ )
{
	CRectangle rect = m_pOutputBox->GetScreenRect();
	if( rect.GetWidth() == 0 || rect.GetHeight() == 0 )
		return;

	if( m_pFont ) 
	{
		g_FontMgr.DeleteGuiFont( m_pFont );
		m_pFont = NULL;
	}

	if( m_TextSurface.m_pTexture ) 
	{
		g_TextureMgr.DeleteTexture( m_TextSurface.m_pTexture );
		m_TextSurface.m_pTexture = NULL;
	}

	m_pFont = g_FontMgr.CreateGuiFont( pFaceName, nHeight, 0 );
	if( m_pFont == NULL )
		return;

	std::list<COutputBox_Item*>::iterator it = m_listDrawItem.begin();
	while( it != m_listDrawItem.end() )
	{
		NTL_DELETE( *it );
		m_listDrawItem.erase( it );
		it = m_listDrawItem.begin();
	}

	for( int i = 0; i<m_nMaxLine; ++i )
	{
		COutputBox_Item* pItem = NTL_NEW COutputBox_Item(m_pFont);
		m_listDrawItem.push_back( pItem );
	}
	m_itDrawItem = m_listDrawItem.begin();
	m_nDrawItemNum = 0;
	
	VisibleLineCount();
	
	// 7 is Magical number to print period.
	m_TextSurface.m_pTexture = g_TextureMgr.CreateTexture( rect.GetWidth() + 7 + m_nTextEffectValue, rect.GetHeight() + m_nTextEffectValue );
	m_TextSurface.m_SnapShot.uRed = 255;
	m_TextSurface.m_SnapShot.uGreen = 255;
	m_TextSurface.m_SnapShot.uBlue = 255;
	m_TextSurface.m_SnapShot.uAlpha = 255;
	m_TextSurface.m_SnapShot.UVs[0] = 0;
	m_TextSurface.m_SnapShot.UVs[1] = 0;
	m_TextSurface.m_SnapShot.UVs[2] = 1;
	m_TextSurface.m_SnapShot.UVs[3] = 1;

	m_TextSurface.m_SnapShot.rtRect.left = rect.left;
	m_TextSurface.m_SnapShot.rtRect.right = rect.left+m_TextSurface.m_pTexture->GetWidth();
	m_TextSurface.m_SnapShot.rtRect.top = rect.top;
	m_TextSurface.m_SnapShot.rtRect.bottom = rect.top+m_TextSurface.m_pTexture->GetHeight();
}

/**
* \brief 현재의 컴포넌트에서 사용할 폰트를 생성한다. ( DEFAULT )
*/
VOID COutputBox_Generic::CreateFont(VOID)
{
	CreateFont( (CHAR*)m_strFont.data(), m_nFontH, m_nFontAttribute );
}

/**
* \brief 컴포넌트에서 문자열을 추가한다.
* \param text			(const CHAR*)	문자열
* \param dwTextColor	(DWORD) 문자의 색깔
* \param nEffectMode	(INT) 문자의 효과
* \param dwEffectColor	(DWORD) 효과의 색깔
* \param nEffectValue	(INT) 효과의 수치
*/
VOID COutputBox_Generic::AddText( const CHAR* text, DWORD dwTextColor, INT nEffectMode, DWORD dwEffectColor, INT nEffectValue )
{
	INT nLen = (INT)strlen( text );
	WCHAR pBuffer[GUI_TEXT_BUFFER_SIZE];

	::MultiByteToWideChar( GetACP(), 0, text, -1, pBuffer, nLen + 1 );

	AddText( pBuffer, dwTextColor, nEffectMode, dwEffectColor, nEffectValue );	
}

VOID COutputBox_Generic::AddText( const WCHAR* text, DWORD dwTextColor, INT nEffectMode, DWORD dwEffectColor, INT nEffectValue )
{
	AddOriginalItem( text, dwTextColor, nEffectMode, dwEffectColor, nEffectValue );
}

/**
* \brief 컴포넌트에 출력할 Surface를 지정한다.
* \param surface	(CSurface) 출력할 서페이스
*/
VOID COutputBox_Generic::AddSurface( CSurface surface )
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = m_pOutputBox->GetScreenRect(); 

	surface.m_SnapShot.rtRect.left = rtScreen.left + rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top + rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left + rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top + rtSnap.GetHeight();

	m_stlSurface.push_back( surface );
}

/**
* \brief 현재 보여지는 스트링의 한 줄 이전을 보여준다.
*/
VOID COutputBox_Generic::PrevLine(VOID)
{
	if( m_nDrawItemNum > m_nRowLimit )
	{
		if( --m_nScrollLine < 0 )
			m_nScrollLine = 0;

		// ScrollBar 조절.
		if( m_dwStyle & COMP_VER_SCROLL )
		{
			gui::CScrollBar* pScrollBar = m_pOutputBox->GetVerticalScrollBar();
			pScrollBar->SetValue( m_nScrollLine );			
		}

		m_bNeedDrawItem = true;
	}	
}

/**
* \brief 현재 보여지는 스트링의 다음 줄을 보여준다.
*/
VOID COutputBox_Generic::NextLine(VOID)
{
	if( m_nDrawItemNum > m_nRowLimit )
	{
		if( ++m_nScrollLine > m_nDrawItemNum - m_nRowLimit )
			m_nScrollLine = m_nDrawItemNum - m_nRowLimit;

		// ScrollBar 조절.
		if( m_dwStyle & COMP_VER_SCROLL )
		{
			gui::CScrollBar* pScrollBar = m_pOutputBox->GetVerticalScrollBar();
			pScrollBar->SetValue( m_nScrollLine );			
		}

		m_bNeedDrawItem = true;
	}	
}

/**
* \brief 컴포넌트가 가지고 있는 가장 첫 줄로 간다.
*/
VOID COutputBox_Generic::FirstLine(VOID)
{
	if( m_nDrawItemNum > m_nRowLimit )
	{
		m_nScrollLine = 0;

		// ScrollBar 조절.
		if( m_dwStyle & COMP_VER_SCROLL )
		{
			gui::CScrollBar* pScrollBar = m_pOutputBox->GetVerticalScrollBar();
			pScrollBar->SetValue( m_nScrollLine );			
		}

		m_bNeedDrawItem = true;
	}	
}

/**
* \brief 컴포넌트가 출력하고 있는 가장 마지막 줄로 간다.
*/
VOID COutputBox_Generic::LastLine(VOID)
{
	if( (INT)m_listDrawItem.size() > m_nRowLimit )
	{
		m_nScrollLine = m_nDrawItemNum - m_nRowLimit;

		// ScrollBar 조절.
		if( m_dwStyle & COMP_VER_SCROLL )
		{
			gui::CScrollBar* pScrollBar = m_pOutputBox->GetVerticalScrollBar();
			pScrollBar->SetValue( m_nScrollLine );			
		}

		m_bNeedDrawItem = true;
	}	
}

void COutputBox_Generic::SetLine(int nLine)
{
	if (m_nDrawItemNum > nLine)
	{
		m_nScrollLine = nLine;

		// ScrollBar 조절.
		if (m_dwStyle & COMP_VER_SCROLL)
		{
			gui::CScrollBar* pScrollBar = m_pOutputBox->GetVerticalScrollBar();
			pScrollBar->SetValue(m_nScrollLine, FALSE);
		}

		m_bNeedDrawItem = true;
	}
}

VOID COutputBox_Generic::SetScrollBarLeft( bool bScrollBarLeft )
{
	m_bScrollbarLeftLine = bScrollBarLeft;
	
	VisibleLineCount();

	if( m_pOutputBox == NULL )
	{
		return;
	}

	CRectangle rect = m_pOutputBox->GetScreenRect();
	m_pOutputBox->ResizeScrollBar();

	INT nWidth = rect.GetWidth() - 2 * m_nMarginX + 7;
	INT nHeight = rect.GetHeight() - 2 * m_nMarginY;

	if( nWidth < 0 )
		nWidth = 1;

	if( nHeight < 0 )
		nHeight = 1;

	if( m_TextSurface.m_pTexture )
	{
		if( m_TextSurface.m_pTexture->GetWidth() < rect.GetWidth()  ||
			m_TextSurface.m_pTexture->GetHeight() < rect.GetHeight() ||
			m_TextSurface.m_pTexture->GetWidth()/2 > rect.GetWidth() ||
			m_TextSurface.m_pTexture->GetHeight()/2 > rect.GetHeight() )
		{
			g_TextureMgr.DeleteTexture( m_TextSurface.m_pTexture );
			m_TextSurface.m_pTexture = NULL;	
		}
	}

	if( m_TextSurface.m_pTexture == NULL )
	{
		m_TextSurface.m_pTexture = g_TextureMgr.CreateTexture( rect.GetWidth(), rect.GetHeight() );

		// 새로운 텍스쳐개 생성되었다면 다시 그려줘야 할 필요가 있다. 
		m_bNeedDrawItem = TRUE;
	}

	m_TextSurface.m_SnapShot.uRed = 255;
	m_TextSurface.m_SnapShot.uGreen = 255;
	m_TextSurface.m_SnapShot.uBlue = 255;
	m_TextSurface.m_SnapShot.uAlpha = 255;
	m_TextSurface.m_SnapShot.UVs[0] = 0;
	m_TextSurface.m_SnapShot.UVs[1] = 0;
	m_TextSurface.m_SnapShot.UVs[2] = 1;
	m_TextSurface.m_SnapShot.UVs[3] = 1;

	m_TextSurface.m_SnapShot.rtRect.left = rect.left/* + m_nMarginX*/;
	m_TextSurface.m_SnapShot.rtRect.right = rect.left + m_TextSurface.m_pTexture->GetWidth()/* + m_nMarginX*/; 
	m_TextSurface.m_SnapShot.rtRect.top = rect.top/* + m_nMarginY*/;
	m_TextSurface.m_SnapShot.rtRect.bottom = rect.top + m_TextSurface.m_pTexture->GetHeight()/* + m_nMarginY*/;

	std::list<COutputBox_Item*>::iterator itDraw = m_listDrawItem.begin();
	for( ; itDraw != m_listDrawItem.end(); ++itDraw )
	{
		(*itDraw)->Clear();
	}

	m_nDrawItemNum = 0;
	m_itDrawItem = m_listDrawItem.begin();

	gui::CScrollBar *pScrollBar = m_pOutputBox->GetVerticalScrollBar();
	m_nScrollLine = 0;
	pScrollBar->SetValue( 0 );

	// Crude Item에 이미 아이템들이 들어가 있다면 resize가 StringDetect() 가
	// 일어나기 전에 2번 이상 출력되었다는 것.
	if( m_listCrudeItem.empty() )
	{
		std::list<COutputBox_Item*>::iterator it = m_listOriginalItem.begin();
		for( ; it != m_listOriginalItem.end(); ++it )
		{
			AddCrudeItem( (*it) );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

/**
* \brief 가지고 있는 원래의 문자열을 가지고 현재의 컴포넌트 사이즈에 맞춰서 줄 바꿈 가공을 하고 렌더링될 자료구조에 넣어준다.
*/
VOID COutputBox_Generic::StringDetect( VOID )
{
	if( m_pOutputBox == NULL || m_pFont == NULL )
		return;

	CScrollBar* pScrollBar = m_pOutputBox->GetVerticalScrollBar();
	CRectangle rect = m_pOutputBox->GetClientRect();
	CRectangle rtScroll = pScrollBar->GetClientRect(); 

	if( !m_listCrudeItem.empty() )
	{
		std::list<COutputBox_Item*>::iterator it = m_listCrudeItem.begin();
		while( it != m_listCrudeItem.end() )
		{
			AddDrawItem( (*it) );

			COutputBox_Item* pItem = (COutputBox_Item*)(*it);

			NTL_DELETE( pItem );

			m_listCrudeItem.erase( it );
			it = m_listCrudeItem.begin();
			
		}
	}

	// Need DrawItem은 Scroll의 변화로 인한 Texture를 다시 그려야 하기 때문에 Scrollbar를 조정하지 않는다.
	if( m_bNeedDrawItem )
		return;

	//if( m_nDrawItemNum > m_nRowLimit )
	//{
	//	gui::CScrollBar *pScrollBar = m_pOutputBox->GetVerticalScrollBar();
	//	m_nScrollLine = m_nDrawItemNum - m_nRowLimit;

	//	if( m_dwStyle & COMP_VER_SCROLL )
	//	{
	//		pScrollBar->SetRange( 0, m_nScrollLine );
	//		pScrollBar->SetValue( m_nScrollLine, FALSE );
	//		if( m_bDynamicScrollBar )
	//			pScrollBar->Show( true );
	//	}
	//}
	//else
	//{
	//	if( ( m_dwStyle & COMP_VER_SCROLL ) )
	//	{
	//		if( m_bDynamicScrollBar )
	//			pScrollBar->Show( false );
	//		else
	//		{
	//			pScrollBar->SetRange( 0, 0 );
	//			pScrollBar->SetValue( 0, FALSE );
	//		}
	//	}
	//}
}

/**
* \brief 원래의 문자열을 저장한다.
*/
VOID COutputBox_Generic::AddOriginalItem( const WCHAR* text, DWORD dwTextColor, INT nEffectMode, DWORD dwEffectColor, INT nEffectValue )
{
	COutputBox_Item *pItem = NTL_NEW COutputBox_Item( text, dwTextColor, dwEffectColor, nEffectMode, nEffectValue, m_pFont, TRUE );
	if( (INT) m_listOriginalItem.size() >= m_nMaxLine )
	{
		std::list<COutputBox_Item*>::iterator it = m_listOriginalItem.begin();

		NTL_DELETE( *it );

		m_listOriginalItem.erase( it );
		it = m_listOriginalItem.begin();
	}

	m_listOriginalItem.push_back( pItem );
	AddCrudeItem( pItem );
}

/**
* \brief 줄바꿈 전의 그려질 문자열을 추가한다.
*/
VOID COutputBox_Generic::AddCrudeItem( COutputBox_Item* pItem )
{
	COutputBox_Item* pCrudeItem = NTL_NEW COutputBox_Item( (*pItem) );
	m_listCrudeItem.push_back( pCrudeItem );
}

/**
* \brief 그려질 문자열을 추가한다.
*/
VOID COutputBox_Generic::AddDrawItem( COutputBox_Item* pItem )
{
	if( m_pOutputBox == NULL || m_pFont == NULL || pItem == NULL  )
		return;

	CScrollBar *pScrollBar = m_pOutputBox->GetVerticalScrollBar();
	CRectangle rect = m_pOutputBox->GetClientRect();
	CRectangle rtScroll = pScrollBar->GetClientRect(); 

	INT nLimitW = rect.GetWidth() - m_nMarginX;
	if( !m_bScrollbarLeftLine )
		nLimitW -= rtScroll.GetWidth();

	INT nTextSize = pItem->m_pBuffer->GetTextSize(); 
	INT nXPos = 0, nNextXPos = 0, nPrevXPos = 0, nStartX = 0, nAllocSize = 0;
	INT i = 0;

	WCHAR InsertBuffer[GUI_TEXT_BUFFER_SIZE];

	for( i = 0; i < nTextSize; i++ )
	{
		BOOL bLineFeed = FALSE;

		nXPos = pItem->m_pBuffer->GetItemSizeByFirst( i );
		nNextXPos = pItem->m_pBuffer->GetItemSizeByFirst( i+1 );

		WCHAR wCurrentChar = STATICBOX_NULL_STRING;
		WCHAR wNextChar = STATICBOX_NULL_STRING;

		// 현재 글자가 '\\' 이고 다음 글자가 'n' 일 경우 현재 문자를 개행 문자로 처리한다. ( bLineFeed = TRUE )
		if( i < nTextSize - 1 )
		{
			wCurrentChar = (*pItem->m_pBuffer)[i];
			wNextChar = (*pItem->m_pBuffer)[i+1];

			if( wCurrentChar == L'\\' && wNextChar == L'n' )
				bLineFeed = TRUE;
		}

		// 다음 X 위치와 이전 X 위치가 제한 LimitW 를 넘어가거나 bLineFeed(줄넘김) flag가 true 일 경우 Queue에 아이템을
		// 넣는다.
		if( nNextXPos - nPrevXPos > nLimitW || bLineFeed )
		{
			/*if( (INT)m_listDrawItem.size() >= m_nMaxLine )
				PopItem();*/
			if( m_itDrawItem == m_listDrawItem.end() )
				PopItem();

			if( !bLineFeed )
			{
				if( wCurrentChar == STATICBOX_SYMBOL_CHARACTER_PERIOD )
					++i;
			}

			nAllocSize = i - nStartX;
			memset( InsertBuffer, 0, sizeof( WCHAR ) * ( nAllocSize + 1 ) );
			memcpy( InsertBuffer, pItem->m_pBuffer->GetBuffer() + nStartX, sizeof( WCHAR ) * nAllocSize );

			(*m_itDrawItem)->SetOutputBox_Item( InsertBuffer, pItem->m_pBuffer->GetSizeBuffer() + nStartX,
				pItem->m_pBuffer->GetCPtoXSizeBuffer() + nStartX, pItem->m_textcolor, pItem->m_effectcolor, pItem->m_nEffectMode,
				pItem->m_nEffectValue, m_pFont );
			m_itDrawItem++;

			if( m_nDrawItemNum < m_nMaxLine )
				++m_nDrawItemNum;

			// 줄넘길 일 경우
			if( bLineFeed )
			{
				INT nNextPos = i + 2;	// 개행 글자를 건너뛴 위치만큼 다음 위치를 잡아주고
				nStartX = nNextPos;
				if( nNextPos < nTextSize )
					nPrevXPos = pItem->m_pBuffer->GetItemSizeByFirst( nNextPos );
				++i; // 필요없는 루프를 줄임.
			}
			else
			{
				nStartX = i;
				nPrevXPos = nXPos;
			}			
		}
	}

	if( nStartX < i )
	{
		if( m_itDrawItem == m_listDrawItem.end() )
			PopItem();

		nAllocSize = i - nStartX;
		memset( InsertBuffer, 0, sizeof( WCHAR ) * ( nAllocSize + 1 ) );
		memcpy( InsertBuffer, pItem->m_pBuffer->GetBuffer() + nStartX, sizeof( WCHAR ) * nAllocSize );
		/*COutputBox_Item* pDrawItem = NTL_NEW COutputBox_Item( InsertBuffer, pItem->m_textcolor, 
			pItem->m_effectcolor, pItem->m_nEffectMode, pItem->m_nEffectValue, m_pFont );
		m_listDrawItem.push_back( pDrawItem ); 		*/

		(*m_itDrawItem)->SetOutputBox_Item( InsertBuffer, pItem->m_pBuffer->GetSizeBuffer() + nStartX,
			pItem->m_pBuffer->GetCPtoXSizeBuffer() + nStartX, pItem->m_textcolor, pItem->m_effectcolor, pItem->m_nEffectMode,
			pItem->m_nEffectValue, m_pFont );
		m_itDrawItem++;

		if( m_nDrawItemNum < m_nMaxLine )
			++m_nDrawItemNum;
	}

	if( m_nDrawItemNum > m_nRowLimit )
	{
		gui::CScrollBar *pScrollBar = m_pOutputBox->GetVerticalScrollBar();
		m_nScrollLine = m_nDrawItemNum - m_nRowLimit;

		if( m_dwStyle & COMP_VER_SCROLL )
		{
			pScrollBar->SetRange( 0, m_nScrollLine );
			pScrollBar->SetValue( m_nScrollLine, FALSE );
			if( m_bDynamicScrollBar )
				pScrollBar->Show( true );
		}
	}
	else
	{
		if( ( m_dwStyle & COMP_VER_SCROLL ) )
		{
			if( m_bDynamicScrollBar )
				pScrollBar->Show( false );
			else
			{
				pScrollBar->SetRange( 0, 0 );
				pScrollBar->SetValue( 0, FALSE );
			}
		}
	}
}

/**
* \brief 현재의 컴포넌트 사이즈에서 보여질 수 있는 아이템의 갯수를 센다.
*/
VOID COutputBox_Generic::VisibleLineCount(VOID)
{
	if( m_pFont == NULL || m_pOutputBox == NULL )
		return;
		
	INT nTextHeight = m_pFont->GetHeight();
	CRectangle rect = m_pOutputBox->GetScreenRect();

	m_nRowLimit = 0;
	INT nCurr = m_nMarginY * 2;
	while( 1 )
	{
		if( nCurr + nTextHeight > rect.GetHeight() )
			break;
		++m_nRowLimit;
		nCurr += ( m_nInterval + nTextHeight );
	}
}

VOID COutputBox_Generic::PopItem(VOID)
{
	std::list<COutputBox_Item*>::iterator it;
	it = m_listDrawItem.begin();
	if( it != m_listDrawItem.end() )
	{
		m_listDrawItem.push_back( (*it) );
		m_listDrawItem.erase( it );
		m_itDrawItem = m_listDrawItem.end();
		--m_itDrawItem;
		return;
	}
}

VOID COutputBox_Generic::DrawItem(VOID)
{
	if( m_listDrawItem.size() == 0 )
		return;
		
	if( m_TextSurface.m_pTexture )
		m_TextSurface.m_pTexture->FillClear(); 

	INT i = 0;
	INT nStartX = m_nMarginX;
	INT nStartY = m_nMarginY;
	INT nTextHeight = m_pFont->GetHeight();

	std::list<COutputBox_Item*>::iterator it = m_listDrawItem.begin();
	for( i = 0 ; i < m_nScrollLine ; ++i )
		++it;

	for( i = 0; i < m_nRowLimit ; ++i )
	{
		if( it == m_listDrawItem.end() )
			break;

		COutputBox_Item *pItem = (*it);
		
		CRectangle rect = m_pFont->TextOutW( pItem->m_pBuffer->GetBuffer() );   
		
		CRectangle rtBound;
		rtBound.SetRect( nStartX, nStartY, nStartX + rect.GetWidth(), nStartY + rect.GetHeight() );   
		g_FontMgr.BitmapToTexture( m_TextSurface.m_pTexture, rtBound, pItem->m_textcolor, pItem->m_nEffectMode, pItem->m_effectcolor, m_nTextEffectValue, m_nBkMode, m_dwBkColor );

		nStartY += ( nTextHeight + m_nInterval );
		++it;
	}
}

/////////////////////////////////////////////////////////////////////////////
// callback function

VOID COutputBox_Generic::OnSetOptions( const CComponentOptions& options )
{
	if( options.Exists( "maxline" ) )
	{
		m_nMaxLine = options.GetValueAsInt( "maxline" );
		m_pOutputBox->SetMaxLine( m_nMaxLine );
	}

	if( options.Exists( "margin_x" ) )
		m_nMarginX = options.GetValueAsInt( "margin_x" );
	if( options.Exists( "margin_y" ) )
		m_nMarginY = options.GetValueAsInt( "margin_y" );
	if( options.Exists( "interval" ) )
		m_nInterval = options.GetValueAsInt( "interval" );
	if( options.Exists( "vertical_scroll" ) )
	{
		if( options.GetValueAsBool( "vertical_scroll" ) )
			m_dwStyle |= COMP_VER_SCROLL;
	}
	if( options.Exists( "scrollbar_leftline" ) )
		m_bScrollbarLeftLine = options.GetValueAsBool( "scrollbar_leftline" );
	if( options.Exists( "scrollbar_dynamicshow" ) )
		m_bDynamicScrollBar = options.GetValueAsBool( "scrollbar_dynamicshow" );

		
	if( options.Exists( "font_name" ) )
		m_strFont = options.GetValue( "font_name" ); 
	
	if( options.Exists( "font_height" ) )
		m_nFontH = options.GetValueAsInt( "font_height"  );

	if( options.Exists( "font_attribute" ) )
	{
		INT nCount = options.Count( "font_attribute" );
		for( INT i = 0 ; i < nCount ; ++i )
			m_nFontAttribute |= options.GetValueAsInt( "font_attribute",i );
	}

	// font create
	CreateFont();

	// text attribute
	BYTE byRed = DEFAULT_TEXTCOLOR_RED;
	BYTE byGreen = DEFAULT_TEXTCOLOR_GREEN;
	BYTE byBlue = DEFAULT_TEXTCOLOR_BLUE;

	if( options.Exists( "text_color_red" ) )
		byRed = options.GetValueAsInt( "text_color_red" );
	if( options.Exists( "text_color_green" ) )
		byGreen = options.GetValueAsInt( "text_color_green" );
	if( options.Exists( "text_color_blue" ) )
		byBlue = options.GetValueAsInt( "text_color_blue" );

	m_dwTextColor = RGB( byRed,byGreen,byBlue );

	byRed = DEFAULT_BKCOLOR_RED;
	byGreen = DEFAULT_BKCOLOR_GREEN;
	byBlue = DEFAULT_BKCOLOR_BLUE;

	if( options.Exists( "text_bkcolor_red" ) )
		byRed = options.GetValueAsInt( "text_bkcolor_red" );
	if( options.Exists( "text_bkcolor_green" ) )
		byGreen = options.GetValueAsInt( "text_bkcolor_green" );
	if( options.Exists( "text_bkcolor_blue" ) )
		byBlue = options.GetValueAsInt( "text_bkcolor_blue" );

	m_dwBkColor = RGB( byRed,byGreen,byBlue );
	
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
			m_nTextEffectMode = TE_SHADOW;
			m_nTextEffectValue = DEFAULT_SHADOW_EFFECT_VALUE;
		}
		else if( effectmode == "outline" )
		{
			m_nTextEffectMode = TE_OUTLINE;
			m_nTextEffectValue = DEFAULT_OUTLINE_EFFECT_VALUE;
		}
	}
	if( options.Exists( "text_effect_value" ) )
	{
		m_nTextEffectValue = options.GetValueAsInt( "text_effect_value" );
	}
	
	m_dwTextEffectColor = RGB( byRed,byGreen,byBlue );
	
	// text
	if( options.Exists( "text" ) )
	{
		INT nTextCount = options.Count( "text" );
		for( INT i = 0 ; i < nTextCount ; ++i )
			AddText( options.GetValue( "text", i ).c_str(), m_dwTextColor, m_nTextEffectMode, m_dwTextEffectColor, m_nTextEffectValue ); 
	}

	// surface
	if( options.Exists( "surface_file" ) )
	{
		std::string file = options.GetValue( "surface_file" ); 
		CSurfaceManager* pSurfaceManager = m_pOutputBox->GetSurfaceManager();
		
		if( pSurfaceManager )
		{
			if( options.Exists( "surface" ) )
			{
				INT nCount = options.Count( "surface" );
				for( INT i = 0 ; i < nCount ; ++i )
					AddSurface( pSurfaceManager->GetSurface( file,options.GetValue( "surface",i ) ) ); 
			}
		}
	}

	Clear();
}

VOID COutputBox_Generic::OnScrollChange( INT nValue )
{
	gui::CScrollBar *pScrollBar = m_pOutputBox->GetVerticalScrollBar();

	INT nMax = pScrollBar->GetMaxValue();
	INT nSize = m_nDrawItemNum;
	INT nScrollLine = m_nDrawItemNum-m_nRowLimit-( nMax-nValue );
	if( nScrollLine == m_nScrollLine )
		return;

	m_nScrollLine = nScrollLine;

	m_bNeedDrawItem = true;
}

VOID COutputBox_Generic::OnMove( INT nOldX,INT nOldY )
{
	if( m_pOutputBox == NULL )
		return;
	
	CRectangle rtScreen,rtSnap;

	CSurface* pSurface;
	std::list<CSurface>::iterator the;

	for( the = m_stlSurface.begin() ; the != m_stlSurface.end() ; ++the )
	{
		pSurface = &(*the);
		rtScreen = m_pOutputBox->GetScreenRect();
		rtSnap = pSurface->m_Original.rtRect;

		pSurface->m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
		pSurface->m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
		pSurface->m_SnapShot.rtRect.right = pSurface->m_SnapShot.rtRect.left+rtSnap.GetWidth();
		pSurface->m_SnapShot.rtRect.bottom = pSurface->m_SnapShot.rtRect.top+rtSnap.GetHeight();
	}

	if( m_TextSurface.m_pTexture == NULL )
		return;
	if( m_pFont == NULL )
		return;

	CRectangle rect = m_pOutputBox->GetScreenRect();

	m_TextSurface.m_SnapShot.rtRect.left = rect.left;
	m_TextSurface.m_SnapShot.rtRect.right = rect.left+m_TextSurface.m_pTexture->GetWidth();  
	m_TextSurface.m_SnapShot.rtRect.top = rect.top;
	m_TextSurface.m_SnapShot.rtRect.bottom = rect.top+m_TextSurface.m_pTexture->GetHeight();  
}

VOID COutputBox_Generic::OnPaint(VOID)
{
	// Item Update
	if( !m_listCrudeItem.empty() || m_bNeedDrawItem )
	{
		StringDetect();

		DrawItem();

		m_bNeedDrawItem = false;
	}

	std::list<CSurface>::iterator the;

	for( the = m_stlSurface.begin() ; the != m_stlSurface.end() ; ++the )
	{
		g_GuiRenderer.RenderQueue( &(*the).m_SnapShot, (*the).m_pTexture );
	}

	g_GuiRenderer.RenderQueue( &m_TextSurface.m_SnapShot, m_TextSurface.m_pTexture );
}

VOID COutputBox_Generic::OnResize( INT nOldX, INT nOldY )
{
	VisibleLineCount();

	if( m_pOutputBox == NULL )
	{
		return;
	}

	CRectangle rect = m_pOutputBox->GetScreenRect();
	m_pOutputBox->ResizeScrollBar();

	INT nWidth = rect.GetWidth() - 2 * m_nMarginX + 7;
	INT nHeight = rect.GetHeight() - 2 * m_nMarginY;

	if( nWidth < 0 )
		nWidth = 1;

	if( nHeight < 0 )
		nHeight = 1;

	if( m_TextSurface.m_pTexture )
	{
		if( m_TextSurface.m_pTexture->GetWidth() < rect.GetWidth()  ||
			m_TextSurface.m_pTexture->GetHeight() < rect.GetHeight() ||
			m_TextSurface.m_pTexture->GetWidth()/2 > rect.GetWidth() ||
			m_TextSurface.m_pTexture->GetHeight()/2 > rect.GetHeight() )
		{
			g_TextureMgr.DeleteTexture( m_TextSurface.m_pTexture );
			m_TextSurface.m_pTexture = NULL;	
		}
	}

	if( m_TextSurface.m_pTexture == NULL )
	{
		m_TextSurface.m_pTexture = g_TextureMgr.CreateTexture( rect.GetWidth(), rect.GetHeight() );

		// 새로운 텍스쳐개 생성되었다면 다시 그려줘야 할 필요가 있다. 
		m_bNeedDrawItem = true;
	}
		
	m_TextSurface.m_SnapShot.uRed = 255;
	m_TextSurface.m_SnapShot.uGreen = 255;
	m_TextSurface.m_SnapShot.uBlue = 255;
	m_TextSurface.m_SnapShot.uAlpha = 255;
	m_TextSurface.m_SnapShot.UVs[0] = 0;
	m_TextSurface.m_SnapShot.UVs[1] = 0;
	m_TextSurface.m_SnapShot.UVs[2] = 1;
	m_TextSurface.m_SnapShot.UVs[3] = 1;

	m_TextSurface.m_SnapShot.rtRect.left = rect.left/* + m_nMarginX*/;
	m_TextSurface.m_SnapShot.rtRect.right = rect.left + m_TextSurface.m_pTexture->GetWidth()/* + m_nMarginX*/; 
	m_TextSurface.m_SnapShot.rtRect.top = rect.top/* + m_nMarginY*/;
	m_TextSurface.m_SnapShot.rtRect.bottom = rect.top + m_TextSurface.m_pTexture->GetHeight()/* + m_nMarginY*/;

	std::list<COutputBox_Item*>::iterator itDraw = m_listDrawItem.begin();
	for( ; itDraw != m_listDrawItem.end(); ++itDraw )
	{
		(*itDraw)->Clear();
	}

	m_nDrawItemNum = 0;
	m_itDrawItem = m_listDrawItem.begin();
	
	gui::CScrollBar *pScrollBar = m_pOutputBox->GetVerticalScrollBar();
	m_nScrollLine = 0;
	pScrollBar->SetValue( 0 );

	// Crude Item에 이미 아이템들이 들어가 있다면 resize가 StringDetect() 가
	// 일어나기 전에 2번 이상 출력되었다는 것.
	if( m_listCrudeItem.empty() )
	{
		std::list<COutputBox_Item*>::iterator it = m_listOriginalItem.begin();
		for( ; it != m_listOriginalItem.end(); ++it )
		{
			AddCrudeItem( (*it) );
		}
	}
}
END_GUI
