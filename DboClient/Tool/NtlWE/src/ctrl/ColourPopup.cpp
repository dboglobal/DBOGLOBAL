// ColourPopup.cpp : implementation file
//
// Written by Chris Maunder (Chris.Maunder@cbr.clw.csiro.au)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// ColourPopup is a helper class for the colour picker control
// CColourPicker. Check out the header file or the accompanying 
// HTML doc file for details.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 

#include "stdafx.h"
#include "afxdlgs.h"
#include <math.h>
#include "ColourPicker.h"
#include "ColourPopup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TEXT_BOX_VALUE -2        // Sorry about this hack...
#define MAX_COLOURS    100


ColourTableEntry CColourPopup::m_crColours[] = 
{
    { RGB(0x00, 0x00, 0x00),    _T("Black")             },
    { RGB(0xA5, 0x2A, 0x00),    _T("Brown")             },
    { RGB(0x00, 0x40, 0x40),    _T("Dark Olive Green")  },
    { RGB(0x00, 0x55, 0x00),    _T("Dark Green")        },
    { RGB(0x00, 0x00, 0x5E),    _T("Dark Teal")         },
    { RGB(0x00, 0x00, 0x8B),    _T("Dark blue")         },
    { RGB(0x4B, 0x00, 0x82),    _T("Indigo")            },
    { RGB(0x28, 0x28, 0x28),    _T("Dark grey")         },

    { RGB(0x8B, 0x00, 0x00),    _T("Dark red")          },
    { RGB(0xFF, 0x68, 0x20),    _T("Orange")            },
    { RGB(0x8B, 0x8B, 0x00),    _T("Dark yellow")       },
    { RGB(0x00, 0x93, 0x00),    _T("Green")             },
    { RGB(0x38, 0x8E, 0x8E),    _T("Teal")              },
    { RGB(0x00, 0x00, 0xFF),    _T("Blue")              },
    { RGB(0x7B, 0x7B, 0xC0),    _T("Blue-grey")         },
    { RGB(0x66, 0x66, 0x66),    _T("Grey - 40")         },

    { RGB(0xFF, 0x00, 0x00),    _T("Red")               },
    { RGB(0xFF, 0xAD, 0x5B),    _T("Light orange")      },
    { RGB(0x32, 0xCD, 0x32),    _T("Lime")              }, 
    { RGB(0x3C, 0xB3, 0x71),    _T("Sea green")         },
    { RGB(0x7F, 0xFF, 0xD4),    _T("Aqua")              },
    { RGB(0x7D, 0x9E, 0xC0),    _T("Light blue")        },
    { RGB(0x80, 0x00, 0x80),    _T("Violet")            },
    { RGB(0x7F, 0x7F, 0x7F),    _T("Grey - 50")         },

    { RGB(0xFF, 0xC0, 0xCB),    _T("Pink")              },
    { RGB(0xFF, 0xD7, 0x00),    _T("Gold")              },
    { RGB(0xFF, 0xFF, 0x00),    _T("Yellow")            },    
    { RGB(0x00, 0xFF, 0x00),    _T("Bright green")      },
    { RGB(0x40, 0xE0, 0xD0),    _T("Turquoise")         },
    { RGB(0xC0, 0xFF, 0xFF),    _T("Skyblue")           },
    { RGB(0x48, 0x00, 0x48),    _T("Plum")              },
    { RGB(0xC0, 0xC0, 0xC0),    _T("Light grey")        },

    { RGB(0xFF, 0xE4, 0xE1),    _T("Rose")              },
    { RGB(0xD2, 0xB4, 0x8C),    _T("Tan")               },
    { RGB(0xFF, 0xFF, 0xE0),    _T("Light yellow")      },
    { RGB(0x98, 0xFB, 0x98),    _T("Pale green ")       },
    { RGB(0xAF, 0xEE, 0xEE),    _T("Pale turquoise")    },
    { RGB(0x68, 0x83, 0x8B),    _T("Pale blue")         },
    { RGB(0xE6, 0xE6, 0xFA),    _T("Lavender")          },
    { RGB(0xFF, 0xFF, 0xFF),    _T("White")             }
};

