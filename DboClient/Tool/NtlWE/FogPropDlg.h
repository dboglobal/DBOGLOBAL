#pragma once


#include "colourpicker.h"

// CFogPropDlg dialog

class CFogPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CFogPropDlg)

public:
	CFogPropDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFogPropDlg();

// Dialog Data
	enum { IDD = IDD_ATTR_FOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	COLORREF m_clrDiffuser;
public:
	CColourPicker m_AttrFogClrPicker;
public:
	CString m_AttrFogClrR;
public:
	CString m_AttrFogClrG;
public:
	CString m_AttrFogClrB;
public:
	CString m_AttrFogNear;
public:
	CString m_AttrFogFar;
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);


public:
	void ApplyData(RwRGBA FogColor, RwReal FogCamPlane[2], RwBool IsMultipleSelected = FALSE);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnClose();
public:
	afx_msg void OnDestroy();

public:
	void SetColor(int r, int g, int b);
};
