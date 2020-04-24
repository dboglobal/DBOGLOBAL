#pragma once
#include "afxwin.h"


// WeatherDlg 대화 상자입니다.

class CWeatherDlg : public CDialog
{
	DECLARE_DYNAMIC(CWeatherDlg)

public:
	CWeatherDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWeatherDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ATTR_WEATHER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

public:
	CComboBox m_cbWeather[5];

protected:
	void	SelectCbWeather(sNTL_FIELD_PROP* pNtlFieldProp);
	RwBool	UpdateCbWeather();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRefresh();
};
