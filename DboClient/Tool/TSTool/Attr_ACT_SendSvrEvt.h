#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_ACT_SendSvrEvt 대화 상자입니다.

class CAttr_ACT_SendSvrEvt : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_ACT_SendSvrEvt)

public:
	typedef std::vector< unsigned int > vecdef_ControlList;
	typedef std::map< unsigned int, vecdef_ControlList > vecdef_MapList;

protected:
	vecdef_MapList m_defMapList;
	int m_nCurSelIndex;

	vecdef_MapList m_defMapSubList;
	int m_nCurSubSelIndex;

public:
	CAttr_ACT_SendSvrEvt();
	virtual ~CAttr_ACT_SendSvrEvt();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_ACT_SEND_SVR_EVT_ATTR_DIAG };

	void InitData( void );
	void InitSubData( void );

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_taID;
	CComboBox m_ctrEvtType;
	CComboBox m_ctrEvtTSType;
	DWORD m_dwEvtId;
	DWORD m_dwTblIdx;

	CComboBox m_ctrEvtSendType;
	float m_fEvtSendRadius;

public:
	void OnCbnSelchangeTsActAttrSendSvrEvtEvtTypeCombo();
	void OnCbnSelchangeTsActAttrSendSvrEvtSendTypeCombo();
};
