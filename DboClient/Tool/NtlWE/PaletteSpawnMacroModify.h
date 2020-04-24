#pragma once
#include "afxwin.h"


// CPaletteSpawnMacroModify 대화 상자입니다.

class CPaletteSpawnMacroModify : public CDialog
{
	DECLARE_DYNAMIC(CPaletteSpawnMacroModify)

public:
	CPaletteSpawnMacroModify(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteSpawnMacroModify();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPAWN_MACRO_MODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	CString m_StrCoolTime;
	CString m_StrWanderRange;
	CString m_StrMoveRange;
	CString m_StrDelayTime;
public:
	CComboBox m_MoveTypeCombo;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeMovetype();
};
