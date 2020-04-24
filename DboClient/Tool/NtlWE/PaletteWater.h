#pragma once


#include "afxwin.h"
#include "colourpicker.h"
#include "NtlWorldSector.h"
#include "afxcmn.h"


class CPaletteWater : public CDialog
{
	DECLARE_DYNAMIC(CPaletteWater)

public:
	CPaletteWater(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPaletteWater();

// Dialog Data
	enum { IDD = IDD_WATER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

public:
	void SetColor(int r, int g, int b);
	void InitializeWorldData();
	void SetWorldDataDefault();
	void GetWaterAttr(sSECTOR_WATER_ATTR& SectorWaterAttr);
	void SetWaterAttr(sSECTOR_WATER_ATTR& SectorWaterAttr);

public:
	CString		m_strHeight;
	CString		m_strAlpha;
	CString		m_strTransformRotation;
	CString		m_strTransformScale;
	CString		m_strWaterDiffuseR;
	CString		m_strWaterDiffuseG;
	CString		m_strWaterDiffuseB;

	CComboBox	m_DepthMapSize;
	CComboBox	m_RenderstateSrc;
	CComboBox	m_RenderstateDst;

	COLORREF		m_clrDiffuser;
	CListBox		m_SequenceType;
	CColourPicker	m_WaterDiffusePicker;

	sSECTOR_WATER_ATTR m_SectorWaterAttr;

	// depth map
	CSliderCtrl m_OpaBrushSlider;
	CString		m_OpaBrushString;
	CButton		m_IsDepthMapEdit;
	CSliderCtrl m_RadiusBrushSlider;
	CString		m_RadiusBrushString;

public:
	afx_msg void OnBnClickedModifyHeight();
	afx_msg void OnBnClickedCreateDepthmap();
	afx_msg void OnBnClickedDeleteDepthmap();
	afx_msg void OnBnClickedCreateWater();
	afx_msg void OnBnClickedModifyWater();
	afx_msg void OnBnClickedDeleteWater();
public:
	afx_msg void OnCbnSelchangeDepthmapSize();
public:
	CButton m_MapActivating;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

public:
	RwInt32 GetCurMode();
	RwInt32 GetCurOpa();
	RwReal	GetCurRadius();
	VOID	Refresh(RwReal _Radius);
};
