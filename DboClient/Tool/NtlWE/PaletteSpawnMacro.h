#pragma once
#include "afxwin.h"


// CPaletteSpawnMacro 대화 상자입니다.

class CPaletteSpawnMacro : public CDialog
{
	DECLARE_DYNAMIC(CPaletteSpawnMacro)

public:
	CPaletteSpawnMacro(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteSpawnMacro();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPAWN_MACRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_CoolTime;
	CComboBox m_MoveType;
	CString m_WanderRange;
	CString m_MoveRange;
	CString m_DelayTime;
public:
	afx_msg void OnBnClickedCreate();
public:
	afx_msg void OnBnClickedDelete();
public:
	CListBox m_SpawnMacro;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	virtual BOOL OnInitDialog();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLbnSelchangeSpawnMacro();

public:
	VOID RefreshSMIListCtrl();
};
