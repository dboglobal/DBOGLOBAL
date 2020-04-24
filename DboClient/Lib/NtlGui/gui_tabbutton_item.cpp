#include "gui_precomp.h"
#include "gui_tabbutton_item.h"
#include "gui_tabbutton_generic.h"
#include "gui_renderer.h"

#ifdef FMOD_SOUND_ATTACHED_TO_GUI
#include "GUISoundDefine.h"
#endif

START_GUI

////////////////////////////////////////////////////////////////////////////////
// Constructors & Destructors

CTabButton_Item::CTabButton_Item( CTabButton_Generic* pGeneric, INT nIndex, std::string& strTab )
: m_nIndex( nIndex ), m_pGeneric( pGeneric ), m_bFocus( FALSE )
{
	m_pText = NTL_NEW CStaticBox( CRectangle( 0, 0, pGeneric->m_nUnselWidth, pGeneric->m_pTabButton->GetHeight() ), pGeneric->m_pTabButton, pGeneric->m_pTabButton->GetSurfaceManager() );
	
	m_slotMouseEnter = m_pText->SigMouseEnter().Connect( this, &CTabButton_Item::OnMouseEnter );
	m_slotMouseLeave = m_pText->SigMouseLeave().Connect( this, &CTabButton_Item::OnMouseLeave );
	m_slotMouseDown = m_pText->SigMouseDown().Connect( this, &CTabButton_Item::OnMouseDown );
	m_slotMouseUp = m_pText->SigMouseUp().Connect( this, &CTabButton_Item::OnMouseUp );
	m_slotMove = m_pText->SigMove().Connect( this, &CTabButton_Item::OnMove );

	ApplyAttribute( FALSE );	
	SetText( strTab.c_str() );
}

CTabButton_Item::CTabButton_Item( CTabButton_Generic* pGeneric, INT nIndex, std::wstring& wstrTab )
: m_nIndex( nIndex ), m_pGeneric( pGeneric ), m_bFocus( FALSE )
{
	m_pText = NTL_NEW CStaticBox( CRectangle( 0, 0, pGeneric->m_nUnselWidth, pGeneric->m_pTabButton->GetHeight() ), pGeneric->m_pTabButton, pGeneric->m_pTabButton->GetSurfaceManager() );
	
	m_slotMouseEnter = m_pText->SigMouseEnter().Connect( this, &CTabButton_Item::OnMouseEnter );
	m_slotMouseLeave = m_pText->SigMouseLeave().Connect( this, &CTabButton_Item::OnMouseLeave );
	m_slotMouseDown = m_pText->SigMouseDown().Connect( this, &CTabButton_Item::OnMouseDown );
	m_slotMouseUp = m_pText->SigMouseUp().Connect( this, &CTabButton_Item::OnMouseUp );
	m_slotMove = m_pText->SigMove().Connect( this, &CTabButton_Item::OnMove );

	ApplyAttribute( FALSE );	
	SetText( wstrTab.c_str() );	
}

CTabButton_Item::~CTabButton_Item(VOID)
{
	NTL_DELETE( m_pText );
}

////////////////////////////////////////////////////////////////////////////////
// Attributes

INT	CTabButton_Item::GetItemWidth(VOID)
{
	return m_pText->GetWidth();
}

////////////////////////////////////////////////////////////////////////////////
// Opration

VOID CTabButton_Item::SetText( const CHAR *pText )
{
	m_pText->SetText( pText );

	INT nWidth = m_pText->GetFitWidthToString();	

	if( nWidth > m_pGeneric->m_nUnselWidth )
		m_pText->SetToolTip( m_pText->GetText() );

	if( m_nIndex == m_pGeneric->GetSelectedTabIndex() )
		m_pGeneric->CalcTabRect();
}

VOID CTabButton_Item::SetText( const WCHAR* pText )
{
	m_pText->SetText( pText );

	INT nWidth = m_pText->GetFitWidthToString();	

	if( nWidth > m_pGeneric->m_nUnselWidth )
		m_pText->SetToolTip( m_pText->GetText() );

	if( m_nIndex == m_pGeneric->GetSelectedTabIndex() )
		m_pGeneric->CalcTabRect();
}

VOID CTabButton_Item::SetTextMargin( INT nX, INT nY )
{
	m_pText->SetMargin( nX, nY );
}

VOID CTabButton_Item::SetTextColor( COLORREF color )
{
	m_pText->SetTextColor( color );
}

VOID CTabButton_Item::SetPosition( INT nX )
{
	m_pText->SetPosition( nX, 0 );
}

VOID CTabButton_Item::Enable( BOOL bEnable )
{
	if( bEnable )
	{
		m_pText->Enable( true );
		
		if( m_pGeneric->GetSelectedTabIndex() == m_nIndex )
		{
			m_pText->SetTextColor( m_pGeneric->m_SelTextColor, TRUE );
		}			
		else if( m_bFocus )
		{
			m_pText->SetTextColor( m_pGeneric->m_FocTextColor, TRUE );
		}
		else
		{
			m_pText->SetTextColor( m_pGeneric->m_TextColor, TRUE );
		}
	}
	else
	{
		m_pText->Enable( false );
		m_pText->SetTextColor( m_pGeneric->m_DisableTextColor, TRUE );
	}
}

