/////////////////////////////////////////////////////////////////////////////
//
// MacButtons.cpp : implementation file
//
// Feel free to modifiy and/or distribute this file, but
// do not remove this header.
//
// I would appreciate a notification of any bugs discovered or 
// improvements that could be made.
//
// This file is provided "as is" with no expressed or implied warranty.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MacButtons.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
//
//	CMacButton class, version 2.0
//
//	Copyright (c) 1999 Paul Meidinger (pmmeidinger@yahoo.com)
//
//	History:
//		PMM	12/13/1999		Initial implementation.		
//
//		PMM	12/29/1999		Minor changes made.
//
/////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------
//
CMacButton::CMacButton()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Standard constructor.
//
{
	m_nType = TYPE_STANDARD;

	m_nCheck = 0;
	m_bMouseDown = FALSE;

	m_bBold = FALSE;

	m_hIcon = NULL;
	m_hBitmap = NULL;
	m_sizeImage = CSize(0, 0);

	m_nImageEffect = 0;

	GetColors();
	CreatePens();
}	// CMacButton

//-------------------------------------------------------------------
//
CMacButton::~CMacButton()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Destructor.
//
{
	DeletePens();
}	// ~CMacButton


BEGIN_MESSAGE_MAP(CMacButton, CButton)
	//{{AFX_MSG_MAP(CMacButton)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacButton message handlers

//-------------------------------------------------------------------
//
void CMacButton::PreSubclassWindow() 
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	This member function is called by the framework to 
//						allow other necessary subclassing to occur before the 
//						window is subclassed. Adds the BS_OWNERDRAW style.
//
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}	// PreSubclassWindow

//-------------------------------------------------------------------
//
void CMacButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
//
// Return Value:	None.
//
// Parameters	:	lpDIS - A long pointer to a DRAWITEMSTRUCT structure.
//
// Remarks		:	Called by the framework when a visual aspect of an 
//						owner-drawn button has changed.
//
{
	DrawButton(lpDIS);
}	// DrawItem

//-------------------------------------------------------------------
//
void CMacButton::DrawButton(LPDRAWITEMSTRUCT lpDIS)
//
// Return Value:	None.
//
// Parameters	:	lpDIS - A long pointer to a DRAWITEMSTRUCT structure.
//
// Remarks		:	Draws a Mac button.
//
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rectItem(lpDIS->rcItem);

	UINT nState = lpDIS->itemState;
	UINT nStyle = GetStyle();
	BOOL bPushLike = BOOL(nStyle & BS_PUSHLIKE);

	// Create a mem DC for drawing
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rectItem.Width(), rectItem.Height());
	CBitmap *pOldBmp = dcMem.SelectObject(&bmp);

	int nSaveDC = dcMem.SaveDC();

	CBrush *pOldBrush = (CBrush *)dcMem.SelectStockObject(NULL_BRUSH);
	dcMem.FillSolidRect(rectItem, m_crFace);

	// If the button is standard or pushlike, draw it now.
	// Wait until after drawing the text/image and focus to draw
	//	the radio button or check box. This way, the text or image
	// will not cover the radio or check.
	if (m_nType == TYPE_STANDARD)
		DrawStandardButton(&dcMem, rectItem, nStyle, nState);
	else if (bPushLike)
		DrawPushLikeButton(&dcMem, rectItem, nStyle, nState);

	CRect rectText(rectItem);
	CRect rectImage(rectItem);
	CString sText;
	GetWindowText(sText);

	// Draw an image or the text.
	if (m_hIcon || m_hBitmap)
		DrawImage(&dcMem, rectImage, nStyle, nState);
	else
		DrawButtonText(&dcMem, rectText, sText, nStyle, nState);

	// Draw the focus rect.
	if (nState & ODS_FOCUS)
	{
		if ((m_nType == TYPE_STANDARD) || bPushLike)
		{
			rectText = lpDIS->rcItem;
			rectText.DeflateRect(4, 4, 4, 4);
			dcMem.DrawFocusRect(rectText);
		}
		else if (m_hIcon || m_hBitmap)
		{
			rectImage.InflateRect(1, 1, 1, 1);
			if (rectImage.top < rectItem.top)
				rectImage.top = rectItem.top;
			if (rectImage.bottom > rectItem.bottom)
				rectImage.bottom = rectItem.bottom;
			if (rectImage.left < rectItem.left)
				rectImage.left = rectItem.left;
			if (rectImage.right > rectItem.right)
				rectImage.right = rectItem.right;

			dcMem.DrawFocusRect(rectImage);
		}
		else
		{
			rectText.InflateRect(1, 1, 1, 2);
			if (rectText.top < rectItem.top)
				rectText.top = rectItem.top;
			if (rectText.bottom > rectItem.bottom)
				rectText.bottom = rectItem.bottom;
			if (rectText.left < rectItem.left)
				rectText.left = rectItem.left;
			if (rectText.right > rectItem.right)
				rectText.right = rectItem.right;

			dcMem.DrawFocusRect(rectText); 
		}
	}	// if the button has focus

	// Draw the check box or radio button now.
	if (!bPushLike)
	{
		// Determine the rect for the check mark.
		CRect rectCheck = GetCheckRect(rectItem, nStyle);

		if (m_nType == TYPE_CHECKBOX)
			DrawCheckBox(&dcMem, rectCheck, nStyle, nState);
		else if (m_nType == TYPE_RADIO)
			DrawRadioButton(&dcMem, rectCheck, nStyle, nState);
	}

	pDC->BitBlt(rectItem.left, rectItem.top, rectItem.Width(), rectItem.Height(), &dcMem, rectItem.left, rectItem.top, SRCCOPY);

	// Clean up.
	dcMem.SelectObject(pOldBrush);
	dcMem.SelectObject(pOldBmp);
	dcMem.RestoreDC(nSaveDC);
	dcMem.DeleteDC();
	bmp.DeleteObject();
}	// DrawButton

//-------------------------------------------------------------------
//
void CMacButton::DrawStandardButton(CDC *pDC, const CRect &rect, UINT nStyle, UINT nState)
//
// Return Value:	None.
//
// Parameters	:	pDC - A pointer to the DC to draw on.
//						rect - The button rectangle.
//						nStyle - The button's style.
//						nState - The button's state.
//
// Remarks		:	Draws the button.
//
{
	// Draw a flat button.
	if (nStyle & BS_FLAT)
	{
		COLORREF crFill = ::GetSysColor(COLOR_WINDOW);
		CBrush brFill(nState & ODS_SELECTED ? ~crFill : crFill);
		CBrush *pOldBrush = (CBrush *)pDC->SelectObject(&brFill);
		CPen pen;
		if (nState & ODS_DISABLED)
			pen.CreatePen(PS_SOLID, 1, m_crShadow);
		else if (m_nCheck || m_bMouseDown)
			pen.CreatePen(PS_SOLID, 1, crFill);
		else
			pen.CreatePen(PS_SOLID, 1, ~crFill);
		CPen *pOldPen = (CPen *)pDC->SelectObject(&pen);
		pDC->RoundRect(rect, CPoint(6, 6));
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
		return;
	}	// if flat
	
	CBrush brFill(nState & ODS_SELECTED ? m_crLiteShadow : m_crFace);
	CBrush *pOldBrush	= (CBrush *)pDC->SelectObject(&brFill);
	CPen penFrame(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOWFRAME));
	CPen *pOldPen = (CPen *)pDC->SelectObject(nState & ODS_DISABLED ? &m_penShadow : &penFrame);

	// Draw a 3D button.
	if (nState & ODS_DISABLED)
		pDC->RoundRect(rect, CPoint(6, 6));
	else if (nState & ODS_SELECTED)
		DrawPressedPushButton(pDC, rect);
	else
		DrawUnpressedPushButton(pDC, rect);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}	// DrawStandardButton

