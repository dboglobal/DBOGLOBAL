#pragma once


// CSpawnPopupDlg 대화 상자입니다.

class CSpawnPopupDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpawnPopupDlg)

public:
	CSpawnPopupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSpawnPopupDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPAWN_POPUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	CString		m_strFieldNumber;
	CString		m_strIndex;
	CString		m_strNestCount;
	CString		m_strCoolTime;
	CString		m_strWanderRange;
	CString		m_strMoveRange;
	CString		m_strDelayTime;
	CString		m_strPathIndex;
	CString		m_strSpawnGroupIdx;
	CString		m_strPlayScriptIdx;
	CString		m_strPlayScriptSceneIdx;
	CString		m_strAIScriptIdx;
	CString		m_strAIScriptSceneIdx;
	CComboBox	m_cbMoveType;
	afx_msg void OnBnClickedOk();
};
