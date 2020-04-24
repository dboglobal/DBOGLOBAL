#pragma once


// CAvatarPropertyDialog 대화 상자입니다.
#include "PropTree.h"
#include "MacSliderCtrl.h"

#include <rwcore.h>

class CAvatarPropertyDialog : public CDialog
{
	DECLARE_DYNCREATE(CAvatarPropertyDialog)

public:
	CAvatarPropertyDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAvatarPropertyDialog();

	void	ResetProperties(RwUInt32 nCurrentPoint = 0);
	void	ApplyProperties(CPropTreeItem* pItem);

	void	ResetAnimationTime(RwReal fCurrentTime, RwUInt32 nMaxTime);

	void	BuildData();

public:
	BOOL						m_Initialized;
	CPropTree					m_PropertyTree;

	RwUInt32					m_nCurrentPoint;


	enum TIME_TYPE
	{
		TIME_TYPE_RESET_PROPERTIES = 0x00,
		TIME_TYPE_EVENT_NAME,
	};

	enum _PROPERTY_CONTROL_ID
	{
		_PROPERTY_CONTROL_NONE	= 0x00,

		// EFFECT
		_PROPERTY_CONTROL_EVENT_NAME,

		_PROPERTY_CONTROL_CLUMP_NAME,
		_PROPERTY_CONTROL_ANIMATION_NAME,

		_PROPERTY_CONTROL_NUM_COUNT,

		_PROPERTY_CONTROL_CURRENT_INDEX,

		_PROPERTY_CONTROL_EFFECT_NAME,
		_PROPERTY_CONTROL_BONE_INDEX,

		_PROPERTY_CONTROL_FADE_IN_TIME,
		_PROPERTY_CONTROL_FADE_OUT_TIME,
		_PROPERTY_CONTROL_FADE_OUT_TYPE,

		_PROPERTY_CONTROL_ID_MAX_COUNT
	};

public:

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AVATAR_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult);
	CMacSliderCtrl m_AnimationSlider;
	CString m_strCurrentTime;
	CString m_strMaxTime;
	CString m_strBallSpeed;
	CString m_strTargetDistance;
	afx_msg void OnEnKillfocusBallLifetime();
	afx_msg void OnEnKillfocusBallSpeed();
	afx_msg void OnEnChangeBallLifetime();
	afx_msg void OnEnChangeBallSpeed();
};
