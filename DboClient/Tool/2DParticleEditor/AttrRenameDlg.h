#pragma once
#include "afxwin.h"


// CAttrRenameDlg 대화 상자입니다.

class CAttrRenameDlg : public CDialog
{
	DECLARE_DYNAMIC(CAttrRenameDlg)

public:
	CAttrRenameDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAttrRenameDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ATTR_RENAME };

	void	SetPrePosition( LPRECT rect );
	CRect	m_rect;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strRename;
public:
	virtual BOOL OnInitDialog();
};
