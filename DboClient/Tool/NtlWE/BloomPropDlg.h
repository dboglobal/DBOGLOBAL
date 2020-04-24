#pragma once


// CBloomPropDlg 대화 상자입니다.

class CBloomPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CBloomPropDlg)

public:
	CBloomPropDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBloomPropDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ATTR_BLOOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CString m_Power;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnDestroy();
};
