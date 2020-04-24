#pragma once

#include "afxwin.h"
#include "CustomItems.h"

struct _SWorldPickInfo;

class CNtlSLCENode;
class CNtlSLCENodeReferee;

// CDirectionPropRefreeView 폼 뷰입니다.

class CDirectionPropRefreeView : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CDirectionPropRefreeView)

protected:
	CDirectionPropRefreeView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CDirectionPropRefreeView();

public:
	enum { IDD = IDD_DIRECTION_PROP_REFREE };

	enum eButtonInputType
	{
		BUTTON_INPUT_TYPE_NONE,
		BUTTON_INPUT_TYPE_POS,
		BUTTON_INPUT_TYPE_TARGET_POS,
	};

	struct sANIM_DATA
	{
		RwUInt32		uiAnimID;
		CString			strAnimName;
	};

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	CStatic							m_staticRefree;

	CXTPPropertyGrid				m_PropGrid;
	CXTPPropertyGridItemCategory*	m_pCategoryItem;

	// Direction type	
	CXTPPropertyGridItemEnum*		m_pDirectionTypeItemEnum;

	CXTPPropertyGridItemDouble*		m_pGridItemAngle;

	// Move
	CXTPPropertyGridItemEnum*		m_pGridItemMoveType;
	CXTPPropertyGridItemDouble*		m_pGridItemMoveSpeed;
	CCustomItemButton*				m_pGridItemPosButton;
	CXTPPropertyGridItemDouble*		m_pGridItemPosX;
	CXTPPropertyGridItemDouble*		m_pGridItemPosY;
	CXTPPropertyGridItemDouble*		m_pGridItemPosZ;
	CCustomItemButton*				m_pGridItemTargetButton;
	CXTPPropertyGridItemDouble*		m_pGridItemTargetX;
	CXTPPropertyGridItemDouble*		m_pGridItemTargetY;
	CXTPPropertyGridItemDouble*		m_pGridItemTargetZ;

	eButtonInputType				m_eButtonInputType;

	CNtlSLCENodeReferee*			m_pCurRefreeData;	

	CListBox						m_lcAnimationList;
	CListBox						m_lcSelectedAnimationList;
	CComboBox						m_ComboAnimPlayType;

	CXTEditListBox					m_ListBoxNarrationList;
	CComboBox						m_ComboBoxNarrationPlayType;

	int								m_iLastSelectedIndex_in_ListBoxNarrationList;

public:
	afx_msg void OnDestroy();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButton_Delete();
	afx_msg void OnBnClickedButton_Up();
	afx_msg void OnBnClickedButton_Down();
	afx_msg void OnLbnDblclkList_AnimList();
	afx_msg void OnCbnSelchangeCombo1();

	void	OnListBoxNewItem_NarrationList();	
	void	OnListBoxDeleteItem_NarrationList();
	void	OnSelchangeEditList_Narration();

	afx_msg void OnLbnSelchangeList_Narration();
	afx_msg void OnBnClickedButton_NarrationDelete();
	afx_msg void OnBnClickedButton_NarrationUp();
	afx_msg void OnBnClickedButton_NarrationDown();
	afx_msg void OnCbnSelchangeCombo_NarrationPlayType();

	LRESULT OnGridNotify(WPARAM wParam, LPARAM lParam);

	void	SetProperty(CNtlSLCENode* pNode);
	void	SetPickInfo(_SWorldPickInfo* pPickInfo);

	void	RefreshNarrationList();

protected:
	void	SetAnimIDEnum();
	void	DestroyListItem();
};