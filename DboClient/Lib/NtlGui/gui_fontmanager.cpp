#include "gui_precomp.h"
#include "gui_fontmanager.h"
#include "gui_font.h"
#include "gui_texture.h"
#include "gui_componenttokenizer.h"
#include <direct.h>

LARGE_INTEGER g_FontTime;

START_GUI

#define GUI_FONT_ALPHA_FIX32(fixvalue) \
	uAlpha = (unsigned char)(*pSrc32);	 \
	if( uAlpha == 255 )					 \
		uAlpha = 255 << 24;				 \
	else								 \
		uAlpha = ( uAlpha > fixvalue ) ? ( uAlpha - fixvalue ) << 24 : 0 ;


#define GUI_FONT_ALPHA_FIX16(fixvalue) \
	uAlpha = (unsigned char)(*pSrc32);	 \
	if( uAlpha == 255 )					 \
		uAlpha = ( 255 >> 4 ) << 12;				 \
	else								 \
		uAlpha = ( uAlpha > fixvalue ) ? ( ( uAlpha - fixvalue ) >> 4 ) << 12 : 0 ;

#define GUI_FONT_ALPHA_FIX_VALUE	48

CFontManager g_FontMgr;

CFontManager* GetGuiFontManager(VOID) { return &g_FontMgr; }

CFontManager::CFontManager()
{
	m_hDC = NULL;
	m_hbmBitmap = NULL;
	m_pBitmapBits = NULL;
	m_nDCWidth = 1024;
	m_nDCHeight = 256;
	m_nDCShift = 10;   // m_nDCWidth = 2^10; 	
}

CFontManager::~CFontManager()
{
	Terminate();
}

bool CFontManager::Initialize( const char* pFilename )
{
	if (!ParseFontData(std::string(pFilename)))
	{
		DBO_ASSERT(FALSE, "Gui Font Parse Fail");
		return false;
	}

	CreateBitmapDC();

	return true;
}

VOID CFontManager::Terminate(VOID)
{
	bool bValid;
	std::map<std::string, CGuiFont*>::iterator the;
	for(the = m_mapFont.begin(); the != m_mapFont.end(); the++)
	{
		bValid = (*the).second->IsValid();

		while(bValid)
		{
			(*the).second->Unload(m_hDC);  
			bValid = (*the).second->IsValid(); 
		}

		NTL_DELETE( (*the).second );
	}

	m_mapFont.clear(); 

	std::map<std::string,stGUIFONTINFO>::iterator it;
	for( it = m_mapFontResource.begin() ; it != m_mapFontResource.end() ; ++it )
		UnRegisterFont( it->first, FALSE );	
	
	m_mapFontResource.clear();

	ReleaseBitmapDC();
}

CGuiFont* CFontManager::CreateGuiFont(const char *pFontName, int nHeight, int nAttribute)
{
	stGUIFONTINFO* pFontInfo = GetFontInfo( pFontName );

	std::string strKey;
	
	if( pFontInfo )
	{
		strKey = GetKey( pFontInfo->strFontname.c_str(), 0, nHeight, nAttribute );
		pFontName = pFontInfo->strFontname.c_str();
	}
	else
		strKey = GetKey( pFontName, 0, nHeight, nAttribute );

	CGuiFont *pFont = Find(strKey);
	
	if(pFont == NULL)
	{
		pFont = NTL_NEW CGuiFont;
		if(!pFont->Load(m_hDC, pFontName, strKey, 0, nHeight, nAttribute, true))
		{
			NTL_DELETE( pFont );
		}
		else
		{
            m_mapFont[strKey] = pFont;
		}
	}
	else
	{
		pFont->Load(m_hDC, pFontName, strKey, 0, nHeight, nAttribute, true);
	}

	return pFont;
}

VOID CFontManager::DeleteGuiFont(CGuiFont *pFont)
{
	if(pFont == NULL)
		return;

	std::string strKey = pFont->Getkey();
	pFont->Unload(m_hDC);

	if(!pFont->IsValid())
	{
		std::map<std::string, CGuiFont*>::iterator the; 
		the = m_mapFont.find(strKey);
		m_mapFont.erase(the);
		
		std::string strFontName = pFont->GetFontName();

		NTL_DELETE( pFont );
	}
}

