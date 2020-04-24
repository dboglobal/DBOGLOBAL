// ColorButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ColorButton.h"
#include ".\colorbutton.h"


// CColorButton

IMPLEMENT_DYNAMIC(CColorButton, CButton)
CColorButton::CColorButton()
{
    m_Color = RGB(255, 255, 255);
}

CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
END_MESSAGE_MAP()



// CColorButton 메시지 처리기입니다.

void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  지정된 항목을 그리는 코드를 추가합니다.
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	DWORD r, g, b;

	r = (m_Color >> 16) & 0xff;
	g = (m_Color >>  8) & 0xff;
	b = m_Color & 0xff;

	pDC->FillSolidRect(&(lpDrawItemStruct->rcItem), m_Color);
}


void CColorButton::SetColor(int r, int g, int b)
{
    m_Color = RGB(r, g, b);
}
