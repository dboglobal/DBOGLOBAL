#pragma once


// CSpawnGroupBasedPropDlg dialog

class CSpawnGroupBasedPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpawnGroupBasedPropDlg)

public:
	CSpawnGroupBasedPropDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSpawnGroupBasedPropDlg();

// Dialog Data
	enum { IDD = IDD_SPAWN_GROUP_PROP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	CString m_strSGroupIdx;
	CString m_strSPlayScriptIdx;
	CString m_strSPlayScriptSceneIdx;
	CString m_strSAIScriptIdx;
	CString m_strSAIScriptSceneIdx;

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();	
};