VOID CFontManager::BitmapToTexture( CTexture *pTexture, const CRectangle& rect, COLORREF textcolor,		
								    INT nEffectMode /* = TE_NONE */, COLORREF effectcolor /* = 0 */, INT nEffectValue /* = 0 */,
									BOOL bUseBgColor /* = FALSE */, COLORREF bgcolor /* = 0  */ )
{
	// TEST BLOCK START
	LARGE_INTEGER begin_tick;
	QueryPerformanceCounter( &begin_tick );
	// TEST BLOCK END

	INT nTWidth = pTexture->GetWidth();
	INT nTHeight = pTexture->GetHeight();

	INT nWidth = rect.GetWidth();
	INT nHeight = rect.GetHeight();

	if( rect.right > nTWidth )
		nWidth -= rect.right - nTWidth;
	
	if( rect.bottom > nTHeight )
		nHeight -= rect.bottom - nTHeight;

	if( nWidth < 0 || nHeight < 0 )
		return;

	BYTE byRed, byGreen, byBlue; byRed = byGreen = byBlue = 0;
	BYTE* pBits = pTexture->LockWrite(); 
	if( !pBits )
	{
		DBO_ASSERT( pBits, "pBits is Null" );
		return;
	}	

	INT nStride = pTexture->GetStride();
	
	DBO_ASSERT( pBits, "pBits is Null" );
	
	unsigned char byBgRed, byBgGreen, byBgBlue;
	byBgRed = GetRValue( bgcolor ); 
	byBgBlue = GetBValue( bgcolor );
	byBgGreen = GetGValue( bgcolor );	

	unsigned char byTextRed, byTextGreen, byTextBlue;
	byTextRed = GetRValue( textcolor ); 
	byTextBlue = GetBValue( textcolor );
	byTextGreen = GetGValue( textcolor );

	unsigned char byEffectRed, byEffectGreen, byEffectBlue;
	byEffectRed = GetRValue( effectcolor ); 
	byEffectBlue = GetBValue( effectcolor );
	byEffectGreen = GetGValue( effectcolor );

	//WORD wEffectColor = (WORD)( ( GetRValue( effectcolor )  >> 4 ) << 8 ) | ( ( GetGValue( effectcolor ) >> 4 ) << 4) | ( GetBValue( effectcolor ) >> 4 );									
	//WORD wTextColor = (WORD)( ( GetRValue( textcolor )  >> 4 ) << 8 ) | ( ( GetGValue( textcolor ) >> 4) << 4 ) | ( GetBValue( textcolor ) >> 4 );									

	DWORD *pSrc32 = m_pBitmapBits;

#ifdef FONT_USE_32BIT
	DWORD *pDest32;
	for (int i = 0; i < nHeight; i++)
	{
		pDest32 = (DWORD*)(pBits + nStride*(i+rect.top) + rect.left*sizeof(DWORD));
		pSrc32 = (m_pBitmapBits + (i << m_nDCShift));

		for (int j = 0; j < nWidth; j++)
		{
			GUI_FONT_ALPHA_FIX32( GUI_FONT_ALPHA_FIX_VALUE );
			
			if( bUseBgColor )
			{
				if( uAlpha == 0 )
					*pDest32 = (DWORD)( ((byBgRed) << 16) | ((byBgGreen) << 8) | (byBgBlue) | 0xff000000 );				
				else
					*pDest32 = (DWORD)( ((byTextRed) << 16) | ((byTextGreen) << 8) | (byTextBlue) | uAlpha );				
			}
			else
			{
				*pDest32 = (DWORD)( ((byTextRed) << 16) | ((byTextGreen) << 8) | (byTextBlue) | uAlpha );				
			}

			pSrc32++;
			pDest32++;
		}
	}	
#else
	WORD* pDest16;

	if( nEffectMode == TE_OUTLINE )
	{
		if( nEffectValue > GUI_FONT_OUTLINE_MAXVALUE )
			nEffectValue = GUI_FONT_OUTLINE_MAXVALUE;	

		INT nRevisedEffectValue = ( nEffectValue * 2 ) + 3;

		for( INT i = 0 ; i < nHeight ; ++i )
		{
			pDest16 = (WORD*)( pBits + nStride * ( i + rect.top ) + rect.left * sizeof( WORD ) );
			pSrc32 = ( m_pBitmapBits + ( i << m_nDCShift ) );

			for( INT j = 0 ; j < nWidth ; ++j )
			{
				if( bUseBgColor )
					SetPixel( pDest16, 255, bgcolor );

				BYTE byAlpha = GetRevisedAlpha( pSrc32, nEffectMode );
				
				if( byAlpha )
				{
					for( INT x = 0 ; x < nRevisedEffectValue ; ++x )
					{
						INT nRevisedXOffset = x - nRevisedEffectValue / 2;

						for( int y = 0 ; y < nRevisedEffectValue ; ++y )
						{
							INT nRevisedYOffset = y - nRevisedEffectValue / 2;

							if( rect.left + j + nRevisedXOffset < 0 || rect.left + j + nRevisedXOffset >= nTWidth ||
								rect.top + i + nRevisedYOffset < 0 || rect.top + i + nRevisedYOffset >= nTHeight )
								continue;

							WORD* pOutLineDest = (WORD*)( (BYTE*)pDest16 + nStride * nRevisedYOffset + nRevisedXOffset * sizeof( WORD ) );							

							if( *pOutLineDest )
							{
								DWORD dwResultAlpha = ( ( *pOutLineDest >> 12 ) << 4 ) + GetOutlineAlpha( nEffectValue, x, y, byAlpha );
								if( dwResultAlpha > 255 )
									dwResultAlpha = 255;								
								SetPixel( pOutLineDest, (BYTE)dwResultAlpha, byEffectRed, byEffectGreen, byEffectBlue );
							}
							else
								SetPixel( pOutLineDest, GetOutlineAlpha( nEffectValue, x, y, byAlpha ), byEffectRed, byEffectGreen, byEffectBlue );			

							//if( *pOutLineDest )
							//{
							//	DWORD dwResultAlpha = ( ( ( ( *pOutLineDest >> 12 ) << 4 ) + GetOutlineAlpha( nEffectValue, x, y, byAlpha ) ) );
							//	if( dwResultAlpha > 255 )
							//		dwResultAlpha = 255;								
							//	wResult = wEffectColor | dwResultAlpha >> 4 << 12;								
							//	*pOutLineDest = wResult;
							//}
							//else
							//{
							//	wResult = wEffectColor | ( GetOutlineAlpha( nEffectValue, x, y, byAlpha ) >> 4 << 12 );
							//	*pOutLineDest =wResult;
							//}
						}
					}
				}				

				pSrc32++;
				pDest16++;
			}
		}

		for( INT i = 0 ; i < nHeight ; ++i )
		{
			pDest16 = (WORD*)( pBits + nStride * ( i + rect.top ) + rect.left * sizeof( WORD ) );
			pSrc32 = ( m_pBitmapBits + ( i << m_nDCShift ) );

			for( INT j = 0 ; j < nWidth ; ++j )
			{
				BYTE byAlpha = GetRevisedAlpha( pSrc32, nEffectMode );

				if( byAlpha )
					SetBlendedPixel( pDest16, byAlpha, byTextRed, byTextGreen, byTextBlue );	

				pSrc32++;
				pDest16++;
			}
		}	
	}	
	else
	{
		for( INT i = 0 ; i < nHeight ; ++i )
		{
			pDest16 = (WORD*)( pBits + nStride * ( i + rect.top ) + rect.left * sizeof( WORD ) );
			pSrc32 = ( m_pBitmapBits + ( i << m_nDCShift ) );

			for( INT j = 0 ; j < nWidth ; ++j )
			{
				BYTE byAlpha = GetRevisedAlpha( pSrc32, nEffectMode );

				if( bUseBgColor )
					SetPixel( pDest16, 255, bgcolor );

				if( byAlpha )
				{
					if( nEffectMode == TE_NONE )
					{
						if( bUseBgColor )
							SetBlendedPixel( pDest16, byAlpha, byTextRed, byTextGreen, byTextBlue );
						else
							SetPixel( pDest16, byAlpha, byTextRed, byTextGreen, byTextBlue );
					}
					else if( nEffectMode == TE_SHADOW )
					{
						WORD* pShadowDest16 = (WORD*)( pBits + nStride * ( i + nEffectValue + rect.top ) + ( j + rect.left + nEffectValue ) * sizeof( WORD ) );

						if( rect.right + nEffectValue < nTWidth && rect.bottom + nEffectValue < nTHeight )
							SetPixel( pShadowDest16, byAlpha, byEffectRed, byEffectGreen, byEffectBlue );						

						SetBlendedPixel( pDest16, byAlpha, byTextRed, byTextGreen, byTextBlue );					
					}						
				}							

				pSrc32++;
				pDest16++;
			}
		}	
	}	

#endif

	pTexture->UnLock();

	LARGE_INTEGER end_tick;
	QueryPerformanceCounter( &end_tick );

	if( g_FontTime.QuadPart < end_tick.QuadPart - begin_tick.QuadPart )
		g_FontTime.QuadPart = end_tick.QuadPart - begin_tick.QuadPart;
}

