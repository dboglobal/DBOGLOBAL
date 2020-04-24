#pragma once


// CGamePropVisibleDlg 대화 상자입니다.

class CGamePropVisibleDlg : public CDialog
{
	DECLARE_DYNAMIC(CGamePropVisibleDlg)

public:
	CGamePropVisibleDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGamePropVisibleDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VISIBLE_PROP };

protected:
	RwUInt32		m_uiFlags;

public:
	void			SetFlags(RwUInt32 uiFlags)	{ m_uiFlags = uiFlags; }
	RwUInt32		GetFlags()					{ return m_uiFlags; }
	RwBool			GetGamePropColor(RwUInt32 uiFlags, RwRGBA* pRGBA);

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual LRESULT	WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL	OnInitDialog();

	void			UpdateFlags();
	void			UpdateUI();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckProp1();
	afx_msg void OnBnClickedCheckProp2();
	afx_msg void OnBnClickedCheckProp3();
	afx_msg void OnBnClickedCheckProp4();
	afx_msg void OnBnClickedCheckProp5();
	afx_msg void OnBnClickedCheckProp6();
	afx_msg void OnBnClickedCheckProp7();
	afx_msg void OnBnClickedCheckProp8();
	afx_msg void OnBnClickedCheckProp9();
	afx_msg void OnBnClickedCheckProp10();
	afx_msg void OnBnClickedCheckProp11();
	afx_msg void OnBnClickedCheckProp12();
	afx_msg void OnBnClickedCheckProp13();
	afx_msg void OnBnClickedCheckProp14();
	afx_msg void OnBnClickedCheckProp15();
	afx_msg void OnBnClickedCheckProp16();
};