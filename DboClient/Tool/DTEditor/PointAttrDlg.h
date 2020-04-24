#pragma once
#include "afxwin.h"
#include "POintCustomEditDlg.h"

class CPointEditDlg;

enum
{
	POINT_ATTR_CAMERA,
	POINT_ATTR_AVATAR
};


// CPointAttrDlg 대화 상자입니다.

class CPointAttrDlg : public CDialog
{
	DECLARE_DYNAMIC(CPointAttrDlg)

public:
	CPointAttrDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPointAttrDlg();

	INT						m_nEditType;
	CPointEditDlg			*m_pEditDlg;
	CPointCustomEditDlg		m_CustomDlg;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_POINTATTRIBUTE };

public:

	void		OnPropUpdate(void);
	void		OnPropSelectUpdate(int nSx, int nEx);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPointattrPointPos();
public:
	CEdit m_editOffsetY;
public:
	CButton m_btnCheckPos;
public:
	CButton m_btnCheckDir;
public:
	CButton m_btnCameraMovePointClick;
public:
	CButton m_btnCameraColli;
public:
	CButton m_btnAvatarColli;
public:
	CComboBox m_comboType;
public:
	float m_fOffsetY;

public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedPointattrAddPointPos();
public:
	afx_msg void OnBnClickedPointattrPointSelectedInterpolation();

public:
	afx_msg void OnCbnSelchangePointattrComboType();
public:
	afx_msg void OnEnChangePointattrCameraSpeed();
public:
	afx_msg void OnBnClickedPointattrCheckPos();
public:
	afx_msg void OnBnClickedPointattrCheckDir();

public:
	afx_msg void OnEnChangePointattrPointBox();
public:
	afx_msg void OnBnClickedPointattrCheckCameraMove();
public:
	afx_msg void OnBnClickedPointattrCheckCameraCollision();
public:
	afx_msg void OnBnClickedPointattrCheckAvatarCollision();
public:
	afx_msg void OnBnClickedPointattrResetAvatarDir();
public:
	afx_msg void OnBnClickedPointCustomDialog();
};
