#pragma once
#include "afxcmn.h"


// CAnimAssetDlg 대화 상자입니다.

class CAnimationTreeDialog : public CDialog
{
	DECLARE_DYNAMIC(CAnimationTreeDialog)

public:
	CAnimationTreeDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimationTreeDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ANIMASSETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_AnimAssetTreeCtrl;
	
	void UpdateTree(CDboCharacterApplication *pDboApp);
	
	
	void CAnimationTreeDialog::DeleteToolAnimData(HTREEITEM hItem);
	void DeleteRecursiveToolAnimData(HTREEITEM hItem);

	afx_msg void OnTvnSelchangedTree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
};
