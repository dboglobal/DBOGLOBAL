#pragma once
#include "afxwin.h"
#include "checkcombobox.h"
#include "NtlWorldFileMemAccessor.h"

// CPalettePerformanceSectorCull 대화 상자입니다.

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

class CPalettePerformanceSectorCull : public CDialog
{
	DECLARE_DYNAMIC(CPalettePerformanceSectorCull)

public:
	CPalettePerformanceSectorCull(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPalettePerformanceSectorCull();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PERFORMANCE_SECTORCULL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void InitializeWorldData();
	afx_msg void OnBnClickedSectorcull();

public:
	CButton m_SectorCull;

public:
	VOID Refresh();

private:
	afx_msg void OnBnClickedAnalyze();
	CCheckComboBox m_SSMCombo;
	CButton m_SSMFieldCheck;
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

class CPalettePerformanceSectorCull : public CDialog, public CNtlWorldFileMemAccessor
{
	DECLARE_DYNAMIC(CPalettePerformanceSectorCull)

public:
	CPalettePerformanceSectorCull(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPalettePerformanceSectorCull();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_PERFORMANCE_SECTORCULL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void InitializeWorldData();
	afx_msg void OnBnClickedSectorcull();

public:
	CButton m_SectorCull;

public:
	VOID Refresh();

private:
	afx_msg void OnBnClickedAnalyze();
	CCheckComboBox m_SSMCombo;
	CButton m_SSMFieldCheck;
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif