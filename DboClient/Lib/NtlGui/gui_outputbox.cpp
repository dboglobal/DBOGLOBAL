#include "gui_precomp.h"
#include "gui_outputbox.h"
#include "gui_outputbox_generic.h"
#include "gui_renderer.h"

START_GUI

/////////////////////////////////////////////////////////////////////////////
// Construction:

COutputBox::COutputBox( CComponent* pParent, CSurfaceManager* pSurfaceManager, DWORD dwStyle /* = 0 */)
:CComponent( pParent, pSurfaceManager ), m_pImpl( NULL ),m_pSBVer( NULL )
{
	m_pSBVer = NTL_NEW CScrollBar( this, pSurfaceManager );
	m_pSBVer->Show( false );
	
	m_pImpl = NTL_NEW COutputBox_Generic( this, 50, 0, 0, false, false, dwStyle );

	m_SlotSetOptions = SigSetOptions().Connect( this, &COutputBox::OnSetOptions );
}

COutputBox::COutputBox( const CRectangle& rtRect, CComponent* pParent, CSurfaceManager* pSurfaceManager, 
					    INT nMarginX /* = 0 */, INT nMarginY /* = 0 */, INT nScrollSize /* = OUTPUTBOX_SCROLLBAR_SIZE */,
						bool bDynamicScrollBar /* = false */, bool bScrollBarLeft /* = false */, DWORD dwStyle /* = 0  */)
:CComponent( rtRect, pParent,pSurfaceManager ), m_pImpl( NULL ),m_pSBVer( NULL )
{
	CRectangle rtScroll;
	CalcSubRect( &rtScroll, nScrollSize, bScrollBarLeft );

	m_pSBVer = NTL_NEW CScrollBar( rtScroll, 0, 0, 0, this, pSurfaceManager );
	
	if( !bDynamicScrollBar )
	{
		m_pSBVer->SetRange( 0, 0 );
		m_pSBVer->Show( true );		
	}
	else
		m_pSBVer->Show( false );
	
	m_pImpl = NTL_NEW COutputBox_Generic( this, 100, nMarginX, nMarginY,  bDynamicScrollBar, bScrollBarLeft, dwStyle );
	
	if( bScrollBarLeft )
		m_pImpl->m_nMarginX += nScrollSize;
	
	m_SlotSetOptions = SigSetOptions().Connect( this, &COutputBox::OnSetOptions );
}


