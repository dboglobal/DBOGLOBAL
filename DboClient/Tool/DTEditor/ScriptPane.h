#pragma once

// CScriptPane

class CScriptPane : public CWnd
{
	DECLARE_DYNAMIC(CScriptPane)

	class CScriptEdit : public CEdit
	{
		LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
	public:
		DECLARE_MESSAGE_MAP()

		HACCEL m_hAccel;
	
	public:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
	public:
		afx_msg void OnEditCopy();
	public:
		afx_msg void OnEditPaste();
	public:
		afx_msg void OnEditUndo();
	public:
		afx_msg void OnEditCut();
	};

	class COutputEdit : public CEdit
	{
		LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
	};

public:
	CScriptPane();
	virtual ~CScriptPane();

// Attributes
public:

	CXTPToolBar m_wndToolBar;
	CScriptEdit m_wndEdit;
	COutputEdit m_wndOutEdit;


	void		Build();
	void		BuildSuccess();
	void		BuildErrorReport(const char *pError);
	void		BuildError(BYTE byErrorCount);
	
	void		Compile();
	void		CompileSuccess();
	void		CompileErrorReport(const char *pError);
	void		CompileError(BYTE byErrorCount);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnScriptPaneBuild();
	afx_msg void OnUpdateScriptPaneBuild(CCmdUI* pCmdUI);
	afx_msg void OnScriptPaneBuildStop();
	afx_msg void OnUpdateScriptPaneBuildStop(CCmdUI* pCmdUI);
	afx_msg void OnScriptPaneCopy();
	afx_msg void OnUpdateScriptPaneCopy(CCmdUI* pCmdUI);
	afx_msg void OnScriptPanePaste();
	afx_msg void OnUpdateScriptPanePaste(CCmdUI* pCmdUI);
	afx_msg void OnScriptPaneCompileAll();
	afx_msg void OnUpdateScriptPaneCompileAll(CCmdUI* pCmdUI);
public:
//	afx_msg void OnEditCopy();
public:
//	afx_msg void OnEditUndo();
public:
//	afx_msg void OnEditPaste();
public:
//	afx_msg void OnEditCut();
};


