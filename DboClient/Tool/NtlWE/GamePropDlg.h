#pragma once


// CGamePropDlg 대화 상자입니다.

class CGamePropDlg : public CDialog
{
	DECLARE_DYNAMIC(CGamePropDlg)

public:
	CGamePropDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGamePropDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ATTR_GAMEPROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

protected:
	CButton					m_btnCheckProp1;
	CButton					m_btnCheckProp2;
	CButton					m_btnCheckProp3;
	CButton					m_btnCheckProp4;
	CButton					m_btnCheckProp5;
	CButton					m_btnCheckProp6;
	CButton					m_btnCheckProp7;
	CButton					m_btnCheckProp8;
	CButton					m_btnCheckProp9;
	CButton					m_btnCheckProp10;
	CButton					m_btnCheckProp11;
	CButton					m_btnCheckProp12;
	CButton					m_btnCheckProp13;
	CButton					m_btnCheckProp14;
	CButton					m_btnCheckProp15;
	CButton					m_btnCheckProp16;

protected:
	void					SetPropertyData(RwUInt32 uiProperty);
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
