
#pragma once


class CPropTree;

// CPropTreeList

class CPropTreeList : public CWnd
{
	DECLARE_DYNAMIC(CPropTreeList)

public:
	static BOOL ApplyPropTreeSetFocus;

protected:
	// CPropTree class that this class belongs
	CPropTree*		m_pProp;

	// bitmap back buffer for flicker free drawing
	CBitmap			m_BackBuffer;

	// current diminsions of the back buffer
	CSize			m_BackBufferSize;

	// splitter pevious position
	LONG			m_nPrevCol;

	// TRUE if we are dragging the splitter
	BOOL			m_bColDrag;

public:
	CPropTreeList();
	virtual ~CPropTreeList();

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void SetPropOwner(CPropTree* pProp);

// Operations
public:
	void UpdateResize();

protected:
	void RecreateBackBuffer(int cx, int cy);
	void CheckVisibleFocus();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


