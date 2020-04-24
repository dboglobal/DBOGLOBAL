#include "gui_precomp.h"
#include "gui_tabbutton_generic.h"

#include "gui_tabbutton_item.h"
#include "gui_renderer.h"

#ifdef FMOD_SOUND_ATTACHED_TO_GUI
#include "GUISoundDefine.h"
#endif

START_GUI

#define TABBUTTON_DEFAULT_MARGIN_X	3
#define TABBUTTON_DEFAULT_MARGIN_Y	3
#define TABBUTTON_UNSEL_WIDTH		30

////////////////////////////////////////////////////////////////////////////////
// Cunstructor & Destructor:

CTabButton_Generic::CTabButton_Generic( CTabButton* pSelf )
: m_nSelectedIndex( -1 ), m_pTabButton( pSelf )
{
	m_TextColor = DEFAULT_TABCOLOR;
	m_SelTextColor = DEFAULT_TABSELCOLOR;
	m_FocTextColor = DEFAULT_TABFOCUSCOLOR;
	m_DisableTextColor = DEFAULT_DISABLECOLOR;
	m_ucTextAlpha = m_ucSelTextAlpha = m_ucFocTextAlpha = m_ucDisableTextAlpha = DEFAULT_ALPHA;
	
	m_strFontname = DEFAULT_FONT;
	m_dwFontHeight = DEFAULT_FONT_SIZE;
	m_dwFontAttr = DEFAULT_FONT_ATTR;
	m_dwTextStyle = COMP_TEXT_CENTER;
	
	m_nTextMarginX = TABBUTTON_DEFAULT_MARGIN_X;
	m_nTextMarginY = TABBUTTON_DEFAULT_MARGIN_Y;
	m_nUnselWidth = TABBUTTON_UNSEL_WIDTH;
	m_nMarginWidth = pSelf->GetWidth();

	m_bLeftOriented = TRUE;

	m_slotSetOptions = pSelf->SigSetOptions().Connect( this, &CTabButton_Generic::OnSetOptions );
	m_slotMove	= pSelf->SigMove().Connect( this, &CTabButton_Generic::OnMove );
	m_slotPaint = pSelf->SigPaint().Connect( this, &CTabButton_Generic::OnPaint );

#ifdef FMOD_SOUND_ATTACHED_TO_GUI
	// sound
	m_strFocusSound = GSD_SYSTEM_BUTTON_FOCUS;
	m_strClickSound = GSD_SYSTEM_TAB_CLICK;
#endif
}

CTabButton_Generic::~CTabButton_Generic(VOID)
{
	for( INT i = 0 ; i < GetTabCount() ; ++i )
	{
		NTL_DELETE( m_vecTabItem[i] );
	}
	m_vecTabItem.clear();
}

////////////////////////////////////////////////////////////////////////////////
// Attributes:

INT	CTabButton_Generic::GetSelectedTabIndex(VOID)
{
	return m_nSelectedIndex;
}

INT CTabButton_Generic::GetTabCount(VOID)
{
	return (INT)m_vecTabItem.size();
}

////////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CTabButton_Generic::SelectTab( INT nIndex, BOOL bDoCallBack )
{
	if( AvailableIndexRange( nIndex ) )
	{
		INT nTemp = m_nSelectedIndex;

		m_nSelectedIndex = nIndex;

		if( bDoCallBack )
			m_SigSelected( nIndex );				// Send Signal~!!

		if( nTemp != nIndex )
		{
			if( AvailableIndexRange( nTemp ) )
				m_vecTabItem[nTemp]->Release();
			m_vecTabItem[nIndex]->Select();

			if( bDoCallBack )
				m_SigSelectChanged( nIndex, nTemp );// Send Signal~!!
		}
	}	

	CalcTabRect();
}

VOID CTabButton_Generic::EnableTab( INT nIndex, BOOL bEnable )
{
	if( !AvailableIndexRange( nIndex ) )
		return;

	m_vecTabItem[nIndex]->Enable( bEnable );
}

VOID CTabButton_Generic::ChangeTabText( INT nIndex, std::string& strText )
{
	if( !AvailableIndexRange( nIndex ) )
		return;

	m_vecTabItem[nIndex]->SetText( strText.c_str() );	
}