VOID CFontManager::BitmapToTexture( CTexture *pTexture, const CRectangle& rtSrc, const CRectangle& rtDest, COLORREF textcolor,
								    INT nEffectMode /* = TE_NONE */, COLORREF effectcolor /* = 0 */, INT nEffectValue /* = 0 */,
									BOOL bUseBgColor /* = FALSE */, COLORREF bgcolor /* = 0  */ )
{
	INT nTWidth = pTexture->GetWidth();
	INT nTHeight = pTexture->GetHeight();

	INT nSrcWidth = rtSrc.GetWidth();
	INT nSrcHeight = rtSrc.GetHeight();

	if( rtDest.right > nTWidth )
		nSrcWidth -= rtDest.right - nTWidth;
	
	if( rtDest.bottom > nTHeight )
		nSrcHeight -= rtDest.bottom - nTHeight;

	if( nSrcHeight < 0 || nSrcWidth < 0 )
		return;

	unsigned char byRed, byGreen, byBlue; byRed = byGreen = byBlue = 0;
	unsigned char *pBits = pTexture->LockWrite(); 

	if( !pBits )
	{
		DBO_ASSERT( pBits, "pBits is Null" );
		return;
	}

	int nStride = pTexture->GetStride();

	DBO_ASSERT( pBits, "pBits is Null" );	
	
	unsigned char byBgRed, byBgGreen, byBgBlue;
	byBgRed = GetRValue( bgcolor ); 
	byBgBlue = GetBValue( bgcolor );
	byBgGreen = GetGValue( bgcolor );	

	unsigned char byTextRed, byTextGreen, byTextBlue;
	byTextRed = GetRValue( textcolor ); 
	byTextBlue = GetBValue( textcolor );
	byTextGreen = GetGValue( textcolor );

	unsigned char byEffectRed, byEffectGreen, byEffectBlue;
	byEffectRed = GetRValue( effectcolor ); 
	byEffectBlue = GetBValue( effectcolor );
	byEffectGreen = GetGValue( effectcolor );

	DWORD *pSrc32 = m_pBitmapBits;

#ifdef FONT_USE_32BIT
    unsigned int uAlpha;
	DWORD *pDest32;
	for (int i = 0; i < nSrcHeight; i++)
	{
		pDest32 = (DWORD*)(pBits + nStride*(i+rtDest.top) + rtDest.left*sizeof(DWORD));
		pSrc32 = (m_pBitmapBits + ((i+rtSrc.top) << m_nDCShift));

		for (int j = 0; j < nSrcWidth; j++)
		{
			GUI_FONT_ALPHA_FIX32( GUI_FONT_ALPHA_FIX_VALUE );

			if( bUseBgColor )
			{
				if( uAlpha == 0 )
					*pDest32 = (DWORD)( ((byBgRed) << 16) | ((byBgGreen) << 8) | (byBgBlue) | 0xff000000 );				
				else
					*pDest32 = (DWORD)( ((byTextRed) << 16) | ((byTextGreen) << 8) | (byTextBlue) | uAlpha );				
			}
			else
			{
				*pDest32 = (DWORD)( ((byTextRed) << 16) | ((byTextGreen) << 8) | (byTextBlue) | uAlpha );				
			}

			pSrc32++;
			pDest32++;
		}
	}	
#else
	WORD* pDest16;

	if( nEffectMode == TE_OUTLINE )
	{
		if( nEffectValue > GUI_FONT_OUTLINE_MAXVALUE )
			nEffectValue = GUI_FONT_OUTLINE_MAXVALUE;	

		INT nRevisedEffectValue = ( nEffectValue * 2 ) + 3;

		for( INT i = 0 ; i < nSrcHeight ; ++i )
		{
			pDest16 = (WORD*)( pBits + nStride * ( i + rtDest.top ) + rtDest.left * sizeof( WORD ) );
			pSrc32 = ( m_pBitmapBits + ( ( i + rtSrc.top ) << m_nDCShift ) ) ;

			for( INT j = 0 ; j < nSrcWidth ; ++j )
			{
				if( bUseBgColor )
					SetPixel( pDest16, 255, bgcolor );

				BYTE byAlpha = GetRevisedAlpha( pSrc32, nEffectMode );

				if( byAlpha )
				{
					for( INT x = 0 ; x < nRevisedEffectValue ; ++x )
					{
						INT nRevisedXOffset = x - nRevisedEffectValue / 2;

						for( int y = 0 ; y < nRevisedEffectValue ; ++y )
						{
							INT nRevisedYOffset = y - nRevisedEffectValue / 2;

							if( rtDest.left + j + nRevisedXOffset < 0 || rtDest.left + j + nRevisedXOffset >= nTWidth ||
								rtDest.top + i + nRevisedYOffset < 0 || rtDest.top + i + nRevisedYOffset >= nTHeight )
								continue;

							WORD* pOutLineDest = (WORD*)( (BYTE*)pDest16 + nStride * nRevisedYOffset + nRevisedXOffset * sizeof( WORD ) );							

							if( *pOutLineDest )
							{
								DWORD dwResultAlpha = ( ( *pOutLineDest >> 12 ) << 4 ) + GetOutlineAlpha( nEffectValue, x, y, byAlpha );
								if( dwResultAlpha > 255 )
									dwResultAlpha = 255;								
								SetPixel( pOutLineDest, (BYTE)dwResultAlpha, byEffectRed, byEffectGreen, byEffectBlue );
							}
							else
								SetPixel( pOutLineDest, GetOutlineAlpha( nEffectValue, x, y, byAlpha ), byEffectRed, byEffectGreen, byEffectBlue );			
						}
					}
				}

				pSrc32++;
				pDest16++;
			}			
		}	

		for( INT i = 0 ; i < nSrcHeight ; ++i )
		{
			pDest16 = (WORD*)( pBits + nStride * ( i + rtDest.top ) + rtDest.left * sizeof( WORD ) );
			pSrc32 = ( m_pBitmapBits + ( ( i + rtSrc.top ) << m_nDCShift ) ) ;

			for( INT j = 0 ; j < nSrcWidth ; ++j )
			{
				BYTE byAlpha = GetRevisedAlpha( pSrc32, nEffectMode );

				if( byAlpha )
					SetBlendedPixel( pDest16, byAlpha, byTextRed, byTextGreen, byTextBlue );	

				pSrc32++;
				pDest16++;
			}
		}	
	}
	else
	{
		for( INT i = 0 ; i < nSrcHeight ; ++i )
		{
			pDest16 = (WORD*)( pBits + nStride * ( i + rtDest.top ) + rtDest.left * sizeof( WORD ) );
			pSrc32 = ( m_pBitmapBits + ( ( i + rtSrc.top ) << m_nDCShift ) ) ;

			for( INT j = 0 ; j < nSrcWidth ; ++j )
			{
				BYTE byAlpha = GetRevisedAlpha( pSrc32, nEffectMode );

				if( bUseBgColor )
					SetPixel( pDest16, 255, bgcolor );

				if( byAlpha )
				{
					if( nEffectMode == TE_NONE )
					{
						if( bUseBgColor )
							SetBlendedPixel( pDest16, byAlpha, byTextRed, byTextGreen, byTextBlue );
						else
							SetPixel( pDest16, byAlpha, byTextRed, byTextGreen, byTextBlue );
					}
					else if( nEffectMode == TE_SHADOW )
					{
						WORD* pShadowDest16 = (WORD*)( pBits + nStride * ( i + nEffectValue + rtDest.top ) + ( j + rtDest.left + nEffectValue ) * sizeof( WORD ) );

						if( rtDest.right + nEffectValue < nTWidth && rtDest.bottom + nEffectValue < nTHeight )
							SetPixel( pShadowDest16, byAlpha, byEffectRed, byEffectGreen, byEffectBlue );						

						SetBlendedPixel( pDest16, byAlpha, byTextRed, byTextGreen, byTextBlue );					
					}						
				}							

				pSrc32++;
				pDest16++;
			}
		}	
	}
	
#endif

	pTexture->UnLock();
}

