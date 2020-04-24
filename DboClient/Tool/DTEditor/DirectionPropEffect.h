/******************************************************************************
* File			: DirectionPropEffecct.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 8. 6
* Abstract		: 
*****************************************************************************
* Desc			: 이펙트 연출의 속성창
*****************************************************************************/

#pragma once


class CNtlSLCENode;
class CNtlSLCENodeEffect;

class CDirectionPropEffect : public CWnd
{
	DECLARE_DYNAMIC(CDirectionPropEffect)

public:
	CDirectionPropEffect();
	virtual ~CDirectionPropEffect();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	LRESULT OnGridNotify(WPARAM wParam, LPARAM lParam);

	void				SetProperty(CNtlSLCENode* pNode);

protected:
	CXTPPropertyGrid				m_PropGrid;
	CXTPPropertyGridItemCategory*	m_pCategoryItem;

	// Direction type	
	CXTPPropertyGridItemEnum*		m_pDirectionTypeItemEnum;

	// Effect name
	CXTPPropertyGridItem*			m_pEffectName;


	CNtlSLCENodeEffect*				m_pCurEffectData;
};