/////////////////////////////////////////////////////////////////////////////
// CColourPopup

CColourPopup::CColourPopup()
{
    Initialise();
}

CColourPopup::CColourPopup(CPoint p, COLORREF crColour, CWnd* pParentWnd, UINT nID, 
                           LPCTSTR szCustomText /* = NULL */)
{
    Initialise();

    m_crColour = m_crInitialColour = crColour;
    if (szCustomText != NULL)
    {
        m_bShowCustom   = TRUE;
        m_strCustomText = szCustomText;
    } else
        m_bShowCustom = FALSE;
    m_pParent           = pParentWnd;

    CColourPopup::Create(p, crColour, pParentWnd, nID, szCustomText);
}

void CColourPopup::Initialise()
{
    m_nNumColours       = sizeof(m_crColours)/sizeof(ColourTableEntry);
    ASSERT(m_nNumColours <= MAX_COLOURS);
    if (m_nNumColours > MAX_COLOURS)
        m_nNumColours = MAX_COLOURS;

    m_nNumColumns       = 0;
    m_nNumRows          = 0;
    m_nBoxSize          = 18;
    m_nMargin           = ::GetSystemMetrics(SM_CXEDGE);
    m_nCurrentRow       = -1;
    m_nCurrentCol       = -1;
    m_nChosenColourRow  = -1;
    m_nChosenColourCol  = -1;
    m_strCustomText     = _T("´Ù¸¥ »ö...");
    m_bShowCustom       = TRUE;
    m_pParent           = NULL;
    m_crColour          = m_crInitialColour = RGB(0,0,0);

    // Idiot check: Make sure the colour square is at least 5 x 5;
    if (m_nBoxSize - 2*m_nMargin - 2 < 5) m_nBoxSize = 5 + 2*m_nMargin + 2;

    // Create the font
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
    m_Font.CreateFontIndirect(&(ncm.lfMessageFont));

    // Create the palette
    struct {
        LOGPALETTE    LogPalette;
        PALETTEENTRY  PalEntry[MAX_COLOURS];
    } pal;

    LOGPALETTE* pLogPalette = (LOGPALETTE*) &pal;
    pLogPalette->palVersion    = 0x300;
    pLogPalette->palNumEntries = m_nNumColours; 

    for (int i = 0; i < m_nNumColours; i++)
    {
        pLogPalette->palPalEntry[i].peRed   = GetRValue(m_crColours[i].crColour);
        pLogPalette->palPalEntry[i].peGreen = GetGValue(m_crColours[i].crColour);
        pLogPalette->palPalEntry[i].peBlue  = GetBValue(m_crColours[i].crColour);
        pLogPalette->palPalEntry[i].peFlags = 0;
    }

    m_Palette.CreatePalette(pLogPalette);
}

CColourPopup::~CColourPopup()
{
    m_Font.DeleteObject();
    m_Palette.DeleteObject();
}

BOOL CColourPopup::Create(CPoint p, COLORREF crColour, CWnd* pParentWnd, UINT nID, 
                          LPCTSTR szCustomText /* = NULL */)
{
    ASSERT(pParentWnd && ::IsWindow(pParentWnd->GetSafeHwnd()));
    ASSERT(pParentWnd->IsKindOf(RUNTIME_CLASS(CColourPicker)));

    m_pParent  = pParentWnd;
    m_crColour = m_crInitialColour = crColour;

    // Get the class name and create the window
    CString szClassName = AfxRegisterWndClass(CS_CLASSDC|CS_SAVEBITS|CS_HREDRAW|CS_VREDRAW,
                                              0, (HBRUSH)GetStockObject(LTGRAY_BRUSH),0);

    if (!CWnd::CreateEx(0, szClassName, _T(""), WS_VISIBLE|WS_POPUP, 
                        p.x, p.y, 100, 100, // size updated soon
                        pParentWnd->GetSafeHwnd(), 0, NULL))
        return FALSE;

    // Store the Custom text
    if (szCustomText != NULL) 
        m_strCustomText = szCustomText;

    // Set the window size
    SetWindowSize();

    // Create the tooltips
    CreateToolTips();

    // Find which cell (if any) corresponds to the initial colour
    FindCellFromColour(crColour);

    // Capture all mouse events for the life of this window
    SetCapture();

    return TRUE;
}

