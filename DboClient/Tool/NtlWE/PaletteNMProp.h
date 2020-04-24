#pragma once
#include "afxwin.h"

#include "NtlWorldPropUtil.h"

// CPaletteNMProp dialog

class CNtlWorldPropNMapController;

class CPaletteNMProp : public CDialog
{
	DECLARE_DYNAMIC(CPaletteNMProp)

public:
	CPaletteNMProp(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPaletteNMProp();

// Dialog Data
	enum { IDD = IDD_NORMAL_MAP_PROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	CNtlWorldPropToolData m_WorldPropToolData;

public:
	CButton m_PropChk00;
	CButton m_PropChk01;
	CButton m_PropChk02;
	CButton m_PropChk03;
	CButton m_PropChk04;
	CButton m_PropChk05;
	CButton m_PropChk06;
	CButton m_PropChk07;
	CButton m_PropChk08;
	CButton m_PropChk09;
	CButton m_PropChk10;
	CButton m_PropChk11;
	CButton m_PropChk12;
	CButton m_PropChk13;
	CButton m_PropChk14;
	CButton m_PropChk15;

	CString m_TName00;
	CString m_TName01;
	CString m_TName02;
	CString m_TName03;
	CString m_TName04;
	CString m_TName05;
	CString m_TName06;
	CString m_TName07;
	CString m_TName08;
	CString m_TName09;
	CString m_TName10;
	CString m_TName11;
	CString m_TName12;
	CString m_TName13;
	CString m_TName14;
	CString m_TName15;

	RwBool m_NMapProp[16];

public:
	afx_msg void OnBnClickedNmpLoad00();
	afx_msg void OnBnClickedNmpLoad01();
	afx_msg void OnBnClickedNmpLoad02();
	afx_msg void OnBnClickedNmpLoad03();
	afx_msg void OnBnClickedNmpLoad04();
	afx_msg void OnBnClickedNmpLoad05();
	afx_msg void OnBnClickedNmpLoad06();
	afx_msg void OnBnClickedNmpLoad07();
	afx_msg void OnBnClickedNmpLoad08();
	afx_msg void OnBnClickedNmpLoad09();
	afx_msg void OnBnClickedNmpLoad10();
	afx_msg void OnBnClickedNmpLoad11();
	afx_msg void OnBnClickedNmpLoad12();
	afx_msg void OnBnClickedNmpLoad13();
	afx_msg void OnBnClickedNmpLoad14();
	afx_msg void OnBnClickedNmpLoad15();

//	afx_msg void OnBnClickedNmpApply();
	afx_msg void OnBnClickedNmpInit();

public:
	void	InitializeWorldData();
	void	LoadPropTex(RwInt32 IdxCurProp);
	RwBool*	GetNMapProp() { return m_NMapProp; }
	RwBool	GetNMapPropActivated();
	DWORD	GetCurProp();
	RwBool	IsMatched(DWORD& _BitProp);
	VOID	UpdateNMapPropData();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	CNtlWorldPropNMapController*	m_pController;

private:
	
	afx_msg void OnBnClickedNmapprop00();
	afx_msg void OnBnClickedNmapprop01();
	afx_msg void OnBnClickedNmapprop02();
	afx_msg void OnBnClickedNmapprop03();
	afx_msg void OnBnClickedNmapprop04();
	afx_msg void OnBnClickedNmapprop05();
	afx_msg void OnBnClickedNmapprop06();
	afx_msg void OnBnClickedNmapprop07();
	afx_msg void OnBnClickedNmapprop08();
	afx_msg void OnBnClickedNmapprop09();
	afx_msg void OnBnClickedNmapprop10();
	afx_msg void OnBnClickedNmapprop11();
	afx_msg void OnBnClickedNmapprop12();
	afx_msg void OnBnClickedNmapprop13();
	afx_msg void OnBnClickedNmapprop14();
	afx_msg void OnBnClickedNmapprop15();
	afx_msg void OnBnClickedExportNmp();
};
