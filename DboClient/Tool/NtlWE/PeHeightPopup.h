#pragma once


// CPeHeightPopup 대화 상자입니다.

class CPeHeightPopup : public CDialog
{
	DECLARE_DYNAMIC(CPeHeightPopup)

public:
	CPeHeightPopup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPeHeightPopup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PE_HEIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual LRESULT	WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CString m_strValue;
	RwReal	m_fValue;
	afx_msg void OnBnClickedOk();
};