VOID CTabButton_Generic::ChangeTabText( INT nIndex, std::wstring& wstrText )
{
	if( !AvailableIndexRange( nIndex ) )
		return;

	m_vecTabItem[nIndex]->SetText( wstrText.c_str() );
}

INT	CTabButton_Generic::AddTab( std::string& strTabText )
{
	INT nIndex = GetTabCount();

	m_vecTabItem.push_back( NTL_NEW CTabButton_Item( this, nIndex, strTabText ) );
	
	AvailableWidthCheck();		

	return nIndex;
}

INT CTabButton_Generic::AddTab( std::wstring& wstrTabText )
{
	INT nIndex = GetTabCount();
	
	m_vecTabItem.push_back( NTL_NEW CTabButton_Item( this, nIndex, wstrTabText ) );

	AvailableWidthCheck();		

	return nIndex;
}

VOID CTabButton_Generic::ClearTab(VOID)
{
	for( INT i = 0 ; i < GetTabCount() ; ++i )
	{
		NTL_DELETE( m_vecTabItem[i] );
	}

	m_vecTabItem.clear();

	m_nSelectedIndex = -1;
}

VOID CTabButton_Generic::CalcTabRect(VOID)
{
	CRectangle* pScreenRect = &m_pTabButton->GetScreenRect();
	INT nTabCount = GetTabCount();
	INT	nCurrentWidth;
	
	if( m_bLeftOriented )
	{
		nCurrentWidth = 0;

		for( INT i = 0 ; i < nTabCount ; ++i )
		{
			m_vecTabItem[i]->SetPosition( nCurrentWidth );
			nCurrentWidth += m_vecTabItem[i]->GetItemWidth();
		}

		// Line
		m_Surface[LINE].m_Original.rtRect.left = nCurrentWidth;
		m_Surface[LINE].m_Original.rtRect.right = pScreenRect->GetWidth();

		m_Surface[LINE].m_SnapShot.rtRect.left = pScreenRect->left + nCurrentWidth;
		m_Surface[LINE].m_SnapShot.rtRect.top = pScreenRect->top;
		m_Surface[LINE].m_SnapShot.rtRect.right = pScreenRect->left + m_Surface[LINE].m_Original.rtRect.right;
		m_Surface[LINE].m_SnapShot.rtRect.bottom = pScreenRect->top + m_Surface[LINE].m_Original.rtRect.bottom;
	}
	else
	{
		nCurrentWidth = pScreenRect->GetWidth();

		for( INT i = nTabCount - 1 ; i >= 0 ; --i )
		{
			nCurrentWidth -= m_vecTabItem[i]->GetItemWidth();
			m_vecTabItem[i]->SetPosition( nCurrentWidth );
		}

		// Line
		m_Surface[LINE].m_Original.rtRect.left = 0;
		m_Surface[LINE].m_Original.rtRect.right = nCurrentWidth; 

		m_Surface[LINE].m_SnapShot.rtRect.left = pScreenRect->left;
		m_Surface[LINE].m_SnapShot.rtRect.top = pScreenRect->top;
		m_Surface[LINE].m_SnapShot.rtRect.right = pScreenRect->left + m_Surface[LINE].m_Original.rtRect.right;
		m_Surface[LINE].m_SnapShot.rtRect.bottom = pScreenRect->top + m_Surface[LINE].m_Original.rtRect.bottom;
	}
}

VOID CTabButton_Generic::InitLine(VOID)
{
	CRectangle* pScreenRect = &m_pTabButton->GetScreenRect();

	m_Surface[LINE].m_Original.rtRect.left = 0;
	m_Surface[LINE].m_Original.rtRect.top = 0;
	m_Surface[LINE].m_Original.rtRect.right = pScreenRect->GetWidth();
	m_Surface[LINE].m_Original.rtRect.bottom = pScreenRect->GetHeight();
}

VOID CTabButton_Generic::ApplyAttribute(VOID)
{
	for( INT i = 0 ; i < GetTabCount() ; ++i )
	{
		m_vecTabItem[i]->ApplyAttribute();
	}	
}

