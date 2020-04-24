// ColourPicker.cpp : implementation file
//
//
// ColourPicker is a drop-in colour picker control. Check out the 
// header file or the accompanying HTML doc file for details.
//
// Written by Chris Maunder (Chris.Maunder@cbr.clw.csiro.au)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//

#include "stdafx.h"
#include "ColourPopup.h"
#include "ColourPicker.h"
#include "ntlworldcommon.h"
#include "NtlWorldPaletteDefine.h"
#include "../../resource.h"
#include "../../ntlwe.h"
#include "../../ntlwedoc.h"
#include "../../ntlwefrm.h"
#include "../../palettewater.h"
#include "../../PaletteShadow.h"
#include "../../PaletteSoftSpot.h"
#include "../../PaletteDecalDlg.h"
#include "../../PaletteWorldLight.h"
#include "../../PaletteLightObject.h"
#include "../../PaletteLightMode.h"
#include "../../PaletteDecalVolumeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI DDX_ColourPicker(CDataExchange *pDX, int nIDC, COLORREF& crColour)
{
    HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
    ASSERT (hWndCtrl != NULL);                
    
    CColourPicker* pColourPicker = (CColourPicker*) CWnd::FromHandle(hWndCtrl);
    if (pDX->m_bSaveAndValidate)
    {
        crColour = pColourPicker->GetColour();
    }
    else // initializing
    {
        pColourPicker->SetColour(crColour);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CColourPicker

CColourPicker::CColourPicker()
{
    SetBkColour(0x00000000);//GetSysColor(COLOR_3DFACE));
    SetTextColour(GetSysColor(COLOR_BTNTEXT));

    m_bTrackSelection = FALSE;
    m_nSelectionMode = CP_MODE_BK;
	
    m_bActive = FALSE;

	m_bTextColorConvert =TRUE;	
}

CColourPicker::~CColourPicker()
{
}

IMPLEMENT_DYNCREATE(CColourPicker, CButton)

BEGIN_MESSAGE_MAP(CColourPicker, CButton)
    //{{AFX_MSG_MAP(CColourPicker)
    ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
    ON_MESSAGE(CPN_SELENDOK,     OnSelEndOK)
    ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
    ON_MESSAGE(CPN_SELCHANGE,    OnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColourPicker message handlers

LONG CColourPicker::OnSelEndOK(UINT lParam, LONG wParam)
{
    COLORREF crNewColour = (COLORREF) lParam;
    m_bActive = FALSE;
    SetColour(crNewColour);

    CWnd *pParent = GetParent();
    if (pParent) {
        pParent->SendMessage(CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
        pParent->SendMessage(CPN_SELENDOK, lParam, (WPARAM) GetDlgCtrlID());
    }

    if (crNewColour != GetColour())
        if (pParent) pParent->SendMessage(CPN_SELCHANGE, lParam, (WPARAM) GetDlgCtrlID());

	DWORD blue	= (crNewColour & 0x00ff0000) >> 16;
	DWORD green = (crNewColour & 0x0000ff00) >> 8;
	DWORD red	= (crNewColour & 0x000000ff) >> 0;

	if(GetDlgCtrlID() == IDC_DIFFUSE_PICKER)
	{
		dGET_CUR_PALETTE() = ePM_DIFFUSE;

		dGET_WORLD_PARAM()->ClrDiffuserPicker = (red << 16) | (green << 8) | (blue);

		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEApp*)(AfxGetApp()))->SetDiffusePickerClr(R, G, B);
	}
	else if(GetDlgCtrlID() == IDC_FOG_COLOR_PICKER)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEDoc*)(((CNtlWEApp*)(AfxGetApp()))->m_pDoc))->m_FogPropDlg.SetColor(red, green, blue);
	}
	else if(GetDlgCtrlID() == IDC_SKY_RGB0_PICKER)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEDoc*)(((CNtlWEApp*)(AfxGetApp()))->m_pDoc))->m_SkyPropDlg.SetColor0(red, green, blue);
	}
	else if(GetDlgCtrlID() == IDC_SKY_RGB1_PICKER)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEDoc*)(((CNtlWEApp*)(AfxGetApp()))->m_pDoc))->m_SkyPropDlg.SetColor1(red, green, blue);
	}
	else if(GetDlgCtrlID() == IDC_SKY_RGB2_PICKER)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEDoc*)(((CNtlWEApp*)(AfxGetApp()))->m_pDoc))->m_SkyPropDlg.SetColor2(red, green, blue);
	}
	else if(GetDlgCtrlID() == IDC_VERT_DIFFUSE)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEFrm*)(((CNtlWEApp*)(AfxGetApp()))->m_pFrm))->m_pWaterPalette->SetColor(red, green, blue);
	}
