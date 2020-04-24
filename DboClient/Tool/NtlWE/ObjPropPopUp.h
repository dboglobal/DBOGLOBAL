#pragma once
#include "afxwin.h"
#include "checkcombobox.h"


// CObjPropPopUp 대화 상자입니다.

class CObjPropPopUp : public CDialog
{
	DECLARE_DYNAMIC(CObjPropPopUp)

public:
	CObjPropPopUp(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjPropPopUp();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OBJ_PROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedTrigger();
	afx_msg void OnEnChangeVisibilityDist();

public:
	sNTL_DOODADS_PROP_PARAM m_NtlDoodadsPropParam;

	CString			m_strGUID;
	CString			m_strAABB;	
	CString			m_strVisibilityDist;
	CButton			m_EnableTrigger;
	CString			m_strMax;

	CComboBox		m_cbObjectType;
	CCheckComboBox	m_cbMinimapLayer;
	CCheckComboBox	m_cbBitProp;
	CCheckComboBox	m_cbLtMapFlagsObject;
	CCheckComboBox	m_cbLtMapFlagsMaterial;

	CString			m_CurName;
	CString			m_NameIndexInput;
	CString			m_DoodadName;
	CString			m_DoodadID;
	CString			m_MilePostGUID;
	CString			m_MilePostMemo;
	CEdit			m_MilePostEditCtrl;
};
