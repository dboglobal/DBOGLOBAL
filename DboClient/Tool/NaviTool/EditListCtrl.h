#if !defined(AFX_MYLIST_H__66BF5629_D02A_421B_9C9F_B7CBFE5D89DD__INCLUDED_)
#define AFX_MYLIST_H__66BF5629_D02A_421B_9C9F_B7CBFE5D89DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditListCtrl window
#include "InPlaceEdit.h"

class CEditListCtrl : public CListCtrl
{
	// Construction
public:
	CEditListCtrl();
	int HitTestEx(CPoint &point, int *col) const;
	CEdit* EditSubLabel( int nItem, int nCol );
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// Attributes
public:
	CEdit m_ctrEdit;
	// Operations
public:
	void GetSubItemFromPt(CPoint point, int &nItem, int &nSubItem, CRect &rect);
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditListCtrl)

	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CEditListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditListCtrl)
	// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLIST_H__66BF5629_D02A_421B_9C9F_B7CBFE5D89DD__INCLUDED_)
