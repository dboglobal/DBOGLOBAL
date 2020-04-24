#pragma once

// core

#include "NtlSerializer.h"

class CNtlDTProp;
class CNtlDTContainer;

#define OBJECT_PANE_SERIALIZE_PARENTRNAME		0x01
#define OBJECT_PANE_SERIALIZE_FOLDERNAME		0x02
#define OBJECT_PANE_SERIALIZE_OBJECTNAME		0x03
#define OBJECT_PANE_SERIALIZE_TREEITEM			0x04
#define OBJECT_PANE_SERIALIZE_END				0xff


// CObjectPane

class CObjectPane : public CWnd
{
	class CObjectTree : public CTreeCtrl
	{
		LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
	public:
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	public:
		afx_msg void OnDestroy();
	};

// Construction
public:

	CObjectPane();
	virtual ~CObjectPane();

// Attributes
public:

	enum 
	{
		OBJ_TREE_FOLDER,
		OBJ_TREE_OBJECT
	};

	typedef struct _SObjTreeData
	{
		BYTE byType;
		void *pData;
	}SObjTreeData;
	
	CXTPToolBar m_wndToolBar;
	CObjectTree m_wndTreeCtrl;
	CImageList m_ilTreeIcons;
	CXTPCommandBars* m_pActiveCommandBars;


	typedef std::map<std::string, HTREEITEM> MapItemMapping;
	MapItemMapping m_mapItemMapping;

public:

	//////////////////////////////////////////
	// tree ui edit
	//////////////////////////////////////////

	void	LoadReset(void);

	BOOL	IsTreeItemFolder(HTREEITEM hItem);
	BOOL	IsTreeItemObject(HTREEITEM hItem);
	void*	GetTreeItemData(HTREEITEM hItem);

	BOOL	IsExistChildName(HTREEITEM hItem, CString& strFindName);
	BOOL	IsExistName(CString& strFindName);

	void	DeleteTreeItem(HTREEITEM hItem);
	void	DeleteTreeItemData(HTREEITEM hItem);

	void	DeleteChildTreeItemData(HTREEITEM hItem);
	void	DeleteAllTreeItemData(void);

	HTREEITEM	FindItemMapping(const CHAR *pKey);

	//////////////////////////////////////////
	// spline edit
	//////////////////////////////////////////

	CNtlDTProp* CreateSplineProp(int nShape);

	//////////////////////////////////////////
	// load and save
	//////////////////////////////////////////

	void	LoadItem(CNtlSerializer& s);
	void	SaveItem(CNtlSerializer& s, HTREEITEM hItem, HTREEITEM hParentItem);
	void	SaveChildItem(CNtlSerializer& s, HTREEITEM hItem);

	RwBool	Load(CNtlSerializer& s);
	RwBool	Save(CNtlSerializer& s);

	void	ImportItem(CNtlSerializer& s, CNtlDTContainer *pDTContainer);
	RwBool	Import(CNtlSerializer& s, CNtlDTContainer *pDTContainer);

// Operations
public:
	void Refresh();
	void RefreshItem(HTREEITEM hItem);

	int BuildToolbarsTree(HTREEITEM hItem, CXTPCommandBar* pCommandBar, BOOL bCheck);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectPane)
	//}}AFX_VIRTUAL



protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnObjPaneAdd();
	afx_msg void OnUpdateObjPaneAdd(CCmdUI* pCmdUI);
	afx_msg void OnObjPaneDelete();
	afx_msg void OnUpdateObjPaneDelete(CCmdUI* pCmdUI);
	afx_msg void OnObjPaneFolderAdd();
	afx_msg void OnUpdateObjPaneFolderAdd(CCmdUI* pCmdUI);
	afx_msg void OnObjPaneRebuild();
	afx_msg void OnUpdateObjPaneRebuild(CCmdUI* pCmdUI);
	afx_msg void OnObjPaneRename();
	afx_msg void OnUpdateObjPaneRename(CCmdUI* pCmdUI);
	afx_msg void OnObjPaneImportFile();
	afx_msg void OnUpdateObjPaneImportFile(CCmdUI* pCmdUI);
public:
	afx_msg void OnDestroy();
};