VOID CTabButton_Item::Focus( BOOL bFocus )
{
	m_bFocus = bFocus;

	if( m_nIndex == m_pGeneric->GetSelectedTabIndex() )
		return;

	if( bFocus )
	{
		m_pText->SetTextColor( m_pGeneric->m_FocTextColor, TRUE );

		SetSurface( m_pGeneric->m_Surface[CTabButton_Generic::FOC_LEFT],
					m_pGeneric->m_Surface[CTabButton_Generic::FOC_CENTER],
					m_pGeneric->m_Surface[CTabButton_Generic::FOC_RIGHT] );		
	}
	else
	{
		m_pText->SetTextColor( m_pGeneric->m_TextColor, TRUE );

		SetSurface( m_pGeneric->m_Surface[CTabButton_Generic::LEFT],
					m_pGeneric->m_Surface[CTabButton_Generic::CENTER],
					m_pGeneric->m_Surface[CTabButton_Generic::RIGHT] );		
	}	
}

VOID CTabButton_Item::Select(VOID)
{
	m_pText->SetTextColor( m_pGeneric->m_SelTextColor, TRUE );

	INT nWidth = m_pText->GetFitWidthToString();	
	
	if( nWidth > m_pGeneric->m_nMarginWidth )
	{
		nWidth = m_pGeneric->m_nMarginWidth;	
		m_pText->SetToolTip( m_pText->GetText() );
	}
	else
	{
		m_pText->DeleteToolTip();		
	}

	if( nWidth > m_pGeneric->m_nUnselWidth )
		m_pText->SetSize( nWidth, m_pText->GetHeight() );	
	
	SetSurface( m_pGeneric->m_Surface[CTabButton_Generic::SEL_LEFT],
				m_pGeneric->m_Surface[CTabButton_Generic::SEL_CENTER],
				m_pGeneric->m_Surface[CTabButton_Generic::SEL_RIGHT] );		
}

VOID CTabButton_Item::Release(VOID)
{
	INT nWidth = m_pText->GetFitWidthToString();	

	if( nWidth > m_pGeneric->m_nUnselWidth )
		m_pText->SetToolTip( m_pText->GetText() );
	
	if( m_pText->GetWidth() != m_pGeneric->m_nUnselWidth )
		m_pText->SetSize( m_pGeneric->m_nUnselWidth, m_pText->GetHeight() );

	if( m_bFocus )
	{
		m_pText->SetTextColor( m_pGeneric->m_FocTextColor, TRUE );

		SetSurface( m_pGeneric->m_Surface[CTabButton_Generic::FOC_LEFT],
					m_pGeneric->m_Surface[CTabButton_Generic::FOC_CENTER],
					m_pGeneric->m_Surface[CTabButton_Generic::FOC_RIGHT] );		
	}
	else
	{
		m_pText->SetTextColor( m_pGeneric->m_TextColor, TRUE );

		SetSurface( m_pGeneric->m_Surface[CTabButton_Generic::LEFT],
					m_pGeneric->m_Surface[CTabButton_Generic::CENTER],
					m_pGeneric->m_Surface[CTabButton_Generic::RIGHT] );		
	}	
}

////////////////////////////////////////////////////////////////////////////////
// Implements

VOID CTabButton_Item::SetSurface( CSurface& leftSurface, CSurface& centerSurface, CSurface& rightSurface )
{
	CRectangle* pScreenRect = &m_pText->GetScreenRect();

	m_Surfaces[LEFT] = leftSurface;
	
	m_Surfaces[LEFT].m_SnapShot.rtRect.left = pScreenRect->left;
	m_Surfaces[LEFT].m_SnapShot.rtRect.top = pScreenRect->top;
	m_Surfaces[LEFT].m_SnapShot.rtRect.right = pScreenRect->left + m_Surfaces[LEFT].m_Original.rtRect.GetWidth();
	m_Surfaces[LEFT].m_SnapShot.rtRect.bottom = pScreenRect->bottom;

	m_Surfaces[RIGHT] = rightSurface;

	m_Surfaces[RIGHT].m_SnapShot.rtRect.left = pScreenRect->right - m_Surfaces[RIGHT].m_Original.rtRect.GetWidth();
	m_Surfaces[RIGHT].m_SnapShot.rtRect.top = pScreenRect->top;
	m_Surfaces[RIGHT].m_SnapShot.rtRect.right = pScreenRect->right;
	m_Surfaces[RIGHT].m_SnapShot.rtRect.bottom = pScreenRect->bottom;

	m_Surfaces[CENTER] = centerSurface;

	m_Surfaces[CENTER].m_SnapShot.rtRect.left = m_Surfaces[LEFT].m_SnapShot.rtRect.right;
	m_Surfaces[CENTER].m_SnapShot.rtRect.top = pScreenRect->top;
	m_Surfaces[CENTER].m_SnapShot.rtRect.right = m_Surfaces[RIGHT].m_SnapShot.rtRect.left;
	m_Surfaces[CENTER].m_SnapShot.rtRect.bottom = pScreenRect->bottom;
}

