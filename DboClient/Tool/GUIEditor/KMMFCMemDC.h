/******************************************************************************

  MemoryDC Class
  ==============
  - MFC/GDI 기반의 Memory DC 다루기 클래스

  - 비어있는 / Bitmap in Resource / 외부 Bitmap 파일 
    HBITMAP 형태 지원

  - 특정 함수 때문에 msimg32.lib 를 별도로 포함시켜줘야 함.
    --> #pragma 이용해서 자동으로 로딩하도록 수정.

  - 특정 함수 때문에 Win9x 에서는 문제 생길 수 있음.
    테스트는 Win2k 에서만 해봤음.

  - MFC의 특성상 MFCMemDC.cpp 파일의 include를 조정할 필요가 있을 것임.


  * Update

    2003.07.15 / klayman@gs
	- 최초작업

    2004.08.17 / klayman@gs
	- 기능보완 --> 이제부턴 함수마다 설명 똑바로 붙일 것!

    2005.06.02 / thkim@mcres.co.kr
	- 클래스 이름 수정. (너무 길어서리...)
	- 선택영역 관련함수 추가.

    2005.06.10 / thkim@mcres.co.kr
	- Thumbnail 지원함수 추가. / HBITMAP이나 MemDC 버전도 필요할 듯.


  * To Do
    - 복사생성자, 각종 연산자 처리
	- 에러처리
			   
******************************************************************************/

// MFCMemDC.h: interface for the CKMMemDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCMEMDC_H__9618ED30_73ED_4971_A1C0_66666A54F315__INCLUDED_)
#define AFX_MFCMEMDC_H__9618ED30_73ED_4971_A1C0_66666A54F315__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxwin.h>


#pragma comment(lib, "msimg32.lib")



class CKMMemDC  
{
protected:

	CDC*	 m_pMDC;

	int		 m_nWidth;
	int		 m_nHeight;
	
public:
	COLORREF m_crColorkey;
	
public:
	CKMMemDC();

	CKMMemDC( CWnd* pSrcWnd, int width, int height, COLORREF clr = 0x00ffffff );
	CKMMemDC( CWnd* pSrcWnd, UINT nIDBitmap, BITMAP* pBMInfo = NULL );
	CKMMemDC( CWnd* pSrcWnd, const char* szFileName, BITMAP* pBMInfo = NULL );
	CKMMemDC( CWnd* pSrcWnd, HBITMAP hBitmap, BITMAP* pBMInfo = NULL );

	virtual ~CKMMemDC();

public:
	void InitMember();
	void Delete();

	// Create Memory DC - Empty/Solid
	int Create        ( CWnd* pSrcWnd, 
						int width, int height, COLORREF clr = 0x00ffffff );

	// Create Memory DC - From Resource ID
	int CreateFromID  ( CWnd* pSrcWnd, UINT nIDBitmap, 
						BITMAP* pBMInfo = NULL );

	// Create Memory DC - From File Name
	int CreateFromFile( CWnd* pSrcWnd, const char* szFileName, 
						BITMAP* pBMInfo = NULL );

	// Create Memory DC - From HBITMAP
	int CreateFromHBM ( CWnd* pSrcWnd, HBITMAP hBitmap, 
						BITMAP* pBMInfo = NULL );

public: //// 화면 찍기 함수

	// 1:1 찍기
	int  BltTo			( CDC* pDestDC, 
						  int dx, int dy,
						  DWORD dwROP = SRCCOPY );

	int  BltTo			( int sx, int sy,
						  CDC* pDestDC, 
						  int dx, int dy,
						  DWORD dwROP = SRCCOPY );

	int  BltTo			( int sx, int sy, int sw, int sh,
						  CDC* pDestDC, 
						  int dx, int dy,
						  DWORD dwROP = SRCCOPY );
	
	// 늘려서/줄여서 찍기
	int  StretchBltTo	( CDC* pDestDC, 
						  int dx, int dy, int dw, int dh,
						  DWORD dwROP = SRCCOPY );

	int  StretchBltTo	( int sx, int sy,
						  CDC* pDestDC, 
						  int dx, int dy, int dw, int dh,
						  DWORD dwROP = SRCCOPY );