VOID CFontManager::CreateBitmapDC(VOID)
{	
	if(m_hDC)
		return;

	BITMAPINFO bmi;
	ZeroMemory( &bmi.bmiHeader,  sizeof(BITMAPINFOHEADER) );
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       =  (int)m_nDCWidth;
	bmi.bmiHeader.biHeight      = -(int)m_nDCHeight;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount    = 32;

	// Create a DC and a bitmap for the font
	m_hDC = CreateCompatibleDC( NULL );
	m_hbmBitmap = CreateDIBSection( m_hDC, &bmi, DIB_RGB_COLORS, (VOID**)&m_pBitmapBits, NULL, 0 );

	m_hbmOldBitmap = (HBITMAP)SelectObject(m_hDC, m_hbmBitmap);
	SetBkMode(m_hDC, TRANSPARENT); 
	SetTextAlign(m_hDC, TA_TOP);
	SetMapMode(m_hDC, MM_TEXT);	
}

VOID CFontManager::ReleaseBitmapDC(VOID)
{
	DeleteObject( SelectObject(m_hDC, m_hbmOldBitmap) );
	DeleteDC( m_hDC );
}

VOID CFontManager::SetBitmapDCWidth(int nDCWidth)
{
	m_nDCWidth = nDCWidth;
}

