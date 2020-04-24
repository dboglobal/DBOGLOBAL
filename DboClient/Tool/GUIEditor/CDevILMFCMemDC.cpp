//-----------------------------------------------------------------------------
// Name: class CDevILMFCMemDC
// Desc: DevIL SDK를 이용하여 MFCMemDC에 띄운다.
//
//			 
// 2006.01.16 Peessi@hitel.net  
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "CDevILMFCMemDC.h"

CDevILMFCMemDC g_DevILImage;

CDevILMFCMemDC::CDevILMFCMemDC()
{
	m_bShowAlpha = FALSE;
}

CDevILMFCMemDC::CDevILMFCMemDC( CWnd* pWnd, CString& strFilename )
{
	m_bShowAlpha = FALSE;
	Load( pWnd, strFilename );
}

BOOL CDevILMFCMemDC::Load( CWnd* pWnd, CString& strFilename )
{
	if( strFilename == m_strFilename )
		return TRUE;

	if( !CDevIL::Load( (LPCTSTR)strFilename ) )
	{
		Unload();
		return FALSE;
	}

	ConvertToMemDC( pWnd );
	m_strFilename = strFilename;

	return TRUE;
}

CDevILMFCMemDC::~CDevILMFCMemDC()
{
	Unload();
}

void CDevILMFCMemDC::Unload()
{
	m_MemDCalpha.Delete();
	m_MemDCimage.Delete();
		
	CDevIL::Unload();

	m_strFilename = "";
}

void CDevILMFCMemDC::ConvertToMemDC( CWnd* pWnd )
{
	m_MemDCimage.Delete();
	m_MemDCalpha.Delete();
	m_MemDCimage.Create( pWnd, m_nWidth, m_nHeight );
	m_MemDCalpha.Create( pWnd, m_nWidth, m_nHeight );

	unsigned char r,g,b,a;
	float		  t;

	for( int y = 0 ; y < m_nHeight ; y++ )
	{
		for( int x = 0 ; x < (m_nWidth*m_nBytes) ;  x+=(m_nBytes) )
		{	
			// Alpha값이 없는 파일인 경우 0xff( 255 )로 세팅되어 있다. 
			a = m_pPic[ (m_nWidth*m_nBytes) * y + x + 3 ];
			t = a / 255.0f;
		
			r = (unsigned char)( m_pPic[ (m_nWidth*m_nBytes) * y + x + 2 ] * t );
			g = (unsigned char)( m_pPic[ (m_nWidth*m_nBytes) * y + x + 1 ] * t );
			b = (unsigned char)( m_pPic[ (m_nWidth*m_nBytes) * y + x     ] * t );
			
			m_MemDCimage.GetMDC()->SetPixel( x / m_nBytes, y, RGB(r,g,b) );
			m_MemDCalpha.GetMDC()->SetPixel( x / m_nBytes, y, RGB(a,a,a) );
		}
	}
}

void CDevILMFCMemDC::DrawMemDC( CDC* pDC, INT sx, INT sy )
{
	if( !IsLoaded() )	
		return;

	if( m_bShowAlpha )
		m_MemDCalpha.BltTo( 0, 0, pDC, sx, sy );
	else
		m_MemDCimage.BltTo( 0, 0, pDC, sx, sy );

}

void CDevILMFCMemDC::DrawMemDC( CDC* pDC, INT sx, INT sy, INT dx, INT dy, INT sw, INT sh )
{
	if( !IsLoaded() )	
		return;

	if( m_bShowAlpha )
		m_MemDCalpha.StretchBltTo( sx, sy, sw, sh, pDC, dx, dy, sw, sh );
	else
		m_MemDCimage.StretchBltTo( sx, sy, sw, sh, pDC, dx, dy, sw, sh );

}

void CDevILMFCMemDC::DrawMemDC( CDC* pDC, INT sx, INT sy, INT dx, INT dy, INT sw, INT sh, INT dw, INT dh )
{
	if( !IsLoaded() )	
		return;

	if( m_bShowAlpha )
		m_MemDCalpha.StretchBltTo( sx, sy, sw, sh, pDC, dx, dy, dw, dh );
	else
		m_MemDCimage.StretchBltTo( sx, sy, sw, sh, pDC, dx, dy, dw, dh );

}

