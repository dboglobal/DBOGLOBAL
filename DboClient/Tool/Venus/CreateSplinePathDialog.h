#pragma once


// CCreateSplinePathDialog 대화 상자입니다.
#include <string>
#include "PropTree.h"

#include "NtlEffectSplinePath.h"
#include "afxwin.h"

class CCreateSplinePathDialog : public CDialog
{
	DECLARE_DYNAMIC(CCreateSplinePathDialog)

public:
	CCreateSplinePathDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCreateSplinePathDialog();

	
public:
	enum TIME_TYPE
	{
		TIME_TYPE_RESET_PROPERTIES = 0x00,
	};

	enum _PROPERTY_CONTROL_ID
	{
		_PROPERTY_CONTROL_NONE	= 0x00,

		// EFFECT
		_PROPERTY_CONTROL_NAME,

		_PROPERTY_CONTROL_AXIS_TYPE,
		_PROPERTY_CONTROL_SPLINE_TYPE,
		_PROPERTY_CONTROL_CURVE_TYPE,

		_PROPERTY_CONTROL_TIME,

		_PROPERTY_CONTROL_CONTROL_COUNT,
		_PROPERTY_CONTROL_CURVE_SUBDIVISION_COUNT,

		_PROPERTY_CONTROL_FROWARD_X,
		_PROPERTY_CONTROL_FROWARD_Y,
		_PROPERTY_CONTROL_FROWARD_Z,

		_PROPERTY_CONTROL_POINT_SPEED,

		_PROPERTY_CONTROL_ROTATE_COUNT,
		_PROPERTY_CONTROL_RADIUS,

		_PROPERTY_CONTROL_ROTATE_DIRECTION,

		_PROPERTY_CONTROL_ID_MAX_COUNT
	};
public:
	BOOL						m_Initialized;
	CPropTree					m_PropertyTree;

	std::string					m_strName;
	CNtlEffectSplinePath::INFO	m_Info;

	int							m_nCurveType;
	int							m_nSplineType;

	CNtlEffectSplinePath*		m_pSplinePath;

	void	SetSplinePath(const RwChar* strName);
	void	SetSplinePath(CNtlEffectSplinePath* pSplinePath);

public:
	void	SetProperties();
	void	SetProperties_Line(CPropTreeItem* pRoot);
	void	SetProperties_Circle(CPropTreeItem* pRoot);
	void	SetProperties_Herix(CPropTreeItem* pRoot);
	void	SetProperties_SineCurve(CPropTreeItem* pRoot);

	void	Properties_PointSpeed(CPropTreeItem* pRoot);
	void	Properties_Forward(CPropTreeItem* pRoot);
	void	Properties_RotateCount(CPropTreeItem* pRoot);
	void	Properties_UseRotateDirection(CPropTreeItem* pRoot);
	void	Properties_Radius(CPropTreeItem* pRoot);


	void	ApplyProperties(CPropTreeItem* pItem);


// 대화 상자 데이터입니다.
	enum { IDD = IDD_CREATE_SPLINE_PATH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg void OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
};