	int  StretchBltTo	( int sx, int sy, int sw, int sh,
						  CDC* pDestDC, 
						  int dx, int dy, int dw, int dh,
						  DWORD dwROP = SRCCOPY );


	// 투명값 적용 / 설정된 Colokey : 추가 library 필요함 ( msimg32.lib )
	int  TransBltTo		( CDC* pDestDC, 
						  int dx, int dy );

	int  TransBltTo		( int sx, int sy,
						  CDC* pDestDC, 
						  int dx, int dy );

	int  TransBltTo		( int sx, int sy, int sw, int sh,
						  CDC* pDestDC, 
						  int dx, int dy, int dw, int dh );

	// 투명값 적용 / Colokey 지정 : 추가 library 필요함 ( msimg32.lib )
	int  TransBltTo		( CDC* pDestDC, 
						  int dx, int dy,
						  COLORREF clrKey );

	int  TransBltTo		( int sx, int sy,
						  CDC* pDestDC, 
						  int dx, int dy,
						  COLORREF clrKey );

	int  TransBltTo		( int sx, int sy, int sw, int sh,
						  CDC* pDestDC, 
						  int dx, int dy, int dw, int dh,
						  COLORREF clrKey );


	// 반투명 적용
	int  AlphaBltTo		( CDC* pDestDC, 
						  int dx, int dy,
						  BYTE alpha = 128 ); // alpha = 0 ~ 255

	int  AlphaBltTo		( int sx, int sy,
						  CDC* pDestDC, 
						  int dx, int dy,
						  BYTE alpha = 128 ); // alpha = 0 ~ 255

	int  AlphaBltTo		( int sx, int sy, int sw, int sh,
						  CDC* pDestDC, 
						  int dx, int dy, int dw, int dh,
						  BYTE alpha = 128 ); // alpha = 0 ~ 255


	// 단순 색채우기
	int FillColor		( COLORREF clr = 0x00ffffff );
	int FillColor		( COLORREF clr, CRect rc );
	int FillColor		( COLORREF clr, int x, int y, int w, int h );


	// 격자 그리기.
	int DrawGrid		( int gridX, int gridY, COLORREF clr, int penWidth=1 );


public: ////

	// Colorkey 설정.
	void SetColorkey	( BYTE r, BYTE g, BYTE b );
	void SetColorkey	( COLORREF colorkey );
	void SetColorkey	( int pixelX, int pixelY );

	// Colorkey와 비교.
	BOOL IsColorkey		( BYTE r, BYTE g, BYTE b );
	BOOL IsColorkey		( COLORREF color );

	// Colorkey와 비교하여 최적화.
	BOOL SelectFix_Optimize	( CRect& rcSelect );

	// MemDC의 폭/높이와 비교하여 수정.
	BOOL SelectFix_Bound	( CRect& rcSelect );

	// Grid간격으로 수정.
	BOOL SelectFix_Grid		( int gridX, int gridY, CRect& rcSelect );

	// RECT 영역안에 위치하고 있는가? / Clipping 검사(?)
	BOOL IsInRect ( CPoint pt, CRect* pRect );


public: ////

	BOOL GetThumbNail	( CWnd* pSrcWnd, CBitmap *pBMOut, 
							int sX, int sY, int sW, int sH, 
							int thW, int thH );

	BOOL GetThumbNail	( CWnd* pSrcWnd, CBitmap *pBMOut, 
							int thW, int thH );


public:
	// 단순 값 돌려주기 함수들 모음
	inline int  GetWidth()	{ return m_nWidth; }
	inline int  GetHeight()	{ return m_nHeight; }

	inline CDC* GetMDC()	{ return m_pMDC; }
	inline HDC	GetHDC()	{ return m_pMDC->m_hDC; }

	inline operator CDC*()	{ return m_pMDC; }
	inline operator HDC()	{ return m_pMDC->m_hDC; }

	//
	int	IsEmpty()			{ return ( (m_pMDC==NULL)?1:0 ); }

};// end of class CKMMemDC

#endif // !defined(AFX_MFCMEMDC_H__9618ED30_73ED_4971_A1C0_66666A54F315__INCLUDED_)
