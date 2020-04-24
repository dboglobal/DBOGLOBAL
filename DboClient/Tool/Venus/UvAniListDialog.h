#pragma once
#include "afxcmn.h"


// CUvAniListDialog 대화 상자입니다.
#include "DataTreeCtrl.h"

class CUvAniListDialog : public CDialog
{
	DECLARE_DYNCREATE(CUvAniListDialog)

public:

public:
	CUvAniListDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUvAniListDialog();

	void	SerializeList();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UVANI_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
protected:
    void OnRefresh();

protected:
    CDataTreeCtrl m_UvAnimationList;
};
