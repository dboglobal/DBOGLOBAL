#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSoundOption 대화 상자입니다.

class CSoundOption : public CDialog
{
	DECLARE_DYNAMIC(CSoundOption)

public:
	CSoundOption(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSoundOption();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SOUND_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:	
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClicked_CheckBgm();
	afx_msg void OnBnClicked_CheckWeatherEffect();
	afx_msg void OnBnClicked_CheckWeatherMusic();
	afx_msg void OnBnClicked_OK();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

protected:
	CButton			m_BtnCheckBGMMute;
	CButton			m_BtnCheckWeatherEffect;
	CButton			m_BtnCheckWeatherMusic;
	CSliderCtrl		m_ScrollbarBGMVolume;
	CSliderCtrl		m_ScrollbarWeatherEffect;
	CSliderCtrl		m_ScrollbarWeatherMusic;
};