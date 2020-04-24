#include "gui_precomp.h"
#include "gui_font.h"
#include <math.h>

START_GUI

#define GUI_FONT_TEXT_COLOR	RGB(255,255,255)
#define GUI_FONT_BG_COLOR	RGB(0,0,0)

CGuiFont::CGuiFont()
{
	m_hFont = NULL;
	m_nPtHeight = 0;		
	m_nPtWidth = 0;			
	m_nAttributes = 0;	
	m_bLogRes = false;
	m_dwWidth = 0;
	m_dwHeight = 0;

	m_nRefCount = 0;
}

CGuiFont::~CGuiFont()
{
}

/**
* \brief Gui 폰트 로드
* \param hDC			(HDC) DC의 핸들
* \param pFontName		(const char*) Font의 이름
* \param strKey			(std::string&) Font의 키
* \param nWidth			(int) 폰트의 넓이
* \param nHeight		(int) 폰트의 높이
* \param nAttribute		(int) 폰트의 특성
* \param bLogRes		(bool) 
*/
bool CGuiFont::Load(HDC hDC, const char *pFontName, std::string& strKey, int nWidth, int nHeight, int nAttribute, bool bLogRes)
{
	// Create a font and increment the reference count only if the reference count is zero.
	// Increase the reference count only if the reference count is greater than 1.
	if(m_nRefCount)
	{
		AddRef();
		return true;
	}
	else
	{
		if(nHeight <= 0)
			return false;
		
		float fDpiCx, fDpiCy;
		
		HFONT hFont, hOldFont;
		LOGFONT lf;
		POINT pt;
		TEXTMETRIC tm;

		SetGraphicsMode(hDC, GM_ADVANCED) ;
		ModifyWorldTransform(hDC, NULL, MWT_IDENTITY) ;
		SetViewportOrgEx(hDC, 0, 0, NULL) ;
		SetWindowOrgEx(hDC, 0, 0, NULL) ;

		if (bLogRes)
		{
			fDpiCx = (float) GetDeviceCaps (hDC, LOGPIXELSX) ;				// 가로방향으로 로지컬 인치당 픽셀수
			fDpiCy = (float) GetDeviceCaps (hDC, LOGPIXELSY) ;				// 세로방향으로 로직컬 인치당 픽셀수
		}
		else
		{
			// HORZRES = 수평픽셀수, HORZSIZE = 밀리미터의 가로크기
			// VERTRES = 수직픽셀수, VERTSIZE = 밀리미터의 세로크기
			fDpiCx = (float) (25.4 * GetDeviceCaps (hDC, HORZRES) / GetDeviceCaps (hDC, HORZSIZE)) ;
	    	fDpiCy = (float) (25.4 * GetDeviceCaps (hDC, VERTRES) / GetDeviceCaps (hDC, VERTSIZE)) ;
		}

		pt.x = (int) (nWidth  * fDpiCx / 72);
		pt.y = (int) (nHeight * fDpiCy / 72);
	
		DPtoLP (hDC, &pt, 1) ;

		lf.lfHeight         = - (int) (fabs((float)pt.y) / 10.0 + 0.5) ;
		lf.lfWidth          = 0 ;
		lf.lfEscapement     = 0 ;
		lf.lfOrientation    = 0 ;
		lf.lfWeight         = nAttribute & EZ_ATTR_BOLD      ? 700 : 0;		///< 700 or 0 if BOLD is declared
		lf.lfItalic         = nAttribute & EZ_ATTR_ITALIC    ?   1 : 0 ;		///< If ITRALIC is declared 1 or 0
		lf.lfUnderline      = nAttribute & EZ_ATTR_UNDERLINE ?   1 : 0 ;		///< 1 or 0 if the UNDERLINE is declared
		lf.lfStrikeOut      = nAttribute & EZ_ATTR_STRIKEOUT ?   1 : 0 ;		///< If STRIKEOUT is declared 1 or 0
		lf.lfCharSet        = DEFAULT_CHARSET;									///< Default character set
		lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
		lf.lfQuality        = ANTIALIASED_QUALITY;								///< Basic 4
		lf.lfPitchAndFamily = VARIABLE_PITCH;					///< Default 0

		WCHAR wszUnicode[1024];
		memset((char*)wszUnicode, 0, sizeof(WCHAR) * (1024));
		::MultiByteToWideChar(GetACP(), 0, pFontName, -1, wszUnicode, 1024);
		lstrcpy (lf.lfFaceName, wszUnicode) ;

		hFont = CreateFontIndirect (&lf) ;
     
		if (nWidth != 0)
		{
			hOldFont = (HFONT) SelectObject (hDC, hFont) ;
			GetTextMetrics (hDC, &tm) ;
			::DeleteObject (SelectObject (hDC, hOldFont)) ;

			lf.lfWidth = (int) (tm.tmAveCharWidth *	fabs ((float)pt.x) / fabs ((float)pt.y) + 0.5) ;

			hFont = CreateFontIndirect (&lf);
		}
		
		hOldFont = (HFONT)SelectObject (hDC, hFont);
		GetTextMetrics (hDC, &tm);
		SelectObject( hDC, hOldFont );
						
		m_strKey = strKey;
		m_nPtWidth = nWidth;		// points
		m_nPtHeight = nHeight;		// points
		m_nAttributes = nAttribute;	// attributes
		m_bLogRes = bLogRes;						
 		m_dwWidth = tm.tmAveCharWidth;
		m_dwHeight = tm.tmHeight;// + tm.tmInternalLeading;
		m_dwInternal = 0;//tm.tmInternalLeading;

		m_hDC = hDC;
		m_hFont = hFont;
		
		m_strFontName = pFontName;		

		AddRef();
	}

	return true;
}

