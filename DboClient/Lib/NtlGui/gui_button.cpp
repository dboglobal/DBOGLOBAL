#include "gui_precomp.h"
#include "mouse.h"
#include "gui_precomp.h"
#include "gui_button.h"
#include "gui_button_generic.h"
#include "gui_staticbox.h"
#include "gui_component_generic.h"
#include "gui_surfacemanager.h"
#include "gui_renderer.h"

START_GUI

////////////////////////////////////////////////////////////////////////////
// Construction:

//: Button Constructor

CButton::CButton(CComponent* pParent, CSurfaceManager* pSurfaceManager, bool bToggleMode /* = false  */)
: CComponent(pParent,pSurfaceManager), m_pImpl( NULL ), m_pButtonText( NULL )
{
	m_pButtonText = NTL_NEW CStaticBox( this, pSurfaceManager, COMP_TEXT_CENTER | COMP_TEXT_VERTICAL_CENTER );		
	m_pButtonText->Enable( false );
	m_pButtonText->Show( false );

	m_pImpl = NTL_NEW CButton_Generic(this, bToggleMode);
	
	m_SlotSetOption = SigSetOptions().Connect(this, &CButton::OnSetOptions);
	m_SlotGetPreferredSize = SigGetPreferredSize().Connect(this, &CButton::OnGetPreferredSize);
	m_SlotMove = SigMove().Connect(this, &CButton::OnMove);
	m_SlotResize = SigResize().Connect( this, &CButton::OnResize );
}

CButton::CButton(CRectangle& rtRect, CComponent* pParent, CSurfaceManager* pSurfaceManager, bool bToggleMode /* = false  */)
: CComponent(rtRect,pParent,pSurfaceManager), m_pImpl( NULL ), m_pButtonText( NULL )
{
	m_pButtonText = NTL_NEW CStaticBox( CRectangle( 0, 0, rtRect.GetWidth(), rtRect.GetHeight() ), this, pSurfaceManager, COMP_TEXT_CENTER | COMP_TEXT_VERTICAL_CENTER );		
	m_pButtonText->Enable( false );
	m_pButtonText->Show( false );

	m_pImpl = NTL_NEW CButton_Generic(this, bToggleMode);

	m_SlotSetOption = SigSetOptions().Connect(this, &CButton::OnSetOptions);
	m_SlotGetPreferredSize = SigGetPreferredSize().Connect(this, &CButton::OnGetPreferredSize);
	m_SlotMove = SigMove().Connect(this, &CButton::OnMove);
	m_SlotResize = SigResize().Connect( this, &CButton::OnResize );
}

CButton::CButton(CRectangle& ptRect, const std::string& text, CComponent* pParent, CSurfaceManager* pSurfaceManager, bool bToggleMode /* = false  */)
: CComponent(ptRect, pParent,pSurfaceManager), m_pImpl( NULL ), m_pButtonText( NULL )
{
	m_pButtonText = NTL_NEW CStaticBox( CRectangle( 0, 0, ptRect.GetWidth(), ptRect.GetHeight() ), this , pSurfaceManager, COMP_TEXT_CENTER | COMP_TEXT_VERTICAL_CENTER );
	m_pButtonText->Enable( false );

	m_pImpl = NTL_NEW CButton_Generic(this, bToggleMode);
	SetText( text );
	FindPreferredSize();

	m_SlotSetOption = SigSetOptions().Connect(this, &CButton::OnSetOptions);
	m_SlotGetPreferredSize = SigGetPreferredSize().Connect(this, &CButton::OnGetPreferredSize);
	m_SlotMove = SigMove().Connect(this, &CButton::OnMove);
	m_SlotResize = SigResize().Connect( this, &CButton::OnResize );
}