//-------------------------------------------------------------------
//
void CMacButton::DrawCheckBox(CDC *pDC, CRect rect, UINT nStyle, UINT nState)
//
// Return Value:	None.
//
// Parameters	:	pDC - A pointer to the DC to draw on.
//						rect - The check mark's rectangle.
//						nStyle - The button's style.
//						nState - The button's state.
//
// Remarks		:	Draws the check mark.
//
{
	BOOL bDisabled = nState & ODS_DISABLED;
	
	// Deflate the rect by two on the right (for the check mark's shadow).
	rect.DeflateRect(0, 0, 2, 0);

	CPen *pOldPen = pDC->GetCurrentPen();
	CBrush *pOldBrush = pDC->GetCurrentBrush();

	// Draw a flat checkbox.
	if (nStyle & BS_FLAT)
	{
		COLORREF crFrame = ::GetSysColor(COLOR_WINDOW);
		CPen penFrame(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOWFRAME));
		CBrush brFill(crFrame);
		pDC->SelectObject(&penFrame);
		pDC->SelectObject(&brFill);

		pDC->Rectangle(rect);

		if (bDisabled)
		{
			for (int i = rect.left + 1, j = rect.top + 1; i < rect.right; i += 2, j += 2)
			{
				pDC->SetPixel(i, rect.top, crFrame);
				pDC->SetPixel(i - 1, rect.bottom - 1, crFrame);
				pDC->SetPixel(rect.left, j, crFrame);
				pDC->SetPixel(rect.right - 1, j - 1, crFrame);
			}
		}

		rect.DeflateRect(1, 1, 1, 1);

		if (m_bMouseDown)
			pDC->Rectangle(rect);

		// Inflate the rect by two on the right (for the check mark's shadow).
		rect.InflateRect(0, 0, 2, 0);

		int nLeft = rect.left;
		int nTop = rect.top;

		CPen penDkShadow(PS_SOLID, 1, m_crDkShadow);
		pDC->SelectObject(&penDkShadow);

		// Draw the check, cross, or tri-state mark.
		if (m_nCheck == 1)
		{
			if (m_nCheckStyle == CHECK_STYLE_CHECK)
			{
				if (!bDisabled)
				{
					pDC->MoveTo(nLeft + 1, nTop + 4);
					pDC->LineTo(nLeft + 4, nTop + 7);
					pDC->LineTo(nLeft + 12,nTop - 1);
				}
				pDC->MoveTo(nLeft + 2, nTop + 4);
				pDC->LineTo(nLeft + 4, nTop + 6);
				pDC->LineTo(nLeft + 11, nTop -1);
			}
			else if (m_nCheckStyle == CHECK_STYLE_CROSS)
			{
				pDC->MoveTo(nLeft, nTop);
				pDC->LineTo(nLeft + 10, nTop + 10);

				int nAdjust = bDisabled ? 1 : 0;
				pDC->MoveTo(nLeft + nAdjust, nTop + 9 - nAdjust);
				pDC->LineTo(nLeft + 10 - nAdjust, nTop - 1 + nAdjust);
			}
		}
		else if (m_nCheck == 2)
		{
			pDC->SelectObject(&m_penDkShadow);
			pDC->MoveTo(nLeft + 2, nTop + 4);
			pDC->LineTo(nLeft + 8, nTop + 4);
			pDC->MoveTo(nLeft + 2, nTop + 5);
			pDC->LineTo(nLeft + 8, nTop + 5);
		}

	}	// if flat
	// Else draw a 3D checkbox.
	else
	{
		CBrush brFrame(bDisabled ? m_crShadow : ::GetSysColor(COLOR_WINDOWFRAME));
		pDC->FrameRect(rect, &brFrame);
		rect.DeflateRect(1, 1);

		if (m_bMouseDown)
		{								
			CBrush brShadow(m_crLiteShadow);
			pDC->FillRect(rect, &brShadow);
			pDC->Draw3dRect(rect, m_crShadow, m_crLiteFace);
		}
		else
		{
			pDC->FillSolidRect(rect, m_crFace);

			if (!bDisabled)
				pDC->Draw3dRect(rect, m_crHilight, m_crShadow);
		}

		pDC->SetPixel(CPoint(rect.right - 1, rect.top), m_crFace);
		pDC->SetPixel(CPoint(rect.left, rect.bottom - 1), m_crFace);

		// Inflate the rect by two on the right (for the check mark's shadow).
		rect.InflateRect(0, 0, 2, 0);

		int nLeft = rect.left;
		int nTop = rect.top;

		// Draw the check, cross, or tri-state mark.
		if (m_nCheck == 1)
		{
			// Draw the check mark's shadow
			if (m_nCheckStyle == CHECK_STYLE_CHECK)
			{
				// Draw the check mark's shadow
				pDC->SelectObject(bDisabled ? &m_penLiteShadow : &m_penShadow);
				pDC->MoveTo(nLeft + 4, nTop + 8);
				pDC->LineTo(nLeft + 9, nTop + 3);
				pDC->LineTo(nLeft + 9, nTop + 5);

				pDC->SelectObject(&m_penLiteShadow);
				pDC->MoveTo(nLeft + 2, nTop + 6);
				pDC->LineTo(nLeft + 4, nTop + 8);
				pDC->MoveTo(nLeft + 5, nTop + 8);
				pDC->LineTo(nLeft + 9, nTop + 4);

				pDC->SetPixel(nLeft + 11, nTop + 1, bDisabled ? m_crLiteShadow : m_crShadow);
				pDC->SetPixel(nLeft + 12, nTop + 1, m_crLiteShadow);
				pDC->SetPixel(nLeft + 11, nTop + 2, m_crLiteShadow);

				// Draw the check mark.
				pDC->SelectObject(bDisabled ? &m_penShadow : &m_penDkShadow);
				pDC->MoveTo(nLeft + 1, nTop + 4);
				pDC->LineTo(nLeft + 4, nTop + 7);
				pDC->LineTo(nLeft + 12,nTop - 1);
				pDC->MoveTo(nLeft + 2, nTop + 4);
				pDC->LineTo(nLeft + 4, nTop + 6);
				pDC->LineTo(nLeft + 11, nTop -1);
			}
			else if (m_nCheckStyle == CHECK_STYLE_CROSS)
			{
				// Draw the check mark's shadow
				pDC->SelectObject(bDisabled ? &m_penLiteShadow : &m_penShadow);
				pDC->MoveTo(nLeft + 3, nTop + 7);
				pDC->LineTo(nLeft + 9, nTop + 1);
				pDC->SelectObject(&m_penLiteShadow);
				pDC->MoveTo(nLeft + 3, nTop + 8);
				pDC->LineTo(nLeft + 9, nTop + 2);
				pDC->SetPixel(nLeft + 8, nTop + 7, bDisabled ? m_crLiteShadow : m_crShadow);
				pDC->SetPixel(nLeft + 8, nTop + 8, m_crLiteShadow);
				
				// Draw the check mark.
				pDC->SelectObject(bDisabled ? &m_penShadow : &m_penDkShadow);
				pDC->MoveTo(nLeft + 2, nTop + 1);
				pDC->LineTo(nLeft + 8, nTop + 7);
				pDC->MoveTo(nLeft + 2, nTop + 2);
				pDC->LineTo(nLeft + 8, nTop + 8);
				
				pDC->MoveTo(nLeft + 2, nTop + 6);
				pDC->LineTo(nLeft + 8, nTop);
				pDC->MoveTo(nLeft + 2, nTop + 7);
				pDC->LineTo(nLeft + 8, nTop + 1);
			}
		}	// if checked
		else if (m_nCheck == 2)
		{
			pDC->SelectObject(&m_penLiteShadow);
			pDC->MoveTo(nLeft + 3, nTop + 6);
			pDC->LineTo(nLeft + 9, nTop + 6);
			pDC->SetPixel(nLeft + 8, nTop + 5, m_crLiteShadow);

			pDC->SelectObject(bDisabled ? &m_penShadow : &m_penDkShadow);
			pDC->MoveTo(nLeft + 2, nTop + 4);
			pDC->LineTo(nLeft + 8, nTop + 4);
			pDC->MoveTo(nLeft + 2, nTop + 5);
			pDC->LineTo(nLeft + 8, nTop + 5);
		}
	}	// else draw 3D checkbox

	// Restore the pen and brush.
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}	// DrawCheckBox

//-------------------------------------------------------------------
//
void CMacButton::DrawRadioButton(CDC *pDC, CRect rect, UINT nStyle, UINT nState)
//
// Return Value:	None.
//
// Parameters	:	pDC - A pointer to the DC to draw on.
//						rect - The radio's rectangle.
//						nStyle - The button's style.
//						nState - The button's state.
//
// Remarks		:	Draws the radio button.
//
{
	// Give the user a visual indication that the button is pressed by darkening some colors.
	BOOL bDarkened = FALSE;
	if (m_bMouseDown && !(nStyle & BS_FLAT))
	{
		m_crFace = DarkenColor(m_crFace, 40);
		m_crShadow = DarkenColor(m_crShadow, 40);
		m_crHilight = DarkenColor(m_crHilight, 40);
		m_crLiteShadow = DarkenColor(m_crLiteShadow, 40);
		m_crLiteFace = DarkenColor(m_crLiteFace, 40);
		CreatePens();
		bDarkened = TRUE;
	}

	BOOL bDisabled = nState & ODS_DISABLED;

	// Draw the outer round rect.
	CPen penFrame;
	CBrush br;
	if (nStyle & BS_FLAT)
	{
		penFrame.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOWFRAME));
		br.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
	}
	else
	{
		penFrame.CreatePen(PS_SOLID, 1, bDisabled ? m_crShadow : ::GetSysColor(COLOR_WINDOWFRAME));
		br.CreateSolidBrush(m_crFace);
	}

	CPen *pOldPen = (CPen *)pDC->SelectObject(&penFrame);
	CBrush *pOldBrush = (CBrush *)pDC->SelectObject(&br);
	pDC->RoundRect(rect, CPoint(10, 10));
	if ((nStyle & BS_FLAT) && m_bMouseDown)
	{
		rect.DeflateRect(1, 1, 1, 1);
		pDC->RoundRect(rect, CPoint(8, 8));
		rect.InflateRect(1, 1, 1, 1);
	}
	pDC->SelectObject(pOldBrush);
	br.DeleteObject();
	penFrame.DeleteObject();

	// Draw some shadowing
	if (!(nStyle & BS_FLAT))
	{
		int nLeft = rect.left;
		int nTop = rect.top;
		int nAdjust = (m_nCheck || m_bMouseDown ? -11 : 0);
		int nSign = (m_nCheck || m_bMouseDown ? -1 : 1);

		if (!bDisabled)
		{
			pDC->SelectObject(&m_penDarkDkShadow);
			pDC->MoveTo(nLeft + 0, nTop + 4);
			pDC->LineTo(nLeft + 5, nTop - 1);
			pDC->MoveTo(nLeft + 7, nTop);
			pDC->LineTo(nLeft + 12, nTop + 5);
			pDC->MoveTo(nLeft + 11, nTop + 7);
			pDC->LineTo(nLeft + 6, nTop + 12);
			pDC->MoveTo(nLeft + 4, nTop + 11);
			pDC->LineTo(nLeft - 1, nTop + 6);
		}

		if (!bDisabled)
		{
			pDC->SelectObject(&m_penLiteFace);
			pDC->MoveTo(nLeft + (nSign * (nAdjust + 1)), nTop + (nSign * (nAdjust + 6)));
			pDC->LineTo(nLeft + (nSign * (nAdjust + 1)), nTop + (nSign * (nAdjust + 4)));
			pDC->LineTo(nLeft + (nSign * (nAdjust + 4)), nTop + (nSign * (nAdjust + 1)));
			pDC->LineTo(nLeft + (nSign * (nAdjust + 7)), nTop + (nSign * (nAdjust + 1)));
			pDC->MoveTo(nLeft + (nSign * (nAdjust + 2)), nTop + (nSign * (nAdjust + 8)));
  			pDC->LineTo(nLeft + (nSign * (nAdjust + 9)), nTop + (nSign * (nAdjust + 1)));
			pDC->MoveTo(nLeft + (nSign * (nAdjust + 3)), nTop + (nSign * (nAdjust + 8)));
  			pDC->LineTo(nLeft + (nSign * (nAdjust + 9)), nTop + (nSign * (nAdjust + 2)));
			pDC->MoveTo(nLeft + (nSign * (nAdjust + 4)), nTop + (nSign * (nAdjust + 8)));
  			pDC->LineTo(nLeft + (nSign * (nAdjust + 9)), nTop + (nSign * (nAdjust + 3)));

			pDC->SelectObject(&m_penHilight);
			pDC->MoveTo(nLeft + (nSign * (nAdjust + 2)), nTop + (nSign * (nAdjust + 7)));
			pDC->LineTo(nLeft + (nSign * (nAdjust + 2)), nTop + (nSign * (nAdjust + 4)));
			pDC->LineTo(nLeft + (nSign * (nAdjust + 6)), nTop + (nSign * (nAdjust + 4)));
			pDC->MoveTo(nLeft + (nSign * (nAdjust + 4)), nTop + (nSign * (nAdjust + 5)));
			pDC->LineTo(nLeft + (nSign * (nAdjust + 4)), nTop + (nSign * (nAdjust + 2)));
			pDC->LineTo(nLeft + (nSign * (nAdjust + 8)), nTop + (nSign * (nAdjust + 2)));

			pDC->SelectObject(&m_penLiteShadow);
			pDC->MoveTo(nLeft + (nSign * (nAdjust + 6)), nTop + (nSign * (nAdjust + 9))); 
			pDC->LineTo(nLeft + (nSign * (nAdjust + 10)), nTop + (nSign * (nAdjust + 5))); 
			pDC->MoveTo(nLeft + (nSign * (nAdjust + 7)), nTop + (nSign * (nAdjust + 9))); 
			pDC->LineTo(nLeft + (nSign * (nAdjust + 10)), nTop + (nSign * (nAdjust + 6))); 

			pDC->SelectObject(&m_penShadow);
			pDC->MoveTo(nLeft + (nSign * (nAdjust + 4)), nTop + (nSign * (nAdjust + 10)));
			pDC->LineTo(nLeft + (nSign * (nAdjust + 7)), nTop + (nSign * (nAdjust + 10))); 
			pDC->LineTo(nLeft + (nSign * (nAdjust + 10)), nTop + (nSign * (nAdjust + 7))); 
			pDC->LineTo(nLeft + (nSign * (nAdjust + 10)), nTop + (nSign * (nAdjust + 3))); 

			pDC->SetPixel(nLeft + (nSign * (nAdjust + 3)), nTop + (nSign * (nAdjust + 5)), m_crHilight);
			pDC->SetPixel(nLeft + (nSign * (nAdjust + 5)), nTop + (nSign * (nAdjust + 3)), m_crHilight);
			pDC->SetPixel(nLeft + (nSign * (nAdjust + 2)), nTop + (nSign * (nAdjust + 2)), m_crFace);
			pDC->SetPixel(nLeft + (nSign * (nAdjust + 3)), nTop + (nSign * (nAdjust + 3)), m_crLiteFace);
			pDC->SetPixel(nLeft + (nSign * (nAdjust + 3)), nTop + (nSign * (nAdjust + 6)), m_crLiteFace);
			pDC->SetPixel(nLeft + (nSign * (nAdjust + 6)), nTop + (nSign * (nAdjust + 3)), m_crLiteFace);
		}

		// Do a little "touch-up" top make the radios look better.
		COLORREF crTemp = bDisabled ? m_crLiteShadow : m_crShadow;
		pDC->SetPixel(nLeft, nTop + 3, crTemp);
		pDC->SetPixel(nLeft + 3, nTop, crTemp);
		pDC->SetPixel(nLeft + 8, nTop, crTemp);
		pDC->SetPixel(nLeft + 11, nTop + 3, crTemp);
		pDC->SetPixel(nLeft + 11, nTop + 8, crTemp);
		pDC->SetPixel(nLeft + 8, nTop + 11, crTemp);
		pDC->SetPixel(nLeft + 3, nTop + 11, crTemp);
		pDC->SetPixel(nLeft, nTop + 8, crTemp);
	}	// if not disabled and not flat

	// Draw the check if necessary.
	if (m_nCheck)
	{
		COLORREF crTemp = (bDisabled && !(nStyle & BS_FLAT) ? m_crShadow : m_crDkShadow);
		CBrush br(crTemp);
		CPen pen(PS_SOLID, 1, crTemp);
		CBrush *pOldBrush = pDC->SelectObject(&br);
		pDC->SelectObject(&pen);
		rect.DeflateRect(3, 3, 3, 3);
		pDC->RoundRect(rect, CPoint(2, 2));
		pDC->SelectObject(pOldBrush);
	}

	// Lighten the colors if they were darkened.
	if (bDarkened)
	{
		m_crFace = LightenColor(m_crFace, 40);
		m_crShadow = LightenColor(m_crShadow, 40);
		m_crHilight = LightenColor(m_crHilight, 40);
		m_crLiteShadow = LightenColor(m_crLiteShadow, 40);
		m_crLiteFace = LightenColor(m_crLiteFace, 40);
		CreatePens();
	}

	pDC->SelectObject(pOldPen);
}	// DrawRadioButton
	
