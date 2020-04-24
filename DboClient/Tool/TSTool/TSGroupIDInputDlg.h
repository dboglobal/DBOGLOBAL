#pragma once


// CTSGroupIDInputDlg 대화 상자입니다.

class CTSGroupIDInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CTSGroupIDInputDlg)

public:
	CTSGroupIDInputDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTSGroupIDInputDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GROUP_ID_INPUT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_ctrGroupID;

	DWORD m_dwMin;
	DWORD m_dwMax;
};
