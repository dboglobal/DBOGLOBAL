#pragma once


// CBotPane

class CBotPane : public CWnd
{
	class CBotTree : public CTreeCtrl
	{
		LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
	public:
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	public:
		afx_msg void OnDestroy();
	};

	DECLARE_DYNAMIC(CBotPane)

public:
	CBotPane();
	virtual ~CBotPane();

public:

	enum 
	{
		BOT_TREE_FOLDER,
		BOT_TREE_DATA
	};

	typedef struct _SBotTreeData
	{
		BYTE byType;
		void *pData;
	}SBotTreeData;

	std::list<SBotTreeData*> m_listTreeData;

	CXTPToolBar m_wndToolBar;
	CImageList	m_ilTreeIcons;
	CBotTree	m_wndTreeCtrl;

	HTREEITEM	m_hMobItem;	
	HTREEITEM	m_hMobLevelItem[10];	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnDestroy();

	afx_msg void OnBotCreate();
	afx_msg void OnUpdateBotCreate(CCmdUI* pCmdUI);
};


