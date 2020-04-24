#pragma once

#include <GdiPlus.h>
using namespace Gdiplus;

// GDI+용 더블 버퍼링 클래스
class CGDIPlusDBuffer
{
    HWND			m_hWnd;				// 더블 버퍼링을 실행할 윈도우 핸들
    Graphics*		m_pGraphics;		// 더블 버퍼링용 Graphics 포인터
    Bitmap*			m_pMemBmp;			// 메모리 비트맵 포인터

public:
    // 생성자 : 반드시 윈도우 핸들을 인자로 설정해야 한다
    CGDIPlusDBuffer(HWND hWnd);


    // 소멸자 : 버퍼에 출력된 그래픽을 화면에 출력한다
    virtual ~CGDIPlusDBuffer();	


    // 더블 버퍼링용 Graphics 포인터를 리턴하는 함수
    Graphics* GetGraphics()		{ return m_pGraphics; }
};