#pragma once


// CEnemyPropertyDialog 대화 상자입니다.
#include "PropTree.h"

#include "rwcore.h"

class CEnemyPropertyDialog : public CDialog
{
	DECLARE_DYNCREATE(CEnemyPropertyDialog)

public:
	CEnemyPropertyDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEnemyPropertyDialog();

	void	ResetProperties(RwUInt32 nCurrentPoint = 0);
	void	ApplyProperties(CPropTreeItem* pItem);

public:
	BOOL						m_Initialized;
	CPropTree					m_PropertyTree;

	enum TIME_TYPE
	{
		TIME_TYPE_RESET_PROPERTIES = 0x00,
	};
	enum _PROPERTY_CONTROL_ID
	{
		_PROPERTY_CONTROL_NONE	= 0x00,

		_PROPERTY_CONTROL_CLUMP_NAME,
		_PROPERTY_CONTROL_IDLE_ANIMATION_NAME,
		_PROPERTY_CONTROL_DAMAGE_ANIMATION_NAME,

		_PROPERTY_CONTROL_EFFECT_NAME,

		_PROPERTY_CONTROL_ID_MAX_COUNT
	};
// 대화 상자 데이터입니다.
	enum { IDD = IDD_ENEMY_PROPERTY };

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
