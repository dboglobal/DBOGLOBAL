#pragma once


#include "ntlworldpalettedefine.h"
#include "colourpicker.h"
#include "afxcmn.h"


class CPaletteTerrain : public CDialog
{
	DECLARE_DYNAMIC(CPaletteTerrain)

public:
	CPaletteTerrain(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteTerrain();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TERRAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRaise();
	afx_msg void OnBnClickedLower();
	afx_msg void OnBnClickedPlateau();
	afx_msg void OnBnClickedJust();
	afx_msg void OnBnClickedStandardize();
	afx_msg void OnBnClickedSmooth();

public:
	void OnCreateBrushCircle();
	void OnCreateBrushRectangle();
	void OnCreateBrushLine();
	void OnCustomizeEdit(RwReal Height);
	void UpdateFieldTextureInfo(RwInt32 _IdxCurPickedField = -1);
	void InitializeWorldData();
	void HDCategoryBtnImgSwapping();
	void OnUpdateBrushSize(int Value2Add);
	void OnUpdateBrushSensitivity(int Value2Add);
	VOID OnResetTexturePanel();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	CString m_strCustomizeEditVal;
	COLORREF m_clrDiffuser;
	CString m_strBrushSize;
	CString m_StrAngBLT0;
	CString m_StrAngDLT0;
	CString m_StrAngDLT1;
	CString m_StrAngDLT2;
	CString m_StrAngDLT3;
	CString m_StrAngDLT4;
	CString m_strBLT0Name;
	CString m_strDLT0Name;
	CString m_strDLT1Name;
	CString m_strDLT2Name;
	CString m_strDLT3Name;
	CString m_strDLT4Name;
	CString m_strSensitivity;
	CColourPicker m_ClrDiffusePicker;
	CString m_ClrDiffusePickerR;
	CString m_ClrDiffusePickerG;
	CString m_ClrDiffusePickerB;
	CString m_StrAddrBLT0;
	CString m_StrAddrDLT0;
	CString m_StrAddrDLT1;
	CString m_StrAddrDLT2;
	CString m_StrAddrDLT3;
	CString m_StrAddrDLT4;

public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedCustomize();
	afx_msg void OnBnClickedBlt0CreateBtn();
	afx_msg void OnBnClickedBlt0DeleteBtn();
	afx_msg void OnBnClickedDlt0CreateBtn();
	afx_msg void OnBnClickedDlt0DeleteBtn();
	afx_msg void OnBnClickedDlt1CreateBtn();
	afx_msg void OnBnClickedDlt1DeleteBtn();
	afx_msg void OnBnClickedDlt2CreateBtn();
	afx_msg void OnBnClickedDlt2DeleteBtn();
	afx_msg void OnEnSetfocusDiffusePickerR();
	afx_msg void OnEnSetfocusDiffusePickerG();
	afx_msg void OnEnSetfocusDiffusePickerB();
	afx_msg void OnBnClickedBlt0ActBtn();
	afx_msg void OnBnClickedDlt0ActBtn();
	afx_msg void OnBnClickedDlt1ActBtn();
	afx_msg void OnBnClickedDlt2ActBtn();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedDlt3ActBtn();
	afx_msg void OnBnClickedDlt4ActBtn();
	afx_msg void OnBnClickedDlt3CreateBtn();
	afx_msg void OnBnClickedDlt4CreateBtn();
	afx_msg void OnBnClickedDlt3DeleteBtn();
	afx_msg void OnBnClickedDlt4DeleteBtn();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CSliderCtrl m_BrushSizeSlider;
	CSliderCtrl m_BrushSensitivitySlider;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBlt0ModifyBtn2();
public:
	afx_msg void OnBnClickedDlt0ModifyBtn2();
public:
	afx_msg void OnBnClickedDlt1ModifyBtn2();
public:
	afx_msg void OnBnClickedDlt2ModifyBtn2();
public:
	afx_msg void OnBnClickedDlt3ModifyBtn2();
public:
	afx_msg void OnBnClickedDlt4ModifyBtn2();
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
public:
	CSliderCtrl m_MovingSpeedSlider;
public:
	CString m_strMovingSpeed;
public:
	CSliderCtrl m_OpacitySlider;
public:
	CString m_strOpacity;
public:
	afx_msg void OnBnClickedCustomizeE();

public:
	RwReal GetCustomizeEnhancedData();
	void SetCustomizeEnhancedData(RwReal _Height);
public:
	afx_msg void OnBnClickedSlope();
public:
	afx_msg void OnBnClickedSmoothrange();
public:
	afx_msg void OnBnClickedRoad();

public:
	RwBool IsInvalidFile(char* pStr);
};
