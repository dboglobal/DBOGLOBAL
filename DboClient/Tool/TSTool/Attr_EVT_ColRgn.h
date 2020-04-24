#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_EVT_ColRgn 대화 상자입니다.

class CAttr_EVT_ColRgn : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_EVT_ColRgn)

public:
	typedef std::vector< unsigned int > vecdef_ControlList;
	typedef std::map< unsigned int, vecdef_ControlList > vecdef_MapList;

protected:
	vecdef_MapList m_defMapList;
	int m_nCurSelIndex;

public:
	CAttr_EVT_ColRgn(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAttr_EVT_ColRgn();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_EVT_COL_RGN_ATTR_DIAG };

	void InitData( void );

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwWorldIdx;
	CButton m_ctrEnter;
	CButton m_ctrLeave;
	CButton m_ctrThrough;
	CComboBox m_ctrCRType;

	float m_fRadiusX;
	float m_fRadiusZ;
	float m_fRadiusR;

	float m_fRectX0;
	float m_fRectZ0;
	float m_fRectX1;
	float m_fRectZ1;
	float m_fRectX2;
	float m_fRectZ2;
	float m_fRectX3;
	float m_fRectZ3;

public:
	afx_msg void OnCbnSelchangeTsEvtAttrColRgnCrtCombo();
};
