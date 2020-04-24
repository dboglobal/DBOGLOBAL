#pragma once


#include "afxcmn.h"
#include "Bookmarker.h"
#include "afxwin.h"


// CBookmarkDlg 대화 상자입니다.

class CBookmarkDlg : public CDialog
{
	DECLARE_DYNAMIC(CBookmarkDlg)

	enum eBOOKMARK_LIST
	{
		eBL_INDEX = 0,
		eBL_USER,
		eBL_POS,
		eBL_DIR,
		eBL_DATE,
		eBL_MEMO,

		eBL_TOT_NUM,
	};

public:
	CBookmarkDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBookmarkDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BOOKMARK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	virtual BOOL OnInitDialog();
public:
	CListCtrl	m_lcBookmark;
	CString		m_strMemoIO;
public:
	afx_msg void OnBnClickedInsert();
public:
	afx_msg void OnBnClickedDelete();

protected:
	CBookmarker* m_pBookmarker;
	RwBool m_AscendingSortFlags[eBL_TOT_NUM];

public:
	VOID	CreateBM() { m_pBookmarker->Create(); }
	VOID	Save();
	VOID	OnUpateList();
	RwBool	OnSortData(RwInt32 _Index);
	VOID	InsertNode(RwInt32 _Index, CString& _User, RwV3d& _Pos, RwV3d& _Dir, CString& _Date, CString& _Memo);
	VOID	RefreshWindow();

public:
	afx_msg void OnLvnItemchangedBookmarkDlg(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLvnColumnclickBookmarkDlg(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMDblclkSpawnList(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CStatic m_Preview;
	HBITMAP m_hBitmap;
public:
	afx_msg void OnNMClickSpawnList(NMHDR *pNMHDR, LRESULT *pResult);
};
