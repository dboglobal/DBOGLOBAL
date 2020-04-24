#pragma once

// gui
#include "gui_particle_doc.h"

// CAttributePane

class CAttributePane : public CWnd
{
	class CAttributeTree : public CTreeCtrl
	{
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	public:
		BOOL		IsName( CString szName, HTREEITEM hSelectedItem );
		
		DECLARE_MESSAGE_MAP()
	
		afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnDestroy();
		afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	};

public:
	CAttributePane();
	virtual ~CAttributePane();

	enum
	{
		ATTR_TREE_GROUP,
		ATTR_TREE_PARTICLE,
	};

	typedef struct _SAttrTreeData
	{
		BYTE	byType;
		VOID*	pData;
	} SAttrTreeData;

// Interface
public:



// Attributes
public:
	CAttributeTree		m_WndTreeCtrl;
	CImageList			m_ilTreeIcons;
	CXTPToolBar			m_wndToolBar;

	void		LoadTree( gui::CParticleDoc* pDoc );

	void		DeleteTreeItemData(HTREEITEM hItem);
	void		DeleteTreeChildItemData(HTREEITEM hItem);
	void		DeleteTreeSelectedItemData(HTREEITEM hItem);
	void		DeleteAllTreeItemData(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	afx_msg void OnAttrGAdd();
	afx_msg void OnUpdateAttrGAdd(CCmdUI* pCmdUI);

	afx_msg void OnAttrPAdd();
	afx_msg void OnUpdateAttrPadd(CCmdUI* pCmdUI);

	afx_msg void OnAttrDel();
	afx_msg	void OnUpdateAttrDel(CCmdUI* pCmdUI);

	afx_msg	void OnAttrPlay();
	afx_msg	void OnUpdateAttrPlay(CCmdUI* pCmdUI);
};


