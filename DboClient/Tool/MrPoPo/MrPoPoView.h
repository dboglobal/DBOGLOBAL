// MrPoPoView.h : interface of the CMrPoPoView class
//


#pragma once


class CMrPoPoView : public CView
{
protected: // create from serialization only
	CMrPoPoView();
	DECLARE_DYNCREATE(CMrPoPoView)

// Attributes
public:
	CMrPoPoDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMrPoPoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MrPoPoView.cpp
inline CMrPoPoDoc* CMrPoPoView::GetDocument() const
   { return reinterpret_cast<CMrPoPoDoc*>(m_pDocument); }
#endif
