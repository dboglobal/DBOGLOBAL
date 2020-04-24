// 2DParticleEditorView.h : interface of the CMy2DParticleEditorView class
//


#pragma once


class CMy2DParticleEditorView : public CView
{
protected: // create from serialization only
	CMy2DParticleEditorView();
	DECLARE_DYNCREATE(CMy2DParticleEditorView)

// Attributes
public:
	CMy2DParticleEditorDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMy2DParticleEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

public:
	void	SetViewCenterPbOffset();

	static  CMy2DParticleEditorView* m_pInstance;
	static	CMy2DParticleEditorView* GetInstance()	{ return m_pInstance; }

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // debug version in 2DParticleEditorView.cpp
inline CMy2DParticleEditorDoc* CMy2DParticleEditorView::GetDocument() const
   { return reinterpret_cast<CMy2DParticleEditorDoc*>(m_pDocument); }
#endif

static CMy2DParticleEditorView* GetEditorView()
{
	return CMy2DParticleEditorView::GetInstance();
}