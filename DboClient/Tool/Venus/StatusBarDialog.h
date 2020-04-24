#pragma once
#include "afxwin.h"


// CStatusBarDialog 대화 상자입니다.

#include "rwcore.h"

#include "resource.h"

#include "MacSliderCtrl.h"
#include "MacButtons.h"


class CStatusBarDialog : public CDialog
{
	DECLARE_DYNAMIC(CStatusBarDialog)

public:
	CStatusBarDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStatusBarDialog();
	static CStatusBarDialog&	GetInstance();

	void	InsertTrace(const RwChar* strTrace);

	void	BuildData();

	void	UpdateDymmyButton();

	void	UpdateFps(RwReal fFps);

	void	UpdateTime();
	void	UpdateTimeButton();
	void	UpdateCount(RwInt32 nEffectCount, RwInt32 nPolygonCount);
public:



// 대화 상자 데이터입니다.
	enum { IDD = IDD_STATUSBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_TraceList;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	float m_fFps;
	CMacSliderCtrl m_TimeSpeedSlider;
	CString m_strTimeSpeed;
	CMacCheckBox m_btnTimePause;
	afx_msg void OnBnClickedTimePause();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int m_nPolygonCount;
	int m_nEffectCount;
	CMacCheckBox m_ShowGrid;
	CMacCheckBox m_ShowTerrain;
	CMacCheckBox m_ShowObject;
	afx_msg void OnBnClickedShowGrid();
	afx_msg void OnBnClickedShowObject();
	afx_msg void OnBnClickedShowTerrain();
	CMacCheckBox m_btnTimeRepeat;
	afx_msg void OnBnClickedTimeRepeat();
};
