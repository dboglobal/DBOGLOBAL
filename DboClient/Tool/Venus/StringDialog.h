#pragma once


// CStringDialog 대화 상자입니다.
#include <string>

class CStringDialog : public CDialog
{
	DECLARE_DYNAMIC(CStringDialog)

public:
	std::string			m_strTitle;
	std::string			m_strEdit;

public:
	CStringDialog(LPCTSTR strTitle, LPCTSTR strEdit = "noname", CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStringDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_STRING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strName;
};
