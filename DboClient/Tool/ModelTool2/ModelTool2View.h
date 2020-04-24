// ModelTool2View.h : interface of the CModelTool2View class
//


#pragma once


class CModelTool2View : public CView
{
protected: // create from serialization only
	CModelTool2View();
	DECLARE_DYNCREATE(CModelTool2View)

// Attributes
public:
	CModelTool2Doc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CModelTool2View();
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

#ifndef _DEBUG  // debug version in ModelTool2View.cpp
inline CModelTool2Doc* CModelTool2View::GetDocument() const
   { return reinterpret_cast<CModelTool2Doc*>(m_pDocument); }
#endif
