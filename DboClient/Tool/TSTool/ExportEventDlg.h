#pragma once


// CExportEventDlg 대화 상자입니다.

class CExportEventDlg : public CDialog
{
	DECLARE_DYNAMIC(CExportEventDlg)

public:
	CExportEventDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CExportEventDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EXPORT_EVENTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedTablePath();
	afx_msg void OnBnClickedTsPath();

	virtual void OnOK();

public:
	CString m_strCurPath;

	CString m_strTblPath;
	CString m_strTSPath;
};
