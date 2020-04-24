#pragma once

#include "NtlPLLightIndoor.h"

#include "colourpicker.h"
#include "afxwin.h"

// PaletteLightIndoor 대화 상자입니다.

#define PLI_PROP_COLOR	0x00000001
#define PLI_PROP_POS	0x00000002
#define PLI_PROP_ROT	0x00000004
#define PLI_PROP_REAL	0x00000008
#define	PLI_PROP_RADIUS	0x00000010
#define	PLI_PROP_ANGLE	0x00000020

class CPaletteLightIndoor : public CDialog
{
	DECLARE_DYNAMIC(CPaletteLightIndoor)

public:
	CPaletteLightIndoor(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteLightIndoor();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LIGHT_INDOOR };

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual LRESULT			WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	virtual BOOL			PreTranslateMessage(MSG* pMsg);

public:
	VOID					InitializeWorldData();

public:
	afx_msg void			OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void			OnLbnSelchangeListLight();

public:
	// pEntity 값으로 Dlg Data를 변경한다.
	void					UpdatePropertyData(const CNtlPLEntity* pEntity);

	SPLEntityCreateParam*	GetCreateParam(CNtlPLEntity* pEntity);

	// Dlg Data를 변경한다.
	void					EnableDlg(RwUInt32 uiFlags);
	void					SetLightData(RwInt32 iIdx);
	void					SetColorData(RwUInt8 r, RwUInt8 g, RwUInt8 b);
	void					SetPositionData(RwReal x, RwReal y, RwReal z);
	void					SetRadiusData(RwReal radius);
	void					SetAngleData(RwReal angle);
	void					SetRotationData(RwReal x, RwReal y, RwReal z);

	void					SetRealLightData(RwBool bBool);

	void					SetLightMapScaleData(RwInt32 iSample);

	// Entity를 Dlg 값으로 세팅한다.
	void					ApplyPosition();
	void					ApplyColor();
	void					ApplyRotate();
	void					ApplyRadius();
	void					ApplyAngle();
	void					ApplyLightMapScale();

	CNtlPLEntity*			CreateLightDirectional(RwBool bRealLight);
	CNtlPLEntity*			CreateLightPoint(RwBool bRealLight);
	CNtlPLEntity*			CreateLightSpot(RwBool bRealLight);
	CNtlPLEntity*			CreateLightSoftSpot(RwBool bRealLight);

public:
	void					SetColor(int r, int g, int b);
	CColourPicker			m_clrPicker;
	COLORREF				m_clrDiffuser;

public:
	CListBox				m_LightType;
	CSliderCtrl				m_RadiusSlider;
	CSliderCtrl				m_AngleSlider;
	CSliderCtrl				m_SampleSlider;
	CString					m_strColorR;
	CString					m_strColorG;
	CString					m_strColorB;
	CString					m_strPosX;
	CString					m_strPosY;
	CString					m_strPosZ;
	CString					m_strRotX;
	CString					m_strRotY;
	CString					m_strRotZ;
	CString					m_strRadius;
	CString					m_strAngle;
	CString					m_strSample;

	CButton					m_BtnVisibleLine;
	CButton					m_BtnVisiblePlane;

protected:
	CNtlPLLightIndoor*			m_pPLLightIndoorCur;
	SPLLightIndoorCreateParam	m_sLightIndoorCreateParam;
public:
	
	afx_msg void OnBnClickedRadioReal();
	afx_msg void OnBnClickedRadioDummy();
	afx_msg void OnBnClickedCheckVisibleLine();
	afx_msg void OnBnClickedCheckVisiblePlane();
};
