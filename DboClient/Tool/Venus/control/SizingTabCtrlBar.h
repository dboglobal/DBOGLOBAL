#ifndef AFX_SIZINGTABCTRLBAR_H__3804CCE3_C325_11D1_91F5_B4375BB20A38__INCLUDED_
#define AFX_SIZINGTABCTRLBAR_H__3804CCE3_C325_11D1_91F5_B4375BB20A38__INCLUDED_

// SizingTabCtrlBar.h : Header-Datei
//
#include <afxtempl.h>
#include "sizecbar.h"

#include "BtnST.h"
#include "SXButton.h"
#include "WBButton.h"
#include "Dib.h"

#include "resource.h"

typedef struct
{
	CWnd *pWnd;
	CHAR szLabel[32];
}TCB_ITEM;

/////////////////////////////////////////////////////////////////////////////
// Fenster CSizingTabCtrlBar 

class CSizingTabCtrlBar : public CSizingControlBar
{
// Konstruktion
public:
	CSizingTabCtrlBar();

// Attribute
public:
	
	CFont   m_font;
	
// Operationen
public:

// ?erschreibungen
	void SetActiveView(int nNewTab);
	void SetActiveView(CRuntimeClass *pViewClass);
	int	 GetActiveViewIndex(void){ return m_nActiveTab; }
	// Vom Klassen-Assistenten generierte virtuelle Funktions?erschreibungen
	
	//{{AFX_VIRTUAL(CSizingTabCtrlBar)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementierung
public:	
	CView* GetActiveView();
	CView* GetView(int nView);
	CView* GetView(CRuntimeClass *pViewClass);
	BOOL AddView(LPCTSTR lpszLabel, CRuntimeClass *pViewClass, CCreateContext *pContext = NULL);
	void RemoveView(int nView);	
	virtual ~CSizingTabCtrlBar();

	CTabCtrl m_tabctrl;

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	int m_nActiveTab;
	CView* m_pActiveView;
	CList <TCB_ITEM *,TCB_ITEM *> m_views;
	CImageList m_images;
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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio f?t zus?zliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_SIZINGTABCTRLBAR_H__3804CCE3_C325_11D1_91F5_B4375BB20A38__INCLUDED_
