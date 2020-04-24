#pragma once


// CMeshLayer 뷰입니다.
#include "RollupCtrl.h"

class CMeshLayer : public CView
{
	DECLARE_DYNCREATE(CMeshLayer)

protected:
	CMeshLayer();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMeshLayer();

public:
	BOOL					m_bInitialized;
	CRollupCtrl				m_RollupControl;

	enum EDIT_DIALOG
	{
		EDIT_DIALOG_MESH_LIST = 0x00,
		EDIT_DIALOG_ANIMATION_LIST,
		EDIT_DIALOG_UV_ANIMATION_LIST,
		EDIT_DIALOG_PROPERTY,
	};

public:

	void	InitializeList();

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


