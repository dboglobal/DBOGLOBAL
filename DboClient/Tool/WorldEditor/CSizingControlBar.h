#ifndef _C_SIZING_CONTROL_BAR_H_
#define _C_SIZING_CONTROL_BAR_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef TCB_ITEM
typedef struct
{
	CWnd *	pWnd;
	CHAR	szLabel[32];
} TCB_ITEM;
#endif //TCB_ITEM


class CSizingControlBar : public CControlBar
{
	protected:

		CSize							m_sizeMin;
		CSize							m_sizeMax;
		CSize							m_sizeHorz;
		CSize							m_sizeVert;
		CSize							m_sizeFloat;
		CRect							m_rectBorder;
		CRect							m_rectTracker;
		UINT							m_nDockBarID;
		CPoint							m_ptOld;
		BOOL							m_bTracking;
		BOOL							m_bInRecalcNC;
		int								m_cxEdge;
		HBRUSH							m_WndClassBrush;

	public:

										CSizingControlBar();
		virtual							~CSizingControlBar();

	public:

		virtual void					OnUpdateCmdUI( CFrameWnd * pTarget, BOOL bDisableIfNoHndler );

	public:

		void							StartTracking();
		void							StopTracking( BOOL bAccept );
		void							OnInvertTracker( const CRect& rect );
	    BOOL							IsHorz() const;	
		void							ToggleDockingPalette( void );
		void							SetFloatingSize( SIZE Size ) { m_sizeFloat = Size; }
		CPoint &						ClientToWnd( CPoint & point );
		UINT							GetDockBarID( void ) const { return m_nDockBarID; }

		// ClassWizard generated virtual function overrides
	public:

		//{{AFX_VIRTUAL(CSizingControlBar)
		virtual BOOL Create(CWnd* pParentWnd, CSize sizeDefault, UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP);
		virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz );
		virtual CSize CalcDynamicLayout( int nLength, DWORD dwMode );
		//}}AFX_VIRTUAL

		// Generated message map functions
	protected:

		//{{AFX_MSG(CSizingControlBar)
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
		afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
		afx_msg void OnNcPaint();
		afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
		afx_msg LRESULT OnNcHitTest(CPoint point);
		afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnCaptureChanged(CWnd *pWnd);
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void OnPaint();
		afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
		//}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif //_C_SIZING_CONTROL_BAR_H_