//-------------------------------------------------------------------
//
void CMacButton::OnLButtonDblClk(UINT /*nFlags*/, CPoint point) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down.
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left
//							 corner of the window.
//
// Remarks		:	The framework calls this member function when the user 
//						double-clicks the left mouse button. The WM_LBUTTONDOWN
//						message is sent to simulate a single click.
//
{
	SendMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(point.x, point.y));	
}	// OnLButtonDblClk

//-------------------------------------------------------------------
//
void CMacButton::OnSysColorChange() 
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	The framework calls this member function for all 
//						top-level windows when a change is made in the 
//						system color setting
//
{
	CButton::OnSysColorChange();
	
	GetColors();
	CreatePens();
}	// OnSysColorChange

//-------------------------------------------------------------------
//
void CMacButton::GetColors()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Sets the COLORREF member variables.
//
{
	if (m_nType == TYPE_MORE)
	{
		m_crDkShadow = ::GetSysColor(COLOR_WINDOWFRAME);
		m_crShadow = ::GetSysColor(COLOR_3DSHADOW);
		m_crLiteFace = LightenColor(m_crFace, 51);
		m_crLiteShadow = DarkenColor(m_crFace, 51);
	}
	else
	{
		m_crHilight = ::GetSysColor(COLOR_3DHILIGHT);
		m_crLiteFace = ::GetSysColor(COLOR_3DLIGHT);
		m_crFace = ::GetSysColor(COLOR_3DFACE);
		m_crShadow = ::GetSysColor(COLOR_3DSHADOW);
		m_crDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);

		// Create some intermediate colors. This is done by averaging two colors.
		BYTE byRedFace = GetRValue(m_crFace);
		BYTE byGreenFace = GetGValue(m_crFace);
		BYTE byBlueFace = GetBValue(m_crFace);
		BYTE byRedShadow = GetRValue(m_crShadow);
		BYTE byGreenShadow = GetGValue(m_crShadow);
		BYTE byBlueShadow = GetBValue(m_crShadow);
		BYTE byRedDkShadow = GetRValue(m_crDkShadow);
		BYTE byGreenDkShadow = GetGValue(m_crDkShadow);
		BYTE byBlueDkShadow = GetBValue(m_crDkShadow);

		m_crLiteShadow = RGB(byRedFace + ((byRedShadow - byRedFace) >> 1),
									byGreenFace + ((byGreenShadow - byGreenFace) >> 1),
									byBlueFace + ((byBlueShadow - byBlueFace) >> 1));

		m_crDarkDkShadow = RGB(byRedShadow + ((byRedDkShadow - byRedShadow) >> 1),
									byGreenShadow + ((byGreenDkShadow - byGreenShadow) >> 1),
									byBlueShadow + ((byBlueDkShadow - byBlueShadow) >> 1));
	}
}	// GetColors

//-------------------------------------------------------------------
//
COLORREF CMacButton::LightenColor(const COLORREF crColor, BYTE byIncreaseVal)
//
// Return Value:	None.
//
// Parameters	:	crColor - References a COLORREF structure.
//						byReduceVal - The amount to reduce the RGB values by.
//
// Remarks		:	Lightens a color by increasing the RGB values by the given number.
//				 
{
	BYTE byRed = GetRValue(crColor);
	BYTE byGreen = GetGValue(crColor);
	BYTE byBlue = GetBValue(crColor);

	byRed = (byRed + byIncreaseVal) <= 255 ? BYTE(byRed + byIncreaseVal) : 255;;
	byGreen = (byGreen + byIncreaseVal)	<= 255 ? BYTE(byGreen + byIncreaseVal) : 255;
	byBlue = (byBlue + byIncreaseVal) <= 255 ? BYTE(byBlue + byIncreaseVal) : 255;

/*	if ((byRed + byIncreaseVal) <= 255)
		byRed = BYTE(byRed + byIncreaseVal);
	if ((byGreen + byIncreaseVal)	<= 255)
		byGreen = BYTE(byGreen + byIncreaseVal);
	if ((byBlue + byIncreaseVal) <= 255)
		byBlue = BYTE(byBlue + byIncreaseVal);
*/
	return RGB(byRed, byGreen, byBlue);
}	// LightenColor

//-------------------------------------------------------------------
//
COLORREF CMacButton::DarkenColor(const COLORREF crColor, BYTE byReduceVal)
//
// Return Value:	None.
//
// Parameters	:	crColor - References a COLORREF structure.
//						byReduceVal - The amount to reduce the RGB values by.
//
// Remarks		:	Darkens a color by reducing the RGB values by the given number.
//
{
	BYTE byRed = GetRValue(crColor);
	BYTE byGreen = GetGValue(crColor);
	BYTE byBlue = GetBValue(crColor);

	byRed = byRed >= byReduceVal ? BYTE(byRed - byReduceVal) : 0;
	byGreen = byGreen >= byReduceVal ? BYTE(byGreen - byReduceVal) : 0;
	byBlue = (byBlue >= byReduceVal) ? BYTE(byBlue - byReduceVal) : 0;

/*	if (byRed >= byReduceVal)
		byRed = BYTE(byRed - byReduceVal);
	if (byGreen >= byReduceVal)
		byGreen = BYTE(byGreen - byReduceVal);
	if (byBlue >= byReduceVal)
		byBlue = BYTE(byBlue - byReduceVal);
*/

	return RGB(byRed, byGreen, byBlue);
}	// DarkenColor

//-------------------------------------------------------------------
//
void CMacButton::CreatePens()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Deletes the pen objects, if necessary, and creates them.
//
{
	DeletePens();

	m_penFace.CreatePen(PS_SOLID, 1, m_crFace);
	m_penLiteFace.CreatePen(PS_SOLID, 1, m_crLiteFace);
	m_penLiteShadow.CreatePen(PS_SOLID, 1, m_crLiteShadow);
	m_penShadow.CreatePen(PS_SOLID, 1, m_crShadow);
	m_penDkShadow.CreatePen(PS_SOLID, 1, m_crDkShadow);

	if (m_nType != TYPE_MORE)
	{
		m_penHilight.CreatePen(PS_SOLID, 1, m_crHilight);
		m_penDarkDkShadow.CreatePen(PS_SOLID, 1, m_crDarkDkShadow);
	}
}	// CreatePens

//-------------------------------------------------------------------
//
void CMacButton::DeletePens()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Deletes the pen objects.
//
{
	if (m_penHilight.m_hObject)
		m_penHilight.DeleteObject();
	if (m_penFace.m_hObject)
		m_penFace.DeleteObject();
	if (m_penLiteFace.m_hObject)
		m_penLiteFace.DeleteObject();
	if (m_penLiteShadow.m_hObject)
		m_penLiteShadow.DeleteObject();
	if (m_penShadow.m_hObject)
		m_penShadow.DeleteObject();
	if (m_penDkShadow.m_hObject)
		m_penDkShadow.DeleteObject();
	if (m_penDarkDkShadow.m_hObject)
		m_penDarkDkShadow.DeleteObject();
}	// DeletePens

