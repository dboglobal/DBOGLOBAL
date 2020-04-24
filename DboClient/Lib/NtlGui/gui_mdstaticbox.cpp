#include "gui_precomp.h"
#include "gui_mdstaticbox.h"
#include "gui_mdstaticbox_generic.h"

START_GUI

/////////////////////////////////////////////////////////////////////////////
// Construction & Destruction:

CMDStaticBox::CMDStaticBox( CComponent *pParent, CSurfaceManager* pSurfaceManager )
:CComponent( pParent, pSurfaceManager ), m_pImpl( NULL )
{
	m_pImpl = NTL_NEW CMDStaticBox_Generic( this );
}


CMDStaticBox::CMDStaticBox( const CRectangle& rtRect, CComponent* pParent, CSurfaceManager* pSurfaceManager )
:CComponent( rtRect, pParent, pSurfaceManager ), m_pImpl( NULL )
{
	m_pImpl = NTL_NEW CMDStaticBox_Generic( this );
}


CMDStaticBox::~CMDStaticBox()
{
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CMDStaticBox::SetDefaultTextStyle( DWORD dwStyle )
{
	m_pImpl->m_dwStyle = dwStyle;
}
	
VOID CMDStaticBox::SetDefaultTextColor( COLORREF color )
{
	m_pImpl->m_colorText = color;
}

VOID CMDStaticBox::SetDefaultEffectColor( COLORREF color )
{
	m_pImpl->m_colorEffect = color;
}

VOID CMDStaticBox::SetDefaultEffectMode( INT nMode )
{
	m_pImpl->m_nEffectMode = nMode;
}

VOID CMDStaticBox::SetDefaultEffectValue( INT nValue )
{
	m_pImpl->m_nEffectValue = nValue;
}

VOID CMDStaticBox::SetBkColor( COLORREF color, BOOL bRedraw /* = TRUE  */ )
{
	m_pImpl->m_colorBK = color;

	if( bRedraw )
		m_pImpl->DrawText();
}

VOID CMDStaticBox::SetBkMode( INT nMode, BOOL bRedraw /* = TRUE  */ )
{
	m_pImpl->m_nBkMode = nMode;

	if( bRedraw )
		m_pImpl->DrawText();
}

VOID CMDStaticBox::SetDefaultFont( DWORD dwFontIndex )
{
	if( dwFontIndex >= (INT)m_pImpl->m_vecFont.size() )
		return;

	m_pImpl->m_dwDefaultFont = dwFontIndex;
}

VOID CMDStaticBox::SetMinSize( INT nCX, INT nCY )
{
	m_pImpl->m_nMinCX = nCY;
	m_pImpl->m_nMinCY = nCY;
}

VOID CMDStaticBox::SetMarginX( INT nMarginX )
{
	m_pImpl->m_nMarginX = nMarginX;
	m_pImpl->m_nMaxWidth = GetWidth() - 2 * nMarginX;
}

VOID CMDStaticBox::SetMarginY( INT nMarginY )
{
	m_pImpl->m_nMarginY = nMarginY;
	m_pImpl->m_nMaxHeight = GetHeight() - 2 * nMarginY;
}

VOID CMDStaticBox::SetInterval( INT nInterval )
{
	m_pImpl->m_nInterval = nInterval;
}

VOID CMDStaticBox::SetBlankLine( INT nCY /* = -1  */)
{
	m_pImpl->SetBlankLine( nCY );
}

INT CMDStaticBox::AddFont( const CHAR* pFontName, INT nHeight, INT nAttributes /* = 0  */ )
{
	return m_pImpl->AddFont( pFontName , nHeight, nAttributes );
}

VOID CMDStaticBox::SetItem( const CHAR* szText, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, INT nX, BOOL bAttachPrevLine /* = FALSE */)
{
	SetItem( szText, szName, dwFontIndex, dwStyle, colorText, m_pImpl->m_colorEffect, m_pImpl->m_nEffectMode, m_pImpl->m_nEffectValue, nX, bAttachPrevLine );	
}

VOID CMDStaticBox::SetItem( const CHAR* szText, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue, INT nX /* = 0*/, BOOL bAttachPrevLine /* = FALSE*/ )
{
	INT nLen = (INT)strlen( szText );
	WCHAR pBuffer[GUI_TEXT_BUFFER_SIZE];

	::MultiByteToWideChar( GetACP(), 0, szText, -1, pBuffer, nLen + 1 );

	m_pImpl->SetItem( pBuffer, szName, dwFontIndex, dwStyle, colorText, colorEffect, nEffectMode, nEffectValue, nX, bAttachPrevLine );
}

VOID CMDStaticBox::SetItem( const WCHAR* szText, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, INT nX, BOOL bAttachPrevLine /* = FALSE */ )
{
	SetItem( szText, szName, dwFontIndex, dwStyle, colorText, m_pImpl->m_colorEffect, m_pImpl->m_nEffectMode, m_pImpl->m_nEffectValue, nX, bAttachPrevLine );	
}

VOID CMDStaticBox::SetItem( const WCHAR* szText, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue, INT nX /* = 0 */, BOOL bAttachPrevLine /* = FALSE  */ )
{
	m_pImpl->SetItem( szText, szName, dwFontIndex, dwStyle, colorText, colorEffect, nEffectMode, nEffectValue, nX, bAttachPrevLine );
}

VOID CMDStaticBox::SetItem( const INT nNumber, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, INT nX , BOOL bAttachPrevLine /* = FALSE */)
{
	SetItem( nNumber, szName, dwFontIndex, dwStyle, colorText, m_pImpl->m_colorEffect, m_pImpl->m_nEffectMode, m_pImpl->m_nEffectValue, nX, bAttachPrevLine );
}

VOID CMDStaticBox::SetItem( const INT nNumber, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue, INT nX /* = 0 */, BOOL bAttachPrevLine /* = FALSE  */ )
{
	CHAR text[GUI_NUMBER_BUFFER_SIZE];
	memset( text, 0, GUI_NUMBER_BUFFER_SIZE );
	
#if defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
	sprintf_s( text, GUI_NUMBER_BUFFER_SIZE, "%d", nNumber );
#else
	sprintf( text, "%d", number );
#endif

	SetItem( text, szName, dwFontIndex, dwStyle, colorText, colorEffect, nEffectMode, nEffectValue, nX, bAttachPrevLine );
}

VOID CMDStaticBox::Format( const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, INT nX, BOOL bAttachPrevLine, const CHAR* format, ... )
{
	CHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined(_MSC_VER) && ( _MSC_VER >= 1400 )
	vsprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vsprintf( text, format, args );
#endif
	va_end( args );

	SetItem( text, szName, dwFontIndex, dwStyle, colorText, nX, bAttachPrevLine );
}

VOID CMDStaticBox::Format( const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue, INT nX, BOOL bAttachPrevLine, const CHAR* format, ... )
{
	CHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined(_MSC_VER) && ( _MSC_VER >= 1400 )
	vsprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vsprintf( text, format, args );
#endif
	va_end( args );

	SetItem( text, szName, dwFontIndex, dwStyle, colorText, colorEffect, nEffectMode, nEffectValue, nX, bAttachPrevLine );
}


VOID CMDStaticBox::Format( const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, INT nX, BOOL bAttachPrevLine, const WCHAR* format, ... )
{
	WCHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined(_MSC_VER) && ( _MSC_VER >= 1400 )
	vswprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vswprintf( text, format, args );
#endif
	va_end( args );

	SetItem( text, szName, dwFontIndex, dwStyle, colorText, nX, bAttachPrevLine );
}

VOID CMDStaticBox::Format( const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue, INT nX, BOOL bAttachPrevLine, const WCHAR* format, ... )
{
	WCHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined(_MSC_VER) && ( _MSC_VER >= 1400 )
	vswprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vswprintf( text, format, args );
#endif
	va_end( args );

	SetItem( text, szName, dwFontIndex, dwStyle, colorText, colorEffect, nEffectMode, nEffectValue, nX, bAttachPrevLine );
}

VOID CMDStaticBox::SetItemXY( const CHAR* szText, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText )
{
	SetItemXY( szText, szName, dwFontIndex, nX, nY, dwStyle, colorText, m_pImpl->m_colorEffect, m_pImpl->m_nEffectMode, m_pImpl->m_nEffectValue );
}

VOID CMDStaticBox::SetItemXY( const CHAR* szText, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue )
{
	INT nLen = (INT)strlen( szText );
	WCHAR pBuffer[GUI_TEXT_BUFFER_SIZE];

	::MultiByteToWideChar( GetACP(), 0, szText, -1, pBuffer, nLen + 1 );

	m_pImpl->SetItemXY( pBuffer, szName, dwFontIndex, nX, nY, dwStyle, colorText, colorEffect, nEffectMode, nEffectValue );
}

VOID CMDStaticBox::SetItemXY( const WCHAR* szText, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText )
{
	SetItemXY( szText, szName, dwFontIndex, nX, nY, dwStyle, colorText, m_pImpl->m_colorEffect, m_pImpl->m_nEffectMode, m_pImpl->m_nEffectValue );
}

VOID CMDStaticBox::SetItemXY( const WCHAR* szText, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue )
{
	m_pImpl->SetItemXY( szText, szName, dwFontIndex, nX, nY, dwStyle, colorText, colorEffect, nEffectMode, nEffectValue );
}

VOID CMDStaticBox::SetItemXY( const INT nNumber, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText )
{
	SetItemXY( nNumber, szName, dwFontIndex, nX, nY, dwStyle, colorText, m_pImpl->m_colorEffect, m_pImpl->m_nEffectMode, m_pImpl->m_nEffectValue );
}

VOID CMDStaticBox::SetItemXY( const INT nNumber, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue )
{
	CHAR text[GUI_NUMBER_BUFFER_SIZE];
	memset( text, 0, GUI_NUMBER_BUFFER_SIZE );

#if defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
	sprintf_s( text, GUI_NUMBER_BUFFER_SIZE, "%d", nNumber );
#else
	sprintf( text, "%d", number );
#endif

	SetItemXY( text, szName, dwFontIndex, nX, nY, dwStyle, colorText, colorEffect, nEffectMode, nEffectValue );
}

VOID CMDStaticBox::FormatXY( const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, const CHAR* format, ... )
{
	CHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined(_MSC_VER) && ( _MSC_VER >= 1400 )
	vsprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vsprintf( text, format, args );
#endif
	va_end( args );

	SetItemXY( text, szName, dwFontIndex, nX, nY, dwStyle, colorText );
}

VOID CMDStaticBox::FormatXY( const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue, const CHAR* format, ... )
{
	CHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined(_MSC_VER) && ( _MSC_VER >= 1400 )
	vsprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vsprintf( text, format, args );
#endif
	va_end( args );

	SetItemXY( text, szName, dwFontIndex, nX, nY, dwStyle, colorText, colorEffect, nEffectMode, nEffectValue );
}

VOID CMDStaticBox::FormatXY( const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, const WCHAR* format, ... )
{
	WCHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined(_MSC_VER) && ( _MSC_VER >= 1400 )
	vswprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vswprintf( text, format, args );
#endif
	va_end( args );

	SetItemXY( text, szName, dwFontIndex, nX, nY, dwStyle, colorText );
}

VOID CMDStaticBox::FormatXY( const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue, const WCHAR* format, ... )
{
	WCHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined(_MSC_VER) && ( _MSC_VER >= 1400 )
	vswprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vswprintf( text, format, args );
#endif
	va_end( args );

	SetItemXY( text, szName, dwFontIndex, nX, nY, dwStyle, colorText, colorEffect, nEffectMode, nEffectValue );
}

VOID CMDStaticBox::DeleteItem( const CHAR* szName )
{
	m_pImpl->DeleteItem( szName );
}

VOID CMDStaticBox::Clear(VOID)
{
	m_pImpl->ClearItem();
}

VOID CMDStaticBox::DrawItem(VOID)
{
	m_pImpl->DrawText();
}

VOID CMDStaticBox::AddSurface( CSurface surface )
{
	m_pImpl->AddSurface( surface );
}

END_GUI