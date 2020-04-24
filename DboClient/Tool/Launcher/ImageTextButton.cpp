       // ImageTextButton.cpp : implementation file
//

#include "stdafx.h"
#include "ImageTextButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CImageTextButton::CImageTextButton():m_strText(""),
                                     m_nXpos(-1),
                                     m_nYpos(-1),
                                     m_uiPos(DT_SINGLELINE | DT_CENTER | DT_VCENTER),
                                     m_bLoaded(FALSE),
                                     m_clrCptnUpDwn(RGB(255,255,255)),
                                     m_clrCptnDisabled(RGB(255,255,255))
{ 
 	m_bHover = FALSE;
	m_bTracking = FALSE;
}

CImageTextButton::~CImageTextButton()
{
    m_ImageList.DeleteImageList();
    m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CImageTextButton, CButton)
	//{{AFX_MSG_MAP(CImageTextButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
 	ON_WM_SETFOCUS()
	ON_WM_ENABLE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageTextButton message handlers

void CImageTextButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct ) 
{
if( lpDrawItemStruct->CtlType != ODT_BUTTON)
      return;
    //To keep the text color 
    COLORREF clrText;
    
    int nImgCount =0;
    //Keeps the index of the image(based on 0) to be drawn
    int nPosImg=0;

    CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
    if(!m_bLoaded)
    {
      m_rectBtn = lpDrawItemStruct->rcItem;
    }
    else
    {
      //Gets the number of image in the image list
      nImgCount = m_ImageList.GetImageCount();
      ::SetWindowPos(lpDrawItemStruct->hwndItem,NULL,m_rectBtn.left,m_rectBtn.top,m_bitmap.bmWidth,m_bitmap.bmHeight,SWP_NOMOVE);

    }

    pDC->DrawFrameControl( &m_rectBtn ,DFC_BUTTON , DFCS_BUTTONPUSH|DFCS_PUSHED);
    //if( m_bLoaded )
    {
	  if(ODS_DISABLED & lpDrawItemStruct->itemState)
	  {
		if( m_bLoaded )
		{
			nPosImg = 3;
		} 
		clrText = m_clrCptnDisabled;
	  }
	  else if(ODS_SELECTED & lpDrawItemStruct->itemState)
	  {
		if( m_bLoaded )
		{
		  if(2 <= nImgCount )
		  nPosImg = 1;
		else
		  nPosImg =0;
		}
		clrText = m_clrCptnUpDwn;
	  }
	  else 
	  {
		
		if( m_bLoaded )
		{
			if(m_bHover)
			{
				nPosImg = 2;
			}
			else
			nPosImg =0;
		}
		clrText = m_clrCptnUpDwn;
	  }

      if( m_bLoaded )
      {
      //Draw the image in to the DC
      m_ImageList.DrawIndirect( 
                               pDC , 
                               nPosImg , 
                               CPoint( m_rectBtn.left,m_rectBtn.top ), 
                               CSize( m_bitmap.bmWidth , m_bitmap.bmHeight ), 
                               CPoint( 0 , 0 ) , 
                               ILD_NORMAL 
                              );
      }
    }
    if(!m_strText.IsEmpty())
    {
      pDC->SetBkMode(TRANSPARENT);
      pDC->SetTextColor( clrText);
      
      HFONT hOldFont=NULL;
     //Sets the font
      if(m_Font.GetSafeHandle() != NULL)
      {
        hOldFont =(HFONT) pDC->SelectObject(m_Font.GetSafeHandle());
      }
      if(m_nYpos!=-1 && m_nXpos!=-1)
      {
        pDC->TextOut(m_nXpos,m_nYpos,m_strText);
      }
      else
      {
        pDC->DrawText( m_strText, m_rectBtn , m_uiPos);
      }
      //Reset the old font
      if(hOldFont != NULL)
      {
        pDC->SelectObject(hOldFont);
      }
  }
}
void CImageTextButton::SetButtonImg(UINT uiImageIDU,UINT uiImgIDD /* =0 */,UINT uiImgIDH /* = 0 */, UINT uiImgIDDis /* = 0 */)
{
  CBitmap bitmapImage;
  if( m_bLoaded )
  {
    m_ImageList.DeleteImageList();
    m_bLoaded = FALSE;
  }
  //Loading Bitmap
  if( bitmapImage.LoadBitmap( uiImageIDU ) ) 
  {
    
    bitmapImage.GetObject( sizeof( m_bitmap ) , &m_bitmap );
    m_rectBtn.top =0;m_rectBtn.left =0;
    m_rectBtn.right = m_bitmap.bmWidth; m_rectBtn.bottom =m_bitmap.bmHeight;
    
    if( m_ImageList.Create( m_bitmap.bmWidth , m_bitmap.bmHeight , ILC_COLORDDB,  1 , 2 ) ) 
    {
		    m_ImageList.Add( &bitmapImage ,&bitmapImage);
        if(0 !=uiImgIDD)
        {
          bitmapImage.DeleteObject();
          bitmapImage.LoadBitmap( uiImgIDD );
          m_ImageList.Add( &bitmapImage ,&bitmapImage);
          
        }
        if(0 !=uiImgIDH)
        {
          bitmapImage.DeleteObject();
          bitmapImage.LoadBitmap( uiImgIDH );
          m_ImageList.Add( &bitmapImage ,&bitmapImage);
          
        }
        if(0 != uiImgIDDis)
        {
            bitmapImage.DeleteObject();
            bitmapImage.LoadBitmap( uiImgIDDis );
            m_ImageList.Add( &bitmapImage ,&bitmapImage);            
        }

        bitmapImage.DeleteObject();
        m_bLoaded = TRUE;
    }
  }
}

