#ifndef C_SIZING_TAB_CONTROL_BAR
#define C_SIZING_TAB_CONTROL_BAR

#include "CSizingControlBar.h"

class CSizingTabControlBar : public CSizingControlBar
{
	protected:

		CFont							m_font;
		CTabCtrl						m_tabctrl;
		int								m_nActiveTab;
		CView *							m_pActiveView;
		CList <TCB_ITEM *,TCB_ITEM *>	m_views;

	public:

										CSizingTabControlBar();
		virtual							~CSizingTabControlBar();

	public:

		void							SetActiveView(int nNewTab);
		void							SetActiveView(CRuntimeClass *pViewClass);
		int								GetActiveViewIndex(void) const { return m_nActiveTab; }
		virtual int						AddViewes( void );

		CView *							GetActiveView();
		CView *							GetView( int nView );
		CView *							GetView( CRuntimeClass * pViewClass );
		BOOL							AddView( LPCTSTR lpszLabel, CRuntimeClass *pViewClass, CCreateContext *pContext = NULL );
		void							RemoveView( int nView );

	protected:

		//{{AFX_VIRTUAL(CSizingTabCtrlBar)
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

	protected:
		
		//{{AFX_MSG(CSizingTabCtrlBar)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult) ;
		afx_msg void OnPaint();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);	
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
		//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


#endif //C_SIZING_TAB_CONTROL_BAR
