#include "StdAfx.h"
#include "GDIPlusDBuffer.h"

// 생성자 : 반드시 윈도우 핸들을 인자로 설정해야 한다
CGDIPlusDBuffer::CGDIPlusDBuffer(HWND hWnd)
{
    // 멤버 변수 초기화
    m_hWnd = hWnd;
    m_pGraphics = NULL;
    m_pMemBmp = NULL;	

    CRect rcClient;
    ::GetClientRect(hWnd, &rcClient);

    // 메모리 비트맵을 생성한다
    m_pMemBmp = new Bitmap(rcClient.Width(), rcClient.Height());
    // 메모리 비트맵에 그래픽을 출력할 수 있는 Graphics 객체를 생성한다
    m_pGraphics = Graphics::FromImage(m_pMemBmp);
}



// 소멸자 : 버퍼에 출력된 그래픽을 화면에 출력한다
CGDIPlusDBuffer::~CGDIPlusDBuffer()
{

    // 만약 객체들이 생성되어 있다면
    if(m_pGraphics && m_pMemBmp)
    {
        // 윈도우의 DC를 얻는다
        HDC hDC = ::GetDC(m_hWnd);
        // 윈도우의 화면 Graphics 객체를 생성한다
        Graphics graphics(hDC);

        // 화면 Graphics 객체를 사용하여 메모리 비트맵의 내용을 복사하여 캐쉬 비트맵을 생성한다
        CachedBitmap *pCachedBmp =  new CachedBitmap(m_pMemBmp, &graphics);

        // 캐쉬 비트맵을 화면에 출력한다
        graphics.DrawCachedBitmap(pCachedBmp, 0, 0);

        // 캐쉬 비트맵을 제거한다
        delete pCachedBmp;
        // DC를 반환한다
        ::ReleaseDC(m_hWnd, hDC);
    }

    // 객체들을 제거한다
    if(m_pGraphics)		delete m_pGraphics;
    if(m_pMemBmp)		delete m_pMemBmp;	

}
