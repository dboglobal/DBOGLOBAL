#pragma once

#include "afxwin.h"
#include "colourpicker.h"
#include "NtlWorldShadow.h"
#include "NtlWorldShadowManager.h"
#include "afxcmn.h"


// CPaletteShadow dialog

class CPaletteShadow : public CDialog
{
	DECLARE_DYNAMIC(CPaletteShadow)

public:
	CPaletteShadow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPaletteShadow();

// Dialog Data
	enum { IDD = IDD_SHADOW };

public:
	void InitializeWorldData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCreateWater();
	afx_msg void OnBnClickedDeleteWater();
	afx_msg void OnBnClickedCreateShadow();
	afx_msg void OnBnClickedDeleteShadow();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetColor(int r, int g, int b);

public:
	CButton m_AntiAlias;
	CButton m_Fade;
	CComboBox m_Direction;
	CComboBox m_BlurPass;
	CComboBox m_MapSize;
	CString m_PSColorR;
	CString m_PSColorG;
	CString m_PSColorB;
	COLORREF	m_clrDiffuser;
	CColourPicker m_ShadowClrPicker;

public:
	VOID SetShadowAttr(CNtlWorldShadow* pNtlWorldShadow);

public:
	afx_msg void OnBnClickedPsModify();
public:
	CSliderCtrl m_PSOpaSlider;
public:
	CString m_StrPSOpa;
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedCreateObjShadow();
public:
	afx_msg void OnBnClickedDeleteObjShadow();
public:
	CString m_strDoodadPropInfo;
public:
	afx_msg void OnBnClickedCreateShadowProp();
public:
	afx_msg void OnBnClickedDeleteShadowProp();
	afx_msg void OnBnClickedUpdateParam();
};
