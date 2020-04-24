#pragma once


// CEventLayer 뷰입니다.
#include "RollupCtrl.h"

class CEventLayer : public CView
{
	DECLARE_DYNCREATE(CEventLayer)

protected:
	CEventLayer();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CEventLayer();

public:
	BOOL					m_bInitialized;
	CRollupCtrl				m_RollupControl;

	enum EDIT_DIALOG
	{
		EDIT_DIALOG_PLAYER_EVENT_LIST = 0x00,
		EDIT_DIALOG_ENEMY_PROPERTY,
		EDIT_DIALOG_AVATAR_PROPERTY,
	};

public:
	void	ResetProperties();
	void	ResetPropertiesEnemy();
	void	ResetPropertiesAvatar();

	void	ResetTreeItemName(const RwChar* strName);
	void	ResetAnimationTime(RwReal fCurrentTime, RwUInt32 nMaxTime);

	void	BuildData();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