//-------------------------------------------------------------------
//
void CMacButton::DrawImage(CDC *pDC, CRect &rect, UINT nStyle, UINT nState)
//
// Return Value:	None.
//
// Parameters	:	pDC - A pointer to the DC to draw on.
//						sText - The text on the button.
//						rect - The button rectangle.
//						nStyle - The button's style.
//						nState - The button's state.
//
// Remarks		:	Draws an icon or bitmap on the button.
//
{
	// Calculate the left and right sides of the rect.
	if ((m_nType == TYPE_STANDARD) || (nStyle & BS_PUSHLIKE))
	{
		if (nStyle & BS_LEFT)
			rect.left += 5;
		else if (nStyle & BS_RIGHT)
			rect.left = rect.right - m_sizeImage.cx - 5;
		else
			rect.left = (rect.Width() - m_sizeImage.cx) >> 1;
	}	// if the button is standard or push-like
	else
	{
		if (nStyle & BS_LEFT)
			rect.left = (nStyle & BS_LEFTTEXT ? 2 : CHECKBOX_HEIGHT + 5);
		else if (nStyle & BS_RIGHT)
			rect.left = rect.right - m_sizeImage.cx - (nStyle & BS_LEFTTEXT ? CHECKBOX_HEIGHT + 7 : 2);
		// Else use the default vertical alignment (left for radios & checkboxes, center for push buttons).
		else
		{
			if (m_nType == TYPE_CHECKBOX || m_nType == TYPE_RADIO)
				rect.left = (nStyle & BS_LEFTTEXT ? 2 : CHECKBOX_HEIGHT + 5);
			else
				rect.left = (rect.Width() - m_sizeImage.cx) >> 1;
		}
	}
	rect.right = rect.left + m_sizeImage.cx;

	// Calculate the top and bottom of the rect.
	if (nStyle & BS_TOP)
		rect.top += ((m_nType == TYPE_STANDARD) || (nStyle & BS_PUSHLIKE) ? 4 : 1);
	else if (nStyle & BS_BOTTOM)
		rect.top = rect.bottom - m_sizeImage.cy - ((m_nType == TYPE_STANDARD) || (nStyle & BS_PUSHLIKE) ? 4 : 1);
	else
		rect.top = (rect.Height() - m_sizeImage.cy) >> 1;
	rect.bottom = rect.top + m_sizeImage.cy;

	// Adjust the image's rectangle depending on the effect
	if (m_nImageEffect == IMAGE_EFFECT_RAISED || m_nImageEffect == IMAGE_EFFECT_SUNKEN)
	{
		if ((m_nType == TYPE_STANDARD) || (nStyle & BS_PUSHLIKE))
		{
			if (nStyle & BS_LEFT)
			{
				rect.left++;
				rect.right++;
			}
			else if (nStyle & BS_RIGHT)
			{
				rect.left--;
				rect.right--;
			}
		}
		
		if (nStyle & BS_TOP)
		{
			rect.top++;
			rect.bottom++;
		}
		else if (nStyle & BS_BOTTOM)
		{
			rect.top--;
			rect.bottom--;
		}
	}

	// Draw the image.
	if (m_hIcon)
		pDC->DrawState(rect.TopLeft(), m_sizeImage, 
						m_hIcon, nState & ODS_DISABLED ? DSS_DISABLED : DSS_NORMAL, (HBRUSH)NULL);
	else if (m_hBitmap)
		pDC->DrawState(rect.TopLeft(), m_sizeImage, 
						m_hBitmap, nState & ODS_DISABLED ? DSS_DISABLED : DSS_NORMAL, (HBRUSH)NULL);

	if (m_nImageEffect)
	{
		rect.InflateRect(1, 1, 1, 1);
		if (m_nImageEffect == IMAGE_EFFECT_RAISED)
			pDC->Draw3dRect(rect, m_crHilight, m_crShadow);
		if (m_nImageEffect == IMAGE_EFFECT_SUNKEN)
			pDC->Draw3dRect(rect, m_crShadow, m_crHilight);
	}
}	// DrawImage

//-------------------------------------------------------------------
//
HICON CMacButton::SetIcon(HICON hIcon)
//
// Return Value:	The handle of an icon previously associated with the button.
//
// Parameters	:	hIcon - The handle of an icon.
//
// Remarks		:	Call this member function to associate a new icon with the button.
//
{
	m_hIcon = hIcon;
	m_hBitmap = 0;

	if (hIcon)
	{
		// Get icon dimensions.
		ICONINFO iconInfo;
		::ZeroMemory(&iconInfo, sizeof(ICONINFO));
		::GetIconInfo(m_hIcon, &iconInfo);

		m_sizeImage.cx = iconInfo.xHotspot << 1;
		m_sizeImage.cy = iconInfo.yHotspot << 1;
	}

	RedrawWindow();

	return CButton::SetIcon(hIcon);
}	// SetIcon

//-------------------------------------------------------------------
//
HBITMAP CMacButton::SetBitmap(HBITMAP hBitmap)
//
// Return Value:	The handle of a bitmap previously associated with the button.
//
// Parameters	:	hBitmap - The handle of a bitmap.
//
// Remarks		:	Call this member function to associate a new bitmap with the button.
//
{
	m_hBitmap = hBitmap;
	m_hIcon = 0;

	if (hBitmap)
	{
		// Get bitmap dimensions.
		CBitmap *pBmp = CBitmap::FromHandle(hBitmap);
		ASSERT(pBmp);
		BITMAP bmp;
		::ZeroMemory(&bmp, sizeof(BITMAP));
		pBmp->GetBitmap(&bmp);

		m_sizeImage.cx = bmp.bmWidth;
		m_sizeImage.cy = bmp.bmHeight;
	}

	RedrawWindow();

	return CButton::SetBitmap(hBitmap);
}	// SetBitmap

//-------------------------------------------------------------------
//
CRect CMacButton::GetCheckRect(const CRect &rect, UINT nStyle)
//
// Return Value:	A CRect containing the dimensions of the radio/check box.
//
// Parameters	:	rect - The rectangle of the entire button.
//						nStyle - The button's style.
//
// Remarks		:	Returns a rectangle containing the dimensions of the 
//						radio/check box. Only used with radio buttons or check boxes.
//
{
	CRect rectCheck(rect);

	// Calculate the left and right sides of the rect.
	if (nStyle & BS_LEFTTEXT)
	{
		rectCheck.left = rect.right - CHECKBOX_HEIGHT - (m_nType == TYPE_RADIO ? 0 : 2);
	}
	else
	{
		rectCheck.left += (m_nType == TYPE_RADIO ? 1 : 0);
		rectCheck.right = rect.left + CHECKBOX_HEIGHT + (m_nType == TYPE_RADIO ? 1 : 2);
	}

	// Calculate the top and bottom sides of the rect.
	if (nStyle & BS_TOP)
		rectCheck.top += 1;
	else if (nStyle & BS_BOTTOM)
		rectCheck.top = rect.bottom - CHECKBOX_HEIGHT - 3;
	else
		rectCheck.top = ((rect.Height() - CHECKBOX_HEIGHT) >> 1) - 1;
	rectCheck.bottom = rectCheck.top + CHECKBOX_HEIGHT;

	return rectCheck;
}	// GetCheckRect

//-------------------------------------------------------------------
//
void CMacButton::DrawUnpressedPushButton(CDC *pDC, const CRect &rect)
//
// Return Value:	None.
//
// Parameters	:	pDC - A pointer to the DC to draw on.
//						rect - The button's rectangle.
//
// Remarks		:	Draws an unpressed push button.
//
{
	pDC->RoundRect(rect, CPoint(6, 6));

	// Save myself some typing.
	int nLeft = rect.left;
	int nTop = rect.top;
	int nRight = rect.right;
	int nBottom = rect.bottom;

	pDC->SelectObject(&m_penHilight);
	pDC->MoveTo(nLeft + 2, nBottom - 4);
	pDC->LineTo(nLeft + 2, nTop + 2);
	pDC->LineTo(nRight - 3, nTop + 2);
	pDC->SetPixel(nLeft + 3, nTop + 3, m_crHilight);

	pDC->SelectObject(&m_penLiteShadow);
	pDC->MoveTo(nLeft + 3, nBottom - 3);
	pDC->LineTo(nRight - 4, nBottom - 3);
	pDC->LineTo(nRight - 4, nBottom - 4);
	pDC->LineTo(nRight - 3, nBottom - 4);
	pDC->LineTo(nRight - 3, nTop + 2);

	pDC->SelectObject(&m_penShadow);
	pDC->MoveTo(nLeft + 3, nBottom - 2);
	pDC->LineTo(nRight - 3, nBottom - 2);
	pDC->LineTo(nRight - 3, nBottom - 3);
	pDC->LineTo(nRight - 2, nBottom - 3);
	pDC->LineTo(nRight - 2, nTop + 2);

	pDC->SetPixel(nLeft, nTop + 2, m_crDarkDkShadow);
	pDC->SetPixel(nLeft + 2, nTop, m_crDarkDkShadow);
	pDC->SetPixel(nRight - 3, nTop, m_crDarkDkShadow);
	pDC->SetPixel(nRight - 1, nTop + 2, m_crDarkDkShadow);
	pDC->SetPixel(nRight - 1, nBottom - 3, m_crDarkDkShadow);
	pDC->SetPixel(nRight - 3, nBottom - 1, m_crDarkDkShadow);
	pDC->SetPixel(nLeft + 2, nBottom - 1, m_crDarkDkShadow);
	pDC->SetPixel(nLeft, nBottom - 3, m_crDarkDkShadow);

	pDC->SetPixel(nLeft + 1, nTop + 2, m_crLiteShadow);
	pDC->SetPixel(nLeft + 2, nTop + 1, m_crLiteShadow);
	pDC->SetPixel(nRight - 3, nTop + 1, m_crLiteShadow);
	pDC->SetPixel(nRight - 2, nTop + 2, m_crLiteShadow);
	pDC->SetPixel(nLeft + 2, nBottom - 2, m_crLiteShadow);
	pDC->SetPixel(nLeft + 1, nBottom - 3, m_crLiteShadow);

	DrawCornerShadows(pDC, rect);
}	// DrawUnpressedPushButton

//-------------------------------------------------------------------
//
void CMacButton::DrawPressedPushButton(CDC *pDC, const CRect &rect)
//
// Return Value:	None.
//
// Parameters	:	pDC - A pointer to the DC to draw on.
//						rect - The button's rectangle.
//
// Remarks		:	Draws a pressed push button.
//
{
	pDC->RoundRect(rect, CPoint(6, 6));

	// Save myself some typing.
	int nLeft = rect.left;
	int nTop = rect.top;
	int nRight = rect.right;
	int nBottom = rect.bottom;

	pDC->SelectObject((m_nType == TYPE_STANDARD) || m_bMouseDown ? &m_penDarkDkShadow : &m_penShadow);
	pDC->MoveTo(nLeft + 1, nBottom - 4);
	pDC->LineTo(nLeft + 1, nTop + 2);
	pDC->LineTo(nLeft + 2, nTop + 2);
	pDC->LineTo(nLeft + 2, nTop + 1);
	pDC->LineTo(nRight - 3, nTop + 1);

	pDC->SelectObject((m_nType == TYPE_STANDARD) || m_bMouseDown ? &m_penShadow : &m_penLiteShadow);
	pDC->MoveTo(nLeft + 2, nBottom - 4);
	pDC->LineTo(nLeft + 2, nTop + 3);
	pDC->LineTo(nLeft + 3, nTop + 3);
	pDC->LineTo(nLeft + 3, nTop + 2);
	pDC->LineTo(nRight - 3, nTop + 2);

	pDC->SelectObject((m_nType == TYPE_STANDARD) || !(m_nCheck && !m_bMouseDown) ? &m_penLiteFace : &m_penHilight);
	pDC->MoveTo(nLeft + 3, nBottom - 2);
	pDC->LineTo(nRight - 4, nBottom - 2);
	pDC->LineTo(nRight - 2, nBottom - 4);
	pDC->LineTo(nRight - 2, nTop + 2);

	pDC->SelectObject((m_nType == TYPE_STANDARD) || !(m_nCheck && !m_bMouseDown) ? &m_penFace : &m_penLiteFace);
	pDC->MoveTo(nLeft + 3, nBottom - 3);
	pDC->LineTo(nRight - 4, nBottom - 3);
	pDC->LineTo(nRight - 4, nBottom - 4);
	pDC->LineTo(nRight - 3, nBottom - 4);
	pDC->LineTo(nRight - 3, nTop + 2);

	pDC->SetPixel(nRight - 2, nTop + 2, ((m_nType == TYPE_STANDARD) || m_bMouseDown ? m_crFace : m_crLiteFace));
	pDC->SetPixel(nLeft + 2, nBottom - 2, ((m_nType == TYPE_STANDARD) || m_bMouseDown ? m_crFace : m_crLiteFace));
	pDC->SetPixel(nRight - 3, nTop + 1, m_crShadow);
	pDC->SetPixel(nLeft + 1, nBottom - 3, m_crShadow);

	DrawCornerShadows(pDC, rect);
}	// DrawPressedPushButton

