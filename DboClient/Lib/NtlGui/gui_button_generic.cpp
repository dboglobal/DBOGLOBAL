#include "gui_precomp.h"
#include "gui_button_generic.h"
#include "gui_componentoption.h"
#include "gui_renderer.h"

#ifdef FMOD_SOUND_ATTACHED_TO_GUI
#include "GUISoundDefine.h"
#endif

START_GUI

CButton_Generic::CButton_Generic(CButton *pSelf, bool bToggleMode)
: m_pButton(pSelf), m_bToggled(false),	m_bToggleMode(bToggleMode),	m_bMouseEnter( FALSE ), m_bClickEnable( TRUE ), m_bEnableMouseCapture( TRUE ),
  m_colorTextUpColor( DEFAULT_TEXTCOLOR ), m_colorTextDownColor( DEFAULT_TEXTCOLOR ),
  m_colorTextFocusColor( DEFAULT_TEXTCOLOR ), m_colorTextDisableColor( DEFAULT_TEXTCOLOR ),
  m_nTextDownCoordDiffX( GUI_BUTTON_DOWN_COORD_X ), m_nTextDownCoordDiffY( GUI_BUTTON_DOWN_COORD_Y ),
  m_nTextCoordX( 0 ), m_nTextCoordY( 0 ), m_ucTextUpAlpha( DEFAULT_ALPHA ), m_ucTextDownAlpha( DEFAULT_ALPHA ),
  m_ucTextFocusAlpha( DEFAULT_ALPHA ), m_ucTextDisableAlpha( DEFAULT_ALPHA )
{
	m_SlotSetOptions = m_pButton->SigSetOptions().Connect(this, &CButton_Generic::OnSetOptions);
	m_SlotKeyDown = m_pButton->SigKeyDown().Connect(this, &CButton_Generic::OnKeyDown);
	m_SlotKeyUp = m_pButton->SigKeyUp().Connect(this, &CButton_Generic::OnKeyUp);
	m_SlotMouseLeave = m_pButton->SigMouseLeave().Connect( this, &CButton_Generic::OnMouseLeave );
	m_SlotMouseDown = m_pButton->SigMouseDown().Connect(this,&CButton_Generic::OnMouseDown);
	m_SlotMouseUp = m_pButton->SigMouseUp().Connect(this,&CButton_Generic::OnMouseUp);
	m_SlotMouseMove = m_pButton->SigMouseMove().Connect( this, &CButton_Generic::OnMouseMove );
	m_SlotPaint = m_pButton->SigPaint().Connect(this,&CButton_Generic::OnPaint);
	m_slotSetAlpha = m_pButton->SigSetAlpha().Connect( this, &CButton_Generic::OnSetAlpha );
	m_slotSetColor = m_pButton->SigSetColor().Connect( this, &CButton_Generic::OnSetColor );
	
	m_surMask.m_pTexture = NULL;
	m_pMaskImage = NULL;
	m_pRenderSurface = &m_stlSurfaceUp;

	// sound
#ifdef FMOD_SOUND_ATTACHED_TO_GUI
	m_strFocusSound = GSD_SYSTEM_BUTTON_FOCUS;
	m_strClickSound = GSD_SYSTEM_BUTTON_CLICK;
	m_strDisableSound = GSD_SYSTEM_BUTTON_DISABLE;
#endif
}


