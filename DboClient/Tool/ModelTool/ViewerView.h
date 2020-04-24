#pragma once


// CViewerView view

class CViewerView : public CView
{
	DECLARE_DYNCREATE(CViewerView)

protected:
	CViewerView();           // protected constructor used by dynamic creation
	virtual ~CViewerView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


