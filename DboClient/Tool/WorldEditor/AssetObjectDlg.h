#pragma once

#include "AssetObjectTreeCtrl.h"
#include "Resource.h"


// CAssetObjectDlg 대화 상자입니다.

class CAssetObjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CAssetObjectDlg)

public:

	CAssetObjectTreeCtrl	m_TreeCtrl;

public:
	CAssetObjectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAssetObjectDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ASSETOBJECTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
