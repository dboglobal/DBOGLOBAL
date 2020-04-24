#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "colourpicker.h"

#include "NtlPLWorldLight_Base.h"
#include "NtlPLWorldLight_Sphere.h"
#include "NtlPLWorldLight_Box.h"

// CPaletteWorldLight 대화 상자입니다.

#define PWL_PROP_COLOR	0x00000001
#define PWL_PROP_POS	0x00000002
#define PWL_PROP_ROT	0x00000004
#define PWL_PROP_SIZE	0x00000008
#define	PWL_PROP_RADIUS	0x00000010
#define	PWL_PROP_TAP	0x00000020



class CPaletteWorldLight : public CDialog
{
	DECLARE_DYNAMIC(CPaletteWorldLight)

public:
	CPaletteWorldLight(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteWorldLight();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WORLDLIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	VOID InitializeWorldData();

public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLbnSelchangeListWorldlight();

public:
	EPLWorldLightType	m_eWorldLightType;
	EPLWorldLightType	m_eWorldLightTypeTemp;

	CListBox			m_WorldLightType;
	COLORREF			m_clrDiffuser;
	CColourPicker		m_clrPicker;
	CSliderCtrl			m_RadiusSlider;
	CString				m_strColorR;
	CString				m_strColorG;
	CString				m_strColorB;
	CString				m_strPosX;
	CString				m_strPosY;
	CString				m_strPosZ;
	CString				m_strRotX;
	CString				m_strRotY;
	CString				m_strRotZ;
	CString				m_strRadius;
	CString				m_strSizeX;
	CString				m_strSizeY;
	CString				m_strSizeZ;
	CString				m_strTapS;
	CString				m_strTapR;
	CString				m_strTapT;

	SPLWorldLightCreateParam_Sphere m_CreateParam_Sphere;
	SPLWorldLightCreateParam_Box	m_CreateParam_Box;

	CNtlPLWorldLight_Base*	m_pCurWorldLight;

public:
	void			SetWorldLight(RwInt32 iIndex);
	void			SetColor(int r, int g, int b);
	void			SetColorData(RwUInt8 r, RwUInt8 g, RwUInt8 b);
	void			SetPositionData(RwReal x, RwReal y, RwReal z);
	void			SetRadiusData(RwReal radius);
	void			SetRotationData(RwReal x, RwReal y, RwReal z);
	void			SetSizeData(RwReal x, RwReal y, RwReal z);

	void			EnableProperty(RwUInt32 flag);
	void			UpdatePropertyData(const CNtlPLEntity* pEntity);

	SPLEntityCreateParam* GetCreateParam();
	SPLEntityCreateParam* GetCreateParam(CNtlPLEntity* pNtlPLEntity);

	void			CreateWorldLightByLightObject(CNtlPLEntity* pPLEntity);

protected:
	CNtlPLEntity*	CreateWorldLightShpere();
	CNtlPLEntity*	CreateWorldLightBox();

	void			ApplyColor();
	void			ApplyRotate();
	void			ApplyRadius();
	void			ApplySize();
};
