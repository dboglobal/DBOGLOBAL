#pragma once


class CFlatCommandBarsTheme : public CXTPNativeXPTheme
{
public:
	CFlatCommandBarsTheme()
	{
		m_bSelectImageInPopupBar = TRUE;
		m_bThickCheckMark = TRUE;
	}

	void FillDockBar(CDC* pDC, CXTPDockBar* pBar)
	{
		pDC->FillSolidRect(CXTPClientRect(pBar), GetXtremeColor(COLOR_3DFACE));
	}

	void RefreshMetrics()
	{
		CXTPNativeXPTheme::RefreshMetrics();
		
		m_arrColor[COLOR_3DFACE].SetStandardValue(GetSysColor(COLOR_3DFACE));

	}

	void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
	{
		if (IsDockingPosition(pBar->GetPosition()))
		{
			CRect rc;
			pBar->GetClientRect(&rc);			
			pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
			return;
		}

		CXTPNativeXPTheme::FillCommandBarEntry(pDC, pBar);

	}
};


class CDockingPaneAdobeTheme : public CXTPDockingPaneDefaultTheme
{
public:
	CDockingPaneAdobeTheme();
	virtual ~CDockingPaneAdobeTheme();
};


class CDockingPaneAdobeContextWnd : public CWnd
{
public:
	CDockingPaneAdobeContextWnd();
protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXTPDockingPaneContextAlphaWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE
public:
	BOOL m_bAttachContext;
	BOOL m_bFloatContext;

	friend class CXTPDockingPaneContext;
};


class CDockingPaneAdobeContext : public CXTPDockingPaneContext
{
public:
	CDockingPaneAdobeContext();

	void InitLoop();
	void CancelLoop();
	void DrawFocusRect(BOOL bRemoveRect = FALSE);
	void FindContainer(CPoint pt);

	CDockingPaneAdobeContextWnd m_wndContext;

};