VOID CTabButton_Item::ApplyAttribute( BOOL bTextRedraw /* = TRUE  */ )
{
	m_pText->CreateFontStd( (CHAR*)m_pGeneric->m_strFontname.c_str(), (INT)m_pGeneric->m_dwFontHeight, (INT)m_pGeneric->m_dwFontAttr );
	m_pText->SetTextColor( m_pGeneric->m_TextColor );
	m_pText->SetTextStyle( m_pGeneric->m_dwTextStyle );
	m_pText->SetMargin( m_pGeneric->m_nTextMarginX, m_pGeneric->m_nTextMarginY );	

	if( bTextRedraw )
		m_pText->Redraw();
	
	SetSurface( m_pGeneric->m_Surface[CTabButton_Generic::LEFT],
				m_pGeneric->m_Surface[CTabButton_Generic::CENTER],
				m_pGeneric->m_Surface[CTabButton_Generic::RIGHT] );		
}

VOID CTabButton_Item::Paint(VOID)
{
	g_GuiRenderer.RenderQueue( &m_Surfaces[LEFT].m_SnapShot, m_Surfaces[LEFT].m_pTexture );
	g_GuiRenderer.RenderQueue( &m_Surfaces[CENTER].m_SnapShot, m_Surfaces[CENTER].m_pTexture );
	g_GuiRenderer.RenderQueue( &m_Surfaces[RIGHT].m_SnapShot, m_Surfaces[RIGHT].m_pTexture );
}

////////////////////////////////////////////////////////////////////////////////
// Callback

VOID CTabButton_Item::OnMouseEnter( CComponent* pComponent )
{
	Focus( TRUE );

#ifdef FMOD_SOUND_ATTACHED_TO_GUI
	// mouse focus sound
	if( !m_pGeneric->m_strFocusSound.empty() )
	{
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_UI_SOUND;
		tSoundParam.pcFileName		= (char*)m_pGeneric->m_strFocusSound.c_str();

		GetSoundManager()->Play( &tSoundParam );
	}
#endif
}

VOID CTabButton_Item::OnMouseLeave( CComponent* pComponent )
{
	Focus( FALSE );
}

VOID CTabButton_Item::OnMouseDown( const CKey& key )
{
	if( key.m_nID != UD_LEFT_BUTTON )
		return;

	if( m_pText->IsMouseCaptured() )
		return;

	m_pText->CaptureMouse();

#ifdef FMOD_SOUND_ATTACHED_TO_GUI
	// mouse down sound
	if( !m_pGeneric->m_strClickSound.empty() )
	{
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_UI_SOUND;
		tSoundParam.pcFileName		= (char*)m_pGeneric->m_strClickSound.c_str();

		GetSoundManager()->Play( &tSoundParam );
	}
#endif
}

VOID CTabButton_Item::OnMouseUp( const CKey& key )
{
	if( key.m_nID != UD_LEFT_BUTTON )
		return;

	if( m_pText->IsMouseCaptured() == false )
		return;

	if( m_pText->GetClientRect().PtInRect( (INT)key.m_fX, (INT)key.m_fY ) )
		m_pGeneric->SelectTab( m_nIndex, TRUE );

	m_pText->ReleaseMouse();
}

VOID CTabButton_Item::OnMove( INT nX, INT nY )
{
	CRectangle* pScreenRect = &m_pText->GetScreenRect();
	
	CRectangle rtSnapShot;

	m_Surfaces[LEFT].m_SnapShot.rtRect.left = pScreenRect->left;
	m_Surfaces[LEFT].m_SnapShot.rtRect.top = pScreenRect->top;
	m_Surfaces[LEFT].m_SnapShot.rtRect.right = pScreenRect->left + m_Surfaces[LEFT].m_Original.rtRect.GetWidth();
	m_Surfaces[LEFT].m_SnapShot.rtRect.bottom = pScreenRect->bottom;

	m_Surfaces[RIGHT].m_SnapShot.rtRect.left = pScreenRect->right - m_Surfaces[LEFT].m_Original.rtRect.GetWidth();
	m_Surfaces[RIGHT].m_SnapShot.rtRect.top = pScreenRect->top;
	m_Surfaces[RIGHT].m_SnapShot.rtRect.right = pScreenRect->right;
	m_Surfaces[RIGHT].m_SnapShot.rtRect.bottom = pScreenRect->bottom;

	m_Surfaces[CENTER].m_SnapShot.rtRect.left = m_Surfaces[LEFT].m_SnapShot.rtRect.right;
	m_Surfaces[CENTER].m_SnapShot.rtRect.top = pScreenRect->top;
	m_Surfaces[CENTER].m_SnapShot.rtRect.right = m_Surfaces[RIGHT].m_SnapShot.rtRect.left;
	m_Surfaces[CENTER].m_SnapShot.rtRect.bottom = pScreenRect->bottom;
}

END_GUI