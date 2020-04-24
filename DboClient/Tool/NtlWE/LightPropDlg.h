#pragma once

#include "colourpicker.h"

class CLightPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CLightPropDlg)

public:
	CLightPropDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightPropDlg();

	// Dialog Data
	enum { IDD = IDD_ATTR_LIGHT_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
 	COLORREF m_ColorAmbient;
	CColourPicker m_PickerAmbient;

	CString m_strColorRAmbient;
	CString m_strColorGAmbient;
	CString m_strColorBAmbient;

	CButton	m_btnCheckAmbient;

	COLORREF m_ColorDirectional[3];
 	CColourPicker m_PickerDirectional[3];

	CString m_strColorRDirectional[3];
	CString m_strColorGDirectional[3];
	CString m_strColorBDirectional[3];

	CString m_strAngleXDirectional[3];
	CString m_strAngleYDirectional[3];
	CString m_strAngleZDirectional[3];

	CButton	m_btnCheckDirectional[3];
		
public:
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	virtual BOOL	OnInitDialog();

	void			SetAmbient(RwRGBAReal& rgbafColor);
	void			SetDirectional(RwInt32 iIdx, RwRGBAReal& rgbafColor);
	void			SetDirectional(RwInt32 iIdx, RwV3d& vAngle);
	

public:
	afx_msg void	OnBnClickedOk();
};