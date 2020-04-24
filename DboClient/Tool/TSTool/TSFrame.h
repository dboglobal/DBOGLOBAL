#pragma once


#include "TSProjectMainMenuBar.h"
#include "TSQuestTextBar.h"


//////////////////////////////////////////////////////////////////////////
//
//	CTSFrame
//
//////////////////////////////////////////////////////////////////////////


class CTSFrame : public CFrameWnd
{
protected:
	DECLARE_DYNCREATE( CTSFrame )
	DECLARE_MESSAGE_MAP()

// Declarations
public:
	enum
	{
		ePAN_INFO_TRIGGER_ID_STR,
		ePAN_INFO_TRIGGER_ID,
		ePAN_INFO_TRIGGER_TITLE_STR,
		ePAN_INFO_TRIGGER_TITLE,
		ePAN_INFO_TRIGGER_REPEAT_STR,
		ePAN_INFO_TRIGGER_REPEAT,
		ePAN_INFO_TRIGGER_SHARE_STR,
		ePAN_INFO_TRIGGER_SHARE,
	};

// Member variables
protected:
	CStatusBar							m_wndStatusBar;
	CToolBar							m_wndToolBar;
	CToolBar							m_wndContToolBar;
	CTSProjectMainMenuBar				m_wndProjMainMenuBar;
	CTSQuestTextBar						m_wndQuestTextBar;

// Constructions and Destructions
protected:
	CTSFrame( void );

public:
	virtual ~CTSFrame( void );

// Methods
public:
	virtual BOOL						PreCreateWindow( CREATESTRUCT& cs );

	void								UpdateStatusBar( CString& strTriggerID, CString& strTitle, CString& strRepeat, CString& strShare );

// Implementations
protected:
	afx_msg int							OnCreate( LPCREATESTRUCT lpCreateStruct );

	afx_msg void						OnViewProjectMainMenu();
	afx_msg void						OnUpdateViewProjectMainMenu(CCmdUI *pCmdUI);

	afx_msg void						OnViewQuestText();
	afx_msg void						OnUpdateViewQuestText(CCmdUI *pCmdUI);

#ifdef _DEBUG
	virtual void						AssertValid( void ) const;
	virtual void						Dump( CDumpContext& dc ) const;
#endif
};


