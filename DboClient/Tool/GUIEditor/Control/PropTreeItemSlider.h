#pragma once


// CPropTreeItemSlider
#include "N3Slider.h"
#include "PropTreeItem.h"

class CPropTreeItemSlider : public CWnd, public CPropTreeItem
{
	DECLARE_DYNAMIC(CPropTreeItemSlider)

protected:
	
	BOOL		m_FloatingPointMode;
	FLOAT		m_FloatingData;

	CString		m_ValueString;
	int			m_iMin;
	int			m_iMax;
	int			m_iPos;
	COLORREF	m_clrFrom;
	COLORREF	m_clrTo;

public:
	CPropTreeItemSlider();
	virtual ~CPropTreeItemSlider();

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

public:
	void SetRange(int iMin, int iMax);
	void SetRange(FLOAT fMin, FLOAT fMax);
	void SetColors(COLORREF From, COLORREF To);
	void SetPos(int nPos);
	int  GetRangeMin()								{ return m_iMin; };
	int  GetRangeMax()								{ return m_iMax; };
	int  GetPos()									{ return m_iPos; };
	int  GetValue()									{ return m_iMin+m_iPos; }
	int  GetRangeSize()								{ return GetRangeMax()-GetRangeMin(); };
	void SetFloatingPointMode( BOOL mode )			{ m_FloatingPointMode = mode; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnClose();
};


