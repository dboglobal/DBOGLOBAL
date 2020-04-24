#pragma once

#include "Control/PropTree.h"
#include "resource.h"

// CSurfaceListDlg 대화 상자입니다.

class CSurfaceListDlg : public CDialog
{
	DECLARE_DYNCREATE(CSurfaceListDlg)

public:
	CSurfaceListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSurfaceListDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SURFACELISTDLG };

	enum PROPERTY_ID
	{
		PROPERTY_NONE = -1,

		PROPERTY_CAPTION = 0,
	};

	void	DisplayList();			// List출력.
	INT		GetFocusID();		// 0 이하면 없음.
		
protected:
	CPropTree	m_PropertyTree;
	BOOL		m_bInitialized;

protected:
	// Internal Function
	void	AddItem( CString strCaption, INT nIndex, CPropTreeItem* pRoot );	
	void	ShowAttribute( CPropTreeItem* pItem );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedRemoveSurfacelist();
	afx_msg void OnBnClickedSaveSurfacelist();
	afx_msg void OnBnClickedClearSurfacelist();

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
