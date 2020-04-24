#pragma once


// CWaterPropDlg dialog

class CWaterPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CWaterPropDlg)

public:
	CWaterPropDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWaterPropDlg();

// Dialog Data
	enum { IDD = IDD_ATTR_WATER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
