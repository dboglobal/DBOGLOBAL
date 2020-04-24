#pragma once


#include "colourpicker.h"
#include "afxwin.h"

// CSkyPropDlg dialog

class CSkyPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CSkyPropDlg)

public:
	CSkyPropDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSkyPropDlg();

// Dialog Data
	enum { IDD = IDD_ATTR_SKY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	COLORREF m_clrDiffuser[3];

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	CColourPicker m_SkyRGBPicker0;
	CColourPicker m_SkyRGBPicker1;
	CColourPicker m_SkyRGBPicker2;
	CString m_SkyRGB0RStr;
	CString m_SkyRGB0GStr;
	CString m_SkyRGB0BStr;
	CString m_SkyRGB1RStr;
	CString m_SkyRGB1GStr;
	CString m_SkyRGB1BStr;
	CString m_SkyRGB2RStr;
	CString m_SkyRGB2GStr;
	CString m_SkyRGB2BStr;
	CString m_SkyBaseTextureNameStr;
	CString m_SkyBaseSpeedStr;
	CString m_SkyBlendedTexture0SpeedStr;
	CString m_SkyBlendedTexture1SpeedStr;
	CString m_SkyBlendedTexture0StrName;
	CString m_SkyBlendedTexture1StrName;
	CButton m_SkyBlendedTexture0Activate;
	CButton m_SkyBlendedTexture1Activate;

public:
	afx_msg void OnBnClickedSkyBaseTextureLoad();
	afx_msg void OnBnClickedSkyBlendedTexture0Load();
	afx_msg void OnBnClickedSkyBlendedTexture1Load();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBaseskyColor();
	afx_msg void OnBnClickedBaseskyTexture();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSkyBlendedTexture1Del();
	afx_msg void OnBnClickedSkyBlendedTexture0Del();
	afx_msg void OnBnClickedSkyBaseTextureDel();

public:
	void SetColor0(int r, int g, int b);
	void SetColor1(int r, int g, int b);
	void SetColor2(int r, int g, int b);
	void ApplyData(sNTL_FIELD_PROP& NtlFieldProp, RwBool IsMultipleSelected = FALSE);
};