VOID CFontManager::SetBitmapDCHeight(int nDCHeight)
{
	m_nDCHeight = nDCHeight;
}

int CFontManager::GetBitmapDCWidth(VOID)
{
	return m_nDCWidth;
}

int CFontManager::GetBitmapDCHeight(VOID)
{
	return m_nDCHeight;
}


std::string CFontManager::GetKey(const char *pFontName, int nWidth, int nHeight, int nAttribute)
{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	char chBuff[1024];
	sprintf_s(chBuff, 1024, "%s%d%d%d", pFontName, nWidth, nHeight, nAttribute);
#else
	char chBuff[1024];
	sprintf(chBuff, "%s%d%d%d", pFontName, nWidth, nHeight, nAttribute);
#endif
	
	return chBuff;
}

CGuiFont* CFontManager::Find(std::string& strKey)
{
	std::map<std::string, CGuiFont*>::iterator the;
	the = m_mapFont.find(strKey);
	if(the == m_mapFont.end())
		return NULL;

	return (*the).second;  
}

BOOL CFontManager::RegisterFont( std::string& strFontFilename, std::string& strFontname, std::string& strDefinename )
{
	BOOL bRes = TRUE;
	//DBO_WARNING_MESSAGE("RegisterFont: " << strFontFilename.c_str() << ", strFontname: "  << strFontname.c_str() << ", strDefinename: " << strDefinename.c_str());
	if( strFontFilename != "system" )
	{
		char buf[GUI_MAX_PATH];
		_getcwd( buf, GUI_MAX_PATH );

		std::string strWorkDir = buf;
		std::string strFullPath = strWorkDir + "\\font\\" + strFontFilename;

		WCHAR wbuf[GUI_MAX_PATH];
		memset( (char*)wbuf, 0, sizeof( WCHAR ) * GUI_MAX_PATH );
		::MultiByteToWideChar( GetACP(), 0, strFullPath.c_str(), -1, wbuf, GUI_MAX_PATH );

		if( !AddFontResourceEx( wbuf, FR_PRIVATE | FR_NOT_ENUM, NULL ) )
		{
			DBO_ASSERT(FALSE, "AddFontResourceEx Fail" );
			bRes = FALSE;
		}
	}

	stGUIFONTINFO stFontInfo;
	stFontInfo.strFilename = strFontFilename;
	stFontInfo.strFontname = strFontname;

	m_mapFontResource[strDefinename] = stFontInfo;
	
	return bRes;
}