// 	else if(GetDlgCtrlID() == IDC_AMBIENT_COLOR_PICKER)
// 	{
// 		CString R, G, B;
// 		R.Format("%d", red);
// 		G.Format("%d", green);
// 		B.Format("%d", blue);
// 
// 		((CNtlWEDoc*)(((CNtlWEApp*)(AfxGetApp()))->m_pDoc))->m_LightPropDlg.SetColorAmbient(red, green, blue);
// 	}
// 	else if(GetDlgCtrlID() == IDC_DIRECTIONAL_COLOR_PICKER)
// 	{
// 		CString R, G, B;
// 		R.Format("%d", red);
// 		G.Format("%d", green);
// 		B.Format("%d", blue);
// 
// 		((CNtlWEDoc*)(((CNtlWEApp*)(AfxGetApp()))->m_pDoc))->m_LightPropDlg.SetColorDirectional(red, green, blue);
// 	}
	else if(GetDlgCtrlID() == IDC_COLOR_PICKER_AMBIENT)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		RwRGBA rgba;
		rgba.alpha	= 255;
		rgba.red	= static_cast<RwUInt8>(red);
		rgba.green	= static_cast<RwUInt8>(green);
		rgba.blue	= static_cast<RwUInt8>(blue);
		RwRGBAReal rgbaf;
		RwRGBARealFromRwRGBA(&rgbaf, &rgba);
		((CNtlWEDoc*)(((CNtlWEApp*)(AfxGetApp()))->m_pDoc))->m_LightPropDlg.SetAmbient(rgbaf);
	}
	else if(GetDlgCtrlID() == IDC_COLOR_PICKER_DIRECTIONAL1)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		RwRGBA rgba;
		rgba.alpha	= 255;
		rgba.red	= static_cast<RwUInt8>(red);
		rgba.green	= static_cast<RwUInt8>(green);
		rgba.blue	= static_cast<RwUInt8>(blue);
		RwRGBAReal rgbaf;
		RwRGBARealFromRwRGBA(&rgbaf, &rgba);
		((CNtlWEDoc*)(((CNtlWEApp*)(AfxGetApp()))->m_pDoc))->m_LightPropDlg.SetDirectional(0, rgbaf);
	}
	else if(GetDlgCtrlID() == IDC_COLOR_PICKER_DIRECTIONAL2)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		RwRGBA rgba;
		rgba.alpha	= 255;
		rgba.red	= static_cast<RwUInt8>(red);
		rgba.green	= static_cast<RwUInt8>(green);
		rgba.blue	= static_cast<RwUInt8>(blue);
		RwRGBAReal rgbaf;
		RwRGBARealFromRwRGBA(&rgbaf, &rgba);
		((CNtlWEDoc*)(((CNtlWEApp*)(AfxGetApp()))->m_pDoc))->m_LightPropDlg.SetDirectional(1, rgbaf);
	}
	else if(GetDlgCtrlID() == IDC_COLOR_PICKER_DIRECTIONAL3)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		RwRGBA rgba;
		rgba.alpha	= 255;
		rgba.red	= static_cast<RwUInt8>(red);
		rgba.green	= static_cast<RwUInt8>(green);
		rgba.blue	= static_cast<RwUInt8>(blue);
		RwRGBAReal rgbaf;
		RwRGBARealFromRwRGBA(&rgbaf, &rgba);
		((CNtlWEDoc*)(((CNtlWEApp*)(AfxGetApp()))->m_pDoc))->m_LightPropDlg.SetDirectional(2, rgbaf);
	}
	else if(GetDlgCtrlID() == IDC_SL_COLOR_PICKER)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEDoc*)(((CNtlWEApp*)(AfxGetApp()))->m_pDoc))->m_SLPropDlg.SetColor(red, green, blue);
	}
	else if(GetDlgCtrlID() == IDC_PS_PICKER)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEFrm*)(((CNtlWEApp*)(AfxGetApp()))->m_pFrm))->m_pShadowPalette->SetColor(red, green, blue);
	}
	// CZ-SS
