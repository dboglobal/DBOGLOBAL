#pragma once
#include "afxwin.h"


// CFileNewDlg 대화 상자입니다.

class CFileNewDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileNewDlg)

public:
	CFileNewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFileNewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FILE_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	
private:
	void Update();

public:
	afx_msg void OnEnChangeWorldHeightFieldNum();
	afx_msg void OnEnChangeWorldSectorTileSize();
	afx_msg void OnEnChangeWorldSectorTileNum();
	afx_msg void OnEnChangeWorldFieldSectorNum();
	afx_msg void OnEnChangeWorldTexAddr();
	afx_msg void OnEnChangeWorldBlockSize();
	afx_msg void OnBnClickedLoadRaw();

	afx_msg void OnCbnSelchangeSkyType();
	afx_msg void OnCbnSelchangeWorldType();
	afx_msg void OnBnClickedOk();

public:
	CString				m_RAWFileName;
	CComboBox			m_SkyTypeIdxCombo;
	CComboBox			m_WorldTypeCombo;

	CEdit m_WorldHeightfieldNum;
	CEdit m_WorldSectorTileSize;
	CEdit m_WorldSectorTileNum;
	CEdit m_WorldFieldSectorNum;
	CEdit m_WorldTexAddr;
	CEdit m_WorldBlockSize;

	bool m_IsDialogInitialize;	
};