#pragma once
#include "afxwin.h"


// CUpdateNewsDialog 대화 상자입니다.

class CUpdateNewsDialog : public CDialog
{
	DECLARE_DYNAMIC(CUpdateNewsDialog)

public:
	CUpdateNewsDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUpdateNewsDialog();

	static CUpdateNewsDialog& GetInstance();

	BOOL	m_bInitialized;

	void	LoadUpdateNews();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UPDATE_NEWS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_UpdateNews;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