//-------------------------------------------------------------------
//
void CMacButton::DrawCornerShadows(CDC *pDC, const CRect &rect)
//
// Return Value:	None.
//
// Parameters	:	pDC - A pointer to the DC to draw on.
//						rect - The button's rectangle.
//
// Remarks		:	Draws the little "shadows" on the corners of push buttons.
//
{
	pDC->SetPixel(rect.left, rect.top + 2, m_crDarkDkShadow);
	pDC->SetPixel(rect.left + 2, rect.top, m_crDarkDkShadow);
	pDC->SetPixel(rect.right - 3, rect.top, m_crDarkDkShadow);
	pDC->SetPixel(rect.right - 1, rect.top + 2, m_crDarkDkShadow);
	pDC->SetPixel(rect.right - 1, rect.bottom - 3, m_crDarkDkShadow);
	pDC->SetPixel(rect.right - 3, rect.bottom - 1, m_crDarkDkShadow);
	pDC->SetPixel(rect.left + 2, rect.bottom - 1, m_crDarkDkShadow);
	pDC->SetPixel(rect.left, rect.bottom - 3, m_crDarkDkShadow);
}	// DrawCornerShadows

//-------------------------------------------------------------------
//
void CMacButton::DrawPushLikeButton(CDC *pDC, const CRect &rect, UINT nStyle, UINT nState)
//
// Return Value:	None.
//
// Parameters	:	pDC - A pointer to the DC to draw on.
//						rect - The button's rectangle.
//						nStyle - The button's style.
//						nState - The button's state.
//
// Remarks		:	Draws a push-like check box. This function is only used
//						to draw radio buttons and check boxes that have the 
//						BS_PUSHLIKE style.
//
{
	CBrush brFill;

	// Draw a flat button.
	if (nStyle & BS_FLAT)
	{
		COLORREF crFill = ::GetSysColor(COLOR_WINDOW);

		HBITMAP hBmp = GetDitherBitmap(pDC, crFill, ~crFill);
		CBitmap *pBmp = (hBmp ? CBitmap::FromHandle(hBmp) : NULL);
		ASSERT(pBmp);

		// If the mouse is down, use a pattern brush to fill with. 
		// If the button is checked, use the inverted window color.
		if (m_bMouseDown)
			brFill.CreatePatternBrush(pBmp);
		else if (m_nCheck)
			brFill.CreateSolidBrush(~crFill);
		else
			brFill.CreateSolidBrush(crFill);

		CBrush *pOldBrush = (CBrush *)pDC->SelectObject(&brFill);
		CPen pen;
		if (nState & ODS_DISABLED)
			pen.CreatePen(PS_SOLID, 1, m_crDarkDkShadow);
		else if (m_nCheck || m_bMouseDown)
			pen.CreatePen(PS_SOLID, 1, crFill);
		else
			pen.CreatePen(PS_SOLID, 1, ~crFill);

		CPen *pOldPen = (CPen *)pDC->SelectObject(&pen);
		pDC->RoundRect(rect, CPoint(6, 6));
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
		brFill.DeleteObject();
		return;
	}	// if flat

	HBITMAP hBmp = GetDitherBitmap(pDC, m_crHilight, m_crFace);
	CBitmap *pBmp = (hBmp ? CBitmap::FromHandle(hBmp) : NULL);
	ASSERT(pBmp);

	// Create the brush to fill with.
	if (m_bMouseDown)
		brFill.CreateSolidBrush(m_crLiteShadow);
	else if (m_nCheck)
		brFill.CreatePatternBrush(pBmp);
	else
		brFill.CreateSolidBrush(m_crFace);

	::DeleteObject(hBmp);

	CBrush *pOldBrush	= (CBrush *)pDC->SelectObject(&brFill);
	CPen penBorder(PS_SOLID, 1, nState & ODS_DISABLED ? m_crShadow : ::GetSysColor(COLOR_WINDOWFRAME));
	CPen *pOldPen = (CPen *)pDC->SelectObject(&penBorder);

	// Draw the pressed or unpressed button
	if (nState & ODS_DISABLED)
		pDC->RoundRect(rect, CPoint(6, 6));
	else if (m_bMouseDown || m_nCheck)
		DrawPressedPushButton(pDC, rect);
	else
		DrawUnpressedPushButton(pDC, rect);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	brFill.DeleteObject();
}	// DrawPushLikeButton

//-------------------------------------------------------------------
//
HBITMAP CMacButton::GetDitherBitmap(CDC *pDC, COLORREF crColor1, COLORREF crColor2)
//
// Return Value:	An HBITMAP.
//
// Parameters	:	pDC - A pointer to the DC being drawn on.
//						crColor1, crColor2 - Specify the colors used to create a 
//							dither bitmap
//
// Remarks		:	Returns a dithered HBITMAP that is used to create
//						a patter brush for filling a push-like radio button or
//						check box when it is checked.
//
{
	struct  // BITMAPINFO with 16 colors
	{
		BITMAPINFOHEADER    bmiHeader;
		RGBQUAD             bmiColors[16];
	}	bmi;

	DWORD patGray[8];

	bmi.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth        = 2;
	bmi.bmiHeader.biHeight       = 2;
	bmi.bmiHeader.biPlanes       = 1;
	bmi.bmiHeader.biBitCount     = 1;
	bmi.bmiHeader.biCompression  = BI_RGB;
	bmi.bmiHeader.biSizeImage    = 0;
	bmi.bmiHeader.biXPelsPerMeter= 0;
	bmi.bmiHeader.biYPelsPerMeter= 0;
	bmi.bmiHeader.biClrUsed      = 0;
	bmi.bmiHeader.biClrImportant = 0;

	bmi.bmiColors[0].rgbRed      = GetRValue(crColor1);
	bmi.bmiColors[0].rgbGreen    = GetGValue(crColor1);
	bmi.bmiColors[0].rgbBlue     = GetBValue(crColor1);
	bmi.bmiColors[0].rgbReserved = 0;

	bmi.bmiColors[1].rgbRed      = GetRValue(crColor2);
	bmi.bmiColors[1].rgbGreen    = GetGValue(crColor2);
	bmi.bmiColors[1].rgbBlue     = GetBValue(crColor2);
	bmi.bmiColors[1].rgbReserved = 0;

   // Create the byte array for CreateDIBitmap.
   patGray[6] = patGray[4] = patGray[2] = patGray[0] = 0x5555AAAAL;
   patGray[7] = patGray[5] = patGray[3] = patGray[1] = 0xAAAA5555L;

	return CreateDIBitmap(pDC->m_hDC, &bmi.bmiHeader, CBM_INIT, patGray, 
									(LPBITMAPINFO)&bmi, DIB_RGB_COLORS);
}	// GetDitherBitmap

//-------------------------------------------------------------------
//
void CMacButton::RedrawCheck()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Invalidates the check rectangle of a radio button or check box.
//
{
	CRect rect;
	GetClientRect(rect);
	InvalidateRect(GetCheckRect(rect, GetStyle()));
}	// RedrawCheck

//-------------------------------------------------------------------
//
void CMacButton::DrawButtonText(CDC *pDC, CRect &rect, const CString &sText, UINT nStyle, UINT nState)
//
// Return Value:	None.
//
// Parameters	:	pDC - A pointer to the DC being drawn on.
//						rect - The button's rectangle.
//						sText - The button's text.
//						nStyle - The button's style.
//						nState - The button's state.
//
// Remarks		:	Draws the text on the button.
//
{
	CFont *pFont = GetFont();
	CFont *pOldFont = (CFont *)pDC->SelectObject(pFont);
	CFont fontBold;

	if (m_bBold)
	{
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		lf.lfWeight = FW_BOLD;
		fontBold.CreateFontIndirect(&lf);
		pDC->SelectObject(&fontBold);
	}
	

	CSize sizeText = pDC->GetTextExtent(sText);

	// Determine the rect for the text.
	if ((m_nType == TYPE_STANDARD) || (nStyle & BS_PUSHLIKE))
	{
		if (nStyle & BS_LEFT)
			rect.left += 5;
		else if (nStyle & BS_RIGHT)
			rect.left = rect.right - sizeText.cx - 5;
		else
			rect.left = (rect.Width() - sizeText.cx) >> 1;
		rect.right = rect.left + sizeText.cx;

		if (nStyle & BS_TOP)
			rect.top += (m_nType == TYPE_RADIO? 4 : 5);
		else if (nStyle & BS_BOTTOM)
			rect.top = rect.bottom - sizeText.cy - 5;
		else
			rect.top = (rect.Height() - sizeText.cy) >> 1;
		rect.bottom = rect.top + sizeText.cy;
	}
	else
	{
		if (nStyle & BS_LEFT)
			rect.left = (nStyle & BS_LEFTTEXT ? 2 : CHECKBOX_HEIGHT + 5);
		else if (nStyle & BS_RIGHT)
			rect.left = rect.right - sizeText.cx - (nStyle & BS_LEFTTEXT ? CHECKBOX_HEIGHT + 10 : 5);
		else
		{
			if (m_nType == TYPE_CHECKBOX || m_nType == TYPE_RADIO)
				rect.left = (nStyle & BS_LEFTTEXT ? 2 : CHECKBOX_HEIGHT + 5);
			else
				rect.left = (rect.Width() - sizeText.cx) >> 1;
		}
		rect.right = rect.left + sizeText.cx + (nStyle & BS_LEFTTEXT ? 2 : 0);

		if (nStyle & BS_TOP)
			rect.top = rect.top;
		else if (nStyle & BS_BOTTOM)
			rect.top = rect.bottom - sizeText.cy - 3;
		else
			rect.top = ((rect.Height() - sizeText.cy) >> 1) - 1;
		rect.bottom = rect.top + sizeText.cy;
	}

	if (((m_nType == TYPE_STANDARD) && (nState & ODS_SELECTED)) || 
		 (nStyle & BS_PUSHLIKE) && (m_bMouseDown || ((nStyle & BS_FLAT) && m_nCheck)))
		pDC->SetTextColor(m_crHilight);
	if ((m_nType == TYPE_CHECKBOX) && (nStyle & BS_PUSHLIKE) && (m_nCheck == 2))
		pDC->SetTextColor(m_crShadow);

	// Draw the text.
	pDC->SetBkMode(TRANSPARENT);
	if (nState & ODS_DISABLED)
	{
		pDC->SetTextColor(m_crShadow);
		pDC->DrawText(sText, rect, DT_CENTER);
	}
	else
		pDC->DrawText(sText, rect, DT_CENTER);

	// Restore the original font.
	pDC->SelectObject(pOldFont);
	if (m_bBold)
		fontBold.DeleteObject();

}	// DrawButtonText