BEGIN_MESSAGE_MAP(CColourPopup, CWnd)
    //{{AFX_MSG_MAP(CColourPopup)
    ON_WM_NCDESTROY()
    ON_WM_LBUTTONUP()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_KEYDOWN()
    ON_WM_QUERYNEWPALETTE()
    ON_WM_PALETTECHANGED()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColourPopup message handlers

// For tooltips
BOOL CColourPopup::PreTranslateMessage(MSG* pMsg) 
{
    m_ToolTip.RelayEvent(pMsg);
    return CWnd::PreTranslateMessage(pMsg);
}

// If an arrow key is pressed, then move the selection
void CColourPopup::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    int row = m_nCurrentRow,
        col = m_nCurrentCol;

    if (nChar == VK_DOWN) 
    {
        if (row < 0) { row = 0; col = 0; }
        else if (row < m_nNumRows-1) row++;
        else { row = TEXT_BOX_VALUE; col = TEXT_BOX_VALUE; }
        ChangeSelection(row, col);
    }

      if (nChar == VK_UP) 
    {
        if (row < 0) { row = m_nNumRows-1; col = 0; }
        else if (row > 0) row--;
        else { row = TEXT_BOX_VALUE; col = TEXT_BOX_VALUE; }
        ChangeSelection(row, col);
    }

    if (nChar == VK_RIGHT) 
    {
        if (col < 0) { row = 0; col = 0; }
        else if (col < m_nNumColumns-1) col++;
        else col = 0;
        ChangeSelection(row, col);
    }

    if (nChar == VK_LEFT) 
    {
        if (col < 0) { row = m_nNumRows-1; col = m_nNumColumns-1; }
        else if (col > 0) col--;
        else col = m_nNumColumns-1;
        ChangeSelection(row, col);
    }

    if (nChar == VK_ESCAPE) 
    {
        m_crColour = m_crInitialColour;
        EndSelection(CPN_SELENDCANCEL);
        return;
    }

    if (nChar == VK_RETURN)
    {
        EndSelection(CPN_SELENDOK);
        return;
    }

    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

// auto-deletion
void CColourPopup::OnNcDestroy() 
{
    CWnd::OnNcDestroy();
    delete this;
}

void CColourPopup::OnPaint() 
{
    CPaintDC dc(this); // device context for painting

    // Draw colour cells
    for (int row = 0; row < m_nNumRows; row++)
        for (int col = 0; col < m_nNumColumns; col++)
            DrawCell(&dc, row, col);
    
    // Draw custom text
    if (m_bShowCustom)
        DrawCell(&dc, TEXT_BOX_VALUE, TEXT_BOX_VALUE);

    // Draw raised window edge (ex-window style WS_EX_WINDOWEDGE is sposed to do this,
    // but for some reason isn't
    CRect rect;
    GetClientRect(rect);
    dc.DrawEdge(rect, EDGE_RAISED, BF_RECT);
}