////////////////////////////////////////////////////////////////////////////////
// Implements:
VOID CTabButton_Generic::AvailableWidthCheck(VOID)
{
	INT nComponentWidth = m_pTabButton->GetWidth();
	INT nItemCount = (INT)m_vecTabItem.size();

	if( nItemCount == 0 )
		return;
	
	if( nComponentWidth < nItemCount * m_nUnselWidth )
		assert( 0 );

	m_nMarginWidth = nComponentWidth - ( ( nItemCount - 1 ) * m_nUnselWidth ); 	
}

////////////////////////////////////////////////////////////////////////////////
// Callbacks:

VOID CTabButton_Generic::OnSetOptions( const CComponentOptions& options )
{
	if( options.Exists( "surface_file" ) )
	{
		std::string file = options.GetValue( "surface_file" ); 
		CSurfaceManager* pSurfaceManager = m_pTabButton->GetSurfaceManager();
					
		if( pSurfaceManager )
		{
			m_Surface[LEFT] = pSurfaceManager->GetSurface( file, options.GetValue( "surface_left", 0 ) );
			m_Surface[CENTER] = pSurfaceManager->GetSurface( file, options.GetValue( "surface_center", 0 ) );
			m_Surface[RIGHT] = pSurfaceManager->GetSurface( file, options.GetValue( "surface_right", 0 ) );
			m_Surface[SEL_LEFT] = pSurfaceManager->GetSurface( file, options.GetValue( "surface_sel_left", 0 ) );
			m_Surface[SEL_CENTER] = pSurfaceManager->GetSurface( file, options.GetValue( "surface_sel_center", 0 ) );
			m_Surface[SEL_RIGHT] = pSurfaceManager->GetSurface( file, options.GetValue( "surface_sel_right", 0 ) );
			m_Surface[FOC_LEFT] = pSurfaceManager->GetSurface( file, options.GetValue( "surface_foc_left", 0 ) );
			m_Surface[FOC_CENTER] = pSurfaceManager->GetSurface( file, options.GetValue( "surface_foc_center", 0 ) );
			m_Surface[FOC_RIGHT] = pSurfaceManager->GetSurface( file, options.GetValue( "surface_foc_right", 0 ) );
			m_Surface[LINE] = pSurfaceManager->GetSurface( file, options.GetValue( "surface_line", 0 ) );

			InitLine();
		}
	}

	BYTE r, g, b, a;
	r = DEFAULT_TABCOLOR_RED; g = DEFAULT_TABCOLOR_GREEN; b = DEFAULT_TABCOLOR_BLUE; a = DEFAULT_ALPHA;
	
	if( options.Exists( "textcolor_red" ) )
		r = options.GetValueAsInt( "textcolor_red" );
	if( options.Exists( "textcolor_green" ) )
		g = options.GetValueAsInt( "textcolor_green" );
	if( options.Exists( "textcolor_red" ) )
		b = options.GetValueAsInt( "textcolor_blue" );
	if( options.Exists( "textalpha" ) )
		a = options.GetValueAsInt( "textalpha" );

	m_TextColor = RGB( r, g, b );
	m_ucTextAlpha = a;

	r = DEFAULT_TABSELCOLOR_RED; g = DEFAULT_TABSELCOLOR_GREEN; b = DEFAULT_TABSELCOLOR_BLUE; a = DEFAULT_ALPHA;

	if( options.Exists( "textcolor_sel_red" ) )
		r = options.GetValueAsInt( "textcolor_sel_red" );
	if( options.Exists( "textcolor_sel_green" ) )
		g = options.GetValueAsInt( "textcolor_sel_green" );
	if( options.Exists( "textcolor_sel_red" ) )
		b = options.GetValueAsInt( "textcolor_sel_blue" );
	if( options.Exists( "textalpha_sel" ) )
		a = options.GetValueAsInt( "textalpha_sel" );

	m_SelTextColor = RGB( r, g, b );
	m_ucSelTextAlpha = a;

	r = DEFAULT_TABFOCUSCOLOR_RED; g = DEFAULT_TABFOCUSCOLOR_GREEN; b = DEFAULT_TABFOCUSCOLOR_BLUE; a = DEFAULT_ALPHA;

	if( options.Exists( "textcolor_foc_red" ) )
		r = options.GetValueAsInt( "textcolor_foc_red" );
	if( options.Exists( "textcolor_foc_green" ) )
		g = options.GetValueAsInt( "textcolor_foc_green" );
	if( options.Exists( "textcolor_foc_red" ) )
		b = options.GetValueAsInt( "textcolor_foc_blue" );
	if( options.Exists( "textalpha_foc" ) )
		a = options.GetValueAsInt( "textalpha_foc" );

	m_FocTextColor = RGB( r, g, b );
	m_ucFocTextAlpha = a;

	r = DEFAULT_DISABLECOLOR_RED; g = DEFAULT_DISABLECOLOR_GREEN; b = DEFAULT_DISABLECOLOR_BLUE; a = DEFAULT_ALPHA;

	if( options.Exists( "textcolor_disable_red" ) )
		r = options.GetValueAsInt( "textcolor_disable_red" );
	if( options.Exists( "textcolor_disable_green" ) )
		g = options.GetValueAsInt( "textcolor_disable_green" );
	if( options.Exists( "textcolor_disable_red" ) )
		b = options.GetValueAsInt( "textcolor_disable_blue" );
	if( options.Exists( "textalpha_disable" ) )
		a = options.GetValueAsInt( "textalpha_disable" );

	m_DisableTextColor = RGB( r, g, b );
	m_ucDisableTextAlpha = a;

	if( options.Exists( "font_name" ) )
		m_strFontname = options.GetValue( "font_name" );
	
	if( options.Exists( "font_height" ) )
		m_dwFontHeight = options.GetValueAsInt( "font_height" );
	
	if( options.Exists( "font_attribute" ) )
		m_dwFontAttr = options.GetValueAsInt( "font_attribute" );

	if( options.Exists( "text_style" ) )
	{
		std::string style = options.GetValue( "text_style" ); 
		if(style == "left")
		{
			m_dwTextStyle = COMP_TEXT_LEFT;
		}
		else if(style == "center")
		{
			m_dwTextStyle = COMP_TEXT_CENTER;
		}
		else if(style == "right")
		{
			m_dwTextStyle = COMP_TEXT_RIGHT;
		}
	}

	if( options.Exists( "tabtext_margin_x" ) )
		m_nTextMarginX = options.GetValueAsInt( "tabtext_margin_x" );
	
	if( options.Exists( "tabtext_margin_y" ) )
		m_nTextMarginY = options.GetValueAsInt( "tabtext_margin_y" );
	
	if( options.Exists( "tab_unsel_width" ) )
		m_nUnselWidth = options.GetValueAsInt( "tab_unsel_width" );
	
	if( options.Exists( "tabtext" ) )
	{
		INT nTabCount = options.Count( "tabtext" );

		for( INT i = 0 ; i < nTabCount ; ++i )
		{
			std::string strTab = options.GetValue( "tabtext", i );
			AddTab( strTab );
		}		
	}

	if( options.Exists( "left_oriented" ) )
		m_bLeftOriented = options.GetValueAsBool( "left_oriented" );


	INT nInitIndex = -1;

	if( options.Exists( "initindex" ) )
		nInitIndex = options.GetValueAsInt( "initindex" );

	SelectTab( nInitIndex, TRUE );
}

VOID CTabButton_Generic::OnPaint(VOID)
{
	for( INT i = 0 ; i < GetTabCount() ; ++i )
	{
		m_vecTabItem[i]->Paint();
	}

	// Line
	g_GuiRenderer.RenderQueue( &m_Surface[LINE].m_SnapShot, m_Surface[LINE].m_pTexture );
}

VOID CTabButton_Generic::OnMove( INT nX, INT nY )
{	
	CRectangle rtScreen = m_pTabButton->GetScreenRect();

	// Line
	m_Surface[LINE].m_SnapShot.rtRect.left = rtScreen.left + m_Surface[LINE].m_Original.rtRect.left;
	m_Surface[LINE].m_SnapShot.rtRect.top = rtScreen.top + m_Surface[LINE].m_Original.rtRect.top;
	m_Surface[LINE].m_SnapShot.rtRect.right = rtScreen.left + m_Surface[LINE].m_Original.rtRect.right;
	m_Surface[LINE].m_SnapShot.rtRect.bottom = rtScreen.top + m_Surface[LINE].m_Original.rtRect.bottom;
}	

END_GUI