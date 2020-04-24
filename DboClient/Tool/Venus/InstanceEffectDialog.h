#pragma once
#include "afxwin.h"


// CInstanceEffectDialog 대화 상자입니다.

#include "Resource.h"

class CInstanceEffectDialog : public CDialog
{
	DECLARE_DYNAMIC(CInstanceEffectDialog)

public:
	CInstanceEffectDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInstanceEffectDialog();

	static CInstanceEffectDialog& GetInstance();

	void	BuildInstanceEffect();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INSTANCE_EFFECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CListBox m_EffectList;
	afx_msg void OnLbnSelchangeEffectList();
};
