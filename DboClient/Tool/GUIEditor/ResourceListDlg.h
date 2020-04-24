#pragma once

// CResourceListDlg 대화 상자입니다.
#include "Control/PropTree.h"

class CResourceListDlg : public CDialog
{
	DECLARE_DYNCREATE(CResourceListDlg)

public:
	CResourceListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CResourceListDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RESOURCELISTDLG };
	
	enum PROPERTY_ID
	{
		PROPERTY_NONE = -1,

		PROPERTY_CAPTION = 0,
	};

	void	DisplayList();					  // List를 출력합니다.
	INT		GetFocusID();					

protected:
	void	AddItem( CString strCaption, INT nIndex, CPropTreeItem* pRoot );	
	void	ShowAttribute( CPropTreeItem* pItem );

protected:
	CPropTree	m_PropertyTree;
	BOOL		m_bInitialized;
		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedRemoveResourcelist();
	afx_msg void OnBnClickedSaveResourcelist();
	afx_msg void OnBnClickedClearResourcelist();

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
