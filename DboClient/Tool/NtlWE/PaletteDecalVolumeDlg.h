#pragma once
#include "afxwin.h"
#include "colourpicker.h"
#include "NtlPLDecalVolume.h"

// PaletteDecalVolumeDlg 대화 상자입니다.

#define PDV_PROP_COLOR	0x00000001
#define PDV_PROP_POS	0x00000002
#define PDV_PROP_ROT	0x00000004
#define PDV_PROP_SCALE	0x00000008
#define PDV_PROP_OFFSET	0x00000010
#define PDV_PROP_DIST	0x00000020
#define PDV_PROP_MODE	0x00000040

class CPaletteDecalVolumeDlg : public CDialog
{
	DECLARE_DYNAMIC(CPaletteDecalVolumeDlg)

public:
	CPaletteDecalVolumeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteDecalVolumeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ORNAMENT_DECAL_VOLUME };

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual LRESULT			WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL			PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:
	void					InitializeWorldData();

public:
	virtual BOOL			OnInitDialog();
	afx_msg void			OnBnClickedOk();
	afx_msg void			OnBnClickedCancel();

public:
	void					UpdatePropertyData(const CNtlPLEntity* pEntity);
	SPLEntityCreateParam*	GetCreateParam(CNtlPLEntity* pEntity);

	void					EnableDlg(RwUInt32 uiFlags);

	void					SetColor(int r, int g, int b);
	void					SetColorData(RwUInt8 r, RwUInt8 g, RwUInt8 b);
	void					SetAlphaData(RwUInt8 a);
	void					ApplyColor();

	void					SetPositinData(RwV3d vPos);
	void					ApplyPosition();

	void					SetRotationData(RwV3d vRot);
	void					ApplyRotation();

	void					SetScaleData(RwV3d vScale);
	void					ApplyScale();

	void					SetDecalVolumeFlagsData(RwUInt32 uiFlags);
	void					ApplyDecalVolumeFlags();

	void					SetAppendOffsetData(RwReal fAppendOffset);
	void					ApplyAppendOffset();

	void					SetVisibleDistData(RwReal fVisibleDist);
	void					ApplyVisibleDst();

public:
	afx_msg void			OnLbnSelchangeListboxDecal();
	afx_msg void			OnCbnSelchangeComboDecalVolume();

public:
	CColourPicker		m_cpDiffuse;
	COLORREF			m_clrDiffuse;

	CListBox			m_listDecalVolume;

	CString				m_strColorR;
	CString				m_strColorG;
	CString				m_strColorB;
	CString				m_strColorA;

	CString				m_strPositionX;
	CString				m_strPositionY;
	CString				m_strPositionZ;

	CString				m_strRotationX;
	CString				m_strRotationY;
	CString				m_strRotationZ;

	CString				m_strScaleX;
	CString				m_strScaleY;
	CString				m_strScaleZ;

	CComboBox			m_cbDecalVolumeType;

	CString				m_strAppendOffset;
	CString				m_strVisibleDist;

public:
	CNtlPLDecalVolume*			m_pPLDecalVolumeCur;
	SPLDecalVolumeCreateParam	m_sDecalCreateParam;
};
