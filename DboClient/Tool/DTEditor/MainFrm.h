// MainFrm.h : interface of the CMainFrame class
//

#pragma once

// core
#include "NtlMath.h"

#include "ObjectPane.h"
#include "WayPointPane.h"
#include "CinematicNodePane.h"
#include "SkillPane.h"
#include "BotPane.h"
#include "PropertyPane.h"
#include "ScriptPane.h"
#include "ControlPane.h"
#include "ActorPane.h"


class CNtlSLCENode;
class CNtlSLTBCrowdController;

class CDirectionPane;
class CPropertyPane;
class CDirectionPropController;
class CDirectionPropEffect;
class CDirectionPropSound;
class CDirectionPropRefreeView;


// class CButtonOptions is used to create the
// radio buttons on the Options toolbar.
class CButtonOptions : public CXTButton
{
protected:
	void OnOptionShow();

protected:
	DECLARE_MESSAGE_MAP()
};


class CMainFrame : public CXTPFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members

	CString			m_strStylesPath;

	CXTPStatusBar  m_wndStatusBar;
	CXTPDockingPaneManager m_paneManager;
	CXTPDockingPane* m_pwndPaneControl;
	CXTPDockingPane* m_pwndPaneSpline;
	CXTPDockingPane* m_pwndPaneSkill;
	CXTPDockingPane* m_pwndPaneBot;
	CXTPDockingPane* m_pwndPaneDirection;
	CXTPDockingPane* m_pwndPaneProperties;
	CXTPDockingPane* m_pwndPaneActor;
	CXTPDockingPane* m_pwndPaneScript;

	CObjectPane					m_ObjectPane;
	CSkillPane					m_SkillPane;
	CBotPane					m_BotPane;
	CDirectionPane*				m_pDirectionPane;	
	CActorPane					m_ActorPane;	
	CControlPane				m_ControlPane;
	CScriptPane					m_ScriptPane;


	// properties
	CPropertyPane				m_PropertyPane;	

	CDirectionPropController*	m_pDirectionController;
	CDirectionPropEffect*		m_pDirectionPropEffect;
	CDirectionPropSound*		m_pDirectionPropSound;
	CDirectionPropRefreeView*	m_pDirectionPropRefree;

	
	CXTPMenuBar		*m_pMenuBar;
	CXTPToolBar		*m_pEditMode;

	CComboBox		m_comboEditMode;

public:

	CObjectPane*				GetObjectPane()			{ return &m_ObjectPane; }
	CSkillPane*					GetSkillPane()			{ return &m_SkillPane; }
	CBotPane*					GetBotPane()			{ return &m_BotPane; }
	CDirectionPane*				GetDirectionPane()		{ return m_pDirectionPane; }
	CPropertyPane*				GetPropertyPane()		{ return &m_PropertyPane; }
	CActorPane*					GetActorPane()			{ return &m_ActorPane; }
	CControlPane*				GetControlPane()		{ return &m_ControlPane; }
	CScriptPane*				GetScriptPane()			{ return &m_ScriptPane; }
	CDirectionPropRefreeView*	GetDirectionRefreePane() { return m_pDirectionPropRefree; }
	CDirectionPropSound*		GetDirectionSoundPane() { return m_pDirectionPropSound; }
	
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl); // Called by the XTK to create the controls on the CommandBars
	afx_msg void OnClose();
	afx_msg void OnCustomize();
	afx_msg void OnSelchangeEditMode();	

	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSplinemarkControlline();
public:
	afx_msg void OnSplinemarkCurveline();
public:
	afx_msg void OnSplinemarkCurverotateline();
public:
	afx_msg void OnUpdateSplinemarkControlline(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateSplinemarkCurveline(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateSplinemarkCurverotateline(CCmdUI *pCmdUI);
public:
	afx_msg void OnRenderviewWorldrenderOnoff();
public:
	afx_msg void OnUpdateRenderviewWorldrenderOnoff(CCmdUI *pCmdUI);
public:
	afx_msg void OnWorldChange();
	afx_msg void OnAvatrRaceClassGender();
	afx_msg void OnAvatarTeleport();
	afx_msg void OnAvatarSpeed();
	afx_msg void OnOptionSound();
	afx_msg void OnSplinemarkCurveCustomdataVisible();
	afx_msg void OnUpdateSplinemarkCurveCustomdataVisible(CCmdUI *pCmdUI);

public:
	void				SetDirectionProperty(CNtlSLTBCrowdController* pController);
	void				SetDirectionProperty(BYTE byNodeType, CNtlSLCENode* pNode);	
public:	
};