//-------------------------------------------------------------------
//
void CMacButton::SetImageEffect(int nEffect)
//
// Return Value:	None.
//
// Parameters	:	nEffect - The effect to add when drawing the image.
//							Can be one of the following values: 
//							- IMAGE_EFFECT_NONE - No effect is added to the image.
//							- IMAGE_EFFECT_RAISED - The image will appear raised.
//							- IMAGE_EFFECT_SUNKEN - The image will appear sunken.
//
// Remarks		:	Sets the image effect member.
//
{
	m_nImageEffect = nEffect;	
	RedrawWindow();
}	// SetImageEffect

//-------------------------------------------------------------------
//
void CMacButton::SetCheck(int nCheck)
//
// Return Value:	None.
//
// Parameters	:	nCheck - Specifies the check state. This parameter 
//							can be one of the following:
//							Value	Meaning 
//							0		Set the button state to unchecked. 
//							1		Set the button state to checked. 
//							2		Set the button state to indeterminate.
//
// Remarks		:	Sets or resets the check state of a radio button or 
//						check box. This member function has no effect on a pushbutton.
//
{
	if (m_nType == TYPE_STANDARD)
		return;

	int nOldCheck = m_nCheck;

	m_nCheck = nCheck;

	if (m_nCheck != nOldCheck)
	{
		if (GetStyle() & BS_PUSHLIKE)
			RedrawWindow();
		else
			RedrawCheck();
	}
}	// SetCheck

//-------------------------------------------------------------------
//
int CMacButton::GetCheck() const
//
// Return Value:	1 if checked, 0 otherwise.
//
// Parameters	:	None.
//
// Remarks		:	Retrieves the check state of button.
//
{
	return m_nCheck;
}	// GetCheck

//-------------------------------------------------------------------
//
void CMacButton::SetBold(BOOL bBold /*= TRUE*/)
//
// Return Value:	None.
//
// Parameters	:	bBold - Used to sed the m_bBold flag. Default value: TRUE.
//
// Remarks		:	Sets the m_bBold flag.
//
{
	m_bBold = bBold;
	RedrawWindow();
}	// SetBold

//-------------------------------------------------------------------
//
BOOL CMacButton::GetBold() const
//
// Return Value:	The bold flag.
//
// Parameters	:	None.
//
// Remarks		:	Returns the bold flag.
//
{
	return m_bBold;
}	// GetBold

/////////////////////////////////////////////////////////////////////////////
//
//	CMacCheckBox class, version 2.0
//
//	Copyright (c) 1999, 2000 Paul M. Meidinger (pmmeidinger@yahoo.com)
//
// Thanks to:
//		Eric Hwang <erichw@21cn.com>
//			For fixing the problem that was not sending a message
//			to the parent when the button was clicked.
//
//	History:
//		PMM	12/13/1999		Initial implementation.		
//
//		PMM	12/17/1999		Modified drawing code to use a memory DC. 
//									Added CPen member variables in an attempt to 
//									speed up drawing. Made other minor changes.
//
//		PMM	12/29/1999		Fixed a bug that would not send a message to the parent
//									when the button was clicked (fix by Eric Hwang). Added
//									code to draw a push-like button when that style is used.
//
/////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------
//
CMacCheckBox::CMacCheckBox()
	: CMacButton()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Standard constructor.
//
{
	m_nType = TYPE_CHECKBOX;
	m_nCheckStyle = CHECK_STYLE_CHECK;
}	// CMacCheckBox

//-------------------------------------------------------------------
//
CMacCheckBox::~CMacCheckBox()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Destructor. Does nothing.
//
{
}	// ~CMacCheckBox


BEGIN_MESSAGE_MAP(CMacCheckBox, CMacButton)
	//{{AFX_MSG_MAP(CMacCheckBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacCheckBox message handlers

//-------------------------------------------------------------------
//
void CMacCheckBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
//
// Return Value:	None.
//
// Parameters	:	lpDIS - A long pointer to a DRAWITEMSTRUCT structure.
//
// Remarks		:	Called by the framework when a visual aspect of an 
//						owner-drawn button has changed.
//
{
	DrawButton(lpDIS);
}	// DrawItem

//-------------------------------------------------------------------
//
void CMacCheckBox::OnLButtonUp(UINT /*nFlags*/, CPoint point) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down. 
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left 
//							corner of the window.
//
// Remarks		:	The framework calls this member function when the user 
//						releases the left mouse button. Checks to see if the mouse
//						was released over the button. If so, check it.
//
{
	ReleaseCapture();

	UINT nStyle = GetStyle();

	if (!(nStyle & BS_AUTOCHECKBOX))
		return;

	CRect rect;
	GetClientRect(rect);

	if (rect.PtInRect(point))
	{
		m_nCheck = (m_nCheck == 0 ? 1 : 0);

		// Send notify message to parent window.
		// Added by Eric Hwang.
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
	}

	m_bMouseDown = FALSE;

	if (nStyle & BS_PUSHLIKE)
		RedrawWindow();
	else
		RedrawCheck();
}	// OnLButtonUp

//-------------------------------------------------------------------
//
void CMacCheckBox::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down. 
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left 
//							corner of the window.
//
// Remarks		:	The framework calls this member function when the user 
//						presses the left mouse button. Sets focus to this button,
//						captures the mouse, and redraws the check.
//
{
	m_bMouseDown = TRUE;
	SetFocus();
	SetCapture();

	if (GetStyle() & BS_PUSHLIKE)
		RedrawWindow();
	else
		RedrawCheck();
}	// OnLButtonDown 

//-------------------------------------------------------------------
//
void CMacCheckBox::OnLButtonDblClk(UINT /*nFlags*/, CPoint point) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down.
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left
//							 corner of the window.
//
// Remarks		:	The framework calls this member function when the user 
//						double-clicks the left mouse button. The WM_LBUTTONDOWN
//						message is sent to simulate a single click.
//
{
	SendMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(point.x, point.y));	
}	// OnLButtonDblClk

//-------------------------------------------------------------------
//
void CMacCheckBox::OnMouseMove(UINT /*nFlags*/, CPoint point) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down. 
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left 
//							corner of the window.
//
// Remarks		:	The framework calls this member function when the 
//						mouse cursor moves. Checks to see if the mouse is over
//						the button, and redraws the check if it is, but wasn't previously.
//
{
	if (GetCapture() != this)
		return;

	BOOL bWasMouseDown = m_bMouseDown;

	CRect rect;
	GetClientRect(rect);
	m_bMouseDown = rect.PtInRect(point);

	if (bWasMouseDown != m_bMouseDown)
	{
		if (GetStyle() & BS_PUSHLIKE)
			RedrawWindow();
		else
			RedrawCheck();
	}
}	// OnMouseMove

//-------------------------------------------------------------------
//
void CMacCheckBox::OnKeyUp(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/) 
//
// Return Value:	None.
//
// Parameters	:	nChar - Specifies the virtual-key code of the given key.
//						nRepCnt - Repeat count (the number of times the keystroke
//							is repeated as a result of the user holding down the key).
//						nFlags - Specifies the scan code, key-transition code, previous 
//							key state, and context code
//
// Remarks		:	The framework calls this member function when a nonsystem 
//						key is released. Checks/unchecks the button when the space bar
//						is pressed.
//
{		
	if (nChar == VK_SPACE)
	{
		m_bMouseDown = FALSE;
		m_nCheck = (m_nCheck == 0 ? 1 : 0);

		// Send notify message to parent window.
		// Added by Eric Hwang.
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());

		if (GetStyle() & BS_PUSHLIKE)
			RedrawWindow();
		else
			RedrawCheck();
	}
}	// OnKeyUp

//-------------------------------------------------------------------
//
void CMacCheckBox::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/) 
//
// Return Value:	None.
//
// Parameters	:	nChar - Specifies the virtual-key code of the given key.
//						nRepCnt - Repeat count (the number of times the keystroke 
//							is repeated as a result of the user holding down the key).
//						nFlags - Specifies the scan code, key-transition code, 
//							previous key state, and context code
//
// Remarks		:	The framework calls this member function when a 
//						nonsystem key is pressed.
//
{
	if (nChar == VK_SPACE)
	{
		if (!m_bMouseDown)
		{
			m_bMouseDown = TRUE;
			if (GetStyle() & BS_PUSHLIKE)
				RedrawWindow();
			else
				RedrawCheck();
		}
	}
}	// OnKeyDown

//-------------------------------------------------------------------
//
void CMacCheckBox::SetCheckStyle(int nStyle /* = CHECK_STYLE_CHECK*/)
//
// Return Value:	None.
//
// Parameters	:	nStyle - The check style to set.	Can be one of the
//							following values:
//								CHECK_STYLE_CHECK		Check mark style.
//								CHECK_STYLE_CROSS		"X" style.
//
// Remarks		:	Sets the check style.
//
{
	m_nCheckStyle = nStyle;
	RedrawCheck();
}	// SetCheckStyle

//-------------------------------------------------------------------
//
int CMacCheckBox::GetCheckStyle() const
//
// Return Value:	The current check style.
//
// Parameters	:	None.
//
// Remarks		:	Gets the current check style.
//
{
	return m_nCheckStyle;
}	// GetCheckStyle

/////////////////////////////////////////////////////////////////////////////
//
//	CMacRadioButton class, version 2.0
//
//	Copyright (c) 1999, 2000 Paul M. Meidinger (pmmeidinger@yahoo.com)
//
// Thanks to:
//		Eric Hwang <erichw@21cn.com>
//			For fixing the problem that was not sending a message
//			to the parent when the button was clicked.
//
//	History:
//		PMM	12/13/1999		Initial implementation.		
//
//		PMM	12/17/1999		Modified drawing code to use a memory DC. 
//									Added CPen member variables in an attempt to 
//									speed up drawing. Made other minor changes.
//
//		PMM	12/29/1999		Fixed a bug that was not checking the radio
//									and unchecking others when the space bar is pressed.
//									Fixed a bug that would not send a message to the parent
//									when the button was clicked (fix by Eric Hwang). Added
//									code to draw a push-like button when that style is used.
//
/////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------
//
CMacRadioButton::CMacRadioButton()
	: CMacButton()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Standard constructor.
//
{
	m_nType = TYPE_RADIO;
}	// CMacRadioButton

//-------------------------------------------------------------------
//
CMacRadioButton::~CMacRadioButton()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Destructor. Does nothing.
//
{
}	// ~CMacRadioButton


