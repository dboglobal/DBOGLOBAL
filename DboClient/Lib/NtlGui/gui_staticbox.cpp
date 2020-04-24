#include "gui_precomp.h"
#include "gui_staticbox.h"
#include "gui_staticbox_generic.h"
#include "gui_renderer.h"

START_GUI

/////////////////////////////////////////////////////////////////////////////
// Construction:

CStaticBox::CStaticBox(	CComponent* pParent, CSurfaceManager* pSurfaceManager, DWORD dwStyle/* =0 */, BOOL bDynamic /* = FALSE  */)
: CComponent( pParent, pSurfaceManager ), m_pImpl( NULL )
{
	m_pImpl = NTL_NEW CStaticBox_Generic( this, dwStyle, bDynamic );

	m_SlotSetOptions = SigSetOptions().Connect( this, &CStaticBox::OnSetOptions );
	m_SlotPaint = SigPaint().Connect( this, &CStaticBox::OnPaint );
	m_SlotMove = SigMove().Connect( this, &CStaticBox::OnMove );	
}

CStaticBox::CStaticBox( const CRectangle& ptRect, CComponent *pParent, CSurfaceManager* pSurfaceManager, DWORD dwStyle,	BOOL bDynamic /* = FALSE */ )
: CComponent( ptRect, pParent, pSurfaceManager ), m_pImpl( NULL )
{
	m_pImpl = NTL_NEW CStaticBox_Generic( this, dwStyle, bDynamic );
	m_pImpl->CreateFont();
		
	m_SlotSetOptions = SigSetOptions().Connect( this, &CStaticBox::OnSetOptions );
	m_SlotPaint = SigPaint().Connect( this, &CStaticBox::OnPaint );
	m_SlotMove = SigMove().Connect( this, &CStaticBox::OnMove );
}

