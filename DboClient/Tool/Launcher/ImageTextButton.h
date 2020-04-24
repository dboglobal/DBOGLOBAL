   #if !defined(AFX_IMAGETEXTBUTTON_H__1830F0FD_C60B_4C36_9583_603844E90E51__INCLUDED_)
#define AFX_IMAGETEXTBUTTON_H__1830F0FD_C60B_4C36_9583_603844E90E51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageTextButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageTextButton window

class CImageTextButton : public CButton
{
// Construction
public:
   CImageTextButton();

// Attributes
public:

	BOOL	m_bTracking;
	BOOL	m_bHover;

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CImageTextButton)
   //}}AFX_VIRTUAL

// Implementation
public:


   void SetTextPosition(UINT uiPos );
   void SetTextPosition(int nXPos ,int nYPos);
   void SetTextColor(const COLORREF& clrUpDwn, const COLORREF& clrDisabled );
   void SetButtonText(const CString& strCaption);
   void SetButtonImg(UINT uiImageIDU,UINT uiImgIDD =0,UINT uiImgIDH = 0, UINT uiImgIDDis = 0);
   void SetFont(CString srtFntName_i, int nSize_i);
   virtual ~CImageTextButton();

   virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

   // Generated message map functions
protected:
   //Keep the size details of the bitmap loaded
   BITMAP         m_bitmap;
   //Keeps image list which keeps the images to be drawn for up/down and disabled states
   //of the button
   CImageList  m_ImageList;
   //Keeps the status of the image load
   BOOL           m_bLoaded;
   //Keeps the text to be shown in the button
   CString     m_strText;
   //Keeps the resized rect of the button
   CRect          m_rectBtn;
   //Keeps the up/down text color of the button
   COLORREF    m_clrCptnUpDwn;
   //Keeps the disabled text color of the button
   COLORREF    m_clrCptnDisabled;
   //Keeps the X position of the Text to be drawn
   int            m_nXpos ;
   //Keeps the Y position of the Text to be drawn
   int            m_nYpos;
   //Keeps the position of the Text like 
   UINT           m_uiPos;
   CFont          m_Font;

   //{{AFX_MSG(CImageTextButton)
      // NOTE - the ClassWizard will add and remove member functions here.
      afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	  afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
      afx_msg void OnSetFocus(CWnd* pOldWnd);
      afx_msg void OnEnable(BOOL bEnable);
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGETEXTBUTTON_H__1830F0FD_C60B_4C36_9583_603844E90E51__INCLUDED_)