CButton::CButton(CRectangle& ptRect, const std::wstring& text, CComponent* pParent, CSurfaceManager* pSurfaceManager, bool bToggleMode /* = false  */)
: CComponent(ptRect, pParent,pSurfaceManager), m_pImpl( NULL ), m_pButtonText( NULL )
{
	m_pButtonText = NTL_NEW CStaticBox( CRectangle( 0, 0, ptRect.GetWidth(), ptRect.GetHeight() ), this , pSurfaceManager, COMP_TEXT_CENTER | COMP_TEXT_VERTICAL_CENTER );
	m_pButtonText->Enable( false );

	m_pImpl = NTL_NEW CButton_Generic(this, bToggleMode);
	SetText( text );
	FindPreferredSize();

	m_SlotSetOption = SigSetOptions().Connect(this, &CButton::OnSetOptions);
	m_SlotGetPreferredSize = SigGetPreferredSize().Connect(this, &CButton::OnGetPreferredSize);
	m_SlotMove = SigMove().Connect(this, &CButton::OnMove);
	m_SlotResize = SigResize().Connect( this, &CButton::OnResize );
}

//: Button Constructor
CButton::CButton( CRectangle& ptRect, const std::string& text, CSurface Up, CSurface Down, CSurface Disable, CSurface Focus,
				  COLORREF colorUpText, COLORREF colorDownText, COLORREF colorFocusText, COLORREF colorDisableText,
				  INT nCoordTextDownX, INT nCoordTextDownY, CComponent* pParent, CSurfaceManager* pSurfaceManager, bool bToggleMode /* = false  */)
: CComponent(ptRect, pParent,pSurfaceManager), m_pImpl( NULL ), m_pButtonText( NULL )
{
	m_pButtonText = NTL_NEW CStaticBox( CRectangle( 0, 0, ptRect.GetWidth(), ptRect.GetHeight() ), this, pSurfaceManager, COMP_TEXT_CENTER | COMP_TEXT_VERTICAL_CENTER );
	m_pButtonText->Enable( false );

	m_pImpl = NTL_NEW CButton_Generic(this, bToggleMode);
	SetText( text );

	SetTextUpColor( colorUpText );
	SetTextDownColor( colorDownText );
	SetTextFocusColor( colorFocusText );
	SetTextDisableColor( colorDisableText );
	SetTextDownCoordDiff( nCoordTextDownX, nCoordTextDownY );	

	FindPreferredSize();

	AddSurfaceUp(Up);
	AddSurfaceDown(Down);
	AddSurfaceFocus(Focus);
	AddSurfaceDisabled(Disable);

	m_SlotSetOption = SigSetOptions().Connect(this, &CButton::OnSetOptions);
	m_SlotGetPreferredSize = SigGetPreferredSize().Connect(this, &CButton::OnGetPreferredSize);
	m_SlotMove = SigMove().Connect(this, &CButton::OnMove);
	m_SlotResize = SigResize().Connect( this, &CButton::OnResize );
}

CButton::CButton( CRectangle& ptRect, const std::wstring& text, CSurface Up, CSurface Down, CSurface Disable, CSurface Focus,
				 COLORREF colorUpText, COLORREF colorDownText, COLORREF colorFocusText, COLORREF colorDisableText,
				 INT nCoordTextDownX, INT nCoordTextDownY, CComponent* pParent, CSurfaceManager* pSurfaceManager, bool bToggleMode /* = false  */)
				 : CComponent(ptRect, pParent,pSurfaceManager), m_pImpl( NULL ), m_pButtonText( NULL )
{
	m_pButtonText = NTL_NEW CStaticBox( CRectangle( 0, 0, ptRect.GetWidth(), ptRect.GetHeight() ), this, pSurfaceManager, COMP_TEXT_CENTER | COMP_TEXT_VERTICAL_CENTER );
	m_pButtonText->Enable( false );

	m_pImpl = NTL_NEW CButton_Generic(this, bToggleMode);
	SetText( text );

	SetTextUpColor( colorUpText );
	SetTextDownColor( colorDownText );
	SetTextFocusColor( colorFocusText );
	SetTextDisableColor( colorDisableText );
	SetTextDownCoordDiff( nCoordTextDownX, nCoordTextDownY );	

	FindPreferredSize();

	AddSurfaceUp(Up);
	AddSurfaceDown(Down);
	AddSurfaceFocus(Focus);
	AddSurfaceDisabled(Disable);

	m_SlotSetOption = SigSetOptions().Connect(this, &CButton::OnSetOptions);
	m_SlotGetPreferredSize = SigGetPreferredSize().Connect(this, &CButton::OnGetPreferredSize);
	m_SlotMove = SigMove().Connect(this, &CButton::OnMove);
	m_SlotResize = SigResize().Connect( this, &CButton::OnResize );
}


