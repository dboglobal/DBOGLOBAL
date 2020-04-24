#include "gui_precomp.h"
#include "gui_inputbox.h"
#include "gui_inputbox_generic.h"
#include "gui_renderer.h"

CallGuiInputBoxFocus g_fnCallInputBoxFocus = NULL;

void LinkGuiInputBoxFocus(CallGuiInputBoxFocus fn)
{
	g_fnCallInputBoxFocus = fn;
}

void UnLinkGuiInputBoxFocus(void)
{
	g_fnCallInputBoxFocus = NULL;
}

START_GUI

/////////////////////////////////////////////////////////////////////////////
// Construction:

CInputBox::CInputBox(
	CComponent *pParent,
	CSurfaceManager *pSurfaceManager,
	DWORD dwStyle)
: CComponent(pParent,pSurfaceManager), m_pImpl(NULL)
{
	m_pScrollBar = NTL_NEW CScrollBar( CRectangle( 0, 0, 0, 0 ), false, this, pSurfaceManager );

	m_pImpl = NTL_NEW CInputBox_Generic(this,dwStyle,1024);

	m_SlotSetOptions = SigSetOptions().Connect(
		this, &CInputBox::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CInputBox::OnPaint);
	m_SlotGotFocus = SigGotFocus().Connect(
		this,&CInputBox::OnGotFocus);
	m_SlotLostFocus = SigLostFocus().Connect(
		this,&CInputBox::OnLostFocus);
	m_SlotKeyDown = SigKeyDown().Connect(
		this,&CInputBox::OnKeyDown); 
	m_SlotKeyUp = SigKeyUp().Connect(
		this,&CInputBox::OnKeyUp); 
	m_SlotMove = SigMove().Connect(
		this,&CInputBox::OnMove); 
}

CInputBox::CInputBox(
	const CRectangle &ptRect,
	CComponent *pParent,
	CSurfaceManager *pSurfaceManager,
	DWORD dwStyle)
: CComponent(ptRect, pParent,pSurfaceManager), m_pImpl(NULL)
{
	CRectangle rtScrollBar;
	CalcScrollBarRect( &rtScrollBar, DEFAULT_SLIDER_WIDTH );

	m_pScrollBar = NTL_NEW CScrollBar( rtScrollBar, false, this, pSurfaceManager );

	m_pImpl = NTL_NEW CInputBox_Generic(this,dwStyle, 1024);

	m_SlotSetOptions = SigSetOptions().Connect(
		this, &CInputBox::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CInputBox::OnPaint);
	m_SlotGotFocus = SigGotFocus().Connect(
		this,&CInputBox::OnGotFocus);
	m_SlotLostFocus = SigLostFocus().Connect(
		this,&CInputBox::OnLostFocus);
	m_SlotKeyDown = SigKeyDown().Connect(
		this,&CInputBox::OnKeyDown); 
	m_SlotKeyUp = SigKeyUp().Connect(
		this,&CInputBox::OnKeyUp); 
	m_SlotMove = SigMove().Connect(
		this,&CInputBox::OnMove); 
}