VOID CFontManager::UnRegisterFont( std::string strDefinename, BOOL bDelIter /* = TRUE  */ )
{
	std::map<std::string,stGUIFONTINFO>::iterator it;

	it = m_mapFontResource.find( strDefinename );

	if( it == m_mapFontResource.end() )
		return;

	stGUIFONTINFO* stFontInfo = &it->second;

	if( stFontInfo->strFilename != "system" )
	{
		char buf[GUI_MAX_PATH];
		_getcwd( buf, GUI_MAX_PATH );
		std::string strWorkDir = buf;
		std::string strFullPath = strWorkDir + "\\font\\" + stFontInfo->strFilename;

		WCHAR wbuf[GUI_MAX_PATH];
		memset( (char*)wbuf, 0, sizeof( WCHAR ) * GUI_MAX_PATH );
		::MultiByteToWideChar( GetACP(), 0, strFullPath.c_str(), -1, wbuf, GUI_MAX_PATH );

		if( !RemoveFontResourceEx( wbuf, FR_PRIVATE | FR_NOT_ENUM, NULL ) )
		{
			_ASSERT( "RemoveFontResourceEx Fail" );			
		}
	}

	if( bDelIter )
		m_mapFontResource.erase( it );
}

stGUIFONTINFO* CFontManager::GetFontInfo( std::string strDefinename )
{
	std::map<std::string,stGUIFONTINFO>::iterator it;

	it = m_mapFontResource.find( strDefinename );

	if( it == m_mapFontResource.end() )
		return NULL;
	
	return &it->second;
}