CButton::~CButton()
{
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:
bool CButton::IsToggleButton(VOID) const
{
	return m_pImpl->m_bToggleMode;
}

bool CButton::IsDown(VOID) const
{
	return m_pImpl->m_bToggled;
}

BOOL CButton::IsClickEnable(VOID) const
{
	return m_pImpl->m_bClickEnable;
}

BOOL CButton::IsFocused(VOID) const
{
	return m_pImpl->m_bMouseEnter;
}

std::list<CSurface>* CButton::GetSurfaceUp() const
{
	return &m_pImpl->m_stlSurfaceUp;
}

std::list<CSurface>* CButton::GetSurfaceDown() const
{
	return &m_pImpl->m_stlSurfaceDown;
}

std::list<CSurface>* CButton::GetSurfaceFocus() const
{
	return &m_pImpl->m_stlSurfaceFocus;
}

std::list<CSurface>* CButton::GetSurfaceDisable(VOID) const
{
	return &m_pImpl->m_stlSurfaceDisabled;
}

CStaticBox* CButton::GetTextComponent(VOID) const
{
	return m_pButtonText;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CButton::SetText( const std::string& text )
{
	m_pButtonText->SetText( text.c_str() );
	m_pButtonText->Show( true );
}

VOID CButton::SetText( const std::wstring& text )
{
	m_pButtonText->SetText( text.c_str() );
	m_pButtonText->Show( true );
}

void CButton::SetText(const int nNumber)
{
	m_pButtonText->SetText(nNumber);
	m_pButtonText->Show(true);
}

VOID CButton::SetTextUpColor( COLORREF Color )
{
	m_pImpl->m_colorTextUpColor = Color;
}

VOID CButton::SetTextUpAlpha( BYTE ucAlpha )
{
	m_pImpl->m_ucTextUpAlpha = ucAlpha;
}

VOID CButton::SetTextDownColor( COLORREF Color )
{
	m_pImpl->m_colorTextDownColor = Color;
}

VOID CButton::SetTextDownAlpha( BYTE ucAlpha )
{
	m_pImpl->m_ucTextDownAlpha = ucAlpha;
}

VOID CButton::SetTextFocusColor( COLORREF Color )
{
	m_pImpl->m_colorTextFocusColor = Color;
}

VOID CButton::SetTextFocusAlpha( BYTE ucAlpha )
{
	m_pImpl->m_ucTextFocusAlpha = ucAlpha;
}

VOID CButton::SetTextDisableColor( COLORREF Color )
{
	m_pImpl->m_colorTextDisableColor = Color;
}

VOID CButton::SetTextDisableAlpha( BYTE ucAlpha )
{
	m_pImpl->m_ucTextDisableAlpha = ucAlpha;
}

VOID CButton::SetTextEffectColor( COLORREF color )
{
	m_pButtonText->SetEffectColor( color );
}

VOID CButton::SetTextEffectMode( INT nMode )
{
	m_pButtonText->SetEffectMode( nMode );
}

VOID CButton::SetTextEffectValue( INT nValue )
{
	m_pButtonText->SetEffectValue( nValue );
}

VOID CButton::SetTextCoord( INT nX, INT nY )
{
	m_pImpl->m_nTextCoordX = nX;
	m_pImpl->m_nTextCoordY = nY;
	
	m_pButtonText->SetSize( GetWidth() - nX, GetHeight() - nY );
	m_pImpl->SetRenderSurface();
}

VOID CButton::SetTextDownCoordDiff( INT nX, INT nY )
{
	m_pImpl->m_nTextDownCoordDiffX = nX;
	m_pImpl->m_nTextDownCoordDiffY = nY;
}

VOID CButton::SetTextFont( char* pFaceName, INT nHeight, INT nAttributes )
{
	m_pButtonText->CreateFontStd( pFaceName, nHeight, nAttributes );
}

VOID CButton::SetTextStyle( DWORD dwStyle )
{
	m_pButtonText->SetTextStyle( dwStyle );
}

VOID CButton::ClearText(VOID)
{
	m_pButtonText->Clear();
	m_pButtonText->Show( false );
}

VOID CButton::ApplyText(VOID)
{
	m_pImpl->SetRenderSurface();
	m_pButtonText->Redraw();
}

VOID CButton::SetToggleMode(bool toggle)
{
	m_pImpl->m_bToggleMode = toggle;
}

VOID CButton::SetDown(bool enable)
{
	m_pImpl->m_bToggled = enable;
	m_pImpl->SetRenderSurface();
}

VOID CButton::Toggle(VOID)
{
	m_pImpl->m_bToggled = !m_pImpl->m_bToggled;
	m_pImpl->m_SigToggled( this, m_pImpl->m_bToggled );
	m_pImpl->SetRenderSurface();
}

VOID CButton::AddSurfaceUp(CSurface surface)
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = GetScreenRect(); 
	
	surface.m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left+rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top+rtSnap.GetHeight();

	m_pImpl->m_stlSurfaceUp.push_back(surface);
}


VOID CButton::AddSurfaceDown(CSurface surface)
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;

	CRectangle rtScreen = GetScreenRect(); 
	
	surface.m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left+rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top+rtSnap.GetHeight();

	m_pImpl->m_stlSurfaceDown.push_back(surface);
}



