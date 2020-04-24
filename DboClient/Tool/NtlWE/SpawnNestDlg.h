#pragma once


// CSpawnNestDlg 대화 상자입니다.

class CSpawnNestDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpawnNestDlg)

public:
	CSpawnNestDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSpawnNestDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPAWN_NEST_CNT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_NestMemberCnt;
public:
	virtual BOOL OnInitDialog();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	CString m_StrPathIndex;
};
