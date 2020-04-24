#pragma once

#include "afxwin.h"
#include "NtlPLDojo.h"

// CDojoPropPopup 대화 상자입니다.

class CDojoPropPopup : public CDialog
{
	DECLARE_DYNAMIC(CDojoPropPopup)

public:
	CDojoPropPopup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDojoPropPopup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ORNAMENT_DOJO_POPUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonLoadObject();
	afx_msg void OnBnClickedButtonLoadEffectUp();
	afx_msg void OnBnClickedButtonLoadEffectDown();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonDelEffectUp();
	afx_msg void OnBnClickedButtonDelEffectDown();
	afx_msg void OnCbnSelchangeComboLevel();
	afx_msg void OnCbnEditchangeComboLevel();
	afx_msg void OnBnClickedCheckFadeUse();
	afx_msg void OnBnClickedCheckFadeUse2();

protected:
	void			GetDojoProp(CNtlPLDojo* pPLDojo);
	void			SetDojoProp(CNtlPLDojo* pPLDojo);

	void			SetDojoLevel(RwInt32 iLevel);

public:
	CString			m_strDojoID;
	CButton			m_btnFadeInUse;
	CString			m_strFadeInTime;
	CButton			m_btnFadeOutUse;
	CString			m_strFadeOutTime;
	CComboBox		m_cbDojoLevel;
	CString			m_strNameObject;
	CString			m_strNameEffectUp;
	CString			m_strNameEffectDown;

	CString			m_astrNameObject[dPL_DOJO_MAX_LEVEL];
	CString			m_astrNameEffectUp[dPL_DOJO_MAX_LEVEL];
	CString			m_astrNameEffectDown[dPL_DOJO_MAX_LEVEL];
};