VOID CButton::AddSurfaceFocus(CSurface surface)
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = GetScreenRect(); 
	
	surface.m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left+rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top+rtSnap.GetHeight();

	m_pImpl->m_stlSurfaceFocus.push_back(surface);
}


VOID CButton::AddSurfaceDisabled(CSurface surface)
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = GetScreenRect(); 
	
	surface.m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left+rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top+rtSnap.GetHeight();

	m_pImpl->m_stlSurfaceDisabled.push_back(surface);
}

VOID CButton::ResizeSurfaceUp(VOID)
{
	CRectangle rtScreen,rtSnap;

	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurfaceUp();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
			the++)
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

VOID CButton::ResizeSurfaceDown(VOID)
{
	CRectangle rtScreen,rtSnap;

	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurfaceDown();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
			the++)
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

VOID CButton::ResizeSurfaceFocus(VOID)
{
	CRectangle rtScreen,rtSnap;

	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurfaceFocus();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
			the++)
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

VOID CButton::ResizeSurfaceDisable(VOID)
{
	CRectangle rtScreen,rtSnap;

	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurfaceDisable();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
			the++)
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

VOID CButton::Enable( bool bEnable /* = true */ )
{
	CComponent::Enable( bEnable );
	m_pImpl->SetRenderSurface();
}

VOID CButton::ClickEnable( BOOL bEnable /* = TRUE  */ )
{
	m_pImpl->m_bClickEnable = bEnable;
	m_pImpl->SetRenderSurface();
}

VOID CButton::SetFocusSound( const CHAR* szFocusSoundFIlename )
{
	if( szFocusSoundFIlename )
		m_pImpl->m_strFocusSound = szFocusSoundFIlename;
	else
		m_pImpl->m_strFocusSound.clear();
}

VOID CButton::SetClickSound( const CHAR* szClickSoundFilename )
{
	if( szClickSoundFilename )
		m_pImpl->m_strClickSound = szClickSoundFilename;
	else
		m_pImpl->m_strClickSound.clear();
}

VOID CButton::SetDisableSound( const CHAR* szDisableSoundFilename )
{
	if( szDisableSoundFilename )
		m_pImpl->m_strDisableSound = szDisableSoundFilename;
	else
		m_pImpl->m_strDisableSound.clear();
}

