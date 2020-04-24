#pragma once


// CSkillPane

class CSkillPane : public CWnd
{
	class CSkillTree : public CTreeCtrl
	{
		LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
	public:
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	public:
		afx_msg void OnDestroy();
	};

	DECLARE_DYNAMIC(CSkillPane)

public:
	CSkillPane();
	virtual ~CSkillPane();

public:

	enum 
	{
		SKILL_TREE_FOLDER,
		SKILL_TREE_DATA
	};

	typedef struct _SSkillTreeData
	{
		BYTE byType;
		void *pData;
	}SSkillTreeData;

	std::list<SSkillTreeData*> m_listTreeData;

	CXTPToolBar m_wndToolBar;
	CImageList	m_ilTreeIcons;
	CSkillTree	m_wndTreeCtrl;

	HTREEITEM	m_hHTBItem;	

	void		InitSkill(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnSkillUse();
	afx_msg void OnUpdateSkillUse(CCmdUI* pCmdUI);
};