void CImageTextButton::SetButtonText(const CString &strCaption)
{
  m_strText = strCaption;
}

void CImageTextButton::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	Invalidate(TRUE);
}
void CImageTextButton::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
	// TODO: Add your message handler code here
	Invalidate(TRUE);
}
//Function used for setting the position of the Text in the button according X y position
void CImageTextButton::SetTextPosition(int nXPos , int nYPos)
{
  m_nXpos = nXPos;
  m_nYpos = nYPos;
}

//Function used for setting the position of the Text in the button according to the center, left etc
void CImageTextButton::SetTextPosition(UINT uiPos )
{
  m_uiPos = uiPos;
}
void CImageTextButton::SetTextColor(const COLORREF &clrUpDwn,const COLORREF &clrDisabled)
{
    m_clrCptnUpDwn    = clrUpDwn;
    m_clrCptnDisabled = clrDisabled;
}

void CImageTextButton::SetFont(CString srtFntName_i, int nSize_i)
{
		LOGFONT lfCtrl = {0};
		lfCtrl.lfOrientation	= 0 ;
		lfCtrl.lfEscapement		= 0 ;

			lfCtrl.lfHeight			= nSize_i;
		
		lfCtrl.lfItalic  = FALSE;
		lfCtrl.lfUnderline = FALSE;
		lfCtrl.lfStrikeOut = FALSE;

		lfCtrl.lfCharSet		= DEFAULT_CHARSET;
		lfCtrl.lfQuality		= DEFAULT_QUALITY;
		lfCtrl.lfOutPrecision	= OUT_DEFAULT_PRECIS;
		lfCtrl.lfPitchAndFamily	= DEFAULT_PITCH;
		_tcscpy(lfCtrl.lfFaceName, srtFntName_i);
	
		//Normal Font
		lfCtrl.lfWeight			= FW_NORMAL;	
		m_Font.CreateFontIndirect( &lfCtrl);
}

void CImageTextButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	//	TODO: Add your message handler code here and/or call default

	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	if( !(nFlags & MK_LBUTTON) )
	{
		if(!m_bHover)
			Invalidate(FALSE);

		m_bHover = TRUE;
	}


	CButton::OnMouseMove(nFlags, point);
}


LRESULT CImageTextButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	if(m_bHover)
		Invalidate(FALSE);

	m_bHover = FALSE;
	m_bTracking = FALSE;

	return 0;
}

