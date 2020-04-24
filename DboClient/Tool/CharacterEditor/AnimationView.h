#pragma once


// CAnimationView 뷰입니다.
#include "AnimationTreeDialog.h"
#include "AnimationSetDialog.h"

class CAnimationView : public CView
{
	DECLARE_DYNCREATE(CAnimationView)

protected:
	CAnimationView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CAnimationView();

public:
	CAnimationTreeDialog	m_AnimationTreeDialog;
	CAnimationSetDialog	    m_AnimationSetDialog;
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
};


