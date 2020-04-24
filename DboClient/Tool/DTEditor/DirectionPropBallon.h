/******************************************************************************
* File			: DirectionPropBallon.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 8. 6
* Abstract		: 
*****************************************************************************
* Desc			: 말풍선 연출의 속성창
*****************************************************************************/

#pragma once

class CNtlSLCENode;
class CNtlSLCENodeBalloon;
struct _SWorldPickInfo;

#include "CustomItems.h"

class CDirectionPropBallon : public CWnd
{
	DECLARE_DYNAMIC(CDirectionPropBallon)

	enum ePositionModifyMode
	{
		PM_START,
		PM_END,

		PM_INVALID,
	};

public:
	CDirectionPropBallon();
	virtual ~CDirectionPropBallon();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	LRESULT OnGridNotify(WPARAM wParam, LPARAM lParam);

	void				SetProperty(CNtlSLCENode* pNode);
	void				SetPickInfo(_SWorldPickInfo* pPickInfo);

protected:
	CXTPPropertyGrid				m_PropGrid;
	CXTPPropertyGridItem*			m_pCategoryItem;

	// Direction type	
	CXTPPropertyGridItemEnum*		m_pDirectionTypeItemEnum;

	// Ballon text table
	CXTPPropertyGridItemNumber*		m_pGridItemTableIndex;

	// Time
	CXTPPropertyGridItemNumber*		m_pGridItemInstanceTime;
	CXTPPropertyGridItemNumber*		m_pGridItemLifeTime;

	// Absoluteness Position
	CCustomItemButton*				m_pGridItemStartButton;
	CXTPPropertyGridItemNumber*		m_pGridItemPositionStartX;
	CXTPPropertyGridItemNumber*		m_pGridItemPositionStartY;
	CXTPPropertyGridItemNumber*		m_pGridItemPositionStartZ;
	CCustomItemButton*				m_pGridItemEndButton;
	CXTPPropertyGridItemNumber*		m_pGridItemPositionEndX;
	CXTPPropertyGridItemNumber*		m_pGridItemPositionEndY;
	CXTPPropertyGridItemNumber*		m_pGridItemPositionEndZ;

	CNtlSLCENodeBalloon*			m_pCurBallonData;

	ePositionModifyMode				m_ePositionModifyMode;	
};