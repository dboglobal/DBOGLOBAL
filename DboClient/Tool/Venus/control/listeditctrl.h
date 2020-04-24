#if !defined(AFX_LISTEDITCTRL_H__3C02B0B1_A395_11D1_9799_002018026B76__INCLUDED_)
#define AFX_LISTEDITCTRL_H__3C02B0B1_A395_11D1_9799_002018026B76__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ListEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListEditCtrl window

class CListEditCtrl : public CEdit
{
// Construction
public:
	CListEditCtrl(int iItem, int iSubItem, CString sInitText);
// Attributes
public:

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListEditCtrl(){};
private:	
	int m_iItem;
	int m_iSubItem;	
	CString m_strInitText;
	BOOL  m_bVK_ESCAPE;
	// Generated message map functions
protected:
	//{{AFX_MSG(CListEditCtrl)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTEDITCTRL_H__3C02B0B1_A395_11D1_9799_002018026B76__INCLUDED_)

