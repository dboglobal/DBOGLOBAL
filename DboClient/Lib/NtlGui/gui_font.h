#ifndef __GUI_FONT_H__
#define __GUI_FONT_H__

#include "gui_define.h"
#include "position.h"
#include "rectangle.h"

START_GUI

#define EZ_ATTR_BOLD			1
#define EZ_ATTR_ITALIC			2
#define EZ_ATTR_UNDERLINE		4
#define EZ_ATTR_STRIKEOUT		8

#define TEXT_UNDERLINE_SIZE		1

class CGuiFont
{
private:

	VOID AddRef(VOID);
	VOID ReleaseRef(VOID);

public:

	CGuiFont();
	~CGuiFont();

	bool Load(HDC hDC, const char *pFontName, std::string& strKey, int nWidth, int nHeight, int nAttribute, bool bLogRes);
	VOID Unload(HDC hDC);

	std::string Getkey(VOID) const;
	std::string GetFontName(VOID) const;

	int GetWidth(VOID) const;
	int GetHeight(VOID) const;
	int GetInternal(VOID) const;

	CPos GetTextSize(const char *pString, int nLen); 
	CPos GetTextWSize(const WCHAR *pString, int nLen); 
	CPos GetBlankTextSize(VOID);

	CRectangle TextOut(const char *pString, int nX = 0, int nY = 0);
	CRectangle TextOutW(const WCHAR *pString,int nX = 0, int nY = 0);
	CRectangle TextOutW(int nLen, const WCHAR *pString, int nX = 0, int nY = 0);

	bool IsValid(VOID);

	HDC GetHdc(VOID);
	HFONT GetHFont(VOID);


private:

	std::string m_strKey;	// for reconstruction
	std::string m_strFontName;
	HDC m_hDC;
	HFONT m_hFont;
	int	m_nPtHeight;		// points
	int	m_nPtWidth;			// points
	int	m_nAttributes;		// attributes
	bool m_bLogRes;
	unsigned int m_dwWidth;		// width
	unsigned int m_dwHeight;	// height
	unsigned int m_dwInternal;	// internal leading

	int	m_nRefCount;
};

END_GUI

#endif
