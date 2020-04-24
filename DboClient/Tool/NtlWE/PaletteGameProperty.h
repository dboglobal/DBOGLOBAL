#pragma once

#include "NtlPLGameProperty.h"
#include "afxwin.h"

// CPaletteGameProperty 대화 상자입니다.

class CPaletteGameProperty : public CDialog
{
	DECLARE_DYNAMIC(CPaletteGameProperty)

public:
	CPaletteGameProperty(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteGameProperty();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GAME_PROPERTY };
	enum EDIRLOG_ENABLE_FLAG
	{
		EDIRLOG_ENABLE_FLAG_POS			= 0x00000001,
		EDIRLOG_ENABLE_FLAG_RADIUS		= 0x00000002,
		EDIRLOG_ENABLE_FLAG_SIZE		= 0x00000004,
		EDIRLOG_ENABLE_FLAG_PROPERTY	= 0x00000008,
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual LRESULT			WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	virtual BOOL			PreTranslateMessage(MSG* pMsg);

public:
	VOID					InitializeWorldData();

public:
	void					UpdatePropertyData(const CNtlPLEntity* pPLEntity);
	void					EnableDialog(RwUInt32 uiFlag);

	CNtlPLEntity*			CreateGamePropertyCircle();
	CNtlPLEntity*			CreateGamePropertyPlane();

	SPLEntityCreateParam*	GetCreateParam();

	void					SetShapeType(RwUInt32 uiShapeType);
	void					SetPositionData(RwReal x, RwReal y, RwReal z);
	void					SetRadiusData(RwReal radius);
	void					SetSizeData(RwReal x, RwReal y, RwReal z);
	void					SetPropertyData(RwUInt32 uiProperty);

	void					ApplyPosition();
	void					ApplyRadius();
	void					ApplySize();
	void					ApplyProperty();


public:
	CListBox				m_listShapeType;
	CSliderCtrl				m_RadiusSlider;
	CString					m_strPosX;
	CString					m_strPosY;
	CString					m_strPosZ;
	CString					m_strRadius;
	CString					m_strSizeX;
	CString					m_strSizeY;
	CString					m_strSizeZ;

	CButton					m_btnCheckProp1;
	CButton					m_btnCheckProp2;
	CButton					m_btnCheckProp3;
	CButton					m_btnCheckProp4;
	CButton					m_btnCheckProp5;
	CButton					m_btnCheckProp6;
	CButton					m_btnCheckProp7;
	CButton					m_btnCheckProp8;
	CButton					m_btnCheckProp9;
	CButton					m_btnCheckProp10;
	CButton					m_btnCheckProp11;
	CButton					m_btnCheckProp12;
	CButton					m_btnCheckProp13;
	CButton					m_btnCheckProp14;
	CButton					m_btnCheckProp15;
	CButton					m_btnCheckProp16;

protected:
	CNtlPLGameProperty*			m_pPLGamePropertyCur;
	SPLGamePropertyCreateParam	m_sCreateParam;

public:
	afx_msg void			OnBnClickedCancel();
	afx_msg void			OnBnClickedOk();
	afx_msg void			OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void			OnLbnSelchangeListShape();
	afx_msg void OnBnClickedCheckProp1();
	afx_msg void OnBnClickedCheckProp2();
	afx_msg void OnBnClickedCheckProp3();
	afx_msg void OnBnClickedCheckProp4();
	afx_msg void OnBnClickedCheckProp5();
	afx_msg void OnBnClickedCheckProp6();
	afx_msg void OnBnClickedCheckProp7();
	afx_msg void OnBnClickedCheckProp8();
	afx_msg void OnBnClickedCheckProp9();
	afx_msg void OnBnClickedCheckProp10();
	afx_msg void OnBnClickedCheckProp11();
	afx_msg void OnBnClickedCheckProp12();
	afx_msg void OnBnClickedCheckProp13();
	afx_msg void OnBnClickedCheckProp14();
	afx_msg void OnBnClickedCheckProp15();
	afx_msg void OnBnClickedCheckProp16();
};
