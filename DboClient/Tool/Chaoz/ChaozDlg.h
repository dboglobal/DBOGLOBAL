// ChaozDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CChaozDlg 대화 상자
class CChaozDlg : public CDialog
{
// 생성입니다.
public:
	CChaozDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CHAOZ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton_CheatPlayManyEffectSound();
	afx_msg void OnBnClickedButton_CheatPitchRate();
	afx_msg void OnBnClickedButton_OpenSound();
	afx_msg void OnBnClickedButton_PlaySound();
	afx_msg void OnBnClickedButton_StopSound();

public:
	bool			SendMessageToDBO(const char* pMessage);
	void			DisplayMessage(_TCHAR* pText);
	
protected:
	// Cheat
	CEdit			m_Edit_PitchRate;
	CComboBox		m_Combo_Channel;

	// message
	CEdit			m_Static_Message;

	// Play sound
	CEdit			m_Edit_FileName;
	CEdit			m_Edit_SoundPosX;
	CEdit			m_Edit_SoundPosY;
	CEdit			m_Edit_SoundPosZ;
	CEdit			m_Edit_Min;
	CEdit			m_Edit_Max;
	CEdit			m_Edit_Volume;
	CButton			m_CheckBox_Loop;
};
