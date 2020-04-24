#pragma once

#include "NtlOccluderManager.h"
#include "NtlPLOccluder_Base.h"
#include "NtlPLOccluder_Quad.h"

#include "afxwin.h"

// CPalettePerformanceOcclusion 대화 상자입니다.

#define POC_PROP_PLANE	0x00000001
#define POC_PROP_POS	0x00000002
#define POC_PROP_ROT	0x00000004
#define POC_PROP_SCALE	0x00000008
#define	POC_PROP_TAP	0x00000010
#define	POC_PROP_ROT_Y  0x00000020

class CPalettePerformanceOcclusion : public CDialog
{
	DECLARE_DYNAMIC(CPalettePerformanceOcclusion)

public:
	CPalettePerformanceOcclusion(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPalettePerformanceOcclusion();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PERFORMANCE_OCCLUSION };

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL			OnInitDialog();
	virtual BOOL			PreTranslateMessage(MSG* pMsg);

public:
	void					InitializeWorldData();

public:
	CNtlPLEntity*			CreateOccluderQuad();

	SPLEntityCreateParam*	GetCreateParam();
	
	void					UpdatePropertyData(CNtlPLEntity* pEntity);
	void					EnableProperty(RwUInt32 flag);

	void					SetPositionData(RwReal x, RwReal y, RwReal z);
	void					SetRotationData(RwReal x, RwReal y, RwReal z);
	void					SetScaleData(RwReal x, RwReal y, RwReal z);
	void					SetPlaneTypeData(RwUInt32 eType);
	void					SetTriggerNameData(const char* pName);

	void					ApplyScale();
	void					ApplyRotate();
	void					ApplyPlaneType();
	void					ApplyTriggerName();


protected:
	//CNtlPLOccluder_Base*	m_pOccluderCurrent;
	EPLOccluderType			m_eOccluderType;
	SPLEntityCreateParam	m_CreateParam;
public:
	afx_msg void OnBnClickedOccluderNew();

public:
	CString				m_strPosX;
	CString				m_strPosY;
	CString				m_strPosZ;
	CString				m_strRotX;
	CString				m_strRotY;
	CString				m_strRotZ;
	CString				m_strScaleX;
	CString				m_strScaleY;
	CString				m_strScaleZ;
	CString				m_strTapS;
	CString				m_strTapR;
	CString				m_strTapT;
	CString				m_strTriggerName;
	CComboBox			m_cbOccluderPlaneType;
	CComboBox			m_cbFuntionFlag;

public:
	afx_msg void OnCbnSelchangePlaneType();
public:
	//afx_msg void OnBnClickedPvs();
	//afx_msg void OnBnClickedPortal();
	
	afx_msg void OnCbnSelchangeComboFunc();
	
};