VOID CGuiFont::Unload(HDC hDC)
{
	ReleaseRef();
	if(m_nRefCount == 0 && m_hFont)
	{
		::DeleteObject(m_hFont);
		m_hFont = NULL;
	}
}

VOID CGuiFont::AddRef(VOID)
{
	m_nRefCount++;
}

VOID CGuiFont::ReleaseRef(VOID)
{
	if(--m_nRefCount < 0)
		m_nRefCount = 0;
}

std::string CGuiFont::Getkey(VOID) const
{
	return m_strKey;
}

std::string CGuiFont::GetFontName(VOID) const
{
	return m_strFontName;
}

int CGuiFont::GetWidth(VOID) const
{
	return (int)m_dwWidth;
}

int CGuiFont::GetHeight(VOID) const
{
	return (int)m_dwHeight;
}

int CGuiFont::GetInternal(VOID) const
{
	return (int)m_dwInternal;
}

CPos CGuiFont::GetTextSize(const char *pString, int nLen)
{
	SIZE size;

	HFONT hFont = (HFONT)SelectObject(m_hDC, m_hFont);

	WCHAR wszUnicode[1024];
	memset((char*)wszUnicode, 0, sizeof(WCHAR) * (1024));
	::MultiByteToWideChar(GetACP(), 0, pString, -1, wszUnicode, 1024);
	nLen = (int)lstrlen(wszUnicode);
	::GetTextExtentPoint32( m_hDC, wszUnicode, nLen, &size );

	SelectObject(m_hDC, hFont);

	return CPos(size.cx, size.cy); 
}

CPos CGuiFont::GetTextWSize(const WCHAR *pString, int nLen) 
{
	SIZE size;

	HFONT hFont = (HFONT)SelectObject(m_hDC, m_hFont);
	
	::GetTextExtentPoint32( m_hDC, pString, nLen, &size );

	SelectObject(m_hDC, hFont);

	return CPos(size.cx, size.cy); 
}

CPos CGuiFont::GetBlankTextSize(VOID)	
{
	static char temp[2] = {32,0};
	return GetTextSize(temp, 1);
}

CRectangle CGuiFont::TextOut(const char *pString, int nX /* = 0 */, int nY /* = 0 */)
{
	CRectangle rect;

	int nLen = (int)strlen(pString);

	HFONT hFont = (HFONT)SelectObject(m_hDC, m_hFont);

	SIZE size;

	WCHAR wszUnicode[1024];

	memset((char*)wszUnicode, 0, sizeof(WCHAR) * (1024));
	::MultiByteToWideChar(GetACP(), 0, pString, -1, wszUnicode, 1024);
	nLen = (int)lstrlen(wszUnicode);
	::GetTextExtentPoint32( m_hDC, wszUnicode, nLen, &size );

	rect.left = nX;
	rect.top = nY;
	rect.right = nX+size.cx;
	rect.bottom = nY+size.cy;

	HBRUSH hBrush = CreateSolidBrush(GUI_FONT_BG_COLOR);
	::FillRect(m_hDC, (LPRECT)rect, hBrush);
	::DeleteObject(hBrush);	

	SetTextColor(m_hDC, GUI_FONT_TEXT_COLOR);
	::TextOut(m_hDC, nX, nY, wszUnicode, nLen);
	
	SelectObject(m_hDC, hFont);

	return rect;
}

CRectangle CGuiFont::TextOutW(const WCHAR *pString,int nX /* = 0 */, int nY /* = 0 */)
{
	CRectangle rect;

	int nLen = (int)lstrlen(pString);

	rect = TextOutW(nLen, pString, nX, nY);

	return rect;
}

CRectangle CGuiFont::TextOutW(int nLen, const WCHAR *pString, int nX /* = 0 */, int nY /* = 0 */)
{
	CRectangle rect;

	HFONT hFont = (HFONT)SelectObject(m_hDC, m_hFont);

	SIZE size;

	::GetTextExtentPoint32( m_hDC, pString, nLen, &size );

	rect.left = nX;
	rect.top = nY;
	rect.right = nX+size.cx;
	rect.bottom = nY+size.cy;

	if( m_nAttributes & EZ_ATTR_UNDERLINE )
		rect.bottom += TEXT_UNDERLINE_SIZE;

	HBRUSH hBrush = CreateSolidBrush(GUI_FONT_BG_COLOR);
	::FillRect(m_hDC, (LPRECT)rect, hBrush);
	::DeleteObject(hBrush);
	
	SetTextColor(m_hDC, GUI_FONT_TEXT_COLOR);
	::TextOut(m_hDC, nX, nY, pString, nLen);

	SelectObject(m_hDC, hFont);

	return rect;
}

bool CGuiFont::IsValid(VOID)
{
	return (m_hFont) ? true : false;
}

HDC CGuiFont::GetHdc(VOID)
{
	return m_hDC;
}

HFONT CGuiFont::GetHFont(VOID)
{
	return m_hFont;
}

END_GUI
