#pragma once
#include "afxcmn.h"


// CPaletteSE 대화 상자입니다.

class CPaletteSE : public CDialog
{
	DECLARE_DYNAMIC(CPaletteSE)

public:
	CPaletteSE(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteSE();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ATTR_SE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl	m_SETree;

public:
	void		CreateLeafNode(const char *pName, HTREEITEM hTreeItem);
	HTREEITEM	GetItemByName(char* szItemName, HTREEITEM hItem);
	RwBool		GetCurBGMFileName(RwChar _FileName[32]);
	RwBool		GetCurCategoryName(RwChar _CategoryName[32]);

public:
	void InitializeWorldData();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTvnSelchangedSeTree(NMHDR *pNMHDR, LRESULT *pResult);

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
};
