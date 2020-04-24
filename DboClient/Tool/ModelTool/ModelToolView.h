// ModelToolView.h : interface of the CModelToolView class
//


#pragma once


class CModelToolView : public CView
{
protected: // create from serialization only
	CModelToolView();
	DECLARE_DYNCREATE(CModelToolView)

// Attributes
public:
	CModelToolDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CModelToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in ModelToolView.cpp
inline CModelToolDoc* CModelToolView::GetDocument() const
   { return reinterpret_cast<CModelToolDoc*>(m_pDocument); }
#endif

