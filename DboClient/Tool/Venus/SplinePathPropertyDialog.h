#pragma once


// CSplinePathPropertyDialog 대화 상자입니다.
#include "PropTree.h"

#include "rwcore.h"

class CSplinePathPropertyDialog : public CDialog
{
	DECLARE_DYNCREATE(CSplinePathPropertyDialog)

public:
	CSplinePathPropertyDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSplinePathPropertyDialog();

	void	ResetProperties(RwUInt32 nCurrentPoint = 0);
	void	ApplyProperties(CPropTreeItem* pItem);

public:
	BOOL						m_Initialized;
	CPropTree					m_PropertyTree;

	RwUInt32					m_nCurrentPoint;

	enum TIME_TYPE
	{
		TIME_TYPE_RESET_PROPERTIES = 0x00,
		TIME_TYPE_SPLINE_PATH_NAME,
	};


	enum _PROPERTY_CONTROL_ID
	{
		_PROPERTY_CONTROL_NONE	= 0x00,

		_PROPERTY_CONTROL_SPLINE_PATH_NAME,

		_PROPERTY_CONTROL_CURRENT_INDEX,

		_PROPERTY_CONTROL_TIME,

		_PROPERTY_CONTROL_ID_MAX_COUNT
	};

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPLINE_PATH_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
