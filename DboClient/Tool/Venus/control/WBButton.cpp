// WBButton.cpp : 
//

#include "stdafx.h"
#include "WBButton.h"
#include "AutoFont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWBButton

CWBButton::CWBButton()
{
    m_TopHeight = 8;
    m_BottomHeight = 8;
    m_LeftWidth = 8;
    m_RightWidth = 17;
    m_State = notInited;
    m_pFnt = new CAutoFont("MS Sans Serif");
    m_BkColor = RGB(255,0,255);
    
    m_RcId       = 0;       // Resource ID
    m_NumofPics  = 0;  

	m_Checked = FALSE;
}

CWBButton::~CWBButton()
{
    NormalBitmap.DeleteObject();
    SelectBitmap.DeleteObject();
    FocusBitmap.DeleteObject();
    DisableBitmap.DeleteObject();
    SAFE_DELETE(m_pFnt);
}


BEGIN_MESSAGE_MAP(CWBButton, CButton)
	//{{AFX_MSG_MAP(CWBButton)
    ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CWBButton::setButtonCheck( BOOL Check )
{ 
	if( m_Checked != Check )
	{
		m_Checked = Check; 
		Invalidate(FALSE);
	}
}


void CWBButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    CDC xdc;
    xdc.Attach( lpDrawItemStruct->hDC );
    CRect rc;
    GetClientRect(rc);
    CMemDC dc(&xdc,rc);
    
    UINT state = lpDrawItemStruct->itemState ;
    bool IsDisable = false;

	if( m_Checked )
	{
		DrawBitmap( &dc, select );		
	}
	else if (state & ODS_FOCUS)
    {
		DrawBitmap( &dc, focus );
		if (state & ODS_SELECTED)
        { 
            DrawBitmap( &dc, select );
            rc.left += 5;
		}		
	}
	else if (state & ODS_DISABLED)
    {
        IsDisable = true;
    	DrawBitmap( &dc, disable );
    }
	else
	{

        DrawBitmap( &dc, normal );
    }

    int imode = dc.SetBkMode(TRANSPARENT);
    CFont *pOldFnt = dc.SelectObject(m_pFnt);
    COLORREF oldColor;

    if(IsDisable)
	{
      oldColor = dc.SetTextColor( GetSysColor(COLOR_GRAYTEXT) );
	  CString txt;
	  GetWindowText(txt);
      dc.DrawText(txt,rc,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
    else
	{
	  CString txt;
	  GetWindowText(txt);

	  oldColor = dc.SetTextColor( GetSysColor(COLOR_GRAYTEXT) );
	  
      dc.DrawText(txt,rc+1,DT_CENTER | DT_VCENTER | DT_SINGLELINE);

      oldColor = dc.SetTextColor( m_pFnt->GetFontColor() );

      dc.DrawText(txt,rc,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
      
    dc.SetTextColor( oldColor );
    dc.SelectObject(pOldFnt);
    dc.SetBkMode(imode );
	
}


bool CWBButton::LoadBitmaps
(
     UINT id, int count ,    
     int TopHeight, int BottomHeight, int LeftWidth, int RightWidth
)
{
    m_TopHeight = TopHeight;
    m_BottomHeight = BottomHeight;
    m_LeftWidth = LeftWidth;
    m_RightWidth = RightWidth;

    m_RcId       = id;       // Resource ID
    m_NumofPics  = count;  


    CBitmap bmp;
    if( !bmp.LoadBitmap(id) ) return false;

    if( !InitBitmap( bmp, NormalBitmap, 0, count ) ) return false;
    if( !InitBitmap( bmp, SelectBitmap, 1, count ) ) return false;
    if( !InitBitmap( bmp, DisableBitmap,2, count ) ) return false;
    if( !InitBitmap( bmp, FocusBitmap,  3, count ) ) return false;

    bmp.DeleteObject();

    return true;
}

bool CWBButton::InitBitmap( CBitmap & src, CBitmap & dist, int index, int count )
{
    CDC * pDC = GetDC();

    CDC memDC; 
    memDC.CreateCompatibleDC(pDC);

    CDC srcDC;
    srcDC.CreateCompatibleDC(pDC);

    CBitmap* pOldBitmap1;
    pOldBitmap1 = srcDC.SelectObject(&src);

    BITMAP bmpinfo;
    src.GetBitmap(&bmpinfo);
    int bmpWidth = bmpinfo.bmWidth / count;
    int bmpHeight = bmpinfo.bmHeight;
    int orix = bmpWidth * index;

    CRect rc;
    GetClientRect(rc);

    CBitmap* pOldBitmap2;
    dist.DeleteObject();
    dist.CreateCompatibleBitmap(pDC,rc.Width(),rc.Height());
    pOldBitmap2 = memDC.SelectObject(&dist);

    // lefttop,leftbottom,righttop,rightbottom
    if( !memDC.BitBlt(0,0,m_LeftWidth, m_TopHeight, &srcDC,orix,0,SRCCOPY) ) return false;
    

    if( !memDC.BitBlt(0,rc.bottom - m_BottomHeight,m_LeftWidth, m_BottomHeight, 
                &srcDC,orix,bmpHeight - m_BottomHeight,SRCCOPY) ) return false;


    if( !memDC.BitBlt(rc.right - m_RightWidth, 0 ,m_RightWidth, m_TopHeight, 
                 &srcDC,orix + bmpWidth - m_RightWidth,0,SRCCOPY) ) return false;


    if( !memDC.BitBlt(rc.right - m_RightWidth,rc.bottom - m_BottomHeight,m_RightWidth,m_BottomHeight,
                 &srcDC,orix + bmpWidth - m_RightWidth,bmpHeight - m_BottomHeight,SRCCOPY) ) return false;

    // entireimage
    memDC.StretchBlt(m_LeftWidth,
                     m_TopHeight,
                     rc.Width()  - ( m_LeftWidth + m_RightWidth ) ,
                     rc.Height() -  ( m_TopHeight + m_BottomHeight) ,
                     &srcDC,
                     orix + m_LeftWidth,
                     m_TopHeight,
                     bmpWidth - ( m_LeftWidth + m_RightWidth ) ,
                     bmpHeight - ( m_TopHeight + m_BottomHeight ) ,SRCCOPY);

    
    // topbar,bottombar( Stretch )
    memDC.StretchBlt(m_LeftWidth,0, rc.Width() - (m_LeftWidth + m_RightWidth), m_TopHeight,
                     &srcDC,orix + m_LeftWidth, 0, bmpWidth - ( m_LeftWidth + m_RightWidth) , m_TopHeight,SRCCOPY);

    memDC.StretchBlt(m_LeftWidth, rc.bottom - m_BottomHeight, rc.Width() - ( m_LeftWidth + m_RightWidth), m_BottomHeight,
                     &srcDC,orix + m_LeftWidth,bmpHeight - m_BottomHeight,bmpWidth - ( m_LeftWidth + m_RightWidth) , m_BottomHeight,SRCCOPY );


    // sidebar ( STretch? )
    memDC.StretchBlt(0,m_TopHeight,m_LeftWidth,rc.bottom - m_TopHeight - m_BottomHeight ,
                     &srcDC, orix,m_TopHeight, m_LeftWidth, bmpHeight - ( m_TopHeight + m_BottomHeight ) ,SRCCOPY);

    memDC.StretchBlt(rc.right - m_RightWidth ,m_TopHeight,m_RightWidth,rc.bottom - m_TopHeight - m_BottomHeight ,
                     &srcDC, orix +  bmpWidth - m_RightWidth,m_TopHeight, m_RightWidth, bmpHeight - m_TopHeight - m_BottomHeight ,SRCCOPY);

    srcDC.SelectObject(pOldBitmap1);
    memDC.SelectObject(pOldBitmap2);

    ReleaseDC(pDC);
    ReleaseDC(&srcDC);
    ReleaseDC(&memDC);

     m_State = FileLoaded;
    
    return true;

}

void CWBButton::DrawBitmap( CDC * pDC, int mode )
{
    if( m_State < FileLoaded ) return;

    CRect rc;
    GetClientRect(rc);
 
	COLORREF crOldBack = pDC->SetBkColor(RGB(255,255,255));
	COLORREF crOldText = pDC->SetTextColor(RGB(0,0,0));
	CDC dcImage, dcTrans;

	// Create two memory dcs for the image and the mask
	dcImage.CreateCompatibleDC(pDC);
	dcTrans.CreateCompatibleDC(pDC);

	// Select the image into the appropriate dc
    CBitmap* pOldBitmapImage;
    switch(mode)
    {
    case normal:
	   pOldBitmapImage  = dcImage.SelectObject(&NormalBitmap);
       break;
    case select:
	   pOldBitmapImage  = dcImage.SelectObject(&SelectBitmap);
       break;
    case focus:
	   pOldBitmapImage  = dcImage.SelectObject(&FocusBitmap);
       break;
    case disable:
	   pOldBitmapImage  = dcImage.SelectObject(&DisableBitmap);
       break;
    default:
        return;
    }
      
	// Create the mask bitmap
	CBitmap bitmapTrans;
	int nWidth = rc.Width();
	int nHeight = rc.Height();
	bitmapTrans.CreateBitmap(nWidth, nHeight, 1, 1, NULL);

    // Select the mask bitmap into the appropriate dc
	CBitmap* pOldBitmapTrans = dcTrans.SelectObject(&bitmapTrans);

	// Build mask based on transparent colour
	dcImage.SetBkColor(m_BkColor);
	dcTrans.BitBlt(0, 0, nWidth, nHeight, &dcImage, 0, 0, SRCCOPY);

	// Do the work - True Mask method - cool if not actual display
	pDC->BitBlt(0, 0, nWidth, nHeight, &dcImage, 0, 0, SRCINVERT);
	pDC->BitBlt(0, 0, nWidth, nHeight, &dcTrans, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, nWidth, nHeight, &dcImage, 0, 0, SRCINVERT);

	// Restore settings
	dcImage.SelectObject(pOldBitmapImage);
	dcTrans.SelectObject(pOldBitmapTrans);
	pDC->SetBkColor(crOldBack);
	pDC->SetTextColor(crOldText);
}



BOOL CWBButton::OnEraseBkgnd(CDC* pDC) 
{
    return FALSE;
}

void CWBButton::SetButtonDef( int TopHeight, int BottomHeight, int LeftWidth, int RightWidth )
{ 
    m_TopHeight = TopHeight;
    m_BottomHeight = BottomHeight;
    m_LeftWidth = LeftWidth;
    m_RightWidth = RightWidth;

    if( m_RcId != 0 && m_NumofPics != 0 )
    {
       LoadBitmaps(m_RcId,m_NumofPics,TopHeight,BottomHeight,LeftWidth,RightWidth);
    }

}

void CWBButton::SetTextFont( CFont & fnt )
{ 

    LOGFONT lf;
    fnt.GetLogFont(&lf);
    SAFE_DELETE(m_pFnt);
    m_pFnt = new CAutoFont(lf);
}

void CWBButton::SetFontColor( COLORREF color )
{
    m_pFnt->SetFontColor(color);
    UpdateWindow();
}