CInputBox::~CInputBox()
{
	CEventTimer::RemoveTimer(m_hTimer);

	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

const WCHAR* CInputBox::GetText(VOID) const
{
	return m_pImpl->m_Buffer.GetBuffer();
}

int CInputBox::GetLength(VOID) const
{
	return m_pImpl->m_Buffer.GetTextSize();
}

int CInputBox::GetMaxLength(VOID) const
{
	return m_pImpl->m_nMaxLength;
}

std::list<CSurface>* CInputBox::GetSurface(VOID) const
{
	return &m_pImpl->m_stlSurface;
}

INT CInputBox::GetCurrentIMEState(VOID) const
{
	return CInputBox_Generic::m_SavedImeState;
}

BOOL CInputBox::IsNumberOnly( VOID ) const
{
	return m_pImpl->IsNumberOnly();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CInputBox::SetPasswordMode(BOOL bPwMode)
{
	m_pImpl->SetPasswordMode(bPwMode);
}

VOID CInputBox::SetNumberOnly( BOOL bNumberMode )
{
	m_pImpl->SetNumberOnly( bNumberMode );
}

VOID CInputBox::SetMultilineMode(BOOL bMultilineMode)
{
	m_pImpl->SetMultilineMode(bMultilineMode);
}

VOID CInputBox::SetLineInterval(INT nInterval)
{
	m_pImpl->SetLineInterval(nInterval);
}

VOID CInputBox::SetReadOnly(BOOL bReadOnly)
{
	m_pImpl->SetReadOnly(bReadOnly);
}


VOID CInputBox::SetText(const char *text)
{
	Clear();

	m_pImpl->SetText(text);
}

VOID CInputBox::SetText(const std::string &text)
{
	Clear();

	m_pImpl->SetText(text.c_str());
}

VOID CInputBox::SetText(int number)
{
	Clear();

	char temp[10];
	memset(temp,0,10);

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s(temp,10, "%d",number);
#else
	sprintf(temp,"%d",number);
#endif

	m_pImpl->SetText(temp);
}


VOID CInputBox::SetText(const WCHAR *text)
{
	Clear();

	m_pImpl->SetText(text);
}

VOID CInputBox::SetMaxLength(int nLength)
{
	if(nLength < 0)
		return;

	m_pImpl->m_nMaxLength = nLength;
	m_pImpl->Clear();
}


VOID CInputBox::Clear(VOID)
{
	m_pImpl->Clear();
}


VOID CInputBox::AddSurface(CSurface surface)
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = GetScreenRect(); 
	
	surface.m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left+rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top+rtSnap.GetHeight();

	m_pImpl->m_stlSurface.push_back(surface);
}

VOID CInputBox::ResizeSurface(VOID)
{
	CRectangle rtScreen,rtSnap;

	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurface();
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

VOID CInputBox::CalcScrollBarRect( CRectangle* prtScrollBar, INT nScrollBarWidth )
{
	CRectangle rtRect = GetClientRect();

	prtScrollBar->left		= rtRect.right - nScrollBarWidth;
	prtScrollBar->top		= rtRect.top;
	prtScrollBar->right		= rtRect.right;
	prtScrollBar->bottom	= rtRect.bottom;
}

VOID CInputBox::SetTextColor(COLORREF color)
{
	m_pImpl->m_TextColor = color;	
}

VOID CInputBox::SetTextBkColor(COLORREF color)  
{
	m_pImpl->m_BkColor = color;
}

VOID CInputBox::SetTextBkMode(int nBkMode)
{
	m_pImpl->m_nBkMode = nBkMode;
}

//: Set Caret Draw show
VOID CInputBox::ShowCaret(VOID)
{
	m_pImpl->m_bShowCaret = true;
}

//: Set Caret Draw hide
VOID CInputBox::HideCaret(VOID)
{
	m_pImpl->m_bShowCaret = false;
}

VOID CInputBox::SetCaretSize(int nWidth,int nHeight)
{
	m_pImpl->SetCaretSize(nWidth,nHeight);
}

//: Set Caret Color
VOID CInputBox::SetCaretColor(BYTE byRed,BYTE byGreen,BYTE byBlue,BYTE byAlpha)
{
	m_pImpl->SetCaretColor(byRed,byGreen,byBlue,byAlpha); 
}

VOID CInputBox::SetEnglishMode (VOID)
{
	CInputBox_Generic::SetEnglishMode();
}

VOID CInputBox::SetEnglishModeDefault()
{
	CInputBox_Generic::SetEnglishModeDefault();
}

VOID CInputBox::SetNativeMode (VOID)
{
	CInputBox_Generic::SetNativeMode();
}

VOID CInputBox::SetNativeModeDefault(VOID)
{
	CInputBox_Generic::SetNativeModeDefault();
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CSignal_v1<const std::string &> &CInputBox::SigChanged(VOID)
{
	return m_pImpl->m_SigChanged;
}

CSignal_v0 &CInputBox::SigReturnPressed(VOID)
{
	return m_pImpl->m_SigReturnPressed;
}

CSignal_v0& CInputBox::SigSpaceUp(VOID)
{
	return m_pImpl->m_SigSpaceUp;
}

CSignal_v0 &CInputBox::SigActivity(VOID)
{
	return m_pImpl->m_SigActivity;
}

CSignal_v1<INT>& CInputBox::SigIMEChanged(VOID)
{
	return CInputBox_Generic::m_SigIMEChanged;
}


/////////////////////////////////////////////////////////////////////////////
// callback functions:

VOID CInputBox::OnSetOptions(const CComponentOptions &options)
{
	int nX=0,nY=0,nWidth=0,nHeight=0;
	
	if(options.Exists("x"))
		nX = options.GetValueAsInt("x");
	if(options.Exists("y"))
		nY = options.GetValueAsInt("y");
	if(options.Exists("width"))
		nWidth = options.GetValueAsInt("width");
	if(options.Exists("height"))
		nHeight = options.GetValueAsInt("height");

	SetPosition(CRectangle(nX,nY,nX+nWidth,nY+nHeight));

	COLORREF color,bkcolor;
    
	BYTE byRed = DEFAULT_TEXTCOLOR_RED;
	BYTE byGreen = DEFAULT_TEXTCOLOR_GREEN;
	BYTE byBlue = DEFAULT_TEXTCOLOR_BLUE;

	if(options.Exists("text_color_red"))
		byRed = options.GetValueAsInt("text_color_red");
	if(options.Exists("text_color_green"))
		byGreen = options.GetValueAsInt("text_color_green");
	if(options.Exists("text_color_blue"))
		byBlue = options.GetValueAsInt("text_color_blue");

	color = RGB(byRed,byGreen,byBlue);
	SetTextColor(color);
	
	byRed = DEFAULT_BKCOLOR_RED;
	byGreen = DEFAULT_BKCOLOR_GREEN;
	byBlue = DEFAULT_BKCOLOR_BLUE;

	if(options.Exists("text_bkcolor_red"))
		byRed = options.GetValueAsInt("text_bkcolor_red");
	if(options.Exists("text_bkcolor_green"))
		byGreen = options.GetValueAsInt("text_bkcolor_green");
	if(options.Exists("text_bkcolor_blue"))
		byBlue = options.GetValueAsInt("text_bkcolor_blue");

	bkcolor = RGB(byRed,byGreen,byBlue);
	SetTextBkColor(bkcolor);
	
	if(options.Exists("surface_file"))
	{
		std::string file = options.GetValue("surface_file"); 
		CSurfaceManager *pSurfaceManager = GetSurfaceManager();
		if(pSurfaceManager)
		{
			int nCount;
			if(options.Exists("surface"))
			{
				nCount = options.Count("surface");
				for(int i=0 ; i<nCount ; i++)
					AddSurface(pSurfaceManager->GetSurface(file,options.GetValue("surface",i))); 
			}
		}
	}

	// scroll bar

	CComponentOptions suboptions = options;
	suboptions.RemoveOption( "x" );
	suboptions.RemoveOption( "y" );
	suboptions.RemoveOption( "width" );
	suboptions.RemoveOption( "height" );

	CRectangle rtScrollBar;
	INT nScrollBarWidth = DEFAULT_SLIDER_WIDTH;
	
	if( suboptions.Exists( "slider_width" ) )
		nScrollBarWidth = suboptions.GetValueAsInt( "slider_width" );

	CalcScrollBarRect( &rtScrollBar, nScrollBarWidth );

	suboptions.AddOption( "x", rtScrollBar.left );
	suboptions.AddOption( "y", rtScrollBar.top );
	suboptions.AddOption( "width", rtScrollBar.GetWidth() );
	suboptions.AddOption( "height", rtScrollBar.GetHeight() );

	m_pScrollBar->SigSetOptions()( suboptions );
	m_pScrollBar->SetRange( 0, 0 );
}


VOID CInputBox::OnPaint(VOID)
{
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
	{
		g_GuiRenderer.RenderQueue(&(*the).m_SnapShot, (*the).m_pTexture);
	}
	
	m_pImpl->Render(); 
}

VOID CInputBox::OnGotFocus(VOID)
{
	m_pImpl->m_bShowCaret = true;
	m_pImpl->m_bCaretMode = true;
	m_pImpl->CaretRenderPosition();

	m_hTimer = CEventTimer::AddTimer(500,this,&CInputBox::OnTimer); 

	if(g_fnCallInputBoxFocus)
	{
		(*g_fnCallInputBoxFocus)(true);
	}
}


VOID CInputBox::OnLostFocus(VOID)
{
	m_pImpl->m_bShowCaret = false;
	CEventTimer::RemoveTimer(m_hTimer); 

	if(g_fnCallInputBoxFocus)
	{
		(*g_fnCallInputBoxFocus)(false);
	}
}

VOID CInputBox::OnKeyDown(CComponent *pComp, CInputDevice *pDevice, const CKey& key)
{
	if(ENABLE_COMPONENT_STYLE(m_pImpl->m_dwStyle,IS_READONLY))
		return;

	if(key.m_dwVKey == VK_DELETE)
	{
		m_pImpl->m_SigActivity();
	}

	if(key.m_dwVKey == VK_RETURN)
	{
		m_pImpl->m_SigReturnPressed();
	}
}

VOID CInputBox::OnKeyUp(CComponent *pComp, CInputDevice *pDevice, const CKey& key)
{
	if(key.m_dwVKey == VK_ESCAPE )
	{
		m_pImpl->m_SigEscKeyUp();
	}

	if (key.m_dwVKey == VK_SPACE)
	{
		m_pImpl->m_SigSpaceUp();
	}
}

VOID CInputBox::OnMove(int nOldX,int nOldY)
{
	ResizeSurface();
}

VOID CInputBox::OnTimer(VOID)
{
	m_pImpl->m_bCaretMode = !m_pImpl->m_bCaretMode;
}

LRESULT CInputBox::OnWindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return m_pImpl->OnWindowProc(hWnd,uMsg,wParam,lParam);
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CSignal_v1<int>& CInputBox::SigCharChanged(VOID)
{
	return m_pImpl->m_SigCharChanged;
}

CSignal_v0 & gui::CInputBox::SigEscKeyUp( VOID ) 
{
	return m_pImpl->m_SigEscKeyUp;
}


END_GUI
