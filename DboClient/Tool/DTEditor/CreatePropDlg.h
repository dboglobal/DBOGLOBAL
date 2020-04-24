#pragma once
#include "afxwin.h"


// CCreatePropDlg 대화 상자입니다.

enum eCreateType
{
	CREATE_SPLINE,
	CREATE_DIRECTION_NODE,

	INVALID_CREATE_TYPE
};

class CCreatePropDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreatePropDlg)

public:
	CCreatePropDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCreatePropDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ADD_PROP };

public:
	int		m_nIndex;
	int		m_nCreateType;
	CString	m_strString;

public:
	void	SetCreateType(eCreateType byType);

	int		GetSelectedIndex(void) const;
	CString	GetSelectedString();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_PropList;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	afx_msg void OnLbnSelchangeAddPropList();
};
