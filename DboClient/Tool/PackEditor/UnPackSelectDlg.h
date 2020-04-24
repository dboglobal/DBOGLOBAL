#pragma once
#include "afxwin.h"
#include "NtlPlResourcePack.h"


// CUnPackSelectDlg 대화 상자입니다.

class CUnPackSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CUnPackSelectDlg)

public:
	CUnPackSelectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnPackSelectDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UNPACK_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CXTCheckListBox m_listPack;    

    static int m_LoadPackType[MAX_NTL_PACK_TYPE];

public:    
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
public:
    afx_msg void OnBnClickedBtAll();
};
