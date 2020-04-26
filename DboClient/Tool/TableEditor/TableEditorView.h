
// TableEditorView.h : interface of the CTableEditorView class
//

#pragma once


class CTableEditorView : public CView
{
protected: // create from serialization only
	CTableEditorView();
	DECLARE_DYNCREATE(CTableEditorView)

// Attributes
public:
	CTableEditorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CTableEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TableEditorView.cpp
inline CTableEditorDoc* CTableEditorView::GetDocument() const
   { return reinterpret_cast<CTableEditorDoc*>(m_pDocument); }
#endif