BEGIN_MESSAGE_MAP(CMacRadioButton, CMacButton)
	//{{AFX_MSG_MAP(CMacRadioButton)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacRadioButton message handlers

//-------------------------------------------------------------------
//
void CMacRadioButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
//
// Return Value:	None.
//
// Parameters	:	lpDIS - A long pointer to a DRAWITEMSTRUCT structure.
//
// Remarks		:	Called by the framework when a visual aspect of an 
//						owner-drawn button has changed.
//
{
	DrawButton(lpDIS);
}	// DrawItem

//-------------------------------------------------------------------
//
BOOL CMacRadioButton::PreTranslateMessage(MSG* pMsg) 
//
// Return Value:	Nonzero if the message was translated and should not 
//						be dispatched; 0 if the message was not translated 
//						and should be dispatched.
//
// Parameters	:	pMsg - Points to a MSG structure that contains the 
//							message to process.
//
// Remarks		:	Used by class CWinApp to translate window messages 
//						before they are dispatched to theTranslateMessage 
//						andDispatchMessage Windows functions. Catches the 
//						WM_KEYDOWN message and handles the up, down, left
//						and right arrows.
//
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DOWN || pMsg->wParam == VK_UP ||
			 pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT)
		{
			UncheckOtherRadios();

			// Get the previous/next button and check it.
			CWnd *pParent = GetParent();
			ASSERT(pParent);

			CWnd *pWnd = pParent->GetNextDlgGroupItem((CWnd *)this, (pMsg->wParam == VK_LEFT || pMsg->wParam == VK_UP ? TRUE : FALSE));
			if (pWnd && (((CButton*)pWnd)->GetButtonStyle() & BS_AUTORADIOBUTTON) && ((CMacRadioButton *)pWnd)->m_nType == TYPE_RADIO)
			{
				((CMacRadioButton *)pWnd)->SetCheck(1);

				// Send notify message to parent window.
				// Added by Eric Hwang.
				pParent->SendMessage(WM_COMMAND, MAKEWPARAM(pWnd->GetDlgCtrlID(), BN_CLICKED), (LPARAM)pWnd->GetSafeHwnd());
			}
			SetCheck(0);
			return FALSE;
		}
	}
	return CMacButton::PreTranslateMessage(pMsg);
}	// PreTranslateMessage

//-------------------------------------------------------------------
//
void CMacRadioButton::OnLButtonUp(UINT /*nFlags*/, CPoint point) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down. 
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left 
//							corner of the window.
//
// Remarks		:	The framework calls this member function when the user 
//						releases the left mouse button. Checks to see if the mouse
//						was released over the button. If so, check it and uncheck others.
//
{
	ReleaseCapture();

	UINT nStyle = GetStyle();

	CRect rect;
	GetClientRect(rect);
	if (rect.PtInRect(point))
	{
		if (!m_nCheck)
			m_nCheck = 1;

		// Uncheck all other radios in the group
		if ((nStyle & BS_AUTORADIOBUTTON) && m_nCheck)
		{
			UncheckOtherRadios();
		}

		// Send notify message to parent window.
		// Added by Eric Hwang
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
	}

	m_bMouseDown = FALSE;

	if (nStyle & BS_PUSHLIKE)
		RedrawWindow();
	else
		RedrawCheck();

//	CMacButton::OnLButtonUp(nFlags, point);
}	// OnLButtonUp

//-------------------------------------------------------------------
//
void CMacRadioButton::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down. 
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left 
//							corner of the window.
//
// Remarks		:	The framework calls this member function when the user 
//						presses the left mouse button. Sets focus to this button,
//						captures the mouse, and redraws the check.
//
{
	m_bMouseDown = TRUE;
	SetFocus();
	SetCapture();

	if (GetStyle() & BS_PUSHLIKE)
		RedrawWindow();
	else
		RedrawCheck();
//	CMacButton::OnLButtonDown(nFlags, point);
}	// OnLButtonDown

//-------------------------------------------------------------------
//
void CMacRadioButton::OnMouseMove(UINT nFlags, CPoint point) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down. 
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left 
//							corner of the window.
//
// Remarks		:	The framework calls this member function when the 
//						mouse cursor moves. Checks to see if the mouse is over
//						the button, and redraws the check if it is, but wasn't previously.
//
{
	if (GetCapture() != this)
		return;

	BOOL bWasMouseDown = m_bMouseDown;

	CRect rect;
	GetClientRect(rect);
	m_bMouseDown = rect.PtInRect(point);

	if (bWasMouseDown != m_bMouseDown)
	{
		if (GetStyle() & BS_PUSHLIKE)
			RedrawWindow();
		else
			RedrawCheck();
	}

	CMacButton::OnMouseMove(nFlags, point);
}	// OnMouseMove

//-------------------------------------------------------------------
//
void CMacRadioButton::OnLButtonDblClk(UINT /*nFlags*/, CPoint point) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down.
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left
//							 corner of the window.
//
// Remarks		:	The framework calls this member function when the user 
//						double-clicks the left mouse button. The WM_LBUTTONDOWN
//						message is sent to simulate a single click.
//
{
	SendMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(point.x, point.y));	
}	// OnLButtonDblClk

//-------------------------------------------------------------------
//
void CMacRadioButton::OnKeyUp(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/) 
//
// Return Value:	None.
//
// Parameters	:	nChar - Specifies the virtual-key code of the given key.
//						nRepCnt - Repeat count (the number of times the keystroke 
//							is repeated as a result of the user holding down the key).
//						nFlags - Specifies the scan code, key-transition code, 
//							previous key state, and context code
//
// Remarks		:	The framework calls this member function when a 
//						nonsystem key is released. 
//
{
	if (nChar == VK_SPACE)
	{
		m_nCheck = 1;

		// Send notify message to parent window.
		// Added by Eric Hwang.
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());

		UncheckOtherRadios();
		m_bMouseDown = FALSE;
		if (GetStyle() & BS_PUSHLIKE)
			RedrawWindow();
		else
			RedrawCheck();
	}
}	// OnKeyUp

//-------------------------------------------------------------------
//
void CMacRadioButton::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/) 
//
// Return Value:	None.
//
// Parameters	:	nChar - Specifies the virtual-key code of the given key.
//						nRepCnt - Repeat count (the number of times the keystroke 
//							is repeated as a result of the user holding down the key).
//						nFlags - Specifies the scan code, key-transition code, 
//							previous key state, and context code
//
// Remarks		:	The framework calls this member function when a 
//						nonsystem key is pressed.
//
{
	if (nChar == VK_SPACE)
	{
		if (!m_bMouseDown)
		{
			m_bMouseDown = TRUE;
			if (GetStyle() & BS_PUSHLIKE)
				RedrawWindow();
			else
				RedrawCheck();
		}
	}
}	// OnKeyDown

//-------------------------------------------------------------------
//
void CMacRadioButton::OnSysColorChange() 
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	The framework calls this member function for all 
//						top-level windows when a change is made in the 
//						system color setting
//
{
	CMacButton::OnSysColorChange();
	
	GetColors();
	CreatePens();
}	// OnSysColorChange

//-------------------------------------------------------------------
//
void CMacRadioButton::UncheckOtherRadios()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Attempts to uncheck all other radio buttons in the group.
//
{
	// Go backwards (GetNextDlgItem will wrap around to the end of the group).
	CWnd *pParent = GetParent();
	ASSERT(pParent);

	CWnd *pWnd = pParent->GetNextDlgGroupItem((CWnd *)this);
	while (pWnd && (pWnd != this))
	{
		if ((((CButton*)pWnd)->GetButtonStyle() & BS_AUTORADIOBUTTON) && 
			 ((CMacRadioButton *)pWnd)->m_nType == TYPE_RADIO)
		{
			((CMacRadioButton *)pWnd)->SetCheck(0);
		}
		pWnd = pParent->GetNextDlgGroupItem(pWnd);
	}
}	// UncheckOtherRadios

//-------------------------------------------------------------------
//
void CMacRadioButton::OnSetFocus(CWnd* pOldWnd) 
//
// Return Value:	None.
//
// Parameters	:	pOldWnd - Contains the CWnd object that loses the 
//							input focus (may be NULL). The pointer may be 
//							temporary and should not be stored for later use.
//
// Remarks		:	The framework calls this member function after gaining 
//						the input focus.
//
{
	CMacButton::OnSetFocus(pOldWnd);
	
	if (m_nCheck || m_bMouseDown)
		return;

	// If another button in this group is checked, set the 
	// focus to that button; if not, set the focus to this button.

	// Get the previous/next button and check it.
	CWnd *pParent = GetParent();
	ASSERT(pParent);

	CWnd *pWnd = pParent->GetNextDlgGroupItem((CWnd *)this);
	while (pWnd && (pWnd != this) && (((CButton*)pWnd)->GetButtonStyle() & BS_AUTORADIOBUTTON) && ((CMacRadioButton *)pWnd)->m_nType == TYPE_RADIO)
	{
		if (((CMacRadioButton*)pWnd)->GetCheck())
		{
			pWnd->SetFocus();
			break;
		}

		pWnd = pParent->GetNextDlgGroupItem(pWnd);
	}
}	// OnSetFocus


/////////////////////////////////////////////////////////////////////////////
//
//	CMacMoreButton class, version 2.0
//
//	Copyright (c) 2000 Paul M. Meidinger (pmmeidinger@yahoo.com)
//
//	History:
//		PMM	1/21/2000		Initial implementation.		
//
/////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------
//
CMacMoreButton::CMacMoreButton()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Standard constructor.
//
{
	m_nType = TYPE_MORE;
	m_nAlignText = ALIGN_TEXT_LEFT;
	m_bMouseDown = FALSE;
	m_bDoEffect = FALSE;
	m_bMoreShown = FALSE;
	SetColor(::GetSysColor(COLOR_3DFACE));
}	// CMacMoreButton

//-------------------------------------------------------------------
//
CMacMoreButton::~CMacMoreButton()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	Destructor. Does nothing.
//
{
}	// ~CMacMoreButton


BEGIN_MESSAGE_MAP(CMacMoreButton, CButton)
	//{{AFX_MSG_MAP(CMacMoreButton)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacMoreButton message handlers

//-------------------------------------------------------------------
//
void CMacMoreButton::PreSubclassWindow()
//
// Return Value:	None.
//
// Parameters	:	None.
//
// Remarks		:	This member function is called by the framework to 
//						allow other necessary subclassing to occur before the 
//						window is subclassed. Adds the BS_OWNERDRAW style and
//						resizes the button.
//
{
	CMacButton::PreSubclassWindow();

	// Add length to the right of the button to accommodate the arrow.
	CRect rect;
	GetClientRect(rect);
	rect.right += 12;

	// Convert the button's client coordinates to the parent's client coordinates.
	ClientToScreen(rect);
	CWnd* pParent = GetParent();
	if (pParent)						
		pParent->ScreenToClient(rect);
	
	// Resize the window.
	MoveWindow(rect, TRUE);
}	// PreSubclassWindow

