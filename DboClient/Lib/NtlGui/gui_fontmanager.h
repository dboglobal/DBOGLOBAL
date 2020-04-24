////////////////////////////////////////////////////////////////////////////////
// Name: class gui::CFontmanager
// Desc: Text를 표현한다.
//
// Created by jhssugi@ntl-inc.com	
// 2007.01.26 Peessi@hitel.net   
//
// To Be Desired: 
//	1. 폰트를 보안을 위하여 파일로 읽지말고 메모리로 읽는 것으로.
////////////////////////////////////////////////////////////////////////////////
#ifndef __GUI_FONTMANAGER_H__
#define __GUI_FONTMANAGER_H__

#include "gui_define.h"
#include "rectangle.h"

extern LARGE_INTEGER g_FontTime;

START_GUI

#define GUI_FONT_OUTLINE_MAXVALUE	4

class CGuiFont;
class CTexture;

typedef struct _stGUIFONTINFO
{
	std::string	strFilename;
	std::string strFontname;
}stGUIFONTINFO;

class CFontManager
{
public:

	CFontManager();
	~CFontManager();

	bool Initialize( const char* pFilename );
	VOID Terminate(VOID);
	
	CGuiFont* CreateGuiFont( const char *pFontName, int nHeight, int nAttribute );
	VOID DeleteGuiFont( CGuiFont *pFont );

	VOID BitmapToTexture( CTexture *pTexture, const CRectangle& rect, COLORREF textcolor, 
						  INT nEffectMode = TE_NONE, COLORREF effectcolor = 0, INT nEffectValue = 0, 
						  BOOL bUseBgColor = FALSE, COLORREF bgcolor = 0 );
	VOID BitmapToTexture( CTexture *pTexture, const CRectangle& rtSrc, const CRectangle& rtDest, COLORREF textcolor, 
						  INT nEffectMode = TE_NONE, COLORREF effectcolor = 0, INT nEffectValue = 0,
						  BOOL bUseBgColor = FALSE, COLORREF bgcolor = 0 );

	VOID SetBitmapDCWidth(int nDCWidth);
	VOID SetBitmapDCHeight(int nDCHeight);
	int GetBitmapDCWidth(VOID);
	int GetBitmapDCHeight(VOID);

private:
	VOID CreateBitmapDC(VOID);
	VOID ReleaseBitmapDC(VOID);

	std::string GetKey(const char *pFontName, int nWidth, int nHeight, int nAttribute);
	CGuiFont* Find(std::string& strKey);

	BOOL RegisterFont( std::string& strFontFilename, std::string& strFontname, std::string& strDefinename );
	VOID UnRegisterFont( std::string strDefinename, BOOL bDelIter = TRUE );	
	stGUIFONTINFO* GetFontInfo( std::string strDefinename );

	BOOL ParseFontData( std::string& pFilename );

	// Image Processing
	BYTE GetOutlineAlpha( INT nEffectValue, INT nX, INT nY, BYTE byAlpha );
	
	inline VOID SetBlendedPixel( WORD* pDest, BYTE byAlpha, DWORD dwColor );
	inline VOID SetBlendedPixel( WORD* pDest, BYTE byAlpha, BYTE byRed, BYTE byGreen, BYTE byBlue );
	inline VOID SetPixel( WORD* pDest, BYTE byAlpha, DWORD dwColor );
	inline VOID SetPixel( WORD* pDest, BYTE byAlpha, BYTE byRed, BYTE byGreen, BYTE byBlue );
	inline BYTE GetRevisedAlpha( DWORD* pBitmap, INT nEffectMode );	

private:

	HDC m_hDC;
	HBITMAP m_hbmBitmap;
	HBITMAP	m_hbmOldBitmap;
	DWORD *m_pBitmapBits;
	int m_nDCWidth;
	int m_nDCHeight;
	int m_nDCShift;

	std::map<std::string,CGuiFont*> m_mapFont;				// 폰트키
	std::map<std::string,stGUIFONTINFO> m_mapFontResource;	// 정의된폰트이름. 폰트실제이름.

	FLOAT* m_pOutlineMatrix[GUI_FONT_OUTLINE_MAXVALUE];
	INT	   m_nOutlineMatrixIndex[GUI_FONT_OUTLINE_MAXVALUE];
};

extern CFontManager g_FontMgr;

CFontManager* GetGuiFontManager(VOID);

END_GUI

#endif
