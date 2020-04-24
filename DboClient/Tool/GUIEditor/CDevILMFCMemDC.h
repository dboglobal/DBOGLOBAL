//-----------------------------------------------------------------------------
// Name: class CDevILMFCMemDC
// Desc: DevIL SDK를 이용하여 MFCMemDC에 띄운다.
//
//			 
// 2006.01.16 Peessi@hitel.net  
//-----------------------------------------------------------------------------

#ifndef __CDEVIL_MFC_MEMDC__
#define __CDEVIL_MFC_MEMDC__

#include "CDevIL.h"
#include "KMMFCMemDC.h"

class CDevILMFCMemDC : public CDevIL
{
	CKMMemDC	m_MemDCalpha;		// 알파값 저장
	CKMMemDC	m_MemDCimage;		// 픽셀값 저장

	BOOL		m_bShowAlpha;
	CString		m_strFilename;

public:

	CDevILMFCMemDC();
	CDevILMFCMemDC( CWnd* pWnd, CString& strFilename );
	~CDevILMFCMemDC();

	BOOL Load( CWnd* pWnd, CString& strFilename );
	void Unload();

	void DrawMemDC( CDC* pDC, INT sx, INT sy );
	void DrawMemDC( CDC* pDC, INT sx, INT sy, INT dx, INT dy, INT sw, INT sh );
	void DrawMemDC( CDC* pDC, INT sx, INT sy, INT dx, INT dy, INT sw, INT sh, INT dw, INT dh );

	// access

	BOOL GetShowAlpha() { return m_bShowAlpha; }
	void SetShowAlpha( BOOL b ) { m_bShowAlpha = b; }
	CString GetFilename() { return m_strFilename; }

protected:

	void ConvertToMemDC( CWnd* pWnd );
};

extern CDevILMFCMemDC g_DevILImage;

#endif//__CDEVIL_MFC_MEMDC__