#pragma once
#include <list>
#include "afxcmn.h"
#include "afxwin.h"
#include "EditListCtrl.h"
#include "NtlNaviDefine.h"

// CDlgPathExport 대화 상자입니다.

class CDlgPathExport : public CDialog
{
	DECLARE_DYNAMIC(CDlgPathExport)

public:
	CDlgPathExport(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPathExport();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EXPORT_PATHDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL GetExportList( mapdef_ExportList& list );

protected:
	CEditListCtrl		m_editListCtrlExport;
	mapdef_ExportList	m_mapExportList;

public:
	afx_msg void OnBnClickedBtnPathExportAll();
	afx_msg void OnBnClickedBtnPathExportSelect();
};
