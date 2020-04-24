#pragma once

#include "checkcombobox.h"

// CEffectPropPopup 대화 상자입니다.

class CEffectPropPopup : public CDialog
{
	DECLARE_DYNAMIC(CEffectPropPopup)

public:
	CEffectPropPopup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffectPropPopup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EFFECT_PROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	CString			m_strVisibleDist;
	CString			m_strName;

	CCheckComboBox	m_cbMinimapLayer;
};
