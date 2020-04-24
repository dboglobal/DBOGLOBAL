#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CPostEffectDialog dialog

#include "MacSliderCtrl.h"
#include "MacButtons.h"

class CPostEffectDialog : public CDialog
{
	DECLARE_DYNAMIC(CPostEffectDialog)

public:
	CPostEffectDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPostEffectDialog();

	static CPostEffectDialog& GetInstance();


	void	InitSliderButton(CMacSliderCtrl* pSlider, int nStart, int nEnd, int nTic, COLORREF color);

// Dialog Data
	enum { IDD = IDD_POST_EFFFECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CButton m_btnFilterEnable;
public:
	CMacSliderCtrl m_sliderPowerHeight;
public:
	CMacSliderCtrl m_sliderSpaceHeight;
public:
	CString m_strPowerHeight;
public:
	CString m_strSpaceHeight;
public:
	afx_msg void OnBnClickedFilterEnable();
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