BOOL CFontManager::ParseFontData( std::string& pFilename )
{
	CComponentTokenizer	lexer( pFilename );

	if( !lexer.IsSuccess() )
		return FALSE;

	enum EParseState
	{
		PARSE_STATE_USERDEFINENAME,
		PARSE_STATE_FILENAME,
		PARSE_STATE_FONTNAME
	};

	EParseState eState = PARSE_STATE_USERDEFINENAME;
	std::string		strDefineName, strFileName, strFontName;

	while( TRUE )
	{
		const std::string& token = lexer.PeekNextToken();

		if( token.empty() )
			break;

		switch( eState )
		{
		case PARSE_STATE_USERDEFINENAME:
			{
				if( token == "=" )
				{
					eState = PARSE_STATE_FILENAME;
				}
				else
				{
					strDefineName += token;
				}			
			}			
			break;
		case PARSE_STATE_FILENAME:
			{
				if( token == "," )
				{
					eState = PARSE_STATE_FONTNAME;
				}
				else
				{
					strFileName += token;
				}
			}
			break;
		case PARSE_STATE_FONTNAME:
			{
				if( token == ";" )
				{
					eState = PARSE_STATE_USERDEFINENAME;
					
					if (RegisterFont(strFileName, strFontName, strDefineName) == FALSE)
						return FALSE;

					strDefineName.clear();
					strFileName.clear();
					strFontName.clear();
				}
				else
				{
					strFontName += token;
				}

			}
			break;
		}

		lexer.PopToPeek();
	}

	return TRUE;
}

BYTE CFontManager::GetOutlineAlpha( INT nEffectValue, INT nX, INT nY, BYTE byAlpha )
{
	static FLOAT GAUSSIAN3X3[3][3] = { 1/2.0f, 2/2.0f, 1/2.0f,
		2/2.0f, 4/2.0f, 2/2.0f,
		1/2.0f, 2/2.0f, 1/2.0f };

	static FLOAT GAUSSIAN5X5[5][5] = { 1/4.0f, 1/4.0f, 2/4.0f, 1/4.0f, 1/4.0f,
		1/4.0f, 2/4.0f, 4/4.0f, 2/4.0f, 1/4.0f,	
		2/4.0f, 4/4.0f, 8/4.0f, 4/4.0f, 2/4.0f,
		1/4.0f, 2/4.0f, 4/4.0f, 2/4.0f, 1/4.0f,	
		1/4.0f, 1/4.0f, 2/4.0f, 1/4.0f, 1/4.0f };

	static FLOAT GAUSSIAN7X7[7][7] = { 1/8.0f, 1/8.0f, 1/8.0f, 2/8.0f, 1/8.0f, 1/8.0f, 1/8.0f,
		1/8.0f, 1/8.0f, 2/8.0f, 4/8.0f, 2/8.0f, 1/8.0f, 1/8.0f,
		1/8.0f, 2/8.0f, 4/8.0f, 8/8.0f, 4/8.0f, 2/8.0f, 1/8.0f,
		2/8.0f, 4/8.0f, 8/8.0f, 16/8.0f, 8/8.0f, 4/8.0f, 2/8.0f,
		1/8.0f, 2/8.0f, 4/8.0f, 8/8.0f, 4/8.0f, 2/8.0f, 1/8.0f,
		1/8.0f, 1/8.0f, 2/8.0f, 4/8.0f, 2/8.0f, 1/8.0f, 1/8.0f,
		1/8.0f, 1/8.0f, 1/8.0f, 2/8.0f, 1/8.0f, 1/8.0f, 1/8.0f };

	static FLOAT GAUSSIAN9X9[9][9] = {	1/16.0f, 1/16.0f, 1/16.0f, 2/16.0f, 2/16.0f, 2/16.0f, 1/16.0f, 1/16.0f, 1/16.0f,
										1/16.0f, 1/16.0f, 2/16.0f, 2/16.0f, 4/16.0f, 2/16.0f, 2/16.0f, 1/16.0f, 1/16.0f,
										1/16.0f, 2/16.0f, 2/16.0f, 4/16.0f, 8/16.0f, 4/16.0f, 2/16.0f, 2/16.0f, 1/16.0f,
										2/16.0f, 2/16.0f, 4/16.0f, 8/16.0f, 16/16.0f, 8/16.0f, 4/16.0f, 2/16.0f, 2/16.0f,
										2/16.0f, 4/16.0f, 8/16.0f, 16/16.0f, 32/16.0f, 16/16.0f, 8/16.0f, 4/16.0f, 2/16.0f,
										2/16.0f, 2/16.0f, 4/16.0f, 8/16.0f, 16/16.0f, 8/16.0f, 4/16.0f, 2/16.0f, 2/16.0f,
										1/16.0f, 2/16.0f, 2/16.0f, 4/16.0f, 8/16.0f, 4/16.0f, 2/16.0f, 2/16.0f, 1/16.0f,
										1/16.0f, 1/16.0f, 2/16.0f, 2/16.0f, 4/16.0f, 2/16.0f, 2/16.0f, 1/16.0f, 1/16.0f,
										1/16.0f, 1/16.0f, 1/16.0f, 2/16.0f, 2/16.0f, 2/16.0f, 1/16.0f, 1/16.0f, 1/16.0f };

	FLOAT fValue = 0.0f;

	switch( nEffectValue )
	{
	case 3: fValue = GAUSSIAN9X9[nX][nY]; break;
	case 2: fValue = GAUSSIAN7X7[nX][nY]; break;
	case 1: fValue = GAUSSIAN5X5[nX][nY]; break;
	case 0: fValue = GAUSSIAN3X3[nX][nY]; break;		
	}

	return (BYTE)( byAlpha * fValue ); 	
}

