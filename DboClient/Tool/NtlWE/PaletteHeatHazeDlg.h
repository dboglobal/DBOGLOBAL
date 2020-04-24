#pragma once
#include "afxcmn.h"


// CPaletteHeatHazeDlg 대화 상자입니다.

class CPaletteHeatHazeDlg : public CDialog
{
	DECLARE_DYNAMIC(CPaletteHeatHazeDlg)

public:
	CPaletteHeatHazeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteHeatHazeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ORNAMENT_HEATHAZE };

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:
	CString m_StringPosX;
	CString m_StringPosY;
	CString m_StringPosZ;
	CString m_StringRotY;
	CString m_StringSclX;
	CString m_StringSclY;
	CString m_StringTapS;
	CString m_StringTapR;
	CString m_StringTapT;

	CSliderCtrl m_SliderMps;
	CSliderCtrl m_SliderExtent;
	CSliderCtrl m_SliderScale;
	CSliderCtrl m_SliderExtentRand;

	CString m_StringMps;
	CString m_StringExtent;
	CString m_StringScale;
	CString m_StringExtentRand;

	CString m_StringDist;

public:
	virtual BOOL		OnInitDialog();
	void				InitializeWorldData();

	void				UpdatePropertyData(CNtlPLEntity* pEntity, RwBool bScrollUpdate = TRUE, RwBool bEditUpdate = TRUE);

public:
	afx_msg void		OnBnClickedCreate();
	afx_msg void		OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
