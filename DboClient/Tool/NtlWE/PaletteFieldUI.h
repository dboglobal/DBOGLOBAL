#pragma once
#include "afxwin.h"


class CPaletteSkyFloatingEntityDlg;

// CPaletteFieldUI dialog

class CPaletteFieldUI : public CDialog
{
	DECLARE_DYNAMIC(CPaletteFieldUI)

public:
	CPaletteFieldUI(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPaletteFieldUI();

// Dialog Data
	enum { IDD = IDD_FIELDUI };

public:
	void InitializeWorldData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedLoadFieldmap();
	afx_msg void OnBnClickedLoadMergemap();
	afx_msg void OnBnClickedFieldVisible();
	afx_msg void OnBnClickedMergeVisible();
	afx_msg void OnBnClickedLinkWorld();
	afx_msg void OnBnClickedMergeHeightfield6();
	afx_msg void OnBnClickedMergeHeightfield();
	afx_msg void OnBnClickedMergeHeightfield2();
	afx_msg void OnBnClickedMergeHeightfield3();
	afx_msg void OnBnClickedMergeHeightfield4();
	afx_msg void OnBnClickedMergeHeightfield5();
	afx_msg void OnBnClickedBtnMergeSpawnDst();
	afx_msg void OnBnClickedBtnMergeSpawnSrc();
	afx_msg void OnBnClickedMergeMapname();
	afx_msg void OnBnClickedMergeBloom();
	afx_msg void OnBnClickedSkyPalette();

public:
	VOID	UpdateMergeAll();
	RwBool	GetWorldMergeStateNothing();
	RwBool	GetSpawnMergeStateNothing();
	VOID	SetCheckSkyPaletteShow(RwBool _ShowFlag);
	VOID	Hide();

public:
	//CPaletteSkyFloatingEntityDlg* m_pPaletteSkyFloatingEntitydlg;

public:
	CString m_MergeSpawnDstName;
	CString m_MergeSpawnDstFullName;
	CString m_MergeSpawnSrcName;
	CString m_MergeSpawnSrcFullName;
	CButton m_MergeSpawn;
	CButton m_MergeSpawnPath;

	CButton m_LoadMinimapBtn;
	CButton m_FieldShowBtn;

	CButton m_MergeBtn;
	CButton m_LoadMergeMinimapBtn;

	CButton m_M0;
	CButton m_M1;
	CButton m_M2;
	CButton m_M3;
	CButton m_M4;
	CButton m_M5;
	CButton m_M6;
	CButton m_MergeAll;

	CButton m_SkyPaletteShow;
	
	CEdit m_MergeWorld;
	CString m_MergeWorldPathName;	
};
