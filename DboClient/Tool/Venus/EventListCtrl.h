#pragma once


// CEventListCtrl
#include "VenusGridCtrl.h"

class CEventListCtrl : public CVenusGridCtrl
{
	DECLARE_DYNAMIC(CEventListCtrl)
public:
	CImageList m_image;//Must provide an imagelist

public:
	CEventListCtrl();
	virtual ~CEventListCtrl();

	void	InitializeGrid();

	void	RebuildPlayerEvent();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnCreateEvent();
	afx_msg void OnDeleteEvent();
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLoadEvent();
	afx_msg void OnSaveEvent();
	afx_msg void OnClearEvent();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
};