// 	else if(GetDlgCtrlID() == IDC_SOFT_PICKER)
// 	{
// 		CString R, G, B;
// 		R.Format("%d", red);
// 		G.Format("%d", green);
// 		B.Format("%d", blue);
// 
// 		((CNtlWEFrm*)(((CNtlWEApp*)(AfxGetApp()))->m_pFrm))->m_pSoftSpotPalette->SetColor(red, green, blue);
// 	}
	else if(GetDlgCtrlID() == IDC_DECAL_COLOR_PICKER)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEFrm*)(((CNtlWEApp*)(AfxGetApp()))->m_pFrm))->m_pDecalPalette->SetColor(red, green, blue);
	}
	else if (GetDlgCtrlID() == IDC_WORLDLIGHT_PICKER)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEFrm*)(((CNtlWEApp*)(AfxGetApp()))->m_pFrm))->m_pWorldLightPalette->SetColor(red, green, blue);
	}
	else if(GetDlgCtrlID() == IDC_FIELDCOLOR_PICKER)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEDoc*)(((CNtlWEApp*)(AfxGetApp()))->m_pDoc))->m_FieldColorPropDlg.SetColor(red, green, blue);
	}
	else if (GetDlgCtrlID() == IDC_LIGHTOBJECT_PICKER)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEFrm*)(((CNtlWEApp*)(AfxGetApp()))->m_pFrm))->m_pLightObjectPalette->SetColor(red, green, blue);
	}
	else if (GetDlgCtrlID() == IDC_LIGHTMODE_PICKER)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEFrm*)(((CNtlWEApp*)(AfxGetApp()))->m_pFrm))->m_pLightModePalette->SetColor(red, green, blue);
	}
	else if (GetDlgCtrlID() == IDC_PICKER_DECAL_VOLUME)
	{
		CString R, G, B;
		R.Format("%d", red);
		G.Format("%d", green);
		B.Format("%d", blue);

		((CNtlWEFrm*)(((CNtlWEApp*)(AfxGetApp()))->m_pFrm))->m_pDecalVolumePalette->SetColor(red, green, blue);
	}
    return TRUE;
}

LONG CColourPicker::OnSelEndCancel(UINT lParam, LONG wParam)
{
    m_bActive = FALSE;
    SetColour((COLORREF) lParam);

    CWnd *pParent = GetParent();
    if (pParent) {
        pParent->SendMessage(CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
        pParent->SendMessage(CPN_SELENDCANCEL, lParam, (WPARAM) GetDlgCtrlID());
    }

    return TRUE;
}

LONG CColourPicker::OnSelChange(UINT lParam, LONG wParam)
{
    if (m_bTrackSelection) SetColour((COLORREF) lParam);

    CWnd *pParent = GetParent();
    if (pParent) pParent->SendMessage(CPN_SELCHANGE, lParam, (WPARAM) GetDlgCtrlID());

    return TRUE;
}

int CColourPicker::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CButton::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    SetWindowSize();    // resize appropriately
    return 0;
}

// On mouse click, create and show a CColourPopup window for colour selection
BOOL CColourPicker::OnClicked()
{
	if(GetDlgCtrlID() == IDC_DIFFUSE_PICKER)
	{
		dGET_CUR_PALETTE() = ePM_DIFFUSE;
	}

    m_bActive = TRUE;
    CRect rect;
    GetWindowRect(rect);
    new CColourPopup(CPoint(rect.left, rect.bottom), GetColour(), this, 10, 
                     _T("다른 색..."));

    CWnd *pParent = GetParent();
    if (pParent)
        pParent->SendMessage(CPN_DROPDOWN, (LPARAM)GetColour(), (WPARAM) GetDlgCtrlID());

    // Docs say I should return FALSE to stop the parent also getting the message.
    // HA! What a joke.

    return TRUE;
}

