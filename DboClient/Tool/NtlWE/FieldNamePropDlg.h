#pragma once


// CFieldNamePropDlg 대화 상자입니다.

class CFieldNamePropDlg : public CDialog
{
	DECLARE_DYNAMIC(CFieldNamePropDlg)

public:
	CFieldNamePropDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFieldNamePropDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ATTR_FIELDNAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	CString m_CurMapNameCode;
public:
	CString m_MapNameCodeInput;
};