//-------------------------------------------------------------------
//
void CMacMoreButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
//
// Return Value:	None.
//
// Parameters	:	lpDIS - A long pointer to a DRAWITEMSTRUCT structure.
//
// Remarks		:	Called by the framework when a visual aspect of an 
//						owner-drawn button has changed.
//
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rectItem(lpDIS->rcItem);

	// Create a mem DC for drawing
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rectItem.Width(), rectItem.Height());
	CBitmap *pOldBmp = dcMem.SelectObject(&bmp);

	int nSaveDC = dcMem.SaveDC();

	dcMem.SelectStockObject(NULL_BRUSH);
	dcMem.FillSolidRect(rectItem, ::GetSysColor(COLOR_3DFACE));

	// Draw the text, if any.
	CString sText;
	GetWindowText(sText);
	if (!sText.IsEmpty())
	{
		CFont *pOldFont = (CFont *)dcMem.SelectObject(GetFont());
		CSize sizeText = dcMem.GetTextExtent(sText);
		dcMem.SetBkMode(TRANSPARENT);

		CRect rectText(rectItem);
		rectText.top = (rectText.Height() - sizeText.cy) >> 1;

		dcMem.SetTextColor(lpDIS->itemState & ODS_DISABLED ? m_crShadow : m_crDkShadow);

		if (m_nAlignText == ALIGN_TEXT_RIGHT)
			rectText.left += 16;

		dcMem.DrawText(sText, rectText, DT_LEFT | DT_VCENTER);

		dcMem.SelectObject(pOldFont);
	}

	int nRight = (sText.IsEmpty() || (m_nAlignText == ALIGN_TEXT_RIGHT) ? 
					rectItem.left + 10 : rectItem.right - 2);
	int nTop = (rectItem.Height() - 11) >> 1;

	if (lpDIS->itemState & ODS_DISABLED)
	{
		// Select the frame pen, since we always draw with that first.
		CPen penShadow(PS_SOLID, 1, m_crShadow);
		CPen *pOldPen = (CPen *)dcMem.SelectObject(&penShadow);
		
		if (m_bMoreShown)
		{
			for (int i = 0; i < 6; i++)
			{
				dcMem.MoveTo(nRight - i, nTop + 3 + i);
				dcMem.LineTo((nRight - 11) + i, nTop + 3 + i);
			}
		}
		else
		{
			for (int i = 0; i < 6; i++)
			{
				dcMem.MoveTo((nRight - 7) + i, nTop + i);
				dcMem.LineTo((nRight - 7) + i, nTop + 11 - i);
			}
		}

		rectItem = lpDIS->rcItem;
		pDC->BitBlt(rectItem.left, rectItem.top, rectItem.Width(), rectItem.Height(), &dcMem, rectItem.left, rectItem.top, SRCCOPY);

		dcMem.SelectObject(pOldPen);
		dcMem.SelectObject(pOldBmp);
		dcMem.RestoreDC(nSaveDC);
		dcMem.DeleteDC();
		bmp.DeleteObject();

		return;
	}	// if disabled


	// Give the effect of the button "turning" by drawing an intermediate step.
	if (m_bDoEffect)
	{
		// Select the frame pen, since we always draw with that first.
		CPen *pOldPen = (CPen *)dcMem.SelectObject(&m_penDkShadow);

		m_bDoEffect = FALSE;

		dcMem.MoveTo(nRight - 10, nTop + 7);
		dcMem.LineTo(nRight - 3, nTop);
		dcMem.LineTo(nRight - 3, nTop + 7);
		dcMem.LineTo(nRight - 10, nTop + 7);

		dcMem.SelectObject(&m_penShadow);
		dcMem.MoveTo(nRight - 9, nTop + 8);
		dcMem.LineTo(nRight - 2, nTop + 8);
		dcMem.LineTo(nRight - 2, nTop);

		dcMem.SelectObject(&m_penFace);
		dcMem.MoveTo(nRight - 7, nTop + 6);
		dcMem.LineTo(nRight - 4, nTop + 3);
		dcMem.SetPixel(nRight - 5, nTop + 5, m_crFace);

		dcMem.SelectObject(&m_penLiteFace);
		dcMem.MoveTo(nRight - 8, nTop + 6);
		dcMem.LineTo(nRight - 3, nTop + 1);

		dcMem.SelectObject(&m_penLiteShadow);
		dcMem.MoveTo(nRight - 6, nTop + 6);
		dcMem.LineTo(nRight - 4, nTop + 6);
		dcMem.LineTo(nRight - 4, nTop + 2);
		
		rectItem = lpDIS->rcItem;
		pDC->BitBlt(rectItem.left, rectItem.top, rectItem.Width(), rectItem.Height(), &dcMem, rectItem.left, rectItem.top, SRCCOPY);

		dcMem.SelectObject(pOldPen);
		dcMem.SelectObject(pOldBmp);
		dcMem.RestoreDC(nSaveDC);
		dcMem.DeleteDC();
		bmp.DeleteObject();

		Sleep(50);
		RedrawWindow();
		return;
	}	// if drawing effect

	// Darken the colors if the mouse is down.
	COLORREF crSaved = m_crFace;
	if (lpDIS->itemState & ODS_SELECTED)
	{
		m_crFace = DarkenColor(m_crFace, 51);
		GetColors();
		CreatePens();
	}

	// Select the frame pen, since we always draw with that first.
	CPen *pOldPen = (CPen *)dcMem.SelectObject(&m_penDkShadow);

	// If "more" is shown, draw the arrow in the "more" position.
	if (m_bMoreShown)
	{
		nTop += 3;

		dcMem.MoveTo(nRight, nTop);
		dcMem.LineTo(nRight - 10, nTop);
		dcMem.LineTo(nRight - 5, nTop + 5);
		dcMem.LineTo(nRight, nTop);

		dcMem.SelectObject(&m_penShadow);
		dcMem.MoveTo(nRight - 4, nTop + 5);
		dcMem.LineTo(nRight + 1, nTop);

		dcMem.SelectObject(&m_penFace);
		dcMem.MoveTo(nRight - 7, nTop + 2);
		dcMem.LineTo(nRight - 3, nTop + 2);
		dcMem.MoveTo(nRight - 6, nTop + 3);
		dcMem.LineTo(nRight - 4, nTop + 3);
		dcMem.SetPixel(nRight - 2, nTop + 1, m_crFace);

		dcMem.SelectObject(&m_penLiteFace);
		dcMem.MoveTo(nRight - 8, nTop + 1);
		dcMem.LineTo(nRight - 2, nTop + 1);

		dcMem.SelectObject(&m_penLiteShadow);
		dcMem.MoveTo(nRight - 5, nTop + 4);
		dcMem.LineTo(nRight - 2, nTop + 1);
	}
	// Else draw the button in the normal position.
	else
	{
		dcMem.MoveTo(nRight - 7, nTop);
		dcMem.LineTo(nRight - 7, nTop + 10);
		dcMem.LineTo(nRight - 2, nTop + 5);
		dcMem.LineTo(nRight - 7, nTop);

		dcMem.SelectObject(&m_penShadow);
		dcMem.MoveTo(nRight - 6, nTop + 10);
		dcMem.LineTo(nRight - 1, nTop + 5);

		dcMem.SelectObject(&m_penFace);
		dcMem.MoveTo(nRight - 5, nTop + 3);
		dcMem.LineTo(nRight - 5, nTop + 7);
		dcMem.MoveTo(nRight - 4, nTop + 4);
		dcMem.LineTo(nRight - 4, nTop + 6);
		dcMem.SetPixel(nRight - 6, nTop + 8, m_crFace);

		dcMem.SelectObject(&m_penLiteFace);
		dcMem.MoveTo(nRight - 6, nTop + 2);
		dcMem.LineTo(nRight - 6, nTop + 8);

		dcMem.SelectObject(&m_penLiteShadow);
		dcMem.MoveTo(nRight - 5, nTop + 7);
		dcMem.LineTo(nRight - 2, nTop + 4);
	}	// else draw normal button

	rectItem = lpDIS->rcItem;
	pDC->BitBlt(rectItem.left, rectItem.top, rectItem.Width(), rectItem.Height(), &dcMem, rectItem.left, rectItem.top, SRCCOPY);

	// Restore the colors if they were darkened.
	if (lpDIS->itemState & ODS_SELECTED)
	{
		m_crFace = crSaved;
		GetColors();
		CreatePens();
	}

	// Clean up.
	dcMem.SelectObject(pOldPen);
	dcMem.SelectObject(pOldBmp);
	dcMem.RestoreDC(nSaveDC);
	dcMem.DeleteDC();
	bmp.DeleteObject();
}	// DrawItem

//-------------------------------------------------------------------
//
void CMacMoreButton::SetColor(COLORREF crColor)
//
// Return Value:	None.
//
// Parameters	:	crColor - Used to set m_crFace.
//
// Remarks		:	Sets m_crFace and gets the other colors.
//
{
	m_crFace = crColor;
	GetColors();
	CreatePens();
}	// SetColor

//-------------------------------------------------------------------
//
void CMacMoreButton::OnLButtonUp(UINT nFlags, CPoint point) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down. 
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left 
//							corner of the window.
//
// Remarks		:	The framework calls this member function when the user 
//						releases the left mouse button. Checks to see if the mouse
//						was released over the button. If so, check it.
//
{
	CMacButton::OnLButtonUp(nFlags, point);

	CRect rect;
	GetClientRect(rect);
	if (m_bMouseDown && rect.PtInRect(point))
	{
		m_bMoreShown = !m_bMoreShown;
		m_bDoEffect = TRUE;
		RedrawWindow();
	}

	m_bMouseDown = FALSE;
}	// OnLButtonUp

//-------------------------------------------------------------------
//
void CMacMoreButton::OnLButtonDown(UINT nFlags, CPoint point) 
//
// Return Value:	None.
//
// Parameters	:	nFlags - Indicates whether various virtual keys are down. 
//						point - Specifies the x- and y-coordinate of the cursor. 
//							These coordinates are always relative to the upper-left 
//							corner of the window.
//
// Remarks		:	The framework calls this member function when the user 
//						presses the left mouse button. Sets focus to this button,
//						captures the mouse, and redraws the check.
//
{
	m_bMouseDown = TRUE;
	
	CMacButton::OnLButtonDown(nFlags, point);
}	// OnLButtonDown

//-------------------------------------------------------------------
//
BOOL CMacMoreButton::GetMoreShown() const
//
// Return Value:	m_bMoreShown.
//
// Parameters	:	None.
//
// Remarks		:	Returns m_bMoreShown.
//
{
	return m_bMoreShown;
}	// GetMoreShown

//-------------------------------------------------------------------
//
void CMacMoreButton::SetTextAlign(int nAlign)
//
// Return Value:	None.
//
// Parameters	:	nAlign - Specifies the text alignment. Can be one of
//							the following values:
//							ALIGN_TEXT_LEFT	Aligns text to the left of the button.
//							ALIGN_TEXT_RIGHT	Aligns text to the right of the button
//
// Remarks		:	Sets the text alignment.
//
{
	m_nAlignText = nAlign;
	RedrawWindow();
}	// SetTextAlign