VOID CButton::SetClippingMode( bool bClipping )
{
	CComponent::SetClippingMode( bClipping );
	m_pButtonText->SetClippingMode( bClipping );
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CSignal_v2<CComponent*,bool> &CButton::SigToggled()
{
	return m_pImpl->m_SigToggled;
}

CSignal_v1<CComponent*> &CButton::SigClicked()
{
	return m_pImpl->m_SigClicked;
}

CSignal_v1<CComponent*> &CButton::SigPressed()
{
	return m_pImpl->m_SigPressed;
}

CSignal_v1<CComponent*> &CButton::SigReleased()
{
	return m_pImpl->m_SigReleased;
}

CSignal_v2<CComponent*,bool>& CButton::SigFocused()
{
	return m_pImpl->m_SigFocused;
}

VOID CButton::OnSetOptions(const CComponentOptions &options)
{
	if(options.Exists("surface_file"))
	{
		std::string file = options.GetValue("surface_file"); 
		CSurfaceManager *pSurfaceManager = GetSurfaceManager();
		if(pSurfaceManager)
		{
			int i,nCount;
			if(options.Exists("surface_up"))
			{
				nCount = options.Count("surface_up");
				for(i=0 ; i<nCount ; i++)
					AddSurfaceUp(pSurfaceManager->GetSurface(file,options.GetValue("surface_up",i))); 
			}
			if(options.Exists("surface_down"))
			{
				nCount = options.Count("surface_down");
				for(i=0 ; i<nCount ; i++)
					AddSurfaceDown(pSurfaceManager->GetSurface(file,options.GetValue("surface_down",i))); 
			}
			if(options.Exists("surface_focus"))
			{
				nCount = options.Count("surface_focus");
				for(i=0 ; i<nCount ; i++)
					AddSurfaceFocus(pSurfaceManager->GetSurface(file,options.GetValue("surface_focus",i))); 
			}
			if(options.Exists("surface_disable"))
			{
				nCount = options.Count("surface_disable");
				for(i=0 ; i<nCount ; i++)
					AddSurfaceDisabled(pSurfaceManager->GetSurface(file,options.GetValue("surface_disable",i))); 
			}

			if(options.Exists("surface_mask"))
			{
				m_pImpl->m_surMask = pSurfaceManager->GetSurface(file,options.GetValue("surface_mask",0));
				if(m_pImpl->m_surMask.m_pTexture )
				{
					m_pImpl->CreateMaskImage(m_pImpl->m_surMask.m_pTexture->GetName()); 
				}
			}
		}
	}

	// Button Text 
	CComponentOptions suboptions = options;
	suboptions.RemoveOption( "x" );
	suboptions.RemoveOption( "y" );
	suboptions.RemoveOption( "width" );
	suboptions.RemoveOption( "height" );
	suboptions.AddOption( "x", m_pImpl->m_nTextCoordX );
	suboptions.AddOption( "y", m_pImpl->m_nTextCoordY );
	suboptions.AddOption( "width", GetWidth() - m_pImpl->m_nTextCoordX );
	suboptions.AddOption( "height", GetHeight() - m_pImpl->m_nTextCoordY );

	// if use button text, show static box
	if( suboptions.Exists("text") )
		m_pButtonText->Show( true );

	m_pButtonText->SigSetOptions()( suboptions );
	m_pImpl->SetRenderSurface();
}

VOID CButton::OnGetPreferredSize(CPos& size)
{
	/*
	CL_Surface *s = button->get_surface_up();
	if(s)
	{
		size.x = s->get_width();
		size.y = s->get_height();
	}
	else
	{
		size.x = font->get_text_width(button->get_text()) + 8;
		size.y = font->get_height() + 4;
	}
	*/
}


VOID CButton::OnMove(int nX,int nY)
{
	ResizeSurfaceUp();
	ResizeSurfaceDown();
	ResizeSurfaceFocus();
	ResizeSurfaceDisable();
}

VOID CButton::OnResize( INT nOldWidth, INT nOldHeight )
{
	m_pButtonText->SetSize( GetWidth(), GetHeight() );
}

END_GUI

