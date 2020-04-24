#pragma once


// COutputPane

class COutputPane : public CWnd
{
	DECLARE_DYNAMIC(COutputPane)

	class COutputEdit : public CEdit
	{
		LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
	};

public:
	COutputPane();
	virtual ~COutputPane();

// Attributes
public:

	COutputEdit m_wndEdit;

	/*
	void Build();
	void BuildSuccess();
	void BuildErrorReport(const char *pError);
	void BuildError(BYTE byErrorCount);
	
	void Compile();
	void CompileSuccess();
	void CompileErrorReport(const char *pError);
	void CompileError(BYTE byErrorCount);
	*/

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


