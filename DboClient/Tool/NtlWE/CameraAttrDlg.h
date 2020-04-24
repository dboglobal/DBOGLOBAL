#pragma once
#include "afxwin.h"


// CCameraAttrDlg 대화 상자입니다.

class CCameraAttrDlg : public CDialog
{
	DECLARE_DYNAMIC(CCameraAttrDlg)

public:
	CCameraAttrDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCameraAttrDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CAMERA_ATTR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CString m_strPortalX;
	CString m_strPortalY;
	CString m_strPortalZ;
	CEdit m_editPortalX;
	CEdit m_editPortalY;
	CEdit m_editPortalZ;

	CString m_FOV;
	CString m_DistFromAvatar;
	CString m_strPortalSector;
	CString m_strPortalField;

	CString m_CamVelocity;
	CComboBox m_CamTypeCombo;

public:
	void	SetInfo();
	VOID	SetCurCamMode(RwInt32 IdxCamMode);
	RwInt32 GetCurCamMode() { return m_CamTypeCombo.GetCurSel(); }

	VOID	InitializeWorldData();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
public:
	afx_msg void OnCbnSelchangeCameratypeCombo();
	
	afx_msg void OnBnClickedCamTranslationGo();
	afx_msg void OnBnClickedSectorTranslationGo();
	afx_msg void OnBnClickedFieldTranslationGo();
	
};
