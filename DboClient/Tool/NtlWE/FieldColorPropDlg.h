#pragma once


// CFieldColorPropDlg 대화 상자입니다.

#include "colourpicker.h"

class CFieldColorPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CFieldColorPropDlg)

public:
	CFieldColorPropDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFieldColorPropDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ATTR_FIELDCOLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	void		 SetColor(int r, int g, int b);

	CString m_strColorR;
	CString m_strColorG;
	CString m_strColorB;
	COLORREF m_clrDiffuser;
	CColourPicker m_clrPicker;

protected:
	void		ApplyData(RwRGBA* pClr);

protected:
	RwRGBA		m_clr;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
};