CButton_Generic::~CButton_Generic()
{
	if(m_pMaskImage)
	{
		RwImageDestroy(m_pMaskImage);
		m_pMaskImage = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

VOID CButton_Generic::OnSetOptions(const CComponentOptions &options)
{
	if(options.Exists("toggled"))
		m_bToggled = options.GetValueAsBool("toggled");

	if(options.Exists("togglemode"))
		m_bToggleMode = options.GetValueAsBool("togglemode");

	BYTE r, g, b, a;
	r = DEFAULT_BUTTON_UP_COLOR_RED;
	g = DEFAULT_BUTTON_UP_COLOR_GREEN;
	b = DEFAULT_BUTTON_UP_COLOR_BLUE;
	a = DEFAULT_ALPHA;
	if( options.Exists( "textcolor_up_red" ) )
		r = options.GetValueAsInt( "textcolor_up_red" );
	if( options.Exists( "textcolor_up_green" ) )
		g = options.GetValueAsInt( "textcolor_up_green" );
	if( options.Exists( "textcolor_up_blue" ) )
		b = options.GetValueAsInt( "textcolor_up_blue" );
	if( options.Exists( "textalpha_up" ) )
		a = options.GetValueAsInt( "textalpha_up" );
	m_colorTextUpColor = RGB( r, g, b );
	m_ucTextUpAlpha = a;

	r = DEFAULT_BUTTON_DOWN_COLOR_RED;
	g = DEFAULT_BUTTON_DOWN_COLOR_GREEN;
	b = DEFAULT_BUTTON_DOWN_COLOR_BLUE;
	a = DEFAULT_ALPHA;
	if( options.Exists( "textcolor_down_red" ) )
		r = m_ucTextDownAlpha = options.GetValueAsInt( "textcolor_down_red" );
	if( options.Exists( "textcolor_down_green" ) )
		g = m_ucTextDownAlpha = options.GetValueAsInt( "textcolor_down_green" );
	if( options.Exists( "textcolor_down_blue" ) )
		b = m_ucTextDownAlpha = options.GetValueAsInt( "textcolor_down_blue" );
	if( options.Exists( "textalpha_down" ) )
		a = options.GetValueAsInt( "textalpha_down" );
	m_colorTextDownColor = RGB( r, g, b );
	m_ucTextDownAlpha = a;
	
	r = DEFAULT_BUTTON_FOCUS_COLOR_RED;
	g = DEFAULT_BUTTON_FOCUS_COLOR_GREEN;
	b = DEFAULT_BUTTON_FOCUS_COLOR_BLUE;
	a = DEFAULT_ALPHA;
	if( options.Exists( "textcolor_focus_red" ) )
		r = m_ucTextFocusAlpha = options.GetValueAsInt( "textcolor_focus_red" );
	if( options.Exists( "textcolor_focus_green" ) )
		g = m_ucTextFocusAlpha = options.GetValueAsInt( "textcolor_focus_green" );
	if( options.Exists( "textcolor_focus_blue" ) )
		b = m_ucTextFocusAlpha = options.GetValueAsInt( "textcolor_focus_blue" );
	if( options.Exists( "textalpha_focus" ) )
		a = options.GetValueAsInt( "textalpha_focus" );
	m_colorTextFocusColor = RGB( r, g, b );
	m_ucTextFocusAlpha = a;

	r = DEFAULT_BUTTON_DISABLED_COLOR_RED;
	g = DEFAULT_BUTTON_DISABLED_COLOR_GREEN;
	b = DEFAULT_BUTTON_DISABLED_COLOR_BLUE;
	a = DEFAULT_ALPHA;
	if( options.Exists( "textcolor_disable_red" ) )
		r = m_ucTextDisableAlpha = options.GetValueAsInt( "textcolor_disable_red" );
	if( options.Exists( "textcolor_disable_green" ) )
		g = m_ucTextDisableAlpha = options.GetValueAsInt( "textcolor_disable_green" );
	if( options.Exists( "textcolor_disable_blue" ) )
		b = m_ucTextDisableAlpha = options.GetValueAsInt( "textcolor_disable_blue" );
	if( options.Exists( "textalpha_disable" ) )
		a = options.GetValueAsInt( "textalpha_disable" );
	m_colorTextDisableColor = RGB( r, g, b );
	m_ucTextDisableAlpha = a;

	if( options.Exists( "text_x" ) )
		m_nTextCoordX = options.GetValueAsInt( "text_x" );
	if( options.Exists( "text_y" ) )
		m_nTextCoordY = options.GetValueAsInt( "text_y" );

	if( options.Exists( "text_downcoord_x" ) )
		m_nTextDownCoordDiffX = options.GetValueAsInt( "text_downcoord_x" );
	if( options.Exists( "text_downcoord_y" ) )
		m_nTextDownCoordDiffY = options.GetValueAsInt( "text_downcoord_y" );

	if( options.Exists( "enablemousecapture" ) )
		m_bEnableMouseCapture = options.GetValueAsBool( "enablemousecapture" );
}

VOID CButton_Generic::OnKeyDown(CComponent *pComponent, CInputDevice *pDevice, const CKey &key)
{
	/*
	if(m_pButton->IsMouseCaptured())
		return;

//	if(key.id != CL_KEY_SPACE)
//		return;

	m_bToggled = !m_bToggled;

	m_SigPressed();
	m_SigToggled(m_bToggled);
	
	m_pButton->CaptureMouse();
	*/
}

VOID CButton_Generic::OnKeyUp(CComponent *comp, CInputDevice *device, const CKey &key)
{
/*
	if(m_pButton->IsMouseCaptured() == false)
		return;

//	if(key.id != CL_KEY_SPACE)
//		return;

	m_SigReleased();

	if (m_pButton->HasMouseOver())
	{
		if(m_bToggleMode == false)
		{
			m_bToggled = !m_bToggled;
			m_SigToggled(m_bToggled);
		}
				
		m_SigClicked();
	}
		
	m_pButton->ReleaseMouse();
*/
}

VOID CButton_Generic::OnMouseLeave( CComponent* pComponent )
{
	if( m_bMouseEnter )
	{
		SetMouseEnter( FALSE );
		SetRenderSurface();
	}
}

VOID CButton_Generic::OnMouseMove( INT nKey, INT nX, INT nY )
{
	if( m_bEnableMouseCapture && m_pButton->IsMouseCaptured() )
		return;

	if( PtInValidRegion( nX, nY ) )
	{
		if( !m_bMouseEnter )
		{
			SetMouseEnter( TRUE );					// RenderSerface 보다 MouseEnter가 먼저 와야 한다. 

			if( !( m_bToggleMode && m_bToggled ) )
				SetRenderSurface();		

			m_SigFocused( m_pButton, true );
#ifdef FMOD_SOUND_ATTACHED_TO_GUI
			// mouse focus sound
			if( !m_strFocusSound.empty() )
			{
				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_UI_SOUND;
				tSoundParam.pcFileName		= (char*)m_strFocusSound.c_str();

				GetSoundManager()->Play( &tSoundParam );
			}
#endif
		}		
	}
	else
	{
		if( m_bMouseEnter )
		{
			SetMouseEnter( FALSE );			
			SetRenderSurface();
		}		
	}	
}

VOID CButton_Generic::OnMouseDown(const CKey& key)
{
	if(key.m_nID != UD_LEFT_BUTTON)
		return;

	if( !m_bClickEnable )
	{
#ifdef FMOD_SOUND_ATTACHED_TO_GUI
		// mouse disable sound
		if( !m_strDisableSound.empty() )
		{
			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup	= CHANNEL_GROUP_UI_SOUND;
			tSoundParam.pcFileName		= (char*)m_strDisableSound.c_str();

			GetSoundManager()->Play( &tSoundParam );
		}
#endif
		return;
	}

	if( m_bEnableMouseCapture && m_pButton->IsMouseCaptured() )
		return;

	if(m_surMask.m_pTexture)
	{
		if(!PtInMask((int)key.m_fX, (int)key.m_fY))
			return;
	}

	m_bToggled = !m_bToggled;
	
	m_SigPressed( m_pButton );
	
#ifdef FMOD_SOUND_ATTACHED_TO_GUI
	// mouse down sound
	if( !m_strClickSound.empty() )
	{
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_UI_SOUND;
		tSoundParam.pcFileName		= (char*)m_strClickSound.c_str();

		GetSoundManager()->Play( &tSoundParam );
	}
#endif

	SetRenderSurface();

	if( m_bEnableMouseCapture )
		m_pButton->CaptureMouse();	
}


VOID CButton_Generic::OnMouseUp(const CKey& key)
{
	if(key.m_nID != UD_LEFT_BUTTON)
		return;

	if( m_bEnableMouseCapture && !m_pButton->IsMouseCaptured() )
		return;

	m_pButton->ReleaseMouse();

	if( !m_bClickEnable )
		return;

//	if(device->get_type() != CL_InputDevice::type_mouse && key.id != CL_KEY_SPACE)
//		return;

	if( PtInValidRegion( (INT)key.m_fX, (INT)key.m_fY ) && m_pButton->IsVisibleTruly() )
	{
		SetMouseEnter( TRUE );

		if( m_bToggleMode )
		{
			m_SigToggled( m_pButton, m_bToggled );	
		}
		else
		{
			m_bToggled = false;
			m_SigClicked( m_pButton );	
		}		
		
		SetRenderSurface();
	}
	else
	{
		m_bToggled = !m_bToggled;
		SetMouseEnter( FALSE );
		SetRenderSurface();
	}

	m_SigReleased( m_pButton );	
}

VOID CButton_Generic::OnPaint(VOID)
{
	CRectangle* pClippingRect = NULL;
	if( m_pButton->IsClipped() )
	{
		pClippingRect = m_pButton->GetClippingRect();
	}

	std::list<CSurface>::iterator it;

	for( it = m_pRenderSurface->begin() ; it != m_pRenderSurface->end() ; ++it )
	{
		g_GuiRenderer.RenderQueue( &(*it).m_SnapShot, (*it).m_pTexture, m_pButton->GetRenderTop(), (*it).m_fAngle, pClippingRect );
	}
}

VOID CButton_Generic::OnSetColor( BYTE byRed, BYTE byGreen, BYTE byBlue )
{
	CSurface* pSurface = NULL;

	std::list<CSurface>::iterator it;

	for( it = m_stlSurfaceUp.begin() ; it != m_stlSurfaceUp.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uRed = byRed;
		pSurface->m_SnapShot.uGreen = byGreen;
		pSurface->m_SnapShot.uBlue = byBlue;
	}

	for( it = m_stlSurfaceDown.begin() ; it != m_stlSurfaceDown.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uRed = byRed;
		pSurface->m_SnapShot.uGreen = byGreen;
		pSurface->m_SnapShot.uBlue = byBlue;
	}

	for( it = m_stlSurfaceFocus.begin() ; it != m_stlSurfaceFocus.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uRed = byRed;
		pSurface->m_SnapShot.uGreen = byGreen;
		pSurface->m_SnapShot.uBlue = byBlue;
	}

	for( it = m_stlSurfaceDisabled.begin() ; it != m_stlSurfaceDisabled.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uRed = byRed;
		pSurface->m_SnapShot.uGreen = byGreen;
		pSurface->m_SnapShot.uBlue = byBlue;
	}
}

VOID CButton_Generic::OnSetAlpha( BYTE ucAlpha )
{
	CSurface* pSurface = NULL;

	std::list<CSurface>::iterator it;

	for( it = m_stlSurfaceUp.begin() ; it != m_stlSurfaceUp.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uAlpha = ucAlpha;
	}

	for( it = m_stlSurfaceDown.begin() ; it != m_stlSurfaceDown.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uAlpha = ucAlpha;
	}

	for( it = m_stlSurfaceFocus.begin() ; it != m_stlSurfaceFocus.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uAlpha = ucAlpha;
	}

	for( it = m_stlSurfaceDisabled.begin() ; it != m_stlSurfaceDisabled.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uAlpha = ucAlpha;
	}
}

bool CButton_Generic::PtInMask(int nX, int nY)
{
	if(nX < 0 || nY < 0)
		return false;

	if(m_pMaskImage == NULL)
		return true;

	int nWidth = RwImageGetWidth(m_pMaskImage);
	int nHeight = RwImageGetHeight(m_pMaskImage);
	int nStride = RwImageGetStride(m_pMaskImage);
	unsigned char *pPixel = RwImageGetPixels(m_pMaskImage);

	int nPosX = (int)(m_surMask.m_SnapShot.UVs[0]*(float)nWidth) + nX;
	int nPosY = (int)(m_surMask.m_SnapShot.UVs[1]*(float)nHeight) + nY;

	if(nPosX < 0 || nPosX >= nWidth || nPosY < 0 || nPosY >= nHeight)
		return false;

	DWORD dwColor = *((DWORD*)(pPixel + nStride * nPosY + nPosX*4));
	if(dwColor == 0xff0000ff)
		return true;

	return false;
}

VOID CButton_Generic::CreateMaskImage(const char *pFileName)
{
	m_pMaskImage = RwImageRead(pFileName);
}

BOOL CButton_Generic::PtInValidRegion( INT nX, INT nY )
{
	if( m_surMask.m_pTexture )
	{
		if( PtInMask( nX, nY ) )
			return TRUE;

		return FALSE;
	}

	if( m_pButton->GetClientRect().PtInRect( nX, nY ) )
		return TRUE;

	
	return FALSE;
}

VOID CButton_Generic::SetRenderSurface(VOID)
{
	if( m_pButton->IsEnabled() && m_bClickEnable )
	{
		if( m_bToggled )
		{
			m_pRenderSurface = &m_stlSurfaceDown;
			m_pButton->m_pButtonText->SetTextColor( m_colorTextDownColor );
			m_pButton->m_pButtonText->SetTextAlphaByRatio( m_ucTextDownAlpha, TRUE );
			m_pButton->m_pButtonText->SetPosition( m_nTextCoordX + m_nTextDownCoordDiffX, m_nTextCoordY + m_nTextDownCoordDiffY );
		}
		else
		{
			if( m_bMouseEnter && !m_bToggled )
			{
				m_pRenderSurface = &m_stlSurfaceFocus;
				m_pButton->m_pButtonText->SetTextColor( m_colorTextFocusColor );
				m_pButton->m_pButtonText->SetTextAlphaByRatio( m_ucTextFocusAlpha, TRUE );
			}
			else
			{
				m_pRenderSurface = &m_stlSurfaceUp;
				m_pButton->m_pButtonText->SetTextColor( m_colorTextUpColor );
				m_pButton->m_pButtonText->SetTextAlphaByRatio( m_ucTextUpAlpha, TRUE );
			}			
			
			m_pButton->m_pButtonText->SetPosition( m_nTextCoordX, m_nTextCoordY );
		}
	}	
	else
	{
		m_pRenderSurface = &m_stlSurfaceDisabled;
		m_pButton->m_pButtonText->SetTextColor( m_colorTextDisableColor );
		m_pButton->m_pButtonText->SetTextAlphaByRatio( m_ucTextDisableAlpha, TRUE );

		m_pButton->m_pButtonText->SetPosition( m_nTextCoordX, m_nTextCoordY );		
	}	
}

VOID CButton_Generic::SetMouseEnter( BOOL bMouseEnter )
{
	if( bMouseEnter )
	{
		if( !m_bMouseEnter )
			m_SigFocused( m_pButton, true );
	}
	else
	{
		if( m_bMouseEnter )
			m_SigFocused( m_pButton, false );
	}

	m_bMouseEnter = bMouseEnter;
}

END_GUI