void CColourPopup::OnMouseMove(UINT nFlags, CPoint point) 
{
    int row, col;

    // Translate points to be relative raised window edge
    point.x -= m_nMargin;
    point.y -= m_nMargin;

    // First check we aren't in text box
    if (m_bShowCustom && m_TextRect.PtInRect(point))
        row = col = TEXT_BOX_VALUE;        // Special value meaning Text Box (hack!)
    else
    {
        // Take into account text box
        if (m_bShowCustom) 
            point.y -= m_TextRect.Height();  

        // Get the row and column
        row = point.y / m_nBoxSize,
        col = point.x / m_nBoxSize;

        // In range? If not, default and exit
        if (row < 0 || row >= m_nNumRows ||
            col < 0 || col >= m_nNumColumns)
        {
            CWnd::OnMouseMove(nFlags, point);
            return;
        }
    }

    // OK - we have the row and column of the current selection (may be TEXT_BOX_VALUE)
    // Has the row/col selection changed? If yes, then redraw old and new cells.
    if (row != m_nCurrentRow || col != m_nCurrentCol)
    {
        ChangeSelection(row, col);
    }

    CWnd::OnMouseMove(nFlags, point);
}

// End selection on LButtonUp
void CColourPopup::OnLButtonUp(UINT nFlags, CPoint point) 
{    
    CWnd::OnLButtonUp(nFlags, point);

    DWORD pos = GetMessagePos();
    point = CPoint(LOWORD(pos), HIWORD(pos));

    if (m_WindowRect.PtInRect(point))
        EndSelection(CPN_SELENDOK);
    else
        EndSelection(CPN_SELENDCANCEL);
}

/////////////////////////////////////////////////////////////////////////////
// CColourPopup implementation

void CColourPopup::FindCellFromColour(COLORREF crColour)
{
    for (int row = 0; row < m_nNumRows; row++)
        for (int col = 0; col < m_nNumColumns; col++)
        {
            if (GetColour(row, col) == crColour)
            {
                m_nChosenColourRow  = row;
                m_nChosenColourCol  = col;
                return;
            }
        }

    m_nChosenColourRow  = TEXT_BOX_VALUE;
    m_nChosenColourCol  = TEXT_BOX_VALUE;
}

// Gets the dimensions of the colour cell given by (row,col)
BOOL CColourPopup::GetCellRect(int row, int col, const LPRECT& rect)
{
    if (row < 0 || row >= m_nNumRows || col < 0 || col >= m_nNumColumns)
        return FALSE;

    rect->left = col*m_nBoxSize + m_nMargin;
    rect->top  = row*m_nBoxSize + m_nMargin;

    // Move everything down if we are displaying text
    if (m_bShowCustom) 
        rect->top += (m_nMargin + m_TextRect.Height());

    rect->right = rect->left + m_nBoxSize;
    rect->bottom = rect->top + m_nBoxSize;

    return TRUE;
}

