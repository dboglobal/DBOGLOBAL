/******************************************************************************
* File			: DirectionPropSound.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 8. 6
* Abstract		: 
*****************************************************************************
* Desc			: 사운드 연출의 속성창
*****************************************************************************/

#pragma once

struct _SWorldPickInfo;

class CNtlSLCENode;
class CNtlSLCENodeSound;

#include "CustomItems.h"

class CDirectionPropSound : public CWnd
{
	DECLARE_DYNAMIC(CDirectionPropSound)

public:
	CDirectionPropSound();
	virtual ~CDirectionPropSound();

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
	CXTPPropertyGridItemCategory*	m_pCategoryItem;

	// Direction type	
	CXTPPropertyGridItemEnum*		m_pDirectionTypeItemEnum;

	// Sound play
	CXTPPropertyGridItemEnum*		m_pChannelGroup;
	CXTPPropertyGridItem*			m_pSoundName;
	CXTPPropertyGridItemBool*		m_pGridItemLoop;

	// Offset
	CCustomItemButton*				m_pGridItemOffsetButton;
	CXTPPropertyGridItemDouble*		m_pGridItemOffsetX;
	CXTPPropertyGridItemDouble*		m_pGridItemOffsetY;
	CXTPPropertyGridItemDouble*		m_pGridItemOffsetZ;

	bool							m_bWaitOffsetClick;

	CNtlSLCENodeSound*				m_pCurSoundData;
};