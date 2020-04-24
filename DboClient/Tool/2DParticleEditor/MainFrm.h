// MainFrm.h : interface of the CMainFrame class
//

#pragma once

// Panes
#include "SystemPane.h"
#include "InfomationPane.h"
#include "AttributePane.h"
#include "PropertiesPane.h"

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

public:

protected:  // control bar embedded members
	CXTPStatusBar			m_wndStatusBar;
	CXTPDockingPaneManager	m_paneManager;

	CXTPDockingPane*		m_pWndPaneInfomation;
	CXTPDockingPane*		m_pWndPaneSystem;
	CXTPDockingPane*		m_pWndPaneAttribute;
	CXTPDockingPane*		m_pWndPaneProperties;

	CAttributePane			m_AttrPane;
	CPropertiesPane			m_PropertisPane;

	CXTPMenuBar*			m_pMenuBar;
	CXTPToolBar*			m_pToolBar;
	/*CXTPToolBar*			m_pOptionTool;*/

public:
	CPropertiesPane*		GetPropertiesPane()			{ return &m_PropertisPane; }
	CAttributePane*			GetAttributePane()			{ return &m_AttrPane; }

	DECLARE_MESSAGE_MAP()

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
};