VOID CFontManager::SetBlendedPixel( WORD* pDest, BYTE byAlpha, DWORD dwColor )
{
	SetBlendedPixel( pDest, byAlpha, GetRValue( dwColor ), GetGValue( dwColor ), GetBValue( dwColor ) );
}

VOID CFontManager::SetBlendedPixel( WORD* pDest, BYTE byAlpha, BYTE byRed, BYTE byGreen, BYTE byBlue )
{
	BYTE byDestAlpha = ( *pDest >> 12 ) << 4;
	BYTE byDestRed = ( *pDest >> 8  ) << 4;
	BYTE byDestGreen = ( *pDest >> 4 ) << 4;
	BYTE byDestBlue	= ( *pDest ) << 4;

	FLOAT fSrcAlpha;
	if( byAlpha )
		fSrcAlpha = byAlpha / 255.0f;//( 1.0f + byAlpha / 255.0f ) / 2;
	else
		fSrcAlpha = 0.0f;

	if( fSrcAlpha > 1.0f )
		fSrcAlpha = 1.0f;

	FLOAT fOneMinusSrcAlpha = 1.0f - fSrcAlpha;
	BYTE byResultAlpha = ( byDestAlpha > byAlpha ) ? byDestAlpha : byAlpha;

	BYTE byResultRed = (BYTE)( byRed * fSrcAlpha + byDestRed * fOneMinusSrcAlpha );
	BYTE byResultGreen = (BYTE)( byGreen * fSrcAlpha + byDestGreen * fOneMinusSrcAlpha );
	BYTE byResultBlue = (BYTE)( byBlue * fSrcAlpha + byDestBlue * fOneMinusSrcAlpha );

	*pDest = (WORD)( (byResultRed >> 4) << 8 | (byResultGreen >> 4) << 4 | byResultBlue >> 4 | ( byResultAlpha >> 4 ) << 12 );	
}

VOID CFontManager::SetPixel( WORD* pDest, BYTE byAlpha, DWORD dwColor )
{
	SetPixel( pDest, byAlpha, GetRValue( dwColor ), GetGValue( dwColor ), GetBValue( dwColor ) );
}

VOID CFontManager::SetPixel( WORD* pDest, BYTE byAlpha, BYTE byRed, BYTE byGreen, BYTE byBlue )
{
	*pDest = (WORD)( ( byRed >> 4 ) << 8 | ( byGreen >> 4 ) << 4 | byBlue >> 4 | ( byAlpha >> 4 ) << 12 );
}

BYTE CFontManager::GetRevisedAlpha( DWORD* pBitmap, INT nEffectMode )
{
	BYTE byAlpha = (BYTE)( *pBitmap );

	if( nEffectMode == TE_NONE )
	{
		if( byAlpha == 255 )
			return byAlpha;
		else 
			return ( byAlpha < GUI_FONT_ALPHA_FIX_VALUE ) ? 0 : byAlpha - GUI_FONT_ALPHA_FIX_VALUE;	
	}
	else
	{
		return byAlpha;
	}	
}

END_GUI