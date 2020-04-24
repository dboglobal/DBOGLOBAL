#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class CPathEngineExportDlg : public CDialog
{
	DECLARE_DYNAMIC(CPathEngineExportDlg)

public:
	CPathEngineExportDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathEngineExportDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_PATH_ENGINE_EXPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	void	InitializeWorldData();	

	afx_msg void OnBnClickedBtnWorldPath();
	afx_msg void OnBnClickedBtnExportPath();

protected:
	CString				m_strWorldPath;
	CString				m_strExportPath;
	CString				m_strWorldName;
	CString				m_strCrossFieldCntOfGroup;
	CString				m_strOverlapSize;
	CString				m_strAgency;
	CString				m_strWorldID;
	sNtlWorldParam		m_NtlWorldParam;
public:
	afx_msg void OnBnClickedBtnPathEngineDataExport();
};