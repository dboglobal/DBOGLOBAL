// VenusFrame.h : CVenusFrame 클래스의 인터페이스
//


#pragma once

#include "NtlDebug.h"

#include "SizingTabCtrlBar.h"

#include "VenusConfig.h"
#include "EditDialog.h"

#include "VenusPlayerEventManager.h"

#include "TrueColorToolBar.h"

#define TOOLBAR_DRAW_BUTTON_WIDTH 24

class CVenusFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CVenusFrame();
	DECLARE_DYNCREATE(CVenusFrame)

// 특성
public:
	CSizingTabCtrlBar		m_wndEditLayer;

	CReBar					m_wndStatusReBar;

	CTrueColorToolBar		m_ToolBar;

// 작업
public:
	void	InitializeEditLayer();
	BOOL	InitializeStatusbar();
	BOOL	InitializeToolbar();

// 재정의
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현
public:
	virtual ~CVenusFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CStatusBar  m_wndStatusBar;

// 메시지 맵 함수를 생성했습니다.
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	virtual void ActivateFrame(int nCmdShow = -1);
	afx_msg void OnClose();
	afx_msg void OnViewEffectList();
	afx_msg void OnUpdateViewEffectList(CCmdUI *pCmdUI);
//	afx_msg void OnFileNew();
//	afx_msg void OnFileOpen();
//	afx_msg void OnFileSave();
//	afx_msg void OnFileSaveAs();
	afx_msg void OnViewOption();
	afx_msg void OnEditModeMove();
	afx_msg void OnEditModeRotate();
	afx_msg void OnEditModeScale();
	afx_msg void OnUpdateEditModeMove(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditModeRotate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditModeScale(CCmdUI *pCmdUI);
	afx_msg void OnViewEditDialog();
	afx_msg void OnFileEffectNew();
	afx_msg void OnFileEffectOpen();
	afx_msg void OnFileEffectSave();
	afx_msg void OnFileEffectSaveAs();
	afx_msg void OnViewUpdateNews();
	afx_msg void OnUpdateViewUpdateNews(CCmdUI *pCmdUI);
	afx_msg void OnViewLight();
	afx_msg void OnFileEffectMerge();
	afx_msg void OnViewPostEffect();
    afx_msg void OnViewBlurfilter();
    afx_msg void OnUpdateViewBlurfilter(CCmdUI *pCmdUI);
    afx_msg void OnViewFramefix();
    afx_msg void OnUpdateViewFramefix(CCmdUI *pCmdUI);
    afx_msg void OnViewBoundingSphere();
    afx_msg void OnUpdateViewBoundingSphere(CCmdUI *pCmdUI);
    afx_msg void OnToolGenarateBoundingSphereAll();

};

inline CView* gGetEditLayer(int index)
{
	NTL_ASSERTE(index < EDIT_LAYER_MAX_COUNT);

	CVenusFrame* pMainfrm = (CVenusFrame*)AfxGetMainWnd();
	NTL_ASSERTE(pMainfrm);

	CView* pView = pMainfrm->m_wndEditLayer.GetView(index);
	NTL_ASSERTE(pView);

	return pView;
}

inline void gSetEditLayer(int layer)
{
	int nOldLayer = CVenusConfig::GetInstance().m_nCurrentLayer;
	CVenusConfig::GetInstance().m_nCurrentLayer = layer;

	if (nOldLayer == EDIT_LAYER_PATH || layer == EDIT_LAYER_PATH)
	{
		CEditDialog::GetInstance().Reset();
	}
	if (nOldLayer == EDIT_LAYER_EVENT && layer != EDIT_LAYER_EVENT)
	{
		CVenusPlayerEventManager::GetInstance().DeleteEffectEntity();
	}
}



