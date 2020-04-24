#pragma once


// CPropTreeItemColor
#include "PropTreeItem.h"

class CPropTreeItemColor : public CWnd, public CPropTreeItem
{
	DECLARE_DYNAMIC(CPropTreeItemColor)
protected:
	COLORREF			m_cColor;
	COLORREF			m_cPrevColor;
	CRect				m_rcButton;
	LONG				m_nSpot;
	BOOL				m_bButton;
	BOOL				m_bInDialog;

	static COLORREF*	s_pColors;

public:
	CPropTreeItemColor();
	virtual ~CPropTreeItemColor();

// Attributes
public:
	// The attribute area needs drawing
	virtual void DrawAttribute(CDC* pDC, const RECT& rc);

	// Retrieve the item's attribute value
	virtual LPARAM GetItemValue();

	// Set the item's attribute value
	virtual void SetItemValue(LPARAM lParam);

	// Called when attribute area has changed size
	virtual void OnMove();

	// Called when the item needs to refresh its data
	virtual void OnRefresh();

	// Called when the item needs to commit its changes
	virtual void OnCommit();

	// Called to activate the item
	virtual void OnActivate();

	static void SetDefaultColorsList(COLORREF* pColors);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


