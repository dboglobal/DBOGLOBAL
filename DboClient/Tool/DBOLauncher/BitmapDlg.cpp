// BitmapDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "Launcher.h"
#include "BitmapDlg.h"
#include "Dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
//  these variables should have been defined in some standard header but is not
#define WS_EX_LAYERED 0x00080000 
#define LWA_COLORKEY 1 // Use color as the transparency color.
#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer


//  ===========================================================================
//  Function pointer for lyering API in User32.dll
//  ===========================================================================
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
            (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes;

/////////////////////////////////////////////////////////////////////////////
// CBitmapDlg dialog


CBitmapDlg::CBitmapDlg(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CBitmapDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	Init();
}

CBitmapDlg::CBitmapDlg(UINT uResourceID, CWnd* pParent /*= NULL*/)
 : CDialog(uResourceID, pParent)
{
	Init();
}

CBitmapDlg::CBitmapDlg(LPCTSTR pszResourceID, CWnd* pParent /*= NULL*/)
 : CDialog(pszResourceID, pParent)
{
	Init();
}

CBitmapDlg::~CBitmapDlg()
{
    FreeResources();

	for(int i=0 ; i<m_svPlane.size() ; i++)
	{
		if(m_svPlane[i].hBitmap)
			::DeleteObject (m_svPlane[i].hBitmap);
		m_svPlane[i].hBitmap = NULL;
	}

	m_svPlane.clear();
}


void CBitmapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBitmapDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBitmapDlg, CDialog)
	//{{AFX_MSG_MAP(CBitmapDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// 


BOOL CBitmapDlg::SetTransparent (BYTE bAlpha)
{
    if (g_pSetLayeredWindowAttributes == NULL)
        return FALSE;

    if (bAlpha < 255)
    {
        //  set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        
        //  call it with 255 as alpha - opacity
        g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
    }
    else
    {
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);

        // Ask the window and its children to repaint
        ::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
    }

    return TRUE;
}


BOOL CBitmapDlg::SetTransparentColor (COLORREF col, BOOL bTrans)
{
	m_colTrans = col;

	/*
	if (g_pSetLayeredWindowAttributes == NULL)
        return FALSE;

    if (bTrans)
    {
        //  set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        //  call it with 0 alpha for the given color
 //       g_pSetLayeredWindowAttributes(m_hWnd, col, 0, LWA_COLORKEY);
    }
    else
    {
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);

        // Ask the window and its children to repaint
        ::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
    }
	*/

	MakeWindowRgn();

    return TRUE;
}


CRect CBitmapDlg::GetChildWndPositon (CWnd *pChild)
{
	CRect rtThis,rtChild,rtNew;

	GetWindowRect(rtThis);
	pChild->GetWindowRect(rtChild);
	
	rtNew.left = rtChild.left-rtThis.left-1;
	rtNew.top = rtChild.top-rtThis.top-1;
	rtNew.right = rtChild.left+rtChild.Width()+2;
	rtNew.bottom = rtChild.top+rtChild.Height()+2;

	return rtNew;
}

void CBitmapDlg::Init()
{
    m_hBitmap = NULL;
    m_bEasyMove = TRUE;
    m_loStyle = LO_DEFAULT;
	m_bTransparent = TRUE;
    //  get the function from the user32.dll 
  //  HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
    //g_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)
      //                  GetProcAddress(hUser32, "SetLayeredWindowAttributes");
}


void CBitmapDlg::FreeResources()
{
    if (m_hBitmap)
        ::DeleteObject (m_hBitmap);
    m_hBitmap = NULL;
}

DWORD CBitmapDlg::SetBitmap(int nBitmap)
{
    HBITMAP    hBitmap       = NULL;
    HINSTANCE  hInstResource = NULL;
    
    // Find correct resource handle
    hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmap), RT_BITMAP);    
    // Load bitmap In
    hBitmap = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmap),
                                   IMAGE_BITMAP, 0, 0, 0);

    return SetBitmap(hBitmap);
}

