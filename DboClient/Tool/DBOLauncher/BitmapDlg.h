#if !defined(AFX_BITMAPDLG_H__D8DB39B4_DF8A_483B_9C2C_EF956BE56E4C__INCLUDED_)
#define AFX_BITMAPDLG_H__D8DB39B4_DF8A_483B_9C2C_EF956BE56E4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BitmapDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitmapDlg dialog

#include <afxcmn.h>
#include <list>
#include <vector>


//  ===========================================================================
//  Layout styles for the background bmp
//  ===========================================================================
enum LayOutStyle
{
    LO_DEFAULT,
    LO_TILE,    // Tile the background picture
    LO_CENTER,  // Center the background picture
    LO_STRETCH, // Stretch the background picture to the dialog window size
    LO_RESIZE   // Resize the dialog so that it just fits the background 
};

typedef struct _tagBitmapPlane
{
	HBITMAP hBitmap;
	int nX,nY;
	int nWidth,nHeight;
	int nCurrPosX,nCurrPosY;
}BITMAP_PLANE;

class CBitmapDlg : public CDialog
{
// Construction
public:

	CBitmapDlg(CWnd* pParent = NULL);   // standard constructor
	CBitmapDlg(UINT uResourceID, CWnd* pParent = NULL);
	CBitmapDlg(LPCTSTR pszResourceID, CWnd* pParent = NULL);
    virtual  ~CBitmapDlg();

    //  =======================================================================
    //  desc    Dialog Background를 위한 bitmap
    //  arg1    See the definition of LayOutStyle above
    //  =======================================================================

    DWORD SetBitmap(HBITMAP hBitmap);
    DWORD SetBitmap(int nBitmap);
    DWORD SetBitmap(LPCTSTR lpszFileName);

    //  =======================================================================
    //  desc    Bitmap 출력.
    //  =======================================================================

    int AddBitmap(int nBitmap,int nX,int nY,int nWidth,int nHeight);

	//  =======================================================================
    //  desc    Bitmap 출력을 어디까지 할 것인가?
    //  =======================================================================
	void SetBltRateX(int nIdx,int nRate,int nTotal, BOOL bErase = FALSE);

	//  =======================================================================
    //  desc    Bitmap 출력을 어디까지 할 것인가?
    //  =======================================================================
	void SetBltRateY(int nIdx,int nRate);
  
    //  =======================================================================
    //  desc    Set the bitmap layout style
    //  arg1    See the definition of LayOutStyle above
    //  =======================================================================
    void SetStyle(LayOutStyle style);

	//  =======================================================================
    //  desc    Allows moving of the dialog by clicking anywhere in the dialog
    //  
    //  arg1    TRUE enables, FLASE disables
    //  =======================================================================
    void EnableEasyMove (BOOL pEnable = TRUE);

    //  =======================================================================
    //  desc    Makes the dialog transparent
    //  
    //  arg1    Alpha range is 0-255. If 255 then dialog becomes opaque
    //  ret     TRUE if successful
    //  =======================================================================
    BOOL SetTransparent (BYTE bAlpha);

    //  =======================================================================
    //  desc    Makes one color on the dialog transparent. This can be used 
    //          to create irregular shaped dialogs
    //  
    //  arg1    color to be made transparent
    //  arg2    TRUE will use the color, FALSE remove transparency
    //  ret     TRUE if successful
    //  =======================================================================
    BOOL SetTransparentColor (COLORREF col, BOOL bTrans = TRUE);

	//  =======================================================================
    //  desc    child window position
    //  =======================================================================
	CRect GetChildWndPositon (CWnd *pChild);

	void MakeWindowRgn ();

// Dialog Data
	//{{AFX_DATA(CBitmapDlg)
	
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBitmapDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    
	BOOL m_bEasyMove;
	void FreeResources();
	void Init();
	HBITMAP		m_hBitmap;
    DWORD		m_dwWidth;			// Width of bitmap
	DWORD		m_dwHeight;			// Height of bitmap
    LayOutStyle m_loStyle;          // LayOutStyle style
	BOOL		m_bTransparent;
	COLORREF	m_colTrans;

	std::vector<BITMAP_PLANE> m_svPlane;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPDLG_H__D8DB39B4_DF8A_483B_9C2C_EF956BE56E4C__INCLUDED_)