void CColourPicker::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    ASSERT(lpDrawItemStruct);
    
    CDC*    pDC     = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect   rect    = lpDrawItemStruct->rcItem;
    UINT    state   = lpDrawItemStruct->itemState;
    DWORD   dwStyle = GetStyle();
    CString m_strText;

    CSize Margins(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));

    // Draw arrow
    if (m_bActive) state |= ODS_SELECTED;
    pDC->DrawFrameControl(&m_ArrowRect, DFC_SCROLL, DFCS_SCROLLDOWN  | 
                          ((state & ODS_SELECTED) ? DFCS_PUSHED : 0) |
                          ((state & ODS_DISABLED) ? DFCS_INACTIVE : 0));

    pDC->DrawEdge(rect, EDGE_SUNKEN, BF_RECT);

    // Must reduce the size of the "client" area of the button due to edge thickness.
    rect.DeflateRect(Margins.cx, Margins.cy);

    // Fill remaining area with colour
    rect.right -= m_ArrowRect.Width();

    CBrush brush((state & ODS_DISABLED)? ::GetSysColor(COLOR_3DFACE) : m_crColourBk);
	CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brush);
	pDC->SelectStockObject(NULL_PEN);
    pDC->Rectangle(rect);
    pDC->SelectObject(pOldBrush);

    // Draw the window text (if any)
	GetWindowText(m_strText);
	
    if (m_strText.GetLength())
    {
        pDC->SetBkMode(TRANSPARENT);
        if (state & ODS_DISABLED)
        {
            rect.OffsetRect(1,1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
            pDC->DrawText(m_strText, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
            rect.OffsetRect(-1,-1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
            pDC->DrawText(m_strText, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
        }
        else
        {
            pDC->SetTextColor(m_crColourText);
            pDC->DrawText(m_strText, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
        }
    }

    // Draw focus rect
    if (state & ODS_FOCUS) 
    {
        rect.DeflateRect(1,1);
        pDC->DrawFocusRect(rect);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CColourPicker overrides

void CColourPicker::PreSubclassWindow() 
{
    ModifyStyle(0, BS_OWNERDRAW);        // Make it owner drawn
    CButton::PreSubclassWindow();
    SetWindowSize();                     // resize appropriately
}

/////////////////////////////////////////////////////////////////////////////
// CColourPicker attributes

void CColourPicker::SetBkColour(COLORREF crColourBk)
{
    m_crColourBk = crColourBk;
	if(m_bTextColorConvert)	// 텍스트의 색깔은 배경과 반대로 한다. 
		SetTextColour(COLORREF(RGB(255,255,255))-m_crColourBk);
    if (IsWindow(m_hWnd)) RedrawWindow();
}


void CColourPicker::SetTextColour(COLORREF crColourText)
{
    m_crColourText = crColourText;
    if (IsWindow(m_hWnd)) RedrawWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CColourPicker implementation

void CColourPicker::SetWindowSize()
{
    // Get size dimensions of edges
    CSize MarginSize(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));

    // Get size of dropdown arrow
    int nArrowWidth = max(::GetSystemMetrics(SM_CXHTHUMB), 5*MarginSize.cx);
    int nArrowHeight = max(::GetSystemMetrics(SM_CYVTHUMB), 5*MarginSize.cy);
    CSize ArrowSize(max(nArrowWidth, nArrowHeight), max(nArrowWidth, nArrowHeight));

    //ArrowSize = CSize(40,40); // for testing

    // Get window size
    CRect rect;
    GetWindowRect(rect);

    CWnd* pParent = GetParent();
    if (pParent)
        pParent->ScreenToClient(rect);

    // Set window size at least as wide as 2 arrows, and as high as arrow + margins
    int nWidth = max(rect.Width(), 2*ArrowSize.cx + 2*MarginSize.cx);
    MoveWindow(rect.left, rect.top, nWidth, ArrowSize.cy+2*MarginSize.cy, TRUE);

    // Get the new coords of this window
    GetWindowRect(rect);
    ScreenToClient(rect);

    // Get the rect where the arrow goes, and convert to client coords.
    m_ArrowRect.SetRect(rect.right - ArrowSize.cx - MarginSize.cx, 
                        rect.top + MarginSize.cy, rect.right - MarginSize.cx,
                        rect.bottom - MarginSize.cy);
}