// Works out an appropriate size and position of this window
void CColourPopup::SetWindowSize()
{
    CSize TextSize;

    // If we are showing a custom text area, get the font and text size.
    if (m_bShowCustom)
    {
        // Get the size of the custom text
        CClientDC dc(this);
        CFont* pOldFont = (CFont*) dc.SelectObject(&m_Font);
        TextSize = dc.GetTextExtent(m_strCustomText) + CSize(2*m_nMargin,2*m_nMargin);
        dc.SelectObject(pOldFont);

        // Add even more space to draw the horizontal line
        TextSize.cy += 2*m_nMargin + 2;
    }

    // Get the number of columns and rows
    //m_nNumColumns = (int) sqrt((double)m_nNumColours);    // for a square window (yuk)
    m_nNumColumns = 8;
    m_nNumRows = m_nNumColours / m_nNumColumns;
    if (m_nNumColours % m_nNumColumns) m_nNumRows++;

    // Get the current window position, and set the new size
    CRect rect;
    GetWindowRect(rect);

    m_WindowRect.SetRect(rect.left, rect.top, 
                         rect.left + m_nNumColumns*m_nBoxSize + 2*m_nMargin,
                         rect.top  + m_nNumRows*m_nBoxSize + 2*m_nMargin);

    // if custom text, then expand window if necessary, and set text width as
    // window width
    if (m_bShowCustom) 
    {
        m_WindowRect.bottom += (m_nMargin + TextSize.cy);
        if (TextSize.cx > m_WindowRect.Width())
            m_WindowRect.right = m_WindowRect.left + TextSize.cx;
        TextSize.cx = m_WindowRect.Width()-2*m_nMargin;

        // Work out the text area
        m_TextRect.SetRect(m_nMargin, m_nMargin, 
                           m_nMargin+TextSize.cx, m_nMargin+TextSize.cy);
    }

    // Need to check it'll fit on screen: Too far right?
    CSize ScreenSize(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
    if (m_WindowRect.right > ScreenSize.cx)
        m_WindowRect.OffsetRect(-(m_WindowRect.right - ScreenSize.cx), 0);

    // Too far left?
    if (m_WindowRect.left < 0)
        m_WindowRect.OffsetRect( -m_WindowRect.left, 0);

    // Bottom falling out of screen?
    if (m_WindowRect.bottom > ScreenSize.cy)
    {
        CRect ParentRect;
        m_pParent->GetWindowRect(ParentRect);
        m_WindowRect.OffsetRect(0, -(ParentRect.Height() + m_WindowRect.Height()));
    }

    // Set the window size and position
    MoveWindow(m_WindowRect, TRUE);
}

void CColourPopup::CreateToolTips()
{
    // Create the tool tip
    if (!m_ToolTip.Create(this)) return;

    // Add a tool for each cell
    for (int row = 0; row < m_nNumRows; row++)
        for (int col = 0; col < m_nNumColumns; col++)
        {
            CRect rect;
            if (!GetCellRect(row, col, rect)) continue;
            m_ToolTip.AddTool(this, GetColourName(row, col), rect, 1);
        }
}

void CColourPopup::ChangeSelection(int row, int col)
{
    CClientDC dc(this);        // device context for drawing

    if ((m_nCurrentRow >= 0 && m_nCurrentRow < m_nNumRows &&
         m_nCurrentCol >= 0 && m_nCurrentCol < m_nNumColumns) ||
        (m_nCurrentCol == TEXT_BOX_VALUE && m_nCurrentCol == TEXT_BOX_VALUE))
    {
        // Set Current selection as invalid and redraw old selection (this way
        // the old selection will be drawn unselected)
        int OldRow = m_nCurrentRow, OldCol = m_nCurrentCol;
        m_nCurrentRow = m_nCurrentCol = -1;
        DrawCell(&dc, OldRow, OldCol);
    }

    // Set the current selection as row/col and draw (it will be drawn selected)
    m_nCurrentRow = row; m_nCurrentCol = col;
    DrawCell(&dc, m_nCurrentRow, m_nCurrentCol);

    // Store the current colour
    if (m_nCurrentRow == TEXT_BOX_VALUE && m_nCurrentCol == TEXT_BOX_VALUE)
        m_pParent->SendMessage(CPN_SELCHANGE, (WPARAM) m_crInitialColour, 0);
    else
    {
        m_crColour = GetColour(m_nCurrentRow, m_nCurrentCol);
        m_pParent->SendMessage(CPN_SELCHANGE, (WPARAM) m_crColour, 0);
    }
}

void CColourPopup::EndSelection(int nMessage)
{
    ReleaseCapture();

    // If custom text selected, perform a custom colour selection
    if (nMessage != CPN_SELENDCANCEL && 
        m_nCurrentCol == TEXT_BOX_VALUE && m_nCurrentRow == TEXT_BOX_VALUE)
    {
        CColorDialog dlg(m_crInitialColour, CC_FULLOPEN | CC_ANYCOLOR, this);

        if (dlg.DoModal() == IDOK)
            m_crColour = dlg.GetColor();
        else
            m_crColour = m_crInitialColour;
   } 

    if (nMessage == CPN_SELENDCANCEL)
        m_crColour = m_crInitialColour;

    m_pParent->SendMessage(nMessage, (WPARAM) m_crColour, 0);
    
    DestroyWindow();
}

void CColourPopup::DrawCell(CDC* pDC, int row, int col)
{
    // This is a special hack for the text box
    if (m_bShowCustom && row == TEXT_BOX_VALUE && row == TEXT_BOX_VALUE)
    {
        // The extent of the actual text button
         CRect TextButtonRect = m_TextRect;
        TextButtonRect.bottom -= (2*m_nMargin+2);

        // Fill background
        //if ( (m_nChosenColourRow == row && m_nChosenColourCol == col)
        //    && !(m_nCurrentRow == row && m_nCurrentCol == col) )
        //    pDC->FillSolidRect(m_TextRect, ::GetSysColor(COLOR_3DHILIGHT));
        //else
            pDC->FillSolidRect(m_TextRect, ::GetSysColor(COLOR_3DFACE));

        // Draw button
        //if (m_nChosenColourRow == row && m_nChosenColourCol == col)
        //    pDC->DrawEdge(TextButtonRect, EDGE_SUNKEN, BF_RECT);
        //else 
        if (m_nCurrentRow == row && m_nCurrentCol == col) 
            pDC->DrawEdge(TextButtonRect, EDGE_RAISED, BF_RECT);


        // Draw custom text
        CFont *pOldFont = (CFont*) pDC->SelectObject(&m_Font);
        pDC->DrawText(m_strCustomText, TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);

        // Draw horizontal line
        pDC->FillSolidRect(m_TextRect.left+2*m_nMargin, m_TextRect.bottom-m_nMargin-2,
                           m_TextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DSHADOW));
        pDC->FillSolidRect(m_TextRect.left+2*m_nMargin, m_TextRect.bottom-m_nMargin-1,
                           m_TextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DHILIGHT));

        return;
    }        

    // row/col in range?
    ASSERT(row >= 0 && row < m_nNumRows);
    ASSERT(col >= 0 && col < m_nNumColumns);

    // Select and realize the palette
    CPalette* pOldPalette;
    if (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
    {
        pOldPalette = pDC->SelectPalette(&m_Palette, FALSE);
        pDC->RealizePalette();
    }

    CRect rect;
    if (!GetCellRect(row, col, rect)) return;

    // fill background
    if ( (m_nChosenColourRow == row && m_nChosenColourCol == col)
        && !(m_nCurrentRow == row && m_nCurrentCol == col) )
        pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DHILIGHT));
    else
        pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));

    // Draw button
    if (m_nChosenColourRow == row && m_nChosenColourCol == col)
        pDC->DrawEdge(rect, EDGE_SUNKEN, BF_RECT);
    else if (m_nCurrentRow == row && m_nCurrentCol == col) 
        pDC->DrawEdge(rect, EDGE_RAISED, BF_RECT);

    // Draw raised edge if selected
    if (m_nCurrentRow == row && m_nCurrentCol == col)
        pDC->DrawEdge(rect, EDGE_RAISED, BF_RECT);

    CBrush brush(PALETTERGB(GetRValue(GetColour(row, col)), 
                            GetGValue(GetColour(row, col)), 
                            GetBValue(GetColour(row, col)) ));
    CPen   pen;
    pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

    CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brush);
    CPen*   pOldPen   = (CPen*)   pDC->SelectObject(&pen);

    // Draw the cell colour
    rect.DeflateRect(m_nMargin+1, m_nMargin+1);
    pDC->Rectangle(rect);

    // restore DC and cleanup
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
    brush.DeleteObject();
    pen.DeleteObject();

    if (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
        pDC->SelectPalette(pOldPalette, FALSE);
}

BOOL CColourPopup::OnQueryNewPalette() 
{
    Invalidate();    
    return CWnd::OnQueryNewPalette();
}

void CColourPopup::OnPaletteChanged(CWnd* pFocusWnd) 
{
    CWnd::OnPaletteChanged(pFocusWnd);

    if (pFocusWnd->GetSafeHwnd() != GetSafeHwnd())
        Invalidate();
}