CStaticBox::~CStaticBox(VOID)
{
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

std::list<CSurface>* CStaticBox::GetSurface(VOID) const
{
	return &m_pImpl->m_stlSurface;
}

std::wstring CStaticBox::GetText(VOID) const
{
	std::wstring strText = m_pImpl->m_pBuffer->GetBuffer();
	return strText;
}

INT CStaticBox::GetFitWidthToString(VOID)
{
	return m_pImpl->GetFitWidthToString();
}

INT CStaticBox::GetFitHeightToString( VOID )
{
	return m_pImpl->GetFitHeightToString();
}

INT CStaticBox::GetLineCount(VOID)
{
	return (INT)m_pImpl->m_listItem.size();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CStaticBox::CreateFontStd( CHAR* pFaceName, INT nHeight, INT nAttributes )
{
	m_pImpl->m_strFont = pFaceName;
	m_pImpl->m_nFontH = nHeight;
	m_pImpl->m_nFontAttribute = nAttributes;
	m_pImpl->CreateFont( pFaceName, nHeight, nAttributes );
}

VOID CStaticBox::SetText( const CHAR* text )
{
	m_pImpl->SetText( text );
}

VOID CStaticBox::SetText( const WCHAR* text )
{
	m_pImpl->SetText( text ); 
}

VOID CStaticBox::SetText( int number )
{
	CHAR temp[GUI_NUMBER_BUFFER_SIZE];
	memset( temp, 0, GUI_NUMBER_BUFFER_SIZE );
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s( temp, GUI_NUMBER_BUFFER_SIZE, "%d", number );
#else
	sprintf( temp, "%d", number );
#endif

	m_pImpl->SetText( temp );
}

VOID CStaticBox::SetText( unsigned int number )
{
	CHAR temp[GUI_NUMBER_BUFFER_SIZE];
	memset( temp, 0, GUI_NUMBER_BUFFER_SIZE );
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s( temp, GUI_NUMBER_BUFFER_SIZE, "%u", number );
#else
	sprintf( temp, "%u", number );
#endif

	m_pImpl->SetText( temp );
}

VOID CStaticBox::AddText( const CHAR* text ) 	
{
	m_pImpl->AddText(text);
}

VOID CStaticBox::AddText( const WCHAR* text )
{
	m_pImpl->AddText(text); 
}

VOID CStaticBox::AddText( int number )
{
	CHAR temp[GUI_NUMBER_BUFFER_SIZE];
	memset( temp, 0, GUI_NUMBER_BUFFER_SIZE );

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s( temp, GUI_NUMBER_BUFFER_SIZE, "%d", number );
#else
	sprintf( temp, "%d", number );
#endif

	m_pImpl->AddText( temp );
}

VOID CStaticBox::AddText( unsigned int number )
{
	CHAR temp[GUI_NUMBER_BUFFER_SIZE];
	memset( temp, 0, GUI_NUMBER_BUFFER_SIZE );

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s( temp, GUI_NUMBER_BUFFER_SIZE, "%u", number );
#else
	sprintf( temp,"%u",number );
#endif

	m_pImpl->AddText( temp );
}

VOID CStaticBox::Format( const CHAR* format, ... )
{
	CHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	vsprintf_s(text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vsprintf( text, format, args );
#endif
	va_end( args );

	m_pImpl->SetText( text );
}

VOID CStaticBox::AddFormat( const CHAR* format, ... )
{
	char text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	vsprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vsprintf( text, format, args );
#endif
	va_end(args);

	m_pImpl->AddText( text );
}

VOID CStaticBox::Format( const WCHAR* format, ... )
{
	WCHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format ); 
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	vswprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vswprintf( text, format, args );	
#endif
	va_end( args );

	m_pImpl->SetText( text );
}

VOID CStaticBox::AddFormat( const WCHAR* format, ... )
{
	WCHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	vswprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vswprintf( text, format, args );
#endif
	va_end( args );

	m_pImpl->AddText( text );
}

VOID CStaticBox::SetTextColor( COLORREF color, BOOL bRedraw /* = FALSE  */ )
{
	m_pImpl->m_Color = color;

	if( bRedraw )
		m_pImpl->DrawItem();
}

VOID CStaticBox::SetTextColor( COLORREF color, COLORREF bkcolor, INT nBkMode, BOOL bRedraw /* = FALSE  */ )
{
	m_pImpl->m_Color = color;
	m_pImpl->m_BkColor = bkcolor;
	m_pImpl->m_nBkMode = nBkMode;

	if( bRedraw )
		m_pImpl->DrawItem(); 
}

VOID CStaticBox::SetBkColor( COLORREF bkcolor, BOOL bRedraw /* = FALSE  */ )
{
	m_pImpl->m_BkColor = bkcolor;

	if( bRedraw )
		m_pImpl->DrawItem();
}

VOID CStaticBox::SetBkMode( int nBkMode, BOOL bRedraw /* = FALSE  */ )
{
	m_pImpl->m_nBkMode = nBkMode;

	if( bRedraw )
		m_pImpl->DrawItem();
}

VOID CStaticBox::SetTextAlpha( BYTE ucAlpha, BOOL bRedraw /* = FALSE  */)
{
	m_pImpl->m_TextSurface.m_SnapShot.uAlpha = ucAlpha;

	if( bRedraw )
		m_pImpl->DrawItem();
}

VOID CStaticBox::SetTextAlphaByRatio( BYTE ucAlpha, BOOL bRedraw /* = FALSE  */ )
{
	m_pImpl->m_TextSurface.m_SnapShot.uAlpha = (BYTE)( m_pImpl->m_ucAlpha / 255.0f * ucAlpha );

	if( bRedraw )
		m_pImpl->DrawItem();
}

VOID CStaticBox::SetEffectMode( INT nMode, BOOL bRedraw /* = FALSE  */)
{
	m_pImpl->SetEffectMode( nMode );

	if( bRedraw )
		m_pImpl->DrawItem(); 
}

VOID CStaticBox::SetEffectColor( COLORREF color, BOOL bRedraw /* = FALSE  */ )
{
	m_pImpl->m_effectColor = color;

	if( bRedraw )
		m_pImpl->DrawItem(); 
}

VOID CStaticBox::SetEffectValue( INT nValue, BOOL bRedraw /* = FALSE  */ )
{
	m_pImpl->SetEffectValue( nValue );

	if( bRedraw )
		m_pImpl->DrawItem();
}

VOID CStaticBox::SetTextStyle( DWORD dwStyle, BOOL bRedraw /* = FALSE  */)
{
	m_pImpl->m_dwStyle = dwStyle;

	if( bRedraw )
		m_pImpl->DrawItem();
}

VOID CStaticBox::SetMargin( INT nMarginX, INT nMarginY, BOOL bRedraw /* = FALSE  */)
{
	m_pImpl->SetMargin( nMarginX, nMarginY );

	if( bRedraw )
		m_pImpl->DrawItem();
}

VOID CStaticBox::SetLineSpace( INT nLineSpace, BOOL bRedraw /* = FALSE  */ )
{
	m_pImpl->m_nLineSpace = nLineSpace;

	if( bRedraw )
		m_pImpl->DrawItem();
}

VOID CStaticBox::SetDynamicMaxSize( INT nCX, INT nCY )
{
	if( m_pImpl->m_bDynamic )
	{
		m_pImpl->m_nMaxCX = nCX;
		m_pImpl->m_nMaxCY = nCY;
		m_pImpl->SetTextSurface( GetScreenRect() );		
	}
}

VOID CStaticBox::SetDynamicMinSize( INT nCX, INT nCY )
{
	if( m_pImpl->m_bDynamic )
	{
		if( m_pImpl->m_nMaxCX > nCX &&
			m_pImpl->m_nMaxCY > nCY )
		{
			m_pImpl->m_nMinCX = nCX;
			m_pImpl->m_nMinCY = nCY;
		}		
	}
}

VOID CStaticBox::Redraw(VOID)
{
	m_pImpl->Redraw();
}

VOID CStaticBox::Clear(VOID)
{
	m_pImpl->Clear();
}

VOID CStaticBox::AddSurface(CSurface surface)
{
	CRectangle rtSnap = surface.m_Original.rtRect;
	CRectangle rtScreen = GetScreenRect(); 
	
	surface.m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left+rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top+rtSnap.GetHeight();

	m_pImpl->m_stlSurface.push_back(surface);
}

VOID CStaticBox::ResizeSurface(VOID)
{
	CRectangle rtScreen,rtSnap;

	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin() ; the!=stlPSurface->end() ; the++)
	{
		pSurface = &(*the);
		rtScreen = GetScreenRect();

		rtSnap = pSurface->m_Original.rtRect;
		
		pSurface->m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
		pSurface->m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
		pSurface->m_SnapShot.rtRect.right = pSurface->m_SnapShot.rtRect.left+rtSnap.GetWidth();
		pSurface->m_SnapShot.rtRect.bottom = pSurface->m_SnapShot.rtRect.top+rtSnap.GetHeight();
	}
}

/////////////////////////////////////////////////////////////////////////////
// callback functions:

VOID CStaticBox::OnSetOptions(const CComponentOptions &options)
{
	int i,nCount;
	
	if(options.Exists("surface_file"))
	{
		std::string file = options.GetValue("surface_file"); 
		CSurfaceManager *pSurfaceManager = GetSurfaceManager();
		if(pSurfaceManager)
		{
			if(options.Exists("surface"))
			{
				nCount = options.Count("surface");
				for(i=0 ; i<nCount ; i++)
					AddSurface(pSurfaceManager->GetSurface(file,options.GetValue("surface",i))); 
			}
		}
	}

	if(options.Exists("text"))
		SetText(options.GetValue("text").c_str());
}

VOID CStaticBox::OnPaint(VOID)
{
	CRectangle* pClippingRect = NULL;
	if( IsClipped() )
	{
		pClippingRect = GetClippingRect();
	}

	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
	{
		g_GuiRenderer.RenderQueue(&((*the).m_SnapShot), (*the).m_pTexture, GetRenderTop(), (*the).m_fAngle, pClippingRect );
	}

	m_pImpl->Render();
}

VOID CStaticBox::OnMove( int nOldX,int nOldY )
{
	ResizeSurface();
}


END_GUI