COutputBox::~COutputBox(VOID)
{
	NTL_DELETE( m_pSBVer );
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

INT COutputBox::GetMaxLine(VOID) const
{
	return m_pImpl->m_nMaxLine;
}

INT COutputBox::GetCurrentLineCount(VOID) const
{
	return (INT)m_pImpl->m_listDrawItem.size();
}

INT COutputBox::GetRowIndexOfTopLine(VOID) const
{
	return m_pImpl->m_nScrollLine;
}

INT COutputBox::GetVisibleLineCount(VOID) const
{
	return m_pImpl->m_nRowLimit;
}

std::list<CSurface>* COutputBox::GetSurface(VOID) const
{
	return &m_pImpl->m_stlSurface;
}

CScrollBar* COutputBox::GetVerticalScrollBar(VOID) const
{
	return m_pSBVer;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

VOID COutputBox::CreateFontStd( CHAR *pFaceName, INT nHeight, INT nAttributes /*= 0 */ )
{
	m_pImpl->CreateFont( pFaceName,nHeight,nAttributes );
}

VOID COutputBox::AddText( const CHAR *text )
{
	m_pImpl->AddText( text, m_pImpl->m_dwTextColor, m_pImpl->m_nTextEffectMode, m_pImpl->m_dwTextEffectColor, m_pImpl->m_nTextEffectValue );
}

VOID COutputBox::AddText( const CHAR *text, COLORREF textcolor )
{
	m_pImpl->AddText( text, textcolor, m_pImpl->m_nTextEffectMode, m_pImpl->m_dwTextEffectColor, m_pImpl->m_nTextEffectValue );
}

VOID COutputBox::AddText( const CHAR* text, COLORREF textcolor, INT nEffectMode, COLORREF effectcolor, INT nEffectValue )
{
	m_pImpl->AddText( text, textcolor, nEffectMode, effectcolor, nEffectValue );
}

VOID COutputBox::AddText( const WCHAR *text )
{
	m_pImpl->AddText( text, m_pImpl->m_dwTextColor, m_pImpl->m_nTextEffectMode, m_pImpl->m_dwTextEffectColor, m_pImpl->m_nTextEffectValue );
}

VOID COutputBox::AddText( const WCHAR *text, COLORREF textcolor )
{
	m_pImpl->AddText( text, textcolor, m_pImpl->m_nTextEffectMode, m_pImpl->m_dwTextEffectColor, m_pImpl->m_nTextEffectValue );
}

VOID COutputBox::AddText( const WCHAR* text, COLORREF textcolor, INT nEffectMode, COLORREF effectcolor, INT nEffectValue )
{
	m_pImpl->AddText( text, textcolor, nEffectMode, effectcolor, nEffectValue );
}

VOID COutputBox::AddText( INT number )
{
	AddText( number, m_pImpl->m_dwTextColor, m_pImpl->m_nTextEffectMode, m_pImpl->m_dwTextEffectColor, m_pImpl->m_nTextEffectValue );
}

VOID COutputBox::AddText( INT number, COLORREF textcolor )
{
	AddText( number, textcolor, m_pImpl->m_nTextEffectMode, m_pImpl->m_dwTextEffectColor, m_pImpl->m_nTextEffectValue );
}

VOID COutputBox::AddText( INT number, COLORREF textcolor, INT nEffectMode, COLORREF effectcolor, INT nEffectValue )
{
	CHAR temp[GUI_NUMBER_BUFFER_SIZE];
	memset( temp, 0, GUI_NUMBER_BUFFER_SIZE );

#if defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
	sprintf_s( temp, GUI_NUMBER_BUFFER_SIZE, "%d", number );
#else
	sprintf( temp, "%d", number );
#endif

	m_pImpl->AddText( temp, textcolor, nEffectMode, effectcolor, nEffectValue );
}

VOID COutputBox::Format( CHAR *format, ... )
{
	CHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
	vsprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vsprintf( text, format, args );
#endif
	va_end( args );

	m_pImpl->AddText( text, m_pImpl->m_dwTextColor, m_pImpl->m_nTextEffectMode, m_pImpl->m_dwTextEffectColor, m_pImpl->m_nTextEffectValue );
}

VOID COutputBox::Format( COLORREF textcolor, CHAR *format,  ... )
{
	CHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
	vsprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vsprintf( text, format, args );
#endif
	va_end( args );

	m_pImpl->AddText( text, textcolor, m_pImpl->m_nTextEffectMode, m_pImpl->m_dwTextEffectColor, m_pImpl->m_nTextEffectValue );
}

VOID COutputBox::Format( COLORREF textcolor, INT nEffectMode, COLORREF effectcolor, INT nEffectValue, CHAR* format, ... )
{
	CHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
	vsprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vsprintf( text, format, args );
#endif
	va_end( args );

	m_pImpl->AddText( text, textcolor, nEffectMode, effectcolor, nEffectValue );
}

VOID COutputBox::Format( WCHAR *format, ... )
{
	WCHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
	vswprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vswprintf( text, format, args );
#endif
	va_end( args );

	m_pImpl->AddText( text, m_pImpl->m_dwTextColor, m_pImpl->m_nTextEffectMode, m_pImpl->m_dwTextEffectColor, m_pImpl->m_nTextEffectValue );
}

VOID COutputBox::Format( COLORREF textcolor, WCHAR *format,  ... )
{
	WCHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
	vswprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vswprintf( text, format, args );
#endif
	va_end( args );

	m_pImpl->AddText( text, textcolor, m_pImpl->m_nTextEffectMode, m_pImpl->m_dwTextEffectColor, m_pImpl->m_nTextEffectValue );
}

VOID COutputBox::Format( COLORREF textcolor, INT nEffectMode, COLORREF effectcolor, INT nEffectValue, WCHAR* format, ... )
{
	WCHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
	vswprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vswprintf( text, format, args );
#endif
	va_end( args );

	m_pImpl->AddText( text, textcolor, nEffectMode, effectcolor, nEffectValue );
}

VOID COutputBox::SetMaxLine( INT nLine )
{	
	m_pImpl->m_nMaxLine = nLine;
}

VOID COutputBox::SetMarginY (INT nMarginY )
{
	m_pImpl->m_nMarginY = nMarginY;
	m_pImpl->VisibleLineCount();
}

VOID COutputBox::SetMarginX( INT nMarginX )
{
	m_pImpl->m_nMarginX = nMarginX;

	// peessi : 문제 있는 코드. 오른쪽도 덩달아 줄어버린다. 
	if( m_pImpl->m_bScrollbarLeftLine )
		m_pImpl->m_nMarginX += m_pSBVer->GetWidth();
}

VOID COutputBox::SetLineSpace( INT nLineSpace )
{
	m_pImpl->m_nInterval = nLineSpace;
	//m_pImpl->VisibleLineCount();
}

VOID COutputBox::SetTextColor( COLORREF color )
{
	m_pImpl->m_dwTextColor = color;
}

VOID COutputBox::SetBkColor( COLORREF color )
{
	m_pImpl->m_dwBkColor = color;
}

VOID COutputBox::SetTextEffectColor( COLORREF color )
{
	m_pImpl->m_dwTextEffectColor = color;
}

COLORREF COutputBox::GetTextColor(VOID)
{
	return m_pImpl->m_dwTextColor;
}

COLORREF COutputBox::GetBkColor(VOID)
{
	return m_pImpl->m_dwBkColor;
}

COLORREF COutputBox::GetTextEffectColor(VOID)
{
	return m_pImpl->m_dwTextEffectColor;
}

VOID COutputBox::SetBkMode( INT nBkMode )
{
	m_pImpl->m_nBkMode = nBkMode;
}

VOID COutputBox::SetTextEffectMode( INT nMode )
{
	m_pImpl->m_nTextEffectMode = nMode;
}

VOID COutputBox::Clear(VOID)
{
	m_pImpl->Clear();
}

VOID COutputBox::AddSurface( CSurface surface )
{
	m_pImpl->AddSurface( surface );
}

VOID COutputBox::ShowScrollBar( bool bShow )
{
	m_pSBVer->Show( bShow );
}

VOID COutputBox::FirstLine(VOID)
{
	m_pImpl->FirstLine();
}

VOID COutputBox::PrevLine(VOID)
{
	m_pImpl->PrevLine();
}

VOID COutputBox::NextLine(VOID)
{
	m_pImpl->NextLine();
}

VOID COutputBox::LastLine(VOID)
{
	m_pImpl->LastLine();
}

void COutputBox::SetLine(int nLine)
{
	m_pImpl->SetLine(nLine);
}

VOID COutputBox::SetScrollBarLeftLine( bool bScrollBarLeft )
{
	m_pImpl->SetScrollBarLeft( bScrollBarLeft );
}

/////////////////////////////////////////////////////////////////////////////
// Implementations:

VOID COutputBox::CalcSubRect( CRectangle* pSBRect, INT nScrollSize, INT bScrollBarLeft )
{
	*pSBRect = GetClientRect();
	
	if( bScrollBarLeft )
		pSBRect->right = pSBRect->left + nScrollSize;
	else
		pSBRect->left = pSBRect->right - nScrollSize;
}

VOID COutputBox::CalcSubRect( CRectangle* pSBRect, INT nScrollSize, INT nScrollMarginTop, INT nScrollMarginBottom, INT bScrollBarLeft )
{
	*pSBRect = GetClientRect();

	if( bScrollBarLeft )
		pSBRect->right = pSBRect->left + nScrollSize;
	else
		pSBRect->left = pSBRect->right - nScrollSize;

	pSBRect->top = pSBRect->top + nScrollMarginTop;
	pSBRect->bottom = pSBRect->bottom - nScrollMarginBottom;
}

/////////////////////////////////////////////////////////////////////////////
// callback functions:

VOID COutputBox::OnSetOptions( const CComponentOptions& options )
{
	INT nScrollBarWidth = 0;
	CComponentOptions suboptions = options;

	suboptions.RemoveOption( "x" );
	suboptions.RemoveOption( "y" );
	suboptions.RemoveOption( "width" );
	suboptions.RemoveOption( "height" );
	suboptions.RemoveOption( "surface" );

	if( options.Exists( "scrollbar_width" ) )
		nScrollBarWidth = suboptions.GetValueAsInt( "scrollbar_width" );
	else
		nScrollBarWidth = OUTPUTBOX_SCROLLBAR_SIZE;

	m_pImpl->m_nScrollBarWidth = nScrollBarWidth;

	if( !options.Exists( "slider_height" ) )
		suboptions.AddOption( "slider_height", nScrollBarWidth );
	if( !options.Exists( "slider_width" ) )
		suboptions.AddOption( "slider_width", nScrollBarWidth );

	CRectangle rtScrollBar;
	INT nScrollBarMarginTop = 0;
	INT nScrollBarMarginBottom = 0;

	if( options.Exists( "scrollbar_margin_top" ) )
		nScrollBarMarginTop = options.GetValueAsInt( "scrollbar_margin_top" );

	if( options.Exists( "scrollbar_margin_bottom" ) )
		nScrollBarMarginBottom = options.GetValueAsInt( "scrollbar_margin_bottom" );

	m_pImpl->m_nScrollBarMarginTop = nScrollBarMarginTop;
	m_pImpl->m_nScrollBarMarginBottom = nScrollBarMarginBottom;
	
	/*CalcSubRect( &rtScrollBar, nScrollBarWidth, m_pImpl->m_bScrollbarLeftLine );*/
	CalcSubRect( &rtScrollBar, nScrollBarWidth, nScrollBarMarginTop, nScrollBarMarginBottom, m_pImpl->m_bScrollbarLeftLine );
		
	suboptions.AddOption( "x", rtScrollBar.left );
	suboptions.AddOption( "y", rtScrollBar.top );
	suboptions.AddOption( "width", rtScrollBar.GetWidth() );
	suboptions.AddOption( "height", rtScrollBar.GetHeight() );

	m_pSBVer->SigSetOptions()( suboptions );

	// 여백조정.
	if( m_pImpl->m_bScrollbarLeftLine )
		m_pImpl->m_nMarginX += nScrollBarWidth;

	// scrollbar setting
	if( !m_pImpl->m_bDynamicScrollBar && ( m_pImpl->m_dwStyle & COMP_VER_SCROLL ) )
	{
		m_pSBVer->SetRange( 0, 0 );
		m_pSBVer->SetValue( 0 );
		m_pSBVer->Show( true );
	}
}

VOID COutputBox::ResizeScrollBar()
{
	CRectangle rtScroll(0,0,0,0);
	/*CalcSubRect( &rtScrollBar, nScrollBarWidth, m_pImpl->m_bScrollbarLeftLine );*/
	CalcSubRect( &rtScroll, m_pImpl->m_nScrollBarWidth, 
		m_pImpl->m_nScrollBarMarginTop, 
		m_pImpl->m_nScrollBarMarginBottom,
		m_pImpl->m_bScrollbarLeftLine );

	m_pSBVer->SetPosition( rtScroll );
	m_pSBVer->ResizeButton();
	m_pSBVer->ResizeSurfaceLayout();
	m_pSBVer->ResizeSurfaceSlider();
}


END_GUI

