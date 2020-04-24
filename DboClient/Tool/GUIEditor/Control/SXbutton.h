// SXButton.cpp : implementation file
// Michael Santoro - msantoro@telerama.lm.com
//
//	Revision History
//	================
//	08-30-97	msantoro	Origional Code
//	11-09-97	msantoro	Added bitmap support
//  11-12-97	msantoro	Fixed drawing of too many focused buttons
//  05-18-98    saubrecht	Resource Cleanup
/////////////////////////////////////////////////////////////////////////////

// CSXButton window

#ifndef _SXBUTTON_H
#define _SXBUTTON_H

#define	SXBUTTON_CENTER	-1

class CSXButton : public CButton
{
// Construction
public:
	CSXButton();

// Attributes
private:
	//	Positioning
	BOOL		m_bUseOffset;				
	CPoint		m_pointImage;
	CPoint		m_pointText;
	int			m_nImageOffsetFromBorder;
	int			m_nTextOffsetFromImage;

	//	Image
	HICON		m_hIcon;					
	HBITMAP		m_hBitmap;
	HBITMAP		m_hBitmapDisabled;
	int			m_nImageWidth, m_nImageHeight;

	//	Color Tab
	char		m_bColorTab;				
	COLORREF	m_crColorTab;

	//	State
	BOOL		m_bDefault;
	UINT		m_nOldAction;
	UINT		m_nOldState;
	

// Operations
public:
	//	Positioning
	int		SetImageOffset( int nPixels ); 
	int		SetTextOffset( int nPixels );
	CPoint	SetImagePos( CPoint p );
	CPoint	SetTextPos( CPoint p );

	//	Image
	BOOL	SetIcon( UINT nID, int nWidth, int nHeight );
	BOOL	SetBitmap( UINT nID, int nWidth, int nHeight );
	BOOL	SetMaskedBitmap( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask );
	BOOL	HasImage() { return (BOOL)( (m_hIcon != 0)  || (m_hBitmap != 0) ); }

	//	Color Tab
	void	SetColorTab(COLORREF crTab);

	//	State
	BOOL	SetDefaultButton( BOOL bState = TRUE );

private:
	BOOL	SetBitmapCommon( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask, BOOL bUseMask );
	void	CheckPointForCentering( CPoint &p, int nWidth, int nHeight );
	void	Redraw();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSXButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSXButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSXButton)
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif
/////////////////////////////////////////////////////////////////////////////
