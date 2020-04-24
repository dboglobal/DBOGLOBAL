#pragma once
#include "afxwin.h"

#include "resource.h"

enum EViewerControlType
{
	EWORLD_CREATE,
	EWORLD_CHANGE,
	EAVATAR_RACE,
	EAVATAR_TELEPORT,
	EAVATAR_SPEED
};

// CViewControlDlg 대화 상자입니다.

class CViewControlDlg : public CDialog
{
	DECLARE_DYNAMIC(CViewControlDlg)

public:

	EViewerControlType m_eType;

	void	InitRace(void);
	void	InitClass(RwUInt8 byRace, RwUInt8 bySelClass);
	void	InitGender(RwUInt8 byRace, RwUInt8 bySelGender);

public:
	CViewControlDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CViewControlDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEWER_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnEnChangeEditTeleportX();
public:
	CComboBox m_comboWorld;
public:
	CComboBox m_comboRace;
public:
	CComboBox m_comboClass;
public:
	CComboBox m_comboGender;
public:
	afx_msg void OnBnClickedOk();
public:
	float m_fTeleportX;
	float m_fTeleportZ;
	float m_fAvatarSpeed;
	float m_fWorldMinX;
	float m_fWorldMinZ;
	float m_fWorldMaxX;
	float m_fWorldMaxZ;

public:
	afx_msg void OnCbnSelchangeComboAvatarRace();
public:
	afx_msg void OnBnClickedCancel();
};
