#pragma once


#include "colourpicker.h"
#include "afxwin.h"
#include "NtlWorldSLManger.h"


class CSLPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CSLPropDlg)

public:
	CSLPropDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSLPropDlg();

	enum { IDD = IDD_ATTR_SL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString m_StrSoftness;
	CString m_StrSLClrR;
	CString m_StrSLClrG;
	CString m_StrSLClrB;
	CString m_StrBrightnessMin;
	CString m_StrBrightnessMax;

	CColourPicker	m_SLClrPickeer;
	CComboBox		m_SLDirCombo;
	CButton			m_ApplySlopeLighting;
	COLORREF		m_ClrSL;

public:
	virtual BOOL	DestroyWindow();
	virtual BOOL	OnInitDialog();
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	void			SetData(sNTL_WORLD_SL& NtlWorldSL, RwBool IsMultipleSelected = FALSE);
	void			SetColor(int r, int g, int b);
	RwBool			GetSLData(RwV3d& Pos, sNTL_WORLD_SL& NtlWorldSL);
};
