#include "stdafx.h"
#include "Theme.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
//
//	CTheme
//
//////////////////////////////////////////////////////////////////////////


CTheme::CTheme( void )
{
	m_pEditFont = NULL;

	m_nHandleSize = 4;

	// Common
	m_LineColor = RGB(0, 0, 0);

	// Grid
	m_nGridX = 10;
	m_nGridY = 10;

	m_GridColor = RGB(100, 100, 100);
	m_GridBackgroundColor = RGB(224, 224, 224);

	// Note
	m_pNoteFont = new CFont();
	VERIFY(m_pNoteFont->CreateFont(14, 0, 0, 0, FW_NORMAL, false, false, false,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, _T("Arial")));

	m_nCornerSize = 20;

	m_NoteBackgroundColor = RGB(200, 255, 255);
	m_NoteTextColor = RGB(0, 0, 0);
	m_NoteShadowColor = RGB(200, 200, 200);

	// Generic entity container
	m_pContTypeFont = new CFont();
	VERIFY(m_pContTypeFont->CreateFont(16, 0, 0, 0, FW_BOLD, false, false, false,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, _T("Arial")));

	m_pContIdFont = new CFont();
	VERIFY(m_pContIdFont->CreateFont(16, 0, 0, 0, FW_NORMAL, false, false, false,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, _T("Arial")));

	m_pAttrFont = new CFont();
	VERIFY(m_pAttrFont->CreateFont(16, 0, 0, 0, FW_NORMAL, false, false, false,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, _T("Arial")));

	m_pEntityFont = new CFont();
	VERIFY(m_pEntityFont->CreateFont(16, 0, 0, 0, FW_NORMAL, false, false, false,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, _T("Arial")));

	m_pSeperatorNameFont = new CFont();
	VERIFY(m_pSeperatorNameFont->CreateFont(14, 0, 0, 0, FW_BOLD, false, false, false,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, _T("Arial")));

	m_nContTypeHeight = 20;
	m_nContIdHeight = 20;
	m_nAttrHeight = 20;
	m_nEntityHeight = 20;
	m_nSeperatorNameHeight = 16;

	m_GECBackgroundColor = RGB(200, 255, 200);
	m_ContTypeColor = RGB(0, 0, 0);
	m_ContIdColor = RGB(0, 0, 0);
	m_AttrColor = RGB(100, 0, 0);
	m_EntityColor = RGB(0, 0, 100);
	m_SeperatorNameColor = RGB(128, 128, 128);

	// Link
	m_LineColor_Def = RGB(0, 0, 0);
	m_LineColor_Yes = RGB(0, 0, 0);
	m_LineColor_No = RGB(0, 0, 0);
	m_LineColor_Error = RGB(255, 0, 0);
	m_LineColor_LogIn = RGB(0, 0, 255);

	m_pBranchNameFont = new CFont();
	VERIFY(m_pBranchNameFont->CreateFont(16, 0, 0, 0, FW_NORMAL, false, false, false,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, _T("Arial")));
}

CTheme::~CTheme()
{
	delete m_pBranchNameFont;

	delete m_pSeperatorNameFont;
	delete m_pEntityFont;
	delete m_pAttrFont;
	delete m_pContIdFont;
	delete m_pContTypeFont;

	delete m_pNoteFont;
}

CFont *CTheme::EditFont( CFont *pRefFont, double scale ) const
{
	LOGFONT logFont;
	pRefFont->GetLogFont( &logFont );

	logFont.lfHeight = long(logFont.lfHeight * scale);

	delete m_pEditFont;
	m_pEditFont = new CFont;
	m_pEditFont->CreateFontIndirect( &logFont );
	return m_pEditFont;
}


