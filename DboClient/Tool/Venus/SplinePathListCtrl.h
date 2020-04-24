#pragma once


// CSplinePathListCtrl
#include "VenusGridCtrl.h"

class CSplinePathListCtrl : public CVenusGridCtrl
{
	DECLARE_DYNAMIC(CSplinePathListCtrl)

public:
	CImageList m_image;//Must provide an imagelist

public:
	CSplinePathListCtrl();
	virtual ~CSplinePathListCtrl();

	void	InitializeGrid();

	void	RebuildSplinePath();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnCreatePath();
	afx_msg void OnDeletePath();
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


