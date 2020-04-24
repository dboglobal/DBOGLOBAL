/******************************************************************************
* File			: DirectionProp3DBallon.h
* Author		: Hong SungBock
* Copyright		: (¡÷)NTL
* Date			: 2008. 8. 12
* Abstract		: 
*****************************************************************************
* Desc			: 3D ∏ª«≥º±
*****************************************************************************/

#pragma once

class CNtlSLCENode;
class CNtlSLCENodeEffect;

#include "CustomItems.h"

class CDirectionProp3DBallon : public CWnd
{
	DECLARE_DYNAMIC(CDirectionProp3DBallon)

public:
	CDirectionProp3DBallon();
	virtual ~CDirectionProp3DBallon();

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
	CXTPPropertyGrid				m_pGrid;
	CXTPPropertyGridItemCategory*	m_pGridCategory;

	// Direction type	
	CXTPPropertyGridItem*			m_pGridItemDirectionType;

	// Ballon Effect name
	CXTPPropertyGridItem*			m_pGridItemEffectName;

	// Off
	CCustomItemButton*				m_pGridItemOffsetButton;
	CXTPPropertyGridItemDouble*		m_pGridItemPositionX;
	CXTPPropertyGridItemDouble*		m_pGridItemPositionY;
	CXTPPropertyGridItemDouble*		m_pGridItemPositionZ;

	bool							m_bWaitOffsetClick;

	CNtlSLCENodeEffect*				m_pCurBallonEffectData;
};