DWORD CBitmapDlg::SetBitmap(HBITMAP hBitmap)
{
    int nRetValue;
    BITMAP  csBitmapSize;
    
    // Free any loaded resource
    FreeResources();
    
    if (hBitmap)
    {
        m_hBitmap = hBitmap;
        
        // Get bitmap size
        nRetValue = ::GetObject(hBitmap, sizeof(csBitmapSize), &csBitmapSize);
        if (nRetValue == 0)
        {
            FreeResources();
            return 0;
        }
        m_dwWidth = (DWORD)csBitmapSize.bmWidth;
        m_dwHeight = (DWORD)csBitmapSize.bmHeight;
    }
    
    if (IsWindow(this->GetSafeHwnd()))
        Invalidate();
    
    return 1;
    
}

DWORD CBitmapDlg::SetBitmap(LPCTSTR lpszFileName)
{
    HBITMAP    hBitmap       = NULL;
    hBitmap = (HBITMAP)::LoadImage(0, lpszFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    return SetBitmap(hBitmap);
}


int CBitmapDlg::AddBitmap(int nBitmap,int nX,int nY,int nWidth,int nHeight)
{
	HBITMAP    hBitmap       = NULL;
    HINSTANCE  hInstResource = NULL;
    
    // Find correct resource handle
    hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmap), RT_BITMAP);    
    // Load bitmap In
    hBitmap = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmap),
                                   IMAGE_BITMAP, 0, 0, 0);

	BITMAP_PLANE plane;
	plane.hBitmap = hBitmap;
	plane.nX = nX;
	plane.nY = nY;
	plane.nWidth = nWidth;
	plane.nHeight = nHeight;
	plane.nCurrPosX = nWidth;
	plane.nCurrPosY = nHeight;

	m_svPlane.push_back(plane);

	return m_svPlane.size()-1;
}

void CBitmapDlg::SetBltRateX(int nIdx,int nRate,int nTotal, BOOL bErase /* = FALSE */)
{
	if(nIdx >= m_svPlane.size())
		return;

	m_svPlane[nIdx].nCurrPosX = m_svPlane[nIdx].nWidth*((float)nRate/(float)nTotal);

	CRect rect(m_svPlane[nIdx].nX,
				m_svPlane[nIdx].nY,
				m_svPlane[nIdx].nX+m_svPlane[nIdx].nWidth,
				m_svPlane[nIdx].nY+m_svPlane[nIdx].nHeight);

	InvalidateRect(rect, bErase);
}

void CBitmapDlg::SetBltRateY(int nIdx,int nRate)
{
	if(nIdx >= m_svPlane.size())
		return;

	m_svPlane[nIdx].nCurrPosY = m_svPlane[nIdx].nHeight*nRate/100;

	CRect rect(m_svPlane[nIdx].nX,
				m_svPlane[nIdx].nY,
				m_svPlane[nIdx].nX+m_svPlane[nIdx].nWidth,
				m_svPlane[nIdx].nY+m_svPlane[nIdx].nHeight);

	InvalidateRect(rect);
}

void CBitmapDlg::EnableEasyMove(BOOL pEnable)
{
    m_bEasyMove = pEnable;
}

void CBitmapDlg::SetStyle(LayOutStyle style)
{
    m_loStyle = style;
    if(m_loStyle == LO_RESIZE && m_hBitmap)
    {
        SetWindowPos(0, 0, 0, m_dwWidth, m_dwHeight, SWP_NOMOVE | SWP_NOREPOSITION );
    }
}


/////////////////////////////////////////////////////////////////////////////
// CBitmapDlg message handlers

BOOL CBitmapDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBitmapDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	 if(point.x >= 0 && point.x < 536 && point.y >= 0 && point.y < 50 )
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	

	
	CDialog::OnLButtonDown(nFlags, point);
}

void CBitmapDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

BOOL CBitmapDlg::OnEraseBkgnd(CDC* pDC) 
{
    if (!m_hBitmap)
        return FALSE;

    CRect   rect;
    GetClientRect(rect);
    
    CDC dc;
    dc.CreateCompatibleDC(pDC);
	
    HBITMAP pbmpOldBmp = (HBITMAP)::SelectObject(dc.m_hDC, m_hBitmap);
   
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0,SRCCOPY);
	
    
	for(int i=0 ; i<m_svPlane.size() ; i++)
	{
		if(pbmpOldBmp)
			::SelectObject(dc.m_hDC, pbmpOldBmp);
		pbmpOldBmp = (HBITMAP)::SelectObject(dc.m_hDC, m_svPlane[i].hBitmap);


		pDC->StretchBlt(m_svPlane[i].nX, m_svPlane[i].nY, m_svPlane[i].nCurrPosX,m_svPlane[i].nCurrPosY,
						&dc, 
						0, 0,m_svPlane[i].nCurrPosX,m_svPlane[i].nCurrPosY,
						SRCCOPY); 

//			pDC->BitBlt(m_svPlane[i].nX, m_svPlane[i].nY, m_svPlane[i].nWidth,m_svPlane[i].nHeight,
//						&dc, 0, 0, SRCCOPY);
	}
    

    ::SelectObject(dc.m_hDC, m_hBitmap);

    return FALSE;
}

void CBitmapDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages

}

void CBitmapDlg::MakeWindowRgn ()
{
	if (!m_bTransparent)
	{
		// Set the window region to the full window
		CRect rc;
		GetWindowRect (rc);
		CRgn rgn;
		rgn.CreateRectRgn (0, 0, rc.Width(), rc.Height());
		SetWindowRgn (rgn, TRUE);
	}
	else
	{
		// Set the region to the window rect minus the client rect
		CRect rcWnd;
		GetWindowRect (rcWnd);

		CRgn rgn;
		rgn.CreateRectRgn (rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom);

		CRect rcClient;
		GetClientRect (rcClient);
		ClientToScreen (rcClient);

		CRgn rgnClient;
		rgnClient.CreateRectRgn (rcClient.left, rcClient.top, rcClient.right,
			rcClient.bottom);

		// Subtract rgnClient from rgn
		rgn.CombineRgn (&rgn, &rgnClient, RGN_XOR);

		// Get a DC for the bitmap
		CDC dcImage;
		dcImage.CreateCompatibleDC (NULL);

	    HBITMAP pbmpOldBmp = (HBITMAP)::SelectObject(dcImage.m_hDC, m_hBitmap);
		
		CRect rc;
		GetClientRect (rc);

		int width = rc.Width();
		int height = rc.Height();

		// Use RLE (run-length) style because it goes faster.
		// Row start is where the first opaque pixel is found.  Once
		// a transparent pixel is found, a line region is created.
		// Then row_start becomes the next opaque pixel.
		
		CDib dib;
		dib.LoadLLE( _T( "Luncher.lle" ) );
		if(dib.IsLoad())
		{
			int nColumn,nRow,nCount;
			for( int i = 0 ; i < dib.m_nSize ; i+=3 ) 
			{
				nColumn = dib.m_pData[i];
				nRow = dib.m_pData[i+1];
				nCount = dib.m_pData[i+2];

				CRgn rgnAdd;
				rgnAdd.CreateRectRgn (rcClient.left+nRow,
					rcClient.top+nColumn, rcClient.left+nRow+nCount, rcClient.top+nColumn+1);
				rgn.CombineRgn (&rgn, &rgnAdd, RGN_OR);
			}
		}
		else
		{
			// Go through all rows
			int row_start;
			
			for (int y=0; y<height; y++)
			{
				// Start looking at the beginning
				row_start = 0;

				// Go through all columns
                int x = 0;
				for (x=0; x<width; x++)
				{
					// If this pixel is transparent
					if (dcImage.GetPixel(x, y) == m_colTrans)
					{
						// If we haven't found an opaque pixel yet, keep searching
						if (row_start == x) 
							row_start ++;
						else
						{
							// We have found the start (row_start) and end (x) of
							// an opaque line.  Add it to the region.
					
							CRgn rgnAdd;
							rgnAdd.CreateRectRgn (rcClient.left+row_start,
								rcClient.top+y, rcClient.left+x, rcClient.top+y+1);
							rgn.CombineRgn (&rgn, &rgnAdd, RGN_OR);
							row_start = x+1;
						}
					}
				}

				// If the last pixel is still opaque, make a region.
				if (row_start != x)
				{
					CRgn rgnAdd;
					rgnAdd.CreateRectRgn (rcClient.left+row_start, rcClient.top+y,
						rcClient.left+x, rcClient.top+y+1);
					rgn.CombineRgn (&rgn, &rgnAdd, RGN_OR);
				}
			}
		}

		SetWindowRgn (rgn, TRUE);
	}
}